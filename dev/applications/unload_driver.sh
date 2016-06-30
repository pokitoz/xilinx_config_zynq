#!/bin/bash


module="$1"

if [ -z "$module" ]; then
	echo "You need to specify a driver name..."
	exit 0
fi


# invoke rmmod with all arguments we got
/sbin/rmmod $module || exit 1

lsmod

# Remove stale nodes
rm -f /dev/${module}

