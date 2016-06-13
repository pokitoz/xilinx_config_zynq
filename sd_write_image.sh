#!/bin/bash

#please specify the SD card location.

echo -e "\e[92m *** START `basename "$0"` *** \e[39m"

sdcard_abs="$1"
echo -e "\e[34m SD Card: $1 \e[39m"

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


echo -e "\e[34m Unmount everything related to the SD card\e[39m"
sudo umount ${sdcard_abs}
sudo umount ${sdcard_abs}$sdcard_fat32_partition_number
sudo umount ${sdcard_abs}$sdcard_dev_ext3_id

echo -e "\e[34m Install pv to have an estimation time \e[39m"
sudo apt-get install pv

echo -e "\e[34m Gunzip the xillinux image \e[39m"
gunzip ./filesystem/xillinux-1.3.img.gz
echo -e "\e[34m Copy the image to the SD card (might take a while) \e[39m"
sudo dd if="./filesystem/xillinux-1.3.img" bs=4M of=${sdcard_abs} | pv | dd of=/dev/null
rm -rf ./filesystem/xillinux-1.3.img
sync

echo -e "\e[92m *** DONE `basename "$0"` *** \e[39m"

