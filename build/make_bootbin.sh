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


linux_kernel_mem_bootarg='1018M'

echo -e "\e[34m create uEnv.txt for uboot.. \e[39m"

cat <<EOF > ./uEnv.txt

bootenv=uEnv.txt

# Time before booting
bootdelay=3

# BITSTREAM
bitstream_image=7z020.bit
loadbit_addr=0x100000

# BOOT
boot_image=BOOT.bin
boot_size=0x080000

#DEVICE TREE
devicetree_image=devicetree.dtb
devicetree_load_address=0x2000000
devicetree_size=0x010000

# KERNEL
kernel_image=uImage
kernel_load_address=0x2080000
kernel_size=0x480000

# RAMDISK
ramdisk_image=uramdisk.image.gz
ramdisk_load_address=0x4000000
ramdisk_size=0x600000


bootargs=console=ttyPS0,115200 root=/dev/mmcblk0p2 mem=${linux_kernel_mem_bootarg} rw earlyprintk rootfstype=ext4 rootwait devtmpfs.mount=0

load_image=fatload mmc 0 \${kernel_load_address} \${kernel_image} && fatload mmc 0 \${devicetree_load_address} \${devicetree_image}

uenvcmd=run mmc_loadbit_fat && echo Copying Linux from SD to RAM... && mmcinfo &&  run load_image && bootm \${kernel_load_address} - \${devicetree_load_address}



###############################################
###############################################

baudrate=115200
bootcmd=run $modeboot

ethact=Gem.e000b000
ethaddr=00:0a:35:00:01:22
fdt_high=0x20000000
filesize=3dbb6c
importbootenv=echo Importing environment from SD ...; env import -t \${loadbootenv_addr} $filesize
initrd_high=0x20000000
ipaddr=192.168.1.55
jtagboot=echo TFTPing Linux to RAM... && tftpboot \${kernel_load_address} \${kernel_image} && tftpboot \${devicetree_load_address} \${devicetree_image} && tftpboot \${ramdisk_load_address} \${ramdisk_image} && }

loadbootenv=fatload mmc 0 \${loadbootenv_addr} \${bootenv}
loadbootenv_addr=0x2000000
mmc_loadbit_fat=echo Loading bitstream from SD/MMC/eMMC to RAM.. && get_bitstream_name && mmcinfo && fatload mmc 0 \${loadbit_addr} \${bitstream_image} && fpga loadb 0 \${loadbit_addr} \${filesize}
modeboot=sdboot
nandboot=echo Copying Linux from NAND flash to RAM... && nand read \${kernel_load_address} 0x100000 \${kernel_size} && nand read \${devicetree_load_address} 0x600000 \${devicetree_size} && echo Copying ramdis}
norboot=echo Copying Linux from NOR flash to RAM... && cp.b 0xE2100000 \${kernel_load_address} \${kernel_size} && cp.b 0xE2600000 \${devicetree_load_address} \${devicetree_size} && echo Copying ramdisk... && }
qboot_addr=0x000000
qbootenv_addr=0x080000
qbootenv_size=0x020000
qdevtree_addr=0x980000
qkernel_addr=0x500000
qramdisk_addr=0x990000
qspiboot=echo Copying Linux from QSPI flash to RAM... && sf probe 0 0 0 && qspi_get_bitsize 0x0A0000 && sf read \${loadbit_addr} 0x0A0004 \${bitsize} && fpga loadb 0 \${loadbit_addr} \${bitsize} && sf read \${}
qspiupdate=echo Update qspi images from sd card... && echo - Init mmc... && mmc rescan && echo - Init qspi flash... && sf probe 0 0 0 && echo - Write boot.bin... && fatload mmc 0 0x200000 boot.bin && sf e.

rsa_jtagboot=echo TFTPing Image to RAM... && tftpboot 0x100000 \${boot_image} && zynqrsa 0x100000 && bootm \${kernel_load_address} \${ramdisk_load_address} \${devicetree_load_address}
rsa_nandboot=echo Copying Image from NAND flash to RAM... && nand read 0x100000 0x0 \${boot_size} && zynqrsa 0x100000 && bootm \${kernel_load_address} \${ramdisk_load_address} \${devicetree_load_address}
rsa_norboot=echo Copying Image from NOR flash to RAM... && cp.b 0xE2100000 0x100000 \${boot_size} && zynqrsa 0x100000 && bootm \${kernel_load_address} \${ramdisk_load_address} \${devicetree_load_address}
rsa_qspiboot=echo Copying Image from QSPI flash to RAM... && sf probe 0 0 0 && sf read 0x100000 0x0 \${boot_size} && zynqrsa 0x100000 && bootm \${kernel_load_address} \${ramdisk_load_address} \${devicetree_lo}
rsa_sdboot=echo Copying Image from SD to RAM... && fatload mmc 0 0x100000 \${boot_image} && zynqrsa 0x100000 && bootm \${kernel_load_address} \${ramdisk_load_address} \${devicetree_load_address}
sdboot=if mmcinfo; then run uenvboot; get_bitstream_name && echo - load \${bitname} to PL... && fatload mmc 0 0x200000 \${bitname} && fpga loadb 0 0x200000 \${filesize} && echo Copying Linux from SD to RAM..i
serverip=192.168.1.13
stderr=serial
stdin=serial
stdout=serial
uenvboot=if run loadbootenv; then echo Loaded environment from \${bootenv}; run importbootenv; fi; if test -n $uenvcmd; then echo Running uenvcmd ...; run uenvcmd; fi

usbboot=if usb start; then run uenvboot; echo Copying Linux from USB to RAM... && fatload usb 0 \${kernel_load_address} \${kernel_image} && fatload usb 0 \${devicetree_load_address} \${devicetree_image} && fai


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

