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
// Description: Interrupt Service Routines
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_ISR_H_
#define _DRV_ISR_H_

#ifdef _DRV_ISR_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

#if 1  //Titania_3

//47~63  reserved
#define FIQ_MIPS_TO_8051                   (MDRV_FIQ_BEGIN + 46)
#define FIQ_MIPS_TO_AEON                  (MDRV_FIQ_BEGIN + 45)
#define FIQ_MIPS_1_TO_MIPS                 (MDRV_FIQ_BEGIN + 44)
//43   reserved
#define FIQ_AEON_TO_8051                 (MDRV_FIQ_BEGIN + 42)
#define FIQ_AEON_TO_MIPS                 (MDRV_FIQ_BEGIN + 41)
#define FIQ_AEON_TO_MIPS_1                 (MDRV_FIQ_BEGIN + 40)
//39  reserved

#define FIQ_8051_TO_AEON                  (MDRV_FIQ_BEGIN + 38)
#define FIQ_8051_TO_MIPS                 (MDRV_FIQ_BEGIN + 37)
#define FIQ_8051_TO_MIPS_1                (MDRV_FIQ_BEGIN + 36)
//34~35  reserved

#define FIQ_AU_DMA_BUFFER_INT_EDGE       (MDRV_FIQ_BEGIN + 33)
#define FIQ_IR_RC                        (MDRV_FIQ_BEGIN + 32)
#define FIQ_DSP2MIPS                     (MDRV_FIQ_BEGIN + 31)
#define FIQ_MIPS_WDT                     (MDRV_FIQ_BEGIN + 30)
#define FIQ_DEC_DSP2UP                   (MDRV_FIQ_BEGIN + 29)
#define FIQ_AFEC_VSYNC                   (MDRV_FIQ_BEGIN + 28)
#define FIQ_IR                           (MDRV_FIQ_BEGIN + 27)
#define FIQ_INT_IN                       (MDRV_FIQ_BEGIN + 26)
#define FIQ_INT_CCFL                     (MDRV_FIQ_BEGIN + 25)
#define FIQ_VE_DONE_TT                   (MDRV_FIQ_BEGIN + 24)
#define FIQ_VDMCU2HK                     (MDRV_FIQ_BEGIN + 23)
#define FIQ_VE_VBI_F1                    (MDRV_FIQ_BEGIN + 22)
#define FIQ_VE_VBI_F0                    (MDRV_FIQ_BEGIN + 21)
//20 reserved
#define FIQ_XIU_TIMEOUT                  (MDRV_FIQ_BEGIN + 19)
#define FIQ_DSP_MIU_PROT                 (MDRV_FIQ_BEGIN + 18)
#define FIQ_VIVALDI_PTS                  (MDRV_FIQ_BEGIN + 17)
#define FIQ_VIVALDI_STR                  (MDRV_FIQ_BEGIN + 16)
#define FIQ_SE_DSP2UP                    (MDRV_FIQ_BEGIN + 14)
#define FIQ_EMAC                         (MDRV_FIQ_BEGIN + 13)
#define FIQ_SPDIF_IN_NON_PCM_INT_OUT     (MDRV_FIQ_BEGIN + 12)
#define FIQ_HDMI_NON_PCM_MODE_INT_OUT    (MDRV_FIQ_BEGIN + 11)
//reserved 3~10
#define FIQ_WDT                          (MDRV_FIQ_BEGIN + 2)
#define FIQ_TIMER1                       (MDRV_FIQ_BEGIN + 1)
#define FIQ_TIMER0                       (MDRV_FIQ_BEGIN + 0)


#define FIQ_AEON_TO_BEON                 (MDRV_FIQ_BEGIN + 41) //FIQ_AEON_TO_MIPS   //for backward competible
#define FIQ_BEON_TO_AEON                 (MDRV_FIQ_BEGIN + 45) //FIQ_MIPS_TO_AEON   //for backward competible

//-------------------------------------------------------
//-------------------------------------------------------
#define    IRQ_HDCP_X74       (MDRV_IRQ_BEGIN + 56)//56 (level)
#define    IRQ_G3D2MCU_DFT    (MDRV_IRQ_BEGIN + 53)//53 (level)
#define    IRQ_DVI_HDMI_HDCP  (MDRV_IRQ_BEGIN + 52)//52 (level)
#define    IRQ_URDMA2MCU      (MDRV_IRQ_BEGIN + 51)//51 (level)
#define    IRQ_UART2MCU       (MDRV_IRQ_BEGIN + 50)//50 (level)
#define    IRQ_BDMA_CH1       (MDRV_IRQ_BEGIN + 49)//49 (level)  channel_1
#define    IRQ_BDMA_CH0       (MDRV_IRQ_BEGIN + 48)//48 (level)   channel_0
#define    IRQ_M4VE           (MDRV_IRQ_BEGIN + 47)//47 (level)
#define    IRQ_DISPI_IRQ_OUT  (MDRV_IRQ_BEGIN + 46)//46 (level)
#define    IRQ_JPD            (MDRV_IRQ_BEGIN + 45)//45 (level)
#define    IRQ_MLINK          (MDRV_IRQ_BEGIN + 44)//44 (level)
#define    IRQ_MPIF           (MDRV_IRQ_BEGIN + 42)//42 (level)
#define    IRQ_UART2          (MDRV_IRQ_BEGIN + 40)//40 (level)
#define    IRQ_UART1          (MDRV_IRQ_BEGIN + 39)//39 (level)
#define    IRQ_AEON2HI        (MDRV_IRQ_BEGIN + 38)//38 (level)
#define    IRQ_TMDDRLINK      (MDRV_IRQ_BEGIN + 37)//37 (level)
#define    IRQ_DPTX           (MDRV_IRQ_BEGIN + 36)//36 (level)
#define    IRQ_MIU            (MDRV_IRQ_BEGIN + 35)//35 (level)
#define    IRQ_UHC_1          (MDRV_IRQ_BEGIN + 34)//34 (level)
#define    IRQ_USB_1          (MDRV_IRQ_BEGIN + 33)//33 (level)
#define    IRQ_HVD            (MDRV_IRQ_BEGIN + 32)//32 (level)
#define    IRQ_ADCDVI2RIU     (MDRV_IRQ_BEGIN + 31)//31 (level)
#define    IRQ_AFEC           (MDRV_IRQ_BEGIN + 30)//30 (level)
#define    IRQ_MSTAR_LINK     (MDRV_IRQ_BEGIN + 29)//29 (level)
#define    IRQ_VBI            (MDRV_IRQ_BEGIN + 28)//28 (level)
#define    IRQ_SCM            (MDRV_IRQ_BEGIN + 27)//27 (level)
#define    IRQ_DDC2BI         (MDRV_IRQ_BEGIN + 26)//26 (level)
#define    IRQ_PM             (MDRV_IRQ_BEGIN + 25)//25 (level)
#define    IRQ_KEYPAD         (MDRV_IRQ_BEGIN + 24)//24 (level)
#define    IRQ_RTC            (MDRV_IRQ_BEGIN + 23)//23 (level)
#define    IRQ_MIIC0          (MDRV_IRQ_BEGIN + 22)//22 (level)
#define    IRQ_PCM2MCU        (MDRV_IRQ_BEGIN + 21)//21 (level)
#define    IRQ_GOP            (MDRV_IRQ_BEGIN + 20)//20 (level)
#define    IRQ_DC             (MDRV_IRQ_BEGIN + 19)//19 (level)
#define    IRQ_CIMAX2MCU      (MDRV_IRQ_BEGIN + 18)//18 (level)
#define    IRQ_VE             (MDRV_IRQ_BEGIN + 17)//17 (level)
#define    IRQ_TSP2HK         (MDRV_IRQ_BEGIN + 16)//16 (level)
#define    IRQ_COMB           (MDRV_IRQ_BEGIN + 14)//14 (level)
#define    IRQ_SBM            (MDRV_IRQ_BEGIN + 13)//13 (level)
#define    IRQ_DHC            (MDRV_IRQ_BEGIN + 11)//11 (level)
#define    IRQ_DISP           (MDRV_IRQ_BEGIN + 10)//10 (level)
#define    IRQ_EMAC           (MDRV_IRQ_BEGIN + 9)//9  (level)
#define    IRQ_EC_BRIDGE      (MDRV_IRQ_BEGIN + 8)//8  (level)
#define    IRQ_UHC            (MDRV_IRQ_BEGIN + 7)//7  (level)
#define    IRQ_USB            (MDRV_IRQ_BEGIN + 6)      //6  (level)
#define    IRQ_NFIE           (MDRV_IRQ_BEGIN + 5)      //5  (level)
#define    IRQ_PS             (MDRV_IRQ_BEGIN + 4)      // //4  (level)
#define    IRQ_MVD            (MDRV_IRQ_BEGIN + 3)       // 3  (level)
#define    IRQ_UART0          (MDRV_IRQ_BEGIN + 0)           //0  (level)

#else   // T2 int table

#define FIQ_TIMER0              (MDRV_FIQ_BEGIN + 0)
#define FIQ_TIMER1              (MDRV_FIQ_BEGIN + 1)
#define FIQ_WDT                 (MDRV_FIQ_BEGIN + 2)
#define FIQ_AEON_TO_8051        (MDRV_FIQ_BEGIN + 3)
#define FIQ_8051_TO_AEON        (MDRV_FIQ_BEGIN + 4)
#define FIQ_8051_TO_BEON        (MDRV_FIQ_BEGIN + 5)
#define FIQ_BEON_TO_8051        (MDRV_FIQ_BEGIN + 6)
#define FIQ_BEON_TO_AEON        (MDRV_FIQ_BEGIN + 7)
#define FIQ_AEON_TO_BEON        (MDRV_FIQ_BEGIN + 8)
#define FIQ_MENULOAD            (MDRV_FIQ_BEGIN + 10)
#define FIQ_HDMI_NON_PCM        (MDRV_FIQ_BEGIN + 11)
#define FIQ_SPDIF_IN_NON_PCM    (MDRV_FIQ_BEGIN + 12)
#define FIQ_EMAC                (MDRV_FIQ_BEGIN + 13)
#define FIQ_SE_DSP2UP           (MDRV_FIQ_BEGIN + 14)
#define FIQ_TSP2AEON            (MDRV_FIQ_BEGIN + 15)
#define FIQ_VIVALDI_STR         (MDRV_FIQ_BEGIN + 16)
#define FIQ_VIVALDI_PTS         (MDRV_FIQ_BEGIN + 17)
#define FIQ_DSP_MIU_PROT        (MDRV_FIQ_BEGIN + 18)
#define FIQ_XIU_TIMEOUT         (MDRV_FIQ_BEGIN + 19)
#define FIQ_DMA_DONE            (MDRV_FIQ_BEGIN + 20)
#define FIQ_VE_VBI_F0           (MDRV_FIQ_BEGIN + 21)
#define FIQ_VE_VBI_F1           (MDRV_FIQ_BEGIN + 22)
#define FIQ_VDMCU2HK            (MDRV_FIQ_BEGIN + 23)
#define FIQ_VE_DONE_TT          (MDRV_FIQ_BEGIN + 24)
#define FIQ_IR                  (MDRV_FIQ_BEGIN + 27)
#define FIQ_AFEC_VSYNC          (MDRV_FIQ_BEGIN + 28)
#define FIQ_DEC_DSP2UP          (MDRV_FIQ_BEGIN + 29)
#define FIQ_BEON_WDT            (MDRV_FIQ_BEGIN + 30)
#define FIQ_DSP2BEON            (MDRV_FIQ_BEGIN + 31)

#define IRQ_UART0               (MDRV_IRQ_BEGIN + 0)
#define IRQ_BDMA_CH0            (MDRV_IRQ_BEGIN + 1)
#define IRQ_BDMA_CH1            (MDRV_IRQ_BEGIN + 2)
#define IRQ_MVD                 (MDRV_IRQ_BEGIN + 3)
#define IRQ_PS                  (MDRV_IRQ_BEGIN + 4)
#define IRQ_NFIE                (MDRV_IRQ_BEGIN + 5)
#define IRQ_USB                 (MDRV_IRQ_BEGIN + 6)
#define IRQ_UHC                 (MDRV_IRQ_BEGIN + 7)
#define IRQ_EC_BRIDGE           (MDRV_IRQ_BEGIN + 8)
#define IRQ_EMAC                (MDRV_IRQ_BEGIN + 9)
#define IRQ_DISP                (MDRV_IRQ_BEGIN + 10)
#define IRQ_PMSLEEP             (MDRV_IRQ_BEGIN + 12)
#define IRQ_SBM                 (MDRV_IRQ_BEGIN + 13)
#define IRQ_COMB                (MDRV_IRQ_BEGIN + 14)
#define IRQ_TSP2HK              (MDRV_IRQ_BEGIN + 16)
#define IRQ_VE                  (MDRV_IRQ_BEGIN + 17)
#define IRQ_CIMAX               (MDRV_IRQ_BEGIN + 18)
#define IRQ_DC                  (MDRV_IRQ_BEGIN + 19)
#define IRQ_GOP                 (MDRV_IRQ_BEGIN + 20)
#define IRQ_PCM                 (MDRV_IRQ_BEGIN + 21)
#define IRQ_IIC0                (MDRV_IRQ_BEGIN + 22)
#define IRQ_KEYPAD              (MDRV_IRQ_BEGIN + 24)
#define IRQ_DDC2BI              (MDRV_IRQ_BEGIN + 26)
#define IRQ_SCM                 (MDRV_IRQ_BEGIN + 27)
#define IRQ_VBI                 (MDRV_IRQ_BEGIN + 28)
#define IRQ_ADCDVI2RIU          (MDRV_IRQ_BEGIN + 31)
#define IRQ_SVD                 (MDRV_IRQ_BEGIN + 32)
#define IRQ_USB2                (MDRV_IRQ_BEGIN + 33)
#define IRQ_UHC2                (MDRV_IRQ_BEGIN + 34)
#define IRQ_MIU                 (MDRV_IRQ_BEGIN + 35)
#endif

#define ISR_TIMER0_PERIOD_US        1000UL  // : 1000us, unit:us, Timer 0 Interrupt period

INTERFACE U32 volatile  gTimerCount0;
INTERFACE U32 volatile  gTimerDownCount0;
INTERFACE U32 volatile  gTimerDownCount1;
INTERFACE U32 volatile  gOutVsyncCount;    //ISR,  //**

INTERFACE U32 volatile      g100msTimeCount;
INTERFACE U8                gu8100msTH;
//INTERFACE U16               gu16SystemTimePeriodTH;
INTERFACE U16        g_u16ElapsedTimeSecond;

INTERFACE U8 gu8MADPlayFile, gu8MADEncodeDone;

#undef INTERFACE

#endif /* _DRV_ISR_H_ */

