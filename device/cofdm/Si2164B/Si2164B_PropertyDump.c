/* Automatically generated from a template on 2013 August 28 at 12:04 */ /*( Processed on PC0008680 (mdorval) ) */
/*************************************************************************************
                  Silicon Laboratories Broadcast Si2164B Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API properties definitions
   FILE: Si2164B_PropertyDump.c
   Supported IC : Si2178,Si2190
   Compiled for ROM 41 firmware 1_1_build_4
   Revision: 0.3
   Tag:  Si2190_11b4_Si2178_22b7_V0.3
   Date: June 07 2013
  (C) Copyright 2013, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#define   Si2164B_COMMAND_PROTOTYPES

#include "Si2164B_L1_API.h"
#include "Si2164B_PropertyDump.h"


signed int demodCount;
signed int demodCmdCount;
unsigned char ddrDemodPropData[MAX_PROPS][PROPS_LENTH];
unsigned char ddrDemodCmdData [MAX_CMDS ][CMDS_LENTH ];

#ifdef TEST_SIM
unsigned char tempData[MAX_PROPS][PROPS_LENTH];
#endif // TEST_SIM
/***********************************************************************************************************************
  Si2164B_DumpProperty function
  Use:        Returns a single property command encoded to i2c bytes

  Parameter: *api     the Si2164B context
  Parameter: prop     property command #
  Parameter: cmdByteBuffer -- buffer for data
  Behavior:  .
               The cmdByteBuffer is arranged so the first byte is the length of the I2C transaction
                The following bytes are the setProperty command.
  Returns:    0 if no error, an error code otherwise
 ***********************************************************************************************************************/
unsigned char Si2164B_DumpCommand     (L1_Si2164B_Context *api, unsigned int cmd_code)
{
//  if (Si2164B_L1_SendCommand2(api, cmd_code) != NO_Si2164B_ERROR) return ERROR_Si2164B_SENDING_COMMAND;
  Si2164B_L1_SendCommand2(api, cmd_code);

  memcpy(ddrDemodCmdData[demodCmdCount++],&api->i2c->writeBuffer,CMDS_LENTH);
  SiTRACE("Command  Bytes: %02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x\n",api->i2c->writeBuffer[0],api->i2c->writeBuffer[1],api->i2c->writeBuffer[2],api->i2c->writeBuffer[3],
                                                 api->i2c->writeBuffer[4],api->i2c->writeBuffer[5],api->i2c->writeBuffer[6],api->i2c->writeBuffer[7],api->i2c->writeBuffer[8]);
	SiTRACE("Si2164B_DumpCommand: Demod Command count : %d\n", demodCmdCount);
  return NO_Si2164B_ERROR;
}
/***********************************************************************************************************************
  Si2164B_DumpProperty function
  Use:        Returns a single property command encoded to i2c bytes

  Parameter: *api     the Si2164B context
  Parameter: prop     property command #
  Parameter: cmdByteBuffer -- buffer for data
  Behavior:  .
               The cmdByteBuffer is arranged so the first byte is the length of the I2C transaction
                The following bytes are the setProperty command.
  Returns:    0 if no error, an error code otherwise
***********************************************************************************************************************/
unsigned char Si2164B_DumpProperty    (L1_Si2164B_Context *api,signed int prop,unsigned char cmdByteBuffer[])
{
    signed int shadowData = 0;
    signed int propData = 0;
    //signed int writeAlways=1;  // set the writeAlways flag to 1 to write all properties to mem
    unsigned char reserved = 0;

    //if (writeAlways)//20131120,prevent tool
    //{
        Si2164B_PackProperty(api->propShadow, prop, &shadowData);
        Si2164B_PackProperty(api->prop, prop, &propData);
    //}
    //if (writeAlways || (shadowData != propData))//if (shadowData != propData)
    //{
        cmdByteBuffer[0] = 6;
        cmdByteBuffer[1] = Si2164B_SET_PROPERTY_CMD;
        cmdByteBuffer[2] = (unsigned char) ( ( reserved & Si2164B_SET_PROPERTY_CMD_RESERVED_MASK ) << Si2164B_SET_PROPERTY_CMD_RESERVED_LSB);
        cmdByteBuffer[3] = (unsigned char) ( ( prop     & Si2164B_SET_PROPERTY_CMD_PROP_MASK     ) << Si2164B_SET_PROPERTY_CMD_PROP_LSB    );
        cmdByteBuffer[4] = (unsigned char) ((( prop     & Si2164B_SET_PROPERTY_CMD_PROP_MASK     ) << Si2164B_SET_PROPERTY_CMD_PROP_LSB    )>>8);
        cmdByteBuffer[5] = (unsigned char) ( ( propData     & Si2164B_SET_PROPERTY_CMD_DATA_MASK     ) << Si2164B_SET_PROPERTY_CMD_DATA_LSB    );
        cmdByteBuffer[6] = (unsigned char) ((( propData     & Si2164B_SET_PROPERTY_CMD_DATA_MASK     ) << Si2164B_SET_PROPERTY_CMD_DATA_LSB    )>>8);
        SiTRACE("Property Bytes:0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",cmdByteBuffer[0],cmdByteBuffer[1],cmdByteBuffer[2],cmdByteBuffer[3],
                cmdByteBuffer[4],cmdByteBuffer[5],cmdByteBuffer[6]);
        return 1;
    //}
    //else
    //{
    //    return 0;
    //}
}
/***********************************************************************************************************************
  Si2164B_DumpProperties function
  Use:        Command to dump all changed properties to DDR memory.

  Parameter: *api     the Si2164B context

  Behavior:  .
               The cmdByteBuffer is arranged so the first byte is the length of the I2C transaction
                The following bytes are the setProperty command.
  Returns:    0 if no error, an error code otherwise
 ***********************************************************************************************************************/
signed int           Si2164B_DumpProperties  (L1_Si2164B_Context *api)
{
  unsigned char tns[7];
#ifdef    Si2164B_MASTER_IEN_PROP
  if (Si2164B_DumpProperty(api, Si2164B_MASTER_IEN_PROP_CODE                     , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_MASTER_IEN_PROP */
#ifdef    Si2164B_DD_BER_RESOL_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_BER_RESOL_PROP_CODE                   , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_BER_RESOL_PROP */
#ifdef    Si2164B_DD_CBER_RESOL_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_CBER_RESOL_PROP_CODE                  , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_CBER_RESOL_PROP */
#ifdef    Si2164B_DD_DISEQC_FREQ_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_DISEQC_FREQ_PROP_CODE                 , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_DISEQC_FREQ_PROP */
#ifdef    Si2164B_DD_DISEQC_PARAM_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_DISEQC_PARAM_PROP_CODE                , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_DISEQC_PARAM_PROP */

#ifdef    Si2164B_DD_FER_RESOL_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_FER_RESOL_PROP_CODE                   , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_FER_RESOL_PROP */
#ifdef    Si2164B_DD_IEN_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_IEN_PROP_CODE                         , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_IEN_PROP */
#ifdef    Si2164B_DD_IF_INPUT_FREQ_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_IF_INPUT_FREQ_PROP_CODE               , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_IF_INPUT_FREQ_PROP */
#ifdef    Si2164B_DD_INT_SENSE_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_INT_SENSE_PROP_CODE                   , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_INT_SENSE_PROP */
#ifdef    Si2164B_DD_MODE_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_MODE_PROP_CODE                        , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_MODE_PROP */
#ifdef    Si2164B_DD_PER_RESOL_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_PER_RESOL_PROP_CODE                   , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_PER_RESOL_PROP */
#ifdef    Si2164B_DD_RSQ_BER_THRESHOLD_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_RSQ_BER_THRESHOLD_PROP_CODE           , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_RSQ_BER_THRESHOLD_PROP */

#ifdef    Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_CODE           , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP */
#ifdef    Si2164B_DD_SEC_TS_SETUP_PAR_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_SEC_TS_SETUP_PAR_PROP_CODE             , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_SEC_TS_SETUP_PAR_PROP */
#ifdef    Si2164B_DD_SEC_TS_SETUP_SER_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_SEC_TS_SETUP_SER_PROP_CODE             , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_SEC_TS_SETUP_SER_PROP */
#ifdef    Si2164B_DD_SEC_TS_SLR_SERIAL_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_CODE            , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_SEC_TS_SLR_SERIAL_PROP */

#ifdef    Si2164B_DD_SSI_SQI_PARAM_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_SSI_SQI_PARAM_PROP_CODE               , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_SSI_SQI_PARAM_PROP */
#ifdef    Si2164B_DD_TS_FREQ_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_TS_FREQ_PROP_CODE                     , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_TS_FREQ_PROP */
#ifdef    Si2164B_DD_TS_MODE_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_TS_MODE_PROP_CODE                     , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_TS_MODE_PROP */
#ifdef    Si2164B_DD_TS_SERIAL_DIFF_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_TS_SERIAL_DIFF_PROP_CODE              , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_TS_SERIAL_DIFF_PROP */
#ifdef    Si2164B_DD_TS_SETUP_PAR_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_TS_SETUP_PAR_PROP_CODE                , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_TS_SETUP_PAR_PROP */
#ifdef    Si2164B_DD_TS_SETUP_SER_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_TS_SETUP_SER_PROP_CODE                , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_TS_SETUP_SER_PROP */
#ifdef    Si2164B_DD_TS_SLR_SERIAL_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DD_TS_SLR_SERIAL_PROP_CODE            , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DD_TS_SLR_SERIAL_PROP */

#ifdef    Si2164B_DVBC_ADC_CREST_FACTOR_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBC_ADC_CREST_FACTOR_PROP_CODE          , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBC_ADC_CREST_FACTOR_PROP */
#ifdef    Si2164B_DVBC_AFC_RANGE_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBC_AFC_RANGE_PROP_CODE                 , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBC_AFC_RANGE_PROP */
#ifdef    Si2164B_DVBC_CONSTELLATION_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBC_CONSTELLATION_PROP_CODE             , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBC_CONSTELLATION_PROP */
#ifdef    Si2164B_DVBC_SYMBOL_RATE_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBC_SYMBOL_RATE_PROP_CODE               , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBC_SYMBOL_RATE_PROP */



#ifdef    Si2164B_DVBC2_ADC_CREST_FACTOR_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBC2_ADC_CREST_FACTOR_PROP_CODE         , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBC2_ADC_CREST_FACTOR_PROP */
#ifdef    Si2164B_DVBC2_AFC_RANGE_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBC2_AFC_RANGE_PROP_CODE                , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBC2_AFC_RANGE_PROP */



#ifdef    Si2164B_DVBS_ADC_CREST_FACTOR_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBS_ADC_CREST_FACTOR_PROP_CODE          , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBS_ADC_CREST_FACTOR_PROP */
#ifdef    Si2164B_DVBS_AFC_RANGE_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBS_AFC_RANGE_PROP_CODE                 , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBS_AFC_RANGE_PROP */
#ifdef    Si2164B_DVBS_SYMBOL_RATE_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBS_SYMBOL_RATE_PROP_CODE               , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBS_SYMBOL_RATE_PROP */
#ifdef    Si2164B_DVBS2_ADC_CREST_FACTOR_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBS2_ADC_CREST_FACTOR_PROP_CODE         , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBS2_ADC_CREST_FACTOR_PROP */
#ifdef    Si2164B_DVBS2_AFC_RANGE_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBS2_AFC_RANGE_PROP_CODE                , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBS2_AFC_RANGE_PROP */
#ifdef    Si2164B_DVBS2_SYMBOL_RATE_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBS2_SYMBOL_RATE_PROP_CODE              , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBS2_SYMBOL_RATE_PROP */



#ifdef    Si2164B_DVBT_ADC_CREST_FACTOR_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBT_ADC_CREST_FACTOR_PROP_CODE          , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBT_ADC_CREST_FACTOR_PROP */
#ifdef    Si2164B_DVBT_AFC_RANGE_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBT_AFC_RANGE_PROP_CODE                 , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBT_AFC_RANGE_PROP */
#ifdef    Si2164B_DVBT_HIERARCHY_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBT_HIERARCHY_PROP_CODE                 , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBT_HIERARCHY_PROP */



#ifdef    Si2164B_DVBT2_ADC_CREST_FACTOR_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBT2_ADC_CREST_FACTOR_PROP_CODE         , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBT2_ADC_CREST_FACTOR_PROP */
#ifdef    Si2164B_DVBT2_AFC_RANGE_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBT2_AFC_RANGE_PROP_CODE                , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBT2_AFC_RANGE_PROP */
#ifdef    Si2164B_DVBT2_FEF_TUNER_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBT2_FEF_TUNER_PROP_CODE                , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBT2_FEF_TUNER_PROP */
#ifdef    Si2164B_DVBT2_MODE_PROP
  if (Si2164B_DumpProperty(api, Si2164B_DVBT2_MODE_PROP_CODE                     , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_DVBT2_MODE_PROP */



#ifdef    Si2164B_MCNS_ADC_CREST_FACTOR_PROP
  if (Si2164B_DumpProperty(api, Si2164B_MCNS_ADC_CREST_FACTOR_PROP_CODE          , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_MCNS_ADC_CREST_FACTOR_PROP */
#ifdef    Si2164B_MCNS_AFC_RANGE_PROP
  if (Si2164B_DumpProperty(api, Si2164B_MCNS_AFC_RANGE_PROP_CODE                 , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_MCNS_AFC_RANGE_PROP */
#ifdef    Si2164B_MCNS_CONSTELLATION_PROP
  if (Si2164B_DumpProperty(api, Si2164B_MCNS_CONSTELLATION_PROP_CODE             , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_MCNS_CONSTELLATION_PROP */
#ifdef    Si2164B_MCNS_SYMBOL_RATE_PROP
  if (Si2164B_DumpProperty(api, Si2164B_MCNS_SYMBOL_RATE_PROP_CODE               , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_MCNS_SYMBOL_RATE_PROP */


#ifdef    Si2164B_SCAN_FMAX_PROP
  if (Si2164B_DumpProperty(api, Si2164B_SCAN_FMAX_PROP_CODE                      , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_SCAN_FMAX_PROP */
#ifdef    Si2164B_SCAN_FMIN_PROP
  if (Si2164B_DumpProperty(api, Si2164B_SCAN_FMIN_PROP_CODE                      , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_SCAN_FMIN_PROP */
#ifdef    Si2164B_SCAN_IEN_PROP
  if (Si2164B_DumpProperty(api, Si2164B_SCAN_IEN_PROP_CODE                       , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_SCAN_IEN_PROP */
#ifdef    Si2164B_SCAN_INT_SENSE_PROP
  if (Si2164B_DumpProperty(api, Si2164B_SCAN_INT_SENSE_PROP_CODE                 , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_SCAN_INT_SENSE_PROP */

#ifdef    Si2164B_SCAN_SAT_CONFIG_PROP
  if (Si2164B_DumpProperty(api, Si2164B_SCAN_SAT_CONFIG_PROP_CODE                , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_SCAN_SAT_CONFIG_PROP */
#ifdef    Si2164B_SCAN_SAT_UNICABLE_BW_PROP
  if (Si2164B_DumpProperty(api, Si2164B_SCAN_SAT_UNICABLE_BW_PROP_CODE           , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_SCAN_SAT_UNICABLE_BW_PROP */
#ifdef    Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP
  if (Si2164B_DumpProperty(api, Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP_CODE, tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP */


#ifdef    Si2164B_SCAN_SYMB_RATE_MAX_PROP
  if (Si2164B_DumpProperty(api, Si2164B_SCAN_SYMB_RATE_MAX_PROP_CODE             , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_SCAN_SYMB_RATE_MAX_PROP */
#ifdef    Si2164B_SCAN_SYMB_RATE_MIN_PROP
  if (Si2164B_DumpProperty(api, Si2164B_SCAN_SYMB_RATE_MIN_PROP_CODE             , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_SCAN_SYMB_RATE_MIN_PROP */

#ifdef    Si2164B_SCAN_TER_CONFIG_PROP
  if (Si2164B_DumpProperty(api, Si2164B_SCAN_TER_CONFIG_PROP_CODE                , tns)) {memcpy(ddrDemodPropData[demodCount++],&tns,7);}
#endif /* Si2164B_SCAN_TER_CONFIG_PROP */





  return 0;
}
/***********************************************************************************************************************
  Si2164B_DumpAllToMem function
  Use:        Dumps all changed properties and the current tuner status to memory

  Parameter: *api     the Si2164B context

  Returns:    0 if no error, an error code otherwise
 ***********************************************************************************************************************/
unsigned char Si2164B_DumpAllToMem    (L1_Si2164B_Context *api, unsigned long *pDataAddr, unsigned long  *ulSize)
{

    demodCount=0;

    memset((unsigned char*)ddrDemodPropData, 0x00, sizeof(ddrDemodPropData));

    // this will reset all shadow properties to the default
    Si2164B_storePropertiesDefaults(api->propShadow);
    /* When used in compatibility mode with Si2167B, AUTO_DETECT/AUTO_T_T2 is not allowed */
    if (api->rsp->part_info.part == 67) {
      if (api->prop->dd_mode.modulation == Si2164B_DD_MODE_PROP_MODULATION_AUTO_DETECT ) {
        if (api->prop->dd_mode.auto_detect == Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_T_T2) {
            api->prop->dd_mode.modulation = Si2164B_DD_MODE_PROP_MODULATION_DVBT;
            SiTRACE ("Si2167B: Forcing dd_mode.modulation to 'DVBT'\n");
        }
      }
    }
    Si2164B_DumpProperties         (api);
	SiTRACE("Demod Property count : %d\n", demodCount);

    *pDataAddr = (unsigned long)ddrDemodPropData;
    *ulSize    =  sizeof(ddrDemodPropData);
     //Si2164B_DumpCmdToMem          (api);
#ifdef TEST_SIM
    signed int i;
    for (i = 0; i < MAX_PROPS; i++) {
        memcpy(tempData[i], &ddrDemodPropData[i], 9);
    }
#endif // TEST_SIM

    return NO_Si2164B_ERROR;
}
unsigned char Si2164B_DumpCmdToMem    (L1_Si2164B_Context *api, unsigned long *pDataAddr, unsigned long  *ulSize)
{

    demodCmdCount=0;

    memset((unsigned char*)ddrDemodCmdData, 0x00, sizeof(ddrDemodCmdData));

    /* Dump commands as in Si2164B_Configure */
    Si2164B_DumpCommand(api, Si2164B_DD_MP_DEFAULTS_CMD_CODE   ); /* 0x88 *///
    Si2164B_DumpCommand(api, Si2164B_DD_EXT_AGC_TER_CMD_CODE   ); /* 0x89 *///
    Si2164B_DumpCommand(api, Si2164B_DVBT2_FEF_CMD_CODE        ); /* 0x51 *///
    Si2164B_DumpCommand(api, Si2164B_DD_EXT_AGC_SAT_CMD_CODE   ); /* 0x8a *///
    Si2164B_DumpCommand(api, Si2164B_CONFIG_PINS_CMD_CODE      ); /* 0x12 *///
#if 0//20140228
	//20140226
    /* This needs to be done for Si2167B (part 67, ROM 0) with the following FW: */
    /*  FW 2_0b23 and above                                     */
    /*  FW 2_2b2  and above                                     */
    if (  (api->rsp->part_info.part     == 67 )
        & (api->rsp->part_info.romid    ==  0 ) ) {
      if ( (
            (api->rsp->get_rev.cmpmajor ==  '2')
          & (api->rsp->get_rev.cmpminor ==  '0')
          & (api->rsp->get_rev.cmpbuild >= 23)
         ) | (
            (api->rsp->get_rev.cmpmajor ==  '2')
          & (api->rsp->get_rev.cmpminor ==  '2')
          & (api->rsp->get_rev.cmpbuild >=  2)
         ) ) {
		    api->cmd->dd_set_reg.reg_code_lsb = 132;
		    api->cmd->dd_set_reg.reg_code_mid = 50;
		    api->cmd->dd_set_reg.reg_code_msb = 10;
		    api->cmd->dd_set_reg.value        = 1;
		     /* 0x8e for DVB-C with Si2167B 2_0b23: fw 2.0b23 is able to behave as 2.0b18 for 32QAM lock time with this code. */
        SiTRACE("[SiTRACE]Si2164B_DumpCommand: DD_SET_REG  (api, 132, 50, 10, 1) for 32QAM lock time with Si2167B\n");
        Si2164B_DumpCommand       (api, Si2164B_DD_SET_REG_CMD_CODE);
      }
    }
#endif
    /* Dump commands as in Si2164B_L2_lock_to_carrier */
    Si2164B_DumpCommand(api, Si2164B_DVBT2_PLP_SELECT_CMD_CODE ); /* 0x52 */
    /* Dump DD_RESTART */
    Si2164B_DumpCommand(api, Si2164B_DD_RESTART_CMD_CODE       ); /* 0x85 */

	SiTRACE("Demod Command count : %d\n", demodCmdCount);

    *pDataAddr = (unsigned long)ddrDemodCmdData;
    *ulSize    =  sizeof(ddrDemodCmdData);

    return NO_Si2164B_ERROR;
}

/***********************************************************************************************************************
  Si2164B_RestoreFromMem function
  Use:        restores all changed properties from the kernel mode and updates the prop and propShadow structures.

  Parameter: *api     the Si2164B context

  Returns:    0 if no error, an error code otherwise
 ***********************************************************************************************************************/
unsigned char Si2164B_RestoreFromMem  (L1_Si2164B_Context *api)
{
    signed int i;
    unsigned int prop;
    signed int data;
    // this will restore the data from ddr memory to the prop structures
    // 1      2   3   4    5     6
    //0x14 0x00 0x0a 0x07 0x00 0x01
    for (i=0;i<demodCount;i++)
    {
        SiTRACE("Property Bytes:0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", ddrDemodPropData[i][0], ddrDemodPropData[i][1], ddrDemodPropData[i][2], ddrDemodPropData[i][3], ddrDemodPropData[i][4], ddrDemodPropData[i][5], ddrDemodPropData[i][6]);
        if (ddrDemodPropData[i][1]==0x14)
        {
            prop = (ddrDemodPropData[i][4] << 8) | ddrDemodPropData[i][3];
            data = (ddrDemodPropData[i][6] << 8) | ddrDemodPropData[i][5];
            Si2164B_UnpackProperty (api->prop,prop,  data);
            Si2164B_UnpackProperty (api->propShadow,prop,  data);
        }
    }

    return NO_Si2164B_ERROR;
}

#ifdef TEST_SIM
unsigned char Data_Compare(void)
{
    signed int i, j;
    for (i = 0; i < MAX_PROPS; i++) {
        SiTRACE("Line %2d ", i);
        SiTRACE("::0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x "  , ddrDemodPropData[i][0],  ddrDemodPropData[i][1],  ddrDemodPropData[i][2],  ddrDemodPropData[i][3],  ddrDemodPropData[i][4],  ddrDemodPropData[i][5],  ddrDemodPropData[i][6],  ddrDemodPropData[i][7],  ddrDemodPropData[i][8]);
        SiTRACE("::0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", tempData[i][0], tempData[i][1], tempData[i][2], tempData[i][3], tempData[i][4], tempData[i][5], tempData[i][6], tempData[i][7], tempData[i][8]);

        for (j = 0; j < 9; j++) {
            if (ddrDemodPropData[i][j] != tempData[i][j]) {
                SiTRACE("DDR data is corrupted\n");
                return 1;
            }
        }
    }
    SiTRACE("DDR data is OK\n");
    return 1;
}
#endif // TEST_SIM
