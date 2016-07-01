#!/bin/bash

set -e

if [ -z ${setup_env+x} ]; then
	echo -e "$c_info Sourcing setup_env.sh.. $c_default"
	source ./setup_env.sh
fi

# make sure to be in the same directory as this script #########################
script_dir_abs=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd "${script_dir_abs}"



# Functions definitions ########################################################

abort() {
	echo -e "$c_error Error in `basename "$0"`$c_default"
    exit 1
}

trap 'abort' 0

echo -e "$c_good *** START `basename "$0"` *** $c_default"
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
		echo -e "$c_errorYou entered a partition. Please enter only the name of the sdcard..$c_default"
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

	sudo mkdir -p "$media_ext4/root/Desktop"
	sudo rm -rf "$media_ext4/root/Desktop/*"



	pushd $applications_dir_r
		#shopt -s extglob
		#shopt -u extglob
		for d in * ; do
		    if [ -d $d ]; then
				ommit="make_*.sh *.pdf *.o ./old *~ .* *.o" 
				echo -e "$c_info Copying $d to \"$media_ext4/root/Desktop/$d\" ommiting $ommit . $c_default"
				#sudo cp -rv "!(*.sh)" "./$d" "$media_ext4/root/Desktop/"
				sudo rsync -rv --exclude 'make_*.sh' "./$d" "$media_ext4/root/Desktop/"
				#Exclude make.sh in the copy
				#cp -r !(make.sh) ./directory

			fi
		done

		echo -e "$c_info -------------------------------- $c_default"

		for d in *.sh ; do
			echo -e "$c_info Copying script $d to "$media_ext4/root/Desktop/" $c_default"
			sudo cp "$d" "$media_ext4/root/Desktop/"
		done

	popd


	pushd $preset_dir_r
		if [ -f "etc_network_interfaces" ]; then
			sudo cp "etc_network_interfaces" "$media_ext4/etc/network/interfaces"
		fi
		if [ -f "rc.local" ]; then
			sudo cp "rc.local" "$media_ext4/etc/rc.local"
		fi

		sudo cp -r ./pkg "$media_ext4/root/Desktop/"

	popd



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
echo -e "$c_good *** DONE `basename "$0"` *** $c_default"
