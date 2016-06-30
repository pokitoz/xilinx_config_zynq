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

echo -e "$c_good *** START `basename "$0"` *** $c_default"

pushd $applications_dir_r


	echo -e "$c_info Sending dma/ folder to $sshcommand:~/Desktop $c_default"
	sshpass -p "$env_sshpassword" scp -r dma/ root@10.42.0.2:~/Desktop
	echo -e "$c_info Sending irq_interrupt/ folder to $sshcommand:~/Desktop $c_default"
	sshpass -p "$env_sshpassword" scp -r irq_interrupt/ $sshcommand:~/Desktop
	echo -e "$c_info Sending bitmap/ folder to $sshcommand:~/Desktop $c_default"
	sshpass -p "$env_sshpassword" scp -r bitmap/ $sshcommand:~/Desktop
	echo -e "$c_info Sending convertor_interface/ folder to $sshcommand:~/Desktop $c_default"
	sshpass -p "$env_sshpassword" scp -r convertor_interface/ $sshcommand:~/Desktop
	echo -e "$c_info Sending convertor_yuv_rgb/ folder to $sshcommand:~/Desktop $c_default"
	sshpass -p "$env_sshpassword" scp -r convertor_yuv_rgb/ $sshcommand:~/Desktop
	echo -e "$c_info Sending yuv_generator/ folder to $sshcommand:~/Desktop $c_default"
	sshpass -p "$env_sshpassword" scp -r yuv_generator/ $sshcommand:~/Desktop


	sshpass -p "$env_sshpassword" scp -r *.sh $sshcommand:~/Desktop


popd


popd

trap : 0
echo -e "$c_good *** DONE `basename "$0"` *** $c_default"
