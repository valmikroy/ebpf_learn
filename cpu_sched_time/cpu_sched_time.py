#!/usr/bin/python


from bcc import BPF
import os
import argparse
from time import sleep, strftime

examples = """examples:
    cpudist -p 185       # trace PID 185 only
"""

parser = argparse.ArgumentParser(
    description="Summarize on-CPU time per task as a histogram.",
    formatter_class=argparse.RawDescriptionHelpFormatter,
    epilog=examples)

parser.add_argument("-p", "--pid",
    help="trace this PID only")
args = parser.parse_args()


# Read ebpf c code
with open( os.path.dirname(__file__) + '/cpu_sched_time.c') as x: bpf_text  = x.read()



# Insert PID to monitor 
if args.pid:
        bpf_text = bpf_text.replace('--PID--',  args.pid)
else:
        print("Provide pid")
        exit()


# debug
#print(bpf_text)


# attach probe
b = BPF(text=bpf_text)
b.attach_kprobe(event="finish_task_switch", fn_name="sched_switch")


print("Tracing PID %s" % args.pid)

# Pull histogram from C to python space
dist = b.get_table("dist")

exiting = 0
interval  = 1
timestamp = 1
label = "usec"
section = args.pid
countdown = 15


# this infinite loop to fetch reading
while (1):
    try:
        sleep(int(interval))
    except KeyboardInterrupt:
        exiting = 1

    print()
    if timestamp:
        print("%-8s\n" % strftime("%H:%M:%S"))

    def pid_to_comm(pid):
        try:
            comm = open("/proc/%d/comm" % pid, "r").read()
            return "%d %s" % (pid, comm)
        except IOError:
            return str(pid)

    dist.print_log2_hist(label, section, section_print_fn=pid_to_comm)
    dist.clear()

    countdown -= 1
    if exiting or countdown == 0:
        exit()
