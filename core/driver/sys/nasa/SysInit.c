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
///@file drvsys.h
///@brief System functions: Initialize, interrupt
///@author MStarSemi Inc.
///
///////////////////////////////////////////////////////////////////////////////

#define  _DRV_SYS_C_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Board.h"


#include "debug.h"
#include "hwreg.h"
#include "MsTypes.h"
#include "MsDevice.h"
//#include "msIR.h"
#include "mstar_debug.h"
#include "sysinfo.h"
#include "SysInit.h"
#if defined(__aeon__)
#include "risc32_spr.h"
#endif
#ifdef __AEONR2__
#include "r2_backtrace.h"
#endif

#include "util_symbol.h"

//#include "Panel.h"
#include "hal_misc.h"

#include "drvGPIO.h"
#include "drvUartDebug.h"
#include "drvMMIO.h"
#include "drvBDMA.h"
#include "drvMIU.h"
#include "drvWDT.h"
#include "drvGlobal.h"
#include "drvISR.h"
#include "drvIIC.h"
#include "drvGPIO.h"
#include "drvUART.h"
#include "drvPM.h"

#include "msAPI_Global.h"
#include "msAPI_Timer.h"

#ifdef ENABLE_MINI_DUMP
#include "msAPI_MSDCtrl.h"
#include "msAPI_FCtrl.h"
#include "imginfo.h"
#endif

//#include "MApp_SaveData.h"
#include "msAPI_MIU.h"


extern U8 DrvMemReadByte(U32 u32MemAddr );
extern void DrvMemWriteByte(U32 u32MemAddr, U8 u8Data );

//=========================================================================

static U8 u8OrigWdtTimer = 0x0A;    //Default 10 sec

/******************************************************************************/
/*                     Local Defines                                          */
/******************************************************************************/
#define PM_LOCK_SUPPORT             1   // 1: Enable 0: Disable
#if (PM_LOCK_SUPPORT == 1)
#include "drvSEM.h"
#endif

// set 1 to enable support for download image to dram and run from MSTV_Tool
#define DOWNLOAD_RUN_SERVER     0
#define DBG_SYS_PNT(x)   //x
//#define PIU_UART0_BAUDRATE          38400
/******************************************************************************/
/*                     Global Variables                                       */
/******************************************************************************/

// memory map for pre-built libraries
//U32 u32SecbufAddr = ((SECBUF_START_MEMORY_TYPE & MIU1) ? (SECBUF_START_ADR | MIU_INTERVAL) : (SECBUF_START_ADR));
//U32 u32SecbufSize = SECBUF_START_LEN;

U32 u32OadbufMonitorAddr = NULL;
U32 u32OadbufMonitorSize = NULL;
U32 u32OadbufDownloadAddr = NULL;
U32 u32OadbufDownloadSize = NULL;

U32 u32SubtitleAddr = ((SUBTITLE_MEMORY_TYPE & MIU1) ? (SUBTITLE_ADR | MIU_INTERVAL) : (SUBTITLE_ADR));
U32 u32SubtitleSize = SUBTITIE_OFFSET;

//U8 u8Sec1KFilterNum  = 8;
//U8 u8Sec4KFilterNum  = 8;
//U8 u8RecordFilterNum = 16;

//U32 u32Sec1KFilterBufferSize = 0x600;   //1.5K
//U32 u32Sec4KFilterBufferSize = 0x4000;  //16 K
//U32 u32Sec4KEitFilterBufferSize = 0x4000; //16K for eit schedule

//U32 u32CRCbufAddr = NULL;


unsigned long RIU_MAP;


#define u8ChipIdMajor   REG8(REG_CHIP_ID_MAJOR)
#define u8ChipIdMinor   REG8(REG_CHIP_ID_MINOR)
#define u8ChipVer       REG8(REG_CHIP_VERSION)
#define u8ChipRev       REG8(REG_CHIP_REVISION)

/******************************************************************************/
#include "SysInit_Common.c"  //include common file for all chip

/******************************************************************************/
void MDrv_Sys_InitUartForXtalClk( char cOutputChar )
{
    UNUSED(cOutputChar);
    MDrv_WriteRegBit(0x1E03, ENABLE, BIT2);          // enable UART RX
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_Sys_Get_BinInfo()
/// @brief \b Function \b Description : Get Specific Bin information
/// @param <IN>        \b None        :
/// @param <OUT>       \b pBinInfo    : Get Bin Information
/// @param <RET>       \b BOOLEAN     : Success or Fail
/// @param <GLOBAL>    \b None        :
////////////////////////////////////////////////////////////////////////////////
void MDrv_Sys_Get_BinInfo(BININFO *pBinInfo, BOOLEAN *pbResult)
{
    BININFO NewBinInfo;

    // Added by coverity_0505
    memset(&NewBinInfo, 0, sizeof(BININFO));

    NewBinInfo.B_ID = pBinInfo->B_ID;
    *pbResult = Get_BinInfo(&NewBinInfo);
    pBinInfo->B_FAddr = NewBinInfo.B_FAddr;
    pBinInfo->B_Len   = NewBinInfo.B_Len;
    pBinInfo->B_IsComp = NewBinInfo.B_IsComp;
}

/******************************************************************************/
/// DO NOT MODIFY THIS FUNCTION, IT IS VERY DANGEROUS! (called by vectors.S)
/******************************************************************************/

#define STACK_SIZE      16384

//---------------------------------------------------------
//---------------------------------------------------------

void console_init(void)
{
    //bit 12 UART Enable, bit 11 UART RX Enable
    *(volatile MS_U32*)(0xA0000000+(0x0709*4)) |= 0x0800;

    // Clear UART_SEL
    *(volatile MS_U32*)(0xA0200000+(0x0F53*4)) = 0xFFFF;
    *(volatile MS_U32*)(0xA0200000+(0x0F54*4)) = 0x000F;

    //enable reg_uart_rx_enable;
    *(volatile MS_U32*)(0xA0200000+(0x0F01*4)) |= (1<<10);

    {
        MDrv_UART_Init(E_UART_AEON_R2, 115200);
        mdrv_uart_connect(E_UART_PORT0,E_UART_AEON_R2);
    }

  #if ENABLE_UART2
    {
        mdrv_uart_connect(E_UART_PORT3,E_UART_PIU_UART1);
    }
 #endif
}

void MDrv_Timer_Init(void) // Now support mode 1
{
    gTimerCount0 = 0;
    gTimerDownCount0 = 0;
    gTimerDownCount1 = 0;
    gu8100msTH = 0;
    g_u16ElapsedTimeSecond = 0;
}

//extern unsigned char MDrv_IRQ_Init(void);
extern U32 __heap;
extern U32 __heap_end;

#ifdef __aeon__
void mhal_interrupt_unmask(MHAL_INTERRUPT_TYPE intr_num);
#endif

extern void MDrv_Timer_ISR_Register(void);
extern void MDrv_Timer_Setting_Register(void);




void start(void)
{
    U32 u32MiuProtectAlign;
    U32 u32Start;
    U32 u32End;
    U32 u32MiuProtectAddr_Start, u32MiuProtectAddr_End;
    // Added by coverity_0210
    U32 u32Tmp;
    extern U8  _readonly_start[];
    extern U8  _readonly_end[];

    //extern void uart_init(U32 aeon_clock, U32 uart_clock);    //T3 FPGA test
    extern int  main(void);
    extern void MDrv_Pad_Init(void);
    extern void MDrv_ISR_Init(void);
    extern void mhal_stack_init(void *stack);

    char *stack;
    // Setup stack

    stack = (char *)malloc(STACK_SIZE);
    if (stack)
    {
        stack += STACK_SIZE;
        mhal_stack_init(stack);
    }
    else
    {
        ASSERT(0);
    }

#if (!BLOADER)
    MDrv_WDT_Init(E_WDT_DBGLV_NONE);
#endif

    MDrv_MMIO_GetBASE((MS_U32 *) &RIU_MAP, &u32Tmp, MS_MODULE_PM);

#if (PM_LOCK_SUPPORT == 1)
    MDrv_SEM_Init();
#endif

    MDrv_MIU_SetIOMapBase();




#if 0//(MEMORY_MAP <= MMAP_32MB)
        {
            // Set all invalid after 32M
            u32Start = 0x2000000; // 32M
            u32End = 0x4000000; // 64M
            MDrv_MIU_Protect(3, zero_array, ALIGN(u32Start + ((1<<pageshift) - 1), pageshift), ALIGN(u32End, pageshift), ENABLE);
        }
#endif

    Util_InitSymbolTBL();

    MDrv_Pad_Init();
    mdrv_gpio_init();

    //RIU[0x101EA6] = 0x0001;
    //RIU[0x101E02] |= BIT10;
    //RIU[0x0e12] &= ~BIT12;

#if 1//(!ENABLE_UART_INIT)
    console_init();
#endif

{
#define PROTECT_BEON_FULL_RANGE   0

#if(PROTECT_BEON_FULL_RANGE)
   U8 au8MiuProtectIds[16] = {MIU_CLIENT_R2M_W, MIU_CLIENT_BDMA_RW, MIU_CLIENT_USB_UHC0_RW,0};
#else
   U8 au8MiuProtectIds[16] = {0};
#endif

#if(PROTECT_BEON_FULL_RANGE)
    u32Start = BEON_MEM_ADR;
    u32End = BEON_MEM_ADR + BEON_MEM_LEN;
#else
    u32Start = MsOS_VA2PA((U32)_readonly_start);
    u32End = MsOS_VA2PA((U32)_readonly_end);
#endif

    MDrv_MIU_ProtectAlign(&u32MiuProtectAlign);

    u32MiuProtectAddr_Start = ALIGN(u32Start + ((1<<u32MiuProtectAlign) - 1), u32MiuProtectAlign);
    u32MiuProtectAddr_End = ALIGN(u32End, u32MiuProtectAlign);

//u32MiuProtectAddr_Start = 0x5DA000;
//u32MiuProtectAddr_End = 0x5DB000;

    printf("  _readonly_start=0x%X, _readonly_end=0x%X\n", _readonly_start, _readonly_end);
    //printf("MIU protect align=%X\n", 1ul << pageshift);
    printf("MIU protect align=0x%X, 0x%X\n", u32MiuProtectAlign, 1ul << u32MiuProtectAlign);
    printf("Beon protect start=%X, end=%X\n", u32MiuProtectAddr_Start, u32MiuProtectAddr_End );

    //MDrv_MIU_Protect(0, zero_array, ALIGN(u32Start + ((1<<pageshift) - 1), pageshift), ALIGN(u32End, pageshift), ENABLE);
    MDrv_MIU_Protect(0, au8MiuProtectIds, u32MiuProtectAddr_Start, u32MiuProtectAddr_End, ENABLE);
}

    //Print_R2_SPR_PICMR(__LINE__, __FUNCTION__);

    static PM_RtcParam PmRtcParam =
    {
        .u8PmRtcIndex = 0,
        .u32RtcCtrlWord = 12000000ul,
        .u32RtcSetMatchCounter = 0,
        .u32RtcGetMatchCounter = 0,
        .u32RtcSetCounter = 0,
        .u32RtcGetCounter = 0
    };

    MDrv_PM_RtcInit((PM_RtcParam*)_VA2PA((MS_U32)&PmRtcParam));

#if 0//(ENABLE_AUTOTEST || ENABLE_BOOTTIME)
    {
        //for auto test recording booting time from Sboot to Ck3
        //U32 PiuTick = MDrv_Read2Byte(0x3048);
        //PiuTick |= (MDrv_Read2Byte(0x304a) << 16);
        U32 PiuTick = MHAL_PIU_TIMER_Get_CurTick(PIU_TIMER_1);
        //printf("PiuTick= 0x%X\n", PiuTick);
        U32 PiuTime = ((float)PiuTick / 12000000ul)*1000 ;  //ms
        PRINT_AUTOTEST_TIME("start sysinit", PiuTime);
        INIT_AUTOTEST_CHECKPT(PiuTime);
    }
#endif

    MDrv_SysInit_CheckSbootUseTime();

    printf("\nHello Nasa:%lx\n",RIU_MAP);
    printf("  stack=0x%X ~ 0x%X\n", (U32)stack-STACK_SIZE, (U32)stack);

    printf("  heap => %x ~ %x \n",&__heap, &__heap_end);
    if ((&__heap_end) <= (&__heap))
    {
        printf("## ERROR!! MMAP lyout for CPU code is not enough!!!\n");
    }
    else if (((U32)&__heap_end - (U32)&__heap) < 0x4B000)
    {
        U32 u32FreeSize;
        u32FreeSize = (U32)&__heap_end - (U32)&__heap;
        printf("## heap size is %dK under 300K!!\n", (u32FreeSize>>10));
    }

    if (((U32)_readonly_start & 0xFFF) || ((U32)_readonly_end & 0xFFF))
        printf("\r\n## ERROR!! ReadOnly area is not alignment 4K\n");


#if 0//( defined(DMD_DTMB_LEN)&&(DMD_DTMB_LEN>0) )
    { // Protect DMD_DTMB_ADR
        U8 au8ProtectIds_DMD[4] = {
            MIU_CLIENT_DEMOD_W,
            MIU_CLIENT_DEMOD_R,
            MIU_CLIENT_DEMOD_ADCDMA_W,
            0 };
        U32 u32Protect_Start, u32Protect_End;
        MIU_PortectInfo stMiuProtectInfo;


        // Set all invalid after 32M
        u32Start = DMD_DTMB_ADR;//0x2000000;
        u32End = DMD_DTMB_ADR + DMD_DTMB_LEN; // 64M

        printf("1. u32Start=0x%X, u32End=0x%X\n", u32Start, u32End);

        u32Protect_Start = ALIGN(u32Start + ((1<<pageshift) - 1), pageshift);
        u32Protect_End = ALIGN(u32End, pageshift);

        printf("2. u32Protect_Start=0x%X, u32Protect_End=0x%X\n", u32Protect_Start, u32Protect_End);


        // Write memory
        DrvMemWriteByte(u32Protect_Start, 0x55);
        printf("*%X=%X\n", u32Protect_Start, DrvMemReadByte(u32Protect_Start));
        DrvMemWriteByte(u32Protect_Start, 0x66);
        printf("*%X=%X\n", u32Protect_Start,DrvMemReadByte(u32Protect_Start));


        // Check miu protect status
        //if( MDrv_MIU_GetProtectInfo(0, &stMiuProtectInfo) )
        MDrv_MIU_Get_ProtectHitLog(&stMiuProtectInfo);
        {
            printf(" Hit = %X\n", stMiuProtectInfo.bHit);
            if(stMiuProtectInfo.bHit)
            {
                printf(" u8Block = %X\n", stMiuProtectInfo.u8Block);
                printf(" u8Group = %X,", stMiuProtectInfo.u8Group);
                printf(" u8ClientID = %X\n", stMiuProtectInfo.u8ClientID);
            }
        }

        //MDrv_MIU_Protect(3, au8ProtectIds_DMD, u32Protect_Start, u32Protect_End, ENABLE);
        {
            // Group 3
            MDrv_WriteByte(0x1012D6, 0x2C); // reg_protect3_id0
            MDrv_WriteByte(0x1012D7, 0x2D); // reg_protect3_id1

            MDrv_Write2Byte(0x1012D8, u32Protect_Start>>12); // addr start
            MDrv_Write2Byte(0x1012DA, (u32Protect_End-1)>>12); // addr end

            MDrv_WriteRegBit(0x1012C0, ENABLE, BIT3);
        }

        // Write memory
        DrvMemWriteByte(u32Protect_Start, 0x77);
        printf("*%X=%X\n", u32Protect_Start, DrvMemReadByte(u32Protect_Start));

        // Clear log
        MDrv_MIU_Clr_ProtectHitLog();


        // Check miu protect status
        //if( MDrv_MIU_GetProtectInfo(0, &stMiuProtectInfo) )
        MDrv_MIU_Get_ProtectHitLog(&stMiuProtectInfo);
        {
            printf(" Hit = %X\n", stMiuProtectInfo.bHit);
            if(stMiuProtectInfo.bHit)
            {
                printf(" u8Block = %X\n", stMiuProtectInfo.u8Block);
                printf(" u8Group = %X,", stMiuProtectInfo.u8Group);
                printf(" u8ClientID = %X\n", stMiuProtectInfo.u8ClientID);
            }
        }

    }
#endif


    MDrv_IIC_Init();

    //if ( ENABLE_POWER_GOOD_DETECT )
    //{
    //    MDrv_WriteRegBit(0x1EE4, ENABLE, 0x40);
    //    MDrv_WriteRegBit(0x1EE4, ENABLE, 0x1F);
    //}

    if (DOWNLOAD_RUN_SERVER)
    {
        //mhal_interrupt_disable();
        MsOS_CPU_DisableInterrupt();
        putchar( 'F' );
        putchar( 'W' );
        while (1)
        {
            MDrv_Sys_ClearWatchDog();
        }
    }
    else
    {
        if (WATCH_DOG == ENABLE)
        {
            MDrv_Sys_ClearWatchDog();
        }
        else
        {
            MDrv_Sys_DisableWatchDog();
        }
    }


    {
        extern void ProcessSysTrap( MHAL_SavedRegisters *pHalReg, U32 vector );
        extern void ProcessEx0Isr( MHAL_SavedRegisters *pHalReg, U32 vector );
        extern void ProcessEx1Isr( MHAL_SavedRegisters *pHalReg, U32 vector );

        //mhal_exception_attach( E_EXCEPTION_TRAP, ProcessSysTrap, E_EXCEPTION_TRAP );
        MsOS_CPU_AttachException( E_EXCEPTION_TRAP, ProcessSysTrap, E_EXCEPTION_TRAP );

    #if 1//( ENABLE_R2_BACKTRACE )
        MsOS_CPU_AttachException( E_EXCEPTION_BUS_ERROR, Aeon_Exception_Handler, E_EXCEPTION_BUS_ERROR );
        MsOS_CPU_AttachException( E_EXCEPTION_UNALIGNED_ACCESS, Aeon_Exception_Handler, E_EXCEPTION_UNALIGNED_ACCESS );
        MsOS_CPU_AttachException( E_EXCEPTION_ILLEGAL_INSTRUCTION, Aeon_Exception_Handler, E_EXCEPTION_ILLEGAL_INSTRUCTION );
    #endif

        MsOS_Init();
        MDrv_ISR_Init();
    }

    MDrv_Timer_ISR_Register();
    MDrv_Timer_Init();

    // Aeon uart exception default is on, must disable it first
#ifdef __aeon__
    MsOS_CPU_MaskInterrupt(E_INTERRUPT_UART);
#endif

    MsOS_CPU_EnableInterrupt();

#ifdef __aeon__
    mhal_interrupt_unmask((MHAL_INTERRUPT_TYPE)3);
#endif


    //DrvMemWriteByte(0x278000, 0x55);

#if( ENABLE_API_MIU_PROTECT )
    // Check MIU protect info...
    {
        MsOS_Dcache_Flush(0, 0x4000000); // 0 ~ 64M
        MsOS_FlushMemory();
        MsOS_DelayTask(10);

        CHECK_MIU_PROTECT_LOG();
    }
#endif

    // jump to main() should never return
    PRINT_AUTOTEST_CHECKPT_TIME("start main");
    main();

    while(1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_Sys_Set_CIDInfo()
/// @brief \b Function \b Description : To Set Customer Info
/// @param <IN>        \b pCIDInfo    : Input the CID Info
/// @param <OUT>       \b None        :
/// @param <GLOBAL>    \b None        :
////////////////////////////////////////////////////////////////////////////////
void MDrv_Sys_Set_CIDInfo(U8 *pCIDInfo)
{
    MDrv_WriteByte(0x1033D0,pCIDInfo[0]);
    MDrv_WriteByte(0x1033D1,pCIDInfo[1]);
    MDrv_WriteByte(0x1033D2,pCIDInfo[2]);
    MDrv_WriteByte(0x1033D3,pCIDInfo[3]);

}

/******************************************************************************/
/// Copy variable from DRAM space to XDATA space
/// @param pAddr \b IN XDATA pointer
/// @param u32MiuAddr \b IN MIU address
/// @param u16Length \b IN size to copy
/******************************************************************************/

void MDrv_Sys_CopyMIU2VAR(U32 srcaddr, void *dstaddr, U16 len)
{
    memcpy(dstaddr, (U8 *)srcaddr, len);
}

// for backward compatible, do not use!
/*
#undef MDrv_MIU_SDRAM2VARCopy
void MDrv_MIU_SDRAM2VARCopy(U32 srcaddr, void *dstaddr, U16 len)
{
    memcpy(dstaddr, (U8 *)srcaddr, len);
}
*/

/******************************************************************************/
/// Copy variable from XDATA space to DRAM space
/// @param pAddr \b IN XDATA pointer
/// @param u32MiuAddr \b IN MIU address
/// @param u16Length \b IN size to copy
/******************************************************************************/

void MDrv_Sys_CopyVAR2MIU(void *srcaddr, U32 dstaddr, U16 len)
{
    memcpy((U8 *)dstaddr, srcaddr, len);
}

// for backward compatible, do not use!
/*
#undef MDrv_MIU_VAR2SDRAMCopy
void MDrv_MIU_VAR2SDRAMCopy(void *srcaddr, U32 dstaddr, U16 len)
{
    memcpy((U8 *)dstaddr, srcaddr, len);
}
*/

/******************************************************************************/
//
//   128T +------+          +------+ 128T
//     |  |      |          |      |  |
//     +--+ 8051 |          | Aeon +--+
//        |      |          |      |
//        +--+---+          +---+--+
//       +---+----+        +----+---+
//       |        |        |        |
//   boot_aeon   sw_rst   sw_rst  boot_8051
//
// When boot from 8051
//      reboot:   use reg_reset_cpu0 with password
//      run aeon: set reg_sw_reset_cpu1 to 1
// When boot from Aeon
//      reboot:   use reg_reset_cpu1 with password
//      run 8051: set reg_sw_reset_cpu0 to 1
/******************************************************************************/
void MDrv_Sys_WholeChipReset( void )
{
    Panel_Backlight_VCC_OFF();
    MDrv_WriteByte( RIUBASE_PM_MISC+0x5C, 0xFF );
    MDrv_Write4Byte(0xEA0, 0x51685168);
    MDrv_WriteRegBit(RIUBASE_PM_MISC+0x52, 1, BIT7);
    MDrv_WriteRegBit(RIUBASE_PM_MISC+0x52, 1, BIT6);
    MDrv_WriteByte( RIUBASE_PM_MISC+0x5C, 0x79 );
    MDrv_Sys_StopCpu(1);
}

/******************************************************************************/
///Reset 8051 to reboot
/******************************************************************************/
void MDrv_Sys_Reboot( void )
{
    MDrv_Write2Byte( REG_RESET_CPU_8051, 0x029F );
    MDrv_Write2Byte( REG_RESET_CPU_8051, 0x829F ); // can't ignore second 0x9F
    MDrv_Sys_StopCpu(1);
}


void MDrv_Sys_SetWatchDogTimer( U8 sec )
{
     if( sec == 0 )
     {
        u8OrigWdtTimer = 1;
     }
     else
     {
        u8OrigWdtTimer = sec;
     }
}

void  MDrv_Sys_EnableWatchDog(void)
{
#if (WATCH_DOG == ENABLE )
    MDrv_Write4Byte(WDT_TIMER_0, ( MDrv_Sys_WatchDogCycles( u8OrigWdtTimer ) ));
#endif
}

//----------------------------------------------------------------
void MDrv_Sys_DisableWatchDog( void )
{
    MDrv_Write4Byte( WDT_TIMER_0, 0x0000UL );
}


