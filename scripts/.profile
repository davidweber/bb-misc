IP=`/sbin/ifconfig eth0 | grep "inet addr" | cut -d":" -f2 | cut -d" " -f1`
PS1=$'\[\e[31;2m\][\u@${IP}]\[\e[34;2m\] ${PWD}\[\e[0m\]\n# '
export PATH=$PATH:/home/root
