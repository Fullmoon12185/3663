#ifndef  _SiLabs_API_L3_Wrapper_H_
#define  _SiLabs_API_L3_Wrapper_H_


#include "Si2164B_Platform_Definition.h"//20130628,csm,13'
#include "SemcoSi2164B.h"//csm,13'

/* Change log:

  As from V2.5.3:
   <new_feature>[Cypress/TS_SLAVE] Adding   SILABS_TS_SLAVE_FIFO = 5, in CUSTOM_TS_Mode_Enum;
   <new_part>[LNB/A8302] Adding SAT_Select_LNB_Chip_lnb_index in SILABS_FE_Context, to store the index.
      This flag indicates which part in a dual LNB controller is in use.

  As from V2.5.0:
   <new_feature> Adding ts_mux_input in SILABS_FE_Context struct to associate a mux input to each front end.
   <new feature>[TER_Tuner/Config]
     adding SiLabs_API_TER_Tuner_Block_VCO2 and SiLabs_API_TER_Tuner_Block_VCO3 to allow configuration of
     the TER_Tuner block_VCO2_code and block_VCO3_code
   <new_feature>[CONFIG/tracing] Adding all configuration fields in SILABS_FE_Context (to enable configuration checking after init).

  As from V2.4.7:
  <compatibility/Tizen> SILABS_QAMAUTO: replacing '-1' by '100. This is because Tizen uses 'unsigned char' when 'char foo;' is defined.
    All other OSs use 'signed char' by default and have no issue with this.

  As from V2.4.0:
  <new_feature>[TS_Crossbar/Duals] Adding TS Crossbar capability via SiLabs_API_L3_TS_Crossbar.c/.h
        This is only valid for dual demodulators with the TS crossbar feature.

  As from V2.3.9:
  <new_part> Adding ISDB-T values in CUSTOM_Status_Struct (isdbt_system_id, nb_seg_a, nb_seg_b, nb_seg_c)

  As from V2.3.7:
  <new_feature> [DVB-S2X] Adding Constellation and code rate enums for DVB-S2X.
  <new_feature> [ISDB-T]  Adding support for ISDB-T.

  As from V2.3.6:
  <new_feature> [no_float] Adding c_n_100, ber_mant, ber_exp, per_mant, per_exp, fer_mant, fer_exp in status structure,
                           These are used whne the application is not allowed to use floats
  <cleanup>     [NO_SAT/NO_TER] Removing NO_SAT and NO_TER compilation flags

  As from V2.1.9:
   Compatibility with 'generic' TER_TUNER_SILABS API, for easier integration of future SiLabs tuners

  As from V2.1.2:
   Adding SiLabs_API_SSI_SQI prototype

  As from V2.0.9:
   Removing commas on last lines of type declarations, to avoid some ISO-C compiler warnings

 *************************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
#include "SiDemod_Si2164B.h"
#define   TERRESTRIAL_FRONT_END
#ifdef TER_TUNER_SILABS
  #include  "SiLabs_TER_Tuner_API.h"
#endif /* TER_TUNER_SILABS */

#ifdef SAT_TUNER_SILABS
  #include  "SiLabs_SAT_Tuner_API.h"
#endif /* SAT_TUNER_SILABS */

 #ifdef    LNBH21_COMPATIBLE
  #include "LNBH21_L1_API.h"
 #endif /* LNBH21_COMPATIBLE */
 #ifdef    LNBH25_COMPATIBLE
  #include "LNBH25_L1_API.h"
 #endif /* LNBH25_COMPATIBLE */
 #ifdef    LNBH26_COMPATIBLE
  #include "LNBH26_L1_API.h"
 #endif /* LNBH26_COMPATIBLE */
 #ifdef    LNBH29_COMPATIBLE
  #include "LNBH29_L1_API.h"
 #endif /* LNBH29_COMPATIBLE */
 #ifdef    A8293_COMPATIBLE
  #include "A8293_L1_API.h"
 #endif /* A8293_COMPATIBLE */
#ifdef    UNICABLE_COMPATIBLE
  #include "SiLabs_Unicable_API.h"
  unsigned char                lnb_type;
  SILABS_Unicable_Context      Unicable_Obj;
  SILABS_Unicable_Context     *unicable;
#endif /* UNICABLE_COMPATIBLE */
typedef enum  _STATUS_SELECTION {
  FE_LOCK_STATE     = 0x01, /* demod_lock, fec_lock,  uncorrs, TS_bitrate_kHz, TS_clock_kHz  */
  FE_LEVELS         = 0x02, /* RSSI, RFagc, IFagc                                            */
  FE_RATES          = 0x04, /* BER, PER, FER (depending on standard)                         */
  FE_SPECIFIC       = 0x08, /* symbol_rate, stream, constellation, c/n, freq_offset,
                                 timing_offset, code_rate, t2_version, num_plp, plp_id
                                 ds_id, cell_id, etc. (generally one command per standard)   */
  FE_QUALITY        = 0x10, /* SSI, SQI                                                      */
  FE_FREQ           = 0x20, /* freq                                                          */
} STATUS_SELECTION;

/* Standard code values used by the top-level application               */
/* <porting> set these values to match the top-level application values */
typedef enum   CUSTOM_Standard_Enum          {
  SILABS_ANALOG = 4,
  SILABS_DVB_T  = 0,
  SILABS_DVB_C  = 1,
  SILABS_DVB_S  = 2,
  SILABS_DVB_S2 = 3,
  SILABS_DVB_T2 = 5,
  SILABS_DSS    = 6,
  SILABS_MCNS   = 7,
  SILABS_DVB_C2 = 8,
  SILABS_ISDB_T = 9,
  SILABS_SLEEP  = 100
} CUSTOM_Standard_Enum;

typedef enum   CUSTOM_Constel_Enum           {
  SILABS_QAMAUTO  = 100,
  SILABS_QAM16    =  0,
  SILABS_QAM32    =  1,
  SILABS_QAM64    =  2,
  SILABS_QAM128   =  3,
  SILABS_QAM256   =  4,
  SILABS_QPSK     =  5,
  SILABS_8PSK     =  6,
  SILABS_QAM1024  =  7,
  SILABS_QAM4096  =  8,
  SILABS_8APSK_L  = 10,
  SILABS_16APSK   = 11,
  SILABS_16APSK_L = 12,
  SILABS_32APSK_1 = 13,
  SILABS_32APSK_2 = 14,
  SILABS_32APSK_L = 15
} CUSTOM_Constel_Enum;

typedef enum   CUSTOM_Stream_Enum            {
  SILABS_HP = 0,
  SILABS_LP = 1
} CUSTOM_Stream_Enum;

typedef enum   CUSTOM_T2_PLP_TYPE            {
  SILABS_PLP_TYPE_COMMON     = 0,
  SILABS_PLP_TYPE_DATA_TYPE1 = 1,
  SILABS_PLP_TYPE_DATA_TYPE2 = 2
} CUSTOM_T2_PLP_TYPE;

typedef enum   CUSTOM_TS_Mode_Enum           {
  SILABS_TS_TRISTATE = 0,
  SILABS_TS_SERIAL   = 1,
  SILABS_TS_PARALLEL = 2,
  SILABS_TS_GPIF     = 3,
  SILABS_TS_OFF      = 4,
  SILABS_TS_SLAVE_FIFO = 5,
  SILABS_TS_STREAMING  = 6,
} CUSTOM_TS_Mode_Enum;

typedef enum   CUSTOM_FFT_Mode_Enum          {
  SILABS_FFT_MODE_2K  = 0,
  SILABS_FFT_MODE_4K  = 1,
  SILABS_FFT_MODE_8K  = 2,
  SILABS_FFT_MODE_16K = 3,
  SILABS_FFT_MODE_32K = 4
} CUSTOM_FFT_Mode_Enum;

typedef enum   CUSTOM_GI_Enum                {
  SILABS_GUARD_INTERVAL_1_32    = 0,
  SILABS_GUARD_INTERVAL_1_16    = 1,
  SILABS_GUARD_INTERVAL_1_8     = 2,
  SILABS_GUARD_INTERVAL_1_4     = 3,
  SILABS_GUARD_INTERVAL_1_128   = 4,
  SILABS_GUARD_INTERVAL_19_128  = 5,
  SILABS_GUARD_INTERVAL_19_256  = 6,
  SILABS_GUARD_INTERVAL_1_64    = 7
} CUSTOM_GI_Enum;

typedef enum   CUSTOM_Coderate_Enum          {
  SILABS_CODERATE_1_2   =  0,
  SILABS_CODERATE_2_3   =  1,
  SILABS_CODERATE_3_4   =  2,
  SILABS_CODERATE_4_5   =  3,
  SILABS_CODERATE_5_6   =  4,
  SILABS_CODERATE_7_8   =  5,
  SILABS_CODERATE_8_9   =  6,
  SILABS_CODERATE_9_10  =  7,
  SILABS_CODERATE_1_3   =  8,
  SILABS_CODERATE_1_4   =  9,
  SILABS_CODERATE_2_5   = 10,
  SILABS_CODERATE_3_5   = 11,
  SILABS_CODERATE_5_9   = 12,
  SILABS_CODERATE_7_9   = 13,
  SILABS_CODERATE_8_15  = 14,
  SILABS_CODERATE_11_15 = 15,
  SILABS_CODERATE_13_18 = 16,
  SILABS_CODERATE_9_20  = 17,
  SILABS_CODERATE_11_20 = 18,
  SILABS_CODERATE_23_36 = 19,
  SILABS_CODERATE_25_36 = 20,
  SILABS_CODERATE_13_45 = 21,
  SILABS_CODERATE_26_45 = 22,
  SILABS_CODERATE_28_45 = 23,
  SILABS_CODERATE_32_45 = 24,
  SILABS_CODERATE_77_90 = 25
} CUSTOM_Coderate_Enum;

typedef enum   CUSTOM_Polarization_Enum      {
  SILABS_POLARIZATION_HORIZONTAL  = 0,
  SILABS_POLARIZATION_VERTICAL,
  SILABS_POLARIZATION_DO_NOT_CHANGE
} CUSTOM_Polarization_Enum;

typedef enum   CUSTOM_Band_Enum              {
  SILABS_BAND_LOW  = 0,
  SILABS_BAND_HIGH,
  SILABS_BAND_DO_NOT_CHANGE
} CUSTOM_Band_Enum;

typedef enum   CUSTOM_Hierarchy_Enum         {
  SILABS_HIERARCHY_NONE  = 0,
  SILABS_HIERARCHY_ALFA1 = 1,
  SILABS_HIERARCHY_ALFA2 = 2,
  SILABS_HIERARCHY_ALFA4 = 3
} CUSTOM_Hierarchy_Enum;

typedef enum   CUSTOM_Video_Sys_Enum         {
  SILABS_VIDEO_SYS_B  = 0,
  SILABS_VIDEO_SYS_GH = 1,
  SILABS_VIDEO_SYS_M  = 2,
  SILABS_VIDEO_SYS_N  = 3,
  SILABS_VIDEO_SYS_I  = 4,
  SILABS_VIDEO_SYS_DK = 5,
  SILABS_VIDEO_SYS_L  = 6,
  SILABS_VIDEO_SYS_LP = 7
} CUSTOM_Video_Sys_Enum;

typedef enum   CUSTOM_Transmission_Mode_Enum {
  SILABS_TRANSMISSION_MODE_TERRESTRIAL = 0,
  SILABS_TRANSMISSION_MODE_CABLE       = 1
} CUSTOM_Transmission_Mode_Enum;

typedef enum   CUSTOM_Pilot_Pattern_Enum     {
  SILABS_PILOT_PATTERN_PP1 = 1,
  SILABS_PILOT_PATTERN_PP2 = 2,
  SILABS_PILOT_PATTERN_PP3 = 3,
  SILABS_PILOT_PATTERN_PP4 = 4,
  SILABS_PILOT_PATTERN_PP5 = 5,
  SILABS_PILOT_PATTERN_PP6 = 6,
  SILABS_PILOT_PATTERN_PP7 = 7,
  SILABS_PILOT_PATTERN_PP8 = 8
} CUSTOM_Pilot_Pattern_Enum;

typedef enum   CUSTOM_Pilot_T2_version_Enum  {
  SILABS_T2_VERSION_1_1_1 = 111,
  SILABS_T2_VERSION_1_2_1 = 121,
  SILABS_T2_VERSION_1_3_1 = 131
} CUSTOM_Pilot_T2_version_Enum;

typedef enum   CUSTOM_Color_Enum             {
  SILABS_COLOR_PAL_NTSC  = 0,
  SILABS_COLOR_SECAM     = 1
} CUSTOM_Color_Enum;

typedef enum   CUSTOM_Audio_Sys_Enum         {
  SILABS_AUDIO_SYS_DEFAULT         = 0,
  SILABS_AUDIO_SYS_MONO            = 1,
  SILABS_AUDIO_SYS_MONO_NICAM      = 2,
  SILABS_AUDIO_SYS_A2              = 3,
  SILABS_AUDIO_SYS_A2_DK2          = 4,
  SILABS_AUDIO_SYS_A2_DK3          = 5,
  SILABS_AUDIO_SYS_BTSC            = 6,
  SILABS_AUDIO_SYS_EIAJ            = 7,
  SILABS_AUDIO_SYS_SCAN            = 8,
  SILABS_AUDIO_SYS_A2_DK4          = 9,
  SILABS_AUDIO_SYS_WIDE_SCAN       = 10,
  SILABS_AUDIO_SYS_MONO_NICAM_6DB  = 11,
  SILABS_AUDIO_SYS_MONO_NICAM_10DB = 12
} CUSTOM_Audio_Sys_Enum;

typedef struct _SILABS_TER_TUNER_Config       {
  unsigned char nSel_dtv_out_type;
  unsigned char nSel_dtv_agc_source;
  signed int           nSel_dtv_lif_freq_offset;
  unsigned char nSel_dtv_mode_bw;
  unsigned char nSel_dtv_mode_invert_spectrum;
  unsigned char nSel_dtv_mode_modulation;
  unsigned char nSel_atv_video_mode_video_sys;
  unsigned char nSel_atv_audio_mode_audio_sys;
  unsigned char nSel_atv_atv_video_mode_tran;
  unsigned char nSel_atv_video_mod_color;
  unsigned char nSel_atv_mode_invert_spectrum;
  unsigned char nSel_atv_mode_invert_signal;
  unsigned char nSel_atv_cvbs_out_fine_amp;
  unsigned char nSel_atv_cvbs_out_fine_offset;
  unsigned char nSel_atv_sif_out_amp;
  unsigned char nSel_atv_sif_out_offset;
  unsigned char if_agc_speed;
  signed char          nSel_dtv_rf_top;
  signed char          nSel_atv_rf_top;
  unsigned long nLocked_Freq;
  unsigned long nCenter_Freq;
  signed int           nCriteriaStep;
  signed int           nLeftMax;
  signed int           nRightMax;
  signed int           nReal_Step;
  signed int           nBeforeStep;
} SILABS_TER_TUNER_Config;

typedef struct _SILABS_SAT_TUNER_Config       {
    signed int    RF;
    signed int    IF;
    signed int    minRF;
    signed int    maxRF;
    signed int  xtal;//hjh  double->
    unsigned long LPF; //hjh double->
    signed int    tunerBytesCount;
    signed int    I2CMuxChannel;
    signed int  RefDiv_value;//hjh double->
    signed int    Mash_Per;
    CONNECTION_TYPE connType;
    unsigned char tuner_log[40];
    unsigned char tuner_read[7];
    signed char          nSel_att_top;
} SILABS_SAT_TUNER_Config;

typedef struct _SILABS_CARRIER_Config         {
  signed int                freq;
  signed int                bandwidth;
  signed int                stream;
  signed int                symbol_rate;
  signed int                constellation;
  signed int                polarization;
  signed int                band;
} SILABS_CARRIER_Config;

typedef struct _SILABS_ANALOG_CARRIER_Config  {
  unsigned char      video_sys;
  unsigned char      trans;
  unsigned char      color;
  unsigned char      invert_signal;
  unsigned char      invert_spectrum;
  //csm,add/
  unsigned char		atv_audio_mode_audio_sys;
  unsigned char		atv_audio_mode_chan_bw;
  unsigned char		atv_cvbs_out_fine_offset;
  unsigned char		atv_cvbs_out_fine_amp;
  unsigned char		atv_sif_out_offset;
  unsigned char		atv_sif_out_amp;
  unsigned char		atv_sound_agc_speed_other;
  unsigned char		atv_agc_speed;
  //end
} SILABS_ANALOG_CARRIER_Config;

typedef struct _SILABS_ANALOG_SIF_Config      {
  unsigned char      stereo_sys;
} SILABS_ANALOG_SIF_Config;

typedef struct _SILABS_FE_Context             {
  unsigned int       chip;
  unsigned int       tuner_ter;
  unsigned int       tuner_sat;
  unsigned char               tag[SILABS_TAG_SIZE];
  unsigned char      fe_index;
  CUSTOM_TS_Mode_Enum active_TS_mode;
  SiDemodContext *SiDemod_FE;
  SiDemodContext  SiDemod_FE_Obj;
  signed int                standard;
  signed int                init_ok;
  signed int                polarization;
  signed int                band;
  SILABS_TER_TUNER_Config      TER_Tuner_Cfg;
  SILABS_SAT_TUNER_Config      SAT_Tuner_Cfg;
  SILABS_CARRIER_Config        Carrier_Cfg;
  SILABS_ANALOG_CARRIER_Config Analog_Cfg;
  SILABS_ANALOG_SIF_Config     Analog_Sif_Cfg;
  signed int                TER_tuner_I2C_connection;
  signed int                SAT_tuner_I2C_connection;
  signed int                I2C_connected;
  unsigned int       config_code;
  signed int                lnb_chip;
  signed int                lnb_chip_address;
  signed int                lnb_chip_init_done;
 #ifdef    LNBH21_COMPATIBLE
  LNBH21_Context               lnbh21_Obj;
  LNBH21_Context              *lnbh21;
 #endif /* LNBH21_COMPATIBLE */
 #ifdef    LNBH25_COMPATIBLE
  LNBH25_Context               lnbh25_Obj;
  LNBH25_Context              *lnbh25;
 #endif /* LNBH25_COMPATIBLE */
 #ifdef    LNBH26_COMPATIBLE
  LNBH26_Context               lnbh26_Obj;
  LNBH26_Context              *lnbh26;
 #endif /* LNBH26_COMPATIBLE */
 #ifdef    LNBH29_COMPATIBLE
  LNBH29_Context               lnbh29_Obj;
  LNBH29_Context              *lnbh29;
 #endif /* LNBH29_COMPATIBLE */
 #ifdef    A8293_COMPATIBLE
  A8293_Context               A8293_Obj;
  A8293_Context              *A8293;
 #endif /* A8293_COMPATIBLE */
 #ifdef    UNICABLE_COMPATIBLE
  unsigned char                lnb_type;
  SILABS_Unicable_Context      Unicable_Obj;
  SILABS_Unicable_Context     *unicable;
 #endif /* UNICABLE_COMPATIBLE */
  /* Configuration fields storage */
  signed int   demod_add;
  unsigned int   SPI_Setup_send_option;
  unsigned int   SPI_Setup_clk_pin;
  unsigned int   SPI_Setup_clk_pola;
  unsigned int   SPI_Setup_data_pin;
  unsigned int   SPI_Setup_data_order;
  signed int   Select_TER_Tuner_ter_tuner_code;
  signed int   Select_TER_Tuner_ter_tuner_index;
  signed int   TER_Address_add;
  signed int   TER_Tuner_ClockConfig_xtal;
  signed int   TER_Tuner_ClockConfig_xout;
  signed int   TER_Clock_clock_source;
  signed int   TER_Clock_clock_input;
  signed int   TER_Clock_clock_freq;
  signed int   TER_Clock_clock_control;
  signed int   TER_FEF_Config_fef_mode;
  signed int   TER_FEF_Config_fef_pin;
  signed int   TER_FEF_Config_fef_level;
  signed int   TER_AGC_agc1_mode;
  signed int   TER_AGC_agc1_inversion;
  signed int   TER_AGC_agc2_mode;
  signed int   TER_AGC_agc2_inversion;
  signed int   TER_Tuner_AGC_Input_dtv_agc_source;
  signed int   TER_Tuner_FEF_Input_dtv_fef_freeze_input;
  signed int   TER_Tuner_IF_Output_dtv_out_type;
  signed int   Select_SAT_Tuner_sat_tuner_code;
  signed int   Select_SAT_Tuner_sat_tuner_index;
  signed int   Select_SAT_Tuner_sat_tuner_sub;
  signed int   SAT_Address_add;
  signed int   SAT_Clock_clock_source;
  signed int   SAT_Clock_clock_input;
  signed int   SAT_Clock_clock_freq;
  signed int   SAT_Clock_clock_control;
  signed int   SAT_AGC_agc1_mode;
  signed int   SAT_AGC_agc1_inversion;
  signed int   SAT_AGC_agc2_mode;
  signed int   SAT_AGC_agc2_inversion;
  signed int   SAT_Spectrum_spectrum_inversion;
  signed int   SAT_Select_LNB_Chip_lnb_code;
  signed int   SAT_Select_LNB_Chip_lnb_index;
  signed int   SAT_Select_LNB_Chip_lnb_chip_address;
} SILABS_FE_Context;

typedef struct _CUSTOM_Status_Struct          {
// TODO (mdorval#1#): Add cber in status structure
  CUSTOM_Standard_Enum standard;
  signed int      clock_mode;
  signed int      never_lock;//csm,13'
  signed int      pcl_lock;//20130807,csm,change(demod_lock -> pcl_lock)
  signed int      dl_lock;//20130807,csm,change(fec_lock -> dl_lock)
  signed int      dl_lock_in_range;//20130807,csm,change(fec_lock_in_range -> dl_lock_in_range)
  signed int      demod_die;
//hjh_change 
  signed int      ber_20000;
  signed int      per_2000;
  signed int      ber_e8;//hjh_change1
//  signed int      ber; //mant*10+exp;
  signed int      ber_2000;
//hjh change end  
  signed int      c_n_100;
  signed int      ber_mant;
  signed int      ber_exp;
  signed int      per_mant;
  signed int      per_exp;
  signed int      fer_mant;
  signed int      fer_exp;
  signed int      uncorrs;
  signed int      RFagc;
  signed int      IFagc;
  signed int      RFlevel;
  signed long     freq_offset;
  signed long     timing_offset;
  signed int      bandwidth_Hz;
  signed int      stream;
  signed int      fft_mode;
  signed int      guard_interval;
  signed int      constellation;
  unsigned int symbol_rate;
  signed int      code_rate_hp;
  signed int      code_rate_lp;
  signed int      hierarchy;
  signed int      spectral_inversion;
  signed int      code_rate;
  signed int      pilots;
  signed int      cell_id;
  signed int      RSSI;
  signed int      SSI;
  signed int      SQI;
  signed int      SatCRAGC;//csm,13'
  signed int      tuner_lock;
  signed int      rotated;
  signed int      pilot_pattern;
  signed int      bw_ext;
  signed int      TS_bitrate_kHz;
  signed int      TS_clock_kHz;
  /*        T2 specifics (FEF) */
  signed int      fef_length;
  signed int      plp_type;
  signed int      t2_system_id;
  signed int      t2_base_lite;
  signed int      t2_version;
  #ifdef SEM_DEBUG
  signed int      t2_scrambled;
  #endif
  /* End of T2 specifics (FEF) */
  /* Start of ISDB-T specifics   */
  signed int      isdbt_system_id;
  signed int      nb_seg_a;
  signed int      nb_seg_b;
  signed int      nb_seg_c;
  signed int      partial_flag;
  /* End  of ISDB-T specifics   */
  /*        T2/C2 specifics (PLP) */
  signed int      num_plp;
  signed int      plp_id;
  signed int      ds_id;
  signed int      plp_payload_type;
  /* End of T2/C2 specifics (PLP) */
  /*        S2X   specifics (ISI) */
  signed int      num_is;
  signed int      isi_id;
  signed int      isi_constellation;
  signed int      isi_code_rate;
  unsigned int    s2x;
  /* End of S2X   specifics (ISI) */
  signed int      tx_mode;
  signed int      short_frame;
  unsigned char attctrl;
  /* TUNER_STATUS */
  unsigned char   tcint;
  unsigned char   rssilint;
  unsigned char   rssihint;
           signed int    vco_code;
  unsigned char   tc;
  unsigned char   rssil;
  unsigned char   rssih;
    signed char   rssi;
  unsigned long   freq;
  unsigned char   mode;
  unsigned long   lo_freq;
  unsigned char   sample_freq;
  /* ATV_STATUS & DTV_STATUS */
  unsigned char   chl;
  /* ATV_STATUS */
  signed int      ATV_Sync_Lock;
  signed int      ATV_Master_Lock;
  unsigned char   audio_chan_filt_bw;
  unsigned char   snrl;
  unsigned char   snrh;
  unsigned char   video_snr;
           signed int    afc_freq;
           signed int    video_sc_spacing;
  unsigned char   video_sys;
  unsigned char   color;
  unsigned char   trans;
  unsigned char   audio_sys;
  unsigned char   audio_demod_mode;
  /* DTV_STATUS */
  unsigned char   chlint;
  unsigned char   bw;
  unsigned char   modulation;
  unsigned char   fef;
  /* MCNS STATUS */
  unsigned char   interleaving;
} CUSTOM_Status_Struct;

/* define how many front-ends will be used */
#ifndef   FRONT_END_COUNT
  #define FRONT_END_COUNT  2
#endif /* FRONT_END_COUNT */
extern SILABS_FE_Context     FrontEnd_Table[FRONT_END_COUNT];

//csm,13'
typedef struct SiLabs_Carriers_Struct {
  signed int            carriers_count ;
  signed int           *standard       ;
  signed int           *freq           ;
  signed int           *bandwidth_Hz   ;
  signed int           *stream         ;
  unsigned int  *symbol_rate_bps;
  signed int           *constellation  ;
  signed int           *polarization   ;
  signed int           *band           ;
  signed int           *data_slice_id  ;
  signed int           *plp_id         ;
  signed int           *T2_base_lite   ;
} SiLabs_Carriers_Struct;

extern SiLabs_Carriers_Struct *Carriers_Table;
//end


/* Translation functions from 'Custom' values to 'SiLabs' values */

signed int   Silabs_standardCode                 (SILABS_FE_Context *front_end, CUSTOM_Standard_Enum          standard);
signed int   Silabs_constelCode                  (SILABS_FE_Context *front_end, CUSTOM_Constel_Enum           constel);
signed int   Silabs_streamCode                   (SILABS_FE_Context *front_end, CUSTOM_Stream_Enum            stream);
signed int   Silabs_plptypeCode                  (SILABS_FE_Context *front_end, CUSTOM_T2_PLP_TYPE            plp_type);
signed int   Silabs_fftCode                      (SILABS_FE_Context *front_end, CUSTOM_FFT_Mode_Enum          fft);
signed int   Silabs_giCode                       (SILABS_FE_Context *front_end, CUSTOM_GI_Enum                gi);
signed int   Silabs_coderateCode                 (SILABS_FE_Context *front_end, CUSTOM_Coderate_Enum          coderate);
signed int   Silabs_hierarchyCode                (SILABS_FE_Context *front_end, CUSTOM_Hierarchy_Enum         hierarchy);
signed int   Silabs_pilotPatternCode             (SILABS_FE_Context *front_end, CUSTOM_Pilot_Pattern_Enum     pilot_pattern);
signed int   Silabs_T2VersionCode                (SILABS_FE_Context* front_end, CUSTOM_Pilot_T2_version_Enum  t2_version);
signed int   Silabs_videoSysCode                 (SILABS_FE_Context *front_end, CUSTOM_Video_Sys_Enum         video_sys);
signed int   Silabs_colorCode                    (SILABS_FE_Context *front_end, CUSTOM_Color_Enum             color);
signed int   Silabs_transmissionCode             (SILABS_FE_Context *front_end, CUSTOM_Transmission_Mode_Enum trans);
signed int   Silabs_audioSysCode                 (SILABS_FE_Context* front_end, CUSTOM_Audio_Sys_Enum audio_sys);//csm,13'
/* Translation functions from 'SiLabs' values to 'Custom' values */

signed int   Custom_standardCode                 (SILABS_FE_Context *front_end, signed int standard);
signed int   Custom_constelCode                  (SILABS_FE_Context *front_end, signed int constel);
signed int   Custom_streamCode                   (SILABS_FE_Context *front_end, signed int stream);
signed int   Custom_plptypeCode                  (SILABS_FE_Context *front_end, signed int plp_type);
signed int   Custom_fftCode                      (SILABS_FE_Context *front_end, signed int fft);
signed int   Custom_giCode                       (SILABS_FE_Context *front_end, signed int gi);
signed int   Custom_coderateCode                 (SILABS_FE_Context *front_end, signed int coderate);
signed int   Custom_hierarchyCode                (SILABS_FE_Context *front_end, signed int hierarchy);
signed int   Custom_pilotPatternCode             (SILABS_FE_Context *front_end, signed int pilot_pattern);
signed int   Custom_T2VersionCode                (SILABS_FE_Context *front_end, signed int t2_version);

/* Text functions returning strings based on 'Custom' values. */

const unsigned char *Silabs_Standard_Text                (CUSTOM_Standard_Enum     standard);
signed int   Silabs_Standard_Capability          		(CUSTOM_Standard_Enum     standard);
const unsigned char *Silabs_Constel_Text                 (CUSTOM_Constel_Enum      constel);
const unsigned char *Silabs_Stream_Text                  (CUSTOM_Stream_Enum       stream);
const unsigned char *Silabs_TS_Mode_Text                 (CUSTOM_TS_Mode_Enum      ts_mode);
const unsigned char *Silabs_PLPType_Text                 (CUSTOM_T2_PLP_TYPE       plp_type);
const unsigned char *Silabs_FFT_Text                     (CUSTOM_FFT_Mode_Enum     fft);
const unsigned char *Silabs_GI_Text                      (CUSTOM_GI_Enum           gi);
const unsigned char *Silabs_Coderate_Text                (CUSTOM_Coderate_Enum     coderate);
const unsigned char *Silabs_Polarization_Text            (CUSTOM_Polarization_Enum polarization);
const unsigned char *Silabs_Band_Text                    (CUSTOM_Band_Enum         band);
const unsigned char *Silabs_Extended_BW_Text             (signed int bw_extended);
const unsigned char *Silabs_Rotated_QAM_Text             (signed int rotated);
const unsigned char *Silabs_T2_Base_Lite_Text            (signed int t2_base_lite);
const unsigned char *Silabs_FEF_Text                     (signed int fef);
const unsigned char *Silabs_MISO_Text                    (signed int siso_miso);
const unsigned char *Silabs_Pilot_Pattern_Text           (CUSTOM_Pilot_Pattern_Enum pilot_pattern);
const unsigned char *Silabs_T2_Version_Text              (CUSTOM_Pilot_T2_version_Enum t2_version);
const unsigned char *Silabs_No_Short_Frame_Text          (signed int no_short_frame);
const unsigned char *Silabs_Pilots_Text                  (signed int pilots);

/* Chip detection function (To Be Defined) */

signed int   SiLabs_chip_detect                  (signed int demodAdd);

/* Internal functions                      */

signed int   SiLabs_API_Demod_status             (SILABS_FE_Context *front_end, CUSTOM_Status_Struct *status);
signed int   SiLabs_API_Demod_status_selection   (SILABS_FE_Context *front_end, CUSTOM_Status_Struct *status, unsigned char status_selection);
signed int   SiLabs_API_TER_Tuner_status         (SILABS_FE_Context *front_end, CUSTOM_Status_Struct *status);
signed int   SiLabs_API_SAT_Tuner_status         (SILABS_FE_Context *front_end, CUSTOM_Status_Struct *status);
signed int   SiLabs_API_FE_status                (SILABS_FE_Context *front_end, CUSTOM_Status_Struct *status);
signed int   SiLabs_API_FE_status_selection      (SILABS_FE_Context *front_end, CUSTOM_Status_Struct *status, unsigned char status_selection);
signed int   SiLabs_API_Lock_status              (SILABS_FE_Context *front_end, CUSTOM_Status_Struct *status);
signed int   SiLabs_API_Text_status              (SILABS_FE_Context *front_end, CUSTOM_Status_Struct *status, unsigned char *formatted_status);
signed int   SiLabs_API_DVB_T2_RSSI_status       (SILABS_FE_Context *front_end, CUSTOM_Status_Struct *status);
signed int   SiLabs_API_Text_status_selection    (SILABS_FE_Context *front_end, CUSTOM_Status_Struct *status, unsigned char *formatted_status, unsigned char status_selection);

/*****************************************************************************************/
/*               SiLabs demodulator API functions (demod and tuner)                      */
/*****************************************************************************************/
/* Main SW init function (to be called first)                                            */
unsigned char  SiLabs_API_SW_Init                  (SILABS_FE_Context *front_end, signed int demodAdd, signed int tunerAdd_Ter, signed int tunerAdd_Sat);
signed int   SiLabs_API_Get_TS_Dividers          (SILABS_FE_Context *front_end, unsigned int *div_a, unsigned int *div_b);
signed int   Silabs_API_TS_Tone_Cancel           (SILABS_FE_Context* front_end, signed int on_off);
//signed int   SiLabs_API_Set_Index_and_Tag        (SILABS_FE_Context *front_end, unsigned char index, unsigned char* tag); // 20140513 del jhkwon
signed int   SiLabs_API_Frontend_Chip            (SILABS_FE_Context *front_end, signed int demod_id);
signed int   SiLabs_API_Handshake_Setup          (SILABS_FE_Context *front_end, signed int handshake_mode, signed int handshake_period_ms);
signed int   SiLabs_API_SPI_Setup                (SILABS_FE_Context *front_end, unsigned int send_option, unsigned int clk_pin, unsigned int clk_pola, unsigned int data_pin, unsigned int data_order);
/* TER tuner selection and configuration functions (to be used after SiLabs_API_SW_Init) */
signed int   SiLabs_API_TER_Possible_Tuners      (SILABS_FE_Context *front_end, unsigned char *tunerList);
signed int   SiLabs_API_Select_TER_Tuner         (SILABS_FE_Context *front_end, signed int   ter_tuner_code, signed int ter_tuner_index);
void* SiLabs_API_TER_Tuner                (SILABS_FE_Context *front_end);
signed int   SiLabs_API_TER_tuner_I2C_connection (SILABS_FE_Context *front_end, signed int   fe_index);
signed int   SiLabs_API_TER_Address              (SILABS_FE_Context *front_end, signed int   add);
signed int   SiLabs_API_TER_Broadcast_I2C        (SILABS_FE_Context *front_ends[], signed int front_end_count);
signed int   SiLabs_API_TER_Tuner_ClockConfig    (SILABS_FE_Context *front_end, signed int xtal, signed int xout);
signed int   SiLabs_API_TER_Clock_Options        (SILABS_FE_Context *front_end, unsigned char *clockOptions);
signed int   SiLabs_API_TER_Clock                (SILABS_FE_Context *front_end, signed int   clock_source, signed int clock_input, signed int clock_freq, signed int clock_control);
signed int   SiLabs_API_TER_FEF_Options          (SILABS_FE_Context *front_end, unsigned char* fefOptions);
signed int   SiLabs_API_TER_FEF_Config           (SILABS_FE_Context *front_end, signed int   fef_mode, signed int fef_pin, signed int fef_level);
signed int   SiLabs_API_TER_AGC_Options          (SILABS_FE_Context *front_end, unsigned char *agcOptions);
signed int   SiLabs_API_TER_AGC                  (SILABS_FE_Context *front_end, signed int   agc1_mode,    signed int agc1_inversion, signed int agc2_mode, signed int agc2_inversion);
signed int   SiLabs_API_TER_Tuner_AGC_Input      (SILABS_FE_Context *front_end, signed int   dtv_agc_source);
signed int   SiLabs_API_TER_Tuner_FEF_Input      (SILABS_FE_Context *front_end, signed int   dtv_fef_freeze_input);
signed int   SiLabs_API_TER_Tuner_IF_Output      (SILABS_FE_Context *front_end, signed int   dtv_out_type);
/* SAT tuner selection and configuration functions (to be used after SiLabs_API_SW_Init) */
signed int   SiLabs_API_SAT_Possible_Tuners      (SILABS_FE_Context *front_end, unsigned char *tunerList);
signed int   SiLabs_API_Select_SAT_Tuner         (SILABS_FE_Context *front_end, signed int   sat_tuner_code, signed int sat_tuner_index);
signed int   SiLabs_API_SAT_Tuner_Sub            (SILABS_FE_Context *front_end, signed   int   sat_tuner_sub);
signed int   SiLabs_API_SAT_tuner_I2C_connection (SILABS_FE_Context *front_end, signed int   fe_index);
signed int   SiLabs_API_SAT_Address              (SILABS_FE_Context *front_end, signed int   add);
signed int   SiLabs_API_SAT_Clock_Options        (SILABS_FE_Context *front_end, unsigned char *clockOptions);
signed int   SiLabs_API_SAT_Clock                (SILABS_FE_Context *front_end, signed int   clock_source, signed int clock_input, signed int clock_freq, signed int clock_control);
signed int   SiLabs_API_SAT_AGC_Options          (SILABS_FE_Context *front_end, unsigned char *agcOptions);
signed int   SiLabs_API_SAT_AGC                  (SILABS_FE_Context *front_end, signed int   agc1_mode,    signed int agc1_inversion, signed int agc2_mode, signed int agc2_inversion);
signed int   SiLabs_API_SAT_Spectrum             (SILABS_FE_Context *front_end, signed int   spectrum_inversion);
signed int   SiLabs_API_SAT_Possible_LNB_Chips   (SILABS_FE_Context *front_end, unsigned char *lnbList);
signed int   SiLabs_API_SAT_Select_LNB_Chip      (SILABS_FE_Context *front_end, signed int   lnb_code, signed int lnb_chip_address);
signed int   SiLabs_API_SAT_LNB_Chip_Index       (SILABS_FE_Context *front_end, signed  int lnb_index);

/* Front_end info, control and status functions                                          */
signed int   SiLabs_API_Infos                    (SILABS_FE_Context *front_end, unsigned char *infoString);
signed int   SiLabs_API_Config_Infos             (SILABS_FE_Context *front_end, unsigned char* config_function, unsigned char *infoString);
unsigned char  SiLabs_API_HW_Connect               (SILABS_FE_Context *front_end, CONNECTION_TYPE connection_mode);
signed char  SiLabs_API_bytes_trace              (SILABS_FE_Context *front_end, unsigned char track_mode);
signed int   SiLabs_API_ReadString               (SILABS_FE_Context *front_end, unsigned char *readString, unsigned char *pbtDataBuffer);
unsigned char  SiLabs_API_WriteString              (SILABS_FE_Context *front_end, unsigned char *writeString);
signed int   SiLabs_API_communication_check      (SILABS_FE_Context *front_end);
signed int   SiLabs_API_switch_to_standard       (SILABS_FE_Context *front_end, unsigned char standard, unsigned char force_full_init);
signed int   SiLabs_API_set_standard             (SILABS_FE_Context *front_end, unsigned char standard);
signed int   SiLabs_API_lock_to_carrier          (SILABS_FE_Context *front_end,
                                           unsigned char standard,
                                           signed int           freq,
                                           signed int           bandwidth_Hz,
                                           unsigned char stream,
                                           unsigned int  symbol_rate_bps,
                                           unsigned char constellation,
                                           unsigned char polarization,
                                           unsigned char band,
                                           signed int           data_slice_id,
                                           signed int           plp_id,
                                           unsigned char T2_lock_mode
                                           );
signed int   SiLabs_API_Tune                     (SILABS_FE_Context *front_end, signed int   freq);
signed int   SiLabs_API_Channel_Lock_Abort       (SILABS_FE_Context *front_end);
signed int   SiLabs_API_Demod_Standby            (SILABS_FE_Context *front_end);
signed int   SiLabs_API_Demod_Wakeup             (SILABS_FE_Context *front_end);
signed int   SiLabs_API_Demod_Silent             (SILABS_FE_Context *front_end, signed int silent);
signed int   SiLabs_API_Demod_ClockOn            (SILABS_FE_Context *front_end);
signed int   SiLabs_API_Reset_Uncorrs            (SILABS_FE_Context *front_end);
signed int   SiLabs_API_TS_Mode                  (SILABS_FE_Context *front_end, signed int   ts_mode);
signed int   SiLabs_API_Get_TS_Dividers          (SILABS_FE_Context *front_end, unsigned int *div_a, unsigned int *div_b);
signed int   Silabs_API_TS_Tone_Cancel           (SILABS_FE_Context* front_end, signed int on_off);
signed int   SiLabs_API_Tuner_I2C_Enable         (SILABS_FE_Context *front_end);
signed int   SiLabs_API_Tuner_I2C_Disable        (SILABS_FE_Context *front_end);
/* Scan functions                                                                        */
signed int   SiLabs_API_Channel_Seek_Init        (SILABS_FE_Context *front_end,
                                            signed int rangeMin,     signed int rangeMax,
                                            signed int seekBWHz,     signed int seekStepHz,
                                            signed int minSRbps,     signed int maxSRbps,
                                            signed int minRSSIdBm,   signed int maxRSSIdBm,
                                            signed int minSNRHalfdB, signed int maxSNRHalfdB);
signed int   SiLabs_API_Channel_Seek_Next        (SILABS_FE_Context *front_end, signed int  *standard, signed int *freq, signed int *bandwidth_Hz, signed int *stream, unsigned int *symbol_rate_bps, signed int *constellation, signed int *polarization, signed int *band, signed int *num_data_slice, signed int *num_plp, signed int *T2_base_lite);
signed int   SiLabs_API_Channel_Seek_Abort       (SILABS_FE_Context *front_end);
signed int   SiLabs_API_Channel_Seek_End         (SILABS_FE_Context *front_end);
//csm,13'
signed int   SiLabs_API_TS_Clk_Shift_Read        (SILABS_FE_Context *front_end);
signed int   SiLabs_API_TS_Clk_Setup_Parallel    (SILABS_FE_Context *front_end, unsigned char ts_clk_shift, unsigned char ts_clk_invert);
signed int   SiLabs_API_TS_Clk_Setup_Serial      (SILABS_FE_Context *front_end, unsigned char ts_clk_invert);
signed int   SiLabs_API_Set_DTV_Agc_Speed        (SILABS_FE_Context *front_end, unsigned char dtv_if_agc_speed, unsigned char dtv_if_decim);
signed int   SiLabs_API_Set_DTV_AGC_AUTO_FREEZE  (SILABS_FE_Context *front_end, signed int fef);
signed int   SiLabs_API_Setup_BER_Resol_exp      (SILABS_FE_Context *front_end, unsigned char isdvbtscan);
signed int   SiLabs_API_Setup_DVBC_AFC_RANGE     (SILABS_FE_Context *front_end, signed int SR);
signed int   SiLabs_API_Set_Invert_Spectrum      (SILABS_FE_Context *front_end, unsigned char invert_spectrum);
signed int   SiLabs_API_Setup_TS_Parallel_Strength_Shape(SILABS_FE_Context *front_end, unsigned char data_strength, unsigned char data_shape, unsigned char clk_strength, unsigned char clk_shape);
signed int   SiLabs_API_Setup_TS_Serial_Strength_Shape  (SILABS_FE_Context *front_end, unsigned char data_strength, unsigned char data_shape, unsigned char clk_strength, unsigned char clk_shape);
signed int   SiLabs_API_Setup_Ter_TS_SLR_SERIAL  (SILABS_FE_Context *front_end);
//end
signed int   SiLabs_API_SAT_AutoDetectCheck      (SILABS_FE_Context *front_end);
signed int   SiLabs_API_SAT_AutoDetect           (SILABS_FE_Context *front_end, signed int  on_off);
signed int   SiLabs_API_SAT_Tuner_Init           (SILABS_FE_Context *front_end);
signed int   SiLabs_API_SAT_Tuner_STANDBY        (SILABS_FE_Context *front_end);
signed int   SiLabs_API_SAT_Tuner_WAKEUP         (SILABS_FE_Context *front_end);
signed int   SiLabs_API_SAT_Tuner_SetLPF         (SILABS_FE_Context *front_end, signed int  lpf_khz);
signed int   SiLabs_API_SAT_voltage              (SILABS_FE_Context *front_end, signed int  voltage);
signed int   SiLabs_API_SAT_tone                 (SILABS_FE_Context *front_end, unsigned char tone);
signed int   SiLabs_API_SAT_Tuner_PLLlock        (SILABS_FE_Context *front_end);//csm,13'
signed int   SiLabs_API_SAT_Tuner_RfSel          (SILABS_FE_Context *front_end, unsigned char rf_chn);//kwon, 14 quad
signed int   SiLabs_API_SAT_Tuner_RfSel_Read     (SILABS_FE_Context *front_end, unsigned char *same_side, unsigned char *diff_side);
signed int   SiLabs_API_SAT_voltage_and_tone     (SILABS_FE_Context *front_end, signed int  voltage, unsigned char tone);
signed int   SiLabs_API_SAT_prepare_diseqc_sequence(SILABS_FE_Context *front_end, signed int  sequence_length, unsigned char *sequence_buffer, unsigned char cont_tone, unsigned char tone_burst, unsigned char burst_sel, unsigned char end_seq, signed int *flags);
signed int   SiLabs_API_SAT_trigger_diseqc_sequence(SILABS_FE_Context *front_end, signed int  flags);
signed int   SiLabs_API_SAT_send_diseqc_sequence   (SILABS_FE_Context *front_end, signed int  sequence_length, unsigned char *sequence_buffer, unsigned char cont_tone, unsigned char tone_burst, unsigned char burst_sel, unsigned char end_seq);
signed int   SiLabs_API_SAT_read_diseqc_reply      (SILABS_FE_Context *front_end, signed int *reply_length   , unsigned char *reply_buffer   );
signed int   SiLabs_API_SAT_set_diseqc_freq      (SILABS_FE_Context *front_end, unsigned int diseqc_freq_hz);
signed int   SiLabs_API_Setup_Sat_TS_SLR_SERIAL  (SILABS_FE_Context *front_end, signed int SR);
signed int   SiLabs_API_Setup_Sat_AFC_range	  (SILABS_FE_Context *front_end, signed int SR, unsigned char fullscan);//20130628,csm,13'
signed int   SiLabs_API_SAT_Tuner_Tune           (SILABS_FE_Context *front_end, signed int freq_kHz);
signed int   SiLabs_API_SAT_Get_AGC              (SILABS_FE_Context *front_end);
#ifdef    UNICABLE_COMPATIBLE
signed int   SiLabs_API_SAT_Unicable_Install     (SILABS_FE_Context *front_end);
signed int   SiLabs_API_SAT_Unicable_Uninstall   (SILABS_FE_Context *front_end);
signed int   SiLabs_API_SAT_Unicable_Tune        (SILABS_FE_Context *front_end, signed int freq_kHz);
#endif /* UNICABLE_COMPATIBLE */
signed int   SiLabs_API_Get_DVBT_Hierarchy       (SILABS_FE_Context *front_end, signed int  *hierarchy);
signed int   SiLabs_API_Get_DS_ID_Num_PLP_Freq   (SILABS_FE_Context *front_end, signed int  ds_index, signed int *data_slice_id, signed int *num_plp, signed int *freq);
signed int   SiLabs_API_Get_PLP_ID_and_TYPE      (SILABS_FE_Context *front_end, signed int  ds_id, signed int  plp_index, signed int *plp_id, signed int *plp_type);
signed int   SiLabs_API_Select_PLP               (SILABS_FE_Context *front_end, signed int  plp_id);
signed int   SiLabs_API_TER_AutoDetect           (SILABS_FE_Context *front_end, signed int  on_off);
signed int   SiLabs_API_TER_T2_lock_mode         (SILABS_FE_Context *front_end, signed int  T2_lock_mode);
signed int   SiLabs_API_TER_Tuner_Fine_Tune      (SILABS_FE_Context *front_end, signed int  offset_500hz);
signed int   SiLabs_API_TER_Tuner_Init           (SILABS_FE_Context *front_end);
signed int   SiLabs_API_TER_Tuner_PowerUpOnly    (SILABS_FE_Context *front_end);//csm,13'
signed int   SiLabs_API_TER_Tuner_Standby        (SILABS_FE_Context *front_end);
signed int   SiLabs_API_TER_Tuner_Wakeup         (SILABS_FE_Context *front_end);
signed int   SiLabs_API_TER_Tuner_SetMFT         (SILABS_FE_Context *front_end, signed int   nStep);
signed int   SiLabs_API_TER_Tuner_Text_status    (SILABS_FE_Context *front_end, unsigned char *separator, unsigned char *msg);
signed int   SiLabs_API_TER_Tuner_ATV_Text_status(SILABS_FE_Context *front_end, unsigned char *separator, unsigned char *msg);
signed int   SiLabs_API_TER_Tuner_DTV_Text_status(SILABS_FE_Context *front_end, unsigned char *separator, unsigned char *msg);

//csm,13'
signed int   SiLabs_API_TER_Tuner_SetDtvRfTop    (SILABS_FE_Context *front_end,unsigned char rf_top);
signed int   SiLabs_API_TER_Tuner_DTV_RESTART    (SILABS_FE_Context *front_end);

//end
signed int   SiLabs_API_Demod_Dual_Driving_Xtal  (SILABS_FE_Context  *front_end_driving_xtal, SILABS_FE_Context  *front_end_receiving_xtal);
signed int   SiLabs_API_Demods_Kickstart         (void);
signed int   SiLabs_API_TER_Tuners_Kickstart     (void);
int   SiLabs_API_Cypress_Ports            (SILABS_FE_Context *front_end, unsigned char OEA, unsigned char IOA, unsigned char OEB, unsigned char IOB, unsigned char OED, unsigned char IOD );
void  SiLabs_API_Demod_reset              (SILABS_FE_Context *front_end);
int   SiLabs_API_Store_FW                 (SILABS_FE_Context *front_end, firmware_struct fw_table[], signed int nbLines);
int   SiLabs_API_Store_SPI_FW             (SILABS_FE_Context *front_end, unsigned char   fw_table[], signed int nbBytes);

signed int   SiLabs_API_Auto_Detect_Demods       (L0_Context* i2c, signed int *Nb_FrontEnd, signed int demod_code[4], signed int demod_add[4], unsigned char *demod_string[4]);

signed int   SiLabs_API_TER_Tuner_I2C_Enable     (SILABS_FE_Context *front_end);
signed int   SiLabs_API_TER_Tuner_I2C_Disable    (SILABS_FE_Context *front_end);
signed int   SiLabs_API_SAT_Tuner_I2C_Enable     (SILABS_FE_Context *front_end);
signed int   SiLabs_API_SAT_Tuner_I2C_Disable    (SILABS_FE_Context *front_end);
int   SiLabs_API_Get_Stream_Info          (SILABS_FE_Context *front_end, signed int  isi_index, signed int *isi_id, signed int *isi_constellation, signed int *isi_code_rate);
int   SiLabs_API_Select_Stream            (SILABS_FE_Context *front_end, signed int stream_id);
signed int   SiLabs_API_TER_Tuner_Dual_Driving_Xtal  (SILABS_FE_Context  *front_end_driving_xtal, SILABS_FE_Context  *front_end_receiving_xtal);


signed int   SiLabs_Scan_Table_Init          (void);
signed int   SiLabs_Scan_Table_Clear         (void);
signed int   SiLabs_Scan_Table_Count         (void);
signed int   SiLabs_Scan_Table_AddOneCarrier (signed int  standard, signed int  freq, signed int  bandwidth_Hz, signed int  stream, unsigned int  symbol_rate_bps, signed int  constellation, signed int  polarization, signed int  band, signed int  data_slice_id, signed int  plp_id, signed int  T2_base_lite);
signed int   SiLabs_API_T2_MPLP              (SILABS_FE_Context *front_end, signed int freq);
//end
void SiLabs_API_TER_XtalConfig(SILABS_FE_Context *front_end, signed int clk_mode);//csm, add, to configure TER clock mode
const unsigned char* SiLabs_API_TAG_TEXT      (void);

//20130903,related to kernel driver,csm
void Tuner_GetFwInfo(SILABS_FE_Context *front_end, unsigned long *fw_addr, unsigned long *fw_size);
void Demod_GetFwInfo(SILABS_FE_Context *front_end, unsigned long *fw_addr, unsigned long *fw_size);
//end
signed int SiLabs_API_firmwares_download(SILABS_FE_Context *front_end, signed int clk_mode, signed int demodAdd, signed int tunerAdd_Ter, signed int tunerAdd_Sat);//20130903

#if defined( __cplusplus )
}
#endif

#endif /* _SiLabs_API_L3_Wrapper_H_ */

