Measures time spent in softirq context with help of tracepoint `softirq_entry` and `softirq_exit` which allows you to keep track by simple subtraction of timestaps measured during entry and exit of softirq.

- `softirq_time_hist.py` will provide histogram of timings of net_rx softirq in csv format.
```
valmikroy@blah:~/git/ebpf_learn/softirq_timing$ sudo python softirq_time_hist.py
Tracing RX softirq time... Hit Ctrl-C to end.
ts,type,0ms,1ms,2ms,3ms,4ms,5ms,6ms,7ms,8ms,9ms,10ms,11ms,12ms,13ms,14ms,15ms,16ms,17ms,18ms,19ms,20ms,21ms,22ms,23ms,24ms,25ms,26ms,27ms,28ms,29ms
1531073064,net_rx_1531073063,0,0,1,5,5,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
1531073065,net_rx_1531073063,0,0,30,39,1,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
1531073066,net_rx_1531073063,0,0,0,5,5,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
1531073067,net_rx_1531073063,0,0,30,56,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
^C1531073068,net_rx_1531073063,0,0,0,0,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
```
- `softirq_time.py` is expanded version which takes input of which type of irq context to measure from following 
```
       Numeric identifier for softirq

       0 - HI
       1 - TIMER
       2 - net_tx
       3 - net_rx
       4 - block
       5 - irq_poll
       6 - tasklet
       7 - sched
       8 - hrtimer
       9 - RCU
```
this provides output in key value pair for easier parsing

```
valmikroy@blah:~/git/ebpf_learn/softirq_timing$ sudo python softirq_time.py -t 3
timestamp=1531073309.946055 type=softirq softirq_type=3 pid=1578 proc_name=irq/120-enp2s0f cpu=6 softirq_time=13127 softirq_count=1
timestamp=1531073310.048327 type=softirq softirq_type=3 pid=1353 proc_name=irq/74-enp2s0f0 cpu=9 softirq_time=17546 softirq_count=1
timestamp=1531073310.048327 type=softirq softirq_type=3 pid=1349 proc_name=irq/68-enp2s0f0 cpu=8 softirq_time=16354 softirq_count=1
timestamp=1531073310.149772 type=softirq softirq_type=3 pid=1353 proc_name=irq/74-enp2s0f0 cpu=9 softirq_time=29291 softirq_count=2
timestamp=1531073310.250869 type=softirq softirq_type=3 pid=1353 proc_name=irq/74-enp2s0f0 cpu=9 softirq_time=20847 softirq_count=1
timestamp=1531073310.250869 type=softirq softirq_type=3 pid=1578 proc_name=irq/120-enp2s0f cpu=6 softirq_time=11490 softirq_count=1
timestamp=1531073310.352226 type=softirq softirq_type=3 pid=1353 proc_name=irq/74-enp2s0f0 cpu=9 softirq_time=22170 softirq_count=1
timestamp=1531073310.453443 type=softirq softirq_type=3 pid=1364 proc_name=irq/84-enp2s0f0 cpu=20 softirq_time=22404 softirq_count=1
timestamp=1531073310.554608 type=softirq softirq_type=3 pid=1364 proc_name=irq/84-enp2s0f0 cpu=20 softirq_time=23501 softirq_count=1
timestamp=1531073310.655991 type=softirq softirq_type=3 pid=1364 proc_name=irq/84-enp2s0f0 cpu=20 softirq_time=21780 softirq_count=1
timestamp=1531073310.757208 type=softirq softirq_type=3 pid=1364 proc_name=irq/84-enp2s0f0 cpu=20 softirq_time=21767 softirq_count=1
timestamp=1531073310.858350 type=softirq softirq_type=3 pid=1370 proc_name=irq/89-enp2s0f0 cpu=27 softirq_time=4230 softirq_count=1
timestamp=1531073310.858350 type=softirq softirq_type=3 pid=1355 proc_name=irq/76-enp2s0f0 cpu=20 softirq_time=4113 softirq_count=1
timestamp=1531073310.858350 type=softirq softirq_type=3 pid=1358 proc_name=irq/78-enp2s0f0 cpu=29 softirq_time=4770 softirq_count=1
timestamp=1531073310.858350 type=softirq softirq_type=3 pid=1376 proc_name=irq/95-enp2s0f0 cpu=20 softirq_time=4330 softirq_count=1
timestamp=1531073310.858350 type=softirq softirq_type=3 pid=1353 proc_name=irq/74-enp2s0f0 cpu=9 softirq_time=4786 softirq_count=1
timestamp=1531073310.858350 type=softirq softirq_type=3 pid=1350 proc_name=irq/69-enp2s0f0 cpu=2 softirq_time=8484 softirq_count=1
timestamp=1531073310.858350 type=softirq softirq_type=3 pid=1354 proc_name=irq/75-enp2s0f0 cpu=20 softirq_time=4070 softirq_count=1
timestamp=1531073310.858350 type=softirq softirq_type=3 pid=1372 proc_name=irq/91-enp2s0f0 cpu=20 softirq_time=4190 softirq_count=1
timestamp=1531073310.858350 type=softirq softirq_type=3 pid=1379 proc_name=irq/98-enp2s0f0 cpu=29 softirq_time=4833 softirq_count=1
timestamp=1531073310.858350 type=softirq softirq_type=3 pid=1366 proc_name=irq/86-enp2s0f0 cpu=29 softirq_time=5323 softirq_count=1
timestamp=1531073310.858350 type=softirq softirq_type=3 pid=1363 proc_name=irq/83-enp2s0f0 cpu=27 softirq_time=4164 softirq_count=1
timestamp=1531073310.858350 type=softirq softirq_type=3 pid=1383 proc_name=irq/102-enp2s0f cpu=3 softirq_time=7403 softirq_count=1
timestamp=1531073310.858350 type=softirq softirq_type=3 pid=1351 proc_name=irq/70-enp2s0f0 cpu=20 softirq_time=4050 softirq_count=1
timestamp=1531073310.858350 type=softirq softirq_type=3 pid=1381 proc_name=irq/100-enp2s0f cpu=26 softirq_time=5684 softirq_count=1
timestamp=1531073310.858350 type=softirq softirq_type=3 pid=1384 proc_name=irq/103-enp2s0f cpu=29 softirq_time=8026 softirq_count=1
timestamp=1531073310.858350 type=softirq softirq_type=3 pid=1387 proc_name=irq/106-enp2s0f cpu=20 softirq_time=4197 softirq_count=1
timestamp=1531073310.858350 type=softirq softirq_type=3 pid=1386 proc_name=irq/105-enp2s0f cpu=20 softirq_time=4187 softirq_count=1
```

- `softirq_time_net.py` is similar to `softirq_time.py` but running for both TX and RX softirqs as I can not run two separate instances of `softirq_time.py` simultaneously
