#!/bin/bash

set -e


abort() {
	echo -e "\e[91m Error in setup_env.sh\e[39m"
    exit 1
}

trap 'abort' 0

# make sure to be in the same directory as this script #########################
script_dir_abs=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd "${script_dir_abs}"

export PATH=$PATH:$PWD/tools/CodeSourcery/Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux/bin
echo $PATH

export CROSS_COMPILE=arm-xilinx-linux-gnueabi-
export ARCH=arm


echo -e "\e[34m Sourcing /opt/Xilinx/SDK/2016.2/settings64.sh.. \e[39m"
source /opt/Xilinx/SDK/2016.2/settings64.sh


export setup_env="1"

export projects_dir="projects"
export applications_dir="applications"
export tools_dir="tools"


export dev_dir="dev"
export linux_dir="linux"
export build_dir="build"
export preset_dir="preset"



#export kernel_dir="linux-xlnx-official"
#export kernel_dir="linux-xlnx-diligent"
export kernel_dir="linux-xlnx-cd-myir"

echo -e "\e[34m Find absolute directory paths.. \e[39m"

export tools_dir_r="$PWD/$tools_dir"
export build_dir_r="$PWD/$build_dir"
export dev_dir_r="$PWD/$dev_dir"
export linux_dir_r="$PWD/$linux_dir"
export kernel_dir_r="$linux_dir_r/$kernel_dir"


export projects_dir_r="$PWD/$projects_dir"
export applications_dir_r="$dev_dir_r/$applications_dir"
export preset_dir_r="$PWD/$preset_dir"

export KDIR="$kernel_dir_r"


#export env_def_config_kernel="zynq_zturn_defconfig"
#export env_def_config_kernel="xilinx_zynq_defconfig"
#export env_def_config_kernel="zynq_custom_defconfig"
#export env_def_config_kernel="zynq_preset_custom"

echo -e "\e[34m set default configuration to $env_def_config_kernel.. \e[39m"

export uboot_configuration="zynq_zturn_config"
echo -e "\e[34m set uboot configuration to $uboot_configuration.. \e[39m"

export KDIR="$kernel_dir_r"
echo -e "\e[34m Set KDIR=$kernel_dir_r\e[39m"
export ARCH=arm
echo -e "\e[34m Set ARCH=$ARCH\e[39m"
export CROSS_COMPILE=arm-xilinx-linux-gnueabi-
echo -e "\e[34m Set CROSS_COMPILE=$CROSS_COMPILE\e[39m"

#OPTIONS MENU MAKE ALL
export OPTIONS_MENU="Make_all Clean_build Make_kernel Make_Uboot Make_dtb Make_fsbl Make_bootbin Make_applications Push_to_sd_card Quit"
export OPTIONS_MENU_KERNEL="Use_default_config Use_custom_config Quit"

#RED
export c_error="\e[91m"
#GREEN
export c_good="\e[92m"
#BLUE
export c_info="\e[94m"
#ORANGE
export c_orange="\e[0;33m"
#default: white
export c_default="\e[39m"



trap : 0
echo -e "\e[92m *** DONE setup_env.sh *** \n\e[39m"
