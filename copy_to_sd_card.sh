#!/bin/bash

set -e

if [ -z ${setup_env+x} ]; then
	echo -e "\e[34m Sourcing setup_env.sh.. \e[39m"
	source ./setup_env.sh
fi

# make sure to be in the same directory as this script #########################
script_dir_abs=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd "${script_dir_abs}"



# Functions definitions ########################################################

abort() {
	echo -e "\e[91m Error in `basename "$0"`\e[39m"
    exit 1
}

trap 'abort' 0

echo -e "\e[92m *** START `basename "$0"` *** \e[39m"
#Copy files to sd card

sdcard_abs="$1"

if [ ! -b "${sdcard_abs}" ]; then

        while [ ! -b "${sdcard_abs}" ]; do

            echo "Error: could not find \"${sdcard_abs}\""
            echo "You can plug the SD card or quit the script with CTRL + C"

            read -p "Press [Enter] key to restart..."

        done
fi

if [ "$(echo "${sdcard_abs}" | grep -P "/dev/sd\w$")" ]; then
		sdcard_fat32_partition_number="1"
		sdcard_dev_ext3_id="2"
		sdcard_preloader_partition_number="3"
	elif [ "$(echo "${sdcard_abs}" | grep -P "/dev/mmcblk\w$")" ]; then
		sdcard_fat32_partition_number="p1"
		sdcard_dev_ext3_id="p2"
		sdcard_preloader_partition_number="p3"
	else
		echo "You entered a partition. Please enter only the name of the sdcard.. "
		exit 1
fi

media_fat="/media/sdcard_fat32"
media_ext4="/media/sdcard_ext4"

set +e
    sudo umount ${sdcard_abs}${sdcard_dev_ext3_id}
    sudo umount ${sdcard_abs}${sdcard_fat32_partition_number}
set -e


    sudo mkdir -p "$media_ext4"
    sudo mount -t ext4 "${sdcard_abs}$sdcard_dev_ext3_id" "$media_ext4"
	#Copy files in rootfs
#	sudo cp "./$drivers_dir/blink.out" "$media_ext4/root/Desktop/"

	sudo sync

    sudo umount "${sdcard_abs}$sdcard_dev_ext3_id"
    sudo rm -rf "$media_ext4"

    sudo mkdir -p "$media_fat"
    sudo mount -t vfat "${sdcard_abs}${sdcard_fat32_partition_number}" "$media_fat"

	sudo rm -rf "$media_fat"/*
    sudo cp "$build_dir_r/uImage" "$media_fat"
    sudo cp "$build_dir_r/devicetree.dtb" "$media_fat"
	sudo cp "$build_dir_r/BOOT.bin" "$media_fat/"
	sudo cp "$build_dir_r/uEnv.txt" "$media_fat/"
	sudo cp "$build_dir_r/7z020.bit" "$media_fat"


	ls -l "$media_fat/"

    sudo sync

    sudo umount "$media_fat"
    sudo rm -rf "$media_fat"


set +e
    sudo umount ${sdcard_abs}${sdcard_dev_ext3_id}
    sudo umount ${sdcard_abs}${sdcard_fat32_partition_number}
set -e




trap : 0
echo -e "\e[92m *** DONE `basename "$0"` *** \e[39m"
