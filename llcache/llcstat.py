#!/usr/bin/python

from __future__ import print_function
import argparse
from bcc import BPF, PerfType, PerfHWConfig
import signal
import os
from time import sleep, strftime
from datetime import datetime

parser = argparse.ArgumentParser(
    description="Summarize cache references and misses by PID",
    formatter_class=argparse.RawDescriptionHelpFormatter)

parser.add_argument("-p", "--pid",
    help="trace this PID only")

args = parser.parse_args()

perf_sample_period =  100

with open(os.path.dirname(__file__) + '/llcstat.c') as x: bpf_text  = x.read()



if args.pid:
        try:
                open('/proc/%s/comm' % args.pid).read().strip()
        except IOError:
                print("Invalid PID %s" % args.pid)
                exit()

        bpf_text = bpf_text.replace('--PID--',  args.pid)
else:
        print("Provide pid")
        exit()

b = BPF(text=bpf_text)
b.attach_perf_event(
    ev_type=PerfType.HARDWARE, ev_config=PerfHWConfig.CACHE_MISSES,
    fn_name="on_cache_miss", sample_period=perf_sample_period)
b.attach_perf_event(
    ev_type=PerfType.HARDWARE, ev_config=PerfHWConfig.CACHE_REFERENCES,
    fn_name="on_cache_ref", sample_period=perf_sample_period)


exiting = 0


while (1):

        try:
            sleep(0.1)
        except KeyboardInterrupt:
            exiting = 1

        ts =  datetime.now().strftime("%s.%f")


        miss_count = {}
        for (k, v) in b.get_table('miss_count').items():
            miss_count[(k.pid, k.cpu, k.name)] = v.value

        for (k, v) in b.get_table('ref_count').items():
            try:
                miss = miss_count[(k.pid, k.cpu, k.name)]
            except KeyError:
                miss = 0

            print('timestamp=%s type=llcache pid=%d proc_name=%s cpu=%d cache_refe=%d cache_miss=%d' % ( ts,  k.pid, k.name.decode(), k.cpu, v.value, miss))


        b.get_table('miss_count').clear()
        b.get_table('ref_count').clear()


        if exiting == 1:
           exit()
