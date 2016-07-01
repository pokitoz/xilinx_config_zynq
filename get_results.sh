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

mkdir -p $results_dir_r

pushd $results_dir_r

	echo -e "$c_info Getting bmp $sshcommand:~/Desktop/convertor_interface/out.bmp $c_default"
	sshpass -p "$env_sshpassword" scp -r $sshcommand:~/Desktop/convertor_interface/*.bmp ./

popd


popd

trap : 0
echo -e "$c_good *** DONE `basename "$0"` *** $c_default"
