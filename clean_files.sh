#!/bin/bash

set -e

if [ -z ${setup_env+x} ]; then
	echo -e "\e[34m Sourcing setup_env.sh.. \e[39m"
	source ./setup_env.sh
fi

# make sure to be in the same directory as this script #########################
script_dir_abs=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd "${script_dir_abs}"



# Functions definitions ########################################################

abort() {
	echo -e "\e[91m Error in `basename "$0"`\e[39m"
    exit 1
}

trap 'abort' 0

echo -e "\e[92m *** START `basename "$0"` *** \e[39m"
#Copy files to sd card



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

pushd $dev_dir_r

popd


trap : 0
echo -e "\e[92m *** DONE `basename "$0"` *** \e[39m"








