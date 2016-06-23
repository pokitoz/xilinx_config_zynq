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

pushd $applications_dir_r
	for d in * ; do

	    if [ -d "$d" ]; then
			pushd "$d/"

				mkdir -p "includes"
				cp -f "$dev_dir_r/fsbl_$1/app/zynq_fsbl_bsp/ps7_cortexa9_0/include"/* ./includes

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
