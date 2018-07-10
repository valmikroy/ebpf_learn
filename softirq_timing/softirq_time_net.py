from __future__ import print_function
import os
from bcc import BPF
from time import sleep, strftime
from datetime import datetime





with open(os.path.dirname(__file__) + '/softirq_time_net.c') as  x: bpf_text  = x.read()


b = BPF(text=bpf_text)

exiting = 0

while (1):

        try:
            sleep(0.1)
        except KeyboardInterrupt:
            exiting = 1

        ts =  datetime.now().strftime("%s.%f")


        softirq_count = {}
        for (k, v) in b.get_table('softirq_count').items():
            softirq_count[(k.vec, k.cpu, k.pid, k.name)] = v.value

        for (k, v) in b.get_table('softirq_time').items():
            try:
                irq_count = softirq_count[(k.vec, k.cpu, k.pid, k.name)]
            except KeyError:
                irq_count = 0

            print('timestamp=%s type=softirq softirq_type=%d pid=%d proc_name=%s cpu=%d softirq_time=%d softirq_count=%d' % ( ts, k.vec,  k.pid, k.name.decode(), k.cpu, v.value, irq_count))


        b.get_table('softirq_count').clear()
        b.get_table('softirq_time').clear()


        if exiting == 1:
           exit()
