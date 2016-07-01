obj-m := pl_axi_dma_driver.o
pl_axi_dma_driver-objs := pl_axi_dma_dev.o ../pl_dma_api.o 
ccflags-y := -std=gnu89 -g -Wall -Wno-unused-function -Wno-declaration-after-statement -I../ -D KERNEL_MODULE_COMPILATION


clean:
	rm -rf *.o *~ core .depend .*.cmd *.mod.c .tmp_versions modules.order Module.symvers ../*.o
