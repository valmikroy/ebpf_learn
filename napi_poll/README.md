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


```
valmikroy@blah:~/git/ebpf_learn/napi_poll$ sudo python  napi_poll.py
timestamp=1531107011.124227 type=napi iface=enp2s0f0 cpu=28 pid=1232 budget=64 proc_name=irq/82-enp2s0f0 poll_count=2 full_count=0
timestamp=1531107011.124227 type=napi iface=enp2s0f0 cpu=29 pid=1230 budget=64 proc_name=irq/80-enp2s0f0 poll_count=2 full_count=0
timestamp=1531107011.124227 type=napi iface=enp2s0f1 cpu=8 pid=1506 budget=64 proc_name=irq/149-enp2s0f poll_count=1 full_count=0
timestamp=1531107011.124227 type=napi iface=enp2s0f1 cpu=20 pid=1489 budget=64 proc_name=irq/132-enp2s0f poll_count=4 full_count=0
timestamp=1531107011.124227 type=napi iface=enp2s0f1 cpu=28 pid=1494 budget=64 proc_name=irq/137-enp2s0f poll_count=1 full_count=0
timestamp=1531107011.124227 type=napi iface=enp2s0f0 cpu=1 pid=1249 budget=64 proc_name=irq/98-enp2s0f0 poll_count=2 full_count=0
timestamp=1531107011.124227 type=napi iface=enp2s0f1 cpu=22 pid=1505 budget=64 proc_name=irq/148-enp2s0f poll_count=3 full_count=0
timestamp=1531107011.226516 type=napi iface=enp2s0f0 cpu=23 pid=1255 budget=64 proc_name=irq/104-enp2s0f poll_count=2 full_count=0
timestamp=1531107011.226516 type=napi iface=enp2s0f0 cpu=20 pid=1229 budget=64 proc_name=irq/79-enp2s0f0 poll_count=1 full_count=0
timestamp=1531107011.226516 type=napi iface=enp2s0f0 cpu=29 pid=1230 budget=64 proc_name=irq/80-enp2s0f0 poll_count=3 full_count=0
timestamp=1531107011.226516 type=napi iface=enp2s0f0 cpu=23 pid=1243 budget=64 proc_name=irq/92-enp2s0f0 poll_count=2 full_count=0
timestamp=1531107011.226516 type=napi iface=enp2s0f1 cpu=8 pid=1506 budget=64 proc_name=irq/149-enp2s0f poll_count=1 full_count=0
timestamp=1531107011.226516 type=napi iface=enp2s0f0 cpu=20 pid=1259 budget=64 proc_name=irq/108-enp2s0f poll_count=2 full_count=0
timestamp=1531107011.226516 type=napi iface=enp2s0f1 cpu=28 pid=1494 budget=64 proc_name=irq/137-enp2s0f poll_count=2 full_count=0
timestamp=1531107011.226516 type=napi iface=enp2s0f1 cpu=20 pid=1511 budget=64 proc_name=irq/154-enp2s0f poll_count=2 full_count=0
timestamp=1531107011.226516 type=napi iface=enp2s0f0 cpu=20 pid=133 budget=64 proc_name=ksoftirqd/20 poll_count=1 full_count=0
timestamp=1531107011.226516 type=napi iface=enp2s0f1 cpu=22 pid=1486 budget=64 proc_name=irq/129-enp2s0f poll_count=3 full_count=0
```
