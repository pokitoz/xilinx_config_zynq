#!/bin/bash

set -e


abort() {
	echo -e "\e[91m Error in setup_env.sh\e[39m"
    exit 1
}

trap 'abort' 0


if [ -z ${setup_env+x} ]; then


	
	c_white="\e[39m"
	c_green="\e[92m"
	c_orange="\e[93m"
	c_blue="\e[94m"
	c_red="\e[91m"

	#RED
	export c_error=$c_red
	#GREEN
	export c_good=$c_green
	#BLUE
	export c_info=$c_blue
	#ORANGE
	export c_orange=$c_orange
	#default: white
	export c_default=$c_white


	echoerr() {
	    echo -e "$c_error ${@} $c_default"
	}

	echowarn(){
	    echo -e "$c_warning ${@} $c_default"
	}

	echogood(){
	    echo -e "$c_good ${@} $c_default"
	}

	echoinfo(){
	    echo -e "$c_info ${@} $c_default"
	}

	echodef(){
	    echo -e "$c_default ${@} $c_default"
	}



	indent_c_code(){
	
		set +e
	
		for source_c_name in *.c; do
			echowarn "Formating c file $source_c_name"
			#indent "$indent_command_flags" ./$source_c_name
		done
	
		for header_c_name in *.h; do
			echowarn "Formating h file $header_c_name"
			#indent "$indent_command_flags" ./$header_c_name
		done

		set -e

	}

	pushd_silent() {
		command pushd "$@" > /dev/null
	}

	popd_silent() {
		command popd "$@" > /dev/null
	}


	export -f echoerr
	export -f echowarn
	export -f echogood
	export -f echoinfo
	export -f echodef
	export -f pushd_silent
	export -f popd_silent
	export -f indent_c_code

	

	echoinfo " Sourcing setup_env.sh.. $c_default"

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
	export results_dir="results"


	#export kernel_dir="linux-xlnx-official"
	#export kernel_dir="linux-xlnx-diligent"
	export kernel_dir="linux-xlnx-cd-myir"

	echo -e "\e[34m Find absolute directory paths.. \e[39m"

	export tools_dir_r="$PWD/$tools_dir"
	export build_dir_r="$PWD/$build_dir"
	export dev_dir_r="$PWD/$dev_dir"
	export linux_dir_r="$PWD/$linux_dir"
	export kernel_dir_r="$linux_dir_r/$kernel_dir"
	export results_dir_r="$PWD/$results_dir"

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
	export OPTIONS_MENU="Make_all Clean_build Make_kernel Make_Uboot Make_dtb Make_fsbl Make_bootbin Make_applications Send_applications Push_to_sd_card Get_Results Quit"
	export OPTIONS_MENU_KERNEL="Use_default_config Use_custom_config Quit"


	#SSH
	export env_sshpassword="1234"
	export sshaddress="10.42.0.2"
	export sshlogin="root"
	export sshcommand="$sshlogin@$sshaddress"



fi

trap : 0
echo -e "\e[92m *** DONE setup_env.sh *** \n\e[39m"
