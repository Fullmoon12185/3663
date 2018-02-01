/*************************************************************************************
                  Silicon Laboratories Broadcast Si2164B Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
    PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API functions prototypes used by commands and properties
   FILE: Si2164B_Commands_Prototypes.h
   Supported IC : Si2164B
   Compiled for ROM 2 firmware 5_B_build_1
   Revision: V0.1.1.0
   Date: March 30 2015
  (C) Copyright 2015, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
/* Change log:

 As from V0.1.0.0:
  <new_feature>[DVB-S2/MULTISTREAM] Adding Si2164B_DVBS2_STREAM_INFO and Si2164B_DVBS2_STREAM_SELECT commands

 As from V0.0.0:
  Initial version (based on Si2164 code V0.3.4)
****************************************************************************************/
#ifndef    Si2164B_COMMANDS_PROTOTYPES_H
#define    Si2164B_COMMANDS_PROTOTYPES_H
#define    Si2164B_GET_COMMAND_STRINGS//csm
#ifdef __cplusplus//csm,test
extern "C" {
#endif
unsigned char Si2164B_CurrentResponseStatus (L1_Si2164B_Context *api, unsigned char ptDataBuffer);
unsigned char Si2164B_pollForCTS            (L1_Si2164B_Context *api);
unsigned char Si2164B_pollForResponse       (L1_Si2164B_Context *api, unsigned int nbBytes, unsigned char *pByteBuffer);
unsigned char Si2164B_L1_SendCommand2       (L1_Si2164B_Context *api, unsigned int cmd_code);
unsigned char Si2164B_L1_SetProperty        (L1_Si2164B_Context *api, unsigned int prop_code, signed int  data);
unsigned char Si2164B_L1_GetProperty        (L1_Si2164B_Context *api, unsigned int prop_code, signed int *data);
unsigned char Si2164B_L1_SetProperty2       (L1_Si2164B_Context *api, unsigned int prop_code);
unsigned char Si2164B_L1_GetProperty2       (L1_Si2164B_Context *api, unsigned int prop_code);

#ifdef    Si2164B_GET_PROPERTY_STRING
unsigned char Si2164B_L1_GetPropertyString  (L1_Si2164B_Context *api, unsigned int prop_code, const unsigned char *separator, unsigned char *msg);
unsigned char Si2164B_L1_PropertyText       (Si2164B_PropObj   *prop, unsigned int prop_code, const unsigned char *separator, unsigned char *msg);
#endif /* Si2164B_GET_PROPERTY_STRING */

#ifdef    Si2164B_GET_COMMAND_STRINGS
  unsigned char   Si2164B_L1_GetCommandResponseString(L1_Si2164B_Context *api, unsigned int cmd_code, const unsigned char *separator, unsigned char *msg);
#endif /* Si2164B_GET_COMMAND_STRINGS */

#ifdef    Si2164B_CONFIG_CLKIO_CMD
unsigned char Si2164B_L1_CONFIG_CLKIO              (L1_Si2164B_Context *api,
                                                   unsigned char   output,
                                                   unsigned char   pre_driver_str,
                                                   unsigned char   driver_str);
#endif /* Si2164B_CONFIG_CLKIO_CMD */
#ifdef    Si2164B_CONFIG_I2C_CMD
unsigned char Si2164B_L1_CONFIG_I2C                (L1_Si2164B_Context *api,
                                                   unsigned char   subcode,
                                                   unsigned char   i2c_broadcast);
#endif /* Si2164B_CONFIG_I2C_CMD */
#ifdef    Si2164B_CONFIG_PINS_CMD
unsigned char Si2164B_L1_CONFIG_PINS               (L1_Si2164B_Context *api,
                                                   unsigned char   gpio0_mode,
                                                   unsigned char   gpio0_read,
                                                   unsigned char   gpio1_mode,
                                                   unsigned char   gpio1_read);
#endif /* Si2164B_CONFIG_PINS_CMD */
#ifdef    Si2164B_DD_BER_CMD
unsigned char Si2164B_L1_DD_BER                    (L1_Si2164B_Context *api,
                                                   unsigned char   rst);
#endif /* Si2164B_DD_BER_CMD */
#ifdef    Si2164B_DD_CBER_CMD
unsigned char Si2164B_L1_DD_CBER                   (L1_Si2164B_Context *api,
                                                   unsigned char   rst);
#endif /* Si2164B_DD_CBER_CMD */
#ifdef    Si2164B_DD_DISEQC_SEND_CMD
unsigned char Si2164B_L1_DD_DISEQC_SEND            (L1_Si2164B_Context *api,
                                                   unsigned char   enable,
                                                   unsigned char   cont_tone,
                                                   unsigned char   tone_burst,
                                                   unsigned char   burst_sel,
                                                   unsigned char   end_seq,
                                                   unsigned char   msg_length,
                                                   unsigned char   msg_byte1,
                                                   unsigned char   msg_byte2,
                                                   unsigned char   msg_byte3,
                                                   unsigned char   msg_byte4,
                                                   unsigned char   msg_byte5,
                                                   unsigned char   msg_byte6);
#endif /* Si2164B_DD_DISEQC_SEND_CMD */
#ifdef    Si2164B_DD_DISEQC_STATUS_CMD
unsigned char Si2164B_L1_DD_DISEQC_STATUS          (L1_Si2164B_Context *api,
                                                   unsigned char   listen);
#endif /* Si2164B_DD_DISEQC_STATUS_CMD */
#ifdef    Si2164B_DD_EXT_AGC_SAT_CMD
unsigned char Si2164B_L1_DD_EXT_AGC_SAT            (L1_Si2164B_Context *api,
                                                   unsigned char   agc_1_mode,
                                                   unsigned char   agc_1_inv,
                                                   unsigned char   agc_2_mode,
                                                   unsigned char   agc_2_inv,
                                                   unsigned char   agc_1_kloop,
                                                   unsigned char   agc_2_kloop,
                                                   unsigned char   agc_1_min,
                                                   unsigned char   agc_2_min);
#endif /* Si2164B_DD_EXT_AGC_SAT_CMD */

#ifdef    Si2164B_DD_EXT_AGC_TER_CMD
unsigned char Si2164B_L1_DD_EXT_AGC_TER            (L1_Si2164B_Context *api,
                                                   unsigned char   agc_1_mode,
                                                   unsigned char   agc_1_inv,
                                                   unsigned char   agc_2_mode,
                                                   unsigned char   agc_2_inv,
                                                   unsigned char   agc_1_kloop,
                                                   unsigned char   agc_2_kloop,
                                                   unsigned char   agc_1_min,
                                                   unsigned char   agc_2_min);
#endif /* Si2164B_DD_EXT_AGC_TER_CMD */

#ifdef    Si2164B_DD_FER_CMD
unsigned char Si2164B_L1_DD_FER                    (L1_Si2164B_Context *api,
                                                   unsigned char   rst);
#endif /* Si2164B_DD_FER_CMD */
#ifdef    Si2164B_DD_GET_REG_CMD
unsigned char Si2164B_L1_DD_GET_REG                (L1_Si2164B_Context *api,
                                                   unsigned char   reg_code_lsb,
                                                   unsigned char   reg_code_mid,
                                                   unsigned char   reg_code_msb);
#endif /* Si2164B_DD_GET_REG_CMD */
#ifdef    Si2164B_DD_MP_DEFAULTS_CMD
unsigned char Si2164B_L1_DD_MP_DEFAULTS            (L1_Si2164B_Context *api,
                                                   unsigned char   mp_a_mode,
                                                   unsigned char   mp_b_mode,
                                                   unsigned char   mp_c_mode,
                                                   unsigned char   mp_d_mode);
#endif /* Si2164B_DD_MP_DEFAULTS_CMD */
#ifdef    Si2164B_DD_PER_CMD
unsigned char Si2164B_L1_DD_PER                    (L1_Si2164B_Context *api,
                                                   unsigned char   rst);
#endif /* Si2164B_DD_PER_CMD */
#ifdef    Si2164B_DD_RESTART_CMD
unsigned char Si2164B_L1_DD_RESTART                (L1_Si2164B_Context *api);
#endif /* Si2164B_DD_RESTART_CMD */
#ifdef    Si2164B_DD_RESTART_EXT_CMD
unsigned char Si2164B_L1_DD_RESTART_EXT            (L1_Si2164B_Context *api,
                                                   unsigned char   freq_plan,
                                                   unsigned char   freq_plan_ts_clk,
                                                   unsigned long   tuned_rf_freq);
#endif /* Si2164B_DD_RESTART_EXT_CMD */
#ifdef    Si2164B_DD_SET_REG_CMD
unsigned char Si2164B_L1_DD_SET_REG                (L1_Si2164B_Context *api,
                                                   unsigned char   reg_code_lsb,
                                                   unsigned char   reg_code_mid,
                                                   unsigned char   reg_code_msb,
                                                   unsigned long   value);
#endif /* Si2164B_DD_SET_REG_CMD */
#ifdef    Si2164B_DD_SSI_SQI_CMD
unsigned char Si2164B_L1_DD_SSI_SQI                (L1_Si2164B_Context *api,
                                                             signed char  tuner_rssi);
#endif /* Si2164B_DD_SSI_SQI_CMD */
#ifdef    Si2164B_DD_STATUS_CMD
unsigned char Si2164B_L1_DD_STATUS                 (L1_Si2164B_Context *api,
                                                   unsigned char   intack);
#endif /* Si2164B_DD_STATUS_CMD */
#ifdef    Si2164B_DD_TS_PINS_CMD
unsigned char Si2164B_L1_DD_TS_PINS                (L1_Si2164B_Context *api,
                                                   unsigned char   primary_ts_mode,
                                                   unsigned char   primary_ts_activity,
                                                   unsigned char   secondary_ts_mode,
                                                   unsigned char   secondary_ts_activity);
#endif /* Si2164B_DD_TS_PINS_CMD */
#ifdef    Si2164B_DD_UNCOR_CMD
unsigned char Si2164B_L1_DD_UNCOR                  (L1_Si2164B_Context *api,
                                                   unsigned char   rst);
#endif /* Si2164B_DD_UNCOR_CMD */
#ifdef    Si2164B_DEMOD_INFO_CMD
unsigned char Si2164B_L1_DEMOD_INFO                (L1_Si2164B_Context *api);
#endif /* Si2164B_DEMOD_INFO_CMD */
#ifdef    Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD
unsigned char Si2164B_L1_DOWNLOAD_DATASET_CONTINUE (L1_Si2164B_Context *api,
                                                   unsigned char   data0,
                                                   unsigned char   data1,
                                                   unsigned char   data2,
                                                   unsigned char   data3,
                                                   unsigned char   data4,
                                                   unsigned char   data5,
                                                   unsigned char   data6);
#endif /* Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD */
#ifdef    Si2164B_DOWNLOAD_DATASET_START_CMD
unsigned char Si2164B_L1_DOWNLOAD_DATASET_START    (L1_Si2164B_Context *api,
                                                   unsigned char   dataset_id,
                                                   unsigned char   dataset_checksum,
                                                   unsigned char   data0,
                                                   unsigned char   data1,
                                                   unsigned char   data2,
                                                   unsigned char   data3,
                                                   unsigned char   data4);
#endif /* Si2164B_DOWNLOAD_DATASET_START_CMD */
#ifdef    Si2164B_DVBC2_CTRL_CMD
unsigned char Si2164B_L1_DVBC2_CTRL                (L1_Si2164B_Context *api,
                                                   unsigned char   action,
                                                   unsigned long   tuned_rf_freq);
#endif /* Si2164B_DVBC2_CTRL_CMD */
#ifdef    Si2164B_DVBC2_DS_INFO_CMD
unsigned char Si2164B_L1_DVBC2_DS_INFO             (L1_Si2164B_Context *api,
                                                   unsigned char   ds_index_or_id,
                                                   unsigned char   ds_select_index_or_id);
#endif /* Si2164B_DVBC2_DS_INFO_CMD */
#ifdef    Si2164B_DVBC2_DS_PLP_SELECT_CMD
unsigned char Si2164B_L1_DVBC2_DS_PLP_SELECT       (L1_Si2164B_Context *api,
                                                   unsigned char   plp_id,
                                                   unsigned char   id_sel_mode,
                                                   unsigned char   ds_id);
#endif /* Si2164B_DVBC2_DS_PLP_SELECT_CMD */
#ifdef    Si2164B_DVBC2_PLP_INFO_CMD
unsigned char Si2164B_L1_DVBC2_PLP_INFO            (L1_Si2164B_Context *api,
                                                   unsigned char   plp_index,
                                                   unsigned char   plp_info_ds_mode,
                                                   unsigned char   ds_index);
#endif /* Si2164B_DVBC2_PLP_INFO_CMD */
#ifdef    Si2164B_DVBC2_STATUS_CMD
unsigned char Si2164B_L1_DVBC2_STATUS              (L1_Si2164B_Context *api,
                                                   unsigned char   intack);
#endif /* Si2164B_DVBC2_STATUS_CMD */
#ifdef    Si2164B_DVBC2_SYS_INFO_CMD
unsigned char Si2164B_L1_DVBC2_SYS_INFO            (L1_Si2164B_Context *api);
#endif /* Si2164B_DVBC2_SYS_INFO_CMD */

#ifdef    Si2164B_DVBC_STATUS_CMD
unsigned char Si2164B_L1_DVBC_STATUS               (L1_Si2164B_Context *api,
                                                   unsigned char   intack);
#endif /* Si2164B_DVBC_STATUS_CMD */

#ifdef    Si2164B_DVBS2_STATUS_CMD
unsigned char Si2164B_L1_DVBS2_STATUS              (L1_Si2164B_Context *api,
                                                   unsigned char   intack);
#endif /* Si2164B_DVBS2_STATUS_CMD */
#ifdef    Si2164B_DVBS2_STREAM_INFO_CMD
unsigned char Si2164B_L1_DVBS2_STREAM_INFO         (L1_Si2164B_Context *api,
                                                   unsigned char   isi_index);
#endif /* Si2164B_DVBS2_STREAM_INFO_CMD */
#ifdef    Si2164B_DVBS2_STREAM_SELECT_CMD
unsigned char Si2164B_L1_DVBS2_STREAM_SELECT       (L1_Si2164B_Context *api,
                                                   unsigned char   stream_id,
                                                   unsigned char   stream_sel_mode);
#endif /* Si2164B_DVBS2_STREAM_SELECT_CMD */
#ifdef    Si2164B_DVBS_STATUS_CMD
unsigned char Si2164B_L1_DVBS_STATUS               (L1_Si2164B_Context *api,
                                                   unsigned char   intack);
#endif /* Si2164B_DVBS_STATUS_CMD */

#ifdef    Si2164B_DVBT2_FEF_CMD
unsigned char Si2164B_L1_DVBT2_FEF                 (L1_Si2164B_Context *api,
                                                   unsigned char   fef_tuner_flag,
                                                   unsigned char   fef_tuner_flag_inv);
#endif /* Si2164B_DVBT2_FEF_CMD */
#ifdef    Si2164B_DVBT2_PLP_INFO_CMD
unsigned char Si2164B_L1_DVBT2_PLP_INFO            (L1_Si2164B_Context *api,
                                                   unsigned char   plp_index);
#endif /* Si2164B_DVBT2_PLP_INFO_CMD */
#ifdef    Si2164B_DVBT2_PLP_SELECT_CMD
unsigned char Si2164B_L1_DVBT2_PLP_SELECT          (L1_Si2164B_Context *api,
                                                   unsigned char   plp_id,
                                                   unsigned char   plp_id_sel_mode);
#endif /* Si2164B_DVBT2_PLP_SELECT_CMD */
#ifdef    Si2164B_DVBT2_STATUS_CMD
unsigned char Si2164B_L1_DVBT2_STATUS              (L1_Si2164B_Context *api,
                                                   unsigned char   intack);
#endif /* Si2164B_DVBT2_STATUS_CMD */
#ifdef    Si2164B_DVBT2_TX_ID_CMD
unsigned char Si2164B_L1_DVBT2_TX_ID               (L1_Si2164B_Context *api);
#endif /* Si2164B_DVBT2_TX_ID_CMD */

#ifdef    Si2164B_DVBT_STATUS_CMD
unsigned char Si2164B_L1_DVBT_STATUS               (L1_Si2164B_Context *api,
                                                   unsigned char   intack);
#endif /* Si2164B_DVBT_STATUS_CMD */
#ifdef    Si2164B_DVBT_TPS_EXTRA_CMD
unsigned char Si2164B_L1_DVBT_TPS_EXTRA            (L1_Si2164B_Context *api);
#endif /* Si2164B_DVBT_TPS_EXTRA_CMD */

#ifdef    Si2164B_EXIT_BOOTLOADER_CMD
unsigned char Si2164B_L1_EXIT_BOOTLOADER           (L1_Si2164B_Context *api,
                                                   unsigned char   func,
                                                   unsigned char   ctsien);
#endif /* Si2164B_EXIT_BOOTLOADER_CMD */
#ifdef    Si2164B_GET_PROPERTY_CMD
unsigned char Si2164B_L1_GET_PROPERTY              (L1_Si2164B_Context *api,
                                                   unsigned char   reserved,
                                                   unsigned int    prop);
#endif /* Si2164B_GET_PROPERTY_CMD */
#ifdef    Si2164B_GET_REV_CMD
unsigned char Si2164B_L1_GET_REV                   (L1_Si2164B_Context *api);
#endif /* Si2164B_GET_REV_CMD */
#ifdef    Si2164B_I2C_PASSTHROUGH_CMD
unsigned char Si2164B_L1_I2C_PASSTHROUGH           (L1_Si2164B_Context *api,
                                                   unsigned char   subcode,
                                                   unsigned char   i2c_passthru,
                                                   unsigned char   reserved);
#endif /* Si2164B_I2C_PASSTHROUGH_CMD */

#ifdef    Si2164B_MCNS_STATUS_CMD
unsigned char Si2164B_L1_MCNS_STATUS               (L1_Si2164B_Context *api,
                                                   unsigned char   intack);
#endif /* Si2164B_MCNS_STATUS_CMD */

#ifdef    Si2164B_PART_INFO_CMD
unsigned char Si2164B_L1_PART_INFO                 (L1_Si2164B_Context *api);
#endif /* Si2164B_PART_INFO_CMD */
#ifdef    Si2164B_POWER_DOWN_CMD
unsigned char Si2164B_L1_POWER_DOWN                (L1_Si2164B_Context *api);
#endif /* Si2164B_POWER_DOWN_CMD */
#ifdef    Si2164B_POWER_UP_CMD
unsigned char Si2164B_L1_POWER_UP                  (L1_Si2164B_Context *api,
                                                   unsigned char   subcode,
                                                   unsigned char   reset,
                                                   unsigned char   reserved2,
                                                   unsigned char   reserved4,
                                                   unsigned char   reserved1,
                                                   unsigned char   addr_mode,
                                                   unsigned char   reserved5,
                                                   unsigned char   func,
                                                   unsigned char   clock_freq,
                                                   unsigned char   ctsien,
                                                   unsigned char   wake_up);
#endif /* Si2164B_POWER_UP_CMD */
#ifdef    Si2164B_RSSI_ADC_CMD
unsigned char Si2164B_L1_RSSI_ADC                  (L1_Si2164B_Context *api,
                                                   unsigned char   on_off);
#endif /* Si2164B_RSSI_ADC_CMD */
#ifdef    Si2164B_SCAN_CTRL_CMD
unsigned char Si2164B_L1_SCAN_CTRL                 (L1_Si2164B_Context *api,
                                                   unsigned char   action,
                                                   unsigned long   tuned_rf_freq);
#endif /* Si2164B_SCAN_CTRL_CMD */
#ifdef    Si2164B_SCAN_STATUS_CMD
unsigned char Si2164B_L1_SCAN_STATUS               (L1_Si2164B_Context *api,
                                                   unsigned char   intack);
#endif /* Si2164B_SCAN_STATUS_CMD */
#ifdef    Si2164B_SET_PROPERTY_CMD
unsigned char Si2164B_L1_SET_PROPERTY              (L1_Si2164B_Context *api,
                                                   unsigned char   reserved,
                                                   unsigned int    prop,
                                                   unsigned int    data);
#endif /* Si2164B_SET_PROPERTY_CMD */
#ifdef    Si2164B_SPI_LINK_CMD
unsigned char Si2164B_L1_SPI_LINK                  (L1_Si2164B_Context *api,
                                                   unsigned char   subcode,
                                                   unsigned char   spi_pbl_key,
                                                   unsigned char   spi_pbl_num,
                                                   unsigned char   spi_conf_clk,
                                                   unsigned char   spi_clk_pola,
                                                   unsigned char   spi_conf_data,
                                                   unsigned char   spi_data_dir,
                                                   unsigned char   spi_enable);
#endif /* Si2164B_SPI_LINK_CMD */
#ifdef    Si2164B_SPI_PASSTHROUGH_CMD
unsigned char Si2164B_L1_SPI_PASSTHROUGH           (L1_Si2164B_Context *api,
                                                   unsigned char   subcode,
                                                   unsigned char   spi_passthr_clk,
                                                   unsigned char   spi_passth_data);
#endif /* Si2164B_SPI_PASSTHROUGH_CMD */
#ifdef    Si2164B_START_CLK_CMD
unsigned char Si2164B_L1_START_CLK                 (L1_Si2164B_Context *api,
                                                   unsigned char   subcode,
                                                   unsigned char   reserved1,
                                                   unsigned char   tune_cap,
                                                   unsigned char   reserved2,
                                                   unsigned int    clk_mode,
                                                   unsigned char   reserved3,
                                                   unsigned char   reserved4,
                                                   unsigned char   start_clk);
#endif /* Si2164B_START_CLK_CMD */
#if defined( __cplusplus )//csm,test
}
#endif
#endif /* Si2164B_COMMANDS_PROTOTYPES_H */












