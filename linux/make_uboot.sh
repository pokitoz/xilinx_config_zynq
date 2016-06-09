#!/bin/bash

set -e

if [ -z ${setup_env+x} ]; then
	echo -e "\e[34m Sourcing setup_env.sh.. \e[39m"
	source ../setup_env.sh
fi

# make sure to be in the same directory as this script #########################
script_dir_abs=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd "${script_dir_abs}"

pushd $linux_dir_r






# Functions definitions ########################################################

abort() {
	echo -e "\e[91m Error in `basename "$0"`\e[39m"
    exit 1
}

trap 'abort' 0


echo -e "\e[92m *** START `basename "$0"` *** \e[39m"

if [ ! -d "u-boot-xlnx" ]; then
	echo `pwd`
	echo -e "\e[91m Please extract the archive. u-boot-xlnx folder not found\e[39m"
	exit 1
fi

pushd ./u-boot-xlnx

echo -e "\e[34m Cleaning.. \e[39m"
make -j4 ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- distclean
echo -e "\e[34m Compiling uboot with zturn configuration \e[39m"
make -j4 ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- zynq_zturn_config
make -j4 ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi-

echo -e "\e[34m Copy the generated u-boot /and rename to u-boot.elf to $build_dir_r\e[39m"
ls -l ./u-boot
cp ./u-boot "$build_dir_r/u-boot.elf"
popd


popd


trap : 0
echo -e "\e[92m *** DONE `basename "$0"` *** \e[39m"
