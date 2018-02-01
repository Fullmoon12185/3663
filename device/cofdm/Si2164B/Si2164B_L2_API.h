/***************************************************************************************
                  Silicon Laboratories Broadcast Si2164B Layer 2 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   L2 API header for commands and properties
   FILE: Si2164B_L2_API.h
   Supported IC : Si2164B
   Compiled for ROM 0 firmware 5_3_build_3
   Revision: V0.1.1.0
   Date: December 12 2014
  (C) Copyright 2014, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
/* Change log:

 As from  V0.1.1.0:
  <improvement>[traces] Adding cumulativeScanTime, cumulativeTimeoutTime, nbTimeouts and nbDecisions to Si2164B_L2_Context

 As from V0.0.9.0
  <new_feature>[superset] Changing tags to allow SILABS_SUPERSET use (one code, all configs)

 As from V0.0.5.0:
   <improvement>[Blindscan/debug] Redefining Si2164B_READ and Si2164B_WRITE to allow using either "pmajor = '4'" register definitions
     or Si2164B register definitions.

 As from V0.0.4.0:
   <renaming>[config_macros] SW_INIT_Si21682_EVB_Rev1_0_Si2164B renamed as SW_INIT_Si21682_EVB_Rev1_0_41A_83A (for GUI purposes)

 As from V0.0.0.1:
   <improvement>[UNICABLE] redefinition of the return type of the Unicable tune function (from void to signed int),
    To allow proving the final Unicable tune freq to Seek_Next.

 As from V0.0.0:
  Initial version (based on Si2164 code V0.3.4)
****************************************************************************************/

#ifndef   Si2164B_L2_API_H
#define   Si2164B_L2_API_H

#define   Si2164B_COMMAND_PROTOTYPES
#include "Si2164B_Platform_Definition.h"
#include "Si2164B_Properties_Functions.h"
#if 0//csm
#ifndef    Si2164_C60_COMPATIBLE
 #ifndef    Si2164_B55_COMPATIBLE
#ifndef    Si2164_B50_COMPATIBLE
 #ifndef    Si2164B_ES_COMPATIBLE
    "If you get a compilation error on these lines, it means that no Si2164B version has been selected.";
     "Please define Si2164_C60_COMPATIBLE, Si2164_B55_COMPATIBLE, Si2164_B50_COMPATIBLE or Si2164B_ES_COMPATIBLE at project level!";
    "Once the flags will be defined, this code will not be visible to the compiler anymore";
    "Do NOT comment these lines, they are here to help, showing if there are missing project flags";
 #endif /* Si2164B_ES_COMPATIBLE */
#endif /* Si2164_B50_COMPATIBLE */
 #endif /* Si2164_B55_COMPATIBLE */
#endif /* Si2164_C60_COMPATIBLE */
#endif//end


 #ifdef    UNICABLE_COMPATIBLE
  #include "SiLabs_Unicable_API.h"
  typedef void (*Si2164B_WRAPPER_TUNER_TUNE_FUNC )   (void*, signed int freq_kHz);
  typedef signed int  (*Si2164B_WRAPPER_UNICABLE_TUNE_FUNC) (void*, signed int freq_kHz);
 #endif /* UNICABLE_COMPATIBLE */

#ifdef    INDIRECT_I2C_CONNECTION
  typedef signed int  (*Si2164B_INDIRECT_I2C_FUNC)  (void*);
#endif /* INDIRECT_I2C_CONNECTION */

typedef struct _Si2164B_L2_Context {
   L1_Si2164B_Context *demod;
   TER_TUNER_CONTEXT *tuner_ter;
 #ifdef    INDIRECT_I2C_CONNECTION
   Si2164B_INDIRECT_I2C_FUNC  f_TER_tuner_enable;
   Si2164B_INDIRECT_I2C_FUNC  f_TER_tuner_disable;
 #endif /* INDIRECT_I2C_CONNECTION */
   SAT_TUNER_CONTEXT *tuner_sat;
 #ifdef    INDIRECT_I2C_CONNECTION
   Si2164B_INDIRECT_I2C_FUNC  f_SAT_tuner_enable;
   Si2164B_INDIRECT_I2C_FUNC  f_SAT_tuner_disable;
 #endif /* INDIRECT_I2C_CONNECTION */
   L1_Si2164B_Context  demodObj;
   TER_TUNER_CONTEXT  tuner_terObj;
 #ifndef   NO_SAT
   SAT_TUNER_CONTEXT  tuner_satObj;
 #endif /* NO_SAT */
   signed int                first_init_done;
   signed int                Si2164B_init_done;
   signed int                TER_init_done;
   signed int                TER_tuner_init_done;
   unsigned char      auto_detect_TER;
   signed int                SAT_init_done;
   signed int                SAT_tuner_init_done;
   unsigned char      auto_detect_SAT;
   unsigned char      satellite_spectrum_inversion;
   unsigned char      external_fw_download;//20130903
   unsigned char      external_rf_fw_download;
 #ifdef    UNICABLE_COMPATIBLE
   unsigned char      lnb_type;
   unsigned char      unicable_spectrum_inversion;
   Si2164B_WRAPPER_TUNER_TUNE_FUNC     f_tuner_tune;
   Si2164B_WRAPPER_UNICABLE_TUNE_FUNC  f_Unicable_tune;
 #endif /* UNICABLE_COMPATIBLE */
   void              *callback;
   signed int                standard;
   signed int                detected_rf;
   signed int                previous_standard;
   signed int                tuneUnitHz;
   signed int                rangeMin;
   signed int                rangeMax;
   signed int                seekBWHz;
   signed int                seekStepHz;
   signed int                minSRbps;
   signed int                maxSRbps;
   signed int                minRSSIdBm;
   signed int                maxRSSIdBm;
   signed int                minSNRHalfdB;
   signed int                maxSNRHalfdB;
   signed int                seekAbort;
   signed int                lockAbort;
   signed int                searchStartTime;  /* searchStartTime  is used to trace the time spent trying to detect a channel.                      */
                                        /* It is set differently from seekStartTime when returning from a handshake                          */
   signed int                timeoutStartTime; /* timeoutStartTime is used in blind mode to make sure the FW is correctly responding.               */
                                        /* timeoutStartTime is used in non-blind mode to manage the timeout when trying to lock on a channel */
   signed int                handshakeUsed;
   signed int                handshakeStart_ms;
   signed int                handshakePeriod_ms;
   signed int                cumulativeScanTime;
   signed int                cumulativeTimeoutTime;
   signed int                nbTimeouts;
   signed int                nbDecisions;
   unsigned char      handshakeOn;
   signed int                center_rf;
} Si2164B_L2_Context;

/* firmware_struct needs to be declared to allow loading the FW in 16 bytes mode */
#ifndef __FIRMWARE_STRUCT__
#define __FIRMWARE_STRUCT__
typedef struct firmware_struct {
  unsigned char firmware_len;
  unsigned char firmware_table[16];
} firmware_struct;
#endif /* __FIRMWARE_STRUCT__ */

signed int   Si2164B_Init                      (L1_Si2164B_Context *api);
signed int   Si2164B_Media                     (L1_Si2164B_Context *api, signed int modulation);
signed int   Si2164B_Configure                 (L1_Si2164B_Context *api);
signed int   Si2164B_PowerUpWithPatch          (L1_Si2164B_Context *api);
signed int   Si2164B_LoadFirmware_16           (L1_Si2164B_Context *api, firmware_struct fw_table[], signed int nbLines);
//signed int   Si2164B_LoadFirmwareSPI           (L1_Si2164B_Context *api, firmware_struct fw_table[], signed int nbLines, unsigned char pbl_key,  unsigned char pbl_num); // 20140513 del jhkwon
//signed int   Si2164B_LoadFirmwareSPI_Split     (L1_Si2164B_Context *api, unsigned char *fw_table, int nbBytes, unsigned char pbl_key,  unsigned char pbl_num , unsigned int num_split, unsigned int *split_list);
signed int   Si2164B_StartFirmware             (L1_Si2164B_Context *api);
signed int   Si2164B_STANDBY                   (L1_Si2164B_Context *api);
signed int   Si2164B_WAKEUP                    (L1_Si2164B_Context *api);
const unsigned char *Si2164B_standardName              (signed int standard);
signed int   Si2164B_L2_SILENT                    (Si2164B_L2_Context *front_end, signed int silent);

/*****************************************************************************************/
/*               SiLabs demodulator API functions (demod and tuner)                      */
/*****************************************************************************************/
    /*  Software info functions */
signed int   Si2164B_L2_Infos                     (Si2164B_L2_Context *front_end, unsigned char *infoString);

    /*  Software init functions */
unsigned char  Si2164B_L2_SW_Init                  (Si2164B_L2_Context *front_end
                                   , signed int demodAdd
                                   , signed int tunerAdd_Ter
 #ifdef    INDIRECT_I2C_CONNECTION
                                   , Si2164B_INDIRECT_I2C_FUNC           TER_tuner_enable_func
                                   , Si2164B_INDIRECT_I2C_FUNC           TER_tuner_disable_func
 #endif /* INDIRECT_I2C_CONNECTION */
                                   , signed int tunerAdd_Sat
#ifdef    UNICABLE_COMPATIBLE
                                   , Si2164B_WRAPPER_TUNER_TUNE_FUNC     tuner_tune
                                   , Si2164B_WRAPPER_UNICABLE_TUNE_FUNC  unicable_tune
#endif /* UNICABLE_COMPATIBLE */
 #ifdef    INDIRECT_I2C_CONNECTION
                                   , Si2164B_INDIRECT_I2C_FUNC           SAT_tuner_enable_func
                                   , Si2164B_INDIRECT_I2C_FUNC           SAT_tuner_disable_func
 #endif /* INDIRECT_I2C_CONNECTION */
                                   , void *p_context
                                   );
//signed int  Si2164B_L2_Set_Index_and_Tag         (Si2164B_L2_Context   *front_end, unsigned char index, unsigned char* tag); // 20140513 del jhkwon
void  Si2164B_L2_HW_Connect               (Si2164B_L2_Context   *front_end, CONNECTION_TYPE mode);
    /*  Locking and status functions */
signed int   Si2164B_L2_switch_to_standard       (Si2164B_L2_Context   *front_end, unsigned char new_standard, unsigned char force_full_init);
signed int   Si2164B_L2_lock_to_carrier   (Si2164B_L2_Context *front_end
                                 ,  signed int standard
                                 ,  signed int freq
                                  , signed int ter_bandwidth_hz
                                  , unsigned char dvb_t_stream
                                  , unsigned int symbol_rate_bps
                                  , unsigned char dvb_c_constellation
                                  , signed int data_slice_id
                                  , signed int plp_id
                                  , unsigned char T2_lock_mode
                                    );
signed int   Si2164B_L2_Channel_Lock_Abort       (Si2164B_L2_Context   *front_end);
signed int   Si2164B_L2_communication_check      (Si2164B_L2_Context   *front_end);
signed int   Si2164B_L2_Part_Check               (Si2164B_L2_Context   *front_end);
unsigned char Si2164B_L2_Set_Invert_Spectrum(Si2164B_L2_Context *front_end);
    /*  TS functions */
signed int   Si2164B_L2_TS_mode                  (Si2164B_L2_Context   *front_end, signed int ts_mode);

    /*  Tuner wrapping functions */
signed int   Si2164B_L2_Tune                     (Si2164B_L2_Context   *front_end, signed int rf);
signed int   Si2164B_L2_Get_RF                   (Si2164B_L2_Context   *front_end);

    /* LNB control and DiSEqC functions */
signed int   Si2164B_L2_prepare_diseqc_sequence  (Si2164B_L2_Context   *front_end, signed int sequence_length, unsigned char *sequenceBuffer, unsigned char cont_tone, unsigned char tone_burst, unsigned char burst_sel, unsigned char end_seq);
signed int   Si2164B_L2_trigger_diseqc_sequence  (Si2164B_L2_Context   *front_end);
signed int   Si2164B_L2_send_diseqc_sequence     (Si2164B_L2_Context   *front_end, signed int sequence_length, unsigned char *sequenceBuffer, unsigned char cont_tone, unsigned char tone_burst, unsigned char burst_sel, unsigned char end_seq);
signed int   Si2164B_L2_read_diseqc_reply        (Si2164B_L2_Context   *front_end, signed int *reply_length  , unsigned char *replyBuffer   );

    /*  Tuner i2c bus control functions */
unsigned char Si2164B_L2_Tuner_I2C_Enable (Si2164B_L2_Context   *front_end);
unsigned char Si2164B_L2_Tuner_I2C_Disable(Si2164B_L2_Context   *front_end);

    /*  Scanning functions */
const unsigned char *Si2164B_L2_Trace_Scan_Status  (signed int scan_status);
signed int   Si2164B_L2_Channel_Seek_Init        (Si2164B_L2_Context   *front_end,
                                          signed int rangeMinHz,   signed int rangeMaxHz,
                                          signed int seekBWHz,     signed int seekStepHz,
                                          signed int minSRbps,     signed int maxSRbps,
                                          signed int minRSSIdBm,   signed int maxRSSIdBm,
                                          signed int minSNRHalfdB, signed int maxSNRHalfdB);
signed int  Si2164B_L2_Channel_Seek_Next         (Si2164B_L2_Context   *front_end
                                           , signed int *standard
                                           , signed int *freq
                                           , signed int *bandwidth_Hz
                                           , signed int *stream
                                           , unsigned int *symbol_rate_bps
                                           , signed int *constellation
                                           , signed int *num_data_slice
                                           , signed int *num_plp
                                           , signed int *T2_base_lite
                                           );
signed int   Si2164B_L2_Channel_Seek_End         (Si2164B_L2_Context   *front_end);
signed int   Si2164B_L2_Channel_Seek_Abort       (Si2164B_L2_Context   *front_end);
#define   AGC_FUNCTIONS
signed int   Si2164B_L2_TER_Clock                (Si2164B_L2_Context   *front_end, signed int clock_source, signed int clock_input, signed int clock_freq, signed int clock_control);
signed int   Si2164B_L2_TER_AGC                  (Si2164B_L2_Context   *front_end, signed int agc1_mode, signed int agc1_inversion, signed int agc2_mode, signed int agc2_inversion);
signed int   Si2164B_L2_TER_FEF_CONFIG           (Si2164B_L2_Context   *front_end, signed int fef_mode, signed int fef_pin, signed int fef_level);
signed int   Si2164B_L2_TER_FEF                  (Si2164B_L2_Context   *front_end, signed int fef);
signed int   Si2164B_L2_TER_FEF_SETUP            (Si2164B_L2_Context   *front_end, signed int fef);
signed int   Si2164B_TerAutoDetect               (Si2164B_L2_Context   *front_end);
signed int   Si2164B_TerAutoDetectOff            (Si2164B_L2_Context   *front_end);
signed int   Si2164B_L2_SAT_Clock                (Si2164B_L2_Context   *front_end, signed int clock_source, signed int clock_input, signed int clock_freq, signed int clock_control);
signed int   Si2164B_L2_SAT_AGC                  (Si2164B_L2_Context   *front_end, signed int agc1_mode, signed int agc1_inversion, signed int agc2_mode, signed int agc2_inversion);
signed int   Si2164B_L2_SAT_Spectrum             (Si2164B_L2_Context   *front_end, signed int spectrum_inversion);
signed int   Si2164B_L2_SAT_LPF                  (Si2164B_L2_Context   *front_end, signed int lpf_khz);
signed int   Si2164B_SatAutoDetect               (Si2164B_L2_Context   *front_end);
signed int   Si2164B_SatAutoDetectOff            (Si2164B_L2_Context   *front_end);

//20130903,related to kernel driver
signed int  Si2164B_L2_firmwares_download        (Si2164B_L2_Context *front_end,signed int clk_mode, signed int demodAdd, signed int tunerAdd_Ter, signed int tunerAdd_Sat);
void  Si2164B_publishFirmwarePointer(Si2164B_L2_Context *front_end,unsigned long *fw_addr,unsigned long *fw_size);
//end

#ifdef    SILABS_API_TEST_PIPE
signed int   Si2164B_L2_Test                     (Si2164B_L2_Context *front_end, unsigned char *target, unsigned char *cmd, unsigned char *sub_cmd, double dval, double *retdval, unsigned char **rettxt);
#endif /* SILABS_API_TEST_PIPE */

#define Si2164B_READ(ptr, register)          if (ptr->rsp->part_info.pmajor == '4' ) { Si2164B_L1_DD_GET_REG (ptr, Si2164_##register##_LSB, Si2164_##register##_MID, Si2164_##register##_MSB);}\
                                          else                                      { Si2164B_L1_DD_GET_REG (ptr, Si2164B_##register##_LSB, Si2164B_##register##_MID, Si2164B_##register##_MSB);}\
                                          register = (ptr->rsp->dd_get_reg.data1<<0) + (ptr->rsp->dd_get_reg.data2<<8) + (ptr->rsp->dd_get_reg.data3<<16) + (ptr->rsp->dd_get_reg.data4<<24);
#define Si2164B_WRITE(ptr, register, value)  if (ptr->rsp->part_info.pmajor == '4' ) { Si2164B_L1_DD_SET_REG (ptr, Si2164_##register##_LSB, Si2164_##register##_MID, Si2164_##register##_MSB,value);}\
                                          else                                      { Si2164B_L1_DD_SET_REG (ptr, Si2164B_##register##_LSB, Si2164B_##register##_MID, Si2164B_##register##_MSB,value);}\


#define   MACROS_FOR_SINGLE_USING_Si2164B
#ifdef    MACROS_FOR_SINGLE_USING_Si2164B

#define SW_INIT_Si216x_EVB_Rev3_0_Si2164B\
  /* SW Init for front end 0 */\
  front_end  = &(FrontEnd_Table[0]);\
  SiLabs_API_Frontend_Chip            (front_end, 0x2164B);\
  SiLabs_API_SW_Init                  (front_end, 0xc8, 0xc0, 0x18);\
  SiLabs_API_Select_TER_Tuner         (front_end, 0x2178, 0);\
  SiLabs_API_TER_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_TER_Tuner_ClockConfig    (front_end, 1, 1);\
  SiLabs_API_TER_Clock                (front_end, 1, 44, 24, 2);\
  SiLabs_API_TER_FEF_Config           (front_end, 1, 0xb, 1);\
  SiLabs_API_TER_AGC                  (front_end, 0x0, 0, 0xa, 0);\
  SiLabs_API_TER_Tuner_AGC_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_FEF_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_IF_Output      (front_end, 0);\
  SiLabs_API_Select_SAT_Tuner         (front_end, 0x5815, 0);\
  SiLabs_API_SAT_Select_LNB_Chip      (front_end, 25, 0x10);\
  SiLabs_API_SAT_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_SAT_Clock                (front_end, 1, 44, 24, 2);\
  SiLabs_API_SAT_Spectrum             (front_end, 1);\
  SiLabs_API_SAT_AGC                  (front_end, 0xd, 1, 0x0, 1);\
  SiLabs_API_Set_Index_and_Tag        (front_end, 0, "fe[0]");\
  SiLabs_API_HW_Connect               (front_end, 1);

#define SW_INIT_Platform_2010_Rev1_0_Si2164B\
  /* SW Init for front end 0 */\
  front_end  = &(FrontEnd_Table[0]);\
  SiLabs_API_Frontend_Chip            (front_end, 0x2164B);\
  SiLabs_API_SW_Init                  (front_end, 0xcc, 0xc0, 0x18);\
  SiLabs_API_Select_TER_Tuner         (front_end, 0x2176, 0);\
  SiLabs_API_TER_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_TER_Tuner_ClockConfig    (front_end, 1, 1);\
  SiLabs_API_TER_Clock                (front_end, 0, 44, 24, 2);\
  SiLabs_API_TER_FEF_Config           (front_end, 1, 0xb, 1);\
  SiLabs_API_TER_AGC                  (front_end, 0x0, 0, 0xa, 0);\
  SiLabs_API_TER_Tuner_AGC_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_FEF_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_IF_Output      (front_end, 0);\
  SiLabs_API_Select_SAT_Tuner         (front_end, 0x5816, 0);\
  SiLabs_API_SAT_Select_LNB_Chip      (front_end, 21, 0x10);\
  SiLabs_API_SAT_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_SAT_Clock                (front_end, 2, 33, 27, 2);\
  SiLabs_API_SAT_Spectrum             (front_end, 1);\
  SiLabs_API_SAT_AGC                  (front_end, 0xd, 1, 0x0, 1);\
  SiLabs_API_Set_Index_and_Tag        (front_end, 0, "fe[0]");\
  front_end->Si2164B_FE->TER_init_done = 1;\
  front_end->Si2164B_FE->TER_tuner_init_done = 1;\
  front_end->Si2164B_FE->first_init_done     = 1;\
  front_end->standard = SILABS_ISDB_T;\
  front_end->Si2164B_FE->previous_standard = Si2164B_DD_MODE_PROP_MODULATION_DVBT;\
  SiLabs_API_HW_Connect               (front_end, 1);

#define SW_INIT_Si216x_SOC_EVB_Rev1_0_Si2178_Si2164B\
  /* SW Init for front end 0 */\
  front_end                   = &(FrontEnd_Table[0]);\
  SiLabs_API_Frontend_Chip            (front_end, 0x2164B);\
  SiLabs_API_SW_Init                  (front_end, 0xc8, 0xc0, 0x18);\
  SiLabs_API_Select_TER_Tuner         (front_end, 0x2178, 0);\
  SiLabs_API_TER_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_TER_Tuner_ClockConfig    (front_end, 1, 1);\
  SiLabs_API_TER_Clock                (front_end, 1, 44, 24, 2);\
  SiLabs_API_TER_FEF_Config           (front_end, 1, 0xb, 1);\
  SiLabs_API_TER_AGC                  (front_end, 0x0, 0, 0xa, 0);\
  SiLabs_API_TER_Tuner_AGC_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_FEF_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_IF_Output      (front_end, 0);\
  SiLabs_API_Select_SAT_Tuner         (front_end, 0x5816, 0);\
  SiLabs_API_SAT_Select_LNB_Chip      (front_end, 26, 0x10);\
  SiLabs_API_SAT_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_SAT_Clock                (front_end, 2, 33, 27, 2);\
  SiLabs_API_SAT_Spectrum             (front_end, 1);\
  SiLabs_API_SAT_AGC                  (front_end, 0xd, 1, 0x0, 1);\
  SiLabs_API_Set_Index_and_Tag        (front_end, 0, "fe[0]");\
  SiLabs_API_HW_Connect               (front_end, 1);

#define SW_INIT_Si216x_SOC_EVB_Rev1_0_Si2178B_Si2164B\
  /* SW Init for front end 0 */\
  front_end                   = &(FrontEnd_Table[0]);\
  SiLabs_API_Frontend_Chip            (front_end, 0x2164B);\
  SiLabs_API_SW_Init                  (front_end, 0xc8, 0xc0, 0x18);\
  SiLabs_API_Select_TER_Tuner         (front_end, 0x2178B, 0);\
  SiLabs_API_TER_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_TER_Tuner_ClockConfig    (front_end, 1, 1);\
  SiLabs_API_TER_Clock                (front_end, 1, 44, 24, 2);\
  SiLabs_API_TER_FEF_Config           (front_end, 1, 0xb, 1);\
  SiLabs_API_TER_AGC                  (front_end, 0x0, 0, 0xa, 0);\
  SiLabs_API_TER_Tuner_AGC_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_FEF_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_IF_Output      (front_end, 0);\
  SiLabs_API_Select_SAT_Tuner         (front_end, 0x5816, 0);\
  SiLabs_API_SAT_Select_LNB_Chip      (front_end, 26, 0x10);\
  SiLabs_API_SAT_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_SAT_Clock                (front_end, 2, 33, 27, 2);\
  SiLabs_API_SAT_Spectrum             (front_end, 1);\
  SiLabs_API_SAT_AGC                  (front_end, 0xd, 1, 0x0, 1);\
  SiLabs_API_Set_Index_and_Tag        (front_end, 0, "fe[0]");\
  SiLabs_API_HW_Connect               (front_end, 1);

#define SW_INIT_Si2141_51_DVBT2TC_DC_Rev1_2_Si2168C\
  /* SW Init for front end 0 */\
  front_end                   = &(FrontEnd_Table[0]);\
  SiLabs_API_Frontend_Chip            (front_end, 0x2164B);\
  SiLabs_API_SW_Init                  (front_end, 0xc8, 0xc0, 0x18);\
  SiLabs_API_Select_TER_Tuner         (front_end, 0x2151, 0);\
  SiLabs_API_TER_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_TER_Tuner_ClockConfig    (front_end, 1, 1);\
  SiLabs_API_TER_Clock                (front_end, 1, 44, 24, 1);\
  SiLabs_API_TER_FEF_Config           (front_end, 1, 0xb, 1);\
  SiLabs_API_TER_AGC                  (front_end, 0x0, 0, 0xa, 0);\
  SiLabs_API_TER_Tuner_AGC_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_IF_Output      (front_end, 0);\
  SiLabs_API_Set_Index_and_Tag        (front_end, 0, "fe[0]");\
  SiLabs_API_HW_Connect               (front_end, 1);

#endif /* MACROS_FOR_SINGLE_USING_Si2164B */

#define   MACROS_FOR_DUAL_USING_Si2164B

#ifdef    MACROS_FOR_DUAL_USING_Si2164B

#define SW_INIT_Si216x2_EVB_Rev1_x_91A_83A\
  /* SW Init for front end 0 */\
  front_end                   = &(FrontEnd_Table[0]);\
  SiLabs_API_Frontend_Chip            (front_end, 0x2164B);\
  SiLabs_API_SW_Init                  (front_end, 0xc8, 0xc0, 0x14);\
  SiLabs_API_Select_TER_Tuner         (front_end, 0x2191, 0);\
  SiLabs_API_TER_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_TER_Tuner_ClockConfig    (front_end, 1, 1);\
  SiLabs_API_TER_Clock                (front_end, 1, 44, 24, 1);\
  SiLabs_API_TER_FEF_Config           (front_end, 1, 0xa, 1);\
  SiLabs_API_TER_AGC                  (front_end, 0x0, 0, 0xc, 0);\
  SiLabs_API_TER_Tuner_AGC_Input      (front_end, 2);\
  SiLabs_API_TER_Tuner_FEF_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_IF_Output      (front_end, 0);\
  SiLabs_API_Select_SAT_Tuner         (front_end, 0x5816, 0);\
  SiLabs_API_SAT_Select_LNB_Chip      (front_end, 26, 0x10);\
  SiLabs_API_SAT_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_SAT_Clock                (front_end, 2, 33, 27, 1);\
  SiLabs_API_SAT_Spectrum             (front_end, 0);\
  SiLabs_API_SAT_AGC                  (front_end, 0xc, 1, 0x0, 0);\
  SiLabs_API_Set_Index_and_Tag        (front_end, 0, "fe[0]");\
  SiLabs_API_HW_Connect               (front_end, 1);\
  /* SW Init for front end 1 */\
  front_end                    = &(FrontEnd_Table[1]);\
  SiLabs_API_Frontend_Chip            (front_end, 0x2164B);\
  SiLabs_API_SW_Init                  (front_end, 0xce, 0xc6, 0x16);\
  SiLabs_API_Select_TER_Tuner         (front_end, 0x2191, 0);\
  SiLabs_API_TER_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_TER_Tuner_ClockConfig    (front_end, 0, 1);\
  SiLabs_API_TER_Clock                (front_end, 1, 44, 24, 0);\
  SiLabs_API_TER_FEF_Config           (front_end, 1, 0xb, 1);\
  SiLabs_API_TER_AGC                  (front_end, 0x0, 0, 0xd, 0);\
  SiLabs_API_TER_Tuner_AGC_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_FEF_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_IF_Output      (front_end, 0);\
  SiLabs_API_Select_SAT_Tuner         (front_end, 0x5816, 0);\
  SiLabs_API_SAT_Select_LNB_Chip      (front_end, 26, 0x10);\
  SiLabs_API_SAT_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_SAT_Clock                (front_end, 2, 33, 27, 0);\
  SiLabs_API_SAT_Spectrum             (front_end, 0);\
  SiLabs_API_SAT_AGC                  (front_end, 0xd, 1, 0x0, 0);\
  SiLabs_API_Set_Index_and_Tag        (front_end, 1, "fe[1]");\
  SiLabs_API_HW_Connect               (front_end, 1);

#define SW_INIT_Si216x2_EVB_Rev1_x_Si2178B_Si2164B\
  /* SW Init for front end 0 */\
  front_end                   = &(FrontEnd_Table[0]);\
  SiLabs_API_Frontend_Chip            (front_end, 0x2164B);\
  SiLabs_API_SW_Init                  (front_end, 0xc8, 0xc0, 0x14);\
  SiLabs_API_Select_TER_Tuner         (front_end, 0x2178B, 0);\
  SiLabs_API_TER_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_TER_Tuner_ClockConfig    (front_end, 1, 1);\
  SiLabs_API_TER_Clock                (front_end, 1, 44, 24, 1);\
  SiLabs_API_TER_FEF_Config           (front_end, 1, 0xa, 1);\
  SiLabs_API_TER_AGC                  (front_end, 0x0, 0, 0xc, 0);\
  SiLabs_API_TER_Tuner_AGC_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_FEF_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_IF_Output      (front_end, 0);\
  SiLabs_API_Select_SAT_Tuner         (front_end, 0x5816, 0);\
  SiLabs_API_SAT_Select_LNB_Chip      (front_end, 26, 0x10);\
  SiLabs_API_SAT_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_SAT_Clock                (front_end, 2, 33, 27, 1);\
  SiLabs_API_SAT_Spectrum             (front_end, 0);\
  SiLabs_API_SAT_AGC                  (front_end, 0xc, 1, 0x0, 0);\
  SiLabs_API_Set_Index_and_Tag        (front_end, 0, "fe[0]");\
  SiLabs_API_HW_Connect               (front_end, 1);\
  /* SW Init for front end 1 */\
  front_end                    = &(FrontEnd_Table[1]);\
  SiLabs_API_Frontend_Chip            (front_end, 0x2164B);\
  SiLabs_API_SW_Init                  (front_end, 0xce, 0xc6, 0x16);\
  SiLabs_API_Select_TER_Tuner         (front_end, 0x2178B, 0);\
  SiLabs_API_TER_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_TER_Tuner_ClockConfig    (front_end, 0, 1);\
  SiLabs_API_TER_Clock                (front_end, 1, 44, 24, 1);\
  SiLabs_API_TER_FEF_Config           (front_end, 1, 0xb, 1);\
  SiLabs_API_TER_AGC                  (front_end, 0x0, 0, 0xd, 0);\
  SiLabs_API_TER_Tuner_AGC_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_FEF_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_IF_Output      (front_end, 0);\
  SiLabs_API_Select_SAT_Tuner         (front_end, 0x5816, 0);\
  SiLabs_API_SAT_Select_LNB_Chip      (front_end, 26, 0x10);\
  SiLabs_API_SAT_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_SAT_Clock                (front_end, 2, 33, 27, 1);\
  SiLabs_API_SAT_Spectrum             (front_end, 0);\
  SiLabs_API_SAT_AGC                  (front_end, 0xd, 1, 0x0, 0);\
  SiLabs_API_Set_Index_and_Tag        (front_end, 1, "fe[1]");\
  SiLabs_API_HW_Connect               (front_end, 1);

#define SW_INIT_Si216x2_EVB_Rev2_0_Si2164B\
  /* SW Init for front end 0 */\
  front_end                   = &(FrontEnd_Table[0]);\
  SiLabs_API_Frontend_Chip            (front_end, 0x2164B);\
  SiLabs_API_SW_Init                  (front_end, 0xc8, 0xc0, 0x14);\
  SiLabs_API_Select_TER_Tuner         (front_end, 0x2177, 0);\
  SiLabs_API_TER_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_TER_Tuner_ClockConfig    (front_end, 1, 1);\
  SiLabs_API_TER_Clock                (front_end, 1, 44, 24, 1);\
  SiLabs_API_TER_FEF_Config           (front_end, 1, 0xa, 1);\
  SiLabs_API_TER_AGC                  (front_end, 0x0, 0, 0xc, 0);\
  SiLabs_API_TER_Tuner_AGC_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_FEF_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_IF_Output      (front_end, 0);\
  SiLabs_API_Select_SAT_Tuner         (front_end, 0x5816, 0);\
  SiLabs_API_SAT_Select_LNB_Chip      (front_end, 26, 0x10);\
  SiLabs_API_SAT_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_SAT_Clock                (front_end, 2, 33, 27, 1);\
  SiLabs_API_SAT_Spectrum             (front_end, 0);\
  SiLabs_API_SAT_AGC                  (front_end, 0xc, 1, 0x0, 0);\
  SiLabs_API_Set_Index_and_Tag        (front_end, 0, "fe[0]");\
  SiLabs_API_HW_Connect               (front_end, 1);\
  /* SW Init for front end 1 */\
  front_end                    = &(FrontEnd_Table[1]);\
  SiLabs_API_Frontend_Chip            (front_end, 0x2164B);\
  SiLabs_API_SW_Init                  (front_end, 0xce, 0xc6, 0x16);\
  SiLabs_API_Select_TER_Tuner         (front_end, 0x2157, 0);\
  SiLabs_API_TER_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_TER_Tuner_ClockConfig    (front_end, 0, 1);\
  SiLabs_API_TER_Clock                (front_end, 1, 44, 24, 2);\
  SiLabs_API_TER_FEF_Config           (front_end, 1, 0xb, 1);\
  SiLabs_API_TER_AGC                  (front_end, 0x0, 0, 0xd, 0);\
  SiLabs_API_TER_Tuner_AGC_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_FEF_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_IF_Output      (front_end, 0);\
  SiLabs_API_Select_SAT_Tuner         (front_end, 0x5816, 0);\
  SiLabs_API_SAT_Select_LNB_Chip      (front_end, 26, 0x10);\
  SiLabs_API_SAT_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_SAT_Clock                (front_end, 2, 33, 27, 0);\
  SiLabs_API_SAT_Spectrum             (front_end, 0);\
  SiLabs_API_SAT_AGC                  (front_end, 0xd, 1, 0x0, 0);\
  SiLabs_API_Set_Index_and_Tag        (front_end, 1, "fe[1]");\
  SiLabs_API_HW_Connect               (front_end, 1);

#define SW_INIT_Si21682_EVB_Rev1_0_41A_83A\
  /* SW Init for front end 0 (TER-ONLY) */\
  front_end                    = &(FrontEnd_Table[0]);\
  SiLabs_API_Frontend_Chip            (front_end, 0x2164B);\
  SiLabs_API_SW_Init                  (front_end, 0xc8, 0xc0, 0);\
  SiLabs_API_Select_TER_Tuner         (front_end, 0x2141, 0);\
  SiLabs_API_TER_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_TER_Tuner_ClockConfig    (front_end, 1, 1);\
  SiLabs_API_TER_Clock                (front_end, 1, 44, 24, 1);\
  SiLabs_API_TER_FEF_Config           (front_end, 1, 0xa, 1);\
  SiLabs_API_TER_AGC                  (front_end, 0x0, 0, 0xc, 0);\
  SiLabs_API_TER_Tuner_AGC_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_FEF_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_IF_Output      (front_end, 0);\
  SiLabs_API_Set_Index_and_Tag        (front_end, 0, "fe[0]");\
  SiLabs_API_HW_Connect               (front_end, 1);\
  /* SW Init for front end 1 (TER-ONLY) */\
  front_end                    = &(FrontEnd_Table[1]);\
  SiLabs_API_Frontend_Chip            (front_end, 0x2164B);\
  SiLabs_API_SW_Init                  (front_end, 0xce, 0xc6, 0);\
  SiLabs_API_Select_TER_Tuner         (front_end, 0x2141, 0);\
  SiLabs_API_TER_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_TER_Tuner_ClockConfig    (front_end, 0, 1);\
  SiLabs_API_TER_Clock                (front_end, 1, 44, 24, 1);\
  SiLabs_API_TER_FEF_Config           (front_end, 1, 0xb, 1);\
  SiLabs_API_TER_AGC                  (front_end, 0x0, 0, 0xd, 0);\
  SiLabs_API_TER_Tuner_AGC_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_FEF_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_IF_Output      (front_end, 0);\
  SiLabs_API_Set_Index_and_Tag        (front_end, 1, "fe[1]");\
  SiLabs_API_HW_Connect               (front_end, 1);

#endif /* MACROS_FOR_DUAL_USING_Si2164B */

 #define   MACROS_FOR_QUAD_USING_Si2164B

#ifdef    MACROS_FOR_QUAD_USING_Si2164B

  #define SW_INIT_Dual_Si2191_Si216x2_Si2164B\
  \
  front_end                    = &(FrontEnd_Table[0]);\
  SiLabs_API_Frontend_Chip            (front_end, 0x2164B);\
  SiLabs_API_SW_Init                  (front_end, 0xc8, 0xc0, 0x14);\
  SiLabs_API_Set_Index_and_Tag        (front_end, 0, "fe[0]");\
  SiLabs_API_Select_TER_Tuner         (front_end, 0x2178B, 0);\
  SiLabs_API_TER_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_TER_Tuner_ClockConfig    (front_end, 1, 1);\
  SiLabs_API_TER_Clock                (front_end, 1, 44, 24, 1);\
  SiLabs_API_TER_AGC                  (front_end, 0x0, 0, 0xc, 0);\
  SiLabs_API_TER_Tuner_AGC_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_FEF_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_IF_Output      (front_end, 0);\
  SiLabs_API_Select_SAT_Tuner         (front_end, 0x5816, 0);\
  SiLabs_API_SAT_Select_LNB_Chip      (front_end, 26, 0x10);\
  SiLabs_API_SAT_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_SAT_Clock                (front_end, 1, 44, 24, 1);\
  SiLabs_API_SAT_Spectrum             (front_end, 1);\
  SiLabs_API_SAT_AGC                  (front_end, 0xc, 1, 0x0 , 0);\
  SiLabs_API_HW_Connect               (front_end, 1);\
  \
  front_end                   = &(FrontEnd_Table[1]);\
  SiLabs_API_Frontend_Chip            (front_end, 0x2164B);\
  SiLabs_API_SW_Init                  (front_end, 0xca, 0xc6, 0x16);\
  SiLabs_API_Set_Index_and_Tag        (front_end, 1, "fe[1]");\
  SiLabs_API_Select_TER_Tuner         (front_end, 0x2178B, 0);\
  SiLabs_API_TER_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_TER_Tuner_ClockConfig    (front_end, 0, 1);\
  SiLabs_API_TER_Clock                (front_end, 1, 44, 24, 0);\
  SiLabs_API_TER_AGC                  (front_end, 0x0, 0, 0xd, 0);\
  SiLabs_API_TER_Tuner_AGC_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_FEF_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_IF_Output      (front_end, 0);\
  SiLabs_API_Select_SAT_Tuner         (front_end, 0x5816, 0);\
  SiLabs_API_SAT_Select_LNB_Chip      (front_end, 26, 0x12);\
  SiLabs_API_SAT_tuner_I2C_connection (front_end, 0);\
  SiLabs_API_SAT_Clock                (front_end, 1, 44, 24, 0);\
  SiLabs_API_SAT_Spectrum             (front_end, 1);\
  SiLabs_API_SAT_AGC                  (front_end, 0xd, 1, 0x0 , 0);\
  SiLabs_API_HW_Connect               (front_end, 1);\
  \
  front_end                   = &(FrontEnd_Table[2]);\
  SiLabs_API_Frontend_Chip            (front_end, 0x2164B);\
  SiLabs_API_SW_Init                  (front_end, 0xce, 0xc2, 0x00);\
  SiLabs_API_Set_Index_and_Tag        (front_end, 2, "fe[2]");\
  SiLabs_API_Select_TER_Tuner         (front_end, 0x2178B, 0);\
  SiLabs_API_TER_tuner_I2C_connection (front_end, 2);\
  SiLabs_API_TER_Tuner_ClockConfig    (front_end, 1, 1);\
  SiLabs_API_TER_Clock                (front_end, 1, 44, 24, 1);\
  SiLabs_API_TER_AGC                  (front_end, 0x0, 0, 0xc, 0);\
  SiLabs_API_TER_Tuner_AGC_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_FEF_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_IF_Output      (front_end, 0);\
  SiLabs_API_Select_SAT_Tuner         (front_end, 0x5816, 0);\
  SiLabs_API_SAT_Select_LNB_Chip      (front_end, 26, 0x14);\
  SiLabs_API_SAT_tuner_I2C_connection (front_end, 2);\
  SiLabs_API_SAT_Clock                (front_end, 1, 44, 24, 1);\
  SiLabs_API_SAT_Spectrum             (front_end, 1);\
  SiLabs_API_SAT_AGC                  (front_end, 0x0, 0, 0x0 , 0);\
  SiLabs_API_HW_Connect               (front_end, 1);\
  \
  front_end                   = &(FrontEnd_Table[3]);\
  SiLabs_API_Frontend_Chip            (front_end, 0x2164B);\
  SiLabs_API_SW_Init                  (front_end, 0xcc, 0xc4, 0x00);\
  SiLabs_API_Set_Index_and_Tag        (front_end, 3, "fe[3]");\
  SiLabs_API_Select_TER_Tuner         (front_end, 0x2178B, 0);\
  SiLabs_API_TER_tuner_I2C_connection (front_end, 2);\
  SiLabs_API_TER_Tuner_ClockConfig    (front_end, 0, 1);\
  SiLabs_API_TER_Clock                (front_end, 1, 44, 24, 0);\
  SiLabs_API_TER_AGC                  (front_end, 0x0, 0, 0xd, 0);\
  SiLabs_API_TER_Tuner_AGC_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_FEF_Input      (front_end, 1);\
  SiLabs_API_TER_Tuner_IF_Output      (front_end, 0);\
  SiLabs_API_SAT_Select_LNB_Chip      (front_end, 26, 0x16);\
  SiLabs_API_Select_SAT_Tuner         (front_end, 0x5816, 0);\
  SiLabs_API_SAT_tuner_I2C_connection (front_end, 2);\
  SiLabs_API_SAT_Clock                (front_end, 1, 44, 24, 0);\
  SiLabs_API_SAT_Spectrum             (front_end, 1);\
  SiLabs_API_SAT_AGC                  (front_end, 0x0, 0, 0x0 , 0);\
  SiLabs_API_HW_Connect               (front_end, 1);

#endif /* MACROS_FOR_QUAD_USING_Si2164B */

#endif /* Si2164B_L2_API_H */







