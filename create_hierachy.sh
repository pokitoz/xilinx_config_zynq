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

set +e
echo -e "\e[34m Install tree package /not needed, CTRL+C if you don't want) \e[39m"
sudo apt-get install tree
set -e

echo -e "\e[34m Create $linux_dir_r\e[39m"
mkdir -p $linux_dir_r
echo -e "\e[34m Create $tools_dir_r\e[39m"
mkdir -p $tools_dir_r

if [ ! -f "xilinx_source.tar.gz" ]; then
	echo -e "\e[34m Download the sources files \e[39m"
	wget "https://www.dropbox.com/s/0zn56diit0mqh2c/xilinx_source.tar.gz"
fi

tar xvf ./xilinx_source.tar.gz 
echo -e "\e[34m Copy the archives to the proper folder \e[39m"
echo -e "\e[34m linux-xlnx.tar.bz2 into $linux_dir_r \e[39m"
cp ./xilinx_source/linux/linux-xlnx.tar.bz2 $linux_dir_r
echo -e "\e[34m u-boot-xlnx.tar.bz2 into $linux_dir_r \e[39m"
cp ./xilinx_source/linux/u-boot-xlnx.tar.bz2 $linux_dir_r
echo -e "\e[34m Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux.tar.bz2 into $tools_dir_r \e[39m"
cp ./xilinx_source/tools/Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux.tar.bz2 $tools_dir_r

pushd $linux_dir_r
	echo -e "\e[34m Untar linux-xlnx.tar.bz2 and u-boot-xlnx.tar.bz2 \e[39m"
	tar xvf ./linux-xlnx.tar.bz2
	tar xvf ./u-boot-xlnx.tar.bz2
popd

pushd $tools_dir_r
	echo -e "\e[34m Untar Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux.tar.bz2 \e[39m"
	tar xvf ./Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux.tar.bz2
popd


pushd $dev_dir_r
	echo -e "\e[34m Create folders $projects_dir_r\e[39m"
	mkdir -p ./projects

	echo -e "\e[34m Create folders $applications_dir_r\e[39m"
	mkdir -p ./applications
popd

echo -e "\e[34m Final hierarchy (if tree package is installed)\e[39m"
set +e
tree -d --filelimit 10000 -L 2 ./
set -e



echo -e "\e[34m Make all the scripts executable using chmod \e[39m"
sudo chmod +x ./sd_write_image.sh
sudo chmod +x ./clean_files.sh
sudo chmod +x ./copy_to_sd_card.sh
sudo chmod +x ./make_all.sh
sudo chmod +x ./part_sd_card.sh
sudo chmod +x ./sd_write_image.sh
sudo chmod +x ./setup_env.sh

sudo chmod +x $build_dir_r/make_bootbin.sh
sudo chmod +x $dev_dir_r/make_dtb.sh
sudo chmod +x $dev_dir_r/make_fsbl.sh
sudo chmod +x $linux_dir_r/make_kernel.sh
sudo chmod +x $linux_dir_r/make_uboot.sh

trap : 0
echo -e "\e[92m *** DONE `basename "$0"` *** \e[39m"
