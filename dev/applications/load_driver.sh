#!/bin/bash

mode="444"

input="$1"


if [ -z "$input" ]; then
	echo "## You need to specify a driver name..."
	exit 0
fi

device_name=`basename $input .ko`
directory=`dirname $input`

# unload module if already loaded
if [ "$(lsmod | grep "${device_name}")" ]; then
	echo "## Module $device_name already loaded. Will be unloaded"
    ./unload_driver.sh ${device_name}
	echo "## Module $device_name unloaded"
fi

echo -e "## Loading module $device_name from $directory\n"
# invoke insmod and use a pathname, as insmod doesn't look in . by default
/sbin/insmod "$directory/$device_name.ko" || exit 1

echo -e "## Lsmod output (all drivers loaded)"
lsmod
echo ""


# retrieve major number
major=$(cat /proc/devices | grep "$device_name")
echo "## Major module: "
echo -e "$major \n"
major_number=($major)
echo "##Major number: ${major_number[0]}"
echo "##Removing stale nodes"
rm -f /dev/${device_name}
echo "##Replacing the old nodes"
mknod /dev/${device_name} c ${major_number[0]} 0




