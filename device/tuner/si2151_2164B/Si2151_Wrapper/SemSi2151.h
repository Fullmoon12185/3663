/***************************************************************************************
   Date: 2012.11.21
****************************************************************************************/

#ifndef _Si2151_H
#define _Si2151_H

#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

#define   Si2151_COMMAND_PROTOTYPES

#include "../Si2151/Si2151_Commands.h"
#include "../Si2151/Si2151_L1_API.h"
#include "../Si2151/Si2151_L2_API.h"
#include "../Si2151/Si2151_Commands_Prototypes.h"
#include "../Si2151/Si2151_Properties_Strings.h"


// property for Configuration and Tune
	typedef struct _Si2151Config {
		unsigned char	atv_video_mode_video_sys;		//api->prop->atv_video_mode.video_sys
		unsigned char	atv_video_mode_color;			//api->prop->atv_video_mode.color
		unsigned char	atv_video_mode_invert_signal;	//api->prop->atv_video_mode.invert_signal
		unsigned int	atv_lif_freq_offset;			//api->prop->atv_lif_freq.offset
		unsigned char	atv_out_type;					//api->prop->dtv_config_if_port.dtv_out_type
		unsigned char	dtv_out_type;					//api->prop->dtv_config_if_port.dtv_out_type
		unsigned char	dtv_agc_source;					//api->prop->dtv_config_if_port.dtv_agc_source
		unsigned char	dtv_agc_overrive;				//api->prop->dtv_config_if_port.dtv_agc_overrive
		unsigned char	dtv_mode_bw;					//api->prop->dtv_mode.bw
		unsigned char	dtv_mode_invert_spectrum;		//api->prop->dtv_mode.invert_spectrum
		unsigned char	dtv_mode_modulation;			//api->prop->dtv_mode.modulation
		unsigned char   dtv_if_agc_speed;				//api->prop->dtv_agc_speed.if_agc_speed
		unsigned char   dtv_agc_decim;					//api->prop->dtv_agc_speed.agc_decim 
		unsigned int	dtv_lif_freq_offset;			//api->prop->dtv_lif_freq.offset

		unsigned char	tu_return_loss_mode;			//api->prop->tuner_return_loss.mode
		unsigned char	tu_return_loss_cfg;				//api->prop->tuner_return_loss.config

	}Si2151ConfigParam, *lpSi2151ConfigParam;

// i2c address
#define   Si2151_BASE_ADDRESS 0xc0

// initailize
int Si2151_Initialize(L1_Si2151_Context *Si2151,int clk_mode);
int Si2151_InitSTR(L1_Si2151_Context *Si2151, char idx);

// status check
int Si2151_DisplayStatusByte (L1_Si2151_Context *Si2151);
int Si2151_GetRevision (L1_Si2151_Context *Si2151,Si2151_GET_REV_CMD_REPLY_struct *pRev);
int Si2151_GetTunerStatus (L1_Si2151_Context *Si2151,Si2151_TUNER_STATUS_CMD_REPLY_struct *pRev);
int Si2151_GetRSSI(L1_Si2151_Context *Si2151);

// property configuration
int Si2151_ConfigATV(L1_Si2151_Context *Si2151);
int Si2151_ConfigDTV(L1_Si2151_Context *Si2151);
int Si2151_ConfigTuner(L1_Si2151_Context *Si2151);
int Si2151_ConfigCommon(L1_Si2151_Context *Si2151);
void Si2151_ConfigTable(L1_Si2151_Context *Si2151,unsigned char video_sys, unsigned char modulation,unsigned char atvifagcspeed);
int Si2151_SetConfig(L1_Si2151_Context *Si2151, Si2151ConfigParam *param,unsigned char atvifagcspeed);
unsigned char Si2151_AtvIfAgcSpeedMapping(unsigned char atvifagcspeed);
// tuning
int Si2151_SetTune(L1_Si2151_Context *Si2151,unsigned char nDtv,unsigned long freq,Si2151ConfigParam *param,unsigned int AtvTimeout);

// etc.
unsigned char Si2151_SetMFT(L1_Si2151_Context *Si2151, int nStep);
int Si2151_XoutputOn(L1_Si2151_Context *Si2151, int on,int clk_mode);
int Si2151_SetPowerDown(L1_Si2151_Context *Si2151);
int Si2151_SetStandBy(L1_Si2151_Context *Si2151);
int Si2151_AGCOverrive(L1_Si2151_Context *Si2151, int sel);
int Si2151_SetGpio1(L1_Si2151_Context *Si2151, unsigned char gpio1_mode);
int Si2151_GetGpio1(L1_Si2151_Context *Si2151, unsigned char gpio1_mode);
int Si2151_SetGpio2(L1_Si2151_Context *Si2151, unsigned char gpio2_mode);
int Si2151_GetGpio2(L1_Si2151_Context *Si2151, unsigned char gpio2_mode);
int Si2151_SetAGC1(L1_Si2151_Context *Si2151, unsigned char AGC1_mode);
int Si2151_GetAGC1(L1_Si2151_Context *Si2151, unsigned char AGC1_mode);
int Si2151_SetAGC2(L1_Si2151_Context *Si2151, unsigned char AGC2_mode);
int Si2151_GetAGC2(L1_Si2151_Context *Si2151, unsigned char AGC2_mode);
int Si2151_SetXoutMode(L1_Si2151_Context *Si2151, unsigned char Xout_mode);
int Si2151_SetDtvLifOutAmp(L1_Si2151_Context *Si2151,unsigned char dtv_lif_out_amp);
int Si2151_SetDtvIfAgcSpeed(L1_Si2151_Context *Si2151,unsigned char dtv_if_decim,unsigned char dtv_if_agc_speed);
int Si2151_SetAudioSys(L1_Si2151_Context *api,unsigned char audio_sys);
int Si2151_SetSifOut(L1_Si2151_Context *api,unsigned char sif_out_offset, unsigned char sif_out_amp);
int Si2151_SetLowIngectionBand(L1_Si2151_Context *api,unsigned char nDtv,unsigned long freq);
int Si2151_SetAtvIfAgcSpeed(L1_Si2151_Context *api,unsigned char agc_speed);
int Si2151_SetAtvRfTop(L1_Si2151_Context *api, unsigned char rf_top);
int Si2151_SetDtvRfTop(L1_Si2151_Context *api, unsigned char rf_top);
int Si2151_AtvRestart(L1_Si2151_Context *api);
int Si2151_DtvRestart(L1_Si2151_Context *api);
int Si2151_Tuner_Block_VCO_Control( L1_Si2151_Context *write_api, int vco_code);
int Si2151_GetVCO				 (L1_Si2151_Context *api);

// debug
char Si2151_GetMessage(L1_Si2151_Context *Si2151, char mode, char *Msg);


#if defined( __cplusplus )
}
#endif

#endif






