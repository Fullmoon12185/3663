#include "Si2164B_L2_API.h"
#define   SiDemodContext                         Si2164B_L2_Context
#define NO_SiDemod_ERROR                         NO_Si2164B_ERROR
#ifdef    UNICABLE_COMPATIBLE
#ifdef    INDIRECT_I2C_CONNECTION
                                  /* case with UNICABLE_COMPATIBLE and INDIRECT_I2C_CONNECTION */
 #define   SiDemod_SW_Init(front_end,demodAdd,tunerAdd_Ter,TER_tuner_enable_func,TER_tuner_disable_func,tunerAdd_Sat,tuner_tune,unicable_tune,SAT_tuner_enable_func,SAT_tuner_disable_func,p_context)  \
        Si2164B_L2_SW_Init(front_end,demodAdd,tunerAdd_Ter,TER_tuner_enable_func,TER_tuner_disable_func,tunerAdd_Sat,tuner_tune,unicable_tune,SAT_tuner_enable_func,SAT_tuner_disable_func,p_context)
#endif /* INDIRECT_I2C_CONNECTION */
#ifndef   INDIRECT_I2C_CONNECTION
                                  /* case with UNICABLE_COMPATIBLE only */
 #define   SiDemod_SW_Init(front_end,demodAdd,tunerAdd_Ter,TER_tuner_enable_func,TER_tuner_disable_func,tunerAdd_Sat,tuner_tune,unicable_tune,SAT_tuner_enable_func,SAT_tuner_disable_func,p_context)  \
        Si2164B_L2_SW_Init(front_end,demodAdd,tunerAdd_Ter,                                             tunerAdd_Sat,tuner_tune,unicable_tune,                                             p_context)
#endif /* INDIRECT_I2C_CONNECTION */
#endif /* UNICABLE_COMPATIBLE */

#ifndef   UNICABLE_COMPATIBLE
#ifdef    INDIRECT_I2C_CONNECTION
                                  /* case with INDIRECT_I2C_CONNECTION only */
 #define   SiDemod_SW_Init(front_end,demodAdd,tunerAdd_Ter,TER_tuner_enable_func,TER_tuner_disable_func,tunerAdd_Sat,                          SAT_tuner_enable_func,SAT_tuner_disable_func,p_context)\
         Si2164B_L2_SW_Init(front_end,demodAdd,tunerAdd_Ter,TER_tuner_enable_func,TER_tuner_disable_func,tunerAdd_Sat,                          SAT_tuner_enable_func,SAT_tuner_disable_func,p_context)
#endif /* INDIRECT_I2C_CONNECTION */
#ifndef   INDIRECT_I2C_CONNECTION
                                  /* case with no options activated */
#if 0//csm
/* #define   SiDemod_SW_Init(front_end,demodAdd,tunerAdd_Ter,TER_tuner_enable_func,TER_tuner_disable_func,tunerAdd_Sat,tuner_tune,unicable_tune,SAT_tuner_enable_func,SAT_tuner_disable_func,p_context)\*/
 #define   SiDemod_SW_Init(front_end,demodAdd,tunerAdd_Ter,TER_tuner_enable_func,TER_tuner_disable_func,tunerAdd_Sat,							SAT_tuner_enable_func,SAT_tuner_disable_func,p_context)\
		 Si2164B_L2_SW_Init(front_end,demodAdd,tunerAdd_Ter,                                             tunerAdd_Sat,                                                                      p_context)
#else
 #define   SiDemod_SW_Init(front_end,demodAdd,tunerAdd_Ter,												tunerAdd_Sat,																		p_context)\
         Si2164B_L2_SW_Init(front_end,demodAdd,tunerAdd_Ter,                                             tunerAdd_Sat,																		p_context)
#endif//end
#endif /* INDIRECT_I2C_CONNECTION */
#endif /* UNICABLE_COMPATIBLE */

#define   SiDemod_Init(front_end)                             Si2164B_Init(front_end->demod)
#define   SiDemod_Media(front_end,modulation)                 Si2164B_Media(front_end->demod,modulation)
#define   SiDemod_Configure(front_end)                        Si2164B_Configure(front_end->demod)
#define   SiDemod_PowerUpWithPatch(front_end)                 Si2164B_PowerUpWithPatch(front_end->demod)
#define   SiDemod_LoadFirmware_16(front_end,fw_table,lines)   Si2164B_LoadFirmware_16(front_end->demod,fw_table,lines)
#define   SiDemod_StartFirmware(front_end)                    Si2164B_StartFirmware(front_end->demod)
#define   SiDemod_STANDBY(front_end)                          Si2164B_STANDBY(front_end->demod)
#define   SiDemod_standardName(standard)                      Si2164B_standardName(standard)

#define   SiDemod_Infos                 Si2164B_L2_Infos
#define   SiDemod_HW_Connect            Si2164B_L2_HW_Connect
#define   SiDemod_switch_to_standard    Si2164B_L2_switch_to_standard

#define   SiDemod_lock_to_carrier(front_end,standard,freq,dvb_t_bandwidth_hz,dvb_t_stream,symbol_rate,dvb_c_constellation, dataslice_id, plp_id,T2_lock_mode) \
        Si2164B_L2_lock_to_carrier(front_end,standard,freq,dvb_t_bandwidth_hz,dvb_t_stream,symbol_rate,dvb_c_constellation, dataslice_id, plp_id,T2_lock_mode)//csm
#define   SiDemod_Channel_Lock_Abort    Si2164B_L2_Channel_Lock_Abort
#define   SiDemod_communication_check   Si2164B_L2_communication_check
#define   SiDemod_TS_mode               Si2164B_L2_TS_mode
#define   SiDemod_Tune                  Si2164B_L2_Tune
#define   SiDemod_Get_RF                Si2164B_L2_Get_RF
#define   SiDemod_SAT_LPF               Si2164B_L2_SAT_LPF

#define   SiDemod_Tuner_I2C_Enable      Si2164B_L2_Tuner_I2C_Enable
#define   SiDemod_Tuner_I2C_Disable     Si2164B_L2_Tuner_I2C_Disable
#define   SiDemod_Channel_Seek_Init     Si2164B_L2_Channel_Seek_Init
#define   SiDemod_Channel_Seek_Next(front_end,standard,freq,bandwidth_Hz,stream,symbol_rate_bps,constellation,num_data_slice,num_plp,T2_base_lite)   \
        Si2164B_L2_Channel_Seek_Next(front_end,standard,freq,bandwidth_Hz,stream,symbol_rate_bps,constellation,num_data_slice,num_plp,T2_base_lite)
#define   SiDemod_Channel_Seek_End      Si2164B_L2_Channel_Seek_End
#define   SiDemod_Channel_Seek_Abort    Si2164B_L2_Channel_Seek_Abort
#define   SiDemod_TER_FEF               Si2164B_L2_TER_FEF
#define   SiDemod_TER_FEF_SETUP         Si2164B_L2_TER_FEF_SETUP
#define   SiDemod_TerAutoDetect         Si2164B_TerAutoDetect
#define   SiDemod_TerAutoDetectOff      Si2164B_TerAutoDetectOff
#define   SiDemod_SatAutoDetect         Si2164B_SatAutoDetect
#define   SiDemod_SatAutoDetectOff      Si2164B_SatAutoDetectOff
#define   SiDemod_L2_T2_MPLP            Si2164B_L2_Test_MPLP

#define   SiDemod_prepare_diseqc_sequence Si2164B_L2_prepare_diseqc_sequence
#define   SiDemod_trigger_diseqc_sequence Si2164B_L2_trigger_diseqc_sequence
#define   SiDemod_send_diseqc_sequence    Si2164B_L2_send_diseqc_sequence
#define   SiDemod_read_diseqc_reply		  Si2164B_L2_read_diseqc_reply

#define   SiDemod_TER_Clock               Si2164B_L2_TER_Clock
#define   SiDemod_TER_AGC                 Si2164B_L2_TER_AGC
#define   SiDemod_SAT_Clock               Si2164B_L2_SAT_Clock
#define   SiDemod_SAT_AGC                 Si2164B_L2_SAT_AGC
#define   SiDemod_SAT_Spectrum            Si2164B_L2_SAT_Spectrum

#define   SiDemod_Test                    Si2164B_L2_Test
#if 0
#endif
