NAPI poll filter
```
valmikroy@blah:~/git/ebpf_learn/napi_poll$ sudo cat  /sys/kernel/debug/tracing/events/napi/napi_poll/format
name: napi_poll
ID: 1233
format:
        field:unsigned short common_type;       offset:0;       size:2; signed:0;
        field:unsigned char common_flags;       offset:2;       size:1; signed:0;
        field:unsigned char common_preempt_count;       offset:3;       size:1; signed:0;
        field:int common_pid;   offset:4;       size:4; signed:1;

        field:struct napi_struct * napi;        offset:8;       size:8; signed:0;
        field:__data_loc char[] dev_name;       offset:16;      size:4; signed:1;
        field:int work; offset:20;      size:4; signed:1;
        field:int budget;       offset:24;      size:4; signed:1;

print fmt: "napi poll on napi struct %p for device %s work %d budget %d", REC->napi, __get_str(dev_name), REC->work, REC->budget
```

napi_poll returns amount of work has done which equal to weight if max work is done. In our script we are counting occurance of napi_poll as napi_count and occurance of max work done as napi_full. ratio of this should give us some feedback on various kernel params to tune.

_NOTE_ I was suspecting that napi_poll would have two budget values, one from device driver's budget for NIC and another softnet data structure backlog processing budget setup by sysctl, though I could not single value of 64 as budget all the time.
