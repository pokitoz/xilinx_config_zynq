#!/bin/bash

set -e

if [ -z ${setup_env+x} ]; then
	echo -e "$c_info Sourcing setup_env.sh.. $c_default"
	source ../setup_env.sh
fi

# make sure to be in the same directory as this script #########################
script_dir_abs=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd "${script_dir_abs}"

pushd $linux_dir_r


if [ -z $1 ]; then
	echo -e "$c_info Nothing specified, using $env_def_config_kernel.. $c_default"
	defconfig_kernel=$env_def_config_kernel
else
	defconfig_kernel=$1
fi


# Functions definitions ########################################################

abort() {
	echo -e "$c_error Error in `basename "$0"`$c_default"
    exit 1
}

trap 'abort' 0


echo -e "$c_good *** START `basename "$0"` *** $c_default"

if [ ! -d "linux-xlnx" ]; then
	echo `pwd`
	echo -e "$c_error Please extract the archive. linux-xlnx folder not found$c_default"
	exit 1
fi


pushd $kernel_dir_r

echo -e "$c_info Cleaning.. $c_default"

cp $preset_dir_r/zynq_zturn_defconfig ./arch/arm/configs/

#make -j4 ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- distclean
echo -e "$c_info Compiling linux kernel with $defconfig_kernel configuration $c_default"
make -j4 ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- $defconfig_kernel
make -j4 ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- UIMAGE_LOADADDR=0x8000 uImage

echo -e "$c_info Generate all the Device tree Binary files $c_default"
make -j4 ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- dtbs

echo -e "$c_info Copy the generated uImage to $build_dir_r$c_default"
ls -l ./arch/arm/boot/uImage
cp ./arch/arm/boot/uImage ../../build/uImage

echo -e "$c_info Copy the standard dtb to $build_dir_r$c_default"
#ls -l ./arch/arm/boot/dts/zynq-zturn.dtb
#cp ./arch/arm/boot/dts/zynq-zturn.dtb "$build_dir_r"

popd


popd

trap : 0
echo -e "$c_good *** DONE `basename "$0"` *** $c_default"
