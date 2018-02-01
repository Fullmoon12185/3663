/*************************************************************************************
                  Silicon Laboratories Broadcast Si2164B Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API properties functions definitions
   FILE: Si2164B_Properties_Functions.h
   Supported IC : Si2164B
   Compiled for ROM 2 firmware 5_B_build_1
   Revision: V0.1.1.0
   Date: March 30 2015
  (C) Copyright 2015, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
/* Change log:
 As from V0.0.0:
  Initial version (based on Si2164 code V0.3.4)
****************************************************************************************/
#ifndef   _Si2164B_PROPERTIES_FUNCTIONS_H_
#define   _Si2164B_PROPERTIES_FUNCTIONS_H_

void          Si2164B_storeUserProperties           (Si2164B_PropObj   *prop);
unsigned char Si2164B_PackProperty                  (Si2164B_PropObj   *prop, unsigned int prop_code, signed int *data);
unsigned char Si2164B_UnpackProperty                (Si2164B_PropObj   *prop, unsigned int prop_code, signed int  data);
void          Si2164B_storePropertiesDefaults       (Si2164B_PropObj   *prop);

signed int  Si2164B_downloadCOMMONProperties(L1_Si2164B_Context *api);
signed int  Si2164B_downloadDDProperties    (L1_Si2164B_Context *api);
signed int  Si2164B_downloadDVBCProperties  (L1_Si2164B_Context *api);

signed int  Si2164B_downloadDVBC2Properties (L1_Si2164B_Context *api);

signed int  Si2164B_downloadDVBSProperties  (L1_Si2164B_Context *api);
signed int  Si2164B_downloadDVBS2Properties (L1_Si2164B_Context *api);

signed int  Si2164B_downloadDVBTProperties  (L1_Si2164B_Context *api);

signed int  Si2164B_downloadDVBT2Properties (L1_Si2164B_Context *api);


signed int  Si2164B_downloadMCNSProperties  (L1_Si2164B_Context *api);

signed int  Si2164B_downloadSCANProperties  (L1_Si2164B_Context *api);
signed int  Si2164B_downloadAllProperties   (L1_Si2164B_Context *api);

#endif /* _Si2164B_PROPERTIES_FUNCTIONS_H_ */







