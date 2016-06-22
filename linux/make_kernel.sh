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

# Functions definitions ########################################################

abort() {
	echo -e "$c_error Error in `basename "$0"`$c_default"
    exit 1
}

trap 'abort' 0


echo -e "$c_good *** START `basename "$0"` *** $c_default"

if [ ! -d "$kernel_dir" ]; then


	echo -e "$c_error Could not find $kernel_dir_r $c_default"
	echo `pwd`

	echo -e ""
	echo -e "$c_error Please extract the archive. $kernel_dir folder not found in $linux_dir_r$c_default"
	echo -e "$c_error Or you can clone one of the following repository in $linux_dir_r$c_default"

	echo -e ""
	echo -e "$c_error https://github.com/Xilinx/linux-xlnx.git $c_default"
	echo -e "$c_error https://github.com/Digilent/linux-digilent $c_default"
	echo -e ""


	echo -e "$c_error Do not forget to update the ./set_env.sh script! $c_default"
	echo -e "$c_error \$kernel_dir should be updated depending the name of the folder$c_default"
	echo -e ""
	exit 1
fi


pushd $kernel_dir


#set +e
#echo -e "$c_info You are using the branch: $c_default"
#git remote show origin
#echo -e "$c_info ------------------------- $c_default"
#set -e


echo -e "$c_info Cleaning.. $c_default"
#make -j4 ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- distclean

if [ -z $1 ]; then
	echo -e "$c_info Nothing specified, using zynq_zturn_defconfig .. $c_default"
	defconfig_kernel=zynq_zturn_defconfig
	cp $preset_dir_r/config_kernel/$defconfig_kernel ./arch/arm/configs/zynq_zturn_defconfig
else
	echo -e "$c_info Using custom config $1 .. $c_default"
	defconfig_kernel=$1
	cp $preset_dir_r/config_kernel/$defconfig_kernel ./arch/arm/configs/zynq_zturn_defconfig
fi

echo -e "$c_info Compiling linux kernel with $defconfig_kernel configuration $c_default"
make -j4 ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- zynq_zturn_defconfig
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
