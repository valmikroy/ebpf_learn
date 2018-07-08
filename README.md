# ebpf_learn
Various preliminary ebpf code


install bcc tools with following snippet

```bash
# cpu layout script
https://raw.githubusercontent.com/emmericp/dpdk/master/usertools/cpu_layout.py
 
# softnet stats
https://github.com/hjr3/softnet-stat#build
 
# perf stats install
# https://perf.wiki.kernel.org/index.php/Tutorial
sudo apt-get install linux-tools-common linux-tools-generic linux-tools-`uname -r`
 
# bcc
# https://github.com/iovisor/bcc/blob/master/INSTALL.md
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys D4284CDD
echo "deb https://repo.iovisor.org/apt/xenial xenial main" | sudo tee /etc/apt/sources.list.d/iovisor.list
sudo apt-get update
sudo apt-get install bcc-tools libbcc-examples linux-headers-$(uname -r)
```
