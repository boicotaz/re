#! /bin/bash
/opt/cross-pi-gcc-7.4.0-0/bin/arm-linux-gnueabihf-gcc -lpthread main.c -o main.out
/opt/cross-pi-gcc-7.4.0-0/bin/arm-linux-gnueabihf-gcc -lpthread freader.c -o reader.out
scp main.out reader.out root@192.168.1.37:/root/

