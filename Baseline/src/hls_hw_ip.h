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
#define	AXILITE_ADDR_INPUT_B_MSB 0x24 
#define	AXILITE_ADDR_INPUT_C_LSB 0x2C 
#define	AXILITE_ADDR_INPUT_C_MSB 0x30 
#define	AXILITE_ADDR_INPUT_D_LSB 0x38 
#define	AXILITE_ADDR_INPUT_D_MSB 0x3C


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

#endif