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
#include "sysinfo.h"
#include "hwreg.h"
#include "drvGPIO.h"
#include "drvUartDebug.h"
#include "MsTypes.h"
#include "MsDevice.h"
#include "drvMMIO.h"
#include "drvBDMA.h"
#include "drvMIU.h"
#include "SysInit.h"
#include "drvGlobal.h"
#include "drvISR.h"
#include "drvIIC.h"
#include "drvGPIO.h"
#include "Board.h"
#include "debug.h"
#include "drvUART.h"
#include "drvPM.h"
#include "msIR.h"
#include "drvWDT.h"
#include "hal_misc.h"


#ifdef ENABLE_MINI_DUMP
#include "msAPI_MSDCtrl.h"
#include "msAPI_FCtrl.h"
#include "imginfo.h"
#endif
#include "util_symbol.h"

#if defined(__aeon__)
#include "risc32_spr.h"
#endif

#include "msAPI_MIU.h"
#include "msAPI_Global.h"
#include "msAPI_Timer.h"


static U8 u8OrigWdtTimer = 0x0A;    //Default 10 sec

/******************************************************************************/
/*                     Local Defines                                          */
/******************************************************************************/
#define PM_LOCK_SUPPORT             1   // 1: Enable 0: Disable
#if (PM_LOCK_SUPPORT == 1)
#include "drvSEM.h"
#endif

// set 1 to enable support for download image to dram and run from MSTV_Tool
//#define DOWNLOAD_RUN_SERVER     0
#define DBG_SYS_PNT(x)   //x
#define PIU_UART0_BAUDRATE          38400
/******************************************************************************/
/*                     Global Variables                                       */
/******************************************************************************/

// memory map for pre-built libraries
//U32 u32SecbufAddr = ((SECBUF_START_MEMORY_TYPE & MIU1) ? (SECBUF_START_ADR | MIU_INTERVAL) : (SECBUF_START_ADR));
//U32 u32SecbufSize = SECBUF_START_LEN;

U32 u32OadbufMonitorAddr = 0;
U32 u32OadbufMonitorSize = 0;
U32 u32OadbufDownloadAddr = 0;
U32 u32OadbufDownloadSize = 0;

//U32 u32SubtitleAddr = ((SUBTITLE_MEMORY_TYPE & MIU1) ? (SUBTITLE_ADR | MIU_INTERVAL) : (SUBTITLE_ADR));
//U32 u32SubtitleSize = SUBTITIE_OFFSET;

//U8 u8Sec1KFilterNum  = 8;
//U8 u8Sec4KFilterNum  = 8;
//U8 u8RecordFilterNum = 16;

//U32 u32Sec1KFilterBufferSize = 0x600;   //1.5K
//U32 u32Sec4KFilterBufferSize = 0x4000;  //16 K
//U32 u32Sec4KEitFilterBufferSize = 0x4000; //16K for eit schedule

//U32 u32CRCbufAddr = NULL;

#ifndef MSOS_TYPE_LINUX
unsigned long RIU_MAP;
#endif

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

/*  //T3 fpga : there is no PM MCU in T3
static U8 code PmCode[] = {
    #include "drvPmCode.h"
};*/

/******************************************************************************/
/// DO NOT MODIFY THIS FUNCTION, IT IS VERY DANGEROUS! (called by vectors.S)
/******************************************************************************/

#define STACK_SIZE      16384

//---------------------------------------------------------
//---------------------------------------------------------
#ifndef MSOS_TYPE_LINUX
static void console_init(void)
{
#if defined(__aeon__)
    MDrv_UART_Init(E_UART_AEON, UART0_BAUDRATE);
#endif

#if defined(MIPS_CHAKRA)
    //bit 12 UART Enable, bit 11 UART RX Enable
    *(volatile MS_U32*)(0xbf000000+(0x0709*4)) |= 0x0800;

	// Clear All UART sel (Different Sel can not use the same source!!!)
    *(volatile MS_U32*)(0xbf200000+(0x0F53*4)) = 0xFFFF;
	*(volatile MS_U32*)(0xbf200000+(0x0F54*4)) = 0x000F;

#if 0 // UART1_ENABLE
    MDrv_UART_Init(E_UART_PIU_UART1, 115200);
#endif

    // Switch UART0 to PIU UART 0
    MDrv_UART_Init(E_UART_PIU_UART0, 115200);
	mdrv_uart_connect(E_UART_PORT0, E_UART_PIU_UART0);


 #if (ENABLE_UART2)
    MS_U32 uart2;
    uart2 = mdrv_uart_open(E_UART_PIU_UART1);
    mdrv_uart_set_baudrate(uart2,115200);
    mdrv_uart_connect(E_UART_PORT2, E_UART_PIU_UART1);
 #endif

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
#endif

extern unsigned char MDrv_IRQ_Init(void);


extern U32 __bss_start;
extern U32 __bss_end;

extern U32 __stack_begin;

extern U32 __heap;
extern U32 __heap_end;

#ifdef __aeon__
void mhal_interrupt_unmask(MHAL_INTERRUPT_TYPE intr_num);
#endif


#ifndef MSOS_TYPE_LINUX
extern void MDrv_Timer_ISR_Register(void);
//extern void MDrv_Timer_Setting_Register(void);

#if( STR_ENABLE )

void sbrk_Reset(void);
unsigned char * sbrk_Get_TOP(void);
void dlmalloc_Reset(void);

void SysInit_Test_Malloc(void)
{
    //PRINT_CURRENT_LINE();

    //Print_Buffer( (U8*)0x80fc6400, 0x80);
    //printf("sbrk_Get_TOP()=0x%X\n", (U32)sbrk_Get_TOP());

    U8* pu8Tmp;
    pu8Tmp = (U8*)malloc(0x10);
    printf("pu8Tmp=0x%X\n", (U32)pu8Tmp);
    free(pu8Tmp);

#if 0
    pu8Tmp = (U8*)malloc(0x20);
    printf("pu8Tmp=0x%X\n", (U32)pu8Tmp);

    printf("sbrk_Get_TOP()=0x%X\n", (U32)sbrk_Get_TOP());
    //sbrk_Reset();
    dlmalloc_Reset();
    pu8Tmp = (U8*)malloc(0x20);
    printf("pu8Tmp=0x%X\n", (U32)pu8Tmp);

    printf("sbrk_Get_TOP()=0x%X\n", (U32)sbrk_Get_TOP());
#endif
}
#endif

void start(void)
{
    U32 u32MiuProtectAlign;
    U32 u32Start;
    U32 u32End;


#define PROTECT_BEON_FULL_RANGE   1

#if defined(MIPS_CHAKRA)
    // Added by coverity_0210
 #if(PROTECT_BEON_FULL_RANGE)
    U8 au8MiuProtectIds[16] = {MIU_CLIENT_MIPS_RW, MIU_CLIENT_BDMA_RW,MIU_CLIENT_USB_UHC0_RW,MIU_CLIENT_USB_UHC1_RW, 0};
 #else
    U8 au8MiuProtectIds[16] = {0};
 #endif

    U32 u32Tmp;
    U32 u32MiuProtectAddr_Start, u32MiuProtectAddr_End;

    extern U8  _readonly_start[];
    extern U8  _readonly_end[];

    //extern void uart_init(U32 aeon_clock, U32 uart_clock);    //T3 FPGA test
    extern int  main(void);
    extern void MDrv_Pad_Init(void);
    extern void MDrv_ISR_Init(void);
    //extern void mhal_stack_init(void *stack);


#if( STR_ENABLE )
    // Reset malloc engine for DC on
    dlmalloc_Reset();
#endif

    MDrv_MMIO_GetBASE((MS_U32 *) &RIU_MAP, &u32Tmp, MS_MODULE_PM);

#if (PM_LOCK_SUPPORT == 1)
    MDrv_SEM_Init();
#endif

    MDrv_MIU_Init();

    MDrv_MIU_SetIOMapBase();

    // Modified by coverity_0210
    MDrv_MIU_ProtectAlign(&u32MiuProtectAlign);

  #if(PROTECT_BEON_FULL_RANGE)
    u32Start = BEON_MEM_ADR;
    u32End = BEON_MEM_ADR + BEON_MEM_LEN;
  #else
    //u32Start = MsOS_VA2PA((U32)_readonly_start);
    u32Start = BEON_MEM_ADR;
    u32End = MsOS_VA2PA((U32)_readonly_end);
  #endif

    u32MiuProtectAddr_Start = ALIGN(u32Start + ((1<<u32MiuProtectAlign) - 1), u32MiuProtectAlign);
    u32MiuProtectAddr_End = ALIGN(u32End, u32MiuProtectAlign);

    MDrv_MIU_Protect(MIU_PROTECT_IDX_0, au8MiuProtectIds, u32MiuProtectAddr_Start, u32MiuProtectAddr_End, ENABLE);

    MsOS_CPU_SetEBASE(BEON_MEM_ADR);

    //Client Select code from MDrv_ClientSelectToMIUx
    MDrv_Write2Byte( ( RIUBASE_MIU+0xf0 ), ClientGruop0_in_MIU1 );
    MDrv_Write2Byte( ( RIUBASE_MIU+0xf2 ), ClientGruop1_in_MIU1 );
    MDrv_Write2Byte( ( RIUBASE_MIU+0xf4 ), ClientGruop2_in_MIU1 );
    MDrv_Write2Byte( ( RIUBASE_MIU+0xf6 ), ClientGruop3_in_MIU1 );

    MDrv_Write2Byte( ( RIUBASE_MIU2+0xf0 ), ClientGruop0_in_IP  );
    MDrv_Write2Byte( ( RIUBASE_MIU2+0xf2 ), ClientGruop1_in_IP  );
    MDrv_Write2Byte( ( RIUBASE_MIU2+0xf4 ), ClientGruop2_in_IP  );
    MDrv_Write2Byte( ( RIUBASE_MIU2+0xf6 ), ClientGruop3_in_IP  );

    MDrv_Pad_Init();

    mdrv_gpio_init();

    console_init();

    //printf(ANSI_COLOR_YELLOW);
    // Change Uart color
    msDebug_ANSI_SetColorText(E_FONT_COLOR_BLACK);
    msDebug_ANSI_SetColorText(E_FONT_BACKGROUND_COLOR_YELLOW);

    MDrv_SysInit_CheckSbootUseTime();

    printf("\nHello Marlon: RIU=0x%x\n",RIU_MAP );
    printf("%s\n",__DATE__);
    printf("%s\n\n",__TIME__);

    //printf(ANSI_COLOR_RESET);

    Print_BeonMemory( (U32)(&__stack_begin), STACK_SIZE );

    printf("_readonly_start = 0x%x, _readonly_end = 0x%x\n", _readonly_start, _readonly_end );

    printf("Miu protect align = %u\n", u32MiuProtectAlign);
    printf("Miu protect: 0x%X ~ 0x%X\n", u32Start, u32End);
    printf("Miu protect-align: 0x%X ~ 0x%X\n", u32MiuProtectAddr_Start, u32MiuProtectAddr_End );

#if( STR_ENABLE )
    SysInit_Test_Malloc();
#endif

//printf("@@ %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
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

    printf("\n");

#if 0
    {
        U32 u32StackEnd = (U32)(&__stack_begin) - 16384;
        U32 u32FreeSize;

       printf("__bss_start = 0x%x, __bss_end = 0x%x\n", &__bss_start, &__bss_end );
       printf("u32StackEnd = 0x%x, __stack_begin = 0x%x\n", u32StackEnd, &__stack_begin );
       printf("ptr1= 0x%x\n", (U32)(&u32FreeSize) );

       u32FreeSize = ((unsigned int)&__heap_end) - ((unsigned int)&__heap);

       printf("heap_start = 0x%x, heap_end = 0x%x\n", &__heap, &__heap_end );
       printf("heap_size => %u(0x%x)\n", u32FreeSize, u32FreeSize );

       if ((&__heap_end) <= (&__heap))
       {
           printf("## ERROR!! MMAP lyout for CPU code is not enough!!!\n");
       }
       else if (u32FreeSize < 0x4B000)
       {
           printf("## heap size is %dK under 300K!!\n", (int)u32FreeSize/1024);
       }
    }
#endif

    {
#define CPU_CLOCK_REG       0x10C26
//#define MIPS_CLOCK_FREQ     (RegReadByte(NON_PM_RIU_BASE, CPU_CLOCK_REG)*12)*1000000

        //printf("CPU = %u\n", MDrv_ReadByte(0x10C26));
        //printf("CPU = %u\n", MDrv_ReadByte(0x10C4C));
    }

    // Check MIU protect status
    CHECK_MIU_PROTECT_LOG();

#if 0 // Test MAD buffer
    {
        U8 au8ProtectIds_3[4] = {0};


        msAPI_MIU_Set_MiuProtect(MIU_PROTECT_IDX_3,
            au8ProtectIds_3, MAD_BASE_BUFFER_2_ADR,
            MAD_BASE_BUFFER_2_ADR + MAD_BASE_BUFFER_2_LEN,
            ENABLE);
    }
#endif

#if 0//(!BLOADER)
    {
        //for auto test recording booting time from Sboot to Ck3
        //U32 PiuTick = MDrv_Read2Byte(0x3048);
        //PiuTick |= (MDrv_Read2Byte(0x304a) << 16);
        //printf("PiuTick= 0x%X\n", PiuTick);

        U32 PiuTick = MHAL_PIU_TIMER_Get_CurTick(PIU_TIMER_1);
        //printf("PiuTick= 0x%X\n", PiuTick);

        U32 PiuTime = ((float)PiuTick / 12000000ul)*1000 ;  //ms
    #if (ENABLE_AUTOTEST || ENABLE_BOOTTIME)
        PRINT_AUTOTEST_TIME("start sysinit", PiuTime);
        INIT_AUTOTEST_CHECKPT(PiuTime);
    #endif
        printf("sboot use %u\n", PiuTime);
    }
#endif

    msDebug_ANSI_AllAttrOffText();


    MDrv_IIC_Init();

#if (!BLOADER)
    MDrv_WDT_Init(E_WDT_DBGLV_NONE);
#endif

    /* if (DOWNLOAD_RUN_SERVER)
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
    else */
    {
    #if(WATCH_DOG == ENABLE)
        {
            MDrv_Sys_ClearWatchDog();
        }
    #else
        {
            MDrv_Sys_DisableWatchDog();
        }
    #endif
    }


    {
        extern void ProcessSysTrap( MHAL_SavedRegisters *pHalReg, U32 vector );
        extern void ProcessEx0Isr( MHAL_SavedRegisters *pHalReg, U32 vector );
        extern void ProcessEx1Isr( MHAL_SavedRegisters *pHalReg, U32 vector );
        extern void ProcessTimer0Isr( MHAL_SavedRegisters *pHalReg, U32 vector );

        //MsOS_CPU_AttachException( E_EXCEPTION_TRAP, ProcessSysTrap, E_EXCEPTION_TRAP );
        //printf("E_EXCEPTION_DIV_BY_ZERO=%u\n", E_EXCEPTION_DIV_BY_ZERO);
        //MsOS_CPU_AttachException( E_EXCEPTION_RESERVED_13, mips_Exception_Handler, E_EXCEPTION_RESERVED_13 );
        //MsOS_CPU_AttachException( E_EXCEPTION_DIV_BY_ZERO, mips_Exception_Handler, E_EXCEPTION_DIV_BY_ZERO );

        MsOS_CPU_AttachInterrupt( E_INTERRUPT_TICK_TIMER, ProcessTimer0Isr, 0 );

        MsOS_Init();
        MDrv_ISR_Init();
    }

    MDrv_Timer_ISR_Register();
    MDrv_Timer_Init();


#if(ENABLE_STACK_CHECK)
    MDrv_StackChk_Init((U32)(&__stack_begin), STACK_SIZE);
    //MDrv_StackChk_PrintInfo();
    //MDrv_StackChk_CheckFlag();
#endif


    // Aeon uart exception default is on, must disable it first
#ifdef __aeon__
    MsOS_CPU_MaskInterrupt(E_INTERRUPT_UART);
#endif

    MsOS_CPU_EnableInterrupt();

#ifdef __aeon__
    mhal_interrupt_unmask((MHAL_INTERRUPT_TYPE)3);
#endif


    // jump to main() should never return
    printf("Enter main()\n");
    main();
    while(1);
#endif

}
#endif
/*
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
*/
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
#undef MDrv_MIU_SDRAM2VARCopy
void MDrv_MIU_SDRAM2VARCopy(U32 srcaddr, void *dstaddr, U16 len)
{
    memcpy(dstaddr, (U8 *)srcaddr, len);
}

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
#undef MDrv_MIU_VAR2SDRAMCopy
void MDrv_MIU_VAR2SDRAMCopy(void *srcaddr, U32 dstaddr, U16 len)
{
    memcpy((U8 *)dstaddr, srcaddr, len);
}

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
    MDrv_WriteByte(0x2E5C, 0xFF );

    MDrv_Write4Byte(0xEA0, 0x51685168);
    MDrv_WriteRegBit(0x2E52, 1, BIT7);
    MDrv_WriteRegBit(0x2E52, 1, BIT6);

    MDrv_WriteByte( 0x2E5C, 0x79 );
    MDrv_Sys_StopCpu(1);


}

/******************************************************************************/
///Reset 8051 to reboot
/******************************************************************************/
void MDrv_Sys_Reboot( void )
{
    // write password to self reset
    MDrv_Write2Byte( REG_RESET_CPU_8051, 0x029F );
    MDrv_Write2Byte( REG_RESET_CPU_8051, 0x829F ); // can't ignore second 0x9F
    MDrv_Sys_StopCpu(1);
}

#if 0//maco
//========================================
// x = 0 for aeon0, x = 1 for aeon1
// start address are 3 bytes
void AEON_StartAddress_Assignment(U8 Select, U32 CodeAddress, U32 DataAddress)
{
    U32 Codetemp;
    U32 Datatemp;

    if(Select>2)
        printf("AEON Error Selection\n");

    Codetemp = CodeAddress / 0x10;  // code unit is 16-bit
    Datatemp = DataAddress / 0x08;  // data unit is 8-bit

    switch(Select)
    {
        case 0:
            // CODE: 0xfe3 is MSB
            MDrv_WriteByte(0xfe3, (Codetemp >> 16)& 0xff);
            MDrv_WriteByte(0xfe5, (Codetemp >> 8) & 0xff);
            MDrv_WriteByte(0xfe4, Codetemp & 0xff);
            // DATA: 0xff6 is MSB
            MDrv_WriteByte(0xff6, (Datatemp >> 16)& 0xff);
            MDrv_WriteByte(0xff5, (Datatemp >> 8) & 0xff);
            MDrv_WriteByte(0xff4, Datatemp & 0xff);
            break;

        case 1:
            // CODE: 0x3e3 is MSB
            MDrv_WriteByte(0x3e3, (Codetemp >> 16)& 0xff);
            MDrv_WriteByte(0x3e5, (Codetemp >> 8) & 0xff);
            MDrv_WriteByte(0x3e4, Codetemp & 0xff);
            // DATA: 0x3f6 is MSB
            MDrv_WriteByte(0x3f6, (Datatemp >> 16)& 0xff);
            MDrv_WriteByte(0x3f5, (Datatemp >> 8) & 0xff);
            MDrv_WriteByte(0x3f4, Datatemp & 0xff);
            break;

        default:
            printf("AEON Error Selection\n");
            break;
    }

}
//========================================
#endif

//below was moved to datatype.h as macro type
/*
U32 MDrv_ByteSwapU32(U32 val)
{
    return ((val             ) >> 24) |
           ((val & 0x00FF0000) >>  8) |
           ((val & 0x0000FF00) <<  8) |
           ((val             ) << 24);
}

U16 MDrv_ByteSwapU16(U16 val)
{
    return (val >> 8) | (val << 8);
}
*/

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

