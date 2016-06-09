#!/bin/bash

set -e

if [ -z ${setup_env+x} ]; then
	echo -e "\e[34m Sourcing setup_env.sh.. \e[39m"
	source ../setup_env.sh
fi

# make sure to be in the same directory as this script #########################
script_dir_abs=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
cd "${script_dir_abs}"

pushd $build_dir_r






# Functions definitions ########################################################

abort() {
	echo -e "\e[91m Error in `basename "$0"`\e[39m"
    exit 1
}

trap 'abort' 0


echo -e "\e[92m *** START `basename "$0"` *** \e[39m"
echo -e "\e[34m Create boot.bif for BOOT.bin.. \e[39m"

cat <<EOF > ./boot.bif
the_ROM_image:
{
	[bootloader]fsbl.elf
	u-boot.elf

}
EOF

echo -e "\e[34m create uEnv.txt for uboot.. \e[39m"

cat <<EOF > ./uEnv.txt
bootargs=console=ttyPS0,115200 root=/dev/mmcblk0p2 rw earlyprintk rootfstype=ext4 rootwait devtmpfs.mount=0
load_image=fatload mmc 0 \${kernel_load_address} \${kernel_image} && fatload mmc 0 \${devicetree_load_address} \${devicetree_image}
uenvcmd=run mmc_loadbit_fat && echo Copying Linux from SD to RAM... && mmcinfo &&  run load_image && bootm \${kernel_load_address} - \${devicetree_load_address}
EOF

hdf_name="$1"

echo -e "\e[34m Get devicetree.dtb of $hdf_name.. \e[39m"
cp $dev_dir_r/dtb_$hdf_name/dts/devicetree.dtb ./
echo -e "\e[34m Get fsbl.elf of $hdf_name.. \e[39m"
cp $dev_dir_r/fsbl_$hdf_name/app/fsbl.elf ./
echo -e "\e[34m Get .bit of $hdf_name.. \e[39m"
cp $dev_dir_r/dtb_$hdf_name/$hdf_name.bit ./7z020.bit


bootgen -image boot.bif -o BOOT.bin -w on


trap : 0
echo -e "\e[92m *** DONE `basename "$0"` *** \e[39m"

