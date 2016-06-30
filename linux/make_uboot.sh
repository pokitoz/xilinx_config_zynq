#!/bin/bash

set -e

# make sure to be in the same directory as this script #########################
current_script_dir=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

if [ -z ${setup_env+x} ]; then
	echo -e "$c_info Sourcing setup_env.sh.. $c_default"
	source ../setup_env.sh
fi

pushd "${current_script_dir}"






# Functions definitions ########################################################

abort() {
	echo -e "$c_error Error in `basename "$0"`$c_default"
    exit 1
}

trap 'abort' 0


echo -e "$c_good *** START `basename "$0"` *** $c_default"

if [ ! -d "u-boot-xlnx" ]; then
	echo `pwd`
	echo -e "$c_error Please extract the archive. u-boot-xlnx folder not found$c_default"
	exit 1
fi

pushd ./u-boot-xlnx

echo -e "$c_info Cleaning.. $c_default"
make -j4 ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- distclean
echo -e "$c_info Compiling uboot with $uboot_configuration $c_default"
make -j4 ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- $uboot_configuration
make -j4 ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi-

echo -e "$c_info Copy the generated u-boot /and rename to u-boot.elf to $build_dir_r$c_default"
ls -l ./u-boot
cp ./u-boot "$build_dir_r/u-boot.elf"
popd


popd


trap : 0
echo -e "$c_good *** DONE `basename "$0"` *** $c_default"
