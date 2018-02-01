#ifndef  TUNER_RDA_SPECIFICS
    #define TUNER_RDA_SPECIFICS

    //#include "Silabs_L1_RF_RDA5816S_API.h"
	#include "../RDA/Semco_RDA.h"//csm

#ifdef    SILABS_EVB
    #define Si2164B_CLOCK_MODE_SAT              Si2164B_START_CLK_CMD_CLK_MODE_XTAL
    #define Si2164B_REF_FREQUENCY_SAT           16
    #define Si2164B_SAT_IQ_SPECTRUM_INVERSION   Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_INVERTED
#else  /* SILABS_EVB */
    #define Si2164B_CLOCK_MODE_SAT              Si2164B_START_CLK_CMD_CLK_MODE_CLK_XTAL_IN
    #define Si2164B_REF_FREQUENCY_SAT           27
    #define Si2164B_SAT_IQ_SPECTRUM_INVERSION   Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_INVERTED
#endif /* SILABS_EVB */

 //   #define SAT1_MAIN_ADDRESS           0x16// DUAL 5816SW 1 -> NEED TO BE CHANGED TO 0x16//12
 //   #define SAT1_SUB_ADDRESS			0x14// DUAL 5816SW 2 -> NEED TO BE CHANGED TO 0x14//10
 //   #define SAT2_ADDRESS				0x18// SINGLE 5815M
	//#define SAT3_ADDRESS				0x1c// SINGLE 5815M, 20130820,csm,for Si2167B test

    #define Si2164B_SAT_AGC_INVERTED
    #define SAT_TUNER_CONTEXT           RDA_Context
    #define SILABS_SAT_TUNER_Context    SAT_TUNER_CONTEXT
    /* Satellite tuner function names */

	#define L1_RF_SAT_TUNER_WAKEUP				L1_RF_RDA_Wakeup
	#define L1_RF_SAT_TUNER_STANDBY				L1_RF_RDA_Standby
    #define L1_RF_SAT_TUNER_Process             L1_RF_RDA_Process
    #define L1_RF_SAT_TUNER_Init                L1_RF_RDA_Init
    //#define SAT_TUNER_INIT                      L1_RF_RDA_InitAfterReset
	#define L1_RF_SAT_TUNER_InitAfterReset      L1_RF_RDA_InitAfterReset//CSM
    #define L1_RF_SAT_TUNER_Get_RF              L1_RF_RDA_Get_RF
    #define L1_RF_SAT_TUNER_Tune                L1_RF_RDA_Tune
    #define L1_RF_SAT_TUNER_LPF                 L1_RF_RDA_LPF
    //#define SAT_TUNER_COMM_CHECK                RDA_Communication_Check
    #define L1_RF_SAT_TUNER_COMM_CHECK          RDA_Communication_Check//CSM
    #define SAT_TUNER_RSSI                      L1_RF_RDA_RSSI//csm//L1_RF_RDA5816S_RSSI
    #define L1_RF_SAT_TUNER_PLLlock             L1_RF_RDA_LockStatus//cms//L1_RF_RDA5816S_PLLlock//csm,13'
	#define L1_RF_SAT_TUNER_RfSel				L1_RF_RDA_RfSel
	#define L1_RF_SAT_TUNER_RfSel_Read			L1_RF_RDA_RfSel_Read
    //#define SAT_TUNER_TAG_TEXT                  L1_RF_RDA_TAG_TEXT
    #define L1_RF_SAT_TUNER_TAG_TEXT            L1_RF_RDA_TAG_TEXT//CSM
#endif /* TUNER_RDA_SPECIFICS */
