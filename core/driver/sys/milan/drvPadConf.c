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

#define _MILAN_ENBEDED_FLASH_      0       // 1: with enbeded flash, 0: without enbeded flash

#if defined (MIPS_CHAKRA) || defined(MSOS_TYPE_LINUX)
    #define _MapBase_PM             RIU_MAP
    #define _MapBase_nonPM          (RIU_MAP + 0x200000)
    #define _MapBase_nonPM_11    (RIU_MAP + 0x220000)
    #define _MapBase_nonPM_12   (RIU_MAP + 0x240000)
#else
    #define _MapBase_PM             0xA0000000
    #define _MapBase_nonPM        0xA0200000
    #define _MapBase_nonPM_11   0xA0220000
    #define _MapBase_nonPM_12   0xA0240000
#endif

#define _MEMMAP_nonPM_          _RVM1(0x0000, 0x10, 0xFF)
#define _MEMMAP_PM_             _RVM1(0x0000, 0x00, 0xFF)
#define _MEMMAP_nonPM_11_   _RVM1(0x0000, 0x11, 0xFF)
#define _MEMMAP_nonPM_12_   _RVM1(0x0000, 0x12, 0xFF)

static U32 u32MapBase;

void MDrv_PAD_WriteByte(U32 u32RegAddr, U8 u8Val)
{
    ((volatile U8*)(u32MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
}

U8 MDrv_PAD_ReadByte(U32 u32RegAddr)
{
    return ((volatile U8*)(u32MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

void MDrv_PAD_WriteByteMask(U32 u32Reg_addr, U8 u8Value, U8 u8Mask)
{
    U8 u8Val = MDrv_PAD_ReadByte(u32Reg_addr);

    u8Val = (u8Val & ~(u8Mask)) | (u8Value & u8Mask);
    MDrv_PAD_WriteByte(u32Reg_addr, u8Val);
}

//static code U8 padInitTbl[] =
volatile U8 code padInitTbl[] __attribute__((__section__ (".isp_info"))) =
{
    0x39, 0xB6, 0x5B, 0x53,     // magic code for ISP_Tool

    // programable device number
    // spi flash count
    1 + (PIN_SPI_CZ1 != 0) + (PIN_SPI_CZ2 != 0) + (PIN_SPI_CZ3 != 0),
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
    //reg_tconconfig[0] = 0b
    _RVM1(0x1ef2, 0, BIT0 ),   //reg[101ef2]#0 = 0b
    //reg_3d_lr_config[2:0] = 00b
    _RVM1(0x1e10, 0, BIT2|BIT1|BIT0 ),   //reg[101e10]#2~#0 = 000b
    //reg_8bit_ttl = 0b
    _RVM1(0x1ef8, 0, BIT1 ),   //reg[101ef8]#1 = 0b
    //reg_mhl_vbus_config[2:0] = 000b
    _RVM1(0x1ef9, 0, BIT7|BIT6|BIT5 ),   //reg[101ef9]#7~#5 = 0b
    //reg_i2soutconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT6|BIT5 ),   //reg[101eae]#6~#5 = 00b
    //reg_led_mode[3:2] = 00b
    _RVM1(0x1ee2, 0, BIT3|BIT2 ),   //reg[101ee2]#3~#2 = 00b
    //reg_ddcrmode[3:0] = 0000b
    _RVM1(0x1ef6, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef6]#3~#0 = 0000b
    //reg_agc_dbg[7] = 0b
    _RVM1(0x1e26, 0, BIT7 ),   //reg[101e26]#7 = 0b
    #endif

    #if(PAD_TCON1_IS_GPIO != GPIO_NONE)
    #define PAD_TCON1_OEN (PAD_TCON1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TCON1_OUT (PAD_TCON1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e5e, PAD_TCON1_OUT, BIT1),
    _RVM1(0x1e60, PAD_TCON1_OEN, BIT1),
    //reg_tconconfig[1] = 0b
    _RVM1(0x1ef2, 0, BIT1 ),   //reg[101ef2]#1 = 0b
    //reg_fastuartmode = 0b
    _RVM1(0x1e04, 0, BIT4 ),   //reg[101e04]#4 = 0b
    //reg_et_mode = 0b
    _RVM1(0x1e40, 0, BIT1|BIT0  ),   //reg[101e40]#1 ~ #0  = 00b
    #endif

    #if(PAD_TCON2_IS_GPIO != GPIO_NONE)
    #define PAD_TCON2_OEN (PAD_TCON2_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_TCON2_OUT (PAD_TCON2_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e5e, PAD_TCON2_OUT, BIT2),
    _RVM1(0x1e60, PAD_TCON2_OEN, BIT2),
    //reg_tconconfig[2]= 0b
    _RVM1(0x1ef2, 0, BIT2 ),   //reg[101ef2]#2 = 0b
    //reg_3d_lr_config[2:0] = 00b
    _RVM1(0x1e10, 0, BIT2|BIT1|BIT0 ),   //reg[101e10]#2~#0 = 000b
    //reg_8bit_ttl = 0b
    _RVM1(0x1ef8, 0, BIT1 ),   //reg[101ef8]#1 = 0b
    //reg_mhl_vbus_config[2:0] = 000b
    _RVM1(0x1ef9, 0, BIT7|BIT6|BIT5 ),   //reg[101ef9]#7~#5 = 0b
    //reg_i2soutconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT6|BIT5 ),   //reg[101eae]#6~#5 = 00b
    //reg_led_mode[3:2] = 00b
    _RVM1(0x1ee2, 0, BIT3|BIT2 ),   //reg[101ee2]#3~#2 = 00b
    //reg_ddcrmode[3:0] = 0000b
    _RVM1(0x1ef6, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef6]#3~#0 = 0000b
    //reg_agc_dbg[7] = 0b
    _RVM1(0x1e26, 0, BIT7 ),   //reg[101e26]#7 = 0b
    #endif

    #if(PAD_TCON3_IS_GPIO != GPIO_NONE)
    #define PAD_TCON3_OEN (PAD_TCON3_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_TCON3_OUT (PAD_TCON3_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e5e, PAD_TCON3_OUT, BIT3),
    _RVM1(0x1e60, PAD_TCON3_OEN, BIT3),
    //reg_tconconfig[3]= 0b
    _RVM1(0x1ef2, 0, BIT3 ),   //reg[101ef2]#3 = 0b
    //reg_3d_lr_config[2:0] = 00b
    _RVM1(0x1e10, 0, BIT2|BIT1|BIT0 ),   //reg[101e10]#2~#0 = 000b
    //reg_8bit_ttl = 0b
    _RVM1(0x1ef8, 0, BIT1 ),   //reg[101ef8]#1 = 0b
    //reg_mhl_vbus_config[2:0] = 000b
    _RVM1(0x1ef9, 0, BIT7|BIT6|BIT5 ),   //reg[101ef9]#7~#5 = 0b
    //reg_i2soutconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT6|BIT5 ),   //reg[101eae]#6~#5 = 00b
    //reg_led_mode[3:2] = 00b
    _RVM1(0x1ee2, 0, BIT3|BIT2 ),   //reg[101ee2]#3~#2 = 00b
    //reg_ddcrmode[3:0] = 0000b
    _RVM1(0x1ef6, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef6]#3~#0 = 0000b
    //reg_agc_dbg[7] = 0b
    _RVM1(0x1e26, 0, BIT7 ),   //reg[101e26]#7 = 0b
    #endif

    #if(PAD_TCON4_IS_GPIO != GPIO_NONE)
    #define PAD_TCON4_OEN (PAD_TCON4_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_TCON4_OUT (PAD_TCON4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e5e, PAD_TCON4_OUT, BIT4),
    _RVM1(0x1e60, PAD_TCON4_OEN, BIT4),
    //reg_tconconfig[4]=0b
    _RVM1(0x1ef2, 0, BIT4 ),   //reg[101ef2]#4 =0b
    //reg_et_mode = 0b
    _RVM1(0x1e40, 0, BIT1|BIT0  ),   //reg[101e40]#1 ~ #0  = 00b
    //reg_ddcrmode[3:0] = 0000b
    _RVM1(0x1ef6, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef6]#3 ~#0 = 0000b
    #endif

    #if(PAD_TCON5_IS_GPIO != GPIO_NONE)
    #define PAD_TCON5_OEN (PAD_TCON5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TCON5_OUT (PAD_TCON5_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e5e, PAD_TCON5_OUT, BIT5),
    _RVM1(0x1e60, PAD_TCON5_OEN, BIT5),
    //reg_tconconfig[5]= 0b
    _RVM1(0x1ef2, 0, BIT5 ),   //reg[101ef2]#5 = 0b
    //reg_i2soutconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT6|BIT5 ),   //reg[101eae]#6 ~#5 = 00b
    //reg_et_mode = 0b
    _RVM1(0x1e40, 0, BIT1|BIT0  ),   //reg[101e40]#1 ~ #0  = 00b
    //reg_ddcrmode[3:0] = 0000b
    _RVM1(0x1ef6, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef6]#3 ~#0 = 0000b
    #endif

    #if(PAD_TCON6_IS_GPIO != GPIO_NONE)
    #define PAD_TCON6_OEN (PAD_TCON6_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_TCON6_OUT (PAD_TCON6_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e5e, PAD_TCON6_OUT, BIT6),
    _RVM1(0x1e60, PAD_TCON6_OEN, BIT6),
    //reg_tconconfig[6]= 0b
    _RVM1(0x1ef2, 0, BIT6 ),   //reg[101ef2]#6 = 0b
    //reg_et_mode = 0b
    _RVM1(0x1e40, 0, BIT1|BIT0  ),   //reg[101e40]#1 ~ #0  = 00b
    #endif

    #if(PAD_TCON7_IS_GPIO != GPIO_NONE)
    #define PAD_TCON7_OEN (PAD_TCON7_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_TCON7_OUT (PAD_TCON7_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e5e, PAD_TCON7_OUT, BIT7),
    _RVM1(0x1e60, PAD_TCON7_OEN, BIT7),
    //reg_tconconfig[7]= 0b
    _RVM1(0x1ef2, 0, BIT7 ),   //reg[101ef2]#7 = 0b
    //reg_et_mode = 0b
    _RVM1(0x1e40, 0, BIT1|BIT0  ),   //reg[101e40]#1 ~ #0  = 00b
    #endif

#if !defined(BENCH_CODE_USAGE)
    #if (PAD_PWM1_IS_GPIO != GPIO_NONE)
    #define PAD_PWM1_OEN (PAD_PWM1_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_PWM1_OUT (PAD_PWM1_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e85, PAD_PWM1_OUT, BIT5),
    _RVM1(0x1e87, PAD_PWM1_OEN, BIT5),
    //reg_PWM1Config = 0b
    _RVM1(0x1e40, 0, BIT5 ),   //reg[101e40]#5 = 0b
    #endif
#endif

    #if(PAD_PWM0_IS_GPIO != GPIO_NONE)
    #define PAD_PWM0_OEN (PAD_PWM0_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_PWM0_OUT (PAD_PWM0_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e85, PAD_PWM0_OUT, BIT4),
    _RVM1(0x1e87, PAD_PWM0_OEN, BIT4),
    //reg_PWM0Config = 0b
    _RVM1(0x1e40, 0, BIT4 ),   //reg[101e40]#4 = 0b
    #endif

    #if(PAD_LVSYNC_IS_GPIO != GPIO_NONE)
    #define PAD_LVSYNC_OEN (PAD_LVSYNC_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_LVSYNC_OUT (PAD_LVSYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x329e, PAD_LVSYNC_OUT, BIT0),
    _RVM1(0x32a2, PAD_LVSYNC_OEN, BIT0),
    //reg_output_conf[1:0]
    _RVM1(0x32da, 0, BIT1|BIT0 ),   //reg[1032da]#1~#0 = 00b
    //reg_ext_data_en[0]
    _RVM1(0x328c, BIT0 , BIT0 ),   //reg[10328c]#0 = 1b
    //reg_gpo_sel[0]
    _RVM1(0x329a, BIT0 , BIT0 ),   //reg[10329a]#0 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_LHSYNC_IS_GPIO != GPIO_NONE)
    #define PAD_LHSYNC_OEN (PAD_LHSYNC_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_LHSYNC_OUT (PAD_LHSYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x329e, PAD_LHSYNC_OUT, BIT1),
    _RVM1(0x32a2, PAD_LHSYNC_OEN, BIT1),
    //reg_output_conf[1:0]
    _RVM1(0x32da, 0, BIT1|BIT0 ),   //reg[1032da]#1~#0 = 00b
    //reg_ext_data_en[1]
    _RVM1(0x328c, BIT1 , BIT1 ),   //reg[10328c]#1 = 1b
    //reg_gpo_sel[1]
    _RVM1(0x329a, BIT1 , BIT1 ),   //reg[10329a]#1 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_LDE_IS_GPIO != GPIO_NONE)
    #define PAD_LDE_OEN (PAD_LDE_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_LDE_OUT (PAD_LDE_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x329e, PAD_LDE_OUT, BIT2),
    _RVM1(0x32a2, PAD_LDE_OEN, BIT2),
    //reg_output_conf[3:2]
    _RVM1(0x32da, 0, BIT3|BIT2 ),   //reg[1032da]#3~#2 = 00b
    //reg_ext_data_en[2]
    _RVM1(0x328c, BIT2 , BIT2 ),   //reg[10328c]#2 = 1b
    //reg_gpo_sel[2]
    _RVM1(0x329a, BIT2 , BIT2 ),   //reg[10329a]#2 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_LCK_IS_GPIO != GPIO_NONE)
    #define PAD_LCK_OEN (PAD_LCK_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_LCK_OUT (PAD_LCK_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x329e, PAD_LCK_OUT, BIT3),
    _RVM1(0x32a2, PAD_LCK_OEN, BIT3),
    //reg_output_conf[3:2]
    _RVM1(0x32da, 0, BIT3|BIT2 ),   //reg[1032da]#3~#2 = 00b
    //reg_ext_data_en[3]
    _RVM1(0x328c, BIT3 , BIT3 ),   //reg[10328c]#3 = 1b
    //reg_gpo_sel[3]
    _RVM1(0x329a, BIT3 , BIT3 ),   //reg[10329a]#3 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_R_ODD7_IS_GPIO != GPIO_NONE)
    #define PAD_R_ODD7_OEN (PAD_R_ODD7_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_R_ODD7_OUT (PAD_R_ODD7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x329e, PAD_R_ODD7_OUT, BIT4),
    _RVM1(0x32a2, PAD_R_ODD7_OEN, BIT4),
    //reg_output_conf[5:4]
    _RVM1(0x32da, 0, BIT5|BIT4 ),   //reg[1032da]#5~#4 = 00b
    //reg_ext_data_en[4]
    _RVM1(0x328c, BIT4 , BIT4 ),   //reg[10328c]#4 = 1b
    //reg_gpo_sel[12]
    _RVM1(0x329a, BIT4 , BIT4 ),   //reg[10329a]#4 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_R_ODD6_IS_GPIO != GPIO_NONE)
    #define PAD_R_ODD6_OEN (PAD_R_ODD6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_R_ODD6_OUT (PAD_R_ODD6_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x329e, PAD_R_ODD6_OUT, BIT5),
    _RVM1(0x32a2, PAD_R_ODD6_OEN, BIT5),
    //reg_output_conf[5:4]
    _RVM1(0x32da, 0, BIT5|BIT4 ),   //reg[1032da]#5~#4 = 00b
    //reg_ext_data_en[5]
    _RVM1(0x328c, BIT5 , BIT5 ),   //reg[10328c]#5 = 1b
    //reg_gpo_sel[5]
    _RVM1(0x329a, BIT5 , BIT5 ),   //reg[10329a]#5 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_R_ODD5_IS_GPIO != GPIO_NONE)
    #define PAD_R_ODD5_OEN (PAD_R_ODD5_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_R_ODD5_OUT (PAD_R_ODD5_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x329e, PAD_R_ODD5_OUT, BIT6),
    _RVM1(0x32a2, PAD_R_ODD5_OEN, BIT6),
    //reg_output_conf[7:6]
    _RVM1(0x32da, 0, BIT7|BIT6 ),   //reg[1032da]#7~#6 = 00b
    //reg_ext_data_en[6]
    _RVM1(0x328c, BIT6 , BIT6 ),   //reg[10328c]#6 = 1b
    //reg_gpo_sel[6]
    _RVM1(0x329a, BIT6 , BIT6 ),   //reg[10329a]#6 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_R_ODD4_IS_GPIO != GPIO_NONE)
    #define PAD_R_ODD4_OEN (PAD_R_ODD4_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_R_ODD4_OUT (PAD_R_ODD4_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x329e, PAD_R_ODD4_OUT, BIT7),
    _RVM1(0x32a2, PAD_R_ODD4_OEN, BIT7),
    //reg_output_conf[7:6]
    _RVM1(0x32da, 0, BIT7|BIT6 ),   //reg[1032da]#7~#6 = 00b
    //reg_ext_data_en[7]
    _RVM1(0x328c, BIT7 , BIT7 ),   //reg[10328c]#7 = 1b
    //reg_gpo_sel[7]
    _RVM1(0x329a, BIT7 , BIT7 ),   //reg[10329a]#7 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_R_ODD3_IS_GPIO != GPIO_NONE)
    #define PAD_R_ODD3_OEN (PAD_R_ODD3_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_R_ODD3_OUT (PAD_R_ODD3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x329f, PAD_R_ODD3_OUT, BIT0),
    _RVM1(0x32a3, PAD_R_ODD3_OEN, BIT0),
    //reg_output_conf[9:8]
    _RVM1(0x32db, 0, BIT1|BIT0 ),   //reg[1032db]#1~#0 = 00b
    //reg_ext_data_en[8]
    _RVM1(0x328d, BIT0 , BIT0 ),   //reg[10328d]#0 = 1b
    //reg_gpo_sel[8]
    _RVM1(0x329b, BIT0 , BIT0 ),   //reg[10329b]#0 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_R_ODD2_IS_GPIO != GPIO_NONE)
    #define PAD_R_ODD2_OEN (PAD_R_ODD2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_R_ODD2_OUT (PAD_R_ODD2_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x329f, PAD_R_ODD2_OUT, BIT1),
    _RVM1(0x32a3, PAD_R_ODD2_OEN, BIT1),
    //reg_output_conf[9:8]
    _RVM1(0x32db, 0, BIT1|BIT0 ),   //reg[1032db]#1~#0 = 00b
    //reg_ext_data_en[9]
    _RVM1(0x328d, BIT1 , BIT1 ),   //reg[10328d]#1 = 1b
    //reg_gpo_sel[9]
    _RVM1(0x329b, BIT1 , BIT1 ),   //reg[10329b]#1 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_R_ODD1_IS_GPIO != GPIO_NONE)
    #define PAD_R_ODD1_OEN (PAD_R_ODD1_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_R_ODD1_OUT (PAD_R_ODD1_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x329f, PAD_R_ODD1_OUT, BIT2),
    _RVM1(0x32a3, PAD_R_ODD1_OEN, BIT2),
    //reg_output_conf[11:10]
    _RVM1(0x32db, 0, BIT3|BIT2 ),   //reg[1032db]#3~#2 = 00b
    //reg_ext_data_en[10]
    _RVM1(0x328d, BIT2 , BIT2 ),   //reg[10328d]#2 = 1b
    //reg_gpo_sel[10]
    _RVM1(0x329b, BIT2 , BIT2 ),   //reg[10329b]#2 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_R_ODD0_IS_GPIO != GPIO_NONE)
    #define PAD_R_ODD0_OEN (PAD_R_ODD0_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_R_ODD0_OUT (PAD_R_ODD0_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x329f, PAD_R_ODD0_OUT, BIT3),
    _RVM1(0x32a3, PAD_R_ODD0_OEN, BIT3),
    //reg_output_conf[11:10]
    _RVM1(0x32db, 0, BIT3|BIT2 ),   //reg[1032db]#3~#2 = 00b
    //reg_ext_data_en[11]
    _RVM1(0x328d, BIT3 , BIT3 ),   //reg[10328d]#3 = 1b
    //reg_gpo_sel[11]
    _RVM1(0x329b, BIT3 , BIT3 ),   //reg[10329b]#3 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_G_ODD7_IS_GPIO != GPIO_NONE)
    #define PAD_G_ODD7_OEN (PAD_G_ODD7_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_G_ODD7_OUT (PAD_G_ODD7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x329f, PAD_G_ODD7_OUT, BIT4),
    _RVM1(0x32a3, PAD_G_ODD7_OEN, BIT4),
    //reg_output_conf[13:12]
    _RVM1(0x32db, 0, BIT5|BIT4 ),   //reg[1032db]#5~#4 = 00b
    //reg_ext_data_en[12]
    _RVM1(0x328d, BIT4 , BIT4 ),   //reg[10328d]#4 = 1b
    //reg_gpo_sel[12]
    _RVM1(0x329b, BIT4 , BIT4 ),   //reg[10329b]#4 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_G_ODD6_IS_GPIO != GPIO_NONE)
    #define PAD_G_ODD6_OEN (PAD_G_ODD6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_G_ODD6_OUT (PAD_G_ODD6_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x329f, PAD_G_ODD6_OUT, BIT5),
    _RVM1(0x32a3, PAD_G_ODD6_OEN, BIT5),
    //reg_output_conf[13:12]
    _RVM1(0x32db, 0, BIT5|BIT4 ),   //reg[1032db]#5~#4 = 00b
    //reg_ext_data_en[13]
    _RVM1(0x328d, BIT5 , BIT5 ),   //reg[10328d]#5 = 1b
    //reg_gpo_sel[13]
    _RVM1(0x329b, BIT5 , BIT5 ),   //reg[10329b]#5 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_G_ODD5_IS_GPIO != GPIO_NONE)
    #define PAD_G_ODD5_OEN (PAD_G_ODD5_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_G_ODD5_OUT (PAD_G_ODD5_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x329f, PAD_G_ODD5_OUT, BIT6),
    _RVM1(0x32a3, PAD_G_ODD5_OEN, BIT6),
    //reg_output_conf[15:14]
    _RVM1(0x32db, 0, BIT7|BIT6 ),   //reg[1032db]#7~#6 = 00b
    //reg_ext_data_en[14]
    _RVM1(0x328d, BIT6 , BIT6 ),   //reg[10328d]#6 = 1b
    //reg_gpo_sel[14]
    _RVM1(0x329b, BIT6 , BIT6 ),   //reg[10329b]#6 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_G_ODD4_IS_GPIO != GPIO_NONE)
    #define PAD_G_ODD4_OEN (PAD_G_ODD4_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_G_ODD4_OUT (PAD_G_ODD4_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x329f, PAD_G_ODD4_OUT, BIT7),
    _RVM1(0x32a3, PAD_G_ODD4_OEN, BIT7),
    //reg_output_conf[15:14]
    _RVM1(0x32db, 0, BIT7|BIT6 ),   //reg[1032db]#7~#6 = 00b
    //reg_ext_data_en[15]
    _RVM1(0x328d, BIT7 , BIT7 ),   //reg[10328d]#7 = 1b
    //reg_gpo_sel[15]
    _RVM1(0x329b, BIT7 , BIT7 ),   //reg[10329b]#7 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_G_ODD3_IS_GPIO != GPIO_NONE)
    #define PAD_G_ODD3_OEN (PAD_G_ODD3_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_G_ODD3_OUT (PAD_G_ODD3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x32a0, PAD_G_ODD3_OUT, BIT0),
    _RVM1(0x32a4, PAD_G_ODD3_OEN, BIT0),
    //reg_output_conf[17:16]
    _RVM1(0x32dc, 0, BIT1|BIT0 ),   //reg[1032dc]#1~#0 = 00b
    //reg_ext_data_en[16]
    _RVM1(0x328e, BIT0 , BIT0 ),   //reg[10328e]#0 = 1b
    //reg_gpo_sel[16]
    _RVM1(0x329c, BIT0 , BIT0 ),   //reg[10329c]#0 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_G_ODD2_IS_GPIO != GPIO_NONE)
    #define PAD_G_ODD2_OEN (PAD_G_ODD2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_G_ODD2_OUT (PAD_G_ODD2_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x32a0, PAD_G_ODD2_OUT, BIT1),
    _RVM1(0x32a4, PAD_G_ODD2_OEN, BIT1),
    //reg_output_conf[17:16]
    _RVM1(0x32dc, 0, BIT1|BIT0 ),   //reg[1032dc]#1~#0 = 00b
    //reg_ext_data_en[17]
    _RVM1(0x328e, BIT1 , BIT1 ),   //reg[10328e]#1 = 1b
    //reg_gpo_sel[17]
    _RVM1(0x329c, BIT1 , BIT1 ),   //reg[10329c]#1 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_G_ODD1_IS_GPIO != GPIO_NONE)
    #define PAD_G_ODD1_OEN (PAD_G_ODD1_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_G_ODD1_OUT (PAD_G_ODD1_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x32a0, PAD_G_ODD1_OUT, BIT2),
    _RVM1(0x32a4, PAD_G_ODD1_OEN, BIT2),
    //reg_output_conf[19:18]
    _RVM1(0x32dc, 0, BIT3|BIT2 ),   //reg[1032dc]#3~#2 = 00b
    //reg_ext_data_en[18]
    _RVM1(0x328e, BIT2 , BIT2 ),   //reg[10328e]#2 = 1b
    //reg_gpo_sel[18]
    _RVM1(0x329c, BIT2 , BIT2 ),   //reg[10329c]#2 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_G_ODD0_IS_GPIO != GPIO_NONE)
    #define PAD_G_ODD0_OEN (PAD_G_ODD0_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_G_ODD0_OUT (PAD_G_ODD0_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x32a0, PAD_G_ODD0_OUT, BIT3),
    _RVM1(0x32a4, PAD_G_ODD0_OEN, BIT3),
    //reg_output_conf[19:18]
    _RVM1(0x32dc, 0, BIT3|BIT2 ),   //reg[1032dc]#3~#2 = 00b
    //reg_ext_data_en[19]
    _RVM1(0x328e, BIT3 , BIT3 ),   //reg[10328e]#3 = 1b
    //reg_gpo_sel[19]
    _RVM1(0x329c, BIT3 , BIT3 ),   //reg[10329c]#3 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_B_ODD7_IS_GPIO != GPIO_NONE)
    #define PAD_B_ODD7_OEN (PAD_B_ODD7_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_B_ODD7_OUT (PAD_B_ODD7_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x32a0, PAD_B_ODD7_OUT, BIT4),
    _RVM1(0x32a4, PAD_B_ODD7_OEN, BIT4),
    //reg_output_conf[21:20]
    _RVM1(0x32dc, 0, BIT5|BIT4 ),   //reg[1032dc]#5~#4 = 00b
    //reg_ext_data_en[20]
    _RVM1(0x328e, BIT4 , BIT4 ),   //reg[10328e]#4 = 1b
    //reg_gpo_sel[20]
    _RVM1(0x329c, BIT4 , BIT4 ),   //reg[10329c]#4 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_B_ODD6_IS_GPIO != GPIO_NONE)
    #define PAD_B_ODD6_OEN (PAD_B_ODD6_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_B_ODD6_OUT (PAD_B_ODD6_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x32a0, PAD_B_ODD6_OUT, BIT5),
    _RVM1(0x32a4, PAD_B_ODD6_OEN, BIT5),
    //reg_output_conf[21:20]
    _RVM1(0x32dc, 0, BIT5|BIT4 ),   //reg[1032dc]#5~#4 = 00b
    //reg_ext_data_en[21]
    _RVM1(0x328e, BIT5 , BIT5 ),   //reg[10328e]#5 = 1b
    //reg_gpo_sel[21]
    _RVM1(0x329c, BIT5 , BIT5 ),   //reg[10329c]#5 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_B_ODD5_IS_GPIO != GPIO_NONE)
    #define PAD_B_ODD5_OEN (PAD_B_ODD5_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_B_ODD5_OUT (PAD_B_ODD5_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x32a0, PAD_B_ODD5_OUT, BIT6),
    _RVM1(0x32a4, PAD_B_ODD5_OEN, BIT6),
    //reg_output_conf[23:22]
    _RVM1(0x32dc, 0, BIT7|BIT6 ),   //reg[1032dc]#7~#6 = 00b
    //reg_ext_data_en[22]
    _RVM1(0x328e, BIT6 , BIT6 ),   //reg[10328e]#6 = 1b
    //reg_gpo_sel[22]
    _RVM1(0x329c, BIT6 , BIT6 ),   //reg[10329c]#6 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_B_ODD4_IS_GPIO != GPIO_NONE)
    #define PAD_B_ODD4_OEN (PAD_B_ODD4_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_B_ODD4_OUT (PAD_B_ODD4_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x32a0, PAD_B_ODD4_OUT, BIT7),
    _RVM1(0x32a4, PAD_B_ODD4_OEN, BIT7),
    //reg_output_conf[23:22]
    _RVM1(0x32dc, 0, BIT7|BIT6 ),   //reg[1032dc]#7~#6 = 00b
    //reg_ext_data_en[23]
    _RVM1(0x328e, BIT7 , BIT7 ),   //reg[10328e]#7 = 1b
    //reg_gpo_sel[23]
    _RVM1(0x329c, BIT7 , BIT7 ),   //reg[10329c]#7 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_B_ODD3_IS_GPIO != GPIO_NONE)
    #define PAD_B_ODD3_OEN (PAD_B_ODD3_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_B_ODD3_OUT (PAD_B_ODD3_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x32a1, PAD_B_ODD3_OUT, BIT0),
    _RVM1(0x32a5, PAD_B_ODD3_OEN, BIT0),
    //reg_output_conf[25:24]
    _RVM1(0x32dd, 0, BIT1|BIT0 ),   //reg[1032dd]#1~#0 = 00b
    //reg_ext_data_en[24]
    _RVM1(0x328f, BIT0 , BIT0 ),   //reg[10328f]#0 = 1b
    //reg_gpo_sel[24]
    _RVM1(0x329d, BIT0 , BIT0 ),   //reg[10329d]#0 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_B_ODD2_IS_GPIO != GPIO_NONE)
    #define PAD_B_ODD2_OEN (PAD_B_ODD2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_B_ODD2_OUT (PAD_B_ODD2_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x32a1, PAD_B_ODD2_OUT, BIT1),
    _RVM1(0x32a5, PAD_B_ODD2_OEN, BIT1),
    //reg_output_conf[25:24]
    _RVM1(0x32dd, 0, BIT1|BIT0 ),   //reg[1032dd]#1~#0 = 00b
    //reg_ext_data_en[25]
    _RVM1(0x328f, BIT1 , BIT1 ),   //reg[10328f]#1 = 1b
    //reg_gpo_sel[25]
    _RVM1(0x329d, BIT1 , BIT1 ),   //reg[10329d]#1 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_B_ODD1_IS_GPIO != GPIO_NONE)
    #define PAD_B_ODD1_OEN (PAD_B_ODD1_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_B_ODD1_OUT (PAD_B_ODD1_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x32a1, PAD_B_ODD1_OUT, BIT2),
    _RVM1(0x32a5, PAD_B_ODD1_OEN, BIT2),
    //reg_output_conf[27:26]
    _RVM1(0x32dd, 0, BIT3|BIT2 ),   //reg[1032dd]#3~#2 = 00b
    //reg_ext_data_en[26]
    _RVM1(0x328f, BIT2 , BIT2 ),   //reg[10328f]#2 = 1b
    //reg_gpo_sel[26]
    _RVM1(0x329d, BIT2 , BIT2 ),   //reg[10329d]#2 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
    #endif

    #if(PAD_B_ODD0_IS_GPIO != GPIO_NONE)
    #define PAD_B_ODD0_OEN (PAD_B_ODD0_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_B_ODD0_OUT (PAD_B_ODD0_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x32a1, PAD_B_ODD0_OUT, BIT3),
    _RVM1(0x32a5, PAD_B_ODD0_OEN, BIT3),
    //reg_output_conf[27:26]
    _RVM1(0x32dd, 0, BIT3|BIT2 ),   //reg[1032dd]#3~#2 = 00b
    //reg_ext_data_en[27]
    _RVM1(0x328f, BIT3 , BIT3 ),   //reg[10328f]#3 = 1b
    //reg_gpo_sel[27]
    _RVM1(0x329d, BIT3 , BIT3 ),   //reg[10329d]#3 = 1b
    _RVM1(0x1e02, BIT0 , BIT0 ),   //reg[101e02]#0 = 1b
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
    //reg_et_mode = 0b
    _RVM1(0x1e40, 0, BIT1|BIT0  ),   //reg[101e40]#1 ~ #0  = 00b
    //reg_ddcrmode[3:0] = 0000b
    _RVM1(0x1ef6, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef6]#3 ~#0 = 0000b
    #endif

    #if(PAD_GPIO1_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO1_OEN (PAD_GPIO1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO1_OUT (PAD_GPIO1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e7a, PAD_GPIO1_OUT, BIT1),
    _RVM1(0x1e7e, PAD_GPIO1_OEN, BIT1),
    //reg_6bit_ttl = 0b
    _RVM1(0x1ef8, 0, BIT0 ),   //reg[101ef8]#0 = 0b
    //reg_et_mode = 0b
    _RVM1(0x1e40, 0, BIT1|BIT0  ),   //reg[101e40]#1 ~ #0  = 00b
    //reg_led_mode[3:2] = 00b
    _RVM1(0x1ee2, 0, BIT3|BIT2 ),   //reg[101ee2]#3~#2 = 00b
    //reg_usbdrvvbusconfig_eq_1 = 0b
    _RVM1(0x1e04, 0, BIT1|BIT0 ),   //reg[101e04]#1~#0 = 00b
    //reg_ddcrmode[3:0] = 0000b
    _RVM1(0x1ef6, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef6]#3 ~#0 = 0000b
    #endif

    #if(PAD_GPIO2_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO2_OEN (PAD_GPIO2_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_GPIO2_OUT (PAD_GPIO2_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e7a, PAD_GPIO2_OUT, BIT2),
    _RVM1(0x1e7e, PAD_GPIO2_OEN, BIT2),
    //reg_thirduartmode = 00b
    _RVM1(0x1e05, 0, BIT3|BIT2 ),   //reg[101e05]#3~#2 = 00b
    //reg_ddcrmode[3:0] = 0000b
    _RVM1(0x1ef6, 0, BIT3|BIT2|BIT1|BIT0 ),   //reg[101ef6]#3 ~#0 = 0000b
    //reg_et_mode = 0b
    _RVM1(0x1e40, 0, BIT1|BIT0  ),   //reg[101e40]#1 ~ #0  = 00b
    #endif

    #if(PAD_GPIO3_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO3_OEN (PAD_GPIO3_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_GPIO3_OUT (PAD_GPIO3_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e7a, PAD_GPIO3_OUT, BIT3),
    _RVM1(0x1e7e, PAD_GPIO3_OEN, BIT3),
    #endif

    #if(PAD_GPIO4_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO4_OEN (PAD_GPIO4_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_GPIO4_OUT (PAD_GPIO4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e7a, PAD_GPIO4_OUT, BIT4),
    _RVM1(0x1e7e, PAD_GPIO4_OEN, BIT4),
    //reg_thirduartmode = 00b
    _RVM1(0x1e05, 0, BIT3|BIT2 ),   //reg[101e05]#3~#2 = 00b
    //reg_diseqc_out_config[1:0] = 00b
    _RVM1(0x1e07, 0, BIT4|BIT3 ),   //reg[101e07]#4 ~#3 = 00b
    #endif

    #if(PAD_GPIO5_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO5_OEN (PAD_GPIO5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO5_OUT (PAD_GPIO5_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e7a, PAD_GPIO5_OUT, BIT5),
    _RVM1(0x1e7e, PAD_GPIO5_OEN, BIT5),
    //reg_diseqc_out_config[1:0] = 00b
    _RVM1(0x1e07, 0, BIT4|BIT3 ),   //reg[101e07]#4 ~#3 = 00b
    //reg_i2soutconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT6|BIT5 ),   //reg[101eae]#6 ~#5 = 00b
    //reg_diseqc_out_config[1:0] = 00b
    _RVM1(0x1e07, 0, BIT4|BIT3 ),   //reg[101e07]#4 ~#3 = 00b
    #endif

    #if(PAD_GPIO6_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO6_OEN (PAD_GPIO6_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_GPIO6_OUT (PAD_GPIO6_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e7a, PAD_GPIO6_OUT, BIT6),
    _RVM1(0x1e7e, PAD_GPIO6_OEN, BIT6),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_PWM5Config = 00b
    _RVM1(0x1e41, 0, BIT3|BIT2 ),   //reg[101e41]#3~#2 = 0b
    //reg_pcmctrlconfig[5] = 0b
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 0b
    #endif

    #if(PAD_GPIO7_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO7_OEN (PAD_GPIO7_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_GPIO7_OUT (PAD_GPIO7_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e7a, PAD_GPIO7_OUT, BIT7),
    _RVM1(0x1e7e, PAD_GPIO7_OEN, BIT7),
    //reg_PWM4Config = 00b
    _RVM1(0x1e41, 0, BIT1|BIT0 ),   //reg[101e41]#1~#0 = 00b
    #endif

    #if(PAD_GPIO8_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO8_OEN (PAD_GPIO8_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO8_OUT (PAD_GPIO8_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e7b, PAD_GPIO8_OUT, BIT0),
    _RVM1(0x1e7f, PAD_GPIO8_OEN, BIT0),
    #endif

    #if(PAD_GPIO9_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO9_OEN (PAD_GPIO9_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO9_OUT (PAD_GPIO9_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e7b, PAD_GPIO9_OUT, BIT1),
    _RVM1(0x1e7f, PAD_GPIO9_OEN, BIT1),
    #endif

    #if(PAD_GPIO10_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO10_OEN (PAD_GPIO10_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_GPIO10_OUT (PAD_GPIO10_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e7b, PAD_GPIO10_OUT, BIT2),
    _RVM1(0x1e7f, PAD_GPIO10_OEN, BIT2),
    //reg_i2soutconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT6|BIT5 ),   //reg[101eae]#6 ~#5 = 00b
    //reg_led_mode[3:2] = 00b
    _RVM1(0x1ee2, 0, BIT3|BIT2 ),   //reg[101ee2]#3~#2 = 00b
    #endif

    #if(PAD_GPIO11_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO11_OEN (PAD_GPIO11_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_GPIO11_OUT (PAD_GPIO11_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e7b, PAD_GPIO11_OUT, BIT3),
    _RVM1(0x1e7f, PAD_GPIO11_OEN, BIT3),
    //reg_i2soutconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT6|BIT5 ),   //reg[101eae]#6 ~#5 = 00b
    //reg_led_mode[3:2] = 00b
    _RVM1(0x1ee2, 0, BIT3|BIT2 ),   //reg[101ee2]#3~#2 = 00b
    #endif

    #if(PAD_GPIO12_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO12_OEN (PAD_GPIO12_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_GPIO12_OUT (PAD_GPIO12_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e7b, PAD_GPIO12_OUT, BIT4),
    _RVM1(0x1e7f, PAD_GPIO12_OEN, BIT4),
    //reg_i2soutconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT6|BIT5 ),   //reg[101eae]#6 ~#5 = 00b
    #endif

    #if(PAD_GPIO13_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO13_OEN (PAD_GPIO13_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO13_OUT (PAD_GPIO13_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e7b, PAD_GPIO13_OUT, BIT5),
    _RVM1(0x1e7f, PAD_GPIO13_OEN, BIT5),
    #endif

    #if(PAD_GPIO14_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO14_OEN (PAD_GPIO14_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_GPIO14_OUT (PAD_GPIO14_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e7b, PAD_GPIO14_OUT, BIT6),
    _RVM1(0x1e7f, PAD_GPIO14_OEN, BIT6),
    #endif

    #if(PAD_GPIO15_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO15_OEN (PAD_GPIO15_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_GPIO15_OUT (PAD_GPIO15_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e7b, PAD_GPIO15_OUT, BIT7),
    _RVM1(0x1e7f, PAD_GPIO15_OEN, BIT7),
    #endif

    #if(PAD_GPIO16_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO16_OEN (PAD_GPIO16_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO16_OUT (PAD_GPIO16_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e7c, PAD_GPIO16_OUT, BIT0),
    _RVM1(0x1e80, PAD_GPIO16_OEN, BIT0),
    #endif

    #if(PAD_GPIO17_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO17_OEN (PAD_GPIO17_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO17_OUT (PAD_GPIO17_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e7c, PAD_GPIO17_OUT, BIT1),
    _RVM1(0x1e80, PAD_GPIO17_OEN, BIT1),
    #endif

    #if(PAD_GPIO18_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO18_OEN (PAD_GPIO18_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_GPIO18_OUT (PAD_GPIO18_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e7c, PAD_GPIO18_OUT, BIT2),
    _RVM1(0x1e80, PAD_GPIO18_OEN, BIT2),
    #endif

    #if(PAD_GPIO19_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO19_OEN (PAD_GPIO19_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_GPIO19_OUT (PAD_GPIO19_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e7c, PAD_GPIO19_OUT, BIT3),
    _RVM1(0x1e80, PAD_GPIO19_OEN, BIT3),
    #endif

    #if(PAD_GPIO20_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO20_OEN (PAD_GPIO20_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_GPIO20_OUT (PAD_GPIO20_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e7c, PAD_GPIO20_OUT, BIT4),
    _RVM1(0x1e80, PAD_GPIO20_OEN, BIT4),
    #endif

    #if(PAD_GPIO21_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO21_OEN (PAD_GPIO21_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO21_OUT (PAD_GPIO21_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e7c, PAD_GPIO21_OUT, BIT5),
    _RVM1(0x1e80, PAD_GPIO21_OEN, BIT5),
    #endif

    #if(PAD_GPIO22_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO22_OEN (PAD_GPIO22_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_GPIO22_OUT (PAD_GPIO22_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e7c, PAD_GPIO22_OUT, BIT6),
    _RVM1(0x1e80, PAD_GPIO22_OEN, BIT6),
    #endif

    #if(PAD_GPIO23_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO23_OEN (PAD_GPIO23_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_GPIO23_OUT (PAD_GPIO23_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e7c, PAD_GPIO23_OUT, BIT7),
    _RVM1(0x1e80, PAD_GPIO23_OEN, BIT7),
    #endif

    #if(PAD_GPIO24_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO24_OEN (PAD_GPIO24_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO24_OUT (PAD_GPIO24_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e7d, PAD_GPIO24_OUT, BIT0),
    _RVM1(0x1e81, PAD_GPIO24_OEN, BIT0),
    //reg_mhl_vbus_config[2:0] = 000b
    _RVM1(0x1ef9, 0, BIT7|BIT6|BIT5 ),   //reg[101ef9]#7~#5 = 0b
    #endif

    #if(PAD_GPIO25_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO25_OEN (PAD_GPIO25_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO25_OUT (PAD_GPIO25_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e7d, PAD_GPIO25_OUT, BIT1),
    _RVM1(0x1e81, PAD_GPIO25_OEN, BIT1),
    #endif

    #if(PAD_GPIO26_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO26_OEN (PAD_GPIO26_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_GPIO26_OUT (PAD_GPIO26_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e7d, PAD_GPIO26_OUT, BIT2),
    _RVM1(0x1e81, PAD_GPIO26_OEN, BIT2),
    #endif

    #if(PAD_GPIO27_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO27_OEN (PAD_GPIO27_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_GPIO27_OUT (PAD_GPIO27_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e7d, PAD_GPIO27_OUT, BIT3),
    _RVM1(0x1e81, PAD_GPIO27_OEN, BIT3),
    #endif

    #if(PAD_GPIO28_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO28_OEN (PAD_GPIO28_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_GPIO28_OUT (PAD_GPIO28_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e7d, PAD_GPIO28_OUT, BIT4),
    _RVM1(0x1e81, PAD_GPIO28_OEN, BIT4),
    #endif

    #if(PAD_GPIO29_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO29_OEN (PAD_GPIO29_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO29_OUT (PAD_GPIO29_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e7d, PAD_GPIO29_OUT, BIT5),
    _RVM1(0x1e81, PAD_GPIO29_OEN, BIT5),
    #endif

    #if(PAD_GPIO30_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO30_OEN (PAD_GPIO30_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_GPIO30_OUT (PAD_GPIO30_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1ee8, PAD_GPIO30_OUT, BIT0),
    _RVM1(0x1ee9, PAD_GPIO30_OEN, BIT0),
    #endif

    #if(PAD_GPIO31_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO31_OEN (PAD_GPIO31_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_GPIO31_OUT (PAD_GPIO31_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1ee8, PAD_GPIO31_OUT, BIT1),
    _RVM1(0x1ee9, PAD_GPIO31_OEN, BIT1),
    #endif

    #if(PAD_GPIO32_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO32_OEN (PAD_GPIO32_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_GPIO32_OUT (PAD_GPIO32_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1ee8, PAD_GPIO32_OUT, BIT2),
    _RVM1(0x1ee9, PAD_GPIO32_OEN, BIT2),
    #endif

    #if(PAD_GPIO33_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO33_OEN (PAD_GPIO33_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_GPIO33_OUT (PAD_GPIO33_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1ee8, PAD_GPIO33_OUT, BIT3),
    _RVM1(0x1ee9, PAD_GPIO33_OEN, BIT3),
    #endif

    #if(PAD_GPIO34_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO34_OEN (PAD_GPIO34_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_GPIO34_OUT (PAD_GPIO34_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1ee8, PAD_GPIO34_OUT, BIT4),
    _RVM1(0x1ee9, PAD_GPIO34_OEN, BIT4),
    #endif

    #if(PAD_GPIO35_IS_GPIO != GPIO_NONE)
    #define PAD_GPIO35_OEN (PAD_GPIO35_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_GPIO35_OUT (PAD_GPIO35_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1ee8, PAD_GPIO35_OUT, BIT5),
    _RVM1(0x1ee9, PAD_GPIO35_OEN, BIT5),
    #endif

    #if(PAD_INT_IS_GPIO != GPIO_NONE)
    #define PAD_INT_OEN (PAD_INT_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_INT_OUT (PAD_INT_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e45, PAD_INT_OUT, BIT2),
    _RVM1(0x1e49, PAD_INT_OEN, BIT2),
    //reg_int_gpio = 0b
    _RVM1(0x1e4f, BIT2 , BIT2 ),   //reg[101e4f]#2 = 1b
    #endif

    #if(PAD_CEC_IS_GPIO != GPIO_NONE)
    #define PAD_CEC_OEN (PAD_CEC_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_CEC_OUT (PAD_CEC_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e08, PAD_CEC_OUT, BIT0),
    _RVM1(0x1e08, PAD_CEC_OEN, BIT1),
    //reg_cec_config = 0
    _RVM1(0x1efb, 0, BIT1|BIT0 ),   //reg[101efb]#1~#0 = 00b
    #endif

    _MEMMAP_PM_,

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
    //reg_ddcda_config[1:0] = 0b
    _RVM1(0x1efa, 0, BIT3|BIT2 ),   //reg[101efa]#3~#2 = 00b
    #endif

    #if(PAD_DDCDA_DAT_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDA_DAT_OEN (PAD_DDCDA_DAT_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_DDCDA_DAT_OUT (PAD_DDCDA_DAT_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e16, PAD_DDCDA_DAT_OUT, BIT1),
    _RVM1(0x1e18, PAD_DDCDA_DAT_OEN, BIT1),
    //reg_ddcda_config[1:0] = 0b
    _RVM1(0x1efa, 0, BIT3|BIT2 ),   //reg[101efa]#3~#2 = 00b
    #endif

    #if(PAD_HOTPLUG_A_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUG_A_OEN (PAD_HOTPLUG_A_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_HOTPLUG_A_OUT (PAD_HOTPLUG_A_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e17, PAD_HOTPLUG_A_OUT, BIT0),
    _RVM1(0x1e19, PAD_HOTPLUG_A_OEN, BIT0),
    //reg_hotpluga_gpio_en = 1b
    _RVM1(0x1e17, BIT5 , BIT5 ),   //reg[101e17]#5 = 1b
    //reg_mhl_cbus_config[1:0] = 00b
    _RVM1(0x1efa, 0, BIT1|BIT0 ),   //reg[101efa]#1~#0 = 00b
    #endif

    #if(PAD_DDCDB_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDB_CLK_OEN (PAD_DDCDB_CLK_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_DDCDB_CLK_OUT (PAD_DDCDB_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e16, PAD_DDCDB_CLK_OUT, BIT2),
    _RVM1(0x1e18, PAD_DDCDB_CLK_OEN, BIT2),
    //reg_ddcdb_config[1:0] = 0b
    _RVM1(0x1efa, 0, BIT5|BIT4 ),   //reg[101efa]#5~#4 = 00b
    #endif

    #if(PAD_DDCDB_DAT_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDB_DAT_OEN (PAD_DDCDB_DAT_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_DDCDB_DAT_OUT (PAD_DDCDB_DAT_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e16, PAD_DDCDB_DAT_OUT, BIT3),
    _RVM1(0x1e18, PAD_DDCDB_DAT_OEN, BIT3),
    //reg_ddcdb_config[1:0] = 0b
    _RVM1(0x1efa, 0, BIT5|BIT4 ),   //reg[101efa]#5~#4 = 00b
    #endif

    #if(PAD_HOTPLUG_B_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUG_B_OEN (PAD_HOTPLUG_B_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_HOTPLUG_B_OUT (PAD_HOTPLUG_B_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e17, PAD_HOTPLUG_B_OUT, BIT1),
    _RVM1(0x1e19, PAD_HOTPLUG_B_OEN, BIT1),
    //reg_mhl_cbus_config[1:0] = 00b
    _RVM1(0x1efa, 0, BIT1|BIT0 ),   //reg[101efa]#1~#0 = 00b
    #endif

    #if(PAD_DDCDC_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDC_CLK_OEN (PAD_DDCDC_CLK_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_DDCDC_CLK_OUT (PAD_DDCDC_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e16, PAD_DDCDC_CLK_OUT, BIT4),
    _RVM1(0x1e18, PAD_DDCDC_CLK_OEN, BIT4),
    //reg_ddcdb_config[1:0] = 0b
    _RVM1(0x1efa, 0, BIT7|BIT6 ),   //reg[101efa]#7~#6 = 00b
    #endif

    #if(PAD_DDCDC_DAT_IS_GPIO != GPIO_NONE)
    #define PAD_DDCDC_DAT_OEN (PAD_DDCDC_DAT_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_DDCDC_DAT_OUT (PAD_DDCDC_DAT_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e16, PAD_DDCDC_DAT_OUT, BIT5),
    _RVM1(0x1e18, PAD_DDCDC_DAT_OEN, BIT5),
    //reg_ddcdb_config[1:0] = 0b
    _RVM1(0x1efa, 0, BIT7|BIT6 ),   //reg[101efa]#7~#6 = 00b
    #endif

    #if(PAD_HOTPLUG_C_IS_GPIO != GPIO_NONE)
    #define PAD_HOTPLUG_C_OEN (PAD_HOTPLUG_C_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_HOTPLUG_C_OUT (PAD_HOTPLUG_C_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e17, PAD_HOTPLUG_C_OUT, BIT2),
    _RVM1(0x1e19, PAD_HOTPLUG_C_OEN, BIT2),
    //reg_mhl_cbus_config[1:0] = 00b
    _RVM1(0x1efa, 0, BIT1|BIT0 ),   //reg[101efa]#1~#0 = 00b
    #endif

    #if(PAD_MHL_DET_IS_GPIO != GPIO_NONE)
    #define PAD_MHL_DET_OEN (PAD_MHL_DET_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_MHL_DET_OUT (PAD_MHL_DET_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e17, PAD_MHL_DET_OUT, BIT3),
    _RVM1(0x1e19, PAD_MHL_DET_OEN, BIT3),
    //reg_mhl_det_en = 0b
    _RVM1(0x1ef9, 0, BIT0 ),   //reg[101ef9]#0 = 0b
    #endif

    #if(PAD_PM_SPI_CZ1_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_CZ1_OEN (PAD_PM_SPI_CZ1_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PM_SPI_CZ1_OUT (PAD_PM_SPI_CZ1_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e8e, PAD_PM_SPI_CZ1_OUT, BIT0),
    _RVM1(0x1e48, PAD_PM_SPI_CZ1_OEN, BIT0),
    //reg_i2soutconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT6|BIT5 ),   //reg[101eae]#6 ~#5 = 00b
    #endif

    #if(PAD_PM_SPI_CZ2_IS_GPIO != GPIO_NONE)
    #define PAD_PM_SPI_CZ2_OEN (PAD_PM_SPI_CZ2_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PM_SPI_CZ2_OUT (PAD_PM_SPI_CZ2_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e8e, PAD_PM_SPI_CZ2_OUT, BIT1),
    _RVM1(0x1e48, PAD_PM_SPI_CZ2_OEN, BIT1),
    //reg_spdifoutconfig[1:0] = 00b
    _RVM1(0x1eaf, 0, BIT1|BIT0 ),   //reg[101eaf]#1~#0 = 00b
    #endif

    #if(PAD_NAND_REZ_IS_GPIO != GPIO_NONE)
    #define PAD_NAND_REZ_OEN (PAD_NAND_REZ_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_NAND_REZ_OUT (PAD_NAND_REZ_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e43, PAD_NAND_REZ_OUT, BIT0),
    _RVM1(0x1e47, PAD_NAND_REZ_OEN, BIT0),
    //reg_spi_nor_mode[3] = 0b
    _RVM1(0x1e08, 0, BIT3 ),   //reg[101e08]#3 = 0b
    //reg_spi_nand_mode[4] = 0b
    _RVM1(0x1e08, 0, BIT4 ),   //reg[101e08]#4 = 0b
    //reg_nand_mode[1:0] = 0b
    _RVM1(0x1e4E, 0, BIT1|BIT0 ),   //reg[101e4E]#1 ~ #0 = 0b
    #endif

    #if(PAD_NAND_ALE_IS_GPIO != GPIO_NONE)
    #define PAD_NAND_ALE_OEN (PAD_NAND_ALE_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_NAND_ALE_OUT (PAD_NAND_ALE_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e43, PAD_NAND_ALE_OUT, BIT2),
    _RVM1(0x1e47, PAD_NAND_ALE_OEN, BIT2),
    //reg_spi_nor_mode[3] = 0b
    _RVM1(0x1e08, 0, BIT3 ),   //reg[101e08]#3 = 0b
    //reg_spi_nand_mode[4] = 0b
    _RVM1(0x1e08, 0, BIT4 ),   //reg[101e08]#4 = 0b
    //reg_nand_mode[1:0] = 0b
    _RVM1(0x1e4E, 0, BIT1|BIT0 ),   //reg[101e4E]#1 ~ #0 = 0b
    #endif

    #if(PAD_NAND_CLE_IS_GPIO != GPIO_NONE)
    #define PAD_NAND_CLE_OEN (PAD_NAND_CLE_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_NAND_CLE_OUT (PAD_NAND_CLE_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e43, PAD_NAND_CLE_OUT, BIT1),
    _RVM1(0x1e47, PAD_NAND_CLE_OEN, BIT1),
    //reg_nand_mode[1:0] = 0b
    _RVM1(0x1e4E, 0, BIT1|BIT0 ),   //reg[101e4E]#1 ~ #0 = 0b
    #endif

    #if(PAD_NAND_CEZ_IS_GPIO != GPIO_NONE)
    #define PAD_NAND_CEZ_OEN (PAD_NAND_CEZ_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_NAND_CEZ_OUT (PAD_NAND_CEZ_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e43, PAD_NAND_CEZ_OUT, BIT3),
    _RVM1(0x1e47, PAD_NAND_CEZ_OEN, BIT3),
    //reg_nand_mode[1:0] = 0b
    _RVM1(0x1e4E, 0, BIT1|BIT0 ),   //reg[101e4E]#1 ~ #0 = 0b
    #endif

    #if(PAD_NAND_WPZ_IS_GPIO != GPIO_NONE)
    #define PAD_NAND_WPZ_OEN (PAD_NAND_WPZ_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_NAND_WPZ_OUT (PAD_NAND_WPZ_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e43, PAD_NAND_WPZ_OUT, BIT4),
    _RVM1(0x1e47, PAD_NAND_WPZ_OEN, BIT4),
    //reg_spi_nand_mode[4] = 0b
    _RVM1(0x1e08, 0, BIT4 ),   //reg[101e08]#4 = 0b
    //reg_nand_mode[1:0] = 0b
    _RVM1(0x1e4E, 0, BIT1|BIT0 ),   //reg[101e4E]#1 ~ #0 = 0b
    #endif

    #if(PAD_NAND_WEZ_IS_GPIO != GPIO_NONE)
    #define PAD_NAND_WEZ_OEN (PAD_NAND_WEZ_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_NAND_WEZ_OUT (PAD_NAND_WEZ_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e43, PAD_NAND_WEZ_OUT, BIT5),
    _RVM1(0x1e47, PAD_NAND_WEZ_OEN, BIT5),
    //reg_spi_nand_mode[4] = 0b
    _RVM1(0x1e08, 0, BIT4 ),   //reg[101e08]#4 = 0b
    //reg_nand_mode[1:0] = 0b
    _RVM1(0x1e4E, 0, BIT1|BIT0 ),   //reg[101e4E]#1 ~ #0 = 0b
    #endif

    #if(PAD_NAND_RBZ_IS_GPIO != GPIO_NONE)
    #define PAD_NAND_RBZ_OEN (PAD_NAND_RBZ_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_NAND_RBZ_OUT (PAD_NAND_RBZ_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e43, PAD_NAND_RBZ_OUT, BIT6),
    _RVM1(0x1e47, PAD_NAND_RBZ_OEN, BIT6),
    //reg_spi_nor_mode[3] = 0b
    _RVM1(0x1e08, 0, BIT3 ),   //reg[101e08]#3 = 0b
    //reg_spi_nand_mode[4] = 0b
    _RVM1(0x1e08, 0, BIT4 ),   //reg[101e08]#4 = 0b
    //reg_nand_mode[1:0] = 0b
    _RVM1(0x1e4E, 0, BIT1|BIT0 ),   //reg[101e4E]#1 ~ #0 = 0b
    #endif

    #if(PAD_NAND_AD0_IS_GPIO != GPIO_NONE)
    #define PAD_NAND_AD0_OEN (PAD_NAND_AD0_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_NAND_AD0_OUT (PAD_NAND_AD0_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e42, PAD_NAND_AD0_OUT, BIT7),
    _RVM1(0x1e46, PAD_NAND_AD0_OEN, BIT7),
    //reg_nand_mode[1:0] = 0b
    _RVM1(0x1e4E, 0, BIT1|BIT0 ),   //reg[101e4E]#1 ~ #0 = 0b
    #endif

    #if(PAD_NAND_AD1_IS_GPIO != GPIO_NONE)
    #define PAD_NAND_AD1_OEN (PAD_NAND_AD1_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_NAND_AD1_OUT (PAD_NAND_AD1_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e42, PAD_NAND_AD1_OUT, BIT6),
    _RVM1(0x1e46, PAD_NAND_AD1_OEN, BIT6),
    //reg_nand_mode[1:0] = 0b
    _RVM1(0x1e4E, 0, BIT1|BIT0 ),   //reg[101e4E]#1 ~ #0 = 0b
    #endif

    #if(PAD_NAND_AD2_IS_GPIO != GPIO_NONE)
    #define PAD_NAND_AD2_OEN (PAD_NAND_AD2_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_NAND_AD2_OUT (PAD_NAND_AD2_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e42, PAD_NAND_AD2_OUT, BIT5),
    _RVM1(0x1e46, PAD_NAND_AD2_OEN, BIT5),
    //reg_nand_mode[1:0] = 0b
    _RVM1(0x1e4E, 0, BIT1|BIT0 ),   //reg[101e4E]#1 ~ #0 = 0b
    #endif

    #if(PAD_NAND_AD3_IS_GPIO != GPIO_NONE)
    #define PAD_NAND_AD3_OEN (PAD_NAND_AD3_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_NAND_AD3_OUT (PAD_NAND_AD3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e42, PAD_NAND_AD3_OUT, BIT4),
    _RVM1(0x1e46, PAD_NAND_AD3_OEN, BIT4),
    //reg_spdifoutconfig[1:0] = 00b
    _RVM1(0x1eaf, 0, BIT1|BIT0 ),   //reg[101eaf]#1~#0 = 00b
    //reg_nand_mode[1:0] = 0b
    _RVM1(0x1e4E, 0, BIT1|BIT0 ),   //reg[101e4E]#1 ~ #0 = 0b
    #endif

    #if(PAD_NAND_AD4_IS_GPIO != GPIO_NONE)
    #define PAD_NAND_AD4_OEN (PAD_NAND_AD4_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_NAND_AD4_OUT (PAD_NAND_AD4_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e42, PAD_NAND_AD4_OUT, BIT3),
    _RVM1(0x1e46, PAD_NAND_AD4_OEN, BIT3),
    //reg_i2sinconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT4|BIT3 ),   //reg[101eae]#4~#3 = 00b
    //reg_i2soutconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT6|BIT5 ),   //reg[101eae]#6~#5 = 00b
    //reg_nand_mode[1:0] = 0b
    _RVM1(0x1e4E, 0, BIT1|BIT0 ),   //reg[101e4E]#1 ~ #0 = 0b
    #endif

    #if(PAD_NAND_AD5_IS_GPIO != GPIO_NONE)
    #define PAD_NAND_AD5_OEN (PAD_NAND_AD5_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_NAND_AD5_OUT (PAD_NAND_AD5_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e42, PAD_NAND_AD5_OUT, BIT2),
    _RVM1(0x1e46, PAD_NAND_AD5_OEN, BIT2),
    //reg_i2sinconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT4|BIT3 ),   //reg[101eae]#4~#3 = 00b
    //reg_i2soutconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT6|BIT5 ),   //reg[101eae]#6~#5 = 00b
    //reg_nand_mode[1:0] = 0b
    _RVM1(0x1e4E, 0, BIT1|BIT0 ),   //reg[101e4E]#1 ~ #0 = 0b
    #endif

    #if(PAD_NAND_AD6_IS_GPIO != GPIO_NONE)
    #define PAD_NAND_AD6_OEN (PAD_NAND_AD6_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_NAND_AD6_OUT (PAD_NAND_AD6_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e42, PAD_NAND_AD6_OUT, BIT1),
    _RVM1(0x1e46, PAD_NAND_AD6_OEN, BIT1),
    //reg_i2sinconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT4|BIT3 ),   //reg[101eae]#4~#3 = 00b
    //reg_i2soutconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT6|BIT5 ),   //reg[101eae]#6~#5 = 00b
    //reg_nand_mode[1:0] = 0b
    _RVM1(0x1e4E, 0, BIT1|BIT0 ),   //reg[101e4E]#1 ~ #0 = 0b
    #endif

    #if(PAD_NAND_AD7_IS_GPIO != GPIO_NONE)
    #define PAD_NAND_AD7_OEN (PAD_NAND_AD7_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_NAND_AD7_OUT (PAD_NAND_AD7_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e42, PAD_NAND_AD7_OUT, BIT0),
    _RVM1(0x1e46, PAD_NAND_AD7_OEN, BIT0),
    //reg_i2sinconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT4|BIT3 ),   //reg[101eae]#4~#3 = 00b
    //reg_i2soutconfig[1:0] = 00b
    _RVM1(0x1eae, 0, BIT6|BIT5 ),   //reg[101eae]#6~#5 = 00b
    //reg_nand_mode[1:0] = 0b
    _RVM1(0x1e4E, 0, BIT1|BIT0 ),   //reg[101e4E]#1 ~ #0 = 0b
    #endif

    #if(PAD_PCM_A0_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A0_OEN (PAD_PCM_A0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PCM_A0_OUT (PAD_PCM_A0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e6c, PAD_PCM_A0_OUT, BIT0),
    _RVM1(0x1e6e, PAD_PCM_A0_OEN, BIT0),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    //reg_ts_out_mode[5:4] = 00b
    _RVM1(0x1eaa, 0, BIT5|BIT4 ),   //reg[101eaa]#5~#4 = 00b
    #endif

    #if(PAD_PCM_A1_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A1_OEN (PAD_PCM_A1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_A1_OUT (PAD_PCM_A1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e6c, PAD_PCM_A1_OUT, BIT1),
    _RVM1(0x1e6e, PAD_PCM_A1_OEN, BIT1),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    //reg_ts_out_mode[5:4] = 00b
    _RVM1(0x1eaa, 0, BIT5|BIT4 ),   //reg[101eaa]#5~#4 = 00b
    #endif

    #if(PAD_PCM_A2_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A2_OEN (PAD_PCM_A2_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_PCM_A2_OUT (PAD_PCM_A2_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e6c, PAD_PCM_A2_OUT, BIT2),
    _RVM1(0x1e6e, PAD_PCM_A2_OEN, BIT2),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    //reg_ts_out_mode[5:4] = 00b
    _RVM1(0x1eaa, 0, BIT5|BIT4 ),   //reg[101eaa]#5~#4 = 00b
    #endif

    #if(PAD_PCM_A3_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A3_OEN (PAD_PCM_A3_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_PCM_A3_OUT (PAD_PCM_A3_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e6c, PAD_PCM_A3_OUT, BIT3),
    _RVM1(0x1e6e, PAD_PCM_A3_OEN, BIT3),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    //reg_ts_out_mode[5:4] = 00b
    _RVM1(0x1eaa, 0, BIT5|BIT4 ),   //reg[101eaa]#5~#4 = 00b
    #endif

    #if(PAD_PCM_A4_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A4_OEN (PAD_PCM_A4_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_PCM_A4_OUT (PAD_PCM_A4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e6c, PAD_PCM_A4_OUT, BIT4),
    _RVM1(0x1e6e, PAD_PCM_A4_OEN, BIT4),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    //reg_fiveuartmode = 0b
    _RVM1(0x1e3e, 0, BIT1 ),   //reg[101e3e]#1 = 00b
    #endif

    #if(PAD_PCM_A5_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A5_OEN (PAD_PCM_A5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_PCM_A5_OUT (PAD_PCM_A5_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e6c, PAD_PCM_A5_OUT, BIT5),
    _RVM1(0x1e6e, PAD_PCM_A5_OEN, BIT5),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    //reg_fiveuartmode = 0b
    _RVM1(0x1e3e, 0, BIT1 ),   //reg[101e3e]#1 = 00b
    #endif

    #if(PAD_PCM_A6_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A6_OEN (PAD_PCM_A6_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_PCM_A6_OUT (PAD_PCM_A6_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e6c, PAD_PCM_A6_OUT, BIT6),
    _RVM1(0x1e6e, PAD_PCM_A6_OEN, BIT6),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    #endif

    #if(PAD_PCM_A7_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A7_OEN (PAD_PCM_A7_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_PCM_A7_OUT (PAD_PCM_A7_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e6c, PAD_PCM_A7_OUT, BIT7),
    _RVM1(0x1e6e, PAD_PCM_A7_OEN, BIT7),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    #endif

    #if(PAD_PCM_A8_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A8_OEN (PAD_PCM_A8_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PCM_A8_OUT (PAD_PCM_A8_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e6d, PAD_PCM_A8_OUT, BIT0),
    _RVM1(0x1e6f, PAD_PCM_A8_OEN, BIT0),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    #endif

    #if(PAD_PCM_A9_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A9_OEN (PAD_PCM_A9_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_A9_OUT (PAD_PCM_A9_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e6d, PAD_PCM_A9_OUT, BIT1),
    _RVM1(0x1e6f, PAD_PCM_A9_OEN, BIT1),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    #endif

    #if(PAD_PCM_A10_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A10_OEN (PAD_PCM_A10_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_PCM_A10_OUT (PAD_PCM_A10_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e6d, PAD_PCM_A10_OUT, BIT2),
    _RVM1(0x1e6f, PAD_PCM_A10_OEN, BIT2),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    #endif

    #if(PAD_PCM_A11_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A11_OEN (PAD_PCM_A11_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_PCM_A11_OUT (PAD_PCM_A11_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e6d, PAD_PCM_A11_OUT, BIT3),
    _RVM1(0x1e6f, PAD_PCM_A11_OEN, BIT3),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    #endif

    #if(PAD_PCM_A12_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A12_OEN (PAD_PCM_A12_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_PCM_A12_OUT (PAD_PCM_A12_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e6d, PAD_PCM_A12_OUT, BIT4),
    _RVM1(0x1e6f, PAD_PCM_A12_OEN, BIT4),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    //reg_seconduartmode = 00b
    _RVM1(0x1e05, 0, BIT1|BIT0 ),   //reg[101e05]#1~#0 = 00b
    //reg_fourthuartmode = 0b
    _RVM1(0x1e04, 0, BIT6 ),   //reg[101e04]#6 = 0b
    #endif

    #if(PAD_PCM_A13_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A13_OEN (PAD_PCM_A13_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_PCM_A13_OUT (PAD_PCM_A13_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e6d, PAD_PCM_A13_OUT, BIT5),
    _RVM1(0x1e6f, PAD_PCM_A13_OEN, BIT5),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    #endif

    #if(PAD_PCM_A14_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_A14_OEN (PAD_PCM_A14_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_PCM_A14_OUT (PAD_PCM_A14_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e6d, PAD_PCM_A14_OUT, BIT6),
    _RVM1(0x1e6f, PAD_PCM_A14_OEN, BIT6),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    //reg_seconduartmode = 00b
    _RVM1(0x1e05, 0, BIT1|BIT0 ),   //reg[101e05]#1~#0 = 00b
    //reg_fourthuartmode = 0b
    _RVM1(0x1e04, 0, BIT6 ),   //reg[101e04]#6 = 0b
    #endif

    #if(PAD_PCM_RESET_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_RESET_OEN (PAD_PCM_RESET_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_PCM_RESET_OUT (PAD_PCM_RESET_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e6d, PAD_PCM_RESET_OUT, BIT7),
    _RVM1(0x1e6f, PAD_PCM_RESET_OEN, BIT7),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmctrlconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 00b
    #endif

    #if(PAD_PCM_IRQA_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_IRQA_N_OEN (PAD_PCM_IRQA_N_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_PCM_IRQA_N_OUT (PAD_PCM_IRQA_N_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e72, PAD_PCM_IRQA_N_OUT, BIT0),
    _RVM1(0x1e74, PAD_PCM_IRQA_N_OEN, BIT0),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmctrlconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 00b
    #endif

    #if(PAD_PCM_CD_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_CD_N_OEN (PAD_PCM_CD_N_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_CD_N_OUT (PAD_PCM_CD_N_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e72, PAD_PCM_CD_N_OUT, BIT1),
    _RVM1(0x1e74, PAD_PCM_CD_N_OEN, BIT1),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmctrlconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 00b
    #endif

    #if(PAD_PCM_REG_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_REG_N_OEN (PAD_PCM_REG_N_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_PCM_REG_N_OUT (PAD_PCM_REG_N_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e72, PAD_PCM_REG_N_OUT, BIT2),
    _RVM1(0x1e74, PAD_PCM_REG_N_OEN, BIT2),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmctrlconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 00b
    #endif

    #if(PAD_PCM_CE_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_CE_N_OEN (PAD_PCM_CE_N_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_PCM_CE_N_OUT (PAD_PCM_CE_N_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e72, PAD_PCM_CE_N_OUT, BIT3),
    _RVM1(0x1e74, PAD_PCM_CE_N_OEN, BIT3),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmctrlconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 00b
    #endif

    #if(PAD_PCM_OE_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_OE_N_OEN (PAD_PCM_OE_N_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_PCM_OE_N_OUT (PAD_PCM_OE_N_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e72, PAD_PCM_OE_N_OUT, BIT4),
    _RVM1(0x1e74, PAD_PCM_OE_N_OEN, BIT4),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmctrlconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 00b
    #endif

    #if(PAD_PCM_WE_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_WE_N_OEN (PAD_PCM_WE_N_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_PCM_WE_N_OUT (PAD_PCM_WE_N_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e72, PAD_PCM_WE_N_OUT, BIT5),
    _RVM1(0x1e74, PAD_PCM_WE_N_OEN, BIT5),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmctrlconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 00b
    #endif

    #if(PAD_PCM_WAIT_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_WAIT_N_OEN (PAD_PCM_WAIT_N_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_PCM_WAIT_N_OUT (PAD_PCM_WAIT_N_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e72, PAD_PCM_WAIT_N_OUT, BIT6),
    _RVM1(0x1e74, PAD_PCM_WAIT_N_OEN, BIT6),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmctrlconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 00b
    #endif

    #if(PAD_PCM_IORD_N_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_IORD_N_OEN (PAD_PCM_IORD_N_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_PCM_IORD_N_OUT (PAD_PCM_IORD_N_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e72, PAD_PCM_IORD_N_OUT, BIT7),
    _RVM1(0x1e74, PAD_PCM_IORD_N_OEN, BIT7),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmctrlconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT5 ),   //reg[101ec8]#5 = 00b
    #endif

    #if(PAD_PCM_D0_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_D0_OEN (PAD_PCM_D0_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_PCM_D0_OUT (PAD_PCM_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e73, PAD_PCM_D0_OUT, BIT1),
    _RVM1(0x1e75, PAD_PCM_D0_OEN, BIT1),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    //reg_ts_out_mode[5:4] = 00b
    _RVM1(0x1eaa, 0, BIT5|BIT4 ),   //reg[101eaa]#5~#4 = 00b
    #endif

    #if(PAD_PCM_D1_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_D1_OEN (PAD_PCM_D1_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_PCM_D1_OUT (PAD_PCM_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e73, PAD_PCM_D1_OUT, BIT2),
    _RVM1(0x1e75, PAD_PCM_D1_OEN, BIT2),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    //reg_ts_out_mode[5:4] = 00b
    _RVM1(0x1eaa, 0, BIT5|BIT4 ),   //reg[101eaa]#5~#4 = 00b
    #endif

    #if(PAD_PCM_D2_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_D2_OEN (PAD_PCM_D2_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_PCM_D2_OUT (PAD_PCM_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e73, PAD_PCM_D2_OUT, BIT3),
    _RVM1(0x1e75, PAD_PCM_D2_OEN, BIT3),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    //reg_ts_out_mode[5:4] = 00b
    _RVM1(0x1eaa, 0, BIT5|BIT4 ),   //reg[101eaa]#5~#4 = 00b
    #endif

    #if(PAD_PCM_D3_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_D3_OEN (PAD_PCM_D3_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_PCM_D3_OUT (PAD_PCM_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e73, PAD_PCM_D3_OUT, BIT4),
    _RVM1(0x1e75, PAD_PCM_D3_OEN, BIT4),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    //reg_ts_out_mode[5:4] = 00b
    _RVM1(0x1eaa, 0, BIT5|BIT4 ),   //reg[101eaa]#5~#4 = 00b
    #endif

    #if(PAD_PCM_D4_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_D4_OEN (PAD_PCM_D4_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_PCM_D4_OUT (PAD_PCM_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e73, PAD_PCM_D4_OUT, BIT5),
    _RVM1(0x1e75, PAD_PCM_D4_OEN, BIT5),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_ddcda[1:0] = 00b
    _RVM1(0x1efa, 0, BIT3|BIT2 ),   //reg[101efa]#3~#2 = 00b
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    //reg_ts_out_mode[5:4] = 00b
    _RVM1(0x1eaa, 0, BIT5|BIT4 ),   //reg[101eaa]#5~#4 = 00b
    #endif

    #if(PAD_PCM_D5_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_D5_OEN (PAD_PCM_D5_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_PCM_D5_OUT (PAD_PCM_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e73, PAD_PCM_D5_OUT, BIT6),
    _RVM1(0x1e75, PAD_PCM_D5_OEN, BIT6),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_ddcda[1:0] = 00b
    _RVM1(0x1efa, 0, BIT3|BIT2 ),   //reg[101efa]#3~#2 = 00b
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    //reg_ts_out_mode[5:4] = 00b
    _RVM1(0x1eaa, 0, BIT5|BIT4 ),   //reg[101eaa]#5~#4 = 00b
    #endif

    #if(PAD_PCM_D6_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_D6_OEN (PAD_PCM_D6_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_PCM_D6_OUT (PAD_PCM_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e73, PAD_PCM_D6_OUT, BIT7),
    _RVM1(0x1e75, PAD_PCM_D6_OEN, BIT7),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_ddcdb[5:4] = 00b
    _RVM1(0x1efa, 0, BIT5|BIT4 ),   //reg[101efa]#5~#4 = 00b
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    //reg_ts_out_mode[5:4] = 00b
    _RVM1(0x1eaa, 0, BIT5|BIT4 ),   //reg[101eaa]#5~#4 = 00b
    #endif

    #if(PAD_PCM_D7_IS_GPIO != GPIO_NONE)
    #define PAD_PCM_D7_OEN (PAD_PCM_D7_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_PCM_D7_OUT (PAD_PCM_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1ea5, PAD_PCM_D7_OUT, BIT2),
    _RVM1(0x1ea5, PAD_PCM_D7_OEN, BIT4),
    //reg_pcm_pe[33:0] = 3ffffh
    _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh
    _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
    _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
    _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
    _RVM1(0x1e10, BIT1|BIT0 , BIT1|BIT0 ),   //reg[101e10]#1 ~#0 = 3h
    //reg_ddcdb[5:4] = 00b
    _RVM1(0x1efa, 0, BIT5|BIT4 ),   //reg[101efa]#5~#4 = 00b
    //reg_pcmadconfig[4] = 0b
    _RVM1(0x1ec8, 0, BIT4 ),   //reg[101ec8]#4 = 00b
    //reg_ts_out_mode[5:4] = 00b
    _RVM1(0x1eaa, 0, BIT5|BIT4 ),   //reg[101eaa]#5~#4 = 00b
    #endif

    #if(PAD_TS0_D0_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D0_OEN (PAD_TS0_D0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_TS0_D0_OUT (PAD_TS0_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e96, PAD_TS0_D0_OUT, BIT0),
    _RVM1(0x1e98, PAD_TS0_D0_OEN, BIT0),
    //reg_ts0_pe[7:0] = ffh
    _RVM1(0x1e12, 0xff, 0xff ),   //reg[101e12]#7~#0 = ffh
    //reg_ts0_pe[10:8] = 111b
    _RVM1(0x1e13, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101e13]#2~#0 = 111b
    //reg_ddcdc[7:6] = 00b
    _RVM1(0x1efa, 0, BIT7|BIT6 ),   //reg[101efa]#7~#6 = 00b
    //reg_ts0config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    //reg_ts_out_mode[5:4] = 00b
    _RVM1(0x1eaa, 0, BIT5|BIT4 ),   //reg[101eaa]#5~#4 = 00b
    #endif

    #if(PAD_TS0_D1_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D1_OEN (PAD_TS0_D1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS0_D1_OUT (PAD_TS0_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e96, PAD_TS0_D1_OUT, BIT1),
    _RVM1(0x1e98, PAD_TS0_D1_OEN, BIT1),
    //reg_ts0_pe[7:0] = ffh
    _RVM1(0x1e12, 0xff, 0xff ),   //reg[101e12]#7~#0 = ffh
    //reg_ts0_pe[10:8] = 0b
    _RVM1(0x1e13, 0, BIT2|BIT1|BIT0 ),   //reg[101e13]#2~#0 = 00b
    //reg_ddcdc[7:6] = 00b
    _RVM1(0x1efa, 0, BIT7|BIT6 ),   //reg[101efa]#7~#6 = 00b
    //reg_ts0config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    //reg_ts_out_mode[5:4] = 00b
    _RVM1(0x1eaa, 0, BIT5|BIT4 ),   //reg[101eaa]#5~#4 = 00b
    #endif

    #if(PAD_TS0_D2_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D2_OEN (PAD_TS0_D2_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_TS0_D2_OUT (PAD_TS0_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e96, PAD_TS0_D2_OUT, BIT2),
    _RVM1(0x1e98, PAD_TS0_D2_OEN, BIT2),
    //reg_ts0_pe[7:0] = ffh
    _RVM1(0x1e12, 0xff, 0xff ),   //reg[101e12]#7~#0 = ffh
    //reg_ts0_pe[10:8] = 111b
    _RVM1(0x1e13, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101e13]#2~#0 = 111b
    //reg_ts0config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    #endif

    #if(PAD_TS0_D3_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D3_OEN (PAD_TS0_D3_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_TS0_D3_OUT (PAD_TS0_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e96, PAD_TS0_D3_OUT, BIT3),
    _RVM1(0x1e98, PAD_TS0_D3_OEN, BIT3),
    //reg_ts0_pe[7:0] = ffh
    _RVM1(0x1e12, 0xff, 0xff ),   //reg[101e12]#7~#0 = ffh
    //reg_ts0_pe[10:8] = 111b
    _RVM1(0x1e13, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101e13]#2~#0 = 111b
    //reg_ts0config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    #endif

    #if(PAD_TS0_D4_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D4_OEN (PAD_TS0_D4_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_TS0_D4_OUT (PAD_TS0_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e96, PAD_TS0_D4_OUT, BIT4),
    _RVM1(0x1e98, PAD_TS0_D4_OEN, BIT4),
    //reg_ts0_pe[7:0] = ffh
    _RVM1(0x1e12, 0xff, 0xff ),   //reg[101e12]#7~#0 = ffh
    //reg_ts0_pe[10:8] = 111b
    _RVM1(0x1e13, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101e13]#2~#0 = 111b
    //reg_ts0config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    #endif

    #if(PAD_TS0_D5_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D5_OEN (PAD_TS0_D5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS0_D5_OUT (PAD_TS0_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e96, PAD_TS0_D5_OUT, BIT5),
    _RVM1(0x1e98, PAD_TS0_D5_OEN, BIT5),
    //reg_ts0_pe[7:0] = ffh
    _RVM1(0x1e12, 0xff, 0xff ),   //reg[101e12]#7~#0 = ffh
    //reg_ts0_pe[10:8] = 111b
    _RVM1(0x1e13, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101e13]#2~#0 = 111b
    //reg_ts0config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    #endif

    #if(PAD_TS0_D6_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D6_OEN (PAD_TS0_D6_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_TS0_D6_OUT (PAD_TS0_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e96, PAD_TS0_D6_OUT, BIT6),
    _RVM1(0x1e98, PAD_TS0_D6_OEN, BIT6),
    //reg_ts0_pe[7:0] = ffh
    _RVM1(0x1e12, 0xff, 0xff ),   //reg[101e12]#7~#0 = ffh
    //reg_ts0_pe[10:8] = 111b
    _RVM1(0x1e13, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101e13]#2~#0 = 111b
    //reg_ts0config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    #endif

    #if(PAD_TS0_D7_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_D7_OEN (PAD_TS0_D7_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_TS0_D7_OUT (PAD_TS0_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e96, PAD_TS0_D7_OUT, BIT7),
    _RVM1(0x1e98, PAD_TS0_D7_OEN, BIT7),
    //reg_ts0_pe[7:0] = ffh
    _RVM1(0x1e12, 0xff, 0xff ),   //reg[101e12]#7~#0 = ffh
    //reg_ts0_pe[10:8] = 111b
    _RVM1(0x1e13, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101e13]#2~#0 = 111b
    //reg_ts0config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    #endif

    #if(PAD_TS0_VLD_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_VLD_OEN (PAD_TS0_VLD_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_TS0_VLD_OUT (PAD_TS0_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e97, PAD_TS0_VLD_OUT, BIT0),
    _RVM1(0x1e99, PAD_TS0_VLD_OEN, BIT0),
    //reg_ts0_pe[7:0] = ffh
    _RVM1(0x1e12, 0xff, 0xff ),   //reg[101e12]#7~#0 = ffh
    //reg_ts0_pe[10:8] = 111b
    _RVM1(0x1e13, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101e13]#2~#0 = 111b
    //reg_ts0config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    #endif

    #if(PAD_TS0_SYNC_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_SYNC_OEN (PAD_TS0_SYNC_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS0_SYNC_OUT (PAD_TS0_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e97, PAD_TS0_SYNC_OUT, BIT1),
    _RVM1(0x1e99, PAD_TS0_SYNC_OEN, BIT1),
    //reg_ts0_pe[7:0] = ffh
    _RVM1(0x1e12, 0xff, 0xff ),   //reg[101e12]#7~#0 = ffh
    //reg_ts0_pe[10:8] = 111b
    _RVM1(0x1e13, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101e13]#2~#0 = 111b
    //reg_ts0config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    #endif

    #if(PAD_TS0_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_TS0_CLK_OEN (PAD_TS0_CLK_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_TS0_CLK_OUT (PAD_TS0_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e97, PAD_TS0_CLK_OUT, BIT2),
    _RVM1(0x1e99, PAD_TS0_CLK_OEN, BIT2),
    //reg_ts0_pe[7:0] = ffh
    _RVM1(0x1e12, 0xff, 0xff ),   //reg[101e12]#7~#0 = ffh
    //reg_ts0_pe[10:8] = 111b
    _RVM1(0x1e13, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101e13]#2~#0 = 111b
    //reg_ts0config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    #endif

    #if(PAD_TS1_D0_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D0_OEN (PAD_TS1_D0_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_TS1_D0_OUT (PAD_TS1_D0_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e52, PAD_TS1_D0_OUT, BIT0),
    _RVM1(0x1e54, PAD_TS1_D0_OEN, BIT0),
    //reg_ts1_pe[7:0] = ffb
    _RVM1(0x1efc, 0xff, 0xff ),   //reg[101efc]#7~#0 = ffh
    //reg_ts1_pe[10:8] = 111b
    _RVM1(0x1efd, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101efd]#2~#0 = 111b
    //reg_ts1config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    //reg_ts_out_mode[7:6] = 00b
    _RVM1(0x1eaa, 0, BIT7|BIT6 ),   //reg[101eaa]#7~#6 = 00b
    #endif

    #if(PAD_TS1_D1_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D1_OEN (PAD_TS1_D1_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS1_D1_OUT (PAD_TS1_D1_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e52, PAD_TS1_D1_OUT, BIT1),
    _RVM1(0x1e54, PAD_TS1_D1_OEN, BIT1),
    //reg_ts1_pe[7:0] = ffb
    _RVM1(0x1efc, 0xff, 0xff ),   //reg[101efc]#7~#0 = ffh
    //reg_ts1_pe[10:8] = 111b
    _RVM1(0x1efd, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101efd]#2~#0 = 111b
    //reg_ts1config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    //reg_ts_out_mode[5:4] = 00b
    _RVM1(0x1eaa, 0, BIT5|BIT4 ),   //reg[101eaa]#5~#4 = 00b
    #endif

    #if(PAD_TS1_D2_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D2_OEN (PAD_TS1_D2_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_TS1_D2_OUT (PAD_TS1_D2_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e52, PAD_TS1_D2_OUT, BIT2),
    _RVM1(0x1e54, PAD_TS1_D2_OEN, BIT2),
    //reg_ts1_pe[7:0] = ffb
    _RVM1(0x1efc, 0xff, 0xff ),   //reg[101efc]#7~#0 = ffh
    //reg_ts1_pe[10:8] = 111b
    _RVM1(0x1efd, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101efd]#2~#0 = 111b
    //reg_ts1config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    #endif

    #if(PAD_TS1_D3_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D3_OEN (PAD_TS1_D3_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_TS1_D3_OUT (PAD_TS1_D3_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e52, PAD_TS1_D3_OUT, BIT3),
    _RVM1(0x1e54, PAD_TS1_D3_OEN, BIT3),
    //reg_ts1_pe[7:0] = ffb
    _RVM1(0x1efc, 0xff, 0xff ),   //reg[101efc]#7~#0 = ffh
    //reg_ts1_pe[10:8] = 111b
    _RVM1(0x1efd, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101efd]#2~#0 = 111b
    //reg_ts1config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    #endif

    #if(PAD_TS1_D4_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D4_OEN (PAD_TS1_D4_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_TS1_D4_OUT (PAD_TS1_D4_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e52, PAD_TS1_D4_OUT, BIT4),
    _RVM1(0x1e54, PAD_TS1_D4_OEN, BIT4),
    //reg_ts1_pe[7:0] = ffb
    _RVM1(0x1efc, 0xff, 0xff ),   //reg[101efc]#7~#0 = ffh
    //reg_ts1_pe[10:8] = 111b
    _RVM1(0x1efd, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101efd]#2~#0 = 111b
    //reg_ts1config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    #endif

    #if(PAD_TS1_D5_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D5_OEN (PAD_TS1_D5_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS1_D5_OUT (PAD_TS1_D5_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e52, PAD_TS1_D5_OUT, BIT5),
    _RVM1(0x1e54, PAD_TS1_D5_OEN, BIT5),
    //reg_ts1_pe[7:0] = ffb
    _RVM1(0x1efc, 0xff, 0xff ),   //reg[101efc]#7~#0 = ffh
    //reg_ts1_pe[10:8] = 111b
    _RVM1(0x1efd, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101efd]#2~#0 = 111b
    //reg_ts1config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    #endif

    #if(PAD_TS1_D6_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D6_OEN (PAD_TS1_D6_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_TS1_D6_OUT (PAD_TS1_D6_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e52, PAD_TS1_D6_OUT, BIT6),
    _RVM1(0x1e54, PAD_TS1_D6_OEN, BIT6),
    //reg_ts1_pe[7:0] = ffb
    _RVM1(0x1efc, 0xff, 0xff ),   //reg[101efc]#7~#0 = ffh
    //reg_ts1_pe[10:8] = 111b
    _RVM1(0x1efd, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101efd]#2~#0 = 111b
    //reg_ts1config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    #endif

    #if(PAD_TS1_D7_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_D7_OEN (PAD_TS1_D7_IS_GPIO == GPIO_IN ? BIT7: 0)
    #define PAD_TS1_D7_OUT (PAD_TS1_D7_IS_GPIO == GPIO_OUT_HIGH ? BIT7: 0)
    _RVM1(0x1e52, PAD_TS1_D7_OUT, BIT7),
    _RVM1(0x1e54, PAD_TS1_D7_OEN, BIT7),
    //reg_ts1_pe[7:0] = ffb
    _RVM1(0x1efc, 0xff, 0xff ),   //reg[101efc]#7~#0 = ffh
    //reg_ts1_pe[10:8] = 111b
    _RVM1(0x1efd, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101efd]#2~#0 = 111b
    //reg_ts1config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    #endif

    #if(PAD_TS1_VLD_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_VLD_OEN (PAD_TS1_VLD_IS_GPIO == GPIO_IN ? BIT0: 0)
    #define PAD_TS1_VLD_OUT (PAD_TS1_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x1e55, PAD_TS1_VLD_OUT, BIT0),
    _RVM1(0x1e51, PAD_TS1_VLD_OEN, BIT0),
    //reg_ts1_pe[7:0] = ffb
    _RVM1(0x1efc, 0xff, 0xff ),   //reg[101efc]#7~#0 = ffh
    //reg_ts1_pe[10:8] = 111b
    _RVM1(0x1efd, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101efd]#2~#0 = 111b
    //reg_ts1config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    #endif

    #if(PAD_TS1_SYNC_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_SYNC_OEN (PAD_TS1_SYNC_IS_GPIO == GPIO_IN ? BIT1: 0)
    #define PAD_TS1_SYNC_OUT (PAD_TS1_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT1: 0)
    _RVM1(0x1e55, PAD_TS1_SYNC_OUT, BIT1),
    _RVM1(0x1e51, PAD_TS1_SYNC_OEN, BIT1),
    //reg_ts1_pe[7:0] = ffb
    _RVM1(0x1efc, 0xff, 0xff ),   //reg[101efc]#7~#0 = ffh
    //reg_ts1_pe[10:8] = 111b
    _RVM1(0x1efd, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101efd]#2~#0 = 111b
    //reg_ts1config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    #endif

    #if(PAD_TS1_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_TS1_CLK_OEN (PAD_TS1_CLK_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_TS1_CLK_OUT (PAD_TS1_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x1e55, PAD_TS1_CLK_OUT, BIT2),
    _RVM1(0x1e51, PAD_TS1_CLK_OEN, BIT2),
    //reg_ts1_pe[7:0] = ffb
    _RVM1(0x1efc, 0xff, 0xff ),   //reg[101efc]#7~#0 = ffh
    //reg_ts1_pe[10:8] = 111b
    _RVM1(0x1efd, BIT2|BIT1|BIT0 , BIT2|BIT1|BIT0 ),   //reg[101efd]#2~#0 = 111b
    //reg_ts1config[2:0] = 0b
    _RVM1(0x1eaf, 0, BIT4|BIT3|BIT2 ),   //reg[101eaf]#4~#2 = 00b
    #endif

    #if(PAD_TS2_D_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_D_OEN (PAD_TS2_D_IS_GPIO == GPIO_IN ? BIT3: 0)
    #define PAD_TS2_D_OUT (PAD_TS2_D_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x1e97, PAD_TS2_D_OUT, BIT3),
    _RVM1(0x1e99, PAD_TS2_D_OEN, BIT3),
    //reg_ts2_pe[7:0] = ffh
    _RVM1(0x1efe, 0xff, 0xff ),   //reg[101efe]#7~#0 = ffh
    //reg_ts2config[2:0] = 0b
    _RVM1(0x1eae, 0, BIT2|BIT1|BIT0 ),   //reg[101eae]#2~#0 = 00b
    //reg_ts_out_mode[7:6] = 00b
    _RVM1(0x1eaa, 0, BIT7|BIT6 ),   //reg[101eaa]#7~#6 = 00b
    #endif

    #if(PAD_TS2_VLD_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_VLD_OEN (PAD_TS2_VLD_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_TS2_VLD_OUT (PAD_TS2_VLD_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e97, PAD_TS2_VLD_OUT, BIT4),
    _RVM1(0x1e99, PAD_TS2_VLD_OEN, BIT4),
    //reg_ts2_pe[7:0] = ffh
    _RVM1(0x1efe, 0xff, 0xff ),   //reg[101efe]#7~#0 = ffh
    //reg_ts2config[2:0] = 0b
    _RVM1(0x1eae, 0, BIT2|BIT1|BIT0 ),   //reg[101eae]#2~#0 = 00b
    #endif

    #if(PAD_TS2_SYNC_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_SYNC_OEN (PAD_TS2_SYNC_IS_GPIO == GPIO_IN ? BIT5: 0)
    #define PAD_TS2_SYNC_OUT (PAD_TS2_SYNC_IS_GPIO == GPIO_OUT_HIGH ? BIT5: 0)
    _RVM1(0x1e97, PAD_TS2_SYNC_OUT, BIT5),
    _RVM1(0x1e99, PAD_TS2_SYNC_OEN, BIT5),
    //reg_ts2_pe[7:0] = ffh
    _RVM1(0x1efe, 0xff, 0xff ),   //reg[101efe]#7~#0 = ffh
    //reg_ts2config[2:0] = 0b
    _RVM1(0x1eae, 0, BIT2|BIT1|BIT0 ),   //reg[101eae]#2~#0 = 00b
    #endif

    #if(PAD_TS2_CLK_IS_GPIO != GPIO_NONE)
    #define PAD_TS2_CLK_OEN (PAD_TS2_CLK_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_TS2_CLK_OUT (PAD_TS2_CLK_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x1e97, PAD_TS2_CLK_OUT, BIT6),
    _RVM1(0x1e99, PAD_TS2_CLK_OEN, BIT6),
    //reg_ts2_pe[7:0] = ffh
    _RVM1(0x1efe, 0xff, 0xff ),   //reg[101efe]#7~#0 = ffh
    //reg_ts2config[2:0] = 0b
    _RVM1(0x1eae, 0, BIT2|BIT1|BIT0 ),   //reg[101eae]#2~#0 = 00b
    #endif

    #if(PAD_ARC_IS_GPIO != GPIO_NONE)
    #define PAD_ARC_OEN (PAD_ARC_IS_GPIO == GPIO_IN ? BIT4: 0)
    #define PAD_ARC_OUT (PAD_ARC_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x1e17, PAD_ARC_OUT, BIT4),
    _RVM1(0x1e19, PAD_ARC_OEN, BIT4),
    //reg_arc_gpio_enable = 1b
    _RVM1(0x1e19, BIT6, BIT6),   //reg[101e19]#6= 1b
    //reg_arc_mode = 0b
    _RVM1(0x1e19, 0, BIT5),   //reg[101e19]#5= 0b
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

    #if(PADA_HSYNC2_IS_GPIO != GPIO_NONE)
    #define PADA_HSYNC2_OEN (PADA_HSYNC2_IS_GPIO == GPIO_IN ? 0 : BIT2)
    #define PADA_HSYNC2_OUT (PADA_HSYNC2_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x255b, PADA_HSYNC2_OUT, BIT0),
    _RVM1(0x255a, PADA_HSYNC2_OEN, BIT2),
    //reg_test_hsync[5] = 0b
    _RVM1(0x2560, 0, BIT5 ),   //reg[102560]#5 = 0b
    //reg_gpio_hsync_en[2] = 1b
    _RVM1(0x2558, BIT2 , BIT2 ),   //reg[102558]#2 = 1b
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

    #if(PADA_VSYNC2_IS_GPIO != GPIO_NONE)
    #define PADA_VSYNC2_OEN (PADA_VSYNC2_IS_GPIO == GPIO_IN ? 0 : BIT5)
    #define PADA_VSYNC2_OUT (PADA_VSYNC2_IS_GPIO == GPIO_OUT_HIGH ? BIT3: 0)
    _RVM1(0x255b, PADA_VSYNC2_OUT, BIT3),
    _RVM1(0x255a, PADA_VSYNC2_OEN, BIT5),
    //reg_test_vsync[2] = 0b
    _RVM1(0x2561, 0, BIT2 ),   //reg[102561]#2 = 0b
    //reg_gpio_vsync_en[5] = 1b
    _RVM1(0x2558, BIT5 , BIT5 ),   //reg[102558]#5 = 1b
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

    #if(PADA_RIN2P_IS_GPIO != GPIO_NONE)
    #define PADA_RIN2P_OEN (PADA_RIN2P_IS_GPIO == GPIO_IN ? 0 : BIT2)
    #define PADA_RIN2P_OUT (PADA_RIN2P_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x2537, PADA_RIN2P_OUT, BIT2),
    _RVM1(0x2536, PADA_RIN2P_OEN, BIT2),
    //reg_gpio_rgb_en[2] = 1b
    _RVM1(0x2534, BIT2 , BIT2 ),   //reg[102534]#2 = 1b
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

    #if(PADA_RIN2M_IS_GPIO != GPIO_NONE)
    #define PADA_RIN2M_OEN (PADA_RIN2M_IS_GPIO == GPIO_IN ? 0 : BIT6)
    #define PADA_RIN2M_OUT (PADA_RIN2M_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x2537, PADA_RIN2M_OUT, BIT6),
    _RVM1(0x2536, PADA_RIN2M_OEN, BIT6),
    //reg_gpio_rgb_en[2] = 1b
    _RVM1(0x2534, BIT2 , BIT2 ),   //reg[102534]#2 = 1b
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

    #if(PADA_GIN2P_IS_GPIO != GPIO_NONE)
    #define PADA_GIN2P_OEN (PADA_GIN2P_IS_GPIO == GPIO_IN ? 0 : BIT2)
    #define PADA_GIN2P_OUT (PADA_GIN2P_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x2539, PADA_GIN2P_OUT, BIT2),
    _RVM1(0x2538, PADA_GIN2P_OEN, BIT2),
    //reg_gpio_rgb_en[2] = 1b
    _RVM1(0x2534, BIT2 , BIT2 ),   //reg[102534]#2 = 1b
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

    #if(PADA_GIN2M_IS_GPIO != GPIO_NONE)
    #define PADA_GIN2M_OEN (PADA_GIN2M_IS_GPIO == GPIO_IN ? 0 : BIT6)
    #define PADA_GIN2M_OUT (PADA_GIN2M_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x2539, PADA_GIN2M_OUT, BIT6),
    _RVM1(0x2538, PADA_GIN2M_OEN, BIT6),
    //reg_gpio_rgb_en[2] = 1b
    _RVM1(0x2534, BIT2 , BIT2 ),   //reg[102534]#2 = 1b
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

    #if(PADA_BIN2P_IS_GPIO != GPIO_NONE)
    #define PADA_BIN2P_OEN (PADA_BIN2P_IS_GPIO == GPIO_IN ? 0 : BIT2)
    #define PADA_BIN2P_OUT (PADA_BIN2P_IS_GPIO == GPIO_OUT_HIGH ? BIT2: 0)
    _RVM1(0x253b, PADA_BIN2P_OUT, BIT2),
    _RVM1(0x253a, PADA_BIN2P_OEN, BIT2),
    //reg_gpio_rgb_en[2] = 1b
    _RVM1(0x2534, BIT2 , BIT2 ),   //reg[102534]#2 = 1b
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

    #if(PADA_BIN2M_IS_GPIO != GPIO_NONE)
    #define PADA_BIN2M_OEN (PADA_BIN2M_IS_GPIO == GPIO_IN ? 0 : BIT6)
    #define PADA_BIN2M_OUT (PADA_BIN2M_IS_GPIO == GPIO_OUT_HIGH ? BIT6: 0)
    _RVM1(0x253b, PADA_BIN2M_OUT, BIT6),
    _RVM1(0x253a, PADA_BIN2M_OEN, BIT6),
    //reg_gpio_rgb_en[2] = 1b
    _RVM1(0x2534, BIT2 , BIT2 ),   //reg[102534]#2 = 1b
    #endif

    _MEMMAP_nonPM_11_,

    #if(PAD_LINEIN_L1_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_L1_OEN (PAD_LINEIN_L1_IS_GPIO == GPIO_IN ? BIT2 : 0)
    #define PAD_LINEIN_L1_OUT (PAD_LINEIN_L1_IS_GPIO == GPIO_OUT_HIGH ? BIT0 : 0)
    _RVM1(0x2cf7, PAD_LINEIN_L1_OUT, BIT0 ),
    _RVM1(0x2cf7, PAD_LINEIN_L1_OEN, BIT2 ),
    _RVM1(0x2cf7, BIT1 , BIT1 ),   //reg[112cf7]#1 = 1b
    #endif

    #if(PAD_LINEIN_R1_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_R1_OEN (PAD_LINEIN_R1_IS_GPIO == GPIO_IN ? BIT6 : 0)
    #define PAD_LINEIN_R1_OUT (PAD_LINEIN_R1_IS_GPIO == GPIO_OUT_HIGH ? BIT4 : 0)
    _RVM1(0x2cf7, PAD_LINEIN_R1_OUT, BIT4 ),
    _RVM1(0x2cf7, PAD_LINEIN_R1_OEN, BIT6 ),
    _RVM1(0x2cf7, BIT5 , BIT5 ),   //reg[112cf7]#5 = 1b
    #endif

    #if(PAD_LINEIN_L2_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_L2_OEN (PAD_LINEIN_L2_IS_GPIO == GPIO_IN ? BIT2  : 0)
    #define PAD_LINEIN_L2_OUT (PAD_LINEIN_L2_IS_GPIO == GPIO_OUT_HIGH ? BIT0  : 0)
    _RVM1(0x2cf8, PAD_LINEIN_L2_OUT, BIT0  ),
    _RVM1(0x2cf8, PAD_LINEIN_L2_OEN, BIT2  ),
    _RVM1(0x2cf8, BIT1 , BIT1 ),   //reg[112cf8]#1 = 1b
    #endif

    #if(PAD_LINEIN_R2_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_R2_OEN (PAD_LINEIN_R2_IS_GPIO == GPIO_IN ? BIT6  : 0)
    #define PAD_LINEIN_R2_OUT (PAD_LINEIN_R2_IS_GPIO == GPIO_OUT_HIGH ? BIT4  : 0)
    _RVM1(0x2cf8, PAD_LINEIN_R2_OUT, BIT4  ),
    _RVM1(0x2cf8, PAD_LINEIN_R2_OEN, BIT6  ),
    _RVM1(0x2cf8, BIT5 , BIT5 ),   //reg[112cf8]#5 = 1b
    #endif

    #if(PAD_LINEIN_L3_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_L3_OEN (PAD_LINEIN_L3_IS_GPIO == GPIO_IN ? BIT2 : 0)
    #define PAD_LINEIN_L3_OUT (PAD_LINEIN_L3_IS_GPIO == GPIO_OUT_HIGH ? BIT0 : 0)
    _RVM1(0x2cf9, PAD_LINEIN_L3_OUT, BIT0 ),
    _RVM1(0x2cf9, PAD_LINEIN_L3_OEN, BIT2 ),
    _RVM1(0x2cf9, BIT1 , BIT1 ),   //reg[112cf9]#1 = 1b
    #endif

    #if(PAD_LINEIN_R3_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_R3_OEN (PAD_LINEIN_R3_IS_GPIO == GPIO_IN ? BIT6 : 0)
    #define PAD_LINEIN_R3_OUT (PAD_LINEIN_R3_IS_GPIO == GPIO_OUT_HIGH ? BIT4 : 0)
    _RVM1(0x2cf9, PAD_LINEIN_R3_OUT, BIT4 ),
    _RVM1(0x2cf9, PAD_LINEIN_R3_OEN, BIT6 ),
    _RVM1(0x2cf9, BIT5 , BIT5 ),   //reg[112cf9]#5 = 1b
    #endif

    #if(PAD_LINEIN_L4_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_L4_OEN (PAD_LINEIN_L4_IS_GPIO == GPIO_IN ? BIT2  : 0)
    #define PAD_LINEIN_L4_OUT (PAD_LINEIN_L4_IS_GPIO == GPIO_OUT_HIGH ? BIT0  : 0)
    _RVM1(0x2cfa, PAD_LINEIN_L4_OUT, BIT0  ),
    _RVM1(0x2cfa, PAD_LINEIN_L4_OEN, BIT2  ),
    _RVM1(0x2cfa, BIT1 , BIT1 ),   //reg[112cfa]#1 = 1b
    #endif

    #if(PAD_LINEIN_R4_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_R4_OEN (PAD_LINEIN_R4_IS_GPIO == GPIO_IN ? BIT6  : 0)
    #define PAD_LINEIN_R4_OUT (PAD_LINEIN_R4_IS_GPIO == GPIO_OUT_HIGH ? BIT4  : 0)
    _RVM1(0x2cfa, PAD_LINEIN_R4_OUT, BIT4  ),
    _RVM1(0x2cfa, PAD_LINEIN_R4_OEN, BIT6  ),
    _RVM1(0x2cfa, BIT5 , BIT5 ),   //reg[112cfa]#5 = 1b
    #endif

    #if(PAD_LINEIN_L5_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_L5_OEN (PAD_LINEIN_L5_IS_GPIO == GPIO_IN ? BIT2: 0)
    #define PAD_LINEIN_L5_OUT (PAD_LINEIN_L5_IS_GPIO == GPIO_OUT_HIGH ? BIT0: 0)
    _RVM1(0x2cfb, PAD_LINEIN_L5_OUT, BIT0),
    _RVM1(0x2cfb, PAD_LINEIN_L5_OEN, BIT2),
    _RVM1(0x2cfb, BIT1 , BIT1 ),   //reg[112cfb]#1 = 1b
    #endif

    #if(PAD_LINEIN_R5_IS_GPIO != GPIO_NONE)
    #define PAD_LINEIN_R5_OEN (PAD_LINEIN_R5_IS_GPIO == GPIO_IN ? BIT6: 0)
    #define PAD_LINEIN_R5_OUT (PAD_LINEIN_R5_IS_GPIO == GPIO_OUT_HIGH ? BIT4: 0)
    _RVM1(0x2cfb, PAD_LINEIN_R5_OUT, BIT4),
    _RVM1(0x2cfb, PAD_LINEIN_R5_OEN, BIT6),
    _RVM1(0x2cfb, BIT5 , BIT5 ),   //reg[112cfb]#5 = 1b
    #endif

    #if((PAD_RXCN_A_IS_GPIO != GPIO_NONE)||(PAD_RX0N_A_IS_GPIO != GPIO_NONE)||\
        (PAD_RX1N_A_IS_GPIO != GPIO_NONE)||(PAD_RX2N_A_IS_GPIO != GPIO_NONE)||\
        (PAD_RXCP_A_IS_GPIO != GPIO_NONE)||(PAD_RX0P_A_IS_GPIO != GPIO_NONE)||\
        (PAD_RX1P_A_IS_GPIO != GPIO_NONE)||(PAD_RX2P_A_IS_GPIO != GPIO_NONE))
    #define PAD_RXCP_A_OEN (PAD_RXCP_A_IS_GPIO == GPIO_IN ? BIT4 : BIT0)
    _RVM1(0x09f9, PAD_RXCP_A_OEN, BIT4|BIT0),
    #endif

    #if((PAD_RXCN_B_IS_GPIO != GPIO_NONE)||(PAD_RX0N_B_IS_GPIO != GPIO_NONE)||\
        (PAD_RX1N_B_IS_GPIO != GPIO_NONE)||(PAD_RX2N_B_IS_GPIO != GPIO_NONE)||\
        (PAD_RXCP_B_IS_GPIO != GPIO_NONE)||(PAD_RX0P_B_IS_GPIO != GPIO_NONE)||\
        (PAD_RX1P_B_IS_GPIO != GPIO_NONE)||(PAD_RX2P_B_IS_GPIO != GPIO_NONE))
    #define PAD_RXCP_B_OEN (PAD_RXCP_B_IS_GPIO == GPIO_IN ? BIT5 : BIT1)
    _RVM1(0x09f9, PAD_RXCP_B_OEN, BIT5|BIT1),
    #endif

    #if(PAD_RXCN_D_IS_GPIO != GPIO_NONE)
    #define PAD_RXCN_D_OEN (PAD_RXCN_D_IS_GPIO == GPIO_IN ? BIT6:BIT0)
    #define PAD_RXCN_D_OUT (PAD_RXCN_D_IS_GPIO == GPIO_OUT_HIGH ? BIT0:0)
    _RVM1(0x09f9, PAD_RXCN_D_OEN, BIT6|BIT0),
    #if(PAD_RXCN_D_IS_GPIO != GPIO_IN)
    _RVM1(0x09f1, BIT0, BIT0),
    #endif
    _RVM1(0x09f0, PAD_RXCN_D_OUT, BIT0),
    #endif

    #if(PAD_RX0N_D_IS_GPIO != GPIO_NONE)
    #define PAD_RX0N_D_OEN (PAD_RX0N_D_IS_GPIO == GPIO_IN ? BIT6:BIT1)
    #define PAD_RX0N_D_OUT (PAD_RX0N_D_IS_GPIO == GPIO_OUT_HIGH ? BIT1:0)
    _RVM1(0x09f9, PAD_RX0N_D_OEN, BIT1|BIT6),
    #if(PAD_RX0N_D_IS_GPIO != GPIO_IN)
    _RVM1(0x09f1, BIT0, BIT0),
    #endif
    _RVM1(0x09f0, PAD_RX0N_D_OUT, BIT1),
    #endif

    #if(PAD_RX1N_D_IS_GPIO != GPIO_NONE)
    #define PAD_RX1N_D_OEN (PAD_RX1N_D_IS_GPIO == GPIO_IN ? BIT6:BIT2)
    #define PAD_RX1N_D_OUT (PAD_RX1N_D_IS_GPIO == GPIO_OUT_HIGH ? BIT2:0)
    _RVM1(0x09f9, PAD_RX1N_D_OEN, BIT2|BIT6),
    #if(PAD_RX1N_D_IS_GPIO != GPIO_IN)
    _RVM1(0x09f1, BIT0, BIT0),
    #endif
    _RVM1(0x09f0, PAD_RX1N_D_OUT, BIT2),
    #endif

    #if(PAD_RX2N_D_IS_GPIO != GPIO_NONE)
    #define PAD_RX2N_D_OEN_H (PAD_RX2N_D_IS_GPIO == GPIO_IN ? 0:BIT2)
    #define PAD_RX2N_D_OEN_L (PAD_RX2N_D_IS_GPIO == GPIO_IN ? BIT6:0)
    #define PAD_RX2N_D_OUT (PAD_RX2N_D_IS_GPIO == GPIO_OUT_HIGH ? BIT3:0)
    _RVM1(0x09c7, PAD_RX2N_D_OEN_H, BIT2),
    _RVM1(0x09f9, PAD_RX2N_D_OEN_L, BIT6),
    #if(PAD_RX2N_D_IS_GPIO != GPIO_IN)
    _RVM1(0x09f1, BIT0, BIT0),
    #endif
    _RVM1(0x09f0, PAD_RX2N_D_OUT, BIT3),
    #endif

    #if(PAD_RXCP_D_IS_GPIO != GPIO_NONE)
    #define PAD_RXCP_D_OEN (PAD_RXCP_D_IS_GPIO == GPIO_IN ? BIT6:BIT0)
    #define PAD_RXCP_D_OUT (PAD_RXCP_D_IS_GPIO == GPIO_OUT_HIGH ? BIT4:0)
    _RVM1(0x09f9, PAD_RXCP_D_OEN, BIT0|BIT6),
    #if(PAD_RXCP_D_IS_GPIO != GPIO_IN)
    _RVM1(0x09f1, BIT0, BIT0),
    #endif
    _RVM1(0x09f0, PAD_RXCP_D_OUT, BIT4),
    #endif

    #if(PAD_RX0P_D_IS_GPIO != GPIO_NONE)
    #define PAD_RX0P_D_OEN (PAD_RX0P_D_IS_GPIO == GPIO_IN ? BIT6:BIT1)
    #define PAD_RX0P_D_OUT (PAD_RX0P_D_IS_GPIO == GPIO_OUT_HIGH ? BIT5:0)
    _RVM1(0x09f9, PAD_RX0P_D_OEN, BIT1|BIT6),
    #if(PAD_RX0P_D_IS_GPIO != GPIO_IN)
    _RVM1(0x09f1, BIT0, BIT0),
    #endif
    _RVM1(0x09f0, PAD_RX0P_D_OUT, BIT5),
    #endif

    #if(PAD_RX1P_D_IS_GPIO != GPIO_NONE)
    #define PAD_RX1P_D_OEN (PAD_RX1P_D_IS_GPIO == GPIO_IN ? BIT6:BIT2)
    #define PAD_RX1P_D_OUT (PAD_RX1P_D_IS_GPIO == GPIO_OUT_HIGH ? BIT6:0)
    _RVM1(0x09f9, PAD_RX1P_D_OEN, BIT2|BIT6),
    #if(PAD_RX1P_D_IS_GPIO != GPIO_IN)
    _RVM1(0x09f1, BIT0, BIT0),
    #endif
    _RVM1(0x09f0, PAD_RX1P_D_OUT, BIT6),
    #endif

    #if(PAD_RX2P_D_IS_GPIO != GPIO_NONE)
    #define PAD_RX2P_D_OEN_H (PAD_RX2P_D_IS_GPIO == GPIO_IN ? 0:BIT2)
    #define PAD_RX2P_D_OEN_L (PAD_RX2P_D_IS_GPIO == GPIO_IN ? BIT6:0)
    #define PAD_RX2P_D_OUT (PAD_RX2P_D_IS_GPIO == GPIO_OUT_HIGH ? BIT7:0)
    _RVM1(0x09c7, PAD_RX2P_D_OEN_H, BIT2),
    _RVM1(0x09f9, PAD_RX2P_D_OEN_L, BIT6),
    #if(PAD_RX2P_D_IS_GPIO != GPIO_IN)
    _RVM1(0x09f1, BIT0, BIT0),
    #endif
    _RVM1(0x09f0, PAD_RX2P_D_OUT, BIT7),
    #endif

    _MEMMAP_nonPM_12_,

    #if(PAD_TP_IS_GPIO != GPIO_NONE)
    #define PAD_TP_OEN (PAD_TP_IS_GPIO == GPIO_IN ? BIT0: 0)
    _RVM1(0x1af0, PAD_TP_OEN, BIT0),
    //reg_atop_rx_inoff = 1b
    _RVM1(0x19d3, BIT6 , BIT6 ),   //reg[1219d3]#6 = 1b
    //reg_eth_gpio_en[3:0] = 5h
    _RVM1(0x1aee, BIT2|BIT0, BIT3|BIT2|BIT1|BIT0 ),   //reg[121aee]#3~#0 = 5h
    #endif

    #if(PAD_TN_IS_GPIO != GPIO_NONE)
    #define PAD_TN_OEN (PAD_TN_IS_GPIO == GPIO_IN ? BIT1: 0)
    _RVM1(0x1af0, PAD_TN_OEN, BIT1),
    //reg_atop_rx_inoff = 1b
    _RVM1(0x19d3, BIT6 , BIT6 ),   //reg[1219d3]#6 = 1b
    //reg_eth_gpio_en[3:0] = 5h
    _RVM1(0x1aee, BIT2|BIT0, BIT3|BIT2|BIT1|BIT0 ),   //reg[121aee]#3~#0 = 5h
    #endif

    #if(PAD_RP_IS_GPIO != GPIO_NONE)
    #define PAD_RP_OEN (PAD_RP_IS_GPIO == GPIO_IN ? BIT2: 0)
    _RVM1(0x1af0, PAD_RP_OEN, BIT2),
    //reg_atop_rx_inoff = 1b
    _RVM1(0x19d3, BIT6 , BIT6 ),   //reg[1219d3]#6 = 1b
    //reg_eth_gpio_en[3:0] = 5h
    _RVM1(0x1aee, BIT2|BIT0, BIT3|BIT2|BIT1|BIT0 ),   //reg[121aee]#3~#0 = 5h
    #endif

    #if(PAD_RN_IS_GPIO != GPIO_NONE)
    #define PAD_RN_OEN (PAD_RN_IS_GPIO == GPIO_IN ? BIT3: 0)
    _RVM1(0x1af0, PAD_RN_OEN, BIT3),
    //reg_atop_rx_inoff = 1b
    _RVM1(0x19d3, BIT6 , BIT6 ),   //reg[1219d3]#6 = 1b
    //reg_eth_gpio_en[3:0] = 5h
    _RVM1(0x1aee, BIT2|BIT0, BIT3|BIT2|BIT1|BIT0 ),   //reg[121aee]#3~#0 = 5h
    #endif

//---------------------------------------------------------------------
// Pad Configuartion
//---------------------------------------------------------------------
    _MEMMAP_nonPM_,

#if (PADS_TCON_MODE != Unknown_pad_mux)

//Configure TCON signal pads to be used
#define TCON0    ((PADS_TCON_CONFIG & BIT0) ? BIT0 : 0)
#define TCON1    ((PADS_TCON_CONFIG & BIT1) ? BIT1 : 0)
#define TCON2    ((PADS_TCON_CONFIG & BIT2) ? BIT2 : 0)
#define TCON3    ((PADS_TCON_CONFIG & BIT3) ? BIT3 : 0)
#define TCON4    ((PADS_TCON_CONFIG & BIT4) ? BIT4 : 0)
#define TCON5    ((PADS_TCON_CONFIG & BIT5) ? BIT5 : 0)
#define TCON6    ((PADS_TCON_CONFIG & BIT6) ? BIT6 : 0)
#define TCON7    ((PADS_TCON_CONFIG & BIT7) ? BIT7 : 0)
#define SET_TCON TCON0|TCON1|TCON2|TCON3|TCON4|TCON5|TCON6|TCON7
//tcon signal output from TCONX pads :  PAD_TCON0 ~ PAD_TCON7
#define TCON_CFG_TCON ((PADS_TCON_MODE == TCON_MODE(1))? SET_TCON : 0)
            _RVM1(0x1EF2, TCON_CFG_TCON, 0xFF),
//tcon signal output from PCMX pads : PAD_PCM_A[7] ~ PAD_PCM_A[14]
#define TCON_CFG_PCM ((PADS_TCON_MODE == TCON_MODE(2))? SET_TCON : 0)
            _RVM1(0x1E3D, TCON_CFG_PCM, 0xFF),
#endif

#if (PADS_UART2_MODE != Unknown_pad_mux)
#define UART2_CFG   ((PADS_UART2_MODE == UART2_MODE(1)) ? BIT0: \
                                       (PADS_UART2_MODE == UART2_MODE(2)) ? BIT1: 0)
            _RVM1(0x1E05, UART2_CFG, BIT1|BIT0),
#endif

#if (PADS_UART3_MODE != Unknown_pad_mux)
#define UART3_CFG1  ((PADS_UART3_MODE == UART3_MODE(1)) ? BIT0: 0)
#define UART3_CFG2  ((PADS_UART3_MODE == UART3_MODE(2)) ? BIT2: 0)
            _RVM1(0x1E3C, UART3_CFG1, BIT0),
            _RVM1(0x1E05, UART3_CFG2, BIT3|BIT2),
#endif

#if (PADS_UART4_MODE != Unknown_pad_mux)
#define UART4_CFG1  ((PADS_UART4_MODE == UART4_MODE(1)) ? BIT6: 0)
#define UART4_CFG2  ((PADS_UART4_MODE == UART4_MODE(2)) ? BIT1: 0)
            _RVM1(0x1E04, UART4_MODE1, BIT7|BIT6),
            _RVM1(0x1E3F, UART4_MODE2, BIT1),

#endif

#if (PADS_UARTF_MODE != Unknown_pad_mux)
#define UARTF_CFG   ((PADS_UARTF_MODE == UARTF_MODE(1)) ? BIT4: 0)
             _RVM1(0x1E04, UARTF_CFG, BIT5|BIT4),
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
#define XC3DLR_CFG   ((PADS_3DLR_MODE == XC3DLR_MODE(1)) ? BIT5: \
                                      (PADS_3DLR_MODE == XC3DLR_MODE(2)) ? BIT6: \
                                      (PADS_3DLR_MODE == XC3DLR_MODE(3)) ? (BIT6|BIT5): \
                                      (PADS_3DLR_MODE == XC3DLR_MODE(4)) ? BIT7: 0)
            _RVM1(0x1E13, XC3DLR_CFG, BIT7|BIT6|BIT5),
#endif

#if (PADS_TTL_MODE != Unknown_pad_mux)
#define TTL_CFG    ((PADS_TTL_MODE == TTL_MODE(1)) ? BIT0 : \
                                (PADS_TTL_MODE == TTL_MODE(2)) ? BIT1 : 0)
            _RVM1(0x1EF8, TTL_CFG, BIT1|BIT0),
#endif

#if (PADS_MHLCBUS_MODE != Unknown_pad_mux)
#define MHLCBUS_CFG  ((PADS_MHLCBUS_MODE == MHLCBUS_MODE(1)) ? BIT0 : \
                                         (PADS_MHLCBUS_MODE == MHLCBUS_MODE(2)) ? BIT1 : \
                                         (PADS_MHLCBUS_MODE == MHLCBUS_MODE(3)) ? (BIT1|BIT0) : 0)
            _RVM1(0x1EFA, MHLCBUS_CFG, BIT1|BIT0),
#endif

#if (PADS_MHLVBUS_MODE != Unknown_pad_mux)
#define MHLVBUS_CFG  ((PADS_MHLVBUS_MODE == MHLVBUS_MODE(1)) ? BIT5 : \
                                         (PADS_MHLVBUS_MODE == MHLVBUS_MODE(2)) ? BIT6 : \
                                         (PADS_MHLVBUS_MODE == MHLVBUS_MODE(3)) ? (BIT6|BIT5) : \
                                         (PADS_MHLVBUS_MODE == MHLVBUS_MODE(4)) ? BIT7 : 0)
            _RVM1(0x1EF9, MHLVBUS_CFG, BIT7|BIT6|BIT5),
#endif

#if (PADS_MHLDBG_MODE != Unknown_pad_mux)
#define MHLDBG_CFG ((PADS_MHLDBG_MODE == MHLDBG_MODE(1)) ? BIT4 : 0)
            _RVM1(0x1EF9, MHLDBG_CFG, BIT4),
#endif

#if (PADS_MHLDET_MODE != Unknown_pad_mux)
#define MHLDET_CFG ((PADS_MHLDET_MODE == MHLDET_MODE(1)) ? BIT0 : 0)
            _RVM1(0x1EF9, MHLDET_CFG, BIT0),
#endif

#if (PADS_DDCDA_MODE != Unknown_pad_mux)
#define DDCDA_CFG ((PADS_DDCDA_MODE == DDCDA_MODE(1)) ? BIT2 :\
                                   (PADS_DDCDA_MODE == DDCDA_MODE(2)) ? BIT3 : 0)
            _RVM1(0x1EFA, DDCDA_CFG, BIT3|BIT2),
            #if (PADS_DDCDA_MODE == DDCDA_MODE(2))
            _RVM1(0x1E0F, BIT7|BIT6, BIT7|BIT6),
            #endif
#endif

#if (PADS_DDCDB_MODE != Unknown_pad_mux)
#define DDCDB_CFG ((PADS_DDCDB_MODE == DDCDB_MODE(1)) ? BIT4 :\
                                   (PADS_DDCDB_MODE == DDCDB_MODE(2)) ? BIT5 : 0)
            _RVM1(0x1EFA, DDCDB_CFG, BIT5|BIT4),
            #if (PADS_DDCDB_MODE == DDCDC_MODE(2))
            _RVM1(0x1E10, BIT1|BIT0, BIT1|BIT0),
            #endif
#endif

#if (PADS_DDCDC_MODE != Unknown_pad_mux)
#define DDCDC_CFG ((PADS_DDCDC_MODE == DDCDC_MODE(1)) ? BIT6 :\
                                   (PADS_DDCDC_MODE == DDCDC_MODE(2)) ? BIT7 : 0)
            _RVM1(0x1EFA, DDCDC_CFG, BIT7|BIT6),
            #if (PADS_DDCDC_MODE == DDCDB_MODE(2))
            _RVM1(0x1E12, BIT1|BIT0, BIT1|BIT0),
            #endif
#endif

#if (PADS_I2SIN_MODE != Unknown_pad_mux)
#define I2SIN_CFG    ((PADS_I2SIN_MODE == I2SIN_MODE(1)) ? BIT3 : 0)
            _RVM1(0x1EAE, I2SIN_CFG, BIT4|BIT3),
#endif

#if (PADS_I2SOUT_MODE != Unknown_pad_mux)
#define I2SOUT_CFG   ((PADS_I2SOUT_MODE == I2SOUT_MODE(1)) ? BIT5 : \
                                     (PADS_I2SOUT_MODE == I2SOUT_MODE(2)) ? BIT6 : \
                                     (PADS_I2SOUT_MODE == I2SOUT_MODE(3)) ? (BIT6|BIT5) : 0)
            _RVM1(0x1EAE, I2SOUT_CFG, BIT6|BIT5),
#endif

#if (PADS_ET_MODE != Unknown_pad_mux)
#define ET_CFG   ((PADS_ET_MODE == ET_MODE(1)) ? BIT0 : \
                             (PADS_ET_MODE == ET_MODE(2)) ? BIT1 :0)
            _RVM1(0x1E40, ET_CFG, BIT1|BIT0),
#endif

#if (PADS_LED_MODE != Unknown_pad_mux)
#define LED_CFG   ((PADS_LED_MODE == LED_MODE(1)) ? BIT2 : \
                                (PADS_LED_MODE == LED_MODE(2)) ? BIT3 : \
                                (PADS_LED_MODE == LED_MODE(3)) ? (BIT3|BIT2) : 0)
            _RVM1(0x1EE2, LED_CFG, BIT3|BIT2),
#endif

#if (PADS_CEC_MODE != Unknown_pad_mux)
#define CEC_CFG   ((PADS_CEC_MODE == CEC_MODE(1)) ? BIT0 : \
                                (PADS_CEC_MODE == CEC_MODE(2)) ? BIT1 : 0)
            _RVM1(0x1EFB, CEC_CFG, BIT1|BIT0),
#endif

#if (PADS_SPDIF_IN != Unknown_pad_mux)
#define SPDIFIN_CFG   ((PADS_SPDIF_IN == SPDIF_IN_MODE(1)) ? BIT7 : 0)
            _RVM1(0x1EAE, SPDIFIN_CFG, BIT7),
#endif

#if (PADS_SPDIF_OUT != Unknown_pad_mux)
#define SPDIFOUT_CFG  ((PADS_SPDIF_OUT == SPDIF_OUT_MODE(1)) ? BIT0 : \
                                        (PADS_SPDIF_OUT == SPDIF_OUT_MODE(2)) ? BIT1 : 0)
            _RVM1(0x1EAF, SPDIFOUT_CFG, BIT1|BIT0),
#endif

#if (PADS_INTGPIO_MODE != Unknown_pad_mux)
#define INT_GPIO   ((PADS_INT_GPIO == INTGPIO_MODE(1)) ? BIT2: 0)
            _RVM1(0x1E4F, INT_GPIO, BIT2),
#endif

#if (PADS_PWM0_MODE != Unknown_pad_mux)
#define PWM0_CFG   ((PADS_PWM0_MODE == PWM0_MODE(1)) ? BIT4: 0)
            _RVM1(0x1E40, PWM0_CFG, BIT4),
            _RVM1(0x1E06, 0, BIT0),//pwm0 output enable
#endif

#if (PADS_PWM1_MODE != Unknown_pad_mux)
#define PWM1_CFG   ((PADS_PWM1_MODE == PWM1_MODE(1)) ? BIT5: 0)
            _RVM1(0x1E40, PWM1_CFG, BIT5),
            _RVM1(0x1E06, 0, BIT1),//pwm1 output enable
#endif

#if (PADS_PWM2_MODE != Unknown_pad_mux)
#define PWM2_CFG   ((PADS_PWM2_MODE == PWM2_MODE(1)) ? BIT6: 0)
            _RVM1(0x1E40, PWM2_CFG, BIT6),
            _RVM1(0x1E06, 0, BIT2),//pwm2 output enable
#endif

#if (PADS_PWM3_MODE != Unknown_pad_mux)
#define PWM3_CFG   ((PADS_PWM3_MODE == PWM3_MODE(1)) ? BIT7: 0)
            _RVM1(0x1E40, PWM3_CFG, BIT7),
            _RVM1(0x1E06, 0, BIT3),//pwm3 output enable
#endif

#if (PADS_PWM4_MODE != Unknown_pad_mux)
#define PWM4_CFG   ((PADS_PWM4_MODE == PWM4_MODE(1)) ? BIT0: 0)
            _RVM1(0x1E41, PWM4_CFG, BIT1|BIT0),
            _RVM1(0x1E06, 0, BIT4),//pwm4 output enable
#endif

#if (PADS_PWM5_MODE != Unknown_pad_mux)
#define PWM5_CFG   ((PADS_PWM5_MODE == PWM5_MODE(1)) ? BIT2: 0)
            _RVM1(0x1E41, PWM5_CFG, BIT3|BIT2),
            _RVM1(0x1E06, 0, BIT5),//pwm5 output enable
#endif

#if (PADS_DDCR_MODE != Unknown_pad_mux)
#define DDCR_CFG   ((PADS_DDCR_MODE == MIIC_MODE(1)) ? BIT0: \
                         (PADS_DDCR_MODE == MIIC_MODE(2)) ? (BIT1|BIT0): \
                         (PADS_DDCR_MODE == MIIC_MODE(3)) ? (BIT2|BIT0): \
                         (PADS_DDCR_MODE == MIIC_MODE(4)) ? (BIT2|BIT1|BIT0): \
                         (PADS_DDCR_MODE == DDCR_MODE(1)) ? BIT1: \
                         (PADS_DDCR_MODE == DDCR_MODE(2)) ? BIT2: \
                         (PADS_DDCR_MODE == DDCR_MODE(3)) ? (BIT2|BIT1): \
                         (PADS_DDCR_MODE == DDCR_MODE(4)) ? BIT3: 0)
            _RVM1(0x1EF6, DDCR_CFG, BIT3|BIT2|BIT1|BIT0),
#endif

#if (PADS_PCMPE_MODE != Unknown_pad_mux)
#define PCMPE_CFG0 ((PADS_PCMPE_MODE == PCMPE_MODE(1)) ? 0xFFFF : 0)
#define PCMPE_CFG1 ((PADS_PCMPE_MODE == PCMPE_MODE(1)) ? 0xFFFF : 0)
#define PCMPE_CFG2 ((PADS_PCMPE_MODE == PCMPE_MODE(1)) ? 0x03 : 0)
            _RVM2(0x1E0C, PCMPE_CFG0, 0xFFFF),
            _RVM2(0x1E0E, PCMPE_CFG1, 0xFFFF),
            _RVM1(0x1E10, PCMPE_CFG2, 0x03),
#endif

#if (PADS_PCMCTRL_MODE != Unknown_pad_mux)
#define PCMCTRL_CFG ((PADS_PCMCTRL_MODE == PCMCTRL_MODE(1)) ? BIT5 : 0)
            _RVM1(0x1EC8, PCMCTRL_CFG, BIT5),
#endif

#if (PADS_PCMAD_MODE != Unknown_pad_mux)
#define PCMAD_CFG ((PADS_PCMAD_MODE == PCMAD_MODE(1)) ? BIT4 : 0)
            _RVM1(0x1EC8, PCMAD_CFG, BIT4),
#endif

#if (PADS_TS0_MODE != Unknown_pad_mux)
#define TS0_CFG ((PADS_TS0_MODE == TS0_MODE(1)) ? BIT2 : \
                             (PADS_TS0_MODE == TS0_MODE(2)) ? BIT3 : 0)
            _RVM1(0x1EAF, TS0_CFG, BIT4|BIT3|BIT2),
#endif

#if (PADS_TS1_MODE != Unknown_pad_mux)
#define TS1_CFG ((PADS_TS1_MODE == TS1_MODE(1)) ? BIT5 : \
                             (PADS_TS1_MODE == TS1_MODE(2)) ? BIT6 : \
                             (PADS_TS1_MODE == TS1_MODE(3)) ? (BIT6|BIT5) : \
                             (PADS_TS1_MODE == TS1_MODE(4)) ? BIT7 : 0)
            _RVM1(0x1EAF, TS1_CFG, BIT7|BIT6|BIT5),
#endif

#if (PADS_TSD_MODE != Unknown_pad_mux)
#define TSD_CFG ((PADS_TSD_MODE == TSD_MODE(1)) ? BIT4 :\
                              (PADS_TSD_MODE == TSD_MODE(2)) ? BIT5 :0)
            _RVM1(0x1EAA, TSD_CFG, BIT5|BIT4),
#endif

#if (PADS_TS2_MODE != Unknown_pad_mux)
#define TS2_CFG ((PADS_TS2_MODE == TS2_MODE(1)) ? BIT0 : \
                             (PADS_TS2_MODE == TS2_MODE(2)) ? BIT1 : \
                             (PADS_TS2_MODE == TS2_MODE(3)) ? (BIT1|BIT0) : 0)
            _RVM1(0x1EAE, TS2_CFG, BIT2|BIT1|BIT0),
#endif

#if (PADS_FLASH_MODE != Unknown_pad_mux)
#define FLASH_CFG ((PADS_FLASH_MODE == FLASH_MODE(1)) ? BIT3 : \
                                  (PADS_FLASH_MODE == FLASH_MODE(2)) ? BIT4 : 0)
#define NAND_CFG ((PADS_FLASH_MODE == FLASH_MODE(3)) ? BIT0 : 0)
#define NAND_SHARE_CFG ((PADS_FLASH_MODE == FLASH_MODE(4)) ? BIT7 : 0)
        _RVM1(0x1E08, FLASH_CFG, BIT4|BIT3 ),  //disable SPINOR or SPINAND
        _RVM1(0x1E4E, NAND_CFG, BIT1|BIT0 ),  //enable nand mode
        _RVM1(0x1E3B, NAND_SHARE_CFG, BIT7 ),  //enable nand share mode
#endif

#if (PADS_DISEQCOUT_MODE != Unknown_pad_mux)
#define DISEQCOUT_CFG ((PADS_DISEQCOUT_MODE == DISEQCOUT_MODE(1)) ? BIT3 : \
                                          (PADS_DISEQCOUT_MODE == DISEQCOUT_MODE(2)) ? BIT4 : 0)
            _RVM1(0x1E07, DISEQCOUT_CFG, BIT4|BIT3),
#endif

#if (PADS_DISEQCIN_MODE != Unknown_pad_mux)
#define DISEQCIN_CFG ((PADS_DISEQCIN_MODE == DISEQCIN_MODE(1)) ? BIT5 : \
                                       (PADS_DISEQCIN_MODE == DISEQCIN_MODE(2)) ? BIT6 : 0)
            _RVM1(0x1E07, DISEQCIN_CFG, BIT6|BIT5),
#endif

#if (PADS_ARC_MODE != Unknown_pad_mux)
#define ARC_CFG ((PADS_ARC_MODE == ARC_MODE(1)) ? BIT5 : 0)
            _RVM1(0x1E19, ARC_CFG, BIT5),
#endif

#if (PADS_CEC_MODE != Unknown_pad_mux)
        _RVM1(0x1E08, 0, BIT1|BIT0 ),   //reg[1E08]#1 ~ #0 = 00b
        _MEMMAP_PM_,
        _RVM1(0x3401, BIT6, BIT6 ),   //reg[3401]#6 = 1b
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
  #if ((MS_BOARD_TYPE_SEL == BD_MST214A_D01A_S_MILAN))//PAD_PCM_A6
        _RVM1(0x1e6c, BIT6, BIT6),      //output high
        _RVM1(0x1e6e, 0, BIT6),           //oen: output(0)
        _RVM1(0x1e0c, 0xff, 0xff ),   //reg[101e0c]#7 ~#0 = ffh (reg_pcm_pe[33:0] = 3ffffffffh)
        _RVM1(0x1e0d, 0xff, 0xff ),   //reg[101e0d]#7 ~#0 = ffh
        _RVM1(0x1e0e, 0xff, 0xff ),   //reg[101e0e]#7 ~#0 = ffh
        _RVM1(0x1e0f, 0xff, 0xff ),   //reg[101e0f]#7 ~#0 = ffh
        _RVM1(0x1e10, 0x03 ,0x03 ),   //reg[101e10]#1 ~#0 = 3h
        _RVM1(0x1ec8, 0, BIT4 ),          //reg[101ec8]#4 = 00b (reg_pcmadconfig[4] = 0b)
  #elif ((MS_BOARD_TYPE_SEL == BD_MST214B_D01A_S_MILAN))//PAD_TCON0
        _RVM1(0x1e5e, BIT0, BIT0),      //output high
        _RVM1(0x1e60, 0, BIT0),           //oen: output(0)
        _RVM1(0x1ef2, 0, BIT0 ),          //reg[101ef2]#0 = 0b (reg_tconconfig[0] = 0b)
        _RVM1(0x1e10, 0, 0x07 ),         //reg[101e10]#2~#0 = 000b (reg_3d_lr_config[2:0] = 00b)
        _RVM1(0x1ef8, 0, BIT1 ),          //reg[101ef8]#1 = 0b (reg_8bit_ttl = 0b)
        _RVM1(0x1ef9, 0, 0xE0),          //reg[101ef9]#7~#5 = 0b (reg_mhl_vbus_config[2:0] = 000b)
        _RVM1(0x1eae, 0, 0x60 ),         //reg[101eae]#6~#5 = 00b (reg_i2soutconfig[1:0] = 00b)
        _RVM1(0x1ee2, 0, 0x0C ),         //reg[101ee2]#3~#2 = 00b (reg_led_mode[3:2] = 00b)
        _RVM1(0x1ef6, 0, 0x0F ),         //reg[101ef6]#3~#0 = 0000b (reg_ddcrmode[3:0] = 0000b)
        _RVM1(0x1e26, 0, BIT7 ),          //reg[101e26]#7 = 0b (reg_agc_dbg[7] = 0b)
  #elif ((MS_BOARD_TYPE_SEL == BD_MST214C_D01A_S_MILAN))//PAD_NAND_WPZ
        _RVM1(0x1e43, BIT5, BIT4),      //output high
        _RVM1(0x1e47, 0, BIT4),           //oen: output(0)
        _RVM1(0x1e08, 0, BIT4 ),          //reg[101e08]#4 = 0b (reg_spi_nand_mode[4] = 0b)
        _RVM1(0x1e4E, 0, BIT1|BIT0 ), //reg[101e4E]#1 ~ #0 = 0b (reg_nand_mode[1:0] = 0b)
  #elif (MS_BOARD_TYPE_SEL == BD_MST214D_D01A_S_MILAN)//PADA_VSYNC2
        _RVM1(0x255b, BIT3, BIT3),      //output high
        _RVM1(0x255a, 0, BIT5),           //oen: output(0)
        _RVM1(0x2561, 0, BIT2 ),          //reg[102561]#2 = 0b (reg_test_vsync[2] = 0b)
        _RVM1(0x2558, BIT5 , BIT5 ),    //reg[102558]#5 = 1b (reg_gpio_vsync_en[5] = 1b)
#elif ((MS_BOARD_TYPE_SEL == BD_MST073B_10AGS))//PAD_NAND_WPZ
        _RVM1(0x1e43, BIT4, BIT4),		//output high
        _RVM1(0x1e47, 0, BIT4), 		  //oen: output(0)
        _RVM1(0x1e08, 0, BIT4 ),		  //reg[101e08]#4 = 0b (reg_spi_nand_mode[4] = 0b)
			_RVM1(0x1e4E, 0, BIT1|BIT0 ), //reg[101e4E]#1 ~ #0 = 0b (reg_nand_mode[1:0] = 0b)
#elif ((MS_BOARD_TYPE_SEL == BD_MST074B_10AGS))//PAD_PCM_A6
        _RVM1(0x1e6c, BIT6, BIT6),	  //output high
        _RVM1(0x1e6e, 0, BIT6),			//oen: output(0)
        _RVM1(0x1e0c, 0xff, 0xff ),	//reg[101e0c]#7 ~#0 = ffh (reg_pcm_pe[33:0] = 3ffffffffh)
        _RVM1(0x1e0d, 0xff, 0xff ),	//reg[101e0d]#7 ~#0 = ffh
        _RVM1(0x1e0e, 0xff, 0xff ),	//reg[101e0e]#7 ~#0 = ffh
        _RVM1(0x1e0f, 0xff, 0xff ),	//reg[101e0f]#7 ~#0 = ffh
        _RVM1(0x1e10, 0x03 ,0x03 ),	//reg[101e10]#1 ~#0 = 3h
        _RVM1(0x1ec8, 0, BIT4 ),			//reg[101ec8]#4 = 00b (reg_pcmadconfig[4] = 0b)
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

    u32MapBase = _MapBase_nonPM;

    pPadConfig = (U8 *)padInitTbl + 10;

    while (1)
    {
        u16Index = (pPadConfig[0] << 8) + (pPadConfig[1]);

        if (u16Index == 0xFFFF) // check end of table
            break;

        pPadConfig += 2;

        if(u16Index == 0x0000) // switch base address
        {
            if(pPadConfig[0]==0x10)
            {
                u32MapBase = _MapBase_nonPM;
            }
            else if(pPadConfig[0]==0x11)
            {
                u32MapBase = _MapBase_nonPM_11;
            }
            else if(pPadConfig[0]==0x00)
            {
                u32MapBase = _MapBase_PM;
            }
            else if(pPadConfig[0]==0x12)
            {
                u32MapBase = _MapBase_nonPM_12;
            }
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

#if (UART0_SRC_SEL == UART1_SRC_SEL) || \
    (UART0_SRC_SEL == UART2_SRC_SEL) || \
    (UART1_SRC_SEL == UART2_SRC_SEL)
    #error "UART src selection conflict"
#endif


