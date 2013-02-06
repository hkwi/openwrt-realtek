//============================================================
//
// File Name: Hal8192CDMOutSrc.c 
//
// Description:
//
// This file is for 92CE/92CU outsource dynamic mechanism for partner.
//
//
//============================================================

#ifndef _HAL8192CDM_C_
#define _HAL8192CDM_C_

#ifdef __KERNEL__
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/fcntl.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <asm/unistd.h>
#include <linux/synclink.h>
#endif

#include "./8192cd_cfg.h"
#include "./8192cd.h"
#include "./8192cd_hw.h"
#include "./8192cd_headers.h"
#include "./8192cd_debug.h"

#ifdef __KERNEL__
#ifdef __LINUX_2_6__
#include <linux/syscalls.h>
#else
#include <linux/fs.h>
#endif
#endif

//============================================================
// Global var
//============================================================


static unsigned int OFDMSwingTable[] = {
	0x7f8001fe, // 0, +6.0dB
	0x788001e2, // 1, +5.5dB
	0x71c001c7, // 2, +5.0dB
	0x6b8001ae, // 3, +4.5dB
	0x65400195, // 4, +4.0dB
	0x5fc0017f, // 5, +3.5dB
	0x5a400169, // 6, +3.0dB
	0x55400155, // 7, +2.5dB
	0x50800142, // 8, +2.0dB
	0x4c000130, // 9, +1.5dB
	0x47c0011f, // 10, +1.0dB
	0x43c0010f, // 11, +0.5dB
	0x40000100, // 12, +0dB
	0x3c8000f2, // 13, -0.5dB
	0x390000e4, // 14, -1.0dB
	0x35c000d7, // 15, -1.5dB
	0x32c000cb, // 16, -2.0dB
	0x300000c0, // 17, -2.5dB
	0x2d4000b5, // 18, -3.0dB
	0x2ac000ab, // 19, -3.5dB
	0x288000a2, // 20, -4.0dB
	0x26000098, // 21, -4.5dB
	0x24000090, // 22, -5.0dB
	0x22000088, // 23, -5.5dB
	0x20000080, // 24, -6.0dB
	0x1e400079, // 25, -6.5dB
	0x1c800072, // 26, -7.0dB
	0x1b00006c, // 27. -7.5dB
	0x19800066, // 28, -8.0dB
	0x18000060, // 29, -8.5dB
	0x16c0005b, // 30, -9.0dB
	0x15800056, // 31, -9.5dB
	0x14400051, // 32, -10.0dB
	0x1300004c, // 33, -10.5dB
	0x12000048, // 34, -11.0dB
	0x11000044, // 35, -11.5dB
	0x10000040, // 36, -12.0dB
};

unsigned int TxPwrTrk_OFDM_SwingTbl[TxPwrTrk_OFDM_SwingTbl_Len] = {
	/*  +6.0dB */ 0x7f8001fe,
	/*  +5.5dB */ 0x788001e2,
	/*  +5.0dB */ 0x71c001c7,
	/*  +4.5dB */ 0x6b8001ae,
	/*  +4.0dB */ 0x65400195,
	/*  +3.5dB */ 0x5fc0017f,
	/*  +3.0dB */ 0x5a400169,
	/*  +2.5dB */ 0x55400155,
	/*  +2.0dB */ 0x50800142,
	/*  +1.5dB */ 0x4c000130,
	/*  +1.0dB */ 0x47c0011f,
	/*  +0.5dB */ 0x43c0010f,
	/*   0.0dB */ 0x40000100,
	/*  -0.5dB */ 0x3c8000f2,
	/*  -1.0dB */ 0x390000e4,
	/*  -1.5dB */ 0x35c000d7,
	/*  -2.0dB */ 0x32c000cb,
	/*  -2.5dB */ 0x300000c0,
	/*  -3.0dB */ 0x2d4000b5,
	/*  -3.5dB */ 0x2ac000ab,
	/*  -4.0dB */ 0x288000a2,
	/*  -4.5dB */ 0x26000098,
	/*  -5.0dB */ 0x24000090,
	/*  -5.5dB */ 0x22000088,
	/*  -6.0dB */ 0x20000080,
	/*  -6.5dB */ 0x1a00006c,
	/*  -7.0dB */ 0x1c800072,
	/*  -7.5dB */ 0x18000060,
	/*  -8.0dB */ 0x19800066,
	/*  -8.5dB */ 0x15800056,
	/*  -9.0dB */ 0x26c0005b,
	/*  -9.5dB */ 0x14400051,
	/* -10.0dB */ 0x24400051,
	/* -10.5dB */ 0x1300004c,
	/* -11.0dB */ 0x12000048,
	/* -11.5dB */ 0x11000044,
	/* -12.0dB */ 0x10000040
};

unsigned char TxPwrTrk_CCK_SwingTbl[TxPwrTrk_CCK_SwingTbl_Len][8] = {
	/*   0.0dB */ {0x36, 0x35, 0x2e, 0x25, 0x1c, 0x12, 0x09, 0x04},
	/*   0.5dB */ {0x33, 0x32, 0x2b, 0x23, 0x1a, 0x11, 0x08, 0x04},
	/*   1.0dB */ {0x30, 0x2f, 0x29, 0x21, 0x19, 0x10, 0x08, 0x03},
	/*   1.5dB */ {0x2d, 0x2d, 0x27, 0x1f, 0x18, 0x0f, 0x08, 0x03},
	/*   2.0dB */ {0x2b, 0x2a, 0x25, 0x1e, 0x16, 0x0e, 0x07, 0x03},
	/*   2.5dB */ {0x28, 0x28, 0x22, 0x1c, 0x15, 0x0d, 0x07, 0x03},
	/*   3.0dB */ {0x26, 0x25, 0x21, 0x1b, 0x14, 0x0d, 0x06, 0x03},
	/*   3.5dB */ {0x24, 0x23, 0x1f, 0x19, 0x13, 0x0c, 0x06, 0x03},
	/*   4.0dB */ {0x22, 0x21, 0x1d, 0x18, 0x11, 0x0b, 0x06, 0x02},
	/*   4.5dB */ {0x20, 0x20, 0x1b, 0x16, 0x11, 0x08, 0x05, 0x02},
	/*   5.0dB */ {0x1f, 0x1e, 0x1a, 0x15, 0x10, 0x0a, 0x05, 0x02},
	/*   5.5dB */ {0x1d, 0x1c, 0x18, 0x14, 0x0f, 0x0a, 0x05, 0x02},
	/*   6.0dB */ {0x1b, 0x1a, 0x17, 0x13, 0x0e, 0x09, 0x04, 0x02},
	/*   6.5dB */ {0x1a, 0x19, 0x16, 0x12, 0x0d, 0x09, 0x04, 0x02},
	/*   7.0dB */ {0x18, 0x17, 0x15, 0x11, 0x0c, 0x08, 0x04, 0x02},
	/*   7.5dB */ {0x17, 0x16, 0x13, 0x10, 0x0c, 0x08, 0x04, 0x02},
	/*   8.0dB */ {0x16, 0x15, 0x12, 0x0f, 0x0b, 0x07, 0x04, 0x01},
	/*   8.5dB */ {0x14, 0x14, 0x11, 0x0e, 0x0b, 0x07, 0x03, 0x02},
	/*   9.0dB */ {0x13, 0x13, 0x10, 0x0d, 0x0a, 0x06, 0x03, 0x01},
	/*   9.5dB */ {0x12, 0x12, 0x0f, 0x0c, 0x09, 0x06, 0x03, 0x01},
	/*  10.0dB */ {0x11, 0x11, 0x0f, 0x0c, 0x09, 0x06, 0x03, 0x01},
	/*  10.5dB */ {0x10, 0x10, 0x0e, 0x0b, 0x08, 0x05, 0x03, 0x01},
	/*  11.0dB */ {0x0f, 0x0f, 0x0d, 0x0b, 0x08, 0x05, 0x03, 0x01}
};

unsigned char TxPwrTrk_CCK_SwingTbl_CH14[TxPwrTrk_CCK_SwingTbl_Len][8] = {
	/*   0.0dB */ {0x36, 0x35, 0x2e, 0x1b, 0x00, 0x00, 0x00, 0x00},
	/*   0.5dB */ {0x33, 0x32, 0x2b, 0x19, 0x00, 0x00, 0x00, 0x00},
	/*   1.0dB */ {0x30, 0x2f, 0x29, 0x18, 0x00, 0x00, 0x00, 0x00},
	/*   1.5dB */ {0x2d, 0x2d, 0x27, 0x17, 0x00, 0x00, 0x00, 0x00},
	/*   2.0dB */ {0x2b, 0x2a, 0x25, 0x15, 0x00, 0x00, 0x00, 0x00},
	/*   2.5dB */ {0x28, 0x28, 0x22, 0x14, 0x00, 0x00, 0x00, 0x00},
	/*   3.0dB */ {0x26, 0x25, 0x21, 0x13, 0x00, 0x00, 0x00, 0x00},
	/*   3.5dB */ {0x24, 0x23, 0x1f, 0x12, 0x00, 0x00, 0x00, 0x00},
	/*   4.0dB */ {0x22, 0x21, 0x1d, 0x11, 0x00, 0x00, 0x00, 0x00},
	/*   4.5dB */ {0x20, 0x20, 0x1b, 0x10, 0x00, 0x00, 0x00, 0x00},
	/*   5.0dB */ {0x1f, 0x1e, 0x1a, 0x0f, 0x00, 0x00, 0x00, 0x00},
	/*   5.5dB */ {0x1d, 0x1c, 0x18, 0x0e, 0x00, 0x00, 0x00, 0x00},
	/*   6.0dB */ {0x1b, 0x1a, 0x17, 0x0e, 0x00, 0x00, 0x00, 0x00},
	/*   6.5dB */ {0x1a, 0x19, 0x16, 0x0d, 0x00, 0x00, 0x00, 0x00},
	/*   7.0dB */ {0x18, 0x17, 0x15, 0x0c, 0x00, 0x00, 0x00, 0x00},
	/*   7.5dB */ {0x17, 0x16, 0x13, 0x0b, 0x00, 0x00, 0x00, 0x00},
	/*   8.0dB */ {0x16, 0x15, 0x12, 0x0b, 0x00, 0x00, 0x00, 0x00},
	/*   8.5dB */ {0x14, 0x14, 0x11, 0x0a, 0x00, 0x00, 0x00, 0x00},
	/*   9.0dB */ {0x13, 0x13, 0x10, 0x0a, 0x00, 0x00, 0x00, 0x00},
	/*   9.5dB */ {0x12, 0x12, 0x0f, 0x09, 0x00, 0x00, 0x00, 0x00},
	/*  10.0dB */ {0x11, 0x11, 0x0f, 0x09, 0x00, 0x00, 0x00, 0x00},
	/*  10.5dB */ {0x10, 0x10, 0x0e, 0x08, 0x00, 0x00, 0x00, 0x00},
	/*  11.0dB */ {0x0f, 0x0f, 0x0d, 0x08, 0x00, 0x00, 0x00, 0x00}
};

unsigned char CCKSwingTable_Ch1_Ch13[][8] = {
{0x36, 0x35, 0x2e, 0x25, 0x1c, 0x12, 0x09, 0x04},	// 0, +0dB
{0x33, 0x32, 0x2b, 0x23, 0x1a, 0x11, 0x08, 0x04},	// 1, -0.5dB
{0x30, 0x2f, 0x29, 0x21, 0x19, 0x10, 0x08, 0x03},	// 2, -1.0dB
{0x2d, 0x2d, 0x27, 0x1f, 0x18, 0x0f, 0x08, 0x03},	// 3, -1.5dB
{0x2b, 0x2a, 0x25, 0x1e, 0x16, 0x0e, 0x07, 0x03},	// 4, -2.0dB
{0x28, 0x28, 0x22, 0x1c, 0x15, 0x0d, 0x07, 0x03},	// 5, -2.5dB
{0x26, 0x25, 0x21, 0x1b, 0x14, 0x0d, 0x06, 0x03},	// 6, -3.0dB
{0x24, 0x23, 0x1f, 0x19, 0x13, 0x0c, 0x06, 0x03},	// 7, -3.5dB
{0x22, 0x21, 0x1d, 0x18, 0x11, 0x0b, 0x06, 0x02},	// 8, -4.0dB
{0x20, 0x20, 0x1b, 0x16, 0x11, 0x08, 0x05, 0x02},	// 9, -4.5dB
{0x1f, 0x1e, 0x1a, 0x15, 0x10, 0x0a, 0x05, 0x02},	// 10, -5.0dB
{0x1d, 0x1c, 0x18, 0x14, 0x0f, 0x0a, 0x05, 0x02},	// 11, -5.5dB
{0x1b, 0x1a, 0x17, 0x13, 0x0e, 0x09, 0x04, 0x02},	// 12, -6.0dB
{0x1a, 0x19, 0x16, 0x12, 0x0d, 0x09, 0x04, 0x02},	// 13, -6.5dB
{0x18, 0x17, 0x15, 0x11, 0x0c, 0x08, 0x04, 0x02},	// 14, -7.0dB
{0x17, 0x16, 0x13, 0x10, 0x0c, 0x08, 0x04, 0x02},	// 15, -7.5dB
{0x16, 0x15, 0x12, 0x0f, 0x0b, 0x07, 0x04, 0x01},	// 16, -8.0dB
{0x14, 0x14, 0x11, 0x0e, 0x0b, 0x07, 0x03, 0x02},	// 17, -8.5dB
{0x13, 0x13, 0x10, 0x0d, 0x0a, 0x06, 0x03, 0x01},	// 18, -9.0dB
{0x12, 0x12, 0x0f, 0x0c, 0x09, 0x06, 0x03, 0x01},	// 19, -9.5dB
{0x11, 0x11, 0x0f, 0x0c, 0x09, 0x06, 0x03, 0x01},	// 20, -10.0dB
{0x10, 0x10, 0x0e, 0x0b, 0x08, 0x05, 0x03, 0x01},	// 21, -10.5dB
{0x0f, 0x0f, 0x0d, 0x0b, 0x08, 0x05, 0x03, 0x01},	// 22, -11.0dB
{0x0e, 0x0e, 0x0c, 0x0a, 0x08, 0x05, 0x02, 0x01},	// 23, -11.5dB
{0x0d, 0x0d, 0x0c, 0x0a, 0x07, 0x05, 0x02, 0x01},	// 24, -12.0dB
{0x0d, 0x0c, 0x0b, 0x09, 0x07, 0x04, 0x02, 0x01},	// 25, -12.5dB
{0x0c, 0x0c, 0x0a, 0x09, 0x06, 0x04, 0x02, 0x01},	// 26, -13.0dB
{0x0b, 0x0b, 0x0a, 0x08, 0x06, 0x04, 0x02, 0x01},	// 27, -13.5dB
{0x0b, 0x0a, 0x09, 0x08, 0x06, 0x04, 0x02, 0x01},	// 28, -14.0dB
{0x0a, 0x0a, 0x09, 0x07, 0x05, 0x03, 0x02, 0x01},	// 29, -14.5dB
{0x0a, 0x09, 0x08, 0x07, 0x05, 0x03, 0x02, 0x01},	// 30, -15.0dB
{0x09, 0x09, 0x08, 0x06, 0x05, 0x03, 0x01, 0x01},	// 31, -15.5dB
{0x09, 0x08, 0x07, 0x06, 0x04, 0x03, 0x01, 0x01}	// 32, -16.0dB
};

unsigned char CCKSwingTable_Ch14 [][8]= {
{0x36, 0x35, 0x2e, 0x1b, 0x00, 0x00, 0x00, 0x00},	// 0, +0dB
{0x33, 0x32, 0x2b, 0x19, 0x00, 0x00, 0x00, 0x00},	// 1, -0.5dB
{0x30, 0x2f, 0x29, 0x18, 0x00, 0x00, 0x00, 0x00},	// 2, -1.0dB
{0x2d, 0x2d, 0x17, 0x17, 0x00, 0x00, 0x00, 0x00},	// 3, -1.5dB
{0x2b, 0x2a, 0x25, 0x15, 0x00, 0x00, 0x00, 0x00},	// 4, -2.0dB
{0x28, 0x28, 0x24, 0x14, 0x00, 0x00, 0x00, 0x00},	// 5, -2.5dB
{0x26, 0x25, 0x21, 0x13, 0x00, 0x00, 0x00, 0x00},	// 6, -3.0dB
{0x24, 0x23, 0x1f, 0x12, 0x00, 0x00, 0x00, 0x00},	// 7, -3.5dB
{0x22, 0x21, 0x1d, 0x11, 0x00, 0x00, 0x00, 0x00},	// 8, -4.0dB
{0x20, 0x20, 0x1b, 0x10, 0x00, 0x00, 0x00, 0x00},	// 9, -4.5dB
{0x1f, 0x1e, 0x1a, 0x0f, 0x00, 0x00, 0x00, 0x00},	// 10, -5.0dB
{0x1d, 0x1c, 0x18, 0x0e, 0x00, 0x00, 0x00, 0x00},	// 11, -5.5dB
{0x1b, 0x1a, 0x17, 0x0e, 0x00, 0x00, 0x00, 0x00},	// 12, -6.0dB
{0x1a, 0x19, 0x16, 0x0d, 0x00, 0x00, 0x00, 0x00},	// 13, -6.5dB
{0x18, 0x17, 0x15, 0x0c, 0x00, 0x00, 0x00, 0x00},	// 14, -7.0dB
{0x17, 0x16, 0x13, 0x0b, 0x00, 0x00, 0x00, 0x00},	// 15, -7.5dB
{0x16, 0x15, 0x12, 0x0b, 0x00, 0x00, 0x00, 0x00},	// 16, -8.0dB
{0x14, 0x14, 0x11, 0x0a, 0x00, 0x00, 0x00, 0x00},	// 17, -8.5dB
{0x13, 0x13, 0x10, 0x0a, 0x00, 0x00, 0x00, 0x00},	// 18, -9.0dB
{0x12, 0x12, 0x0f, 0x09, 0x00, 0x00, 0x00, 0x00},	// 19, -9.5dB
{0x11, 0x11, 0x0f, 0x09, 0x00, 0x00, 0x00, 0x00},	// 20, -10.0dB
{0x10, 0x10, 0x0e, 0x08, 0x00, 0x00, 0x00, 0x00},	// 21, -10.5dB
{0x0f, 0x0f, 0x0d, 0x08, 0x00, 0x00, 0x00, 0x00},	// 22, -11.0dB
{0x0e, 0x0e, 0x0c, 0x07, 0x00, 0x00, 0x00, 0x00},	// 23, -11.5dB
{0x0d, 0x0d, 0x0c, 0x07, 0x00, 0x00, 0x00, 0x00},	// 24, -12.0dB
{0x0d, 0x0c, 0x0b, 0x06, 0x00, 0x00, 0x00, 0x00},	// 25, -12.5dB
{0x0c, 0x0c, 0x0a, 0x06, 0x00, 0x00, 0x00, 0x00},	// 26, -13.0dB
{0x0b, 0x0b, 0x0a, 0x06, 0x00, 0x00, 0x00, 0x00},	// 27, -13.5dB
{0x0b, 0x0a, 0x09, 0x05, 0x00, 0x00, 0x00, 0x00},	// 28, -14.0dB
{0x0a, 0x0a, 0x09, 0x05, 0x00, 0x00, 0x00, 0x00},	// 29, -14.5dB
{0x0a, 0x09, 0x08, 0x05, 0x00, 0x00, 0x00, 0x00},	// 30, -15.0dB
{0x09, 0x09, 0x08, 0x05, 0x00, 0x00, 0x00, 0x00},	// 31, -15.5dB
{0x09, 0x08, 0x07, 0x04, 0x00, 0x00, 0x00, 0x00}	// 32, -16.0dB
};

const int OFDM_TABLE_SIZE= sizeof(OFDMSwingTable)/sizeof(int);
const int CCK_TABLE_SIZE= sizeof(CCKSwingTable_Ch1_Ch13) >>3;


#ifdef CONFIG_RTL_92D_SUPPORT

static unsigned int OFDMSwingTable_92D[] = {
	0x7f8001fe, // 0, +6.0dB
	0x788001e2, // 1, +5.5dB
	0x71c001c7, // 2, +5.0dB
	0x6b8001ae, // 3, +4.5dB
	0x65400195, // 4, +4.0dB
	0x5fc0017f, // 5, +3.5dB
	0x5a400169, // 6, +3.0dB
	0x55400155, // 7, +2.5dB
	0x50800142, // 8, +2.0dB
	0x4c000130, // 9, +1.5dB
	0x47c0011f, // 10, +1.0dB
	0x43c0010f, // 11, +0.5dB
	0x40000100, // 12, +0dB
	0x3c8000f2, // 13, -0.5dB
	0x390000e4, // 14, -1.0dB
	0x35c000d7, // 15, -1.5dB
	0x32c000cb, // 16, -2.0dB
	0x300000c0, // 17, -2.5dB
	0x2d4000b5, // 18, -3.0dB
	0x2ac000ab, // 19, -3.5dB
	0x288000a2, // 20, -4.0dB
	0x26000098, // 21, -4.5dB
	0x24000090, // 22, -5.0dB
	0x22000088, // 23, -5.5dB
	0x20000080, // 24, -6.0dB
	0x1e400079, // 25, -6.5dB
	0x1c800072, // 26, -7.0dB
	0x1b00006c, // 27. -7.5dB
	0x19800066, // 28, -8.0dB
	0x18000060, // 29, -8.5dB
	0x16c0005b, // 30, -9.0dB
	0x15800056, // 31, -9.5dB
	0x14400051, // 32, -10.0dB
	0x1300004c, // 33, -10.5dB
	0x12000048, // 34, -11.0dB
	0x11000044, // 35, -11.5dB
	0x10000040, // 36, -12.0dB
	0x0f00003c,// 37, -12.5dB
	0x0e400039,// 38, -13.0dB
	0x0d800036,// 39, -13.5dB
    0x0cc00033,// 40, -14.0dB
	0x0c000030,// 41, -14.5dB
	0x0b40002d,// 42, -15.0dB
};
#endif


#ifdef HW_ANT_SWITCH
#define RXDVY_A_EN		((HW_DIV_ENABLE && !priv->pshare->rf_ft_var.antSw_select) ? 0x80 : 0)
#define RXDVY_B_EN		((HW_DIV_ENABLE &&  priv->pshare->rf_ft_var.antSw_select) ? 0x80 : 0)
#endif


//3 ============================================================
//3 DIG related functions
//3 ============================================================

int getIGIFor1RCCA(int value_IGI)
{
	#define ONERCCA_LOW_TH		0x30
	#define ONERCCA_LOW_DIFF	8

	if (value_IGI < ONERCCA_LOW_TH) {
		if ((ONERCCA_LOW_TH - value_IGI) < ONERCCA_LOW_DIFF)
			return ONERCCA_LOW_TH;
		else
			return value_IGI + ONERCCA_LOW_DIFF;
	} else {
		return value_IGI;
	}
}


void set_DIG_state(struct rtl8192cd_priv *priv, int state)
{
	int value_IGI;

	if (state) {
		priv->pshare->DIG_on = 1;
		priv->pshare->restore = 0;
	}
	else {
		priv->pshare->DIG_on = 0;
		if (priv->pshare->restore == 0) {
			if (priv->pshare->rf_ft_var.use_ext_lna == 1)
				value_IGI = 0x30;
			else
				value_IGI = 0x20;

#if defined(HW_ANT_SWITCH)
			// wirte new initial gain index into regC50/C58
			if (priv->pshare->rf_ft_var.one_path_cca == 0)	{
				RTL_W8(0xc50, value_IGI | RXDVY_A_EN);
#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
				if (
#ifdef CONFIG_RTL_92C_SUPPORT
					(GET_CHIP_VER(priv)==VERSION_8192C) || (GET_CHIP_VER(priv)==VERSION_8188C) 
#endif
#ifdef CONFIG_RTL_92D_SUPPORT
#ifdef CONFIG_RTL_92C_SUPPORT
					|| 
#endif
					(GET_CHIP_VER(priv)==VERSION_8192D)
#endif
					)
					RTL_W8(0xc58, value_IGI | RXDVY_B_EN);
#endif
			} else if (priv->pshare->rf_ft_var.one_path_cca == 1) {
				RTL_W8(0xc50, value_IGI | RXDVY_A_EN);
#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
				if (
#ifdef CONFIG_RTL_92C_SUPPORT
					(GET_CHIP_VER(priv)==VERSION_8192C) || (GET_CHIP_VER(priv)==VERSION_8188C) 
#endif
#ifdef CONFIG_RTL_92D_SUPPORT
#ifdef CONFIG_RTL_92C_SUPPORT
					|| 
#endif
					(GET_CHIP_VER(priv)==VERSION_8192D)
#endif
					)
					RTL_W8(0xc58, getIGIFor1RCCA(value_IGI) | RXDVY_B_EN);
#endif
			} else if (priv->pshare->rf_ft_var.one_path_cca == 2) {
				RTL_W8(0xc50, getIGIFor1RCCA(value_IGI) | RXDVY_A_EN);
#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
				if (
#ifdef CONFIG_RTL_92C_SUPPORT
					(GET_CHIP_VER(priv)==VERSION_8192C) || (GET_CHIP_VER(priv)==VERSION_8188C) 
#endif
#ifdef CONFIG_RTL_92D_SUPPORT
#ifdef CONFIG_RTL_92C_SUPPORT
					|| 
#endif
					(GET_CHIP_VER(priv)==VERSION_8192D)
#endif
					)
					RTL_W8(0xc58, value_IGI | RXDVY_B_EN);
#endif
			}
#else
			// Write IGI into HW
			if (priv->pshare->rf_ft_var.one_path_cca == 0) 	{
				RTL_W8(0xc50, value_IGI);
#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
				if (
#ifdef CONFIG_RTL_92C_SUPPORT
					(GET_CHIP_VER(priv)==VERSION_8192C) || (GET_CHIP_VER(priv)==VERSION_8188C) 
#endif
#ifdef CONFIG_RTL_92D_SUPPORT
#ifdef CONFIG_RTL_92C_SUPPORT
					|| 
#endif
					(GET_CHIP_VER(priv)==VERSION_8192D)
#endif
					)
					RTL_W8(0xc58, value_IGI);
#endif
			} else if (priv->pshare->rf_ft_var.one_path_cca == 1) {
				RTL_W8(0xc50, value_IGI);
#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
				if (
#ifdef CONFIG_RTL_92C_SUPPORT
					(GET_CHIP_VER(priv)==VERSION_8192C) || (GET_CHIP_VER(priv)==VERSION_8188C) 
#endif
#ifdef CONFIG_RTL_92D_SUPPORT
#ifdef CONFIG_RTL_92C_SUPPORT
					|| 
#endif
					(GET_CHIP_VER(priv)==VERSION_8192D)
#endif
					)
					RTL_W8(0xc58, getIGIFor1RCCA(value_IGI));
#endif
			} else if (priv->pshare->rf_ft_var.one_path_cca == 2) {
				RTL_W8(0xc50, getIGIFor1RCCA(value_IGI));
#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
				if (
#ifdef CONFIG_RTL_92C_SUPPORT
					(GET_CHIP_VER(priv)==VERSION_8192C) || (GET_CHIP_VER(priv)==VERSION_8188C) 
#endif
#ifdef CONFIG_RTL_92D_SUPPORT
#ifdef CONFIG_RTL_92C_SUPPORT
					|| 
#endif
					(GET_CHIP_VER(priv)==VERSION_8192D)
#endif
					)
					RTL_W8(0xc58, value_IGI);
#endif
			}
#endif
			priv->pshare->restore = 1;
		}
#ifdef INTERFERENCE_CONTROL
		priv->pshare->phw->signal_strength = 0;
#endif
	}
}


void DIG_process(struct rtl8192cd_priv *priv)
{
	#define DEAD_POINT_TH		10000
	#define DOWN_IG_HIT_TH		5
	#define DEAD_POINT_HIT_TH	3

	unsigned char value_IGI;
	signed char value8;

#ifdef INTERFERENCE_CONTROL
	unsigned short thd0, thd1, thd2;
#endif

	if (priv->pshare->DIG_on == 1)
	{
		if (priv->pshare->rf_ft_var.use_ext_lna == 1) {
//			priv->pshare->FA_upper = 0x42;
			priv->pshare->FA_upper = MIN_NUM(0x42, priv->pshare->rssi_min+36);
			priv->pshare->FA_lower = 0x30;
		} else {
			// Reset initial gain upper & lower bounds
#ifdef DFS
			if (!priv->pmib->dot11DFSEntry.disable_DFS &&
				(OPMODE & WIFI_AP_STATE) &&
				(((priv->pmib->dot11RFEntry.dot11channel >= 52) &&
				(priv->pmib->dot11RFEntry.dot11channel <= 64)) ||
				((priv->pmib->dot11RFEntry.dot11channel >= 100) &&
				(priv->pmib->dot11RFEntry.dot11channel <= 140))))
				priv->pshare->FA_upper = 0x24;
			else
#endif
			{
#ifdef INTERFERENCE_CONTROL
				priv->pshare->FA_lower = 0x20;

				if (priv->pshare->rssi_min != 0xFF)
				{
//					priv->pshare->FA_upper = 0x3E;
					
					if (priv->pshare->rssi_min > 30)
						priv->pshare->FA_lower = 0x24;
					else if (priv->pshare->rssi_min > 25)
						priv->pshare->FA_lower = 0x22;

					// limit upper bound to prevent the minimal signal sta from disconnect
//					if ((priv->pshare->rssi_min + 10) < priv->pshare->FA_upper)
//						priv->pshare->FA_upper = priv->pshare->rssi_min + 10;
					priv->pshare->FA_upper = MIN_NUM(0x3E, priv->pshare->rssi_min+20);

					thd0 = priv->pshare->threshold0;
					thd1 = priv->pshare->threshold1;
					thd2 = priv->pshare->threshold2;
				}
				else		// before link
				{
					priv->pshare->FA_upper = 0x32;
					
					thd0 = 500;
					thd1 = 8000;
					thd2 = 10000;
				}
#else
				if (priv->pmib->dot11RFEntry.tx2path) {
					if (priv->pmib->dot11BssType.net_work_type == WIRELESS_11B)
						priv->pshare->FA_upper = MIN_NUM(0x2A, priv->pshare->rssi_min+20);
					else
						priv->pshare->FA_upper = MIN_NUM(0x32, priv->pshare->rssi_min+20);
				}
				else
					priv->pshare->FA_upper = MIN_NUM(0x32, priv->pshare->rssi_min+20);
				priv->pshare->FA_lower = 0x20;

				if (priv->pshare->rssi_min > 30)
					priv->pshare->FA_lower = 0x24;
				else if (priv->pshare->rssi_min > 25)
					priv->pshare->FA_lower = 0x22;
#endif
			}
		}

		// determine a new initial gain index according to the sumation of all FA counters as well as upper & lower bounds
		value8 = RTL_R8(0xc50);
#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
		if (
#ifdef CONFIG_RTL_92C_SUPPORT
			(GET_CHIP_VER(priv)==VERSION_8192C) || (GET_CHIP_VER(priv)==VERSION_8188C) 
#endif
#ifdef CONFIG_RTL_92D_SUPPORT
#ifdef CONFIG_RTL_92C_SUPPORT
			|| 
#endif
			(GET_CHIP_VER(priv)==VERSION_8192D)
#endif
			) {
			if(priv->pshare->rf_ft_var.one_path_cca==2)
				value8 = RTL_R8(0xc58);
		}
#endif

		value_IGI = (value8 & 0x7F);

#if  defined(CONFIG_RTL_NOISE_CONTROL_92C)
	if(priv->pshare->rf_ft_var.dnc_enable)
	if ((GET_CHIP_VER(priv) == VERSION_8192C)||(GET_CHIP_VER(priv) == VERSION_8188C)){
		unsigned long tp_now = (priv->ext_stats.tx_avarage+priv->ext_stats.rx_avarage)>>17;
		if(priv->pshare->rf_ft_var.use_ext_lna) {
			if( (priv->pshare->rssi_min > 50)  )  {
				if((!priv->pshare->DNC_on) && (value_IGI >= priv->pshare->FA_upper) && (priv->pshare->FA_total_cnt > priv->pshare->threshold2)) {
					priv->pshare->DNC_on = 1;
					priv->ext_stats.tp_average_pre = tp_now;
					priv->pshare->FA_lower = 0x20;
					PHY_SetBBReg(priv, 0x870, bMaskDWord, RTL_R32(0x870)|BIT(5)|BIT(6)|BIT(21)|BIT(22));

#ifdef HW_ANT_SWITCH
					PHY_SetBBReg(priv, 0xc50, bMaskByte0, priv->pshare->FA_lower | RXDVY_A_EN);
					PHY_SetBBReg(priv, 0xc58, bMaskByte0, priv->pshare->FA_lower | RXDVY_B_EN);
#else
					PHY_SetBBReg(priv, 0xc50, bMaskByte0, priv->pshare->FA_lower);
					PHY_SetBBReg(priv, 0xc58, bMaskByte0, priv->pshare->FA_lower);
#endif

				} else if(priv->pshare->DNC_on ==1)  {
					if(tp_now < priv->ext_stats.tp_average_pre + 2) {
						priv->pshare->DNC_on = 0;
					}
					else {
						priv->pshare->DNC_on =2;
						priv->ext_stats.tp_average_pre = tp_now;
					}
				} else if(priv->pshare->DNC_on >= 2 ) {
					if(( tp_now+10 < priv->ext_stats.tp_average_pre ) || (tp_now < 1) ) {
							priv->pshare->DNC_on = 0;
					} else if(priv->pshare->DNC_on<5) {
						priv->ext_stats.tp_average_pre = tp_now;
						++priv->pshare->DNC_on;
					}
				}
			 }else {
				priv->pshare->DNC_on = 0;
			}

			if(	priv->pshare->DNC_on )
				return;
			else
				PHY_SetBBReg(priv, 0x870, bMaskDWord, RTL_R32(0x870)&  ~(BIT(5)|BIT(6)|BIT(21)|BIT(22)));

		} else {
			if( (priv->pshare->rssi_min > 40) && (value_IGI >= priv->pshare->FA_upper) )  {
//				unsigned long tp_now = (priv->ext_stats.tx_avarage+priv->ext_stats.rx_avarage)>>17;
				if((!priv->pshare->DNC_on) && (priv->pshare->FA_total_cnt > priv->pshare->threshold2)) {
					priv->pshare->DNC_on = 1;
					priv->ext_stats.tp_average_pre = tp_now;
				} else if(priv->pshare->DNC_on ==1)  {
					if(tp_now < priv->ext_stats.tp_average_pre + 2) {
						priv->pshare->DNC_on = 0;
					}
					else {
						priv->pshare->DNC_on = 2;
						priv->ext_stats.tp_average_pre = tp_now;
					}
				} else if(priv->pshare->DNC_on >= 2 ) {
					if((tp_now +10 < priv->ext_stats.tp_average_pre )
						|| ((priv->ext_stats.tp_average_pre < 10) && (priv->pshare->FA_total_cnt < priv->pshare->threshold1))) {
						priv->pshare->DNC_on = 0;
					} else 	if(priv->pshare->DNC_on<6) {
						priv->ext_stats.tp_average_pre = tp_now;
						++priv->pshare->DNC_on;
					}
				}
				if(priv->pshare->DNC_on) {
					priv->pshare->FA_upper = 0x3e;
				}
			}else {
				priv->pshare->DNC_on = 0;
			}
		}
	}
#endif

		if ((priv->pshare->digDeadPoint == 0) && (priv->pshare->FA_total_cnt > DEAD_POINT_TH)) {
			if ((priv->pshare->digDeadPointHitCount > 0) && (priv->pshare->digDeadPointCandidate == value_IGI)) {
				priv->pshare->digDeadPointHitCount++;
				if (priv->pshare->digDeadPointHitCount == DEAD_POINT_HIT_TH) {
					priv->pshare->digDeadPoint = priv->pshare->digDeadPointCandidate;
				}
			} else {
				priv->pshare->digDeadPointCandidate = value_IGI;
				priv->pshare->digDeadPointHitCount = 1;
			}
		}

#ifdef INTERFERENCE_CONTROL
		if (priv->pshare->FA_total_cnt < thd0) {
#else
		if (priv->pshare->FA_total_cnt < priv->pshare->threshold0) {
#endif
			priv->pshare->digDownCount++;
			if (priv->pshare->digDownCount > DOWN_IG_HIT_TH) {
				// Reset deadpoint hit count
				if ((priv->pshare->digDeadPoint == 0) && (priv->pshare->digDeadPointHitCount > 0) && (value_IGI == priv->pshare->digDeadPointCandidate))
					priv->pshare->digDeadPointHitCount = 0;

				value_IGI--;

				// Check if the new value is dead point
				if ((priv->pshare->digDeadPoint > 0) && (value_IGI == priv->pshare->digDeadPoint))
					value_IGI++;
			}
#ifdef INTERFERENCE_CONTROL
		} else if (priv->pshare->FA_total_cnt < thd1) {
#else
		} else if (priv->pshare->FA_total_cnt < priv->pshare->threshold1) {
#endif
			value_IGI += 0;
			priv->pshare->digDownCount = 0;
#ifdef INTERFERENCE_CONTROL
		} else if (priv->pshare->FA_total_cnt < thd2) {
#else
		} else if (priv->pshare->FA_total_cnt < priv->pshare->threshold2) {
#endif
			value_IGI++;
			priv->pshare->digDownCount = 0;
#ifdef INTERFERENCE_CONTROL
		} else if (priv->pshare->FA_total_cnt >= thd2) {
#else
		} else if (priv->pshare->FA_total_cnt >= priv->pshare->threshold2) {
#endif
			value_IGI += 2;
			priv->pshare->digDownCount = 0;
		} else {
			priv->pshare->digDownCount = 0;
		}

		if (value_IGI > priv->pshare->FA_upper)
			value_IGI = priv->pshare->FA_upper;
		else if (value_IGI < priv->pshare->FA_lower)
			value_IGI = priv->pshare->FA_lower;

#if defined(HW_ANT_SWITCH)
		// wirte new initial gain index into regC50/C58
		if (priv->pshare->rf_ft_var.one_path_cca == 0)	{
			RTL_W8(0xc50, value_IGI | RXDVY_A_EN);
#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
			if (
#ifdef CONFIG_RTL_92C_SUPPORT
				(GET_CHIP_VER(priv)==VERSION_8192C) || (GET_CHIP_VER(priv)==VERSION_8188C) 
#endif
#ifdef CONFIG_RTL_92D_SUPPORT
#ifdef CONFIG_RTL_92C_SUPPORT
				|| 
#endif
				(GET_CHIP_VER(priv)==VERSION_8192D)
#endif
				)
				RTL_W8(0xc58, value_IGI | RXDVY_B_EN);
#endif
		} else if (priv->pshare->rf_ft_var.one_path_cca == 1)	{
			RTL_W8(0xc50, value_IGI | RXDVY_A_EN);
#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
			if (
#ifdef CONFIG_RTL_92C_SUPPORT
				(GET_CHIP_VER(priv)==VERSION_8192C) || (GET_CHIP_VER(priv)==VERSION_8188C) 
#endif
#ifdef CONFIG_RTL_92D_SUPPORT
#ifdef CONFIG_RTL_92C_SUPPORT
				|| 
#endif
				(GET_CHIP_VER(priv)==VERSION_8192D)
#endif
				)
				RTL_W8(0xc58, getIGIFor1RCCA(value_IGI) | RXDVY_B_EN);
#endif
		} else if (priv->pshare->rf_ft_var.one_path_cca == 2)		{
			RTL_W8(0xc50, getIGIFor1RCCA(value_IGI) | RXDVY_A_EN);
#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
			if (
#ifdef CONFIG_RTL_92C_SUPPORT
				(GET_CHIP_VER(priv)==VERSION_8192C) || (GET_CHIP_VER(priv)==VERSION_8188C) 
#endif
#ifdef CONFIG_RTL_92D_SUPPORT
#ifdef CONFIG_RTL_92C_SUPPORT
				|| 
#endif
				(GET_CHIP_VER(priv)==VERSION_8192D)
#endif
				)
				RTL_W8(0xc58, value_IGI| RXDVY_B_EN);
#endif
		}
#else
		// Write IGI into HW
		if (priv->pshare->rf_ft_var.one_path_cca == 0) {
			RTL_W8(0xc50, value_IGI);
#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
			if (
#ifdef CONFIG_RTL_92C_SUPPORT
				(GET_CHIP_VER(priv)==VERSION_8192C) || (GET_CHIP_VER(priv)==VERSION_8188C) 
#endif
#ifdef CONFIG_RTL_92D_SUPPORT
#ifdef CONFIG_RTL_92C_SUPPORT
				|| 
#endif
				(GET_CHIP_VER(priv)==VERSION_8192D)
#endif
				)
				RTL_W8(0xc58, value_IGI);
#endif
		} else if (priv->pshare->rf_ft_var.one_path_cca == 1) {
			RTL_W8(0xc50, value_IGI);
#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
			if (
#ifdef CONFIG_RTL_92C_SUPPORT
				(GET_CHIP_VER(priv)==VERSION_8192C) || (GET_CHIP_VER(priv)==VERSION_8188C) 
#endif
#ifdef CONFIG_RTL_92D_SUPPORT
#ifdef CONFIG_RTL_92C_SUPPORT
				|| 
#endif
				(GET_CHIP_VER(priv)==VERSION_8192D)
#endif
				)
				RTL_W8(0xc58, getIGIFor1RCCA(value_IGI));
#endif
		} else if (priv->pshare->rf_ft_var.one_path_cca == 2) {
			RTL_W8(0xc50, getIGIFor1RCCA(value_IGI));
#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
			if (
#ifdef CONFIG_RTL_92C_SUPPORT
				(GET_CHIP_VER(priv)==VERSION_8192C) || (GET_CHIP_VER(priv)==VERSION_8188C) 
#endif
#ifdef CONFIG_RTL_92D_SUPPORT
#ifdef CONFIG_RTL_92C_SUPPORT
				|| 
#endif
				(GET_CHIP_VER(priv)==VERSION_8192D)
#endif
				)
				RTL_W8(0xc58, value_IGI);
#endif
		}
#endif

	}
}


void check_DIG_by_rssi(struct rtl8192cd_priv *priv, unsigned char rssi_strength)
{
	unsigned int dig_on = 0;

	if (OPMODE & WIFI_SITE_MONITOR)
		return;

	if ((rssi_strength > priv->pshare->rf_ft_var.digGoUpperLevel)
		&& (rssi_strength < HP_LOWER+1) && (priv->pshare->phw->signal_strength != 2)) {
#ifndef CONFIG_RTL_92D_SUPPORT
		if (priv->pshare->is_40m_bw)
			// RTL_W8(0xc87, (RTL_R8(0xc87) & 0xf) | 0x30); 92D
			RTL_W8(0xc87, 0x30);
		else
			RTL_W8(0xc30, 0x44);
#endif

		if (priv->pshare->phw->signal_strength != 3)
			dig_on++;

		priv->pshare->phw->signal_strength = 2;
	}
	else if ((rssi_strength > HP_LOWER+5) && (priv->pshare->phw->signal_strength != 3)) {
#ifndef CONFIG_RTL_92D_SUPPORT
		if (priv->pshare->is_40m_bw)
			// RTL_W8(0xc87, (RTL_R8(0xc87) & 0xf) | 0x30); 92D
			RTL_W8(0xc87, 0x30);
		else
			RTL_W8(0xc30, 0x44);
#endif

		if (priv->pshare->phw->signal_strength != 2)
			dig_on++;

		priv->pshare->phw->signal_strength = 3;
	}
	else if (((rssi_strength < priv->pshare->rf_ft_var.digGoLowerLevel)
		&& (priv->pshare->phw->signal_strength != 1)) || !priv->pshare->phw->signal_strength) {
		// DIG off
//		set_DIG_state(priv, 0);

#ifndef CONFIG_RTL_92D_SUPPORT
		if (priv->pshare->is_40m_bw)
			//RTL_W8(0xc87, (RTL_R8(0xc87) & 0xf) | 0x30); 92D
			RTL_W8(0xc87, 0x30);
		else
			RTL_W8(0xc30, 0x44);
#endif

		priv->pshare->phw->signal_strength = 1;
	}

	if (dig_on) {
		// DIG on
		set_DIG_state(priv, 1);
	}

	//check_DC_TH_by_rssi(priv, rssi_strength);
}


void DIG_for_site_survey(struct rtl8192cd_priv *priv, int do_ss)
{
	if (do_ss) {
		// DIG off
		set_DIG_state(priv, 0);
	}
	else {
		// DIG on
#ifndef INTERFERENCE_CONTROL
		if (priv->pshare->phw->signal_strength > 1) 
#endif
		{
			set_DIG_state(priv, 1);
		}
	}
}

#ifdef INTERFERENCE_CONTROL
void check_NBI_by_rssi(struct rtl8192cd_priv *priv, unsigned char rssi_strength)
{
	if (OPMODE & WIFI_SITE_MONITOR)
		return;

	if (priv->pshare->phw->nbi_filter_on) {
		if (rssi_strength < 20) {
			priv->pshare->phw->nbi_filter_on = 0;
			RTL_W16(rOFDM0_RxDSP, RTL_R16(rOFDM0_RxDSP) & ~ BIT(9));	// NBI off
		}
	} else {	// NBI OFF previous
		if (rssi_strength > 25) {
			priv->pshare->phw->nbi_filter_on = 1;
			RTL_W16(rOFDM0_RxDSP, RTL_R16(rOFDM0_RxDSP) | BIT(9));		// NBI on
		}
	}
}
#endif

/*
 * dynamic CCK CCA enhance by rssi
 */
void CCK_CCA_dynamic_enhance(struct rtl8192cd_priv *priv, unsigned char rssi_strength)
{
#if 1
	unsigned int cck_fa = priv->pshare->FA_total_cnt;
	int rssi_thd = 30;

	if (rssi_strength == 0xff) {
		if (cck_fa < 1000) {
			if (priv->pshare->phw->CCK_CCA_enhanced != 2) {
				RTL_W8(0xa0a, 0x40);
				priv->pshare->phw->CCK_CCA_enhanced = 2;
			}
		} else {
			if (priv->pshare->phw->CCK_CCA_enhanced != 1) {
				RTL_W8(0xa0a, 0x83);
				priv->pshare->phw->CCK_CCA_enhanced = 1;
			}
		}
		return;
	}

	if (rssi_strength > rssi_thd+5) {
		if (priv->pshare->phw->CCK_CCA_enhanced != 0) {
			RTL_W8(0xa0a, 0xcd);
			priv->pshare->phw->CCK_CCA_enhanced = 0;
		}
	} else if (rssi_strength< rssi_thd) {
		if ((rssi_strength > 9) || (priv->assoc_num >1)) {
			if (priv->pshare->phw->CCK_CCA_enhanced != 1) {
				RTL_W8(0xa0a, 0x83);
				priv->pshare->phw->CCK_CCA_enhanced = 1;
			}
		} else {
			if(cck_fa<1000) {
				if (priv->pshare->phw->CCK_CCA_enhanced != 2) {
					RTL_W8(0xa0a, 0x40);
					priv->pshare->phw->CCK_CCA_enhanced = 2;
				}
			} else {
				if (priv->pshare->phw->CCK_CCA_enhanced != 1) {
					RTL_W8(0xa0a, 0x83);
					priv->pshare->phw->CCK_CCA_enhanced = 1;
				}
			}
		}
	}

#else

	if (rssi_strength == 0xff)
		return;

	if (!priv->pshare->phw->CCK_CCA_enhanced && (rssi_strength < 30)) {
		priv->pshare->phw->CCK_CCA_enhanced = TRUE;
		RTL_W8(0xa0a, 0x83);
	}
	else if (priv->pshare->phw->CCK_CCA_enhanced && (rssi_strength > 35)) {
		priv->pshare->phw->CCK_CCA_enhanced = FALSE;
		RTL_W8(0xa0a, 0xcd);
	}
#endif
}


//3 ============================================================
//3 Dynamic Tx Power / Power Tracking
//3 ============================================================

#ifdef HIGH_POWER_EXT_PA
void tx_power_control(struct rtl8192cd_priv *priv)
{
	unsigned long x;

	int pwr_value = 0x10101010;
	if( priv->pshare->phw->signal_strength == 3 && priv->pshare->phw->lower_tx_power== 0) {
		SAVE_INT_AND_CLI(x);
		priv->pshare->phw->power_backup[0x00] = RTL_R32(rTxAGC_A_Rate18_06);
		priv->pshare->phw->power_backup[0x01] = RTL_R32(rTxAGC_A_Rate54_24);
		priv->pshare->phw->power_backup[0x02] = RTL_R32(rTxAGC_B_Rate18_06);
		priv->pshare->phw->power_backup[0x03] = RTL_R32(rTxAGC_B_Rate54_24);
		priv->pshare->phw->power_backup[0x04] = RTL_R32(rTxAGC_A_Mcs03_Mcs00);
		priv->pshare->phw->power_backup[0x05] = RTL_R32(rTxAGC_A_Mcs07_Mcs04);
		priv->pshare->phw->power_backup[0x06] = RTL_R32(rTxAGC_A_Mcs11_Mcs08);
		priv->pshare->phw->power_backup[0x07] = RTL_R32(rTxAGC_A_Mcs15_Mcs12);
		priv->pshare->phw->power_backup[0x08] = RTL_R32(rTxAGC_B_Mcs03_Mcs00);
		priv->pshare->phw->power_backup[0x09] = RTL_R32(rTxAGC_B_Mcs07_Mcs04);
		priv->pshare->phw->power_backup[0x0a] = RTL_R32(rTxAGC_B_Mcs11_Mcs08);
		priv->pshare->phw->power_backup[0x0b] = RTL_R32(rTxAGC_B_Mcs15_Mcs12);
		priv->pshare->phw->power_backup[0x0c] = RTL_R32(rTxAGC_A_CCK11_2_B_CCK11);
		priv->pshare->phw->power_backup[0x0d] = RTL_R32(rTxAGC_A_CCK1_Mcs32);
		priv->pshare->phw->power_backup[0x0e] = RTL_R32(rTxAGC_B_CCK5_1_Mcs32);
		RTL_W32(rTxAGC_A_Rate18_06, pwr_value);
		RTL_W32(rTxAGC_A_Rate54_24, pwr_value);
		RTL_W32(rTxAGC_B_Rate18_06, pwr_value);
		RTL_W32(rTxAGC_B_Rate54_24, pwr_value);
		RTL_W32(rTxAGC_A_Mcs03_Mcs00, pwr_value);
		RTL_W32(rTxAGC_A_Mcs07_Mcs04, pwr_value);
		RTL_W32(rTxAGC_A_Mcs11_Mcs08, pwr_value);
		RTL_W32(rTxAGC_A_Mcs15_Mcs12, pwr_value);
		RTL_W32(rTxAGC_B_Mcs03_Mcs00, pwr_value);
		RTL_W32(rTxAGC_B_Mcs07_Mcs04, pwr_value);
		RTL_W32(rTxAGC_B_Mcs11_Mcs08, pwr_value);
		RTL_W32(rTxAGC_B_Mcs15_Mcs12, pwr_value);
		RTL_W32(rTxAGC_A_CCK11_2_B_CCK11, pwr_value);
		RTL_W32(rTxAGC_A_CCK1_Mcs32, (pwr_value & 0x0000ff00) | (priv->pshare->phw->power_backup[0x0d] &0xffff00ff));
		RTL_W32(rTxAGC_B_CCK5_1_Mcs32, (pwr_value & 0xffffff00) | (priv->pshare->phw->power_backup[0x0e] &0x000000ff));
		priv->pshare->phw->lower_tx_power = 1;
		RESTORE_INT(x);
	}
	else if( priv->pshare->phw->signal_strength != 3 && priv->pshare->phw->lower_tx_power) {
		SAVE_INT_AND_CLI(x);
		RTL_W32(rTxAGC_A_Rate18_06, priv->pshare->phw->power_backup[0x00]);
		RTL_W32(rTxAGC_A_Rate54_24, priv->pshare->phw->power_backup[0x01]);
		RTL_W32(rTxAGC_B_Rate18_06, priv->pshare->phw->power_backup[0x02]);
		RTL_W32(rTxAGC_B_Rate54_24, priv->pshare->phw->power_backup[0x03]);
		RTL_W32(rTxAGC_A_Mcs03_Mcs00, priv->pshare->phw->power_backup[0x04]);
		RTL_W32(rTxAGC_A_Mcs07_Mcs04, priv->pshare->phw->power_backup[0x05]);
		RTL_W32(rTxAGC_A_Mcs11_Mcs08, priv->pshare->phw->power_backup[0x06]);
		RTL_W32(rTxAGC_A_Mcs15_Mcs12, priv->pshare->phw->power_backup[0x07]);
		RTL_W32(rTxAGC_B_Mcs03_Mcs00, priv->pshare->phw->power_backup[0x08]);
		RTL_W32(rTxAGC_B_Mcs07_Mcs04, priv->pshare->phw->power_backup[0x09]);
		RTL_W32(rTxAGC_B_Mcs11_Mcs08, priv->pshare->phw->power_backup[0x0a]);
		RTL_W32(rTxAGC_B_Mcs15_Mcs12, priv->pshare->phw->power_backup[0x0b]);
		RTL_W32(rTxAGC_A_CCK11_2_B_CCK11, priv->pshare->phw->power_backup[0x0c]);
		RTL_W32(rTxAGC_A_CCK1_Mcs32,  priv->pshare->phw->power_backup[0x0d]);
		RTL_W32(rTxAGC_B_CCK5_1_Mcs32,  priv->pshare->phw->power_backup[0x0e]);
		priv->pshare->phw->lower_tx_power = 0;
		RESTORE_INT(x);
	}
}
#endif


int get_CCK_swing_index(struct rtl8192cd_priv *priv)
{
	int TempCCk, index=12, i;
	short channel;
#ifdef MP_TEST
	if ((OPMODE & WIFI_MP_STATE) || priv->pshare->rf_ft_var.mp_specific)
		channel=priv->pshare->working_channel;
	else
#endif
		channel = (priv->pmib->dot11RFEntry.dot11channel);

	//Query CCK default setting From 0xa24
	TempCCk = PHY_QueryBBReg(priv, rCCK0_TxFilter2, bMaskDWord)&bMaskCCK;
	TempCCk = cpu_to_le32(TempCCk);
	for(i=0 ; i<CCK_TABLE_SIZE ; i++)		{
		if(channel==14) {
			if(memcmp((void*)&TempCCk, (void*)&CCKSwingTable_Ch14[i][2], 4)==0) {
				index = i;
				break;
			}
		} else {
			if(memcmp((void*)&TempCCk, (void*)&CCKSwingTable_Ch1_Ch13[i][2], 4)==0) {
				index = i;
				break;
			}
		}
	}
	DEBUG_INFO("Initial reg0x%x = 0x%lx, CCK_index=0x%x, ch %d\n",
							rCCK0_TxFilter2, TempCCk, index, channel);
	return index;
}

void set_CCK_swing_index(struct rtl8192cd_priv *priv, short CCK_index)
{
	short channel;
#ifdef MP_TEST
	if ((OPMODE & WIFI_MP_STATE) || priv->pshare->rf_ft_var.mp_specific)
		channel=priv->pshare->working_channel;
	else
#endif
		channel = (priv->pmib->dot11RFEntry.dot11channel);

	if(channel !=14) {
		RTL_W8( 0xa22, CCKSwingTable_Ch1_Ch13[CCK_index][0]);
		RTL_W8( 0xa23, CCKSwingTable_Ch1_Ch13[CCK_index][1]);
		RTL_W8( 0xa24, CCKSwingTable_Ch1_Ch13[CCK_index][2]);
		RTL_W8( 0xa25, CCKSwingTable_Ch1_Ch13[CCK_index][3]);
		RTL_W8( 0xa26, CCKSwingTable_Ch1_Ch13[CCK_index][4]);
		RTL_W8( 0xa27, CCKSwingTable_Ch1_Ch13[CCK_index][5]);
		RTL_W8( 0xa28, CCKSwingTable_Ch1_Ch13[CCK_index][6]);
		RTL_W8( 0xa29, CCKSwingTable_Ch1_Ch13[CCK_index][7]);
	}
	else{
		RTL_W8( 0xa22, CCKSwingTable_Ch14[CCK_index][0]);
		RTL_W8( 0xa23, CCKSwingTable_Ch14[CCK_index][1]);
		RTL_W8( 0xa24, CCKSwingTable_Ch14[CCK_index][2]);
		RTL_W8( 0xa25, CCKSwingTable_Ch14[CCK_index][3]);
		RTL_W8( 0xa26, CCKSwingTable_Ch14[CCK_index][4]);
		RTL_W8( 0xa27, CCKSwingTable_Ch14[CCK_index][5]);
		RTL_W8( 0xa28, CCKSwingTable_Ch14[CCK_index][6]);
		RTL_W8( 0xa29, CCKSwingTable_Ch14[CCK_index][7]);
	}
}

unsigned char getThermalValue(struct rtl8192cd_priv *priv)
{
	unsigned char	ThermalValue;
	int sum=0, i=0;
	PHY_SetRFReg(priv, RF92CD_PATH_A, RF_T_METER, bMask20Bits, 0x60);
	while ((PHY_QueryRFReg(priv, RF92CD_PATH_A, RF_T_METER, bMask20Bits, 1) > 0x1f) && ((i++) < 1000)) {//<20ms, test is in 20 us
		delay_us(20);
	}	
	ThermalValue =(unsigned char)PHY_QueryRFReg(priv, RF92CD_PATH_A, RF_T_METER, bMask20Bits, 1) & 0x01f;
	priv->pshare->Thermal_idx = (priv->pshare->Thermal_idx+1)%8;
	priv->pshare->Thermal_log[ priv->pshare->Thermal_idx ] = ThermalValue;
	for(i=0; i<8; i++) {
		if(!priv->pshare->Thermal_log[i])
			return ThermalValue;
		sum += priv->pshare->Thermal_log[i];
	}
	return (sum+4)>>3;
}



#ifdef _TRACKING_TABLE_FILE
int get_tx_tracking_index(struct rtl8192cd_priv *priv, int channel, int i, int delta, int is_decrease, int is_CCK)
{
	int index = 0;

	if(delta == 0)
		return 0; 

	if(delta > index_mapping_NUM_MAX)
		delta = index_mapping_NUM_MAX;

	printk("\n\n_eric_tracking +++ channel = %d, i = %d, delta = %d, is_decrease = %d, is_CCK = %d\n", 
					channel, i, delta, is_decrease, is_CCK);

	delta = delta - 1;

	if (channel > 14)
	{
		if(channel <= 99)
		{
			index = priv->pshare->txpwr_tracking_5GL[(i*2)+ is_decrease][delta];
		}
		else if(channel <= 140)
		{

			index = priv->pshare->txpwr_tracking_5GM[(i*2)+ is_decrease][delta];
		}
		else
		{
			index = priv->pshare->txpwr_tracking_5GH[(i*2)+ is_decrease][delta];
		}
	}
	else
	{
		if(is_CCK)
		{
			index = priv->pshare->txpwr_tracking_2G_CCK[(i*2)+ is_decrease][delta];
		}
		else
		{
			index = priv->pshare->txpwr_tracking_2G_OFDM[(i*2)+ is_decrease][delta];
		}
	}

	printk("_eric_tracking +++ offset = %d\n\n", index);

	return index; 

}
#endif


#ifdef CONFIG_RTL_92C_SUPPORT	

#ifdef HIGH_POWER_EXT_PA
void swingIndexRemap2(int *a, int b, int i)
{

	u8		index_mapping_HighPower_92C[4][15] = {
			{0,  1,  3,  4,  6,  7,  9, 10, 12, 13, 15, 16, 18, 18, 18}, //2.4G, path A/MAC 0, decrease power
			{0,  2,  4,  5,  7,  8, 10, 11, 13, 14, 16, 17, 19, 20, 22}, //2.4G, path A/MAC 0, increase power
			{0,  1,  3,  4,  6,  7,  9, 10, 12, 13, 15, 16, 18, 18, 18}, //2.4G, path A/MAC 0, decrease power
			{0,  3,  5,  6,  8,  9, 11, 12, 14, 15, 17, 18, 20, 21, 23}, //2.4G, path A/MAC 0, increase power
			};

	int d = RTL_ABS(*a, b);
	int offset = 0;


	if(i == 0)
		offset = 1;
	else 
		offset = 3;

	if(*a < b )
	{
		//printk("\n\n  Increase Power !! \n\n");
		*a = b - index_mapping_HighPower_92C[offset][d];
	}
	else
	{
		//printk("\n\n  Decrease Power !! \n\n");
		offset = offset - 1;
		*a = b + index_mapping_HighPower_92C[offset][d];
	}

	//printk("\n\ a = %d, b = %d, offset = %d, d = %d, diff = %d \n\n", 
		//*a, b, offset, d, index_mapping_HighPower_92C[offset][d]);
	
}
void swingIndexRemap(int *a, int b)
{
	int d = (RTL_ABS(*a, b) *3)>>1;
	if(*a < b )
		*a = b - d;
	else
		*a = b + d;
}
#endif

void tx_power_tracking(struct rtl8192cd_priv *priv)
{
	unsigned char	ThermalValue = 0, delta, delta_LCK, delta_IQK;
	int 			ele_A, ele_D, value32, X, Y, ele_C;
	int			OFDM_index[2]={0,0}, CCK_index;
	int	    		i = 0;
	char			is2T = ((GET_CHIP_VER(priv) == VERSION_8192C) ?1 :0);
	unsigned char		TxPwrLevel[2];
	unsigned char 		channel, OFDM_min_index = 6, rf=1; //OFDM BB Swing should be less than +3.0dB, which is required by Arthur
#ifdef MP_TEST
	if ((OPMODE & WIFI_MP_STATE) || priv->pshare->rf_ft_var.mp_specific) {
		channel=priv->pshare->working_channel;
		if(priv->pshare->mp_txpwr_tracking == FALSE)
			return;
	} else
#endif
	{
		channel = (priv->pmib->dot11RFEntry.dot11channel);
	}

	ThermalValue = getThermalValue(priv);

	rf += is2T;
	if(ThermalValue)	{

		if(!priv->pshare->ThermalValue)	{
			priv->pshare->ThermalValue = priv->pmib->dot11RFEntry.ther;
			priv->pshare->ThermalValue_LCK = ThermalValue;
			priv->pshare->ThermalValue_IQK = ThermalValue;

			//Query OFDM path A default setting
			ele_D = PHY_QueryBBReg(priv, rOFDM0_XATxIQImbalance, bMaskDWord)&bMaskOFDM_D;
			for(i=0; i<OFDM_TABLE_SIZE; i++)	{
				if(ele_D == (OFDMSwingTable[i]&bMaskOFDM_D))	{
					priv->pshare->OFDM_index[0] = i;
					priv->pshare->OFDM_index0[0] = i;
					break;
				}
			}

			//Query OFDM path B default setting
			if(is2T)	{
				ele_D = PHY_QueryBBReg(priv, rOFDM0_XBTxIQImbalance, bMaskDWord)&bMaskOFDM_D;
				for(i=0; i<OFDM_TABLE_SIZE; i++)			{
					if(ele_D == (OFDMSwingTable[i]&bMaskOFDM_D))	{
						priv->pshare->OFDM_index[1] = i;
						priv->pshare->OFDM_index0[1] = i;
						break;
					}
				}
			}
			priv->pshare->CCK_index = get_CCK_swing_index(priv);
			priv->pshare->CCK_index0 = priv->pshare->CCK_index;

		}

		delta     = RTL_ABS(ThermalValue, priv->pshare->ThermalValue);
		delta_LCK = RTL_ABS(ThermalValue, priv->pshare->ThermalValue_LCK);
		delta_IQK = RTL_ABS(ThermalValue, priv->pshare->ThermalValue_IQK);

//		printk("Readback Thermal Meter = 0x%lx pre thermal meter 0x%lx EEPROMthermalmeter 0x%lx delta 0x%lx delta_LCK 0x%lx delta_IQK 0x%lx\n",
//			ThermalValue, priv->pshare->ThermalValue, priv->pmib->dot11RFEntry.ther, delta, delta_LCK, delta_IQK);

		if(delta_LCK > 1)	{
			priv->pshare->ThermalValue_LCK = ThermalValue;

#ifdef MP_TEST
			if(priv->pshare->rf_ft_var.mp_specific)
			{
				if((OPMODE & WIFI_MP_CTX_BACKGROUND) && !(OPMODE & WIFI_MP_CTX_PACKET))
					printk("NOT do LCK during ctx !!!! \n"); 
				else
			PHY_LCCalibrate(priv);
		}
			else
#endif
				PHY_LCCalibrate(priv);
			
		}

		if(delta > 0)	{
			if(ThermalValue > priv->pshare->ThermalValue)	{
				for(i = 0; i < rf; i++)
				 	priv->pshare->OFDM_index[i] -= delta;
				priv->pshare->CCK_index -= delta;
			} else {
				for(i = 0; i < rf; i++)
					priv->pshare->OFDM_index[i] += delta;
				priv->pshare->CCK_index += delta;
			}
			if(ThermalValue > priv->pmib->dot11RFEntry.ther)	{
				for(i = 0; i < rf; i++)
					OFDM_index[i] = priv->pshare->OFDM_index[i]+1;
				CCK_index = priv->pshare->CCK_index+1;
			} else {
				for(i = 0; i < rf; i++)
					OFDM_index[i] = priv->pshare->OFDM_index[i];
				CCK_index = priv->pshare->CCK_index;
			}
#ifdef MP_TEST
			if ((OPMODE & WIFI_MP_STATE) || priv->pshare->rf_ft_var.mp_specific) {
				TxPwrLevel[0] = priv->pshare->mp_txpwr_patha;
				TxPwrLevel[1] = priv->pshare->mp_txpwr_pathb;
			} else
#endif
			{
				TxPwrLevel[0] = priv->pmib->dot11RFEntry.pwrlevelHT40_1S_A[channel-1];
				TxPwrLevel[1] = priv->pmib->dot11RFEntry.pwrlevelHT40_1S_B[channel-1];

				if (priv->pshare->CurrentChannelBW == HT_CHANNEL_WIDTH_20) {
					unsigned char offset = (priv->pmib->dot11RFEntry.pwrdiffHT20[channel-1] & 0x0f);
					TxPwrLevel[0] = COUNT_SIGN_OFFSET(TxPwrLevel[0], offset);
					offset = ((priv->pmib->dot11RFEntry.pwrdiffOFDM[channel-1] & 0xf0) >> 4);
					TxPwrLevel[1] = COUNT_SIGN_OFFSET(TxPwrLevel[1], offset);
				}
			}

//			printk("TxPwrLevel[0]=%d, TxPwrLevel[1]=%d\n", TxPwrLevel[0], TxPwrLevel[1]);
			for(i = 0; i < rf; i++)		{
				if(/*TxPwrLevel[i] >=0 &&*/ TxPwrLevel[i] <=26)	{
					if(ThermalValue > priv->pmib->dot11RFEntry.ther) {
						if (delta < 5)
							OFDM_index[i] -= 1;
						else
							OFDM_index[i] -= 2;
					} else if(delta > 5 && ThermalValue < priv->pmib->dot11RFEntry.ther) {
						OFDM_index[i] += 1;
					}
				} else if (TxPwrLevel[i] >= 27 && TxPwrLevel[i] <= 32 && ThermalValue > priv->pmib->dot11RFEntry.ther) {
					if (delta < 5)
						OFDM_index[i] -= 1;
					else
						OFDM_index[i] -= 2;
				} else if (TxPwrLevel[i] >= 32 && TxPwrLevel[i] <= 38 && ThermalValue > priv->pmib->dot11RFEntry.ther && delta > 5) {
					OFDM_index[i] -= 1;
				}

#ifdef _TRACKING_TABLE_FILE

				{
					int d = 0; 
					
					OFDM_index[i] = priv->pshare->OFDM_index[i];
					d = RTL_ABS(OFDM_index[i], priv->pshare->OFDM_index0[i]);

					if(OFDM_index[i] < priv->pshare->OFDM_index0[i])
					{
						OFDM_index[i] = priv->pshare->OFDM_index0[i] - get_tx_tracking_index(priv, channel, i, d, 0, 0);
					}
					else
					{
						OFDM_index[i] = priv->pshare->OFDM_index0[i] + get_tx_tracking_index(priv, channel, i, d, 1, 0);
					}

				}


#else
#ifdef HIGH_POWER_EXT_PA
				if (priv->pshare->rf_ft_var.use_ext_pa) {
					OFDM_index[i] = priv->pshare->OFDM_index[i];
					swingIndexRemap2(&OFDM_index[i], priv->pshare->OFDM_index0[i], i); //Modify HP tracking table, from Arthur 2012.02.13
					//swingIndexRemap(&OFDM_index[i], priv->pshare->OFDM_index0[i]);
				}
#endif
#endif
				if(OFDM_index[i] > OFDM_TABLE_SIZE-1)
					OFDM_index[i] = OFDM_TABLE_SIZE-1;
				else if (OFDM_index[i] < OFDM_min_index)
					OFDM_index[i] = OFDM_min_index;
			}
			i=0;
			{
				if(/*TxPwrLevel[i] >=0 &&*/ TxPwrLevel[i] <=26)		{
					if(ThermalValue > priv->pmib->dot11RFEntry.ther)	{
						if (delta < 5)
							CCK_index -= 1;
						else
							CCK_index -= 2;
					} else if(delta > 5 && ThermalValue < priv->pmib->dot11RFEntry.ther) {
						CCK_index += 1;
					}
				} else if (TxPwrLevel[i] >= 27 && TxPwrLevel[i] <= 32 && ThermalValue > priv->pmib->dot11RFEntry.ther) {
					if (delta < 5)
						CCK_index -= 1;
					else
						CCK_index -= 2;
				} else if (TxPwrLevel[i] >= 32 && TxPwrLevel[i] <= 38 && ThermalValue > priv->pmib->dot11RFEntry.ther && delta > 5) {
					CCK_index -= 1;
				}

#ifdef _TRACKING_TABLE_FILE
				 {
					int d = 0; 
					
					CCK_index = priv->pshare->CCK_index;
					d = RTL_ABS(CCK_index, priv->pshare->CCK_index0);

					if(CCK_index < priv->pshare->CCK_index0)
					{
						CCK_index = priv->pshare->CCK_index0 - get_tx_tracking_index(priv, channel, i, d, 0, 1);
					}
					else
					{
						CCK_index = priv->pshare->CCK_index0 + get_tx_tracking_index(priv, channel, i, d, 1, 1);
					}

				}
#else
#ifdef HIGH_POWER_EXT_PA
				if (priv->pshare->rf_ft_var.use_ext_pa) {
					CCK_index = priv->pshare->CCK_index;
					swingIndexRemap2( &CCK_index, priv->pshare->CCK_index0, i); //Modify HP tracking table, from Arthur 2012.02.13
					//swingIndexRemap( &CCK_index, priv->pshare->CCK_index0);
				}
#endif
#endif
				if(CCK_index > CCK_TABLE_SIZE-1)
					CCK_index = CCK_TABLE_SIZE-1;
				else if (CCK_index < 0)
					CCK_index = 0;
			}

			//Adujst OFDM Ant_A according to IQK result
			ele_D = (OFDMSwingTable[(unsigned int)OFDM_index[0]] & 0xFFC00000)>>22;
			X = priv->pshare->RegE94;
			Y = priv->pshare->RegE9C;

			if(X != 0) {
				if ((X & 0x00000200) != 0)
					X = X | 0xFFFFFC00;
				ele_A = ((X * ele_D)>>8)&0x000003FF;

				//new element C = element D x Y
				if ((Y & 0x00000200) != 0)
					Y = Y | 0xFFFFFC00;
				ele_C = ((Y * ele_D)>>8)&0x000003FF;

				//wirte new elements A, C, D to regC80 and regC94, element B is always 0
				value32 = (ele_D<<22)|((ele_C&0x3F)<<16)|ele_A;
				PHY_SetBBReg(priv, rOFDM0_XATxIQImbalance, bMaskDWord, value32);

				value32 = (ele_C&0x000003C0)>>6;
				PHY_SetBBReg(priv, rOFDM0_XCTxAFE, bMaskH4Bits, value32);

				value32 = ((X * ele_D)>>7)&0x01;
				PHY_SetBBReg(priv, rOFDM0_ECCAThreshold, BIT(24), value32);

			} else {
				PHY_SetBBReg(priv, rOFDM0_XATxIQImbalance, bMaskDWord, OFDMSwingTable[(unsigned int)OFDM_index[0]]);
				PHY_SetBBReg(priv, rOFDM0_XCTxAFE, bMaskH4Bits, 0x00);
				PHY_SetBBReg(priv, rOFDM0_ECCAThreshold, BIT(24), 0x00);
			}


			set_CCK_swing_index(priv, CCK_index);


			if(is2T) {
				ele_D = (OFDMSwingTable[(unsigned int)OFDM_index[1]] & 0xFFC00000)>>22;
				X = priv->pshare->RegEB4;
				Y = priv->pshare->RegEBC;

				if(X != 0) {
					if ((X & 0x00000200) != 0)	//consider minus
						X = X | 0xFFFFFC00;
					ele_A = ((X * ele_D)>>8)&0x000003FF;

					//new element C = element D x Y
					if ((Y & 0x00000200) != 0)
						Y = Y | 0xFFFFFC00;
					ele_C = ((Y * ele_D)>>8)&0x00003FF;

					//wirte new elements A, C, D to regC88 and regC9C, element B is always 0
					value32=(ele_D<<22)|((ele_C&0x3F)<<16) |ele_A;
					PHY_SetBBReg(priv, rOFDM0_XBTxIQImbalance, bMaskDWord, value32);

					value32 = (ele_C&0x000003C0)>>6;
					PHY_SetBBReg(priv, rOFDM0_XDTxAFE, bMaskH4Bits, value32);

					value32 = ((X * ele_D)>>7)&0x01;
					PHY_SetBBReg(priv, rOFDM0_ECCAThreshold, BIT(28), value32);

				} else {
					PHY_SetBBReg(priv, rOFDM0_XBTxIQImbalance, bMaskDWord, OFDMSwingTable[(unsigned int)OFDM_index[1]]);
					PHY_SetBBReg(priv, rOFDM0_XDTxAFE, bMaskH4Bits, 0x00);
					PHY_SetBBReg(priv, rOFDM0_ECCAThreshold, BIT(28), 0x00);
				}
			}
		}

		if(delta_IQK > 3) {
			priv->pshare->ThermalValue_IQK = ThermalValue;

#ifdef MP_TEST
			if(priv->pshare->rf_ft_var.mp_specific)
			{
				if((OPMODE & WIFI_MP_CTX_BACKGROUND) && !(OPMODE & WIFI_MP_CTX_PACKET))
					printk("NOT do IQK during ctx !!!! \n"); 
				else
					PHY_IQCalibrate(priv);
			}
			else
#endif
			PHY_IQCalibrate(priv);
		}

		//update thermal meter value
		priv->pshare->ThermalValue = ThermalValue;

	}
}
#endif


#ifdef RX_GAIN_TRACK_92D
static void rx_gain_tracking_92D(struct rtl8192cd_priv *priv)
{
	u8	index_mapping[Rx_index_mapping_NUM] = {
						0x0f,	0x0f,	0x0f,	0x0f,	0x0b,
						0x0a,	0x09,	0x08,	0x07,	0x06,
						0x05,	0x04,	0x04,	0x03,	0x02
					};

	u8	eRFPath, curMaxRFPath;
	u32	u4tmp;

	u4tmp = (index_mapping[(priv->pmib->dot11RFEntry.ther - priv->pshare->ThermalValue_RxGain)]) << 12;

	DEBUG_INFO("===>%s interface %d  Rx Gain %x\n", __FUNCTION__, priv->pshare->wlandev_idx, u4tmp);

	if (priv->pmib->dot11RFEntry.macPhyMode == DUALMAC_DUALPHY)
		curMaxRFPath = RF92CD_PATH_B;
	else
		curMaxRFPath = RF92CD_PATH_MAX;

	for(eRFPath = RF92CD_PATH_A; eRFPath < curMaxRFPath; eRFPath++)
		PHY_SetRFReg(priv, eRFPath, 0x3C, bMask20Bits, (priv->pshare->RegRF3C[eRFPath]&(~(0xF000)))|u4tmp);

};

#endif

#ifdef CONFIG_RTL_92D_SUPPORT

void getDeltaValue(struct rtl8192cd_priv *priv)
{
	unsigned int tempval[2];

	tempval[0] = priv->pmib->dot11RFEntry.deltaIQK;
	tempval[1] = priv->pmib->dot11RFEntry.deltaLCK;

	switch(tempval[0])
	{
		case 0:
			tempval[0] = 5;
			break;

		case 1:
			tempval[0] = 4;
			break;

		case 2:
			tempval[0] = 3;
			break;

		case 3:
		default:
			tempval[0] = 0;
			break;
	}

	switch(tempval[1])
	{
		case 0:
			tempval[1] = 4;
			break;

		case 1:
			tempval[1] = 3;
			break;

		case 2:
			tempval[1] = 2;
			break;

		case 3:
		default:
			tempval[1] = 0;
			break;
	}

	priv->pshare->Delta_IQK = tempval[0];
	priv->pshare->Delta_LCK = tempval[1];
}

void tx_power_tracking_92D(struct rtl8192cd_priv *priv)
{
	u8		ThermalValue = 0, delta, delta_LCK, delta_IQK, index[2], offset, ThermalValue_AVG_count = 0;
	u32		ThermalValue_AVG = 0;
	int 	ele_A, ele_D, X, value32, Y, ele_C;
	char	OFDM_index[2], CCK_index=0;
	int	   	i = 0;
	char	is2T = ((priv->pmib->dot11RFEntry.macPhyMode != DUALMAC_DUALPHY) ?1 :0);
	u8 		OFDM_min_index = 6, OFDM_min_index_internalPA = 5, rf=1, channel; //OFDM BB Swing should be less than +3.0dB, which is required by Arthur	u1Byte			OFDM_min_index = 6, rf; //OFDM BB Swing should be less than +3.0dB, which is required by Arthur

	u8		index_mapping[5][index_mapping_NUM] = {
					{0,	1,	3,	6,	8,	9,				//5G, path A/MAC 0, decrease power
					11,	13,	14,	16,	17,	18, 18},
					{0,	2,	4,	5,	7,	10,				//5G, path A/MAC 0, increase power
					12,	14,	16,	18,	18,	18,	18},
					{0,	2,	3,	6,	8,	9,				//5G, path B/MAC 1, decrease power
					11,	13,	14,	16,	17,	18,	18},
					{0,	2,	4,	5,	7,	10,				//5G, path B/MAC 1, increase power
					13,	16,	16,	18,	18,	18,	18},
					{0,	1,	2,	3,	4,	5,				//2.4G, for decreas power
					6,	7,	7,	8,	9,	10,	10},
					};

#if defined(RTL8192D_INT_PA)

	u8		index_mapping_internalPA[8][index_mapping_NUM] = {
					 {0,  1, 3, 4, 6, 7,	//5G, path A/MAC 0, ch36-64, decrease power
					 9,  11,  13,  15,	16,  16,  16},
					 {0,  1, 3, 4, 6, 7,	 //5G, path A/MAC 0, ch36-64, increase power
					 9,  11,  13,  15,	16,  18,  20},
					 {0,  1, 3, 4, 6, 7,	//5G, path A/MAC 0, ch100-165, decrease power
					 9, 11,  13,  15,  16,	16,  16},
					 {0,  1, 3, 4, 6, 7,	 //5G, path A/MAC 0, ch100-165, increase power
					 9,  11,  13,  15,	16,  18,  20},
					 {0,  1, 3, 4, 6, 7,	//5G, path B/MAC 1, ch36-64, decrease power
					 9, 11,  13,  15,  16,	16,  16},
					 {0,  1, 3, 4, 6, 7,	 //5G, path B/MAC 1, ch36-64, increase power
					 9,  11,  13,  15,	16,  18,  20},
					 {0,  1, 3, 4, 6, 7,	//5G, path B/MAC 1, ch100-165, decrease power
					 9, 11, 13,  15,  16,  16,	16},
					 {0,  1, 3, 4, 6, 7,	 //5G, path B/MAC 1, ch100-165, increase power
					 9,  11,  13,  15,	16,  18,  20},
					};	

	u8			bInteralPA[2];	
				
#endif

#ifdef DPK_92D
	short	index_mapping_DPK[4][index_mapping_DPK_NUM]={
				{0, 0,	1,	2,	2,				//path A current thermal > PG thermal
				3,	4,	5,	5,	6,		
				7,	7,	8,	9,	9},
				{0, 0,	-1, -2, -3, 			//path A current thermal < PG thermal
				-3, -4, -5, -6, -6, 	
				-7, -8, -9, -9, -10},
				{0, 0,	1,	2,	2,				//path B current thermal > PG thermal
				3,	4,	5,	5,	6,		
				7,	7,	8,	9,	9},
				{0, 0,	-1, -2, -3, 			//path B current thermal < PG thermal
				-3, -4, -5, -6, -6, 	
				-7, -8, -9, -9, -10}					
				};

	u8		delta_DPK;
	short	index_DPK[2] = { 0xb68,	0xb6c }, value_DPK, value_DPK_shift;
	int j;

	if(priv->pshare->bDPKworking) {
		DEBUG_INFO("DPK in progress abort tx power tracking \n");
		return; 
	}

#endif


#ifdef HIGH_POWER_EXT_PA //Modify HP tracking table, from Arthur 2012.02.13

u8		index_mapping_HighPower_PA[12][index_mapping_NUM] = {
		{0,  2,  3,  4,  7,  8, 10, 12, 13, 15,	16, 17, 18}, //5G, path A/MAC 0, ch36-64, decrease power
		{0,  2,  4,  7,  8, 10,	11, 15, 17, 19,	21, 23, 23}, //5G, path A/MAC 0, ch36-64, increase power
		{0,  4,  5,  8,  9, 11, 14, 15, 16, 17,	18, 19, 20}, //5G, path A/MAC 0, ch100-140, decrease power
		{0,  2,  4,  5,  7,  9,	13, 15, 19, 21,	22, 23, 23}, //5G, path A/MAC 0, ch100-140, increase power
		{0,  4,  5,  8,  9, 11, 14, 15, 17, 18, 19, 20, 21}, //5G, path A/MAC 0, ch149-165, decrease power
		{0,  2,  4,  6,  8, 10, 14, 16, 19, 21, 22, 24, 24}, //5G, path A/MAC 0, ch149-165, increase power
		{0,  4,  5,  6,  8,  9, 11, 12, 13, 14,	15, 16, 17}, //5G, path B/MAC 1, ch36-64, decrease power
		{0,  2,  4,  7,  8, 10,	11, 15, 17, 19,	21, 23, 23}, //5G, path B/MAC 1, ch36-64, increase power
		{0,  3,  4,  6,  7,  9, 12, 13, 14, 15,	17, 18, 19}, //5G, path B/MAC 1, ch100-140, decrease power
		{0,  2,  4,  5,  7,  9,	13, 15, 19, 21,	22, 23, 23}, //5G, path B/MAC 1, ch100-140, increase power
		{0,  3,  4,  6,  7,  9, 12, 13, 15, 16, 17, 18, 19}, //5G, path B/MAC 1, ch149-165, decrease power
		{0,  3,  5,  7,  9, 11, 13, 17, 19, 21, 22, 23, 23}, //5G, path B/MAC 1, ch149-165, increase power
		};

#endif

#ifdef MP_TEST
	if ((OPMODE & WIFI_MP_STATE) || priv->pshare->rf_ft_var.mp_specific) {
		channel=priv->pshare->working_channel;
		if(priv->pshare->mp_txpwr_tracking == FALSE)
			return;
	} else
#endif
	{
		channel = (priv->pmib->dot11RFEntry.dot11channel);
	}

	if (priv->pshare->pwr_trk_ongoing==0) {
		PHY_SetRFReg(priv, RF92CD_PATH_A, RF_T_METER_92D, bMask20Bits, 0x30000);
		priv->pshare->pwr_trk_ongoing = 1;
		return;
	}else{
		ThermalValue =(unsigned char)PHY_QueryRFReg(priv, RF92CD_PATH_A, RF_T_METER_92D, 0xf800, 1);
		priv->pshare->pwr_trk_ongoing = 0;
#ifdef DPK_92D
		priv->pshare->ThermalValue_DPKtrack = ThermalValue;
#endif
	}
	DEBUG_INFO("Readback Thermal Meter = 0x%lx pre thermal meter 0x%lx EEPROMthermalmeter 0x%lx\n", ThermalValue,
				priv->pshare->ThermalValue, priv->pmib->dot11RFEntry.ther);

	if(is2T)
		rf = 2;
	else
		rf = 1;

	if (ThermalValue) {

		//Query OFDM path A default setting
		ele_D = PHY_QueryBBReg(priv, rOFDM0_XATxIQImbalance, bMaskDWord)&bMaskOFDM_D;
		for(i=0; i<OFDM_TABLE_SIZE_92D; i++) {	//find the index
			if(ele_D == (OFDMSwingTable_92D[i]&bMaskOFDM_D))	{
				priv->pshare->OFDM_index0[0] = i;
				DEBUG_INFO("Initial pathA ele_D reg0x%x = 0x%lx, OFDM_index=0x%x\n",
						rOFDM0_XATxIQImbalance, ele_D, priv->pshare->OFDM_index0[0]);
				break;
			}
		}

		//Query OFDM path B default setting
		if(is2T)	{
			ele_D = PHY_QueryBBReg(priv, rOFDM0_XBTxIQImbalance, bMaskDWord)&bMaskOFDM_D;
			for(i=0; i<OFDM_TABLE_SIZE_92D; i++)			{
				if(ele_D == (OFDMSwingTable_92D[i]&bMaskOFDM_D))	{
					priv->pshare->OFDM_index0[1] = i;
					DEBUG_INFO("Initial pathB ele_D reg0x%x = 0x%lx, OFDM_index=0x%x\n",
							rOFDM0_XBTxIQImbalance, ele_D, priv->pshare->OFDM_index0[1]);
					break;
				}
			}
		}

		if(priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_2G) {
			priv->pshare->CCK_index0 = get_CCK_swing_index(priv);
		} else {
			priv->pshare->CCK_index0 = 12;
		}

		if(!priv->pshare->ThermalValue)	{
			priv->pshare->ThermalValue = priv->pmib->dot11RFEntry.ther;
			priv->pshare->ThermalValue_LCK = ThermalValue;
			priv->pshare->ThermalValue_IQK = ThermalValue;
#ifdef RX_GAIN_TRACK_92D
			priv->pshare->ThermalValue_RxGain = priv->pmib->dot11RFEntry.ther;
#endif
#ifdef DPK_92D
			priv->pshare->ThermalValue_DPK = ThermalValue;
#endif
			for(i = 0; i < rf; i++)
				priv->pshare->OFDM_index[i] = priv->pshare->OFDM_index0[i];
			priv->pshare->CCK_index = priv->pshare->CCK_index0;
		}


		//calculate average thermal meter
		{
			priv->pshare->Thermal_log[priv->pshare->Thermal_idx] = ThermalValue;
			priv->pshare->Thermal_idx = (priv->pshare->Thermal_idx+1)%8;

			for(i=0; i<8; i++) {
				if(priv->pshare->Thermal_log[i]) {
					ThermalValue_AVG += priv->pshare->Thermal_log[i];
					ThermalValue_AVG_count++;
				}
			}

			if(ThermalValue_AVG_count)
				ThermalValue = (u8)(ThermalValue_AVG / ThermalValue_AVG_count);
		}


		delta     = RTL_ABS(ThermalValue, priv->pshare->ThermalValue);
		delta_LCK = RTL_ABS(ThermalValue, priv->pshare->ThermalValue_LCK);
		delta_IQK = RTL_ABS(ThermalValue, priv->pshare->ThermalValue_IQK);

//		printk("Readback Thermal Meter = 0x%lx pre thermal meter 0x%lx EEPROMthermalmeter 0x%lx delta 0x%lx delta_LCK 0x%lx delta_IQK 0x%lx\n",
//			ThermalValue, priv->pshare->ThermalValue, priv->pmib->dot11RFEntry.ther, delta, delta_LCK, delta_IQK);

		getDeltaValue(priv);

#ifdef DPK_92D

		if(priv->pshare->bDPKstore)	{

			priv->pshare->ThermalValue_DPK = ThermalValue;
			delta_DPK = 0;

			for(j = 0; j < rf; j++)	{

				if(priv->pshare->ThermalValue_DPKstore > priv->pmib->dot11RFEntry.ther)
					value_DPK_shift = index_mapping_DPK[j*2][priv->pshare->ThermalValue_DPKstore- priv->pmib->dot11RFEntry.ther];
				else
					value_DPK_shift = index_mapping_DPK[j*2+1][priv->pmib->dot11RFEntry.ther- priv->pshare->ThermalValue_DPKstore];

				for(i = 0; i < index_mapping_DPK_NUM; i++) 	{
					priv->pshare->index_mapping_DPK_current[j*2][i] = 
						index_mapping_DPK[j*2][i]-value_DPK_shift;
					priv->pshare->index_mapping_DPK_current[j*2+1][i] = 
						index_mapping_DPK[j*2+1][i]-value_DPK_shift;										
				}				
			}		
		}
		else
		{
			delta_DPK = RTL_ABS(ThermalValue, priv->pshare->ThermalValue_DPK);
		}

		for(j = 0; j < rf; j++)			{
			if(!priv->pshare->bDPKdone[j])
				priv->pshare->OFDM_min_index_internalPA_DPK[j] = 0;
		}

#endif

#if 1
		if ((delta_LCK > priv->pshare->Delta_LCK) && (priv->pshare->Delta_LCK != 0)) {
			priv->pshare->ThermalValue_LCK = ThermalValue;
			PHY_LCCalibrate_92D(priv);
		}
#endif
		if(delta > 0
#ifdef DPK_92D
			||(priv->pshare->bDPKstore)			
#endif
		){
			if(delta == 0 && priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_2G)
				goto TxPowerDPK;
#ifdef DPK_92D
			if(priv->pshare->bDPKstore)
				priv->pshare->bDPKstore = FALSE;
#endif
			delta	= RTL_ABS(ThermalValue, priv->pmib->dot11RFEntry.ther);

			//calculate new OFDM / CCK offset
			{
				if (priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_2G){

#ifdef _TRACKING_TABLE_FILE
				if(ThermalValue > priv->pmib->dot11RFEntry.ther)
				{
					for(i = 0; i < rf; i++)
						 OFDM_index[i] = priv->pshare->OFDM_index[i] - get_tx_tracking_index(priv, channel, i, delta, 0, 0);
					
					CCK_index = priv->pshare->CCK_index - get_tx_tracking_index(priv, channel, 0, delta, 0, 1);
				}
				else
				{
					for(i = 0; i < rf; i++)
						OFDM_index[i] = priv->pshare->OFDM_index[i] + get_tx_tracking_index(priv, channel, i, delta, 1, 0);
					
					CCK_index = priv->pshare->CCK_index + get_tx_tracking_index(priv, channel, i, delta, 1, 1);
				}

#else
					offset = 4;

					if(delta > index_mapping_NUM-1)
						index[0] = index_mapping[offset][index_mapping_NUM-1];
					else
						index[0] = index_mapping[offset][delta];

					if(ThermalValue > priv->pmib->dot11RFEntry.ther)	{
						for(i = 0; i < rf; i++)
						 	OFDM_index[i] = priv->pshare->OFDM_index[i] - delta;
						CCK_index = priv->pshare->CCK_index - delta;
					}
					else	{
						for(i = 0; i < rf; i++)
							OFDM_index[i] = priv->pshare->OFDM_index[i] + index[0];
						CCK_index = priv->pshare->CCK_index + index[0];
					}
#endif
				} else if (priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_5G) {
					for(i = 0; i < rf; i++){

#if defined(RTL8192D_INT_PA)

						if (priv->pmib->dot11RFEntry.macPhyMode == DUALMAC_DUALPHY && priv->pshare->wlandev_idx==1)		//MAC 1 5G
							bInteralPA[i] = priv->pshare->phw->InternalPA5G[1];
						else
							bInteralPA[i] = priv->pshare->phw->InternalPA5G[i];	

						if(bInteralPA[i]) {
							if(priv->pshare->wlandev_idx == 1 || i == 1/*rf*/)
								offset = 4;
							else
								offset = 0;
							if(channel >= 100 && channel <= 165)
								offset += 2;													
						}
						else
#endif
						{
							if(priv->pshare->wlandev_idx == 1 || i == 1)
								offset = 2;
							else
								offset = 0;
						}


#ifdef HIGH_POWER_EXT_PA //Modify HP tracking table, from Arthur 2012.02.13
						if(i == 0)
						{
							if(channel <= 99)
								offset = 0; 
							else if(channel <= 140)
								offset = 2;
							else
								offset = 4;
						}
						else
						{
							if(channel <= 99)
								offset = 6; 
							else if(channel <= 140)
								offset = 8;
							else
								offset = 10;
						}
#endif


						if(ThermalValue > priv->pmib->dot11RFEntry.ther) //set larger Tx power
							offset++;
#if defined(RTL8192D_INT_PA)
						if(bInteralPA[i]) {
							if(delta > index_mapping_NUM-1)
								index[i] = index_mapping_internalPA[offset][index_mapping_NUM-1];
							else
								index[i] = index_mapping_internalPA[offset][delta];
						} else
#endif
						{
							if(delta > index_mapping_NUM-1)
								index[i] = index_mapping[offset][index_mapping_NUM-1];
							else
								index[i] = index_mapping[offset][delta];
						}



#ifdef _TRACKING_TABLE_FILE
						{
							if(ThermalValue > priv->pmib->dot11RFEntry.ther)
								index[i] = get_tx_tracking_index(priv, channel, i, delta, 0, 0);	
							else
								index[i] = get_tx_tracking_index(priv, channel, i, delta, 1, 0);
						}

#else

#ifdef HIGH_POWER_EXT_PA //Modify HP tracking table, from Arthur 2012.02.13
  						{
							if(delta > index_mapping_NUM-1)
								index[i] = index_mapping_HighPower_PA[offset][index_mapping_NUM-1];
							else
								index[i] = index_mapping_HighPower_PA[offset][delta];

							//printk("\n\n offset = %d delta = %d \n", offset, delta);
							//printk("index[%d]= %d\n\n", i, index[i]);
						}
#endif

#endif


						if(ThermalValue > priv->pmib->dot11RFEntry.ther) //set larger Tx power
						{
#if 0						
							if(bInteralPA[i] && ThermalValue > 0x12)
								index[i] = ((delta/2)*3+(delta%2));	
#endif							
							OFDM_index[i] = priv->pshare->OFDM_index[i] -index[i];
						}
						else
						{
							OFDM_index[i] = priv->pshare->OFDM_index[i] + index[i];
						}
					}
				}

				if(is2T)
				{
					DEBUG_INFO("temp OFDM_A_index=0x%x, OFDM_B_index=0x%x, CCK_index=0x%x\n",
						priv->pshare->OFDM_index[0], priv->pshare->OFDM_index[1], priv->pshare->CCK_index);
				}
				else
				{
					DEBUG_INFO("temp OFDM_A_index=0x%x, CCK_index=0x%x\n",
						priv->pshare->OFDM_index[0], priv->pshare->CCK_index);
				}

				for(i = 0; i < rf; i++)
				{
					if(OFDM_index[i] > OFDM_TABLE_SIZE_92D-1) {
						OFDM_index[i] = OFDM_TABLE_SIZE_92D-1;
					}
					else if(priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_2G) {
						if (OFDM_index[i] < (OFDM_min_index_internalPA))
							OFDM_index[i] = (OFDM_min_index_internalPA);
					} else if(bInteralPA[i]) {
#ifdef DPK_92D
						if (OFDM_index[i] < (OFDM_min_index_internalPA+ priv->pshare->OFDM_min_index_internalPA_DPK[i]))
						{
							priv->pshare->TxPowerLevelDPK[i] = OFDM_min_index_internalPA+ priv->pshare->OFDM_min_index_internalPA_DPK[i]-OFDM_index[i];
							OFDM_index[i] = (OFDM_min_index_internalPA+ priv->pshare->OFDM_min_index_internalPA_DPK[i]);				
						}
						else
						{
							priv->pshare->TxPowerLevelDPK[i] = 0;
						}
#else
                                                if (OFDM_index[i] < (OFDM_min_index_internalPA))
                                                {
                                                        OFDM_index[i] = (OFDM_min_index_internalPA);                   
                                                }
#endif				
					} else if(OFDM_index[i] < OFDM_min_index) {
#ifdef HIGH_POWER_EXT_PA //Modify HP tracking table, from Arthur 2012.02.13
#else
						OFDM_index[i] = OFDM_min_index;
#endif
					}
				}

				if (priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_2G){
					if(CCK_index > CCK_TABLE_SIZE_92D-1)
						CCK_index = CCK_TABLE_SIZE_92D-1;
					else if (CCK_index < 0)
						CCK_index = 0;
				}

				if(is2T) {
					DEBUG_INFO("new OFDM_A_index=0x%x, OFDM_B_index=0x%x, CCK_index=0x%x\n",
							OFDM_index[0], OFDM_index[1], CCK_index);
				}
				else
				{
					DEBUG_INFO("new OFDM_A_index=0x%x, CCK_index=0x%x\n",
							OFDM_index[0], CCK_index);
				}
			}

			//Config by SwingTable
			{
				//Adujst OFDM Ant_A according to IQK result
				ele_D = (OFDMSwingTable_92D[OFDM_index[0]] & 0xFFC00000)>>22;
				X = priv->pshare->RegE94;
				Y = priv->pshare->RegE9C;

				if(X != 0 && (priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_2G)){
					if ((X & 0x00000200) != 0)
						X = X | 0xFFFFFC00;
					ele_A = ((X * ele_D)>>8)&0x000003FF;

					//new element C = element D x Y
					if ((Y & 0x00000200) != 0)
						Y = Y | 0xFFFFFC00;
					ele_C = ((Y * ele_D)>>8)&0x000003FF;

					//wirte new elements A, C, D to regC80 and regC94, element B is always 0
					value32 = (ele_D<<22)|((ele_C&0x3F)<<16)|ele_A;
					PHY_SetBBReg(priv, rOFDM0_XATxIQImbalance, bMaskDWord, value32);

					value32 = (ele_C&0x000003C0)>>6;
					PHY_SetBBReg(priv, rOFDM0_XCTxAFE, bMaskH4Bits, value32);

					value32 = ((X * ele_D)>>7)&0x01;
					PHY_SetBBReg(priv, rOFDM0_ECCAThreshold, BIT(24), value32);

				}
				else
				{
					PHY_SetBBReg(priv, rOFDM0_XATxIQImbalance, bMaskDWord, OFDMSwingTable_92D[OFDM_index[0]]);
					PHY_SetBBReg(priv, rOFDM0_XCTxAFE, bMaskH4Bits, 0x00);
					PHY_SetBBReg(priv, rOFDM0_ECCAThreshold, BIT(24), 0x00);
#ifdef MP_TEST
					if ((priv->pshare->rf_ft_var.mp_specific) && (!is2T)) {
						unsigned char str[50];
						sprintf(str, "patha=%d,pathb=%d", priv->pshare->mp_txpwr_patha, priv->pshare->mp_txpwr_pathb);
						mp_set_tx_power(priv, str);
					}
#endif
				}
				DEBUG_INFO("TxPwrTracking for interface %d path A: X = 0x%x, Y = 0x%x ele_A = 0x%x ele_C = 0x%x ele_D = 0x%x\n",
							priv->pshare->wlandev_idx, X, Y, ele_A, ele_C, ele_D);


				if(priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_2G)
				{
					//Adjust CCK according to IQK result
					set_CCK_swing_index(priv, CCK_index);
				}

				if(is2T)
				{
					ele_D = (OFDMSwingTable_92D[OFDM_index[1]] & 0xFFC00000)>>22;

					//new element A = element D x X
					X = priv->pshare->RegEB4;
					Y = priv->pshare->RegEBC;

					if(X != 0 && (priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_2G)){
						if ((X & 0x00000200) != 0)	//consider minus
							X = X | 0xFFFFFC00;
						ele_A = ((X * ele_D)>>8)&0x000003FF;

						//new element C = element D x Y
						if ((Y & 0x00000200) != 0)
							Y = Y | 0xFFFFFC00;
						ele_C = ((Y * ele_D)>>8)&0x00003FF;

						//wirte new elements A, C, D to regC88 and regC9C, element B is always 0
						value32=(ele_D<<22)|((ele_C&0x3F)<<16) |ele_A;
						PHY_SetBBReg(priv, rOFDM0_XBTxIQImbalance, bMaskDWord, value32);

						value32 = (ele_C&0x000003C0)>>6;
						PHY_SetBBReg(priv, rOFDM0_XDTxAFE, bMaskH4Bits, value32);

						value32 = ((X * ele_D)>>7)&0x01;
						PHY_SetBBReg(priv, rOFDM0_ECCAThreshold, BIT(28), value32);

					}
					else{
						PHY_SetBBReg(priv, rOFDM0_XBTxIQImbalance, bMaskDWord, OFDMSwingTable_92D[OFDM_index[1]]);
						PHY_SetBBReg(priv, rOFDM0_XDTxAFE, bMaskH4Bits, 0x00);
						PHY_SetBBReg(priv, rOFDM0_ECCAThreshold, BIT(28), 0x00);
#ifdef MP_TEST
					if ((priv->pshare->rf_ft_var.mp_specific) ) {
						unsigned char str[50];
						sprintf(str, "patha=%d,pathb=%d", priv->pshare->mp_txpwr_patha, priv->pshare->mp_txpwr_pathb);
						mp_set_tx_power(priv, str);

					}
#endif				
					}

					DEBUG_INFO("TxPwrTracking path B: X = 0x%x, Y = 0x%x ele_A = 0x%x ele_C = 0x%x ele_D = 0x%x\n",
									X, Y, ele_A, ele_C, ele_D);
				}

				DEBUG_INFO("TxPwrTracking 0xc80 = 0x%x, 0xc94 = 0x%x RF 0x24 = 0x%x\n", PHY_QueryBBReg(priv, 0xc80, bMaskDWord),
						PHY_QueryBBReg(priv, 0xc94, bMaskDWord), PHY_QueryRFReg(priv, RF92CD_PATH_A, 0x24, bMask20Bits,1));
			}
		}

TxPowerDPK:
#ifdef DPK_92D
		{
			char bNOPG = FALSE;
			unsigned char pwrlevelHT40_1S_A = priv->pmib->dot11RFEntry.pwrlevelHT40_1S_A[channel-1];
			if (priv->pmib->dot11RFEntry.phyBandSelect & PHY_BAND_5G)
				pwrlevelHT40_1S_A = priv->pmib->dot11RFEntry.pwrlevel5GHT40_1S_A[channel-1];
#ifdef CONFIG_RTL_92D_DMDP			
			if ((priv->pmib->dot11RFEntry.macPhyMode==DUALMAC_DUALPHY) &&
				(priv->pshare->wlandev_idx == 1) && (priv->pmib->dot11RFEntry.phyBandSelect & PHY_BAND_5G))
					pwrlevelHT40_1S_A = priv->pmib->dot11RFEntry.pwrlevel5GHT40_1S_B[channel-1];
			if (pwrlevelHT40_1S_A == 0)
				bNOPG = TRUE;
#endif

			//for DPK
			if(delta_DPK > 0 && !bNOPG /*&& pHalData->bDPKdone*/) {
				for(i = 0; i < rf; i++) {
					if(bInteralPA[i] && priv->pshare->bDPKdone[i]) {				
						if(ThermalValue > priv->pmib->dot11RFEntry.ther) 	
							value_DPK = priv->pshare->index_mapping_DPK_current[i*2][ThermalValue-priv->pmib->dot11RFEntry.ther];
						else
							value_DPK = priv->pshare->index_mapping_DPK_current[i*2+1][priv->pmib->dot11RFEntry.ther-ThermalValue];
						
						PHY_SetBBReg(priv, index_DPK[i], 0x7c00, value_DPK);						
					}
				}				
				priv->pshare->ThermalValue_DPK = ThermalValue;
			}
		}
#endif
		priv->pshare->pwr_trk_ongoing = 0;
#if 1
		if ((delta_IQK > priv->pshare->Delta_IQK) && (priv->pshare->Delta_IQK != 0)) {
			priv->pshare->ThermalValue_IQK = ThermalValue;
			PHY_IQCalibrate(priv);
		}
#endif

#ifdef RX_GAIN_TRACK_92D
		if(priv->pmib->dot11RFEntry.ther && (priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_5G) &&
			(ThermalValue < priv->pmib->dot11RFEntry.ther)) { 
			priv->pshare->ThermalValue_RxGain = ThermalValue;
			rx_gain_tracking_92D(priv);
		}
#endif

		//update thermal meter value
		priv->pshare->ThermalValue = ThermalValue;
	}
}

#endif

//3 ============================================================
//3 EDCA Turbo
//3 ============================================================

void init_EDCA_para(struct rtl8192cd_priv *priv, int mode)
{
	static unsigned int slot_time, VO_TXOP, VI_TXOP, sifs_time;

     struct ParaRecord EDCA[4];
#ifdef RTL_MANUAL_EDCA
     //unsigned char acm_bitmap;
#endif

	slot_time = 20;
	sifs_time = 10;

	if (mode & WIRELESS_11N)
		sifs_time = 16;

#ifdef RTL_MANUAL_EDCA
	 if( priv->pmib->dot11QosEntry.ManualEDCA ) {
		 memset(EDCA, 0, 4*sizeof(struct ParaRecord));
		 if( OPMODE & WIFI_AP_STATE )
			 memcpy(EDCA, priv->pmib->dot11QosEntry.AP_manualEDCA, 4*sizeof(struct ParaRecord));
		 else
			 memcpy(EDCA, priv->pmib->dot11QosEntry.STA_manualEDCA, 4*sizeof(struct ParaRecord));


		if ((mode & WIRELESS_11N) ||
			(mode & WIRELESS_11G)) {
			slot_time = 9;
		}


		RTL_W32(EDCA_VO_PARA, (EDCA[VO].TXOPlimit << 16) | (EDCA[VO].ECWmax << 12) | (EDCA[VO].ECWmin << 8) | (sifs_time + EDCA[VO].AIFSN * slot_time));
#ifdef WIFI_WMM
		if (QOS_ENABLE)
			RTL_W32(EDCA_VI_PARA, (EDCA[VI].TXOPlimit << 16) | (EDCA[VI].ECWmax << 12) | (EDCA[VI].ECWmin << 8) | (sifs_time + EDCA[VI].AIFSN * slot_time));
		else
#endif
			RTL_W32(EDCA_VI_PARA, (EDCA[BE].TXOPlimit << 16) | (EDCA[BE].ECWmax << 12) | (EDCA[BE].ECWmin << 8) | (sifs_time + EDCA[VI].AIFSN * slot_time));

		RTL_W32(EDCA_BE_PARA, (EDCA[BE].TXOPlimit << 16) | (EDCA[BE].ECWmax << 12) | (EDCA[BE].ECWmin << 8) | (sifs_time + EDCA[BE].AIFSN * slot_time));

		RTL_W32(EDCA_BK_PARA, (EDCA[BK].TXOPlimit << 16) | (EDCA[BK].ECWmax << 12) | (EDCA[BK].ECWmin << 8) | (sifs_time + EDCA[BK].AIFSN * slot_time));
	}else
#endif //RTL_MANUAL_EDCA
	{
		 memset(EDCA, 0, 4*sizeof(struct ParaRecord));
		 /* copy BE, BK from static data */
		 if( OPMODE & WIFI_AP_STATE )
				 memcpy(EDCA, rtl_ap_EDCA, 2*sizeof(struct ParaRecord));
		 else
				 memcpy(EDCA, rtl_sta_EDCA, 2*sizeof(struct ParaRecord));

		 /* VI, VO apply settings in AG by default */
		 if( OPMODE & WIFI_AP_STATE )
				 memcpy(&EDCA[2], &rtl_ap_EDCA[VI_AG], 2*sizeof(struct ParaRecord));
		 else
				 memcpy(&EDCA[2], &rtl_sta_EDCA[VI_AG], 2*sizeof(struct ParaRecord));

		 if ((mode & WIRELESS_11N) ||
				 (mode & WIRELESS_11G)) {
				 slot_time = 9;
		 } else {
				 /* replace with settings in B */
				 if( OPMODE & WIFI_AP_STATE )
						 memcpy(&EDCA[2], &rtl_ap_EDCA[VI], 2*sizeof(struct ParaRecord));
				 else
						 memcpy(&EDCA[2], &rtl_sta_EDCA[VI], 2*sizeof(struct ParaRecord));
		 }
		 VO_TXOP = EDCA[VO].TXOPlimit;
		 VI_TXOP = EDCA[VI].TXOPlimit;

		 RTL_W32(EDCA_VO_PARA, (VO_TXOP << 16) | (EDCA[VO].ECWmax << 12) | (EDCA[VO].ECWmin << 8) | (sifs_time + EDCA[VO].AIFSN * slot_time));
#ifdef WIFI_WMM
		 if (QOS_ENABLE)
				 RTL_W32(EDCA_VI_PARA, (VI_TXOP << 16) | (EDCA[VI].ECWmax << 12) | (EDCA[VI].ECWmin << 8) | (sifs_time + EDCA[VI].AIFSN * slot_time));
		 else
#endif
				 RTL_W32(EDCA_VI_PARA, (EDCA[BK].ECWmax << 12) | (EDCA[BK].ECWmin << 8) | (sifs_time + EDCA[VI].AIFSN * slot_time));

		 RTL_W32(EDCA_BE_PARA, ((EDCA[BE].ECWmax) << 12) | (EDCA[BE].ECWmin << 8) | (sifs_time + EDCA[BE].AIFSN * slot_time));
		 RTL_W32(EDCA_BK_PARA, (EDCA[BK].ECWmax << 12) | (EDCA[BK].ECWmin << 8) | (sifs_time + EDCA[BK].AIFSN * slot_time));


		RTL_W8(ACMHWCTRL, 0x00);
	}

	priv->pshare->iot_mode_enable = 0;
	if (priv->pshare->rf_ft_var.wifi_beq_iot)
		priv->pshare->iot_mode_VI_exist = 0;
	priv->pshare->iot_mode_VO_exist = 0;

#ifdef WMM_VIBE_PRI
	priv->pshare->iot_mode_BE_exist = 0;
#endif
#ifdef LOW_TP_TXOP
	priv->pshare->BE_cwmax_enhance = 0;
#endif
}

void choose_IOT_main_sta(struct rtl8192cd_priv *priv, struct stat_info *pstat)
{
	if ((GET_ROOT(priv)->up_time % 2) == 0) {
		unsigned int tx_2s_avg = 0;
		unsigned int rx_2s_avg = 0;
		int i=0, aggReady=0;
		unsigned long total_sum = (priv->pshare->current_tx_bytes+priv->pshare->current_rx_bytes);

		pstat->current_tx_bytes += pstat->tx_byte_cnt;
		pstat->current_rx_bytes += pstat->rx_byte_cnt;

		if (total_sum != 0) {
			if (total_sum <= 100) {
				tx_2s_avg = (unsigned int)((pstat->current_tx_bytes*100) / total_sum);
				rx_2s_avg = (unsigned int)((pstat->current_rx_bytes*100) / total_sum);
			} else {
				tx_2s_avg = (unsigned int)(pstat->current_tx_bytes / (total_sum / 100));
				rx_2s_avg = (unsigned int)(pstat->current_rx_bytes / (total_sum / 100));
			}
		}

		for(i=0; i<8; i++)
			aggReady += (pstat->ADDBA_ready[i]);
		if (pstat->ht_cap_len && aggReady) {
			if ((tx_2s_avg + rx_2s_avg >= 50)) {
				priv->pshare->highTP_found_pstat = pstat;
			}
#ifdef CLIENT_MODE
			if (OPMODE & WIFI_STATION_STATE) {
				if(pstat->is_ralink_sta && ((tx_2s_avg + rx_2s_avg) >= 45))
					priv->pshare->highTP_found_pstat = pstat;
			}	
#endif				
		}
	}
	else {
		pstat->current_tx_bytes = pstat->tx_byte_cnt;
		pstat->current_rx_bytes = pstat->rx_byte_cnt;
	}
}


void rxBB_dm(struct rtl8192cd_priv *priv)
{
	if ((priv->up_time % 3) == 1) {
		if (priv->pshare->rssi_min != 0xff) {
			if (priv->pshare->rf_ft_var.dig_enable) {
				// for DIG checking
				check_DIG_by_rssi(priv, priv->pshare->rssi_min);
			}
#ifdef INTERFERENCE_CONTROL
			if (priv->pshare->rf_ft_var.nbi_filter_enable) {
				check_NBI_by_rssi(priv, priv->pshare->rssi_min);
			}
#endif
		}

		check_EDCCA(priv, priv->pshare->rssi_min);

#ifdef MP_TEST
		if (!((OPMODE & WIFI_MP_STATE) || priv->pshare->rf_ft_var.mp_specific))
#endif
		{
			if (!priv->pshare->rf_ft_var.use_ext_lna)
				CCK_CCA_dynamic_enhance(priv, priv->pshare->rssi_min);
		}
	}
}

/*
 * IOT related functions
 */
void IOT_engine(struct rtl8192cd_priv *priv)
{
#ifdef WIFI_WMM
	unsigned int switch_turbo = 0;
#endif
	struct stat_info *pstat = priv->pshare->highTP_found_pstat;

#if defined(RTL_MANUAL_EDCA) && defined(WIFI_WMM)
	if(priv->pmib->dot11QosEntry.ManualEDCA)
		return ;
#endif

#ifdef WIFI_WMM
	if (QOS_ENABLE) {
		if (!priv->pmib->dot11OperationEntry.wifi_specific || 
			((OPMODE & WIFI_AP_STATE) && (priv->pmib->dot11OperationEntry.wifi_specific == 2))) {
			if (priv->pshare->iot_mode_enable &&
				((priv->pshare->phw->VO_pkt_count > 50) ||
				(priv->pshare->phw->VI_pkt_count > 50) ||
				(priv->pshare->phw->BK_pkt_count > 50))) {
				priv->pshare->iot_mode_enable = 0;
				switch_turbo++;
			} else if ((!priv->pshare->iot_mode_enable) &&
				((priv->pshare->phw->VO_pkt_count < 50) &&
				(priv->pshare->phw->VI_pkt_count < 50) &&
				(priv->pshare->phw->BK_pkt_count < 50))) {
					priv->pshare->iot_mode_enable++;
					switch_turbo++;
			}
		}

		if ((OPMODE & WIFI_AP_STATE) && priv->pmib->dot11OperationEntry.wifi_specific) {
			if (!priv->pshare->iot_mode_VO_exist && (priv->pshare->phw->VO_pkt_count > 50)) {
				priv->pshare->iot_mode_VO_exist++;
				switch_turbo++;
			} else if (priv->pshare->iot_mode_VO_exist && (priv->pshare->phw->VO_pkt_count < 50)) {
				priv->pshare->iot_mode_VO_exist = 0;
				switch_turbo++;
			}

#ifdef WMM_VIBE_PRI
			if (priv->pshare->iot_mode_VO_exist) {
				//printk("[%s %d] BE_pkt_count=%d\n", __FUNCTION__, __LINE__, priv->pshare->phw->BE_pkt_count);
				if (!priv->pshare->iot_mode_BE_exist && (priv->pshare->phw->BE_pkt_count > 250)) {
					priv->pshare->iot_mode_BE_exist++;
					switch_turbo++;
				} else if (priv->pshare->iot_mode_BE_exist && (priv->pshare->phw->BE_pkt_count < 250)) {
					priv->pshare->iot_mode_BE_exist = 0;
					switch_turbo++;
				}
			}
#endif

			if (priv->pshare->rf_ft_var.wifi_beq_iot) {
				if (!priv->pshare->iot_mode_VI_exist && (priv->pshare->phw->VI_rx_pkt_count > 50)) {
					priv->pshare->iot_mode_VI_exist++;
					switch_turbo++;
				} else if (priv->pshare->iot_mode_VI_exist && (priv->pshare->phw->VI_rx_pkt_count < 50)) {
					priv->pshare->iot_mode_VI_exist = 0;
					switch_turbo++;
				}
			}
		}

#ifdef CLIENT_MODE
        if ((OPMODE & WIFI_STATION_STATE) && (priv->pmib->dot11OperationEntry.wifi_specific == 2))
        {
            if (priv->pshare->iot_mode_enable &&
                (((priv->pshare->phw->VO_pkt_count > 50) ||
                 (priv->pshare->phw->VI_pkt_count > 50) ||
                 (priv->pshare->phw->BK_pkt_count > 50)) ||
                 (pstat && (!pstat->ADDBA_ready[0]) & (!pstat->ADDBA_ready[3]))))
            {
                priv->pshare->iot_mode_enable = 0;
                switch_turbo++;
            }
            else if ((!priv->pshare->iot_mode_enable) &&
                (((priv->pshare->phw->VO_pkt_count < 50) &&
                 (priv->pshare->phw->VI_pkt_count < 50) &&
                 (priv->pshare->phw->BK_pkt_count < 50)) &&
                 (pstat && (pstat->ADDBA_ready[0] | pstat->ADDBA_ready[3]))))
            {
                priv->pshare->iot_mode_enable++;
                switch_turbo++;
            }
        }
#endif

		priv->pshare->phw->VO_pkt_count = 0;
		priv->pshare->phw->VI_pkt_count = 0;
		if (priv->pshare->rf_ft_var.wifi_beq_iot)
			priv->pshare->phw->VI_rx_pkt_count = 0;
		priv->pshare->phw->BK_pkt_count = 0;
#ifdef WMM_VIBE_PRI
		priv->pshare->phw->BE_pkt_count = 0;
#endif
	}
#endif

		if ((priv->up_time % 2) == 0) {
			/*
			 * decide EDCA content for different chip vendor
			 */
#ifdef WIFI_WMM
		if (QOS_ENABLE && (!priv->pmib->dot11OperationEntry.wifi_specific || 
			((OPMODE & WIFI_AP_STATE) && (priv->pmib->dot11OperationEntry.wifi_specific == 2))
#ifdef CLIENT_MODE
            || ((OPMODE & WIFI_STATION_STATE) && (priv->pmib->dot11OperationEntry.wifi_specific == 2))
#endif
			)) {
			if (pstat && pstat->rssi >= priv->pshare->rf_ft_var.txop_enlarge_upper) {
#ifdef LOW_TP_TXOP
				if (pstat->is_intel_sta) {
					if (priv->pshare->txop_enlarge != 0xe) {
						priv->pshare->txop_enlarge = 0xe;
						if (priv->pshare->iot_mode_enable)
							switch_turbo++;
					}
				} else if (priv->pshare->txop_enlarge != 2) {
					priv->pshare->txop_enlarge = 2;
					if (priv->pshare->iot_mode_enable)
						switch_turbo++;
				}
#else
				if (priv->pshare->txop_enlarge != 2) {
					if (pstat->is_intel_sta)
						priv->pshare->txop_enlarge = 0xe;						
					else if (pstat->is_ralink_sta)
						priv->pshare->txop_enlarge = 0xd;						
					else
						priv->pshare->txop_enlarge = 2;

					if (priv->pshare->iot_mode_enable)
						switch_turbo++;
				}
#endif
			} else if (!pstat || pstat->rssi < priv->pshare->rf_ft_var.txop_enlarge_lower) {
				if (priv->pshare->txop_enlarge) {
					priv->pshare->txop_enlarge = 0;
					if (priv->pshare->iot_mode_enable)
						switch_turbo++;
				}
			}
#ifdef LOW_TP_TXOP
			// for Intel IOT, need to enlarge CW MAX from 6 to 10
			if (pstat && pstat->is_intel_sta && (((pstat->tx_avarage+pstat->rx_avarage)>>10) < 
					priv->pshare->rf_ft_var.cwmax_enhance_thd)) {
				if (!priv->pshare->BE_cwmax_enhance && priv->pshare->iot_mode_enable) {
					priv->pshare->BE_cwmax_enhance = 1;
					switch_turbo++;
				}
			} else {
				if (priv->pshare->BE_cwmax_enhance) {
					priv->pshare->BE_cwmax_enhance = 0;
					switch_turbo++;
				}
			}
#endif
		}
#endif

		priv->pshare->current_tx_bytes = 0;
		priv->pshare->current_rx_bytes = 0;
	}

#ifdef SW_TX_QUEUE
	if ((priv->assoc_num > 1) && (AMPDU_ENABLE))
   	{
       	if (priv->swq_txmac_chg >= priv->pshare->rf_ft_var.swq_en_highthd)
        {
			if ((priv->swq_en == 0))
			{
				switch_turbo++;
                if (priv->pshare->txop_enlarge == 0)
					priv->pshare->txop_enlarge = 2;
        	    priv->swq_en = 1;
            }
            else
	        {
				if ((switch_turbo > 0) && (priv->pshare->txop_enlarge == 0) && (priv->pshare->iot_mode_enable != 0))
                {
					priv->pshare->txop_enlarge = 2;
                    switch_turbo--;
	            }
			}
       	}
	    else if(priv->swq_txmac_chg <= priv->pshare->rf_ft_var.swq_dis_lowthd)
        {
			priv->swq_en = 0;
	    }
        else if ((priv->swq_en == 1) && (switch_turbo > 0) && (priv->pshare->txop_enlarge == 0) && (priv->pshare->iot_mode_enable != 0))
        {
           	priv->pshare->txop_enlarge = 2;
	        switch_turbo--;
        }
		
		//debug msg	
		//printk("swq=%d,sw=%d,en=%d,mode=%d\n", priv->swq_en, switch_turbo, priv->pshare->txop_enlarge, priv->pshare->iot_mode_enable);
    }
#if defined(CONFIG_RTL_819XD)
    else if((priv->assoc_num == 1) && (AMPDU_ENABLE)) {
        if (pstat) {
            if ((pstat->current_tx_bytes > 14417920) && (pstat->current_rx_bytes > 14417920) && (priv->swq_en == 0))  { //55Mbps
                priv->swq_en = 1;
            }
            else if (((pstat->tx_avarage < 4587520) || (pstat->rx_avarage < 4587520)) && (priv->swq_en == 1)) { //35Mbps
                priv->swq_en = 0;
            }
        }
        else
            priv->swq_en = 0;
    }
#endif
#endif

#ifdef WIFI_WMM
#ifdef LOW_TP_TXOP
	if ((!priv->pmib->dot11OperationEntry.wifi_specific || (priv->pmib->dot11OperationEntry.wifi_specific == 2))
		&& QOS_ENABLE) {

		if (switch_turbo || priv->pshare->rf_ft_var.low_tp_txop) {
			unsigned int thd_tp;
			unsigned char under_thd;
			unsigned int curr_tp;

			if (priv->pmib->dot11BssType.net_work_type & (WIRELESS_11N | WIRELESS_11G))
			{
				// Determine the upper bound throughput threshold.
				if (priv->pmib->dot11BssType.net_work_type & WIRELESS_11N) {
					if (priv->assoc_num && priv->assoc_num != priv->pshare->ht_sta_num)
						thd_tp = priv->pshare->rf_ft_var.low_tp_txop_thd_g;
					else
						thd_tp = priv->pshare->rf_ft_var.low_tp_txop_thd_n;
				}
				else
					thd_tp = priv->pshare->rf_ft_var.low_tp_txop_thd_g;

				// Determine to close txop.
				curr_tp = (unsigned int)(priv->ext_stats.tx_avarage>>17) + (unsigned int)(priv->ext_stats.rx_avarage>>17);
				if (curr_tp <= thd_tp && curr_tp >= priv->pshare->rf_ft_var.low_tp_txop_thd_low)
					under_thd = 1;
				else
					under_thd = 0;
			}
			else
			{
				under_thd = 0;
			}

			if (switch_turbo) {
				priv->pshare->rf_ft_var.low_tp_txop_close = under_thd;
				priv->pshare->rf_ft_var.low_tp_txop_count = 0;
			} else if (priv->pshare->iot_mode_enable && (priv->pshare->rf_ft_var.low_tp_txop_close != under_thd)) {
				priv->pshare->rf_ft_var.low_tp_txop_count++;
				if (priv->pshare->rf_ft_var.low_tp_txop_close) {
					priv->pshare->rf_ft_var.low_tp_txop_count = priv->pshare->rf_ft_var.low_tp_txop_delay;
				}
				if (priv->pshare->rf_ft_var.low_tp_txop_count == priv->pshare->rf_ft_var.low_tp_txop_delay) {
					priv->pshare->rf_ft_var.low_tp_txop_count = 0;
					priv->pshare->rf_ft_var.low_tp_txop_close = under_thd;
					switch_turbo++;
				}
			} else {
				priv->pshare->rf_ft_var.low_tp_txop_count = 0;
			}
		}
	}
#endif

	if (switch_turbo)
		IOT_EDCA_switch(priv, priv->pmib->dot11BssType.net_work_type, priv->pshare->iot_mode_enable);
#endif
}


#ifdef WIFI_WMM
void IOT_EDCA_switch(struct rtl8192cd_priv *priv, int mode, char enable)
{
	unsigned int slot_time = 20, sifs_time = 10, BE_TXOP = 47, VI_TXOP = 94;
	unsigned int vi_cw_max = 4, vi_cw_min = 3, vi_aifs;

	if (!(!priv->pmib->dot11OperationEntry.wifi_specific ||
		((OPMODE & WIFI_AP_STATE) && (priv->pmib->dot11OperationEntry.wifi_specific == 2))
#ifdef CLIENT_MODE
        || ((OPMODE & WIFI_STATION_STATE) && (priv->pmib->dot11OperationEntry.wifi_specific == 2))
#endif
		))
		return;

	if ((mode & WIRELESS_11N) && (priv->pshare->ht_sta_num
#ifdef WDS
		|| ((OPMODE & WIFI_AP_STATE) && priv->pmib->dot11WdsInfo.wdsEnabled && priv->pmib->dot11WdsInfo.wdsNum)
#endif
		))
		sifs_time = 16;

	if ((mode & WIRELESS_11N) || (mode & WIRELESS_11G)) {
		slot_time = 9;
	} else {
		BE_TXOP = 94;
		VI_TXOP = 188;
	}

#if 0 //defined(CONFIG_RTL_8196D) || defined(CONFIG_RTL_8196E) || (defined(CONFIG_RTL_8197D) && !defined(CONFIG_PORT0_EXT_GIGA))
	if (priv->pshare->is_40m_bw)
	{
		BE_TXOP = 23;
	}
#endif

	if ((OPMODE & WIFI_AP_STATE) && priv->pmib->dot11OperationEntry.wifi_specific) {
		if (priv->pshare->iot_mode_VO_exist) {
#ifdef WMM_VIBE_PRI
			if (priv->pshare->iot_mode_BE_exist) {
				vi_cw_max = 5;
				vi_cw_min = 3;
				vi_aifs = (sifs_time + ((OPMODE & WIFI_AP_STATE)?1:2) * slot_time);
			} else 
#endif
			{
				vi_cw_max = 6;
				vi_cw_min = 4;
				vi_aifs = 0x2b;
			}
		} else {
			vi_aifs = (sifs_time + ((OPMODE & WIFI_AP_STATE)?1:2) * slot_time);
		}

		RTL_W32(EDCA_VI_PARA, ((VI_TXOP*(1-priv->pshare->iot_mode_VO_exist)) << 16)
			| (vi_cw_max << 12) | (vi_cw_min << 8) | vi_aifs);
	}

	if (!enable || (priv->pshare->rf_ft_var.wifi_beq_iot && priv->pshare->iot_mode_VI_exist)) {
		if (priv->pshare->rf_ft_var.wifi_beq_iot && priv->pshare->iot_mode_VI_exist) {
			RTL_W32(EDCA_BE_PARA, (10 << 12) | (4 << 8) | 0x4f);
		} else {
			RTL_W32(EDCA_BE_PARA, (((OPMODE & WIFI_AP_STATE)?6:10) << 12) | (4 << 8)
					| (sifs_time + 3 * slot_time));
		}
	} else {
#ifdef LOW_TP_TXOP
		int txop;
		unsigned int cw_max;
		unsigned int txop_close;
		
		cw_max = ((priv->pshare->BE_cwmax_enhance) ? 10 : 6);
		txop_close = ((priv->pshare->rf_ft_var.low_tp_txop && priv->pshare->rf_ft_var.low_tp_txop_close) ? 1 : 0);
		txop = (txop_close ? 0 : (BE_TXOP*2));
#endif
		if (priv->pshare->ht_sta_num
#ifdef WDS
			|| ((OPMODE & WIFI_AP_STATE) && (mode & WIRELESS_11N) &&
			priv->pmib->dot11WdsInfo.wdsEnabled && priv->pmib->dot11WdsInfo.wdsNum)
#endif
			) {
/*
			if (priv->pshare->txop_enlarge == 0xf) {
				// is 8192S client
				RTL_W32(EDCA_BE_PARA, ((BE_TXOP*2) << 16) |
							(6 << 12) | (4 << 8) | (sifs_time + slot_time+ 0xf)); // 0xf is 92s circuit delay
				priv->pshare->txop_enlarge = 2;
			}
			else
*/
			if (priv->pshare->txop_enlarge == 0xe) {
#ifndef LOW_TP_TXOP
				// is intel client, use a different edca value
#if 0 //defined(CONFIG_RTL_8196D) || defined(CONFIG_RTL_8196E) || (defined(CONFIG_RTL_8197D) && !defined(CONFIG_PORT0_EXT_GIGA))
				RTL_W32(EDCA_BE_PARA, (BE_TXOP*2 << 16) | (6 << 12) | (5 << 8) | 0x1f);
#else
				RTL_W32(EDCA_BE_PARA, (BE_TXOP*2 << 16) | (6 << 12) | (4 << 8) | 0x1f);
#endif
				priv->pshare->txop_enlarge = 2;
			} else if (priv->pshare->txop_enlarge == 0xd) {
				// is intel ralink, use a different edca value
				RTL_W32(EDCA_BE_PARA, (BE_TXOP*2 << 16) | (4 << 12) | (3 << 8) | 0x19);
				priv->pshare->txop_enlarge = 2;
			} else {
				if (get_rf_mimo_mode(priv) == MIMO_2T2R)
#if 0 //defined(CONFIG_RTL_8196D) || defined(CONFIG_RTL_8196E) || (defined(CONFIG_RTL_8197D) && !defined(CONFIG_PORT0_EXT_GIGA))
					RTL_W32(EDCA_BE_PARA, ((BE_TXOP*priv->pshare->txop_enlarge) << 16) |
                            (6 << 12) | (5 << 8) | (sifs_time + 3 * slot_time));
#else
					RTL_W32(EDCA_BE_PARA, ((BE_TXOP*priv->pshare->txop_enlarge) << 16) |
							(6 << 12) | (4 << 8) | (sifs_time + 3 * slot_time));
#endif
				else
#if 0 //defined(CONFIG_RTL_8196D) || defined(CONFIG_RTL_8196E) || (defined(CONFIG_RTL_8197D) && !defined(CONFIG_PORT0_EXT_GIGA))
					RTL_W32(EDCA_BE_PARA, ((BE_TXOP*priv->pshare->txop_enlarge) << 16) |
                            (5 << 12) | (4 << 8) | (sifs_time + 2 * slot_time));
#else
					RTL_W32(EDCA_BE_PARA, ((BE_TXOP*priv->pshare->txop_enlarge) << 16) |
							(5 << 12) | (3 << 8) | (sifs_time + 2 * slot_time));
#endif

#else
				// is intel client, use a different edca value
				RTL_W32(EDCA_BE_PARA, (txop << 16) | (cw_max << 12) | (4 << 8) | 0x1f);
			} else {
				txop = (txop_close ? 0: (BE_TXOP*priv->pshare->txop_enlarge));
			
				if (get_rf_mimo_mode(priv) == MIMO_2T2R)
					RTL_W32(EDCA_BE_PARA, (txop << 16) | (cw_max << 12) | (4 << 8) | (sifs_time + 3 * slot_time));
				else
					RTL_W32(EDCA_BE_PARA, (txop << 16) | (((priv->pshare->BE_cwmax_enhance) ? 10 : 5) << 12) |
						(3 << 8) | (sifs_time + 2 * slot_time));
#endif
			}
		} else {
#ifdef LOW_TP_TXOP
			RTL_W32(EDCA_BE_PARA, (txop << 16) | (cw_max << 12) | (4 << 8) | (sifs_time + 3 * slot_time));
#else
#if defined(CONFIG_RTL_8196D) || defined(CONFIG_RTL_8196E) || (defined(CONFIG_RTL_8197D) && !defined(CONFIG_PORT0_EXT_GIGA))
			RTL_W32(EDCA_BE_PARA, (BE_TXOP*2 << 16) | (6 << 12) | (5 << 8) | (sifs_time + 3 * slot_time));
#else
			RTL_W32(EDCA_BE_PARA, (BE_TXOP*2 << 16) | (6 << 12) | (4 << 8) | (sifs_time + 3 * slot_time));
#endif
#endif
		}
/*
		if (priv->pmib->dot11OperationEntry.wifi_specific == 2) {
			RTL_W16(NAV_PROT_LEN, 0x01C0);
			RTL_W8(CFEND_TH, 0xFF);
			set_fw_reg(priv, 0xfd000ab0, 0, 0);
		}
*/
	}
}

void check_NAV_prot_len(struct rtl8192cd_priv *priv, struct stat_info *pstat, unsigned int disassoc)
{
	if ((priv->pmib->dot11BssType.net_work_type & WIRELESS_11N) && pstat
		&& pstat->ht_cap_len && pstat->is_intel_sta) {
		if (!disassoc && (pstat->MIMO_ps & _HT_MIMO_PS_DYNAMIC_)) {
#ifdef STA_EXT
			if (pstat->aid <= FW_NUM_STAT)
				priv->pshare->mimo_ps_dynamic_sta |= BIT(pstat->aid - 1);
			else
				priv->pshare->mimo_ps_dynamic_sta_ext |= BIT(pstat->aid - 1 - FW_NUM_STAT);
#else
#ifdef CONFIG_RTL_88E_SUPPORT
			if (GET_CHIP_VER(priv) == VERSION_8188E) {
				if (pstat->aid <= 32)
					priv->pshare->mimo_ps_dynamic_sta |= BIT(pstat->aid - 1);
				else
					priv->pshare->mimo_ps_dynamic_sta_88e_hw_ext |= BIT(pstat->aid - 1 - 32);
			} else
#endif
			{
				priv->pshare->mimo_ps_dynamic_sta |= BIT(pstat->aid -1);
			}
#endif
		} else {
#ifdef STA_EXT
			if (pstat->aid <= FW_NUM_STAT)
				priv->pshare->mimo_ps_dynamic_sta &= ~BIT(pstat->aid - 1);
			else
				priv->pshare->mimo_ps_dynamic_sta_ext &= ~BIT(pstat->aid - 1 - FW_NUM_STAT);
#else
#ifdef CONFIG_RTL_88E_SUPPORT
			if (GET_CHIP_VER(priv) == VERSION_8188E) {
				if (pstat->aid <= 32)
					priv->pshare->mimo_ps_dynamic_sta &= ~BIT(pstat->aid - 1);
				else
					priv->pshare->mimo_ps_dynamic_sta_88e_hw_ext &= ~BIT(pstat->aid - 1 - 32);
			} else
#endif
			{
				priv->pshare->mimo_ps_dynamic_sta &= ~BIT(pstat->aid -1);
			}
#endif
		}

#ifdef CONFIG_RTL_88E_SUPPORT
		if (GET_CHIP_VER(priv) != VERSION_8188E)
#endif
		{
			if (priv->pshare->mimo_ps_dynamic_sta
#ifdef STA_EXT
				|| priv->pshare->mimo_ps_dynamic_sta_ext
#endif
				) {
				RTL_W8(NAV_PROT_LEN, 0x40);
			} else {
				RTL_W8(NAV_PROT_LEN, 0x20);
			}
		}
	}
}
#endif


//3 ============================================================
//3 FA statistic functions
//3 ============================================================

#if !defined(CONFIG_RTL_NEW_AUTOCH)
static
#endif
void reset_FA_reg(struct rtl8192cd_priv *priv)
{
#if !defined(CONFIG_RTL_NEW_AUTOCH)
	unsigned char value8;

	value8 = RTL_R8(0xd03);
	RTL_W8(0xd03, value8 | 0x08);	// regD00[27]=1 to reset these OFDM FA counters
	value8 = RTL_R8(0xd03);
	RTL_W8(0xd03, value8 & 0xF7);	// regD00[27]=0 to start counting
	value8 = RTL_R8(0xa2d);
	RTL_W8(0xa2d, value8 & 0x3F);	// regA2D[7:6]=00 to disable counting
	value8 = RTL_R8(0xa2d);
	RTL_W8(0xa2d, value8 | 0x80);	// regA2D[7:6]=10 to enable counting

#ifdef INTERFERENCE_CONTROL
	// do BB reset to clear Reg0xCF0 & Reg0xCF2
	RTL_W8(TXPAUSE, 0xff);
	value8 = RTL_R8(SYS_FUNC_EN);
	RTL_W8(SYS_FUNC_EN, value8 & ~FEN_BBRSTB);
	RTL_W8(SYS_FUNC_EN, value8 | FEN_BBRSTB);
	RTL_W8(TXPAUSE, 0x00);
#endif
#else
	unsigned char value8;

	/* cck CCA */
	PHY_SetBBReg(priv, 0xa2c, BIT(13) | BIT(12), 0);
	PHY_SetBBReg(priv, 0xa2c, BIT(13) | BIT(12), 2);
	/* cck FA*/
	PHY_SetBBReg(priv, 0xa2c, BIT(15) | BIT(14), 0);
	PHY_SetBBReg(priv, 0xa2c, BIT(15) | BIT(14), 2);
	/* ofdm */
	PHY_SetBBReg(priv, 0xd00, BIT(27), 1);
	PHY_SetBBReg(priv, 0xd00, BIT(27), 0);

#ifdef INTERFERENCE_CONTROL
	// do BB reset to clear Reg0xCF0 & Reg0xCF2
	RTL_W8(TXPAUSE, 0xff);
	value8 = RTL_R8(SYS_FUNC_EN);
	RTL_W8(SYS_FUNC_EN, value8 & ~FEN_BBRSTB);
	RTL_W8(SYS_FUNC_EN, value8 | FEN_BBRSTB);
	RTL_W8(TXPAUSE, 0x00);
#endif
#endif

#if defined(CONFIG_RTL_92D_SUPPORT) && defined(CONFIG_RTL_NOISE_CONTROL)
	if (GET_CHIP_VER(priv) == VERSION_8192D){
		PHY_SetBBReg(priv, 0xf14, BIT(16),1);
		PHY_SetBBReg(priv, 0xf14, BIT(16),0);
		RTL_W32(RXERR_RPT, RTL_R32(RXERR_RPT)|BIT(27));
		RTL_W32(RXERR_RPT, RTL_R32(RXERR_RPT)&(~BIT(27)));
	}
#endif

#ifdef CONFIG_RTL_88E_SUPPORT
	if (GET_CHIP_VER(priv)==VERSION_8188E) {
		PHY_SetBBReg(priv, 0xc0c, BIT(31), 1);
		PHY_SetBBReg(priv, 0xc0c, BIT(31), 0);
	}
#endif
}

#if defined(CONFIG_RTL_NEW_AUTOCH)
void hold_CCA_FA_counter(struct rtl8192cd_priv *priv)
{
	/* hold cck CCA & FA counter */
	PHY_SetBBReg(priv, 0xa2c, BIT(12), 1);
	PHY_SetBBReg(priv, 0xa2c, BIT(14), 1);

	/* hold ofdm CCA & FA counter */
	PHY_SetBBReg(priv, 0xc00, BIT(31), 1);
	PHY_SetBBReg(priv, 0xd00, BIT(31), 1);
}

void release_CCA_FA_counter(struct rtl8192cd_priv *priv)
{
	/* release cck CCA & FA counter */
	PHY_SetBBReg(priv, 0xa2c, BIT(12), 0);
	PHY_SetBBReg(priv, 0xa2c, BIT(14), 0);

	/* release ofdm CCA & FA counter */
	PHY_SetBBReg(priv, 0xc00, BIT(31), 0);
	PHY_SetBBReg(priv, 0xd00, BIT(31), 0);

#ifdef CONFIG_RTL_88E_SUPPORT
	if (GET_CHIP_VER(priv)==VERSION_8188E) {
		PHY_SetBBReg(priv, 0xc0c, BIT(31), 1);
		PHY_SetBBReg(priv, 0xc0c, BIT(31), 0);
	}
#endif
}


void _FA_statistic(struct rtl8192cd_priv *priv)
{
	// read OFDM FA counters
	priv->pshare->ofdm_FA_cnt1 = RTL_R16(0xda2);
	priv->pshare->ofdm_FA_cnt2 = RTL_R16(0xda4);
	priv->pshare->ofdm_FA_cnt3 = RTL_R16(0xda6);
	priv->pshare->ofdm_FA_cnt4 = RTL_R16(0xda8);

	priv->pshare->cck_FA_cnt = (RTL_R8(0xa5b) << 8) + RTL_R8(0xa5c);

#ifdef INTERFERENCE_CONTROL
	priv->pshare->ofdm_FA_total_cnt = (unsigned int) priv->pshare->ofdm_FA_cnt1 +
			priv->pshare->ofdm_FA_cnt2 + priv->pshare->ofdm_FA_cnt3 +
			priv->pshare->ofdm_FA_cnt4 + RTL_R16(0xcf0) + RTL_R16(0xcf2);
	
	priv->pshare->FA_total_cnt = priv->pshare->ofdm_FA_total_cnt + priv->pshare->cck_FA_cnt;
#else
	priv->pshare->FA_total_cnt = priv->pshare->ofdm_FA_cnt1 + priv->pshare->ofdm_FA_cnt2 +
	                             priv->pshare->ofdm_FA_cnt3 + priv->pshare->ofdm_FA_cnt4 +
	                             priv->pshare->cck_FA_cnt + RTL_R16(0xcf0) + RTL_R16(0xcf2);
#endif
}
#endif

void FA_statistic(struct rtl8192cd_priv *priv)
{

#if defined(CONFIG_RTL_92D_SUPPORT) && defined(CONFIG_RTL_NOISE_CONTROL)
	if (GET_CHIP_VER(priv) == VERSION_8192D){
//		priv->pshare->F90_cnt = PHY_QueryBBReg(priv, 0xf90, bMaskHWord);
		priv->pshare->F94_cnt = PHY_QueryBBReg(priv, 0xf94, bMaskHWord);
		priv->pshare->F94_cntOK = PHY_QueryBBReg(priv, 0xf94, bMaskLWord);
		RTL_W32(RXERR_RPT,(RTL_R32(RXERR_RPT)&0x0fffffff)|0x70000000);
		priv->pshare->Reg664_cnt = RTL_R32(RXERR_RPT) & 0xfffff;
		RTL_W32(RXERR_RPT,(RTL_R32(RXERR_RPT)&0x0fffffff)|0x60000000);
		priv->pshare->Reg664_cntOK = RTL_R32(RXERR_RPT) & 0xfffff;
	}
#endif

#if !defined(CONFIG_RTL_NEW_AUTOCH)
	signed char value8;

	// read OFDM FA counters
	priv->pshare->ofdm_FA_cnt1 = RTL_R16(0xda2);
	priv->pshare->ofdm_FA_cnt2 = RTL_R16(0xda4);
	priv->pshare->ofdm_FA_cnt3 = RTL_R16(0xda6);
	priv->pshare->ofdm_FA_cnt4 = RTL_R16(0xda8);

	// read the CCK FA counters
	value8 = RTL_R8(0xa2d);
	RTL_W8(0xa2d, value8 | 0x40);	// regA2D[6]=1 to hold and read the CCK FA counters
	priv->pshare->cck_FA_cnt = RTL_R8(0xa5b);
	priv->pshare->cck_FA_cnt = priv->pshare->cck_FA_cnt << 8;
	priv->pshare->cck_FA_cnt += RTL_R8(0xa5c);

	priv->pshare->FA_total_cnt = priv->pshare->ofdm_FA_cnt1 + priv->pshare->ofdm_FA_cnt2 +
	                             priv->pshare->ofdm_FA_cnt3 + priv->pshare->ofdm_FA_cnt4 +
	                             priv->pshare->cck_FA_cnt + RTL_R16(0xcf0) + RTL_R16(0xcf2);

	if (priv->pshare->rf_ft_var.rssi_dump)
		priv->pshare->CCA_total_cnt = ((RTL_R8(0xa60)<<8)|RTL_R8(0xa61)) + RTL_R16(0xda0);
#else
	hold_CCA_FA_counter(priv);
	_FA_statistic(priv);

	if (priv->pshare->rf_ft_var.rssi_dump)
		priv->pshare->CCA_total_cnt = ((RTL_R8(0xa60)<<8)|RTL_R8(0xa61)) + RTL_R16(0xda0);

	release_CCA_FA_counter(priv);
#endif

	reset_FA_reg(priv);

#if defined(CONFIG_RTL_92D_SUPPORT) && defined(CONFIG_RTL_NOISE_CONTROL)
	if (GET_CHIP_VER(priv) == VERSION_8192D){
		if (priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_5G  && !(OPMODE & WIFI_SITE_MONITOR)) {
			if (priv->pshare->DNC_on == 0){
				//if ((priv->pshare->F94_cnt + priv->pshare->F90_cnt)> 3000){
				/* Reg 664: x > y && x > 1000
				    Reg F94: x > 0.75*y && x > 1000 */
				if (((priv->pshare->Reg664_cnt>priv->pshare->Reg664_cntOK) && (priv->pshare->Reg664_cnt > 1000))||
					((priv->pshare->F94_cnt > ((priv->pshare->Reg664_cntOK*3)>>2)) && (priv->pshare->F94_cnt > 1000))) {
					priv->ext_stats.tp_average_pre = (priv->ext_stats.tx_avarage+priv->ext_stats.rx_avarage)>>17;
					priv->pshare->DNC_on = 1;
					priv->pshare->DNC_chk_cnt = 1;
					priv->pshare->DNC_chk = 2; // 0: don't check, 1; check, 2: just entering DNC
					//PHY_SetBBReg(priv, 0xb30, bMaskDWord, 0x00a00000);
					PHY_SetBBReg(priv, 0x870, bMaskDWord, 0x07600760);
					PHY_SetBBReg(priv, 0xc50, bMaskByte0, 0x20);
					PHY_SetBBReg(priv, 0xc58, bMaskByte0, 0x20);
					//printk("Dynamic Noise Control ON\n");
				}
			} else {
				if ((priv->pshare->DNC_chk_cnt % 5)==0){ // check every 5*2=10 seconds
					unsigned long tp_now = (priv->ext_stats.tx_avarage+priv->ext_stats.rx_avarage)>>17;
					priv->pshare->DNC_chk_cnt = 0;

 					if ((priv->pshare->DNC_chk == 2) && (tp_now < priv->ext_stats.tp_average_pre+5)){
						//no advantage, leave DNC state
						priv->pshare->DNC_on = 0;
						priv->pshare->DNC_chk = 0;
						//PHY_SetBBReg(priv, 0xb30, bMaskDWord, 0);
						PHY_SetBBReg(priv, 0x870, bMaskDWord, 0x07000700);
					}
					else
					{
						priv->pshare->DNC_chk = 0;

						/* If TP < 20M or TP varies more than 5M. Start Checking...*/
						if ((tp_now < 20) || ((tp_now < (priv->ext_stats.tp_average_pre-5))|| (tp_now > (priv->ext_stats.tp_average_pre+5)))){
							priv->pshare->DNC_chk = 1;
							//PHY_SetBBReg(priv, 0xb30, bMaskDWord, 0);
							PHY_SetBBReg(priv, 0x870, bMaskDWord, 0x07000700);
							if (!timer_pending(&priv->dnc_timer)) {
								//printk("... Start Check Noise ...\n");
								mod_timer(&priv->dnc_timer, jiffies + 10);	// 100 ms
							}
						}
					}

					priv->ext_stats.tp_average_pre = tp_now;

				} else if ((priv->pshare->DNC_chk_cnt % 5)==1 && priv->pshare->DNC_chk == 1) {
					priv->pshare->DNC_chk = 0;
					//if ((priv->pshare->F94_cnt + priv->pshare->F90_cnt) < 120) {
					if ((priv->pshare->F94_cnt + priv->pshare->Reg664_cnt) < 120) {
						priv->pshare->DNC_on = 0;
						//PHY_SetBBReg(priv, 0xb30, bMaskDWord, 0);
						PHY_SetBBReg(priv, 0x870, bMaskDWord, 0x07000700);
						//printk("Dynamic Noise Control OFF\n");
					}
				}
				priv->pshare->DNC_chk_cnt++;
			}
		}
	}
#endif
}


//3 ============================================================
//3 Rate Adaptive
//3 ============================================================

void check_RA_by_rssi(struct rtl8192cd_priv *priv, struct stat_info *pstat)
{
	int level = 0;

	switch (pstat->rssi_level) {
		case 1:
			if (pstat->rssi >= priv->pshare->rf_ft_var.raGoDownUpper)
				level = 1;
			else if ((pstat->rssi >= priv->pshare->rf_ft_var.raGoDown20MLower) ||
				((priv->pshare->is_40m_bw) && (pstat->ht_cap_len) &&
				(pstat->rssi >= priv->pshare->rf_ft_var.raGoDown40MLower) &&
				(pstat->ht_cap_buf.ht_cap_info & cpu_to_le16(_HTCAP_SUPPORT_CH_WDTH_))))
				level = 2;
			else
				level = 3;
			break;
		case 2:
			if (pstat->rssi > priv->pshare->rf_ft_var.raGoUpUpper)
				level = 1;
			else if ((pstat->rssi < priv->pshare->rf_ft_var.raGoDown40MLower) ||
				((!pstat->ht_cap_len || !priv->pshare->is_40m_bw ||
				!(pstat->ht_cap_buf.ht_cap_info & cpu_to_le16(_HTCAP_SUPPORT_CH_WDTH_))) &&
				(pstat->rssi < priv->pshare->rf_ft_var.raGoDown20MLower)))
				level = 3;
			else
				level = 2;
			break;
		case 3:
			if (pstat->rssi > priv->pshare->rf_ft_var.raGoUpUpper)
				level = 1;
			else if ((pstat->rssi > priv->pshare->rf_ft_var.raGoUp20MLower) ||
				((priv->pshare->is_40m_bw) && (pstat->ht_cap_len) &&
				(pstat->rssi > priv->pshare->rf_ft_var.raGoUp40MLower) &&
				(pstat->ht_cap_buf.ht_cap_info & cpu_to_le16(_HTCAP_SUPPORT_CH_WDTH_))))
				level = 2;
			else
				level = 3;
			break;
		default:
			if (isErpSta(pstat))
				DEBUG_ERR("wrong rssi level setting\n");
			break;
	}

	if (level != pstat->rssi_level) {
		pstat->rssi_level = level;
#ifdef CONFIG_RTL_88E_SUPPORT
		if (GET_CHIP_VER(priv)==VERSION_8188E) {
#ifdef TXREPORT
			add_RATid(priv, pstat);
#endif
		} else
#endif
		{
			add_update_RATid(priv, pstat);
		}
	}
}


void check_txrate_by_reg(struct rtl8192cd_priv *priv, struct stat_info *pstat)
{
	unsigned char initial_rate = 0x7f;
	unsigned char legacyRA =0 ;

	if( should_restrict_Nrate(priv, pstat) && is_fixedMCSTxRate(priv))
		legacyRA = 1;
	
#ifdef STA_EXT
	if (pstat->remapped_aid && (pstat->remapped_aid < FW_NUM_STAT-1))
#else
	if (pstat->aid && (pstat->aid < 32))
#endif
	{
#ifdef WDS
		if (((pstat->state & WIFI_WDS) && (priv->pmib->dot11WdsInfo.entry[pstat->wds_idx].txRate == 0)) ||
			(!(pstat->state & WIFI_WDS) && (priv->pmib->dot11StationConfigEntry.autoRate)) || legacyRA)
#else
		if (priv->pmib->dot11StationConfigEntry.autoRate || legacyRA)
#endif
		{
			initial_rate = RTL_R8(INIDATA_RATE_SEL + REMAP_AID(pstat)) & 0x7f;
			if (initial_rate == 0x7f)
				return;

			if ((initial_rate&0x3f) < 12) {
				pstat->current_tx_rate = dot11_rate_table[initial_rate&0x3f];
				pstat->ht_current_tx_info &= ~TX_USE_SHORT_GI;				
			} else {
				pstat->current_tx_rate = 0x80|((initial_rate&0x3f) -12);
				if (initial_rate & BIT(6))
					pstat->ht_current_tx_info |= TX_USE_SHORT_GI;
				else
					pstat->ht_current_tx_info &= ~TX_USE_SHORT_GI;
			}

			priv->pshare->current_tx_rate    = pstat->current_tx_rate;
			priv->pshare->ht_current_tx_info = pstat->ht_current_tx_info;
		} else if (pstat->ht_cap_len) {
			unsigned int is_sgi = 0;

			if (priv->pshare->is_40m_bw && (pstat->tx_bw == HT_CHANNEL_WIDTH_20_40)
#ifdef WIFI_11N_2040_COEXIST
				&& !((OPMODE & WIFI_AP_STATE) && priv->pmib->dot11nConfigEntry.dot11nCoexist &&
				(priv->bg_ap_timeout || priv->force_20_sta || priv->switch_20_sta
#ifdef CONFIG_RTL_88E_SUPPORT
				|| (GET_CHIP_VER(priv) == VERSION_8188E)?(priv->force_20_sta_88e_hw_ext 
				|| priv->switch_20_sta_88e_hw_ext):(0)
#endif
#ifdef STA_EXT
				|| priv->force_20_sta_ext || priv->switch_20_sta_ext
#endif
				))
#endif
			) {
				if (priv->pmib->dot11nConfigEntry.dot11nShortGIfor40M
					&& (pstat->ht_cap_buf.ht_cap_info & cpu_to_le16(_HTCAP_SHORTGI_40M_)))
					is_sgi++;
			} else if (priv->pmib->dot11nConfigEntry.dot11nShortGIfor20M
				&& (pstat->ht_cap_buf.ht_cap_info & cpu_to_le16(_HTCAP_SHORTGI_20M_))) {
				is_sgi++;
			}

			if (is_sgi)
				pstat->ht_current_tx_info |= TX_USE_SHORT_GI;
			else
				pstat->ht_current_tx_info &= ~TX_USE_SHORT_GI;
		}

		if (pstat->ht_cap_len) {
			if (priv->pshare->is_40m_bw && (pstat->tx_bw == HT_CHANNEL_WIDTH_20_40))
				pstat->ht_current_tx_info |= TX_USE_40M_MODE;
			else
				pstat->ht_current_tx_info &= ~TX_USE_40M_MODE;
		}

		priv->pshare->ht_current_tx_info = pstat->ht_current_tx_info;
	} else {
		DEBUG_INFO("sta has no aid found to check current tx rate\n");
	}
}

void add_RATid(struct rtl8192cd_priv *priv, struct stat_info *pstat)
{
	unsigned char limit=16;
	int i;
	unsigned long flags;
	unsigned int update_reg=0;

	SAVE_INT_AND_CLI(flags);

	pstat->tx_ra_bitmap = 0;

	for (i=0; i<32; i++) {
		if (pstat->bssrateset[i])
			pstat->tx_ra_bitmap |= get_bit_value_from_ieee_value(pstat->bssrateset[i]&0x7f);
	}

	if (pstat->ht_cap_len) {
		if ((pstat->MIMO_ps & _HT_MIMO_PS_STATIC_) ||
			(get_rf_mimo_mode(priv)== MIMO_1T2R) ||
			(get_rf_mimo_mode(priv)== MIMO_1T1R))
			limit=8;

		for (i=0; i<limit; i++) {
			if (pstat->ht_cap_buf.support_mcs[i/8] & BIT(i%8))
				pstat->tx_ra_bitmap |= BIT(i+12);
		}
	}

	if (pstat->ht_cap_len) {
		unsigned int set_sgi = 0;
		if (priv->pshare->is_40m_bw && (pstat->tx_bw == HT_CHANNEL_WIDTH_20_40)
#ifdef WIFI_11N_2040_COEXIST
			&& !((OPMODE & WIFI_AP_STATE) && priv->pmib->dot11nConfigEntry.dot11nCoexist &&
			(priv->bg_ap_timeout || priv->force_20_sta || priv->switch_20_sta
#ifdef CONFIG_RTL_88E_SUPPORT
				|| (GET_CHIP_VER(priv) == VERSION_8188E)?(priv->force_20_sta_88e_hw_ext 
				|| priv->switch_20_sta_88e_hw_ext):(0)
#endif
#ifdef STA_EXT
			|| priv->force_20_sta_ext || priv->switch_20_sta_ext
#endif
			))
#endif
			) {
			if (pstat->ht_cap_buf.ht_cap_info & cpu_to_le16(_HTCAP_SHORTGI_40M_)
				&& priv->pmib->dot11nConfigEntry.dot11nShortGIfor40M)
				set_sgi++;
		} else if (pstat->ht_cap_buf.ht_cap_info & cpu_to_le16(_HTCAP_SHORTGI_20M_) &&
			priv->pmib->dot11nConfigEntry.dot11nShortGIfor20M) {
			set_sgi++;
		}

		if (set_sgi) {
#if defined(CONFIG_RTL_88E_SUPPORT) && defined(TXREPORT)
			if (GET_CHIP_VER(priv)==VERSION_8188E)
				priv->pshare->RaInfo[pstat->aid].SGIEnable = 1;
			else
#endif
				pstat->tx_ra_bitmap |= BIT(28);
		}
#if defined(CONFIG_RTL_88E_SUPPORT) && defined(TXREPORT)
		else {
			if (GET_CHIP_VER(priv)==VERSION_8188E)
				priv->pshare->RaInfo[pstat->aid].SGIEnable = 0;
		}
#endif
	}
#if defined(CONFIG_RTL_88E_SUPPORT) && defined(TXREPORT)
	else {
		if (GET_CHIP_VER(priv)==VERSION_8188E)
			priv->pshare->RaInfo[pstat->aid].SGIEnable = 0;
	}
#endif

	if ((pstat->rssi_level < 1) || (pstat->rssi_level > 3)) {
		if (pstat->rssi >= priv->pshare->rf_ft_var.raGoDownUpper)
			pstat->rssi_level = 1;
		else if ((pstat->rssi >= priv->pshare->rf_ft_var.raGoDown20MLower) ||
			((priv->pshare->is_40m_bw) && (pstat->ht_cap_len) &&
			(pstat->rssi >= priv->pshare->rf_ft_var.raGoDown40MLower) &&
			(pstat->ht_cap_buf.ht_cap_info & cpu_to_le16(_HTCAP_SUPPORT_CH_WDTH_))))
			pstat->rssi_level = 2;
		else
			pstat->rssi_level = 3;
	}

	if ((priv->pmib->dot11BssType.net_work_type & WIRELESS_11A) &&
		((OPMODE & WIFI_AP_STATE) || (priv->pmib->dot11RFEntry.phyBandSelect & PHY_BAND_5G)))
		pstat->tx_ra_bitmap &= 0xfffffff0; //disable cck rate

#ifdef P2P_SUPPORT
	if(pstat->is_p2p_client){ 
		pstat->tx_ra_bitmap &= 0xfffffff0; //disable cck rate
	}
#endif

	// rate adaptive by rssi
	if ((priv->pmib->dot11BssType.net_work_type & WIRELESS_11N) && pstat->ht_cap_len && (!should_restrict_Nrate(priv, pstat))) {
		if ((get_rf_mimo_mode(priv) == MIMO_1T2R) || (get_rf_mimo_mode(priv) == MIMO_1T1R)) {
			switch (pstat->rssi_level) {
				case 1:
					pstat->tx_ra_bitmap &= 0x100f0000;
					break;
				case 2:
					pstat->tx_ra_bitmap &= 0x100ff000;
					break;
				case 3:
					if (priv->pshare->is_40m_bw)
						pstat->tx_ra_bitmap &= 0x100ff005;
					else
						pstat->tx_ra_bitmap &= 0x100ff001;
					break;
			}
		} else {
			switch (pstat->rssi_level) {
				case 1:
					pstat->tx_ra_bitmap &= 0x1f8f0000;
					break;
				case 2:
					pstat->tx_ra_bitmap &= 0x1f8ff000;
					break;
				case 3:
					if (priv->pshare->is_40m_bw)
						pstat->tx_ra_bitmap &= 0x010ff005;
					else
						pstat->tx_ra_bitmap &= 0x010ff001;
					break;
			}

			// Don't need to mask high rates due to new rate adaptive parameters
			//if (pstat->is_broadcom_sta)		// use MCS12 as the highest rate vs. Broadcom sta
			//	pstat->tx_ra_bitmap &= 0x81ffffff;

			// NIC driver will report not supporting MCS15 and MCS14 in asoc req
			//if (pstat->is_rtl8190_sta && !pstat->is_2t_mimo_sta)
			//	pstat->tx_ra_bitmap &= 0x83ffffff;		// if Realtek 1x2 sta, don't use MCS15 and MCS14
		}
	}
	else if (((priv->pmib->dot11BssType.net_work_type & WIRELESS_11G) && isErpSta(pstat)) ||
			((priv->pmib->dot11BssType.net_work_type & WIRELESS_11A) &&
			((OPMODE & WIFI_AP_STATE) || (priv->pmib->dot11RFEntry.phyBandSelect & PHY_BAND_5G))))
	{
		switch (pstat->rssi_level) {
			case 1:
				pstat->tx_ra_bitmap &= 0x00000f00;
				break;
			case 2:
				pstat->tx_ra_bitmap &= 0x00000ff0;
				break;
			case 3:
				pstat->tx_ra_bitmap &= 0x00000ff5;
				break;
		}
	} else {
		pstat->tx_ra_bitmap &= 0x0000000d;
	}

// Client mode IOT issue, Button 2009.07.17
#ifdef CLIENT_MODE
	if(OPMODE & WIFI_STATION_STATE) {
		if(!pstat->is_rtl8192s_sta && pstat->is_realtek_sta && pstat->is_legacy_encrpt)
			pstat->tx_ra_bitmap &= 0x0001ffff;					// up to MCS4
	}
#endif
#if defined(CONFIG_RTL_92D_SUPPORT) && defined (USB_POWER_SUPPORT)
	if ((GET_CHIP_VER(priv)==VERSION_8192D) &&	(priv->pmib->dot11RFEntry.phyBandSelect & PHY_BAND_5G))
		pstat->tx_ra_bitmap &= USB_RA_MASK;
#endif

#ifdef STA_EXT
	//update STA_map
	{
		int remapped_aid = 0;
		remapped_aid = find_reampped_aid(priv, pstat->aid);
		if(remapped_aid == 0) {
			/*WARNING:  THIS SHOULD NOT HAPPEN*/
			printk("add AID fail!!\n");
			BUG();
		}
		if(remapped_aid >= (FW_NUM_STAT-1)){// no room for the STA
//			priv->STA_map |= (1<< pstat->aid) ;
			pstat->remapped_aid = FW_NUM_STAT-1;
			pstat->sta_in_firmware = 0; // this value will updated in expire_timer
		} else if(priv->pshare->remapped_aidarray[remapped_aid]  == 0) { // if not 0, it should have been added before
			//we got a room
			//clear STA_map
//			priv->STA_map &= ~(BIT(pstat->aid));
			pstat->remapped_aid = remapped_aid;
			priv->pshare->remapped_aidarray[remapped_aid] = pstat->aid;
			pstat->sta_in_firmware = 1; // this value will updated in expire_timer
			priv->pshare->fw_free_space --;
		} else {// added before
			pstat->sta_in_firmware = 1;
		}
	}
#endif// STA_EXT

#if defined(CONFIG_RTL_88E_SUPPORT) && defined(TXREPORT)
	if (GET_CHIP_VER(priv)==VERSION_8188E) {
		if (pstat->tx_ra_bitmap & 0xff000) {
			if (priv->pshare->is_40m_bw)
				priv->pshare->RaInfo[pstat->aid].RateID = ARFR_1T_40M;
			else
				priv->pshare->RaInfo[pstat->aid].RateID = ARFR_1T_20M;
		} else if (pstat->tx_ra_bitmap & 0xff0) {
			priv->pshare->RaInfo[pstat->aid].RateID = ARFR_BG_MIX;
		} else {
			priv->pshare->RaInfo[pstat->aid].RateID = ARFR_B_ONLY;
		}

		priv->pshare->RaInfo[pstat->aid].RateMask = pstat->tx_ra_bitmap;
		ARFBRefresh(priv, &priv->pshare->RaInfo[pstat->aid]);
	} else
#endif
	{
#ifdef STA_EXT
		if (REMAP_AID(pstat) < FW_NUM_STAT-1)
#else
		if (REMAP_AID(pstat) < 32)
#endif
		{
#ifdef CONFIG_RTL_92D_SUPPORT
			if (priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_5G) 
			{
				pstat->tx_ra_bitmap &= 0xfffffff0;
				if (pstat->tx_ra_bitmap & 0xff00000) {
					if (priv->pshare->is_40m_bw)
						set_RATid_cmd(priv, REMAP_AID(pstat), ARFR_2T_Band_A_40M, pstat->tx_ra_bitmap);
					else
						set_RATid_cmd(priv, REMAP_AID(pstat), ARFR_2T_Band_A_20M, pstat->tx_ra_bitmap);
					update_reg++;
				} else if (pstat->tx_ra_bitmap & 0xff000) {
					if (priv->pshare->is_40m_bw)
						set_RATid_cmd(priv, REMAP_AID(pstat), ARFR_2T_Band_A_40M, pstat->tx_ra_bitmap);
					else
						set_RATid_cmd(priv, REMAP_AID(pstat), ARFR_2T_Band_A_20M, pstat->tx_ra_bitmap);
				} else if (pstat->tx_ra_bitmap & 0xff0) {
					set_RATid_cmd(priv, REMAP_AID(pstat), ARFR_Band_A_BMC, pstat->tx_ra_bitmap);
				} else {
					set_RATid_cmd(priv, REMAP_AID(pstat), ARFR_Band_A_BMC, pstat->tx_ra_bitmap);
				}
			} else 
#endif
			{
				if (pstat->tx_ra_bitmap & 0xff00000) {
					if (priv->pshare->is_40m_bw)
						set_RATid_cmd(priv, REMAP_AID(pstat), ARFR_2T_40M, pstat->tx_ra_bitmap);
					else
						set_RATid_cmd(priv, REMAP_AID(pstat), ARFR_2T_20M, pstat->tx_ra_bitmap);
					update_reg++;
				} else if (pstat->tx_ra_bitmap & 0xff000) {
					if (priv->pshare->is_40m_bw)
						set_RATid_cmd(priv, REMAP_AID(pstat), ARFR_1T_40M, pstat->tx_ra_bitmap);
					else
						set_RATid_cmd(priv, REMAP_AID(pstat), ARFR_1T_20M, pstat->tx_ra_bitmap);
				} else if (pstat->tx_ra_bitmap & 0xff0) {
					set_RATid_cmd(priv, REMAP_AID(pstat), ARFR_BG_MIX, pstat->tx_ra_bitmap);
				} else {
					set_RATid_cmd(priv, REMAP_AID(pstat), ARFR_B_ONLY, pstat->tx_ra_bitmap);
				}
			}

			/*
			 * Rate adaptive algorithm.
			 * If the STA is 2R, we set the inti rate to MCS 15
			 */
			if (update_reg) {
				if (!pstat->check_init_tx_rate && (pstat->rssi > 55)) {
					RTL_W8(INIDATA_RATE_SEL + REMAP_AID(pstat), 0x1b);
					pstat->check_init_tx_rate = 1;
				}
			}
			DEBUG_INFO("Add id %d val %08x to ratr\n", pstat->aid, pstat->tx_ra_bitmap);
		} else {
#ifdef STA_EXT
#ifdef CONFIG_RTL_92D_SUPPORT
			if (priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_5G) {
				if (priv->pshare->is_40m_bw)
					set_RATid_cmd(priv, (FW_NUM_STAT-1), ARFR_2T_Band_A_40M, 0x1ffffff0);
				else
					set_RATid_cmd(priv, (FW_NUM_STAT-1), ARFR_2T_Band_A_20M, 0x1ffffff0);
			} else
#endif
			{
				if (priv->pshare->is_40m_bw)
					set_RATid_cmd(priv, (FW_NUM_STAT-1), ARFR_2T_40M, 0x1fffffff);
				else
					set_RATid_cmd(priv, (FW_NUM_STAT-1), ARFR_2T_20M, 0x1fffffff);
			}
#else
			DEBUG_ERR("station aid %d exceed the max number\n", pstat->aid);
#endif
		}
	}

	RESTORE_INT(flags);
}

void set_rssi_cmd(struct rtl8192cd_priv *priv, struct stat_info *pstat)
{
	unsigned long flags;
	unsigned int content = 0;

	int rssi = pstat->rssi;

#ifdef HIGH_POWER_EXT_PA
	if( priv->pshare->rf_ft_var.use_ext_pa ) 
		rssi += RSSI_DIFF_PA;
	if( rssi > 100)
		rssi = 100;
#endif

	
	SAVE_INT_AND_CLI(flags);

	/*
	 * set rssi
	 */
	 content = rssi<< 24;

#ifdef CONFIG_RTL_92D_SUPPORT
	/*
	 * set max macid
	 */
	if (GET_CHIP_VER(priv) == VERSION_8192D){
		 content |= priv->pshare->max_fw_macid << 16;
	}
#endif

	/*
	 * set macid
	 */
	 content |= pstat->aid << 8;

	/*
	 * set cmd id
	 */
	 content |= H2C_CMD_RSSI;

	signin_h2c_cmd(priv, content, 0);

	RESTORE_INT(flags);
}


void add_rssi_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;
	struct stat_info *pstat = NULL;
	unsigned int set_timer = 0;
	unsigned long flags;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	if (timer_pending(&priv->add_rssi_timer))
		del_timer_sync(&priv->add_rssi_timer);

#ifdef PCIE_POWER_SAVING
	if ((priv->pwr_state == L2) || (priv->pwr_state == L1))
			return;
#endif

	if (!list_empty(&priv->addrssi_list)) {
		pstat = list_entry(priv->addrssi_list.next, struct stat_info, addrssi_list);
		if (!pstat)
			return;

		if (!is_h2c_buf_occupy(priv)) {
			set_rssi_cmd(priv, pstat);
			if (!list_empty(&pstat->addrssi_list)) {
				SAVE_INT_AND_CLI(flags);
				SMP_LOCK(flags);
				list_del_init(&pstat->addrssi_list);
				RESTORE_INT(flags);
				SMP_UNLOCK(flags);
			}

			if (!list_empty(&priv->addrssi_list))
				set_timer++;
		} else {
			set_timer++;
		}
	}

	if (set_timer)
		mod_timer(&priv->add_rssi_timer, jiffies + RTL_MILISECONDS_TO_JIFFIES(50));	// 50 ms
}


void add_update_rssi(struct rtl8192cd_priv *priv, struct stat_info *pstat)
{
	unsigned long flags;

	if (is_h2c_buf_occupy(priv)) {
		if (list_empty(&pstat->addrssi_list)) {
			SAVE_INT_AND_CLI(flags);
			list_add_tail(&(pstat->addrssi_list), &(priv->addrssi_list));
			RESTORE_INT(flags);

			if (!timer_pending(&priv->add_rssi_timer))
				mod_timer(&priv->add_rssi_timer, jiffies + RTL_MILISECONDS_TO_JIFFIES(50));	// 50 ms
		}
	} else {
		set_rssi_cmd(priv, pstat);
	}
}


void set_RATid_cmd(struct rtl8192cd_priv *priv, unsigned int macid, unsigned int rateid, unsigned int ratemask)
{
	unsigned int content = 0;
	unsigned short ext_content = 0;

	/*
	 * set ratemask
	 */
	ext_content = ratemask & 0xffff;
	content = ((ratemask & 0xfff0000) >> 16) << 8;

	/*
	 * set short GI
	 */
	if (ratemask & BIT(28))
		content |= BIT(29);

	/*
	 * set macid (station aid)
	 */
	content |= (macid & 0x1f) << 24;

	/*
	 * set rateid (ARFR table)
	 */
	content |= (rateid & 0xf) << 20;

	/*
	 * set ext_content used
	 */
	content |= BIT(7);

	/*
	 * set cmd id
	 */
	content |= H2C_CMD_MACID;

	signin_h2c_cmd(priv, content, ext_content);
}

//3 ============================================================
//3 EDCCA
//3 ============================================================

void check_EDCCA(struct rtl8192cd_priv *priv, short rssi)
{
	if ((priv->pshare->rf_ft_var.edcca_thd) && (priv->pmib->dot11RFEntry.dot11channel==14
		|| priv->pshare->is_40m_bw
#if defined(CONFIG_RTL_92D_SUPPORT)
		||  (priv->pmib->dot11RFEntry.phyBandSelect & PHY_BAND_5G)
#endif
	)) {
		if((rssi > priv->pshare->rf_ft_var.edcca_thd) && (priv->pshare->phw->EDCCA_on == 0)) {
			RTL_W32(rOFDM0_ECCAThreshold, 0xfc03fd);
			priv->pshare->phw->EDCCA_on =1;
		} else if( (rssi < priv->pshare->rf_ft_var.edcca_thd-5) && priv->pshare->phw->EDCCA_on) {
			RTL_W32(rOFDM0_ECCAThreshold, 0x7f037f);
			priv->pshare->phw->EDCCA_on =0;
		}
	}
	if ((!priv->pshare->rf_ft_var.edcca_thd) && priv->pshare->phw->EDCCA_on) {
		RTL_W32(0xc4c, 0x7f037f);
		priv->pshare->phw->EDCCA_on = 0;
	}	
}


//3 ============================================================
//3 Antenna Diversity
//3 ============================================================
#ifdef SW_ANT_SWITCH

//
// 20100514 Luke/Joseph:
// Add new function to reset antenna diversity state after link.
//

void resetSwAntDivVariable(struct rtl8192cd_priv *priv)
{
	priv->pshare->RSSI_sum_R = 0;
	priv->pshare->RSSI_cnt_R = 0;
	priv->pshare->RSSI_sum_L = 0;
	priv->pshare->RSSI_cnt_L = 0;
	priv->pshare->TXByteCnt_R = 0;
	priv->pshare->TXByteCnt_L = 0;
	priv->pshare->RXByteCnt_R = 0;
	priv->pshare->RXByteCnt_L = 0;

}
void SwAntDivRestAfterLink(struct rtl8192cd_priv *priv)
{
	priv->pshare->RSSI_test = FALSE;
	priv->pshare->DM_SWAT_Table.try_flag = SWAW_STEP_RESET;
	memset(priv->pshare->DM_SWAT_Table.SelectAntennaMap, 0, sizeof(priv->pshare->DM_SWAT_Table.SelectAntennaMap));
	priv->pshare->DM_SWAT_Table.mapIndex = 0;
	priv->pshare->lastTxOkCnt = priv->net_stats.tx_bytes;
	priv->pshare->lastRxOkCnt = priv->net_stats.rx_bytes;
	resetSwAntDivVariable(priv);
}


void dm_SW_AntennaSwitchInit(struct rtl8192cd_priv *priv)
{
	if(!priv->pshare->rf_ft_var.antSw_enable)
		return;

//	if (GET_CHIP_VER(priv) == VERSION_8188C)
	if(get_rf_mimo_mode(priv)== MIMO_1T1R)
		priv->pshare->rf_ft_var.antSw_select = 0;

	//RT_TRACE(COMP_SWAS, DBG_LOUD, ("SWAS:Init SW Antenna Switch\n"));
	resetSwAntDivVariable(priv);
	priv->pshare->DM_SWAT_Table.CurAntenna = Antenna_L;
	priv->pshare->DM_SWAT_Table.try_flag = SWAW_STEP_RESET;
	memset(priv->pshare->DM_SWAT_Table.SelectAntennaMap, 0, sizeof(priv->pshare->DM_SWAT_Table.SelectAntennaMap));
	priv->pshare->DM_SWAT_Table.mapIndex = 0;

#ifdef GPIO_ANT_SWITCH
#ifdef CONFIG_RTL_92D_DMDP
		 if(priv->pshare->wlandev_idx==0)
			priv->pshare->rf_ft_var.antHw_enable=0;
		 else 
		 	return;
#endif
// GPIO 45 : 
// GPIO_MOD     => data port
// GPIO_IO_SEL  => output 
	RTL_W32(GPIO_PIN_CTRL, 0x00300000| RTL_R32(GPIO_PIN_CTRL));
	PHY_SetBBReg(priv, GPIO_PIN_CTRL, 0x3000, priv->pshare->DM_SWAT_Table.CurAntenna);
	RTL_W32(rFPGA0_XCD_RFParameter, RTL_R32(rFPGA0_XCD_RFParameter)| BIT(15)|BIT(16));		// enable ANTSEL

#else
	RTL_W32(LEDCFG, RTL_R32(LEDCFG) | BIT(23) );	//enable LED[1:0] pin as ANTSEL

	if ( !priv->pshare->rf_ft_var.antSw_select)	{
		RTL_W32(rFPGA0_XAB_RFParameter, RTL_R32(rFPGA0_XAB_RFParameter) | BIT(13) );	 //select ANTESEL from path A
		RTL_W32(rFPGA0_XAB_RFInterfaceSW, RTL_R32(rFPGA0_XAB_RFInterfaceSW) | BIT(8)| BIT(9) );  // enable ANTSEL  A as SW control
		RTL_W32(rFPGA0_XA_RFInterfaceOE, (RTL_R32(rFPGA0_XA_RFInterfaceOE) & ~(BIT(8)|BIT(9)))| 0x01<<8 );	// 0x01: left antenna, 0x02: right antenna
	} else 	{
		RTL_W32(rFPGA0_XAB_RFParameter, RTL_R32(rFPGA0_XAB_RFParameter) & ~ BIT(13) );	 //select ANTESEL from path B
		RTL_W32(rFPGA0_XAB_RFInterfaceSW, RTL_R32(rFPGA0_XAB_RFInterfaceSW) | BIT(24)| BIT(25) );  // enable ANTSEL B as SW control
		RTL_W32(rFPGA0_XB_RFInterfaceOE, (RTL_R32(rFPGA0_XB_RFInterfaceOE) & ~(BIT(8)|BIT(9)))| 0x01<<8 );	// 0x01: left antenna, 0x02: right antenna
	}
	RTL_W16(rFPGA0_TxInfo, (RTL_R16(rFPGA0_TxInfo)&0xf0ff) | BIT(8) );	// b11-b8=0001
#endif

	// Move the timer initialization to InitializeVariables function.
	//PlatformInitializeTimer(Adapter, &pMgntInfo->SwAntennaSwitchTimer, (RT_TIMER_CALL_BACK)dm_SW_AntennaSwitchCallback, NULL, "SwAntennaSwitchTimer");
}
//
// 20100514 Luke/Joseph:
// Add new function for antenna diversity after link.
// This is the main function of antenna diversity after link.
// This function is called in HalDmWatchDog() and dm_SW_AntennaSwitchCallback().
// HalDmWatchDog() calls this function with SWAW_STEP_PEAK to initialize the antenna test.
// In SWAW_STEP_PEAK, another antenna and a 500ms timer will be set for testing.
// After 500ms, dm_SW_AntennaSwitchCallback() calls this function to compare the signal just
// listened on the air with the RSSI of original antenna.
// It chooses the antenna with better RSSI.
// There is also a aged policy for error trying. Each error trying will cost more 5 seconds waiting
// penalty to get next try.
//
void dm_SW_AntennaSwitch(struct rtl8192cd_priv *priv, char Step)
{
	unsigned int	curTxOkCnt, curRxOkCnt;
	unsigned int	CurByteCnt, PreByteCnt;
	int 			Score_R=0, Score_L=0;
	int				RSSI_R, RSSI_L;
	char 			nextAntenna=priv->pshare->DM_SWAT_Table.CurAntenna;
	int				i;

//1 1. Determine which condition should turn off Antenna Diversity

#ifdef MP_TEST
	if ((OPMODE & WIFI_MP_STATE) || priv->pshare->rf_ft_var.mp_specific)
		return;
#endif

//	if(!(GET_CHIP_VER(priv) == VERSION_8188C) || !priv->pshare->rf_ft_var.antSw_enable)
//		return;

	if((!priv->assoc_num)
#ifdef PCIE_POWER_SAVING
	|| (priv->pwr_state == L2) || (priv->pwr_state == L1)
#endif
	){
		SwAntDivRestAfterLink(priv);
		return;
	}

	// Handling step mismatch condition.
	// Peak step is not finished at last time. Recover the variable and check again.
	if( Step != priv->pshare->DM_SWAT_Table.try_flag)
	{
		SwAntDivRestAfterLink(priv);
	}

//1 2. Initialization: Select a assocaiated AP or STA as RSSI target
	if(priv->pshare->DM_SWAT_Table.try_flag == SWAW_STEP_RESET) {
#ifdef CLIENT_MODE
		if((OPMODE & (WIFI_STATION_STATE | WIFI_ASOC_STATE)) == (WIFI_STATION_STATE | WIFI_ASOC_STATE)) {
			// Target: Infrastructure mode AP.
			priv->pshare->RSSI_target = NULL;
		}
#endif
		resetSwAntDivVariable(priv);
		priv->pshare->DM_SWAT_Table.try_flag = SWAW_STEP_PEAK;
		return;
	}
	else  {

//1 3. Antenna Diversity

		//2 Calculate TX and RX OK bytes

		curTxOkCnt = priv->net_stats.tx_bytes - priv->pshare->lastTxOkCnt;
		curRxOkCnt = priv->net_stats.rx_bytes - priv->pshare->lastRxOkCnt;
		priv->pshare->lastTxOkCnt = priv->net_stats.tx_bytes;
		priv->pshare->lastRxOkCnt = priv->net_stats.rx_bytes;

		//2 Try State
		if(priv->pshare->DM_SWAT_Table.try_flag == SWAW_STEP_DETERMINE)	{
			//3 1. Seperately caculate TX and RX OK byte counter for ant A and B
			if(priv->pshare->DM_SWAT_Table.CurAntenna == Antenna_R) {
				priv->pshare->TXByteCnt_R += curTxOkCnt;
				priv->pshare->RXByteCnt_R += curRxOkCnt;
			} else 	{
				priv->pshare->TXByteCnt_L += curTxOkCnt;
				priv->pshare->RXByteCnt_L += curRxOkCnt;
			}

			//3 2. Change anntena for testing
			if(priv->pshare->DM_SWAT_Table.RSSI_Trying != 0) {
				nextAntenna = (priv->pshare->DM_SWAT_Table.CurAntenna ) ^ Antenna_MAX;
				priv->pshare->DM_SWAT_Table.RSSI_Trying--;
			}

			//2 Try State End: Determine the best antenna

			if(priv->pshare->DM_SWAT_Table.RSSI_Trying==0) {
				nextAntenna = priv->pshare->DM_SWAT_Table.CurAntenna;
				priv->pshare->DM_SWAT_Table.mapIndex = (priv->pshare->DM_SWAT_Table.mapIndex+1)%SELANT_MAP_SIZE;

				//3 TP Mode: Determine the best antenna by throuhgput
				if(priv->pshare->DM_SWAT_Table.TestMode == TP_MODE)  {



					//3 (1) Saperately caculate total byte count for two antennas
					if(priv->pshare->DM_SWAT_Table.CurAntenna == Antenna_R) {
						CurByteCnt = (priv->pshare->TXByteCnt_R + (priv->pshare->RXByteCnt_R<<1));
						PreByteCnt = (priv->pshare->TXByteCnt_L + (priv->pshare->RXByteCnt_L<<1));
					} else {
						CurByteCnt = (priv->pshare->TXByteCnt_L + (priv->pshare->RXByteCnt_L<<1));
						PreByteCnt = (priv->pshare->TXByteCnt_R + (priv->pshare->RXByteCnt_R<<1));
					}

					//3 (2) Throughput Normalization
					if(priv->pshare->TrafficLoad == TRAFFIC_HIGH)
						CurByteCnt >>=3;
					else if(priv->pshare->TrafficLoad == TRAFFIC_LOW)
						CurByteCnt >>=1;

					if(priv->pshare->DM_SWAT_Table.CurAntenna == Antenna_R) {
						priv->pshare->DM_SWAT_Table.SelectAntennaMap[0][priv->pshare->DM_SWAT_Table.mapIndex] = PreByteCnt;
						priv->pshare->DM_SWAT_Table.SelectAntennaMap[1][priv->pshare->DM_SWAT_Table.mapIndex] = CurByteCnt;
					} else {
						priv->pshare->DM_SWAT_Table.SelectAntennaMap[0][priv->pshare->DM_SWAT_Table.mapIndex] = CurByteCnt;
						priv->pshare->DM_SWAT_Table.SelectAntennaMap[1][priv->pshare->DM_SWAT_Table.mapIndex] = PreByteCnt;
					}

					Score_R = Score_L=0;
					for (i= 0; i<SELANT_MAP_SIZE; i++) {
						Score_L += priv->pshare->DM_SWAT_Table.SelectAntennaMap[0][i];
						Score_R += priv->pshare->DM_SWAT_Table.SelectAntennaMap[1][i];
					}

					nextAntenna = (Score_L > Score_R) ? Antenna_L : Antenna_R;

					if(priv->pshare->rf_ft_var.ant_dump&8)
						panic_printk("Mode TP, select Ant%d, [Score1=%d,Score2=%d]\n", nextAntenna, Score_L, Score_R);

				}

				//3 RSSI Mode: Determine the best anntena by RSSI
				else if(priv->pshare->DM_SWAT_Table.TestMode == RSSI_MODE) {

					//2 Saperately caculate average RSSI for two antennas
					RSSI_L = RSSI_R = 0;

					if(priv->pshare->RSSI_cnt_R > 0)
						RSSI_R = priv->pshare->RSSI_sum_R/priv->pshare->RSSI_cnt_R;
					if(priv->pshare->RSSI_cnt_L > 0)
						RSSI_L = priv->pshare->RSSI_sum_L/priv->pshare->RSSI_cnt_L;

					if(RSSI_L && RSSI_R )
						nextAntenna =  (RSSI_L > RSSI_R) ? Antenna_L : Antenna_R;

					if(priv->pshare->rf_ft_var.ant_dump&8)
						panic_printk("Mode RSSI, RSSI_R=%d(%d), RSSI_L=%d(%d), Ant=%d\n",
						RSSI_R, priv->pshare->RSSI_cnt_R, RSSI_L,  priv->pshare->RSSI_cnt_L, nextAntenna);

				}

				//3 Reset state
				resetSwAntDivVariable(priv);
				priv->pshare->DM_SWAT_Table.try_flag = SWAW_STEP_PEAK;
				priv->pshare->RSSI_test = FALSE;
			}
		}

		//1 Normal State
		else if(priv->pshare->DM_SWAT_Table.try_flag == SWAW_STEP_PEAK)	{

			//3 Determine TP/RSSI mode by TRX OK count
			if((curRxOkCnt+curTxOkCnt) > TP_MODE_THD) {
				//2 Determine current traffic is high or low
				if((curTxOkCnt+curRxOkCnt) > TRAFFIC_THRESHOLD)
					priv->pshare->TrafficLoad = TRAFFIC_HIGH;
				else
					priv->pshare->TrafficLoad = TRAFFIC_LOW;

				priv->pshare->DM_SWAT_Table.RSSI_Trying = 10;
				priv->pshare->DM_SWAT_Table.TestMode = TP_MODE;
			} else	{

				int idx = 0;
				struct stat_info* pEntry = findNextSTA(priv, &idx);
				priv->pshare->RSSI_target = NULL;
				while(pEntry) {
					if(pEntry && pEntry->expire_to) {
						if(!priv->pshare->RSSI_target)
							priv->pshare->RSSI_target = pEntry;
						else if( pEntry->rssi < priv->pshare->RSSI_target->rssi )
							priv->pshare->RSSI_target = pEntry;
					}
					pEntry = findNextSTA(priv, &idx);
				};

				priv->pshare->DM_SWAT_Table.RSSI_Trying = 6;
				priv->pshare->DM_SWAT_Table.TestMode = RSSI_MODE;

				if(priv->pshare->RSSI_target == NULL)	{
					SwAntDivRestAfterLink(priv);
					return;
				}

				//3 reset state
				memset(priv->pshare->DM_SWAT_Table.SelectAntennaMap, 0, sizeof(priv->pshare->DM_SWAT_Table.SelectAntennaMap));
			}

			//3 Begin  to enter Try State
			nextAntenna = (priv->pshare->DM_SWAT_Table.CurAntenna ) ^ Antenna_MAX;
			priv->pshare->DM_SWAT_Table.try_flag = SWAW_STEP_DETERMINE;
			priv->pshare->RSSI_test = TRUE;

			//3 Reset variables
			resetSwAntDivVariable(priv);
		}
	}

//1 4.Change TRX antenna
	if(nextAntenna != priv->pshare->DM_SWAT_Table.CurAntenna) {

#ifdef GPIO_ANT_SWITCH
		PHY_SetBBReg(priv, GPIO_PIN_CTRL, 0x3000, nextAntenna);	
#else		
		if (!priv->pshare->rf_ft_var.antSw_select)
			PHY_SetBBReg(priv, rFPGA0_XA_RFInterfaceOE, 0x300, nextAntenna);
		else
			PHY_SetBBReg(priv, rFPGA0_XB_RFInterfaceOE, 0x300, nextAntenna);
#endif
	}

//1 5.Reset Statistics
	priv->pshare->DM_SWAT_Table.CurAntenna = nextAntenna;

//1 6.Set next timer

	if(priv->pshare->DM_SWAT_Table.RSSI_Trying == 0) {
		return;
	}

	if(priv->pshare->DM_SWAT_Table.TestMode == RSSI_MODE)	{
		mod_timer(&priv->pshare->swAntennaSwitchTimer, jiffies +40);		// 400 ms
	} else if(priv->pshare->DM_SWAT_Table.TestMode == TP_MODE)	{

		if(priv->pshare->TrafficLoad == TRAFFIC_HIGH)	{
			if(priv->pshare->DM_SWAT_Table.RSSI_Trying%2 == 0)
				mod_timer(&priv->pshare->swAntennaSwitchTimer, jiffies + 1);	// 10 ms
			else
				mod_timer(&priv->pshare->swAntennaSwitchTimer, jiffies + 8);	// 80 ms

		} else if(priv->pshare->TrafficLoad == TRAFFIC_LOW) {
			if(priv->pshare->DM_SWAT_Table.RSSI_Trying%2 == 0)
				mod_timer(&priv->pshare->swAntennaSwitchTimer, jiffies + 4);	// 40 ms
			else
				mod_timer(&priv->pshare->swAntennaSwitchTimer, jiffies + 8);	// 80 ms
		}
	}

}


void dm_SW_AntennaSwitchCallback(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv*)task_priv;
	unsigned long flags;
	SAVE_INT_AND_CLI(flags);
	dm_SW_AntennaSwitch(priv, SWAW_STEP_DETERMINE);
	RESTORE_INT(flags);
}

//
// 20100514 Luke/Joseph:
// This function is used to gather the RSSI information for antenna testing.
// It selects the RSSI of the peer STA that we want to know.
//
void dm_SWAW_RSSI_Check(struct rtl8192cd_priv *priv, struct rx_frinfo *pfrinfo)
{
	struct stat_info* pEntry = NULL;
	pEntry = get_stainfo(priv, GetAddr2Ptr(get_pframe(pfrinfo)));

	if((priv->pshare->RSSI_target==NULL)||(priv->pshare->RSSI_target==pEntry)) {
		//1 RSSI for SW Antenna Switch
		if(priv->pshare->DM_SWAT_Table.CurAntenna == Antenna_R)
		{
			priv->pshare->RSSI_sum_R += pfrinfo->rssi;
			priv->pshare->RSSI_cnt_R++;
		} else {
			priv->pshare->RSSI_sum_L += pfrinfo->rssi;
			priv->pshare->RSSI_cnt_L++;
		}
	}
}

#ifndef HW_ANT_SWITCH

int diversity_antenna_select(struct rtl8192cd_priv *priv, unsigned char *data)
{
	int ant = _atoi(data, 16);
//	if(GET_CHIP_VER(priv) != VERSION_8188C)
//		return 0;

#ifdef PCIE_POWER_SAVING
		PCIeWakeUp(priv, POWER_DOWN_T0);
#endif
	if(ant==Antenna_L || ant==Antenna_R) {
#ifdef GPIO_ANT_SWITCH
		PHY_SetBBReg(priv, GPIO_PIN_CTRL, 0x3000, ant); 
#else		
		if (!priv->pshare->rf_ft_var.antSw_select)
			PHY_SetBBReg(priv, rFPGA0_XA_RFInterfaceOE, 0x300, ant);
		else
			PHY_SetBBReg(priv, rFPGA0_XB_RFInterfaceOE, 0x300, ant);
#endif		
		priv->pshare->DM_SWAT_Table.CurAntenna = ant;
		priv->pshare->rf_ft_var.antSw_enable = 0;
		SwAntDivRestAfterLink(priv);
		memset(priv->pshare->DM_SWAT_Table.SelectAntennaMap, 0, sizeof(priv->pshare->DM_SWAT_Table.SelectAntennaMap));
		return 1;
	} else {
		priv->pshare->rf_ft_var.antSw_enable = 1;
		priv->pshare->lastTxOkCnt = priv->net_stats.tx_bytes;
		priv->pshare->lastRxOkCnt = priv->net_stats.rx_bytes;

		return 0;
	}
}
#endif
#endif
#if defined(HW_ANT_SWITCH)

void dm_HW_AntennaSwitchInit(struct rtl8192cd_priv *priv)
{
	if(!priv->pshare->rf_ft_var.antHw_enable)
		return;
#ifdef SW_ANT_SWITCH
	priv->pshare->rf_ft_var.antSw_enable =0;
#endif

//	if (GET_CHIP_VER(priv) == VERSION_8188C)
	if(get_rf_mimo_mode(priv)== MIMO_1T1R)
		priv->pshare->rf_ft_var.antSw_select = 0;
	
	if ( !priv->pshare->rf_ft_var.antSw_select)	{
		RTL_W32(rFPGA0_XAB_RFParameter, RTL_R32(rFPGA0_XAB_RFParameter) | BIT(13) );	 //select ANTESEL from path A
		RTL_W32(rFPGA0_XAB_RFInterfaceSW, RTL_R32(rFPGA0_XAB_RFInterfaceSW) & ~(BIT(8)| BIT(9)) ); // ANTSEL as HW control
		RTL_W32(rFPGA0_XA_RFInterfaceOE, (RTL_R32(rFPGA0_XA_RFInterfaceOE) & ~(BIT(8)|BIT(9)))| 0x01<<8 );	// 0x01: left antenna, 0x02: right antenna
		RTL_W8(0xc50, RTL_R8(0xc50) | BIT(7));	// Enable Hardware antenna switch
		RTL_W32(0xc54, RTL_R32(0xc54) | BIT(23) );	// Decide final antenna by comparing 2 antennas' pwdb
	} else 	{
		RTL_W32(rFPGA0_XAB_RFParameter, RTL_R32(rFPGA0_XAB_RFParameter) & ~ BIT(13) );	 //select ANTESEL from path B
		RTL_W32(rFPGA0_XAB_RFInterfaceSW, RTL_R32(rFPGA0_XAB_RFInterfaceSW) & ~(BIT(24)| BIT(25)) ); // ANTSEL as HW control
		RTL_W32(rFPGA0_XB_RFInterfaceOE, (RTL_R32(rFPGA0_XB_RFInterfaceOE) & ~(BIT(8)|BIT(9)))| 0x01<<8 );	// 0x01: left antenna, 0x02: right antenna
#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
		if (
#ifdef CONFIG_RTL_92C_SUPPORT
			(GET_CHIP_VER(priv)==VERSION_8192C) || (GET_CHIP_VER(priv)==VERSION_8188C) 
#endif
#ifdef CONFIG_RTL_92D_SUPPORT
#ifdef CONFIG_RTL_92C_SUPPORT
			|| 
#endif
			(GET_CHIP_VER(priv)==VERSION_8192D)
#endif
			)
			RTL_W8(0xc58, RTL_R8(0xc58) | BIT(7));	// Enable Hardware antenna switch
#endif
		RTL_W32(0xc5C, RTL_R32(0xc5c) | BIT(23) );	// Decide final antenna by comparing 2 antennas' pwdb
	}

	priv->pshare->rf_ft_var.CurAntenna = 0;

	RTL_W32(LEDCFG, RTL_R32(LEDCFG) | BIT(23) );	//enable LED[1:0] pin as ANTSEL
	RTL_W16(0xca4, (RTL_R16(0xca4) & ~(0xfff))|0x0c0); 	// Pwdb threshold=12dB
	RTL_W32(0x874, RTL_R32(0x874) & ~ BIT(23) );	// No update ANTSEL during GNT_BT=1
	RTL_W16(rFPGA0_TxInfo, (RTL_R16(rFPGA0_TxInfo)&0xf0ff) | BIT(8) );	// b11-b8=0001
	RTL_W32(0x80c, RTL_R32(0x80c) | BIT(21) );		// assign antenna by tx desc

	// CCK setting
	RTL_W8(0xa01, RTL_R8(0xa01) | BIT(7));			// enable hw ant diversity
	RTL_W8(0xa0c, (RTL_R8(0xa0c) & 0xe0) | 0x0f );	// b4=0, b3:0 = 1111	32 sample
	RTL_W8(0xa11, RTL_R8(0xa11) | BIT(5));			// do not change default optional antenna
	RTL_W8(0xa14, (RTL_R8(0xa14) & 0xe0) | 0x08 );	// default : optional = 1:1
#ifdef GPIO_ANT_SWITCH
	PHY_SetBBReg(priv, rFPGA0_XCD_RFParameter, 0x40000000, 0x01);		// enable ANTSEL
#endif	

}

void setRxIdleAnt(struct rtl8192cd_priv *priv, char Ant)
{
	if(priv->pshare->rf_ft_var.CurAntenna != Ant) {
		if(Ant) {
			RTL_W32(0x858, 0x65a965a9);
//			RTL_W8(0x6d8, RTL_R8(0x6d8) | BIT(6) );
		}
		else {
			RTL_W32(0x858, 0x569a569a);
//			RTL_W8(0x6d8, RTL_R8(0x6d8) & (~ BIT(6)));
		}
		priv->pshare->rf_ft_var.CurAntenna = Ant;
	}
}

void dm_STA_Ant_Select(struct rtl8192cd_priv *priv, struct stat_info *pstat)
{
	int ScoreA=0, ScoreB=0, i, nextAnt= pstat->CurAntenna, idleAnt=priv->pshare->rf_ft_var.CurAntenna;

	if((priv->pshare->rf_ft_var.CurAntenna & 0x80)
		|| ((pstat->hwRxAntSel[0] + pstat->hwRxAntSel[1])==0 && (pstat->cckPktCount[0] + pstat->cckPktCount[1])<10)	)
		return;

	for(i=0; i<2; i++) {
		if(pstat->cckPktCount[i]==0 && pstat->hwRxAntSel[i]==0)
			pstat->AntRSSI[i] = 0;
	}

	if(pstat->hwRxAntSel[0] || pstat->hwRxAntSel[1]) {
		ScoreA = pstat->hwRxAntSel[0];
		ScoreB = pstat->hwRxAntSel[1];

		if(ScoreA != ScoreB) {
			if(ScoreA > ScoreB)
				nextAnt = 0;
			else
				nextAnt = 1;
		}
	} else {
		ScoreA = pstat->cckPktCount[0];
		ScoreB = pstat->cckPktCount[1];

		if(ScoreA > 5*ScoreB)
			nextAnt = 0;
		else if(ScoreB > 5*ScoreA)
			nextAnt = 1;
		else if(ScoreA > ScoreB)
			nextAnt = 1;
		else if(ScoreB > ScoreA)
			nextAnt = 0;
	}

	pstat->CurAntenna = nextAnt;

	if(priv->pshare->rf_ft_var.ant_dump&2) {
		panic_printk("id=%d, OFDM/CCK: (%d, %d/%d, %d), RSSI:(%d, %d), ant=%d, RxIdle=%d\n",
			pstat->aid,
			pstat->hwRxAntSel[1],
			pstat->hwRxAntSel[0],
			pstat->cckPktCount[1],
			pstat->cckPktCount[0],
			pstat->AntRSSI[1],
			pstat->AntRSSI[0],
			(pstat->CurAntenna==0 ? 2: 1)
			 ,((priv->pshare->rf_ft_var.CurAntenna&1)==0 ? 2 : 1)
			 );
	}

	if(pstat->AntRSSI[idleAnt]==0)
		pstat->AntRSSI[idleAnt] = pstat->AntRSSI[idleAnt^1];

// reset variables
	pstat->hwRxAntSel[1] = pstat->hwRxAntSel[0] =0;
	pstat->cckPktCount[1]= pstat->cckPktCount[0] =0;

}

void dm_HW_IdleAntennaSelect(struct rtl8192cd_priv *priv)
{
	struct stat_info	*pstat, *pstat_min=NULL;
	struct list_head	*phead, *plist;
	int rssi_min= 0xff, i;

	if(priv->pshare->rf_ft_var.CurAntenna & 0x80)
		return;

	phead = &priv->asoc_list;
	plist = phead->next;

	while(plist != phead)	{
		pstat = list_entry(plist, struct stat_info, asoc_list);
		if((pstat->expire_to) && (pstat->AntRSSI[0] || pstat->AntRSSI[1])) {
			int rssi = (pstat->AntRSSI[0] < pstat->AntRSSI[1]) ? pstat->AntRSSI[0] : pstat->AntRSSI[1];
			if((!pstat_min) || ( rssi < rssi_min) ) {
				pstat_min = pstat;
				rssi_min = rssi;
			}
		}
		if (plist == plist->next)
			break;
		plist = plist->next;
	};

	if(pstat_min)
		setRxIdleAnt(priv, pstat_min->CurAntenna);


#ifdef TX_SHORTCUT
	if (!priv->pmib->dot11OperationEntry.disable_txsc) {
		plist = phead->next;
		while(plist != phead)	{
			pstat = list_entry(plist, struct stat_info, asoc_list);
			if(pstat->expire_to) {
				for (i=0; i<TX_SC_ENTRY_NUM; i++) {
					struct tx_desc *pdesc= &(pstat->tx_sc_ent[i].hwdesc1);	
					pdesc->Dword2 &= set_desc(~ (BIT(24)|BIT(25)));
					if((pstat->CurAntenna^priv->pshare->rf_ft_var.CurAntenna)&1)
						pdesc->Dword2 |= set_desc(BIT(24)|BIT(25));
					pdesc= &(pstat->tx_sc_ent[i].hwdesc2);	
					pdesc->Dword2 &= set_desc(~ (BIT(24)|BIT(25)));
					if((pstat->CurAntenna^priv->pshare->rf_ft_var.CurAntenna)&1)
						pdesc->Dword2 |= set_desc(BIT(24)|BIT(25));					
				}
			}		

			if (plist == plist->next)
				break;
			plist = plist->next;
		};
	}
#endif	

}

int diversity_antenna_select(struct rtl8192cd_priv *priv, unsigned char *data)
{
	int ant = _atoi(data, 16);

#ifdef PCIE_POWER_SAVING
	PCIeWakeUp(priv, POWER_DOWN_T0);
#endif

	if (ant==Antenna_L || ant==Antenna_R) {

#ifdef GPIO_ANT_SWITCH		
		if(priv->pshare->rf_ft_var.antSw_enable)  {
			PHY_SetBBReg(priv, GPIO_PIN_CTRL, 0x3000, ant); 
		} else
#endif
		{
			if ( !priv->pshare->rf_ft_var.antSw_select) {
				RTL_W32(rFPGA0_XAB_RFInterfaceSW, RTL_R32(rFPGA0_XAB_RFInterfaceSW) | BIT(8)| BIT(9) );  //  ANTSEL A as SW control
				RTL_W8(0xc50, RTL_R8(0xc50) & (~ BIT(7)));	// rx OFDM SW control
				PHY_SetBBReg(priv, rFPGA0_XA_RFInterfaceOE, 0x300, ant);
			} else {
				RTL_W32(rFPGA0_XAB_RFInterfaceSW, RTL_R32(rFPGA0_XAB_RFInterfaceSW) | BIT(24)| BIT(25) ); // ANTSEL B as HW control
				PHY_SetBBReg(priv, rFPGA0_XB_RFInterfaceOE, 0x300, ant);
#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
				if (
#ifdef CONFIG_RTL_92C_SUPPORT
					(GET_CHIP_VER(priv)==VERSION_8192C) || (GET_CHIP_VER(priv)==VERSION_8188C) 
#endif
#ifdef CONFIG_RTL_92D_SUPPORT
#ifdef CONFIG_RTL_92C_SUPPORT
					|| 
#endif
					(GET_CHIP_VER(priv)==VERSION_8192D)
#endif
					)
					RTL_W8(0xc58, RTL_R8(0xc58) & (~ BIT(7)));		// rx OFDM SW control
#endif
			}
			RTL_W8(0xa01, RTL_R8(0xa01) & (~ BIT(7)));	// rx CCK SW control
			RTL_W32(0x80c, RTL_R32(0x80c) & (~ BIT(21))); // select ant by tx desc
			RTL_W32(0x858, 0x569a569a);
		}
		if(HW_DIV_ENABLE)
			priv->pshare->rf_ft_var.antHw_enable = BIT(5);			
		priv->pshare->rf_ft_var.CurAntenna  = (ant%2);
#ifdef SW_ANT_SWITCH
		if(priv->pshare->rf_ft_var.antSw_enable) 
			priv->pshare->rf_ft_var.antSw_enable = BIT(5);
		priv->pshare->DM_SWAT_Table.CurAntenna = ant;
		priv->pshare->RSSI_test =0;
#endif
	}
	else if(ant==0){
#ifdef GPIO_ANT_SWITCH		
			if(priv->pshare->rf_ft_var.antHw_enable) 
#endif
			{
			if (!priv->pshare->rf_ft_var.antSw_select)  {
				RTL_W32(rFPGA0_XAB_RFInterfaceSW, RTL_R32(rFPGA0_XAB_RFInterfaceSW) & ~(BIT(8)| BIT(9)) );
				RTL_W8(0xc50, RTL_R8(0xc50) | BIT(7));	// OFDM HW control
			} else {
				RTL_W32(rFPGA0_XAB_RFInterfaceSW, RTL_R32(rFPGA0_XAB_RFInterfaceSW) & ~(BIT(24)| BIT(25)) );
#if defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT)
			if (
#ifdef CONFIG_RTL_92C_SUPPORT
				(GET_CHIP_VER(priv)==VERSION_8192C) || (GET_CHIP_VER(priv)==VERSION_8188C) 
#endif
#ifdef CONFIG_RTL_92D_SUPPORT
#ifdef CONFIG_RTL_92C_SUPPORT
				|| 
#endif
				(GET_CHIP_VER(priv)==VERSION_8192D)
#endif
				)
				RTL_W8(0xc58, RTL_R8(0xc58) | BIT(7));	// OFDM HW control
#endif
			}

			RTL_W8(0xa01, RTL_R8(0xa01) | BIT(7));	// CCK HW control
			RTL_W32(0x80c, RTL_R32(0x80c) | BIT(21) ); // by tx desc
			priv->pshare->rf_ft_var.CurAntenna = 0;
			RTL_W32(0x858, 0x569a569a);
			priv->pshare->rf_ft_var.antHw_enable = 1;
#ifdef SW_ANT_SWITCH
			priv->pshare->rf_ft_var.antSw_enable = 0;
			priv->pshare->RSSI_test =0;
#endif
		}
#ifdef SW_ANT_SWITCH
		if(priv->pshare->rf_ft_var.antSw_enable) {
			dm_SW_AntennaSwitchInit(priv);
			RTL_W32(0x858, 0x569a569a);
			priv->pshare->lastTxOkCnt = priv->net_stats.tx_bytes;
			priv->pshare->lastRxOkCnt = priv->net_stats.rx_bytes;
			priv->pshare->rf_ft_var.antHw_enable = 0;
			priv->pshare->rf_ft_var.antSw_enable = 1;
		}
#endif

	}
#if defined(SW_ANT_SWITCH) && !defined(GPIO_ANT_SWITCH)
	else if(ant==3) {
		if(!priv->pshare->rf_ft_var.antSw_enable) {
			dm_SW_AntennaSwitchInit(priv);
			RTL_W32(0x858, 0x569a569a);
			priv->pshare->lastTxOkCnt = priv->net_stats.tx_bytes;
			priv->pshare->lastRxOkCnt = priv->net_stats.rx_bytes;
		}
#ifdef CONFIG_RTL_88E_SUPPORT
		if (GET_CHIP_VER(priv)==VERSION_8188E) {
				RTL_W8(0xc50, RTL_R8(0xc50) & (~ BIT(7)));	// rx OFDM SW control
		} else
#endif
		{
			if ( !priv->pshare->rf_ft_var.antSw_select)
				RTL_W8(0xc50, RTL_R8(0xc50) & (~ BIT(7)));	// rx OFDM SW control
			else
				RTL_W8(0xc58, RTL_R8(0xc58) & (~ BIT(7)));	// rx OFDM SW control
		}

		RTL_W8(0xa01, RTL_R8(0xa01) & (~ BIT(7)));		// rx CCK SW control
		RTL_W32(0x80c, RTL_R32(0x80c) & (~ BIT(21))); 	// select ant by tx desc
		priv->pshare->rf_ft_var.antHw_enable = 0;
		priv->pshare->rf_ft_var.antSw_enable = 1;

	}
#endif

	return 1;
}

#endif

//3 ============================================================
//3 Dynamic Noise Control
//3 ============================================================

#if defined(CONFIG_RTL_92D_SUPPORT) && defined(CONFIG_RTL_NOISE_CONTROL)
void dnc_timer(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;
	struct stat_info *pstat = NULL;
	unsigned int set_timer = 0;
	unsigned long flags;

	if (!(priv->drv_state & DRV_STATE_OPEN))
		return;

	if (timer_pending(&priv->dnc_timer))
		del_timer_sync(&priv->dnc_timer);

#ifdef PCIE_POWER_SAVING
	if ((priv->pwr_state == L2) || (priv->pwr_state == L1))
			return;
#endif

	if (priv->pmib->dot11RFEntry.phyBandSelect == PHY_BAND_5G) {
		//PHY_SetBBReg(priv, 0xb30, bMaskDWord, 0x00a00000);
		PHY_SetBBReg(priv, 0x870, bMaskDWord, 0x07600760);
		PHY_SetBBReg(priv, 0xc50, bMaskByte0, 0x20);
		PHY_SetBBReg(priv, 0xc58, bMaskByte0, 0x20);
	}
}
#endif


//3 ============================================================
//3 Leaving STA check
//3 ============================================================

#if defined(DETECT_STA_EXISTANCE) && (defined(CONFIG_RTL_92C_SUPPORT) || defined(CONFIG_RTL_92D_SUPPORT))
// Check for STA existance. If STA disappears, disconnect it. Added by Annie, 2010-08-10.
void DetectSTAExistance(struct rtl8192cd_priv *priv, struct tx_rpt *report, struct stat_info *pstat)
{
	unsigned char tmpbuf[16];

	// Parameters
	const unsigned int		maxTxFailCnt = 300;		// MAX Tx fail packet count
	const unsigned int		minTxFailCnt = 30;		// MIN Tx fail packet count; this value should be less than maxTxFailCnt.
	const unsigned int		txFailSecThr= 3;			// threshold of Tx Fail Time (in second)

	// Temporarily change Retry Limit when TxFail. (tfrl: TxFailRetryLimit)
	const unsigned char	TFRL = 7;				// New Retry Limit value
	const unsigned char	TFRL_FailCnt = 2;		// Tx Fail Count threshold to set Retry Limit
	const unsigned char	TFRL_SetTime = 2;		// Time to set Retry Limit (in second)
	const unsigned char	TFRL_RcvTime = 10;		// Time to recover Retry Limit (in second)

	if( report->txok != 0 )
	{ // Reset Counter
		pstat->tx_conti_fail_cnt = 0;
		pstat->tx_last_good_time = priv->up_time;
		pstat->leave = 0;
	}
	else if( report->txfail != 0 )
	{
		pstat->tx_conti_fail_cnt += report->txfail;
		DEBUG_WARN( "detect: txfail=%d, tx_conti_fail_cnt=%d\n", report->txfail, pstat->tx_conti_fail_cnt );

		if(	priv->up_time >= (pstat->tx_last_good_time+TFRL_SetTime) &&
			pstat->tx_conti_fail_cnt >= TFRL_FailCnt && 
			//!pstat->ht_cap_len && // legacy rate only
			!priv->pshare->bRLShortened )
		{ // Shorten retry limit, because AP spending too much time to send out g mode STA pending packets in HW queue.
			RTL_W16(RL, (TFRL&SRL_Mask)<<SRL_SHIFT|(TFRL&LRL_Mask)<<LRL_SHIFT);
			priv->pshare->bRLShortened = TRUE;
			DEBUG_WARN( "== Shorten RetryLimit to 0x%04X ==\n", RTL_R16(RL) );
		}

		if( 	(pstat->tx_conti_fail_cnt >= maxTxFailCnt) ||
			(pstat->tx_conti_fail_cnt >= minTxFailCnt && priv->up_time >= (pstat->tx_last_good_time+txFailSecThr) )
			)
		{ // This STA is considered as disappeared, so delete it.
			DEBUG_WARN( "** tx_conti_fail_cnt=%d (min=%d,max=%d)\n", pstat->tx_conti_fail_cnt, minTxFailCnt, maxTxFailCnt);
			DEBUG_WARN( "** tx_last_good_time=%d, up_time=%d (Thr:%d)\n", (int)pstat->tx_last_good_time, (int)priv->up_time, txFailSecThr );
			DEBUG_WARN( "AP is going to del_sta %02X:%02X:%02X:%02X:%02X:%02X\n", pstat->hwaddr[0],pstat->hwaddr[1],pstat->hwaddr[2],pstat->hwaddr[3],pstat->hwaddr[4],pstat->hwaddr[5] );

			sprintf((char *)tmpbuf, "%02x%02x%02x%02x%02x%02x", pstat->hwaddr[0],pstat->hwaddr[1],pstat->hwaddr[2],pstat->hwaddr[3],pstat->hwaddr[4],pstat->hwaddr[5]);
		
//			del_sta(priv, tmpbuf);
			++(pstat->leave);

			if (timer_pending(&priv->pshare->rl_recover_timer))
				del_timer_sync (&priv->pshare->rl_recover_timer);
			mod_timer(&priv->pshare->rl_recover_timer, jiffies + EXPIRE_TO*TFRL_RcvTime);

			// Reset Counter
			pstat->tx_conti_fail_cnt = 0;
			pstat->tx_last_good_time = priv->up_time;
		}
	}
}

// Timer callback function to recover hardware retry limit register. Added by Annie, 2010-08-10.
void RetryLimitRecovery(unsigned long task_priv)
{
	struct rtl8192cd_priv *priv = (struct rtl8192cd_priv *)task_priv;
	if( priv->pshare->bRLShortened )
	{
		RTL_W16(RL, (priv->pshare->RLShort&SRL_Mask)<<SRL_SHIFT|(priv->pshare->RLLong&LRL_Mask)<<LRL_SHIFT);
		priv->pshare->bRLShortened = FALSE;
		DEBUG_WARN( "== Recover RetryLimit to 0x%04X ==\n", RTL_R16(RL) );
	}
}

// Chack STA leaving status; per interface. Added by Annie, 2010-08-10.
unsigned char NoLeavingSTA(struct rtl8192cd_priv *priv)
{
	unsigned char bStaAllOK = TRUE;
	struct list_head *phead, *plist;
	struct stat_info *pstat;

	phead = &priv->asoc_list;
	if (!netif_running(priv->dev) || list_empty(phead))
		return bStaAllOK;

	plist = phead->next;
	while (plist != phead)  {
		pstat = list_entry(plist, struct stat_info, asoc_list);
		if( pstat->tx_conti_fail_cnt != 0 ) {
			bStaAllOK = FALSE;
			break;
		}
		plist = plist->next;
	}

	return bStaAllOK;
}

// Chack STA leaving status for all active interface and recover retry limit register value. Added by Annie, 2010-08-10.
void LeavingSTA_RLCheck(struct rtl8192cd_priv *priv)
{
	unsigned char bIfAllOK = TRUE;
	static int AllOKTimes = 0;
#ifdef MBSSID
	int i;
#endif
	// Parameter
	const unsigned char	TFRL_RcvTime = 10;		// Time to recover Retry Limit (in second)

	if( !NoLeavingSTA(priv) )
		bIfAllOK = FALSE;

#ifdef UNIVERSAL_REPEATER
	if (IS_ROOT_INTERFACE(priv) && GET_VXD_PRIV(priv) ) {
		if( !NoLeavingSTA(GET_VXD_PRIV(priv)) )
			bIfAllOK = FALSE;
	}
#endif

#ifdef MBSSID
	if (IS_ROOT_INTERFACE(priv)) {
		if (GET_ROOT(priv)->pmib->miscEntry.vap_enable) {
			for (i=0; i<RTL8192CD_NUM_VWLAN; i++) {
				if (IS_DRV_OPEN(priv->pvap_priv[i])) {
					if( !NoLeavingSTA(priv->pvap_priv[i]) )
						bIfAllOK = FALSE;
				}
			}
		}
	}
#endif

	if( bIfAllOK ) {
		AllOKTimes ++;

		if( AllOKTimes >= TFRL_RcvTime )
			RetryLimitRecovery((unsigned long)priv);
	}
	else {
		AllOKTimes = 0;
	}
}
#endif


#endif

