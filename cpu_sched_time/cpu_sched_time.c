#include <uapi/linux/ptrace.h>
#include <linux/sched.h>


// Type which holds value for histogram 
// eventually exported in python space
typedef struct pid_key {
    u64 id;
    u64 slot;
} pid_key_t;
BPF_HISTOGRAM(dist, pid_key_t);



// hash to store timestamp when pid got task switched
BPF_HASH(start, u32, u64);


// initiate bpf hash
static inline void store_start(u32 tgid, u32 pid, u64 ts)
{
    if (tgid != --PID--)
        return;
    start.update(&pid, &ts);
}



// Update histogram value
static inline void update_hist(u32 tgid, u32 pid, u64 ts)
{
    if (tgid != --PID--)
        return;
    u64 *tsp = start.lookup(&pid);
    if (tsp == 0)
        return;
    if (ts < *tsp) {
        // Probably a clock issue where the recorded on-CPU event had a
        // timestamp later than the recorded off-CPU event, or vice versa.
        return;
    }
    u64 delta = ts - *tsp;
    //FACTOR
    delta /= 1000;

   //STORE
    pid_key_t key = {
        .id = --PID--,
        .slot = bpf_log2l(delta)
    };
    dist.increment(key);
}


// This is main flow 

int sched_switch(struct pt_regs *ctx, struct task_struct *prev)
{

    // Fetch timestamp when finish_task_switch() happens
    u64 ts = bpf_ktime_get_ns();

    // get pid and tid of task
    u64 pid_tgid = bpf_get_current_pid_tgid();
    u32 tgid = pid_tgid >> 32, pid = pid_tgid;

    // Store start timestamp for above pid at BAIL 
    



    if (prev->state == TASK_RUNNING) {
        u32 prev_pid = prev->pid;
        u32 prev_tgid = prev->tgid;

        // retirve start timestamp from BPF_HASH for previous pid
        // current timestamp aka finish timestamp for previous pid
        // difference would be amount of CPU time given pid got
        update_hist(prev_tgid, prev_pid, ts);
    }


BAIL:
    store_start(tgid, pid, ts);
    return 0;

}
