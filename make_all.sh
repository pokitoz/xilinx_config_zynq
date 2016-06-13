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

if [ -z "$1" ]; then
	echo -e "$c_error You need to specify a HDF location$c_default"
	exit 1
fi


hdf_location="$1"
sdcard_abs="$2"

./clean_files.sh

abs_path_hdf=`readlink -f $hdf_location`
hdf_name_only=`basename "$abs_path_hdf" .hdf`

$linux_dir_r/make_kernel.sh
$linux_dir_r/make_uboot.sh
$dev_dir_r/make_dtb.sh $abs_path_hdf
$dev_dir_r/make_fsbl.sh $abs_path_hdf
$build_dir_r/make_bootbin.sh $hdf_name_only




pushd $applications_dir_r
	for d in * ; do

	    if [ -d "$d" ]; then
			pushd "$d/"

				mkdir -p "includes"
				cp -f "$dev_dir_r/fsbl_$hdf_name_only/app/zynq_fsbl_bsp/ps7_cortexa9_0/include"/* ./includes

				if [ -f "make_$d.sh" ]; then
					echo -e "$c_info Compiling $d $c_default"
					./"make_$d.sh"
				fi
			popd
		fi
	done	
popd


./copy_to_sd_card.sh $sdcard_abs



echo -e "$c_info Check ./build/make_bootbin.sh to get the baud rate for the UART (should be 115200)$c_default"
echo -e "$c_info Use minicom or miniterm.py$c_default"


trap : 0
echo -e "$c_good *** DONE `basename "$0"` *** $c_default"
