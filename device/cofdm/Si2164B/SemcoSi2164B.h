#ifdef __cplusplus
extern "C" {
#endif

#include "Si2164B_Platform_Definition.h"


/* Check Never Lock */
unsigned char SiDemod_Check_NeverLock(L1_Si2164B_Context *demod);
/* Get DVB-C Demod lock in range */
signed char SiDemod_L1_DVB_C_get_dl_lock_in_range(L1_Si2164B_Context *demod, signed long range_kBps);
/* Get DVB-S/S2 lock in AFC range */
signed char SiDemod_L1_DVB_S_S2_get_lock_in_AFC_range(L1_Si2164B_Context *demod);
/* DVB-C SSI */
signed int SiDemod_DVB_C_SSI_for_Received_Power(L1_Si2164B_Context *demod, signed int Prec);
/* DVB-C SQI */
signed int SiDemod_DVB_C_SQI(L1_Si2164B_Context *demod);
/* T2CS2_DUAL DVB-S/S2 SSI */
signed int SiDemod_DVB_SAT_SSI_T2S2_DUAL(L1_Si2164B_Context *demod, signed int rssi);
/* T2CS2_Single NIM DVB-S/S2 SSI */
signed int SiDemod_DVB_SAT_SSI_T2CS2_SINGLE(L1_Si2164B_Context *demod, signed int rssi);
/* DVB-S SQI */
signed int SiDemod_DVB_S_SQI(L1_Si2164B_Context *demod);
/* DVB-S2 SQI */
signed int SiDemod_DVB_S2_SQI(L1_Si2164B_Context *demod);
/* DVB-S2 PER */
signed int SiDemod_DVB_S2_PER(L1_Si2164B_Context *demod);

#ifdef __cplusplus
}
#endif
