#!/bin/bash

if [ -z ${setup_env+x} ]; then
	echo -e "$c_info Sourcing setup_env.sh.. $c_default"
	source ../../../setup_env.sh
fi

pushd $applications_dir_r/zynq-xdma/dev
make clean
export KDIR"=$kernel_dir_r"
make
popd

#pushd $applications_dir_r/zynq-xdma/lib
#make
#make install
#popd

pushd $applications_dir_r/zynq-xdma/demo
make clean
make 
popd
