//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "hwreg.h"
#include "drvGlobal.h"
#include "MsTypes.h"
#include "chip/bond.h"

#define GPIO_NONE               0       // Not GPIO pin (default)
#define GPIO_IN                 1       // GPI
#define GPIO_OUT_LOW            2       // GPO output low
#define GPIO_OUT_HIGH           3       // GPO output high

#define _MapBase_nonPM_MAYA 0xA0200000
#define _MapBase_PM_MAYA    0xA0000000

#define _MEMMAP_nonPM_  _RVM1(0x0000, 0x10, 0xFF)
#define _MEMMAP_nonBK17_  _RVM1(0x0000, 0x17, 0xFF)
#define _MEMMAP_nonBK11_  _RVM1(0x0000, 0x11, 0xFF)
#define _MEMMAP_PM_     _RVM1(0x0000, 0x00, 0xFF)

static U32 u32MapBase;

static void MDrv_PAD_WriteByte(U32 u32RegAddr, U8 u8Val)
{
    ((volatile U8*)(u32MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
}

static U8 MDrv_PAD_ReadByte(U32 u32RegAddr)
{
    return ((volatile U8*)(u32MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

static void MDrv_PAD_WriteByteMask(U32 u32Reg_addr, U8 u8Value, U8 u8Mask)
{
    U8 u8Val = MDrv_PAD_ReadByte(u32Reg_addr);

    u8Val = (u8Val & ~(u8Mask)) | (u8Value & u8Mask);
    MDrv_PAD_WriteByte(u32Reg_addr, u8Val);
}

//static code U8 padInitTbl[] =
volatile U8 code padInitTbl[]= //__attribute__((__section__ (".isp_info"))) =
{
    0x39, 0xB6, 0x5B, 0x53,     // magic code for ISP_Tool

    // programable device number
    // spi flash count
    2,
    //1 + (PIN_SPI_CZ1 != 0) + (PIN_SPI_CZ2 != 0) + (PIN_SPI_CZ3 != 0),
    0x00,                       // nor
    0x00,                       // nand
    0x00,                       // reserved
    0x00,                       // reserved
    0x00,                       // reserved

//---------------------------------------------------------------------
// GPIO Configuartion
//---------------------------------------------------------------------
    _MEMMAP_nonPM_,

    #if(PAD_TCON0_IS_GPIO != GPIO_NONE)
    #define PAD_TCON0_OEN (PAD_TCON0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_TCON0_OUT (PAD_TCON0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e5e, PAD_TCON0_OUT, BIT0),
    _RVM1(0x1e60, PAD_TCON0_OEN, BIT0),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4),   //reg[101e24]#5~#4 = 00b
    //reg_tconconfig[0] = 0b
    _RVM1(0x1ef2, 0, BIT0 ),   //reg[101ef2]#0 = 0b
    //reg_fastuartmode = 0b
    _RVM1(0x1e04, 0, BIT4 ),   //reg[101e04]#4 = 0b
    //reg_3d_lr_config[2:0] = 00b
    _RVM1(0x1e10, 0, BIT2|BIT1|BIT0 ),   //reg[101e10]#2~#0 = 000b
    //reg_8bit_ttl = 0b
    _RVM1(0x1ef8, 0, BIT1 ),   //reg[101ef8]#1 = 0b
    //reg_parallel_TS_in_Config[1:0] = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5~#4 = 000b
    //reg_serial_TS_in_Config[1:0] = 00b
    _RVM1(0x1ef7, 0, BIT1|BIT0 ),   //reg[101ef7]#1~#0 = 00b
    //reg_parallel_ts_out_config[2:0] = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2~#0 = 000b
    //reg_mspi1_config[1:0] = 00b
    _RVM1(0x1e07, 0, BIT7|BIT6 ),   //reg[101e07]#7~#6 = 0b
    #endif

    #if(PAD_TCON1_IS_GPIO != GPIO_NONE)
    #define PAD_TCON1_OEN (PAD_TCON1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TCON1_OUT (PAD_TCON1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e5e, PAD_TCON1_OUT, BIT1),
    _RVM1(0x1e60, PAD_TCON1_OEN, BIT1),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_tconconfig[1] = 0b
    _RVM1(0x1ef2, 0, BIT1 ),   //reg[101ef2]#1 = 0b
    //reg_fastuartmode = 0b
    _RVM1(0x1e04, 0, BIT4 ),   //reg[101e04]#4 = 0b
    //reg_8bit_ttl = 0b
    _RVM1(0x1ef8, 0, BIT1 ),   //reg[101ef8]#1 = 0b
    //reg_parallel_TS_in_Config[1:0]= 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5~#4 = 00b
    //reg_parallel_ts_out_config[2:0] = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT0 ),   //reg[101e1c]#2~#0 = 000b
    //reg_mspi0_config[1:0] = 00b
    _RVM1(0x1e07, 0, BIT5|BIT4 ),   //reg[101e07]#5~#4 = 0b
    //reg_mspi1_config[1:0] = 00b
    _RVM1(0x1e07, 0, BIT7|BIT6 ),   //reg[101e07]#7~#6 = 0b
    #endif

    #if(PAD_TCON2_IS_GPIO != GPIO_NONE)
    #define PAD_TCON2_OEN (PAD_TCON2_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_TCON2_OUT (PAD_TCON2_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e5e, PAD_TCON2_OUT, BIT2),
    _RVM1(0x1e60, PAD_TCON2_OEN, BIT2),
    //reg_tconconfig[2]= 0b
    _RVM1(0x1ef2, 0, BIT2 ),   //reg[101ef2]#2 = 0b
    //reg_8bit_ttl = 0b
    _RVM1(0x1ef8, 0, BIT1 ),   //reg[101ef8]#1 = 0b
    //reg_parallel_TS_in_Config[1:0] = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5~#4 = 00b
    //reg_parallel_ts_out_config[2:0] = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2~#0 = 000b
    //reg_fourthuartmode[1:0] = 00b
    _RVM1(0x1e04, 0, BIT7|BIT6 ),   //reg[101e04]#7~#6 = 0b
    //reg_mspi1_config[1:0] = 00b
    _RVM1(0x1e07, 0, BIT7|BIT6 ),   //reg[101e07]#7~#6 = 0b
    #endif

    #if(PAD_TCON3_IS_GPIO != GPIO_NONE)
    #define PAD_TCON3_OEN (PAD_TCON3_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_TCON3_OUT (PAD_TCON3_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e5e, PAD_TCON3_OUT, BIT3),
    _RVM1(0x1e60, PAD_TCON3_OEN, BIT3),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_tconconfig[3]= 0b
    _RVM1(0x1ef2, 0, BIT3 ),   //reg[101ef2]#3 = 0b
    //reg_8bit_ttl = 0b
    _RVM1(0x1ef8, 0, BIT1 ),   //reg[101ef8]#1 = 0b
    //reg_parallel_TS_in_Config[1:0] = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5~#4 = 00b
    //reg_parallel_ts_out_config[2:0] = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2~#0 = 000b
    //reg_mspi1_config[1:0] = 00b
    _RVM1(0x1e07, 0, BIT7|BIT6 ),   //reg[101e07]#7~#6 = 0b
    #endif

    #if(PAD_TCON4_IS_GPIO != GPIO_NONE)
    #define PAD_TCON4_OEN (PAD_TCON4_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_TCON4_OUT (PAD_TCON4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e5e, PAD_TCON4_OUT, BIT4),
    _RVM1(0x1e60, PAD_TCON4_OEN, BIT4),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_tconconfig[4]=0b
    _RVM1(0x1ef2, 0, BIT4 ),   //reg[101ef2]#4 =0b
    //reg_parallel_TS_in_Config[1:0] = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5~#4 = 00b
    //reg_serial_TS_in_Config[1:0] = 00b
    _RVM1(0x1ef7, 0, BIT1|BIT0 ),   //reg[101ef7]#1~#0 = 00b
    //reg_parallel_ts_out_config[2:0] =000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2~#0 = 000b
    //reg_vsync_like[2:0] = 000b
    _RVM1(0x1e0b, 0, BIT7|BIT6|BIT5 ),   //reg[101e0b]#7~5 = 000b
    //reg_mspi1_config[1:0] = 00b
    _RVM1(0x1e07, 0, BIT7|BIT6 ),   //reg[101e07]#7~#6 = 0b
    #endif

    #if(PAD_TCON5_IS_GPIO != GPIO_NONE)
    #define PAD_TCON5_OEN (PAD_TCON5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TCON5_OUT (PAD_TCON5_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e5e, PAD_TCON5_OUT, BIT5),
    _RVM1(0x1e60, PAD_TCON5_OEN, BIT5),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_tconconfig[5]= 0b
    _RVM1(0x1ef2, 0, BIT5 ),   //reg[101ef2]#5 = 0b
    //reg_parallel_TS_in_Config[1:0] = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5~#4 = 00b
    //reg_serial_TS_in_Config[1:0] = 00b
    _RVM1(0x1ef7, 0, BIT1|BIT0 ),   //reg[101ef7]#1~#0 = 00b
    //reg_parallel_ts_out_config[2:0] = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2~#0 = 000b
    //reg_vsync_like[2:0] = 000b
    _RVM1(0x1e0b, 0, BIT7|BIT6|BIT5 ),   //reg[101e0b]#7~5 = 000b
    #endif

    #if(PAD_TCON6_IS_GPIO != GPIO_NONE)
    #define PAD_TCON6_OEN (PAD_TCON6_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_TCON6_OUT (PAD_TCON6_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e5e, PAD_TCON6_OUT, BIT6),
    _RVM1(0x1e60, PAD_TCON6_OEN, BIT6),
    //reg_tconconfig[6]= 0b
    _RVM1(0x1ef2, 0, BIT6 ),   //reg[101ef2]#6 = 0b
    //reg_parallel_TS_in_Config[1:0] = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5~#4 = 00b
    //reg_serial_TS_in_Config[1:0] = 00b
    _RVM1(0x1ef7, 0, BIT1|BIT0 ),   //reg[101ef7]#1~#0 = 00b
    //reg_parallel_ts_out_config[2:0] = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2~#0 = 000b
    //reg_mspi0_config[1:0] = 00b
    _RVM1(0x1e07, 0, BIT5|BIT4 ),   //reg[101e07]#5~#4 = 0b
    #endif

    #if(PAD_TCON7_IS_GPIO != GPIO_NONE)
    #define PAD_TCON7_OEN (PAD_TCON7_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_TCON7_OUT (PAD_TCON7_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e5e, PAD_TCON7_OUT, BIT7),
    _RVM1(0x1e60, PAD_TCON7_OEN, BIT7),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_tconconfig[7]= 0b
    _RVM1(0x1ef2, 0, BIT7 ),   //reg[101ef2]#7 = 0b
    //reg_parallel_TS_in_Config[1:0] = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5~#4 = 00b
    //reg_serial_TS_in_Config[1:0] = 00b
    _RVM1(0x1ef7, 0, BIT1|BIT0 ),   //reg[101ef7]#1~#0 = 00b
    //reg_parallel_ts_out_config[2:0] = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2~#0 = 000b
    //reg_mspi0_config[1:0] = 00b
    _RVM1(0x1e07, 0, BIT5|BIT4 ),   //reg[101e07]#5~#4 = 0b
    #endif

    #if(PAD_TCON8_IS_GPIO != GPIO_NONE)
    #define PAD_TCON8_OEN (PAD_TCON8_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_TCON8_OUT (PAD_TCON8_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e5f, PAD_TCON8_OUT, BIT0),
    _RVM1(0x1e61, PAD_TCON8_OEN, BIT0),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_tconconfig[8]= 0b
    _RVM1(0x1ef3, 0, BIT0 ),   //reg[101ef3]#0 = 0b
    //reg_parallel_TS_in_Config[1:0] = 00b
    _RVM1(0x1ef6, 0, BIT5|BIT4 ),   //reg[101ef6]#5~#4 = 00b
    //reg_serial_TS_in_Config[1:0] = 00b
    _RVM1(0x1ef6, 0, BIT1|BIT0 ),   //reg[101ef6]#1~#0 = 00b
    //reg_parallel_ts_out_config[2:0] = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2~#0 = 000b
    //reg_mspi0_config[1:0] = 00b
    _RVM1(0x1e07, 0, BIT5|BIT4 ),   //reg[101e07]#5~#4 = 0b
    #endif

    #if(PAD_TCON9_IS_GPIO != GPIO_NONE)
    #define PAD_TCON9_OEN (PAD_TCON9_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TCON9_OUT (PAD_TCON9_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e5f, PAD_TCON9_OUT, BIT1),
    _RVM1(0x1e61, PAD_TCON9_OEN, BIT1),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_tconconfig[9]= 0b
    _RVM1(0x1ef3, 0, BIT1 ),   //reg[101ef3]#1 = 0b
    //reg_parallel_TS_in_Config[1:0] = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5~#4 = 00b
    //reg_serial_TS_in_Config[1:0] = 00b
    _RVM1(0x1ef7, 0, BIT1|BIT0 ),   //reg[101ef7]#1~#0 = 00b
    //reg_parallel_ts_out_config[2:0] = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2~#0 = 000b
    //reg_mspi0_config[1:0] = 00b
    _RVM1(0x1e07, 0, BIT5|BIT4 ),   //reg[101e07]#5~#4 = 0b
    #endif

    #if(PAD_TCON10_IS_GPIO != GPIO_NONE)
    #define PAD_TCON10_OEN (PAD_TCON10_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_TCON10_OUT (PAD_TCON10_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e5f, PAD_TCON10_OUT, BIT2),
    _RVM1(0x1e61, PAD_TCON10_OEN, BIT2),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_tconconfig[10]= 0b
    _RVM1(0x1ef3, 0, BIT2 ),   //reg[101ef3]#2 = 0b
    //reg_parallel_TS_in_Config[1:0] = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5~#4 = 00b
    //reg_serial_TS_in_Config[1:0] = 00b
    _RVM1(0x1ef6, 0, BIT0 ),   //reg[101ef6]19~#0 = 00b
    //reg_parallel_ts_out_config[2:0] = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2~#0 = 000b
    #endif

    #if(PAD_TCON11_IS_GPIO != GPIO_NONE)
    #define PAD_TCON11_OEN (PAD_TCON11_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_TCON11_OUT (PAD_TCON11_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e52, PAD_TCON11_OUT, BIT0),
    _RVM1(0x1e54, PAD_TCON11_OEN, BIT0),
    //reg_tconconfig[11]= 0b
    _RVM1(0x1ef3, 0, BIT3 ),   //reg[101ef3]#3 = 0b
    //reg_fourthuartmode = 0b
    _RVM1(0x1e04, 0, BIT6 ),   //reg[101e04]#6 = 0b
    //reg_parallel_ts_out_config[2:0] = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2~#0 = 000b
    #endif

    #if(PAD_PWM3_IS_GPIO != GPIO_NONE)
    #define PAD_PWM3_OEN (PAD_PWM3_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_PWM3_OUT (PAD_PWM3_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e85, PAD_PWM3_OUT, BIT7),
    _RVM1(0x1e87, PAD_PWM3_OEN, BIT7),
    //reg_PWM3Config = 0b
    _RVM1(0x1e40, 0, BIT7 ),   //reg[101e40]#7 = 0b
    #endif

    #if(PAD_PWM2_IS_GPIO != GPIO_NONE)
    #define PAD_PWM2_OEN (PAD_PWM2_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_PWM2_OUT (PAD_PWM2_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e85, PAD_PWM2_OUT, BIT6),
    _RVM1(0x1e87, PAD_PWM2_OEN, BIT6),
    //reg_PWM2Config = 0b
    _RVM1(0x1e40, 0, BIT6 ),   //reg[101e40]#6 = 0b
    #endif

    #if(PAD_PWM1_IS_GPIO != GPIO_NONE)
    #define PAD_PWM1_OEN (PAD_PWM1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_PWM1_OUT (PAD_PWM1_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e85, PAD_PWM1_OUT, BIT5),
    _RVM1(0x1e87, PAD_PWM1_OEN, BIT5),
    //reg_mhl_vbus_config[2:0] = 000b
    _RVM1(0x1ef9, 0, BIT7|BIT6|BIT5 ),   //reg[101ef9]#7~#5 = 000b
    //reg_PWM1Config = 0b
    _RVM1(0x1e40, 0, BIT5 ),   //reg[101e40]#5 = 0b
    //reg_mcu_jtag_mode = 00b
    _RVM1(0x1ede, 0, BIT3|BIT2 ),   //reg[101ede]#3~#2 = 00b
    //reg_dspejtagmode = 000b
    _RVM1(0x1e07, 0, BIT2|BIT1|BIT0 ),   //reg[101e07]#2~#0 = 000b
    //reg_chiptop_i2s_out_mclk_pwm1 = 0b
    //(reg_chiptop_dummy_3[10])
    _RVM1(0x1e1f, 0, BIT2 ),   //reg[101e1f]#2 = 0b
    #endif

    #if(PAD_PWM0_IS_GPIO != GPIO_NONE)
    #define PAD_PWM0_OEN (PAD_PWM0_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_PWM0_OUT (PAD_PWM0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e85, PAD_PWM0_OUT, BIT4),
    _RVM1(0x1e87, PAD_PWM0_OEN, BIT4),
    //reg_mhl_vbus_config[2:0] = 000b
    _RVM1(0x1ef9, 0, BIT7|BIT6|BIT5 ),   //reg[101ef9]#7~#5 = 000b
    //reg_PWM0Config = 0b
    _RVM1(0x1e40, 0, BIT4 ),   //reg[101e40]#4 = 0b
    #endif

    #if(PAD_LVSYNC_IS_GPIO != GPIO_NONE)
    #define PAD_LVSYNC_OEN (PAD_LVSYNC_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_LVSYNC_OUT (PAD_LVSYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e8c, PAD_LVSYNC_OUT, BIT0),
    _RVM1(0x1e84, PAD_LVSYNC_OEN, BIT0),
    //reg_gcr_outconf_ch0[1:0]
    _RVM1(0x1e00, 0, BIT1|BIT0 ),   //reg[111e00]#1~#0 = 00b
    //reg_ext_data_en[0]
    _RVM1(0x1e80, BIT0 , BIT0 ),   //reg[111e80]#0 = 1b
    //reg_gpo_sel[0]
    _RVM1(0x1e90, BIT0 , BIT0 ),   //reg[111e90]#0 = 1b
    #endif

    #if(PAD_LHSYNC_IS_GPIO != GPIO_NONE)
    #define PAD_LHSYNC_OEN (PAD_LHSYNC_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_LHSYNC_OUT (PAD_LHSYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e8c, PAD_LHSYNC_OUT, BIT1),
    _RVM1(0x1e84, PAD_LHSYNC_OEN, BIT1),
    //reg_gcr_outconf_ch0[1:0]
    _RVM1(0x1e00, 0, BIT1|BIT0 ),   //reg[111e00]#1~#0 = 00b
    //reg_ext_data_en[1]
    _RVM1(0x1e80, BIT1 , BIT1 ),   //reg[111e80]#1 = 1b
    //reg_gpo_sel[1]
    _RVM1(0x1e90, BIT1 , BIT1 ),   //reg[111e90]#1 = 1b
    #endif

    #if(PAD_LDE_IS_GPIO != GPIO_NONE)
    #define PAD_LDE_OEN (PAD_LDE_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_LDE_OUT (PAD_LDE_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e8c, PAD_LDE_OUT, BIT2),
    _RVM1(0x1e84, PAD_LDE_OEN, BIT2),
    //reg_gcr_outconf_ch1[1:0]
    _RVM1(0x1e00, 0, BIT3|BIT2 ),   //reg[111e00]#3~#2 = 00b
    //reg_ext_data_en[2]
    _RVM1(0x1e80, BIT2 , BIT2 ),   //reg[111e80]#2 = 1b
    //reg_gpo_sel[2]
    _RVM1(0x1e90, BIT2 , BIT2 ),   //reg[111e90]#2 = 1b
    #endif

    #if(PAD_LCK_IS_GPIO != GPIO_NONE)
    #define PAD_LCK_OEN (PAD_LCK_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_LCK_OUT (PAD_LCK_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e8c, PAD_LCK_OUT, BIT3),
    _RVM1(0x1e84, PAD_LCK_OEN, BIT3),
    //reg_gcr_outconf_ch1[1:0]
    _RVM1(0x1e00, 0, BIT3|BIT2 ),   //reg[111e00]#3~#2 = 00b
    //reg_ext_data_en[3]
    _RVM1(0x1e80, BIT3 , BIT3 ),   //reg[111e80]#3 = 1b
    //reg_gpo_sel[3]
    _RVM1(0x1e90, BIT3 , BIT3 ),   //reg[111e90]#3 = 1b
    #endif

    #if(PAD_R_ODD7_IS_GPIO != GPIO_NONE)
    #define PAD_R_ODD7_OEN (PAD_R_ODD7_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_R_ODD7_OUT (PAD_R_ODD7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e8c, PAD_R_ODD7_OUT, BIT4),
    _RVM1(0x1e84, PAD_R_ODD7_OEN, BIT4),
    //reg_gcr_outconf_ch2[1:0]
    _RVM1(0x1e00, 0, BIT5|BIT4 ),   //reg[111e00]#5~#4 = 00b
    //reg_ext_data_en[4]
    _RVM1(0x1e80, BIT4 , BIT4 ),   //reg[111e80]#4 = 1b
    //reg_gpo_sel[12]
    _RVM1(0x1e90, BIT4 , BIT4 ),   //reg[111e90]#4 = 1b
    #endif

    #if(PAD_R_ODD6_IS_GPIO != GPIO_NONE)
    #define PAD_R_ODD6_OEN (PAD_R_ODD6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_R_ODD6_OUT (PAD_R_ODD6_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e8c, PAD_R_ODD6_OUT, BIT5),
    _RVM1(0x1e84, PAD_R_ODD6_OEN, BIT5),
    //reg_gcr_outconf_ch2[1:0]
    _RVM1(0x1e00, 0, BIT5|BIT4 ),   //reg[111e00]#5~#4 = 00b
    //reg_ext_data_en[5]
    _RVM1(0x1e80, BIT5 , BIT5 ),   //reg[111e80]#5 = 1b
    //reg_gpo_sel[5]
    _RVM1(0x1e90, BIT5 , BIT5 ),   //reg[111e90]#5 = 1b
    #endif

    #if(PAD_R_ODD5_IS_GPIO != GPIO_NONE)
    #define PAD_R_ODD5_OEN (PAD_R_ODD5_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_R_ODD5_OUT (PAD_R_ODD5_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e8c, PAD_R_ODD5_OUT, BIT6),
    _RVM1(0x1e84, PAD_R_ODD5_OEN, BIT6),
    //reg_gcr_outconf_ch3[1:0]
    _RVM1(0x1e00, 0, BIT7|BIT6 ),   //reg[111e00]#7~#6 = 00b
    //reg_ext_data_en[6]
    _RVM1(0x1e80, BIT6 , BIT6 ),   //reg[111e80]#6 = 1b
    //reg_gpo_sel[6]
    _RVM1(0x1e90, BIT6 , BIT6 ),   //reg[111e90]#6 = 1b
    #endif

    #if(PAD_R_ODD4_IS_GPIO != GPIO_NONE)
    #define PAD_R_ODD4_OEN (PAD_R_ODD4_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_R_ODD4_OUT (PAD_R_ODD4_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e8c, PAD_R_ODD4_OUT, BIT7),
    _RVM1(0x1e84, PAD_R_ODD4_OEN, BIT7),
    //reg_gcr_outconf_ch3[1:0]
    _RVM1(0x1e00, 0, BIT7|BIT6 ),   //reg[111e00]#7~#6 = 00b
    //reg_ext_data_en[7]
    _RVM1(0x1e80, BIT7 , BIT7 ),   //reg[111e80]#7 = 1b
    //reg_gpo_sel[7]
    _RVM1(0x1e90, BIT7 , BIT7 ),   //reg[111e90]#7 = 1b
    #endif

    #if(PAD_R_ODD3_IS_GPIO != GPIO_NONE)
    #define PAD_R_ODD3_OEN (PAD_R_ODD3_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_R_ODD3_OUT (PAD_R_ODD3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e8d, PAD_R_ODD3_OUT, BIT0),
    _RVM1(0x1e85, PAD_R_ODD3_OEN, BIT0),
    //reg_gcr_outconf_ch4[1:0]
    _RVM1(0x1e01, 0, BIT1|BIT0 ),   //reg[111e01]#1~#0 = 00b
    //reg_ext_data_en[8]
    _RVM1(0x1e81, BIT0 , BIT0 ),   //reg[111e81]#0 = 1b
    //reg_gpo_sel[8]
    _RVM1(0x1e91, BIT0 , BIT0 ),   //reg[111e91]#0 = 1b
    #endif

    #if(PAD_R_ODD2_IS_GPIO != GPIO_NONE)
    #define PAD_R_ODD2_OEN (PAD_R_ODD2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_R_ODD2_OUT (PAD_R_ODD2_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e8d, PAD_R_ODD2_OUT, BIT1),
    _RVM1(0x1e85, PAD_R_ODD2_OEN, BIT1),
    //reg_gcr_outconf_ch4[1:0]
    _RVM1(0x1e01, 0, BIT1|BIT0 ),   //reg[111e01]#1~#0 = 00b
    //reg_ext_data_en[9]
    _RVM1(0x1e81, BIT1 , BIT1 ),   //reg[111e81]#1 = 1b
    //reg_gpo_sel[9]
    _RVM1(0x1e91, BIT1 , BIT1 ),   //reg[111e91]#1 = 1b
    #endif

    #if(PAD_R_ODD1_IS_GPIO != GPIO_NONE)
    #define PAD_R_ODD1_OEN (PAD_R_ODD1_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_R_ODD1_OUT (PAD_R_ODD1_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e8d, PAD_R_ODD1_OUT, BIT2),
    _RVM1(0x1e85, PAD_R_ODD1_OEN, BIT2),
    //reg_gcr_outconf_ch5[1:0]
    _RVM1(0x1e01, 0, BIT3|BIT2 ),   //reg[111e01]#3~#2 = 00b
    //reg_ext_data_en[10]
    _RVM1(0x1e81, BIT2 , BIT2 ),   //reg[111e81]#2 = 1b
    //reg_gpo_sel[10]
    _RVM1(0x1e91, BIT2 , BIT2 ),   //reg[111e91]#2 = 1b
    #endif

    #if(PAD_R_ODD0_IS_GPIO != GPIO_NONE)
    #define PAD_R_ODD0_OEN (PAD_R_ODD0_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_R_ODD0_OUT (PAD_R_ODD0_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e8d, PAD_R_ODD0_OUT, BIT3),
    _RVM1(0x1e85, PAD_R_ODD0_OEN, BIT3),
    //reg_gcr_outconf_ch5[1:0]
    _RVM1(0x1e01, 0, BIT3|BIT2 ),   //reg[111e01]#3~#2 = 00b
    //reg_ext_data_en[11]
    _RVM1(0x1e81, BIT3 , BIT3 ),   //reg[111e81]#3 = 1b
    //reg_gpo_sel[11]
    _RVM1(0x1e91, BIT3 , BIT3 ),   //reg[111e91]#3 = 1b
    #endif

    #if(PAD_G_ODD7_IS_GPIO != GPIO_NONE)
    #define PAD_G_ODD7_OEN (PAD_G_ODD7_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_G_ODD7_OUT (PAD_G_ODD7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e8d, PAD_G_ODD7_OUT, BIT4),
    _RVM1(0x1e85, PAD_G_ODD7_OEN, BIT4),
    //reg_gcr_outconf_ch6[1:0]
    _RVM1(0x1e01, 0, BIT5|BIT4 ),   //reg[111e01]#5~#4 = 00b
    //reg_ext_data_en[12]
    _RVM1(0x1e81, BIT4 , BIT4 ),   //reg[111e81]#4 = 1b
    //reg_gpo_sel[12]
    _RVM1(0x1e91, BIT4 , BIT4 ),   //reg[111e91]#4 = 1b
    #endif

    #if(PAD_G_ODD6_IS_GPIO != GPIO_NONE)
    #define PAD_G_ODD6_OEN (PAD_G_ODD6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_G_ODD6_OUT (PAD_G_ODD6_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e8d, PAD_G_ODD6_OUT, BIT5),
    _RVM1(0x1e85, PAD_G_ODD6_OEN, BIT5),
    //reg_gcr_outconf_ch6[1:0]
    _RVM1(0x1e01, 0, BIT5|BIT4 ),   //reg[111e01]#5~#4 = 00b
    //reg_ext_data_en[13]
    _RVM1(0x1e81, BIT5 , BIT5 ),   //reg[111e81]#5 = 1b
    //reg_gpo_sel[13]
    _RVM1(0x1e91, BIT5 , BIT5 ),   //reg[111e91]#5 = 1b
    #endif

    #if(PAD_G_ODD5_IS_GPIO != GPIO_NONE)
    #define PAD_G_ODD5_OEN (PAD_G_ODD5_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_G_ODD5_OUT (PAD_G_ODD5_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e8d, PAD_G_ODD5_OUT, BIT6),
    _RVM1(0x1e85, PAD_G_ODD5_OEN, BIT6),
    //reg_gcr_outconf_ch7[1:0]
    _RVM1(0x1e01, 0, BIT7|BIT6 ),   //reg[111e01]#7~#6 = 00b
    //reg_ext_data_en[14]
    _RVM1(0x1e81, BIT6 , BIT6 ),   //reg[111e81]#6 = 1b
    //reg_gpo_sel[14]
    _RVM1(0x1e91, BIT6 , BIT6 ),   //reg[111e91]#6 = 1b
    #endif

    #if(PAD_G_ODD4_IS_GPIO != GPIO_NONE)
    #define PAD_G_ODD4_OEN (PAD_G_ODD4_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_G_ODD4_OUT (PAD_G_ODD4_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e8d, PAD_G_ODD4_OUT, BIT7),
    _RVM1(0x1e85, PAD_G_ODD4_OEN, BIT7),
    //reg_gcr_outconf_ch7[1:0]
    _RVM1(0x1e01, 0, BIT7|BIT6 ),   //reg[111e01]#7~#6 = 00b
    //reg_ext_data_en[15]
    _RVM1(0x1e81, BIT7 , BIT7 ),   //reg[111e81]#7 = 1b
    //reg_gpo_sel[15]
    _RVM1(0x1e91, BIT7 , BIT7 ),   //reg[111e91]#7 = 1b
    #endif

    #if(PAD_G_ODD3_IS_GPIO != GPIO_NONE)
    #define PAD_G_ODD3_OEN (PAD_G_ODD3_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_G_ODD3_OUT (PAD_G_ODD3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e8e, PAD_G_ODD3_OUT, BIT0),
    _RVM1(0x1e86, PAD_G_ODD3_OEN, BIT0),
    //reg_gcr_outconf_ch8[1:0]
    _RVM1(0x1e02, 0, BIT1|BIT0 ),   //reg[111e02]#1~#0 = 00b
    //reg_ext_data_en[16]
    _RVM1(0x1e82, BIT0 , BIT0 ),   //reg[111e82]#0 = 1b
    //reg_gpo_sel[16]
    _RVM1(0x1e92, BIT0 , BIT0 ),   //reg[111e92]#0 = 1b
    #endif

    #if(PAD_G_ODD2_IS_GPIO != GPIO_NONE)
    #define PAD_G_ODD2_OEN (PAD_G_ODD2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_G_ODD2_OUT (PAD_G_ODD2_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e8e, PAD_G_ODD2_OUT, BIT1),
    _RVM1(0x1e86, PAD_G_ODD2_OEN, BIT1),
    //reg_gcr_outconf_ch8[1:0]
    _RVM1(0x1e02, 0, BIT1|BIT0 ),   //reg[111e02]#1~#0 = 00b
    //reg_ext_data_en[17]
    _RVM1(0x1e82, BIT1 , BIT1 ),   //reg[111e82]#1 = 1b
    //reg_gpo_sel[17]
    _RVM1(0x1e92, BIT1 , BIT1 ),   //reg[111e92]#1 = 1b
    #endif

    #if(PAD_G_ODD1_IS_GPIO != GPIO_NONE)
    #define PAD_G_ODD1_OEN (PAD_G_ODD1_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_G_ODD1_OUT (PAD_G_ODD1_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e8e, PAD_G_ODD1_OUT, BIT2),
    _RVM1(0x1e86, PAD_G_ODD1_OEN, BIT2),
    //reg_gcr_outconf_ch9[1:0]
    _RVM1(0x1e02, 0, BIT3|BIT2 ),   //reg[111e02]#3~#2 = 00b
    //reg_ext_data_en[18]
    _RVM1(0x1e82, BIT2 , BIT2 ),   //reg[111e82]#2 = 1b
    //reg_gpo_sel[18]
    _RVM1(0x1e92, BIT2 , BIT2 ),   //reg[111e92]#2 = 1b
    #endif

    #if(PAD_G_ODD0_IS_GPIO != GPIO_NONE)
    #define PAD_G_ODD0_OEN (PAD_G_ODD0_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_G_ODD0_OUT (PAD_G_ODD0_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e8e, PAD_G_ODD0_OUT, BIT3),
    _RVM1(0x1e86, PAD_G_ODD0_OEN, BIT3),
    //reg_gcr_outconf_ch9[1:0]
    _RVM1(0x1e02, 0, BIT3|BIT2 ),   //reg[111e02]#3~#2 = 00b
    //reg_ext_data_en[19]
    _RVM1(0x1e82, BIT3 , BIT3 ),   //reg[111e82]#3 = 1b
    //reg_gpo_sel[19]
    _RVM1(0x1e92, BIT3 , BIT3 ),   //reg[111e92]#3 = 1b
    #endif

    #if(PAD_B_ODD7_IS_GPIO != GPIO_NONE)
    #define PAD_B_ODD7_OEN (PAD_B_ODD7_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_B_ODD7_OUT (PAD_B_ODD7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e8e, PAD_B_ODD7_OUT, BIT4),
    _RVM1(0x1e86, PAD_B_ODD7_OEN, BIT4),
    //reg_gcr_outconf_ch10[1:0]
    _RVM1(0x1e02, 0, BIT5|BIT4 ),   //reg[111e02]#5~#4 = 00b
    //reg_ext_data_en[20]
    _RVM1(0x1e82, BIT4 , BIT4 ),   //reg[111e82]#4 = 1b
    //reg_gpo_sel[20]
    _RVM1(0x1e92, BIT4 , BIT4 ),   //reg[111e92]#4 = 1b
    #endif

    #if(PAD_B_ODD6_IS_GPIO != GPIO_NONE)
    #define PAD_B_ODD6_OEN (PAD_B_ODD6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_B_ODD6_OUT (PAD_B_ODD6_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e8e, PAD_B_ODD6_OUT, BIT5),
    _RVM1(0x1e86, PAD_B_ODD6_OEN, BIT5),
    //reg_gcr_outconf_ch10[1:0]
    _RVM1(0x1e02, 0, BIT5|BIT4 ),   //reg[111e02]#5~#4 = 00b
    //reg_ext_data_en[21]
    _RVM1(0x1e82, BIT5 , BIT5 ),   //reg[111e82]#5 = 1b
    //reg_gpo_sel[21]
    _RVM1(0x1e92, BIT5 , BIT5 ),   //reg[111e92]#5 = 1b
    #endif

    #if(PAD_B_ODD5_IS_GPIO != GPIO_NONE)
    #define PAD_B_ODD5_OEN (PAD_B_ODD5_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_B_ODD5_OUT (PAD_B_ODD5_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e8e, PAD_B_ODD5_OUT, BIT6),
    _RVM1(0x1e86, PAD_B_ODD5_OEN, BIT6),
    //reg_gcr_outconf_ch11[1:0]
    _RVM1(0x1e02, 0, BIT7|BIT6 ),   //reg[111e02]#7~#6 = 00b
    //reg_ext_data_en[22]
    _RVM1(0x1e82, BIT6 , BIT6 ),   //reg[111e82]#6 = 1b
    //reg_gpo_sel[22]
    _RVM1(0x1e92, BIT6 , BIT6 ),   //reg[111e92]#6 = 1b
    #endif

    #if(PAD_B_ODD4_IS_GPIO != GPIO_NONE)
    #define PAD_B_ODD4_OEN (PAD_B_ODD4_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_B_ODD4_OUT (PAD_B_ODD4_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e8e, PAD_B_ODD4_OUT, BIT7),
    _RVM1(0x1e86, PAD_B_ODD4_OEN, BIT7),
    //reg_gcr_outconf_ch11[1:0]
    _RVM1(0x1e02, 0, BIT7|BIT6 ),   //reg[111e02]#7~#6 = 00b
    //reg_ext_data_en[23]
    _RVM1(0x1e82, BIT7 , BIT7 ),   //reg[111e82]#7 = 1b
    //reg_gpo_sel[23]
    _RVM1(0x1e92, BIT7 , BIT7 ),   //reg[111e92]#7 = 1b
    #endif

    #if(PAD_B_ODD3_IS_GPIO != GPIO_NONE)
    #define PAD_B_ODD3_OEN (PAD_B_ODD3_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_B_ODD3_OUT (PAD_B_ODD3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e8f, PAD_B_ODD3_OUT, BIT0),
    _RVM1(0x1e87, PAD_B_ODD3_OEN, BIT0),
    //reg_gcr_outconf_ch12[1:0]
    _RVM1(0x1e03, 0, BIT1|BIT0 ),   //reg[111e03]#1~#0 = 00b
    //reg_ext_data_en[24]
    _RVM1(0x1e83, BIT0 , BIT0 ),   //reg[111e83]#0 = 1b
    //reg_gpo_sel[24]
    _RVM1(0x1e93, BIT0 , BIT0 ),   //reg[111e93]#0 = 1b
    #endif

    #if(PAD_B_ODD2_IS_GPIO != GPIO_NONE)
    #define PAD_B_ODD2_OEN (PAD_B_ODD2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_B_ODD2_OUT (PAD_B_ODD2_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e8f, PAD_B_ODD2_OUT, BIT1),
    _RVM1(0x1e87, PAD_B_ODD2_OEN, BIT1),
    //reg_gcr_outconf_ch12[1:0]
    _RVM1(0x1e03, 0, BIT1|BIT0 ),   //reg[111e03]#1~#0 = 00b
    //reg_ext_data_en[25]
    _RVM1(0x1e83, BIT1 , BIT1 ),   //reg[111e83]#1 = 1b
    //reg_gpo_sel[25]
    _RVM1(0x1e93, BIT1 , BIT1 ),   //reg[111e93]#1 = 1b
    #endif

    #if(PAD_B_ODD1_IS_GPIO != GPIO_NONE)
    #define PAD_B_ODD1_OEN (PAD_B_ODD1_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_B_ODD1_OUT (PAD_B_ODD1_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e8f, PAD_B_ODD1_OUT, BIT2),
    _RVM1(0x1e87, PAD_B_ODD1_OEN, BIT2),
    //reg_gcr_outconf_ch13[1:0]
    _RVM1(0x1e03, 0, BIT3|BIT2 ),   //reg[111e03]#3~#2 = 00b
    //reg_ext_data_en[26]
    _RVM1(0x1e83, BIT2 , BIT2 ),   //reg[111e83]#2 = 1b
    //reg_gpo_sel[26]
    _RVM1(0x1e93, BIT2 , BIT2 ),   //reg[111e93]#2 = 1b
    #endif

    #if(PAD_B_ODD0_IS_GPIO != GPIO_NONE)
    #define PAD_B_ODD0_OEN (PAD_B_ODD0_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_B_ODD0_OUT (PAD_B_ODD0_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e8f, PAD_B_ODD0_OUT, BIT3),
    _RVM1(0x1e87, PAD_B_ODD0_OEN, BIT3),
    //reg_gcr_outconf_ch13[1:0]
    _RVM1(0x1e03, 0, BIT3|BIT2 ),   //reg[111e03]#3~#2 = 00b
    //reg_ext_data_en[27]
    _RVM1(0x1e83, BIT3 , BIT3 ),   //reg[111e83]#3 = 1b
    //reg_gpo_sel[27]
    _RVM1(0x1e93, BIT3 , BIT3 ),   //reg[111e93]#3 = 1b
    #endif

    #if(PAD_GPIO0_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO0_OEN (PAD_GPIO0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO0_OUT (PAD_GPIO0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e7a, PAD_GPIO0_OUT, BIT0),
    _RVM1(0x1e7e, PAD_GPIO0_OEN, BIT0),
    //reg_seconduartmode = 00b
    _RVM1(0x1e05, 0, BIT1|BIT0 ),   //reg[101e05]#1~#0 = 00b
    //reg_3d_lr_config[2:0] = 000b
    _RVM1(0x1e10, 0, BIT2|BIT1|BIT0 ),   //reg[101e10]#2~#0 = 000b
    //reg_6bit_ttl = 0b
    _RVM1(0x1ef8, 0, BIT0 ),   //reg[101ef8]#0 = 0b
    //reg_usbdrvvbusconfig_eq_1 = 0b
    _RVM1(0x1e04, 0, BIT1|BIT0 ),   //reg[101e04]#1~#0 = 00b
    //reg_mhl_vbus_config[2:0] = 000b
    _RVM1(0x1ef9, 0, BIT7|BIT6|BIT5 ),   //reg[101ef9]#7~#5 = 000b
    //reg_mhl_debug_en = 0b
    _RVM1(0x1ef9, 0, BIT4 ),   //reg[101ef9]#4 = 0b
    //reg_ddcrmode[3:0] = 0000b
    _RVM1(0x1ef6, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef6]#3 ~#0 = 0000b
    //reg_mcu_jtag_mode = 00b
    _RVM1(0x1ede, 0, BIT3|BIT2 ),   //reg[101ede]#3 ~#2 = 0b
    //reg_dspejtagmode = 000b
    _RVM1(0x1e07, 0, BIT2|BIT1|BIT0 ),   //reg[101e07]#2~#0 = 000b
    #endif

    #if(PAD_GPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO1_OEN (PAD_GPIO1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO1_OUT (PAD_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e7a, PAD_GPIO1_OUT, BIT1),
    _RVM1(0x1e7e, PAD_GPIO1_OEN, BIT1),
    //reg_seconduartmode = 00b
    _RVM1(0x1e05, 0, BIT1|BIT0 ),   //reg[101e05]#1~#0 = 00b
    //reg_6bit_ttl = 0b
    _RVM1(0x1ef8, 0, BIT0 ),   //reg[101ef8]#0 = 0b
    //reg_usbdrvvbusconfig_eq_1 = 0b
    _RVM1(0x1e04, 0, BIT1|BIT0 ),   //reg[101e04]#1~#0 = 00b
    //reg_ddcrmode[3:0] = 0000b
    _RVM1(0x1ef6, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef6]#3 ~#0 = 0000b
    //reg_mcu_jtag_mode = 00b
    _RVM1(0x1ede, 0, BIT3|BIT2 ),   //reg[101ede]#3 ~#2 = 00b
    //reg_dspejtagmode = 000b
    _RVM1(0x1e07, 0, BIT2|BIT1|BIT0 ),   //reg[101e07]#2~#0 = 000b
    #endif

    #if(PAD_GPIO2_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO2_OEN (PAD_GPIO2_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_GPIO2_OUT (PAD_GPIO2_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e7a, PAD_GPIO2_OUT, BIT2),
    _RVM1(0x1e7e, PAD_GPIO2_OEN, BIT2),
    //reg_mhl_vbus_config[2:0] = 000b
    _RVM1(0x1ef9, 0, BIT7|BIT6|BIT5 ),   //reg[101ef9]#7~#5 = 000b
    //reg_ddcrmode[3:0] = 0000b
    _RVM1(0x1ef6, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef6]#3 ~#0 = 0000b
    #endif

    #if(PAD_GPIO3_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO3_OEN (PAD_GPIO3_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_GPIO3_OUT (PAD_GPIO3_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e7a, PAD_GPIO3_OUT, BIT3),
    _RVM1(0x1e7e, PAD_GPIO3_OEN, BIT3),
    //reg_ddcrmode[3:0] = 0000b
    _RVM1(0x1ef6, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef6]#3 ~#0 = 0000b
    #endif

    #if(PAD_GPIO4_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO4_OEN (PAD_GPIO4_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_GPIO4_OUT (PAD_GPIO4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e7a, PAD_GPIO4_OUT, BIT4),
    _RVM1(0x1e7e, PAD_GPIO4_OEN, BIT4),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_i2soutconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT6|BIT5 ),   //reg[101eae]#6 ~#5 = 00b
    //reg_chiptop_i2s_out_mclk_pwm1 = 0b
    _RVM1(0x1e3f, 0, BIT2 ),   //reg[101e1f]#2 = 0b
    //reg_chiptop_i2s_out_mclk_gpio10 = 0b
    _RVM1(0x1e3f, 0, BIT3 ),   //reg[101e1f]#3 = 0b
    //reg_chiptop_i2s_out_mclk_int = 0b
    _RVM1(0x1e3f, 0, BIT4 ),   //reg[101e1f]#4 = 0b
    #endif

    #if(PAD_GPIO5_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO5_OEN (PAD_GPIO5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO5_OUT (PAD_GPIO5_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e7a, PAD_GPIO5_OUT, BIT5),
    _RVM1(0x1e7e, PAD_GPIO5_OEN, BIT5),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_fourthuartmode[1:0] = 00b
    _RVM1(0x1e04, 0, BIT7|BIT6 ),   //reg[101e04]#7~#6 = 00b
    //reg_spdifoutconfig = 0b
    _RVM1(0x1eaf, 0, BIT1 ),   //reg[101eaf]#1 = 0b
    #endif

    #if(PAD_GPIO6_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO6_OEN (PAD_GPIO6_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_GPIO6_OUT (PAD_GPIO6_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e7a, PAD_GPIO6_OUT, BIT6),
    _RVM1(0x1e7e, PAD_GPIO6_OEN, BIT6),
    //reg_i2sinconfig = 00b
    _RVM1(0x1eae, 0, BIT4|BIT3 ),   //reg[101eae]#4~#3 = 0b
    //reg_i2soutconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT6|BIT5 ),   //reg[101eae]#6 ~#5 = 00b
    //reg_PWM5Config = 00b
    _RVM1(0x1e41, 0, BIT3|BIT2 ),   //reg[101e41]#3~#2 = 0b
    //reg_ddcrmode[3:0] = 0000b
    _RVM1(0x1ef6, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef6]#3 ~#0 = 0000b
    //reg_chiptop_i2s_out_mclk_pwm1 = 0b
    _RVM1(0x1e3f, 0, BIT2 ),   //reg[101e1f]#2 = 0b
    //reg_chiptop_i2s_out_mclk_gpio10 = 0b
    _RVM1(0x1e3f, 0, BIT3 ),   //reg[101e1f]#3 = 0b
    //reg_chiptop_i2s_out_mclk_int = 0b
    _RVM1(0x1e3f, 0, BIT4 ),   //reg[101e1f]#4 = 0b
    //reg_serial_TS_in_Config = 0b
    _RVM1(0x1ef7, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef7]#3~0 = 0b
    //reg_mspi1_config[1:0] = 00b
    _RVM1(0x1e07, 0, BIT7|BIT6 ),   //reg[101e07]#7~#6 = 0b
    #endif

    #if(PAD_GPIO7_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO7_OEN (PAD_GPIO7_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_GPIO7_OUT (PAD_GPIO7_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e7a, PAD_GPIO7_OUT, BIT7),
    _RVM1(0x1e7e, PAD_GPIO7_OEN, BIT7),
    //reg_i2soutconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT6|BIT5 ),   //reg[101eae]#6 ~#5 = 00b
    //reg_spdifinconfig = 0b
    _RVM1(0x1eae, 0, BIT7 ),   //reg[101eae]#7 = 0b
    //reg_PWM4Config = 00b
    _RVM1(0x1e41, 0, BIT1|BIT0 ),   //reg[101e41]#1~#0 = 00b
    //reg_ddcrmode[3:0] = 0000b
    _RVM1(0x1ef6, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef6]#3 ~#0 = 0000b
    //reg_chiptop_i2s_out_mclk_pwm1 = 0b
    _RVM1(0x1e3f, 0, BIT2 ),   //reg[101e1f]#2 = 0b
    //reg_chiptop_i2s_out_mclk_gpio10 = 0b
    _RVM1(0x1e3f, 0, BIT3 ),   //reg[101e1f]#3 = 0b
    //reg_chiptop_i2s_out_mclk_int = 0b
    _RVM1(0x1e3f, 0, BIT4 ),   //reg[101e1f]#4 = 0b
    //reg_serial_TS_in_Config = 0b
    _RVM1(0x1ef7, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef7]#3~0 = 0b
    //reg_mspi1_config[1:0] = 00b
    _RVM1(0x1e07, 0, BIT7|BIT6 ),   //reg[101e07]#7~#6 = 0b
    #endif

    #if(PAD_GPIO8_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO8_OEN (PAD_GPIO8_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO8_OUT (PAD_GPIO8_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e7b, PAD_GPIO8_OUT, BIT0),
    _RVM1(0x1e7f, PAD_GPIO8_OEN, BIT0),
    //reg_thirduartmode = 00b
    _RVM1(0x1e05, 0, BIT3|BIT2 ),   //reg[101e05]#3~#2 = 00b
    //reg_i2sinconfig = 00b
    _RVM1(0x1eae, 0, BIT4|BIT3 ),   //reg[101eae]#4~#3 = 0b
    //reg_i2soutconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT6|BIT5 ),   //reg[101eae]#6 ~#5 = 00b
    //reg_chiptop_i2s_out_mclk_pwm1 = 0b
    _RVM1(0x1e3f, 0, BIT2 ),   //reg[101e1f]#2 = 0b
    //reg_chiptop_i2s_out_mclk_gpio10 = 0b
    _RVM1(0x1e3f, 0, BIT3 ),   //reg[101e1f]#3 = 0b
    //reg_chiptop_i2s_out_mclk_int = 0b
    _RVM1(0x1e3f, 0, BIT4 ),   //reg[101e1f]#4 = 0b
    //reg_serial_TS_in_Config = 0b
    _RVM1(0x1ef7, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef7]#3~0 = 0b
    //reg_mspi1_config[1:0] = 00b
    _RVM1(0x1e07, 0, BIT7|BIT6 ),   //reg[101e07]#7~#6 = 0b
    #endif

    #if(PAD_GPIO9_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO9_OEN (PAD_GPIO9_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO9_OUT (PAD_GPIO9_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e7b, PAD_GPIO9_OUT, BIT1),
    _RVM1(0x1e7f, PAD_GPIO9_OEN, BIT1),
    //reg_thirduartmode = 00b
    _RVM1(0x1e05, 0, BIT3|BIT2 ),   //reg[101e05]#3~#2 = 00b
    //reg_i2sinconfig = 00b
    _RVM1(0x1eae, 0, BIT4|BIT3 ),   //reg[101eae]#4~#3 = 0b
    //reg_i2soutconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT6|BIT5 ),   //reg[101eae]#6 ~#5 = 00b
    //reg_parallel_TS_in_Config = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5 ~#4 = 00b
    //reg_parallel_ts_out_config = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2 ~#0 = 000b
    //reg_chiptop_i2s_out_mclk_pwm1 = 0b
    _RVM1(0x1e3f, 0, BIT2 ),   //reg[101e1f]#2 = 0b
    //reg_chiptop_i2s_out_mclk_gpio10 = 0b
    _RVM1(0x1e3f, 0, BIT3 ),   //reg[101e1f]#3 = 0b
    //reg_chiptop_i2s_out_mclk_int = 0b
    _RVM1(0x1e3f, 0, BIT4 ),   //reg[101e1f]#4 = 0b
    //reg_serial_TS_in_Config = 0b
    _RVM1(0x1ef7, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef7]#3~0 = 0b
    //reg_mspi1_config[1:0] = 00b
    _RVM1(0x1e07, 0, BIT7|BIT6 ),   //reg[101e07]#7~#6 = 0b
    #endif

    #if(PAD_GPIO10_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO10_OEN (PAD_GPIO10_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_GPIO10_OUT (PAD_GPIO10_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e7b, PAD_GPIO10_OUT, BIT2),
    _RVM1(0x1e7f, PAD_GPIO10_OEN, BIT2),
    //reg_thirduartmode[1:0] = 00b
    _RVM1(0x1e05, 0, BIT3|BIT2 ),   //reg[101e05]#3~2 = 00b
    //reg_chiptop_i2s_out_mclk_gpio10 = 0b
    _RVM1(0x1e1f, 0, BIT3 ),   //reg[101e1f]#3 = 0b
    //reg_chiptop_spdif_mux_gpio10 = 0b
    //(reg_chiptop_dummy_3[14])
    _RVM1(0x1e1f, 0, BIT6 ),   //reg[101e1f]#6 = 0b
    //reg_ddcrmode[3:0] = 0000b
    _RVM1(0x1ef6, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef6]#3 ~#0 = 0000b
    //reg_parallel_TS_in_Config = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5 ~#4 = 00b
    //reg_parallel_ts_out_config = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2 ~#0 = 000b
    #endif

    #if(PAD_GPIO11_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO11_OEN (PAD_GPIO11_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_GPIO11_OUT (PAD_GPIO11_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e7b, PAD_GPIO11_OUT, BIT3),
    _RVM1(0x1e7f, PAD_GPIO11_OEN, BIT3),
    //reg_thirduartmode[1:0] = 00b
    _RVM1(0x1e05, 0, BIT3|BIT2 ),   //reg[101e05]#3~2 = 00b
    //reg_ddcrmode[3:0] = 0000b
    _RVM1(0x1ef6, 0, BIT3|BIT1|BIT0 ),   //reg[101ef6]#3 ~#0 = 0000b
    //reg_parallel_TS_in_Config = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5 ~#4 = 00b
    //reg_parallel_ts_out_config = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2 ~#0 = 000b
    #endif

    #if(PAD_GPIO12_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO12_OEN (PAD_GPIO12_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_GPIO12_OUT (PAD_GPIO12_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e7b, PAD_GPIO12_OUT, BIT4),
    _RVM1(0x1e7f, PAD_GPIO12_OEN, BIT4),
    //reg_parallel_TS_in_Config = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5 ~#4 = 00b
    //reg_parallel_ts_out_config = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2 ~#0 = 000b
    #endif

    #if(PAD_GPIO13_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO13_OEN (PAD_GPIO13_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO13_OUT (PAD_GPIO13_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e7b, PAD_GPIO13_OUT, BIT5),
    _RVM1(0x1e7f, PAD_GPIO13_OEN, BIT5),
    //reg_parallel_TS_in_Config = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5 ~#4 = 00b
    //reg_parallel_ts_out_config = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2 ~#0 = 000b
    #endif

    #if(PAD_GPIO14_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO14_OEN (PAD_GPIO14_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_GPIO14_OUT (PAD_GPIO14_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e7b, PAD_GPIO14_OUT, BIT6),
    _RVM1(0x1e7f, PAD_GPIO14_OEN, BIT6),
    //reg_parallel_TS_in_Config = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5 ~#4 = 00b
    //reg_parallel_ts_out_config = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2 ~#0 = 000b
    #endif

    #if(PAD_GPIO15_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO15_OEN (PAD_GPIO15_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_GPIO15_OUT (PAD_GPIO15_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e7b, PAD_GPIO15_OUT, BIT7),
    _RVM1(0x1e7f, PAD_GPIO15_OEN, BIT7),
    //reg_parallel_TS_in_Config = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5 ~#4 = 00b
    //reg_parallel_ts_out_config = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2 ~#0 = 000b
    #endif

    #if(PAD_GPIO16_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO16_OEN (PAD_GPIO16_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO16_OUT (PAD_GPIO16_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e7c, PAD_GPIO16_OUT, BIT0),
    _RVM1(0x1e80, PAD_GPIO16_OEN, BIT0),
    //reg_parallel_TS_in_Config = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5 ~#4 = 00b
    //reg_parallel_ts_out_config = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2 ~#0 = 000b
    #endif

    #if(PAD_GPIO17_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO17_OEN (PAD_GPIO17_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO17_OUT (PAD_GPIO17_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e7c, PAD_GPIO17_OUT, BIT1),
    _RVM1(0x1e80, PAD_GPIO17_OEN, BIT1),
    //reg_parallel_TS_in_Config = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5 ~#4 = 00b
    //reg_parallel_ts_out_config = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2 ~#0 = 000b
    #endif

    #if(PAD_GPIO18_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO18_OEN (PAD_GPIO18_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_GPIO18_OUT (PAD_GPIO18_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e7c, PAD_GPIO18_OUT, BIT2),
    _RVM1(0x1e80, PAD_GPIO18_OEN, BIT2),
    //reg_parallel_TS_in_Config = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5 ~#4 = 00b
    //reg_parallel_ts_out_config = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2 ~#0 = 000b
    #endif

    #if(PAD_GPIO19_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO19_OEN (PAD_GPIO19_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_GPIO19_OUT (PAD_GPIO19_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e7c, PAD_GPIO19_OUT, BIT3),
    _RVM1(0x1e80, PAD_GPIO19_OEN, BIT3),
    //reg_parallel_TS_in_Config = 00b
    _RVM1(0x1ef7, 0, BIT5|BIT4 ),   //reg[101ef7]#5 ~#4 = 00b
    //reg_parallel_ts_out_config = 000b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ),   //reg[101e1c]#2 ~#0 = 000b
    #endif

    #if(PAD_INT_IS_GPIO != GPIO_NONE)
    #define PAD_INT_OEN (PAD_INT_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_INT_OUT (PAD_INT_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e45, PAD_INT_OUT, BIT2),
    _RVM1(0x1e49, PAD_INT_OEN, BIT2),
    //reg_int_gpio = 0b
    _RVM1(0x1e4f, BIT2 , BIT2 ),   //reg[101e4f]#2 = 1b
    //reg_chiptop_i2s_out_mclk_int = 0b
    _RVM1(0x1e3f, 0, BIT4 ),   //reg[101e3f]#4 = 0b
    //reg_chiptop_spdif_mux_int = 0b
    //(reg_chiptop_dummy_3[15])
    _RVM1(0x1e3f, 0, BIT7 ),   //reg[101e3f]#7 = 0b
    #endif

    _MEMMAP_PM_,

    #if(PAD_PMGPIO_IS_GPIO != GPIO_NONE)
    #define PAD_PMGPIO_OEN (PAD_PMGPIO_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PMGPIO_OUT (PAD_PMGPIO_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0e3a, PAD_PMGPIO_OUT, BIT1),
    _RVM1(0x0e3c, PAD_PMGPIO_OEN, BIT1),
    _MEMMAP_nonPM_,
    //reg_3d_lr_config[2:0] = 000b
    _RVM1(0x1e10, 0, BIT2|BIT1|BIT0 ),   //reg[101e10]#2~#0 = 000b
    #endif

    _MEMMAP_PM_,

    #if(PAD_GPIO_PM1_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM1_OEN (PAD_GPIO_PM1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO_PM1_OUT (PAD_GPIO_PM1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x0e20, PAD_GPIO_PM1_OUT, BIT1),
    _RVM1(0x0e1e, PAD_GPIO_PM1_OEN, BIT1),
    #endif

    #if(PAD_GPIO_PM4_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM4_OEN (PAD_GPIO_PM4_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_GPIO_PM4_OUT (PAD_GPIO_PM4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x0e20, PAD_GPIO_PM4_OUT, BIT4),
    _RVM1(0x0e1e, PAD_GPIO_PM4_OEN, BIT4),
    #endif

    #if(PAD_GPIO_PM5_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM5_OEN (PAD_GPIO_PM5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO_PM5_OUT (PAD_GPIO_PM5_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x0e20, PAD_GPIO_PM5_OUT, BIT5),
    _RVM1(0x0e1e, PAD_GPIO_PM5_OEN, BIT5),
    #endif

    #if(PAD_GPIO_PM6_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO_PM6_OEN (PAD_GPIO_PM6_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_GPIO_PM6_OUT (PAD_GPIO_PM6_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x0e20, PAD_GPIO_PM6_OUT, BIT6),
    _RVM1(0x0e1e, PAD_GPIO_PM6_OEN, BIT6),
    #endif

    #if(PAD_CEC_IS_GPIO != GPIO_NONE)
    #define PAD_CEC_OEN (PAD_CEC_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_CEC_OUT (PAD_CEC_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x0E3a, PAD_CEC_OUT, BIT2),
    _RVM1(0x0E3c, PAD_CEC_OEN, BIT2),
    //reg_pmcec_en[6] = 0b
    _RVM1(0x3401, 0, BIT6 ),   //reg[3401]#6 = 0b
    //reg_cec_is_gpio = 1b
    _RVM1(0x0E38, BIT6 , BIT6 ),   //reg[0E38]#6 = 1b
    #endif

    #if(PAD_SAR3_IS_GPIO != GPIO_NONE)
    #define PAD_SAR3_OEN (PAD_SAR3_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_SAR3_OUT (PAD_SAR3_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1424, PAD_SAR3_OUT, BIT3),
    _RVM1(0x1423, PAD_SAR3_OEN, BIT3),
    _RVM1(0x1422, 0, BIT3 ),   //reg[1422] #3 = 0b
    #endif

    #if(PAD_SAR2_IS_GPIO != GPIO_NONE)
    #define PAD_SAR2_OEN (PAD_SAR2_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_SAR2_OUT (PAD_SAR2_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1424, PAD_SAR2_OUT, BIT2),
    _RVM1(0x1423, PAD_SAR2_OEN, BIT2),
    _RVM1(0x1422, 0, BIT2 ),   //reg[1422] #2 = 0b
    #endif

    #if(PAD_SAR1_IS_GPIO != GPIO_NONE)
    #define PAD_SAR1_OEN (PAD_SAR1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_SAR1_OUT (PAD_SAR1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1424, PAD_SAR1_OUT, BIT1),
    _RVM1(0x1423, PAD_SAR1_OEN, BIT1),
    _RVM1(0x1422, 0, BIT1 ),   //reg[1422] #1 = 0b
    #endif

    #if(PAD_SAR0_IS_GPIO != GPIO_NONE)
    #define PAD_SAR0_OEN (PAD_SAR0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_SAR0_OUT (PAD_SAR0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1424, PAD_SAR0_OUT, BIT0),
    _RVM1(0x1423, PAD_SAR0_OEN, BIT0),
    _RVM1(0x1422, 0, BIT0 ),   //reg[1422] #0 = 0b
    #endif

    _MEMMAP_nonPM_,

    #if(PAD_DDCDA_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDA_CLK_OEN (PAD_DDCDA_CLK_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_DDCDA_CLK_OUT (PAD_DDCDA_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e16, PAD_DDCDA_CLK_OUT, BIT0),
    _RVM1(0x1e18, PAD_DDCDA_CLK_OEN, BIT0),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_ddcda_en = 0b
    _RVM1(0x1ef9, 0, BIT1 ),   //reg[101ef9]#1 = 0b
    #endif

    #if(PAD_DDCDA_DAT_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDA_DAT_OEN (PAD_DDCDA_DAT_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_DDCDA_DAT_OUT (PAD_DDCDA_DAT_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e16, PAD_DDCDA_DAT_OUT, BIT1),
    _RVM1(0x1e18, PAD_DDCDA_DAT_OEN, BIT1),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_ddcda_en = 0b
    _RVM1(0x1ef9, 0, BIT1 ),   //reg[101ef9]#1 = 0b
    #endif

    #if(PAD_HOTPLUG_A_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUG_A_OEN (PAD_HOTPLUG_A_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_HOTPLUG_A_OUT (PAD_HOTPLUG_A_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e17, PAD_HOTPLUG_A_OUT, BIT0),
    _RVM1(0x1e19, PAD_HOTPLUG_A_OEN, BIT0),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_mhl_cbus_config[2:0] = 00b
    _RVM1(0x1efa, 0, BIT2|BIT1|BIT0 ),   //reg[101efa]#2~#0 = 00b
    #endif

    #if(PAD_DDCDB_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDB_CLK_OEN (PAD_DDCDB_CLK_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_DDCDB_CLK_OUT (PAD_DDCDB_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e16, PAD_DDCDB_CLK_OUT, BIT2),
    _RVM1(0x1e18, PAD_DDCDB_CLK_OEN, BIT2),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_ddcdb_en = 0b
    _RVM1(0x1ef9, 0, BIT2 ),   //reg[101ef9]#2 = 0b
    #endif

    #if(PAD_DDCDB_DAT_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDB_DAT_OEN (PAD_DDCDB_DAT_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_DDCDB_DAT_OUT (PAD_DDCDB_DAT_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e16, PAD_DDCDB_DAT_OUT, BIT3),
    _RVM1(0x1e18, PAD_DDCDB_DAT_OEN, BIT3),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_ddcdb_en = 0b
    _RVM1(0x1ef9, 0, BIT2 ),   //reg[101ef9]#2 = 0b
    #endif

    #if(PAD_HOTPLUG_B_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUG_B_OEN (PAD_HOTPLUG_B_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_HOTPLUG_B_OUT (PAD_HOTPLUG_B_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e17, PAD_HOTPLUG_B_OUT, BIT1),
    _RVM1(0x1e19, PAD_HOTPLUG_B_OEN, BIT1),
    //reg_mhl_cbus_config[2:0] = 00b
    _RVM1(0x1efa, 0, BIT2|BIT1|BIT0 ),   //reg[101efa]#2~#0 = 00b
    #endif

    #if(PAD_DDCDC_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDC_CLK_OEN (PAD_DDCDC_CLK_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_DDCDC_CLK_OUT (PAD_DDCDC_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e16, PAD_DDCDC_CLK_OUT, BIT4),
    _RVM1(0x1e18, PAD_DDCDC_CLK_OEN, BIT4),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_ddcdc_en = 0b
    _RVM1(0x1ef9, 0, BIT3 ),   //reg[101ef9]#3 = 0b
    #endif

    #if(PAD_DDCDC_DAT_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDC_DAT_OEN (PAD_DDCDC_DAT_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_DDCDC_DAT_OUT (PAD_DDCDC_DAT_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e16, PAD_DDCDC_DAT_OUT, BIT5),
    _RVM1(0x1e18, PAD_DDCDC_DAT_OEN, BIT5),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_ddcdc_en = 0b
    _RVM1(0x1ef9, 0, BIT3 ),   //reg[101ef9]#3 = 0b
    #endif

    #if(PAD_HOTPLUG_C_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUG_C_OEN (PAD_HOTPLUG_C_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_HOTPLUG_C_OUT (PAD_HOTPLUG_C_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e17, PAD_HOTPLUG_C_OUT, BIT2),
    _RVM1(0x1e19, PAD_HOTPLUG_C_OEN, BIT2),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_mhl_cbus_config[2:0] = 00b
    _RVM1(0x1efa, 0, BIT2|BIT1|BIT0 ),   //reg[101efa]#2~#0 = 00b
    #endif

    #if(PAD_DDCDD_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDD_CLK_OEN (PAD_DDCDD_CLK_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_DDCDD_CLK_OUT (PAD_DDCDD_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e16, PAD_DDCDD_CLK_OUT, BIT6),
    _RVM1(0x1e18, PAD_DDCDD_CLK_OEN, BIT6),
    //reg_ddcdd_en = 0b
    _RVM1(0x1efb, 0, BIT7 ),   //reg[101efb]#7 = 0b
    #endif

    #if(PAD_DDCDD_DAT_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDD_DAT_OEN (PAD_DDCDD_DAT_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_DDCDD_DAT_OUT (PAD_DDCDD_DAT_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e16, PAD_DDCDD_DAT_OUT, BIT7),
    _RVM1(0x1e18, PAD_DDCDD_DAT_OEN, BIT7),
    //reg_ddcdd_en = 0b
    _RVM1(0x1efb, 0, BIT7 ),   //reg[101efb]#7 = 0b
    #endif

    #if(PAD_HOTPLUG_D_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUG_D_OEN (PAD_HOTPLUG_D_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_HOTPLUG_D_OUT (PAD_HOTPLUG_D_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e12, PAD_HOTPLUG_D_OUT, BIT5),
    _RVM1(0x1e13, PAD_HOTPLUG_D_OEN, BIT2),
    //reg_mhl_cbus_config[2:0] = 00b
    _RVM1(0x1efa, 0, BIT2|BIT1|BIT0 ),   //reg[101efa]#2~#0 = 00b
    #endif

    #if(PAD_MHL_DET_IS_GPIO != GPIO_NONE)
    #define PAD_MHL_DET_OEN (PAD_MHL_DET_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_MHL_DET_OUT (PAD_MHL_DET_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e17, PAD_MHL_DET_OUT, BIT3),
    _RVM1(0x1e19, PAD_MHL_DET_OEN, BIT3),
    //reg_test_in_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT1|BIT0 ),   //reg[101e24]#1~#0 = 00b
    //reg_test_out_mode[1:0] = 00b
    _RVM1(0x1e24, 0, BIT5|BIT4 ),   //reg[101e24]#5~#4 = 00b
    //reg_mhl_det_en = 0b
    _RVM1(0x1ef9, 0, BIT0 ),   //reg[101ef9]#0 = 0b
    #endif

    #if(PAD_ARC_IS_GPIO != GPIO_NONE)
    #define PAD_ARC_OEN (PAD_ARC_IS_GPIO == GPIO_IN ? BIT4 : 0)
    #define PAD_ARC_OUT (PAD_ARC_IS_GPIO == GPIO_OUT_HIGH ? BIT4 : 0)
    _RVM1(0x1e17, PAD_ARC_OUT, BIT4 ),
    _RVM1(0x1e19, PAD_ARC_OEN, BIT4 ),
    //reg_spdifinconfig = 0b
    _RVM1(0x1eae, 0, BIT7 ),   //reg[101eae]#7 = 0b
    //reg_spdifout2arc= 0b
    _RVM1(0x1eae, 0, BIT0 ),   //reg[101eae]#0 = 0b
    #endif

    #if(PAD_HOTPLUGA_HDMI20_5V_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUGA_HDMI20_5V_OEN (PAD_HOTPLUGA_HDMI20_5V_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_HOTPLUGA_HDMI20_5V_OUT (PAD_HOTPLUGA_HDMI20_5V_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e12, PAD_HOTPLUGA_HDMI20_5V_OUT, BIT6),
    _RVM1(0x1e13, PAD_HOTPLUGA_HDMI20_5V_OEN, BIT3),
    #endif

    #if(PAD_HOTPLUGB_HDMI20_5V_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUGB_HDMI20_5V_OEN (PAD_HOTPLUGB_HDMI20_5V_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_HOTPLUGB_HDMI20_5V_OUT (PAD_HOTPLUGB_HDMI20_5V_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e12, PAD_HOTPLUGB_HDMI20_5V_OUT, BIT7),
    _RVM1(0x1e13, PAD_HOTPLUGB_HDMI20_5V_OEN, BIT4),
    #endif

    #if(PAD_HOTPLUGC_HDMI20_5V_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUGC_HDMI20_5V_OEN (PAD_HOTPLUGC_HDMI20_5V_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_HOTPLUGC_HDMI20_5V_OUT (PAD_HOTPLUGC_HDMI20_5V_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e13, PAD_HOTPLUGC_HDMI20_5V_OUT, BIT0),
    _RVM1(0x1e13, PAD_HOTPLUGC_HDMI20_5V_OEN, BIT5),
    #endif

    #if(PAD_HOTPLUGD_HDMI20_5V_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUGD_HDMI20_5V_OEN (PAD_HOTPLUGD_HDMI20_5V_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_HOTPLUGD_HDMI20_5V_OUT (PAD_HOTPLUGD_HDMI20_5V_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e13, PAD_HOTPLUGD_HDMI20_5V_OUT, BIT1),
    _RVM1(0x1e13, PAD_HOTPLUGD_HDMI20_5V_OEN, BIT6),
    #endif

    #if(PADA_HSYNC0_IS_GPIO != GPIO_NONE)
    #define PADA_HSYNC0_OEN (PADA_HSYNC0_IS_GPIO == GPIO_IN ? 0 : BIT0)
    #define PADA_HSYNC0_OUT (PADA_HSYNC0_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x255a, PADA_HSYNC0_OUT, BIT6),
    _RVM1(0x255a, PADA_HSYNC0_OEN, BIT0),
    //reg_test_hsync[3] = 0b
    _RVM1(0x2560, 0, BIT3 ),   //reg[102560]#3 = 0b
    //reg_gpio_hsync_en[0] = 1b
    _RVM1(0x2558, BIT0 , BIT0 ),   //reg[102558]#0 = 1b
    #endif

    #if(PADA_HSYNC1_IS_GPIO != GPIO_NONE)
    #define PADA_HSYNC1_OEN (PADA_HSYNC1_IS_GPIO == GPIO_IN ? 0 : BIT1)
    #define PADA_HSYNC1_OUT (PADA_HSYNC1_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x255a, PADA_HSYNC1_OUT, BIT7),
    _RVM1(0x255a, PADA_HSYNC1_OEN, BIT1),
    //reg_test_hsync[4] = 0b
    _RVM1(0x2560, 0, BIT4 ),   //reg[102560]#4 = 0b
    //reg_gpio_hsync_en[1] = 1b
    _RVM1(0x2558, BIT1 , BIT1 ),   //reg[102558]#1 = 1b
    #endif

    #if(PADA_VSYNC0_IS_GPIO != GPIO_NONE)
    #define PADA_VSYNC0_OEN (PADA_VSYNC0_IS_GPIO == GPIO_IN ? 0 : BIT3)
    #define PADA_VSYNC0_OUT (PADA_VSYNC0_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x255b, PADA_VSYNC0_OUT, BIT1),
    _RVM1(0x255a, PADA_VSYNC0_OEN, BIT3),
    //reg_test_vsync[0] = 0b
    _RVM1(0x2561, 0, BIT0 ),   //reg[102561]#0 = 0b
    //reg_gpio_vsync_en[3] = 1b
    _RVM1(0x2558, BIT3 , BIT3 ),   //reg[102558]#3 = 1b
    #endif

    #if(PADA_VSYNC1_IS_GPIO != GPIO_NONE)
    #define PADA_VSYNC1_OEN (PADA_VSYNC1_IS_GPIO == GPIO_IN ? 0 : BIT4)
    #define PADA_VSYNC1_OUT (PADA_VSYNC1_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x255b, PADA_VSYNC1_OUT, BIT2),
    _RVM1(0x255a, PADA_VSYNC1_OEN, BIT4),
    //reg_test_vsync[1] = 0b
    _RVM1(0x2561, 0, BIT1 ),   //reg[102561]#1 = 0b
    //reg_gpio_vsync_en[4] = 1b
    _RVM1(0x2558, BIT4 , BIT4 ),   //reg[102558]#4 = 1b
    #endif

    #if(PADA_RIN0P_IS_GPIO != GPIO_NONE)
    #define PADA_RIN0P_OEN (PADA_RIN0P_IS_GPIO == GPIO_IN ? 0 : BIT0)
    #define PADA_RIN0P_OUT (PADA_RIN0P_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2537, PADA_RIN0P_OUT, BIT0),
    _RVM1(0x2536, PADA_RIN0P_OEN, BIT0),
    //reg_gpio_rgb_en[0] = 1b
    _RVM1(0x2534, BIT0 , BIT0 ),   //reg[102534]#0 = 1b
    #endif

    #if(PADA_RIN1P_IS_GPIO != GPIO_NONE)
    #define PADA_RIN1P_OEN (PADA_RIN1P_IS_GPIO == GPIO_IN ? 0 : BIT1)
    #define PADA_RIN1P_OUT (PADA_RIN1P_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x2537, PADA_RIN1P_OUT, BIT1),
    _RVM1(0x2536, PADA_RIN1P_OEN, BIT1),
    //reg_gpio_rgb_en[1] = 1b
    _RVM1(0x2534, BIT1 , BIT1 ),   //reg[102534]#1 = 1b
    #endif

    #if(PADA_RIN0M_IS_GPIO != GPIO_NONE)
    #define PADA_RIN0M_OEN (PADA_RIN0M_IS_GPIO == GPIO_IN ? 0 : BIT4)
    #define PADA_RIN0M_OUT (PADA_RIN0M_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2537, PADA_RIN0M_OUT, BIT4),
    _RVM1(0x2536, PADA_RIN0M_OEN, BIT4),
    //reg_gpio_rgb_en[0] = 1b
    _RVM1(0x2534, BIT0 , BIT0 ),   //reg[102534]#0 = 1b
    #endif

    #if(PADA_RIN1M_IS_GPIO != GPIO_NONE)
    #define PADA_RIN1M_OEN (PADA_RIN1M_IS_GPIO == GPIO_IN ? 0 : BIT5)
    #define PADA_RIN1M_OUT (PADA_RIN1M_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x2537, PADA_RIN1M_OUT, BIT5),
    _RVM1(0x2536, PADA_RIN1M_OEN, BIT5),
    //reg_gpio_rgb_en[1] = 1b
    _RVM1(0x2534, BIT1 , BIT1 ),   //reg[102534]#1 = 1b
    #endif

    #if(PADA_GIN0P_IS_GPIO != GPIO_NONE)
    #define PADA_GIN0P_OEN (PADA_GIN0P_IS_GPIO == GPIO_IN ? 0 : BIT0)
    #define PADA_GIN0P_OUT (PADA_GIN0P_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2539, PADA_GIN0P_OUT, BIT0),
    _RVM1(0x2538, PADA_GIN0P_OEN, BIT0),
    //reg_gpio_rgb_en[0] = 1b
    _RVM1(0x2534, BIT0 , BIT0 ),   //reg[102534]#0 = 1b
    #endif

    #if(PADA_GIN1P_IS_GPIO != GPIO_NONE)
    #define PADA_GIN1P_OEN (PADA_GIN1P_IS_GPIO == GPIO_IN ? 0 : BIT1)
    #define PADA_GIN1P_OUT (PADA_GIN1P_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x2539, PADA_GIN1P_OUT, BIT1),
    _RVM1(0x2538, PADA_GIN1P_OEN, BIT1),
    //reg_gpio_rgb_en[1] = 1b
    _RVM1(0x2534, BIT1 , BIT1 ),   //reg[102534]#1 = 1b
    #endif

    #if(PADA_GIN0M_IS_GPIO != GPIO_NONE)
    #define PADA_GIN0M_OEN (PADA_GIN0M_IS_GPIO == GPIO_IN ? 0 : BIT4)
    #define PADA_GIN0M_OUT (PADA_GIN0M_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2539, PADA_GIN0M_OUT, BIT4),
    _RVM1(0x2538, PADA_GIN0M_OEN, BIT4),
    //reg_gpio_rgb_en[0] = 1b
    _RVM1(0x2534, BIT0 , BIT0 ),   //reg[102534]#0 = 1b
    #endif

    #if(PADA_GIN1M_IS_GPIO != GPIO_NONE)
    #define PADA_GIN1M_OEN (PADA_GIN1M_IS_GPIO == GPIO_IN ? 0 : BIT5)
    #define PADA_GIN1M_OUT (PADA_GIN1M_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x2539, PADA_GIN1M_OUT, BIT5),
    _RVM1(0x2538, PADA_GIN1M_OEN, BIT5),
    //reg_gpio_rgb_en[1] = 1b
    _RVM1(0x2534, BIT1 , BIT1 ),   //reg[102534]#1 = 1b
    #endif

    #if(PADA_BIN0P_IS_GPIO != GPIO_NONE)
    #define PADA_BIN0P_OEN (PADA_BIN0P_IS_GPIO == GPIO_IN ? 0 : BIT0)
    #define PADA_BIN0P_OUT (PADA_BIN0P_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x253b, PADA_BIN0P_OUT, BIT0),
    _RVM1(0x253a, PADA_BIN0P_OEN, BIT0),
    //reg_gpio_rgb_en[0] = 1b
    _RVM1(0x2534, BIT0 , BIT0 ),   //reg[102534]#0 = 1b
    #endif

    #if(PADA_BIN1P_IS_GPIO != GPIO_NONE)
    #define PADA_BIN1P_OEN (PADA_BIN1P_IS_GPIO == GPIO_IN ? 0 : BIT1)
    #define PADA_BIN1P_OUT (PADA_BIN1P_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x253b, PADA_BIN1P_OUT, BIT1),
    _RVM1(0x253a, PADA_BIN1P_OEN, BIT1),
    //reg_gpio_rgb_en[1] = 1b
    _RVM1(0x2534, BIT1 , BIT1 ),   //reg[102534]#1 = 1b
    #endif

    #if(PADA_BIN0M_IS_GPIO != GPIO_NONE)
    #define PADA_BIN0M_OEN (PADA_BIN0M_IS_GPIO == GPIO_IN ? 0 : BIT4)
    #define PADA_BIN0M_OUT (PADA_BIN0M_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x253b, PADA_BIN0M_OUT, BIT4),
    _RVM1(0x253a, PADA_BIN0M_OEN, BIT4),
    //reg_gpio_rgb_en[0] = 1b
    _RVM1(0x2534, BIT0 , BIT0 ),   //reg[102534]#0 = 1b
    #endif

    #if(PADA_BIN1M_IS_GPIO != GPIO_NONE)
    #define PADA_BIN1M_OEN (PADA_BIN1M_IS_GPIO == GPIO_IN ? 0 : BIT5)
    #define PADA_BIN1M_OUT (PADA_BIN1M_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x253b, PADA_BIN1M_OUT, BIT5),
    _RVM1(0x253a, PADA_BIN1M_OEN, BIT5),
    //reg_gpio_rgb_en[1] = 1b
    _RVM1(0x2534, BIT1 , BIT1 ),   //reg[102534]#1 = 1b
    #endif

    _MEMMAP_PM_,

    #if(PAD_LINEIN_L0_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_L0_OEN (PAD_LINEIN_L0_IS_GPIO == GPIO_IN ? BIT2 : 0)
    #define PAD_LINEIN_L0_OUT (PAD_LINEIN_L0_IS_GPIO == GPIO_OUT_HIGH ? BIT0 : 0)
    _MEMMAP_nonBK11_,
    _RVM1(0x2cf6, PAD_LINEIN_L0_OUT, BIT0 ),
    _RVM1(0x2cf6, PAD_LINEIN_L0_OEN, BIT2 ),
    _RVM1(0x2cf6, BIT1 , BIT1 ),   //reg[112cf6]#1 = 1b
    #endif

    #if(PAD_LINEIN_R0_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_R0_OEN (PAD_LINEIN_R0_IS_GPIO == GPIO_IN ? BIT6 : 0)
    #define PAD_LINEIN_R0_OUT (PAD_LINEIN_R0_IS_GPIO == GPIO_OUT_HIGH ? BIT4 : 0)
    _MEMMAP_nonBK11_,
    _RVM1(0x2cf6, PAD_LINEIN_R0_OUT, BIT4 ),
    _RVM1(0x2cf6, PAD_LINEIN_R0_OEN, BIT6 ),
    _RVM1(0x2cf6, BIT5 , BIT5 ),   //reg[112cf6]#5 = 1b
    #endif

    #if(PAD_LINEIN_L1_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_L1_OEN (PAD_LINEIN_L1_IS_GPIO == GPIO_IN ? BIT2 : 0)
    #define PAD_LINEIN_L1_OUT (PAD_LINEIN_L1_IS_GPIO == GPIO_OUT_HIGH ? BIT0 : 0)
    _MEMMAP_nonBK11_,
    _RVM1(0x2cf7, PAD_LINEIN_L1_OUT, BIT0 ),
    _RVM1(0x2cf7, PAD_LINEIN_L1_OEN, BIT2 ),
    _RVM1(0x2cf7, BIT1 , BIT1 ),   //reg[112cf7]#1 = 1b
    #endif

    #if(PAD_LINEIN_R1_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_R1_OEN (PAD_LINEIN_R1_IS_GPIO == GPIO_IN ? BIT6 : 0)
    #define PAD_LINEIN_R1_OUT (PAD_LINEIN_R1_IS_GPIO == GPIO_OUT_HIGH ? BIT4 : 0)
    _MEMMAP_nonBK11_,
    _RVM1(0x2cf7, PAD_LINEIN_R1_OUT, BIT4 ),
    _RVM1(0x2cf7, PAD_LINEIN_R1_OEN, BIT6 ),
    _RVM1(0x2cf7, BIT5 , BIT5 ),   //reg[112cf7]#5 = 1b
    #endif

    #if(PAD_LINEIN_L2_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_L2_OEN (PAD_LINEIN_L2_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_LINEIN_L2_OUT (PAD_LINEIN_L2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _MEMMAP_nonBK11_,
    _RVM1(0x2cf8, PAD_LINEIN_L2_OUT, BIT0),
    _RVM1(0x2cf8, PAD_LINEIN_L2_OEN, BIT2),
    _RVM1(0x2cf8, BIT1 , BIT1 ),   //reg[0x112cf8]#1 = 1b
    #endif

    #if(PAD_LINEIN_R2_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_R2_OEN (PAD_LINEIN_R2_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_LINEIN_R2_OUT (PAD_LINEIN_R2_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _MEMMAP_nonBK11_,
    _RVM1(0x2cf8, PAD_LINEIN_R2_OUT, BIT4),
    _RVM1(0x2cf8, PAD_LINEIN_R2_OEN, BIT6),
    _RVM1(0x2cf8, BIT5 , BIT5 ),   //reg[0x112cf8]#5 = 1b
    #endif

    #if(PAD_LINEIN_L3_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_L3_OEN (PAD_LINEIN_L3_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_LINEIN_L3_OUT (PAD_LINEIN_L3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _MEMMAP_nonBK11_,
    _RVM1(0x2cf9, PAD_LINEIN_L3_OUT, BIT0),
    _RVM1(0x2cf9, PAD_LINEIN_L3_OEN, BIT2),
    _RVM1(0x2cf9, BIT1 , BIT1 ),   //reg[0x112cf9]#1 = 1b
    #endif

    #if(PAD_LINEIN_R3_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_R3_OEN (PAD_LINEIN_R3_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_LINEIN_R3_OUT (PAD_LINEIN_R3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _MEMMAP_nonBK11_,
    _RVM1(0x2cf9, PAD_LINEIN_R3_OUT, BIT4),
    _RVM1(0x2cf9, PAD_LINEIN_R3_OEN, BIT6),
    _RVM1(0x2cf9, BIT5 , BIT5 ),   //reg[0x112cf9]#5 = 1b
    #endif

    #if(PAD_LINEIN_L4_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_L4_OEN (PAD_LINEIN_L4_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_LINEIN_L4_OUT (PAD_LINEIN_L4_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _MEMMAP_nonBK11_,
    _RVM1(0x2cfa, PAD_LINEIN_L4_OUT, BIT0),
    _RVM1(0x2cfa, PAD_LINEIN_L4_OEN, BIT2),
    _RVM1(0x2cfa, BIT1 , BIT1 ),   //reg[0x112cfa]#1 = 1b
    #endif

    #if(PAD_LINEIN_R4_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_R4_OEN (PAD_LINEIN_R4_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_LINEIN_R4_OUT (PAD_LINEIN_R4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _MEMMAP_nonBK11_,
    _RVM1(0x2cfa, PAD_LINEIN_R4_OUT, BIT4),
    _RVM1(0x2cfa, PAD_LINEIN_R4_OEN, BIT6),
    _RVM1(0x2cfa, BIT5 , BIT5 ),   //reg[0x112cfa]#5 = 1b
    #endif

    #if(PAD_LINEIN_L5_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_L5_OEN (PAD_LINEIN_L5_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_LINEIN_L5_OUT (PAD_LINEIN_L5_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _MEMMAP_nonBK11_,
    _RVM1(0x2cfb, PAD_LINEIN_L5_OUT, BIT0),
    _RVM1(0x2cfb, PAD_LINEIN_L5_OEN, BIT2),
    _RVM1(0x2cfb, BIT1 , BIT1 ),   //reg[0x112cfb]#1 = 1b
    #endif

    #if(PAD_LINEIN_R5_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_R5_OEN (PAD_LINEIN_R5_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_LINEIN_R5_OUT (PAD_LINEIN_R5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _MEMMAP_nonBK11_,
    _RVM1(0x2cfb, PAD_LINEIN_R5_OUT, BIT4),
    _RVM1(0x2cfb, PAD_LINEIN_R5_OEN, BIT6),
    _RVM1(0x2cfb, BIT5 , BIT5 ),   //reg[0x112cfb]#5 = 1b
    #endif

    _MEMMAP_nonPM_,

    #if(PORT_A_PAD_RX0P_IS_GPIO != GPIO_NONE)
    #define PORT_A_PAD_RX0P_OEN (PORT_A_PAD_RX0P_IS_GPIO == GPIO_IN ? 0 : BIT0)
    #define PORT_A_PAD_RX0P_IEN (PORT_A_PAD_RX0P_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PORT_A_PAD_RX0P_OUT (PORT_A_PAD_RX0P_IS_GPIO == GPIO_OUT_HIGH ? BIT0 : 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a1, PORT_A_PAD_RX0P_OUT, BIT0 ),
    _RVM1(0x03a0, PORT_A_PAD_RX0P_OEN, BIT0),
    _RVM1(0x03a2, PORT_A_PAD_RX0P_IEN, BIT4),
    _MEMMAP_PM_,
    _RVM1(0x2ec6, BIT0, BIT0),   //reg[002ec6]#0=1
    _RVM1(0x2ec6, BIT1, BIT1),   //reg[002ec6]#1=1
    _RVM1(0x2ec6, BIT2, BIT2),   //reg[002ec6]#2=1
    _RVM1(0x2ec6, BIT3, BIT3),   //reg[002ec6]#3=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT3),   //reg[17033f]#3=0
    _RVM1(0x03a3, BIT4, BIT4),   //reg[1703a3]#4=1
    _RVM1(0x03a1, 0, BIT4),   //reg[1703a1]#4=0
    _RVM1(0x03a1, 0, BIT5),   //reg[1703a1]#5=0
    _RVM1(0x03a1, 0, BIT6),   //reg[1703a1]#6=0
    _RVM1(0x03a1, 0, BIT7),   //reg[1703a1]#7=0
    #endif

    #if(PORT_A_PAD_RX1P_IS_GPIO != GPIO_NONE)
    #define PORT_A_PAD_RX1P_OEN (PORT_A_PAD_RX1P_IS_GPIO == GPIO_IN ? 0 : BIT1)
    #define PORT_A_PAD_RX1P_IEN (PORT_A_PAD_RX1P_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PORT_A_PAD_RX1P_OUT (PORT_A_PAD_RX1P_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a1, PORT_A_PAD_RX1P_OUT, BIT1),
    _RVM1(0x03a0, PORT_A_PAD_RX1P_OEN, BIT1),
    _RVM1(0x03a2, PORT_A_PAD_RX1P_IEN, BIT5),
    _MEMMAP_PM_,
    _RVM1(0x2ec6, BIT0, BIT0),   //reg[002ec6]#0=1
    _RVM1(0x2ec6, BIT1, BIT1),   //reg[002ec6]#1=1
    _RVM1(0x2ec6, BIT2, BIT2),   //reg[002ec6]#2=1
    _RVM1(0x2ec6, BIT3, BIT3),   //reg[002ec6]#3=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT3),   //reg[17033f]#3=0
    _RVM1(0x03a3, BIT4, BIT4),   //reg[1703a3]#4=1
    _RVM1(0x03a1, 0, BIT4),   //reg[1703a1]#4=0
    _RVM1(0x03a1, 0, BIT5),   //reg[1703a1]#5=0
    _RVM1(0x03a1, 0, BIT6),   //reg[1703a1]#6=0
    _RVM1(0x03a1, 0, BIT7),   //reg[1703a1]#7=0
    #endif

    #if(PORT_A_PAD_RX2P_IS_GPIO != GPIO_NONE)
    #define PORT_A_PAD_RX2P_OEN (PORT_A_PAD_RX2P_IS_GPIO == GPIO_IN ? 0 : BIT2)
    #define PORT_A_PAD_RX2P_IEN (PORT_A_PAD_RX2P_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PORT_A_PAD_RX2P_OUT (PORT_A_PAD_RX2P_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a1, PORT_A_PAD_RX2P_OUT, BIT2),
    _RVM1(0x03a0, PORT_A_PAD_RX2P_OEN, BIT2),
    _RVM1(0x03a2, PORT_A_PAD_RX2P_IEN, BIT6),
    _MEMMAP_PM_,
    _RVM1(0x2ec6, BIT0, BIT0),   //reg[002ec6]#0=1
    _RVM1(0x2ec6, BIT1, BIT1),   //reg[002ec6]#1=1
    _RVM1(0x2ec6, BIT2, BIT2),   //reg[002ec6]#2=1
    _RVM1(0x2ec6, BIT3, BIT3),   //reg[002ec6]#3=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT3),   //reg[17033f]#3=0
    _RVM1(0x03a3, BIT4, BIT4),   //reg[1703a3]#4=1
    _RVM1(0x03a1, 0, BIT4),   //reg[1703a1]#4=0
    _RVM1(0x03a1, 0, BIT5),   //reg[1703a1]#5=0
    _RVM1(0x03a1, 0, BIT6),   //reg[1703a1]#6=0
    _RVM1(0x03a1, 0, BIT7),   //reg[1703a1]#7=0
    #endif

    #if(PORT_A_PAD_RXCP_IS_GPIO != GPIO_NONE)
    #define PORT_A_PAD_RXCP_OEN (PORT_A_PAD_RXCP_IS_GPIO == GPIO_IN ? 0 : BIT3)
    #define PORT_A_PAD_RXCP_IEN (PORT_A_PAD_RXCP_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PORT_A_PAD_RXCP_OUT (PORT_A_PAD_RXCP_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a1, PORT_A_PAD_RXCP_OUT, BIT3),
    _RVM1(0x03a0, PORT_A_PAD_RXCP_OEN, BIT3),
    _RVM1(0x03a2, PORT_A_PAD_RXCP_IEN, BIT7),
    _MEMMAP_PM_,
    _RVM1(0x2ec6, BIT0, BIT0),   //reg[002ec6]#0=1
    _RVM1(0x2ec6, BIT1, BIT1),   //reg[002ec6]#1=1
    _RVM1(0x2ec6, BIT2, BIT2),   //reg[002ec6]#2=1
    _RVM1(0x2ec6, BIT3, BIT3),   //reg[002ec6]#3=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT3),   //reg[17033f]#3=0
    _RVM1(0x03a3, BIT4, BIT4),   //reg[1703a3]#4=1
    _RVM1(0x03a1, 0, BIT4),   //reg[1703a1]#4=0
    _RVM1(0x03a1, 0, BIT5),   //reg[1703a1]#5=0
    _RVM1(0x03a1, 0, BIT6),   //reg[1703a1]#6=0
    _RVM1(0x03a1, 0, BIT7),   //reg[1703a1]#7=0
    #endif

    #if(PORT_A_PAD_RX0N_IS_GPIO != GPIO_NONE)
    #define PORT_A_PAD_RX0N_OEN (PORT_A_PAD_RX0N_IS_GPIO == GPIO_IN ? 0 : BIT4)
    #define PORT_A_PAD_RX0N_IEN (PORT_A_PAD_RX0N_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PORT_A_PAD_RX0N_OUT (PORT_A_PAD_RX0N_IS_GPIO == GPIO_OUT_HIGH ? BIT0 : 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a2, PORT_A_PAD_RX0N_OUT, BIT0 ),
    _RVM1(0x03a0, PORT_A_PAD_RX0N_OEN, BIT4),
    _RVM1(0x03a3, PORT_A_PAD_RX0N_IEN, BIT0),
    _MEMMAP_PM_,
    _RVM1(0x2ec6, BIT0, BIT0),   //reg[002ec6]#0=1
    _RVM1(0x2ec6, BIT1, BIT1),   //reg[002ec6]#1=1
    _RVM1(0x2ec6, BIT2, BIT2),   //reg[002ec6]#2=1
    _RVM1(0x2ec6, BIT3, BIT3),   //reg[002ec6]#3=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT3),   //reg[17033f]#3=0
    _RVM1(0x03a3, BIT4, BIT4),   //reg[1703a3]#4=1
    _RVM1(0x03a1, 0, BIT4),   //reg[1703a1]#4=0
    _RVM1(0x03a1, 0, BIT5),   //reg[1703a1]#5=0
    _RVM1(0x03a1, 0, BIT6),   //reg[1703a1]#6=0
    _RVM1(0x03a1, 0, BIT7),   //reg[1703a1]#7=0
    #endif

    #if(PORT_A_PAD_RX1N_IS_GPIO != GPIO_NONE)
    #define PORT_A_PAD_RX1N_OEN (PORT_A_PAD_RX1N_IS_GPIO == GPIO_IN ? 0 : BIT5)
    #define PORT_A_PAD_RX1N_OEN (PORT_A_PAD_RX1N_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PORT_A_PAD_RX1N_OUT (PORT_A_PAD_RX1N_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a2, PORT_A_PAD_RX1N_OUT, BIT1),
    _RVM1(0x03a0, PORT_A_PAD_RX1N_OEN, BIT5),
    _RVM1(0x03a3, PORT_A_PAD_RX1N_IEN, BIT1),
    _MEMMAP_PM_,
    _RVM1(0x2ec6, BIT0, BIT0),   //reg[002ec6]#0=1
    _RVM1(0x2ec6, BIT1, BIT1),   //reg[002ec6]#1=1
    _RVM1(0x2ec6, BIT2, BIT2),   //reg[002ec6]#2=1
    _RVM1(0x2ec6, BIT3, BIT3),   //reg[002ec6]#3=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT3),   //reg[17033f]#3=0
    _RVM1(0x03a3, BIT4, BIT4),   //reg[1703a3]#4=1
    _RVM1(0x03a1, 0, BIT4),   //reg[1703a1]#4=0
    _RVM1(0x03a1, 0, BIT5),   //reg[1703a1]#5=0
    _RVM1(0x03a1, 0, BIT6),   //reg[1703a1]#6=0
    _RVM1(0x03a1, 0, BIT7),   //reg[1703a1]#7=0
    #endif

    #if(PORT_A_PAD_RX2N_IS_GPIO != GPIO_NONE)
    #define PORT_A_PAD_RX2N_OEN (PORT_A_PAD_RX2N_IS_GPIO == GPIO_IN ? 0 : BIT6)
    #define PORT_A_PAD_RX2N_IEN (PORT_A_PAD_RX2N_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PORT_A_PAD_RX2N_OUT (PORT_A_PAD_RX2N_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a2, PORT_A_PAD_RX2N_OUT, BIT2),
    _RVM1(0x03a0, PORT_A_PAD_RX2N_OEN, BIT6),
    _RVM1(0x03a3, PORT_A_PAD_RX2N_IEN, BIT2),
    _MEMMAP_PM_,
    _RVM1(0x2ec6, BIT0, BIT0),   //reg[002e63]#0=1
    _RVM1(0x2ec6, BIT1, BIT1),   //reg[002e63]#1=1
    _RVM1(0x2ec6, BIT2, BIT2),   //reg[002e63]#2=1
    _RVM1(0x2ec6, BIT3, BIT3),   //reg[002e63]#3=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT3),   //reg[17033f]#3=0
    _RVM1(0x03a3, BIT4, BIT4),   //reg[1703a3]#4=1
    _RVM1(0x03a1, 0, BIT4),   //reg[1703a1]#4=0
    _RVM1(0x03a1, 0, BIT5),   //reg[1703a1]#5=0
    _RVM1(0x03a1, 0, BIT6),   //reg[1703a1]#6=0
    _RVM1(0x03a1, 0, BIT7),   //reg[1703a1]#7=0
    #endif

    #if(PORT_A_PAD_RXCN_IS_GPIO != GPIO_NONE)
    #define PORT_A_PAD_RXCN_OEN (PORT_A_PAD_RXCN_IS_GPIO == GPIO_IN ? 0 : BIT7)
    #define PORT_A_PAD_RXCN_IEN (PORT_A_PAD_RXCN_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PORT_A_PAD_RXCN_OUT (PORT_A_PAD_RXCN_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a2, PORT_A_PAD_RXCN_OUT, BIT3),
    _RVM1(0x03a0, PORT_A_PAD_RXCN_OEN, BIT7),
    _RVM1(0x03a3, PORT_A_PAD_RXCN_IEN, BIT3),
    _MEMMAP_PM_,
    _RVM1(0x2ec6, BIT0, BIT0),   //reg[002e63]#0=1
    _RVM1(0x2ec6, BIT1, BIT1),   //reg[002e63]#1=1
    _RVM1(0x2ec6, BIT2, BIT2),   //reg[002e63]#2=1
    _RVM1(0x2ec6, BIT3, BIT3),   //reg[002e63]#3=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT3),   //reg[17033f]#3=0
    _RVM1(0x03a3, BIT4, BIT4),   //reg[1703a3]#4=1
    _RVM1(0x03a1, 0, BIT4),   //reg[1703a1]#4=0
    _RVM1(0x03a1, 0, BIT5),   //reg[1703a1]#5=0
    _RVM1(0x03a1, 0, BIT6),   //reg[1703a1]#6=0
    _RVM1(0x03a1, 0, BIT7),   //reg[1703a1]#7=0
    #endif

    #if(PORT_B_PAD_RX0P_IS_GPIO != GPIO_NONE)
    #define PORT_B_PAD_RX0P_OEN (PORT_B_PAD_RX0P_IS_GPIO == GPIO_IN ? 0 : BIT0)
    #define PORT_B_PAD_RX0P_IEN (PORT_B_PAD_RX0P_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PORT_B_PAD_RX0P_OUT (PORT_B_PAD_RX0P_IS_GPIO == GPIO_OUT_HIGH ? BIT0 : 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a5, PORT_B_PAD_RX0P_OUT, BIT0 ),
    _RVM1(0x03a4, PORT_B_PAD_RX0P_OEN, BIT0),
    _RVM1(0x03a6, PORT_B_PAD_RX0P_IEN, BIT4),
    _MEMMAP_PM_,
    _RVM1(0x2ec6, BIT4, BIT4),   //reg[002e63]#4=1
    _RVM1(0x2ec6, BIT5, BIT5),   //reg[002e63]#5=1
    _RVM1(0x2ec6, BIT6, BIT6),   //reg[002e63]#6=1
    _RVM1(0x2ec6, BIT7, BIT7),   //reg[002e63]#7=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT4),   //reg[17033f]#4=0
    _RVM1(0x03a7, BIT4, BIT4),   //reg[1703a7]#4=1
    _RVM1(0x03a5, 0, BIT4),   //reg[1703a5]#4=0
    _RVM1(0x03a5, 0, BIT5),   //reg[1703a5]#5=0
    _RVM1(0x03a5, 0, BIT6),   //reg[1703a5]#6=0
    _RVM1(0x03a5, 0, BIT7),   //reg[1703a5]#7=0
    #endif

    #if(PORT_B_PAD_RX1P_IS_GPIO != GPIO_NONE)
    #define PORT_B_PAD_RX1P_OEN (PORT_B_PAD_RX1P_IS_GPIO == GPIO_IN ? 0 : BIT1)
    #define PORT_B_PAD_RX1P_IEN (PORT_B_PAD_RX1P_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PORT_B_PAD_RX1P_OUT (PORT_B_PAD_RX1P_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a5, PORT_B_PAD_RX1P_OUT, BIT1),
    _RVM1(0x03a4, PORT_B_PAD_RX1P_OEN, BIT1),
    _RVM1(0x03a6, PORT_B_PAD_RX1P_IEN, BIT5),
    _MEMMAP_PM_,
    _RVM1(0x2ec6, BIT4, BIT4),   //reg[002e63]#4=1
    _RVM1(0x2ec6, BIT5, BIT5),   //reg[002e63]#5=1
    _RVM1(0x2ec6, BIT6, BIT6),   //reg[002e63]#6=1
    _RVM1(0x2ec6, BIT7, BIT7),   //reg[002e63]#7=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT4),   //reg[17033f]#4=0
    _RVM1(0x03a7, BIT4, BIT4),   //reg[1703a7]#4=1
    _RVM1(0x03a5, 0, BIT4),   //reg[1703a5]#4=0
    _RVM1(0x03a5, 0, BIT5),   //reg[1703a5]#5=0
    _RVM1(0x03a5, 0, BIT6),   //reg[1703a5]#6=0
    _RVM1(0x03a5, 0, BIT7),   //reg[1703a5]#7=0
    #endif

    #if(PORT_B_PAD_RX2P_IS_GPIO != GPIO_NONE)
    #define PORT_B_PAD_RX2P_OEN (PORT_B_PAD_RX2P_IS_GPIO == GPIO_IN ? 0 : BIT2)
    #define PORT_B_PAD_RX2P_IEN (PORT_B_PAD_RX2P_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PORT_B_PAD_RX2P_OUT (PORT_B_PAD_RX2P_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a5, PORT_B_PAD_RX2P_OUT, BIT2),
    _RVM1(0x03a4, PORT_B_PAD_RX2P_OEN, BIT2),
    _RVM1(0x03a6, PORT_B_PAD_RX2P_IEN, BIT6),
    _MEMMAP_PM_,
    _RVM1(0x2ec6, BIT4, BIT4),   //reg[002ec6]#4=1
    _RVM1(0x2ec6, BIT5, BIT5),   //reg[002ec6]#5=1
    _RVM1(0x2ec6, BIT6, BIT6),   //reg[002ec6]#6=1
    _RVM1(0x2ec6, BIT7, BIT7),   //reg[002ec6]#7=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT4),   //reg[17033f]#4=0
    _RVM1(0x03a7, BIT4, BIT4),   //reg[1703a7]#4=1
    _RVM1(0x03a5, 0, BIT4),   //reg[1703a5]#4=0
    _RVM1(0x03a5, 0, BIT5),   //reg[1703a5]#5=0
    _RVM1(0x03a5, 0, BIT6),   //reg[1703a5]#6=0
    _RVM1(0x03a5, 0, BIT7),   //reg[1703a5]#7=0
    #endif

    #if(PORT_B_PAD_RXCP_IS_GPIO != GPIO_NONE)
    #define PORT_B_PAD_RXCP_OEN (PORT_B_PAD_RXCP_IS_GPIO == GPIO_IN ? 0 : BIT3)
    #define PORT_B_PAD_RXCP_IEN (PORT_B_PAD_RXCP_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PORT_B_PAD_RXCP_OUT (PORT_B_PAD_RXCP_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a5, PORT_B_PAD_RXCP_OUT, BIT3),
    _RVM1(0x03a4, PORT_B_PAD_RXCP_OEN, BIT3),
    _RVM1(0x03a6, PORT_B_PAD_RXCP_IEN, BIT7),
    _MEMMAP_PM_,
    _RVM1(0x2ec6, BIT4, BIT4),   //reg[002ec6]#4=1
    _RVM1(0x2ec6, BIT5, BIT5),   //reg[002ec6]#5=1
    _RVM1(0x2ec6, BIT6, BIT6),   //reg[002ec6]#6=1
    _RVM1(0x2ec6, BIT7, BIT7),   //reg[002ec6]#7=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT4),   //reg[17033f]#4=0
    _RVM1(0x03a7, BIT4, BIT4),   //reg[1703a7]#4=1
    _RVM1(0x03a5, 0, BIT4),   //reg[1703a5]#4=0
    _RVM1(0x03a5, 0, BIT5),   //reg[1703a5]#5=0
    _RVM1(0x03a5, 0, BIT6),   //reg[1703a5]#6=0
    _RVM1(0x03a5, 0, BIT7),   //reg[1703a5]#7=0
    #endif

    #if(PORT_B_PAD_RX0N_IS_GPIO != GPIO_NONE)
    #define PORT_B_PAD_RX0N_OEN (PORT_B_PAD_RX0N_IS_GPIO == GPIO_IN ? 0 : BIT4)
    #define PORT_B_PAD_RX0N_IEN (PORT_B_PAD_RX0N_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PORT_B_PAD_RX0N_OUT (PORT_B_PAD_RX0N_IS_GPIO == GPIO_OUT_HIGH ? BIT0 : 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a6, PORT_B_PAD_RX0N_OUT, BIT0 ),
    _RVM1(0x03a4, PORT_B_PAD_RX0N_OEN, BIT4),
    _RVM1(0x03a7, PORT_B_PAD_RX0N_IEN, BIT0),
    _MEMMAP_PM_,
    _RVM1(0x2ec6, BIT4, BIT4),   //reg[002ec6]#4=1
    _RVM1(0x2ec6, BIT5, BIT5),   //reg[002ec6]#5=1
    _RVM1(0x2ec6, BIT6, BIT6),   //reg[002ec6]#6=1
    _RVM1(0x2ec6, BIT7, BIT7),   //reg[002ec6]#7=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT4),   //reg[17033f]#4=0
    _RVM1(0x03a7, BIT4, BIT4),   //reg[1703a7]#4=1
    _RVM1(0x03a5, 0, BIT4),   //reg[1703a5]#4=0
    _RVM1(0x03a5, 0, BIT5),   //reg[1703a5]#5=0
    _RVM1(0x03a5, 0, BIT6),   //reg[1703a5]#6=0
    _RVM1(0x03a5, 0, BIT7),   //reg[1703a5]#7=0
    #endif

    #if(PORT_B_PAD_RX1N_IS_GPIO != GPIO_NONE)
    #define PORT_B_PAD_RX1N_OEN (PORT_B_PAD_RX1N_IS_GPIO == GPIO_IN ? 0 : BIT5)
    #define PORT_B_PAD_RX1N_IEN (PORT_B_PAD_RX1N_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PORT_B_PAD_RX1N_OUT (PORT_B_PAD_RX1N_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a6, PORT_B_PAD_RX1N_OUT, BIT1),
    _RVM1(0x03a4, PORT_B_PAD_RX1N_OEN, BIT5),
    _RVM1(0x03a7, PORT_B_PAD_RX1N_IEN, BIT1),
    _MEMMAP_PM_,
    _RVM1(0x2ec6, BIT4, BIT4),   //reg[002ec6]#4=1
    _RVM1(0x2ec6, BIT5, BIT5),   //reg[002ec6]#5=1
    _RVM1(0x2ec6, BIT6, BIT6),   //reg[002ec6]#6=1
    _RVM1(0x2ec6, BIT7, BIT7),   //reg[002ec6]#7=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT4),   //reg[17033f]#4=0
    _RVM1(0x03a7, BIT4, BIT4),   //reg[1703a7]#4=1
    _RVM1(0x03a5, 0, BIT4),   //reg[1703a5]#4=0
    _RVM1(0x03a5, 0, BIT5),   //reg[1703a5]#5=0
    _RVM1(0x03a5, 0, BIT6),   //reg[1703a5]#6=0
    _RVM1(0x03a5, 0, BIT7),   //reg[1703a5]#7=0
    #endif

    #if(PORT_B_PAD_RX2N_IS_GPIO != GPIO_NONE)
    #define PORT_B_PAD_RX2N_OEN (PORT_B_PAD_RX2N_IS_GPIO == GPIO_IN ? 0 : BIT6)
    #define PORT_B_PAD_RX2N_IEN (PORT_B_PAD_RX2N_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PORT_B_PAD_RX2N_OUT (PORT_B_PAD_RX2N_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a6, PORT_B_PAD_RX2N_OUT, BIT2),
    _RVM1(0x03a4, PORT_B_PAD_RX2N_OEN, BIT6),
    _RVM1(0x03a7, PORT_B_PAD_RX2N_IEN, BIT2),
    _MEMMAP_PM_,
    _RVM1(0x2ec6, BIT4, BIT4),   //reg[002ec6]#4=1
    _RVM1(0x2ec6, BIT5, BIT5),   //reg[002ec6]#5=1
    _RVM1(0x2ec6, BIT6, BIT6),   //reg[002ec6]#6=1
    _RVM1(0x2ec6, BIT7, BIT7),   //reg[002ec6]#7=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT4),   //reg[17033f]#4=0
    _RVM1(0x03a7, BIT4, BIT4),   //reg[1703a7]#4=1
    _RVM1(0x03a5, 0, BIT4),   //reg[1703a5]#4=0
    _RVM1(0x03a5, 0, BIT5),   //reg[1703a5]#5=0
    _RVM1(0x03a5, 0, BIT6),   //reg[1703a5]#6=0
    _RVM1(0x03a5, 0, BIT7),   //reg[1703a5]#7=0
    #endif

    #if(PORT_B_PAD_RXCN_IS_GPIO != GPIO_NONE)
    #define PORT_B_PAD_RXCN_OEN (PORT_B_PAD_RXCN_IS_GPIO == GPIO_IN ? 0 : BIT7)
    #define PORT_B_PAD_RXCN_IEN (PORT_B_PAD_RXCN_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PORT_B_PAD_RXCN_OUT (PORT_B_PAD_RXCN_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a6, PORT_B_PAD_RXCN_OUT, BIT3),
    _RVM1(0x03a4, PORT_B_PAD_RXCN_OEN, BIT7),
    _RVM1(0x03a7, PORT_B_PAD_RXCN_IEN, BIT3),
    _MEMMAP_PM_,
    _RVM1(0x2ec6, BIT4, BIT4),   //reg[002ec6]#4=1
    _RVM1(0x2ec6, BIT5, BIT5),   //reg[002ec6]#5=1
    _RVM1(0x2ec6, BIT6, BIT6),   //reg[002ec6]#6=1
    _RVM1(0x2ec6, BIT7, BIT7),   //reg[002ec6]#7=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT4),   //reg[17033f]#4=0
    _RVM1(0x03a7, BIT4, BIT4),   //reg[1703a7]#4=1
    _RVM1(0x03a5, 0, BIT4),   //reg[1703a5]#4=0
    _RVM1(0x03a5, 0, BIT5),   //reg[1703a5]#5=0
    _RVM1(0x03a5, 0, BIT6),   //reg[1703a5]#6=0
    _RVM1(0x03a5, 0, BIT7),   //reg[1703a5]#7=0
    #endif

    #if(PORT_C_PAD_RX0P_IS_GPIO != GPIO_NONE)
    #define PORT_C_PAD_RX0P_OEN (PORT_C_PAD_RX0P_IS_GPIO == GPIO_IN ? 0 : BIT0)
    #define PORT_C_PAD_RX0P_IEN (PORT_C_PAD_RX0P_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PORT_C_PAD_RX0P_OUT (PORT_C_PAD_RX0P_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a9, PORT_C_PAD_RX0P_OUT, BIT0),
    _RVM1(0x03a8, PORT_C_PAD_RX0P_OEN, BIT0),
    _RVM1(0x03aa, PORT_C_PAD_RX0P_IEN, BIT4),
    _MEMMAP_PM_,
    _RVM1(0x2ec7, BIT0, BIT0),   //reg[002e64]#0=1
    _RVM1(0x2ec7, BIT1, BIT1),   //reg[002e64]#1=1
    _RVM1(0x2ec7, BIT2, BIT2),   //reg[002e64]#2=1
    _RVM1(0x2ec7, BIT3, BIT3),   //reg[002e64]#3=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT5),   //reg[17033f]#5=0
    _RVM1(0x03ab, BIT4, BIT4),   //reg[1703ab]#4=1
    _RVM1(0x03a9, 0, BIT4),   //reg[1703a9]#4=0
    _RVM1(0x03a9, 0, BIT5),   //reg[1703a9]#5=0
    _RVM1(0x03a9, 0, BIT6),   //reg[1703a9]#6=0
    _RVM1(0x03a9, 0, BIT7),   //reg[1703a9]#7=0
    #endif

    #if(PORT_C_PAD_RX1P_IS_GPIO != GPIO_NONE)
    #define PORT_C_PAD_RX1P_OEN (PORT_C_PAD_RX1P_IS_GPIO == GPIO_IN ? 0 : BIT1)
    #define PORT_C_PAD_RX1P_IEN (PORT_C_PAD_RX1P_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PORT_C_PAD_RX1P_OUT (PORT_C_PAD_RX1P_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a9, PORT_C_PAD_RX1P_OUT, BIT1),
    _RVM1(0x03a8, PORT_C_PAD_RX1P_OEN, BIT1),
    _RVM1(0x03aa, PORT_C_PAD_RX1P_IEN, BIT1),
    _MEMMAP_PM_,
    _RVM1(0x2ec7, BIT0, BIT0),   //reg[002e64]#0=1
    _RVM1(0x2ec7, BIT1, BIT1),   //reg[002e64]#1=1
    _RVM1(0x2ec7, BIT2, BIT2),   //reg[002e64]#2=1
    _RVM1(0x2ec7, BIT3, BIT3),   //reg[002e64]#3=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT5),   //reg[17033f]#5=0
    _RVM1(0x03ab, BIT4, BIT4),   //reg[1703ab]#4=1
    _RVM1(0x03a9, 0, BIT4),   //reg[1703a9]#4=0
    _RVM1(0x03a9, 0, BIT5),   //reg[1703a9]#5=0
    _RVM1(0x03a9, 0, BIT6),   //reg[1703a9]#6=0
    _RVM1(0x03a9, 0, BIT7),   //reg[1703a9]#7=0
    #endif

    #if(PORT_C_PAD_RX2P_IS_GPIO != GPIO_NONE)
    #define PORT_C_PAD_RX2P_OEN (PORT_C_PAD_RX2P_IS_GPIO == GPIO_IN ? 0 : BIT2)
    #define PORT_C_PAD_RX2P_IEN (PORT_C_PAD_RX2P_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PORT_C_PAD_RX2P_OUT (PORT_C_PAD_RX2P_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a9, PORT_C_PAD_RX2P_OUT, BIT2),
    _RVM1(0x03a8, PORT_C_PAD_RX2P_OEN, BIT2),
    _RVM1(0x03aa, PORT_C_PAD_RX2P_IEN, BIT6),
    _MEMMAP_PM_,
    _RVM1(0x2ec7, BIT0, BIT0),   //reg[002e64]#0=1
    _RVM1(0x2ec7, BIT1, BIT1),   //reg[002e64]#1=1
    _RVM1(0x2ec7, BIT2, BIT2),   //reg[002e64]#2=1
    _RVM1(0x2ec7, BIT3, BIT3),   //reg[002e64]#3=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT5),   //reg[17033f]#5=0
    _RVM1(0x03ab, BIT4, BIT4),   //reg[1703ab]#4=1
    _RVM1(0x03a9, 0, BIT4),   //reg[1703a9]#4=0
    _RVM1(0x03a9, 0, BIT5),   //reg[1703a9]#5=0
    _RVM1(0x03a9, 0, BIT6),   //reg[1703a9]#6=0
    _RVM1(0x03a9, 0, BIT7),   //reg[1703a9]#7=0
    #endif

    #if(PORT_C_PAD_RXCP_IS_GPIO != GPIO_NONE)
    #define PORT_C_PAD_RXCP_OEN (PORT_C_PAD_RXCP_IS_GPIO == GPIO_IN ? 0 : BIT3)
    #define PORT_C_PAD_RXCP_IEN (PORT_C_PAD_RXCP_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PORT_C_PAD_RXCP_OUT (PORT_C_PAD_RXCP_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03a9, PORT_C_PAD_RXCP_OUT, BIT3),
    _RVM1(0x03a8, PORT_C_PAD_RXCP_OEN, BIT3),
    _RVM1(0x03aa, PORT_C_PAD_RXCP_IEN, BIT7),
    _MEMMAP_PM_,
    _RVM1(0x2ec7, BIT0, BIT0),   //reg[002e64]#0=1
    _RVM1(0x2ec7, BIT1, BIT1),   //reg[002e64]#1=1
    _RVM1(0x2ec7, BIT2, BIT2),   //reg[002e64]#2=1
    _RVM1(0x2ec7, BIT3, BIT3),   //reg[002e64]#3=1
    _MEMMAP_nonBK17_,
    //power down MHL3_ATOP
    _RVM1(0x033f, 0, BIT5),   //reg[17033f]#5=0
    _RVM1(0x03ab, BIT4, BIT4),   //reg[1703ab]#4=1
    _RVM1(0x03a9, 0, BIT4),   //reg[1703a9]#4=0
    _RVM1(0x03a9, 0, BIT5),   //reg[1703a9]#5=0
    _RVM1(0x03a9, 0, BIT6),   //reg[1703a9]#6=0
    _RVM1(0x03a9, 0, BIT7),   //reg[1703a9]#7=0
    #endif

    #if(PORT_C_PAD_RX0N_IS_GPIO != GPIO_NONE)
    #define PORT_C_PAD_RX0N_OEN (PORT_C_PAD_RX0N_IS_GPIO == GPIO_IN ? 0 : BIT4)
    #define PORT_C_PAD_RX0N_IEN (PORT_C_PAD_RX0N_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PORT_C_PAD_RX0N_OUT (PORT_C_PAD_RX0N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03aa, PORT_C_PAD_RX0N_OUT, BIT0),
    _RVM1(0x03a8, PORT_C_PAD_RX0N_OEN, BIT4),
    _RVM1(0x03ab, PORT_C_PAD_RX0N_IEN, BIT0),
    _MEMMAP_PM_,
    _RVM1(0x2ec7, BIT0, BIT0),   //reg[002ec7]#0=1
    _RVM1(0x2ec7, BIT1, BIT1),   //reg[002ec7]#1=1
    _RVM1(0x2ec7, BIT2, BIT2),   //reg[002ec7]#2=1
    _RVM1(0x2ec7, BIT3, BIT3),   //reg[002ec7]#3=1
    _MEMMAP_nonBK17_,
    //power down MHL3_ATOP
    _RVM1(0x033f, 0, BIT5),   //reg[17033f]#5=0
    _RVM1(0x03ab, BIT4, BIT4),   //reg[1703ab]#4=1
    _RVM1(0x03a9, 0, BIT4),   //reg[1703a9]#4=0
    _RVM1(0x03a9, 0, BIT5),   //reg[1703a9]#5=0
    _RVM1(0x03a9, 0, BIT6),   //reg[1703a9]#6=0
    _RVM1(0x03a9, 0, BIT7),   //reg[1703a9]#7=0
    #endif

    #if(PORT_C_PAD_RX1N_IS_GPIO != GPIO_NONE)
    #define PORT_C_PAD_RX1N_OEN (PORT_C_PAD_RX1N_IS_GPIO == GPIO_IN ? 0 : BIT5)
    #define PORT_C_PAD_RX1N_IEN (PORT_C_PAD_RX1N_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PORT_C_PAD_RX1N_OUT (PORT_C_PAD_RX1N_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03aa, PORT_C_PAD_RX1N_OUT, BIT1),
    _RVM1(0x03a8, PORT_C_PAD_RX1N_OEN, BIT5),
    _RVM1(0x03ab, PORT_C_PAD_RX1N_IEN, BIT1),
     _MEMMAP_PM_,
    _RVM1(0x2ec7, BIT0, BIT0),   //reg[002ec7]#0=1
    _RVM1(0x2ec7, BIT1, BIT1),   //reg[002ec7]#1=1
    _RVM1(0x2ec7, BIT2, BIT2),   //reg[002ec7]#2=1
    _RVM1(0x2ec7, BIT3, BIT3),   //reg[002ec7]#3=1
    _MEMMAP_nonBK17_,
    //power down MHL3_ATOP
    _RVM1(0x033f, 0, BIT5),   //reg[17033f]#5=0
    _RVM1(0x03ab, BIT4, BIT4),   //reg[1703ab]#4=1
    _RVM1(0x03a9, 0, BIT4),   //reg[1703a9]#4=0
    _RVM1(0x03a9, 0, BIT5),   //reg[1703a9]#5=0
    _RVM1(0x03a9, 0, BIT6),   //reg[1703a9]#6=0
    _RVM1(0x03a9, 0, BIT7),   //reg[1703a9]#7=0
    #endif

    #if(PORT_C_PAD_RX2N_IS_GPIO != GPIO_NONE)
    #define PORT_C_PAD_RX2N_OEN (PORT_C_PAD_RX2N_IS_GPIO == GPIO_IN ? 0 : BIT6)
    #define PORT_C_PAD_RX2N_IEN (PORT_C_PAD_RX2N_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PORT_C_PAD_RX2N_OUT (PORT_C_PAD_RX2N_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03aa, PORT_C_PAD_RX2N_OUT, BIT2),
    _RVM1(0x03a8, PORT_C_PAD_RX2N_OEN, BIT6),
    _RVM1(0x03ab, PORT_C_PAD_RX2N_IEN, BIT2),
    _MEMMAP_PM_,
    _RVM1(0x2ec7, BIT0, BIT0),   //reg[002ec7]#0=1
    _RVM1(0x2ec7, BIT1, BIT1),   //reg[002ec7]#1=1
    _RVM1(0x2ec7, BIT2, BIT2),   //reg[002ec7]#2=1
    _RVM1(0x2ec7, BIT3, BIT3),   //reg[002ec7]#3=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT5),   //reg[17033f]#5=0
    _RVM1(0x03ab, BIT4, BIT4),   //reg[1703ab]#4=1
    _RVM1(0x03a9, 0, BIT4),   //reg[1703a9]#4=0
    _RVM1(0x03a9, 0, BIT5),   //reg[1703a9]#5=0
    _RVM1(0x03a9, 0, BIT6),   //reg[1703a9]#6=0
    _RVM1(0x03a9, 0, BIT7),   //reg[1703a9]#7=0
    #endif

    #if(PORT_C_PAD_RXCN_IS_GPIO != GPIO_NONE)
    #define PORT_C_PAD_RXCN_OEN (PORT_C_PAD_RXCN_IS_GPIO == GPIO_IN ? 0 : BIT7)
    #define PORT_C_PAD_RXCN_IEN (PORT_C_PAD_RXCN_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PORT_C_PAD_RXCN_OUT (PORT_C_PAD_RXCN_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03aa, PORT_C_PAD_RXCN_OUT, BIT3),
    _RVM1(0x03a8, PORT_C_PAD_RXCN_OEN, BIT7),
    _RVM1(0x03ab, PORT_C_PAD_RXCN_IEN, BIT3),
    _MEMMAP_PM_,
    _RVM1(0x2ec7, BIT0, BIT0),   //reg[002ec7]#0=1
    _RVM1(0x2ec7, BIT1, BIT1),   //reg[002ec7]#1=1
    _RVM1(0x2ec7, BIT2, BIT2),   //reg[002ec7]#2=1
    _RVM1(0x2ec7, BIT3, BIT3),   //reg[002ec7]#3=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT5),   //reg[17033f]#5=0
    _RVM1(0x03ab, BIT4, BIT4),   //reg[1703ab]#4=1
    _RVM1(0x03a9, 0, BIT4),   //reg[1703a9]#4=0
    _RVM1(0x03a9, 0, BIT5),   //reg[1703a9]#5=0
    _RVM1(0x03a9, 0, BIT6),   //reg[1703a9]#6=0
    _RVM1(0x03a9, 0, BIT7),   //reg[1703a9]#7=0
    #endif

    #if(PORT_D_PAD_RX0P_IS_GPIO != GPIO_NONE)
    #define PORT_D_PAD_RX0P_OEN (PORT_D_PAD_RX0P_IS_GPIO == GPIO_IN ? 0 : BIT0)
    #define PORT_D_PAD_RX0P_IEN (PORT_D_PAD_RX0P_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PORT_D_PAD_RX0P_OUT (PORT_D_PAD_RX0P_IS_GPIO == GPIO_OUT_HIGH ? BIT0 : 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03ad, PORT_D_PAD_RX0P_OUT, BIT0 ),
    _RVM1(0x03ac, PORT_D_PAD_RX0P_OEN, BIT0),
    _RVM1(0x03ae, PORT_D_PAD_RX0P_OEN, BIT4),
    _MEMMAP_PM_,
    _RVM1(0x2ec7, BIT4, BIT4),   //reg[002ec7]#4=1
    _RVM1(0x2ec7, BIT5, BIT5),   //reg[002ec7]#5=1
    _RVM1(0x2ec7, BIT6, BIT6),   //reg[002ec7]#6=1
    _RVM1(0x2ec7, BIT7, BIT7),   //reg[002ec7]#7=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT6),   //reg[17033f]#6=0
    _RVM1(0x03af, BIT4, BIT4),   //reg[1703af]#4=1
    _RVM1(0x03ad, 0, BIT4),   //reg[1703ad]#4=0
    _RVM1(0x03ad, 0, BIT5),   //reg[1703ad]#5=0
    _RVM1(0x03ad, 0, BIT6),   //reg[1703ad]#6=0
    _RVM1(0x03ad, 0, BIT7),   //reg[1703ad]#7=0
    #endif

    #if(PORT_D_PAD_RX1P_IS_GPIO != GPIO_NONE)
    #define PORT_D_PAD_RX1P_OEN (PORT_D_PAD_RX1P_IS_GPIO == GPIO_IN ? 0 : BIT1)
    #define PORT_D_PAD_RX1P_IEN (PORT_D_PAD_RX1P_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PORT_D_PAD_RX1P_OUT (PORT_D_PAD_RX1P_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03ad, PORT_D_PAD_RX1P_OUT, BIT1),
    _RVM1(0x03ac, PORT_D_PAD_RX1P_OEN, BIT1),
    _RVM1(0x03ae, PORT_D_PAD_RX1P_IEN, BIT5),
    _MEMMAP_PM_,
    _RVM1(0x2ec7, BIT4, BIT4),   //reg[002ec7]#4=1
    _RVM1(0x2ec7, BIT5, BIT5),   //reg[002ec7]#5=1
    _RVM1(0x2ec7, BIT6, BIT6),   //reg[002ec7]#6=1
    _RVM1(0x2ec7, BIT7, BIT7),   //reg[002ec7]#7=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT6),   //reg[17033f]#6=0
    _RVM1(0x03af, BIT4, BIT4),   //reg[1703af]#4=1
    _RVM1(0x03ad, 0, BIT4),   //reg[1703ad]#4=0
    _RVM1(0x03ad, 0, BIT5),   //reg[1703ad]#5=0
    _RVM1(0x03ad, 0, BIT6),   //reg[1703ad]#6=0
    _RVM1(0x03ad, 0, BIT7),   //reg[1703ad]#7=0
    #endif

    #if(PORT_D_PAD_RX2P_IS_GPIO != GPIO_NONE)
    #define PORT_D_PAD_RX2P_OEN (PORT_D_PAD_RX2P_IS_GPIO == GPIO_IN ? 0 : BIT2)
    #define PORT_D_PAD_RX2P_IEN (PORT_D_PAD_RX2P_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PORT_D_PAD_RX2P_OUT (PORT_D_PAD_RX2P_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03ad, PORT_D_PAD_RX2P_OUT, BIT2),
    _RVM1(0x03ac, PORT_D_PAD_RX2P_OEN, BIT2),
    _RVM1(0x03ae, PORT_D_PAD_RX2P_IEN, BIT6),
    _MEMMAP_PM_,
    _RVM1(0x2ec7, BIT4, BIT4),   //reg[002ec7]#4=1
    _RVM1(0x2ec7, BIT5, BIT5),   //reg[002ec7]#5=1
    _RVM1(0x2ec7, BIT6, BIT6),   //reg[002ec7]#6=1
    _RVM1(0x2ec7, BIT7, BIT7),   //reg[002ec7]#7=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT6),   //reg[17033f]#6=0
    _RVM1(0x03af, BIT4, BIT4),   //reg[1703af]#4=1
    _RVM1(0x03ad, 0, BIT4),   //reg[1703ad]#4=0
    _RVM1(0x03ad, 0, BIT5),   //reg[1703ad]#5=0
    _RVM1(0x03ad, 0, BIT6),   //reg[1703ad]#6=0
    _RVM1(0x03ad, 0, BIT7),   //reg[1703ad]#7=0
    #endif

    #if(PORT_D_PAD_RXCP_IS_GPIO != GPIO_NONE)
    #define PORT_D_PAD_RXCP_OEN (PORT_D_PAD_RXCP_IS_GPIO == GPIO_IN ? 0 : BIT3)
    #define PORT_D_PAD_RXCP_IEN (PORT_D_PAD_RXCP_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PORT_D_PAD_RXCP_OUT (PORT_D_PAD_RXCP_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03ad, PORT_D_PAD_RXCP_OUT, BIT3),
    _RVM1(0x03ac, PORT_D_PAD_RXCP_OEN, BIT3),
    _RVM1(0x03ae, PORT_D_PAD_RXCP_IEN, BIT7),
    _MEMMAP_PM_,
    _RVM1(0x2ec7, BIT4, BIT4),   //reg[002ec7]#4=1
    _RVM1(0x2ec7, BIT5, BIT5),   //reg[002ec7]#5=1
    _RVM1(0x2ec7, BIT6, BIT6),   //reg[002ec7]#6=1
    _RVM1(0x2ec7, BIT7, BIT7),   //reg[002ec7]#7=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT6),   //reg[17033f]#6=0
    _RVM1(0x03af, BIT4, BIT4),   //reg[1703af]#4=1
    _RVM1(0x03ad, 0, BIT4),   //reg[1703ad]#4=0
    _RVM1(0x03ad, 0, BIT5),   //reg[1703ad]#5=0
    _RVM1(0x03ad, 0, BIT6),   //reg[1703ad]#6=0
    _RVM1(0x03ad, 0, BIT7),   //reg[1703ad]#7=0
    #endif

    #if(PORT_D_PAD_RX0N_IS_GPIO != GPIO_NONE)
    #define PORT_D_PAD_RX0N_OEN (PORT_D_PAD_RX0N_IS_GPIO == GPIO_IN ? 0 : BIT4)
    #define PORT_D_PAD_RX0N_IEN (PORT_D_PAD_RX0N_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PORT_D_PAD_RX0N_OUT (PORT_D_PAD_RX0N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03ae, PORT_D_PAD_RX0N_OUT, BIT0),
    _RVM1(0x03ac, PORT_D_PAD_RX0N_OEN, BIT4),
    _RVM1(0x03af, PORT_D_PAD_RX0N_IEN, BIT0),
    _MEMMAP_PM_,
    _RVM1(0x2ec7, BIT4, BIT4),   //reg[002ec7]#4=1
    _RVM1(0x2ec7, BIT5, BIT5),   //reg[002ec7]#5=1
    _RVM1(0x2ec7, BIT6, BIT6),   //reg[002ec7]#6=1
    _RVM1(0x2ec7, BIT7, BIT7),   //reg[002ec7]#7=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT6),   //reg[17033f]#6=0
    _RVM1(0x03af, BIT4, BIT4),   //reg[1703af]#4=1
    _RVM1(0x03ad, 0, BIT4),   //reg[1703ad]#4=0
    _RVM1(0x03ad, 0, BIT5),   //reg[1703ad]#5=0
    _RVM1(0x03ad, 0, BIT6),   //reg[1703ad]#6=0
    _RVM1(0x03ad, 0, BIT7),   //reg[1703ad]#7=0
    #endif

    #if(PORT_D_PAD_RX1N_IS_GPIO != GPIO_NONE)
    #define PORT_D_PAD_RX1N_OEN (PORT_D_PAD_RX1N_IS_GPIO == GPIO_IN ? 0 : BIT5)
    #define PORT_D_PAD_RX1N_IEN (PORT_D_PAD_RX1N_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PORT_D_PAD_RX1N_OUT (PORT_D_PAD_RX1N_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03ae, PORT_D_PAD_RX1N_OUT, BIT1),
    _RVM1(0x03ac, PORT_D_PAD_RX1N_OEN, BIT5),
    _RVM1(0x03af, PORT_D_PAD_RX1N_IEN, BIT1),
    _MEMMAP_PM_,
    _RVM1(0x2ec7, BIT4, BIT4),   //reg[002e64]#4=1
    _RVM1(0x2ec7, BIT5, BIT5),   //reg[002e64]#5=1
    _RVM1(0x2ec7, BIT6, BIT6),   //reg[002e64]#6=1
    _RVM1(0x2ec7, BIT7, BIT7),   //reg[002e64]#7=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT6),   //reg[17033f]#6=0
    _RVM1(0x03af, BIT4, BIT4),   //reg[1703af]#4=1
    _RVM1(0x03ad, 0, BIT4),   //reg[1703ad]#4=0
    _RVM1(0x03ad, 0, BIT5),   //reg[1703ad]#5=0
    _RVM1(0x03ad, 0, BIT6),   //reg[1703ad]#6=0
    _RVM1(0x03ad, 0, BIT7),   //reg[1703ad]#7=0
    #endif

    #if(PORT_D_PAD_RX2N_IS_GPIO != GPIO_NONE)
    #define PORT_D_PAD_RX2N_OEN (PORT_D_PAD_RX2N_IS_GPIO == GPIO_IN ? 0 : BIT6)
    #define PORT_D_PAD_RX2N_IEN (PORT_D_PAD_RX2N_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PORT_D_PAD_RX2N_OUT (PORT_D_PAD_RX2N_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03ae, PORT_D_PAD_RX2N_OUT, BIT2),
    _RVM1(0x03ac, PORT_D_PAD_RX2N_OEN, BIT6),
    _RVM1(0x03af, PORT_D_PAD_RX2N_IEN, BIT3),
    _MEMMAP_PM_,
    _RVM1(0x2ec7, BIT4, BIT4),   //reg[002e64]#4=1
    _RVM1(0x2ec7, BIT5, BIT5),   //reg[002e64]#5=1
    _RVM1(0x2ec7, BIT6, BIT6),   //reg[002e64]#6=1
    _RVM1(0x2ec7, BIT7, BIT7),   //reg[002e64]#7=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT6),   //reg[17033f]#6=0
    _RVM1(0x03af, BIT4, BIT4),   //reg[1703af]#4=1
    _RVM1(0x03ad, 0, BIT4),   //reg[1703ad]#4=0
    _RVM1(0x03ad, 0, BIT5),   //reg[1703ad]#5=0
    _RVM1(0x03ad, 0, BIT6),   //reg[1703ad]#6=0
    _RVM1(0x03ad, 0, BIT7),   //reg[1703ad]#7=0
    #endif

    #if(PORT_D_PAD_RXCN_IS_GPIO != GPIO_NONE)
    #define PORT_D_PAD_RXCN_OEN (PORT_D_PAD_RXCN_IS_GPIO == GPIO_IN ? 0 : BIT7)
    #define PORT_D_PAD_RXCN_IEN (PORT_D_PAD_RXCN_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PORT_D_PAD_RXCN_OUT (PORT_D_PAD_RXCN_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _MEMMAP_nonBK17_,
    _RVM1(0x03ae, PORT_D_PAD_RXCN_OUT, BIT3),
    _RVM1(0x03ac, PORT_D_PAD_RXCN_OEN, BIT7),
    _RVM1(0x03af, PORT_D_PAD_RXCN_IEN, BIT4),
    _MEMMAP_PM_,
    _RVM1(0x2ec7, BIT4, BIT4),   //reg[002e64]#4=1
    _RVM1(0x2ec7, BIT5, BIT5),   //reg[002e64]#5=1
    _RVM1(0x2ec7, BIT6, BIT6),   //reg[002e64]#6=1
    _RVM1(0x2ec7, BIT7, BIT7),   //reg[002e64]#7=1
    _MEMMAP_nonBK17_,
    _RVM1(0x033f, 0, BIT6),   //reg[17033f]#6=0
    _RVM1(0x03af, BIT4, BIT4),   //reg[1703af]#4=1
    _RVM1(0x03ad, 0, BIT4),   //reg[1703ad]#4=0
    _RVM1(0x03ad, 0, BIT5),   //reg[1703ad]#5=0
    _RVM1(0x03ad, 0, BIT6),   //reg[1703ad]#6=0
    _RVM1(0x03ad, 0, BIT7),   //reg[1703ad]#7=0
    #endif

//---------------------------------------------------------------------
// Pad Configuartion
//---------------------------------------------------------------------
    _MEMMAP_nonPM_,

#if (PADS_TCON_CONFIG != Unknown_pad_mux)
#define TCON0    ((PADS_TCON_CONFIG & BIT0) ? BIT0 : 0)
#define TCON1    ((PADS_TCON_CONFIG & BIT1) ? BIT1 : 0)
#define TCON2    ((PADS_TCON_CONFIG & BIT2) ? BIT2 : 0)
#define TCON3    ((PADS_TCON_CONFIG & BIT3) ? BIT3 : 0)
#define TCON4    ((PADS_TCON_CONFIG & BIT4) ? BIT4 : 0)
#define TCON5    ((PADS_TCON_CONFIG & BIT5) ? BIT5 : 0)
#define TCON6    ((PADS_TCON_CONFIG & BIT6) ? BIT6 : 0)
#define TCON7    ((PADS_TCON_CONFIG & BIT7) ? BIT7 : 0)
#define TCON8    ((PADS_TCON_CONFIG & BIT8) ? BIT8 : 0)
#define TCON9    ((PADS_TCON_CONFIG & BIT9) ? BIT9 : 0)
#define TCON10   ((PADS_TCON_CONFIG & BIT10) ? BIT10 : 0)
#define TCON11   ((PADS_TCON_CONFIG & BIT11) ? BIT11 : 0)
#define TCON12   ((PADS_TCON_CONFIG & BIT11) ? BIT12 : 0)
#define TCON13   ((PADS_TCON_CONFIG & BIT11) ? BIT13 : 0)
#define TCON14   ((PADS_TCON_CONFIG & BIT11) ? BIT14 : 0)
#define TCON15   ((PADS_TCON_CONFIG & BIT11) ? BIT15 : 0)
#define TCON16   ((PADS_TCON_CONFIG & BIT11) ? BIT16 : 0)
#define TCON17   ((PADS_TCON_CONFIG & BIT11) ? BIT17 : 0)
#define TCON18   ((PADS_TCON_CONFIG & BIT11) ? BIT18 : 0)
#define TCON19   ((PADS_TCON_CONFIG & BIT11) ? BIT19 : 0)
#define TCON20   ((PADS_TCON_CONFIG & BIT11) ? BIT20 : 0)
#define TCON21   ((PADS_TCON_CONFIG & BIT11) ? BIT21 : 0)
#define SET_TCON TCON0|TCON1|TCON2|TCON3|TCON4|TCON5|TCON6|TCON7|TCON8|TCON9|TCON10|TCON11|TCON12|TCON13|TCON14|TCON15|TCON16|TCON17|TCON18|TCON19|TCON20|TCON21
        _RVM3(0x1EF2, SET_TCON, 0x3FFFFF),
#endif

#if (PADS_UART2_MODE != Unknown_pad_mux)
//2.	TX1/RX1
//Path 1:  PAD_GPIO0/PAD_GPIO1
#define UART2_MODE   ((PADS_UART2_MODE == PAD_GPIO0) ? BIT0: \
                      (PADS_UART2_MODE == PAD_GPIO1) ? BIT0: 0)
        _RVM1(0x1E05, UART2_MODE, BIT1|BIT0),
#endif

#if (PADS_UART3_MODE != Unknown_pad_mux)
//3.	TX2/RX2
//Path 1:  PAD_GPIO8/PAD_GPIO9
//Path 2  PAD_GPIO10/PAD_GPIO11
#define UART3_MODE   ((PADS_UART3_MODE == PAD_GPIO8) ? BIT2: \
                      (PADS_UART3_MODE == PAD_GPIO9) ? BIT2: \
                      (PADS_UART4_MODE == PAD_GPIO10) ? BIT3: \
                      (PADS_UART4_MODE == PAD_GPIO11) ? BIT3: 0)
)
        _RVM1(0x1E05, UART3_MODE, BIT3|BIT2),
#endif

#if (PADS_UART4_MODE != Unknown_pad_mux)
//4.	TX3/RX3
//Path 1:  PAD_TCON11/PAD_GPIO5
//Path 2:  PAD_TCON2/ PAD_TCON3
#define UART4_MODE   ((PADS_UART4_MODE == PAD_TCON11) ? BIT6: \
                      (PADS_UART4_MODE == PAD_GPIO5) ? BIT6: \
                      (PADS_UART4_MODE == PAD_TCON2) ? BIT7: \
                      (PADS_UART4_MODE == PAD_TCON3) ? BIT7: 0)
        _RVM1(0x1E04, UART4_MODE, BIT7|BIT6),
#endif

#if (PADS_FAST_UART_MODE != Unknown_pad_mux)
//5.	CTS0/RTS0
//Path 1:  PAD_TCON0/PAD_TCON1
#define FAST_UART_MODE   ((PADS_FAST_UART_MODE == PAD_TCON0) ? BIT4: \
                          (PADS_FAST_UART_MODE == PAD_TCON1) ? BIT4: 0)
        _RVM1(0x1E04, FAST_UART_MODE, BIT5|BIT4),
#endif

#define UART_INV    ((UART0_INV ? BIT0 : 0) | \
                     (UART1_INV ? BIT1 : 0) | \
                     (UART2_INV ? BIT2 : 0) | \
                     (UART3_INV ? BIT3 : 0) | \
                     (UART4_INV ? BIT4 : 0))

        _RVM1(0x1EAB, UART_INV, BITMASK(4:0)),
        _RVM1(0x1EA6, (UART1_SRC_SEL << 4) | (UART0_SRC_SEL << 0), BITMASK(7:0)),
        _RVM1(0x1EA7, (UART3_SRC_SEL << 4) | (UART2_SRC_SEL << 0), BITMASK(7:0)),
        _RVM1(0x1EA8, (UART4_SRC_SEL << 0), BITMASK(3:0)),

#if (PADS_3DLR_MODE != Unknown_pad_mux)
//XC3DLR_PATH1 : PAD_TCON0
//XC3DLR_PATH2 : PAD_GPIO0
//XC3DLR_VSYNC_PATH1 : PAD_TCON0 (scaler out with VSync)
//XC3DLR_VSYNC_PATH2 : PAD_GPIO0 (scaler out with VSync)
#define LR_3D_MODE   ((PADS_3DLR_MODE == XC3DLR_PATH1) ? BIT0: \
                      (PADS_3DLR_MODE == XC3DLR_PATH2) ? BIT1: \
                      (PADS_3DLR_MODE == XC3DLR_VSYNC_PATH1) ? (BIT1|BIT0): \
                      (PADS_3DLR_MODE == XC3DLR_VSYNC_PATH2) ? BIT2: 0)
        _RVM1(0x1E10, LR_3D_MODE, BIT2|BIT1|BIT0),
#endif

#if (PADS_8BIT_TTL != Unknown_pad_mux)
//TTL_8BIT_MODE : PAD_TCON0, PAD_TCON1, PAD_TCON2, PAD_TCON3
#define TTL_8BIT    ((PADS_8BIT_TTL == TTL_8BIT_MODE) ? BIT1 : 0)
        _RVM1(0x1EF8, TTL_8BIT, BIT1),
#endif

#if (PADS_6BIT_TTL != Unknown_pad_mux)
//TTL_6BIT_MODE : PAD_GPIO0, PAD_GPIO1
#define TTL_6BIT    ((PADS_6BIT_TTL == TTL_6BIT_MODE) ? BIT0 : 0)
        _RVM1(0x1EF8, TTL_6BIT, BIT0),
#endif

#if (PADS_USBDRVVBUS != Unknown_pad_mux)
//USB DRV Bus Configuration
//01:  use {PAD_TCON33, PAD_TCON4}
//10:  use {PAD_GPIO3, PAD_GPIO4}
//11:  use {PAD_PCM2_RESET, PAD_PCM2_CD_N}
#define USBDRVVBUS ((PADS_USBDRVVBUS == USB_TCON) ? (BIT0) : \
                    (PADS_USBDRVVBUS == USB_GPIO) ? (BIT1) : \
                    (PADS_USBDRVVBUS == USB_PCM2) ? (BIT1|BIT0) : 0)
        _RVM1(0x1E04, USBDRVVBUS, BIT1|BIT0)
#endif

#if (PADS_MHL_CBUS != Unknown_pad_mux)
//MHL_CBUS_MODE1 : PAD_HOTPLUG_A
//MHL_CBUS_MODE2 : PAD_HOTPLUG_B
//MHL_CBUS_MODE3 : PAD_HOTPLUG_C
#define MHL_CBUS  ((PADS_MHL_CBUS == MHL_CBUS_MODE1) ? BIT0 : \
                   (PADS_MHL_CBUS == MHL_CBUS_MODE2) ? BIT1 : \
                   (PADS_MHL_CBUS == MHL_CBUS_MODE3) ? (BIT1|BIT0) : 0)
        _RVM1(0x1EFA, MHL_CBUS, BIT1|BIT0),
        _MEMMAP_PM_,
        _RVM1(0x2F40, 0, BIT3), //oen
        _RVM1(0x2F40, 0, BIT4), //output low for hot plug
        _MEMMAP_nonPM_,
#endif

#if (PADS_MHL_VBUS != Unknown_pad_mux)
//MHL_VBUS_MODE1 : PAD_GPIO2
//MHL_VBUS_MODE2 : PAD_GPIO0
//MHL_VBUS_MODE3 : PAD_PWM0
//MHL_VBUS_MODE4 : PAD_PWM1
#define MHL_VBUS  ((PADS_MHL_VBUS == MHL_VBUS_MODE1) ? BIT5 : \
                   (PADS_MHL_VBUS == MHL_VBUS_MODE2) ? BIT6 : \
                   (PADS_MHL_VBUS == MHL_VBUS_MODE3) ? (BIT6|BIT5) : \
                   (PADS_MHL_VBUS == MHL_VBUS_MODE4) ? BIT7 : 0)
        _RVM1(0x1EF9, MHL_VBUS, BIT7|BIT6|BIT5),
#endif

#if (PADS_MHL_DEBUG != Unknown_pad_mux)
#define MHL_DEBUG ((PADS_MHL_DEBUG == PAD_GPIO0) ? BIT4 : 0)
        _RVM1(0x1EF9, MHL_DEBUG, BIT4),
#endif

#if (PADS_MHL_DET != Unknown_pad_mux)
#define MHL_DET ((PADS_MHL_DET == PAD_MHL_DET) ? BIT0 : 0)
        _RVM1(0x1EF9, MHL_DET, BIT0),
#endif


#if (PADS_DDCDA != Unknown_pad_mux)
// DDCDA_ENABLE : PAD_DDCDA_CLK, PAD_DDCDA_DAT
#define DDCDA ((PADS_DDCDA == DDCDA_ENABLE) ? BIT1 : 0)
        _RVM1(0x1EF9, DDCDA, BIT1),
#endif

#if (PADS_DDCDB != Unknown_pad_mux)
// DDCDB_ENABLE : PAD_DDCDB_CLK, PAD_DDCDB_DAT
#define DDCDB ((PADS_DDCDB == DDCDB_ENABLE) ? BIT2 : 0)
        _RVM1(0x1EF9, DDCDB, BIT2),
#endif

#if (PADS_DDCDC != Unknown_pad_mux)
// DDCDC_ENABLE : PAD_DDCDC_CLK, PAD_DDCDC_DAT
#define DDCDC ((PADS_DDCDC == DDCDC_ENABLE) ? BIT3 : 0)
        _RVM1(0x1EF9, DDCDC, BIT3),
#endif

#if (PADS_DDCDD != Unknown_pad_mux)
// DDCDC_ENABLE : PAD_DDCDD_CLK, PAD_DDCDD_DAT
#define DDCDD ((PADS_DDCDD == DDCDD_ENABLE) ? BIT7 : 0)
        _RVM1(0x1EFB, DDCDD, BIT7),
#endif

#if (PADS_I2S_IN != Unknown_pad_mux)
//I2S_IN_MODE1 : PAD_GPIO6, PAD_GPIO8, PAD_GPIO9
#define I2S_IN    ((PADS_I2S_IN == I2S_IN_MODE1) ? BIT3 : 0)
        _RVM1(0x1EAE, I2S_IN, BIT4|BIT3),
#endif

#if (PADS_I2S_OUT != Unknown_pad_mux)
//I2S_OUT_MODE1 : PAD_GPIO4, PAD_GPIO6, PAD_GPIO7, PAD_GPIO8, PAD_GPIO9
#define I2S_OUT   ((PADS_I2S_OUT == I2S_OUT_MODE1) ? BIT5 : 0)
#define I2S_OUT2  ((PADS_I2S_OUT == I2S_OUT_MODE2) ? BIT2 : \
                   (PADS_I2S_OUT == I2S_OUT_MODE3) ? BIT3 : \
                   (PADS_I2S_OUT == I2S_OUT_MODE4) ? BIT4 : 0)
        _RVM1(0x1EAE, I2S_OUT, BIT6|BIT5),
        _RVM1(0x1E3F, I2S_OUT2, BIT4|BIT3|BIT2),
#endif

#if (PADS_SPDIF_IN != Unknown_pad_mux)
//b"01:PAD_GPIO7
//b"10:PAD_ARC
//b"11:PAD_GPIO10
#define SPDIF_IN   ((PADS_SPDIF_IN == PAD_GPIO7) ? BIT7 : \
                    (PADS_SPDIF_IN == PAD_ARC) ? BIT0: \
                    (PADS_SPDIF_IN == PAD_GPIO10) ? (BIT0|BIT7): 0)
        _RVM2(0x1EAE, SPDIF_IN, BIT0|BIT7),
#endif

#if (PADS_SPDIF_OUT != Unknown_pad_mux)
#define SPDIF_OUT   ((PADS_SPDIF_OUT == PAD_GPIO5) ? BIT1 : 0)
#define SPDIF_OUT2  ((PADS_SPDIF_OUT == PAD_PWM1) ? BIT5 : \
                     (PADS_SPDIF_OUT == PAD_GPIO10) ? BIT6 : \
                     (PADS_SPDIF_OUT == PAD_INT) ? BIT7 : 0)
        _RVM1(0x1EAF, SPDIF_OUT, BIT1),
        _RVM1(0x1E3F, SPDIF_OUT2, BIT7|BIT6|BIT5),
#endif

#if (PADS_INT_GPIO != Unknown_pad_mux)
#define INT_GPIO   ((PADS_INT_GPIO == PAD_INT) ? BIT2: 0)
        _RVM1(0x1E4F, INT_GPIO, BIT2),
#endif

#if (PADS_PWM0_MODE != Unknown_pad_mux)
#define PWM0_MODE   ((PADS_PWM0_MODE == PAD_PWM0) ? BIT4: 0)
        _RVM1(0x1E40, PWM0_MODE, BIT4),
        _RVM1(0x1E06, 0, BIT0),//pwm0 output enable
#endif

#if (PADS_PWM1_MODE != Unknown_pad_mux)
#define PWM1_MODE   ((PADS_PWM1_MODE == PAD_PWM1) ? BIT5: 0)
        _RVM1(0x1E40, PWM1_MODE, BIT5),
        _RVM1(0x1E06, 0, BIT1),//pwm1 output enable
#endif

#if (PADS_PWM2_MODE != Unknown_pad_mux)
#define PWM2_MODE   ((PADS_PWM2_MODE == PAD_PWM2) ? BIT6: 0)
        _RVM1(0x1E40, PWM2_MODE, BIT6),
        _RVM1(0x1E06, 0, BIT2),//pwm2 output enable
#endif

#if (PADS_PWM3_MODE != Unknown_pad_mux)
#define PWM3_MODE   ((PADS_PWM3_MODE == PAD_PWM3) ? BIT7: 0)
        _RVM1(0x1E40, PWM3_MODE, BIT7),
        _RVM1(0x1E06, 0, BIT3),//pwm3 output enable
#endif

#if (PADS_PWM4_MODE != Unknown_pad_mux)
#define PWM4_MODE   ((PADS_PWM4_MODE == PAD_GPIO7) ? BIT0: 0)
        _RVM1(0x1E41, PWM4_MODE, BIT1|BIT0),
        _RVM1(0x1E06, 0, BIT4),//pwm4 output enable
#endif

#if (PADS_PWM5_MODE != Unknown_pad_mux)
#define PWM5_MODE   ((PADS_PWM5_MODE == PAD_GPIO6) ? BIT2: 0)
        _RVM1(0x1E41, PWM5_MODE, BIT3|BIT2),
        _RVM1(0x1E06, 0, BIT5),//pwm5 output enable
#endif

#if (PADS_DDCR_MODE != Unknown_pad_mux)
#define DDCR_MODE   ((PADS_DDCR_MODE == IIC_PATH(1)) ? BIT0: \
                     (PADS_DDCR_MODE == IIC_PATH(2)) ? (BIT0|BIT1): \
                     (PADS_DDCR_MODE == IIC_PATH(3)) ? (BIT2|BIT0): \
                     (PADS_DDCR_MODE == IIC_PATH(4)) ? (BIT2|BIT1|BIT0): \
                     (PADS_DDCR_MODE == DDCR_PATH(1)) ? BIT1: \
                     (PADS_DDCR_MODE == DDCR_PATH(2)) ? BIT2: \
                     (PADS_DDCR_MODE == DDCR_PATH(3)) ? (BIT2|BIT1): \
                     (PADS_DDCR_MODE == DDCR_PATH(4)) ? BIT3: 0)
        _RVM1(0x1EF6, DDCR_MODE, BIT3|BIT2|BIT1|BIT0),
#endif

#if (PADS_TS_IN_P_CFG != Unknown_pad_mux)
//TS_IN_P_MODE(1) : PAD_TCON0 ~ PAD_TCON10
//TS_IN_P_MODE(2) : PAD_GPIO9 ~ PAD_GPIO19
#define TS_IN_P_CFG ((PADS_TS_IN_P_CFG == TS_IN_P_MODE(1)) ? BIT4 : \
                     (PADS_TS_IN_P_CFG == TS_IN_P_MODE(2)) ? BIT5 : 0)
        _RVM1(0x1EF7, TS_IN_P_CFG, BIT5|BIT4),
#endif

#if (PADS_TS_IN_S_CFG != Unknown_pad_mux)
//TS_IN_S_MODE(1) : PAD_TCON0, PAD_TCON8, PAD_TCON9, PAD_TCON10
//TS_IN_S_MODE(2) : PAD_TCON4, PAD_TCON5, PAD_TCON6, PAD_TCON7
#define TS_IN_S_CFG ((PADS_TS_IN_S_CFG == TS_IN_S_MODE(1)) ? BIT0 : \
                     (PADS_TS_IN_S_CFG == TS_IN_S_MODE(2)) ? BIT1 : \
                     (PADS_TS_IN_S_CFG == TS_IN_S_MODE(3)) ? (BIT1|BIT0) :0)
        _RVM1(0x1EF7, TS_IN_S_CFG, BIT3|BIT2|BIT1|BIT0),
#endif

#if (PADS_VSYNC_LIKE != Unknown_pad_mux)
#define VSYNC_LIKE  ((PADS_VSYNC_LIKE == VSYNC_LIKE_MODE(1)) ? BIT5 : \
                     (PADS_VSYNC_LIKE == VSYNC_LIKE_MODE(2)) ? BIT6 : \
                     (PADS_VSYNC_LIKE == VSYNC_LIKE_MODE(3)) ? (BIT6|BIT5) : \
                     (PADS_VSYNC_LIKE == VSYNC_LIKE_MODE(4)) ? BIT7 : \
                     (PADS_VSYNC_LIKE == VSYNC_LIKE_MODE(5)) ? (BIT7|BIT5) : \
                     (PADS_VSYNC_LIKE == VSYNC_LIKE_MODE(6)) ? (BIT7|BIT6) : \
                     (PADS_VSYNC_LIKE == VSYNC_LIKE_MODE(7)) ? (BIT7|BIT6|BIT5) : 0)
//        _RVM1(0x1E05, VSYNC_LIKE, BIT7|BIT6|BIT5),
#endif

#if (PADS_MSPI0 != Unknown_pad_mux)
#define MSPI0_SET   ( (PADS_MSPI0 == MSPI0_MODE(1)) ? BIT4 : \
                     (PADS_MSPI0 == MSPI0_MODE(2)) ? BIT5 : 0)
        _RVM1(0x1E07, MSPI0_SET, BIT5|BIT4),
#endif

#if (PADS_MSPI1 != Unknown_pad_mux)
#define MSPI1_SET   ( (PADS_MSPI1 == MSPI1_MODE(1)) ? BIT6 : \
                     (PADS_MSPI1 == MSPI1_MODE(2)) ? BIT7 : 0)
        _RVM1(0x1E07, MSPI1_SET, BIT7|BIT6),
#endif

#if (PADS_TS_OUT_P_CFG != Unknown_pad_mux)
//TS_OUT_P_MODE(1) :
//TS_OUT_P_MODE(2) :
//TS_OUT_P_MODE(3) :
//TS_OUT_P_MODE(4) :
#define TS_OUT_P_CFG ((PADS_TS_OUT_P_CFG == TS_OUT_P_MODE(1)) ? BIT0 : \
                      (PADS_TS_OUT_P_CFG == TS_OUT_P_MODE(2)) ? BIT1 : \
                      (PADS_TS_OUT_P_CFG == TS_OUT_P_MODE(3)) ? (BIT1|BIT0) : \
                      (PADS_TS_OUT_P_CFG == TS_OUT_P_MODE(4)) ? BIT2 : 0)
        _RVM1(0x1E1C, TS_OUT_P_CFG, BIT2|BIT1|BIT0),
#endif

#if (PADS_CEC_CONFIG != Unknown_pad_mux)
    _MEMMAP_PM_,
    _RVM1(0x3401, BIT6, BIT6 ),   //reg[3401]#6 = 1b
#endif

#if (PADS_MHL_PORT_CFG!= Unknown_pad_mux)
       _MEMMAP_PM_,
#define MHL_PORT_MODE  ((PADS_MHL_PORT_CFG == MHL_PORT_MODE_HIGH) ? (BIT4|BIT3) : 0)
        _RVM1(0x2F40, MHL_PORT_MODE, BIT4|BIT3),
#endif

    _MEMMAP_nonPM_,

    // Clear all pad in
    _RVM1(0x1EA1, 0, BIT7),
    _END_OF_TBL2_,

    //external flash #1
    _MEMMAP_PM_,
    _RVM1(0x0E6A, 0x00, BIT0), //reg_spi_gpio set to 0 (swtch to spi_do, spi_di, spi_ck)
    _RVM1(0x0E6A, 0x00, BIT1), //reg_spicsz0_gpio set to 0 (enable spi_cz0)
    _RVM1(0x08F4, 0x00, 0xFF), //reg_chip_select set to 0 (select external flash #1)

    _MEMMAP_nonPM_,

#if (PIN_FLASH_WP0)
  #if ( (MS_BOARD_TYPE_SEL == BD_MST222B_D01A_S) \
     || (MS_BOARD_TYPE_SEL == BD_MST222A_D01A_S) \
     || (MS_BOARD_TYPE_SEL == BD_MST084B_10AHB_15043) \
      )//PAD_PWM0
    _RVM1(0x1e40,    0,  BIT4), //reg[101e40]#4 = 0b
    _RVM1(0x1e85, BIT4,  BIT4), //output high
    _RVM1(0x1e87,    0,  BIT4), //oen: output(0)
  #elif ((MS_BOARD_TYPE_SEL == BD_MST222F_D01A_S) \
    || (MS_BOARD_TYPE_SEL == BD_SOLUM_XL1U))//PAD_GPIO12
    _RVM1(0x1ef7, 0, BIT5|BIT4 ), //reg[101ef7]#5 ~#4 = 00b
    _RVM1(0x1e1c, 0, BIT2|BIT1|BIT0 ), //reg[101e1c]#2 ~#0 = 000b
    _RVM1(0x1e7b, BIT4, BIT4), //output high
    _RVM1(0x1e7f, 0, BIT4), //oen: output(0)
  #else
    #error "Please select correct boards"
  #endif
#endif
    _END_OF_TBL2_,

};

void MDrv_Pad_Init( void )
{
    U8 code *pPadConfig;
    U8  u8Length;
    U16 u16Index; // register index
    U32 u32RegAddr;

    u32MapBase = _MapBase_nonPM_MAYA;

    pPadConfig = (U8 *)padInitTbl + 10;

    while (1)
    {
        u16Index = (pPadConfig[0] << 8) + (pPadConfig[1]);

        if (u16Index == 0xFFFF) // check end of table
            break;

        pPadConfig += 2;

        if(u16Index == 0x0000) // switch base address
        {
            u32RegAddr = (U32)(((U32)pPadConfig[0])<<16);
            u32MapBase = (U32)_MapBase_PM_MAYA + (u32RegAddr<<1);
            pPadConfig += 2;
            continue;
        }

        u8Length = HIGHBYTE(u16Index) >> 6;
        u16Index &= 0x3FFF;

        while (1)
        {
            //printf("%x = %bx & %bx\n", u16Index, pPadConfig[0], pPadConfig[1]);
            MDrv_PAD_WriteByteMask((U32)u16Index, pPadConfig[0], pPadConfig[1]);
            pPadConfig += 2;

            if (u8Length == 0)
                break;

            u8Length--;
            u16Index++;
        }
    }

}

//-----------------------------------------------------------------
// Sanity check for PAD_TOP setting (TODO)
//-----------------------------------------------------------------
#if 0
#if (UART0_SRC_SEL == UART1_SRC_SEL) || \
    (UART0_SRC_SEL == UART2_SRC_SEL) || \
    (UART1_SRC_SEL == UART2_SRC_SEL)
    #error "UART src selection conflict"
#endif
#endif


