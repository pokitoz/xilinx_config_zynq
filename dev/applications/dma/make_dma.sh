#!/bin/bash



#!/bin/bash

if [ -z ${setup_env+x} ]; then
	echo -e "$c_info Sourcing setup_env.sh.. $c_default"
	source ../../../setup_env.sh
fi

pushd $applications_dir_r/dma

	make -C $kernel_dir_r ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi- M=`pwd`
	gcc application_dma.c

popd


