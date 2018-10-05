/*************************************************************************************
                  Silicon Laboratories Broadcast Si2164B Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API functions prototypes used by commands and properties
   FILE: Si2164B_L1_API.h
   Supported IC : Si2164B
   Compiled for ROM 2 firmware 5_B_build_1
   Revision: V0.1.1.0
   Date: March 30 2015
  (C) Copyright 2015, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
/* Change log:
 As from V0.9.0.0:
  <new_feature>[superset] Adding tags to allow SILABS_SUPERSET use (one code, all configs)

 As from V0.0.0:
  Initial version (based on Si2164 code V0.3.4)
****************************************************************************************/
#ifndef    _Si2164B_L1_API_H_
#define    _Si2164B_L1_API_H_

#include "../../tuner/si2151_2164B/Si_I2C/Silabs_L0_API.h"

#ifdef    DEMOD_Si2164B
  #define DEMOD_ISDB_T
#endif /* DEMOD_Si2164B */

#include "Si2164B_Commands.h"
#include "Si2164B_Properties.h"
#include "Si2164B_typedefs.h"
#include "Si2164B_Commands_Prototypes.h"
#include "Si2164B_Properties_Functions.h"

#define NO_Si2164B_ERROR                     0x00
#define ERROR_Si2164B_PARAMETER_OUT_OF_RANGE 0x01
#define ERROR_Si2164B_ALLOCATING_CONTEXT     0x02
#define ERROR_Si2164B_SENDING_COMMAND        0x03
#define ERROR_Si2164B_CTS_TIMEOUT            0x04
#define ERROR_Si2164B_ERR                    0x05
#define ERROR_Si2164B_POLLING_CTS            0x06
#define ERROR_Si2164B_POLLING_RESPONSE       0x07
#define ERROR_Si2164B_LOADING_FIRMWARE       0x08
#define ERROR_Si2164B_LOADING_BOOTBLOCK      0x09
#define ERROR_Si2164B_STARTING_FIRMWARE      0x0a
#define ERROR_Si2164B_SW_RESET               0x0b
#define ERROR_Si2164B_INCOMPATIBLE_PART      0x0c
#define ERROR_Si2164B_DISEQC_BUS_NOT_READY   0x0d
#define ERROR_Si2164B_UNKNOWN_COMMAND        0xf0
#define ERROR_Si2164B_UNKNOWN_PROPERTY       0xf1


unsigned char Si2164B_L1_API_Init    (L1_Si2164B_Context *api, signed int add);
unsigned char Si2164B_L1_API_Patch   (L1_Si2164B_Context *api, signed int iNbBytes, unsigned char *pucDataBuffer);
unsigned char Si2164B_L1_CheckStatus (L1_Si2164B_Context *api);
const unsigned char*         Si2164B_L1_API_ERROR_TEXT(signed int  error_code);
const unsigned char*         Si2164B_L1_API_TAG_TEXT  (void);
signed   int  Si2164B_L1_GET_REG      (L1_Si2164B_Context *api, unsigned char   reg_code_lsb, unsigned char   reg_code_mid, unsigned char   reg_code_msb);

#define Si2164B_TAG Si2164B_L1_API_TAG_TEXT


#ifdef    Si2164B_GET_PROPERTY_STRING
void Si2164B_L1_FillPropertyStringText(L1_Si2164B_Context *api, unsigned int prop_code, const unsigned char *separator, unsigned char *msg);
#endif /* Si2164B_GET_PROPERTY_STRING */

#endif /* _Si2164B_L1_API_H_ */








