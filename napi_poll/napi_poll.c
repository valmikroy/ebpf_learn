#include <uapi/linux/ptrace.h>
#include <uapi/linux/bpf_perf_event.h>




struct key_t {
    char dev[64];
    int cpu;
    int pid;
    char name[TASK_COMM_LEN];
};



BPF_HASH(napi_poll_count, struct key_t, u64);
BPF_HASH(napi_poll_full, struct key_t, u64);






static inline __attribute__((always_inline)) void get_key(struct key_t* key) {
    key->pid = bpf_get_current_pid_tgid();
    key->cpu = bpf_get_smp_processor_id();
    bpf_get_current_comm(&(key->name), sizeof(key->name));
}



TRACEPOINT_PROBE(napi, napi_poll)
{

    u64 *val_count, *val_full, zero = 0;
    struct key_t key = {};
    char iface[64];


    get_key(&key);

    TP_DATA_LOC_READ_CONST(iface, dev_name, 64);
    __builtin_memcpy(&(key.dev), iface, 64);

    val_count = napi_poll_count.lookup_or_init(&key,&zero);
    (*val_count) += 1;

    if ( args->work == args->budget ) {
        val_full = napi_poll_full.lookup_or_init(&key,&zero);
        (*val_full) += 1;
    }

    return 0;
}
