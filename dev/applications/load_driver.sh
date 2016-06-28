#!/bin/sh

mode="444"

array="$1"

device=$array
module=$array

# unload module if already loaded
if [ "$(lsmod | grep "${device}")" ]; then
    sh unload_driver.sh
fi

# invoke insmod and use a pathname, as insmod doesn't look in . by default
/sbin/insmod ./$module.ko || exit 1

# retrieve major number
major=$(awk "\$2==\"$module\" {print \$1}" /proc/devices)

# Remove stale nodes and replace them
rm -f /dev/${device}
#mknod /dev/${device} c $major 0




