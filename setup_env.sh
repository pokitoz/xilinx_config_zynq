#!/bin/bash

# make sure to be in the same directory as this script #########################
script_dir_abs=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd "${script_dir_abs}"

export PATH=$PATH:$PWD/tools/CodeSourcery/Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux/bin
echo $PATH

export CROSS_COMPILE=arm-xilinx-linux-gnueabi-
export ARCH=arm

source /opt/Xilinx/SDK/2016.2/settings64.sh


export setup_env="1"

export projects_dir="projects"
export applications_dir="applications"
export tools_dir="tools"

export dev_dir="dev"
export linux_dir="linux"
export build_dir="build"

export tools_dir_r="$PWD/$tools_dir"
export build_dir_r="$PWD/$build_dir"
export dev_dir_r="$PWD/$dev_dir"
export linux_dir_r="$PWD/$linux_dir"

export projects_dir_r="$PWD/$projects_dir"
export applications_dir_r="$dev_dir_r/$applications_dir"
