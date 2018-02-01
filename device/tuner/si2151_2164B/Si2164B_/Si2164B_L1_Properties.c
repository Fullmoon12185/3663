/*************************************************************************************
                  Silicon Laboratories Broadcast Si2164B Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API properties definitions
   FILE: Si2164B_L1_Properties.c
   Supported IC : Si2164B
   Compiled for ROM 2 firmware 5_B_build_1
   Revision: V0.1.1.0
   Tag:  V0.1.1.0
   Date: March 30 2015
  (C) Copyright 2015, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
/* Change log:

 As from V0.1.0.0:
  <improvement>[traces] In Si2164B_L1_SetProperty: tracing property fields from prop instead of propShadow,
   to trace the final values of the property fields instead of the previous ones
  <improvement>[DVB-C/BLINDSCAN] setting prop->dvbc_afc_range.range_khz to 200 kHz, to follow FW changes.

 As from V0.0.8.0: <improvement>[SAT/AFC_RANGE] In Si2164B_storeUserProperties: SAT afc_range set to 5000 instead of 4000 previously.
  This is to adapt the afc range to the new behavior of the FW, which is now returning 'no lock' as soon as the is above the selected afc_range.
  (The previous FW behavior lead to a 25% margin on afc_range, so 4000 corresponded to max 5000 in reality.)

 As from V0.0.7.0: <new_feature>[Properties/Traces] In Si2164B_L1_SetProperty: tracing property text in all cases, not only when it works.
  This makes it easier to identify properties generating errors.

 As from V0.0.6.0:
  <compatibility>[Duals/Si216x2] In Si2164B_downloadDDProperties: Setting 'dual' properties only for Si216x2 parts
  <new_feature>[TS/CLOCK] Adding Si2164B_DD_TS_FREQ_MAX_PROP

 As from V0.0.6.0:
  <new_feature>[ISDBT/MODE] Adding isdbt_mode.dl_config to Si2164B_ISDBT_MODE property

 As from V0.0.5.0:
  <improvement>[Code_size] Using the textBuffer in Si2164B_L1_Context when filling text strings
    In Si2164B_L1_SetProperty and Si2164B_L1_SetProperty2
  <improvement>[SAT/TONE] In Si2164B_storeUserProperties: Setting api->prop->dd_diseqc_freq.freq_hz to 0 to select 'envelop mode'
  <improvement> [TS_spurious/DUAL] In Si2164B_storeUserProperties: adapting parallel TS for no TS interference (from field experience):
      prop->dd_sec_ts_setup_par.ts_data_strength                             =     3;
      prop->dd_sec_ts_setup_par.ts_data_shape                                =     2;
      prop->dd_sec_ts_setup_par.ts_clk_strength                              =     3;
      prop->dd_sec_ts_setup_par.ts_clk_shape                                 =     2;

      prop->dd_ts_setup_par.ts_data_strength                                 =     3;
      prop->dd_ts_setup_par.ts_data_shape                                    =     2;
      prop->dd_ts_setup_par.ts_clk_strength                                  =     3;
      prop->dd_ts_setup_par.ts_clk_shape                                     =     2;

 As from V0.0.4.0:
  <improvement>[NOT_a_DUAL] In Si2164B_downloadDDProperties: skipping DD_SEC_TS property settings if demod is a single (to avoid raising API unnecessary errors).

 As from V0.0.3.0:
   <improvement>[TERACOM/BER] In Si2164B_storeUserProperties: adding caution message to warn the user that BER settings are
   overwritten at L3 Wrapper level
   <new_feature/DISEQC> Adding dd_diseqc_param.input_pin field
   <new_feature/TS_SERIAL/D7> Adding dd_ts_mode.serial_pin_selection, to allow routing serial TS on Dx (DO is used by default)
   <improvement/MCNS> mcns_symbol_rate.rate set by default at 5361, a MCNS-compatible SR.

 As from V0.0.0.3:
   <improvement> [TS_spurious/DUAL] In Si2164B_storeUserProperties:
      prop->dd_ts_setup_par.ts_data_strength                                 =     4;
      prop->dd_ts_setup_par.ts_data_shape                                    =     0;
      prop->dd_ts_setup_par.ts_clk_strength                                  =     4;
      prop->dd_ts_setup_par.ts_clk_shape                                     =     0;

      prop->dd_ts_setup_ser.ts_data_strength                                 =     7;
      prop->dd_ts_setup_ser.ts_data_shape                                    =     0;
      prop->dd_ts_setup_ser.ts_clk_strength                                  =     7;
      prop->dd_ts_setup_ser.ts_clk_shape                                     =     0;

   <improvement> [Teracom/BEST_MUX] In Si2164B_storeUserProperties: Changing BER resolution
    to match Teracom BEST_MUX test specification
     prop->dd_ber_resol.exp  = 5;
     prop->dd_ber_resol.mant = 8;

   <new_feature> [ISDB-T/LAYER] Adding ISDBT_MODE property to select the layer used
    for BER, CBER, PER, and UNCOR monitoring.

 As from V0.0.0.0:
  Initial version (based on Si2164 code V0.3.4)
****************************************************************************************/

#define   Si2164B_COMMAND_PROTOTYPES

/* Before including the headers, define SiLevel and SiTAG */
//#define   SiLEVEL          1

#include "Si2164B_Platform_Definition.h"

/* Re-definition of SiTRACE for Si2164B_PropObj */
#ifdef    SiTRACES
  #undef  SiTRACE
  #define SiTRACE(...)        SiTraceFunction(__FILE__, __LINE__, __FUNCTION__ ,__VA_ARGS__)
#endif /* SiTRACES */

/***********************************************************************************************************************
  Si2164B_storeUserProperties function
  Use:        property preparation function
              Used to fill the prop structure with user values.
  Parameter: *prop    a property structure to be filled

  Returns:    void
 ***********************************************************************************************************************/
void          Si2164B_storeUserProperties    (Si2164B_PropObj   *prop) {
#ifdef    Si2164B_MASTER_IEN_PROP
  prop->master_ien.ddien                                                 = Si2164B_MASTER_IEN_PROP_DDIEN_OFF   ; /* (default 'OFF') */
  prop->master_ien.scanien                                               = Si2164B_MASTER_IEN_PROP_SCANIEN_OFF ; /* (default 'OFF') */
  prop->master_ien.errien                                                = Si2164B_MASTER_IEN_PROP_ERRIEN_OFF  ; /* (default 'OFF') */
  prop->master_ien.ctsien                                                = Si2164B_MASTER_IEN_PROP_CTSIEN_OFF  ; /* (default 'OFF') */
#endif /* Si2164B_MASTER_IEN_PROP */

#ifdef    Si2164B_DD_BER_RESOL_PROP
  prop->dd_ber_resol.exp                                                 =     7; /* (default     7) */
  prop->dd_ber_resol.mant                                                =     1; /* (default     1) */
  /* CAUTION: to be able to pass TERACOM DVB-T and DVB-T2 certification,
  BER (exp;mant) values must be set to (6;1) for DVB-T, and (7;1) for all other standards
  This is done at L3 Wrapper level, so the above settings are overwritten inside SiLabs_API_Demod_status_selection
  when locked
  */
#endif /* Si2164B_DD_BER_RESOL_PROP */

#ifdef    Si2164B_DD_CBER_RESOL_PROP
  prop->dd_cber_resol.exp                                                =     5; /* (default     5) */
  prop->dd_cber_resol.mant                                               =     1; /* (default     1) */
#endif /* Si2164B_DD_CBER_RESOL_PROP */

#ifdef    Si2164B_DD_DISEQC_FREQ_PROP
  prop->dd_diseqc_freq.freq_hz                                           = 22000; /* (default 22000) */
#endif /* Si2164B_DD_DISEQC_FREQ_PROP */

#ifdef    Si2164B_DD_DISEQC_PARAM_PROP
  prop->dd_diseqc_param.sequence_mode                                    = Si2164B_DD_DISEQC_PARAM_PROP_SEQUENCE_MODE_GAP       ; /* (default 'GAP') */
  prop->dd_diseqc_param.input_pin                                        = Si2164B_DD_DISEQC_PARAM_PROP_INPUT_PIN_DISEQC_IN     ; /* (default 'DISEQC_IN') */
#endif /* Si2164B_DD_DISEQC_PARAM_PROP */

#ifdef    Si2164B_DD_FER_RESOL_PROP
  prop->dd_fer_resol.exp                                                 =     3; /* (default     3) */
  prop->dd_fer_resol.mant                                                =     1; /* (default     1) */
#endif /* Si2164B_DD_FER_RESOL_PROP */

#ifdef    Si2164B_DD_IEN_PROP
  prop->dd_ien.ien_bit0                                                  = Si2164B_DD_IEN_PROP_IEN_BIT0_DISABLE ; /* (default 'DISABLE') */
  prop->dd_ien.ien_bit1                                                  = Si2164B_DD_IEN_PROP_IEN_BIT1_DISABLE ; /* (default 'DISABLE') */
  prop->dd_ien.ien_bit2                                                  = Si2164B_DD_IEN_PROP_IEN_BIT2_DISABLE ; /* (default 'DISABLE') */
  prop->dd_ien.ien_bit3                                                  = Si2164B_DD_IEN_PROP_IEN_BIT3_DISABLE ; /* (default 'DISABLE') */
  prop->dd_ien.ien_bit4                                                  = Si2164B_DD_IEN_PROP_IEN_BIT4_DISABLE ; /* (default 'DISABLE') */
  prop->dd_ien.ien_bit5                                                  = Si2164B_DD_IEN_PROP_IEN_BIT5_DISABLE ; /* (default 'DISABLE') */
  prop->dd_ien.ien_bit6                                                  = Si2164B_DD_IEN_PROP_IEN_BIT6_DISABLE ; /* (default 'DISABLE') */
  prop->dd_ien.ien_bit7                                                  = Si2164B_DD_IEN_PROP_IEN_BIT7_DISABLE ; /* (default 'DISABLE') */
#endif /* Si2164B_DD_IEN_PROP */

#ifdef    Si2164B_DD_IF_INPUT_FREQ_PROP
  prop->dd_if_input_freq.offset                                          =  5000; /* (default  5000) */
#endif /* Si2164B_DD_IF_INPUT_FREQ_PROP */

#ifdef    Si2164B_DD_INT_SENSE_PROP
  prop->dd_int_sense.neg_bit0                                            = Si2164B_DD_INT_SENSE_PROP_NEG_BIT0_DISABLE ; /* (default 'DISABLE') */
  prop->dd_int_sense.neg_bit1                                            = Si2164B_DD_INT_SENSE_PROP_NEG_BIT1_DISABLE ; /* (default 'DISABLE') */
  prop->dd_int_sense.neg_bit2                                            = Si2164B_DD_INT_SENSE_PROP_NEG_BIT2_DISABLE ; /* (default 'DISABLE') */
  prop->dd_int_sense.neg_bit3                                            = Si2164B_DD_INT_SENSE_PROP_NEG_BIT3_DISABLE ; /* (default 'DISABLE') */
  prop->dd_int_sense.neg_bit4                                            = Si2164B_DD_INT_SENSE_PROP_NEG_BIT4_DISABLE ; /* (default 'DISABLE') */
  prop->dd_int_sense.neg_bit5                                            = Si2164B_DD_INT_SENSE_PROP_NEG_BIT5_DISABLE ; /* (default 'DISABLE') */
  prop->dd_int_sense.neg_bit6                                            = Si2164B_DD_INT_SENSE_PROP_NEG_BIT6_DISABLE ; /* (default 'DISABLE') */
  prop->dd_int_sense.neg_bit7                                            = Si2164B_DD_INT_SENSE_PROP_NEG_BIT7_DISABLE ; /* (default 'DISABLE') */
  prop->dd_int_sense.pos_bit0                                            = Si2164B_DD_INT_SENSE_PROP_POS_BIT0_DISABLE  ; /* (default 'ENABLE') */
  prop->dd_int_sense.pos_bit1                                            = Si2164B_DD_INT_SENSE_PROP_POS_BIT1_DISABLE  ; /* (default 'ENABLE') */
  prop->dd_int_sense.pos_bit2                                            = Si2164B_DD_INT_SENSE_PROP_POS_BIT2_DISABLE  ; /* (default 'ENABLE') */
  prop->dd_int_sense.pos_bit3                                            = Si2164B_DD_INT_SENSE_PROP_POS_BIT3_DISABLE  ; /* (default 'ENABLE') */
  prop->dd_int_sense.pos_bit4                                            = Si2164B_DD_INT_SENSE_PROP_POS_BIT4_DISABLE  ; /* (default 'ENABLE') */
  prop->dd_int_sense.pos_bit5                                            = Si2164B_DD_INT_SENSE_PROP_POS_BIT5_ENABLE  ; /* (default 'ENABLE') */
  prop->dd_int_sense.pos_bit6                                            = Si2164B_DD_INT_SENSE_PROP_POS_BIT6_DISABLE  ; /* (default 'ENABLE') */
  prop->dd_int_sense.pos_bit7                                            = Si2164B_DD_INT_SENSE_PROP_POS_BIT7_DISABLE  ; /* (default 'ENABLE') */
#endif /* Si2164B_DD_INT_SENSE_PROP */

#ifdef    Si2164B_DD_MODE_PROP
  prop->dd_mode.bw                                                       = Si2164B_DD_MODE_PROP_BW_BW_8MHZ              ; /* (default 'BW_8MHZ') */
  prop->dd_mode.modulation                                               = Si2164B_DD_MODE_PROP_MODULATION_DVBT         ; /* (default 'DVBT') */
  prop->dd_mode.invert_spectrum                                          = Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_NORMAL  ; /* (default 'NORMAL') */
  prop->dd_mode.auto_detect                                              = Si2164B_DD_MODE_PROP_AUTO_DETECT_NONE        ; /* (default 'NONE') */
#endif /* Si2164B_DD_MODE_PROP */

#ifdef    Si2164B_DD_PER_RESOL_PROP
  prop->dd_per_resol.exp                                                 =     5; /* (default     5) */
  prop->dd_per_resol.mant                                                =     1; /* (default     1) */
#endif /* Si2164B_DD_PER_RESOL_PROP */

#ifdef    Si2164B_DD_RSQ_BER_THRESHOLD_PROP
  prop->dd_rsq_ber_threshold.exp                                         =     1; /* (default     1) */
  prop->dd_rsq_ber_threshold.mant                                        =    10; /* (default    10) */
#endif /* Si2164B_DD_RSQ_BER_THRESHOLD_PROP */

#ifdef    Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP
  prop->dd_sec_ts_serial_diff.ts_data1_strength                          =    15; /* (default    15) */
  prop->dd_sec_ts_serial_diff.ts_data1_shape                             =     3; /* (default     3) */
  prop->dd_sec_ts_serial_diff.ts_data2_strength                          =    15; /* (default    15) */
  prop->dd_sec_ts_serial_diff.ts_data2_shape                             =     3; /* (default     3) */
  prop->dd_sec_ts_serial_diff.ts_clkb_on_data1                           = Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_DISABLE   ; /* (default 'DISABLE') */
  prop->dd_sec_ts_serial_diff.ts_data0b_on_data2                         = Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_DISABLE ; /* (default 'DISABLE') */
#endif /* Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP */

#ifdef    Si2164B_DD_SEC_TS_SETUP_PAR_PROP
  prop->dd_sec_ts_setup_par.ts_data_strength                             =     4; /* (default     3) */
  prop->dd_sec_ts_setup_par.ts_data_shape                                =     0; /* (default     1) */
  prop->dd_sec_ts_setup_par.ts_clk_strength                              =     4; /* (default     3) */
  prop->dd_sec_ts_setup_par.ts_clk_shape                                 =     0; /* (default     1) */
#endif /* Si2164B_DD_SEC_TS_SETUP_PAR_PROP */

#ifdef    Si2164B_DD_SEC_TS_SETUP_SER_PROP
  prop->dd_sec_ts_setup_ser.ts_data_strength                             =     15; /* (default     3) */
  prop->dd_sec_ts_setup_ser.ts_data_shape                                =     0; /* (default     3) */
  prop->dd_sec_ts_setup_ser.ts_clk_strength                              =     15; /* (default    15) */
  prop->dd_sec_ts_setup_ser.ts_clk_shape                                 =     0; /* (default     3) */
#endif /* Si2164B_DD_SEC_TS_SETUP_SER_PROP */

#ifdef    Si2164B_DD_SEC_TS_SLR_SERIAL_PROP
  prop->dd_sec_ts_slr_serial.ts_data_slr                                 =     0; /* (default     0) */
  prop->dd_sec_ts_slr_serial.ts_data_slr_on                              = Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_DISABLE  ; /* (default 'DISABLE') */
  prop->dd_sec_ts_slr_serial.ts_data1_slr                                =     0; /* (default     0) */
  prop->dd_sec_ts_slr_serial.ts_data1_slr_on                             = Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_DISABLE ; /* (default 'DISABLE') */
  prop->dd_sec_ts_slr_serial.ts_data2_slr                                =     0; /* (default     0) */
  prop->dd_sec_ts_slr_serial.ts_data2_slr_on                             = Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_DISABLE ; /* (default 'DISABLE') */
  prop->dd_sec_ts_slr_serial.ts_clk_slr                                  =     0; /* (default     0) */
  prop->dd_sec_ts_slr_serial.ts_clk_slr_on                               = Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_DISABLE   ; /* (default 'DISABLE') */
#endif /* Si2164B_DD_SEC_TS_SLR_SERIAL_PROP */

#ifdef    Si2164B_DD_SSI_SQI_PARAM_PROP
  prop->dd_ssi_sqi_param.sqi_average                                     =    30; /* (default     1) */
#endif /* Si2164B_DD_SSI_SQI_PARAM_PROP */

#ifdef    Si2164B_DD_TS_FREQ_PROP
  prop->dd_ts_freq.req_freq_10khz                                        =   720; /* (default   720) */
#endif /* Si2164B_DD_TS_FREQ_PROP */

#ifdef    Si2164B_DD_TS_FREQ_MAX_PROP
  prop->dd_ts_freq_max.req_freq_max_10khz                                =  14550; /* (default   14550) */
#endif /* Si2164B_DD_TS_FREQ_PROP */

#ifdef    Si2164B_DD_TS_MODE_PROP
  prop->dd_ts_mode.mode                                                  = Si2164B_DD_TS_MODE_PROP_MODE_TRISTATE                   ; /* (default 'TRISTATE') */
  prop->dd_ts_mode.clock                                                 = Si2164B_DD_TS_MODE_PROP_CLOCK_AUTO_ADAPT                ; /* (default 'AUTO_FIXED') */
  prop->dd_ts_mode.clk_gapped_en                                         = Si2164B_DD_TS_MODE_PROP_CLK_GAPPED_EN_DISABLED          ; /* (default 'DISABLED') */
  prop->dd_ts_mode.ts_err_polarity                                       = Si2164B_DD_TS_MODE_PROP_TS_ERR_POLARITY_NOT_INVERTED    ; /* (default 'NOT_INVERTED') */
  prop->dd_ts_mode.special                                               = Si2164B_DD_TS_MODE_PROP_SPECIAL_FULL_TS                 ; /* (default 'FULL_TS') */
  prop->dd_ts_mode.ts_freq_resolution                                    = Si2164B_DD_TS_MODE_PROP_TS_FREQ_RESOLUTION_NORMAL       ; /* (default 'NORMAL') */
  prop->dd_ts_mode.serial_pin_selection                                  = Si2164B_DD_TS_MODE_PROP_SERIAL_PIN_SELECTION_D0           ; /* (default 'D0') */
#endif /* Si2164B_DD_TS_MODE_PROP */

#ifdef    Si2164B_DD_TS_SERIAL_DIFF_PROP
  prop->dd_ts_serial_diff.ts_data1_strength                              =    15; /* (default    15) */
  prop->dd_ts_serial_diff.ts_data1_shape                                 =     3; /* (default     3) */
  prop->dd_ts_serial_diff.ts_data2_strength                              =    15; /* (default    15) */
  prop->dd_ts_serial_diff.ts_data2_shape                                 =     3; /* (default     3) */
  prop->dd_ts_serial_diff.ts_clkb_on_data1                               = Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_DISABLE   ; /* (default 'DISABLE') */
  prop->dd_ts_serial_diff.ts_data0b_on_data2                             = Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_DISABLE ; /* (default 'DISABLE') */
#endif /* Si2164B_DD_TS_SERIAL_DIFF_PROP */

#ifdef    Si2164B_DD_TS_SETUP_PAR_PROP
  prop->dd_ts_setup_par.ts_data_strength                                 =     4; /* (default     3) */
  prop->dd_ts_setup_par.ts_data_shape                                    =     0; /* (default     1) */
  prop->dd_ts_setup_par.ts_clk_strength                                  =     4; /* (default     3) */
  prop->dd_ts_setup_par.ts_clk_shape                                     =     0; /* (default     1) */
  prop->dd_ts_setup_par.ts_clk_invert                                    = Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_INVERT_INVERTED    ; /* (default 'INVERTED') */
  prop->dd_ts_setup_par.ts_clk_shift                                     =     0; /* (default     0) */
#endif /* Si2164B_DD_TS_SETUP_PAR_PROP */

#ifdef    Si2164B_DD_TS_SETUP_SER_PROP
  prop->dd_ts_setup_ser.ts_data_strength                                 =     10; /* (default    15) */  // 20150107 9-> 10
  prop->dd_ts_setup_ser.ts_data_shape                                    =     0; /* (default     3) */
  prop->dd_ts_setup_ser.ts_clk_strength                                  =     10; /* (default    15) */  // 20150107 9-> 10
  prop->dd_ts_setup_ser.ts_clk_shape                                     =     0; /* (default     3) */
  prop->dd_ts_setup_ser.ts_clk_invert                                    = Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_INVERT_INVERTED      ; /* (default 'INVERTED') */
  prop->dd_ts_setup_ser.ts_sync_duration                                 = Si2164B_DD_TS_SETUP_SER_PROP_TS_SYNC_DURATION_FIRST_BYTE ; /* (default 'FIRST_BYTE') */
  prop->dd_ts_setup_ser.ts_byte_order                                    = Si2164B_DD_TS_SETUP_SER_PROP_TS_BYTE_ORDER_MSB_FIRST     ; /* (default 'MSB_FIRST') */
#endif /* Si2164B_DD_TS_SETUP_SER_PROP */

#ifdef    Si2164B_DD_TS_SLR_SERIAL_PROP
  prop->dd_ts_slr_serial.ts_data_slr                                     =     1; /* (default     0) */
  prop->dd_ts_slr_serial.ts_data_slr_on                                  = 1  ; /* (default 'DISABLE') */
  prop->dd_ts_slr_serial.ts_data1_slr                                    =     1; /* (default     0) */
  prop->dd_ts_slr_serial.ts_data1_slr_on                                 = 1 ; /* (default 'DISABLE') */
  prop->dd_ts_slr_serial.ts_data2_slr                                    =     1; /* (default     0) */
  prop->dd_ts_slr_serial.ts_data2_slr_on                                 = 1 ; /* (default 'DISABLE') */
  prop->dd_ts_slr_serial.ts_clk_slr                                      =     1; /* (default     0) */
  prop->dd_ts_slr_serial.ts_clk_slr_on                                   = 1   ; /* (default 'DISABLE') */
#endif /* Si2164B_DD_TS_SLR_SERIAL_PROP */

#ifdef    Si2164B_DVBC_ADC_CREST_FACTOR_PROP
  prop->dvbc_adc_crest_factor.crest_factor                               =   112; /* (default   112) */
#endif /* Si2164B_DVBC_ADC_CREST_FACTOR_PROP */

#ifdef    Si2164B_DVBC_AFC_RANGE_PROP
  prop->dvbc_afc_range.range_khz                                         =   200; /* (default   100) */
#endif /* Si2164B_DVBC_AFC_RANGE_PROP */

#ifdef    Si2164B_DVBC_CONSTELLATION_PROP
  prop->dvbc_constellation.constellation                                 = Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_AUTO ; /* (default 'AUTO') */
#endif /* Si2164B_DVBC_CONSTELLATION_PROP */

#ifdef    Si2164B_DVBC_SYMBOL_RATE_PROP
  prop->dvbc_symbol_rate.rate                                            =  6900; /* (default  6900) */
#endif /* Si2164B_DVBC_SYMBOL_RATE_PROP */


#ifdef    Si2164B_DVBC2_ADC_CREST_FACTOR_PROP
  prop->dvbc2_adc_crest_factor.crest_factor                              =   130; /* (default   130) */
#endif /* Si2164B_DVBC2_ADC_CREST_FACTOR_PROP */

#ifdef    Si2164B_DVBC2_AFC_RANGE_PROP
  prop->dvbc2_afc_range.range_khz                                        =   550; /* (default   550) */
#endif /* Si2164B_DVBC2_AFC_RANGE_PROP */


#ifdef    Si2164B_DVBS_ADC_CREST_FACTOR_PROP
  prop->dvbs_adc_crest_factor.crest_factor                               =   104; /* (default   104) */
#endif /* Si2164B_DVBS_ADC_CREST_FACTOR_PROP */

#ifdef    Si2164B_DVBS_AFC_RANGE_PROP
  prop->dvbs_afc_range.range_khz                                         =  5000; /* (default  4000) */
#endif /* Si2164B_DVBS_AFC_RANGE_PROP */

#ifdef    Si2164B_DVBS_SYMBOL_RATE_PROP
  prop->dvbs_symbol_rate.rate                                            = 27500; /* (default 27500) */
#endif /* Si2164B_DVBS_SYMBOL_RATE_PROP */

#ifdef    Si2164B_DVBS2_ADC_CREST_FACTOR_PROP
  prop->dvbs2_adc_crest_factor.crest_factor                              =   104; /* (default   104) */
#endif /* Si2164B_DVBS2_ADC_CREST_FACTOR_PROP */

#ifdef    Si2164B_DVBS2_AFC_RANGE_PROP
  prop->dvbs2_afc_range.range_khz                                        =  5000; /* (default  4000) */
#endif /* Si2164B_DVBS2_AFC_RANGE_PROP */

#ifdef    Si2164B_DVBS2_SYMBOL_RATE_PROP
  prop->dvbs2_symbol_rate.rate                                           = 27500; /* (default 27500) */
#endif /* Si2164B_DVBS2_SYMBOL_RATE_PROP */


#ifdef    Si2164B_DVBT_ADC_CREST_FACTOR_PROP
  prop->dvbt_adc_crest_factor.crest_factor                               =   130; /* (default   130) */
#endif /* Si2164B_DVBT_ADC_CREST_FACTOR_PROP */

#ifdef    Si2164B_DVBT_AFC_RANGE_PROP
  prop->dvbt_afc_range.range_khz                                         =   550; /* (default   550) */
#endif /* Si2164B_DVBT_AFC_RANGE_PROP */

#ifdef    Si2164B_DVBT_HIERARCHY_PROP
  prop->dvbt_hierarchy.stream                                            = Si2164B_DVBT_HIERARCHY_PROP_STREAM_HP ; /* (default 'HP') */
#endif /* Si2164B_DVBT_HIERARCHY_PROP */


#ifdef    Si2164B_DVBT2_ADC_CREST_FACTOR_PROP
  prop->dvbt2_adc_crest_factor.crest_factor                              =   130; /* (default   130) */
#endif /* Si2164B_DVBT2_ADC_CREST_FACTOR_PROP */

#ifdef    Si2164B_DVBT2_AFC_RANGE_PROP
  prop->dvbt2_afc_range.range_khz                                        =   550; /* (default   550) */
#endif /* Si2164B_DVBT2_AFC_RANGE_PROP */

#ifdef    Si2164B_DVBT2_FEF_TUNER_PROP
  prop->dvbt2_fef_tuner.tuner_delay                                      =     1; /* (default     1) */
  prop->dvbt2_fef_tuner.tuner_freeze_time                                =     1; /* (default     1) */
  prop->dvbt2_fef_tuner.tuner_unfreeze_time                              =     1; /* (default     1) */
#endif /* Si2164B_DVBT2_FEF_TUNER_PROP */

#ifdef    Si2164B_DVBT2_MODE_PROP
  prop->dvbt2_mode.lock_mode                                             = Si2164B_DVBT2_MODE_PROP_LOCK_MODE_ANY ; /* (default 'ANY') */
#endif /* Si2164B_DVBT2_MODE_PROP */



#ifdef    Si2164B_MCNS_ADC_CREST_FACTOR_PROP
  prop->mcns_adc_crest_factor.crest_factor                               =   112; /* (default   112) */
#endif /* Si2164B_MCNS_ADC_CREST_FACTOR_PROP */

#ifdef    Si2164B_MCNS_AFC_RANGE_PROP
  prop->mcns_afc_range.range_khz                                         =   100; /* (default   100) */
#endif /* Si2164B_MCNS_AFC_RANGE_PROP */

#ifdef    Si2164B_MCNS_CONSTELLATION_PROP
  prop->mcns_constellation.constellation                                 = Si2164B_MCNS_CONSTELLATION_PROP_CONSTELLATION_AUTO ; /* (default 'AUTO') */
#endif /* Si2164B_MCNS_CONSTELLATION_PROP */

#ifdef    Si2164B_MCNS_SYMBOL_RATE_PROP
  prop->mcns_symbol_rate.rate                                            =  5361; /* (default  5361) */
#endif /* Si2164B_MCNS_SYMBOL_RATE_PROP */


#ifdef    Si2164B_SCAN_FMAX_PROP
  prop->scan_fmax.scan_fmax                                              =     0; /* (default     0) */
#endif /* Si2164B_SCAN_FMAX_PROP */

#ifdef    Si2164B_SCAN_FMIN_PROP
  prop->scan_fmin.scan_fmin                                              =     0; /* (default     0) */
#endif /* Si2164B_SCAN_FMIN_PROP */

#ifdef    Si2164B_SCAN_IEN_PROP
  prop->scan_ien.buzien                                                  = Si2164B_SCAN_IEN_PROP_BUZIEN_DISABLE ; /* (default 'DISABLE') */
  prop->scan_ien.reqien                                                  = Si2164B_SCAN_IEN_PROP_REQIEN_DISABLE ; /* (default 'DISABLE') */
#endif /* Si2164B_SCAN_IEN_PROP */

#ifdef    Si2164B_SCAN_INT_SENSE_PROP
  prop->scan_int_sense.buznegen                                          = Si2164B_SCAN_INT_SENSE_PROP_BUZNEGEN_ENABLE  ; /* (default 'ENABLE') */
  prop->scan_int_sense.reqnegen                                          = Si2164B_SCAN_INT_SENSE_PROP_REQNEGEN_DISABLE ; /* (default 'DISABLE') */
  prop->scan_int_sense.buzposen                                          = Si2164B_SCAN_INT_SENSE_PROP_BUZPOSEN_DISABLE ; /* (default 'DISABLE') */
  prop->scan_int_sense.reqposen                                          = Si2164B_SCAN_INT_SENSE_PROP_REQPOSEN_ENABLE  ; /* (default 'ENABLE') */
#endif /* Si2164B_SCAN_INT_SENSE_PROP */

#ifdef    Si2164B_SCAN_SAT_CONFIG_PROP
  prop->scan_sat_config.analog_detect                                    = Si2164B_SCAN_SAT_CONFIG_PROP_ANALOG_DETECT_DISABLED ; /* (default 'DISABLED') */
  prop->scan_sat_config.reserved1                                        =     0; /* (default     0) */
  prop->scan_sat_config.reserved2                                        =    12; /* (default    12) */
#endif /* Si2164B_SCAN_SAT_CONFIG_PROP */

#ifdef    Si2164B_SCAN_SAT_UNICABLE_BW_PROP
  prop->scan_sat_unicable_bw.scan_sat_unicable_bw                        =     0; /* (default     0) */
#endif /* Si2164B_SCAN_SAT_UNICABLE_BW_PROP */

#ifdef    Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP
  prop->scan_sat_unicable_min_tune_step.scan_sat_unicable_min_tune_step  =    50; /* (default    50) */
#endif /* Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP */

#ifdef    Si2164B_SCAN_SYMB_RATE_MAX_PROP
  prop->scan_symb_rate_max.scan_symb_rate_max                            =     0; /* (default     0) */
#endif /* Si2164B_SCAN_SYMB_RATE_MAX_PROP */

#ifdef    Si2164B_SCAN_SYMB_RATE_MIN_PROP
  prop->scan_symb_rate_min.scan_symb_rate_min                            =     0; /* (default     0) */
#endif /* Si2164B_SCAN_SYMB_RATE_MIN_PROP */

#ifdef    Si2164B_SCAN_TER_CONFIG_PROP
  prop->scan_ter_config.mode                                             = Si2164B_SCAN_TER_CONFIG_PROP_MODE_BLIND_SCAN          ; /* (default 'BLIND_SCAN') */
  prop->scan_ter_config.analog_bw                                        = Si2164B_SCAN_TER_CONFIG_PROP_ANALOG_BW_8MHZ           ; /* (default '8MHZ') */
  prop->scan_ter_config.search_analog                                    = Si2164B_SCAN_TER_CONFIG_PROP_SEARCH_ANALOG_DISABLE    ; /* (default 'DISABLE') */
#endif /* Si2164B_SCAN_TER_CONFIG_PROP */

}
/* Re-definition of SiTRACE for L1_Si2164B_Context */
#ifdef    SiTRACES
  #undef  SiTRACE
  #define SiTRACE(...)        SiTraceFunction(__FILE__, __LINE__, __FUNCTION__ ,__VA_ARGS__)
#endif /* SiTRACES */
/***********************************************************************************************************************
  Si2164B_L1_SetProperty function
  Use:        property set function
              Used to call L1_SET_PROPERTY with the property Id and data provided.
  Parameter: *api     the Si2164B context
  Parameter: prop     the property Id
  Parameter: data     the property bytes
  Behavior:  This function will only download the property if required.
               Conditions to download the property are:
                - The property changes
                - The propertyWriteMode is set to Si2164B_DOWNLOAD_ALWAYS
                - The property is unknown to Si2164B_PackProperty (this may be useful for debug purpose)
  Returns:    0 if no error, an error code otherwise
 ***********************************************************************************************************************/
unsigned char Si2164B_L1_SetProperty         (L1_Si2164B_Context *api, unsigned int prop_code, signed int  data) {
#ifdef    Si2164B_GET_PROPERTY_STRING
    unsigned char *msg;
    signed int            shadowData;
#endif /* Si2164B_GET_PROPERTY_STRING */
    unsigned char  res;
    unsigned char  reserved;

    reserved          = 0;

    res = Si2164B_PackProperty(api->propShadow, prop_code, &shadowData);

    /* -- Download property only if required --     */
    if ( ( (data != shadowData)  || (api->propertyWriteMode == Si2164B_DOWNLOAD_ALWAYS) ) & ( res != ERROR_Si2164B_UNKNOWN_PROPERTY ) ) {
      SiTRACE("Si2164B_L1_SetProperty: Setting Property 0x%04x to 0x%04x(%d)\n", prop_code,data,data);
      res = Si2164B_L1_SET_PROPERTY (api, reserved, prop_code, data);
#ifdef    Si2164B_GET_PROPERTY_STRING
		msg = malloc(sizeof(unsigned char)*1000);
		if(msg != 0)
		{
        	Si2164B_L1_PropertyText(api->prop, prop_code, (unsigned char*)" ", (unsigned char*)msg);
        	SiTRACE("%s\n",msg);            
            free(msg);
		}
		else
        {      
			SiTRACE("Si2164B_L1_SetProperty : fail to create buffer to print log.\n");
        }    
#endif /* Si2164B_GET_PROPERTY_STRING */
      if (res != NO_Si2164B_ERROR) {
        SiTRACE("\nERROR: Si2164B_L1_SetProperty: %s 0x%04x!\n\n", Si2164B_L1_API_ERROR_TEXT(res), prop_code);
      } else {
        /* Update propShadow with latest data if correctly set */
        Si2164B_UnpackProperty   (api->propShadow, prop_code, data);
      }
    }
  return res;
}
/***********************************************************************************************************************
  Si2164B_L1_GetProperty function
  Use:        property get function
              Used to call L1_GET_PROPERTY with the property Id provided.
  Parameter: *api     the Si2164B context
  Parameter: prop     the property Id
  Parameter: *data    a buffer to store the property bytes into
  Returns:    0 if no error, an error code otherwise
 ***********************************************************************************************************************/
unsigned char Si2164B_L1_GetProperty         (L1_Si2164B_Context *api, unsigned int prop_code, signed int *data) {
    unsigned char  reserved          = 0;
    unsigned char res;
    res = Si2164B_L1_GET_PROPERTY (api, reserved, prop_code);
    *data = api->rsp->get_property.data;
    return res;
}
 /***********************************************************************************************************************
  Si2164B_L1_SetProperty2 function
  Use:        Sets the property given the property code.
  Parameter: *api     the Si2164B context
  Parameter: prop     the property Id

  Returns:    NO_Si2164B_ERROR if successful.
 ***********************************************************************************************************************/
unsigned char Si2164B_L1_SetProperty2        (L1_Si2164B_Context *api, unsigned int prop_code) {
    signed int data, res;
    unsigned char *msg;
    res = Si2164B_PackProperty(api->prop, prop_code, &data);
    if (res != NO_Si2164B_ERROR) {
	  msg = malloc(sizeof(unsigned char)*1000);
	  if(msg != 0)
	  {
      	safe_sprintf(msg, "\nSi2164B_L1_SetProperty2: %s 0x%04x!\n\n", (char*)Si2164B_L1_API_ERROR_TEXT(res), prop_code);
      	SiTRACE(msg);
      	SiERROR((char*)msg);
		free(msg);
	  }
	  else
     {   
	  	SiTRACE("Si2164B_L1_SetProperty2 : fail to create buffer to print log.\n");
      }
      return res;
    }
    return Si2164B_L1_SetProperty (api, prop_code & 0xffff, data);
  }
 /***********************************************************************************************************************
  Si2164B_L1_GetProperty2 function
  Use:        property get function
              Used to call L1_GET_PROPERTY with the property Id provided.
  Parameter: *api     the Si2164B context
  Parameter: prop     the property Id

  Returns:    NO_Si2164B_ERROR if successful.
 ***********************************************************************************************************************/
unsigned char Si2164B_L1_GetProperty2        (L1_Si2164B_Context *api, unsigned int prop_code) {
  signed int data, res;
  res = Si2164B_L1_GetProperty(api, prop_code & 0xffff, &data);
  if (res != NO_Si2164B_ERROR) {
    SiTRACE("Si2164B_L1_GetProperty2: %s 0x%04x\n", Si2164B_L1_API_ERROR_TEXT(res), prop_code);
    SiERROR((char*)Si2164B_L1_API_ERROR_TEXT(res));
    return res;
  }
  return Si2164B_UnpackProperty(api->prop, prop_code, data);
}
/*****************************************************************************************
 NAME: Si2164B_downloadCOMMONProperties
  DESCRIPTION: Setup Si2164B COMMON properties configuration
  This function will download all the COMMON configuration properties.
  The function Si2164B_storeUserProperties should be called before the first call to this function.
  Parameter:  Pointer to Si2164B Context
  Returns:    I2C transaction error code, NO_Si2164B_ERROR if successful
  Programming Guide Reference:    COMMON setup flowchart
******************************************************************************************/
signed int  Si2164B_downloadCOMMONProperties      (L1_Si2164B_Context *api) {
  SiTRACE("Si2164B_downloadCOMMONProperties\n");
#ifdef    Si2164B_MASTER_IEN_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_MASTER_IEN_PROP_CODE                     ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_MASTER_IEN_PROP */
return NO_Si2164B_ERROR;
}
/*****************************************************************************************
 NAME: Si2164B_downloadDDProperties
  DESCRIPTION: Setup Si2164B DD properties configuration
  This function will download all the DD configuration properties.
  The function Si2164B_storeUserProperties should be called before the first call to this function.
  Parameter:  Pointer to Si2164B Context
  Returns:    I2C transaction error code, NO_Si2164B_ERROR if successful
  Programming Guide Reference:    DD setup flowchart
******************************************************************************************/
signed int  Si2164B_downloadDDProperties          (L1_Si2164B_Context *api) {
  SiTRACE("Si2164B_downloadDDProperties\n");
#ifdef    Si2164B_DD_BER_RESOL_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_BER_RESOL_PROP_CODE                   ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_BER_RESOL_PROP */
#ifdef    Si2164B_DD_CBER_RESOL_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_CBER_RESOL_PROP_CODE                  ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_CBER_RESOL_PROP */
#ifdef    Si2164B_DD_DISEQC_FREQ_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_DISEQC_FREQ_PROP_CODE                 ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_DISEQC_FREQ_PROP */
#ifdef    Si2164B_DD_DISEQC_PARAM_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_DISEQC_PARAM_PROP_CODE                ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_DISEQC_PARAM_PROP */

#ifdef    Si2164B_DD_FER_RESOL_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_FER_RESOL_PROP_CODE                   ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_FER_RESOL_PROP */
#ifdef    Si2164B_DD_IEN_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_IEN_PROP_CODE                         ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_IEN_PROP */
#ifdef    Si2164B_DD_IF_INPUT_FREQ_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_IF_INPUT_FREQ_PROP_CODE               ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_IF_INPUT_FREQ_PROP */
#ifdef    Si2164B_DD_INT_SENSE_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_INT_SENSE_PROP_CODE                   ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_INT_SENSE_PROP */
#ifdef    Si2164B_DD_MODE_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_MODE_PROP_CODE                        ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_MODE_PROP */
#ifdef    Si2164B_DD_PER_RESOL_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_PER_RESOL_PROP_CODE                   ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_PER_RESOL_PROP */
#ifdef    Si2164B_DD_RSQ_BER_THRESHOLD_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_RSQ_BER_THRESHOLD_PROP_CODE           ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_RSQ_BER_THRESHOLD_PROP */
  if (api->rsp->get_rev.mcm_die != Si2164B_GET_REV_RESPONSE_MCM_DIE_SINGLE) { /* Only for Si216x2 parts */
#ifdef    Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_CODE          ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP */
#ifdef    Si2164B_DD_SEC_TS_SETUP_PAR_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_SEC_TS_SETUP_PAR_PROP_CODE            ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_SEC_TS_SETUP_PAR_PROP */
#ifdef    Si2164B_DD_SEC_TS_SETUP_SER_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_SEC_TS_SETUP_SER_PROP_CODE            ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_SEC_TS_SETUP_SER_PROP */
#ifdef    Si2164B_DD_SEC_TS_SLR_SERIAL_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_CODE           ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_SEC_TS_SLR_SERIAL_PROP */
  }
#ifdef    Si2164B_DD_SSI_SQI_PARAM_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_SSI_SQI_PARAM_PROP_CODE               ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_SSI_SQI_PARAM_PROP */
#ifdef    Si2164B_DD_TS_FREQ_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_TS_FREQ_PROP_CODE                     ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_TS_FREQ_PROP */
#ifdef    Si2164B_DD_TS_FREQ_MAX_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_TS_FREQ_MAX_PROP_CODE                 ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_TS_FREQ_MAX_PROP */
#ifdef    Si2164B_DD_TS_MODE_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_TS_MODE_PROP_CODE                     ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_TS_MODE_PROP */
#ifdef    Si2164B_DD_TS_SERIAL_DIFF_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_TS_SERIAL_DIFF_PROP_CODE              ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_TS_SERIAL_DIFF_PROP */
#ifdef    Si2164B_DD_TS_SETUP_PAR_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_TS_SETUP_PAR_PROP_CODE                ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_TS_SETUP_PAR_PROP */
#ifdef    Si2164B_DD_TS_SETUP_SER_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_TS_SETUP_SER_PROP_CODE                ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_TS_SETUP_SER_PROP */
#ifdef    Si2164B_DD_TS_SLR_SERIAL_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DD_TS_SLR_SERIAL_PROP_CODE               ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DD_TS_SLR_SERIAL_PROP */
return NO_Si2164B_ERROR;
}
/*****************************************************************************************
 NAME: Si2164B_downloadDVBCProperties
  DESCRIPTION: Setup Si2164B DVBC properties configuration
  This function will download all the DVBC configuration properties.
  The function Si2164B_storeUserProperties should be called before the first call to this function.
  Parameter:  Pointer to Si2164B Context
  Returns:    I2C transaction error code, NO_Si2164B_ERROR if successful
  Programming Guide Reference:    DVBC setup flowchart
******************************************************************************************/
signed int  Si2164B_downloadDVBCProperties        (L1_Si2164B_Context *api) {
  SiTRACE("Si2164B_downloadDVBCProperties\n");
#ifdef    Si2164B_DVBC_ADC_CREST_FACTOR_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBC_ADC_CREST_FACTOR_PROP_CODE          ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBC_ADC_CREST_FACTOR_PROP */
#ifdef    Si2164B_DVBC_AFC_RANGE_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBC_AFC_RANGE_PROP_CODE                 ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBC_AFC_RANGE_PROP */
#ifdef    Si2164B_DVBC_CONSTELLATION_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBC_CONSTELLATION_PROP_CODE             ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBC_CONSTELLATION_PROP */
#ifdef    Si2164B_DVBC_SYMBOL_RATE_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBC_SYMBOL_RATE_PROP_CODE               ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBC_SYMBOL_RATE_PROP */
return NO_Si2164B_ERROR;
}

/*****************************************************************************************
 NAME: Si2164B_downloadDVBC2Properties
  DESCRIPTION: Setup Si2164B DVBC2 properties configuration
  This function will download all the DVBC2 configuration properties.
  The function Si2164B_storeUserProperties should be called before the first call to this function.
  Parameter:  Pointer to Si2164B Context
  Returns:    I2C transaction error code, NO_Si2164B_ERROR if successful
  Programming Guide Reference:    DVBC2 setup flowchart
******************************************************************************************/
signed int  Si2164B_downloadDVBC2Properties       (L1_Si2164B_Context *api) {
  SiTRACE("Si2164B_downloadDVBC2Properties\n");
#ifdef    Si2164B_DVBC2_ADC_CREST_FACTOR_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBC2_ADC_CREST_FACTOR_PROP_CODE         ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBC2_ADC_CREST_FACTOR_PROP */
#ifdef    Si2164B_DVBC2_AFC_RANGE_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBC2_AFC_RANGE_PROP_CODE                ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBC2_AFC_RANGE_PROP */
return NO_Si2164B_ERROR;
}

/*****************************************************************************************
 NAME: Si2164B_downloadDVBSProperties
  DESCRIPTION: Setup Si2164B DVBS properties configuration
  This function will download all the DVBS configuration properties.
  The function Si2164B_storeUserProperties should be called before the first call to this function.
  Parameter:  Pointer to Si2164B Context
  Returns:    I2C transaction error code, NO_Si2164B_ERROR if successful
  Programming Guide Reference:    DVBS setup flowchart
******************************************************************************************/
signed int  Si2164B_downloadDVBSProperties        (L1_Si2164B_Context *api) {
  SiTRACE("Si2164B_downloadDVBSProperties\n");
#ifdef    Si2164B_DVBS_ADC_CREST_FACTOR_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBS_ADC_CREST_FACTOR_PROP_CODE          ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBS_ADC_CREST_FACTOR_PROP */
#ifdef    Si2164B_DVBS_AFC_RANGE_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBS_AFC_RANGE_PROP_CODE                 ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBS_AFC_RANGE_PROP */
#ifdef    Si2164B_DVBS_SYMBOL_RATE_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBS_SYMBOL_RATE_PROP_CODE               ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBS_SYMBOL_RATE_PROP */
return NO_Si2164B_ERROR;
}
/*****************************************************************************************
 NAME: Si2164B_downloadDVBS2Properties
  DESCRIPTION: Setup Si2164B DVBS2 properties configuration
  This function will download all the DVBS2 configuration properties.
  The function Si2164B_storeUserProperties should be called before the first call to this function.
  Parameter:  Pointer to Si2164B Context
  Returns:    I2C transaction error code, NO_Si2164B_ERROR if successful
  Programming Guide Reference:    DVBS2 setup flowchart
******************************************************************************************/
signed int  Si2164B_downloadDVBS2Properties       (L1_Si2164B_Context *api) {
  SiTRACE("Si2164B_downloadDVBS2Properties\n");
#ifdef    Si2164B_DVBS2_ADC_CREST_FACTOR_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBS2_ADC_CREST_FACTOR_PROP_CODE         ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBS2_ADC_CREST_FACTOR_PROP */
#ifdef    Si2164B_DVBS2_AFC_RANGE_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBS2_AFC_RANGE_PROP_CODE                ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBS2_AFC_RANGE_PROP */
#ifdef    Si2164B_DVBS2_SYMBOL_RATE_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBS2_SYMBOL_RATE_PROP_CODE              ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBS2_SYMBOL_RATE_PROP */
return NO_Si2164B_ERROR;
}

/*****************************************************************************************
 NAME: Si2164B_downloadDVBTProperties
  DESCRIPTION: Setup Si2164B DVBT properties configuration
  This function will download all the DVBT configuration properties.
  The function Si2164B_storeUserProperties should be called before the first call to this function.
  Parameter:  Pointer to Si2164B Context
  Returns:    I2C transaction error code, NO_Si2164B_ERROR if successful
  Programming Guide Reference:    DVBT setup flowchart
******************************************************************************************/
signed int  Si2164B_downloadDVBTProperties        (L1_Si2164B_Context *api) {
  SiTRACE("Si2164B_downloadDVBTProperties\n");
#ifdef    Si2164B_DVBT_ADC_CREST_FACTOR_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBT_ADC_CREST_FACTOR_PROP_CODE          ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBT_ADC_CREST_FACTOR_PROP */
#ifdef    Si2164B_DVBT_AFC_RANGE_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBT_AFC_RANGE_PROP_CODE                 ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBT_AFC_RANGE_PROP */
#ifdef    Si2164B_DVBT_HIERARCHY_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBT_HIERARCHY_PROP_CODE                 ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBT_HIERARCHY_PROP */
return NO_Si2164B_ERROR;
}

/*****************************************************************************************
 NAME: Si2164B_downloadDVBT2Properties
  DESCRIPTION: Setup Si2164B DVBT2 properties configuration
  This function will download all the DVBT2 configuration properties.
  The function Si2164B_storeUserProperties should be called before the first call to this function.
  Parameter:  Pointer to Si2164B Context
  Returns:    I2C transaction error code, NO_Si2164B_ERROR if successful
  Programming Guide Reference:    DVBT2 setup flowchart
******************************************************************************************/
signed int  Si2164B_downloadDVBT2Properties       (L1_Si2164B_Context *api) {
  SiTRACE("Si2164B_downloadDVBT2Properties\n");
#ifdef    Si2164B_DVBT2_ADC_CREST_FACTOR_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBT2_ADC_CREST_FACTOR_PROP_CODE         ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBT2_ADC_CREST_FACTOR_PROP */
#ifdef    Si2164B_DVBT2_AFC_RANGE_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBT2_AFC_RANGE_PROP_CODE                ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBT2_AFC_RANGE_PROP */
#ifdef    Si2164B_DVBT2_FEF_TUNER_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBT2_FEF_TUNER_PROP_CODE                ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBT2_FEF_TUNER_PROP */
#ifdef    Si2164B_DVBT2_MODE_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_DVBT2_MODE_PROP_CODE                     ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_DVBT2_MODE_PROP */
return NO_Si2164B_ERROR;
}


/*****************************************************************************************
 NAME: Si2164B_downloadMCNSProperties
  DESCRIPTION: Setup Si2164B MCNS properties configuration
  This function will download all the MCNS configuration properties.
  The function Si2164B_storeUserProperties should be called before the first call to this function.
  Parameter:  Pointer to Si2164B Context
  Returns:    I2C transaction error code, NO_Si2164B_ERROR if successful
  Programming Guide Reference:    MCNS setup flowchart
******************************************************************************************/
signed int  Si2164B_downloadMCNSProperties        (L1_Si2164B_Context *api) {
  SiTRACE("Si2164B_downloadMCNSProperties\n");
#ifdef    Si2164B_MCNS_ADC_CREST_FACTOR_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_MCNS_ADC_CREST_FACTOR_PROP_CODE          ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_MCNS_ADC_CREST_FACTOR_PROP */
#ifdef    Si2164B_MCNS_AFC_RANGE_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_MCNS_AFC_RANGE_PROP_CODE                 ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_MCNS_AFC_RANGE_PROP */
#ifdef    Si2164B_MCNS_CONSTELLATION_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_MCNS_CONSTELLATION_PROP_CODE             ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_MCNS_CONSTELLATION_PROP */
#ifdef    Si2164B_MCNS_SYMBOL_RATE_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_MCNS_SYMBOL_RATE_PROP_CODE               ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_MCNS_SYMBOL_RATE_PROP */
return NO_Si2164B_ERROR;
}

/*****************************************************************************************
 NAME: Si2164B_downloadSCANProperties
  DESCRIPTION: Setup Si2164B SCAN properties configuration
  This function will download all the SCAN configuration properties.
  The function Si2164B_storeUserProperties should be called before the first call to this function.
  Parameter:  Pointer to Si2164B Context
  Returns:    I2C transaction error code, NO_Si2164B_ERROR if successful
  Programming Guide Reference:    SCAN setup flowchart
******************************************************************************************/
signed int  Si2164B_downloadSCANProperties        (L1_Si2164B_Context *api) {
  SiTRACE("Si2164B_downloadSCANProperties\n");
#ifdef    Si2164B_SCAN_FMAX_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_SCAN_FMAX_PROP_CODE                      ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_SCAN_FMAX_PROP */
#ifdef    Si2164B_SCAN_FMIN_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_SCAN_FMIN_PROP_CODE                      ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_SCAN_FMIN_PROP */
#ifdef    Si2164B_SCAN_IEN_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_SCAN_IEN_PROP_CODE                       ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_SCAN_IEN_PROP */
#ifdef    Si2164B_SCAN_INT_SENSE_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_SCAN_INT_SENSE_PROP_CODE                 ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_SCAN_INT_SENSE_PROP */
#ifdef    Si2164B_SCAN_SAT_CONFIG_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_SCAN_SAT_CONFIG_PROP_CODE                ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_SCAN_SAT_CONFIG_PROP */
#ifdef    Si2164B_SCAN_SAT_UNICABLE_BW_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_SCAN_SAT_UNICABLE_BW_PROP_CODE           ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_SCAN_SAT_UNICABLE_BW_PROP */
#ifdef    Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP_CODE) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP */

#ifdef    Si2164B_SCAN_SYMB_RATE_MAX_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_SCAN_SYMB_RATE_MAX_PROP_CODE             ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_SCAN_SYMB_RATE_MAX_PROP */
#ifdef    Si2164B_SCAN_SYMB_RATE_MIN_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_SCAN_SYMB_RATE_MIN_PROP_CODE             ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_SCAN_SYMB_RATE_MIN_PROP */
#ifdef    Si2164B_SCAN_TER_CONFIG_PROP
  if (Si2164B_L1_SetProperty2(api, Si2164B_SCAN_TER_CONFIG_PROP_CODE                ) != NO_Si2164B_ERROR) {return ERROR_Si2164B_SENDING_COMMAND;}
#endif /* Si2164B_SCAN_TER_CONFIG_PROP */
return NO_Si2164B_ERROR;
}

signed int  Si2164B_downloadAllProperties         (L1_Si2164B_Context *api) {
  Si2164B_downloadCOMMONProperties      (api);
  Si2164B_downloadDDProperties          (api);
  Si2164B_downloadDVBCProperties        (api);

  Si2164B_downloadDVBC2Properties       (api);

  Si2164B_downloadDVBSProperties        (api);
  Si2164B_downloadDVBS2Properties       (api);

  Si2164B_downloadDVBTProperties        (api);

  Si2164B_downloadDVBT2Properties       (api);


  Si2164B_downloadMCNSProperties        (api);

  Si2164B_downloadSCANProperties        (api);
  return 0;
}

/* Re-definition of SiTRACE for Si2164B_PropObj */
#ifdef    SiTRACES
  #undef  SiTRACE
  #define SiTRACE(...)        SiTraceFunction(__FILE__, __LINE__, __FUNCTION__ ,__VA_ARGS__)
#endif /* SiTRACES */

/***********************************************************************************************************************
  Si2164B_PackProperty function
  Use:        This function will pack all the members of a property into an integer for the SetProperty function.

  Parameter: *prop          the Si2164B property context
  Parameter:  prop_code     the property Id
  Parameter:  *data         an signed int to store the property data

  Returns:    NO_Si2164B_ERROR if the property exists.
 ***********************************************************************************************************************/
unsigned char Si2164B_PackProperty            (Si2164B_PropObj   *prop, unsigned int prop_code, signed int *data) {
    switch (prop_code) {
    #ifdef        Si2164B_DD_BER_RESOL_PROP
     case         Si2164B_DD_BER_RESOL_PROP_CODE:
      *data = (prop->dd_ber_resol.exp  & Si2164B_DD_BER_RESOL_PROP_EXP_MASK ) << Si2164B_DD_BER_RESOL_PROP_EXP_LSB  |
              (prop->dd_ber_resol.mant & Si2164B_DD_BER_RESOL_PROP_MANT_MASK) << Si2164B_DD_BER_RESOL_PROP_MANT_LSB ;
     break;
    #endif /*     Si2164B_DD_BER_RESOL_PROP */
    #ifdef        Si2164B_DD_CBER_RESOL_PROP
     case         Si2164B_DD_CBER_RESOL_PROP_CODE:
      *data = (prop->dd_cber_resol.exp  & Si2164B_DD_CBER_RESOL_PROP_EXP_MASK ) << Si2164B_DD_CBER_RESOL_PROP_EXP_LSB  |
              (prop->dd_cber_resol.mant & Si2164B_DD_CBER_RESOL_PROP_MANT_MASK) << Si2164B_DD_CBER_RESOL_PROP_MANT_LSB ;
     break;
    #endif /*     Si2164B_DD_CBER_RESOL_PROP */
    #ifdef        Si2164B_DD_DISEQC_FREQ_PROP
     case         Si2164B_DD_DISEQC_FREQ_PROP_CODE:
      *data = (prop->dd_diseqc_freq.freq_hz & Si2164B_DD_DISEQC_FREQ_PROP_FREQ_HZ_MASK) << Si2164B_DD_DISEQC_FREQ_PROP_FREQ_HZ_LSB ;
     break;
    #endif /*     Si2164B_DD_DISEQC_FREQ_PROP */
    #ifdef        Si2164B_DD_DISEQC_PARAM_PROP
     case         Si2164B_DD_DISEQC_PARAM_PROP_CODE:
      *data = (prop->dd_diseqc_param.sequence_mode & Si2164B_DD_DISEQC_PARAM_PROP_SEQUENCE_MODE_MASK) << Si2164B_DD_DISEQC_PARAM_PROP_SEQUENCE_MODE_LSB  |
              (prop->dd_diseqc_param.input_pin     & Si2164B_DD_DISEQC_PARAM_PROP_INPUT_PIN_MASK    ) << Si2164B_DD_DISEQC_PARAM_PROP_INPUT_PIN_LSB ;
     break;
    #endif /*     Si2164B_DD_DISEQC_PARAM_PROP */

    #ifdef        Si2164B_DD_FER_RESOL_PROP
     case         Si2164B_DD_FER_RESOL_PROP_CODE:
      *data = (prop->dd_fer_resol.exp  & Si2164B_DD_FER_RESOL_PROP_EXP_MASK ) << Si2164B_DD_FER_RESOL_PROP_EXP_LSB  |
              (prop->dd_fer_resol.mant & Si2164B_DD_FER_RESOL_PROP_MANT_MASK) << Si2164B_DD_FER_RESOL_PROP_MANT_LSB ;
     break;
    #endif /*     Si2164B_DD_FER_RESOL_PROP */
    #ifdef        Si2164B_DD_IEN_PROP
     case         Si2164B_DD_IEN_PROP_CODE:
      *data = (prop->dd_ien.ien_bit0 & Si2164B_DD_IEN_PROP_IEN_BIT0_MASK) << Si2164B_DD_IEN_PROP_IEN_BIT0_LSB  |
              (prop->dd_ien.ien_bit1 & Si2164B_DD_IEN_PROP_IEN_BIT1_MASK) << Si2164B_DD_IEN_PROP_IEN_BIT1_LSB  |
              (prop->dd_ien.ien_bit2 & Si2164B_DD_IEN_PROP_IEN_BIT2_MASK) << Si2164B_DD_IEN_PROP_IEN_BIT2_LSB  |
              (prop->dd_ien.ien_bit3 & Si2164B_DD_IEN_PROP_IEN_BIT3_MASK) << Si2164B_DD_IEN_PROP_IEN_BIT3_LSB  |
              (prop->dd_ien.ien_bit4 & Si2164B_DD_IEN_PROP_IEN_BIT4_MASK) << Si2164B_DD_IEN_PROP_IEN_BIT4_LSB  |
              (prop->dd_ien.ien_bit5 & Si2164B_DD_IEN_PROP_IEN_BIT5_MASK) << Si2164B_DD_IEN_PROP_IEN_BIT5_LSB  |
              (prop->dd_ien.ien_bit6 & Si2164B_DD_IEN_PROP_IEN_BIT6_MASK) << Si2164B_DD_IEN_PROP_IEN_BIT6_LSB  |
              (prop->dd_ien.ien_bit7 & Si2164B_DD_IEN_PROP_IEN_BIT7_MASK) << Si2164B_DD_IEN_PROP_IEN_BIT7_LSB ;
     break;
    #endif /*     Si2164B_DD_IEN_PROP */
    #ifdef        Si2164B_DD_IF_INPUT_FREQ_PROP
     case         Si2164B_DD_IF_INPUT_FREQ_PROP_CODE:
      *data = (prop->dd_if_input_freq.offset & Si2164B_DD_IF_INPUT_FREQ_PROP_OFFSET_MASK) << Si2164B_DD_IF_INPUT_FREQ_PROP_OFFSET_LSB ;
     break;
    #endif /*     Si2164B_DD_IF_INPUT_FREQ_PROP */
    #ifdef        Si2164B_DD_INT_SENSE_PROP
     case         Si2164B_DD_INT_SENSE_PROP_CODE:
      *data = (prop->dd_int_sense.neg_bit0 & Si2164B_DD_INT_SENSE_PROP_NEG_BIT0_MASK) << Si2164B_DD_INT_SENSE_PROP_NEG_BIT0_LSB  |
              (prop->dd_int_sense.neg_bit1 & Si2164B_DD_INT_SENSE_PROP_NEG_BIT1_MASK) << Si2164B_DD_INT_SENSE_PROP_NEG_BIT1_LSB  |
              (prop->dd_int_sense.neg_bit2 & Si2164B_DD_INT_SENSE_PROP_NEG_BIT2_MASK) << Si2164B_DD_INT_SENSE_PROP_NEG_BIT2_LSB  |
              (prop->dd_int_sense.neg_bit3 & Si2164B_DD_INT_SENSE_PROP_NEG_BIT3_MASK) << Si2164B_DD_INT_SENSE_PROP_NEG_BIT3_LSB  |
              (prop->dd_int_sense.neg_bit4 & Si2164B_DD_INT_SENSE_PROP_NEG_BIT4_MASK) << Si2164B_DD_INT_SENSE_PROP_NEG_BIT4_LSB  |
              (prop->dd_int_sense.neg_bit5 & Si2164B_DD_INT_SENSE_PROP_NEG_BIT5_MASK) << Si2164B_DD_INT_SENSE_PROP_NEG_BIT5_LSB  |
              (prop->dd_int_sense.neg_bit6 & Si2164B_DD_INT_SENSE_PROP_NEG_BIT6_MASK) << Si2164B_DD_INT_SENSE_PROP_NEG_BIT6_LSB  |
              (prop->dd_int_sense.neg_bit7 & Si2164B_DD_INT_SENSE_PROP_NEG_BIT7_MASK) << Si2164B_DD_INT_SENSE_PROP_NEG_BIT7_LSB  |
              (prop->dd_int_sense.pos_bit0 & Si2164B_DD_INT_SENSE_PROP_POS_BIT0_MASK) << Si2164B_DD_INT_SENSE_PROP_POS_BIT0_LSB  |
              (prop->dd_int_sense.pos_bit1 & Si2164B_DD_INT_SENSE_PROP_POS_BIT1_MASK) << Si2164B_DD_INT_SENSE_PROP_POS_BIT1_LSB  |
              (prop->dd_int_sense.pos_bit2 & Si2164B_DD_INT_SENSE_PROP_POS_BIT2_MASK) << Si2164B_DD_INT_SENSE_PROP_POS_BIT2_LSB  |
              (prop->dd_int_sense.pos_bit3 & Si2164B_DD_INT_SENSE_PROP_POS_BIT3_MASK) << Si2164B_DD_INT_SENSE_PROP_POS_BIT3_LSB  |
              (prop->dd_int_sense.pos_bit4 & Si2164B_DD_INT_SENSE_PROP_POS_BIT4_MASK) << Si2164B_DD_INT_SENSE_PROP_POS_BIT4_LSB  |
              (prop->dd_int_sense.pos_bit5 & Si2164B_DD_INT_SENSE_PROP_POS_BIT5_MASK) << Si2164B_DD_INT_SENSE_PROP_POS_BIT5_LSB  |
              (prop->dd_int_sense.pos_bit6 & Si2164B_DD_INT_SENSE_PROP_POS_BIT6_MASK) << Si2164B_DD_INT_SENSE_PROP_POS_BIT6_LSB  |
              (prop->dd_int_sense.pos_bit7 & Si2164B_DD_INT_SENSE_PROP_POS_BIT7_MASK) << Si2164B_DD_INT_SENSE_PROP_POS_BIT7_LSB ;
     break;
    #endif /*     Si2164B_DD_INT_SENSE_PROP */
    #ifdef        Si2164B_DD_MODE_PROP
     case         Si2164B_DD_MODE_PROP_CODE:
      *data = (prop->dd_mode.bw              & Si2164B_DD_MODE_PROP_BW_MASK             ) << Si2164B_DD_MODE_PROP_BW_LSB  |
              (prop->dd_mode.modulation      & Si2164B_DD_MODE_PROP_MODULATION_MASK     ) << Si2164B_DD_MODE_PROP_MODULATION_LSB  |
              (prop->dd_mode.invert_spectrum & Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_MASK) << Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_LSB  |
              (prop->dd_mode.auto_detect     & Si2164B_DD_MODE_PROP_AUTO_DETECT_MASK    ) << Si2164B_DD_MODE_PROP_AUTO_DETECT_LSB ;
     break;
    #endif /*     Si2164B_DD_MODE_PROP */
    #ifdef        Si2164B_DD_PER_RESOL_PROP
     case         Si2164B_DD_PER_RESOL_PROP_CODE:
      *data = (prop->dd_per_resol.exp  & Si2164B_DD_PER_RESOL_PROP_EXP_MASK ) << Si2164B_DD_PER_RESOL_PROP_EXP_LSB  |
              (prop->dd_per_resol.mant & Si2164B_DD_PER_RESOL_PROP_MANT_MASK) << Si2164B_DD_PER_RESOL_PROP_MANT_LSB ;
     break;
    #endif /*     Si2164B_DD_PER_RESOL_PROP */
    #ifdef        Si2164B_DD_RSQ_BER_THRESHOLD_PROP
     case         Si2164B_DD_RSQ_BER_THRESHOLD_PROP_CODE:
      *data = (prop->dd_rsq_ber_threshold.exp  & Si2164B_DD_RSQ_BER_THRESHOLD_PROP_EXP_MASK ) << Si2164B_DD_RSQ_BER_THRESHOLD_PROP_EXP_LSB  |
              (prop->dd_rsq_ber_threshold.mant & Si2164B_DD_RSQ_BER_THRESHOLD_PROP_MANT_MASK) << Si2164B_DD_RSQ_BER_THRESHOLD_PROP_MANT_LSB ;
     break;
    #endif /*     Si2164B_DD_RSQ_BER_THRESHOLD_PROP */
    #ifdef        Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP
     case         Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_CODE:
      *data = (prop->dd_sec_ts_serial_diff.ts_data1_strength  & Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA1_STRENGTH_MASK ) << Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA1_STRENGTH_LSB  |
              (prop->dd_sec_ts_serial_diff.ts_data1_shape     & Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA1_SHAPE_MASK    ) << Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA1_SHAPE_LSB  |
              (prop->dd_sec_ts_serial_diff.ts_data2_strength  & Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA2_STRENGTH_MASK ) << Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA2_STRENGTH_LSB  |
              (prop->dd_sec_ts_serial_diff.ts_data2_shape     & Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA2_SHAPE_MASK    ) << Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA2_SHAPE_LSB  |
              (prop->dd_sec_ts_serial_diff.ts_clkb_on_data1   & Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_MASK  ) << Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_LSB  |
              (prop->dd_sec_ts_serial_diff.ts_data0b_on_data2 & Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_MASK) << Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_LSB ;
     break;
    #endif /*     Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP */
    #ifdef        Si2164B_DD_SEC_TS_SETUP_PAR_PROP
     case         Si2164B_DD_SEC_TS_SETUP_PAR_PROP_CODE:
      *data = (prop->dd_sec_ts_setup_par.ts_data_strength & Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_DATA_STRENGTH_MASK) << Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_DATA_STRENGTH_LSB  |
              (prop->dd_sec_ts_setup_par.ts_data_shape    & Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_DATA_SHAPE_MASK   ) << Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_DATA_SHAPE_LSB  |
              (prop->dd_sec_ts_setup_par.ts_clk_strength  & Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_CLK_STRENGTH_MASK ) << Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_CLK_STRENGTH_LSB  |
              (prop->dd_sec_ts_setup_par.ts_clk_shape     & Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_CLK_SHAPE_MASK    ) << Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_CLK_SHAPE_LSB ;
     break;
    #endif /*     Si2164B_DD_SEC_TS_SETUP_PAR_PROP */
    #ifdef        Si2164B_DD_SEC_TS_SETUP_SER_PROP
     case         Si2164B_DD_SEC_TS_SETUP_SER_PROP_CODE:
      *data = (prop->dd_sec_ts_setup_ser.ts_data_strength & Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_DATA_STRENGTH_MASK) << Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_DATA_STRENGTH_LSB  |
              (prop->dd_sec_ts_setup_ser.ts_data_shape    & Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_DATA_SHAPE_MASK   ) << Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_DATA_SHAPE_LSB  |
              (prop->dd_sec_ts_setup_ser.ts_clk_strength  & Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_CLK_STRENGTH_MASK ) << Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_CLK_STRENGTH_LSB  |
              (prop->dd_sec_ts_setup_ser.ts_clk_shape     & Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_CLK_SHAPE_MASK    ) << Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_CLK_SHAPE_LSB ;
     break;
    #endif /*     Si2164B_DD_SEC_TS_SETUP_SER_PROP */
    #ifdef        Si2164B_DD_SEC_TS_SLR_SERIAL_PROP
     case         Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_CODE:
      *data = (prop->dd_sec_ts_slr_serial.ts_data_slr     & Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA_SLR_MASK    ) << Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA_SLR_LSB  |
              (prop->dd_sec_ts_slr_serial.ts_data_slr_on  & Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_MASK ) << Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_LSB  |
              (prop->dd_sec_ts_slr_serial.ts_data1_slr    & Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_MASK   ) << Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_LSB  |
              (prop->dd_sec_ts_slr_serial.ts_data1_slr_on & Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_MASK) << Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_LSB  |
              (prop->dd_sec_ts_slr_serial.ts_data2_slr    & Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_MASK   ) << Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_LSB  |
              (prop->dd_sec_ts_slr_serial.ts_data2_slr_on & Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_MASK) << Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_LSB  |
              (prop->dd_sec_ts_slr_serial.ts_clk_slr      & Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_CLK_SLR_MASK     ) << Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_CLK_SLR_LSB  |
              (prop->dd_sec_ts_slr_serial.ts_clk_slr_on   & Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_MASK  ) << Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_LSB ;
     break;
    #endif /*     Si2164B_DD_SEC_TS_SLR_SERIAL_PROP */
    #ifdef        Si2164B_DD_SSI_SQI_PARAM_PROP
     case         Si2164B_DD_SSI_SQI_PARAM_PROP_CODE:
      *data = (prop->dd_ssi_sqi_param.sqi_average & Si2164B_DD_SSI_SQI_PARAM_PROP_SQI_AVERAGE_MASK) << Si2164B_DD_SSI_SQI_PARAM_PROP_SQI_AVERAGE_LSB ;
     break;
    #endif /*     Si2164B_DD_SSI_SQI_PARAM_PROP */
    #ifdef        Si2164B_DD_TS_FREQ_PROP
     case         Si2164B_DD_TS_FREQ_PROP_CODE:
      *data = (prop->dd_ts_freq.req_freq_10khz & Si2164B_DD_TS_FREQ_PROP_REQ_FREQ_10KHZ_MASK) << Si2164B_DD_TS_FREQ_PROP_REQ_FREQ_10KHZ_LSB ;
     break;
    #endif /*     Si2164B_DD_TS_FREQ_PROP */
    #ifdef        Si2164B_DD_TS_FREQ_MAX_PROP
     case         Si2164B_DD_TS_FREQ_MAX_PROP_CODE:
      *data = (prop->dd_ts_freq_max.req_freq_max_10khz & Si2164B_DD_TS_FREQ_MAX_PROP_REQ_FREQ_MAX_10KHZ_MASK) << Si2164B_DD_TS_FREQ_MAX_PROP_REQ_FREQ_MAX_10KHZ_LSB ;
     break;
    #endif /*     Si2164B_DD_TS_FREQ_MAX_PROP */
    #ifdef        Si2164B_DD_TS_MODE_PROP
     case         Si2164B_DD_TS_MODE_PROP_CODE:
      *data = (prop->dd_ts_mode.mode               & Si2164B_DD_TS_MODE_PROP_MODE_MASK              ) << Si2164B_DD_TS_MODE_PROP_MODE_LSB  |
              (prop->dd_ts_mode.clock              & Si2164B_DD_TS_MODE_PROP_CLOCK_MASK             ) << Si2164B_DD_TS_MODE_PROP_CLOCK_LSB  |
              (prop->dd_ts_mode.clk_gapped_en      & Si2164B_DD_TS_MODE_PROP_CLK_GAPPED_EN_MASK     ) << Si2164B_DD_TS_MODE_PROP_CLK_GAPPED_EN_LSB  |
              (prop->dd_ts_mode.ts_err_polarity    & Si2164B_DD_TS_MODE_PROP_TS_ERR_POLARITY_MASK   ) << Si2164B_DD_TS_MODE_PROP_TS_ERR_POLARITY_LSB  |
              (prop->dd_ts_mode.special            & Si2164B_DD_TS_MODE_PROP_SPECIAL_MASK           ) << Si2164B_DD_TS_MODE_PROP_SPECIAL_LSB  |
              (prop->dd_ts_mode.ts_freq_resolution   & Si2164B_DD_TS_MODE_PROP_TS_FREQ_RESOLUTION_MASK  ) << Si2164B_DD_TS_MODE_PROP_TS_FREQ_RESOLUTION_LSB  |
              (prop->dd_ts_mode.serial_pin_selection & Si2164B_DD_TS_MODE_PROP_SERIAL_PIN_SELECTION_MASK) << Si2164B_DD_TS_MODE_PROP_SERIAL_PIN_SELECTION_LSB ;
     break;
    #endif /*     Si2164B_DD_TS_MODE_PROP */
    #ifdef        Si2164B_DD_TS_SERIAL_DIFF_PROP
     case         Si2164B_DD_TS_SERIAL_DIFF_PROP_CODE:
      *data = (prop->dd_ts_serial_diff.ts_data1_strength  & Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA1_STRENGTH_MASK ) << Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA1_STRENGTH_LSB  |
              (prop->dd_ts_serial_diff.ts_data1_shape     & Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA1_SHAPE_MASK    ) << Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA1_SHAPE_LSB  |
              (prop->dd_ts_serial_diff.ts_data2_strength  & Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA2_STRENGTH_MASK ) << Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA2_STRENGTH_LSB  |
              (prop->dd_ts_serial_diff.ts_data2_shape     & Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA2_SHAPE_MASK    ) << Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA2_SHAPE_LSB  |
              (prop->dd_ts_serial_diff.ts_clkb_on_data1   & Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_MASK  ) << Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_LSB  |
              (prop->dd_ts_serial_diff.ts_data0b_on_data2 & Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_MASK) << Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_LSB ;
     break;
    #endif /*     Si2164B_DD_TS_SERIAL_DIFF_PROP */
    #ifdef        Si2164B_DD_TS_SETUP_PAR_PROP
     case         Si2164B_DD_TS_SETUP_PAR_PROP_CODE:
      *data = (prop->dd_ts_setup_par.ts_data_strength & Si2164B_DD_TS_SETUP_PAR_PROP_TS_DATA_STRENGTH_MASK) << Si2164B_DD_TS_SETUP_PAR_PROP_TS_DATA_STRENGTH_LSB  |
              (prop->dd_ts_setup_par.ts_data_shape    & Si2164B_DD_TS_SETUP_PAR_PROP_TS_DATA_SHAPE_MASK   ) << Si2164B_DD_TS_SETUP_PAR_PROP_TS_DATA_SHAPE_LSB  |
              (prop->dd_ts_setup_par.ts_clk_strength  & Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_STRENGTH_MASK ) << Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_STRENGTH_LSB  |
              (prop->dd_ts_setup_par.ts_clk_shape     & Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_SHAPE_MASK    ) << Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_SHAPE_LSB  |
              (prop->dd_ts_setup_par.ts_clk_invert    & Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_INVERT_MASK   ) << Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_INVERT_LSB  |
              (prop->dd_ts_setup_par.ts_clk_shift     & Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_SHIFT_MASK    ) << Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_SHIFT_LSB ;
     break;
    #endif /*     Si2164B_DD_TS_SETUP_PAR_PROP */
    #ifdef        Si2164B_DD_TS_SETUP_SER_PROP
     case         Si2164B_DD_TS_SETUP_SER_PROP_CODE:
      *data = (prop->dd_ts_setup_ser.ts_data_strength & Si2164B_DD_TS_SETUP_SER_PROP_TS_DATA_STRENGTH_MASK) << Si2164B_DD_TS_SETUP_SER_PROP_TS_DATA_STRENGTH_LSB  |
              (prop->dd_ts_setup_ser.ts_data_shape    & Si2164B_DD_TS_SETUP_SER_PROP_TS_DATA_SHAPE_MASK   ) << Si2164B_DD_TS_SETUP_SER_PROP_TS_DATA_SHAPE_LSB  |
              (prop->dd_ts_setup_ser.ts_clk_strength  & Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_STRENGTH_MASK ) << Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_STRENGTH_LSB  |
              (prop->dd_ts_setup_ser.ts_clk_shape     & Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_SHAPE_MASK    ) << Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_SHAPE_LSB  |
              (prop->dd_ts_setup_ser.ts_clk_invert    & Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_INVERT_MASK   ) << Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_INVERT_LSB  |
              (prop->dd_ts_setup_ser.ts_sync_duration & Si2164B_DD_TS_SETUP_SER_PROP_TS_SYNC_DURATION_MASK) << Si2164B_DD_TS_SETUP_SER_PROP_TS_SYNC_DURATION_LSB  |
              (prop->dd_ts_setup_ser.ts_byte_order    & Si2164B_DD_TS_SETUP_SER_PROP_TS_BYTE_ORDER_MASK   ) << Si2164B_DD_TS_SETUP_SER_PROP_TS_BYTE_ORDER_LSB ;
     break;
    #endif /*     Si2164B_DD_TS_SETUP_SER_PROP */
    #ifdef        Si2164B_DD_TS_SLR_SERIAL_PROP
     case         Si2164B_DD_TS_SLR_SERIAL_PROP_CODE:
      *data = (prop->dd_ts_slr_serial.ts_data_slr     & Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA_SLR_MASK    ) << Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA_SLR_LSB  |
              (prop->dd_ts_slr_serial.ts_data_slr_on  & Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_MASK ) << Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_LSB  |
              (prop->dd_ts_slr_serial.ts_data1_slr    & Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_MASK   ) << Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_LSB  |
              (prop->dd_ts_slr_serial.ts_data1_slr_on & Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_MASK) << Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_LSB  |
              (prop->dd_ts_slr_serial.ts_data2_slr    & Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_MASK   ) << Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_LSB  |
              (prop->dd_ts_slr_serial.ts_data2_slr_on & Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_MASK) << Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_LSB  |
              (prop->dd_ts_slr_serial.ts_clk_slr      & Si2164B_DD_TS_SLR_SERIAL_PROP_TS_CLK_SLR_MASK     ) << Si2164B_DD_TS_SLR_SERIAL_PROP_TS_CLK_SLR_LSB  |
              (prop->dd_ts_slr_serial.ts_clk_slr_on   & Si2164B_DD_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_MASK  ) << Si2164B_DD_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_LSB ;
     break;
    #endif /*     Si2164B_DD_TS_SLR_SERIAL_PROP */
    #ifdef        Si2164B_DVBC2_ADC_CREST_FACTOR_PROP
     case         Si2164B_DVBC2_ADC_CREST_FACTOR_PROP_CODE:
      *data = (prop->dvbc2_adc_crest_factor.crest_factor & Si2164B_DVBC2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK) << Si2164B_DVBC2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB ;
     break;
    #endif /*     Si2164B_DVBC2_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_DVBC2_AFC_RANGE_PROP
     case         Si2164B_DVBC2_AFC_RANGE_PROP_CODE:
      *data = (prop->dvbc2_afc_range.range_khz & Si2164B_DVBC2_AFC_RANGE_PROP_RANGE_KHZ_MASK) << Si2164B_DVBC2_AFC_RANGE_PROP_RANGE_KHZ_LSB ;
     break;
    #endif /*     Si2164B_DVBC2_AFC_RANGE_PROP */

    #ifdef        Si2164B_DVBC_ADC_CREST_FACTOR_PROP
     case         Si2164B_DVBC_ADC_CREST_FACTOR_PROP_CODE:
      *data = (prop->dvbc_adc_crest_factor.crest_factor & Si2164B_DVBC_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK) << Si2164B_DVBC_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB ;
     break;
    #endif /*     Si2164B_DVBC_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_DVBC_AFC_RANGE_PROP
     case         Si2164B_DVBC_AFC_RANGE_PROP_CODE:
      *data = (prop->dvbc_afc_range.range_khz & Si2164B_DVBC_AFC_RANGE_PROP_RANGE_KHZ_MASK) << Si2164B_DVBC_AFC_RANGE_PROP_RANGE_KHZ_LSB ;
     break;
    #endif /*     Si2164B_DVBC_AFC_RANGE_PROP */
    #ifdef        Si2164B_DVBC_CONSTELLATION_PROP
     case         Si2164B_DVBC_CONSTELLATION_PROP_CODE:
      *data = (prop->dvbc_constellation.constellation & Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_MASK) << Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_LSB ;
     break;
    #endif /*     Si2164B_DVBC_CONSTELLATION_PROP */
    #ifdef        Si2164B_DVBC_SYMBOL_RATE_PROP
     case         Si2164B_DVBC_SYMBOL_RATE_PROP_CODE:
      *data = (prop->dvbc_symbol_rate.rate & Si2164B_DVBC_SYMBOL_RATE_PROP_RATE_MASK) << Si2164B_DVBC_SYMBOL_RATE_PROP_RATE_LSB ;
     break;
    #endif /*     Si2164B_DVBC_SYMBOL_RATE_PROP */

    #ifdef        Si2164B_DVBS2_ADC_CREST_FACTOR_PROP
     case         Si2164B_DVBS2_ADC_CREST_FACTOR_PROP_CODE:
      *data = (prop->dvbs2_adc_crest_factor.crest_factor & Si2164B_DVBS2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK) << Si2164B_DVBS2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB ;
     break;
    #endif /*     Si2164B_DVBS2_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_DVBS2_AFC_RANGE_PROP
     case         Si2164B_DVBS2_AFC_RANGE_PROP_CODE:
      *data = (prop->dvbs2_afc_range.range_khz & Si2164B_DVBS2_AFC_RANGE_PROP_RANGE_KHZ_MASK) << Si2164B_DVBS2_AFC_RANGE_PROP_RANGE_KHZ_LSB ;
     break;
    #endif /*     Si2164B_DVBS2_AFC_RANGE_PROP */
    #ifdef        Si2164B_DVBS2_SYMBOL_RATE_PROP
     case         Si2164B_DVBS2_SYMBOL_RATE_PROP_CODE:
      *data = (prop->dvbs2_symbol_rate.rate & Si2164B_DVBS2_SYMBOL_RATE_PROP_RATE_MASK) << Si2164B_DVBS2_SYMBOL_RATE_PROP_RATE_LSB ;
     break;
    #endif /*     Si2164B_DVBS2_SYMBOL_RATE_PROP */
    #ifdef        Si2164B_DVBS_ADC_CREST_FACTOR_PROP
     case         Si2164B_DVBS_ADC_CREST_FACTOR_PROP_CODE:
      *data = (prop->dvbs_adc_crest_factor.crest_factor & Si2164B_DVBS_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK) << Si2164B_DVBS_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB ;
     break;
    #endif /*     Si2164B_DVBS_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_DVBS_AFC_RANGE_PROP
     case         Si2164B_DVBS_AFC_RANGE_PROP_CODE:
      *data = (prop->dvbs_afc_range.range_khz & Si2164B_DVBS_AFC_RANGE_PROP_RANGE_KHZ_MASK) << Si2164B_DVBS_AFC_RANGE_PROP_RANGE_KHZ_LSB ;
     break;
    #endif /*     Si2164B_DVBS_AFC_RANGE_PROP */
    #ifdef        Si2164B_DVBS_SYMBOL_RATE_PROP
     case         Si2164B_DVBS_SYMBOL_RATE_PROP_CODE:
      *data = (prop->dvbs_symbol_rate.rate & Si2164B_DVBS_SYMBOL_RATE_PROP_RATE_MASK) << Si2164B_DVBS_SYMBOL_RATE_PROP_RATE_LSB ;
     break;
    #endif /*     Si2164B_DVBS_SYMBOL_RATE_PROP */

    #ifdef        Si2164B_DVBT2_ADC_CREST_FACTOR_PROP
     case         Si2164B_DVBT2_ADC_CREST_FACTOR_PROP_CODE:
      *data = (prop->dvbt2_adc_crest_factor.crest_factor & Si2164B_DVBT2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK) << Si2164B_DVBT2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB ;
     break;
    #endif /*     Si2164B_DVBT2_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_DVBT2_AFC_RANGE_PROP
     case         Si2164B_DVBT2_AFC_RANGE_PROP_CODE:
      *data = (prop->dvbt2_afc_range.range_khz & Si2164B_DVBT2_AFC_RANGE_PROP_RANGE_KHZ_MASK) << Si2164B_DVBT2_AFC_RANGE_PROP_RANGE_KHZ_LSB ;
     break;
    #endif /*     Si2164B_DVBT2_AFC_RANGE_PROP */
    #ifdef        Si2164B_DVBT2_FEF_TUNER_PROP
     case         Si2164B_DVBT2_FEF_TUNER_PROP_CODE:
      *data = (prop->dvbt2_fef_tuner.tuner_delay         & Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_DELAY_MASK        ) << Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_DELAY_LSB  |
              (prop->dvbt2_fef_tuner.tuner_freeze_time   & Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_FREEZE_TIME_MASK  ) << Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_FREEZE_TIME_LSB  |
              (prop->dvbt2_fef_tuner.tuner_unfreeze_time & Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_UNFREEZE_TIME_MASK) << Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_UNFREEZE_TIME_LSB ;
     break;
    #endif /*     Si2164B_DVBT2_FEF_TUNER_PROP */
    #ifdef        Si2164B_DVBT2_MODE_PROP
     case         Si2164B_DVBT2_MODE_PROP_CODE:
      *data = (prop->dvbt2_mode.lock_mode & Si2164B_DVBT2_MODE_PROP_LOCK_MODE_MASK) << Si2164B_DVBT2_MODE_PROP_LOCK_MODE_LSB ;
     break;
    #endif /*     Si2164B_DVBT2_MODE_PROP */

    #ifdef        Si2164B_DVBT_ADC_CREST_FACTOR_PROP
     case         Si2164B_DVBT_ADC_CREST_FACTOR_PROP_CODE:
      *data = (prop->dvbt_adc_crest_factor.crest_factor & Si2164B_DVBT_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK) << Si2164B_DVBT_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB ;
     break;
    #endif /*     Si2164B_DVBT_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_DVBT_AFC_RANGE_PROP
     case         Si2164B_DVBT_AFC_RANGE_PROP_CODE:
      *data = (prop->dvbt_afc_range.range_khz & Si2164B_DVBT_AFC_RANGE_PROP_RANGE_KHZ_MASK) << Si2164B_DVBT_AFC_RANGE_PROP_RANGE_KHZ_LSB ;
     break;
    #endif /*     Si2164B_DVBT_AFC_RANGE_PROP */
    #ifdef        Si2164B_DVBT_HIERARCHY_PROP
     case         Si2164B_DVBT_HIERARCHY_PROP_CODE:
      *data = (prop->dvbt_hierarchy.stream & Si2164B_DVBT_HIERARCHY_PROP_STREAM_MASK) << Si2164B_DVBT_HIERARCHY_PROP_STREAM_LSB ;
     break;
    #endif /*     Si2164B_DVBT_HIERARCHY_PROP */


    #ifdef        Si2164B_MASTER_IEN_PROP
     case         Si2164B_MASTER_IEN_PROP_CODE:
      *data = (prop->master_ien.ddien   & Si2164B_MASTER_IEN_PROP_DDIEN_MASK  ) << Si2164B_MASTER_IEN_PROP_DDIEN_LSB  |
              (prop->master_ien.scanien & Si2164B_MASTER_IEN_PROP_SCANIEN_MASK) << Si2164B_MASTER_IEN_PROP_SCANIEN_LSB  |
              (prop->master_ien.errien  & Si2164B_MASTER_IEN_PROP_ERRIEN_MASK ) << Si2164B_MASTER_IEN_PROP_ERRIEN_LSB  |
              (prop->master_ien.ctsien  & Si2164B_MASTER_IEN_PROP_CTSIEN_MASK ) << Si2164B_MASTER_IEN_PROP_CTSIEN_LSB ;
     break;
    #endif /*     Si2164B_MASTER_IEN_PROP */
    #ifdef        Si2164B_MCNS_ADC_CREST_FACTOR_PROP
     case         Si2164B_MCNS_ADC_CREST_FACTOR_PROP_CODE:
      *data = (prop->mcns_adc_crest_factor.crest_factor & Si2164B_MCNS_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK) << Si2164B_MCNS_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB ;
     break;
    #endif /*     Si2164B_MCNS_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_MCNS_AFC_RANGE_PROP
     case         Si2164B_MCNS_AFC_RANGE_PROP_CODE:
      *data = (prop->mcns_afc_range.range_khz & Si2164B_MCNS_AFC_RANGE_PROP_RANGE_KHZ_MASK) << Si2164B_MCNS_AFC_RANGE_PROP_RANGE_KHZ_LSB ;
     break;
    #endif /*     Si2164B_MCNS_AFC_RANGE_PROP */
    #ifdef        Si2164B_MCNS_CONSTELLATION_PROP
     case         Si2164B_MCNS_CONSTELLATION_PROP_CODE:
      *data = (prop->mcns_constellation.constellation & Si2164B_MCNS_CONSTELLATION_PROP_CONSTELLATION_MASK) << Si2164B_MCNS_CONSTELLATION_PROP_CONSTELLATION_LSB ;
     break;
    #endif /*     Si2164B_MCNS_CONSTELLATION_PROP */
    #ifdef        Si2164B_MCNS_SYMBOL_RATE_PROP
     case         Si2164B_MCNS_SYMBOL_RATE_PROP_CODE:
      *data = (prop->mcns_symbol_rate.rate & Si2164B_MCNS_SYMBOL_RATE_PROP_RATE_MASK) << Si2164B_MCNS_SYMBOL_RATE_PROP_RATE_LSB ;
     break;
    #endif /*     Si2164B_MCNS_SYMBOL_RATE_PROP */

    #ifdef        Si2164B_SCAN_FMAX_PROP
     case         Si2164B_SCAN_FMAX_PROP_CODE:
      *data = (prop->scan_fmax.scan_fmax & Si2164B_SCAN_FMAX_PROP_SCAN_FMAX_MASK) << Si2164B_SCAN_FMAX_PROP_SCAN_FMAX_LSB ;
     break;
    #endif /*     Si2164B_SCAN_FMAX_PROP */
    #ifdef        Si2164B_SCAN_FMIN_PROP
     case         Si2164B_SCAN_FMIN_PROP_CODE:
      *data = (prop->scan_fmin.scan_fmin & Si2164B_SCAN_FMIN_PROP_SCAN_FMIN_MASK) << Si2164B_SCAN_FMIN_PROP_SCAN_FMIN_LSB ;
     break;
    #endif /*     Si2164B_SCAN_FMIN_PROP */
    #ifdef        Si2164B_SCAN_IEN_PROP
     case         Si2164B_SCAN_IEN_PROP_CODE:
      *data = (prop->scan_ien.buzien & Si2164B_SCAN_IEN_PROP_BUZIEN_MASK) << Si2164B_SCAN_IEN_PROP_BUZIEN_LSB  |
              (prop->scan_ien.reqien & Si2164B_SCAN_IEN_PROP_REQIEN_MASK) << Si2164B_SCAN_IEN_PROP_REQIEN_LSB ;
     break;
    #endif /*     Si2164B_SCAN_IEN_PROP */
    #ifdef        Si2164B_SCAN_INT_SENSE_PROP
     case         Si2164B_SCAN_INT_SENSE_PROP_CODE:
      *data = (prop->scan_int_sense.buznegen & Si2164B_SCAN_INT_SENSE_PROP_BUZNEGEN_MASK) << Si2164B_SCAN_INT_SENSE_PROP_BUZNEGEN_LSB  |
              (prop->scan_int_sense.reqnegen & Si2164B_SCAN_INT_SENSE_PROP_REQNEGEN_MASK) << Si2164B_SCAN_INT_SENSE_PROP_REQNEGEN_LSB  |
              (prop->scan_int_sense.buzposen & Si2164B_SCAN_INT_SENSE_PROP_BUZPOSEN_MASK) << Si2164B_SCAN_INT_SENSE_PROP_BUZPOSEN_LSB  |
              (prop->scan_int_sense.reqposen & Si2164B_SCAN_INT_SENSE_PROP_REQPOSEN_MASK) << Si2164B_SCAN_INT_SENSE_PROP_REQPOSEN_LSB ;
     break;
    #endif /*     Si2164B_SCAN_INT_SENSE_PROP */
    #ifdef        Si2164B_SCAN_SAT_CONFIG_PROP
     case         Si2164B_SCAN_SAT_CONFIG_PROP_CODE:
      *data = (prop->scan_sat_config.analog_detect & Si2164B_SCAN_SAT_CONFIG_PROP_ANALOG_DETECT_MASK) << Si2164B_SCAN_SAT_CONFIG_PROP_ANALOG_DETECT_LSB  |
              (prop->scan_sat_config.reserved1     & Si2164B_SCAN_SAT_CONFIG_PROP_RESERVED1_MASK    ) << Si2164B_SCAN_SAT_CONFIG_PROP_RESERVED1_LSB  |
              (prop->scan_sat_config.reserved2     & Si2164B_SCAN_SAT_CONFIG_PROP_RESERVED2_MASK    ) << Si2164B_SCAN_SAT_CONFIG_PROP_RESERVED2_LSB |
              (prop->scan_sat_config.scan_debug    & Si2164B_SCAN_SAT_CONFIG_PROP_SCAN_DEBUG_MASK   ) << Si2164B_SCAN_SAT_CONFIG_PROP_SCAN_DEBUG_LSB ;
     break;
    #endif /*     Si2164B_SCAN_SAT_CONFIG_PROP */
    #ifdef        Si2164B_SCAN_SAT_UNICABLE_BW_PROP
     case         Si2164B_SCAN_SAT_UNICABLE_BW_PROP_CODE:
      *data = (prop->scan_sat_unicable_bw.scan_sat_unicable_bw & Si2164B_SCAN_SAT_UNICABLE_BW_PROP_SCAN_SAT_UNICABLE_BW_MASK) << Si2164B_SCAN_SAT_UNICABLE_BW_PROP_SCAN_SAT_UNICABLE_BW_LSB ;
     break;
    #endif /*     Si2164B_SCAN_SAT_UNICABLE_BW_PROP */
    #ifdef        Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP
     case         Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP_CODE:
      *data = (prop->scan_sat_unicable_min_tune_step.scan_sat_unicable_min_tune_step & Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_MASK) << Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_LSB ;
     break;
    #endif /*     Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP */

    #ifdef        Si2164B_SCAN_SYMB_RATE_MAX_PROP
     case         Si2164B_SCAN_SYMB_RATE_MAX_PROP_CODE:
      *data = (prop->scan_symb_rate_max.scan_symb_rate_max & Si2164B_SCAN_SYMB_RATE_MAX_PROP_SCAN_SYMB_RATE_MAX_MASK) << Si2164B_SCAN_SYMB_RATE_MAX_PROP_SCAN_SYMB_RATE_MAX_LSB ;
     break;
    #endif /*     Si2164B_SCAN_SYMB_RATE_MAX_PROP */
    #ifdef        Si2164B_SCAN_SYMB_RATE_MIN_PROP
     case         Si2164B_SCAN_SYMB_RATE_MIN_PROP_CODE:
      *data = (prop->scan_symb_rate_min.scan_symb_rate_min & Si2164B_SCAN_SYMB_RATE_MIN_PROP_SCAN_SYMB_RATE_MIN_MASK) << Si2164B_SCAN_SYMB_RATE_MIN_PROP_SCAN_SYMB_RATE_MIN_LSB ;
     break;
    #endif /*     Si2164B_SCAN_SYMB_RATE_MIN_PROP */
    #ifdef        Si2164B_SCAN_TER_CONFIG_PROP
     case         Si2164B_SCAN_TER_CONFIG_PROP_CODE:
      *data = (prop->scan_ter_config.mode          & Si2164B_SCAN_TER_CONFIG_PROP_MODE_MASK         ) << Si2164B_SCAN_TER_CONFIG_PROP_MODE_LSB  |
              (prop->scan_ter_config.analog_bw     & Si2164B_SCAN_TER_CONFIG_PROP_ANALOG_BW_MASK    ) << Si2164B_SCAN_TER_CONFIG_PROP_ANALOG_BW_LSB  |
              (prop->scan_ter_config.search_analog & Si2164B_SCAN_TER_CONFIG_PROP_SEARCH_ANALOG_MASK) << Si2164B_SCAN_TER_CONFIG_PROP_SEARCH_ANALOG_LSB |
              (prop->scan_ter_config.scan_debug    & Si2164B_SCAN_TER_CONFIG_PROP_SCAN_DEBUG_MASK   ) << Si2164B_SCAN_TER_CONFIG_PROP_SCAN_DEBUG_LSB ;
     break;
    #endif /*     Si2164B_SCAN_TER_CONFIG_PROP */

     default : return ERROR_Si2164B_UNKNOWN_PROPERTY; break;
    }
    return NO_Si2164B_ERROR;
}


/***********************************************************************************************************************
  Si2164B_UnpackProperty function
  Use:        This function will unpack all the members of a property from an integer from the GetProperty function.

  Parameter: *prop          the Si2164B property context
  Parameter:  prop_code     the property Id
  Parameter:  data          the property data

  Returns:    NO_Si2164B_ERROR if the property exists.
 ***********************************************************************************************************************/
unsigned char Si2164B_UnpackProperty          (Si2164B_PropObj   *prop, unsigned int prop_code, signed int  data) {
    switch (prop_code) {
    #ifdef        Si2164B_DD_BER_RESOL_PROP
     case         Si2164B_DD_BER_RESOL_PROP_CODE:
               prop->dd_ber_resol.exp  = (data >> Si2164B_DD_BER_RESOL_PROP_EXP_LSB ) & Si2164B_DD_BER_RESOL_PROP_EXP_MASK;
               prop->dd_ber_resol.mant = (data >> Si2164B_DD_BER_RESOL_PROP_MANT_LSB) & Si2164B_DD_BER_RESOL_PROP_MANT_MASK;
     break;
    #endif /*     Si2164B_DD_BER_RESOL_PROP */
    #ifdef        Si2164B_DD_CBER_RESOL_PROP
     case         Si2164B_DD_CBER_RESOL_PROP_CODE:
               prop->dd_cber_resol.exp  = (data >> Si2164B_DD_CBER_RESOL_PROP_EXP_LSB ) & Si2164B_DD_CBER_RESOL_PROP_EXP_MASK;
               prop->dd_cber_resol.mant = (data >> Si2164B_DD_CBER_RESOL_PROP_MANT_LSB) & Si2164B_DD_CBER_RESOL_PROP_MANT_MASK;
     break;
    #endif /*     Si2164B_DD_CBER_RESOL_PROP */
    #ifdef        Si2164B_DD_DISEQC_FREQ_PROP
     case         Si2164B_DD_DISEQC_FREQ_PROP_CODE:
               prop->dd_diseqc_freq.freq_hz = (data >> Si2164B_DD_DISEQC_FREQ_PROP_FREQ_HZ_LSB) & Si2164B_DD_DISEQC_FREQ_PROP_FREQ_HZ_MASK;
     break;
    #endif /*     Si2164B_DD_DISEQC_FREQ_PROP */
    #ifdef        Si2164B_DD_DISEQC_PARAM_PROP
     case         Si2164B_DD_DISEQC_PARAM_PROP_CODE:
               prop->dd_diseqc_param.sequence_mode = (data >> Si2164B_DD_DISEQC_PARAM_PROP_SEQUENCE_MODE_LSB) & Si2164B_DD_DISEQC_PARAM_PROP_SEQUENCE_MODE_MASK;
               prop->dd_diseqc_param.input_pin     = (data >> Si2164B_DD_DISEQC_PARAM_PROP_INPUT_PIN_LSB    ) & Si2164B_DD_DISEQC_PARAM_PROP_INPUT_PIN_MASK;
     break;
    #endif /*     Si2164B_DD_DISEQC_PARAM_PROP */

    #ifdef        Si2164B_DD_FER_RESOL_PROP
     case         Si2164B_DD_FER_RESOL_PROP_CODE:
               prop->dd_fer_resol.exp  = (data >> Si2164B_DD_FER_RESOL_PROP_EXP_LSB ) & Si2164B_DD_FER_RESOL_PROP_EXP_MASK;
               prop->dd_fer_resol.mant = (data >> Si2164B_DD_FER_RESOL_PROP_MANT_LSB) & Si2164B_DD_FER_RESOL_PROP_MANT_MASK;
     break;
    #endif /*     Si2164B_DD_FER_RESOL_PROP */
    #ifdef        Si2164B_DD_IEN_PROP
     case         Si2164B_DD_IEN_PROP_CODE:
               prop->dd_ien.ien_bit0 = (data >> Si2164B_DD_IEN_PROP_IEN_BIT0_LSB) & Si2164B_DD_IEN_PROP_IEN_BIT0_MASK;
               prop->dd_ien.ien_bit1 = (data >> Si2164B_DD_IEN_PROP_IEN_BIT1_LSB) & Si2164B_DD_IEN_PROP_IEN_BIT1_MASK;
               prop->dd_ien.ien_bit2 = (data >> Si2164B_DD_IEN_PROP_IEN_BIT2_LSB) & Si2164B_DD_IEN_PROP_IEN_BIT2_MASK;
               prop->dd_ien.ien_bit3 = (data >> Si2164B_DD_IEN_PROP_IEN_BIT3_LSB) & Si2164B_DD_IEN_PROP_IEN_BIT3_MASK;
               prop->dd_ien.ien_bit4 = (data >> Si2164B_DD_IEN_PROP_IEN_BIT4_LSB) & Si2164B_DD_IEN_PROP_IEN_BIT4_MASK;
               prop->dd_ien.ien_bit5 = (data >> Si2164B_DD_IEN_PROP_IEN_BIT5_LSB) & Si2164B_DD_IEN_PROP_IEN_BIT5_MASK;
               prop->dd_ien.ien_bit6 = (data >> Si2164B_DD_IEN_PROP_IEN_BIT6_LSB) & Si2164B_DD_IEN_PROP_IEN_BIT6_MASK;
               prop->dd_ien.ien_bit7 = (data >> Si2164B_DD_IEN_PROP_IEN_BIT7_LSB) & Si2164B_DD_IEN_PROP_IEN_BIT7_MASK;
     break;
    #endif /*     Si2164B_DD_IEN_PROP */
    #ifdef        Si2164B_DD_IF_INPUT_FREQ_PROP
     case         Si2164B_DD_IF_INPUT_FREQ_PROP_CODE:
               prop->dd_if_input_freq.offset = (data >> Si2164B_DD_IF_INPUT_FREQ_PROP_OFFSET_LSB) & Si2164B_DD_IF_INPUT_FREQ_PROP_OFFSET_MASK;
     break;
    #endif /*     Si2164B_DD_IF_INPUT_FREQ_PROP */
    #ifdef        Si2164B_DD_INT_SENSE_PROP
     case         Si2164B_DD_INT_SENSE_PROP_CODE:
               prop->dd_int_sense.neg_bit0 = (data >> Si2164B_DD_INT_SENSE_PROP_NEG_BIT0_LSB) & Si2164B_DD_INT_SENSE_PROP_NEG_BIT0_MASK;
               prop->dd_int_sense.neg_bit1 = (data >> Si2164B_DD_INT_SENSE_PROP_NEG_BIT1_LSB) & Si2164B_DD_INT_SENSE_PROP_NEG_BIT1_MASK;
               prop->dd_int_sense.neg_bit2 = (data >> Si2164B_DD_INT_SENSE_PROP_NEG_BIT2_LSB) & Si2164B_DD_INT_SENSE_PROP_NEG_BIT2_MASK;
               prop->dd_int_sense.neg_bit3 = (data >> Si2164B_DD_INT_SENSE_PROP_NEG_BIT3_LSB) & Si2164B_DD_INT_SENSE_PROP_NEG_BIT3_MASK;
               prop->dd_int_sense.neg_bit4 = (data >> Si2164B_DD_INT_SENSE_PROP_NEG_BIT4_LSB) & Si2164B_DD_INT_SENSE_PROP_NEG_BIT4_MASK;
               prop->dd_int_sense.neg_bit5 = (data >> Si2164B_DD_INT_SENSE_PROP_NEG_BIT5_LSB) & Si2164B_DD_INT_SENSE_PROP_NEG_BIT5_MASK;
               prop->dd_int_sense.neg_bit6 = (data >> Si2164B_DD_INT_SENSE_PROP_NEG_BIT6_LSB) & Si2164B_DD_INT_SENSE_PROP_NEG_BIT6_MASK;
               prop->dd_int_sense.neg_bit7 = (data >> Si2164B_DD_INT_SENSE_PROP_NEG_BIT7_LSB) & Si2164B_DD_INT_SENSE_PROP_NEG_BIT7_MASK;
               prop->dd_int_sense.pos_bit0 = (data >> Si2164B_DD_INT_SENSE_PROP_POS_BIT0_LSB) & Si2164B_DD_INT_SENSE_PROP_POS_BIT0_MASK;
               prop->dd_int_sense.pos_bit1 = (data >> Si2164B_DD_INT_SENSE_PROP_POS_BIT1_LSB) & Si2164B_DD_INT_SENSE_PROP_POS_BIT1_MASK;
               prop->dd_int_sense.pos_bit2 = (data >> Si2164B_DD_INT_SENSE_PROP_POS_BIT2_LSB) & Si2164B_DD_INT_SENSE_PROP_POS_BIT2_MASK;
               prop->dd_int_sense.pos_bit3 = (data >> Si2164B_DD_INT_SENSE_PROP_POS_BIT3_LSB) & Si2164B_DD_INT_SENSE_PROP_POS_BIT3_MASK;
               prop->dd_int_sense.pos_bit4 = (data >> Si2164B_DD_INT_SENSE_PROP_POS_BIT4_LSB) & Si2164B_DD_INT_SENSE_PROP_POS_BIT4_MASK;
               prop->dd_int_sense.pos_bit5 = (data >> Si2164B_DD_INT_SENSE_PROP_POS_BIT5_LSB) & Si2164B_DD_INT_SENSE_PROP_POS_BIT5_MASK;
               prop->dd_int_sense.pos_bit6 = (data >> Si2164B_DD_INT_SENSE_PROP_POS_BIT6_LSB) & Si2164B_DD_INT_SENSE_PROP_POS_BIT6_MASK;
               prop->dd_int_sense.pos_bit7 = (data >> Si2164B_DD_INT_SENSE_PROP_POS_BIT7_LSB) & Si2164B_DD_INT_SENSE_PROP_POS_BIT7_MASK;
     break;
    #endif /*     Si2164B_DD_INT_SENSE_PROP */
    #ifdef        Si2164B_DD_MODE_PROP
     case         Si2164B_DD_MODE_PROP_CODE:
               prop->dd_mode.bw              = (data >> Si2164B_DD_MODE_PROP_BW_LSB             ) & Si2164B_DD_MODE_PROP_BW_MASK;
               prop->dd_mode.modulation      = (data >> Si2164B_DD_MODE_PROP_MODULATION_LSB     ) & Si2164B_DD_MODE_PROP_MODULATION_MASK;
               prop->dd_mode.invert_spectrum = (data >> Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_LSB) & Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_MASK;
               prop->dd_mode.auto_detect     = (data >> Si2164B_DD_MODE_PROP_AUTO_DETECT_LSB    ) & Si2164B_DD_MODE_PROP_AUTO_DETECT_MASK;
     break;
    #endif /*     Si2164B_DD_MODE_PROP */
    #ifdef        Si2164B_DD_PER_RESOL_PROP
     case         Si2164B_DD_PER_RESOL_PROP_CODE:
               prop->dd_per_resol.exp  = (data >> Si2164B_DD_PER_RESOL_PROP_EXP_LSB ) & Si2164B_DD_PER_RESOL_PROP_EXP_MASK;
               prop->dd_per_resol.mant = (data >> Si2164B_DD_PER_RESOL_PROP_MANT_LSB) & Si2164B_DD_PER_RESOL_PROP_MANT_MASK;
     break;
    #endif /*     Si2164B_DD_PER_RESOL_PROP */
    #ifdef        Si2164B_DD_RSQ_BER_THRESHOLD_PROP
     case         Si2164B_DD_RSQ_BER_THRESHOLD_PROP_CODE:
               prop->dd_rsq_ber_threshold.exp  = (data >> Si2164B_DD_RSQ_BER_THRESHOLD_PROP_EXP_LSB ) & Si2164B_DD_RSQ_BER_THRESHOLD_PROP_EXP_MASK;
               prop->dd_rsq_ber_threshold.mant = (data >> Si2164B_DD_RSQ_BER_THRESHOLD_PROP_MANT_LSB) & Si2164B_DD_RSQ_BER_THRESHOLD_PROP_MANT_MASK;
     break;
    #endif /*     Si2164B_DD_RSQ_BER_THRESHOLD_PROP */
    #ifdef        Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP
     case         Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_CODE:
               prop->dd_sec_ts_serial_diff.ts_data1_strength  = (data >> Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA1_STRENGTH_LSB ) & Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA1_STRENGTH_MASK;
               prop->dd_sec_ts_serial_diff.ts_data1_shape     = (data >> Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA1_SHAPE_LSB    ) & Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA1_SHAPE_MASK;
               prop->dd_sec_ts_serial_diff.ts_data2_strength  = (data >> Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA2_STRENGTH_LSB ) & Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA2_STRENGTH_MASK;
               prop->dd_sec_ts_serial_diff.ts_data2_shape     = (data >> Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA2_SHAPE_LSB    ) & Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA2_SHAPE_MASK;
               prop->dd_sec_ts_serial_diff.ts_clkb_on_data1   = (data >> Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_LSB  ) & Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_MASK;
               prop->dd_sec_ts_serial_diff.ts_data0b_on_data2 = (data >> Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_LSB) & Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_MASK;
     break;
    #endif /*     Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP */
    #ifdef        Si2164B_DD_SEC_TS_SETUP_PAR_PROP
     case         Si2164B_DD_SEC_TS_SETUP_PAR_PROP_CODE:
               prop->dd_sec_ts_setup_par.ts_data_strength = (data >> Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_DATA_STRENGTH_LSB) & Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_DATA_STRENGTH_MASK;
               prop->dd_sec_ts_setup_par.ts_data_shape    = (data >> Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_DATA_SHAPE_LSB   ) & Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_DATA_SHAPE_MASK;
               prop->dd_sec_ts_setup_par.ts_clk_strength  = (data >> Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_CLK_STRENGTH_LSB ) & Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_CLK_STRENGTH_MASK;
               prop->dd_sec_ts_setup_par.ts_clk_shape     = (data >> Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_CLK_SHAPE_LSB    ) & Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_CLK_SHAPE_MASK;
     break;
    #endif /*     Si2164B_DD_SEC_TS_SETUP_PAR_PROP */
    #ifdef        Si2164B_DD_SEC_TS_SETUP_SER_PROP
     case         Si2164B_DD_SEC_TS_SETUP_SER_PROP_CODE:
               prop->dd_sec_ts_setup_ser.ts_data_strength = (data >> Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_DATA_STRENGTH_LSB) & Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_DATA_STRENGTH_MASK;
               prop->dd_sec_ts_setup_ser.ts_data_shape    = (data >> Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_DATA_SHAPE_LSB   ) & Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_DATA_SHAPE_MASK;
               prop->dd_sec_ts_setup_ser.ts_clk_strength  = (data >> Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_CLK_STRENGTH_LSB ) & Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_CLK_STRENGTH_MASK;
               prop->dd_sec_ts_setup_ser.ts_clk_shape     = (data >> Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_CLK_SHAPE_LSB    ) & Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_CLK_SHAPE_MASK;
     break;
    #endif /*     Si2164B_DD_SEC_TS_SETUP_SER_PROP */
    #ifdef        Si2164B_DD_SEC_TS_SLR_SERIAL_PROP
     case         Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_CODE:
               prop->dd_sec_ts_slr_serial.ts_data_slr     = (data >> Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA_SLR_LSB    ) & Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA_SLR_MASK;
               prop->dd_sec_ts_slr_serial.ts_data_slr_on  = (data >> Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_LSB ) & Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_MASK;
               prop->dd_sec_ts_slr_serial.ts_data1_slr    = (data >> Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_LSB   ) & Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_MASK;
               prop->dd_sec_ts_slr_serial.ts_data1_slr_on = (data >> Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_LSB) & Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_MASK;
               prop->dd_sec_ts_slr_serial.ts_data2_slr    = (data >> Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_LSB   ) & Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_MASK;
               prop->dd_sec_ts_slr_serial.ts_data2_slr_on = (data >> Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_LSB) & Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_MASK;
               prop->dd_sec_ts_slr_serial.ts_clk_slr      = (data >> Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_CLK_SLR_LSB     ) & Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_CLK_SLR_MASK;
               prop->dd_sec_ts_slr_serial.ts_clk_slr_on   = (data >> Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_LSB  ) & Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_MASK;
     break;
    #endif /*     Si2164B_DD_SEC_TS_SLR_SERIAL_PROP */
    #ifdef        Si2164B_DD_SSI_SQI_PARAM_PROP
     case         Si2164B_DD_SSI_SQI_PARAM_PROP_CODE:
               prop->dd_ssi_sqi_param.sqi_average = (data >> Si2164B_DD_SSI_SQI_PARAM_PROP_SQI_AVERAGE_LSB) & Si2164B_DD_SSI_SQI_PARAM_PROP_SQI_AVERAGE_MASK;
     break;
    #endif /*     Si2164B_DD_SSI_SQI_PARAM_PROP */
    #ifdef        Si2164B_DD_TS_FREQ_PROP
     case         Si2164B_DD_TS_FREQ_PROP_CODE:
               prop->dd_ts_freq.req_freq_10khz = (data >> Si2164B_DD_TS_FREQ_PROP_REQ_FREQ_10KHZ_LSB) & Si2164B_DD_TS_FREQ_PROP_REQ_FREQ_10KHZ_MASK;
     break;
    #endif /*     Si2164B_DD_TS_FREQ_PROP */
    #ifdef        Si2164B_DD_TS_FREQ_MAX_PROP
     case         Si2164B_DD_TS_FREQ_MAX_PROP_CODE:
               prop->dd_ts_freq_max.req_freq_max_10khz = (data >> Si2164B_DD_TS_FREQ_MAX_PROP_REQ_FREQ_MAX_10KHZ_LSB) & Si2164B_DD_TS_FREQ_MAX_PROP_REQ_FREQ_MAX_10KHZ_MASK;
     break;
    #endif /*     Si2164B_DD_TS_FREQ_MAX_PROP */
    #ifdef        Si2164B_DD_TS_MODE_PROP
     case         Si2164B_DD_TS_MODE_PROP_CODE:
               prop->dd_ts_mode.mode               = (data >> Si2164B_DD_TS_MODE_PROP_MODE_LSB              ) & Si2164B_DD_TS_MODE_PROP_MODE_MASK;
               prop->dd_ts_mode.clock              = (data >> Si2164B_DD_TS_MODE_PROP_CLOCK_LSB             ) & Si2164B_DD_TS_MODE_PROP_CLOCK_MASK;
               prop->dd_ts_mode.clk_gapped_en      = (data >> Si2164B_DD_TS_MODE_PROP_CLK_GAPPED_EN_LSB     ) & Si2164B_DD_TS_MODE_PROP_CLK_GAPPED_EN_MASK;
               prop->dd_ts_mode.ts_err_polarity    = (data >> Si2164B_DD_TS_MODE_PROP_TS_ERR_POLARITY_LSB   ) & Si2164B_DD_TS_MODE_PROP_TS_ERR_POLARITY_MASK;
               prop->dd_ts_mode.special            = (data >> Si2164B_DD_TS_MODE_PROP_SPECIAL_LSB           ) & Si2164B_DD_TS_MODE_PROP_SPECIAL_MASK;
               prop->dd_ts_mode.ts_freq_resolution = (data >> Si2164B_DD_TS_MODE_PROP_TS_FREQ_RESOLUTION_LSB) & Si2164B_DD_TS_MODE_PROP_TS_FREQ_RESOLUTION_MASK;
               prop->dd_ts_mode.serial_pin_selection = (data >> Si2164B_DD_TS_MODE_PROP_SERIAL_PIN_SELECTION_LSB) & Si2164B_DD_TS_MODE_PROP_SERIAL_PIN_SELECTION_MASK;
     break;
    #endif /*     Si2164B_DD_TS_MODE_PROP */
    #ifdef        Si2164B_DD_TS_SERIAL_DIFF_PROP
     case         Si2164B_DD_TS_SERIAL_DIFF_PROP_CODE:
               prop->dd_ts_serial_diff.ts_data1_strength  = (data >> Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA1_STRENGTH_LSB ) & Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA1_STRENGTH_MASK;
               prop->dd_ts_serial_diff.ts_data1_shape     = (data >> Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA1_SHAPE_LSB    ) & Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA1_SHAPE_MASK;
               prop->dd_ts_serial_diff.ts_data2_strength  = (data >> Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA2_STRENGTH_LSB ) & Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA2_STRENGTH_MASK;
               prop->dd_ts_serial_diff.ts_data2_shape     = (data >> Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA2_SHAPE_LSB    ) & Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA2_SHAPE_MASK;
               prop->dd_ts_serial_diff.ts_clkb_on_data1   = (data >> Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_LSB  ) & Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_MASK;
               prop->dd_ts_serial_diff.ts_data0b_on_data2 = (data >> Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_LSB) & Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_MASK;
     break;
    #endif /*     Si2164B_DD_TS_SERIAL_DIFF_PROP */
    #ifdef        Si2164B_DD_TS_SETUP_PAR_PROP
     case         Si2164B_DD_TS_SETUP_PAR_PROP_CODE:
               prop->dd_ts_setup_par.ts_data_strength = (data >> Si2164B_DD_TS_SETUP_PAR_PROP_TS_DATA_STRENGTH_LSB) & Si2164B_DD_TS_SETUP_PAR_PROP_TS_DATA_STRENGTH_MASK;
               prop->dd_ts_setup_par.ts_data_shape    = (data >> Si2164B_DD_TS_SETUP_PAR_PROP_TS_DATA_SHAPE_LSB   ) & Si2164B_DD_TS_SETUP_PAR_PROP_TS_DATA_SHAPE_MASK;
               prop->dd_ts_setup_par.ts_clk_strength  = (data >> Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_STRENGTH_LSB ) & Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_STRENGTH_MASK;
               prop->dd_ts_setup_par.ts_clk_shape     = (data >> Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_SHAPE_LSB    ) & Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_SHAPE_MASK;
               prop->dd_ts_setup_par.ts_clk_invert    = (data >> Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_INVERT_LSB   ) & Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_INVERT_MASK;
               prop->dd_ts_setup_par.ts_clk_shift     = (data >> Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_SHIFT_LSB    ) & Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_SHIFT_MASK;
     break;
    #endif /*     Si2164B_DD_TS_SETUP_PAR_PROP */
    #ifdef        Si2164B_DD_TS_SETUP_SER_PROP
     case         Si2164B_DD_TS_SETUP_SER_PROP_CODE:
               prop->dd_ts_setup_ser.ts_data_strength = (data >> Si2164B_DD_TS_SETUP_SER_PROP_TS_DATA_STRENGTH_LSB) & Si2164B_DD_TS_SETUP_SER_PROP_TS_DATA_STRENGTH_MASK;
               prop->dd_ts_setup_ser.ts_data_shape    = (data >> Si2164B_DD_TS_SETUP_SER_PROP_TS_DATA_SHAPE_LSB   ) & Si2164B_DD_TS_SETUP_SER_PROP_TS_DATA_SHAPE_MASK;
               prop->dd_ts_setup_ser.ts_clk_strength  = (data >> Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_STRENGTH_LSB ) & Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_STRENGTH_MASK;
               prop->dd_ts_setup_ser.ts_clk_shape     = (data >> Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_SHAPE_LSB    ) & Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_SHAPE_MASK;
               prop->dd_ts_setup_ser.ts_clk_invert    = (data >> Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_INVERT_LSB   ) & Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_INVERT_MASK;
               prop->dd_ts_setup_ser.ts_sync_duration = (data >> Si2164B_DD_TS_SETUP_SER_PROP_TS_SYNC_DURATION_LSB) & Si2164B_DD_TS_SETUP_SER_PROP_TS_SYNC_DURATION_MASK;
               prop->dd_ts_setup_ser.ts_byte_order    = (data >> Si2164B_DD_TS_SETUP_SER_PROP_TS_BYTE_ORDER_LSB   ) & Si2164B_DD_TS_SETUP_SER_PROP_TS_BYTE_ORDER_MASK;
     break;
    #endif /*     Si2164B_DD_TS_SETUP_SER_PROP */
    #ifdef        Si2164B_DD_TS_SLR_SERIAL_PROP
     case         Si2164B_DD_TS_SLR_SERIAL_PROP_CODE:
               prop->dd_ts_slr_serial.ts_data_slr     = (data >> Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA_SLR_LSB    ) & Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA_SLR_MASK;
               prop->dd_ts_slr_serial.ts_data_slr_on  = (data >> Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_LSB ) & Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_MASK;
               prop->dd_ts_slr_serial.ts_data1_slr    = (data >> Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_LSB   ) & Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_MASK;
               prop->dd_ts_slr_serial.ts_data1_slr_on = (data >> Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_LSB) & Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_MASK;
               prop->dd_ts_slr_serial.ts_data2_slr    = (data >> Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_LSB   ) & Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_MASK;
               prop->dd_ts_slr_serial.ts_data2_slr_on = (data >> Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_LSB) & Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_MASK;
               prop->dd_ts_slr_serial.ts_clk_slr      = (data >> Si2164B_DD_TS_SLR_SERIAL_PROP_TS_CLK_SLR_LSB     ) & Si2164B_DD_TS_SLR_SERIAL_PROP_TS_CLK_SLR_MASK;
               prop->dd_ts_slr_serial.ts_clk_slr_on   = (data >> Si2164B_DD_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_LSB  ) & Si2164B_DD_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_MASK;
     break;
    #endif /*     Si2164B_DD_TS_SLR_SERIAL_PROP */
    #ifdef        Si2164B_DVBC2_ADC_CREST_FACTOR_PROP
     case         Si2164B_DVBC2_ADC_CREST_FACTOR_PROP_CODE:
               prop->dvbc2_adc_crest_factor.crest_factor = (data >> Si2164B_DVBC2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB) & Si2164B_DVBC2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK;
     break;
    #endif /*     Si2164B_DVBC2_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_DVBC2_AFC_RANGE_PROP
     case         Si2164B_DVBC2_AFC_RANGE_PROP_CODE:
               prop->dvbc2_afc_range.range_khz = (data >> Si2164B_DVBC2_AFC_RANGE_PROP_RANGE_KHZ_LSB) & Si2164B_DVBC2_AFC_RANGE_PROP_RANGE_KHZ_MASK;
     break;
    #endif /*     Si2164B_DVBC2_AFC_RANGE_PROP */

    #ifdef        Si2164B_DVBC_ADC_CREST_FACTOR_PROP
     case         Si2164B_DVBC_ADC_CREST_FACTOR_PROP_CODE:
               prop->dvbc_adc_crest_factor.crest_factor = (data >> Si2164B_DVBC_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB) & Si2164B_DVBC_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK;
     break;
    #endif /*     Si2164B_DVBC_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_DVBC_AFC_RANGE_PROP
     case         Si2164B_DVBC_AFC_RANGE_PROP_CODE:
               prop->dvbc_afc_range.range_khz = (data >> Si2164B_DVBC_AFC_RANGE_PROP_RANGE_KHZ_LSB) & Si2164B_DVBC_AFC_RANGE_PROP_RANGE_KHZ_MASK;
     break;
    #endif /*     Si2164B_DVBC_AFC_RANGE_PROP */
    #ifdef        Si2164B_DVBC_CONSTELLATION_PROP
     case         Si2164B_DVBC_CONSTELLATION_PROP_CODE:
               prop->dvbc_constellation.constellation = (data >> Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_LSB) & Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_MASK;
     break;
    #endif /*     Si2164B_DVBC_CONSTELLATION_PROP */
    #ifdef        Si2164B_DVBC_SYMBOL_RATE_PROP
     case         Si2164B_DVBC_SYMBOL_RATE_PROP_CODE:
               prop->dvbc_symbol_rate.rate = (data >> Si2164B_DVBC_SYMBOL_RATE_PROP_RATE_LSB) & Si2164B_DVBC_SYMBOL_RATE_PROP_RATE_MASK;
     break;
    #endif /*     Si2164B_DVBC_SYMBOL_RATE_PROP */

    #ifdef        Si2164B_DVBS2_ADC_CREST_FACTOR_PROP
     case         Si2164B_DVBS2_ADC_CREST_FACTOR_PROP_CODE:
               prop->dvbs2_adc_crest_factor.crest_factor = (data >> Si2164B_DVBS2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB) & Si2164B_DVBS2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK;
     break;
    #endif /*     Si2164B_DVBS2_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_DVBS2_AFC_RANGE_PROP
     case         Si2164B_DVBS2_AFC_RANGE_PROP_CODE:
               prop->dvbs2_afc_range.range_khz = (data >> Si2164B_DVBS2_AFC_RANGE_PROP_RANGE_KHZ_LSB) & Si2164B_DVBS2_AFC_RANGE_PROP_RANGE_KHZ_MASK;
     break;
    #endif /*     Si2164B_DVBS2_AFC_RANGE_PROP */
    #ifdef        Si2164B_DVBS2_SYMBOL_RATE_PROP
     case         Si2164B_DVBS2_SYMBOL_RATE_PROP_CODE:
               prop->dvbs2_symbol_rate.rate = (data >> Si2164B_DVBS2_SYMBOL_RATE_PROP_RATE_LSB) & Si2164B_DVBS2_SYMBOL_RATE_PROP_RATE_MASK;
     break;
    #endif /*     Si2164B_DVBS2_SYMBOL_RATE_PROP */
    #ifdef        Si2164B_DVBS_ADC_CREST_FACTOR_PROP
     case         Si2164B_DVBS_ADC_CREST_FACTOR_PROP_CODE:
               prop->dvbs_adc_crest_factor.crest_factor = (data >> Si2164B_DVBS_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB) & Si2164B_DVBS_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK;
     break;
    #endif /*     Si2164B_DVBS_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_DVBS_AFC_RANGE_PROP
     case         Si2164B_DVBS_AFC_RANGE_PROP_CODE:
               prop->dvbs_afc_range.range_khz = (data >> Si2164B_DVBS_AFC_RANGE_PROP_RANGE_KHZ_LSB) & Si2164B_DVBS_AFC_RANGE_PROP_RANGE_KHZ_MASK;
     break;
    #endif /*     Si2164B_DVBS_AFC_RANGE_PROP */
    #ifdef        Si2164B_DVBS_SYMBOL_RATE_PROP
     case         Si2164B_DVBS_SYMBOL_RATE_PROP_CODE:
               prop->dvbs_symbol_rate.rate = (data >> Si2164B_DVBS_SYMBOL_RATE_PROP_RATE_LSB) & Si2164B_DVBS_SYMBOL_RATE_PROP_RATE_MASK;
     break;
    #endif /*     Si2164B_DVBS_SYMBOL_RATE_PROP */

    #ifdef        Si2164B_DVBT2_ADC_CREST_FACTOR_PROP
     case         Si2164B_DVBT2_ADC_CREST_FACTOR_PROP_CODE:
               prop->dvbt2_adc_crest_factor.crest_factor = (data >> Si2164B_DVBT2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB) & Si2164B_DVBT2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK;
     break;
    #endif /*     Si2164B_DVBT2_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_DVBT2_AFC_RANGE_PROP
     case         Si2164B_DVBT2_AFC_RANGE_PROP_CODE:
               prop->dvbt2_afc_range.range_khz = (data >> Si2164B_DVBT2_AFC_RANGE_PROP_RANGE_KHZ_LSB) & Si2164B_DVBT2_AFC_RANGE_PROP_RANGE_KHZ_MASK;
     break;
    #endif /*     Si2164B_DVBT2_AFC_RANGE_PROP */
    #ifdef        Si2164B_DVBT2_FEF_TUNER_PROP
     case         Si2164B_DVBT2_FEF_TUNER_PROP_CODE:
               prop->dvbt2_fef_tuner.tuner_delay         = (data >> Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_DELAY_LSB        ) & Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_DELAY_MASK;
               prop->dvbt2_fef_tuner.tuner_freeze_time   = (data >> Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_FREEZE_TIME_LSB  ) & Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_FREEZE_TIME_MASK;
               prop->dvbt2_fef_tuner.tuner_unfreeze_time = (data >> Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_UNFREEZE_TIME_LSB) & Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_UNFREEZE_TIME_MASK;
     break;
    #endif /*     Si2164B_DVBT2_FEF_TUNER_PROP */
    #ifdef        Si2164B_DVBT2_MODE_PROP
     case         Si2164B_DVBT2_MODE_PROP_CODE:
               prop->dvbt2_mode.lock_mode = (data >> Si2164B_DVBT2_MODE_PROP_LOCK_MODE_LSB) & Si2164B_DVBT2_MODE_PROP_LOCK_MODE_MASK;
     break;
    #endif /*     Si2164B_DVBT2_MODE_PROP */

    #ifdef        Si2164B_DVBT_ADC_CREST_FACTOR_PROP
     case         Si2164B_DVBT_ADC_CREST_FACTOR_PROP_CODE:
               prop->dvbt_adc_crest_factor.crest_factor = (data >> Si2164B_DVBT_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB) & Si2164B_DVBT_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK;
     break;
    #endif /*     Si2164B_DVBT_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_DVBT_AFC_RANGE_PROP
     case         Si2164B_DVBT_AFC_RANGE_PROP_CODE:
               prop->dvbt_afc_range.range_khz = (data >> Si2164B_DVBT_AFC_RANGE_PROP_RANGE_KHZ_LSB) & Si2164B_DVBT_AFC_RANGE_PROP_RANGE_KHZ_MASK;
     break;
    #endif /*     Si2164B_DVBT_AFC_RANGE_PROP */
    #ifdef        Si2164B_DVBT_HIERARCHY_PROP
     case         Si2164B_DVBT_HIERARCHY_PROP_CODE:
               prop->dvbt_hierarchy.stream = (data >> Si2164B_DVBT_HIERARCHY_PROP_STREAM_LSB) & Si2164B_DVBT_HIERARCHY_PROP_STREAM_MASK;
     break;
    #endif /*     Si2164B_DVBT_HIERARCHY_PROP */


    #ifdef        Si2164B_MASTER_IEN_PROP
     case         Si2164B_MASTER_IEN_PROP_CODE:
               prop->master_ien.ddien   = (data >> Si2164B_MASTER_IEN_PROP_DDIEN_LSB  ) & Si2164B_MASTER_IEN_PROP_DDIEN_MASK;
               prop->master_ien.scanien = (data >> Si2164B_MASTER_IEN_PROP_SCANIEN_LSB) & Si2164B_MASTER_IEN_PROP_SCANIEN_MASK;
               prop->master_ien.errien  = (data >> Si2164B_MASTER_IEN_PROP_ERRIEN_LSB ) & Si2164B_MASTER_IEN_PROP_ERRIEN_MASK;
               prop->master_ien.ctsien  = (data >> Si2164B_MASTER_IEN_PROP_CTSIEN_LSB ) & Si2164B_MASTER_IEN_PROP_CTSIEN_MASK;
     break;
    #endif /*     Si2164B_MASTER_IEN_PROP */
    #ifdef        Si2164B_MCNS_ADC_CREST_FACTOR_PROP
     case         Si2164B_MCNS_ADC_CREST_FACTOR_PROP_CODE:
               prop->mcns_adc_crest_factor.crest_factor = (data >> Si2164B_MCNS_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB) & Si2164B_MCNS_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK;
     break;
    #endif /*     Si2164B_MCNS_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_MCNS_AFC_RANGE_PROP
     case         Si2164B_MCNS_AFC_RANGE_PROP_CODE:
               prop->mcns_afc_range.range_khz = (data >> Si2164B_MCNS_AFC_RANGE_PROP_RANGE_KHZ_LSB) & Si2164B_MCNS_AFC_RANGE_PROP_RANGE_KHZ_MASK;
     break;
    #endif /*     Si2164B_MCNS_AFC_RANGE_PROP */
    #ifdef        Si2164B_MCNS_CONSTELLATION_PROP
     case         Si2164B_MCNS_CONSTELLATION_PROP_CODE:
               prop->mcns_constellation.constellation = (data >> Si2164B_MCNS_CONSTELLATION_PROP_CONSTELLATION_LSB) & Si2164B_MCNS_CONSTELLATION_PROP_CONSTELLATION_MASK;
     break;
    #endif /*     Si2164B_MCNS_CONSTELLATION_PROP */
    #ifdef        Si2164B_MCNS_SYMBOL_RATE_PROP
     case         Si2164B_MCNS_SYMBOL_RATE_PROP_CODE:
               prop->mcns_symbol_rate.rate = (data >> Si2164B_MCNS_SYMBOL_RATE_PROP_RATE_LSB) & Si2164B_MCNS_SYMBOL_RATE_PROP_RATE_MASK;
     break;
    #endif /*     Si2164B_MCNS_SYMBOL_RATE_PROP */

    #ifdef        Si2164B_SCAN_FMAX_PROP
     case         Si2164B_SCAN_FMAX_PROP_CODE:
               prop->scan_fmax.scan_fmax = (data >> Si2164B_SCAN_FMAX_PROP_SCAN_FMAX_LSB) & Si2164B_SCAN_FMAX_PROP_SCAN_FMAX_MASK;
     break;
    #endif /*     Si2164B_SCAN_FMAX_PROP */
    #ifdef        Si2164B_SCAN_FMIN_PROP
     case         Si2164B_SCAN_FMIN_PROP_CODE:
               prop->scan_fmin.scan_fmin = (data >> Si2164B_SCAN_FMIN_PROP_SCAN_FMIN_LSB) & Si2164B_SCAN_FMIN_PROP_SCAN_FMIN_MASK;
     break;
    #endif /*     Si2164B_SCAN_FMIN_PROP */
    #ifdef        Si2164B_SCAN_IEN_PROP
     case         Si2164B_SCAN_IEN_PROP_CODE:
               prop->scan_ien.buzien = (data >> Si2164B_SCAN_IEN_PROP_BUZIEN_LSB) & Si2164B_SCAN_IEN_PROP_BUZIEN_MASK;
               prop->scan_ien.reqien = (data >> Si2164B_SCAN_IEN_PROP_REQIEN_LSB) & Si2164B_SCAN_IEN_PROP_REQIEN_MASK;
     break;
    #endif /*     Si2164B_SCAN_IEN_PROP */
    #ifdef        Si2164B_SCAN_INT_SENSE_PROP
     case         Si2164B_SCAN_INT_SENSE_PROP_CODE:
               prop->scan_int_sense.buznegen = (data >> Si2164B_SCAN_INT_SENSE_PROP_BUZNEGEN_LSB) & Si2164B_SCAN_INT_SENSE_PROP_BUZNEGEN_MASK;
               prop->scan_int_sense.reqnegen = (data >> Si2164B_SCAN_INT_SENSE_PROP_REQNEGEN_LSB) & Si2164B_SCAN_INT_SENSE_PROP_REQNEGEN_MASK;
               prop->scan_int_sense.buzposen = (data >> Si2164B_SCAN_INT_SENSE_PROP_BUZPOSEN_LSB) & Si2164B_SCAN_INT_SENSE_PROP_BUZPOSEN_MASK;
               prop->scan_int_sense.reqposen = (data >> Si2164B_SCAN_INT_SENSE_PROP_REQPOSEN_LSB) & Si2164B_SCAN_INT_SENSE_PROP_REQPOSEN_MASK;
     break;
    #endif /*     Si2164B_SCAN_INT_SENSE_PROP */
    #ifdef        Si2164B_SCAN_SAT_CONFIG_PROP
     case         Si2164B_SCAN_SAT_CONFIG_PROP_CODE:
               prop->scan_sat_config.analog_detect = (data >> Si2164B_SCAN_SAT_CONFIG_PROP_ANALOG_DETECT_LSB) & Si2164B_SCAN_SAT_CONFIG_PROP_ANALOG_DETECT_MASK;
               prop->scan_sat_config.reserved1     = (data >> Si2164B_SCAN_SAT_CONFIG_PROP_RESERVED1_LSB    ) & Si2164B_SCAN_SAT_CONFIG_PROP_RESERVED1_MASK;
               prop->scan_sat_config.reserved2     = (data >> Si2164B_SCAN_SAT_CONFIG_PROP_RESERVED2_LSB    ) & Si2164B_SCAN_SAT_CONFIG_PROP_RESERVED2_MASK;
     break;
    #endif /*     Si2164B_SCAN_SAT_CONFIG_PROP */
    #ifdef        Si2164B_SCAN_SAT_UNICABLE_BW_PROP
     case         Si2164B_SCAN_SAT_UNICABLE_BW_PROP_CODE:
               prop->scan_sat_unicable_bw.scan_sat_unicable_bw = (data >> Si2164B_SCAN_SAT_UNICABLE_BW_PROP_SCAN_SAT_UNICABLE_BW_LSB) & Si2164B_SCAN_SAT_UNICABLE_BW_PROP_SCAN_SAT_UNICABLE_BW_MASK;
     break;
    #endif /*     Si2164B_SCAN_SAT_UNICABLE_BW_PROP */
    #ifdef        Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP
     case         Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP_CODE:
               prop->scan_sat_unicable_min_tune_step.scan_sat_unicable_min_tune_step = (data >> Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_LSB) & Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_MASK;
     break;
    #endif /*     Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP */

    #ifdef        Si2164B_SCAN_SYMB_RATE_MAX_PROP
     case         Si2164B_SCAN_SYMB_RATE_MAX_PROP_CODE:
               prop->scan_symb_rate_max.scan_symb_rate_max = (data >> Si2164B_SCAN_SYMB_RATE_MAX_PROP_SCAN_SYMB_RATE_MAX_LSB) & Si2164B_SCAN_SYMB_RATE_MAX_PROP_SCAN_SYMB_RATE_MAX_MASK;
     break;
    #endif /*     Si2164B_SCAN_SYMB_RATE_MAX_PROP */
    #ifdef        Si2164B_SCAN_SYMB_RATE_MIN_PROP
     case         Si2164B_SCAN_SYMB_RATE_MIN_PROP_CODE:
               prop->scan_symb_rate_min.scan_symb_rate_min = (data >> Si2164B_SCAN_SYMB_RATE_MIN_PROP_SCAN_SYMB_RATE_MIN_LSB) & Si2164B_SCAN_SYMB_RATE_MIN_PROP_SCAN_SYMB_RATE_MIN_MASK;
     break;
    #endif /*     Si2164B_SCAN_SYMB_RATE_MIN_PROP */
    #ifdef        Si2164B_SCAN_TER_CONFIG_PROP
     case         Si2164B_SCAN_TER_CONFIG_PROP_CODE:
               prop->scan_ter_config.mode          = (data >> Si2164B_SCAN_TER_CONFIG_PROP_MODE_LSB         ) & Si2164B_SCAN_TER_CONFIG_PROP_MODE_MASK;
               prop->scan_ter_config.analog_bw     = (data >> Si2164B_SCAN_TER_CONFIG_PROP_ANALOG_BW_LSB    ) & Si2164B_SCAN_TER_CONFIG_PROP_ANALOG_BW_MASK;
               prop->scan_ter_config.search_analog = (data >> Si2164B_SCAN_TER_CONFIG_PROP_SEARCH_ANALOG_LSB) & Si2164B_SCAN_TER_CONFIG_PROP_SEARCH_ANALOG_MASK;
               prop->scan_ter_config.scan_debug    = (data >> Si2164B_SCAN_TER_CONFIG_PROP_SCAN_DEBUG_LSB   ) & Si2164B_SCAN_TER_CONFIG_PROP_SCAN_DEBUG_MASK;
     break;
    #endif /*     Si2164B_SCAN_TER_CONFIG_PROP */

     default : return ERROR_Si2164B_UNKNOWN_PROPERTY; break;
    }
    return NO_Si2164B_ERROR;
}
/***********************************************************************************************************************
  Si2164B_storePropertiesDefaults function
  Use:        property defaults function
              Used to fill the propShadow structure with startup values.
  Parameter: *prop     the Si2164B_PropObject structure

 |---------------------------------------------------------------------------------------------------------------------|
 | Do NOT change this code unless you really know what you're doing!                                                   |
 | It should reflect the part internal property settings after firmware download                                       |
 |---------------------------------------------------------------------------------------------------------------------|

 Returns:    void
 ***********************************************************************************************************************/
void          Si2164B_storePropertiesDefaults (Si2164B_PropObj   *prop) {
#ifdef    Si2164B_MASTER_IEN_PROP
  prop->master_ien.ddien                                                 = Si2164B_MASTER_IEN_PROP_DDIEN_OFF   ; /* (default 'OFF') */
  prop->master_ien.scanien                                               = Si2164B_MASTER_IEN_PROP_SCANIEN_OFF ; /* (default 'OFF') */
  prop->master_ien.errien                                                = Si2164B_MASTER_IEN_PROP_ERRIEN_OFF  ; /* (default 'OFF') */
  prop->master_ien.ctsien                                                = Si2164B_MASTER_IEN_PROP_CTSIEN_OFF  ; /* (default 'OFF') */
#endif /* Si2164B_MASTER_IEN_PROP */

#ifdef    Si2164B_DD_BER_RESOL_PROP
  prop->dd_ber_resol.exp                                                 =     7; /* (default     7) */
  prop->dd_ber_resol.mant                                                =     1; /* (default     1) */
#endif /* Si2164B_DD_BER_RESOL_PROP */

#ifdef    Si2164B_DD_CBER_RESOL_PROP
  prop->dd_cber_resol.exp                                                =     5; /* (default     5) */
  prop->dd_cber_resol.mant                                               =     1; /* (default     1) */
#endif /* Si2164B_DD_CBER_RESOL_PROP */

#ifdef    Si2164B_DD_DISEQC_FREQ_PROP
  prop->dd_diseqc_freq.freq_hz                                           = 22000; /* (default 22000) */
#endif /* Si2164B_DD_DISEQC_FREQ_PROP */

#ifdef    Si2164B_DD_DISEQC_PARAM_PROP
  prop->dd_diseqc_param.sequence_mode                                    = Si2164B_DD_DISEQC_PARAM_PROP_SEQUENCE_MODE_GAP       ; /* (default 'GAP') */
  prop->dd_diseqc_param.input_pin                                        = Si2164B_DD_DISEQC_PARAM_PROP_INPUT_PIN_DISEQC_IN     ; /* (default 'DISEQC_IN') */
#endif /* Si2164B_DD_DISEQC_PARAM_PROP */

#ifdef    Si2164B_DD_FER_RESOL_PROP
  prop->dd_fer_resol.exp                                                 =     3; /* (default     3) */
  prop->dd_fer_resol.mant                                                =     1; /* (default     1) */
#endif /* Si2164B_DD_FER_RESOL_PROP */

#ifdef    Si2164B_DD_IEN_PROP
  prop->dd_ien.ien_bit0                                                  = Si2164B_DD_IEN_PROP_IEN_BIT0_DISABLE ; /* (default 'DISABLE') */
  prop->dd_ien.ien_bit1                                                  = Si2164B_DD_IEN_PROP_IEN_BIT1_DISABLE ; /* (default 'DISABLE') */
  prop->dd_ien.ien_bit2                                                  = Si2164B_DD_IEN_PROP_IEN_BIT2_DISABLE ; /* (default 'DISABLE') */
  prop->dd_ien.ien_bit3                                                  = Si2164B_DD_IEN_PROP_IEN_BIT3_DISABLE ; /* (default 'DISABLE') */
  prop->dd_ien.ien_bit4                                                  = Si2164B_DD_IEN_PROP_IEN_BIT4_DISABLE ; /* (default 'DISABLE') */
  prop->dd_ien.ien_bit5                                                  = Si2164B_DD_IEN_PROP_IEN_BIT5_DISABLE ; /* (default 'DISABLE') */
  prop->dd_ien.ien_bit6                                                  = Si2164B_DD_IEN_PROP_IEN_BIT6_DISABLE ; /* (default 'DISABLE') */
  prop->dd_ien.ien_bit7                                                  = Si2164B_DD_IEN_PROP_IEN_BIT7_DISABLE ; /* (default 'DISABLE') */
#endif /* Si2164B_DD_IEN_PROP */

#ifdef    Si2164B_DD_IF_INPUT_FREQ_PROP
  prop->dd_if_input_freq.offset                                          =  5000; /* (default  5000) */
#endif /* Si2164B_DD_IF_INPUT_FREQ_PROP */

#ifdef    Si2164B_DD_INT_SENSE_PROP
  prop->dd_int_sense.neg_bit0                                            = Si2164B_DD_INT_SENSE_PROP_NEG_BIT0_DISABLE ; /* (default 'DISABLE') */
  prop->dd_int_sense.neg_bit1                                            = Si2164B_DD_INT_SENSE_PROP_NEG_BIT1_DISABLE ; /* (default 'DISABLE') */
  prop->dd_int_sense.neg_bit2                                            = Si2164B_DD_INT_SENSE_PROP_NEG_BIT2_DISABLE ; /* (default 'DISABLE') */
  prop->dd_int_sense.neg_bit3                                            = Si2164B_DD_INT_SENSE_PROP_NEG_BIT3_DISABLE ; /* (default 'DISABLE') */
  prop->dd_int_sense.neg_bit4                                            = Si2164B_DD_INT_SENSE_PROP_NEG_BIT4_DISABLE ; /* (default 'DISABLE') */
  prop->dd_int_sense.neg_bit5                                            = Si2164B_DD_INT_SENSE_PROP_NEG_BIT5_DISABLE ; /* (default 'DISABLE') */
  prop->dd_int_sense.neg_bit6                                            = Si2164B_DD_INT_SENSE_PROP_NEG_BIT6_DISABLE ; /* (default 'DISABLE') */
  prop->dd_int_sense.neg_bit7                                            = Si2164B_DD_INT_SENSE_PROP_NEG_BIT7_DISABLE ; /* (default 'DISABLE') */
  prop->dd_int_sense.pos_bit0                                            = Si2164B_DD_INT_SENSE_PROP_POS_BIT0_ENABLE  ; /* (default 'ENABLE') */
  prop->dd_int_sense.pos_bit1                                            = Si2164B_DD_INT_SENSE_PROP_POS_BIT1_ENABLE  ; /* (default 'ENABLE') */
  prop->dd_int_sense.pos_bit2                                            = Si2164B_DD_INT_SENSE_PROP_POS_BIT2_ENABLE  ; /* (default 'ENABLE') */
  prop->dd_int_sense.pos_bit3                                            = Si2164B_DD_INT_SENSE_PROP_POS_BIT3_ENABLE  ; /* (default 'ENABLE') */
  prop->dd_int_sense.pos_bit4                                            = Si2164B_DD_INT_SENSE_PROP_POS_BIT4_ENABLE  ; /* (default 'ENABLE') */
  prop->dd_int_sense.pos_bit5                                            = Si2164B_DD_INT_SENSE_PROP_POS_BIT5_ENABLE  ; /* (default 'ENABLE') */
  prop->dd_int_sense.pos_bit6                                            = Si2164B_DD_INT_SENSE_PROP_POS_BIT6_ENABLE  ; /* (default 'ENABLE') */
  prop->dd_int_sense.pos_bit7                                            = Si2164B_DD_INT_SENSE_PROP_POS_BIT7_ENABLE  ; /* (default 'ENABLE') */
#endif /* Si2164B_DD_INT_SENSE_PROP */

#ifdef    Si2164B_DD_MODE_PROP
  prop->dd_mode.bw                                                       = Si2164B_DD_MODE_PROP_BW_BW_8MHZ              ; /* (default 'BW_8MHZ') */
  prop->dd_mode.modulation                                               = Si2164B_DD_MODE_PROP_MODULATION_DVBT         ; /* (default 'DVBT') */
  prop->dd_mode.invert_spectrum                                          = Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_NORMAL  ; /* (default 'NORMAL') */
  prop->dd_mode.auto_detect                                              = Si2164B_DD_MODE_PROP_AUTO_DETECT_NONE        ; /* (default 'NONE') */
#endif /* Si2164B_DD_MODE_PROP */

#ifdef    Si2164B_DD_PER_RESOL_PROP
  prop->dd_per_resol.exp                                                 =     5; /* (default     5) */
  prop->dd_per_resol.mant                                                =     1; /* (default     1) */
#endif /* Si2164B_DD_PER_RESOL_PROP */

#ifdef    Si2164B_DD_RSQ_BER_THRESHOLD_PROP
  prop->dd_rsq_ber_threshold.exp                                         =     1; /* (default     1) */
  prop->dd_rsq_ber_threshold.mant                                        =    10; /* (default    10) */
#endif /* Si2164B_DD_RSQ_BER_THRESHOLD_PROP */

#ifdef    Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP
  prop->dd_sec_ts_serial_diff.ts_data1_strength                          =    15; /* (default    15) */
  prop->dd_sec_ts_serial_diff.ts_data1_shape                             =     3; /* (default     3) */
  prop->dd_sec_ts_serial_diff.ts_data2_strength                          =    15; /* (default    15) */
  prop->dd_sec_ts_serial_diff.ts_data2_shape                             =     3; /* (default     3) */
  prop->dd_sec_ts_serial_diff.ts_clkb_on_data1                           = Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_DISABLE   ; /* (default 'DISABLE') */
  prop->dd_sec_ts_serial_diff.ts_data0b_on_data2                         = Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_DISABLE ; /* (default 'DISABLE') */
#endif /* Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP */

#ifdef    Si2164B_DD_SEC_TS_SETUP_PAR_PROP
  prop->dd_sec_ts_setup_par.ts_data_strength                             =     3; /* (default     3) */
  prop->dd_sec_ts_setup_par.ts_data_shape                                =     1; /* (default     1) */
  prop->dd_sec_ts_setup_par.ts_clk_strength                              =     3; /* (default     3) */
  prop->dd_sec_ts_setup_par.ts_clk_shape                                 =     1; /* (default     1) */
#endif /* Si2164B_DD_SEC_TS_SETUP_PAR_PROP */

#ifdef    Si2164B_DD_SEC_TS_SETUP_SER_PROP
  prop->dd_sec_ts_setup_ser.ts_data_strength                             =     3; /* (default     3) */
  prop->dd_sec_ts_setup_ser.ts_data_shape                                =     3; /* (default     3) */
  prop->dd_sec_ts_setup_ser.ts_clk_strength                              =    15; /* (default    15) */
  prop->dd_sec_ts_setup_ser.ts_clk_shape                                 =     3; /* (default     3) */
#endif /* Si2164B_DD_SEC_TS_SETUP_SER_PROP */

#ifdef    Si2164B_DD_SEC_TS_SLR_SERIAL_PROP
  prop->dd_sec_ts_slr_serial.ts_data_slr                                 =     0; /* (default     0) */
  prop->dd_sec_ts_slr_serial.ts_data_slr_on                              = Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_DISABLE  ; /* (default 'DISABLE') */
  prop->dd_sec_ts_slr_serial.ts_data1_slr                                =     0; /* (default     0) */
  prop->dd_sec_ts_slr_serial.ts_data1_slr_on                             = Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_DISABLE ; /* (default 'DISABLE') */
  prop->dd_sec_ts_slr_serial.ts_data2_slr                                =     0; /* (default     0) */
  prop->dd_sec_ts_slr_serial.ts_data2_slr_on                             = Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_DISABLE ; /* (default 'DISABLE') */
  prop->dd_sec_ts_slr_serial.ts_clk_slr                                  =     0; /* (default     0) */
  prop->dd_sec_ts_slr_serial.ts_clk_slr_on                               = Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_DISABLE   ; /* (default 'DISABLE') */
#endif /* Si2164B_DD_SEC_TS_SLR_SERIAL_PROP */

#ifdef    Si2164B_DD_SSI_SQI_PARAM_PROP
  prop->dd_ssi_sqi_param.sqi_average                                     =     1; /* (default     1) */
#endif /* Si2164B_DD_SSI_SQI_PARAM_PROP */

#ifdef    Si2164B_DD_TS_FREQ_PROP
  prop->dd_ts_freq.req_freq_10khz                                        =   720; /* (default   720) */
#endif /* Si2164B_DD_TS_FREQ_PROP */

#ifdef    Si2164B_DD_TS_FREQ_MAX_PROP
  prop->dd_ts_freq_max.req_freq_max_10khz                                = 14550; /* (default 14550) */
#endif /* Si2164B_DD_TS_FREQ_MAX_PROP */

#ifdef    Si2164B_DD_TS_MODE_PROP
  prop->dd_ts_mode.mode                                                  = Si2164B_DD_TS_MODE_PROP_MODE_TRISTATE                   ; /* (default 'TRISTATE') */
  prop->dd_ts_mode.clock                                                 = Si2164B_DD_TS_MODE_PROP_CLOCK_AUTO_FIXED                ; /* (default 'AUTO_FIXED') */
  prop->dd_ts_mode.clk_gapped_en                                         = Si2164B_DD_TS_MODE_PROP_CLK_GAPPED_EN_DISABLED          ; /* (default 'DISABLED') */
  prop->dd_ts_mode.ts_err_polarity                                       = Si2164B_DD_TS_MODE_PROP_TS_ERR_POLARITY_NOT_INVERTED    ; /* (default 'NOT_INVERTED') */
  prop->dd_ts_mode.special                                               = Si2164B_DD_TS_MODE_PROP_SPECIAL_FULL_TS                 ; /* (default 'FULL_TS') */
  prop->dd_ts_mode.ts_freq_resolution                                    = Si2164B_DD_TS_MODE_PROP_TS_FREQ_RESOLUTION_NORMAL       ; /* (default 'NORMAL') */
  prop->dd_ts_mode.serial_pin_selection                                  = Si2164B_DD_TS_MODE_PROP_SERIAL_PIN_SELECTION_D0           ; /* (default 'D0') */
#endif /* Si2164B_DD_TS_MODE_PROP */

#ifdef    Si2164B_DD_TS_SERIAL_DIFF_PROP
  prop->dd_ts_serial_diff.ts_data1_strength                              =    15; /* (default    15) */
  prop->dd_ts_serial_diff.ts_data1_shape                                 =     3; /* (default     3) */
  prop->dd_ts_serial_diff.ts_data2_strength                              =    15; /* (default    15) */
  prop->dd_ts_serial_diff.ts_data2_shape                                 =     3; /* (default     3) */
  prop->dd_ts_serial_diff.ts_clkb_on_data1                               = Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_DISABLE   ; /* (default 'DISABLE') */
  prop->dd_ts_serial_diff.ts_data0b_on_data2                             = Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_DISABLE ; /* (default 'DISABLE') */
#endif /* Si2164B_DD_TS_SERIAL_DIFF_PROP */

#ifdef    Si2164B_DD_TS_SETUP_PAR_PROP
  prop->dd_ts_setup_par.ts_data_strength                                 =     3; /* (default     3) */
  prop->dd_ts_setup_par.ts_data_shape                                    =     1; /* (default     1) */
  prop->dd_ts_setup_par.ts_clk_strength                                  =     3; /* (default     3) */
  prop->dd_ts_setup_par.ts_clk_shape                                     =     1; /* (default     1) */
  prop->dd_ts_setup_par.ts_clk_invert                                    = Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_INVERT_INVERTED    ; /* (default 'INVERTED') */
  prop->dd_ts_setup_par.ts_clk_shift                                     =     0; /* (default     0) */
#endif /* Si2164B_DD_TS_SETUP_PAR_PROP */

#ifdef    Si2164B_DD_TS_SETUP_SER_PROP
  prop->dd_ts_setup_ser.ts_data_strength                                 =    15; /* (default    15) */
  prop->dd_ts_setup_ser.ts_data_shape                                    =     3; /* (default     3) */
  prop->dd_ts_setup_ser.ts_clk_strength                                  =    15; /* (default    15) */
  prop->dd_ts_setup_ser.ts_clk_shape                                     =     3; /* (default     3) */
  prop->dd_ts_setup_ser.ts_clk_invert                                    = Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_INVERT_INVERTED      ; /* (default 'INVERTED') */
  prop->dd_ts_setup_ser.ts_sync_duration                                 = Si2164B_DD_TS_SETUP_SER_PROP_TS_SYNC_DURATION_FIRST_BYTE ; /* (default 'FIRST_BYTE') */
  prop->dd_ts_setup_ser.ts_byte_order                                    = Si2164B_DD_TS_SETUP_SER_PROP_TS_BYTE_ORDER_MSB_FIRST     ; /* (default 'MSB_FIRST') */
#endif /* Si2164B_DD_TS_SETUP_SER_PROP */

#ifdef    Si2164B_DD_TS_SLR_SERIAL_PROP
  prop->dd_ts_slr_serial.ts_data_slr                                     =     0; /* (default     0) */
  prop->dd_ts_slr_serial.ts_data_slr_on                                  = Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_DISABLE  ; /* (default 'DISABLE') */
  prop->dd_ts_slr_serial.ts_data1_slr                                    =     0; /* (default     0) */
  prop->dd_ts_slr_serial.ts_data1_slr_on                                 = Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_DISABLE ; /* (default 'DISABLE') */
  prop->dd_ts_slr_serial.ts_data2_slr                                    =     0; /* (default     0) */
  prop->dd_ts_slr_serial.ts_data2_slr_on                                 = Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_DISABLE ; /* (default 'DISABLE') */
  prop->dd_ts_slr_serial.ts_clk_slr                                      =     0; /* (default     0) */
  prop->dd_ts_slr_serial.ts_clk_slr_on                                   = Si2164B_DD_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_DISABLE   ; /* (default 'DISABLE') */
#endif /* Si2164B_DD_TS_SLR_SERIAL_PROP */

#ifdef    Si2164B_DVBC_ADC_CREST_FACTOR_PROP
  prop->dvbc_adc_crest_factor.crest_factor                               =   112; /* (default   112) */
#endif /* Si2164B_DVBC_ADC_CREST_FACTOR_PROP */

#ifdef    Si2164B_DVBC_AFC_RANGE_PROP
  prop->dvbc_afc_range.range_khz                                         =   100; /* (default   100) */
#endif /* Si2164B_DVBC_AFC_RANGE_PROP */

#ifdef    Si2164B_DVBC_CONSTELLATION_PROP
  prop->dvbc_constellation.constellation                                 = Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_AUTO ; /* (default 'AUTO') */
#endif /* Si2164B_DVBC_CONSTELLATION_PROP */

#ifdef    Si2164B_DVBC_SYMBOL_RATE_PROP
  prop->dvbc_symbol_rate.rate                                            =  6900; /* (default  6900) */
#endif /* Si2164B_DVBC_SYMBOL_RATE_PROP */


#ifdef    Si2164B_DVBC2_ADC_CREST_FACTOR_PROP
  prop->dvbc2_adc_crest_factor.crest_factor                              =   130; /* (default   130) */
#endif /* Si2164B_DVBC2_ADC_CREST_FACTOR_PROP */

#ifdef    Si2164B_DVBC2_AFC_RANGE_PROP
  prop->dvbc2_afc_range.range_khz                                        =   550; /* (default   550) */
#endif /* Si2164B_DVBC2_AFC_RANGE_PROP */


#ifdef    Si2164B_DVBS_ADC_CREST_FACTOR_PROP
  prop->dvbs_adc_crest_factor.crest_factor                               =   104; /* (default   104) */
#endif /* Si2164B_DVBS_ADC_CREST_FACTOR_PROP */

#ifdef    Si2164B_DVBS_AFC_RANGE_PROP
  prop->dvbs_afc_range.range_khz                                         =  4000; /* (default  4000) */
#endif /* Si2164B_DVBS_AFC_RANGE_PROP */

#ifdef    Si2164B_DVBS_SYMBOL_RATE_PROP
  prop->dvbs_symbol_rate.rate                                            = 27500; /* (default 27500) */
#endif /* Si2164B_DVBS_SYMBOL_RATE_PROP */

#ifdef    Si2164B_DVBS2_ADC_CREST_FACTOR_PROP
  prop->dvbs2_adc_crest_factor.crest_factor                              =   104; /* (default   104) */
#endif /* Si2164B_DVBS2_ADC_CREST_FACTOR_PROP */

#ifdef    Si2164B_DVBS2_AFC_RANGE_PROP
  prop->dvbs2_afc_range.range_khz                                        =  4000; /* (default  4000) */
#endif /* Si2164B_DVBS2_AFC_RANGE_PROP */

#ifdef    Si2164B_DVBS2_SYMBOL_RATE_PROP
  prop->dvbs2_symbol_rate.rate                                           = 27500; /* (default 27500) */
#endif /* Si2164B_DVBS2_SYMBOL_RATE_PROP */


#ifdef    Si2164B_DVBT_ADC_CREST_FACTOR_PROP
  prop->dvbt_adc_crest_factor.crest_factor                               =   130; /* (default   130) */
#endif /* Si2164B_DVBT_ADC_CREST_FACTOR_PROP */

#ifdef    Si2164B_DVBT_AFC_RANGE_PROP
  prop->dvbt_afc_range.range_khz                                         =   550; /* (default   550) */
#endif /* Si2164B_DVBT_AFC_RANGE_PROP */

#ifdef    Si2164B_DVBT_HIERARCHY_PROP
  prop->dvbt_hierarchy.stream                                            = Si2164B_DVBT_HIERARCHY_PROP_STREAM_HP ; /* (default 'HP') */
#endif /* Si2164B_DVBT_HIERARCHY_PROP */


#ifdef    Si2164B_DVBT2_ADC_CREST_FACTOR_PROP
  prop->dvbt2_adc_crest_factor.crest_factor                              =   130; /* (default   130) */
#endif /* Si2164B_DVBT2_ADC_CREST_FACTOR_PROP */

#ifdef    Si2164B_DVBT2_AFC_RANGE_PROP
  prop->dvbt2_afc_range.range_khz                                        =   550; /* (default   550) */
#endif /* Si2164B_DVBT2_AFC_RANGE_PROP */

#ifdef    Si2164B_DVBT2_FEF_TUNER_PROP
  prop->dvbt2_fef_tuner.tuner_delay                                      =     1; /* (default     1) */
  prop->dvbt2_fef_tuner.tuner_freeze_time                                =     1; /* (default     1) */
  prop->dvbt2_fef_tuner.tuner_unfreeze_time                              =     1; /* (default     1) */
#endif /* Si2164B_DVBT2_FEF_TUNER_PROP */

#ifdef    Si2164B_DVBT2_MODE_PROP
  prop->dvbt2_mode.lock_mode                                             = Si2164B_DVBT2_MODE_PROP_LOCK_MODE_ANY ; /* (default 'ANY') */
#endif /* Si2164B_DVBT2_MODE_PROP */



#ifdef    Si2164B_MCNS_ADC_CREST_FACTOR_PROP
  prop->mcns_adc_crest_factor.crest_factor                               =   112; /* (default   112) */
#endif /* Si2164B_MCNS_ADC_CREST_FACTOR_PROP */

#ifdef    Si2164B_MCNS_AFC_RANGE_PROP
  prop->mcns_afc_range.range_khz                                         =   100; /* (default   100) */
#endif /* Si2164B_MCNS_AFC_RANGE_PROP */

#ifdef    Si2164B_MCNS_CONSTELLATION_PROP
  prop->mcns_constellation.constellation                                 = Si2164B_MCNS_CONSTELLATION_PROP_CONSTELLATION_AUTO ; /* (default 'AUTO') */
#endif /* Si2164B_MCNS_CONSTELLATION_PROP */

#ifdef    Si2164B_MCNS_SYMBOL_RATE_PROP
  prop->mcns_symbol_rate.rate                                            =  5361; /* (default  5361) */
#endif /* Si2164B_MCNS_SYMBOL_RATE_PROP */


#ifdef    Si2164B_SCAN_FMAX_PROP
  prop->scan_fmax.scan_fmax                                              =     0; /* (default     0) */
#endif /* Si2164B_SCAN_FMAX_PROP */

#ifdef    Si2164B_SCAN_FMIN_PROP
  prop->scan_fmin.scan_fmin                                              =     0; /* (default     0) */
#endif /* Si2164B_SCAN_FMIN_PROP */

#ifdef    Si2164B_SCAN_IEN_PROP
  prop->scan_ien.buzien                                                  = Si2164B_SCAN_IEN_PROP_BUZIEN_DISABLE ; /* (default 'DISABLE') */
  prop->scan_ien.reqien                                                  = Si2164B_SCAN_IEN_PROP_REQIEN_DISABLE ; /* (default 'DISABLE') */
#endif /* Si2164B_SCAN_IEN_PROP */

#ifdef    Si2164B_SCAN_INT_SENSE_PROP
  prop->scan_int_sense.buznegen                                          = Si2164B_SCAN_INT_SENSE_PROP_BUZNEGEN_ENABLE  ; /* (default 'ENABLE') */
  prop->scan_int_sense.reqnegen                                          = Si2164B_SCAN_INT_SENSE_PROP_REQNEGEN_DISABLE ; /* (default 'DISABLE') */
  prop->scan_int_sense.buzposen                                          = Si2164B_SCAN_INT_SENSE_PROP_BUZPOSEN_DISABLE ; /* (default 'DISABLE') */
  prop->scan_int_sense.reqposen                                          = Si2164B_SCAN_INT_SENSE_PROP_REQPOSEN_ENABLE  ; /* (default 'ENABLE') */
#endif /* Si2164B_SCAN_INT_SENSE_PROP */

#ifdef    Si2164B_SCAN_SAT_CONFIG_PROP
  prop->scan_sat_config.analog_detect                                    = Si2164B_SCAN_SAT_CONFIG_PROP_ANALOG_DETECT_DISABLED ; /* (default 'DISABLED') */
  prop->scan_sat_config.reserved1                                        =     0; /* (default     0) */
  prop->scan_sat_config.reserved2                                        =    12; /* (default    12) */
#endif /* Si2164B_SCAN_SAT_CONFIG_PROP */

#ifdef    Si2164B_SCAN_SAT_UNICABLE_BW_PROP
  prop->scan_sat_unicable_bw.scan_sat_unicable_bw                        =     0; /* (default     0) */
#endif /* Si2164B_SCAN_SAT_UNICABLE_BW_PROP */

#ifdef    Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP
  prop->scan_sat_unicable_min_tune_step.scan_sat_unicable_min_tune_step  =    50; /* (default    50) */
#endif /* Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP */

#ifdef    Si2164B_SCAN_SYMB_RATE_MAX_PROP
  prop->scan_symb_rate_max.scan_symb_rate_max                            =     0; /* (default     0) */
#endif /* Si2164B_SCAN_SYMB_RATE_MAX_PROP */

#ifdef    Si2164B_SCAN_SYMB_RATE_MIN_PROP
  prop->scan_symb_rate_min.scan_symb_rate_min                            =     0; /* (default     0) */
#endif /* Si2164B_SCAN_SYMB_RATE_MIN_PROP */

#ifdef    Si2164B_SCAN_TER_CONFIG_PROP
  prop->scan_ter_config.mode                                             = Si2164B_SCAN_TER_CONFIG_PROP_MODE_MAPPING_SCAN        ; /* (default 'BLIND_SCAN') */
  prop->scan_ter_config.analog_bw                                        = Si2164B_SCAN_TER_CONFIG_PROP_ANALOG_BW_8MHZ           ; /* (default '8MHZ') */
  prop->scan_ter_config.search_analog                                    = Si2164B_SCAN_TER_CONFIG_PROP_SEARCH_ANALOG_DISABLE    ; /* (default 'DISABLE') */
#endif /* Si2164B_SCAN_TER_CONFIG_PROP */

}
#ifdef    Si2164B_GET_PROPERTY_STRING
/***********************************************************************************************************************
  Si2164B_L1_PropertyText function
  Use:        property text function
              Used to turn the property data into clear text.
  Parameter: *prop     the Si2164B property structure (containing all properties)
  Parameter: prop_code the property Id (used to know which property to use)
  Parameter: separator the string to use between fields (often either a blank or a newline character)
  Parameter: msg       the string used to store the resulting string
                       It must be declared by the caller with a size of 1000 bytes
  Returns:    NO_Si2164B_ERROR if successful.
 ***********************************************************************************************************************/
unsigned char Si2164B_L1_PropertyText          (Si2164B_PropObj   *prop, unsigned int prop_code, const unsigned char *separator, unsigned char *msg) {
    switch (prop_code) {
    #ifdef        Si2164B_DD_BER_RESOL_PROP
     case         Si2164B_DD_BER_RESOL_PROP_CODE:
      safe_sprintf(msg,"DD_BER_RESOL");
       safe_strcat(msg,separator); safe_strcat(msg,"-EXP "); safe_sprintf(msg,"%s%d", msg, prop->dd_ber_resol.exp);
       safe_strcat(msg,separator); safe_strcat(msg,"-MANT "); safe_sprintf(msg,"%s%d", msg, prop->dd_ber_resol.mant);
     break;
    #endif /*     Si2164B_DD_BER_RESOL_PROP */
    #ifdef        Si2164B_DD_CBER_RESOL_PROP
     case         Si2164B_DD_CBER_RESOL_PROP_CODE:
      safe_sprintf(msg,"DD_CBER_RESOL");
       safe_strcat(msg,separator); safe_strcat(msg,"-EXP "); safe_sprintf(msg,"%s%d", msg, prop->dd_cber_resol.exp);
       safe_strcat(msg,separator); safe_strcat(msg,"-MANT "); safe_sprintf(msg,"%s%d", msg, prop->dd_cber_resol.mant);
     break;
    #endif /*     Si2164B_DD_CBER_RESOL_PROP */
    #ifdef        Si2164B_DD_DISEQC_FREQ_PROP
     case         Si2164B_DD_DISEQC_FREQ_PROP_CODE:
      safe_sprintf(msg,"DD_DISEQC_FREQ");
       safe_strcat(msg,separator); safe_strcat(msg,"-FREQ_HZ "); safe_sprintf(msg,"%s%d", msg, prop->dd_diseqc_freq.freq_hz);
     break;
    #endif /*     Si2164B_DD_DISEQC_FREQ_PROP */
    #ifdef        Si2164B_DD_DISEQC_PARAM_PROP
     case         Si2164B_DD_DISEQC_PARAM_PROP_CODE:
      safe_sprintf(msg,"DD_DISEQC_PARAM");
       safe_strcat(msg,separator); safe_strcat(msg,"-SEQUENCE_MODE ");
           if  (prop->dd_diseqc_param.sequence_mode ==     0) safe_strcat(msg,"GAP    ");
      else if  (prop->dd_diseqc_param.sequence_mode ==     1) safe_strcat(msg,"NO_GAP ");
      else                                                   safe_sprintf(msg,"%s%d", msg, prop->dd_diseqc_param.sequence_mode);
       safe_strcat(msg,separator); safe_strcat(msg,"-INPUT_PIN ");
           if  (prop->dd_diseqc_param.input_pin     ==     0) safe_strcat(msg,"DISEQC_IN  ");
      else if  (prop->dd_diseqc_param.input_pin     ==     1) safe_strcat(msg,"DISEQC_CMD ");
      else                                                   safe_sprintf(msg, "%d", msg, prop->dd_diseqc_param.input_pin);
     break;
    #endif /*     Si2164B_DD_DISEQC_PARAM_PROP */

    #ifdef        Si2164B_DD_FER_RESOL_PROP
     case         Si2164B_DD_FER_RESOL_PROP_CODE:
      safe_sprintf(msg,"DD_FER_RESOL");
       safe_strcat(msg,separator); safe_strcat(msg,"-EXP "); safe_sprintf(msg,"%s%d", msg, prop->dd_fer_resol.exp);
       safe_strcat(msg,separator); safe_strcat(msg,"-MANT "); safe_sprintf(msg,"%s%d", msg, prop->dd_fer_resol.mant);
     break;
    #endif /*     Si2164B_DD_FER_RESOL_PROP */
    #ifdef        Si2164B_DD_IEN_PROP
     case         Si2164B_DD_IEN_PROP_CODE:
      safe_sprintf(msg,"DD_IEN");
       safe_strcat(msg,separator); safe_strcat(msg,"-IEN_BIT0 ");
           if  (prop->dd_ien.ien_bit0 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_ien.ien_bit0 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                     safe_sprintf(msg,"%s%d", msg, prop->dd_ien.ien_bit0);
       safe_strcat(msg,separator); safe_strcat(msg,"-IEN_BIT1 ");
           if  (prop->dd_ien.ien_bit1 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_ien.ien_bit1 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                     safe_sprintf(msg,"%s%d", msg, prop->dd_ien.ien_bit1);
       safe_strcat(msg,separator); safe_strcat(msg,"-IEN_BIT2 ");
           if  (prop->dd_ien.ien_bit2 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_ien.ien_bit2 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                     safe_sprintf(msg,"%s%d", msg, prop->dd_ien.ien_bit2);
       safe_strcat(msg,separator); safe_strcat(msg,"-IEN_BIT3 ");
           if  (prop->dd_ien.ien_bit3 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_ien.ien_bit3 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                     safe_sprintf(msg,"%s%d", msg, prop->dd_ien.ien_bit3);
       safe_strcat(msg,separator); safe_strcat(msg,"-IEN_BIT4 ");
           if  (prop->dd_ien.ien_bit4 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_ien.ien_bit4 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                     safe_sprintf(msg,"%s%d", msg, prop->dd_ien.ien_bit4);
       safe_strcat(msg,separator); safe_strcat(msg,"-IEN_BIT5 ");
           if  (prop->dd_ien.ien_bit5 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_ien.ien_bit5 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                     safe_sprintf(msg,"%s%d", msg, prop->dd_ien.ien_bit5);
       safe_strcat(msg,separator); safe_strcat(msg,"-IEN_BIT6 ");
           if  (prop->dd_ien.ien_bit6 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_ien.ien_bit6 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                     safe_sprintf(msg,"%s%d", msg, prop->dd_ien.ien_bit6);
       safe_strcat(msg,separator); safe_strcat(msg,"-IEN_BIT7 ");
           if  (prop->dd_ien.ien_bit7 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_ien.ien_bit7 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                     safe_sprintf(msg,"%s%d", msg, prop->dd_ien.ien_bit7);
     break;
    #endif /*     Si2164B_DD_IEN_PROP */
    #ifdef        Si2164B_DD_IF_INPUT_FREQ_PROP
     case         Si2164B_DD_IF_INPUT_FREQ_PROP_CODE:
      safe_sprintf(msg,"DD_IF_INPUT_FREQ");
       safe_strcat(msg,separator); safe_strcat(msg,"-OFFSET "); safe_sprintf(msg,"%s%d", msg, prop->dd_if_input_freq.offset);
     break;
    #endif /*     Si2164B_DD_IF_INPUT_FREQ_PROP */
    #ifdef        Si2164B_DD_INT_SENSE_PROP
     case         Si2164B_DD_INT_SENSE_PROP_CODE:
      safe_sprintf(msg,"DD_INT_SENSE");
       safe_strcat(msg,separator); safe_strcat(msg,"-NEG_BIT0 ");
           if  (prop->dd_int_sense.neg_bit0 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_int_sense.neg_bit0 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                           safe_sprintf(msg,"%s%d", msg, prop->dd_int_sense.neg_bit0);
       safe_strcat(msg,separator); safe_strcat(msg,"-NEG_BIT1 ");
           if  (prop->dd_int_sense.neg_bit1 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_int_sense.neg_bit1 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                           safe_sprintf(msg,"%s%d", msg, prop->dd_int_sense.neg_bit1);
       safe_strcat(msg,separator); safe_strcat(msg,"-NEG_BIT2 ");
           if  (prop->dd_int_sense.neg_bit2 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_int_sense.neg_bit2 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                           safe_sprintf(msg,"%s%d", msg, prop->dd_int_sense.neg_bit2);
       safe_strcat(msg,separator); safe_strcat(msg,"-NEG_BIT3 ");
           if  (prop->dd_int_sense.neg_bit3 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_int_sense.neg_bit3 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                           safe_sprintf(msg,"%s%d", msg, prop->dd_int_sense.neg_bit3);
       safe_strcat(msg,separator); safe_strcat(msg,"-NEG_BIT4 ");
           if  (prop->dd_int_sense.neg_bit4 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_int_sense.neg_bit4 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                           safe_sprintf(msg,"%s%d", msg, prop->dd_int_sense.neg_bit4);
       safe_strcat(msg,separator); safe_strcat(msg,"-NEG_BIT5 ");
           if  (prop->dd_int_sense.neg_bit5 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_int_sense.neg_bit5 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                           safe_sprintf(msg,"%s%d", msg, prop->dd_int_sense.neg_bit5);
       safe_strcat(msg,separator); safe_strcat(msg,"-NEG_BIT6 ");
           if  (prop->dd_int_sense.neg_bit6 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_int_sense.neg_bit6 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                           safe_sprintf(msg,"%s%d", msg, prop->dd_int_sense.neg_bit6);
       safe_strcat(msg,separator); safe_strcat(msg,"-NEG_BIT7 ");
           if  (prop->dd_int_sense.neg_bit7 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_int_sense.neg_bit7 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                           safe_sprintf(msg,"%s%d", msg, prop->dd_int_sense.neg_bit7);
       safe_strcat(msg,separator); safe_strcat(msg,"-POS_BIT0 ");
           if  (prop->dd_int_sense.pos_bit0 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_int_sense.pos_bit0 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                           safe_sprintf(msg,"%s%d", msg, prop->dd_int_sense.pos_bit0);
       safe_strcat(msg,separator); safe_strcat(msg,"-POS_BIT1 ");
           if  (prop->dd_int_sense.pos_bit1 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_int_sense.pos_bit1 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                           safe_sprintf(msg,"%s%d", msg, prop->dd_int_sense.pos_bit1);
       safe_strcat(msg,separator); safe_strcat(msg,"-POS_BIT2 ");
           if  (prop->dd_int_sense.pos_bit2 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_int_sense.pos_bit2 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                           safe_sprintf(msg,"%s%d", msg, prop->dd_int_sense.pos_bit2);
       safe_strcat(msg,separator); safe_strcat(msg,"-POS_BIT3 ");
           if  (prop->dd_int_sense.pos_bit3 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_int_sense.pos_bit3 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                           safe_sprintf(msg,"%s%d", msg, prop->dd_int_sense.pos_bit3);
       safe_strcat(msg,separator); safe_strcat(msg,"-POS_BIT4 ");
           if  (prop->dd_int_sense.pos_bit4 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_int_sense.pos_bit4 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                           safe_sprintf(msg,"%s%d", msg, prop->dd_int_sense.pos_bit4);
       safe_strcat(msg,separator); safe_strcat(msg,"-POS_BIT5 ");
           if  (prop->dd_int_sense.pos_bit5 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_int_sense.pos_bit5 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                           safe_sprintf(msg,"%s%d", msg, prop->dd_int_sense.pos_bit5);
       safe_strcat(msg,separator); safe_strcat(msg,"-POS_BIT6 ");
           if  (prop->dd_int_sense.pos_bit6 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_int_sense.pos_bit6 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                           safe_sprintf(msg,"%s%d", msg, prop->dd_int_sense.pos_bit6);
       safe_strcat(msg,separator); safe_strcat(msg,"-POS_BIT7 ");
           if  (prop->dd_int_sense.pos_bit7 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_int_sense.pos_bit7 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                           safe_sprintf(msg,"%s%d", msg, prop->dd_int_sense.pos_bit7);
     break;
    #endif /*     Si2164B_DD_INT_SENSE_PROP */
    #ifdef        Si2164B_DD_MODE_PROP
     case         Si2164B_DD_MODE_PROP_CODE:
      safe_sprintf(msg,"DD_MODE");
       safe_strcat(msg,separator); safe_strcat(msg,"-BW ");
           if  (prop->dd_mode.bw              ==     5) safe_strcat(msg,"BW_5MHZ   ");
      else if  (prop->dd_mode.bw              ==     6) safe_strcat(msg,"BW_6MHZ   ");
      else if  (prop->dd_mode.bw              ==     7) safe_strcat(msg,"BW_7MHZ   ");
      else if  (prop->dd_mode.bw              ==     8) safe_strcat(msg,"BW_8MHZ   ");
      else if  (prop->dd_mode.bw              ==     2) safe_strcat(msg,"BW_1D7MHZ ");
      else                                             safe_sprintf(msg,"%s%d", msg, prop->dd_mode.bw);
       safe_strcat(msg,separator); safe_strcat(msg,"-MODULATION ");
           if  (prop->dd_mode.modulation      ==     1) safe_strcat(msg,"MCNS        ");
      else if  (prop->dd_mode.modulation      ==     2) safe_strcat(msg,"DVBT        ");
      else if  (prop->dd_mode.modulation      ==     3) safe_strcat(msg,"DVBC        ");
      else if  (prop->dd_mode.modulation      ==     7) safe_strcat(msg,"DVBT2       ");
      else if  (prop->dd_mode.modulation      ==     8) safe_strcat(msg,"DVBS        ");
      else if  (prop->dd_mode.modulation      ==     9) safe_strcat(msg,"DVBS2       ");
      else if  (prop->dd_mode.modulation      ==    10) safe_strcat(msg,"DSS         ");
      else if  (prop->dd_mode.modulation      ==    11) safe_strcat(msg,"DVBC2       ");
      else if  (prop->dd_mode.modulation      ==    15) safe_strcat(msg,"AUTO_DETECT ");
      else                                             safe_sprintf(msg,"%s%d", msg, prop->dd_mode.modulation);
       safe_strcat(msg,separator); safe_strcat(msg,"-INVERT_SPECTRUM ");
           if  (prop->dd_mode.invert_spectrum ==     0) safe_strcat(msg,"NORMAL   ");
      else if  (prop->dd_mode.invert_spectrum ==     1) safe_strcat(msg,"INVERTED ");
      else                                             safe_sprintf(msg,"%s%d", msg, prop->dd_mode.invert_spectrum);
       safe_strcat(msg,separator); safe_strcat(msg,"-AUTO_DETECT ");
           if  (prop->dd_mode.auto_detect     ==     0) safe_strcat(msg,"NONE              ");
      else if  (prop->dd_mode.auto_detect     ==     1) safe_strcat(msg,"AUTO_DVB_T_T2     ");
      else if  (prop->dd_mode.auto_detect     ==     2) safe_strcat(msg,"AUTO_DVB_S_S2     ");
      else if  (prop->dd_mode.auto_detect     ==     3) safe_strcat(msg,"AUTO_DVB_S_S2_DSS ");
      else                                             safe_sprintf(msg,"%s%d", msg, prop->dd_mode.auto_detect);
     break;
    #endif /*     Si2164B_DD_MODE_PROP */
    #ifdef        Si2164B_DD_PER_RESOL_PROP
     case         Si2164B_DD_PER_RESOL_PROP_CODE:
      safe_sprintf(msg,"DD_PER_RESOL");
       safe_strcat(msg,separator); safe_strcat(msg,"-EXP "); safe_sprintf(msg,"%s%d", msg, prop->dd_per_resol.exp);
       safe_strcat(msg,separator); safe_strcat(msg,"-MANT "); safe_sprintf(msg,"%s%d", msg, prop->dd_per_resol.mant);
     break;
    #endif /*     Si2164B_DD_PER_RESOL_PROP */
    #ifdef        Si2164B_DD_RSQ_BER_THRESHOLD_PROP
     case         Si2164B_DD_RSQ_BER_THRESHOLD_PROP_CODE:
      safe_sprintf(msg,"DD_RSQ_BER_THRESHOLD");
       safe_strcat(msg,separator); safe_strcat(msg,"-EXP "); safe_sprintf(msg,"%s%d", msg, prop->dd_rsq_ber_threshold.exp);
       safe_strcat(msg,separator); safe_strcat(msg,"-MANT "); safe_sprintf(msg,"%s%d", msg, prop->dd_rsq_ber_threshold.mant);
     break;
    #endif /*     Si2164B_DD_RSQ_BER_THRESHOLD_PROP */
    #ifdef        Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP
     case         Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_CODE:
      safe_sprintf(msg,"DD_SEC_TS_SERIAL_DIFF");
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA1_STRENGTH "); safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_serial_diff.ts_data1_strength);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA1_SHAPE "); safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_serial_diff.ts_data1_shape);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA2_STRENGTH "); safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_serial_diff.ts_data2_strength);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA2_SHAPE "); safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_serial_diff.ts_data2_shape);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_CLKB_ON_DATA1 ");
           if  (prop->dd_sec_ts_serial_diff.ts_clkb_on_data1   ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_sec_ts_serial_diff.ts_clkb_on_data1   ==     1) safe_strcat(msg,"ENABLE  ");
      else                                                              safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_serial_diff.ts_clkb_on_data1);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA0B_ON_DATA2 ");
           if  (prop->dd_sec_ts_serial_diff.ts_data0b_on_data2 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_sec_ts_serial_diff.ts_data0b_on_data2 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                                              safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_serial_diff.ts_data0b_on_data2);
     break;
    #endif /*     Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP */
    #ifdef        Si2164B_DD_SEC_TS_SETUP_PAR_PROP
     case         Si2164B_DD_SEC_TS_SETUP_PAR_PROP_CODE:
      safe_sprintf(msg,"DD_SEC_TS_SETUP_PAR");
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA_STRENGTH "); safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_setup_par.ts_data_strength);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA_SHAPE "); safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_setup_par.ts_data_shape);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_CLK_STRENGTH "); safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_setup_par.ts_clk_strength);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_CLK_SHAPE "); safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_setup_par.ts_clk_shape);
     break;
    #endif /*     Si2164B_DD_SEC_TS_SETUP_PAR_PROP */
    #ifdef        Si2164B_DD_SEC_TS_SETUP_SER_PROP
     case         Si2164B_DD_SEC_TS_SETUP_SER_PROP_CODE:
      safe_sprintf(msg,"DD_SEC_TS_SETUP_SER");
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA_STRENGTH "); safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_setup_ser.ts_data_strength);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA_SHAPE "); safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_setup_ser.ts_data_shape);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_CLK_STRENGTH "); safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_setup_ser.ts_clk_strength);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_CLK_SHAPE "); safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_setup_ser.ts_clk_shape);
     break;
    #endif /*     Si2164B_DD_SEC_TS_SETUP_SER_PROP */
    #ifdef        Si2164B_DD_SEC_TS_SLR_SERIAL_PROP
     case         Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_CODE:
      safe_sprintf(msg,"DD_SEC_TS_SLR_SERIAL");
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA_SLR "); safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_slr_serial.ts_data_slr);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA_SLR_ON ");
           if  (prop->dd_sec_ts_slr_serial.ts_data_slr_on  ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_sec_ts_slr_serial.ts_data_slr_on  ==     1) safe_strcat(msg,"ENABLE  ");
      else                                                          safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_slr_serial.ts_data_slr_on);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA1_SLR "); safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_slr_serial.ts_data1_slr);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA1_SLR_ON ");
           if  (prop->dd_sec_ts_slr_serial.ts_data1_slr_on ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_sec_ts_slr_serial.ts_data1_slr_on ==     1) safe_strcat(msg,"ENABLE  ");
      else                                                          safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_slr_serial.ts_data1_slr_on);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA2_SLR "); safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_slr_serial.ts_data2_slr);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA2_SLR_ON ");
           if  (prop->dd_sec_ts_slr_serial.ts_data2_slr_on ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_sec_ts_slr_serial.ts_data2_slr_on ==     1) safe_strcat(msg,"ENABLE  ");
      else                                                          safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_slr_serial.ts_data2_slr_on);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_CLK_SLR "); safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_slr_serial.ts_clk_slr);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_CLK_SLR_ON ");
           if  (prop->dd_sec_ts_slr_serial.ts_clk_slr_on   ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_sec_ts_slr_serial.ts_clk_slr_on   ==     1) safe_strcat(msg,"ENABLE  ");
      else                                                          safe_sprintf(msg,"%s%d", msg, prop->dd_sec_ts_slr_serial.ts_clk_slr_on);
     break;
    #endif /*     Si2164B_DD_SEC_TS_SLR_SERIAL_PROP */
    #ifdef        Si2164B_DD_SSI_SQI_PARAM_PROP
     case         Si2164B_DD_SSI_SQI_PARAM_PROP_CODE:
      safe_sprintf(msg,"DD_SSI_SQI_PARAM");
       safe_strcat(msg,separator); safe_strcat(msg,"-SQI_AVERAGE "); safe_sprintf(msg,"%s%d", msg, prop->dd_ssi_sqi_param.sqi_average);
     break;
    #endif /*     Si2164B_DD_SSI_SQI_PARAM_PROP */
    #ifdef        Si2164B_DD_TS_FREQ_PROP
     case         Si2164B_DD_TS_FREQ_PROP_CODE:
      safe_sprintf(msg,"DD_TS_FREQ");
       safe_strcat(msg,separator); safe_strcat(msg,"-REQ_FREQ_10KHZ "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_freq.req_freq_10khz);
     break;
    #endif /*     Si2164B_DD_TS_FREQ_PROP */
    #ifdef        Si2164B_DD_TS_FREQ_MAX_PROP
     case         Si2164B_DD_TS_FREQ_MAX_PROP_CODE:
      safe_sprintf(msg,"DD_TS_FREQ_MAX");
       safe_strcat(msg,separator); safe_strcat(msg,"-REQ_FREQ_MAX_10KHZ "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_freq_max.req_freq_max_10khz);
     break;
    #endif /*     Si2164B_DD_TS_FREQ_MAX_PROP */
    #ifdef        Si2164B_DD_TS_MODE_PROP
     case         Si2164B_DD_TS_MODE_PROP_CODE:
      safe_sprintf(msg,"DD_TS_MODE");
       safe_strcat(msg,separator); safe_strcat(msg,"-MODE ");
           if  (prop->dd_ts_mode.mode            ==     0) safe_strcat(msg,"TRISTATE ");
      else if  (prop->dd_ts_mode.mode            ==     1) safe_strcat(msg,"OFF      ");
      else if  (prop->dd_ts_mode.mode            ==     3) safe_strcat(msg,"SERIAL   ");
      else if  (prop->dd_ts_mode.mode            ==     6) safe_strcat(msg,"PARALLEL ");
      else if  (prop->dd_ts_mode.mode            ==     7) safe_strcat(msg,"GPIF     ");
      else                                                safe_sprintf(msg,"%s%d", msg, prop->dd_ts_mode.mode);
       safe_strcat(msg,separator); safe_strcat(msg,"-CLOCK ");
           if  (prop->dd_ts_mode.clock           ==     0) safe_strcat(msg,"AUTO_FIXED ");
      else if  (prop->dd_ts_mode.clock           ==     1) safe_strcat(msg,"AUTO_ADAPT ");
      else if  (prop->dd_ts_mode.clock           ==     2) safe_strcat(msg,"MANUAL     ");
      else                                                safe_sprintf(msg,"%s%d", msg, prop->dd_ts_mode.clock);
       safe_strcat(msg,separator); safe_strcat(msg,"-CLK_GAPPED_EN ");
           if  (prop->dd_ts_mode.clk_gapped_en   ==     0) safe_strcat(msg,"DISABLED ");
      else if  (prop->dd_ts_mode.clk_gapped_en   ==     1) safe_strcat(msg,"ENABLED  ");
      else                                                safe_sprintf(msg,"%s%d", msg, prop->dd_ts_mode.clk_gapped_en);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_ERR_POLARITY ");
           if  (prop->dd_ts_mode.ts_err_polarity ==     0) safe_strcat(msg,"NOT_INVERTED ");
      else if  (prop->dd_ts_mode.ts_err_polarity ==     1) safe_strcat(msg,"INVERTED     ");
      else                                                safe_sprintf(msg,"%s%d", msg, prop->dd_ts_mode.ts_err_polarity);
       safe_strcat(msg,separator); safe_strcat(msg,"-SPECIAL ");
           if  (prop->dd_ts_mode.special         ==     0) safe_strcat(msg,"FULL_TS        ");
      else if  (prop->dd_ts_mode.special         ==     1) safe_strcat(msg,"DATAS_TRISTATE ");
      else                                                safe_sprintf(msg,"%s%d", msg, prop->dd_ts_mode.special);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_FREQ_RESOLUTION ");
           if  (prop->dd_ts_mode.ts_freq_resolution ==     0) safe_strcat(msg,"NORMAL ");
      else if  (prop->dd_ts_mode.ts_freq_resolution ==     1) safe_strcat(msg,"FINE   ");
      else                                                   safe_sprintf(msg,"%s%d", msg, prop->dd_ts_mode.ts_freq_resolution);
       safe_strcat(msg,separator); safe_strcat(msg,"-SERIAL_PIN_SELECTION ");
           if  (prop->dd_ts_mode.serial_pin_selection ==     0) safe_strcat(msg,"D0 ");
      else if  (prop->dd_ts_mode.serial_pin_selection ==     1) safe_strcat(msg,"D1 ");
      else if  (prop->dd_ts_mode.serial_pin_selection ==     2) safe_strcat(msg,"D2 ");
      else if  (prop->dd_ts_mode.serial_pin_selection ==     3) safe_strcat(msg,"D3 ");
      else if  (prop->dd_ts_mode.serial_pin_selection ==     4) safe_strcat(msg,"D4 ");
      else if  (prop->dd_ts_mode.serial_pin_selection ==     5) safe_strcat(msg,"D5 ");
      else if  (prop->dd_ts_mode.serial_pin_selection ==     6) safe_strcat(msg,"D6 ");
      else if  (prop->dd_ts_mode.serial_pin_selection ==     7) safe_strcat(msg,"D7 ");
      else                                                     safe_sprintf(msg,"%d", msg, prop->dd_ts_mode.serial_pin_selection);
     break;
    #endif /*     Si2164B_DD_TS_MODE_PROP */
    #ifdef        Si2164B_DD_TS_SERIAL_DIFF_PROP
     case         Si2164B_DD_TS_SERIAL_DIFF_PROP_CODE:
      safe_sprintf(msg,"DD_TS_SERIAL_DIFF");
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA1_STRENGTH "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_serial_diff.ts_data1_strength);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA1_SHAPE "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_serial_diff.ts_data1_shape);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA2_STRENGTH "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_serial_diff.ts_data2_strength);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA2_SHAPE "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_serial_diff.ts_data2_shape);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_CLKB_ON_DATA1 ");
           if  (prop->dd_ts_serial_diff.ts_clkb_on_data1   ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_ts_serial_diff.ts_clkb_on_data1   ==     1) safe_strcat(msg,"ENABLE  ");
      else                                                          safe_sprintf(msg,"%s%d", msg, prop->dd_ts_serial_diff.ts_clkb_on_data1);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA0B_ON_DATA2 ");
           if  (prop->dd_ts_serial_diff.ts_data0b_on_data2 ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_ts_serial_diff.ts_data0b_on_data2 ==     1) safe_strcat(msg,"ENABLE  ");
      else                                                          safe_sprintf(msg,"%s%d", msg, prop->dd_ts_serial_diff.ts_data0b_on_data2);
     break;
    #endif /*     Si2164B_DD_TS_SERIAL_DIFF_PROP */
    #ifdef        Si2164B_DD_TS_SETUP_PAR_PROP
     case         Si2164B_DD_TS_SETUP_PAR_PROP_CODE:
      safe_sprintf(msg,"DD_TS_SETUP_PAR");
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA_STRENGTH "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_setup_par.ts_data_strength);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA_SHAPE "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_setup_par.ts_data_shape);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_CLK_STRENGTH "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_setup_par.ts_clk_strength);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_CLK_SHAPE "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_setup_par.ts_clk_shape);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_CLK_INVERT ");
           if  (prop->dd_ts_setup_par.ts_clk_invert    ==     0) safe_strcat(msg,"NOT_INVERTED ");
      else if  (prop->dd_ts_setup_par.ts_clk_invert    ==     1) safe_strcat(msg,"INVERTED     ");
      else                                                      safe_sprintf(msg,"%s%d", msg, prop->dd_ts_setup_par.ts_clk_invert);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_CLK_SHIFT "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_setup_par.ts_clk_shift);
     break;
    #endif /*     Si2164B_DD_TS_SETUP_PAR_PROP */
    #ifdef        Si2164B_DD_TS_SETUP_SER_PROP
     case         Si2164B_DD_TS_SETUP_SER_PROP_CODE:
      safe_sprintf(msg,"DD_TS_SETUP_SER");
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA_STRENGTH "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_setup_ser.ts_data_strength);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA_SHAPE "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_setup_ser.ts_data_shape);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_CLK_STRENGTH "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_setup_ser.ts_clk_strength);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_CLK_SHAPE "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_setup_ser.ts_clk_shape);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_CLK_INVERT ");
           if  (prop->dd_ts_setup_ser.ts_clk_invert    ==     0) safe_strcat(msg,"NOT_INVERTED ");
      else if  (prop->dd_ts_setup_ser.ts_clk_invert    ==     1) safe_strcat(msg,"INVERTED     ");
      else                                                      safe_sprintf(msg,"%s%d", msg, prop->dd_ts_setup_ser.ts_clk_invert);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_SYNC_DURATION ");
           if  (prop->dd_ts_setup_ser.ts_sync_duration ==     0) safe_strcat(msg,"FIRST_BYTE ");
      else if  (prop->dd_ts_setup_ser.ts_sync_duration ==     1) safe_strcat(msg,"FIRST_BIT  ");
      else                                                      safe_sprintf(msg,"%s%d", msg, prop->dd_ts_setup_ser.ts_sync_duration);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_BYTE_ORDER ");
           if  (prop->dd_ts_setup_ser.ts_byte_order    ==     0) safe_strcat(msg,"MSB_FIRST ");
      else if  (prop->dd_ts_setup_ser.ts_byte_order    ==     1) safe_strcat(msg,"LSB_FIRST ");
      else                                                      safe_sprintf(msg,"%s%d", msg, prop->dd_ts_setup_ser.ts_byte_order);
     break;
    #endif /*     Si2164B_DD_TS_SETUP_SER_PROP */
    #ifdef        Si2164B_DD_TS_SLR_SERIAL_PROP
     case         Si2164B_DD_TS_SLR_SERIAL_PROP_CODE:
      safe_sprintf(msg,"DD_TS_SLR_SERIAL");
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA_SLR "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_slr_serial.ts_data_slr);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA_SLR_ON ");
           if  (prop->dd_ts_slr_serial.ts_data_slr_on  ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_ts_slr_serial.ts_data_slr_on  ==     1) safe_strcat(msg,"ENABLE  ");
      else                                                      safe_sprintf(msg,"%s%d", msg, prop->dd_ts_slr_serial.ts_data_slr_on);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA1_SLR "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_slr_serial.ts_data1_slr);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA1_SLR_ON ");
           if  (prop->dd_ts_slr_serial.ts_data1_slr_on ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_ts_slr_serial.ts_data1_slr_on ==     1) safe_strcat(msg,"ENABLE  ");
      else                                                      safe_sprintf(msg,"%s%d", msg, prop->dd_ts_slr_serial.ts_data1_slr_on);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA2_SLR "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_slr_serial.ts_data2_slr);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_DATA2_SLR_ON ");
           if  (prop->dd_ts_slr_serial.ts_data2_slr_on ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_ts_slr_serial.ts_data2_slr_on ==     1) safe_strcat(msg,"ENABLE  ");
      else                                                      safe_sprintf(msg,"%s%d", msg, prop->dd_ts_slr_serial.ts_data2_slr_on);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_CLK_SLR "); safe_sprintf(msg,"%s%d", msg, prop->dd_ts_slr_serial.ts_clk_slr);
       safe_strcat(msg,separator); safe_strcat(msg,"-TS_CLK_SLR_ON ");
           if  (prop->dd_ts_slr_serial.ts_clk_slr_on   ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->dd_ts_slr_serial.ts_clk_slr_on   ==     1) safe_strcat(msg,"ENABLE  ");
      else                                                      safe_sprintf(msg,"%s%d", msg, prop->dd_ts_slr_serial.ts_clk_slr_on);
     break;
    #endif /*     Si2164B_DD_TS_SLR_SERIAL_PROP */
    #ifdef        Si2164B_DVBC2_ADC_CREST_FACTOR_PROP
     case         Si2164B_DVBC2_ADC_CREST_FACTOR_PROP_CODE:
      safe_sprintf(msg,"DVBC2_ADC_CREST_FACTOR");
       safe_strcat(msg,separator); safe_strcat(msg,"-CREST_FACTOR "); safe_sprintf(msg,"%s%d", msg, prop->dvbc2_adc_crest_factor.crest_factor);
     break;
    #endif /*     Si2164B_DVBC2_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_DVBC2_AFC_RANGE_PROP
     case         Si2164B_DVBC2_AFC_RANGE_PROP_CODE:
      safe_sprintf(msg,"DVBC2_AFC_RANGE");
       safe_strcat(msg,separator); safe_strcat(msg,"-RANGE_KHZ "); safe_sprintf(msg,"%s%d", msg, prop->dvbc2_afc_range.range_khz);
     break;
    #endif /*     Si2164B_DVBC2_AFC_RANGE_PROP */

    #ifdef        Si2164B_DVBC_ADC_CREST_FACTOR_PROP
     case         Si2164B_DVBC_ADC_CREST_FACTOR_PROP_CODE:
      safe_sprintf(msg,"DVBC_ADC_CREST_FACTOR");
       safe_strcat(msg,separator); safe_strcat(msg,"-CREST_FACTOR "); safe_sprintf(msg,"%s%d", msg, prop->dvbc_adc_crest_factor.crest_factor);
     break;
    #endif /*     Si2164B_DVBC_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_DVBC_AFC_RANGE_PROP
     case         Si2164B_DVBC_AFC_RANGE_PROP_CODE:
      safe_sprintf(msg,"DVBC_AFC_RANGE");
       safe_strcat(msg,separator); safe_strcat(msg,"-RANGE_KHZ "); safe_sprintf(msg,"%s%d", msg, prop->dvbc_afc_range.range_khz);
     break;
    #endif /*     Si2164B_DVBC_AFC_RANGE_PROP */
    #ifdef        Si2164B_DVBC_CONSTELLATION_PROP
     case         Si2164B_DVBC_CONSTELLATION_PROP_CODE:
      safe_sprintf(msg,"DVBC_CONSTELLATION");
       safe_strcat(msg,separator); safe_strcat(msg,"-CONSTELLATION ");
           if  (prop->dvbc_constellation.constellation ==     0) safe_strcat(msg,"AUTO   ");
      else if  (prop->dvbc_constellation.constellation ==     7) safe_strcat(msg,"QAM16  ");
      else if  (prop->dvbc_constellation.constellation ==     8) safe_strcat(msg,"QAM32  ");
      else if  (prop->dvbc_constellation.constellation ==     9) safe_strcat(msg,"QAM64  ");
      else if  (prop->dvbc_constellation.constellation ==    10) safe_strcat(msg,"QAM128 ");
      else if  (prop->dvbc_constellation.constellation ==    11) safe_strcat(msg,"QAM256 ");
      else                                                      safe_sprintf(msg,"%s%d", msg, prop->dvbc_constellation.constellation);
     break;
    #endif /*     Si2164B_DVBC_CONSTELLATION_PROP */
    #ifdef        Si2164B_DVBC_SYMBOL_RATE_PROP
     case         Si2164B_DVBC_SYMBOL_RATE_PROP_CODE:
      safe_sprintf(msg,"DVBC_SYMBOL_RATE");
       safe_strcat(msg,separator); safe_strcat(msg,"-RATE "); safe_sprintf(msg,"%s%d", msg, prop->dvbc_symbol_rate.rate);
     break;
    #endif /*     Si2164B_DVBC_SYMBOL_RATE_PROP */

    #ifdef        Si2164B_DVBS2_ADC_CREST_FACTOR_PROP
     case         Si2164B_DVBS2_ADC_CREST_FACTOR_PROP_CODE:
      safe_sprintf(msg,"DVBS2_ADC_CREST_FACTOR");
       safe_strcat(msg,separator); safe_strcat(msg,"-CREST_FACTOR "); safe_sprintf(msg,"%s%d", msg, prop->dvbs2_adc_crest_factor.crest_factor);
     break;
    #endif /*     Si2164B_DVBS2_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_DVBS2_AFC_RANGE_PROP
     case         Si2164B_DVBS2_AFC_RANGE_PROP_CODE:
      safe_sprintf(msg,"DVBS2_AFC_RANGE");
       safe_strcat(msg,separator); safe_strcat(msg,"-RANGE_KHZ "); safe_sprintf(msg,"%s%d", msg, prop->dvbs2_afc_range.range_khz);
     break;
    #endif /*     Si2164B_DVBS2_AFC_RANGE_PROP */
    #ifdef        Si2164B_DVBS2_SYMBOL_RATE_PROP
     case         Si2164B_DVBS2_SYMBOL_RATE_PROP_CODE:
      safe_sprintf(msg,"DVBS2_SYMBOL_RATE");
       safe_strcat(msg,separator); safe_strcat(msg,"-RATE "); safe_sprintf(msg,"%s%d", msg, prop->dvbs2_symbol_rate.rate);
     break;
    #endif /*     Si2164B_DVBS2_SYMBOL_RATE_PROP */
    #ifdef        Si2164B_DVBS_ADC_CREST_FACTOR_PROP
     case         Si2164B_DVBS_ADC_CREST_FACTOR_PROP_CODE:
      safe_sprintf(msg,"DVBS_ADC_CREST_FACTOR");
       safe_strcat(msg,separator); safe_strcat(msg,"-CREST_FACTOR "); safe_sprintf(msg,"%s%d", msg, prop->dvbs_adc_crest_factor.crest_factor);
     break;
    #endif /*     Si2164B_DVBS_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_DVBS_AFC_RANGE_PROP
     case         Si2164B_DVBS_AFC_RANGE_PROP_CODE:
      safe_sprintf(msg,"DVBS_AFC_RANGE");
       safe_strcat(msg,separator); safe_strcat(msg,"-RANGE_KHZ "); safe_sprintf(msg,"%s%d", msg, prop->dvbs_afc_range.range_khz);
     break;
    #endif /*     Si2164B_DVBS_AFC_RANGE_PROP */
    #ifdef        Si2164B_DVBS_SYMBOL_RATE_PROP
     case         Si2164B_DVBS_SYMBOL_RATE_PROP_CODE:
      safe_sprintf(msg,"DVBS_SYMBOL_RATE");
       safe_strcat(msg,separator); safe_strcat(msg,"-RATE "); safe_sprintf(msg,"%s%d", msg, prop->dvbs_symbol_rate.rate);
     break;
    #endif /*     Si2164B_DVBS_SYMBOL_RATE_PROP */

    #ifdef        Si2164B_DVBT2_ADC_CREST_FACTOR_PROP
     case         Si2164B_DVBT2_ADC_CREST_FACTOR_PROP_CODE:
      safe_sprintf(msg,"DVBT2_ADC_CREST_FACTOR");
       safe_strcat(msg,separator); safe_strcat(msg,"-CREST_FACTOR "); safe_sprintf(msg,"%s%d", msg, prop->dvbt2_adc_crest_factor.crest_factor);
     break;
    #endif /*     Si2164B_DVBT2_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_DVBT2_AFC_RANGE_PROP
     case         Si2164B_DVBT2_AFC_RANGE_PROP_CODE:
      safe_sprintf(msg,"DVBT2_AFC_RANGE");
       safe_strcat(msg,separator); safe_strcat(msg,"-RANGE_KHZ "); safe_sprintf(msg,"%s%d", msg, prop->dvbt2_afc_range.range_khz);
     break;
    #endif /*     Si2164B_DVBT2_AFC_RANGE_PROP */
    #ifdef        Si2164B_DVBT2_FEF_TUNER_PROP
     case         Si2164B_DVBT2_FEF_TUNER_PROP_CODE:
      safe_sprintf(msg,"DVBT2_FEF_TUNER");
       safe_strcat(msg,separator); safe_strcat(msg,"-TUNER_DELAY "); safe_sprintf(msg,"%s%d", msg, prop->dvbt2_fef_tuner.tuner_delay);
       safe_strcat(msg,separator); safe_strcat(msg,"-TUNER_FREEZE_TIME "); safe_sprintf(msg,"%s%d", msg, prop->dvbt2_fef_tuner.tuner_freeze_time);
       safe_strcat(msg,separator); safe_strcat(msg,"-TUNER_UNFREEZE_TIME "); safe_sprintf(msg,"%s%d", msg, prop->dvbt2_fef_tuner.tuner_unfreeze_time);
     break;
    #endif /*     Si2164B_DVBT2_FEF_TUNER_PROP */
    #ifdef        Si2164B_DVBT2_MODE_PROP
     case         Si2164B_DVBT2_MODE_PROP_CODE:
      safe_sprintf(msg,"DVBT2_MODE");
       safe_strcat(msg,separator); safe_strcat(msg,"-LOCK_MODE ");
           if  (prop->dvbt2_mode.lock_mode ==     0) safe_strcat(msg,"ANY       ");
      else if  (prop->dvbt2_mode.lock_mode ==     1) safe_strcat(msg,"BASE_ONLY ");
      else if  (prop->dvbt2_mode.lock_mode ==     2) safe_strcat(msg,"LITE_ONLY ");
      else if  (prop->dvbt2_mode.lock_mode ==     3) safe_strcat(msg,"RESERVED  ");
      else                                          safe_sprintf(msg,"%s%d", msg, prop->dvbt2_mode.lock_mode);
     break;
    #endif /*     Si2164B_DVBT2_MODE_PROP */

    #ifdef        Si2164B_DVBT_ADC_CREST_FACTOR_PROP
     case         Si2164B_DVBT_ADC_CREST_FACTOR_PROP_CODE:
      safe_sprintf(msg,"DVBT_ADC_CREST_FACTOR");
       safe_strcat(msg,separator); safe_strcat(msg,"-CREST_FACTOR "); safe_sprintf(msg,"%s%d", msg, prop->dvbt_adc_crest_factor.crest_factor);
     break;
    #endif /*     Si2164B_DVBT_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_DVBT_AFC_RANGE_PROP
     case         Si2164B_DVBT_AFC_RANGE_PROP_CODE:
      safe_sprintf(msg,"DVBT_AFC_RANGE");
       safe_strcat(msg,separator); safe_strcat(msg,"-RANGE_KHZ "); safe_sprintf(msg,"%s%d", msg, prop->dvbt_afc_range.range_khz);
     break;
    #endif /*     Si2164B_DVBT_AFC_RANGE_PROP */
    #ifdef        Si2164B_DVBT_HIERARCHY_PROP
     case         Si2164B_DVBT_HIERARCHY_PROP_CODE:
      safe_sprintf(msg,"DVBT_HIERARCHY");
       safe_strcat(msg,separator); safe_strcat(msg,"-STREAM ");
           if  (prop->dvbt_hierarchy.stream ==     0) safe_strcat(msg,"HP ");
      else if  (prop->dvbt_hierarchy.stream ==     1) safe_strcat(msg,"LP ");
      else                                           safe_sprintf(msg,"%s%d", msg, prop->dvbt_hierarchy.stream);
     break;
    #endif /*     Si2164B_DVBT_HIERARCHY_PROP */


    #ifdef        Si2164B_MASTER_IEN_PROP
     case         Si2164B_MASTER_IEN_PROP_CODE:
      safe_sprintf(msg,"MASTER_IEN");
       safe_strcat(msg,separator); safe_strcat(msg,"-DDIEN ");
           if  (prop->master_ien.ddien   ==     0) safe_strcat(msg,"OFF ");
      else if  (prop->master_ien.ddien   ==     1) safe_strcat(msg,"ON  ");
      else                                        safe_sprintf(msg,"%s%d", msg, prop->master_ien.ddien);
       safe_strcat(msg,separator); safe_strcat(msg,"-SCANIEN ");
           if  (prop->master_ien.scanien ==     0) safe_strcat(msg,"OFF ");
      else if  (prop->master_ien.scanien ==     1) safe_strcat(msg,"ON  ");
      else                                        safe_sprintf(msg,"%s%d", msg, prop->master_ien.scanien);
       safe_strcat(msg,separator); safe_strcat(msg,"-ERRIEN ");
           if  (prop->master_ien.errien  ==     0) safe_strcat(msg,"OFF ");
      else if  (prop->master_ien.errien  ==     1) safe_strcat(msg,"ON  ");
      else                                        safe_sprintf(msg,"%s%d", msg, prop->master_ien.errien);
       safe_strcat(msg,separator); safe_strcat(msg,"-CTSIEN ");
           if  (prop->master_ien.ctsien  ==     0) safe_strcat(msg,"OFF ");
      else if  (prop->master_ien.ctsien  ==     1) safe_strcat(msg,"ON  ");
      else                                        safe_sprintf(msg,"%s%d", msg, prop->master_ien.ctsien);
     break;
    #endif /*     Si2164B_MASTER_IEN_PROP */
    #ifdef        Si2164B_MCNS_ADC_CREST_FACTOR_PROP
     case         Si2164B_MCNS_ADC_CREST_FACTOR_PROP_CODE:
      safe_sprintf(msg,"MCNS_ADC_CREST_FACTOR");
       safe_strcat(msg,separator); safe_strcat(msg,"-CREST_FACTOR "); safe_sprintf(msg,"%s%d", msg, prop->mcns_adc_crest_factor.crest_factor);
     break;
    #endif /*     Si2164B_MCNS_ADC_CREST_FACTOR_PROP */
    #ifdef        Si2164B_MCNS_AFC_RANGE_PROP
     case         Si2164B_MCNS_AFC_RANGE_PROP_CODE:
      safe_sprintf(msg,"MCNS_AFC_RANGE");
       safe_strcat(msg,separator); safe_strcat(msg,"-RANGE_KHZ "); safe_sprintf(msg,"%s%d", msg, prop->mcns_afc_range.range_khz);
     break;
    #endif /*     Si2164B_MCNS_AFC_RANGE_PROP */
    #ifdef        Si2164B_MCNS_CONSTELLATION_PROP
     case         Si2164B_MCNS_CONSTELLATION_PROP_CODE:
      safe_sprintf(msg,"MCNS_CONSTELLATION");
       safe_strcat(msg,separator); safe_strcat(msg,"-CONSTELLATION ");
           if  (prop->mcns_constellation.constellation ==     0) safe_strcat(msg,"AUTO   ");
      else if  (prop->mcns_constellation.constellation ==     9) safe_strcat(msg,"QAM64  ");
      else if  (prop->mcns_constellation.constellation ==    11) safe_strcat(msg,"QAM256 ");
      else                                                      safe_sprintf(msg,"%s%d", msg, prop->mcns_constellation.constellation);
     break;
    #endif /*     Si2164B_MCNS_CONSTELLATION_PROP */
    #ifdef        Si2164B_MCNS_SYMBOL_RATE_PROP
     case         Si2164B_MCNS_SYMBOL_RATE_PROP_CODE:
      safe_sprintf(msg,"MCNS_SYMBOL_RATE");
       safe_strcat(msg,separator); safe_strcat(msg,"-RATE "); safe_sprintf(msg,"%s%d", msg, prop->mcns_symbol_rate.rate);
     break;
    #endif /*     Si2164B_MCNS_SYMBOL_RATE_PROP */

    #ifdef        Si2164B_SCAN_FMAX_PROP
     case         Si2164B_SCAN_FMAX_PROP_CODE:
      safe_sprintf(msg,"SCAN_FMAX");
       safe_strcat(msg,separator); safe_strcat(msg,"-SCAN_FMAX "); safe_sprintf(msg,"%s%d", msg, prop->scan_fmax.scan_fmax);
     break;
    #endif /*     Si2164B_SCAN_FMAX_PROP */
    #ifdef        Si2164B_SCAN_FMIN_PROP
     case         Si2164B_SCAN_FMIN_PROP_CODE:
      safe_sprintf(msg,"SCAN_FMIN");
       safe_strcat(msg,separator); safe_strcat(msg,"-SCAN_FMIN "); safe_sprintf(msg,"%s%d", msg, prop->scan_fmin.scan_fmin);
     break;
    #endif /*     Si2164B_SCAN_FMIN_PROP */
    #ifdef        Si2164B_SCAN_IEN_PROP
     case         Si2164B_SCAN_IEN_PROP_CODE:
      safe_sprintf(msg,"SCAN_IEN");
       safe_strcat(msg,separator); safe_strcat(msg,"-BUZIEN ");
           if  (prop->scan_ien.buzien ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->scan_ien.buzien ==     1) safe_strcat(msg,"ENABLE  ");
      else                                     safe_sprintf(msg,"%s%d", msg, prop->scan_ien.buzien);
       safe_strcat(msg,separator); safe_strcat(msg,"-REQIEN ");
           if  (prop->scan_ien.reqien ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->scan_ien.reqien ==     1) safe_strcat(msg,"ENABLE  ");
      else                                     safe_sprintf(msg,"%s%d", msg, prop->scan_ien.reqien);
     break;
    #endif /*     Si2164B_SCAN_IEN_PROP */
    #ifdef        Si2164B_SCAN_INT_SENSE_PROP
     case         Si2164B_SCAN_INT_SENSE_PROP_CODE:
      safe_sprintf(msg,"SCAN_INT_SENSE");
       safe_strcat(msg,separator); safe_strcat(msg,"-BUZNEGEN ");
           if  (prop->scan_int_sense.buznegen ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->scan_int_sense.buznegen ==     1) safe_strcat(msg,"ENABLE  ");
      else                                             safe_sprintf(msg,"%s%d", msg, prop->scan_int_sense.buznegen);
       safe_strcat(msg,separator); safe_strcat(msg,"-REQNEGEN ");
           if  (prop->scan_int_sense.reqnegen ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->scan_int_sense.reqnegen ==     1) safe_strcat(msg,"ENABLE  ");
      else                                             safe_sprintf(msg,"%s%d", msg, prop->scan_int_sense.reqnegen);
       safe_strcat(msg,separator); safe_strcat(msg,"-BUZPOSEN ");
           if  (prop->scan_int_sense.buzposen ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->scan_int_sense.buzposen ==     1) safe_strcat(msg,"ENABLE  ");
      else                                             safe_sprintf(msg,"%s%d", msg, prop->scan_int_sense.buzposen);
       safe_strcat(msg,separator); safe_strcat(msg,"-REQPOSEN ");
           if  (prop->scan_int_sense.reqposen ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->scan_int_sense.reqposen ==     1) safe_strcat(msg,"ENABLE  ");
      else                                             safe_sprintf(msg,"%s%d", msg, prop->scan_int_sense.reqposen);
     break;
    #endif /*     Si2164B_SCAN_INT_SENSE_PROP */
    #ifdef        Si2164B_SCAN_SAT_CONFIG_PROP
     case         Si2164B_SCAN_SAT_CONFIG_PROP_CODE:
      safe_sprintf(msg,"SCAN_SAT_CONFIG");
       safe_strcat(msg,separator); safe_strcat(msg,"-ANALOG_DETECT ");
           if  (prop->scan_sat_config.analog_detect ==     0) safe_strcat(msg,"DISABLED ");
      else if  (prop->scan_sat_config.analog_detect ==     1) safe_strcat(msg,"ENABLED  ");
      else                                                   safe_sprintf(msg,"%s%d", msg, prop->scan_sat_config.analog_detect);
       safe_strcat(msg,separator); safe_strcat(msg,"-RESERVED1 "); safe_sprintf(msg,"%s%d", msg, prop->scan_sat_config.reserved1);
       safe_strcat(msg,separator); safe_strcat(msg,"-RESERVED2 "); safe_sprintf(msg,"%s%d", msg, prop->scan_sat_config.reserved2);
     break;
    #endif /*     Si2164B_SCAN_SAT_CONFIG_PROP */
    #ifdef        Si2164B_SCAN_SAT_UNICABLE_BW_PROP
     case         Si2164B_SCAN_SAT_UNICABLE_BW_PROP_CODE:
      safe_sprintf(msg,"SCAN_SAT_UNICABLE_BW");
       safe_strcat(msg,separator); safe_strcat(msg,"-SCAN_SAT_UNICABLE_BW "); safe_sprintf(msg,"%s%d", msg, prop->scan_sat_unicable_bw.scan_sat_unicable_bw);
     break;
    #endif /*     Si2164B_SCAN_SAT_UNICABLE_BW_PROP */
    #ifdef        Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP
     case         Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP_CODE:
      safe_sprintf(msg,"SCAN_SAT_UNICABLE_MIN_TUNE_STEP");
       safe_strcat(msg,separator); safe_strcat(msg,"-SCAN_SAT_UNICABLE_MIN_TUNE_STEP "); safe_sprintf(msg,"%s%d", msg, prop->scan_sat_unicable_min_tune_step.scan_sat_unicable_min_tune_step);
     break;
    #endif /*     Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP */

    #ifdef        Si2164B_SCAN_SYMB_RATE_MAX_PROP
     case         Si2164B_SCAN_SYMB_RATE_MAX_PROP_CODE:
      safe_sprintf(msg,"SCAN_SYMB_RATE_MAX");
       safe_strcat(msg,separator); safe_strcat(msg,"-SCAN_SYMB_RATE_MAX "); safe_sprintf(msg,"%s%d", msg, prop->scan_symb_rate_max.scan_symb_rate_max);
     break;
    #endif /*     Si2164B_SCAN_SYMB_RATE_MAX_PROP */
    #ifdef        Si2164B_SCAN_SYMB_RATE_MIN_PROP
     case         Si2164B_SCAN_SYMB_RATE_MIN_PROP_CODE:
      safe_sprintf(msg,"SCAN_SYMB_RATE_MIN");
       safe_strcat(msg,separator); safe_strcat(msg,"-SCAN_SYMB_RATE_MIN "); safe_sprintf(msg,"%s%d", msg, prop->scan_symb_rate_min.scan_symb_rate_min);
     break;
    #endif /*     Si2164B_SCAN_SYMB_RATE_MIN_PROP */
    #ifdef        Si2164B_SCAN_TER_CONFIG_PROP
     case         Si2164B_SCAN_TER_CONFIG_PROP_CODE:
      safe_sprintf(msg,"SCAN_TER_CONFIG");
       safe_strcat(msg,separator); safe_strcat(msg,"-MODE ");
           if  (prop->scan_ter_config.mode          ==     0) safe_strcat(msg,"BLIND_SCAN   ");
      else if  (prop->scan_ter_config.mode          ==     1) safe_strcat(msg,"MAPPING_SCAN ");
      else if  (prop->scan_ter_config.mode          ==     2) safe_strcat(msg,"BLIND_LOCK   ");
      else                                                   safe_sprintf(msg,"%s%d", msg, prop->scan_ter_config.mode);
       safe_strcat(msg,separator); safe_strcat(msg,"-ANALOG_BW ");
           if  (prop->scan_ter_config.analog_bw     ==     1) safe_strcat(msg,"6MHZ ");
      else if  (prop->scan_ter_config.analog_bw     ==     2) safe_strcat(msg,"7MHZ ");
      else if  (prop->scan_ter_config.analog_bw     ==     3) safe_strcat(msg,"8MHZ ");
      else                                                   safe_sprintf(msg,"%s%d", msg, prop->scan_ter_config.analog_bw);
       safe_strcat(msg,separator); safe_strcat(msg,"-SEARCH_ANALOG ");
           if  (prop->scan_ter_config.search_analog ==     0) safe_strcat(msg,"DISABLE ");
      else if  (prop->scan_ter_config.search_analog ==     1) safe_strcat(msg,"ENABLE  ");
      else                                                   safe_sprintf(msg,"%s%d", msg, prop->scan_ter_config.search_analog);
     break;
    #endif /*     Si2164B_SCAN_TER_CONFIG_PROP */

     default : safe_sprintf(msg,"Unknown property code '0x%06x'\n", prop_code); return ERROR_Si2164B_UNKNOWN_PROPERTY; break;
    }
  return NO_Si2164B_ERROR;
}
/***********************************************************************************************************************
  Si2164B_L1_FillPropertyStringText function
  Use:        property text retrieval function
              Used to retrieve the property text for a selected property.
  Parameter: *api      the Si2164B context
  Parameter: prop_code the property Id (used to know which property to use)
  Parameter: separator the string to use between fields (often either a blank or a newline character)
  Parameter: msg       the string used to store the resulting string
                       It must be declared by the caller with a size of 1000 bytes
  Returns:    NO_Si2164B_ERROR if successful.
 ***********************************************************************************************************************/
void          Si2164B_L1_FillPropertyStringText(L1_Si2164B_Context *api, unsigned int prop_code, const unsigned char *separator, unsigned char *msg) {
  Si2164B_L1_PropertyText (api->prop, prop_code, separator, msg);
}
/***********************************************************************************************************************
  Si2164B_L1_GetPropertyString function
  Use:        current property text retrieval function
              Used to retrieve the property value from the hardware then retrieve the corresponding property text.
  Parameter: *api      the Si2164B context
  Parameter: prop_code the property Id (used to know which property to use)
  Parameter: separator the string to use between fields (often either a blank or a newline character)
  Parameter: msg       the string used to store the resulting string
                       It must be declared by the caller with a size of 1000 bytes
  Returns:    NO_Si2164B_ERROR if successful.
 ***********************************************************************************************************************/
unsigned char Si2164B_L1_GetPropertyString     (L1_Si2164B_Context *api, unsigned int prop_code, const unsigned char *separator, unsigned char *msg) {
    signed int res;
    res = Si2164B_L1_GetProperty2(api,prop_code);
    if (res!=NO_Si2164B_ERROR) { safe_sprintf(msg, "%s",Si2164B_L1_API_ERROR_TEXT(res)); return res; }
    Si2164B_L1_PropertyText(api->prop, prop_code, separator, msg);
    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_GET_PROPERTY_STRING */








