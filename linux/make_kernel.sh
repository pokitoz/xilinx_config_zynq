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


if [ -z $1 ]; then
	echo -e "\e[34m Nothing specified, using $env_def_config_kernel.. \e[39m"
	defconfig_kernel=$env_def_config_kernel
else
	defconfig_kernel=$1
fi


# Functions definitions ########################################################

abort() {
	echo -e "\e[91m Error in `basename "$0"`\e[39m"
    exit 1
}

trap 'abort' 0


echo -e "\e[92m *** START `basename "$0"` *** \e[39m"

if [ ! -d "linux-xlnx" ]; then
	echo `pwd`
	echo -e "\e[91m Please extract the archive. linux-xlnx folder not found\e[39m"
	exit 1
fi


pushd $kernel_dir_r

echo -e "\e[34m Cleaning.. \e[39m"

cp $preset_dir_r/zynq_zturn_defconfig ./arch/arm/configs/

#make -j4 ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- distclean
echo -e "\e[34m Compiling linux kernel with $defconfig_kernel configuration \e[39m"
make -j4 ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- $defconfig_kernel
make -j4 ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- UIMAGE_LOADADDR=0x8000 uImage

echo -e "\e[34m Generate all the Device tree Binary files \e[39m"
make -j4 ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- dtbs

echo -e "\e[34m Copy the generated uImage to $build_dir_r\e[39m"
ls -l ./arch/arm/boot/uImage
cp ./arch/arm/boot/uImage ../../build/uImage

echo -e "\e[34m Copy the standard dtb to $build_dir_r\e[39m"
#ls -l ./arch/arm/boot/dts/zynq-zturn.dtb
#cp ./arch/arm/boot/dts/zynq-zturn.dtb "$build_dir_r"

popd


popd

trap : 0
echo -e "\e[92m *** DONE `basename "$0"` *** \e[39m"
