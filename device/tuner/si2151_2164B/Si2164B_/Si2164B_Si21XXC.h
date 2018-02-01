#ifndef  TUNER_Si2151_SPECIFICS
#define  TUNER_Si2151_SPECIFICS
#define   Si2151_COMMAND_PROTOTYPES//csm,13'
    #include "../Si2151/Si2151_L2_API.h"

#ifdef    SILABS_EVB
    #define Si2164B_CLOCK_MODE_TER              Si2164B_START_CLK_CMD_CLK_MODE_CLK_CLKIO
    #define Si2164B_REF_FREQUENCY_TER           24
#else  /* SILABS_EVB */
    #define Si2164B_CLOCK_MODE_TER              Si2164B_START_CLK_CMD_CLK_MODE_CLK_CLKIO
    #define Si2164B_REF_FREQUENCY_TER           24
#endif /* SILABS_EVB */
    #define TUNERTER_API
    #define TER_TUNER_CONTEXT            L1_Si2151_Context
    #define TER1_MAIN_ADDRESS			0xc2// DUAL T2CS2
	#define TER1_SUB_ADDRESS            0xc4// DUAL T2CS2
	#define TER2_MAIN_ADDRESS			0xc0// DUAL TCS2, SINGLE TCS2
	#define TER2_SUB_ADDRESS			0xc6// DUAL TCS2

    #define TER_TUNER_INIT(api)           Si2151_Init(api)

/*  SiLabs: do NOT define this macro when using the TER tuner AGC in 'INTERNAL' mode */
/*
    #define TER_TUNER_AGC_EXTERNAL(api)   api->prop->dtv_config_if_port.dtv_out_type    = Si2151_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_LIF_IF1;\
                                          api->prop->dtv_config_if_port.dtv_agc_source  = Si2151_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE_AGC1_3DB;\
                                          Si2151_L1_SetProperty2(api, Si2151_DTV_CONFIG_IF_PORT_PROP_CODE);
*/

/*  SiLabs: adapt dtv_lif_out amplitude differently for QAM or OFDM standards
  For all families of tuner and newer demods , AMP values are:
  -	DVBT/DVBT2/DVBC2 (OFDM standards): (amp_index = 0) DTV_LIF_OUT:AMP=32
  -	DVBC/MCNS        (QAM  standards): (amp_index = 1) DTV_LIF_OUT:AMP=43
*/
  #define TER_TUNER_DTV_LIF_OUT_AMP(api,amp_index) \
    api->prop->dtv_lif_out.amp    = 32 + (11*amp_index);\
    api->prop->dtv_lif_out.offset = 148;\
    Si2151_L1_SetProperty2(api, Si2151_DTV_LIF_OUT_PROP_CODE);


    #define SILABS_TER_TUNER_Context TER_TUNER_CONTEXT

    #define TER_TUNER_WAKEUP(api)         Si2151_pollForCTS(api)
    /* Compatibility with Si2178 ROM12 & ROM13 APIs */
   #ifdef    Si2151_STANDBY_CMD_TYPE_MIN
     #define TER_TUNER_STANDBY(api)        Si2151_L1_STANDBY(api, Si2151_STANDBY_CMD_TYPE_MIN)
   #else  /* Si2151_STANDBY_CMD_TYPE_MIN */
    #define TER_TUNER_STANDBY(api)        Si2151_L1_STANDBY(api)
   #endif /* Si2151_STANDBY_CMD_TYPE_MIN */
    #define TER_TUNER_CLOCK_OFF(api)      Si2151_XoutOff(api)
    #define TER_TUNER_CLOCK_ON(api)       Si2151_XoutOn(api)
    #define TER_TUNER_ERROR_TEXT(res)     Si2151_L1_API_ERROR_TEXT(res)
#ifdef    Si2164B_COMMAND_LINE_APPLICATION
    #define TER_TUNER_MENU(full)          Si2151_menu(full)
    #define TER_TUNER_LOOP(api,choice)    Si2151_demoLoop(api,choice)
#endif /* Si2164B_COMMAND_LINE_APPLICATION */

    #define TER_TUNER_MODULATION_DVBT     Si2151_DTV_MODE_PROP_MODULATION_DVBT
    #define TER_TUNER_MODULATION_DVBC     Si2151_DTV_MODE_PROP_MODULATION_DVBC
//#define SiLabs_TER_Tuner_ATVTune(tuner_ter,freq,video_sys,trans,color,invert_signal) Si2151_ATVTune(tuner_ter,freq,video_sys,color,invert_signal)
#define SiLabs_TER_Tuner_ATVTune(tuner_ter,freq,video_sys,trans,color,invert_signal,tunint_check,AtvTimeout) Si2151_ATVTune(tuner_ter,freq,video_sys,color,invert_signal,tunint_check,AtvTimeout)//csm,13
#define SiLabs_TER_Tuner_DTVTune Si2151_DTVTune
    #define L1_RF_TER_TUNER_Init(api,add) Si2151_L1_API_Init(api, add);
    #define L1_RF_TER_TUNER_Tune(api,rf)  Si2151_DTVTune(api, rf, bw, modulation, Si2151_DTV_MODE_PROP_INVERT_SPECTRUM_NORMAL); system_wait(85);

    #define L1_RF_TER_TUNER_Get_RF

    #define L1_RF_TER_TUNER_MODULATION_DVBC  Si2151_DTV_MODE_PROP_MODULATION_DVBC
    #define L1_RF_TER_TUNER_MODULATION_DVBT  Si2151_DTV_MODE_PROP_MODULATION_DVBT
    #define L1_RF_TER_TUNER_MODULATION_DVBT2 Si2151_DTV_MODE_PROP_MODULATION_DVBT

    #define   L1_RF_TER_TUNER_FEF_MODE_FREEZE_PIN_SETUP(api) \
	if((api->rsp->part_info.part == 78)||(api->rsp->part_info.part == 48)){\
    api->cmd->config_pins.gpio1_mode               = Si2178B_CONFIG_PINS_CMD_GPIO1_MODE_DISABLE;\
    api->cmd->config_pins.gpio1_read               = Si2178B_CONFIG_PINS_CMD_GPIO1_READ_DO_NOT_READ;\
    api->cmd->config_pins.gpio2_mode               = Si2178B_CONFIG_PINS_CMD_GPIO2_MODE_DISABLE;\
    api->cmd->config_pins.gpio2_read               = Si2178B_CONFIG_PINS_CMD_GPIO2_READ_DO_NOT_READ;\
    api->cmd->config_pins.reserved1                = Si2178B_CONFIG_PINS_CMD_RESERVED1_RESERVED;\
    api->cmd->config_pins.reserved2                = Si2178B_CONFIG_PINS_CMD_RESERVED2_RESERVED;\
    api->cmd->config_pins.reserved3                = Si2178B_CONFIG_PINS_CMD_RESERVED3_RESERVED;\
    api->prop->dtv_initial_agc_speed.agc_decim     = Si2151_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_OFF;\
    api->prop->dtv_initial_agc_speed.if_agc_speed  = Si2151_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO;\
    api->prop->dtv_initial_agc_speed_period.period = 0;\
    api->prop->dtv_agc_speed.agc_decim             = Si2151_DTV_AGC_SPEED_PROP_AGC_DECIM_OFF;\
    api->prop->dtv_agc_speed.if_agc_speed          = Si2151_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO;\
    Si2151_L1_SendCommand2(api, Si2178B_CONFIG_PINS_CMD_CODE);\
    Si2151_L1_SetProperty2(api, Si2151_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_CODE);\
    Si2151_L1_SetProperty2(api, Si2151_DTV_AGC_SPEED_PROP_CODE);\
    } else {\
    api->cmd->config_pins.gpio1_mode               = Si2190B_CONFIG_PINS_CMD_GPIO1_MODE_DISABLE;\
    api->cmd->config_pins.gpio1_read               = Si2190B_CONFIG_PINS_CMD_GPIO1_READ_DO_NOT_READ;\
    api->cmd->config_pins.gpio2_mode               = Si2190B_CONFIG_PINS_CMD_GPIO2_MODE_NO_CHANGE;\
    api->cmd->config_pins.gpio2_read               = Si2190B_CONFIG_PINS_CMD_GPIO2_READ_DO_NOT_READ;\
    api->cmd->config_pins.agc1_mode                = Si2190B_CONFIG_PINS_CMD_AGC1_MODE_NO_CHANGE;\
    api->cmd->config_pins.agc1_read                = Si2190B_CONFIG_PINS_CMD_AGC1_READ_READ;\
    api->cmd->config_pins.agc2_mode                = Si2190B_CONFIG_PINS_CMD_AGC2_MODE_NO_CHANGE;\
    api->cmd->config_pins.agc2_read                = Si2190B_CONFIG_PINS_CMD_AGC2_READ_READ;\
    api->cmd->config_pins.reserved3                = Si2190B_CONFIG_PINS_CMD_RESERVED3_RESERVED;\
    api->prop->dtv_initial_agc_speed.agc_decim     = Si2151_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_OFF;\
    api->prop->dtv_initial_agc_speed.if_agc_speed  = Si2151_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO;\
    api->prop->dtv_initial_agc_speed_period.period = 0;\
    api->prop->dtv_agc_speed.agc_decim             = Si2151_DTV_AGC_SPEED_PROP_AGC_DECIM_OFF;\
    api->prop->dtv_agc_speed.if_agc_speed          = Si2151_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO;\
    Si2151_L1_SendCommand2(api, Si2190B_CONFIG_PINS_CMD_CODE);\
    Si2151_L1_SetProperty2(api, Si2151_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_CODE);\
    Si2151_L1_SetProperty2(api, Si2151_DTV_AGC_SPEED_PROP_CODE);\
    }\

	#define   L1_RF_TER_TUNER_DTV_AGC_AUTO_FREEZE(api,fef) \
    if (fef == 0) {\
      api->prop->dtv_agc_auto_freeze.thld          = 255;\
      api->prop->dtv_agc_auto_freeze.timeout       = 1;\
    } else {\
      api->prop->dtv_agc_auto_freeze.thld          = 9;\
      api->prop->dtv_agc_auto_freeze.timeout       = 63;\
    }\
    Si2151_L1_SetProperty2(api, Si2151_DTV_AGC_AUTO_FREEZE_PROP_CODE);

    #define   L1_RF_TER_TUNER_FEF_MODE_FREEZE_PIN(api,fef) \
    api->prop->dtv_agc_freeze_input.level          = Si2151_DTV_AGC_FREEZE_INPUT_PROP_LEVEL_HIGH;\
    if (fef == 0) {\
      api->prop->dtv_agc_freeze_input.pin          = Si2151_DTV_AGC_FREEZE_INPUT_PROP_PIN_NONE;\
    } else {\
      api->prop->dtv_agc_freeze_input.pin          = Si2151_DTV_AGC_FREEZE_INPUT_PROP_PIN_GPIO1;\
    }\
    Si2151_L1_SetProperty2(api, Si2151_DTV_AGC_FREEZE_INPUT_PROP_CODE);

    #define   L1_RF_TER_TUNER_FEF_MODE_SLOW_INITIAL_AGC_SETUP(api,fef) \
      if (fef == 0) {\
        api->prop->dtv_initial_agc_speed.agc_decim     = Si2151_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_OFF;\
        api->prop->dtv_initial_agc_speed.if_agc_speed  = Si2151_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO;\
        api->prop->dtv_initial_agc_speed_period.period = 0;\
        api->prop->dtv_agc_speed.agc_decim             = Si2151_DTV_AGC_SPEED_PROP_AGC_DECIM_OFF;\
        api->prop->dtv_agc_speed.if_agc_speed          = Si2151_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO;\
      } else {\
        api->prop->dtv_initial_agc_speed.agc_decim     = Si2151_DTV_INITIAL_AGC_SPEED_PROP_AGC_DECIM_OFF;\
        api->prop->dtv_initial_agc_speed.if_agc_speed  = Si2151_DTV_INITIAL_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO;\
        api->prop->dtv_initial_agc_speed_period.period = 240;\
        api->prop->dtv_agc_speed.agc_decim             = Si2151_DTV_AGC_SPEED_PROP_AGC_DECIM_4;\
        api->prop->dtv_agc_speed.if_agc_speed          = Si2151_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_39;\
      }\
      Si2151_L1_SetProperty2(api, Si2151_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_CODE);\
      Si2151_L1_SetProperty2(api, Si2151_DTV_AGC_SPEED_PROP_CODE);


    #define   L1_RF_TER_TUNER_FEF_MODE_SLOW_NORMAL_AGC_SETUP(api,fef) \
      api->prop->dtv_initial_agc_speed_period.period = 0;\
      if (fef == 0) {\
        api->prop->dtv_agc_speed.agc_decim             = Si2151_DTV_AGC_SPEED_PROP_AGC_DECIM_OFF;\
        api->prop->dtv_agc_speed.if_agc_speed          = Si2151_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO;\
      } else {\
        api->prop->dtv_agc_speed.if_agc_speed          = Si2151_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_39;\
        api->prop->dtv_agc_speed.agc_decim             = Si2151_DTV_AGC_SPEED_PROP_AGC_DECIM_4;\
      }\
      Si2151_L1_SetProperty2(api, Si2151_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_CODE);\
      Si2151_L1_SetProperty2(api, Si2151_DTV_AGC_SPEED_PROP_CODE);

    #define   L1_RF_TER_TUNER_FEF_MODE_SLOW_NORMAL_AGC(api,fef) \
      if (fef == 0) {\
        api->prop->dtv_agc_speed.agc_decim             = Si2151_DTV_AGC_SPEED_PROP_AGC_DECIM_OFF;\
        api->prop->dtv_agc_speed.if_agc_speed          = Si2151_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_AUTO;\
      } else {\
        api->prop->dtv_agc_speed.if_agc_speed          = Si2151_DTV_AGC_SPEED_PROP_IF_AGC_SPEED_39;\
        api->prop->dtv_agc_speed.agc_decim             = Si2151_DTV_AGC_SPEED_PROP_AGC_DECIM_4;\
      }\
      Si2151_L1_SetProperty2(api, Si2151_DTV_AGC_SPEED_PROP_CODE);

#endif /* TUNER_Si2151_SPECIFICS */

