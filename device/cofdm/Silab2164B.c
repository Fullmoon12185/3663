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
* File  : device_tuner_si_2151.cpp
**********************************************************************/
/*@ </FileComment ID=1246257763274> @*/

/*@ <Include> @*/

#define __SILAB_2164B_C__

#include "drvIIC.h"


#include <math.h>
#include <string.h>
#include "MsCommon.h"
#include "drvIIC.h"
#include "MsOS.h"
#include "Board.h"
//#include "msb124x.h"
#include "drvGPIO.h"
#include "apiDigiTuner.h"
#include "apiDMX.h"
#include "drvPCMCIA.h"
#include "drvSYS.h"
#include "drvDMD_common.h"
#include "drvIIC.h"
#include "debug.h"
#include "MApp_Scan.h"
#include "msAPI_Memory.h"
#include "drvDMD_INTERN_DVBT.h"
#include "drvDMD_INTERN_DVBC.h"
#include "mapp_si.h"
#include "MApp_GlobalVar.h"
#include "msAPI_Timer.h"

#define FRONT_END_COUNT 1


#include "./Si2164B/SemcoSi2164B.h"
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

#include "../tuner/si2151_2164B/Si2151_Wrapper/SemSi2151.h"



#include "Si2164B/Si2164B_L1_API.c"
#include "Si2164B/Si2164B_L1_Commands.c"
#include "Si2164B/Si2164B_L1_Properties.c"
#include "Si2164B/Si2164B_L2_API.c"
#include "Si2164B/Si2164B_PropertyDump.c"
#include "Si2164B/SiLabs_API_L3_Wrapper.c"
#include "Si2164B/SemcoSi2164B.c"



#if 0
#include "Silab2164B.h"
#include "Si2164B/Si2164B_L1_API.h"


#include "Si2164B/Si2164B_L1_API.c"
#include "Si2164B/Si2164B_L1_Commands.c"
#include "Si2164B/Si2164B_L1_Properties.c"


/* Si2151 API Specific Includes */
//#include "Si2164B/Si2164B_L2_API.h"               /* Include file for this code */

//static int Si2151_printf(const char *format, ...);
#if 0
static MS_U8 platform_I2CRead(
    MS_U8  bus_id,
    MS_U8  i2c_slave_id,
    MS_U8* subadrptr,
    MS_U16 subadrlen,
    MS_U8* dataptr,
    MS_U16 datalen
    );

static MS_U8 platform_I2CWrite(
    MS_U8  bus_id,
    MS_U8  i2c_slave_id,
    MS_U8* subadrptr,
    MS_U16 subadrlen,
    MS_U8* dataptr,
    MS_U16 datalen
    );
#endif
#if 0
static void platform_ms_delay(MS_U32);
static MS_U8 platform_gpio_control(MS_U8 high);
#endif

#define DBG_TUNER(x)   //x

#define C_Si2164B_LO_ADDRESS           ((U8)  0xC8)
#define SILAB_2164B_I2C_ADDR (((U16)DEMOD_IIC_BUS<<8)| C_Si2164B_LO_ADDRESS)






//*--------------------------------------------------------------------------------------
//* Function Name       :   platform_I2CRead
//* Object              :
//* Input Parameters    :   MS_U8  bus_id,
//*                         MS_U8  i2c_slave_id,
//*                         MS_U8* subadrptr,
//*                         MS_U16 subadrlen,
//*                         MS_U8* dataptr,
//*                         MS_U16 datalen,
//* Output Parameters   :   MS_U8 bRet,
//*                         1:OK, 0:Fail
//*--------------------------------------------------------------------------------------
static MS_U8 platform_I2CRead(
    MS_U8  bus_id,
    MS_U8  i2c_slave_id,
    MS_U8* subadrptr,
    MS_U16 subadrlen,
    MS_U8* dataptr,
    MS_U16 datalen
    )
{
    /* Variable declarations */
    U8 bRet = 1;

    UNUSED(bus_id);
    UNUSED(i2c_slave_id);

    if(MDrv_IIC_ReadBytes(SILAB_2164B_I2C_ADDR, (U8) subadrlen, subadrptr, (U16) datalen, dataptr) == false)
    {
        bRet = 0;
        printf("\nTuner_IIC read  fail\n");
    }
    else
    {
        // printf("\nReadData-");
        bRet = 1;
    }

    return bRet;
}

//*--------------------------------------------------------------------------------------
//* Function Name       :   platform_I2CWrite
//* Object              :
//* Input Parameters    :   MS_U8  bus_id,
//*                         MS_U8  i2c_slave_id,
//*                         MS_U8* subadrptr,
//*                         MS_U16 subadrlen,
//*                         MS_U8* dataptr,
//*                         MS_U16 datalen,
//* Output Parameters   :   MS_U8 bRet,
//*                         1:OK, 0:Fail
//*--------------------------------------------------------------------------------------
static MS_U8 platform_I2CWrite(
    MS_U8  bus_id,
    MS_U8  i2c_slave_id,
    MS_U8* subadrptr,
    MS_U16 subadrlen,
    MS_U8* dataptr,
    MS_U16 datalen
    )
{
    /* Variable declarations */
    U8 bRet = 1;

    UNUSED(bus_id);
    UNUSED(i2c_slave_id);

    if(MDrv_IIC_WriteBytes(SILAB_2164B_I2C_ADDR, (U8) subadrlen, subadrptr, (U16) datalen, dataptr) == false)
    {
        bRet = 0;
        printf("\nTuner_IIC read  fail\n");
    }
    else
    {
        // printf("\nReadData-");
        bRet = 1;
    }

    return bRet;
}


#include "Si2164B/Si2164B_i2c_api.c"
#include "Si2164B/Silabs_L0_Connection.c"
#endif


SILABS_FE_Context     FrontEnd_Table   [FRONT_END_COUNT];
SILABS_FE_Context    *front_end[FRONT_END_COUNT];
CUSTOM_Status_Struct  FE_Status   [FRONT_END_COUNT];
CUSTOM_Status_Struct *custom_status;

L1_Si2151_Context          Si2151_FE_Table   [FRONT_END_COUNT];
L1_Si2151_Context         *Si2151_fe;
Si2151ConfigParam param;

int fe;

extern EN_TUNER_MODE m_eMode;



static CUSTOM_Standard_Enum devConvertTunerModetoSilabStd(EN_TUNER_MODE eMode)
{
    CUSTOM_Standard_Enum si_std;
    switch(eMode)
    {
        case E_TUNER_DTV_DVB_T_MODE:
            si_std=SILABS_DVB_T;
            break;
        case E_TUNER_DTV_DVB_C_MODE:
            si_std=SILABS_DVB_C;
            break;
            
        case E_TUNER_DTV_DVB_S_MODE:
            si_std=SILABS_DVB_S;
            break;
            
        case E_TUNER_ATV_PAL_MODE:
        case E_TUNER_ATV_SECAM_L_PRIME_MODE:
        case E_TUNER_ATV_NTSC_MODE:
        case E_TUNER_ATV_SECAM_L_MODE:
            
            si_std=SILABS_ANALOG;
            break;
            
        case E_TUNER_DTV_ISDB_MODE:
            si_std=SILABS_ISDB_T;
            break;
            
        case E_TUNER_DTV_DVB_T2_MODE:
            si_std=SILABS_DVB_T2;
            break;
            
    
        default:
            si_std=SILABS_DVB_T;
            break;        

    }
    return si_std;

}


static U32 devConvertRFBandWith(RF_CHANNEL_BANDWIDTH eBandWidth)
{
    return (eBandWidth+5)*1000000;

}

void Si2151_2164_SW_Init(void)
{

    fe = 0;
    front_end[fe]     = &(FrontEnd_Table[fe]);
    Si2151_fe = &(Si2151_FE_Table[0]);

    DEMOD_RESET_ON();
    system_wait(5);    
    DEMOD_RESET_OFF();

    /* Software Init */
    SiLabs_API_SW_Init   (front_end[fe], DEMOD1_MAIN2_ADDRESS, TER2_MAIN_ADDRESS, 0xFF);

    Si2151_InitSTR(Si2151_fe,0);
    SiLabs_API_Tuner_I2C_Enable(front_end[fe]);
    Si2151_Initialize(Si2151_fe, 0);
    Si2151_XoutputOn(Si2151_fe, 1, 0);
    SiLabs_API_Tuner_I2C_Disable(front_end[fe]);
    SiLabs_API_switch_to_standard (front_end[fe], SILABS_DVB_T , 0);
//    CPU_PAUSE();
}

void devDemod_Silab2164B_DVBTInit(void)
{

}


MAPI_BOOL devDemod_Silab2164B_DTV_Serial_Control(MAPI_BOOL bEnable)
{
  bEnable=bEnable;
  return TRUE;
}

/*
SiLabs_API_lock_to_carrier function
Use:      relocking function
          Used to relock on a channel for the required standard
Parameter: standard the standard to lock to
Parameter: freq                the frequency to lock to    (in Hz for TER, in kHz for SAT)
Parameter: bandwidth_Hz        the channel bandwidth in Hz (only for DVB-T, DVB-T2, ISDB-T)
Parameter: dvb_t_stream        the HP/LP stream            (only for DVB-T)
Parameter: symbol_rate_bps     the symbol rate in baud/s   (for DVB-C, MCNS and SAT)
Parameter: dvb_c_constellation the DVB-C constellation     (only for DVB-C)
Parameter: data_slice_id       the DATA SLICE Id           (only for DVB-C2 when num_dslice  > 1)
Parameter: plp_id              the PLP Id                  (only for DVB-T2 and DVB-C2 when num_plp > 1)
Parameter: T2_lock_mode        the DVB-T2 lock mode        (0='ANY', 1='T2-Base', 2='T2-Lite')
Return:    1 if locked, 0 otherwise
*/



//signed int   SiLabs_API_lock_to_carrier          (SILABS_FE_Context *front_end, unsigned char standard, signed int freq, signed int bandwidth_Hz, unsigned char stream, unsigned int symbol_rate_bps, unsigned char constellation, unsigned char polarization, unsigned char band, signed int data_slice_id, signed int plp_id, unsigned char T2_lock_mode)


MAPI_BOOL devDemod_Silab2164B_DTV_SetFrequency(MAPI_U32 u32Frequency, RF_CHANNEL_BANDWIDTH eBandWidth,RF_CHANNEL_HP_LP eHPLP,U8 u8PlpID)
{

    CUSTOM_Standard_Enum std;

    std=devConvertTunerModetoSilabStd(m_eMode);

    SiLabs_API_lock_to_carrier(front_end[fe],std,u32Frequency*1000,devConvertRFBandWith(eBandWidth), (unsigned char)eHPLP,0,0,0,0,0,u8PlpID,0);

    printf("\r\n devDemod_Silab2164B_DTV_SetFrequency %d", u32Frequency*1000);
    //CPU_PAUSE();
    //u32Frequency= u32Frequency;
    //eBandWidth = eBandWidth;
    //bPalBG = bPalBG;
    //bLPsel = bLPsel;
    
    
    return TRUE;
}


MAPI_U16 devDemod_Silab2164B_DTV_GetSignalStrength(void)
{
    return 0;
    
}


MAPI_U16 devDemod_Silab2164B_DTV_GetCellID(void)
{
    return 0;
}

MAPI_BOOL devDemod_Silab2164B_DTV_DVB_T_Get_TPS_Parameter(MAPI_U16 * pu16TPS_parameter, E_SIGNAL_TYPE eSignalType)
{
  pu16TPS_parameter = pu16TPS_parameter;
  eSignalType=eSignalType;
  return TRUE;

}


void mdev_CofdmSetFrequency(const ICE_TuneToParams* params, RF_CHANNEL_BANDWIDTH bandWith)
{
    bandWith=bandWith;
    params=params;
    
}

void devDemod_Silab2164B_DVBT2Init(void)
{

}

//MAPI_BOOL devDemod_Silab2164B_DVBT2_Ready(void)
//{
//}


void devDemod_Silab2164B_DVBT2_SetPower(BOOL bPowerOn)
{
    bPowerOn=bPowerOn;
    return;
}


MAPI_BOOL devDemod_Silab2164B_Ready(void)
{

    return TRUE;
}


MAPI_BOOL devDemod_Silab2164B_DVBT2_GetLockStatus(void)
{

    return FALSE;
}


MAPI_BOOL devDemod_Silab2164B_DVBT2_GetPostBER(float *p_postBer)
{
    *p_postBer=0;

    return FALSE;

}

MAPI_U16 devDemod_Silab2164B_DVBT_GetSignalQuality(void)
{


    return 0;
}

MAPI_U16 devDemod_Silab2164B_DVBT2_GetSignalQuality(void)
{


    return 0;
}




MAPI_BOOL mdev_CofdmGetSignalStrength(MS_U16 *strength)
{
    *strength=0;
    return TRUE;

}

MS_U8 mdev_CofdmGetPlpIDList(void)
{
    return 0;
}

MS_BOOL mdev_CofdmGetCurrentPlpID(MS_U8 *pu8PlpID)
{
    pu8PlpID=pu8PlpID;
    return FALSE;
}

MS_BOOL mdev_CofdmSetPlpGroupID(MS_U8 u8PlpID, MS_U8 u8GroupID)
{

    u8PlpID=u8PlpID;
    u8GroupID=u8GroupID;

    return TRUE;
}

void mdev_ControlTSOutput(BOOLEAN bEnable)
{
    bEnable=bEnable;
    
}


void mdev_ControlAGCOutput(BOOLEAN bEnable)
{
  bEnable=bEnable;
}



MAPI_BOOL devDemod_Silab2164B_DVBT_GetPostBER(float *p_postBer)
{
    p_postBer=p_postBer;
    return TRUE;

}


MAPI_BOOL devDemod_Silab2164B_DVBT_GetLockStatus(COFDM_LOCK_STATUS eStatus)
{
    eStatus=eStatus;
    //return FALSE;
    SiLabs_API_FE_status (front_end[fe], custom_status);
    return custom_status->dl_lock;
    //return SiLabs_API_Lock_status();
}

EN_FRONTEND_SIGNAL_CONDITION devDemod_Silab2164B_GetSNR(void)
{
    return E_FE_SIGNAL_NO;    
}


//DTV_GetSignalStrength


