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

#define BOARD_NAME                  "BD_MST269B_D01A_S_MARLON"

#define PIN_SPI_CZ1                     PIN_167
#define PIN_SPI_CZ2                     0
#define PIN_SPI_CZ3                     0
#define PIN_FLASH_WP0               PIN_171
#define PIN_FLASH_WP1               0

//------PAD MUX setting-------------------------------------------------------
#define Unknown_pad_mux         0
#define TCON_MODE(X)            X //1: TCON pads, 2: PCM pads (Select Mode first, and then Configure Signal Pads)
#define UART2_MODE(X)           X //(TX1,RX1) X=1: PAD_PCM_A14/PAD_PCM_A12, X=2: PAD_GPIO30/PAD_GPIO31
#define UART3_MODE(X)           X //(TX2,RX2) X=1: PAD_GPIO2/PAD_TCON2,         X=2: PAD_GPIO2/PAD_GPIO4
#define UART4_MODE(X)           X //(TX3,RX3) x=1: PAD_PCM_A14/PAD_PCM_A12,  X=2: PAD_PCM_A5/PAD_PCM_A4
#define UARTF_MODE(X)           X //(CTS3,RTS3) X=1: PAD_TCON0/PAD_TCON1
#define XC3DLR_MODE(X)          X //1: PAD_TCON0, 2: PAD_GPIO0, 3:PAD_TCON0 (scaler out with VSync), 4: PAD_GPIO0 (scaler out with VSync)
#define TTL_MODE(X)             X //1(6 bit):PAD_GPIO0/PAD_GPIO1, 2(8 bit):PAD_TCON0/PAD_TCON2/PAD_TCON3/PAD_TCON4
#define USBVBUS_MODE(X)         X //1: PAD_GPIO0, 2: PAD_GPIO1
#define MHLCBUS_MODE(X)         X //1: PAD_HOTPLUG_A, 2: PAD_HOTPLUG_B, 3: PAD_HOTPLUG_C
#define MHLVBUS_MODE(X)         X //1: PAD_GPIO24, 2: PAD_GPIO33, 3: PAD_TCON2, 4: PAD_PWM1, 5:PAD_GPIO30, 6:PAD_GPIO10, 7:PAD_TCON0
#define MHLDBG_MODE(X)          X //1: PAD_GPIO0
#define MHLDET_MODE(X)          X //1: PAD_MHL_DET
#define DDCDA_MODE(X)           X //1: PAD_DDCDA_CLK, PAD_DDCDA_DAT, 2: PAD_PCM_D4, PAD_PCM_D5
#define DDCDB_MODE(X)           X //1: PAD_DDCDB_CLK, PAD_DDCDB_DAT, 2: PAD_PCM_D6,PAD_PCM_D7
#define DDCDC_MODE(X)           X //1: PAD_DDCDC_CLK, PAD_DDCDC_DAT, 2: PAD_TS0_D0,PAD_TS0_D1
#define I2SIN_MODE(X)           X //1: PAD_NAND_AD4,PAD_NAND_AD6,PAD_NAND_AD7
#define I2SOUT_MODE(X)          X
    // 1: PAD_NAND_AD4, PAD_NAND_AD5, PAD_NAND_AD6, PAD_NAND_AD7
    // 2: PAD_GPIO12, PAD_GPIO5, PAD_TCON2, PAD_TCON0
    // 3: PAD_PM_SPI_CZ1, PAD_GPIO5, PAD_GPIO11, PAD_GPIO10
    // 4: Dummy1[12]reg_i2sout_tcon0_off_mck  [BCK(PAD_TCON2), WS(PAD_GPIO5), SD(PAD_GPIO12)]
    // 5: Dummy1[13]reg_i2sout_gpio10_off_mck [BCK(PAD_GPIO10), WS(PAD_GPIO5), SD(PAD_PM_SPI_CZ1)]
    // 6: Dummy1[11]reg_i2sout_ginga               [MCK(PAD_GPIO29), BCK(PAD_GPIO28), WS(PAD_GPIO26), SD(PAD_GPIO27)]
    // 7: Dummy1[10]reg_i2sout_ginga_off_mck   [BCK(PAD_GPIO28), WS(PAD_GPIO26), SD(PAD_GPIO27)]
    // 8: Dummy3[12]reg_i2sout_gpio7               [MCK(PAD_GPIO7), BCK(PAD_GPIO6), WS(PAD_GPIO3), SD(PAD_GPIO4)]
    // 9: Dummy3[11]reg_i2sout_gpio7_off_mck   [BCK(PAD_GPIO6), WS(PAD_GPIO3), SD(PAD_GPIO4)]
    //10: Dummy1[09]reg_i2sout_whisky             [MCK(PAD_TCON5), BCK(PAD_TCON4), WS(PAD_GPIO2), SD(PAD_TCON2)]
#define ET_MODE(X)              X //1: PAD_TCON0~7,PAD_GPIO0, 2: PAD_TCON0~5,PAD_GPIO0~2
#define LED_MODE(X)             X //1: PAD_TCON0, PAD_TCON2, 2: PAD_GPIO0, PAD_GPIO1, 3: PAD_GPIO10,PAD_GPIO11
#define CEC_MODE(X)             X //1: PAD_CEC, 2: PAD_PCM_D0
#define SPDIF_IN_MODE(X)        X
    // 1: PAD_GPIO35
    // 2: Dummy1[7]reg_spdifin_gpio3       [SPDIF_IN(PAD_GPIO3)]
    // 3: Dummy1[6]reg_spdifin_nand_cle  [SPDIF_IN(PAD_NAND_CLE)]
#define SPDIF_OUT_MODE(X)       X
    // 1: PAD_PM_SPI_CZ2
    // 2: PAD_NAND_AD3
    // 3: Dummy3[14]reg_spdifout_eco_tcon0 [SPDIF_OUT (PAD_TCON0)]
    // 4: Dummy3[13]reg_spdifout_eco_pwm1 [PSDIF_OUT (PAD_PWM1)]
#define INTGPIO_MODE(X)         X //1: PAD_INT
#define PWM0_MODE(X)            X //PAD_PWM0
#define PWM1_MODE(X)            X //PAD_PWM1
#define PWM2_MODE(X)            X //No pad out
#define PWM3_MODE(X)            X //No pad out
#define PWM4_MODE(X)            X //PAD_GPIO7
#define PWM5_MODE(X)            X //PAD_GPIO6
#define DDCR_MODE(X)            X //1: PAD_TCON3/PAD_TCON2, 2: PAD_NAND_AD6/PAD_NAND_AD7, 3: PAD_GPIO1/PAD_GPIO0, 4: PAD_TCON0/PAD_TCON2
#define MIIC_MODE(X)            X //1: PAD_TCON3/PAD_TCON2, 2: PAD_NAND_AD6/PAD_NAND_AD7, 3: PAD_GPIO1/PAD_GPIO0, 4: PAD_TCON0/PAD_TCON2
#define PCMPE_MODE(X)           X //1:
#define PCMCTRL_MODE(X)         X //1:
#define PCMAD_MODE(X)           X //1:
#define TS0_MODE(X)             X //1: P_IN, 2: S_IN,
#define TS1_MODE(X)             X //1: P_IN, 2: P_OUT, 3: S_IN, 4: MSPI
#define TSD_MODE(X)             X //1: Demod ts_out to TS1, 2: Demod ts_out to PCM
#define TS2_MODE(X)             X //1: P_IN, 2: S_IN, 3: MSPI
#define FLASH_MODE(X)           X
    // 1: SPINOR
    // 2: SPINAND
    // 3: NAND
    // 4: NANDSHARE(AD:PCM)   : Dummy1[15]reg_nand_share_mode [AD[7:0] --> PCM_A[7:0]]
    // 5: NANDSHARE(AD:TCON) : Dummy1[14]reg_nand_share_mode [AD[7:0 --> TCON[7:0]]
#define DISEQCOUT_MODE(X)       X //1: PAD_GPIO4, 2: PAD_GPIO5
#define DISEQCIN_MODE(X)        X //1: PAD_GPIO33, 2: PAD_GPIO34
#define ARC_MODE(X)             X //1: PAD_ARC

//define the following values from 1
#define PADS_TCON_MODE       Unknown_pad_mux
    #define PADS_TCON_CONFIG 0x00//Configure TCON signal pads to be used
#define PADS_UART2_MODE      Unknown_pad_mux
#define PADS_UART3_MODE      Unknown_pad_mux
#define PADS_UART4_MODE      Unknown_pad_mux
#define PADS_UARTF_MODE      Unknown_pad_mux
#define PADS_3DLR_MODE       Unknown_pad_mux
#define PADS_TTL_MODE        Unknown_pad_mux
#define PADS_USBVBUS_MODE    Unknown_pad_mux
#define PADS_MHLCBUS_MODE    MHLCBUS_MODE(1)
#define PADS_MHLVBUS_MODE    MHLVBUS_MODE(1)
#define PADS_MHLDBG_MODE     Unknown_pad_mux
#define PADS_MHLDET_MODE     MHLDET_MODE(1)
#define PADS_DDCDA_MODE      DDCDA_MODE(1)
#define PADS_DDCDB_MODE      DDCDB_MODE(1)
#define PADS_DDCDC_MODE      DDCDC_MODE(1)
#define PADS_I2SIN_MODE      Unknown_pad_mux
#define PADS_I2SOUT_MODE     Unknown_pad_mux
#define PADS_ET_MODE         Unknown_pad_mux
#define PADS_LED_MODE        Unknown_pad_mux
#define PADS_CEC_MODE        CEC_MODE(1)
#define PADS_SPDIF_IN        Unknown_pad_mux
#if ENABLE_AUTOTEST
#define PADS_SPDIF_OUT       SPDIF_OUT_MODE(1)
#else
#define PADS_SPDIF_OUT       Unknown_pad_mux
#endif
#define PADS_INTGPIO_MODE    Unknown_pad_mux
#define PADS_PWM0_MODE       PWM0_MODE(1)
#define PADS_PWM1_MODE       Unknown_pad_mux
#define PADS_PWM2_MODE       Unknown_pad_mux
#define PADS_PWM3_MODE       Unknown_pad_mux
#define PADS_PWM4_MODE       Unknown_pad_mux
#define PADS_PWM5_MODE       Unknown_pad_mux
#define PADS_DDCR_MODE       Unknown_pad_mux
#define PADS_PCMPE_MODE      PCMPE_MODE(1)
#define PADS_PCMCTRL_MODE    PCMCTRL_MODE(1)
#define PADS_PCMAD_MODE      PCMAD_MODE(1)
#define PADS_TS0_MODE        TS0_MODE(1)
#define PADS_TS1_MODE        Unknown_pad_mux
#define PADS_TSD_MODE        TSD_MODE(1)     //TS1 pad output from demod
#define PADS_TS2_MODE        Unknown_pad_mux
#define PADS_FLASH_MODE      FLASH_MODE(1)
#define PADS_DISEQCIN_MODE   Unknown_pad_mux
#define PADS_DISEQCOUT_MODE  DISEQCOUT_MODE(1)
#define PADS_ARC_MODE        ARC_MODE(1)

//#############################
//### To be reviewed later
//#############################
#define PADS_MHLPORT_MODE    Unknown_pad_mux
#define PADS_SPDIFARC_MODE   DISABLE


//------GPIO setting(default GPIO pin level)------------------------------------
#define PIN_67_IS_GPIO              GPIO_IN       	//GPIO44, Tuner_SDA
#define PIN_68_IS_GPIO              GPIO_IN       	//GPIO45, Tuner_SCL
#define PIN_72_IS_GPIO              GPIO_OUT_HIGH//GPIO49, AUDIO-EN (RT9116 Audio Amp Enable, L:Enable)  init: MUTE
#define PIN_159_IS_GPIO            GPIO_OUT_LOW //GPIO56, PreAMP_EN (Audio Mute, H:Mute -> set low)
#define PIN_161_IS_GPIO            GPIO_OUT_HIGH//GPIO0, MUTE_S       (I2S Audio Mute, L:Mute -> set high)
#define PIN_160_IS_GPIO            GPIO_IN             //GPIO2, USB0_OCD_N (USB Port 0 Over Current Detect)
#define PIN_165_IS_GPIO            GPIO_OUT_LOW//GPIO14, PANEL_ON_OFF (Panel Power Enable, L:On)
#define PIN_69_IS_GPIO              GPIO_OUT_HIGH//GPIO46, NORMAL_PWR-ON_OFF
#define PIN_70_IS_GPIO              GPIO_IN             //GPIO47, HP_DET
#define PIN_178_IS_GPIO            GPIO_IN             //GPIO94, CABLE_DET
#define PIN_175_IS_GPIO            GPIO_OUT_HIGH //GPIO96, PCMCIA Power Control

//------Chip Type---------------------------------------------------------------
#include "chip/MSD92LBSW1.h"

#define  SHARE_GND               	ENABLE

//------I2C devices-------------------------------------------------------------
#if USE_SW_I2C
#undef USE_SW_I2C
#define USE_SW_I2C                  1
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
#define PANEL_TYPE_SEL            	g_PNL_TypeSel//PNL_AU37_T370HW01_HD //PNL_AU20_T200XW02_WXGA//PNL_LG32_WXGA //PNL_AU19PW01_WXGA//PNL_AU20_T200XW02_WXGA //PNL_LG19_SXGA  //PNL_CMO22_WSXGA  //PNL_AU20_T200XW02_WXGA  // PNL_CMO22_WSXGA  // PNL_AU20_T200XW02_WXGA // PNL_AU17_EN05_SXGA
//#define FLASH_SIZE              FLASH_SIZE_8MB //depend on SW_Config

#define ENABLE_DDC_RAM            	ENABLE
#define RM_EEPROM_TYPE          	RM_TYPE_24C64//RM_TYPE_24C512
#define DIGITAL_I2S_SELECT        	AUDIO_I2S_NONE

#define INPUT_AV_VIDEO_COUNT      	1
#define INPUT_SV_VIDEO_COUNT       	0
#define INPUT_YPBPR_VIDEO_COUNT    	1
#ifdef ATSC_SYSTEM
#define INPUT_SCART_VIDEO_COUNT   	0
#else
#define INPUT_SCART_VIDEO_COUNT    	1
#endif
#define INPUT_HDMI_VIDEO_COUNT     	3
#ifdef ATSC_SYSTEM
#define ENABLE_SCART_VIDEO         	0
#else
#define ENABLE_SCART_VIDEO        	1
#endif

//------Input Source Mux--------------------------------------------------------
#define INPUT_VGA_MUX               INPUT_PORT_ANALOG0
#define INPUT_VGA_SYNC_MUX          INPUT_PORT_ANALOG0_SYNC //SYNC port of VGA. There is a case which data and sync use different port.
#define INPUT_YPBPR_MUX             INPUT_PORT_ANALOG2
#define INPUT_YPBPR2_MUX            INPUT_PORT_NONE_PORT
#define INPUT_TV_YMUX               INPUT_PORT_YMUX_CVBS0
#define INPUT_AV_YMUX               INPUT_PORT_YMUX_CVBS0
#define INPUT_AV2_YMUX              INPUT_PORT_NONE_PORT
#define INPUT_AV3_YMUX              INPUT_PORT_NONE_PORT
#define INPUT_SV_YMUX               INPUT_PORT_NONE_PORT
#define INPUT_SV_CMUX               INPUT_PORT_NONE_PORT
#define INPUT_SV2_YMUX              INPUT_PORT_NONE_PORT
#define INPUT_SV2_CMUX              INPUT_PORT_NONE_PORT
#define INPUT_SCART_YMUX            INPUT_PORT_YMUX_CVBS1
#define INPUT_SCART_RGBMUX          INPUT_PORT_ANALOG1
#define INPUT_SCART_FB_MUX          SCART_FB1
#define INPUT_SCART2_YMUX           INPUT_PORT_NONE_PORT
#define INPUT_SCART2_RGBMUX         INPUT_PORT_NONE_PORT
#define INPUT_SCART2_FB_MUX         INPUT_PORT_NONE_PORT

#define INPUT_HDMI1_MUX             INPUT_PORT_DVI0
#define INPUT_HDMI2_MUX             INPUT_PORT_DVI1
#define INPUT_HDMI3_MUX             INPUT_PORT_DVI3

/// for PWS path mapping
#define PWS_INPUT_HDMI1_PATH        _HDMI1_
#define PWS_INPUT_HDMI2_PATH        _HDMI3_
#define PWS_INPUT_HDMI3_PATH        _HDMI4_

#define SCART_ID_SEL                0x02   // HSYNC1
#define SCART2_ID_SEL               0x04   // HSYNC2
#define SCART_ID1_SAR_CHAN          KEYPAD_ADC_CHANNEL_2    //Due to PAD_SAR1 set to do SCART Hsync.
#define SCART_ID2_SAR_CHAN          0xFF //set invalid channel

//============================================
// Use ATSC code base audio path setting
//============================================
#define AUDIO_SOURCE_DTV            AUDIO_DSP1_DVB_INPUT
#define AUDIO_SOURCE_DTV2           AUDIO_DSP2_DVB_INPUT
#define AUDIO_SOURCE_ATV            AUDIO_DSP3_SIF_INPUT
#define AUDIO_SOURCE_PC             AUDIO_AUIN4_INPUT       // no PC line_in, so connect to AV input
#define AUDIO_SOURCE_YPBPR          AUDIO_AUIN2_INPUT
#define AUDIO_SOURCE_YPBPR2         AUDIO_NULL_INPUT
#define AUDIO_SOURCE_AV             AUDIO_AUIN4_INPUT
#define AUDIO_SOURCE_AV2            AUDIO_NULL_INPUT
#define AUDIO_SOURCE_AV3            AUDIO_NULL_INPUT
#define AUDIO_SOURCE_SV             AUDIO_NULL_INPUT
#define AUDIO_SOURCE_SV2            AUDIO_NULL_INPUT
#define AUDIO_SOURCE_SCART          AUDIO_AUIN3_INPUT
#define AUDIO_SOURCE_SCART2         AUDIO_NULL_INPUT
#define AUDIO_SOURCE_HDMI           AUDIO_HDMI_INPUT
#define AUDIO_SOURCE_HDMI2          AUDIO_HDMI_INPUT
#define AUDIO_SOURCE_HDMI3          AUDIO_HDMI_INPUT
#define AUDIO_SOURCE_DVI            AUDIO_SOURCE_PC         // default: AUDIO_SOURCE_PC
#define AUDIO_SOURCE_DVI2           AUDIO_SOURCE_PC
#define AUDIO_SOURCE_DVI3           AUDIO_SOURCE_PC
#define AUDIO_SOURCE_KTV            AUDIO_NULL_INPUT

#define AUDIO_PATH_MAIN_SPEAKER     AUDIO_T3_PATH_AUOUT3
#define AUDIO_PATH_HP               AUDIO_T3_PATH_AUOUT1
#define AUDIO_PATH_MONITOROUT       AUDIO_PATH_NULL
#define AUDIO_PATH_SCART            AUDIO_PATH_NULL   // always output ATV/DTV sound
#define AUDIO_PATH_SPDIF            AUDIO_T3_PATH_SPDIF
#define AUDIO_PATH_SPEAKER          AUDIO_PATH_NULL
#define AUDIO_PATH_LINEOUT          AUDIO_PATH_NULL
#define AUDIO_PATH_SIFOUT           AUDIO_PATH_NULL
#define AUDIO_PATH_SCART1           AUDIO_PATH_NULL

#define AUDIO_OUTPUT_MAIN_SPEAKER   AUDIO_AUOUT3_OUTPUT
#define AUDIO_OUTPUT_HP             AUDIO_HP_OUTPUT         // default: AUDIO_HP_OUTPUT, if no headphone output, define NULL
#define AUDIO_OUTPUT_MONITOROUT     AUDIO_NULL_OUTPUT
#define AUDIO_OUTPUT_SCART          AUDIO_NULL_OUTPUT  // define NULL when no used
#define AUDIO_OUTPUT_LINEOUT        AUDIO_NULL_OUTPUT
#define AUDIO_OUTPUT_SIFOUT         AUDIO_NULL_OUTPUT

//-----------------------Add GPIO switch setting -------------------------
#define Switch_PC()                 _FUNC_NOT_USED()
#define Switch_YPBPR()              _FUNC_NOT_USED()
#define Switch_YPBPR2()             _FUNC_NOT_USED()
#define Switch_AV()                 _FUNC_NOT_USED()
#define Switch_AV2()                _FUNC_NOT_USED()
#define Switch_AV3()                _FUNC_NOT_USED()
#define Switch_SV()                 _FUNC_NOT_USED()
#define Switch_SV2()                _FUNC_NOT_USED()
#define Switch_SCART()              _FUNC_NOT_USED()
#define Switch_SCART2()             _FUNC_NOT_USED()
#define Switch_DVI()                MApi_XC_DVI_SwitchSrc(INPUT_PORT_DVI0)
#define Switch_DVI2()               MApi_XC_DVI_SwitchSrc(INPUT_PORT_DVI1)
#define Switch_DVI3()               _FUNC_NOT_USED()
#define Switch_DVI4()               _FUNC_NOT_USED()
#define Switch_DEFAULT()            _FUNC_NOT_USED()
#define Demod_Reset_On()            _FUNC_NOT_USED() //   mdrv_gpio_set_low( PIN_156 )
#define Demod_Reset_Off()           _FUNC_NOT_USED() //   mdrv_gpio_set_high( PIN_156 )
//------HDMI ARC Contrl-------------------------------------------------------
#define ARC_CTRL_ON()                _FUNC_NOT_USED()
#define ARC_CTRL_OFF()                _FUNC_NOT_USED()
#define ARC_SUPPORT_PORT             3   //connect to UI port3


// HDMI switch Setting
#define HDMI_SWITCH_SELECT  HDMI_SWITCH_NONE

//------MHL Setting-----------------------------------------------------------
#define ENABLE_MHL                              ENABLE
#define HDMI_PORT_FOR_MHL                       UI_INPUT_SOURCE_HDMI
//------ HDMI RELATED ---------------------------------------------------------
#define HDCP_HPD_INVERSE                ENABLE
#define HDCP_HPD_2_INVERSE              1
#define HDCP_HPD_3_INVERSE              1
#define HDCP_HPD_4_INVERSE              1

//For Support MHL chip
#if (ENABLE_MHL == ENABLE)
#define INPUT_SUPPORT_MHL_PATH                  E_INPUT_SUPPORT_MHL_PORT_DVI0
#define MHL_TYPE MHL_TYPE_INTERNAL
#else
#define INPUT_SUPPORT_MHL_PATH                  E_INPUT_NOT_SUPPORT_MHL
#define MHL_TYPE MHL_TYPE_NONE
#endif

//------Tuner Setting-----------------------------------------------------------
/*
#ifdef ATSC_SYSTEM
#define TS_CLK_INV                      0
#define TS_PARALLEL_OUTPUT              1
#define FRONTEND_DEMOD_TYPE             EMBEDDED_ATSC_DEMOD
#define FRONTEND_TUNER_TYPE             SILAB_2158_TUNER_A20 //SILAB_2158_TUNER
#define FRONTEND_IF_MIXER_TYPE          XUGUANG_T126CWADC //PHILIPS_FQD1216_TUNER
#define FRONTEND_IF_DEMODE_TYPE         MSTAR_INTERN_VIF  //PHILIPS_TDA9886 //MSTAR_VIF_MSB1210   //MSTAR_VIF //PHILIPS_TDA9886
#define FRONTEND_SECOND_DEMOD_TYPE      MSTAR_NONE_DEMOD
#else
#define TS_CLK_INV                      0
#define TS_PARALLEL_OUTPUT              1
#define TS_SERIAL_OUTPUT_IF_CI_REMOVED  0
#define USE_UTOPIA                      ENABLE
  #if (DTV_SYSTEM_SEL == DTV_SYSTEM_DVBC)
    #define FRONTEND_DEMOD_TYPE             EMBEDDED_DVBC_DEMOD
    #define FRONTEND_SECOND_DEMOD_TYPE      MSTAR_NONE_DEMOD
  #elif (DTV_SYSTEM_SEL == DTV_SYSTEM_DVBT)
    #define FRONTEND_DEMOD_TYPE             EMBEDDED_DVBT_DEMOD
    #define FRONTEND_DEMOD_T2_TYPE          EMBEDDED_DVBT2_DEMOD//MSTAR_MSB123X_DVBT2//EMBEDDED_DVBT2_DEMOD
    #define FRONTEND_SECOND_DEMOD_TYPE      MSTAR_NONE_DEMOD
  #elif (DTV_SYSTEM_SEL == DTV_SYSTEM_DVBT_Plus_DVBC)
    #define FRONTEND_DEMOD_TYPE             EMBEDDED_DVBT_DEMOD
    #define FRONTEND_SECOND_DEMOD_TYPE      EMBEDDED_DVBC_DEMOD
    #define FRONTEND_DEMOD_T2_TYPE          EMBEDDED_DVBT2_DEMOD
  #else
    #define FRONTEND_DEMOD_TYPE             EMBEDDED_DVBT_DEMOD
    #define FRONTEND_SECOND_DEMOD_TYPE      MSTAR_NONE_DEMOD
  #endif

#define FRONTEND_TUNER_TYPE             MxL_601SI_TUNER	//NXP_FH2608_TUNER //NUTUNE_FK1602_TUNER
#define FRONTEND_IF_MIXER_TYPE          XUGUANG_T126CWADC //PHILIPS_FQD1216_TUNER
#define FRONTEND_IF_DEMODE_TYPE         MSTAR_INTERN_VIF  //PHILIPS_TDA9886 //MSTAR_VIF_MSB1210   //MSTAR_VIF //PHILIPS_TDA9886
//#define FRONTEND_SECOND_DEMOD_TYPE      MSTAR_NONE_DEMOD
#endif
*/
#define TS_CLK_INV                      0
#define TS_SERIAL_OUTPUT_IF_CI_REMOVED  0
#define FRONTEND_TUNER_TYPE             MxL_661SI_TUNER // MxL_601SI_TUNER	//NXP_FH2608_TUNER //NUTUNE_FK1602_TUNER
#define FRONTEND_IF_MIXER_TYPE          XUGUANG_T126CWADC //PHILIPS_FQD1216_TUNER
#define FRONTEND_IF_DEMODE_TYPE         MSTAR_INTERN_VIF  //PHILIPS_TDA9886 //MSTAR_VIF_MSB1210   //MSTAR_VIF //PHILIPS_TDA9886
#define FRONTEND_SECOND_DEMOD_TYPE      MSTAR_NONE_DEMOD

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
    #define FRONTEND_DEMOD_DVBT_TYPE            EMBEDDED_DVBT_DEMOD
    #define FRONTEND_DEMOD_T2_TYPE          EMBEDDED_DVBT2_DEMOD
    #define TS_PARALLEL_DVBT_OUTPUT             1 // 0: Serial; 1: paralell
    #define DEMOD_DVBT_USE_UTOPIA               1
#else
    #define FRONTEND_DEMOD_DVBT_TYPE            MSTAR_NONE_DEMOD
    #define TS_PARALLEL_DVBT_OUTPUT             1 // 0: Serial; 1: paralell
    #define DEMOD_DVBT_USE_UTOPIA               0
#endif

#if ENABLE_DTMB
    #define FRONTEND_DEMOD_DTMB_TYPE            EMBEDDED_DTMB_DEMOD
    #define TS_PARALLEL_DTMB_OUTPUT             0 // 0: Serial; 1: paralell
    #define DEMOD_DTMB_USE_UTOPIA               1
#else
    #define FRONTEND_DEMOD_DTMB_TYPE            MSTAR_NONE_DEMOD
    #define TS_PARALLEL_DTMB_OUTPUT             1 // 0: Serial; 1: paralell
    #define DEMOD_DTMB_USE_UTOPIA               0
#endif

#if ENABLE_DVBC
    #define FRONTEND_DEMOD_DVBC_TYPE            EMBEDDED_DVBC_DEMOD
    #define TS_PARALLEL_DVBC_OUTPUT             1 // 0: Serial; 1: paralell
    #define DEMOD_DVBC_USE_UTOPIA               1
#else
    #define FRONTEND_DEMOD_DVBC_TYPE            MSTAR_NONE_DEMOD
    #define TS_PARALLEL_DVBC_OUTPUT             1 // 0: Serial; 1: paralell
    #define DEMOD_DVBC_USE_UTOPIA               0
#endif

#if ENABLE_ISDBT
  #if(CHIP_FAMILY_MARLON==CHIP_FAMILY_TYPE)
    #define FRONTEND_DEMOD_ISDBT_TYPE           EMBEDDED_ISDBT_DEMOD
    #define TS_PARALLEL_ISDBT_OUTPUT            1 // 0: Serial; 1: paralell
    #define DEMOD_ISDBT_USE_UTOPIA              1
  #else
    #define FRONTEND_DEMOD_ISDBT_TYPE           EMBEDDED_ISDBT_DEMOD
    #define TS_PARALLEL_ISDBT_OUTPUT            0 // 0: Serial; 1: paralell
    #define DEMOD_ISDBT_USE_UTOPIA              0
  #endif
#else
    #define FRONTEND_DEMOD_ISDBT_TYPE           MSTAR_NONE_DEMOD
    #define TS_PARALLEL_ISDBT_OUTPUT            1 // 0: Serial; 1: paralell
    #define DEMOD_ISDBT_USE_UTOPIA              0
#endif

#if ENABLE_S2
#define FRONTEND_TUNER_S2_TYPE          AVAILINK_AV2012
#define FRONTEND_DEMOD_S2_TYPE          EMBEDDED_DVBS_DEMOD
#define DEMOD_S2_USE_UTOPIA       1
#define DISH_TYPE                       DISH_A8304 //DISH_A8293 //DISH_DEMOD
#define RT5006                          0
#define DEMOD_RESET_PIN_INIT()          //mdrv_gpio_set_high(BALL_K21)
#define DEMOD_RESET(x)                  (x=x) // For fix compile warning, please look BD_MST157E_C02A_EMERALD.h, old PCB board have this define.
#endif

#define VIF_TUNER_TYPE                  1                   // 0: RF Tuner; 1: Silicon Tuner
#define VIF_SAW_ARCH                    2                   // 0: Dual SAW; 1: external Single SAW; 2:silicon tuner; 3: no saw(VIF); 4: internal single SAW; 5: no SAW(DIF)

#define MSB1210_TS_SERIAL_INVERSION                     0
#define MSB1210_TS_PARALLEL_INVERSION                   1
#define MSB1210_DTV_DRIVING_LEVEL                       1 //0 or 1
#define MSB1210_WEAK_SIGNAL_PICTURE_FREEZE_ENABLE       1

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
#define ENABLE_POWER_SAVING_DPMS_DVI    0

#define ENABLE_PWS                      1   // Analog IP PWS //temporarily mark out while bring-up
#define ENABLE_DIP_PWS                  0   //Digital IP PWS, for U3 only now
#define ENABLE_DIP_MONITOR              0   //For U3 oly now


//------Memory Setting----------------------------------------------------------
#define BOOTUP_MIU_BIST                 1
#ifndef MEMORY_MAP
#define MEMORY_MAP                      MMAP_128MB
#endif
#define  MIU_INTERFACE                  DDR3_INTERFACE_BGA   //DDR3_INTERFACE_BGA

//------Analog Function Setting-------------------------------------------------
#define MOD_LVDS_GPIO                   0x820

#define LVDS_PN_SWAP_L                  0xFC
#define LVDS_PN_SWAP_H                  0x3F

#if ENABLE_AUTOTEST
#define ENABLE_SSC                      DISABLE
#else
#define ENABLE_SSC                      ENABLE // DISABLE
#endif

#define ENABLE_LVDSTORGB_CONVERTER      DISABLE
#if ENABLE_SSC
#define MIU_SSC_SPAN_DEFAULT            25
#define MIU_SSC_STEP_DEFAULT            10
#define MIU_SSC_SPAN_MAX                40
#define MIU_SSC_STEP_MAX                20
#define LVDS_SSC_SPAN_DEFAULT           350
#define LVDS_SSC_STEP_DEFAULT           200
#define LVDS_SSC_SPAN_MAX               500
#define LVDS_SSC_STEP_MAX               300
#define LVDS_SSC_SWING_DEFAULT          250
#define LVDS_SSC_SWING_MIN              40
#define LVDS_SSC_SWING_MAX              600
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
#define Audio_Amplifier_ON()            _FUNC_NOT_USED()
#define Audio_Amplifier_OFF()           _FUNC_NOT_USED()

#define Adj_Volume_On()                 mdrv_gpio_set_low( PIN_72 ) // RT9116 Audio Amp, ENABLE
#define Adj_Volume_Off()                mdrv_gpio_set_high( PIN_72 ) // RT9116 Audio Amp, MUTE

#define Panel_VCC_ON()                  mdrv_gpio_set_low( PIN_165 )
#define Panel_VCC_OFF()                 mdrv_gpio_set_high( PIN_165 )

#define Panel_Backlight_VCC_ON()        mdrv_gpio_set_low( PIN_165 )
#define Panel_Backlight_VCC_OFF()       mdrv_gpio_set_high( PIN_165 )

#define Panel_Backlight_PWM_ADJ(x)      MDrv_PWM_DutyCycle(E_PWM_CH0, x)
#define Panel_Backlight_Max_Current(x)  MDrv_PWM_DutyCycle(E_PWM_CH0, x)

#define Panel_VG_HL_CTL_ON()            _FUNC_NOT_USED()
#define Panel_VG_HL_CTL_OFF()           _FUNC_NOT_USED()

// PCMCIA power control
#define PCMCIA_VCC_ON()                 mdrv_gpio_set_low( PIN_175 )
#define PCMCIA_VCC_OFF()                mdrv_gpio_set_high( PIN_175 )

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

#define TunerOffPCMCIA()                PCMCIA_VCC_ON()
#define TunerOnPCMCIA()                 PCMCIA_VCC_OFF()

// LED Control
#define LED_RED_ON()                    _FUNC_NOT_USED()
#define LED_RED_OFF()                   _FUNC_NOT_USED()
#define LED_GREEN_ON()                  _FUNC_NOT_USED()
#define LED_GREEN_OFF()                 _FUNC_NOT_USED()

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

//------MST Keypad definition---------------------------------------------------
#define KEYPAD_CHANNEL_SUPPORT          4 //Maximun supported keypad channels
#define ADC_KEY_CHANNEL_NUM             1 //Real supported keypad channels
#define ADC_KEY_LAST_CHANNEL            ADC_KEY_CHANNEL_NUM - 1
//config which keypad channel enabled
#define ENABLE_KPDCHAN_1                ENABLE
#define ENABLE_KPDCHAN_2                ENABLE // for scart
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
#define KEYPAD_CH1_LB                   0x70//0x80
#define KEYPAD_CH2_UB                   0xFF
#define KEYPAD_CH2_LB                   0x70//0x80
#define KEYPAD_CH3_UB                   0xFF
#define KEYPAD_CH3_LB                   0x70//0x80
#define KEYPAD_CH4_UB                   0xFF
#define KEYPAD_CH4_LB                   0x70//0x80

//### MAX support 8 level for each channel
#define ADC_KEY_LEVEL                   8
#define ADC_KEY_L0                      0x12
#define ADC_KEY_L1                      0x36
#define ADC_KEY_L2                      0x56
#define ADC_KEY_L3                      0x7B
#define ADC_KEY_LEVEL_MAX               ADC_KEY_L3

//### Currently Only Support 2 keypad Channels
//### Support un-balanced levels for each channel by spec. requirement
#define ADC_CH1_LEVELS                  4 //### must be <= ADC_KEY_LEVEL
#define ADC_CH2_LEVELS                  4 //### must be <= ADC_KEY_LEVEL
#define ADC_CH3_LEVELS                  4 //### must be <= ADC_KEY_LEVEL
#define ADC_CH4_LEVELS                  4 //### must be <= ADC_KEY_LEVEL

#define ADC_KEY_1_L0                    0x10//0x27
#define ADC_KEY_1_L1                    0x30//0x47
#define ADC_KEY_1_L2                    0x50//0x63
#define ADC_KEY_1_L3                    0x6A//0x7B
#define ADC_KEY_1_L4                    0x00//RFU
#define ADC_KEY_1_L5                    0x00//RFU
#define ADC_KEY_1_L6                    0x00//RFU
#define ADC_KEY_1_L7                    0x00//RFU

#define ADC_KEY_2_L0                    0x10//0x27
#define ADC_KEY_2_L1                    0x30//0x47
#define ADC_KEY_2_L2                    0x50//0x63
#define ADC_KEY_2_L3                    0x6A//0x7B
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

#define PWM0_MUX_SEL                    0x00

//------8051 Serial Port Setting------------------------------------------------

//----------------------------------------------------------------------------
// UART_SRC_SEL
//----------------------------------------------------------------------------

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
#if (ENABLE_IC_LVDS_SWAP==ENABLE)
#define PANEL_PDP_10BIT                 1
#define PANEL_SWAP_PORT                 0
#define PANEL_CONNECTOR_SWAP_LVDS_CH    0
#define PANEL_CONNECTOR_SWAP_LVDS_POL   0
#define PANEL_CONNECTOR_SWAP_PORT       1

#define PANEL_SWAP_LVDS_POL             1
#define PANEL_SWAP_LVDS_CH              1
#define BD_LVDS_CONNECT_TYPE            7
#else
#define PANEL_PDP_10BIT                 1
#define PANEL_SWAP_PORT                 0
#define PANEL_CONNECTOR_SWAP_LVDS_CH    0
#define PANEL_CONNECTOR_SWAP_LVDS_POL   0
#define PANEL_CONNECTOR_SWAP_PORT       1

#define PANEL_SWAP_LVDS_POL             1
#define PANEL_SWAP_LVDS_CH              0
#define BD_LVDS_CONNECT_TYPE            1
#endif

#if(FRONTEND_TUNER_TYPE==MxL_301RF_TUNER)
#define D_DMD_IF_DVBT_8M  6000L
#define D_DMD_IF_DVBT_7M  6000L
#define D_DMD_IF_DVBT_6M  6000L
#define D_DMD_IF_DVBC_8M  6000L
#define D_DMD_IF_DVBC_7M  6000L
#define D_DMD_IF_DVBC_6M  6000L
#define D_DMD_TUNER_IQSWAP 1
#elif (FRONTEND_TUNER_TYPE == LG_G230D_TUNER)
#define D_DMD_IF_DVBT_8M  5500L
#define D_DMD_IF_DVBT_7M  5500L
#define D_DMD_IF_DVBT_6M  5500L
#define D_DMD_IF_DVBC_8M  5500L
#define D_DMD_IF_DVBC_7M  5500L
#define D_DMD_IF_DVBC_6M  5500L
#define D_DMD_TUNER_IQSWAP 0
#else
#define D_DMD_IF_DVBT_8M 4800
#define D_DMD_IF_DVBT_7M 4500
#define D_DMD_IF_DVBT_6M 3500
#define D_DMD_IF_DVBC_8M 5000
#define D_DMD_IF_DVBC_7M 5000
#define D_DMD_IF_DVBC_6M 5000
#define D_DMD_TUNER_IQSWAP 1
#endif

#define D_DMD_SAR_CHANNEL 0xFF
#define D_DMD_SSI_TABLE_RFAGC \
{ \
    {-15.000000,    0x1c}, \
    {-15.500000,    0x1c}, \
    {-16.000000,    0x1c}, \
    {-16.500000,    0x1d}, \
    {-17.000000,    0x1e}, \
    {-17.500000,    0x1e}, \
    {-18.000000,    0x1f}, \
    {-18.500000,    0x20}, \
    {-19.000000,    0x20}, \
    {-19.500000,    0x21}, \
    {-20.000000,    0x22}, \
    {-20.500000,    0x23}, \
    {-21.000000,    0x23}, \
    {-21.500000,    0x25}, \
    {-22.000000,    0x26}, \
    {-22.500000,    0x27}, \
    {-23.000000,    0x28}, \
    {-23.500000,    0x2a}, \
    {-24.000000,    0x2b}, \
    {-24.500000,    0x2d}, \
    {-25.000000,    0x2f}, \
    {-25.500000,    0x31}, \
    {-26.000000,    0x33}, \
    {-26.500000,    0x35}, \
    {-27.000000,    0x38}, \
    {-27.500000,    0x3a}, \
    {-28.000000,    0x3d}, \
    {-28.500000,    0x40}, \
    {-29.000000,    0x42}, \
    {-29.500000,    0x45}, \
    {-30.000000,    0x47}, \
    {-30.500000,    0x49}, \
    {-31.000000,    0x4c}, \
    {-31.500000,    0x4e}, \
    {-32.000000,    0x51}, \
    {-32.500000,    0x52}, \
    {-33.000000,    0x55}, \
    {-33.500000,    0x56}, \
    {-34.000000,    0x59}, \
    {-34.500000,    0x5a}, \
    {-35.000000,    0x5c}, \
    {-35.500000,    0x5e}, \
    {-36.000000,    0x60}, \
    {-36.500000,    0x61}, \
    {-37.000000,    0x63}, \
    {-37.500000,    0x64}, \
    {-38.000000,    0x66}, \
    {-38.500000,    0x67}, \
    {-39.000000,    0x69}, \
    {-39.500000,    0x6b}, \
    {-40.000000,    0x6c}, \
    {-40.500000,    0x6d}, \
    {-41.000000,    0x6e}, \
    {-41.500000,    0x70}, \
    {-42.000000,    0x71}, \
    {-42.500000,    0x72}, \
    {-43.000000,    0x74}, \
    {-43.500000,    0x75}, \
    {-44.000000,    0x76}, \
    {-44.500000,    0x78}, \
    {-45.000000,    0x79}, \
    {-45.500000,    0x7a}, \
    {-46.000000,    0x7b}, \
    {-46.500000,    0x7c}, \
    {-47.000000,    0x7d}, \
    {-47.500000,    0x7e}, \
    {-48.000000,    0x7f}, \
    {-48.500000,    0x81}, \
    {-49.000000,    0x82}, \
    {-49.500000,    0x83}, \
    {-50.000000,    0x84}, \
    {-50.500000,    0x85}, \
    {-51.000000,    0x87}, \
    {-51.500000,    0x88}, \
    {-52.000000,    0x89}, \
    {-52.500000,    0x8b}, \
    {-53.000000,    0x8d}, \
    {-53.500000,    0x8f}, \
    {-54.000000,    0x91}, \
    {-54.500000,    0x94}, \
    {-55.000000,    0x9a}, \
    {-55.500000,    0xae}, \
    {-56.000000,    0xeb}, \
    {-56.500000,    0xef}, \
    {-57.000000,    0xf0}, \
} \

#define D_DMD_SSI_TABLE_IFAGC \
{ \
    {-56.000000,    0xb1}, \
    {-56.500000,    0xb2}, \
    {-57.000000,    0xb2}, \
    {-57.500000,    0xb3}, \
    {-58.000000,    0xb4}, \
    {-58.500000,    0xb4}, \
    {-59.000000,    0xb5}, \
    {-59.500000,    0xb6}, \
    {-60.000000,    0xb6}, \
    {-60.500000,    0xb7}, \
    {-61.000000,    0xb8}, \
    {-61.500000,    0xb9}, \
    {-62.000000,    0xb9}, \
    {-62.500000,    0xba}, \
    {-63.000000,    0xba}, \
    {-63.500000,    0xbb}, \
    {-64.000000,    0xbc}, \
    {-64.500000,    0xbc}, \
    {-65.000000,    0xbd}, \
    {-65.500000,    0xbe}, \
    {-66.000000,    0xbe}, \
    {-66.500000,    0xbf}, \
    {-67.000000,    0xbf}, \
    {-67.500000,    0xc0}, \
    {-68.000000,    0xc0}, \
    {-68.500000,    0xc1}, \
    {-69.000000,    0xc1}, \
    {-69.500000,    0xc1}, \
    {-70.000000,    0xc2}, \
    {-70.500000,    0xc2}, \
    {-71.000000,    0xc3}, \
    {-71.500000,    0xc3}, \
    {-72.000000,    0xc3}, \
    {-72.500000,    0xc4}, \
    {-73.000000,    0xc4}, \
    {-73.500000,    0xc5}, \
    {-74.000000,    0xc5}, \
    {-74.500000,    0xc6}, \
    {-75.000000,    0xc6}, \
    {-75.500000,    0xc6}, \
    {-76.000000,    0xc7}, \
    {-76.500000,    0xc7}, \
    {-77.000000,    0xc8}, \
    {-77.500000,    0xc8}, \
    {-78.000000,    0xc9}, \
    {-78.500000,    0xc9}, \
    {-79.000000,    0xca}, \
    {-79.500000,    0xca}, \
    {-80.000000,    0xcb}, \
    {-80.500000,    0xcc}, \
    {-81.000000,    0xcc}, \
    {-81.500000,    0xcd}, \
    {-82.000000,    0xcd}, \
    {-82.500000,    0xcd}, \
    {-83.000000,    0xce}, \
    {-83.500000,    0xce}, \
    {-84.000000,    0xcf}, \
    {-84.500000,    0xcf}, \
    {-85.000000,    0xd0}, \
    {-85.500000,    0xd0}, \
    {-86.000000,    0xd1}, \
    {-86.500000,    0xd1}, \
    {-87.000000,    0xd2}, \
    {-87.500000,    0xd3}, \
    {-88.000000,    0xee}, \
    {-93.000000,    0xff}, \
} \

#define D_DMD_SSI_TABLE_IFAGC_ERR_LO \
{ \
    {0.00,        0x0A}, \
    {0.00,        0xFF}, \
} \

#define D_DMD_SSI_TABLE_IFAGC_ERR_HI \
{ \
    {0.00,        0x2D}, \
    {0.00,        0xFF}, \
} \

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
} \

#define D_DMD_DVBT_DSPRegInitExt \
{\
     1,\
     0,\
}
/*
// 0:DUAL_SAW
    3,// version
    0,// reserved
    0xFF,// TS_CLK
    1,// RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning
    (MS_U8)(36167>>24),// IF Frequency
    (MS_U8)(36167>>16),
    (MS_U8)(36167>>8),
    (MS_U8)(36167>>0),
    (MS_U8)(45474>>24),// FS Frequency
    (MS_U8)(45474>>16),
    (MS_U8)(45474>>8),
    (MS_U8)(45474>>0),
    0,// IQ Swap
    0,// u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
    0,// u8PadSel : 0=Normal, 1=analog pad
    0,// bPGAEnable : 0=disable, 1=enable
    5,// u8PGAGain : default 5
*/
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
    (MS_U8)(5500>>24), \
    (MS_U8)(5500>>16), \
    (MS_U8)(5500>>8), \
    (MS_U8)(5500>>0), \
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

#define D_DMD_DVBC_DSPRegInitExt \
{\
    1,\
    0,\
    E_DMD_DVBC_NO_SIGNAL_NUM_TH,\
    E_DMD_DVBC_NO_SIGNAL_NUM_TH>>8,\
    0xFF,\
    0x14,\
    E_DMD_DVBC_NO_SIGNAL_GAIN_TH_L,\
    E_DMD_DVBC_NO_SIGNAL_GAIN_TH_L>>8,\
    0xFF,\
    0x00,\
    E_DMD_DVBC_NO_SIGNAL_GAIN_TH_H,\
    E_DMD_DVBC_NO_SIGNAL_GAIN_TH_H>>8,\
    0xFF,\
    0x80,\
}//pls ask BY.Tsai why modify this value (from 0x90 to 0x80)can improve scan speed.
/*
// 0:DUAL_SAW
    3,// version
    0,// reserved
    0xFF,// TS_CLK
    1,// RFAGC tristate control default value, 1:trisate 0:non-tristate,never modify unless you know the meaning
    (MS_U8)(36167>>24),// IF Frequency
    (MS_U8)(36167>>16),
    (MS_U8)(36167>>8),
    (MS_U8)(36167>>0),
    (MS_U8)(45474>>24),// FS Frequency
    (MS_U8)(45474>>16),
    (MS_U8)(45474>>8),
    (MS_U8)(45474>>0),
    0,// IQ Swap
    0,// u8ADCIQMode : 0=I path, 1=Q path, 2=both IQ
    0,// u8PadSel : 0=Normal, 1=analog pad
    0,// bPGAEnable : 0=disable, 1=enable
    5,// u8PGAGain : default 5
*/
#if (VIF_SAW_ARCH==0)
#define D_DMD_DVBC_InitExt \
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
#define D_DMD_DVBC_InitExt \
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
#define D_DMD_DVBC_InitExt \
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
#define D_DMD_DVBC_InitExt \
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
#define D_DMD_DVBC_InitExt \
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
#define D_DMD_DVBC_InitExt \
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
#define D_DMD_DVBC_InitExt \
{ \
    3, \
    0, \
    0xFF, \
    1, \
    (MS_U8)(5000>>24), \
    (MS_U8)(5000>>16), \
    (MS_U8)(5000>>8), \
    (MS_U8)(5000>>0), \
    (MS_U8)(24000>>24), \
    (MS_U8)(24000>>16), \
    (MS_U8)(24000>>8), \
    (MS_U8)(24000>>0), \
    0, \
    1, \
    1, \
    0, \
    5, \
}
#else
#define D_DMD_DVBC_InitExt NULL
#print "BOARD_SAW_TYPE Error"
#endif

#endif // _MSBOARD_H_

