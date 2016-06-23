#!/bin/bash

echo 1 | cp util/80-xdma.rules /etc/udev/rules.d/
insmod dev/xdma.ko

