proc getPresetInfo {} {
  return [dict create name {zynq7_config} description {zynq7_config}  vlnv xilinx.com:ip:processing_system7:5.5 display_name {zynq7_config} ]
}

proc validate_preset {IPINST} { return true }


proc apply_preset {IPINST} {
return [dict create \
    CONFIG.PCW_DDR_RAM_HIGHADDR {0x3FFFFFFF}  \
    CONFIG.PCW_UIPARAM_DDR_ROW_ADDR_COUNT {15}  \
    CONFIG.PCW_UIPARAM_DDR_T_RC {48.91}  \
    CONFIG.PCW_UIPARAM_DDR_T_FAW {40.0}  \
    CONFIG.PCW_UIPARAM_DDR_DQS_TO_CLK_DELAY_0 {0.229}  \
    CONFIG.PCW_UIPARAM_DDR_DQS_TO_CLK_DELAY_1 {0.250}  \
    CONFIG.PCW_UIPARAM_DDR_DQS_TO_CLK_DELAY_2 {0.121}  \
    CONFIG.PCW_UIPARAM_DDR_DQS_TO_CLK_DELAY_3 {0.146}  \
    CONFIG.PCW_UIPARAM_DDR_BOARD_DELAY0 {0.271}  \
    CONFIG.PCW_UIPARAM_DDR_BOARD_DELAY1 {0.259}  \
    CONFIG.PCW_UIPARAM_DDR_BOARD_DELAY2 {0.219}  \
    CONFIG.PCW_UIPARAM_DDR_BOARD_DELAY3 {0.207}  \
    CONFIG.PCW_CRYSTAL_PERIPHERAL_FREQMHZ {33.3333333}  \
    CONFIG.PCW_APU_PERIPHERAL_FREQMHZ {600}  \
    CONFIG.PCW_FPGA0_PERIPHERAL_FREQMHZ {100}  \
    CONFIG.PCW_ACT_APU_PERIPHERAL_FREQMHZ {600.000000}  \
    CONFIG.PCW_ACT_SDIO_PERIPHERAL_FREQMHZ {100.000000}  \
    CONFIG.PCW_ACT_UART_PERIPHERAL_FREQMHZ {100.000000}  \
    CONFIG.PCW_ACT_WDT_PERIPHERAL_FREQMHZ {100.000000}  \
    CONFIG.PCW_ACT_FPGA0_PERIPHERAL_FREQMHZ {100.000000}  \
    CONFIG.PCW_ACT_TTC0_CLK0_PERIPHERAL_FREQMHZ {100.000000}  \
    CONFIG.PCW_ACT_TTC0_CLK1_PERIPHERAL_FREQMHZ {100.000000}  \
    CONFIG.PCW_ACT_TTC0_CLK2_PERIPHERAL_FREQMHZ {100.000000}  \
    CONFIG.PCW_ACT_TTC1_CLK0_PERIPHERAL_FREQMHZ {100.000000}  \
    CONFIG.PCW_ACT_TTC1_CLK1_PERIPHERAL_FREQMHZ {100.000000}  \
    CONFIG.PCW_ACT_TTC1_CLK2_PERIPHERAL_FREQMHZ {100.000000}  \
    CONFIG.PCW_CLK0_FREQ {100000000}  \
    CONFIG.PCW_SDIO_PERIPHERAL_DIVISOR0 {16}  \
    CONFIG.PCW_UART_PERIPHERAL_DIVISOR0 {16}  \
    CONFIG.PCW_FCLK0_PERIPHERAL_DIVISOR0 {4}  \
    CONFIG.PCW_ARMPLL_CTRL_FBDIV {36}  \
    CONFIG.PCW_CPU_CPU_PLL_FREQMHZ {1200.000}  \
    CONFIG.PCW_SDIO_PERIPHERAL_VALID {1}  \
    CONFIG.PCW_UART_PERIPHERAL_VALID {1}  \
    CONFIG.PCW_M_AXI_GP0_FREQMHZ {100}  \
    CONFIG.PCW_EN_SDIO0 {1}  \
    CONFIG.PCW_EN_UART1 {1}  \
    CONFIG.PCW_PRESET_BANK1_VOLTAGE {LVCMOS 1.8V}  \
    CONFIG.PCW_UIPARAM_DDR_PARTNO {MT41J256M16 RE-125}  \
    CONFIG.PCW_UIPARAM_DDR_DRAM_WIDTH {16 Bits}  \
    CONFIG.PCW_UIPARAM_DDR_DEVICE_CAPACITY {4096 MBits}  \
    CONFIG.PCW_SD0_PERIPHERAL_ENABLE {1}  \
    CONFIG.PCW_SD0_SD0_IO {MIO 40 .. 45}  \
    CONFIG.PCW_SD0_GRP_CD_ENABLE {1}  \
    CONFIG.PCW_SD0_GRP_CD_IO {MIO 46}  \
    CONFIG.PCW_SD0_GRP_WP_ENABLE {1}  \
    CONFIG.PCW_SD0_GRP_WP_IO {MIO 47}  \
    CONFIG.PCW_UART1_PERIPHERAL_ENABLE {1}  \
    CONFIG.PCW_UART1_UART1_IO {MIO 48 .. 49}  \
    CONFIG.PCW_GPIO_PERIPHERAL_ENABLE {1}  \
    CONFIG.PCW_ENET0_PERIPHERAL_CLKSRC {ARM PLL}  \
    CONFIG.PCW_MIO_40_PULLUP {enabled}  \
    CONFIG.PCW_MIO_40_IOTYPE {LVCMOS 1.8V}  \
    CONFIG.PCW_MIO_40_DIRECTION {inout}  \
    CONFIG.PCW_MIO_40_SLEW {slow}  \
    CONFIG.PCW_MIO_41_PULLUP {enabled}  \
    CONFIG.PCW_MIO_41_IOTYPE {LVCMOS 1.8V}  \
    CONFIG.PCW_MIO_41_DIRECTION {inout}  \
    CONFIG.PCW_MIO_41_SLEW {slow}  \
    CONFIG.PCW_MIO_42_PULLUP {enabled}  \
    CONFIG.PCW_MIO_42_IOTYPE {LVCMOS 1.8V}  \
    CONFIG.PCW_MIO_42_DIRECTION {inout}  \
    CONFIG.PCW_MIO_42_SLEW {slow}  \
    CONFIG.PCW_MIO_43_PULLUP {enabled}  \
    CONFIG.PCW_MIO_43_IOTYPE {LVCMOS 1.8V}  \
    CONFIG.PCW_MIO_43_DIRECTION {inout}  \
    CONFIG.PCW_MIO_43_SLEW {slow}  \
    CONFIG.PCW_MIO_44_PULLUP {enabled}  \
    CONFIG.PCW_MIO_44_IOTYPE {LVCMOS 1.8V}  \
    CONFIG.PCW_MIO_44_DIRECTION {inout}  \
    CONFIG.PCW_MIO_44_SLEW {slow}  \
    CONFIG.PCW_MIO_45_PULLUP {enabled}  \
    CONFIG.PCW_MIO_45_IOTYPE {LVCMOS 1.8V}  \
    CONFIG.PCW_MIO_45_DIRECTION {inout}  \
    CONFIG.PCW_MIO_45_SLEW {slow}  \
    CONFIG.PCW_MIO_46_PULLUP {disabled}  \
    CONFIG.PCW_MIO_46_IOTYPE {LVCMOS 1.8V}  \
    CONFIG.PCW_MIO_46_DIRECTION {in}  \
    CONFIG.PCW_MIO_46_SLEW {slow}  \
    CONFIG.PCW_MIO_47_PULLUP {disabled}  \
    CONFIG.PCW_MIO_47_IOTYPE {LVCMOS 1.8V}  \
    CONFIG.PCW_MIO_47_DIRECTION {in}  \
    CONFIG.PCW_MIO_47_SLEW {slow}  \
    CONFIG.PCW_MIO_48_PULLUP {enabled}  \
    CONFIG.PCW_MIO_48_IOTYPE {LVCMOS 1.8V}  \
    CONFIG.PCW_MIO_48_DIRECTION {out}  \
    CONFIG.PCW_MIO_48_SLEW {slow}  \
    CONFIG.PCW_MIO_49_PULLUP {enabled}  \
    CONFIG.PCW_MIO_49_IOTYPE {LVCMOS 1.8V}  \
    CONFIG.PCW_MIO_49_DIRECTION {in}  \
    CONFIG.PCW_MIO_49_SLEW {slow}  \
    CONFIG.PCW_MIO_TREE_PERIPHERALS {unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#SD 0#SD 0#SD 0#SD 0#SD 0#SD 0#SD 0#SD 0#UART 1#UART 1#unassigned#unassigned#unassigned#unassigned}  \
    CONFIG.PCW_MIO_TREE_SIGNALS {unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#unassigned#clk#cmd#data[0]#data[1]#data[2]#data[3]#cd#wp#tx#rx#unassigned#unassigned#unassigned#unassigned}  \
]
}


