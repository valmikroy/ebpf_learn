asmlinkage __visible void __softirq_entry __do_softirq(void)
{
        unsigned long end = jiffies + MAX_SOFTIRQ_TIME;
        unsigned long old_flags = current->flags;
        int max_restart = MAX_SOFTIRQ_RESTART;
        struct softirq_action *h;
        bool in_hardirq;
        __u32 pending;
        int softirq_bit;

        /*
         * Mask out PF_MEMALLOC s current task context is borrowed for the
         * softirq. A softirq handled such as network RX might set PF_MEMALLOC
         * again if the socket is related to swap
         */
        current->flags &= ~PF_MEMALLOC;

        pending = local_softirq_pending();
        account_irq_enter_time(current);

        __local_bh_disable_ip(_RET_IP_, SOFTIRQ_OFFSET);
        in_hardirq = lockdep_softirq_start();
restart:
        /* Reset the pending bitmask before enabling irqs */
        set_softirq_pending(0);

        local_irq_enable();

        /*
                h = softirq_vec;
                where
                static struct softirq_action softirq_vec[NR_SOFTIRQS] __cacheline_aligned_in_smp;

                and

                struct softirq_action
                {
                        void    (*action)(struct softirq_action *);
                };


        */

        h = softirq_vec;
                while ((softirq_bit = ffs(pending))) {
                unsigned int vec_nr;
                int prev_count;

                h += softirq_bit - 1;

                vec_nr = h - softirq_vec;
                prev_count = preempt_count();

                kstat_incr_softirqs_this_cpu(vec_nr);

                /** XXX
                This time between tacepoint entry and exit below , is amount of time spent in softirq
                **/
                trace_softirq_entry(vec_nr);
                h->action(h);
                trace_softirq_exit(vec_nr);
                if (unlikely(prev_count != preempt_count())) {
                        pr_err("huh, entered softirq %u %s %p with preempt_count %08x, exited with %08x?\n",
                               vec_nr, softirq_to_name[vec_nr], h->action,
                               prev_count, preempt_count());
                        preempt_count_set(prev_count);
                }
                h++;
                pending >>= softirq_bit;
        }

        rcu_bh_qs();
        local_irq_disable();

        pending = local_softirq_pending();
        if (pending) {
                if (time_before(jiffies, end) && !need_resched() &&
                    --max_restart)
                        goto restart;

                wakeup_softirqd();
        }

        lockdep_softirq_end(in_hardirq);
        account_irq_exit_time(current);
        __local_bh_enable(SOFTIRQ_OFFSET);
        WARN_ON_ONCE(in_interrupt());
        current_restore_flags(old_flags, PF_MEMALLOC);
}

asmlinkage __visible void do_softirq(void)
{
        __u32 pending;
        unsigned long flags;

        if (in_interrupt())
                return;

        local_irq_save(flags);

        pending = local_softirq_pending();

        if (pending && !ksoftirqd_running())
                do_softirq_own_stack();

        local_irq_restore(flags);
}
