#include <uapi/linux/ptrace.h>


typedef struct irq_key {
    u32 vec;
    u64 slot;
} irq_key_t;

typedef struct account_val {
    u64 ts;
    u32 vec;
} account_val_t;

BPF_HASH(start, u32, account_val_t);
BPF_HASH(iptr, u32);
BPF_HISTOGRAM(dist, irq_key_t);

TRACEPOINT_PROBE(irq, softirq_entry)
{

    u32 pid = bpf_get_current_pid_tgid();
    account_val_t val = {};

   /**
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

    // net_rx only
    if ( args->vec != 3 )
        return 0;

    val.ts = bpf_ktime_get_ns();
    // identifier for softirq is args->vec
    val.vec = args->vec;
    start.update(&pid, &val);
    return 0;
}

TRACEPOINT_PROBE(irq, softirq_exit)
{
    u64 delta;
    u32 vec;
    u32 pid = bpf_get_current_pid_tgid();
    account_val_t *valp;
    irq_key_t key = {0};

    // fetch timestamp and calculate delta
    valp = start.lookup(&pid);
    if (valp == 0) {
        return 0;   // missed start
    }

    // Kernel time is in ns
    delta = bpf_ktime_get_ns() - valp->ts;
    vec = valp->vec;

    key.vec = vec;
    key.slot = bpf_log2l(delta /1000 );

    dist.increment(key);
    start.delete(&pid);
    return 0;
}
