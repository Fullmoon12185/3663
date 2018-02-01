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
////////////////////////////////////////////////////////////////////////////////
#ifndef _MSBOARD_H_
#define _MSBOARD_H_

#define BOARD_NAME                  "BD_MST084B_10AHB_15043"


//------MHL Setting-----------------------------------------------------------
#define ENABLE_MHL                  ENABLE
#define HDMI_PORT_FOR_MHL           UI_INPUT_SOURCE_HDMI3

//For Support MHL chip
#if (ENABLE_MHL == ENABLE)
#define INPUT_SUPPORT_MHL_PATH      E_INPUT_SUPPORT_MHL_PORT_DVI2
#define MHL_TYPE                    MHL_TYPE_INTERNAL
#else
#define INPUT_SUPPORT_MHL_PATH      E_INPUT_NOT_SUPPORT_MHL
#define MHL_TYPE                    MHL_TYPE_NONE
#endif

//------ HDMI RELATED ---------------------------------------------------------
#define HDCP_HPD_INVERSE            ENABLE

#define HDCP_HPD_2_INVERSE          0
#define HDCP_HPD_3_INVERSE          0
#define HDCP_HPD_4_INVERSE          0

#define HDMI_DRIVER_PACKET_TYPE     E_PACKAGE_TYPE_QFP156_UHD

//------HDMI ARC Contrl-------------------------------------------------------
#define ARC_CTRL_ON()               _FUNC_NOT_USED()
#define ARC_CTRL_OFF()              _FUNC_NOT_USED()
#define ARC_SUPPORT_PORT            4   //connect to UI port4

//-----------------------------------------------------------------------------

#define PIN_FLASH_WP0               0//PIN_133 (Optional, default : 0) => because been used by PWR

#define Unknown_pad_mux             0
#define XC3DLR_PATH1                1
#define XC3DLR_PATH2                2
#define XC3DLR_VSYNC_PATH1          3
#define XC3DLR_VSYNC_PATH2          4
#define TTL_8BIT_MODE               1
#define TTL_6BIT_MODE               1
#define MHL_CBUS_MODE1              1 //PAD_HOTPLUG_A
#define MHL_CBUS_MODE2              2 //PAD_HOTPLUG_B
#define MHL_CBUS_MODE3              3 //PAD_HOTPLUG_C
#define MHL_VBUS_MODE1              1 //PAD_GPIO2
#define MHL_VBUS_MODE2              2 //PAD_GPIO0
#define MHL_VBUS_MODE3              3 //PAD_PWM0
#define MHL_VBUS_MODE4              4 //PAD_PWM1
#define MHL_PORT_MODE_HIGH          1
#define MHL_PORT_MODE_LOW           2
#define DDCDA_ENABLE                1 //PAD_DDCDA_CLK, PAD_DDCDA_DAT
#define DDCDB_ENABLE                1 //PAD_DDCDB_CLK, PAD_DDCDB_DAT
#define DDCDC_ENABLE                1 //PAD_DDCDC_CLK, PAD_DDCDC_DAT
#define DDCDD_ENABLE                1 //PAD_DDCDD_CLK, PAD_DDCDD_DAT
#define I2S_IN_MODE1                1 //PAD_GPIO6, PAD_GPIO8, PAD_GPIO9
#define I2S_OUT_MODE1               1 //PAD_GPIO4, PAD_GPIO6, PAD_GPIO8, PAD_GPIO9, PAD_GPIO7
#define I2S_OUT_MODE2               2 //PAD_GPIO4, PAD_GPIO6, PAD_GPIO8, PAD_GPIO9, PWM1
#define I2S_OUT_MODE3               3 //PAD_GPIO4, PAD_GPIO6, PAD_GPIO8, PAD_GPIO9, PAD_GPIO10
#define I2S_OUT_MODE4               4 //PAD_GPIO4, PAD_GPIO6, PAD_GPIO8, PAD_GPIO9, PAD_INT
#define DDCR_PATH(X)                X // 1 ~ 4
#define IIC_PATH(X)                 X // 1 ~ 4
#define TS_IN_P_MODE(X)             X // 1 ~ 2
#define TS_IN_S_MODE(X)             X // 1 ~ 2
#define TS_OUT_P_MODE(X)            X // 1 ~ 4
#define USB_TCON                    0
#define USB_GPIO                    1
#define USB_PCM2                    2
#define VSYNC_LIKE_MODE(X)          X // 1 ~ 7
#define MSPI0_MODE(X)               X // 1 ~ 2
#define MSPI1_MODE(X)               X // 1 ~ 2

//define the following values from 1
#define PADS_CEC_CONFIG             PAD_CEC //Unknown_pad_mux
#define PADS_TCON_CONFIG            Unknown_pad_mux //TCON0~TCON10
#define PADS_UART2_MODE             Unknown_pad_mux
#define PADS_UART3_MODE             Unknown_pad_mux
#define PADS_UART4_MODE             Unknown_pad_mux
#define PADS_FAST_UART_MODE         Unknown_pad_mux
#define PADS_3DLR_MODE              Unknown_pad_mux
#define PADS_8BIT_TTL               Unknown_pad_mux
#define PADS_6BIT_TTL               Unknown_pad_mux
#if (ENABLE_MHL == ENABLE)
#define PADS_MHL_CBUS               MHL_CBUS_MODE3
#define PADS_MHL_VBUS               Unknown_pad_mux // MHL_VBUS_MODE1
#define PADS_MHL_DEBUG              Unknown_pad_mux //PAD_GPIO0
#define PADS_MHL_DET                PAD_MHL_DET //PAD_MHL_DET
#define PADS_MHL_PORT_CFG           MHL_PORT_MODE_LOW
#else
#define PADS_MHL_CBUS               Unknown_pad_mux
#define PADS_MHL_VBUS               Unknown_pad_mux
#define PADS_MHL_DEBUG              Unknown_pad_mux
#define PADS_MHL_DET                Unknown_pad_mux
#define PADS_MHL_PORT_CFG           Unknown_pad_mux
#endif
#define PADS_DDCDA                  DDCDA_ENABLE
#define PADS_DDCDB                  DDCDB_ENABLE
#define PADS_DDCDC                  DDCDC_ENABLE
#define PADS_DDCDD                  DDCDD_ENABLE
#define PADS_I2S_IN                 Unknown_pad_mux
#define PADS_I2S_OUT                Unknown_pad_mux//I2S_OUT_MODE1
#define PADS_SPDIF_IN               Unknown_pad_mux
#define PADS_SPDIF_OUT              PAD_GPIO5 //PAD_GPIO5,PAD_PWM1,PAD_GPIO10,PAD_INT
#define PADS_INT_GPIO               Unknown_pad_mux
#define PADS_PWM0_MODE              PAD_PWM0
#define PADS_PWM1_MODE              PAD_PWM1
#define PADS_PWM2_MODE              Unknown_pad_mux //PAD_PWM2
#define PADS_PWM3_MODE              Unknown_pad_mux //PAD_PWM3

#if defined(LD_DEMO)&&(LD_DEMO)
#define PADS_PWM4_MODE              PAD_GPIO7
#else
#define PADS_PWM4_MODE              Unknown_pad_mux //PAD_GPIO7
#endif

#define PADS_PWM5_MODE              Unknown_pad_mux //PAD_GPIO6
#define PADS_DDCR_MODE              Unknown_pad_mux
#define PADS_TS_IN_P_CFG            Unknown_pad_mux

#if defined(LD_DEMO)&&(LD_DEMO)
    #define PADS_TS_IN_S_CFG        Unknown_pad_mux
#else
    #define PADS_TS_IN_S_CFG        TS_IN_S_MODE(3)
#endif

#define PADS_TS_OUT_P_CFG           Unknown_pad_mux
#define PADS_USBDRVVBUS             Unknown_pad_mux
#define PADS_VSYNC_LIKE             Unknown_pad_mux
#define PADS_MSPI0                  Unknown_pad_mux

#if defined(LD_DEMO)&&(LD_DEMO)
    #define PADS_MSPI1              MSPI1_MODE(1)
#else
    #define PADS_MSPI1              Unknown_pad_mux
#endif

//------GPIO setting(default GPIO pin level)------------------------------------
#define PIN_65_IS_GPIO             GPIO_OUT_HIGH
#define PIN_94_IS_GPIO             GPIO_OUT_HIGH   //panel_vcc
#define PIN_95_IS_GPIO             GPIO_OUT_HIGH     //diff from socket board
#define PIN_96_IS_GPIO             GPIO_OUT_LOW    //panel_backlight
//#define PIN_98_IS_GPIO             GPIO_OUT_LOW

#define PIN_99_IS_GPIO             GPIO_OUT_LOW    //LR_SYNC diff from socket board

#define PIN_115_IS_GPIO            GPIO_IN
#define PIN_116_IS_GPIO            GPIO_OUT_HIGH

#define PIN_117_IS_GPIO            GPIO_OUT_LOW     //3D_EN diff from socket board

#define PIN_118_IS_GPIO            GPIO_IN          // HDMI0
#define PIN_136_IS_GPIO            GPIO_IN          // HDMI1
#define PIN_137_IS_GPIO            GPIO_IN          // HDMI2
#define PIN_119_IS_GPIO            GPIO_IN          // HDMI3


//------Chip Type---------------------------------------------------------------
#include "chip/MSD91H1BN2.h"

#define  SHARE_GND                ENABLE

//------I2C devices-------------------------------------------------------------
#if USE_SW_I2C
#undef USE_SW_I2C
#define USE_SW_I2C 1
#define USE_SW_I2C_HIGHSPEED        0
#endif

#define I2C_DEV_DATABASE            ((E_I2C_BUS_SYS << 8) | 0xA4)
#define I2C_DEV_HDCPKEY             ((E_I2C_BUS_SYS << 8) | 0xA8)
#define I2C_DEV_EDID_A0             ((E_I2C_BUS_DDCA0 << 8) | 0xA0)
#define I2C_DEV_EDID_D0             ((E_I2C_BUS_DDCD0 << 8) | 0xA0)
#define I2C_DEV_EDID_D1             ((E_I2C_BUS_DDCD1 << 8) | 0xA0)
#define I2C_DEV_EDID_D2             ((E_I2C_BUS_DDCD2 << 8) | 0xA0)

//----------------------------------------------------
#define TUNER_IIC_BUS               E_I2C_BUS_DDCD0
#define DEMOD_IIC_BUS               E_I2C_BUS_DDCD0
#define MHL_IIC_BUS                 E_I2C_BUS_DDCD0

#define DVBS_DEMOD_IIC_BUS          E_I2C_BUS_DDCD0
#define DVBS_TUNER_IIC_BUS          E_I2C_BUS_DDCD0

#define RM_DEVICE_ADR               I2C_DEV_DATABASE
#define RM_HDCP_ADR                 I2C_DEV_HDCPKEY

//------Peripheral Device Setting-----------------------------------------------
#define PANEL_TYPE_SEL                  g_PNL_TypeSel//PNL_AU37_T370HW01_HD //PNL_AU20_T200XW02_WXGA//PNL_LG32_WXGA //PNL_AU19PW01_WXGA//PNL_AU20_T200XW02_WXGA //PNL_LG19_SXGA  //PNL_CMO22_WSXGA  //PNL_AU20_T200XW02_WXGA  // PNL_CMO22_WSXGA  // PNL_AU20_T200XW02_WXGA // PNL_AU17_EN05_SXGA
#define SATURN_FLASH_TYPE               FLASH_TYPE_SERIAL
//#define SATURN_FLASH_IC                 FLASH_IC_MX25L6445E


#ifndef FLASH_SIZE
    #define FLASH_SIZE          FLASH_SIZE_8MB
#else
//    #undef FLASH_SIZE
//    #define FLASH_SIZE          FLASH_SIZE_8MB
#endif

#define ENABLE_DDC_RAM                  ENABLE
#define RM_EEPROM_TYPE                  RM_TYPE_24C64//RM_TYPE_24C512
#define DIGITAL_I2S_SELECT              AUDIO_I2S_NONE

#define INPUT_AV_VIDEO_COUNT            1
#define INPUT_SV_VIDEO_COUNT            0
#define INPUT_YPBPR_VIDEO_COUNT         1
#ifdef ATSC_SYSTEM
#define INPUT_SCART_VIDEO_COUNT         0
#else
#define INPUT_SCART_VIDEO_COUNT         0
#endif
#define INPUT_HDMI_VIDEO_COUNT          4 //  4
#ifdef ATSC_SYSTEM
#define ENABLE_SCART_VIDEO              0
#else
#define ENABLE_SCART_VIDEO              0
#endif
#define INPUT_VGA_COUNT                 1

//------Input Source Mux--------------------------------------------------------
#define INPUT_VGA_MUX               INPUT_PORT_ANALOG1
#define INPUT_VGA_SYNC_MUX          INPUT_PORT_ANALOG1_SYNC //SYNC port of VGA. There is a case which data and sync use different port.
#define INPUT_YPBPR_MUX             INPUT_PORT_ANALOG0
#define INPUT_YPBPR2_MUX            INPUT_PORT_NONE_PORT
#define INPUT_TV_YMUX               INPUT_PORT_YMUX_CVBS0
#define INPUT_AV_YMUX               INPUT_PORT_YMUX_CVBS1
#define INPUT_AV2_YMUX              INPUT_PORT_YMUX_CVBS0 //No AV audio inpu
#define INPUT_AV3_YMUX              INPUT_PORT_NONE_PORT
#define INPUT_SV_YMUX               INPUT_PORT_NONE_PORT
#define INPUT_SV_CMUX               INPUT_PORT_NONE_PORT
#define INPUT_SV2_YMUX              INPUT_PORT_NONE_PORT
#define INPUT_SV2_CMUX              INPUT_PORT_NONE_PORT
#define INPUT_SCART_YMUX            INPUT_PORT_NONE_PORT
#define INPUT_SCART_RGBMUX          INPUT_PORT_ANALOG1
#define INPUT_SCART_FB_MUX          SCART_FB1
#define INPUT_SCART2_YMUX           INPUT_PORT_NONE_PORT
#define INPUT_SCART2_RGBMUX         INPUT_PORT_NONE_PORT
#define INPUT_SCART2_FB_MUX         INPUT_PORT_NONE_PORT
#define INPUT_HDMI1_MUX             INPUT_PORT_DVI0
#define INPUT_HDMI2_MUX             INPUT_PORT_DVI1
#define INPUT_HDMI3_MUX             INPUT_PORT_DVI2
#define INPUT_HDMI4_MUX             INPUT_PORT_DVI3

// For special case
#define INPUT_HDMI3_DDC_PORT        E_XC_PROG_DVI3_EDID
#define INPUT_HDMI4_DDC_PORT        E_XC_PROG_DVI2_EDID

//============================================
// Use ATSC code base audio path setting
//============================================
#define AUDIO_SOURCE_DTV            AUDIO_DSP1_DVB_INPUT
#define AUDIO_SOURCE_DTV2           AUDIO_DSP2_DVB_INPUT
#define AUDIO_SOURCE_ATV            AUDIO_DSP3_SIF_INPUT

#define AUDIO_SOURCE_PC             AUDIO_AUIN0_INPUT
#define AUDIO_SOURCE_YPBPR          AUDIO_AUIN0_INPUT
#define AUDIO_SOURCE_YPBPR2         AUDIO_NULL_INPUT
#define AUDIO_SOURCE_AV             AUDIO_AUIN0_INPUT
#define AUDIO_SOURCE_AV2            AUDIO_NULL_INPUT
#define AUDIO_SOURCE_AV3            AUDIO_NULL_INPUT
#define AUDIO_SOURCE_SV             AUDIO_NULL_INPUT
#define AUDIO_SOURCE_SV2            AUDIO_NULL_INPUT
#define AUDIO_SOURCE_SCART          AUDIO_NULL_INPUT
#define AUDIO_SOURCE_SCART2         AUDIO_NULL_INPUT
#define AUDIO_SOURCE_HDMI           AUDIO_HDMI_INPUT
#define AUDIO_SOURCE_HDMI2          AUDIO_HDMI_INPUT
#define AUDIO_SOURCE_HDMI3          AUDIO_HDMI_INPUT
#define AUDIO_SOURCE_HDMI4          AUDIO_HDMI_INPUT
#define AUDIO_SOURCE_DVI            AUDIO_SOURCE_PC
#define AUDIO_SOURCE_DVI2           AUDIO_SOURCE_PC
#define AUDIO_SOURCE_DVI3           AUDIO_SOURCE_PC
#define AUDIO_SOURCE_DVI4           AUDIO_SOURCE_PC
#define AUDIO_SOURCE_KTV            AUDIO_SOURCE_AV

#define AUDIO_PATH_MAIN_SPEAKER     AUDIO_T3_PATH_AUOUT1
#define AUDIO_PATH_SRC              AUDIO_PATH_NULL
#define AUDIO_PATH_HP               AUDIO_PATH_NULL
#define AUDIO_PATH_MONITOROUT        AUDIO_PATH_NULL
#define AUDIO_PATH_SCART1            AUDIO_PATH_NULL   // always output ATV/DTV sound
#define AUDIO_PATH_SPDIF            AUDIO_PATH_NULL
#define AUDIO_PATH_LINEOUT          AUDIO_PATH_NULL
#define AUDIO_PATH_SIFOUT           AUDIO_PATH_NULL

#define AUDIO_OUTPUT_MAIN_SPEAKER      AUDIO_AUOUT1_OUTPUT
#define AUDIO_OUTPUT_HP                AUDIO_NULL_OUTPUT
#define AUDIO_OUTPUT_MONITOROUT        AUDIO_NULL_OUTPUT
#define AUDIO_OUTPUT_SCART            AUDIO_NULL_OUTPUT  // define NULL when no used
#define AUDIO_OUTPUT_LINEOUT           AUDIO_NULL_OUTPUT
#define AUDIO_OUTPUT_SIFOUT            AUDIO_NULL_OUTPUT

//-----------------------Add GPIO switch setting -------------------------
#define Switch_PC()        _FUNC_NOT_USED()
#define Switch_YPBPR()     _FUNC_NOT_USED()
#define Switch_YPBPR2()    _FUNC_NOT_USED()
#define Switch_AV()        _FUNC_NOT_USED()
#define Switch_AV2()       _FUNC_NOT_USED()
#define Switch_AV3()       _FUNC_NOT_USED()
#define Switch_SV()        _FUNC_NOT_USED()
#define Switch_SV2()       _FUNC_NOT_USED()
#define Switch_SCART()     _FUNC_NOT_USED()
#define Switch_SCART2()    _FUNC_NOT_USED()
#define Switch_DVI()       MApi_XC_DVI_SwitchSrc(INPUT_PORT_DVI0)
#define Switch_DVI2()      MApi_XC_DVI_SwitchSrc(INPUT_PORT_DVI1)
#define Switch_DVI3()      MApi_XC_DVI_SwitchSrc(INPUT_PORT_DVI2)
#define Switch_DVI4()      MApi_XC_DVI_SwitchSrc(INPUT_PORT_DVI3)
#define Switch_DEFAULT()   _FUNC_NOT_USED()
#define Demod_Reset_On()   _FUNC_NOT_USED() //     mdrv_gpio_set_low( PIN_156 )
#define Demod_Reset_Off()  _FUNC_NOT_USED() //     mdrv_gpio_set_high( PIN_156 )
#define USBPowerOn()       _FUNC_NOT_USED()
#define USBPowerOff()      _FUNC_NOT_USED()

// HDMI switch Setting
#define HDMI_SWITCH_SELECT    HDMI_SWITCH_NONE

//------Tuner Setting-----------------------------------------------------------

    #define FRONTEND_TUNER_TYPE                 MxL_661SI_TUNER
    #define FRONTEND_IF_DEMODE_TYPE             MSTAR_INTERN_VIF
    #define TS_SERIAL_OUTPUT_IF_CI_REMOVED      1

#if ENABLE_ATSC
    #define FRONTEND_DEMOD_ATSC_TYPE            EMBEDDED_ATSC_DEMOD
    #define TS_PARALLEL_ATSC_OUTPUT             1 // 0: Serial; 1: paralell
    #define DEMOD_ATSC_USE_UTOPIA               1
#else
    #define FRONTEND_DEMOD_ATSC_TYPE            MSTAR_NONE_DEMOD
    #define TS_PARALLEL_ATSC_OUTPUT             1 // 0: Serial; 1: paralell
    #define DEMOD_ATSC_USE_UTOPIA               0
#endif

#if ENABLE_DVBT
    #define FRONTEND_DEMOD_DVBT_TYPE            MSTAR_MSB124X_DVBT
    #define TS_PARALLEL_DVBT_OUTPUT             0 // 0: Serial; 1: paralell
    #define DEMOD_DVBT_USE_UTOPIA               0
#else
    #define FRONTEND_DEMOD_DVBT_TYPE            MSTAR_NONE_DEMOD
    #define TS_PARALLEL_DVBT_OUTPUT             1 // 0: Serial; 1: paralell
    #define DEMOD_DVBT_USE_UTOPIA               0
#endif

#if( ENABLE_DVB_T2 )
    #define FRONTEND_DEMOD_T2_TYPE              MSTAR_MSB124X_DVBT
    #define TS_PARALLEL_DVBT2_OUTPUT            0 // 0: Serial; 1: paralell
    #define DEMOD_DVBT2_USE_UTOPIA              0
#else
    #define FRONTEND_DEMOD_T2_TYPE              MSTAR_NONE_DEMOD //MSTAR_MSB124X_DVBT2//MSTAR_MSB123X_DVBT2 //
#endif

#if ENABLE_DTMB
    #define FRONTEND_DEMOD_DTMB_TYPE            MSTAR_MSB101M_DTMB_MSD046B//MSTAR_MSB101M_DTMB
    #define TS_PARALLEL_DTMB_OUTPUT             0 // 0: Serial; 1: paralell
    #define DMX_FLOW_INPUT_DTMB                 DMX_FLOW_INPUT_EXT_INPUT0
    #define DEMOD_DTMB_USE_UTOPIA               1
#else
    #define FRONTEND_DEMOD_DTMB_TYPE            MSTAR_NONE_DEMOD
    #define TS_PARALLEL_DTMB_OUTPUT             1 // 0: Serial; 1: paralell
    #define DEMOD_DTMB_USE_UTOPIA               0
#endif

#if ENABLE_DVBC
    #define FRONTEND_DEMOD_DVBC_TYPE            MSTAR_MSB124X_DVBT
    #define TS_PARALLEL_DVBC_OUTPUT             0 // 0: Serial; 1: paralell
    #define DEMOD_DVBC_USE_UTOPIA               0
#else
    #define FRONTEND_DEMOD_DVBC_TYPE            MSTAR_NONE_DEMOD
    #define TS_PARALLEL_DVBC_OUTPUT             1 // 0: Serial; 1: paralell
    #define DEMOD_DVBC_USE_UTOPIA               0
#endif

#if ENABLE_ISDBT
    #define FRONTEND_DEMOD_ISDBT_TYPE           MSTAR_MSB1400_DEMOD
    #define TS_PARALLEL_ISDBT_OUTPUT            0 // 0: Serial; 1: paralell
    #define DEMOD_ISDBT_USE_UTOPIA              1
#else
    #define FRONTEND_DEMOD_ISDBT_TYPE           MSTAR_NONE_DEMOD
    #define TS_PARALLEL_ISDBT_OUTPUT            1 // 0: Serial; 1: paralell
    #define DEMOD_ISDBT_USE_UTOPIA              0
#endif

#define VIF_SAW_ARCH                            6



#if ENABLE_S2
    #define FRONTEND_TUNER_S2_TYPE              AVAILINK_AV2012
    #define FRONTEND_DEMOD_S2_TYPE              MSTAR_MSB124X_DVBT//MSTAR_MSB131X_DVBS2//MSTAR_MSB124X_DVBT
    #define TS_PARALLEL_S2_OUTPUT               0 // 0: Serial; 1: paralell
    #define DISH_TYPE                           DISH_A8304// DISH_A8293 //DISH_DEMOD
    #define RT5006                              0
    #define DEMOD_RESET_PIN_INIT()              //mdrv_gpio_set_high(BALL_K21)
    #define DEMOD_RESET(x)                      (x=x) // For fix compile warning, please look BD_MST157E_C02A_EMERALD.h, old PCB board have this define.
    #define DEMOD_S2_USE_UTOPIA                 0
#else
    #define FRONTEND_DEMOD_S2_TYPE              MSTAR_NONE_DEMOD//MSTAR_MSB131X_DVBS2//MSTAR_MSB124X_DVBT
    #define DEMOD_S2_USE_UTOPIA                 0
#endif


#define D_DMD_SQI_CN_NORDIGP1 \
{ \
    {_QPSK,  _CR1Y2, 5.1 }, \
    {_QPSK,  _CR2Y3, 6.9 }, \
    {_QPSK,  _CR3Y4, 7.9 }, \
    {_QPSK,  _CR5Y6, 8.9 }, \
    {_QPSK,  _CR7Y8, 9.7 }, \
    {_16QAM, _CR1Y2, 10.8}, \
    {_16QAM, _CR2Y3, 13.1}, \
    {_16QAM, _CR3Y4, 12.2}, \
    {_16QAM, _CR5Y6, 15.6}, \
    {_16QAM, _CR7Y8, 16.0}, \
    {_64QAM, _CR1Y2, 16.5}, \
    {_64QAM, _CR2Y3, 16.3}, \
    {_64QAM, _CR3Y4, 17.8}, \
    {_64QAM, _CR3Y4, 21.2}, \
    {_64QAM, _CR5Y6, 21.6}, \
    {_64QAM, _CR7Y8, 22.5}, \
}
#define D_DMD_DVBT_DSPRegInitExt \
{\
     1,\
     0,\
}
#if (VIF_SAW_ARCH==0)
#define D_DMD_DVBT_InitExt \
{ \
    3, \
    0, \
    0xFF, \
    1, \
    (MS_U8)(36167>>24), \
    (MS_U8)(36167>>16), \
    (MS_U8)(36167>>8), \
    (MS_U8)(36167>>0), \
    (MS_U8)(45474>>24), \
    (MS_U8)(45474>>16), \
    (MS_U8)(45474>>8), \
    (MS_U8)(45474>>0), \
    0, \
    0, \
    0, \
    0, \
    5, \
}
#elif (VIF_SAW_ARCH==1)
#define D_DMD_DVBT_InitExt \
{ \
    3, \
    0, \
    0xFF, \
    1, \
    (MS_U8)(36167>>24), \
    (MS_U8)(36167>>16), \
    (MS_U8)(36167>>8), \
    (MS_U8)(36167>>0), \
    (MS_U8)(45474>>24), \
    (MS_U8)(45474>>16), \
    (MS_U8)(45474>>8), \
    (MS_U8)(45474>>0), \
    0, \
    0, \
    0, \
    0, \
    5, \
}
#elif (VIF_SAW_ARCH==2)
#define D_DMD_DVBT_InitExt \
{ \
    3, \
    0, \
    0xFF, \
    1, \
    (MS_U8)(36167>>24), \
    (MS_U8)(36167>>16), \
    (MS_U8)(36167>>8), \
    (MS_U8)(36167>>0), \
    (MS_U8)(45474>>24), \
    (MS_U8)(45474>>16), \
    (MS_U8)(45474>>8), \
    (MS_U8)(45474>>0), \
    0, \
    1, \
    0, \
    0, \
    5, \
}
#elif (VIF_SAW_ARCH==3)
#define D_DMD_DVBT_InitExt \
{ \
    3, \
    0, \
    0xFF, \
    1, \
    (MS_U8)(36167>>24), \
    (MS_U8)(36167>>16), \
    (MS_U8)(36167>>8), \
    (MS_U8)(36167>>0), \
    (MS_U8)(45474>>24), \
    (MS_U8)(45474>>16), \
    (MS_U8)(45474>>8), \
    (MS_U8)(45474>>0), \
    0, \
    0, \
    0, \
    0, \
    5, \
}
#elif (VIF_SAW_ARCH==4)
#define D_DMD_DVBT_InitExt \
{ \
    3, \
    0, \
    0xFF, \
    1, \
    (MS_U8)(36167>>24), \
    (MS_U8)(36167>>16), \
    (MS_U8)(36167>>8), \
    (MS_U8)(36167>>0), \
    (MS_U8)(45474>>24), \
    (MS_U8)(45474>>16), \
    (MS_U8)(45474>>8), \
    (MS_U8)(45474>>0), \
    0, \
    1, \
    1, \
    0, \
    5, \
}
#elif (VIF_SAW_ARCH==5)
#define D_DMD_DVBT_InitExt \
{ \
    3, \
    0, \
    0xFF, \
    1, \
    (MS_U8)(36167>>24), \
    (MS_U8)(36167>>16), \
    (MS_U8)(36167>>8), \
    (MS_U8)(36167>>0), \
    (MS_U8)(45474>>24), \
    (MS_U8)(45474>>16), \
    (MS_U8)(45474>>8), \
    (MS_U8)(45474>>0), \
    0, \
    0, \
    0, \
    0, \
    5, \
}
#elif (VIF_SAW_ARCH==6)
#define D_DMD_DVBT_InitExt \
{ \
    3, \
    0, \
    0xFF, \
    1, \
    (MS_U8)(36167>>24), \
    (MS_U8)(36167>>16), \
    (MS_U8)(36167>>8), \
    (MS_U8)(36167>>0), \
    (MS_U8)(45474>>24), \
    (MS_U8)(45474>>16), \
    (MS_U8)(45474>>8), \
    (MS_U8)(45474>>0), \
    0, \
    1, \
    1, \
    0, \
    5, \
}
#else
#define D_DMD_DVBT_InitExt NULL
#print "BOARD_SAW_TYPE Error"
#endif
#define D_DMD_SAR_CHANNEL 0xFF

//------------------------------------------------------------------------------------
//FRONTEND_DEMO_TUNER_IIC_TYPE
//------------------------------------------------------------------------------------
#define TWO_ROAD_SEPARATE       1
#define ONE_ROAD_BUS            2
#define DEMO_TO_TUNER           3

//------Tuner Demo IIC Setting-----------------------------------------------------------
#define DEMO_TUNER_IIC_TYPE             TWO_ROAD_SEPARATE //ONE_ROAD_BUS //DEMO_TO_TUNER

#define FRONTEND_SECOND_DEMOD_TYPE      MSTAR_NONE_DEMOD

#if (FRONTEND_TUNER_TYPE == NXP_TDA18273_TUNER)
#define VIF_SAW_ARCH                    6                   // 0: Dual SAW; 1: external Single SAW; 2:silicon tuner; 3: no saw(VIF); 4: internal single SAW; 5: no SAW(DIF)
#define VIF_TUNER_TYPE                  1                   // 0: RF Tuner; 1: Silicon Tuner

#define DEMOD_IF_KHz                    4750
#define DEMOD_IQSWAP                    1
#elif (FRONTEND_TUNER_TYPE == NXP_TDA18274_TUNER)
#define VIF_SAW_ARCH                    6                   // 0: Dual SAW; 1: external Single SAW; 2:silicon tuner; 3: no saw(VIF); 4: internal single SAW; 5: no SAW(DIF)
#define VIF_TUNER_TYPE                  1                   // 0: RF Tuner; 1: Silicon Tuner

#define DEMOD_IF_KHz                    4500
#define DEMOD_IQSWAP                    1
#elif 0//(FRONTEND_TUNER_TYPE == PANASONIC_ENV56U02D8F)
#define VIF_SAW_ARCH                    3// 6                   // 0: Dual SAW; 1: external Single SAW; 2:silicon tuner; 3: no saw(VIF); 4: internal single SAW; 5: no SAW(DIF)
#define VIF_TUNER_TYPE                  0                   // 0: RF Tuner; 1: Silicon Tuner

#define DEMOD_IF_KHz                    44000
#define DEMOD_IQSWAP                    0
#elif (FRONTEND_TUNER_TYPE == TCL_DA91WT_13_E_TUNER)
#define VIF_SAW_ARCH                    3                   // 0: Dual SAW; 1: external Single SAW; 2:silicon tuner; 3: no saw(VIF); 4: internal single SAW; 5: no SAW(DIF)
#define VIF_TUNER_TYPE                  0                   // 0: RF Tuner; 1: Silicon Tuner

#define DEMOD_IF_KHz                    44000
#define DEMOD_IQSWAP                    0
#elif 0//(FRONTEND_TUNER_TYPE == RAFAEL_RT810_TUNER)
#define VIF_SAW_ARCH                    6// 3                   // 0: Dual SAW; 1: external Single SAW; 2:silicon tuner; 3: no saw(VIF); 4: internal single SAW; 5: no SAW(DIF)
#define VIF_TUNER_TYPE                  1                   // 0: RF Tuner; 1: Silicon Tuner

#define DEMOD_IF_KHz                    4063
#define DEMOD_IQSWAP                    0
#elif 0//(FRONTEND_TUNER_TYPE == XUGUANG_MDVT8BCW41F2_TUNER)
#define VIF_SAW_ARCH                    3// 6                   // 0: Dual SAW; 1: external Single SAW; 2:silicon tuner; 3: no saw(VIF); 4: internal single SAW; 5: no SAW(DIF)
#define VIF_TUNER_TYPE                  0                   // 0: RF Tuner; 1: Silicon Tuner

#define DEMOD_IF_KHz                    44000
#define DEMOD_IQSWAP                    0
#elif 0//(FRONTEND_TUNER_TYPE == MAXLINEAR_MXL601_TUNER)
#define VIF_SAW_ARCH                    6                   // 0: Dual SAW; 1: external Single SAW; 2:silicon tuner; 3: no saw(VIF); 4: internal single SAW; 5: no SAW(DIF)
#define VIF_TUNER_TYPE                  1                   // 0: RF Tuner; 1: Silicon Tuner

#define DEMOD_IF_KHz                    5000
#define DEMOD_IQSWAP                    0
#elif (FRONTEND_TUNER_TYPE == MxL_661SI_TUNER)
#define VIF_SAW_ARCH                    6                   // 0: Dual SAW; 1: external Single SAW; 2:silicon tuner; 3: no saw(VIF); 4: internal single SAW; 5: no SAW(DIF)
#define VIF_TUNER_TYPE                  1                   // 0: RF Tuner; 1: Silicon Tuner

#define DEMOD_IF_KHz                    5000
#define DEMOD_IQSWAP                    0
#elif (FRONTEND_TUNER_TYPE == RAFAEL_R840_TUNER/*RAFAEL_R840_TUNER*/)
#define VIF_SAW_ARCH                    6                   // 0: Dual SAW; 1: external Single SAW; 2:silicon tuner; 3: no saw(VIF); 4: internal single SAW; 5: no SAW(DIF)
#define VIF_TUNER_TYPE                  1                   // 0: RF Tuner; 1: Silicon Tuner

#define DEMOD_IF_KHz                    5000
#define DEMOD_IQSWAP                    1
#elif (FRONTEND_TUNER_TYPE == SILAB_2158_TUNER)
#define VIF_SAW_ARCH                    6                   // 0: Dual SAW; 1: external Single SAW; 2:silicon tuner; 3: no saw(VIF); 4: internal single SAW; 5: no SAW(DIF)
#define VIF_TUNER_TYPE                  1                   // 0: RF Tuner; 1: Silicon Tuner

#define DEMOD_IF_KHz                    6000
#define DEMOD_IQSWAP                    0
#elif (FRONTEND_TUNER_TYPE == RDA5158_TUNER)
#define VIF_SAW_ARCH                    6                   // 0: Dual SAW; 1: external Single SAW; 2:silicon tuner; 3: no saw(VIF); 4: internal single SAW; 5: no SAW(DIF)
#define VIF_TUNER_TYPE                  1                   // 0: RF Tuner; 1: Silicon Tuner

#define DEMOD_IF_KHz                    5000
#define DEMOD_IQSWAP                    1
#else
#error "TUNER not support"
#endif

#define ATSC_DEMOD_VIF_IN                           1

#define MSB1210_TS_SERIAL_INVERSION                 0
#define MSB1210_TS_PARALLEL_INVERSION               1
#define MSB1210_DTV_DRIVING_LEVEL                   1 //0 or 1
#define MSB1210_WEAK_SIGNAL_PICTURE_FREEZE_ENABLE   1

#define INTERN_DVBT_TS_SERIAL_INVERSION                 0
#define INTERN_DVBT_TS_PARALLEL_INVERSION               1
#define INTERN_DVBT_DTV_DRIVING_LEVEL                   1
#define INTERN_DVBT_WEAK_SIGNAL_PICTURE_FREEZE_ENABLE   1

#define SECAM_L_PRIME_ON()              _FUNC_NOT_USED()
#define SECAM_L_PRIME_OFF()             _FUNC_NOT_USED()
#define EXT_RF_AGC_ON()                 _FUNC_NOT_USED() // ATV mode: external RF AGC
#define EXT_RF_AGC_OFF()                _FUNC_NOT_USED() // DTV mode: internal RF AGC

//------IR & Key Setting--------------------------------------------------------
#define IR_TYPE_SEL                     IR_TYPE_MSTAR_DTV   // IR_TYPE_MSTAR_DTV // IR_TYPE_CUS03_DTV // IR_TYPE_NEW
#define KEYPAD_TYPE_SEL                 KEYPAD_TYPE_ORIG    // KEYPAD_TYPE_DEMO
#define POWER_KEY_SEL                   POWER_KEY_PAD_INT

//------Power Setting-----------------------------------------------------------
#define ENABLE_POWER_SAVING             0
#define POWER_DOWN_SEQ                  1
#define POWER_SAVING_T                  0
#define SCREENSAVER_ENABLE              1
#define NO_SIGNAL_AUTO_SHUTDOWN         1
#define STANDBY_MODE                    POWERMODE_S3
#define POWERUP_MODE                    PUMODE_WORK
#define ENABLE_POWER_GOOD_DETECT        1
#define ENABLE_POWER_SAVING_SIF         1
#define ENABLE_POWER_SAVING_VDMVD       0
#define ENABLE_POWER_SAVING_DPMS        1
//#define ENABLE_POWER_SAVING_DPMS_DVI    1

#define ENABLE_PWS                      1   // Analog IP PWS
#define ENABLE_DIP_PWS                  0   //Digital IP PWS, for U3 only now
#define ENABLE_DIP_MONITOR              0   //For U3 oly now


//------Memory Setting----------------------------------------------------------
#define BOOTUP_MIU_BIST                 1
#ifndef MEMORY_MAP
#define MEMORY_MAP                      MMAP_128MB
#endif
#define  MIU_INTERFACE                  DDR2_INTERFACE_BGA   //DDR3_INTERFACE_BGA

//------Analog Function Setting-------------------------------------------------
#define MOD_LVDS_GPIO                   0x820

#define LVDS_PN_SWAP_L                  0x00
#define LVDS_PN_SWAP_H                  0x00

#define ENABLE_SSC                      DISABLE
#define ENABLE_LVDSTORGB_CONVERTER      DISABLE
#if ENABLE_SSC
#define MIU_SSC_SPAN_DEFAULT            30
#define MIU_SSC_STEP_DEFAULT            1
#define MIU_SSC_SPAN_MAX                40
#define MIU_SSC_STEP_MAX                20
#define LVDS_SSC_SPAN_DEFAULT           300
#define LVDS_SSC_STEP_FHD_DEFAULT       100
#define LVDS_SSC_STEP_HD_DEFAULT        100//50
#define LVDS_SSC_SPAN_MAX               600
#define LVDS_SSC_SPAN_MIN               300
#define LVDS_SSC_STEP_MAX               200
#define LVDS_SSC_STEP_DEFAULT           LVDS_SSC_STEP_FHD_DEFAULT
#endif

//------ETHNET PHY_TYPE---------------------------------------------------------
#define ETHNET_PHY_LAN8700              0x0f
#define ETHNET_PHY_IP101ALF             0x01
#define ETHNET_PHY_TYPE                 ETHNET_PHY_IP101ALF

//------DRAM Config---------------------------------------------------------------
#define DRAM_TYPE                       DDR_II
#define DRAM_BUS                        DRAM_BUS_16
#define DDRPLL_FREQ                     DDRLLL_FREQ_400
#define DDRII_ODT

#define MIU_0_02                        0x0C45
#define MIU_0_1A                        0x5151
#define MIU_0_36                        0x0244
#define MIU_0_38                        0x0070


#define TV_FREQ_SHIFT_CLOCK             DISABLE
//------MCU use Scaler internal MPLL clock-------------------
#define MCU_CLOCK_SEL                   MCUCLK_144MHZ

#define MST_XTAL_CLOCK_HZ               FREQ_12MHZ
#define MST_XTAL_CLOCK_KHZ              (MST_XTAL_CLOCK_HZ/1000UL)
#define MST_XTAL_CLOCK_MHZ              (MST_XTAL_CLOCK_KHZ/1000UL)

//------MCU Code----------------------------------------------------------------
#define ENABLE_HKMCU_ICACHE_BYPASS      0
#define ENABLE_HKMCU_CODE_ECC           0

//------Extra-------------------------------------------------------------------
#define POWER_DOWN_INFORM_EXTERNALMCU   0

#if POWER_DOWN_INFORM_EXTERNALMCU
#define EXMCU_SLAVE_ADDR                0xA8
#define EXMCU_SUBADDRESS                0x04
#define EXMCU_SLEEP_MODE                0x00
#endif

#define IIC_BY_HW                       0 //
#define IIC_BY_SW                       1 //
#define _EEPROM_ACCESS                  IIC_BY_SW//IIC_BY_HW
#define EEPROM_CLK_SEL                  EEPROM_CLK_100KHZ

//------MST I/O control definition----------------------------------------------
#define ENABLE_DPWM_FUNCTION            0

//-------------------------------------------------
#define SCART_OUT_ON()                  _FUNC_NOT_USED()
#define SCART_OUT_OFF()                 _FUNC_NOT_USED()

#define SET_STB_CHECK_LOW               _FUNC_NOT_USED()
#define SET_STB_CHECK_HIGH              _FUNC_NOT_USED()
#define SET_UNDERDRIVE_HIGH()           _FUNC_NOT_USED()
#define SET_UNDERDRIVE_LOW()            _FUNC_NOT_USED()

// Video switch Setting
#define Switch_YPbPr1()                 _FUNC_NOT_USED()
#define Switch_YPbPr2()                 _FUNC_NOT_USED()

#define SwitchRGBToSCART()              _FUNC_NOT_USED()
#define SwitchRGBToDSUB()               _FUNC_NOT_USED()

#define MDrv_Sys_GetUsbOcdN()           _FUNC_NOT_USED()
#define MDrv_Sys_GetRgbSw()             _FUNC_NOT_USED()
//MHL
#define MHL_Reset_High()               _FUNC_NOT_USED()
#define MHL_Reset_Low()                _FUNC_NOT_USED()

// Audio Amplifier
#define Audio_Amplifier_ON()            _FUNC_NOT_USED()//mdrv_gpio_set_high( PIN_97 )
#define Audio_Amplifier_OFF()           _FUNC_NOT_USED()//mdrv_gpio_set_low( PIN_97 )
#define Adj_Volume_On()                 mdrv_gpio_set_low( PIN_95 )
#define Adj_Volume_Off()                mdrv_gpio_set_high( PIN_95 )

//3D EN
#define Set_3D_ON()                     mdrv_gpio_set_high( PIN_117 )
#define Set_3D_OFF()                    mdrv_gpio_set_low( PIN_117 )

//3D EN
#define Set_Panel_PWM0_ON()             mdrv_gpio_set_high( PIN_99)
#define Set_Panel_PWM0_OFF()            mdrv_gpio_set_high( PIN_99)

#define Panel_VCC_ON()                  mdrv_gpio_set_high( PIN_94 )
#define Panel_VCC_OFF()                 mdrv_gpio_set_low( PIN_94 )
#define Panel_Backlight_VCC_ON()        mdrv_gpio_set_high( PIN_96 )
#define Panel_Backlight_VCC_OFF()       mdrv_gpio_set_low( PIN_96 )


#define HDMI1_INPUT_DETECT()            mdrv_gpio_get_level(PIN_118)     // HDMI0
#define HDMI2_INPUT_DETECT()            mdrv_gpio_get_level(PIN_136)     // HDMI1
#define HDMI3_INPUT_DETECT()            mdrv_gpio_get_level(PIN_137)     // HDMI2
#define HDMI4_INPUT_DETECT()            mdrv_gpio_get_level(PIN_119)     // HMDI3

#define Panel_Backlight_PWM_ADJ(x)      MDrv_PWM_DutyCycle(E_PWM_CH1, x)
#define Panel_Backlight_Max_Current(x)  MDrv_PWM_DutyCycle(E_PWM_CH1, x)

#define Panel_VG_HL_CTL_ON()            _FUNC_NOT_USED()
#define Panel_VG_HL_CTL_OFF()           _FUNC_NOT_USED()

// Power Saving
#define Power_On()                      _FUNC_NOT_USED()
#define Power_Off()                     _FUNC_NOT_USED()
#define MDrv_Sys_GetSvideoSw()          _FUNC_NOT_USED()

#define Peripheral_Device_Reset_ON()    _FUNC_NOT_USED()
#define Peripheral_Device_Reset_OFF()   _FUNC_NOT_USED()
#define Tuner_ON()                      _FUNC_NOT_USED()
#define Tuner_OFF()                     _FUNC_NOT_USED()
#define Demodulator_ON()                _FUNC_NOT_USED()
#define Demodulator_OFF()               _FUNC_NOT_USED()
#define LAN_ON()                        _FUNC_NOT_USED()
#define LAN_OFF()                       _FUNC_NOT_USED()

#define TunerOffPCMCIA()                _FUNC_NOT_USED()
#define TunerOnPCMCIA()                 _FUNC_NOT_USED()

// LED Control
#define LED_RED_ON()                    _FUNC_NOT_USED()
#define LED_RED_OFF()                   _FUNC_NOT_USED()
#define LED_GREEN_ON()                  _FUNC_NOT_USED()
#define LED_GREEN_OFF()                 _FUNC_NOT_USED()
#define LED_SEC_RED_ON()                _FUNC_NOT_USED()
#define LED_SEC_RED_OFF()               _FUNC_NOT_USED()

#define ST_DET_Read()                   0
#define ANT_5V_MNT_Read()               0
#define TU_ERROR_N_Read()               0
#define HDMI_5V_Read()                  0
#define COMP_SW_Read()                  1
#define PANEL_CTL_Off()                 Panel_VCC_OFF()
#define PANEL_CTL_On()                  Panel_VCC_ON()
#define INV_CTL_Off()                   Panel_Backlight_VCC_OFF()
#define INV_CTL_On()                    Panel_Backlight_VCC_ON()
#define POWER_ON_OFF1_On()              Power_On()
#define POWER_ON_OFF1_Off()             Power_Off()
#define MUTE_On()                       Adj_Volume_Off()
#define MUTE_Off()                      Adj_Volume_On()
#define EEPROM_WP_On()                  _FUNC_NOT_USED()
#define EEPROM_WP_Off()                 _FUNC_NOT_USED()
#define LED_GRN_Off()                   LED_GREEN_OFF()
#define LED_GRN_On()                    LED_GREEN_ON()
#define LED_RED_Off()                   LED_GRN_On()
#define LED_RED_On()                    LED_GRN_Off()
#define ANT_5V_CTL_Off()                _FUNC_NOT_USED()
#define ANT_5V_CTL_On()                 _FUNC_NOT_USED()
#define BOOSTER_Off()                   _FUNC_NOT_USED()
#define BOOSTER_On()                    _FUNC_NOT_USED()
#define RGB_SW_On()                     _FUNC_NOT_USED()
#define RGB_SW_Off()                    _FUNC_NOT_USED()
#define SC_RE1_On()                     _FUNC_NOT_USED()
#define SC_RE1_Off()                    _FUNC_NOT_USED()
#define SC_RE2_On()                     _FUNC_NOT_USED()
#define SC_RE2_Off()                    _FUNC_NOT_USED()
#define TU_RESET_N_On()                 _FUNC_NOT_USED()
#define TU_RESET_N_Off()                _FUNC_NOT_USED()
#define DeactivateScartRecord1()        _FUNC_NOT_USED()
#define ActivateScartRecord1()          _FUNC_NOT_USED()
#define DeactivateScartRecord2()        _FUNC_NOT_USED()
#define ActivateScartRecord2()          _FUNC_NOT_USED()
#define USBPowerOn()                    _FUNC_NOT_USED()
#define USBPowerOff()                   _FUNC_NOT_USED()

#define UART_SEL(x)                     _FUNC_NOT_USED()

//------HDMI ARC Contrl-------------------------------------------------------
#define ARC_CTRL_ON()                   _FUNC_NOT_USED()
#define ARC_CTRL_OFF()                  _FUNC_NOT_USED()


#define ComponentDetect()               TRUE//_FUNC_NOT_USED()
//AV1
#define CVBSDetect()                    TRUE//_FUNC_NOT_USED()
#define CVBS1Detect()                   TRUE//(mdrv_gpio_get_level(PIN_105))//AV2
#define HDMIDetect()                    TRUE//_FUNC_NOT_USED()
#define HDMI2Detect()                   TRUE//_FUNC_NOT_USED()
#define HPDetect()                      TRUE//_FUNC_NOT_USED()
//------MST Keypad definition---------------------------------------------------
#define KEYPAD_CHANNEL_SUPPORT          4 //Maximun supported keypad channels
#define ADC_KEY_CHANNEL_NUM             1 //Real supported keypad channels
#define ADC_KEY_LAST_CHANNEL            ADC_KEY_CHANNEL_NUM - 1
//config which keypad channel enabled
#define ENABLE_KPDCHAN_1                ENABLE
#define ENABLE_KPDCHAN_2                DISABLE
#define ENABLE_KPDCHAN_3                DISABLE
#define ENABLE_KPDCHAN_4                DISABLE

#define KEYPAD_KEY_VALIDATION           3
#define KEYPAD_REPEAT_KEY_CHECK         KEYPAD_KEY_VALIDATION + 2
#define KEYPAD_REPEAT_KEY_CHECK_1       KEYPAD_KEY_VALIDATION + 3
#define KEYPAD_STABLE_NUM               10
#define KEYPAD_STABLE_NUM_MIN           9
#define KEYPAD_REPEAT_PERIOD            2 // 6
#define KEYPAD_REPEAT_PERIOD_1          KEYPAD_REPEAT_PERIOD/2

//------------------------------------------------------------------------------
// SAR boundary define
//------------------------------------------------------------------------------
#define KEYPAD_CH1_UB                   0xFF
#define KEYPAD_CH1_LB                   0xF0//0x80
#define KEYPAD_CH2_UB                   0xFF
#define KEYPAD_CH2_LB                   0x70//0x80
#define KEYPAD_CH3_UB                   0xFF
#define KEYPAD_CH3_LB                   0x70//0x80
#define KEYPAD_CH4_UB                   0xFF
#define KEYPAD_CH4_LB                   0x70//0x80

//### MAX support 8 level for each channel
#define ADC_KEY_LEVEL                   8
//### Currently Only Support 2 keypad Channels
//### Support un-balanced levels for each channel by spec. requirement
#define ADC_CH1_LEVELS                  8 //### must be <= ADC_KEY_LEVEL
#define ADC_CH2_LEVELS                  4 //### must be <= ADC_KEY_LEVEL
#define ADC_CH3_LEVELS                  4 //### must be <= ADC_KEY_LEVEL
#define ADC_CH4_LEVELS                  4 //### must be <= ADC_KEY_LEVEL

#define ADC_KEY_1_L0                    0x01//0x27
#define ADC_KEY_1_L1                    0x20//0x47
#define ADC_KEY_1_L2                    0x3D//0x63
#define ADC_KEY_1_L3                    0x5D//0x5C//0x7B
#define ADC_KEY_1_L4                    0x86//0x85//RFU
#define ADC_KEY_1_L5                    0x9E//0x9A//RFU
#define ADC_KEY_1_L6                    0xBB//0xB8//RFU
#define ADC_KEY_1_L7                    0xD2//0xCE//RFU

#define ADC_KEY_2_L0                    0x00//0x27
#define ADC_KEY_2_L1                    0x00//0x47
#define ADC_KEY_2_L2                    0x00//0x63
#define ADC_KEY_2_L3                    0x00//0x7B
#define ADC_KEY_2_L4                    0x00//RFU
#define ADC_KEY_2_L5                    0x00//RFU
#define ADC_KEY_2_L6                    0x00//RFU
#define ADC_KEY_2_L7                    0x00//RFU

#if (KEYPAD_TYPE_SEL == KEYPAD_TYPE_CUSTMOER)   // CUSTMOER keypad
#define ADC_KEY_1_L0_FLAG               IRKEY_UP
#define ADC_KEY_1_L1_FLAG               IRKEY_MENU
#define ADC_KEY_1_L2_FLAG               IRKEY_LEFT
#define ADC_KEY_1_L3_FLAG               IRKEY_MUTE
#define ADC_KEY_1_L4_FLAG               IRKEY_DUMY//RFU
#define ADC_KEY_1_L5_FLAG               IRKEY_DUMY//RFU
#define ADC_KEY_1_L6_FLAG               IRKEY_DUMY//RFU
#define ADC_KEY_1_L7_FLAG               IRKEY_DUMY//RFU

#define ADC_KEY_2_L0_FLAG               IRKEY_POWER
#define ADC_KEY_2_L1_FLAG               IRKEY_INPUT_SOURCE
#define ADC_KEY_2_L2_FLAG               IRKEY_RIGHT
#define ADC_KEY_2_L3_FLAG               IRKEY_DOWN
#define ADC_KEY_2_L4_FLAG               IRKEY_DUMY//RFU
#define ADC_KEY_2_L5_FLAG               IRKEY_DUMY//RFU
#define ADC_KEY_2_L6_FLAG               IRKEY_DUMY//RFU
#define ADC_KEY_2_L7_FLAG               IRKEY_DUMY//RFU
#elif (KEYPAD_TYPE_SEL == KEYPAD_TYPE_ORIG)   // MStar normal keypad
#define ADC_KEY_1_L0_FLAG               IRKEY_POWER
#define ADC_KEY_1_L1_FLAG               IRKEY_CHANNEL_PLUS//IRKEY_UP
#define ADC_KEY_1_L2_FLAG               IRKEY_CHANNEL_MINUS//IRKEY_DOWN
#define ADC_KEY_1_L3_FLAG               IRKEY_VOLUME_PLUS
#define ADC_KEY_1_L4_FLAG               IRKEY_VOLUME_MINUS
#define ADC_KEY_1_L5_FLAG               IRKEY_SELECT
#define ADC_KEY_1_L6_FLAG               IRKEY_INPUT_SOURCE
#define ADC_KEY_1_L7_FLAG               IRKEY_MENU

#define ADC_KEY_2_L0_FLAG               IRKEY_DUMY
#define ADC_KEY_2_L1_FLAG               IRKEY_DUMY
#define ADC_KEY_2_L2_FLAG               IRKEY_DUMY
#define ADC_KEY_2_L3_FLAG               IRKEY_DUMY
#define ADC_KEY_2_L4_FLAG               IRKEY_DUMY
#define ADC_KEY_2_L5_FLAG               IRKEY_DUMY
#define ADC_KEY_2_L6_FLAG               IRKEY_DUMY
#define ADC_KEY_2_L7_FLAG               IRKEY_DUMY
#elif (KEYPAD_TYPE_SEL == KEYPAD_TYPE_DEMO) // MStar demo set keypad
#define ADC_KEY_1_L0_FLAG               IRKEY_MUTE
#define ADC_KEY_1_L1_FLAG               IRKEY_VOLUME_MINUS
#define ADC_KEY_1_L2_FLAG               IRKEY_VOLUME_PLUS
#define ADC_KEY_1_L3_FLAG               IRKEY_DOWN
#define ADC_KEY_1_L4_FLAG               IRKEY_DUMY//RFU
#define ADC_KEY_1_L5_FLAG               IRKEY_DUMY//RFU
#define ADC_KEY_1_L6_FLAG               IRKEY_DUMY//RFU
#define ADC_KEY_1_L7_FLAG               IRKEY_DUMY//RFU

#define ADC_KEY_2_L0_FLAG               IRKEY_POWER
#define ADC_KEY_2_L1_FLAG               IRKEY_UP
#define ADC_KEY_2_L2_FLAG               IRKEY_MENU
#define ADC_KEY_2_L3_FLAG               IRKEY_INPUT_SOURCE
#define ADC_KEY_2_L4_FLAG               IRKEY_DUMY//RFU
#define ADC_KEY_2_L5_FLAG               IRKEY_DUMY//RFU
#define ADC_KEY_2_L6_FLAG               IRKEY_DUMY//RFU
#define ADC_KEY_2_L7_FLAG               IRKEY_DUMY//RFU
#endif

//-----PIN_OUT_SELECT------------------------------------------------------------------------
//PWMX Period=( PWMX_PERIOD+1 ) *( 1/ Xtal) //X=0~5
//PWMX_duty= (Init_PwmX_DUTY +1 ) * (1/XTAL) //X=0~5
#if (PADS_PWM0_MODE!=Unknown_pad_mux)
#define PWM0_PERIOD                     0xff
#define INIT_PWM0_DUTY                  0x7e
#define PWM0_DIV                        0x00
#endif
#if (PADS_PWM1_MODE!=Unknown_pad_mux)
#define PWM1_PERIOD                     0xff
#define INIT_PWM1_DUTY                  0x7e
#endif
#if (PADS_PWM2_MODE!=Unknown_pad_mux)
#define PWM2_PERIOD                     0xff
#define INIT_PWM2_DUTY                  0x7e
#endif
#if (PADS_PWM3_MODE!=Unknown_pad_mux) //Especially for Tuner+30V Power
#define PWM3_PERIOD                     0x23
#define INIT_PWM3_DUTY                  0x0A
#endif
#if (PADS_PWM4_MODE!=Unknown_pad_mux)
#define PWM4_PERIOD                     0xff
#define INIT_PWM4_DUTY                  0x7e
#endif
#if (PADS_PWM5_MODE!=Unknown_pad_mux)
#define PWM5_PERIOD                     0xff
#define INIT_PWM5_DUTY                  0x7e
#endif

#define BACKLITE_INIT_SETTING           ENABLE

#define PWM2_MUX_SEL                    0x00

//------8051 Serial Port Setting------------------------------------------------

#if(ENABLE_UART1_DEBUG)
#define ENABLE_UART0                    DISABLE
#define ENABLE_UART0_INTERRUPT          DISABLE
#define ENABLE_UART1                    ENABLE
#define ENABLE_UART1_INTERRUPT          ENABLE
//------STDIO device setting----------------------------------------------------
#define STDIN_DEVICE                    IO_DEV_UART1
#define STDOUT_DEVICE                   IO_DEV_UART1
#else
#define ENABLE_UART0                    ENABLE
#define ENABLE_UART0_INTERRUPT          ENABLE
#define ENABLE_UART1                    DISABLE
#define ENABLE_UART1_INTERRUPT          DISABLE
//------STDIO device setting----------------------------------------------------
#define STDIN_DEVICE                    IO_DEV_UART0
#define STDOUT_DEVICE                   IO_DEV_UART0
#endif

#define ENABLE_PIU_UART0                DISABLE
#define ENABLE_PIU_UART0_INTERRUPT      DISABLE

#define ENABLE_PIU_UART1                DISABLE
#define ENABLE_PIU_UART1_INTERRUPT      DISABLE

//------ PANEL RELATED ---------------------------------------------------------
#define PANEL_PDP_10BIT                 1
#define PANEL_SWAP_PORT                 0
#define PANEL_CONNECTOR_SWAP_LVDS_CH    0
#define PANEL_CONNECTOR_SWAP_LVDS_POL   0
#define PANEL_CONNECTOR_SWAP_PORT       0//1

#define PANEL_SWAP_LVDS_POL             0
#define PANEL_SWAP_LVDS_CH              0
#define BD_LVDS_CONNECT_TYPE            0 //0:156, 1: 100,128

#define ENABLE_VX1_CONVERTER_BOARD      1


//DDR Related
#define MIU0_SIZE_128M      0x8000000UL  //refine later
#define MIU0_BANK           0x8
#define S7J_MIU_UNIT        0x8

#define  MEMCLK_1066MHZ     1
#define  MEMCLK_1300MHZ     2
#define  MEMCLK_800MHZ      3
#define  MEMCLK_667MHZ      4
#define  MEMPLL_SEL         MEMCLK_800MHZ

//#define ENABLE_3D_PROCESS       ENABLE

#define ENABLE_ARC_DET          DISABLE

#if(LD_ENABLE)
    #define LD_SPI_PORT     1
    #define LD_DMA_PORT     1
#endif


#endif // _MSBOARD_H_

