#!/bin/bash

#please specify the SD card location.
#It will paritionate the SDcard to have 2 parts.

sdcard_abs="$1"

dmesg_var=`dmesg | tail -n 3`
dmesg_var_grep=`echo $dmesg_var | grep "new high speed SD card"`

if [ -n ${dmesg_var+x} ]; then
	echo -e $dmesg_var
fi

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


#The fdisk utility does not erase the first few bytes of the first sector in the card
#dd is used to erase the first sector.
sudo dd if=/dev/zero of=${sdcard_abs} bs=1024 count=1

size_sd_byte=`sudo fdisk -l ${sdcard_abs} | head -n 2`
echo "Size sd card "$size_sd_byte

arrIN=(${size_sd_byte// / })
size_sd_card=${arrIN[4]}


#Look for the size of the device in bytes and calculate the new number of cylinders 
#using the following formula, dropping all fractions:
#new_cylinders = <size> / 8225280
new_cylinders=$(($size_sd_card/8225280))
echo $new_cylinders


#Partition the SD card. 
#Create two partitions on the SD card.
# One 200 MB sized boot partition. 
# Second partition taking the remaining space on the SD card.


#Configure the sectors, heads and cylinders of the SD card.
#x				Expert command
#h				#heads
#255
#s				#sector
#63
#c				#cylinders
#$new_cylinders
#r

heads=255
sectors=63
lastsector="+200M"
echo -e "x\nh\n$heads\ns\n$sector\nc\n$new_cylinders\nr\nn\np\n1\n\n$lastsector\nn\np\n2\n\n\na\n1\nt\n1\nc\nt\n2\n83\np\nw\n" | sudo fdisk "${sdcard_abs}"


sudo mkfs.vfat -F 32 -n boot ${sdcard_abs}$sdcard_fat32_partition_number
sudo mkfs.ext4 -L root ${sdcard_abs}$sdcard_dev_ext3_id


sudo mkdir -p /mnt/boot
sudo mount ${sdcard_abs}$sdcard_fat32_partition_number /mnt/boot


sudo umount ${sdcard_abs}$sdcard_fat32_partition_number
sudo umount ${sdcard_abs}$sdcard_dev_ext3_id

# delete mount points for sdcard
sudo rm -rf /mnt/boot

sudo sync


if [ "$2" -eq "1" ]; then

	chmod +x ./sd_write_image.sh
	./sd_write_image.sh ${sdcard_abs}

fi



echo ""
echo "You may need to plug/unplug the SD card to see the partitions."
echo ""


