#!/bin/bash

#please specify the SD card location.
#It will paritionate the SDcard to have 2 parts.

echo -e "\e[92m *** START `basename "$0"` *** \e[39m"

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



sudo umount ${sdcard_abs}
sudo umount ${sdcard_abs}$sdcard_fat32_partition_number
sudo umount ${sdcard_abs}$sdcard_dev_ext3_id

sudo apt-get install pv

gunzip ./filesystem/xillinux-1.3.img.gz
sudo dd if="./filesystem/xillinux-1.3.img" bs=4M of=${sdcard_abs} | pv | dd of=/dev/null
rm -rf ./filesystem/xillinux-1.3.img
sync
