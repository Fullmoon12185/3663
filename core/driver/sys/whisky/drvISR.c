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

#define _DRV_ISR_C_

#include <stdio.h>
#include <string.h>
#include "MsTypes.h"

#if defined(__aeon__)
#include "risc32_spr.h"
#endif

#include "sysinfo.h"
#include "drvCPU.h"
#include "sysinfo.h"

#include "hwreg.h"

#include "GPIO.h"
#include "drvpower_if.h"
#include "drvGlobal.h"
#include "drvUartDebug.h"

#include "msIR.h"
#include "msKeypad.h"
#include "drvISR.h"
#include "SysInit.h"
#include "util_minidump.h"
#include "MsOS.h"
#include "drvXC_HDMI_if.h"
#include "apiXC_Hdmi.h"
#include "MApp_MultiTasks.h"

#if ENABLE_TTX
#include "msAPI_TTX.h"
#endif
#if (ENABLE_SUPPORT_MM_PHOTO_4K_2K ==1)
#include "drvUrsa6M40.h"
#endif

#if (ENABLE_SECURITY_R2_HDCP22)
#include "msAPI_SecurityR2.h"
#endif

extern void mdrv_dvi_clock_70mhz_swpatch2(INPUT_SOURCE_TYPE_t enInputSourceType);

#ifdef ENABLE_MINI_DUMP
extern void ProcessWDTIsr(MHAL_SavedRegisters *pHalReg, U32 vector);
extern void ProcessMIUIsr(MHAL_SavedRegisters *pHalReg, U32 vector);
#endif

#if defined(MIPS_CHAKRA) || defined(__AEONR2__)

#if (ENABLE_SECURITY_R2)

// 0_maya_intr_vector_1222.xls
//Host list
//          0: secure_R2
//          1: aeon
//          2: R2
//          3 :None


void drv_HDCPRx_SetR2EventFlag(void);
#endif

#if (ENABLE_SECURITY_R2)
static void ProcessCPU0toCPU2Isr(MHAL_SavedRegisters *pHalReg, U32 vector)
{
    UNUSED(pHalReg);
    UNUSED(vector);
    drv_HDCPRx_SetR2EventFlag();
    MsOS_EnableInterrupt(E_INT_FIQ_HST0TO2);
}
#endif

static void ProcessCPU1toCPU2Isr(MHAL_SavedRegisters *pHalReg, U32 vector)
{
    UNUSED(pHalReg);
    UNUSED(vector);
#if (ENABLE_SECURITY_R2)
	MsOS_EnableInterrupt(E_INT_FIQ_HST1TO2);
#endif
}
#endif

#if (defined(__aeon__) && !defined(__AEONR2__))
static void ProcessCPU0toCPU1Isr(MHAL_SavedRegisters *pHalReg, U32 vector)
{
    UNUSED(pHalReg);
    UNUSED(vector);
#if (ENABLE_SECURITY_R2)
    MsOS_EnableInterrupt(E_INT_FIQ_HST0TO1);
#endif
}

static void ProcessCPU2toCPU1Isr(MHAL_SavedRegisters *pHalReg, U32 vector)
{
    UNUSED(pHalReg);
    UNUSED(vector);

    MsOS_EnableInterrupt(E_INT_FIQ_BEON_TO_AEON);
}
#endif

#if (ENABLE_SUPPORT_MM_PHOTO_4K_2K ==1)
static MS_U32 u32framecounter = 0;
static MS_U8 b4K2KMode = FALSE;
static MS_U32 gopframeslides[4] ={0};

//for 4K2K
#define GOP_REG(bk, reg)                                   (((MS_U32)(bk)<<16) + (reg) * 2)
#define GOP_INTMASK_VS0             BIT(0)
#define GOP_INTMASK_VS1             BIT(1)
#define GOP_INT_VS0                      BIT(8)
#define GOP_INT_VS1                      BIT(9)
#define GOP_WRITE_2_BYTE(BK, REG,VALUE)	 	MDrv_Write2Byte(RIUBASE_GOP + GOP_REG(BK, REG),VALUE)
#define GOP_READ_2_BYTE(bk,REG)	 	MDrv_Read2Byte(RIUBASE_GOP + GOP_REG(bk, REG))
#define MSTAR_DEBUG				printf("\n---------->%s, %d\n",__FUNCTION__,__LINE__);

void MDrv_Photo_Display_Set4K2KMode(BOOLEAN bSetOnOff)
{
	b4K2KMode = bSetOnOff;
}
MS_U8 MDrv_Photo_Display_Get4K2KMode(void)
{
	return b4K2KMode;
}
void MDrv_Photo_Display_Set4K2KPhotoBufAdd(MS_U8 gopframeindex, MS_U32 bufaddr)
{
	if (gopframeindex < sizeof(gopframeslides))
		gopframeslides[gopframeindex] = bufaddr >> 3;
	else
		printf("\n %s, index error!!!\n",__FUNCTION__);
}
MS_BOOL GOP_IntEnable(MS_U32 u32GopIdx, MS_BOOL bEnable)
{
    MS_U16 u16RegVal;

	if (u32GopIdx == 0)
	{
		u16RegVal = GOP_READ_2_BYTE(0, 0x8);
	}
	else
	{
		GOP_WRITE_2_BYTE(0, 0x7F, 0x0203);//SWITCH banks 3
		u16RegVal = GOP_READ_2_BYTE(0, 0x8);
	}

	if(bEnable)
	{
		u16RegVal &= ~GOP_INTMASK_VS0;
	}
	else
	{
		u16RegVal |= (GOP_INTMASK_VS0 | GOP_INTMASK_VS1);
	}

      GOP_WRITE_2_BYTE(0, 0x8, u16RegVal);//GOP Interrupt bank
      GOP_WRITE_2_BYTE(0, 0x7F, 0x0 );//SWITCH original bank

    return TRUE;
}
MS_BOOL GOP_IsVSyncInt(MS_U32 u32GopIdx)
{
    MS_U16 u16RegVal=0;
    MS_U8 oldbank = 0;

	oldbank = GOP_READ_2_BYTE(0, 0x7F)&0xF;

	if (u32GopIdx == 0)
	{
    		u16RegVal = GOP_READ_2_BYTE(0, 0x8);
	}
	else
	{
		GOP_WRITE_2_BYTE(0, 0x7F, 0x0203);//SWITCH 3
		u16RegVal = GOP_READ_2_BYTE(0, 0x8);
	}


    if(u16RegVal & GOP_INT_VS0)
    {
	 GOP_WRITE_2_BYTE(0, 0x7F, 0x0 );//SWITCH original bank
        return TRUE;
    }

	GOP_WRITE_2_BYTE(0, 0x7F, 0x0 );//SWITCH original bank

    return FALSE;
}
static void GOP_Clear_IRQ(void)
{
	MS_U32 u32GopIdx;

    for(u32GopIdx=0; u32GopIdx<2; u32GopIdx++)
    {
        if(GOP_IsVSyncInt(u32GopIdx) == FALSE)
        {
            continue;
        }
        GOP_IntEnable(u32GopIdx, FALSE);
        GOP_IntEnable(u32GopIdx, TRUE);
    }
}

static void GOP_Process_IRQ(void)
{

	MS_U16 u16RegVal = 0;
	MS_U16 u16RegVal1 = 0;

	if (GOP_IsVSyncInt(0) == TRUE && GOP_IsVSyncInt(1)== TRUE)
	{
		GOP_WRITE_2_BYTE(0, 0x7F, 0x0204); //switch to bank 4
		   u16RegVal = GOP_READ_2_BYTE(0, 0x0); //read bank 0
		GOP_WRITE_2_BYTE(0, 1, (gopframeslides[u32framecounter%4]&0xFFFF)); //write low byte of frame address
		GOP_WRITE_2_BYTE(0, 2, ((gopframeslides[u32framecounter%4]>>16)&0xFFFF)); //write high byte of frame address

		u16RegVal |= BIT(0);
		GOP_WRITE_2_BYTE(0, 0, u16RegVal);//Enable GWIN2 (Photo gwin)
		GOP_WRITE_2_BYTE(0, 0x7F, 0x0201);//switch to bank 1 (gop 0)
		u16RegVal = GOP_READ_2_BYTE(0, 0x00); //read bank 0
		u16RegVal1 = GOP_READ_2_BYTE(0, 0x20); //read bank 0
		 if ((u32framecounter) % 4 == MM_PHOTO_4KX2K_OSD_POSITION)
		  {
			u16RegVal |= BIT(0);
			u16RegVal1 |= BIT(0);
			//GOP_WRITE_2_BYTE(0, 0x00, u16RegVal);//Enable UI gwin 0
			GOP_WRITE_2_BYTE(0, 0x20, u16RegVal1);//Enable UI gwin 1
		  }
		  else
		   {
			u16RegVal &= ~BIT(0);
			u16RegVal1 &= ~BIT(0);
			//GOP_WRITE_2_BYTE(0, 0x00, u16RegVal);//Disable UI gwin 0
			GOP_WRITE_2_BYTE(0, 0x20, u16RegVal1);//Disable UI gwin 1
		   }

			if ((u32framecounter) % 4 == 0)
			{
					//mdrv_gpio_set_high( 50 );
				Set4K2K_FIRST_FRAME_FLAG_On();
			}
			else
			{
					//mdrv_gpio_set_low( 50 );
					Set4K2K_FIRST_FRAME_FLAG_Off();
			 }
			u32framecounter++;

			if (u32framecounter == 4)
			{
				u32framecounter = 0;
			}
	}
}


void GOPVsyncIsr(MHAL_SavedRegisters *pHalReg, U32 vector)
{
    UNUSED(pHalReg);
    UNUSED(vector);

	if (b4K2KMode)
	{
	   GOP_Process_IRQ();
	}
		//Clear IRQ
	GOP_Clear_IRQ();

	MsOS_EnableInterrupt(E_INT_IRQ_GOP);
}
#endif


void MDrv_ISR_Init(void)
{

#if defined(MIPS_CHAKRA) || defined(__AEONR2__)
#if (ENABLE_SECURITY_R2)
    MsOS_AttachInterrupt(E_INT_FIQ_HST0TO2, (InterruptCb)ProcessCPU0toCPU2Isr);
    MsOS_EnableInterrupt(E_INT_FIQ_HST0TO2);
#endif
    MsOS_AttachInterrupt(E_INT_FIQ_HST1TO2, (InterruptCb)ProcessCPU1toCPU2Isr);
    MsOS_EnableInterrupt(E_INT_FIQ_HST1TO2);
#endif

#if (defined(__aeon__) && !defined(__AEONR2__))
    MsOS_AttachInterrupt(E_INT_FIQ_HST0TO1, (InterruptCb)ProcessCPU0toCPU1Isr);
    MsOS_AttachInterrupt(E_INT_FIQ_HST2TO1, (InterruptCb)ProcessCPU2toCPU1Isr);
    MsOS_EnableInterrupt(E_INT_FIQ_HST0TO1);
    MsOS_EnableInterrupt(E_INT_FIQ_HST2TO1);
#endif

#ifdef ENABLE_MINI_DUMP
    MsOS_AttachInterrupt(E_INT_FIQ_WDT, (InterruptCb)ProcessWDTIsr);
    MsOS_AttachInterrupt(E_INT_IRQ_MIU, (InterruptCb)ProcessMIUIsr);
#endif
}
extern MS_U32 gsystem_time_ms;

//------------------------------------------------------------------------------------
//extern void Timer_Setting_Register(void *ptCb);
extern void Timer_IRQ_Register(void *ptCb);
extern void msAPI_Timer_1ms_ISR(void);

#ifndef MSOS_TYPE_LINUX
#if ENABLE_TTX
BOOLEAN bTTXInISR=FALSE;
#endif

#if(COMB_2D3D_SWITCH_PATCH)  //For FT unstable Htotal channel in Shanghai 20101013
extern WORD g_Comb1msCounter;
#endif

static void TimerISR(void)
{
    //U32 u32Bak=mfspr(SPR_SR);
    //mtspr (SPR_SR, u32Bak|BIT2 );

    gTimerCount0++;
    gu8100msTH++;

#if (ENABLE_SECURITY_R2_HDCP22)
    if(g_u16HdcpDedicatedTimeMs) g_u16HdcpDedicatedTimeMs--;
#endif

    msAPI_Timer_1ms_ISR();


    // Use a threshold to check the 100 ms. If the threshold
    // is greater or equal than the 100 ms. Increase the 100 ms
    // counter.
    if (gu8100msTH >= 100)
    {
        g100msTimeCount++;
        gu8100msTH = 0;
    }

    if (gTimerCount0%1000==0)
    {
        gSystemTimeCount++;
        gSystemTimeCount_TCON++;

    }

    //--------------------------------
    //down time counter:
    if ( gTimerDownCount0 > 0 )
        gTimerDownCount0--;

    if ( gTimerDownCount1 > 0 )
        gTimerDownCount1--;

#if (IR_MODE_SEL == IR_TYPE_SWDECODE_MODE)
    if(g_ucIrRepeatTimer > 0)
        g_ucIrRepeatTimer--;
#elif (IR_MODE_SEL == IR_TYPE_SWDECODE_KON_MODE)
    if (g_u8IrKonRepeatTimeout)
        g_u8IrKonRepeatTimeout--;
/*
#elif(IR_MODE_SEL == IR_TYPE_SWDECODE_TOSHIBA_MODE||IR_MODE_SEL == IR_TYPE_SWDECODE_CH_MODE)
    if (g_wIRRepeatTimer > 1)
        g_wIRRepeatTimer--;
    if (g_wIRCycleTimer > 1)
        g_wIRCycleTimer--;
*/
#elif(IR_MODE_SEL == IR_TYPE_SWDECODE_RCA_MODE)
       if(g_wIrRcaRemoteTimer > 0)
    {
           g_wIrRcaRemoteTimer--;
    }
       else
    {
        g_ucIrCodeBuf=IRKEY_DUMY;
    }

    if (g_u8IrKeyRepeatCounter>0)
        g_u8IrKeyRepeatCounter--;
/*
#elif (IR_MODE_SEL == IR_TYPE_SWDECODE_NEC_MODE)
    if(g_ucIrRepeatTimer > 0)
        g_ucIrRepeatTimer--;
#elif (IR_MODE_SEL == IR_TYPE_SWDECODE_MITSUBISHI_MODE)
    if(g_ucIrRepeatTimer > 0)
        g_ucIrRepeatTimer--;
#elif (IR_MODE_SEL == IR_TYPE_SWDECODE_PIONEER)
    if(g_u8IRDetectTimer < 0xFF)
        g_u8IRDetectTimer++;
    if(g_u8IRRepeatInvalid < 0xFF)
        g_u8IRRepeatInvalid++;
*/
#endif

#if ((KEYPAD_TYPE_SEL != KEYPAD_TYPE_NONE) && (KEYPAD_USE_NEW_CHECK))
    if(g_ucKeyPadRepeatTimer < 0xFFFF)
        g_ucKeyPadRepeatTimer++;
#endif

    //-------------------------------
#if (ENABLE_UART2)
    if (g_Uart2CheckTick)
    {
        --g_Uart2CheckTick;
        if ( g_Uart2CheckTick == 0 )
        {
            g_bUart2Detected = FALSE;
            g_Uart2Command.Index = 0;
        }
    }
#endif // #if (ENABLE_UART2)

    //-------------------------------

    #if (!BLOADER)
    #if (ENABLE_MSTV_UART_DEBUG )
    if (g_Uart0CheckTick)
    {
        --g_Uart0CheckTick;
        if ( g_Uart0CheckTick == 0 )
        {
            g_bUart0Detected = FALSE;
            g_UartCommand.Index = 0;
        }
    }
    #elif XMODEM_DWNLD_ENABLE //for xmodem
     if ( g_Uart0CheckTick )
    {
        --g_Uart0CheckTick;
        if ( g_Uart0CheckTick == 0 )
        {
            XModem_Rx_Timeout = TRUE;
        }
    }
    #endif
#endif // #if (!BLOADER)

    //-------------------------------

#if (ENABLE_UART1_INTERRUPT == ENABLE)
    #if(ENABLE_UART1_DEBUG)
    if (g_Uart1CheckTick)
    {
        --g_Uart1CheckTick;
        if ( g_Uart1CheckTick == 0 )
        {
            g_bUart1Detected = FALSE;
            g_ucUart1BufIndex = 0;
        }
    }
    #endif
#endif

    #if (!BLOADER)
    #if ENABLE_TTX
    {
        static U8 u8TTXTimer = 0;

        bTTXInISR=TRUE;

        if ((u8TTXTimer++ % 6) == 0)
        msAPI_TTX_CollectPackets();

        bTTXInISR=FALSE;
    }
    #endif
#endif

  #if (!BLOADER)
    if(MApi_XC_HDMI_Func_Caps() & CAPS_DVI_ADJ_70M)
    {
#ifdef DVI_NEW_Patch
    // 20081226 - DVI+HDCP snow noise patch - start ...
        if(g_DVI70MhzPollingCounter)
        {
            g_DVI70MhzPollingCounter--;

            if(g_DVI70MhzPollingCounter == 0)
                mdrv_dvi_clock_70mhz_swpatch2(SYS_INPUT_SOURCE_TYPE(MAIN_WINDOW));
        }
    // 20081226 - DVI+HDCP snow noise patch - end ...
#endif
    }
    #endif

    //mtspr (SPR_SR, u32Bak );
}

void MDrv_Timer_ISR_Register(void)
{
    MS_U32 check_timer_status;

    check_timer_status = MsOS_CreateTimer( (TimerCb) TimerISR,
                                                  0,
                                                  1,
                                                  TRUE,
                                                  "Check timer");
}
#endif

/**************************
System trap function
 vectors.s assumes that this function will never return
****************************/
extern void msAPI_ShowWarningMessage(BOOLEAN bShow, const char *warningMsg);
char g_strTrapFileLineName[256]={0};
void trapBackUpLocation(const char *filename, unsigned long line)
{
    memset(g_strTrapFileLineName, 0, sizeof(g_strTrapFileLineName));
    snprintf(g_strTrapFileLineName, sizeof(g_strTrapFileLineName), "System trap at %s :%u", filename, line);
}
void ProcessSysTrap(MHAL_SavedRegisters *pHalReg, U32 vector)
{
   printf("system trap");
   msAPI_ShowWarningMessage(TRUE, g_strTrapFileLineName);

#ifdef ENABLE_MINI_DUMP
   MDrv_Sys_MiniDump(pHalReg, vector, MINI_DUMP_FOR_SYS_TRAP);
#else
   UNUSED(pHalReg);
   UNUSED(vector);
#endif

   MDrv_Sys_DisableWatchDog();
   MDrv_Sys_StopCpu(1);
}
