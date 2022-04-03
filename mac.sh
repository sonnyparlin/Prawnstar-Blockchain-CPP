echo kern.maxfiles=10240 | sudo tee -a /etc/sysctl.conf
echo kern.maxfilesperproc=10240 | sudo tee -a /etc/sysctl.conf
sudo sysctl -w kern.maxfiles=10240
sudo sysctl -w kern.maxfilesperproc=10240
ulimit -n 10240 
