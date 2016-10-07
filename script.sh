#!/bin/sh

umount /fusemnt 
rmdir /fusemnt 
make clean
make
mkdir /fusemnt
./ctlfs /fusemnt
ls /fusemnt

