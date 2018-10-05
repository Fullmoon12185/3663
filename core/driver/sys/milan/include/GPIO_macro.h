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
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_GPIO_H_
#define _DRV_GPIO_H_

#include "chip/bond.h"  // pad index, and gpio mapping
#include "drvGlobal.h"  // register access functions
#include "drvGPIO.h"
#include "drvPM.h"

//------------------------------GPIO_PIN----------------------------------------
#define GPIO_NONE               0       // Not GPIO pin (default)
#define GPIO_IN                 1       // GPI
#define GPIO_OUT_LOW            2       // GPO output low
#define GPIO_OUT_HIGH           3       // GPO output high

//----------------------------------------------------------------------------
// Private Macros
//----------------------------------------------------------------------------

#define __GPIO_REG(g)       ___GPIO_REG(g)
#define __GPIO_BIT(g)       ___GPIO_BIT(g)

// don't use these outside this header
#define ___GPIO_REG(r, b)   r
#define ___GPIO_BIT(r, b)   b

//----------------------------------------------------------------------------
// Internal GPIO
//----------------------------------------------------------------------------
#define GPIO0_PAD PAD_TCON0
#define GPIO0_OEN 0x101e60, BIT0
#define GPIO0_OUT 0x101e5e, BIT0
#define GPIO0_IN  0x101e5c, BIT0

#define GPIO1_PAD PAD_TCON1
#define GPIO1_OEN 0x101e60, BIT1
#define GPIO1_OUT 0x101e5e, BIT1
#define GPIO1_IN  0x101e5c, BIT1

#define GPIO2_PAD PAD_TCON2
#define GPIO2_OEN 0x101e60, BIT2
#define GPIO2_OUT 0x101e5e, BIT2
#define GPIO2_IN  0x101e5c, BIT2

#define GPIO3_PAD PAD_TCON3
#define GPIO3_OEN 0x101e60, BIT3
#define GPIO3_OUT 0x101e5e, BIT3
#define GPIO3_IN  0x101e5c, BIT3

#define GPIO4_PAD PAD_TCON4
#define GPIO4_OEN 0x101e60, BIT4
#define GPIO4_OUT 0x101e5e, BIT4
#define GPIO4_IN  0x101e5c, BIT4

#define GPIO5_PAD PAD_TCON5
#define GPIO5_OEN 0x101e60, BIT5
#define GPIO5_OUT 0x101e5e, BIT5
#define GPIO5_IN  0x101e5c, BIT5

#define GPIO6_PAD PAD_TCON6
#define GPIO6_OEN 0x101e60, BIT6
#define GPIO6_OUT 0x101e5e, BIT6
#define GPIO6_IN  0x101e5c, BIT6

#define GPIO7_PAD PAD_TCON7
#define GPIO7_OEN 0x101e60, BIT7
#define GPIO7_OUT 0x101e5e, BIT7
#define GPIO7_IN  0x101e5c, BIT7

//##################
//
// Skip GPIO8~ GPIO13
//
//##################

#define GPIO14_PAD PAD_PWM1
#define GPIO14_OEN 0x101e87, BIT5
#define GPIO14_OUT 0x101e85, BIT5
#define GPIO14_IN  0x101e83, BIT5

#define GPIO15_PAD PAD_PWM0
#define GPIO15_OEN 0x101e87, BIT4
#define GPIO15_OUT 0x101e85, BIT4
#define GPIO15_IN  0x101e83, BIT4

#define GPIO16_PAD PAD_LVSYNC
#define GPIO16_OEN 0x1032a2, BIT0
#define GPIO16_OUT 0x10329e, BIT0
#define GPIO16_IN  0x1032aa, BIT0

#define GPIO17_PAD PAD_LHSYNC
#define GPIO17_OEN 0x1032a2, BIT1
#define GPIO17_OUT 0x10329e, BIT1
#define GPIO17_IN  0x1032aa, BIT1

#define GPIO18_PAD PAD_LDE
#define GPIO18_OEN 0x1032a2, BIT2
#define GPIO18_OUT 0x10329e, BIT2
#define GPIO18_IN  0x1032aa, BIT2

#define GPIO19_PAD PAD_LCK
#define GPIO19_OEN 0x1032a2, BIT3
#define GPIO19_OUT 0x10329e, BIT3
#define GPIO19_IN  0x1032aa, BIT3

#define GPIO20_PAD PAD_R_ODD7
#define GPIO20_OEN 0x1032a2, BIT4
#define GPIO20_OUT 0x10329e, BIT4
#define GPIO20_IN  0x1032aa, BIT4

#define GPIO21_PAD PAD_R_ODD6
#define GPIO21_OEN 0x1032a2, BIT5
#define GPIO21_OUT 0x10329e, BIT5
#define GPIO21_IN  0x1032aa, BIT5

#define GPIO22_PAD PAD_R_ODD5
#define GPIO22_OEN 0x1032a2, BIT6
#define GPIO22_OUT 0x10329e, BIT6
#define GPIO22_IN  0x1032aa, BIT6

#define GPIO23_PAD PAD_R_ODD4
#define GPIO23_OEN 0x1032a2, BIT7
#define GPIO23_OUT 0x10329e, BIT7
#define GPIO23_IN  0x1032aa, BIT7

#define GPIO24_PAD PAD_R_ODD3
#define GPIO24_OEN 0x1032a3, BIT0
#define GPIO24_OUT 0x10329f, BIT0
#define GPIO24_IN  0x1032ab, BIT0

#define GPIO25_PAD PAD_R_ODD2
#define GPIO25_OEN 0x1032a3, BIT1
#define GPIO25_OUT 0x10329f, BIT1
#define GPIO25_IN  0x1032ab, BIT1

#define GPIO26_PAD PAD_R_ODD1
#define GPIO26_OEN 0x1032a3, BIT2
#define GPIO26_OUT 0x10329f, BIT2
#define GPIO26_IN  0x1032ab, BIT2

#define GPIO27_PAD PAD_R_ODD0
#define GPIO27_OEN 0x1032a3, BIT3
#define GPIO27_OUT 0x10329f, BIT3
#define GPIO27_IN  0x1032ab, BIT3

#define GPIO28_PAD PAD_G_ODD7
#define GPIO28_OEN 0x1032a3, BIT4
#define GPIO28_OUT 0x10329f, BIT4
#define GPIO28_IN  0x1032ab, BIT4

#define GPIO29_PAD PAD_G_ODD6
#define GPIO29_OEN 0x1032a3, BIT5
#define GPIO29_OUT 0x10329f, BIT5
#define GPIO29_IN  0x1032ab, BIT5

#define GPIO30_PAD PAD_G_ODD5
#define GPIO30_OEN 0x1032a3, BIT6
#define GPIO30_OUT 0x10329f, BIT6
#define GPIO30_IN  0x1032ab, BIT6

#define GPIO31_PAD PAD_G_ODD4
#define GPIO31_OEN 0x1032a3, BIT7
#define GPIO31_OUT 0x10329f, BIT7
#define GPIO31_IN  0x1032ab, BIT7

#define GPIO32_PAD PAD_G_ODD3
#define GPIO32_OEN 0x1032a4, BIT0
#define GPIO32_OUT 0x1032a0, BIT0
#define GPIO32_IN  0x1032ac, BIT0

#define GPIO33_PAD PAD_G_ODD2
#define GPIO33_OEN 0x1032a4, BIT1
#define GPIO33_OUT 0x1032a0, BIT1
#define GPIO33_IN  0x1032ac, BIT1

#define GPIO34_PAD PAD_G_ODD1
#define GPIO34_OEN 0x1032a4, BIT2
#define GPIO34_OUT 0x1032a0, BIT2
#define GPIO34_IN  0x1032ac, BIT2

#define GPIO35_PAD PAD_G_ODD0
#define GPIO35_OEN 0x1032a4, BIT3
#define GPIO35_OUT 0x1032a0, BIT3
#define GPIO35_IN  0x1032ac, BIT3

#define GPIO36_PAD PAD_B_ODD7
#define GPIO36_OEN 0x1032a4, BIT4
#define GPIO36_OUT 0x1032a0, BIT4
#define GPIO36_IN  0x1032ac, BIT4

#define GPIO37_PAD PAD_B_ODD6
#define GPIO37_OEN 0x1032a4, BIT5
#define GPIO37_OUT 0x1032a0, BIT5
#define GPIO37_IN  0x1032ac, BIT5

#define GPIO38_PAD PAD_B_ODD5
#define GPIO38_OEN 0x1032a4, BIT6
#define GPIO38_OUT 0x1032a0, BIT6
#define GPIO38_IN  0x1032ac, BIT6

#define GPIO39_PAD PAD_B_ODD4
#define GPIO39_OEN 0x1032a4, BIT7
#define GPIO39_OUT 0x1032a0, BIT7
#define GPIO39_IN  0x1032ac, BIT7

#define GPIO40_PAD PAD_B_ODD3
#define GPIO40_OEN 0x1032a5, BIT0
#define GPIO40_OUT 0x1032a1, BIT0
#define GPIO40_IN  0x1032ad, BIT0

#define GPIO41_PAD PAD_B_ODD2
#define GPIO41_OEN 0x1032a5, BIT1
#define GPIO41_OUT 0x1032a1, BIT1
#define GPIO41_IN  0x1032ad, BIT1

#define GPIO42_PAD PAD_B_ODD1
#define GPIO42_OEN 0x1032a5, BIT2
#define GPIO42_OUT 0x1032a1, BIT2
#define GPIO42_IN  0x1032ad, BIT2

#define GPIO43_PAD PAD_B_ODD0
#define GPIO43_OEN 0x1032a5, BIT3
#define GPIO43_OUT 0x1032a1, BIT3
#define GPIO43_IN  0x1032ad, BIT3

#define GPIO44_PAD PAD_GPIO0
#define GPIO44_OEN 0x101e7e, BIT0
#define GPIO44_OUT 0x101e7a, BIT0
#define GPIO44_IN  0x101e76, BIT0

#define GPIO45_PAD PAD_GPIO1
#define GPIO45_OEN 0x101e7e, BIT1
#define GPIO45_OUT 0x101e7a, BIT1
#define GPIO45_IN  0x101e76, BIT1

#define GPIO46_PAD PAD_GPIO2
#define GPIO46_OEN 0x101e7e, BIT2
#define GPIO46_OUT 0x101e7a, BIT2
#define GPIO46_IN  0x101e76, BIT2

#define GPIO47_PAD PAD_GPIO3
#define GPIO47_OEN 0x101e7e, BIT3
#define GPIO47_OUT 0x101e7a, BIT3
#define GPIO47_IN  0x101e76, BIT3

#define GPIO48_PAD PAD_GPIO4
#define GPIO48_OEN 0x101e7e, BIT4
#define GPIO48_OUT 0x101e7a, BIT4
#define GPIO48_IN  0x101e76, BIT4

#define GPIO49_PAD PAD_GPIO5
#define GPIO49_OEN 0x101e7e, BIT5
#define GPIO49_OUT 0x101e7a, BIT5
#define GPIO49_IN  0x101e76, BIT5

#define GPIO50_PAD PAD_GPIO6
#define GPIO50_OEN 0x101e7e, BIT6
#define GPIO50_OUT 0x101e7a, BIT6
#define GPIO50_IN  0x101e76, BIT6

#define GPIO51_PAD PAD_GPIO7
#define GPIO51_OEN 0x101e7e, BIT7
#define GPIO51_OUT 0x101e7a, BIT7
#define GPIO51_IN  0x101e76, BIT7

#define GPIO52_PAD PAD_GPIO8
#define GPIO52_OEN 0x101e7f, BIT0
#define GPIO52_OUT 0x101e7b, BIT0
#define GPIO52_IN  0x101e77, BIT0

#define GPIO53_PAD PAD_GPIO9
#define GPIO53_OEN 0x101e7f, BIT1
#define GPIO53_OUT 0x101e7b, BIT1
#define GPIO53_IN  0x101e77, BIT1

#define GPIO54_PAD PAD_GPIO10
#define GPIO54_OEN 0x101e7f, BIT2
#define GPIO54_OUT 0x101e7b, BIT2
#define GPIO54_IN  0x101e77, BIT2

#define GPIO55_PAD PAD_GPIO11
#define GPIO55_OEN 0x101e7f, BIT3
#define GPIO55_OUT 0x101e7b, BIT3
#define GPIO55_IN  0x101e77, BIT3

#define GPIO56_PAD PAD_GPIO12
#define GPIO56_OEN 0x101e7f, BIT4
#define GPIO56_OUT 0x101e7b, BIT4
#define GPIO56_IN  0x101e77, BIT4

#define GPIO57_PAD PAD_GPIO13
#define GPIO57_OEN 0x101e7f, BIT5
#define GPIO57_OUT 0x101e7b, BIT5
#define GPIO57_IN  0x101e77, BIT5

#define GPIO58_PAD PAD_GPIO14
#define GPIO58_OEN 0x101e7f, BIT6
#define GPIO58_OUT 0x101e7b, BIT6
#define GPIO58_IN  0x101e77, BIT6

#define GPIO59_PAD PAD_GPIO15
#define GPIO59_OEN 0x101e7f, BIT7
#define GPIO59_OUT 0x101e7b, BIT7
#define GPIO59_IN  0x101e77, BIT7

#define GPIO60_PAD PAD_GPIO16
#define GPIO60_OEN 0x101e80, BIT0
#define GPIO60_OUT 0x101e7c, BIT0
#define GPIO60_IN  0x101e78, BIT0

#define GPIO61_PAD PAD_GPIO17
#define GPIO61_OEN 0x101e80, BIT1
#define GPIO61_OUT 0x101e7c, BIT1
#define GPIO61_IN  0x101e78, BIT1

#define GPIO62_PAD PAD_GPIO18
#define GPIO62_OEN 0x101e80, BIT2
#define GPIO62_OUT 0x101e7c, BIT2
#define GPIO62_IN  0x101e78, BIT2

#define GPIO63_PAD PAD_GPIO19
#define GPIO63_OEN 0x101e80, BIT3
#define GPIO63_OUT 0x101e7c, BIT3
#define GPIO63_IN  0x101e78, BIT3

#define GPIO64_PAD PAD_GPIO20
#define GPIO64_OEN 0x101e80, BIT4
#define GPIO64_OUT 0x101e7c, BIT4
#define GPIO64_IN  0x101e78, BIT4

#define GPIO65_PAD PAD_GPIO21
#define GPIO65_OEN 0x101e80, BIT5
#define GPIO65_OUT 0x101e7c, BIT5
#define GPIO65_IN  0x101e78, BIT5

#define GPIO66_PAD PAD_GPIO22
#define GPIO66_OEN 0x101e80, BIT6
#define GPIO66_OUT 0x101e7c, BIT6
#define GPIO66_IN  0x101e78, BIT6

#define GPIO67_PAD PAD_GPIO23
#define GPIO67_OEN 0x101e80, BIT7
#define GPIO67_OUT 0x101e7c, BIT7
#define GPIO67_IN  0x101e78, BIT7

#define GPIO68_PAD PAD_GPIO24
#define GPIO68_OEN 0x101e81, BIT0
#define GPIO68_OUT 0x101e7d, BIT0
#define GPIO68_IN  0x101e79, BIT0

#define GPIO69_PAD PAD_GPIO25
#define GPIO69_OEN 0x101e81, BIT1
#define GPIO69_OUT 0x101e7d, BIT1
#define GPIO69_IN  0x101e79, BIT1

#define GPIO70_PAD PAD_GPIO26
#define GPIO70_OEN 0x101e81, BIT2
#define GPIO70_OUT 0x101e7d, BIT2
#define GPIO70_IN  0x101e79, BIT2

#define GPIO71_PAD PAD_GPIO27
#define GPIO71_OEN 0x101e81, BIT3
#define GPIO71_OUT 0x101e7d, BIT3
#define GPIO71_IN  0x101e79, BIT3

#define GPIO72_PAD PAD_GPIO28
#define GPIO72_OEN 0x101e81, BIT4
#define GPIO72_OUT 0x101e7d, BIT4
#define GPIO72_IN  0x101e79, BIT4

#define GPIO73_PAD PAD_GPIO29
#define GPIO73_OEN 0x101e81, BIT5
#define GPIO73_OUT 0x101e7d, BIT5
#define GPIO73_IN  0x101e79, BIT5

#define GPIO74_PAD PAD_GPIO30
#define GPIO74_OEN 0x101ee9, BIT0
#define GPIO74_OUT 0x101ee8, BIT0
#define GPIO74_IN  0x101ee6, BIT0

#define GPIO75_PAD PAD_GPIO31
#define GPIO75_OEN 0x101ee9, BIT1
#define GPIO75_OUT 0x101ee8, BIT1
#define GPIO75_IN  0x101ee6, BIT1

#define GPIO76_PAD PAD_GPIO32
#define GPIO76_OEN 0x101ee9, BIT2
#define GPIO76_OUT 0x101ee8, BIT2
#define GPIO76_IN  0x101ee6, BIT2

#define GPIO77_PAD PAD_GPIO33
#define GPIO77_OEN 0x101ee9, BIT3
#define GPIO77_OUT 0x101ee8, BIT3
#define GPIO77_IN  0x101ee6, BIT3

#define GPIO78_PAD PAD_GPIO34
#define GPIO78_OEN 0x101ee9, BIT4
#define GPIO78_OUT 0x101ee8, BIT4
#define GPIO78_IN  0x101ee6, BIT4

#define GPIO79_PAD PAD_GPIO35
#define GPIO79_OEN 0x101ee9, BIT5
#define GPIO79_OUT 0x101ee8, BIT5
#define GPIO79_IN  0x101ee6, BIT5

#define GPIO80_PAD PAD_INT
#define GPIO80_OEN 0x101e49, BIT2
#define GPIO80_OUT 0x101e45, BIT2
#define GPIO80_IN  0x101e4d, BIT2

#define GPIO81_PAD PAD_CEC
#define GPIO81_OEN 0x101e08, BIT1
#define GPIO81_OUT 0x101e08, BIT0
#define GPIO81_IN  0x101e08, BIT2

#define GPIO82_PAD PAD_SAR2
#define GPIO82_OEN 0x1423, BIT2
#define GPIO82_OUT 0x1424, BIT2
#define GPIO82_IN  0x1425, BIT2

#define GPIO83_PAD PAD_SAR1
#define GPIO83_OEN 0x1423, BIT1
#define GPIO83_OUT 0x1424, BIT1
#define GPIO83_IN  0x1425, BIT1

#define GPIO84_PAD PAD_SAR0
#define GPIO84_OEN 0x1423, BIT0
#define GPIO84_OUT 0x1424, BIT0
#define GPIO84_IN  0x1425, BIT0

#define GPIO85_PAD PAD_DDCDA_CLK
#define GPIO85_OEN 0x101e18, BIT0
#define GPIO85_OUT 0x101e16, BIT0
#define GPIO85_IN  0x101e14, BIT0

#define GPIO86_PAD PAD_DDCDA_DAT
#define GPIO86_OEN 0x101e18, BIT1
#define GPIO86_OUT 0x101e16, BIT1
#define GPIO86_IN  0x101e14, BIT1

#define GPIO87_PAD PAD_HOTPLUG_A
#define GPIO87_OEN 0x101e19, BIT0
#define GPIO87_OUT 0x101e17, BIT0
#define GPIO87_IN  0x101e15, BIT0

#define GPIO88_PAD PAD_DDCDB_CLK
#define GPIO88_OEN 0x101e18, BIT2
#define GPIO88_OUT 0x101e16, BIT2
#define GPIO88_IN  0x101e14, BIT2

#define GPIO89_PAD PAD_DDCDB_DAT
#define GPIO89_OEN 0x101e18, BIT3
#define GPIO89_OUT 0x101e16, BIT3
#define GPIO89_IN  0x101e14, BIT3

#define GPIO90_PAD PAD_HOTPLUG_B
#define GPIO90_OEN 0x101e19, BIT1
#define GPIO90_OUT 0x101e17, BIT1
#define GPIO90_IN  0x101e15, BIT1

#define GPIO91_PAD PAD_DDCDC_CLK
#define GPIO91_OEN 0x101e18, BIT4
#define GPIO91_OUT 0x101e16, BIT4
#define GPIO91_IN  0x101e14, BIT4

#define GPIO92_PAD PAD_DDCDC_DAT
#define GPIO92_OEN 0x101e18, BIT5
#define GPIO92_OUT 0x101e16, BIT5
#define GPIO92_IN  0x101e14, BIT5

#define GPIO93_PAD PAD_HOTPLUG_C
#define GPIO93_OEN 0x101e19, BIT2
#define GPIO93_OUT 0x101e17, BIT2
#define GPIO93_IN  0x101e15, BIT2

#define GPIO94_PAD PAD_MHL_DET
#define GPIO94_OEN 0x101e19, BIT3
#define GPIO94_OUT 0x101e17, BIT3
#define GPIO94_IN  0x101e15, BIT3

#define GPIO95_PAD PAD_PM_SPI_CZ1
#define GPIO95_OEN 0x101e48, BIT0
#define GPIO95_OUT 0x101e8e, BIT0
#define GPIO95_IN  0x101e9C, BIT0

#define GPIO96_PAD PAD_PM_SPI_CZ2
#define GPIO96_OEN 0x101e48, BIT1
#define GPIO96_OUT 0x101e8e, BIT1
#define GPIO96_IN  0x101e9C, BIT1

#define GPIO97_PAD PAD_NAND_REZ
#define GPIO97_OEN 0x101e47, BIT0
#define GPIO97_OUT 0x101e43, BIT0
#define GPIO97_IN  0x101e4b, BIT0

#define GPIO98_PAD PAD_NAND_ALE
#define GPIO98_OEN 0x101e47, BIT2
#define GPIO98_OUT 0x101e43, BIT2
#define GPIO98_IN  0x101e4b, BIT2

#define GPIO99_PAD PAD_NAND_CLE
#define GPIO99_OEN 0x101e47, BIT1
#define GPIO99_OUT 0x101e43, BIT1
#define GPIO99_IN  0x101e4b, BIT1

#define GPIO100_PAD PAD_NAND_CEZ
#define GPIO100_OEN 0x101e47, BIT3
#define GPIO100_OUT 0x101e43, BIT3
#define GPIO100_IN  0x101e4b, BIT3

#define GPIO101_PAD PAD_NAND_WPZ
#define GPIO101_OEN 0x101e47, BIT5
#define GPIO101_OUT 0x101e43, BIT5
#define GPIO101_IN  0x101e4b, BIT5

#define GPIO102_PAD PAD_NAND_WEZ
#define GPIO102_OEN 0x101e47, BIT6
#define GPIO102_OUT 0x101e43, BIT6
#define GPIO102_IN  0x101e4b, BIT6

#define GPIO103_PAD PAD_NAND_RBZ
#define GPIO103_OEN 0x101e47, BIT7
#define GPIO103_OUT 0x101e43, BIT7
#define GPIO103_IN  0x101e4b, BIT7

#define GPIO104_PAD PAD_NAND_AD0
#define GPIO104_OEN 0x101e46, BIT7
#define GPIO104_OUT 0x101e42, BIT7
#define GPIO104_IN  0x101e4a, BIT7

#define GPIO105_PAD PAD_NAND_AD1
#define GPIO105_OEN 0x101e46, BIT6
#define GPIO105_OUT 0x101e42, BIT6
#define GPIO105_IN  0x101e4a, BIT6

#define GPIO106_PAD PAD_NAND_AD2
#define GPIO106_OEN 0x101e46, BIT5
#define GPIO106_OUT 0x101e42, BIT5
#define GPIO106_IN  0x101e4a, BIT5

#define GPIO107_PAD PAD_NAND_AD3
#define GPIO107_OEN 0x101e46, BIT4
#define GPIO107_OUT 0x101e42, BIT4
#define GPIO107_IN  0x101e4a, BIT4

#define GPIO108_PAD PAD_NAND_AD4
#define GPIO108_OEN 0x101e46, BIT3
#define GPIO108_OUT 0x101e42, BIT3
#define GPIO108_IN  0x101e4a, BIT3

#define GPIO109_PAD PAD_NAND_AD5
#define GPIO109_OEN 0x101e46, BIT2
#define GPIO109_OUT 0x101e42, BIT2
#define GPIO109_IN  0x101e4a, BIT2

#define GPIO110_PAD PAD_NAND_AD6
#define GPIO110_OEN 0x101e46, BIT1
#define GPIO110_OUT 0x101e42, BIT1
#define GPIO110_IN  0x101e4a, BIT1

#define GPIO111_PAD PAD_NAND_AD7
#define GPIO111_OEN 0x101e46, BIT0
#define GPIO111_OUT 0x101e42, BIT0
#define GPIO111_IN  0x101e4a, BIT0

#define GPIO112_PAD PAD_PCM_A0
#define GPIO112_OEN 0x101e6e, BIT0
#define GPIO112_OUT 0x101e6c, BIT0
#define GPIO112_IN  0x101e6a, BIT0

#define GPIO113_PAD PAD_PCM_A1
#define GPIO113_OEN 0x101e6e, BIT1
#define GPIO113_OUT 0x101e6c, BIT1
#define GPIO113_IN  0x101e6a, BIT1

#define GPIO114_PAD PAD_PCM_A2
#define GPIO114_OEN 0x101e6e, BIT2
#define GPIO114_OUT 0x101e6c, BIT2
#define GPIO114_IN  0x101e6a, BIT2

#define GPIO115_PAD PAD_PCM_A3
#define GPIO115_OEN 0x101e6e, BIT3
#define GPIO115_OUT 0x101e6c, BIT3
#define GPIO115_IN  0x101e6a, BIT3

#define GPIO116_PAD PAD_PCM_A4
#define GPIO116_OEN 0x101e6e, BIT4
#define GPIO116_OUT 0x101e6c, BIT4
#define GPIO116_IN  0x101e6a, BIT4

#define GPIO117_PAD PAD_PCM_A5
#define GPIO117_OEN 0x101e6e, BIT5
#define GPIO117_OUT 0x101e6c, BIT5
#define GPIO117_IN  0x101e6a, BIT5

#define GPIO118_PAD PAD_PCM_A6
#define GPIO118_OEN 0x101e6e, BIT6
#define GPIO118_OUT 0x101e6c, BIT6
#define GPIO118_IN  0x101e6a, BIT6

#define GPIO119_PAD PAD_PCM_A7
#define GPIO119_OEN 0x101e6e, BIT7
#define GPIO119_OUT 0x101e6c, BIT7
#define GPIO119_IN  0x101e6a, BIT7

#define GPIO120_PAD PAD_PCM_A8
#define GPIO120_OEN 0x101e6f, BIT0
#define GPIO120_OUT 0x101e6d, BIT0
#define GPIO120_IN  0x101e6b, BIT0

#define GPIO121_PAD PAD_PCM_A9
#define GPIO121_OEN 0x101e6f, BIT1
#define GPIO121_OUT 0x101e6d, BIT1
#define GPIO121_IN  0x101e6b, BIT1

#define GPIO122_PAD PAD_PCM_A10
#define GPIO122_OEN 0x101e6f, BIT2
#define GPIO122_OUT 0x101e6d, BIT2
#define GPIO122_IN  0x101e6b, BIT2

#define GPIO123_PAD PAD_PCM_A11
#define GPIO123_OEN 0x101e6f, BIT3
#define GPIO123_OUT 0x101e6d, BIT3
#define GPIO123_IN  0x101e6b, BIT3

#define GPIO124_PAD PAD_PCM_A12
#define GPIO124_OEN 0x101e6f, BIT4
#define GPIO124_OUT 0x101e6d, BIT4
#define GPIO124_IN  0x101e6b, BIT4

#define GPIO125_PAD PAD_PCM_A13
#define GPIO125_OEN 0x101e6f, BIT5
#define GPIO125_OUT 0x101e6d, BIT5
#define GPIO125_IN  0x101e6b, BIT5

#define GPIO126_PAD PAD_PCM_A14
#define GPIO126_OEN 0x101e6f, BIT6
#define GPIO126_OUT 0x101e6d, BIT6
#define GPIO126_IN  0x101e6b, BIT6

#define GPIO127_PAD PAD_PCM_RESET
#define GPIO127_OEN 0x101e6f, BIT7
#define GPIO127_OUT 0x101e6d, BIT7
#define GPIO127_IN  0x101e6b, BIT7

#define GPIO128_PAD PAD_PCM_IRQA_N
#define GPIO128_OEN 0x101e74, BIT0
#define GPIO128_OUT 0x101e72, BIT0
#define GPIO128_IN  0x101e70, BIT0

#define GPIO129_PAD PAD_PCM_CD_N
#define GPIO129_OEN 0x101e74, BIT1
#define GPIO129_OUT 0x101e72, BIT1
#define GPIO129_IN  0x101e70, BIT1

#define GPIO130_PAD PAD_PCM_REG_N
#define GPIO130_OEN 0x101e74, BIT2
#define GPIO130_OUT 0x101e72, BIT2
#define GPIO130_IN  0x101e70, BIT2

#define GPIO131_PAD PAD_PCM_CE_N
#define GPIO131_OEN 0x101e74, BIT3
#define GPIO131_OUT 0x101e72, BIT3
#define GPIO131_IN  0x101e70, BIT3

#define GPIO132_PAD PAD_PCM_OE_N
#define GPIO132_OEN 0x101e74, BIT4
#define GPIO132_OUT 0x101e72, BIT4
#define GPIO132_IN  0x101e70, BIT4

#define GPIO133_PAD PAD_PCM_WE_N
#define GPIO133_OEN 0x101e74, BIT5
#define GPIO133_OUT 0x101e72, BIT5
#define GPIO133_IN  0x101e70, BIT5

#define GPIO134_PAD PAD_PCM_WAIT_N
#define GPIO134_OEN 0x101e74, BIT6
#define GPIO134_OUT 0x101e72, BIT6
#define GPIO134_IN  0x101e70, BIT6

#define GPIO135_PAD PAD_PCM_IORD_N
#define GPIO135_OEN 0x101e74, BIT7
#define GPIO135_OUT 0x101e72, BIT7
#define GPIO135_IN  0x101e70, BIT7

#define GPIO136_PAD PAD_PCM_D0
#define GPIO136_OEN 0x101e75, BIT0
#define GPIO136_OUT 0x101e73, BIT0
#define GPIO136_IN  0x101e71, BIT0

#define GPIO137_PAD PAD_PCM_D1
#define GPIO137_OEN 0x101e75, BIT1
#define GPIO137_OUT 0x101e73, BIT1
#define GPIO137_IN  0x101e71, BIT1

#define GPIO138_PAD PAD_PCM_D2
#define GPIO138_OEN 0x101e75, BIT2
#define GPIO138_OUT 0x101e73, BIT2
#define GPIO138_IN  0x101e71, BIT2

#define GPIO139_PAD PAD_PCM_D3
#define GPIO139_OEN 0x101e75, BIT3
#define GPIO139_OUT 0x101e73, BIT3
#define GPIO139_IN  0x101e71, BIT3

#define GPIO140_PAD PAD_PCM_D4
#define GPIO140_OEN 0x101e75, BIT4
#define GPIO140_OUT 0x101e73, BIT4
#define GPIO140_IN  0x101e71, BIT4

#define GPIO141_PAD PAD_PCM_D5
#define GPIO141_OEN 0x101e75, BIT5
#define GPIO141_OUT 0x101e73, BIT5
#define GPIO141_IN  0x101e71, BIT5

#define GPIO142_PAD PAD_PCM_D6
#define GPIO142_OEN 0x101e75, BIT6
#define GPIO142_OUT 0x101e73, BIT6
#define GPIO142_IN  0x101e71, BIT6

#define GPIO143_PAD PAD_PCM_D7
#define GPIO143_OEN 0x101e75, BIT7
#define GPIO143_OUT 0x101e73, BIT7
#define GPIO143_IN  0x101e71, BIT7

#define GPIO144_PAD PAD_TS0_D0
#define GPIO144_OEN 0x101e98, BIT0
#define GPIO144_OUT 0x101e96, BIT0
#define GPIO144_IN  0x101e94, BIT0

#define GPIO145_PAD PAD_TS0_D1
#define GPIO145_OEN 0x101e98, BIT1
#define GPIO145_OUT 0x101e96, BIT1
#define GPIO145_IN  0x101e94, BIT1

#define GPIO146_PAD PAD_TS0_D2
#define GPIO146_OEN 0x101e98, BIT2
#define GPIO146_OUT 0x101e96, BIT2
#define GPIO146_IN  0x101e94, BIT2

#define GPIO147_PAD PAD_TS0_D3
#define GPIO147_OEN 0x101e98, BIT3
#define GPIO147_OUT 0x101e96, BIT3
#define GPIO147_IN  0x101e94, BIT3

#define GPIO148_PAD PAD_TS0_D4
#define GPIO148_OEN 0x101e98, BIT4
#define GPIO148_OUT 0x101e96, BIT4
#define GPIO148_IN  0x101e94, BIT4

#define GPIO149_PAD PAD_TS0_D5
#define GPIO149_OEN 0x101e98, BIT5
#define GPIO149_OUT 0x101e96, BIT5
#define GPIO149_IN  0x101e94, BIT5

#define GPIO150_PAD PAD_TS0_D6
#define GPIO150_OEN 0x101e98, BIT6
#define GPIO150_OUT 0x101e96, BIT6
#define GPIO150_IN  0x101e94, BIT6

#define GPIO151_PAD PAD_TS0_D7
#define GPIO151_OEN 0x101e98, BIT7
#define GPIO151_OUT 0x101e96, BIT7
#define GPIO151_IN  0x101e94, BIT7

#define GPIO152_PAD PAD_TS0_VLD
#define GPIO152_OEN 0x101e99, BIT0
#define GPIO152_OUT 0x101e97, BIT0
#define GPIO152_IN  0x101e95, BIT0

#define GPIO153_PAD PAD_TS0_SYNC
#define GPIO153_OEN 0x101e99, BIT1
#define GPIO153_OUT 0x101e97, BIT1
#define GPIO153_IN  0x101e95, BIT1

#define GPIO154_PAD PAD_TS0_CLK
#define GPIO154_OEN 0x101e99, BIT2
#define GPIO154_OUT 0x101e97, BIT2
#define GPIO154_IN  0x101e95, BIT2

#define GPIO155_PAD PAD_TS1_D0
#define GPIO155_OEN 0x101e54, BIT0
#define GPIO155_OUT 0x101e52, BIT0
#define GPIO155_IN  0x101e50, BIT0

#define GPIO156_PAD PAD_TS1_D1
#define GPIO156_OEN 0x101e54, BIT1
#define GPIO156_OUT 0x101e52, BIT1
#define GPIO156_IN  0x101e50, BIT1

#define GPIO157_PAD PAD_TS1_D2
#define GPIO157_OEN 0x101e54, BIT2
#define GPIO157_OUT 0x101e52, BIT2
#define GPIO157_IN  0x101e50, BIT2

#define GPIO158_PAD PAD_TS1_D3
#define GPIO158_OEN 0x101e54, BIT3
#define GPIO158_OUT 0x101e52, BIT3
#define GPIO158_IN  0x101e50, BIT3

#define GPIO159_PAD PAD_TS1_D4
#define GPIO159_OEN 0x101e54, BIT4
#define GPIO159_OUT 0x101e52, BIT4
#define GPIO159_IN  0x101e50, BIT4

#define GPIO160_PAD PAD_TS1_D5
#define GPIO160_OEN 0x101e54, BIT5
#define GPIO160_OUT 0x101e52, BIT5
#define GPIO160_IN  0x101e50, BIT5

#define GPIO161_PAD PAD_TS1_D6
#define GPIO161_OEN 0x101e54, BIT6
#define GPIO161_OUT 0x101e52, BIT6
#define GPIO161_IN  0x101e50, BIT6

#define GPIO162_PAD PAD_TS1_D7
#define GPIO162_OEN 0x101e54, BIT7
#define GPIO162_OUT 0x101e52, BIT7
#define GPIO162_IN  0x101e50, BIT7

#define GPIO163_PAD PAD_TS1_VLD
#define GPIO163_OEN 0x101e55, BIT0
#define GPIO163_OUT 0x101e55, BIT0
#define GPIO163_IN  0x101e55, BIT0

#define GPIO164_PAD PAD_TS1_SYNC
#define GPIO164_OEN 0x101e55, BIT1
#define GPIO164_OUT 0x101e55, BIT1
#define GPIO164_IN  0x101e55, BIT1

#define GPIO165_PAD PAD_TS1_CLK
#define GPIO165_OEN 0x101e55, BIT2
#define GPIO165_OUT 0x101e55, BIT2
#define GPIO165_IN  0x101e55, BIT2

#define GPIO166_PAD PAD_TS2_D
#define GPIO166_OEN 0x101e99, BIT4
#define GPIO166_OUT 0x101e97, BIT4
#define GPIO166_IN  0x101e95, BIT4

#define GPIO167_PAD PAD_TS2_VLD
#define GPIO167_OEN 0x101e99, BIT5
#define GPIO167_OUT 0x101e97, BIT5
#define GPIO167_IN  0x101e95, BIT5

#define GPIO168_PAD PAD_TS2_SYNC
#define GPIO168_OEN 0x101e99, BIT6
#define GPIO168_OUT 0x101e97, BIT6
#define GPIO168_IN  0x101e95, BIT6

#define GPIO169_PAD PAD_TS2_CLK
#define GPIO169_OEN 0x101e99, BIT7
#define GPIO169_OUT 0x101e97, BIT7
#define GPIO169_IN  0x101e95, BIT7

#define GPIO170_PAD PAD_ARC
#define GPIO170_OEN 0x101e19, BIT4
#define GPIO170_OUT 0x101e17, BIT4
#define GPIO170_IN  0x101e15, BIT4

#define GPIO171_PAD PADA_HSYNC0
#define GPIO171_OEN 0x10255a, BIT0
#define GPIO171_OUT 0x10255a, BIT6
#define GPIO171_IN  0x10255c, BIT0

#define GPIO172_PAD PADA_HSYNC1
#define GPIO172_OEN 0x10255a, BIT1
#define GPIO172_OUT 0x10255a, BIT7
#define GPIO172_IN  0x10255c, BIT1

#define GPIO173_PAD PADA_HSYNC2
#define GPIO173_OEN 0x10255a, BIT2
#define GPIO173_OUT 0x10255b, BIT0
#define GPIO173_IN  0x10255c, BIT2

#define GPIO174_PAD PADA_VSYNC0
#define GPIO174_OEN 0x10255a, BIT3
#define GPIO174_OUT 0x10255b, BIT1
#define GPIO174_IN  0x10255c, BIT3

#define GPIO175_PAD PADA_VSYNC1
#define GPIO175_OEN 0x10255a, BIT4
#define GPIO175_OUT 0x10255b, BIT2
#define GPIO175_IN  0x10255c, BIT4

#define GPIO176_PAD PADA_VSYNC2
#define GPIO176_OEN 0x10255a, BIT5
#define GPIO176_OUT 0x10255b, BIT3
#define GPIO176_IN  0x10255c, BIT5

#define GPIO177_PAD PADA_RIN0P
#define GPIO177_OEN 0x102536, BIT0
#define GPIO177_OUT 0x102537, BIT0
#define GPIO177_IN  0x10253c, BIT0

#define GPIO178_PAD PADA_RIN1P
#define GPIO178_OEN 0x102536, BIT1
#define GPIO178_OUT 0x102537, BIT1
#define GPIO178_IN  0x10253c, BIT1

#define GPIO179_PAD PADA_RIN2P
#define GPIO179_OEN 0x102536, BIT2
#define GPIO179_OUT 0x102537, BIT2
#define GPIO179_IN  0x10253c, BIT2

#define GPIO180_PAD PADA_RIN0M
#define GPIO180_OEN 0x102536, BIT4
#define GPIO180_OUT 0x102537, BIT4
#define GPIO180_IN  0x10253c, BIT4

#define GPIO181_PAD PADA_RIN1M
#define GPIO181_OEN 0x102536, BIT5
#define GPIO181_OUT 0x102537, BIT5
#define GPIO181_IN  0x10253c, BIT5

#define GPIO182_PAD PADA_RIN2M
#define GPIO182_OEN 0x102536, BIT6
#define GPIO182_OUT 0x102537, BIT6
#define GPIO182_IN  0x10253c, BIT6

#define GPIO183_PAD PADA_GIN0P
#define GPIO183_OEN 0x102538, BIT0
#define GPIO183_OUT 0x102539, BIT0
#define GPIO183_IN  0x10253d, BIT0

#define GPIO184_PAD PADA_GIN1P
#define GPIO184_OEN 0x102538, BIT1
#define GPIO184_OUT 0x102539, BIT1
#define GPIO184_IN  0x10253d, BIT1

#define GPIO185_PAD PADA_GIN2P
#define GPIO185_OEN 0x102538, BIT2
#define GPIO185_OUT 0x102539, BIT2
#define GPIO185_IN  0x10253d, BIT2

#define GPIO186_PAD PADA_GIN0M
#define GPIO186_OEN 0x102538, BIT4
#define GPIO186_OUT 0x102539, BIT4
#define GPIO186_IN  0x10253d, BIT4

#define GPIO187_PAD PADA_GIN1M
#define GPIO187_OEN 0x102538, BIT5
#define GPIO187_OUT 0x102539, BIT5
#define GPIO187_IN  0x10253d, BIT5

#define GPIO188_PAD PADA_GIN2M
#define GPIO188_OEN 0x102538, BIT6
#define GPIO188_OUT 0x102539, BIT6
#define GPIO188_IN  0x10253d, BIT6

#define GPIO189_PAD PADA_BIN0P
#define GPIO189_OEN 0x10253a, BIT0
#define GPIO189_OUT 0x10253b, BIT0
#define GPIO189_IN  0x10253e, BIT0

#define GPIO190_PAD PADA_BIN1P
#define GPIO190_OEN 0x10253a, BIT1
#define GPIO190_OUT 0x10253b, BIT1
#define GPIO190_IN  0x10253e, BIT1

#define GPIO191_PAD PADA_BIN2P
#define GPIO191_OEN 0x10253a, BIT2
#define GPIO191_OUT 0x10253b, BIT2
#define GPIO191_IN  0x10253e, BIT2

#define GPIO192_PAD PADA_BIN0M
#define GPIO192_OEN 0x10253a, BIT4
#define GPIO192_OUT 0x10253b, BIT4
#define GPIO192_IN  0x10253e, BIT4

#define GPIO193_PAD PADA_BIN1M
#define GPIO193_OEN 0x10253a, BIT5
#define GPIO193_OUT 0x10253b, BIT5
#define GPIO193_IN  0x10253e, BIT5

#define GPIO194_PAD PADA_BIN2M
#define GPIO194_OEN 0x10253a, BIT6
#define GPIO194_OUT 0x10253b, BIT6
#define GPIO194_IN  0x10253e, BIT6

#define GPIO195_PAD PAD_LINEIN_L1
#define GPIO195_OEN 0x112cf7, BIT2 
#define GPIO195_OUT 0x112cf7, BIT0 
#define GPIO195_IN  0x112cf7, BIT3 

#define GPIO196_PAD PAD_LINEIN_R1
#define GPIO196_OEN 0x112cf7, BIT6 
#define GPIO196_OUT 0x112cf7, BIT4 
#define GPIO196_IN  0x112cf7, BIT7 

#define GPIO197_PAD PAD_LINEIN_L2
#define GPIO197_OEN 0x112cf8, BIT2  
#define GPIO197_OUT 0x112cf8, BIT0  
#define GPIO197_IN  0x112cf8, BIT3  

#define GPIO198_PAD PAD_LINEIN_R2
#define GPIO198_OEN 0x112cf8, BIT6  
#define GPIO198_OUT 0x112cf8, BIT4  
#define GPIO198_IN  0x112cf8, BIT7  

#define GPIO199_PAD PAD_LINEIN_L3
#define GPIO199_OEN 0x112cf9, BIT2 
#define GPIO199_OUT 0x112cf9, BIT0 
#define GPIO199_IN  0x112cf9, BIT3 

#define GPIO200_PAD PAD_LINEIN_R3
#define GPIO200_OEN 0x112cf9, BIT6 
#define GPIO200_OUT 0x112cf9, BIT4 
#define GPIO200_IN  0x112cf9, BIT7 

#define GPIO201_PAD PAD_LINEIN_L4
#define GPIO201_OEN 0x112cfa, BIT2  
#define GPIO201_OUT 0x112cfa, BIT0  
#define GPIO201_IN  0x112cfa, BIT3  

#define GPIO202_PAD PAD_LINEIN_R4
#define GPIO202_OEN 0x112cfa, BIT6  
#define GPIO202_OUT 0x112cfa, BIT4  
#define GPIO202_IN  0x112cfa, BIT7  

#define GPIO203_PAD PAD_LINEIN_L5
#define GPIO203_OEN 0x112cfb, BIT2
#define GPIO203_OUT 0x112cfb, BIT0
#define GPIO203_IN  0x112cfb, BIT3

#define GPIO204_PAD PAD_LINEIN_R5
#define GPIO204_OEN 0x112cfb, BIT6
#define GPIO204_OUT 0x112cfb, BIT4
#define GPIO204_IN  0x112cfb, BIT7

#define GPIO205_PAD PAD_RXCN_A //0x1109f9[4][0]=2'10 (output), 0x1109f9[4][0]=2'01 (input)
#define GPIO205_OEN 0x1109f9, BIT0
#define GPIO205_OUT 0x1109f9, BIT4
#define GPIO205_IN  0x1109f5, BIT0

#define GPIO206_PAD PAD_RX0N_A
#define GPIO206_OEN 0x1109f9, BIT0
#define GPIO206_OUT 0x1109f9, BIT4
#define GPIO206_IN  0x1109f5, BIT1

#define GPIO207_PAD PAD_RX1N_A
#define GPIO207_OEN 0x1109f9, BIT0
#define GPIO207_OUT 0x1109f9, BIT4
#define GPIO207_IN  0x1109f5, BIT2

#define GPIO208_PAD PAD_RX2N_A
#define GPIO208_OEN 0x1109f9, BIT0
#define GPIO208_OUT 0x1109f9, BIT4
#define GPIO208_IN  0x1109f5, BIT3

#define GPIO209_PAD PAD_RXCP_A
#define GPIO209_OEN 0x1109f9, BIT0
#define GPIO209_OUT 0x1109f9, BIT4
#define GPIO209_IN  0x1109f5, BIT4

#define GPIO210_PAD PAD_RX0P_A
#define GPIO210_OEN 0x1109f9, BIT0
#define GPIO210_OUT 0x1109f9, BIT4
#define GPIO210_IN  0x1109f5, BIT5

#define GPIO211_PAD PAD_RX1P_A
#define GPIO211_OEN 0x1109f9, BIT0
#define GPIO211_OUT 0x1109f9, BIT4
#define GPIO211_IN  0x1109f5, BIT6

#define GPIO212_PAD PAD_RX2P_A
#define GPIO212_OEN 0x1109f9, BIT0
#define GPIO212_OUT 0x1109f9, BIT4
#define GPIO212_IN  0x1109f5, BIT7

#define GPIO213_PAD PAD_RXCN_B //0x1109f9[5][1]=2'10 (output), 0x1109f9[5][1]=2'01 (input)
#define GPIO213_OEN 0x1109f9, BIT1
#define GPIO213_OUT 0x1109f9, BIT5
#define GPIO213_IN  0x1109f6, BIT0

#define GPIO214_PAD PAD_RX0N_B
#define GPIO214_OEN 0x1109f9, BIT1
#define GPIO214_OUT 0x1109f9, BIT5
#define GPIO214_IN  0x1109f6, BIT1

#define GPIO215_PAD PAD_RX1N_B
#define GPIO215_OEN 0x1109f9, BIT1
#define GPIO215_OUT 0x1109f9, BIT5
#define GPIO215_IN  0x1109f6, BIT2

#define GPIO216_PAD PAD_RX2N_B
#define GPIO216_OEN 0x1109f9, BIT1
#define GPIO216_OUT 0x1109f9, BIT5
#define GPIO216_IN  0x1109f6, BIT3

#define GPIO217_PAD PAD_RXCP_B
#define GPIO217_OEN 0x1109f9, BIT1
#define GPIO217_OUT 0x1109f9, BIT5
#define GPIO217_IN  0x1109f6, BIT4

#define GPIO218_PAD PAD_RX0P_B
#define GPIO218_OEN 0x1109f9, BIT1
#define GPIO218_OUT 0x1109f9, BIT5
#define GPIO218_IN  0x1109f6, BIT5

#define GPIO219_PAD PAD_RX1P_B
#define GPIO219_OEN 0x1109f9, BIT1
#define GPIO219_OUT 0x1109f9, BIT5
#define GPIO219_IN  0x1109f6, BIT6

#define GPIO220_PAD PAD_RX2P_B
#define GPIO220_OEN 0x1109f9, BIT1
#define GPIO220_OUT 0x1109f9, BIT5
#define GPIO220_IN  0x1109f6, BIT7

#define GPIO221_PAD PAD_RXCN_D //0x1109f9[6][2]=2'10 (output), 0x1109f9[6][2]=2'01 (input)
#define GPIO221_OEN 0x1109f9, BIT2
#define GPIO221_OUT 0x1109f9, BIT6
#define GPIO221_IN  0x1109f7, BIT0

#define GPIO222_PAD PAD_RX0N_D
#define GPIO222_OEN 0x1109f9, BIT2
#define GPIO222_OUT 0x1109f9, BIT6
#define GPIO222_IN  0x1109f7, BIT1

#define GPIO223_PAD PAD_RX1N_D
#define GPIO223_OEN 0x1109f9, BIT2
#define GPIO223_OUT 0x1109f9, BIT6
#define GPIO223_IN  0x1109f7, BIT2

#define GPIO224_PAD PAD_RX2N_D
#define GPIO224_OEN 0x1109f9, BIT2
#define GPIO224_OUT 0x1109f9, BIT6
#define GPIO224_IN  0x1109f7, BIT3

#define GPIO225_PAD PAD_RXCP_D
#define GPIO225_OEN 0x1109f9, BIT2
#define GPIO225_OUT 0x1109f9, BIT6
#define GPIO225_IN  0x1109f7, BIT4

#define GPIO226_PAD PAD_RX0P_D
#define GPIO226_OEN 0x1109f9, BIT2
#define GPIO226_OUT 0x1109f9, BIT6
#define GPIO226_IN  0x1109f7, BIT5

#define GPIO227_PAD PAD_RX1P_D
#define GPIO227_OEN 0x1109f9, BIT2
#define GPIO227_OUT 0x1109f9, BIT6
#define GPIO227_IN  0x1109f7, BIT6

#define GPIO228_PAD PAD_RX2P_D
#define GPIO228_OEN 0x1109f9, BIT2
#define GPIO228_OUT 0x1109f9, BIT6
#define GPIO228_IN  0x1109f7, BIT7

//----------------------------------------------------------------------------
// GPIO dispatcher
//----------------------------------------------------------------------------

#define GPIO_NUM(pin)       CONCAT(GPIO_PAD_, pin)

//----------------------------------------------------------------------------
// Multi-SPI Flash
//----------------------------------------------------------------------------
#define SPI_SEL_NONE()      MDrv_WriteByte(ISP_REG_BASE+0xF4, 0x00)   // external #1
#define SPI_SEL_CZ0()       MDrv_WriteByte(ISP_REG_BASE+0xF4, 0x00)   // external #1
#define SPI_SEL_CZ1()       MDrv_WriteByte(ISP_REG_BASE+0xF4, 0x01)   // external #2
#define SPI_SEL_CZ2()       MDrv_WriteByte(ISP_REG_BASE+0xF4, 0x02)   // external #3
#define SPI_SEL_CZ3()       MDrv_WriteByte(ISP_REG_BASE+0xF4, 0x03)   // Reserved

#endif /* _DRV_GPIO_H_ */
