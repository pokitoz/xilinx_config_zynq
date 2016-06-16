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


hdf_path="$1"
basename_hdf=`basename $1 .hdf`
dir_for_new_app="fsbl_$basename_hdf"

mkdir -p $dir_for_new_app
cp $hdf_path "./$dir_for_new_app"
pushd $dir_for_new_app
if [ ! -f "/usr/bin/gmake" ]; then
	echo -e "$c_info Need to have an alias of gmake pointing to make.. $c_default" 
	sudo ln -s /usr/bin/make /usr/bin/gmake
fi

echo -e "$c_info Creating fsbl for $hdf_path.. $c_default" 
mkdir -p "app"

pushd app
	if [ -f "Makefile" ]; then
		make clean
	fi
popd

echo "open_hw_design $basename_hdf.hdf; generate_app -hw $basename_hdf -os standalone -proc ps7_cortexa9_0 -app zynq_fsbl -compile -sw fsbl -dir app" | hsi
echo -e "$c_info Renaming of executable.elf to fsbl.elf.. $c_default" 
mv -f ./app/executable.elf ./app/fsbl.elf

popd

popd



trap : 0
echo -e "$c_good *** DONE `basename "$0"` *** $c_default"
