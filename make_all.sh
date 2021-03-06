#!/bin/bash

#Trap in case of error
set -e
#Print all commands executed.. Only for debug
#set -o verbose

if [ -z ${setup_env+x} ]; then
	echo -e "$c_info Sourcing setup_env.sh.. $c_default"
	source ./setup_env.sh
fi

# make sure to be in the same directory as this script #########################
script_dir_abs=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd "${script_dir_abs}"



# trap ctrl-c and call ctrl_c()
trap ctrl_c INT


# Functions definitions ########################################################

print_info(){
	echo -e ""
	echo -e ""
	
	echo -e "$c_info Check ./build/make_bootbin.sh to get the baud rate for the UART (should be 115200)$c_default"
	echo -e "$c_info Use 'minicom -c on' or 'miniterm.py' to use the UART-USB$c_default"
	echo -e "$c_info You can type ssh $sshcommand (password root) $c_default"
	echo -e "$c_info If you don't have ssh working: dpkg -i *.deb in Desktop/pkg $c_default"
	echo -e "$c_info \t Or you can type sudo apt-get install openssh-server $c_default"	
	date
}

function ctrl_c() {
	print_info
  	echo -e "$c_good \nExited by user with CTRL+C $c_default"
	echo -e "$c_good *** DONE `basename "$0"` *** $c_default"
	trap : 0
	exit 0
}

abort() {
	echo -e "$c_error Error in `basename "$0"`$c_default"
    exit 1
}


function hcenter {

  text="$1"

  cols=`tput cols`

  IFS=$'\n'$'\r'
  for line in $(echo -e $text); do

    line_length=`echo $line| wc -c`
    half_of_line_length=`expr $line_length / 2`
    center=`expr \( $cols / 2 \) - $half_of_line_length`

    spaces=""
    for ((i=0; i < $center; i++)) {
      spaces="$spaces "
    }

    echo -e "$spaces$line"

  done

}



function asking_to_do {
	ACTION=$1
	COMMAND=$2
	echo -e "\e[0;33m"
  read -r -s -n 1 -p "Do $ACTION ? [y,n] : " doit
  case $doit in
    y|Y) echo -e "$c_orange doing $ACTION $c_default" && $COMMAND;;
    n|N) echo -e "$c_orange passing $ACTION $c_default" ;;
    *) echo -e "$c_orange bad option $c_orange" && asking_to_do $ACTION $COMMAND ;;
  esac
}

trap 'abort' 0

echo -e "$c_good *** START `basename "$0"` *** $c_default"
#Copy files to sd card

if [ -z "$1" ]; then
	echo -e "$c_error You need to specify a HDF location$c_default"
	exit 1
fi



$preset_dir_r/make_preset.sh

hdf_location="$1"
sdcard_abs="$2"

#if [ "$3" = "y" ]
#then
#	./clean_files.sh
#else
#	asking_to_do "clean files" ./clean_files.sh
#fi
#./clean_files.sh

abs_path_hdf=`readlink -f $hdf_location`
hdf_name_only=`basename "$abs_path_hdf" .hdf`

#echo -e ""
#echo -e "$c_orange \t\t\t --------------------- $c_default"
#echo -e "$c_orange \t\t\t|    Menu MakeAll     |$c_default"
#echo -e "$c_orange \t\t\t --------------------- $c_default"


old_IFS=$IFS
echo -e ""
hcenter "$c_orange --------------------- $c_default"
hcenter "$c_orange Menu_MakeAll$c_default"
hcenter "$c_orange --------------------- $c_default"
echo -e ""
echo -e ""


PS3="Menu #?> "
IFS=$old_IFS


select opt in $OPTIONS_MENU; do
		
		if [ "$opt" = "Quit" ]; then
		 echo "Exit menu"
		 break
		elif [ "$opt" = "Make_all" ]; then
			#./clean_files.sh
			#$linux_dir_r/make_kernel.sh
			$linux_dir_r/make_uboot.sh
			$dev_dir_r/make_dtb.sh $abs_path_hdf
			$dev_dir_r/make_fsbl.sh $abs_path_hdf
			$build_dir_r/make_bootbin.sh $hdf_name_only
			$dev_dir_r/make_applications.sh $hdf_name_only
			./copy_to_sd_card.sh $sdcard_abs
		elif [ "$opt" = "Clean_build" ]; then
			./clean_files.sh
		elif [ "$opt" = "Make_kernel" ]; then
			select opt_s in $OPTIONS_MENU_KERNEL; do
				if [ "$opt_s" = "Quit" ]; then
				 echo "Exit sub menu"
				 break
			 elif [ "$opt_s" = "Use_default_config" ]; then
					$linux_dir_r/make_kernel.sh
				elif [ "$opt_s" = "Use_custom_config" ]; then
					$linux_dir_r/make_kernel.sh zynq_custom_defconfig
				else
				 echo "Bad option"
				fi
			done
		elif [ "$opt" = "Make_Uboot" ]; then
			$linux_dir_r/make_uboot.sh
		elif [ "$opt" = "Make_dtb" ]; then
			$dev_dir_r/make_dtb.sh $abs_path_hdf
		elif [ "$opt" = "Make_fsbl" ]; then
			$dev_dir_r/make_fsbl.sh $abs_path_hdf
		elif [ "$opt" = "Make_bootbin" ]; then
			$build_dir_r/make_bootbin.sh $hdf_name_only
		elif [ "$opt" = "Make_applications" ]; then
			$dev_dir_r/make_applications.sh $hdf_name_only
		elif [ "$opt" = "Send_applications" ]; then
			$dev_dir_r/send_applications.sh $hdf_name_only
		elif [ "$opt" = "Push_to_sd_card" ]; then
			./copy_to_sd_card.sh $sdcard_abs
		elif [ "$opt" = "Get_Results" ]; then
			./get_results.sh
		else
		 echo "Bad option"
		fi

		echo -e ""
done


trap : 0
print_info
echo -e "$c_good \nExited by user $c_default"
echo -e "$c_good *** DONE `basename "$0"` *** $c_default"
