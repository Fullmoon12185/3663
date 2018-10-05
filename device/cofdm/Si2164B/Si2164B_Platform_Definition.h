#ifndef    _Si2164B_PLATFORM_DEFINITION_H_
#define    _Si2164B_PLATFORM_DEFINITION_H_
/* Change log

 As from V0.0.9.0
  <new_feature>[superset] Changing tags to allow SILABS_SUPERSET use (one code, all configs)

*/
//csm
/*****************************************************************************/
/* Default values and demod address                                          */
/*****************************************************************************/
#define DEMOD1_MAIN1_ADDRESS            0xCE// DUAL T2CS2 DEMOD1 BLOCK B(SI21692)
#define DEMOD1_MAIN2_ADDRESS            0xC8// DUAL T2CS2 DEMOD1 BLOCK A(SI21692)
#define DEMOD2_MAIN1_ADDRESS			0xCC// DUAL TCS2  DEMOD1        (SI2167B)
#define DEMOD2_MAIN2_ADDRESS			0xCA// DUAL TCS2  DEMOD2        (SI2167B)....NOT DEFINED!!
#define DEMOD3_ADDRESS					0xCC// reserved
#define DEMOD4_ADDRESS					0xCC// SINGLE TCS2 DEMOD        (SI2167B)
#define DEMOD_DUMMY_ADDRESS             0x00// Dummy demod. Address
/*****************************************************************************/
/* Possible DTV standards                                                    */
/*****************************************************************************/
#ifdef    DEMOD_Si2164B
  #define DEMOD_ISDB_T
#endif /* DEMOD_Si2164B */

/******************************************************************************/
/* TER Tuner FEF management options */
/******************************************************************************/
#define Si2164B_FEF_MODE_SLOW_NORMAL_AGC  0
#define Si2164B_FEF_MODE_FREEZE_PIN       1
#define Si2164B_FEF_MODE_SLOW_INITIAL_AGC 2

/******************************************************************************/
/* TER Tuner FEF management selection (possible values are defined above) */
/* NB : This selection is the ‘preferred?solution.                           */
/* The code will use more compilation flags to slect the final mode based     */
/*  on what the TER tuner can actually do.                                    */
/******************************************************************************/
#define Si2164B_FEF_MODE    Si2164B_FEF_MODE_FREEZE_PIN

/******************************************************************************/
/* Tuners selection (one terrestrial tuner, one satellite tuner )             */
/******************************************************************************/

#ifdef    TER_TUNER_SILABS
  #include  "SiLabs_TER_Tuner_API.h"
#endif /* TER_TUNER_SILABS */

  #include  "Si2164B_Si21XXC.h"
  //#include  "SiTerRf_Si21XXC.h"//csm

#ifdef    TER_TUNER_NO_TER
  #include  "Si2164B_NO_TER.h"
#endif /* TER_TUNER_NO_TER */

  #include  "Si2164B_RDA.h"



/******************************************************************************/
/* Clock sources definition (allows using 'clear' names for clock sources)    */
/******************************************************************************/
typedef enum Si2164B_CLOCK_SOURCE {
  Si2164B_Xtal_clock = 0,
  Si2164B_TER_Tuner_clock,
  Si2164B_SAT_Tuner_clock
} Si2164B_CLOCK_SOURCE;

/******************************************************************************/
/* TER and SAT clock source selection (used by Si2164B_switch_to_standard)     */
/* ( possible values are those defined above in Si2164B_CLOCK_SOURCE )         */
/******************************************************************************/
#ifdef   SILABS_EVB
  #define Si2164B_TER_CLOCK_SOURCE            Si2164B_TER_Tuner_clock
  #define Si2164B_SAT_CLOCK_SOURCE            Si2164B_Xtal_clock
#else
  #define Si2164B_TER_CLOCK_SOURCE            Si2164B_TER_Tuner_clock
  #define Si2164B_SAT_CLOCK_SOURCE            Si2164B_SAT_Tuner_clock
#endif /* SILABS_EVB */

#include "Si2164B_L1_API.h"
#include "Si2164B_Properties_Functions.h"

#endif /* _Si2164B_PLATFORM_DEFINITION_H_ */
