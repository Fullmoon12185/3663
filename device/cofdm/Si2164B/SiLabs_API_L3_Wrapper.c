/*************************************************************************************************************/
/*                                  Silicon Laboratories                                                     */
/*                                  Broadcast Video Group                                                    */
/*                                  SiLabs API Functions                                                     */
/*-----------------------------------------------------------------------------------------------------------*/
/*   This source code contains all API functions for a SiLabs digital TV demodulator Evaluation board        */
/*-----------------------------------------------------------------------------------------------------------*/
/* Change log: */
/* Last  changes:

  As from V2.5.6:
    <improvement>[DVB_S2/status]   In SiLabs_API_Demod_status_selection: storing status->num_is and status->isi_id when locked in DVB_S2,
                                     setting status->s2x to 1 if locked in a DVB-S2X MODCODE combination
                                   In SiLabs_API_Text_status_selection: filling text with status->num_is and status->isi_id values when locked in DVB_S2

    <improvement>[SLEEP/ANALOG]    In SiLabs_API_lock_to_carrier: returning 1 after calling SiLabs_API_switch_to_standard in ANALOG or SLEEP modes.
    <improvement>[SOC_EVB/Si2165D] In SiLabs_API_Demod_status_selection for Si2165D: setting ber_mant/ber_exp based on status->ber, to allow proper BER display
     using SiLabs_API_Text_status_selection.
    <improvement>[GPIF/Cypress] In SiLabs_API_TS_Mode: not using the result of the check on '-gpif', since it doesn't return the proper value.
    <new_feature>[Cypress/streaming] In SiLabs_API_TS_Mode: allowing SILABS_TS_STREAMING option, to configure TS streaming independently from the demod settings
    <new_feature>[SAT/TAG] In Silabs_API_Test: adding 'sat_tag' option

  As from V2.5.5:
    <new_feature>[SILABS_SUPERSET] Adding tags to allow compilation for TER-only/SAT-only/TER+SAT based on the superset code.

  As from V2.5.3:
    <new_feature>[Cypress/TS_SLAVE] In SiLabs_API_TS_Mode: Adding code to support SILABS_TS_SLAVE_FIFO (parallel TS retrieved using Cypress chip)
    <improvement>{sw_options/LNBH29] In SiLabs_API_SAT_Possible_LNB_Chips: Adding text for LNBH29.
    <new_feature>[Cypress/process]  In Silabs_API_Test: Adding acess to L0_Cypress_Process
    <new_part>[LNB/A8302] Adding compatibility with Allegro A8302 SAT LNB controller
    <new_feature>[LNB/index] Adding SiLabs_API_SAT_LNB_Chip_Index, a function used to select the portion of
        an LNB controller is use. (set to 0 or 1 depending on the case). Compatible with LNBH26 and A8302.
    <new_feature>[SAT_TUNER/sub] Adding SiLabs_API_SAT_Tuner_Sub, a funcitonused to select the sub-portion of a dual SAT tuner

  As from V2.5.2:
    <correction>[ATV/TEXT status] In SiLabs_API_TER_Tuner_ATV_Text_status: enabling/disabling i2c to execute TER tuner status
    <improvement>[SAT/LNB] In SiLabs_API_SAT_voltage: calling the L1_xxxx_InitAfterReset function for LNBH25, LNBH26 and LNBH29.
       (other LNB chips don't need this, since all registers are written for each call)
    <new_feature>[TS_spurs]
      Adding SiLabs_API_Get_TS_Dividers to retrieve the TS clock dividers (only supported by Si2164B with recent FWs (above 5_0b13) )
      Adding Silabs_API_TS_Tone_Cancel  to activate the Tone cancellation in the TER tuner (only supported by Si2190B initially)
    <improvement>[Duals/SiLabs_EVBs] In SiLabs_API_SW_Init: surrounding ts_mux related code by USB_Capability, since it's only valid on some SiLabs EVBs

  As from V2.5.1:
    <correction>  [status/BER] In SiLabs_API_Demod_status_selection: Swapping dd_ber_resol exp and mant for Si2164/83 for exp=7 and mant=1 (in place of exp=1 mant=7)
    <improvement>[TERACOM/BER] In SiLabs_API_Demod_status_selection: Changing BER settings when locked in DVB-T (1;6) vs other standards (1;7).
              This is to improve measurement accuracy for BER criteria.
    <improvement> [status/return value] In SiLabs_API_Demod_status_selection: returning 1 when status function meets no problem.
    <new_feature>[TER_Tuner/Config]
      Adding SiLabs_API_TER_Tuner_FEF_Input to allow configuration of the TER tuner FEF input. This needs to be added to the configuration macros.
      The default value is '1' to select GPIO1 on the TER tuner side.
    <improvement>[cleanup] In SiLabs_API_TER_Tuner_ATV_Tune: removing invert_spectrum
   <improvement/compatibility>
	   In SiLabs_API_SAT_Possible_LNB_Chips: Setting i to avoid warning when not used.
     In SiLabs_API_Select_PLP: Setting plp_id and plp_mode to avoid warning when not used.
     In SiLabs_API_TER_Tuner_ATV_Tune: Setting all variables to avoid warnings when not used.
     In Silabs_API_Test: using standard to avoid warning when not used.
    <correction>  [constel/DVB-S2X] In Silabs_constelCode and Custom_constelCode: Adding DVB-S2-X specific constellations.

  As from 2.5.0:
   <improvement>[status/ISDBT] Adding partial_flag in _CUSTOM_Status_Struct
    In SiLabs_API_Demod_status_selection: storing partial flag information in status->partial_flag.
   <new_feature>[Test_Pipe/init_ok] In Silabs_API_Test: adding 'init_ok' to know if demod init is done
   <new feature>[TER_Tuner/Config]
     adding SiLabs_API_TER_Tuner_Block_VCO2 and SiLabs_API_TER_Tuner_Block_VCO3 to allow configuration of
     the TER_Tuner block_VCO2_code and block_VCO3_code
   <new_feature>[I2C/Tuners_Direct] In SiLabs_API_XXX_Tuner_I2C_Enable/SiLabs_API_XXX_Tuner_I2C_Enable: using a special value (100) to allow
    having direct connection to tuners (without demod pass-through).
    API CONFIG in such case:
     SiLabs_API_TER_tuner_I2C_connection(front_end, 100);
     SiLabs_API_SAT_tuner_I2C_connection(front_end, 100);
   <new_feature>[Test_Pipe/LNBH26] In Silabs_API_Test: adding 'lnbh26' 'a_b' '0/1' option to select which LNB controller is used (LNBH26 is a dual)
   <new_feature>[CONFIG/tracing] In all configuration functions: storing all configuration fields in SILABS_FE_Context (to enable configuration checking after init).
      NB: This allows removing some previous code used to avoid compilation warnings, since all fields are not used.
      Adding SiLabs_API_Config_Infos. This function is useful to check the configuration parameters based on the related function name.
        Use "Full" for the function name to get the entire configuration.

  As from V2.4.9:
   <improvement/Duals> Adding demod_die in _CUSTOM_Status_Struct
    In SiLabs_API_Demod_status_selection: storing demod die information in status->demod_die.
    In SiLabs_API_Text_status_selection:  adding die information to status text.
   <typo/T2_lock_mode> In SiLabs_API_lock_to_carrier comments: T2_lock_mode is independent of num_lp.
     Parameter: T2_lock_mode        the DVB-T2 lock mode        (0='ANY', 1='T2-Base', 2='T2-Lite')
     <typo/ISDB-T> In SiLabs_API_lock_to_carrier comments: bandwidth_Hz is also used for ISDB-T
     Parameter: bandwidth_Hz        the channel bandwidth in Hz (only for DVB-T, DVB-T2, ISDB-T)
   <correction/Duals> In SiLabs_API_Demods_Kickstart: directly setting demods to their final clock input.
    NB: For duals (Si216x2), the clock source should not change between TER and SAT, and the clock should be 'always-on':
      In calls to SiLabs_API_TER_Clock/SiLabs_API_SAT_Clock: use identical settings and force 'clock_control = 1'.

  As from V2.4.8:
   <new_part/A8293/LNB_Supply> Compatibility with Allegro's A8293 (needs A8293_COMPATIBLE)

  As from V2.4.7:
   <improvement/Settings> In SiLabs_API_Demod_status_selection: Saving status->IFagc depending on the AGC in use.
                           Previous versions assumed that AGC1 for SAT and AGC2 for TER. This restriction doesn't apply anymore.
   <improvement/compatibility> Changing all C++ comments to C style
                               In SiLabs_API_SAT_Tuner_status:      setting sat_tuner by default to avoid compiler warnings.
                               In SiLabs_API_TER_Tuner_ClockConfig: setting tuner_ter by default to avoid compiler warnings.
                               In SiLabs_API_TS_Mode:               moving SiTRACE after all variables are declared.
                               In SiLabs_API_SAT_AutoDetectCheck :  moving SiTRACE after all variables are declared.
   <improvement/NO_FLOAT> Adding SiLabs_API_SSI_SQI_no_float and Silabs_Log10_10000 to enable SSI and SQI computing when
                            not done by FW and when using floats is not allowed.
   <correction/BER> In SiLabs_API_Demod_status_selection:
                       storing ber_exp+1 in status->ber_exp.
                       storing per_exp+1 in status->per_exp.
                       storing fer_exp+1 in status->fer_exp.
                    In rate_f_mant_exp: exp treated to match the changes in SiLabs_API_Demod_status_selection.
   <correction/RSSI/Si2167B> In SiLabs_API_Demod_status_selection: Calling SAT_TUNER_RSSI_FROM_IFAGC for Si2167B in SAT

  As from V2.4.6:
   <improvement>[TERACOM/BER] In SiLabs_API_Demod_status_selection: Changing BER settings when locked in DVB-T (5;8) vs other standards (1;7). ONLY for Si2164B.
   <improvement>[DUALS/XTAL]  In SiLabs_API_Demods_Kickstart: Using TUNE_CAP_15P6 and CLK_MODE_XTAL to make sure demodulators are not pulling
    the clock input pin low.

  As from V2.4.4:
    <new_feature>[TER_Tuner/Config]
     Adding SiLabs_API_TER_Tuner_AGC_Input and SiLabs_API_TER_Tuner_IF_Output to allow configuration of the
       TER tuner IF output and AGC input
    <improvement>[STATUS/selection] In SiLabs_API_Demod_status_selection:
       Forcing flags to 1 if status_selection is 0x00, to allow compatibility for applications calling this
        function directly

  As from V2.4.3:
   <improvement> In SiLabs_API_TS_Mode: removing TS parallel shape setup for Si2164B.
                  Only keeping values of '7070' for GPIF mode.
                  GPIF mode is only used with SiLabs EVBs and is never used together with serial or parallel.

  As from V2.4.2
   <correction> [Si2164A/Si2169B/Si2168B/DVBT2_C/N] In SiLabs_API_Demod_status_selection:
   Correctly setting status->c_n to dvbt2_status.cnr/4.0 for the related parts.
   <correction>[TS_Crossbar] TS crossbar feature now working (with duals with this capability) as from FW 4_ab4

  As from V2.4.1
   <improvement>[Si2167B] In Silabs_coderateCode/Custom_coderateCode: tags changed to allow export for Si21652B
   <new_feature> [ISDB-T/LAYER] Adding SiLabs_API_TER_ISDBT_Monitoring_mode property to select the layer used
    for BER, CBER, PER, and UNCOR monitoring in ISDB-T
   <improvement> [TUNER_STATUS] In SiLabs_API_FE_status_selection: only tracing RSSI and freq if tuner status has been done
               Not returning anymore if the standard is unknown, to allow AGC monitoring.

  As from V2.4.0:
   <new_feature> [tag/level] adding tag and level support for Si2167B and derivatives
   <improvement> [Unicable/Multi-Treading/Multiple frontends] In SiLabs_API_SAT_Tuner_Tune: removing I2C pass-through enable/disable.
    This is only called while tuning in Unicable, and the i2c pass-through is already enabled when calling this function.
    The change removes nested i2c pass-through enable/disable calls.
    This is generally not an issue, unless several tuners are using the same i2c address and the application is multi-threading.

  As from V2.3.9:
   <new_parts> Status updated for ISDB-T support

  As from V2.3.8:
    <new_feature> Using STRING_APPEND_SAFE macro (defined in Si_I2C V3.4.5) for Linux compatibility.

  As from V2.3.7:
    <new_parts> [Si2164B] Adding Si2164B support
    <new_standard> [ISDB-T] Adding support for ISDB-T
    <new_feature> [DVB-S2X] Adding Constellation and code rate functions for DVB-S2X.
    <new_feature> [TAG/LEVEL] Adding definitions for TAG and level
                Adding SiLabs_API_Set_Index_and_Tag
    <new_feature> Using STRING_APPEND_SAFE macro (defined in Si_I2C V3.4.5) for Linux compatibility.

  As from V2.3.6:
    <correction> [LNBH29] In SiLabs_API_SAT_Select_LNB_Chip: using lnb_code 29 to select LNBH29
    <new_feature> [STATUS/SELECTION] Adding SiLabs_API_Demod_status_selection / SiLabs_API_FE_status_selection / SiLabs_API_Text_status_selection.
            The behavior when calling SiLabs_API_Demod_status / SiLabs_API_FE_status_ / SiLabs_API_Text_status is unchanged, since these call the new functions
              with the value '0x00' which means 'status all items'
            These can be used to status only a portion of the CUSTOM_Status_Struct, depending on a status_selection bit field, using the following bit flags:
              FE_LOCK_STATE     : demod_lock, fec_lock,  uncorrs, TS_bitrate_kHz, TS_clock_kHz
              FE_LEVELS         : RSSI, RFagc, IFagc
              FE_RATES          : BER, PER, FER (depending on standard)
              FE_SPECIFIC       : symbol_rate, stream, constellation, c/n, freq_offset, timing_offset, code_rate, t2_version,
                                  num_plp, plp_id, ds_id, cell_id, etc (generally one function called per standard).
              FE_QUALITY        : SSI, SQI
              FE_FREQ           : freq
    <new_feature> [SW_CONFIG] Adding SiLabs_API_Frontend_Chip / SiLabs_API_TER_tuner_I2C_connection / SiLabs_API_SAT_tuner_I2C_connection
            These will be used instead of direct access to the L3 context values.
            They also allow easier access from the top level, and allow configuring the GUI using script files.
    <improvement> [T2/C2/MPLP/SEEK] In SiLabs_API_Channel_Seek_Next: if locked, updating value of front_end->standard.
            This removes the need to call SiLabs_API_Demod_status to update this value, which is used when retrieving the plp_ids and ds_ids.
    <improvement> [portability] In SiLabs_API_SSI_SQI: moving code after all declarations, because this creates compilation errors with some compilers.
    <improvement> [portability/NO_FLOATS_ALLOWED] In status functions, store information as rate_mant/rate_exp for ber/per/fer,
              and use these instead of the double fields.
    <improvement> [renaming] SiLabs_API_TER_FEF_CONFIG renamed as SiLabs_API_TER_FEF_Config, for consistency with other configuration functions.
    <improvement> [traces] Adding dedicated trace messages to help trace wrapper function calls:
      'API CALL CONFIG' for SW configuration functions, formatted as in configurations macros.
            These will be useful to check the SW configuration in the traces, and create the corresponding configuration macros.
      'API CALL SEEK'   for scan-related functions
      'API CALL INIT'   for init-related functions
      'API CALL LOCK'   for lock-related functions
      'API CALL STATUS' for statusing functions
    <improvement> [DVB-T2] In SiLabs_API_Demod_status: statusing status->t2_system_id
    <improvement> [Si2164/ANALOG] In Silabs_standardCode: Adding 'ANALOG' value for Si2164.

*/
/* Older changes:

  As from V2.3.5:
    <new_feature> [Si2164/SPI) Adding SiLabs_API_SPI_Setup (Only available with Si2164 derivatives as from today)
       CAUTION1: In any case, this requires updating the following item to support SPI download:
        - SiLabs_L0 source code. The SPI support functions also need to be ported to your platform(s)
       CAUTION2: When used with SiLabs EVBs, this requires updating the following items to support SPI download using the Cypress chip:
        - Cypress FW
        - Cypress DLL
      Adding SiLabs_API_SPI_Setup
    <improvement> [code_checkers] In text-oriented functions using sprintf:
      Replacing sprintf by snprintf with a max size at 1000, as this is safer.
      The only constraint is that the text strings need to be declared with a minimum size of 1000 bytes.
      This should be enough to pass through code checkers.
    <comments> In SiLabs_API_TER_Tuner_ClockConfig: documenting input parameters use

  As from V2.3.4:
    <new_feature> [TER TUNER/Multi-frontend] Adding SiLabs_API_TER_Tuner_ClockConfig, to easily configure the TER tuner clock:
    signed int   SiLabs_API_TER_Tuner_ClockConfig    (SILABS_FE_Context *front_end, signed int xtal, signed int xout);
     xtal = 1: a Xtal is connected to and driven by the TER tuner.
     xtal = 0: a clock signal is connected to the TER tuner, which doesn't drive a Xtal.
     xout = 1: the clock is going out of the TER tuner.
     xout = 0: no  clock is going out of the TER tuner.

  As from V2.3.3:
    <new feature> [handshake] Adding SiLabs_API_Handshake_Setup, to easily control the handshake parameters from the wrapper level
    <new feature> [Si2164] Adding SiLabs_API_TER_T2_lock_mode, to select the T2 lock mode. It can be used to select the T2 lock mode during channel Seek.
                   This avoids the need to add a parameter to Seek_Init

  As from V2.3.2:
    <new feature> [Si21x8 tuners] Adding SiLabs_API_TER_Broadcast_I2C, useful to enable the broadcast i2c feature (only available with Si21x8B tuners)
    <improvement> [AUTO_T_T2] In SiLabs_API_Demod_status: setting front_end->standard to match status->standard when locked.
                   This is useful for SiLabs_API_Get_PLP_ID_and_TYPE when in AUTO_T_T2 and locked on a T2 signal:
                    if front_end->standard is left as 'SILABS_DVB_T' the function returns 0 while it needs to call Si216x_L1_DVBT2_PLP_INFO
    <new feature> [MCNS] In SiLabs_API_Demod_status: adding MCNS support

  As from  V2.3.1:
    <correction>  In SiLabs_API_TS_Mode:
     Correcting the 'SILABS_TS_TRISTATE' case to use the 'TRISTATE' mode
     Adding  SILABS_TS_OFF in CUSTOM_TS_Mode_Enum structure
     Adding 'SILABS_TS_OFF' case for Si2164/Si2167B/Si2169A
    <correction> In SiLabs_API_DEMOD_Status:
      Adding the SILABS_MCNS case
    <new feature> Adding 'signed int clock_control' to SiLabs_API_TER_Clock and SiLabs_API_SAT_Clock prototypes.
      This is used for multi-frontends applications when a tuner's clock is forwarded to another frontend.
        In this case it needs to be 'ALWAYS_ON'.
        To keep the previous behavior, use '2' (i.e. the 'MANAGED' mode)
      Adding the corresponding code in:
       SiLabs_API_TER_Clock_Options/SiLabs_API_TER_Clock
       SiLabs_API_SAT_Clock_Options/SiLabs_API_SAT_Clock
    <new feature> In SiLabs_API_SAT_Clock_Options: adding Si2167B
    <new_feature> Adding t2_version monitoring and related functions

  As from V2.3.0:
    Reverting changes to constellation type in SiLabs_API_lock_to_carrier function, as this forbids using the value of '-1' as SILABS_QAMAUTO.
    Using 'unsigned char constellation' broke the DVB-C AUTO qam capability.

    SiLabs_API_lock_to_carrier prototype is now:
    signed int   SiLabs_API_lock_to_carrier (SILABS_FE_Context *front_end,
                                       unsigned char standard,
                                                 signed int freq,
                                                 signed int bandwidth_Hz,
                                       unsigned char stream,
                                       unsigned int  symbol_rate_bps,
                                                char constellation,
                                       unsigned char polarization,
                                       unsigned char band,
                                                 signed int data_slice_id,
                                                 signed int plp_id,
                                       unsigned char T2_lock_mode);

  As from V2.2.9:
    In Custom_giCode / Silabs_giCode / Silabs_GI_Text:
     Adding 1/64 GI code handling (for DVB-C2)
    Adding lnb_chip_address to SILABS_FE_Context
    Adding SiLabs_API_TER_FEF_Options and SiLabs_API_TER_FEF_Config functions, to allow different FEF configuration depending on the frontend.
     This is required when using dual demodulators, where there are restrictions on MP_x and GPIOx pin usage.
    Changing SiLabs_API_SAT_Select_LNB_Chip function definition to add the lnb chip address.
     This is required for multi-frontend SAT applications.
    Changing SiLabs_API_switch_to_standard and SiLabs_API_set_standard function definitions to use 'unsigned char' instead of 'signed int' for standard.
    Changing SiLabs_API_lock_to_carrier function definition to use 'unsigned char' instead of 'signed int' for standard/stream/constellation/polarization/band/T2_lock_Mode.
     This avoids casting the related values to (unsigned int) within the functions.
      data_slice_id and plp_id are kept as 'signed int', as they may take a value of '-1' at wrapper level to select the corresponding 'auto' modes.
    In SiLabs_API_Demod_status:
     Adding one SiTRACE right after DD_STATUS, to trace the demod address (useful in multi-front-ends), the lock state and the standard.
    In SiLabs_API_Demod_status/SiLabs_API_SAT_Tuner_status/SiLabs_API_SAT_Tuner_Tune/SiLabs_API_TER_Tuner_Init/SiLabs_API_TER_Tuner_Text_status/SiLabs_API_TER_Tuner_ATV_Tune/SiLabs_API_TER_Tuner_Block_VCO:
     Changing I2C Enable/Disable calls to use the TER and SAT indirect i2c enable/disable calls, to allow tuner rssi statusing if INDIRECT_I2C_CONNECTION is used
     In SiLabs_API_SAT_Tuner_I2C_Enable and SiLabs_API_TER_Tuner_I2C_Enable:
      Replacing 'count' by 'fe_count', as 'count' may be a reserved word in some implementations.
     In SiLabs_API_SAT_Tuner_I2C_Enable and SiLabs_API_SAT_Tuner_I2C_Disable:
      Correcting code to properly connect the required i2c pass-through (previously only working for the SAT tuner on frontend 0 only).
    In SiLabs_API_FE_status:
     Directly tracing freq and tuner rssi before calling SiLabs_API_Demod_status.
    In SiLabs_API_Text_status:
     Adding config_code to text status. This is useful to knwo which frontend is statused in multi-frontend applications
    In SiLabs_API_SSI_SQI:
     Correcting SiTRACEs to display entire messages (last parameter wasn't displayed).
    In SiLabs_API_Select_PLP:
     Adding DVB-C2

  As from V2.2.8:
    Adding t2_base_lite in CUSTOM_Status_Struct
    Adding Silabs_T2_Base_Lite_Text function.
    In SiLabs_API_Text_status:
      Adding T2 base/lite text for T2
      Added MCNS in frequency display. MCNS text status didn't fill entirely due to this.
      Reduced code for frequency diplay.
    In SiLabs_API_TER_Clock / SiLabs_API_TER_AGC : adding tags to remove code for non-TER parts
    In SiLabs_API_SAT_Clock / SiLabs_API_SAT_AGC / SiLabs_API_SAT_Spectrum : adding tags to remove code for non-SAT parts

  As from V2.2.7:
    In SiLabs_API_Get_PLP_ID_and_TYPE: comparing standards value to SILABS_DVB_T2.
     (previously using Si2164_DD_MODE_PROP_MODULATION_DVBT2, which is incorrect at wrapper level).
    In SiLabs_API_TER_Clock: correction of Si2165 text related to clock source pin numbers
    In SiLabs_API_TER_AGC: correction of code used for Si2165

  As from V2.2.6:
    In SILABS_FE_Context structure: Adding config_code, used to store the i2c addresses of the TER tuner (bits[23:16]), the SAT tuner (bits[15:8]) the demod (bits[7:0]).
      This is used to know which path is controlled in multi-frontend applications, even when not tracing L0 bytes.
    In SiLabs_API_Channel_Seek_Next:   Adding T2_base_lite flag
    In SiLabs_API_Channel_Seek_Next:   Adding T2_base_lite flag  (indicates whether the locked signal is T2-Base or T2-Lite)
    In SiLabs_API_SAT_Select_LNB_Chip: Returning front_end->lnb_chip if OK, 0 otherwise. This compiles correctly for non-SAT products.
    In SiLabs_API_lock_to_carrier:     Adding T2_lock_mode flag (selects whether to lock on the T2-Base or T2-Lite signal (o='any'))
    In SiLabs_API_Tune:                Compatibility with Si2169B
    In SiLabs_API_Get_PLP_ID_and_TYPE: Adding C2 compatibility (for Si2164)
    Adding SiLabs_API_Get_DS_ID_Num_PLP_Freq function, for DVB-C2 Dataslice handling
    Adding SiLabs_API_Auto_Detect_Demods, for demodulators auto-detection.
    In SiLabs_API_TER_Tuner_Text_status:     Compatibility with SiLabs_TER_Tuner wrapper
    In SiLabs_API_TER_Tuner_ATV_Text_status: Compatibility with SiLabs_TER_Tuner wrapper
    In SiLabs_API_TER_Tuner_DTV_Text_status: Compatibility with SiLabs_TER_Tuner wrapper
    In SiLabs_API_TER_Tuner_ATV_Tune:        Compatibility with SiLabs_TER_Tuner wrapper
    In SiLabs_API_TER_Tuner_Block_VCO:       Compatibility with SiLabs_TER_Tuner wrapper
    WARNING: The latest TER tuners are NOT supported if not using the SiLabs_TER_Tuner wrapper
    In SiLabs_API_SSI_SQI: Adding C2 SSI SQI

  As from V2.2.5:
    In SiLabs_API_SSI_SQI: added DVB-C capability
    In SiLabs_API_Demod_status: calling SiLabs_API_SSI_SQI whenever SSI/SQI values haven't been set earlier.
      (the latest demodulators will have the SSI/SQI feature implemented in FW, and SiLabs_API_Demod_status will use the FW function if this is the case)
      Updating value of status->TS_clock_kHz for Si2165

  As from V2.2.4:
    In SiLabs_API_SAT_Tuner_status: compatibilty with SILABS_SAT_TUNER_API

  As from V2.2.3:
    Compatibility with several LNBH controllers in the same application.
     Adding SiLabs_API_SAT_Possible_LNB_Chips and SiLabs_API_SAT_Select_LNB_Chip to allow easy selection of the LNB controller
    Si2167B compatibility with INDIRECT_I2C_CONNECTION
    Si2167B compatibility with TER and SAT configuration

  As from V2.2.2:
   SILABS_SAT_TUNER_API compatibility (the only way to work with Si2164):
    Adding SiLabs_API_Select_SAT_Tuner function, useful to select the SAT tuner for each demodulator
    Adding SiLabs_API_SAT_Address function, useful to set the I2C address of any TER tuner
    Adding SiLabs_API_SAT_Clock and SiLabs_API_SAT_AGC functions, to configure the clock paths (source, input, freq) and AGC.
     NB: This only works if matching functions are added to the demodulator code.
     NB: In this first version, these functions only support SI2164, to keep the 'legacy' device codes untouched

  As from V2.2.1:
   Adding INDIRECT_I2C_CONNECTION control, allowing tuner i2c connection via any demodulator.
    This is used for applications with multiple demodulators
    Adding SiLabs_API_SAT_Tuner_I2C_Enable, SiLabs_API_SAT_Tuner_I2C_Disable,
           SiLabs_API_TER_Tuner_I2C_Enable, SiLabs_API_TER_Tuner_I2C_Disable
      These functions are used for INDIRECT_I2C_CONNECTION control
   SILABS_TER_TUNER_API compatibility (the only way to work with Si2164):
    Adding SiLabs_API_Select_TER_Tuner function, useful to select the TER tuner for each demodulator
    Adding SiLabs_API_TER_Address function, useful to set the I2C address of any TER tuner
    Adding SiLabs_API_TER_Clock and SiLabs_API_SAT_Clock functions, to configure the clock paths (source, input, freq).
     NB: This only works if matching functions are added to the demodulator code.
     NB: In this first version, these functions only support SI2164, to keep the 'legacy' device codes untouched
   In SiLabs_API_TER_Tuner_ATV_Text_status: not implemented for SILABS_TER_TUNER_API (not sure it was ever used)
   In SiLabs_API_Demod_status: tracing front_end->chip code also in hexadecimal.

  As from V2.2.0:
   In SiLabs_API_SAT_Unicable_Install:
    For Si2167: Using ds_sequence_mode 'manual' to select 'no_gap' sequences in Unicable mode
   In SiLabs_API_SAT_Unicable_Uninstall:
    For Si2167: Using ds_sequence_mode 'auto' to select 'gap' sequences in Normal mode

  As from V2.1.9:
   In SiLabs_API_SAT_voltage, for LNBH25 (as this part requires an init):
    if (front_end->lnb_chip_init_done == 0) { front_end->lnb_chip_init_done = L1_LNBH25_InitAfterReset(front_end->lnbh25); }
   Adding SiLabs_API_SAT_Unicable_Uninstall, to allow easily switching between NORMAL and UNICABLE modes
   In SiLabs_API_SAT_Unicable_Install:
    Using new DD_DISEQC_PARAM property to select 'no_gap' sequences in Unicable mode
   In SiLabs_API_SAT_Unicable_Uninstall:
    Using new DD_DISEQC_PARAM property to select 'gap' sequences in Normal mode

  As from V2.1.8:
   In SiLabs_API_SAT_voltage, for LNBH25 (as this part requires an init):
    if (front_end->lnb_chip_init_done == 0) { front_end->lnb_chip_init_done = L1_LNBH25_InitAfterReset(front_end->lnbh25); }
   Adding SiLabs_API_SAT_Unicable_Uninstall, to allow easily switching between NORMAL and UNICABLE modes

  As from V2.1.7:
   Adding SiLabs_API_SAT_voltage and SiLabs_API_SAT_tone, to allow managing the voltage separately from the tone.
     This is mostly interesting for Unicable, where the tone is not used to select the band.
     It's used in the Unicable code as from 2013/03/14 (SVN3657) to save time when sending a Unicable message over the DiSEqC bus
   In SiLabs_API_SAT_prepare_diseqc_sequence:
     Adding Si2164/Si2167B/Si2169
   In SiLabs_API_SAT_trigger_diseqc_sequence:
     Adding Si2164/Si2167B/Si2169

  As from V2.1.6:
   Adding SiLabs_API_SAT_prepare_diseqc_sequence and SiLabs_API_SAT_trigger_diseqc_sequence, to allow preparing the DiSEqC
     message and sending it in two steps. This is required for Unicable with some demodulator (such as Si2167A), as
     otherwise the preparation takes too much time to stay within the Unicable Td specification (4 to 22 ms)
   In SiLabs_API_Demod_status:
    Added comments to differentiate the various status blocks
   In SiLabs_API_SW_Init:
    Adding initialization of two new functions for Unicable:
    SiLabs_API_SAT_prepare_diseqc_sequence
    SiLabs_API_SAT_trigger_diseqc_sequence
   In SiLabs_API_SAT_voltage_and_tone:
    tracing lnb_chip value

  As from V2.1.5:
   In Custom_constelCode and Silabs_Constel_Text: adding QAM1024 and QAM4096 (for DVB C2)
   In SiLabs_API_Demod_status:
    More DVB-C2 statuses

  As from V2.1.4:
   In Silabs_UserInput_demod:
    Compatibility with Si2185
   In SiLabs_API_Demod_status:
    Si2164: first statuses for DVB-C2 added
    Setting BER, PER for Si2185

  As from V2.1.3:
   Compatibility with Si2191
   In Silabs_UserInput_demod:
    Compatibility with Si2185
   In Silabs_API_Test:
   Init of num_data_slice, to avoid compilation warning when not used

  As from V2.1.2:
   Compatibility with Si2164 (first C2 chip):
    data_slice_id added as a parameter for 'lock_to_carrier'
    *num_data_slice' addes as a parameter for 'Seek_Next'
   Adding SiLabs_API_SSI_SQI function (for S/S2 reception only)
   In SiLabs_API_Demod_status:
    Correcting status->uncorrs for all API controlled demodulators: '(uncor_msb<<8) + uncor_lsb' instead of '(uncor_msb<<16) + uncor_lsb'
   In SiLabs_API_SAT_Unicable_Tune:
    Removing one printf
   In SiLabs_API_lock_to_carrier:
    Correcting voltage levels for SAT polarization selection:
     13V is for 'Vertical', 18V is for 'Horizontal'
    Removing copy of front_end polarization and band values to Unicable structure
    (these may use different values)
   In SiLabs_API_Channel_Seek_Next:
    Removing Unicable polarization and band setup (the reference valeus are those in the front_end structure)
   In SiLabs_API_SAT_voltage_and_tone:
     Correcting voltage levels for SAT polarization selection:
     13V is for 'Vertical', 18V is for 'Horizontal'
   In Silabs_API_Test:
    Initializing all variables to avoid warnings when not used
    Adding access to test pipe for Si2165D
    Adding sat_scan_unicable option

  As from V2.1.1:
  In SiLabs_API_Demod_status:
   Setting status->cell_id by default at 0.
   Updating status->cell_id for Si2165, Si2167 and Si2167B
  In SiLabs_API_lock_to_carrier:
   Tracing input parameters with the corresponding names

  As from V2.1.0:
  In SiLabs_API_Demod_status:
   Calling SAT_TUNER_RSSI_FROM_IFAGC if it exists
  In SiLabs_API_SAT_Tuner_status:
   Removed duplicate call to SiLabs_API_Tuner_I2C_Enable

  As from V2.0.9:
  In SiLabs_API_Demod_status:
   For Si2165D:
    status->spectral_inversion = Si2165_L1_DVB_T_get_spectral_inversion (front_end->Si2165_FE->demod);
   For Si2167:
    status->spectral_inversion = Si2167_L1_DVB_T_get_spectral_inversion (front_end->Si2167_FE->demod);
   For Si2169:
    removing duplicate status->num_plp = ... line
  In SiLabs_API_Text_status:
    Comparing 'float' ratios with signed int values using (signed int) cast
  In SiLabs_API_Channel_Seek_Next:
    *num_plp     = 0; (the previous code, without the '*', 'erased' the pointer...)
  In Silabs_API_Test:
   Adding easy access to VDAPPS functions (for internal use)

  As from V2.0.8:
   Adding SiLabs_API_Get_DVBT_Hierarchy function, to retrieve the hiearchy information from the wrapper
   In SiLabs_API_TER_Tuner_status & SiLabs_API_SAT_Tuner_status:
    Moving lines for compatibility with VisualStudio (all variables need to be declared before any one is used).
    Without this it can be quite complex to debug, as the compiler message is not really explicit.
   In SiLabs_API_bytes_trace:
    Corrected when trackWrite flag was sent twice, instead of setting trackWrite & trackRead.
   In SiLabs_API_SAT_voltage_and_tone:
    Tracing voltage and tone values

  As from V2.0.7:
   Compatibility with LNBH29
   In SiLabs_API_Channel_Seek_Next:
    Compatibility with 'handshake' feature
   In SiLabs_API_SW_Init:
     Using a compilation flag to set the LNBH controller chip address if not defined at project level.
     It is written to allow the LNBH_I2C_ADDRESS flag to be defined at project level.
     If not defined at project level, it defaults to '#define LNBH_I2C_ADDRESS 0x10'
   In SiLabs_API_SAT_voltage_and_tone:
     Displaying 'in Unicable Mode' trace only when in Unicable mode.
   In SiLabs_API_Channel_Seek_Init: improved function comments

  As from V2.0.6:
   In SiLabs_API_Demod_status:
    For Si2165D:
     status->IFagc = Si2165_L1_Demod_get_IFagc        (front_end->Si2165_FE->demod);
     (previously returning aci_agc_cmd)
    For Si2169:
     Calling Si2169_L1_DVBT2_TX_ID
     status->cell_id            = front_end->Si2169_FE->demod->rsp->dvbt2_tx_id.cell_id;

  As from V2.0.5:
   LNBH init correction:
   In SiLabs_API_SW_Init, front_end->lnb_chip_init_done = 0; to force the flag at '0'.
    (Some compilers may set it randomly, so it needs to be forced to '0' for compatibility reasons.)

  As from V2.0.4:
   ADDED FEATURE: Added SiLabs_API_Channel_Lock_Abort function, to allow aborting a call to SiLabs_API_lock_to_carrier.
   In SiLabs_API_Demod_status:
    Added Si2167 compatibility with TER tuners not from SiLabs

  As from V2.0.3:
   In SiLabs_API_TS_Mode:
    For Si2167B and Si2169: settings ts parallel clock and data shape to 7 for GPIF mode, and back to 2 for parallel mode

  As from V2.0.2:
   In SiLabs_API_Select_PLP:
    For Si2169: if (plp_mode == Si2169_DVBT2_PLP_SELECT_CMD_PLP_ID_SEL_MODE_AUTO) { Si2169_L1_DD_RESTART(front_end->Si2169_FE->demod); system_wait(300); }

  As from V2.0.1:
   Added lnb_chip_init_done in SILABS_FE_Context
   In SiLabs_API_SAT_voltage_and_tone:
    if (front_end->lnb_chip_init_done == 0) { front_end->lnb_chip_init_done = L1_LNBH25_InitAfterReset(front_end->lnbh25); }
    This is because the LNBH25 requires an init of all registers

  As from V2.0.0:
   In SiLabs_API_Text_status:
    sprintf(formatted_status, "%s TS bitrate       %d kbps\n", status->TS_bitrate_kHz);
    sprintf(formatted_status, "%s TS clock         %d kHz\n" , status->TS_clock_kHz  );
   SiLabs_API_Reset_Uncorrs compatibility with Si2165
   SiLabs_API_Demod_reset   compatibility with Si2165 and Si2167

  As from V1.9.9:
    Adding NO_SAT tags to allow using Si2169 code without SAT features

  As from V1.9.8:
    Compatibility with TER_TUNER_Si2190
    Compatibility with TER_TUNER_CUSTOMTER
    In SiLabs_API_SAT_AutoDetectCheck:
     Adapting Si2169 code to return the current SAT standard when locked, 0 otherwise.
    In Silabs_API_Test:
    adding wrapper/sat_auto_detect option
    Compatibility with SAT_TUNER_RDA5816S

  As from V1.9.7:
    Adding LNB control in the API, to allow driving LNBH25 or LNBH21 easily
    In SiLabs_API_Demod_status: If Si2169, setting plp_id based on rsp.dvbt2_status.plp_id

  As from V1.9.6:
    Passing pointer to LNB function when calling SiLabs_Unicable_API_Init, following the new definition of SiLabs_Unicable_API_Init

  As from V1.9.5:
    Adding SILABS_MCNS, SILABS_DVB-C2 and SILABS_SLEEP possibilities
    Adding MCNS statusing
    In SiLabs_API_Demod_status: no demod status in SLEEP mode

  As from V1.9.4:
    In SiLabs_API_SAT_Get_AGC:
      Corrected value returned for Si2169 SAT AGC
    In Silabs_API_Test:
      Added Unicable test pipe access

  As from V1.9.3:
    adding TS_bitrate_kHz and TS_clock_kHz in demod status

  As from V1.9.2:
    Si2146 ATV and DTV STATUS removed (not in the Si2146 API anymore)

  As from V1.9.1:
    added/moved tags to allow Si2166B export
   In SiLabs_API_Demod_status:
    setting SSi and SQI at 0 by default.
    compatibility with rssi from CUSTOMTER and CUSTOMSAT tuners

  As from V1.9.0:
   In SiLabs_API_SAT_Tuner_status: moving tags to allow export for Si2168
   Wrapper code compatible with Si2167B: checked to be able to lock a Si2169 board when using the Si2167B code with the proper FW.

  As from V1.8.9:
   Tracing Wrapper source code info during init and in SiLabs_API_Infos
   SiLabs_API_SatAutoDetectCheck renamed as SiLabs_API_SAT_AutoDetectCheck for consistency
   Adding Test Pipe feature (only if SILABS_API_TEST_PIPE is defined at project level), using new Silabs_API_Test function
   Adding PLP management (for DVB_T2 only).
   In SiLabs_API_Demod_status:
    updating spectral_inversion for Si2169 in DVB-T and DBVB-T2
   In SiLabs_API_TS_Mode:
    Stopping GPIF clock if using the Cypress USB interface and not using GPIF mode
   In SiLabs_API_Demod_status and SiLabs_API_Text_status:
    Not storing current standard as front_end->standard, to avoid creating problems with standard switching.
    Using status->standard in all switches.
   In SiLabs_API_TER_Tuner_status and SiLabs_API_SAT_Tuner_status:
    Enabling i2c passthru before statusing tuners

  As from V1.8.8:
   In SiLabs_API_Demod_status:
    setting more statuses by default to indicate a no-lock:
      status->c_n                = 0;
      status->freq_offset        = 0;
      status->timing_offset      = 0;
      status->code_rate          = -1;
      status->SSI                = 0;
      status->SQI                = 0;
    For Si2169: returning '0' immediately in case a standard-specific status returns with an error.

  As from V1.8.7:
   Compatibility with Si2167B (coming soon)

  As from V1.8.6:
   Compatibility with Si2148/Si2158

  As from V1.8.5:
   Adding auto-detect functions:
   signed int   SiLabs_API_SAT_AutoDetect           (SILABS_FE_Context *front_end, signed int  on_off);
   signed int   SiLabs_API_TER_AutoDetect           (SILABS_FE_Context *front_end, signed int  on_off);

  As from V1.8.4:
   In SiLabs_API_SAT_voltage_and_tone: disegBuffer value correction
   Adding UNICABLE functions (compiled if #define UNICABLE_COMPATIBLE).

  As from V1.8.3:
   Compatibility with Si2178

  As from V1.8.2:
   In SiLabs_API_Demod_status:
    setting ber, fer and per by default at '-1' to indicate unavailability if not set later on.
   In SiLabs_API_Text_status:
    ber and per displayed as '--------' when not available

  As from V1.8.1:
   In SiLabs_API_Demod_status:
    (Si2169) Comments correction indicating that the rate checks are done on the exponent
    Comments correction indicating that the rate checks are done on the exponent
   In SiLabs_API_Text_status:
    spectral inversion added to text status

  As from V1.8.0:
   Compatibility with DTT759x (Terrestrial can tuner)
   Checking exponent for rate in SiLabs_API_Demod_status, to return -1 if not available

  As from V1.7.9:
   voltage_and_tone working with Si2167

  As from V1.7.8:
   voltage_and_tone working with LNBH21
   SAT and DVB-C blindscan working for Si2169
   compatibility with NO_TER 'dummy' TER tuner (for lab use)

  As from V1.7.7:
   BER monitored for Si2169 in DVB-T2 and DVB-S2 as well as for all DTV standards (previously not in FW so it was skipped)

  As from V1.7.6:
   2 lines added to allow exporting for demods with no 'STANDBY' or 'CLOCK_ON' feature

  As from 1.7.3:
   some lines moved for greater compatibility with Visual Studio

  As from 1.7.2:
   Si2169 agc values retrieved in SiLabs_API_Demod_status

  As from 1.7.0:
   adding WrapperI2C context to allow easy i2c read/write
   added SiLabs_API_ReadString/SiLabs_API_WriteString functions
   For SAT: added voltage/tone and DiSEqC functions

  As from 1.6.9:
   For Si2169: status->stream based on demod->prop->dvbt_hierarchy.stream;

  As from 1.6.7:
   Compatibility with NXP20142 SAT tuner
  API change: using Si2169 DD_SSI_SQI instead of Si2169_DVBT_SSI_SQI (also available in DVB-T2)

  As from 1.6.6:
   Adding missing BER status for Si2169

  As from 1.6.5:
   as it makes it easier to handle C-only or T-only exports

  As from 1.6.1:
   Compatibility with TER tuner cans (not using API mode)
   SiLabs_API_TER_Tuner_ATV_Tune compatible with Si2165

  As from 1.6.0:
   Added Si2185 support
   In   SiLabs_API_Demod_status:
     Set to 0 all info used to relock (bandwidth_Hz, symbol_rate, stream, constellation)
   In SiLabs_API_switch_to_standard:
     For Si2169: Checking dd_status.modulation if switch_to_standard fails
   In SiLabs_API_lock_to_carrier:
     Returning 0 if switch_to_standard fails

  As from 1.5.1:
        power_of_n corrected to return the proper value

 *************************************************************************************************************/
/* TAG V2.5.6 */

/* Before including the headers, define SiLevel and SiTAG */
//#define   SiLEVEL          3
//#define   SiTAG            front_end->tag
#include "SiLabs_API_L3_Wrapper.h"


#ifdef    __cplusplus
extern "C" {
#endif /* __cplusplus */

signed int   Silabs_init_done = 0;
signed int   Silabs_multiple_front_end_init_done = 0;
/* Also init a simple i2c context to allow i2c communication checking */
L0_Context* WrapperI2C;
L0_Context  WrapperI2C_context;

//csm,13'
unsigned char  messageBuffer[10000];
SiLabs_Carriers_Struct *Carriers_Table = NULL;
//end,13'

/* Translation functions from 'Custom' values to 'SiLabs' values */
#if 1
signed int   power_of_n (signed int n, signed int m) {
  signed int i;
  signed int p;
  p = 1;
  for (i=1; i<= m; i++) {
    p = p*n;
  }
  return p;
}
/************************************************************************************************************************
  Silabs_standardCode function
  Use:        standard code function
              Used to retrieve the standard value used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  standard, the value used by the top-level application (configurable in CUSTOM_Standard_Enum)
************************************************************************************************************************/
signed int   Silabs_standardCode                 (SILABS_FE_Context* front_end, CUSTOM_Standard_Enum          standard)
{
  front_end = front_end; /* to avoid compiler warning if not used */
  switch (standard) {
    case SILABS_DVB_T : return Si2164B_DD_MODE_PROP_MODULATION_DVBT;
    case SILABS_DVB_T2: return Si2164B_DD_MODE_PROP_MODULATION_DVBT2;
    case SILABS_MCNS  : return Si2164B_DD_MODE_PROP_MODULATION_MCNS;
    case SILABS_DVB_C : return Si2164B_DD_MODE_PROP_MODULATION_DVBC;
    case SILABS_DVB_C2: return Si2164B_DD_MODE_PROP_MODULATION_DVBC2;//csm
    case SILABS_DVB_S : return Si2164B_DD_MODE_PROP_MODULATION_DVBS;
    case SILABS_DVB_S2: return Si2164B_DD_MODE_PROP_MODULATION_DVBS2;
    case SILABS_DSS   : return Si2164B_DD_MODE_PROP_MODULATION_DSS;
    case SILABS_ANALOG: return Si2164B_DD_MODE_PROP_MODULATION_ANALOG;//kwon
    default           : return -1;
  }
  return -1;
}
/************************************************************************************************************************
  Silabs_constelCode function
  Use:        constel code function
              Used to retrieve the constel value used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  constel, the value used by the top-level application (configurable in CUSTOM_Constel_Enum)
************************************************************************************************************************/
signed int   Silabs_constelCode                  (SILABS_FE_Context* front_end, CUSTOM_Constel_Enum           constel)
{
  front_end = front_end; /* to avoid compiler warning if not used */
    switch (constel) {
      case SILABS_QAMAUTO : return Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_AUTO ;
      case SILABS_QAM16   : return Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM16 ;
      case SILABS_QAM32   : return Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM32 ;
      case SILABS_QAM64   : return Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM64 ;
      case SILABS_QAM128  : return Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM128;
      case SILABS_QAM256  : return Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM256;
      case SILABS_QPSK    : return Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_QPSK;
      case SILABS_8PSK    : return Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_8PSK;
      case SILABS_8APSK_L  : return Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_8APSK_L ;
      case SILABS_16APSK   : return Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_16APSK  ;
      case SILABS_16APSK_L : return Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_16APSK_L;
      case SILABS_32APSK_1 : return Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_32APSK_1;
      case SILABS_32APSK_2 : return Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_32APSK_2;
      case SILABS_32APSK_L : return Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_32APSK_L;
  #ifndef   Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_4_5
      case SILABS_QPSK    : return Si2164B_DVBT2_STATUS_RESPONSE_CONSTELLATION_QPSK;
  #endif /* Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_4_5 */
    default             : return -1;
  }
  return -1;
}
/************************************************************************************************************************
  Silabs_streamCode function
  Use:        stream code function
              Used to retrieve the stream value used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  stream, the value used by the top-level application (configurable in CUSTOM_Stream_Enum)
************************************************************************************************************************/
signed int   Silabs_streamCode                   (SILABS_FE_Context* front_end, CUSTOM_Stream_Enum            stream)
{
  front_end = front_end; /* to avoid compiler warning if not used */
  stream    = stream;    /* to avoid compiler warning if not used */
  switch (stream) {
    case SILABS_HP : return Si2164B_DVBT_HIERARCHY_PROP_STREAM_HP   ;
    case SILABS_LP : return Si2164B_DVBT_HIERARCHY_PROP_STREAM_LP   ;
    default           : return -1;
  }
  return -1;
}
/************************************************************************************************************************
  Silabs_plptypeCode function
  Use:        plp type code function
              Used to retrieve the plp type value used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  plp_type, the value used by the top-level application (configurable in CUSTOM_T2_PLP_TYPE)
************************************************************************************************************************/
signed int   Silabs_plptypeCode                  (SILABS_FE_Context* front_end, CUSTOM_T2_PLP_TYPE            plp_type)
{
  front_end = front_end; /* to avoid compiler warning if not used */
  plp_type  = plp_type ; /* To avoid compiler warning if not used */
  switch (plp_type) {
    case SILABS_PLP_TYPE_COMMON     : return Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_TYPE_COMMON    ;
    case SILABS_PLP_TYPE_DATA_TYPE1 : return Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_TYPE_DATA_TYPE1;
    case SILABS_PLP_TYPE_DATA_TYPE2 : return Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_TYPE_DATA_TYPE2;
    default           : return -1;
  }
  return -1;
}
/************************************************************************************************************************
  Silabs_fftCode function
  Use:        fft code function
              Used to retrieve the fft value used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  fft, the value used by the top-level application (configurable in CUSTOM_FFT_Mode_Enum)
************************************************************************************************************************/
signed int   Silabs_fftCode                      (SILABS_FE_Context* front_end, CUSTOM_FFT_Mode_Enum          fft)
{
  front_end = front_end; /* to avoid compiler warning if not used */
  fft       = fft;       /* to avoid compiler warning if not used */
  switch (fft) {
    case SILABS_FFT_MODE_2K : return Si2164B_DVBT_STATUS_RESPONSE_FFT_MODE_2K;
    case SILABS_FFT_MODE_4K : return Si2164B_DVBT_STATUS_RESPONSE_FFT_MODE_4K;
    case SILABS_FFT_MODE_8K : return Si2164B_DVBT_STATUS_RESPONSE_FFT_MODE_8K;
    case SILABS_FFT_MODE_16K: return Si2164B_DVBT2_STATUS_RESPONSE_FFT_MODE_16K;
    case SILABS_FFT_MODE_32K: return Si2164B_DVBT2_STATUS_RESPONSE_FFT_MODE_32K;
    default           : return -1;
  }
  return -1;
}
/************************************************************************************************************************
  Silabs_giCode function
  Use:        gi code function
              Used to retrieve the gi value used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  gi, the value used by the top-level application (configurable in CUSTOM_gi_Mode_Enum)
************************************************************************************************************************/
signed int   Silabs_giCode                       (SILABS_FE_Context* front_end, CUSTOM_GI_Enum                gi)
{
  front_end = front_end; /* to avoid compiler warning if not used */
  gi        = gi;        /* to avoid compiler warning if not used */
  switch (gi) {
      case SILABS_GUARD_INTERVAL_1_32  : return Si2164B_DVBT_STATUS_RESPONSE_GUARD_INT_1_32;
      case SILABS_GUARD_INTERVAL_1_16  : return Si2164B_DVBT_STATUS_RESPONSE_GUARD_INT_1_16;
      case SILABS_GUARD_INTERVAL_1_8   : return Si2164B_DVBT_STATUS_RESPONSE_GUARD_INT_1_8 ;
      case SILABS_GUARD_INTERVAL_1_4   : return Si2164B_DVBT_STATUS_RESPONSE_GUARD_INT_1_4 ;
      case SILABS_GUARD_INTERVAL_1_128 : return Si2164B_DVBT2_STATUS_RESPONSE_GUARD_INT_1_128 ;
      case SILABS_GUARD_INTERVAL_19_128: return Si2164B_DVBT2_STATUS_RESPONSE_GUARD_INT_19_128 ;
      case SILABS_GUARD_INTERVAL_19_256: return Si2164B_DVBT2_STATUS_RESPONSE_GUARD_INT_19_256 ;
 #ifndef    Si2164B_DVBT2_STATUS_RESPONSE_GUARD_INT_1_128
      case SILABS_GUARD_INTERVAL_1_128 : return Si2164B_DVBC2_STATUS_RESPONSE_GUARD_INT_1_128 ;
 #endif  /* Si2164B_DVBT2_STATUS_RESPONSE_GUARD_INT_1_128 */
      case SILABS_GUARD_INTERVAL_1_64  : return Si2164B_DVBC2_STATUS_RESPONSE_GUARD_INT_1_64   ;
    default           : return -1;
  }
  return -1;
}
/************************************************************************************************************************
  Silabs_coderateCode function
  Use:        coderate code function
              Used to retrieve the coderate value used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  coderate, the value used by the top-level application (configurable in CUSTOM_Coderate_Enum)
************************************************************************************************************************/
signed int   Silabs_coderateCode                 (SILABS_FE_Context* front_end, CUSTOM_Coderate_Enum          coderate)
{
   front_end = front_end; //kennydebug 
 switch (coderate) {
    case SILABS_CODERATE_1_2 : return Si2164B_DVBT_STATUS_RESPONSE_RATE_HP_1_2;
    case SILABS_CODERATE_2_3 : return Si2164B_DVBT_STATUS_RESPONSE_RATE_HP_2_3;
    case SILABS_CODERATE_3_4 : return Si2164B_DVBT_STATUS_RESPONSE_RATE_HP_3_4;
    case SILABS_CODERATE_4_5 : return Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_4_5;
    case SILABS_CODERATE_5_6 : return Si2164B_DVBT_STATUS_RESPONSE_RATE_HP_5_6;
    case SILABS_CODERATE_7_8 : return Si2164B_DVBT_STATUS_RESPONSE_RATE_HP_7_8;
  #ifndef   Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_4_5
      case SILABS_CODERATE_4_5 : return Si2164B_DVBT2_STATUS_RESPONSE_CODE_RATE_4_5;
      case SILABS_CODERATE_1_3 : return Si2164B_DVBT2_STATUS_RESPONSE_CODE_RATE_1_3;
      case SILABS_CODERATE_2_5 : return Si2164B_DVBT2_STATUS_RESPONSE_CODE_RATE_2_5;
      case SILABS_CODERATE_3_5 : return Si2164B_DVBT2_STATUS_RESPONSE_CODE_RATE_3_5;
  #endif /* Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_4_5 */
    case SILABS_CODERATE_8_9 : return Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_8_9;
    case SILABS_CODERATE_9_10: return Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_9_10;
    case SILABS_CODERATE_1_3 : return Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_1_3;
    case SILABS_CODERATE_2_5 : return Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_2_5;
    case SILABS_CODERATE_3_5 : return Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_3_5;
    default           : return -1;
  }
  return -1;
}
/************************************************************************************************************************
  Silabs_hierarchyCode function
  Use:        hierarchy code function
              Used to retrieve the hierarchy value used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  hierarchy, the value used by the top-level application (configurable in CUSTOM_Hierarchy_Enum)
************************************************************************************************************************/
signed int   Silabs_hierarchyCode                (SILABS_FE_Context* front_end, CUSTOM_Hierarchy_Enum         hierarchy)
{
   front_end =front_end; //kennydebug
  switch (hierarchy) {
    case SILABS_HIERARCHY_NONE : return Si2164B_DVBT_STATUS_RESPONSE_HIERARCHY_NONE;
    case SILABS_HIERARCHY_ALFA1: return Si2164B_DVBT_STATUS_RESPONSE_HIERARCHY_ALFA1;
    case SILABS_HIERARCHY_ALFA2: return Si2164B_DVBT_STATUS_RESPONSE_HIERARCHY_ALFA2;
    case SILABS_HIERARCHY_ALFA4: return Si2164B_DVBT_STATUS_RESPONSE_HIERARCHY_ALFA4;
    default           : return -1;
  }
  return -1;
}
/************************************************************************************************************************
  Silabs_pilotPatternCode function
  Use:        pilot pattern code function
              Used to retrieve the pilot pattern value used by DVB-T2 demodulators
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  pilot_pattern, the value used by the top-level application (configurable in CUSTOM_Pilot_Pattern_Enum)
************************************************************************************************************************/
signed int   Silabs_pilotPatternCode             (SILABS_FE_Context* front_end, CUSTOM_Pilot_Pattern_Enum     pilot_pattern)
{
  //front_end->chip = front_end->chip; /* To avoid compiler warning *///20140103,(no effect)
  //pilot_pattern   = pilot_pattern  ; /* To avoid compiler warning */
  front_end =front_end;  /* To avoid compiler warning */
  
  switch (pilot_pattern) {
    case SILABS_PILOT_PATTERN_PP1 : return Si2164B_DVBT2_STATUS_RESPONSE_PILOT_PATTERN_PP1;
    case SILABS_PILOT_PATTERN_PP2 : return Si2164B_DVBT2_STATUS_RESPONSE_PILOT_PATTERN_PP2;
    case SILABS_PILOT_PATTERN_PP3 : return Si2164B_DVBT2_STATUS_RESPONSE_PILOT_PATTERN_PP3;
    case SILABS_PILOT_PATTERN_PP4 : return Si2164B_DVBT2_STATUS_RESPONSE_PILOT_PATTERN_PP4;
    case SILABS_PILOT_PATTERN_PP5 : return Si2164B_DVBT2_STATUS_RESPONSE_PILOT_PATTERN_PP5;
    case SILABS_PILOT_PATTERN_PP6 : return Si2164B_DVBT2_STATUS_RESPONSE_PILOT_PATTERN_PP6;
    case SILABS_PILOT_PATTERN_PP7 : return Si2164B_DVBT2_STATUS_RESPONSE_PILOT_PATTERN_PP7;
    case SILABS_PILOT_PATTERN_PP8 : return Si2164B_DVBT2_STATUS_RESPONSE_PILOT_PATTERN_PP8;
    default           : return -1;
  }
  return -1;
}
/************************************************************************************************************************
  Silabs_T2VersionCode function
  Use:        Te version code function
              Used to retrieve the T2 version value used by DVB-T2 demodulators
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  t2_version, the value used by the top-level application
************************************************************************************************************************/
signed int   Silabs_T2VersionCode                (SILABS_FE_Context* front_end, CUSTOM_Pilot_T2_version_Enum  t2_version)
{
  //front_end->chip = front_end->chip; /* To avoid compiler warning *///20140103,(no effect)
  //t2_version      = t2_version     ; /* To avoid compiler warning */
//#ifdef    Si2164B_COMPATIBLE
  //if (front_end->chip ==   0x2164 ) {

     front_end =front_end;  /* To avoid compiler warning */

  
    switch (t2_version) {
      case SILABS_T2_VERSION_1_1_1 : return Si2164B_DVBT2_STATUS_RESPONSE_T2_VERSION_1_1_1;
      case SILABS_T2_VERSION_1_2_1 : return Si2164B_DVBT2_STATUS_RESPONSE_T2_VERSION_1_2_1;
      case SILABS_T2_VERSION_1_3_1 : return Si2164B_DVBT2_STATUS_RESPONSE_T2_VERSION_1_3_1;
      default                      : return -1;
    }
  //}
//#endif /* Si2164B_COMPATIBLE */
  //return -1;
}
/************************************************************************************************************************
  Silabs_videoSysCode function
  Use:        analog video system code function
              Used to retrieve the analog video system value used by the tuner in custom format
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  video_sys, the value used by the top-level application (as returned by the tuner)
************************************************************************************************************************/
signed int   Silabs_videoSysCode                 (SILABS_FE_Context* front_end, CUSTOM_Video_Sys_Enum         video_sys)
{
  front_end = front_end; /* To avoid compiler warning */
  video_sys = video_sys;  /* To avoid compiler warning */
  SiTRACE("Silabs_videoSysCode %d\n", video_sys);
#if 0
  switch (video_sys) {
    case SILABS_VIDEO_SYS_B  : return Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_B;
    case SILABS_VIDEO_SYS_GH : return Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_GH;
    case SILABS_VIDEO_SYS_M  : return Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_M;
    case SILABS_VIDEO_SYS_N  : return Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_N;
    case SILABS_VIDEO_SYS_I  : return Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_I;
    case SILABS_VIDEO_SYS_DK : return Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_DK;
    case SILABS_VIDEO_SYS_L  : return Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_L;
    case SILABS_VIDEO_SYS_LP : return Si2151_ATV_VIDEO_MODE_PROP_VIDEO_SYS_LP;
    default           : return -1;
  }
#endif
  return -1;
}
/************************************************************************************************************************
  Silabs_colorCode function
  Use:        analog video color code function
              Used to retrieve the analog video color value used by the tuner in custom format
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  color, the value used by the top-level application (as returned by the tuner)
************************************************************************************************************************/
signed int   Silabs_colorCode                    (SILABS_FE_Context* front_end, CUSTOM_Color_Enum             color)
{
  front_end = front_end; /* To avoid compiler warning */
  color=color;           /* To avoid compiler warning */


  
  SiTRACE("Silabs_colorCode %d\n", color);
#if 0
  switch (color) {
    case SILABS_COLOR_PAL_NTSC : return Si2151_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC;
    case SILABS_COLOR_SECAM    : return Si2151_ATV_VIDEO_MODE_PROP_COLOR_SECAM;
    default           : return -1;
  }
#endif
  return -1;
}
/************************************************************************************************************************
  Silabs_transmissionCode function
  Use:        analog video transmission code function
              Used to retrieve the analog video transmission value used by the tuner in custom format
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  trans, the value used by the top-level application (as returned by the tuner)
************************************************************************************************************************/
signed int   Silabs_transmissionCode             (SILABS_FE_Context* front_end, CUSTOM_Transmission_Mode_Enum trans)
{
  front_end = front_end; /* To avoid compiler warning if not supported */
  trans     = trans;     /* To avoid compiler warning if not supported */

  switch (trans) {
 #ifdef   Si2151_ATV_VIDEO_MODE_PROP_TRANS_TERRESTRIAL
    case SILABS_TRANSMISSION_MODE_TERRESTRIAL : return Si2151_ATV_VIDEO_MODE_PROP_TRANS_TERRESTRIAL;
    case SILABS_TRANSMISSION_MODE_CABLE       : return Si2151_ATV_VIDEO_MODE_PROP_TRANS_CABLE;
 #endif /* Si2151_ATV_VIDEO_MODE_PROP_TRANS_TERRESTRIAL */
    default           : return -1;
  }

  return -1;
}
#endif /* Translation functions from 'Custom' values to 'SiLabs' values  */
       /* Translation functions from 'SiLabs' values to 'Custom' values */

//csm,13'
signed int   Silabs_audioSysCode(SILABS_FE_Context* front_end, CUSTOM_Audio_Sys_Enum audio_sys)
{

front_end =front_end;  /* To avoid compiler warning */
audio_sys = audio_sys; /* To avoid compiler warning */

#if 0
  switch (audio_sys) {
    case SILABS_AUDIO_SYS_DEFAULT         : return Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_DEFAULT;
    case SILABS_AUDIO_SYS_MONO            : return Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_MONO;
    case SILABS_AUDIO_SYS_MONO_NICAM      : return Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_MONO_NICAM;
    case SILABS_AUDIO_SYS_A2              : return Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_A2;
    case SILABS_AUDIO_SYS_A2_DK2          : return Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_A2_DK2;
    case SILABS_AUDIO_SYS_A2_DK3          : return Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_A2_DK3;
    case SILABS_AUDIO_SYS_BTSC            : return Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_BTSC;
    case SILABS_AUDIO_SYS_EIAJ            : return Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_EIAJ;
    case SILABS_AUDIO_SYS_SCAN            : return Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_SCAN;
    case SILABS_AUDIO_SYS_A2_DK4          : return Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_A2_DK4;
    case SILABS_AUDIO_SYS_WIDE_SCAN       : return Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_WIDE_SCAN;
    case SILABS_AUDIO_SYS_MONO_NICAM_6DB  : return Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_MONO_NICAM_6DB;
    case SILABS_AUDIO_SYS_MONO_NICAM_10DB : return Si2151_ATV_AUDIO_MODE_PROP_AUDIO_SYS_MONO_NICAM_10DB;
    default           : return -1;
  }
#endif
  return -1;
}

#if 1
/************************************************************************************************************************
  Custom_standardCode function
  Use:        standard code function
              Used to retrieve the standard value used by the DTV demodulator in custom format
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  standard, the value used by the top-level application (as returned by the demod)
************************************************************************************************************************/
signed int   Custom_standardCode                 (SILABS_FE_Context* front_end, signed int standard)
{
    front_end = front_end; /* To avoid compiler warning */
  switch (standard) {
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT : return SILABS_DVB_T ;
    case Si2164B_DD_MODE_PROP_MODULATION_DVBT2: return SILABS_DVB_T2;
    case Si2164B_DD_MODE_PROP_MODULATION_DVBC : return SILABS_DVB_C ;
    case Si2164B_DD_MODE_PROP_MODULATION_DVBC2: return SILABS_DVB_C2;//csm
    case Si2164B_DD_MODE_PROP_MODULATION_MCNS : return SILABS_MCNS  ;
    case Si2164B_DD_MODE_PROP_MODULATION_DVBS : return SILABS_DVB_S ;
    case Si2164B_DD_MODE_PROP_MODULATION_DVBS2: return SILABS_DVB_S2;
    case Si2164B_DD_MODE_PROP_MODULATION_DSS  : return SILABS_DSS   ;
    default                                  : return -1;
  }
  return -1;
}
/************************************************************************************************************************
  Custom_constelCode function
  Use:        constel code function
              Used to retrieve the constel value  used by the DTV demodulator in custom format
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  constel, the value used by the top-level application (as returned by the demod)
************************************************************************************************************************/
signed int   Custom_constelCode                  (SILABS_FE_Context* front_end, signed int constel)
{
    front_end = front_end;  /* To avoid compiler warning */
    
    switch (constel) {
      case Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_AUTO  : return SILABS_QAMAUTO ;
      case Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM16 : return SILABS_QAM16   ;
      case Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM32 : return SILABS_QAM32   ;
      case Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM64 : return SILABS_QAM64   ;
      case Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM128: return SILABS_QAM128  ;
      case Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM256: return SILABS_QAM256  ;
      case Si2164B_DVBC2_STATUS_RESPONSE_CONSTELLATION_QAM1024 : return SILABS_QAM1024 ;
      case Si2164B_DVBC2_STATUS_RESPONSE_CONSTELLATION_QAM4096 : return SILABS_QAM4096 ;
      case Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_QPSK    : return SILABS_QPSK    ;
      case Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_8PSK    : return SILABS_8PSK    ;
      case Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_16APSK  : return SILABS_16APSK  ;
      case Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_32APSK_1: return SILABS_32APSK_1;
      case Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_8APSK_L : return SILABS_8APSK_L ;
      case Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_16APSK_L: return SILABS_16APSK_L;
      case Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_32APSK_2: return SILABS_32APSK_2;
      case Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_32APSK_L: return SILABS_32APSK_L;
  #ifndef   Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_QPSK
      case Si2164B_DVBT2_STATUS_RESPONSE_CONSTELLATION_QPSK    : return SILABS_QPSK    ;
  #endif /* Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_QPSK */
    default                                                 : return -1;
  }
  return -1;
}
/************************************************************************************************************************
  Custom_streamCode function
  Use:        stream code function
              Used to retrieve the stream value used by the DTV demodulator in custom format
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  stream, the value used by the top-level application (as returned by the demod)
************************************************************************************************************************/
signed int   Custom_streamCode                   (SILABS_FE_Context* front_end, signed int stream)
{
  front_end = front_end; /* To avoid compiler warning if not supported */
  stream    = stream;    /* To avoid compiler warning if not supported */
  switch (stream) {
    case Si2164B_DVBT_HIERARCHY_PROP_STREAM_HP: return SILABS_HP;
    case Si2164B_DVBT_HIERARCHY_PROP_STREAM_LP: return SILABS_LP;
    default                                  : return -1;
  }
  return -1;
}
/************************************************************************************************************************
  Custom_plptypeCode function
  Use:        plp type code function
              Used to retrieve the plp type value used by the DTV demodulator in custom format
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  plp_type, the value used by the top-level application (as returned by the demod)
************************************************************************************************************************/
signed int   Custom_plptypeCode                  (SILABS_FE_Context* front_end, signed int plp_type)
{
  front_end = front_end; /* To avoid compiler warning if not supported */
  plp_type  = plp_type;    /* To avoid compiler warning if not supported */
  switch (plp_type) {
    case Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_TYPE_COMMON    : return SILABS_PLP_TYPE_COMMON;
    case Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_TYPE_DATA_TYPE1: return SILABS_PLP_TYPE_DATA_TYPE1;
    case Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_TYPE_DATA_TYPE2: return SILABS_PLP_TYPE_DATA_TYPE2;
    default                                  : return -1;
  }
  return -1;
}
/************************************************************************************************************************
  Custom_fftCode function
  Use:        fft code function
              Used to retrieve the fft value used by the DTV demodulator in custom format
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  fft, the value used by the top-level application (as returned by the demod)
************************************************************************************************************************/
signed int   Custom_fftCode                      (SILABS_FE_Context* front_end, signed int fft)
{
  front_end = front_end; /* To avoid compiler warning if not supported */
  fft       = fft;       /* To avoid compiler warning if not supported */
  switch (fft) {
    case Si2164B_DVBT_STATUS_RESPONSE_FFT_MODE_2K  : return SILABS_FFT_MODE_2K ;
    case Si2164B_DVBT_STATUS_RESPONSE_FFT_MODE_4K  : return SILABS_FFT_MODE_4K ;
    case Si2164B_DVBT_STATUS_RESPONSE_FFT_MODE_8K  : return SILABS_FFT_MODE_8K ;
    case Si2164B_DVBT2_STATUS_RESPONSE_FFT_MODE_16K: return SILABS_FFT_MODE_16K;
    case Si2164B_DVBT2_STATUS_RESPONSE_FFT_MODE_32K: return SILABS_FFT_MODE_32K;
    default                                       : return -1;
  }
  return -1;
}
/************************************************************************************************************************
  Custom_giCode function
  Use:        gi code function
              Used to retrieve the gi value used by the DTV demodulator in custom format
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  gi, the value used by the top-level application (as returned by the demod)
************************************************************************************************************************/
signed int   Custom_giCode                       (SILABS_FE_Context* front_end, signed int gi)
{
  front_end = front_end; /* To avoid compiler warning if not supported */
  gi        = gi;        /* To avoid compiler warning if not supported */
  switch (gi) {
      case Si2164B_DVBT_STATUS_RESPONSE_GUARD_INT_1_32   : return SILABS_GUARD_INTERVAL_1_32  ;
      case Si2164B_DVBT_STATUS_RESPONSE_GUARD_INT_1_16   : return SILABS_GUARD_INTERVAL_1_16  ;
      case Si2164B_DVBT_STATUS_RESPONSE_GUARD_INT_1_8    : return SILABS_GUARD_INTERVAL_1_8   ;
      case Si2164B_DVBT_STATUS_RESPONSE_GUARD_INT_1_4    : return SILABS_GUARD_INTERVAL_1_4   ;
      case Si2164B_DVBT2_STATUS_RESPONSE_GUARD_INT_1_128 : return SILABS_GUARD_INTERVAL_1_128 ;
      case Si2164B_DVBT2_STATUS_RESPONSE_GUARD_INT_19_128: return SILABS_GUARD_INTERVAL_19_128;
      case Si2164B_DVBT2_STATUS_RESPONSE_GUARD_INT_19_256: return SILABS_GUARD_INTERVAL_19_256;
#ifndef    Si2164B_DVBT2_STATUS_RESPONSE_GUARD_INT_1_128
      case Si2164B_DVBC2_STATUS_RESPONSE_GUARD_INT_1_128 : return SILABS_GUARD_INTERVAL_1_128 ;
#endif /* Si2164B_DVBT2_STATUS_RESPONSE_GUARD_INT_1_128 */
      case Si2164B_DVBC2_STATUS_RESPONSE_GUARD_INT_1_64  : return SILABS_GUARD_INTERVAL_1_64  ;
    default                                           : return -1;
  }
  return -1;
}
/************************************************************************************************************************
  Custom_coderateCode function
  Use:        coderate code function
              Used to retrieve the coderate value used by the DTV demodulator in custom format
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  coderate, the value used by the top-level application (as returned by the demod)
************************************************************************************************************************/
signed int   Custom_coderateCode                 (SILABS_FE_Context* front_end, signed int coderate)
{
  front_end = front_end; /* To avoid compiler warning if not supported */
  coderate  = coderate;  /* To avoid compiler warning if not supported */
    switch (coderate) {
      case Si2164B_DVBT_STATUS_RESPONSE_RATE_HP_1_2    : return SILABS_CODERATE_1_2;
      case Si2164B_DVBT_STATUS_RESPONSE_RATE_HP_2_3    : return SILABS_CODERATE_2_3;
      case Si2164B_DVBT_STATUS_RESPONSE_RATE_HP_3_4    : return SILABS_CODERATE_3_4;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_4_5 : return SILABS_CODERATE_4_5;
      case Si2164B_DVBT_STATUS_RESPONSE_RATE_HP_5_6    : return SILABS_CODERATE_5_6;
      case Si2164B_DVBT_STATUS_RESPONSE_RATE_HP_7_8    : return SILABS_CODERATE_7_8;
  #ifndef    Si2164B_DVBT_STATUS_RESPONSE_RATE_HP_1_2
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_1_2 : return SILABS_CODERATE_1_2;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_2_3 : return SILABS_CODERATE_2_3;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_3_4 : return SILABS_CODERATE_3_4;
  #endif  /* Si2164B_DVBT_STATUS_RESPONSE_RATE_HP_1_2 */
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_8_9 : return SILABS_CODERATE_8_9;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_9_10: return SILABS_CODERATE_9_10;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_1_3 : return SILABS_CODERATE_1_3;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_1_4 : return SILABS_CODERATE_1_4;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_2_5 : return SILABS_CODERATE_2_5;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_3_5 : return SILABS_CODERATE_3_5;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_13_45 : return SILABS_CODERATE_13_45;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_9_20  : return SILABS_CODERATE_9_20;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_8_15  : return SILABS_CODERATE_8_15;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_11_20 : return SILABS_CODERATE_11_20;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_5_9   : return SILABS_CODERATE_5_9;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_26_45 : return SILABS_CODERATE_26_45;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_28_45 : return SILABS_CODERATE_28_45;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_23_36 : return SILABS_CODERATE_23_36;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_25_36 : return SILABS_CODERATE_25_36;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_32_45 : return SILABS_CODERATE_32_45;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_13_18 : return SILABS_CODERATE_13_18;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_11_15 : return SILABS_CODERATE_11_15;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_7_9   : return SILABS_CODERATE_7_9;
      case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_77_90 : return SILABS_CODERATE_77_90;
  #ifndef    Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_4_5
      case Si2164B_DVBT2_STATUS_RESPONSE_CODE_RATE_4_5 : return SILABS_CODERATE_4_5;
      case Si2164B_DVBT2_STATUS_RESPONSE_CODE_RATE_1_3 : return SILABS_CODERATE_1_3;
      case Si2164B_DVBT2_STATUS_RESPONSE_CODE_RATE_2_5 : return SILABS_CODERATE_2_5;
      case Si2164B_DVBT2_STATUS_RESPONSE_CODE_RATE_3_5 : return SILABS_CODERATE_3_5;
  #endif /* Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_4_5 */
      default                                         : return -1;
    }
  return -1;
}
/************************************************************************************************************************
  Custom_hierarchyCode function
  Use:        hierarchy code function
              Used to retrieve the hierarchy value used by the DTV demodulator in custom format
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  hierarchy, the value used by the top-level application (as returned by the demod)
************************************************************************************************************************/
signed int   Custom_hierarchyCode                (SILABS_FE_Context* front_end, signed int hierarchy)
{
  front_end = front_end; /* To avoid compiler warning if not supported */
  hierarchy = hierarchy; /* To avoid compiler warning if not supported */
    switch (hierarchy) {
      case Si2164B_DVBT_STATUS_RESPONSE_HIERARCHY_NONE : return SILABS_HIERARCHY_NONE;
      case Si2164B_DVBT_STATUS_RESPONSE_HIERARCHY_ALFA1: return SILABS_HIERARCHY_ALFA1;
      case Si2164B_DVBT_STATUS_RESPONSE_HIERARCHY_ALFA2: return SILABS_HIERARCHY_ALFA2;
      case Si2164B_DVBT_STATUS_RESPONSE_HIERARCHY_ALFA4: return SILABS_HIERARCHY_ALFA4;
      default                                         : return -1;
    }
  return -1;
}
/************************************************************************************************************************
  Custom_pilotPatternCode function
  Use:        pilot pattern code function
              Used to retrieve the pilot pattern value used by the DVB-T2 demodulator in custom format
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  pilot_pattern, the value used by the top-level application (as returned by the demod)
************************************************************************************************************************/
signed int   Custom_pilotPatternCode             (SILABS_FE_Context* front_end, signed int pilot_pattern)
{
  //front_end->chip = front_end->chip; /* To avoid compiler warning if not supported *///20140103,(no effect)
  //pilot_pattern   = pilot_pattern  ; /* To avoid compiler warning if not supported */

  front_end = front_end; /* To avoid compiler warning if not supported */
  pilot_pattern = pilot_pattern; /* To avoid compiler warning if not supported */
  
  
  switch (pilot_pattern) {
    case Si2164B_DVBT2_STATUS_RESPONSE_PILOT_PATTERN_PP1: return SILABS_PILOT_PATTERN_PP1;
    case Si2164B_DVBT2_STATUS_RESPONSE_PILOT_PATTERN_PP2: return SILABS_PILOT_PATTERN_PP2;
    case Si2164B_DVBT2_STATUS_RESPONSE_PILOT_PATTERN_PP3: return SILABS_PILOT_PATTERN_PP3;
    case Si2164B_DVBT2_STATUS_RESPONSE_PILOT_PATTERN_PP4: return SILABS_PILOT_PATTERN_PP4;
    case Si2164B_DVBT2_STATUS_RESPONSE_PILOT_PATTERN_PP5: return SILABS_PILOT_PATTERN_PP5;
    case Si2164B_DVBT2_STATUS_RESPONSE_PILOT_PATTERN_PP6: return SILABS_PILOT_PATTERN_PP6;
    case Si2164B_DVBT2_STATUS_RESPONSE_PILOT_PATTERN_PP7: return SILABS_PILOT_PATTERN_PP7;
    case Si2164B_DVBT2_STATUS_RESPONSE_PILOT_PATTERN_PP8: return SILABS_PILOT_PATTERN_PP8;
    default                                            : return -1;
  }
  return -1;
}
/************************************************************************************************************************
  Custom_T2VersionCode function
  Use:        pilot pattern code function
              Used to retrieve the t2_version value used by the DVB-T2 demodulator in custom format
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  t2_version, the value used by the top-level application (as returned by the demod)
************************************************************************************************************************/
signed int   Custom_T2VersionCode                (SILABS_FE_Context* front_end, signed int t2_version)
{
  //front_end->chip = front_end->chip; /* To avoid compiler warning if not supported *///20140103,(no effect)
  //t2_version      = t2_version     ; /* To avoid compiler warning if not supported */
//#ifdef    Si2164B_COMPATIBLE
//  if (front_end->chip ==   0x2164 ) {

  front_end = front_end; /* To avoid compiler warning if not supported */
  t2_version = t2_version; /* To avoid compiler warning if not supported */



    switch (t2_version) {
      case Si2164B_DVBT2_STATUS_RESPONSE_T2_VERSION_1_1_1: return SILABS_T2_VERSION_1_1_1;
      case Si2164B_DVBT2_STATUS_RESPONSE_T2_VERSION_1_2_1: return SILABS_T2_VERSION_1_2_1;
      case Si2164B_DVBT2_STATUS_RESPONSE_T2_VERSION_1_3_1: return SILABS_T2_VERSION_1_3_1;
      default                                            : return -1;
    }
//  }
//#endif /* Si2164B_COMPATIBLE */
//  return -1;
}

#endif /* Translation functions from 'SiLabs' values to 'Custom' values */
       /* Text functions returning strings based on 'Custom' values.    */
#if 1
/************************************************************************************************************************
  Silabs_Standard_Text function
  Use:        standard text retrieval function
              Used to retrieve the standard text used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  standard, the value used by the top-level application (configurable in CUSTOM_Standard_Enum)
************************************************************************************************************************/
const unsigned char *Silabs_Standard_Text                (CUSTOM_Standard_Enum  standard)
{
  switch (standard) {
    case SILABS_ANALOG: {return (unsigned char*)"ANALOG" ;}
    case SILABS_DVB_T : {return (unsigned char*)"DVB-T"  ;}
    case SILABS_DVB_T2: {return (unsigned char*)"DVB-T2" ;}
    case SILABS_DVB_C : {return (unsigned char*)"DVB-C"  ;}
    case SILABS_DVB_C2: {return (unsigned char*)"DVB-C2" ;}
    case SILABS_MCNS  : {return (unsigned char*)"MCNS"   ;}
    case SILABS_DVB_S : {return (unsigned char*)"DVB-S"  ;}
    case SILABS_DVB_S2: {return (unsigned char*)"DVB-S2" ;}
    case SILABS_DSS   : {return (unsigned char*)"DSS"    ;}
    case SILABS_ISDB_T: {return (unsigned char*)"ISDB-T" ;}
    case SILABS_SLEEP : {return (unsigned char*)"SLEEP"  ;}
    default           : {return (unsigned char*)"UNKNOWN";}
  }
}
/************************************************************************************************************************
  Silabs_Standard_Capability function
  Use:        standard capability retrieval function
              Used to know whether the front-end can handle a given standard
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  standard, the value used by the top-level application (configurable in CUSTOM_Standard_Enum)
  Returns  :  1 if the front-end can demodulate the standard
************************************************************************************************************************/
signed int   Silabs_Standard_Capability          (CUSTOM_Standard_Enum  standard)
{
  switch (standard) {
    case SILABS_ANALOG: {return 1;}
    case SILABS_DVB_T : {return 1;}
    case SILABS_DVB_T2: {return 1;}
    case SILABS_DVB_C : {return 1;}
    case SILABS_DVB_C2: {return 1;}
    case SILABS_MCNS  : {return 1;}
    case SILABS_DVB_S :
    case SILABS_DVB_S2:
    case SILABS_DSS   : {return 1;}
    default           : {return 0;}
  }
  return 0;
}
/************************************************************************************************************************
  Silabs_Constel_Text function
  Use:        constel text retrieval function
              Used to retrieve the constel text used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  constel, the value used by the top-level application (configurable in CUSTOM_Constel_Enum)
************************************************************************************************************************/
const unsigned char *Silabs_Constel_Text                 (CUSTOM_Constel_Enum   constel)
{
  switch (constel) {
    case SILABS_QAMAUTO : { return (unsigned char*)"QAMAUTO"; break;}
    case SILABS_QAM16   : { return (unsigned char*)"QAM16"  ; break;}
    case SILABS_QAM32   : { return (unsigned char*)"QAM32"  ; break;}
    case SILABS_QAM64   : { return (unsigned char*)"QAM64"  ; break;}
    case SILABS_QAM128  : { return (unsigned char*)"QAM128" ; break;}
    case SILABS_QAM256  : { return (unsigned char*)"QAM256" ; break;}
    case SILABS_QAM1024 : { return (unsigned char*)"QAM1024"; break;}
    case SILABS_QAM4096 : { return (unsigned char*)"QAM4096"; break;}
    case SILABS_QPSK    : { return (unsigned char*)"QPSK"   ; break;}
    case SILABS_8PSK    : { return (unsigned char*)"8PSK"   ; break;}
    case SILABS_8APSK_L : { return (unsigned char*)"8APSK"   ; break;}
    case SILABS_16APSK  : { return (unsigned char*)"16APSK"  ; break;}
    case SILABS_16APSK_L: { return (unsigned char*)"16APSK_L"; break;}
    case SILABS_32APSK_1: { return (unsigned char*)"32APSK_1"; break;}
    case SILABS_32APSK_2: { return (unsigned char*)"32APSK_2"; break;}
    case SILABS_32APSK_L: { return (unsigned char*)"32APSK_L"; break;}
    default             : { return (unsigned char*)"UNKNOWN"; break;}
  }
}
/************************************************************************************************************************
  Silabs_Polarization_Text function
  Use:        polarization text retrieval function
              Used to retrieve the polarization text used by the front-end
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  polarization, the value used by the top-level application (configurable in CUSTOM_Polarization_Enum)
************************************************************************************************************************/
const unsigned char *Silabs_Polarization_Text            (CUSTOM_Polarization_Enum   polarization)
{
  switch (polarization) {
    case SILABS_POLARIZATION_HORIZONTAL    : { return (unsigned char*)"Horizontal"; break;}
    case SILABS_POLARIZATION_VERTICAL      : { return (unsigned char*)"Vertical"  ; break;}
    case SILABS_POLARIZATION_DO_NOT_CHANGE : { return (unsigned char*)"Unchanged" ; break;}
    default                                : { return (unsigned char*)"UNKNOWN"   ; break;}
  }
}
/************************************************************************************************************************
  Silabs_Band_Text function
  Use:        polarization text retrieval function
              Used to retrieve the polarization text used by the front-end
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  polarization, the value used by the top-level application (configurable in CUSTOM_Band_Enum)
************************************************************************************************************************/
const unsigned char *Silabs_Band_Text                    (CUSTOM_Band_Enum      band)
{
  switch (band) {
    case SILABS_BAND_LOW          : { return (unsigned char*)"Low "     ; break;}
    case SILABS_BAND_HIGH         : { return (unsigned char*)"High"     ; break;}
    case SILABS_BAND_DO_NOT_CHANGE: { return (unsigned char*)"Unchanged"; break;}
    default                       : { return (unsigned char*)"UNKNOWN"  ; break;}
  }
}
/************************************************************************************************************************
  Silabs_Stream_Text function
  Use:        stream text retrieval function
              Used to retrieve the stream text used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  stream, the value used by the top-level application (configurable in CUSTOM_Stream_Enum)
************************************************************************************************************************/
const unsigned char *Silabs_Stream_Text                  (CUSTOM_Stream_Enum    stream)
{
  switch (stream) {
    case SILABS_HP    : { return (unsigned char*)"HP"     ; break;}
    case SILABS_LP    : { return (unsigned char*)"LP"     ; break;}
    default           : { return (unsigned char*)"UNKNOWN"; break;}
  }
}
/************************************************************************************************************************
  Silabs_TS_Mode_Text function
  Use:        Ts mode text retrieval function
              Used to retrieve the stream text used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  ts_mode, the value used by the top-level application (configurable in CUSTOM_Stream_Enum)
************************************************************************************************************************/
const unsigned char *Silabs_TS_Mode_Text                  (CUSTOM_TS_Mode_Enum    ts_mode)
{
  switch (ts_mode) {
#ifdef    USB_Capability
    case SILABS_TS_GPIF      : { return (unsigned char*)"GPIF"       ; break;}
#endif /* USB_Capability */
    case SILABS_TS_OFF       : { return (unsigned char*)"OFF"        ; break;}
    case SILABS_TS_PARALLEL  : { return (unsigned char*)"PARALLEL"   ; break;}
    case SILABS_TS_SERIAL    : { return (unsigned char*)"SERIAL"     ; break;}
    case SILABS_TS_TRISTATE  : { return (unsigned char*)"TRISTATE"   ; break;}
    default                  : { return (unsigned char*)"UNKNOWN"    ; break;}
  }
}
/************************************************************************************************************************
  Silabs_PLPType_Text function
  Use:        plp type text retrieval function
              Used to retrieve the plp type text string
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  plp_type, the value used by the top-level application (configurable in CUSTOM_T2_PLP_TYPE)
************************************************************************************************************************/
const unsigned char *Silabs_PLPType_Text                 (CUSTOM_T2_PLP_TYPE    plp_type)
{
  switch (plp_type) {
    case SILABS_PLP_TYPE_COMMON     : { return (unsigned char*)"COMMON"     ; break;}
    case SILABS_PLP_TYPE_DATA_TYPE1 : { return (unsigned char*)"DATA_TYPE1" ; break;}
    case SILABS_PLP_TYPE_DATA_TYPE2 : { return (unsigned char*)"DATA_TYPE2" ; break;}
    default           : { return (unsigned char*)"UNKNOWN"; break;}
  }
}
/************************************************************************************************************************
  Silabs_FFT_Text function
  Use:        fft text retrieval function
              Used to retrieve the fft text used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  fft, the value used by the top-level application (configurable in CUSTOM_FFT_Mode_Enum)
************************************************************************************************************************/
const unsigned char *Silabs_FFT_Text                     (CUSTOM_FFT_Mode_Enum  fft)
{
  switch (fft) {
    case SILABS_FFT_MODE_2K   : { return (unsigned char*)" 2K"    ; break;}
    case SILABS_FFT_MODE_4K   : { return (unsigned char*)" 4K"    ; break;}
    case SILABS_FFT_MODE_8K   : { return (unsigned char*)" 8K"    ; break;}
    case SILABS_FFT_MODE_16K  : { return (unsigned char*)"16K"    ; break;}
    case SILABS_FFT_MODE_32K  : { return (unsigned char*)"32K"    ; break;}
    default                   : { SiTRACE("UNKNOWN FFT mode value %d\n", fft); return (unsigned char*)"UNKNOWN"; break;}
  }
}
/************************************************************************************************************************
  Silabs_GI_Text function
  Use:        gi text retrieval function
              Used to retrieve the gi text used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  gi, the value used by the top-level application (configurable in CUSTOM_GI_Enum)
************************************************************************************************************************/
const unsigned char *Silabs_GI_Text                      (CUSTOM_GI_Enum        gi)
{
  switch (gi) {
    case SILABS_GUARD_INTERVAL_1_32     : { return (unsigned char*)"1/32"    ; break;}
    case SILABS_GUARD_INTERVAL_1_16     : { return (unsigned char*)"1/16"    ; break;}
    case SILABS_GUARD_INTERVAL_1_8      : { return (unsigned char*)"1/8"     ; break;}
    case SILABS_GUARD_INTERVAL_1_4      : { return (unsigned char*)"1/4"     ; break;}
    case SILABS_GUARD_INTERVAL_1_128    : { return (unsigned char*)"1/128"   ; break;}
    case SILABS_GUARD_INTERVAL_19_128   : { return (unsigned char*)"19/128"  ; break;}
    case SILABS_GUARD_INTERVAL_19_256   : { return (unsigned char*)"19/256"  ; break;}
    case SILABS_GUARD_INTERVAL_1_64     : { return (unsigned char*)"(unsigned char*)1/64"    ; break;}
    default                             : { SiTRACE("UNKNOWN GI value %d\n", gi); return (unsigned char*)"UNKNOWN" ; break;}
  }
}
/************************************************************************************************************************
  Silabs_Coderate_Text function
  Use:        coderate text retrieval function
              Used to retrieve the coderate text used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  coderate, the value used by the top-level application (configurable in CUSTOM_Coderate_Enum)
************************************************************************************************************************/
const unsigned char *Silabs_Coderate_Text                (CUSTOM_Coderate_Enum  coderate)
{
  switch (coderate) {
    case SILABS_CODERATE_1_2    : { return (unsigned char*)"1/2"    ; break;}
    case SILABS_CODERATE_2_3    : { return (unsigned char*)"2/3"    ; break;}
    case SILABS_CODERATE_3_4    : { return (unsigned char*)"3/4"    ; break;}
    case SILABS_CODERATE_4_5    : { return (unsigned char*)"4/5"    ; break;}
    case SILABS_CODERATE_5_6    : { return (unsigned char*)"5/6"    ; break;}
    case SILABS_CODERATE_7_8    : { return (unsigned char*)"7/8"    ; break;}
    case SILABS_CODERATE_8_9    : { return (unsigned char*)"8/9"    ; break;}
    case SILABS_CODERATE_9_10   : { return (unsigned char*)"9/10"   ; break;}
    case SILABS_CODERATE_1_3    : { return (unsigned char*)"1/3"    ; break;}
    case SILABS_CODERATE_1_4    : { return (unsigned char*)"1/4"    ; break;}
    case SILABS_CODERATE_2_5    : { return (unsigned char*)"2/5"    ; break;}
    case SILABS_CODERATE_3_5    : { return (unsigned char*)"3/5"    ; break;}
    case SILABS_CODERATE_5_9    : { return (unsigned char*)"5/9"    ; break;}
    case SILABS_CODERATE_7_9    : { return (unsigned char*)"7/9"    ; break;}
    case SILABS_CODERATE_8_15   : { return (unsigned char*)"8/15"   ; break;}
    case SILABS_CODERATE_11_15  : { return (unsigned char*)"11/15"  ; break;}
    case SILABS_CODERATE_13_18  : { return (unsigned char*)"13/18"  ; break;}
    case SILABS_CODERATE_9_20   : { return (unsigned char*)"9/20"   ; break;}
    case SILABS_CODERATE_11_20  : { return (unsigned char*)"11/20"  ; break;}
    case SILABS_CODERATE_23_36  : { return (unsigned char*)"23/36"  ; break;}
    case SILABS_CODERATE_25_36  : { return (unsigned char*)"25/36"  ; break;}
    case SILABS_CODERATE_13_45  : { return (unsigned char*)"13/45"  ; break;}
    case SILABS_CODERATE_26_45  : { return (unsigned char*)"26/45"  ; break;}
    case SILABS_CODERATE_28_45  : { return (unsigned char*)"28/45"  ; break;}
    case SILABS_CODERATE_32_45  : { return (unsigned char*)"32/45"  ; break;}
    case SILABS_CODERATE_77_90  : { return (unsigned char*)"77/90"  ; break;}
    default                     : { SiTRACE("UNKNOWN code rate value %d\n", coderate); return (unsigned char*)"UNKNOWN"; break;}
  }
}
/************************************************************************************************************************
  Silabs_Extended_BW_Text function
  Use:        bw_ext text retrieval function
              Used to retrieve the bw_ext text used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  bw_extended, value used by the top-level application (configurable in CUSTOM_T2_BwExtended_Enum)
************************************************************************************************************************/
const unsigned char *Silabs_Extended_BW_Text             (signed int bw_extended)
{
  switch (bw_extended) {
    case 0  : { return (unsigned char*)"normal  " ; break;}
    case 1  : { return (unsigned char*)"extended" ; break;}
    default : { SiTRACE("UNKNOWN bw_extended value %d\n",bw_extended); return (unsigned char*)"UNKNOWN" ; break;}
  }
}
/************************************************************************************************************************
  Silabs_Rotated_QAM_Text function
  Use:        rotated text retrieval function
              Used to retrieve the rotated text used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  rotated, a 0/1 flag indicating the QAM rotation (0 = 'normal', 1 = 'rotated')
************************************************************************************************************************/
const unsigned char *Silabs_Rotated_QAM_Text             (signed int rotated)
{
  switch (rotated) {
    case 0  : { return (unsigned char*)"normal "     ; break;}
    case 1  : { return (unsigned char*)"rotated"     ; break;}
    default : { SiTRACE("UNKNOWN QAM rotation value %d\n",rotated); return (unsigned char*)"UNKNOWN"; break;}
  }
}
/************************************************************************************************************************
  Silabs_T2_Base_Lite_Text function
  Use:        T2 Base / Lite text retrieval function
              Used to retrieve the 'base/lite' T2 mode used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  t2_base_lite, a 0/1 flag indicating the T2 mode (0 = 'base', 1 = 'lite')
************************************************************************************************************************/
const unsigned char *Silabs_T2_Base_Lite_Text            (signed int t2_base_lite)
{
  switch (t2_base_lite) {
    case 0  : { return (unsigned char*)"T2-Base"     ; break;}
    case 1  : { return (unsigned char*)"T2-Lite"     ; break;}
    default : { SiTRACE("UNKNOWN T2 base/lite value %d\n",t2_base_lite); return (unsigned char*)"UNKNOWN"; break;}
  }
}
/************************************************************************************************************************
  Silabs_T2_Version_Text function
  Use:        T2 Version text retrieval function
              Used to retrieve the T2 version used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  t2_version, a value indicating the T2 version
************************************************************************************************************************/
const unsigned char *Silabs_T2_Version_Text              (CUSTOM_Pilot_T2_version_Enum t2_version)
{
  switch (t2_version) {
    case SILABS_T2_VERSION_1_1_1  : { return (unsigned char*)"1.1.1"     ; break;}
    case SILABS_T2_VERSION_1_2_1  : { return (unsigned char*)"1.2.1"     ; break;}
    case SILABS_T2_VERSION_1_3_1  : { return (unsigned char*)"1.3.1"     ; break;}
    default : { SiTRACE("UNKNOWN T2 version value %d\n",t2_version); return (unsigned char*)"UNKNOWN"; break;}
  }
}
/************************************************************************************************************************
  Silabs_FEF_Text function
  Use:        fef text retrieval function
              Used to retrieve the fef text used by the demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  fef, a 0/1 flag indicating the fef presence (0 = 'no fef', 1 = 'fef')
************************************************************************************************************************/
const unsigned char *Silabs_FEF_Text                     (signed int fef)
{
  switch (fef) {
    case 0   : { return (unsigned char*)"  no"   ; break;}
    case 1   : { return (unsigned char*)"with"   ; break;}
    default  : { SiTRACE("UNKNOWN FEF value %d\n",fef); return (unsigned char*)"UNKNOWN"; break;}
  }
}
/************************************************************************************************************************
  Silabs_MISO_Text function
  Use:        tx text retrieval function
              Used to retrieve the tx text used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  tx, the value used by the top-level application (1 for MISO, 0 otherwise)
************************************************************************************************************************/
const unsigned char *Silabs_MISO_Text                    (signed int siso_miso)
{
  switch (siso_miso) {
    case 0   : { return (unsigned char*)"SISO"  ; break;}
    case 1   : { return (unsigned char*)"MISO"  ; break;}
    default  : { SiTRACE("UNKNOWN tx_mode %d\n", siso_miso); return (unsigned char*)"UNKNOWN"; break;}
  }
}
/************************************************************************************************************************
  Silabs_Pilot_Pattern_Text function
  Use:        pilot_pattern text retrieval function
              Used to retrieve the pilot_pattern text used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  pilot_pattern, the value used by the top-level application (configurable in CUSTOM_Pilot_Pattern_Enum)
************************************************************************************************************************/
const unsigned char *Silabs_Pilot_Pattern_Text           (CUSTOM_Pilot_Pattern_Enum pilot_pattern)
{
  switch (pilot_pattern) {
    case SILABS_PILOT_PATTERN_PP1   : { return (unsigned char*)"PP1"  ; break;}
    case SILABS_PILOT_PATTERN_PP2   : { return (unsigned char*)"PP2"  ; break;}
    case SILABS_PILOT_PATTERN_PP3   : { return (unsigned char*)"PP3"  ; break;}
    case SILABS_PILOT_PATTERN_PP4   : { return (unsigned char*)"PP4"  ; break;}
    case SILABS_PILOT_PATTERN_PP5   : { return (unsigned char*)"PP5"  ; break;}
    case SILABS_PILOT_PATTERN_PP6   : { return (unsigned char*)"PP6"  ; break;}
    case SILABS_PILOT_PATTERN_PP7   : { return (unsigned char*)"PP7"  ; break;}
    case SILABS_PILOT_PATTERN_PP8   : { return (unsigned char*)"PP8"  ; break;}
    default                         : { SiTRACE("UNKNOWN Pilot Pattern %d\n", pilot_pattern); return (unsigned char*)"UNKNOWN"; break;}
  }
}
/************************************************************************************************************************
  Silabs_No_Short_Frame_Text function
  Use:        frame text retrieval function
              Used to retrieve the frame text used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  no_short_frame, the value used by the top-level application (0 for 'short frame', 1 for 'normal frame')
************************************************************************************************************************/
const unsigned char *Silabs_No_Short_Frame_Text          (signed int no_short_frame)
{
  switch (no_short_frame) {
    case 0   : { return (unsigned char*)"short  "; break;}
    case 1   : { return (unsigned char*)"normal "; break;}
    default  : { SiTRACE("UNKNOWN short frame value %d\n", no_short_frame); return (unsigned char*)"UNKNOWN"; break;}
  }
}
/************************************************************************************************************************
  Silabs_Pilots_Text function
  Use:        frame text retrieval function
              Used to retrieve the frame text used by the DTV demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  pilots, the value used by the top-level application (0 for 'off', 1 for 'on')
************************************************************************************************************************/
const unsigned char *Silabs_Pilots_Text                  (signed int pilots)
{
  switch (pilots) {
    case 0   : { return (unsigned char*)"OFF"    ; break;}
    case 1   : { return (unsigned char*)"ON "    ; break;}
    default  : { SiTRACE("UNKNOWN pilots flag %d\n",pilots); return (unsigned char*)"UNKNOWN"; break;}
  }
}
#endif /* Text functions returning strings based on 'Custom' values.    */

/************************************************************************************************************************
  rate_f_mant_exp function
  Use:        rate computing function
              Used to fill the integer/decimal/exponent parts of a scientific-displayed rate depending on
               the mant and exp values returned by the SiLabs API functions (used for BER/PER/FER display)
  Parameter:  mant, the matissa value
  Parameter:  exp , the exponent value
  Parameter:  *rate_i, a pointer to a char used to store the integer part of the rate
  Parameter:  *rate_d, a pointer to a char used to store the decimal part of the rate
  Parameter:  *rate_e, a pointer to a char used to store the exponent part of the rate
  Returns:    0
************************************************************************************************************************/
signed int   rate_f_mant_exp                    (signed int mant, signed int int_exp, signed char *rate_i, signed char *rate_d, signed char *rate_e )
{
  if (mant ==  0) {
    *rate_i = *rate_d = *rate_e = 0;
    return 0;
  }
  if (mant >= 10) {
    *rate_i = mant/10;
    *rate_d = mant%10;
    *rate_e = -int_exp+1;
  } else {
    *rate_i = mant;
    *rate_d = 0;
    *rate_e = -int_exp;
  }
  return 0;
}
/* Chip detection function (To Be Defined) */
/************************************************************************************************************************
  SiLabs_chip_detect function
  Use:        chip detection function
              Used to detect whether the demodulator is a DTV demodulator
  Behavior:   This function uses raw i2c reads to check the presence of either a Si2167 or a Si2169
  Parameter:  demodAdd, the I2C address of the demod
  Returns:    2167 if there is a Si2167, 2169 if there is a 2169, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_chip_detect                  (signed int demodAdd)
{
  signed int chip;
  chip = 0;
  SiTRACE("Detecting chip at address 0x%02x\n", demodAdd);
  demodAdd = demodAdd;
  chip = 0x2164B;
  SiTRACE("Chip  %d   (%X)\n", chip, chip);
  return chip;
}
/************************************************************************************************************************
  SiLabs_API_Demod_status function
  Use:        demodulator status function
              Used to retrieve the status of the demodulator in a structure
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  status, a pointer to the status structure (configurable in CUSTOM_Status_Struct)
  Return:     1 if successful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_Demod_status             (SILABS_FE_Context* front_end, CUSTOM_Status_Struct *status)
{
  SiTRACE("API CALL STATUS: SiLabs_API_Demod_status (front_end, &status);\n");
  return  SiLabs_API_Demod_status_selection ( front_end, status, 0x00);
}
/************************************************************************************************************************
  SiLabs_API_Demod_status_selection function
  Use:        demodulator status function, with control of the status items to be refreshed
              Used to retrieve the status of the demodulator in a structure
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  status, a pointer to the status structure (configurable in CUSTOM_Status_Struct)
  Parameter:  status_selection, an 8 bit field used to control which items to refresh. Use 0x00 for 'all'.
  Return:     1 if successful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_Demod_status_selection   (SILABS_FE_Context* front_end, CUSTOM_Status_Struct *status, unsigned char status_selection)
{
  SiTRACE("API CALL STATUS: SiLabs_API_Demod_status_control %d/%x status_selection 0x%02x);\n", front_end->chip, front_end->chip, status_selection);

  if (status_selection == 0x00  )    { status_selection = FE_LOCK_STATE + FE_LEVELS + FE_RATES + FE_SPECIFIC + FE_QUALITY + FE_FREQ ;}

  /* Set to 0 all info which will be refreshed. This allows the status to be filled in several steps */
  if (status_selection & FE_LOCK_STATE) {
	status->never_lock         =     0;
    status->pcl_lock           =     0;
    status->dl_lock            =     0;
    status->dl_lock_in_range   =     0;
    status->uncorrs            = 65535; /* Set to max, the value it will take if not locked */
    status->TS_bitrate_kHz     =     0;
    status->TS_clock_kHz       =     0;
    status->s2x                =     0;
  }
  if (status_selection & FE_LEVELS    ) {
    status->RSSI               =  0;
    status->RFagc              =  0;
    status->IFagc              =  0;
  }
  if (status_selection & FE_RATES     ) {
    status->ber_mant           = -1; /* Set to '-1' to signal unavailability if not set later on */
    status->ber_exp            =  0; /* Set to '0'  to signal unavailability if not set later on */
    status->per_mant           = -1; /* Set to '-1' to signal unavailability if not set later on */
    status->per_exp            =  0; /* Set to '0'  to signal unavailability if not set later on */
    status->fer_mant           = -1; /* Set to '-1' to signal unavailability if not set later on */
    status->fer_exp            =  0; /* Set to '0'  to signal unavailability if not set later on */
    status->ber_20000         = 20000;
  }
  if (status_selection & FE_SPECIFIC  ) {
    status->bandwidth_Hz       =  0;
    status->symbol_rate        =  0;
    status->stream             =  0;
    status->constellation      =  0;
    status->c_n_100            =  0;
    status->freq_offset        =  0;
    status->timing_offset      =  0;
    status->code_rate          = -1;
    status->t2_version         =  0;
    status->num_plp            = -1; /* Set to '-1' to allow 'auto' plp mode selection           */
    status->plp_id             =  0;
    status->ds_id              =  0;
    status->cell_id            =  0;
  }
  if (status_selection & FE_QUALITY   ) {
    status->SSI                = -1; /* Set to '-1' to call SiLabs_API_SSI_SQI if not set later on */
    status->SQI                = -1; /* Set to '-1' to call SiLabs_API_SSI_SQI if not set later on */
  }

  if (front_end->standard == SILABS_SLEEP) {status->standard = SILABS_SLEEP; return 0;}

    status->demod_die = front_end->SiDemod_FE->demod->rsp->get_rev.mcm_die;
    /* Mimick Si2167 clock_mode register values */
    switch (front_end->SiDemod_FE->demod->cmd->start_clk.clk_mode) {
      case Si2164B_START_CLK_CMD_CLK_MODE_CLK_CLKIO   : status->clock_mode =  32; break;
      case Si2164B_START_CLK_CMD_CLK_MODE_CLK_XTAL_IN : status->clock_mode =  34; break;
      case Si2164B_START_CLK_CMD_CLK_MODE_XTAL        : status->clock_mode =  33; break;
      default                                        : status->clock_mode =   0; break;
    }
    if (status_selection & FE_LOCK_STATE) { SiTRACE("Checking FE_LOCK_STATE\n");
    if (Si2164B_L1_DD_STATUS (front_end->SiDemod_FE->demod, Si2164B_DD_STATUS_CMD_INTACK_OK) != NO_SiDemod_ERROR) {
      SiERROR("Si2164B_L1_DD_STATUS ERROR\n");
      SiTRACE("Si2164B_L1_DD_STATUS ERROR\n");
      return 0;
    }
    status->standard = Custom_standardCode(front_end, front_end->SiDemod_FE->demod->rsp->dd_status.modulation);
	SiTRACE("DD_STATUS neverlock: %d fef: %d sync_lock: %d Master_lock: %d, current standard: %s\n", status->never_lock, status->fef, front_end->SiDemod_FE->demod->rsp->dd_status.pcl, front_end->SiDemod_FE->demod->rsp->dd_status.dl, Silabs_Standard_Text((CUSTOM_Standard_Enum)status->standard) );
	/* Retrieving TS  values */
    status->TS_bitrate_kHz  = front_end->SiDemod_FE->demod->rsp->dd_status.ts_bit_rate*10;
    status->TS_clock_kHz    = front_end->SiDemod_FE->demod->rsp->dd_status.ts_clk_freq*10;
      status->pcl_lock      = front_end->SiDemod_FE->demod->rsp->dd_status.pcl;
      status->dl_lock        = front_end->SiDemod_FE->demod->rsp->dd_status.dl;
      if (status->dl_lock) {
        if ( Si2164B_L1_DD_UNCOR  (front_end->SiDemod_FE->demod, Si2164B_DD_UNCOR_CMD_RST_RUN) != NO_Si2164B_ERROR ) return 0;
        status->uncorrs            = (front_end->SiDemod_FE->demod->rsp->dd_uncor.uncor_msb<<8) + front_end->SiDemod_FE->demod->rsp->dd_uncor.uncor_lsb;
      }
      /* when unlocked, FE_RATES   will return 1 for all rates, so skip FE_QUALITY if not locked */
      if ((status_selection & FE_RATES    ) && !(status->dl_lock)) {status_selection = status_selection - FE_RATES   ;}
      /* when unlocked, FE_SPECIFIC   will not be valid, so skip FE_QUALITY if not locked */
      if ((status_selection & FE_SPECIFIC ) && !(status->dl_lock)) {status_selection = status_selection - FE_SPECIFIC;}
    } else {
      /* if the standard is not refreshed, use the previous value, which is stored in the context */
      status->standard = Custom_standardCode(front_end, front_end->SiDemod_FE->demod->prop->dd_mode.modulation);
    }
    /* Retrieving AGC values */
    if (status_selection & FE_LEVELS    ) { SiTRACE("Checking FE_LEVELS\n"    );
      switch (status->standard) {
        case SILABS_DVB_T :
        case SILABS_DVB_C :
        case SILABS_MCNS :
        case SILABS_DVB_C2 :
        case SILABS_DVB_T2:
        {
#if 1//original code //20140228 0->1
        front_end->SiDemod_FE->demod->cmd->dd_ext_agc_ter.agc_1_mode  = Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_NO_CHANGE;
        front_end->SiDemod_FE->demod->cmd->dd_ext_agc_ter.agc_2_mode  = Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MODE_NO_CHANGE;
        SiTRACE("Si2164B_L1_SendCommand2(front_end->SiDemod_FE->demod, Si2164B_DD_EXT_AGC_TER_CMD_CODE)\n");
        Si2164B_L1_SendCommand2(front_end->SiDemod_FE->demod, Si2164B_DD_EXT_AGC_TER_CMD_CODE);
#else//Satellite Instant power On fail with strong input signal(20140226)
		Si2164B_L1_DD_EXT_AGC_TER (front_end->SiDemod_FE->demod,
		Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_NO_CHANGE,
		front_end->SiDemod_FE->demod->cmd->dd_ext_agc_ter.agc_1_inv,
		Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MODE_NO_CHANGE,
		front_end->SiDemod_FE->demod->cmd->dd_ext_agc_ter.agc_2_inv,
		front_end->SiDemod_FE->demod->cmd->dd_ext_agc_ter.agc_1_kloop,
		front_end->SiDemod_FE->demod->cmd->dd_ext_agc_ter.agc_2_kloop,
		front_end->SiDemod_FE->demod->cmd->dd_ext_agc_ter.agc_1_min,
		front_end->SiDemod_FE->demod->cmd->dd_ext_agc_ter.agc_2_min );
#endif
        status->RFagc              = 0;
          if (front_end->SiDemod_FE->demod->cmd->dd_ext_agc_ter.agc_1_mode  != Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_NOT_USED) {
            status->IFagc              = front_end->SiDemod_FE->demod->rsp->dd_ext_agc_ter.agc_1_level;
          } else {
            status->IFagc              = front_end->SiDemod_FE->demod->rsp->dd_ext_agc_ter.agc_2_level;
          }
        break;
      }
      case SILABS_DVB_S :
      case SILABS_DVB_S2:
      case SILABS_DSS   :
      {
		Si2164B_L1_DD_EXT_AGC_SAT (front_end->SiDemod_FE->demod,
		Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MODE_NO_CHANGE,
		front_end->SiDemod_FE->demod->cmd->dd_ext_agc_sat.agc_1_inv,
		Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MODE_NO_CHANGE,
		front_end->SiDemod_FE->demod->cmd->dd_ext_agc_sat.agc_2_inv,
		front_end->SiDemod_FE->demod->cmd->dd_ext_agc_sat.agc_1_kloop,
		front_end->SiDemod_FE->demod->cmd->dd_ext_agc_sat.agc_2_kloop,
		front_end->SiDemod_FE->demod->cmd->dd_ext_agc_sat.agc_1_min,
		front_end->SiDemod_FE->demod->cmd->dd_ext_agc_sat.agc_2_min );
          //status->RFagc              = 0;
          //if (front_end->SiDemod_FE->demod->cmd->dd_ext_agc_sat.agc_1_mode  != Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MODE_NOT_USED) {
          //  status->IFagc            = front_end->SiDemod_FE->demod->rsp->dd_ext_agc_sat.agc_1_level;
          //} else {
          //  status->IFagc            = front_end->SiDemod_FE->demod->rsp->dd_ext_agc_sat.agc_2_level;
          //}
		status->RFagc              = front_end->SiDemod_FE->demod->rsp->dd_ext_agc_sat.agc_1_level;
        status->IFagc              = front_end->SiDemod_FE->demod->rsp->dd_ext_agc_sat.agc_2_level;
  #ifndef   NO_SAT
          #ifndef   SAT_TUNER_RSSI_FROM_IFAGC
          status->RSSI               = status->rssi*1; /* dBm conversion of rssi */
          #else  /* SAT_TUNER_RSSI_FROM_IFAGC */
          SiLabs_API_SAT_Tuner_I2C_Enable  (front_end);
          status->RSSI               = SAT_TUNER_RSSI_FROM_IFAGC(front_end->SiDemod_FE->tuner_sat, status->IFagc);
          SiLabs_API_SAT_Tuner_I2C_Disable (front_end);
          #endif /* SAT_TUNER_RSSI_FROM_IFAGC */
  #endif /* NO_SAT */
          break;
        }
        default           : {
          status->RFagc              = 0;
          status->IFagc              = 0;
          break;
        }
      }
     }
    /* Retrieving BER/PER/FER values */
    if (status_selection & FE_RATES     ) { SiTRACE("Checking FE_RATES\n"     );
    status->ber_20000 = 20000;
      if (front_end->SiDemod_FE->demod->rsp->dd_status.dl == 1) { /* Only check rates when locked */
			switch (status->standard) { /* retrieving BER, PER, FER depending on the standard */
			  case SILABS_ANALOG: {
				return 1;
            break;
          }
		  case SILABS_DVB_T :
		  case SILABS_DVB_C :
		  case SILABS_MCNS :
		  case SILABS_DVB_C2:
		  case SILABS_DVB_S :
		  case SILABS_DSS   :
		  {
			if ( Si2164B_L1_DD_BER    (front_end->SiDemod_FE->demod, Si2164B_DD_BER_CMD_RST_RUN  ) != NO_SiDemod_ERROR ) return 0;

          /* CHECK the exponent value to know if the BER is available or not */
          if (front_end->SiDemod_FE->demod->rsp->dd_ber.exp!=0) {
            status->ber_mant            = (front_end->SiDemod_FE->demod->rsp->dd_ber.mant );
            status->ber_exp             = (front_end->SiDemod_FE->demod->rsp->dd_ber.exp+1);
          /* The following tests my not be required, since the API should never return out of ramge values when dd_bner.exp != -1 */
            if      ( status->ber_mant <  0 ) status->ber_mant =  0;
            else if ( status->ber_mant > 99 ) status->ber_mant = 99;
            if      ( status->ber_exp  <  0 ) status->ber_mant =  0;
            else if ( status->ber_exp  >  8 ) status->ber_mant =  8;

            status->ber_e8  =       status->ber_mant    * power_of_n(10, (8-status->ber_exp) ); /* hjh_change1 */
			}
			if ( Si2164B_L1_DD_PER    (front_end->SiDemod_FE->demod, Si2164B_DD_PER_CMD_RST_RUN  ) != NO_SiDemod_ERROR ) return 0;
			/* CHECK the exponent value to know if the PER is available or not */
			if (front_end->SiDemod_FE->demod->rsp->dd_per.exp!=0) {
					  status->per_mant           = (front_end->SiDemod_FE->demod->rsp->dd_per.mant);
					  status->per_exp            = (front_end->SiDemod_FE->demod->rsp->dd_per.exp+1);
			}
			break;
		  }
		  case SILABS_DVB_S2:
		  case SILABS_DVB_T2:
		   {
			if ( Si2164B_L1_DD_BER    (front_end->SiDemod_FE->demod, Si2164B_DD_BER_CMD_RST_RUN  ) != NO_SiDemod_ERROR ) return 0;
			/* CHECK the exponent value to know if the BER is available or not*/
			if(front_end->SiDemod_FE->demod->rsp->dd_ber.exp!=0) {
				  status->ber_mant           = (front_end->SiDemod_FE->demod->rsp->dd_ber.mant);
				  status->ber_exp            = (front_end->SiDemod_FE->demod->rsp->dd_ber.exp+1);
			}
			if ( Si2164B_L1_DD_FER    (front_end->SiDemod_FE->demod, Si2164B_DD_FER_CMD_RST_RUN  ) != NO_SiDemod_ERROR ) return 0;
			/* CHECK the exponent value to know if the FER is available or not*/
			if(front_end->SiDemod_FE->demod->rsp->dd_fer.exp!=0) {
				  status->fer_mant           = (front_end->SiDemod_FE->demod->rsp->dd_fer.mant);
				  status->fer_exp            = (front_end->SiDemod_FE->demod->rsp->dd_fer.exp+1);
			}
			if ( Si2164B_L1_DD_PER    (front_end->SiDemod_FE->demod, Si2164B_DD_PER_CMD_RST_RUN  ) != NO_SiDemod_ERROR ) return 0;
			/* CHECK the exponent value to know if the PER is available or not*/
			if(front_end->SiDemod_FE->demod->rsp->dd_per.exp!=0) {
				  status->per_mant           = (front_end->SiDemod_FE->demod->rsp->dd_per.mant);
				  status->per_exp            = (front_end->SiDemod_FE->demod->rsp->dd_per.exp+1);
			}
            break;
          }
          default           : { return 0; break; }
        }
        switch (status->standard) { /* setting  ber_200000      depending on the standard */
          case SILABS_DVB_T :
          {
          /* DVB-T ber_2000: status->ber*10e6                          */
          /* ber_2000 management: ber_2000 = ber*10e6, limited to 2000 */
          status->ber_20000 = (  1*status->ber_e8  +  5 )/10;
          break;
        }
        case SILABS_DVB_C :
        {
          /* DVB-C ber_2000: status->ber*(2/7)*1e7  */
          status->ber_20000 = (  2*status->ber_e8  +  3 )/ 7;
          break;
        }
        case SILABS_DVB_S :
        case SILABS_DSS   :
        {
          status->ber_20000 = (  5*status->ber_e8  + 20 )/40;
          break;
        }
        default           : { break; }
      }
      /* limiting ber_20000 to 20000 */
      if((status->ber_20000>=20000)||(status->ber_20000<0)) { status->ber_20000 = 20000; }
    }
    status->ber_2000 = status->ber_20000/10;
    }
    /* Retrieving standard specific values */
    if (status_selection & FE_SPECIFIC  ) { SiTRACE("Checking FE_SPECIFIC\n"  );
    switch (status->standard) {
      case SILABS_DVB_T : {
        if (Si2164B_L1_DVBT_STATUS    (front_end->SiDemod_FE->demod, Si2164B_DVBT_STATUS_CMD_INTACK_OK)                        != NO_SiDemod_ERROR) return 0;
        if (Si2164B_L1_DVBT_TPS_EXTRA (front_end->SiDemod_FE->demod)                                                          != NO_SiDemod_ERROR) return 0;
#ifdef    TUNERTER_API
		if((front_end->SiDemod_FE->tuner_ter->i2c->address == TER1_MAIN_ADDRESS) || (front_end->SiDemod_FE->tuner_ter->i2c->address == TER1_SUB_ADDRESS)) // for Dual T2CS2 & Dual T2C model 
			status->RSSI               = front_end->SiDemod_FE->tuner_ter->rsp->tuner_status.rssi - 6;
		else   // for Single T2CS2 & Single T2C & Single T2C(Si2168C) model
			status->RSSI               = front_end->SiDemod_FE->tuner_ter->rsp->tuner_status.rssi;
#else  /* TUNERTER_API */
        status->RSSI               = front_end->SiDemod_FE->tuner_ter->rssi;
#endif /* TUNERTER_API */
        if (Si2164B_L1_DD_SSI_SQI     (front_end->SiDemod_FE->demod, status->RSSI) != NO_SiDemod_ERROR) return 0;
        status->never_lock         = SiDemod_Check_NeverLock(front_end->SiDemod_FE->demod);//csm,13'
        status->pcl_lock         = front_end->SiDemod_FE->demod->rsp->dvbt_status.pcl;
        status->dl_lock           = front_end->SiDemod_FE->demod->rsp->dvbt_status.dl;
        status->spectral_inversion = front_end->SiDemod_FE->demod->rsp->dvbt_status.sp_inv;
        status->c_n_100            = front_end->SiDemod_FE->demod->rsp->dvbt_status.cnr*25;
        status->freq_offset        = front_end->SiDemod_FE->demod->rsp->dvbt_status.afc_freq;
        status->timing_offset      = front_end->SiDemod_FE->demod->rsp->dvbt_status.timing_offset;
        status->bandwidth_Hz       = front_end->SiDemod_FE->demod->prop->dd_mode.bw*1000000;
        status->stream             = Custom_streamCode   (front_end, front_end->SiDemod_FE->demod->prop->dvbt_hierarchy.stream);
        status->fft_mode           = Custom_fftCode      (front_end, front_end->SiDemod_FE->demod->rsp->dvbt_status.fft_mode);
        status->guard_interval     = Custom_giCode       (front_end, front_end->SiDemod_FE->demod->rsp->dvbt_status.guard_int);
        status->constellation      = Custom_constelCode  (front_end, front_end->SiDemod_FE->demod->rsp->dvbt_status.constellation);
        status->hierarchy          = Custom_hierarchyCode(front_end, front_end->SiDemod_FE->demod->rsp->dvbt_status.hierarchy);
        status->code_rate_hp       = Custom_coderateCode (front_end, front_end->SiDemod_FE->demod->rsp->dvbt_status.rate_hp);
        status->code_rate_lp       = Custom_coderateCode (front_end, front_end->SiDemod_FE->demod->rsp->dvbt_status.rate_lp);
        status->symbol_rate        = 0;
        status->SSI                = front_end->SiDemod_FE->demod->rsp->dd_ssi_sqi.ssi;
        status->SQI                = front_end->SiDemod_FE->demod->rsp->dd_ssi_sqi.sqi;
        status->cell_id            = front_end->SiDemod_FE->demod->rsp->dvbt_tps_extra.cell_id;
        break;
      }
      case SILABS_DVB_T2 : {
        if (Si2164B_L1_DVBT2_STATUS   (front_end->SiDemod_FE->demod, Si2164B_DVBT2_STATUS_CMD_INTACK_OK)                       != NO_SiDemod_ERROR) return 0;
        status->never_lock         = SiDemod_Check_NeverLock(front_end->SiDemod_FE->demod);
		status->pcl_lock         = front_end->SiDemod_FE->demod->rsp->dvbt2_status.pcl;
        status->dl_lock           = front_end->SiDemod_FE->demod->rsp->dvbt2_status.dl;
        status->spectral_inversion = front_end->SiDemod_FE->demod->rsp->dvbt2_status.sp_inv;
        status->c_n_100            = front_end->SiDemod_FE->demod->rsp->dvbt2_status.cnr*25;
        status->freq_offset        = front_end->SiDemod_FE->demod->rsp->dvbt2_status.afc_freq;
        status->timing_offset      = front_end->SiDemod_FE->demod->rsp->dvbt2_status.timing_offset;
        status->bandwidth_Hz       = front_end->SiDemod_FE->demod->prop->dd_mode.bw*1000000;
        status->stream             = Custom_streamCode      (front_end, 0);
        status->fft_mode           = Custom_fftCode         (front_end, front_end->SiDemod_FE->demod->rsp->dvbt2_status.fft_mode);
        status->guard_interval     = Custom_giCode          (front_end, front_end->SiDemod_FE->demod->rsp->dvbt2_status.guard_int);
        status->constellation      = Custom_constelCode     (front_end, front_end->SiDemod_FE->demod->rsp->dvbt2_status.constellation);
        status->code_rate          = Custom_coderateCode    (front_end, front_end->SiDemod_FE->demod->rsp->dvbt2_status.code_rate);
        status->num_plp            = front_end->SiDemod_FE->demod->rsp->dvbt2_status.num_plp;
        status->rotated            = front_end->SiDemod_FE->demod->rsp->dvbt2_status.rotated;
        status->pilot_pattern      = Custom_pilotPatternCode(front_end, front_end->SiDemod_FE->demod->rsp->dvbt2_status.pilot_pattern);
        status->bw_ext             = front_end->SiDemod_FE->demod->rsp->dvbt2_status.bw_ext;
        status->plp_id             = front_end->SiDemod_FE->demod->rsp->dvbt2_status.plp_id;
        status->tx_mode            = front_end->SiDemod_FE->demod->rsp->dvbt2_status.tx_mode;
        status->short_frame        = front_end->SiDemod_FE->demod->rsp->dvbt2_status.short_frame;
        status->fef                = front_end->SiDemod_FE->demod->rsp->dvbt2_status.fef;
        status->t2_base_lite       = front_end->SiDemod_FE->demod->rsp->dvbt2_status.t2_mode;
        status->t2_version         = Custom_T2VersionCode   (front_end, front_end->SiDemod_FE->demod->rsp->dvbt2_status.t2_version);
        if (Si2164B_L1_DVBT2_TX_ID   (front_end->SiDemod_FE->demod)                       != NO_SiDemod_ERROR) return 0;
        status->cell_id            = front_end->SiDemod_FE->demod->rsp->dvbt2_tx_id.cell_id;
        status->t2_system_id       = front_end->SiDemod_FE->demod->rsp->dvbt2_tx_id.t2_system_id;
#ifdef    TUNERTER_API
		if((front_end->SiDemod_FE->tuner_ter->i2c->address == TER1_MAIN_ADDRESS) || (front_end->SiDemod_FE->tuner_ter->i2c->address == TER1_SUB_ADDRESS)) // for Dual T2CS2 & Dual T2C model 
			status->RSSI               = front_end->SiDemod_FE->tuner_ter->rsp->tuner_status.rssi - 6;
		else   // for Single T2CS2 & Single T2C & Single T2C(Si2168C) model
			status->RSSI               = front_end->SiDemod_FE->tuner_ter->rsp->tuner_status.rssi;
#else  /* TUNERTER_API */
        status->RSSI               = front_end->SiDemod_FE->tuner_ter->rssi;
#endif /* TUNERTER_API */
        if (Si2164B_L1_DD_SSI_SQI     (front_end->SiDemod_FE->demod, status->RSSI) != NO_SiDemod_ERROR) return 0;
        status->SSI                = front_end->SiDemod_FE->demod->rsp->dd_ssi_sqi.ssi;
        //status->SQI                = front_end->SiDemod_FE->demod->rsp->dd_ssi_sqi.sqi;//csm,disable
		//csm,13' dual, kwon, 14 quad, c2
		if(front_end->SiDemod_FE->demod->rsp->dd_ssi_sqi.sqi <= 0)	{ status->SQI = 0; }
		else                       { status->SQI = front_end->SiDemod_FE->demod->rsp->dd_ssi_sqi.sqi;}
		if((100 - status->SQI)*2000/85 >= 2000)	{ status->per_2000 = 2000; }
		else                       { status->per_2000 = (100 - status->SQI)*2000/85;}
		//end
        status->symbol_rate        = 0;

		#ifdef SEM_DEBUG
		//Si2164B_L1_DD_GET_REG (front_end->SiDemod_FE->demod, 231, 81, 12);
		Si2164B_L1_DD_GET_REG (front_end->SiDemod_FE->demod, 231, 61, 13); // 20161117 mod for x55, x60 demod
		if ( (front_end->SiDemod_FE->demod->rsp->dd_get_reg.data4<<24)
			+(front_end->SiDemod_FE->demod->rsp->dd_get_reg.data3<<16)
			+(front_end->SiDemod_FE->demod->rsp->dd_get_reg.data2<<8 )
			+(front_end->SiDemod_FE->demod->rsp->dd_get_reg.data1<<0 ) ==1 ) 
		{
			SiTRACE("L1_POST is scrambled\n");
			status->t2_scrambled = 1;
		}
		else
		{
			SiTRACE("L1_POST is non-scrambled\n");
			status->t2_scrambled = 0;
		}
		#endif

        break;
      }
      case SILABS_DVB_C : {
        if (Si2164B_L1_DVBC_STATUS    (front_end->SiDemod_FE->demod, Si2164B_DVBC_STATUS_CMD_INTACK_OK) != NO_SiDemod_ERROR) return 0;
        status->pcl_lock         = front_end->SiDemod_FE->demod->rsp->dvbc_status.pcl;
        status->dl_lock           = front_end->SiDemod_FE->demod->rsp->dvbc_status.dl;
        status->symbol_rate        = front_end->SiDemod_FE->demod->prop->dvbc_symbol_rate.rate*1000;
		status->dl_lock_in_range  = SiDemod_L1_DVB_C_get_dl_lock_in_range(front_end->SiDemod_FE->demod, status->symbol_rate/1000);//kwon
        status->constellation      = Custom_constelCode (front_end, front_end->SiDemod_FE->demod->rsp->dvbc_status.constellation);
        status->spectral_inversion = front_end->SiDemod_FE->demod->rsp->dvbc_status.sp_inv;
        status->c_n_100            = front_end->SiDemod_FE->demod->rsp->dvbc_status.cnr*25;
        status->freq_offset        = front_end->SiDemod_FE->demod->rsp->dvbc_status.afc_freq;
        status->timing_offset      = front_end->SiDemod_FE->demod->rsp->dvbc_status.timing_offset;

		if((front_end->SiDemod_FE->tuner_ter->i2c->address == TER1_MAIN_ADDRESS) || (front_end->SiDemod_FE->tuner_ter->i2c->address == TER1_SUB_ADDRESS)) // for Dual T2CS2 & Dual T2C model 
			status->RSSI               = front_end->SiDemod_FE->tuner_ter->rsp->tuner_status.rssi - 9;
		else   // for Single T2CS2 & Single T2C & Single T2C(Si2168C) model
			status->RSSI               = front_end->SiDemod_FE->tuner_ter->rsp->tuner_status.rssi;

		status->SSI                = SiDemod_DVB_C_SSI_for_Received_Power    (front_end->SiDemod_FE->demod, status->RSSI);

		if(status->dl_lock_in_range == 0) { status->SQI = 0;}
		else                               { status->SQI= SiDemod_DVB_C_SQI(front_end->SiDemod_FE->demod);}
        //end
		break;
      }
	  case SILABS_MCNS  : {
        if (Si2164B_L1_MCNS_STATUS   (front_end->SiDemod_FE->demod, Si2164B_MCNS_STATUS_CMD_INTACK_OK) != NO_Si2164B_ERROR) return 0;
        status->pcl_lock         = front_end->SiDemod_FE->demod->rsp->mcns_status.pcl;
        status->dl_lock           = front_end->SiDemod_FE->demod->rsp->mcns_status.dl;
        status->symbol_rate        = front_end->SiDemod_FE->demod->prop->mcns_symbol_rate.rate*1000;
        status->constellation      = Custom_constelCode (front_end, front_end->SiDemod_FE->demod->rsp->mcns_status.constellation);
        status->spectral_inversion = front_end->SiDemod_FE->demod->rsp->mcns_status.sp_inv;
        status->c_n_100            = front_end->SiDemod_FE->demod->rsp->mcns_status.cnr*25;
        status->freq_offset        = front_end->SiDemod_FE->demod->rsp->mcns_status.afc_freq;
        status->timing_offset      = front_end->SiDemod_FE->demod->rsp->mcns_status.timing_offset;
#ifdef    TUNERTER_API
        status->RSSI               = front_end->SiDemod_FE->tuner_ter->rsp->tuner_status.rssi;
#else  /* TUNERTER_API */
        status->RSSI               = front_end->SiDemod_FE->tuner_ter->rssi;
#endif /* TUNERTER_API */
        break;
      }
      case SILABS_DVB_C2 : {
        if (Si2164B_L1_DVBC2_STATUS   (front_end->SiDemod_FE->demod, Si2164B_DVBC2_STATUS_CMD_INTACK_OK)                       != NO_SiDemod_ERROR) return 0;
        if (Si2164B_L1_DVBC2_DS_INFO  (front_end->SiDemod_FE->demod, front_end->SiDemod_FE->demod->rsp->dvbc2_status.ds_id, Si2164B_DVBC2_DS_INFO_CMD_DS_SELECT_INDEX_OR_ID_ID)    != NO_SiDemod_ERROR) return 0;
        status->pcl_lock         = front_end->SiDemod_FE->demod->rsp->dvbc2_status.pcl;
        status->dl_lock           = front_end->SiDemod_FE->demod->rsp->dvbc2_status.dl;
        status->spectral_inversion = front_end->SiDemod_FE->demod->rsp->dvbc2_status.sp_inv;
        status->c_n_100            = front_end->SiDemod_FE->demod->rsp->dvbc2_status.cnr*25;
        status->freq_offset        = front_end->SiDemod_FE->demod->rsp->dvbc2_status.afc_freq;
        status->timing_offset      = front_end->SiDemod_FE->demod->rsp->dvbc2_status.timing_offset;
        status->bandwidth_Hz       = front_end->SiDemod_FE->demod->prop->dd_mode.bw*1000000;
        status->guard_interval     = Custom_giCode          (front_end, front_end->SiDemod_FE->demod->rsp->dvbc2_status.guard_int);
        status->constellation      = Custom_constelCode     (front_end, front_end->SiDemod_FE->demod->rsp->dvbc2_status.constellation);
        status->code_rate          = Custom_coderateCode    (front_end, front_end->SiDemod_FE->demod->rsp->dvbc2_status.code_rate);
        status->num_plp            = front_end->SiDemod_FE->demod->rsp->dvbc2_ds_info.dslice_num_plp;
        status->plp_id             = front_end->SiDemod_FE->demod->rsp->dvbc2_status.plp_id;
        status->ds_id              = front_end->SiDemod_FE->demod->rsp->dvbc2_status.ds_id;
#ifdef    TUNERTER_API
        status->RSSI               = front_end->SiDemod_FE->tuner_ter->rsp->tuner_status.rssi;
#else  /* TUNERTER_API */
        status->RSSI               = front_end->SiDemod_FE->tuner_ter->rssi;
#endif /* TUNERTER_API */
        //if (Si2164B_L1_DD_SSI_SQI     (front_end->SiDemod_FE->demod, status->RSSI) != NO_SiDemod_ERROR) return 0; // Si2164B_L1_DD_SSI_SQI() function is available for Si2164B ROM1. KWON
        //status->SSI                = front_end->SiDemod_FE->demod->rsp->dd_ssi_sqi.ssi;
        //status->SQI                = front_end->SiDemod_FE->demod->rsp->dd_ssi_sqi.sqi;
		if (Si2164B_L1_DD_SSI_SQI     (front_end->SiDemod_FE->demod, status->RSSI) != NO_SiDemod_ERROR) {
			SiTRACE("Si2164B_L1_DD_SSI_SQI ERROR\n");//add,csm
        } else {
          status->SSI                = front_end->SiDemod_FE->demod->rsp->dd_ssi_sqi.ssi;
          status->SQI                = front_end->SiDemod_FE->demod->rsp->dd_ssi_sqi.sqi;
        }
        status->symbol_rate        = 0;
        break;
      }	  
      case SILABS_DVB_S2: {
        #ifndef   SAT_TUNER_RSSI_FROM_IFAGC
        status->RSSI               = (signed int)status->rssi; /* dBm conversion of rssi */
        #else  /* SAT_TUNER_RSSI_FROM_IFAGC */
        SiLabs_API_Tuner_I2C_Enable  (front_end);
        status->RSSI               = SAT_TUNER_RSSI_FROM_IFAGC(front_end->SiDemod_FE->tuner_sat, status->IFagc);
        SiLabs_API_Tuner_I2C_Disable (front_end);
        #endif /* SAT_TUNER_RSSI_FROM_IFAGC */
        if (Si2164B_L1_DVBS2_STATUS    (front_end->SiDemod_FE->demod, Si2164B_DVBS2_STATUS_CMD_INTACK_OK) != NO_SiDemod_ERROR) return 0;
        status->pcl_lock         = front_end->SiDemod_FE->demod->rsp->dvbs2_status.pcl;
        //status->dl_lock           = front_end->SiDemod_FE->demod->rsp->dvbs2_status.dl;//kwon,14,disable
		status->dl_lock           = SiDemod_L1_DVB_S_S2_get_lock_in_AFC_range(front_end->SiDemod_FE->demod);//kwon
        status->symbol_rate        = front_end->SiDemod_FE->demod->prop->dvbs2_symbol_rate.rate*1000;
        status->constellation      = Custom_constelCode  (front_end, front_end->SiDemod_FE->demod->rsp->dvbs2_status.constellation);
        status->spectral_inversion = front_end->SiDemod_FE->demod->rsp->dvbs2_status.sp_inv;
        status->c_n_100            = front_end->SiDemod_FE->demod->rsp->dvbs2_status.cnr*25;
        status->freq_offset        = front_end->SiDemod_FE->demod->rsp->dvbs2_status.afc_freq;
        status->timing_offset      = front_end->SiDemod_FE->demod->rsp->dvbs2_status.timing_offset;
        status->code_rate          = Custom_coderateCode (front_end, front_end->SiDemod_FE->demod->rsp->dvbs2_status.code_rate);
        status->pilots             = front_end->SiDemod_FE->demod->rsp->dvbs2_status.pilots;
        status->num_is             = front_end->SiDemod_FE->demod->rsp->dvbs2_status.num_is;
        status->isi_id             = front_end->SiDemod_FE->demod->rsp->dvbs2_status.isi_id;
        if (Si2164B_L1_DD_FER          (front_end->SiDemod_FE->demod, Si2164B_DD_FER_CMD_RST_RUN)         != NO_SiDemod_ERROR) return 0;
#ifndef   NO_SAT
		if(status->RFagc == Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MIN_AGC_1_MIN_MAX) { status->SatCRAGC = status->IFagc; }
		else                                                                   { status->SatCRAGC = status->RFagc; }
#endif /* NO_SAT */
        status->fer_mant         = (front_end->SiDemod_FE->demod->rsp->dd_fer.mant);
        status->fer_exp          = (front_end->SiDemod_FE->demod->rsp->dd_fer.exp+1);
		status->per_2000           = SiDemod_DVB_S2_PER (front_end->SiDemod_FE->demod);

		if((front_end->SiDemod_FE->tuner_ter->i2c->address == TER1_MAIN_ADDRESS) || (front_end->SiDemod_FE->tuner_ter->i2c->address == TER1_SUB_ADDRESS)) // for Dual T2CS2 model 
			status->SSI = SiDemod_DVB_SAT_SSI_T2S2_DUAL (front_end->SiDemod_FE->demod, status->RSSI);//SiDemod_DVB_SAT_SSI_T2S2_DUAL (front_end->SiDemod_FE->demod, status->freq);
		else   // for Single T2CS2 model
			status->SSI = SiDemod_DVB_SAT_SSI_T2CS2_SINGLE (front_end->SiDemod_FE->demod, status->RSSI);//SiDemod_DVB_SAT_SSI_T2CS2_SINGLE (front_end->SiDemod_FE->demod, status->freq);

        status->SQI                = SiDemod_DVB_S2_SQI (front_end->SiDemod_FE->demod);
        break;
      }
      case SILABS_DVB_S :
      case SILABS_DSS   : {
        #ifndef   SAT_TUNER_RSSI_FROM_IFAGC
        status->RSSI               = (signed int)status->rssi; /* dBm conversion of rssi */
        #else  /* SAT_TUNER_RSSI_FROM_IFAGC */
        SiLabs_API_Tuner_I2C_Enable  (front_end);
        status->RSSI               = SAT_TUNER_RSSI_FROM_IFAGC(front_end->SiDemod_FE->tuner_sat, status->IFagc);
        SiLabs_API_Tuner_I2C_Disable (front_end);
        #endif /* SAT_TUNER_RSSI_FROM_IFAGC */
        if (Si2164B_L1_DVBS_STATUS     (front_end->SiDemod_FE->demod, Si2164B_DVBS_STATUS_CMD_INTACK_OK) != NO_SiDemod_ERROR) return 0;
        /* Settings for SAT AGC. These settings need to match the HW design for SAT AGCs */
        status->pcl_lock         = front_end->SiDemod_FE->demod->rsp->dvbs_status.pcl;
        //status->dl_lock           = front_end->SiDemod_FE->demod->rsp->dvbs_status.dl;//kwon,disable
		status->dl_lock           = SiDemod_L1_DVB_S_S2_get_lock_in_AFC_range(front_end->SiDemod_FE->demod);//kwon
        status->symbol_rate        = front_end->SiDemod_FE->demod->prop->dvbs_symbol_rate.rate*1000;
        status->constellation      = Custom_constelCode  (front_end, front_end->SiDemod_FE->demod->rsp->dvbs_status.constellation);
        status->spectral_inversion = front_end->SiDemod_FE->demod->rsp->dvbs_status.sp_inv;
        status->c_n_100            = front_end->SiDemod_FE->demod->rsp->dvbs_status.cnr*25;
        status->freq_offset        = front_end->SiDemod_FE->demod->rsp->dvbs_status.afc_freq;
        status->timing_offset      = front_end->SiDemod_FE->demod->rsp->dvbs_status.timing_offset;
        status->code_rate          = Custom_coderateCode (front_end, front_end->SiDemod_FE->demod->rsp->dvbs_status.code_rate);

#ifndef   NO_SAT
		if(status->RFagc == Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MIN_AGC_1_MIN_MAX) { status->SatCRAGC = status->IFagc; }
		else                                                                   { status->SatCRAGC = status->RFagc; }
#endif /* NO_SAT */


		if((front_end->SiDemod_FE->tuner_ter->i2c->address == TER1_MAIN_ADDRESS) || (front_end->SiDemod_FE->tuner_ter->i2c->address == TER1_SUB_ADDRESS)) // for Dual T2CS2 model 
			status->SSI = SiDemod_DVB_SAT_SSI_T2S2_DUAL (front_end->SiDemod_FE->demod, status->RSSI);//SiDemod_DVB_SAT_SSI_T2S2_DUAL (front_end->SiDemod_FE->demod, status->freq);
		else   // for Single T2CS2 model
			status->SSI = SiDemod_DVB_SAT_SSI_T2CS2_SINGLE (front_end->SiDemod_FE->demod, status->RSSI);//SiDemod_DVB_SAT_SSI_T2CS2_SINGLE (front_end->SiDemod_FE->demod, status->freq);

        status->SQI                = SiDemod_DVB_S_SQI (front_end->SiDemod_FE->demod);

        break;
      }
      default           : {
        return 0;
        break;
      }
    }
  }

  if (status->standard == SILABS_DVB_S2) {
    switch (status->constellation) {
      case SILABS_QPSK:     {
        switch (status->code_rate) {
          case SILABS_CODERATE_1_4:
          case SILABS_CODERATE_1_3:
          case SILABS_CODERATE_2_5:
          case SILABS_CODERATE_1_2:
          case SILABS_CODERATE_3_5:
          case SILABS_CODERATE_2_3:
          case SILABS_CODERATE_3_4:
          case SILABS_CODERATE_4_5:
          case SILABS_CODERATE_5_6:
          case SILABS_CODERATE_8_9:
          case SILABS_CODERATE_9_10: {
            status->s2x = 1;
            break;
          }
          default: {
            status->s2x = 0;
            break;
          }
        }
        break;
      }
      case SILABS_8PSK:     {
        switch (status->code_rate) {
          case SILABS_CODERATE_3_5:
          case SILABS_CODERATE_2_3:
          case SILABS_CODERATE_3_4:
          case SILABS_CODERATE_5_6:
          case SILABS_CODERATE_8_9:
          case SILABS_CODERATE_9_10: {
            status->s2x = 1;
            break;
          }
          default: {
            status->s2x = 0;
            break;
          }
        }
        break;
      }
      case SILABS_16APSK:   {
        switch (status->code_rate) {
          case SILABS_CODERATE_2_3:
          case SILABS_CODERATE_3_4:
          case SILABS_CODERATE_4_5:
          case SILABS_CODERATE_5_6:
          case SILABS_CODERATE_8_9:
          case SILABS_CODERATE_9_10: {
            status->s2x = 1;
            break;
          }
          default: {
            status->s2x = 0;
            break;
          }
        }
        break;
      }
      case SILABS_32APSK_1: {
        switch (status->code_rate) {
          case SILABS_CODERATE_3_4:
          case SILABS_CODERATE_4_5:
          case SILABS_CODERATE_5_6:
          case SILABS_CODERATE_8_9:
          case SILABS_CODERATE_9_10: {
            status->s2x = 1;
            break;
          }
          default: {
            status->s2x = 0;
            break;
          }
        }
        break;
      }
      default: {
        status->s2x = 0;
        break;
      }
    }
  }
  if(status->dl_lock == 1) {front_end->standard = status->standard;}

  return 1;
}
/************************************************************************************************************************
  SiLabs_API_TER_Tuner_status function
  Use:        Terrestrial tuner status function
              Used to retrieve the status of the TER tuner in a structure
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  status, a pointer to the status structure (configurable in CUSTOM_Status_Struct)
  Return:     1 if successful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_TER_Tuner_status         (SILABS_FE_Context* front_end, CUSTOM_Status_Struct *status)
{
  SILABS_TER_TUNER_Context *tuner_ter;
  SiTRACE("SiLabs_API_TER_Tuner_status\n");
  tuner_ter = front_end->SiDemod_FE->tuner_ter;
  if (tuner_ter == NULL) {
      SiTRACE("No tuner_ter defined, SiLabs_API_TER_Tuner_status can't be executed!\n");
    return 0;
  }
  SiLabs_API_Tuner_I2C_Enable  (front_end);
  Si2151_L1_TUNER_STATUS(tuner_ter);
  status->vco_code =  tuner_ter->rsp->tuner_status.vco_code;
  status->rssi     =  tuner_ter->rsp->tuner_status.rssi;
  status->freq     =  tuner_ter->rsp->tuner_status.freq;
  status->mode     =  tuner_ter->rsp->tuner_status.mode;

  SiLabs_API_Tuner_I2C_Disable (front_end);
  return 1;
}
/************************************************************************************************************************
  SiLabs_API_SAT_Tuner_status function
  Use:        Satellite tuner status function
              Used to retrieve the status of the SAT tuner in a structure
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  status, a pointer to the status structure (configurable in CUSTOM_Status_Struct)
  Return:     1 if successful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_SAT_Tuner_status         (SILABS_FE_Context* front_end, CUSTOM_Status_Struct *status)
{
#if(!SEC_T2_TO_CHECK_02)
  signed int ref_level=0;
  SAT_TUNER_CONTEXT *sat_tuner;//CSM//RDA5816S_Context *sat_tuner;
  front_end = front_end; /* To avoid compiler warning */
  status    = status;    /* To avoid compiler warning */
  ref_level = 0;

  SiLabs_API_Tuner_I2C_Enable  (front_end);
  SiTRACE("SiLabs_API_SAT_Tuner_status\n");
  
  sat_tuner = front_end->SiDemod_FE->tuner_sat; ref_level = 0;
  status->freq = sat_tuner->RF;

  /* TBD: Add here the call to the SAT tuner status function */
  status->rssi = 0;
  status->rssi = SAT_TUNER_RSSI(sat_tuner);
  SiLabs_API_Tuner_I2C_Disable (front_end);
#else
  front_end=front_end;
  status=status;
#endif
  return 1;
}
/************************************************************************************************************************
  SiLabs_API_FE_status function
  Use:        Front-End status function
              Used to retrieve the complete status of the front-end in a structure
  Behavior:   It will call SiLabs_API_FE_status_selection with the status_selection flag corresponding to 'all'
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  status, a pointer to the status structure (configurable in CUSTOM_Status_Struct)
  Return:     1 if successful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_FE_status                (SILABS_FE_Context* front_end, CUSTOM_Status_Struct *status)
{
  SiTRACE("API CALL STATUS: SiLabs_API_FE_status (front_end, &status);\n");
  return SiLabs_API_FE_status_selection (front_end, status, 0x00);
}
/************************************************************************************************************************
  SiLabs_API_FE_status_selection function
  Use:        Front-End status function, with selection of 'all' or partial statuses
              Used to retrieve the status of the front-end in a structure
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  status, a pointer to the status structure (configurable in CUSTOM_Status_Struct)
  Parameter:  status_selection, an 8 bit field used to control which items to refresh. Use 0x00 for 'all'.
  Return:     1 if successful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_FE_status_selection      (SILABS_FE_Context* front_end, CUSTOM_Status_Struct *status, unsigned char status_selection)
{
  signed int tuner_statused;

  SiTRACE("API CALL STATUS: SiLabs_API_FE_status_selection (front_end, &status, status_selection 0x%02x);\n", status_selection);
  SiTRACE("SiLabs_API_FE_status in %s\n", Silabs_Standard_Text(front_end->standard));
  SiTRACE("SiLabs_API_FE_status config_code 0x%06x\n", front_end->config_code);
  tuner_statused = 0;
  if (status_selection == 0x00  )    { status_selection = FE_LOCK_STATE + FE_LEVELS + FE_RATES + FE_SPECIFIC + FE_QUALITY + FE_FREQ ;}

  if (status_selection & FE_LOCK_STATE) { SiTRACE("FE_LOCK_STATE\n"); }
  if (status_selection & FE_LEVELS    ) { SiTRACE("FE_LEVELS\n"    ); }
  if (status_selection & FE_RATES     ) { SiTRACE("FE_RATES\n"     ); }
  if (status_selection & FE_SPECIFIC  ) { SiTRACE("FE_SPECIFIC\n"  ); }
  if (status_selection & FE_QUALITY   ) { SiTRACE("FE_QUALITY\n"   ); }
  if (status_selection & FE_FREQ      ) { SiTRACE("FE_FREQ\n"      ); }

  /* For FE_QUALITY monitoring (SSI), retrieving the rssi from the tuner is required, so we add FE_LEVELS if it's not on */
  if ( (status_selection & FE_QUALITY) && ((status_selection & FE_LEVELS    ) == 0) ) { status_selection = status_selection + FE_LEVELS;     }
  /* For FE_RATES   monitoring (ber/fer/per), the demodulator needs to be locked, so we add FE_LOCK_STATE if it's not on */
  if ( (status_selection & FE_RATES  ) && ((status_selection & FE_LOCK_STATE) == 0) ) { status_selection = status_selection + FE_LOCK_STATE; }
  SiTRACE("SiLabs_API_FE_status status_selection 0x%02x\n", status_selection);

  /* Call SiLabs_API_TER_Tuner_status only if FE_FREQ or FE_LEVELS monitoring */
  if ((status_selection & FE_LEVELS) || (status_selection & FE_FREQ) ) {
    switch (front_end->standard) {
      case SILABS_ANALOG: {
        //SiLabs_API_TER_Tuner_status   (front_end, status);  // del jhkwon 20170826 for XL1P
        status->standard = SILABS_ANALOG;
        tuner_statused++;
        break;
      }
      case SILABS_DVB_T :
      case SILABS_ISDB_T:
      case SILABS_DVB_T2:
      case SILABS_DVB_C :
      case SILABS_DVB_C2:
      case SILABS_MCNS  : {
        //SiLabs_API_TER_Tuner_status   (front_end, status);  // del jhkwon 20170826 for XL1P
        tuner_statused++;
        break;
      }
      case SILABS_DVB_S :
      case SILABS_DVB_S2:
      case SILABS_DSS   : {
        SiLabs_API_SAT_Tuner_status   (front_end, status);
        tuner_statused++;
        break;
      }
      case SILABS_SLEEP : {
        status->standard = SILABS_SLEEP;
        break;
      }
      default           : { break; }
    }
    if (tuner_statused) { SiTRACE("Tuner freq %ld, tuner rssi %d\n", status->freq, status->rssi); }
  }
  if (front_end->standard == SILABS_ANALOG) {return 1;}

  SiLabs_API_Demod_status_selection (front_end, status, status_selection);
  return 1;
}
/************************************************************************************************************************
  SiLabs_API_Lock_status function
  Use:        Lock status function,
              Used to retrieve the lock status except for satellite
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  status, a pointer to the status structure (configurable in CUSTOM_Status_Struct)
  Return:     1 if successful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_Lock_status              (SILABS_FE_Context *front_end, CUSTOM_Status_Struct *status)
{
	SiTRACE("SiLabs_API_Lock_status\n");

	status->never_lock       =  0;
    status->pcl_lock         =  0;
    status->dl_lock          =  0;
    status->dl_lock_in_range =  0;



	if (Si2164B_L1_DD_STATUS (front_end->SiDemod_FE->demod, Si2164B_DD_STATUS_CMD_INTACK_OK) != NO_SiDemod_ERROR) {
      SiERROR("Si2164B_L1_DD_STATUS ERROR\n");
      SiTRACE("Si2164B_L1_DD_STATUS ERROR\n");
      return 0;
    }
    status->standard = Custom_standardCode(front_end, front_end->SiDemod_FE->demod->rsp->dd_status.modulation);
	SiTRACE("DD_STATUS neverlock: %d fef: %d sync_lock: %d Master_lock: %d, current standard: %s\n", status->never_lock, status->fef, front_end->SiDemod_FE->demod->rsp->dd_status.pcl, front_end->SiDemod_FE->demod->rsp->dd_status.dl, Silabs_Standard_Text((CUSTOM_Standard_Enum)status->standard) );

	status->pcl_lock      = front_end->SiDemod_FE->demod->rsp->dd_status.pcl;
	status->dl_lock        = front_end->SiDemod_FE->demod->rsp->dd_status.dl;

    switch (status->standard) {
      case SILABS_DVB_T : {
        if (Si2164B_L1_DVBT_STATUS    (front_end->SiDemod_FE->demod, Si2164B_DVBT_STATUS_CMD_INTACK_OK)                        != NO_SiDemod_ERROR) return 0;
        status->never_lock         = SiDemod_Check_NeverLock(front_end->SiDemod_FE->demod);
        status->pcl_lock         = front_end->SiDemod_FE->demod->rsp->dvbt_status.pcl;
        status->dl_lock           = front_end->SiDemod_FE->demod->rsp->dvbt_status.dl;
        break;
      }
      case SILABS_DVB_T2 : {
        if (Si2164B_L1_DVBT2_STATUS   (front_end->SiDemod_FE->demod, Si2164B_DVBT2_STATUS_CMD_INTACK_OK)                       != NO_SiDemod_ERROR) return 0;
        status->never_lock         = SiDemod_Check_NeverLock(front_end->SiDemod_FE->demod);
		status->pcl_lock         = front_end->SiDemod_FE->demod->rsp->dvbt2_status.pcl;
        status->dl_lock           = front_end->SiDemod_FE->demod->rsp->dvbt2_status.dl;
        break;
      }
      case SILABS_DVB_C : {
        if (Si2164B_L1_DVBC_STATUS    (front_end->SiDemod_FE->demod, Si2164B_DVBC_STATUS_CMD_INTACK_OK) != NO_SiDemod_ERROR) return 0;
        status->pcl_lock         = front_end->SiDemod_FE->demod->rsp->dvbc_status.pcl;
        status->dl_lock           = front_end->SiDemod_FE->demod->rsp->dvbc_status.dl;
		status->dl_lock_in_range  = SiDemod_L1_DVB_C_get_dl_lock_in_range(front_end->SiDemod_FE->demod, status->symbol_rate/1000);//kwon
		break;
      }
	  case SILABS_MCNS  : {
        if (Si2164B_L1_MCNS_STATUS   (front_end->SiDemod_FE->demod, Si2164B_MCNS_STATUS_CMD_INTACK_OK) != NO_Si2164B_ERROR) return 0;
        status->pcl_lock         = front_end->SiDemod_FE->demod->rsp->mcns_status.pcl;
        status->dl_lock           = front_end->SiDemod_FE->demod->rsp->mcns_status.dl;
        break;
      }
      case SILABS_DVB_C2 : {
        if (Si2164B_L1_DVBC2_STATUS   (front_end->SiDemod_FE->demod, Si2164B_DVBC2_STATUS_CMD_INTACK_OK)                       != NO_SiDemod_ERROR) return 0;
        status->pcl_lock         = front_end->SiDemod_FE->demod->rsp->dvbc2_status.pcl;
        status->dl_lock           = front_end->SiDemod_FE->demod->rsp->dvbc2_status.dl;
        break;
      }	  
      default           : {
        return 0;
        break;
      }
    }

	if(status->dl_lock == 1) {front_end->standard = status->standard;}

	return 1;
}

// Get RSSI 
signed int   SiLabs_API_DVB_T2_RSSI_status (SILABS_FE_Context *front_end, CUSTOM_Status_Struct *status)
{
	SILABS_TER_TUNER_Context *tuner_ter;
	SiTRACE("SiLabs_API_RSSI_status\n");
	tuner_ter = front_end->SiDemod_FE->tuner_ter;
    status=status;  /* To avoid compiler warning */
	if (tuner_ter == NULL) {
		SiTRACE("No tuner_ter defined, SiLabs_API_RSSI_status can't be executed!\n");
	return 0;
	}
	SiLabs_API_Tuner_I2C_Enable  (front_end);
	Si2151_L1_TUNER_STATUS            (tuner_ter);

	SiLabs_API_Tuner_I2C_Disable (front_end);

   return ((signed int)(tuner_ter->rsp->tuner_status.rssi - 6));
}

/************************************************************************************************************************
  SiLabs_API_Text_status function
  Use:        Front-End status function
              Used to retrieve the status of the front-end in a text
  Behavior:   It will call SiLabs_API_Text_status_selection with the status_selection flag corresponding to 'all'
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  status, a pointer to the status structure (configurable in CUSTOM_Status_Struct)
  Parameter:  formatted_string, a text buffer to store the result
  Return:     1 if successful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_Text_status              (SILABS_FE_Context* front_end, CUSTOM_Status_Struct *status, unsigned char *formatted_status)
{
  SiTRACE("API CALL STATUS: SiLabs_API_Text_status (front_end, &status, &formatted_status);\n");
  return SiLabs_API_Text_status_selection (front_end, status, formatted_status, 0x00);
}
/************************************************************************************************************************
  SiLabs_API_Text_status_selection function
  Use:        Front-End status function, with selection of 'all' or partial statuses
              Used to retrieve the status of the front-end in a text
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  status, a pointer to the status structure (configurable in CUSTOM_Status_Struct)
  Parameter:  formatted_status, a text buffer to store the result
  Parameter:  status_selection, an 8 bit field used to control which items to refresh. Use 0x00 for 'all'.
  Return:     1 if successful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_Text_status_selection    (SILABS_FE_Context* front_end, CUSTOM_Status_Struct *status, unsigned char *formatted_status, unsigned char status_selection)
{
  signed char rate_i, rate_d, rate_e;
  SiTRACE("API CALL STATUS: SiLabs_API_Text_status_selection (front_end, &status, &formatted_status, status_selection 0x%02x);\n", status_selection);
  status_selection=status_selection;/*avoid compile warning*/
  rate_i = rate_d = rate_e = 0;
    safe_sprintf(formatted_status, "\nFront_end status:");
  if (status->demod_die == 0) { safe_sprintf(formatted_status, " SINGLE  "); }
  if (status->demod_die == 1) { safe_sprintf(formatted_status, " DEMOD A "); }
  if (status->demod_die == 2) { safe_sprintf(formatted_status, " DEMOD B "); }
    safe_sprintf(formatted_status, "%s (0x%06x)\n\n", formatted_status, front_end->config_code );
    switch (status->standard)
    {
        case SILABS_ANALOG: { safe_sprintf(formatted_status, "%s standard       ANALOG MODE\n", formatted_status); return 0 ; break; }
        case SILABS_DVB_T : { safe_sprintf(formatted_status, "%s standard       DVB-T\n" , formatted_status); break; }
        case SILABS_DVB_T2: { safe_sprintf(formatted_status, "%s standard       DVB-T2\n", formatted_status); break; }
        case SILABS_ISDB_T: { safe_sprintf(formatted_status, "%s standard       ISDB-T\n", formatted_status); break; }
        case SILABS_DVB_C : { safe_sprintf(formatted_status, "%s standard       DVB-C\n" , formatted_status); break; }
        case SILABS_DVB_C2: { safe_sprintf(formatted_status, "%s standard       DVB-C2\n", formatted_status); break; }
        case SILABS_MCNS  : { safe_sprintf(formatted_status, "%s standard       MCNS\n"  , formatted_status); break; }
        case SILABS_DVB_S : { safe_sprintf(formatted_status, "%s standard       DVB-S\n" , formatted_status); break; }
        case SILABS_DVB_S2: { safe_sprintf(formatted_status, "%s standard       DVB-S2\n", formatted_status); break; }
        case SILABS_DSS   : { safe_sprintf(formatted_status, "%s standard       DSS\n"   , formatted_status); break; }
        case SILABS_SLEEP : { safe_sprintf(formatted_status, "%s SLEEP MODE\n"           , formatted_status); return 0 ; break; }
        default           : { safe_sprintf(formatted_status, "%s INVALID standard (%d)!\n",formatted_status, front_end->standard); return 0; break; }
    }
    switch (status->standard)
    {
        case SILABS_ANALOG:
        case SILABS_DVB_T :
        case SILABS_DVB_T2:
    case SILABS_ISDB_T:
        case SILABS_DVB_C :
        case SILABS_DVB_C2:
        case SILABS_MCNS  : { safe_sprintf(formatted_status, "%s frequency         %ld  Hz\n", formatted_status, status->freq); break; }
        case SILABS_DVB_S :
        case SILABS_DVB_S2:
        case SILABS_DSS   : { safe_sprintf(formatted_status, "%s frequency         %ld kHz\n", formatted_status, status->freq); break; }
        default           : { safe_sprintf(formatted_status, "%s INVALID standard (%d)!\n",    formatted_status, front_end->standard); return 0; break; }
    }

    safe_sprintf(formatted_status, "%s pcl_lock        %d", formatted_status, status->pcl_lock);
    if (status->pcl_lock) { safe_sprintf(formatted_status, "%s : locked\n"  , formatted_status);
    } else                  { safe_sprintf(formatted_status, "%s : unlocked\n", formatted_status);
    }
    safe_sprintf(formatted_status, "%s dl_lock          %d", formatted_status, status->dl_lock);
    if (status->dl_lock)   { safe_sprintf(formatted_status, "%s : locked\n"  , formatted_status);
    } else {                  safe_sprintf(formatted_status, "%s : unlocked\n", formatted_status);
    }

    switch (status->standard)
    {
        case SILABS_ANALOG: {
          safe_sprintf(formatted_status, "%s standard       ANALOG\n", formatted_status);
          break;
        }
        case SILABS_DVB_T : {
          safe_sprintf(formatted_status, "%s characteristics     %s ", formatted_status, Silabs_FFT_Text(status->fft_mode));
          safe_sprintf(formatted_status, "%s %s "                , formatted_status, Silabs_Constel_Text(status->constellation));
          safe_sprintf(formatted_status, "%s %s "                , formatted_status, Silabs_Coderate_Text(status->code_rate_hp));
          safe_sprintf(formatted_status, "%s %s "                , formatted_status, Silabs_GI_Text(status->guard_interval));
          safe_sprintf(formatted_status, "%s %d kHz \n"          , formatted_status, status->bandwidth_Hz/1000);
          break;
        }
    case SILABS_ISDB_T: {
      safe_sprintf(formatted_status, "%s characteristics     %s ", formatted_status, Silabs_FFT_Text(status->fft_mode));
      safe_sprintf(formatted_status, "%s %s "                    , formatted_status, Silabs_GI_Text(status->guard_interval));
      safe_sprintf(formatted_status, "%s nb_segments %d/%d/%d "  , formatted_status, status->nb_seg_a, status->nb_seg_b, status->nb_seg_c );
      safe_sprintf(formatted_status, "%s %d kHz \n"              , formatted_status, status->bandwidth_Hz/1000);
      safe_sprintf(formatted_status, "%s system_id:%d  \n"       , formatted_status, status->isdbt_system_id );
      break;
    }
        case SILABS_DVB_C : {
          safe_sprintf(formatted_status, "%s characteristics     %s ", formatted_status, Silabs_Constel_Text(status->constellation));
          safe_sprintf(formatted_status, "%s %d S/s \n"              , formatted_status, status->symbol_rate);
          break;
        }
        case SILABS_DVB_C2: {
          safe_sprintf(formatted_status, "%s characteristics    "   , formatted_status);
          safe_sprintf(formatted_status, "%s %s "                   , formatted_status, Silabs_Constel_Text(status->constellation));
          safe_sprintf(formatted_status, "%s %s "                   , formatted_status, Silabs_Coderate_Text(status->code_rate));
          safe_sprintf(formatted_status, "%s %s "                   , formatted_status, Silabs_GI_Text(status->guard_interval));
          safe_sprintf(formatted_status, "%s %d kHz \n"             , formatted_status, status->bandwidth_Hz/1000);
          safe_sprintf(formatted_status, "%s ds_id:%d "             , formatted_status, status->ds_id);
          safe_sprintf(formatted_status, "%s num_plp:%d "           , formatted_status, status->num_plp);
          safe_sprintf(formatted_status, "%s plp_id:%d "            , formatted_status, status->plp_id);
          safe_sprintf(formatted_status, "%s\n"                     , formatted_status);
          safe_sprintf(formatted_status, "%s fer        %d.%de%d\n" , formatted_status, rate_i, rate_d, -rate_e);
          if (status->fer_mant == -1) {
          safe_sprintf(formatted_status, "%s fer              --------\n", formatted_status);
          } else {
          rate_f_mant_exp(status->fer_mant, status->fer_exp, &rate_i, &rate_d, &rate_e);
          safe_sprintf(formatted_status, "%s fer              %d.%de%d\n", formatted_status, rate_i, rate_d, rate_e);
          }
          break;
        }
        case SILABS_MCNS  : {
          safe_sprintf(formatted_status, "%s characteristics     %s ", formatted_status, Silabs_Constel_Text(status->constellation));
          safe_sprintf(formatted_status, "%s %d S/s \n"              , formatted_status, status->symbol_rate);
          break;
        }
        case SILABS_DVB_S : {
          safe_sprintf(formatted_status, "%s characteristics     %s ", formatted_status, Silabs_Constel_Text(status->constellation));
          safe_sprintf(formatted_status, "%s %d S/s "                , formatted_status, status->symbol_rate);
          safe_sprintf(formatted_status, "%s %s \n"                  , formatted_status, Silabs_Coderate_Text(status->code_rate));
          break;
        }
        case SILABS_DSS   : {
          safe_sprintf(formatted_status, "%s characteristics     %s ", formatted_status, Silabs_Constel_Text(status->constellation));
          safe_sprintf(formatted_status, "%s %d S/s "                , formatted_status, status->symbol_rate);
          safe_sprintf(formatted_status, "%s %s \n"                 , formatted_status, Silabs_Coderate_Text(status->code_rate));
          break;
        }
        case SILABS_DVB_S2: {
          safe_sprintf(formatted_status, "%s characteristics     %s ", formatted_status, Silabs_Constel_Text(status->constellation));
          safe_sprintf(formatted_status, "%s %d S/s "                , formatted_status, status->symbol_rate);
          safe_sprintf(formatted_status, "%s %s "                    , formatted_status, Silabs_Coderate_Text(status->code_rate));
          safe_sprintf(formatted_status, "%s %s "          , formatted_status, Silabs_Pilots_Text(status->pilots));
          safe_sprintf(formatted_status, "%s num_is %d "             , formatted_status, status->num_is);
          safe_sprintf(formatted_status, "%s isi_id %d\n"            , formatted_status, status->isi_id);
          if (status->fer_mant == -1) {
          safe_sprintf(formatted_status, "%s fer              --------\n", formatted_status);
          } else {
          rate_f_mant_exp(status->fer_mant, status->fer_exp, &rate_i, &rate_d, &rate_e);
          safe_sprintf(formatted_status, "%s fer              %d.%de%d\n", formatted_status, rate_i, rate_d, rate_e);
          }
          break;
        }
        case SILABS_DVB_T2: {
          safe_sprintf(formatted_status, "%s characteristics     %s", formatted_status, Silabs_FFT_Text(status->fft_mode));
          safe_sprintf(formatted_status, "%s %s "                   , formatted_status, Silabs_T2_Base_Lite_Text(status->t2_base_lite));
          safe_sprintf(formatted_status, "%s %s "                   , formatted_status, Silabs_Extended_BW_Text(status->bw_ext));
          safe_sprintf(formatted_status, "%s %s"                    , formatted_status, Silabs_Constel_Text(status->constellation));
          safe_sprintf(formatted_status, "%s %s "                   , formatted_status, Silabs_Rotated_QAM_Text(status->rotated));
          safe_sprintf(formatted_status, "%s %s "                   , formatted_status, Silabs_Coderate_Text(status->code_rate));
          safe_sprintf(formatted_status, "%s %s "                   , formatted_status, Silabs_GI_Text(status->guard_interval));
          safe_sprintf(formatted_status, "%s %d kHz \n"             , formatted_status, status->bandwidth_Hz/1000);
          safe_sprintf(formatted_status, "%s %s "                   , formatted_status, Silabs_T2_Version_Text(status->t2_version));
          safe_sprintf(formatted_status, "%s %s "                   , formatted_status, Silabs_FEF_Text(status->fef));
          safe_sprintf(formatted_status, "%s %s "                   , formatted_status, Silabs_MISO_Text(status->tx_mode));
          safe_sprintf(formatted_status, "%s %s "                   , formatted_status, Silabs_Pilot_Pattern_Text(status->pilot_pattern));
          safe_sprintf(formatted_status, "%s num_plp:%d "           , formatted_status, status->num_plp);
          safe_sprintf(formatted_status, "%s plp_id:%d "            , formatted_status, status->plp_id);
          safe_sprintf(formatted_status, "%s system_id:%d "         , formatted_status, status->isdbt_system_id);
          safe_sprintf(formatted_status, "%s %s \n"                 , formatted_status, Silabs_No_Short_Frame_Text(status->short_frame));
          if (status->fer_mant == -1) {
          safe_sprintf(formatted_status, "%s fer              --------\n", formatted_status);
          } else {
          rate_f_mant_exp(status->fer_mant, status->fer_exp, &rate_i, &rate_d, &rate_e);
          safe_sprintf(formatted_status, "%s fer              %d.%de%d\n", formatted_status, rate_i, rate_d, rate_e);
          }
          break;
        }
        default           : {
          safe_sprintf(formatted_status, "%s INVALID standard (%d)!!!!\n"  , formatted_status, front_end->standard);
          break;
        }
    }

    if (status->ber_mant     == -1) {
    safe_sprintf(formatted_status, "%s ber              --------\n", formatted_status);
    } else {
    rate_f_mant_exp(status->ber_mant, status->ber_exp, &rate_i, &rate_d, &rate_e);
    safe_sprintf(formatted_status, "%s ber              %d.%de%d\n"   , formatted_status, rate_i, rate_d, rate_e);
    }

    if (status->per_mant     == -1) {
    safe_sprintf(formatted_status, "%s per              --------\n", formatted_status);
    } else {
    rate_f_mant_exp(status->per_mant, status->per_exp, &rate_i, &rate_d, &rate_e);
    safe_sprintf(formatted_status, "%s per              %d.%de%d\n"   , formatted_status, rate_i, rate_d, rate_e);
    }

    safe_sprintf(formatted_status, "%s c_n              %2d.%02d\n"   , formatted_status, status->c_n_100/100, (status->c_n_100 - 100*(status->c_n_100/100)) );
    if (status->uncorrs == -1) {
    safe_sprintf(formatted_status, "%s uncorrs          --------\n", formatted_status);
    } else {
    safe_sprintf(formatted_status, "%s uncorrs          %8d\n"     , formatted_status, status->uncorrs);
    }
    if (status->SQI == -1) status->SQI = 0;
    safe_sprintf(formatted_status, "%s spectral_invers. %8d\n"      , formatted_status, status->spectral_inversion);
    safe_sprintf(formatted_status, "%s RFagc            %8d\n"      , formatted_status, status->RFagc);
    safe_sprintf(formatted_status, "%s IFagc            %8d\n"      , formatted_status, status->IFagc);
    safe_sprintf(formatted_status, "%s freq_offset      %8ld\n"     , formatted_status, status->freq_offset);
    safe_sprintf(formatted_status, "%s timing_offset    %8ld\n"     , formatted_status, status->timing_offset);
    safe_sprintf(formatted_status, "%s RSSI             %f\n"       , formatted_status, status->RSSI);
    safe_sprintf(formatted_status, "%s SSI              %3d <%0*d%*s\n", formatted_status, status->SSI, (status->SSI)+1, 0, 100 - (status->SSI) +1, ">");
    safe_sprintf(formatted_status, "%s SQI              %3d <%0*d%*s\n", formatted_status, status->SQI, (status->SQI)+1, 0, 100 - (status->SQI) +1, ">");
    safe_sprintf(formatted_status, "%s TS bitrate       %d kbit/s\n", formatted_status, status->TS_bitrate_kHz);
    safe_sprintf(formatted_status, "%s TS clock         %d kHz\n"   , formatted_status, status->TS_clock_kHz  );
    return status->pcl_lock;

}
/*****************************************************************************************/
/*               SiLabs demodulator API functions (demod and tuner)                      */
/*****************************************************************************************/
/* Main SW init function (to be called first)                                            */
/************************************************************************************************************************
  SiLabs_API_SW_Init function
  Use:        software initialization function
              Used to initialize the DTV demodulator and tuner structures
  Behavior:   This function performs all the steps necessary to initialize the demod and tuner instances
  Parameter:  front_end, a pointer to the SILABS_FE_Context context to be initialized
  Parameter:  demodAdd, the I2C address of the demod
  Parameter:  tunerAdd, the I2C address of the tuner
  Comments:     It MUST be called first and once before using any other function.
                It can be used to build a multi-demod/multi-tuner application, if called several times from the upper
                  layer with different pointers and addresses.
                After execution, all demod and tuner functions are accessible.
************************************************************************************************************************/
unsigned char  SiLabs_API_SW_Init                  (SILABS_FE_Context *front_end, signed int demodAdd, signed int tunerAdd_Ter, signed int tunerAdd_Sat)
{
  signed int chip;
  signed int i;
  i            = 0;            /* To avoid compiler warning when not used */

  front_end->TER_Address_add = tunerAdd_Ter;
  front_end->SAT_Address_add = tunerAdd_Sat;
  front_end->demod_add       = demodAdd;

  SiTRACE("API CALL CONFIG: SiLabs_API_SW_Init                       (front_end, 0x%02x, 0x%02x, 0x%02x);\n", demodAdd, tunerAdd_Ter, tunerAdd_Sat);
  SiTRACE("Wrapper              Source code %s\n", SiLabs_API_TAG_TEXT() );
  SiTRACE("tunerAdd_Ter 0x%02x\n", tunerAdd_Ter);
  SiTRACE("tunerAdd_Sat 0x%02x\n", tunerAdd_Sat);
  front_end->config_code = (tunerAdd_Ter<<16) + (tunerAdd_Sat<<8) + (demodAdd<<0);
  SiTRACE("config_code 0x%06x\n", front_end->config_code);
  /* Start by detecting the chip type */
  if (front_end->chip == 0) {
    chip = SiLabs_chip_detect(demodAdd);
  } else {
    chip = front_end->chip;
  }
  SiTRACE("chip '%d' ('%X')\n", chip, chip);
  front_end->standard = -1;
  front_end->active_TS_mode = SILABS_TS_PARALLEL;
  WrapperI2C = &WrapperI2C_context;
  WrapperI2C->indexSize = 0;
  WrapperI2C->mustReadWithoutStop = 0;
#ifdef   SiTRACES
  //snprintf(front_end->tag, SILABS_TAG_SIZE ,"fe[%d]", front_end->fe_index);
#endif /* SiTRACES */
#ifdef    INDIRECT_I2C_CONNECTION
  SiTRACE("INDIRECT_I2C_CONNECTION allowed.\n");
  for (i=0; i< FRONT_END_COUNT; i++) {
    if ( front_end  == &(FrontEnd_Table[i]) ) {
      front_end->TER_tuner_I2C_connection = i;
      front_end->SAT_tuner_I2C_connection = i;
    }
  }
  SiTRACE("Default front_end->TER_tuner_I2C_connection: %d\n", front_end->TER_tuner_I2C_connection );
  SiTRACE("Default front_end->SAT_tuner_I2C_connection: %d\n", front_end->SAT_tuner_I2C_connection );
#endif /* INDIRECT_I2C_CONNECTION */
#ifndef   LNBH_I2C_ADDRESS
  #define LNBH_I2C_ADDRESS 0x10
#endif /* LNBH_I2C_ADDRESS */
#ifdef    UNICABLE_COMPATIBLE
  front_end->unicable = &(front_end->Unicable_Obj);
  front_end->lnb_type = UNICABLE_LNB_TYPE_NORMAL;
  SiLabs_Unicable_API_Init (
           front_end->unicable,
           front_end,
           (void*)&SiLabs_API_SAT_Tuner_Tune,
           (void*)&SiLabs_API_SAT_Get_AGC,
           (void*)&SiLabs_API_SAT_prepare_diseqc_sequence,
           (void*)&SiLabs_API_SAT_trigger_diseqc_sequence,
           (void*)&SiLabs_API_SAT_send_diseqc_sequence,
           (void*)&SiLabs_API_SAT_Tuner_SetLPF,
           (void*)&SiLabs_API_SAT_voltage,
           (void*)&SiLabs_API_SAT_tone
  );
#endif /* UNICABLE_COMPATIBLE */
#ifdef    LNBH21_COMPATIBLE
  front_end->lnb_chip = 21;
  front_end->lnb_chip_init_done = 0;
  front_end->lnbh21 = &(front_end->lnbh21_Obj);
  L1_LNBH21_Init(front_end->lnbh21, LNBH_I2C_ADDRESS);
#endif /* LNBH21_COMPATIBLE */
#ifdef    LNBH25_COMPATIBLE
  front_end->lnb_chip = 25;
  front_end->lnb_chip_init_done = 0;
  front_end->lnbh25 = &(front_end->lnbh25_Obj);
  L1_LNBH25_Init(front_end->lnbh25, LNBH_I2C_ADDRESS);
#endif /* LNBH25_COMPATIBLE */
#ifdef    LNBH26_COMPATIBLE
  front_end->lnb_chip = 26;
  front_end->lnb_chip_init_done = 0;
  front_end->lnbh26 = &(front_end->lnbh26_Obj);
  L1_LNBH26_Init(front_end->lnbh26, LNBH_I2C_ADDRESS);
#endif /* LNBH26_COMPATIBLE */
#ifdef    LNBH29_COMPATIBLE
  front_end->lnb_chip = 29;
  front_end->lnb_chip_init_done = 0;
  front_end->lnbh29 = &(front_end->lnbh29_Obj);
  L1_LNBH29_Init(front_end->lnbh29, LNBH_I2C_ADDRESS);
#endif /* LNBH29_COMPATIBLE */
#ifdef    A8293_COMPATIBLE
  front_end->lnb_chip = 0xA8293;
  front_end->lnb_chip_init_done = 0;
  front_end->A8293 = &(front_end->A8293_Obj);
  L1_A8293_Init(front_end->A8293, LNBH_I2C_ADDRESS);
#endif /* A8293_COMPATIBLE */
  front_end->SAT_Select_LNB_Chip_lnb_index = 0;

    front_end->SiDemod_FE = &(front_end->SiDemod_FE_Obj);
#if 0//csm
    if (SiDemod_SW_Init   (front_end->SiDemod_FE
                             , demodAdd
                             , tunerAdd_Ter
#ifdef    INDIRECT_I2C_CONNECTION
                             ,(void*)&SiLabs_API_TER_Tuner_I2C_Enable
                             ,(void*)&SiLabs_API_TER_Tuner_I2C_Disable
#endif /* INDIRECT_I2C_CONNECTION */
                             , tunerAdd_Sat
#ifdef    UNICABLE_COMPATIBLE
                             ,(void*)&SiLabs_API_SAT_Tuner_Tune
                             ,(void*)&SiLabs_API_SAT_Unicable_Tune
#endif /* UNICABLE_COMPATIBLE */
#ifdef    INDIRECT_I2C_CONNECTION
                             ,(void*)&SiLabs_API_SAT_Tuner_I2C_Enable
                             ,(void*)&SiLabs_API_SAT_Tuner_I2C_Disable
#endif /* INDIRECT_I2C_CONNECTION */
                             ,(void*)front_end
                             ) ){
#else
	   if (Si2164B_L2_SW_Init   (front_end->SiDemod_FE
                             , demodAdd
                             , tunerAdd_Ter
#ifdef    INDIRECT_I2C_CONNECTION
                             ,(void*)&SiLabs_API_TER_Tuner_I2C_Enable
                             ,(void*)&SiLabs_API_TER_Tuner_I2C_Disable
#endif /* INDIRECT_I2C_CONNECTION */
                             , tunerAdd_Sat
#ifdef    UNICABLE_COMPATIBLE
                             ,(void*)&SiLabs_API_SAT_Tuner_Tune
                             ,(void*)&SiLabs_API_SAT_Unicable_Tune
#endif /* UNICABLE_COMPATIBLE */
#ifdef    INDIRECT_I2C_CONNECTION
                             ,(void*)&SiLabs_API_SAT_Tuner_I2C_Enable
                             ,(void*)&SiLabs_API_SAT_Tuner_I2C_Disable
#endif /* INDIRECT_I2C_CONNECTION */
                             ,(void*)front_end
                             ) ) {
#endif//end
      front_end->chip = chip;
      Silabs_init_done = 1;
      return 1;
    } else {
      SiTRACE("ERROR initializing Si2164B context\n");
      return 0;
    }
  SiTRACE("Unknown chip '%d'\n", front_end->chip);
  SiERROR("SiLabs_API_SW_Init: Unknown chip !\n");
  return 0;
}
/************************************************************************************************************************
  SiLabs_API_Set_Index_and_Tag function
  Use:        Front-End index and tag function
              Used to store the frontend index and tag, which will be used in traces called from L3 level
              Behavior:   It will store the front_end index and tag in the SILABS_FE_Context
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  index, the frontend index
  Parameter:  tag,   the frontend tag (a string limited to SILABS_TAG_SIZE characters)
  Return:     the length of the frontend->tag
************************************************************************************************************************/
#if 0 // 20140513 del jhkwon
signed int   SiLabs_API_Set_Index_and_Tag        (SILABS_FE_Context* front_end, unsigned char index, unsigned char* tag)
{
  signed int L2_tag_len;
  L2_tag_len = 0;

  SiTRACE("API CALL CONFIG: SiLabs_API_Set_Index_and_Tag (front_end, %d, %s);\n", index, tag);
  front_end->fe_index = index;

  L2_tag_len = Si2164B_L2_Set_Index_and_Tag(front_end->SiDemod_FE, index, tag);

  //snprintf(front_end->tag, SILABS_TAG_SIZE, "%s %*s", tag, strlen(tag) +1 - L2_tag_len, " ");
  return strlen(front_end->tag);
}
#endif
/************************************************************************************************************************
  SiLabs_API_Frontend_Chip function
  Use:        demodulator source code selection function
              Used to select which source code will be used to control the demodulator
  Behavior:   This function sets the chip value in the front-end context
  Parameter:  front_end, a pointer to the SILABS_FE_Context context to be initialized
  Parameter:  demod_id, the code to use in the wrapper to select the demodulator code
************************************************************************************************************************/
signed int   SiLabs_API_Frontend_Chip            (SILABS_FE_Context *front_end, signed int demod_id)
{
  front_end->chip = demod_id;
  SiTRACE("API CALL CONFIG: SiLabs_API_Frontend_Chip                 (front_end, 0x%x/%d);\n", demod_id, demod_id);
  return front_end->chip;
}
/************************************************************************************************************************
  SiLabs_API_TER_tuner_I2C_connection function
  Use:        TER tuner I2C passthrough selection function
              Used to select which demodulator passthrough needs to be used to connect with the TER tuner I2C
  Behavior:   This function sets the TER_tuner_I2C_connection value in the front-end context
************************************************************************************************************************/
signed int   SiLabs_API_TER_tuner_I2C_connection (SILABS_FE_Context *front_end, signed int fe_index)
{
  SiTRACE("API CALL CONFIG: SiLabs_API_TER_tuner_I2C_connection (front_end, %d);\n", fe_index);
  front_end->TER_tuner_I2C_connection = fe_index;
  return front_end->TER_tuner_I2C_connection;
}
/************************************************************************************************************************
  SiLabs_API_SAT_tuner_I2C_connection function
  Use:        SAT tuner I2C passthrough selection function
              Used to select which demodulator passthrough needs to be used to connect with the SAT tuner I2C
  Behavior:   This function sets the SAT_tuner_I2C_connection value in the front-end context
************************************************************************************************************************/
signed int   SiLabs_API_SAT_tuner_I2C_connection (SILABS_FE_Context *front_end, signed int fe_index)
{
  SiTRACE("API CALL CONFIG: SiLabs_API_SAT_tuner_I2C_connection (front_end, %d);\n", fe_index);
  front_end->SAT_tuner_I2C_connection = fe_index;
  return front_end->SAT_tuner_I2C_connection;
}
/************************************************************************************************************************
  SiLabs_API_Handshake_Setup function
  Use:        handshake setup function
              Used to set the handshake mode and period
  Behavior:   This function sets the handshake mode and period in the demod instance
  Parameter:  front_end, a pointer to the SILABS_FE_Context context to be initialized
  Parameter:  handshake_mode,   (0='OFF', 1='ON')
  Parameter:  handshake_period_ms, the duration of each handshake period in ms
************************************************************************************************************************/
signed int   SiLabs_API_Handshake_Setup          (SILABS_FE_Context *front_end, signed int handshake_mode, signed int handshake_period_ms)
{
  SiTRACE("API CALL CONFIG: SiLabs_API_Handshake_Setup          (front_end, %d, %d);\n", handshake_mode, handshake_period_ms);
  front_end           = front_end;           /* To avoid compiler warning if not used */
  handshake_mode      = handshake_mode;      /* To avoid compiler warning if not used */
  handshake_period_ms = handshake_period_ms; /* To avoid compiler warning if not used */
  front_end->SiDemod_FE->handshakeUsed  = handshake_mode; 
  front_end->SiDemod_FE->handshakePeriod_ms  = handshake_period_ms; 
  return handshake_mode*handshake_period_ms;
}
/************************************************************************************************************************
  SiLabs_API_SPI_Setup function
  Use:        SPI fw download configuration function
              Used to configure the SPI download
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  send_option: optional SPI configuration byte.
  Parameter:  clk_pin:    where the spi clock signal comes from.
  Parameter:  clk_pola:   the spi clock signal polarity, 'rising' or 'falling'
  Parameter:  data_pin:   where the spi data signal comes from.
  Parameter:  data_order: whether the spi data signal comes in 'LSB first' or 'MSB first'.
      Please refer to the demodulator's code for possible values.
  Returns:    1 if the part supports SPI, 0 otherwise.
************************************************************************************************************************/
signed int   SiLabs_API_SPI_Setup                (SILABS_FE_Context *front_end, unsigned int send_option, unsigned int clk_pin, unsigned int clk_pola, unsigned int data_pin, unsigned int data_order)
{
  SiTRACE("API CALL CONFIG: SiLabs_API_SPI_Setup                (front_end, %d, %d, %d, %d, %d);\n", send_option, clk_pin, clk_pola, data_pin, data_order);
  front_end->SPI_Setup_send_option = send_option;
  front_end->SPI_Setup_clk_pin     = clk_pin;
  front_end->SPI_Setup_clk_pola    = clk_pola;
  front_end->SPI_Setup_data_pin    = data_pin;
  front_end->SPI_Setup_data_order  = data_order;

  front_end->SiDemod_FE->demod->spi_send_option = send_option;
    if (send_option != 0x00) {
      front_end->SiDemod_FE->demod->spi_download    = 1;
    }
  front_end->SiDemod_FE->demod->spi_clk_pin     = clk_pin;
  front_end->SiDemod_FE->demod->spi_clk_pola    = clk_pola;
  front_end->SiDemod_FE->demod->spi_data_pin    = data_pin;
  front_end->SiDemod_FE->demod->spi_data_order  = data_order;

    return 1;

}
/* TER tuner selection and configuration functions (to be used after SiLabs_API_SW_Init) */
/************************************************************************************************************************
  SiLabs_API_TER_Possible_Tuners function
  Use:        TER tuner information function
              Used to know which TER tuners can be used (selected using SiLabs_API_Select_TER_Tuner)
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  tunerList, a string to store the list of TER tuners
************************************************************************************************************************/
signed int   SiLabs_API_TER_Possible_Tuners      (SILABS_FE_Context *front_end, unsigned char *tunerList)
{
 front_end =front_end; /*to avoid compile warning*/
 tunerList = tunerList; /*to avoid compile warning*/
#ifdef    TER_TUNER_SILABS
 return SiLabs_TER_Tuner_Possible_Tuners(front_end->SiDemod_FE->tuner_ter, tunerList);
#endif /* TER_TUNER_SILABS */
return 0;
}
/************************************************************************************************************************
  SiLabs_API_Select_TER_Tuner function
  Use:        TER tuner selection function
              Used to select which TER tuner is in use
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  ter_tuner_code, the code used for the TER tuner selection,
   easily readible when in hexadecimal, as it matched the part name, i.e '0x2178b' for Si2151
  Parameter:  ter_tuner_index, the index of the tuner in the array of the selected tuner,
   in most cases this will be '0'. This is a provision to allow several tuners to be selected during execution
************************************************************************************************************************/
signed int   SiLabs_API_Select_TER_Tuner         (SILABS_FE_Context *front_end, signed int ter_tuner_code, signed int ter_tuner_index)
{
  SiTRACE("API CALL CONFIG: SiLabs_API_Select_TER_Tuner         (front_end, 0x%02x, %d);\n", ter_tuner_code, ter_tuner_index);
  front_end->Select_TER_Tuner_ter_tuner_code  = ter_tuner_code;
  front_end->Select_TER_Tuner_ter_tuner_index = ter_tuner_index;
#ifdef    TER_TUNER_SILABS
 return SiLabs_TER_Tuner_Select_Tuner (front_end->SiDemod_FE->tuner_ter,  ter_tuner_code, ter_tuner_index);
#endif /* TER_TUNER_SILABS */
return 0;
}
/************************************************************************************************************************
  SiLabs_API_TER_Tuner function
  Use:        TER tuner pointer retrieval function
              Used to retrieve a pointer to the TER tuner in use
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  returns: The pointer to the selected TER tuner
************************************************************************************************************************/
void* SiLabs_API_TER_Tuner                (SILABS_FE_Context *front_end)
{
  SiTRACE("API CALL TER   : SiLabs_API_TER_Tuner         (front_end);\n");
#ifdef   SILABS_TER_TUNER_API

  return front_end->SiDemod_FE->tuner_ter;
#else  /* SILABS_TER_TUNER_API */
  front_end = front_end;             /* To avoid compiler warning if not used */
#endif /* SILABS_TER_TUNER_API */
  return NULL;
}
/************************************************************************************************************************
  SiLabs_API_TER_Address function
  Use:        TER tuner address selection function
              Used to set the I2C address of the TER tuner in use (selected using SiLabs_API_Select_TER_Tuner)
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  add, the i2c address of the TER tuner
************************************************************************************************************************/
signed int   SiLabs_API_TER_Address              (SILABS_FE_Context *front_end, signed int add)
{
  SiTRACE("API CALL CONFIG: SiLabs_API_TER_Address         (front_end, add 0x%02x);\n", add);
  front_end->TER_Address_add  = add;
#ifdef    TER_TUNER_SILABS
 return SiLabs_TER_Tuner_Set_Address(front_end->SiDemod_FE->tuner_ter,  add);
#endif /* TER_TUNER_SILABS */
return 0;
}
/************************************************************************************************************************
  SiLabs_API_TER_Broadcast_I2C function
  Use:        TER tuner I2C broadcast selection function
              Used to enable/disable the broadcast I2C feature of the TER tuner in use (selected using SiLabs_API_Select_TER_Tuner)
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  phase, a flag indicating the broadcast state
  Parameter:  front_end_count, a flag indicating to the tuner if it needs to load fw
************************************************************************************************************************/
signed int   SiLabs_API_TER_Broadcast_I2C        (SILABS_FE_Context *front_ends[], signed int front_end_count)
{
#ifdef   SILABS_TER_TUNER_API
  signed int i;
  signed int c;
  SILABS_TER_TUNER_Context *silabs_tuners[front_end_count];
  SiTRACE_X("API CALL INIT  : SiLabs_API_TER_Broadcast_I2C         (front_ends, %d);\n", front_end_count);
  c = 0;
  for (i = 0; i < front_end_count; i++) {
#ifdef    Si2164B_COMPATIBLE
  if (front_ends[i]->chip == 0x2164B  ) { silabs_tuners[i] = front_ends[i]->SiDemod_FE->tuner_ter;  c++; }
#endif /* Si2164B_COMPATIBLE */
  }
  if (c) { return SiLabs_TER_Tuner_Broadcast_I2C( silabs_tuners, c); }
#endif /* SILABS_TER_TUNER_API */
  front_ends         = front_ends;         /* To avoid compiler warning if not used */
  front_end_count    = front_end_count;    /* To avoid compiler warning if not used */
  return 0;
}
/************************************************************************************************************************
  SiLabs_API_TER_Tuner_ClockConfig function
  Use:        TER tuner clock configuration function
              Used to configure the TER tuner clock path
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  xtal:          is the tuner driving a xtal ('0': no xtal(external clock) '1': xtal driven by tuner).
  Parameter:  xout:          the tuner clock output state ('0': off, '1': on).
************************************************************************************************************************/
signed int   SiLabs_API_TER_Tuner_ClockConfig    (SILABS_FE_Context *front_end, signed int xtal, signed int xout)
{
#ifdef    TER_TUNER_SILABS
  SILABS_TER_TUNER_Context *tuner_ter;
  tuner_ter = NULL;
#endif /* TER_TUNER_SILABS */
  SiTRACE("API CALL CONFIG: SiLabs_API_TER_Tuner_ClockConfig    (front_end, %d, %d);\n", xtal, xout);
  front_end->TER_Tuner_ClockConfig_xtal      = xtal;
  front_end->TER_Tuner_ClockConfig_xout      = xout;
#ifdef    TER_TUNER_SILABS
#ifdef    Si2164B_COMPATIBLE
  if (front_end->chip ==   0x2164B ) { tuner_ter = front_end->SiDemod_FE->tuner_ter; }
#endif /* Si2164B_COMPATIBLE */
  if (tuner_ter == NULL) {
    SiTRACE("No tuner_ter defined, SiLabs_API_TER_Tuner_ClockConfig can't be executed!\n");
    return 0;
  }
  SiLabs_TER_Tuner_ClockConfig (tuner_ter, xtal, xout);
#endif /* TER_TUNER_SILABS */
  return 1;
}
/************************************************************************************************************************
  SiLabs_API_TER_Clock_Options function
  Use:        TER tuner options display function
              Used to known which options are possible for the TER tuner clock path
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  clockOptions:  a string used to sore the options text
  Return: 0 if error, otherwise the tuner code
************************************************************************************************************************/
signed int   SiLabs_API_TER_Clock_Options        (SILABS_FE_Context *front_end, unsigned char* clockOptions)
{
  front_end     = front_end;    /* To avoid compiler warning if not used */
  clockOptions  = clockOptions; /* To avoid compiler warning if not used */
  safe_sprintf(clockOptions, "%s", "");
  safe_sprintf(clockOptions, "%s", "Si2164B clock options:\n");
  safe_sprintf(clockOptions, "%s clock_source  (Xtal_clock:%d, TER_Tuner_clock:%d, SAT_Tuner_clock:%d)\n", clockOptions, Si2164B_Xtal_clock, Si2164B_TER_Tuner_clock, Si2164B_SAT_Tuner_clock);
  safe_sprintf(clockOptions, "%s clock_input   (CLK_CLKIO:%d,  CLK_XTAL_IN:%d,  XTAL(driven by the Si2164B):%d)\n", clockOptions, 44, 33, 32);
  safe_sprintf(clockOptions, "%s clock_freq    (CLK_16MHZ:%d, CLK_24MHZ:%d, CLK_27MHZ:%d\n", clockOptions, 16, 24, 27);
  safe_sprintf(clockOptions, "%s clock_control (ALWAYS_ON:%d,  ALWAYS_OFF:%d, MANAGED:%d\n", clockOptions,  1, 0, 2);

  SiTRACE("SiLabs_API_TER_Clock_Options not supporting the current chip '%d'/'0x%x'\n", front_end->chip, front_end->chip);
  return 0x2164B;
}
/************************************************************************************************************************
  SiLabs_API_TER_Clock function
  Use:        TER tuner clock configuration function, at demodulator level
              Used to configure the TER tuner clock path from TER tuner to demodulator
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  clock_source:  where the clock signal comes from.
  Parameter:  clock_input:   on which pin is the clock received.
  Parameter:  clock_freq:    clock frequency
  Parameter:  clock_control: how to control the clock (always_on/always_off/managed)
   Refer to the demodulator's code for possible values
************************************************************************************************************************/
signed int   SiLabs_API_TER_Clock                (SILABS_FE_Context *front_end, signed int clock_source, signed int clock_input, signed int clock_freq, signed int clock_control)
{
  SiTRACE("API CALL CONFIG: SiLabs_API_TER_Clock                (front_end, %d, %d, %d, %d);\n", clock_source, clock_input, clock_freq, clock_control);
  front_end->TER_Clock_clock_source  = clock_source;
  front_end->TER_Clock_clock_input   = clock_input;
  front_end->TER_Clock_clock_freq    = clock_freq;
  front_end->TER_Clock_clock_control = clock_control;
 return SiDemod_TER_Clock (front_end->SiDemod_FE, clock_source, clock_input, clock_freq, clock_control);
 //return 0;
}
/************************************************************************************************************************
  SiLabs_API_TER_FEF_Options function
  Use:        TER FEF options display function
              Used to known which options are possible for the TER FEF
  parameter:  front_end, a pointer to the SILABS_FE_Context context
  parameter:  fefOptions:  a string used to store the options text
                It needs to be declared with a length of 1000
************************************************************************************************************************/
signed int   SiLabs_API_TER_FEF_Options          (SILABS_FE_Context *front_end, unsigned char* fefOptions)
{
  front_end      = front_end;        /* To avoid compiler warning if not used */
  fefOptions     = fefOptions;       /* To avoid compiler warning if not used */
  safe_sprintf(fefOptions, "%s", "");
//#ifdef    Si2164B_COMPATIBLE
//  if (front_end->chip ==   0x2164B ) {
    safe_sprintf(fefOptions, "%s", "(Si2164B FEF options)\n");
    safe_sprintf(fefOptions, "%s fef mode      (SLOW_NORMAL_AGC:0, FREEZE_PIN:1, SLOW_INITIAL_AGC:2)\n", fefOptions);
    safe_sprintf(fefOptions, "%s fef pin       (Unused:0x0, MP_A:0xa, MP_B:0xb, MP_C:0xc, MP_D:0xd)\n" , fefOptions);
    safe_sprintf(fefOptions, "%s fef level     (0: active low, 1: active high)\n", fefOptions);
    return 0x2164B;
//  }
//#endif /* Si2164B_COMPATIBLE */
//  SiTRACE("SiLabs_API_TER_AGC not supporting the current chip '%d'/'0x%x'\n", front_end->chip, front_end->chip);
//  return 0;
}
/************************************************************************************************************************
  SiLabs_API_TER_FEF_Config function
  Use:        TER tuner FEF configuration function
              Used to configure the TER tuner FEF
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  fef_mode: how the FEF signal is controlled
  Parameter:  fef_pin:  where the FEF signal comes from.
  Parameter:  fef_level: logical level when active.
   Refer to the demodulator's code for possible values
************************************************************************************************************************/
signed int   SiLabs_API_TER_FEF_Config           (SILABS_FE_Context *front_end, signed int fef_mode, signed int fef_pin, signed int fef_level)
{
  SiTRACE("API CALL CONFIG: SiLabs_API_TER_FEF_Config           (front_end, %d, 0x%x, %d);\n", fef_mode, fef_pin, fef_level);
  front_end->TER_FEF_Config_fef_mode       = fef_mode;
  front_end->TER_FEF_Config_fef_pin        = fef_pin;
  front_end->TER_FEF_Config_fef_level      = fef_level;
//#ifdef    Si2164B_COMPATIBLE
  return Si2164B_L2_TER_FEF_CONFIG (front_end->SiDemod_FE, fef_mode, fef_pin, fef_level);
//#endif /* Si2164B_COMPATIBLE */
  //SiTRACE("SiLabs_API_TER_FEF not supporting the current chip '%d'/'0x%x'\n", front_end->chip, front_end->chip);
  //return 0;
}
/************************************************************************************************************************
  SiLabs_API_TER_AGC_Options function
  Use:        TER AGC options display function
              Used to known which options are possible for the TER AGC
  parameter:  front_end, a pointer to the SILABS_FE_Context context
  parameter:  agcOptions:  a string used to store the options text
                It needs to be declared with a length of 1000
************************************************************************************************************************/
signed int   SiLabs_API_TER_AGC_Options          (SILABS_FE_Context *front_end, unsigned char* agcOptions)
{
  front_end      = front_end;        /* To avoid compiler warning if not used */
  agcOptions     = agcOptions;       /* To avoid compiler warning if not used */
  safe_sprintf(agcOptions, "%s", "");
  safe_sprintf(agcOptions, "%s", "(Si2164B AGC options)\n");
  safe_sprintf(agcOptions, "%s agc mode      (Unused:0x0, MP_A:0xa, MP_B:0xb, MP_C:0xc, MP_D:0xd)\n", agcOptions);
  safe_sprintf(agcOptions, "%s agc inversion (0: not_inverted, 1: inverted)\n", agcOptions);
  return 0x2164B;
}
/************************************************************************************************************************
  SiLabs_API_TER_AGC function
  Use:        TER demod AGC configuration function
              Used to configure the TER AGC in the demod
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  agc1_mode:  where the AGC 1 PWM comes from.
  Parameter:  agc1_inversion: 0/1 to indicate if AGC 1 is inverted or not (depends on the TER tuner and HW design)
  Parameter:  agc2_mode:  where the AGC 2 PWM comes from.
  Parameter:  agc2_inversion: 0/1 to indicate if AGC 2 is inverted or not (depends on the TER tuner and HW design)
   Refer to the demodulator's code for possible values
************************************************************************************************************************/
signed int   SiLabs_API_TER_AGC                  (SILABS_FE_Context *front_end, signed int agc1_mode, signed int agc1_inversion, signed int agc2_mode, signed int agc2_inversion)
{
  SiTRACE("API CALL CONFIG: SiLabs_API_TER_AGC                  (front_end, 0x%x, %d, 0x%x, %d);\n", agc1_mode, agc1_inversion, agc2_mode, agc2_inversion);
  front_end->TER_AGC_agc1_mode      = agc1_mode;
  front_end->TER_AGC_agc1_inversion = agc1_inversion;
  front_end->TER_AGC_agc2_mode      = agc2_mode;
  front_end->TER_AGC_agc2_inversion = agc2_inversion;
//  if (front_end->chip == 0x2164B) //csm
  { return SiDemod_TER_AGC (front_end->SiDemod_FE, agc1_mode, agc1_inversion, agc2_mode, agc2_inversion); }
  SiTRACE("SiLabs_API_TER_AGC not supporting the current chip '%d'/'0x%x'\n", front_end->chip, front_end->chip);
  return 0;
}
/************************************************************************************************************************
  SiLabs_API_TER_Tuner_AGC_Input function
  Use:        TER tuner AGC configuration function
              Used to configure the TER tuner AGC in the tuner
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  dtv_agc_source: where the AGC 1 PWM enters the TER tuner
   Refer to the TER tuner code for possible values
************************************************************************************************************************/
signed int   SiLabs_API_TER_Tuner_AGC_Input      (SILABS_FE_Context *front_end, signed int dtv_agc_source )
{
  SiTRACE("API CALL CONFIG: SiLabs_API_TER_Tuner_AGC_Input (front_end, 0x%x);\n", dtv_agc_source);
  front_end->TER_Tuner_AGC_Input_dtv_agc_source = dtv_agc_source;
  front_end->SiDemod_FE->demod->TER_tuner_agc_input = dtv_agc_source;

  return 1;
}
/************************************************************************************************************************
  SiLabs_API_TER_Tuner_FEF_Input function
  Use:        TER tuner FEF configuration function
              Used to configure the TER tuner FEF input in the tuner
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  dtv_fef_freeze_input: where the FEF freeze signal enters the TER tuner
   Referring to the TER tuner code for possible values, these are:

    Si21XXB_DTV_AGC_FREEZE_INPUT_PROP_PIN_NONE      0
    Si21XXB_DTV_AGC_FREEZE_INPUT_PROP_PIN_GPIO1     1
    Si21XXB_DTV_AGC_FREEZE_INPUT_PROP_PIN_GPIO2     2
    Si21XXB_DTV_AGC_FREEZE_INPUT_PROP_PIN_AGC1      4
    Si21XXB_DTV_AGC_FREEZE_INPUT_PROP_PIN_AGC2      5

************************************************************************************************************************/
signed int   SiLabs_API_TER_Tuner_FEF_Input      (SILABS_FE_Context *front_end, signed int dtv_fef_freeze_input )
{
  SiTRACE("API CALL CONFIG: SiLabs_API_TER_Tuner_FEF_Input (front_end, 0x%x);\n", dtv_fef_freeze_input);
  front_end->TER_Tuner_FEF_Input_dtv_fef_freeze_input = dtv_fef_freeze_input;
#ifdef    Si2164B_COMPATIBLE
  if (front_end->chip ==   0x2164B ) { return SiLabs_TER_Tuner_DTV_FEF_SOURCE(front_end->SiDemod_FE->tuner_ter, dtv_fef_freeze_input); }
#endif /* Si2164B_COMPATIBLE */
  SiTRACE("SiLabs_API_TER_Tuner_FEF_Input not supporting the current chip '%d'/'0x%x'\n", front_end->chip, front_end->chip);
  return 0;
}
/************************************************************************************************************************
  SiLabs_API_TER_Tuner_IF_Output function
  Use:        TER tuner IF configuration function
              Used to configure the TER tuner IF in the tuner
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  dtv_out_type: where the AGC 1 PWM exits the TER tuner from
   Refer to the TER tuner code for possible values
************************************************************************************************************************/
signed int   SiLabs_API_TER_Tuner_IF_Output      (SILABS_FE_Context *front_end, signed int dtv_out_type )
{
  SiTRACE("API CALL CONFIG: SiLabs_API_TER_Tuner_IF_Output (front_end, 0x%x);\n", dtv_out_type);
  front_end->TER_Tuner_IF_Output_dtv_out_type = dtv_out_type;

  return 1;
}
/* SAT tuner selection and configuration functions (to be used after SiLabs_API_SW_Init) */
/************************************************************************************************************************
  SiLabs_API_SAT_Possible_Tuners function
  Use:        SAT tuner information function
              Used to know which SAT tuners can be used (selected using SiLabs_API_Select_SAT_Tuner)
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  tunerList, a string to store the list of SAT tuners
************************************************************************************************************************/
signed int   SiLabs_API_SAT_Possible_Tuners      (SILABS_FE_Context *front_end, unsigned char *tunerList)
{
    front_end =front_end; /*to avoid compile warning*/
    tunerList = tunerList; /*to avoid compile warning*/

#ifdef   SILABS_SAT_TUNER_API
 return SiLabs_SAT_Tuner_Possible_Tuners(front_end->SiDemod_FE->tuner_sat, tunerList);
#endif /* SILABS_SAT_TUNER_API */
return 0;
}
/************************************************************************************************************************
  SiLabs_API_Select_SAT_Tuner function
  Use:        SAT tuner selection function
              Used to select which SAT tuner is in use
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  ter_tuner_code, the code used for the SAT tuner selection,
   easily readible when in hexadecimal, as it matched the part name, i.e '0x2178b' for Si2151
  Parameter:  ter_tuner_index, the index of the tuner in the array of the selected tuner,
   in most cases this will be '0'. This is a provision to allow several tuners to be selected during execution
************************************************************************************************************************/
signed int   SiLabs_API_Select_SAT_Tuner         (SILABS_FE_Context *front_end, signed int sat_tuner_code, signed int sat_tuner_index)
{
  SiTRACE("API CALL CONFIG: SiLabs_API_Select_SAT_Tuner         (front_end, 0x%x, %d);\n", sat_tuner_code, sat_tuner_index);
  front_end->Select_SAT_Tuner_sat_tuner_code  = sat_tuner_code;
  front_end->Select_SAT_Tuner_sat_tuner_index = sat_tuner_index;
#ifdef   SILABS_SAT_TUNER_API
 return SiLabs_SAT_Tuner_Select_Tuner (front_end->SiDemod_FE->tuner_sat,  sat_tuner_code, sat_tuner_index);
#endif /* SILABS_SAT_TUNER_API */
return 0;
}
/************************************************************************************************************************
  SiLabs_API_SAT_Tuner_Sub function
  Use:        SAT tuner sub-division selection function
              Used to select which part of a SAT tuner to use, when the SAT tuner is a dual.
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  ter_tuner_sub, the flag used in the SAT tuner driver to select between sub-sections
   in most cases this will be '0'. It's only set to 1 for secondary paths in dual SAT tuners
************************************************************************************************************************/
signed int   SiLabs_API_SAT_Tuner_Sub            (SILABS_FE_Context *front_end, signed   int sat_tuner_sub)
{
  SiTRACE("API CALL CONFIG: SiLabs_API_SAT_Tuner_Sub            (front_end, %d);\n", sat_tuner_sub);
  front_end->Select_SAT_Tuner_sat_tuner_sub   = sat_tuner_sub;
#ifdef   SILABS_SAT_TUNER_API
 return SiLabs_SAT_Tuner_Sub (front_end->SiDemod_FE->tuner_sat,  sat_tuner_sub);
#endif /* SILABS_SAT_TUNER_API */
  return 0;
}
/************************************************************************************************************************
  SiLabs_API_SAT_Address function
  Use:        SAT tuner address selection function
              Used to set the I2C address of the SAT tuner in use (selected using SiLabs_API_Select_SAT_Tuner)
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  add, the i2c address of the SAT tuner
************************************************************************************************************************/
signed int   SiLabs_API_SAT_Address              (SILABS_FE_Context *front_end, signed int add)
{
  SiTRACE("API CALL CONFIG: SiLabs_API_SAT_Address         (front_end, add 0x%02x);\n", add);
  front_end->SAT_Address_add = add;
#ifdef   SILABS_SAT_TUNER_API
 return SiLabs_SAT_Tuner_Set_Address(front_end->SiDemod_FE->tuner_sat,  add);
#endif /* SILABS_SAT_TUNER_API */
return 0;
}
/************************************************************************************************************************
  SiLabs_API_SAT_Clock_Options function
  Use:        SAT tuner options display function
              Used to known which options are possible for the SAT tuner clock path
  parameter:  front_end, a pointer to the SILABS_FE_Context context
  parameter:  clockOptions:  a string used to store the options text
              It needs to be declared with a length of 1000
  Return: 0 if error, otherwise the tuner code
************************************************************************************************************************/
signed int   SiLabs_API_SAT_Clock_Options        (SILABS_FE_Context *front_end, unsigned char* clockOptions)
{
  front_end     = front_end;    /* To avoid compiler warning if not used */
  clockOptions  = clockOptions; /* To avoid compiler warning if not used */
  safe_sprintf(clockOptions, "%s", "");
  safe_sprintf(clockOptions, "%s", "Si2164B clock options:\n");
  safe_sprintf(clockOptions, "%s clock_source  (Xtal_clock:%d,  TER_Tuner_clock:%d,  SAT_Tuner_clock:%d)\n", clockOptions, Si2164B_Xtal_clock, Si2164B_TER_Tuner_clock, Si2164B_SAT_Tuner_clock);
  safe_sprintf(clockOptions, "%s clock_input   (CLK_CLKIO:%d,  CLK_XTAL_IN:%d,  XTAL(driven by the Si264):%d)\n", clockOptions, 44, 33, 32);
  safe_sprintf(clockOptions, "%s clock_freq    (CLK_16MHZ:%d,  CLK_24MHZ:%d,  CLK_27MHZ:%d\n", clockOptions, 16, 24, 27);
  safe_sprintf(clockOptions, "%s clock_control (ALWAYS_ON:%d, ALWAYS_OFF:%d, MANAGED:%d\n"   , clockOptions,  1, 0, 2);
  SiTRACE("SiLabs_API_SAT_Clock not supporting the current chip '%d'/'0x%x'\n", front_end->chip, front_end->chip);
  return 0x2164B;
}
/************************************************************************************************************************
  SiLabs_API_SAT_Clock function
  Use:        SAT tuner configuration function
              Used to configure the SAT tuner clock path
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  clock_source:  where the clock signal comes from.
  Parameter:  clock_input:   on which pin is the clock received.
  Parameter:  clock_freq:    clock frequency
  Parameter:  clock_control: how to control to clock (always_on/always_off/managed)
   Refer to the demodulator's code for possible values
************************************************************************************************************************/
signed int   SiLabs_API_SAT_Clock                (SILABS_FE_Context *front_end, signed int clock_source, signed int clock_input, signed int clock_freq, signed int clock_control)
{
  SiTRACE("API CALL CONFIG: SiLabs_API_SAT_Clock                (front_end, %d, %d, %d, %d);\n", clock_source, clock_input, clock_freq, clock_control);
  front_end->SAT_Clock_clock_source  = clock_source;
  front_end->SAT_Clock_clock_input   = clock_input;
  front_end->SAT_Clock_clock_freq    = clock_freq;
  front_end->SAT_Clock_clock_control = clock_control;
 return SiDemod_SAT_Clock (front_end->SiDemod_FE, clock_source, clock_input, clock_freq, clock_control);
 return 0;
}
/************************************************************************************************************************
  SiLabs_API_SAT_AGC_Options function
  Use:        SAT AGC options display function
              Used to known which options are possible for the SAT AGC
  parameter:  front_end, a pointer to the SILABS_FE_Context context
  parameter:  agcOptions:  a string used to store the options text
                It needs to be declared with a length of 1000
************************************************************************************************************************/
signed int   SiLabs_API_SAT_AGC_Options          (SILABS_FE_Context *front_end, unsigned char* agcOptions)
{
  front_end      = front_end;        /* To avoid compiler warning if not used */
  agcOptions     = agcOptions;       /* To avoid compiler warning if not used */
  safe_sprintf(agcOptions, "%s", "");
  safe_sprintf(agcOptions, "%s", "(Si2164B AGC options)\n");
  safe_sprintf(agcOptions, "%s agc mode      (Unused:0x0, MP_A:0xa, MP_B:0xb, MP_C:0xc, MP_D:0xd)\n", agcOptions);
  safe_sprintf(agcOptions, "%s agc inversion (0: not_inverted, 1: inverted)\n", agcOptions);
  return 0x2164B;
}
/************************************************************************************************************************
  SiLabs_API_SAT_AGC function
  Use:        SAT tuner AGC configuration function
              Used to configure the SAT tuner AGC
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  agc1_mode:  where the AGC 1 PWM comes from.
  Parameter:  agc1_inversion: 0/1 to indicate if AGC 1 is inverted or not (depends on the SAT tuner and HW design)
  Parameter:  agc2_mode:  where the AGC 2 PWM comes from.
  Parameter:  agc2_inversion: 0/1 to indicate if AGC 2 is inverted or not (depends on the SAT tuner and HW design)
   Refer to the demodulator's code for possible values
************************************************************************************************************************/
signed int   SiLabs_API_SAT_AGC                  (SILABS_FE_Context *front_end, signed int agc1_mode, signed int agc1_inversion, signed int agc2_mode, signed int agc2_inversion)
{
  SiTRACE("API CALL CONFIG: SiLabs_API_SAT_AGC                  (front_end, 0x%x, %d, 0x%x, %d);\n", agc1_mode, agc1_inversion, agc2_mode, agc2_inversion);
  front_end->SAT_AGC_agc1_mode      = agc1_mode;
  front_end->SAT_AGC_agc1_inversion = agc1_inversion;
  front_end->SAT_AGC_agc2_mode      = agc2_mode;
  front_end->SAT_AGC_agc2_inversion = agc2_inversion;
 return SiDemod_SAT_AGC (front_end->SiDemod_FE, agc1_mode, agc1_inversion, agc2_mode, agc2_inversion);
}
/************************************************************************************************************************
  SiLabs_API_SAT_Spectrum function
  Use:        SAT spectrum inversion function
              Used to configure the SAT ZIF spectrum inversion
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  spectrum_inversion:   a flag indicating if the SAT signal appears inverted.
               possible values: 0(not_inverted), 1(inverted)
               This situation is possible if there is an I/Q swap in the HW design, for easier routing
               It is perfectly OK to do that to route easily, but the SW needs to be informed of this inversion.
               NB: The additional inversion added by Unicable equipement should not be taken into account here
                    The spectrum_inversion set here corresponds to the 'normal' mode
  Returns:    0 if no error, -1 if not implemented for the current demod
************************************************************************************************************************/
signed int   SiLabs_API_SAT_Spectrum             (SILABS_FE_Context *front_end, signed int spectrum_inversion)
{
  SiTRACE("API CALL CONFIG: SiLabs_API_SAT_Spectrum             (front_end, %d);\n", spectrum_inversion);
  front_end->SAT_Spectrum_spectrum_inversion = spectrum_inversion;
  SiTRACE("SiLabs_API_SAT_Spectrum not supporting the current chip '%d'/'0x%x'\n", front_end->chip, front_end->chip);
 return SiDemod_SAT_Spectrum(front_end->SiDemod_FE, spectrum_inversion);
}
/************************************************************************************************************************
  SiLabs_API_SAT_Possible_LNB_Chips function
  Use:        LNB controller information function
              Used to know which LNB controllers can be used (selected using SiLabs_API_Select_LNB_Chip)
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  lnbList, a string to store the list of LNB Controllers
************************************************************************************************************************/
signed int   SiLabs_API_SAT_Possible_LNB_Chips   (SILABS_FE_Context *front_end, unsigned char *lnbList)
{
  signed int i;
  i = 0;
  safe_sprintf(lnbList, "%s", "");
#ifdef    LNBH21_COMPATIBLE
  safe_sprintf(lnbList, "%s LNBH21", lnbList); i++;
#endif /* LNBH21_COMPATIBLE */
#ifdef    LNBH25_COMPATIBLE
  safe_sprintf(lnbList, "%s LNBH25", lnbList); i++;
#endif /* LNBH25_COMPATIBLE */
#ifdef    LNBH26_COMPATIBLE
  safe_sprintf(lnbList, "%s LNBH26", lnbList); i++;
#endif /* LNBH26_COMPATIBLE */
#ifdef    LNBH29_COMPATIBLE
  safe_sprintf(lnbList, "%s LNBH29", lnbList); i++;
#endif /* LNBH29_COMPATIBLE */
#ifdef    A8293_COMPATIBLE
  safe_sprintf(lnbList, "%s A8293", lnbList); i++;
#endif /* A8293_COMPATIBLE */
#ifdef    A8302_COMPATIBLE
  safe_sprintf(lnbList, "%s A8302", lnbList); i++;
#endif /* A8302_COMPATIBLE */
  front_end = front_end; /* To avoid compiler warning if not used */
  lnbList   = lnbList;   /* To avoid compiler warning if not used */
  i         = i;         /* To avoid compiler warning if not used */
  return 0;
}
/************************************************************************************************************************
  SiLabs_API_SAT_Select_LNB_Chip function
  Use:        LNB controller selection function
              Used to select the LNB controller chip
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  lnb_code, an signed int used to select the LNB Controller
  Parameter:  lnb_chip_address, an signed int used to select the LNB Controller's i2c address
************************************************************************************************************************/
signed int   SiLabs_API_SAT_Select_LNB_Chip      (SILABS_FE_Context *front_end, signed int lnb_code, signed int lnb_chip_address)
{
  SiTRACE("API CALL CONFIG: SiLabs_API_SAT_Select_LNB_Chip      (front_end, %d, 0x%02x);\n", lnb_code, lnb_chip_address);
  front_end->SAT_Select_LNB_Chip_lnb_code         = lnb_code;
  front_end->SAT_Select_LNB_Chip_lnb_chip_address = lnb_chip_address;
#ifdef    LNBH21_COMPATIBLE
  if (lnb_code               == 21 ) { front_end->lnb_chip = lnb_code; front_end->lnb_chip_address = lnb_chip_address; return front_end->lnb_chip;}
#endif /* LNBH21_COMPATIBLE */
#ifdef    LNBH25_COMPATIBLE
  if (lnb_code               == 25 ) { front_end->lnb_chip = lnb_code; front_end->lnb_chip_address = lnb_chip_address; return front_end->lnb_chip;}
#endif /* LNBH25_COMPATIBLE */
#ifdef    LNBH26_COMPATIBLE
  if (lnb_code               == 26 ) { front_end->lnb_chip = lnb_code; front_end->lnb_chip_address = lnb_chip_address; return front_end->lnb_chip;}
#endif /* LNBH26_COMPATIBLE */
#ifdef    LNBH29_COMPATIBLE
  if (lnb_code               == 29 ) { front_end->lnb_chip = lnb_code; front_end->lnb_chip_address = lnb_chip_address; return front_end->lnb_chip;}
#endif /* LNBH29_COMPATIBLE */
#ifdef    A8293_COMPATIBLE
  if (lnb_code               == 0xA8293 ) { front_end->lnb_chip = lnb_code; front_end->lnb_chip_address = lnb_chip_address; return front_end->lnb_chip;}
#endif /* A8293_COMPATIBLE */
#ifdef    A8302_COMPATIBLE
  if (lnb_code               == 0xA8302 ) { front_end->lnb_chip = lnb_code; front_end->lnb_chip_address = lnb_chip_address; return front_end->lnb_chip;}
#endif /* A8302_COMPATIBLE */
  return 0;
}
/************************************************************************************************************************
  SiLabs_API_SAT_LNB_Chip_Index function
  Use:        LNB controller index setting function
              Used to select the LNB controller chip index, when using a dual part
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  lnb_code, an int used to select the LNB Controller
  Parameter:  lnb_chip_address, an int used to select the LNB Controller's i2c address
************************************************************************************************************************/
int   SiLabs_API_SAT_LNB_Chip_Index       (SILABS_FE_Context *front_end, signed  int lnb_index)
{
  SiTRACE("API CALL CONFIG: SiLabs_API_SAT_LNB_Chip_Index       (front_end, %d);\n", lnb_index);
  front_end->SAT_Select_LNB_Chip_lnb_index        = lnb_index;
#ifdef    LNBH26_COMPATIBLE
  if (front_end->lnb_chip ==      26) { if (lnb_index > 0) { front_end->lnbh26->a_b = 1; } else { front_end->lnbh26->a_b = 0;}; return 1; }
#endif /* LNBH26_COMPATIBLE */
#ifdef    A8302_COMPATIBLE
  if (front_end->lnb_chip == 0xA8302) { return L1_A8302_Index(front_end->A8302, lnb_index  );}
#endif /* A8302_COMPATIBLE */
  return 0;
}
/* Front_end info, control and status functions                                          */
/************************************************************************************************************************
  SiLabs_API_Infos  function
  Use:        software information function
              Used to retrieve various information about the front-end code
  Parameter:  front_end, a pointer to the SILABS_FE_Context context to be initialized
  Parameter:  infoString, a text buffer to be filled with the information.
               It must be initialized by the caller with a minimal size of 1000.
  Return:     the length of the information string
************************************************************************************************************************/
signed int   SiLabs_API_Infos                    (SILABS_FE_Context *front_end, unsigned char *infoString)
{
  front_end = front_end; /* To avoid compiler warning */
  if (infoString       == NULL) return 0;
  if (Silabs_init_done ==    0) {
    safe_sprintf(infoString, "Call SiLabs_API_SW_Init first!\n");
  } else {
    SiDemod_Infos  (front_end->SiDemod_FE, infoString);
  }
  safe_sprintf(infoString, "%sWrapper              Source code %s\n"   , infoString, SiLabs_API_TAG_TEXT() );
#ifdef    LNBH21_COMPATIBLE
  safe_sprintf(infoString, "%sLNB CHIP             LNBH21 at 0x%02x\n" , infoString, front_end->lnbh21->i2c->address );
  safe_sprintf(infoString, "%sLNB CHIP             Source code %s\n"   , infoString, L1_LNBH21_TAG_Text() );
#endif /* LNBH21_COMPATIBLE */
#ifdef    LNBH25_COMPATIBLE
  safe_sprintf(infoString, "%sLNB POWER CHIP       LNBH25 at 0x%02x\n" , infoString, front_end->lnbh25->i2c->address );
  safe_sprintf(infoString, "%sLNB POWER CHIP       Source code %s\n"   , infoString, L1_LNBH25_TAG_Text() );
#endif /* LNBH25_COMPATIBLE */
#ifdef    LNBH26_COMPATIBLE
  safe_sprintf(infoString, "%sLNB POWER CHIP       LNBH26 at 0x%02x\n" , infoString, front_end->lnbh26->i2c->address );
  safe_sprintf(infoString, "%sLNB POWER CHIP       Source code %s\n"   , infoString, L1_LNBH26_TAG_Text() );
#endif /* LNBH26_COMPATIBLE */
#ifdef    LNBH29_COMPATIBLE
  safe_sprintf(infoString, "%sLNB POWER CHIP       LNBH29 at 0x%02x\n" , infoString, front_end->lnbh29->i2c->address );
  safe_sprintf(infoString, "%sLNB POWER CHIP       Source code %s\n"   , infoString, L1_LNBH29_TAG_Text() );
#endif /* LNBH29_COMPATIBLE */
#ifdef    A8293_COMPATIBLE
  safe_sprintf(infoString, "%sLNB POWER CHIP       A8293 at 0x%02x\n" , infoString, front_end->A8293->i2c->address );
  safe_sprintf(infoString, "%sLNB POWER CHIP       Source code %s\n"   , infoString, L1_A8293_TAG_Text() );
#endif /* A8293_COMPATIBLE */
#ifdef    A8302_COMPATIBLE
  safe_sprintf(infoString, "%sLNB POWER CHIP       A8302 at 0x%02x\n" , infoString, front_end->A8302->i2c->address );
  safe_sprintf(infoString, "%sLNB POWER CHIP       Source code %s\n"   , infoString, L1_A8302_TAG_Text() );
#endif /* A8302_COMPATIBLE */
  safe_sprintf(infoString, "%s--------------------------------------\n", infoString);

  return strlen((char*)infoString);
}
/************************************************************************************************************************
  SiLabs_API_Config_Infos  function
  Use:        software configuration information function
              Used to retrieve information about the front-end configuration
  Parameter:  front_end, a pointer to the SILABS_FE_Context context to be initialized
  Parameter:  config_function, the name of the configuration function
  Parameter:  infoString, a text buffer to be filled with the information.
               It must be initialized by the caller with a minimal size of 1000.
  Return:     the length of the information string
************************************************************************************************************************/
signed int   SiLabs_API_Config_Infos             (SILABS_FE_Context *front_end, unsigned char* config_function, unsigned char *infoString)
{
  signed int   match;
  unsigned char  tmp_info_string_buffer[1000];
  unsigned char *tmp_info_string;

  if (infoString       == NULL) return 0;
  if (Silabs_init_done ==    0) {
    safe_sprintf(infoString, "Call SiLabs_API_SW_Init first!\n");
    return strlen((char*)infoString);
  }
  match = 0;
  tmp_info_string = &(tmp_info_string_buffer[0]);
  safe_sprintf(infoString, " %-24s ("   , infoString, config_function);
  if (strcmp_nocase(config_function, (unsigned char*)""           ) ==0) {
    match ++; safe_sprintf(infoString, "0x%x"              , infoString, front_end->chip );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"Demod_Address"           ) ==0) {
    match ++; safe_sprintf(infoString, "0x%02x"            , infoString, front_end->demod_add );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"SPI_Setup"               ) ==0) {
    match ++; safe_sprintf(infoString,"%d, %d, %d, %d, %d", infoString, front_end->SPI_Setup_send_option, front_end->SPI_Setup_clk_pin, front_end->SPI_Setup_clk_pola, front_end->SPI_Setup_data_pin, front_end->SPI_Setup_data_order );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"Select_TER_Tuner"        ) ==0) {
    match ++; safe_sprintf(infoString,"0x%x, %d"          , infoString, front_end->Select_TER_Tuner_ter_tuner_code, front_end->Select_TER_Tuner_ter_tuner_index );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"TER_tuner_I2C_connection") ==0) {
    match ++; safe_sprintf(infoString,"%d"                , infoString, front_end->TER_tuner_I2C_connection );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"TER_Address"             ) ==0) {
    match ++; safe_sprintf(infoString,"0x%02x"            , infoString, front_end->TER_Address_add );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"TER_Tuner_ClockConfig"   ) ==0) {
    match ++; safe_sprintf(infoString,"%d, %d"            , infoString, front_end->TER_Tuner_ClockConfig_xtal, front_end->TER_Tuner_ClockConfig_xout );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"TER_Clock"               ) ==0) {
    match ++; safe_sprintf(infoString,"%d, %d, %d, %d"    , infoString, front_end->TER_Clock_clock_source, front_end->TER_Clock_clock_input, front_end->TER_Clock_clock_freq, front_end->TER_Clock_clock_control );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"TER_FEF_Config"          ) ==0) {
    match ++; safe_sprintf(infoString,"%d, %d, %d"        , infoString, front_end->TER_FEF_Config_fef_mode, front_end->TER_FEF_Config_fef_pin, front_end->TER_FEF_Config_fef_level );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"TER_AGC"                 ) ==0) {
    match ++; safe_sprintf(infoString,"0x%x, %d, 0x%x, %d", infoString, front_end->TER_AGC_agc1_mode, front_end->TER_AGC_agc1_inversion, front_end->TER_AGC_agc2_mode, front_end->TER_AGC_agc2_inversion );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"TER_Tuner_AGC_Input"     ) ==0) {
    match ++; safe_sprintf(infoString,"%d"                , infoString, front_end->TER_Tuner_AGC_Input_dtv_agc_source );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"TER_Tuner_FEF_Input"     ) ==0) {
    match ++; safe_sprintf(infoString,"%d"                , infoString, front_end->TER_Tuner_FEF_Input_dtv_fef_freeze_input );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"TER_Tuner_IF_Output"     ) ==0) {
    match ++; safe_sprintf(infoString,"%d"                , infoString, front_end->TER_Tuner_IF_Output_dtv_out_type );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"Select_SAT_Tuner"        ) ==0) {
    match ++; safe_sprintf(infoString,"0x%x, %d"          , infoString, front_end->Select_SAT_Tuner_sat_tuner_code, front_end->Select_SAT_Tuner_sat_tuner_index );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"SAT_tuner_I2C_connection") ==0) {
    match ++; safe_sprintf(infoString,"%d"                , infoString, front_end->SAT_tuner_I2C_connection );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"SAT_Address"             ) ==0) {
    match ++; safe_sprintf(infoString,"0x%02x"            , infoString, front_end->SAT_Address_add );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"SAT_Clock"               ) ==0) {
    match ++; safe_sprintf(infoString,"%d, %d, %d, %d"    , infoString, front_end->SAT_Clock_clock_source, front_end->SAT_Clock_clock_input, front_end->SAT_Clock_clock_freq, front_end->SAT_Clock_clock_control );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"SAT_Spectrum"            ) ==0) {
    match ++; safe_sprintf(infoString,"%d"                , infoString, infoString, front_end->SAT_Spectrum_spectrum_inversion );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"SAT_AGC"                 ) ==0) {
    match ++; safe_sprintf(infoString,"0x%x, %d, 0x%x, %d", infoString, front_end->SAT_AGC_agc1_mode, front_end->SAT_AGC_agc1_inversion, front_end->SAT_AGC_agc2_mode, front_end->SAT_AGC_agc2_inversion );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"SAT_Select_LNB_Chip"     ) ==0) {
    match ++; safe_sprintf(infoString,"%d, 0x%02x"        , infoString, front_end->SAT_Select_LNB_Chip_lnb_code, front_end->SAT_Select_LNB_Chip_lnb_chip_address );
  }
  if (strcmp_nocase(config_function, (unsigned char*)"Set_Index_and_Tag"       ) ==0) {
    match ++; safe_sprintf(infoString,"%d, '%s'"        , infoString, front_end->fe_index, front_end->tag );
  }
  if (match) {
    safe_sprintf(infoString, ");\n");
  } else {
    safe_sprintf(infoString, "ERROR: unknown '%s' config_function !\n" , config_function);
  }
  if (strcmp_nocase((unsigned char*)config_function, (unsigned char*)"Full"                    ) ==0) {
    safe_sprintf(infoString, "%s", "");
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"Frontend_Chip"           , tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"Demod_Address"           , tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"SPI_Setup"               , tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"Select_TER_Tuner"        , tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"TER_tuner_I2C_connection", tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"TER_Address"             , tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"TER_Tuner_ClockConfig"   , tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"TER_Clock"               , tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"TER_FEF_Config"          , tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"TER_AGC"                 , tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"TER_Tuner_IF_Output"     , tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"TER_Tuner_IF_Output"     , tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"Select_SAT_Tuner"        , tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"SAT_tuner_I2C_connection", tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"SAT_Address"             , tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"SAT_Clock"               , tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"SAT_Spectrum"            , tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"SAT_AGC"                 , tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"SAT_Select_LNB_Chip"     , tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    SiLabs_API_Config_Infos (front_end, (unsigned char*)"Set_Index_and_Tag"       , tmp_info_string ); safe_sprintf(infoString, "%s", infoString, tmp_info_string );
    safe_sprintf(infoString,"%s","\n");
  }
  return strlen((char*)infoString);
}
/************************************************************************************************************************
  SiLabs_API_HW_Connect function
  Use:        hardware connection function
              Used to connect the HW in the selected mode
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  connection_mode, the required mode
  Porting:    This function may be removed in the final application. It is useful during development to allow connection in various modes
************************************************************************************************************************/
unsigned char  SiLabs_API_HW_Connect               (SILABS_FE_Context *front_end, CONNECTION_TYPE connection_mode)
{
  SiTRACE("API CALL CONFIG: SiLabs_API_HW_Connect           (front_end, %d);\n", connection_mode);
  SiDemod_HW_Connect (front_end->SiDemod_FE, connection_mode);
  L0_Connect(WrapperI2C,    connection_mode);
#ifdef    UNICABLE_COMPATIBLE
  L0_Connect(front_end->unicable->i2c, connection_mode);
#endif /* UNICABLE_COMPATIBLE */
#ifdef    LNBH21_COMPATIBLE
  if (front_end->lnb_chip == 21) { L0_Connect(front_end->lnbh21->i2c, connection_mode); }
#endif /* LNBH21_COMPATIBLE */
#ifdef    LNBH25_COMPATIBLE
  if (front_end->lnb_chip == 25) { L0_Connect(front_end->lnbh25->i2c, connection_mode); }
#endif /* LNBH25_COMPATIBLE */
#ifdef    LNBH26_COMPATIBLE
  if (front_end->lnb_chip == 26) { L0_Connect(front_end->lnbh26->i2c, connection_mode); }
#endif /* LNBH26_COMPATIBLE */
#ifdef    LNBH29_COMPATIBLE
  if (front_end->lnb_chip == 29) { L0_Connect(front_end->lnbh29->i2c, connection_mode); }
#endif /* LNBH29_COMPATIBLE */
#ifdef    A8293_COMPATIBLE
  if (front_end->lnb_chip == 0xA8293) { L0_Connect(front_end->A8293->i2c, connection_mode); }
#endif /* A8293_COMPATIBLE */
#ifdef    A8302_COMPATIBLE
  if (front_end->lnb_chip == 0xA8302) { L0_Connect(front_end->A8302->i2c, connection_mode); }
#endif /* A8302_COMPATIBLE */
  SiTRACE("SiLabs_API_HW_Connect in mode %d\n", connection_mode);
  return 1;
}
/************************************************************************************************************************
  SiLabs_API_ReadString function
  Use:        low-level i2c read function
              Used to easily read i2c bytes from the wrapper
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  readString, the string to be used
  Parameter:  pbtDataBuffer, a buffer to stor ethe read bytes
  Behavior:   Split the input string in tokens (separated by 'space') and read the corresponding bytes
              The chip address is the first byte
              The number of bytes to read is equal to the last token
              The index size if the number of tokens - 2
  Example:    'SiLabs_API_ReadString (front_end, "0xC8 0x12 0x34 5", pbtDataBuffer);' will read 5 bytes from the chip at address 0xc8 starting from index 0x1234
               0xC8 0x12 0x34  5
                |   ---------  |
                |       |      ----> 5 bytes to read
                |       -----------> index 0x1234 (on 2 bytes)
                -------------------> address = 0xc8
  Example:    'SiLabs_API_ReadString (front_end, "0xA0 0x00 5"     , pbtDataBuffer);' will read 5 bytes from the chip at address 0xA0 starting from index 0x00 (This will return the content of the Cypress eeprom)
  Example:    'SiLabs_API_ReadString (front_end, "0xC0 1"          , pbtDataBuffer);' will read 1 byte  from the chip at address 0xC0 (This will return the status byte for a command-mode chip)
  Returns:    The number of bytes written
************************************************************************************************************************/
signed int   SiLabs_API_ReadString               (SILABS_FE_Context *front_end, unsigned char *readString, unsigned char *pbtDataBuffer)
{
    front_end = front_end; /* To avoid compiler warning */

#if(!SEC_T2_TO_CHECK)

  return L0_ReadString (WrapperI2C, readString, pbtDataBuffer);
#else
  readString=readString;
  pbtDataBuffer= pbtDataBuffer;

  return 0;
#endif
}
/************************************************************************************************************************
  SiLabs_API_WriteString function
  Use:        low_level i2c write function
              Used to easily write bytes from the wrapper, based on a string input
  Behavior:   Split the input string in tokens (separated by 'space') and write the corresponding bytes
              The chip address is the first byte
              The number of bytes to write is equal to the number of tokens -1
  Example:    'SiLabs_API_WriteString (front_end, "0xC8 0x12 0x34 0x37");' will write '0x37' at index 0x1234 in the chip at address 0xc8
  Returns:    The number of bytes written
************************************************************************************************************************/
unsigned char  SiLabs_API_WriteString              (SILABS_FE_Context *front_end, unsigned char *writeString)
{
    front_end = front_end; /* To avoid compiler warning */

#if(!SEC_T2_TO_CHECK)
  return L0_WriteString(WrapperI2C, writeString);
#else
    writeString=writeString;
  return 0;
#endif
  
}
/************************************************************************************************************************
  SiLabs_API_bytes_trace function
  Use:        Byte-level tracing function
              Used to toggle the L0 traces
  Parameter:  front_end, a pointer to the SILABS_FE_Context context
  Parameter:  track_mode, the required trace mode
  Porting:    This function may be removed in the final application. It is useful during development to check i2c communication
  Returns:    The final track_mode
************************************************************************************************************************/
signed char  SiLabs_API_bytes_trace              (SILABS_FE_Context *front_end, unsigned char track_mode)
{
  SiTRACE("API CALL TRACE: SiLabs_API_bytes_trace (front_end, %d);\n", track_mode);
  front_end->SiDemod_FE->demod->i2c->trackWrite     = track_mode;
  front_end->SiDemod_FE->demod->i2c->trackRead      = track_mode;
  front_end->SiDemod_FE->tuner_ter->i2c->trackWrite = track_mode;
  front_end->SiDemod_FE->tuner_ter->i2c->trackRead  = track_mode;
  front_end->SiDemod_FE->tuner_sat->i2c->trackWrite = track_mode;
  front_end->SiDemod_FE->tuner_sat->i2c->trackRead  = track_mode;
  WrapperI2C->trackWrite     = track_mode;
  WrapperI2C->trackRead      = track_mode;
#ifdef    LNBH21_COMPATIBLE
  front_end->lnbh21->i2c->trackWrite = track_mode;
  front_end->lnbh21->i2c->trackRead  = track_mode;
#endif /* LNBH21_COMPATIBLE */
#ifdef    LNBH25_COMPATIBLE
  front_end->lnbh25->i2c->trackWrite = track_mode;
  front_end->lnbh25->i2c->trackRead  = track_mode;
#endif /* LNBH25_COMPATIBLE */
#ifdef    LNBH26_COMPATIBLE
  front_end->lnbh26->i2c->trackWrite = track_mode;
  front_end->lnbh26->i2c->trackRead  = track_mode;
#endif /* LNBH26_COMPATIBLE */
#ifdef    LNBH29_COMPATIBLE
  front_end->lnbh29->i2c->trackWrite = track_mode;
  front_end->lnbh29->i2c->trackRead  = track_mode;
#endif /* LNBH29_COMPATIBLE */
#ifdef    A8293_COMPATIBLE
  front_end->A8293->i2c->trackWrite = track_mode;
  front_end->A8293->i2c->trackRead  = track_mode;
#endif /* A8293_COMPATIBLE */
#ifdef    A8302_COMPATIBLE
  front_end->A8302->i2c->trackWrite = track_mode;
  front_end->A8302->i2c->trackRead  = track_mode;
#endif /* A8302_COMPATIBLE */
#ifdef    UNICABLE_COMPATIBLE
  front_end->unicable->i2c->trackWrite = track_mode;
  front_end->unicable->i2c->trackRead  = track_mode;
#endif /* UNICABLE_COMPATIBLE */
  return track_mode;
}
/************************************************************************************************************************
  SiLabs_API_communication_check function
  Use:      Communication check function
            Used to make sure all chips are connected
  Parameter: front_end a pointer to the front-end structure
  Return:    1 if sucessful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_communication_check      (SILABS_FE_Context *front_end)
{
 return SiDemod_communication_check (front_end->SiDemod_FE);
}
/************************************************************************************************************************
  SiLabs_API_switch_to_standard function
  Use:      Standard switching function selection
            Used to switch nicely to the wanted standard, taking into account the previous state
  Parameter: new_standard the wanted standard to switch to
  Return:    1 if sucessful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_switch_to_standard       (SILABS_FE_Context *front_end, unsigned char standard, unsigned char force_full_init)
{
  front_end->init_ok = 0;
  SiTRACE("API CALL LOCK  : SiLabs_API_switch_to_standard standard %d, force_full_init %d);\n", standard, force_full_init);
  SiTRACE("Wrapper switching to %s\n", Silabs_Standard_Text(standard) );
  SiTRACE("SiLabs_API_switch_to_standard config_code 0x%06x\n", front_end->config_code);

  if((front_end->SiDemod_FE->tuner_ter->i2c->address == TER1_MAIN_ADDRESS) || (front_end->SiDemod_FE->demod->i2c->address == TER1_SUB_ADDRESS)) // for Dual T2CS2 & Dual T2C model 
  {
    if (Silabs_multiple_front_end_init_done == 0) {
      SiTRACE("SiLabs_API_switch_to_standard calling SiLabs_API_Demods_Kickstart to enable the clocks (only for multiple frontends)\n");
      Silabs_multiple_front_end_init_done = SiLabs_API_Demods_Kickstart ();
  #ifdef    SILABS_TER_TUNER_API
    /*  SiLabs_API_TER_Tuners_Kickstart (); */
  #endif /* SILABS_TER_TUNER_API */
    }
  }

  front_end->init_ok = SiDemod_switch_to_standard (front_end->SiDemod_FE , Silabs_standardCode(front_end, standard), force_full_init);
  if (front_end->init_ok == 0) {
    SiTRACE("Problem switching to %s\n", Silabs_Standard_Text(standard));
  } else {
    front_end->standard = standard;
  }
  return front_end->init_ok;
}
/************************************************************************************************************************
  SiLabs_API_set_standard function
  Use:      Standard switching function selection
            Used to change the current standard only
  Behavior:  WARNING: use with caution, this is only used for SAT, where the demod can lock in S2 when set to S (and vice-versa)!
  Parameter: new_standard the wanted standard to switch to
  Return:    1 if sucessful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_set_standard             (SILABS_FE_Context *front_end, unsigned char standard)
{
  SiTRACE("API CALL LOCK  : SiLabs_API_set_standard standard %d);\n", standard);
 if (SiDemod_switch_to_standard (front_end->SiDemod_FE,        Silabs_standardCode(front_end, (CUSTOM_Standard_Enum)standard), 0)) { front_end->standard = standard; return 1;}
  SiTRACE("Problem switching to %s\n", Silabs_Standard_Text(standard));
  return 0;
}
/************************************************************************************************************************
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
************************************************************************************************************************/
signed int   SiLabs_API_lock_to_carrier          (SILABS_FE_Context *front_end, unsigned char standard, signed int freq, signed int bandwidth_Hz, unsigned char stream, unsigned int symbol_rate_bps, unsigned char constellation, unsigned char polarization, unsigned char band, signed int data_slice_id, signed int plp_id, unsigned char T2_lock_mode)
{
  signed int standard_code;
  signed int constel_code;
  signed int stream_code;
  signed int voltage;
  signed int tone;
  data_slice_id = data_slice_id; /* to avoid compiler warning if not used */
  plp_id        = plp_id;        /* to avoid compiler warning if not used */

  //SiTRACE("API CALL LOCK  : SiLabs_API_lock_to_carrier (front_end, %8d, %d, %d, %2d, %9d, %8d, %10d, %4d, %4d, %4d, %4d);\n", standard, freq, bandwidth_Hz, stream, symbol_rate_bps, constellation, polarization, band, data_slice_id, plp_id, T2_lock_mode);
  //SiTRACE("SiLabs_API_lock_to_carrier config_code 0x%06x\n", front_end->config_code);

  printf("API CALL LOCK  : SiLabs_API_lock_to_carrier (front_end, %8d, %d, %d, %2d, %9d, %8d, %10d, %4d, %4d, %4d, %4d);\n", standard, freq, bandwidth_Hz, stream, symbol_rate_bps, constellation, polarization, band, data_slice_id, plp_id, T2_lock_mode);
  printf("SiLabs_API_lock_to_carrier config_code 0x%06x\n", front_end->config_code);


  standard_code = Silabs_standardCode(front_end, (CUSTOM_Standard_Enum)standard);
  constel_code  = Silabs_constelCode (front_end, (CUSTOM_Constel_Enum)constellation);
  stream_code   = Silabs_streamCode  (front_end, (CUSTOM_Stream_Enum)stream);

  //SiTRACE("SiLabs_API_lock_to_carrier (front_end, %8s, %d, %d, %2s, %9d, %8s, %10s, %4s, %4d, %4d, %4d)\n", Silabs_Standard_Text(standard), freq, bandwidth_Hz, Silabs_Stream_Text(stream), symbol_rate_bps, Silabs_Constel_Text(constellation), Silabs_Polarization_Text(polarization), Silabs_Band_Text(band), data_slice_id , plp_id , T2_lock_mode);
  //SiTRACE("SiLabs_API_lock_to_carrier (front_end, standard %8s, freq %d, bandwidth_Hz %d, stream %2s, symbol_rate_bps %9d, constellation %8s, polarization %10s, band %4s, data_slice_id %4d, plp_id %4d, T2_lock_mode %4d)\n",
  printf("SiLabs_API_lock_to_carrier (front_end, %8s, %d, %d, %2s, %9d, %8s, %10s, %4s, %4d, %4d, %4d)\n", Silabs_Standard_Text(standard), freq, bandwidth_Hz, Silabs_Stream_Text(stream), symbol_rate_bps, Silabs_Constel_Text(constellation), Silabs_Polarization_Text(polarization), Silabs_Band_Text(band), data_slice_id , plp_id , T2_lock_mode);
  printf("SiLabs_API_lock_to_carrier (front_end, standard %8s, freq %d, bandwidth_Hz %d, stream %2s, symbol_rate_bps %9d, constellation %8s, polarization %10s, band %4s, data_slice_id %4d, plp_id %4d, T2_lock_mode %4d)\n",
  
          Silabs_Standard_Text(standard), freq, bandwidth_Hz, Silabs_Stream_Text(stream), symbol_rate_bps, Silabs_Constel_Text(constellation), Silabs_Polarization_Text(polarization), Silabs_Band_Text(band), data_slice_id, plp_id, T2_lock_mode );

  /* Use the API wrapper function to switch standard if required. */
  if (standard != front_end->standard) {
    if (SiLabs_API_switch_to_standard(front_end, standard, 0) == 0) {
      return 0;
    }
  }
  if ( (standard == SILABS_SLEEP) || (standard == SILABS_ANALOG) ) {
    SiTRACE("SiLabs_API_lock_to_carrier %10s: nothing more to do, returning 1\n", Silabs_Standard_Text(standard) );
    return 1;
  }
  switch (standard) {
    case SILABS_DVB_S  :
    case SILABS_DVB_S2 :
    case SILABS_DSS    : {
      voltage = 0;
      tone    = 0;
      SiTRACE("%s %s\n", Silabs_Polarization_Text((CUSTOM_Polarization_Enum)polarization) ,Silabs_Band_Text((CUSTOM_Band_Enum)band) );
#ifdef    UNICABLE_COMPATIBLE
      if (front_end->unicable->installed) {
        if (polarization == SILABS_POLARIZATION_HORIZONTAL) { front_end->unicable->polarization = UNICABLE_HORIZONTAL; }
        if (polarization == SILABS_POLARIZATION_VERTICAL  ) { front_end->unicable->polarization = UNICABLE_VERTICAL  ; }
        if (band         == SILABS_BAND_LOW               ) { front_end->unicable->band         = UNICABLE_LOW_BAND  ; }
        if (band         == SILABS_BAND_HIGH              ) { front_end->unicable->band         = UNICABLE_HIGH_BAND ; }
      } else {
#endif /* UNICABLE_COMPATIBLE */
      if (polarization == SILABS_POLARIZATION_HORIZONTAL) { voltage = 13; }//csm,13'//kwon,14',c2,18
      if (polarization == SILABS_POLARIZATION_VERTICAL  ) { voltage = 18; }//csm,13'//kwon,14',c2,13
      if (band         == SILABS_BAND_LOW  ) { tone    =  0; }
      if (band         == SILABS_BAND_HIGH ) { tone    =  1; }
      //SiLabs_API_SAT_voltage_and_tone (front_end, voltage, tone);//csm,13'//kwon,14,c2,enable
#ifdef    UNICABLE_COMPATIBLE
      }
#endif /* UNICABLE_COMPATIBLE */
      break;
    }
    default            : { break; }
  }

  SiTRACE("Si2164B_lock_to_carrier  (front_end->SiDemod_FE, %d, %d, %d, %d, %d, %d, %d, %d, %d)\n", standard_code, freq, bandwidth_Hz, stream_code, symbol_rate_bps, constel_code, data_slice_id,plp_id, T2_lock_mode);
    return SiDemod_lock_to_carrier  (front_end->SiDemod_FE, standard_code, freq
                                  , bandwidth_Hz
                                  , stream_code
                                  , symbol_rate_bps
                                  , constel_code
                                  , data_slice_id
                                  , plp_id
                                  , T2_lock_mode
                                    );
}
/************************************************************************************************************************
  SiLabs_API_Tune function
  Use:      tuning function
            Used to tune on a channel for the current standard
  Parameter: freq                the frequency to lock to (in Hz for TER, in kHz for SAT)
  Return:    the tuned freq, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_Tune                     (SILABS_FE_Context *front_end, signed int freq)
{
  SiTRACE("API CALL LOCK  : SiLabs_API_Tune (front_end, %10d);\n", freq);
  return   SiDemod_Tune  (front_end->SiDemod_FE, freq );
}
/************************************************************************************************************************
  SiLabs_API_Demod_Standby function
  Use:      Demodulator standby function
            Used to switch the demodulator in standby mode
  Parameter: front_end, a pointer to the front End
  Returns:    1 if successful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_Demod_Standby            (SILABS_FE_Context *front_end)
{
	signed int ret=1;

	SiTRACE("API CALL LOCK  : SiLabs_API_Demod_Standby (front_end);\n");
	front_end = front_end; /* To avoid compiler warning */
	if(Si2164B_STANDBY(front_end->SiDemod_FE->demod) != NO_Si2164B_ERROR)
		ret = 0;//error

	return ret;
}
/************************************************************************************************************************
  SiLabs_API_Demod_Wakeup function
  Use:      Demodulator wakeup function
            Used to switch the demodulator in wakeup mode
  Parameter: front_end, a pointer to the front End
  Returns:    1 if successful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_Demod_Wakeup            (SILABS_FE_Context *front_end)
{
	signed int ret=1;

	SiTRACE("API CALL LOCK  : SiLabs_API_Demod_Wakeup (front_end);\n");
	front_end = front_end; /* To avoid compiler warning */
	if(Si2164B_WAKEUP(front_end->SiDemod_FE->demod) != NO_Si2164B_ERROR)
		ret = 0;//error

	return ret;
}
/************************************************************************************************************************
  SiLabs_API_Demod_Silent function
  Use:      Demodulator Silent function
            Used to switch the demodulator in Silent mode
  Parameter: front_end, a pointer to the front End
  Returns:    1 if successful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_Demod_Silent             (SILABS_FE_Context *front_end, signed int silent)
{
  SiTRACE("API CALL LOCK  : SiLabs_API_Demod_Silent (front_end, %d);\n", silent);
  front_end = front_end; /* To avoid compiler warning */

  Si2164B_L2_SILENT (front_end->SiDemod_FE, silent); 
  return 1;
}
/************************************************************************************************************************
  SiLabs_API_Demod_ClockOn function
  Use:      Demodulator clock activation function
            Used to switch the demodulator clock on
  Parameter: front_end, a pointer to the front End
  Returns:    1 if successful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_Demod_ClockOn            (SILABS_FE_Context *front_end)
{
  SiTRACE("API CALL LOCK  : SiLabs_API_Demod_ClockOn (front_end);\n");
  front_end = front_end; /* To avoid compiler warning */

  return 1;
}
/************************************************************************************************************************
  SiLabs_API_Reset_Uncorrs function
  Use:      uncorrectable packets counter reset function
            Used to reset the uncor counter
  Parameter: front_end, a pointer to the front End
  Returns:    1 if successful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_Reset_Uncorrs            (SILABS_FE_Context *front_end)
{
 Si2164B_L1_DD_UNCOR (front_end->SiDemod_FE->demod, Si2164B_DD_UNCOR_CMD_RST_CLEAR);
 return 1;
}
/************************************************************************************************************************
  SiLabs_API_TS_Mode function
  Use:      Transport Stream control function
            Used to switch the TS output in the desired mode
  Parameter: mode the mode to switch to
************************************************************************************************************************/
signed int   SiLabs_API_TS_Mode                  (SILABS_FE_Context *front_end, signed int ts_mode)
{
  signed int valid_mode;
#ifdef    USB_Capability //hjh cypress 관련함수에서 사용하는 double은 제거하지 못함 . 
  double        retdval;
  unsigned char rettxtBuffer[256];
  unsigned char *rettxt;
  rettxt = rettxtBuffer;
#endif /* USB_Capability */
  SiTRACE("API CALL LOCK  : SiLabs_API_TS_Mode (front_end, %d);\n", ts_mode);
  valid_mode = 0;
#ifdef    USB_Capability
  if (ts_mode != SILABS_TS_GPIF) { L0_Cypress_Configure("-gpif"     ,"off", 0   , &retdval, &rettxt);}
#endif /* USB_Capability */

    switch (ts_mode) {
      case SILABS_TS_SERIAL  : { front_end->SiDemod_FE->demod->prop->dd_ts_mode.mode = Si2164B_DD_TS_MODE_PROP_MODE_SERIAL  ; break; }
      case SILABS_TS_PARALLEL: { front_end->SiDemod_FE->demod->prop->dd_ts_mode.mode = Si2164B_DD_TS_MODE_PROP_MODE_PARALLEL; break; }
      case SILABS_TS_TRISTATE: { front_end->SiDemod_FE->demod->prop->dd_ts_mode.mode = Si2164B_DD_TS_MODE_PROP_MODE_TRISTATE; break; }
      case SILABS_TS_OFF     : { front_end->SiDemod_FE->demod->prop->dd_ts_mode.mode = Si2164B_DD_TS_MODE_PROP_MODE_OFF;      break; }
      case SILABS_TS_STREAMING: { break; }
      case SILABS_TS_GPIF    : { front_end->SiDemod_FE->demod->prop->dd_ts_setup_par.ts_data_shape = 7;
                                 front_end->SiDemod_FE->demod->prop->dd_ts_setup_par.ts_clk_shape  = 7;
                                 Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DD_TS_SETUP_PAR_PROP_CODE);
                                 front_end->SiDemod_FE->demod->prop->dd_ts_mode.clock = Si2164B_DD_TS_MODE_PROP_CLOCK_AUTO_ADAPT;
                                 front_end->SiDemod_FE->demod->prop->dd_ts_mode.mode = Si2164B_DD_TS_MODE_PROP_MODE_GPIF    ; break; }
      case SILABS_TS_SLAVE_FIFO:{front_end->SiDemod_FE->demod->prop->dd_ts_setup_par.ts_data_shape = 7;
                                 front_end->SiDemod_FE->demod->prop->dd_ts_setup_par.ts_clk_shape  = 7;
                                 front_end->SiDemod_FE->demod->prop->dd_ts_setup_par.ts_clk_invert   = Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_INVERT_INVERTED;
                                 Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod,  Si2164B_DD_TS_SETUP_PAR_PROP_CODE);
                                 front_end->SiDemod_FE->demod->prop->dd_ts_freq.req_freq_10khz     = 2000;
                                 Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod,  Si2164B_DD_TS_FREQ_PROP_CODE);
                                 SiLabs_API_Demod_reset(front_end);
                                 front_end->SiDemod_FE->demod->prop->dd_ts_mode.clock = Si2164B_DD_TS_MODE_PROP_CLOCK_MANUAL;
                                 front_end->SiDemod_FE->demod->prop->dd_ts_mode.mode  = Si2164B_DD_TS_MODE_PROP_MODE_PARALLEL; break; }
      default                : { return SiLabs_API_TS_Mode(front_end, SILABS_TS_TRISTATE)                                  ; break; }
    }
    SiTRACE("front_end->SiDemod_FE->demod->prop->dd_ts_mode.mode %d\n",front_end->SiDemod_FE->demod->prop->dd_ts_mode.mode);
    Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DD_TS_MODE_PROP_CODE);
    valid_mode = 1;
  //if (valid_mode) {//20131120,prevent tool
#ifdef    USB_Capability
    if (ts_mode == SILABS_TS_STREAMING ) {
      L0_Cypress_Configure("-gpif"                 ,"on"      ,    0,                       &retdval, &rettxt);  /* Starting Cypress gpif state machine   */
      L0_Cypress_Configure("-gpif_clk"             ,"on"      ,    0,                       &retdval, &rettxt);  /* Starting Cypress gpif clock           */
      L0_Cypress_Process  ("ts"                    ,"start"   ,    0,                       &retdval, &rettxt);  /* Starting Cypress TS transfer over USB */
      return ts_mode;
    }
    /* Checking previous mode to stop possible USB transfers */
    L0_Cypress_Cget ("-gpif"    , "", 0, &retdval, &rettxt);  /* retrieving Cypress gpif mode          */
    SiTRACE("L0_Cypress_Cget  (-gpif) %s\n", rettxt);
    if (ts_mode != SILABS_TS_GPIF       ) {
//      if (strcmp_nocase(rettxt, "on")==0 ) {
        SiTRACE("L0_Cypress_Process  (ts ,stop)\n");
        L0_Cypress_Process  ("ts"                  ,"stop"    ,    0,                       &retdval, &rettxt);  /* Stopping Cypress TS transfer over USB */
        L0_Cypress_Configure("-gpif"               ,"off"     ,    0,                       &retdval, &rettxt);  /* Stopping Cypress gpif state machine   */
        L0_Cypress_Configure("-gpif_clk"           ,"tristate",    0,                       &retdval, &rettxt);  /* Stopping Cypress gpif clock           */
//      }
    }
    if (ts_mode != SILABS_TS_SLAVE_FIFO ) {
      L0_Cypress_Cget ("-ts_slave", "", 0, &retdval, &rettxt);  /* retrieving Cypress ts slave fifo mode */
//      if (strcmp_nocase(rettxt, "on")==0 ) {
        L0_Cypress_Configure("-ts_slave"           ,"off"      ,    0,                      &retdval, &rettxt);  /* Starting Cypress ts slave fifo mode   */
//      }
      L0_Cypress_Cget ("-gpif"    , "", 0, &retdval, &rettxt);  /* retrieving Cypress gpif mode          */
//      if (strcmp_nocase(rettxt, "on")==0 ) {
        L0_Cypress_Process  ("ts"                  ,"stop"    ,    0,                       &retdval, &rettxt);  /* Stopping Cypress TS transfer over USB */
        L0_Cypress_Configure("-gpif"               ,"off"     ,    0,                       &retdval, &rettxt);  /* Stopping Cypress gpif state machine   */
        L0_Cypress_Configure("-gpif_clk"           ,"tristate",    0,                       &retdval, &rettxt);  /* Stopping Cypress gpif clock           */
//      }
    }
    if (ts_mode == SILABS_TS_GPIF       ) {
      L0_Cypress_Configure("-gpif"                 ,"on"      ,    0,                       &retdval, &rettxt);  /* Starting Cypress gpif state machine   */
      L0_Cypress_Configure("-gpif_clk"             ,"on"      ,    0,                       &retdval, &rettxt);  /* Starting Cypress gpif clock           */
      L0_Cypress_Process  ("ts"                    ,"start"   ,    0,                       &retdval, &rettxt);  /* Starting Cypress TS transfer over USB */
      L0_Cypress_Process  ("output_enable_io_port" ,"d"       , 0x0C,                       &retdval, &rettxt);  /* Starting Mux commands                 */
      //L0_Cypress_Process  ("write_io_port"         ,"d"       , front_end->ts_mux_input,    &retdval, &rettxt);  /* Select input of Mux for GPIF          */
    }
    if (ts_mode == SILABS_TS_SLAVE_FIFO ) {
//      L0_Cypress_Configure("-vendor_debug"         ,""        ,    1,                       &retdval, &rettxt);  /* Enabling vendor command tracing       */
//      L0_Cypress_Configure("-ts_slave"             ,"on"      ,    0,                       &retdval, &rettxt);  /* Starting Cypress ts slave fifo mode   */
      L0_Cypress_Process  ("ts"                    ,"start"   ,    0,                       &retdval, &rettxt);  /* Starting Cypress TS transfer over USB */
//      L0_Cypress_Process  ("output_enable_io_port" ,"a"       , 0x0B,                       &retdval, &rettxt);  /* Starting Mux commands                 */
//      L0_Cypress_Process  ("write_io_port"         ,"a"       , 0x09,                       &retdval, &rettxt);  /* Select input of Mux for GPIF          */
    }
#endif /* USB_Capability */
    if ( (ts_mode != SILABS_TS_TRISTATE) | (ts_mode != SILABS_TS_OFF) ) {
      front_end->active_TS_mode = ts_mode;
    }
    return ts_mode;
  //} else {//20131120,prevent tool
  //  return -1;
  //}
}
/************************************************************************************************************************
  SiLabs_API_Get_TS_Dividers function
  Use:      retrieving TS clock dividers function
            Used to retrieve the TS clock dividers
  Parameter: front_end, a pointer to the front End
  Parameter: div_a, a pointer to an unsigned int used to store the numerator value
  Parameter: div_b, a pointer to an unsigned int used to store the denominator value
  Returns:    0 if successful, 1 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_Get_TS_Dividers          (SILABS_FE_Context *front_end, unsigned int *div_a, unsigned int *div_b)
{
  SiTRACE("API CALL INFO : SiLabs_API_Get_TS_Dividers (front_end, &div_a, &div_a);\n");
  *div_a = *div_b = 0;
  if (Si2164B_L1_DEMOD_INFO (front_end->SiDemod_FE->demod) == NO_Si2164B_ERROR) {
    *div_a = front_end->SiDemod_FE->demod->rsp->demod_info.div_a;
    *div_b = front_end->SiDemod_FE->demod->rsp->demod_info.div_b;
    return 0;
  } else {
    SiERROR("Error retrieving TS dividers!\n");
    return ERROR_Si2164B_SENDING_COMMAND;
  }
  SiTRACE("SiLabs_API_Get_TS_Dividers not supporting the current chip '%d'/'0x%x'\n", front_end->chip, front_end->chip);
  return 0;
}
/************************************************************************************************************************
  NAME: Silabs_API_TS_Tone_Cancel
  DESCRIPTION: enables/disables TS spurs transparently
	When tone cancel is disabled DIV_A and DIV_B are set to 0
  Parameter: on_off , 1= on, 0= off
  Returns:    '0' if OK, any other value indicates an error
************************************************************************************************************************/
signed int   Silabs_API_TS_Tone_Cancel               (SILABS_FE_Context* front_end, signed int on_off)
{
    int return_code;
    unsigned int DIV_A;
    unsigned int DIV_B;
    front_end = front_end; /* to avoid compiler warning if not used */
    return_code = on_off + 1;

    DIV_A = DIV_B = 0;

    if (on_off == 1) {
      return_code = SiLabs_API_Get_TS_Dividers(front_end, &DIV_A, &DIV_B );
      if ( return_code != 0) {
        SiERROR("Error: Silabs_TS_Tone_Cancel can't get TS clock dividers from demod\n");
        return return_code;
      }
    }

    SiTRACE("Dividers for Tone canceller, DIV_A=%d, DIV_B=%d\n", DIV_A, DIV_B);
    SiLabs_API_Tuner_I2C_Enable (front_end);
    //return_code = Si2190B_L1_TUNER_CANCEL_TONE(front_end->SiDemod_FE->tuner_ter, DIV_A, DIV_B);
    if (return_code != 0) {
      SiERROR("SiLabs_TER_Tuner_Tone_Cancel failed !\n");
    }
    SiLabs_API_Tuner_I2C_Disable (front_end);
    return return_code;
}
/************************************************************************************************************************
  SiLabs_API_Tuner_I2C_Enable function
  Use:      Demod Loop through control function
            Used to switch the I2C loopthrough on, allowing commnication with the tuners
  Return:    the final mode (-1 if not known)
***************************.********************************************************************************************/
signed int   SiLabs_API_Tuner_I2C_Enable         (SILABS_FE_Context *front_end)
{
  SiTRACE("API CALL LOCK  : SiLabs_API_Tuner_I2C_Enable (front_end);\n");
 SiDemod_Tuner_I2C_Enable (front_end->SiDemod_FE);
 return 1;
}
/************************************************************************************************************************
  SiLabs_API_Tuner_I2C_Disable function
  Use:      Demod Loop through control function
            Used to switch the I2C loopthrough off, stopping commnication with the tuners
  Return:    the final mode (-1 if not known)
************************************************************************************************************************/
signed int   SiLabs_API_Tuner_I2C_Disable        (SILABS_FE_Context *front_end)
{
  SiTRACE("API CALL LOCK  : SiLabs_API_Tuner_I2C_Disable (front_end);\n");
 SiDemod_Tuner_I2C_Disable (front_end->SiDemod_FE);
 return 0;
}
/************************************************************************************************************************
  NAME: SiLabs_API_Channel_Seek_Init
  DESCRIPTION: logs the seek parameters in the context structure

  Scan algorithm based on the seek feature:

    SiLabs_API_switch_to_standard(front_end, standard, 0);

    SiLabs_API_Channel_Seek_Init (front_end, rangeMin, rangeMax,...);

    While ( SiLabs_API_Channel_Seek_Init (front_end, rangeMin, rangeMax,...) !=0 ) {
      SiLabs_API_TS_Mode (front_end, SILABS_TS_SERIAL/SILABS_TS_PARALLEL);

      the new carrier is at front_end->detected_rf with front_end->detected_sr
      look for PSI/SI information

      SiLabs_API_TS_Mode (front_end, SILABS_TS_TRISTATE);
    }

    SiLabs_API_Channel_Seek_End (front_end);

  Parameter:  Pointer to SILABS_FE_Context
  Parameter:  rangeMin starting Frequency (in Hz for TER, in kHz for SAT)
  Parameter:  rangeMax ending   Frequency (in Hz for TER, in kHz for SAT)
  Parameter:  minSRbps minimum SR to detect
  Parameter:  maxSRbps maximum SR to detect
  Parameter:  max RSSI dBm
  Parameter:  min RSSI dBm
  Parameter:  max RSSI dBm
  Parameter:  min SNR 1/2 dB
  Parameter:  max SNR 1/2 dB
  Returns:    0 if successful, otherwise an error.
************************************************************************************************************************/
signed int   SiLabs_API_Channel_Seek_Init        (SILABS_FE_Context *front_end,
                                            signed int rangeMin,     signed int rangeMax,
                                            signed int seekBWHz,     signed int seekStepHz,
                                            signed int minSRbps,     signed int maxSRbps,
                                            signed int minRSSIdBm,   signed int maxRSSIdBm,
                                            signed int minSNRHalfdB, signed int maxSNRHalfdB)
{
  SiTRACE("API CALL SEEK  : SiLabs_API_Channel_Seek_Init (front_end, rangeMin %d, rangeMax %d, seekBWHz %d, seekStepHz %d, minSRbps %d, maxSRbps %d, minRSSIdBm %d, maxRSSIdBm %d, minSNRHalfdB %d, maxSNRHalfdB %d);\n", rangeMin, rangeMax, seekBWHz, seekStepHz, minSRbps, maxSRbps, minRSSIdBm, maxRSSIdBm, minSNRHalfdB, maxSNRHalfdB);
 return SiDemod_Channel_Seek_Init (front_end->SiDemod_FE,  rangeMin, rangeMax, seekBWHz, seekStepHz, minSRbps, maxSRbps, minRSSIdBm, maxRSSIdBm, minSNRHalfdB, maxSNRHalfdB);
}
/************************************************************************************************************************
  NAME: SiLabs_API_Channel_Seek_Next
  DESCRIPTION: Looks for the next channel, starting from the last detected channel
  Parameter:  Pointer to SILABS_FE_Context
  Returns:    1 if channel is found, 0 if abort or end of range, any other value is the handshake duration
************************************************************************************************************************/
signed int   SiLabs_API_Channel_Seek_Next        (SILABS_FE_Context *front_end, signed int *standard, signed int *freq, signed int *bandwidth_Hz, signed int *stream, unsigned int *symbol_rate_bps, signed int *constellation, signed int *polarization, signed int *band, signed int *num_data_slice, signed int *num_plp, signed int *T2_base_lite)
{
  signed int seek_result;
  num_data_slice = num_data_slice;/* to avoid compiler warning if not used */
  num_plp        = num_plp;       /* to avoid compiler warning if not used */
  bandwidth_Hz   = bandwidth_Hz;  /* to avoid compiler warning if not used */
  polarization   = polarization;  /* to avoid compiler warning if not used */
  band           = band;          /* to avoid compiler warning if not used */
  *num_data_slice = 0;            /* set to '0' by default (to avoid processing DS   for standards not supporting this feature) */
  *num_plp        = 0;            /* set to '0' by default (to avoid processing MPLP for standards not supporting this feature) */
  *T2_base_lite   = 0;            /* set to '0' by default (to avoid processing T2 lite for standards not supporting this feature) */
  seek_result     = -1;
  SiTRACE("API CALL SEEK  : SiLabs_API_Channel_Seek_Next (front_end, &standard, &freq, &bandwidth_Hz, &stream, &symbol_rate_bps, &constellation, &polarization, &band, &num_data_slice, &num_plp, &T2_base_lite);\n");
  SiTRACE("SiLabs_API_Channel_Seek_Next config_code 0x%06x\n", front_end->config_code);

      seek_result = SiDemod_Channel_Seek_Next (front_end->SiDemod_FE, standard, freq
                                           , bandwidth_Hz
                                           , stream
                                           , symbol_rate_bps
                                           , constellation
                                           , num_data_slice
                                           , num_plp
                                           , T2_base_lite
                                           );
  *polarization = front_end->polarization;
  *band         = front_end->band;
  if (seek_result ==  1) {
    *standard = Custom_standardCode(front_end, *standard);
    front_end->standard = *standard;
    /* Translate demod-specific values to CUSTOM values */
    switch (*standard)
    {
      case SILABS_DVB_T : {
        *stream        = Custom_streamCode (front_end, *stream);
        break;
      }
      case SILABS_DVB_C : {
        *constellation = Custom_constelCode(front_end, *constellation);
        break;
      }
      default           : {
        break;
      }
    }
    return 1;
  }
  if (seek_result >   1) {
    return seek_result;
  }
  if (seek_result == -1) {
    SiTRACE("Chip '%d' not handled by SiLabs_API_Channel_Seek_Next\n", front_end->chip);
  }
  return 0;
}
/************************************************************************************************************************
  NAME: SiLabs_API_Channel_Seek_Abort
  DESCRIPTION: aborts the channel seek for the next channel
  Parameter:  Pointer to SILABS_FE_Context
  Returns:    1 if successful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_Channel_Seek_Abort       (SILABS_FE_Context *front_end)
{
  SiTRACE("API CALL SEEK  : SiLabs_API_Channel_Seek_Abort (front_end);\n");
 return SiDemod_Channel_Seek_Abort (front_end->SiDemod_FE);
}
/************************************************************************************************************************
  NAME: SiLabs_API_Channel_Lock_Abort
  DESCRIPTION: aborts the channel lock for the current channel
  Parameter:  Pointer to SILABS_FE_Context
  Returns:    1 if successful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_Channel_Lock_Abort       (SILABS_FE_Context *front_end)
{
  SiTRACE("API CALL LOCK  : SiLabs_API_Channel_Lock_Abort (front_end);\n");
 return SiDemod_Channel_Lock_Abort (front_end->SiDemod_FE);
}
/************************************************************************************************************************
  NAME: SiLabs_API_Channel_Seek_End
  DESCRIPTION: returns the chip back to normal following a seek
  Parameter:  Pointer to SILABS_FE_Context
  Returns:    1 if successful, 0 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_Channel_Seek_End         (SILABS_FE_Context *front_end)
{
  SiTRACE("API CALL SEEK  : SiLabs_API_Channel_Seek_End (front_end);\n");
  return SiDemod_Channel_Seek_End (front_end->SiDemod_FE);
}
/************************************************************************************************************************
  NAME: SiLabs_API_SAT_AutoDetectCheck
  DESCRIPTION: check function for the SAT auto detect mode
  Parameter:  Pointer to SILABS_FE_Context
  Returns:    -1 if error, 0 if not locked on SAT, otherwise the current SAT standard code,
************************************************************************************************************************/
signed int   SiLabs_API_SAT_AutoDetectCheck      (SILABS_FE_Context *front_end)
{
  signed int detected;
  SiTRACE("API CALL SEEK  : SiLabs_API_SAT_AutoDetectCheck (front_end);\n");
  front_end = front_end; /* To avoid compiler warning if not used */
  detected = -1;
  //kwon, 13
  if (SiDemod_L1_DVB_S_S2_get_lock_in_AFC_range(front_end->SiDemod_FE->demod) == 0) {
      return 0;
  }
  //end
#if 0//KWON,13 dual,DISABLE
    if (Si2164B_L1_DD_STATUS (front_end->SiDemod_FE->demod, Si2164B_DD_STATUS_CMD_INTACK_OK) != NO_SiDemod_ERROR) {
      SiERROR("Si2164B_L1_DD_STATUS ERROR\n");
      return 0;
    }
    if (front_end->SiDemod_FE->demod->rsp->dd_status.dl == 0) {return 0;}
#endif
    detected = Custom_standardCode(front_end, front_end->SiDemod_FE->demod->rsp->dd_status.modulation);
    switch ( detected ) {
      case SILABS_DVB_S :
      case SILABS_DVB_S2: { return detected; break; }
      default:            { return       0 ; break; }
    }
  return -1;
}
/************************************************************************************************************************
  NAME: SiLabs_API_SAT_AutoDetect
  DESCRIPTION: activation function for the SAT auto detect mode
  Parameter: front_end, Pointer to SILABS_FE_Context
  Parameter: on_off, which is set to '0' to de-activate the SAT auto-detect feature,
              set to '1' to activate it and to any other value to retrieve the current status
  Returns:    the current state of auto_detect_SAT
************************************************************************************************************************/
signed int   SiLabs_API_SAT_AutoDetect           (SILABS_FE_Context *front_end, signed int on_off)
{
  front_end = front_end; /* To avoid compiler warning if not used */
  on_off    = on_off;    /* To avoid compiler warning if not used */
    if (on_off == 0) { Si2164B_SatAutoDetectOff(front_end->SiDemod_FE); }
    if (on_off == 1) { Si2164B_SatAutoDetect   (front_end->SiDemod_FE); }
    return front_end->SiDemod_FE->auto_detect_SAT;
  return -1;
}
/************************************************************************************************************************
  NAME: SiLabs_API_SAT_Tuner_Init
  DESCRIPTION: initialization function for the SAT tuner
  Parameter:  Pointer to SILABS_FE_Context
  Returns:    0 if error, 1 if success
************************************************************************************************************************/
signed int   SiLabs_API_SAT_Tuner_Init           (SILABS_FE_Context *front_end)
{
    SiTRACE("SiLabs_API_SAT_Tuner_Init: Init satellite tuner\n");

	SiLabs_API_Tuner_I2C_Enable (front_end);//20130819,csm
	L1_RF_SAT_TUNER_InitAfterReset(front_end->SiDemod_FE->tuner_sat);
	SiTRACE("Satellite tuner Version: 0x%02x\n",front_end->SiDemod_FE->tuner_sat->version);
	SiLabs_API_Tuner_I2C_Disable (front_end);//20130819,csm

  return 1;
}

signed int   SiLabs_API_SAT_Tuner_STANDBY           (SILABS_FE_Context *front_end)
{
    SiTRACE("SiLabs_API_SAT_Tuner_STANDBY\n");

	SiLabs_API_Tuner_I2C_Enable (front_end);//20130819,csm
	L1_RF_SAT_TUNER_STANDBY(front_end->SiDemod_FE->tuner_sat);
	SiTRACE("Satellite tuner Version: 0x%02x\n",front_end->SiDemod_FE->tuner_sat->version);
	SiLabs_API_Tuner_I2C_Disable (front_end);//20130819,csm

  return 1;
}

signed int   SiLabs_API_SAT_Tuner_WAKEUP           (SILABS_FE_Context *front_end)
{
    SiTRACE("SiLabs_API_SAT_Tuner_WAKEUP\n");

	SiLabs_API_Tuner_I2C_Enable (front_end);
	L1_RF_SAT_TUNER_WAKEUP(front_end->SiDemod_FE->tuner_sat);
	SiLabs_API_Tuner_I2C_Disable (front_end);

  return 1;
}

/************************************************************************************************************************
  NAME: SiLabs_API_SAT_Tuner_SetLPF
  DESCRIPTION: sets the SAT tuner low pass filter
  Parameter:  Pointer to SILABS_FE_Context
  Parameter:  lpf_khz, the lowpass filter (in khz)
  Returns:    the final lpf value
************************************************************************************************************************/
signed int   SiLabs_API_SAT_Tuner_SetLPF         (SILABS_FE_Context *front_end, signed int lpf_khz)
{
 return SiDemod_SAT_LPF (front_end->SiDemod_FE , lpf_khz);
  return 0;
}
//csm,13'
/************************************************************************************************************************
  NAME: SiLabs_API_SAT_Tuner_PLLlock
  DESCRIPTION: gets the SAT tuner PLL Lock
  Parameter:  Pointer to SILABS_FE_Context
  Returns:    PLL lock
************************************************************************************************************************/
signed int   SiLabs_API_SAT_Tuner_PLLlock         (SILABS_FE_Context *front_end)
{
	signed int pll_lock=0;

	SiLabs_API_Tuner_I2C_Enable (front_end);

	{ pll_lock = L1_RF_SAT_TUNER_PLLlock (front_end->SiDemod_FE->tuner_sat); }

	SiLabs_API_Tuner_I2C_Disable (front_end);
	
	return pll_lock;
}
//end,13'

//kwon, 14 quad
/************************************************************************************************************************
  NAME: SiLabs_API_SAT_Tuner_RfSel
  DESCRIPTION: selects the SAT tuner RF
  Parameter:  Pointer to SILABS_FE_Context
  Returns:     0 if error, 1 if success
************************************************************************************************************************/
signed int   SiLabs_API_SAT_Tuner_RfSel           (SILABS_FE_Context *front_end, unsigned char ch_sel)
{
	SiLabs_API_Tuner_I2C_Enable (front_end);//20130819,csm
	L1_RF_SAT_TUNER_RfSel(front_end->SiDemod_FE->tuner_sat, ch_sel);
	SiLabs_API_Tuner_I2C_Disable (front_end);//20130819,csm

	return 1;
}
//end

/************************************************************************************************************************
  NAME: SiLabs_API_SAT_Tuner_RfSel_Read
  DESCRIPTION: Read the SAT tuner RF sel value
  Parameter:  Pointer to SILABS_FE_Context
              same_side/diff_side, '1' is on, '0' is off 
  Returns:     0 if error, 1 if success
************************************************************************************************************************/
signed int   SiLabs_API_SAT_Tuner_RfSel_Read           (SILABS_FE_Context *front_end, unsigned char *same_side, unsigned char *diff_side)
{
	SiLabs_API_Tuner_I2C_Enable (front_end);//20130819,csm
	L1_RF_SAT_TUNER_RfSel_Read(front_end->SiDemod_FE->tuner_sat, same_side, diff_side);
	SiLabs_API_Tuner_I2C_Disable (front_end);//20130819,csm

	return 1;
}

/************************************************************************************************************************
  NAME: SiLabs_API_SAT_voltage
  DESCRIPTION: sets the LNB supply voltage
  Parameter:  Pointer to SILABS_FE_Context
  Parameter:  voltage, the LNB voltage level (allows 0, 13 or 18)
  Returns:    1 if successful, -1 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_SAT_voltage              (SILABS_FE_Context *front_end, signed int  voltage)
{
  front_end=front_end;  /*to avoid compile warning*/
  voltage=voltage;      /*to avoid compile warning*/

  SiTRACE("SiLabs_API_SAT_voltage voltage %d, front_end->lnb_chip %d\n", voltage, front_end->lnb_chip );
#ifdef    LNBH21_COMPATIBLE
  if (front_end->lnb_chip ==     21 ) { L1_LNBH21_Voltage(front_end->lnbh21, voltage  ); return 1;}
#endif /* LNBH21_COMPATIBLE */
#ifdef    LNBH25_COMPATIBLE
  if (front_end->lnb_chip ==     25 ) {
    if (front_end->lnb_chip_init_done == 0) { front_end->lnb_chip_init_done = L1_LNBH25_InitAfterReset(front_end->lnbh25); }
    L1_LNBH25_Voltage(front_end->lnbh25, voltage ); return 1;
  }
#endif /* LNBH25_COMPATIBLE */
#ifdef    LNBH26_COMPATIBLE
  if (front_end->lnb_chip ==     26 ) {
    if (front_end->lnb_chip_init_done == 0) { front_end->lnb_chip_init_done = L1_LNBH26_InitAfterReset(front_end->lnbh26); }
    L1_LNBH26_Voltage(front_end->lnbh26, voltage ); return 1;
  }
#endif /* LNBH26_COMPATIBLE */
#ifdef    LNBH29_COMPATIBLE
  if (front_end->lnb_chip ==     29 ) {
    if (front_end->lnb_chip_init_done == 0) { front_end->lnb_chip_init_done = L1_LNBH29_InitAfterReset(front_end->lnbh29); }
    L1_LNBH29_Voltage(front_end->lnbh29, voltage ); return 1;
  }
#endif /* LNBH29_COMPATIBLE */
#ifdef    A8293_COMPATIBLE
  if (front_end->lnb_chip == 0xA8293) { L1_A8293_Voltage(front_end->A8293, voltage  ); return 1;}
#endif /* A8293_COMPATIBLE */
#ifdef    A8302_COMPATIBLE
  if (front_end->lnb_chip == 0xA8302) { L1_A8302_Voltage(front_end->A8302, voltage  ); return 1;}
#endif /* A8302_COMPATIBLE */
  return -1;
}
/************************************************************************************************************************
  NAME: SiLabs_API_SAT_and_tone
  DESCRIPTION: sets the LNB 22kHz tone
  Parameter:  Pointer to SILABS_FE_Context
  Parameter:  tone, a flag to enable the 22khz tone (allows 0 or 1)
  Returns:    1 if successful, -1 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_SAT_tone                 (SILABS_FE_Context *front_end, unsigned char tone)
{
  signed int cont_tone;
  unsigned char  diseqBuffer[1];

  SiTRACE("SiLabs_API_SAT_tone, tone %d\n", tone);
  switch (tone   ) {
    case  1: cont_tone = 1; break;
    default: cont_tone = 0; break;
  }
  diseqBuffer[0] = 0x00;
  SiDemod_send_diseqc_sequence (front_end->SiDemod_FE , 0, &diseqBuffer[0], cont_tone, 0, 0, 1);
  return 1;
}
/************************************************************************************************************************
  NAME: SiLabs_API_SAT_voltage_and_tone
  DESCRIPTION: sets the LNB supply voltage and 22kHz tone
  Parameter:  Pointer to SILABS_FE_Context
  Parameter:  voltage, the LNB voltage level (allows 0, 13 or 18)
  Parameter:  tone, a flag to enable the 22khz tone (allows 0 or 1)
  Returns:    1 if successful, -1 otherwise
************************************************************************************************************************/
signed int   SiLabs_API_SAT_voltage_and_tone     (SILABS_FE_Context *front_end, signed int  voltage, unsigned char tone)
{
  SiTRACE("SiLabs_API_SAT_voltage_and_tone voltage %d, tone %d front_end->lnb_chip %d\n", voltage, tone, front_end->lnb_chip );
  if (SiLabs_API_SAT_voltage (front_end, voltage) == -1) {
    SiTRACE("SiLabs_API_SAT_voltage not supporting the current LNB controller (%d)!\n", front_end->lnb_chip);
    SiERROR("SiLabs_API_SAT_voltage not supporting the current LNB controller\n");
    return -1;
  }
  if (SiLabs_API_SAT_tone    (front_end, tone   ) == -1) {
    SiTRACE("SiLabs_API_SAT_tone not supporting the current demodulator (%d)!\n", front_end->chip);
    SiERROR("SiLabs_API_SAT_tone not supporting the current demodulator!\n");
    return -1;
  }
  return 1;
}
/************************************************************************************************************************
  NAME: SiLabs_API_SAT_prepare_diseqc_sequence
  DESCRIPTION: prepare a DiSEqC sequence to be sent on the SAT tuner input
  Parameter:  Pointer to SILABS_FE_Context
  Parameter:  sequence_length, the number of DiSEqC bytes to send
  Parameter:  sequence_buffer, a pointer to the DiSEqC bytes to send
  Parameter:  cont_tone, a flag indicating if the 22khz tone is to be sent
  Parameter:  tone_burst, a flag indicating if a 22khz tone needs to be sent around each DiSEqC message
  Parameter:  burst_sel, a flag indicating which satellite input is addressed by the DiSEqC message
  Parameter:  end_seq, a flag indicating if the current sequence is the last sequence in a series of DiSEqC messages
  Returns:    a value potentially used to call the SiLabs_API_SAT_trigger_diseqc_sequence
************************************************************************************************************************/
signed int   SiLabs_API_SAT_prepare_diseqc_sequence (SILABS_FE_Context *front_end, signed int sequence_length, unsigned char *sequence_buffer, unsigned char cont_tone, unsigned char tone_burst, unsigned char burst_sel, unsigned char end_seq, signed int *flags)
{
  front_end       = front_end;
  sequence_length = sequence_length;
  sequence_buffer = sequence_buffer;
  cont_tone       = cont_tone;
  tone_burst      = tone_burst;
  burst_sel       = burst_sel;
  end_seq         = end_seq;
  *flags          = 0;
 if (SiDemod_prepare_diseqc_sequence(front_end->SiDemod_FE, sequence_length, sequence_buffer, cont_tone, tone_burst, burst_sel, end_seq) != NO_SiDemod_ERROR ) {*flags = 0;} else {*flags = 1;}
 return 1;
}
/************************************************************************************************************************
  NAME: SiLabs_API_SAT_trigger_diseqc_sequence
  DESCRIPTION: send a DiSEqC sequence prepared using SiLabs_API_SAT_prepare_diseqc_sequence on the SAT tuner input
  Parameter:  flags, a value potentially used to call the SiLabs_API_SAT_trigger_diseqc_sequence
  Returns:    0
************************************************************************************************************************/
signed int   SiLabs_API_SAT_trigger_diseqc_sequence (SILABS_FE_Context *front_end, signed int flags)
{
  front_end       = front_end;
  flags           = flags;
  SiDemod_trigger_diseqc_sequence(front_end->SiDemod_FE); return 1;
  return 0;
}
/************************************************************************************************************************
  NAME: SiLabs_API_SAT_send_diseqc_sequence
  DESCRIPTION: sends a DiSEqC sequence on the SAT tuner input
  Parameter:  Pointer to SILABS_FE_Context
  Parameter:  sequence_length, the number of DiSEqC bytes to send
  Parameter:  sequence_buffer, a pointer to the DiSEqC bytes to send
  Parameter:  cont_tone, a flag indicating if the 22khz tone is to be sent
  Parameter:  tone_burst, a flag indicating if a 22khz tone needs to be sent around each DiSEqC message
  Parameter:  burst_sel, a flag indicating which satellite input is addressed by the DiSEqC message
  Parameter:  end_seq, a flag indicating if the current sequence is the last sequence in a series of DiSEqC messages
  Returns:    1 if no error
************************************************************************************************************************/
signed int   SiLabs_API_SAT_send_diseqc_sequence (SILABS_FE_Context *front_end, signed int sequence_length, unsigned char *sequence_buffer, unsigned char cont_tone, unsigned char tone_burst, unsigned char burst_sel, unsigned char end_seq)
{
  SiDemod_send_diseqc_sequence(front_end->SiDemod_FE, sequence_length, sequence_buffer, cont_tone, tone_burst, burst_sel, end_seq);
  return 1;
}
/************************************************************************************************************************
  NAME: SiLabs_API_SAT_read_diseqc_reply
  DESCRIPTION: reads a DiSEqC sequence from the SAT tuner input
  Parameter:  Pointer to SILABS_FE_Context
  Parameter:  lpf_khz, the lowpass filter (in khz)
  Parameter:  reply_length, a pointer to the number of DiSEqC bytes read
  Parameter:  reply_buffer, a pointer to store the DiSEqC bytes read
  Returns:    1 if no error
************************************************************************************************************************/
signed int   SiLabs_API_SAT_read_diseqc_reply    (SILABS_FE_Context *front_end, signed int *reply_length  , unsigned char *reply_buffer   )
{
  SiDemod_read_diseqc_reply (front_end->SiDemod_FE, reply_length, reply_buffer);
  return 1;
}
signed int   SiLabs_API_SAT_Tuner_Tune           (SILABS_FE_Context *front_end, signed int freq_kHz)
{
  SiTRACE("SiLabs_API_SAT_Tuner_Tune %d\n", freq_kHz);
  SiLabs_API_Tuner_I2C_Enable(front_end);
  L1_RF_SAT_TUNER_Tune(front_end->SiDemod_FE->tuner_sat, freq_kHz);
  SiLabs_API_Tuner_I2C_Disable(front_end);
  return 1;
}
signed int   SiLabs_API_SAT_Get_AGC              (SILABS_FE_Context *front_end)
{
  signed int agc;
  agc = -1;

front_end=front_end;  /*to avoid compile warning*/
  
#ifdef    Si2164B_COMPATIBLE
    if (front_end->chip ==   0x2164B ) {
      Si2164B_L1_DD_EXT_AGC_SAT (front_end->SiDemod_FE->demod,
                                Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MODE_NO_CHANGE, 0,
                                Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MODE_NO_CHANGE, 0,
                                0, 0, 0, 0);
      agc =  front_end->SiDemod_FE->demod->rsp->dd_ext_agc_sat.agc_1_level;
    }
#endif /* Si2164B_COMPATIBLE */
  return agc;
}
/************************************************************************************************************************
  NAME: SiLabs_API_SAT_set_diseqc_freq
  DESCRIPTION: set DiSEqC frequency
  Parameter:  Pointer to SILABS_FE_Context
  Parameter:  diseqc_freq_hz, When set to 0, envelope mode is selected, otherwise sets the frequency in Hz
  Returns:    1 if no error
************************************************************************************************************************/
signed int   SiLabs_API_SAT_set_diseqc_freq  (SILABS_FE_Context *front_end, unsigned int diseqc_freq_hz)
{

	front_end->SiDemod_FE->demod->prop->dd_diseqc_freq.freq_hz =  diseqc_freq_hz;

	Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DD_DISEQC_FREQ_PROP_CODE);

	return 1;
}

/************************************************************************************************************************
  NAME: SiLabs_API_Setup_Sat_TS_SLR_SERIAL
  DESCRIPTION: set Satellite TS SLR SERIAL
  Parameter:  Pointer to SILABS_FE_Context
  Parameter: SR     the symbol rate in baud/s   (for SAT)
  Returns:    1 if no error
************************************************************************************************************************/
signed int   SiLabs_API_Setup_Sat_TS_SLR_SERIAL  (SILABS_FE_Context *front_end, signed int SR)
{
	if((front_end->standard == SILABS_DVB_S2) && (SR > 32000000))
	{
		front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_data_slr                                     =     0; /* (default     0) */
		front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_data_slr_on                                  = Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_DISABLE  ; /* (default 'DISABLE') */
		front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_data1_slr                                    =     0; /* (default     0) */
		front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_data1_slr_on                                 = Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_DISABLE ; /* (default 'DISABLE') */
		front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_data2_slr                                    =     0; /* (default     0) */
		front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_data2_slr_on                                 = Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_DISABLE ; /* (default 'DISABLE') */
		front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_clk_slr                                      =     0; /* (default     0) */
		front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_clk_slr_on                                   = Si2164B_DD_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_DISABLE   ; /* (default 'DISABLE') */
		SiTRACE("SiLabs_API_Setup_Sat_TS_SLR_SERIAL SLR 0\n");

		front_end->SiDemod_FE->demod->prop->dd_ts_setup_ser.ts_data_strength = 10;
		front_end->SiDemod_FE->demod->prop->dd_ts_setup_ser.ts_data_shape = 0;
		front_end->SiDemod_FE->demod->prop->dd_ts_setup_ser.ts_clk_strength = 10;
		front_end->SiDemod_FE->demod->prop->dd_ts_setup_ser.ts_clk_shape = 0;
	}
	else
	{
		front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_data_slr                                     =     1; /* (default     0) */
		front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_data_slr_on                                  = 1  ; /* (default 'DISABLE') */
		front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_data1_slr                                    =     1; /* (default     0) */
		front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_data1_slr_on                                 = 1 ; /* (default 'DISABLE') */
		front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_data2_slr                                    =     1; /* (default     0) */
		front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_data2_slr_on                                 = 1 ; /* (default 'DISABLE') */
		front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_clk_slr                                      =     1; /* (default     0) */
		front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_clk_slr_on                                   = 1   ; /* (default 'DISABLE') */
		SiTRACE("SiLabs_API_Setup_Sat_TS_SLR_SERIAL SLR 1\n");

		front_end->SiDemod_FE->demod->prop->dd_ts_setup_ser.ts_data_strength = 15;
		front_end->SiDemod_FE->demod->prop->dd_ts_setup_ser.ts_data_shape = 0;
		front_end->SiDemod_FE->demod->prop->dd_ts_setup_ser.ts_clk_strength = 15;
		front_end->SiDemod_FE->demod->prop->dd_ts_setup_ser.ts_clk_shape = 0;
	}

	if (Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DD_TS_SLR_SERIAL_PROP_CODE) != NO_Si2164B_ERROR)
	{
		SiTRACE("SATELLITE: Si2164B_DD_TS_SLR_SERIAL_PROP failed !\n");
		return 0;
	} else {
		SiTRACE("SATELLITE: Si2164B_DD_TS_SLR_SERIAL_PROP complete !\n");
	}

	Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DD_TS_SETUP_SER_PROP_CODE);

	return 1;
}

//csm,13'
signed int   SiLabs_API_Setup_Sat_AFC_range  (SILABS_FE_Context *front_end, signed int SR, unsigned char fullscan)
{
	if((fullscan == 1) || (SR < 20000000))
	{
		front_end->SiDemod_FE->demod->prop->dvbs_afc_range.range_khz =  5000;
		front_end->SiDemod_FE->demod->prop->dvbs2_afc_range.range_khz = 5000;
	}
	else
	{
		front_end->SiDemod_FE->demod->prop->dvbs_afc_range.range_khz =  11000;
		front_end->SiDemod_FE->demod->prop->dvbs2_afc_range.range_khz = 11000;	
	}


	{
		Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DVBS_AFC_RANGE_PROP_CODE);
		Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DVBS2_AFC_RANGE_PROP_CODE);	
	} 

	return 1;
  
}
//end,13'
#ifdef    UNICABLE_COMPATIBLE
signed int   SiLabs_API_SAT_Unicable_Install     (SILABS_FE_Context *front_end)
{
  signed int install_res;
  install_res = SiLabs_Unicable_API_Install(front_end->unicable);

  if (install_res > 0) {
    front_end->lnb_type = UNICABLE_LNB_TYPE_UNICABLE;
    SiTRACE("front_end->lnb_type = UNICABLE_LNB_TYPE_UNICABLE;\n");
  }
  front_end->SiDemod_FE->lnb_type = front_end->lnb_type;
  front_end->SiDemod_FE->demod->prop->dd_diseqc_param.sequence_mode = Si2164B_DD_DISEQC_PARAM_PROP_SEQUENCE_MODE_NO_GAP;
  Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DD_DISEQC_PARAM_PROP_CODE);
  return install_res;
}
signed int   SiLabs_API_SAT_Unicable_Uninstall   (SILABS_FE_Context *front_end)
{
  front_end->lnb_type = UNICABLE_LNB_TYPE_NORMAL;
  SiTRACE("front_end->lnb_type = UNICABLE_LNB_TYPE_NORMAL;\n");
  front_end->SiDemod_FE->lnb_type = front_end->lnb_type;
  front_end->SiDemod_FE->demod->prop->dd_diseqc_param.sequence_mode = Si2164B_DD_DISEQC_PARAM_PROP_SEQUENCE_MODE_GAP;
  Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DD_DISEQC_PARAM_PROP_CODE);
  front_end->unicable->installed = 0;
  return 0;
}
signed int   SiLabs_API_SAT_Unicable_Tune        (SILABS_FE_Context *front_end, signed int freq_kHz)
{
  signed int L_freq;
  L_freq = SiLabs_Unicable_API_Tune(front_end->unicable, freq_kHz);
    front_end->SiDemod_FE->tuner_sat->RF = L_freq;
  return L_freq;
}
#endif /* UNICABLE_COMPATIBLE */
/************************************************************************************************************************
  SiLabs_API_SAT_Tuner_I2C_Enable function
  Use:      Demod Loop through control function,
            Used to switch the I2C loopthrough on, allowing communication with the SAT tuner
            This function can control the I2C passthrough for any front-end in the front-end table,
             and is useful mainly in multi-front-end applications with dual tuners or dual demodulators,
             when the SAT tuner I2C is not directly connected to the corresponding demodulator.
  Return:    the final mode (-1 if not known)
************************************************************************************************************************/
signed int   SiLabs_API_SAT_Tuner_I2C_Enable     (SILABS_FE_Context *front_end)
{
#ifdef    INDIRECT_I2C_CONNECTION
  signed int fe;
  signed int requester;
  signed int connecter;
  signed int fe_count;
  fe_count = FRONT_END_COUNT;
  for (fe=0; fe< fe_count; fe++) {
    if ( front_end  == &(FrontEnd_Table[fe]) ) {
      requester = fe;
      connecter = front_end->SAT_tuner_I2C_connection;
      if (connecter == 100) {
        /* 100 is a special value to allow having tuners connected directly.                                   */
        /* If tuners are connected directly, the i2c pass-through should NOT be closed, to avoid i2c deadlock. */
        return 1;
      }
      SiTRACE("-- I2C -- SiLabs_API_SAT_Tuner_I2C_Enable  request for front_end %d via front_end %d\n", requester, connecter);
      if (connecter < fe_count) {
        if (requester != connecter) {
      SiTRACE("-- I2C -- Enabling  indirect SAT tuner connection  for front_end %d via front_end %d\n", requester, connecter);
        }
        return SiLabs_API_Tuner_I2C_Enable(&(FrontEnd_Table[connecter]) );
      }
      break;
    }
  }
  SiTRACE("-- I2C -- SiLabs_API_SAT_Tuner_I2C_Enable  request failed! Unable to find a match for the caller front_end! (0x%08x)\n", (signed int)front_end);
  SiERROR("-- I2C -- SiLabs_API_SAT_Tuner_I2C_Enable  request failed! Unable to find a match for the caller front_end!\n");
  return 0;
#endif /* INDIRECT_I2C_CONNECTION */
  return SiLabs_API_Tuner_I2C_Enable(front_end);
}
/************************************************************************************************************************
  SiLabs_API_SAT_Tuner_I2C_Disable function
  Use:      Demod Loop through control function,
            Used to switch the I2C loopthrough off, disabling communication with the SAT tuner
            This function can control the I2C passthrough for any front-end in the front-end table,
             and is useful mainly in multi-front-end applications with dual tuners or dual demodulators,
             when the SAT tuner I2C is not directly connected to the corresponding demodulator.
  Return:    the final mode (-1 if not known)
************************************************************************************************************************/
signed int   SiLabs_API_SAT_Tuner_I2C_Disable    (SILABS_FE_Context *front_end)
{
#ifdef    INDIRECT_I2C_CONNECTION
  signed int fe;
  signed int requester;
  signed int connecter;
  signed int fe_count;
  fe_count = FRONT_END_COUNT;
  for (fe=0; fe< fe_count; fe++) {
    if ( front_end  == &(FrontEnd_Table[fe]) ) {
      requester = fe;
      connecter = front_end->SAT_tuner_I2C_connection;
      if (connecter == 100) {
        /* 100 is a special value to allow having tuners connected directly.                                   */
        /* If tuners are connected directly, the i2c pass-through should NOT be closed, to avoid i2c deadlock. */
        return 1;
      }
      SiTRACE("-- I2C --SiLabs_API_SAT_Tuner_I2C_Disable request for front_end %d via front_end %d\n", requester, connecter);
      if (connecter < fe_count) {
        if (requester != connecter) {
      SiTRACE("-- I2C -- Disabling indirect TER tuner connection for front_end %d via front_end %d\n", requester, connecter);
        }
        return SiLabs_API_Tuner_I2C_Disable(&(FrontEnd_Table[connecter]) );
      }
      break;
    }
  }
  SiTRACE("-- I2C -- SiLabs_API_SAT_Tuner_I2C_Disable request failed! Unable to find a match for the caller front_end! (0x%08x)\n", (signed int)front_end);
  SiERROR("-- I2C -- SiLabs_API_SAT_Tuner_I2C_Disable request failed! Unable to find a match for the caller front_end!\n");
  return 0;
#endif /* INDIRECT_I2C_CONNECTION */
  return SiLabs_API_Tuner_I2C_Disable(front_end);
}
/************************************************************************************************************************
  NAME: SiLabs_API_Get_Stream_Info
  DESCRIPTION: Stream info function for DVB-S2X
  Parameter: front_end, Pointer to SILABS_FE_Context
  Parameter:  isi_index, the data slice id containing the plp (if applicable, initially only used for DVB-C2),
  Parameter:  isi_index, the index of the required ISI,
  Parameter: *isi_id, a pointer to the isi id,
  Parameter: *isi_constellation, a pointer to the isi constellation,
  Parameter: *isi_code_rate, a pointer to the isi code_rate,
  Returns:    0 if no error value, -1 in case of an error
************************************************************************************************************************/
int   SiLabs_API_Get_Stream_Info          (SILABS_FE_Context *front_end, int  isi_index, int *isi_id, int *isi_constellation, int *isi_code_rate )
{
  front_end         = front_end        ; /* To avoid compiler warning if not used */
  isi_index         = isi_index        ; /* To avoid compiler warning if not used */
  isi_id            = isi_id           ; /* To avoid compiler warning if not used */
  isi_constellation = isi_constellation; /* To avoid compiler warning if not used */
  isi_code_rate     = isi_code_rate    ; /* To avoid compiler warning if not used */

    if (front_end->standard == SILABS_DVB_S2) {
      if (Si2164B_L1_DVBS2_STREAM_INFO(front_end->SiDemod_FE->demod, isi_index) != NO_Si2164B_ERROR) {
        SiTRACE("Si2164B_L1_DVBS2_STREAM_INFO error when checking ISI index %d!\n", isi_index);
        SiERROR("Si2164B_L1_DVBS2_STREAM_INFO error!\n");
        return -1;
      }
      *isi_id            = front_end->SiDemod_FE->demod->rsp->dvbs2_stream_info.isi_id;
      *isi_constellation = front_end->SiDemod_FE->demod->rsp->dvbs2_stream_info.isi_constellation;
      *isi_code_rate     = front_end->SiDemod_FE->demod->rsp->dvbs2_stream_info.isi_code_rate;
    }
    return 0;
}
/************************************************************************************************************************
  NAME: SiLabs_API_Select_Stream
  DESCRIPTION: Stream selection function for DVB-S2X
  Parameter: front_end, Pointer to SILABS_FE_Context
  Parameter:  stream_id, the STREAM id of the required ISI (must be lower than num_is for the selected carrier)
                Use '-1' for auto selection,
  Returns:    0 if no error value, -1 in case of an error
************************************************************************************************************************/
int   SiLabs_API_Select_Stream            (SILABS_FE_Context *front_end, signed int stream_id)
{
  int stream_sel_mode;
  SiTRACE("API CALL LOCK  : SiLabs_API_Select_Stream (front_end, %d);\n", stream_id);
  front_end       = front_end;        /* To avoid compiler warning if not used */
  stream_id       = stream_id   ; /* To avoid compiler warning if not used */
  stream_sel_mode = 0;

    if (stream_id < 0) {
      stream_sel_mode = Si2164B_DVBS2_STREAM_SELECT_CMD_STREAM_SEL_MODE_AUTO;
    } else {
      stream_sel_mode = Si2164B_DVBS2_STREAM_SELECT_CMD_STREAM_SEL_MODE_MANUAL;
    }
    if (Si2164B_L1_DVBS2_STREAM_SELECT(front_end->SiDemod_FE->demod, stream_id, stream_sel_mode) != NO_Si2164B_ERROR) {
      SiTRACE("Si2164B_L1_DVBS2_STREAM_SELECT error when selecting PLP %d!\n", stream_id);
      SiERROR("Si2164B_L1_DVBS2_STREAM_SELECT error!\n");
      return -1;
    }
/*    if (stream_sel_mode == Si2164B_DVBS2_STREAM_SELECT_CMD_STREAM_SEL_MODE_AUTO) { Si2164B_L1_DD_RESTART(front_end->SiDemod_FE->demod); system_wait(300); }*/
    return 0;

}
/************************************************************************************************************************
  NAME: SiLabs_API_Get_DVBT_Hierarchy
  DESCRIPTION: Hierarchy info function for DVB-T
  Parameter: front_end, Pointer to SILABS_FE_Context
  Parameter: *hierarchy, a pointer to the hierarchy,
  Returns:    0 if no error value, -1 in case of an error
************************************************************************************************************************/
signed int   SiLabs_API_Get_DVBT_Hierarchy       (SILABS_FE_Context *front_end, signed int *hierarchy)
{
  front_end = front_end;        /* To avoid compiler warning if not used */
  hierarchy = hierarchy;        /* To avoid compiler warning if not used */
    if (Si2164B_L1_DVBT_STATUS(front_end->SiDemod_FE->demod, Si2164B_DVBT_STATUS_CMD_INTACK_OK) != NO_SiDemod_ERROR) {
      SiTRACE("Si2164B_L1_DVBT_STATUS error when checking DVB-T hierarchy!\n");
      SiERROR("Si2164B_L1_DVBT_STATUS error!\n");
      return -1;
    }
    *hierarchy   = Custom_hierarchyCode(front_end, front_end->SiDemod_FE->demod->rsp->dvbt_status.hierarchy);
    return 0;
  //return -1;
}
/************************************************************************************************************************
  NAME: SiLabs_API_Get_DS_ID_Num_PLP_Freq
  DESCRIPTION: Dataslice ID, Num PLP and Frequency info function for DVB-C2.
             In a C2 system the initial parameter is then number of dataslices.
             For each dataslice (selected by its index), this function retrieves the
  Parameter: front_end, Pointer to SILABS_FE_Context
  Parameter:  ds_index, the index of the data slice in the C2 system,
  Parameter: *data_slice_id, a pointer to the dataslice Id,
  Parameter: *num_plp, a pointer to the number of PLPs in the dataslice,
  Parameter: *freq, a pointer to the dataslice frequency, in Hz,
  Returns:    0 if no error value, -1 in case of an error
************************************************************************************************************************/
signed int   SiLabs_API_Get_DS_ID_Num_PLP_Freq   (SILABS_FE_Context *front_end, signed int ds_index, signed int *ds_id, signed int *num_plp, signed int *freq_hz)
{
  front_end = front_end; /* To avoid compiler warning if not used */
  ds_index  = ds_index ; /* To avoid compiler warning if not used */
  ds_id     = ds_id    ; /* To avoid compiler warning if not used */
  num_plp   = num_plp  ; /* To avoid compiler warning if not used */
  freq_hz   = freq_hz  ; /* To avoid compiler warning if not used */

    if (front_end->standard == SILABS_DVB_C2) {
      if (Si2164B_L1_DVBC2_DS_INFO(front_end->SiDemod_FE->demod, ds_index, Si2164B_DVBC2_DS_INFO_CMD_DS_SELECT_INDEX_OR_ID_INDEX) != NO_SiDemod_ERROR) {
        SiTRACE("Si2164B_L1_DVBC2_DS_INFO error when checking DS index %d!\n", ds_index);
        SiERROR("Si2164B_L1_DVBC2_DS_INFO error!\n");
        return -1;
      }
      *ds_id    = front_end->SiDemod_FE->demod->rsp->dvbc2_ds_info.ds_id;
      *num_plp  = front_end->SiDemod_FE->demod->rsp->dvbc2_ds_info.dslice_num_plp;
      *freq_hz  = front_end->SiDemod_FE->demod->rsp->dvbc2_ds_info.dslice_tune_pos_hz;
    }
    SiTRACE("DS index %3d: DS ID %3d, Num PLP %3d, Freq %d Hz\n", ds_index, *ds_id, *num_plp, *freq_hz);
    return 0;
}
/************************************************************************************************************************
  NAME: SiLabs_API_Get_PLP_ID_and_TYPE
  DESCRIPTION: PLP ID and TYPE info function for DVB-T2 and DVB-C2
  Parameter: front_end, Pointer to SILABS_FE_Context
  Parameter:  ds_id, the data slice id containing the plp (if applicable, initially only used for DVB-C2),
  Parameter:  plp_index, the index of the required PLP (must be lower than num_plp for the selected carrier),
  Parameter: *plp_id, a pointer to the plp id,
  Parameter: *plp_type, a pointer to the plp type,
  Returns:    0 if no error value, -1 in case of an error
************************************************************************************************************************/
signed int   SiLabs_API_Get_PLP_ID_and_TYPE      (SILABS_FE_Context *front_end, signed int  ds_id, signed int plp_index, signed int *plp_id, signed int *plp_type)
{
  front_end = front_end; /* To avoid compiler warning if not used */
  plp_index = plp_index; /* To avoid compiler warning if not used */
  ds_id     = ds_id    ; /* To avoid compiler warning if not used */
  plp_id    = plp_id   ; /* To avoid compiler warning if not used */
  plp_type  = plp_type ; /* To avoid compiler warning if not used */

    if (front_end->standard == SILABS_DVB_C2) {
      if (Si2164B_L1_DVBC2_PLP_INFO(front_end->SiDemod_FE->demod, plp_index, Si2164B_DVBC2_PLP_INFO_CMD_PLP_INFO_DS_MODE_ANY, ds_id) != NO_SiDemod_ERROR) {
        SiTRACE("Si2164B_L1_DVBC2_PLP_INFO error when checking PLP index %d!\n", plp_index);
        SiERROR("Si2164B_L1_DVBC2_PLP_INFO error!\n");
        return -1;
      }
      *plp_id   = front_end->SiDemod_FE->demod->rsp->dvbc2_plp_info.plp_id;
      *plp_type = front_end->SiDemod_FE->demod->rsp->dvbc2_plp_info.plp_type;
    }
    if (front_end->standard == SILABS_DVB_T2) {
    if (Si2164B_L1_DVBT2_PLP_INFO(front_end->SiDemod_FE->demod, plp_index) != NO_SiDemod_ERROR) {
      SiTRACE("Si2164B_L1_DVBT2_PLP_INFO error when checking PLP index %d!\n", plp_index);
      SiERROR("Si2164B_L1_DVBT2_PLP_INFO error!\n");
      return -1;
    }
    *plp_id   = front_end->SiDemod_FE->demod->rsp->dvbt2_plp_info.plp_id;
    *plp_type = front_end->SiDemod_FE->demod->rsp->dvbt2_plp_info.plp_type;
    }
    SiTRACE("PLP index %3d: PLP ID %3d, PLP TYPE %d\n", plp_index, *plp_id, *plp_type);
    return 0;
}
/************************************************************************************************************************
  NAME: SiLabs_API_Select_PLP
  DESCRIPTION: PLP selection function for DVB-T2
  Parameter: front_end, Pointer to SILABS_FE_Context
  Parameter:  plp_id, the PLP id of the required PLP (must be lower than num_plp for the selected carrier)
                Use '-1' for auto selection,
  Returns:    0 if no error value, -1 in case of an error
************************************************************************************************************************/
signed int   SiLabs_API_Select_PLP               (SILABS_FE_Context *front_end, signed int plp_id)
{
  signed int plp_mode;
  SiTRACE("API CALL LOCK  : SiLabs_API_Select_PLP (front_end, %d);\n", plp_id);
  front_end = front_end;        /* To avoid compiler warning if not used */
  plp_id    = plp_id   ; /* To avoid compiler warning if not used */
  plp_mode  = 0;
    if (plp_id < 0) {
      plp_mode = Si2164B_DVBT2_PLP_SELECT_CMD_PLP_ID_SEL_MODE_AUTO;
    } else {
      plp_mode = Si2164B_DVBT2_PLP_SELECT_CMD_PLP_ID_SEL_MODE_MANUAL;
    }
    if (Si2164B_L1_DVBT2_PLP_SELECT(front_end->SiDemod_FE->demod, plp_id, plp_mode) != NO_SiDemod_ERROR) {
      SiTRACE("Si2164B_L1_DVBT2_PLP_SELECT error when selecting PLP %d!\n", plp_id);
      SiERROR("Si2164B_L1_DVBT2_PLP_SELECT error!\n");
      return -1;
    }
    if (plp_mode == Si2164B_DVBT2_PLP_SELECT_CMD_PLP_ID_SEL_MODE_AUTO) { Si2164B_L1_DD_RESTART(front_end->SiDemod_FE->demod); system_wait(300); }
    if (front_end->SiDemod_FE->demod->prop->dd_mode.modulation == Si2164B_DD_MODE_PROP_MODULATION_DVBC2) {
      if (plp_id < 0) {
        front_end->SiDemod_FE->demod->cmd->dvbc2_ds_plp_select.id_sel_mode = Si2164B_DVBC2_DS_PLP_SELECT_CMD_ID_SEL_MODE_AUTO;
        front_end->SiDemod_FE->demod->cmd->dvbc2_ds_plp_select.plp_id      = 0;
      } else {
        front_end->SiDemod_FE->demod->cmd->dvbc2_ds_plp_select.id_sel_mode = Si2164B_DVBC2_DS_PLP_SELECT_CMD_ID_SEL_MODE_MANUAL;
        front_end->SiDemod_FE->demod->cmd->dvbc2_ds_plp_select.plp_id      = plp_id;
      }
      if (Si2164B_L1_DVBC2_DS_PLP_SELECT(front_end->SiDemod_FE->demod,
                                        front_end->SiDemod_FE->demod->cmd->dvbc2_ds_plp_select.ds_id,
                                        front_end->SiDemod_FE->demod->cmd->dvbc2_ds_plp_select.plp_id,
                                        front_end->SiDemod_FE->demod->cmd->dvbc2_ds_plp_select.id_sel_mode) != NO_SiDemod_ERROR) {
        SiTRACE("Si2164B_L1_DVBC2_DS_PLP_SELECT error when selecting PLP %d!\n", plp_id);
        SiERROR("Si2164B_L1_DVBT2_DS_PLP_SELECT error!\n");
        return -1;
      }
      if (plp_mode == Si2164B_DVBC2_DS_PLP_SELECT_CMD_ID_SEL_MODE_AUTO) { Si2164B_L1_DD_RESTART(front_end->SiDemod_FE->demod); system_wait(300); }
    }
    return 0;
  //return -1;
}
/************************************************************************************************************************
  NAME: SiLabs_API_TER_AutoDetect
  DESCRIPTION: activation function for the TER auto detect mode
  Parameter: front_end, Pointer to SILABS_FE_Context
  Parameter: on_off, which is set to '0' to de-activate the TER auto-detect feature,
              set to '1' to activate it and to any other value to retrieve the current status
  Returns:    the current state of auto_detect_TER
************************************************************************************************************************/
signed int   SiLabs_API_TER_AutoDetect           (SILABS_FE_Context *front_end, signed int on_off)
{
  front_end = front_end; /* To avoid compiler warning if not used */
  on_off    = on_off;    /* To avoid compiler warning if not used */
    if (on_off == 0) { Si2164B_TerAutoDetectOff(front_end->SiDemod_FE); }
    if (on_off == 1) { Si2164B_TerAutoDetect   (front_end->SiDemod_FE); }
    return front_end->SiDemod_FE->auto_detect_TER;
  //return -1;
}
/************************************************************************************************************************
  NAME: SiLabs_API_TER_T2_lock_mode
  DESCRIPTION: selection function for the TER auto detect mode
  Parameter: front_end, Pointer to SILABS_FE_Context
  Parameter: T2_lock_mode        the DVB-T2 lock mode        (0='ANY', 1='T2-Base', 2='T2-Lite')
  Returns:    the current state of the DVB-T2 lock mode (using a value out of the above returns the current value)
************************************************************************************************************************/
signed int   SiLabs_API_TER_T2_lock_mode         (SILABS_FE_Context *front_end, signed int T2_lock_mode)
{
  front_end    = front_end;    /* To avoid compiler warning if not used */
  T2_lock_mode = T2_lock_mode; /* To avoid compiler warning if not used */

	if (T2_lock_mode == 0) { front_end->SiDemod_FE->demod->prop->dvbt2_mode.lock_mode = Si2164B_DVBT2_MODE_PROP_LOCK_MODE_ANY;       }
	if (T2_lock_mode == 1) { front_end->SiDemod_FE->demod->prop->dvbt2_mode.lock_mode = Si2164B_DVBT2_MODE_PROP_LOCK_MODE_BASE_ONLY; }
	if (T2_lock_mode == 2) { front_end->SiDemod_FE->demod->prop->dvbt2_mode.lock_mode = Si2164B_DVBT2_MODE_PROP_LOCK_MODE_LITE_ONLY; }
	Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DVBT2_MODE_PROP_CODE);
	return front_end->SiDemod_FE->demod->prop->dvbt2_mode.lock_mode;
}

signed int   SiLabs_API_TER_Tuner_Fine_Tune      (SILABS_FE_Context *front_end, signed int offset_500hz)
{
  front_end    = front_end;    /* To avoid compiler warning if not supported */
  offset_500hz = offset_500hz; /* To avoid compiler warning if not supported */
	Si2151_L1_FINE_TUNE(front_end->SiDemod_FE->tuner_ter,0, offset_500hz);//csm,Refer to half tuner code.
    return 1;
  //return -1;
}
signed int   SiLabs_API_TER_Tuner_Init           (SILABS_FE_Context *front_end)
{
  front_end = front_end; /* To avoid compiler warning if not supported */
  SiLabs_API_Tuner_I2C_Enable (front_end);
  TER_TUNER_INIT(front_end->SiDemod_FE->tuner_ter);
  //model specific setting,20130821,csm,xtal freq tolerance 0ppm(actual:-20~-10ppm) for dual model
  if((front_end->SiDemod_FE->tuner_ter->i2c->address == TER1_MAIN_ADDRESS) || (front_end->SiDemod_FE->tuner_ter->i2c->address == TER1_SUB_ADDRESS)) // for Dual T2CS2 & Dual T2C model 
  {
	  front_end->SiDemod_FE->tuner_ter->prop->crystal_trim.xo_cap = 6; // 6p2pF
	        
	  if (Si2151_L1_SetProperty2(front_end->SiDemod_FE->tuner_ter, Si2151_CRYSTAL_TRIM_PROP_CODE) != NO_Si2151_ERROR) 
	  {
		  SiLabs_API_Tuner_I2C_Disable(front_end);
		  return -1;
	  }
  }
  //20130821,end
  SiLabs_API_Tuner_I2C_Disable(front_end);
  return 1;//return -1;//CSM
}

//csm,13'
signed int   SiLabs_API_TER_Tuner_PowerUpOnly           (SILABS_FE_Context *front_end)
{
  front_end = front_end; /* To avoid compiler warning if not supported */
#ifdef    TER_TUNER_PowerUpOnly
  SiLabs_API_Tuner_I2C_Enable (front_end);
  //TER_TUNER_PowerUpOnly(front_end->SiDemod_FE->tuner_ter); 
  SiLabs_API_Tuner_I2C_Disable(front_end);
  return 1; 
#endif /* TER_TUNER_PowerUpOnly */
  return -1;
}

signed int   SiLabs_API_TER_Tuner_Standby           (SILABS_FE_Context *front_end)
{
	signed int ret=1;

	SiTRACE("SiLabs_API_TER_Tuner_Standby();\n");
	front_end = front_end; /* To avoid compiler warning if not supported */

	SiLabs_API_Tuner_I2C_Enable (front_end);
	if(TER_TUNER_STANDBY(front_end->SiDemod_FE->tuner_ter) != NO_Si2151_ERROR)
		ret = 0;//error
	SiLabs_API_Tuner_I2C_Disable(front_end);
	return ret;
}

signed int   SiLabs_API_TER_Tuner_Wakeup           (SILABS_FE_Context *front_end)
{
	signed int ret=1;

	SiTRACE("SiLabs_API_TER_Tuner_Wakeup();\n");
	front_end = front_end; /* To avoid compiler warning if not supported */

	SiLabs_API_Tuner_I2C_Enable (front_end);
	if(TER_TUNER_WAKEUP(front_end->SiDemod_FE->tuner_ter) != NO_Si2151_ERROR)
		ret = 0;//error
	SiLabs_API_Tuner_I2C_Disable(front_end);

	return ret;
}

signed int   SiLabs_API_TER_Tuner_SetMFT         (SILABS_FE_Context *front_end, signed int nStep)
{
	signed int ret=1;

	SiLabs_API_Tuner_I2C_Enable (front_end);
	if(Si2151_L1_FINE_TUNE (front_end->SiDemod_FE->tuner_ter, Si2151_FINE_TUNE_CMD_PERSISTENCE_NON_PERSISTENT,(125*nStep)) != NO_Si2151_ERROR)
		ret = 0;//error
  SiLabs_API_Tuner_I2C_Disable(front_end);

	return ret;

}
signed int   SiLabs_API_TER_Tuner_Text_status    (SILABS_FE_Context *front_end, unsigned char *separator, unsigned char *msg)
{
  front_end = front_end; /* To avoid compiler warning */
  separator = separator; /* To avoid compiler warning */
  msg       = msg;       /* To avoid compiler warning */
#ifdef    TUNERTER_API
  SiLabs_API_Tuner_I2C_Enable (front_end);

	Si2151_L1_TUNER_STATUS            (front_end->SiDemod_FE->tuner_ter);
	Si2151_L1_GetCommandResponseString(front_end->SiDemod_FE->tuner_ter, Si2151_TUNER_STATUS_CMD_CODE, (char *)separator,(char *) msg);

	
  SiLabs_API_Tuner_I2C_Disable(front_end);
  return 1;
#endif /* TUNERTER_API */
  return -1;
}
signed int   SiLabs_API_TER_Tuner_ATV_Text_status(SILABS_FE_Context *front_end, unsigned char *separator, unsigned char *msg)
{
  front_end = front_end; /* To avoid compiler warning if not used */
  separator = separator; /* To avoid compiler warning if not used */
  msg       = msg      ; /* To avoid compiler warning if not used */
  SiLabs_API_TER_Tuner_I2C_Enable (front_end);
#ifdef    TER_TUNER_SILABS
    SiLabs_TER_Tuner_ATV_Text_STATUS (front_end->SiDemod_FE->tuner_ter , separator, msg);
#endif /* TER_TUNER_SILABS */
  return -1;
}
signed int   SiLabs_API_TER_Tuner_DTV_Text_status(SILABS_FE_Context *front_end, unsigned char *separator, unsigned char *msg)
{
  front_end = front_end; /* To avoid compiler warning */
  separator = separator; /* To avoid compiler warning */
  msg       = msg;       /* To avoid compiler warning */
#ifdef    TUNERTER_API
  SiLabs_API_Tuner_I2C_Enable (front_end);
    //Si2151_L1_DTV_STATUS              (front_end->SiDemod_FE->tuner_ter, Si2151_DTV_STATUS_CMD_INTACK_OK );
    //Si2151_L1_GetCommandResponseString(front_end->SiDemod_FE->tuner_ter, Si2151_DTV_STATUS_CMD_CODE, separator, msg);
  SiLabs_API_Tuner_I2C_Disable(front_end);

    return 1;
#endif /* TUNERTER_API */
  return -1;
}


/************************************************************************************************************************
  SiLabs_API_TER_Tuner_I2C_Enable function
  Use:      Demod Loop through control function,
            Used to switch the I2C loopthrough on, allowing communication with the tuners
            This function can control the I2C passthrough for any front-end in the front-end table,
             and is useful mainly in multi-front-end applications with dual tuners or dual demodulators,
             when the TER tuner I2C is not directly connected to the corresponding demodulator.
  Return:    the final mode (-1 if not known)
************************************************************************************************************************/
signed int   SiLabs_API_TER_Tuner_I2C_Enable     (SILABS_FE_Context *front_end)
{
#ifdef    INDIRECT_I2C_CONNECTION
  signed int fe;
  signed int requester;
  signed int connecter;
  signed int fe_count;
  fe_count = FRONT_END_COUNT;
  for (fe=0; fe< fe_count; fe++) {
    if ( front_end  == &(FrontEnd_Table[fe]) ) {
      requester = fe;
      connecter = front_end->TER_tuner_I2C_connection;
      if (connecter == 100) {
        /* 100 is a special value to allow having tuners connected directly.                                   */
        /* If tuners are connected directly, the i2c pass-through should NOT be closed, to avoid i2c deadlock. */
        return 1;
      }
      SiTRACE("-- I2C -- SiLabs_API_TER_Tuner_I2C_Enable  request for front_end %d via front_end %d\n", requester, connecter);
      if (connecter < fe_count) {
        if (requester != connecter) {
      SiTRACE("-- I2C -- Enabling  indirect TER tuner connection  for front_end %d via front_end %d\n", requester, connecter);
        }
        return SiLabs_API_Tuner_I2C_Enable(&(FrontEnd_Table[connecter]) );
      }
      break;
    }
  }
  SiTRACE("-- I2C -- SiLabs_API_TER_Tuner_I2C_Enable  request failed! Unable to find a match for the caller front_end! (0x%08x)\n", (signed int)front_end);
  SiERROR("-- I2C -- SiLabs_API_TER_Tuner_I2C_Enable  request failed! Unable to find a match for the caller front_end!\n");
  return 0;
#endif /* INDIRECT_I2C_CONNECTION */
  return SiLabs_API_Tuner_I2C_Enable(front_end);
}
/************************************************************************************************************************
  SiLabs_API_TER_Tuner_I2C_Disable function
  Use:      Demod Loop through control function,
            Used to switch the I2C loopthrough off, disabling communication with the TER tuner
            This function can control the I2C passthrough for any front-end in the front-end table,
             and is useful mainly in multi-front-end applications with dual tuners or dual demodulators,
             when the TER tuner I2C is not directly connected to the corresponding demodulator.
  Return:    the final mode (-1 if not known)
************************************************************************************************************************/
signed int   SiLabs_API_TER_Tuner_I2C_Disable    (SILABS_FE_Context *front_end)
{
#ifdef    INDIRECT_I2C_CONNECTION
  signed int fe;
  signed int requester;
  signed int connecter;
  signed int fe_count;
  fe_count = FRONT_END_COUNT;
  for (fe=0; fe< fe_count; fe++) {
    if ( front_end  == &(FrontEnd_Table[fe]) ) {
      requester = fe;
      connecter = front_end->TER_tuner_I2C_connection;
      if (connecter == 100) {
        /* 100 is a special value to allow having tuners connected directly.                                   */
        /* If tuners are connected directly, the i2c pass-through should NOT be closed, to avoid i2c deadlock. */
        return 1;
      }
      SiTRACE("-- I2C -- SiLabs_API_TER_Tuner_I2C_Disable request for front_end %d via front_end %d\n", requester, connecter);
      if (connecter < fe_count) {
        if (requester != connecter) {
      SiTRACE("-- I2C -- Disabling indirect TER tuner connection  for front_end %d via front_end %d\n", requester, connecter);
        }
        return SiLabs_API_Tuner_I2C_Disable(&(FrontEnd_Table[connecter]) );
      }
      break;
    }
  }
  SiTRACE("-- I2C -- SiLabs_API_TER_Tuner_I2C_Disable request failed! Unable to find a match for the caller front_end! (0x%08x)\n", (signed int)front_end);
  SiERROR("-- I2C -- SiLabs_API_TER_Tuner_I2C_Disable request failed! Unable to find a match for the caller front_end!\n");
  return 0;
#endif /* INDIRECT_I2C_CONNECTION */
  return SiLabs_API_Tuner_I2C_Disable(front_end);
}

signed int   SiLabs_API_Demods_Kickstart     (void)
{
  signed int fe;
  SILABS_FE_Context *front_end;
  SiTRACE("API CALL INIT : SiLabs_API_Demods_Kickstart ()\n");
  if (FRONT_END_COUNT == 1) {
    return 1;
  }
  for (fe=0; fe < FRONT_END_COUNT; fe++) {
    front_end  = &(FrontEnd_Table[fe]);
      front_end->SiDemod_FE->demod->cmd->start_clk.clk_mode = front_end->SiDemod_FE->demod->tuner_sat_clock_input;
      front_end->SiDemod_FE->demod->cmd->start_clk.clk_mode = front_end->SiDemod_FE->demod->tuner_ter_clock_input;
      if (front_end->SiDemod_FE->demod->cmd->start_clk.clk_mode == Si2164B_START_CLK_CMD_CLK_MODE_XTAL) {
        front_end->SiDemod_FE->demod->cmd->start_clk.tune_cap = Si2164B_START_CLK_CMD_TUNE_CAP_15P6;
      } else {
        front_end->SiDemod_FE->demod->cmd->start_clk.tune_cap = Si2164B_START_CLK_CMD_TUNE_CAP_EXT_CLK;
      }
      Si2164B_L1_START_CLK (front_end->SiDemod_FE->demod,
        Si2164B_START_CLK_CMD_SUBCODE_CODE,
        Si2164B_START_CLK_CMD_RESERVED1_RESERVED,
        front_end->SiDemod_FE->demod->cmd->start_clk.tune_cap,
        Si2164B_START_CLK_CMD_RESERVED2_RESERVED,
        front_end->SiDemod_FE->demod->cmd->start_clk.clk_mode,
        Si2164B_START_CLK_CMD_RESERVED3_RESERVED,
        Si2164B_START_CLK_CMD_RESERVED4_RESERVED,
        Si2164B_START_CLK_CMD_START_CLK_START_CLK);

  }
  return fe;
}

signed int   SiLabs_API_TER_Tuners_Kickstart (void)
{
  signed int fe;
#ifdef    TER_TUNER_SILABS
  SILABS_FE_Context *front_end;
  SILABS_TER_TUNER_Context *silabs_tuner;
#endif /* TER_TUNER_SILABS */
  fe = 0;
  SiTRACE("API CALL INIT : SiLabs_API_TER_Tuners_Kickstart ()\n");
  if (FRONT_END_COUNT == 1) {
    return 1;
  }
#ifdef    TER_TUNER_SILABS
  for (fe=0; fe < FRONT_END_COUNT; fe++) {
    front_end  = &(FrontEnd_Table[fe]);
    silabs_tuner = SiLabs_API_TER_Tuner(front_end);
    SiTRACE("silabs_tuner 0x%08x\n", silabs_tuner);
    if (silabs_tuner != 0) {
      SiLabs_API_TER_Tuner_I2C_Enable  (front_end);
      SiLabs_TER_Tuner_Tuner_kickstart (silabs_tuner);
      SiLabs_API_TER_Tuner_I2C_Disable (front_end);
    }
  }
#endif /* TER_TUNER_SILABS */
  return fe;
}
int   SiLabs_API_Cypress_Ports        (SILABS_FE_Context *front_end, unsigned char OEA, unsigned char IOA, unsigned char OEB, unsigned char IOB, unsigned char OED, unsigned char IOD )
{
#ifdef    USB_Capability
  double retdval;
  char *entry;
  char  entryBuffer[1000];
  entry = &(entryBuffer[0]);
  SiTRACE("API CALL CONFIG: SiLabs_API_Cypress_Ports        (front_end, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x);\n", OEA, IOA, OEB, IOB, OED, IOD);
  L0_Cypress_Process("output_enable_io_port", "a", (double)OEA, &retdval, &entry);
  L0_Cypress_Process("write_io_port"        , "a", (double)IOA, &retdval, &entry);
  L0_Cypress_Process("output_enable_io_port", "b", (double)OEB, &retdval, &entry);
  L0_Cypress_Process("write_io_port"        , "b", (double)IOB, &retdval, &entry);
  L0_Cypress_Process("output_enable_io_port", "d", (double)OEB, &retdval, &entry);
  L0_Cypress_Process("write_io_port"        , "d", (double)IOB, &retdval, &entry);
  return (IOA<<24)+(IOB<<16)+(IOD<<0);
#endif /* USB_Capability */
  front_end = front_end;        /* To avoid compiler warning */
  IOA=OEA=IOB=OEB=IOD=OED=0x00; /* To avoid compiler warning if not used */
  return (IOA<<24)+(IOB<<16)+(IOD<<0);
}
//csm,13'
signed int   SiLabs_API_TER_Tuner_SetDtvRfTop  (SILABS_FE_Context *front_end,unsigned char rf_top)
{
	switch(rf_top)
	{
		case  0: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_AUTO; break;
		case  1: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P6DB; break;
		case  2: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P5P5DB; break;
		case  3: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P5DB; break;
		case  4: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P4P5DB; break;
		case  5: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P4DB; break;
		case  6: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P3P5DB; break;
		case  7: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P3DB; break;
		case  8: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P2P5DB; break;
		case  9: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P2DB; break;
		case 10: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P1P5DB; break;
		case 11: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P1DB; break;
		case 12: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_P0P5DB; break;
		case 13: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_0DB; break;
		case 14: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M0P5DB; break;
		case 15: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M1DB; break;
		case 16: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M1P5DB; break;
		case 17: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M2DB; break;
		case 18: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M2P5DB; break;
		case 19: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M3DB; break;
		case 20: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M3P5DB; break;
		case 21: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M4DB; break;
		case 22: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M4P5DB; break;
		case 23: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M5DB; break;
		case 24: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M5P5DB; break;
		case 25: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M6DB; break;
		case 26: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M6P5DB; break;
		case 27: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M7DB; break;
		case 28: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M7P5DB; break;
		case 29: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M8DB; break;
		case 30: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M8P5DB; break;
		case 31: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M9DB; break;
		case 32: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M9P5DB; break;
		case 33: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M10DB; break;
		case 34: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M10P5DB; break;
		case 35: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_M11DB; break;
		default: front_end->SiDemod_FE->tuner_ter->prop->dtv_rf_top.dtv_rf_top = Si2151_DTV_RF_TOP_PROP_DTV_RF_TOP_DEFAULT; break;
	}

	SiLabs_API_Tuner_I2C_Enable (front_end);

	if(Si2151_L1_SetProperty2(front_end->SiDemod_FE->tuner_ter, Si2151_DTV_RF_TOP_PROP_CODE) != NO_Si2151_ERROR)
	{
		SiLabs_API_Tuner_I2C_Disable(front_end);
		return ERROR_Si2151_SENDING_COMMAND;
	}
	
	SiLabs_API_Tuner_I2C_Disable(front_end);
    
	return 1;
}


signed int   SiLabs_API_TER_Tuner_DTV_RESTART  (SILABS_FE_Context *front_end)
{
	SiLabs_API_Tuner_I2C_Enable (front_end);

	//if(Si2151_L1_DTV_RESTART(front_end->SiDemod_FE->tuner_ter))
	//{
	//	SiLabs_API_Tuner_I2C_Disable(front_end);
	//	return ERROR_Si2151_SENDING_COMMAND;
	//}

	SiLabs_API_Tuner_I2C_Disable(front_end);

    return 1;
}


//end,13'
/************************************************************************************************************************
  NAME: SiLabs_API_Demod_reset
  DESCRIPTION: demodulator reset function
  Parameter:  Pointer to SILABS_FE_Context
  Returns:    void
************************************************************************************************************************/
void  SiLabs_API_Demod_reset              (SILABS_FE_Context *front_end)
{
    if (Si2164B_L1_DD_RESTART  (front_end->SiDemod_FE->demod)!=0) {
      SiTRACE("Demod reset failed!\n");
    }
}
int   SiLabs_API_Store_FW                 (SILABS_FE_Context *front_end, firmware_struct fw_table[], int nbLines) {
  int stored, line, i; line = i = stored = 0;

    front_end->SiDemod_FE->demod->nbLines  = nbLines;
    front_end->SiDemod_FE->demod->fw_table = (firmware_struct*)realloc(front_end->SiDemod_FE->demod->fw_table, sizeof(firmware_struct)*nbLines);
    for (line = 0; line < nbLines; line++) {
      front_end->SiDemod_FE->demod->fw_table[line].firmware_len = fw_table[line].firmware_len;
      for (i = 0; i < 16; i++) {    front_end->SiDemod_FE->demod->fw_table[line].firmware_table[i] = fw_table[line].firmware_table[i]; }
    }
    stored++;

  if (stored) {
    for (line = 0; line < nbLines; line++) {
      if (fw_table[line].firmware_len > 0) {
        SiTRACE ("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n"
                , fw_table[line].firmware_table[ 0], fw_table[line].firmware_table[ 1], fw_table[line].firmware_table[ 2], fw_table[line].firmware_table[ 3]
                , fw_table[line].firmware_table[ 4], fw_table[line].firmware_table[ 5], fw_table[line].firmware_table[ 6], fw_table[line].firmware_table[ 7]
                , fw_table[line].firmware_table[ 8], fw_table[line].firmware_table[ 9], fw_table[line].firmware_table[10], fw_table[line].firmware_table[11]
                , fw_table[line].firmware_table[12], fw_table[line].firmware_table[13], fw_table[line].firmware_table[14], fw_table[line].firmware_table[15]); }
      if (line== 3         ) {SiTRACE (". . .\n"); SiTraceConfiguration("traces suspend"); }
      if (line==nbLines - 5) {                     SiTraceConfiguration("traces resume" ); }
    }
    SiTraceConfiguration("traces resume");
  }
  return stored*nbLines;
}
int   SiLabs_API_Store_SPI_FW             (SILABS_FE_Context *front_end, unsigned char spi_table[], int nbBytes) {
  int stored, i; i = stored = 0;

    front_end->SiDemod_FE->demod->nbSpiBytes  = nbBytes;
    front_end->SiDemod_FE->demod->spi_table   = (unsigned char*)realloc(front_end->SiDemod_FE->demod->spi_table, sizeof(unsigned char)*nbBytes);
    for (i = 0; i < nbBytes; i++) {    front_end->SiDemod_FE->demod->spi_table[i] = spi_table[i]; }
    stored++;

  if (stored) {
    for (i = 0; i < nbBytes; i++) {
      SiTRACE ("0x%02x ", spi_table[i]);
      if (i%8 == 7)         { SiTRACE (" 0x%02x\n", spi_table[i]); } else { SiTRACE (" 0x%02x", spi_table[i]); }
      if (i == 24         ) {SiTRACE (". . .\n"); SiTraceConfiguration("traces suspend"); }
      if (i==nbBytes - 40 ) {                     SiTraceConfiguration("traces resume" ); }
    }
    SiTraceConfiguration("traces resume");
  }
  return stored*nbBytes;
}
signed int   SiLabs_API_Auto_Detect_Demods       (L0_Context* i2c, signed int *Nb_FrontEnd, signed int demod_code[4], signed int demod_add[4], unsigned char *demod_string[4]) {
  unsigned char replyBytes[16];
  unsigned char START_CLK_Bytes[13] = { 0xc0, 0x12, 0x00, 0x0c, 0x00, 0x0d, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  unsigned char POWER_UP_Bytes[8]   = { 0xc0, 0x06, 0x01, 0x0f, 0x00, 0x20, 0x20, 0x01 };
  unsigned char PART_INFO_Bytes[1]  = { 0x02 };
  signed int add;
  signed int readBytes;
  signed int CTS;
  signed int chiprev;
  signed int part   ;
  signed int pmajor ;
  signed int pminor ;
  signed int pbuild ;
  signed int romid  ;

  *Nb_FrontEnd = 0;
  i2c->indexSize = 0;
  for (add = 0xC8; add <= 0xCE; add=add+2 ) {
    demod_code[*Nb_FrontEnd] = demod_add[*Nb_FrontEnd] = 0;
    /* SiTRACE("Auto detecting demod at 0x%02X\n", add); */
    i2c->address = add;
    L0_WriteBytes(i2c, 0x00, sizeof(START_CLK_Bytes), START_CLK_Bytes);
    L0_WriteBytes(i2c, 0x00, sizeof(POWER_UP_Bytes),  POWER_UP_Bytes);
    CTS = 0;
    while (CTS == 0) {
      readBytes = L0_ReadBytes (i2c, 0x00, 1, replyBytes);
      if (readBytes == 0) {break;}
      if ( (replyBytes[0] & 0x80) == 0x80 ) { CTS = 1;}
    }
    if (CTS == 0) {
      /* SiTRACE("Read failed at add 0x%02X: no chip at this address\n", add); */
    } else {
      L0_WriteBytes(i2c, 0x00, sizeof(PART_INFO_Bytes),  PART_INFO_Bytes);
      CTS = 0;
      while (CTS == 0) {
        readBytes = L0_ReadBytes (i2c, 0x00, 13, replyBytes);
        if (readBytes == 0) {break;}
        if ( (replyBytes[0] & 0x80) == 0x80 ) { CTS = 1;}
      }
      if (CTS == 0) {
        /* SiTRACE("PART_INFO Read failed at add 0x%02X: no chip at this address\n", add); */
      } else {
        chiprev  =   replyBytes[1] & 0x0f;
        part     =   replyBytes[2] ;
        pmajor   =   replyBytes[3] ;
        pminor   =   replyBytes[4] ;
        pbuild   =   replyBytes[5] ;
        romid    =   replyBytes[12];
        demod_code[*Nb_FrontEnd] = 2100 + part;
        demod_add [*Nb_FrontEnd] = add;
        safe_sprintf (demod_string[*Nb_FrontEnd], "Add 0x%02X: 'Si21%02d-%c%c%c ROM%x NVM%c_%cb%d'", add, part, chiprev + 0x40, pmajor, pminor, romid, pmajor, pminor, pbuild );
        *Nb_FrontEnd = *Nb_FrontEnd + 1;
      }
    }
  }
  return *Nb_FrontEnd;
}
#ifdef    SILABS_API_TEST_PIPE
 #ifdef   SILABS_API_VDAPPS
  #include "SiLabs_API_VDAPPS.c"
 #endif /* SILABS_API_VDAPPS */
#endif /* SILABS_API_TEST_PIPE */


/************************************************************************************************************************
  SiLabs_Scan_Table_Init function
  Use:      carriers table initialization function
            Used to allocate the carriers table structure
  Returns: 0 if OK (allocation worked or already allocated), -1 if allocation error
************************************************************************************************************************/
signed int  SiLabs_Scan_Table_Init      (void)
{
  if (Carriers_Table == NULL) {

    Carriers_Table       = (SiLabs_Carriers_Struct*) malloc(sizeof(SiLabs_Carriers_Struct));
    if (Carriers_Table == NULL) { return -1; }

    Carriers_Table->carriers_count  = 0;
    Carriers_Table->standard        = (signed int *) malloc(sizeof(signed int )*Carriers_Table->carriers_count);
    Carriers_Table->freq            = (signed int *) malloc(sizeof(signed int )*Carriers_Table->carriers_count);
    Carriers_Table->bandwidth_Hz    = (signed int *) malloc(sizeof(signed int )*Carriers_Table->carriers_count);
    Carriers_Table->stream          = (signed int *) malloc(sizeof(signed int )*Carriers_Table->carriers_count);
    Carriers_Table->symbol_rate_bps = (unsigned int *) malloc(sizeof(unsigned int )*Carriers_Table->carriers_count);
    Carriers_Table->constellation   = (signed int *) malloc(sizeof(signed int )*Carriers_Table->carriers_count);
    Carriers_Table->polarization    = (signed int *) malloc(sizeof(signed int )*Carriers_Table->carriers_count);
    Carriers_Table->band            = (signed int *) malloc(sizeof(signed int )*Carriers_Table->carriers_count);
    Carriers_Table->data_slice_id   = (signed int *) malloc(sizeof(signed int )*Carriers_Table->carriers_count);
    Carriers_Table->plp_id          = (signed int *) malloc(sizeof(signed int )*Carriers_Table->carriers_count);
    Carriers_Table->T2_base_lite    = (signed int *) malloc(sizeof(signed int )*Carriers_Table->carriers_count);
  }
  return 0;
}
/************************************************************************************************************************
  SiLabs_Scan_Table_Clear function
  Use:      carriers table de-initialization function
            Used to clear the carriers table structure
  Returns: 0
************************************************************************************************************************/
signed int  SiLabs_Scan_Table_Clear     (void)
{
  if    (Carriers_Table != NULL) {
    free(Carriers_Table->standard        );
    free(Carriers_Table->freq            );
    free(Carriers_Table->bandwidth_Hz    );
    free(Carriers_Table->stream          );
    free(Carriers_Table->symbol_rate_bps );
    free(Carriers_Table->constellation   );
    free(Carriers_Table->polarization    );
    free(Carriers_Table->band            );
    free(Carriers_Table->data_slice_id   );
    free(Carriers_Table->plp_id          );
    free(Carriers_Table->T2_base_lite    );
    Carriers_Table->carriers_count = 0;
    free(Carriers_Table);
    Carriers_Table = NULL;
  }
  return 0;
}
/************************************************************************************************************************
  SiLabs_Scan_Table_Count function
  Use:      retrieving the number of channels in the carriers table
            Used to know how many carriers are in the carriers table
  Returns: The number of carriers in the table
************************************************************************************************************************/
signed int  SiLabs_Scan_Table_Count     (void)
{
  if (SiLabs_Scan_Table_Init() == 0) {
    return Carriers_Table->carriers_count;
  }
  return 0;
}

/************************************************************************************************************************
  SiLabs_Scan_Table_AddOneCarrier function
  Use:      carrier addition function
            Used to add a carrier in the scan table
  Returns: The index of the new carrier (-1 if allocation error)
************************************************************************************************************************/
signed int  SiLabs_Scan_Table_AddOneCarrier (                   signed int  standard, signed int  freq, signed int  bandwidth_Hz, signed int  stream, unsigned int  symbol_rate_bps, signed int  constellation, signed int polarization, signed int band, signed int data_slice_id, signed int plp_id, signed int T2_base_lite)
{
  signed int carrier_index;
  if (SiLabs_Scan_Table_Init() == 0) {
    Carriers_Table->carriers_count  = Carriers_Table->carriers_count +1;
    Carriers_Table->standard        = (signed int *) realloc(Carriers_Table->standard       , sizeof(signed int )*Carriers_Table->carriers_count);
    Carriers_Table->freq            = (signed int *) realloc(Carriers_Table->freq           , sizeof(signed int )*Carriers_Table->carriers_count);
    Carriers_Table->bandwidth_Hz    = (signed int *) realloc(Carriers_Table->bandwidth_Hz   , sizeof(signed int )*Carriers_Table->carriers_count);
    Carriers_Table->stream          = (signed int *) realloc(Carriers_Table->stream         , sizeof(signed int )*Carriers_Table->carriers_count);
    Carriers_Table->symbol_rate_bps = (unsigned int *) realloc(Carriers_Table->symbol_rate_bps, sizeof(unsigned int )*Carriers_Table->carriers_count);
    Carriers_Table->constellation   = (signed int *) realloc(Carriers_Table->constellation  , sizeof(signed int )*Carriers_Table->carriers_count);
    Carriers_Table->polarization    = (signed int *) realloc(Carriers_Table->polarization   , sizeof(signed int )*Carriers_Table->carriers_count);
    Carriers_Table->band            = (signed int *) realloc(Carriers_Table->band           , sizeof(signed int )*Carriers_Table->carriers_count);
    Carriers_Table->data_slice_id   = (signed int *) realloc(Carriers_Table->data_slice_id  , sizeof(signed int )*Carriers_Table->carriers_count);
    Carriers_Table->plp_id          = (signed int *) realloc(Carriers_Table->plp_id         , sizeof(signed int )*Carriers_Table->carriers_count);
    Carriers_Table->T2_base_lite    = (signed int *) realloc(Carriers_Table->T2_base_lite   , sizeof(signed int )*Carriers_Table->carriers_count);

    carrier_index = Carriers_Table->carriers_count -1;

    Carriers_Table->standard[carrier_index]        = standard;
    Carriers_Table->freq[carrier_index]            = freq;
    Carriers_Table->bandwidth_Hz[carrier_index]    = bandwidth_Hz;
    Carriers_Table->stream[carrier_index]          = stream;
    Carriers_Table->symbol_rate_bps[carrier_index] = symbol_rate_bps;
    Carriers_Table->constellation[carrier_index]   = constellation;
    Carriers_Table->polarization[carrier_index ]   = polarization;
    Carriers_Table->band[carrier_index]            = band;
    Carriers_Table->data_slice_id[carrier_index]   = data_slice_id;
    Carriers_Table->plp_id[carrier_index]          = plp_id;
    Carriers_Table->T2_base_lite[carrier_index]    = T2_base_lite;

    return carrier_index;
  } else {
    SiTRACE("SiLabs_Scan_Table_AddOneCarrier: Carriers_Table allocation error !\n");
#if 1	// prevent defect : CID42481 NEGATIVE_RETURUNS//20140115
	return 0;
#else
    return -1;
#endif
  }
}

/************************************************************************************************************************
  NAME: SiLabs_API_TS_Clk_Shift_Read
  DESCRIPTION: Get TS clock shift
  Parameter:  Pointer to SILABS_FE_Context
  Returns:    ts_clk_shift
************************************************************************************************************************/
signed int   SiLabs_API_TS_Clk_Shift_Read  (SILABS_FE_Context *front_end) {
  Si2164B_L1_GetProperty2(front_end->SiDemod_FE->demod, Si2164B_DD_TS_SETUP_PAR_PROP_CODE);

  return front_end->SiDemod_FE->demod->prop->dd_ts_setup_par.ts_clk_shift;
}
/************************************************************************************************************************
  NAME: SiLabs_API_TS_Clk_Setup_Parallel
  DESCRIPTION: TS clock parallel setup
  Parameter:  Pointer to SILABS_FE_Context
  Parameter:  ts_clk_shift
  Parameter:  ts_clk_invert, Si2169_DD_TS_SETUP_PAR_PROP_TS_CLK_INVERT_INVERTED: falling edge, Si2169_DD_TS_SETUP_PAR_PROP_TS_CLK_INVERT_NOT_INVERTED: rising edge
  Returns:    1 if sucess
************************************************************************************************************************/
signed int   SiLabs_API_TS_Clk_Setup_Parallel  (SILABS_FE_Context *front_end, unsigned char ts_clk_shift, unsigned char ts_clk_invert)
{
	front_end->SiDemod_FE->demod->prop->dd_ts_setup_par.ts_clk_shift = ts_clk_shift;
	front_end->SiDemod_FE->demod->prop->dd_ts_setup_par.ts_clk_invert = ts_clk_invert;

    Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DD_TS_SETUP_PAR_PROP_CODE);

    return 1;
}
/************************************************************************************************************************
  NAME: SiLabs_API_TS_Clk_Setup_Serial
  DESCRIPTION: TS clock serial setup
  Parameter:  Pointer to SILABS_FE_Context
  Parameter:  ts_clk_invert, Si2169_DD_TS_SETUP_SER_PROP_TS_CLK_INVERT_INVERTED: falling edge, Si2169_DD_TS_SETUP_SER_PROP_TS_CLK_INVERT_NOT_INVERTED: rising edge
  Returns:    1 if sucess
************************************************************************************************************************/
signed int   SiLabs_API_TS_Clk_Setup_Serial  (SILABS_FE_Context *front_end, unsigned char ts_clk_invert)
{
	front_end->SiDemod_FE->demod->prop->dd_ts_setup_ser.ts_clk_invert = ts_clk_invert;

	Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DD_TS_SETUP_SER_PROP_CODE);

    return 1;
}

/************************************************************************************************************************
  NAME: SiLabs_API_Set_DTV_Agc_Speed
  DESCRIPTION: Set DTV Agc Speed
  Parameter:  Pointer to SILABS_FE_Context
  Returns:    1 if sucess
************************************************************************************************************************/
signed int   SiLabs_API_Set_DTV_Agc_Speed  (SILABS_FE_Context *front_end, unsigned char dtv_if_agc_speed, unsigned char dtv_if_decim)
{
	front_end->SiDemod_FE->tuner_ter->prop->dtv_agc_speed.if_agc_speed = dtv_if_agc_speed;
	front_end->SiDemod_FE->tuner_ter->prop->dtv_agc_speed.agc_decim= dtv_if_decim;

	SiLabs_API_Tuner_I2C_Enable (front_end);

	if(Si2151_L1_SetProperty2(front_end->SiDemod_FE->tuner_ter, Si2151_DTV_AGC_SPEED_PROP_CODE) != NO_Si2151_ERROR)
	{
		SiLabs_API_Tuner_I2C_Disable(front_end);
		return ERROR_Si2151_SENDING_COMMAND;
	}

	SiLabs_API_Tuner_I2C_Disable(front_end);

  return 1;
}

signed int   SiLabs_API_Set_DTV_AGC_AUTO_FREEZE  (SILABS_FE_Context *front_end, signed int fef)
{
	SiLabs_API_Tuner_I2C_Enable (front_end);

    if (fef == 0) {
      front_end->SiDemod_FE->tuner_ter->prop->dtv_agc_auto_freeze.thld          = 255;
      front_end->SiDemod_FE->tuner_ter->prop->dtv_agc_auto_freeze.timeout       = 1;
    } else {
      front_end->SiDemod_FE->tuner_ter->prop->dtv_agc_auto_freeze.thld          = 9;
      front_end->SiDemod_FE->tuner_ter->prop->dtv_agc_auto_freeze.timeout       = 63;
    }

    Si2151_L1_SetProperty2(front_end->SiDemod_FE->tuner_ter, Si2151_DTV_AGC_AUTO_FREEZE_PROP_CODE);

	SiLabs_API_Tuner_I2C_Disable(front_end);

	return 1;
}

/************************************************************************************************************************
  NAME: SiLabs_API_Setup_BER_RESOL_EXP
  DESCRIPTION: BER resolution exponent setup
  Parameter:  Pointer to SILABS_FE_Context
  Parameter:  isdvbtscan
  Returns:    1 if sucess
************************************************************************************************************************/
signed int   SiLabs_API_Setup_BER_Resol_exp  (SILABS_FE_Context *front_end, unsigned char isdvbtscan)
{
	if(isdvbtscan == 1)
	{
		front_end->SiDemod_FE->demod->prop->dd_ber_resol.exp = 5;
		front_end->SiDemod_FE->demod->prop->dd_ber_resol.mant = 3;
		front_end->SiDemod_FE->demod->prop->dd_ssi_sqi_param.sqi_average = 30;
	}
	else
	{
		front_end->SiDemod_FE->demod->prop->dd_ber_resol.exp = 7;
		front_end->SiDemod_FE->demod->prop->dd_ber_resol.mant = 1;
		front_end->SiDemod_FE->demod->prop->dd_ssi_sqi_param.sqi_average = 1;
	}

	Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DD_BER_RESOL_PROP_CODE);
	Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DD_SSI_SQI_PARAM_PROP_CODE);
 
    return 1;
}

/************************************************************************************************************************
  NAME: SiLabs_API_Setup_DVBC_AFC_RANGE
  DESCRIPTION: Set up DVB-C afc range
  Parameter:  Pointer to SILABS_FE_Context
  Parameter:  SR(Hz)
  Returns:    1 if sucess
************************************************************************************************************************/
signed int   SiLabs_API_Setup_DVBC_AFC_RANGE  (SILABS_FE_Context *front_end, signed int SR)
{
	if(SR < 6000000)
	{
		front_end->SiDemod_FE->demod->prop->dvbc_afc_range.range_khz =  100;
	}
	else
	{
		front_end->SiDemod_FE->demod->prop->dvbc_afc_range.range_khz =  250;
	}
	
	Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DVBC_AFC_RANGE_PROP_CODE);
 
	return 1;
}

signed int   SiLabs_API_T2_MPLP  (SILABS_FE_Context *front_end, signed int freq)  // freq: Hz
{
  front_end=front_end; /*avoid compile warning*/ 
  freq=freq; /*avoid compile warning*/ 
  
#if 0//csm..N/A
  Si2164B_L2_Test_MPLP(front_end->SiDemod_FE, (double)freq);//Si2169_L2_T2_MPLP(front_end->SiDemod_FE, (double)freq);
#endif
  return 1;
}

/************************************************************************************************************************
  NAME: SiLabs_API_Set_Invert_Spectrum
  DESCRIPTION: Set up invert spectrum
  Parameter:  Pointer to SILABS_FE_Context
  Parameter:  IsSatCR: 1(SatCR), 0(normal)
  Returns:    1 if success
************************************************************************************************************************/
signed int   SiLabs_API_Set_Invert_Spectrum  (SILABS_FE_Context *front_end, unsigned char IsSatCR)
{
	if(IsSatCR == 1)
	{
	  if((front_end->SiDemod_FE->tuner_sat->i2c->address == SAT1_MAIN_ADDRESS) || (front_end->SiDemod_FE->tuner_sat->i2c->address == SAT1_SUB_ADDRESS))//5816SW
		  front_end->SiDemod_FE->demod->prop->dd_mode.invert_spectrum = Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_INVERTED;
	  else if(front_end->SiDemod_FE->tuner_sat->i2c->address == SAT2_ADDRESS)//5815M
		  front_end->SiDemod_FE->demod->prop->dd_mode.invert_spectrum = Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_NORMAL;
	}
	else
	{
	  if((front_end->SiDemod_FE->tuner_sat->i2c->address == SAT1_MAIN_ADDRESS) || (front_end->SiDemod_FE->tuner_sat->i2c->address == SAT1_SUB_ADDRESS))//5816SW
		  front_end->SiDemod_FE->demod->prop->dd_mode.invert_spectrum = Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_NORMAL;
	  else if(front_end->SiDemod_FE->tuner_sat->i2c->address == SAT2_ADDRESS)//5815M
		  front_end->SiDemod_FE->demod->prop->dd_mode.invert_spectrum = Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_INVERTED;
	}

    if (Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DD_MODE_PROP_CODE)==0) {
      Si2164B_L1_DD_RESTART(front_end->SiDemod_FE->demod);
    } else {
      SiTRACE("Demod restart failed !\n");
      return 0;
    }

	return 1;
}
//end,13'


signed int   SiLabs_API_Setup_TS_Parallel_Strength_Shape  (SILABS_FE_Context *front_end, unsigned char data_strength, unsigned char data_shape, unsigned char clk_strength, unsigned char clk_shape)
{
	front_end->SiDemod_FE->demod->prop->dd_ts_setup_par.ts_data_strength = data_strength;
	front_end->SiDemod_FE->demod->prop->dd_ts_setup_par.ts_data_shape = data_shape;
	front_end->SiDemod_FE->demod->prop->dd_ts_setup_par.ts_clk_strength = clk_strength;
	front_end->SiDemod_FE->demod->prop->dd_ts_setup_par.ts_clk_shape = clk_shape;

	Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DD_TS_SETUP_PAR_PROP_CODE);

  return 1;
}

signed int   SiLabs_API_Setup_TS_Serial_Strength_Shape  (SILABS_FE_Context *front_end, unsigned char data_strength, unsigned char data_shape, unsigned char clk_strength, unsigned char clk_shape)
{
	front_end->SiDemod_FE->demod->prop->dd_ts_setup_ser.ts_data_strength = data_strength;
	front_end->SiDemod_FE->demod->prop->dd_ts_setup_ser.ts_data_shape = data_shape;
	front_end->SiDemod_FE->demod->prop->dd_ts_setup_ser.ts_clk_strength = clk_strength;
	front_end->SiDemod_FE->demod->prop->dd_ts_setup_ser.ts_clk_shape = clk_shape;

	Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DD_TS_SETUP_SER_PROP_CODE);

  return 1;
}

/************************************************************************************************************************
  NAME: SiLabs_API_Setup_Ter_TS_SLR_SERIAL
  DESCRIPTION: set Terrestrial TS SLR SERIAL
  Parameter:  Pointer to SILABS_FE_Context
  Returns:    1 if no error
************************************************************************************************************************/
signed int   SiLabs_API_Setup_Ter_TS_SLR_SERIAL  (SILABS_FE_Context *front_end)
{
	front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_data_slr                                     =     1; /* (default     0) */
	front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_data_slr_on                                  = 1  ; /* (default 'DISABLE') */
	front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_data1_slr                                    =     1; /* (default     0) */
	front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_data1_slr_on                                 = 1 ; /* (default 'DISABLE') */
	front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_data2_slr                                    =     1; /* (default     0) */
	front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_data2_slr_on                                 = 1 ; /* (default 'DISABLE') */
	front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_clk_slr                                      =     1; /* (default     0) */
	front_end->SiDemod_FE->demod->prop->dd_ts_slr_serial.ts_clk_slr_on                                   = 1   ; /* (default 'DISABLE') */
	SiTRACE("SiLabs_API_Setup_Ter_TS_SLR_SERIAL SLR 1\n");

	front_end->SiDemod_FE->demod->prop->dd_ts_setup_ser.ts_data_strength = 10;
	front_end->SiDemod_FE->demod->prop->dd_ts_setup_ser.ts_data_shape = 0;
	front_end->SiDemod_FE->demod->prop->dd_ts_setup_ser.ts_clk_strength = 10;
	front_end->SiDemod_FE->demod->prop->dd_ts_setup_ser.ts_clk_shape = 0;

	if (Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DD_TS_SLR_SERIAL_PROP_CODE) != NO_Si2164B_ERROR)
	{
		SiTRACE("TERRESTRIAL: Si2164B_DD_TS_SLR_SERIAL_PROP failed !\n");
		return 0;
	} else {
		SiTRACE("TERRESTRIAL: Si2164B_DD_TS_SLR_SERIAL_PROP complete !\n");
	}

	Si2164B_L1_SetProperty2(front_end->SiDemod_FE->demod, Si2164B_DD_TS_SETUP_SER_PROP_CODE);

	return 1;
}

/************************************************************************************************************************
  NAME: SiLabs_API_TAG_TEXT
  DESCRIPTION: SiLabs API information function used to retrieve the version information of the SiLabs API wrapper
  Returns:    the SiLabs API version information string
************************************************************************************************************************/
const unsigned char* SiLabs_API_TAG_TEXT      (void) { return (unsigned char*)"V2.5.6"; }

/************************************************************************************************************************
  NAME: SiLabs_API_TAG_TEXT
  DESCRIPTION: //csm, add, to configure TER clock mode. This function just set value but does not send command.
  Parameter: clk_mode '0'- XTAL, clk_mode '1' - external clock
  Returns:    the SiLabs API version information string
************************************************************************************************************************/
void SiLabs_API_TER_XtalConfig(SILABS_FE_Context *front_end, signed int clk_mode)
{
	if(clk_mode)
	{
		front_end->SiDemod_FE->tuner_ter->cmd->power_up.clock_mode = Si2151_POWER_UP_CMD_CLOCK_MODE_EXTCLK;
		front_end->SiDemod_FE->tuner_ter->cmd->power_up.en_xout    = Si2151_POWER_UP_CMD_EN_XOUT_EN_XOUT;
		front_end->SiDemod_FE->tuner_ter->cmd->config_clocks.clock_mode = Si2151_CONFIG_CLOCKS_CMD_CLOCK_MODE_EXTCLK;
	}
	else
	{
		front_end->SiDemod_FE->tuner_ter->cmd->power_up.clock_mode = Si2151_POWER_UP_CMD_CLOCK_MODE_XTAL;
		front_end->SiDemod_FE->tuner_ter->cmd->power_up.en_xout    = Si2151_POWER_UP_CMD_EN_XOUT_EN_XOUT;
		front_end->SiDemod_FE->tuner_ter->cmd->config_clocks.clock_mode = Si2151_CONFIG_CLOCKS_CMD_CLOCK_MODE_XTAL;
	}
}

//20130903,related to kernel driver,csm
void Tuner_GetFwInfo(SILABS_FE_Context *front_end, unsigned long *fw_addr, unsigned long *fw_size)
{
    front_end=front_end; /*avoid compile warning*/
    fw_addr=fw_addr; /*avoid compile warning*/
    fw_size=fw_size; /*avoid compile warning*/
    
	//publishFirmwarePointer(front_end->SiDemod_FE->tuner_ter,fw_addr, fw_size);
}

void Demod_GetFwInfo(SILABS_FE_Context *front_end, unsigned long *fw_addr, unsigned long *fw_size)
{
	Si2164B_publishFirmwarePointer(front_end->SiDemod_FE,fw_addr, fw_size);
}
//end

/************************************************************************************************************************
  NAME: SiLabs_API_firmwares_download
  DESCRIPTION: 20130903, to seperate initialization process
  Parameter: clk_mode '0'- XTAL, clk_mode '1' - external clock
  Returns:    '0' if OK, any other value indicates an error
************************************************************************************************************************/
signed int SiLabs_API_firmwares_download(SILABS_FE_Context *front_end, signed int clk_mode, signed int demodAdd, signed int tunerAdd_Ter, signed int tunerAdd_Sat)
{
	return Si2164B_L2_firmwares_download(front_end->SiDemod_FE, clk_mode, demodAdd, tunerAdd_Ter, tunerAdd_Sat);
}

#ifdef    __cplusplus
}
#endif /* __cplusplus */
