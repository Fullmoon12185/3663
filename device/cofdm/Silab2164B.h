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

* Class : device_tuner_si_2151
* File  : device_tuner_si_2151.h
**********************************************************************/

#ifndef __SILAB_2164B_H__
#define __SILAB_2164B_H__

#ifdef __SILAB_2164B_C__
#define INTERFACE
#else
#define INTERFACE extern
#endif





#include "MsCommon.h"
#include "MsOS.h"
#include "Board.h"
#include "Tuner.h"
#include "msAPI_DTVSystem.h"
#include "drvIIC.h"
#include "apiDTVSatTable.h"
#include "drvDemod.h"
// For basic define
#include "msAPI_Demod_DVBC.h"

#define mcSHOW_HW_MSG(x)   // printf x
#define mcSHOW_DBG_MSG4(x) // printf x

//#define NULL 0

#if 0

//#include "./Si2164B/SemcoSi2164B.h"
#include "./Si2164B/Si2164B_Commands.h"
#include "./Si2164B/Si2164B_Commands_Prototypes.h"
#include "./Si2164B/Si2164B_L1_API.h"

#include "./Si2164B/Si2164B_L2_API.h"
#include "./Si2164B/Si2164B_Platform_Definition.h"
#include "./Si2164B/Si2164B_Properties.h"
#include "./Si2164B/Si2164B_Properties_Functions.h"
#include "./Si2164B/Si2164B_PropertyDump.h"
#include "./Si2164B/SiDemod_Si2164B.h"
#include "./Si2164B/SiLabs_API_L3_Wrapper.h"
#endif



//#include "Si2164B/Si2164B_L1_Commands.h"
//#include "Si2164B/Si2164B_L1_Properties.h"
//#include "Si2164B/Si2164B_L2_API.h"
//#include "Si2164B/Si2164B_PropertyDump.h"
//#include "Si2164B/SiLabs_API_L3_Wrapper.h"
//#include "Si2164B/SemcoSi2164B.h"
//#include "SiDemod_Si2164B.h"




/*@ <Definitions> @*/
#ifndef MAPI_U8
#define MAPI_U8 MS_U8
#endif

#ifndef MAPI_U16
#define MAPI_U16 MS_U16
#endif

#ifndef MAPI_U32
#define MAPI_U32 MS_U32
#endif

#ifndef MAPI_BOOL
#define MAPI_BOOL MS_BOOL
#endif

#ifndef MAPI_S16
#define MAPI_S16 MS_S16
#endif

#define MAPI_TRUE TRUE
#define MAPI_FALSE FALSE


typedef enum
{
    TS_MODUL_MODE,
    TS_FFT_VALUE,
    TS_GUARD_INTERVAL,
    TS_CODE_RATE,
    TS_PREAMBLE,
    TS_S1_SIGNALLING,
    TS_PILOT_PATTERN,
    TS_BW_EXT,
    TS_PAPR_REDUCTION,
    TS_OFDM_SYMBOLS_PER_FRAME,
    TS_T2_GUARD_INTERVAL,
    TS_T2_FFT_VALUE,
    TS_PARAM_MAX_NUM
}E_SIGNAL_TYPE;


#if (ENABLE_DVB_T2)
#include "Enum_Device.h"
//#include "COFDM_Demodulator.h"
//#include "drvGPIO.h"

#if 0
typedef enum
{
    E_Network_Type_DVBT =1,
    E_Network_Type_DVBC ,
    E_Network_Type_Analog_T ,
    E_Network_Type_Analog_C ,
    E_Network_Type_DVBT2
} Network_Type;

typedef struct
{
    MS_BOOL IsDTVActive;
    RF_CHANNEL_BANDWIDTH RfBw;
    MS_U32 RfFreqInKHz;
}CUR_RF_INFO;
#endif


typedef enum
{
    //Internal Demodulator
    E_COFDM_INTERNAL_DVBT,
    E_COFDM_INTERNAL_DVBC,
    //External Demodulator
    E_COFDM_EXTERNAL_DVBT2,
    E_COFDM_EXTERNAL_DVBS,
    E_COFDM_INVALID
}CUR_COFDM_CFG;
#endif


typedef enum
{
    TS_CLK_NO_INV = 0,
    TS_CLK_INVERSE = 1,

    TS_CLK_MODE_MAX_NUM
}E_TS_CLK_MODE;

typedef struct
{
    MS_U16 Constellation    : 3;
    MS_U16 Hierarchy         :3;
    MS_U16 LpCodeRate         :3;
    MS_U16 HpCodeRate         :3;
    MS_U16 GI                       :2;
    MS_U16 FFTmode         :1;
    MS_U16 Priority             :1;
} S_TPS_INFO;

typedef struct
{
    MS_U64 Constellation        :3;
    MS_U64 CodeRate              :3;
    MS_U64 GI                          :3;
    MS_U64 FFTmode               :3;
    MS_U64 Preamble              :1;
    MS_U64 S1Signalling          :2;
    MS_U64 l1PrePilot              :4;
    MS_U64 BwExt                     :1;
    MS_U64 l1PrePAPRReduction :3;
    MS_U64 ofdm_symbols_per_frame :12;
} S_TPS_INFO_T2;

typedef enum
{
    E_CONST_QPSK,    //DVBT
    E_CONST_16QAM,  //DVBT DVBC
    E_CONST_64QAM,  //DVBT DVBC
    E_CONST_32QAM,  //DVBC
    E_CONST_128QAM,  //DVBC
    E_CONST_256QAM,  //DVBC
    E_CONST_INVALID,
}E_TPS_CONST;

typedef enum
{
    E_HIERARCHY_NONE,
    E_HIERARCHY_APLHA1,
    E_HIERARCHY_APLHA2,
    E_HIERARCHY_APLHA4,
}E_TPS_HIERARCHY;

typedef enum
{
    E_FEC_1_2,
    E_FEC_2_3,
    E_FEC_3_4,
    E_FEC_5_6,
    E_FEC_7_8,
    E_FEC_4_5,
    E_FEC_6_7,
    E_FEC_8_9,
}E_TPS_FEC;

typedef enum
{
    E_FEC_T2_1_2,
    E_FEC_T2_3_5,
    E_FEC_T2_2_3,
    E_FEC_T2_3_4,
    E_FEC_T2_4_5,
    E_FEC_T2_5_6,
}E_TPS_FEC_T2;

typedef enum
{
    E_GI_1_32,
    E_GI_1_16,
    E_GI_1_8,
    E_GI_1_4,
    E_GI_1_128,  //DVBT2
    E_GI_19_128,  //DVBT2
    E_GI_19_256,  //DVBT2
}E_TPS_GI;

typedef enum
{
    E_FFT_2K,
    E_FFT_8K,
    E_FFT_1K,    //DVBT2
    E_FFT_4K,    //DVBT2
    E_FFT_16K,  //DVBT2
    E_FFT_32K,  //DVBT2
}E_TPS_FFT;

typedef enum
{
    E_PREAMBLE_NOT_MIXED = 0x00, //DVBT2
    E_PREAMBLE_MIXED = 0x01,  //DVBT2
}E_TPS_PREAMBLE;

typedef enum
{
    E_S1_SIGNALLING_T2_SISO = 0x00,  //DVBT2
    E_S1_SIGNALLING_T2_MISO = 0x01,  //DVBT2
    E_S1_SIGNALLING_NON_T2 = 0x02,  //DVBT2
    E_S1_SIGNALLING_RESERVED,  //DVBT2
}E_TPS_S1_SIGNALLING;

typedef enum
{
    E_l1_PRE_PILOT_PATTERN_1 = 0x00,  //DVBT2
    E_l1_PRE_PILOT_PATTERN_2 = 0x01,  //DVBT2
    E_l1_PRE_PILOT_PATTERN_3 = 0x02,  //DVBT2
    E_l1_PRE_PILOT_PATTERN_4 = 0x03,  //DVBT2
    E_l1_PRE_PILOT_PATTERN_5 = 0x04,  //DVBT2
    E_l1_PRE_PILOT_PATTERN_6 = 0x05,  //DVBT2
    E_l1_PRE_PILOT_PATTERN_7 = 0x06,  //DVBT2
    E_l1_PRE_PILOT_PATTERN_8 = 0x07,  //DVBT2
    E_l1_PRE_PILOT_PATTERN_RESERVED,  //DVBT2
}E_TPS_l1_PRE_PILOT;

typedef enum
{
    E_BW_NORMAL_MODE = 0x00,  //DVBT2
    E_BW_EXTENSION_MODE = 0x01,  //DVBT2
}E_TPS_BW_EXT;

typedef enum
{
    E_l1_PRE_PAPR_REDUCTION_NONE = 0x00,  //DVBT2
    E_l1_PRE_PAPR_REDUCTION_ACE = 0x01,  //DVBT2
    E_l1_PRE_PAPR_REDUCTION_TR = 0x02,  //DVBT2
    E_l1_PRE_PAPR_REDUCTION_TR_AND_ACE = 0x03,  //DVBT2
    E_l1_PRE_PAPR_REDUCTION_RESERVED,  //DVBT2
}E_TPS_l1_PRE_PEAK_AVERAGE_POWER_RATIO;

typedef enum
{
    agc_ref_small = 0,
    agc_ref_large,
    agc_ref_aci,
    ripple_switch_th_l,
    ripple_switch_th_h,

    TUNED_PARAM_MAX_NUM
}E_TUNED_PARAM;

typedef enum
{
    DEMOD_CFG_DVBT = 0,
    DEMOD_CFG_DVBC,
    DEMOD_CFG_ATV,
    DEMOD_CFG_DVBT2,
    DEMOD_CFG_NUM,
} E_DEMOD_DSP_CFG;
typedef enum
{
    ICE_network_dvbt2 = 0,
    ICE_network_dvbt,
    ICE_network_dvbc,
    ICE_network_analog,
    ICE_network_num
}E_ICE_NETWORK_TYPE;
typedef struct
{
    U32 channel_frequency_khz;
    E_ICE_NETWORK_TYPE  network_type;
}ICE_TuneToParams;




extern void devDemod_Silab2164B_Init(void);
extern void devDemod_Silab2164B_DVBTInit(void);
extern MAPI_BOOL devDemod_Silab2164B_DTV_Serial_Control(MAPI_BOOL bEnable);
//extern MAPI_BOOL devDemod_Silab2164B_DTV_SetFrequency(MAPI_U32 u32Frequency, RF_CHANNEL_BANDWIDTH eBandWidth, MAPI_BOOL bPalBG, MAPI_BOOL bLPsel);
extern MAPI_BOOL devDemod_Silab2164B_DTV_SetFrequency(MAPI_U32 u32Frequency, RF_CHANNEL_BANDWIDTH eBandWidth,RF_CHANNEL_HP_LP eHPLP,U8 u8PlpID);

extern MAPI_U16 devDemod_Silab2164B_DTV_GetSignalStrength(void);
extern MAPI_U16 devDemod_Silab2164B_DTV_GetCellID(void);
extern MAPI_BOOL devDemod_Silab2164B_DTV_DVB_T_Get_TPS_Parameter(MAPI_U16 * pu16TPS_parameter, E_SIGNAL_TYPE eSignalType);
extern void mdev_CofdmSetFrequency(const ICE_TuneToParams* params, RF_CHANNEL_BANDWIDTH bandWith);
extern MAPI_BOOL devDemod_Silab2164B_DVBT_GetPostBER(float *p_postBer);
extern MAPI_BOOL devDemod_Silab2164B_DVBT_GetLockStatus(COFDM_LOCK_STATUS eStatus);
extern EN_FRONTEND_SIGNAL_CONDITION devDemod_Silab2164B_GetSNR(void);



extern void devDemod_Silab2164B_DVBT2Init(void);
extern MAPI_BOOL devDemod_Silab2164B_Ready(void);
extern void devDemod_Silab2164B_DVBT2_SetPower(BOOL bPowerOn);
extern MAPI_BOOL devDemod_Silab2164B_DVBT2_GetLockStatus(void);
extern MAPI_BOOL devDemod_Silab2164B_DVBT2_GetPostBER(float *p_postBer);

extern MAPI_U16 devDemod_Silab2164B_DVBT_GetSignalQuality(void);
extern MAPI_U16 devDemod_Silab2164B_DVBT2_GetSignalQuality(void);


extern MAPI_BOOL mdev_CofdmGetSignalStrength(MS_U16 *strength);

extern MS_U8 mdev_CofdmGetPlpIDList(void);
extern MS_BOOL mdev_CofdmGetCurrentPlpID(MS_U8 *pu8PlpID);
extern MS_BOOL mdev_CofdmSetPlpGroupID(MS_U8 u8PlpID, MS_U8 u8GroupID);
extern void mdev_ControlTSOutput(BOOLEAN bEnable);
extern void mdev_ControlAGCOutput(BOOLEAN bEnable);

extern void Si2151_2164_SW_Init(void);


#endif // __SILAB_2151_H__

