#!/bin/bash

set -e

# make sure to be in the same directory as this script #########################
current_script_dir=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

if [ -z ${setup_env+x} ]; then
	echo -e "$c_info Sourcing setup_env.sh.. $c_default"
	source ../../../setup_env.sh
fi

pushd "${current_script_dir}"



abort() {
	echo -e "\e[91m Error in `basename "$0"`\e[39m"
    exit 1
}

trap 'abort' 0



echo -e "\e[92m *** START `basename "$0"` *** \e[39m"
pushd $applications_dir_r/blinkled
echo -e "\e[34m Compiling blink.out.. \e[39m"
arm-xilinx-linux-gnueabi-gcc blink_led.c -o blink.out

popd



trap : 0
echo -e "\e[92m *** DONE `basename "$0"` *** \e[39m"
