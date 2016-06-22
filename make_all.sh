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

function asking_to_do {
	ACTION=$1
	COMMAND=$2
	echo -e "\e[0;33m"
  read -r -s -n 1 -p "Do $ACTION ? [y,n] : " doit
  case $doit in
    y|Y) echo -e "$c_orange doing $ACTION $c_orange" && $COMMAND;;
    n|N) echo -e "$c_orange passing $ACTION $c_orange" ;;
    *) echo -e "$c_orange bad option $c_orange" && asking_to_do $ACTION $COMMAND ;;
  esac
}

trap 'abort' 0

echo -e "$c_good *** START `basename "$0"` *** $c_default"
#Copy files to sd card

if [ -z "$1" ]; then
	echo -e "$c_error You need to specify a HDF location$c_default"
	exit 1
fi


hdf_location="$1"
sdcard_abs="$2"

#if [ "$3" = "y" ]
#then
#	./clean_files.sh
#else
#	asking_to_do "clean files" ./clean_files.sh
#fi
#./clean_files.sh

abs_path_hdf=`readlink -f $hdf_location`
hdf_name_only=`basename "$abs_path_hdf" .hdf`


echo -e "$c_orange MENU DE LA MORT $c_default"

select opt in $OPTIONS_MENU; do
		if [ "$opt" = "Quit" ]; then
		 echo "Exit menu"
		 break
		elif [ "$opt" = "Make_all" ]; then
			#./clean_files.sh
			#$linux_dir_r/make_kernel.sh
			$linux_dir_r/make_uboot.sh
			$dev_dir_r/make_dtb.sh $abs_path_hdf
			$dev_dir_r/make_fsbl.sh $abs_path_hdf
			$build_dir_r/make_bootbin.sh $hdf_name_only
			./copy_to_sd_card.sh $sdcard_abs
		elif [ "$opt" = "Clean_build" ]; then
			./clean_files.sh
		elif [ "$opt" = "Make_kernel" ]; then
			select opt_s in $OPTIONS_MENU_KERNEL; do
				if [ "$opt_s" = "Quit" ]; then
				 echo "Exit sub menu"
				 break
			 elif [ "$opt_s" = "Use_default_config" ]; then
					$linux_dir_r/make_kernel.sh
				elif [ "$opt_s" = "Use_custom_config" ]; then
					$linux_dir_r/make_kernel.sh zynq_custom_defconfig
				else
				 echo "Bad option"
				fi
			done
		elif [ "$opt" = "Make_Uboot" ]; then
			$linux_dir_r/make_uboot.sh
		elif [ "$opt" = "Make_dtb" ]; then
			$dev_dir_r/make_dtb.sh $abs_path_hdf
		elif [ "$opt" = "Make_fsbl" ]; then
			$dev_dir_r/make_fsbl.sh $abs_path_hdf
		elif [ "$opt" = "Make_bootbin" ]; then
			$build_dir_r/make_bootbin.sh $hdf_name_only
		elif [ "$opt" = "Push_to_sd_card" ]; then
			./copy_to_sd_card.sh $sdcard_abs
		else
		 echo "Bad option"
		fi
done

#if [ "$3" = "y" ]
#then
#	$linux_dir_r/make_kernel.sh
#	$linux_dir_r/make_uboot.sh
#	$dev_dir_r/make_dtb.sh $abs_path_hdf
#	$dev_dir_r/make_fsbl.sh $abs_path_hdf
#	$build_dir_r/make_bootbin.sh $hdf_name_only
#else
#	asking_to_do "build kernel" $linux_dir_r/make_kernel.sh
#	asking_to_do "build uboot" $linux_dir_r/make_uboot.sh
#	asking_to_do "build dtb" "$dev_dir_r/make_dtb.sh $abs_path_hdf"
#	asking_to_do "make fsbl" "$dev_dir_r/make_fsbl.sh $abs_path_hdf"
#	asking_to_do "make bootbin" "$build_dir_r/make_bootbin.sh $hdf_name_only"
#fi

#$linux_dir_r/make_kernel.sh
#$linux_dir_r/make_uboot.sh
#$dev_dir_r/make_dtb.sh $abs_path_hdf
#$dev_dir_r/make_fsbl.sh $abs_path_hdf
#$build_dir_r/make_bootbin.sh $hdf_name_only




pushd $applications_dir_r
	for d in * ; do

	    if [ -d "$d" ]; then
			pushd "$d/"

				mkdir -p "includes"
				cp -f "$dev_dir_r/fsbl_$hdf_name_only/app/zynq_fsbl_bsp/ps7_cortexa9_0/include"/* ./includes

				if [ -f "make_$d.sh" ]; then
					echo -e "$c_info Changing chmod of the script $d $c_default"
					chmod +x ./"make_$d.sh"
					echo -e "$c_info Compiling $d $c_default"
					./"make_$d.sh"
				fi
			popd
		fi
	done
popd


#if [ "$3" = "y" ]
#then
#	./copy_to_sd_card.sh $sdcard_abs
#else
#	asking_to_do "copy to sd card" "./copy_to_sd_card.sh $sdcard_abs"
#fi
#./copy_to_sd_card.sh $sdcard_abs



echo -e "$c_info Check ./build/make_bootbin.sh to get the baud rate for the UART (should be 115200)$c_default"
echo -e "$c_info Use minicom or miniterm.py$c_default"
date

trap : 0
echo -e "$c_good *** DONE `basename "$0"` *** $c_default"
