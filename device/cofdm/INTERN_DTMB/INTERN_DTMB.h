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
/**********************************************************************
 Copyright (c) 2006-2009 MStar Semiconductor, Inc.
 All rights reserved.

 Unless otherwise stipulated in writing, any and all information contained
 herein regardless in any format shall remain the sole proprietary of
 MStar Semiconductor Inc. and be kept in strict confidence
 (MStar Confidential Information) by the recipient.
 Any unauthorized act including without limitation unauthorized disclosure,
 copying, use, reproduction, sale, distribution, modification, disassembling,
 reverse engineering and compiling of the contents of MStar Confidential
 Information is unlawful and strictly prohibited. MStar hereby reserves the
 rights to any and all damages, losses, costs and expenses resulting therefrom.
**********************************************************************/
#ifndef _INTERN_DTMB_H_
#define _INTERN_DTMB_H_

#ifdef _INTERN_DTMB_C_
#define EXTSEL
#else
#define EXTSEL extern
#endif

#ifndef MAPI_BOOL
#define MAPI_BOOL BOOLEAN
#endif

#ifndef MAPI_TRUE
#define MAPI_TRUE TRUE
#endif

#ifndef MAPI_FALSE
#define MAPI_FALSE FALSE
#endif

#ifndef MAPI_U8
#define MAPI_U8 U8
#endif

#ifndef MAPI_U16
#define MAPI_U16 U16
#endif

#ifndef MAPI_U32
#define MAPI_U32 U32
#endif

#ifndef _BIT0
#define _BIT0  0x0001
#endif
#ifndef _BIT1
#define _BIT1  0x0002
#endif
#ifndef _BIT2
#define _BIT2  0x0004
#endif
#ifndef _BIT3
#define _BIT3  0x0008
#endif
#ifndef _BIT4
#define _BIT4  0x0010
#endif
#ifndef _BIT5
#define _BIT5  0x0020
#endif
#ifndef _BIT6
#define _BIT6  0x0040
#endif
#ifndef _BIT7
#define _BIT7  0x0080
#endif

#define     DEMOD_DYNAMIC_SLAVE_ID_INTERN_DTMB_1          0x01
#define     DEMOD_DYNAMIC_SLAVE_ID_INTERN_DTMB_2          0x02
#define     DEMOD_DYNAMIC_SLAVE_ID_INTERN_DTMB_3          0x03
#define     DEMOD_DYNAMIC_SLAVE_ID_INTERN_DTMB_4          0xD2

#if 0 // Use COFDM_Demodulator.h
typedef enum
{
	  COFDM_FEC_LOCK,
    COFDM_PSYNC_LOCK,
    //COFDM_FSATRACK_LOCK,
    //COFDM_FEC_LOCK,
    //COFDM_PSYNC_LOCK,
    //COFDM_ICFO_CH_EXIST_FLAG,
    //COFDM_TPS_LOCK,
} COFDM_LOCK_STATUS;
#endif


/// the demod lock status
typedef enum
{
    /// lock
    E_DTMB_DEMOD_LOCK,
    /// is checking
    E_DTMB_DEMOD_CHECKING,
    /// after checking
    E_DTMB_DEMOD_CHECKEND,
    /// unlock
    E_DTMB_DEMOD_UNLOCK,
    /// NULL state
    E_DEMOD_NULL,
} EN_DTMB_LOCK_STATUS;

/*
typedef enum
{
    E_DTMB_Layer_A = 0x00,
    E_DTMB_Layer_B = 0x01,
    E_DTMB_Layer_C = 0x02,
    E_DTMB_Layer_INVALID,
}EN_DTMB_Layer;

/// DTMB FFT enum
typedef enum
{
    /// 2K
    E_DTMB_FFT_2K = 0x00,
    /// 4k
    E_DTMB_FFT_4K = 0x01,
    /// 8k
    E_DTMB_FFT_8K = 0x02,
    /// invalid indicator
    E_DTMB_FFT_INVALID,
} EN_DTMB_FFT_VAL;

/// DTMB modulation mode
typedef enum
{
    /// DQPSK
    E_DTMB_DQPSK   = 0,
    /// QPSK
    E_DTMB_QPSK    = 1,
    /// 16QAM
    E_DTMB_16QAM   = 2,
    /// 64QAM
    E_DTMB_64QAM   = 3,
    /// invalid indicator
    E_DTMB_QAM_INVALID,
} EN_DTMB_CONSTEL_TYPE;

/// DTMB code rate
typedef enum
{
    /// 1/2
    E_DTMB_CODERATE_1_2 = 0,
    /// 2/3
    E_DTMB_CODERATE_2_3 = 1,
    /// 3/4
    E_DTMB_CODERATE_3_4 = 2,
    /// 5/6
    E_DTMB_CODERATE_5_6 = 3,
    /// 7/8
    E_DTMB_CODERATE_7_8 = 4,
    /// invalid indicator
    E_DTMB_CODERATE_INVALID,
} EN_DTMB_CODE_RATE;

/// DTMB guard interval enum
typedef enum
{
    /// 1/4
    E_DTMB_GUARD_INTERVAL_1_4  = 0,
    /// 1/8
    E_DTMB_GUARD_INTERVAL_1_8  = 1,
    /// 1/16
    E_DTMB_GUARD_INTERVAL_1_16 = 2,
    /// 1/32
    E_DTMB_GUARD_INTERVAL_1_32 = 3,
     /// invalid indicator
    E_DTMB_GUARD_INTERVAL_INVALID,
} EN_DTMB_GUARD_INTERVAL;

/// DTMB Time Interleaving enum
typedef enum
{
// 2K mode
    /// Tdi = 0
    E_DTMB_2K_TDI_0 = 0,
    /// Tdi = 4
    E_DTMB_2K_TDI_4 = 1,
    /// Tdi = 8
    E_DTMB_2K_TDI_8 = 2,
    /// Tdi = 16
    E_DTMB_2K_TDI_16 = 3,
// 4K mode
    /// Tdi = 0
    E_DTMB_4K_TDI_0 = 4,
    /// Tdi = 2
    E_DTMB_4K_TDI_2 = 5,
    /// Tdi = 4
    E_DTMB_4K_TDI_4 = 6,
    /// Tdi = 8
    E_DTMB_4K_TDI_8 = 7,
// 8K mode
    /// Tdi = 0
    E_DTMB_8K_TDI_0 = 8,
    /// Tdi = 1
    E_DTMB_8K_TDI_1 = 9,
    /// Tdi = 2
    E_DTMB_8K_TDI_2 = 10,
    /// Tdi = 4
    E_DTMB_8K_TDI_4 = 11,
    /// invalid indicator
    E_DTMB_TDI_INVALID,
} EN_DTMB_TIME_INTERLEAVING;
*/
typedef struct
{
    float    power_db;
    MAPI_U8  agc_val;
}S_DTMB_IFAGC_SSI;

typedef struct
{
    float    power_db;
    MAPI_U8  sar3_val;
}S_DTMB_RFAGC_SSI;

typedef enum
{
    E_DTMB_SSIMethod1 = 0x00,
    E_DTMB_SSIMethod2,
    E_DTMB_SSIMethod3,
}DTMB_SSIMethod;


typedef struct _extern_dtmb_func
{
    BOOLEAN (*WriteBytes)(U8,U8*,U16,U8*);
    BOOLEAN (*ReadBytes)(U8,U8*,U16,U8*);
}S_extern_dtmb_func, *pS_extern_dtmb_func;

typedef enum
{
    E_DTMB_HUMDET_EN = 0x00,   //0x00
    E_DTMB_DCR_EN,             //0x01
    E_DTMB_IIS_EN,             //0x02
    E_DTMB_CCI_EN,             //0x03
    E_DTMB_ACI_EN,             //0x04
    E_DTMB_IQB_EN,             //0x05
    E_DTMB_AUTO_IQ_SWAP_EN,    //0x06
    E_DTMB_FIX_MODE_CP_EN,     //0x07
    E_DTMB_FIX_TMCC_EN,        //0x08
    E_DTMB_WINDOWING_EN,       //0x09
    E_DTMB_FORCE_ACI_EN,       //0x0A
    E_DTMB_RF_MAX,             //0x0B
    E_DTMB_RSSI,               //0x0C
    E_DTMB_ZIF,                //0x0D
    E_DTMB_FS_L,               //0x0E
    E_DTMB_FS_H,               //0x0F
    E_DTMB_FIF_L,              //0x10
    E_DTMB_FIF_H,              //0x11
    E_DTMB_FC_L,               //0x12
    E_DTMB_FC_H,               //0x13
    E_DTMB_MODE,               //0x14
    E_DTMB_CP,                 //0x15
    E_DTMB_IQ_SWAP,            //0x16
    E_DTMB_CCI,                //0x17
    E_DTMB_IFAGC_REF,          //0x18
    E_DTMB_IFAGC_REF_ACI,      //0x19
    E_DTMB_IFAGC_REF_IIS,      //0x1A
    E_DTMB_TS_SERIAL,          //0x1B
    E_DTMB_TS_CLK_INV,         //0x1C
    E_DTMB_TS_DATA_SWAP,       //0x1D
    E_DTMB_FineSA_LeftA,       //0x1E
    DTMB_PARAM_LEN,
} DTMB_Param;

EXTSEL BOOLEAN INTERN_DTMB_Power_On_Initialization(void);
EXTSEL BOOLEAN INTERN_DTMB_Lock(COFDM_LOCK_STATUS eStatus);
EXTSEL BOOLEAN INTERN_DTMB_Config(RF_CHANNEL_BANDWIDTH BW, BOOLEAN bSerialTS, BOOLEAN bPalBG, BOOLEAN bLPSel);
EXTSEL BOOLEAN INTERN_DTMB_Active(BOOLEAN bEnable);
EXTSEL BOOLEAN INTERN_DTMB_Serial_Control(BOOLEAN bEnable);
EXTSEL float INTERN_DTMB_GetSNR(void);
EXTSEL FUNCTION_RESULT INTERN_DTMB_GetSignalStrength(WORD *strength);
EXTSEL FUNCTION_RESULT INTERN_DTMB_GetSignalQuality(WORD *quality);
EXTSEL BOOLEAN INTERN_DTMB_WriteReg(MS_U16 RegAddr, MS_U8 u8Data);
EXTSEL BOOLEAN INTERN_DTMB_ReadReg(MS_U16 RegAddr, MS_U8* pu8Data);
EXTSEL BOOLEAN INTERN_DTMB_Exit( void );
EXTSEL void INTERN_DTMB_Power_ON_OFF(MS_U8 bPowerOn);
EXTSEL BOOLEAN INTERN_DTMB_Power_Save(void);
EXTSEL void INTERN_DTMB_PAD_TS1_Enable(BOOLEAN flag);



#endif

