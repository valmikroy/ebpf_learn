#include <linux/ptrace.h>
#include <uapi/linux/bpf_perf_event.h>

struct key_t {
    int cpu;
    int pid;
    char name[TASK_COMM_LEN];
};

BPF_HASH(ref_count, struct key_t);
BPF_HASH(miss_count, struct key_t);

static inline __attribute__((always_inline)) void get_key(struct key_t* key) {
    key->pid = bpf_get_current_pid_tgid();

    if (key->pid == --PID-- ){
            key->cpu = bpf_get_smp_processor_id();
            bpf_get_current_comm(&(key->name), sizeof(key->name));
    }
    else {
        key->cpu = 100;
    }

}

int on_cache_miss(struct bpf_perf_event_data *ctx) {
    struct key_t key = {};
    get_key(&key);

    if ( key.cpu == 100 )
        return 0;

    u64 zero = 0, *val;
    val = miss_count.lookup_or_init(&key, &zero);
    (*val) += ctx->sample_period;

    return 0;
}

int on_cache_ref(struct bpf_perf_event_data *ctx) {
    struct key_t key = {};
    get_key(&key);
    if ( key.cpu == 100 )
        return 0;

    u64 zero = 0, *val;
    val = ref_count.lookup_or_init(&key, &zero);
    (*val) += ctx->sample_period;

    return 0;
}
