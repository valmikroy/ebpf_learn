
//  /sys/kernel/debug/tracing/events/irq/softirq_entry/format

#include <uapi/linux/ptrace.h>
#include <uapi/linux/bpf_perf_event.h>




struct irq_key_t {
    u32 vec;
    int cpu;
    int pid;
    char name[TASK_COMM_LEN];
};



BPF_HASH(softirq_count, struct irq_key_t, u64);
BPF_HASH(softirq_time, struct irq_key_t, u64);


BPF_HASH(tracker, struct irq_key_t, u64);




static inline __attribute__((always_inline)) void get_key(struct irq_key_t* key) {
    key->pid = bpf_get_current_pid_tgid();
    key->cpu = bpf_get_smp_processor_id();
    bpf_get_current_comm(&(key->name), sizeof(key->name));

}



TRACEPOINT_PROBE(irq, softirq_entry)
{

    struct irq_key_t key = {};
    get_key(&key);

    // net rx or tx only
    if ( args->vec != 2 || args->vec != 3 )
  	return 0;

    u64 ts = bpf_ktime_get_ns();

   /**
    *   identifier for softirq is args->vec
    *   vec values
    *   0 - HI
    *   1 - TIMER
    *   2 - net_tx
    *   3 - net_rx
    *   4 - block
    *   5 - irq_poll
    *   6 - tasklet
    *   7 - sched
    *   8 - hrtimer
    *   9 - RCU
    *
    **/

    key.vec = args->vec;
    tracker.update(&key, &ts);
    return 0;
}

TRACEPOINT_PROBE(irq, softirq_exit)
{
    u64 delta, *ts;
    u64 *irq_cnt, *irq_time, zero = 0;
    struct irq_key_t key = {};



    get_key(&key);
    key.vec = args->vec;
    ts = tracker.lookup(&key);

    if (ts == 0) {
        return 0;   // missed start
    }

    delta = bpf_ktime_get_ns() - *ts;

    irq_cnt = softirq_count.lookup_or_init(&key,&zero);
    (*irq_cnt) += 1;


    irq_time = softirq_time.lookup_or_init(&key,&zero);
    (*irq_time) += delta;


    tracker.delete(&key);
    return 0;
}

