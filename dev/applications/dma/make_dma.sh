#!/bin/bash

set -e

# make sure to be in the same directory as this script #########################
current_script_dir=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

if [ -z ${setup_env+x} ]; then
	echo -e "$c_info Sourcing setup_env.sh.. $c_default"
	source ../../../setup_env.sh
fi

pushd "${current_script_dir}"
	
	
	#Make driver
	pushd ./driver
		rm -rf "*.ko"
		make clean
		make -C $kernel_dir_r ARCH=$ARCH CROSS_COMPILE=$CROSS_COMPILE M=`pwd`
		make clean	
	popd
	
	#Make application
	make
	make clean
	
popd


