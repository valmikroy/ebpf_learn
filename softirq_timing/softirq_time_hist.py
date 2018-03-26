from __future__ import print_function
from bcc import BPF
from time import sleep, strftime

with open('softirq_time_hist.c') as  x: bpf_text  = x.read()

b = BPF(text=bpf_text)
print("Tracing RX softirq time... Hit Ctrl-C to end.")

exiting = 0

test_ts =  strftime("%s")
dist = b.get_table("dist")

header = "ts,type"
for i in range(30):
    header = "%s,%sms" % (  header, i )

print(header)
while (1):
    try:
        sleep(int(1))
    except KeyboardInterrupt:
        exiting = 1

    ts =  strftime("%s")
    data = []
    for i in range(30):
      data.append(0)

    for k, v in dist.items():
        data[k.slot] = "%d" %  v.value

    convert_first_to_generator = (str(w) for w in data)
    print ( "%-8s,net_rx_%s,%s" % (ts , test_ts , ','.join(convert_first_to_generator)))

    dist.clear()

    if exiting == 1:
        exit()
