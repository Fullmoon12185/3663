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

#ifndef _DRVSYS_H_
#define _DRVSYS_H_
#include "hwreg.h"
#include "drvGlobal.h"
#include "Board.h"
#include "InfoBlock.h"

#ifdef _DRV_SYS_C_
    #define DRVSYS_EX
#else
    #define DRVSYS_EX extern
#endif

#define SUBTITIE_OFFSET             0x10000

////////////////////////////////////////////////////////////////////////////////
// prototype for putcharUARTn
////////////////////////////////////////////////////////////////////////////////

#if ( ENABLE_UART0 )
DRVSYS_EX char putcharUART0( char c );
#else
#define putcharUART0( c )   _FUNC_NOT_USED()
#endif

#if 0//( ENABLE_UART1 )
DRVSYS_EX char putcharUART1( char c );
#else
#define putcharUART1( c )   _FUNC_NOT_USED()
#endif

DRVSYS_EX void puthex( U16 u16HexVal );

////////////////////////////////////////////////////////////////////////////////
// PIU Watch Dog Timer
////////////////////////////////////////////////////////////////////////////////

// Disable watchdog is not suggested at any time, please ask related people first
// 1. During firmware upgrade, disable WDT before erase flash
// 2. In factory menu, WDT on/off option

#define MDrv_Sys_WatchDogCycles( sec ) (  (((sec) * MST_XTAL_CLOCK_HZ)) )



#define MDrv_Sys_ClearWatchDog()        MDrv_WriteByte(RIUBASE_WDT, MDrv_ReadByte(RIUBASE_WDT)|(BIT0))


#define  MDrv_Sys_IsWatchDogEnabled()    (MDrv_Read4Byte(WDT_TIMER_0))

DRVSYS_EX void  MDrv_Sys_EnableWatchDog(void);
DRVSYS_EX void MDrv_Sys_SetWatchDogTimer( U8 sec );
DRVSYS_EX void MDrv_Sys_DisableWatchDog( void );




// This is not really necessary, WDT is enabled after MCU reset

//#define MDrv_Sys_IsWatchDogEnabled()    (MDrv_Read2Byte(WDT_RST) != 0xAA55)

#define MDrv_Sys_IsResetByWatchDog()    (MDrv_ReadByte(WDT_RST) & BIT0)


#define MDrv_Sys_ClearWatchDogStatus()  MDrv_WriteByte(WDT_RST, BIT0)




////////////////////////////////////////////////////////////////////////////////
// Uart mux selection (see UART_SEL_TYPE in hwreg_S4L.h for mux)
////////////////////////////////////////////////////////////////////////////////

#define MDrv_Sys_SetUart0Mux(mux)   MDrv_WriteByteMask( 0x1EAA, ((mux) << 2), BITMASK( 4:2 ) )
#define MDrv_Sys_SetUart1Mux(mux)   MDrv_WriteByteMask( 0x1EAA, ((mux) << 5), BITMASK( 7:5 ) )
#define MDrv_Sys_SetUart2Mux(mux)   MDrv_WriteByteMask( 0x1EAB, ((mux) << 0), BITMASK( 2:0 ) )

////////////////////////////////////////////////////////////////////////////////
//extern U32 u32SecbufAddr;
//extern U32 u32SecbufSize;

extern U32 u32OadbufMonitorAddr;
extern U32 u32OadbufMonitorSize;
extern U32 u32OadbufDownloadAddr;
extern U32 u32OadbufDownloadSize;

extern U32 u32SubtitleAddr;
extern U32 u32SubtitleSize;

DRVSYS_EX U8 u8ChipIdMajor;
DRVSYS_EX U8 u8ChipIdMinor;
DRVSYS_EX U8 u8ChipVer;
DRVSYS_EX U8 u8ChipRev;

DRVSYS_EX void MDrv_Sys_InitUartForXtalClk( char cOutputChar );
DRVSYS_EX bit  MDrv_Sys_IsCodeInSPI( void );
DRVSYS_EX void MDrv_Sys_RunCodeInMIU( U16 u16MIUAddrInBank, U8 u8NumBanks, BOOLEAN bBypassCache );
DRVSYS_EX void MDrv_Sys_RunCodeInSPI( void );
DRVSYS_EX void MDrv_Sys_WatchDogEnable(U8 mode);
DRVSYS_EX void MDrv_Sys_Set_ECC_DMA(U8 mode);
DRVSYS_EX void MDrv_Sys_Reboot( void );
DRVSYS_EX void MDrv_Sys_WholeChipReset( void );
DRVSYS_EX void MDrv_Sys_ResetAeon( BOOLEAN bReset );

// use "void *" as generic pointer type
DRVSYS_EX void MDrv_Sys_CopyMIU2VAR(U32 srcaddr, void *dstaddr, U16 len);
DRVSYS_EX void MDrv_Sys_CopyVAR2MIU(void *srcaddr, U32 dstaddr, U16 len);

DRVSYS_EX void MDrv_Sys_UartControl(BOOLEAN bEnable);
//DRVSYS_EX void AEON_StartAddress_Assignment(U8 Select, U32 CodeAddress, U32 DataAddress);


#if (XMODEM_DWNLD_ENABLE)
    DRVSYS_EX void MDrv_Sys_XmodemUartInit( void );
#endif

DRVSYS_EX void MDrv_Sys_Get_BinInfo(BININFO *pBinInfo, BOOLEAN *pbResult);
//DRVSYS_EX void MDrv_Sys_Set_CIDInfo(U8 *pCIDInfo);

//*****************************************************************************
#include "SysInit_Common.h"  //include common file for all chip
//*****************************************************************************


#undef DRVSYS_EX
#endif // _DRVSYS_H_
