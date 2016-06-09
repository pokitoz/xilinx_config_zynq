#!/bin/bash

set -e

if [ -z ${setup_env+x} ]; then
	echo -e "\e[34m Sourcing setup_env.sh.. \e[39m"
	source ../setup_env.sh
fi

# make sure to be in the same directory as this script #########################
script_dir_abs=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd "${script_dir_abs}"



pushd $dev_dir_r



abort() {
	echo -e "\e[91m Error in `basename "$0"`\e[39m"
    exit 1
}

trap 'abort' 0




echo -e "\e[92m *** START `basename "$0"` *** \e[39m"

if [ ! -d "dtc" ]; then
	echo -e "\e[34m Cloning from git the dtc repository.. \e[39m" 
	git clone https://git.kernel.org/pub/scm/utils/dtc/dtc.git
fi

echo -e "\e[34m Build Device Tree Compiler (dtc).. \e[39m" 
pushd "dtc"
make -j4
echo -e "\e[34m Add the dtc command to path.. \e[39m" 
export PATH=`pwd`:$PATH
popd


if [ ! -d "device-tree-xlnx" ]; then
	echo -e "\e[34m Cloning from git the device tree repository.. \e[39m" 
	git clone https://github.com/Xilinx/device-tree-xlnx.git
fi

pushd "device-tree-xlnx"
	echo -e "\e[34m Add the device tree source to the path (needed for hsi).. \e[39m" 
	device_tree_xlnx_path=`pwd`
popd

hdf_path="$1"
basename_hdf=`basename $1 .hdf`


mkdir -p "dtb_$basename_hdf"
echo -e "\e[34m Copy $hdf_path to ./dtb_$basename_hdf.. \e[39m" 
cp $hdf_path "./dtb_$basename_hdf"
pushd "dtb_$basename_hdf"

	echo "open_hw_design $basename_hdf.hdf; set_repo_path $device_tree_xlnx_path; create_sw_design device-tree -os device_tree -proc ps7_cortexa9_0; generate_target -dir dts" | hsi
	
	echo -e "\e[34m Generate the DTB in `pwd`/dts.. \e[39m" 
	pushd "dts"
		dtc -I dts -O dtb -o devicetree.dtb system.dts
	popd

popd


popd

trap : 0
echo -e "\e[92m *** DONE `basename "$0"` *** \e[39m"
