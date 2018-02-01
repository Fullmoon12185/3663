/*************************************************************************************
                  Silicon Laboratories Broadcast Si2164B Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API types used by commands and properties
   FILE: Si2164B_typedefs.h
   Supported IC : Si2164B
   Compiled for ROM 2 firmware 5_B_build_1
   Revision: V0.1.1.0
   Date: March 30 2015
  (C) Copyright 2015, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
/* Change log:

 As from V0.0.9.0
  <new_feature>[FW/From File] Moving definition of FW structure to allow using a pointer to the structure in the L1_Si2164B_Context

 As from V0.0.7.0: <new_feature>[SPI/logs] Adding spi_download_ms and i2c_download_ms in Si2164B_L1_Context.
    These values will be used to monitor the FW download times in SPI and I2C modes

 As from V0.0.6.0:
    <improvement>[Code_size] Adding a textBuffer in Si2164B_L1_Context used when filling text strings

 As from V0.0.1.0:
  Adding TER_tuner_agc_input and TER_tuner_if_output to L1 context,
    to configure the TER IF interface
  Adding comments in L1_Si2164B_Context to match first version of Si2164B documentation.
  Removing unused members of L1_Si2164B_Context

 As from V0.0.0.0:
  Initial version (based on Si2164 code V0.3.4)
****************************************************************************************/
#ifndef   Si2164B_TYPEDEFS_H
#define   Si2164B_TYPEDEFS_H

/* _additional_defines_point */
#define Si2164B_MAX_LENGTH         80

#define Si2164B_DOWNLOAD_ON_CHANGE 1
#define Si2164B_DOWNLOAD_ALWAYS    0

  #define Si2164B_TERRESTRIAL 1

  #define Si2164B_DVBT_MIN_LOCK_TIME    100
  #define Si2164B_DVBT_MAX_LOCK_TIME   2000


#define Si2164B_CLOCK_ALWAYS_OFF 0
#define Si2164B_CLOCK_ALWAYS_ON  1
#define Si2164B_CLOCK_MANAGED    2

  #define Si2164B_DVBT2_MIN_LOCK_TIME       100
  #define Si2164B_DVBT2_MAX_LOCK_TIME      2000

  #define Si2164B_DVBC_MIN_LOCK_TIME     20
  #define Si2164B_DVBC_MAX_SEARCH_TIME 900//5000//20131217,Eric,to reduce lock time,5000->900

  #define Si2164B_DVBC2_MIN_LOCK_TIME     20
  #define Si2164B_DVBC2_MAX_LOCK_TIME   5000


  #define Si2164B_DVBS_MIN_LOCK_TIME     50
  #define Si2164B_DVBS_MAX_LOCK_TIME   1000

  #define Si2164B_DVBS2_MIN_LOCK_TIME    50
  #define Si2164B_DVBS2_MAX_LOCK_TIME  1000

  #define Si2164B_SAT_MAX_SEARCH_TIME 60000

  #define Si2164B_SATELLITE   2


#ifndef __FIRMWARE_STRUCT__
#define __FIRMWARE_STRUCT__
typedef struct  {
  unsigned char firmware_len;
  unsigned char firmware_table[16];
} firmware_struct;
#endif /* __FIRMWARE_STRUCT__ */

typedef struct _L1_Si2164B_Context {
  L0_Context                 *i2c;
  L0_Context                  i2cObj;
  Si2164B_CmdObj              *cmd;
  Si2164B_CmdObj               cmdObj;
  Si2164B_CmdReplyObj         *rsp;
  Si2164B_CmdReplyObj          rspObj;
  Si2164B_PropObj             *prop;
  Si2164B_PropObj              propObj;
  Si2164B_PropObj             *propShadow;
  Si2164B_PropObj              propShadowObj;
  Si2164B_COMMON_REPLY_struct *status;
  Si2164B_COMMON_REPLY_struct  statusObj;
  signed int                         standard;
  signed int                         media;
  signed int propertyWriteMode;                 /* Selection of DOWNLOAD_ALWAYS/DOWNLOAD_ON_CHANGE      */
                                         /*  for properties.                                     */
                                         /*  Use ‘DOWNLOAD_ALWAYS?only for debugging purposes   */
  unsigned int  tuner_ter_clock_source;  /* TER clock source configuration                       */
  unsigned int  tuner_ter_clock_freq;    /* TER clock frequency configuration                    */
  unsigned int  tuner_ter_clock_input;   /* TER clock input pin configuration                    */
  unsigned int  tuner_ter_clock_control; /* TER clock management configuration                   */
  unsigned int  tuner_ter_agc_control;   /* TER AGC output and polarity configuration            */
           signed int  TER_tuner_agc_input;     /* TER AGC input on tuner side configuration            */
           signed int  TER_tuner_if_output;     /* TER IF output on tuner side configuration            */
  unsigned int  fef_selection;           /* Preferred FEF mode. May not be possible              */
  unsigned int  fef_mode;                /* Final FEF mode, taking into account HW limitations   */
  unsigned int  fef_pin;                 /* FEF pin used for TER tuner AGC freeze                */
  unsigned int  fef_level;               /* GPIO state on FEF_pin when used (during FEF periods) */
  unsigned int                load_DVB_C_Blindlock_Patch;
  unsigned int  tuner_sat_clock_source;  /* SAT clock source configuration                       */
  unsigned int  tuner_sat_clock_freq;    /* SAT clock frequency configuration                    */
  unsigned int  tuner_sat_clock_input;   /* SAT clock input pin configuration                    */
  unsigned int  tuner_sat_clock_control; /* SAT clock management configuration                   */
  unsigned int  spi_download;
  unsigned int  spi_send_option;
  unsigned int  spi_clk_pin;
  unsigned int  spi_clk_pola;
  unsigned int  spi_data_pin;
  unsigned int  spi_data_order;
  unsigned int  spi_buffer_size;
  unsigned int  spi_download_ms;
  unsigned int  i2c_download_ms;
  int                nbLines;
  firmware_struct   *fw_table;
  int                nbSpiBytes;
  unsigned char     *spi_table;
} L1_Si2164B_Context;


#endif /* Si2164B_TYPEDEFS_H */







