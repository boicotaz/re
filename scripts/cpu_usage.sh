#! /bin/bash
pid=$(ps aux | grep "./main.out" | grep -v 'grep' | tr -s " " | cut -d " " -f2)

while [ $(ps aux | grep "./main.out" | grep -v 'grep' | tr -s " " | cut -d " " -f2) ]
    do 
    ps -p $pid -o %cpu | grep [0-9] >> cpu_usage.txt
    # cat cpu_usage.txt
    sleep 1
    done