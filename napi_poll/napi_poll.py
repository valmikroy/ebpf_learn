from __future__ import print_function
import argparse
from bcc import BPF
from time import sleep, strftime
from datetime import datetime




with open('napi_poll.c') as  x: bpf_text  = x.read()

b = BPF(text=bpf_text)

exiting = 0


while (1):

        try:
            sleep(0.1)
        except KeyboardInterrupt:
            exiting = 1

        ts =  datetime.now().strftime("%s.%f")

        napi_poll_full = {}
        for (k, v) in b.get_table('napi_poll_full').items():
            napi_poll_full[(k.dev, k.cpu, k.pid, k.budget, k.name)] = v.value


        for (k, v) in b.get_table('napi_poll_count').items():

            try:
                full_count = napi_poll_full[(k.dev, k.cpu, k.pid, k.budget, k.name)]
            except KeyError:
                full_count = 0
            print('timestamp=%s type=napi iface=%s cpu=%d pid=%d budget=%d proc_name=%s poll_count=%d full_count=%d' % ( ts, k.dev, k.cpu,  k.pid, k.budget, k.name.decode(), v.value, full_count ))


        b.get_table('napi_poll_count').clear()
        b.get_table('napi_poll_full').clear()

        if exiting == 1:
           exit()
