# ebpf_learn
Various preliminary ebpf code


install bcc tools with following snippet

```bash
# install 4.13 series kernel 
sudo apt-get install `apt-cache search linux | grep 4.13.0-45 | awk -F' - ' '{print $1}' | xargs ` -y


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
