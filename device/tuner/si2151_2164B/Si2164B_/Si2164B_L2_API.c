/***************************************************************************************
                  Silicon Laboratories Broadcast Si2164B Layer 2 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   L2 API for commands and properties
   FILE: Si2164B_L2_API.c
   Supported IC : Si2164B
   Compiled for ROM 2 firmware 5_B_build_1
   Revision: V0.1.1.0
   Tag:  V0.1.1.0
   Date: March 30 2015
  (C) Copyright 2015, Silicon Laboratories, Inc. All rights reserved.
****************************************************************7**********************/
/* Change log:

 As from  V0.1.1.0:
    <firmware> With FW 5_Bb2  on X5A parts
    <firmware> With FW 5_5b5  on X55 parts
    <new_feature>[TER_TUNER/DTV_INTERNAL_ZIF] Adding calls to SiLabs_TER_Tuner_DTV_INTERNAL_ZIF_DVBT to select the best internal IF configuration for the TER tuners
    <new_Part>[chiprev/3] In Si2164B_L2_Test: Adding compatibility with ROM2 parts.
    <new_feature>[SPI/split] Adding Si2164B_LoadFirmwareSPI_Split to allow sending FW over SPI in smaller portions (min SPI buffer size is currently 1024 bytes)
    <compatibility>[AUTO_T_T2] In Si2164B_TerAutoDetect: not setting front_end->auto_detect_TER for parts not supporting DVB-T2.
    <improvement>[traces] In Seek functions: adding dedicated traces to show the delays between DD_RESTART and the decision (lock/never lock) or the timeout as well as the
      cumulative durations corresponding to these.

 As from V0.1.0.0:
    <new_Part>[Si2164_C5A] In Si2164B_PowerUpWithPatch: Adding compatibility with Si2164_C5A.
    <wrapper> With wrapper V2.5.6
    <firmware> With FW 6_0b1  on X60 parts
    <firmware> With FW 5_3b4  on Si2180 X50 parts
    <firmware> With FW 5_5b4  on X55 parts (except Si2180)
    <firmware> With FW 5_Bb1  on X5A parts
    <improvement>[traces] In Si2164B_PowerUpWithPatch and Si2164B_LoadFirmwareSPI: typo correction with proper function name
    <improvement>[SILENT/DUAL] Si2164B_L2_SILENT updated to properly handle duals, taking into account pin usage restrictions:
        Die A can control MP_A, MP_C, GPIO1
        Die B can control MP_B, MP_D, GPIO0
    <improvement>[SLEEP/switch_to_standard] In Si2164B_L2_switch_to_standard: setting DD_MODE only
        when dtv_demod_needed = 1, to avoid calling this when in SLEEP mode.
    <compatibility>[Si2165D] In Si2164B_L2_Test, option 'demod/chip_detect': allowing detection of a
     non API-controlled part, by default considered being Si2165D.
    This assumes that the TER tuner address is 0xC0, to match SiLabs EVBs.
    <new_part> Adding support for Si2167B-22 (requires the compilation flag 'Si2167B_22_COMPATIBLE' )

 As from V0.0.8.0:
    <wrapper> Wrapper V2.5.5
    <firmware> With FW 5_0b15 on X50 parts (full download for ES parts, patch for production parts)
    <firmware> With FW 5_5b3  on X55 parts (except Si2180)
    <new_feature>[FW_from_table] In Si2164B_L2_SW_Init/Si2164B_PowerUpWithPatch: Adding the capability to load FW from a table, either over I2C or over SPI.
      NB: In Si2164B_L2_SW_Init: The corresponding lines using ¡®realloc¡¯ need to be commented if dynamic memory allocation is not allowed.

 As from V0.0.7.0:
    <wrapper> Wrapper V2.5.4
    <new_feature>[FW_from_table] Adding the capability to load FW from a table, either over I2C or over SPI.
    <new_feature>[SPI/logs]
     In Si2164B_LoadFirmware:    storing the FW download time in I2C mode
     In Si2164B_LoadFirmware_16: storing the FW download time in I2C mode
     In Si2164B_LoadFirmwareSPI: storing the FW download time in SPI mode
     In Si2164B_L2_Test : Adding the "download" "duration" option to display the FW download times
    <improvement>[traces/SPI] In Si2164B_PowerUpWithPatch: tracing api->spi_download
    <improvement>[Switch/DSS] In Si2164B_L2_switch_to_standard: Setting auto mode to 'AUTO_DVB_S_S2_DSS' only if new_standard is DSS

 As from V0.0.6.0:
    <wrapper> Wrapper V2.5.3
    <new_Part>[Si2166_C55] In Si2164B_PowerUpWithPatch: Adding compatibilty with Si2167_C55 and Si2166_C55.
    <firmware> With FW 5_0b14 on X50 parts (full download for ES parts, patch for production parts)
    <firmware> With FW 5_5b2  on X55 parts (except Si2180)
    <firmware> With FW 5_3b2  on Si2180 X50 parts
    <improvement>[scan/not_blind] In Si2164B_L2_Channel_Seek_Next: Checking front_end->seek_abort flag to allow an abort.
      The previous version only allowed seek aborting when in blind mode (for SAT and DVB-C).
      The previous version only allowed seek aborting when in blind mode (for SAT and DVB-C).
    <improvement>[suspend/resume] In Si2164B_L2_send_diseqc_sequence: Storing DiSEqC parameters in L1 context to allow saving them during 'resume'.
    <improvement>[SPI/SPIoverGPIF] In Si2164B_LoadFirmwareSPI: using new Cypress feature to load FW in SPI mode using GPIF (typical FW download time below 80 ms).
    <improvement>[DVB-C/timeout] In Si2164B_L2_lock_to_carrier; resetting front_end->searchStartTime after tuning is complete (if tuning), to be tuner-independent
      in the lock timeout management.
    <improvement>[DVB-C/porting] In Si2164B_DVB_C_max_lock_ms: removing float use.
    <improvement>[SEEK/NO_DVBT2] In Si2164B_L2_Channel_Seek_Next:
      Not allowing AUTO_DETECT in DVB-T for parts not supporting DVB-T2.
      This is done using the front_end->auto_detect_TER flag, which should not be '1' for parts not supporting T2.
    <improvement>[SEEK/DSS] In Si2164B_L2_Channel_Seek_Next and Si2164B_L2_lock_to_carrier
      Added compatibility with DSS, with no impact on AUTO_DVB_S_S2 mode:
       AUTO_DVB_S_S2_DSS is only used if the standard is explicitly DSS.
       This is because otherwise the auto lock is a bit slower, while most platforms don't need to support DSS.

 As from V0.0.5.0:
    <wrapper> Wrapper V2.5.1
    <new_Part>[Si2164_B55] Adding FW download code for X55 parts
    <new_Part>[Si2164_C60] Adding FW download code for X60 parts (which will not be available before mid 2015)
    <firmware> With FW 5_0b12 on X50 parts (full download for ES parts, patch for production parts)
    <firmware> With FW 5_5b1  on X55 parts
    <improvement>[code_checker] adding lines to avoid code checker warnings:
      In Si2164B_L2_Channel_Seek_Next: setting flags to 0 by default (overwritten later on in the function)
    <improvement>[Code_size] Using the textBuffer in Si2164B_L1_Context when filling text strings
      In Si2164B_L2_SW_Init (buffer init)
      In Si2164B_L2_switch_to_standard and Si2164B_L2_Test
    <improvement>[blindscan/debug] In Si2164B_plot: (only when ALLOW_Si2164B_BLINDSCAN_DEBUG is declared)
      Spectrum traces now working (register definitions where those of "pmajor = '4'" parts)
      Compatibility with Si2164 parts (with pmajor = '4').
      Removing unused variables.

*/
/* Older changes:

 As from V0.0.4.0:
    <improvement>[TestPipe/chip_detect] Return 83A in default case for chips not burned
    <firmware> Adding firmware for Si2164B ROM1 / NO NVM (full download)
    <new_feature>[TestPipe/demod] In Si2164B_L2_Test for demod adding command chip_detect to return parts and chiprev of the demod
    <firmware> New firmware: FW 5_3b1 (full download for Si2180 parts only, without T2/C2/S2)
    <firmware> New firmware: FW 5_0b10 (full download for ES parts, patch for production parts)
    <correction>[blindscan] In Si2164B_L2_Channel_Seek_Next: in blind_mode, when a scan interrupt is triggered and Si2164B_L1_SCAN_STATUS is 'BUZY' we wait until Si2164B_L1_SCAN_STATUS is not 'BUZY'
    <new_feature>[TestPipe/demod] In Si2164B_L2_Test for demod with command part_info adding subcommands part and chiprev
    <new_feature>[TestPipe/demod] In Si2164B_L2_Test for demod adding command isdbt-t with subcommand layer_info
    <wrapper> Wrapper V2.5.0
    <firmware> With FW 5_6b1 (full download for Si2180 parts only, without T2/C2/S2)
    <firmware> With FW 5_0b8 (full download for ES parts, patch for production parts)
    <improvement>[code_checker] adding lines to avoid code checker warnings:
      In Si2164B_L2_lock_to_carrier:     setting default values for min_lock_time_ms and min_lock_time_ms (overwritten later on in the function)
      In Si2164B_L2_Channel_Seek_Init:   returning ERROR_Si2164B_ERR in case dd_mode.modulation doesn't match any valid standard (this is not possible by design)
      In Si2164B_L2_Channel_Seek_Init:   setting front_end->searchStartTime before leaving the function   (overwritten later on inside Si2164B_L2_Channel_Seek_Next)
      In Si2164B_L2_Set_Invert_Spectrum: setting inversion to 0 by default (overwritten later on in the function)
    <correction>[lock/MPLP] In Si2164B_L2_lock_to_carrier: using plp_id = plp_id to avoid compiler warning when not used while keeping plp_id value.
      (regression introduced in V0.0.3.0 with 'plp_id = 0;')
    <improvement>[TestPipe/No SiTRACES] In Si2164B_L2_Read_L1_Post_Data: Incrementing word_index outside the SiTRACE, to make sure it will increase even when SiTRACES are not used.

 As from V0.0.3.0:
    <wrapper> Wrapper V2.4.7
    <firmware> With FW 5_0b7 (full download for ES parts, patch for production parts)
    <correction/TESTPIPE> In Si2164B_L2_Read_L1_Misc_Data: storing djb_alarm_comm in the proper field

 As from V0.0.2.0:
    <firmware> With FW 5_0b6 (full download for ES parts, patch for production parts)
    <wrapper> Wrapper V2.4.6
    <new_feature>[SiLOGS] In Si2164B_PowerUpWithPatch: Adding new lines for logging the
      build options and some important lines. Requires updating Si_I2C to V3.4.8, otherwise defining SiLOGS as SiTRACE.
    <correction>[LOAD_FW] In Si2164B_PowerUpWithPatch: Correcting part_info.pminor check
      (incorrectly compared to Si2164B_PATCH16_5_0b4_PMAJOR)

 As from V0.0.1.0:
    <wrapper> Wrapper V2.4.5
    <TER_Tuner_Wrapper> TER Tuner API V0.4.0
    <firmware> With FW 5_0b4 (full download for ES parts, patch for production parts)
    <new_feature>[TER_Tuner/Config] In Si2164B_L2_switch_to_standard:
     Calling SiLabs_TER_Tuner_DTV_OUT_TYPE and SiLabs_TER_Tuner_DTV_AGC_SOURCE instead of TER_TUNER_AGC_EXTERNAL
     NB: To take benefit of this modification, update your TER Tuner wrapper to V0.4.1 or above, to get access to
      the SiLabs_TER_Tuner_DTV_OUT_TYPE and SiLabs_TER_Tuner_DTV_AGC_SOURCE functions.
     NB: No change required for existing applications, since this is mostly useful to use LIF_IF1 with SiLabs TER tuners,
      when compared with the previous versions which by default use LIF_IF2.
    <improvement>[SAT blindscan] In Si2164B_plot: Adding additional spectrum debug information. SAT blindscan now ready to be
        provided to FW team for debug in their simulation environment.

 As from V0.0.0.4:
    <wrapper> Wrapper V2.4.2
    <firmware> With FW 4_Ab4
     Now prepared to use the 16 bytes download FWs (requires Si2164_B50_COMPATIBLE)
    <TER_Tuner_API>[V0.3.9] Using TER-Tuner API V0.3.9, to benefit from the 1.7 MHz filtering feature (not available with all TER tuners).
    <improvement>[TER_BW/1.7MHz] In Si2164B_L2_Tune: now using SILABS_BW enum as defined in SiLABS_TER_TUNER API V0.3.9, to
      use the 1.7 MHz filtering feature in SiLabs TER tuners whenever possible.

 As from V0.0.0.3:
    <wrapper> Wrapper V2.4.1
    <improvement> [Src_code_GUIs] In Si2164B_L2_Test: more complete testpipe 'demod help'
    <correction>[Tuner_i2c] In Si2164B_L2_Tune: Moving 'UNICABLE_COMPATIBLE' line around the closing
      bracket after disabling the SAT tuner i2c.
      The previous version didn't disable the tuner i2c with the following compilation flags:
      UNICABLE_COMPATIBLE      NOT defined
      INDIRECT_I2C_CONNECTION  defined
    <correction> [SILENT/SLEEP/ANALOG] In Si2164B_L2_switch_to_standard: Adding dtv_demod_sleeping flag to
        more easily handle the 'sleep' mode, which can occur upon a clock source change in DTV, or when
        going to 'ANALOG' or 'ATV'. The WAKEUP sequence is required in the first case, not in the second case.
    <improvement> [Unicable/Multi-Treading/Multiple frontends] In Si2164B_L2_TER_FEF_SETUP: removing I2C pass-through enable/disable.
    This is only called from switch_to_standard, and the i2c pass-through is already enabled when calling this function.
    The change removes nested i2c pass-through enable/disable calls.
    These had generally no consequences, except for duals when several tuners use the same i2c address.
    <improvement> [BLINDSCAN/DEBUG/SPECTRUM] In Si2164B_L2_Channel_Seek_Init:
        front_end->demod->prop->scan_sat_config.scan_debug = 0x03; (the previous value of 0x07 doesn't work anymore.

 As from V0.0.0.1:
 <correction>[Tuner_i2c] In Si2164B_L2_Tune: Moving 'ifdef    UNICABLE_COMPATIBLE' line around the closing
  bracket after disabling the SAT tuner i2c.
  The previous version didn't disable the tuner i2c with the following compilation flags:
   UNICABLE_COMPATIBLE NOT defined
  INDIRECT_I2C_CONNECTION  defined

 As from V0.0.0.0:
  Initial version (based on Si2164 code V0.3.4)
****************************************************************************************/

/* Si2164B API Specific Includes */
/* Before including the headers, define SiLevel and SiTAG */
//#define   SiLEVEL          2

#include "Si2164B_L2_API.h"               /* Include file for this code */

/* Re-definition of SiTRACE for L1_Si2164B_Context */
#ifdef    SiTRACES
  #undef  SiTRACE
  #define SiTRACE(...)        SiTraceFunction(__FILE__, __LINE__, __FUNCTION__ ,__VA_ARGS__)
#endif /* SiTRACES */

//#define   Si2164_C60_COMPATIBLE
//#ifdef    Si2164_C60_COMPATIBLE
 #include "Si2164C_ROM2_Patch16_6_0b7.h"     /* firmware compatible with Si2164C     (ROM2/NVM 6_0b1)                                       */
//#endif    Si2164_C60_COMPATIBLE

//#ifdef    Si2164_C5A_COMPATIBLE
 #include "Si2164C_ROM2_Firmware16_5_Bb4.h"     /* firmware compatible with Si2164B     (ROM2/NVM 5_Bb0)                                       */
//#endif    Si2164_C5A_COMPATIBLE 

//#ifdef    Si2164_B55_COMPATIBLE
 #include "Si2164B_ROM1_Patch16_5_6b10.h"     /* firmware compatible with Si2164B     (ROM1/NVM 5_5b1)                                       */
//#endif   Si2164_B55_COMPATIBLE 
//#ifdef    Si2164B_A50_COMPATIBLE
#include "Si2164B_ROM0_Patch16_5_1b8.h"     /* firmware compatible with Si2164B     (ROM0/NVM 5_0b3)*/
/* #include "Si2164B_ROM0_SPI_5_0b6.h" */    /* SPI mode compatible with Si2164B     (ROM0/NVM 5_0b3) */
//#endif  Si2164B_A50_COMPATIBLE 

//#ifdef    Si2164B_ES_COMPATIBLE
 #include "Si2164B_ROM0_Firmware16_5_1b8.h"  /* firmware compatible with Si2164B     (ROM0/no NVM)    */
/* #include "Si2164B_ROM0_SPI_3_Ab14.h"    */    /* SPI      mode patch compatible with Si2164B-C, Si2169-C and Si268-C (smaller patch with no DVB-C2) */
//#endif   Si2164B_ES_COMPATIBLE 

/* Allowing compatibility with previous parts (also reflected in Si2164B_PowerUpWithPatch) */
#ifdef    Si2164_A40_COMPATIBLE
  /* FW with    DVB-C2 (4_4b1 is the minimal patch for Si2164-A40/Si2162-A40/Si2160-A40 NIM production testing) */
 #include "Si2164_ROM1_Patch16_4_4b16_for_83.h"       /* 16 bytes mode patch compatible with Si2164-A40 (With DVB-C2) */
/* #include "Si2164_ROM1_SPI_svn_1758_16_Bytes_as_4_4b10.h"*/ /* SPI      mode patch compatible with Si2164-A40 (With DVB-C2) */
 /* FW without DVB-C2 (4_0b3 is the minimal patch for Si2169B-A40/Si2169B-40 NIM production testing) */
 #include "Si2164_ROM1_Patch16_4_0b18_for_83.h"     /* 16 bytes mode patch compatible with Si2164-A40, Si2169-B40 and Si268-B40 (smaller patch with no DVB-C2) */
/*#include "Si2164_ROM1_SPI_4_0b13.h"*/      /* SPI      mode patch compatible with Si2164-A40, Si2169-B40 and Si268-B40 (smaller patch with no DVB-C2) */
#endif /* Si2164_A40_COMPATIBLE */
#ifdef    Si2169_30_COMPATIBLE
/* #include "Si2169_30_ROM3_Patch_3_0b18.h" */         /* patch    compatible with Si2169-30 ROM 3 */
#endif /* Si2169_30_COMPATIBLE */
#ifdef    Si2167B_20_COMPATIBLE
 #include "Si2167B_ROM0_Patch_2_0b26.h"         /* firmware compatible with Si2167B_20 */
#endif /* Si2167B_20_COMPATIBLE */
/* End of   compatibility with previous parts */

#define Si2164B_BYTES_PER_LINE 8
/*#define   ALLOW_Si2164B_BLINDSCAN_DEBUG*/
#ifdef    ALLOW_Si2164B_BLINDSCAN_DEBUG
 #ifdef    SiTRACES
  #define Si2164B_FORCED_TRACE(...)             SiTraceConfiguration("traces resume"); SiTraceFunction(0,"plot ",__FILE__, __LINE__, __func__,__VA_ARGS__); SiTraceConfiguration("traces suspend");
 #else  /* SiTRACES */
  #define Si2164B_FORCED_TRACE                  safe_printf
 #endif /* SiTRACES */
signed int  inter_carrier_space;
#endif /* ALLOW_Si2164B_BLINDSCAN_DEBUG */
/************************************************************************************************************************
  NAME: Si2164B_Configure
  DESCRIPTION: Setup TER and SAT AGCs, Common Properties startup
  Parameter:  Pointer to Si2164B Context
  Returns:    I2C transaction error code, NO_Si2164B_ERROR if successful
************************************************************************************************************************/
signed int Si2164B_Configure           (L1_Si2164B_Context *api)
{
    signed int return_code;
    return_code = NO_Si2164B_ERROR;

    SiTRACE("media %d\n",api->media);

    /* AGC settings when not used */
    if        ( api->rsp->get_rev.mcm_die == Si2164B_GET_REV_RESPONSE_MCM_DIE_DIE_A) {
      api->cmd->dd_mp_defaults.mp_a_mode   = Si2164B_DD_MP_DEFAULTS_CMD_MP_A_MODE_DISABLE;
      api->cmd->dd_mp_defaults.mp_b_mode   = Si2164B_DD_MP_DEFAULTS_CMD_MP_B_MODE_NO_CHANGE;
      api->cmd->dd_mp_defaults.mp_c_mode   = Si2164B_DD_MP_DEFAULTS_CMD_MP_C_MODE_DISABLE;
      api->cmd->dd_mp_defaults.mp_d_mode   = Si2164B_DD_MP_DEFAULTS_CMD_MP_D_MODE_NO_CHANGE;
    } else if ( api->rsp->get_rev.mcm_die == Si2164B_GET_REV_RESPONSE_MCM_DIE_DIE_B) {
      api->cmd->dd_mp_defaults.mp_a_mode   = Si2164B_DD_MP_DEFAULTS_CMD_MP_A_MODE_NO_CHANGE;
      api->cmd->dd_mp_defaults.mp_b_mode   = Si2164B_DD_MP_DEFAULTS_CMD_MP_B_MODE_DISABLE;
      api->cmd->dd_mp_defaults.mp_c_mode   = Si2164B_DD_MP_DEFAULTS_CMD_MP_C_MODE_NO_CHANGE;
      api->cmd->dd_mp_defaults.mp_d_mode   = Si2164B_DD_MP_DEFAULTS_CMD_MP_D_MODE_DISABLE;
    } else {
      api->cmd->dd_mp_defaults.mp_a_mode   = Si2164B_DD_MP_DEFAULTS_CMD_MP_A_MODE_DISABLE;
      api->cmd->dd_mp_defaults.mp_b_mode   = Si2164B_DD_MP_DEFAULTS_CMD_MP_B_MODE_DISABLE;
      api->cmd->dd_mp_defaults.mp_c_mode   = Si2164B_DD_MP_DEFAULTS_CMD_MP_C_MODE_DISABLE;
      api->cmd->dd_mp_defaults.mp_d_mode   = Si2164B_DD_MP_DEFAULTS_CMD_MP_D_MODE_DISABLE;
    }
    /*  For DVB_T2, if the TER tuner has a FEF freeze input pin, drive this pin to 0 or 1 when NOT in T2 */
    /* if FEF is active high, set the pin to 0 when NOT in T2 */
    /* if FEF is active low,  set the pin to 1 when NOT in T2 */
    if (api->fef_mode == Si2164B_FEF_MODE_FREEZE_PIN) {
      switch (api->fef_pin) {
        case Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_MP_A: {
          api->cmd->dvbt2_fef.fef_tuner_flag      = Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_MP_A;
          if (api->fef_level == 1) { api->cmd->dd_mp_defaults.mp_a_mode = Si2164B_DD_MP_DEFAULTS_CMD_MP_A_MODE_DRIVE_0; }
          else                     { api->cmd->dd_mp_defaults.mp_a_mode = Si2164B_DD_MP_DEFAULTS_CMD_MP_A_MODE_DRIVE_1; }
          break;
        }
        case Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_MP_B: {
          api->cmd->dvbt2_fef.fef_tuner_flag      = Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_MP_B;
          if (api->fef_level == 1) { api->cmd->dd_mp_defaults.mp_b_mode = Si2164B_DD_MP_DEFAULTS_CMD_MP_B_MODE_DRIVE_0; }
          else                     { api->cmd->dd_mp_defaults.mp_b_mode = Si2164B_DD_MP_DEFAULTS_CMD_MP_B_MODE_DRIVE_1; }
          break;
        }
        case Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_MP_C: {
          api->cmd->dvbt2_fef.fef_tuner_flag      = Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_MP_C;
          if (api->fef_level == 1) { api->cmd->dd_mp_defaults.mp_c_mode = Si2164B_DD_MP_DEFAULTS_CMD_MP_C_MODE_DRIVE_0; }
          else                     { api->cmd->dd_mp_defaults.mp_c_mode = Si2164B_DD_MP_DEFAULTS_CMD_MP_C_MODE_DRIVE_1; }
          break;
        }
        case Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_MP_D: {
          api->cmd->dvbt2_fef.fef_tuner_flag      = Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_MP_D;
          if (api->fef_level == 1) { api->cmd->dd_mp_defaults.mp_d_mode = Si2164B_DD_MP_DEFAULTS_CMD_MP_D_MODE_DRIVE_0; }
          else                     { api->cmd->dd_mp_defaults.mp_d_mode = Si2164B_DD_MP_DEFAULTS_CMD_MP_D_MODE_DRIVE_1; }
          break;
        }
        default: break;
      }
    }
    Si2164B_L1_SendCommand2(api, Si2164B_DVBT2_FEF_CMD_CODE);
    Si2164B_L1_SendCommand2(api, Si2164B_DD_MP_DEFAULTS_CMD_CODE);

    if (api->media == Si2164B_TERRESTRIAL) {
      /* TER AGC pins and inversion are previously selected using Si2164B_L2_TER_AGC */
      api->cmd->dd_ext_agc_ter.agc_1_kloop = Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_KLOOP_MIN;
      api->cmd->dd_ext_agc_ter.agc_1_min   = Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MIN_MIN;

      api->cmd->dd_ext_agc_ter.agc_2_kloop = 18;
      api->cmd->dd_ext_agc_ter.agc_2_min   = Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MIN_MIN;
      Si2164B_L1_SendCommand2(api, Si2164B_DD_EXT_AGC_TER_CMD_CODE);
    }

    if (api->media == Si2164B_SATELLITE  ) {
      /* SAT AGC pins and inversion are previously selected using Si2164B_L2_SAT_AGC */
      api->cmd->dd_ext_agc_sat.agc_1_kloop = 18;
      api->cmd->dd_ext_agc_sat.agc_1_min   = Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MIN_MIN;

      api->cmd->dd_ext_agc_sat.agc_2_kloop = Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_KLOOP_MIN;
      api->cmd->dd_ext_agc_sat.agc_2_min   = Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MIN_MIN;

      Si2164B_L1_SendCommand2(api, Si2164B_DD_EXT_AGC_SAT_CMD_CODE);
    }

    /* LEDS MANAGEMENT */
    /* set hardware lock on LED */
    if        ( api->rsp->get_rev.mcm_die == Si2164B_GET_REV_RESPONSE_MCM_DIE_DIE_A) {
      api->cmd->config_pins.gpio0_mode     = Si2164B_CONFIG_PINS_CMD_GPIO0_MODE_NO_CHANGE;
      api->cmd->config_pins.gpio0_read     = Si2164B_CONFIG_PINS_CMD_GPIO0_READ_DO_NOT_READ;
      api->cmd->config_pins.gpio1_mode     = Si2164B_CONFIG_PINS_CMD_GPIO1_MODE_HW_LOCK;
      api->cmd->config_pins.gpio1_read     = Si2164B_CONFIG_PINS_CMD_GPIO1_READ_DO_NOT_READ;
    } else if ( api->rsp->get_rev.mcm_die == Si2164B_GET_REV_RESPONSE_MCM_DIE_DIE_B) {
      api->cmd->config_pins.gpio0_mode     = Si2164B_CONFIG_PINS_CMD_GPIO0_MODE_HW_LOCK;
      api->cmd->config_pins.gpio0_read     = Si2164B_CONFIG_PINS_CMD_GPIO0_READ_DO_NOT_READ;
      api->cmd->config_pins.gpio1_mode     = Si2164B_CONFIG_PINS_CMD_GPIO1_MODE_NO_CHANGE;
      api->cmd->config_pins.gpio1_read     = Si2164B_CONFIG_PINS_CMD_GPIO1_READ_DO_NOT_READ;
    } else {
      api->cmd->config_pins.gpio0_mode     = Si2164B_CONFIG_PINS_CMD_GPIO0_MODE_HW_LOCK;
      api->cmd->config_pins.gpio0_read     = Si2164B_CONFIG_PINS_CMD_GPIO0_READ_DO_NOT_READ;
      api->cmd->config_pins.gpio1_mode     = Si2164B_CONFIG_PINS_CMD_GPIO1_MODE_TS_ERR;
      api->cmd->config_pins.gpio1_read     = Si2164B_CONFIG_PINS_CMD_GPIO1_READ_DO_NOT_READ;
    }
    Si2164B_L1_SendCommand2(api, Si2164B_CONFIG_PINS_CMD_CODE);

    /* Edit the procedure below if you have any properties settings different from the standard defaults */
    Si2164B_storeUserProperties     (api->prop);
    /* Download properties different from 'default' */
    Si2164B_downloadAllProperties(api);

    /* debug unicable scan !!! 0 better ; SR/8 default ; > worst */
    /* Si2164B_L1_DD_SET_REG  (api, 7 , 71, 10, 0); */

/* Re-lock improvement per T2 PLP Number change with property 0x1013*/
/* automatic PLP is enabled when property bit[4] is set to 1 for FW 50b9, bit[5] is set to 1 for FW 50b10 onwards , no effect for other FWs*/
/* automatic PLP is disabled when property bit[4] is set to 0 1 for FW 50b9, bit[5] is set to 0 for FW 50b10 onwards , no effect for other FWs */

/* in MPLP and in PLP_AUTO, the PLP selected depends on the property 0x1013*/
/* mode1: most robust is selected when property bit[2:1] are set to 0 */
/* mode2: first PLP and then most robust is selected when property bit[2:1] are set to 1 */
/* mode3: first PLP is selected when property bit[2:1] are set to 2 */
if ( ( api->rsp->get_rev.cmpmajor == 53) && ( api->rsp->get_rev.cmpminor == 48) && ( api->rsp->get_rev.cmpbuild == 9) )
{
  Si2164B_L1_SET_PROPERTY(api, 0, 0x1013,0x12);
} else {Si2164B_L1_SET_PROPERTY(api, 0, 0x1013,0x22);}

/* Change the terrestrial system clock to 47 MHz only valid for ROM0 FW5.1b1 */
    if  ( ( api->rsp->get_rev.cmpmajor == 53) && ( api->rsp->get_rev.cmpminor == 49) && ( api->rsp->get_rev.cmpbuild >= 1) )
    {
        Si2164B_L1_SET_PROPERTY(api, 0, 0x1013,0x62);
    }
/* Change the terrestrial system clock to 47 MHz only valid for ROM1 FW5.6b2 */
    if  ( ( api->rsp->get_rev.cmpmajor == 53) && ( api->rsp->get_rev.cmpminor == 54) && ( api->rsp->get_rev.cmpbuild >= 2) )
    {
        Si2164B_L1_SET_PROPERTY(api, 0, 0x1013,0x62);
    }

#ifdef    USB_Capability
    if        ( api->rsp->get_rev.mcm_die == Si2164B_GET_REV_RESPONSE_MCM_DIE_SINGLE) {
      /* Setting GPIF clock to not_inverted to allow TS over USB transfer */
      Si2164B_L1_DD_SET_REG  (api, 0 , 35, 1, 0);
    }
#endif /* USB_Capability */
#ifdef    Si2167B_20_COMPATIBLE
    //fw 2.0b23 is able to behave as 2.0b18 for 32QAM lock time with this code.20140225
    /* This needs to be done for Si21F67B (part 67, ROM 0) with the following FW: */
    /*  FW 2_0b23 and above                                     */
    /*  FW 2_2b2  and above                                     */
    if (  (api->rsp->part_info.part     == 67 )
        & (api->rsp->part_info.romid    ==  0 ) ) {
      if ( (
            (api->rsp->get_rev.cmpmajor == '2')
          & (api->rsp->get_rev.cmpminor == '0')
          & (api->rsp->get_rev.cmpbuild >= 23)
         ) | (
            (api->rsp->get_rev.cmpmajor == '2')
          & (api->rsp->get_rev.cmpminor == '2')
          & (api->rsp->get_rev.cmpbuild >=  2)
         ) ) {
		    api->cmd->dd_set_reg.reg_code_lsb = 132;
		    api->cmd->dd_set_reg.reg_code_mid = 50;
		    api->cmd->dd_set_reg.reg_code_msb = 10;
		    api->cmd->dd_set_reg.value        = 1;
		     /* 0x8e for DVB-C with Si2167B 2_0b23: fw 2.0b23 is able to behave as 2.0b18 for 32QAM lock time with this code. */
        SiTRACE("[SiTRACE]Si2164B_Configure: DD_SET_REG  (api, 132, 50, 10, 1) for 32QAM lock time with Si2167B\n");
        Si2164B_L1_SendCommand2 (api, Si2164B_DD_SET_REG_CMD_CODE);
      }
    }
#endif /* Si2167B_20_COMPATIBLE */

      /* register setting for carrier phase modulation immunity */
      if  ( ((api->rsp->part_info.part == 60 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 62 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 64 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 68 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 69 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         )  {
		    api->cmd->dd_set_reg.reg_code_lsb = 4;
		    api->cmd->dd_set_reg.reg_code_mid = 218;
		    api->cmd->dd_set_reg.reg_code_msb = 8;
		    api->cmd->dd_set_reg.value        = 6;
        SiTRACE("[SiTRACE]Si2164B_Configure: DD_SET_REG  (api, 4, 218, 8, 6) for carrier phase modulation immunity with Si2164B\n");
        Si2164B_L1_SendCommand2 (api, Si2164B_DD_SET_REG_CMD_CODE);
      }

    return return_code;
}
/************************************************************************************************************************
  NAME: Si2164B_STANDBY
  DESCRIPTION:
  Parameter:  Pointer to Si2164B Context
  Returns:    I2C transaction error code, NO_Si2164B_ERROR if successful
************************************************************************************************************************/
signed int Si2164B_STANDBY             (L1_Si2164B_Context *api)
{
    return Si2164B_L1_POWER_DOWN (api);
}
/************************************************************************************************************************
  NAME: Si2164B_WAKEUP
  DESCRIPTION:
  Parameter:  Pointer to Si2164B Context
  Returns:    I2C transaction error code, NO_Si2164B_ERROR if successful
************************************************************************************************************************/
signed int Si2164B_WAKEUP              (L1_Si2164B_Context *api)
{
    signed int return_code;
    signed int media;

    return_code = NO_Si2164B_ERROR;
    media       = Si2164B_Media(api, api->standard);
    SiTRACE ("Si2164B_WAKEUP: media %d\n", media);

    /* Clock source selection */
    switch (media) {
      default                                  :
      case Si2164B_TERRESTRIAL : {
        api->cmd->start_clk.clk_mode = api->tuner_ter_clock_input;
        break;
      }
      case Si2164B_SATELLITE   : {
        api->cmd->start_clk.clk_mode = api->tuner_sat_clock_input;
        break;
      }
    }
    if (api->cmd->start_clk.clk_mode == Si2164B_START_CLK_CMD_CLK_MODE_XTAL) {
      api->cmd->start_clk.tune_cap = Si2164B_START_CLK_CMD_TUNE_CAP_15P6;
    } else {
      api->cmd->start_clk.tune_cap = Si2164B_START_CLK_CMD_TUNE_CAP_EXT_CLK;
    }
    Si2164B_L1_START_CLK (api,
                            Si2164B_START_CLK_CMD_SUBCODE_CODE,
                            Si2164B_START_CLK_CMD_RESERVED1_RESERVED,
                            api->cmd->start_clk.tune_cap,
                            Si2164B_START_CLK_CMD_RESERVED2_RESERVED,
                            api->cmd->start_clk.clk_mode,
                            Si2164B_START_CLK_CMD_RESERVED3_RESERVED,
                            Si2164B_START_CLK_CMD_RESERVED4_RESERVED,
                            Si2164B_START_CLK_CMD_START_CLK_START_CLK);
    /* Reference frequency selection */
    switch (media) {
      default                 :
      case Si2164B_TERRESTRIAL : {
        if (api->tuner_ter_clock_freq == 16) {
          SiTRACE("Si2164B_POWER_UP_CMD_CLOCK_FREQ_CLK_16MHZ\n");
          api->cmd->power_up.clock_freq = Si2164B_POWER_UP_CMD_CLOCK_FREQ_CLK_16MHZ;
        } else if (api->tuner_ter_clock_freq == 24) {
          SiTRACE("Si2164B_POWER_UP_CMD_CLOCK_FREQ_CLK_24MHZ\n");
          api->cmd->power_up.clock_freq = Si2164B_POWER_UP_CMD_CLOCK_FREQ_CLK_24MHZ;
        } else {
          SiTRACE("Si2164B_POWER_UP_CMD_CLOCK_FREQ_CLK_27MHZ\n");
          api->cmd->power_up.clock_freq = Si2164B_POWER_UP_CMD_CLOCK_FREQ_CLK_27MHZ;
        }
        break;
      }
      case Si2164B_SATELLITE   : {
        if (api->tuner_sat_clock_freq == 16) {
          SiTRACE("Si2164B_POWER_UP_CMD_CLOCK_FREQ_CLK_16MHZ\n");
          api->cmd->power_up.clock_freq = Si2164B_POWER_UP_CMD_CLOCK_FREQ_CLK_16MHZ;
        } else if (api->tuner_sat_clock_freq == 24) {
          SiTRACE("Si2164B_POWER_UP_CMD_CLOCK_FREQ_CLK_24MHZ\n");
          api->cmd->power_up.clock_freq = Si2164B_POWER_UP_CMD_CLOCK_FREQ_CLK_24MHZ;
        } else {
          SiTRACE("Si2164B_POWER_UP_CMD_CLOCK_FREQ_CLK_27MHZ\n");
          api->cmd->power_up.clock_freq = Si2164B_POWER_UP_CMD_CLOCK_FREQ_CLK_27MHZ;
        }
        break;
      }
    }

    return_code = Si2164B_L1_POWER_UP (api,
                            Si2164B_POWER_UP_CMD_SUBCODE_CODE,
                            api->cmd->power_up.reset,
                            Si2164B_POWER_UP_CMD_RESERVED2_RESERVED,
                            Si2164B_POWER_UP_CMD_RESERVED4_RESERVED,
                            Si2164B_POWER_UP_CMD_RESERVED1_RESERVED,
                            Si2164B_POWER_UP_CMD_ADDR_MODE_CURRENT,
                            Si2164B_POWER_UP_CMD_RESERVED5_RESERVED,
                            api->cmd->power_up.func,
                            api->cmd->power_up.clock_freq,
                            Si2164B_POWER_UP_CMD_CTSIEN_DISABLE,
                            Si2164B_POWER_UP_CMD_WAKE_UP_WAKE_UP);

         if (api->cmd->start_clk.clk_mode == Si2164B_START_CLK_CMD_CLK_MODE_CLK_CLKIO  ) { SiTRACE ("Si2164B_START_CLK_CMD_CLK_MODE_CLK_CLKIO\n"  );}
    else if (api->cmd->start_clk.clk_mode == Si2164B_START_CLK_CMD_CLK_MODE_CLK_XTAL_IN) { SiTRACE ("Si2164B_START_CLK_CMD_CLK_MODE_CLK_XTAL_IN\n");}
    else if (api->cmd->start_clk.clk_mode == Si2164B_START_CLK_CMD_CLK_MODE_XTAL       ) { SiTRACE ("Si2164B_START_CLK_CMD_CLK_MODE_XTAL\n"       );}

         if (api->cmd->power_up.reset == Si2164B_POWER_UP_CMD_RESET_RESET  ) { SiTRACE ("Si2164B_POWER_UP_CMD_RESET_RESET\n"  );}
    else if (api->cmd->power_up.reset == Si2164B_POWER_UP_CMD_RESET_RESUME ) { SiTRACE ("Si2164B_POWER_UP_CMD_RESET_RESUME\n");}

    if (return_code != NO_Si2164B_ERROR ) {
      SiTRACE("Si2164B_WAKEUP: POWER_UP ERROR!\n");
      SiERROR("Si2164B_WAKEUP: POWER_UP ERROR!\n");
      return return_code;
    }
    /* After a successful POWER_UP, set values for 'resume' only */
    api->cmd->power_up.reset = Si2164B_POWER_UP_CMD_RESET_RESUME;

    return NO_Si2164B_ERROR;
}
/************************************************************************************************************************
  NAME: Si2164B_PowerUpWithPatch
  DESCRIPTION: Send Si2164B API PowerUp Command with PowerUp to bootloader,
  Check the Chip rev and part, and ROMID are compared to expected values.
  Load the Firmware Patch then Start the Firmware.
  Programming Guide Reference:    Flowchart A.2 (POWER_UP with patch flowchart)

  Parameter:  pointer to Si2164B Context
  Returns:    Si2164B/I2C transaction error code, NO_Si2164B_ERROR if successful
************************************************************************************************************************/
signed int Si2164B_PowerUpWithPatch    (L1_Si2164B_Context *api)
{
    signed int return_code;
    signed int fw_loaded;
    return_code = NO_Si2164B_ERROR;
    fw_loaded   = 0;

    /* Before patching, set POWER_UP values for 'RESET' and 'BOOTLOADER' */
    api->cmd->power_up.reset = Si2164B_POWER_UP_CMD_RESET_RESET;
    api->cmd->power_up.func  = Si2164B_POWER_UP_CMD_FUNC_BOOTLOADER;

    return_code = Si2164B_WAKEUP(api);

    if (return_code != NO_Si2164B_ERROR) {
      SiERROR("Si2164B_PowerUpWithPatch: WAKEUP error!\n");
        return return_code;
    }

    /* Get the Part Info from the chip.   This command is only valid in Bootloader mode */
    if ((return_code = Si2164B_L1_PART_INFO(api)) != NO_Si2164B_ERROR) {
        SiTRACE ("Si2164B_L1_PART_INFO error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }
    SiTRACE("part    Si21%02d",   api->rsp->part_info.part   );
           if (api->rsp->part_info.chiprev == Si2164B_PART_INFO_RESPONSE_CHIPREV_A) {
    SiTRACE("A\n");
    } else if (api->rsp->part_info.chiprev == Si2164B_PART_INFO_RESPONSE_CHIPREV_B) {
    SiTRACE("B\n");
    } else if (api->rsp->part_info.chiprev == Si2164B_PART_INFO_RESPONSE_CHIPREV_C) {
    SiTRACE("C\n");
    } else if (api->rsp->part_info.chiprev == Si2164B_PART_INFO_RESPONSE_CHIPREV_D) {
    SiTRACE("D\n");
    } else if (api->rsp->part_info.chiprev == Si2164B_PART_INFO_RESPONSE_CHIPREV_E) {
    SiTRACE("E\n");
    } else {
    SiTRACE("\nchiprev %d\n",        api->rsp->part_info.chiprev);
    }
    SiTRACE("romid   %d\n",        api->rsp->part_info.romid  );
    SiTRACE("pmajor  0x%02x\n",    api->rsp->part_info.pmajor );
    SiTRACE("pminor  0x%02x\n",    api->rsp->part_info.pminor );
    SiTRACE("pbuild  %d\n",        api->rsp->part_info.pbuild );
    SiTRACE("chiprev 0x%02x\n",    api->rsp->part_info.chiprev);
    SiTRACE("chiprev %c\n",        api->rsp->part_info.chiprev + 0x40 );
    if ((api->rsp->part_info.pmajor >= 0x30) & (api->rsp->part_info.pminor >= 0x30)) {
    SiTRACE("Full Info       'Si21%02d-%c%c%c ROM%x NVM%c_%cb%d'\n\n\n", api->rsp->part_info.part, api->rsp->part_info.chiprev + 0x40, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.romid, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
    }

    /* Load FW from a table */
    if (api->nbLines) {
      SiTRACE("loading FW from stored table (%d lines))\n", api->nbLines);
      if ( Si2164B_LoadFirmware_16(api, api->fw_table, api->nbLines) != NO_Si2164B_ERROR ) {
        SiERROR ("Error loading stored FW table (16 Bytes)\n");
      } else {
        fw_loaded++; SiERROR ("Loaded FW table\n");
      }
    }

    SiTRACE ("api->spi_download %d\n",api->spi_download);
    /* Check part info values and load the proper firmware */
//#ifdef    Si2164_C60_COMPATIBLE
    SiTRACE ("Build Option: Si2164_C60_COMPATIBLE\n");
    #ifdef    Si2164C_SPI_6_0b1_BYTES
    if ( (api->spi_download) & (!fw_loaded) ) {
      SiTRACE   ("Checking if we can load Si2164C_SPI_6_0b1\n");
      SiTRACE  ("Si2164C_SPI_6_0b1_LINES: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2164C_SPI_6_0b1_PART, Si2164C_SPI_6_0b1_ROM, Si2164C_SPI_6_0b1_PMAJOR, Si2164C_SPI_6_0b1_PMINOR, Si2164C_SPI_6_0b1_PBUILD );
      if ((api->rsp->part_info.romid  == Si2164C_SPI_6_0b1_ROM   )
        &(
            ((api->rsp->part_info.part == 69 ) & ((api->rsp->part_info.chiprev + 0x40 == 'D') )
         || ((api->rsp->part_info.part == 68 ) & ((api->rsp->part_info.chiprev + 0x40 == 'D') )
         || ((api->rsp->part_info.part == 67 ) & ((api->rsp->part_info.chiprev + 0x40 == 'D') )
         || ((api->rsp->part_info.part == 66 ) & ((api->rsp->part_info.chiprev + 0x40 == 'D') )
         || ((api->rsp->part_info.part == 64 ) & ((api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 62 ) & ((api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 60 ) & ((api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ( api->rsp->part_info.part == 0  )
        )
          ) {
        SiTRACE("Updating FW via SPI for 'Si21%2d NVM%c_%cb%d'\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
        #ifdef    Si2164C_SPI_6_0b1_SPLITS
        if ((return_code = Si2164B_LoadFirmwareSPI_Split(api, Si2164C_SPI_6_0b1, Si2164C_SPI_6_0b1_BYTES, Si2164C_SPI_6_0b1_KEY, Si2164C_SPI_6_0b1_NUM, Si2164C_SPI_6_0b1_SPLITS, Si2164C_SPI_6_0b1_SPLIT_LIST )) != NO_Si2164B_ERROR) {
          SiTRACE ("Si2164B_LoadFirmwareSPI_Split  error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
          return return_code;
        }
        #else  /* Si2164C_SPI_6_0b1_SPLITS */
        if ((return_code = Si2164B_LoadFirmwareSPI(api, Si2164C_SPI_6_0b1, Si2164C_SPI_6_0b1_BYTES, Si2164C_SPI_6_0b1_KEY, Si2164C_SPI_6_0b1_NUM )) != NO_Si2164B_ERROR) {
          SiTRACE ("Si2164C_LoadFirmwareSPI  error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
          return return_code;
        }
        #endif /* Si2164B_SPI_6_0b1_SPLITS */
        #ifdef    Si2164C_SPI_6_0b1_INFOS
          SiERROR(Si2164C_SPI_6_0b1_INFOS);
        #endif /* Si2164C_SPI_6_0b1_INFOS */
        fw_loaded++;
      }
    }
    #endif /* Si2164C_SPI_6_0b1_BYTES */
    #ifdef    Si2164C_PATCH16_6_0b7_LINES
    if (!fw_loaded) {
      SiTRACE   ("Checking if we can load Si2164C_PATCH16_6_0b7\n");
      SiTRACE  ("Si2164C_PATCH16_6_0b7: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2164C_PATCH16_6_0b7_PART, Si2164C_PATCH16_6_0b7_ROM, Si2164C_PATCH16_6_0b7_PMAJOR, Si2164C_PATCH16_6_0b7_PMINOR, Si2164C_PATCH16_6_0b7_PBUILD );
      if  (((api->rsp->part_info.romid  == Si2164C_PATCH16_6_0b7_ROM   )
        & (
            ((api->rsp->part_info.part == 60 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 62 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 64 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 66 ) & (api->rsp->part_info.chiprev + 0x40 == 'D') )
         || ((api->rsp->part_info.part == 67 ) & (api->rsp->part_info.chiprev + 0x40 == 'D') )
         || ((api->rsp->part_info.part == 68 ) & (api->rsp->part_info.chiprev + 0x40 == 'D') )
         || ((api->rsp->part_info.part == 69 ) & (api->rsp->part_info.chiprev + 0x40 == 'D') )
         || ((api->rsp->part_info.part == 0  ) & (1) )
        ) & (
           (api->rsp->part_info.pmajor == Si2164C_PATCH16_6_0b7_PMAJOR)
         & (api->rsp->part_info.pminor == Si2164C_PATCH16_6_0b7_PMINOR)
         & (api->rsp->part_info.pbuild == Si2164C_PATCH16_6_0b7_PBUILD)
        ) )
        ) {
        SiTRACE("Updating FW for 'Si21%2d NVM%c_%cb%d' (patch)\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
        #ifdef    Si2164C_PATCH16_6_0b7_INFOS
          SiERROR(Si2164C_PATCH16_6_0b7_INFOS);
        #endif /* Si2164C_PATCH16_6_0b7_INFOS */
        if ((return_code = Si2164B_LoadFirmware_16(api, Si2164C_PATCH16_6_0b7, Si2164C_PATCH16_6_0b7_LINES)) != NO_Si2164B_ERROR) {
           SiTRACE ("Si2164C_LoadFirmware_16 Si2164C_PATCH16_6_0b7 error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
           return return_code;
         }
        fw_loaded++;
      }
    }
    #endif /* Si2164C_PATCH16_6_0b7 _LINES */
//#endif /* Si2164_C60_COMPATIBLE */
//#ifdef    Si2164_C5A_COMPATIBLE
    SiTRACE ("Build Option: Si2164_C5A_COMPATIBLE\n");
    #ifdef    Si2164B_SPI_5_Bb2_BYTES
    if ( (api->spi_download) & (!fw_loaded) ) {
      SiLOGS   ("Checking if we can load Si2164B_SPI_5_Bb2\n");
      SiTRACE  ("Si2164B_SPI_5_Bb2_LINES: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2164B_SPI_5_Bb2_PART, Si2164B_SPI_5_Bb2_ROM, Si2164B_SPI_5_Bb2_PMAJOR, Si2164B_SPI_5_Bb2_PMINOR, Si2164B_SPI_5_Bb2_PBUILD );
      if ((api->rsp->part_info.romid  == Si2164B_SPI_5_Bb2_ROM   )
        &(
            ((api->rsp->part_info.part == 69 ) & (api->rsp->part_info.chiprev + 0x40 == 'D') )
         || ((api->rsp->part_info.part == 68 ) & (api->rsp->part_info.chiprev + 0x40 == 'D') )
         || ((api->rsp->part_info.part == 64 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 62 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 60 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ( api->rsp->part_info.part == 0  )
        )
          ) {
        SiTRACE("Updating FW via SPI for 'Si21%2d NVM%c_%cb%d'\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
        #ifdef    Si2164B_SPI_5_Bb2_SPLITS
        if ((return_code = Si2164B_LoadFirmwareSPI_Split(api, Si2164B_SPI_5_Bb2, Si2164B_SPI_5_Bb2_BYTES, Si2164B_SPI_5_Bb2_KEY, Si2164B_SPI_5_Bb2_NUM, Si2164B_SPI_5_Bb2_SPLITS, Si2164B_SPI_5_Bb2_SPLIT_LIST )) != NO_Si2164B_ERROR) {
          SiTRACE ("Si2164B_LoadFirmwareSPI_Split  error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
          return return_code;
        }
        #else  /* Si2164B_SPI_5_Bb2_SPLITS */
        if ((return_code = Si2164B_LoadFirmwareSPI(api, Si2164B_SPI_5_Bb2, Si2164B_SPI_5_Bb2_BYTES, Si2164B_SPI_5_Bb2_KEY, Si2164B_SPI_5_Bb2_NUM )) != NO_Si2164B_ERROR) {
          SiTRACE ("Si2164B_LoadFirmwareSPI  error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
          return return_code;
        }
        #endif /* Si2164B_SPI_5_Bb2_SPLITS */
        #ifdef    Si2164B_SPI_5_Bb2_INFOS
          SiERROR(Si2164B_SPI_5_Bb2_INFOS);
        #endif /* Si2164B_SPI_5_Bb2_INFOS */
        fw_loaded++;
      }
    }
    #endif /* Si2164B_SPI_5_Bb2_BYTES */
    #ifdef    Si2164C_FIRMWARE16_5_Bb4_LINES
    if (!fw_loaded) {
      SiTRACE   ("Checking if we can load Si2164C_FIRMWARE16_5_Bb4\n");
      SiTRACE  ("Si2164C_FIRMWARE16_5_Bb4: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2164C_FIRMWARE16_5_Bb4_PART, Si2164C_FIRMWARE16_5_Bb4_ROM, Si2164C_FIRMWARE16_5_Bb4_PMAJOR, Si2164C_FIRMWARE16_5_Bb4_PMINOR, Si2164C_FIRMWARE16_5_Bb4_PBUILD );
      if  (((api->rsp->part_info.romid  == Si2164C_FIRMWARE16_5_Bb4_ROM   )
        & (
            ((api->rsp->part_info.part == 60 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 62 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 64 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 66 ) & (api->rsp->part_info.chiprev + 0x40 == 'D') )
         || ((api->rsp->part_info.part == 67 ) & (api->rsp->part_info.chiprev + 0x40 == 'D') )
         || ((api->rsp->part_info.part == 68 ) & (api->rsp->part_info.chiprev + 0x40 == 'D') )
         || ((api->rsp->part_info.part == 69 ) & (api->rsp->part_info.chiprev + 0x40 == 'D') )
        ) & (
           (api->rsp->part_info.pmajor == Si2164C_FIRMWARE16_5_Bb4_PMAJOR)
         & (api->rsp->part_info.pminor == Si2164C_FIRMWARE16_5_Bb4_PMINOR)
         & (api->rsp->part_info.pbuild == Si2164C_FIRMWARE16_5_Bb4_PBUILD)
        ) )
        ) {
        SiTRACE("Updating FW for 'Si21%2d NVM%c_%cb%d' (patch)\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
        #ifdef    Si2164C_FIRMWARE16_5_Bb4_INFOS
          SiERROR(Si2164C_FIRMWARE16_5_Bb4_INFOS);
        #endif /* Si2164C_FIRMWARE16_5_Bb4_INFOS */
        if ((return_code = Si2164B_LoadFirmware_16(api, Si2164C_FIRMWARE16_5_Bb4, Si2164C_FIRMWARE16_5_Bb4_LINES)) != NO_Si2164B_ERROR) {
           SiTRACE ("Si2164B_LoadFirmware Si2164C_FIRMWARE16_5_Bb4 error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
           return return_code;
         }
        fw_loaded++;
      }
    }
    #endif /* Si2164C_FIRMWARE16_5_Bb4 _LINES */
//#endif /* Si2164_C5A_COMPATIBLE */
//#ifdef    Si2164_B55_COMPATIBLE
    SiTRACE ("Build Option: Si2164_B55_COMPATIBLE\n");
    #ifdef    Si2164B_SPI_5_5b5_BYTES
    if ( (api->spi_download) & (!fw_loaded) ) {
      SiTRACE  ("Si2164B_SPI_5_5b5_LINES: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2164B_SPI_5_5b5_PART, Si2164B_SPI_5_5b5_ROM, Si2164B_SPI_5_5b5_PMAJOR, Si2164B_SPI_5_5b5_PMINOR, Si2164B_SPI_5_5b5_PBUILD );
      if ((api->rsp->part_info.romid  == Si2164B_SPI_5_5b5_ROM   )
        &(
            ((api->rsp->part_info.part == 69 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 68 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 64 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 62 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 60 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ( api->rsp->part_info.part == 0  )
        )
          ) {
        SiTRACE("Updating FW via SPI for 'Si21%2d NVM%c_%cb%d'\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
        #ifdef    Si2164B_SPI_5_5b5_SPLITS
        if ((return_code = Si2164B_LoadFirmwareSPI_Split(api, Si2164B_SPI_5_5b5, Si2164B_SPI_5_5b5_BYTES, Si2164B_SPI_5_5b5_KEY, Si2164B_SPI_5_5b5_NUM, Si2164B_SPI_5_5b5_SPLITS, Si2164B_SPI_5_5b5_SPLIT_LIST )) != NO_Si2164B_ERROR) {
          SiTRACE ("Si2164B_LoadFirmwareSPI_Split  error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
          return return_code;
        }
        #else  /* Si2164B_SPI_5_5b5_SPLITS */
        if ((return_code = Si2164B_LoadFirmwareSPI(api, Si2164B_SPI_5_5b5, Si2164B_SPI_5_5b5_BYTES, Si2164B_SPI_5_5b5_KEY, Si2164B_SPI_5_5b5_NUM )) != NO_Si2164B_ERROR) {
          SiTRACE ("Si2164B_LoadFirmwareSPI  error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
          return return_code;
        }
        #endif /* Si2164B_SPI_5_5b5_SPLITS */
        #ifdef    Si2164B_SPI_5_5b5_INFOS
          SiERROR(Si2164B_SPI_5_5b5_INFOS);
        #endif /* Si2164B_SPI_5_5b5_INFOS */
        fw_loaded++;
      }
    }
    #endif /* Si2164B_SPI_5_5b5_BYTES */
    #ifdef    Si2164B_PATCH16_5_6b10_LINES
    SiTRACE ("Checking part info for Si2164B_PATCH16_5_6b10\n");
    if (!fw_loaded) {
      SiTRACE  ("Si2164B_PATCH16_5_6b10: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2164B_PATCH16_5_6b10_PART, Si2164B_PATCH16_5_6b10_ROM, Si2164B_PATCH16_5_6b10_PMAJOR, Si2164B_PATCH16_5_6b10_PMINOR, Si2164B_PATCH16_5_6b10_PBUILD );
      if  (((api->rsp->part_info.romid  == Si2164B_PATCH16_5_6b10_ROM   )
        & (
            ((api->rsp->part_info.part == 60 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 62 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 64 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 68 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 69 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
        ) & (
           (api->rsp->part_info.pmajor == Si2164B_PATCH16_5_6b10_PMAJOR)
         & (api->rsp->part_info.pminor == Si2164B_PATCH16_5_6b10_PMINOR)
         & (api->rsp->part_info.pbuild == Si2164B_PATCH16_5_6b10_PBUILD)
        ) )
        ) {
        SiTRACE("Updating FW for 'Si21%2d NVM%c_%cb%d' (patch)\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
        #ifdef    Si2164B_PATCH16_5_6b10_INFOS
          SiTRACE(Si2164B_PATCH16_5_6b10_INFOS);
          SiERROR(Si2164B_PATCH16_5_6b10_INFOS);
        #endif /* Si2164B_PATCH16_5_6b10_INFOS */
        if ((return_code = Si2164B_LoadFirmware_16(api, Si2164B_PATCH16_5_6b10, Si2164B_PATCH16_5_6b10_LINES)) != NO_Si2164B_ERROR) {
           SiTRACE ("Si2164B_LoadFirmware Si2164B_PATCH16_5_6b10 error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
           return return_code;
         }
        fw_loaded++;
      }
    }
    #endif /* Si2164B_PATCH16_5_6b10_LINES */
//#endif /* Si2164_B55_COMPATIBLE */
//#ifdef    Si2164B_A50_COMPATIBLE
    SiTRACE ("Build Option: Si2164_B50_COMPATIBLE\n");
    #ifdef    Si2164B_SPI_5_0b15_BYTES
    if ( (api->spi_download) & (!fw_loaded) ) {
      SiTRACE  ("Si2164B_SPI_5_0b15_BYTES: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2164B_SPI_5_0b15_PART, Si2164B_SPI_5_0b15_ROM, Si2164B_SPI_5_0b15_PMAJOR, Si2164B_SPI_5_0b15_PMINOR, Si2164B_SPI_5_0b15_PBUILD );
      if ((api->rsp->part_info.romid  == Si2164B_SPI_5_0b15_ROM   )
        &(
            ((api->rsp->part_info.part == 69 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 68 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 64 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 62 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 60 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ( api->rsp->part_info.part == 0  )
        )
          ) {
        SiTRACE("Updating FW via SPI for 'Si21%2d NVM%c_%cb%d'\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
        #ifdef    Si2164B_SPI_5_0b15_SPLITS
        if ((return_code = Si2164B_LoadFirmwareSPI_Split(api, Si2164B_SPI_5_0b15, Si2164B_SPI_5_0b15_BYTES, Si2164B_SPI_5_0b15_KEY, Si2164B_SPI_5_0b15_NUM, Si2164B_SPI_5_0b15_SPLITS, Si2164B_SPI_5_0b15_SPLIT_LIST )) != NO_Si2164B_ERROR) {
          SiTRACE ("Si2164B_LoadFirmwareSPI_Split  error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
          return return_code;
        }
        #else  /* Si2164B_SPI_5_0b15_SPLITS */
        if ((return_code = Si2164B_LoadFirmwareSPI(api, Si2164B_SPI_5_0b15, Si2164B_SPI_5_0b15_BYTES, Si2164B_SPI_5_0b15_KEY, Si2164B_SPI_5_0b15_NUM )) != NO_Si2164B_ERROR) {
          SiTRACE ("Si2164B_LoadFirmwareSPI  error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
          return return_code;
        }
        #endif /* Si2164B_SPI_5_0b15_SPLITS */
        #ifdef    Si2164B_SPI_5_0b15_INFOS
          SiERROR(Si2164B_SPI_5_0b15_INFOS);
        #endif /* Si2164B_SPI_5_0b15_INFOS */
        fw_loaded++;
      }
    }
    #endif /* Si2164B_SPI_5_0b15_BYTES */
    #ifdef    Si2164B_PATCH16_5_1b8_LINES
    SiTRACE ("Checking part info for Si2164B_PATCH16_5_1b8\n");
    if (!fw_loaded) {
      SiTRACE  ("Si2164B_PATCH16_5_1b8: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2164B_PATCH16_5_1b8_PART, Si2164B_PATCH16_5_1b8_ROM, Si2164B_PATCH16_5_1b8_PMAJOR, Si2164B_PATCH16_5_1b8_PMINOR, Si2164B_PATCH16_5_1b8_PBUILD );
      if  (((api->rsp->part_info.romid  == Si2164B_PATCH16_5_1b8_ROM   )
        & (
            ((api->rsp->part_info.part == 60 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 62 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 64 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 68 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 69 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
        ) & (
           (api->rsp->part_info.pmajor == Si2164B_PATCH16_5_1b8_PMAJOR)
         & (api->rsp->part_info.pminor == Si2164B_PATCH16_5_1b8_PMINOR)
         & (api->rsp->part_info.pbuild == Si2164B_PATCH16_5_1b8_PBUILD)
        ) )
        ) {
        SiTRACE("Updating FW for 'Si21%2d NVM%c_%cb%d' (full download)\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
        #ifdef    Si2164B_PATCH16_5_1b8_INFOS
          SiTRACE(Si2164B_PATCH16_5_1b8_INFOS);
          SiERROR(Si2164B_PATCH16_5_1b8_INFOS);
        #endif /* Si2164B_PATCH16_5_1b8_INFOS */
        if ((return_code = Si2164B_LoadFirmware_16(api, Si2164B_PATCH16_5_1b8, Si2164B_PATCH16_5_1b8_LINES)) != NO_Si2164B_ERROR) {
           SiTRACE ("Si2164B_LoadFirmware Si2164B_PATCH16_5_1b8 error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
           return return_code;
         }
        fw_loaded++;
      }
    }
    #endif /* Si2164B_PATCH16_5_1b8_LINES */
//#endif /* Si2164B_A50_COMPATIBLE */
//#ifdef    Si2164B_ES_COMPATIBLE
    SiTRACE ("Build Option: Si2164B_ES_COMPATIBLE\n");
    #ifdef    Si2164B_FIRMWARE_ES_FW_LINES
    if (!fw_loaded) {
      SiTRACE  ("Si2164B_FIRMWARE_ES_FW: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2164B_FIRMWARE_ES_FW_PART, Si2164B_FIRMWARE_ES_FW_ROM, Si2164B_FIRMWARE_ES_FW_PMAJOR, Si2164B_FIRMWARE_ES_FW_PMINOR, Si2164B_FIRMWARE_ES_FW_PBUILD );
      if  (((api->rsp->part_info.romid  == Si2164B_FIRMWARE_ES_FW_ROM   )
        &(
            ((api->rsp->part_info.part == 69 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 68 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 64 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 62 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 60 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
        ) )
        /*  One line for compatibility with early Si2164B and derivatives samples with NO NVM content */
        || ((api->rsp->part_info.romid  == 0   ) & ( api->rsp->part_info.part == 0  ) )
        ) {
        SiTRACE("Updating FW for 'Si21%2d NVM%c_%cb%d' (full download)\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
        #ifdef    Si2164B_FIRMWARE_ES_FW_INFOS
          SiTRACE(Si2164B_FIRMWARE_ES_FW_INFOS);
          SiERROR(Si2164B_FIRMWARE_ES_FW_INFOS);
        #endif /* Si2164B_FIRMWARE_ES_FW_INFOS */
        if ((return_code = Si2164B_LoadFirmware_16(api, Si2164B_FIRMWARE_ES_FW, Si2164B_FIRMWARE_ES_FW_LINES)) != NO_Si2164B_ERROR) {
           SiTRACE ("Si2164B_LoadFirmware error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
           return return_code;
         }
        fw_loaded++;
      }
    }
    #endif /* Si2164B_FIRMWARE_ES_FW_LINES */
    #ifdef    Si2164B_FIRMWARE16_0_Bb1_LINES
    if (!fw_loaded) {
      SiTRACE  ("Si2164B_FIRMWARE16_0_Bb1: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2164B_FIRMWARE16_0_Bb1_PART, Si2164B_FIRMWARE16_0_Bb1_ROM, Si2164B_FIRMWARE16_0_Bb1_PMAJOR, Si2164B_FIRMWARE16_0_Bb1_PMINOR, Si2164B_FIRMWARE16_0_Bb1_PBUILD );
      if  (((api->rsp->part_info.romid  == Si2164B_FIRMWARE16_0_Bb1_ROM   )
        &(
            ((api->rsp->part_info.part == 69 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 68 ) & (api->rsp->part_info.chiprev + 0x40 == 'C') )
         || ((api->rsp->part_info.part == 64 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 62 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 60 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
        ) )
        /*  One line for compatibility with early Si2164B and derivatives samples with NO NVM content */
        || ((api->rsp->part_info.romid  == Si2164B_FIRMWARE16_0_Bb1_ROM   ) & ( api->rsp->part_info.part == 0  ) )
        ) {
        SiTRACE("Updating FW for 'Si21%2d NVM%c_%cb%d' (full download)\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
        #ifdef    Si2164B_FIRMWARE16_0_Bb1_INFOS
          SiERROR(Si2164B_FIRMWARE16_0_Bb1_INFOS);
        #endif /* Si2164B_FIRMWARE16_0_Bb1_INFOS */
        if ((return_code = Si2164B_LoadFirmware_16(api, Si2164B_FIRMWARE16_0_Bb1, Si2164B_FIRMWARE16_0_Bb1_LINES)) != NO_Si2164B_ERROR) {
           SiTRACE ("Si2164B_LoadFirmware_16 error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
           return return_code;
         }
        fw_loaded++;
      }
    }
    #endif /*Si2164B_FIRMWARE16_0_Bb1_LINES*/
//#endif /* Si2164B_ES_COMPATIBLE */
#ifdef    Si2167_B25_COMPATIBLE
    SiLOGS ("Build Option: Si2167_B25_COMPATIBLE\n");
    #ifdef    Si2167B_PATCH16_2_5b0_LINES
    if (!fw_loaded) {
      SiTRACE  ("Si2164B_PATCH16_2_5b0: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2164B_PATCH16_2_5b0_PART, Si2164B_PATCH16_2_5b0_ROM, Si2164B_PATCH16_2_5b0_PMAJOR, Si2164B_PATCH16_2_5b0_PMINOR, Si2164B_PATCH16_2_5b0_PBUILD );
      if  ((api->rsp->part_info.romid  == Si2164B_PATCH16_2_5b0_ROM   )
         & (api->rsp->part_info.part == 67 ) & (api->rsp->part_info.chiprev + 0x40 == 'B')
         & (api->rsp->part_info.pmajor == Si2164B_PATCH16_2_5b0_PMAJOR)
         & (api->rsp->part_info.pminor == Si2164B_PATCH16_2_5b0_PMAJOR)
         & (api->rsp->part_info.pbuild == Si2164B_PATCH16_2_5b0_PBUILD)
        ) {
        SiTRACE("Updating FW for 'Si21%2d NVM%c_%cb%d' (patch)\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
        #ifdef    Si2164B_PATCH16_2_5b0_INFOS
          SiERROR(Si2164B_PATCH16_2_5b0_INFOS);
        #endif /* Si2164B_PATCH16_2_5b0_INFOS */
        if ((return_code = Si2164B_LoadFirmware_16(api, Si2164B_PATCH16_2_5b0, Si2164B_PATCH16_2_5b0_LINES)) != NO_Si2164B_ERROR) {
           SiTRACE ("Si2164B_LoadFirmware_16 Si2164B_PATCH16_2_5b0 error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
           return return_code;
         }
        fw_loaded++;
      }
    }
    #endif /* Si2164B_PATCH16_2_5b0_LINES */
#endif /* Si2167_B25_COMPATIBLE */
#ifdef    Si2164_A40_COMPATIBLE
    SiLOGS ("Build Option: Si2164_A40_COMPATIBLE\n");
    #ifdef    Si2164_SPI_4_0b28_BYTES
    if ( (api->spi_download) & (!fw_loaded) ) {
      SiLOGS   ("Checking if we can load Si2164_SPI_4_0b28\n");
      SiTRACE  ("Si2164_SPI_4_0b28_LINES: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2164_SPI_4_0b28_PART, Si2164_SPI_4_0b28_ROM, Si2164_SPI_4_0b28_PMAJOR, Si2164_SPI_4_0b28_PMINOR, Si2164_SPI_4_0b28_PBUILD );
      if ((api->rsp->part_info.romid  == Si2164_SPI_4_0b28_ROM   )
        &(  ((api->rsp->part_info.part == 64 ) & (api->rsp->part_info.chiprev + 0x40 == 'A') )
         || ((api->rsp->part_info.part == 62 ) & (api->rsp->part_info.chiprev + 0x40 == 'A') )
         || ((api->rsp->part_info.part == 60 ) & (api->rsp->part_info.chiprev + 0x40 == 'A') )
        )
        & (api->rsp->part_info.pmajor == Si2164_SPI_4_0b28_PMAJOR)
        & (api->rsp->part_info.pminor == Si2164_SPI_4_0b28_PMINOR)
        & (api->rsp->part_info.pbuild == Si2164_SPI_4_0b28_PBUILD)
          ) {
        SiTRACE("Updating FW via SPI for 'Si21%2d NVM%c_%cb%d'\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
        if ((return_code = Si2164B_LoadFirmwareSPI(api, Si2164_SPI_4_0b28, Si2164_SPI_4_0b28_BYTES, Si2164_SPI_4_0b28_KEY, Si2164_SPI_4_0b28_NUM )) != NO_Si2164B_ERROR) {
          SiTRACE ("Si2164B_LoadFirmwareSPI  error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
          return return_code;
        }
        #ifdef    Si2164_SPI_4_0b28_INFOS
          SiERROR(Si2164_SPI_4_0b28_INFOS);
        #endif /* Si2164_SPI_4_0b28_INFOS */
        fw_loaded++;
      }
    }
    #endif /* Si2164_SPI_4_0b28_BYTES */
    #ifdef    Si2164_PATCH16_current_64_LINES
    if (!fw_loaded) {
      SiTRACE  ("Si2164_PATCH16_current_64: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2164_PATCH16_current_64_PART, Si2164_PATCH16_current_64_ROM, Si2164_PATCH16_current_64_PMAJOR, Si2164_PATCH16_current_64_PMINOR, Si2164_PATCH16_current_64_PBUILD );
      if ((api->rsp->part_info.romid  == Si2164_PATCH16_current_64_ROM   )
        &(  ((api->rsp->part_info.part == 64 ) & (api->rsp->part_info.chiprev + 0x40 == 'A') )
         || ((api->rsp->part_info.part == 62 ) & (api->rsp->part_info.chiprev + 0x40 == 'A') )
         || ((api->rsp->part_info.part == 60 ) & (api->rsp->part_info.chiprev + 0x40 == 'A') )
        )
        & (api->rsp->part_info.pmajor == Si2164_PATCH16_current_64_PMAJOR)
        & (api->rsp->part_info.pminor == Si2164_PATCH16_current_64_PMINOR)
        & (api->rsp->part_info.pbuild == Si2164_PATCH16_current_64_PBUILD)
          ) {
        SiTRACE("Updating FW for 'Si21%2d NVM%c_%cb%d'\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
        if ((return_code = Si2164B_LoadFirmware_16(api, Si2164_PATCH16_current_64, Si2164_PATCH16_current_64_LINES)) != NO_Si2164B_ERROR) {
          SiTRACE ("Si2164B_LoadFirmware_16 error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
          return return_code;
        }
        #ifdef    Si2164_PATCH16_current_64_INFOS
          SiERROR(Si2164_PATCH16_current_64_INFOS);
        #endif /* Si2164_PATCH16_current_64_INFOS */
        fw_loaded++;
      }
    }
    #endif /* Si2164_PATCH16_current_64_LINES */
    #ifdef    Si2164_SPI_current_69B_LINES
    if ( (api->spi_download) & (!fw_loaded) ) {
      SiLOGS   ("Checking if we can load Si2164_SPI_current_69B\n");
      SiTRACE  ("Si2164_SPI_current_69B_LINES: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2164_SPI_current_69B_PART, Si2164_SPI_current_69B_ROM, Si2164_SPI_current_69B_PMAJOR, Si2164_SPI_current_69B_PMINOR, Si2164_SPI_current_69B_PBUILD );
      if ((api->rsp->part_info.romid  == Si2164_SPI_current_69B_ROM   )
        &(  ((api->rsp->part_info.part == 69 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 68 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
        )
        & (api->rsp->part_info.pmajor == Si2164_SPI_current_69B_PMAJOR)
        & (api->rsp->part_info.pminor == Si2164_SPI_current_69B_PMINOR)
        & (api->rsp->part_info.pbuild == Si2164_SPI_current_69B_PBUILD)
          ) {
        SiTRACE("Updating FW via SPI for 'Si21%2d NVM%c_%cb%d'\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
        if ((return_code = Si2164B_LoadFirmwareSPI(api, Si2164_SPI_current_69B, Si2164_SPI_current_69B_LINES, Si2164_SPI_current_69B_KEY, Si2164_SPI_current_69B_NUM )) != NO_Si2164B_ERROR) {
          SiTRACE ("Si2164B_LoadFirmwareSPI  error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
          return return_code;
        }
        #ifdef    Si2164_SPI_current_69B_INFOS
          SiERROR(Si2164_SPI_current_69B_INFOS);
        #endif /* Si2164_SPI_current_69B_INFOS */
        fw_loaded++;
      }
    }
    #endif /* Si2164_SPI_current_69B_LINES */
    #ifdef    Si2164_PATCH16_current_69B_LINES
    if (!fw_loaded) {
      SiTRACE  ("Si2164_PATCH16_current_69B: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2164_PATCH16_current_69B_PART, Si2164_PATCH16_current_69B_ROM, Si2164_PATCH16_current_69B_PMAJOR, Si2164_PATCH16_current_69B_PMINOR, Si2164_PATCH16_current_69B_PBUILD );
      if ((api->rsp->part_info.romid  == Si2164_PATCH16_current_69B_ROM   )
        &(  ((api->rsp->part_info.part == 69 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
         || ((api->rsp->part_info.part == 68 ) & (api->rsp->part_info.chiprev + 0x40 == 'B') )
        )
        & (api->rsp->part_info.pmajor == Si2164_PATCH16_current_69B_PMAJOR)
        & (api->rsp->part_info.pminor == Si2164_PATCH16_current_69B_PMINOR)
        & (api->rsp->part_info.pbuild == Si2164_PATCH16_current_69B_PBUILD)
          ) {
        SiTRACE("Updating FW for 'Si21%2d NVM%c_%cb%d'\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
        if ((return_code = Si2164B_LoadFirmware_16(api, Si2164_PATCH16_current_69B, Si2164_PATCH16_current_69B_LINES)) != NO_Si2164B_ERROR) {
          SiTRACE ("Si2164B_LoadFirmware_16 error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
          return return_code;
        }
        #ifdef    Si2164_PATCH16_current_69B_INFOS
          SiERROR(Si2164_PATCH16_current_69B_INFOS);
        #endif /* Si2164_PATCH16_current_69B_INFOS */
        fw_loaded++;
      }
    }
    #endif /* Si2164_PATCH16_current_69B_LINES */
#endif /* Si2164_A40_COMPATIBLE */
#ifdef    Si2169_30_COMPATIBLE
    SiLOGS ("Build Option: Si2169_30_COMPATIBLE\n");
    #ifdef    Si2169_PATCH_3_0b18_LINES
    if (!fw_loaded) {
      SiTRACE  ("Si2169_PATCH_3_0b18: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2169_PATCH_3_0b18_PART, Si2169_PATCH_3_0b18_ROM, Si2169_PATCH_3_0b18_PMAJOR, Si2169_PATCH_3_0b18_PMINOR, Si2169_PATCH_3_0b18_PBUILD );
      if ((api->rsp->part_info.romid  == Si2169_PATCH_3_0b18_ROM   )
        &((api->rsp->part_info.part   == 69 ) || (api->rsp->part_info.part == 68 ))
        & (api->rsp->part_info.pmajor == Si2169_PATCH_3_0b18_PMAJOR)
        & (api->rsp->part_info.pminor == Si2169_PATCH_3_0b18_PMINOR)
        & (api->rsp->part_info.pbuild == Si2169_PATCH_3_0b18_PBUILD)) {
          SiTRACE("Updating FW for 'Si21%2d_ROM%x %c_%c_b%d'\n", api->rsp->part_info.part, api->rsp->part_info.romid, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
          #ifdef    Si2169_PATCH_3_0b18_INFOS
          SiERROR(Si2169_PATCH_3_0b18_INFOS);
          #endif /* Si2169_PATCH_3_0b18_INFOS */
          if ((return_code = Si2164B_LoadFirmware(api, Si2169_PATCH_3_0b18, Si2169_PATCH_3_0b18_LINES)) != NO_Si2164B_ERROR) {
              SiTRACE ("Si2169_LoadFirmware error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
              return return_code;
          }
          fw_loaded++;
        }
    }
    #endif /* Si2169_PATCH_3_0b18_LINES */
#endif /* Si2169_30_COMPATIBLE */
#ifdef    Si2167B_20_COMPATIBLE
    SiLOGS ("Build Option: Si2167B_20_COMPATIBLE\n");
    #ifdef    Si2167B_PATCH_2_0b5_LINES
    if (!fw_loaded) {
      SiTRACE  ("Si2167B_PATCH_2_0b5: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2167B_PATCH_2_0b5_PART, Si2167B_PATCH_2_0b5_ROM, Si2167B_PATCH_2_0b5_PMAJOR, Si2167B_PATCH_2_0b5_PMINOR, Si2167B_PATCH_2_0b5_PBUILD );
      if ((api->rsp->part_info.romid  == Si2167B_PATCH_2_0b5_ROM   )
        &((api->rsp->part_info.part   == 67 ) || (api->rsp->part_info.part == 66 ))
        & (api->rsp->part_info.pmajor == Si2167B_PATCH_2_0b5_PMAJOR)
        & (api->rsp->part_info.pminor == Si2167B_PATCH_2_0b5_PMINOR)
        & (api->rsp->part_info.pbuild == Si2167B_PATCH_2_0b5_PBUILD)) {
        SiTRACE("Updating FW for 'Si21%2d_FW_%c_%c_b%d'\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
        #ifdef    Si2167B_PATCH_2_0b5_INFOS
        SiERROR(Si2167B_PATCH_2_0b5_INFOS);
        #endif /* Si2167B_PATCH_2_0b5_INFOS */
        if ((return_code = Si2167B_LoadFirmware(api, Si2167B_PATCH_2_0b5, Si2167B_PATCH_2_0b5_LINES)) != NO_Si2167B_ERROR) {
          SiTRACE ("Si2167B_LoadPatch error 0x%02x: %s\n", return_code, Si2167B_L1_API_ERROR_TEXT(return_code) );
          return return_code;
        }
        fw_loaded++;
      }
    }
    #endif /* Si2167B_PATCH_2_0b5_LINES */
    #ifdef    Si2167B_20_PATCH_CUSTOMER_LINES
    if (!fw_loaded) {
      SiLOGS ("Si2167B_20_PATCH_CUSTOMER: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2167B_20_PATCH_CUSTOMER_PART, Si2167B_20_PATCH_CUSTOMER_ROM, Si2167B_20_PATCH_CUSTOMER_PMAJOR, Si2167B_20_PATCH_CUSTOMER_PMINOR, Si2167B_20_PATCH_CUSTOMER_PBUILD );
      if ((api->rsp->part_info.romid  == Si2167B_20_PATCH_CUSTOMER_ROM   )
        &((api->rsp->part_info.part   == 67 ) || (api->rsp->part_info.part == 66 ))
        & (api->rsp->part_info.pmajor == Si2167B_20_PATCH_CUSTOMER_PMAJOR)
        & (api->rsp->part_info.pminor == Si2167B_20_PATCH_CUSTOMER_PMINOR)
        & (api->rsp->part_info.pbuild == Si2167B_20_PATCH_CUSTOMER_PBUILD)) {
        SiTRACE("Updating FW for 'Si21%2d_ROM%x %c_%c_b%d'\n", api->rsp->part_info.part, api->rsp->part_info.romid, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
          #ifdef    Si2167B_20_PATCH_CUSTOMER_INFOS
          SiERROR(Si2167B_20_PATCH_CUSTOMER_INFOS);
          #endif /* Si2167B_20_PATCH_CUSTOMER_INFOS */
          if ((return_code = Si2167B_LoadFirmware(api, Si2167B_20_PATCH_CUSTOMER, Si2167B_20_PATCH_CUSTOMER_LINES)) != NO_Si2167B_ERROR) {
            SiTRACE ("Si2167B_LoadFirmware error 0x%02x: %s\n", return_code, Si2167B_L1_API_ERROR_TEXT(return_code) );
            return return_code;
          }
          fw_loaded++;
        }
    }
    #endif /* Si2167B_20_PATCH_CUSTOMER_LINES */
    #ifdef    Si2167B_PATCH_2_9b1_LINES
    SiTRACE("api->load_DVB_C_Blindlock_Patch %d\n",api->load_DVB_C_Blindlock_Patch);
    if (api->load_DVB_C_Blindlock_Patch == 1) {
      if (!fw_loaded) {
        SiTRACE  ("Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2167B_PATCH_2_9b1_PART, Si2167B_PATCH_2_9b1_ROM, Si2167B_PATCH_2_9b1_PMAJOR, Si2167B_PATCH_2_9b1_PMINOR, Si2167B_PATCH_2_9b1_PBUILD );
        if ((api->rsp->part_info.romid  == Si2167B_PATCH_2_9b1_ROM   )
          &(
            ((api->rsp->part_info.part   == 67 ) & (api->rsp->part_info.chiprev = 'B') )
         || ((api->rsp->part_info.part   == 66 ) & (api->rsp->part_info.chiprev = 'B') )
         || ((api->rsp->part_info.part   == 65 ) & (api->rsp->part_info.chiprev = 'B') )
          )
          & (api->rsp->part_info.pmajor == '2')
          & (api->rsp->part_info.pminor == '0')
          & (api->rsp->part_info.pbuild ==  5 )) {
          SiTRACE("Updating FW for 'Si21%2d_FW_%c_%c_b%d'\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
          #ifdef    Si2167B_PATCH_2_9b1_INFOS
          SiTRACE("%s\n", Si2167B_PATCH_2_9b1_INFOS);
          #endif /* Si2167B_PATCH_2_9b1_INFOS */
          SiTRACE("%s\n", "Si2167B_PATCH_2_9b1");
          if ((return_code = Si2164B_LoadFirmware(api, Si2167B_PATCH_2_9b1, Si2167B_PATCH_2_9b1_LINES)) != NO_Si2164B_ERROR) {
            SiTRACE ("Si2164B_LoadPatch error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
            return return_code;
          }
          fw_loaded++;
        }
      }
    }
    #endif /* Si2167B_PATCH_2_9b1_LINES */
    #ifdef    Si2167B_PATCH_2_0b26_LINES
    if (!fw_loaded) {
      SiTRACE  ("Si2167B_PATCH_2_0b26: Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2167B_PATCH_2_0b26_PART, Si2167B_PATCH_2_0b26_ROM, Si2167B_PATCH_2_0b26_PMAJOR, Si2167B_PATCH_2_0b26_PMINOR, Si2167B_PATCH_2_0b26_PBUILD );
      if ((api->rsp->part_info.romid  == Si2167B_PATCH_2_0b26_ROM   )
        &((api->rsp->part_info.part   == 67 ) || (api->rsp->part_info.part == 66 ))
        & (api->rsp->part_info.pmajor == Si2167B_PATCH_2_0b26_PMAJOR)
        & (api->rsp->part_info.pminor == Si2167B_PATCH_2_0b26_PMINOR)
        & (api->rsp->part_info.pbuild == Si2167B_PATCH_2_0b26_PBUILD)) {
        SiTRACE("Updating FW for 'Si21%2d_FW_%c_%c_b%d'\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
        #ifdef    Si2167B_PATCH_2_0b26_INFOS
        SiERROR(Si2167B_PATCH_2_0b26_INFOS);
        #endif /* Si2167B_PATCH_2_0b26_INFOS */
        if ((return_code = Si2164B_LoadFirmware(api, Si2167B_PATCH_2_0b26, Si2167B_PATCH_2_0b26_LINES)) != NO_Si2164B_ERROR) {
          SiTRACE ("Si2167B_LoadPatch error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
           return return_code;
         }
        fw_loaded++;
      }
    }
    #endif /* Si2167B_PATCH_2_0b26_LINES */
#endif /* Si2167B_20_COMPATIBLE */
#ifdef    Si2167B_22_COMPATIBLE
    SiLOGS("Build Option: Si2167B_22_COMPATIBLE\n");
    #ifdef    Si2167B_PATCH_2_8b1_LINES
    SiTRACE("api->load_DVB_C_Blindlock_Patch %d\n",api->load_DVB_C_Blindlock_Patch);
    if (api->load_DVB_C_Blindlock_Patch == 1) {
      if (!fw_loaded) {
        SiTRACE  ("Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2167B_PATCH_2_8b1_PART, Si2167B_PATCH_2_8b1_ROM, Si2167B_PATCH_2_8b1_PMAJOR, Si2167B_PATCH_2_8b1_PMINOR, Si2167B_PATCH_2_8b1_PBUILD );
        if ((api->rsp->part_info.romid  == Si2167B_PATCH_2_8b1_ROM   )
          &(
            ((api->rsp->part_info.part   == 67 ) & (api->rsp->part_info.chiprev = 'B') )
         || ((api->rsp->part_info.part   == 66 ) & (api->rsp->part_info.chiprev = 'B') )
         || ((api->rsp->part_info.part   == 65 ) & (api->rsp->part_info.chiprev = 'B') )
          )
          & (api->rsp->part_info.pmajor == '2')
          & (api->rsp->part_info.pminor == '2')
          & (api->rsp->part_info.pbuild ==  1 )) {
          SiTRACE("Updating FW for 'Si21%2d_FW_%c_%c_b%d'\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
          #ifdef    Si2167B_PATCH_2_8b1_INFOS
          SiTRACE("%s\n", Si2167B_PATCH_2_8b1_INFOS);
          #endif /* Si2167B_PATCH_2_8b1_INFOS */
          SiTRACE("%s\n", "Si2167B_PATCH_2_8b1");
          if ((return_code = Si2164B_LoadFirmware(api, Si2167B_PATCH_2_8b1, Si2167B_PATCH_2_8b1_LINES)) != NO_Si2164B_ERROR) {
            SiTRACE ("Si2164B_LoadPatch error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
            return return_code;
          }
          fw_loaded++;
        }
      }
    }
    #endif /* Si2167B_PATCH_2_8b1_LINES */
    #ifdef    Si2167B_PATCH_2_2b7_LINES
    if (!fw_loaded) {
      SiTRACE  ("Is this part a  'Si21%2d_ROM%x_%c_%c_b%d'?\n", Si2167B_PATCH_2_2b7_PART, Si2167B_PATCH_2_2b7_ROM, Si2167B_PATCH_2_2b7_PMAJOR, Si2167B_PATCH_2_2b7_PMINOR, Si2167B_PATCH_2_2b7_PBUILD );
      if ((api->rsp->part_info.romid  == Si2167B_PATCH_2_2b7_ROM   )
        &(
          (api->rsp->part_info.part   == 67 )
       || (api->rsp->part_info.part   == 66 )
       || (api->rsp->part_info.part   == 65 )
       || (api->rsp->part_info.part   ==  0 )
        )
        & (api->rsp->part_info.pmajor == Si2167B_PATCH_2_2b7_PMAJOR)
        & (api->rsp->part_info.pminor == Si2167B_PATCH_2_2b7_PMINOR)
        & (api->rsp->part_info.pbuild == Si2167B_PATCH_2_2b7_PBUILD)
        ) {
        SiTRACE("Updating FW for 'Si21%2d_FW_%c_%c_b%d'\n", api->rsp->part_info.part, api->rsp->part_info.pmajor, api->rsp->part_info.pminor, api->rsp->part_info.pbuild );
        #ifdef    Si2167B_PATCH_2_2b7_INFOS
        SiTRACE("%s\n", Si2167B_PATCH_2_2b7_INFOS);
        #endif /* Si2167B_PATCH_2_2b7_INFOS */
        SiTRACE("%s\n", "Si2167B_PATCH__2_2b7");
        if ((return_code = Si2164B_LoadFirmware(api, Si2167B_PATCH_2_2b7, Si2167B_PATCH_2_2b7_LINES)) != NO_Si2164B_ERROR) {
          SiTRACE ("Si2164B_LoadPatch error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
          return return_code;
        }
        fw_loaded++;
      }
    }
    #endif /* Si2167B_PATCH_2_2b7_LINES */
#endif /* Si2167B_22_COMPATIBLE */

    if (!fw_loaded) {
      SiTRACE ("Si2164B_LoadFirmware error: NO Firmware Loaded! Possible part/code/fw incompatibility !\n");
      SiERROR ("Si2164B_LoadFirmware error: NO Firmware Loaded! Possible part/code/fw incompatibility !\n");
      return ERROR_Si2164B_LOADING_FIRMWARE;
    }

    /* Start the Firmware */
    if ((return_code = Si2164B_StartFirmware(api)) != NO_Si2164B_ERROR) {
        /* Start firmware */
        SiTRACE ("Si2164B_StartFirmware error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }

    /* Check the FW version */
    if ((return_code = Si2164B_L1_GET_REV (api)) != NO_Si2164B_ERROR) {
      SiTRACE ("Si2164B_L1_GET_REV error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
      if (api->spi_download) {
        api->spi_download = 0;
        SiERROR ("FW download using SPI failed, trying i2c download as fallback solution\n");
        return Si2164B_PowerUpWithPatch(api);
      } else {
        return return_code;
      }
    }

    if ((api->rsp->get_rev.mcm_die) != Si2164B_GET_REV_RESPONSE_MCM_DIE_SINGLE) {
      SiTRACE("Si21%2d%d-%c%c%c Die %c Part running 'FW_%c_%cb%d'\n", api->rsp->part_info.part
                                                                    , 2
                                                                    , api->rsp->part_info.chiprev + 0x40
                                                                    , api->rsp->part_info.pmajor
                                                                    , api->rsp->part_info.pminor
                                                                    , api->rsp->get_rev.mcm_die   + 0x40
                                                                    , api->rsp->get_rev.cmpmajor
                                                                    , api->rsp->get_rev.cmpminor
                                                                    , api->rsp->get_rev.cmpbuild );
    } else {
      SiTRACE("Si21%2d-%c%c%c Part running 'FW_%c_%cb%d'\n", api->rsp->part_info.part
                                                                    , api->rsp->part_info.chiprev + 0x40
                                                                    , api->rsp->part_info.pmajor
                                                                    , api->rsp->part_info.pminor
                                                                    , api->rsp->get_rev.cmpmajor
                                                                    , api->rsp->get_rev.cmpminor
                                                                    , api->rsp->get_rev.cmpbuild );
    }

    return NO_Si2164B_ERROR;
}
/************************************************************************************************************************
  NAME: Si2164B_LoadFirmware_16
  DESCRIPTION: Load firmware from firmware_struct array in Si2164B_Firmware_x_y_build_z.h file into Si2164B
              Requires Si2164B to be in bootloader mode after PowerUp

  Parameter:  Si2164B Context (I2C address)
  Parameter:  pointer to firmware_struct array
  Parameter:  number of lines in firmware table array (size in bytes / firmware_struct)
  Returns:    Si2164B/I2C transaction error code, NO_Si2164B_ERROR if successful
************************************************************************************************************************/
signed int Si2164B_LoadFirmware_16     (L1_Si2164B_Context *api, firmware_struct fw_table[], signed int nbLines)
{
    signed int return_code;
    signed int line;
    signed int load_start_ms;
    return_code = NO_Si2164B_ERROR;

    SiTRACE ("Si2164B_LoadFirmware_16 starting...\n");
    SiTRACE ("Si2164B_LoadFirmware_16 nbLines %d\n", nbLines);
    load_start_ms = system_time();
    /* for each line in fw_table */
    for (line = 0; line < nbLines; line++) {
      if (fw_table[line].firmware_len > 0)  /* don't download if length is 0 , e.g. dummy firmware */
    {
        /* send firmware_len bytes (up to 16) to Si2164B */
        if ((return_code = Si2164B_L1_API_Patch(api, fw_table[line].firmware_len, fw_table[line].firmware_table)) != NO_Si2164B_ERROR)
        {
          SiTRACE("Si2164B_LoadFirmware_16 error 0x%02x patching line %d: %s\n", return_code, line, Si2164B_L1_API_ERROR_TEXT(return_code) );
          if (line == 0) {
          SiTRACE("The firmware is incompatible with the part!\n");
          }
          return ERROR_Si2164B_LOADING_FIRMWARE;
        }
        if (line==3) {SiTraceConfiguration("traces suspend");}
    }
    }
    SiTraceConfiguration("traces resume");
    api->i2c_download_ms = system_time() - load_start_ms;
    SiTRACE ("Si2164B_LoadFirmware_16 took %4d ms\n", api->i2c_download_ms);
    /* Storing Properties startup configuration in propShadow                              */
    /* !! Do NOT change the content of Si2164B_storePropertiesDefaults                   !! */
    /* !! It should reflect the part internal property settings after firmware download !! */
    SiTRACE ("Si2164B_storePropertiesDefaults\n");
    Si2164B_storePropertiesDefaults (api->propShadow);

    SiTRACE ("Si2164B_LoadFirmware_16 complete...\n");
    return NO_Si2164B_ERROR;
}
/************************************************************************************************************************
  NAME: Si2164B_LoadFirmwareSPI
  DESCRIPTON: Load firmware from FIRMWARE_TABLE array in Si2164B_Firmware_x_y_build_z.h file into Si2164B
              Requires Si2164B to be in bootloader mode after PowerUp
  Programming Guide Reference:    Flowchart A.3 (Download FW PATCH flowchart)

  Parameter:  Si2164B Context (I2C address)
  Parameter:  pointer to firmware table array
  Parameter:  number of lines in firmware table array (size in bytes / BYTES_PER_LINE)
  Returns:    Si2164B/I2C transaction error code, NO_Si2164B_ERROR if successful
************************************************************************************************************************/
#if 0 // 20140513 del jhkwon
signed int Si2164B_LoadFirmwareSPI     (L1_Si2164B_Context *api, firmware_struct fw_table[], signed int nbLines, unsigned char pbl_key,  unsigned char pbl_num )
{
    #define SPI_MAX_BUFFER_SIZE 64
    signed int return_code;
    signed int load_start_ms;
    signed int fw_index;

//  #define   SPI_DEBUGGING
  #ifdef    SPI_DEBUGGING
    unsigned char spi_bytes[4];
    unsigned char reg_byte [1];
  #endif /* SPI_DEBUGGING */

    unsigned char PATCH_IMAGE_SPI[] = {0x05,0x20,0x00,0x00,0x00,0x00,0x00,0x00};

    return_code = NO_Si2164B_ERROR;
    fw_index = 0;

    /* <porting> For SiLabs EVB, no call to L0_EnableSPI is necessary
          because all settings are handled internally to the Cypress FW.
          This needs to be adapted to each SPI setup
    */
    if ( L0_EnableSPI(0x00) == 0) {
      SiERROR ("SPI can't be enabled. It's not supported by the L0!\n");
      return ERROR_Si2164B_LOADING_FIRMWARE;
    }

    SiTRACE ("Si2164B_LoadFirmwareSPI starting... nbBytes %d\n", nbBytes);

    L0_WriteCommandBytes  (api->i2c, 8, PATCH_IMAGE_SPI);
    Si2164B_L1_CheckStatus (api);

    /* Set all spi__link fields to allow SPI download */
    api->cmd->spi_link.subcode       = Si2164B_SPI_LINK_CMD_SUBCODE_CODE;
    api->cmd->spi_link.spi_pbl_key   = pbl_key;
    api->cmd->spi_link.spi_pbl_num   = pbl_num;
    api->cmd->spi_link.spi_conf_clk  = api->spi_clk_pin;
    api->cmd->spi_link.spi_clk_pola  = api->spi_clk_pola;
    api->cmd->spi_link.spi_conf_data = api->spi_data_pin;
    api->cmd->spi_link.spi_data_dir  = api->spi_data_order;
    api->cmd->spi_link.spi_enable    = Si2164B_SPI_LINK_CMD_SPI_ENABLE_ENABLE;

    Si2164B_L1_SendCommand2(api, Si2164B_SPI_LINK_CMD_CODE);

    /* Set spi_link fields to prepare disabling SPI download */
    api->cmd->spi_link.spi_pbl_key   =   0;
    api->cmd->spi_link.spi_conf_clk  = Si2164B_SPI_LINK_CMD_SPI_CONF_CLK_DISABLE;
    api->cmd->spi_link.spi_conf_data = Si2164B_SPI_LINK_CMD_SPI_CONF_DATA_DISABLE;
    api->cmd->spi_link.spi_enable    = Si2164B_SPI_LINK_CMD_SPI_ENABLE_DISABLE;

    load_start_ms = system_time();

 #ifdef    SPI_DEBUGGING
    spi_bytes[0] = 0xff; spi_bytes[1] = 0x00; L0_WriteBytes(api->i2c, 0x00, 2, spi_bytes);
    api->i2c->indexSize  = 2; api->i2c->mustReadWithoutStop = 0;

    L0_ReadBytes(api->i2c, 0x57, 1, reg_byte); SiTRACE(" spi_state      %d\n", reg_byte[0]&0x07 );
    L0_ReadBytes(api->i2c, 0x55, 1, reg_byte); SiTRACE(" spi_crc_status %d\n", reg_byte[0]&0x01 );

    for (fw_index = 0; fw_index < 20; fw_index ++) {
      return_code = L0_LoadSPIoverGPIF(fw_table+fw_index,  1);
      L0_ReadBytes(api->i2c, 0x57, 1, reg_byte);  SiTRACE(" spi_state      %d\n", reg_byte[0]&0x07 );
    }
 #endif /* SPI_DEBUGGING */

    return_code = L0_LoadSPIoverGPIF(fw_table+fw_index, nbBytes-fw_index);
    api->spi_download_ms = system_time() - load_start_ms;
    SiTRACE ("L0_LoadSPIoverGPIF     took %4d ms\n", api->spi_download_ms);

 #ifdef    SPI_DEBUGGING
    L0_ReadBytes(api->i2c, 0x57, 1, reg_byte); SiTRACE(" spi_state      %d\n", reg_byte[0]&0x07 );
    L0_ReadBytes(api->i2c, 0x55, 1, reg_byte); SiTRACE(" spi_crc_status %d\n", reg_byte[0]&0x01 );

    api->i2c->indexSize  = 0;
    spi_bytes[0] = 0xfe; spi_bytes[1] = 0x00; L0_WriteBytes(api->i2c, 0x00, 2, spi_bytes);
 #endif /* SPI_DEBUGGING */

    if ( return_code != 0 )
    {
      SiTRACE("Si2164B_LoadFirmwareSPI  error 0x%02x loading %d bytes: %s\n", return_code, nbBytes, Si2164B_L1_API_ERROR_TEXT(return_code) );
      SiERROR("Si2164B_LoadFirmwareSPI  error\n");
      return_code = ERROR_Si2164B_LOADING_FIRMWARE;
    } else {
      /* Storing Properties startup configuration in propShadow                              */
      /* !! Do NOT change the content of Si2164B_storePropertiesDefaults                   !! */
      /* !! It should reflect the part internal property settings after firmware download !! */
      SiTRACE ("Si2164B_storePropertiesDefaults\n");
      Si2164B_storePropertiesDefaults (api->propShadow);
      SiTRACE ("Si2164B_LoadFirmwareSPI complete...\n");
      return_code = NO_Si2164B_ERROR;
    }

    Si2164B_L1_SendCommand2(api, Si2164B_SPI_LINK_CMD_CODE);

    L0_DisableSPI();

    return return_code;
}
#endif
#if 0
/************************************************************************************************************************
  NAME: Si2164B_LoadFirmwareSPI_Split
  DESCRIPTON: Load firmware from FIRMWARE_TABLE array in Si2164B_Firmware_x_y_build_z.h file into Si2164B
              Requires Si2164B to be in bootloader mode after PowerUp
  Programming Guide Reference:

  Parameter:  Si2164B Context (I2C address)
  Parameter:  fw_table, pointer to firmware table array
  Parameter:  nbBytes, number of bytes in firmware table array
  Parameter:  pbl_key, one byte KEY used to detect the start of a FW sequence
  Parameter:  pbl_num, number of pbl_key bytes used to detect the start of a FW sequence
  Parameter:  num_split, number of portions in the FW_table
  Returns:    split_list, list of split points
************************************************************************************************************************/
int Si2164B_LoadFirmwareSPI_Split(L1_Si2164B_Context *api, unsigned char *fw_table, int nbBytes, unsigned char pbl_key,  unsigned char pbl_num , unsigned int num_split, unsigned int *split_list)
{
    #define SPI_MAX_BUFFER_SIZE 64
    int return_code;
    int load_start_ms;
    int fw_index;
    int i;

//  #define   SPI_DEBUGGING
  #ifdef    SPI_DEBUGGING
    unsigned char spi_bytes[4];
    unsigned char reg_byte [1];
  #endif /* SPI_DEBUGGING */

    unsigned char PATCH_IMAGE_SPI[] = {0x05,0x20,0x00,0x00,0x00,0x00,0x00,0x00};

    return_code = NO_Si2164B_ERROR;
    fw_index = 0;

    /* <porting> For SiLabs EVB, no call to L0_EnableSPI is necessary
          because all settings are handled internally to the Cypress FW.
          This needs to be adapted to each SPI setup
    */
    if ( L0_EnableSPI(0x00) == 0) {
      SiERROR ("SPI can't be enabled. It's not supported by the L0!\n");
      return ERROR_Si2164B_LOADING_FIRMWARE;
    }

    SiTRACE ("Si2164B_LoadFirmwareSPI_Split starting... nbBytes %d num_split %d\n", nbBytes, num_split);

    L0_WriteCommandBytes  (api->i2c, 8, PATCH_IMAGE_SPI);
    Si2164B_L1_CheckStatus (api);

    /* Set all spi__link fields to allow SPI download */
    api->cmd->spi_link.subcode       = Si2164B_SPI_LINK_CMD_SUBCODE_CODE;
    api->cmd->spi_link.spi_pbl_key   = pbl_key;
    api->cmd->spi_link.spi_pbl_num   = pbl_num;
    api->cmd->spi_link.spi_conf_clk  = api->spi_clk_pin;
    api->cmd->spi_link.spi_clk_pola  = api->spi_clk_pola;
    api->cmd->spi_link.spi_conf_data = api->spi_data_pin;
    api->cmd->spi_link.spi_data_dir  = api->spi_data_order;
    api->cmd->spi_link.spi_enable    = Si2164B_SPI_LINK_CMD_SPI_ENABLE_ENABLE;

    Si2164B_L1_SendCommand2(api, Si2164B_SPI_LINK_CMD_CODE);

    /* Set spi_link fields to prepare disabling SPI download */
    api->cmd->spi_link.spi_pbl_key   =   0;
    api->cmd->spi_link.spi_conf_clk  = Si2164B_SPI_LINK_CMD_SPI_CONF_CLK_DISABLE;
    api->cmd->spi_link.spi_conf_data = Si2164B_SPI_LINK_CMD_SPI_CONF_DATA_DISABLE;
    api->cmd->spi_link.spi_enable    = Si2164B_SPI_LINK_CMD_SPI_ENABLE_DISABLE;

    load_start_ms = system_time();

 #ifdef    SPI_DEBUGGING
    spi_bytes[0] = 0xff; spi_bytes[1] = 0x00; L0_WriteBytes(api->i2c, 0x00, 2, spi_bytes);
    api->i2c->indexSize  = 2; api->i2c->mustReadWithoutStop = 0;

    L0_ReadBytes(api->i2c, 0x57, 1, reg_byte); SiTRACE(" spi_state      %d\n", reg_byte[0]&0x07 );
    L0_ReadBytes(api->i2c, 0x55, 1, reg_byte); SiTRACE(" spi_crc_status %d\n", reg_byte[0]&0x01 );

    for (fw_index = 0; fw_index < 20; fw_index ++) {
      return_code = L0_LoadSPIoverGPIF(fw_table+fw_index,  1);
      L0_ReadBytes(api->i2c, 0x57, 1, reg_byte);  SiTRACE(" spi_state      %d\n", reg_byte[0]&0x07 );
    }
 #endif /* SPI_DEBUGGING */

    /* Send SPI bytes, using max available buffer size */
    for (i=0; i< num_split; i++) {
      if (split_list[i]-fw_index > api->spi_buffer_size) {
        return_code = L0_LoadSPIoverGPIF(fw_table+fw_index, split_list[i-1]-fw_index);
        fw_index = split_list[i-1];
      }
    }
     /* Send trailing SPI bytes */
    if (split_list[i-1]-fw_index > 0) {
      return_code = L0_LoadSPIoverGPIF(fw_table+fw_index, split_list[i-1]-fw_index);
    }

    api->spi_download_ms = system_time() - load_start_ms;
    SiTRACE ("L0_LoadSPIoverGPIF     took %4d ms\n", api->spi_download_ms);

 #ifdef    SPI_DEBUGGING
    L0_ReadBytes(api->i2c, 0x57, 1, reg_byte); SiTRACE(" spi_state      %d\n", reg_byte[0]&0x07 );
    L0_ReadBytes(api->i2c, 0x55, 1, reg_byte); SiTRACE(" spi_crc_status %d\n", reg_byte[0]&0x01 );

    api->i2c->indexSize  = 0;
    spi_bytes[0] = 0xfe; spi_bytes[1] = 0x00; L0_WriteBytes(api->i2c, 0x00, 2, spi_bytes);
 #endif /* SPI_DEBUGGING */

    if ( return_code != 0 )
    {
      SiTRACE("Si2164B_LoadFirmwareSPI_Split  error 0x%02x loading %d bytes: %s\n", return_code, nbBytes, Si2164B_L1_API_ERROR_TEXT(return_code) );
      SiERROR("Si2164B_LoadFirmwareSPI_Split  error\n");
      return_code = ERROR_Si2164B_LOADING_FIRMWARE;
    } else {
      /* Storing Properties startup configuration in propShadow                              */
      /* !! Do NOT change the content of Si2164B_storePropertiesDefaults                   !! */
      /* !! It should reflect the part internal property settings after firmware download !! */
      SiTRACE ("Si2164B_storePropertiesDefaults\n");
      Si2164B_storePropertiesDefaults (api->propShadow);
      SiTRACE ("Si2164B_LoadFirmwareSPI_Split complete...\n");
      return_code = NO_Si2164B_ERROR;
    }

    Si2164B_L1_SendCommand2(api, Si2164B_SPI_LINK_CMD_CODE);

    L0_DisableSPI();

    return return_code;
}
#endif
/************************************************************************************************************************
  NAME: Si2164B_StartFirmware
  DESCRIPTION: Start Si2164B firmware (put the Si2164B into run mode)
  Parameter:   Si2164B Context (I2C address)
  Parameter (passed by Reference):   ExitBootloadeer Response Status byte : tunint, atvint, dtvint, err, cts
  Returns:     I2C transaction error code, NO_Si2164B_ERROR if successful
************************************************************************************************************************/
signed int Si2164B_StartFirmware       (L1_Si2164B_Context *api)
{
    if (Si2164B_L1_EXIT_BOOTLOADER(api, Si2164B_EXIT_BOOTLOADER_CMD_FUNC_NORMAL, Si2164B_EXIT_BOOTLOADER_CMD_CTSIEN_OFF) != NO_Si2164B_ERROR)
    {
        return ERROR_Si2164B_STARTING_FIRMWARE;
    }

    return NO_Si2164B_ERROR;
}
/************************************************************************************************************************
  NAME: Si2164B_Init
  DESCRIPTION:Reset and Initialize Si2164B
  Parameter:  Si2164B Context (I2C address)
  Returns:    I2C transaction error code, NO_Si2164B_ERROR if successful
************************************************************************************************************************/
signed int Si2164B_Init                (L1_Si2164B_Context *api)
{
    signed int return_code;
    SiTRACE("Si2164B_Init starting...\n");

    if ((return_code = Si2164B_PowerUpWithPatch(api)) != NO_Si2164B_ERROR) {   /* PowerUp into bootloader */
        SiTRACE ("Si2164B_PowerUpWithPatch error 0x%02x: %s\n", return_code, Si2164B_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }
    /* At this point, FW is loaded and started.  */
    Si2164B_Configure(api);
    SiTRACE("Si2164B_Init complete...\n");
    return NO_Si2164B_ERROR;
}
/************************************************************************************************************************
  Si2164B_Media function
  Use:        media retrieval function
              Used to retrieve the media used by the Si2164B
************************************************************************************************************************/
signed int Si2164B_Media               (L1_Si2164B_Context *api, signed int modulation)
{
  switch (modulation) {
    default             :
    case Si2164B_DD_MODE_PROP_MODULATION_AUTO_DETECT : {
      switch (api->prop->dd_mode.auto_detect) {
        default             : break;
        case Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_T_T2     : return Si2164B_TERRESTRIAL; break;
 /*         case Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_C_C2     : return Si2164B_TERRESTRIAL; break; */
        case Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_S_S2     :
        case Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_S_S2_DSS : return Si2164B_SATELLITE; break;
      }
      break;
    }
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT : return Si2164B_TERRESTRIAL; break;
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT2: return Si2164B_TERRESTRIAL; break;
    case Si2164B_DD_MODE_PROP_MODULATION_DVBC : return Si2164B_TERRESTRIAL; break;
    case Si2164B_DD_MODE_PROP_MODULATION_DVBC2: return Si2164B_TERRESTRIAL; break;
    case Si2164B_DD_MODE_PROP_MODULATION_MCNS : return Si2164B_TERRESTRIAL; break;

    case Si2164B_DD_MODE_PROP_MODULATION_DSS  :
    case Si2164B_DD_MODE_PROP_MODULATION_DVBS :
    case Si2164B_DD_MODE_PROP_MODULATION_DVBS2: return Si2164B_SATELLITE; break;
  }
  SiERROR("UNKNOWN media!\n");
  return 0;
}
/************************************************************************************************************************
  Si2164B_DVB_C_max_lock_ms function
  Use:        DVB-C lock time retrieval function
              Used to know how much time DVB-C lock will take in the worst case
************************************************************************************************************************/
signed int Si2164B_DVB_C_max_lock_ms   (L1_Si2164B_Context *api, signed int constellation, signed int symbol_rate_baud)
{
  signed int   swt;
  signed int   afc_khz;
  signed int   swt_coeff;
  /* To avoid division by 0, return 5000 if SR is 0 */
  if (symbol_rate_baud == 0) return 5000;
  afc_khz = api->prop->dvbc_afc_range.range_khz;
  if (afc_khz*1000 > symbol_rate_baud*11/100 ) { afc_khz = symbol_rate_baud*11/100000;}
  swt      = (signed int)(1 + 2*afc_khz*16777216/((symbol_rate_baud/1000)*(symbol_rate_baud/1000)*3));
  switch (constellation) {
    case    Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM64  :
    case    Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM16  :
    case    Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM256 : { swt_coeff =  3; break; }
    case    Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM128 : { swt_coeff =  4; break; }
    case    Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_AUTO   :
    default                                                      : { swt_coeff = 11; break; }
  }
  SiTRACE("afc_khz %3d, swt %6d, swt_coeff %2d DVB_C_max_lock_ms %d\n", afc_khz, swt, swt_coeff, (signed int)(720000000/symbol_rate_baud + swt*swt_coeff)+ 100 );
  return (signed int)(720000000/symbol_rate_baud + swt*swt_coeff)+ 100;
  /* To avoid compilation error when not compiling for DVB_C */
  constellation = symbol_rate_baud = 5000;
  return constellation;
}
/*****************************************************************************************/
/*               SiLabs demodulator API functions (demod and tuner)                      */
/*****************************************************************************************/
/* Allow profiling information during Si2164B_switch_to_standard */
#define PROFILING
/************************************************************************************************************************
  Si2164B_standardName function
  Use:        standard text retrieval function
              Used to retrieve the standard text used by the Si2164B
  Parameter:  standard, the value of the standard
************************************************************************************************************************/
const unsigned char *Si2164B_standardName (signed int standard)
{
  switch (standard)
  {
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT    : {return (unsigned char*)"DVB-T"  ;}
    case Si2164B_DD_MODE_PROP_MODULATION_MCNS    : {return (unsigned char*)"MCNS"   ;}
    case Si2164B_DD_MODE_PROP_MODULATION_DVBC    : {return (unsigned char*)"DVB-C"  ;}
    case Si2164B_DD_MODE_PROP_MODULATION_DVBC2   : {return (unsigned char*)"DVB-C2" ;}
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT2   : {return (unsigned char*)"DVB-T2" ;}
    case Si2164B_DD_MODE_PROP_MODULATION_DVBS    : {return (unsigned char*)"DVB-S"  ;}
    case Si2164B_DD_MODE_PROP_MODULATION_DVBS2   : {return (unsigned char*)"DVB-S2" ;}
    case Si2164B_DD_MODE_PROP_MODULATION_DSS     : {return (unsigned char*)"DSS"    ;}
	case Si2164B_DD_MODE_PROP_MODULATION_ANALOG  : {return (unsigned char*)"ANALOG" ;}
    default                                     : {return (unsigned char*)"UNKNOWN";}
  }
}

/* Re-definition of SiTRACE for Si2164B_L2_Context */
#ifdef    SiTRACES
  #undef  SiTRACE
  #define SiTRACE(...)        SiTraceFunction(__FILE__, __LINE__, __FUNCTION__ ,__VA_ARGS__)
#endif /* SiTRACES */

/************************************************************************************************************************
  Si2164B_L2_Infos function
  Use:        software information function
              Used to retrieve information about the compilation
  Parameter:  front_end, a pointer to the Si2164B_L2_Context context to be initialized
  Parameter:  infoString, a text buffer to be filled with the information.
              It must be initialized by the caller with a size of 1000.
  Return:     the length of the information string
************************************************************************************************************************/
signed int  Si2164B_L2_Infos              (Si2164B_L2_Context *front_end, unsigned char *infoString)
{
    if (infoString == NULL) return 0;
    if (front_end  == NULL) {
      safe_sprintf(infoString, "Si2164B front-end not initialized yet. Call Si2164B_L2_SW_Init first!\n");
      return strlen((char*)infoString);
    }

    safe_sprintf(infoString, "\n");
    safe_sprintf(infoString, "%s--------------------------------------\n", infoString);
    safe_sprintf(infoString, "%sSi_I2C               Source code %s\n"   , infoString, Si_I2C_TAG_TEXT() );
    safe_sprintf(infoString, "%sDemod                Si2164B  at 0x%02x\n", infoString , front_end->demod->i2c->address);
    safe_sprintf(infoString, "%sDemod                Source code %s\n"   , infoString , Si2164B_L1_API_TAG_TEXT() );
#ifdef    TER_TUNER_SILABS
    safe_sprintf(infoString, "%sTerrestrial tuner    SiLabs    at 0x%02x\n", infoString , front_end->tuner_ter->i2c->address);
#endif /* TER_TUNER_SILABS */
#ifdef    TER_TUNER_CUSTOMTER
    safe_sprintf(infoString, "%sTerrestrial tuner    CUSTOMTER  at 0x%02x\n", infoString , front_end->tuner_ter->i2c->address);
#endif /* TER_TUNER_CUSTOMTER */
#ifdef    TER_TUNER_NO_TER
    safe_sprintf(infoString, "%sTerrestrial tuner    NO_TER  at 0x%02x\n", infoString , front_end->tuner_ter->i2c->address);
#endif /* TER_TUNER_NO_TER */
#ifdef    TER_TUNER_TAG_TEXT
    safe_sprintf(infoString, "%sTerrestrial tuner    Source code %s\n"   , infoString , TER_TUNER_TAG_TEXT() );
#endif /* TER_TUNER_TAG_TEXT */

/* At project level, define the SAT_TUNER_xxxxxx value corresponding to the required terrestrial tuner */
#ifdef    SAT_TUNER_SILABS
    safe_sprintf(infoString, "%sSatellite tuner via SiLabs_SAT_Tuner at 0x%02x\n", infoString , front_end->tuner_sat->i2c->address);
#endif /* SAT_TUNER_SILABS */

  if ( front_end->demod->tuner_ter_clock_source == Si2164B_TER_Tuner_clock) safe_sprintf(infoString, "%sTER clock from  TER Tuner ", infoString);
  if ( front_end->demod->tuner_ter_clock_source == Si2164B_SAT_Tuner_clock) safe_sprintf(infoString, "%sTER clock from  SAT Tuner ", infoString);
  if ( front_end->demod->tuner_ter_clock_source == Si2164B_Xtal_clock     ) safe_sprintf(infoString, "%sTER clock from  Xtal      ", infoString);
                                                                           safe_sprintf(infoString, "%s(%d MHz)\n", infoString , front_end->demod->tuner_ter_clock_freq);
  if ( front_end->demod->tuner_ter_clock_input  == Si2164B_START_CLK_CMD_CLK_MODE_CLK_CLKIO  ) safe_sprintf(infoString, "%sTER clock input CLKIO\n"  , infoString);
  if ( front_end->demod->tuner_ter_clock_input  == Si2164B_START_CLK_CMD_CLK_MODE_CLK_XTAL_IN) safe_sprintf(infoString, "%sTER clock input XTAL_IN\n", infoString);
  if ( front_end->demod->tuner_ter_clock_input  == Si2164B_START_CLK_CMD_CLK_MODE_XTAL       ) safe_sprintf(infoString, "%sTER clock input XTAL\n"   , infoString);

  if (front_end->demod->fef_mode                 == Si2164B_FEF_MODE_SLOW_NORMAL_AGC          ) safe_sprintf(infoString, "%sFEF mode 'SLOW NORMAL AGC'" , infoString);
  if (front_end->demod->fef_mode                 == Si2164B_FEF_MODE_SLOW_INITIAL_AGC         ) safe_sprintf(infoString, "%sFEF mode 'SLOW INITIAL AGC'", infoString);
  if (front_end->demod->fef_mode                 == Si2164B_FEF_MODE_FREEZE_PIN               ) safe_sprintf(infoString, "%sFEF mode 'FREEZE PIN'"      , infoString);
  if (front_end->demod->fef_mode                 != front_end->demod->fef_selection          ) safe_sprintf(infoString, "%s(CHANGED!)"                 , infoString);
  safe_sprintf(infoString, "%s\n", infoString);

  if ( front_end->demod->tuner_sat_clock_source == Si2164B_TER_Tuner_clock) safe_sprintf(infoString, "%sSAT clock from  TER Tuner ", infoString);
  if ( front_end->demod->tuner_sat_clock_source == Si2164B_SAT_Tuner_clock) safe_sprintf(infoString, "%sSAT clock from  SAT Tuner ", infoString);
  if ( front_end->demod->tuner_sat_clock_source == Si2164B_Xtal_clock     ) safe_sprintf(infoString, "%sSAT clock from  Xtal      ", infoString);
                                                                           safe_sprintf(infoString, "%s(%d MHz)\n", infoString , front_end->demod->tuner_sat_clock_freq);
  if ( front_end->demod->tuner_sat_clock_input  == Si2164B_START_CLK_CMD_CLK_MODE_CLK_CLKIO  ) safe_sprintf(infoString, "%sSAT clock input CLKIO\n"  , infoString);
  if ( front_end->demod->tuner_sat_clock_input  == Si2164B_START_CLK_CMD_CLK_MODE_CLK_XTAL_IN) safe_sprintf(infoString, "%sSAT clock input XTAL_IN\n", infoString);
  if ( front_end->demod->tuner_sat_clock_input  == Si2164B_START_CLK_CMD_CLK_MODE_XTAL       ) safe_sprintf(infoString, "%sSAT clock input XTAL\n"   , infoString);
  #ifdef    UNICABLE_COMPATIBLE
  safe_sprintf(infoString, "%sCompiled with UNICABLE compatibility\n", infoString);
  if (front_end->lnb_type == UNICABLE_LNB_TYPE_UNICABLE) {
    safe_sprintf(infoString, "%sUNICABLE LNB installed\n"    , infoString);
  } else {
    safe_sprintf(infoString, "%sUNICABLE LNB not installed\n", infoString);
  }
  #endif /*  */
  //UNICABLE_COMPATIBLE

  safe_sprintf((char*)infoString, "%s--------------------------------------\n", (char*)infoString);
  return strlen((char*)infoString);
}
/************************************************************************************************************************
  Si2164B_L2_SW_Init function
  Use:        software initialization function
              Used to initialize the Si2164B and tuner structures
  Behavior:   This function performs all the steps necessary to initialize the Si2164B and tuner instances
  Parameter:  front_end, a pointer to the Si2164B_L2_Context context to be initialized
  Parameter:  demodAdd, the I2C address of the demod
  Parameter:  tunerAdd, the I2C address of the tuner
  Comments:     It MUST be called first and once before using any other function.
                It can be used to build a multi-demod/multi-tuner application, if called several times from the upper layer with different pointers and addresses
                After execution, all demod and tuner functions are accessible.
************************************************************************************************************************/
unsigned char Si2164B_L2_SW_Init            (Si2164B_L2_Context *front_end
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
                                   )
{
    unsigned char *infoString;

    SiTRACE("Si2164B_L2_EVB_SW_Init starting...\n");

    /* Pointers initialization */
    front_end->demod     = &(front_end->demodObj    );
    front_end->Si2164B_init_done    = 0;
    front_end->first_init_done     = 0;
    front_end->handshakeUsed       = 0; /* set to '0' by default for compatibility with previous versions */
    front_end->handshakeOn         = 0;
    front_end->handshakePeriod_ms  = 1000;
    front_end->tuner_ter = &(front_end->tuner_terObj);
    front_end->TER_init_done       = 0;
    front_end->TER_tuner_init_done = 0;
    front_end->auto_detect_TER     = 1;
 #ifdef    INDIRECT_I2C_CONNECTION
    front_end->f_TER_tuner_enable  = TER_tuner_enable_func;
    front_end->f_TER_tuner_disable = TER_tuner_disable_func;
 #endif /* INDIRECT_I2C_CONNECTION */
    front_end->tuner_sat = &(front_end->tuner_satObj);
    front_end->SAT_init_done       = 0;
    front_end->SAT_tuner_init_done = 0;
    front_end->previous_standard   = 0;
    front_end->auto_detect_SAT     = 1;
 #ifdef    INDIRECT_I2C_CONNECTION
    front_end->f_SAT_tuner_enable  = SAT_tuner_enable_func;
    front_end->f_SAT_tuner_disable = SAT_tuner_disable_func;
 #endif /* INDIRECT_I2C_CONNECTION */
    /* Calling underlying SW initialization functions */
    Si2164B_L1_API_Init      (front_end->demod,     demodAdd);
	// 20150611, change Main Demod ter clk configuration, x-tal 24MHz
	//if(front_end->demod->i2c->address == DEMOD_DUMMY_ADDRESS)
		//Si2164B_L2_TER_Clock(front_end,Si2164B_Xtal_clock,32,24,Si2164B_CLOCK_MANAGED);
    L1_RF_TER_TUNER_Init    (front_end->tuner_ter, tunerAdd_Ter);
    front_end->demod->fef_mode      = Si2164B_FEF_MODE_SLOW_NORMAL_AGC;
    front_end->demod->fef_selection = Si2164B_FEF_MODE;
    front_end->demod->fef_pin       = Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_MP_B;
 #ifdef    L1_RF_TER_TUNER_FEF_MODE_SLOW_INITIAL_AGC_SETUP
    /* If the TER tuner has initial AGC speed control and it's the selected mode, activate it */
    if (front_end->demod->fef_selection == Si2164B_FEF_MODE_SLOW_INITIAL_AGC) {
      SiTRACE("TER tuner FEF set to 'SLOW_INITIAL_AGC' mode\n");
      front_end->demod->fef_mode = Si2164B_FEF_MODE_SLOW_INITIAL_AGC;
    }
  #ifdef    L1_RF_TER_TUNER_FEF_MODE_FREEZE_PIN_SETUP
    /* If the TER tuner has an AGC freeze pin and it's the selected mode, activate it */
    if (front_end->demod->fef_selection == Si2164B_FEF_MODE_FREEZE_PIN      ) {
      SiTRACE("TER tuner FEF set to 'FREEZE_PIN' mode\n");
      front_end->demod->fef_mode = Si2164B_FEF_MODE_FREEZE_PIN;
    }
  #else  /* L1_RF_TER_TUNER_FEF_MODE_FREEZE_PIN_SETUP */
    if (front_end->demod->fef_selection == Si2164B_FEF_MODE_FREEZE_PIN      ) {
      SiTRACE("TER tuner FEF can not use 'FREEZE_PIN' mode, using 'SLOW_INITIAL_AGC' mode instead\n");
      front_end->demod->fef_mode = Si2164B_FEF_MODE_SLOW_INITIAL_AGC;
    }
  #endif /* L1_RF_TER_TUNER_FEF_MODE_FREEZE_PIN_SETUP */
 #else  /* L1_RF_TER_TUNER_FEF_MODE_SLOW_INITIAL_AGC_SETUP */
      SiTRACE("TER tuner FEF set to 'SLOW_NORMAL_AGC' mode\n");
 #endif /* L1_RF_TER_TUNER_FEF_MODE_SLOW_INITIAL_AGC_SETUP */
    L1_RF_SAT_TUNER_Init    (front_end->tuner_sat, tunerAdd_Sat);

	// 20150611, change Main Demod sat clk configuration, x-tal 24MHz
	//if(front_end->demod->i2c->address == DEMOD_DUMMY_ADDRESS)
		Si2164B_L2_SAT_Clock(front_end,Si2164B_Xtal_clock,32,24,Si2164B_CLOCK_MANAGED);
	//else
	//	Si2164B_L2_SAT_Clock(front_end,Si2164B_TER_Tuner_clock,44,24,Si2164B_CLOCK_MANAGED);
	//end
    //front_end->satellite_spectrum_inversion = 0;//csm,disable
	//model specific setting,csm,add
	if((front_end->tuner_sat->i2c->address == SAT1_MAIN_ADDRESS) || (front_end->tuner_sat->i2c->address == SAT1_SUB_ADDRESS))//5816SW
		front_end->demod->prop->dd_mode.invert_spectrum = Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_NORMAL;
	else if(front_end->tuner_sat->i2c->address == SAT2_ADDRESS)//5815M
		front_end->demod->prop->dd_mode.invert_spectrum = Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_INVERTED;
	//end
#ifdef    UNICABLE_COMPATIBLE
    front_end->lnb_type        = UNICABLE_LNB_TYPE_NORMAL;
    front_end->unicable_spectrum_inversion = 1;
    front_end->f_tuner_tune    = tuner_tune;
    front_end->f_Unicable_tune = unicable_tune;
#endif /* UNICABLE_COMPATIBLE */
    front_end->callback        = p_context;
#ifdef    SiTRACE
	infoString = malloc(sizeof(unsigned char)*1000);
	if(infoString != 0)
	{
		if (Si2164B_L2_Infos(front_end, infoString))  {SiTRACE("%s\n", infoString);}
		free(infoString);
	}
	else
	{	SiTRACE("Si2164B_L2_EVB_SW_Init : fail to create buffer to print log.\n");
    }
#endif /* SiTRACE */
    front_end->demod->nbLines = 0;
    front_end->demod->fw_table = NULL;
    front_end->demod->nbSpiBytes = 0;
    front_end->demod->spi_table = NULL;
    front_end->demod->spi_buffer_size = 2048;  /* Needs to be at least 1024 for the current FW portions. Contact SiLabs in case you need a smaller size */
    /* <porting> if not allowed to use dynamic memory allocation, comment the following lines. */
    front_end->demod->fw_table = (firmware_struct*)realloc(front_end->demod->fw_table, sizeof(firmware_struct)*front_end->demod->nbLines);
    front_end->demod->spi_table = (unsigned char *)realloc(front_end->demod->spi_table, sizeof(unsigned char)*front_end->demod->nbSpiBytes);
    /* <porting> end of dynamic memory allocation */
    SiTRACE("Si2164B_L2_EVB_SW_Init complete\n");
    return 1;
}
/************************************************************************************************************************
  Si2164B_L2_Set_Index_and_Tag function
  Use:        Front-End index and tag function
              Used to store the frontend index and tag, which will be used in traces called from L2 level
              Behavior:   It will store the front_end index and tag in the Si2164B_L2_Context demod->i2c, adding 'DTV' to the tag
              Behavior:   It will store the front_end index and tag in the Si2164B_L2_Context tuner_ter->i2c, adding 'TER' to the tag
              Behavior:   It will store the front_end index and tag in the Si2164B_L2_Context tuner_sat->i2c, adding 'SAT' to the tag
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  index, the frontend index
  Parameter:  tag,   the frontend tag (a string limited to SILABS_TAG_SIZE characters)
  Return:     the length of the demod->i2c->tag
************************************************************************************************************************/
#if 0 // 20140513 del jhkwon
signed int  Si2164B_L2_Set_Index_and_Tag  (Si2164B_L2_Context *front_end, unsigned char index, unsigned char* tag)
{
  front_end->demod->i2c->tag_index = index;
  //snprintf(front_end->demod->i2c->tag,     SILABS_TAG_SIZE, "%s DTV", tag);
  //snprintf(front_end->tuner_ter->i2c->tag, SILABS_TAG_SIZE, "%s TER", tag);
  //snprintf(front_end->tuner_sat->i2c->tag, SILABS_TAG_SIZE, "%s SAT", tag);
  return strlen(front_end->demod->i2c->tag);
}
#endif
/************************************************************************************************************************
  NAME: Si2164B_L2_SILENT
  DESCRIPTION: Turning Demod pins tristate, to allow using another demod
  Parameter:  Pointer to Si2164B Context
  Parameter:  silent, a flag used to select the result: '1' -> go silent, '0' -> return to active mode
  Returns:    I2C transaction error code, NO_Si2164B_ERROR if successful
************************************************************************************************************************/
signed int Si2164B_L2_SILENT              (Si2164B_L2_Context *front_end, signed int silent)
{
  SiTRACE ("Si2164B_L2_SILENT: silent %d\n", silent);
  if (silent) {
    /* turn all possible I/Os to tristate, to allow using another demod */
    /* AGC settings when not used */
    if        ( front_end->demod->rsp->get_rev.mcm_die == Si2164B_GET_REV_RESPONSE_MCM_DIE_DIE_A) {
      Si2164B_L1_DD_MP_DEFAULTS(front_end->demod, Si2164B_DD_MP_DEFAULTS_CMD_MP_A_MODE_DISABLE  , Si2164B_DD_MP_DEFAULTS_CMD_MP_B_MODE_NO_CHANGE
                                               , Si2164B_DD_MP_DEFAULTS_CMD_MP_C_MODE_DISABLE  , Si2164B_DD_MP_DEFAULTS_CMD_MP_D_MODE_NO_CHANGE);
    } else if ( front_end->demod->rsp->get_rev.mcm_die == Si2164B_GET_REV_RESPONSE_MCM_DIE_DIE_B) {
      Si2164B_L1_DD_MP_DEFAULTS(front_end->demod, Si2164B_DD_MP_DEFAULTS_CMD_MP_A_MODE_NO_CHANGE, Si2164B_DD_MP_DEFAULTS_CMD_MP_B_MODE_DISABLE
                                               , Si2164B_DD_MP_DEFAULTS_CMD_MP_C_MODE_NO_CHANGE, Si2164B_DD_MP_DEFAULTS_CMD_MP_D_MODE_DISABLE);
    } else {
      Si2164B_L1_DD_MP_DEFAULTS(front_end->demod, Si2164B_DD_MP_DEFAULTS_CMD_MP_A_MODE_DISABLE  , Si2164B_DD_MP_DEFAULTS_CMD_MP_B_MODE_DISABLE
                                               , Si2164B_DD_MP_DEFAULTS_CMD_MP_C_MODE_DISABLE  , Si2164B_DD_MP_DEFAULTS_CMD_MP_D_MODE_DISABLE);
    }
    Si2164B_L1_DD_EXT_AGC_TER(front_end->demod, Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_NOT_USED,     front_end->demod->cmd->dd_ext_agc_ter.agc_1_inv
                                             , Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MODE_NOT_USED,     front_end->demod->cmd->dd_ext_agc_ter.agc_2_inv
                                             , front_end->demod->cmd->dd_ext_agc_ter.agc_1_kloop, front_end->demod->cmd->dd_ext_agc_ter.agc_2_kloop
                                             , front_end->demod->cmd->dd_ext_agc_ter.agc_1_min  , front_end->demod->cmd->dd_ext_agc_ter.agc_2_min);
    front_end->TER_init_done = 0;
    Si2164B_L1_DD_EXT_AGC_SAT(front_end->demod, Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MODE_NOT_USED,     front_end->demod->cmd->dd_ext_agc_sat.agc_1_inv
                                             , Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MODE_NOT_USED,     front_end->demod->cmd->dd_ext_agc_sat.agc_2_inv
                                             , front_end->demod->cmd->dd_ext_agc_sat.agc_1_kloop, front_end->demod->cmd->dd_ext_agc_sat.agc_2_kloop
                                             , front_end->demod->cmd->dd_ext_agc_sat.agc_1_min  , front_end->demod->cmd->dd_ext_agc_sat.agc_2_min);
    front_end->SAT_init_done = 0;
    if        ( front_end->demod->rsp->get_rev.mcm_die == Si2164B_GET_REV_RESPONSE_MCM_DIE_DIE_A) {
      Si2164B_L1_CONFIG_PINS   (front_end->demod, Si2164B_CONFIG_PINS_CMD_GPIO0_MODE_NO_CHANGE, Si2164B_CONFIG_PINS_CMD_GPIO0_READ_DO_NOT_READ
                                               , Si2164B_CONFIG_PINS_CMD_GPIO1_MODE_DISABLE  , Si2164B_CONFIG_PINS_CMD_GPIO1_READ_DO_NOT_READ);
    } else if ( front_end->demod->rsp->get_rev.mcm_die == Si2164B_GET_REV_RESPONSE_MCM_DIE_DIE_B) {
      Si2164B_L1_CONFIG_PINS   (front_end->demod, Si2164B_CONFIG_PINS_CMD_GPIO0_MODE_DISABLE  , Si2164B_CONFIG_PINS_CMD_GPIO0_READ_DO_NOT_READ
                                               , Si2164B_CONFIG_PINS_CMD_GPIO1_MODE_NO_CHANGE, Si2164B_CONFIG_PINS_CMD_GPIO1_READ_DO_NOT_READ);
    } else {
      Si2164B_L1_CONFIG_PINS   (front_end->demod, Si2164B_CONFIG_PINS_CMD_GPIO0_MODE_DISABLE  , Si2164B_CONFIG_PINS_CMD_GPIO0_READ_DO_NOT_READ
                                               , Si2164B_CONFIG_PINS_CMD_GPIO1_MODE_DISABLE  , Si2164B_CONFIG_PINS_CMD_GPIO1_READ_DO_NOT_READ);
    }
    /* turn TS pins to tristate */
    front_end->demod->prop->dd_ts_mode.mode =  Si2164B_DD_TS_MODE_PROP_MODE_TRISTATE;
    Si2164B_L1_SetProperty2  (front_end->demod, Si2164B_DD_TS_MODE_PROP_CODE);
    Si2164B_L1_DD_RESTART    (front_end->demod);
	if(front_end->demod->i2c->address != 0x00)
		system_wait(8); /* Wait at least 8 ms before issuing POWER_DOWN, to make sure that the AGCs are in tristate. */
  } else {
    /* return to 'active mode' (it will be necessary to turn TS on again after locking) */
    Si2164B_WAKEUP        (front_end->demod);
  }
  return silent;
}
/************************************************************************************************************************
  Si2164B_L2_HW_Connect function
  Use:        Front-End connection function
              Specific to SiLabs USB connection!
  Porting:    Remove or replace by the final application corresponding calls
  Behavior:   This function connects the Si2164B, and the tuners via the Cypress chip
  Parameter:  *front_end, the front-end handle
  Parameter   mode, the required connection mode
************************************************************************************************************************/
void Si2164B_L2_HW_Connect         (Si2164B_L2_Context *front_end, CONNECTION_TYPE mode)
{
    L0_Connect(front_end->demod->i2c,    mode);
    L0_Connect(front_end->tuner_ter->i2c,mode);
    L0_Connect(front_end->tuner_sat->i2c,mode);
}
/************************************************************************************************************************
  NAME: Si2164B_L2_Part_Check
  DESCRIPTION:startup and part checking Si2164B
  Parameter:  Si2164B Context
  Returns:    the part_info 'part' field value
************************************************************************************************************************/
signed int Si2164B_L2_Part_Check          (Si2164B_L2_Context *front_end)
{
  signed int start_time_ms;
  front_end->demod->rsp->part_info.part = 0;
  start_time_ms = system_time();
  front_end->demod->cmd->power_up.reset = Si2164B_POWER_UP_CMD_RESET_RESET;
  front_end->demod->cmd->power_up.func  = Si2164B_POWER_UP_CMD_FUNC_BOOTLOADER;
#ifdef    SiTRACES
    SiTraceConfiguration("traces suspend");
#endif /* SiTRACES */
  Si2164B_WAKEUP      (front_end->demod);
  Si2164B_L1_PART_INFO(front_end->demod);
#ifdef    SiTRACES
  SiTraceConfiguration("traces resume");
#endif /* SiTRACES */
  SiTRACE("Si2164B_Part_Check took %3d ms. Part is Si21%2d\n", system_time() - start_time_ms, front_end->demod->rsp->part_info.part );
  return front_end->demod->rsp->part_info.part;
}
/************************************************************************************************************************
  Si2164B_L2_Tuner_I2C_Enable function
  Use:        Tuner i2c bus connection
              Used to allow communication with the tuners
  Parameter:  *front_end, the front-end handle
************************************************************************************************************************/
unsigned char Si2164B_L2_Tuner_I2C_Enable (Si2164B_L2_Context *front_end)
{
    return Si2164B_L1_I2C_PASSTHROUGH(front_end->demod, Si2164B_I2C_PASSTHROUGH_CMD_SUBCODE_CODE, Si2164B_I2C_PASSTHROUGH_CMD_I2C_PASSTHRU_CLOSE, Si2164B_I2C_PASSTHROUGH_CMD_RESERVED_RESERVED);
}
/************************************************************************************************************************
  Si2164B_L2_Tuner_I2C_Disable function
  Use:        Tuner i2c bus connection
              Used to disconnect i2c communication with the tuners
  Parameter:  *front_end, the front-end handle
************************************************************************************************************************/
unsigned char Si2164B_L2_Tuner_I2C_Disable(Si2164B_L2_Context *front_end)
{
	unsigned char ret = 0;

    ret = Si2164B_L1_I2C_PASSTHROUGH(front_end->demod, Si2164B_I2C_PASSTHROUGH_CMD_SUBCODE_CODE, Si2164B_I2C_PASSTHROUGH_CMD_I2C_PASSTHRU_OPEN, Si2164B_I2C_PASSTHROUGH_CMD_RESERVED_RESERVED);

	return ret;
}
/************************************************************************************************************************
  Si2164B_L2_communication_check function
  Use:        Si2164B front i2c bus connection check
              Used to check i2c communication with the demod and the tuners
  Parameter:  *front_end, the front-end handle
************************************************************************************************************************/
signed int  Si2164B_L2_communication_check(Si2164B_L2_Context *front_end)
{
  signed int comm_errors;
  comm_errors=0;
  /* Close i2c Passthru       */
  Si2164B_L2_Tuner_I2C_Enable(front_end);
  /* Check i2c Passthru value */
  if ((signed int)Si2164B_L1_CheckStatus(front_end->demod) != NO_Si2164B_ERROR) {
    SiTRACE ("DEMOD Communication error ! \n");
    comm_errors++;
  } else {
    SiTRACE ("DEMOD Communication OK\n");
  }
 #ifdef    INDIRECT_I2C_CONNECTION
  front_end->f_TER_tuner_enable(front_end->callback);
 #endif /* INDIRECT_I2C_CONNECTION */
  /* Check TER tuner read     */
    #ifdef    TER_TUNER_COMM_CHECK
  if (TER_TUNER_COMM_CHECK(front_end->tuner_ter) !=1) {
    SiTRACE ("TER tuner Communication error ! \n");
    comm_errors++;
  } else {
    SiTRACE ("TER tuner Communication OK\n");
  }
    #endif /* TER_TUNER_COMM_CHECK */

 #ifdef    INDIRECT_I2C_CONNECTION
  front_end->f_SAT_tuner_enable(front_end->callback);
 #endif /* INDIRECT_I2C_CONNECTION */
  /* Check SAT tuner read     */
    #ifdef    L1_RF_SAT_TUNER_COMM_CHECK
  if (L1_RF_SAT_TUNER_COMM_CHECK(front_end->tuner_sat) !=1) {
    SiTRACE ("SAT tuner Communication error ! \n");
    comm_errors++;
    comm_errors--; /* This is a trick to avoid returning an error and thus stopping the standard switching */
                   /* It needs to be removed once the RDA5815 can be read back                             */
  } else {
    SiTRACE ("SAT tuner Communication OK\n");
  }
    #endif /* L1_RF_SAT_TUNER_COMM_CHECK */

  /* Open  i2c Passthru       */
 #ifdef    INDIRECT_I2C_CONNECTION
  front_end->f_TER_tuner_disable(front_end->callback);
  front_end->f_SAT_tuner_disable(front_end->callback);
 #endif /* INDIRECT_I2C_CONNECTION */
  Si2164B_L2_Tuner_I2C_Disable(front_end);

  if (comm_errors) return 0;

  return 1;
}
/************************************************************************************************************************
  Si2164B_L2_switch_to_standard function
  Use:      Standard switching function selection
            Used to switch nicely to the wanted standard, taking into account the previous state
  Parameter: new_standard the wanted standard to switch to
  Behavior: This function positions a set of flags to easily decide what needs to be done to
              switch between standards.
************************************************************************************************************************/
signed int  Si2164B_L2_switch_to_standard (Si2164B_L2_Context *front_end, unsigned char new_standard, unsigned char force_full_init)
{
  /* previous state flags */
  signed int dtv_demod_already_used = 0;
  #ifdef    INDIRECT_I2C_CONNECTION
  signed int ter_i2c_connected      = 0;
  signed int sat_i2c_connected      = 0;
  #else  /* INDIRECT_I2C_CONNECTION */
  signed int i2c_connected          = 0;
  #endif /* INDIRECT_I2C_CONNECTION */
  signed int ter_tuner_already_used = 0;
  signed int sat_tuner_already_used = 0;
  signed int ter_clock_already_used = 0;
  signed int sat_clock_already_used = 0;
  /* new state flags      */
  signed int dtv_demod_needed       = 0;
  signed int ter_tuner_needed       = 0;
  signed int sat_tuner_needed       = 0;
  signed int ter_clock_needed       = 0;
  signed int sat_clock_needed       = 0;
  signed int dtv_demod_sleep_request= 0;
  signed int dtv_demod_sleeping     = 0;
  signed int res;

#ifdef    PROFILING
  signed int start;
  signed int ter_tuner_delay   = 0;
  signed int sat_tuner_delay   = 0;
  signed int dtv_demod_delay   = 0;
  signed int switch_start;
  unsigned char *sequence;
  #define TER_DELAY  ter_tuner_delay=ter_tuner_delay+system_time()-start;start=system_time();
  #define SAT_DELAY  sat_tuner_delay=sat_tuner_delay+system_time()-start;start=system_time();
  #define DTV_DELAY  dtv_demod_delay=dtv_demod_delay+system_time()-start;start=system_time();
#else
  #define TER_DELAY
  #define SAT_DELAY
  #define DTV_DELAY
#endif /* PROFILING */

#ifdef    PROFILING
  start = switch_start = system_time();
  SiTRACE("%s->%s\n", Si2164B_standardName(front_end->previous_standard), Si2164B_standardName(new_standard) );
#endif /* PROFILING */

  SiTRACE("Si2164B_switch_to_standard starting...\n");
#ifndef   ORIGINAL_Si2164B_init_done
  if (front_end->external_fw_download == 1) {
    front_end->Si2164B_init_done = 1;//20131231,cold booting: set all flags before using them
    SiTRACE("Demod init already done outside switch_to_standard\n");
  }
#endif /* ORIGINAL_Si2164B_init_done:  This needs to be moved at the beginning of the function */
  SiTRACE(" %s-->%s switch starting with Si2164B_init_done %d, first_init_done %d\n", Si2164B_standardName(front_end->previous_standard), Si2164B_standardName(new_standard), front_end->Si2164B_init_done, front_end->first_init_done);
  SiTRACE("TER flags:    TER_init_done    %d, TER_tuner_init_done %d\n", front_end->TER_init_done, front_end->TER_tuner_init_done);
  SiTRACE("SAT flags:    SAT_init_done    %d, SAT_tuner_init_done %d\n", front_end->SAT_init_done, front_end->SAT_tuner_init_done);

  /* In this function is called for the first time, force a full init */
  /* When INIT1 is used, don't allow this feature (to make flags management easier) */
  if (front_end->external_fw_download == 0) {
	if (front_end->first_init_done == 0) {force_full_init = 1;}
  }
  /* ------------------------------------------------------------ */
  /* Set Previous Flags                                           */
  /* Setting flags representing the previous state                */
  /* NB: Any value not matching a known standard will init as ATV */
  /* Logic applied:                                               */
  /*  dtv demod was used for TERRESTRIAL and SATELLITE reception  */
  /*  ter tuner was used for TERRESTRIAL reception                */
  /*   and for SATELLITE reception if it is the SAT clock source  */
  /*  sat tuner was used for SATELLITE reception                  */
  /*   and for TERRESTRIAL reception if it is the TER clock source*/
  /* ------------------------------------------------------------ */
  switch (front_end->previous_standard) {
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT :
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT2:
    case Si2164B_DD_MODE_PROP_MODULATION_MCNS :
    case Si2164B_DD_MODE_PROP_MODULATION_DVBC : {
      dtv_demod_already_used = 1;
      ter_tuner_already_used = 1;
      if ( front_end->demod->tuner_ter_clock_source == Si2164B_TER_Tuner_clock) {
        ter_clock_already_used = 1;
      }
      if ( front_end->demod->tuner_ter_clock_source == Si2164B_SAT_Tuner_clock) {
        sat_tuner_already_used = 1;
        sat_clock_already_used = 1;
      }
      break;
    }
    case Si2164B_DD_MODE_PROP_MODULATION_DVBC2: {
      dtv_demod_already_used = 1;
      ter_tuner_already_used = 1;
      if ( front_end->demod->tuner_ter_clock_source == Si2164B_TER_Tuner_clock) {
        ter_clock_already_used = 1;
      }
      if ( front_end->demod->tuner_ter_clock_source == Si2164B_SAT_Tuner_clock) {
        sat_tuner_already_used = 1;
        sat_clock_already_used = 1;
      }
      break;
    }
    case Si2164B_DD_MODE_PROP_MODULATION_DVBS  :
    case Si2164B_DD_MODE_PROP_MODULATION_DVBS2 :
    case Si2164B_DD_MODE_PROP_MODULATION_DSS   : {
      dtv_demod_already_used = 1;
      sat_tuner_already_used = 1;
      if ( front_end->demod->tuner_sat_clock_source == Si2164B_TER_Tuner_clock) {
        ter_tuner_already_used = 1;
        ter_clock_already_used = 1;
      }
      if ( front_end->demod->tuner_sat_clock_source == Si2164B_SAT_Tuner_clock) {
        sat_clock_already_used = 1;
      }
      break;
    }
    case Si2164B_DD_MODE_PROP_MODULATION_ANALOG: {
      ter_tuner_already_used = 1;
      dtv_demod_sleeping = 1;
      break;
    }
    /* Special case for external fw download:
        following external FW download,
        the TER tuner is on and its clock is active */
	case 0:{
	  if(front_end->external_rf_fw_download == 1){//if(front_end->external_fw_download == 1){
	    ter_clock_already_used = 1;
		//ter_tuner_already_used = 1;//20131203,disable to call 'Si2151_Configure'
	  }
	  break;
	}
    default : /* SLEEP */   {
      ter_tuner_already_used = 0;
      dtv_demod_sleeping = 1;
      break;
    }
  }

  /* ------------------------------------------------------------ */
  /* Set Needed Flags                                             */
  /* Setting flags representing the new state                     */
  /* Logic applied:                                               */
  /*  dtv demod is needed for TERRESTRIAL and SATELLITE reception */
  /*  ter tuner is needed for TERRESTRIAL reception               */
  /*   and for SATELLITE reception if it is the SAT clock source  */
  /*  sat tuner is needed for SATELLITE reception                 */
  /*   and for TERRESTRIAL reception if it is the TER clock source*/
  /* ------------------------------------------------------------ */
  switch (new_standard) {
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT :
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT2:
    case Si2164B_DD_MODE_PROP_MODULATION_MCNS :
    case Si2164B_DD_MODE_PROP_MODULATION_DVBC : {
      dtv_demod_needed = 1;
      ter_tuner_needed = 1;
      if ( front_end->demod->tuner_ter_clock_source == Si2164B_TER_Tuner_clock) {
        ter_clock_needed = 1;
      }
      if ( front_end->demod->tuner_ter_clock_source == Si2164B_SAT_Tuner_clock) {
        sat_clock_needed = 1;
        sat_tuner_needed = 1;
      }
      break;
    }
    case Si2164B_DD_MODE_PROP_MODULATION_DVBC2: {
      dtv_demod_needed = 1;
      ter_tuner_needed = 1;
      if ( front_end->demod->tuner_ter_clock_source == Si2164B_TER_Tuner_clock) {
        ter_clock_needed = 1;
      }
      if ( front_end->demod->tuner_ter_clock_source == Si2164B_SAT_Tuner_clock) {
        sat_tuner_needed = 1;
        sat_clock_needed = 1;
      }
      break;
    }
    case Si2164B_DD_MODE_PROP_MODULATION_DVBS :
    case Si2164B_DD_MODE_PROP_MODULATION_DVBS2:
    case Si2164B_DD_MODE_PROP_MODULATION_DSS  : {
      dtv_demod_needed = 1;
      sat_tuner_needed = 1;
      if ( front_end->demod->tuner_sat_clock_source == Si2164B_TER_Tuner_clock) {
        ter_clock_needed = 1;
        ter_tuner_needed = 1;
      }
      if ( front_end->demod->tuner_sat_clock_source == Si2164B_SAT_Tuner_clock) {
        sat_clock_needed = 1;
      }
      break;
    }
    case Si2164B_DD_MODE_PROP_MODULATION_ANALOG: {
      ter_tuner_needed = 1;
      break;
    }
    default : /* SLEEP */   {
      ter_tuner_needed = 0;
      break;
    }
  }

  /* ------------------------------------------------------------ */
  /* For multiple front-ends: override clock_needed flags         */
  /*  to avoid switching shared clocks                            */
  /* ------------------------------------------------------------ */
  /* For multiple front-ends: override clock_needed flags to avoid switching shared clocks */
  if (ter_clock_already_used == 0) {
    if (front_end->TER_tuner_init_done == 1) {
      if ( front_end->demod->tuner_ter_clock_control == Si2164B_CLOCK_ALWAYS_ON ) { SiTRACE("forcing ter_clock_already_used = 1\n"); ter_clock_already_used = 1; }
    }
  } else {
    if ( front_end->demod->tuner_ter_clock_control == Si2164B_CLOCK_ALWAYS_OFF  ) { SiTRACE("forcing ter_clock_already_used = 0\n"); ter_clock_already_used = 0; }
  }
  if (ter_clock_needed == 0) {
    if ( front_end->demod->tuner_ter_clock_control == Si2164B_CLOCK_ALWAYS_ON   ) { SiTRACE("forcing ter_clock_needed = 1\n"); ter_clock_needed = 1; }
  } else {
    if ( front_end->demod->tuner_ter_clock_control == Si2164B_CLOCK_ALWAYS_OFF  ) { SiTRACE("forcing ter_clock_needed = 0\n"); ter_clock_needed = 0; }
  }
  if (sat_clock_already_used == 0) {
    if (front_end->SAT_tuner_init_done == 1) {
      if ( front_end->demod->tuner_sat_clock_control == Si2164B_CLOCK_ALWAYS_ON ) { SiTRACE("forcing sat_clock_already_used = 1\n"); sat_clock_already_used = 1; }
    }
  } else {
    if ( front_end->demod->tuner_sat_clock_control == Si2164B_CLOCK_ALWAYS_OFF  ) { SiTRACE("forcing sat_clock_already_used = 0\n"); sat_clock_already_used = 0; }
  }
  if (sat_clock_needed == 0) {
    if ( front_end->demod->tuner_sat_clock_control == Si2164B_CLOCK_ALWAYS_ON   ) { SiTRACE("forcing sat_clock_needed = 1\n"); sat_clock_needed = 1; }
  } else {
    if ( front_end->demod->tuner_sat_clock_control == Si2164B_CLOCK_ALWAYS_OFF  ) { SiTRACE("forcing sat_clock_needed = 0\n"); sat_clock_needed = 0; }
  }

  /* ------------------------------------------------------------ */
  /* if 'force' flag is set, set flags to trigger a full init     */
  /* This can be used to re-init the NIM after a power cycle      */
  /*  or a HW reset                                               */
  /* ------------------------------------------------------------ */
  if (force_full_init) {
    SiTRACE("Forcing full init\n");
    /* set 'init_done' flags to force full init     */
    front_end->first_init_done     = 0;
    front_end->Si2164B_init_done    = 0;
    front_end->TER_init_done       = 0;
    front_end->TER_tuner_init_done = 0;
    front_end->SAT_init_done       = 0;
    front_end->SAT_tuner_init_done = 0;
    /* set 'already used' flags to force full init  */
    ter_tuner_already_used = 0;
    sat_tuner_already_used = 0;
    dtv_demod_already_used = 0;
    /* set 'needed' flags to force tuner init if required by the clock control flags */
    if ( front_end->demod->tuner_ter_clock_control == Si2164B_CLOCK_ALWAYS_ON ) { SiTRACE("forcing ter_tuner_needed = 1\n"); ter_tuner_needed = 1; }
    if ( front_end->demod->tuner_sat_clock_control == Si2164B_CLOCK_ALWAYS_ON ) { SiTRACE("forcing sat_tuner_needed = 1\n"); sat_tuner_needed = 1; }
  }

  /* ------------------------------------------------------------ */
  /* Request demodulator sleep if its clock will be stopped       */
  /* ------------------------------------------------------------ */
  SiTRACE("ter_tuner_already_used %d, ter_tuner_needed %d\n", ter_tuner_already_used,ter_tuner_needed);
  SiTRACE("ter_clock_already_used %d, ter_clock_needed %d\n", ter_clock_already_used,ter_clock_needed);
  if ((ter_tuner_already_used == 1) & (ter_tuner_needed == 0) ) { SiTRACE("TER tuner 1->0 "); }
  if ((ter_tuner_already_used == 0) & (ter_tuner_needed == 1) ) { SiTRACE("TER tuner 0->1 "); }
  if ((ter_clock_already_used == 1) & (ter_clock_needed == 0) ) { SiTRACE("TER clock 1->0 "); dtv_demod_sleep_request = 1; }
  if ((ter_clock_already_used == 0) & (ter_clock_needed == 1) ) { SiTRACE("TER clock 0->1 "); dtv_demod_sleep_request = 1; }
  SiTRACE("sat_tuner_already_used %d, sat_tuner_needed %d\n", sat_tuner_already_used,sat_tuner_needed);
  SiTRACE("sat_clock_already_used %d, sat_clock_needed %d\n", sat_clock_already_used,sat_clock_needed);
  if ((sat_tuner_already_used == 1) & (sat_tuner_needed == 0) ) { SiTRACE("SAT tuner 1->0 "); }
  if ((sat_tuner_already_used == 0) & (sat_tuner_needed == 1) ) { SiTRACE("SAT tuner 0->1 "); }
  if ((sat_clock_already_used == 1) & (sat_clock_needed == 0) ) { SiTRACE("SAT clock 1->0 "); dtv_demod_sleep_request = 1; }
  if ((sat_clock_already_used == 0) & (sat_clock_needed == 1) ) { SiTRACE("SAT clock 0->1 "); dtv_demod_sleep_request = 1; }
  SiTRACE("\n");
  /* ------------------------------------------------------------ */
  /* Request demodulator sleep if transition from '1' to '0'      */
  /* ------------------------------------------------------------ */
  if ((dtv_demod_already_used == 1) & (dtv_demod_needed == 0) ) { dtv_demod_sleep_request = 1; }
  SiTRACE(" %s-->%s switch flags    dtv_demod_already_used %d, dtv_demod_needed %d, dtv_demod_sleep_request %d, dtv_demod_sleeping %d\n", Si2164B_standardName(front_end->previous_standard), Si2164B_standardName(new_standard),
                                 dtv_demod_already_used,    dtv_demod_needed,    dtv_demod_sleep_request,    dtv_demod_sleeping   );
  /* ------------------------------------------------------------ */
  /* Sleep dtv demodulator if requested                           */
  /* ------------------------------------------------------------ */
  if (dtv_demod_sleep_request == 1) {
    SiTRACE("Sleep DTV demod\n");
    /* To avoid issues with the FEF pin when switching from T2 to ANALOG, set the demodulator for DVB-T/non auto detect reception before POWER_DOWN */
    if (new_standard                       == Si2164B_DD_MODE_PROP_MODULATION_ANALOG) {
      if ( ( (front_end->previous_standard == Si2164B_DD_MODE_PROP_MODULATION_DVBT  )
           & (front_end->demod->prop->dd_mode.auto_detect == Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_T_T2) )
           | (front_end->previous_standard == Si2164B_DD_MODE_PROP_MODULATION_DVBT2 ) ) {
        front_end->demod->prop->dd_mode.modulation  = Si2164B_DD_MODE_PROP_MODULATION_DVBT;
        front_end->demod->prop->dd_mode.auto_detect = Si2164B_DD_MODE_PROP_AUTO_DETECT_NONE;
        Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DD_MODE_PROP_CODE);
        Si2164B_L1_DD_RESTART  (front_end->demod);
      }
    }
    /* If the demod is not needed, it means it's either going to ANALOG or SLEEP   */
    /*  In this case, set the demod silent, putting all possible pins to tristate  */
    /* To nicely recover from this, Si2164B_L2_Configure will need to be re-applied */
    /*  so set the flags to allow this                                             */
    if (dtv_demod_needed == 0) {
      Si2164B_L2_SILENT(front_end, 1);
    }
    Si2164B_STANDBY (front_end->demod);
    dtv_demod_sleeping = 1;
    SiTRACE(" %s-->%s switch now   dtv_demod_sleeping %d\n", Si2164B_standardName(front_end->previous_standard), Si2164B_standardName(new_standard), dtv_demod_sleeping   );
    DTV_DELAY
  }

  /* ------------------------------------------------------------ */
  /* Set media for new standard                                   */
  /* ------------------------------------------------------------ */
  if (dtv_demod_needed == 1) {
    front_end->demod->prop->dd_mode.modulation = new_standard;
    front_end->demod->media = Si2164B_Media(front_end->demod, front_end->demod->prop->dd_mode.modulation);
  }

  /* ------------------------------------------------------------ */
  /* Allow i2c traffic to reach the tuners                        */
  /* ------------------------------------------------------------ */
   #ifdef    INDIRECT_I2C_CONNECTION
   /* Connection will be done later on, depending on TER/SAT */
   #else  /* INDIRECT_I2C_CONNECTION */
    SiTRACE("Connect tuners i2c\n");
    Si2164B_L2_Tuner_I2C_Enable(front_end);
    DTV_DELAY
    i2c_connected = 1;
   #endif /* INDIRECT_I2C_CONNECTION */

  /* ------------------------------------------------------------ */
  /* Sleep Sat Tuner                                              */
  /* Sleep satellite   tuner if transition from '1' to '0'        */
  /* ------------------------------------------------------------ */
  if ((sat_tuner_already_used == 1) & (sat_tuner_needed == 0) ) {
   #ifdef    INDIRECT_I2C_CONNECTION
    SiTRACE("Connect SAT tuner i2c to put it in sleep mode?\n");
    if (sat_i2c_connected==0) {
      SiTRACE("-- I2C -- Connect SAT tuner i2c to put it in sleep mode\n");
      front_end->f_SAT_tuner_enable(front_end->callback);
      sat_i2c_connected++;
    }
    DTV_DELAY
   #endif /* INDIRECT_I2C_CONNECTION */
    SiTRACE("Sleep satellite tuner\n");
    #ifdef    SAT_TUNER_CLOCK_OFF
    if (front_end->demod->tuner_sat_clock_control != Si2164B_CLOCK_ALWAYS_ON) {
		if ( (res = SAT_TUNER_CLOCK_OFF(front_end->tuner_sat)) !=0 ) {
		  SiTRACE("Satellite tuner CLOCK OFF error: 0x%02x\n",res );
		  //csm,add
		  SiTRACE("Disconnect tuners i2c\n");
		  Si2164B_L2_Tuner_I2C_Disable(front_end);
		  DTV_DELAY
		  //end
		  return 0;
		}
    }
    #endif /* SAT_TUNER_CLOCK_OFF */
    #ifdef    L1_RF_SAT_TUNER_STANDBY
    if ((res= L1_RF_SAT_TUNER_STANDBY(front_end->tuner_sat)) !=0 ) {
      SiTRACE("Satellite tuner Standby error: 0x%02x\n",res );
	  //csm,add
	  SiTRACE("Disconnect tuners i2c\n");
	  Si2164B_L2_Tuner_I2C_Disable(front_end);
	  DTV_DELAY
	  //end
      return 0;
    }
    #endif /* L1_RF_SAT_TUNER_STANDBY */
    SAT_DELAY
  }
  //csm,add,20130802
  SiTRACE("Disconnect tuners i2c\n");
  Si2164B_L2_Tuner_I2C_Disable(front_end);
  DTV_DELAY

  SiTRACE("Connect    tuners i2c\n");
  Si2164B_L2_Tuner_I2C_Enable(front_end);
  DTV_DELAY
  //end
  /* ------------------------------------------------------------ */
  /* Sleep Ter Tuner                                              */
  /* Sleep terrestrial tuner  if transition from '1' to '0'       */
  /* ------------------------------------------------------------ */

#if 0
  // Si2190B XOUT disable in Analog, for Single T2CS2 & Single T2C & Single T2C(Si2168C) model
  if ((new_standard == Si2164B_DD_MODE_PROP_MODULATION_ANALOG) && (front_end->tuner_ter->i2c->address == TER2_MAIN_ADDRESS))
  {
	if ((res= TER_TUNER_CLOCK_OFF(front_end->tuner_ter)) !=0 ) {
		SiTRACE("Terrestrial tuner CLOCK OFF error: 0x%02x : %s\n",res, TER_TUNER_ERROR_TEXT(res) );
		SiERROR("Terrestrial tuner CLOCK OFF error!\n");
		//csm,add
		SiTRACE("Disconnect tuners i2c\n");
		Si2164B_L2_Tuner_I2C_Disable(front_end);
		//end
		return 0;
	}
  }
#endif

  if ((ter_tuner_already_used == 1) & (ter_tuner_needed == 0) ) {
   #ifdef    INDIRECT_I2C_CONNECTION
    if (ter_i2c_connected==0) {
      SiTRACE("-- I2C -- Connect TER tuner i2c to sleep it\n");
      front_end->f_TER_tuner_enable(front_end->callback);
      ter_i2c_connected++;
    }
    DTV_DELAY
   #endif /* INDIRECT_I2C_CONNECTION */
    SiTRACE("Sleep terrestrial tuner\n");
#if 0//sem
    #ifdef    TER_TUNER_CLOCK_OFF
    if (front_end->demod->tuner_ter_clock_control != Si2164B_CLOCK_ALWAYS_ON) {
		SiTRACE("Terrestrial tuner clock OFF\n");
		if ((res= TER_TUNER_CLOCK_OFF(front_end->tuner_ter)) !=0 ) {
		  SiTRACE("Terrestrial tuner CLOCK OFF error: 0x%02x : %s\n",res, TER_TUNER_ERROR_TEXT(res) );
		  SiERROR("Terrestrial tuner CLOCK OFF error!\n");
		  //csm,add
		  SiTRACE("Disconnect tuners i2c\n");
		  Si2164B_L2_Tuner_I2C_Disable(front_end);
		  //end
		  return 0;
		}
	}
    #endif /* TER_TUNER_CLOCK_OFF */
#endif

    #ifdef    TER_TUNER_STANDBY
    SiTRACE("Terrestrial tuner STANDBY\n");
    if ((res= TER_TUNER_STANDBY(front_end->tuner_ter)) !=0 ) {
      SiTRACE("Terrestrial tuner Standby error: 0x%02x : %s\n",res, TER_TUNER_ERROR_TEXT(res) );
      SiERROR("Terrestrial tuner Standby error!\n");
	  //csm,add
	  SiTRACE("Disconnect tuners i2c\n");
	  Si2164B_L2_Tuner_I2C_Disable(front_end);
	  DTV_DELAY
	  //end
      return 0;
    }
    #endif /* TER_TUNER_STANDBY */
    TER_DELAY
  }
  //csm,add,20130802
  SiTRACE("Disconnect tuners i2c\n");
  Si2164B_L2_Tuner_I2C_Disable(front_end);
  DTV_DELAY

  SiTRACE("Connect    tuners i2c\n");
  Si2164B_L2_Tuner_I2C_Enable(front_end);
  DTV_DELAY
  //end
  /* ------------------------------------------------------------ */
  /* Wakeup Sat Tuner                                             */
  /* Wake up satellite   tuner if transition from '0' to '1'      */
  /* ------------------------------------------------------------ */
  if ((sat_tuner_already_used == 0) & (sat_tuner_needed == 1)) {
   #ifdef    INDIRECT_I2C_CONNECTION
    if (sat_i2c_connected==0) {
      SiTRACE("-- I2C -- Connect SAT tuner i2c to init/wakeup it\n");
      front_end->f_SAT_tuner_enable(front_end->callback);
      sat_i2c_connected++;
    }
    DTV_DELAY
   #endif /* INDIRECT_I2C_CONNECTION */
    if (front_end->SAT_tuner_init_done==0) {
      SiTRACE("Init satellite tuner\n");
#if 0//20130819,csm -> SiLabs_API_SAT_Tuner_Init
      #ifdef    L1_RF_SAT_TUNER_InitAfterReset
      if ((res= L1_RF_SAT_TUNER_InitAfterReset(front_end->tuner_sat)) !=0) {
        SiTRACE("Satellite tuner HW init error: 0x%02x\n",res );
		//csm,add
	  	SiTRACE("Disconnect tuners i2c\n");
	  	Si2164B_L2_Tuner_I2C_Disable(front_end);
		DTV_DELAY
	  	//end
        return 0;
      }
      #endif /* L1_RF_SAT_TUNER_InitAfterReset */
#endif
      front_end->SAT_tuner_init_done = 1;
    } else {
      SiTRACE("Wakeup satellite tuner\n");
      #ifdef    L1_RF_SAT_TUNER_WAKEUP
      if ((res= L1_RF_SAT_TUNER_WAKEUP(front_end->tuner_sat)) !=0) {
        SiTRACE("Satellite tuner wake up error: 0x%02x\n",res );
		//csm,add
	  	SiTRACE("Disconnect tuners i2c\n");
	  	Si2164B_L2_Tuner_I2C_Disable(front_end);
		DTV_DELAY
	  	//end
        return 0;
      }
      #endif /* L1_RF_SAT_TUNER_WAKEUP */
    }
    SAT_DELAY
  }
  /* ------------------------------------------------------------ */
  /* If the satellite tuner's clock is required, activate it      */
  /* ------------------------------------------------------------ */
  SiTRACE("sat_clock_needed %d\n",sat_clock_needed);
  if (sat_clock_needed) {
    #ifdef    SAT_TUNER_CLOCK_ON
     #ifdef    INDIRECT_I2C_CONNECTION
      if (sat_i2c_connected==0) {
        SiTRACE("-- I2C -- Connect SAT tuner i2c to start its clock\n");
        front_end->f_SAT_tuner_enable(front_end->callback);
        sat_i2c_connected++;
      }
      DTV_DELAY
     #endif /* INDIRECT_I2C_CONNECTION */
    if (front_end->demod->tuner_sat_clock_control != Si2164B_CLOCK_ALWAYS_OFF) {
      SiTRACE("Turn satellite tuner clock on\n");
      if ((res= SAT_TUNER_CLOCK_ON(front_end->tuner_sat) ) !=0) {
        SiTRACE("Satellite tuner CLOCK ON error: 0x%02x\n",res );
        return 0;
      }
    }
    #endif /* SAT_TUNER_CLOCK_ON */
    SAT_DELAY
  }
  //csm,add,20130802
  SiTRACE("Disconnect tuners i2c\n");
  Si2164B_L2_Tuner_I2C_Disable(front_end);
  DTV_DELAY

  SiTRACE("Connect    tuners i2c\n");
  Si2164B_L2_Tuner_I2C_Enable(front_end);
  DTV_DELAY
  //end
  /* ------------------------------------------------------------ */
  /* Wakeup Ter Tuner                                             */
  /* Wake up terrestrial tuner if transition from '0' to '1'      */
  /* ------------------------------------------------------------ */
  if ((ter_tuner_already_used == 0) & (ter_tuner_needed == 1)) {
   #ifdef    INDIRECT_I2C_CONNECTION
    if (ter_i2c_connected==0) {
      SiTRACE("-- I2C -- Connect TER tuner i2c to init/wakeup it\n");
      front_end->f_TER_tuner_enable(front_end->callback);
      ter_i2c_connected++;
    }
    DTV_DELAY
   #endif /* INDIRECT_I2C_CONNECTION */
    /* Do a full init of the Ter Tuner only if it has not been already done */
    if (front_end->TER_tuner_init_done==0) {
#if 0 // jhkwon_temp
      SiTRACE("Init terrestrial tuner\n");
      #ifdef    TER_TUNER_INIT
	  /* When INIT1 is used, only configure the tuner (FW is already downloaded) */
	  if(front_end->external_rf_fw_download == 1){//if (front_end->external_fw_download == 1) {
		  res= Si2151_Configure(front_end->tuner_ter);
		  SiTRACE("Tuner configuration\n");
	  }
	  //end
	  /* When INIT1 is not used, load the tuner fw and configure the tuner       */
      if (front_end->external_rf_fw_download == 0) {//if (front_end->external_fw_download == 0) {
		  if ((res= TER_TUNER_INIT(front_end->tuner_ter)) !=0) {
			#ifdef    TER_TUNER_ERROR_TEXT
			SiTRACE("Terrestrial tuner HW init error: 0x%02x : %s\n",res, TER_TUNER_ERROR_TEXT(res) );
			#endif /* TER_TUNER_ERROR_TEXT */
			SiERROR("Terrestrial tuner HW init error!\n");
			//csm,add
	  		SiTRACE("Disconnect tuners i2c\n");
	  		Si2164B_L2_Tuner_I2C_Disable(front_end);
			DTV_DELAY
	  		//end
			return 0;
		  }
	  }
	  //model specific setting,20130821,csm,xtal freq tolerance 0ppm(actual:-20~-10ppm) for dual model
	  if((front_end->tuner_ter->i2c->address == TER1_MAIN_ADDRESS) || (front_end->tuner_ter->i2c->address == TER1_SUB_ADDRESS)) // for Dual T2CS2 & Dual T2C model
	  {
		  front_end->tuner_ter->prop->crystal_trim.xo_cap = 6; // 6p2pF

		  if (Si2151_L1_SetProperty2(front_end->tuner_ter, Si2151_CRYSTAL_TRIM_PROP_CODE) != NO_Si2151_ERROR)
		  {
			  SiTRACE("Si2151_CRYSTAL_TRIM_PROP_CODE error\n");
			  SiTRACE("Disconnect tuners i2c\n");
			  Si2164B_L2_Tuner_I2C_Disable(front_end);
			  DTV_DELAY
			  return 0;
		  }
		  SiTRACE("Si2151_CRYSTAL_TRIM_PROP_CODE complete...\n");
	  }
      //20130821,end
      //SiLabs_TER_Tuner_DTV_OUT_TYPE      (front_end->tuner_ter,front_end->demod->TER_tuner_if_output);
      //SiLabs_TER_Tuner_DTV_AGC_SOURCE    (front_end->tuner_ter,front_end->demod->TER_tuner_agc_input);
      #ifdef    TER_TUNER_AGC_EXTERNAL
	  SiTRACE("TER_TUNER_AGC_EXTERNAL setting...\n");
      if (front_end->demod->tuner_ter_agc_control) {
        TER_TUNER_AGC_EXTERNAL(front_end->tuner_ter);
      }
      #endif /* TER_TUNER_AGC_EXTERNAL */
      #endif /* TER_TUNER_INIT */
#endif
      front_end->TER_tuner_init_done =1;
    } else {
      SiTRACE("Wakeup terrestrial tuner\n");
      #ifdef    TER_TUNER_WAKEUP
      if ((res= TER_TUNER_WAKEUP(front_end->tuner_ter)) !=0) {
        SiTRACE("Terrestrial tuner wake up error: 0x%02x : %s\n",res, TER_TUNER_ERROR_TEXT(res) );
        SiERROR("Terrestrial tuner wake up error!\n");
		//csm,add
	  	SiTRACE("Disconnect tuners i2c\n");
	  	Si2164B_L2_Tuner_I2C_Disable(front_end);
		DTV_DELAY
	  	//end
        return 0;
      }
      #endif /* TER_TUNER_WAKEUP */
    }
    TER_DELAY
  }
    /* ------------------------------------------------------------ */
    /* If the terrestrial tuner's clock is required, activate it    */
    /* ------------------------------------------------------------ */
  SiTRACE("ter_clock_needed %d\n",ter_clock_needed);
  if (ter_clock_needed) {
    SiTRACE("Turn terrestrial tuner clock on\n");
    #ifdef    TER_TUNER_CLOCK_ON
     #ifdef    INDIRECT_I2C_CONNECTION
      if (ter_i2c_connected==0) {
        SiTRACE("-- I2C -- Connect TER tuner i2c to start its clock\n");
        front_end->f_TER_tuner_enable(front_end->callback);
        ter_i2c_connected++;
      }
      DTV_DELAY
     #endif /* INDIRECT_I2C_CONNECTION */
    if (front_end->demod->tuner_ter_clock_control != Si2164B_CLOCK_ALWAYS_OFF) {
#if 0 // jhkwon_temp
    SiTRACE("Terrestrial tuner CLOCK ON\n");
    if ((res= TER_TUNER_CLOCK_ON(front_end->tuner_ter) ) !=0) {
      SiTRACE("Terrestrial tuner CLOCK ON error: 0x%02x : %s\n",res, TER_TUNER_ERROR_TEXT(res) );
      SiERROR("Terrestrial tuner CLOCK ON error!\n");
	  //csm,add
	  SiTRACE("Disconnect tuners i2c\n");
	  Si2164B_L2_Tuner_I2C_Disable(front_end);
	  DTV_DELAY
	  //end
      return 0;
      }
#endif
    }
    #endif /* TER_TUNER_CLOCK_ON */
    TER_DELAY
  }
  if ((front_end->previous_standard != new_standard) & (dtv_demod_needed == 0) & (front_end->demod->media == Si2164B_TERRESTRIAL)) {
   if (front_end->demod->media == Si2164B_TERRESTRIAL) {
    #ifdef    TER_TUNER_ATV_LO_INJECTION
     TER_TUNER_ATV_LO_INJECTION(front_end->tuner_ter);
    #endif /* TER_TUNER_ATV_LO_INJECTION */

  }
 }

  /* ------------------------------------------------------------ */
  /* Change Dtv Demod standard if required                        */
  /* ------------------------------------------------------------ */
  if ((front_end->previous_standard != new_standard) & (dtv_demod_needed == 1)) {
    SiTRACE("Store demod standard (%d)\n", new_standard);
    front_end->demod->standard = new_standard;
    DTV_DELAY
    if (front_end->demod->media == Si2164B_TERRESTRIAL) {
    #ifdef    TER_TUNER_DTV_LO_INJECTION
     TER_TUNER_DTV_LO_INJECTION(front_end->tuner_ter);
    #endif /* TER_TUNER_DTV_LO_INJECTION */
    #ifdef    TER_TUNER_DTV_LIF_OUT_AMP
    /* Adjusting LIF signal for cable or terrestrial reception */
      switch (new_standard) {
        case Si2164B_DD_MODE_PROP_MODULATION_DVBT :
        case Si2164B_DD_MODE_PROP_MODULATION_DVBC2:
        case Si2164B_DD_MODE_PROP_MODULATION_DVBT2:
        {
          TER_TUNER_DTV_LIF_OUT_AMP(front_end->tuner_ter, 0);
          break;
        }
       case Si2164B_DD_MODE_PROP_MODULATION_MCNS :
       case Si2164B_DD_MODE_PROP_MODULATION_DVBC : {
          TER_TUNER_DTV_LIF_OUT_AMP(front_end->tuner_ter, 1);
         break;
       }
       default: break;
      }
    #endif /* TER_TUNER_DTV_LIF_OUT_AMP */
    }
  }
  //csm,add
  SiTRACE("Disconnect tuners i2c\n");
  Si2164B_L2_Tuner_I2C_Disable(front_end);
  DTV_DELAY
  //end
  /* ------------------------------------------------------------ */
  /* Wakeup Dtv Demod                                             */
  /*  if it has been put in 'standby mode' and is needed          */
  /* ------------------------------------------------------------ */
  if (front_end->Si2164B_init_done) {
    SiTRACE("dtv_demod_sleeping %d\n", dtv_demod_sleeping);
    if ((dtv_demod_sleeping == 1) & (dtv_demod_needed == 1) ) {
      if (dtv_demod_already_used == 0) {
        SiTRACE("Take DTV demod out of SILENT mode\n");
        Si2164B_L2_SILENT(front_end, 0);
      } else {
        SiTRACE("Wake UP DTV demod\n");
        if (Si2164B_WAKEUP (front_end->demod) == NO_Si2164B_ERROR) {
          SiTRACE("Wake UP DTV demod OK\n");
        } else {
          SiERROR("Wake UP DTV demod failed!\n");
          SiTRACE("Wake UP DTV demod failed!\n");
          return 0;
        }
      }
    }
  }
  /* ------------------------------------------------------------ */
  /* Setup Dtv Demod                                              */
  /* Setup dtv demodulator if transition from '0' to '1'          */
  /* ------------------------------------------------------------ */
  if (dtv_demod_needed == 1) {
    /* Do the 'first init' only the first time, plus if requested  */
    /* (when 'force' flag is 1, Si2164B_init_done is set to '0')   */
    if (!front_end->Si2164B_init_done) {
      SiTRACE("Init demod\n");
#ifdef    ORIGINAL_Si2164B_init_done
      if (front_end->external_fw_download == 1) {
        front_end->Si2164B_init_done = 1;//20131127,cold booting
        SiTRACE("Demod init already done outside switch_to_standard\n");
      }
#endif /* ORIGINAL_Si2164B_init_done:  This needs to be moved at the beginning of the function */
      if (front_end->external_fw_download == 0) {
		  if (Si2164B_Init(front_end->demod) == NO_Si2164B_ERROR) {
			front_end->Si2164B_init_done = 1;
			SiTRACE("Demod init OK\n");
	        if ( (front_end->demod->rsp->part_info.part == 60) | (front_end->demod->rsp->part_info.part == 80) | (front_end->demod->rsp->part_info.part == 81) ) {
	          front_end->auto_detect_TER     = 0; /* auto_detect_TER set to '0' by default for '60', '80' and '81' parts, which don't support DVB_T2 */
	        }
		  } else {
			SiTRACE("Demod init failed!\n");
			SiERROR("Demod init failed!\n");
			return 0;
		  }
	  }
    }
    if (front_end->demod->media == Si2164B_TERRESTRIAL) {
      SiTRACE("front_end->demod->media Si2164B_TERRESTRIAL\n");
      if (front_end->TER_init_done == 0) {
        SiTRACE("Configure demod for TER\n");
        if (Si2164B_Configure(front_end->demod) == NO_Si2164B_ERROR) {
          /* set dd_mode.modulation again, as it is overwritten by Si2164B_Configure */
          front_end->demod->prop->dd_mode.modulation = new_standard;
          front_end->TER_init_done = 1;
        } else {
          SiTRACE("Demod TER configuration failed !\n");
          SiERROR("Demod TER configuration failed !\n");
          return 0;
        }
      }

		/* 20150226 [Improvement Main Analog beat when tuning Sub T2] Start */
		front_end->demod->prop->dd_ts_setup_par.ts_data_strength = 3;
		front_end->demod->prop->dd_ts_setup_par.ts_data_shape = 2;
		front_end->demod->prop->dd_ts_setup_par.ts_clk_strength = 3;
		front_end->demod->prop->dd_ts_setup_par.ts_clk_shape = 2;

		Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DD_TS_SETUP_PAR_PROP_CODE);
		/* 20150226 [Improvement Main Analog beat when tuning Sub T2] End */

      DTV_DELAY
      //csm,add,20130802
      SiTRACE("Connect    tuners i2c\n");
      Si2164B_L2_Tuner_I2C_Enable(front_end);
      DTV_DELAY
	  //end
      /* ------------------------------------------------------------ */
      /* Manage FEF mode in TER tuner                                 */
      /* ------------------------------------------------------------ */
      if (new_standard == Si2164B_DD_MODE_PROP_MODULATION_DVBT2) {
        Si2164B_L2_TER_FEF_SETUP (front_end, 1);
      } else {
        Si2164B_L2_TER_FEF_SETUP (front_end, 0);
      }
      TER_DELAY
	  //csm,add,20130802
	  SiTRACE("Disconnect tuners i2c\n");
      Si2164B_L2_Tuner_I2C_Disable(front_end);
      DTV_DELAY
      //end
    }
    if (front_end->demod->media == Si2164B_SATELLITE  ) {
      SiTRACE("front_end->demod->media Si2164B_SATELLITE\n");
      if (front_end->SAT_init_done == 0) {
        SiTRACE("Configure demod for SAT\n");
        if (Si2164B_Configure(front_end->demod) == NO_Si2164B_ERROR) {
          /* set dd_mode.modulation again, as it is overwritten by Si2164B_Configure */
          front_end->demod->prop->dd_mode.modulation      = Si2164B_DD_MODE_PROP_MODULATION_AUTO_DETECT;
          if ( new_standard == Si2164B_DD_MODE_PROP_MODULATION_DSS ) {
          front_end->demod->prop->dd_mode.auto_detect     = Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_S_S2_DSS;
          } else {
          front_end->demod->prop->dd_mode.auto_detect     = Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_S_S2;
          }
          front_end->SAT_init_done = 1;
        } else {
          SiTRACE("Demod SAT configuration failed !\n");
          SiERROR("Demod SAT configuration failed !\n");
          return 0;
        }
      }

		/* 20150226 [Improvement Main Analog beat when tuning Sub T2] Start */
		front_end->demod->prop->dd_ts_setup_par.ts_data_strength = 4;
		front_end->demod->prop->dd_ts_setup_par.ts_data_shape = 0;
		front_end->demod->prop->dd_ts_setup_par.ts_clk_strength = 4;
		front_end->demod->prop->dd_ts_setup_par.ts_clk_shape = 0;

		Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DD_TS_SETUP_PAR_PROP_CODE);
		/* 20150226 [Improvement Main Analog beat when tuning Sub T2] End */
    }

    front_end->demod->prop->dd_mode.invert_spectrum = Si2164B_L2_Set_Invert_Spectrum(front_end);
	front_end->demod->prop->dd_mode.modulation = new_standard;
	SiTRACE("Set dd_mode.modulation=(%d) now sending the property DD_MODE\n", new_standard);
    if (Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DD_MODE_PROP_CODE)==0) {
	  SiTRACE("Property DD_MODE should be sent \n", new_standard);
      Si2164B_L1_DD_RESTART(front_end->demod);
    } else {
      SiTRACE("Demod restart failed !\n");
      return 0;
    }
  }
  DTV_DELAY

  /* ------------------------------------------------------------ */
  /* Forbid i2c traffic to reach the tuners                       */
  /* ------------------------------------------------------------ */
  #ifdef    INDIRECT_I2C_CONNECTION
  if (sat_i2c_connected) {
    SiTRACE("-- I2C -- Disconnect SAT tuner i2c\n");
    front_end->f_SAT_tuner_disable(front_end->callback);
    DTV_DELAY
  }
  if (ter_i2c_connected) {
    SiTRACE("-- I2C -- Disconnect TER tuner i2c\n");
    front_end->f_TER_tuner_disable(front_end->callback);
    DTV_DELAY
  }
  #else  /* INDIRECT_I2C_CONNECTION */
  if (i2c_connected) {
    SiTRACE("Disconnect tuners i2c\n");
    Si2164B_L2_Tuner_I2C_Disable(front_end);
    DTV_DELAY
  }
  #endif /* INDIRECT_I2C_CONNECTION */
  /* ------------------------------------------------------------ */
  /* update value of previous_standard to prepare next call       */
  /* ------------------------------------------------------------ */
  front_end->previous_standard = new_standard;
  front_end->demod->standard   = new_standard;

  front_end->first_init_done = 1;
#ifdef    PROFILING
  sequence = malloc(sizeof(unsigned char)*1000);
  if(sequence != 0)
  {
	  safe_sprintf(sequence,"%s","");
	  safe_sprintf(sequence,"%s| TER: %4d ms ", sequence, ter_tuner_delay);
	  safe_sprintf(sequence,"%s| SAT: %4d ms ", sequence, sat_tuner_delay);
	  safe_sprintf(sequence,"%s| DTV: %4d ms ", sequence, dtv_demod_delay);
	  safe_sprintf(sequence,"%s| (%5d ms) ",    sequence, system_time()-switch_start);
	  SiTRACE("%s\n", sequence);
      free(sequence);
  }
  else
  {	SiTRACE("fail to create buffer to print log\n");
  }
#endif /* PROFILING */

  SiTRACE("Si2164B_switch_to_standard complete\n");
  return 1;
}
/************************************************************************************************************************
  Si2164B_lock_to_carrier function
  Use:      relocking function
            Used to relock on a channel for the current standard
            options:
              if freq = 0, do not tune. This is useful to test the lock time without the tuner delays.
              if freq < 0, do not tune and don't change settings. Just do a DD_RESTART. This is useful to test the relock time upom a reset.
  Parameter: standard the standard to lock to
  Parameter: freq                the frequency to lock to    (in Hz for TER, in kHz for SAT)
  Parameter: dvb_t_bandwidth_hz  the channel bandwidth in Hz (only for DVB-T, DVB-T2, ISDB-T)
  Parameter: dvb_t_stream        the HP/LP stream            (only for DVB-T)
  Parameter: symbol_rate_bps     the symbol rate             (for DVB-C, MCNS and SAT)
  Parameter: dvb_c_constellation the DVB-C constellation     (only for DVB-C)
  Parameter: data_slice_id       the DVB-C2 data slice Id    (only for DVB-C2)
  Parameter: plp_id              the PLP Id                  (only for DVB-T2 and DVB-C2 when num_dslice  > 1)
  Parameter: T2_lock_mode        the DVB-T2 lock mode        (0='ANY', 1='T2-Base', 2='T2-Lite')
  Return:    1 if locked, 0 otherwise
************************************************************************************************************************/
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
                                  )
{
  signed int return_code;
  signed int lockStartTime;       /* lockStartTime is used to trace the time spent in Si2164B_L2_lock_to_carrier and is only set at when entering the function                       */
  signed int startTime;           /* startTime is used to measure internal durations. It is set in various places, whenever required                                                */
  signed int searchDelay;
  signed int handshakeDelay;
  signed int lock;
  signed int new_lock;
  signed int max_lock_time_ms;
  signed int min_lock_time_ms;
  signed int lpf_khz;
  /* init all flags to avoid compiler warnings */
  return_code      = 0; /* To avoid code checker warning */
  startTime        = 0; /* To avoid code checker warning */
  searchDelay      = 0; /* To avoid code checker warning */
  handshakeDelay   = 0; /* To avoid code checker warning */
  lock             = 0; /* To avoid code checker warning */
  min_lock_time_ms = 0; /* To avoid code checker warning */
  max_lock_time_ms = 0; /* To avoid code checker warning */
  plp_id = plp_id; /* to avoid compiler warning when not used */

  lockStartTime    = system_time(); /* lockStartTime is used to trace the time spent in Si2164B_L2_lock_to_carrier and is only set here */
  new_lock         = 1;

  data_slice_id = data_slice_id; /* to avoid compiler warning when not used */

  front_end->lockAbort = 0;
  SiTRACE ("relock to %s at %d\n", Si2164B_standardName(standard), freq);

  if (front_end->handshakeUsed == 0) {
    new_lock = 1;
    front_end->searchStartTime = lockStartTime;
  }
  if (front_end->handshakeUsed == 1) {
    if (front_end->handshakeOn == 1) {
      new_lock = 0;
      SiTRACE("lock_to_carrier_handshake : recalled after   handshake.\n");
    }
    if (front_end->handshakeOn == 0) {
      new_lock = 1;
      front_end->handshakeStart_ms = lockStartTime;
    }
    front_end->searchStartTime = front_end->handshakeStart_ms;
    SiTRACE("lock_to_carrier_handshake : handshake start %d\n", front_end->handshakeStart_ms);
  }

  /* Setting max_lock_time_ms and min_lock_time_ms for locking on required standard */
  switch (standard)
  {
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT : {
      max_lock_time_ms = Si2164B_DVBT_MAX_LOCK_TIME;
      min_lock_time_ms = Si2164B_DVBT_MIN_LOCK_TIME;
      if (front_end->auto_detect_TER) {
        max_lock_time_ms = Si2164B_DVBT2_MAX_LOCK_TIME;
      }
      break;
    }
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT2: {
      max_lock_time_ms = Si2164B_DVBT2_MAX_LOCK_TIME;
      min_lock_time_ms = Si2164B_DVBT2_MIN_LOCK_TIME;
      if (front_end->auto_detect_TER) {
        min_lock_time_ms = Si2164B_DVBT_MIN_LOCK_TIME;
      }
      break;
    }
    case Si2164B_DD_MODE_PROP_MODULATION_DVBC : {
      max_lock_time_ms = Si2164B_DVB_C_max_lock_ms(front_end->demod, dvb_c_constellation, symbol_rate_bps);
      min_lock_time_ms = Si2164B_DVBC_MIN_LOCK_TIME;
      break;
    }
      case Si2164B_DD_MODE_PROP_MODULATION_DVBC2 : {
        max_lock_time_ms = Si2164B_DVBC2_MAX_LOCK_TIME;
        min_lock_time_ms = Si2164B_DVBC2_MIN_LOCK_TIME;
        break;
      }
    case Si2164B_DD_MODE_PROP_MODULATION_MCNS : {
      max_lock_time_ms = Si2164B_DVB_C_max_lock_ms(front_end->demod, dvb_c_constellation, symbol_rate_bps);
      min_lock_time_ms = Si2164B_DVBC_MIN_LOCK_TIME;
      break;
    }
      case Si2164B_DD_MODE_PROP_MODULATION_DSS  :
      case Si2164B_DD_MODE_PROP_MODULATION_DVBS :
      case Si2164B_DD_MODE_PROP_MODULATION_DVBS2: {
        if (front_end->auto_detect_SAT) {
          max_lock_time_ms = Si2164B_DVBS2_MAX_LOCK_TIME*2;
          min_lock_time_ms = Si2164B_DVBS2_MIN_LOCK_TIME;
        } else {
          if ( (standard == Si2164B_DD_MODE_PROP_MODULATION_DVBS ) || (standard == Si2164B_DD_MODE_PROP_MODULATION_DSS ) ) {
            max_lock_time_ms = Si2164B_DVBS_MAX_LOCK_TIME*2;
            min_lock_time_ms = Si2164B_DVBS_MIN_LOCK_TIME;
          }
          if   (standard == Si2164B_DD_MODE_PROP_MODULATION_DVBS2) {
            max_lock_time_ms = Si2164B_DVBS2_MAX_LOCK_TIME*2;
            min_lock_time_ms = Si2164B_DVBS2_MIN_LOCK_TIME;
          }
        }
        break;
      }
    default : /* ATV */   {
      break;
    }
  }

  /* change settings only if not testing the relock time upon a reset (activated if freq<0) */
  if ( (freq >= 0 ) && (new_lock == 1) ) {
  if (front_end->demod->media == Si2164B_SATELLITE) {
    switch (standard) {
      case Si2164B_DD_MODE_PROP_MODULATION_DSS  :
      case Si2164B_DD_MODE_PROP_MODULATION_DVBS : {
        SiTRACE("dvbs_afc_range.range_khz = %d\n", front_end->demod->prop->dvbs_afc_range.range_khz);
/*      lpf_khz = (signed int)((symbol_rate_bps*(1.35/2.0))/1000.0 + front_end->demod->prop->dvbs_afc_range.range_khz); */
        //lpf_khz = (((symbol_rate_bps/40000)*27)  + front_end->demod->prop->dvbs_afc_range.range_khz);
        lpf_khz = (signed int)symbol_rate_bps;  // 20120821 mod jhkwon
        break;
      }
      case Si2164B_DD_MODE_PROP_MODULATION_DVBS2: {
        SiTRACE("dvbs2_afc_range.range_khz = %d\n",front_end->demod->prop->dvbs2_afc_range.range_khz);
/*      lpf_khz = (signed int)((symbol_rate_bps*(1.35/2.0))/1000 + front_end->demod->prop->dvbs2_afc_range.range_khz); */
        //lpf_khz = (((symbol_rate_bps/40000)*27)  + front_end->demod->prop->dvbs2_afc_range.range_khz);
        lpf_khz = (signed int)symbol_rate_bps;  // 20120821 mod jhkwon
        break;
      }
      default: lpf_khz = 63000; break;
    }
    SiTRACE("lpf_khz = %d \n", lpf_khz);
    Si2164B_L2_SAT_LPF (front_end, lpf_khz);
  }
  /* Setting demod for locking on required standard */
    switch (standard)
    {
      case Si2164B_DD_MODE_PROP_MODULATION_DVBT2:
      case Si2164B_DD_MODE_PROP_MODULATION_DVBT : {
        front_end->demod->prop->dd_mode.bw                = ter_bandwidth_hz/1000000;
        front_end->demod->prop->dvbt_hierarchy.stream = dvb_t_stream;
        return_code = Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DVBT_HIERARCHY_PROP_CODE);
        if (ter_bandwidth_hz == 1700000) {
          front_end->demod->prop->dd_mode.bw              = Si2164B_DD_MODE_PROP_BW_BW_1D7MHZ;
        }
        if (front_end->auto_detect_TER) {
          SiTRACE("DVB-T/T2 auto detect\n");
          if (plp_id != -1) {
            front_end->demod->cmd->dvbt2_plp_select.plp_id_sel_mode = Si2164B_DVBT2_PLP_SELECT_CMD_PLP_ID_SEL_MODE_MANUAL;
            front_end->demod->cmd->dvbt2_plp_select.plp_id = (unsigned char)plp_id;
          } else {
            front_end->demod->cmd->dvbt2_plp_select.plp_id_sel_mode = Si2164B_DVBT2_PLP_SELECT_CMD_PLP_ID_SEL_MODE_AUTO;
            front_end->demod->cmd->dvbt2_plp_select.plp_id = 0;
          }
          Si2164B_L1_DVBT2_PLP_SELECT    (front_end->demod, front_end->demod->cmd->dvbt2_plp_select.plp_id , front_end->demod->cmd->dvbt2_plp_select.plp_id_sel_mode);
          front_end->demod->prop->dd_mode.modulation  = Si2164B_DD_MODE_PROP_MODULATION_AUTO_DETECT;
          front_end->demod->prop->dd_mode.auto_detect = Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_T_T2;
          front_end->demod->prop->dvbt2_mode.lock_mode= T2_lock_mode;
          Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DVBT2_MODE_PROP_CODE);
          SiTRACE ("T2_lock_mode %d\n",T2_lock_mode);
        } else {
          if (standard == Si2164B_DD_MODE_PROP_MODULATION_DVBT ) {
            front_end->demod->prop->dvbt_hierarchy.stream     = dvb_t_stream;
            return_code = Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DVBT_HIERARCHY_PROP_CODE);
          }
          if (standard == Si2164B_DD_MODE_PROP_MODULATION_DVBT2) {
            if (plp_id != -1) {
              front_end->demod->cmd->dvbt2_plp_select.plp_id_sel_mode = Si2164B_DVBT2_PLP_SELECT_CMD_PLP_ID_SEL_MODE_MANUAL;
              front_end->demod->cmd->dvbt2_plp_select.plp_id = (unsigned char)plp_id;
            } else {
              front_end->demod->cmd->dvbt2_plp_select.plp_id_sel_mode = Si2164B_DVBT2_PLP_SELECT_CMD_PLP_ID_SEL_MODE_AUTO;
              front_end->demod->cmd->dvbt2_plp_select.plp_id = 0;
            }
            Si2164B_L1_DVBT2_PLP_SELECT    (front_end->demod, front_end->demod->cmd->dvbt2_plp_select.plp_id , front_end->demod->cmd->dvbt2_plp_select.plp_id_sel_mode);
            front_end->demod->prop->dvbt2_mode.lock_mode= T2_lock_mode;
            Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DVBT2_MODE_PROP_CODE);
            SiTRACE ("T2_lock_mode %d\n",T2_lock_mode);
          }
        }
        Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DD_MODE_PROP_CODE);
        SiTRACE("bw %d Hz\n", ter_bandwidth_hz);
        break;
      }
      case Si2164B_DD_MODE_PROP_MODULATION_DVBC : {
        front_end->demod->prop->dd_mode.bw                       = 8;
        front_end->demod->prop->dvbc_symbol_rate.rate            = symbol_rate_bps/1000;
        front_end->demod->prop->dvbc_constellation.constellation = dvb_c_constellation;
        Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DD_MODE_PROP_CODE);
        Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DVBC_SYMBOL_RATE_PROP_CODE);
        Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DVBC_CONSTELLATION_PROP_CODE);
        SiTRACE("sr %d bps, constel %d\n", symbol_rate_bps, dvb_c_constellation);
        break;
      }
      case Si2164B_DD_MODE_PROP_MODULATION_DVBC2 : {
        if (data_slice_id != -1) {
          front_end->demod->cmd->dvbc2_ds_plp_select.ds_id             = (unsigned char)data_slice_id;
          front_end->demod->cmd->dvbc2_ds_plp_select.plp_id            = (unsigned char)plp_id;
          front_end->demod->cmd->dvbc2_ds_plp_select.id_sel_mode     = Si2164B_DVBC2_DS_PLP_SELECT_CMD_ID_SEL_MODE_MANUAL;
        } else {
          front_end->demod->cmd->dvbc2_ds_plp_select.ds_id             = 0;
          front_end->demod->cmd->dvbc2_ds_plp_select.plp_id            = 0;
          front_end->demod->cmd->dvbc2_ds_plp_select.id_sel_mode     = Si2164B_DVBC2_DS_PLP_SELECT_CMD_ID_SEL_MODE_AUTO;
        }
        Si2164B_L1_DVBC2_DS_PLP_SELECT      (front_end->demod, front_end->demod->cmd->dvbc2_ds_plp_select.plp_id , front_end->demod->cmd->dvbc2_ds_plp_select.id_sel_mode, front_end->demod->cmd->dvbc2_ds_plp_select.ds_id);
        front_end->demod->prop->dd_mode.bw                       = 8;
        Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DD_MODE_PROP_CODE);
        break;
      }
    case Si2164B_DD_MODE_PROP_MODULATION_MCNS : {
      front_end->demod->prop->dd_mode.bw                       = 8;
      front_end->demod->prop->mcns_symbol_rate.rate            = symbol_rate_bps/1000;
      front_end->demod->prop->mcns_constellation.constellation = dvb_c_constellation;
      Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DD_MODE_PROP_CODE);
      Si2164B_L1_SetProperty2(front_end->demod, Si2164B_MCNS_SYMBOL_RATE_PROP_CODE);
      Si2164B_L1_SetProperty2(front_end->demod, Si2164B_MCNS_CONSTELLATION_PROP_CODE);
      SiTRACE("sr %d bps, constel %d\n", symbol_rate_bps, dvb_c_constellation);
      break;
    }
      case Si2164B_DD_MODE_PROP_MODULATION_DSS  :
      case Si2164B_DD_MODE_PROP_MODULATION_DVBS :
      case Si2164B_DD_MODE_PROP_MODULATION_DVBS2: {
        if (front_end->auto_detect_SAT) {
          SiTRACE("DVB-S/S2 auto detect\n");
          /* in AUTO_RELOCK, set dvbs2_symbol_rate as it is used as the SAT sr value */
          front_end->demod->prop->dvbs2_symbol_rate.rate           = symbol_rate_bps/1000;
          Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DVBS2_SYMBOL_RATE_PROP_CODE);
          /* Set dvbs_symbol_rate also because it will be used for monitoring the SR */
          front_end->demod->prop->dvbs_symbol_rate.rate            = symbol_rate_bps/1000;
          front_end->demod->prop->dd_mode.modulation  = Si2164B_DD_MODE_PROP_MODULATION_AUTO_DETECT;
          /* Trick to avoid slowing down AUTO SAT mode when not using DSS: */
          /*  AUTO_DVB_S_S2_DSS is only used if the input standard is DSS  */
          if (standard == Si2164B_DD_MODE_PROP_MODULATION_DSS) {
            front_end->demod->prop->dd_mode.auto_detect = Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_S_S2_DSS;
          } else {
            front_end->demod->prop->dd_mode.auto_detect = Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_S_S2;
          }
        } else {
          if ( (standard == Si2164B_DD_MODE_PROP_MODULATION_DVBS ) || (standard == Si2164B_DD_MODE_PROP_MODULATION_DSS ) ) {
          front_end->demod->prop->dvbs_symbol_rate.rate            = symbol_rate_bps/1000;
          Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DVBS_SYMBOL_RATE_PROP_CODE);
          }
          if   (standard == Si2164B_DD_MODE_PROP_MODULATION_DVBS2) {
          front_end->demod->prop->dvbs2_symbol_rate.rate           = symbol_rate_bps/1000;
          Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DVBS2_SYMBOL_RATE_PROP_CODE);
          }
        }
        Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DD_MODE_PROP_CODE);
        SiTRACE("sr %d bps\n", symbol_rate_bps);
        break;
      }
      default : /* ATV */   {
        SiTRACE("'%d' standard (%s) is not managed by Si2164B_lock_to_carrier\n", standard, Si2164B_standardName(standard));
        return 0;
        break;
      }
    }

    if (front_end->lockAbort) {
      SiTRACE("Si2164B_L2_lock_to_carrier : lock aborted before tuning, after %d ms.\n", system_time() - lockStartTime );
      return 0;
    }

    /* ALlow using this function without tuning */
    if (freq >  0) {
      startTime = system_time();
      Si2164B_L2_Tune                  (front_end,       freq);
      SiTRACE ("Si2164B_lock_to_carrier 'tune'  took %3d ms\n", system_time() - startTime);
    }

    startTime = system_time();
    /* For standards not DVB-C2, start the lock process using Si2164B_L1_DD_RESTART */
    if (standard != Si2164B_DD_MODE_PROP_MODULATION_DVBC2) {
      Si2164B_L1_DD_RESTART            (front_end->demod);
      SiTRACE   ("Si2164B_lock_to_carrier 'reset' took %3d ms\n", system_time() - startTime);
    }
    /* For DVB-C2, start the lock process using Si2164B_L1_DVBC2_CTRL */
    if (standard == Si2164B_DD_MODE_PROP_MODULATION_DVBC2) {
      startTime = system_time();
      front_end->demod->cmd->dvbc2_ctrl.action        = Si2164B_DVBC2_CTRL_CMD_ACTION_START;
      front_end->demod->cmd->dvbc2_ctrl.tuned_rf_freq = freq;
      Si2164B_L1_DVBC2_CTRL (front_end->demod, front_end->demod->cmd->dvbc2_ctrl.action , front_end->demod->cmd->dvbc2_ctrl.tuned_rf_freq);
      SiTRACE   ("Si2164B_lock_to_carrier 'dvbc2_ctrl/START' took %3d ms\n", system_time() - startTime);
    }

    /* If tuning, reset the timeout reference time. This is done here to be independent on the tuning time */
    if (freq >  0) {
      front_end->searchStartTime = system_time();
    }

    /* as we will not lock in less than min_lock_time_ms, wait a while..., but check for a possible 'abort' from the application */
    startTime = system_time();

#if 0//kwon
    while (system_time() - startTime < min_lock_time_ms) {
      if (front_end->lockAbort) {
        SiTRACE("Si2164B_L2_lock_to_carrier : lock aborted before checking lock status, after %d ms.\n", system_time() - lockStartTime );
        return 0;
      }
      /* Adapt here the minimal 'reaction time' of the application*/
      system_wait(20);
    }
#endif//kwon
  }
  /* testing the relock time upon a reset (activated if freq<0) */
  if (freq < 0) {
    SiTRACE   ("Si2164B_lock_to_carrier 'only_reset'\n");
    Si2164B_L1_DD_RESTART (front_end->demod);
  }

#if 0//kwon

  /* The actual lock check loop */
  while (1) {

    searchDelay = system_time() - front_end->searchStartTime;

    /* Check the status for the current modulation */

    switch (standard) {
      default                                   :
      case Si2164B_DD_MODE_PROP_MODULATION_DVBT2 :
      case Si2164B_DD_MODE_PROP_MODULATION_DVBT  : {
        /* DVB-T/T2 auto detect seek loop, using Si2164B_L1_DD_STATUS                                          */
        /* if DL LOCKED                            : demod is locked on a dd_status->modulation signal        */
        /* if DL NO_LOCK and rsqint_bit5 NO_CHANGE : demod is searching for a DVB-T/T2 signal                 */
        /* if DL NO_LOCK and rsqint_bit5 CHANGE    : demod says this is not a DVB-T/T2 signal (= 'neverlock') */
        return_code = Si2164B_L1_DD_STATUS(front_end->demod, Si2164B_DD_STATUS_CMD_INTACK_CLEAR);
        if (return_code != NO_Si2164B_ERROR) {
          SiTRACE("Si2164B_lock_to_carrier: Si2164B_L1_DD_STATUS error\n");
          SiERROR("Si2164B_lock_to_carrier: Si2164B_L1_DD_STATUS error\n");
          goto exit_lock;
          break;
        }

        if (  front_end->demod->rsp->dd_status.dl  == Si2164B_DD_STATUS_RESPONSE_DL_LOCKED   ) {
          /* Return 1 to signal that the Si2164B is locked on a valid DVB-T/T2 channel */
          SiTRACE("Si2164B_lock_to_carrier: locked on a %s signal\n", Si2164B_standardName(front_end->demod->rsp->dd_status.modulation) );
          lock = 1;
          /* Make sure FEF mode is ON when locked on a T2 channel */
          if   (front_end->demod->rsp->dd_status.modulation == Si2164B_DD_MODE_PROP_MODULATION_DVBT2) {
           #ifdef    INDIRECT_I2C_CONNECTION
            front_end->f_TER_tuner_enable(front_end->callback);
           #else  /* INDIRECT_I2C_CONNECTION */
            Si2164B_L2_Tuner_I2C_Enable(front_end);
           #endif /* INDIRECT_I2C_CONNECTION */
            Si2164B_L2_TER_FEF(front_end, 1);
           #ifdef    INDIRECT_I2C_CONNECTION
            front_end->f_TER_tuner_disable(front_end->callback);
           #else  /* INDIRECT_I2C_CONNECTION */
            Si2164B_L2_Tuner_I2C_Disable(front_end);
           #endif /* INDIRECT_I2C_CONNECTION */
          }
          goto exit_lock;
        } else {
          if (  front_end->demod->rsp->dd_status.rsqint_bit5 == Si2164B_DD_STATUS_RESPONSE_RSQINT_BIT5_CHANGED ) {
          /* Return 0 if firmware signals 'no DVB-T/T2 channel' */
          SiTRACE ("'no DVB-T/T2 channel': not locked after %7d ms\n", searchDelay);
          goto exit_lock;
          }
        }
        break;
      }
      case Si2164B_DD_MODE_PROP_MODULATION_DSS   :
      case Si2164B_DD_MODE_PROP_MODULATION_DVBS  : {
        return_code = Si2164B_L1_DD_STATUS  (front_end->demod, Si2164B_DD_STATUS_CMD_INTACK_CLEAR);
        if (return_code != NO_Si2164B_ERROR) {
          SiTRACE("Si2164B_lock_to_carrier: Si2164B_L1_DD_STATUS error\n");
          SiERROR("Si2164B_lock_to_carrier: Si2164B_L1_DD_STATUS error\n");
          goto exit_lock;
          break;
        }

        if (front_end->demod->rsp->dd_status.dl == Si2164B_DD_STATUS_RESPONSE_DL_LOCKED) {
          /* Return 1 to signal that the Si2164B is locked on a valid DVB-S channel */
          lock = 1;
          goto exit_lock;
        }
        break;
      }
      case Si2164B_DD_MODE_PROP_MODULATION_DVBS2 : {
        return_code = Si2164B_L1_DD_STATUS(front_end->demod, Si2164B_DD_STATUS_CMD_INTACK_CLEAR);
        if (return_code != NO_Si2164B_ERROR) {
          SiTRACE("Si2164B_lock_to_carrier: Si2164B_L1_DD_STATUS error\n");
          SiERROR("Si2164B_lock_to_carrier: Si2164B_L1_DD_STATUS error\n");
          goto exit_lock;
          break;
        }
        if (front_end->demod->rsp->dd_status.dl == Si2164B_DD_STATUS_RESPONSE_DL_LOCKED) {
          /* Return 1 to signal that the Si2164B is locked on a valid DVB-S2 channel */
          lock = 1;
          goto exit_lock;
        }
        break;
      }
      case Si2164B_DD_MODE_PROP_MODULATION_DVBC  : {
        return_code = Si2164B_L1_DD_STATUS(front_end->demod, Si2164B_DD_STATUS_CMD_INTACK_CLEAR);

        if (return_code != NO_Si2164B_ERROR) {
          SiTRACE("Si2164B_lock_to_carrier: Si2164B_L1_DD_STATUS error\n");
          SiERROR("Si2164B_lock_to_carrier: Si2164B_L1_DD_STATUS error\n");
          goto exit_lock;
          break;
        }

        if ( (front_end->demod->rsp->dd_status.dl    == Si2164B_DD_STATUS_RESPONSE_DL_LOCKED     ) ) {
          /* Return 1 to signal that the Si2164B is locked on a valid SAT channel */
          SiTRACE("%s lock\n", Si2164B_standardName(front_end->demod->rsp->dd_status.modulation));
          lock = 1;
          goto exit_lock;
        }
        break;
      }
      case Si2164B_DD_MODE_PROP_MODULATION_DVBC2  : {
        return_code = Si2164B_L1_DVBC2_STATUS(front_end->demod, Si2164B_DVBC2_STATUS_CMD_INTACK_CLEAR);

        if (return_code != NO_Si2164B_ERROR) {
          SiTRACE("Si2164B_lock_to_carrier: Si2164B_DVBC2_STATUS error\n");
          SiERROR("Si2164B_lock_to_carrier: Si2164B_DVBC2_STATUS error\n");
          goto exit_lock;
          break;
        }

        SiTRACE("Si2164B_lock_to_carrier: Si2164B_DVBC2_STATUS PCL %d, DL %d, DVBC2_STATUS %d\n", front_end->demod->rsp->dvbc2_status.pcl, front_end->demod->rsp->dvbc2_status.dl, front_end->demod->rsp->dvbc2_status.dvbc2_status );
           if  (front_end->demod->rsp->dvbc2_status.dvbc2_status  == Si2164B_DVBC2_STATUS_RESPONSE_DVBC2_STATUS_IDLE        ) { SiTRACE("Si2164B_DVBC2_STATUS / DVBC2_STATUS %s\n", "IDLE        "); }
      else if  (front_end->demod->rsp->dvbc2_status.dvbc2_status  == Si2164B_DVBC2_STATUS_RESPONSE_DVBC2_STATUS_READY       ) { SiTRACE("Si2164B_DVBC2_STATUS / DVBC2_STATUS %s\n", "READY       "); }
      else if  (front_end->demod->rsp->dvbc2_status.dvbc2_status  == Si2164B_DVBC2_STATUS_RESPONSE_DVBC2_STATUS_SEARCHING   ) { SiTRACE("Si2164B_DVBC2_STATUS / DVBC2_STATUS %s\n", "SEARCHING   "); }
      else if  (front_end->demod->rsp->dvbc2_status.dvbc2_status  == Si2164B_DVBC2_STATUS_RESPONSE_DVBC2_STATUS_TUNE_REQUEST) { SiTRACE("Si2164B_DVBC2_STATUS / DVBC2_STATUS %s at %ld Hz\n", "--------------- TUNE_REQUEST -----------------", front_end->demod->rsp->dvbc2_status.rf_freq); }

        if ( (front_end->demod->rsp->dvbc2_status.dvbc2_status == Si2164B_DVBC2_STATUS_RESPONSE_DVBC2_STATUS_TUNE_REQUEST ) ) {
          /* Retune to lock to the selected C2 data slice */
          freq = front_end->demod->rsp->dvbc2_status.rf_freq;
          Si2164B_L2_Tune                  (front_end,       freq);
          startTime = system_time();
          front_end->demod->cmd->dvbc2_ctrl.action        = Si2164B_DVBC2_CTRL_CMD_ACTION_RESUME;
          front_end->demod->cmd->dvbc2_ctrl.tuned_rf_freq = freq;
          /* Resume C2 lock on the new frequency */
          return_code = Si2164B_L1_DVBC2_CTRL (front_end->demod, front_end->demod->cmd->dvbc2_ctrl.action , front_end->demod->cmd->dvbc2_ctrl.tuned_rf_freq);
          return_code = Si2164B_L1_DVBC2_CTRL (front_end->demod, front_end->demod->cmd->dvbc2_ctrl.action , front_end->demod->cmd->dvbc2_ctrl.tuned_rf_freq);
          if (return_code != NO_Si2164B_ERROR) {
            SiTRACE("Si2164B_lock_to_carrier: Si2164B_L1_DVBC2_CTRL error (dvbc2_ctrl/RESUME)\n");
            SiERROR("Si2164B_lock_to_carrier: Si2164B_L1_DVBC2_CTRL error (dvbc2_ctrl/RESUME)\n");
            goto exit_lock;
            break;
          }
          SiTRACE   ("Si2164B_lock_to_carrier 'dvbc2_ctrl/RESUME' at %d took %3d ms\n", freq, system_time() - startTime);
          /* Reset the lock start reference */
          front_end->searchStartTime = system_time();
        } else {
          if ( (front_end->demod->rsp->dvbc2_status.pcl   == Si2164B_DVBC2_STATUS_RESPONSE_PCL_LOCKED    ) ) {
            Si2164B_L1_DD_STATUS     (front_end->demod, Si2164B_DD_STATUS_CMD_INTACK_CLEAR);
            return_code = Si2164B_L1_DVBC2_SYS_INFO(front_end->demod);
            if (return_code != NO_Si2164B_ERROR) {
              SiTRACE("Si2164B_lock_to_carrier: Si2164B_L1_DVBC2_SYS_INFO error \n");
              SiERROR("Si2164B_lock_to_carrier: Si2164B_L1_DVBC2_SYS_INFO error \n");
              goto exit_lock;
              break;
            }
            SiTRACE("%s L1 Signalling found at %d, start_frequency_hz %ld\n", Si2164B_standardName(front_end->demod->rsp->dd_status.modulation), freq , front_end->demod->rsp->dvbc2_sys_info.start_frequency_hz);
          }
          if ( (front_end->demod->rsp->dvbc2_status.dl    == Si2164B_DVBC2_STATUS_RESPONSE_DL_LOCKED     ) ) {
            Si2164B_L1_DD_STATUS(front_end->demod, Si2164B_DD_STATUS_CMD_INTACK_CLEAR);
            /* Return 1 to signal that the Si2164B is locked on a valid C2 channel */
            SiTRACE("%s lock on DS_ID %3d, PLP_ID %3d at %d\n", Si2164B_standardName(front_end->demod->rsp->dd_status.modulation), front_end->demod->rsp->dvbc2_status.ds_id, front_end->demod->rsp->dvbc2_status.plp_id, freq );
            lock = 1;
            goto exit_lock;
          }
        }
        break;
      }
      case Si2164B_DD_MODE_PROP_MODULATION_MCNS  : {
        return_code = Si2164B_L1_DD_STATUS(front_end->demod, Si2164B_DD_STATUS_CMD_INTACK_CLEAR);

        if (return_code != NO_Si2164B_ERROR) {
          SiTRACE("Si2164B_lock_to_carrier: Si2164B_L1_DD_STATUS error\n");
          SiERROR("Si2164B_lock_to_carrier: Si2164B_L1_DD_STATUS error\n");
          goto exit_lock;
          break;
        }

        if ( (front_end->demod->rsp->dd_status.dl    == Si2164B_DD_STATUS_RESPONSE_DL_LOCKED     ) ) {
          /* Return 1 to signal that the Si2164B is locked on a valid SAT channel */
          SiTRACE("%s lock\n", Si2164B_standardName(front_end->demod->rsp->dd_status.modulation));
          lock = 1;
          goto exit_lock;
        }
        break;
      }
    }

    /* timeout management (this should never happen if timeout values are correctly set) */
    searchDelay = system_time() - front_end->searchStartTime;
    if (searchDelay >= max_lock_time_ms) {
      SiTRACE ("Si2164B_lock_to_carrier timeout(%d) after %d ms\n", max_lock_time_ms, searchDelay);
      goto exit_lock;
      break;
    }

    if (front_end->handshakeUsed == 1) {
      handshakeDelay = system_time() - lockStartTime;
      if (handshakeDelay >= front_end->handshakePeriod_ms) {
        SiTRACE ("lock_to_carrier_handshake : handshake after %5d ms (at %10d). (search delay %6d ms)\n\n", handshakeDelay, freq, searchDelay);
        front_end->handshakeOn = 1;
        /* The application will check handshakeStart_ms to know whether the lock is complete or not */
        return searchDelay;
      } else {
        SiTRACE ("lock_to_carrier_handshake : no handshake yet. (handshake delay %6d ms, search delay %6d ms)\n", handshakeDelay, searchDelay);
      }
    }

    if (front_end->lockAbort) {
      SiTRACE("Si2164B_L2_lock_to_carrier : lock aborted after %d ms.\n", system_time() - lockStartTime);
      goto exit_lock;
    }

    /* Check status every 5 ms */
    system_wait(5);
  }

  exit_lock:

  front_end->handshakeOn = 0;
  searchDelay = system_time() - front_end->searchStartTime;

  if (lock) {
    Si2164B_L1_DD_BER(front_end->demod, Si2164B_DD_BER_CMD_RST_CLEAR);
    SiTRACE ("Si2164B_lock_to_carrier 'lock'  took %3d ms\n"        , searchDelay);
  } else {
    SiTRACE ("Si2164B_lock_to_carrier at %10d (%s) failed after %d ms\n",freq, Si2164B_standardName(front_end->demod->rsp->dd_status.modulation), searchDelay);
  }
#endif//kwon

  return lock;
}
/************************************************************************************************************************
  Si2164B_L2_Tune function
  Use:        tuner current frequency retrieval function
              Used to retrieve the current RF from the tuner's driver.
  Porting:    Replace the internal TUNER function calls by the final tuner's corresponding calls
  Comments:   If the tuner is connected via the demodulator's I2C switch, enabling/disabling the i2c_passthru is required before/after tuning.
  Behavior:   This function closes the Si2164B's I2C switch then tunes and finally reopens the I2C switch
  Parameter:  *front_end, the front-end handle
  Parameter:  rf, the frequency to tune at
  Returns:    rf
************************************************************************************************************************/
signed int  Si2164B_L2_Tune               (Si2164B_L2_Context *front_end, signed int rf)
{
#ifdef    TUNERTER_API
  #define Si2164B_USING_SILABS_TER_TUNER
#else  /* TUNERTER_API */
  #ifdef    SILABS_TER_TUNER_API
    #define Si2164B_USING_SILABS_TER_TUNER
  #endif /* SILABS_TER_TUNER_API */
#endif /* TUNERTER_API */

#ifdef    Si2164B_USING_SILABS_TER_TUNER
    unsigned char bw;
    unsigned char modulation;
#endif /* Si2164B_USING_SILABS_TER_TUNER */

    SiTRACE("Si2164B_L2_Tune at %d\n",rf);

 #ifdef    INDIRECT_I2C_CONNECTION
  /*  I2C connection will be done later on, depending on the media */
 #else  /* INDIRECT_I2C_CONNECTION */
    Si2164B_L2_Tuner_I2C_Enable(front_end);
 #endif /* INDIRECT_I2C_CONNECTION */

    if (front_end->demod->media == Si2164B_TERRESTRIAL) {
 #ifdef    INDIRECT_I2C_CONNECTION
      front_end->f_TER_tuner_enable(front_end->callback);
 #endif /* INDIRECT_I2C_CONNECTION */
      Si2164B_L2_TER_FEF (front_end, 0);

	  /* 20151130 add ReturnLoss Start */
	  if (front_end->demod->prop->dd_mode.modulation == Si2164B_DD_MODE_PROP_MODULATION_DVBC  ) {
		front_end->tuner_ter->prop->tuner_return_loss.config = Si2151_TUNER_RETURN_LOSS_PROP_CONFIG_127; /* (default '127') */
		front_end->tuner_ter->prop->tuner_return_loss.mode     = Si2151_TUNER_RETURN_LOSS_PROP_MODE_CABLE; /* (default 'TERRESTRIAL') */
      } else {
		front_end->tuner_ter->prop->tuner_return_loss.config = Si2151_TUNER_RETURN_LOSS_PROP_CONFIG_127; /* (default '127') */
		front_end->tuner_ter->prop->tuner_return_loss.mode     = Si2151_TUNER_RETURN_LOSS_PROP_MODE_TERRESTRIAL; /* (default 'TERRESTRIAL') */
	  }
	  Si2151_L1_SetProperty2(front_end->tuner_ter, Si2151_TUNER_RETURN_LOSS_PROP);
	  /* 20151130 add ReturnLoss End */

#ifdef    Si2164B_USING_SILABS_TER_TUNER
      if (front_end->demod->prop->dd_mode.modulation == Si2164B_DD_MODE_PROP_MODULATION_DVBC  ) {
        modulation = L1_RF_TER_TUNER_MODULATION_DVBC;
        bw         = 8;
      } else {
        modulation = L1_RF_TER_TUNER_MODULATION_DVBT;
        switch (front_end->demod->prop->dd_mode.bw) {
          case Si2164B_DD_MODE_PROP_BW_BW_1D7MHZ   : bw = 6; break;
          case Si2164B_DD_MODE_PROP_BW_BW_5MHZ     : bw = 6; break;
          case Si2164B_DD_MODE_PROP_BW_BW_6MHZ     : bw = 6; break;
          case Si2164B_DD_MODE_PROP_BW_BW_7MHZ     : bw = 7; break;
          case Si2164B_DD_MODE_PROP_BW_BW_8MHZ     : bw = 8; break;
          default: {
            SiTRACE("Si2164B_L2_Tune: Invalid dd_mode.bw (%d)\n", front_end->demod->prop->dd_mode.bw);
            SiERROR("Si2164B_L2_Tune: Invalid dd_mode.bw\n");
            bw = 8; break;
          }
        }
      }
#if 0 // 20150527 temp
      if (front_end->demod->prop->dd_mode.modulation == Si2164B_DD_MODE_PROP_MODULATION_DVBT  ) {
          SiLabs_TER_Tuner_DTV_INTERNAL_ZIF_DVBT(front_end->tuner_ter, 0); /* use default value for field DVBT of tuner property DTV_INTERNAL_ZIF */
      }
      if (front_end->demod->prop->dd_mode.modulation == Si2164B_DD_MODE_PROP_MODULATION_DVBT2 ) {
        if (front_end->demod->prop->dd_mode.bw == Si2164B_DD_MODE_PROP_BW_BW_1D7MHZ ) {
          SiLabs_TER_Tuner_DTV_INTERNAL_ZIF_DVBT(front_end->tuner_ter, 1); /* field DVBT of tuner property DTV_INTERNAL_ZIF set to LIF */
        } else {
          SiLabs_TER_Tuner_DTV_INTERNAL_ZIF_DVBT(front_end->tuner_ter, 0); /* use default value for field DVBT of tuner property DTV_INTERNAL_ZIF */
        }
      }
      if (front_end->demod->prop->dd_mode.modulation == Si2164B_DD_MODE_PROP_MODULATION_DVBC2 ) {
          SiLabs_TER_Tuner_DTV_INTERNAL_ZIF_DVBT(front_end->tuner_ter, 1); /* field DVBT of tuner property DTV_INTERNAL_ZIF set to LIF */
      }
#endif
#endif /* Si2164B_USING_SILABS_TER_TUNER */
      //L1_RF_TER_TUNER_Tune       (front_end->tuner_ter , rf);  // jhkwon_temp
      /* Activate FEF management in all cases where the signal can be DVB-T2 */
      if (   (front_end->demod->prop->dd_mode.modulation  == Si2164B_DD_MODE_PROP_MODULATION_DVBT2)
          | ((front_end->demod->prop->dd_mode.modulation  == Si2164B_DD_MODE_PROP_MODULATION_AUTO_DETECT)
            &(front_end->demod->prop->dd_mode.auto_detect == Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_T_T2))
         ) { Si2164B_L2_TER_FEF (front_end, 1); }
 #ifdef    INDIRECT_I2C_CONNECTION
      front_end->f_TER_tuner_disable(front_end->callback);
 #endif /* INDIRECT_I2C_CONNECTION */
    }

    if (front_end->demod->media == Si2164B_SATELLITE  ) {
 #ifdef    INDIRECT_I2C_CONNECTION
      front_end->f_SAT_tuner_enable(front_end->callback);
 #endif /* INDIRECT_I2C_CONNECTION */
#ifdef    UNICABLE_COMPATIBLE
      if (front_end->lnb_type == UNICABLE_LNB_TYPE_UNICABLE) {
        rf = front_end->f_Unicable_tune(front_end->callback, rf);
        SiTRACE(" Unicable L2_TUNE returning rf=%d\n",rf);
      } else {
#endif /* UNICABLE_COMPATIBLE */
#if(!SEC_T2_TO_CHECK_02)
        L1_RF_SAT_TUNER_Tune (front_end->tuner_sat, rf);
#endif
#ifdef    UNICABLE_COMPATIBLE
      }
#endif /* UNICABLE_COMPATIBLE */
 #ifdef    INDIRECT_I2C_CONNECTION
      front_end->f_SAT_tuner_disable(front_end->callback);
 #endif /* INDIRECT_I2C_CONNECTION */
    }

   #ifdef    INDIRECT_I2C_CONNECTION
   #else  /* INDIRECT_I2C_CONNECTION */
    Si2164B_L2_Tuner_I2C_Disable(front_end);
   #endif /* INDIRECT_I2C_CONNECTION */

    return rf;
}
/************************************************************************************************************************
  Si2164B_L2_Get_RF function
  Use:        tuner current frequency retrieval function
              Used to retrieve the current RF from the tuner's driver.
  Porting:    Replace the internal TUNER function calls by the final tuner's corresponding calls
  Behavior:   This function does not need to activate the Si2164B's I2C switch, as the required value is part of the tuner's structure
  Parameter:  *front_end, the front-end handle
************************************************************************************************************************/
signed int  Si2164B_L2_Get_RF             (Si2164B_L2_Context *front_end) {

  if (front_end->demod->media == Si2164B_TERRESTRIAL) {
#ifdef    TUNERTER_API
  #ifdef   TUNERTER_SILABS_API
    return (signed int)L1_RF_TER_TUNER_Get_RF (front_end->tuner_ter);
  #else  /* TUNERTER_SILABS_API */
    return front_end->tuner_ter->cmd->tuner_tune_freq.freq;
  #endif /* TUNERTER_SILABS_API */
#else  /* TUNERTER_API */
    return (signed int)L1_RF_TER_TUNER_Get_RF (front_end->tuner_ter);
#endif /* TUNERTER_API */
  }

  if (front_end->demod->media == Si2164B_SATELLITE  ) {
    return (signed int)L1_RF_SAT_TUNER_Get_RF (front_end->tuner_sat);
  }

  return 0;
}
/************************************************************************************************************************
  Si2164B_L2_TER_Clock function
  Use:        Terrestrial clock configuration function
              Used to set the terrestrial clock source, input pin and frequency
  Parameter:  clock_source:  where the clock signal comes from.
              possible sources:
                Si2164B_Xtal_clock,
                Si2164B_TER_Tuner_clock,
                Si2164B_SAT_Tuner_clock
  Parameter:  clock_input:   on which pin is the clock received.
              possible inputs:
                44 for Si2164B_START_CLK_CMD_CLK_MODE_CLK_CLKIO    (pin 44 for 'single' parts)
                33 for Si2164B_START_CLK_CMD_CLK_MODE_CLK_XTAL_IN  (pin 33 for 'single' parts)
                32 for Si2164B_START_CLK_CMD_CLK_MODE_XTAL         (Xtal connected on pins 32/33 for 'single' parts, driven by the Si264)
  Parameter:  clock_freq:   clock frequency
              possible frequencies:
                Si2164B_POWER_UP_CMD_CLOCK_FREQ_CLK_16MHZ
                Si2164B_POWER_UP_CMD_CLOCK_FREQ_CLK_24MHZ
                Si2164B_POWER_UP_CMD_CLOCK_FREQ_CLK_27MHZ
  Parameter:  clock_control:   how the TER clock must be controlled
              possible modes:
                Si2164B_CLOCK_ALWAYS_ON                             turn clock ON then never switch it off, used when the clock is provided to another part
                Si2164B_CLOCK_ALWAYS_OFF                            never switch it on, used when the clock is not going anywhere
                Si2164B_CLOCK_MANAGED                               control clock state as before
 ***********************************************************************************************************************/
signed int  Si2164B_L2_TER_Clock          (Si2164B_L2_Context *front_end, signed int clock_source, signed int clock_input, signed int clock_freq, signed int clock_control)
{
  front_end->demod->tuner_ter_clock_source = clock_source;
  switch (clock_input) {
    case 44: {
      front_end->demod->tuner_ter_clock_input  = Si2164B_START_CLK_CMD_CLK_MODE_CLK_CLKIO;
      break;
    }
    case 33: {
      front_end->demod->tuner_ter_clock_input  = Si2164B_START_CLK_CMD_CLK_MODE_CLK_XTAL_IN;
      break;
    }
    default:
    case 32: {
      front_end->demod->tuner_ter_clock_input  = Si2164B_START_CLK_CMD_CLK_MODE_XTAL;
      break;
    }
  }
  front_end->demod->tuner_ter_clock_freq   = clock_freq;
  front_end->demod->tuner_ter_clock_control= clock_control;
  return NO_Si2164B_ERROR;
}
/************************************************************************************************************************
  Si2164B_L2_TER_AGC function
  Use:        Terrestrial AGC configuration function
              Used to set the terrestrial AGC source, input pin and frequency
  Parameter:  agc1_mode:  where the AGC 1 PWM comes from.
              possible modes:
                0x0: Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_NOT_USED,
                0xA: Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_MP_A,
                0xB: Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_MP_B,
                0xC: Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_MP_C,
                0xD: Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_MP_D,
  Parameter:  agc1_inversion: 0/1 to indicate if AGC 1 is inverted or not (depends on the TER tuner and HW design)
  Parameter:  agc2_mode:  where the AGC 2 PWM comes from.
              possible modes:
                0x0: Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MODE_NOT_USED,
                0xA: Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MODE_MP_A,
                0xB: Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MODE_MP_B,
                0xC: Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MODE_MP_C,
                0xD: Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MODE_MP_D,
  Parameter:  agc2_inversion: 0/1 to indicate if AGC 2 is inverted or not (depends on the TER tuner and HW design)
  Returns:    0 if no error
 ***********************************************************************************************************************/
signed int  Si2164B_L2_TER_AGC            (Si2164B_L2_Context *front_end, signed int agc1_mode, signed int agc1_inversion, signed int agc2_mode, signed int agc2_inversion)
{
  switch (agc1_mode) {
    default:
    case 0x0: { front_end->demod->cmd->dd_ext_agc_ter.agc_1_mode  = Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_NOT_USED; break; }
    case 0xA: { front_end->demod->cmd->dd_ext_agc_ter.agc_1_mode  = Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_MP_A    ; break; }
    case 0xB: { front_end->demod->cmd->dd_ext_agc_ter.agc_1_mode  = Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_MP_B    ; break; }
    case 0xC: { front_end->demod->cmd->dd_ext_agc_ter.agc_1_mode  = Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_MP_C    ; break; }
    case 0xD: { front_end->demod->cmd->dd_ext_agc_ter.agc_1_mode  = Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_MP_D    ; break; }
  }

  if (agc1_inversion == 0) {
                front_end->demod->cmd->dd_ext_agc_ter.agc_1_inv   = Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_INV_NOT_INVERTED;}
   else {       front_end->demod->cmd->dd_ext_agc_ter.agc_1_inv   = Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_INV_INVERTED;    }

  switch (agc2_mode) {
    default:
    case 0x0: { front_end->demod->cmd->dd_ext_agc_ter.agc_2_mode  = Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MODE_NOT_USED; break; }
    case 0xA: { front_end->demod->cmd->dd_ext_agc_ter.agc_2_mode  = Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MODE_MP_A    ; break; }
    case 0xB: { front_end->demod->cmd->dd_ext_agc_ter.agc_2_mode  = Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MODE_MP_B    ; break; }
    case 0xC: { front_end->demod->cmd->dd_ext_agc_ter.agc_2_mode  = Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MODE_MP_C    ; break; }
    case 0xD: { front_end->demod->cmd->dd_ext_agc_ter.agc_2_mode  = Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MODE_MP_D    ; break; }
  }
  if (agc2_inversion == 0) {
                front_end->demod->cmd->dd_ext_agc_ter.agc_2_inv   = Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_INV_NOT_INVERTED;}
  else {        front_end->demod->cmd->dd_ext_agc_ter.agc_2_inv   = Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_INV_INVERTED;}

  if ( (agc1_mode == 0) & (agc2_mode == 0) ) {
    front_end->demod->tuner_ter_agc_control = 0;
  } else {
    front_end->demod->tuner_ter_agc_control = 1;
  }

  return NO_Si2164B_ERROR;
}
/************************************************************************************************************************
  Si2164B_L2_TER_FEF_CONFIG function
  Use:        TER tuner FEF pin selection function
              Used to select the FEF pin connected to the terrestrial tuner
  Parameter:  *front_end, the front-end handle
  Parameter:  fef_mode, a flag controlling the FEF mode between SLOW_NORMAL_AGC(0), FREEZE_PIN(1)' and SLOW_INITIAL_AGC(2)
  Parameter:  fef_pin: where the FEF signal comes from.
              possible values:
                0x0: Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_NOT_USED
                0xA: Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_MP_A
                0xB: Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_MP_B
                0xC: Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_MP_C
                0xD: Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_MP_D
  Parameter:  fef_level, a flag controlling the FEF signal level when active between 'low'(0) and 'high'(1)
  Returns:    1
************************************************************************************************************************/
signed int   Si2164B_L2_TER_FEF_CONFIG    (Si2164B_L2_Context   *front_end, signed int fef_mode, signed int fef_pin, signed int fef_level)
{
  switch (fef_mode) {
    default :
    case   0: { front_end->demod->fef_mode = Si2164B_FEF_MODE_SLOW_NORMAL_AGC ; front_end->demod->fef_pin = 0x0    ; front_end->demod->fef_level = 0        ; break; }
    case   1: { front_end->demod->fef_mode = Si2164B_FEF_MODE_FREEZE_PIN      ; front_end->demod->fef_pin = fef_pin; front_end->demod->fef_level = fef_level; break; }
    case   2: { front_end->demod->fef_mode = Si2164B_FEF_MODE_SLOW_INITIAL_AGC; front_end->demod->fef_pin = 0x0    ; front_end->demod->fef_level = 0        ; break; }
  }
  switch (front_end->demod->fef_pin) {
    default  :
    case 0x0: { front_end->demod->cmd->dvbt2_fef.fef_tuner_flag  = Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_NOT_USED; break; }
    case 0xA: { front_end->demod->cmd->dvbt2_fef.fef_tuner_flag  = Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_MP_A    ; break; }
    case 0xB: { front_end->demod->cmd->dvbt2_fef.fef_tuner_flag  = Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_MP_B    ; break; }
    case 0xC: { front_end->demod->cmd->dvbt2_fef.fef_tuner_flag  = Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_MP_C    ; break; }
    case 0xD: { front_end->demod->cmd->dvbt2_fef.fef_tuner_flag  = Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_MP_D    ; break; }
  }
  if (front_end->demod->fef_level == 0) {
                front_end->demod->cmd->dvbt2_fef.fef_tuner_flag_inv  = Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_INV_FEF_LOW ; }
   else {       front_end->demod->cmd->dvbt2_fef.fef_tuner_flag_inv  = Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_INV_FEF_HIGH; }

  return NO_Si2164B_ERROR;
}
/************************************************************************************************************************
  Si2164B_L2_TER_FEF function
  Use:        TER tuner FEF activation function
              Used to enable/disable the FEF mode in the terrestrial tuner
  Comments:   If the tuner is connected via the demodulator's I2C switch, enabling/disabling the i2c_passthru is required before/after tuning.
  Parameter:  *front_end, the front-end handle
  Parameter:  fef, a flag controlling the selection between FEF 'off'(0) and FEF 'on'(1)
  Returns:    1
************************************************************************************************************************/
signed int  Si2164B_L2_TER_FEF            (Si2164B_L2_Context *front_end, signed int fef)
{
  front_end = front_end; /* To avoid compiler warning if not used */
  SiTRACE("Si2164B_L2_TER_FEF %d \n",fef);

  #ifdef    L1_RF_TER_TUNER_DTV_AGC_AUTO_FREEZE
    SiTRACE("TER tuner: AUTO_AGC_FREEZE\n");
    L1_RF_TER_TUNER_DTV_AGC_AUTO_FREEZE(front_end->tuner_ter,fef);
  #endif /* L1_RF_TER_TUNER_DTV_AGC_AUTO_FREEZE */

  #ifdef    L1_RF_TER_TUNER_FEF_MODE_FREEZE_PIN
  if (front_end->demod->fef_mode == Si2164B_FEF_MODE_FREEZE_PIN      ) {
    SiTRACE("FEF mode Si2164B_FEF_MODE_FREEZE_PIN\n");
    //L1_RF_TER_TUNER_FEF_MODE_FREEZE_PIN(front_end->tuner_ter, fef);
  }
  #endif /* L1_RF_TER_TUNER_FEF_MODE_FREEZE_PIN */

  #ifdef    L1_RF_TER_TUNER_FEF_MODE_SLOW_INITIAL_AGC_SETUP
  if (front_end->demod->fef_mode == Si2164B_FEF_MODE_SLOW_INITIAL_AGC) {
    SiTRACE("FEF mode Si2164B_FEF_MODE_SLOW_INITIAL_AGC (AGC slowed down after tuning)\n");
  }
  #endif /* L1_RF_TER_TUNER_FEF_MODE_SLOW_INITIAL_AGC_SETUP */

  #ifdef    L1_RF_TER_TUNER_FEF_MODE_SLOW_NORMAL_AGC_SETUP
  if (front_end->demod->fef_mode == Si2164B_FEF_MODE_SLOW_NORMAL_AGC ) {
    SiTRACE("FEF mode Si2164B_FEF_MODE_SLOW_NORMAL_AGC: AGC slowed down\n");
    L1_RF_TER_TUNER_FEF_MODE_SLOW_NORMAL_AGC(front_end->tuner_ter, fef);
  }
  #endif /* L1_RF_TER_TUNER_FEF_MODE_SLOW_NORMAL_AGC_SETUP */
  SiTRACE("Si2164B_L2_TER_FEF done\n");
  return 1;
}
/************************************************************************************************************************
  Si2164B_L2_TER_FEF_SETUP function
  Use:        TER tuner LPF setting function
              Used to configure the FEF mode in the terrestrial tuner
  Comments:   If the tuner is connected via the demodulator's I2C switch, enabling/disabling the i2c_passthru is required before/after tuning.
  Behavior:   This function closes the Si2164B's I2C switch then sets the TER FEF mode and finally reopens the I2C switch
  Parameter:  *front_end, the front-end handle
  Parameter:  fef, a flag controlling the selection between FEF 'off'(0) and FEF 'on'(1)
  Returns:    1
************************************************************************************************************************/
signed int  Si2164B_L2_TER_FEF_SETUP      (Si2164B_L2_Context *front_end, signed int fef)
{
  SiTRACE("Si2164B_L2_TER_FEF_SETUP %d\n",fef);

  #ifdef    L1_RF_TER_TUNER_FEF_MODE_FREEZE_PIN_SETUP
  if (front_end->demod->fef_mode == Si2164B_FEF_MODE_FREEZE_PIN      ) {
    SiTRACE("FEF mode Si2164B_FEF_MODE_FREEZE_PIN\n");
//    L1_RF_TER_TUNER_FEF_MODE_FREEZE_PIN_SETUP(front_end->tuner_ter);
  }
  #endif /* L1_RF_TER_TUNER_FEF_MODE_FREEZE_PIN_SETUP */

  #ifdef    L1_RF_TER_TUNER_FEF_MODE_SLOW_INITIAL_AGC_SETUP
  if (front_end->demod->fef_mode == Si2164B_FEF_MODE_SLOW_INITIAL_AGC) {
    SiTRACE("FEF mode Si2164B_FEF_MODE_SLOW_INITIAL_AGC (AGC slowed down after tuning)\n");
    L1_RF_TER_TUNER_FEF_MODE_SLOW_INITIAL_AGC_SETUP(front_end->tuner_ter, fef);
  }
  #endif /* L1_RF_TER_TUNER_FEF_MODE_SLOW_INITIAL_AGC_SETUP */

  #ifdef    L1_RF_TER_TUNER_FEF_MODE_SLOW_NORMAL_AGC_SETUP
  if (front_end->demod->fef_mode == Si2164B_FEF_MODE_SLOW_NORMAL_AGC ) {
    SiTRACE("FEF mode Si2164B_FEF_MODE_SLOW_NORMAL_AGC: AGC slowed down\n");
    L1_RF_TER_TUNER_FEF_MODE_SLOW_NORMAL_AGC_SETUP(front_end->tuner_ter, fef);
  }
  #endif /* L1_RF_TER_TUNER_FEF_MODE_SLOW_NORMAL_AGC */

  Si2164B_L2_TER_FEF(front_end, fef);

  SiTRACE("Si2164B_L2_TER_FEF_SETUP done\n");
  return 1;
}
/************************************************************************************************************************
  Si2164B_L2_SAT_Clock function
  Use:        Satellite clock configuration function
              Used to set the satellite clock source, input pin and frequency
  Returns:    0 if no error
  Parameter:  clock_source:  where the clock signal comes from.
              possible sources:
                Si2164B_Xtal_clock,
                Si2164B_TER_Tuner_clock,
                Si2164B_SAT_Tuner_clock
  Parameter:  clock_input:   on which pin is the clock received.
              possible inputs:
                44 for Si2164B_START_CLK_CMD_CLK_MODE_CLK_CLKIO    (pin 44 for 'single' parts)
                33 for Si2164B_START_CLK_CMD_CLK_MODE_CLK_XTAL_IN  (pin 33 for 'single' parts)
                32 for Si2164B_START_CLK_CMD_CLK_MODE_XTAL         (Xtal connected on pins 32/33 for 'single' parts, driven by the Si264)
  Parameter:  clock_freq:   clock frequency
              possible frequencies:
                Si2164B_POWER_UP_CMD_CLOCK_FREQ_CLK_16MHZ
                Si2164B_POWER_UP_CMD_CLOCK_FREQ_CLK_24MHZ
                Si2164B_POWER_UP_CMD_CLOCK_FREQ_CLK_27MHZ
  Parameter:  clock_control:   how the SAT clock must be controlled
              possible modes:
                Si2164B_CLOCK_ALWAYS_ON                             turn clock ON then never switch it off, used when the clock is provided to another part
                Si2164B_CLOCK_ALWAYS_OFF                            never switch it on, used when the clock is not going anywhere
                Si2164B_CLOCK_MANAGED                               control clock state as before
 ***********************************************************************************************************************/
signed int  Si2164B_L2_SAT_Clock          (Si2164B_L2_Context *front_end, signed int clock_source, signed int clock_input, signed int clock_freq, signed int clock_control)
{
  front_end->demod->tuner_sat_clock_source = clock_source;
  front_end->demod->tuner_sat_clock_input  = Si2164B_START_CLK_CMD_CLK_MODE_XTAL;
  switch (clock_input) {
    case 44: { /* Si2164B_START_CLK_CMD_CLK_MODE_CLK_CLKIO   */
      front_end->demod->tuner_sat_clock_input  = Si2164B_START_CLK_CMD_CLK_MODE_CLK_CLKIO;
      break;
    }
    case 33: { /* Si2164B_START_CLK_CMD_CLK_MODE_CLK_XTAL_IN */
      front_end->demod->tuner_sat_clock_input  = Si2164B_START_CLK_CMD_CLK_MODE_CLK_XTAL_IN;
      break;
    }
    default:
    case 32: { /* Si2164B_START_CLK_CMD_CLK_MODE_XTAL        */
      front_end->demod->tuner_sat_clock_input  = Si2164B_START_CLK_CMD_CLK_MODE_XTAL;
      break;
    }
  }
  front_end->demod->tuner_sat_clock_freq   = clock_freq;
  front_end->demod->tuner_sat_clock_control= clock_control;
  return NO_Si2164B_ERROR;
}
/************************************************************************************************************************
  Si2164B_L2_SAT_AGC function
  Use:        Satellite AGC configuration function
              Used to set the Satellite AGC source, input pin and frequency
  Parameter:  agc1_mode:  where the AGC 1 PWM comes from.
              possible modes:
                0x0: Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MODE_NOT_USED,
                0xA: Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MODE_MP_A,
                0xB: Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MODE_MP_B,
                0xC: Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MODE_MP_C,
                0xD: Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MODE_MP_D,
  Parameter:  agc1_inversion: 0/1 to indicate if AGC 1 is inverted or not (depends on the SAT tuner and HW design)
  Parameter:  agc2_mode:  where the AGC 2 PWM comes from.
              possible modes:
                0x0: Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MODE_NOT_USED,
                0xA: Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MODE_MP_A,
                0xB: Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MODE_MP_B,
                0xC: Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MODE_MP_C,
                0xD: Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MODE_MP_D,
  Parameter:  agc2_inversion: 0/1 to indicate if AGC 2 is inverted or not (depends on the SAT tuner and HW design)
  Returns:    0 if no error
 ***********************************************************************************************************************/
signed int  Si2164B_L2_SAT_AGC            (Si2164B_L2_Context *front_end, signed int agc1_mode, signed int agc1_inversion, signed int agc2_mode, signed int agc2_inversion)
{
  switch (agc1_mode) {
    default  :
    case 0x0: { front_end->demod->cmd->dd_ext_agc_sat.agc_1_mode  = Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MODE_NOT_USED; break; }
    case 0xA: { front_end->demod->cmd->dd_ext_agc_sat.agc_1_mode  = Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MODE_MP_A    ; break; }
    case 0xB: { front_end->demod->cmd->dd_ext_agc_sat.agc_1_mode  = Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MODE_MP_B    ; break; }
    case 0xC: { front_end->demod->cmd->dd_ext_agc_sat.agc_1_mode  = Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MODE_MP_C    ; break; }
    case 0xD: { front_end->demod->cmd->dd_ext_agc_sat.agc_1_mode  = Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MODE_MP_D    ; break; }
  }
  if (agc1_inversion == 0) {
                front_end->demod->cmd->dd_ext_agc_sat.agc_1_inv   = Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_INV_NOT_INVERTED; }
  else {        front_end->demod->cmd->dd_ext_agc_sat.agc_1_inv   = Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_INV_INVERTED;     }

  switch (agc2_mode) {
    default  :
    case 0x0: { front_end->demod->cmd->dd_ext_agc_sat.agc_2_mode  = Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MODE_NOT_USED; break; }
    case 0xA: { front_end->demod->cmd->dd_ext_agc_sat.agc_2_mode  = Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MODE_MP_A    ; break; }
    case 0xB: { front_end->demod->cmd->dd_ext_agc_sat.agc_2_mode  = Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MODE_MP_B    ; break; }
    case 0xC: { front_end->demod->cmd->dd_ext_agc_sat.agc_2_mode  = Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MODE_MP_C    ; break; }
    case 0xD: { front_end->demod->cmd->dd_ext_agc_sat.agc_2_mode  = Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MODE_MP_D    ; break; }
  }
  if (agc2_inversion == 0) {
                front_end->demod->cmd->dd_ext_agc_sat.agc_2_inv   = Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_INV_NOT_INVERTED; }
  else {        front_end->demod->cmd->dd_ext_agc_sat.agc_2_inv   = Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_INV_INVERTED;     }

  return NO_Si2164B_ERROR;
}
/************************************************************************************************************************
  Si2164B_L2_SAT_Spectrum function
  Use:        Satellite IF configuration function
              Used to store the satellite spectrum inversion flag
  Parameter:  spectrum_inversion:   a flag indicating if the SAT signal appears inverted.
               This situation is possible if there is an I/Q swap in the HW design, for easier routing
               It is perfectly OK to do that to route easily, but the SW needs to be informed of this inversion.
               NB: The additional inversion added by Unicable equipement should not be taken into account here
                    The spectrum_inversion set here corresponds to the 'normal' mode
  Returns:    0 if no error
 ***********************************************************************************************************************/
signed int  Si2164B_L2_SAT_Spectrum       (Si2164B_L2_Context *front_end, signed int spectrum_inversion)
{
  SiTRACE("Si2164B_L2_SAT_Spectrum spectrum_inversion %d\n", spectrum_inversion);
  front_end->satellite_spectrum_inversion   = spectrum_inversion;
  return NO_Si2164B_ERROR;
}
/************************************************************************************************************************
  Si2164B_L2_SAT_LPF function
  Use:        SAT tuner LPF setting function
              Used to set the satellite low pass filter
  Comments:   If the tuner is connected via the demodulator's I2C switch, enabling/disabling the i2c_passthru is required before/after tuning.
  Behavior:   This function closes the Si2164B's I2C switch then sets the SAT lpf and finally reopens the I2C switch
  Parameter:  *front_end, the front-end handle
  Parameter:  lph_khz, the low_pass filter frequency
  Returns:    the final value
************************************************************************************************************************/
signed int  Si2164B_L2_SAT_LPF            (Si2164B_L2_Context   *front_end, signed int lpf_khz)
{
  signed int ret;
  ret = 0;
  front_end = front_end; /* To avoid compiler warning if not used */
  lpf_khz   = lpf_khz;   /* To avoid compiler warning if not used */
 #ifdef    INDIRECT_I2C_CONNECTION
  front_end->f_SAT_tuner_enable(front_end->callback);
 #else  /* INDIRECT_I2C_CONNECTION */
  Si2164B_L2_Tuner_I2C_Enable(front_end);
 #endif /* INDIRECT_I2C_CONNECTION */
#if(! SEC_T2_TO_CHECK_02)
  ret  = (signed int)L1_RF_SAT_TUNER_LPF(front_end->tuner_sat, lpf_khz);
#endif
 #ifdef    INDIRECT_I2C_CONNECTION
  front_end->f_SAT_tuner_disable(front_end->callback);
 #else  /* INDIRECT_I2C_CONNECTION */
  Si2164B_L2_Tuner_I2C_Disable(front_end);
 #endif /* INDIRECT_I2C_CONNECTION */
  return ret;
}
/***********************************************************************************************************************
  Si2164B_L2_prepare_diseqc_sequence function
  Use:      DiSEqC sequence preparation function
            Used to prepare a DiSEqC sequence, checking the bus readiness and preparing all registers
  Returns:      0 if OK
  Parameter:  *front_end, the front-end handle
  Parameter:   sequence_length  the number of bytes to send
  Parameter:   sequenceBuffer   a buffer containing the DiSeqEc bytes
  Parameter:   cont_tone  a flag for continuous tone control: 0 = OFF, 1 = ON. When set to ON, a continuous tone is present before and after the message or the sequence of messages and/or tone burst.
  Parameter:   tone_burst a flag for tone burst control: 0 = OFF, 1 = ON. Used to send a Tone Burst sequence.
  Parameter:   burst_sel  a flag for tone burst selection: 0 = SA, 1 = SB. Selects which satellite is selected in the Tone Burst sequence.
  Parameter:   end_seq    a flag for end of sequence control: 0 = NOT_END, 1 = END. When set to END, Diseqc sequence is resumed after the current message. When set to NOT_END, current message is not the end of the full sequence. It's used in the case of sequence composed of repeated messages
 ***********************************************************************************************************************/
signed int  Si2164B_L2_prepare_diseqc_sequence(Si2164B_L2_Context *front_end, signed int sequence_length, unsigned char *sequenceBuffer, unsigned char cont_tone, unsigned char tone_burst, unsigned char burst_sel, unsigned char end_seq)
{
  unsigned char   msg_byte1;
  unsigned char   msg_byte2;
  unsigned char   msg_byte3;
  unsigned char   msg_byte4;
  unsigned char   msg_byte5;
  unsigned char   msg_byte6;
  signed int i;
  signed int             start_time;
  signed int             bus_state;

  SiTRACE("Si2164B_L2_prepare_diseqc_sequence(front_end->SiDemod_FE, sequence_length %d, sequence_buffer, cont_tone %d, tone_burst %d, burst_sel %d, end_seq %d);\n", sequence_length, cont_tone, tone_burst, burst_sel, end_seq);

  /* Waiting for DiSeqC bus readiness for max 72 ms (each DiSEqC byte lasts 12 ms, and the max sequence_length is 6) */
  bus_state          = Si2164B_DD_DISEQC_STATUS_RESPONSE_BUS_STATE_BUSY;
  start_time = system_time();
  while ( bus_state != Si2164B_DD_DISEQC_STATUS_RESPONSE_BUS_STATE_READY ) {
    Si2164B_L1_DD_DISEQC_STATUS(front_end->demod, Si2164B_DD_DISEQC_STATUS_CMD_LISTEN_NO_CHANGE);
    bus_state = front_end->demod->rsp->dd_diseqc_status.bus_state;
    if ((system_time() - start_time) > 200 ) { break; }
    system_wait(12);
  }
  if ( front_end->demod->rsp->dd_diseqc_status.bus_state != Si2164B_DD_DISEQC_STATUS_RESPONSE_BUS_STATE_READY ) {
    SiTRACE ("DiSeqC bus not ready to send a message\n");
    SiERROR ("DiSeqC bus not ready to send a message\n");
    return ERROR_Si2164B_DISEQC_BUS_NOT_READY;
  }

  i = 0;

  if (i<sequence_length) { msg_byte1 = sequenceBuffer[i++];} else {msg_byte1 = 0x00;}
  if (i<sequence_length) { msg_byte2 = sequenceBuffer[i++];} else {msg_byte2 = 0x00;}
  if (i<sequence_length) { msg_byte3 = sequenceBuffer[i++];} else {msg_byte3 = 0x00;}
  if (i<sequence_length) { msg_byte4 = sequenceBuffer[i++];} else {msg_byte4 = 0x00;}
  if (i<sequence_length) { msg_byte5 = sequenceBuffer[i++];} else {msg_byte5 = 0x00;}
  if (i<sequence_length) { msg_byte6 = sequenceBuffer[i++];} else {msg_byte6 = 0x00;}


  front_end->demod->cmd->dd_diseqc_send.cont_tone  = cont_tone;
  front_end->demod->cmd->dd_diseqc_send.tone_burst = tone_burst;
  front_end->demod->cmd->dd_diseqc_send.burst_sel  = burst_sel;
  front_end->demod->cmd->dd_diseqc_send.end_seq    = end_seq;
  front_end->demod->cmd->dd_diseqc_send.msg_length = sequence_length;
  front_end->demod->cmd->dd_diseqc_send.msg_byte1  = msg_byte1;
  front_end->demod->cmd->dd_diseqc_send.msg_byte2  = msg_byte2;
  front_end->demod->cmd->dd_diseqc_send.msg_byte3  = msg_byte3;
  front_end->demod->cmd->dd_diseqc_send.msg_byte4  = msg_byte4;
  front_end->demod->cmd->dd_diseqc_send.msg_byte5  = msg_byte5;
  front_end->demod->cmd->dd_diseqc_send.msg_byte6  = msg_byte6;

  return NO_Si2164B_ERROR;
}
/***********************************************************************************************************************
  Si2164B_L2_trigger_diseqc_sequence function
  Use:      DiSEqC sequence send function
            Used to send a DiSEqC sequence prepared using Si2167_L1_Demod_prepare_diseqc_sequence
  Parameter:   *demod          the demod handle
  Porting:     This can be removed in not using SAT standards or not using DiSEqC
  Registers:   ds_send
 ***********************************************************************************************************************/
signed int  Si2164B_L2_trigger_diseqc_sequence(Si2164B_L2_Context *front_end)
{
  return Si2164B_L1_DD_DISEQC_SEND (front_end->demod, 1,
                            front_end->demod->cmd->dd_diseqc_send.cont_tone,
                            front_end->demod->cmd->dd_diseqc_send.tone_burst,
                            front_end->demod->cmd->dd_diseqc_send.burst_sel,
                            front_end->demod->cmd->dd_diseqc_send.end_seq,
                            front_end->demod->cmd->dd_diseqc_send.msg_length,
                            front_end->demod->cmd->dd_diseqc_send.msg_byte1,
                            front_end->demod->cmd->dd_diseqc_send.msg_byte2,
                            front_end->demod->cmd->dd_diseqc_send.msg_byte3,
                            front_end->demod->cmd->dd_diseqc_send.msg_byte4,
                            front_end->demod->cmd->dd_diseqc_send.msg_byte5,
                            front_end->demod->cmd->dd_diseqc_send.msg_byte6);
}
/***********************************************************************************************************************
  Si2164B_L2_send_diseqc_sequence function
  Use:      DiSEqC sequence send function
            Used to send a DiSEqC sequence
  Returns:      0 if OK
  Parameter:  *front_end, the front-end handle
  Parameter:   sequence_length  the number of bytes to send
  Parameter:   sequenceBuffer   a buffer containing the DiSeqEc bytes
  Parameter:   cont_tone  a flag for continuous tone control: 0 = OFF, 1 = ON. When set to ON, a continuous tone is present before and after the message or the sequence of messages and/or tone burst.
  Parameter:   tone_burst a flag for tone burst control: 0 = OFF, 1 = ON. Used to send a Tone Burst sequence.
  Parameter:   burst_sel  a flag for tone burst selection: 0 = SA, 1 = SB. Selects which satellite is selected in the Tone Burst sequence.
  Parameter:   end_seq    a flag for end of sequence control: 0 = NOT_END, 1 = END. When set to END, Diseqc sequence is resumed after the current message. When set to NOT_END, current message is not the end of the full sequence. It's used in the case of sequence composed of repeated messages
 ***********************************************************************************************************************/
signed int  Si2164B_L2_send_diseqc_sequence(Si2164B_L2_Context *front_end, signed int sequence_length, unsigned char *sequenceBuffer, unsigned char cont_tone, unsigned char tone_burst, unsigned char burst_sel, unsigned char end_seq)
{
  signed int i;
  signed int             start_time;
  signed int             bus_state;

  SiTRACE("Si2164B_L2_send_diseqc_sequence(front_end->Si2164B_FE, sequence_length %d, sequence_buffer, cont_tone %d, tone_burst %d, burst_sel %d, end_seq %d);\n", sequence_length, cont_tone, tone_burst, burst_sel, end_seq);

  /* Waiting for DiSeqC bus readiness for max 72 ms (each DiSEqC byte lasts 12 ms, and the max sequence_length is 6) */
  bus_state          = Si2164B_DD_DISEQC_STATUS_RESPONSE_BUS_STATE_BUSY;
  start_time = system_time();
  while ( bus_state != Si2164B_DD_DISEQC_STATUS_RESPONSE_BUS_STATE_READY ) {
    Si2164B_L1_DD_DISEQC_STATUS(front_end->demod, Si2164B_DD_DISEQC_STATUS_CMD_LISTEN_NO_CHANGE);
    bus_state = front_end->demod->rsp->dd_diseqc_status.bus_state;
    if ((system_time() - start_time) > 200 ) { break; }
    system_wait(12);
  }
  if ( front_end->demod->rsp->dd_diseqc_status.bus_state != Si2164B_DD_DISEQC_STATUS_RESPONSE_BUS_STATE_READY ) {
    SiTRACE ("DiSeqC bus not ready to send a message\n");
    SiERROR ("DiSeqC bus not ready to send a message\n");
    return ERROR_Si2164B_DISEQC_BUS_NOT_READY;
  }

  i = 0;

  front_end->demod->cmd->dd_diseqc_send.enable          = 1;
  front_end->demod->cmd->dd_diseqc_send.cont_tone       = cont_tone;
  front_end->demod->cmd->dd_diseqc_send.tone_burst      = tone_burst;
  front_end->demod->cmd->dd_diseqc_send.burst_sel       = burst_sel;
  front_end->demod->cmd->dd_diseqc_send.end_seq         = end_seq;
  front_end->demod->cmd->dd_diseqc_send.msg_length      = sequence_length;

  if (i<sequence_length) { front_end->demod->cmd->dd_diseqc_send.msg_byte1 = sequenceBuffer[i++];} else {front_end->demod->cmd->dd_diseqc_send.msg_byte1 = 0x00;}
  if (i<sequence_length) { front_end->demod->cmd->dd_diseqc_send.msg_byte2 = sequenceBuffer[i++];} else {front_end->demod->cmd->dd_diseqc_send.msg_byte2 = 0x00;}
  if (i<sequence_length) { front_end->demod->cmd->dd_diseqc_send.msg_byte3 = sequenceBuffer[i++];} else {front_end->demod->cmd->dd_diseqc_send.msg_byte3 = 0x00;}
  if (i<sequence_length) { front_end->demod->cmd->dd_diseqc_send.msg_byte4 = sequenceBuffer[i++];} else {front_end->demod->cmd->dd_diseqc_send.msg_byte4 = 0x00;}
  if (i<sequence_length) { front_end->demod->cmd->dd_diseqc_send.msg_byte5 = sequenceBuffer[i++];} else {front_end->demod->cmd->dd_diseqc_send.msg_byte5 = 0x00;}
  if (i<sequence_length) { front_end->demod->cmd->dd_diseqc_send.msg_byte6 = sequenceBuffer[i++];} else {front_end->demod->cmd->dd_diseqc_send.msg_byte6 = 0x00;}

  return Si2164B_L1_DD_DISEQC_SEND            (front_end->demod, 1, cont_tone, tone_burst, burst_sel, end_seq, sequence_length, front_end->demod->cmd->dd_diseqc_send.msg_byte1, front_end->demod->cmd->dd_diseqc_send.msg_byte2, front_end->demod->cmd->dd_diseqc_send.msg_byte3, front_end->demod->cmd->dd_diseqc_send.msg_byte4, front_end->demod->cmd->dd_diseqc_send.msg_byte5, front_end->demod->cmd->dd_diseqc_send.msg_byte6);
}
/***********************************************************************************************************************
  Si2164B_L2_read_diseqc_reply function
  Use:      DiSEqC reply message read function
            Used to read a DiSEqC message
  Returns:      0 if OK
  Parameter:  *front_end, the front-end handle
  Parameter:  *reply_length a pointer to the number of bytes in the reply
  Parameter:   replyBuffer   a buffer used to store the DiSeqEc bytes
               NOTE: This length being unknown at the time of calling, a 3 bytes buffer MUST be allocated by the caller
 ***********************************************************************************************************************/
signed int  Si2164B_L2_read_diseqc_reply   (Si2164B_L2_Context *front_end, signed int *reply_length, unsigned char *replyBuffer)
{
  signed int return_code;
  signed int i;
  i = 0;
  if ( (return_code = Si2164B_L1_DD_DISEQC_STATUS(front_end->demod, Si2164B_DD_DISEQC_STATUS_CMD_LISTEN_NO_CHANGE) ) != NO_Si2164B_ERROR) {
    *reply_length = front_end->demod->rsp->dd_diseqc_status.reply_length;
    if (i<*reply_length) { replyBuffer[i] = front_end->demod->rsp->dd_diseqc_status.reply_byte1; i++;}
    if (i<*reply_length) { replyBuffer[i] = front_end->demod->rsp->dd_diseqc_status.reply_byte2; i++;}
    if (i<*reply_length) { replyBuffer[i] = front_end->demod->rsp->dd_diseqc_status.reply_byte3; i++;}
    return return_code;
  }
  return  -1 /* To Be Defined */;
}
/************************************************************************************************************************
  NAME: Si2164B_L2_Trace_Scan_Status
  DESCRIPTION: traces the scan_status
  Parameter:  Pointer to Si2164B Context
  Returns:    void
************************************************************************************************************************/
const unsigned char *Si2164B_L2_Trace_Scan_Status  (signed int scan_status)
{
    switch (scan_status) {
      case  Si2164B_SCAN_STATUS_RESPONSE_SCAN_STATUS_ANALOG_CHANNEL_FOUND  : { return (unsigned char *)"ANALOG  CHANNEL_FOUND"; break; }
      case  Si2164B_SCAN_STATUS_RESPONSE_SCAN_STATUS_DIGITAL_CHANNEL_FOUND : { return (unsigned char *)"DIGITAL CHANNEL_FOUND"; break; }
      case  Si2164B_SCAN_STATUS_RESPONSE_SCAN_STATUS_DEBUG                 : { return (unsigned char *)"DEBUG"                ; break; }
      case  Si2164B_SCAN_STATUS_RESPONSE_SCAN_STATUS_ERROR                 : { return (unsigned char *)"ERROR"                ; break; }
      case  Si2164B_SCAN_STATUS_RESPONSE_SCAN_STATUS_ENDED                 : { return (unsigned char *)"ENDED"                ; break; }
      case  Si2164B_SCAN_STATUS_RESPONSE_SCAN_STATUS_IDLE                  : { return (unsigned char *)"IDLE"                 ; break; }
      case  Si2164B_SCAN_STATUS_RESPONSE_SCAN_STATUS_SEARCHING             : { return (unsigned char *)"SEARCHING"            ; break; }
      case  Si2164B_SCAN_STATUS_RESPONSE_SCAN_STATUS_TUNE_REQUEST          : { return (unsigned char *)"TUNE_REQUEST"         ; break; }
      default                                                             : { return (unsigned char *)"Unknown!"             ; break; }
    }
}
/************************************************************************************************************************
  NAME: Si2164B_L2_Set_Invert_Spectrum
  DESCRIPTION: return the required invert_spectrum value depending on the settings:
              front_end->demod->media
              front_end->satellite_spectrum_inversion
              front_end->lnb_type
              front_end->unicable_spectrum_inversion

  Parameter:  Pointer to Si2164B Context
  Returns:    the required invert_spectrum value
************************************************************************************************************************/
unsigned char Si2164B_L2_Set_Invert_Spectrum (Si2164B_L2_Context *front_end)
{
  unsigned char inversion;
  inversion = 0; /* to avoid compile error */
  if (front_end->demod->media == Si2164B_TERRESTRIAL) {
    inversion = Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_NORMAL;
  }
  if (front_end->demod->media == Si2164B_SATELLITE  ) {
    //inversion = front_end->satellite_spectrum_inversion;//csm,disable
	  //model specific setting,csm, add
	  if((front_end->tuner_sat->i2c->address == SAT1_MAIN_ADDRESS) || (front_end->tuner_sat->i2c->address == SAT1_SUB_ADDRESS))//5816SW
		  inversion = Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_NORMAL;
	  else if(front_end->tuner_sat->i2c->address == SAT2_ADDRESS)//5815M
		  inversion = Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_INVERTED;
	  //end
#ifdef   UNICABLE_COMPATIBLE
    if (front_end->lnb_type == UNICABLE_LNB_TYPE_UNICABLE) {
      if (front_end->unicable_spectrum_inversion) inversion = !inversion;
    }
#endif /* UNICABLE_COMPATIBLE */
  }
  return inversion;
}
/************************************************************************************************************************
  NAME: Si2164B_L2_Channel_Seek_Init
  DESCRIPTION: logs the seek parameters in the context structure
  Programming Guide Reference:    Flowchart TBD (Channel Scan flowchart)
              standards where the parameter is used:     T   T2    C MCNS   C2    S   S2   DSS  ATV
  Parameter:  front_end Pointer to Si2164B Context        x    x    x    x    x    x    x     x    x
  Parameter:  rangeMin     starting Frequency Hz         x    x    x    x    x    x    x     x    x
  Parameter:  rangeMax     ending Frequency Hz           x    x    x    x    x    x    x     x    x
  Parameter:  seekBWHz     Signal Bandwidth (for T/T2)   x    x
  Parameter:  seekStepHz   Frequency step in Hz          x    x
  Parameter:  minSRbps     minimum symbol rate in Baud             x    x         x    x     x
  Parameter:  maxSRbps     maximum symbol rate in Baud             x    x         x    x     x
  Parameter:  minRSSIdBm   min RSSI dBm                  x    x                                   x
  Parameter:  maxRSSIdBm   max RSSI dBm                  x    x                                   x
  Parameter:  minSNRHalfdB min SNR 1/2 dB                x    x                                   x
  Parameter:  maxSNRHalfdB max SNR 1/2 dB                x    x                                   x
  Returns:    0 if successful, otherwise an error.
************************************************************************************************************************/
signed int  Si2164B_L2_Channel_Seek_Init  (Si2164B_L2_Context *front_end, signed int rangeMin, signed int rangeMax, signed int seekBWHz, signed int seekStepHz, signed int minSRbps, signed int maxSRbps, signed int minRSSIdBm, signed int maxRSSIdBm, signed int minSNRHalfdB, signed int maxSNRHalfdB)
{
  signed int modulation;
  modulation = 0; /* to avoid compiler error */
  if (front_end->demod->media == Si2164B_TERRESTRIAL) {
    SiTRACE("media TERRESTRIAL\n");
    front_end->tuneUnitHz   =    1;
  }
  if (front_end->demod->media == Si2164B_SATELLITE  ) {
    SiTRACE("media SATELLITE\n");
    front_end->tuneUnitHz   = 1000;
   /* Set SAT tuner LPF to max to allow blindscan (in kHz) */
    Si2164B_L2_SAT_LPF(front_end, 100000);
  }
  SiTRACE ("blindscan_interaction >> (init  ) Si2164B_L1_SCAN_CTRL( front_end->demod, Si2164B_SCAN_CTRL_CMD_ACTION_ABORT)\n");
  Si2164B_L1_SCAN_CTRL (front_end->demod, Si2164B_SCAN_CTRL_CMD_ACTION_ABORT , 0);
  /* Check detection standard based on dd_mode.modulation and dd_mode.auto_detect */
  SiTRACE("standard %d, dd_mode.modulation %d, dd_mode.auto_detect %d\n",  front_end->standard, front_end->demod->prop->dd_mode.modulation, front_end->demod->prop->dd_mode.auto_detect);
  switch (front_end->demod->prop->dd_mode.modulation) {
    case Si2164B_DD_MODE_PROP_MODULATION_AUTO_DETECT : {
      switch (front_end->demod->prop->dd_mode.auto_detect) {
        case Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_S_S2     : {
          modulation = Si2164B_DD_MODE_PROP_MODULATION_DVBS2;
          break;
        }
        case Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_S_S2_DSS : {
          modulation = Si2164B_DD_MODE_PROP_MODULATION_DSS;
          break;
        }
        case Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_T_T2     : {
          modulation = Si2164B_DD_MODE_PROP_MODULATION_DVBT2;
          break;
        }
        default : {
          SiTRACE("AUTO DETECT '%d' is not managed by Si2164B_L2_Channel_Seek_Init\n", front_end->demod->prop->dd_mode.auto_detect);
          break;
        }
      }
      break;
    }
    case Si2164B_DD_MODE_PROP_MODULATION_DSS  :
    case Si2164B_DD_MODE_PROP_MODULATION_DVBS :
    case Si2164B_DD_MODE_PROP_MODULATION_DVBS2: {
      modulation = front_end->demod->prop->dd_mode.modulation;
      break;
    }
    case Si2164B_DD_MODE_PROP_MODULATION_DVBC :
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT2:
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT : {
      modulation = front_end->demod->prop->dd_mode.modulation;
      break;
    }
    default : {
      SiTRACE("'%d' modulation (%s) is not managed by Si2164B_L2_Channel_Seek_Init\n", front_end->demod->prop->dd_mode.modulation, Si2164B_standardName(front_end->demod->prop->dd_mode.modulation));
      return ERROR_Si2164B_ERR;
      break;
    }
  }
  SiTRACE("Si2164B_L2_Channel_Seek_Init for %s (%d)\n", Si2164B_standardName(modulation), modulation );
  switch (modulation) {
    case Si2164B_DD_MODE_PROP_MODULATION_DSS  :
    case Si2164B_DD_MODE_PROP_MODULATION_DVBS :
    case Si2164B_DD_MODE_PROP_MODULATION_DVBS2: {
      if (front_end->auto_detect_SAT==1) {
        front_end->demod->prop->dd_mode.modulation  = Si2164B_DD_MODE_PROP_MODULATION_AUTO_DETECT;
        /* Trick to avoid slowing down AUTO SAT mode when not using DSS: */
        /*  AUTO_DVB_S_S2_DSS is only used if the input standard is DSS  */
        if (modulation == Si2164B_DD_MODE_PROP_MODULATION_DSS) {
          front_end->demod->prop->dd_mode.auto_detect = Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_S_S2_DSS;
        } else {
          front_end->demod->prop->dd_mode.auto_detect = Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_S_S2;
        }
      }
      SiTRACE("DVB-S AFC range %d DVB-S2 AFC range %d\n", front_end->demod->prop->dvbs_afc_range.range_khz, front_end->demod->prop->dvbs2_afc_range.range_khz );
#ifdef   UNICABLE_COMPATIBLE
      if (front_end->lnb_type == UNICABLE_LNB_TYPE_UNICABLE) {
        front_end->demod->prop->scan_sat_unicable_bw.scan_sat_unicable_bw  =   480;
        front_end->demod->prop->scan_sat_unicable_min_tune_step.scan_sat_unicable_min_tune_step  =  50;
      } else {
        front_end->demod->prop->scan_sat_unicable_bw.scan_sat_unicable_bw  =     0;
        front_end->demod->prop->scan_sat_unicable_min_tune_step.scan_sat_unicable_min_tune_step  =   0;
      }
      Si2164B_L1_SetProperty2(front_end->demod, Si2164B_SCAN_SAT_UNICABLE_BW_PROP_CODE);
      Si2164B_L1_SetProperty2(front_end->demod, Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP_CODE  );
      front_end->demod->prop->dd_mode.invert_spectrum = Si2164B_L2_Set_Invert_Spectrum(front_end);
#endif /* UNICABLE_COMPATIBLE */
      break;
    }
    case Si2164B_DD_MODE_PROP_MODULATION_DVBC :
     /* Forcing BW to 8 MHz for DVB-C */
      seekBWHz = 8000000;
      front_end->demod->prop->dd_mode.modulation  = Si2164B_DD_MODE_PROP_MODULATION_DVBC;
      front_end->demod->prop->dd_mode.auto_detect = Si2164B_DD_MODE_PROP_AUTO_DETECT_NONE;
#ifdef    Si2167B_BLINDSCAN_PATCH
      if  ((((api->rsp->part_info.part == 65 ) & (api->rsp->part_info.chiprev + 0x40 == 'B'))
         || ((api->rsp->part_info.part == 67 ) & (api->rsp->part_info.chiprev + 0x40 == 'B'))
        ) & ((api->rsp->part_info.pmajor == '2') & (api->rsp->part_info.pminor == '2')) ) {
        front_end->previous_standard = 2;
        front_end->Si2167B_init_done = 0;
        front_end->demod->load_DVB_C_Blindlock_Patch = 1;
        Si2167B_L2_switch_to_standard(front_end, Si2167B_DD_MODE_PROP_MODULATION_DVBC, 0);
      }
#endif /* Si2167B_BLINDSCAN_PATCH */
      front_end->demod->prop->dvbc_constellation.constellation = Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_AUTO;
      Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DVBC_CONSTELLATION_PROP_CODE);
      SiTRACE("DVB-C AFC range %d\n", front_end->demod->prop->dvbc_afc_range.range_khz);
      break;
    case Si2164B_DD_MODE_PROP_MODULATION_MCNS :
     /* Forcing BW to 8 MHz for MCNS */
      seekBWHz = 8000000;
      front_end->demod->prop->dd_mode.modulation  = Si2164B_DD_MODE_PROP_MODULATION_MCNS;
      front_end->demod->prop->dd_mode.auto_detect = Si2164B_DD_MODE_PROP_AUTO_DETECT_NONE;
      front_end->demod->prop->mcns_constellation.constellation = Si2164B_MCNS_CONSTELLATION_PROP_CONSTELLATION_AUTO;
      Si2164B_L1_SetProperty2(front_end->demod, Si2164B_MCNS_CONSTELLATION_PROP_CODE);
      SiTRACE("MCNS AFC range %d\n", front_end->demod->prop->mcns_afc_range.range_khz);
      break;
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT2:
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT :
      front_end->demod->prop->dvbt_hierarchy.stream = Si2164B_DVBT_HIERARCHY_PROP_STREAM_HP;
      Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DVBT_HIERARCHY_PROP_CODE);
      SiTRACE("DVB-T AFC range %d\n", front_end->demod->prop->dvbt_afc_range.range_khz);
      if (front_end->auto_detect_TER==1) {
        front_end->demod->prop->dd_mode.modulation  = Si2164B_DD_MODE_PROP_MODULATION_AUTO_DETECT;
        front_end->demod->prop->dd_mode.auto_detect = Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_T_T2;
      }
      SiTRACE("DVB-T2 AFC range %d\n", front_end->demod->prop->dvbt2_afc_range.range_khz);
      Si2164B_L1_DVBT2_PLP_SELECT    (front_end->demod, 0, Si2164B_DVBT2_PLP_SELECT_CMD_PLP_ID_SEL_MODE_AUTO);
      break;
    case Si2164B_DD_MODE_PROP_MODULATION_DVBC2:
     /* Forcing BW to 8 MHz for DVB-C2 */
      seekBWHz = 8000000;
      front_end->demod->prop->dd_mode.modulation  = Si2164B_DD_MODE_PROP_MODULATION_DVBC;
      front_end->demod->prop->dd_mode.auto_detect = Si2164B_DD_MODE_PROP_AUTO_DETECT_NONE;
      front_end->demod->cmd->dvbc2_ds_plp_select.plp_id      = 0;
      front_end->demod->cmd->dvbc2_ds_plp_select.id_sel_mode = Si2164B_DVBC2_DS_PLP_SELECT_CMD_ID_SEL_MODE_AUTO;
      front_end->demod->cmd->dvbc2_ds_plp_select.ds_id       = 0;
      Si2164B_L1_DVBC2_DS_PLP_SELECT      (front_end->demod,
                                          front_end->demod->cmd->dvbc2_ds_plp_select.plp_id,
                                          front_end->demod->cmd->dvbc2_ds_plp_select.id_sel_mode,
                                          front_end->demod->cmd->dvbc2_ds_plp_select.ds_id);
      SiTRACE("DVB-C2 AFC range %d\n", front_end->demod->prop->dvbc2_afc_range.range_khz);
      break;
    default : {
      SiTRACE("'%d' modulation (%s) is not managed by Si2164B_L2_Channel_Seek_Init\n", modulation, Si2164B_standardName(modulation));
      break;
    }
  }

  front_end->seekBWHz     = seekBWHz;
  front_end->seekStepHz   = seekStepHz;
  front_end->minSRbps     = minSRbps;
  front_end->maxSRbps     = maxSRbps;
  front_end->rangeMin     = rangeMin;
  front_end->rangeMax     = rangeMax;
  front_end->minRSSIdBm   = minRSSIdBm;
  front_end->maxRSSIdBm   = maxRSSIdBm;
  front_end->minSNRHalfdB = minSNRHalfdB;
  front_end->maxSNRHalfdB = maxSNRHalfdB;
  front_end->cumulativeScanTime    = 0;
  front_end->cumulativeTimeoutTime = 0;
  front_end->nbTimeouts   = 0;
  front_end->nbDecisions  = 0;
  front_end->seekAbort    = 0;

  SiTRACE("Si2164B_L2_Channel_Seek_Init with %d to  %d, sawBW %d, minSR %d, maxSR %d\n", front_end->rangeMin, front_end->rangeMax, front_end->seekBWHz, front_end->minSRbps, front_end->maxSRbps);
  SiTRACE("spectrum inversion %d\n",front_end->demod->prop->dd_mode.invert_spectrum );
  front_end->demod->prop->scan_fmin.scan_fmin                   = (signed int)((front_end->rangeMin*front_end->tuneUnitHz)/65536);
  front_end->demod->prop->scan_fmax.scan_fmax                   = (signed int)((front_end->rangeMax*front_end->tuneUnitHz)/65536);

  front_end->demod->prop->scan_symb_rate_min.scan_symb_rate_min = front_end->minSRbps/1000;
  front_end->demod->prop->scan_symb_rate_max.scan_symb_rate_max = front_end->maxSRbps/1000;

  Si2164B_L1_SetProperty2(front_end->demod, Si2164B_SCAN_FMIN_PROP_CODE);
  Si2164B_L1_SetProperty2(front_end->demod, Si2164B_SCAN_FMAX_PROP_CODE);
  Si2164B_L1_SetProperty2(front_end->demod, Si2164B_SCAN_SYMB_RATE_MIN_PROP_CODE);
  Si2164B_L1_SetProperty2(front_end->demod, Si2164B_SCAN_SYMB_RATE_MAX_PROP_CODE);



  front_end->demod->prop->scan_ien.buzien           = Si2164B_SCAN_IEN_PROP_BUZIEN_ENABLE  ; /* (default 'DISABLE') */
  front_end->demod->prop->scan_ien.reqien           = Si2164B_SCAN_IEN_PROP_REQIEN_ENABLE  ; /* (default 'DISABLE') */
  Si2164B_L1_SetProperty2(front_end->demod, Si2164B_SCAN_IEN_PROP_CODE);

  front_end->demod->prop->scan_int_sense.reqnegen   = Si2164B_SCAN_INT_SENSE_PROP_REQNEGEN_DISABLE ; /* (default 'DISABLE') */
  front_end->demod->prop->scan_int_sense.reqposen   = Si2164B_SCAN_INT_SENSE_PROP_REQPOSEN_ENABLE  ; /* (default 'ENABLE') */
  Si2164B_L1_SetProperty2(front_end->demod, Si2164B_SCAN_INT_SENSE_PROP_CODE);


#ifdef    ALLOW_Si2164B_BLINDSCAN_DEBUG
  front_end->demod->prop->scan_ter_config.scan_debug = 0x0f;
#endif /* ALLOW_Si2164B_BLINDSCAN_DEBUG */
  if (front_end->demod->media == Si2164B_TERRESTRIAL) {
    front_end->demod->prop->scan_ter_config.analog_bw     = Si2164B_SCAN_TER_CONFIG_PROP_ANALOG_BW_8MHZ;
    if ( front_end->rangeMin == front_end->rangeMax ) {
      front_end->demod->prop->scan_ter_config.mode        = Si2164B_SCAN_TER_CONFIG_PROP_MODE_BLIND_LOCK;
      SiTRACE("Blindlock < %8d %8d > < %8d %8d >\n", front_end->demod->prop->scan_fmin.scan_fmin, front_end->demod->prop->scan_fmax.scan_fmax, front_end->demod->prop->scan_symb_rate_min.scan_symb_rate_min, front_end->demod->prop->scan_symb_rate_max.scan_symb_rate_max);
    } else {
      front_end->demod->prop->scan_ter_config.mode        = Si2164B_SCAN_TER_CONFIG_PROP_MODE_BLIND_SCAN;
      SiTRACE("Blindscan < %8d %8d > < %8d %8d >\n", front_end->demod->prop->scan_fmin.scan_fmin, front_end->demod->prop->scan_fmax.scan_fmax, front_end->demod->prop->scan_symb_rate_min.scan_symb_rate_min, front_end->demod->prop->scan_symb_rate_max.scan_symb_rate_max);
    }
    front_end->demod->prop->scan_ter_config.search_analog = Si2164B_SCAN_TER_CONFIG_PROP_SEARCH_ANALOG_DISABLE;
    Si2164B_L1_SetProperty2(front_end->demod, Si2164B_SCAN_TER_CONFIG_PROP_CODE);
    if (seekBWHz == 1700000) {
      front_end->demod->prop->dd_mode.bw = Si2164B_DD_MODE_PROP_BW_BW_1D7MHZ;
    } else {
      front_end->demod->prop->dd_mode.bw = seekBWHz/1000000;
    }
  }
  Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DD_MODE_PROP_CODE);

#ifdef    ALLOW_Si2164B_BLINDSCAN_DEBUG
  front_end->demod->prop->scan_sat_config.scan_debug = 0x03;
#endif /* ALLOW_Si2164B_BLINDSCAN_DEBUG */
  if (front_end->demod->media == Si2164B_SATELLITE  ) {
    front_end->demod->prop->scan_sat_config.analog_detect = Si2164B_SCAN_SAT_CONFIG_PROP_ANALOG_DETECT_ENABLED;
    front_end->demod->prop->scan_sat_config.reserved1     =  0;
    front_end->demod->prop->scan_sat_config.reserved2     = 12;
    Si2164B_L1_SetProperty2(front_end->demod, Si2164B_SCAN_SAT_CONFIG_PROP_CODE);
  }
  Si2164B_L1_DD_RESTART  (front_end->demod);

  Si2164B_L1_SCAN_STATUS (front_end->demod, Si2164B_SCAN_STATUS_CMD_INTACK_OK);
  SiTRACE("blindscan_status leaving Seek_Init %s\n", Si2164B_L2_Trace_Scan_Status(front_end->demod->rsp->scan_status.scan_status) );
  /* Preparing the next call to Si2164B_L1_SCAN_CTRL which needs to be a 'START'*/
  front_end->demod->cmd->scan_ctrl.action = Si2164B_SCAN_CTRL_CMD_ACTION_START;
  front_end->handshakeOn       = 0;
  front_end->searchStartTime   = system_time();
  SiTRACE("blindscan_handshake : Seek_Next will return every ~%d ms\n", front_end->handshakePeriod_ms );
  return 0;
}
/************************************************************************************************************************
  NAME: Si2164B_L2_Channel_Seek_Next
  DESCRIPTION: Looks for the next channel, starting from the last detected channel
  Programming Guide Reference:    Flowchart TBD (Channel Scan flowchart)

  Parameter:  Pointer to Si2164B Context
  Returns:    1 if channel is found, 0 otherwise (either abort or end of range)
              Any other value represents the time spent searching (if front_end->handshakeUsed == 1)
************************************************************************************************************************/
signed int  Si2164B_L2_Channel_Seek_Next  (Si2164B_L2_Context *front_end
                                           , signed int *standard
                                           , signed int *freq
                                           , signed int *bandwidth_Hz
                                           , signed int *stream
                                           , unsigned int *symbol_rate_bps
                                           , signed int *constellation
                                           , signed int *num_data_slice
                                           , signed int *num_plp
                                           , signed int *T2_base_lite
                                           )
{
    signed int           return_code;
    signed int           seek_freq;
    signed int           seek_freq_kHz;
    signed int           channelIncrement;
    signed int           seekStartTime;    /* seekStartTime    is used to trace the time spent in Si2164B_L2_Channel_Seek_Next and is only set when entering the function                            */
    signed int           buzyStartTime;    /* buzyStartTime    is used to trace the time spent waiting for scan_status.buz to be different from 'BUZY'                                              */
    signed int           ddRestartTime;    /* ddRestartTime    is used to trace the time spent between DD_RESTART and the decision                                                                  */
    signed int           timeoutDelay;
    signed int           handshakeDelay;
    signed int           searchDelay;
    signed int           decisionDelay;
    signed int           max_lock_time_ms;
    signed int           min_lock_time_ms;
    signed int           max_decision_time_ms;
    signed int           blind_mode;
    signed int           skip_resume;
    signed int           start_resume;
    signed int           previous_scan_status;
    unsigned char jump_to_next_channel;
    L1_Si2164B_Context *api;
    /* init all flags to avoid compiler warnings */
    return_code          = 0; /* To avoid code checker warning */
    seek_freq            = 0; /* To avoid code checker warning */
    seek_freq_kHz        = 0; /* To avoid code checker warning */
    channelIncrement     = 0; /* To avoid code checker warning */
    seekStartTime        = 0; /* To avoid code checker warning */
    buzyStartTime        = 0; /* To avoid code checker warning */
    timeoutDelay         = 0; /* To avoid code checker warning */
    handshakeDelay       = 0; /* To avoid code checker warning */
    max_lock_time_ms     = 0; /* To avoid code checker warning */
    min_lock_time_ms     = 0; /* To avoid code checker warning */
    max_decision_time_ms = 0; /* To avoid code checker warning */
    skip_resume          = 0; /* To avoid code checker warning */
    previous_scan_status = 0; /* To avoid code checker warning */
    searchDelay          = 0; /* To avoid code checker warning */
    start_resume         = 0; /* To avoid code checker warning */
    ddRestartTime        =0;

    api = front_end->demod;

    blind_mode   = 0;

    /* Clear all return values which may not be used depending on the standard */
    *bandwidth_Hz    = 0;
    *stream          = 0;
    *symbol_rate_bps = 0;
    *constellation   = 0;
     *num_plp        = 0;
     *T2_base_lite   = 0;
     *num_plp        = 0;
     *num_data_slice = 0;

    if (front_end->seekAbort) {
      SiTRACE("Si2164B_L2_Channel_Seek_Next : previous run aborted. Please Si2164B_L2_Channel_Seek_Init to perform a new search.\n");
      return 0;
    }

    SiTRACE("front_end->demod->standard %d (%s)\n",front_end->demod->standard, Si2164B_standardName(front_end->demod->standard) );

    /* Setting max and max lock times and blind_mode flag */
    switch ( front_end->demod->standard )
    {
      /* For T/T2 detection, use the max value between Si2164B_DVBT_MAX_LOCK_TIME and Si2164B_DVBT2_MAX_LOCK_TIME */
      /* With Si2164B-A, it's Si2164B_DVBT2_MAX_LOCK_TIME                                                         */
      /* This value will be refined as soon as the standard is known, i.e. when PCL = 1                         */
      case Si2164B_DD_MODE_PROP_MODULATION_DVBT2:
      case Si2164B_DD_MODE_PROP_MODULATION_DVBT: {
        blind_mode = 0;
        max_lock_time_ms = Si2164B_DVBT_MAX_LOCK_TIME;
          max_lock_time_ms = Si2164B_DVBT2_MAX_LOCK_TIME;
        min_lock_time_ms = Si2164B_DVBT_MIN_LOCK_TIME;
        break;
      }
      case Si2164B_DD_MODE_PROP_MODULATION_DVBC : {
        blind_mode = 1;
        max_lock_time_ms = Si2164B_DVBC_MAX_SEARCH_TIME;
        min_lock_time_ms = Si2164B_DVBC_MIN_LOCK_TIME;
        break;
      }
      case Si2164B_DD_MODE_PROP_MODULATION_DVBC2: {
        *standard = Si2164B_DD_MODE_PROP_MODULATION_DVBC2;
        return_code = Si2164B_L2_lock_to_carrier   (front_end
                                 ,  front_end->demod->standard
                                 ,  front_end->rangeMin
                                  , front_end->seekBWHz
                                  , 0
                                  , 0
                                  , 0
                                  , -1
                                  ,  0
                                  ,  Si2164B_DVBT2_MODE_PROP_LOCK_MODE_ANY
                                    );
        if (return_code == 1) {
          Si2164B_L1_DVBC2_SYS_INFO (front_end->demod);
          *num_data_slice = front_end->demod->rsp->dvbc2_sys_info.num_dslice;
        }
        return return_code;
        break;
      }
      case Si2164B_DD_MODE_PROP_MODULATION_DSS  :
      case Si2164B_DD_MODE_PROP_MODULATION_DVBS :
      case Si2164B_DD_MODE_PROP_MODULATION_DVBS2: {
        blind_mode = 1;
        max_lock_time_ms = Si2164B_SAT_MAX_SEARCH_TIME;
        min_lock_time_ms = Si2164B_DVBS_MIN_LOCK_TIME;
        break;
      }
      default                                  : {
        SiTRACE("'%d' standard (%s) is not managed by Si2164B_L2_Channel_Seek_Next\n", front_end->demod->prop->dd_mode.modulation, Si2164B_standardName(front_end->demod->prop->dd_mode.modulation));
        front_end->seekAbort = 1;
        return 0;
        break;
      }
    }
    SiTRACE("blindscan : max_lock_time_ms %d\n", max_lock_time_ms);

    seekStartTime = system_time();

    if (front_end->handshakeUsed == 0) {
      start_resume = 1;
      front_end->searchStartTime = seekStartTime;
    }
    if (front_end->handshakeUsed == 1) {
      min_lock_time_ms = 0;
      /* Recalled after handshaking    */
      if (front_end->handshakeOn == 1) {
        /* Skip tuner and demod settings */
        start_resume = 0;
        SiTRACE("blindscan_handshake : recalled after   handshake. Skipping tuner and demod settings\n");
      }
      /* When recalled after a lock */
      if (front_end->handshakeOn == 0) {
        /* Allow tuner and demod settings */
        start_resume = 1;
        if (blind_mode == 1) { /* DVB-C / DVB-S / DVB-S2 / MCNS */
          if (front_end->demod->cmd->scan_ctrl.action == Si2164B_SCAN_CTRL_CMD_ACTION_START) {
            SiTRACE("blindscan_handshake : no handshake : starting.\n");
          } else {
            SiTRACE("blindscan_handshake : no handshake : resuming.\n");
          }
        }
        front_end->searchStartTime = seekStartTime;
      }
    }
    front_end->handshakeStart_ms = seekStartTime;

    if (start_resume == 1) {
      /* Enabling FEF control for T/T2 */
      switch ( front_end->demod->standard )
      {
        case Si2164B_DD_MODE_PROP_MODULATION_DVBT2 :
        case Si2164B_DD_MODE_PROP_MODULATION_DVBT: {
         #ifdef    INDIRECT_I2C_CONNECTION
          front_end->f_TER_tuner_enable(front_end->callback);
         #else  /* INDIRECT_I2C_CONNECTION */
          Si2164B_L2_Tuner_I2C_Enable(front_end);
         #endif /* INDIRECT_I2C_CONNECTION */
          Si2164B_L2_TER_FEF (front_end,1);
        #ifdef    INDIRECT_I2C_CONNECTION
          front_end->f_TER_tuner_disable(front_end->callback);
        #else  /* INDIRECT_I2C_CONNECTION */
          Si2164B_L2_Tuner_I2C_Disable(front_end);
        #endif /* INDIRECT_I2C_CONNECTION */
          break;
        }
        default                                  : {
          break;
        }
      }
    }

    max_decision_time_ms = max_lock_time_ms;

    /* Select TER channel increment (this value will only be used for 'TER' scanning) */
    channelIncrement = front_end->seekStepHz;

    /* Start Seeking */
    SiTRACE("Si2164B_L2_Channel_Seek_Next front_end->rangeMin %10d,front_end->rangeMax %10d blind_mode %d\n", front_end->rangeMin,front_end->rangeMax, blind_mode);

    seek_freq = front_end->rangeMin;
    SiTraceConfiguration("traces resume");

  if (blind_mode == 0) { /* DVB-T / DVB-T2 / ISDB-T */
    while ( seek_freq <= front_end->rangeMax )
    {
      if (start_resume) {
        SiTraceConfiguration("traces suspend");
        /* Call the Si2164B_L2_Tune command to tune the frequency */
        if (Si2164B_L2_Tune (front_end, seek_freq )!= seek_freq) {
          /* Manage possible tune error */
          SiTRACE("Si2164B_L2_Channel_Seek_Next Tune error at %d, aborting (skipped)\n", seek_freq);
          front_end->seekAbort = 1;
          return 0;
        }
        ddRestartTime = system_time();
        Si2164B_L1_DD_RESTART        (api);
        /* In non-blind mode, the timeout reference is the last DD_RESTART */
        front_end->timeoutStartTime = system_time();
        /* as we will not lock in less than min_lock_time_ms, wait a while... */
        system_wait(min_lock_time_ms);
      }

      jump_to_next_channel = 0;

      while (!jump_to_next_channel) {

        if ((front_end->demod->standard == Si2164B_DD_MODE_PROP_MODULATION_DVBT)
          | (front_end->demod->standard == Si2164B_DD_MODE_PROP_MODULATION_DVBT2)
          ) {
          return_code = Si2164B_L1_DD_STATUS(api, Si2164B_DD_STATUS_CMD_INTACK_CLEAR);
          if (return_code != NO_Si2164B_ERROR) {
            SiTraceConfiguration("traces resume");
            SiTRACE("Si2164B_L2_Channel_Seek_Next: Si2164B_L1_DD_STATUS error at %d, aborting\n", seek_freq);
            front_end->handshakeOn = 0;
            front_end->seekAbort   = 1;
            return 0;
          }
          Si2164B_L1_DD_EXT_AGC_TER (api,
                                    Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_NO_CHANGE,
                                    api->cmd->dd_ext_agc_ter.agc_1_inv,
                                    Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MODE_NO_CHANGE,
                                    api->cmd->dd_ext_agc_ter.agc_2_inv,
                                    api->cmd->dd_ext_agc_ter.agc_1_kloop,
                                    api->cmd->dd_ext_agc_ter.agc_2_kloop,
                                    api->cmd->dd_ext_agc_ter.agc_1_min,
                                    api->cmd->dd_ext_agc_ter.agc_2_min );

          searchDelay = system_time() - front_end->searchStartTime;

          if ( (front_end->demod->rsp->dd_status.dl  == Si2164B_DD_STATUS_RESPONSE_DL_NO_LOCK  ) & ( front_end->demod->rsp->dd_status.rsqstat_bit5   == Si2164B_DD_STATUS_RESPONSE_RSQINT_BIT5_NO_CHANGE ) ) {
            /* Check PCL to refine the max_lock_time_ms value if the standard has been detected */
            if   (front_end->demod->rsp->dd_status.pcl        == Si2164B_DD_STATUS_RESPONSE_PCL_LOCKED) {
              if (front_end->demod->rsp->dd_status.modulation == Si2164B_DD_STATUS_RESPONSE_MODULATION_DVBT ) { max_lock_time_ms = Si2164B_DVBT_MAX_LOCK_TIME ;}
            }
          }
          if ( (front_end->demod->rsp->dd_status.dl  == Si2164B_DD_STATUS_RESPONSE_DL_NO_LOCK  ) & ( front_end->demod->rsp->dd_status.rsqstat_bit5   == Si2164B_DD_STATUS_RESPONSE_RSQINT_BIT5_CHANGED   ) ) {
            SiTraceConfiguration("traces resume");
            decisionDelay = system_time() - ddRestartTime; front_end->cumulativeScanTime = front_end->cumulativeScanTime + decisionDelay; front_end->nbDecisions++;
            SiTRACE ("NO DVBT/T2. Jumping from  %d after %7d ms. Delay from DD_RESTART %4d ms AGC2 %3d\n", seek_freq, searchDelay, decisionDelay, api->rsp->dd_ext_agc_ter.agc_2_level);
            if(seek_freq==front_end->rangeMax) {front_end->rangeMin=seek_freq;}
            seek_freq = seek_freq + channelIncrement;
            jump_to_next_channel = 1;
            start_resume         = 1;
            break;
          }
          if ( (front_end->demod->rsp->dd_status.dl  == Si2164B_DD_STATUS_RESPONSE_DL_LOCKED   ) & ( front_end->demod->rsp->dd_status.rsqstat_bit5   == Si2164B_DD_STATUS_RESPONSE_RSQINT_BIT5_NO_CHANGE ) ) {
            *standard     = front_end->demod->rsp->dd_status.modulation;
            if (*standard == Si2164B_DD_STATUS_RESPONSE_MODULATION_DVBT ) {
              Si2164B_L1_DVBT_STATUS (api, Si2164B_DVBT_STATUS_CMD_INTACK_CLEAR);
              front_end->detected_rf = seek_freq + front_end->demod->rsp->dvbt_status.afc_freq*1000;
              if (front_end->demod->rsp->dvbt_status.hierarchy == Si2164B_DVBT_STATUS_RESPONSE_HIERARCHY_NONE) {
                *stream       = Si2164B_DVBT_HIERARCHY_PROP_STREAM_HP;
              } else {
                *stream       = front_end->demod->prop->dvbt_hierarchy.stream;
              }
              *bandwidth_Hz = front_end->demod->prop->dd_mode.bw*1000000;
              *freq         = front_end->detected_rf;
              SiTraceConfiguration("traces resume");
              decisionDelay = system_time() - ddRestartTime; front_end->cumulativeScanTime = front_end->cumulativeScanTime + decisionDelay; front_end->nbDecisions++;
              SiTRACE ("DVB-T  lock at %d after %7d ms. Delay from DD_RESTART %4d ms AGC2 %3d\n", (front_end->detected_rf)/1000, searchDelay, decisionDelay, api->rsp->dd_ext_agc_ter.agc_2_level);
            }
            if (*standard == Si2164B_DD_STATUS_RESPONSE_MODULATION_DVBT2) {
           #ifdef    INDIRECT_I2C_CONNECTION
             front_end->f_TER_tuner_enable(front_end->callback);
           #else  /* INDIRECT_I2C_CONNECTION */
             Si2164B_L2_Tuner_I2C_Enable(front_end);
           #endif /* INDIRECT_I2C_CONNECTION */
              Si2164B_L2_TER_FEF (front_end,1);
           #ifdef    INDIRECT_I2C_CONNECTION
             front_end->f_TER_tuner_disable(front_end->callback);
           #else  /* INDIRECT_I2C_CONNECTION */
             Si2164B_L2_Tuner_I2C_Disable(front_end);
           #endif /* INDIRECT_I2C_CONNECTION */
              Si2164B_L1_DVBT2_STATUS (api, Si2164B_DVBT2_STATUS_CMD_INTACK_CLEAR);
              *num_plp = api->rsp->dvbt2_status.num_plp;
              front_end->detected_rf = seek_freq + front_end->demod->rsp->dvbt2_status.afc_freq*1000;
              SiTraceConfiguration("traces resume");
              decisionDelay = system_time() - ddRestartTime; front_end->cumulativeScanTime = front_end->cumulativeScanTime + decisionDelay; front_end->nbDecisions++;
              SiTRACE ("DVB-T2 lock at %d after %7d ms. Delay from DD_RESTART %4d ms AGC2 %3d\n", (front_end->detected_rf)/1000, searchDelay, decisionDelay, api->rsp->dd_ext_agc_ter.agc_2_level);
              switch (front_end->demod->prop->dd_mode.bw) {
                case Si2164B_DD_MODE_PROP_BW_BW_1D7MHZ : { *bandwidth_Hz =                                    1700000; break; }
                default                               : { *bandwidth_Hz = front_end->demod->prop->dd_mode.bw*1000000; break; }
              }
              *T2_base_lite = api->rsp->dvbt2_status.t2_mode + 1; /* Adding 1 to match dvbt2_mode.lock_mode values (0='ANY', 1='T2-Base', 2='T2-Lite')*/
              *freq         = front_end->detected_rf;
            }
            /* Set min seek_freq for next seek */
            front_end->rangeMin = seek_freq + front_end->seekBWHz;
            /* Return 1 to signal that the Si2164B is locked on a valid channel */
            front_end->handshakeOn = 0;
            return 1;
          }
        }

        /* timeout management (this should only trigger if the channel is very difficult, i.e. when pcl = 1 and dl = 0 until the timeout) */
        timeoutDelay = system_time() - front_end->timeoutStartTime;
        if (timeoutDelay >= max_lock_time_ms) {
          decisionDelay = system_time() - ddRestartTime; front_end->cumulativeTimeoutTime = front_end->cumulativeTimeoutTime + decisionDelay; front_end->nbTimeouts++;
          SiTraceConfiguration("traces resume");
          SiTRACE ("----------- Timeout from  %d after %7d ms. Delay from DD_RESTART %4d ms AGC2 %3d\n", seek_freq/1000, timeoutDelay, decisionDelay, api->rsp->dd_ext_agc_ter.agc_2_level);
//          SiERROR ("Timeout (blind_mode = 0)\n");
          seek_freq = seek_freq + channelIncrement;
          jump_to_next_channel = 1;
          start_resume         = 1;
          break;
        }

        if (front_end->handshakeUsed) {
          handshakeDelay = system_time() - front_end->handshakeStart_ms;
          if (handshakeDelay >= front_end->handshakePeriod_ms) {
            SiTRACE ("blindscan_handshake : handshake after %5d ms (at %10d). (search delay %6d ms) %*s\n", handshakeDelay, front_end->rangeMin, searchDelay, (searchDelay)/1000,"*");
           *freq                   = seek_freq;
            front_end->rangeMin    = seek_freq;
            front_end->handshakeOn = 1;
            /* The application will check handshakeStart_ms to know whether the blindscan is ended or not */
            return searchDelay;
          } else {
            SiTRACE ("blindscan_handshake : no handshake yet. (handshake delay %6d ms, search delay %6d ms)\n", handshakeDelay, searchDelay);
          }
        }
      /* Check seekAbort flag (set in case of timeout or by the top-level application) */

      if (front_end->seekAbort) {
        /* Abort the SCAN loop to allow it to restart with the new rangeMin frequency */
        SiTRACE ("seek_next no blind_mode >> (abort!)    Si2164B_L1_SCAN_CTRL( front_end->demod, Si2164B_SCAN_CTRL_CMD_ACTION_ABORT)\n");
                  Si2164B_L1_SCAN_CTRL (front_end->demod, Si2164B_SCAN_CTRL_CMD_ACTION_ABORT , 0);
        front_end->handshakeOn = 0;
        return 0;
        break;
      }
        /* Check status every n ms */
        system_wait(10);
      }
    }
  }

  if (blind_mode == 1) { /* DVB-C / DVB-S / DVB-S2 / MCNS */

    if (front_end->tuneUnitHz == 1) {
      seek_freq_kHz = seek_freq/1000;
    } else {
      seek_freq_kHz = seek_freq;
    }

    previous_scan_status = front_end->demod->rsp->scan_status.scan_status;

    /* Checking blindscan status before issuing a 'start' or 'resume' */
    Si2164B_L1_SCAN_STATUS (front_end->demod, Si2164B_SCAN_STATUS_CMD_INTACK_OK);
    SiTRACE("blindscan_status      %s buz %d\n", Si2164B_L2_Trace_Scan_Status(front_end->demod->rsp->scan_status.scan_status),front_end->demod->rsp->scan_status.buz );

    if (front_end->demod->rsp->scan_status.scan_status != previous_scan_status) {
      SiTRACE ("scan_status changed from %s to %s\n", Si2164B_L2_Trace_Scan_Status(previous_scan_status), Si2164B_L2_Trace_Scan_Status(front_end->demod->rsp->scan_status.scan_status));
    }

    if (start_resume) {
      /* Wait for scan_status.buz to be '0' before issuing SCAN_CTRL */
      buzyStartTime = system_time();
      while (front_end->demod->rsp->scan_status.buz == Si2164B_SCAN_STATUS_RESPONSE_BUZ_BUSY) {
        Si2164B_L1_SCAN_STATUS (front_end->demod, Si2164B_SCAN_STATUS_CMD_INTACK_OK);
        SiTRACE ("blindscan_interaction ?? (buzy)   Si2164B_L1_SCAN_STATUS scan_status.buz %d after %d ms\n", front_end->demod->rsp->scan_status.buz, system_time() - buzyStartTime);
        if (system_time() - buzyStartTime > 100) {
        SiTRACE ("blindscan_interaction -- (error)  Si2164B_L1_SCAN_STATUS is always 'BUZY'\n");
          return 0;
        }
      }
      if (front_end->demod->cmd->scan_ctrl.action == Si2164B_SCAN_CTRL_CMD_ACTION_START) {
        SiTRACE ("blindscan_interaction >> (start ) Si2164B_L1_SCAN_CTRL( front_end->demod, %d, %8d) \n", front_end->demod->cmd->scan_ctrl.action, seek_freq_kHz);
      } else {
        SiTRACE ("blindscan_interaction >> (resume) Si2164B_L1_SCAN_CTRL( front_end->demod, %d, %8d) \n", front_end->demod->cmd->scan_ctrl.action, seek_freq_kHz);
      }
      return_code = Si2164B_L1_SCAN_CTRL (front_end->demod,               front_end->demod->cmd->scan_ctrl.action, seek_freq_kHz);
      front_end->demod->cmd->scan_ctrl.action = Si2164B_SCAN_CTRL_CMD_ACTION_RESUME;
      if (return_code != NO_Si2164B_ERROR) {
        SiTRACE ("blindscan_interaction -- (error1) Si2164B_L1_SCAN_CTRL %d      ERROR at %10d (%d)\n!!!!!!!!!!!!!!!!!!!!!!!\n", front_end->demod->cmd->scan_ctrl.action, seek_freq_kHz, front_end->demod->rsp->scan_status.scan_status);
        SiTRACE("scan_status.buz %d\n", front_end->demod->rsp->scan_status.buz);
        return 0;
      }
      /* In blind mode, the timeout reference is the SCAN_CTRL */
      front_end->timeoutStartTime = system_time();
    }
    front_end->demod->cmd->scan_ctrl.action = Si2164B_SCAN_CTRL_CMD_ACTION_RESUME;

    /* The actual search loop... */
    while ( 1 ) {

      Si2164B_L1_CheckStatus (front_end->demod);

      searchDelay = system_time() - front_end->searchStartTime;

      if ( (front_end->demod->status->scanint == Si2164B_STATUS_SCANINT_TRIGGERED) ) {

        /* There is an interaction with the FW, refresh the timeoutStartTime */
        front_end->timeoutStartTime = system_time();

        Si2164B_L1_SCAN_STATUS (front_end->demod, Si2164B_SCAN_STATUS_CMD_INTACK_CLEAR);
        SiTRACE("blindscan_status      %s\n", Si2164B_L2_Trace_Scan_Status(front_end->demod->rsp->scan_status.scan_status) );
        skip_resume = 0;

        buzyStartTime = system_time();
        while (front_end->demod->rsp->scan_status.buz == Si2164B_SCAN_STATUS_RESPONSE_BUZ_BUSY) {
          Si2164B_L1_SCAN_STATUS (front_end->demod, Si2164B_SCAN_STATUS_CMD_INTACK_OK);
          SiTRACE ("blindscan_interaction ?? (buzy)   Si2164B_L1_SCAN_STATUS status.scanint %d scan_status.buz %d after %d ms\n", front_end->demod->rsp->scan_status.STATUS->scanint, front_end->demod->rsp->scan_status.buz, system_time() - buzyStartTime);
          if (system_time() - buzyStartTime > 100) {
            SiTRACE ("blindscan_interaction -- (error)  Si2164B_L1_SCAN_STATUS is always 'BUZY'\n");
            return 0;
          }
        }

        switch (front_end->demod->rsp->scan_status.scan_status) {
          case  Si2164B_SCAN_STATUS_RESPONSE_SCAN_STATUS_TUNE_REQUEST          : {
            SiTRACE ("blindscan_interaction -- (tune  ) SCAN TUNE_REQUEST at %8ld kHz\n", front_end->demod->rsp->scan_status.rf_freq);
            if (front_end->tuneUnitHz == 1) {
              seek_freq = Si2164B_L2_Tune (front_end, front_end->demod->rsp->scan_status.rf_freq*1000 );
              seek_freq_kHz = seek_freq/1000;
            } else {
              seek_freq = Si2164B_L2_Tune (front_end, front_end->demod->rsp->scan_status.rf_freq );
              seek_freq_kHz = seek_freq;
            }
            *freq = front_end->rangeMin = seek_freq;
            /* as we will not lock in less than min_lock_time_ms, wait a while... */
            system_wait(min_lock_time_ms);
            break;
          }
          case  Si2164B_SCAN_STATUS_RESPONSE_SCAN_STATUS_DIGITAL_CHANNEL_FOUND : {
            *standard        = front_end->demod->rsp->scan_status.modulation;
            switch (front_end->demod->rsp->scan_status.modulation) {
              case Si2164B_SCAN_STATUS_RESPONSE_MODULATION_DVBC : {
                *freq            = front_end->demod->rsp->scan_status.rf_freq*1000;
                *symbol_rate_bps = front_end->demod->rsp->scan_status.symb_rate*1000;
                Si2164B_L1_DVBC_STATUS(front_end->demod, Si2164B_DVBC_STATUS_CMD_INTACK_OK);
                front_end->demod->prop->dvbc_symbol_rate.rate = front_end->demod->rsp->scan_status.symb_rate;
                *constellation   = front_end->demod->rsp->dvbc_status.constellation;
                break;
              }
              case Si2164B_SCAN_STATUS_RESPONSE_MODULATION_MCNS : {
                *freq            = front_end->demod->rsp->scan_status.rf_freq*1000;
                *symbol_rate_bps = front_end->demod->rsp->scan_status.symb_rate*1000;
                Si2164B_L1_MCNS_STATUS(front_end->demod, Si2164B_MCNS_STATUS_CMD_INTACK_OK);
                front_end->demod->prop->mcns_symbol_rate.rate = front_end->demod->rsp->scan_status.symb_rate;
                *constellation   = front_end->demod->rsp->mcns_status.constellation;
                break;
              }
              case Si2164B_SCAN_STATUS_RESPONSE_MODULATION_DSS   :
              case Si2164B_SCAN_STATUS_RESPONSE_MODULATION_DVBS  :
              case Si2164B_SCAN_STATUS_RESPONSE_MODULATION_DVBS2 : {
                *freq            = front_end->demod->rsp->scan_status.rf_freq;
                *symbol_rate_bps = front_end->demod->rsp->scan_status.symb_rate*1000;
                if (*standard == Si2164B_SCAN_STATUS_RESPONSE_MODULATION_DVBS ) { front_end->demod->prop->dvbs_symbol_rate.rate  = front_end->demod->rsp->scan_status.symb_rate; }
                if (*standard == Si2164B_SCAN_STATUS_RESPONSE_MODULATION_DVBS2) { front_end->demod->prop->dvbs2_symbol_rate.rate = front_end->demod->rsp->scan_status.symb_rate; }
                front_end->tuner_sat->RF = *freq;
                break;
              }
              default : {
                SiTRACE("Si2164B_L2_Channel_Seek_Next DIGITAL_CHANNEL_FOUND error at %d: un-handled modulation (%d), aborting (skipped)\n", seek_freq, front_end->demod->rsp->scan_status.modulation);
                front_end->seekAbort = 1;
                return 0;
                break;
              }
            }
            /* When locked, clear scanint before returning from SeekNext, to avoid seeing it again on the 'RESUME', with fast i2c platforms */
            Si2164B_L1_SCAN_STATUS (front_end->demod, Si2164B_SCAN_STATUS_CMD_INTACK_CLEAR);
            SiTRACE ("blindscan_interaction -- (locked) SCAN DIGITAL lock at %d kHz after %7d ms. modulation %3d (%s)\n", *freq, searchDelay, *standard, Si2164B_standardName(*standard) );
            front_end->handshakeOn = 0;
            return 1;
            break;
          }
          case  Si2164B_SCAN_STATUS_RESPONSE_SCAN_STATUS_ERROR                 : {
            SiTRACE ("blindscan_interaction -- (error2) SCAN error at %d after %4d ms\n", seek_freq/1000, searchDelay);
            SiERROR ("SCAN status returns 'ERROR'\n");
            front_end->handshakeOn = 0;
            return 0;
            break;
          }
          case  Si2164B_SCAN_STATUS_RESPONSE_SCAN_STATUS_SEARCHING             : {
            SiTRACE("SCAN Searching...\n");
            skip_resume = 1;
            break;
          }
          case  Si2164B_SCAN_STATUS_RESPONSE_SCAN_STATUS_ENDED                 : {
            SiTRACE ("blindscan_interaction -- (ended ) SCAN ENDED\n");
            Si2164B_L1_SCAN_CTRL (front_end->demod, Si2164B_SCAN_CTRL_CMD_ACTION_ABORT , 0);
            front_end->handshakeOn = 0;
            return 0;
            break;
          }
#ifdef    ALLOW_Si2164B_BLINDSCAN_DEBUG
           case  Si2164B_SCAN_STATUS_RESPONSE_SCAN_STATUS_DEBUG                 : {
            SiTRACE ("blindscan_interaction -- (debug) SCAN DEBUG code %d\n", front_end->demod->rsp->scan_status.symb_rate);
            switch (front_end->demod->rsp->scan_status.symb_rate) {
              case 4: { /* SPECTRUM */
              #ifndef   DO_NOT_DRAW_SPECTRUM
                Si2164B_plot(front_end, "spectrum", 0, seek_freq);
              #endif /* DO_NOT_DRAW_SPECTRUM */
                break;
              }
              case 9: { /* TRYLOCK */
              #ifndef   DO_NOT_TRACK_TRYLOCKS
                Si2164B_plot(front_end, "trylock", 0, seek_freq);
              #endif /* DO_NOT_DRAW_SPECTRUM */
                break;
              }
              default: {}
            }
            /* There has been a debug request by the FW, refresh the timeoutStartTime */
            front_end->timeoutStartTime = system_time();
            break;
          }
#else  /* ALLOW_Si2164B_BLINDSCAN_DEBUG */
           case  63                 : {
             SiERROR("blindscan_interaction -- (warning) You probably run a DEBUG fw, so you need to define ALLOW_Si2164B_BLINDSCAN_DEBUG at project level\n");
             break;
           }
#endif /* ALLOW_Si2164B_BLINDSCAN_DEBUG */
          default : {
            SiTRACE("unknown scan_status %d\n", front_end->demod->rsp->scan_status.scan_status);
            skip_resume = 1;
            break;
          }
        }

        if (skip_resume == 0) {
          SiTRACE ("blindscan_interaction >> (resume) Si2164B_L1_SCAN_CTRL( front_end->demod, %d, %8d)\n", front_end->demod->cmd->scan_ctrl.action, seek_freq_kHz);
          return_code = Si2164B_L1_SCAN_CTRL (front_end->demod,              front_end->demod->cmd->scan_ctrl.action, seek_freq_kHz);
          if (return_code != NO_Si2164B_ERROR) {
            SiTRACE ( "Si2164B_L1_SCAN_CTRL ERROR at %d (%d)\n!!!!!!!!!!!!!!!!!!!!!!!\n", seek_freq_kHz, front_end->demod->rsp->scan_status.scan_status);
            SiERROR ( "Si2164B_L1_SCAN_CTRL 'RESUME' ERROR during seek loop\n");
          }
        }
      }

      /* timeout management (this should never happen if timeout values are correctly set) */
      timeoutDelay = system_time() - front_end->timeoutStartTime;
      if (timeoutDelay >= max_decision_time_ms) {
        SiTRACE ("Scan decision timeout from  %d after %d ms. Check your timeout limits!\n", seek_freq_kHz, timeoutDelay);
        SiERROR ("Scan decision timeout (blind_mode = 1)\n");
        front_end->seekAbort   = 1;
        front_end->handshakeOn = 0;
        break;
      }

        if (front_end->handshakeUsed) {
        handshakeDelay = system_time() - front_end->handshakeStart_ms;
        if (handshakeDelay >= front_end->handshakePeriod_ms) {
          SiTRACE ("blindscan_handshake : handshake after %5d ms (at %10d). (search delay %6d ms) %*s\n", handshakeDelay, front_end->rangeMin, searchDelay, (searchDelay)/1000,"*");
         *freq                    = seek_freq;
          front_end->handshakeOn = 1;
          /* The application will check handshakeStart_ms to know whether the blindscan is ended or not */
          return searchDelay;
        } else {
          SiTRACE ("blindscan_handshake : no handshake yet. (handshake delay %6d ms, search delay %6d ms)\n", handshakeDelay, searchDelay);
        }
      }

      /* Check seekAbort flag (set in case of timeout or by the top-level application) */
      if (front_end->seekAbort) {
        /* Abort the SCAN loop to allow it to restart with the new rangeMin frequency */
        SiTRACE ("blindscan_interaction >> (abort!) Si2164B_L1_SCAN_CTRL( front_end->demod, Si2164B_SCAN_CTRL_CMD_ACTION_ABORT)\n");
                  Si2164B_L1_SCAN_CTRL (front_end->demod, Si2164B_SCAN_CTRL_CMD_ACTION_ABORT , 0);
        front_end->handshakeOn = 0;
        return 0;
        break;
      }

      /* Check status every 100 ms */
      system_wait(100);

    }
  }
  front_end->handshakeOn = 0;
  return 0;
}
/************************************************************************************************************************
  NAME: Si2164B_L2_Channel_Seek_Abort
  DESCRIPTION: aborts the seek
  Programming Guide Reference:    Flowchart TBD (Channel Scan flowchart)

  Parameter:  Pointer to Si2164B Context
  Returns:    0 if successful, otherwise an error.
************************************************************************************************************************/
signed int  Si2164B_L2_Channel_Seek_Abort (Si2164B_L2_Context *front_end)
{
  front_end->seekAbort   = 1;
  front_end->handshakeOn = 0;
  return 0;
}
/************************************************************************************************************************
  NAME: Si2164B_L2_Channel_Lock_Abort
  DESCRIPTION: aborts the lock_to_carrier
  Programming Guide Reference:    Flowchart TBD (Channel Lock flowchart)

  Parameter:  Pointer to Si2164B Context
  Returns:    0 if successful, otherwise an error.
************************************************************************************************************************/
signed int  Si2164B_L2_Channel_Lock_Abort (Si2164B_L2_Context *front_end)
{
  front_end->lockAbort = 1;
  front_end->handshakeOn = 0;
  return 0;
}
/************************************************************************************************************************
  NAME: Si2164B_L2_Channel_Seek_End
  DESCRIPTION: returns the chip back to normal use following a seek sequence
  Programming Guide Reference:    Flowchart TBD (Channel Scan flowchart)

  Parameter:  Pointer to Si2164B Context
  Returns:    0 if successful, otherwise an error.
************************************************************************************************************************/
signed int  Si2164B_L2_Channel_Seek_End   (Si2164B_L2_Context *front_end)
{
  front_end = front_end; /* To avoid compiler warning */
#ifdef    Si2167B_BLINDSCAN_PATCH
  if  ((((api->rsp->part_info.part == 67 ) & (api->rsp->part_info.chiprev + 0x40 == 'B'))
     || ((api->rsp->part_info.part == 65 ) & (api->rsp->part_info.chiprev + 0x40 == 'B'))
    ) & ((api->rsp->part_info.pmajor == '2') & (api->rsp->part_info.pminor == '2')) ) {
    switch (front_end->demod->standard)
    {
      case Si2164B_DD_MODE_PROP_MODULATION_MCNS    :
      case Si2164B_DD_MODE_PROP_MODULATION_DVBC    : {
        front_end->previous_standard = Si2164B_DD_MODE_PROP_MODULATION_DVBT;
        front_end->demod->load_DVB_C_Blindlock_Patch = 0;
        front_end->Si2164B_init_done = 0;
        Si2164B_L2_switch_to_standard(front_end, Si2164B_DD_MODE_PROP_MODULATION_DVBC, 0);
        break;
      }
      default: break;
    }
  }
#endif /* Si2167B_BLINDSCAN_PATCH */

  front_end->demod->prop->scan_ien.buzien           = Si2164B_SCAN_IEN_PROP_BUZIEN_DISABLE ; /* (default 'DISABLE') */
  front_end->demod->prop->scan_ien.reqien           = Si2164B_SCAN_IEN_PROP_REQIEN_DISABLE ; /* (default 'DISABLE') */
  Si2164B_L1_SetProperty2(front_end->demod, Si2164B_SCAN_IEN_PROP_CODE);
  SiTRACE("front_end->cumulativeScanTime    %d %d'%d'' (%d decisions)\n", front_end->cumulativeScanTime   , front_end->cumulativeScanTime   /60000, (front_end->cumulativeScanTime   /1000)%60, front_end->nbDecisions );
  SiTRACE("front_end->cumulativeTimeoutTime %d %d'%d'' (%d timeouts )\n", front_end->cumulativeTimeoutTime, front_end->cumulativeTimeoutTime/60000, (front_end->cumulativeTimeoutTime/1000)%60, front_end->nbTimeouts  );
  switch (front_end->demod->standard)
  {
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT    : { front_end->demod->prop->dd_mode.modulation = Si2164B_DD_MODE_PROP_MODULATION_DVBT ; break; }
    case Si2164B_DD_MODE_PROP_MODULATION_DVBC    : { front_end->demod->prop->dd_mode.modulation = Si2164B_DD_MODE_PROP_MODULATION_DVBC ; break; }
    case Si2164B_DD_MODE_PROP_MODULATION_MCNS    : { front_end->demod->prop->dd_mode.modulation = Si2164B_DD_MODE_PROP_MODULATION_MCNS ; break; }
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT2   : { front_end->demod->prop->dd_mode.modulation = Si2164B_DD_MODE_PROP_MODULATION_DVBT2; break; }
    case Si2164B_DD_MODE_PROP_MODULATION_DVBS    : { front_end->demod->prop->dd_mode.modulation = Si2164B_DD_MODE_PROP_MODULATION_DVBS ; break; }
    case Si2164B_DD_MODE_PROP_MODULATION_DVBS2   : { front_end->demod->prop->dd_mode.modulation = Si2164B_DD_MODE_PROP_MODULATION_DVBS2; break; }
    case Si2164B_DD_MODE_PROP_MODULATION_DSS     : { front_end->demod->prop->dd_mode.modulation = Si2164B_DD_MODE_PROP_MODULATION_DSS  ; break; }
    default                                     : { SiTRACE("UNKNOWN standard %d\n", front_end->demod->standard); break;}
  }

  SiTRACE("auto_detect_TER %d\n",front_end->auto_detect_TER);
  if (front_end->auto_detect_TER) {
    switch (front_end->demod->standard)
    {
      case Si2164B_DD_MODE_PROP_MODULATION_DVBT    :
      case Si2164B_DD_MODE_PROP_MODULATION_DVBT2   : {
        front_end->demod->prop->dd_mode.modulation  = Si2164B_DD_MODE_PROP_MODULATION_AUTO_DETECT;
        front_end->demod->prop->dd_mode.auto_detect = Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_T_T2;
      }
      default                                     : { break;}
    }
  }
  if (front_end->auto_detect_SAT) {
    switch (front_end->demod->standard)
    {
      case Si2164B_DD_MODE_PROP_MODULATION_DSS     :
      case Si2164B_DD_MODE_PROP_MODULATION_DVBS    :
      case Si2164B_DD_MODE_PROP_MODULATION_DVBS2   : {
        front_end->demod->prop->dd_mode.modulation  = Si2164B_DD_MODE_PROP_MODULATION_AUTO_DETECT;
        if (front_end->demod->standard == Si2164B_DD_MODE_PROP_MODULATION_DSS) {
          front_end->demod->prop->dd_mode.auto_detect = Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_S_S2_DSS;
        } else {
          front_end->demod->prop->dd_mode.auto_detect = Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_S_S2;
        }
      }
      default                                     : { break;}
    }
  }
  Si2164B_L1_SetProperty2(front_end->demod, Si2164B_DD_MODE_PROP_CODE);

  return 0;
}
/************************************************************************************************************************
  NAME: Si2164B_SatAutoDetect
  DESCRIPTION: Set the Si2164B in Sat Auto Detect mode

  Parameter:  Pointer to Si2164B Context
  Returns:    front_end->auto_detect_SAT
************************************************************************************************************************/
signed int  Si2164B_SatAutoDetect         (Si2164B_L2_Context *front_end)
{
  front_end->auto_detect_SAT = 1;
  return front_end->auto_detect_SAT;
}
/************************************************************************************************************************
  NAME: Si2164B_SatAutoDetectOff
  DESCRIPTION: Set the Si2164B in Sat Auto Detect 'off' mode

  Parameter:  Pointer to Si2164B Context
  Returns:    front_end->auto_detect_SAT
************************************************************************************************************************/
signed int  Si2164B_SatAutoDetectOff      (Si2164B_L2_Context *front_end)
{
  front_end->auto_detect_SAT = 0;
  return front_end->auto_detect_SAT;
}
/************************************************************************************************************************
  NAME: Si2164B_TerAutoDetect
  DESCRIPTION: Set the Si2164B in Ter Auto Detect mode

  Parameter:  Pointer to Si2164B Context
  Returns:    front_end->auto_detect_TER
************************************************************************************************************************/
signed int  Si2164B_TerAutoDetect         (Si2164B_L2_Context *front_end)
{
  if (   (front_end->demod->rsp->part_info.part == 60)
      || (front_end->demod->rsp->part_info.part == 63)
      || (front_end->demod->rsp->part_info.part == 65)
      || (front_end->demod->rsp->part_info.part == 66)
      || (front_end->demod->rsp->part_info.part == 67)
      || (front_end->demod->rsp->part_info.part == 80)
      || (front_end->demod->rsp->part_info.part == 81) ) {
    front_end->auto_detect_TER = 0;
    SiERROR("ERROR: your part doesn't support DVB-T2. auto_detect_TER forced to 0\n");
  } else {
    front_end->auto_detect_TER = 1;
  }
  return front_end->auto_detect_TER;
}
/************************************************************************************************************************
  NAME: Si2164B_TerAutoDetectOff
  DESCRIPTION: Set the Si2164B in Ter Auto Detect 'off' mode

  Parameter:  Pointer to Si2164B Context
  Returns:    front_end->auto_detect_TER
************************************************************************************************************************/
signed int  Si2164B_TerAutoDetectOff      (Si2164B_L2_Context *front_end)
{
  front_end->auto_detect_TER = 0;
  return front_end->auto_detect_TER;
}

//20130903,related to kernel driver,csm
/************************************************************************************************************************
  Si2164B_L2_firmwares_download function
  Use:      FW download in TER tuner and DEMOD function
  Use:      This is the 'INIT1' phase
  parameter: clk_mode(0:XTAL, 1:EXT CLK)

  Behavior: 1- initialize the context
            2- i2c connect
            3- allow i2c communication with tuners
            4- download TER tuner firmware
            5- download Si2164B firmware
            6- free the memory
            7- exit ('0' if OK, any other value indicates an error)
************************************************************************************************************************/
signed int  Si2164B_L2_firmwares_download (Si2164B_L2_Context *front_end,signed int clk_mode
                                   , signed int demodAdd
                                   , signed int tunerAdd_Ter
                                   , signed int tunerAdd_Sat)
{
  signed int res;
  signed int start_time_ms=0;
  //Si2164B_L2_Context *front_end;
  //Si2164B_L2_Context  front_end_Memory;
  res = 1;
  start_time_ms = start_time_ms;
  start_time_ms = system_time();
  //front_end = &front_end_Memory;
  Si2164B_L2_SW_Init            (front_end, demodAdd, tunerAdd_Ter, tunerAdd_Sat, (void*)front_end);
  SiTRACE("tracing times... SW init in %d ms\n", system_time() - start_time_ms);  start_time_ms = system_time();
  Si2164B_L2_HW_Connect         (front_end, CUSTOMER);
  SiTRACE("tracing times... connect in %d ms\n", system_time() - start_time_ms);  start_time_ms = system_time();
  Si2164B_L2_Tuner_I2C_Enable   (front_end);
  SiTRACE("tracing times... I2C enable in %d ms\n", system_time() - start_time_ms);  start_time_ms = system_time();
  start_time_ms = system_time();

  if (clk_mode == 1) {  // EXT CLK
    front_end->tuner_ter->cmd->power_up.clock_mode      = Si2151_POWER_UP_CMD_CLOCK_MODE_EXTCLK;       /* options: XTAL     EXTCLK   */
    front_end->tuner_ter->cmd->power_up.en_xout         = Si2151_POWER_UP_CMD_EN_XOUT_EN_XOUT;         /* options: EN_XOUT  DIS_XOUT */
    front_end->tuner_ter->cmd->config_clocks.clock_mode = Si2151_CONFIG_CLOCKS_CMD_CLOCK_MODE_EXTCLK;  /* options: XTAL     EXTCLK   */
  }
  else if (clk_mode == 0) {  // XTAL
	front_end->tuner_ter->cmd->power_up.clock_mode      = Si2151_POWER_UP_CMD_CLOCK_MODE_XTAL;         /* options: XTAL     EXTCLK   */
    front_end->tuner_ter->cmd->power_up.en_xout         = Si2151_POWER_UP_CMD_EN_XOUT_EN_XOUT;         /* options: EN_XOUT  DIS_XOUT */
    front_end->tuner_ter->cmd->config_clocks.clock_mode = Si2151_CONFIG_CLOCKS_CMD_CLOCK_MODE_XTAL;    /* options: XTAL     EXTCLK   */
  }
  if(tunerAdd_Ter != TER1_MAIN_ADDRESS) // for Single models
  {
	  res = Si2151_PowerUpWithPatch(front_end->tuner_ter);
	  if (res != 0) {
		goto exit_fw_download;
	  }
	  SiTRACE("tracing times... TER tuner fw download in %d ms\n", system_time() - start_time_ms);  start_time_ms = system_time();
	  if ((res = TER_TUNER_CLOCK_ON(front_end->tuner_ter) ) !=0) {
		return 0;
	  }
	  SiTRACE("tracing times... TER Tuner clock on in %d ms\n", system_time() - start_time_ms);  start_time_ms = system_time();
  }
  Si2164B_L2_Tuner_I2C_Disable(front_end);
  SiTRACE("tracing times... I2C Disable in %d ms\n", system_time() - start_time_ms);  start_time_ms = system_time();
  if ((res = Si2164B_Init       (front_end->demod)) != NO_Si2164B_ERROR) {
    goto exit_fw_download;
  }
  SiTRACE("tracing times... Si2164B fw download in %d ms\n", system_time() - start_time_ms);  start_time_ms = system_time();
  exit_fw_download:
  return res;
}

void  Si2164B_publishFirmwarePointer(Si2164B_L2_Context *front_end,unsigned long *fw_addr,unsigned long *fw_size)
{
    if  (((front_end->demod->rsp->part_info.romid  == Si2164C_PATCH16_6_0b7_ROM   )
    & (
        ((front_end->demod->rsp->part_info.part == 60 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'C') )
        || ((front_end->demod->rsp->part_info.part == 62 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'C') )
        || ((front_end->demod->rsp->part_info.part == 64 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'C') )
        || ((front_end->demod->rsp->part_info.part == 66 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'D') )
        || ((front_end->demod->rsp->part_info.part == 67 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'D') )
        || ((front_end->demod->rsp->part_info.part == 68 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'D') )
        || ((front_end->demod->rsp->part_info.part == 69 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'D') )
        || ((front_end->demod->rsp->part_info.part == 0  ) & (1) )
    ) & (
        (front_end->demod->rsp->part_info.pmajor == Si2164C_PATCH16_6_0b7_PMAJOR)
        & (front_end->demod->rsp->part_info.pminor == Si2164C_PATCH16_6_0b7_PMINOR)
        & (front_end->demod->rsp->part_info.pbuild == Si2164C_PATCH16_6_0b7_PBUILD)
    ) )
    ) 
	{
		*fw_addr=(unsigned long)Si2164C_PATCH16_6_0b7;
		*fw_size=sizeof(Si2164C_PATCH16_6_0b7);
	}

    else if  (((front_end->demod->rsp->part_info.romid  == Si2164C_FIRMWARE16_5_Bb4_ROM   )
    & (
        ((front_end->demod->rsp->part_info.part == 60 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'C') )
        || ((front_end->demod->rsp->part_info.part == 62 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'C') )
        || ((front_end->demod->rsp->part_info.part == 64 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'C') )
        || ((front_end->demod->rsp->part_info.part == 66 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'D') )
        || ((front_end->demod->rsp->part_info.part == 67 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'D') )
        || ((front_end->demod->rsp->part_info.part == 68 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'D') )
        || ((front_end->demod->rsp->part_info.part == 69 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'D') )
    ) & (
        (front_end->demod->rsp->part_info.pmajor == Si2164C_FIRMWARE16_5_Bb4_PMAJOR)
        & (front_end->demod->rsp->part_info.pminor == Si2164C_FIRMWARE16_5_Bb4_PMINOR)
        & (front_end->demod->rsp->part_info.pbuild == Si2164C_FIRMWARE16_5_Bb4_PBUILD)
    ) )
    )
	{
		*fw_addr=(unsigned long)Si2164C_FIRMWARE16_5_Bb4;
		*fw_size=sizeof(Si2164C_FIRMWARE16_5_Bb4);
	}

	else if  (((front_end->demod->rsp->part_info.romid  == Si2164B_PATCH16_5_6b10_ROM   )
    & (
        ((front_end->demod->rsp->part_info.part == 60 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'B') )
        || ((front_end->demod->rsp->part_info.part == 62 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'B') )
        || ((front_end->demod->rsp->part_info.part == 64 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'B') )
        || ((front_end->demod->rsp->part_info.part == 68 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'C') )
        || ((front_end->demod->rsp->part_info.part == 69 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'C') )
    ) & (
        (front_end->demod->rsp->part_info.pmajor == Si2164B_PATCH16_5_6b10_PMAJOR)
        & (front_end->demod->rsp->part_info.pminor == Si2164B_PATCH16_5_6b10_PMINOR)
        & (front_end->demod->rsp->part_info.pbuild == Si2164B_PATCH16_5_6b10_PBUILD)
    ) )
    )
	{
		*fw_addr=(unsigned long)Si2164B_PATCH16_5_6b10;
		*fw_size=sizeof(Si2164B_PATCH16_5_6b10);
	}

    else if  (((front_end->demod->rsp->part_info.romid  == Si2164B_PATCH16_5_1b8_ROM   )
    & (
        ((front_end->demod->rsp->part_info.part == 60 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'B') )
        || ((front_end->demod->rsp->part_info.part == 62 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'B') )
        || ((front_end->demod->rsp->part_info.part == 64 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'B') )
        || ((front_end->demod->rsp->part_info.part == 68 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'C') )
        || ((front_end->demod->rsp->part_info.part == 69 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'C') )
    ) & (
        (front_end->demod->rsp->part_info.pmajor == Si2164B_PATCH16_5_1b8_PMAJOR)
        & (front_end->demod->rsp->part_info.pminor == Si2164B_PATCH16_5_1b8_PMINOR)
        & (front_end->demod->rsp->part_info.pbuild == Si2164B_PATCH16_5_1b8_PBUILD)
    ) )
    )
	{
		*fw_addr=(unsigned long)Si2164B_PATCH16_5_1b8;
		*fw_size=sizeof(Si2164B_PATCH16_5_1b8);
	}

    else if  (((front_end->demod->rsp->part_info.romid  == Si2164B_FIRMWARE_ES_FW_ROM   )
    &(
        ((front_end->demod->rsp->part_info.part == 69 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'C') )
        || ((front_end->demod->rsp->part_info.part == 68 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'C') )
        || ((front_end->demod->rsp->part_info.part == 64 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'B') )
        || ((front_end->demod->rsp->part_info.part == 62 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'B') )
        || ((front_end->demod->rsp->part_info.part == 60 ) & (front_end->demod->rsp->part_info.chiprev + 0x40 == 'B') )
    ) )
    /*  One line for compatibility with early Si2164B and derivatives samples with NO NVM content */
    || ((front_end->demod->rsp->part_info.romid  == 0   ) & ( front_end->demod->rsp->part_info.part == 0  ) )
    )
	{
		*fw_addr=(unsigned long)Si2164B_FIRMWARE_ES_FW;
		*fw_size=sizeof(Si2164B_FIRMWARE_ES_FW);
	}
}
//20130903,end


#ifdef    SILABS_API_TEST_PIPE
signed int   Si2164B_L2_GET_REG           (Si2164B_L2_Context *front_end, unsigned char   reg_code_lsb, unsigned char   reg_code_mid, unsigned char   reg_code_msb) {
  signed int res;
  SiTraceConfiguration("traces suspend");
  if (Si2164B_L1_DD_GET_REG  (front_end->demod, reg_code_lsb, reg_code_mid, reg_code_msb) != NO_Si2164B_ERROR) {
    SiERROR("Error calling Si2164B_L1_DD_GET_REG\n");
    SiTraceConfiguration("traces resume");
    return 0;
  }
  SiTraceConfiguration("traces resume");
  res =  (front_end->demod->rsp->dd_get_reg.data4<<24)
        +(front_end->demod->rsp->dd_get_reg.data3<<16)
        +(front_end->demod->rsp->dd_get_reg.data2<<8 )
        +(front_end->demod->rsp->dd_get_reg.data1<<0 );
  return res;
}
#define get_reg(ptr, register)               SiTRACE("%s %3d  ", #register, Si2164B_L2_GET_REG (ptr, Si2164B_##register##_LSB, Si2164B_##register##_MID, Si2164B_##register##_MSB));
#define get_reg_from_code(ptr,register)      SiTRACE("%s %3d  ", #register, Si2164B_L2_GET_REG (ptr, Si2164B_##register##_CODE));
#if 1  /* Codes for GET_REG */
  #define Si2164B_auto_relock_CODE          0,78,10
  #define Si2164B_en_rst_error_CODE         0,244,9
  #define Si2164B_demod_lock_t_CODE         0,0,8
  #define Si2164B_fec_lock_t2_CODE          0,140,18
  #define Si2164B_tbd_a_CODE               15,88,10
  #define Si2164B_trap_type_CODE            7,246,9
  #define Si2164B_firmware_sm_CODE          7,76,10
  #define Si2164B_pre_crc_ok_CODE           0,82,12
  #define Si2164B_pre_crc_nok_tog_CODE      0,84,12
  #define Si2164B_l1_post_crc_ok_CODE       0,14,12
  #define Si2164B_l1_post_crc_nok_tog_CODE  0,15,12
  #define Si2164B_td_alarms_CODE            8,148,14
  #define Si2164B_td_alarms_first_CODE      8,144,14
  #define Si2164B_fd_alarms_CODE           31,56,14
  #define Si2164B_fd_alarms_first_CODE     31,60,14
  #define Si2164B_mp_a_mux_CODE             3,106,1
  #define Si2164B_mp_b_mux_CODE             3,109,1
  #define Si2164B_mp_c_mux_CODE             3,112,1
  #define Si2164B_mp_d_mux_CODE             3,115,1
  #define Si2164B_agc1_cmd_CODE             7,38,4
  #define Si2164B_agc2_cmd_CODE             7,50,4
  #define Si2164B_agc1_pola_CODE           33,36,4
  #define Si2164B_agc2_pola_CODE           33,48,4
  #define Si2164B_agc1_min_CODE             7,34,4
  #define Si2164B_agc1_max_CODE             7,35,4
  #define Si2164B_agc1_freeze_CODE          0,36,4
  #define Si2164B_agc2_min_CODE             7,46,4
  #define Si2164B_agc2_max_CODE             7,47,4
  #define Si2164B_agc2_freeze_CODE          0,48,4
  #define Si2164B_standard_CODE             5,116,4
  #define Si2164B_mailbox61_CODE            7,165,0
  #define Si2164B_wdog_error_CODE          66,245,9
#endif /* Codes for GET_REG */
signed int   Si2164B_L2_CheckLoop         (Si2164B_L2_Context *front_end) {

  get_reg_from_code(front_end,mp_a_mux);
  get_reg_from_code(front_end,mp_b_mux);
  get_reg_from_code(front_end,mp_c_mux);
  get_reg_from_code(front_end,mp_d_mux);

  get_reg_from_code(front_end,firmware_sm);
  get_reg_from_code(front_end,agc1_min);
  get_reg_from_code(front_end,agc2_min);
  get_reg_from_code(front_end,agc1_max);
  get_reg_from_code(front_end,agc2_max);
  get_reg_from_code(front_end,agc1_pola);
  get_reg_from_code(front_end,agc2_pola);
  get_reg_from_code(front_end,agc1_freeze);
  get_reg_from_code(front_end,agc2_freeze);
  get_reg_from_code(front_end,agc1_cmd);
  get_reg_from_code(front_end,agc2_cmd);

  safe_printf("\n");

  safe_printf("\n");
  return 1;
}
signed int   Si2164B_L2_Health_Check      (Si2164B_L2_Context *front_end) {
  signed int standard;
  signed int firmware_sm;
  signed int mailbox61;
  signed int tbd_a;
  signed int wdog_error;
  signed int trap_type;

  standard    = Si2164B_L2_GET_REG (front_end, Si2164B_standard_CODE);
  firmware_sm = Si2164B_L2_GET_REG (front_end, Si2164B_firmware_sm_CODE);
  tbd_a       = Si2164B_L2_GET_REG (front_end, Si2164B_tbd_a_CODE);
  mailbox61   = Si2164B_L2_GET_REG (front_end, Si2164B_mailbox61_CODE);
  wdog_error  = Si2164B_L2_GET_REG (front_end, Si2164B_wdog_error_CODE);
  trap_type   = Si2164B_L2_GET_REG (front_end, Si2164B_trap_type_CODE);

  SiTRACE("std %2d, fw_sm %4d / tbd_a %4d / mb61 %4d / wdog %4d / trap_type %4d\n", standard, firmware_sm, tbd_a, mailbox61, wdog_error, trap_type);

  return 1;
}
/*#include "Si_2164B_dump.c"*/
/* #include "Si_2164B_Get_Echoes.c" */
/*#include "SiLabs_DVB_T2_Signalling.h"*/
#ifdef    _DVB_T2_SIGNALLING_H_
#if 1

#define Si2164B_get_reg(ptr,register)       Si2164B_L2_GET_REG            (ptr, Si2164B_##register##_LSB, Si2164B_##register##_MID, Si2164B_##register##_MSB)

#define Si2164B_pre_type_LSB   7
#define Si2164B_pre_type_MID   24
#define Si2164B_pre_type_MSB   13

#define Si2164B_pre_bwt_ext_LSB   0
#define Si2164B_pre_bwt_ext_MID   25
#define Si2164B_pre_bwt_ext_MSB   13

#define Si2164B_pre_s1_LSB   2
#define Si2164B_pre_s1_MID   26
#define Si2164B_pre_s1_MSB   13

#define Si2164B_pre_s2_LSB   3
#define Si2164B_pre_s2_MID   27
#define Si2164B_pre_s2_MSB   13

#define Si2164B_pre_l1_repetition_flag_LSB   0
#define Si2164B_pre_l1_repetition_flag_MID   28
#define Si2164B_pre_l1_repetition_flag_MSB   13

#define Si2164B_pre_guard_interval_LSB   2
#define Si2164B_pre_guard_interval_MID   29
#define Si2164B_pre_guard_interval_MSB   13

#define Si2164B_pre_papr_LSB   3
#define Si2164B_pre_papr_MID   30
#define Si2164B_pre_papr_MSB   13

#define Si2164B_pre_l1_mod_LSB   3
#define Si2164B_pre_l1_mod_MID   31
#define Si2164B_pre_l1_mod_MSB   13

#define Si2164B_pre_l1_cod_LSB   1
#define Si2164B_pre_l1_cod_MID   32
#define Si2164B_pre_l1_cod_MSB   13

#define Si2164B_pre_l1_fec_type_LSB   1
#define Si2164B_pre_l1_fec_type_MID   33
#define Si2164B_pre_l1_fec_type_MSB   13

#define Si2164B_pre_l1_post_size_LSB   17
#define Si2164B_pre_l1_post_size_MID   36
#define Si2164B_pre_l1_post_size_MSB   13

#define Si2164B_pre_l1_post_info_size_LSB   17
#define Si2164B_pre_l1_post_info_size_MID   40
#define Si2164B_pre_l1_post_info_size_MSB   13

#define Si2164B_pre_pilot_pattern_LSB   3
#define Si2164B_pre_pilot_pattern_MID   44
#define Si2164B_pre_pilot_pattern_MSB   13

#define Si2164B_pre_tx_id_availability_LSB   7
#define Si2164B_pre_tx_id_availability_MID   45
#define Si2164B_pre_tx_id_availability_MSB   13

#define Si2164B_pre_cell_id_LSB   15
#define Si2164B_pre_cell_id_MID   46
#define Si2164B_pre_cell_id_MSB   13

#define Si2164B_pre_network_id_LSB   15
#define Si2164B_pre_network_id_MID   48
#define Si2164B_pre_network_id_MSB   13

#define Si2164B_pre_t2_system_id_LSB   15
#define Si2164B_pre_t2_system_id_MID   50
#define Si2164B_pre_t2_system_id_MSB   13

#define Si2164B_pre_num_t2_frames_LSB   7
#define Si2164B_pre_num_t2_frames_MID   52
#define Si2164B_pre_num_t2_frames_MSB   13

#define Si2164B_pre_num_data_symbols_LSB   11
#define Si2164B_pre_num_data_symbols_MID   54
#define Si2164B_pre_num_data_symbols_MSB   13

#define Si2164B_pre_regen_flag_LSB   2
#define Si2164B_pre_regen_flag_MID   56
#define Si2164B_pre_regen_flag_MSB   13

#define Si2164B_pre_l1_post_extension_LSB   0
#define Si2164B_pre_l1_post_extension_MID   57
#define Si2164B_pre_l1_post_extension_MSB   13

#define Si2164B_pre_num_rf_LSB   2
#define Si2164B_pre_num_rf_MID   58
#define Si2164B_pre_num_rf_MSB   13

#define Si2164B_pre_reserved_LSB   5
#define Si2164B_pre_reserved_MID   61
#define Si2164B_pre_reserved_MSB   13

#define Si2164B_num_rf_LSB   2
#define Si2164B_num_rf_MID   210
#define Si2164B_num_rf_MSB   13

#define Si2164B_pre_current_rf_idx_LSB   2
#define Si2164B_pre_current_rf_idx_MID   59
#define Si2164B_pre_current_rf_idx_MSB   13

#define Si2164B_pre_t2_version_LSB   3
#define Si2164B_pre_t2_version_MID   60
#define Si2164B_pre_t2_version_MSB   13

#define Si2164B_bbdf_data_issyi_status_LSB   1
#define Si2164B_bbdf_data_issyi_status_MID   122
#define Si2164B_bbdf_data_issyi_status_MSB   20

#define Si2164B_bbdf_comm_issyi_status_LSB   1
#define Si2164B_bbdf_comm_issyi_status_MID   123
#define Si2164B_bbdf_comm_issyi_status_MSB   20

#define Si2164B_bbdf_data_npd_active_LSB   0
#define Si2164B_bbdf_data_npd_active_MID   120
#define Si2164B_bbdf_data_npd_active_MSB   20

#define Si2164B_bbdf_comm_npd_active_LSB   0
#define Si2164B_bbdf_comm_npd_active_MID   121
#define Si2164B_bbdf_comm_npd_active_MSB   20

#define Si2164B_djb_alarm_data_LSB   11
#define Si2164B_djb_alarm_data_MID   48
#define Si2164B_djb_alarm_data_MSB   22

#define Si2164B_djb_alarm_comm_LSB   11
#define Si2164B_djb_alarm_comm_MID   50
#define Si2164B_djb_alarm_comm_MSB   22

#define Si2164B_ts_rate_in_LSB   27
#define Si2164B_ts_rate_in_MID   72
#define Si2164B_ts_rate_in_MSB   22

#define Si2164B_storing_enable_LSB   0
#define Si2164B_storing_enable_MID   72
#define Si2164B_storing_enable_MSB   14

#define Si2164B_stored_l1_post_nb_LSB   12
#define Si2164B_stored_l1_post_nb_MID   74
#define Si2164B_stored_l1_post_nb_MSB   14

#define Si2164B_start_ibs_storing_data_LSB   10
#define Si2164B_start_ibs_storing_data_MID   76
#define Si2164B_start_ibs_storing_data_MSB   14

#define Si2164B_store_l1_post_en_LSB   33
#define Si2164B_store_l1_post_en_MID   72
#define Si2164B_store_l1_post_en_MSB   14

#define Si2164B_base_read_address_LSB   10
#define Si2164B_base_read_address_MID   84
#define Si2164B_base_read_address_MSB   14

#define Si2164B_read_data_LSB   31
#define Si2164B_read_data_MID   88
#define Si2164B_read_data_MSB   14

#define Si2164B_read_data_latch_LSB   33
#define Si2164B_read_data_latch_MID   114
#define Si2164B_read_data_latch_MSB   12

#endif

unsigned int   Si2164B_n_bits             (unsigned int *wordTable, signed int n, unsigned int *bitIndex) {
  signed int startByteIndex;
  signed int stopByteIndex;
  signed int leftBitShift;
  signed int bitShift;
  signed int wordShift;
  signed int intSize;
  signed int b;
  unsigned int res;
  SiTRACE("Si2164B_n_bits(wordTable, %2d, %4d) ", n, *bitIndex);
  intSize        = sizeof(signed int)*8;
  res            = 0x00000000;
  wordShift      = 0;
  startByteIndex =  *bitIndex/intSize;
  stopByteIndex  = (*bitIndex+n-1)/intSize;
  leftBitShift   = (*bitIndex) - (startByteIndex*intSize);
  bitShift       = ((stopByteIndex+1)*intSize) - (*bitIndex+n);
  *bitIndex      = *bitIndex + n;
  for (b = startByteIndex; b <= stopByteIndex; b++) {
    res = res + (wordTable[b]<<wordShift);
    if (b == startByteIndex) {
      res = res << leftBitShift;
      res = res >> leftBitShift;
    }
    wordShift = wordShift+intSize;
  }
  res = res >> bitShift;
  SiTRACE("0x%08x (%12d)\n", res, res);
  return res;
}
signed int   Si2164B_L2_Read_L1_Misc_Data        (Si2164B_L2_Context *front_end, SiLabs_T2_Misc_Signalling    *misc) {
  signed int issyi_status;
  misc->bw                 = front_end->demod->prop->dd_mode.bw;
  issyi_status             = Si2164B_get_reg (front_end, bbdf_data_issyi_status);
       if (issyi_status == 0) {misc->data_issy = 0; misc->data_issy_ts_long  = 0; }
  else if (issyi_status == 1) {misc->data_issy = 1; misc->data_issy_ts_long  = 1; }
  else if (issyi_status == 2) {misc->data_issy = 1; misc->data_issy_ts_long  = 0; }
  misc->data_npd           = Si2164B_get_reg (front_end, bbdf_data_npd_active);
  misc->data_ts_rate       = Si2164B_get_reg (front_end, ts_rate_in);
  misc->data_djb_alarm     = Si2164B_get_reg (front_end, djb_alarm_data);
  issyi_status             = Si2164B_get_reg (front_end, bbdf_comm_issyi_status);
       if (issyi_status == 0) {misc->comm_issy = 0; misc->comm_issy_ts_long  = 0; }
  else if (issyi_status == 1) {misc->comm_issy = 1; misc->comm_issy_ts_long  = 1; }
  else if (issyi_status == 2) {misc->comm_issy = 1; misc->comm_issy_ts_long  = 0; }
  misc->comm_npd           = Si2164B_get_reg (front_end, bbdf_comm_npd_active);
  misc->comm_ts_rate       = 0;
  misc->comm_djb_alarm     = Si2164B_get_reg (front_end, djb_alarm_comm);
  return 1;
}
signed int   Si2164B_L2_Read_L1_Pre_Data         (Si2164B_L2_Context *front_end, SiLabs_T2_L1_Pre_Signalling  *pre ) {
  pre->type               = Si2164B_get_reg (front_end, pre_type);
  pre->bwt_ext            = Si2164B_get_reg (front_end, pre_bwt_ext);
  pre->s1                 = Si2164B_get_reg (front_end, pre_s1);
  pre->s2                 = Si2164B_get_reg (front_end, pre_s2);
  pre->l1_repetition_flag = Si2164B_get_reg (front_end, pre_l1_repetition_flag);
  pre->guard_interval     = Si2164B_get_reg (front_end, pre_guard_interval);
  pre->papr               = Si2164B_get_reg (front_end, pre_papr);
  pre->l1_mod             = Si2164B_get_reg (front_end, pre_l1_mod);
  pre->l1_cod             = Si2164B_get_reg (front_end, pre_l1_cod);
  pre->l1_fec_type        = Si2164B_get_reg (front_end, pre_l1_fec_type);
  pre->l1_post_size       = Si2164B_get_reg (front_end, pre_l1_post_size);
  pre->l1_post_info_size  = Si2164B_get_reg (front_end, pre_l1_post_info_size);
  pre->pilot_pattern      = Si2164B_get_reg (front_end, pre_pilot_pattern);
  pre->tx_id_availability = Si2164B_get_reg (front_end, pre_tx_id_availability);
  pre->cell_id            = Si2164B_get_reg (front_end, pre_cell_id);
  pre->network_id         = Si2164B_get_reg (front_end, pre_network_id);
  pre->t2_system_id       = Si2164B_get_reg (front_end, pre_t2_system_id);
  pre->num_t2_frames      = Si2164B_get_reg (front_end, pre_num_t2_frames);
  pre->num_data_symbols   = Si2164B_get_reg (front_end, pre_num_data_symbols);
  pre->regen_flag         = Si2164B_get_reg (front_end, pre_regen_flag);
  pre->l1_post_extension  = Si2164B_get_reg (front_end, pre_l1_post_extension);
  pre->num_rf             = Si2164B_get_reg (front_end, pre_num_rf);
  pre->current_rf_idx     = Si2164B_get_reg (front_end, pre_current_rf_idx);
  pre->t2_version         = Si2164B_get_reg (front_end, pre_t2_version);
  return 1;
}
signed int   Si2164B_L2_Read_L1_Post_Data        (Si2164B_L2_Context *front_end, SiLabs_T2_L1_Post_Signalling *post) {
  signed int stored_l1_post_nb;
  signed int start_ibs_storing_data;
  signed int read_data;
  signed int max_l1_storing;
  signed int word_index;
  unsigned int i;
  unsigned int bitIndex;
  signed int storing_enable;
  unsigned int  intTable[50000];
  unsigned int *wordTable;

  wordTable  = &intTable[0];

  /* Read L1 Post table data */
  word_index = 0;
  Si2164B_READ(front_end->demod, storing_enable);
  if (storing_enable==0) {
    Si2164B_WRITE(front_end->demod, storing_enable, 1);
    system_wait(5000);
  }

  Si2164B_READ (front_end->demod, stored_l1_post_nb);
  Si2164B_READ (front_end->demod, start_ibs_storing_data);
  max_l1_storing = start_ibs_storing_data;
  Si2164B_WRITE(front_end->demod, base_read_address, 0);
  Si2164B_WRITE(front_end->demod, read_data_latch, 1);
  SiTRACE("Load L1 signalling (%s) from the storing memory (%d x 32 bits words).\n", Si2164B_standardName(front_end->demod->rsp->dd_status.modulation), stored_l1_post_nb);

  SiTRACE("reading until %d or %d\n", stored_l1_post_nb-1, max_l1_storing);

  while ( (word_index < stored_l1_post_nb-1) && (word_index < max_l1_storing) ) {
    Si2164B_READ (front_end->demod, read_data);
    Si2164B_WRITE(front_end->demod, read_data_latch, 1);
    SiTRACE("word %3d = 0x%08x\n", word_index, read_data);
    wordTable[word_index++] = read_data;
  }
  SiTRACE("\n");
  Si2164B_WRITE(front_end->demod, store_l1_post_en, 1);

  word_index = 0;
  while ( (word_index < stored_l1_post_nb-1) && (word_index < max_l1_storing) ) {
    SiTRACE("%08x ", wordTable[word_index]);
    word_index++;
  }
  SiTRACE("\n");

  /* Parse L1 Post table data */
  bitIndex = 0;

  post->configurable.num_rf               = Si2164B_get_reg (front_end, num_rf);
  post->configurable.s2                   = Si2164B_get_reg (front_end, pre_s2);
  post->configurable.sub_slices_per_frame = Si2164B_n_bits(wordTable, 15, &bitIndex);
  post->configurable.num_plp              = Si2164B_n_bits(wordTable,  8, &bitIndex);
  post->configurable.num_aux              = Si2164B_n_bits(wordTable,  4, &bitIndex);
  post->configurable.aux_config_rfu       = Si2164B_n_bits(wordTable,  8, &bitIndex);
  for (i=0; i < post->configurable.num_rf; i++) {
  post->configurable.rf[i].rf_idx         = Si2164B_n_bits(wordTable,  3, &bitIndex);
  post->configurable.rf[i].frequency      = Si2164B_n_bits(wordTable, 32, &bitIndex);
  }
  if ( (post->configurable.s2&0x0f) == 0x01) {
  post->configurable.fef_type             = Si2164B_n_bits(wordTable,  4, &bitIndex);
  post->configurable.fef_length           = Si2164B_n_bits(wordTable, 22, &bitIndex);
  post->configurable.fef_interval         = Si2164B_n_bits(wordTable,  8, &bitIndex);
 }
 for (i = 0; i < post->configurable.num_plp; i++) {
  post->configurable.plp[i].plp_id               = Si2164B_n_bits(wordTable,  8, &bitIndex);
  post->configurable.plp[i].plp_type             = Si2164B_n_bits(wordTable,  3, &bitIndex);
  post->configurable.plp[i].plp_payload_type     = Si2164B_n_bits(wordTable,  5, &bitIndex);
  post->configurable.plp[i].ff_flag              = Si2164B_n_bits(wordTable,  1, &bitIndex);
  post->configurable.plp[i].first_rf_idx         = Si2164B_n_bits(wordTable,  3, &bitIndex);
  post->configurable.plp[i].first_frame_idx      = Si2164B_n_bits(wordTable,  8, &bitIndex);
  post->configurable.plp[i].plp_group_id         = Si2164B_n_bits(wordTable,  8, &bitIndex);
  post->configurable.plp[i].plp_cod              = Si2164B_n_bits(wordTable,  3, &bitIndex);
  post->configurable.plp[i].plp_mod              = Si2164B_n_bits(wordTable,  3, &bitIndex);
  post->configurable.plp[i].plp_rotation         = Si2164B_n_bits(wordTable,  1, &bitIndex);
  post->configurable.plp[i].plp_fec_type         = Si2164B_n_bits(wordTable,  2, &bitIndex);
  post->configurable.plp[i].plp_num_blocks_max   = Si2164B_n_bits(wordTable, 10, &bitIndex);
  post->configurable.plp[i].frame_interval       = Si2164B_n_bits(wordTable,  8, &bitIndex);
  post->configurable.plp[i].time_il_length       = Si2164B_n_bits(wordTable,  8, &bitIndex);
  post->configurable.plp[i].time_il_type         = Si2164B_n_bits(wordTable,  1, &bitIndex);
  post->configurable.plp[i].in_band_a_flag       = Si2164B_n_bits(wordTable,  1, &bitIndex);
  post->configurable.plp[i].in_band_b_flag       = Si2164B_n_bits(wordTable,  1, &bitIndex);
  post->configurable.plp[i].reserved_1           = Si2164B_n_bits(wordTable, 11, &bitIndex);
  post->configurable.plp[i].plp_mode             = Si2164B_n_bits(wordTable,  2, &bitIndex);
  post->configurable.plp[i].static_flag          = Si2164B_n_bits(wordTable,  1, &bitIndex);
  post->configurable.plp[i].static_padding_flag  = Si2164B_n_bits(wordTable,  1, &bitIndex);
  }
  post->configurable.fef_length_msb              = Si2164B_n_bits(wordTable,  2, &bitIndex);
  post->configurable.reserved_2                  = Si2164B_n_bits(wordTable, 30, &bitIndex);
  for (i=0; i<post->configurable.num_aux; i++) {
  post->configurable.aux[i].aux_stream_type      = Si2164B_n_bits(wordTable,  4, &bitIndex);
  post->configurable.aux[i].aux_private_conf     = Si2164B_n_bits(wordTable, 28, &bitIndex);
  }
  post->dynamic.frame_idx            = Si2164B_n_bits(wordTable,  8, &bitIndex);
  post->dynamic.sub_slice_interval   = Si2164B_n_bits(wordTable, 22, &bitIndex);
  post->dynamic.type_2_start         = Si2164B_n_bits(wordTable, 22, &bitIndex);
  post->dynamic.l1_change_counter    = Si2164B_n_bits(wordTable,  8, &bitIndex);
  post->dynamic.start_rf_idx         = Si2164B_n_bits(wordTable,  3, &bitIndex);
  post->dynamic.reserved_1           = Si2164B_n_bits(wordTable,  8, &bitIndex);
  for (i=0; i< post->dynamic.num_plp; i++) {
  post->dynamic.plp[i].plp_id               = Si2164B_n_bits(wordTable,  8, &bitIndex);
  post->dynamic.plp[i].plp_start            = Si2164B_n_bits(wordTable, 22, &bitIndex);
  post->dynamic.plp[i].plp_num_blocks       = Si2164B_n_bits(wordTable, 10, &bitIndex);
  post->dynamic.plp[i].reserved_2           = Si2164B_n_bits(wordTable,  8, &bitIndex);
  }
  post->dynamic.reserved_3           = Si2164B_n_bits(wordTable,  8, &bitIndex);
  for (i=0; i <post->configurable.num_aux; i++) {
  post->dynamic.aux_private_dyn[i].msb  = Si2164B_n_bits(wordTable, 24, &bitIndex);
  post->dynamic.aux_private_dyn[i].lsb  = Si2164B_n_bits(wordTable, 24, &bitIndex);
  }
  return 1;
}
#endif /* _DVB_T2_SIGNALLING_H_ */
signed int   Si2164B_L2_Test_MPLP                (Si2164B_L2_Context *front_end, signed int freq) {
  signed int data_plp_count;
  signed int num_plp;
  signed int plp_index;
  signed int plp_id;
  signed int plp_type;
  signed int lock_wait_start_ms;
  front_end = front_end;
  freq = freq;
  data_plp_count = 0;
  lock_wait_start_ms = system_time();
  Si2164B_L2_lock_to_carrier (front_end, Si2164B_DD_MODE_PROP_MODULATION_DVBT2, freq, 8000000, 0, 0, 0
                             , 0
                             , -1
                             , 0
                             );
  if (front_end->demod->rsp->dd_status.dl == 0) {
    SiTRACE("Demod timeout after %4d ms\n", system_time() -  lock_wait_start_ms);
    return 0;
  }
  /* Locked; Now check DVBT2 status */
  if (Si2164B_L1_DVBT2_STATUS    (front_end->demod, Si2164B_DVBT2_STATUS_CMD_INTACK_OK) != NO_Si2164B_ERROR) {
    SiTRACE("Si2164B_L1_DVBT2_STATUS error\n");
    return 0;
  }
  SiTRACE("dvbt2_status.pcl     %d\n", front_end->demod->rsp->dvbt2_status.pcl);
  SiTRACE("dvbt2_status.num_plp %d\n", front_end->demod->rsp->dvbt2_status.num_plp);
  SiTRACE("dvbt2_status.plp_id  %d\n", front_end->demod->rsp->dvbt2_status.plp_id);
  num_plp = front_end->demod->rsp->dvbt2_status.num_plp;
  SiTRACE("There are %d PLPs in this stream\n", num_plp);
  data_plp_count = 0;
  for (plp_index=0; plp_index<num_plp; plp_index++) {
    Si2164B_L1_DVBT2_PLP_INFO (front_end->demod, plp_index);
    plp_id   = front_end->demod->rsp->dvbt2_plp_info.plp_id;
    plp_type = front_end->demod->rsp->dvbt2_plp_info.plp_type;
    SiTRACE("PLP index %3d: PLP ID %3d, PLP TYPE %d : ", plp_index, plp_id, plp_type);
    if (plp_type == Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_TYPE_COMMON) {
      SiTRACE("COMMON PLP at index %3d: PLP ID %3d, PLP TYPE %d\n", plp_index, plp_id, plp_type);
    } else {
      SiTRACE("DATA   PLP at index %3d: PLP ID %3d, PLP TYPE %d\n", plp_index, plp_id, plp_type);
      data_plp_count++;
    }
  }
  return data_plp_count;
}
unsigned char Si2164B_iq_demap = 0;
signed int   Si2164B_L2_Get_Constellation_IQ_rx_type (Si2164B_L2_Context   *front_end, unsigned char rx_type) {
  #define Si2164B_rx_type_CODE       2,225,14
  #define Si2164B_rx_type_l1_pre      0
  #define Si2164B_rx_type_l1_post     1
  #define Si2164B_rx_type_data_plp    2
  #define Si2164B_rx_type_common_plp  3
  #define Si2164B_rx_type_all         4
  #define Si2164B_rx_type_off         5

  if (rx_type  < 6) {
    Si2164B_iq_demap = 1;
  } else {
    Si2164B_iq_demap = 0;
  }
  return Si2164B_L1_DD_SET_REG (front_end->demod, Si2164B_rx_type_CODE, rx_type);
}
signed int   Si2164B_L2_Get_Constellation_IQ         (Si2164B_L2_Context   *front_end, signed int *i, signed int *q) {
  signed int           return_code;
  unsigned int  imask;
  unsigned int  qmask;
  unsigned int  ishift;
  unsigned int  iq_coef;
  unsigned int  limit;
  unsigned int  value;
  unsigned int  sign_bit;
  signed int  ival;
  signed int  qval;

  *i = *q = 0;

  #define Si2164B_symb_iq_CODE       19,44,7
  #define Si2164B_iq_symb_CODE       31,232,8
  #define Si2164B_iq_demod_sat_CODE  15,248,5
  #define Si2164B_rx_iq_CODE         31,220,14

  switch (front_end->demod->standard)
  {
    case Si2164B_DD_MODE_PROP_MODULATION_DVBC2:
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT :
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT2: {
      if (Si2164B_iq_demap != 0) {
        return_code = Si2164B_L1_DD_GET_REG (front_end->demod, Si2164B_rx_iq_CODE);
      } else {
        return_code = Si2164B_L1_DD_GET_REG (front_end->demod, Si2164B_symb_iq_CODE);
      }
      imask = 0x000ffc00 ; qmask = 0x000003ff ; ishift = 10 ; iq_coef = 4; limit = qmask+1;
      break;
    }
    case Si2164B_DD_MODE_PROP_MODULATION_DVBC : {
      return_code = Si2164B_L1_DD_GET_REG (front_end->demod, Si2164B_iq_symb_CODE);
      imask = 0xffff0000 ; qmask = 0x0000ffff ; ishift = 16 ; iq_coef = 2; limit = qmask+1;
      break;
    }
    case Si2164B_DD_MODE_PROP_MODULATION_DSS  :
    case Si2164B_DD_MODE_PROP_MODULATION_DVBS :
    case Si2164B_DD_MODE_PROP_MODULATION_DVBS2: {
      return_code = Si2164B_L1_DD_GET_REG (front_end->demod, Si2164B_iq_demod_sat_CODE);
      imask = 0x0000ff00 ; qmask = 0x000000ff ; ishift =  8 ; iq_coef = 4; limit = qmask+1;
      break;
    }
    default : {
      SiTRACE("IQ not possible with standard %d\n", front_end->demod->standard);
      SiERROR("IQ not possible with the current standard\n");
      return 0;
      break;
   }
  }

  if (return_code != NO_Si2164B_ERROR) return 0;

  value = (front_end->demod->rsp->dd_get_reg.data1<< 0)
        + (front_end->demod->rsp->dd_get_reg.data2<< 8)
        + (front_end->demod->rsp->dd_get_reg.data3<<16)
        + (front_end->demod->rsp->dd_get_reg.data4<<24);

  sign_bit = ishift -1;

  ival = (value & imask) >> ishift;
  if (ival >> sign_bit) { ival = ival - limit; }

  qval = (value & qmask);
  if (qval >> sign_bit) { qval = qval - limit; }

  *i = ival*iq_coef;
  *q = qval*iq_coef;

  return 1;
}
#endif /* SILABS_API_TEST_PIPE */

/**/



