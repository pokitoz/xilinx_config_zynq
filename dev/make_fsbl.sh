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


hdf_path="$1"
basename_hdf=`basename $1 .hdf`
dir_for_new_app="fsbl_$basename_hdf"

mkdir -p $dir_for_new_app
cp $hdf_path "./$dir_for_new_app"
pushd $dir_for_new_app
if [ ! -f "/usr/bin/gmake" ]; then
	echo -e "\e[34m Need to have an alias of gmake pointing to make.. \e[39m" 
	sudo ln -s /usr/bin/make /usr/bin/gmake
fi

echo -e "\e[34m Creating fsbl for $hdf_path.. \e[39m" 
mkdir -p "app"
echo "open_hw_design $basename_hdf.hdf; generate_app -hw $basename_hdf -os standalone -proc ps7_cortexa9_0 -app zynq_fsbl -compile -sw fsbl -dir app" | hsi
echo -e "\e[34m Renaming of executable.elf to fsbl.elf.. \e[39m" 
mv -f ./app/executable.elf ./app/fsbl.elf

popd

popd



trap : 0
echo -e "\e[92m *** DONE `basename "$0"` *** \e[39m"
