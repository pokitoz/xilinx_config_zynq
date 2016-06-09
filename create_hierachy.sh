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

mkdir -p $linux_dir_r
mkdir -p $tools_dir_r

if [ ! -f "xilinx_source.tar.gz" ]; then
	wget "https://www.dropbox.com/s/0zn56diit0mqh2c/xilinx_source.tar.gz"
fi

tar xvf ./xilinx_source.tar.gz 
cp ./xilinx_source/linux/linux-xlnx.tar.bz2 $linux_dir_r
cp ./xilinx_source/linux/u-boot-xlnx.tar.bz2 $linux_dir_r
cp ./xilinx_source/tools/Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux.tar.bz2 $tools_dir_r

pushd $linux_dir_r
	tar xvf ./linux-xlnx.tar.bz2
	tar xvf ./u-boot-xlnx.tar.bz2
popd

pushd $tools_dir_r
	tar xvf ./Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux.tar.bz2
popd


pushd $dev_dir_r
	mkdir -p ./projects
popd


trap : 0
echo -e "\e[92m *** DONE `basename "$0"` *** \e[39m"
