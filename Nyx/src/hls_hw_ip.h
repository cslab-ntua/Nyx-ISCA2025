//------------------------Address Info-------------------
// 0x00 : Control signals
//        bit 0  - ap_start (Read/Write/COH)
//        bit 1  - ap_done (Read)
//        bit 2  - ap_idle (Read)
//        bit 3  - ap_ready (Read/COR)
//        bit 4  - ap_continue (Read/Write/SC)
//        bit 7  - auto_restart (Read/Write)
//        bit 9  - interrupt (Read)
//        others - reserved
// 0x04 : Global Interrupt Enable Register
//        bit 0  - Global Interrupt Enable (Read/Write)
//        others - reserved
// 0x08 : IP Interrupt Enable Register (Read/Write)
//        bit 0 - enable ap_done interrupt (Read/Write)
//        bit 1 - enable ap_ready interrupt (Read/Write)
//        others - reserved
// 0x0c : IP Interrupt Status Register (Read/TOW)
//        bit 0 - ap_done (Read/TOW)
//        bit 1 - ap_ready (Read/TOW)
//        others - reserved
// 0x10 : Data signal of a
//        bit 7~0 - a[7:0] (Read/Write)
//        others  - reserved
// 0x14 : reserved
// 0x18 : Data signal of b
//        bit 7~0 - b[7:0] (Read/Write)
//        others  - reserved
// 0x1c : reserved
// 0x20 : Data signal of c_i
//        bit 7~0 - c_i[7:0] (Read/Write)
//        others  - reserved
// 0x24 : reserved
// 0x28 : Data signal of c_o
//        bit 7~0 - c_o[7:0] (Read)
//        others  - reserved
// 0x2c : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#ifndef HLS_HW_IP_H
#define HLS_HW_IP_H

//32-bit addresses for AXI-Lite
//64-bit addresses for IP inputs 
#define AXILITE_BITS 32
#define AXILITE_BYTES 4

#define ICAP_STATUS	0x0000004010000110
#define ICAP_LENGTH	0x0000004010000108
#define ICAP_ADDR 	0x0000004014000000


#define AXILITE_ADDR_AP_CTRL 0x00
#define AXILITE_ADDR_GIER 0x04
#define AXILITE_ADDR_IER 0x08
#define AXILITE_ADDR_ISR 0x0C
#define AXILITE_ADDR_RESTART_COUNTER 0x10
#define AXILITE_ADDR_DATA_INPUT 0x14
#define AXILITE_ADDR_STALL_INPUT 0x1C
#define AXILITE_ADDR_INPUT_A_LSB 0x14 
#define	AXILITE_ADDR_INPUT_A_MSB 0x18 
#define AXILITE_ADDR_INPUT_B_LSB 0x20 
#define AXILITE_ADDR_INPUT_B_MSB 0x24 

#define RECONFIG_REGIONS 8

#define RECONFIG_REGION_0_AXILITE_ADDR 0x0000004000000000
#define RECONFIG_REGION_1_AXILITE_ADDR 0x0000004000010000
#define RECONFIG_REGION_2_AXILITE_ADDR 0x0000004000020000
#define RECONFIG_REGION_3_AXILITE_ADDR 0x0000004000030000
#define RECONFIG_REGION_4_AXILITE_ADDR 0x0000004000040000
#define RECONFIG_REGION_5_AXILITE_ADDR 0x0000004000050000
#define RECONFIG_REGION_6_AXILITE_ADDR 0x0000004000060000
#define RECONFIG_REGION_7_AXILITE_ADDR 0x0000004000070000

#define RECONFIG_REGION_0_DECOUPLE_ADDR 0x0000004000100000
#define RECONFIG_REGION_1_DECOUPLE_ADDR 0x0000004000110000
#define RECONFIG_REGION_2_DECOUPLE_ADDR 0x0000004000120000
#define RECONFIG_REGION_3_DECOUPLE_ADDR 0x0000004000130000
#define RECONFIG_REGION_4_DECOUPLE_ADDR 0x0000004000140000
#define RECONFIG_REGION_5_DECOUPLE_ADDR 0x0000004000150000
#define RECONFIG_REGION_6_DECOUPLE_ADDR 0x0000004000160000
#define RECONFIG_REGION_7_DECOUPLE_ADDR 0x0000004000170000

#define RECONFIG_REGION_0_VFIFO_CTRL_ADDR 0x0000004000200000
#define RECONFIG_REGION_1_VFIFO_CTRL_ADDR 0x0000004000210000
#define RECONFIG_REGION_2_VFIFO_CTRL_ADDR 0x0000004000220000
#define RECONFIG_REGION_3_VFIFO_CTRL_ADDR 0x0000004000230000
#define RECONFIG_REGION_4_VFIFO_CTRL_ADDR 0x0000004000240000
#define RECONFIG_REGION_5_VFIFO_CTRL_ADDR 0x0000004000250000
#define RECONFIG_REGION_6_VFIFO_CTRL_ADDR 0x0000004000260000
#define RECONFIG_REGION_7_VFIFO_CTRL_ADDR 0x0000004000270000

#define RECONFIG_REGION_0_SRC 0x00
#define RECONFIG_REGION_1_SRC 0x04
#define RECONFIG_REGION_2_SRC 0x08
#define RECONFIG_REGION_3_SRC 0x0C
#define RECONFIG_REGION_4_SRC 0x10
#define RECONFIG_REGION_5_SRC 0x14
#define RECONFIG_REGION_6_SRC 0x18
#define RECONFIG_REGION_7_SRC 0x1C

#define RECONFIG_REGION_0_DEST_0 0x00 //0
#define RECONFIG_REGION_0_DEST_1 0x01 //1

#define RECONFIG_REGION_1_DEST_0 0x04 //4
#define RECONFIG_REGION_1_DEST_1 0x05 //5

#define RECONFIG_REGION_2_DEST_0 0x08 //8
#define RECONFIG_REGION_2_DEST_1 0x09 //9

#define RECONFIG_REGION_3_DEST_0 0x0C //12
#define RECONFIG_REGION_3_DEST_1 0x0D //13

#define RECONFIG_REGION_4_DEST_0 0x10 //16
#define RECONFIG_REGION_4_DEST_1 0x11 //17

#define RECONFIG_REGION_5_DEST_0 0x14 //20
#define RECONFIG_REGION_5_DEST_1 0x15 //21

#define RECONFIG_REGION_6_DEST_0 0x18 //24
#define RECONFIG_REGION_6_DEST_1 0x19 //25

#define RECONFIG_REGION_7_DEST_0 0x1C //28
#define RECONFIG_REGION_7_DEST_1 0x1D //29

#endif




