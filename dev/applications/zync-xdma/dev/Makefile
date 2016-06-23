ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m  := xdma.o
ccflags-y := -std=gnu89 -g -Wall -Wno-unused-function -Wno-declaration-after-statement

else
# normal makefile
KDIR ?= /home/fdepraz/Desktop/git/fpga_soc_project_xilinx/xilinx_config_zynq/linux/linux-xlnx-cd-myir

all:
	$(MAKE) -C $(KDIR) M=$$PWD ARCH=arm CROSS_COMPILE=arm-xilinx-linux-gnueabi-

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order Module.symvers


endif
