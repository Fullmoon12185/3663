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
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#define MAPP_MAIN_C

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

#include "Board.h"
#include "datatype.h"

// Common Definition
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "debug.h"
#include "apiXC.h"
#include "MsTypes.h"
#include "drvBDMA.h"
#include "drvUART.h"
#include "drvUartDebug.h"
#include "msAPI_MIU.h"
#include "msAPI_DrvInit.h"
#include "msAPI_Memory.h"
#include "msAPI_Timer.h"
#include "MApp_MultiTasks.h"
#include "MApp_Main.h"
#include "MApp_Init.h"
#include "MApp_IR.h"
#include "MApp_TopStateMachine.h"
#include "MApp_TV.h"
#include "MApp_Scan.h"
#include "GPIO_macro.h"
#include "MApp_SaveData.h"
#include "MApp_ZUI_ACTmainpage.h"

#include "MApp_InputSource.h"
#include "msIR.h"

#if (ENABLE_SECURITY_R2)
#include "msAPI_SecurityR2.h"
#endif

#if 0//ENABLE_SBTVD_CM_APP
#include "msAPI_ChProc.h"
#include "msAPI_ATVSystem.h"
#endif

#if ENABLE_ATSC_TTS
#include "MApp_TTSControlHandler.h"
#endif
#if HDMITX_4030_ENABLE
#include"Device_hdmitx_4030.h"
#endif

#if(ENABLE_POWERON_MUSIC)
#include "MApp_PowerOnMusic.h"
#endif

#if (ENABLE_FLASH_ON_DRAM)
#include "msAPI_FlashOnDram.h"
#endif

#if GAME_ENABLE
#include "MApp_Game_Main.h"
#endif

#if ENABLE_KEY_TO_SPEECH
#include "MApp_Audio.h"
#include "MApp_KeyToSpeech.h"
#endif

#if ENABLE_AUTOTEST
extern BOOLEAN g_bAutobuildDebug;
#endif

//------------------------------------------------------------------------------
// Locals
//------------------------------------------------------------------------------
#define CURRENT_TESTING 0 //ENABLE
#define     DEBUG_HOME_SHOP(x)  //x

#define     BACKLIGHT_LEVEL_1   30
#define     BACKLIGHT_LEVEL_2   (30 + BACKLIGHT_LEVEL_1)
#define     BACKLIGHT_LEVEL_3   (30 + BACKLIGHT_LEVEL_2)
#define     BACKLIGHT_LEVEL_4   (100 + BACKLIGHT_LEVEL_3) /*  5/11/2018 anh Luat thay doi 60 => 100  */
#define     MAX_COUNT           BACKLIGHT_LEVEL_4

/*
LED 300mA - Panel 32 inch JP
100%    300mA   111     81V
90%     270mA   103     80V
80%     240mA   89      78V
70%     210mA   82      77V
60%     180mA   74      76V
*/
#if (UBC_TV40 == 1)
    #define     SHOP_BACKLIGHT      217
    #define     HOME_BACKLIGHT_1    195
    #define     HOME_BACKLIGHT_2    173
    #define     HOME_BACKLIGHT_3    152
#elif (UBC_TV32 == 1)
    #if(TV32_INCH_LSC == 1)
        #if(TV32_LSC_300mA == 1)
            #define     SHOP_BACKLIGHT      111  //300mA
            #define     HOME_BACKLIGHT_1    100  //280mA
            #define     HOME_BACKLIGHT_2    89   //
            #define     HOME_BACKLIGHT_3    78
        #elif(TV32_LSC_CVT_600mA == 1)
            #define     SHOP_BACKLIGHT      217  //590mA
            #define     HOME_BACKLIGHT_1    195  //550mA
            #define     HOME_BACKLIGHT_2    173  //500mA  
            #define     HOME_BACKLIGHT_3    152  //470mA  
        #endif
    #elif(TV32_315_1A_600mA == 1) 
        #define     SHOP_BACKLIGHT      220   //360mA
        #define     HOME_BACKLIGHT_1    200   //330mA
        #define     HOME_BACKLIGHT_2    180   //300mA
        #define     HOME_BACKLIGHT_3    160   //280mA
    #elif(TV32_315_1A_300mA == 1) 
        #define     SHOP_BACKLIGHT      111   //360mA
        #define     HOME_BACKLIGHT_1    100   //330mA
        #define     HOME_BACKLIGHT_2    89   //300mA
        #define     HOME_BACKLIGHT_3    78   //280mA
    #elif(TV32_PT315 == 1) 
        #define     SHOP_BACKLIGHT      111  //300mA
        #define     HOME_BACKLIGHT_1    100  //280mA
        #define     HOME_BACKLIGHT_2    89   //
        #define     HOME_BACKLIGHT_3    78
    #endif
#endif


static U16 fourKeyPressed = 0;
static U16 countForHomeShop = 0;
static U16 countForHomeShopSaved = 0;
U8 buff_count[2];
U8 keytemp = 0;

HomeShop_FSM_STATE homeshop_state = HOMESHOP_INIT;
//static U32 timeLast = 0;
//static U32 timeCurr = 0;




#ifdef ENABLE_MINI_DUMP
extern void   MiniDump_MountDrive(void);
#endif

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

void MApp_Power_On_Mini_Dump(void)
{
#ifdef ENABLE_MINI_DUMP
    if(!IsStorageInUse())
    {
        MiniDump_MountDrive();
    }
#endif
}

/***************************************************************************************/

#define UART_DEBUG_STOP_MAIN_LOOP   TRUE
#define UART_DEBUG_NORMAL_MAIN_LOOP FALSE

MS_BOOL MApp_PowerOn_Stage_Debug(void)
{
#if (ENABLE_MSTV_UART_DEBUG)
    if ( msAPI_UART_DecodeCommand() )
    {
    #if ( !CHANNEL_SCAN_AUTO_TEST )

      #if ( WATCH_DOG == ENABLE )
        msAPI_Timer_ResetWDT();
      #endif

        return UART_DEBUG_STOP_MAIN_LOOP; // stop main loop for debug

    #endif
    }

    if ( g_bDebugProgStopFlag )
    {
    #if ( WATCH_DOG == ENABLE )
        msAPI_Timer_ResetWDT();
    #endif

        return UART_DEBUG_STOP_MAIN_LOOP;
    }
#endif

    return UART_DEBUG_NORMAL_MAIN_LOOP;
}

/***************************************************************************************/

#define MAIN_FUNC_STATE_DBG(x)  //x
//#define MAIN_FUNC_STATE_DBG(x)  do{ msDebug_ANSI_SetColorText_2(E_FONT_COLOR_BLACK,E_FONT_BACKGROUND_COLOR_PURPLE); x; msDebug_ANSI_AllAttrOffText(); } while(0)


En_MSTAR_MAIN_FUNCTION_STAGE enPower_ON_Stage;
En_MSTAR_MAIN_FUNCTION_STAGE enPrePower_ON_Stage;

En_PRE_INIT_SUB_STATE   enPreInit_Sub_State;
En_WHILE_LOOP_SUB_STATE enWhileLoop_Sub_State;

/***************************************************************************************/

#define PRE_INIT_ONGOING    TRUE
#define PRE_INIT_DONE       FALSE

#ifdef BENCH_CODE_USAGE

#if (CHIP_FAMILY_TYPE == CHIP_FAMILY_MILAN)

#define BENCH_CHIP          (char*)"MILAN"
#define BENCH_BOOTSTATUS    (char*)"BOOT_PASS"
#define BENCH_VERSION       (char*)"V03" //DDR2 Etron U03 setting
#if (MS_BOARD_TYPE_SEL == BD_MST214A_D01A_S_MILAN)
#define BENCH_BOARD         (char*)"214A"
#elif (MS_BOARD_TYPE_SEL == BD_MST214B_D01A_S_MILAN)
#define BENCH_BOARD         (char*)"214B"
#elif (MS_BOARD_TYPE_SEL == BD_MST214C_D01A_S_MILAN)
#define BENCH_BOARD         (char*)"214C"
#elif (MS_BOARD_TYPE_SEL == BD_MST214D_D01A_S_MILAN)
#define BENCH_BOARD         (char*)"214D"
#elif (MS_BOARD_TYPE_SEL == BD_MST074B_10AGS)
#define BENCH_BOARD         (char*)"074B"
#elif (MS_BOARD_TYPE_SEL == BD_MST073B_10AGS)
#define BENCH_BOARD         (char*)"073B"
#else
#error "Please select board type"
#endif

#else
#define BENCH_CHIP          (char*)"CHIP"
#define BENCH_BOOTSTATUS    (char*)"BOOTSTATUS"
#define BENCH_VERSION       (char*)"VERSION"
#define BENCH_BOARD         (char*)"BOARD"
#endif

void MApp_Bench_PowerOffLED(void)
{
  #if (CHIP_FAMILY_TYPE == CHIP_FAMILY_MILAN)
    #if (MS_BOARD_TYPE_SEL == BD_MST214A_D01A_S_MILAN) || (MS_BOARD_TYPE_SEL == BD_MST074B_10AGS)
    mdrv_gpio_set_low(PAD_TCON0);
    #else
    mdrv_gpio_set_low(PAD_GPIO2);
    #endif
  #endif
  printf("\n[BENCH][%s][%s] %s_%s\n",BENCH_CHIP,BENCH_BOARD,BENCH_BOOTSTATUS,BENCH_VERSION);
}
#endif

#define DEBUG_INIT_STATE_NAME(x) //x

MS_BOOL MApp_PreInit_State(void)
{
    MS_BOOL bPreInitDone = PRE_INIT_ONGOING;
    U32 u32Time_StateStart = MsOS_GetSystemTime();
    U32 u32Time_StateEnd;
    U32 u32StateUseTime;
    
    //printf(" npn --- npn \n");
    if( MApp_PowerOn_Stage_Debug() == UART_DEBUG_STOP_MAIN_LOOP )
    {
        return PRE_INIT_ONGOING;
    }


    MAIN_FUNC_STATE_DBG(printf(" ---------- [MApp_PreInit_State %u] start at %u ms\n", enPreInit_Sub_State, u32Time_StateStart));

    CHECK_MIU_PROTECT_LOG_AND_CLEAR();

#if(ENABLE_STACK_CHECK)
    CHECK_STACK();
#endif
    DEBUG_INIT_STATE_NAME(printf("Nguyen Nguyen UI_INPUT_SOURCE_TYPE = %u\n", UI_INPUT_SOURCE_TYPE););
    switch ( enPreInit_Sub_State )
    {
        case EN_PRE_INIT_INITIATION:
            
            break;

        case EN_PRE_INIT_STAGE_AUTOTEST_INIT:
            DEBUG_INIT_STATE_NAME( printf(">> NPN --  MApp_PreInit_Autotest_Init() \n"););
            MApp_PreInit_Autotest_Init();
            break;

        case EN_PRE_INIT_STAGE_GLOBALVAR_INIT:
            DEBUG_INIT_STATE_NAME( printf(">> NPN --  MApp_PreInit_GlobalVariable_Init() \n"););
            MApp_PreInit_GlobalVariable_Init();
            
            break;

        case EN_PRE_INIT_STAGE_POWERSETTING_INIT:
            DEBUG_INIT_STATE_NAME( printf(">> NPN --  MApp_PreInit_Chip_Init() \n"););
            MApp_PreInit_PowerSettingVariable_Init();

        #if(ENABLE_FLASH_ON_DRAM)
            msAPI_FlashOnDram_Init();
        #endif

            break;

        case EN_PRE_INIT_STAGE_CHIP_INIT:
           DEBUG_INIT_STATE_NAME( printf(">> NPN --  MApp_PreInit_Chip_Init() \n"););
            MApp_PreInit_Chip_Init();
            break;

        case EN_PRE_INIT_STAGE_AUDIO_INIT_1:
            DEBUG_INIT_STATE_NAME( printf(">> NPN --  MApp_PreInit_Audio_Init_1() \n"););
            MApp_PreInit_Audio_Init_1();
            break;

        case EN_PRE_INIT_STAGE_CUSTOMER_INFO_INIT:
            DEBUG_INIT_STATE_NAME( printf(">> NPN --  MApp_PreInit_CustomerInfo() \n"););
            MApp_PreInit_CustomerInfo();
            break;

        case EN_PRE_INIT_STAGE_PANEL_BACKLIGHT_OFF:
            DEBUG_INIT_STATE_NAME( printf(">> NPN --  MApp_PreInit_Panel_Backlight_Off() \n"););
            MApp_PreInit_Panel_Backlight_Off();
            break;

        case EN_PRE_INIT_STAGE_READ_DDRPHASE_INIT:
            DEBUG_INIT_STATE_NAME( printf(">> NPN --  MApp_PreInit_ReadDDRPhase() \n"););
            MApp_PreInit_ReadDDRPhase();
            break;

        case EN_PRE_INIT_STAGE_DRIVER_STEP1_INIT:
            PRINT_AUTOTEST_CHECKPT_TIME("start driver init");
            MApp_PreInit_Driver_Step1_Init();
            break;

        case EN_PRE_INIT_STAGE_INTERRUPT_INIT:
            MApp_PreInit_Interrupt_Init();
            break;

        case EN_PRE_INIT_STAGE_DATABASE_INIT:
            DEBUG_INIT_STATE_NAME( printf("EN_PRE_INIT_STAGE_DATABASE_INIT\n"); );
            MApp_PreInit_DateBase_Init();
            //default input source
            //UI_INPUT_SOURCE_TYPE = 10; //(E_UI_INPUT_SOURCE)MApp_Load_Input_Source();
            DEBUG_INIT_STATE_NAME(printf("Nguyen MApp_PreInit_DateBase_Init UI_INPUT_SOURCE_TYPE = %u\n", UI_INPUT_SOURCE_TYPE););
            break;

        case EN_PRE_INIT_STAGE_ATVPROC_AUDIO_INIT:
        //Reduce power-on music playing time.
            PRINT_AUTOTEST_CHECKPT_TIME("start ATVProc_Init_Audio");
            DEBUG_INIT_STATE_NAME(printf("Nguyen MApp_PreInit_ATVProc_Audio_Init UI_INPUT_SOURCE_TYPE = %u\n", UI_INPUT_SOURCE_TYPE););
            MApp_PreInit_ATVProc_Audio_Init();
            DEBUG_INIT_STATE_NAME(printf("Nguyen Nguyen UI_INPUT_SOURCE_TYPE = %u\n", UI_INPUT_SOURCE_TYPE););
            PRINT_AUTOTEST_CHECKPT_TIME("start AUD_Setting");
            MApp_PreInit_Audio_Setting_Init();
            DEBUG_INIT_STATE_NAME(printf("Nguyen MApp_PreInit_Audio_Setting_Init UI_INPUT_SOURCE_TYPE = %u\n", UI_INPUT_SOURCE_TYPE););
            break;

    #if(ENABLE_ATSC_TTS)
        case EN_PRE_INIT_STAGE_TTS_INIT:
            MApp_PreInit_TTS_Init();
            break;
    #endif

    #if(ENABLE_KEY_TO_SPEECH)
        case EN_PRE_INIT_STAGE_KTS_INIT:
             MApp_Audio_Set_AmpMute(AUDIO_AMP_MUTE_OFF);
             MApp_KTS_Say(EN_KTS_POWER_ON);
            break;
    #endif

    #if(ENABLE_POWERON_MUSIC)
        case EN_PRE_INIT_STAGE_POWER_ON_MUSIC_INIT:
            MApp_PreInit_PowerOnMusic_Init();
            break;
    #endif


        case EN_PRE_INIT_STAGE_XC_HDMI_INIT:
            DEBUG_INIT_STATE_NAME( printf("EN_PRE_INIT_STAGE_XC_HDMI_INIT\n"); );
            MApp_PreInit_XC_HDMI_Init();
            
            break;

        case EN_PRE_INIT_STAGE_DRIVER_IR_KEYPAD_INIT:
            DEBUG_INIT_STATE_NAME( printf("EN_PRE_INIT_STAGE_DRIVER_IR_KEYPAD_INIT\n"); );
            MApp_PreInit_Driver_IR_KeyPad_Init();
            break;

        case EN_PRE_INIT_STAGE_CHECK_TO_POWER_DOWN:
            DEBUG_INIT_STATE_NAME( printf("EN_PRE_INIT_STAGE_CHECK_TO_POWER_DOWN\n"); );
            MApp_PreInit_CheckGoToPowerDown();
            break;

        case EN_PRE_INIT_STAGE_DRIVER_STEP2_INIT:
            DEBUG_INIT_STATE_NAME( printf("EN_PRE_INIT_STAGE_DRIVER_STEP2_INIT\n"); );
            MApp_PreInit_Driver_Step2_Init();
            break;

        case EN_PRE_INIT_STAGE_PANEL_INIT:
            DEBUG_INIT_STATE_NAME( printf("MApp_PreInit_Panel_Init()\n"); );
            MApp_PreInit_Panel_Init();
            break;

        case EN_PRE_INIT_STAGE_ENABLE_DCC_DDC2BI_INIT:
            DEBUG_INIT_STATE_NAME(printf(">> NPN --  MApp_PreInit_EnableDCC_DDC2BI_Init() \n"););
            MApp_PreInit_EnableDCC_DDC2BI_Init();
            break;

        case EN_PRE_INIT_STAGE_DATABASE_CHECKING_INIT:
            DEBUG_INIT_STATE_NAME( printf("EN_PRE_INIT_STAGE_DATABASE_CHECKING_INIT\n"); );
            PRINT_AUTOTEST_CHECKPT_TIME("start DB Check");
            MApp_PreInit_DataBase_Checking();
            break;

        case EN_PRE_INIT_STAGE_ADC_INIT:
            DEBUG_INIT_STATE_NAME( printf("EN_PRE_INIT_STAGE_ADC_INIT\n"); );
            MApp_PreInit_ADC_Init();
            break;

        case EN_PRE_INIT_STAGE_SSC_SETTING_INIT:
            DEBUG_INIT_STATE_NAME( printf("EN_PRE_INIT_STAGE_SSC_SETTING_INIT\n"); );
            MApp_PreInit_SSC_Setting_Init();
            break;

        case EN_PRE_INIT_STAGE_TTX_INIT:
            DEBUG_INIT_STATE_NAME( printf("EN_PRE_INIT_STAGE_TTX_INIT\n"); );
            MApp_PreInit_TTX_Init();
            break;

        case EN_PRE_INIT_STAGE_IMAGE_SETMIRROR_INIT:
            DEBUG_INIT_STATE_NAME( printf("EN_PRE_INIT_STAGE_IMAGE_SETMIRROR_INIT\n"); );
            MApp_PreInit_Image_SetMirror();
            break;

        case EN_PRE_INIT_STAGE_UI_INPUTSOURCE_INIT:
            DEBUG_INIT_STATE_NAME( printf("MApp_PreInit_UI_InputSource_Init\n"); );
            MApp_PreInit_UI_InputSource_Init();
            break;

        case EN_PRE_INIT_STAGE_CI_INIT:
            DEBUG_INIT_STATE_NAME( printf("MApp_PreInit_CI_Init\n"); );
            MApp_PreInit_CI_Init();
            break;

        case EN_PRE_INIT_STAGE_DevHDMISWITCH_INIT:
            DEBUG_INIT_STATE_NAME( printf("MApp_PreInit_DevHDMISwitch_Init\n"); );
            MApp_PreInit_DevHDMISwitch_Init();
            break;

        case EN_PRE_INIT_STAGE_DB_CH_INIT:
            DEBUG_INIT_STATE_NAME( printf("MApp_PreInit_DB_CH_Init\n"); );
            MApp_PreInit_DB_CH_Init();
            break;

        case EN_PRE_INIT_STAGE_TIME_SETTING_INIT:
            DEBUG_INIT_STATE_NAME( printf("MApp_PreInit_Time_Setting_Init\n"); );
            MApp_PreInit_Time_Setting_Init();
            break;

        case EN_PRE_INIT_STAGE_EPGTIMER_DATA_INIT:
            DEBUG_INIT_STATE_NAME( printf("MApp_PreInit_EPGTimer_Data_Init\n"); );
            MApp_PreInit_EPGTimer_Data_Init();
            break;

        case EN_PRE_INIT_STAGE_CHECKONTIMER_INIT:
            DEBUG_INIT_STATE_NAME( printf("MApp_PreInit_CheckOnTimer_Init\n"); );
            MApp_PreInit_CheckOnTimer_Init();
            break;

        case EN_PRE_INIT_STAGE_HDMI_HDP_INIT:
            DEBUG_INIT_STATE_NAME( printf("MApp_PreInit_HDMI_HPD_Init\n"); );
            MApp_PreInit_HDMI_HPD_Init();
            break;

        case EN_PRE_INIT_STAGE_LOGO_INIT:
            PRINT_AUTOTEST_CHECKPT_TIME("start Logo");
            DEBUG_INIT_STATE_NAME(printf("MApp_PreInit_Logo_Init()\n"); );
            MApp_PreInit_Logo_Init();
            break;

        case EN_PRE_INIT_STAGE_TUNER_DEMOD_INIT:
            DEBUG_INIT_STATE_NAME(printf("MApp_PreInit_Tuner_And_Demod_Init()\n"); );
            MApp_PreInit_Tuner_And_Demod_Init();
            break;

        case EN_PRE_INIT_STAGE_PRINT_SYSTEM_INFO:
        #if (ENABLE_MSTV_UART_DEBUG)
            dbgVersionMessage();
        #endif
            break;

        case EN_PRE_INIT_STAGE_FONT_INIT:
            DEBUG_INIT_STATE_NAME( printf("EN_PRE_INIT_STAGE_FONT_INIT\n"); );
            PRINT_AUTOTEST_CHECKPT_TIME("start FontInit");
            MApp_PreInit_Font_Init();
            break;

        case EN_PRE_INIT_STAGE_USB_INIT:
            DEBUG_INIT_STATE_NAME( printf("MApp_PreInit_USB_Init\n"); );
            MApp_PreInit_USB_Init();
            firmwareUpgradeViaUSB();
            break;

        case EN_PRE_INIT_STAGE_TUNER_INIT:
            DEBUG_INIT_STATE_NAME( printf("MApp_PreInit_Tuner_Init\n"); );
            MApp_PreInit_Tuner_Init();
            break;

        case EN_PRE_INIT_STAGE_ATV_SYSTEM_INIT:
            PRINT_AUTOTEST_CHECKPT_TIME("start ATVProc_Init_Others");
            DEBUG_INIT_STATE_NAME( printf("MApp_PreInit_ATVSystem_Init\n"); );
            MApp_PreInit_ATVSystem_Init();
            break;

        case EN_PRE_INIT_STAGE_AVD_INIT:
            DEBUG_INIT_STATE_NAME( printf("MApp_PreInit_AVD_Init\n"); );
            MApp_PreInit_AVD_Init();
            break;

        case EN_PRE_INIT_STAGE_OCP_INIT:
            DEBUG_INIT_STATE_NAME( printf("EN_PRE_INIT_STAGE_OCP_INIT\n"); );
            PRINT_AUTOTEST_CHECKPT_TIME("start OCP_Init");
            MApp_PreInit_OCP_Init();
            break;

        case EN_PRE_INIT_STAGE_ENVIRONMENT_INIT:
            DEBUG_INIT_STATE_NAME( printf("EN_PRE_INIT_STAGE_ENVIRONMENT_INIT\n"); );
            PRINT_AUTOTEST_CHECKPT_TIME("start env init");
            MApp_PreInit_Environment_Init();
            break;

    #if(ENABLE_OAD)
        case EN_PRE_INIT_STAGE_OAD_INIT:
            MApp_PreInit_OAD_Init();
            break;
    #endif

        case EN_PRE_INIT_STAGE_LOGO_OFF:
            DEBUG_INIT_STATE_NAME(printf("MApp_PreInit_Logo_Off()\n"); );
            MApp_PreInit_Logo_Off();
            break;

        case EN_PRE_INIT_STAGE_DTV_SYSTEM_INIT:
            //PRINT_AUTOTEST_CHECKPT_TIME("start display step 1 init");
            DEBUG_INIT_STATE_NAME(printf("MApp_PreInit_DTV_System_Init()\n"); );
            MApp_PreInit_DTV_System_Init();
            break;

        case EN_PRE_INIT_STAGE_PVR_INIT:
            DEBUG_INIT_STATE_NAME(printf("MApp_PreInit_PVR_Init()\n"); );    
            MApp_PreInit_PVR_Init();
            break;

        case EN_PRE_INIT_STAGE_BEFORE_CHANGE_SOURCE:
            DEBUG_INIT_STATE_NAME(printf("MApp_PreInit_Before_ChangeSource()\n"); );    
            
            MApp_PreInit_Before_ChangeSource();
            break;

        case EN_PRE_INIT_STAGE_SORCE_CHANGE_INIT:
            DEBUG_INIT_STATE_NAME(printf("MApp_PreInit_Source_Change_Init()\n"); );    
            MApp_PreInit_Source_Change_Init();
            break;

        case EN_PRE_INIT_STAGE_DISPLAY_STEP2_INIT:

            DEBUG_INIT_STATE_NAME(printf("MApp_PreInit_Display_Step2_Init()\n"); );    
            PRINT_AUTOTEST_CHECKPT_TIME("start display step 2 init");
            MApp_PreInit_Display_Step2_Init();
            break;

        case EN_PRE_INIT_STAGE_OTHERS_INIT:
            DEBUG_INIT_STATE_NAME(printf("MApp_PreInit_Others_Init()\n"); );    
            MApp_PreInit_Others_Init();
            break;

        case EN_PRE_INIT_STAGE_DONE:
            MAIN_FUNC_STATE_DBG(printf("    Pre Init DONE! \n"));
            bPreInitDone = PRE_INIT_DONE;    // PreInit Done, go to next MAIN FUNCTION State
            break;
    }

    u32Time_StateEnd = MsOS_GetSystemTime();
    u32StateUseTime = msAPI_Timer_DiffTime_2(u32Time_StateStart, u32Time_StateEnd);

    if( u32StateUseTime > 10 )
    {
        MAIN_FUNC_STATE_DBG(printf(" >>>>> state %u end at %u ms, use %u ms\n\n", enPreInit_Sub_State, u32Time_StateEnd, u32StateUseTime););
    }

    CHECK_MIU_PROTECT_LOG_AND_CLEAR();

#if(ENABLE_CHECK_MEMORY_CHANGE)
    if( FALSE == msDebug_CheckMemChg_Check(enPreInit_Sub_State) )
    {
        PRINT_CURRENT_LINE();
    }
#endif

#if(ENABLE_STACK_CHECK)
    CHECK_STACK();
#endif

#if( ENABLE_ATSC_TTS )
    if( MApp_TTS_Get_Enable() )
    {
        MApp_TTSControlHandler_Main();
    }
#endif

#if(ENABLE_POWERON_MUSIC)
    if( (enPreInit_Sub_State >= EN_PRE_INIT_STAGE_POWER_ON_MUSIC_INIT)
      ||(enPreInit_Sub_State < EN_PRE_INIT_STAGE_SORCE_CHANGE_INIT)
      )
    {
        MApp_PowerOnMusic_Task();
    }
#endif


#if( ENABLE_HDMI_HPD_TASK )
    //Back ground task, handle HDMI HPD hi/low control
    if( MApi_XC_HPD_Is_Task_Working() )
    {
        MApi_XC_HPD_Task();
    }

    /*if( (bPreInitDone == PRE_INIT_DONE) && MApi_XC_HPD_Is_Task_Working() )
    {
        printf("HPD task is working!\n");
        bPreInitDone = PRE_INIT_ONGOING;
    }*/
#endif


    if( enPreInit_Sub_State < EN_PRE_INIT_STAGE_DONE )
    {
        enPreInit_Sub_State ++; // go to next Preinit Sub state
    }

    CHECK_MIU_PROTECT_LOG_AND_CLEAR();

    return bPreInitDone;
}

/***************************************************************************************/

void MApp_While_Loop_State(void)
{
    
    switch (enWhileLoop_Sub_State)
    {
        default:
        {
            printf("\n !!! Wrong enWhileLoop_Sub_State(%d) \n", enWhileLoop_Sub_State);
            break;
        }

        case EN_WHILE_LOOP_INITIATION:
        {
            enWhileLoop_Sub_State++;  // go to next WHILELOOP Sub state
            break;
        }

        case EN_WHILE_LOOP_UART_PAUSE:
        {
            if (MApp_PowerOn_Stage_Debug() == UART_DEBUG_STOP_MAIN_LOOP)
            {
                ;   // stay in this "Uart Pause" state
            }
            else
            {
                enWhileLoop_Sub_State++;  // go to next WHILELOOP Sub state
            }
            break;
        }

        case EN_WHILE_LOOP_MULTITASKS:
        {
    #if GAME_ENABLE
            if(MApp_TopStateMachine_GetTopState() != STATE_TOP_GAME)
    #endif      
                // printf("npn -- MApp_While_Loop_State -- MApp_MultiTasks\n");
                MApp_MultiTasks();

            enWhileLoop_Sub_State++;  // go to next WHILELOOP Sub state
            break;
        }

        case EN_WHILE_LOOP_TOPSTATEMACHINE:
        {
            // printf("npn -- MApp_While_Loop_State -- MApp_TopStateMachine\n");
            MApp_TopStateMachine();

            enWhileLoop_Sub_State++;  // go to next WHILELOOP Sub state
            break;
        }

        case EN_WHILE_LOOP_INTERMEDIATE_MINI_DUMP:
        {
            // printf("npn -- MApp_While_Loop_State -- MApp_Power_On_Mini_Dump\n");    
            MApp_Power_On_Mini_Dump();

            enWhileLoop_Sub_State++;  // go to next WHILELOOP Sub state
            break;
        }

        case EN_WHILE_LOOP_IDLE:
        {   
            enWhileLoop_Sub_State = EN_WHILE_LOOP_INITIATION;
            break;
        }
    }
}

#define MAIN_CHECK_POINT()  //CHECK_MIU_PROTECT_LOG_AND_CLEAR()

int main(void)
{
    // U32 u32MainLoopTime_Last = 0;
    // U32 u32MainLoopTime_Cur = 0;

    //nguyen
    // U32 bToogleLED = 0;
    homeshop_state = HOMESHOP_INIT;
    //U8 pwmValueBackLight = 0;
    //U32 bBacklight= false;

    DEBUG_BOOT_TIME( printf("main() at %u\n", MsOS_GetSystemTime()); );

    enPower_ON_Stage    = EN_MSTAR_MAIN_FUNCTION_POWER_ON_INITIATION;

    enPreInit_Sub_State = EN_PRE_INIT_INITIATION;
    enWhileLoop_Sub_State = EN_WHILE_LOOP_INITIATION;



    while ( 1 )
    {
        enPrePower_ON_Stage = enPower_ON_Stage;

        switch (enPower_ON_Stage)
        {
            default:
            {
                MAIN_FUNC_STATE_DBG(printf("\n !!! Wrong enPower_ON_Stage(%d) \n", enPower_ON_Stage));
                break;
            }

            case EN_MSTAR_MAIN_FUNCTION_POWER_ON_INITIATION:
            {
                MAIN_FUNC_STATE_DBG(printf(" [EN_MSTAR_MAIN_FUNCTION_POWER_ON_INITIATION] at %u\n", MsOS_GetSystemTime()); );

                enPower_ON_Stage++;  // go to next POWER_ON_INIT state
                break;
            }

            case EN_MSTAR_MAIN_FUNCTION_STANDBY2ON:
            {
                MAIN_FUNC_STATE_DBG(printf(" [EN_MSTAR_MAIN_FUNCTION_STANDBY2ON] at %u\n", MsOS_GetSystemTime()); );

                MApp_Power_On_Init_Standby2On();

                enPower_ON_Stage++;  // go to next POWER_ON_INIT state
                break;
            }

            case EN_MSTAR_MAIN_FUNCTION_PRE_INIT:
            {
                if ( MApp_PreInit_State() == PRE_INIT_DONE )
                {
                    MAIN_FUNC_STATE_DBG(printf(" >>>>> Total Pre-Init time use %u ms\n\n", MsOS_GetSystemTime()););
                    enPower_ON_Stage++;  // go to next POWER_ON_INIT state
                }
                else //if ( MApp_PreInit_State() == PRE_INIT_ONGOING )
                {
                    // PreInit not finished yet...
                }

                break;
            }

            case EN_MSTAR_MAIN_FUNCTION_POWERON:
            {
                MAIN_FUNC_STATE_DBG(printf(" [EN_MSTAR_MAIN_FUNCTION_POWERON] at %u\n", MsOS_GetSystemTime()); );

                MAIN_CHECK_POINT();

                MApp_Power_On_Init_On();

                MAIN_CHECK_POINT();

            #if( HDMITX_4030_ENABLE==ENABLE)
                device_hdmitx_SetTmingByPanel(PANEL_DEFAULT_TYPE_SEL);

                MAIN_CHECK_POINT();
            #endif

            #if (ENABLE_SECURITY_R2)
                msAPI_SecurityR2_Start();
                #if (HDCP22_ALL_R2_TASK_ENABLE)
                msAPI_SecurityR2_MISCInit();
                #endif

                MAIN_CHECK_POINT();
            #endif

                enPower_ON_Stage++;  // go to next POWER_ON_INIT state
                break;
            }

            case EN_MSTAR_MAIN_FUNCTION_ENTERING_WHILE_LOOP:
            {
                MAIN_FUNC_STATE_DBG(printf(" [EN_MSTAR_MAIN_FUNCTION_ENTERING_WHILE_LOOP] at %u\n", MsOS_GetSystemTime()));

            #if( ENABLE_FLASH_ON_DRAM && defined(MMAP_FOD_ADR) )
                msAPI_FlashOnDram_Disable();
            #endif

                printf("\n===== Enter main loop at %u\n", MsOS_GetSystemTime());
                //printf("\n===== Timestamp %s, %s\n",__DATE__, __TIME__);
                //MApp_InitGenSetting();
                
                MApp_PreInit_TurnOnPanel_();
              #ifdef BENCH_CODE_USAGE
                MApp_Bench_AudioTest();
                MApp_Bench_PowerOffLED();
              #endif

                enPower_ON_Stage++;  // go to next POWER_ON_INIT state

                break;
            }

            case EN_MSTAR_MAIN_FUNCTION_WHILE_LOOP:
            {
                //MAIN_FUNC_STATE_DBG(printf(" %d: [EN_MSTAR_MAIN_FUNCTION_ENTERING_WHILE_LOOP] \n", __LINE__));

                // u32MainLoopTime_Cur = MsOS_GetSystemTime();
                // if( msAPI_Timer_DiffTime_2(u32MainLoopTime_Last, u32MainLoopTime_Cur) > 3000 )
                // {
                //     u32MainLoopTime_Last = u32MainLoopTime_Cur;
                    
                //     // if(ANDROID_STATUS() == 0){
                //     //     printf("0\n");
                //     // } else {
                //     //     printf("1\n");
                //     // }
                //     //nguyen
                //     if (bToogleLED == 0){
                //         // LED_RED_Off();
                //         // LED_GRN_On(); 
                //         //EX_ACTIVE_IC_ON();
                //         // b_ON();
                //         // r_ON();
                //         // g_ON();
                //         //MApp_PreInit_Logo_Init();
                //         //MApi_PNL_En(TRUE);
                //         //MApi_PNL_SetBackLight(ENABLE);
                //         bToogleLED = 1;    
                //     } else {
                //         // b_OFF();
                //         // g_OFF();
                //         // r_OFF();
                //         // LED_RED_On();
                //         // LED_GRN_Off();
                //         //EX_ACTIVE_IC_OFF();
                //         //MApi_PNL_En(FALSE);
                //         //MApi_PNL_SetBackLight(DISABLE);
                //         //HDMI2_OFF();
                //         bToogleLED = 0;
                //     }
                    
                    
                //     //printf("t=%u\n", u32MainLoopTime_Cur );
                // }
                
                #if(SMART_TV)
                if(isCodeReadyToSend()){
                    MApp_IR_out();        
                } else 
                #endif
                {
                    MApp_While_Loop_State();
                #if(SMART_TV)
                    SendIROut_FSM();
                #endif
                    HomeShop_FSM();
                    //isTVThongminh();    
                }
                
                break;
            }

        }

    }//main while end

    return 0;
}

BOOL MApp_Main_Is_PowerOnPreInitFinish(void)
{
    if( enPower_ON_Stage > EN_MSTAR_MAIN_FUNCTION_PRE_INIT )
        return TRUE;

    return FALSE;
}

BOOL MApp_Main_Is_PowerOnInitFinish(void)
{
    if( enPower_ON_Stage >= EN_MSTAR_MAIN_FUNCTION_WHILE_LOOP )
        return TRUE;

    return FALSE;
}

//nguyen

void update_count_for_home_shop(void){
    if(countForHomeShop < MAX_COUNT){
        countForHomeShop ++;
        MApp_Save_UserDataForHomeShop(countForHomeShop);
        DEBUG_HOME_SHOP(printf("\n*************************************************\n"););
        DEBUG_HOME_SHOP(printf("\nXXXXXXX countForHomeShop %u\n", countForHomeShop););
        DEBUG_HOME_SHOP(printf("\n*************************************************\n"););
        countForHomeShopSaved = MApp_Load_UserDataForHomeShop();
        
        // if(countForHomeShop != countForHomeShopSaved){
        //     MApi_PNL_BackLight_Adjust(HOME_BACKLIGHT);
            DEBUG_HOME_SHOP(printf("\nXXXXXXX countForHomeShopSaved %u\n", countForHomeShopSaved););
        // } else {
        //     MApi_PNL_BackLight_Adjust(HOME_BACKLIGHT_1);      
        // }
        
    } else {
        // DEBUG_HOME_SHOP(printf("\n*************************************************\n"););
        // DEBUG_HOME_SHOP(printf("\ncountForHomeShop %u > NUM_PRESS_TIME_BEFORE_GO_TO_HOME\n", countForHomeShop););
        // DEBUG_HOME_SHOP(printf("\n*************************************************\n"););
    }
    DEBUG_HOME_SHOP(printf("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"););
    DEBUG_HOME_SHOP(printf("\n******homeshop_state %u\n", homeshop_state););
    DEBUG_HOME_SHOP(printf("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"););
    MApp_Save_Input_Source((U8)UI_INPUT_SOURCE_TYPE);
    DEBUG_HOME_SHOP(printf("\n******UI_INPUT_SOURCE_TYPE %u\n", UI_INPUT_SOURCE_TYPE););

}
void HomeShop_FSM (void){

#if CURRENT_TESTING
    
    switch(homeshop_state){
        case HOMESHOP_INIT:
            fourKeyPressed = 1;
            countForHomeShop = 50;
            homeshop_state = SHOP_STATE;
        break;
        case SHOP_STATE:
            if(getKeyPressed() == KEY_VOLUME_PLUS){
                countForHomeShop++;
                MApi_PNL_BackLight_Adjust(countForHomeShop);
                DEBUG_HOME_SHOP(printf("\n*************************************************\n"););
                DEBUG_HOME_SHOP(printf("\nXXXXXXX countForHomeShop %u\n", countForHomeShop););
                DEBUG_HOME_SHOP(printf("\n*************************************************\n"););
            }else if(getKeyPressed() == KEY_VOLUME_MINUS) {
                countForHomeShop--;
                MApi_PNL_BackLight_Adjust(countForHomeShop);
                DEBUG_HOME_SHOP(printf("\n*************************************************\n"););
                DEBUG_HOME_SHOP(printf("\nXXXXXXX countForHomeShop %u\n", countForHomeShop););
                DEBUG_HOME_SHOP(printf("\n*************************************************\n"););
            }
            break;
        case HOME_STATE:
        break;
        default:
        break;
    }
#else
    if(is_key_pressed()){ 
        clear_key_pressed();
        keytemp = getKeyPressed() - KEY_0;
        if(keytemp <= 9){
            fourKeyPressed = (fourKeyPressed<<4)|keytemp;
            DEBUG_HOME_SHOP(printf("\n*************************************************\n"););
            DEBUG_HOME_SHOP(printf("\nFourKeyPressed 0x%x\n", fourKeyPressed););
            DEBUG_HOME_SHOP(printf("\n*************************************************\n"););
            if(fourKeyPressed == 0x1349){
                homeshop_state = SHOP_STATE;
                countForHomeShop = 0;
                MApp_Save_UserDataForHomeShop(countForHomeShop);
                MApi_PNL_BackLight_Adjust(SHOP_BACKLIGHT);
            }
        }
        DEBUG_HOME_SHOP(printf("\n******homeshop_state %u -- keytemp  = %u\n", homeshop_state, keytemp););
    }
    switch(homeshop_state){
        case HOMESHOP_INIT:
            countForHomeShop = MApp_Load_UserDataForHomeShop();
            if(countForHomeShop > MAX_COUNT){
                countForHomeShop = 0;    
                MApp_Save_UserDataForHomeShop(countForHomeShop);
            } 
            if(countForHomeShop <= BACKLIGHT_LEVEL_1){
                MApi_PNL_BackLight_Adjust(SHOP_BACKLIGHT);     
                DEBUG_HOME_SHOP(printf("\nMApi_PNL_BackLight_Adjust(SHOP_BACKLIGHT);  0x%x\n", SHOP_BACKLIGHT););     
            } else if (countForHomeShop <= BACKLIGHT_LEVEL_2){
                MApi_PNL_BackLight_Adjust(HOME_BACKLIGHT_1);
                DEBUG_HOME_SHOP(printf("\nMApi_PNL_BackLight_Adjust(HOME_BACKLIGHT_1);  0x%x\n", HOME_BACKLIGHT_1);); 
            }else if (countForHomeShop <= BACKLIGHT_LEVEL_3){
                MApi_PNL_BackLight_Adjust(HOME_BACKLIGHT_2);
                DEBUG_HOME_SHOP(printf("\nMApi_PNL_BackLight_Adjust(HOME_BACKLIGHT_2);  0x%x\n", HOME_BACKLIGHT_2);); 
            }else if (countForHomeShop <= BACKLIGHT_LEVEL_4){
                MApi_PNL_BackLight_Adjust(HOME_BACKLIGHT_3);
                DEBUG_HOME_SHOP(printf("\nMApi_PNL_BackLight_Adjust(HOME_BACKLIGHT_3);  0x%x\n", HOME_BACKLIGHT_3);); 
            }
            homeshop_state = SHOP_STATE;
        break;
        case SHOP_STATE:
            // if(countForHomeShop >= MAX_COUNT){
            //     homeshop_state = HOME_STATE;  
            //     MApi_PNL_BackLight_Adjust(backLightCompute(countForHomeShop));   
            // }
        break;
        case HOME_STATE:
        break;
        default:
        break;
    }
#endif
    
}

#if(SMART_TV == 1)
#define     ANDROID_STANDBY_MODE     1
#define     ANDROID_ACTIVE_MODE      0
SendIROut_STATE sendirout_state = SEND_IR_OUT_INIT;
SendIROut_STATE sendirhome_standby = SEND_IR_KEY_HOME;
U8 temp_key = 0xff;
static U32 timeLastAndroid = 0;
static U32 timeCurrAndroid = 0;
static U8 timeout_for_sending_powerkey = 0;
void SendIROut_FSM(void){
    if(MApp_InputSrc_Get_UiInputSrcType() == UI_INPUT_SOURCE_HDMI2){
        if(ANDROID_STATUS() == ANDROID_STANDBY_MODE){
            timeCurrAndroid = MsOS_GetSystemTime();
            if(timeout_for_sending_powerkey == 0){
                if( msAPI_Timer_DiffTime_2(timeLastAndroid, timeCurrAndroid) > 500){
                    MApp_IR_sendIROut(IRKEY_POWER);     
                    timeLastAndroid = timeCurrAndroid;   
                    timeout_for_sending_powerkey = 1;
                }
            } else {
                if( msAPI_Timer_DiffTime_2(timeLastAndroid, timeCurrAndroid) > 5000){
                    MApp_IR_sendIROut(IRKEY_POWER);     
                    timeLastAndroid = timeCurrAndroid;   
                }
            }  
        } 
        sendirhome_standby = SEND_IR_KEY_HOME;
    } else {
        if(ANDROID_STATUS() == ANDROID_ACTIVE_MODE){
            timeCurrAndroid = MsOS_GetSystemTime();
            switch(sendirhome_standby){
                case SEND_IR_KEY_HOME:
                    if(msAPI_Timer_DiffTime_2(timeLastAndroid, timeCurrAndroid) > 200){
                        MApp_IR_sendIROut(IRKEY_HOME);
                        sendirhome_standby = SEND_IR_KEY_HOME_1;
                        timeLastAndroid = timeCurrAndroid;   
                    }
                break;
                case SEND_IR_KEY_HOME_1:
                    if( msAPI_Timer_DiffTime_2(timeLastAndroid, timeCurrAndroid) > 500 ){
                        MApp_IR_sendIROut(IRKEY_HOME);
                        timeLastAndroid = timeCurrAndroid;   
                        sendirhome_standby = SEND_IR_KEY_STANDBY;
                    }
                break;
                case SEND_IR_KEY_STANDBY:
                    if( msAPI_Timer_DiffTime_2(timeLastAndroid, timeCurrAndroid) > 1000 ){
                        MApp_IR_sendIROut(IRKEY_POWER);     
                        timeLastAndroid = timeCurrAndroid;   
                        sendirhome_standby = SEND_IR_KEY_STANDBY_1;
                    }
                break;
                case SEND_IR_KEY_STANDBY_1:
                    if( msAPI_Timer_DiffTime_2(timeLastAndroid, timeCurrAndroid) > 5000 ){
                        sendirhome_standby = SEND_IR_KEY_HOME;    
                        timeLastAndroid = timeCurrAndroid;
                    }
                break;
                default:
                    sendirhome_standby = SEND_IR_KEY_HOME;
                break;
            }
            timeout_for_sending_powerkey = 0;
        }
           
    }
}
#endif
void isTVThongminh(void){
    if(MApp_InputSrc_Get_UiInputSrcType() == UI_INPUT_SOURCE_HDMI2){
        IR_ON();
    } else {
        IR_OFF();
    }
    
}
//nguyen

#undef MAPP_MAIN_C

