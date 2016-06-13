#!/bin/bash

set -e

if [ -z ${setup_env+x} ]; then
	echo -e "$c_info Sourcing setup_env.sh.. $c_default"
	source ./setup_env.sh
fi

# make sure to be in the same directory as this script #########################
script_dir_abs=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd "${script_dir_abs}"



# Functions definitions ########################################################

abort() {
	echo -e "$c_error Error in  `basename "$0"`$c_default"
    exit 1
}

trap 'abort' 0

echo -e "$c_good *** START `basename "$0"` *** $c_default"
#Copy files to sd card

echo -e "$c_info Clean $build_dir_r $c_default"
pushd $build_dir_r
rm -f ./*.bit
rm -f ./*.bif
rm -f ./*.bin
rm -f ./*.dtb
rm -f ./*.elf
rm -f ./*.txt
rm -f ./uImage
rm -f ./*.dtb	
popd

echo -e "$c_info Clean $dev_dir_r $c_default"
pushd $dev_dir_r

popd


trap : 0
echo -e "$c_good *** DONE `basename "$0"` *** $c_default"








