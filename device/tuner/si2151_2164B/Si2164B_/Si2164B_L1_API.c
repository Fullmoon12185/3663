/*************************************************************************************
                  Silicon Laboratories Broadcast Si2164B Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API functions definitions used by commands and properties
   FILE: Si2164B_L1_API.c
   Supported IC : Si2164B
   Compiled for ROM 2 firmware 5_B_build_1
   Revision: V0.1.1.0
   Tag:  V0.1.1.0
   Date: March 30 2015
  (C) Copyright 2015, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
/* Change log:

 As from V0.0.7.0: <new_feature>[SPI/logs] In Si2164B_L1_API_Init: setting api->spi_download_ms and api->i2c_download_ms to 0;

 As from V0.0.5.0:
  <improvement>[Code_size] Using the textBuffer in Si2164B_L1_Context when filling text strings
    In Si2164B_L1_API_Init

 As from V0.0.1.0:
  <new_feature/TESTPIPE> adding access to plp_info and tx_id for demod in T2
  <improvement>[SPI FW download] In Si2164B_L1_API_Init: Settings adapted to SiLabs SOCKET EVB
   (which is wired to allow SPI download)

 As from V0.0.0:
  Initial version (based on Si2164 code V0.3.4)
**************************************************************************************/
#define   Si2164B_COMMAND_PROTOTYPES

/* Before including the headers, define SiLevel and SiTAG */
//#define   SiLEVEL          1
//#define   SiTAG            api->i2c->tag
#include "Si2164B_Platform_Definition.h"

/***********************************************************************************************************************
  Si2164B_L1_API_Init function
  Use:        software initialisation function
              Used to initialize the software context
  Returns:    0 if no error
  Comments:   It should be called first and once only when starting the application
  Parameter:   **ppapi         a pointer to the api context to initialize
  Parameter:  add            the Si2164B I2C address
  Porting:    Allocation errors need to be properly managed.
  Porting:    I2C initialization needs to be adapted to use the available I2C functions
 ***********************************************************************************************************************/
unsigned char    Si2164B_L1_API_Init      (L1_Si2164B_Context *api, signed int add) {
    api->i2c = &(api->i2cObj);

    L0_Init(api->i2c);
    L0_SetAddress(api->i2c, add, 0);

    api->cmd               = &(api->cmdObj);
    api->rsp               = &(api->rspObj);
    api->prop              = &(api->propObj);
    api->status            = &(api->statusObj);
    api->propShadow        = &(api->propShadowObj);
    /* Set the propertyWriteMode to Si2164B_DOWNLOAD_ON_CHANGE to only download property settings on change (recommended) */
    /*      if propertyWriteMode is set to Si2164B_DOWNLOAD_ALWAYS the properties will be downloaded regardless of change */
    api->propertyWriteMode = Si2164B_DOWNLOAD_ON_CHANGE;
    /* SPI download default values */
    api->spi_download    = 0;
    api->spi_download_ms = 0;
    api->i2c_download_ms = 0;
    /* <porting> Set these values to match your SPI HW, or use 'SiLabs_API_SPI_Setup' to set them later on */
    api->spi_send_option = 0x01;
    api->spi_clk_pin     = Si2164B_SPI_LINK_CMD_SPI_CONF_CLK_DISEQC_CMD;
    api->spi_clk_pola    = Si2164B_SPI_LINK_CMD_SPI_CLK_POLA_RISING;
    api->spi_data_pin    = Si2164B_SPI_LINK_CMD_SPI_CONF_DATA_MP_C;
    api->spi_data_order  = Si2164B_SPI_LINK_CMD_SPI_DATA_DIR_LSB_FIRST;
    api->load_DVB_C_Blindlock_Patch = 0;
    /* Clock settings as per compilation flags                     */
    /*  For multi-frontend HW, these may be adapted later on,      */
    /*   using Si2164B_L1_API_TER_Clock and Si2164B_L1_API_SAT_Clock */
  api->cmd->dvbt2_fef.fef_tuner_flag  = Si2164B_DVBT2_FEF_CMD_FEF_TUNER_FLAG_NOT_USED;
 #ifndef     Si2164B_TER_CLOCK_SOURCE
  #define    Si2164B_TER_CLOCK_SOURCE            Si2164B_TER_Tuner_clock
 #endif   /* Si2164B_TER_CLOCK_SOURCE */
    api->tuner_ter_clock_source  = Si2164B_TER_CLOCK_SOURCE;
    api->tuner_ter_clock_control = Si2164B_CLOCK_MANAGED;
    api->tuner_ter_agc_control   = 1;

 #ifndef     Si2164B_CLOCK_MODE_TER
  #define    Si2164B_CLOCK_MODE_TER                Si2164B_START_CLK_CMD_CLK_MODE_CLK_CLKIO
 #endif   /* Si2164B_CLOCK_MODE_TER */
    api->tuner_ter_clock_input = Si2164B_CLOCK_MODE_TER;

 #ifndef     Si2164B_REF_FREQUENCY_TER
  #define    Si2164B_REF_FREQUENCY_TER                                             24
 #endif   /* Si2164B_REF_FREQUENCY_TER */
    api->tuner_ter_clock_freq  = Si2164B_REF_FREQUENCY_TER;

 #ifndef     Si2164B_SAT_CLOCK_SOURCE
  #define    Si2164B_SAT_CLOCK_SOURCE            Si2164B_Xtal_clock
 #endif   /* Si2164B_SAT_CLOCK_SOURCE */
    api->tuner_sat_clock_source  = Si2164B_SAT_CLOCK_SOURCE;
    api->tuner_sat_clock_control = Si2164B_CLOCK_MANAGED;

 #ifndef     Si2164B_CLOCK_MODE_SAT
  #define    Si2164B_CLOCK_MODE_SAT                Si2164B_START_CLK_CMD_CLK_MODE_CLK_XTAL_IN
 #endif   /* Si2164B_CLOCK_MODE_SAT */
    api->tuner_sat_clock_input = Si2164B_CLOCK_MODE_SAT;

 #ifndef     Si2164B_REF_FREQUENCY_SAT
  #define    Si2164B_REF_FREQUENCY_SAT                                             16
 #endif   /* Si2164B_REF_FREQUENCY_SAT */
    api->tuner_sat_clock_freq  = Si2164B_REF_FREQUENCY_SAT;

#ifdef    Si2164B_DD_TS_PINS_CMD
  api->cmd->dd_ts_pins.primary_ts_activity   = Si2164B_DD_TS_PINS_CMD_PRIMARY_TS_ACTIVITY_RUN;
  api->cmd->dd_ts_pins.secondary_ts_activity = Si2164B_DD_TS_PINS_CMD_SECONDARY_TS_ACTIVITY_RUN;
  api->cmd->dd_ts_pins.primary_ts_mode       = Si2164B_DD_TS_PINS_CMD_PRIMARY_TS_MODE_NOT_USED;
  api->cmd->dd_ts_pins.secondary_ts_mode     = Si2164B_DD_TS_PINS_CMD_SECONDARY_TS_MODE_NOT_USED;
#endif /* Si2164B_DD_TS_PINS_CMD */

    return NO_Si2164B_ERROR;
}
/***********************************************************************************************************************
  Si2164B_L1_API_Patch function
  Use:        Patch information function
              Used to send a number of bytes to the Si2164B. Useful to download the firmware.
  Returns:    0 if no error
  Parameter:  error_code the error code.
  Porting:    Useful for application development for debug purposes.
  Porting:    May not be required for the final application, can be removed if not used.
 ***********************************************************************************************************************/
unsigned char    Si2164B_L1_API_Patch     (L1_Si2164B_Context *api, signed int iNbBytes, unsigned char *pucDataBuffer) {
    signed int res;
    unsigned char rspByteBuffer[1];

    SiTRACE("Si2164B Patch %d bytes\n",iNbBytes);

    res = L0_WriteCommandBytes(api->i2c, iNbBytes, pucDataBuffer);
    if (res!=iNbBytes) {
      SiTRACE("Si2164B_L1_API_Patch error writing bytes: %s\n", Si2164B_L1_API_ERROR_TEXT(ERROR_Si2164B_LOADING_FIRMWARE) );
      return ERROR_Si2164B_LOADING_FIRMWARE;
    }

    res = Si2164B_pollForResponse(api, 1, rspByteBuffer);
    if (res != NO_Si2164B_ERROR) {
      SiTRACE("Si2164B_L1_API_Patch error 0x%02x polling response: %s\n", res, Si2164B_L1_API_ERROR_TEXT(res) );
      return ERROR_Si2164B_POLLING_RESPONSE;
    }

    return NO_Si2164B_ERROR;
}
/***********************************************************************************************************************
  Si2164B_L1_CheckStatus function
  Use:        Status information function
              Used to retrieve the status byte
  Returns:    0 if no error
  Parameter:  error_code the error code.
 ***********************************************************************************************************************/
unsigned char    Si2164B_L1_CheckStatus   (L1_Si2164B_Context *api) {
    unsigned char rspByteBuffer[1];
    return Si2164B_pollForResponse(api, 1, rspByteBuffer);
}
/***********************************************************************************************************************
  Si2164B_L1_API_ERROR_TEXT function
  Use:        Error information function
              Used to retrieve a text based on an error code
  Returns:    the error text
  Parameter:  error_code the error code.
  Porting:    Useful for application development for debug purposes.
  Porting:    May not be required for the final application, can be removed if not used.
 ***********************************************************************************************************************/
const unsigned char*            Si2164B_L1_API_ERROR_TEXT(signed int error_code) {
    switch (error_code) {
        case NO_Si2164B_ERROR                     : return (unsigned char*)"No Si2164B error";
        case ERROR_Si2164B_ALLOCATING_CONTEXT     : return (unsigned char*)"Error while allocating Si2164B context";
        case ERROR_Si2164B_PARAMETER_OUT_OF_RANGE : return (unsigned char*)"Si2164B parameter(s) out of range";
        case ERROR_Si2164B_SENDING_COMMAND        : return (unsigned char*)"Error while sending Si2164B command";
        case ERROR_Si2164B_CTS_TIMEOUT            : return (unsigned char*)"Si2164B CTS timeout";
        case ERROR_Si2164B_ERR                    : return (unsigned char*)"Si2164B Error (status 'err' bit 1)";
        case ERROR_Si2164B_POLLING_CTS            : return (unsigned char*)"Si2164B Error while polling CTS";
        case ERROR_Si2164B_POLLING_RESPONSE       : return (unsigned char*)"Si2164B Error while polling response";
        case ERROR_Si2164B_LOADING_FIRMWARE       : return (unsigned char*)"Si2164B Error while loading firmware";
        case ERROR_Si2164B_LOADING_BOOTBLOCK      : return (unsigned char*)"Si2164B Error while loading bootblock";
        case ERROR_Si2164B_STARTING_FIRMWARE      : return (unsigned char*)"Si2164B Error while starting firmware";
        case ERROR_Si2164B_SW_RESET               : return (unsigned char*)"Si2164B Error during software reset";
        case ERROR_Si2164B_INCOMPATIBLE_PART      : return (unsigned char*)"Si2164B Error Incompatible part";
        case ERROR_Si2164B_UNKNOWN_COMMAND        : return (unsigned char*)"Si2164B Error unknown command";
        case ERROR_Si2164B_UNKNOWN_PROPERTY       : return (unsigned char*)"Si2164B Error unknown property";
        default                                  : return (unsigned char*)"Unknown Si2164B error code";
    }
}
/***********************************************************************************************************************
  Si2164B_L1_API_TAG_TEXT function
  Use:        Error information function
              Used to retrieve a text containing the TAG information (related to the code version)
  Returns:    the TAG text
  Porting:    May not be required for the final application, can be removed if not used.
 ***********************************************************************************************************************/
const unsigned char*            Si2164B_L1_API_TAG_TEXT(void) { return (unsigned char*)"V0.1.1.0";}

/************************************************************************************************************************
  NAME: Si2164B_L1_Status
  DESCRIPTION: Calls the Si2164B global status function (DD_STATUS) and then the standard-specific status functions
  Porting:    Remove the un-necessary functions calls, if any. (Checking the TPS status may not be required)

  Parameter:  Pointer to Si2164B Context
  Returns:    1 if the current modulation is valid, 0 otherwise
************************************************************************************************************************/
signed int Si2164B_L1_Status            (L1_Si2164B_Context *api)
{
    /* Call the demod global status function */
    Si2164B_L1_DD_STATUS (api, Si2164B_DD_STATUS_CMD_INTACK_OK);

    /* Call the standard-specific status function */
    switch (api->rsp->dd_status.modulation)
    {
      case Si2164B_DD_STATUS_RESPONSE_MODULATION_DVBT : { Si2164B_L1_DVBT_STATUS  (api, Si2164B_DVBT_STATUS_CMD_INTACK_OK );
        break;
      }

      case Si2164B_DD_STATUS_RESPONSE_MODULATION_DVBT2: { Si2164B_L1_DVBT2_STATUS (api, Si2164B_DVBT2_STATUS_CMD_INTACK_OK);
        break;
      }

      case Si2164B_DD_STATUS_RESPONSE_MODULATION_DVBC : { Si2164B_L1_DVBC_STATUS  (api, Si2164B_DVBC_STATUS_CMD_INTACK_OK );
        break;
      }

      case Si2164B_DD_STATUS_RESPONSE_MODULATION_DVBC2: { Si2164B_L1_DVBC2_STATUS (api, Si2164B_DVBC2_STATUS_CMD_INTACK_OK );
        break;
      }

      case Si2164B_DD_STATUS_RESPONSE_MODULATION_DSS  :
      case Si2164B_DD_STATUS_RESPONSE_MODULATION_DVBS : { Si2164B_L1_DVBS_STATUS  (api, Si2164B_DVBS_STATUS_CMD_INTACK_OK );
        break;
      }
      case Si2164B_DD_STATUS_RESPONSE_MODULATION_DVBS2: { Si2164B_L1_DVBS2_STATUS (api, Si2164B_DVBS2_STATUS_CMD_INTACK_OK);
        break;
      }

      default                                  : { return 0; break; }
    }

    switch (api->rsp->dd_status.modulation)
    {
      case Si2164B_DD_STATUS_RESPONSE_MODULATION_DVBT2:

      case Si2164B_DD_STATUS_RESPONSE_MODULATION_DVBS2: { Si2164B_L1_DD_FER (api, Si2164B_DD_FER_CMD_RST_RUN);
        break;
      }

      default                                  : { Si2164B_L1_DD_PER (api, Si2164B_DD_PER_CMD_RST_RUN);
        break;
      }
    }
    switch (api->rsp->dd_status.modulation)
    {
      case Si2164B_DD_STATUS_RESPONSE_MODULATION_DVBT : {
                                                   Si2164B_L1_DVBT_TPS_EXTRA (api);
        break;
      }
      default                                  : { break; }
    }

 return 1;
}







