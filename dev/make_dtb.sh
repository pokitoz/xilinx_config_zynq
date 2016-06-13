#!/bin/bash

set -e

if [ -z ${setup_env+x} ]; then
	echo -e "$c_info Sourcing setup_env.sh.. $c_default"
	source ../setup_env.sh
fi

# make sure to be in the same directory as this script #########################
script_dir_abs=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd "${script_dir_abs}"



pushd $dev_dir_r



abort() {
	echo -e "$c_error Error in `basename "$0"`$c_default"
    exit 1
}

trap 'abort' 0




echo -e "$c_good *** START `basename "$0"` *** $c_default"

if [ ! -d "dtc" ]; then
	echo -e "$c_info Cloning from git the dtc repository.. $c_default" 
	git clone https://git.kernel.org/pub/scm/utils/dtc/dtc.git
fi

echo -e "$c_info Build Device Tree Compiler (dtc).. $c_default" 
pushd "dtc"
make -j4
echo -e "$c_info Add the dtc command to path.. $c_default" 
export PATH=`pwd`:$PATH
popd


if [ ! -d "device-tree-xlnx" ]; then
	echo -e "$c_info Cloning from git the device tree repository.. $c_default" 
	git clone https://github.com/Xilinx/device-tree-xlnx.git
fi

pushd "device-tree-xlnx"
	echo -e "$c_info Add the device tree source to the path (needed for hsi).. $c_default" 
	device_tree_xlnx_path=`pwd`
popd

hdf_path="$1"
basename_hdf=`basename $1 .hdf`


mkdir -p "dtb_$basename_hdf"
echo -e "$c_info Copy $hdf_path to ./dtb_$basename_hdf.. $c_default" 
cp $hdf_path "./dtb_$basename_hdf"
pushd "dtb_$basename_hdf"

	echo "open_hw_design $basename_hdf.hdf; set_repo_path $device_tree_xlnx_path; create_sw_design device-tree -os device_tree -proc ps7_cortexa9_0; generate_target -dir dts" | hsi
	
	echo -e "$c_info Generate the DTB in `pwd`/dts.. $c_default" 
	pushd "dts"
		dtc -I dts -O dtb -o devicetree.dtb system.dts
		dtc -I dtb -O dts -o out_system.dts devicetree.dtb 
	popd

popd


popd

trap : 0
echo -e "$c_good *** DONE `basename "$0"` *** $c_default"
