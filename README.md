# xilinx_config_zynq


Those scripts generates a complete system for a **Myir** board.
- Linux
- Config of the FPGA


*Please download:*
	https://www.dropbox.com/s/0zn56diit0mqh2c/xilinx_source.tar.gz

and put it to the begining of the folder hierarchy.


#Run *./create_hierarchy.sh*
- Creates all the folders
- Download (if not already) the "xilinx_source.tar.gz" archive
- Uncompress and send the source to the proper folders
- Make all the script executable (chmod)

	./
	* build
		* make_bootbin.sh
	* clean_files.sh
	* copy_to_sd_card.sh
	* create_hierachy.sh
	* dev
		* applications
		* make_dtb.sh
		* make_fsbl.sh
		* projects
	* linux
		* linux-xlnx
		* linux-xlnx.tar.bz2
		* make_kernel.sh
		* make_uboot.sh
		* u-boot-xlnx
		* u-boot-xlnx.tar.bz2
	* make_all.sh
	* part_sd_card.sh
	* preset
		* zynq_preset.tcl
	* README.md
	* sd_write_image.sh
	* setup_env.sh
	* tools
		* CodeSourcery
		* Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux.tar.bz2
	* xilinx_source
		* filesystem
		* linux
		* tools
	* xilinx_source.tar.gz



#Source ./setup_env.sh
To have all the necessary commands/variables


#./part_sd_card.sh <SD CARD ABSOLUTE PATH> <1 or 0>
This script will create the partitions needed to run linux on the SD card
You need to specify the path of the SD card: /dev/sdc
- This script will create a partition vfat32 (named boot) and a partition ext4 (named rootfs)
- If the second argument is 1, it calls ./sd_write_image.sh <SD CARD ABSOLUTE PATH>


#./sd_write_image.sh <SD CARD ABSOLUTE PATH>
This script will copy the linux image located in ./filesystem/xillinux-1.3.img.gz to the SD card specified.
All file in the SD card will be removed and the default configuration will be set

#./clean_file.sh
Remove all the generated files

#./copy_to_sd_card.sh <SD CARD ABSOLUTE PATH>
Copy all necessary files to the SD card.

#./make_all.sh <Path to the hdf file> <sd card absolute path>





















Will become a heading
==============

Will become a sub heading
--------------

*This will be Italic*

**This will be Bold**

- This will be a list item
- This will be a list item

    Add a indent and this will end up as code
