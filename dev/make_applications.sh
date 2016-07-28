#!/bin/bash

set -e

# make sure to be in the same directory as this script #########################
current_script_dir=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

if [ -z ${setup_env+x} ]; then
	echo -e "$c_info Sourcing setup_env.sh.. $c_default"
	source ../setup_env.sh
fi

pushd "${current_script_dir}"



abort() {
	echo -e "$c_error Error in `basename "$0"`$c_default"
    exit 1
}

trap 'abort' 0

echogood -e "$c_good *** START `basename "$0"` *** $c_default"

mkdir -p "includes"
cp -f "$dev_dir_r/fsbl_$1/app/zynq_fsbl_bsp/ps7_cortexa9_0/include"/xparameters.h ./includes/
cp -f "$dev_dir_r/fsbl_$1/app/zynq_fsbl_bsp/ps7_cortexa9_0/include"/xparameters_ps.h ./includes/
cp -f "$preset_dir_r/pl_io_define.h" ./includes/

pushd $applications_dir_r
	for d in * ; do

	    if [ -d "$d" ]; then
			
			pushd "$d/"

				if [ -f "make_$d.sh" ]; then
					echo -e "$c_info Changing chmod of the script $d $c_default"
					chmod +x ./"make_$d.sh"
					echo -e "$c_info Compiling $d $c_default"
					./"make_$d.sh"
				fi
			popd
		fi
	done
popd


popd

trap : 0
echo -e "$c_good *** DONE `basename "$0"` *** $c_default"
