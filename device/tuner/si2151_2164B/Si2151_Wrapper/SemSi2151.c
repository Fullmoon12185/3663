/*-------------------------------------------------------------------------
File        : SemSi2151.c
Description : Modified wrapper file  to use Si2151 driver 
Revision History:
20130829 Created by S.Yu Email seongjun.yu@samsung.com
-------------------------------------------------------------------------*/

#include "../UserDefinedIIC.h"
#include "SemSi2151.h"



static unsigned char bCheckInjection_83_25;//20121127, atv ch6 low injection

// idx: index 0-address c0, 1-address c2
/*-------------------------------------------------------------------------
function name      Si2151_InitSTR
author             
Description :      Initialization of Si2151 Structure
return             NO_Si2151_ERROR
-------------------------------------------------------------------------*/
int Si2151_InitSTR(L1_Si2151_Context *Si2151, char idx)
{
	//Si2151_L1_API_Init(Si2151,Si2151_BASE_ADDRESS+(6*idx));
	Si2151_L1_API_Init(Si2151,Si2151_BASE_ADDRESS+(2*idx));  //Main, sub, c6
	return NO_Si2151_ERROR;
}

// clk_mode: 0-외부 XTAL로부터 직접 입력받음, 1-다른 Si2151의 xout 으로부터 clk 받음
/*-------------------------------------------------------------------------
function name      Si2151_Initialize
author             
Description :      Initialization of Si2151
param :            char rettxtBuffer[256];
                   double retdval = 0;
                   char *rettxt;
                   rettxt = rettxtBuffer;
return             ERROR_Si2151_SENDING_COMMAND
                   NO_Si2151_ERROR
-------------------------------------------------------------------------*/
int Si2151_Initialize(L1_Si2151_Context *Si2151,int clk_mode)
{
	if(clk_mode){
		Si2151->cmd->power_up.clock_mode =  Si2151_POWER_UP_CMD_CLOCK_MODE_EXTCLK;		//sub
		Si2151->cmd->power_up.en_xout    =  Si2151_POWER_UP_CMD_EN_XOUT_DIS_XOUT;
		Si2151->cmd->config_clocks.clock_mode = Si2151_CONFIG_CLOCKS_CMD_CLOCK_MODE_EXTCLK;
	}
	else{
		Si2151->cmd->power_up.clock_mode =  Si2151_POWER_UP_CMD_CLOCK_MODE_XTAL;	//main
		Si2151->cmd->power_up.en_xout    =  Si2151_POWER_UP_CMD_EN_XOUT_EN_XOUT;
		Si2151->cmd->config_clocks.clock_mode = Si2151_CONFIG_CLOCKS_CMD_CLOCK_MODE_XTAL;
	}
	if (Si2151_Init(Si2151) != 0 )
		return ERROR_Si2151_SENDING_COMMAND;

	return NO_Si2151_ERROR;
}

/************************************************************************************************************************
  NAME: Si2151_DisplayStatusByte
  DESCRIPTION:Read Si2151 STATUS byte and display status bits
  Parameter:  Si2151 Context (I2C address)
  Returns:    Si2151/I2C transaction error code
************************************************************************************************************************/
int  Si2151_DisplayStatusByte(L1_Si2151_Context *Si2151)
{
    int error_code;

    error_code = Si2151_L1_CheckStatus(Si2151);
    if (error_code)
    {
        return error_code;
    }
	//아래의 값을 사용할 수 있다.
	//CTS: clear to send, 0: wait, 1: complete
	//INT: interrupt 표시, 0: Not Triggered, 1: Triggered

    return 0;
}

/************************************************************************************************************************
  NAME : Si2151_Tuner_Block_VCO_Control
  DESCRIPTION:  Write Blocked VCO
  Parameter:  int select_tu : 0(Main), 1(Sub)
  Returns:    I2C transaction error code
************************************************************************************************************************/

int Si2151_Tuner_Block_VCO_Control( L1_Si2151_Context *write_api, int vco_code)
{
/* VCO blocking capability has been added to support the dual tuner to avoid VCO pulling.   After the tune,  TUNER_STATUS is
    executed and the VCO code is sent to the other tuner via setting the property TUNER_BLOCKED_VCO in the other tuner object (!fe).
    See the function Si2151_Tuner_Block_VCO for details on setting the TUNER_VCO_CODE property.
  */
    if (Si2151_Tuner_Block_VCO(write_api,vco_code) != NO_Si2151_ERROR)
    {
        SiTRACE("Error sending Si2151_Tuner_Block_VCO\n");

		return ERROR_Si2151_SENDING_COMMAND;
    }
	
	return NO_Si2151_ERROR;
}

/************************************************************************************************************************
  NAME : Si2151_GetRevision
  DESCRIPTION:  Execute Si2151 GET_REV function and display response fields
  Parameter:  Si2151 Context (I2C address)
  Returns:    I2C transaction error code
************************************************************************************************************************/
int  Si2151_GetRevision(L1_Si2151_Context *Si2151,Si2151_GET_REV_CMD_REPLY_struct *pRev)
{
    if (Si2151_L1_GET_REV(Si2151) != NO_Si2151_ERROR)
    {
        return ERROR_Si2151_SENDING_COMMAND;
    }
	//아래의 값을 사용할 수 있다.
	pRev->chiprev =Si2151->rsp->get_rev.chiprev;	//hex
	pRev->cmpbuild = Si2151->rsp->get_rev.cmpbuild;	//ascii
	pRev->cmpmajor = Si2151->rsp->get_rev.cmpmajor;	//ascii
	pRev->cmpminor = Si2151->rsp->get_rev.cmpminor;	//hex
	pRev->fwmajor = Si2151->rsp->get_rev.fwmajor;	//ascii
	pRev->fwminor = Si2151->rsp->get_rev.fwminor;	//ascii
	pRev->patch = Si2151->rsp->get_rev.patch;		//0~255
	pRev->pn = Si2151->rsp->get_rev.pn;				//1:A, 2:B

    return 0;
}


/************************************************************************************************************************
  NAME: Si2151_GetTunerStatus
  DESCRIPTION:        Reports tuner status from TUNER_STATUS response
  Parameter:  Pointer to Si2151 Context (I2C address)
  Returns:    I2C transaction error code
************************************************************************************************************************/
int Si2151_GetTunerStatus (L1_Si2151_Context *Si2151,Si2151_TUNER_STATUS_CMD_REPLY_struct *pRev)
{
    if (Si2151_L1_TUNER_STATUS(Si2151) != NO_Si2151_ERROR) {
        return ERROR_Si2151_SENDING_COMMAND;
    }
	//아래의 값을 함수 밖에서 사용할 수 있다.
	pRev->STATUS->tunint = Si2151->rsp->tuner_status.STATUS->tunint;
	pRev->STATUS->atvint = Si2151->rsp->tuner_status.STATUS->atvint;
	pRev->STATUS->dtvint = Si2151->rsp->tuner_status.STATUS->dtvint;
	pRev->STATUS->err = Si2151->rsp->tuner_status.STATUS->err;
	pRev->STATUS->cts = Si2151->rsp->tuner_status.STATUS->cts;
	pRev->vco_code = Si2151->rsp->tuner_status.vco_code;			//vco freq code
	pRev->rssi = Si2151->rsp->tuner_status.rssi;
	pRev->freq = Si2151->rsp->tuner_status.freq;					//hz
	pRev->mode = Si2151->rsp->tuner_status.mode;					//0:dtv,1:atv

    return 0;
}

/* Si2151_ConfigATV 주요 파라미터
api->prop->atv_audio_mode.audio_sys
api->prop->atv_video_mode.video_sys
api->prop->atv_video_mode.color 
api->prop->atv_video_mode.invert_signal
api->prop->atv_cvbs_out_fine.offset
api->prop->atv_cvbs_out_fine.amp
api->prop->atv_sif_out.offset 
api->prop->atv_sif_out.amp
api->prop->atv_afc_range.range_khz
*/

int Si2151_ConfigATV(L1_Si2151_Context *Si2151)
{
	if(Si2151_downloadATVProperties(Si2151) != NO_Si2151_ERROR)
		return ERROR_Si2151_SENDING_COMMAND;

	return NO_Si2151_ERROR;
}


/* Si2151_ConfigDTV 주요 파라미터
api->prop->dtv_config_if_port.dtv_out_type
api->prop->dtv_config_if_port.dtv_agc_source
api->prop->dtv_lif_freq.offset
api->prop->dtv_mode.bw
api->prop->dtv_mode.invert_spectrum
api->prop->dtv_mode.modulation
api->prop->dtv_agc_speed.if_agc_speed 
*/
int Si2151_ConfigDTV(L1_Si2151_Context *Si2151)
{
	if(Si2151_downloadDTVProperties(Si2151) != NO_Si2151_ERROR)
		return ERROR_Si2151_SENDING_COMMAND;

	return NO_Si2151_ERROR;
}

/* Si2151_ConfigTuner 주요 파라미터
api->prop->tuner_return_loss.mode
*/
int Si2151_ConfigTuner(L1_Si2151_Context *Si2151)
{
	if(Si2151_downloadTUNERProperties(Si2151) != NO_Si2151_ERROR)
		return ERROR_Si2151_SENDING_COMMAND;

	return NO_Si2151_ERROR;
}

int Si2151_ConfigCommon(L1_Si2151_Context *Si2151)
{
	if(Si2151_downloadCOMMONProperties(Si2151) != NO_Si2151_ERROR)
		return ERROR_Si2151_SENDING_COMMAND;

	return NO_Si2151_ERROR;
}

void Si2151_ConfigTable(L1_Si2151_Context *Si2151,unsigned char video_sys, unsigned char modulation,unsigned char atvifagcspeed)
{

	switch(video_sys)
	{
	case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_M:
	case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_N:
	//	Si2151->prop->atv_video_mode.color = Si2151_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC;
		//if(((Si2151->rsp->part_info.part == 78) && (Si2151->rsp->part_info.romid >= 0x33)) || ((Si2151->rsp->part_info.part == 90) && (Si2151->rsp->part_info.romid >= 0x40)))
			Si2151->prop->atv_agc_speed.if_agc_speed = Si2151_AtvIfAgcSpeedMapping(atvifagcspeed);
		//else
		//	Si2151->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_251;
		break;
	case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_B:
	case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_GH:
		
	//	Si2151->prop->atv_video_mode.color = Si2151_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC;
		//if(((Si2151->rsp->part_info.part == 78) && (Si2151->rsp->part_info.romid >= 0x33)) || ((Si2151->rsp->part_info.part == 90) && (Si2151->rsp->part_info.romid >= 0x40)))
			Si2151->prop->atv_agc_speed.if_agc_speed = Si2151_AtvIfAgcSpeedMapping(atvifagcspeed);
		//else
		//	Si2151->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_251;
		break;
	case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_I:
//		Si2151->prop->atv_video_mode.color = Si2151_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC;
		//if(((Si2151->rsp->part_info.part == 78) && (Si2151->rsp->part_info.romid >= 0x33)) || ((Si2151->rsp->part_info.part == 90) && (Si2151->rsp->part_info.romid >= 0x40)))
			Si2151->prop->atv_agc_speed.if_agc_speed = Si2151_AtvIfAgcSpeedMapping(atvifagcspeed);
		//else
		//	Si2151->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_251;
		break;
	case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_DK:
//		Si2151->prop->atv_video_mode.color = Si2151_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC;
		//if(((Si2151->rsp->part_info.part == 78) && (Si2151->rsp->part_info.romid >= 0x33)) || ((Si2151->rsp->part_info.part == 90) && (Si2151->rsp->part_info.romid >= 0x40)))
			Si2151->prop->atv_agc_speed.if_agc_speed = Si2151_AtvIfAgcSpeedMapping(atvifagcspeed);
		//else
		//	Si2151->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_251;
		break;
	case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_L:
	case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_LP:
//		Si2151->prop->atv_video_mode.color = Si2151_ATV_VIDEO_MODE_PROP_COLOR_SECAM;
		
		if(((Si2151->rsp->part_info.part == 78) && (Si2151->rsp->part_info.romid >= 0x33)) || ((Si2151->rsp->part_info.part == 90) && (Si2151->rsp->part_info.romid >= 0x40)))
			Si2151->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO;
		else
			Si2151->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_185;
		break;
	default:
		break;
	}


	switch(modulation)
	{
	case Si2151_DTV_MODE_PROP_MODULATION_DVBT:
	case Si2151_DTV_MODE_PROP_MODULATION_ATSC:
	case Si2151_DTV_MODE_PROP_MODULATION_ISDBT:
	case Si2151_DTV_MODE_PROP_MODULATION_DTMB:
		if((Si2151->rsp->part_info.part == 57) && (Si2151->rsp->part_info.romid >= 0x50))
		{
			//Si2151->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_AUTO;
			Si2151->prop->dtv_agc_speed.if_agc_speed = Si2151_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO;
			Si2151->prop->dtv_agc_speed.agc_decim = Si2151_DTV_AGC_SPEED_PROP_AGC_DECIM_DEFAULT;
		}
		else{
			//Si2151->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P6DB;
			Si2151->prop->dtv_agc_speed.if_agc_speed = Si2151_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_121;
			Si2151->prop->dtv_agc_speed.agc_decim = Si2151_DTV_AGC_SPEED_PROP_AGC_DECIM_DEFAULT;
		}
		break;
	case Si2151_DTV_MODE_PROP_MODULATION_DVBC:
	case Si2151_DTV_MODE_PROP_MODULATION_QAM_US:
		if((Si2151->rsp->part_info.part == 57) && (Si2151->rsp->part_info.romid >= 0x50))
		{
			//Si2151->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_AUTO;
			Si2151->prop->dtv_agc_speed.if_agc_speed = Si2151_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO;
			Si2151->prop->dtv_agc_speed.agc_decim = Si2151_DTV_AGC_SPEED_PROP_AGC_DECIM_DEFAULT;
		}
		else{
			//Si2151->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M8DB;
			Si2151->prop->dtv_agc_speed.if_agc_speed = Si2151_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_121;
			Si2151->prop->dtv_agc_speed.agc_decim = Si2151_DTV_AGC_SPEED_PROP_AGC_DECIM_DEFAULT;
		}
		break;
	default:
		break;
	}
}

// ATV, DTV, Tuner configuration 통합.
int Si2151_SetConfig(L1_Si2151_Context *Si2151, Si2151ConfigParam *param,unsigned char atvifagcspeed)
{
	Si2151_ConfigTable(Si2151,param->atv_video_mode_video_sys, param->dtv_mode_modulation,atvifagcspeed);

	//ATV
	//atv_video_mode.video_sys,atv_video_mode.invert_signal,atv_video_mode.color
	Si2151->prop->atv_video_mode.video_sys = param->atv_video_mode_video_sys;
//	Si2151->prop->atv_video_mode.color = param->atv_video_mode_color;
	Si2151->prop->atv_video_mode.invert_spectrum = param->atv_video_mode_invert_signal;
	if(Si2151_L1_SetProperty2(Si2151, Si2151_ATV_VIDEO_MODE_PROP_CODE) != NO_Si2151_ERROR)
    {
      return ERROR_Si2151_SENDING_COMMAND;
    }
    //Analog LIF offet.     
    switch(param->atv_video_mode_video_sys)
	{
		case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_B:
			param->atv_lif_freq_offset = 6400-2250;
			break;
		
		case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_M:
		case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_N:
			param->atv_lif_freq_offset = 5400-1750;
			break;

		case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_GH:
			param->atv_lif_freq_offset = 6750-2750;
			break;
			
		case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_I:
			param->atv_lif_freq_offset = 7250-2750;
			break;
		case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_L:
			param->atv_lif_freq_offset = 6250-2750;
			break;
		case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_DK:
			param->atv_lif_freq_offset = 7250-2750;
			break;
		
			//20130114,Issue: SECAM-DK/PAL-BG, 199.25 Mhz horizontal noise by ACI
		case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_LP:
			param->atv_lif_freq_offset = 1250+2750;
			break;

		default:
			return -1;
    }
	 Si2151->prop->atv_lif_freq.offset = param->atv_lif_freq_offset;
	if(Si2151_L1_SetProperty2(Si2151, Si2151_ATV_LIF_FREQ_PROP_CODE) != NO_Si2151_ERROR)
    {
      return ERROR_Si2151_SENDING_COMMAND;
    }

	//atv_config_if_port.atv_out_type //20131230. S.Yu
	if(Si2151_L1_SetProperty2(Si2151, Si2151_ATV_CONFIG_IF_PORT_PROP_CODE) != NO_Si2151_ERROR)
    {
       return ERROR_Si2151_SENDING_COMMAND;
    }
		
	//atv_agc_speed.if_agc_speed
	if(Si2151_L1_SetProperty2(Si2151, Si2151_ATV_AGC_SPEED_PROP_CODE) != NO_Si2151_ERROR)
    {
       return ERROR_Si2151_SENDING_COMMAND;
    }
	
	//atv locking time improvement
	Si2151->prop->atv_artificial_snow.period = Si2151_ATV_ARTIFICIAL_SNOW_PROP_PERIOD_SHORT;//long:0(def), short:1
	
	//20121228,SAVINA DETUNE ISSUE
	Si2151->prop->atv_artificial_snow.period = Si2151_ATV_ARTIFICIAL_SNOW_PROP_PERIOD_LONG;
	Si2151->prop->atv_artificial_snow.gain = Si2151_ATV_ARTIFICIAL_SNOW_PROP_GAIN_DEFAULT;//default 0 -> 4
	//end
	if(Si2151_L1_SetProperty2(Si2151, Si2151_ATV_ARTIFICIAL_SNOW_PROP_CODE) != NO_Si2151_ERROR)
    {
       return ERROR_Si2151_SENDING_COMMAND;
    }
	
	//DTV
	Si2151->prop->dtv_config_if_port.dtv_out_type = param->dtv_out_type;
//	Si2151->prop->dtv_config_if_port.dtv_agc_source = param->dtv_agc_source;
	if(Si2151_L1_SetProperty2(Si2151, Si2151_DTV_CONFIG_IF_PORT_PROP_CODE) != NO_Si2151_ERROR)
    {
       return ERROR_Si2151_SENDING_COMMAND;
    }

	Si2151->prop->dtv_lif_freq.offset = param->dtv_lif_freq_offset;
	if(Si2151_L1_SetProperty2(Si2151, Si2151_DTV_LIF_FREQ_PROP) != NO_Si2151_ERROR)
    {
       return ERROR_Si2151_SENDING_COMMAND;
    }
	
	Si2151->prop->dtv_mode.bw = param->dtv_mode_bw;
	Si2151->prop->dtv_mode.modulation = param->dtv_mode_modulation;
	Si2151->prop->dtv_mode.invert_spectrum = param->dtv_mode_invert_spectrum;
	if(Si2151_L1_SetProperty2(Si2151, Si2151_DTV_MODE_PROP_CODE) != NO_Si2151_ERROR)
    {
       return ERROR_Si2151_SENDING_COMMAND;
    }
	
	//dtv_agc_speed.if_agc_speed
	Si2151->prop->dtv_agc_speed.if_agc_speed  =	param->dtv_if_agc_speed;	//Si2151_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO ; /* (default 'AUTO') */
	Si2151->prop->dtv_agc_speed.agc_decim     =	param->dtv_agc_decim;	//Si2151_DTV_AGC_SPEED_PROP_AGC_DECIM_OFF     ; /* (default 'OFF') */

	if(Si2151_L1_SetProperty2(Si2151, Si2151_DTV_AGC_SPEED_PROP_CODE) != NO_Si2151_ERROR)
    {
       return ERROR_Si2151_SENDING_COMMAND;
    }
	//dtv_rf_top.dtv_rf_top
	if(Si2151_L1_SetProperty2(Si2151, Si2151_DTV_RF_TOP_PROP_CODE) != NO_Si2151_ERROR)
    {
       return ERROR_Si2151_SENDING_COMMAND;
    }

	//TUNER
	Si2151->prop->tuner_return_loss.config = param->tu_return_loss_cfg;
	Si2151->prop->tuner_return_loss.mode = param->tu_return_loss_mode;
	if(Si2151_L1_SetProperty2(Si2151, Si2151_TUNER_RETURN_LOSS_PROP_CODE) != NO_Si2151_ERROR)
    {
       return ERROR_Si2151_SENDING_COMMAND;
    }

	return NO_Si2151_ERROR;
}

// nDtv: 0 - atv, 1 - dtv
// freq: kHz
// video system M/N 에서 주파수 변환할 경우, 'SEM_ATV_FREQ_MN'를 정의하라.
int Si2151_SetTune(L1_Si2151_Context *Si2151,unsigned char nDtv,unsigned long freq,Si2151ConfigParam *param,unsigned int AtvTimeout)
{
	unsigned char mode = 0;


    AtvTimeout=AtvTimeout;  //kennydebug for compile warning


	if(nDtv)   //Digital
	{
		mode = Si2151_TUNER_TUNE_FREQ_CMD_MODE_DTV;
	}
	else //Analog
	{
        //Analog freq offset.
		switch(param->atv_video_mode_video_sys)
		{
		case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_B:
			param->atv_lif_freq_offset = 6400-2250;
			freq += 2250;//2250 khz
			break;
		case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_M:
		case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_N:
			param->atv_lif_freq_offset = 5400-1750;
			freq += 1750;
			break;
		case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_GH:
			param->atv_lif_freq_offset = 6750-2750;
			freq += 2750;
			break;
		case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_I:
			param->atv_lif_freq_offset = 7250-2750;
			freq += 2750;
			break;
		case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_L:
			param->atv_lif_freq_offset = 6250-2750;
			freq += 2750;
			break;
		case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_DK:
			param->atv_lif_freq_offset = 7250-2750;
			freq += 2750;
			break;
			//20130114,Issue: SECAM-DK/PAL-BG, 199.25 Mhz horizontal noise by ACI
		case Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_LP:
			param->atv_lif_freq_offset = 1250+2750;
			freq -= 2750;
			break;

		default:
			return -1;
		}
		mode = Si2151_TUNER_TUNE_FREQ_CMD_MODE_ATV;
	}

	if(Si2151_Tune (Si2151, mode, freq*1000) != NO_Si2151_ERROR)//freq*1000(Mhz)
		return ERROR_Si2151_SENDING_COMMAND;

	return NO_Si2151_ERROR;
}

// clk_mode: 0-외부 XTAL로부터 직접 입력받음, 1-다른 Si2151의 xout 으로부터 clk 받음
int Si2151_XoutputOn(L1_Si2151_Context *Si2151, int on,int clk_mode)
{
	int ret;

	if(clk_mode){
		Si2151->cmd->config_clocks.clock_mode = Si2151_CONFIG_CLOCKS_CMD_CLOCK_MODE_EXTCLK;
	}
	else{
		Si2151->cmd->config_clocks.clock_mode = Si2151_CONFIG_CLOCKS_CMD_CLOCK_MODE_XTAL;
	}

	if(on)
		ret = Si2151_XoutOn(Si2151);
	else
		ret = Si2151_XoutOff(Si2151);

	return ret;
}

int Si2151_SetPowerDown(L1_Si2151_Context *Si2151)
{
	if(Si2151_Powerdown(Si2151) != NO_Si2151_ERROR)
		return ERROR_Si2151_SENDING_COMMAND;
	
	return NO_Si2151_ERROR;
}

int Si2151_SetStandBy(L1_Si2151_Context *Si2151)
{
	if(Si2151_Standby(Si2151) != NO_Si2151_ERROR)
		return ERROR_Si2151_SENDING_COMMAND;
	
	return NO_Si2151_ERROR;
}

//int sel: 0-normal,1-max,2-top
int Si2151_AGCOverrive(L1_Si2151_Context *Si2151, int sel)
{
	if(Si2151_AGC_Override(Si2151,(unsigned char)sel) != NO_Si2151_ERROR)
		return ERROR_Si2151_SENDING_COMMAND;

	return NO_Si2151_ERROR;
}

#if 1
//for debugging, 사용하기 위해선 'SEM_DEBUG' 정의하라.
//mode: 0-Dtv, 1-Atv, 2-Tuner
char Si2151_GetMessage(L1_Si2151_Context *Si2151, char mode, char *Msg)
{
 if(mode == 2)
		Si2151_L1_GetCommandResponseString(Si2151, Si2151_TUNER_STATUS_CMD_CODE, "\n", Msg);
	return 0;
}
#endif 

//  Set MFT
unsigned char Si2151_SetMFT(L1_Si2151_Context *Si2151, int nStep)
{
	int ret=1;

	Si2151_L1_FINE_TUNE (Si2151, Si2151_FINE_TUNE_CMD_PERSISTENCE_NON_PERSISTENT, (125*nStep));
	return (unsigned char)ret;

}


// Get RSSI 
int Si2151_GetRSSI(L1_Si2151_Context *Si2151)
{
	if (Si2151_L1_TUNER_STATUS(Si2151) != NO_Si2151_ERROR) {
        return ERROR_Si2151_SENDING_COMMAND;
    }

   return ((int)(Si2151->rsp->tuner_status.rssi));
}

 /************************************************************************************************************************
  NAME: Si2151_GetVCO
  DESCRIPTIION: Retrieve Si2151 VCO value

  Parameter:  Pointer to Si2151 Context (I2C address)
  Returns  :  VCO vlaue 
************************************************************************************************************************/
int  Si2151_GetVCO             (L1_Si2151_Context *api)
{
    Si2151_L1_TUNER_STATUS (api);
	return api->rsp->tuner_status.vco_code;
}

//unsigned char   gpio1_mode:0-no change,1-disable,2-drive 0(Output Low),3-drive 1(Output High)
//                           7-DTV_FREEZE_HIGH, 8-DTV_FREEZE_LOW, 9-ATV_HSYNC_OUT
int Si2151_SetGpio1(L1_Si2151_Context *Si2151, unsigned char gpio1_mode)
{
	//kennydebug unsigned char gpio2_mode = Si2151_CONFIG_PINS_CMD_GPIO2_READ_DO_NOT_READ;
	if(Si2151_L1_CONFIG_PINS(Si2151,gpio1_mode, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0) != NO_Si2151_ERROR){
		return ERROR_Si2151_SENDING_COMMAND;
	}

	return NO_Si2151_ERROR;
}

//unsigned char   gpio1_mode:0-no change,1-disable,2-drive 0(Output Low),3-drive 1(Output High)
//                           7-DTV_FREEZE_HIGH, 8-DTV_FREEZE_LOW, 9-ATV_HSYNC_OUT
//unsigned char   gpio1_read:0-do not read,1-read the current pin state
int Si2151_GetGpio1(L1_Si2151_Context *Si2151, unsigned char gpio1_mode)
{
	 //kennydebug unsigned char gpio2_mode = Si2151_CONFIG_PINS_CMD_GPIO2_READ_DO_NOT_READ;
	if(Si2151_L1_CONFIG_PINS(Si2151,gpio1_mode, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0) != NO_Si2151_ERROR){
		return ERROR_Si2151_SENDING_COMMAND;
	}

	return NO_Si2151_ERROR;
}

//unsigned char   gpio2_mode:0-no change,1-disable,2-drive 0(Output Low),3-drive 1(Output High)
//                           7-DTV_FREEZE_HIGH, 8-DTV_FREEZE_LOW, 9-ATV_HSYNC_OUT
int Si2151_SetGpio2(L1_Si2151_Context *Si2151, unsigned char gpio2_mode)
{
	if(Si2151_L1_CONFIG_PINS(Si2151, 0x0, 0x0,gpio2_mode,0x0, 0x0, 0x0, 0x0, 0x0, 0x0) != NO_Si2151_ERROR){
		return ERROR_Si2151_SENDING_COMMAND;
	}

	return NO_Si2151_ERROR;
}

//unsigned char   gpio2_mode:0-no change,1-disable,2-drive 0(Output Low),3-drive 1(Output High)
//                           7-DTV_FREEZE_HIGH, 8-DTV_FREEZE_LOW, 9-ATV_HSYNC_OUT
//unsigned char   gpio2_read:0-do not read,1-read the current pin state
int Si2151_GetGpio2(L1_Si2151_Context *Si2151, unsigned char gpio2_mode)
{
	if(Si2151_L1_CONFIG_PINS(Si2151, 0x0, 0x0,gpio2_mode,0x1, 0x0, 0x0, 0x0, 0x0, 0x0) != NO_Si2151_ERROR){
		return ERROR_Si2151_SENDING_COMMAND;
	}

	return NO_Si2151_ERROR;
}


//unsigned char   AGC1_mode:0-no change,1-disable,2-drive 0(Output Low),3-drive 1(Output High), 4-ATV_AGC, 5-DTV_AGC, 6-ATV_DTV_AGC
//                          7-DTV_FREEZE_HIGH, 8-DTV_FREEZE_LOW, 9-ATV_HSYNC_OUT
int Si2151_SetAGC1(L1_Si2151_Context *Si2151, unsigned char AGC1_mode)
{
	 //kennydebug unsigned char gpio2_mode = Si2151_CONFIG_PINS_CMD_GPIO2_READ_DO_NOT_READ;
	if(Si2151_L1_CONFIG_PINS(Si2151, 0x0, 0x0, 0x0, 0x0, AGC1_mode, 0x0, 0x0, 0x0, 0x0) != NO_Si2151_ERROR){
		return ERROR_Si2151_SENDING_COMMAND;
	}

	return NO_Si2151_ERROR;
}

//unsigned char   AGC1_mode:0-no change,1-disable,2-drive 0(Output Low),3-drive 1(Output High), 4-ATV_AGC, 5-DTV_AGC, 6-ATV_DTV_AGC
//                          7-DTV_FREEZE_HIGH, 8-DTV_FREEZE_LOW, 9-ATV_HSYNC_OUT
//unsigned char   AGC1_read:0-do not read,1-read the current pin state
int Si2151_GetAGC1(L1_Si2151_Context *Si2151, unsigned char AGC1_mode)
{
	 //kennydebug unsigned char gpio2_mode = Si2151_CONFIG_PINS_CMD_GPIO2_READ_DO_NOT_READ;
	if(Si2151_L1_CONFIG_PINS(Si2151, 0x0, 0x0, 0x0, 0x0, AGC1_mode, 0x1, 0x0, 0x0, 0x0) != NO_Si2151_ERROR){
		return ERROR_Si2151_SENDING_COMMAND;
	}

	return NO_Si2151_ERROR;
}

//unsigned char   AGC2_mode:0-no change,1-disable,2-drive 0(Output Low),3-drive 1(Output High), 4-ATV_AGC, 5-DTV_AGC, 6-ATV_DTV_AGC
//                          7-DTV_FREEZE_HIGH, 8-DTV_FREEZE_LOW, 9-ATV_HSYNC_OUT
int Si2151_SetAGC2(L1_Si2151_Context *Si2151, unsigned char AGC2_mode)
{
	 //kennydebug unsigned char gpio2_mode = Si2151_CONFIG_PINS_CMD_GPIO2_READ_DO_NOT_READ;
	if(Si2151_L1_CONFIG_PINS(Si2151, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, AGC2_mode, 0x0, 0x0) != NO_Si2151_ERROR){
		return ERROR_Si2151_SENDING_COMMAND;
	}

	return NO_Si2151_ERROR;
}

//unsigned char   AGC2_mode:0-no change,1-disable,2-drive 0(Output Low),3-drive 1(Output High), 4-ATV_AGC, 5-DTV_AGC, 6-ATV_DTV_AGC
//                          7-DTV_FREEZE_HIGH, 8-DTV_FREEZE_LOW, 9-ATV_HSYNC_OUT
//unsigned char   AGC2_read:0-do not read,1-read the current pin state
int Si2151_GetAGC2(L1_Si2151_Context *Si2151, unsigned char AGC2_mode)
{
	 //kennydebug unsigned char gpio2_mode = Si2151_CONFIG_PINS_CMD_GPIO2_READ_DO_NOT_READ;
	if(Si2151_L1_CONFIG_PINS(Si2151, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, AGC2_mode, 0x1, 0x0) != NO_Si2151_ERROR){
		return ERROR_Si2151_SENDING_COMMAND;
	}

	return NO_Si2151_ERROR;
}

//unsigned char   AGC2_mode:0-no change,1-disable,2-drive 0(Output Low),3-drive 1(Output High), 9-ATV_HSYNC_OUT
int Si2151_SetXoutMode(L1_Si2151_Context *Si2151, unsigned char Xout_mode)
{
	 //kennydebug unsigned char gpio2_mode = Si2151_CONFIG_PINS_CMD_GPIO2_READ_DO_NOT_READ;
	if(Si2151_L1_CONFIG_PINS(Si2151, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, Xout_mode) != NO_Si2151_ERROR){
		return ERROR_Si2151_SENDING_COMMAND;
	}

	return NO_Si2151_ERROR;
}

//dtv_lif_out_amp: 27(DIF 출력값 160mV), 54(300mV), 90(500mV), 110(600mV)
int Si2151_SetDtvLifOutAmp(L1_Si2151_Context *Si2151,unsigned char dtv_lif_out_amp)
{
	Si2151->prop->dtv_lif_out.amp = dtv_lif_out_amp;
	if(Si2151_L1_SetProperty2(Si2151, Si2151_DTV_LIF_OUT_PROP_CODE) != NO_Si2151_ERROR)
    {
       return ERROR_Si2151_SENDING_COMMAND;
    }

	return NO_Si2151_ERROR;
}

int Si2151_SetDtvIfAgcSpeed(L1_Si2151_Context *Si2151,unsigned char dtv_if_decim,unsigned char dtv_if_agc_speed)
{
	Si2151->prop->dtv_agc_speed.if_agc_speed = dtv_if_agc_speed;
	Si2151->prop->dtv_agc_speed.agc_decim= dtv_if_decim;

	if(Si2151_L1_SetProperty2(Si2151, Si2151_DTV_AGC_SPEED_PROP_CODE) != NO_Si2151_ERROR)
    {
       return ERROR_Si2151_SENDING_COMMAND;
    }
	return NO_Si2151_ERROR;
}

#if 0 // covered by S.Yu. because this is for ATV. 2013.03.26 There is no variables to set audio properties.
int Si2151_SetAudioSys(L1_Si2151_Context *api,unsigned char audio_sys)
{
	/* setup ATV audio property for SCAN Mode */
	if(audio_sys == Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_SCAN)
	{
		api->prop->atv_audio_mode.audio_sys = Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_SCAN;
	}
	else
	{
		if(audio_sys == Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_A2_DK4)
		{
			api->prop->atv_audio_mode.audio_sys = Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_A2_DK4;
		}
		else if(audio_sys == Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_A2)
		{
			api->prop->atv_audio_mode.audio_sys = Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_A2;
		}
		else
		{
			api->prop->atv_audio_mode.audio_sys = Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_MONO_NICAM;
		}
	}
	
	if(Si2151_L1_SetProperty2(api, Si2151_ATV_AUDIO_MODE_PROP_CODE) != NO_Si2151_ERROR)
    {
       return ERROR_Si2151_SENDING_COMMAND;
    }

	return NO_Si2151_ERROR;
}


//NTSC-M, PAL-BG/DK/I: Fox(30), Mstar(40), Novatek(45)
//SECAM: Fox(26), Mstar(35), Novatek(40)
int Si2151_SetSifOut(L1_Si2151_Context *api,unsigned char sif_out_offset, unsigned char sif_out_amp)
{
	api->prop->atv_sif_out.offset = sif_out_offset;
	api->prop->atv_sif_out.amp = sif_out_amp;
	if(Si2151_L1_SetProperty2(api, Si2151_ATV_SIF_OUT_PROP_CODE) != NO_Si2151_ERROR)
    {
      return ERROR_Si2151_SENDING_COMMAND;
    }
	return NO_Si2151_ERROR;
}
#endif

// freq: kHz
// nDtv: 0 - atv, 1 - dtv
// to improve the issue with red color bluring on NTSC cable channel 6(83.25MHZ)
// 미주향 모델만 적용: 아날로그 6번채널만, 수신시 high injection -> low injection 으로 변경
// Si2151_SetConfig() 이후, Si2151_SetTune() 이전에 사용
int Si2151_SetLowIngectionBand(L1_Si2151_Context *api,unsigned char nDtv,unsigned long freq)
{
	if(nDtv == 0)
	{

		//1. analog
		if(freq >= 81250 && freq <= 85250)
		{//1-1. analog, ch6
			if(bCheckInjection_83_25==0)
			{
				api->prop->tuner_lo_injection.band_1 = Si2151_TUNER_LO_INJECTION_PROP_BAND_1_LOW_SIDE ;//low 로 변경
				api->prop->tuner_lo_injection.band_2 = Si2151_TUNER_LO_INJECTION_PROP_BAND_2_LOW_SIDE  ;
				api->prop->tuner_lo_injection.band_3 = Si2151_TUNER_LO_INJECTION_PROP_BAND_3_LOW_SIDE  ;

				if (Si2151_L1_SetProperty2(api, Si2151_TUNER_LO_INJECTION_PROP_CODE) != NO_Si2151_ERROR)
				{
					return ERROR_Si2151_SENDING_COMMAND;
				}
				else{}

				bCheckInjection_83_25 = 1;
			}
			else{}
		}
		else
		{//1-2. analog, ch6 x
			if(bCheckInjection_83_25)
			{
				api->prop->tuner_lo_injection.band_1 = Si2151_TUNER_LO_INJECTION_PROP_BAND_1_HIGH_SIDE ; /* (default 'HIGH_SIDE') */
				api->prop->tuner_lo_injection.band_2 = Si2151_TUNER_LO_INJECTION_PROP_BAND_2_LOW_SIDE  ; /* (default 'LOW_SIDE') */
				api->prop->tuner_lo_injection.band_3 = Si2151_TUNER_LO_INJECTION_PROP_BAND_3_LOW_SIDE  ; /* (default 'LOW_SIDE') */

				if (Si2151_L1_SetProperty2(api, Si2151_TUNER_LO_INJECTION_PROP_CODE) != NO_Si2151_ERROR)
				{
					return ERROR_Si2151_SENDING_COMMAND;
				}
				else{}

				bCheckInjection_83_25 = 0;
			}
			else{}
		}

	}
	else
	{//2. digital
		if(bCheckInjection_83_25)
		{//analog, 6ch -> digital
			api->prop->tuner_lo_injection.band_1 = Si2151_TUNER_LO_INJECTION_PROP_BAND_1_HIGH_SIDE ; /* (default 'HIGH_SIDE') */
			api->prop->tuner_lo_injection.band_2 = Si2151_TUNER_LO_INJECTION_PROP_BAND_2_LOW_SIDE  ; /* (default 'LOW_SIDE') */
			api->prop->tuner_lo_injection.band_3 = Si2151_TUNER_LO_INJECTION_PROP_BAND_3_LOW_SIDE  ; /* (default 'LOW_SIDE') */

			if (Si2151_L1_SetProperty2(api, Si2151_TUNER_LO_INJECTION_PROP_CODE) != NO_Si2151_ERROR)
			{
				return ERROR_Si2151_SENDING_COMMAND;
			}
			else{}

			bCheckInjection_83_25 = 0;
		}
		else{}

	}

	return NO_Si2151_ERROR;
}


// SET factory mode 사용
// ATV TUNE 이후에 사용
int Si2151_SetAtvIfAgcSpeed(L1_Si2151_Context *api,unsigned char agc_speed)
{
	switch(agc_speed)
	{
		case  0: api->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO; break;
		case  1: api->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_89; break;
		case  2: api->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_105; break;
		case  3: api->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_121; break;
		case  4: api->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_137; break;
		case  5: api->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_158; break;
		case  6: api->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_172; break;
		case  7: api->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_178; break;
		case  8: api->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_185; break;
		case  9: api->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_196; break;
		case 10: api->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_206; break;
		case 11: api->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_216; break;
		case 12: api->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_219; break;
		case 13: api->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_222; break;
		case 14: api->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_248; break;
		case 15: api->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_250; break;
		case 16: api->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_251; break;
		default: api->prop->atv_agc_speed.if_agc_speed = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO; break;
	}
	if(Si2151_L1_SetProperty2(api, Si2151_ATV_AGC_SPEED_PROP_CODE) != NO_Si2151_ERROR)
	{
		return ERROR_Si2151_SENDING_COMMAND;
	}
//	if(Si2151_L1_ATV_RESTART(api))
//	{
//		return ERROR_Si2151_SENDING_COMMAND;
//	}
	return NO_Si2151_ERROR;
}

unsigned char Si2151_AtvIfAgcSpeedMapping(unsigned char atvifagcspeed)
{
	unsigned char ret = 0;//default, auto
	switch(atvifagcspeed)
	{
		case  0: ret = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO; break;
		case  1: ret = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_89; break;
		case  2: ret = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_105; break;
		case  3: ret = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_121; break;
		case  4: ret = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_137; break;
		case  5: ret = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_158; break;
		case  6: ret = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_172; break;
		case  7: ret = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_178; break;
		case  8: ret = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_185; break;
		case  9: ret = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_196; break;
		case 10: ret = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_206; break;
		case 11: ret = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_216; break;
		case 12: ret = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_219; break;
		case 13: ret = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_222; break;
		case 14: ret = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_248; break;
		case 15: ret = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_250; break;
		case 16: ret = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_251; break;
		default: ret = Si2151_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO; break;
	}
	return ret;
}


// SET factory mode 사용
// ATV TUNE 이후에 실행한다.(파라미터 'nDtv = 0'인 Si2151_SetTune() 실행 이후에 사용)
int Si2151_SetAtvRfTop(L1_Si2151_Context *api, unsigned char rf_top)
{
	switch(rf_top)
	{
	case 0: api->prop->atv_rf_top.atv_rf_top = Si2151_ATV_RF_TOP_PROP_ATV_RF_TOP_AUTO; break;
	case 1: api->prop->atv_rf_top.atv_rf_top = Si2151_ATV_RF_TOP_PROP_ATV_RF_TOP_P2DB; break;
	case 2: api->prop->atv_rf_top.atv_rf_top = Si2151_ATV_RF_TOP_PROP_ATV_RF_TOP_P1DB; break;
	case 3: api->prop->atv_rf_top.atv_rf_top = Si2151_ATV_RF_TOP_PROP_ATV_RF_TOP_0DB; break;
	case 4: api->prop->atv_rf_top.atv_rf_top = Si2151_ATV_RF_TOP_PROP_ATV_RF_TOP_M1DB; break;
	case 5: api->prop->atv_rf_top.atv_rf_top = Si2151_ATV_RF_TOP_PROP_ATV_RF_TOP_M2DB; break;
	case 6: api->prop->atv_rf_top.atv_rf_top = Si2151_ATV_RF_TOP_PROP_ATV_RF_TOP_M3DB; break;
	case 7: api->prop->atv_rf_top.atv_rf_top = Si2151_ATV_RF_TOP_PROP_ATV_RF_TOP_M4DB; break;
	case 8: api->prop->atv_rf_top.atv_rf_top = Si2151_ATV_RF_TOP_PROP_ATV_RF_TOP_M5DB; break;
	case 9: api->prop->atv_rf_top.atv_rf_top = Si2151_ATV_RF_TOP_PROP_ATV_RF_TOP_M6DB; break;
	case 10: api->prop->atv_rf_top.atv_rf_top = Si2151_ATV_RF_TOP_PROP_ATV_RF_TOP_M7DB; break;
	case 11: api->prop->atv_rf_top.atv_rf_top = Si2151_ATV_RF_TOP_PROP_ATV_RF_TOP_M8DB; break;
	case 12: api->prop->atv_rf_top.atv_rf_top = Si2151_ATV_RF_TOP_PROP_ATV_RF_TOP_M9DB; break;
	case 13: api->prop->atv_rf_top.atv_rf_top = Si2151_ATV_RF_TOP_PROP_ATV_RF_TOP_M10DB; break;
	case 14: api->prop->atv_rf_top.atv_rf_top = Si2151_ATV_RF_TOP_PROP_ATV_RF_TOP_M11DB; break;
	default: api->prop->atv_rf_top.atv_rf_top = Si2151_ATV_RF_TOP_PROP_ATV_RF_TOP_AUTO; break;
	}
	if(Si2151_L1_SetProperty2(api, Si2151_ATV_RF_TOP_PROP_CODE) != NO_Si2151_ERROR)
	{
		return ERROR_Si2151_SENDING_COMMAND;
	}
	return NO_Si2151_ERROR;
}

// SET factory mode 사용
// DTV TUNE 이후에 실행한다.(파라미터 'nDtv = 1'인 Si2151_SetTune() 실행 이후에 사용)
int Si2151_SetDtvRfTop(L1_Si2151_Context *api, unsigned char rf_top)
{
	switch(rf_top)
	{
	case 0: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_AUTO; break;
	case 1: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P6DB; break;
	case 2: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P5P5DB; break;
	case 3: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P5DB; break;
	case 4: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P4P5DB; break;
	case 5: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P4DB; break;
	case 6: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P3P5DB; break;
	case 7: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P3DB; break;
	case 8: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P2P5DB; break;
	case 9: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P2DB; break;
	case 10: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P1P5DB; break;
	case 11: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P1DB; break;
	case 12: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P0P5DB; break;
	case 13: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_0DB; break;
	case 14: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M0P5DB; break;
	case 15: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M1DB; break;
	case 16: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M1P5DB; break;
	case 17: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M2DB; break;
	case 18: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M2P5DB; break;
	case 19: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M3DB; break;
	case 20: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M3P5DB; break;
	case 21: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M4DB; break;
	case 22: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M4P5DB; break;
	case 23: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M5DB; break;
	case 24: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M5P5DB; break;
	case 25: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M6DB; break;
	case 26: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M6P5DB; break;
	case 27: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M7DB; break;
	case 28: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M7P5DB; break;
	case 29: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M8DB; break;
	case 30: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M8P5DB; break;
	case 31: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M9DB; break;
	case 32: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M9P5DB; break;
	case 33: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M10DB; break;
	case 34: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M10P5DB; break;
	case 35: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M11DB; break;
	default: api->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_AUTO; break;
	}
	if(Si2151_L1_SetProperty2(api, Si2151_DTV_RF_TOP_PROP_CODE) != NO_Si2151_ERROR)
	{
		return ERROR_Si2151_SENDING_COMMAND;
	}
	return NO_Si2151_ERROR;
}



#if 0
// ATV TUNE 이후에 사용
// mode: 0 - restart ATV audio & video system, 1 - restart ATV audio system only
int Si2151_AtvRestart(L1_Si2151_Context *api)
{
	if(Si2151_L1_ATV_RESTART(api))
	{
		return ERROR_Si2151_SENDING_COMMAND;
	}
	return NO_Si2151_ERROR;
}


// DTV TUNE 이후에 사용
int Si2151_DtvRestart(L1_Si2151_Context *api)
{
	if(Si2151_L1_DTV_RESTART(api))
	{
		return ERROR_Si2151_SENDING_COMMAND;
	}
	return NO_Si2151_ERROR;
}
#endif
