/*************************************************************************************
                  Silicon Laboratories Broadcast Si2164B Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API commands definitions
   FILE: Si2164B_L1_Commands.c
   Supported IC : Si2164B
   Compiled for ROM 2 firmware 5_B_build_1
   Revision: V0.1.1.0
   Tag:  V0.1.1.0
   Date: March 30 2015
  (C) Copyright 2015, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
/* Change log:

 As from V0.1.0.0:
  <compatibility>[SILABS_SUPERSET] declaring signed   int  Si2164B_L1_GET_REG for all media
  <new_feature>[DVB-S2/MULTISTREAM] Adding Si2164B_DVBS2_STREAM_INFO and Si2164B_DVBS2_STREAM_SELECT commands

 As from V0.0.8.0:
  <improvement>[DD_RESTART/fast i2c] In Si2164B_L1_DD_RESTART: Wait at least 10 ms after DD_RESTART to make sure the DSP is started.
   (otherwise some commands may not succeed, especially when using TS_FREQ_RESOL=FINE).

 As from V0.0.7.0:
  <compatibility>[Si2167B/SAT] In Si2164B_L1_DVBS_STATUS: compiling code for SAT FREQ OFFSET workaround only when Si2167B_20_COMPATIBLE is defined
  <new_feature> [DD_RESTART] Adding Si2164B_DD_RESTART_EXT_CMD
  <new_feature> [DVB-S2/STATUS] Adding fields in DVBS2_STATUS_CMD_REPLY

 As from V0.0.6.0:
  <new_feature>[ISDBT/MODE] Adding dl_a/dl_b/dl_c to Si2164B_ISDBT_STATUS_CMD_REPLY_struct
  <improvement>[DD_RESTART/fast i2c] In Si2164B_L1_DD_RESTART: Wait at least 6 ms after DD_RESTART to make sure the DSP is started (otherwise some commands may not succeed).

 As from V0.0.5.0:
  <improvement>[POWER_UP] In Si2164B_L1_POWER_UP: adding 10ms delay after a power to be sure the firmware is ready to receive a command
  <improvement>[warnings/Si2167B] In Si2164B_L1_DVBS2_STATUS: declaring variables used for FREQ OFFSET workaround only if Si2167B_20_COMPATIBLE is defined
  <improvement>[Code_size] Using the textBuffer in Si2164B_L1_Context when filling text strings
    In Si2164B_L1_DD_TS_PINS

 As from V0.0.4.0:
  <improvement>[code_checker/Si2167B] In Si2164B_L1_DVBS2_STATUS: returning with an error if fe_clk_freq register is read as '0'.
   NB: this would happen only if:
    - Si2167B_20_COMPATIBLE is defined
    - The part is a Si2167B
    - i2c communication is suddenly broken after properly retrieving the DVB-S2 status response.

 As from V0.0.2.0:
  <improvement> In Si2164B_L1_DD_EXT_AGC_SAT/Si2164B_L1_DD_EXT_AGC_TER: removing range checks on agc1_kloopl/agc2_kloop,
   since these will always be within range due to their type. This may have generated warnings with some compilers
   when DEBUG_RANGE_CHECK is defined.

 As from V0.0.0.3:
  <compatibility> [Compilers] Using conversion functions, to be compatible with 8 bytes processing

 As from V0.0.0.0:
  Initial version (based on Si2164 code V0.3.4)
****************************************************************************************/

/* Before including the headers, define SiLevel and SiTAG */
//#define   SiLEVEL          1
//#define   SiTAG            api->i2c->tag

#define   Si2164B_COMMAND_PROTOTYPES
#define   DEBUG_RANGE_CHECK
#include "Si2164B_Platform_Definition.h"

/******   conversion functions, used to fill command response fields ***************************************************
  These functions provide compatibility with 8 bytes processing on some compilers
 ***********************************************************************************************************************/
unsigned char Si2164B_convert_to_byte (const unsigned char* buffer, unsigned char shift, unsigned char mask) {
  unsigned int rspBuffer = *buffer;
  return ((rspBuffer >> shift) & mask);
}
unsigned long Si2164B_convert_to_ulong(const unsigned char* buffer, unsigned char shift, unsigned long mask) {
  return ((( ( (unsigned long)buffer[0]) | ((unsigned long)buffer[1] << 8) | ((unsigned long)buffer[2]<<16) | ((unsigned long)buffer[3]<<24)) >> shift) & mask );
}
unsigned int  Si2164B_convert_to_uint (const unsigned char* buffer, unsigned char shift, unsigned int  mask) {
  return (( ( (unsigned int)buffer[0]) | (((unsigned int)buffer[1]) << 8) >> shift) & mask);
}
signed short         Si2164B_convert_to_int  (const unsigned char* buffer, unsigned char shift, unsigned int  mask) {
  return (( ( (unsigned int)buffer[0]) | (((unsigned int)buffer[1]) << 8) >> shift) & mask);
}
/***********************************************************************************************************************
  Si2164B_CurrentResponseStatus function
  Use:        status checking function
              Used to fill the Si2164B_COMMON_REPLY_struct members with the ptDataBuffer byte's bits
  Comments:   The status byte definition being identical for all commands,
              using this function to fill the status structure helps reducing the code size
  Parameter: ptDataBuffer  a single byte received when reading a command's response (the first byte)
  Returns:   0 if the err bit (bit 6) is unset, 1 otherwise
 ***********************************************************************************************************************/
unsigned char Si2164B_CurrentResponseStatus (L1_Si2164B_Context *api, unsigned char ptDataBuffer)
{
    api->status->ddint   =     Si2164B_convert_to_byte(&ptDataBuffer, 0, 0x01);
    api->status->scanint =     Si2164B_convert_to_byte(&ptDataBuffer, 1, 0x01);
    api->status->err     =     Si2164B_convert_to_byte(&ptDataBuffer, 6, 0x01);
    api->status->cts     =     Si2164B_convert_to_byte(&ptDataBuffer, 7, 0x01);
  return (api->status->err ? ERROR_Si2164B_ERR : NO_Si2164B_ERROR);
}

/***********************************************************************************************************************
  Si2164B_pollForCTS function
  Use:        CTS checking function
              Used to check the CTS bit until it is set before sending the next command
  Comments:   The status byte definition being identical for all commands,
              using this function to fill the status structure helps reducing the code size
              max timeout = 1000 ms

  Returns:   1 if the CTS bit is set, 0 otherwise
 ***********************************************************************************************************************/
unsigned char Si2164B_pollForCTS (L1_Si2164B_Context *api)
{
  unsigned char rspByteBuffer[1];
  unsigned int start_time;

  start_time = system_time();

  while (system_time() - start_time <500)  { /* wait a maximum of 1000ms */ // 20150902 [Change timeout 1000->500ms]
    if (L0_ReadCommandBytes(api->i2c, 1, rspByteBuffer) != 1) {
      SiTRACE("Si2164B_pollForCTS ERROR reading byte 0!\n");
      return ERROR_Si2164B_POLLING_CTS;
    }
    /* return OK if CTS set */
    if (rspByteBuffer[0] & 0x80) {
      return NO_Si2164B_ERROR;
    }
  }

  SiTRACE("Si2164B_pollForCTS ERROR CTS Timeout!\n");
  return ERROR_Si2164B_CTS_TIMEOUT;
}

/***********************************************************************************************************************
  Si2164B_pollForResponse function
  Use:        command response retrieval function
              Used to retrieve the command response in the provided buffer
  Comments:   The status byte definition being identical for all commands,
              using this function to fill the status structure helps reducing the code size
              max timeout = 1000 ms

  Parameter:  nbBytes          the number of response bytes to read
  Parameter:  pByteBuffer      a buffer into which bytes will be stored
  Returns:    0 if no error, an error code otherwise
 ***********************************************************************************************************************/
unsigned char Si2164B_pollForResponse (L1_Si2164B_Context *api, unsigned int nbBytes, unsigned char *pByteBuffer)
{
  unsigned char debugBuffer[7];
  unsigned int start_time;

  start_time = system_time();

  while (system_time() - start_time <500)  { /* wait a maximum of 1000ms */ // 20150902 [Change timeout 1000->500ms]
    if ((unsigned int)L0_ReadCommandBytes(api->i2c, nbBytes, pByteBuffer) != nbBytes) {
      SiTRACE("Si2164B_pollForResponse ERROR reading byte 0!\n");
      return ERROR_Si2164B_POLLING_RESPONSE;
    }
    /* return response err flag if CTS set */
    if (pByteBuffer[0] & 0x80)  {
      /* for debug purpose, read and trace 2 bytes in case the error bit is set */
      if (pByteBuffer[0] & 0x40)  {
        L0_ReadCommandBytes(api->i2c, 7, &(debugBuffer[0]) );
        SiTRACE("Si2164B debug bytes 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n", debugBuffer[0], debugBuffer[1], debugBuffer[2], debugBuffer[3], debugBuffer[4], debugBuffer[5], debugBuffer[6]);
      }
      return Si2164B_CurrentResponseStatus(api, pByteBuffer[0]);
    }
  }

  SiTRACE("Si2164B_pollForResponse ERROR CTS Timeout!\n");
  return ERROR_Si2164B_CTS_TIMEOUT;
}

#ifdef    Si2164B_CONFIG_CLKIO_CMD
/*---------------------------------------------------*/
/* Si2164B_CONFIG_CLKIO COMMAND                                   */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_CONFIG_CLKIO              (L1_Si2164B_Context *api,
                                                   unsigned char   output,
                                                   unsigned char   pre_driver_str,
                                                   unsigned char   driver_str)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[4];
    api->rsp->config_clkio.STATUS = api->status;

    SiTRACE("Si2164B CONFIG_CLKIO ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((output         > Si2164B_CONFIG_CLKIO_CMD_OUTPUT_MAX        ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("OUTPUT %d "        , output         );
    if ((pre_driver_str > Si2164B_CONFIG_CLKIO_CMD_PRE_DRIVER_STR_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("PRE_DRIVER_STR %d ", pre_driver_str );
    if ((driver_str     > Si2164B_CONFIG_CLKIO_CMD_DRIVER_STR_MAX    ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("DRIVER_STR %d "    , driver_str     );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_CONFIG_CLKIO_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( output         & Si2164B_CONFIG_CLKIO_CMD_OUTPUT_MASK         ) << Si2164B_CONFIG_CLKIO_CMD_OUTPUT_LSB        |
                                         ( pre_driver_str & Si2164B_CONFIG_CLKIO_CMD_PRE_DRIVER_STR_MASK ) << Si2164B_CONFIG_CLKIO_CMD_PRE_DRIVER_STR_LSB|
                                         ( driver_str     & Si2164B_CONFIG_CLKIO_CMD_DRIVER_STR_MASK     ) << Si2164B_CONFIG_CLKIO_CMD_DRIVER_STR_LSB    );

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing CONFIG_CLKIO bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 4, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling CONFIG_CLKIO response\n");
      return error_code;
    }

    api->rsp->config_clkio.mode           = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_CONFIG_CLKIO_RESPONSE_MODE_LSB            , Si2164B_CONFIG_CLKIO_RESPONSE_MODE_MASK           );
    api->rsp->config_clkio.pre_driver_str = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_CONFIG_CLKIO_RESPONSE_PRE_DRIVER_STR_LSB  , Si2164B_CONFIG_CLKIO_RESPONSE_PRE_DRIVER_STR_MASK );
    api->rsp->config_clkio.driver_str     = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_CONFIG_CLKIO_RESPONSE_DRIVER_STR_LSB      , Si2164B_CONFIG_CLKIO_RESPONSE_DRIVER_STR_MASK     );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_CONFIG_CLKIO_CMD */
#ifdef    Si2164B_CONFIG_I2C_CMD
/*---------------------------------------------------*/
/* Si2164B_CONFIG_I2C COMMAND                                       */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_CONFIG_I2C                (L1_Si2164B_Context *api,
                                                   unsigned char   subcode,
                                                   unsigned char   i2c_broadcast)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[3];
    api->rsp->config_i2c.STATUS = api->status;

    SiTRACE("Si2164B CONFIG_I2C ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((subcode       > Si2164B_CONFIG_I2C_CMD_SUBCODE_MAX      )  || (subcode       < Si2164B_CONFIG_I2C_CMD_SUBCODE_MIN      ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("SUBCODE %d "      , subcode       );
    if ((i2c_broadcast > Si2164B_CONFIG_I2C_CMD_I2C_BROADCAST_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("I2C_BROADCAST %d ", i2c_broadcast );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_CONFIG_I2C_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( subcode       & Si2164B_CONFIG_I2C_CMD_SUBCODE_MASK       ) << Si2164B_CONFIG_I2C_CMD_SUBCODE_LSB      );
    cmdByteBuffer[2] = (unsigned char) ( ( i2c_broadcast & Si2164B_CONFIG_I2C_CMD_I2C_BROADCAST_MASK ) << Si2164B_CONFIG_I2C_CMD_I2C_BROADCAST_LSB);

    if (L0_WriteCommandBytes(api->i2c, 3, cmdByteBuffer) != 3) {
      SiTRACE("Error writing CONFIG_I2C bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_CONFIG_I2C_CMD */
#ifdef    Si2164B_CONFIG_PINS_CMD
/*---------------------------------------------------*/
/* Si2164B_CONFIG_PINS COMMAND                                     */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_CONFIG_PINS               (L1_Si2164B_Context *api,
                                                   unsigned char   gpio0_mode,
                                                   unsigned char   gpio0_read,
                                                   unsigned char   gpio1_mode,
                                                   unsigned char   gpio1_read)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[3];
    unsigned char rspByteBuffer[3];
    api->rsp->config_pins.STATUS = api->status;

    SiTRACE("Si2164B CONFIG_PINS ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((gpio0_mode > Si2164B_CONFIG_PINS_CMD_GPIO0_MODE_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("GPIO0_MODE %d ", gpio0_mode );
    if ((gpio0_read > Si2164B_CONFIG_PINS_CMD_GPIO0_READ_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("GPIO0_READ %d ", gpio0_read );
    if ((gpio1_mode > Si2164B_CONFIG_PINS_CMD_GPIO1_MODE_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("GPIO1_MODE %d ", gpio1_mode );
    if ((gpio1_read > Si2164B_CONFIG_PINS_CMD_GPIO1_READ_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("GPIO1_READ %d ", gpio1_read );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_CONFIG_PINS_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( gpio0_mode & Si2164B_CONFIG_PINS_CMD_GPIO0_MODE_MASK ) << Si2164B_CONFIG_PINS_CMD_GPIO0_MODE_LSB|
                                         ( gpio0_read & Si2164B_CONFIG_PINS_CMD_GPIO0_READ_MASK ) << Si2164B_CONFIG_PINS_CMD_GPIO0_READ_LSB);
    cmdByteBuffer[2] = (unsigned char) ( ( gpio1_mode & Si2164B_CONFIG_PINS_CMD_GPIO1_MODE_MASK ) << Si2164B_CONFIG_PINS_CMD_GPIO1_MODE_LSB|
                                         ( gpio1_read & Si2164B_CONFIG_PINS_CMD_GPIO1_READ_MASK ) << Si2164B_CONFIG_PINS_CMD_GPIO1_READ_LSB);

    if (L0_WriteCommandBytes(api->i2c, 3, cmdByteBuffer) != 3) {
      SiTRACE("Error writing CONFIG_PINS bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 3, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling CONFIG_PINS response\n");
      return error_code;
    }

    api->rsp->config_pins.gpio0_mode  = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_CONFIG_PINS_RESPONSE_GPIO0_MODE_LSB   , Si2164B_CONFIG_PINS_RESPONSE_GPIO0_MODE_MASK  );
    api->rsp->config_pins.gpio0_state = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_CONFIG_PINS_RESPONSE_GPIO0_STATE_LSB  , Si2164B_CONFIG_PINS_RESPONSE_GPIO0_STATE_MASK );
    api->rsp->config_pins.gpio1_mode  = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_CONFIG_PINS_RESPONSE_GPIO1_MODE_LSB   , Si2164B_CONFIG_PINS_RESPONSE_GPIO1_MODE_MASK  );
    api->rsp->config_pins.gpio1_state = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_CONFIG_PINS_RESPONSE_GPIO1_STATE_LSB  , Si2164B_CONFIG_PINS_RESPONSE_GPIO1_STATE_MASK );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_CONFIG_PINS_CMD */
#ifdef    Si2164B_DD_BER_CMD
/*---------------------------------------------------*/
/* Si2164B_DD_BER COMMAND                                               */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DD_BER                    (L1_Si2164B_Context *api,
                                                   unsigned char   rst)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[3];
    api->rsp->dd_ber.STATUS = api->status;

    SiTRACE("Si2164B DD_BER ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((rst > Si2164B_DD_BER_CMD_RST_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("RST %d ", rst );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DD_BER_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( rst & Si2164B_DD_BER_CMD_RST_MASK ) << Si2164B_DD_BER_CMD_RST_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing DD_BER bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 3, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DD_BER response\n");
      return error_code;
    }

    api->rsp->dd_ber.exp  = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_BER_RESPONSE_EXP_LSB   , Si2164B_DD_BER_RESPONSE_EXP_MASK  );
    api->rsp->dd_ber.mant = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_BER_RESPONSE_MANT_LSB  , Si2164B_DD_BER_RESPONSE_MANT_MASK );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DD_BER_CMD */
#ifdef    Si2164B_DD_CBER_CMD
/*---------------------------------------------------*/
/* Si2164B_DD_CBER COMMAND                                             */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DD_CBER                   (L1_Si2164B_Context *api,
                                                   unsigned char   rst)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[3];
    api->rsp->dd_cber.STATUS = api->status;

    SiTRACE("Si2164B DD_CBER ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((rst > Si2164B_DD_CBER_CMD_RST_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("RST %d ", rst );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DD_CBER_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( rst & Si2164B_DD_CBER_CMD_RST_MASK ) << Si2164B_DD_CBER_CMD_RST_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing DD_CBER bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 3, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DD_CBER response\n");
      return error_code;
    }

    api->rsp->dd_cber.exp  = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_CBER_RESPONSE_EXP_LSB   , Si2164B_DD_CBER_RESPONSE_EXP_MASK  );
    api->rsp->dd_cber.mant = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_CBER_RESPONSE_MANT_LSB  , Si2164B_DD_CBER_RESPONSE_MANT_MASK );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DD_CBER_CMD */
#ifdef    Si2164B_DD_DISEQC_SEND_CMD
/*---------------------------------------------------*/
/* Si2164B_DD_DISEQC_SEND COMMAND                               */
/*---------------------------------------------------*/
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
                                                   unsigned char   msg_byte6)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[8];
    unsigned char rspByteBuffer[1];
    api->rsp->dd_diseqc_send.STATUS = api->status;

    SiTRACE("Si2164B DD_DISEQC_SEND ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((enable     > Si2164B_DD_DISEQC_SEND_CMD_ENABLE_MAX    ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("ENABLE %d "    , enable     );
    if ((cont_tone  > Si2164B_DD_DISEQC_SEND_CMD_CONT_TONE_MAX ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("CONT_TONE %d " , cont_tone  );
    if ((tone_burst > Si2164B_DD_DISEQC_SEND_CMD_TONE_BURST_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("TONE_BURST %d ", tone_burst );
    if ((burst_sel  > Si2164B_DD_DISEQC_SEND_CMD_BURST_SEL_MAX ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("BURST_SEL %d " , burst_sel  );
    if ((end_seq    > Si2164B_DD_DISEQC_SEND_CMD_END_SEQ_MAX   ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("END_SEQ %d "   , end_seq    );
    if ((msg_length > Si2164B_DD_DISEQC_SEND_CMD_MSG_LENGTH_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("MSG_LENGTH %d ", msg_length );
    SiTRACE("MSG_BYTE1 %d " , msg_byte1  );
    SiTRACE("MSG_BYTE2 %d " , msg_byte2  );
    SiTRACE("MSG_BYTE3 %d " , msg_byte3  );
    SiTRACE("MSG_BYTE4 %d " , msg_byte4  );
    SiTRACE("MSG_BYTE5 %d " , msg_byte5  );
    SiTRACE("MSG_BYTE6 %d " , msg_byte6  );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DD_DISEQC_SEND_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( enable     & Si2164B_DD_DISEQC_SEND_CMD_ENABLE_MASK     ) << Si2164B_DD_DISEQC_SEND_CMD_ENABLE_LSB    |
                                         ( cont_tone  & Si2164B_DD_DISEQC_SEND_CMD_CONT_TONE_MASK  ) << Si2164B_DD_DISEQC_SEND_CMD_CONT_TONE_LSB |
                                         ( tone_burst & Si2164B_DD_DISEQC_SEND_CMD_TONE_BURST_MASK ) << Si2164B_DD_DISEQC_SEND_CMD_TONE_BURST_LSB|
                                         ( burst_sel  & Si2164B_DD_DISEQC_SEND_CMD_BURST_SEL_MASK  ) << Si2164B_DD_DISEQC_SEND_CMD_BURST_SEL_LSB |
                                         ( end_seq    & Si2164B_DD_DISEQC_SEND_CMD_END_SEQ_MASK    ) << Si2164B_DD_DISEQC_SEND_CMD_END_SEQ_LSB   |
                                         ( msg_length & Si2164B_DD_DISEQC_SEND_CMD_MSG_LENGTH_MASK ) << Si2164B_DD_DISEQC_SEND_CMD_MSG_LENGTH_LSB);
    cmdByteBuffer[2] = (unsigned char) ( ( msg_byte1  & Si2164B_DD_DISEQC_SEND_CMD_MSG_BYTE1_MASK  ) << Si2164B_DD_DISEQC_SEND_CMD_MSG_BYTE1_LSB );
    cmdByteBuffer[3] = (unsigned char) ( ( msg_byte2  & Si2164B_DD_DISEQC_SEND_CMD_MSG_BYTE2_MASK  ) << Si2164B_DD_DISEQC_SEND_CMD_MSG_BYTE2_LSB );
    cmdByteBuffer[4] = (unsigned char) ( ( msg_byte3  & Si2164B_DD_DISEQC_SEND_CMD_MSG_BYTE3_MASK  ) << Si2164B_DD_DISEQC_SEND_CMD_MSG_BYTE3_LSB );
    cmdByteBuffer[5] = (unsigned char) ( ( msg_byte4  & Si2164B_DD_DISEQC_SEND_CMD_MSG_BYTE4_MASK  ) << Si2164B_DD_DISEQC_SEND_CMD_MSG_BYTE4_LSB );
    cmdByteBuffer[6] = (unsigned char) ( ( msg_byte5  & Si2164B_DD_DISEQC_SEND_CMD_MSG_BYTE5_MASK  ) << Si2164B_DD_DISEQC_SEND_CMD_MSG_BYTE5_LSB );
    cmdByteBuffer[7] = (unsigned char) ( ( msg_byte6  & Si2164B_DD_DISEQC_SEND_CMD_MSG_BYTE6_MASK  ) << Si2164B_DD_DISEQC_SEND_CMD_MSG_BYTE6_LSB );

    if (L0_WriteCommandBytes(api->i2c, 8, cmdByteBuffer) != 8) {
      SiTRACE("Error writing DD_DISEQC_SEND bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 1, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DD_DISEQC_SEND response\n");
      return error_code;
    }


    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DD_DISEQC_SEND_CMD */
#ifdef    Si2164B_DD_DISEQC_STATUS_CMD
/*---------------------------------------------------*/
/* Si2164B_DD_DISEQC_STATUS COMMAND                           */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DD_DISEQC_STATUS          (L1_Si2164B_Context *api,
                                                   unsigned char   listen)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[5];
    api->rsp->dd_diseqc_status.STATUS = api->status;

    SiTRACE("Si2164B DD_DISEQC_STATUS ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((listen > Si2164B_DD_DISEQC_STATUS_CMD_LISTEN_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("LISTEN %d ", listen );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DD_DISEQC_STATUS_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( listen & Si2164B_DD_DISEQC_STATUS_CMD_LISTEN_MASK ) << Si2164B_DD_DISEQC_STATUS_CMD_LISTEN_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing DD_DISEQC_STATUS bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 5, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DD_DISEQC_STATUS response\n");
      return error_code;
    }

    api->rsp->dd_diseqc_status.bus_state    = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_DISEQC_STATUS_RESPONSE_BUS_STATE_LSB     , Si2164B_DD_DISEQC_STATUS_RESPONSE_BUS_STATE_MASK    );
    api->rsp->dd_diseqc_status.reply_status = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_DISEQC_STATUS_RESPONSE_REPLY_STATUS_LSB  , Si2164B_DD_DISEQC_STATUS_RESPONSE_REPLY_STATUS_MASK );
    api->rsp->dd_diseqc_status.reply_length = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_DISEQC_STATUS_RESPONSE_REPLY_LENGTH_LSB  , Si2164B_DD_DISEQC_STATUS_RESPONSE_REPLY_LENGTH_MASK );
    api->rsp->dd_diseqc_status.reply_toggle = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_DISEQC_STATUS_RESPONSE_REPLY_TOGGLE_LSB  , Si2164B_DD_DISEQC_STATUS_RESPONSE_REPLY_TOGGLE_MASK );
    api->rsp->dd_diseqc_status.end_of_reply = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_DISEQC_STATUS_RESPONSE_END_OF_REPLY_LSB  , Si2164B_DD_DISEQC_STATUS_RESPONSE_END_OF_REPLY_MASK );
    api->rsp->dd_diseqc_status.diseqc_mode  = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_DISEQC_STATUS_RESPONSE_DISEQC_MODE_LSB   , Si2164B_DD_DISEQC_STATUS_RESPONSE_DISEQC_MODE_MASK  );
    api->rsp->dd_diseqc_status.reply_byte1  = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_DISEQC_STATUS_RESPONSE_REPLY_BYTE1_LSB   , Si2164B_DD_DISEQC_STATUS_RESPONSE_REPLY_BYTE1_MASK  );
    api->rsp->dd_diseqc_status.reply_byte2  = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_DD_DISEQC_STATUS_RESPONSE_REPLY_BYTE2_LSB   , Si2164B_DD_DISEQC_STATUS_RESPONSE_REPLY_BYTE2_MASK  );
    api->rsp->dd_diseqc_status.reply_byte3  = Si2164B_convert_to_byte  (&rspByteBuffer[ 4] , Si2164B_DD_DISEQC_STATUS_RESPONSE_REPLY_BYTE3_LSB   , Si2164B_DD_DISEQC_STATUS_RESPONSE_REPLY_BYTE3_MASK  );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DD_DISEQC_STATUS_CMD */
#ifdef    Si2164B_DD_EXT_AGC_SAT_CMD
/*---------------------------------------------------*/
/* Si2164B_DD_EXT_AGC_SAT COMMAND                               */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DD_EXT_AGC_SAT            (L1_Si2164B_Context *api,
                                                   unsigned char   agc_1_mode,
                                                   unsigned char   agc_1_inv,
                                                   unsigned char   agc_2_mode,
                                                   unsigned char   agc_2_inv,
                                                   unsigned char   agc_1_kloop,
                                                   unsigned char   agc_2_kloop,
                                                   unsigned char   agc_1_min,
                                                   unsigned char   agc_2_min)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[6];
    unsigned char rspByteBuffer[3];
    api->rsp->dd_ext_agc_sat.STATUS = api->status;

    SiTRACE("Si2164B DD_EXT_AGC_SAT ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((agc_1_mode  > Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MODE_MAX ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("AGC_1_MODE %d " , agc_1_mode  );
    if ((agc_1_inv   > Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_INV_MAX  ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("AGC_1_INV %d "  , agc_1_inv   );
    if ((agc_2_mode  > Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MODE_MAX ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("AGC_2_MODE %d " , agc_2_mode  );
    if ((agc_2_inv   > Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_INV_MAX  ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("AGC_2_INV %d "  , agc_2_inv   );
    SiTRACE("AGC_1_KLOOP %d ", agc_1_kloop );
    SiTRACE("AGC_2_KLOOP %d ", agc_2_kloop );
    SiTRACE("AGC_1_MIN %d "  , agc_1_min   );
    SiTRACE("AGC_2_MIN %d "  , agc_2_min   );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DD_EXT_AGC_SAT_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( agc_1_mode  & Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MODE_MASK  ) << Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MODE_LSB |
                                         ( agc_1_inv   & Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_INV_MASK   ) << Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_INV_LSB  |
                                         ( agc_2_mode  & Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MODE_MASK  ) << Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MODE_LSB |
                                         ( agc_2_inv   & Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_INV_MASK   ) << Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_INV_LSB  );
    cmdByteBuffer[2] = (unsigned char) ( ( agc_1_kloop & Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_KLOOP_MASK ) << Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_KLOOP_LSB);
    cmdByteBuffer[3] = (unsigned char) ( ( agc_2_kloop & Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_KLOOP_MASK ) << Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_KLOOP_LSB);
    cmdByteBuffer[4] = (unsigned char) ( ( agc_1_min   & Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MIN_MASK   ) << Si2164B_DD_EXT_AGC_SAT_CMD_AGC_1_MIN_LSB  );
    cmdByteBuffer[5] = (unsigned char) ( ( agc_2_min   & Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MIN_MASK   ) << Si2164B_DD_EXT_AGC_SAT_CMD_AGC_2_MIN_LSB  );

    if (L0_WriteCommandBytes(api->i2c, 6, cmdByteBuffer) != 6) {
      SiTRACE("Error writing DD_EXT_AGC_SAT bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 3, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DD_EXT_AGC_SAT response\n");
      return error_code;
    }

    api->rsp->dd_ext_agc_sat.agc_1_level = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_EXT_AGC_SAT_RESPONSE_AGC_1_LEVEL_LSB  , Si2164B_DD_EXT_AGC_SAT_RESPONSE_AGC_1_LEVEL_MASK );
    api->rsp->dd_ext_agc_sat.agc_2_level = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_EXT_AGC_SAT_RESPONSE_AGC_2_LEVEL_LSB  , Si2164B_DD_EXT_AGC_SAT_RESPONSE_AGC_2_LEVEL_MASK );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DD_EXT_AGC_SAT_CMD */

#ifdef    Si2164B_DD_EXT_AGC_TER_CMD
/*---------------------------------------------------*/
/* Si2164B_DD_EXT_AGC_TER COMMAND                               */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DD_EXT_AGC_TER            (L1_Si2164B_Context *api,
                                                   unsigned char   agc_1_mode,
                                                   unsigned char   agc_1_inv,
                                                   unsigned char   agc_2_mode,
                                                   unsigned char   agc_2_inv,
                                                   unsigned char   agc_1_kloop,
                                                   unsigned char   agc_2_kloop,
                                                   unsigned char   agc_1_min,
                                                   unsigned char   agc_2_min)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[6];
    unsigned char rspByteBuffer[3];
    api->rsp->dd_ext_agc_ter.STATUS = api->status;

    SiTRACE("Si2164B DD_EXT_AGC_TER ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((agc_1_mode  > Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_MAX ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("AGC_1_MODE %d " , agc_1_mode  );
    if ((agc_1_inv   > Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_INV_MAX  ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("AGC_1_INV %d "  , agc_1_inv   );
    if ((agc_2_mode  > Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MODE_MAX ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("AGC_2_MODE %d " , agc_2_mode  );
    if ((agc_2_inv   > Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_INV_MAX  ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("AGC_2_INV %d "  , agc_2_inv   );
    SiTRACE("AGC_1_KLOOP %d ", agc_1_kloop );
    SiTRACE("AGC_2_KLOOP %d ", agc_2_kloop );
    SiTRACE("AGC_1_MIN %d "  , agc_1_min   );
    SiTRACE("AGC_2_MIN %d "  , agc_2_min   );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DD_EXT_AGC_TER_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( agc_1_mode  & Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_MASK  ) << Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MODE_LSB |
                                         ( agc_1_inv   & Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_INV_MASK   ) << Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_INV_LSB  |
                                         ( agc_2_mode  & Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MODE_MASK  ) << Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MODE_LSB |
                                         ( agc_2_inv   & Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_INV_MASK   ) << Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_INV_LSB  );
    cmdByteBuffer[2] = (unsigned char) ( ( agc_1_kloop & Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_KLOOP_MASK ) << Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_KLOOP_LSB);
    cmdByteBuffer[3] = (unsigned char) ( ( agc_2_kloop & Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_KLOOP_MASK ) << Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_KLOOP_LSB);
    cmdByteBuffer[4] = (unsigned char) ( ( agc_1_min   & Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MIN_MASK   ) << Si2164B_DD_EXT_AGC_TER_CMD_AGC_1_MIN_LSB  );
    cmdByteBuffer[5] = (unsigned char) ( ( agc_2_min   & Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MIN_MASK   ) << Si2164B_DD_EXT_AGC_TER_CMD_AGC_2_MIN_LSB  );

    if (L0_WriteCommandBytes(api->i2c, 6, cmdByteBuffer) != 6) {
      SiTRACE("Error writing DD_EXT_AGC_TER bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 3, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DD_EXT_AGC_TER response\n");
      return error_code;
    }

    api->rsp->dd_ext_agc_ter.agc_1_level = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_EXT_AGC_TER_RESPONSE_AGC_1_LEVEL_LSB  , Si2164B_DD_EXT_AGC_TER_RESPONSE_AGC_1_LEVEL_MASK );
    api->rsp->dd_ext_agc_ter.agc_2_level = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_EXT_AGC_TER_RESPONSE_AGC_2_LEVEL_LSB  , Si2164B_DD_EXT_AGC_TER_RESPONSE_AGC_2_LEVEL_MASK );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DD_EXT_AGC_TER_CMD */

#ifdef    Si2164B_DD_FER_CMD
/*---------------------------------------------------*/
/* Si2164B_DD_FER COMMAND                                               */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DD_FER                    (L1_Si2164B_Context *api,
                                                   unsigned char   rst)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[3];
    api->rsp->dd_fer.STATUS = api->status;

    SiTRACE("Si2164B DD_FER ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((rst > Si2164B_DD_FER_CMD_RST_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("RST %d ", rst );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DD_FER_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( rst & Si2164B_DD_FER_CMD_RST_MASK ) << Si2164B_DD_FER_CMD_RST_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing DD_FER bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 3, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DD_FER response\n");
      return error_code;
    }

    api->rsp->dd_fer.exp  = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_FER_RESPONSE_EXP_LSB   , Si2164B_DD_FER_RESPONSE_EXP_MASK  );
    api->rsp->dd_fer.mant = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_FER_RESPONSE_MANT_LSB  , Si2164B_DD_FER_RESPONSE_MANT_MASK );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DD_FER_CMD */
#ifdef    Si2164B_DD_GET_REG_CMD
/*---------------------------------------------------*/
/* Si2164B_DD_GET_REG COMMAND                                       */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DD_GET_REG                (L1_Si2164B_Context *api,
                                                   unsigned char   reg_code_lsb,
                                                   unsigned char   reg_code_mid,
                                                   unsigned char   reg_code_msb)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[4];
    unsigned char rspByteBuffer[5];
    api->rsp->dd_get_reg.STATUS = api->status;

    SiTRACE("Si2164B DD_GET_REG ");
  #ifdef   DEBUG_RANGE_CHECK
    SiTRACE("REG_CODE_LSB %d ", reg_code_lsb );
    SiTRACE("REG_CODE_MID %d ", reg_code_mid );
    SiTRACE("REG_CODE_MSB %d ", reg_code_msb );
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DD_GET_REG_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( reg_code_lsb & Si2164B_DD_GET_REG_CMD_REG_CODE_LSB_MASK ) << Si2164B_DD_GET_REG_CMD_REG_CODE_LSB_LSB);
    cmdByteBuffer[2] = (unsigned char) ( ( reg_code_mid & Si2164B_DD_GET_REG_CMD_REG_CODE_MID_MASK ) << Si2164B_DD_GET_REG_CMD_REG_CODE_MID_LSB);
    cmdByteBuffer[3] = (unsigned char) ( ( reg_code_msb & Si2164B_DD_GET_REG_CMD_REG_CODE_MSB_MASK ) << Si2164B_DD_GET_REG_CMD_REG_CODE_MSB_LSB);

    if (L0_WriteCommandBytes(api->i2c, 4, cmdByteBuffer) != 4) {
      SiTRACE("Error writing DD_GET_REG bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 5, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DD_GET_REG response\n");
      return error_code;
    }

    api->rsp->dd_get_reg.data1 = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_GET_REG_RESPONSE_DATA1_LSB  , Si2164B_DD_GET_REG_RESPONSE_DATA1_MASK );
    api->rsp->dd_get_reg.data2 = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_GET_REG_RESPONSE_DATA2_LSB  , Si2164B_DD_GET_REG_RESPONSE_DATA2_MASK );
    api->rsp->dd_get_reg.data3 = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_DD_GET_REG_RESPONSE_DATA3_LSB  , Si2164B_DD_GET_REG_RESPONSE_DATA3_MASK );
    api->rsp->dd_get_reg.data4 = Si2164B_convert_to_byte  (&rspByteBuffer[ 4] , Si2164B_DD_GET_REG_RESPONSE_DATA4_LSB  , Si2164B_DD_GET_REG_RESPONSE_DATA4_MASK );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DD_GET_REG_CMD */
#ifdef    Si2164B_DD_MP_DEFAULTS_CMD
/*---------------------------------------------------*/
/* Si2164B_DD_MP_DEFAULTS COMMAND                               */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DD_MP_DEFAULTS            (L1_Si2164B_Context *api,
                                                   unsigned char   mp_a_mode,
                                                   unsigned char   mp_b_mode,
                                                   unsigned char   mp_c_mode,
                                                   unsigned char   mp_d_mode)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[5];
    unsigned char rspByteBuffer[5];
    api->rsp->dd_mp_defaults.STATUS = api->status;

    SiTRACE("Si2164B DD_MP_DEFAULTS ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((mp_a_mode > Si2164B_DD_MP_DEFAULTS_CMD_MP_A_MODE_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("MP_A_MODE %d ", mp_a_mode );
    if ((mp_b_mode > Si2164B_DD_MP_DEFAULTS_CMD_MP_B_MODE_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("MP_B_MODE %d ", mp_b_mode );
    if ((mp_c_mode > Si2164B_DD_MP_DEFAULTS_CMD_MP_C_MODE_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("MP_C_MODE %d ", mp_c_mode );
    if ((mp_d_mode > Si2164B_DD_MP_DEFAULTS_CMD_MP_D_MODE_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("MP_D_MODE %d ", mp_d_mode );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DD_MP_DEFAULTS_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( mp_a_mode & Si2164B_DD_MP_DEFAULTS_CMD_MP_A_MODE_MASK ) << Si2164B_DD_MP_DEFAULTS_CMD_MP_A_MODE_LSB);
    cmdByteBuffer[2] = (unsigned char) ( ( mp_b_mode & Si2164B_DD_MP_DEFAULTS_CMD_MP_B_MODE_MASK ) << Si2164B_DD_MP_DEFAULTS_CMD_MP_B_MODE_LSB);
    cmdByteBuffer[3] = (unsigned char) ( ( mp_c_mode & Si2164B_DD_MP_DEFAULTS_CMD_MP_C_MODE_MASK ) << Si2164B_DD_MP_DEFAULTS_CMD_MP_C_MODE_LSB);
    cmdByteBuffer[4] = (unsigned char) ( ( mp_d_mode & Si2164B_DD_MP_DEFAULTS_CMD_MP_D_MODE_MASK ) << Si2164B_DD_MP_DEFAULTS_CMD_MP_D_MODE_LSB);

    if (L0_WriteCommandBytes(api->i2c, 5, cmdByteBuffer) != 5) {
      SiTRACE("Error writing DD_MP_DEFAULTS bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 5, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DD_MP_DEFAULTS response\n");
      return error_code;
    }

    api->rsp->dd_mp_defaults.mp_a_mode = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_MP_DEFAULTS_RESPONSE_MP_A_MODE_LSB  , Si2164B_DD_MP_DEFAULTS_RESPONSE_MP_A_MODE_MASK );
    api->rsp->dd_mp_defaults.mp_b_mode = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_MP_DEFAULTS_RESPONSE_MP_B_MODE_LSB  , Si2164B_DD_MP_DEFAULTS_RESPONSE_MP_B_MODE_MASK );
    api->rsp->dd_mp_defaults.mp_c_mode = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_DD_MP_DEFAULTS_RESPONSE_MP_C_MODE_LSB  , Si2164B_DD_MP_DEFAULTS_RESPONSE_MP_C_MODE_MASK );
    api->rsp->dd_mp_defaults.mp_d_mode = Si2164B_convert_to_byte  (&rspByteBuffer[ 4] , Si2164B_DD_MP_DEFAULTS_RESPONSE_MP_D_MODE_LSB  , Si2164B_DD_MP_DEFAULTS_RESPONSE_MP_D_MODE_MASK );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DD_MP_DEFAULTS_CMD */
#ifdef    Si2164B_DD_PER_CMD
/*---------------------------------------------------*/
/* Si2164B_DD_PER COMMAND                                               */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DD_PER                    (L1_Si2164B_Context *api,
                                                   unsigned char   rst)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[3];
    api->rsp->dd_per.STATUS = api->status;

    SiTRACE("Si2164B DD_PER ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((rst > Si2164B_DD_PER_CMD_RST_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("RST %d ", rst );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DD_PER_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( rst & Si2164B_DD_PER_CMD_RST_MASK ) << Si2164B_DD_PER_CMD_RST_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing DD_PER bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 3, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DD_PER response\n");
      return error_code;
    }

    api->rsp->dd_per.exp  = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_PER_RESPONSE_EXP_LSB   , Si2164B_DD_PER_RESPONSE_EXP_MASK  );
    api->rsp->dd_per.mant = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_PER_RESPONSE_MANT_LSB  , Si2164B_DD_PER_RESPONSE_MANT_MASK );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DD_PER_CMD */
#ifdef    Si2164B_DD_RESTART_CMD
/*---------------------------------------------------*/
/* Si2164B_DD_RESTART COMMAND                                       */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DD_RESTART                (L1_Si2164B_Context *api)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[1];
    unsigned char rspByteBuffer[1];
    api->rsp->dd_restart.STATUS = api->status;

    SiTRACE("Si2164B DD_RESTART ");
    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DD_RESTART_CMD;

    if (L0_WriteCommandBytes(api->i2c, 1, cmdByteBuffer) != 1) {
      SiTRACE("Error writing DD_RESTART bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    /* Wait at least 10 ms after DD_RESTART to make sure the DSP is started (otherwise some commands may not succeed) */
    if(api->i2c->address != 0x00)
		system_wait(10);

    error_code = Si2164B_pollForResponse(api, 1, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DD_RESTART response\n");
      return error_code;
    }


    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DD_RESTART_CMD */
#ifdef    Si2164B_DD_RESTART_EXT_CMD
/*---------------------------------------------------*/
/* Si2164B_DD_RESTART_EXT COMMAND                               */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DD_RESTART_EXT            (L1_Si2164B_Context *api,
                                                   unsigned char   freq_plan,
                                                   unsigned char   freq_plan_ts_clk,
                                                   unsigned long   tuned_rf_freq)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[8];
    unsigned char rspByteBuffer[1];
    api->rsp->dd_restart_ext.STATUS = api->status;

    SiTRACE("Si2164B DD_RESTART_EXT ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((freq_plan        > Si2164B_DD_RESTART_EXT_CMD_FREQ_PLAN_MAX       ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("FREQ_PLAN %d "       , freq_plan        );
    if ((freq_plan_ts_clk > Si2164B_DD_RESTART_EXT_CMD_FREQ_PLAN_TS_CLK_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("FREQ_PLAN_TS_CLK %d ", freq_plan_ts_clk );
    SiTRACE("TUNED_RF_FREQ %d "   , tuned_rf_freq    );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DD_RESTART_EXT_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( freq_plan        & Si2164B_DD_RESTART_EXT_CMD_FREQ_PLAN_MASK        ) << Si2164B_DD_RESTART_EXT_CMD_FREQ_PLAN_LSB       |
                                         ( freq_plan_ts_clk & Si2164B_DD_RESTART_EXT_CMD_FREQ_PLAN_TS_CLK_MASK ) << Si2164B_DD_RESTART_EXT_CMD_FREQ_PLAN_TS_CLK_LSB);
    cmdByteBuffer[2] = (unsigned char)0x00;
    cmdByteBuffer[3] = (unsigned char)0x00;
    cmdByteBuffer[4] = (unsigned char) ( ( tuned_rf_freq    & Si2164B_DD_RESTART_EXT_CMD_TUNED_RF_FREQ_MASK    ) << Si2164B_DD_RESTART_EXT_CMD_TUNED_RF_FREQ_LSB   );
    cmdByteBuffer[5] = (unsigned char) ((( tuned_rf_freq    & Si2164B_DD_RESTART_EXT_CMD_TUNED_RF_FREQ_MASK    ) << Si2164B_DD_RESTART_EXT_CMD_TUNED_RF_FREQ_LSB   )>>8);
    cmdByteBuffer[6] = (unsigned char) ((( tuned_rf_freq    & Si2164B_DD_RESTART_EXT_CMD_TUNED_RF_FREQ_MASK    ) << Si2164B_DD_RESTART_EXT_CMD_TUNED_RF_FREQ_LSB   )>>16);
    cmdByteBuffer[7] = (unsigned char) ((( tuned_rf_freq    & Si2164B_DD_RESTART_EXT_CMD_TUNED_RF_FREQ_MASK    ) << Si2164B_DD_RESTART_EXT_CMD_TUNED_RF_FREQ_LSB   )>>24);

    if (L0_WriteCommandBytes(api->i2c, 8, cmdByteBuffer) != 8) {
      SiTRACE("Error writing DD_RESTART_EXT bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 1, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DD_RESTART_EXT response\n");
      return error_code;
    }


    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DD_RESTART_EXT_CMD */
#ifdef    Si2164B_DD_SET_REG_CMD
/*---------------------------------------------------*/
/* Si2164B_DD_SET_REG COMMAND                                       */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DD_SET_REG                (L1_Si2164B_Context *api,
                                                   unsigned char   reg_code_lsb,
                                                   unsigned char   reg_code_mid,
                                                   unsigned char   reg_code_msb,
                                                   unsigned long   value)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[8];
    unsigned char rspByteBuffer[1];
    api->rsp->dd_set_reg.STATUS = api->status;

    SiTRACE("Si2164B DD_SET_REG ");
  #ifdef   DEBUG_RANGE_CHECK
    SiTRACE("REG_CODE_LSB %d ", reg_code_lsb );
    SiTRACE("REG_CODE_MID %d ", reg_code_mid );
    SiTRACE("REG_CODE_MSB %d ", reg_code_msb );
    SiTRACE("VALUE %ld "      , value        );
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DD_SET_REG_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( reg_code_lsb & Si2164B_DD_SET_REG_CMD_REG_CODE_LSB_MASK ) << Si2164B_DD_SET_REG_CMD_REG_CODE_LSB_LSB);
    cmdByteBuffer[2] = (unsigned char) ( ( reg_code_mid & Si2164B_DD_SET_REG_CMD_REG_CODE_MID_MASK ) << Si2164B_DD_SET_REG_CMD_REG_CODE_MID_LSB);
    cmdByteBuffer[3] = (unsigned char) ( ( reg_code_msb & Si2164B_DD_SET_REG_CMD_REG_CODE_MSB_MASK ) << Si2164B_DD_SET_REG_CMD_REG_CODE_MSB_LSB);
    cmdByteBuffer[4] = (unsigned char) ( ( value        & Si2164B_DD_SET_REG_CMD_VALUE_MASK        ) << Si2164B_DD_SET_REG_CMD_VALUE_LSB       );
    cmdByteBuffer[5] = (unsigned char) ((( value        & Si2164B_DD_SET_REG_CMD_VALUE_MASK        ) << Si2164B_DD_SET_REG_CMD_VALUE_LSB       )>>8);
    cmdByteBuffer[6] = (unsigned char) ((( value        & Si2164B_DD_SET_REG_CMD_VALUE_MASK        ) << Si2164B_DD_SET_REG_CMD_VALUE_LSB       )>>16);
    cmdByteBuffer[7] = (unsigned char) ((( value        & Si2164B_DD_SET_REG_CMD_VALUE_MASK        ) << Si2164B_DD_SET_REG_CMD_VALUE_LSB       )>>24);

    if (L0_WriteCommandBytes(api->i2c, 8, cmdByteBuffer) != 8) {
      SiTRACE("Error writing DD_SET_REG bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 1, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DD_SET_REG response\n");
      return error_code;
    }


    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DD_SET_REG_CMD */
#ifdef    Si2164B_DD_SSI_SQI_CMD
/*---------------------------------------------------*/
/* Si2164B_DD_SSI_SQI COMMAND                                       */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DD_SSI_SQI                (L1_Si2164B_Context *api,
                                                            signed char  tuner_rssi)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[3];
    api->rsp->dd_ssi_sqi.STATUS = api->status;

    SiTRACE("Si2164B DD_SSI_SQI ");
  #ifdef   DEBUG_RANGE_CHECK
    SiTRACE("TUNER_RSSI %d ", tuner_rssi );
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DD_SSI_SQI_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( tuner_rssi & Si2164B_DD_SSI_SQI_CMD_TUNER_RSSI_MASK ) << Si2164B_DD_SSI_SQI_CMD_TUNER_RSSI_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing DD_SSI_SQI bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 3, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DD_SSI_SQI response\n");
      return error_code;
    }

    api->rsp->dd_ssi_sqi.ssi = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_SSI_SQI_RESPONSE_SSI_LSB  , Si2164B_DD_SSI_SQI_RESPONSE_SSI_MASK );
    api->rsp->dd_ssi_sqi.sqi = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_SSI_SQI_RESPONSE_SQI_LSB  , Si2164B_DD_SSI_SQI_RESPONSE_SQI_MASK );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DD_SSI_SQI_CMD */
#ifdef    Si2164B_DD_STATUS_CMD
/*---------------------------------------------------*/
/* Si2164B_DD_STATUS COMMAND                                         */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DD_STATUS                 (L1_Si2164B_Context *api,
                                                   unsigned char   intack)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[8];
    api->rsp->dd_status.STATUS = api->status;

    SiTRACE("Si2164B DD_STATUS ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((intack > Si2164B_DD_STATUS_CMD_INTACK_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("INTACK %d ", intack );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DD_STATUS_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( intack & Si2164B_DD_STATUS_CMD_INTACK_MASK ) << Si2164B_DD_STATUS_CMD_INTACK_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing DD_STATUS bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 8, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DD_STATUS response\n");
      return error_code;
    }

    api->rsp->dd_status.pclint       = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_STATUS_RESPONSE_PCLINT_LSB        , Si2164B_DD_STATUS_RESPONSE_PCLINT_MASK       );
    api->rsp->dd_status.dlint        = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_STATUS_RESPONSE_DLINT_LSB         , Si2164B_DD_STATUS_RESPONSE_DLINT_MASK        );
    api->rsp->dd_status.berint       = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_STATUS_RESPONSE_BERINT_LSB        , Si2164B_DD_STATUS_RESPONSE_BERINT_MASK       );
    api->rsp->dd_status.uncorint     = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_STATUS_RESPONSE_UNCORINT_LSB      , Si2164B_DD_STATUS_RESPONSE_UNCORINT_MASK     );
    api->rsp->dd_status.rsqint_bit5  = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_STATUS_RESPONSE_RSQINT_BIT5_LSB   , Si2164B_DD_STATUS_RESPONSE_RSQINT_BIT5_MASK  );
    api->rsp->dd_status.rsqint_bit6  = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_STATUS_RESPONSE_RSQINT_BIT6_LSB   , Si2164B_DD_STATUS_RESPONSE_RSQINT_BIT6_MASK  );
    api->rsp->dd_status.rsqint_bit7  = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_STATUS_RESPONSE_RSQINT_BIT7_LSB   , Si2164B_DD_STATUS_RESPONSE_RSQINT_BIT7_MASK  );
    api->rsp->dd_status.pcl          = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_STATUS_RESPONSE_PCL_LSB           , Si2164B_DD_STATUS_RESPONSE_PCL_MASK          );
    api->rsp->dd_status.dl           = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_STATUS_RESPONSE_DL_LSB            , Si2164B_DD_STATUS_RESPONSE_DL_MASK           );
    api->rsp->dd_status.ber          = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_STATUS_RESPONSE_BER_LSB           , Si2164B_DD_STATUS_RESPONSE_BER_MASK          );
    api->rsp->dd_status.uncor        = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_STATUS_RESPONSE_UNCOR_LSB         , Si2164B_DD_STATUS_RESPONSE_UNCOR_MASK        );
    api->rsp->dd_status.rsqstat_bit5 = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_STATUS_RESPONSE_RSQSTAT_BIT5_LSB  , Si2164B_DD_STATUS_RESPONSE_RSQSTAT_BIT5_MASK );
    api->rsp->dd_status.rsqstat_bit6 = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_STATUS_RESPONSE_RSQSTAT_BIT6_LSB  , Si2164B_DD_STATUS_RESPONSE_RSQSTAT_BIT6_MASK );
    api->rsp->dd_status.rsqstat_bit7 = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_STATUS_RESPONSE_RSQSTAT_BIT7_LSB  , Si2164B_DD_STATUS_RESPONSE_RSQSTAT_BIT7_MASK );
    api->rsp->dd_status.modulation   = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_DD_STATUS_RESPONSE_MODULATION_LSB    , Si2164B_DD_STATUS_RESPONSE_MODULATION_MASK   );
    api->rsp->dd_status.ts_bit_rate  = Si2164B_convert_to_uint  (&rspByteBuffer[ 4] , Si2164B_DD_STATUS_RESPONSE_TS_BIT_RATE_LSB   , Si2164B_DD_STATUS_RESPONSE_TS_BIT_RATE_MASK  );
    api->rsp->dd_status.ts_clk_freq  = Si2164B_convert_to_uint  (&rspByteBuffer[ 6] , Si2164B_DD_STATUS_RESPONSE_TS_CLK_FREQ_LSB   , Si2164B_DD_STATUS_RESPONSE_TS_CLK_FREQ_MASK  );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DD_STATUS_CMD */
#ifdef    Si2164B_DD_TS_PINS_CMD
/*---------------------------------------------------*/
/* Si2164B_DD_TS_PINS COMMAND                                       */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DD_TS_PINS                (L1_Si2164B_Context *api,
                                                   unsigned char   primary_ts_mode,
                                                   unsigned char   primary_ts_activity,
                                                   unsigned char   secondary_ts_mode,
                                                   unsigned char   secondary_ts_activity)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[3];
    unsigned char rspByteBuffer[3];
#ifdef    Si2164B_GET_COMMAND_STRINGS
    unsigned char *msg;
#endif /* Si2164B_GET_COMMAND_STRINGS */
    api->rsp->dd_ts_pins.STATUS = api->status;

    SiTRACE("Si2164B DD_TS_PINS ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((primary_ts_mode       > Si2164B_DD_TS_PINS_CMD_PRIMARY_TS_MODE_MAX      ) ) {error_code++; SiTRACE("\nOut of range: ");};
      if (primary_ts_mode   == Si2164B_DD_TS_PINS_CMD_PRIMARY_TS_MODE_DRIVE_TS   ) { SiTRACE("PRIMARY_TS_MODE    DRIVE_TS  " ); }
      if (primary_ts_mode   == Si2164B_DD_TS_PINS_CMD_PRIMARY_TS_MODE_NOT_USED   ) { SiTRACE("PRIMARY_TS_MODE    NOT_USED  " ); }
      if (primary_ts_mode   == Si2164B_DD_TS_PINS_CMD_PRIMARY_TS_MODE_NO_CHANGE  ) { SiTRACE("PRIMARY_TS_MODE    NO_CHANGE " ); }
    if ((primary_ts_activity   > Si2164B_DD_TS_PINS_CMD_PRIMARY_TS_ACTIVITY_MAX  ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("PRIMARY_TS_ACTIVITY  %d ", primary_ts_activity   );
    if ((secondary_ts_mode     > Si2164B_DD_TS_PINS_CMD_SECONDARY_TS_MODE_MAX    ) ) {error_code++; SiTRACE("\nOut of range: ");};
      if (secondary_ts_mode == Si2164B_DD_TS_PINS_CMD_SECONDARY_TS_MODE_DRIVE_TS ) { SiTRACE("SECONDARY_TS_MODE  DRIVE_TS  " ); }
      if (secondary_ts_mode == Si2164B_DD_TS_PINS_CMD_SECONDARY_TS_MODE_NOT_USED ) { SiTRACE("SECONDARY_TS_MODE  NOT_USED  " ); }
      if (secondary_ts_mode == Si2164B_DD_TS_PINS_CMD_SECONDARY_TS_MODE_NO_CHANGE) { SiTRACE("SECONDARY_TS_MODE  NO_CHANGE " ); }
    if ((secondary_ts_activity > Si2164B_DD_TS_PINS_CMD_SECONDARY_TS_ACTIVITY_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("SECONDARY_TS_ACTIVITY %d ", secondary_ts_activity );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DD_TS_PINS_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( primary_ts_mode       & Si2164B_DD_TS_PINS_CMD_PRIMARY_TS_MODE_MASK       ) << Si2164B_DD_TS_PINS_CMD_PRIMARY_TS_MODE_LSB      |
                                         ( primary_ts_activity   & Si2164B_DD_TS_PINS_CMD_PRIMARY_TS_ACTIVITY_MASK   ) << Si2164B_DD_TS_PINS_CMD_PRIMARY_TS_ACTIVITY_LSB  );
    cmdByteBuffer[2] = (unsigned char) ( ( secondary_ts_mode     & Si2164B_DD_TS_PINS_CMD_SECONDARY_TS_MODE_MASK     ) << Si2164B_DD_TS_PINS_CMD_SECONDARY_TS_MODE_LSB    |
                                         ( secondary_ts_activity & Si2164B_DD_TS_PINS_CMD_SECONDARY_TS_ACTIVITY_MASK ) << Si2164B_DD_TS_PINS_CMD_SECONDARY_TS_ACTIVITY_LSB);

    if (L0_WriteCommandBytes(api->i2c, 3, cmdByteBuffer) != 3) {
      SiTRACE("Error writing DD_TS_PINS bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 3, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DD_TS_PINS response\n");
      return error_code;
    }

    api->rsp->dd_ts_pins.primary_ts_mode       = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_TS_PINS_RESPONSE_PRIMARY_TS_MODE_LSB        , Si2164B_DD_TS_PINS_RESPONSE_PRIMARY_TS_MODE_MASK       );
    api->rsp->dd_ts_pins.primary_ts_activity   = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_TS_PINS_RESPONSE_PRIMARY_TS_ACTIVITY_LSB    , Si2164B_DD_TS_PINS_RESPONSE_PRIMARY_TS_ACTIVITY_MASK   );
    api->rsp->dd_ts_pins.secondary_ts_mode     = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_TS_PINS_RESPONSE_SECONDARY_TS_MODE_LSB      , Si2164B_DD_TS_PINS_RESPONSE_SECONDARY_TS_MODE_MASK     );
    api->rsp->dd_ts_pins.secondary_ts_activity = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_TS_PINS_RESPONSE_SECONDARY_TS_ACTIVITY_LSB  , Si2164B_DD_TS_PINS_RESPONSE_SECONDARY_TS_ACTIVITY_MASK );
#ifdef    Si2164B_GET_COMMAND_STRINGS
    msg = malloc(sizeof(unsigned char)*1000);
	if(msg != 0)
	{
	    Si2164B_L1_GetCommandResponseString (api, Si2164B_DD_TS_PINS_CMD_CODE, (const unsigned char*)" ", (unsigned char*)msg); 
		SiTRACE("response for %s\n", msg);
		free(msg);
	}
	else
	{
	    SiTRACE("Si2164B_L1_DD_TS_PINS : fail to create buffer to print log\n");
    }
#endif /* Si2164B_GET_COMMAND_STRINGS */

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DD_TS_PINS_CMD */
#ifdef    Si2164B_DD_UNCOR_CMD
/*---------------------------------------------------*/
/* Si2164B_DD_UNCOR COMMAND                                           */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DD_UNCOR                  (L1_Si2164B_Context *api,
                                                   unsigned char   rst)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[3];
    api->rsp->dd_uncor.STATUS = api->status;

    SiTRACE("Si2164B DD_UNCOR ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((rst > Si2164B_DD_UNCOR_CMD_RST_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("RST %d ", rst );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DD_UNCOR_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( rst & Si2164B_DD_UNCOR_CMD_RST_MASK ) << Si2164B_DD_UNCOR_CMD_RST_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing DD_UNCOR bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 3, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DD_UNCOR response\n");
      return error_code;
    }

    api->rsp->dd_uncor.uncor_lsb = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DD_UNCOR_RESPONSE_UNCOR_LSB_LSB  , Si2164B_DD_UNCOR_RESPONSE_UNCOR_LSB_MASK );
    api->rsp->dd_uncor.uncor_msb = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DD_UNCOR_RESPONSE_UNCOR_MSB_LSB  , Si2164B_DD_UNCOR_RESPONSE_UNCOR_MSB_MASK );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DD_UNCOR_CMD */
#ifdef    Si2164B_DEMOD_INFO_CMD
/*---------------------------------------------------*/
/* Si2164B_DEMOD_INFO COMMAND                                       */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DEMOD_INFO                (L1_Si2164B_Context *api)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[1];
    unsigned char rspByteBuffer[6];
    api->rsp->demod_info.STATUS = api->status;

    SiTRACE("Si2164B DEMOD_INFO ");
    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DEMOD_INFO_CMD;

    if (L0_WriteCommandBytes(api->i2c, 1, cmdByteBuffer) != 1) {
      SiTRACE("Error writing DEMOD_INFO bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 6, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DEMOD_INFO response\n");
      return error_code;
    }

    api->rsp->demod_info.reserved = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DEMOD_INFO_RESPONSE_RESERVED_LSB  , Si2164B_DEMOD_INFO_RESPONSE_RESERVED_MASK );
    api->rsp->demod_info.div_a    = Si2164B_convert_to_uint  (&rspByteBuffer[ 2] , Si2164B_DEMOD_INFO_RESPONSE_DIV_A_LSB     , Si2164B_DEMOD_INFO_RESPONSE_DIV_A_MASK );
    api->rsp->demod_info.div_b    = Si2164B_convert_to_uint  (&rspByteBuffer[ 4] , Si2164B_DEMOD_INFO_RESPONSE_DIV_B_LSB     , Si2164B_DEMOD_INFO_RESPONSE_DIV_B_MASK );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DEMOD_INFO_CMD */
#ifdef    Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD
/*---------------------------------------------------*/
/* Si2164B_DOWNLOAD_DATASET_CONTINUE COMMAND         */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DOWNLOAD_DATASET_CONTINUE (L1_Si2164B_Context *api,
                                                   unsigned char   data0,
                                                   unsigned char   data1,
                                                   unsigned char   data2,
                                                   unsigned char   data3,
                                                   unsigned char   data4,
                                                   unsigned char   data5,
                                                   unsigned char   data6)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[8];
    unsigned char rspByteBuffer[1];
    api->rsp->download_dataset_continue.STATUS = api->status;

    SiTRACE("Si2164B DOWNLOAD_DATASET_CONTINUE ");
  #ifdef   DEBUG_RANGE_CHECK
    SiTRACE("DATA0 %d ", data0 );
    SiTRACE("DATA1 %d ", data1 );
    SiTRACE("DATA2 %d ", data2 );
    SiTRACE("DATA3 %d ", data3 );
    SiTRACE("DATA4 %d ", data4 );
    SiTRACE("DATA5 %d ", data5 );
    SiTRACE("DATA6 %d ", data6 );
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( data0 & Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD_DATA0_MASK ) << Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD_DATA0_LSB);
    cmdByteBuffer[2] = (unsigned char) ( ( data1 & Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD_DATA1_MASK ) << Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD_DATA1_LSB);
    cmdByteBuffer[3] = (unsigned char) ( ( data2 & Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD_DATA2_MASK ) << Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD_DATA2_LSB);
    cmdByteBuffer[4] = (unsigned char) ( ( data3 & Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD_DATA3_MASK ) << Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD_DATA3_LSB);
    cmdByteBuffer[5] = (unsigned char) ( ( data4 & Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD_DATA4_MASK ) << Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD_DATA4_LSB);
    cmdByteBuffer[6] = (unsigned char) ( ( data5 & Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD_DATA5_MASK ) << Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD_DATA5_LSB);
    cmdByteBuffer[7] = (unsigned char) ( ( data6 & Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD_DATA6_MASK ) << Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD_DATA6_LSB);

    if (L0_WriteCommandBytes(api->i2c, 8, cmdByteBuffer) != 8) {
      SiTRACE("Error writing DOWNLOAD_DATASET_CONTINUE bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 1, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DOWNLOAD_DATASET_CONTINUE response\n");
      return error_code;
    }


    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD */
#ifdef    Si2164B_DOWNLOAD_DATASET_START_CMD
/*---------------------------------------------------*/
/* Si2164B_DOWNLOAD_DATASET_START COMMAND               */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DOWNLOAD_DATASET_START    (L1_Si2164B_Context *api,
                                                   unsigned char   dataset_id,
                                                   unsigned char   dataset_checksum,
                                                   unsigned char   data0,
                                                   unsigned char   data1,
                                                   unsigned char   data2,
                                                   unsigned char   data3,
                                                   unsigned char   data4)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[8];
    unsigned char rspByteBuffer[1];
    api->rsp->download_dataset_start.STATUS = api->status;

    SiTRACE("Si2164B DOWNLOAD_DATASET_START ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((dataset_id       > Si2164B_DOWNLOAD_DATASET_START_CMD_DATASET_ID_MAX      ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("DATASET_ID %d "      , dataset_id       );
    SiTRACE("DATASET_CHECKSUM %d ", dataset_checksum );
    SiTRACE("DATA0 %d "           , data0            );
    SiTRACE("DATA1 %d "           , data1            );
    SiTRACE("DATA2 %d "           , data2            );
    SiTRACE("DATA3 %d "           , data3            );
    SiTRACE("DATA4 %d "           , data4            );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DOWNLOAD_DATASET_START_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( dataset_id       & Si2164B_DOWNLOAD_DATASET_START_CMD_DATASET_ID_MASK       ) << Si2164B_DOWNLOAD_DATASET_START_CMD_DATASET_ID_LSB      );
    cmdByteBuffer[2] = (unsigned char) ( ( dataset_checksum & Si2164B_DOWNLOAD_DATASET_START_CMD_DATASET_CHECKSUM_MASK ) << Si2164B_DOWNLOAD_DATASET_START_CMD_DATASET_CHECKSUM_LSB);
    cmdByteBuffer[3] = (unsigned char) ( ( data0            & Si2164B_DOWNLOAD_DATASET_START_CMD_DATA0_MASK            ) << Si2164B_DOWNLOAD_DATASET_START_CMD_DATA0_LSB           );
    cmdByteBuffer[4] = (unsigned char) ( ( data1            & Si2164B_DOWNLOAD_DATASET_START_CMD_DATA1_MASK            ) << Si2164B_DOWNLOAD_DATASET_START_CMD_DATA1_LSB           );
    cmdByteBuffer[5] = (unsigned char) ( ( data2            & Si2164B_DOWNLOAD_DATASET_START_CMD_DATA2_MASK            ) << Si2164B_DOWNLOAD_DATASET_START_CMD_DATA2_LSB           );
    cmdByteBuffer[6] = (unsigned char) ( ( data3            & Si2164B_DOWNLOAD_DATASET_START_CMD_DATA3_MASK            ) << Si2164B_DOWNLOAD_DATASET_START_CMD_DATA3_LSB           );
    cmdByteBuffer[7] = (unsigned char) ( ( data4            & Si2164B_DOWNLOAD_DATASET_START_CMD_DATA4_MASK            ) << Si2164B_DOWNLOAD_DATASET_START_CMD_DATA4_LSB           );

    if (L0_WriteCommandBytes(api->i2c, 8, cmdByteBuffer) != 8) {
      SiTRACE("Error writing DOWNLOAD_DATASET_START bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 1, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DOWNLOAD_DATASET_START response\n");
      return error_code;
    }


    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DOWNLOAD_DATASET_START_CMD */
#ifdef    Si2164B_DVBC2_CTRL_CMD
/*---------------------------------------------------*/
/* Si2164B_DVBC2_CTRL COMMAND                                       */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DVBC2_CTRL                (L1_Si2164B_Context *api,
                                                   unsigned char   action,
                                                   unsigned long   tuned_rf_freq)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[8];
    unsigned char rspByteBuffer[1];
    api->rsp->dvbc2_ctrl.STATUS = api->status;

    SiTRACE("Si2164B DVBC2_CTRL ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((action        > Si2164B_DVBC2_CTRL_CMD_ACTION_MAX       )  || (action        < Si2164B_DVBC2_CTRL_CMD_ACTION_MIN       ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("ACTION %d "       , action        );
    SiTRACE("TUNED_RF_FREQ %ld ", tuned_rf_freq );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DVBC2_CTRL_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( action        & Si2164B_DVBC2_CTRL_CMD_ACTION_MASK        ) << Si2164B_DVBC2_CTRL_CMD_ACTION_LSB       );
    cmdByteBuffer[2] = (unsigned char)0x00;
    cmdByteBuffer[3] = (unsigned char)0x00;
    cmdByteBuffer[4] = (unsigned char) ( ( tuned_rf_freq & Si2164B_DVBC2_CTRL_CMD_TUNED_RF_FREQ_MASK ) << Si2164B_DVBC2_CTRL_CMD_TUNED_RF_FREQ_LSB);
    cmdByteBuffer[5] = (unsigned char) ((( tuned_rf_freq & Si2164B_DVBC2_CTRL_CMD_TUNED_RF_FREQ_MASK ) << Si2164B_DVBC2_CTRL_CMD_TUNED_RF_FREQ_LSB)>>8);
    cmdByteBuffer[6] = (unsigned char) ((( tuned_rf_freq & Si2164B_DVBC2_CTRL_CMD_TUNED_RF_FREQ_MASK ) << Si2164B_DVBC2_CTRL_CMD_TUNED_RF_FREQ_LSB)>>16);
    cmdByteBuffer[7] = (unsigned char) ((( tuned_rf_freq & Si2164B_DVBC2_CTRL_CMD_TUNED_RF_FREQ_MASK ) << Si2164B_DVBC2_CTRL_CMD_TUNED_RF_FREQ_LSB)>>24);

    if (L0_WriteCommandBytes(api->i2c, 8, cmdByteBuffer) != 8) {
      SiTRACE("Error writing DVBC2_CTRL bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 1, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DVBC2_CTRL response\n");
      return error_code;
    }


    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DVBC2_CTRL_CMD */
#ifdef    Si2164B_DVBC2_DS_INFO_CMD
/*---------------------------------------------------*/
/* Si2164B_DVBC2_DS_INFO COMMAND                                 */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DVBC2_DS_INFO             (L1_Si2164B_Context *api,
                                                   unsigned char   ds_index_or_id,
                                                   unsigned char   ds_select_index_or_id)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[3];
    unsigned char rspByteBuffer[8];
    api->rsp->dvbc2_ds_info.STATUS = api->status;

    SiTRACE("Si2164B DVBC2_DS_INFO ");
  #ifdef   DEBUG_RANGE_CHECK
    SiTRACE("DS_INDEX_OR_ID %d "       , ds_index_or_id        );
    if ((ds_select_index_or_id > Si2164B_DVBC2_DS_INFO_CMD_DS_SELECT_INDEX_OR_ID_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("DS_SELECT_INDEX_OR_ID %d ", ds_select_index_or_id );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DVBC2_DS_INFO_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( ds_index_or_id        & Si2164B_DVBC2_DS_INFO_CMD_DS_INDEX_OR_ID_MASK        ) << Si2164B_DVBC2_DS_INFO_CMD_DS_INDEX_OR_ID_LSB       );
    cmdByteBuffer[2] = (unsigned char) ( ( ds_select_index_or_id & Si2164B_DVBC2_DS_INFO_CMD_DS_SELECT_INDEX_OR_ID_MASK ) << Si2164B_DVBC2_DS_INFO_CMD_DS_SELECT_INDEX_OR_ID_LSB);

    if (L0_WriteCommandBytes(api->i2c, 3, cmdByteBuffer) != 3) {
      SiTRACE("Error writing DVBC2_DS_INFO bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 8, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DVBC2_DS_INFO response\n");
      return error_code;
    }

    api->rsp->dvbc2_ds_info.ds_id              = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBC2_DS_INFO_RESPONSE_DS_ID_LSB               , Si2164B_DVBC2_DS_INFO_RESPONSE_DS_ID_MASK              );
    api->rsp->dvbc2_ds_info.dslice_num_plp     = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBC2_DS_INFO_RESPONSE_DSLICE_NUM_PLP_LSB      , Si2164B_DVBC2_DS_INFO_RESPONSE_DSLICE_NUM_PLP_MASK     );
    api->rsp->dvbc2_ds_info.reserved_2         = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_DVBC2_DS_INFO_RESPONSE_RESERVED_2_LSB          , Si2164B_DVBC2_DS_INFO_RESPONSE_RESERVED_2_MASK         );
    api->rsp->dvbc2_ds_info.dslice_tune_pos_hz = Si2164B_convert_to_ulong (&rspByteBuffer[ 4] , Si2164B_DVBC2_DS_INFO_RESPONSE_DSLICE_TUNE_POS_HZ_LSB  , Si2164B_DVBC2_DS_INFO_RESPONSE_DSLICE_TUNE_POS_HZ_MASK );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DVBC2_DS_INFO_CMD */
#ifdef    Si2164B_DVBC2_DS_PLP_SELECT_CMD
/*---------------------------------------------------*/
/* Si2164B_DVBC2_DS_PLP_SELECT COMMAND                     */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DVBC2_DS_PLP_SELECT       (L1_Si2164B_Context *api,
                                                   unsigned char   plp_id,
                                                   unsigned char   id_sel_mode,
                                                   unsigned char   ds_id)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[4];
    unsigned char rspByteBuffer[1];
    api->rsp->dvbc2_ds_plp_select.STATUS = api->status;

    SiTRACE("Si2164B DVBC2_DS_PLP_SELECT ");
  #ifdef   DEBUG_RANGE_CHECK
    SiTRACE("PLP_ID %d "     , plp_id      );
    if ((id_sel_mode > Si2164B_DVBC2_DS_PLP_SELECT_CMD_ID_SEL_MODE_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("ID_SEL_MODE %d ", id_sel_mode );
    SiTRACE("DS_ID %d "      , ds_id       );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DVBC2_DS_PLP_SELECT_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( plp_id      & Si2164B_DVBC2_DS_PLP_SELECT_CMD_PLP_ID_MASK      ) << Si2164B_DVBC2_DS_PLP_SELECT_CMD_PLP_ID_LSB     );
    cmdByteBuffer[2] = (unsigned char) ( ( id_sel_mode & Si2164B_DVBC2_DS_PLP_SELECT_CMD_ID_SEL_MODE_MASK ) << Si2164B_DVBC2_DS_PLP_SELECT_CMD_ID_SEL_MODE_LSB);
    cmdByteBuffer[3] = (unsigned char) ( ( ds_id       & Si2164B_DVBC2_DS_PLP_SELECT_CMD_DS_ID_MASK       ) << Si2164B_DVBC2_DS_PLP_SELECT_CMD_DS_ID_LSB      );

    if (L0_WriteCommandBytes(api->i2c, 4, cmdByteBuffer) != 4) {
      SiTRACE("Error writing DVBC2_DS_PLP_SELECT bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 1, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DVBC2_DS_PLP_SELECT response\n");
      return error_code;
    }


    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DVBC2_DS_PLP_SELECT_CMD */
#ifdef    Si2164B_DVBC2_PLP_INFO_CMD
/*---------------------------------------------------*/
/* Si2164B_DVBC2_PLP_INFO COMMAND                               */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DVBC2_PLP_INFO            (L1_Si2164B_Context *api,
                                                   unsigned char   plp_index,
                                                   unsigned char   plp_info_ds_mode,
                                                   unsigned char   ds_index)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[4];
    unsigned char rspByteBuffer[3];
    api->rsp->dvbc2_plp_info.STATUS = api->status;

    SiTRACE("Si2164B DVBC2_PLP_INFO ");
  #ifdef   DEBUG_RANGE_CHECK
    SiTRACE("PLP_INDEX %d "       , plp_index        );
    if ((plp_info_ds_mode > Si2164B_DVBC2_PLP_INFO_CMD_PLP_INFO_DS_MODE_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("PLP_INFO_DS_MODE %d ", plp_info_ds_mode );
    SiTRACE("DS_INDEX %d "        , ds_index         );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DVBC2_PLP_INFO_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( plp_index        & Si2164B_DVBC2_PLP_INFO_CMD_PLP_INDEX_MASK        ) << Si2164B_DVBC2_PLP_INFO_CMD_PLP_INDEX_LSB       );
    cmdByteBuffer[2] = (unsigned char) ( ( plp_info_ds_mode & Si2164B_DVBC2_PLP_INFO_CMD_PLP_INFO_DS_MODE_MASK ) << Si2164B_DVBC2_PLP_INFO_CMD_PLP_INFO_DS_MODE_LSB);
    cmdByteBuffer[3] = (unsigned char) ( ( ds_index         & Si2164B_DVBC2_PLP_INFO_CMD_DS_INDEX_MASK         ) << Si2164B_DVBC2_PLP_INFO_CMD_DS_INDEX_LSB        );

    if (L0_WriteCommandBytes(api->i2c, 4, cmdByteBuffer) != 4) {
      SiTRACE("Error writing DVBC2_PLP_INFO bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 3, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DVBC2_PLP_INFO response\n");
      return error_code;
    }

    api->rsp->dvbc2_plp_info.plp_id           = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBC2_PLP_INFO_RESPONSE_PLP_ID_LSB            , Si2164B_DVBC2_PLP_INFO_RESPONSE_PLP_ID_MASK           );
    api->rsp->dvbc2_plp_info.plp_payload_type = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBC2_PLP_INFO_RESPONSE_PLP_PAYLOAD_TYPE_LSB  , Si2164B_DVBC2_PLP_INFO_RESPONSE_PLP_PAYLOAD_TYPE_MASK );
    api->rsp->dvbc2_plp_info.plp_type         = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBC2_PLP_INFO_RESPONSE_PLP_TYPE_LSB          , Si2164B_DVBC2_PLP_INFO_RESPONSE_PLP_TYPE_MASK         );
    api->rsp->dvbc2_plp_info.plp_bundled      = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBC2_PLP_INFO_RESPONSE_PLP_BUNDLED_LSB       , Si2164B_DVBC2_PLP_INFO_RESPONSE_PLP_BUNDLED_MASK      );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DVBC2_PLP_INFO_CMD */
#ifdef    Si2164B_DVBC2_STATUS_CMD
/*---------------------------------------------------*/
/* Si2164B_DVBC2_STATUS COMMAND                                   */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DVBC2_STATUS              (L1_Si2164B_Context *api,
                                                   unsigned char   intack)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[16];
    api->rsp->dvbc2_status.STATUS = api->status;

    SiTRACE("Si2164B DVBC2_STATUS ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((intack > Si2164B_DVBC2_STATUS_CMD_INTACK_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("INTACK %d ", intack );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DVBC2_STATUS_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( intack & Si2164B_DVBC2_STATUS_CMD_INTACK_MASK ) << Si2164B_DVBC2_STATUS_CMD_INTACK_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing DVBC2_STATUS bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 16, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DVBC2_STATUS response\n");
      return error_code;
    }

    api->rsp->dvbc2_status.pclint        = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBC2_STATUS_RESPONSE_PCLINT_LSB         , Si2164B_DVBC2_STATUS_RESPONSE_PCLINT_MASK        );
    api->rsp->dvbc2_status.dlint         = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBC2_STATUS_RESPONSE_DLINT_LSB          , Si2164B_DVBC2_STATUS_RESPONSE_DLINT_MASK         );
    api->rsp->dvbc2_status.berint        = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBC2_STATUS_RESPONSE_BERINT_LSB         , Si2164B_DVBC2_STATUS_RESPONSE_BERINT_MASK        );
    api->rsp->dvbc2_status.uncorint      = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBC2_STATUS_RESPONSE_UNCORINT_LSB       , Si2164B_DVBC2_STATUS_RESPONSE_UNCORINT_MASK      );
    api->rsp->dvbc2_status.notdvbc2int   = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBC2_STATUS_RESPONSE_NOTDVBC2INT_LSB    , Si2164B_DVBC2_STATUS_RESPONSE_NOTDVBC2INT_MASK   );
    api->rsp->dvbc2_status.reqint        = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBC2_STATUS_RESPONSE_REQINT_LSB         , Si2164B_DVBC2_STATUS_RESPONSE_REQINT_MASK        );
    api->rsp->dvbc2_status.ewbsint       = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBC2_STATUS_RESPONSE_EWBSINT_LSB        , Si2164B_DVBC2_STATUS_RESPONSE_EWBSINT_MASK       );
    api->rsp->dvbc2_status.pcl           = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBC2_STATUS_RESPONSE_PCL_LSB            , Si2164B_DVBC2_STATUS_RESPONSE_PCL_MASK           );
    api->rsp->dvbc2_status.dl            = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBC2_STATUS_RESPONSE_DL_LSB             , Si2164B_DVBC2_STATUS_RESPONSE_DL_MASK            );
    api->rsp->dvbc2_status.ber           = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBC2_STATUS_RESPONSE_BER_LSB            , Si2164B_DVBC2_STATUS_RESPONSE_BER_MASK           );
    api->rsp->dvbc2_status.uncor         = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBC2_STATUS_RESPONSE_UNCOR_LSB          , Si2164B_DVBC2_STATUS_RESPONSE_UNCOR_MASK         );
    api->rsp->dvbc2_status.notdvbc2      = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBC2_STATUS_RESPONSE_NOTDVBC2_LSB       , Si2164B_DVBC2_STATUS_RESPONSE_NOTDVBC2_MASK      );
    api->rsp->dvbc2_status.req           = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBC2_STATUS_RESPONSE_REQ_LSB            , Si2164B_DVBC2_STATUS_RESPONSE_REQ_MASK           );
    api->rsp->dvbc2_status.ewbs          = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBC2_STATUS_RESPONSE_EWBS_LSB           , Si2164B_DVBC2_STATUS_RESPONSE_EWBS_MASK          );
    api->rsp->dvbc2_status.cnr           = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_DVBC2_STATUS_RESPONSE_CNR_LSB            , Si2164B_DVBC2_STATUS_RESPONSE_CNR_MASK           );
    api->rsp->dvbc2_status.afc_freq      = Si2164B_convert_to_int   (&rspByteBuffer[ 4] , Si2164B_DVBC2_STATUS_RESPONSE_AFC_FREQ_LSB       , Si2164B_DVBC2_STATUS_RESPONSE_AFC_FREQ_MASK      );
    api->rsp->dvbc2_status.timing_offset = Si2164B_convert_to_int   (&rspByteBuffer[ 6] , Si2164B_DVBC2_STATUS_RESPONSE_TIMING_OFFSET_LSB  , Si2164B_DVBC2_STATUS_RESPONSE_TIMING_OFFSET_MASK );
    api->rsp->dvbc2_status.dvbc2_status  = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_DVBC2_STATUS_RESPONSE_DVBC2_STATUS_LSB   , Si2164B_DVBC2_STATUS_RESPONSE_DVBC2_STATUS_MASK  );
    api->rsp->dvbc2_status.constellation = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_DVBC2_STATUS_RESPONSE_CONSTELLATION_LSB  , Si2164B_DVBC2_STATUS_RESPONSE_CONSTELLATION_MASK );
    api->rsp->dvbc2_status.sp_inv        = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_DVBC2_STATUS_RESPONSE_SP_INV_LSB         , Si2164B_DVBC2_STATUS_RESPONSE_SP_INV_MASK        );
    api->rsp->dvbc2_status.code_rate     = Si2164B_convert_to_byte  (&rspByteBuffer[ 9] , Si2164B_DVBC2_STATUS_RESPONSE_CODE_RATE_LSB      , Si2164B_DVBC2_STATUS_RESPONSE_CODE_RATE_MASK     );
    api->rsp->dvbc2_status.guard_int     = Si2164B_convert_to_byte  (&rspByteBuffer[ 9] , Si2164B_DVBC2_STATUS_RESPONSE_GUARD_INT_LSB      , Si2164B_DVBC2_STATUS_RESPONSE_GUARD_INT_MASK     );
    api->rsp->dvbc2_status.ds_id         = Si2164B_convert_to_byte  (&rspByteBuffer[10] , Si2164B_DVBC2_STATUS_RESPONSE_DS_ID_LSB          , Si2164B_DVBC2_STATUS_RESPONSE_DS_ID_MASK         );
    api->rsp->dvbc2_status.plp_id        = Si2164B_convert_to_byte  (&rspByteBuffer[11] , Si2164B_DVBC2_STATUS_RESPONSE_PLP_ID_LSB         , Si2164B_DVBC2_STATUS_RESPONSE_PLP_ID_MASK        );
    api->rsp->dvbc2_status.rf_freq       = Si2164B_convert_to_ulong (&rspByteBuffer[12] , Si2164B_DVBC2_STATUS_RESPONSE_RF_FREQ_LSB        , Si2164B_DVBC2_STATUS_RESPONSE_RF_FREQ_MASK       );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DVBC2_STATUS_CMD */
#ifdef    Si2164B_DVBC2_SYS_INFO_CMD
/*---------------------------------------------------*/
/* Si2164B_DVBC2_SYS_INFO COMMAND                               */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DVBC2_SYS_INFO            (L1_Si2164B_Context *api)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[1];
    unsigned char rspByteBuffer[16];
    api->rsp->dvbc2_sys_info.STATUS = api->status;

    SiTRACE("Si2164B DVBC2_SYS_INFO ");
    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DVBC2_SYS_INFO_CMD;

    if (L0_WriteCommandBytes(api->i2c, 1, cmdByteBuffer) != 1) {
      SiTRACE("Error writing DVBC2_SYS_INFO bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 16, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DVBC2_SYS_INFO response\n");
      return error_code;
    }

    api->rsp->dvbc2_sys_info.num_dslice         = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBC2_SYS_INFO_RESPONSE_NUM_DSLICE_LSB          , Si2164B_DVBC2_SYS_INFO_RESPONSE_NUM_DSLICE_MASK         );
    api->rsp->dvbc2_sys_info.network_id         = Si2164B_convert_to_uint  (&rspByteBuffer[ 2] , Si2164B_DVBC2_SYS_INFO_RESPONSE_NETWORK_ID_LSB          , Si2164B_DVBC2_SYS_INFO_RESPONSE_NETWORK_ID_MASK         );
    api->rsp->dvbc2_sys_info.c2_bandwidth_hz    = Si2164B_convert_to_ulong (&rspByteBuffer[ 4] , Si2164B_DVBC2_SYS_INFO_RESPONSE_C2_BANDWIDTH_HZ_LSB     , Si2164B_DVBC2_SYS_INFO_RESPONSE_C2_BANDWIDTH_HZ_MASK    );
    api->rsp->dvbc2_sys_info.start_frequency_hz = Si2164B_convert_to_ulong (&rspByteBuffer[ 8] , Si2164B_DVBC2_SYS_INFO_RESPONSE_START_FREQUENCY_HZ_LSB  , Si2164B_DVBC2_SYS_INFO_RESPONSE_START_FREQUENCY_HZ_MASK );
    api->rsp->dvbc2_sys_info.c2_system_id       = Si2164B_convert_to_uint  (&rspByteBuffer[12] , Si2164B_DVBC2_SYS_INFO_RESPONSE_C2_SYSTEM_ID_LSB        , Si2164B_DVBC2_SYS_INFO_RESPONSE_C2_SYSTEM_ID_MASK       );
    api->rsp->dvbc2_sys_info.reserved_4         = Si2164B_convert_to_uint  (&rspByteBuffer[14] , Si2164B_DVBC2_SYS_INFO_RESPONSE_RESERVED_4_LSB          , Si2164B_DVBC2_SYS_INFO_RESPONSE_RESERVED_4_MASK         );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DVBC2_SYS_INFO_CMD */

#ifdef    Si2164B_DVBC_STATUS_CMD
/*---------------------------------------------------*/
/* Si2164B_DVBC_STATUS COMMAND                                     */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DVBC_STATUS               (L1_Si2164B_Context *api,
                                                   unsigned char   intack)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[9];
    api->rsp->dvbc_status.STATUS = api->status;

    SiTRACE("Si2164B DVBC_STATUS ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((intack > Si2164B_DVBC_STATUS_CMD_INTACK_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("INTACK %d ", intack );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DVBC_STATUS_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( intack & Si2164B_DVBC_STATUS_CMD_INTACK_MASK ) << Si2164B_DVBC_STATUS_CMD_INTACK_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing DVBC_STATUS bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 9, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DVBC_STATUS response\n");
      return error_code;
    }

    api->rsp->dvbc_status.pclint        = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBC_STATUS_RESPONSE_PCLINT_LSB         , Si2164B_DVBC_STATUS_RESPONSE_PCLINT_MASK        );
    api->rsp->dvbc_status.dlint         = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBC_STATUS_RESPONSE_DLINT_LSB          , Si2164B_DVBC_STATUS_RESPONSE_DLINT_MASK         );
    api->rsp->dvbc_status.berint        = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBC_STATUS_RESPONSE_BERINT_LSB         , Si2164B_DVBC_STATUS_RESPONSE_BERINT_MASK        );
    api->rsp->dvbc_status.uncorint      = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBC_STATUS_RESPONSE_UNCORINT_LSB       , Si2164B_DVBC_STATUS_RESPONSE_UNCORINT_MASK      );
    api->rsp->dvbc_status.notdvbcint    = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBC_STATUS_RESPONSE_NOTDVBCINT_LSB     , Si2164B_DVBC_STATUS_RESPONSE_NOTDVBCINT_MASK    );
    api->rsp->dvbc_status.pcl           = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBC_STATUS_RESPONSE_PCL_LSB            , Si2164B_DVBC_STATUS_RESPONSE_PCL_MASK           );
    api->rsp->dvbc_status.dl            = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBC_STATUS_RESPONSE_DL_LSB             , Si2164B_DVBC_STATUS_RESPONSE_DL_MASK            );
    api->rsp->dvbc_status.ber           = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBC_STATUS_RESPONSE_BER_LSB            , Si2164B_DVBC_STATUS_RESPONSE_BER_MASK           );
    api->rsp->dvbc_status.uncor         = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBC_STATUS_RESPONSE_UNCOR_LSB          , Si2164B_DVBC_STATUS_RESPONSE_UNCOR_MASK         );
    api->rsp->dvbc_status.notdvbc       = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBC_STATUS_RESPONSE_NOTDVBC_LSB        , Si2164B_DVBC_STATUS_RESPONSE_NOTDVBC_MASK       );
    api->rsp->dvbc_status.cnr           = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_DVBC_STATUS_RESPONSE_CNR_LSB            , Si2164B_DVBC_STATUS_RESPONSE_CNR_MASK           );
    api->rsp->dvbc_status.afc_freq      = Si2164B_convert_to_int   (&rspByteBuffer[ 4] , Si2164B_DVBC_STATUS_RESPONSE_AFC_FREQ_LSB       , Si2164B_DVBC_STATUS_RESPONSE_AFC_FREQ_MASK      );
    api->rsp->dvbc_status.timing_offset = Si2164B_convert_to_int   (&rspByteBuffer[ 6] , Si2164B_DVBC_STATUS_RESPONSE_TIMING_OFFSET_LSB  , Si2164B_DVBC_STATUS_RESPONSE_TIMING_OFFSET_MASK );
    api->rsp->dvbc_status.constellation = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_DVBC_STATUS_RESPONSE_CONSTELLATION_LSB  , Si2164B_DVBC_STATUS_RESPONSE_CONSTELLATION_MASK );
    api->rsp->dvbc_status.sp_inv        = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_DVBC_STATUS_RESPONSE_SP_INV_LSB         , Si2164B_DVBC_STATUS_RESPONSE_SP_INV_MASK        );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DVBC_STATUS_CMD */


signed int   Si2164B_L1_GET_REG           (L1_Si2164B_Context *api, unsigned char   reg_code_lsb, unsigned char   reg_code_mid, unsigned char   reg_code_msb) {
  signed int res;
  if (Si2164B_L1_DD_GET_REG (api, reg_code_lsb, reg_code_mid, reg_code_msb) != NO_Si2164B_ERROR) {
    SiERROR("Error calling Si2167B_L1_DD_GET_REG\n");
    SiTraceConfiguration("traces resume");
    return 0;
  }
  SiTraceConfiguration("traces resume");
  res =  (api->rsp->dd_get_reg.data4<<24)
        +(api->rsp->dd_get_reg.data3<<16)
        +(api->rsp->dd_get_reg.data2<<8 )
        +(api->rsp->dd_get_reg.data1<<0 );
  return res;
}

#ifdef    Si2164B_DVBS2_STATUS_CMD
/*---------------------------------------------------*/
/* Si2164B_DVBS2_STATUS COMMAND                                   */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DVBS2_STATUS              (L1_Si2164B_Context *api,
                                                   unsigned char   intack)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[13];
#ifdef    Si2167B_20_COMPATIBLE
  /* FREQ OFFSET workaround variables (only for Si2167B/Si2166B compatibility) */
    unsigned int adc_mode;
    unsigned int spectral_inv_status_s2;
    signed int freq_corr_sat;
    signed int if_freq_shift;
    signed int fe_clk_freq;
    signed int freq_offset_tmp1;
    signed int freq_offset_tmp2;
    signed int freq_offset;
  /* end FREQ OFFSET workaround variables */
#endif /* Si2167B_20_COMPATIBLE */
    api->rsp->dvbs2_status.STATUS = api->status;

    SiTRACE("Si2164B DVBS2_STATUS ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((intack > Si2164B_DVBS2_STATUS_CMD_INTACK_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("INTACK %d ", intack );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DVBS2_STATUS_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( intack & Si2164B_DVBS2_STATUS_CMD_INTACK_MASK ) << Si2164B_DVBS2_STATUS_CMD_INTACK_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing DVBS2_STATUS bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 13, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DVBS2_STATUS response\n");
      return error_code;
    }

    api->rsp->dvbs2_status.pclint        = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBS2_STATUS_RESPONSE_PCLINT_LSB         , Si2164B_DVBS2_STATUS_RESPONSE_PCLINT_MASK        );
    api->rsp->dvbs2_status.dlint         = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBS2_STATUS_RESPONSE_DLINT_LSB          , Si2164B_DVBS2_STATUS_RESPONSE_DLINT_MASK         );
    api->rsp->dvbs2_status.berint        = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBS2_STATUS_RESPONSE_BERINT_LSB         , Si2164B_DVBS2_STATUS_RESPONSE_BERINT_MASK        );
    api->rsp->dvbs2_status.uncorint      = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBS2_STATUS_RESPONSE_UNCORINT_LSB       , Si2164B_DVBS2_STATUS_RESPONSE_UNCORINT_MASK      );
    api->rsp->dvbs2_status.pcl           = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBS2_STATUS_RESPONSE_PCL_LSB            , Si2164B_DVBS2_STATUS_RESPONSE_PCL_MASK           );
    api->rsp->dvbs2_status.dl            = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBS2_STATUS_RESPONSE_DL_LSB             , Si2164B_DVBS2_STATUS_RESPONSE_DL_MASK            );
    api->rsp->dvbs2_status.ber           = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBS2_STATUS_RESPONSE_BER_LSB            , Si2164B_DVBS2_STATUS_RESPONSE_BER_MASK           );
    api->rsp->dvbs2_status.uncor         = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBS2_STATUS_RESPONSE_UNCOR_LSB          , Si2164B_DVBS2_STATUS_RESPONSE_UNCOR_MASK         );
    api->rsp->dvbs2_status.cnr           = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_DVBS2_STATUS_RESPONSE_CNR_LSB            , Si2164B_DVBS2_STATUS_RESPONSE_CNR_MASK           );
    api->rsp->dvbs2_status.afc_freq      = Si2164B_convert_to_int   (&rspByteBuffer[ 4] , Si2164B_DVBS2_STATUS_RESPONSE_AFC_FREQ_LSB       , Si2164B_DVBS2_STATUS_RESPONSE_AFC_FREQ_MASK      );
    api->rsp->dvbs2_status.timing_offset = Si2164B_convert_to_int   (&rspByteBuffer[ 6] , Si2164B_DVBS2_STATUS_RESPONSE_TIMING_OFFSET_LSB  , Si2164B_DVBS2_STATUS_RESPONSE_TIMING_OFFSET_MASK );
    api->rsp->dvbs2_status.constellation = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_LSB  , Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_MASK );
    api->rsp->dvbs2_status.sp_inv        = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_DVBS2_STATUS_RESPONSE_SP_INV_LSB         , Si2164B_DVBS2_STATUS_RESPONSE_SP_INV_MASK        );
    api->rsp->dvbs2_status.pilots        = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_DVBS2_STATUS_RESPONSE_PILOTS_LSB         , Si2164B_DVBS2_STATUS_RESPONSE_PILOTS_MASK        );
    api->rsp->dvbs2_status.code_rate     = Si2164B_convert_to_byte  (&rspByteBuffer[ 9] , Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_LSB      , Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_MASK     );
    api->rsp->dvbs2_status.roll_off      = Si2164B_convert_to_byte  (&rspByteBuffer[10] , Si2164B_DVBS2_STATUS_RESPONSE_ROLL_OFF_LSB       , Si2164B_DVBS2_STATUS_RESPONSE_ROLL_OFF_MASK      );
    api->rsp->dvbs2_status.ccm_vcm       = Si2164B_convert_to_byte  (&rspByteBuffer[10] , Si2164B_DVBS2_STATUS_RESPONSE_CCM_VCM_LSB        , Si2164B_DVBS2_STATUS_RESPONSE_CCM_VCM_MASK       );
    api->rsp->dvbs2_status.sis_mis       = Si2164B_convert_to_byte  (&rspByteBuffer[10] , Si2164B_DVBS2_STATUS_RESPONSE_SIS_MIS_LSB        , Si2164B_DVBS2_STATUS_RESPONSE_SIS_MIS_MASK       );
    api->rsp->dvbs2_status.num_is        = Si2164B_convert_to_byte  (&rspByteBuffer[11] , Si2164B_DVBS2_STATUS_RESPONSE_NUM_IS_LSB         , Si2164B_DVBS2_STATUS_RESPONSE_NUM_IS_MASK        );
    api->rsp->dvbs2_status.isi_id        = Si2164B_convert_to_byte  (&rspByteBuffer[12] , Si2164B_DVBS2_STATUS_RESPONSE_ISI_ID_LSB         , Si2164B_DVBS2_STATUS_RESPONSE_ISI_ID_MASK        );

#ifdef    Si2167B_20_COMPATIBLE
 /* FREQ OFFSET workaround only for Si2167B */
    if (  (api->rsp->part_info.part     == 67 ) & (api->rsp->part_info.romid    ==  0 ) ) {
    SiTRACE("API      dvbs2_status.afc_freq : %d \n", api->rsp->dvbs2_status.afc_freq);

    /* read adc_mode register */
    adc_mode               = Si2164B_L1_GET_REG (api,  1,  40,  1);
    SiTRACE("adc_mode               : %u \n ", adc_mode);
    /* read spectral_inv_status_s2 register */
    spectral_inv_status_s2 = Si2164B_L1_GET_REG (api, 33,  87,  5);
    SiTRACE("spectral_inv_status_s2 : %u \n", spectral_inv_status_s2);
    /* read freq_corr_sat register */
    freq_corr_sat          = Si2164B_L1_GET_REG (api, 27, 144,  5);
    SiTRACE("freq_corr_sat          : %d \n", freq_corr_sat);
    /* read if_freq_shift register */
    if_freq_shift          = Si2164B_L1_GET_REG (api, 28,  92,  4);
    SiTRACE("if_freq_shift          : %d \n", if_freq_shift);
    /* read fe_clk_freq register */
    fe_clk_freq            = Si2164B_L1_GET_REG (api, 31, 108, 10);
    SiTRACE("fe_clk_freq            : %d \n", fe_clk_freq);
    if (fe_clk_freq == 0) {SiERROR("Problem reading fe_clk_freq (returning 0)\n"); return ERROR_Si2164B_ERR;}

    freq_offset_tmp1 = ( 2*freq_corr_sat+if_freq_shift)/((1<<29)/(fe_clk_freq/1000));
    freq_offset_tmp2 = (-2*freq_corr_sat+if_freq_shift)/((1<<29)/(fe_clk_freq/1000));

    if (adc_mode==1) {
      if (spectral_inv_status_s2==1) {freq_offset = -freq_offset_tmp2;} else {freq_offset = -freq_offset_tmp1;}
    } else {
      if (spectral_inv_status_s2==1) {freq_offset =  freq_offset_tmp2;} else {freq_offset =  freq_offset_tmp1;}
    }
    SiTRACE(" freq_offset1 = %d \n",freq_offset_tmp1);
    SiTRACE(" freq_offset2 = %d \n",freq_offset_tmp2);
    SiTRACE(" freq_offset  = %d \n",freq_offset);

    api->rsp->dvbs2_status.afc_freq = freq_offset;
    SiTRACE("COMPUTED dvbs2_status.afc_freq : %d \n", api->rsp->dvbs2_status.afc_freq);
 /* end FREQ OFFSET workaround */
    }
#endif /* Si2167B_20_COMPATIBLE */

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DVBS2_STATUS_CMD */
#ifdef    Si2164B_DVBS2_STREAM_INFO_CMD
/*---------------------------------------------------*/
/* Si2164B_DVBS2_STREAM_INFO COMMAND                         */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DVBS2_STREAM_INFO         (L1_Si2164B_Context *api,
                                                   unsigned char   isi_index)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[4];
    api->rsp->dvbs2_stream_info.STATUS = api->status;

    SiTRACE("Si2164B DVBS2_STREAM_INFO ");
  #ifdef   DEBUG_RANGE_CHECK
    SiTRACE("ISI_INDEX %d ", isi_index );
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DVBS2_STREAM_INFO_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( isi_index & Si2164B_DVBS2_STREAM_INFO_CMD_ISI_INDEX_MASK ) << Si2164B_DVBS2_STREAM_INFO_CMD_ISI_INDEX_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing DVBS2_STREAM_INFO bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 4, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DVBS2_STREAM_INFO response\n");
      return error_code;
    }

    api->rsp->dvbs2_stream_info.isi_id            = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBS2_STREAM_INFO_RESPONSE_ISI_ID_LSB             , Si2164B_DVBS2_STREAM_INFO_RESPONSE_ISI_ID_MASK            );
    api->rsp->dvbs2_stream_info.isi_constellation = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBS2_STREAM_INFO_RESPONSE_ISI_CONSTELLATION_LSB  , Si2164B_DVBS2_STREAM_INFO_RESPONSE_ISI_CONSTELLATION_MASK );
    api->rsp->dvbs2_stream_info.isi_code_rate     = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_DVBS2_STREAM_INFO_RESPONSE_ISI_CODE_RATE_LSB      , Si2164B_DVBS2_STREAM_INFO_RESPONSE_ISI_CODE_RATE_MASK     );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DVBS2_STREAM_INFO_CMD */
#ifdef    Si2164B_DVBS2_STREAM_SELECT_CMD
/*---------------------------------------------------*/
/* Si2164B_DVBS2_STREAM_SELECT COMMAND                     */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DVBS2_STREAM_SELECT       (L1_Si2164B_Context *api,
                                                   unsigned char   stream_id,
                                                   unsigned char   stream_sel_mode)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[3];
    unsigned char rspByteBuffer[1];
    api->rsp->dvbs2_stream_select.STATUS = api->status;

    SiTRACE("Si2164B DVBS2_STREAM_SELECT ");
  #ifdef   DEBUG_RANGE_CHECK
    SiTRACE("STREAM_ID %d "      , stream_id       );
    if ((stream_sel_mode > Si2164B_DVBS2_STREAM_SELECT_CMD_STREAM_SEL_MODE_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("STREAM_SEL_MODE %d ", stream_sel_mode );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DVBS2_STREAM_SELECT_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( stream_id       & Si2164B_DVBS2_STREAM_SELECT_CMD_STREAM_ID_MASK       ) << Si2164B_DVBS2_STREAM_SELECT_CMD_STREAM_ID_LSB      );
    cmdByteBuffer[2] = (unsigned char) ( ( stream_sel_mode & Si2164B_DVBS2_STREAM_SELECT_CMD_STREAM_SEL_MODE_MASK ) << Si2164B_DVBS2_STREAM_SELECT_CMD_STREAM_SEL_MODE_LSB);

    if (L0_WriteCommandBytes(api->i2c, 3, cmdByteBuffer) != 3) {
      SiTRACE("Error writing DVBS2_STREAM_SELECT bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 1, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DVBS2_STREAM_SELECT response\n");
      return error_code;
    }


    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DVBS2_STREAM_SELECT_CMD */
#ifdef    Si2164B_DVBS_STATUS_CMD
/*---------------------------------------------------*/
/* Si2164B_DVBS_STATUS COMMAND                                     */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DVBS_STATUS               (L1_Si2164B_Context *api,
                                                   unsigned char   intack)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[10];
  /* FREQ OFFSET workaround variables  (only for Si2167B/Si2166B compatibility) */
    unsigned int adc_mode;
  /* end FREQ OFFSET workaround variables */
    api->rsp->dvbs_status.STATUS = api->status;

    SiTRACE("Si2164B DVBS_STATUS ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((intack > Si2164B_DVBS_STATUS_CMD_INTACK_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("INTACK %d ", intack );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DVBS_STATUS_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( intack & Si2164B_DVBS_STATUS_CMD_INTACK_MASK ) << Si2164B_DVBS_STATUS_CMD_INTACK_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing DVBS_STATUS bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 10, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DVBS_STATUS response\n");
      return error_code;
    }

    api->rsp->dvbs_status.pclint        = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBS_STATUS_RESPONSE_PCLINT_LSB         , Si2164B_DVBS_STATUS_RESPONSE_PCLINT_MASK        );
    api->rsp->dvbs_status.dlint         = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBS_STATUS_RESPONSE_DLINT_LSB          , Si2164B_DVBS_STATUS_RESPONSE_DLINT_MASK         );
    api->rsp->dvbs_status.berint        = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBS_STATUS_RESPONSE_BERINT_LSB         , Si2164B_DVBS_STATUS_RESPONSE_BERINT_MASK        );
    api->rsp->dvbs_status.uncorint      = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBS_STATUS_RESPONSE_UNCORINT_LSB       , Si2164B_DVBS_STATUS_RESPONSE_UNCORINT_MASK      );
    api->rsp->dvbs_status.pcl           = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBS_STATUS_RESPONSE_PCL_LSB            , Si2164B_DVBS_STATUS_RESPONSE_PCL_MASK           );
    api->rsp->dvbs_status.dl            = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBS_STATUS_RESPONSE_DL_LSB             , Si2164B_DVBS_STATUS_RESPONSE_DL_MASK            );
    api->rsp->dvbs_status.ber           = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBS_STATUS_RESPONSE_BER_LSB            , Si2164B_DVBS_STATUS_RESPONSE_BER_MASK           );
    api->rsp->dvbs_status.uncor         = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBS_STATUS_RESPONSE_UNCOR_LSB          , Si2164B_DVBS_STATUS_RESPONSE_UNCOR_MASK         );
    api->rsp->dvbs_status.cnr           = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_DVBS_STATUS_RESPONSE_CNR_LSB            , Si2164B_DVBS_STATUS_RESPONSE_CNR_MASK           );
    api->rsp->dvbs_status.afc_freq      = Si2164B_convert_to_int   (&rspByteBuffer[ 4] , Si2164B_DVBS_STATUS_RESPONSE_AFC_FREQ_LSB       , Si2164B_DVBS_STATUS_RESPONSE_AFC_FREQ_MASK      );
    api->rsp->dvbs_status.timing_offset = Si2164B_convert_to_int   (&rspByteBuffer[ 6] , Si2164B_DVBS_STATUS_RESPONSE_TIMING_OFFSET_LSB  , Si2164B_DVBS_STATUS_RESPONSE_TIMING_OFFSET_MASK );
    api->rsp->dvbs_status.constellation = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_DVBS_STATUS_RESPONSE_CONSTELLATION_LSB  , Si2164B_DVBS_STATUS_RESPONSE_CONSTELLATION_MASK );
    api->rsp->dvbs_status.sp_inv        = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_DVBS_STATUS_RESPONSE_SP_INV_LSB         , Si2164B_DVBS_STATUS_RESPONSE_SP_INV_MASK        );
    api->rsp->dvbs_status.code_rate     = Si2164B_convert_to_byte  (&rspByteBuffer[ 9] , Si2164B_DVBS_STATUS_RESPONSE_CODE_RATE_LSB      , Si2164B_DVBS_STATUS_RESPONSE_CODE_RATE_MASK     );

	/* FREQ OFFSET workaround */
    if (  (api->rsp->part_info.part     == 67 ) & (api->rsp->part_info.romid    ==  0 ) ) {
	  SiTRACE("API      dvbs_status.afc_freq : %d \n", api->rsp->dvbs_status.afc_freq);

	  /* read adc_mode register */
      adc_mode               = Si2164B_L1_GET_REG (api,  1,  40,  1);
      SiTRACE("adc_mode               : %u \n ", adc_mode);
	  if (adc_mode==0){
	    api->rsp->dvbs_status.afc_freq = -(api->rsp->dvbs_status.afc_freq);
	  }
	}
    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DVBS_STATUS_CMD */

#ifdef    Si2164B_DVBT2_FEF_CMD
/*---------------------------------------------------*/
/* Si2164B_DVBT2_FEF COMMAND                                         */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DVBT2_FEF                 (L1_Si2164B_Context *api,
                                                   unsigned char   fef_tuner_flag,
                                                   unsigned char   fef_tuner_flag_inv)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[12];
    api->rsp->dvbt2_fef.STATUS = api->status;

    SiTRACE("Si2164B DVBT2_FEF ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((fef_tuner_flag     > Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_MAX    ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("FEF_TUNER_FLAG %d "    , fef_tuner_flag     );
    if ((fef_tuner_flag_inv > Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_INV_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("FEF_TUNER_FLAG_INV %d ", fef_tuner_flag_inv );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DVBT2_FEF_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( fef_tuner_flag     & Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_MASK     ) << Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_LSB    |
                                         ( fef_tuner_flag_inv & Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_INV_MASK ) << Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_INV_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing DVBT2_FEF bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 12, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DVBT2_FEF response\n");
      return error_code;
    }

    api->rsp->dvbt2_fef.fef_type       = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBT2_FEF_RESPONSE_FEF_TYPE_LSB        , Si2164B_DVBT2_FEF_RESPONSE_FEF_TYPE_MASK       );
    api->rsp->dvbt2_fef.fef_length     = Si2164B_convert_to_ulong (&rspByteBuffer[ 4] , Si2164B_DVBT2_FEF_RESPONSE_FEF_LENGTH_LSB      , Si2164B_DVBT2_FEF_RESPONSE_FEF_LENGTH_MASK     );
    api->rsp->dvbt2_fef.fef_repetition = Si2164B_convert_to_ulong (&rspByteBuffer[ 8] , Si2164B_DVBT2_FEF_RESPONSE_FEF_REPETITION_LSB  , Si2164B_DVBT2_FEF_RESPONSE_FEF_REPETITION_MASK );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DVBT2_FEF_CMD */
#ifdef    Si2164B_DVBT2_PLP_INFO_CMD
/*---------------------------------------------------*/
/* Si2164B_DVBT2_PLP_INFO COMMAND                               */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DVBT2_PLP_INFO            (L1_Si2164B_Context *api,
                                                   unsigned char   plp_index)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[13];
    api->rsp->dvbt2_plp_info.STATUS = api->status;

    SiTRACE("Si2164B DVBT2_PLP_INFO ");
  #ifdef   DEBUG_RANGE_CHECK
    SiTRACE("PLP_INDEX %d ", plp_index );
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DVBT2_PLP_INFO_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( plp_index & Si2164B_DVBT2_PLP_INFO_CMD_PLP_INDEX_MASK ) << Si2164B_DVBT2_PLP_INFO_CMD_PLP_INDEX_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing DVBT2_PLP_INFO bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 13, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DVBT2_PLP_INFO response\n");
      return error_code;
    }

    api->rsp->dvbt2_plp_info.plp_id                 = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_ID_LSB                  , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_ID_MASK                 );
    api->rsp->dvbt2_plp_info.plp_payload_type       = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_PAYLOAD_TYPE_LSB        , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_PAYLOAD_TYPE_MASK       );
    api->rsp->dvbt2_plp_info.plp_type               = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_TYPE_LSB                , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_TYPE_MASK               );
    api->rsp->dvbt2_plp_info.first_frame_idx_msb    = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_DVBT2_PLP_INFO_RESPONSE_FIRST_FRAME_IDX_MSB_LSB     , Si2164B_DVBT2_PLP_INFO_RESPONSE_FIRST_FRAME_IDX_MSB_MASK    );
    api->rsp->dvbt2_plp_info.first_rf_idx           = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_DVBT2_PLP_INFO_RESPONSE_FIRST_RF_IDX_LSB            , Si2164B_DVBT2_PLP_INFO_RESPONSE_FIRST_RF_IDX_MASK           );
    api->rsp->dvbt2_plp_info.ff_flag                = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_DVBT2_PLP_INFO_RESPONSE_FF_FLAG_LSB                 , Si2164B_DVBT2_PLP_INFO_RESPONSE_FF_FLAG_MASK                );
    api->rsp->dvbt2_plp_info.plp_group_id_msb       = Si2164B_convert_to_byte  (&rspByteBuffer[ 4] , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_GROUP_ID_MSB_LSB        , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_GROUP_ID_MSB_MASK       );
    api->rsp->dvbt2_plp_info.first_frame_idx_lsb    = Si2164B_convert_to_byte  (&rspByteBuffer[ 4] , Si2164B_DVBT2_PLP_INFO_RESPONSE_FIRST_FRAME_IDX_LSB_LSB     , Si2164B_DVBT2_PLP_INFO_RESPONSE_FIRST_FRAME_IDX_LSB_MASK    );
    api->rsp->dvbt2_plp_info.plp_mod_msb            = Si2164B_convert_to_byte  (&rspByteBuffer[ 5] , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_MOD_MSB_LSB             , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_MOD_MSB_MASK            );
    api->rsp->dvbt2_plp_info.plp_cod                = Si2164B_convert_to_byte  (&rspByteBuffer[ 5] , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_COD_LSB                 , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_COD_MASK                );
    api->rsp->dvbt2_plp_info.plp_group_id_lsb       = Si2164B_convert_to_byte  (&rspByteBuffer[ 5] , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_GROUP_ID_LSB_LSB        , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_GROUP_ID_LSB_MASK       );
    api->rsp->dvbt2_plp_info.plp_num_blocks_max_msb = Si2164B_convert_to_byte  (&rspByteBuffer[ 6] , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_NUM_BLOCKS_MAX_MSB_LSB  , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_NUM_BLOCKS_MAX_MSB_MASK );
    api->rsp->dvbt2_plp_info.plp_fec_type           = Si2164B_convert_to_byte  (&rspByteBuffer[ 6] , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_FEC_TYPE_LSB            , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_FEC_TYPE_MASK           );
    api->rsp->dvbt2_plp_info.plp_rot                = Si2164B_convert_to_byte  (&rspByteBuffer[ 6] , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_ROT_LSB                 , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_ROT_MASK                );
    api->rsp->dvbt2_plp_info.plp_mod_lsb            = Si2164B_convert_to_byte  (&rspByteBuffer[ 6] , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_MOD_LSB_LSB             , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_MOD_LSB_MASK            );
    api->rsp->dvbt2_plp_info.frame_interval_msb     = Si2164B_convert_to_byte  (&rspByteBuffer[ 7] , Si2164B_DVBT2_PLP_INFO_RESPONSE_FRAME_INTERVAL_MSB_LSB      , Si2164B_DVBT2_PLP_INFO_RESPONSE_FRAME_INTERVAL_MSB_MASK     );
    api->rsp->dvbt2_plp_info.plp_num_blocks_max_lsb = Si2164B_convert_to_byte  (&rspByteBuffer[ 7] , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_NUM_BLOCKS_MAX_LSB_LSB  , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_NUM_BLOCKS_MAX_LSB_MASK );
    api->rsp->dvbt2_plp_info.time_il_length_msb     = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_DVBT2_PLP_INFO_RESPONSE_TIME_IL_LENGTH_MSB_LSB      , Si2164B_DVBT2_PLP_INFO_RESPONSE_TIME_IL_LENGTH_MSB_MASK     );
    api->rsp->dvbt2_plp_info.frame_interval_lsb     = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_DVBT2_PLP_INFO_RESPONSE_FRAME_INTERVAL_LSB_LSB      , Si2164B_DVBT2_PLP_INFO_RESPONSE_FRAME_INTERVAL_LSB_MASK     );
    api->rsp->dvbt2_plp_info.time_il_type           = Si2164B_convert_to_byte  (&rspByteBuffer[ 9] , Si2164B_DVBT2_PLP_INFO_RESPONSE_TIME_IL_TYPE_LSB            , Si2164B_DVBT2_PLP_INFO_RESPONSE_TIME_IL_TYPE_MASK           );
    api->rsp->dvbt2_plp_info.time_il_length_lsb     = Si2164B_convert_to_byte  (&rspByteBuffer[ 9] , Si2164B_DVBT2_PLP_INFO_RESPONSE_TIME_IL_LENGTH_LSB_LSB      , Si2164B_DVBT2_PLP_INFO_RESPONSE_TIME_IL_LENGTH_LSB_MASK     );
    api->rsp->dvbt2_plp_info.reserved_1_1           = Si2164B_convert_to_byte  (&rspByteBuffer[10] , Si2164B_DVBT2_PLP_INFO_RESPONSE_RESERVED_1_1_LSB            , Si2164B_DVBT2_PLP_INFO_RESPONSE_RESERVED_1_1_MASK           );
    api->rsp->dvbt2_plp_info.in_band_b_flag         = Si2164B_convert_to_byte  (&rspByteBuffer[10] , Si2164B_DVBT2_PLP_INFO_RESPONSE_IN_BAND_B_FLAG_LSB          , Si2164B_DVBT2_PLP_INFO_RESPONSE_IN_BAND_B_FLAG_MASK         );
    api->rsp->dvbt2_plp_info.in_band_a_flag         = Si2164B_convert_to_byte  (&rspByteBuffer[10] , Si2164B_DVBT2_PLP_INFO_RESPONSE_IN_BAND_A_FLAG_LSB          , Si2164B_DVBT2_PLP_INFO_RESPONSE_IN_BAND_A_FLAG_MASK         );
    api->rsp->dvbt2_plp_info.static_flag            = Si2164B_convert_to_byte  (&rspByteBuffer[11] , Si2164B_DVBT2_PLP_INFO_RESPONSE_STATIC_FLAG_LSB             , Si2164B_DVBT2_PLP_INFO_RESPONSE_STATIC_FLAG_MASK            );
    api->rsp->dvbt2_plp_info.plp_mode               = Si2164B_convert_to_byte  (&rspByteBuffer[11] , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_MODE_LSB                , Si2164B_DVBT2_PLP_INFO_RESPONSE_PLP_MODE_MASK               );
    api->rsp->dvbt2_plp_info.reserved_1_2           = Si2164B_convert_to_byte  (&rspByteBuffer[11] , Si2164B_DVBT2_PLP_INFO_RESPONSE_RESERVED_1_2_LSB            , Si2164B_DVBT2_PLP_INFO_RESPONSE_RESERVED_1_2_MASK           );
    api->rsp->dvbt2_plp_info.static_padding_flag    = Si2164B_convert_to_byte  (&rspByteBuffer[12] , Si2164B_DVBT2_PLP_INFO_RESPONSE_STATIC_PADDING_FLAG_LSB     , Si2164B_DVBT2_PLP_INFO_RESPONSE_STATIC_PADDING_FLAG_MASK    );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DVBT2_PLP_INFO_CMD */
#ifdef    Si2164B_DVBT2_PLP_SELECT_CMD
/*---------------------------------------------------*/
/* Si2164B_DVBT2_PLP_SELECT COMMAND                           */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DVBT2_PLP_SELECT          (L1_Si2164B_Context *api,
                                                   unsigned char   plp_id,
                                                   unsigned char   plp_id_sel_mode)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[3];
    unsigned char rspByteBuffer[1];
    api->rsp->dvbt2_plp_select.STATUS = api->status;

    SiTRACE("Si2164B DVBT2_PLP_SELECT ");
  #ifdef   DEBUG_RANGE_CHECK
    SiTRACE("PLP_ID %d "         , plp_id          );
    if ((plp_id_sel_mode > Si2164B_DVBT2_PLP_SELECT_CMD_PLP_ID_SEL_MODE_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("PLP_ID_SEL_MODE %d ", plp_id_sel_mode );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DVBT2_PLP_SELECT_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( plp_id          & Si2164B_DVBT2_PLP_SELECT_CMD_PLP_ID_MASK          ) << Si2164B_DVBT2_PLP_SELECT_CMD_PLP_ID_LSB         );
    cmdByteBuffer[2] = (unsigned char) ( ( plp_id_sel_mode & Si2164B_DVBT2_PLP_SELECT_CMD_PLP_ID_SEL_MODE_MASK ) << Si2164B_DVBT2_PLP_SELECT_CMD_PLP_ID_SEL_MODE_LSB);

    if (L0_WriteCommandBytes(api->i2c, 3, cmdByteBuffer) != 3) {
      SiTRACE("Error writing DVBT2_PLP_SELECT bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 1, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DVBT2_PLP_SELECT response\n");
      return error_code;
    }


    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DVBT2_PLP_SELECT_CMD */
#ifdef    Si2164B_DVBT2_STATUS_CMD
/*---------------------------------------------------*/
/* Si2164B_DVBT2_STATUS COMMAND                                   */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DVBT2_STATUS              (L1_Si2164B_Context *api,
                                                   unsigned char   intack)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[14];
    api->rsp->dvbt2_status.STATUS = api->status;

    SiTRACE("Si2164B DVBT2_STATUS ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((intack > Si2164B_DVBT2_STATUS_CMD_INTACK_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("INTACK %d ", intack );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DVBT2_STATUS_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( intack & Si2164B_DVBT2_STATUS_CMD_INTACK_MASK ) << Si2164B_DVBT2_STATUS_CMD_INTACK_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing DVBT2_STATUS bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 14, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DVBT2_STATUS response\n");
      return error_code;
    }

    api->rsp->dvbt2_status.pclint        = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBT2_STATUS_RESPONSE_PCLINT_LSB         , Si2164B_DVBT2_STATUS_RESPONSE_PCLINT_MASK        );
    api->rsp->dvbt2_status.dlint         = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBT2_STATUS_RESPONSE_DLINT_LSB          , Si2164B_DVBT2_STATUS_RESPONSE_DLINT_MASK         );
    api->rsp->dvbt2_status.berint        = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBT2_STATUS_RESPONSE_BERINT_LSB         , Si2164B_DVBT2_STATUS_RESPONSE_BERINT_MASK        );
    api->rsp->dvbt2_status.uncorint      = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBT2_STATUS_RESPONSE_UNCORINT_LSB       , Si2164B_DVBT2_STATUS_RESPONSE_UNCORINT_MASK      );
    api->rsp->dvbt2_status.notdvbt2int   = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBT2_STATUS_RESPONSE_NOTDVBT2INT_LSB    , Si2164B_DVBT2_STATUS_RESPONSE_NOTDVBT2INT_MASK   );
    api->rsp->dvbt2_status.pcl           = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBT2_STATUS_RESPONSE_PCL_LSB            , Si2164B_DVBT2_STATUS_RESPONSE_PCL_MASK           );
    api->rsp->dvbt2_status.dl            = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBT2_STATUS_RESPONSE_DL_LSB             , Si2164B_DVBT2_STATUS_RESPONSE_DL_MASK            );
    api->rsp->dvbt2_status.ber           = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBT2_STATUS_RESPONSE_BER_LSB            , Si2164B_DVBT2_STATUS_RESPONSE_BER_MASK           );
    api->rsp->dvbt2_status.uncor         = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBT2_STATUS_RESPONSE_UNCOR_LSB          , Si2164B_DVBT2_STATUS_RESPONSE_UNCOR_MASK         );
    api->rsp->dvbt2_status.notdvbt2      = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBT2_STATUS_RESPONSE_NOTDVBT2_LSB       , Si2164B_DVBT2_STATUS_RESPONSE_NOTDVBT2_MASK      );
    api->rsp->dvbt2_status.cnr           = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_DVBT2_STATUS_RESPONSE_CNR_LSB            , Si2164B_DVBT2_STATUS_RESPONSE_CNR_MASK           );
    api->rsp->dvbt2_status.afc_freq      = Si2164B_convert_to_int   (&rspByteBuffer[ 4] , Si2164B_DVBT2_STATUS_RESPONSE_AFC_FREQ_LSB       , Si2164B_DVBT2_STATUS_RESPONSE_AFC_FREQ_MASK      );
    api->rsp->dvbt2_status.timing_offset = Si2164B_convert_to_int   (&rspByteBuffer[ 6] , Si2164B_DVBT2_STATUS_RESPONSE_TIMING_OFFSET_LSB  , Si2164B_DVBT2_STATUS_RESPONSE_TIMING_OFFSET_MASK );
    api->rsp->dvbt2_status.constellation = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_DVBT2_STATUS_RESPONSE_CONSTELLATION_LSB  , Si2164B_DVBT2_STATUS_RESPONSE_CONSTELLATION_MASK );
    api->rsp->dvbt2_status.sp_inv        = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_DVBT2_STATUS_RESPONSE_SP_INV_LSB         , Si2164B_DVBT2_STATUS_RESPONSE_SP_INV_MASK        );
    api->rsp->dvbt2_status.fef           = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_DVBT2_STATUS_RESPONSE_FEF_LSB            , Si2164B_DVBT2_STATUS_RESPONSE_FEF_MASK           );
    api->rsp->dvbt2_status.fft_mode      = Si2164B_convert_to_byte  (&rspByteBuffer[ 9] , Si2164B_DVBT2_STATUS_RESPONSE_FFT_MODE_LSB       , Si2164B_DVBT2_STATUS_RESPONSE_FFT_MODE_MASK      );
    api->rsp->dvbt2_status.guard_int     = Si2164B_convert_to_byte  (&rspByteBuffer[ 9] , Si2164B_DVBT2_STATUS_RESPONSE_GUARD_INT_LSB      , Si2164B_DVBT2_STATUS_RESPONSE_GUARD_INT_MASK     );
    api->rsp->dvbt2_status.bw_ext        = Si2164B_convert_to_byte  (&rspByteBuffer[ 9] , Si2164B_DVBT2_STATUS_RESPONSE_BW_EXT_LSB         , Si2164B_DVBT2_STATUS_RESPONSE_BW_EXT_MASK        );
    api->rsp->dvbt2_status.num_plp       = Si2164B_convert_to_byte  (&rspByteBuffer[10] , Si2164B_DVBT2_STATUS_RESPONSE_NUM_PLP_LSB        , Si2164B_DVBT2_STATUS_RESPONSE_NUM_PLP_MASK       );
    api->rsp->dvbt2_status.pilot_pattern = Si2164B_convert_to_byte  (&rspByteBuffer[11] , Si2164B_DVBT2_STATUS_RESPONSE_PILOT_PATTERN_LSB  , Si2164B_DVBT2_STATUS_RESPONSE_PILOT_PATTERN_MASK );
    api->rsp->dvbt2_status.tx_mode       = Si2164B_convert_to_byte  (&rspByteBuffer[11] , Si2164B_DVBT2_STATUS_RESPONSE_TX_MODE_LSB        , Si2164B_DVBT2_STATUS_RESPONSE_TX_MODE_MASK       );
    api->rsp->dvbt2_status.rotated       = Si2164B_convert_to_byte  (&rspByteBuffer[11] , Si2164B_DVBT2_STATUS_RESPONSE_ROTATED_LSB        , Si2164B_DVBT2_STATUS_RESPONSE_ROTATED_MASK       );
    api->rsp->dvbt2_status.short_frame   = Si2164B_convert_to_byte  (&rspByteBuffer[11] , Si2164B_DVBT2_STATUS_RESPONSE_SHORT_FRAME_LSB    , Si2164B_DVBT2_STATUS_RESPONSE_SHORT_FRAME_MASK   );
    api->rsp->dvbt2_status.t2_mode       = Si2164B_convert_to_byte  (&rspByteBuffer[11] , Si2164B_DVBT2_STATUS_RESPONSE_T2_MODE_LSB        , Si2164B_DVBT2_STATUS_RESPONSE_T2_MODE_MASK       );
    api->rsp->dvbt2_status.code_rate     = Si2164B_convert_to_byte  (&rspByteBuffer[12] , Si2164B_DVBT2_STATUS_RESPONSE_CODE_RATE_LSB      , Si2164B_DVBT2_STATUS_RESPONSE_CODE_RATE_MASK     );
    api->rsp->dvbt2_status.t2_version    = Si2164B_convert_to_byte  (&rspByteBuffer[12] , Si2164B_DVBT2_STATUS_RESPONSE_T2_VERSION_LSB     , Si2164B_DVBT2_STATUS_RESPONSE_T2_VERSION_MASK    );
    api->rsp->dvbt2_status.plp_id        = Si2164B_convert_to_byte  (&rspByteBuffer[13] , Si2164B_DVBT2_STATUS_RESPONSE_PLP_ID_LSB         , Si2164B_DVBT2_STATUS_RESPONSE_PLP_ID_MASK        );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DVBT2_STATUS_CMD */
#ifdef    Si2164B_DVBT2_TX_ID_CMD
/*---------------------------------------------------*/
/* Si2164B_DVBT2_TX_ID COMMAND                                     */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DVBT2_TX_ID               (L1_Si2164B_Context *api)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[1];
    unsigned char rspByteBuffer[8];
    api->rsp->dvbt2_tx_id.STATUS = api->status;

    SiTRACE("Si2164B DVBT2_TX_ID ");
    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DVBT2_TX_ID_CMD;

    if (L0_WriteCommandBytes(api->i2c, 1, cmdByteBuffer) != 1) {
      SiTRACE("Error writing DVBT2_TX_ID bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 8, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DVBT2_TX_ID response\n");
      return error_code;
    }

    api->rsp->dvbt2_tx_id.tx_id_availability = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBT2_TX_ID_RESPONSE_TX_ID_AVAILABILITY_LSB  , Si2164B_DVBT2_TX_ID_RESPONSE_TX_ID_AVAILABILITY_MASK );
    api->rsp->dvbt2_tx_id.cell_id            = Si2164B_convert_to_uint  (&rspByteBuffer[ 2] , Si2164B_DVBT2_TX_ID_RESPONSE_CELL_ID_LSB             , Si2164B_DVBT2_TX_ID_RESPONSE_CELL_ID_MASK            );
    api->rsp->dvbt2_tx_id.network_id         = Si2164B_convert_to_uint  (&rspByteBuffer[ 4] , Si2164B_DVBT2_TX_ID_RESPONSE_NETWORK_ID_LSB          , Si2164B_DVBT2_TX_ID_RESPONSE_NETWORK_ID_MASK         );
    api->rsp->dvbt2_tx_id.t2_system_id       = Si2164B_convert_to_uint  (&rspByteBuffer[ 6] , Si2164B_DVBT2_TX_ID_RESPONSE_T2_SYSTEM_ID_LSB        , Si2164B_DVBT2_TX_ID_RESPONSE_T2_SYSTEM_ID_MASK       );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DVBT2_TX_ID_CMD */

#ifdef    Si2164B_DVBT_STATUS_CMD
/*---------------------------------------------------*/
/* Si2164B_DVBT_STATUS COMMAND                                     */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DVBT_STATUS               (L1_Si2164B_Context *api,
                                                   unsigned char   intack)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[13];
    api->rsp->dvbt_status.STATUS = api->status;

    SiTRACE("Si2164B DVBT_STATUS ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((intack > Si2164B_DVBT_STATUS_CMD_INTACK_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("INTACK %d ", intack );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DVBT_STATUS_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( intack & Si2164B_DVBT_STATUS_CMD_INTACK_MASK ) << Si2164B_DVBT_STATUS_CMD_INTACK_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing DVBT_STATUS bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 13, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DVBT_STATUS response\n");
      return error_code;
    }

    api->rsp->dvbt_status.pclint        = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBT_STATUS_RESPONSE_PCLINT_LSB         , Si2164B_DVBT_STATUS_RESPONSE_PCLINT_MASK        );
    api->rsp->dvbt_status.dlint         = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBT_STATUS_RESPONSE_DLINT_LSB          , Si2164B_DVBT_STATUS_RESPONSE_DLINT_MASK         );
    api->rsp->dvbt_status.berint        = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBT_STATUS_RESPONSE_BERINT_LSB         , Si2164B_DVBT_STATUS_RESPONSE_BERINT_MASK        );
    api->rsp->dvbt_status.uncorint      = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBT_STATUS_RESPONSE_UNCORINT_LSB       , Si2164B_DVBT_STATUS_RESPONSE_UNCORINT_MASK      );
    api->rsp->dvbt_status.notdvbtint    = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBT_STATUS_RESPONSE_NOTDVBTINT_LSB     , Si2164B_DVBT_STATUS_RESPONSE_NOTDVBTINT_MASK    );
    api->rsp->dvbt_status.pcl           = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBT_STATUS_RESPONSE_PCL_LSB            , Si2164B_DVBT_STATUS_RESPONSE_PCL_MASK           );
    api->rsp->dvbt_status.dl            = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBT_STATUS_RESPONSE_DL_LSB             , Si2164B_DVBT_STATUS_RESPONSE_DL_MASK            );
    api->rsp->dvbt_status.ber           = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBT_STATUS_RESPONSE_BER_LSB            , Si2164B_DVBT_STATUS_RESPONSE_BER_MASK           );
    api->rsp->dvbt_status.uncor         = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBT_STATUS_RESPONSE_UNCOR_LSB          , Si2164B_DVBT_STATUS_RESPONSE_UNCOR_MASK         );
    api->rsp->dvbt_status.notdvbt       = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_DVBT_STATUS_RESPONSE_NOTDVBT_LSB        , Si2164B_DVBT_STATUS_RESPONSE_NOTDVBT_MASK       );
    api->rsp->dvbt_status.cnr           = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_DVBT_STATUS_RESPONSE_CNR_LSB            , Si2164B_DVBT_STATUS_RESPONSE_CNR_MASK           );
    api->rsp->dvbt_status.afc_freq      = Si2164B_convert_to_int   (&rspByteBuffer[ 4] , Si2164B_DVBT_STATUS_RESPONSE_AFC_FREQ_LSB       , Si2164B_DVBT_STATUS_RESPONSE_AFC_FREQ_MASK      );
    api->rsp->dvbt_status.timing_offset = Si2164B_convert_to_int   (&rspByteBuffer[ 6] , Si2164B_DVBT_STATUS_RESPONSE_TIMING_OFFSET_LSB  , Si2164B_DVBT_STATUS_RESPONSE_TIMING_OFFSET_MASK );
    api->rsp->dvbt_status.constellation = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_DVBT_STATUS_RESPONSE_CONSTELLATION_LSB  , Si2164B_DVBT_STATUS_RESPONSE_CONSTELLATION_MASK );
    api->rsp->dvbt_status.sp_inv        = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_DVBT_STATUS_RESPONSE_SP_INV_LSB         , Si2164B_DVBT_STATUS_RESPONSE_SP_INV_MASK        );
    api->rsp->dvbt_status.rate_hp       = Si2164B_convert_to_byte  (&rspByteBuffer[ 9] , Si2164B_DVBT_STATUS_RESPONSE_RATE_HP_LSB        , Si2164B_DVBT_STATUS_RESPONSE_RATE_HP_MASK       );
    api->rsp->dvbt_status.rate_lp       = Si2164B_convert_to_byte  (&rspByteBuffer[ 9] , Si2164B_DVBT_STATUS_RESPONSE_RATE_LP_LSB        , Si2164B_DVBT_STATUS_RESPONSE_RATE_LP_MASK       );
    api->rsp->dvbt_status.fft_mode      = Si2164B_convert_to_byte  (&rspByteBuffer[10] , Si2164B_DVBT_STATUS_RESPONSE_FFT_MODE_LSB       , Si2164B_DVBT_STATUS_RESPONSE_FFT_MODE_MASK      );
    api->rsp->dvbt_status.guard_int     = Si2164B_convert_to_byte  (&rspByteBuffer[10] , Si2164B_DVBT_STATUS_RESPONSE_GUARD_INT_LSB      , Si2164B_DVBT_STATUS_RESPONSE_GUARD_INT_MASK     );
    api->rsp->dvbt_status.hierarchy     = Si2164B_convert_to_byte  (&rspByteBuffer[11] , Si2164B_DVBT_STATUS_RESPONSE_HIERARCHY_LSB      , Si2164B_DVBT_STATUS_RESPONSE_HIERARCHY_MASK     );
    api->rsp->dvbt_status.tps_length    = Si2164B_convert_to_byte  (&rspByteBuffer[12] , Si2164B_DVBT_STATUS_RESPONSE_TPS_LENGTH_LSB     , Si2164B_DVBT_STATUS_RESPONSE_TPS_LENGTH_MASK    );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DVBT_STATUS_CMD */
#ifdef    Si2164B_DVBT_TPS_EXTRA_CMD
/*---------------------------------------------------*/
/* Si2164B_DVBT_TPS_EXTRA COMMAND                               */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_DVBT_TPS_EXTRA            (L1_Si2164B_Context *api)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[1];
    unsigned char rspByteBuffer[6];
    api->rsp->dvbt_tps_extra.STATUS = api->status;

    SiTRACE("Si2164B DVBT_TPS_EXTRA ");
    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_DVBT_TPS_EXTRA_CMD;

    if (L0_WriteCommandBytes(api->i2c, 1, cmdByteBuffer) != 1) {
      SiTRACE("Error writing DVBT_TPS_EXTRA bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 6, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling DVBT_TPS_EXTRA response\n");
      return error_code;
    }

    api->rsp->dvbt_tps_extra.lptimeslice = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBT_TPS_EXTRA_RESPONSE_LPTIMESLICE_LSB  , Si2164B_DVBT_TPS_EXTRA_RESPONSE_LPTIMESLICE_MASK );
    api->rsp->dvbt_tps_extra.hptimeslice = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBT_TPS_EXTRA_RESPONSE_HPTIMESLICE_LSB  , Si2164B_DVBT_TPS_EXTRA_RESPONSE_HPTIMESLICE_MASK );
    api->rsp->dvbt_tps_extra.lpmpefec    = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBT_TPS_EXTRA_RESPONSE_LPMPEFEC_LSB     , Si2164B_DVBT_TPS_EXTRA_RESPONSE_LPMPEFEC_MASK    );
    api->rsp->dvbt_tps_extra.hpmpefec    = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBT_TPS_EXTRA_RESPONSE_HPMPEFEC_LSB     , Si2164B_DVBT_TPS_EXTRA_RESPONSE_HPMPEFEC_MASK    );
    api->rsp->dvbt_tps_extra.dvbhinter   = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_DVBT_TPS_EXTRA_RESPONSE_DVBHINTER_LSB    , Si2164B_DVBT_TPS_EXTRA_RESPONSE_DVBHINTER_MASK   );
    api->rsp->dvbt_tps_extra.cell_id     = Si2164B_convert_to_int   (&rspByteBuffer[ 2] , Si2164B_DVBT_TPS_EXTRA_RESPONSE_CELL_ID_LSB      , Si2164B_DVBT_TPS_EXTRA_RESPONSE_CELL_ID_MASK     );
    api->rsp->dvbt_tps_extra.tps_res1    = Si2164B_convert_to_byte  (&rspByteBuffer[ 4] , Si2164B_DVBT_TPS_EXTRA_RESPONSE_TPS_RES1_LSB     , Si2164B_DVBT_TPS_EXTRA_RESPONSE_TPS_RES1_MASK    );
    api->rsp->dvbt_tps_extra.tps_res2    = Si2164B_convert_to_byte  (&rspByteBuffer[ 4] , Si2164B_DVBT_TPS_EXTRA_RESPONSE_TPS_RES2_LSB     , Si2164B_DVBT_TPS_EXTRA_RESPONSE_TPS_RES2_MASK    );
    api->rsp->dvbt_tps_extra.tps_res3    = Si2164B_convert_to_byte  (&rspByteBuffer[ 5] , Si2164B_DVBT_TPS_EXTRA_RESPONSE_TPS_RES3_LSB     , Si2164B_DVBT_TPS_EXTRA_RESPONSE_TPS_RES3_MASK    );
    api->rsp->dvbt_tps_extra.tps_res4    = Si2164B_convert_to_byte  (&rspByteBuffer[ 5] , Si2164B_DVBT_TPS_EXTRA_RESPONSE_TPS_RES4_LSB     , Si2164B_DVBT_TPS_EXTRA_RESPONSE_TPS_RES4_MASK    );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_DVBT_TPS_EXTRA_CMD */

#ifdef    Si2164B_EXIT_BOOTLOADER_CMD
/*---------------------------------------------------*/
/* Si2164B_EXIT_BOOTLOADER COMMAND                             */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_EXIT_BOOTLOADER           (L1_Si2164B_Context *api,
                                                   unsigned char   func,
                                                   unsigned char   ctsien)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[1];
    api->rsp->exit_bootloader.STATUS = api->status;

    SiTRACE("Si2164B EXIT_BOOTLOADER ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((func   > Si2164B_EXIT_BOOTLOADER_CMD_FUNC_MAX  )  || (func   < Si2164B_EXIT_BOOTLOADER_CMD_FUNC_MIN  ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("FUNC %d "  , func   );
    if ((ctsien > Si2164B_EXIT_BOOTLOADER_CMD_CTSIEN_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("CTSIEN %d ", ctsien );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_EXIT_BOOTLOADER_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( func   & Si2164B_EXIT_BOOTLOADER_CMD_FUNC_MASK   ) << Si2164B_EXIT_BOOTLOADER_CMD_FUNC_LSB  |
                                         ( ctsien & Si2164B_EXIT_BOOTLOADER_CMD_CTSIEN_MASK ) << Si2164B_EXIT_BOOTLOADER_CMD_CTSIEN_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing EXIT_BOOTLOADER bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 1, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling EXIT_BOOTLOADER response\n");
      return error_code;
    }


    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_EXIT_BOOTLOADER_CMD */
#ifdef    Si2164B_GET_PROPERTY_CMD
/*---------------------------------------------------*/
/* Si2164B_GET_PROPERTY COMMAND                                   */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_GET_PROPERTY              (L1_Si2164B_Context *api,
                                                   unsigned char   reserved,
                                                   unsigned int    prop)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[4];
    unsigned char rspByteBuffer[4];
    api->rsp->get_property.STATUS = api->status;

    SiTRACE("Si2164B GET_PROPERTY ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((reserved > Si2164B_GET_PROPERTY_CMD_RESERVED_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("RESERVED %d ", reserved );
    SiTRACE("PROP %d "    , prop     );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_GET_PROPERTY_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( reserved & Si2164B_GET_PROPERTY_CMD_RESERVED_MASK ) << Si2164B_GET_PROPERTY_CMD_RESERVED_LSB);
    cmdByteBuffer[2] = (unsigned char) ( ( prop     & Si2164B_GET_PROPERTY_CMD_PROP_MASK     ) << Si2164B_GET_PROPERTY_CMD_PROP_LSB    );
    cmdByteBuffer[3] = (unsigned char) ((( prop     & Si2164B_GET_PROPERTY_CMD_PROP_MASK     ) << Si2164B_GET_PROPERTY_CMD_PROP_LSB    )>>8);

    if (L0_WriteCommandBytes(api->i2c, 4, cmdByteBuffer) != 4) {
      SiTRACE("Error writing GET_PROPERTY bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 4, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling GET_PROPERTY response\n");
      return error_code;
    }

    api->rsp->get_property.reserved = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_GET_PROPERTY_RESPONSE_RESERVED_LSB  , Si2164B_GET_PROPERTY_RESPONSE_RESERVED_MASK );
    api->rsp->get_property.data     = Si2164B_convert_to_uint  (&rspByteBuffer[ 2] , Si2164B_GET_PROPERTY_RESPONSE_DATA_LSB      , Si2164B_GET_PROPERTY_RESPONSE_DATA_MASK     );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_GET_PROPERTY_CMD */
#ifdef    Si2164B_GET_REV_CMD
/*---------------------------------------------------*/
/* Si2164B_GET_REV COMMAND                                             */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_GET_REV                   (L1_Si2164B_Context *api)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[1];
    unsigned char rspByteBuffer[13];
    api->rsp->get_rev.STATUS = api->status;

    SiTRACE("Si2164B GET_REV ");
    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_GET_REV_CMD;

    if (L0_WriteCommandBytes(api->i2c, 1, cmdByteBuffer) != 1) {
      SiTRACE("Error writing GET_REV bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 13, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling GET_REV response\n");
      return error_code;
    }

    api->rsp->get_rev.pn       = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_GET_REV_RESPONSE_PN_LSB        , Si2164B_GET_REV_RESPONSE_PN_MASK       );
    api->rsp->get_rev.fwmajor  = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_GET_REV_RESPONSE_FWMAJOR_LSB   , Si2164B_GET_REV_RESPONSE_FWMAJOR_MASK  );
    api->rsp->get_rev.fwminor  = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_GET_REV_RESPONSE_FWMINOR_LSB   , Si2164B_GET_REV_RESPONSE_FWMINOR_MASK  );
    api->rsp->get_rev.patch    = Si2164B_convert_to_uint  (&rspByteBuffer[ 4] , Si2164B_GET_REV_RESPONSE_PATCH_LSB     , Si2164B_GET_REV_RESPONSE_PATCH_MASK    );
    api->rsp->get_rev.cmpmajor = Si2164B_convert_to_byte  (&rspByteBuffer[ 6] , Si2164B_GET_REV_RESPONSE_CMPMAJOR_LSB  , Si2164B_GET_REV_RESPONSE_CMPMAJOR_MASK );
    api->rsp->get_rev.cmpminor = Si2164B_convert_to_byte  (&rspByteBuffer[ 7] , Si2164B_GET_REV_RESPONSE_CMPMINOR_LSB  , Si2164B_GET_REV_RESPONSE_CMPMINOR_MASK );
    api->rsp->get_rev.cmpbuild = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_GET_REV_RESPONSE_CMPBUILD_LSB  , Si2164B_GET_REV_RESPONSE_CMPBUILD_MASK );
    api->rsp->get_rev.chiprev  = Si2164B_convert_to_byte  (&rspByteBuffer[ 9] , Si2164B_GET_REV_RESPONSE_CHIPREV_LSB   , Si2164B_GET_REV_RESPONSE_CHIPREV_MASK  );
    api->rsp->get_rev.mcm_die  = Si2164B_convert_to_byte  (&rspByteBuffer[ 9] , Si2164B_GET_REV_RESPONSE_MCM_DIE_LSB   , Si2164B_GET_REV_RESPONSE_MCM_DIE_MASK  );
    api->rsp->get_rev.rx       = Si2164B_convert_to_byte  (&rspByteBuffer[12] , Si2164B_GET_REV_RESPONSE_RX_LSB        , Si2164B_GET_REV_RESPONSE_RX_MASK       );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_GET_REV_CMD */
#ifdef    Si2164B_I2C_PASSTHROUGH_CMD
/*---------------------------------------------------*/
/* Si2164B_I2C_PASSTHROUGH COMMAND                             */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_I2C_PASSTHROUGH           (L1_Si2164B_Context *api,
                                                   unsigned char   subcode,
                                                   unsigned char   i2c_passthru,
                                                   unsigned char   reserved)
{
    unsigned char cmdByteBuffer[3];
  #ifdef   DEBUG_RANGE_CHECK
    unsigned char error_code = 0;
  #endif /* DEBUG_RANGE_CHECK */
    api->rsp->i2c_passthrough.STATUS = api->status;

    SiTRACE("Si2164B I2C_PASSTHROUGH ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((subcode      > Si2164B_I2C_PASSTHROUGH_CMD_SUBCODE_MAX     )  || (subcode      < Si2164B_I2C_PASSTHROUGH_CMD_SUBCODE_MIN     ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("SUBCODE %d "     , subcode      );
    if ((i2c_passthru > Si2164B_I2C_PASSTHROUGH_CMD_I2C_PASSTHRU_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("I2C_PASSTHRU %d ", i2c_passthru );
    if ((reserved     > Si2164B_I2C_PASSTHROUGH_CMD_RESERVED_MAX    ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("RESERVED %d "    , reserved     );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_I2C_PASSTHROUGH_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( subcode      & Si2164B_I2C_PASSTHROUGH_CMD_SUBCODE_MASK      ) << Si2164B_I2C_PASSTHROUGH_CMD_SUBCODE_LSB     );
    cmdByteBuffer[2] = (unsigned char) ( ( i2c_passthru & Si2164B_I2C_PASSTHROUGH_CMD_I2C_PASSTHRU_MASK ) << Si2164B_I2C_PASSTHROUGH_CMD_I2C_PASSTHRU_LSB|
                                         ( reserved     & Si2164B_I2C_PASSTHROUGH_CMD_RESERVED_MASK     ) << Si2164B_I2C_PASSTHROUGH_CMD_RESERVED_LSB    );

    if (L0_WriteCommandBytes(api->i2c, 3, cmdByteBuffer) != 3) {
      SiTRACE("Error writing I2C_PASSTHROUGH bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_I2C_PASSTHROUGH_CMD */

#ifdef    Si2164B_MCNS_STATUS_CMD
/*---------------------------------------------------*/
/* Si2164B_MCNS_STATUS COMMAND                                     */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_MCNS_STATUS               (L1_Si2164B_Context *api,
                                                   unsigned char   intack)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[10];
    api->rsp->mcns_status.STATUS = api->status;

    SiTRACE("Si2164B MCNS_STATUS ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((intack > Si2164B_MCNS_STATUS_CMD_INTACK_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("INTACK %d ", intack );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_MCNS_STATUS_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( intack & Si2164B_MCNS_STATUS_CMD_INTACK_MASK ) << Si2164B_MCNS_STATUS_CMD_INTACK_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing MCNS_STATUS bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 10, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling MCNS_STATUS response\n");
      return error_code;
    }

    api->rsp->mcns_status.pclint        = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_MCNS_STATUS_RESPONSE_PCLINT_LSB         , Si2164B_MCNS_STATUS_RESPONSE_PCLINT_MASK        );
    api->rsp->mcns_status.dlint         = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_MCNS_STATUS_RESPONSE_DLINT_LSB          , Si2164B_MCNS_STATUS_RESPONSE_DLINT_MASK         );
    api->rsp->mcns_status.berint        = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_MCNS_STATUS_RESPONSE_BERINT_LSB         , Si2164B_MCNS_STATUS_RESPONSE_BERINT_MASK        );
    api->rsp->mcns_status.uncorint      = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_MCNS_STATUS_RESPONSE_UNCORINT_LSB       , Si2164B_MCNS_STATUS_RESPONSE_UNCORINT_MASK      );
    api->rsp->mcns_status.pcl           = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_MCNS_STATUS_RESPONSE_PCL_LSB            , Si2164B_MCNS_STATUS_RESPONSE_PCL_MASK           );
    api->rsp->mcns_status.dl            = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_MCNS_STATUS_RESPONSE_DL_LSB             , Si2164B_MCNS_STATUS_RESPONSE_DL_MASK            );
    api->rsp->mcns_status.ber           = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_MCNS_STATUS_RESPONSE_BER_LSB            , Si2164B_MCNS_STATUS_RESPONSE_BER_MASK           );
    api->rsp->mcns_status.uncor         = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_MCNS_STATUS_RESPONSE_UNCOR_LSB          , Si2164B_MCNS_STATUS_RESPONSE_UNCOR_MASK         );
    api->rsp->mcns_status.cnr           = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_MCNS_STATUS_RESPONSE_CNR_LSB            , Si2164B_MCNS_STATUS_RESPONSE_CNR_MASK           );
    api->rsp->mcns_status.afc_freq      = Si2164B_convert_to_int   (&rspByteBuffer[ 4] , Si2164B_MCNS_STATUS_RESPONSE_AFC_FREQ_LSB       , Si2164B_MCNS_STATUS_RESPONSE_AFC_FREQ_MASK      );
    api->rsp->mcns_status.timing_offset = Si2164B_convert_to_int   (&rspByteBuffer[ 6] , Si2164B_MCNS_STATUS_RESPONSE_TIMING_OFFSET_LSB  , Si2164B_MCNS_STATUS_RESPONSE_TIMING_OFFSET_MASK );
    api->rsp->mcns_status.constellation = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_MCNS_STATUS_RESPONSE_CONSTELLATION_LSB  , Si2164B_MCNS_STATUS_RESPONSE_CONSTELLATION_MASK );
    api->rsp->mcns_status.sp_inv        = Si2164B_convert_to_byte  (&rspByteBuffer[ 8] , Si2164B_MCNS_STATUS_RESPONSE_SP_INV_LSB         , Si2164B_MCNS_STATUS_RESPONSE_SP_INV_MASK        );
    api->rsp->mcns_status.interleaving  = Si2164B_convert_to_byte  (&rspByteBuffer[ 9] , Si2164B_MCNS_STATUS_RESPONSE_INTERLEAVING_LSB   , Si2164B_MCNS_STATUS_RESPONSE_INTERLEAVING_MASK  );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_MCNS_STATUS_CMD */

#ifdef    Si2164B_PART_INFO_CMD
/*---------------------------------------------------*/
/* Si2164B_PART_INFO COMMAND                                         */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_PART_INFO                 (L1_Si2164B_Context *api)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[1];
    unsigned char rspByteBuffer[14];
    api->rsp->part_info.STATUS = api->status;

    SiTRACE("Si2164B PART_INFO ");
    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_PART_INFO_CMD;

    if (L0_WriteCommandBytes(api->i2c, 1, cmdByteBuffer) != 1) {
      SiTRACE("Error writing PART_INFO bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 14, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling PART_INFO response\n");
      return error_code;
    }

    api->rsp->part_info.chiprev  = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_PART_INFO_RESPONSE_CHIPREV_LSB   , Si2164B_PART_INFO_RESPONSE_CHIPREV_MASK  );
    api->rsp->part_info.part     = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_PART_INFO_RESPONSE_PART_LSB      , Si2164B_PART_INFO_RESPONSE_PART_MASK     );
    api->rsp->part_info.pmajor   = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_PART_INFO_RESPONSE_PMAJOR_LSB    , Si2164B_PART_INFO_RESPONSE_PMAJOR_MASK   );
    api->rsp->part_info.pminor   = Si2164B_convert_to_byte  (&rspByteBuffer[ 4] , Si2164B_PART_INFO_RESPONSE_PMINOR_LSB    , Si2164B_PART_INFO_RESPONSE_PMINOR_MASK   );
    api->rsp->part_info.pbuild   = Si2164B_convert_to_byte  (&rspByteBuffer[ 5] , Si2164B_PART_INFO_RESPONSE_PBUILD_LSB    , Si2164B_PART_INFO_RESPONSE_PBUILD_MASK   );
    api->rsp->part_info.reserved = Si2164B_convert_to_uint  (&rspByteBuffer[ 6] , Si2164B_PART_INFO_RESPONSE_RESERVED_LSB  , Si2164B_PART_INFO_RESPONSE_RESERVED_MASK );
    api->rsp->part_info.serial   = Si2164B_convert_to_ulong (&rspByteBuffer[ 8] , Si2164B_PART_INFO_RESPONSE_SERIAL_LSB    , Si2164B_PART_INFO_RESPONSE_SERIAL_MASK   );
    api->rsp->part_info.romid    = Si2164B_convert_to_byte  (&rspByteBuffer[12] , Si2164B_PART_INFO_RESPONSE_ROMID_LSB     , Si2164B_PART_INFO_RESPONSE_ROMID_MASK    );
    api->rsp->part_info.rx       = Si2164B_convert_to_byte  (&rspByteBuffer[13] , Si2164B_PART_INFO_RESPONSE_RX_LSB        , Si2164B_PART_INFO_RESPONSE_RX_MASK       );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_PART_INFO_CMD */
#ifdef    Si2164B_POWER_DOWN_CMD
/*---------------------------------------------------*/
/* Si2164B_POWER_DOWN COMMAND                                       */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_POWER_DOWN                (L1_Si2164B_Context *api)
{
    unsigned char cmdByteBuffer[1];
    api->rsp->power_down.STATUS = api->status;

    SiTRACE("Si2164B POWER_DOWN ");
    SiTRACE("\n");

    system_wait(2); /* Make sure that the FW 'main' function has applied any previous settings before going to standby */

    cmdByteBuffer[0] = Si2164B_POWER_DOWN_CMD;

    if (L0_WriteCommandBytes(api->i2c, 1, cmdByteBuffer) != 1) {
      SiTRACE("Error writing POWER_DOWN bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_POWER_DOWN_CMD */
#ifdef    Si2164B_POWER_UP_CMD
/*---------------------------------------------------*/
/* Si2164B_POWER_UP COMMAND                                           */
/*---------------------------------------------------*/
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
                                                   unsigned char   wake_up)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[8];
    unsigned char rspByteBuffer[1];
    api->rsp->power_up.STATUS = api->status;

    SiTRACE("Si2164B POWER_UP ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((subcode    > Si2164B_POWER_UP_CMD_SUBCODE_MAX   )  || (subcode    < Si2164B_POWER_UP_CMD_SUBCODE_MIN   ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("SUBCODE %d "   , subcode    );
    if ((reset      > Si2164B_POWER_UP_CMD_RESET_MAX     )  || (reset      < Si2164B_POWER_UP_CMD_RESET_MIN     ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("RESET %d "     , reset      );
    if ((reserved2  > Si2164B_POWER_UP_CMD_RESERVED2_MAX )  || (reserved2  < Si2164B_POWER_UP_CMD_RESERVED2_MIN ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("RESERVED2 %d " , reserved2  );
    if ((reserved4  > Si2164B_POWER_UP_CMD_RESERVED4_MAX ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("RESERVED4 %d " , reserved4  );
    if ((reserved1  > Si2164B_POWER_UP_CMD_RESERVED1_MAX ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("RESERVED1 %d " , reserved1  );
    if ((addr_mode  > Si2164B_POWER_UP_CMD_ADDR_MODE_MAX ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("ADDR_MODE %d " , addr_mode  );
    if ((reserved5  > Si2164B_POWER_UP_CMD_RESERVED5_MAX )  || (reserved5  < Si2164B_POWER_UP_CMD_RESERVED5_MIN ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("RESERVED5 %d " , reserved5  );
    if ((func       > Si2164B_POWER_UP_CMD_FUNC_MAX      ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("FUNC %d "      , func       );
    if ((clock_freq > Si2164B_POWER_UP_CMD_CLOCK_FREQ_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("CLOCK_FREQ %d ", clock_freq );
    if ((ctsien     > Si2164B_POWER_UP_CMD_CTSIEN_MAX    ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("CTSIEN %d "    , ctsien     );
    if ((wake_up    > Si2164B_POWER_UP_CMD_WAKE_UP_MAX   )  || (wake_up    < Si2164B_POWER_UP_CMD_WAKE_UP_MIN   ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("WAKE_UP %d "   , wake_up    );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_POWER_UP_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( subcode    & Si2164B_POWER_UP_CMD_SUBCODE_MASK    ) << Si2164B_POWER_UP_CMD_SUBCODE_LSB   );
    cmdByteBuffer[2] = (unsigned char) ( ( reset      & Si2164B_POWER_UP_CMD_RESET_MASK      ) << Si2164B_POWER_UP_CMD_RESET_LSB     );
    cmdByteBuffer[3] = (unsigned char) ( ( reserved2  & Si2164B_POWER_UP_CMD_RESERVED2_MASK  ) << Si2164B_POWER_UP_CMD_RESERVED2_LSB );
    cmdByteBuffer[4] = (unsigned char) ( ( reserved4  & Si2164B_POWER_UP_CMD_RESERVED4_MASK  ) << Si2164B_POWER_UP_CMD_RESERVED4_LSB );
    cmdByteBuffer[5] = (unsigned char) ( ( reserved1  & Si2164B_POWER_UP_CMD_RESERVED1_MASK  ) << Si2164B_POWER_UP_CMD_RESERVED1_LSB |
                                         ( addr_mode  & Si2164B_POWER_UP_CMD_ADDR_MODE_MASK  ) << Si2164B_POWER_UP_CMD_ADDR_MODE_LSB |
                                         ( reserved5  & Si2164B_POWER_UP_CMD_RESERVED5_MASK  ) << Si2164B_POWER_UP_CMD_RESERVED5_LSB );
    cmdByteBuffer[6] = (unsigned char) ( ( func       & Si2164B_POWER_UP_CMD_FUNC_MASK       ) << Si2164B_POWER_UP_CMD_FUNC_LSB      |
                                         ( clock_freq & Si2164B_POWER_UP_CMD_CLOCK_FREQ_MASK ) << Si2164B_POWER_UP_CMD_CLOCK_FREQ_LSB|
                                         ( ctsien     & Si2164B_POWER_UP_CMD_CTSIEN_MASK     ) << Si2164B_POWER_UP_CMD_CTSIEN_LSB    );
    cmdByteBuffer[7] = (unsigned char) ( ( wake_up    & Si2164B_POWER_UP_CMD_WAKE_UP_MASK    ) << Si2164B_POWER_UP_CMD_WAKE_UP_LSB   );

    if (L0_WriteCommandBytes(api->i2c, 8, cmdByteBuffer) != 8) {
      SiTRACE("Error writing POWER_UP bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 1, rspByteBuffer);

    system_wait(10); /* We have to wait 10ms after a power up to send a command to the firmware*/

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_POWER_UP_CMD */
#ifdef    Si2164B_RSSI_ADC_CMD
/*---------------------------------------------------*/
/* Si2164B_RSSI_ADC COMMAND                                           */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_RSSI_ADC                  (L1_Si2164B_Context *api,
                                                   unsigned char   on_off)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[2];
    api->rsp->rssi_adc.STATUS = api->status;

    SiTRACE("Si2164B RSSI_ADC ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((on_off > Si2164B_RSSI_ADC_CMD_ON_OFF_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("ON_OFF %d ", on_off );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_RSSI_ADC_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( on_off & Si2164B_RSSI_ADC_CMD_ON_OFF_MASK ) << Si2164B_RSSI_ADC_CMD_ON_OFF_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing RSSI_ADC bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 2, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling RSSI_ADC response\n");
      return error_code;
    }

    api->rsp->rssi_adc.level = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_RSSI_ADC_RESPONSE_LEVEL_LSB  , Si2164B_RSSI_ADC_RESPONSE_LEVEL_MASK );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_RSSI_ADC_CMD */
#ifdef    Si2164B_SCAN_CTRL_CMD
/*---------------------------------------------------*/
/* Si2164B_SCAN_CTRL COMMAND                                         */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_SCAN_CTRL                 (L1_Si2164B_Context *api,
                                                   unsigned char   action,
                                                   unsigned long   tuned_rf_freq)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[8];
    unsigned char rspByteBuffer[1];
    api->rsp->scan_ctrl.STATUS = api->status;

    SiTRACE("Si2164B SCAN_CTRL ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((action        > Si2164B_SCAN_CTRL_CMD_ACTION_MAX       )  || (action        < Si2164B_SCAN_CTRL_CMD_ACTION_MIN       ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("ACTION %d "       , action        );
    SiTRACE("TUNED_RF_FREQ %ld ", tuned_rf_freq );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_SCAN_CTRL_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( action        & Si2164B_SCAN_CTRL_CMD_ACTION_MASK        ) << Si2164B_SCAN_CTRL_CMD_ACTION_LSB       );
    cmdByteBuffer[2] = (unsigned char)0x00;
    cmdByteBuffer[3] = (unsigned char)0x00;
    cmdByteBuffer[4] = (unsigned char) ( ( tuned_rf_freq & Si2164B_SCAN_CTRL_CMD_TUNED_RF_FREQ_MASK ) << Si2164B_SCAN_CTRL_CMD_TUNED_RF_FREQ_LSB);
    cmdByteBuffer[5] = (unsigned char) ((( tuned_rf_freq & Si2164B_SCAN_CTRL_CMD_TUNED_RF_FREQ_MASK ) << Si2164B_SCAN_CTRL_CMD_TUNED_RF_FREQ_LSB)>>8);
    cmdByteBuffer[6] = (unsigned char) ((( tuned_rf_freq & Si2164B_SCAN_CTRL_CMD_TUNED_RF_FREQ_MASK ) << Si2164B_SCAN_CTRL_CMD_TUNED_RF_FREQ_LSB)>>16);
    cmdByteBuffer[7] = (unsigned char) ((( tuned_rf_freq & Si2164B_SCAN_CTRL_CMD_TUNED_RF_FREQ_MASK ) << Si2164B_SCAN_CTRL_CMD_TUNED_RF_FREQ_LSB)>>24);

    if (L0_WriteCommandBytes(api->i2c, 8, cmdByteBuffer) != 8) {
      SiTRACE("Error writing SCAN_CTRL bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 1, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling SCAN_CTRL response\n");
      return error_code;
    }


    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_SCAN_CTRL_CMD */
#ifdef    Si2164B_SCAN_STATUS_CMD
/*---------------------------------------------------*/
/* Si2164B_SCAN_STATUS COMMAND                                     */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_SCAN_STATUS               (L1_Si2164B_Context *api,
                                                   unsigned char   intack)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[2];
    unsigned char rspByteBuffer[11];
    api->rsp->scan_status.STATUS = api->status;

    SiTRACE("Si2164B SCAN_STATUS ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((intack > Si2164B_SCAN_STATUS_CMD_INTACK_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("INTACK %d ", intack );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_SCAN_STATUS_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( intack & Si2164B_SCAN_STATUS_CMD_INTACK_MASK ) << Si2164B_SCAN_STATUS_CMD_INTACK_LSB);

    if (L0_WriteCommandBytes(api->i2c, 2, cmdByteBuffer) != 2) {
      SiTRACE("Error writing SCAN_STATUS bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 11, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling SCAN_STATUS response\n");
      return error_code;
    }

    api->rsp->scan_status.buzint      = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_SCAN_STATUS_RESPONSE_BUZINT_LSB       , Si2164B_SCAN_STATUS_RESPONSE_BUZINT_MASK      );
    api->rsp->scan_status.reqint      = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_SCAN_STATUS_RESPONSE_REQINT_LSB       , Si2164B_SCAN_STATUS_RESPONSE_REQINT_MASK      );
    api->rsp->scan_status.buz         = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_SCAN_STATUS_RESPONSE_BUZ_LSB          , Si2164B_SCAN_STATUS_RESPONSE_BUZ_MASK         );
    api->rsp->scan_status.req         = Si2164B_convert_to_byte  (&rspByteBuffer[ 2] , Si2164B_SCAN_STATUS_RESPONSE_REQ_LSB          , Si2164B_SCAN_STATUS_RESPONSE_REQ_MASK         );
    api->rsp->scan_status.scan_status = Si2164B_convert_to_byte  (&rspByteBuffer[ 3] , Si2164B_SCAN_STATUS_RESPONSE_SCAN_STATUS_LSB  , Si2164B_SCAN_STATUS_RESPONSE_SCAN_STATUS_MASK );
    api->rsp->scan_status.rf_freq     = Si2164B_convert_to_ulong (&rspByteBuffer[ 4] , Si2164B_SCAN_STATUS_RESPONSE_RF_FREQ_LSB      , Si2164B_SCAN_STATUS_RESPONSE_RF_FREQ_MASK     );
    api->rsp->scan_status.symb_rate   = Si2164B_convert_to_uint  (&rspByteBuffer[ 8] , Si2164B_SCAN_STATUS_RESPONSE_SYMB_RATE_LSB    , Si2164B_SCAN_STATUS_RESPONSE_SYMB_RATE_MASK   );
    api->rsp->scan_status.modulation  = Si2164B_convert_to_byte  (&rspByteBuffer[10] , Si2164B_SCAN_STATUS_RESPONSE_MODULATION_LSB   , Si2164B_SCAN_STATUS_RESPONSE_MODULATION_MASK  );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_SCAN_STATUS_CMD */
#ifdef    Si2164B_SET_PROPERTY_CMD
/*---------------------------------------------------*/
/* Si2164B_SET_PROPERTY COMMAND                                   */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_SET_PROPERTY              (L1_Si2164B_Context *api,
                                                   unsigned char   reserved,
                                                   unsigned int    prop,
                                                   unsigned int    data)
{
    unsigned char error_code = 0;
    unsigned char cmdByteBuffer[6];
    unsigned char rspByteBuffer[4];
    api->rsp->set_property.STATUS = api->status;

    SiTRACE("Si2164B SET_PROPERTY ");
  #ifdef   DEBUG_RANGE_CHECK
    SiTRACE("RESERVED %d ", reserved );
    SiTRACE("PROP 0x%04x ", prop     );
    SiTRACE("DATA %d "    , data     );
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_SET_PROPERTY_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( reserved & Si2164B_SET_PROPERTY_CMD_RESERVED_MASK ) << Si2164B_SET_PROPERTY_CMD_RESERVED_LSB);
    cmdByteBuffer[2] = (unsigned char) ( ( prop     & Si2164B_SET_PROPERTY_CMD_PROP_MASK     ) << Si2164B_SET_PROPERTY_CMD_PROP_LSB    );
    cmdByteBuffer[3] = (unsigned char) ((( prop     & Si2164B_SET_PROPERTY_CMD_PROP_MASK     ) << Si2164B_SET_PROPERTY_CMD_PROP_LSB    )>>8);
    cmdByteBuffer[4] = (unsigned char) ( ( data     & Si2164B_SET_PROPERTY_CMD_DATA_MASK     ) << Si2164B_SET_PROPERTY_CMD_DATA_LSB    );
    cmdByteBuffer[5] = (unsigned char) ((( data     & Si2164B_SET_PROPERTY_CMD_DATA_MASK     ) << Si2164B_SET_PROPERTY_CMD_DATA_LSB    )>>8);

    if (L0_WriteCommandBytes(api->i2c, 6, cmdByteBuffer) != 6) {
      SiTRACE("Error writing SET_PROPERTY bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    error_code = Si2164B_pollForResponse(api, 4, rspByteBuffer);
    if (error_code) {
      SiTRACE("Error polling SET_PROPERTY response\n");
      return error_code;
    }

    api->rsp->set_property.reserved = Si2164B_convert_to_byte  (&rspByteBuffer[ 1] , Si2164B_SET_PROPERTY_RESPONSE_RESERVED_LSB  , Si2164B_SET_PROPERTY_RESPONSE_RESERVED_MASK );
    api->rsp->set_property.data     = Si2164B_convert_to_uint  (&rspByteBuffer[ 2] , Si2164B_SET_PROPERTY_RESPONSE_DATA_LSB      , Si2164B_SET_PROPERTY_RESPONSE_DATA_MASK     );

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_SET_PROPERTY_CMD */
#ifdef    Si2164B_SPI_LINK_CMD
/*---------------------------------------------------*/
/* Si2164B_SPI_LINK COMMAND                                           */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_SPI_LINK                  (L1_Si2164B_Context *api,
                                                   unsigned char   subcode,
                                                   unsigned char   spi_pbl_key,
                                                   unsigned char   spi_pbl_num,
                                                   unsigned char   spi_conf_clk,
                                                   unsigned char   spi_clk_pola,
                                                   unsigned char   spi_conf_data,
                                                   unsigned char   spi_data_dir,
                                                   unsigned char   spi_enable)
{
    unsigned char cmdByteBuffer[7];
  #ifdef   DEBUG_RANGE_CHECK
    unsigned char error_code = 0;
  #endif /* DEBUG_RANGE_CHECK */
    api->rsp->spi_link.STATUS = api->status;

    SiTRACE("Si2164B SPI_LINK ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((subcode       > Si2164B_SPI_LINK_CMD_SUBCODE_MAX      )  || (subcode       < Si2164B_SPI_LINK_CMD_SUBCODE_MIN      ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("SUBCODE %d "      , subcode       );
    SiTRACE("SPI_PBL_KEY %d "  , spi_pbl_key   );
    if ((spi_pbl_num   > Si2164B_SPI_LINK_CMD_SPI_PBL_NUM_MAX  ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("SPI_PBL_NUM %d "  , spi_pbl_num   );
    if ((spi_conf_clk  > Si2164B_SPI_LINK_CMD_SPI_CONF_CLK_MAX ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("SPI_CONF_CLK %d " , spi_conf_clk  );
    if ((spi_clk_pola  > Si2164B_SPI_LINK_CMD_SPI_CLK_POLA_MAX ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("SPI_CLK_POLA %d " , spi_clk_pola  );
    if ((spi_conf_data > Si2164B_SPI_LINK_CMD_SPI_CONF_DATA_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("SPI_CONF_DATA %d ", spi_conf_data );
    if ((spi_data_dir  > Si2164B_SPI_LINK_CMD_SPI_DATA_DIR_MAX ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("SPI_DATA_DIR %d " , spi_data_dir  );
    if ((spi_enable    > Si2164B_SPI_LINK_CMD_SPI_ENABLE_MAX   ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("SPI_ENABLE %d "   , spi_enable    );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_SPI_LINK_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( subcode       & Si2164B_SPI_LINK_CMD_SUBCODE_MASK       ) << Si2164B_SPI_LINK_CMD_SUBCODE_LSB      );
    cmdByteBuffer[2] = (unsigned char) ( ( spi_pbl_key   & Si2164B_SPI_LINK_CMD_SPI_PBL_KEY_MASK   ) << Si2164B_SPI_LINK_CMD_SPI_PBL_KEY_LSB  );
    cmdByteBuffer[3] = (unsigned char) ( ( spi_pbl_num   & Si2164B_SPI_LINK_CMD_SPI_PBL_NUM_MASK   ) << Si2164B_SPI_LINK_CMD_SPI_PBL_NUM_LSB  );
    cmdByteBuffer[4] = (unsigned char) ( ( spi_conf_clk  & Si2164B_SPI_LINK_CMD_SPI_CONF_CLK_MASK  ) << Si2164B_SPI_LINK_CMD_SPI_CONF_CLK_LSB |
                                         ( spi_clk_pola  & Si2164B_SPI_LINK_CMD_SPI_CLK_POLA_MASK  ) << Si2164B_SPI_LINK_CMD_SPI_CLK_POLA_LSB );
    cmdByteBuffer[5] = (unsigned char) ( ( spi_conf_data & Si2164B_SPI_LINK_CMD_SPI_CONF_DATA_MASK ) << Si2164B_SPI_LINK_CMD_SPI_CONF_DATA_LSB|
                                         ( spi_data_dir  & Si2164B_SPI_LINK_CMD_SPI_DATA_DIR_MASK  ) << Si2164B_SPI_LINK_CMD_SPI_DATA_DIR_LSB );
    cmdByteBuffer[6] = (unsigned char) ( ( spi_enable    & Si2164B_SPI_LINK_CMD_SPI_ENABLE_MASK    ) << Si2164B_SPI_LINK_CMD_SPI_ENABLE_LSB   );

    if (L0_WriteCommandBytes(api->i2c, 7, cmdByteBuffer) != 7) {
      SiTRACE("Error writing SPI_LINK bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_SPI_LINK_CMD */
#ifdef    Si2164B_SPI_PASSTHROUGH_CMD
/*---------------------------------------------------*/
/* Si2164B_SPI_PASSTHROUGH COMMAND                             */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_SPI_PASSTHROUGH           (L1_Si2164B_Context *api,
                                                   unsigned char   subcode,
                                                   unsigned char   spi_passthr_clk,
                                                   unsigned char   spi_passth_data)
{
    unsigned char cmdByteBuffer[4];
  #ifdef   DEBUG_RANGE_CHECK
    unsigned char error_code = 0;
  #endif /* DEBUG_RANGE_CHECK */
    api->rsp->spi_passthrough.STATUS = api->status;

    SiTRACE("Si2164B SPI_PASSTHROUGH ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((subcode         > Si2164B_SPI_PASSTHROUGH_CMD_SUBCODE_MAX        )  || (subcode         < Si2164B_SPI_PASSTHROUGH_CMD_SUBCODE_MIN        ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("SUBCODE %d "        , subcode         );
    if ((spi_passthr_clk > Si2164B_SPI_PASSTHROUGH_CMD_SPI_PASSTHR_CLK_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("SPI_PASSTHR_CLK %d ", spi_passthr_clk );
    if ((spi_passth_data > Si2164B_SPI_PASSTHROUGH_CMD_SPI_PASSTH_DATA_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("SPI_PASSTH_DATA %d ", spi_passth_data );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_SPI_PASSTHROUGH_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( subcode         & Si2164B_SPI_PASSTHROUGH_CMD_SUBCODE_MASK         ) << Si2164B_SPI_PASSTHROUGH_CMD_SUBCODE_LSB        );
    cmdByteBuffer[2] = (unsigned char) ( ( spi_passthr_clk & Si2164B_SPI_PASSTHROUGH_CMD_SPI_PASSTHR_CLK_MASK ) << Si2164B_SPI_PASSTHROUGH_CMD_SPI_PASSTHR_CLK_LSB);
    cmdByteBuffer[3] = (unsigned char) ( ( spi_passth_data & Si2164B_SPI_PASSTHROUGH_CMD_SPI_PASSTH_DATA_MASK ) << Si2164B_SPI_PASSTHROUGH_CMD_SPI_PASSTH_DATA_LSB);

    if (L0_WriteCommandBytes(api->i2c, 4, cmdByteBuffer) != 4) {
      SiTRACE("Error writing SPI_PASSTHROUGH bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_SPI_PASSTHROUGH_CMD */
#ifdef    Si2164B_START_CLK_CMD
/*---------------------------------------------------*/
/* Si2164B_START_CLK COMMAND                                         */
/*---------------------------------------------------*/
unsigned char Si2164B_L1_START_CLK                 (L1_Si2164B_Context *api,
                                                   unsigned char   subcode,
                                                   unsigned char   reserved1,
                                                   unsigned char   tune_cap,
                                                   unsigned char   reserved2,
                                                   unsigned int    clk_mode,
                                                   unsigned char   reserved3,
                                                   unsigned char   reserved4,
                                                   unsigned char   start_clk)
{
    unsigned char cmdByteBuffer[13];
  #ifdef   DEBUG_RANGE_CHECK
    unsigned char error_code = 0;
  #endif /* DEBUG_RANGE_CHECK */
    api->rsp->start_clk.STATUS = api->status;

    SiTRACE("Si2164B START_CLK ");
  #ifdef   DEBUG_RANGE_CHECK
    if ((subcode   > Si2164B_START_CLK_CMD_SUBCODE_MAX  )  || (subcode   < Si2164B_START_CLK_CMD_SUBCODE_MIN  ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("SUBCODE %d "  , subcode   );
    if ((reserved1 > Si2164B_START_CLK_CMD_RESERVED1_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("RESERVED1 %d ", reserved1 );
    if ((tune_cap  > Si2164B_START_CLK_CMD_TUNE_CAP_MAX ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("TUNE_CAP %d " , tune_cap  );
    if ((reserved2 > Si2164B_START_CLK_CMD_RESERVED2_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("RESERVED2 %d ", reserved2 );
    if ((clk_mode  > Si2164B_START_CLK_CMD_CLK_MODE_MAX )  || (clk_mode  < Si2164B_START_CLK_CMD_CLK_MODE_MIN ) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("CLK_MODE %d " , clk_mode  );
    if ((reserved3 > Si2164B_START_CLK_CMD_RESERVED3_MAX)  || (reserved3 < Si2164B_START_CLK_CMD_RESERVED3_MIN) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("RESERVED3 %d ", reserved3 );
    if ((reserved4 > Si2164B_START_CLK_CMD_RESERVED4_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("RESERVED4 %d ", reserved4 );
    if ((start_clk > Si2164B_START_CLK_CMD_START_CLK_MAX) ) {error_code++; SiTRACE("\nOut of range: ");}; SiTRACE("START_CLK %d ", start_clk );
    if (error_code) {
      SiTRACE("%d out of range parameters\n", error_code);
      return ERROR_Si2164B_PARAMETER_OUT_OF_RANGE;
    }
  #endif /* DEBUG_RANGE_CHECK */

    SiTRACE("\n");
    cmdByteBuffer[0] = Si2164B_START_CLK_CMD;
    cmdByteBuffer[1] = (unsigned char) ( ( subcode   & Si2164B_START_CLK_CMD_SUBCODE_MASK   ) << Si2164B_START_CLK_CMD_SUBCODE_LSB  );
    cmdByteBuffer[2] = (unsigned char) ( ( reserved1 & Si2164B_START_CLK_CMD_RESERVED1_MASK ) << Si2164B_START_CLK_CMD_RESERVED1_LSB);
    cmdByteBuffer[3] = (unsigned char) ( ( tune_cap  & Si2164B_START_CLK_CMD_TUNE_CAP_MASK  ) << Si2164B_START_CLK_CMD_TUNE_CAP_LSB |
                                         ( reserved2 & Si2164B_START_CLK_CMD_RESERVED2_MASK ) << Si2164B_START_CLK_CMD_RESERVED2_LSB);
    cmdByteBuffer[4] = (unsigned char) ( ( clk_mode  & Si2164B_START_CLK_CMD_CLK_MODE_MASK  ) << Si2164B_START_CLK_CMD_CLK_MODE_LSB );
    cmdByteBuffer[5] = (unsigned char) ((( clk_mode  & Si2164B_START_CLK_CMD_CLK_MODE_MASK  ) << Si2164B_START_CLK_CMD_CLK_MODE_LSB )>>8);
    cmdByteBuffer[6] = (unsigned char) ( ( reserved3 & Si2164B_START_CLK_CMD_RESERVED3_MASK ) << Si2164B_START_CLK_CMD_RESERVED3_LSB);
    cmdByteBuffer[7] = (unsigned char) ( ( reserved4 & Si2164B_START_CLK_CMD_RESERVED4_MASK ) << Si2164B_START_CLK_CMD_RESERVED4_LSB);
    cmdByteBuffer[8] = (unsigned char)0x00;
    cmdByteBuffer[9] = (unsigned char)0x00;
    cmdByteBuffer[10] = (unsigned char)0x00;
    cmdByteBuffer[11] = (unsigned char)0x00;
    cmdByteBuffer[12] = (unsigned char) ( ( start_clk & Si2164B_START_CLK_CMD_START_CLK_MASK ) << Si2164B_START_CLK_CMD_START_CLK_LSB);

    if (L0_WriteCommandBytes(api->i2c, 13, cmdByteBuffer) != 13) {
      SiTRACE("Error writing START_CLK bytes!\n");
      return ERROR_Si2164B_SENDING_COMMAND;
    }

    return NO_Si2164B_ERROR;
}
#endif /* Si2164B_START_CLK_CMD */


  /* --------------------------------------------*/
  /* SEND_COMMAND2 FUNCTION                      */
  /* --------------------------------------------*/
unsigned char   Si2164B_L1_SendCommand2(L1_Si2164B_Context *api, unsigned int cmd_code) {

api=api;
cmd_code=cmd_code;

#if 1 //(!SEC_T2_TO_CHECK_01)  //debug
    switch (cmd_code) {
    #ifdef        Si2164B_CONFIG_CLKIO_CMD
     case         Si2164B_CONFIG_CLKIO_CMD_CODE:
       return Si2164B_L1_CONFIG_CLKIO (api, api->cmd->config_clkio.output, api->cmd->config_clkio.pre_driver_str, api->cmd->config_clkio.driver_str );
     break;
    #endif /*     Si2164B_CONFIG_CLKIO_CMD */
    #ifdef        Si2164B_CONFIG_I2C_CMD
     case         Si2164B_CONFIG_I2C_CMD_CODE:
       return Si2164B_L1_CONFIG_I2C (api, api->cmd->config_i2c.subcode, api->cmd->config_i2c.i2c_broadcast );
     break;
    #endif /*     Si2164B_CONFIG_I2C_CMD */
    #ifdef        Si2164B_CONFIG_PINS_CMD
     case         Si2164B_CONFIG_PINS_CMD_CODE:
       return Si2164B_L1_CONFIG_PINS (api, api->cmd->config_pins.gpio0_mode, api->cmd->config_pins.gpio0_read, api->cmd->config_pins.gpio1_mode, api->cmd->config_pins.gpio1_read );
     break;
    #endif /*     Si2164B_CONFIG_PINS_CMD */
    #ifdef        Si2164B_DD_BER_CMD
     case         Si2164B_DD_BER_CMD_CODE:
       return Si2164B_L1_DD_BER (api, api->cmd->dd_ber.rst );
     break;
    #endif /*     Si2164B_DD_BER_CMD */
    #ifdef        Si2164B_DD_CBER_CMD
     case         Si2164B_DD_CBER_CMD_CODE:
       return Si2164B_L1_DD_CBER (api, api->cmd->dd_cber.rst );
     break;
    #endif /*     Si2164B_DD_CBER_CMD */
    #ifdef        Si2164B_DD_DISEQC_SEND_CMD
     case         Si2164B_DD_DISEQC_SEND_CMD_CODE:
       return Si2164B_L1_DD_DISEQC_SEND (api, api->cmd->dd_diseqc_send.enable, api->cmd->dd_diseqc_send.cont_tone, api->cmd->dd_diseqc_send.tone_burst, api->cmd->dd_diseqc_send.burst_sel, api->cmd->dd_diseqc_send.end_seq, api->cmd->dd_diseqc_send.msg_length, api->cmd->dd_diseqc_send.msg_byte1, api->cmd->dd_diseqc_send.msg_byte2, api->cmd->dd_diseqc_send.msg_byte3, api->cmd->dd_diseqc_send.msg_byte4, api->cmd->dd_diseqc_send.msg_byte5, api->cmd->dd_diseqc_send.msg_byte6 );
     break;
    #endif /*     Si2164B_DD_DISEQC_SEND_CMD */
    #ifdef        Si2164B_DD_DISEQC_STATUS_CMD
     case         Si2164B_DD_DISEQC_STATUS_CMD_CODE:
       return Si2164B_L1_DD_DISEQC_STATUS (api, api->cmd->dd_diseqc_status.listen );
     break;
    #endif /*     Si2164B_DD_DISEQC_STATUS_CMD */
    #ifdef        Si2164B_DD_EXT_AGC_SAT_CMD
     case         Si2164B_DD_EXT_AGC_SAT_CMD_CODE:
       return Si2164B_L1_DD_EXT_AGC_SAT (api, api->cmd->dd_ext_agc_sat.agc_1_mode, api->cmd->dd_ext_agc_sat.agc_1_inv, api->cmd->dd_ext_agc_sat.agc_2_mode, api->cmd->dd_ext_agc_sat.agc_2_inv, api->cmd->dd_ext_agc_sat.agc_1_kloop, api->cmd->dd_ext_agc_sat.agc_2_kloop, api->cmd->dd_ext_agc_sat.agc_1_min, api->cmd->dd_ext_agc_sat.agc_2_min );
     break;
    #endif /*     Si2164B_DD_EXT_AGC_SAT_CMD */

    #ifdef        Si2164B_DD_EXT_AGC_TER_CMD
     case         Si2164B_DD_EXT_AGC_TER_CMD_CODE:
       return Si2164B_L1_DD_EXT_AGC_TER (api, api->cmd->dd_ext_agc_ter.agc_1_mode, api->cmd->dd_ext_agc_ter.agc_1_inv, api->cmd->dd_ext_agc_ter.agc_2_mode, api->cmd->dd_ext_agc_ter.agc_2_inv, api->cmd->dd_ext_agc_ter.agc_1_kloop, api->cmd->dd_ext_agc_ter.agc_2_kloop, api->cmd->dd_ext_agc_ter.agc_1_min, api->cmd->dd_ext_agc_ter.agc_2_min );
     break;
    #endif /*     Si2164B_DD_EXT_AGC_TER_CMD */

    #ifdef        Si2164B_DD_FER_CMD
     case         Si2164B_DD_FER_CMD_CODE:
       return Si2164B_L1_DD_FER (api, api->cmd->dd_fer.rst );
     break;
    #endif /*     Si2164B_DD_FER_CMD */
    #ifdef        Si2164B_DD_GET_REG_CMD
     case         Si2164B_DD_GET_REG_CMD_CODE:
       return Si2164B_L1_DD_GET_REG (api, api->cmd->dd_get_reg.reg_code_lsb, api->cmd->dd_get_reg.reg_code_mid, api->cmd->dd_get_reg.reg_code_msb );
     break;
    #endif /*     Si2164B_DD_GET_REG_CMD */
    #ifdef        Si2164B_DD_MP_DEFAULTS_CMD
     case         Si2164B_DD_MP_DEFAULTS_CMD_CODE:
       return Si2164B_L1_DD_MP_DEFAULTS (api, api->cmd->dd_mp_defaults.mp_a_mode, api->cmd->dd_mp_defaults.mp_b_mode, api->cmd->dd_mp_defaults.mp_c_mode, api->cmd->dd_mp_defaults.mp_d_mode );
     break;
    #endif /*     Si2164B_DD_MP_DEFAULTS_CMD */
    #ifdef        Si2164B_DD_PER_CMD
     case         Si2164B_DD_PER_CMD_CODE:
       return Si2164B_L1_DD_PER (api, api->cmd->dd_per.rst );
     break;
    #endif /*     Si2164B_DD_PER_CMD */
    #ifdef        Si2164B_DD_RESTART_CMD
     case         Si2164B_DD_RESTART_CMD_CODE:
       return Si2164B_L1_DD_RESTART (api );
     break;
    #endif /*     Si2164B_DD_RESTART_CMD */
    #ifdef        Si2164B_DD_RESTART_EXT_CMD
     case         Si2164B_DD_RESTART_EXT_CMD_CODE:
       return Si2164B_L1_DD_RESTART_EXT (api, api->cmd->dd_restart_ext.freq_plan, api->cmd->dd_restart_ext.freq_plan_ts_clk, api->cmd->dd_restart_ext.tuned_rf_freq );
     break;
    #endif /*     Si2164B_DD_RESTART_EXT_CMD */
    #ifdef        Si2164B_DD_SET_REG_CMD
     case         Si2164B_DD_SET_REG_CMD_CODE:
       return Si2164B_L1_DD_SET_REG (api, api->cmd->dd_set_reg.reg_code_lsb, api->cmd->dd_set_reg.reg_code_mid, api->cmd->dd_set_reg.reg_code_msb, api->cmd->dd_set_reg.value );
     break;
    #endif /*     Si2164B_DD_SET_REG_CMD */
    #ifdef        Si2164B_DD_SSI_SQI_CMD
     case         Si2164B_DD_SSI_SQI_CMD_CODE:
       return Si2164B_L1_DD_SSI_SQI (api, api->cmd->dd_ssi_sqi.tuner_rssi );
     break;
    #endif /*     Si2164B_DD_SSI_SQI_CMD */
    #ifdef        Si2164B_DD_STATUS_CMD
     case         Si2164B_DD_STATUS_CMD_CODE:
       return Si2164B_L1_DD_STATUS (api, api->cmd->dd_status.intack );
     break;
    #endif /*     Si2164B_DD_STATUS_CMD */
    #ifdef        Si2164B_DD_TS_PINS_CMD
     case         Si2164B_DD_TS_PINS_CMD_CODE:
       return Si2164B_L1_DD_TS_PINS (api, api->cmd->dd_ts_pins.primary_ts_mode, api->cmd->dd_ts_pins.primary_ts_activity, api->cmd->dd_ts_pins.secondary_ts_mode, api->cmd->dd_ts_pins.secondary_ts_activity );
     break;
    #endif /*     Si2164B_DD_TS_PINS_CMD */
    #ifdef        Si2164B_DD_UNCOR_CMD
     case         Si2164B_DD_UNCOR_CMD_CODE:
       return Si2164B_L1_DD_UNCOR (api, api->cmd->dd_uncor.rst );
     break;
    #endif /*     Si2164B_DD_UNCOR_CMD */
    #ifdef        Si2164B_DEMOD_INFO_CMD
     case         Si2164B_DEMOD_INFO_CMD_CODE:
       return Si2164B_L1_DEMOD_INFO (api );
     break;
    #endif /*     Si2164B_DEMOD_INFO_CMD */
    #ifdef        Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD
     case         Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD_CODE:
       return Si2164B_L1_DOWNLOAD_DATASET_CONTINUE (api, api->cmd->download_dataset_continue.data0, api->cmd->download_dataset_continue.data1, api->cmd->download_dataset_continue.data2, api->cmd->download_dataset_continue.data3, api->cmd->download_dataset_continue.data4, api->cmd->download_dataset_continue.data5, api->cmd->download_dataset_continue.data6 );
     break;
    #endif /*     Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD */
    #ifdef        Si2164B_DOWNLOAD_DATASET_START_CMD
     case         Si2164B_DOWNLOAD_DATASET_START_CMD_CODE:
       return Si2164B_L1_DOWNLOAD_DATASET_START (api, api->cmd->download_dataset_start.dataset_id, api->cmd->download_dataset_start.dataset_checksum, api->cmd->download_dataset_start.data0, api->cmd->download_dataset_start.data1, api->cmd->download_dataset_start.data2, api->cmd->download_dataset_start.data3, api->cmd->download_dataset_start.data4 );
     break;
    #endif /*     Si2164B_DOWNLOAD_DATASET_START_CMD */
    #ifdef        Si2164B_DVBC2_CTRL_CMD
     case         Si2164B_DVBC2_CTRL_CMD_CODE:
       return Si2164B_L1_DVBC2_CTRL (api, api->cmd->dvbc2_ctrl.action, api->cmd->dvbc2_ctrl.tuned_rf_freq );
     break;
    #endif /*     Si2164B_DVBC2_CTRL_CMD */
    #ifdef        Si2164B_DVBC2_DS_INFO_CMD
     case         Si2164B_DVBC2_DS_INFO_CMD_CODE:
       return Si2164B_L1_DVBC2_DS_INFO (api, api->cmd->dvbc2_ds_info.ds_index_or_id, api->cmd->dvbc2_ds_info.ds_select_index_or_id );
     break;
    #endif /*     Si2164B_DVBC2_DS_INFO_CMD */
    #ifdef        Si2164B_DVBC2_DS_PLP_SELECT_CMD
     case         Si2164B_DVBC2_DS_PLP_SELECT_CMD_CODE:
       return Si2164B_L1_DVBC2_DS_PLP_SELECT (api, api->cmd->dvbc2_ds_plp_select.plp_id, api->cmd->dvbc2_ds_plp_select.id_sel_mode, api->cmd->dvbc2_ds_plp_select.ds_id );
     break;
    #endif /*     Si2164B_DVBC2_DS_PLP_SELECT_CMD */
    #ifdef        Si2164B_DVBC2_PLP_INFO_CMD
     case         Si2164B_DVBC2_PLP_INFO_CMD_CODE:
       return Si2164B_L1_DVBC2_PLP_INFO (api, api->cmd->dvbc2_plp_info.plp_index, api->cmd->dvbc2_plp_info.plp_info_ds_mode, api->cmd->dvbc2_plp_info.ds_index );
     break;
    #endif /*     Si2164B_DVBC2_PLP_INFO_CMD */
    #ifdef        Si2164B_DVBC2_STATUS_CMD
     case         Si2164B_DVBC2_STATUS_CMD_CODE:
       return Si2164B_L1_DVBC2_STATUS (api, api->cmd->dvbc2_status.intack );
     break;
    #endif /*     Si2164B_DVBC2_STATUS_CMD */
    #ifdef        Si2164B_DVBC2_SYS_INFO_CMD
     case         Si2164B_DVBC2_SYS_INFO_CMD_CODE:
       return Si2164B_L1_DVBC2_SYS_INFO (api );
     break;
    #endif /*     Si2164B_DVBC2_SYS_INFO_CMD */

    #ifdef        Si2164B_DVBC_STATUS_CMD
     case         Si2164B_DVBC_STATUS_CMD_CODE:
       return Si2164B_L1_DVBC_STATUS (api, api->cmd->dvbc_status.intack );
     break;
    #endif /*     Si2164B_DVBC_STATUS_CMD */

    #ifdef        Si2164B_DVBS2_STATUS_CMD
     case         Si2164B_DVBS2_STATUS_CMD_CODE:
       return Si2164B_L1_DVBS2_STATUS (api, api->cmd->dvbs2_status.intack );
     break;
    #endif /*     Si2164B_DVBS2_STATUS_CMD */
    #ifdef        Si2164B_DVBS2_STREAM_INFO_CMD
     case         Si2164B_DVBS2_STREAM_INFO_CMD_CODE:
       return Si2164B_L1_DVBS2_STREAM_INFO (api, api->cmd->dvbs2_stream_info.isi_index );
     break;
    #endif /*     Si2164B_DVBS2_STREAM_INFO_CMD */
    #ifdef        Si2164B_DVBS2_STREAM_SELECT_CMD
     case         Si2164B_DVBS2_STREAM_SELECT_CMD_CODE:
       return Si2164B_L1_DVBS2_STREAM_SELECT (api, api->cmd->dvbs2_stream_select.stream_id, api->cmd->dvbs2_stream_select.stream_sel_mode );
     break;
    #endif /*     Si2164B_DVBS2_STREAM_SELECT_CMD */
    #ifdef        Si2164B_DVBS_STATUS_CMD
     case         Si2164B_DVBS_STATUS_CMD_CODE:
       return Si2164B_L1_DVBS_STATUS (api, api->cmd->dvbs_status.intack );
     break;
    #endif /*     Si2164B_DVBS_STATUS_CMD */

    #ifdef        Si2164B_DVBT2_FEF_CMD
     case         Si2164B_DVBT2_FEF_CMD_CODE:
       return Si2164B_L1_DVBT2_FEF (api, api->cmd->dvbt2_fef.fef_tuner_flag, api->cmd->dvbt2_fef.fef_tuner_flag_inv );
     break;
    #endif /*     Si2164B_DVBT2_FEF_CMD */
    #ifdef        Si2164B_DVBT2_PLP_INFO_CMD
     case         Si2164B_DVBT2_PLP_INFO_CMD_CODE:
       return Si2164B_L1_DVBT2_PLP_INFO (api, api->cmd->dvbt2_plp_info.plp_index );
     break;
    #endif /*     Si2164B_DVBT2_PLP_INFO_CMD */
    #ifdef        Si2164B_DVBT2_PLP_SELECT_CMD
     case         Si2164B_DVBT2_PLP_SELECT_CMD_CODE:
       return Si2164B_L1_DVBT2_PLP_SELECT (api, api->cmd->dvbt2_plp_select.plp_id, api->cmd->dvbt2_plp_select.plp_id_sel_mode );
     break;
    #endif /*     Si2164B_DVBT2_PLP_SELECT_CMD */
    #ifdef        Si2164B_DVBT2_STATUS_CMD
     case         Si2164B_DVBT2_STATUS_CMD_CODE:
       return Si2164B_L1_DVBT2_STATUS (api, api->cmd->dvbt2_status.intack );
     break;
    #endif /*     Si2164B_DVBT2_STATUS_CMD */
    #ifdef        Si2164B_DVBT2_TX_ID_CMD
     case         Si2164B_DVBT2_TX_ID_CMD_CODE:
       return Si2164B_L1_DVBT2_TX_ID (api );
     break;
    #endif /*     Si2164B_DVBT2_TX_ID_CMD */

    #ifdef        Si2164B_DVBT_STATUS_CMD
     case         Si2164B_DVBT_STATUS_CMD_CODE:
       return Si2164B_L1_DVBT_STATUS (api, api->cmd->dvbt_status.intack );
     break;
    #endif /*     Si2164B_DVBT_STATUS_CMD */
    #ifdef        Si2164B_DVBT_TPS_EXTRA_CMD
     case         Si2164B_DVBT_TPS_EXTRA_CMD_CODE:
       return Si2164B_L1_DVBT_TPS_EXTRA (api );
     break;
    #endif /*     Si2164B_DVBT_TPS_EXTRA_CMD */

    #ifdef        Si2164B_EXIT_BOOTLOADER_CMD
     case         Si2164B_EXIT_BOOTLOADER_CMD_CODE:
       return Si2164B_L1_EXIT_BOOTLOADER (api, api->cmd->exit_bootloader.func, api->cmd->exit_bootloader.ctsien );
     break;
    #endif /*     Si2164B_EXIT_BOOTLOADER_CMD */
    #ifdef        Si2164B_GET_PROPERTY_CMD
     case         Si2164B_GET_PROPERTY_CMD_CODE:
       return Si2164B_L1_GET_PROPERTY (api, api->cmd->get_property.reserved, api->cmd->get_property.prop );
     break;
    #endif /*     Si2164B_GET_PROPERTY_CMD */
    #ifdef        Si2164B_GET_REV_CMD
     case         Si2164B_GET_REV_CMD_CODE:
       return Si2164B_L1_GET_REV (api );
     break;
    #endif /*     Si2164B_GET_REV_CMD */
    #ifdef        Si2164B_I2C_PASSTHROUGH_CMD
     case         Si2164B_I2C_PASSTHROUGH_CMD_CODE:
       return Si2164B_L1_I2C_PASSTHROUGH (api, api->cmd->i2c_passthrough.subcode, api->cmd->i2c_passthrough.i2c_passthru, api->cmd->i2c_passthrough.reserved );
     break;
    #endif /*     Si2164B_I2C_PASSTHROUGH_CMD */

    #ifdef        Si2164B_MCNS_STATUS_CMD
     case         Si2164B_MCNS_STATUS_CMD_CODE:
       return Si2164B_L1_MCNS_STATUS (api, api->cmd->mcns_status.intack );
     break;
    #endif /*     Si2164B_MCNS_STATUS_CMD */

    #ifdef        Si2164B_PART_INFO_CMD
     case         Si2164B_PART_INFO_CMD_CODE:
       return Si2164B_L1_PART_INFO (api );
     break;
    #endif /*     Si2164B_PART_INFO_CMD */
    #ifdef        Si2164B_POWER_DOWN_CMD
     case         Si2164B_POWER_DOWN_CMD_CODE:
       return Si2164B_L1_POWER_DOWN (api );
     break;
    #endif /*     Si2164B_POWER_DOWN_CMD */
    #ifdef        Si2164B_POWER_UP_CMD
     case         Si2164B_POWER_UP_CMD_CODE:
       return Si2164B_L1_POWER_UP (api, api->cmd->power_up.subcode, api->cmd->power_up.reset, api->cmd->power_up.reserved2, api->cmd->power_up.reserved4, api->cmd->power_up.reserved1, api->cmd->power_up.addr_mode, api->cmd->power_up.reserved5, api->cmd->power_up.func, api->cmd->power_up.clock_freq, api->cmd->power_up.ctsien, api->cmd->power_up.wake_up );
     break;
    #endif /*     Si2164B_POWER_UP_CMD */
    #ifdef        Si2164B_RSSI_ADC_CMD
     case         Si2164B_RSSI_ADC_CMD_CODE:
       return Si2164B_L1_RSSI_ADC (api, api->cmd->rssi_adc.on_off );
     break;
    #endif /*     Si2164B_RSSI_ADC_CMD */
    #ifdef        Si2164B_SCAN_CTRL_CMD
     case         Si2164B_SCAN_CTRL_CMD_CODE:
       return Si2164B_L1_SCAN_CTRL (api, api->cmd->scan_ctrl.action, api->cmd->scan_ctrl.tuned_rf_freq );
     break;
    #endif /*     Si2164B_SCAN_CTRL_CMD */
    #ifdef        Si2164B_SCAN_STATUS_CMD
     case         Si2164B_SCAN_STATUS_CMD_CODE:
       return Si2164B_L1_SCAN_STATUS (api, api->cmd->scan_status.intack );
     break;
    #endif /*     Si2164B_SCAN_STATUS_CMD */
    #ifdef        Si2164B_SET_PROPERTY_CMD
     case         Si2164B_SET_PROPERTY_CMD_CODE:
       return Si2164B_L1_SET_PROPERTY (api, api->cmd->set_property.reserved, api->cmd->set_property.prop, api->cmd->set_property.data );
     break;
    #endif /*     Si2164B_SET_PROPERTY_CMD */
    #ifdef        Si2164B_SPI_LINK_CMD
     case         Si2164B_SPI_LINK_CMD_CODE:
       return Si2164B_L1_SPI_LINK (api, api->cmd->spi_link.subcode, api->cmd->spi_link.spi_pbl_key, api->cmd->spi_link.spi_pbl_num, api->cmd->spi_link.spi_conf_clk, api->cmd->spi_link.spi_clk_pola, api->cmd->spi_link.spi_conf_data, api->cmd->spi_link.spi_data_dir, api->cmd->spi_link.spi_enable );
     break;
    #endif /*     Si2164B_SPI_LINK_CMD */
    #ifdef        Si2164B_SPI_PASSTHROUGH_CMD
     case         Si2164B_SPI_PASSTHROUGH_CMD_CODE:
       return Si2164B_L1_SPI_PASSTHROUGH (api, api->cmd->spi_passthrough.subcode, api->cmd->spi_passthrough.spi_passthr_clk, api->cmd->spi_passthrough.spi_passth_data );
     break;
    #endif /*     Si2164B_SPI_PASSTHROUGH_CMD */
    #ifdef        Si2164B_START_CLK_CMD
     case         Si2164B_START_CLK_CMD_CODE:
       return Si2164B_L1_START_CLK (api, api->cmd->start_clk.subcode, api->cmd->start_clk.reserved1, api->cmd->start_clk.tune_cap, api->cmd->start_clk.reserved2, api->cmd->start_clk.clk_mode, api->cmd->start_clk.reserved3, api->cmd->start_clk.reserved4, api->cmd->start_clk.start_clk );
     break;
    #endif /*     Si2164B_START_CLK_CMD */
   default : break;
    }
#endif    
     return 0;
  }

#ifdef    Si2164B_GET_COMMAND_STRINGS

  /* --------------------------------------------*/
  /* GET_COMMAND_RESPONSE_STRING FUNCTION        */
  /* --------------------------------------------*/
unsigned char   Si2164B_L1_GetCommandResponseString(L1_Si2164B_Context *api, unsigned int cmd_code, const unsigned char *separator, unsigned char *msg) {

    api=api;
    cmd_code=cmd_code;
    separator=separator;
    msg=msg;


    
#if (!SEC_T2_TO_CHECK_01)
    switch (cmd_code) {
    #ifdef        Si2164B_CONFIG_CLKIO_CMD
     case         Si2164B_CONFIG_CLKIO_CMD_CODE:
      safe_sprintf(msg,"CONFIG_CLKIO ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT          ");
           if  (api->rsp->config_clkio.STATUS->ddint          ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->config_clkio.STATUS->ddint          ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                             safe_sprintf(msg,"%s%d", msg, api->rsp->config_clkio.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT        ");
           if  (api->rsp->config_clkio.STATUS->scanint        ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->config_clkio.STATUS->scanint        ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                             safe_sprintf(msg,"%s%d", msg, api->rsp->config_clkio.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR            ");
           if  (api->rsp->config_clkio.STATUS->err            ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->config_clkio.STATUS->err            ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                             safe_sprintf(msg,"%s%d", msg, api->rsp->config_clkio.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS            ");
           if  (api->rsp->config_clkio.STATUS->cts            ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->config_clkio.STATUS->cts            ==     0) safe_strcat(msg,"WAIT     ");
      else                                                             safe_sprintf(msg,"%s%d", msg, api->rsp->config_clkio.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-MODE           ");
           if  (api->rsp->config_clkio.mode           ==     2) safe_strcat(msg,"CLK_INPUT ");
      else if  (api->rsp->config_clkio.mode           ==     1) safe_strcat(msg,"CLK_OUTPUT");
      else if  (api->rsp->config_clkio.mode           ==     0) safe_strcat(msg,"UNUSED    ");
      else                                                     safe_sprintf(msg,"%s%d", msg, api->rsp->config_clkio.mode);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PRE_DRIVER_STR "); safe_sprintf(msg,"%s%d", msg, api->rsp->config_clkio.pre_driver_str);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DRIVER_STR     "); safe_sprintf(msg,"%s%d", msg, api->rsp->config_clkio.driver_str);
     break;
    #endif /*     Si2164B_CONFIG_CLKIO_CMD */

    #ifdef        Si2164B_CONFIG_I2C_CMD
     case         Si2164B_CONFIG_I2C_CMD_CODE:
      safe_sprintf(msg,"CONFIG_I2C ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->config_i2c.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->config_i2c.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->config_i2c.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->config_i2c.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->config_i2c.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->config_i2c.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->config_i2c.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->config_i2c.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->config_i2c.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->config_i2c.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->config_i2c.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->config_i2c.STATUS->cts);
     break;
    #endif /*     Si2164B_CONFIG_I2C_CMD */

    #ifdef        Si2164B_CONFIG_PINS_CMD
     case         Si2164B_CONFIG_PINS_CMD_CODE:
      safe_sprintf(msg,"CONFIG_PINS ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT       ");
           if  (api->rsp->config_pins.STATUS->ddint       ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->config_pins.STATUS->ddint       ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->config_pins.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT     ");
           if  (api->rsp->config_pins.STATUS->scanint     ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->config_pins.STATUS->scanint     ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->config_pins.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR         ");
           if  (api->rsp->config_pins.STATUS->err         ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->config_pins.STATUS->err         ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->config_pins.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS         ");
           if  (api->rsp->config_pins.STATUS->cts         ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->config_pins.STATUS->cts         ==     0) safe_strcat(msg,"WAIT     ");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->config_pins.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-GPIO0_MODE  ");
           if  (api->rsp->config_pins.gpio0_mode  ==     1) safe_strcat(msg,"DISABLE ");
      else if  (api->rsp->config_pins.gpio0_mode  ==     2) safe_strcat(msg,"DRIVE_0 ");
      else if  (api->rsp->config_pins.gpio0_mode  ==     3) safe_strcat(msg,"DRIVE_1 ");
      else if  (api->rsp->config_pins.gpio0_mode  ==     5) safe_strcat(msg,"FEF     ");
      else if  (api->rsp->config_pins.gpio0_mode  ==     8) safe_strcat(msg,"HW_LOCK ");
      else if  (api->rsp->config_pins.gpio0_mode  ==     7) safe_strcat(msg,"INT_FLAG");
      else if  (api->rsp->config_pins.gpio0_mode  ==     4) safe_strcat(msg,"TS_ERR  ");
      else                                                 safe_sprintf(msg,"%s%d", msg, api->rsp->config_pins.gpio0_mode);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-GPIO0_STATE ");
           if  (api->rsp->config_pins.gpio0_state ==     0) safe_strcat(msg,"READ_0");
      else if  (api->rsp->config_pins.gpio0_state ==     1) safe_strcat(msg,"READ_1");
      else                                                 safe_sprintf(msg,"%s%d", msg, api->rsp->config_pins.gpio0_state);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-GPIO1_MODE  ");
           if  (api->rsp->config_pins.gpio1_mode  ==     1) safe_strcat(msg,"DISABLE ");
      else if  (api->rsp->config_pins.gpio1_mode  ==     2) safe_strcat(msg,"DRIVE_0 ");
      else if  (api->rsp->config_pins.gpio1_mode  ==     3) safe_strcat(msg,"DRIVE_1 ");
      else if  (api->rsp->config_pins.gpio1_mode  ==     5) safe_strcat(msg,"FEF     ");
      else if  (api->rsp->config_pins.gpio1_mode  ==     8) safe_strcat(msg,"HW_LOCK ");
      else if  (api->rsp->config_pins.gpio1_mode  ==     7) safe_strcat(msg,"INT_FLAG");
      else if  (api->rsp->config_pins.gpio1_mode  ==     4) safe_strcat(msg,"TS_ERR  ");
      else                                                 safe_sprintf(msg,"%s%d", msg, api->rsp->config_pins.gpio1_mode);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-GPIO1_STATE ");
           if  (api->rsp->config_pins.gpio1_state ==     0) safe_strcat(msg,"READ_0");
      else if  (api->rsp->config_pins.gpio1_state ==     1) safe_strcat(msg,"READ_1");
      else                                                 safe_sprintf(msg,"%s%d", msg, api->rsp->config_pins.gpio1_state);
     break;
    #endif /*     Si2164B_CONFIG_PINS_CMD */

    #ifdef        Si2164B_DD_BER_CMD
     case         Si2164B_DD_BER_CMD_CODE:
      safe_sprintf(msg,"DD_BER ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->dd_ber.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_ber.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ber.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->dd_ber.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_ber.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ber.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->dd_ber.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dd_ber.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ber.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->dd_ber.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dd_ber.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ber.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-EXP     "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ber.exp);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-MANT    "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ber.mant);
     break;
    #endif /*     Si2164B_DD_BER_CMD */

    #ifdef        Si2164B_DD_CBER_CMD
     case         Si2164B_DD_CBER_CMD_CODE:
      safe_sprintf(msg,"DD_CBER ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->dd_cber.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_cber.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                 safe_sprintf(msg,"%s%d", msg, api->rsp->dd_cber.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->dd_cber.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_cber.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                 safe_sprintf(msg,"%s%d", msg, api->rsp->dd_cber.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->dd_cber.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dd_cber.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                 safe_sprintf(msg,"%s%d", msg, api->rsp->dd_cber.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->dd_cber.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dd_cber.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                 safe_sprintf(msg,"%s%d", msg, api->rsp->dd_cber.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-EXP     "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_cber.exp);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-MANT    "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_cber.mant);
     break;
    #endif /*     Si2164B_DD_CBER_CMD */

    #ifdef        Si2164B_DD_DISEQC_SEND_CMD
     case         Si2164B_DD_DISEQC_SEND_CMD_CODE:
      safe_sprintf(msg,"DD_DISEQC_SEND ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->dd_diseqc_send.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_diseqc_send.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                        safe_sprintf(msg,"%s%d", msg, api->rsp->dd_diseqc_send.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->dd_diseqc_send.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_diseqc_send.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                        safe_sprintf(msg,"%s%d", msg, api->rsp->dd_diseqc_send.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->dd_diseqc_send.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dd_diseqc_send.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                        safe_sprintf(msg,"%s%d", msg, api->rsp->dd_diseqc_send.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->dd_diseqc_send.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dd_diseqc_send.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                        safe_sprintf(msg,"%s%d", msg, api->rsp->dd_diseqc_send.STATUS->cts);
     break;
    #endif /*     Si2164B_DD_DISEQC_SEND_CMD */

    #ifdef        Si2164B_DD_DISEQC_STATUS_CMD
     case         Si2164B_DD_DISEQC_STATUS_CMD_CODE:
      safe_sprintf(msg,"DD_DISEQC_STATUS ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT        ");
           if  (api->rsp->dd_diseqc_status.STATUS->ddint        ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_diseqc_status.STATUS->ddint        ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                               safe_sprintf(msg,"%s%d", msg, api->rsp->dd_diseqc_status.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT      ");
           if  (api->rsp->dd_diseqc_status.STATUS->scanint      ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_diseqc_status.STATUS->scanint      ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                               safe_sprintf(msg,"%s%d", msg, api->rsp->dd_diseqc_status.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR          ");
           if  (api->rsp->dd_diseqc_status.STATUS->err          ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dd_diseqc_status.STATUS->err          ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                               safe_sprintf(msg,"%s%d", msg, api->rsp->dd_diseqc_status.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS          ");
           if  (api->rsp->dd_diseqc_status.STATUS->cts          ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dd_diseqc_status.STATUS->cts          ==     0) safe_strcat(msg,"WAIT     ");
      else                                                               safe_sprintf(msg,"%s%d", msg, api->rsp->dd_diseqc_status.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BUS_STATE    ");
           if  (api->rsp->dd_diseqc_status.bus_state    ==     0) safe_strcat(msg,"BUSY ");
      else if  (api->rsp->dd_diseqc_status.bus_state    ==     1) safe_strcat(msg,"READY");
      else                                                       safe_sprintf(msg,"%s%d", msg, api->rsp->dd_diseqc_status.bus_state);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-REPLY_STATUS ");
           if  (api->rsp->dd_diseqc_status.reply_status ==     2) safe_strcat(msg,"MISSING_BITS");
      else if  (api->rsp->dd_diseqc_status.reply_status ==     0) safe_strcat(msg,"NO_REPLY    ");
      else if  (api->rsp->dd_diseqc_status.reply_status ==     1) safe_strcat(msg,"PARITY_ERROR");
      else if  (api->rsp->dd_diseqc_status.reply_status ==     3) safe_strcat(msg,"REPLY_OK    ");
      else                                                       safe_sprintf(msg,"%s%d", msg, api->rsp->dd_diseqc_status.reply_status);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-REPLY_LENGTH "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_diseqc_status.reply_length);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-REPLY_TOGGLE "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_diseqc_status.reply_toggle);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-END_OF_REPLY "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_diseqc_status.end_of_reply);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DISEQC_MODE  ");
           if  (api->rsp->dd_diseqc_status.diseqc_mode  ==     0) safe_strcat(msg,"AUTO  ");
      else if  (api->rsp->dd_diseqc_status.diseqc_mode  ==     1) safe_strcat(msg,"LISTEN");
      else                                                       safe_sprintf(msg,"%s%d", msg, api->rsp->dd_diseqc_status.diseqc_mode);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-REPLY_BYTE1  "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_diseqc_status.reply_byte1);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-REPLY_BYTE2  "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_diseqc_status.reply_byte2);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-REPLY_BYTE3  "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_diseqc_status.reply_byte3);
     break;
    #endif /*     Si2164B_DD_DISEQC_STATUS_CMD */

    #ifdef        Si2164B_DD_EXT_AGC_SAT_CMD
     case         Si2164B_DD_EXT_AGC_SAT_CMD_CODE:
      safe_sprintf(msg,"DD_EXT_AGC_SAT ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT       ");
           if  (api->rsp->dd_ext_agc_sat.STATUS->ddint       ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_ext_agc_sat.STATUS->ddint       ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ext_agc_sat.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT     ");
           if  (api->rsp->dd_ext_agc_sat.STATUS->scanint     ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_ext_agc_sat.STATUS->scanint     ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ext_agc_sat.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR         ");
           if  (api->rsp->dd_ext_agc_sat.STATUS->err         ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dd_ext_agc_sat.STATUS->err         ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ext_agc_sat.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS         ");
           if  (api->rsp->dd_ext_agc_sat.STATUS->cts         ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dd_ext_agc_sat.STATUS->cts         ==     0) safe_strcat(msg,"WAIT     ");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ext_agc_sat.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-AGC_1_LEVEL "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ext_agc_sat.agc_1_level);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-AGC_2_LEVEL "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ext_agc_sat.agc_2_level);
     break;
    #endif /*     Si2164B_DD_EXT_AGC_SAT_CMD */


    #ifdef        Si2164B_DD_EXT_AGC_TER_CMD
     case         Si2164B_DD_EXT_AGC_TER_CMD_CODE:
      safe_sprintf(msg,"DD_EXT_AGC_TER ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT       ");
           if  (api->rsp->dd_ext_agc_ter.STATUS->ddint       ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_ext_agc_ter.STATUS->ddint       ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ext_agc_ter.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT     ");
           if  (api->rsp->dd_ext_agc_ter.STATUS->scanint     ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_ext_agc_ter.STATUS->scanint     ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ext_agc_ter.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR         ");
           if  (api->rsp->dd_ext_agc_ter.STATUS->err         ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dd_ext_agc_ter.STATUS->err         ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ext_agc_ter.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS         ");
           if  (api->rsp->dd_ext_agc_ter.STATUS->cts         ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dd_ext_agc_ter.STATUS->cts         ==     0) safe_strcat(msg,"WAIT     ");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ext_agc_ter.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-AGC_1_LEVEL "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ext_agc_ter.agc_1_level);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-AGC_2_LEVEL "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ext_agc_ter.agc_2_level);
     break;
    #endif /*     Si2164B_DD_EXT_AGC_TER_CMD */


    #ifdef        Si2164B_DD_FER_CMD
     case         Si2164B_DD_FER_CMD_CODE:
      safe_sprintf(msg,"DD_FER ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->dd_fer.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_fer.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_fer.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->dd_fer.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_fer.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_fer.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->dd_fer.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dd_fer.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_fer.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->dd_fer.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dd_fer.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_fer.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-EXP     "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_fer.exp);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-MANT    "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_fer.mant);
     break;
    #endif /*     Si2164B_DD_FER_CMD */

    #ifdef        Si2164B_DD_GET_REG_CMD
     case         Si2164B_DD_GET_REG_CMD_CODE:
      safe_sprintf(msg,"DD_GET_REG ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->dd_get_reg.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_get_reg.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_get_reg.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->dd_get_reg.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_get_reg.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_get_reg.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->dd_get_reg.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dd_get_reg.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_get_reg.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->dd_get_reg.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dd_get_reg.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_get_reg.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DATA1   "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_get_reg.data1);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DATA2   "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_get_reg.data2);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DATA3   "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_get_reg.data3);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DATA4   "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_get_reg.data4);
     break;
    #endif /*     Si2164B_DD_GET_REG_CMD */

    #ifdef        Si2164B_DD_MP_DEFAULTS_CMD
     case         Si2164B_DD_MP_DEFAULTS_CMD_CODE:
      safe_sprintf(msg,"DD_MP_DEFAULTS ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT     ");
           if  (api->rsp->dd_mp_defaults.STATUS->ddint     ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_mp_defaults.STATUS->ddint     ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                          safe_sprintf(msg,"%s%d", msg, api->rsp->dd_mp_defaults.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT   ");
           if  (api->rsp->dd_mp_defaults.STATUS->scanint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_mp_defaults.STATUS->scanint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                          safe_sprintf(msg,"%s%d", msg, api->rsp->dd_mp_defaults.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR       ");
           if  (api->rsp->dd_mp_defaults.STATUS->err       ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dd_mp_defaults.STATUS->err       ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                          safe_sprintf(msg,"%s%d", msg, api->rsp->dd_mp_defaults.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS       ");
           if  (api->rsp->dd_mp_defaults.STATUS->cts       ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dd_mp_defaults.STATUS->cts       ==     0) safe_strcat(msg,"WAIT     ");
      else                                                          safe_sprintf(msg,"%s%d", msg, api->rsp->dd_mp_defaults.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-MP_A_MODE ");
           if  (api->rsp->dd_mp_defaults.mp_a_mode ==     3) safe_strcat(msg,"AGC_1         ");
      else if  (api->rsp->dd_mp_defaults.mp_a_mode ==     4) safe_strcat(msg,"AGC_1_INVERTED");
      else if  (api->rsp->dd_mp_defaults.mp_a_mode ==     5) safe_strcat(msg,"AGC_2         ");
      else if  (api->rsp->dd_mp_defaults.mp_a_mode ==     6) safe_strcat(msg,"AGC_2_INVERTED");
      else if  (api->rsp->dd_mp_defaults.mp_a_mode ==     0) safe_strcat(msg,"DISABLE       ");
      else if  (api->rsp->dd_mp_defaults.mp_a_mode ==     1) safe_strcat(msg,"DRIVE_0       ");
      else if  (api->rsp->dd_mp_defaults.mp_a_mode ==     2) safe_strcat(msg,"DRIVE_1       ");
      else if  (api->rsp->dd_mp_defaults.mp_a_mode ==     7) safe_strcat(msg,"FEF           ");
      else if  (api->rsp->dd_mp_defaults.mp_a_mode ==     8) safe_strcat(msg,"FEF_INVERTED  ");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->dd_mp_defaults.mp_a_mode);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-MP_B_MODE ");
           if  (api->rsp->dd_mp_defaults.mp_b_mode ==     3) safe_strcat(msg,"AGC_1         ");
      else if  (api->rsp->dd_mp_defaults.mp_b_mode ==     4) safe_strcat(msg,"AGC_1_INVERTED");
      else if  (api->rsp->dd_mp_defaults.mp_b_mode ==     5) safe_strcat(msg,"AGC_2         ");
      else if  (api->rsp->dd_mp_defaults.mp_b_mode ==     6) safe_strcat(msg,"AGC_2_INVERTED");
      else if  (api->rsp->dd_mp_defaults.mp_b_mode ==     0) safe_strcat(msg,"DISABLE       ");
      else if  (api->rsp->dd_mp_defaults.mp_b_mode ==     1) safe_strcat(msg,"DRIVE_0       ");
      else if  (api->rsp->dd_mp_defaults.mp_b_mode ==     2) safe_strcat(msg,"DRIVE_1       ");
      else if  (api->rsp->dd_mp_defaults.mp_b_mode ==     7) safe_strcat(msg,"FEF           ");
      else if  (api->rsp->dd_mp_defaults.mp_b_mode ==     8) safe_strcat(msg,"FEF_INVERTED  ");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->dd_mp_defaults.mp_b_mode);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-MP_C_MODE ");
           if  (api->rsp->dd_mp_defaults.mp_c_mode ==     3) safe_strcat(msg,"AGC_1         ");
      else if  (api->rsp->dd_mp_defaults.mp_c_mode ==     4) safe_strcat(msg,"AGC_1_INVERTED");
      else if  (api->rsp->dd_mp_defaults.mp_c_mode ==     5) safe_strcat(msg,"AGC_2         ");
      else if  (api->rsp->dd_mp_defaults.mp_c_mode ==     6) safe_strcat(msg,"AGC_2_INVERTED");
      else if  (api->rsp->dd_mp_defaults.mp_c_mode ==     0) safe_strcat(msg,"DISABLE       ");
      else if  (api->rsp->dd_mp_defaults.mp_c_mode ==     1) safe_strcat(msg,"DRIVE_0       ");
      else if  (api->rsp->dd_mp_defaults.mp_c_mode ==     2) safe_strcat(msg,"DRIVE_1       ");
      else if  (api->rsp->dd_mp_defaults.mp_c_mode ==     7) safe_strcat(msg,"FEF           ");
      else if  (api->rsp->dd_mp_defaults.mp_c_mode ==     8) safe_strcat(msg,"FEF_INVERTED  ");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->dd_mp_defaults.mp_c_mode);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-MP_D_MODE ");
           if  (api->rsp->dd_mp_defaults.mp_d_mode ==     3) safe_strcat(msg,"AGC_1         ");
      else if  (api->rsp->dd_mp_defaults.mp_d_mode ==     4) safe_strcat(msg,"AGC_1_INVERTED");
      else if  (api->rsp->dd_mp_defaults.mp_d_mode ==     5) safe_strcat(msg,"AGC_2         ");
      else if  (api->rsp->dd_mp_defaults.mp_d_mode ==     6) safe_strcat(msg,"AGC_2_INVERTED");
      else if  (api->rsp->dd_mp_defaults.mp_d_mode ==     0) safe_strcat(msg,"DISABLE       ");
      else if  (api->rsp->dd_mp_defaults.mp_d_mode ==     1) safe_strcat(msg,"DRIVE_0       ");
      else if  (api->rsp->dd_mp_defaults.mp_d_mode ==     2) safe_strcat(msg,"DRIVE_1       ");
      else if  (api->rsp->dd_mp_defaults.mp_d_mode ==     7) safe_strcat(msg,"FEF           ");
      else if  (api->rsp->dd_mp_defaults.mp_d_mode ==     8) safe_strcat(msg,"FEF_INVERTED  ");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->dd_mp_defaults.mp_d_mode);
     break;
    #endif /*     Si2164B_DD_MP_DEFAULTS_CMD */

    #ifdef        Si2164B_DD_PER_CMD
     case         Si2164B_DD_PER_CMD_CODE:
      safe_sprintf(msg,"DD_PER ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->dd_per.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_per.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_per.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->dd_per.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_per.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_per.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->dd_per.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dd_per.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_per.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->dd_per.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dd_per.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_per.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-EXP     "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_per.exp);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-MANT    "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_per.mant);
     break;
    #endif /*     Si2164B_DD_PER_CMD */

    #ifdef        Si2164B_DD_RESTART_CMD
     case         Si2164B_DD_RESTART_CMD_CODE:
      safe_sprintf(msg,"DD_RESTART ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->dd_restart.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_restart.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_restart.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->dd_restart.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_restart.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_restart.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->dd_restart.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dd_restart.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_restart.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->dd_restart.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dd_restart.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_restart.STATUS->cts);
     break;
    #endif /*     Si2164B_DD_RESTART_CMD */

    #ifdef        Si2164B_DD_RESTART_EXT_CMD
     case         Si2164B_DD_RESTART_EXT_CMD_CODE:
      safe_sprintf(msg,"DD_RESTART_EXT ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->dd_restart_ext.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_restart_ext.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                        safe_sprintf(msg,"%s%d", msg, api->rsp->dd_restart_ext.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->dd_restart_ext.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_restart_ext.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                        safe_sprintf(msg,"%s%d", msg, api->rsp->dd_restart_ext.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->dd_restart_ext.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dd_restart_ext.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                        safe_sprintf(msg,"%s%d", msg, api->rsp->dd_restart_ext.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->dd_restart_ext.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dd_restart_ext.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                        safe_sprintf(msg,"%s%d", msg, api->rsp->dd_restart_ext.STATUS->cts);
     break;
    #endif /*     Si2164B_DD_RESTART_EXT_CMD */

    #ifdef        Si2164B_DD_SET_REG_CMD
     case         Si2164B_DD_SET_REG_CMD_CODE:
      safe_sprintf(msg,"DD_SET_REG ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->dd_set_reg.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_set_reg.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_set_reg.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->dd_set_reg.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_set_reg.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_set_reg.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->dd_set_reg.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dd_set_reg.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_set_reg.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->dd_set_reg.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dd_set_reg.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_set_reg.STATUS->cts);
     break;
    #endif /*     Si2164B_DD_SET_REG_CMD */

    #ifdef        Si2164B_DD_SSI_SQI_CMD
     case         Si2164B_DD_SSI_SQI_CMD_CODE:
      safe_sprintf(msg,"DD_SSI_SQI ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->dd_ssi_sqi.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_ssi_sqi.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ssi_sqi.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->dd_ssi_sqi.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_ssi_sqi.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ssi_sqi.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->dd_ssi_sqi.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dd_ssi_sqi.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ssi_sqi.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->dd_ssi_sqi.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dd_ssi_sqi.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ssi_sqi.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SSI     "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ssi_sqi.ssi);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SQI     "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ssi_sqi.sqi);
     break;
    #endif /*     Si2164B_DD_SSI_SQI_CMD */

    #ifdef        Si2164B_DD_STATUS_CMD
     case         Si2164B_DD_STATUS_CMD_CODE:
      safe_sprintf(msg,"DD_STATUS ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT        ");
           if  (api->rsp->dd_status.STATUS->ddint        ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_status.STATUS->ddint        ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                        safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT      ");
           if  (api->rsp->dd_status.STATUS->scanint      ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_status.STATUS->scanint      ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                        safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR          ");
           if  (api->rsp->dd_status.STATUS->err          ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dd_status.STATUS->err          ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                        safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS          ");
           if  (api->rsp->dd_status.STATUS->cts          ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dd_status.STATUS->cts          ==     0) safe_strcat(msg,"WAIT     ");
      else                                                        safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PCLINT       ");
           if  (api->rsp->dd_status.pclint       ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dd_status.pclint       ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.pclint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DLINT        ");
           if  (api->rsp->dd_status.dlint        ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dd_status.dlint        ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.dlint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BERINT       ");
           if  (api->rsp->dd_status.berint       ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dd_status.berint       ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.berint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-UNCORINT     ");
           if  (api->rsp->dd_status.uncorint     ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dd_status.uncorint     ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.uncorint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RSQINT_BIT5  ");
           if  (api->rsp->dd_status.rsqint_bit5  ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dd_status.rsqint_bit5  ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.rsqint_bit5);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RSQINT_BIT6  ");
           if  (api->rsp->dd_status.rsqint_bit6  ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dd_status.rsqint_bit6  ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.rsqint_bit6);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RSQINT_BIT7  ");
           if  (api->rsp->dd_status.rsqint_bit7  ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dd_status.rsqint_bit7  ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.rsqint_bit7);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PCL          ");
           if  (api->rsp->dd_status.pcl          ==     1) safe_strcat(msg,"LOCKED ");
      else if  (api->rsp->dd_status.pcl          ==     0) safe_strcat(msg,"NO_LOCK");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.pcl);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DL           ");
           if  (api->rsp->dd_status.dl           ==     1) safe_strcat(msg,"LOCKED ");
      else if  (api->rsp->dd_status.dl           ==     0) safe_strcat(msg,"NO_LOCK");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.dl);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BER          ");
           if  (api->rsp->dd_status.ber          ==     1) safe_strcat(msg,"BER_ABOVE");
      else if  (api->rsp->dd_status.ber          ==     0) safe_strcat(msg,"BER_BELOW");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.ber);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-UNCOR        ");
           if  (api->rsp->dd_status.uncor        ==     0) safe_strcat(msg,"NO_UNCOR_FOUND");
      else if  (api->rsp->dd_status.uncor        ==     1) safe_strcat(msg,"UNCOR_FOUND   ");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.uncor);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RSQSTAT_BIT5 "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.rsqstat_bit5);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RSQSTAT_BIT6 "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.rsqstat_bit6);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RSQSTAT_BIT7 "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.rsqstat_bit7);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-MODULATION   ");
           if  (api->rsp->dd_status.modulation   ==    10) safe_strcat(msg,"DSS  ");
      else if  (api->rsp->dd_status.modulation   ==     3) safe_strcat(msg,"DVBC ");
      else if  (api->rsp->dd_status.modulation   ==    11) safe_strcat(msg,"DVBC2");
      else if  (api->rsp->dd_status.modulation   ==     8) safe_strcat(msg,"DVBS ");
      else if  (api->rsp->dd_status.modulation   ==     9) safe_strcat(msg,"DVBS2");
      else if  (api->rsp->dd_status.modulation   ==     2) safe_strcat(msg,"DVBT ");
      else if  (api->rsp->dd_status.modulation   ==     7) safe_strcat(msg,"DVBT2");
      else if  (api->rsp->dd_status.modulation   ==     4) safe_strcat(msg,"ISDBT ");
      else if  (api->rsp->dd_status.modulation   ==     1) safe_strcat(msg,"MCNS ");
      else                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.modulation);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TS_BIT_RATE  "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.ts_bit_rate);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TS_CLK_FREQ  "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_status.ts_clk_freq);
     break;
    #endif /*     Si2164B_DD_STATUS_CMD */

    #ifdef        Si2164B_DD_TS_PINS_CMD
     case         Si2164B_DD_TS_PINS_CMD_CODE:
      safe_sprintf(msg,"DD_TS_PINS ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT                 ");
           if  (api->rsp->dd_ts_pins.STATUS->ddint                 ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_ts_pins.STATUS->ddint                 ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ts_pins.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT               ");
           if  (api->rsp->dd_ts_pins.STATUS->scanint               ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_ts_pins.STATUS->scanint               ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ts_pins.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR                   ");
           if  (api->rsp->dd_ts_pins.STATUS->err                   ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dd_ts_pins.STATUS->err                   ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ts_pins.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS                   ");
           if  (api->rsp->dd_ts_pins.STATUS->cts                   ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dd_ts_pins.STATUS->cts                   ==     0) safe_strcat(msg,"WAIT     ");
      else                                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ts_pins.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PRIMARY_TS_MODE       ");
           if  (api->rsp->dd_ts_pins.primary_ts_mode       ==     1) safe_strcat(msg,"DISABLED   ");
      else if  (api->rsp->dd_ts_pins.primary_ts_mode       ==     2) safe_strcat(msg,"DRIVING_TS1");
      else if  (api->rsp->dd_ts_pins.primary_ts_mode       ==     3) safe_strcat(msg,"DRIVING_TS2");
      else                                                          safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ts_pins.primary_ts_mode);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PRIMARY_TS_ACTIVITY   ");
           if  (api->rsp->dd_ts_pins.primary_ts_activity   ==     3) safe_strcat(msg,"ACTIVITY");
      else if  (api->rsp->dd_ts_pins.primary_ts_activity   ==     2) safe_strcat(msg,"CONFLICT");
      else if  (api->rsp->dd_ts_pins.primary_ts_activity   ==     0) safe_strcat(msg,"DRIVING ");
      else if  (api->rsp->dd_ts_pins.primary_ts_activity   ==     1) safe_strcat(msg,"QUIET   ");
      else                                                          safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ts_pins.primary_ts_activity);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SECONDARY_TS_MODE     ");
           if  (api->rsp->dd_ts_pins.secondary_ts_mode     ==     1) safe_strcat(msg,"DISABLED   ");
      else if  (api->rsp->dd_ts_pins.secondary_ts_mode     ==     2) safe_strcat(msg,"DRIVING_TS1");
      else if  (api->rsp->dd_ts_pins.secondary_ts_mode     ==     3) safe_strcat(msg,"DRIVING_TS2");
      else                                                          safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ts_pins.secondary_ts_mode);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SECONDARY_TS_ACTIVITY ");
           if  (api->rsp->dd_ts_pins.secondary_ts_activity ==     3) safe_strcat(msg,"ACTIVITY");
      else if  (api->rsp->dd_ts_pins.secondary_ts_activity ==     2) safe_strcat(msg,"CONFLICT");
      else if  (api->rsp->dd_ts_pins.secondary_ts_activity ==     0) safe_strcat(msg,"DRIVING ");
      else if  (api->rsp->dd_ts_pins.secondary_ts_activity ==     1) safe_strcat(msg,"QUIET   ");
      else                                                          safe_sprintf(msg,"%s%d", msg, api->rsp->dd_ts_pins.secondary_ts_activity);
     break;
    #endif /*     Si2164B_DD_TS_PINS_CMD */

    #ifdef        Si2164B_DD_UNCOR_CMD
     case         Si2164B_DD_UNCOR_CMD_CODE:
      safe_sprintf(msg,"DD_UNCOR ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT     ");
           if  (api->rsp->dd_uncor.STATUS->ddint     ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_uncor.STATUS->ddint     ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_uncor.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT   ");
           if  (api->rsp->dd_uncor.STATUS->scanint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dd_uncor.STATUS->scanint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_uncor.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR       ");
           if  (api->rsp->dd_uncor.STATUS->err       ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dd_uncor.STATUS->err       ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_uncor.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS       ");
           if  (api->rsp->dd_uncor.STATUS->cts       ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dd_uncor.STATUS->cts       ==     0) safe_strcat(msg,"WAIT     ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dd_uncor.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-UNCOR_LSB "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_uncor.uncor_lsb);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-UNCOR_MSB "); safe_sprintf(msg,"%s%d", msg, api->rsp->dd_uncor.uncor_msb);
     break;
    #endif /*     Si2164B_DD_UNCOR_CMD */

    #ifdef        Si2164B_DEMOD_INFO_CMD
     case         Si2164B_DEMOD_INFO_CMD_CODE:
      safe_sprintf(msg,"DEMOD_INFO ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT "  );
           if  (api->rsp->demod_info.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->demod_info.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->demod_info.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->demod_info.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->demod_info.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->demod_info.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR "    );
           if  (api->rsp->demod_info.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->demod_info.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->demod_info.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS "    );
           if  (api->rsp->demod_info.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->demod_info.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->demod_info.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RESERVED "); safe_sprintf(msg,"%s%d", msg, api->rsp->demod_info.reserved);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DIV_A    "); safe_sprintf(msg,"%s%d", msg, api->rsp->demod_info.div_a);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DIV_B    "); safe_sprintf(msg,"%s%d", msg, api->rsp->demod_info.div_b);
     break;
    #endif /*     Si2164B_DEMOD_INFO_CMD */

    #ifdef        Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD
     case         Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD_CODE:
      safe_sprintf(msg,"DOWNLOAD_DATASET_CONTINUE ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->download_dataset_continue.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->download_dataset_continue.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->download_dataset_continue.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->download_dataset_continue.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->download_dataset_continue.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->download_dataset_continue.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->download_dataset_continue.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->download_dataset_continue.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->download_dataset_continue.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->download_dataset_continue.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->download_dataset_continue.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->download_dataset_continue.STATUS->cts);
     break;
    #endif /*     Si2164B_DOWNLOAD_DATASET_CONTINUE_CMD */

    #ifdef        Si2164B_DOWNLOAD_DATASET_START_CMD
     case         Si2164B_DOWNLOAD_DATASET_START_CMD_CODE:
      safe_sprintf(msg,"DOWNLOAD_DATASET_START ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->download_dataset_start.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->download_dataset_start.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                                safe_sprintf(msg,"%s%d", msg, api->rsp->download_dataset_start.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->download_dataset_start.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->download_dataset_start.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                                safe_sprintf(msg,"%s%d", msg, api->rsp->download_dataset_start.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->download_dataset_start.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->download_dataset_start.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                                safe_sprintf(msg,"%s%d", msg, api->rsp->download_dataset_start.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->download_dataset_start.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->download_dataset_start.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                                safe_sprintf(msg,"%s%d", msg, api->rsp->download_dataset_start.STATUS->cts);
     break;
    #endif /*     Si2164B_DOWNLOAD_DATASET_START_CMD */

    #ifdef        Si2164B_DVBC2_CTRL_CMD
     case         Si2164B_DVBC2_CTRL_CMD_CODE:
      safe_sprintf(msg,"DVBC2_CTRL ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->dvbc2_ctrl.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbc2_ctrl.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_ctrl.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->dvbc2_ctrl.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbc2_ctrl.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_ctrl.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->dvbc2_ctrl.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dvbc2_ctrl.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_ctrl.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->dvbc2_ctrl.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dvbc2_ctrl.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_ctrl.STATUS->cts);
     break;
    #endif /*     Si2164B_DVBC2_CTRL_CMD */

    #ifdef        Si2164B_DVBC2_DS_INFO_CMD
     case         Si2164B_DVBC2_DS_INFO_CMD_CODE:
      safe_sprintf(msg,"DVBC2_DS_INFO ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT              ");
           if  (api->rsp->dvbc2_ds_info.STATUS->ddint              ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbc2_ds_info.STATUS->ddint              ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_ds_info.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT            ");
           if  (api->rsp->dvbc2_ds_info.STATUS->scanint            ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbc2_ds_info.STATUS->scanint            ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_ds_info.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR                ");
           if  (api->rsp->dvbc2_ds_info.STATUS->err                ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dvbc2_ds_info.STATUS->err                ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_ds_info.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS                ");
           if  (api->rsp->dvbc2_ds_info.STATUS->cts                ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dvbc2_ds_info.STATUS->cts                ==     0) safe_strcat(msg,"WAIT     ");
      else                                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_ds_info.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DS_ID              "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_ds_info.ds_id);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DSLICE_NUM_PLP     "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_ds_info.dslice_num_plp);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RESERVED_2         "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_ds_info.reserved_2);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DSLICE_TUNE_POS_HZ "); safe_sprintf(msg,"%s%ld", msg, api->rsp->dvbc2_ds_info.dslice_tune_pos_hz);
     break;
    #endif /*     Si2164B_DVBC2_DS_INFO_CMD */

    #ifdef        Si2164B_DVBC2_DS_PLP_SELECT_CMD
     case         Si2164B_DVBC2_DS_PLP_SELECT_CMD_CODE:
      safe_sprintf(msg,"DVBC2_DS_PLP_SELECT ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->dvbc2_ds_plp_select.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbc2_ds_plp_select.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                             safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_ds_plp_select.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->dvbc2_ds_plp_select.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbc2_ds_plp_select.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                             safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_ds_plp_select.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->dvbc2_ds_plp_select.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dvbc2_ds_plp_select.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                             safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_ds_plp_select.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->dvbc2_ds_plp_select.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dvbc2_ds_plp_select.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                             safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_ds_plp_select.STATUS->cts);
     break;
    #endif /*     Si2164B_DVBC2_DS_PLP_SELECT_CMD */

    #ifdef        Si2164B_DVBC2_PLP_INFO_CMD
     case         Si2164B_DVBC2_PLP_INFO_CMD_CODE:
      safe_sprintf(msg,"DVBC2_PLP_INFO ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT            ");
           if  (api->rsp->dvbc2_plp_info.STATUS->ddint            ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbc2_plp_info.STATUS->ddint            ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                                 safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_plp_info.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT          ");
           if  (api->rsp->dvbc2_plp_info.STATUS->scanint          ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbc2_plp_info.STATUS->scanint          ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                                 safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_plp_info.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR              ");
           if  (api->rsp->dvbc2_plp_info.STATUS->err              ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dvbc2_plp_info.STATUS->err              ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                                 safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_plp_info.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS              ");
           if  (api->rsp->dvbc2_plp_info.STATUS->cts              ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dvbc2_plp_info.STATUS->cts              ==     0) safe_strcat(msg,"WAIT     ");
      else                                                                 safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_plp_info.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_ID           "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_plp_info.plp_id);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_PAYLOAD_TYPE ");
           if  (api->rsp->dvbc2_plp_info.plp_payload_type ==     1) safe_strcat(msg,"GCS ");
      else if  (api->rsp->dvbc2_plp_info.plp_payload_type ==     0) safe_strcat(msg,"GFPS");
      else if  (api->rsp->dvbc2_plp_info.plp_payload_type ==     2) safe_strcat(msg,"GSE ");
      else if  (api->rsp->dvbc2_plp_info.plp_payload_type ==     3) safe_strcat(msg,"TS  ");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_plp_info.plp_payload_type);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_TYPE         ");
           if  (api->rsp->dvbc2_plp_info.plp_type         ==     0) safe_strcat(msg,"COMMON    ");
      else if  (api->rsp->dvbc2_plp_info.plp_type         ==     1) safe_strcat(msg,"GROUPED ");
      else if  (api->rsp->dvbc2_plp_info.plp_type         ==     2) safe_strcat(msg,"NORMAL  ");
      else if  (api->rsp->dvbc2_plp_info.plp_type         ==     3) safe_strcat(msg,"RESERVED");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_plp_info.plp_type);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_BUNDLED      ");
           if  (api->rsp->dvbc2_plp_info.plp_bundled      ==     1) safe_strcat(msg,"BUNDLED    ");
      else if  (api->rsp->dvbc2_plp_info.plp_bundled      ==     0) safe_strcat(msg,"NOT_BUNDLED");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_plp_info.plp_bundled);
     break;
    #endif /*     Si2164B_DVBC2_PLP_INFO_CMD */

    #ifdef        Si2164B_DVBC2_STATUS_CMD
     case         Si2164B_DVBC2_STATUS_CMD_CODE:
      safe_sprintf(msg,"DVBC2_STATUS ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT         ");
           if  (api->rsp->dvbc2_status.STATUS->ddint         ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbc2_status.STATUS->ddint         ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT       ");
           if  (api->rsp->dvbc2_status.STATUS->scanint       ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbc2_status.STATUS->scanint       ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR           ");
           if  (api->rsp->dvbc2_status.STATUS->err           ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dvbc2_status.STATUS->err           ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS           ");
           if  (api->rsp->dvbc2_status.STATUS->cts           ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dvbc2_status.STATUS->cts           ==     0) safe_strcat(msg,"WAIT     ");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PCLINT        ");
           if  (api->rsp->dvbc2_status.pclint        ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbc2_status.pclint        ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.pclint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DLINT         ");
           if  (api->rsp->dvbc2_status.dlint         ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbc2_status.dlint         ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.dlint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BERINT        ");
           if  (api->rsp->dvbc2_status.berint        ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbc2_status.berint        ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.berint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-UNCORINT      ");
           if  (api->rsp->dvbc2_status.uncorint      ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbc2_status.uncorint      ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.uncorint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-NOTDVBC2INT   ");
           if  (api->rsp->dvbc2_status.notdvbc2int   ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbc2_status.notdvbc2int   ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.notdvbc2int);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-REQINT        ");
           if  (api->rsp->dvbc2_status.reqint        ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbc2_status.reqint        ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.reqint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-EWBSINT       ");
           if  (api->rsp->dvbc2_status.ewbsint       ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbc2_status.ewbsint       ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.reqint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PCL           ");
           if  (api->rsp->dvbc2_status.pcl           ==     1) safe_strcat(msg,"LOCKED ");
      else if  (api->rsp->dvbc2_status.pcl           ==     0) safe_strcat(msg,"NO_LOCK");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.pcl);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DL            ");
           if  (api->rsp->dvbc2_status.dl            ==     1) safe_strcat(msg,"LOCKED ");
      else if  (api->rsp->dvbc2_status.dl            ==     0) safe_strcat(msg,"NO_LOCK");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.dl);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BER           ");
           if  (api->rsp->dvbc2_status.ber           ==     1) safe_strcat(msg,"BER_ABOVE");
      else if  (api->rsp->dvbc2_status.ber           ==     0) safe_strcat(msg,"BER_BELOW");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.ber);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-UNCOR         ");
           if  (api->rsp->dvbc2_status.uncor         ==     0) safe_strcat(msg,"NO_UNCOR_FOUND");
      else if  (api->rsp->dvbc2_status.uncor         ==     1) safe_strcat(msg,"UNCOR_FOUND   ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.uncor);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-NOTDVBC2      ");
           if  (api->rsp->dvbc2_status.notdvbc2      ==     0) safe_strcat(msg,"DVBC2    ");
      else if  (api->rsp->dvbc2_status.notdvbc2      ==     1) safe_strcat(msg,"NOT_DVBC2");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.notdvbc2);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-REQ           ");
           if  (api->rsp->dvbc2_status.req           ==     0) safe_strcat(msg,"NO_REQUEST");
      else if  (api->rsp->dvbc2_status.req           ==     1) safe_strcat(msg,"REQUEST   ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.req);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-EWBS          ");
           if  (api->rsp->dvbc2_status.ewbs          ==     0) safe_strcat(msg,"NORMAL ");
      else if  (api->rsp->dvbc2_status.ewbs          ==     1) safe_strcat(msg,"WARNING");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.ewbs);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CNR           "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.cnr);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-AFC_FREQ      "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.afc_freq);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TIMING_OFFSET "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.timing_offset);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DVBC2_STATUS  ");
           if  (api->rsp->dvbc2_status.dvbc2_status  ==     0) safe_strcat(msg,"IDLE        ");
      else if  (api->rsp->dvbc2_status.dvbc2_status  ==     3) safe_strcat(msg,"INVALID_DS  ");
      else if  (api->rsp->dvbc2_status.dvbc2_status  ==     2) safe_strcat(msg,"READY       ");
      else if  (api->rsp->dvbc2_status.dvbc2_status  ==     1) safe_strcat(msg,"SEARCHING   ");
      else if  (api->rsp->dvbc2_status.dvbc2_status  ==     4) safe_strcat(msg,"TUNE_REQUEST");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.dvbc2_status);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CONSTELLATION ");
           if  (api->rsp->dvbc2_status.constellation ==    13) safe_strcat(msg,"QAM1024");
      else if  (api->rsp->dvbc2_status.constellation ==     7) safe_strcat(msg,"QAM16  ");
      else if  (api->rsp->dvbc2_status.constellation ==    11) safe_strcat(msg,"QAM256 ");
      else if  (api->rsp->dvbc2_status.constellation ==    15) safe_strcat(msg,"QAM4096");
      else if  (api->rsp->dvbc2_status.constellation ==     9) safe_strcat(msg,"QAM64  ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.constellation);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SP_INV        ");
           if  (api->rsp->dvbc2_status.sp_inv        ==     1) safe_strcat(msg,"INVERTED");
      else if  (api->rsp->dvbc2_status.sp_inv        ==     0) safe_strcat(msg,"NORMAL  ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.sp_inv);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CODE_RATE     ");
           if  (api->rsp->dvbc2_status.code_rate     ==     2) safe_strcat(msg,"2_3 ");
      else if  (api->rsp->dvbc2_status.code_rate     ==     3) safe_strcat(msg,"3_4 ");
      else if  (api->rsp->dvbc2_status.code_rate     ==     4) safe_strcat(msg,"4_5 ");
      else if  (api->rsp->dvbc2_status.code_rate     ==     5) safe_strcat(msg,"5_6 ");
      else if  (api->rsp->dvbc2_status.code_rate     ==     8) safe_strcat(msg,"8_9 ");
      else if  (api->rsp->dvbc2_status.code_rate     ==     9) safe_strcat(msg,"9_10");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.code_rate);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-GUARD_INT     ");
           if  (api->rsp->dvbc2_status.guard_int     ==     0) safe_strcat(msg,"1_128");
      else if  (api->rsp->dvbc2_status.guard_int     ==     1) safe_strcat(msg,"1_64 ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.guard_int);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DS_ID         "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.ds_id);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_ID        "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_status.plp_id);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RF_FREQ       "); safe_sprintf(msg,"%s%ld", msg, api->rsp->dvbc2_status.rf_freq);
     break;
    #endif /*     Si2164B_DVBC2_STATUS_CMD */

    #ifdef        Si2164B_DVBC2_SYS_INFO_CMD
     case         Si2164B_DVBC2_SYS_INFO_CMD_CODE:
      safe_sprintf(msg,"DVBC2_SYS_INFO ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT              ");
           if  (api->rsp->dvbc2_sys_info.STATUS->ddint              ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbc2_sys_info.STATUS->ddint              ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_sys_info.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT            ");
           if  (api->rsp->dvbc2_sys_info.STATUS->scanint            ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbc2_sys_info.STATUS->scanint            ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_sys_info.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR                ");
           if  (api->rsp->dvbc2_sys_info.STATUS->err                ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dvbc2_sys_info.STATUS->err                ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_sys_info.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS                ");
           if  (api->rsp->dvbc2_sys_info.STATUS->cts                ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dvbc2_sys_info.STATUS->cts                ==     0) safe_strcat(msg,"WAIT     ");
      else                                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_sys_info.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-NUM_DSLICE         "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_sys_info.num_dslice);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-NETWORK_ID         "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_sys_info.network_id);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-C2_BANDWIDTH_HZ    "); safe_sprintf(msg,"%s%ld", msg, api->rsp->dvbc2_sys_info.c2_bandwidth_hz);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-START_FREQUENCY_HZ "); safe_sprintf(msg,"%s%ld", msg, api->rsp->dvbc2_sys_info.start_frequency_hz);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-C2_SYSTEM_ID       "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_sys_info.c2_system_id);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RESERVED_4         "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc2_sys_info.reserved_4);
     break;
    #endif /*     Si2164B_DVBC2_SYS_INFO_CMD */


    #ifdef        Si2164B_DVBC_STATUS_CMD
     case         Si2164B_DVBC_STATUS_CMD_CODE:
      safe_sprintf(msg,"DVBC_STATUS ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT         ");
           if  (api->rsp->dvbc_status.STATUS->ddint         ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbc_status.STATUS->ddint         ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                           safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT       ");
           if  (api->rsp->dvbc_status.STATUS->scanint       ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbc_status.STATUS->scanint       ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                           safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR           ");
           if  (api->rsp->dvbc_status.STATUS->err           ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dvbc_status.STATUS->err           ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                           safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS           ");
           if  (api->rsp->dvbc_status.STATUS->cts           ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dvbc_status.STATUS->cts           ==     0) safe_strcat(msg,"WAIT     ");
      else                                                           safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PCLINT        ");
           if  (api->rsp->dvbc_status.pclint        ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbc_status.pclint        ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.pclint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DLINT         ");
           if  (api->rsp->dvbc_status.dlint         ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbc_status.dlint         ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.dlint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BERINT        ");
           if  (api->rsp->dvbc_status.berint        ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbc_status.berint        ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.berint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-UNCORINT      ");
           if  (api->rsp->dvbc_status.uncorint      ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbc_status.uncorint      ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.uncorint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-NOTDVBCINT    ");
           if  (api->rsp->dvbc_status.notdvbcint    ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbc_status.notdvbcint    ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.notdvbcint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PCL           ");
           if  (api->rsp->dvbc_status.pcl           ==     1) safe_strcat(msg,"LOCKED ");
      else if  (api->rsp->dvbc_status.pcl           ==     0) safe_strcat(msg,"NO_LOCK");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.pcl);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DL            ");
           if  (api->rsp->dvbc_status.dl            ==     1) safe_strcat(msg,"LOCKED ");
      else if  (api->rsp->dvbc_status.dl            ==     0) safe_strcat(msg,"NO_LOCK");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.dl);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BER           ");
           if  (api->rsp->dvbc_status.ber           ==     1) safe_strcat(msg,"BER_ABOVE");
      else if  (api->rsp->dvbc_status.ber           ==     0) safe_strcat(msg,"BER_BELOW");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.ber);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-UNCOR         ");
           if  (api->rsp->dvbc_status.uncor         ==     0) safe_strcat(msg,"NO_UNCOR_FOUND");
      else if  (api->rsp->dvbc_status.uncor         ==     1) safe_strcat(msg,"UNCOR_FOUND   ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.uncor);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-NOTDVBC       "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.notdvbc);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CNR           "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.cnr);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-AFC_FREQ      "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.afc_freq);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TIMING_OFFSET "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.timing_offset);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CONSTELLATION ");
           if  (api->rsp->dvbc_status.constellation ==    10) safe_strcat(msg,"QAM128");
      else if  (api->rsp->dvbc_status.constellation ==     7) safe_strcat(msg,"QAM16 ");
      else if  (api->rsp->dvbc_status.constellation ==    11) safe_strcat(msg,"QAM256");
      else if  (api->rsp->dvbc_status.constellation ==     8) safe_strcat(msg,"QAM32 ");
      else if  (api->rsp->dvbc_status.constellation ==     9) safe_strcat(msg,"QAM64 ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.constellation);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SP_INV        ");
           if  (api->rsp->dvbc_status.sp_inv        ==     1) safe_strcat(msg,"INVERTED");
      else if  (api->rsp->dvbc_status.sp_inv        ==     0) safe_strcat(msg,"NORMAL  ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbc_status.sp_inv);
     break;
    #endif /*     Si2164B_DVBC_STATUS_CMD */


    #ifdef        Si2164B_DVBS2_STATUS_CMD
     case         Si2164B_DVBS2_STATUS_CMD_CODE:
      safe_sprintf(msg,"DVBS2_STATUS ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT         ");
           if  (api->rsp->dvbs2_status.STATUS->ddint         ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbs2_status.STATUS->ddint         ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT       ");
           if  (api->rsp->dvbs2_status.STATUS->scanint       ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbs2_status.STATUS->scanint       ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR           ");
           if  (api->rsp->dvbs2_status.STATUS->err           ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dvbs2_status.STATUS->err           ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS           ");
           if  (api->rsp->dvbs2_status.STATUS->cts           ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dvbs2_status.STATUS->cts           ==     0) safe_strcat(msg,"WAIT     ");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PCLINT        ");
           if  (api->rsp->dvbs2_status.pclint        ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbs2_status.pclint        ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.pclint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DLINT         ");
           if  (api->rsp->dvbs2_status.dlint         ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbs2_status.dlint         ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.dlint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BERINT        ");
           if  (api->rsp->dvbs2_status.berint        ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbs2_status.berint        ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.berint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-UNCORINT      ");
           if  (api->rsp->dvbs2_status.uncorint      ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbs2_status.uncorint      ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.uncorint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PCL           ");
           if  (api->rsp->dvbs2_status.pcl           ==     1) safe_strcat(msg,"LOCKED ");
      else if  (api->rsp->dvbs2_status.pcl           ==     0) safe_strcat(msg,"NO_LOCK");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.pcl);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DL            ");
           if  (api->rsp->dvbs2_status.dl            ==     1) safe_strcat(msg,"LOCKED ");
      else if  (api->rsp->dvbs2_status.dl            ==     0) safe_strcat(msg,"NO_LOCK");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.dl);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BER           ");
           if  (api->rsp->dvbs2_status.ber           ==     1) safe_strcat(msg,"BER_ABOVE");
      else if  (api->rsp->dvbs2_status.ber           ==     0) safe_strcat(msg,"BER_BELOW");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.ber);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-UNCOR         ");
           if  (api->rsp->dvbs2_status.uncor         ==     0) safe_strcat(msg,"NO_UNCOR_FOUND");
      else if  (api->rsp->dvbs2_status.uncor         ==     1) safe_strcat(msg,"UNCOR_FOUND   ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.uncor);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CNR           "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.cnr);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-AFC_FREQ      "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.afc_freq);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TIMING_OFFSET "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.timing_offset);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CONSTELLATION ");
           if  (api->rsp->dvbs2_status.constellation ==    20) safe_strcat(msg,"16APSK  ");
      else if  (api->rsp->dvbs2_status.constellation ==    24) safe_strcat(msg,"16APSK_L");
      else if  (api->rsp->dvbs2_status.constellation ==    21) safe_strcat(msg,"32APSK_1");
      else if  (api->rsp->dvbs2_status.constellation ==    26) safe_strcat(msg,"32APSK_2");
      else if  (api->rsp->dvbs2_status.constellation ==    25) safe_strcat(msg,"32APSK_L");
      else if  (api->rsp->dvbs2_status.constellation ==    23) safe_strcat(msg,"8APSK_L ");
      else if  (api->rsp->dvbs2_status.constellation ==    14) safe_strcat(msg,"8PSK");
      else if  (api->rsp->dvbs2_status.constellation ==     3) safe_strcat(msg,"QPSK");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.constellation);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SP_INV        ");
           if  (api->rsp->dvbs2_status.sp_inv        ==     1) safe_strcat(msg,"INVERTED");
      else if  (api->rsp->dvbs2_status.sp_inv        ==     0) safe_strcat(msg,"NORMAL  ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.sp_inv);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PILOTS        ");
           if  (api->rsp->dvbs2_status.pilots        ==     0) safe_strcat(msg,"OFF");
      else if  (api->rsp->dvbs2_status.pilots        ==     1) safe_strcat(msg,"ON ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.pilots);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CODE_RATE     ");
           if  (api->rsp->dvbs2_status.code_rate     ==    27) safe_strcat(msg,"11_15");
      else if  (api->rsp->dvbs2_status.code_rate     ==    19) safe_strcat(msg,"11_20");
      else if  (api->rsp->dvbs2_status.code_rate     ==    26) safe_strcat(msg,"13_18");
      else if  (api->rsp->dvbs2_status.code_rate     ==    16) safe_strcat(msg,"13_45");
      else if  (api->rsp->dvbs2_status.code_rate     ==     1) safe_strcat(msg,"1_2 ");
      else if  (api->rsp->dvbs2_status.code_rate     ==    10) safe_strcat(msg,"1_3 ");
      else if  (api->rsp->dvbs2_status.code_rate     ==    11) safe_strcat(msg,"1_4 ");
      else if  (api->rsp->dvbs2_status.code_rate     ==    23) safe_strcat(msg,"23_36");
      else if  (api->rsp->dvbs2_status.code_rate     ==    24) safe_strcat(msg,"25_36");
      else if  (api->rsp->dvbs2_status.code_rate     ==    21) safe_strcat(msg,"26_45");
      else if  (api->rsp->dvbs2_status.code_rate     ==    22) safe_strcat(msg,"28_45");
      else if  (api->rsp->dvbs2_status.code_rate     ==     2) safe_strcat(msg,"2_3 ");
      else if  (api->rsp->dvbs2_status.code_rate     ==    12) safe_strcat(msg,"2_5 ");
      else if  (api->rsp->dvbs2_status.code_rate     ==    25) safe_strcat(msg,"32_45");
      else if  (api->rsp->dvbs2_status.code_rate     ==     3) safe_strcat(msg,"3_4 ");
      else if  (api->rsp->dvbs2_status.code_rate     ==    13) safe_strcat(msg,"3_5 ");
      else if  (api->rsp->dvbs2_status.code_rate     ==     4) safe_strcat(msg,"4_5 ");
      else if  (api->rsp->dvbs2_status.code_rate     ==     5) safe_strcat(msg,"5_6 ");
      else if  (api->rsp->dvbs2_status.code_rate     ==    20) safe_strcat(msg,"5_9  ");
      else if  (api->rsp->dvbs2_status.code_rate     ==    29) safe_strcat(msg,"77_90");
      else if  (api->rsp->dvbs2_status.code_rate     ==    28) safe_strcat(msg,"7_9  ");
      else if  (api->rsp->dvbs2_status.code_rate     ==    18) safe_strcat(msg,"8_15 ");
      else if  (api->rsp->dvbs2_status.code_rate     ==     8) safe_strcat(msg,"8_9 ");
      else if  (api->rsp->dvbs2_status.code_rate     ==     9) safe_strcat(msg,"9_10");
      else if  (api->rsp->dvbs2_status.code_rate     ==    17) safe_strcat(msg,"9_20 ");
      else                                                    safe_sprintf(msg,"%d", msg, api->rsp->dvbs2_status.code_rate);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ROLL_OFF      ");
           if  (api->rsp->dvbs2_status.roll_off      ==     6) safe_strcat(msg,"0_05");
      else if  (api->rsp->dvbs2_status.roll_off      ==     5) safe_strcat(msg,"0_10");
      else if  (api->rsp->dvbs2_status.roll_off      ==     4) safe_strcat(msg,"0_15");
      else if  (api->rsp->dvbs2_status.roll_off      ==     2) safe_strcat(msg,"0_20");
      else if  (api->rsp->dvbs2_status.roll_off      ==     1) safe_strcat(msg,"0_25");
      else if  (api->rsp->dvbs2_status.roll_off      ==     0) safe_strcat(msg,"0_35");
      else if  (api->rsp->dvbs2_status.roll_off      ==     3) safe_strcat(msg,"N_A ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.roll_off);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CCM_VCM       ");
           if  (api->rsp->dvbs2_status.ccm_vcm       ==     1) safe_strcat(msg,"CCM");
      else if  (api->rsp->dvbs2_status.ccm_vcm       ==     0) safe_strcat(msg,"VCM");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.ccm_vcm);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SIS_MIS       ");
           if  (api->rsp->dvbs2_status.sis_mis       ==     0) safe_strcat(msg,"MIS");
      else if  (api->rsp->dvbs2_status.sis_mis       ==     1) safe_strcat(msg,"SIS");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_status.sis_mis);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-NUM_IS        "); safe_sprintf(msg,"%s%d" , msg, api->rsp->dvbs2_status.num_is);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ISI_ID        "); safe_sprintf(msg,"%s%d" , msg, api->rsp->dvbs2_status.isi_id);
     break;
    #endif /*     Si2164B_DVBS2_STATUS_CMD */

    #ifdef        Si2164B_DVBS2_STREAM_INFO_CMD
     case         Si2164B_DVBS2_STREAM_INFO_CMD_CODE:
      safe_sprintf(msg,"DVBS2_STREAM_INFO ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT             ");
           if  (api->rsp->dvbs2_stream_info.STATUS->ddint             ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbs2_stream_info.STATUS->ddint             ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                                     safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_stream_info.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT           ");
           if  (api->rsp->dvbs2_stream_info.STATUS->scanint           ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbs2_stream_info.STATUS->scanint           ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                                     safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_stream_info.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR               ");
           if  (api->rsp->dvbs2_stream_info.STATUS->err               ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dvbs2_stream_info.STATUS->err               ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                                     safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_stream_info.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS               ");
           if  (api->rsp->dvbs2_stream_info.STATUS->cts               ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dvbs2_stream_info.STATUS->cts               ==     0) safe_strcat(msg,"WAIT     ");
      else                                                                     safe_sprintf(msg,"%s%d", api->rsp->dvbs2_stream_info.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ISI_ID            "); safe_sprintf(msg,"%s%d" , msg, api->rsp->dvbs2_stream_info.isi_id);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ISI_CONSTELLATION ");
           if  (api->rsp->dvbs2_stream_info.isi_constellation ==    20) safe_strcat(msg,"16APSK  ");
      else if  (api->rsp->dvbs2_stream_info.isi_constellation ==    24) safe_strcat(msg,"16APSK_L");
      else if  (api->rsp->dvbs2_stream_info.isi_constellation ==    21) safe_strcat(msg,"32APSK_1");
      else if  (api->rsp->dvbs2_stream_info.isi_constellation ==    26) safe_strcat(msg,"32APSK_2");
      else if  (api->rsp->dvbs2_stream_info.isi_constellation ==    25) safe_strcat(msg,"32APSK_L");
      else if  (api->rsp->dvbs2_stream_info.isi_constellation ==    23) safe_strcat(msg,"8APSK_L ");
      else if  (api->rsp->dvbs2_stream_info.isi_constellation ==    14) safe_strcat(msg,"8PSK    ");
      else if  (api->rsp->dvbs2_stream_info.isi_constellation ==     3) safe_strcat(msg,"QPSK    ");
      else                                                             safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_stream_info.isi_constellation);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ISI_CODE_RATE     ");
           if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==    27) safe_strcat(msg,"11_15");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==    19) safe_strcat(msg,"11_20");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==    26) safe_strcat(msg,"13_18");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==    16) safe_strcat(msg,"13_45");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==     1) safe_strcat(msg,"1_2  ");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==    10) safe_strcat(msg,"1_3  ");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==    11) safe_strcat(msg,"1_4  ");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==    23) safe_strcat(msg,"23_36");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==    24) safe_strcat(msg,"25_36");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==    21) safe_strcat(msg,"26_45");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==    22) safe_strcat(msg,"28_45");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==     2) safe_strcat(msg,"2_3  ");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==    12) safe_strcat(msg,"2_5  ");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==    25) safe_strcat(msg,"32_45");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==     3) safe_strcat(msg,"3_4  ");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==    13) safe_strcat(msg,"3_5  ");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==     4) safe_strcat(msg,"4_5  ");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==     5) safe_strcat(msg,"5_6  ");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==    20) safe_strcat(msg,"5_9  ");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==    29) safe_strcat(msg,"77_90");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==    28) safe_strcat(msg,"7_9  ");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==    18) safe_strcat(msg,"8_15 ");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==     8) safe_strcat(msg,"8_9  ");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==     9) safe_strcat(msg,"9_10 ");
      else if  (api->rsp->dvbs2_stream_info.isi_code_rate     ==    17) safe_strcat(msg,"9_20 ");
      else                                                             safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_stream_info.isi_code_rate);
     break;
    #endif /*     Si2164B_DVBS2_STREAM_INFO_CMD */

    #ifdef        Si2164B_DVBS2_STREAM_SELECT_CMD
     case         Si2164B_DVBS2_STREAM_SELECT_CMD_CODE:
      safe_sprintf(msg,"DVBS2_STREAM_SELECT ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->dvbs2_stream_select.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbs2_stream_select.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                             safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_stream_select.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->dvbs2_stream_select.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbs2_stream_select.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                             safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_stream_select.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->dvbs2_stream_select.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dvbs2_stream_select.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                             safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_stream_select.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->dvbs2_stream_select.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dvbs2_stream_select.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                             safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs2_stream_select.STATUS->cts);
     break;
    #endif /*     Si2164B_DVBS2_STREAM_SELECT_CMD */

    #ifdef        Si2164B_DVBS_STATUS_CMD
     case         Si2164B_DVBS_STATUS_CMD_CODE:
      safe_sprintf(msg,"DVBS_STATUS ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT         ");
           if  (api->rsp->dvbs_status.STATUS->ddint         ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbs_status.STATUS->ddint         ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                           safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs_status.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT       ");
           if  (api->rsp->dvbs_status.STATUS->scanint       ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbs_status.STATUS->scanint       ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                           safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs_status.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR           ");
           if  (api->rsp->dvbs_status.STATUS->err           ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dvbs_status.STATUS->err           ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                           safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs_status.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS           ");
           if  (api->rsp->dvbs_status.STATUS->cts           ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dvbs_status.STATUS->cts           ==     0) safe_strcat(msg,"WAIT     ");
      else                                                           safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs_status.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PCLINT        ");
           if  (api->rsp->dvbs_status.pclint        ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbs_status.pclint        ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs_status.pclint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DLINT         ");
           if  (api->rsp->dvbs_status.dlint         ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbs_status.dlint         ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs_status.dlint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BERINT        ");
           if  (api->rsp->dvbs_status.berint        ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbs_status.berint        ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs_status.berint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-UNCORINT      ");
           if  (api->rsp->dvbs_status.uncorint      ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbs_status.uncorint      ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs_status.uncorint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PCL           ");
           if  (api->rsp->dvbs_status.pcl           ==     1) safe_strcat(msg,"LOCKED ");
      else if  (api->rsp->dvbs_status.pcl           ==     0) safe_strcat(msg,"NO_LOCK");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs_status.pcl);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DL            ");
           if  (api->rsp->dvbs_status.dl            ==     1) safe_strcat(msg,"LOCKED ");
      else if  (api->rsp->dvbs_status.dl            ==     0) safe_strcat(msg,"NO_LOCK");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs_status.dl);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BER           ");
           if  (api->rsp->dvbs_status.ber           ==     1) safe_strcat(msg,"BER_ABOVE");
      else if  (api->rsp->dvbs_status.ber           ==     0) safe_strcat(msg,"BER_BELOW");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs_status.ber);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-UNCOR         ");
           if  (api->rsp->dvbs_status.uncor         ==     0) safe_strcat(msg,"NO_UNCOR_FOUND");
      else if  (api->rsp->dvbs_status.uncor         ==     1) safe_strcat(msg,"UNCOR_FOUND   ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs_status.uncor);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CNR           "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs_status.cnr);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-AFC_FREQ      "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs_status.afc_freq);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TIMING_OFFSET "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs_status.timing_offset);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CONSTELLATION ");
           if  (api->rsp->dvbs_status.constellation ==     3) safe_strcat(msg,"QPSK");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs_status.constellation);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SP_INV        ");
           if  (api->rsp->dvbs_status.sp_inv        ==     1) safe_strcat(msg,"INVERTED");
      else if  (api->rsp->dvbs_status.sp_inv        ==     0) safe_strcat(msg,"NORMAL  ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs_status.sp_inv);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CODE_RATE     ");
           if  (api->rsp->dvbs_status.code_rate     ==     1) safe_strcat(msg,"1_2");
      else if  (api->rsp->dvbs_status.code_rate     ==     2) safe_strcat(msg,"2_3");
      else if  (api->rsp->dvbs_status.code_rate     ==     3) safe_strcat(msg,"3_4");
      else if  (api->rsp->dvbs_status.code_rate     ==     5) safe_strcat(msg,"5_6");
      else if  (api->rsp->dvbs_status.code_rate     ==     6) safe_strcat(msg,"6_7");
      else if  (api->rsp->dvbs_status.code_rate     ==     7) safe_strcat(msg,"7_8");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbs_status.code_rate);
     break;
    #endif /*     Si2164B_DVBS_STATUS_CMD */


    #ifdef        Si2164B_DVBT2_FEF_CMD
     case         Si2164B_DVBT2_FEF_CMD_CODE:
      safe_sprintf(msg,"DVBT2_FEF ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT          ");
           if  (api->rsp->dvbt2_fef.STATUS->ddint          ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbt2_fef.STATUS->ddint          ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                          safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_fef.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT        ");
           if  (api->rsp->dvbt2_fef.STATUS->scanint        ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbt2_fef.STATUS->scanint        ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                          safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_fef.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR            ");
           if  (api->rsp->dvbt2_fef.STATUS->err            ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dvbt2_fef.STATUS->err            ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                          safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_fef.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS            ");
           if  (api->rsp->dvbt2_fef.STATUS->cts            ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dvbt2_fef.STATUS->cts            ==     0) safe_strcat(msg,"WAIT     ");
      else                                                          safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_fef.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-FEF_TYPE       "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_fef.fef_type);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-FEF_LENGTH     "); safe_sprintf(msg,"%s%ld", msg, api->rsp->dvbt2_fef.fef_length);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-FEF_REPETITION "); safe_sprintf(msg,"%s%ld", msg, api->rsp->dvbt2_fef.fef_repetition);
     break;
    #endif /*     Si2164B_DVBT2_FEF_CMD */

    #ifdef        Si2164B_DVBT2_PLP_INFO_CMD
     case         Si2164B_DVBT2_PLP_INFO_CMD_CODE:
      safe_sprintf(msg,"DVBT2_PLP_INFO ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT                  ");
           if  (api->rsp->dvbt2_plp_info.STATUS->ddint                  ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbt2_plp_info.STATUS->ddint                  ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                                       safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT                ");
           if  (api->rsp->dvbt2_plp_info.STATUS->scanint                ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbt2_plp_info.STATUS->scanint                ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                                       safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR                    ");
           if  (api->rsp->dvbt2_plp_info.STATUS->err                    ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dvbt2_plp_info.STATUS->err                    ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                                       safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS                    ");
           if  (api->rsp->dvbt2_plp_info.STATUS->cts                    ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dvbt2_plp_info.STATUS->cts                    ==     0) safe_strcat(msg,"WAIT     ");
      else                                                                       safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_ID                 "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.plp_id);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_PAYLOAD_TYPE       ");
           if  (api->rsp->dvbt2_plp_info.plp_payload_type       ==     1) safe_strcat(msg,"GCS ");
      else if  (api->rsp->dvbt2_plp_info.plp_payload_type       ==     0) safe_strcat(msg,"GFPS");
      else if  (api->rsp->dvbt2_plp_info.plp_payload_type       ==     2) safe_strcat(msg,"GSE ");
      else if  (api->rsp->dvbt2_plp_info.plp_payload_type       ==     3) safe_strcat(msg,"TS  ");
      else                                                               safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.plp_payload_type);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_TYPE               ");
           if  (api->rsp->dvbt2_plp_info.plp_type               ==     0) safe_strcat(msg,"COMMON    ");
      else if  (api->rsp->dvbt2_plp_info.plp_type               ==     1) safe_strcat(msg,"Data_Type1");
      else if  (api->rsp->dvbt2_plp_info.plp_type               ==     2) safe_strcat(msg,"Data_Type2");
      else                                                               safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.plp_type);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-FIRST_FRAME_IDX_MSB    "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.first_frame_idx_msb);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-FIRST_RF_IDX           "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.first_rf_idx);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-FF_FLAG                "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.ff_flag);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_GROUP_ID_MSB       "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.plp_group_id_msb);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-FIRST_FRAME_IDX_LSB    "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.first_frame_idx_lsb);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_MOD_MSB            "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.plp_mod_msb);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_COD                ");
           if  (api->rsp->dvbt2_plp_info.plp_cod                ==     0) safe_strcat(msg,"1_2");
      else if  (api->rsp->dvbt2_plp_info.plp_cod                ==     6) safe_strcat(msg,"1_3");
      else if  (api->rsp->dvbt2_plp_info.plp_cod                ==     2) safe_strcat(msg,"2_3");
      else if  (api->rsp->dvbt2_plp_info.plp_cod                ==     7) safe_strcat(msg,"2_5");
      else if  (api->rsp->dvbt2_plp_info.plp_cod                ==     3) safe_strcat(msg,"3_4");
      else if  (api->rsp->dvbt2_plp_info.plp_cod                ==     1) safe_strcat(msg,"3_5");
      else if  (api->rsp->dvbt2_plp_info.plp_cod                ==     4) safe_strcat(msg,"4_5");
      else if  (api->rsp->dvbt2_plp_info.plp_cod                ==     5) safe_strcat(msg,"5_6");
      else                                                               safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.plp_cod);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_GROUP_ID_LSB       "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.plp_group_id_lsb);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_NUM_BLOCKS_MAX_MSB "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.plp_num_blocks_max_msb);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_FEC_TYPE           ");
           if  (api->rsp->dvbt2_plp_info.plp_fec_type           ==     0) safe_strcat(msg,"16K_LDPC");
      else if  (api->rsp->dvbt2_plp_info.plp_fec_type           ==     1) safe_strcat(msg,"64K_LDPC");
      else                                                               safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.plp_fec_type);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_ROT                ");
           if  (api->rsp->dvbt2_plp_info.plp_rot                ==     0) safe_strcat(msg,"NOT_ROTATED");
      else if  (api->rsp->dvbt2_plp_info.plp_rot                ==     1) safe_strcat(msg,"ROTATED    ");
      else                                                               safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.plp_rot);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_MOD_LSB            "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.plp_mod_lsb);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-FRAME_INTERVAL_MSB     "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.frame_interval_msb);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_NUM_BLOCKS_MAX_LSB "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.plp_num_blocks_max_lsb);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TIME_IL_LENGTH_MSB     "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.time_il_length_msb);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-FRAME_INTERVAL_LSB     "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.frame_interval_lsb);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TIME_IL_TYPE           "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.time_il_type);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TIME_IL_LENGTH_LSB     "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.time_il_length_lsb);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RESERVED_1_1           "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.reserved_1_1);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-IN_BAND_B_FLAG         "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.in_band_b_flag);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-IN_BAND_A_FLAG         "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.in_band_a_flag);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-STATIC_FLAG            "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.static_flag);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_MODE               ");
           if  (api->rsp->dvbt2_plp_info.plp_mode               ==     2) safe_strcat(msg,"HIGH_EFFICIENCY_MODE");
      else if  (api->rsp->dvbt2_plp_info.plp_mode               ==     1) safe_strcat(msg,"NORMAL_MODE         ");
      else if  (api->rsp->dvbt2_plp_info.plp_mode               ==     0) safe_strcat(msg,"NOT_SPECIFIED       ");
      else if  (api->rsp->dvbt2_plp_info.plp_mode               ==     3) safe_strcat(msg,"RESERVED            ");
      else                                                               safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.plp_mode);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RESERVED_1_2           "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.reserved_1_2);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-STATIC_PADDING_FLAG    "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_info.static_padding_flag);
     break;
    #endif /*     Si2164B_DVBT2_PLP_INFO_CMD */

    #ifdef        Si2164B_DVBT2_PLP_SELECT_CMD
     case         Si2164B_DVBT2_PLP_SELECT_CMD_CODE:
      safe_sprintf(msg,"DVBT2_PLP_SELECT ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->dvbt2_plp_select.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbt2_plp_select.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                          safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_select.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->dvbt2_plp_select.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbt2_plp_select.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                          safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_select.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->dvbt2_plp_select.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dvbt2_plp_select.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                          safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_select.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->dvbt2_plp_select.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dvbt2_plp_select.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                          safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_plp_select.STATUS->cts);
     break;
    #endif /*     Si2164B_DVBT2_PLP_SELECT_CMD */

    #ifdef        Si2164B_DVBT2_STATUS_CMD
     case         Si2164B_DVBT2_STATUS_CMD_CODE:
      safe_sprintf(msg,"DVBT2_STATUS ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT         ");
           if  (api->rsp->dvbt2_status.STATUS->ddint         ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbt2_status.STATUS->ddint         ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT       ");
           if  (api->rsp->dvbt2_status.STATUS->scanint       ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbt2_status.STATUS->scanint       ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR           ");
           if  (api->rsp->dvbt2_status.STATUS->err           ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dvbt2_status.STATUS->err           ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS           ");
           if  (api->rsp->dvbt2_status.STATUS->cts           ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dvbt2_status.STATUS->cts           ==     0) safe_strcat(msg,"WAIT     ");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PCLINT        ");
           if  (api->rsp->dvbt2_status.pclint        ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbt2_status.pclint        ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.pclint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DLINT         ");
           if  (api->rsp->dvbt2_status.dlint         ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbt2_status.dlint         ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.dlint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BERINT        ");
           if  (api->rsp->dvbt2_status.berint        ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbt2_status.berint        ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.berint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-UNCORINT      ");
           if  (api->rsp->dvbt2_status.uncorint      ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbt2_status.uncorint      ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.uncorint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-NOTDVBT2INT   ");
           if  (api->rsp->dvbt2_status.notdvbt2int   ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbt2_status.notdvbt2int   ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.notdvbt2int);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PCL           ");
           if  (api->rsp->dvbt2_status.pcl           ==     1) safe_strcat(msg,"LOCKED ");
      else if  (api->rsp->dvbt2_status.pcl           ==     0) safe_strcat(msg,"NO_LOCK");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.pcl);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DL            ");
           if  (api->rsp->dvbt2_status.dl            ==     1) safe_strcat(msg,"LOCKED ");
      else if  (api->rsp->dvbt2_status.dl            ==     0) safe_strcat(msg,"NO_LOCK");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.dl);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BER           ");
           if  (api->rsp->dvbt2_status.ber           ==     1) safe_strcat(msg,"BER_ABOVE");
      else if  (api->rsp->dvbt2_status.ber           ==     0) safe_strcat(msg,"BER_BELOW");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.ber);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-UNCOR         ");
           if  (api->rsp->dvbt2_status.uncor         ==     0) safe_strcat(msg,"NO_UNCOR_FOUND");
      else if  (api->rsp->dvbt2_status.uncor         ==     1) safe_strcat(msg,"UNCOR_FOUND   ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.uncor);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-NOTDVBT2      ");
           if  (api->rsp->dvbt2_status.notdvbt2      ==     0) safe_strcat(msg,"DVBT2    ");
      else if  (api->rsp->dvbt2_status.notdvbt2      ==     1) safe_strcat(msg,"NOT_DVBT2");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.notdvbt2);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CNR           "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.cnr);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-AFC_FREQ      "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.afc_freq);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TIMING_OFFSET "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.timing_offset);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CONSTELLATION ");
           if  (api->rsp->dvbt2_status.constellation ==    10) safe_strcat(msg,"QAM128");
      else if  (api->rsp->dvbt2_status.constellation ==     7) safe_strcat(msg,"QAM16 ");
      else if  (api->rsp->dvbt2_status.constellation ==    11) safe_strcat(msg,"QAM256");
      else if  (api->rsp->dvbt2_status.constellation ==     8) safe_strcat(msg,"QAM32 ");
      else if  (api->rsp->dvbt2_status.constellation ==     9) safe_strcat(msg,"QAM64 ");
      else if  (api->rsp->dvbt2_status.constellation ==     3) safe_strcat(msg,"QPSK  ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.constellation);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SP_INV        ");
           if  (api->rsp->dvbt2_status.sp_inv        ==     1) safe_strcat(msg,"INVERTED");
      else if  (api->rsp->dvbt2_status.sp_inv        ==     0) safe_strcat(msg,"NORMAL  ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.sp_inv);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-FEF           ");
           if  (api->rsp->dvbt2_status.fef           ==     1) safe_strcat(msg,"FEF   ");
      else if  (api->rsp->dvbt2_status.fef           ==     0) safe_strcat(msg,"NO_FEF");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.fef);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-FFT_MODE      ");
           if  (api->rsp->dvbt2_status.fft_mode      ==    14) safe_strcat(msg,"16K");
      else if  (api->rsp->dvbt2_status.fft_mode      ==    10) safe_strcat(msg,"1K ");
      else if  (api->rsp->dvbt2_status.fft_mode      ==    11) safe_strcat(msg,"2K ");
      else if  (api->rsp->dvbt2_status.fft_mode      ==    15) safe_strcat(msg,"32K");
      else if  (api->rsp->dvbt2_status.fft_mode      ==    12) safe_strcat(msg,"4K ");
      else if  (api->rsp->dvbt2_status.fft_mode      ==    13) safe_strcat(msg,"8K ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.fft_mode);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-GUARD_INT     ");
           if  (api->rsp->dvbt2_status.guard_int     ==     6) safe_strcat(msg,"19_128");
      else if  (api->rsp->dvbt2_status.guard_int     ==     7) safe_strcat(msg,"19_256");
      else if  (api->rsp->dvbt2_status.guard_int     ==     5) safe_strcat(msg,"1_128 ");
      else if  (api->rsp->dvbt2_status.guard_int     ==     2) safe_strcat(msg,"1_16  ");
      else if  (api->rsp->dvbt2_status.guard_int     ==     1) safe_strcat(msg,"1_32  ");
      else if  (api->rsp->dvbt2_status.guard_int     ==     4) safe_strcat(msg,"1_4   ");
      else if  (api->rsp->dvbt2_status.guard_int     ==     3) safe_strcat(msg,"1_8   ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.guard_int);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BW_EXT        ");
           if  (api->rsp->dvbt2_status.bw_ext        ==     1) safe_strcat(msg,"EXTENDED");
      else if  (api->rsp->dvbt2_status.bw_ext        ==     0) safe_strcat(msg,"NORMAL  ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.bw_ext);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-NUM_PLP       "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.num_plp);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PILOT_PATTERN ");
           if  (api->rsp->dvbt2_status.pilot_pattern ==     0) safe_strcat(msg,"PP1");
      else if  (api->rsp->dvbt2_status.pilot_pattern ==     1) safe_strcat(msg,"PP2");
      else if  (api->rsp->dvbt2_status.pilot_pattern ==     2) safe_strcat(msg,"PP3");
      else if  (api->rsp->dvbt2_status.pilot_pattern ==     3) safe_strcat(msg,"PP4");
      else if  (api->rsp->dvbt2_status.pilot_pattern ==     4) safe_strcat(msg,"PP5");
      else if  (api->rsp->dvbt2_status.pilot_pattern ==     5) safe_strcat(msg,"PP6");
      else if  (api->rsp->dvbt2_status.pilot_pattern ==     6) safe_strcat(msg,"PP7");
      else if  (api->rsp->dvbt2_status.pilot_pattern ==     7) safe_strcat(msg,"PP8");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.pilot_pattern);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TX_MODE       ");
           if  (api->rsp->dvbt2_status.tx_mode       ==     1) safe_strcat(msg,"MISO");
      else if  (api->rsp->dvbt2_status.tx_mode       ==     0) safe_strcat(msg,"SISO");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.tx_mode);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ROTATED       ");
           if  (api->rsp->dvbt2_status.rotated       ==     0) safe_strcat(msg,"NORMAL ");
      else if  (api->rsp->dvbt2_status.rotated       ==     1) safe_strcat(msg,"ROTATED");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.rotated);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SHORT_FRAME   ");
           if  (api->rsp->dvbt2_status.short_frame   ==     0) safe_strcat(msg,"16K_LDPC");
      else if  (api->rsp->dvbt2_status.short_frame   ==     1) safe_strcat(msg,"64K_LDPC");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.short_frame);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-T2_MODE       ");
           if  (api->rsp->dvbt2_status.t2_mode       ==     0) safe_strcat(msg,"BASE");
      else if  (api->rsp->dvbt2_status.t2_mode       ==     1) safe_strcat(msg,"LITE");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.t2_mode);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CODE_RATE     ");
           if  (api->rsp->dvbt2_status.code_rate     ==     1) safe_strcat(msg,"1_2");
      else if  (api->rsp->dvbt2_status.code_rate     ==    10) safe_strcat(msg,"1_3");
      else if  (api->rsp->dvbt2_status.code_rate     ==     2) safe_strcat(msg,"2_3");
      else if  (api->rsp->dvbt2_status.code_rate     ==    12) safe_strcat(msg,"2_5");
      else if  (api->rsp->dvbt2_status.code_rate     ==     3) safe_strcat(msg,"3_4");
      else if  (api->rsp->dvbt2_status.code_rate     ==    13) safe_strcat(msg,"3_5");
      else if  (api->rsp->dvbt2_status.code_rate     ==     4) safe_strcat(msg,"4_5");
      else if  (api->rsp->dvbt2_status.code_rate     ==     5) safe_strcat(msg,"5_6");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.code_rate);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-T2_VERSION    ");
           if  (api->rsp->dvbt2_status.t2_version    ==     0) safe_strcat(msg,"1_1_1");
      else if  (api->rsp->dvbt2_status.t2_version    ==     1) safe_strcat(msg,"1_2_1");
      else if  (api->rsp->dvbt2_status.t2_version    ==     2) safe_strcat(msg,"1_3_1");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.t2_version);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PLP_ID        "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_status.plp_id);
     break;
    #endif /*     Si2164B_DVBT2_STATUS_CMD */

    #ifdef        Si2164B_DVBT2_TX_ID_CMD
     case         Si2164B_DVBT2_TX_ID_CMD_CODE:
      safe_sprintf(msg,"DVBT2_TX_ID ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT              ");
           if  (api->rsp->dvbt2_tx_id.STATUS->ddint              ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbt2_tx_id.STATUS->ddint              ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_tx_id.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT            ");
           if  (api->rsp->dvbt2_tx_id.STATUS->scanint            ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbt2_tx_id.STATUS->scanint            ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_tx_id.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR                ");
           if  (api->rsp->dvbt2_tx_id.STATUS->err                ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dvbt2_tx_id.STATUS->err                ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_tx_id.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS                ");
           if  (api->rsp->dvbt2_tx_id.STATUS->cts                ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dvbt2_tx_id.STATUS->cts                ==     0) safe_strcat(msg,"WAIT     ");
      else                                                                safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_tx_id.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TX_ID_AVAILABILITY "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_tx_id.tx_id_availability);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CELL_ID            "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_tx_id.cell_id);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-NETWORK_ID         "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_tx_id.network_id);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-T2_SYSTEM_ID       "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt2_tx_id.t2_system_id);
     break;
    #endif /*     Si2164B_DVBT2_TX_ID_CMD */


    #ifdef        Si2164B_DVBT_STATUS_CMD
     case         Si2164B_DVBT_STATUS_CMD_CODE:
      safe_sprintf(msg,"DVBT_STATUS ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT         ");
           if  (api->rsp->dvbt_status.STATUS->ddint         ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbt_status.STATUS->ddint         ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                           safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT       ");
           if  (api->rsp->dvbt_status.STATUS->scanint       ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbt_status.STATUS->scanint       ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                           safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR           ");
           if  (api->rsp->dvbt_status.STATUS->err           ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dvbt_status.STATUS->err           ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                           safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS           ");
           if  (api->rsp->dvbt_status.STATUS->cts           ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dvbt_status.STATUS->cts           ==     0) safe_strcat(msg,"WAIT     ");
      else                                                           safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PCLINT        ");
           if  (api->rsp->dvbt_status.pclint        ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbt_status.pclint        ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.pclint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DLINT         ");
           if  (api->rsp->dvbt_status.dlint         ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbt_status.dlint         ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.dlint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BERINT        ");
           if  (api->rsp->dvbt_status.berint        ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbt_status.berint        ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.berint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-UNCORINT      ");
           if  (api->rsp->dvbt_status.uncorint      ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbt_status.uncorint      ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.uncorint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-NOTDVBTINT    ");
           if  (api->rsp->dvbt_status.notdvbtint    ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->dvbt_status.notdvbtint    ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.notdvbtint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PCL           ");
           if  (api->rsp->dvbt_status.pcl           ==     1) safe_strcat(msg,"LOCKED ");
      else if  (api->rsp->dvbt_status.pcl           ==     0) safe_strcat(msg,"NO_LOCK");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.pcl);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DL            ");
           if  (api->rsp->dvbt_status.dl            ==     1) safe_strcat(msg,"LOCKED ");
      else if  (api->rsp->dvbt_status.dl            ==     0) safe_strcat(msg,"NO_LOCK");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.dl);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BER           ");
           if  (api->rsp->dvbt_status.ber           ==     1) safe_strcat(msg,"BER_ABOVE");
      else if  (api->rsp->dvbt_status.ber           ==     0) safe_strcat(msg,"BER_BELOW");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.ber);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-UNCOR         ");
           if  (api->rsp->dvbt_status.uncor         ==     0) safe_strcat(msg,"NO_UNCOR_FOUND");
      else if  (api->rsp->dvbt_status.uncor         ==     1) safe_strcat(msg,"UNCOR_FOUND   ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.uncor);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-NOTDVBT       ");
           if  (api->rsp->dvbt_status.notdvbt       ==     0) safe_strcat(msg,"DVBT    ");
      else if  (api->rsp->dvbt_status.notdvbt       ==     1) safe_strcat(msg,"NOT_DVBT");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.notdvbt);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CNR           "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.cnr);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-AFC_FREQ      "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.afc_freq);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TIMING_OFFSET "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.timing_offset);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CONSTELLATION ");
           if  (api->rsp->dvbt_status.constellation ==     7) safe_strcat(msg,"QAM16");
      else if  (api->rsp->dvbt_status.constellation ==     9) safe_strcat(msg,"QAM64");
      else if  (api->rsp->dvbt_status.constellation ==     3) safe_strcat(msg,"QPSK ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.constellation);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SP_INV        ");
           if  (api->rsp->dvbt_status.sp_inv        ==     1) safe_strcat(msg,"INVERTED");
      else if  (api->rsp->dvbt_status.sp_inv        ==     0) safe_strcat(msg,"NORMAL  ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.sp_inv);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RATE_HP       ");
           if  (api->rsp->dvbt_status.rate_hp       ==     1) safe_strcat(msg,"1_2");
      else if  (api->rsp->dvbt_status.rate_hp       ==     2) safe_strcat(msg,"2_3");
      else if  (api->rsp->dvbt_status.rate_hp       ==     3) safe_strcat(msg,"3_4");
      else if  (api->rsp->dvbt_status.rate_hp       ==     5) safe_strcat(msg,"5_6");
      else if  (api->rsp->dvbt_status.rate_hp       ==     7) safe_strcat(msg,"7_8");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.rate_hp);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RATE_LP       ");
           if  (api->rsp->dvbt_status.rate_lp       ==     1) safe_strcat(msg,"1_2");
      else if  (api->rsp->dvbt_status.rate_lp       ==     2) safe_strcat(msg,"2_3");
      else if  (api->rsp->dvbt_status.rate_lp       ==     3) safe_strcat(msg,"3_4");
      else if  (api->rsp->dvbt_status.rate_lp       ==     5) safe_strcat(msg,"5_6");
      else if  (api->rsp->dvbt_status.rate_lp       ==     7) safe_strcat(msg,"7_8");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.rate_lp);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-FFT_MODE      ");
           if  (api->rsp->dvbt_status.fft_mode      ==    11) safe_strcat(msg,"2K");
      else if  (api->rsp->dvbt_status.fft_mode      ==    12) safe_strcat(msg,"4K");
      else if  (api->rsp->dvbt_status.fft_mode      ==    13) safe_strcat(msg,"8K");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.fft_mode);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-GUARD_INT     ");
           if  (api->rsp->dvbt_status.guard_int     ==     2) safe_strcat(msg,"1_16");
      else if  (api->rsp->dvbt_status.guard_int     ==     1) safe_strcat(msg,"1_32");
      else if  (api->rsp->dvbt_status.guard_int     ==     4) safe_strcat(msg,"1_4 ");
      else if  (api->rsp->dvbt_status.guard_int     ==     3) safe_strcat(msg,"1_8 ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.guard_int);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-HIERARCHY     ");
           if  (api->rsp->dvbt_status.hierarchy     ==     2) safe_strcat(msg,"ALFA1");
      else if  (api->rsp->dvbt_status.hierarchy     ==     3) safe_strcat(msg,"ALFA2");
      else if  (api->rsp->dvbt_status.hierarchy     ==     5) safe_strcat(msg,"ALFA4");
      else if  (api->rsp->dvbt_status.hierarchy     ==     1) safe_strcat(msg,"NONE ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.hierarchy);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TPS_LENGTH    "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_status.tps_length);
     break;
    #endif /*     Si2164B_DVBT_STATUS_CMD */

    #ifdef        Si2164B_DVBT_TPS_EXTRA_CMD
     case         Si2164B_DVBT_TPS_EXTRA_CMD_CODE:
      safe_sprintf(msg,"DVBT_TPS_EXTRA ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT       ");
           if  (api->rsp->dvbt_tps_extra.STATUS->ddint       ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbt_tps_extra.STATUS->ddint       ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_tps_extra.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT     ");
           if  (api->rsp->dvbt_tps_extra.STATUS->scanint     ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->dvbt_tps_extra.STATUS->scanint     ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_tps_extra.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR         ");
           if  (api->rsp->dvbt_tps_extra.STATUS->err         ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->dvbt_tps_extra.STATUS->err         ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_tps_extra.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS         ");
           if  (api->rsp->dvbt_tps_extra.STATUS->cts         ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->dvbt_tps_extra.STATUS->cts         ==     0) safe_strcat(msg,"WAIT     ");
      else                                                            safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_tps_extra.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-LPTIMESLICE ");
           if  (api->rsp->dvbt_tps_extra.lptimeslice ==     0) safe_strcat(msg,"off");
      else if  (api->rsp->dvbt_tps_extra.lptimeslice ==     1) safe_strcat(msg,"on ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_tps_extra.lptimeslice);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-HPTIMESLICE ");
           if  (api->rsp->dvbt_tps_extra.hptimeslice ==     0) safe_strcat(msg,"off");
      else if  (api->rsp->dvbt_tps_extra.hptimeslice ==     1) safe_strcat(msg,"on ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_tps_extra.hptimeslice);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-LPMPEFEC    ");
           if  (api->rsp->dvbt_tps_extra.lpmpefec    ==     0) safe_strcat(msg,"off");
      else if  (api->rsp->dvbt_tps_extra.lpmpefec    ==     1) safe_strcat(msg,"on ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_tps_extra.lpmpefec);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-HPMPEFEC    ");
           if  (api->rsp->dvbt_tps_extra.hpmpefec    ==     0) safe_strcat(msg,"off");
      else if  (api->rsp->dvbt_tps_extra.hpmpefec    ==     1) safe_strcat(msg,"on ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_tps_extra.hpmpefec);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DVBHINTER   ");
           if  (api->rsp->dvbt_tps_extra.dvbhinter   ==     1) safe_strcat(msg,"in_depth");
      else if  (api->rsp->dvbt_tps_extra.dvbhinter   ==     0) safe_strcat(msg,"native  ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_tps_extra.dvbhinter);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CELL_ID     "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_tps_extra.cell_id);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TPS_RES1    "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_tps_extra.tps_res1);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TPS_RES2    "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_tps_extra.tps_res2);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TPS_RES3    "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_tps_extra.tps_res3);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TPS_RES4    "); safe_sprintf(msg,"%s%d", msg, api->rsp->dvbt_tps_extra.tps_res4);
     break;
    #endif /*     Si2164B_DVBT_TPS_EXTRA_CMD */


    #ifdef        Si2164B_EXIT_BOOTLOADER_CMD
     case         Si2164B_EXIT_BOOTLOADER_CMD_CODE:
      safe_sprintf(msg,"EXIT_BOOTLOADER ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->exit_bootloader.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->exit_bootloader.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->exit_bootloader.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->exit_bootloader.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->exit_bootloader.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->exit_bootloader.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->exit_bootloader.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->exit_bootloader.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->exit_bootloader.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->exit_bootloader.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->exit_bootloader.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->exit_bootloader.STATUS->cts);
     break;
    #endif /*     Si2164B_EXIT_BOOTLOADER_CMD */

    #ifdef        Si2164B_GET_PROPERTY_CMD
     case         Si2164B_GET_PROPERTY_CMD_CODE:
      safe_sprintf(msg,"GET_PROPERTY ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT    ");
           if  (api->rsp->get_property.STATUS->ddint    ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->get_property.STATUS->ddint    ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                       safe_sprintf(msg,"%s%d", msg, api->rsp->get_property.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT  ");
           if  (api->rsp->get_property.STATUS->scanint  ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->get_property.STATUS->scanint  ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                       safe_sprintf(msg,"%s%d", msg, api->rsp->get_property.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR      ");
           if  (api->rsp->get_property.STATUS->err      ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->get_property.STATUS->err      ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                       safe_sprintf(msg,"%s%d", msg, api->rsp->get_property.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS      ");
           if  (api->rsp->get_property.STATUS->cts      ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->get_property.STATUS->cts      ==     0) safe_strcat(msg,"WAIT     ");
      else                                                       safe_sprintf(msg,"%s%d", msg, api->rsp->get_property.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RESERVED "); safe_sprintf(msg,"%s%d", msg, api->rsp->get_property.reserved);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DATA     "); safe_sprintf(msg,"%s%d", msg, api->rsp->get_property.data);
     break;
    #endif /*     Si2164B_GET_PROPERTY_CMD */

    #ifdef        Si2164B_GET_REV_CMD
     case         Si2164B_GET_REV_CMD_CODE:
      safe_sprintf(msg,"GET_REV ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT    ");
           if  (api->rsp->get_rev.STATUS->ddint    ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->get_rev.STATUS->ddint    ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->get_rev.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT  ");
           if  (api->rsp->get_rev.STATUS->scanint  ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->get_rev.STATUS->scanint  ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->get_rev.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR      ");
           if  (api->rsp->get_rev.STATUS->err      ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->get_rev.STATUS->err      ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->get_rev.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS      ");
           if  (api->rsp->get_rev.STATUS->cts      ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->get_rev.STATUS->cts      ==     0) safe_strcat(msg,"WAIT     ");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->get_rev.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PN       "); safe_sprintf(msg,"%s%d", msg, api->rsp->get_rev.pn);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-FWMAJOR  "); safe_sprintf(msg,"%s%d", msg, api->rsp->get_rev.fwmajor);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-FWMINOR  "); safe_sprintf(msg,"%s%d", msg, api->rsp->get_rev.fwminor);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PATCH    "); safe_sprintf(msg,"%s%d", msg, api->rsp->get_rev.patch);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CMPMAJOR "); safe_sprintf(msg,"%s%d", msg, api->rsp->get_rev.cmpmajor);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CMPMINOR "); safe_sprintf(msg,"%s%d", msg, api->rsp->get_rev.cmpminor);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CMPBUILD "); safe_sprintf(msg,"%s%d", msg, api->rsp->get_rev.cmpbuild);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CHIPREV  ");
           if  (api->rsp->get_rev.chiprev  ==     1) safe_strcat(msg,"A");
      else if  (api->rsp->get_rev.chiprev  ==     2) safe_strcat(msg,"B");
      else if  (api->rsp->get_rev.chiprev  ==     3) safe_strcat(msg,"C");
      else if  (api->rsp->get_rev.chiprev  ==     4) safe_strcat(msg,"D");
      else if  (api->rsp->get_rev.chiprev  ==     5) safe_strcat(msg,"E");
      else                                          safe_sprintf(msg,"%s%d", msg, api->rsp->get_rev.chiprev);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-MCM_DIE  ");
           if  (api->rsp->get_rev.mcm_die  ==     1) safe_strcat(msg,"DIE_A ");
      else if  (api->rsp->get_rev.mcm_die  ==     2) safe_strcat(msg,"DIE_B ");
      else if  (api->rsp->get_rev.mcm_die  ==     0) safe_strcat(msg,"SINGLE");
      else                                          safe_sprintf(msg,"%s%d", msg, api->rsp->get_rev.mcm_die);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RX       ");
           if  (api->rsp->get_rev.rx       ==     1) safe_strcat(msg,"RX        ");
      else if  (api->rsp->get_rev.rx       ==     0) safe_strcat(msg,"STANDALONE");
      else                                          safe_sprintf(msg,"%s%d", msg, api->rsp->get_rev.rx);
     break;
    #endif /*     Si2164B_GET_REV_CMD */

    #ifdef        Si2164B_I2C_PASSTHROUGH_CMD
     case         Si2164B_I2C_PASSTHROUGH_CMD_CODE:
      safe_sprintf(msg,"I2C_PASSTHROUGH ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->i2c_passthrough.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->i2c_passthrough.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->i2c_passthrough.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->i2c_passthrough.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->i2c_passthrough.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->i2c_passthrough.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->i2c_passthrough.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->i2c_passthrough.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->i2c_passthrough.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->i2c_passthrough.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->i2c_passthrough.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->i2c_passthrough.STATUS->cts);
     break;
    #endif /*     Si2164B_I2C_PASSTHROUGH_CMD */


    #ifdef        Si2164B_MCNS_STATUS_CMD
     case         Si2164B_MCNS_STATUS_CMD_CODE:
      safe_sprintf(msg,"MCNS_STATUS ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT         ");
           if  (api->rsp->mcns_status.STATUS->ddint         ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->mcns_status.STATUS->ddint         ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                           safe_sprintf(msg,"%s%d", msg, api->rsp->mcns_status.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT       ");
           if  (api->rsp->mcns_status.STATUS->scanint       ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->mcns_status.STATUS->scanint       ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                           safe_sprintf(msg,"%s%d", msg, api->rsp->mcns_status.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR           ");
           if  (api->rsp->mcns_status.STATUS->err           ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->mcns_status.STATUS->err           ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                           safe_sprintf(msg,"%s%d", msg, api->rsp->mcns_status.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS           ");
           if  (api->rsp->mcns_status.STATUS->cts           ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->mcns_status.STATUS->cts           ==     0) safe_strcat(msg,"WAIT     ");
      else                                                           safe_sprintf(msg,"%s%d", msg, api->rsp->mcns_status.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PCLINT        ");
           if  (api->rsp->mcns_status.pclint        ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->mcns_status.pclint        ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->mcns_status.pclint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DLINT         ");
           if  (api->rsp->mcns_status.dlint         ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->mcns_status.dlint         ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->mcns_status.dlint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BERINT        ");
           if  (api->rsp->mcns_status.berint        ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->mcns_status.berint        ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->mcns_status.berint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-UNCORINT      ");
           if  (api->rsp->mcns_status.uncorint      ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->mcns_status.uncorint      ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->mcns_status.uncorint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PCL           ");
           if  (api->rsp->mcns_status.pcl           ==     1) safe_strcat(msg,"LOCKED ");
      else if  (api->rsp->mcns_status.pcl           ==     0) safe_strcat(msg,"NO_LOCK");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->mcns_status.pcl);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DL            ");
           if  (api->rsp->mcns_status.dl            ==     1) safe_strcat(msg,"LOCKED ");
      else if  (api->rsp->mcns_status.dl            ==     0) safe_strcat(msg,"NO_LOCK");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->mcns_status.dl);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BER           ");
           if  (api->rsp->mcns_status.ber           ==     1) safe_strcat(msg,"BER_ABOVE");
      else if  (api->rsp->mcns_status.ber           ==     0) safe_strcat(msg,"BER_BELOW");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->mcns_status.ber);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-UNCOR         ");
           if  (api->rsp->mcns_status.uncor         ==     0) safe_strcat(msg,"NO_UNCOR_FOUND");
      else if  (api->rsp->mcns_status.uncor         ==     1) safe_strcat(msg,"UNCOR_FOUND   ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->mcns_status.uncor);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CNR           "); safe_sprintf(msg,"%s%d", msg, api->rsp->mcns_status.cnr);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-AFC_FREQ      "); safe_sprintf(msg,"%s%d", msg, api->rsp->mcns_status.afc_freq);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-TIMING_OFFSET "); safe_sprintf(msg,"%s%d", msg, api->rsp->mcns_status.timing_offset);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CONSTELLATION ");
           if  (api->rsp->mcns_status.constellation ==    11) safe_strcat(msg,"QAM256");
      else if  (api->rsp->mcns_status.constellation ==     9) safe_strcat(msg,"QAM64 ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->mcns_status.constellation);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SP_INV        ");
           if  (api->rsp->mcns_status.sp_inv        ==     1) safe_strcat(msg,"INVERTED");
      else if  (api->rsp->mcns_status.sp_inv        ==     0) safe_strcat(msg,"NORMAL  ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->mcns_status.sp_inv);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-INTERLEAVING  ");
           if  (api->rsp->mcns_status.interleaving  ==     0) safe_strcat(msg,"0__128_1    ");
      else if  (api->rsp->mcns_status.interleaving  ==    10) safe_strcat(msg,"10__128_6   ");
      else if  (api->rsp->mcns_status.interleaving  ==    11) safe_strcat(msg,"11__RESERVED");
      else if  (api->rsp->mcns_status.interleaving  ==    12) safe_strcat(msg,"12__128_7   ");
      else if  (api->rsp->mcns_status.interleaving  ==    13) safe_strcat(msg,"13__RESERVED");
      else if  (api->rsp->mcns_status.interleaving  ==    14) safe_strcat(msg,"14__128_8   ");
      else if  (api->rsp->mcns_status.interleaving  ==    15) safe_strcat(msg,"15__RESERVED");
      else if  (api->rsp->mcns_status.interleaving  ==     1) safe_strcat(msg,"1__128_1    ");
      else if  (api->rsp->mcns_status.interleaving  ==     2) safe_strcat(msg,"2__128_2    ");
      else if  (api->rsp->mcns_status.interleaving  ==     3) safe_strcat(msg,"3__64_2     ");
      else if  (api->rsp->mcns_status.interleaving  ==     4) safe_strcat(msg,"4__128_3    ");
      else if  (api->rsp->mcns_status.interleaving  ==     5) safe_strcat(msg,"5__32_4     ");
      else if  (api->rsp->mcns_status.interleaving  ==     6) safe_strcat(msg,"6__128_4    ");
      else if  (api->rsp->mcns_status.interleaving  ==     7) safe_strcat(msg,"7__16_8     ");
      else if  (api->rsp->mcns_status.interleaving  ==     8) safe_strcat(msg,"8__128_5    ");
      else if  (api->rsp->mcns_status.interleaving  ==     9) safe_strcat(msg,"9__8_16     ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->mcns_status.interleaving);
     break;
    #endif /*     Si2164B_MCNS_STATUS_CMD */


    #ifdef        Si2164B_PART_INFO_CMD
     case         Si2164B_PART_INFO_CMD_CODE:
      safe_sprintf(msg,"PART_INFO ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT    ");
           if  (api->rsp->part_info.STATUS->ddint    ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->part_info.STATUS->ddint    ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->part_info.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT  ");
           if  (api->rsp->part_info.STATUS->scanint  ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->part_info.STATUS->scanint  ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->part_info.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR      ");
           if  (api->rsp->part_info.STATUS->err      ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->part_info.STATUS->err      ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->part_info.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS      ");
           if  (api->rsp->part_info.STATUS->cts      ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->part_info.STATUS->cts      ==     0) safe_strcat(msg,"WAIT     ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->part_info.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CHIPREV  ");
           if  (api->rsp->part_info.chiprev  ==     1) safe_strcat(msg,"A");
      else if  (api->rsp->part_info.chiprev  ==     2) safe_strcat(msg,"B");
      else if  (api->rsp->part_info.chiprev  ==     3) safe_strcat(msg,"C");
      else if  (api->rsp->part_info.chiprev  ==     4) safe_strcat(msg,"D");
      else if  (api->rsp->part_info.chiprev  ==     5) safe_strcat(msg,"E");
      else                                            safe_sprintf(msg,"%s%d", msg, api->rsp->part_info.chiprev);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PART     "); safe_sprintf(msg,"%s%d", msg, api->rsp->part_info.part);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PMAJOR   "); safe_sprintf(msg,"%s%d", msg, api->rsp->part_info.pmajor);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PMINOR   "); safe_sprintf(msg,"%s%d", msg, api->rsp->part_info.pminor);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-PBUILD   "); safe_sprintf(msg,"%s%d", msg, api->rsp->part_info.pbuild);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RESERVED "); safe_sprintf(msg,"%s%d", msg, api->rsp->part_info.reserved);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SERIAL   "); safe_sprintf(msg,"%s%ld", msg, api->rsp->part_info.serial);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ROMID    "); safe_sprintf(msg,"%s%d", msg, api->rsp->part_info.romid);
     break;
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RX       ");
           if  (api->rsp->part_info.rx       ==     1) safe_strcat(msg,"RX        ");
      else if  (api->rsp->part_info.rx       ==     0) safe_strcat(msg,"STANDALONE");
      else                                            safe_sprintf(msg,"%s%d", msg, api->rsp->part_info.rx);
     break;
    #endif /*     Si2164B_PART_INFO_CMD */

    #ifdef        Si2164B_POWER_DOWN_CMD
     case         Si2164B_POWER_DOWN_CMD_CODE:
      safe_sprintf(msg,"POWER_DOWN ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->power_down.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->power_down.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->power_down.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->power_down.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->power_down.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->power_down.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->power_down.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->power_down.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->power_down.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->power_down.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->power_down.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                    safe_sprintf(msg,"%s%d", msg, api->rsp->power_down.STATUS->cts);
     break;
    #endif /*     Si2164B_POWER_DOWN_CMD */

    #ifdef        Si2164B_POWER_UP_CMD
     case         Si2164B_POWER_UP_CMD_CODE:
      safe_sprintf(msg,"POWER_UP ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->power_up.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->power_up.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->power_up.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->power_up.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->power_up.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->power_up.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->power_up.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->power_up.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->power_up.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->power_up.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->power_up.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->power_up.STATUS->cts);
     break;
    #endif /*     Si2164B_POWER_UP_CMD */

    #ifdef        Si2164B_RSSI_ADC_CMD
     case         Si2164B_RSSI_ADC_CMD_CODE:
      safe_sprintf(msg,"RSSI_ADC ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->rssi_adc.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->rssi_adc.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->rssi_adc.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->rssi_adc.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->rssi_adc.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->rssi_adc.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->rssi_adc.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->rssi_adc.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->rssi_adc.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->rssi_adc.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->rssi_adc.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->rssi_adc.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-LEVEL   "); safe_sprintf(msg,"%s%d", msg, api->rsp->rssi_adc.level);
     break;
    #endif /*     Si2164B_RSSI_ADC_CMD */

    #ifdef        Si2164B_SCAN_CTRL_CMD
     case         Si2164B_SCAN_CTRL_CMD_CODE:
      safe_sprintf(msg,"SCAN_CTRL ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->scan_ctrl.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->scan_ctrl.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->scan_ctrl.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->scan_ctrl.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->scan_ctrl.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->scan_ctrl.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->scan_ctrl.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->scan_ctrl.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->scan_ctrl.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->scan_ctrl.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->scan_ctrl.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->scan_ctrl.STATUS->cts);
     break;
    #endif /*     Si2164B_SCAN_CTRL_CMD */

    #ifdef        Si2164B_SCAN_STATUS_CMD
     case         Si2164B_SCAN_STATUS_CMD_CODE:
      safe_sprintf(msg,"SCAN_STATUS ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT       ");
           if  (api->rsp->scan_status.STATUS->ddint       ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->scan_status.STATUS->ddint       ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->scan_status.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT     ");
           if  (api->rsp->scan_status.STATUS->scanint     ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->scan_status.STATUS->scanint     ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->scan_status.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR         ");
           if  (api->rsp->scan_status.STATUS->err         ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->scan_status.STATUS->err         ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->scan_status.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS         ");
           if  (api->rsp->scan_status.STATUS->cts         ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->scan_status.STATUS->cts         ==     0) safe_strcat(msg,"WAIT     ");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->scan_status.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BUZINT      ");
           if  (api->rsp->scan_status.buzint      ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->scan_status.buzint      ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                 safe_sprintf(msg,"%s%d", msg, api->rsp->scan_status.buzint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-REQINT      ");
           if  (api->rsp->scan_status.reqint      ==     1) safe_strcat(msg,"CHANGED  ");
      else if  (api->rsp->scan_status.reqint      ==     0) safe_strcat(msg,"NO_CHANGE");
      else                                                 safe_sprintf(msg,"%s%d", msg, api->rsp->scan_status.reqint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-BUZ         ");
           if  (api->rsp->scan_status.buz         ==     1) safe_strcat(msg,"BUSY");
      else if  (api->rsp->scan_status.buz         ==     0) safe_strcat(msg,"CTS ");
      else                                                 safe_sprintf(msg,"%s%d", msg, api->rsp->scan_status.buz);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-REQ         ");
           if  (api->rsp->scan_status.req         ==     0) safe_strcat(msg,"NO_REQUEST");
      else if  (api->rsp->scan_status.req         ==     1) safe_strcat(msg,"REQUEST   ");
      else                                                 safe_sprintf(msg,"%s%d", msg, api->rsp->scan_status.req);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCAN_STATUS ");
           if  (api->rsp->scan_status.scan_status ==     6) safe_strcat(msg,"ANALOG_CHANNEL_FOUND ");
      else if  (api->rsp->scan_status.scan_status ==    63) safe_strcat(msg,"DEBUG                ");
      else if  (api->rsp->scan_status.scan_status ==     5) safe_strcat(msg,"DIGITAL_CHANNEL_FOUND");
      else if  (api->rsp->scan_status.scan_status ==     2) safe_strcat(msg,"ENDED                ");
      else if  (api->rsp->scan_status.scan_status ==     3) safe_strcat(msg,"ERROR                ");
      else if  (api->rsp->scan_status.scan_status ==     0) safe_strcat(msg,"IDLE                 ");
      else if  (api->rsp->scan_status.scan_status ==     1) safe_strcat(msg,"SEARCHING            ");
      else if  (api->rsp->scan_status.scan_status ==     4) safe_strcat(msg,"TUNE_REQUEST         ");
      else                                                 safe_sprintf(msg,"%s%d", msg, api->rsp->scan_status.scan_status);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RF_FREQ     "); safe_sprintf(msg,"%s%ld", msg, api->rsp->scan_status.rf_freq);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SYMB_RATE   "); safe_sprintf(msg,"%s%d", msg, api->rsp->scan_status.symb_rate);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-MODULATION  ");
           if  (api->rsp->scan_status.modulation  ==    10) safe_strcat(msg,"DSS  ");
      else if  (api->rsp->scan_status.modulation  ==     3) safe_strcat(msg,"DVBC ");
      else if  (api->rsp->scan_status.modulation  ==    11) safe_strcat(msg,"DVBC2");
      else if  (api->rsp->scan_status.modulation  ==     8) safe_strcat(msg,"DVBS ");
      else if  (api->rsp->scan_status.modulation  ==     9) safe_strcat(msg,"DVBS2");
      else if  (api->rsp->scan_status.modulation  ==     2) safe_strcat(msg,"DVBT ");
      else if  (api->rsp->scan_status.modulation  ==     7) safe_strcat(msg,"DVBT2");
      else if  (api->rsp->scan_status.modulation  ==     4) safe_strcat(msg,"ISDBT");
      else if  (api->rsp->scan_status.modulation  ==     1) safe_strcat(msg,"MCNS ");
      else                                                 safe_sprintf(msg,"%s%d", msg, api->rsp->scan_status.modulation);
     break;
    #endif /*     Si2164B_SCAN_STATUS_CMD */

    #ifdef        Si2164B_SET_PROPERTY_CMD
     case         Si2164B_SET_PROPERTY_CMD_CODE:
      safe_sprintf(msg,"SET_PROPERTY ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT    ");
           if  (api->rsp->set_property.STATUS->ddint    ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->set_property.STATUS->ddint    ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                       safe_sprintf(msg,"%s%d", msg, api->rsp->set_property.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT  ");
           if  (api->rsp->set_property.STATUS->scanint  ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->set_property.STATUS->scanint  ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                       safe_sprintf(msg,"%s%d", msg, api->rsp->set_property.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR      ");
           if  (api->rsp->set_property.STATUS->err      ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->set_property.STATUS->err      ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                       safe_sprintf(msg,"%s%d", msg, api->rsp->set_property.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS      ");
           if  (api->rsp->set_property.STATUS->cts      ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->set_property.STATUS->cts      ==     0) safe_strcat(msg,"WAIT     ");
      else                                                       safe_sprintf(msg,"%s%d", msg, api->rsp->set_property.STATUS->cts);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-RESERVED "); safe_sprintf(msg,"%s%d", msg, api->rsp->set_property.reserved);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DATA     "); safe_sprintf(msg,"%s%d", msg, api->rsp->set_property.data);
     break;
    #endif /*     Si2164B_SET_PROPERTY_CMD */

    #ifdef        Si2164B_SPI_LINK_CMD
     case         Si2164B_SPI_LINK_CMD_CODE:
      safe_sprintf(msg,"SPI_LINK ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->spi_link.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->spi_link.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->spi_link.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->spi_link.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->spi_link.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->spi_link.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->spi_link.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->spi_link.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->spi_link.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->spi_link.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->spi_link.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                  safe_sprintf(msg,"%s%d", msg, api->rsp->spi_link.STATUS->cts);
     break;
    #endif /*     Si2164B_SPI_LINK_CMD */

    #ifdef        Si2164B_SPI_PASSTHROUGH_CMD
     case         Si2164B_SPI_PASSTHROUGH_CMD_CODE:
      safe_sprintf(msg,"SPI_PASSTHROUGH ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->spi_passthrough.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->spi_passthrough.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->spi_passthrough.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->spi_passthrough.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->spi_passthrough.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->spi_passthrough.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->spi_passthrough.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->spi_passthrough.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->spi_passthrough.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->spi_passthrough.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->spi_passthrough.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                         safe_sprintf(msg,"%s%d", msg, api->rsp->spi_passthrough.STATUS->cts);
     break;
    #endif /*     Si2164B_SPI_PASSTHROUGH_CMD */

    #ifdef        Si2164B_START_CLK_CMD
     case         Si2164B_START_CLK_CMD_CODE:
      safe_sprintf(msg,"START_CLK ");
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-DDINT   ");
           if  (api->rsp->start_clk.STATUS->ddint   ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->start_clk.STATUS->ddint   ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->start_clk.STATUS->ddint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-SCANINT ");
           if  (api->rsp->start_clk.STATUS->scanint ==     0) safe_strcat(msg,"NOT_TRIGGERED");
      else if  (api->rsp->start_clk.STATUS->scanint ==     1) safe_strcat(msg,"TRIGGERED    ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->start_clk.STATUS->scanint);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-ERR     ");
           if  (api->rsp->start_clk.STATUS->err     ==     1) safe_strcat(msg,"ERROR   ");
      else if  (api->rsp->start_clk.STATUS->err     ==     0) safe_strcat(msg,"NO_ERROR");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->start_clk.STATUS->err);
       safe_sprintf(msg,"%s%s",msg,separator); safe_strcat(msg,"-CTS     ");
           if  (api->rsp->start_clk.STATUS->cts     ==     1) safe_strcat(msg,"COMPLETED");
      else if  (api->rsp->start_clk.STATUS->cts     ==     0) safe_strcat(msg,"WAIT     ");
      else                                                   safe_sprintf(msg,"%s%d", msg, api->rsp->start_clk.STATUS->cts);
     break;
    #endif /*     Si2164B_START_CLK_CMD */

     default : break;
    }

#endif    
    return 0;
  }
#endif /* Si2164B_GET_COMMAND_STRINGS */








