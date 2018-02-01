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
// (MStar Confidential Information) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
///@file drvuart.h
///@brief System functions:UART Module
/// This file is vaild when "ENABLE_MSTV_UART_DEBUG" is enable
/// The brief function is get the register content by UART without halt the whole
/// system.
//
///////////////////////////////////////////////////////////////////////////////

#define _DRVUART_C_
#include "Board.h"

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "msAPI_Timer.h"

// Common Definition
#include "debug.h"
#include "apiXC.h"
#include "hwreg.h"
#include "drvUartDebug.h"
#include "sysinfo.h"
#include "drvIIC.h"
#include "apiXC_Dlc.h"
#include "apiXC_DBC.h"
#include "SysInit.h"
#include "apiXC_Dlc.h"
#include "apiGOP.h"
#include "drvGlobal.h"
#include "drvUART.h"
#include "Panel.h"
#include "msAPI_Ram.h"
#include "apiXC_Ace.h"
#include "apiXC_Sys.h"
#include "msAPI_audio.h"
#include "msAPI_Demod.h"
#include "apiXC_Dlc.h"
#include "drvPWM.h"

#include "autotest.h"
#include "MApp_USBDownload.h"


#include "MApp_SaveData.h"
#include "string.h"

#include "MApp_AutoTest.h"
#include "SW_Config.h"
#include "drvUSB.h"

#ifdef PRI_DEBUG
#include "R2A10406NP.h"
#endif


#include "COFDM_Demodulator.h"
#include "IF_Demodulator.h"

#if (ENABLE_DTV_EPG)
#include "mapp_eit.h"
#endif

#if ENABLE_EWS
#include "MApp_EWS.h"
#endif

#if (ENABLE_MSTV_UART_DEBUG)

#include "MApp_Key.h"
#include "MApp_UiMenuDef.h"

#include "MApp_KeyLogger.h"

// panel
#include "Panel.h"
#include "apiPNL.h"

//=======================================================================

#define UARTMSG(x)    //x

#if (FRONTEND_TUNER_TYPE == MSTAR_MSR1200_TUNER)
#define CANOPUS_DEBUG 1
#else
#define CANOPUS_DEBUG 0
#endif

#define PHOENIX_DEBUG   0

#if (CHANNEL_SCAN_AUTO_TEST)
#define CHSCANTEST_MSG(x)    x
#else
#define CHSCANTEST_MSG(x)
#endif

static XDATA U16 u16ACE_Report_Type = 0;

#if ENABLE_AUTOTEST
extern BOOLEAN g_bAutobuildDebug;
#endif

#if (ENABLE_MMS)
int DbgSystem = 0;
int CatEye    = 0;
int UnitTest  = 0;

extern U8 ParaTbl[PARA_ROW_SIZE][PARA_COL_SIZE];
void MDrv_LogoPrint(void);
#endif

/******************************************************************************/
/// UART device: putchar and ISR
/******************************************************************************/

static MS_U32 uart_debug;
U16 g_HDCP_KeyCounter ;
void putcharb(MS_U8 byte)
{
    if (uart_debug)
    {
        mdrv_uart_write(uart_debug, (MS_U8 *)&byte, 1);
    }
}

#define ENABLE_UART_CHECKSUM    1

#if( ENABLE_UART_CHECKSUM )
// Check uart protocal
bit IsUartCmdValid(void)
{
    BYTE i, ucLen;
    BYTE ucCS = 0;

    if( UART_EXT )
    {
        ucLen = UART_CMD_EXT_LENGTH;
    }
    else
    {
        ucLen = UART_CMD_LENGTH;
    }

    ucCS = 0;
    for( i = 0; i < ucLen; ++ i )
    {
        ucCS += g_UartCommand.Buffer[i];
    }

    if( ucCS == 0xFF )
        return 1;

    return 0;
}
#endif



void MDrv_UsbUpdate(void);
extern void msAPI_BLoader_Reboot(void);

#if(ENABLE_CONSOLE_CMD)
void msAPI_ConsoleCmd_RecvData(U8 u8Data);
#endif

void MDrv_UART_RecvHandler(int c, MS_U32 pHalReg)
{
#if( CHAKRA3_AUTO_TEST )
    MApp_AT_RecvDataHandler(c);

    if( MApp_AT_Is_NeedDisableOtherUartFunction() )
        return;
#endif

#if(ENABLE_CONSOLE_CMD)
    msAPI_ConsoleCmd_RecvData(c);
#endif

    g_Uart0CheckTick = 50;// time-out control ms

    if (g_UartCommand.Index >= _UART_CMD_LENGTH_)
    {
        g_UartCommand.Index = 0;
    }

#if 1//def MSOS_TYPE_NOS
        if( (c == 0x7F) && (g_UartCommand.Index==0) )// Press "Delete" to get PC & RA
        {
        #if 1
            msDebug_mips_BackTrace(pHalReg);
        #else
            unsigned long *addr;
            unsigned long *ra;
            unsigned long *sp;
            unsigned short depth;
            unsigned short ra_offset;
            unsigned short stack_size;

            ra = (unsigned long *)(((MHAL_SavedRegisters *) pHalReg)->d[31]);
            sp = (unsigned long *)(((MHAL_SavedRegisters *) pHalReg)->d[29]);

            printf("=================================================\n");
            printf("stack backtrace\n");
            printf("pc = %08lx\n", ((MHAL_SavedRegisters *) pHalReg)->pc);
            printf("#stack frame #0: ra = %08lx, sp = %08lx\n", (unsigned long)ra, (unsigned long)sp);
            //printf(" mips-sde-elf-addr2line -e AP.elf -f 0x%x\n", ra);
            printf(" aeon-addr2line -e AP.elf -f 0x%x\n", ra);

            extern U8  _readonly_start[];
            extern U8  _readonly_end[];

            if ( ((unsigned long)ra >= (unsigned long)_readonly_start) ||
                 ((unsigned long)ra <= (unsigned long)_readonly_end) )
            {
                // Only print out depth 16 call stack
                // Stop when *sp = 0xDEADBEEF (0xDEADBEEF is set in the head stack memory)
                for (depth = 0; (depth < 0x10) && (*sp != 0xDEADBEEF); ++depth)
                {
                    ra_offset = 0;
                    stack_size = 0;

                    for (addr = ra; !ra_offset || !stack_size; --addr)
                    {
                        switch(*addr & 0xffff0000)
                        {
                            case 0x27bd0000: // 0x27bdxxxx: addiu sp, sp, xxxx
                                stack_size = abs((short)(*addr & 0xffff));
                                break;
                            case 0xafbf0000: // 0xafbfxxxx: sw ra ,xxxx(sp)
                                ra_offset = (unsigned short)(*addr & 0xffff);
                                break;
                            default:
                                break;
                        }
                    }

                    ra = *(unsigned long **)((unsigned long)sp + (unsigned long)ra_offset);
                    sp = (unsigned long *)((unsigned long)sp + (unsigned long)stack_size);

                    printf("#stack frame #%d: ra = %08lx, sp = %08lx\n", depth+1, (unsigned long)ra, (unsigned long)sp);
                    //printf(" mips-sde-elf-addr2line -e AP.elf -f 0x%x\n", ra);
                    printf(" aeon-addr2line -e AP.elf -f 0x%x\n", ra);

                    if ( ((unsigned long)ra < (unsigned long)_readonly_start) ||
                         ((unsigned long)ra > (unsigned long)_readonly_end) )
                    {
                        printf("!!wrong ra!! Try again\n");
                        break;
                    }
                }
                printf("=================================================\n\n");
            }
        #endif
        }
#endif

#if (ENABLE_MMS)
    // detect dash
    if ((0x2D == c)&& (1 == DbgMenuMode))
    {
        InputPara = 1;
    }

    // recieve byte
    if (!InputPara)
    {
#endif
        g_UartCommand.Buffer[g_UartCommand.Index] = c;
#if (ENABLE_MMS)
    } else
    {
        MDrv_MMS_ParseCmdParm(c);
    }

    // receive for menu mode
    if (1 == DbgMenuMode)
    {
        MDrv_MMS_UARTRecvHandler(c);
        return;
    }

#endif

    // check command buffer index
    if (UART_EXT)
    {
        if (g_UartCommand.Index == 0) // check 1st data
        {
            g_UartCommand.Index++; // for get UART_CMD_EXT_LENGTH
        }
        else
        {
            if (g_UartCommand.Index < UART_CMD_EXT_LENGTH) // still read command
            {
                g_UartCommand.Index++; // next index of command buffer
            }
            if (g_UartCommand.Index >= UART_CMD_EXT_LENGTH) // read command ok
            {
                g_bUart0Detected = TRUE; // command  buffer recieve ok
                g_UartCommand.u8CmdLen = g_UartCommand.Index;
                g_UartCommand.Index = 0; // reset index of command buffer
                g_Uart0CheckTick = 0;
            }
        }
    }
    else
    {
        if (g_UartCommand.Index < UART_CMD_LENGTH) // still read command
        {
            g_UartCommand.Index++; // next index of command buffer
        }

        if (g_UartCommand.Index >= UART_CMD_LENGTH) // read command ok
        {
            // check if Enter Key. (0x0D)
            if (g_UartCommand.Buffer[_UART_CMD_CODE_] == 0x0D) {
              #if (ENABLE_MMS)
                //set flag to enter menu mode
                DbgMenuMode = 1;
                MDrv_LogoPrint();
                if (!(DbgMenuLevel ||CatMenuLevel || CfgMenuLevel || UntMenuLevel))
                    MDrv_MainPagePrint();
               #endif
            }

        #if( ENABLE_UART_CHECKSUM )
            if( IsUartCmdValid() )
        #endif
            {
                g_bUart0Detected = TRUE; // command  buffer recieve ok
                g_UartCommand.u8CmdLen = g_UartCommand.Index;
            }

            g_UartCommand.Index = 0; // reset index of command buffer
            g_Uart0CheckTick = 0;
        }
    }

    #if defined(MIPS_CHAKRA) || defined(__AEONR2__)
        MsOS_EnableInterrupt(E_INT_IRQ_UART0);
    #endif
}


/******************************************************************************/
///Initial UART relative variable and flag.
/******************************************************************************/
void MDrv_UART_DebugInit(void)
{
    // reset Uart variables

    g_UartCommand.Index = 0;

    g_bDebugASCIICommandFlag = 0;
    g_bDebugProgStopFlag = FALSE;

    g_Uart0CheckTick = 0;

#if defined(__aeon__)
    uart_debug = mdrv_uart_open(E_UART_AEON);
#else
    uart_debug = mdrv_uart_open(E_UART_PIU_UART0);
#endif

    if (uart_debug)
    {
#ifdef MSOS_TYPE_LINUX
        mdrv_uart_set_rx_callback(uart_debug, MDrv_UART_RecvHandler);
#else
        mdrv_uart_set_rx_callback_halreg(uart_debug, MDrv_UART_RecvHandler);
#endif
    }

}

//extern void MApp_UsbDownload_Monitor(void);
#if (ENABLE_TCON_BIN_IN_DB)
void MDrv_TCON_Save_SUBBIN(void);
#endif

void MDrv_UART_ExecTestCommand(void)
{
    BYTE ucPara1, ucPara2;

    ucPara1 = g_UartCommand.Buffer[_UART_CMD_INDEX2_];
    ucPara2 = g_UartCommand.Buffer[_UART_CMD_INDEX3_];

    switch ( g_UartCommand.Buffer[_UART_CMD_INDEX1_] )
    {
        case 0x50:
            g_bDebugProgStopFlag = !g_bDebugProgStopFlag;
            if( g_bDebugProgStopFlag )
            {
                printf("<Debug>\r\n");
            }
            else
            {
                printf("<Free>\r\n");
            }
            break;

#ifdef PRI_DEBUG
        case 0x51:
            {
                BYTE cValue;
                devCOFDM_PassThroughI2C_ReadBytes(_IF_DEM_I2C_ID, 0, NULL, 1, &cValue);
                printf("\r\n R2A10406 AFT = %bx", (cValue&_MASK_OF_AFC)>>5);
            }
            break;
#endif

#if (SUPPORT_PEQ_TOOL)
        case 0x59:
            printf("G %d ",g_UartCommand.Buffer[_UART_CMD_INDEX2_]);
            printf("Fh %d ",g_UartCommand.Buffer[_UART_CMD_INDEX3_]);
            printf("Fl %d ",g_UartCommand.Buffer[_UART_CMD_INDEX4_]);
            printf("Q %d\r\n",g_UartCommand.Buffer[_UART_CMD_INDEX5_]);
            msAPI_AUD_SetPEQ(0, g_UartCommand.Buffer[_UART_CMD_INDEX2_], g_UartCommand.Buffer[_UART_CMD_INDEX3_], g_UartCommand.Buffer[_UART_CMD_INDEX4_], g_UartCommand.Buffer[_UART_CMD_INDEX5_]);
            break;

        case 0x5A:
            printf("G %d ",g_UartCommand.Buffer[_UART_CMD_INDEX2_]);
            printf("Fh %d ",g_UartCommand.Buffer[_UART_CMD_INDEX3_]);
            printf("Fl %d ",g_UartCommand.Buffer[_UART_CMD_INDEX4_]);
            printf("Q %d\r\n",g_UartCommand.Buffer[_UART_CMD_INDEX5_]);
            msAPI_AUD_SetPEQ(1, g_UartCommand.Buffer[_UART_CMD_INDEX2_], g_UartCommand.Buffer[_UART_CMD_INDEX3_], g_UartCommand.Buffer[_UART_CMD_INDEX4_], g_UartCommand.Buffer[_UART_CMD_INDEX5_]);
            break;

        case 0x5B:
            printf("G %d ",g_UartCommand.Buffer[_UART_CMD_INDEX2_]);
            printf("Fh %d ",g_UartCommand.Buffer[_UART_CMD_INDEX3_]);
            printf("Fl %d ",g_UartCommand.Buffer[_UART_CMD_INDEX4_]);
            printf("Q %d\r\n",g_UartCommand.Buffer[_UART_CMD_INDEX5_]);
            msAPI_AUD_SetPEQ(2, g_UartCommand.Buffer[_UART_CMD_INDEX2_], g_UartCommand.Buffer[_UART_CMD_INDEX3_], g_UartCommand.Buffer[_UART_CMD_INDEX4_], g_UartCommand.Buffer[_UART_CMD_INDEX5_]);
            break;
#if (PEQ_BANDS == 5)
        case 0x5C:
            printf("G %d ",g_UartCommand.Buffer[_UART_CMD_INDEX2_]);
            printf("Fh %d ",g_UartCommand.Buffer[_UART_CMD_INDEX3_]);
            printf("Fl %d ",g_UartCommand.Buffer[_UART_CMD_INDEX4_]);
            printf("Q %d\r\n",g_UartCommand.Buffer[_UART_CMD_INDEX5_]);
            msAPI_AUD_SetPEQ(3, g_UartCommand.Buffer[_UART_CMD_INDEX2_], g_UartCommand.Buffer[_UART_CMD_INDEX3_], g_UartCommand.Buffer[_UART_CMD_INDEX4_], g_UartCommand.Buffer[_UART_CMD_INDEX5_]);
            break;

        case 0x5D:
            printf("G %d ",g_UartCommand.Buffer[_UART_CMD_INDEX2_]);
            printf("Fh %d ",g_UartCommand.Buffer[_UART_CMD_INDEX3_]);
            printf("Fl %d ",g_UartCommand.Buffer[_UART_CMD_INDEX4_]);
            printf("Q %d\r\n",g_UartCommand.Buffer[_UART_CMD_INDEX5_]);
            msAPI_AUD_SetPEQ(4, g_UartCommand.Buffer[_UART_CMD_INDEX2_], g_UartCommand.Buffer[_UART_CMD_INDEX3_], g_UartCommand.Buffer[_UART_CMD_INDEX4_], g_UartCommand.Buffer[_UART_CMD_INDEX5_]);
            break;
#endif
#endif

        case 0x63:  // Keypad Up
            u8KeyCode = KEY_UP;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;

        case 0x72:  // Keypad Left
            u8KeyCode = KEY_LEFT;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0x73:  // Keypad Menu
            u8KeyCode = KEY_MENU;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0x74:  // Keypad Right
            u8KeyCode = KEY_RIGHT;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0x75:  // Keypad Source
            u8KeyCode = KEY_INPUT_SOURCE;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0x76:  // Keypad Power
            u8KeyCode = KEY_POWER;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0x83:  // Keypad Down
            u8KeyCode = KEY_DOWN;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;

        case 0x90:  // Irkey Input
            u8KeyCode = KEY_INPUT_SOURCE;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0x91:  // Irkey Sleep
            u8KeyCode = KEY_SLEEP;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0x92:  // Irkey 1
            u8KeyCode = KEY_1;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0x93:  // Irkey 4
            u8KeyCode = KEY_4;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0x94:  // Irkey 7
            u8KeyCode = KEY_7;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0x95:  // Irkey 0
            u8KeyCode = KEY_0;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0x96:  // Irkey Select
            u8KeyCode = KEY_SELECT;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0x97:  // Irkey Menu
            u8KeyCode = KEY_MENU;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xA1:  // Irkey Display
            u8KeyCode = KEY_INFO;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xA2:  // Irkey 2
            u8KeyCode = KEY_2;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xA3:  // Irkey 5
            u8KeyCode = KEY_5;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xA4:  // Irkey 8
            u8KeyCode = KEY_8;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xA5:  // Irkey -
            u8KeyCode = KEY_DASH;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xA6:  // Irkey Up
            u8KeyCode = KEY_INPUT_SOURCE;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xA7:  // Irkey Left
            u8KeyCode = KEY_LEFT;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xB1:  // Irkey MTS
            u8KeyCode = KEY_MTS;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xB2:  // Irkey 3
            u8KeyCode = KEY_3;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xB3:  // Irkey 6
            u8KeyCode = KEY_6;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xB4:  // Irkey 9
            u8KeyCode = KEY_9;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
//        case 0xB5:  // Irkey CC
//            u8KeyCode = KEY_CC;
//            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
//            break;
        case 0xB5:  // Irkey Freeze
            u8KeyCode = KEY_FREEZE;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xB6:  // Irkey Zoom
            u8KeyCode = KEY_ZOOM;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xB7:  // Irkey Right
            u8KeyCode = KEY_RIGHT;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xB8:  // Irkey Exit
            u8KeyCode = KEY_EXIT;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xC1:  // Irkey Mute
            u8KeyCode = KEY_MUTE;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xC2:  // Irkey Vol+
            u8KeyCode = KEY_VOLUME_PLUS;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xC3:  // Irkey Vol-
            u8KeyCode = KEY_VOLUME_MINUS;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xC4:  // Irkey Ch+
            u8KeyCode = KEY_CHANNEL_PLUS;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xC5:  // Irkey Ch-
            u8KeyCode = KEY_CHANNEL_MINUS;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xC6:  // Irkey Picture
            u8KeyCode = KEY_PICTURE;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xC7:  // Irkey Down
            u8KeyCode = KEY_DOWN;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xD0:  // Irkey Power
            u8KeyCode = KEY_POWER;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xD1:  // Irkey List
            u8KeyCode = KEY_CHANNEL_LIST;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xD2:  // Irkey Fav
            u8KeyCode = KEY_CHANNEL_FAV_LIST;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xD3:  // Irkey Sound
            u8KeyCode = KEY_AUDIO;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
//        case 0xD4:  // Irkey GUID
//            u8KeyCode = KEY_GUIDE;
//            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xD5:  // Irkey EPG
            u8KeyCode = KEY_EPG;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xD6:  // Irkey EXIT
            u8KeyCode = KEY_EXIT;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
        case 0xD7:  // Irkey Up
            u8KeyCode = KEY_UP;
            u32MonitorOsdTimer = msAPI_Timer_GetTime0();
            break;
#ifndef ATSC_SYSTEM
        case 0xBA:
           // devCOFDM_Init();
        break;
#if (ENABLE_DTV_EPG)
    case 0xBB:
        {
            MS_EIT_CUR_EVENT_PF astEventInfo[2];
            if(MApp_EIT_GetCurPFInfo(astEventInfo) == FALSE)break;
            printf("\nf:><VersionNUm-Cur:%u>\n",astEventInfo ->version_number);
            if(MApp_EIT_Is_Cur_PF_Change())
            printf("Version Changing\n");
                else
            printf("VC End\n");
        }
        break;
#endif  //#if (ENABLE_DTV_EPG)
#endif
    case 0xBC:
            //MApp_SI_INFO();
        break;
    case 0xCA:
        printf("enable gwin\n");
        MApi_GOP_GWIN_Enable(g_UartCommand.Buffer[_UART_CMD_INDEX2_],TRUE);
        break;
    case 0xCB:
        printf("disable gwin\n");
        MApi_GOP_GWIN_Enable(g_UartCommand.Buffer[_UART_CMD_INDEX2_],FALSE);
        break;
    case 0xCC:
        #if (ENABLE_TCON_BIN_IN_DB)
        MDrv_TCON_Save_SUBBIN();
        #endif
        break;
    case 0xDA:
        break;
    case 0xDB:
          break;
    case 0xDC:
        break;
#ifndef ATSC_SYSTEM
    case 0x67:
        {
#if ( (ENABLE_USB || ENABLE_USB_2) && ENABLE_FILESYSTEM )
#ifndef MSOS_TYPE_LINUX
            extern U8 MDrv_USBGetPortEnableStatus(void);
            extern void MApp_ZUI_SwUpdate_ProgressBar(U8 percent);
            extern void msAPI_BLoader_Reboot(void);
            U8 u8PortEnStatus = 0;

            printf("\r\n --> USB SW Update!\n");
            u8PortEnStatus = MDrv_USBGetPortEnableStatus();
            if(((u8PortEnStatus & BIT0) == BIT0)&&MDrv_UsbDeviceConnect())
            {
                MApp_UsbDownload_Init(BIT0, MApp_ZUI_SwUpdate_ProgressBar);
            }
            else if(((u8PortEnStatus & BIT1) == BIT1)&&MDrv_UsbDeviceConnect_Port2())
            {
                MApp_UsbDownload_Init(BIT1, MApp_ZUI_SwUpdate_ProgressBar);
            }
            else
            {
                printf("Error> Unknown USB port\n");
                return;
            }

            if(!MW_UsbDownload_Search())
            {
                break;
            }

            if (MW_UsbDownload_Start())
            {
                msAPI_BLoader_Reboot();
            }
#else
    printf("Un-support USB Update\n");
#endif
#endif
        }
        break;
#endif

    case 0x68:
        {
			if(g_UartCommand.Buffer[_UART_CMD_INDEX2_] == 0)
            {
                msAPI_Demod_Init(E_INIT_DEMOD_FOR_CHANGE_SOURCE, (EN_DEMODULATOR_TYPE)g_UartCommand.Buffer[_UART_CMD_INDEX3_]);
                //msAPI_Tuner_SetCurrentDemodType((EN_DEMODULATOR_TYPE)g_UartCommand.Buffer[_UART_CMD_INDEX3_]);
            }
			else
			{
                printf("\nmsAPI_Demod_GetCurrentDemodType=%d", (U8)msAPI_Demod_GetCurrentDemodType());
			}
        }
        break;
#if ENABLE_KEY_LOGGER
    case 0x6D:
        {
            if(g_UartCommand.Buffer[_UART_CMD_INDEX2_] == 0)
            {
                MApp_KeyLogger_Action_DumpAll();
            }
            else if(g_UartCommand.Buffer[_UART_CMD_INDEX2_] == 1)
            {
                MApp_KeyLogger_SetMode((EN_KEYLOGGER_MODE)g_UartCommand.Buffer[_UART_CMD_INDEX3_]);
            }
            else if(g_UartCommand.Buffer[_UART_CMD_INDEX2_] == 2)
            {
                MApp_KeyLogger_Init();
            }
            else if(g_UartCommand.Buffer[_UART_CMD_INDEX2_] == 3)
            {
                stGenSetting.stDebugSetting.bKeyLoggerEnable = g_UartCommand.Buffer[_UART_CMD_INDEX3_];
            }
            else if(g_UartCommand.Buffer[_UART_CMD_INDEX2_] == 4)
            {
                MApp_KeyLogger_ModifyConfigBin((EN_KEYLOGGER_MODE)g_UartCommand.Buffer[_UART_CMD_INDEX3_]);
            }
            else
            {
                MApp_KeyLogger_Encapsulate((EN_KEYLOGGER_PACK_MODE)g_UartCommand.Buffer[_UART_CMD_INDEX3_]);
            }
        }
        break;
#endif

#if DYNAMIC_VECTOR_FONT_ENABLE
    case 0x61:
            {
                extern void msAPI_MVF_test(void);
                msAPI_MVF_test();
            }
            break;
    case 0x62:
            {
                extern void msAPI_MVF_LoadFont(void);
                msAPI_MVF_LoadFont();
            }
            break;
#endif


    }
}
/******************************************************************************/
/// Decode the Command gets from UART port for main chip.
/// Command Buffer [0]:
///  -bit7~bit5 :Command length:1 ~ 6
///  -bit4~bit0 :UART command
/// Command Buffer [1]:
///  -Control address high.
/// Command Buffer [2]:
///  -Control address low.
/// if write command:
///   -Command buffer [3]: control data
/******************************************************************************/
void MDrv_UART_DecodeNormalCommand(void)
{
    //printf("MDrv_UART_DecodeNormalCommand %x\n", (U16)UART_CMD );
    U8 u8Value;
#if PHOENIX_DEBUG
    U16 RegAddr, RegData = 0;
#endif
    U16 u16EEADDR;

    switch ( UART_CMD )
    {
        case uartWrite_MST_Bank_n:
            MDrv_WriteByte(((U8)UART_CMD_MS_BANK_H << 16) |
                           ((U8)UART_CMD_MS_BANK_L << 8)  |
                            (U8)UART_CMD_MS_REGINDEX,
                                UART_CMD_MS_REGDATA
                          );
            putcharb( 0xF1 );
            break;
        case uartRead_MST_Bank_n:
            putcharb( 0xF2 );
            putcharb(MDrv_ReadByte(((U8)UART_CMD_MS_BANK_H << 16) |
                                   ((U8)UART_CMD_MS_BANK_L << 8)  |
                                   (U8)UART_CMD_MS_REGINDEX
                                  )
                    );
            break;
        case uartWrite_MCU_XDATA:
             MDrv_WriteByte(((U8)UART_CMD_MCU_IDX_H1 << 16) |
                           ((U8)UART_CMD_MCU_IDX_H2 << 8)  |
                            (U8)UART_CMD_MCU_IDX_L,
                                UART_CMD_MCU_DATA
                          );
            putcharb( 0xF1 );
            // MDrv_WriteByte(((U8)UART_CMD_MCU_IDX_H<<8) | (U8)UART_CMD_MCU_IDX_L, UART_CMD_MCU_DATA);
            //putcharb(0xF1);
            break;
        case uartRead_MCU_XDATA:
            putcharb( 0xF2 );
            putcharb(MDrv_ReadByte(((U8)UART_CMD_MCU_IDX_H1 << 16) |
                                   ((U8)UART_CMD_MCU_IDX_H2 << 8)  |
                                   (U8)UART_CMD_MCU_IDX_L
                                  )
                    );
            // putcharb(0xF2);
            //  putcharb(MDrv_ReadByte(((U8)UART_CMD_MCU_IDX_H<<8)|(U8)UART_CMD_MCU_IDX_L));
            break;

        case uartI2C_Device_Write:
            if ( FALSE == MDrv_IIC_WriteByte( UART_CMD_MCU_IDX_H, UART_CMD_MCU_IDX_L, UART_CMD_MCU_DATA ) )
            {
                printf( "I2C Error!!! uartI2C_Device_Write\n" );
            }

            putcharb( 0xF1 );
            break;

        case uartI2C_Device_Read:
            putcharb( 0xF2 );
            if ( MDrv_IIC_ReadByte( UART_CMD_MCU_IDX_H, UART_CMD_MCU_IDX_L, &u8Value ) )
                putcharb( u8Value );
            else
                putcharb( 0xFF );
            break;

        case uartEEPROM_Write:
            u16EEADDR = MAKEWORD( g_UartCommand.Buffer[_UART_CMD_INDEX1_], g_UartCommand.Buffer[_UART_CMD_INDEX2_] );
            msAPI_rmWriteByte( u16EEADDR, UART_CMD_MCU_DATA );
            printf( "\r\n u16EEADDR : 0x%x   data : 0x%x", u16EEADDR, ( U16 ) UART_CMD_MCU_DATA );
            putcharb( 0xF1 );
            break;

        case uartEEPROM_Read:
            u16EEADDR = MAKEWORD( g_UartCommand.Buffer[_UART_CMD_INDEX1_], g_UartCommand.Buffer[_UART_CMD_INDEX2_] );
            g_UartCommand.Buffer[_UART_CMD_INDEX3_] = msAPI_rmReadByte( u16EEADDR );
            putchar( 0xF2 );
            putchar( g_UartCommand.Buffer[_UART_CMD_INDEX3_] );
            /*
            if (g_bDebugASCIICommandFlag)
            {
                printf("EEPROM[0x%x]=", uwEEADDR);
                printf("0x%x \r\n", g_UartCommand.Buffer[_UART_CMD_INDEX3_]);
            }*/
            break;

        case uartTest_Command:
            MDrv_UART_ExecTestCommand();
            break;

#if PHOENIX_DEBUG
        case uartRead_Demod:
            #if 0
            {
                RegAddr = ( ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX1_] << 8 ) | ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX2_];
                RegData = devCOFDM_ReadReg( RegAddr );
                putcharb( 0xF3 );
                putcharb( ( U8 ) ( RegData >> 8 ) );
                putcharb( ( U8 ) ( RegData & 0xff ) );
                //printf("[%x] = %x\n", RegAddr, RegData);
            }
            #else
            {
                RegAddr = ( ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX1_] << 8 ) | ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX2_];
                switch(UART_EXT_CMD)
                {
                    #if (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
                    case E_DEMOD_DVBT:
                    {
                        RegData = devCOFDM_ReadReg( RegAddr );
                    }
                    break;
                    #endif
                    #if ENABLE_DVB_T2
                    case E_DEMOD_DVBT2:
                    {
                        MS_U8 u8RegData = 0;
                        mdev_CofdmReadReg( RegAddr, &u8RegData );
                        RegData = u8RegData;
                    }
                    break;
                    #endif
                    #if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
                    case E_DEMOD_DVBC:
                    {
                        RegData = devQAM_ReadReg( RegAddr );
                    }
                    break;
                    #endif
                    #if 0
                    case E_DEMOD_ISDBT:
                    {
                        RegData = devCOFDM_ReadReg( RegAddr );
                    }
                    break;
                    case E_DEMOD_DVBS:
                    case E_DEMOD_DVBS2:
                    {
                        RegData = devCOFDM_ReadReg( RegAddr );
                    }
                    break;
                    case E_DEMOD_DTMB:
                    {
                        RegData = devCOFDM_ReadReg( RegAddr );
                    }
                    break;
                    #endif
                    #if (FRONTEND_DEMOD_TYPE == EMBEDDED_ATSC_DEMOD)
                    case E_DEMOD_ATSC:
                    {
                        RegData = INTERN_ATSC_ReadReg( RegAddr );
                    }
                    break;
                    #endif
                    default:
                    {
                        ASSERT(0);
                    }
                    break;
                }
                putcharb( 0xF3 );
                putcharb( ( U8 ) ( RegData >> 8 ) );
                putcharb( ( U8 ) ( RegData & 0xff ) );

                //printf("R:[%x] = %x\n", RegAddr, RegData);
            }
            #endif
            break;
        case uartWrite_Demod:
            #if 0
            {
                RegAddr = ( ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX1_] << 8 ) | ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX2_];
                RegData = ( ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX3_] << 8 ) | ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX4_];
                RegData = devCOFDM_WriteReg( RegAddr, RegData );
                putcharb( 0xF1 );
                //printf("[%x] = %x\n", RegAddr, RegData);
            }
            #else
            {
                MS_BOOL bRet;
                RegAddr = ( ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX1_] << 8 ) | ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX2_];
                RegData = ( ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX3_] << 8 ) | ( U16 ) g_UartCommand.Buffer[_UART_CMD_INDEX4_];
                switch(UART_EXT_CMD)
                {
                    #if (FRONTEND_DEMOD_TYPE == EMBEDDED_DVBT_DEMOD)
                    case E_DEMOD_DVBT:
                    {
                        bRet=devCOFDM_WriteReg( RegAddr, RegData );
                    }
                    break;
                    #endif
                    #if ENABLE_DVB_T2
                    case E_DEMOD_DVBT2:
                    {
                        bRet=mdev_CofdmWriteReg( RegAddr, RegData );
                    }
                    break;
                    #endif
                    #if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
                    case E_DEMOD_DVBC:
                    {
                        bRet = devQAM_WriteReg( RegAddr, RegData );
                    }
                    break;
                    #endif
                    #if 0
                    case E_DEMOD_ISDBT:
                    {
                        RegData = devCOFDM_ReadReg( RegAddr );
                    }
                    break;
                    case E_DEMOD_DVBS:
                    case E_DEMOD_DVBS2:
                    {
                        RegData = devCOFDM_ReadReg( RegAddr );
                    }
                    break;
                    case E_DEMOD_DTMB:
                    {
                        RegData = devCOFDM_ReadReg( RegAddr );
                    }
                    break;
                    #endif
                    #if (FRONTEND_DEMOD_TYPE == EMBEDDED_ATSC_DEMOD)
                    case E_DEMOD_ATSC:
                    {
                        bRet = INTERN_ATSC_WriteReg( RegAddr, RegData );
                    }
                    break;
                    #endif
                    default:
                    {
                        ASSERT(0);
                    }
                    break;
                }
                putcharb( 0xF1 );

                //printf("W:[%x] = %x\n", RegAddr, RegData);
            }
            #endif
            break;
#endif
#if CANOPUS_DEBUG
        case uartRead_Tuner:
            {
                devCOFDM_PassThroughI2C_ReadBytes(_TUNER_I2C_ID,
                                              1,
                                              &g_UartCommand.Buffer[_UART_CMD_INDEX1_],
                                              1,
                                              &u8Value);
                putcharb( 0xF2 );
                putcharb( u8Value );
                //printf("[%x] = %x\n", RegAddr, RegData);
            }
            break;
        case uartWrite_Tuner:
            {
                  devCOFDM_PassThroughI2C_WriteBytes(_TUNER_I2C_ID,
                                               1,
                                               &g_UartCommand.Buffer[_UART_CMD_INDEX1_],
                                               1,
                                               &g_UartCommand.Buffer[_UART_CMD_INDEX2_]);
                putcharb( 0xF1 );
                //printf("[%x] = %x\n", RegAddr, RegData);
            }
            break;
        case uartRead_PI2C:
            {
                devCOFDM_PassThroughI2C_ReadBytes(_IF_DEM_I2C_ID,
                                              0,
                                              NULL,
                                              1,
                                              &u8Value);
                putcharb( 0xF2 );
                putcharb( u8Value );
            }
            break;
        case uartWrite_PI2C:
            {
                  devCOFDM_PassThroughI2C_WriteBytes(_IF_DEM_I2C_ID,
                                               1,
                                               &g_UartCommand.Buffer[_UART_CMD_INDEX1_],
                                               1,
                                               &g_UartCommand.Buffer[_UART_CMD_INDEX2_]);
                putcharb( 0xF1 );
                printf( "TDA9886:[%d] = 0x%x\n", ( int ) g_UartCommand.Buffer[_UART_CMD_INDEX1_], ( int ) g_UartCommand.Buffer[_UART_CMD_INDEX2_] );
            }
            break;
#endif
        default:
            // printf("Unsupport Normal Command: 0x%x\n", (U16)UART_CMD );
            break;
    }
}

/******************************************************************************/
///     @brief Adust PWM, which is board&GPIO dependent.
///     @param pwm_value   IN:value to set the backlight, from 0 up to 255.
/******************************************************************************/
void _MDrv_UART_SetBacklightDutyCycle(MS_U32 pwm_value)
{
    printf("<Debug><_MDrv_UART_SetBacklightDutyCycle> pwm_value=%ld\r\n",pwm_value);

    MDrv_PWM_DutyCycle((PWM_ChNum)UART_EXT_CMD_MS_DAT2, pwm_value);
}


/******************************************************************************/
/// Decode the Command gets from UART port for extra device(like tunner/demodulator).
/// Command Buffer [0]:
///  -bit7~bit5 :111b
///  -bit4~bit0 :extra UART command
/// Command Buffer [1~8]:
///  -Extra Control data buffer.
/******************************************************************************/
void MDrv_UART_DecodeExtCommand(void)
{
    MS_U8 u8addressCount, u8dataCount;
    MS_U8 u8Result;

    switch( UART_EXT_DEV )
    {
#if ENABLE_DLC
        #if (ENABLE_DLC_TEST_BY_MSTVTOOL)
        case uartExtDev_MstDlcTest:
        {
            tDLC_CONTROL_PARAMS tDLC_Params = {
                .pCmdBuff       = g_UartCommand.Buffer,
                .u16PNL_Width   = g_IPanel.Width(),
                .u16PNL_Height  = g_IPanel.Height(),
                .fnDLC_Putchar  = putcharb
             };

            MApi_XC_DLC_DecodeExtCmd(&tDLC_Params);
            break;
        }
    #endif //ENABLE_DLC_TEST_BY_MSTVTOOL
#endif
    #if (ENABLE_DBC)
        case DEV_MST_DBC_TEST:
        {
             tDBC_CONTROL_PARAMS tDBC_Params ={
                 .pCmdBuff              = g_UartCommand.Buffer,
                 .fnDBC_AP_OnOff        = MApi_XC_Sys_DLC_DBC_OnOff,
                 .fnDBC_AdjustBacklight = _MDrv_UART_SetBacklightDutyCycle,
                 .fnDBC_Putchar         = putcharb
             };
             MApi_XC_DBC_DecodeExtCmd(&tDBC_Params);
            break;
        }
    #else
        case DEV_MST_DBC_TEST:
        {
            putcharb(0xF2);
            putcharb(0);
            break;
        }
    #endif //ENABLE_DBC

#if 1
        //ENABLE_ACE_REPORT
        case uartExtDev_ACE_REPORT:
        {
            switch(UART_EXT_CMD)
            {
                case URCMD_MST_ACE_REPORT_TYPE:
                    u16ACE_Report_Type = UART_EXT_CMD_MS_DAT1;
                    break;

                case URCMD_MST_ACE_REPORT_CONTRAST:
                    if(0)//( u16ACE_Report_Type&ACE_REPORT_CONTRAST_10BITS ) // New tool command
                    {
                        putcharb(0xF3);
                        putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_CONTRAST)>>9 );
                        putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_CONTRAST)>>1 );
                    }
                    else
                    {
                        putcharb(0xF2);
                        putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_CONTRAST)>>3 );
                    }
                    break;
                case URCMD_MST_ACE_REPORT_SATURATION:
                    putcharb(0xF2);
                    putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_SATURATION) );
                    break;
                case URCMD_MST_ACE_REPORT_HUE:
                    putcharb(0xF2);
                    putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_HUE) );
                    break;
                case URCMD_MST_ACE_REPORT_R_GAIN:
                    if(0)//( u16ACE_Report_Type&ACE_REPORT_CONTRAST_10BITS ) // New tool command
                    {
                        putcharb(0xF3);
                        putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_R_GAIN)>>9 );
                        putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_R_GAIN)>>1 );
                    }
                    else
                    {
                        putcharb(0xF2);
                        putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_R_GAIN)>>3 );
                    }
                    break;
                case URCMD_MST_ACE_REPORT_G_GAIN:
                    if(0)//( u16ACE_Report_Type&ACE_REPORT_CONTRAST_10BITS ) // New tool command
                    {
                        putcharb(0xF3);
                        putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_G_GAIN)>>9 );
                        putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_G_GAIN)>>1 );
                    }
                    else
                    {
                        putcharb(0xF2);
                        putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_G_GAIN)>>3 );
                    }
                    break;
                case URCMD_MST_ACE_REPORT_B_GAIN:
                    if(0)//( u16ACE_Report_Type&ACE_REPORT_CONTRAST_10BITS ) // New tool command
                    {
                        putcharb(0xF3);
                        putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_G_GAIN)>>9 );
                        putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_B_GAIN)>>1 );
                    }
                    else
                    {
                        putcharb(0xF2);
                        putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_B_GAIN)>>3 );
                    }
                    break;
                case URCMD_MST_ACE_REPORT_R_BRI:
                    putcharb(0xF2);
                    #if 0
                    MDrv_WriteByte(BK_SELECT_00, REG_BANK_S_VOP);
                    //if (1)//(UART_EXT_CMD_MS_DAT1 == 0) // Main
                        putcharb( MDrv_Read2Byte(L_BK_S_VOP(0x36)) >> 3 );
                    //else
                        //putcharb( (msRead2Bytes(SCF_72) >> 3) );
                    #else
                    // Temporarily changed, remove "Analog_Reg.h"
                    {
                        MDrv_WriteByte(0x102F00, 0x0F);
                        putcharb( MDrv_Read2Byte(((U32)0x102F00 | ((U16)0x36 << 1))) >> 3 );
                    }
                    #endif
                    break;
                case URCMD_MST_ACE_REPORT_G_BRI:
                    putcharb(0xF2);
                    #if 0
                    MDrv_WriteByte(BK_SELECT_00, REG_BANK_S_VOP);
                    //if (1)//(UART_EXT_CMD_MS_DAT1 == 0) // Main
                        putcharb( MDrv_Read2Byte(L_BK_S_VOP(0x37)) >> 3 );
                    //else
                        //putcharb( (msRead2Bytes(SCF_74) >> 3) );
                    #else
                    // Temporarily changed, remove "Analog_Reg.h"
                    {
                        MDrv_WriteByte(0x102F00, 0x0F);
                        putcharb( MDrv_Read2Byte(((U32)0x102F00 | ((U16)0x37 << 1))) >> 3 );
                    }
                    #endif
                    break;
                case URCMD_MST_ACE_REPORT_B_BRI:
                    putcharb(0xF2);
                    #if 0
                    MDrv_WriteByte(BK_SELECT_00, REG_BANK_S_VOP);
                    //if (1)//(UART_EXT_CMD_MS_DAT1 == 0) // Main
                        putcharb( MDrv_Read2Byte(L_BK_S_VOP(0x38)) >> 3 );
                   // else
                        //putcharb( (msRead2Bytes(SCF_76) >> 3) );
                    #else
                    // Temporarily changed, remove "Analog_Reg.h"
                    {
                        MDrv_WriteByte(0x102F00, 0x0F);
                        putcharb( MDrv_Read2Byte(((U32)0x102F00 | ((U16)0x38 << 1))) >> 3 );
                    }
                    #endif
                    break;
                case URCMD_MST_ACE_REPORT_COLOR_CORRECTION_XY_R:
                    putcharb(0xF2);
                    putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_COLOR_CORRECTION_XY_R) );
                    break;
                case URCMD_MST_ACE_REPORT_COLOR_CORRECTION_XY_G:
                    putcharb(0xF2);
                    putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_COLOR_CORRECTION_XY_G) );
                    break;
                case URCMD_MST_ACE_REPORT_COLOR_CORRECTION_XY_B:
                    putcharb(0xF2);
                    putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_COLOR_CORRECTION_XY_B) );
                    break;
                case URCMD_MST_ACE_REPORT_COLOR_CORRECTION_OFFSET_R:
                    putcharb(0xF2);
                    putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_R) );
					printf("R:%02x\n",MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_R));
                    break;
                case URCMD_MST_ACE_REPORT_COLOR_CORRECTION_OFFSET_G:
                    putcharb(0xF2);
                    putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_G) );
					printf("G:%02x\n",MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_G));
                    break;
                case URCMD_MST_ACE_REPORT_COLOR_CORRECTION_OFFSET_B:
                    putcharb(0xF2);
                    putcharb( MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_B) );
					printf("B:%02x\n",MApi_XC_ACE_GetACEInfo(UART_EXT_CMD_MS_DAT1, E_XC_ACE_INFO_TYPE_COLOR_CORRECTION_OFFSET_B));
                    break;
                case URCMD_MST_ACE_REPORT_IF_SUPPORT_RGB_GAIN:
                    putcharb(0xF2);
                    putcharb(ACE_INFOEX_MODE_POST_RGB_GAIN);
                    break;
            } // End of switch(dev_cmd)
            break;
        }
#endif
        case uartExtDev_I2C_DEVICE:
             u8addressCount = (UART_EXT_CMD_MS_DAT1 & 0xF0) >> 4;
             u8dataCount    = (UART_EXT_CMD_MS_DAT1 & 0x0F);
             if( u8addressCount + u8dataCount > 12 ) //UART_EXT_CMD_MS_DAT2...UART_EXT_CMD_MS_DAT13
               {   UARTMSG(printf("I2C error!")); }
	 	{
			U16 u16I2CAddr=((UART_EXT_CMD&0xF0)==0xc0?0x200|UART_EXT_CMD:UART_EXT_CMD);//Add for Tuner debug
			//printf("u16I2CAddr:0x%02X ", u16I2CAddr);
		if( UART_EXT_CMD & 0x01 ) // I2C read
                 u8Result = MDrv_IIC_ReadBytes( u16I2CAddr, //UART_EXT_CMD,
                                                u8addressCount,
                                                &UART_EXT_CMD_MS_DAT2,
                                                u8dataCount,
                                                (&UART_EXT_CMD_MS_DAT2)+u8addressCount);
             else                      // I2C write
                 u8Result = MDrv_IIC_WriteBytes( u16I2CAddr, //UART_EXT_CMD,
                                                u8addressCount,
                                                &UART_EXT_CMD_MS_DAT2,
                                                u8dataCount,
                                                (&UART_EXT_CMD_MS_DAT2)+u8addressCount);
             	}

             if( u8Result )
             {
                BYTE i;
                 i = _UART_CMD_INDEX5_;
                 UARTMSG(printf("0x%02X ", (U16)UART_EXT_CMD ));
                 while( u8addressCount > 0 )
                 {
                     UARTMSG(printf("W0x%02X ", (U16)g_UartCommand.Buffer[i]));
                     --u8addressCount;
                     ++i;
                 }
                 while( u8dataCount > 0 )
                 {
                     if( UART_EXT_CMD & 0x01 ) {
                         UARTMSG(printf("R0x%02X ", (U16)g_UartCommand.Buffer[i]));}
                     else {
                         UARTMSG(printf("W0x%02X ", (U16)g_UartCommand.Buffer[i]));}
                     --u8dataCount;
                     ++i;
                 }
                 UARTMSG(printf("\n"));
             }
             else {
                 UARTMSG(printf("0x%02X doesn't exist!\n", (U16)UART_EXT_CMD));
             }
             break;

        case uartExtDev_SAMSUNG_S5H1409_DEMODE:
        {
            BYTE i;
             i = 0;
             while( 1 )
             {
                 if( MDrv_IIC_ReadBytes( UART_EXT_CMD|0x01 , 1, &i, 2, &UART_EXT_CMD_MS_DAT12) )
                     UARTMSG(printf("Reg 0x%02X = 0x%02X 0x%02X\n", (U16)i, (U16)UART_EXT_CMD_MS_DAT12, (U16)UART_EXT_CMD_MS_DAT13));
                 else
                 {
                     UARTMSG(printf("0x%02X doesn't exist!\n", (U16)UART_EXT_CMD));
                     break;
                 }
                 if( i == 0xFF )
                    break;
                 ++i;
             }
        }
             break;

       case uartExtDev_XC_rw:

         if (UART_EXT_CMD == 0x02) {
               MDrv_WriteByte(0x102F00, UART_EXT_CMD_MS_DAT1 );

	       MDrv_WriteByte(((U8)UART_EXT_CMD_MS_DAT2 << 16) |
					            ((U8)UART_EXT_CMD_MS_DAT3 << 8)  |
					            (U8)UART_EXT_CMD_MS_DAT4,
					            (U8)UART_EXT_CMD_MS_DAT5
					           );
                putcharb(0xF1);
         }else if (UART_EXT_CMD == 0x01) {
                putcharb(0xF2);

				MDrv_WriteByte(0x102F00, UART_EXT_CMD_MS_DAT1 );
                putcharb(MDrv_ReadByte(((U8)UART_EXT_CMD_MS_DAT2 << 16) |
                                       ((U8)UART_EXT_CMD_MS_DAT3 << 8)  |
                                       (U8)UART_EXT_CMD_MS_DAT4
                                      )
                        );
          }
       break;

#if ((CHANNEL_SCAN_AUTO_TEST) || (SFU_AUTO_TEST))
        case uartExtDev_I2C_DEVICE_NEW:
             u8addressCount = (UART_EXT_CMD_MS_DAT1 & 0xF0) >> 4;
             u8dataCount    = (UART_EXT_CMD_MS_DAT1 & 0x0F);
             if( u8addressCount + u8dataCount > 12 ) //UART_EXT_CMD_MS_DAT2...UART_EXT_CMD_MS_DAT13
             {    CHSCANTEST_MSG(printf("I2C error!"));}
             if( UART_EXT_CMD & 0x01 ) // I2C read
                 u8Result = MDrv_IIC_ReadBytes( UART_EXT_CMD,
                                                u8addressCount,
                                                &UART_EXT_CMD_MS_DAT2,
                                                u8dataCount,
                                                (&UART_EXT_CMD_MS_DAT2)+u8addressCount);
             else                      // I2C write
                 u8Result = MDrv_IIC_WriteBytes( UART_EXT_CMD,
                                                u8addressCount,
                                                &UART_EXT_CMD_MS_DAT2,
                                                u8dataCount,
                                                (&UART_EXT_CMD_MS_DAT2)+u8addressCount);

             if( u8Result )
             {
                 if( UART_EXT_CMD & 0x01 )  // I2C Read
                 {
                    BYTE i;
                     i = _UART_CMD_INDEX5_+u8addressCount;
                     while( u8dataCount > 0 )
                     {
                        CHSCANTEST_MSG(printf("%02bx ", g_UartCommand.Buffer[i]));
                        --u8dataCount;
                        ++i;
                     }
                     CHSCANTEST_MSG(printf("\n"));
                 }
                 else                       // I2C Write
                 {
                    CHSCANTEST_MSG(printf("OK\n"));
                 }
             }
             else
             {
                 CHSCANTEST_MSG(printf("err\n"));
             }
             break;
#endif

#if (INPUT_HDMI_VIDEO_COUNT > 0)
#if (HDCP_KEY_TYPE==HDCP_KEY_IN_24C04 || HDCP_KEY_TYPE==HDCP_KEY_IN_24C64)
         case uartExtDev_EEPROM_HDCP:
         {
             U16 g_wHDCP_KeyChkSum =0;
             switch(UART_EXT_CMD)
             {
                 case URCMD_EEPROM_HDCP_GET_ADDR:  // mothod 1/2
                    g_bDebugProgStopFlag = 0; // pause the program
                    putcharb(0xF3);
                    //putcharb( HIBYTE(HDCP_KEY_ADDR) );
                    //putcharb( LOBYTE(HDCP_KEY_ADDR) );
                    putcharb( HIBYTE(0x00) );
                    putcharb( LOBYTE(0x00) );
                    g_HDCP_KeyCounter = 0;
                    break;
                 case URCMD_EEPROM_HDCP_WRITE:   // mothod 2
                     {
                        BYTE u8Length = (((UART_CMD_EXT_LENGTH&0xF0) >> 4)* 10)+(UART_CMD_EXT_LENGTH&0x0F)+1;
                        msAPI_hdcpkeyBurstWriteBytes(g_HDCP_KeyCounter+HDCP_KEY_ADDR,&(g_UartCommand.Buffer[_UART_CMD_INDEX4_]),u8Length);
                        g_HDCP_KeyCounter+=u8Length;
                     }
                     break;
                 case URCMD_EEPROM_HDCP_WRITE_END: // mothod 2
                     {
                        BYTE u8Length = (((UART_CMD_EXT_LENGTH&0xF0) >> 4)* 10)+(UART_CMD_EXT_LENGTH&0x0F)+1;
                        msAPI_hdcpkeyBurstWriteBytes(g_HDCP_KeyCounter+HDCP_KEY_ADDR,&(g_UartCommand.Buffer[_UART_CMD_INDEX4_]),u8Length);
                        g_HDCP_KeyCounter+=u8Length;
                     }
                     break;
                 case URCMD_EEPROM_HDCP_GET_CHECKSUM:    // mothod 1/2
                    {
                        U16 i;
                        for(i = 0 , g_wHDCP_KeyChkSum=0; i <304; i++)
                        {
                            U8 u8Value;
                            msAPI_hdcpkeyReadByte(i+HDCP_KEY_ADDR,&u8Value);
                            g_wHDCP_KeyChkSum += u8Value;
                        }
                    }
                    putcharb(0xF3);
                    putcharb( HIBYTE(g_wHDCP_KeyChkSum) );
                    putcharb( LOBYTE(g_wHDCP_KeyChkSum) );
                     //msInitHDCPProductionKey();
                    g_bDebugProgStopFlag = 0; // restart the program
                     break;
             } // End of switch(dev_cmd)
         }
         break;

#endif //#if (HDCP_KEY_TYPE==HDCP_KEY_IN_24C04 || HDCP_KEY_TYPE==HDCP_KEY_IN_24C64)
#endif

#ifndef ATSC_SYSTEM
         case uartExtDev_Power_Test:
              #if ENABLE_AUTOTEST
              //MDrv_PWMGT_Excute(UART_EXT_CMD);
              #endif
              break;
         case uartExtDev_USB_Download:
         {
#if ( (ENABLE_USB||ENABLE_USB_2) && ENABLE_FILESYSTEM )
#ifndef MSOS_TYPE_LINUX
            extern U8 MDrv_USBGetPortEnableStatus(void);
            extern void MApp_ZUI_SwUpdate_ProgressBar(U8 percent);
            extern void msAPI_BLoader_Reboot(void);
            U8 u8PortEnStatus = 0;

            printf("USB SW Update!\n");
            u8PortEnStatus = MDrv_USBGetPortEnableStatus();
            if((u8PortEnStatus & BIT0) == BIT0)
            {
                MApp_UsbDownload_Init(BIT0, MApp_ZUI_SwUpdate_ProgressBar);
            }
            else if((u8PortEnStatus & BIT1) == BIT1)
            {
                MApp_UsbDownload_Init(BIT1, MApp_ZUI_SwUpdate_ProgressBar);
            }
            else
            {
                printf("Error> Unknown USB port\n");
                return;
            }

            if(!MW_UsbDownload_Search())
            {
                break;
            }

            if (MW_UsbDownload_Start())
            {
                msAPI_BLoader_Reboot();
            }
#else
    printf("Un-support USB Update\n");
#endif
#endif
         }
              break;
         case uartExtDev_Autotest_Key:
              #if ENABLE_AUTOTEST
              MDrv_Autotest_Excute_Key(UART_EXT_CMD);
              #endif
              break;
#endif
         case uartExtDev_Autotest_Sp:
              #if ENABLE_AUTOTEST
              MDrv_Autotest_Excute_Sp(UART_EXT_CMD);
              #endif
              break;

         default:

             UARTMSG(printf("Unsupport ExtDev %d\n", (U16)UART_EXT_DEV));
             break;
    }
}

#else
// Fixed compiling warning : No one refer to u8Dummy.
//static U8 code u8Dummy;
static MS_U32 uart_debug;
static void MDrv_Empty_UART_RecvHandler(int c)
{
    UNUSED(c);
}
void MDrv_UART_Register_Empty_RecvHandler(void)
{
    uart_debug = mdrv_uart_open(E_UART_AEON);
    mdrv_uart_set_rx_callback(uart_debug, MDrv_Empty_UART_RecvHandler);
}
#endif /* ENABLE_MSTV_UART_DEBUG */


BOOLEAN UART_GetChar(BYTE *u8Char)
{
    BOOLEAN bOK;

    //ES1 = 0;                      // disable uart interrupt
    //_SetInt_SERIAL1(DISABLE);
    if( uartRXByteCount > 0 )                 /* is there any char in buffer   */
    {
        bOK = TRUE;
        *u8Char = uartRXBuf[uartRxQueueTail];        /* fetch the character           */

        uartRxQueueTail++;                    /* adjust it to the next         */

        if( uartRxQueueTail == UART_RXBUF_SIZE)
        {
            uartRxQueueTail = 0x00;                   /* going round                   */
        }
        uartRXByteCount--;                     /* decrement character counter   */
    }
    else
    {
        bOK = FALSE;
    }
    //ES1 = 1;                      // enable uart interrupt
    //_SetInt_SERIAL1(ENABLE);

    return (bOK);

} /* UART_GetChar() */

void UART_Clear(void)
{
    uartRXByteCount   = 0x00;                  /* reset pointers and counter    */
     uartRxQueueHeader = 0x00;
    uartRxQueueTail = 0x00;
}

#if(ENABLE_UART2)
static MS_U32 uart2_debug;
U8 g_Uart2_ack = 0;
static void MDrv_UART2_RecvHandler(int c)
{
    g_Uart2CheckTick = 50;// time-out control ms
    if (g_Uart2Command.Index >= _UART_CMD_LENGTH_)
    {
        g_Uart2Command.Index = 0;
    }
    g_Uart2Command.Buffer[g_Uart2Command.Index] = c; // recieve byte
    g_Uart2_ack = 0xE5;

    g_Uart2Command.Index ++;
    if (g_Uart2Command.Index == 9)// 每个指令9字节.
    {
        g_bUart2Detected     = TRUE; // command  buffer recieve ok
        g_Uart2Command.Index = 0;    // reset index of command buffer
        g_Uart2CheckTick     = 0;
    }
    else if(g_Uart2Command.Buffer[0] == 0x69 && g_Uart2Command.Index == 3)
    {// Mstar 工具指令.
        g_bUart2Detected     = TRUE; // command  buffer recieve ok
        g_Uart2Command.Index = 0;    // reset index of command buffer
        g_Uart2CheckTick     = 0;
    }

    MsOS_EnableInterrupt(E_INT_IRQ_UART1);
}

/******************************************************************************/
///Initial UART2 relative variable and flag.
/******************************************************************************/
void MDrv_UART2_DebugInit(void)
{
    PrintfUart2(printf("MDrv_UART2_DebugInit()\n"));
    // reset Uart variables
    MS_U16 u16Res = 0x00;
    g_Uart2Command.Index = 0;
    g_Uart2CheckTick = 0;

    uart2_debug = mdrv_uart_open(E_UART_PIU_UART1);
    printf("[UART2Init]mdrv_uart_open()>>%u\n", uart2_debug);
    u16Res = mdrv_uart_connect(E_UART_PORT2, E_UART_PIU_UART1);
    PrintfUart2(printf("[UART2Init]mdrv_uart_connect()>>%u\n", u16Res));

    if (uart2_debug)
    {
        u16Res = mdrv_uart_set_baudrate(uart2_debug, 115200);
        PrintfUart2(printf("[UART2Init]mdrv_uart_set_baudrate()>>%u\n", u16Res));
        u16Res = mdrv_uart_rx_enable(uart2_debug);
        PrintfUart2(printf("[UART2Init]mdrv_uart_rx_enable()>>%u\n", u16Res));
        u16Res = mdrv_uart_tx_enable(uart2_debug);
        PrintfUart2(printf("[UART2Init]mdrv_uart_tx_enable()>>%u\n", u16Res));
        u16Res = mdrv_uart_set_rx_callback(uart2_debug, MDrv_UART2_RecvHandler);
        PrintfUart2(printf("[UART2Init]mdrv_uart_set_rx_callback()>>%u\n", u16Res));
    }
    else
    {
        PrintfUart2(printf("[UART2Init]open uart2 fail\n"));
    }

    if(u16Res==0xFE)
    {
        MDrv_UART2_RecvHandler(0x00);
    }
}

void MDrv_UART2_Close(void)
{
    g_Uart2Command.Index = 0;
    memset((void*)g_Uart2Command.Buffer, 0, sizeof(UartCommadType));
    g_Uart2CheckTick = 0;

    if(uart2_debug)
    {
        PrintfUart2(printf("[UART2Close]open uart2 Succeed\n"));
       mdrv_uart_close(uart2_debug);
    }
    else
    {
        PrintfUart2(printf("[UART2Close]open uart2 fail\n"));
    }
}

void MDrv_UART2_SendCommand(MS_U8 *ucCommand, U16 u16Len)
{
    if(uart2_debug)
    {
        mdrv_uart_write(uart2_debug, ucCommand, u16Len);
    }
    else
    {
        PrintfUart2(printf("[UART2SendCommand]open uart2 fail\n"));
    }
}

void MDrv_UART2_Read(MS_U8 *buf, MS_U32 len)
{
    if(uart2_debug)
    {
        PrintfUart2(printf("[UART2Read]open uart2 Succeed\n"));
        mdrv_uart_read(uart2_debug, buf, len);
    }
    else
    {
        PrintfUart2(printf("[UART2Read]open uart2 fail\n"));
    }
}

#if(ENABLE_UART2_TEST)
void MDrv_UART2_TestRXD(void)
{
    if (g_bUart2Detected) // check command flag
    {
        g_bUart2Detected = FALSE;
        // MENU: >> 69 73 23
        // LEFT: >> 69 72 24
        // RIGHT:>> 69 74 22
        // EXIT: >> 69 84 12
        // SOURCE>> 69 75 21

      #if(1)
        printf("\n\n\n\n[UART2] >> ");
        if(g_Uart2Command.Buffer[_UART_CMD_CODE_]>0x0F)  printf("%x ", g_Uart2Command.Buffer[_UART_CMD_CODE_]);  else printf("0%x ", g_Uart2Command.Buffer[_UART_CMD_CODE_]);
        if(g_Uart2Command.Buffer[_UART_CMD_INDEX1_]>0x0F)printf("%x ", g_Uart2Command.Buffer[_UART_CMD_INDEX1_]);else printf("0%x ", g_Uart2Command.Buffer[_UART_CMD_INDEX1_]);
        if(g_Uart2Command.Buffer[_UART_CMD_INDEX2_]>0x0F)printf("%x ", g_Uart2Command.Buffer[_UART_CMD_INDEX2_]);else printf("0%x ", g_Uart2Command.Buffer[_UART_CMD_INDEX2_]);
        if(g_Uart2Command.Buffer[_UART_CMD_INDEX3_]>0x0F)printf("%x ", g_Uart2Command.Buffer[_UART_CMD_INDEX3_]);else printf("0%x ", g_Uart2Command.Buffer[_UART_CMD_INDEX3_]);
        if(g_Uart2Command.Buffer[_UART_CMD_INDEX4_]>0x0F)printf("%x ", g_Uart2Command.Buffer[_UART_CMD_INDEX4_]);else printf("0%x ", g_Uart2Command.Buffer[_UART_CMD_INDEX4_]);
        if(g_Uart2Command.Buffer[_UART_CMD_INDEX5_]>0x0F)printf("%x ", g_Uart2Command.Buffer[_UART_CMD_INDEX5_]);else printf("0%x ", g_Uart2Command.Buffer[_UART_CMD_INDEX5_]);
        if(g_Uart2Command.Buffer[_UART_CMD_INDEX6_]>0x0F)printf("%x ", g_Uart2Command.Buffer[_UART_CMD_INDEX6_]);else printf("0%x ", g_Uart2Command.Buffer[_UART_CMD_INDEX6_]);
        if(g_Uart2Command.Buffer[_UART_CMD_INDEX7_]>0x0F)printf("%x ", g_Uart2Command.Buffer[_UART_CMD_INDEX7_]);else printf("0%x ", g_Uart2Command.Buffer[_UART_CMD_INDEX7_]);
        if(g_Uart2Command.Buffer[_UART_CMD_INDEX8_]>0x0F)printf("%x ", g_Uart2Command.Buffer[_UART_CMD_INDEX8_]);else printf("0%x ", g_Uart2Command.Buffer[_UART_CMD_INDEX8_]);
        printf("\n\n\n\n");
      #endif

        //command normal mode
        if(g_Uart2Command.Buffer[0] == 0x69)
        {
            switch( g_Uart2Command.Buffer[_UART_CMD_INDEX1_] )
            {
                case 0x63:  // Keypad Up
                    u8KeyCode = KEY_UP;
                    u32MonitorOsdTimer = msAPI_Timer_GetTime0();
                    break;
                case 0x72:  // Keypad Left
                    u8KeyCode = KEY_LEFT;
                    u32MonitorOsdTimer = msAPI_Timer_GetTime0();
                    break;
                case 0x73:  // Keypad Menu
                    u8KeyCode = KEY_MENU;
                    u32MonitorOsdTimer = msAPI_Timer_GetTime0();
                    break;
                case 0x74:  // Keypad Right
                    u8KeyCode = KEY_RIGHT;
                    u32MonitorOsdTimer = msAPI_Timer_GetTime0();
                    break;
                case 0x75:  // Keypad Source
                    u8KeyCode = KEY_INPUT_SOURCE;
                    u32MonitorOsdTimer = msAPI_Timer_GetTime0();
                    break;
                case 0x76:  // Keypad Power
                    u8KeyCode = KEY_POWER;
                    u32MonitorOsdTimer = msAPI_Timer_GetTime0();
                    break;
                case 0x83:  // Keypad Down
                    u8KeyCode = KEY_DOWN;
                    u32MonitorOsdTimer = msAPI_Timer_GetTime0();
                    break;

                case 0x84:  // IRKEY_EXIT
                    u8KeyCode = KEY_EXIT;
                    u32MonitorOsdTimer = msAPI_Timer_GetTime0();
                    break;
            }
        }
    }
}

void MDrv_UART2_TestTXD(void)
{
	MS_U8 u8TXDList[8] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};
	MDrv_UART2_SendCommand((MS_U8*)u8TXDList, 8);
    PrintfUart2(printf("[UART2_TestTXD][12345678]>>\n0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38\n"));
}
#endif

#endif


#undef _DRVUART_C_
