#ifndef _L1_RF_RDA_API_H_
#define _L1_RF_RDA_API_H_

#include "../Si_I2C/Silabs_L0_API.h"

#define SAT1_MAIN_ADDRESS           0x16// DUAL 5816SW 1 -> NEED TO BE CHANGED TO 0x16//12
#define SAT1_SUB_ADDRESS			0x14// DUAL 5816SW 2 -> NEED TO BE CHANGED TO 0x14//10
#define SAT2_ADDRESS				0x18// SINGLE 5815M
#define SAT3_ADDRESS				0x1C// SINGLE 5815M, 20130820,csm,for Si2167B test
#define SAT4_ADDRESS				0xC4

#ifdef __cplusplus
extern "C"{
#endif
typedef struct RDA_Context {
    L0_Context    *i2c;
    L0_Context     i2cObj;
    signed int    RF;
    signed int    IF;
    signed int    minRF;
    signed int    maxRF;
	int    tuner_crystal;
    signed int    LPF;
	unsigned char Status;
	unsigned char bytes[40];
	unsigned char version;
	unsigned char tuner_log[50];
    int     rssi;
} RDA_Context;

signed int  RDA_Communication_Check (RDA_Context *context);
void L1_RF_RDA_Init          (RDA_Context *context, unsigned int add);
signed int  L1_RF_RDA_InitAfterReset(RDA_Context *context);
signed int  L1_RF_RDA_Get_RF        (RDA_Context *context);
signed int  L1_RF_RDA_Tune          (RDA_Context *context, signed int rf);
signed int  L1_RF_RDA_LPF           (RDA_Context *context, signed int lpf);
unsigned char L1_RF_RDA_Wakeup       (RDA_Context *context);
unsigned char L1_RF_RDA_Standby      (RDA_Context *context);
unsigned char L1_RF_RDA_LockStatus   (RDA_Context *context);
unsigned int  L1_RF_RDA_RfSel        (RDA_Context *context, unsigned char rf_chn);
unsigned int  L1_RF_RDA_RfSel_Read   (RDA_Context *context, unsigned char *same_side, unsigned char *diff_side);
signed int  L1_RF_RDA_RSSI (RDA_Context *context);
const unsigned char*         L1_RF_RDA_TAG_TEXT      (void);//csm


/* AV2018 registers definitions */
/* This allows using the following syntax:
 to set  register foo:             AV2018_WRITE(foo, foo_on);
 to read register foo: foo_value = AV2018_READ(foo);

 The above macros handle the necessary reading/masing/shifting for any register from 1 to 32 bits. (Casting may be required for 32 bit signed registers)

 */
#define int_ADDRESS  0x00
#define int_OFFSET   0
#define int_NBBIT    7
#define int_ALONE    1
#define int_SIGNED   0

#define frac_reversed_ADDRESS  0x01
#define frac_reversed_OFFSET   0
#define frac_reversed_NBBIT    17
#define frac_reversed_ALONE    0
#define frac_reversed_SIGNED   0

#define bf_ADDRESS  0x05
#define bf_OFFSET   0
#define bf_NBBIT    8
#define bf_ALONE    1
#define bf_SIGNED   0
 #define bf_off   0
 #define bf_on    1

#define gc2_ADDRESS  0x08
#define gc2_OFFSET   3
#define gc2_NBBIT    4
#define gc2_ALONE    0
#define gc2_SIGNED   0
 #define gc2_0_0_dB    0
 #define gc2_1_5_dB    1
 #define gc2_3_0_dB    2
 #define gc2_4_5_dB    3
 #define gc2_6_0_dB    4
 #define gc2_7_5_dB    5
 #define gc2_9_0_dB    6
 #define gc2_10_5_dB   7
 #define gc2_15_5_dB   8

#define pgaout_cs_ADDRESS  0x08
#define pgaout_cs_OFFSET   0
#define pgaout_cs_NBBIT    2
#define pgaout_cs_ALONE    0
#define pgaout_cs_SIGNED   0
  #define pgaout_cs_0_5_mA  0
  #define pgaout_cs_1_0_mA  1
  #define pgaout_cs_1_5_mA  2
  #define pgaout_cs_2_0_mA  3

#define xocore_ct_ADDRESS  0x09
#define xocore_ct_OFFSET   6
#define xocore_ct_NBBIT    2
#define xocore_ct_ALONE    0
#define xocore_ct_SIGNED   0
 #define xocore_ct_min_current  0
 #define xocore_ct_max_current  3

#define bwlf_ADDRESS  0x0b
#define bwlf_OFFSET   1
#define bwlf_NBBIT    1
#define bwlf_ALONE    0
#define bwlf_SIGNED   0
 #define bwlf_unlock   0
 #define bwlf_lock     1

#define chlf_ADDRESS  0x0b
#define chlf_OFFSET   0
#define chlf_NBBIT    1
#define chlf_ALONE    0
#define chlf_SIGNED   0
 #define chlf_unlock   0
 #define chlf_lock    1

#define xocore_ena_ADDRESS  0x0c
#define xocore_ena_OFFSET   7
#define xocore_ena_NBBIT    1
#define xocore_ena_ALONE    0
#define xocore_ena_SIGNED   0

#define rflp_ena_ADDRESS  xocore_ena_ADDRESS
#define rflp_ena_OFFSET   6
#define rflp_ena_NBBIT    1
#define rflp_ena_ALONE    0
#define rflp_ena_SIGNED   0

#define pd_soft_ADDRESS  xocore_ena_ADDRESS
#define pd_soft_OFFSET   5
#define pd_soft_NBBIT    1
#define pd_soft_ALONE    0
#define pd_soft_SIGNED   0
 #define pd_soft_power_up      0
 #define pd_soft_power_down    1

#define ft_block_ADDRESS  0x25
#define ft_block_OFFSET   2
#define ft_block_NBBIT    1
#define ft_block_ALONE    0
#define ft_block_SIGNED   0

#define ft_en_ADDRESS  ft_block_ADDRESS
#define ft_en_OFFSET   1
#define ft_en_NBBIT    1
#define ft_en_ALONE    0
#define ft_en_SIGNED   0

#define ft_hold_ADDRESS  ft_block_ADDRESS
#define ft_hold_OFFSET   0
#define ft_hold_NBBIT    1
#define ft_hold_ALONE    0
#define ft_hold_SIGNED   0

#ifdef RWTRACES
  #define AV2018_WRITE(context, register, v ) L0_WriteRegisterTrace (context->i2c, #register, #v, register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_ALONE, v)
  #define AV2018_READ(context,  register)     L0_ReadRegisterTrace  (context->i2c, #register,     register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_SIGNED)
#else
  #define AV2018_WRITE(context, register, v ) L0_WriteRegister      (context->i2c,                register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_ALONE, v)
  #define AV2018_READ(context, register)      L0_ReadRegister       (context->i2c,                register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_ALONE)
#endif

int    L1_RF_AV2018_Wakeup         (RDA_Context *context);
int    L1_RF_AV2018_Standby        (RDA_Context *context);
int    L1_RF_AV2018_InitAfterReset (RDA_Context *context);
int    L1_RF_AV2018_Tune           (RDA_Context *context, int rf);
int    L1_RF_AV2018_ClockOn        (RDA_Context *context);
int    L1_RF_AV2018_ClockOff       (RDA_Context *context);
int    L1_RF_AV2018_LockStatus     (RDA_Context *context);
int    L1_RF_AV2018_Get_IF         (RDA_Context *context);
int    L1_RF_AV2018_Get_RF         (RDA_Context *context);
int    L1_RF_AV2018_Get_minRF      (RDA_Context *context);
int    L1_RF_AV2018_Get_maxRF      (RDA_Context *context);


#ifdef __cplusplus
}
#endif
#endif /* _L1_RF_RDA_API_H_ */
