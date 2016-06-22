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

set +e
echo -e "$c_info Install tree package /not needed, CTRL+C if you don't want) $c_default"
sudo apt-get install tree
set -e

echo -e "$c_info Create $linux_dir_r$c_default"
mkdir -p $linux_dir_r
echo -e "$c_info Create $tools_dir_r$c_default"
mkdir -p $tools_dir_r

if [ ! -f "xilinx_source.tar.gz" ]; then
	echo -e "$c_info Download the sources files $c_default"
	wget "https://www.dropbox.com/s/0zn56diit0mqh2c/xilinx_source.tar.gz"
fi

tar xvf ./xilinx_source.tar.gz 
echo -e "$c_info Copy the archives to the proper folder $c_default"
echo -e "$c_info linux-xlnx.tar.bz2 into $linux_dir_r $c_default"
cp ./xilinx_source/linux/linux-xlnx.tar.bz2 $linux_dir_r
echo -e "$c_info u-boot-xlnx.tar.bz2 into $linux_dir_r $c_default"
cp ./xilinx_source/linux/u-boot-xlnx.tar.bz2 $linux_dir_r
echo -e "$c_info Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux.tar.bz2 into $tools_dir_r $c_default"
cp ./xilinx_source/tools/Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux.tar.bz2 $tools_dir_r

pushd $linux_dir_r
	echo -e "$c_info Untar linux-xlnx.tar.bz2 and u-boot-xlnx.tar.bz2 $c_default"
	tar xvf ./linux-xlnx.tar.bz2
	tar xvf ./u-boot-xlnx.tar.bz2
popd

pushd $tools_dir_r
	echo -e "$c_info Untar Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux.tar.bz2 $c_default"
	tar xvf ./Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux.tar.bz2
popd


pushd $dev_dir_r
	echo -e "$c_info Create folders $projects_dir_r$c_default"
	mkdir -p ./projects

	echo -e "$c_info Create folders $applications_dir_r$c_default"
	mkdir -p ./applications
popd

echo -e "$c_info Final hierarchy (if tree package is installed)$c_default"
set +e
tree -d --filelimit 10000 -L 2 ./
set -e



echo -e "$c_info Make all the scripts executable using chmod $c_default"
sudo chmod +x ./sd_write_image.sh
sudo chmod +x ./clean_files.sh
sudo chmod +x ./copy_to_sd_card.sh
sudo chmod +x ./make_all.sh
sudo chmod +x ./part_sd_card.sh
sudo chmod +x ./sd_write_image.sh
sudo chmod +x ./setup_env.sh

sudo chmod +x $build_dir_r/make_bootbin.sh
sudo chmod +x $dev_dir_r/make_dtb.sh
sudo chmod +x $dev_dir_r/make_applications.sh
sudo chmod +x $dev_dir_r/make_fsbl.sh
sudo chmod +x $linux_dir_r/make_kernel.sh
sudo chmod +x $linux_dir_r/make_uboot.sh

trap : 0
echo -e "$c_good *** DONE `basename "$0"` *** $c_default"
