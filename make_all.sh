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


#Copy files to sd card

hdf_location="$1"
sdcard_abs="$2"


abs_path_hdf=`readlink -f $hdf_location`
 
#$linux_dir_r/make_kernel.sh
$linux_dir_r/make_uboot.sh
$dev_dir_r/make_dtb.sh $abs_path_hdf
$dev_dir_r/make_fsbl.sh $abs_path_hdf
$build_dir_r/make_bootbin.sh `basename "$abs_path_hdf" .hdf` 
./copy_to_sd_card.sh $sdcard_abs




trap : 0
echo -e "\e[92m *** DONE `basename "$0"` *** \e[39m"
