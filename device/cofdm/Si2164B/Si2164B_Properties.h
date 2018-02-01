/*************************************************************************************
                  Silicon Laboratories Broadcast Si2164B Layer 1 API

   EVALUATION AND USE OF THIS SOFTWARE IS SUBJECT TO THE TERMS AND CONDITIONS OF
     THE SOFTWARE LICENSE AGREEMENT IN THE DOCUMENTATION FILE CORRESPONDING
     TO THIS SOURCE FILE.
   IF YOU DO NOT AGREE TO THE LIMITED LICENSE AND CONDITIONS OF SUCH AGREEMENT,
     PLEASE RETURN ALL SOURCE FILES TO SILICON LABORATORIES.

   API properties structures and functions definitions
   FILE: Si2164B_Properties.h
   Supported IC : Si2164B
   Compiled for ROM 2 firmware 5_B_build_1
   Revision: V0.1.1.0
   Date: March 30 2015
  (C) Copyright 2015, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
/* Change log:

 As from V0.0.6.0:
  <new_feature>[ISDBT/MODE] Adding isdbt_mode.dl_config to Si2164B_ISDBT_MODE property 

 As from V0.0.0:
  Initial version (based on Si2164 code V0.3.4)
****************************************************************************************/
#ifndef   _Si2164B_PROPERTIES_H_
#define   _Si2164B_PROPERTIES_H_

/* Si2164B DD_BER_RESOL property definition */
#define   Si2164B_DD_BER_RESOL_PROP 0x1003

#ifdef    Si2164B_DD_BER_RESOL_PROP
  #define Si2164B_DD_BER_RESOL_PROP_CODE 0x001003


    typedef struct { /* Si2164B_DD_BER_RESOL_PROP_struct */
      unsigned char   exp;
      unsigned char   mant;
   } Si2164B_DD_BER_RESOL_PROP_struct;

   /* DD_BER_RESOL property, EXP field definition (NO TITLE)*/
   #define  Si2164B_DD_BER_RESOL_PROP_EXP_LSB         0
   #define  Si2164B_DD_BER_RESOL_PROP_EXP_MASK        0x0f
   #define  Si2164B_DD_BER_RESOL_PROP_EXP_DEFAULT    7
    #define Si2164B_DD_BER_RESOL_PROP_EXP_EXPLO_MIN  1
    #define Si2164B_DD_BER_RESOL_PROP_EXP_EXPLO_MAX  8

   /* DD_BER_RESOL property, MANT field definition (NO TITLE)*/
   #define  Si2164B_DD_BER_RESOL_PROP_MANT_LSB         4
   #define  Si2164B_DD_BER_RESOL_PROP_MANT_MASK        0x0f
   #define  Si2164B_DD_BER_RESOL_PROP_MANT_DEFAULT    1
    #define Si2164B_DD_BER_RESOL_PROP_MANT_MANTLO_MIN  1
    #define Si2164B_DD_BER_RESOL_PROP_MANT_MANTLO_MAX  9

#endif /* Si2164B_DD_BER_RESOL_PROP */

/* Si2164B DD_CBER_RESOL property definition */
#define   Si2164B_DD_CBER_RESOL_PROP 0x1002

#ifdef    Si2164B_DD_CBER_RESOL_PROP
  #define Si2164B_DD_CBER_RESOL_PROP_CODE 0x001002


    typedef struct { /* Si2164B_DD_CBER_RESOL_PROP_struct */
      unsigned char   exp;
      unsigned char   mant;
   } Si2164B_DD_CBER_RESOL_PROP_struct;

   /* DD_CBER_RESOL property, EXP field definition (NO TITLE)*/
   #define  Si2164B_DD_CBER_RESOL_PROP_EXP_LSB         0
   #define  Si2164B_DD_CBER_RESOL_PROP_EXP_MASK        0x0f
   #define  Si2164B_DD_CBER_RESOL_PROP_EXP_DEFAULT    5
    #define Si2164B_DD_CBER_RESOL_PROP_EXP_EXPLO_MIN  1
    #define Si2164B_DD_CBER_RESOL_PROP_EXP_EXPLO_MAX  8

   /* DD_CBER_RESOL property, MANT field definition (NO TITLE)*/
   #define  Si2164B_DD_CBER_RESOL_PROP_MANT_LSB         4
   #define  Si2164B_DD_CBER_RESOL_PROP_MANT_MASK        0x0f
   #define  Si2164B_DD_CBER_RESOL_PROP_MANT_DEFAULT    1
    #define Si2164B_DD_CBER_RESOL_PROP_MANT_MANTLO_MIN  1
    #define Si2164B_DD_CBER_RESOL_PROP_MANT_MANTLO_MAX  9

#endif /* Si2164B_DD_CBER_RESOL_PROP */

/* Si2164B DD_DISEQC_FREQ property definition */
#define   Si2164B_DD_DISEQC_FREQ_PROP 0x100e

#ifdef    Si2164B_DD_DISEQC_FREQ_PROP
  #define Si2164B_DD_DISEQC_FREQ_PROP_CODE 0x00100e


    typedef struct { /* Si2164B_DD_DISEQC_FREQ_PROP_struct */
      unsigned int    freq_hz;
   } Si2164B_DD_DISEQC_FREQ_PROP_struct;

   /* DD_DISEQC_FREQ property, FREQ_HZ field definition (NO TITLE)*/
   #define  Si2164B_DD_DISEQC_FREQ_PROP_FREQ_HZ_LSB         0
   #define  Si2164B_DD_DISEQC_FREQ_PROP_FREQ_HZ_MASK        0xffff
   #define  Si2164B_DD_DISEQC_FREQ_PROP_FREQ_HZ_DEFAULT    22000
#endif /* Si2164B_DD_DISEQC_FREQ_PROP */

/* Si2164B DD_DISEQC_PARAM property definition */
#define   Si2164B_DD_DISEQC_PARAM_PROP 0x1010

#ifdef    Si2164B_DD_DISEQC_PARAM_PROP
  #define Si2164B_DD_DISEQC_PARAM_PROP_CODE 0x001010


    typedef struct { /* Si2164B_DD_DISEQC_PARAM_PROP_struct */
      unsigned char   input_pin;
      unsigned char   sequence_mode;
   } Si2164B_DD_DISEQC_PARAM_PROP_struct;

   /* DD_DISEQC_PARAM property, INPUT_PIN field definition (NO TITLE)*/
   #define  Si2164B_DD_DISEQC_PARAM_PROP_INPUT_PIN_LSB         1
   #define  Si2164B_DD_DISEQC_PARAM_PROP_INPUT_PIN_MASK        0x01
   #define  Si2164B_DD_DISEQC_PARAM_PROP_INPUT_PIN_DEFAULT    0
    #define Si2164B_DD_DISEQC_PARAM_PROP_INPUT_PIN_DISEQC_IN   0
    #define Si2164B_DD_DISEQC_PARAM_PROP_INPUT_PIN_DISEQC_CMD  1

   /* DD_DISEQC_PARAM property, SEQUENCE_MODE field definition (NO TITLE)*/
   #define  Si2164B_DD_DISEQC_PARAM_PROP_SEQUENCE_MODE_LSB         0
   #define  Si2164B_DD_DISEQC_PARAM_PROP_SEQUENCE_MODE_MASK        0x01
   #define  Si2164B_DD_DISEQC_PARAM_PROP_SEQUENCE_MODE_DEFAULT    0
    #define Si2164B_DD_DISEQC_PARAM_PROP_SEQUENCE_MODE_GAP     0
    #define Si2164B_DD_DISEQC_PARAM_PROP_SEQUENCE_MODE_NO_GAP  1

#endif /* Si2164B_DD_DISEQC_PARAM_PROP */


/* Si2164B DD_FER_RESOL property definition */
#define   Si2164B_DD_FER_RESOL_PROP 0x100c

#ifdef    Si2164B_DD_FER_RESOL_PROP
  #define Si2164B_DD_FER_RESOL_PROP_CODE 0x00100c


    typedef struct { /* Si2164B_DD_FER_RESOL_PROP_struct */
      unsigned char   exp;
      unsigned char   mant;
   } Si2164B_DD_FER_RESOL_PROP_struct;

   /* DD_FER_RESOL property, EXP field definition (NO TITLE)*/
   #define  Si2164B_DD_FER_RESOL_PROP_EXP_LSB         0
   #define  Si2164B_DD_FER_RESOL_PROP_EXP_MASK        0x0f
   #define  Si2164B_DD_FER_RESOL_PROP_EXP_DEFAULT    3
    #define Si2164B_DD_FER_RESOL_PROP_EXP_EXP_MIN  1
    #define Si2164B_DD_FER_RESOL_PROP_EXP_EXP_MAX  4

   /* DD_FER_RESOL property, MANT field definition (NO TITLE)*/
   #define  Si2164B_DD_FER_RESOL_PROP_MANT_LSB         4
   #define  Si2164B_DD_FER_RESOL_PROP_MANT_MASK        0x0f
   #define  Si2164B_DD_FER_RESOL_PROP_MANT_DEFAULT    1
    #define Si2164B_DD_FER_RESOL_PROP_MANT_MANT_MIN  1
    #define Si2164B_DD_FER_RESOL_PROP_MANT_MANT_MAX  9

#endif /* Si2164B_DD_FER_RESOL_PROP */

/* Si2164B DD_IEN property definition */
#define   Si2164B_DD_IEN_PROP 0x1006

#ifdef    Si2164B_DD_IEN_PROP
  #define Si2164B_DD_IEN_PROP_CODE 0x001006


    typedef struct { /* Si2164B_DD_IEN_PROP_struct */
      unsigned char   ien_bit0;
      unsigned char   ien_bit1;
      unsigned char   ien_bit2;
      unsigned char   ien_bit3;
      unsigned char   ien_bit4;
      unsigned char   ien_bit5;
      unsigned char   ien_bit6;
      unsigned char   ien_bit7;
   } Si2164B_DD_IEN_PROP_struct;

   /* DD_IEN property, IEN_BIT0 field definition (NO TITLE)*/
   #define  Si2164B_DD_IEN_PROP_IEN_BIT0_LSB         0
   #define  Si2164B_DD_IEN_PROP_IEN_BIT0_MASK        0x01
   #define  Si2164B_DD_IEN_PROP_IEN_BIT0_DEFAULT    0
    #define Si2164B_DD_IEN_PROP_IEN_BIT0_DISABLE  0
    #define Si2164B_DD_IEN_PROP_IEN_BIT0_ENABLE   1

   /* DD_IEN property, IEN_BIT1 field definition (NO TITLE)*/
   #define  Si2164B_DD_IEN_PROP_IEN_BIT1_LSB         1
   #define  Si2164B_DD_IEN_PROP_IEN_BIT1_MASK        0x01
   #define  Si2164B_DD_IEN_PROP_IEN_BIT1_DEFAULT    0
    #define Si2164B_DD_IEN_PROP_IEN_BIT1_DISABLE  0
    #define Si2164B_DD_IEN_PROP_IEN_BIT1_ENABLE   1

   /* DD_IEN property, IEN_BIT2 field definition (NO TITLE)*/
   #define  Si2164B_DD_IEN_PROP_IEN_BIT2_LSB         2
   #define  Si2164B_DD_IEN_PROP_IEN_BIT2_MASK        0x01
   #define  Si2164B_DD_IEN_PROP_IEN_BIT2_DEFAULT    0
    #define Si2164B_DD_IEN_PROP_IEN_BIT2_DISABLE  0
    #define Si2164B_DD_IEN_PROP_IEN_BIT2_ENABLE   1

   /* DD_IEN property, IEN_BIT3 field definition (NO TITLE)*/
   #define  Si2164B_DD_IEN_PROP_IEN_BIT3_LSB         3
   #define  Si2164B_DD_IEN_PROP_IEN_BIT3_MASK        0x01
   #define  Si2164B_DD_IEN_PROP_IEN_BIT3_DEFAULT    0
    #define Si2164B_DD_IEN_PROP_IEN_BIT3_DISABLE  0
    #define Si2164B_DD_IEN_PROP_IEN_BIT3_ENABLE   1

   /* DD_IEN property, IEN_BIT4 field definition (NO TITLE)*/
   #define  Si2164B_DD_IEN_PROP_IEN_BIT4_LSB         4
   #define  Si2164B_DD_IEN_PROP_IEN_BIT4_MASK        0x01
   #define  Si2164B_DD_IEN_PROP_IEN_BIT4_DEFAULT    0
    #define Si2164B_DD_IEN_PROP_IEN_BIT4_DISABLE  0
    #define Si2164B_DD_IEN_PROP_IEN_BIT4_ENABLE   1

   /* DD_IEN property, IEN_BIT5 field definition (NO TITLE)*/
   #define  Si2164B_DD_IEN_PROP_IEN_BIT5_LSB         5
   #define  Si2164B_DD_IEN_PROP_IEN_BIT5_MASK        0x01
   #define  Si2164B_DD_IEN_PROP_IEN_BIT5_DEFAULT    0
    #define Si2164B_DD_IEN_PROP_IEN_BIT5_DISABLE  0
    #define Si2164B_DD_IEN_PROP_IEN_BIT5_ENABLE   1

   /* DD_IEN property, IEN_BIT6 field definition (NO TITLE)*/
   #define  Si2164B_DD_IEN_PROP_IEN_BIT6_LSB         6
   #define  Si2164B_DD_IEN_PROP_IEN_BIT6_MASK        0x01
   #define  Si2164B_DD_IEN_PROP_IEN_BIT6_DEFAULT    0
    #define Si2164B_DD_IEN_PROP_IEN_BIT6_DISABLE  0
    #define Si2164B_DD_IEN_PROP_IEN_BIT6_ENABLE   1

   /* DD_IEN property, IEN_BIT7 field definition (NO TITLE)*/
   #define  Si2164B_DD_IEN_PROP_IEN_BIT7_LSB         7
   #define  Si2164B_DD_IEN_PROP_IEN_BIT7_MASK        0x01
   #define  Si2164B_DD_IEN_PROP_IEN_BIT7_DEFAULT    0
    #define Si2164B_DD_IEN_PROP_IEN_BIT7_DISABLE  0
    #define Si2164B_DD_IEN_PROP_IEN_BIT7_ENABLE   1

#endif /* Si2164B_DD_IEN_PROP */

/* Si2164B DD_IF_INPUT_FREQ property definition */
#define   Si2164B_DD_IF_INPUT_FREQ_PROP 0x100b

#ifdef    Si2164B_DD_IF_INPUT_FREQ_PROP
  #define Si2164B_DD_IF_INPUT_FREQ_PROP_CODE 0x00100b


    typedef struct { /* Si2164B_DD_IF_INPUT_FREQ_PROP_struct */
      unsigned int    offset;
   } Si2164B_DD_IF_INPUT_FREQ_PROP_struct;

   /* DD_IF_INPUT_FREQ property, OFFSET field definition (NO TITLE)*/
   #define  Si2164B_DD_IF_INPUT_FREQ_PROP_OFFSET_LSB         0
   #define  Si2164B_DD_IF_INPUT_FREQ_PROP_OFFSET_MASK        0xffff
   #define  Si2164B_DD_IF_INPUT_FREQ_PROP_OFFSET_DEFAULT    5000
    #define Si2164B_DD_IF_INPUT_FREQ_PROP_OFFSET_OFFSET_MIN  0
    #define Si2164B_DD_IF_INPUT_FREQ_PROP_OFFSET_OFFSET_MAX  36000

#endif /* Si2164B_DD_IF_INPUT_FREQ_PROP */

/* Si2164B DD_INT_SENSE property definition */
#define   Si2164B_DD_INT_SENSE_PROP 0x1007

#ifdef    Si2164B_DD_INT_SENSE_PROP
  #define Si2164B_DD_INT_SENSE_PROP_CODE 0x001007


    typedef struct { /* Si2164B_DD_INT_SENSE_PROP_struct */
      unsigned char   neg_bit0;
      unsigned char   neg_bit1;
      unsigned char   neg_bit2;
      unsigned char   neg_bit3;
      unsigned char   neg_bit4;
      unsigned char   neg_bit5;
      unsigned char   neg_bit6;
      unsigned char   neg_bit7;
      unsigned char   pos_bit0;
      unsigned char   pos_bit1;
      unsigned char   pos_bit2;
      unsigned char   pos_bit3;
      unsigned char   pos_bit4;
      unsigned char   pos_bit5;
      unsigned char   pos_bit6;
      unsigned char   pos_bit7;
   } Si2164B_DD_INT_SENSE_PROP_struct;

   /* DD_INT_SENSE property, NEG_BIT0 field definition (NO TITLE)*/
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT0_LSB         0
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT0_MASK        0x01
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT0_DEFAULT    0
    #define Si2164B_DD_INT_SENSE_PROP_NEG_BIT0_DISABLE  0
    #define Si2164B_DD_INT_SENSE_PROP_NEG_BIT0_ENABLE   1

   /* DD_INT_SENSE property, NEG_BIT1 field definition (NO TITLE)*/
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT1_LSB         1
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT1_MASK        0x01
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT1_DEFAULT    0
    #define Si2164B_DD_INT_SENSE_PROP_NEG_BIT1_DISABLE  0
    #define Si2164B_DD_INT_SENSE_PROP_NEG_BIT1_ENABLE   1

   /* DD_INT_SENSE property, NEG_BIT2 field definition (NO TITLE)*/
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT2_LSB         2
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT2_MASK        0x01
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT2_DEFAULT    0
    #define Si2164B_DD_INT_SENSE_PROP_NEG_BIT2_DISABLE  0
    #define Si2164B_DD_INT_SENSE_PROP_NEG_BIT2_ENABLE   1

   /* DD_INT_SENSE property, NEG_BIT3 field definition (NO TITLE)*/
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT3_LSB         3
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT3_MASK        0x01
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT3_DEFAULT    0
    #define Si2164B_DD_INT_SENSE_PROP_NEG_BIT3_DISABLE  0
    #define Si2164B_DD_INT_SENSE_PROP_NEG_BIT3_ENABLE   1

   /* DD_INT_SENSE property, NEG_BIT4 field definition (NO TITLE)*/
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT4_LSB         4
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT4_MASK        0x01
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT4_DEFAULT    0
    #define Si2164B_DD_INT_SENSE_PROP_NEG_BIT4_DISABLE  0
    #define Si2164B_DD_INT_SENSE_PROP_NEG_BIT4_ENABLE   1

   /* DD_INT_SENSE property, NEG_BIT5 field definition (NO TITLE)*/
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT5_LSB         5
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT5_MASK        0x01
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT5_DEFAULT    0
    #define Si2164B_DD_INT_SENSE_PROP_NEG_BIT5_DISABLE  0
    #define Si2164B_DD_INT_SENSE_PROP_NEG_BIT5_ENABLE   1

   /* DD_INT_SENSE property, NEG_BIT6 field definition (NO TITLE)*/
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT6_LSB         6
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT6_MASK        0x01
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT6_DEFAULT    0
    #define Si2164B_DD_INT_SENSE_PROP_NEG_BIT6_DISABLE  0
    #define Si2164B_DD_INT_SENSE_PROP_NEG_BIT6_ENABLE   1

   /* DD_INT_SENSE property, NEG_BIT7 field definition (NO TITLE)*/
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT7_LSB         7
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT7_MASK        0x01
   #define  Si2164B_DD_INT_SENSE_PROP_NEG_BIT7_DEFAULT    0
    #define Si2164B_DD_INT_SENSE_PROP_NEG_BIT7_DISABLE  0
    #define Si2164B_DD_INT_SENSE_PROP_NEG_BIT7_ENABLE   1

   /* DD_INT_SENSE property, POS_BIT0 field definition (NO TITLE)*/
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT0_LSB         8
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT0_MASK        0x01
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT0_DEFAULT    1
    #define Si2164B_DD_INT_SENSE_PROP_POS_BIT0_DISABLE  0
    #define Si2164B_DD_INT_SENSE_PROP_POS_BIT0_ENABLE   1

   /* DD_INT_SENSE property, POS_BIT1 field definition (NO TITLE)*/
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT1_LSB         9
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT1_MASK        0x01
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT1_DEFAULT    1
    #define Si2164B_DD_INT_SENSE_PROP_POS_BIT1_DISABLE  0
    #define Si2164B_DD_INT_SENSE_PROP_POS_BIT1_ENABLE   1

   /* DD_INT_SENSE property, POS_BIT2 field definition (NO TITLE)*/
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT2_LSB         10
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT2_MASK        0x01
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT2_DEFAULT    1
    #define Si2164B_DD_INT_SENSE_PROP_POS_BIT2_DISABLE  0
    #define Si2164B_DD_INT_SENSE_PROP_POS_BIT2_ENABLE   1

   /* DD_INT_SENSE property, POS_BIT3 field definition (NO TITLE)*/
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT3_LSB         11
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT3_MASK        0x01
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT3_DEFAULT    1
    #define Si2164B_DD_INT_SENSE_PROP_POS_BIT3_DISABLE  0
    #define Si2164B_DD_INT_SENSE_PROP_POS_BIT3_ENABLE   1

   /* DD_INT_SENSE property, POS_BIT4 field definition (NO TITLE)*/
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT4_LSB         12
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT4_MASK        0x01
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT4_DEFAULT    1
    #define Si2164B_DD_INT_SENSE_PROP_POS_BIT4_DISABLE  0
    #define Si2164B_DD_INT_SENSE_PROP_POS_BIT4_ENABLE   1

   /* DD_INT_SENSE property, POS_BIT5 field definition (NO TITLE)*/
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT5_LSB         13
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT5_MASK        0x01
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT5_DEFAULT    1
    #define Si2164B_DD_INT_SENSE_PROP_POS_BIT5_DISABLE  0
    #define Si2164B_DD_INT_SENSE_PROP_POS_BIT5_ENABLE   1

   /* DD_INT_SENSE property, POS_BIT6 field definition (NO TITLE)*/
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT6_LSB         14
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT6_MASK        0x01
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT6_DEFAULT    1
    #define Si2164B_DD_INT_SENSE_PROP_POS_BIT6_DISABLE  0
    #define Si2164B_DD_INT_SENSE_PROP_POS_BIT6_ENABLE   1

   /* DD_INT_SENSE property, POS_BIT7 field definition (NO TITLE)*/
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT7_LSB         15
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT7_MASK        0x01
   #define  Si2164B_DD_INT_SENSE_PROP_POS_BIT7_DEFAULT    1
    #define Si2164B_DD_INT_SENSE_PROP_POS_BIT7_DISABLE  0
    #define Si2164B_DD_INT_SENSE_PROP_POS_BIT7_ENABLE   1

#endif /* Si2164B_DD_INT_SENSE_PROP */

/* Si2164B DD_MODE property definition */
#define   Si2164B_DD_MODE_PROP 0x100a

#ifdef    Si2164B_DD_MODE_PROP
  #define Si2164B_DD_MODE_PROP_CODE 0x00100a


    typedef struct { /* Si2164B_DD_MODE_PROP_struct */
      unsigned char   auto_detect;
      unsigned char   bw;
      unsigned char   invert_spectrum;
      unsigned char   modulation;
   } Si2164B_DD_MODE_PROP_struct;

   /* DD_MODE property, AUTO_DETECT field definition (NO TITLE)*/
   #define  Si2164B_DD_MODE_PROP_AUTO_DETECT_LSB         9
   #define  Si2164B_DD_MODE_PROP_AUTO_DETECT_MASK        0x07
   #define  Si2164B_DD_MODE_PROP_AUTO_DETECT_DEFAULT    0
    #define Si2164B_DD_MODE_PROP_AUTO_DETECT_NONE               0
    #define Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_T_T2      1
    #define Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_S_S2      2
    #define Si2164B_DD_MODE_PROP_AUTO_DETECT_AUTO_DVB_S_S2_DSS  3

   /* DD_MODE property, BW field definition (NO TITLE)*/
   #define  Si2164B_DD_MODE_PROP_BW_LSB         0
   #define  Si2164B_DD_MODE_PROP_BW_MASK        0x0f
   #define  Si2164B_DD_MODE_PROP_BW_DEFAULT    8
    #define Si2164B_DD_MODE_PROP_BW_BW_5MHZ    5
    #define Si2164B_DD_MODE_PROP_BW_BW_6MHZ    6
    #define Si2164B_DD_MODE_PROP_BW_BW_7MHZ    7
    #define Si2164B_DD_MODE_PROP_BW_BW_8MHZ    8
    #define Si2164B_DD_MODE_PROP_BW_BW_1D7MHZ  2

   /* DD_MODE property, INVERT_SPECTRUM field definition (NO TITLE)*/
   #define  Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_LSB         8
   #define  Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_MASK        0x01
   #define  Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_DEFAULT    0
    #define Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_NORMAL    0
    #define Si2164B_DD_MODE_PROP_INVERT_SPECTRUM_INVERTED  1

   /* DD_MODE property, MODULATION field definition (NO TITLE)*/
   #define  Si2164B_DD_MODE_PROP_MODULATION_LSB         4
   #define  Si2164B_DD_MODE_PROP_MODULATION_MASK        0x0f
   #define  Si2164B_DD_MODE_PROP_MODULATION_DEFAULT    2
    #define Si2164B_DD_MODE_PROP_MODULATION_MCNS         1
    #define Si2164B_DD_MODE_PROP_MODULATION_DVBT         2
    #define Si2164B_DD_MODE_PROP_MODULATION_DVBC         3
    #define Si2164B_DD_MODE_PROP_MODULATION_ISDBT        4
    #define Si2164B_DD_MODE_PROP_MODULATION_DVBT2        7
    #define Si2164B_DD_MODE_PROP_MODULATION_DVBS         8
    #define Si2164B_DD_MODE_PROP_MODULATION_DVBS2        9
    #define Si2164B_DD_MODE_PROP_MODULATION_DSS          10
    #define Si2164B_DD_MODE_PROP_MODULATION_DVBC2        11
    #define Si2164B_DD_MODE_PROP_MODULATION_AUTO_DETECT  15
    #define Si2164B_DD_MODE_PROP_MODULATION_ANALOG     100

#endif /* Si2164B_DD_MODE_PROP */

/* Si2164B DD_PER_RESOL property definition */
#define   Si2164B_DD_PER_RESOL_PROP 0x1004

#ifdef    Si2164B_DD_PER_RESOL_PROP
  #define Si2164B_DD_PER_RESOL_PROP_CODE 0x001004


    typedef struct { /* Si2164B_DD_PER_RESOL_PROP_struct */
      unsigned char   exp;
      unsigned char   mant;
   } Si2164B_DD_PER_RESOL_PROP_struct;

   /* DD_PER_RESOL property, EXP field definition (NO TITLE)*/
   #define  Si2164B_DD_PER_RESOL_PROP_EXP_LSB         0
   #define  Si2164B_DD_PER_RESOL_PROP_EXP_MASK        0x0f
   #define  Si2164B_DD_PER_RESOL_PROP_EXP_DEFAULT    5
    #define Si2164B_DD_PER_RESOL_PROP_EXP_EXPLO_MIN  1
    #define Si2164B_DD_PER_RESOL_PROP_EXP_EXPLO_MAX  9

   /* DD_PER_RESOL property, MANT field definition (NO TITLE)*/
   #define  Si2164B_DD_PER_RESOL_PROP_MANT_LSB         4
   #define  Si2164B_DD_PER_RESOL_PROP_MANT_MASK        0x0f
   #define  Si2164B_DD_PER_RESOL_PROP_MANT_DEFAULT    1
    #define Si2164B_DD_PER_RESOL_PROP_MANT_MANTLO_MIN  1
    #define Si2164B_DD_PER_RESOL_PROP_MANT_MANTLO_MAX  9

#endif /* Si2164B_DD_PER_RESOL_PROP */

/* Si2164B DD_RSQ_BER_THRESHOLD property definition */
#define   Si2164B_DD_RSQ_BER_THRESHOLD_PROP 0x1005

#ifdef    Si2164B_DD_RSQ_BER_THRESHOLD_PROP
  #define Si2164B_DD_RSQ_BER_THRESHOLD_PROP_CODE 0x001005


    typedef struct { /* Si2164B_DD_RSQ_BER_THRESHOLD_PROP_struct */
      unsigned char   exp;
      unsigned char   mant;
   } Si2164B_DD_RSQ_BER_THRESHOLD_PROP_struct;

   /* DD_RSQ_BER_THRESHOLD property, EXP field definition (NO TITLE)*/
   #define  Si2164B_DD_RSQ_BER_THRESHOLD_PROP_EXP_LSB         0
   #define  Si2164B_DD_RSQ_BER_THRESHOLD_PROP_EXP_MASK        0x0f
   #define  Si2164B_DD_RSQ_BER_THRESHOLD_PROP_EXP_DEFAULT    1
    #define Si2164B_DD_RSQ_BER_THRESHOLD_PROP_EXP_EXP_MIN  1
    #define Si2164B_DD_RSQ_BER_THRESHOLD_PROP_EXP_EXP_MAX  8

   /* DD_RSQ_BER_THRESHOLD property, MANT field definition (NO TITLE)*/
   #define  Si2164B_DD_RSQ_BER_THRESHOLD_PROP_MANT_LSB         4
   #define  Si2164B_DD_RSQ_BER_THRESHOLD_PROP_MANT_MASK        0x0f
   #define  Si2164B_DD_RSQ_BER_THRESHOLD_PROP_MANT_DEFAULT    10
    #define Si2164B_DD_RSQ_BER_THRESHOLD_PROP_MANT_MANT_MIN  0
    #define Si2164B_DD_RSQ_BER_THRESHOLD_PROP_MANT_MANT_MAX  99

#endif /* Si2164B_DD_RSQ_BER_THRESHOLD_PROP */

/* Si2164B DD_SEC_TS_SERIAL_DIFF property definition */
#define   Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP 0x1018

#ifdef    Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP
  #define Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_CODE 0x001018


    typedef struct { /* Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_struct */
      unsigned char   ts_clkb_on_data1;
      unsigned char   ts_data0b_on_data2;
      unsigned char   ts_data1_shape;
      unsigned char   ts_data1_strength;
      unsigned char   ts_data2_shape;
      unsigned char   ts_data2_strength;
   } Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_struct;

   /* DD_SEC_TS_SERIAL_DIFF property, TS_CLKB_ON_DATA1 field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_LSB         12
   #define  Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_MASK        0x01
   #define  Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_DEFAULT    0
    #define Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_DISABLE  0
    #define Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_ENABLE   1

   /* DD_SEC_TS_SERIAL_DIFF property, TS_DATA0B_ON_DATA2 field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_LSB         13
   #define  Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_MASK        0x01
   #define  Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_DEFAULT    0
    #define Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_DISABLE  0
    #define Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_ENABLE   1

   /* DD_SEC_TS_SERIAL_DIFF property, TS_DATA1_SHAPE field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA1_SHAPE_LSB         4
   #define  Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA1_SHAPE_MASK        0x03
   #define  Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA1_SHAPE_DEFAULT    3
   /* DD_SEC_TS_SERIAL_DIFF property, TS_DATA1_STRENGTH field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA1_STRENGTH_LSB         0
   #define  Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA1_STRENGTH_MASK        0x0f
   #define  Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA1_STRENGTH_DEFAULT    15
   /* DD_SEC_TS_SERIAL_DIFF property, TS_DATA2_SHAPE field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA2_SHAPE_LSB         10
   #define  Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA2_SHAPE_MASK        0x03
   #define  Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA2_SHAPE_DEFAULT    3
   /* DD_SEC_TS_SERIAL_DIFF property, TS_DATA2_STRENGTH field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA2_STRENGTH_LSB         6
   #define  Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA2_STRENGTH_MASK        0x0f
   #define  Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_TS_DATA2_STRENGTH_DEFAULT    15
#endif /* Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP */

/* Si2164B DD_SEC_TS_SETUP_PAR property definition */
#define   Si2164B_DD_SEC_TS_SETUP_PAR_PROP 0x1015

#ifdef    Si2164B_DD_SEC_TS_SETUP_PAR_PROP
  #define Si2164B_DD_SEC_TS_SETUP_PAR_PROP_CODE 0x001015


    typedef struct { /* Si2164B_DD_SEC_TS_SETUP_PAR_PROP_struct */
      unsigned char   ts_clk_shape;
      unsigned char   ts_clk_strength;
      unsigned char   ts_data_shape;
      unsigned char   ts_data_strength;
   } Si2164B_DD_SEC_TS_SETUP_PAR_PROP_struct;

   /* DD_SEC_TS_SETUP_PAR property, TS_CLK_SHAPE field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_CLK_SHAPE_LSB         10
   #define  Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_CLK_SHAPE_MASK        0x03
   #define  Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_CLK_SHAPE_DEFAULT    1
   /* DD_SEC_TS_SETUP_PAR property, TS_CLK_STRENGTH field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_CLK_STRENGTH_LSB         6
   #define  Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_CLK_STRENGTH_MASK        0x0f
   #define  Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_CLK_STRENGTH_DEFAULT    3
   /* DD_SEC_TS_SETUP_PAR property, TS_DATA_SHAPE field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_DATA_SHAPE_LSB         4
   #define  Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_DATA_SHAPE_MASK        0x03
   #define  Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_DATA_SHAPE_DEFAULT    1
   /* DD_SEC_TS_SETUP_PAR property, TS_DATA_STRENGTH field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_DATA_STRENGTH_LSB         0
   #define  Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_DATA_STRENGTH_MASK        0x0f
   #define  Si2164B_DD_SEC_TS_SETUP_PAR_PROP_TS_DATA_STRENGTH_DEFAULT    3
#endif /* Si2164B_DD_SEC_TS_SETUP_PAR_PROP */

/* Si2164B DD_SEC_TS_SETUP_SER property definition */
#define   Si2164B_DD_SEC_TS_SETUP_SER_PROP 0x1016

#ifdef    Si2164B_DD_SEC_TS_SETUP_SER_PROP
  #define Si2164B_DD_SEC_TS_SETUP_SER_PROP_CODE 0x001016


    typedef struct { /* Si2164B_DD_SEC_TS_SETUP_SER_PROP_struct */
      unsigned char   ts_clk_shape;
      unsigned char   ts_clk_strength;
      unsigned char   ts_data_shape;
      unsigned char   ts_data_strength;
   } Si2164B_DD_SEC_TS_SETUP_SER_PROP_struct;

   /* DD_SEC_TS_SETUP_SER property, TS_CLK_SHAPE field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_CLK_SHAPE_LSB         10
   #define  Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_CLK_SHAPE_MASK        0x03
   #define  Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_CLK_SHAPE_DEFAULT    3
   /* DD_SEC_TS_SETUP_SER property, TS_CLK_STRENGTH field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_CLK_STRENGTH_LSB         6
   #define  Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_CLK_STRENGTH_MASK        0x0f
   #define  Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_CLK_STRENGTH_DEFAULT    15
   /* DD_SEC_TS_SETUP_SER property, TS_DATA_SHAPE field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_DATA_SHAPE_LSB         4
   #define  Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_DATA_SHAPE_MASK        0x03
   #define  Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_DATA_SHAPE_DEFAULT    3
   /* DD_SEC_TS_SETUP_SER property, TS_DATA_STRENGTH field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_DATA_STRENGTH_LSB         0
   #define  Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_DATA_STRENGTH_MASK        0x0f
   #define  Si2164B_DD_SEC_TS_SETUP_SER_PROP_TS_DATA_STRENGTH_DEFAULT    3
#endif /* Si2164B_DD_SEC_TS_SETUP_SER_PROP */

/* Si2164B DD_SEC_TS_SLR_SERIAL property definition */
#define   Si2164B_DD_SEC_TS_SLR_SERIAL_PROP 0x1017

#ifdef    Si2164B_DD_SEC_TS_SLR_SERIAL_PROP
  #define Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_CODE 0x001017


    typedef struct { /* Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_struct */
      unsigned char   ts_clk_slr;
      unsigned char   ts_clk_slr_on;
      unsigned char   ts_data1_slr;
      unsigned char   ts_data1_slr_on;
      unsigned char   ts_data2_slr;
      unsigned char   ts_data2_slr_on;
      unsigned char   ts_data_slr;
      unsigned char   ts_data_slr_on;
   } Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_struct;

   /* DD_SEC_TS_SLR_SERIAL property, TS_CLK_SLR field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_CLK_SLR_LSB         12
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_CLK_SLR_MASK        0x07
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_CLK_SLR_DEFAULT    0
   /* DD_SEC_TS_SLR_SERIAL property, TS_CLK_SLR_ON field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_LSB         15
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_MASK        0x01
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_DEFAULT    0
    #define Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_DISABLE  0
    #define Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_ENABLE   1

   /* DD_SEC_TS_SLR_SERIAL property, TS_DATA1_SLR field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_LSB         4
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_MASK        0x07
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_DEFAULT    0
   /* DD_SEC_TS_SLR_SERIAL property, TS_DATA1_SLR_ON field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_LSB         7
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_MASK        0x01
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_DEFAULT    0
    #define Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_DISABLE  0
    #define Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_ENABLE   1

   /* DD_SEC_TS_SLR_SERIAL property, TS_DATA2_SLR field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_LSB         8
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_MASK        0x07
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_DEFAULT    0
   /* DD_SEC_TS_SLR_SERIAL property, TS_DATA2_SLR_ON field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_LSB         11
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_MASK        0x01
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_DEFAULT    0
    #define Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_DISABLE  0
    #define Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_ENABLE   1

   /* DD_SEC_TS_SLR_SERIAL property, TS_DATA_SLR field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA_SLR_LSB         0
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA_SLR_MASK        0x07
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA_SLR_DEFAULT    0
   /* DD_SEC_TS_SLR_SERIAL property, TS_DATA_SLR_ON field definition (NO TITLE)*/
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_LSB         3
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_MASK        0x01
   #define  Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_DEFAULT    0
    #define Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_DISABLE  0
    #define Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_ENABLE   1

#endif /* Si2164B_DD_SEC_TS_SLR_SERIAL_PROP */

/* Si2164B DD_SSI_SQI_PARAM property definition */
#define   Si2164B_DD_SSI_SQI_PARAM_PROP 0x100f

#ifdef    Si2164B_DD_SSI_SQI_PARAM_PROP
  #define Si2164B_DD_SSI_SQI_PARAM_PROP_CODE 0x00100f


    typedef struct { /* Si2164B_DD_SSI_SQI_PARAM_PROP_struct */
      unsigned char   sqi_average;
   } Si2164B_DD_SSI_SQI_PARAM_PROP_struct;

   /* DD_SSI_SQI_PARAM property, SQI_AVERAGE field definition (NO TITLE)*/
   #define  Si2164B_DD_SSI_SQI_PARAM_PROP_SQI_AVERAGE_LSB         0
   #define  Si2164B_DD_SSI_SQI_PARAM_PROP_SQI_AVERAGE_MASK        0x1f
   #define  Si2164B_DD_SSI_SQI_PARAM_PROP_SQI_AVERAGE_DEFAULT    1
    #define Si2164B_DD_SSI_SQI_PARAM_PROP_SQI_AVERAGE_SQI_AVERAGE_MIN  1
    #define Si2164B_DD_SSI_SQI_PARAM_PROP_SQI_AVERAGE_SQI_AVERAGE_MAX  30

#endif /* Si2164B_DD_SSI_SQI_PARAM_PROP */

/* Si2164B DD_TS_FREQ property definition */
#define   Si2164B_DD_TS_FREQ_PROP 0x100d

#ifdef    Si2164B_DD_TS_FREQ_PROP
  #define Si2164B_DD_TS_FREQ_PROP_CODE 0x00100d


    typedef struct { /* Si2164B_DD_TS_FREQ_PROP_struct */
      unsigned int    req_freq_10khz;
   } Si2164B_DD_TS_FREQ_PROP_struct;

   /* DD_TS_FREQ property, REQ_FREQ_10KHZ field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_FREQ_PROP_REQ_FREQ_10KHZ_LSB         0
   #define  Si2164B_DD_TS_FREQ_PROP_REQ_FREQ_10KHZ_MASK        0x3fff
   #define  Si2164B_DD_TS_FREQ_PROP_REQ_FREQ_10KHZ_DEFAULT    720
    #define Si2164B_DD_TS_FREQ_PROP_REQ_FREQ_10KHZ_REQ_FREQ_10KHZ_MIN  0
    #define Si2164B_DD_TS_FREQ_PROP_REQ_FREQ_10KHZ_REQ_FREQ_10KHZ_MAX  14550

#endif /* Si2164B_DD_TS_FREQ_PROP */

/* Si2164B DD_TS_FREQ_MAX property definition */
#define   Si2164B_DD_TS_FREQ_MAX_PROP 0x1019

#ifdef    Si2164B_DD_TS_FREQ_MAX_PROP
  #define Si2164B_DD_TS_FREQ_MAX_PROP_CODE 0x001019


    typedef struct { /* Si2164B_DD_TS_FREQ_MAX_PROP_struct */
      unsigned int    req_freq_max_10khz;
   } Si2164B_DD_TS_FREQ_MAX_PROP_struct;

   /* DD_TS_FREQ_MAX property, REQ_FREQ_MAX_10KHZ field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_FREQ_MAX_PROP_REQ_FREQ_MAX_10KHZ_LSB         0
   #define  Si2164B_DD_TS_FREQ_MAX_PROP_REQ_FREQ_MAX_10KHZ_MASK        0x3fff
   #define  Si2164B_DD_TS_FREQ_MAX_PROP_REQ_FREQ_MAX_10KHZ_DEFAULT    14550
    #define Si2164B_DD_TS_FREQ_MAX_PROP_REQ_FREQ_MAX_10KHZ_REQ_FREQ_MAX_10KHZ_MIN  0
    #define Si2164B_DD_TS_FREQ_MAX_PROP_REQ_FREQ_MAX_10KHZ_REQ_FREQ_MAX_10KHZ_MAX  14550

#endif /* Si2164B_DD_TS_FREQ_MAX_PROP */

/* Si2164B DD_TS_MODE property definition */
#define   Si2164B_DD_TS_MODE_PROP 0x1001

#ifdef    Si2164B_DD_TS_MODE_PROP
  #define Si2164B_DD_TS_MODE_PROP_CODE 0x001001


    typedef struct { /* Si2164B_DD_TS_MODE_PROP_struct */
      unsigned char   clk_gapped_en;
      unsigned char   clock;
      unsigned char   mode;
      unsigned char   serial_pin_selection;
      unsigned char   special;
      unsigned char   ts_err_polarity;
      unsigned char   ts_freq_resolution;
   } Si2164B_DD_TS_MODE_PROP_struct;

   /* DD_TS_MODE property, CLK_GAPPED_EN field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_MODE_PROP_CLK_GAPPED_EN_LSB         6
   #define  Si2164B_DD_TS_MODE_PROP_CLK_GAPPED_EN_MASK        0x01
   #define  Si2164B_DD_TS_MODE_PROP_CLK_GAPPED_EN_DEFAULT    0
    #define Si2164B_DD_TS_MODE_PROP_CLK_GAPPED_EN_DISABLED  0
    #define Si2164B_DD_TS_MODE_PROP_CLK_GAPPED_EN_ENABLED   1

   /* DD_TS_MODE property, CLOCK field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_MODE_PROP_CLOCK_LSB         4
   #define  Si2164B_DD_TS_MODE_PROP_CLOCK_MASK        0x03
   #define  Si2164B_DD_TS_MODE_PROP_CLOCK_DEFAULT    0
    #define Si2164B_DD_TS_MODE_PROP_CLOCK_AUTO_FIXED  0
    #define Si2164B_DD_TS_MODE_PROP_CLOCK_AUTO_ADAPT  1
    #define Si2164B_DD_TS_MODE_PROP_CLOCK_MANUAL      2

   /* DD_TS_MODE property, MODE field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_MODE_PROP_MODE_LSB         0
   #define  Si2164B_DD_TS_MODE_PROP_MODE_MASK        0x0f
   #define  Si2164B_DD_TS_MODE_PROP_MODE_DEFAULT    0
    #define Si2164B_DD_TS_MODE_PROP_MODE_TRISTATE  0
    #define Si2164B_DD_TS_MODE_PROP_MODE_OFF       1
    #define Si2164B_DD_TS_MODE_PROP_MODE_SERIAL    3
    #define Si2164B_DD_TS_MODE_PROP_MODE_PARALLEL  6
    #define Si2164B_DD_TS_MODE_PROP_MODE_GPIF      7

   /* DD_TS_MODE property, SERIAL_PIN_SELECTION field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_MODE_PROP_SERIAL_PIN_SELECTION_LSB         11
   #define  Si2164B_DD_TS_MODE_PROP_SERIAL_PIN_SELECTION_MASK        0x07
   #define  Si2164B_DD_TS_MODE_PROP_SERIAL_PIN_SELECTION_DEFAULT    0
    #define Si2164B_DD_TS_MODE_PROP_SERIAL_PIN_SELECTION_D0  0
    #define Si2164B_DD_TS_MODE_PROP_SERIAL_PIN_SELECTION_D7  7

   /* DD_TS_MODE property, SPECIAL field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_MODE_PROP_SPECIAL_LSB         8
   #define  Si2164B_DD_TS_MODE_PROP_SPECIAL_MASK        0x03
   #define  Si2164B_DD_TS_MODE_PROP_SPECIAL_DEFAULT    0
    #define Si2164B_DD_TS_MODE_PROP_SPECIAL_FULL_TS         0
    #define Si2164B_DD_TS_MODE_PROP_SPECIAL_DATAS_TRISTATE  1

   /* DD_TS_MODE property, TS_ERR_POLARITY field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_MODE_PROP_TS_ERR_POLARITY_LSB         7
   #define  Si2164B_DD_TS_MODE_PROP_TS_ERR_POLARITY_MASK        0x01
   #define  Si2164B_DD_TS_MODE_PROP_TS_ERR_POLARITY_DEFAULT    0
    #define Si2164B_DD_TS_MODE_PROP_TS_ERR_POLARITY_NOT_INVERTED  0
    #define Si2164B_DD_TS_MODE_PROP_TS_ERR_POLARITY_INVERTED      1

   /* DD_TS_MODE property, TS_FREQ_RESOLUTION field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_MODE_PROP_TS_FREQ_RESOLUTION_LSB         10
   #define  Si2164B_DD_TS_MODE_PROP_TS_FREQ_RESOLUTION_MASK        0x01
   #define  Si2164B_DD_TS_MODE_PROP_TS_FREQ_RESOLUTION_DEFAULT    0
    #define Si2164B_DD_TS_MODE_PROP_TS_FREQ_RESOLUTION_NORMAL  0
    #define Si2164B_DD_TS_MODE_PROP_TS_FREQ_RESOLUTION_FINE    1

#endif /* Si2164B_DD_TS_MODE_PROP */

/* Si2164B DD_TS_SERIAL_DIFF property definition */
#define   Si2164B_DD_TS_SERIAL_DIFF_PROP 0x1012

#ifdef    Si2164B_DD_TS_SERIAL_DIFF_PROP
  #define Si2164B_DD_TS_SERIAL_DIFF_PROP_CODE 0x001012


    typedef struct { /* Si2164B_DD_TS_SERIAL_DIFF_PROP_struct */
      unsigned char   ts_clkb_on_data1;
      unsigned char   ts_data0b_on_data2;
      unsigned char   ts_data1_shape;
      unsigned char   ts_data1_strength;
      unsigned char   ts_data2_shape;
      unsigned char   ts_data2_strength;
   } Si2164B_DD_TS_SERIAL_DIFF_PROP_struct;

   /* DD_TS_SERIAL_DIFF property, TS_CLKB_ON_DATA1 field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_LSB         12
   #define  Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_MASK        0x01
   #define  Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_DEFAULT    0
    #define Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_DISABLE  0
    #define Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_CLKB_ON_DATA1_ENABLE   1

   /* DD_TS_SERIAL_DIFF property, TS_DATA0B_ON_DATA2 field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_LSB         13
   #define  Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_MASK        0x01
   #define  Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_DEFAULT    0
    #define Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_DISABLE  0
    #define Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA0B_ON_DATA2_ENABLE   1

   /* DD_TS_SERIAL_DIFF property, TS_DATA1_SHAPE field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA1_SHAPE_LSB         4
   #define  Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA1_SHAPE_MASK        0x03
   #define  Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA1_SHAPE_DEFAULT    3
   /* DD_TS_SERIAL_DIFF property, TS_DATA1_STRENGTH field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA1_STRENGTH_LSB         0
   #define  Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA1_STRENGTH_MASK        0x0f
   #define  Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA1_STRENGTH_DEFAULT    15
   /* DD_TS_SERIAL_DIFF property, TS_DATA2_SHAPE field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA2_SHAPE_LSB         10
   #define  Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA2_SHAPE_MASK        0x03
   #define  Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA2_SHAPE_DEFAULT    3
   /* DD_TS_SERIAL_DIFF property, TS_DATA2_STRENGTH field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA2_STRENGTH_LSB         6
   #define  Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA2_STRENGTH_MASK        0x0f
   #define  Si2164B_DD_TS_SERIAL_DIFF_PROP_TS_DATA2_STRENGTH_DEFAULT    15
#endif /* Si2164B_DD_TS_SERIAL_DIFF_PROP */

/* Si2164B DD_TS_SETUP_PAR property definition */
#define   Si2164B_DD_TS_SETUP_PAR_PROP 0x1009

#ifdef    Si2164B_DD_TS_SETUP_PAR_PROP
  #define Si2164B_DD_TS_SETUP_PAR_PROP_CODE 0x001009


    typedef struct { /* Si2164B_DD_TS_SETUP_PAR_PROP_struct */
      unsigned char   ts_clk_invert;
      unsigned char   ts_clk_shape;
      unsigned char   ts_clk_shift;
      unsigned char   ts_clk_strength;
      unsigned char   ts_data_shape;
      unsigned char   ts_data_strength;
   } Si2164B_DD_TS_SETUP_PAR_PROP_struct;

   /* DD_TS_SETUP_PAR property, TS_CLK_INVERT field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_INVERT_LSB         12
   #define  Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_INVERT_MASK        0x01
   #define  Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_INVERT_DEFAULT    1
    #define Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_INVERT_NOT_INVERTED  0
    #define Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_INVERT_INVERTED      1

   /* DD_TS_SETUP_PAR property, TS_CLK_SHAPE field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_SHAPE_LSB         10
   #define  Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_SHAPE_MASK        0x03
   #define  Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_SHAPE_DEFAULT    1
   /* DD_TS_SETUP_PAR property, TS_CLK_SHIFT field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_SHIFT_LSB         13
   #define  Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_SHIFT_MASK        0x07
   #define  Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_SHIFT_DEFAULT    0
   /* DD_TS_SETUP_PAR property, TS_CLK_STRENGTH field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_STRENGTH_LSB         6
   #define  Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_STRENGTH_MASK        0x0f
   #define  Si2164B_DD_TS_SETUP_PAR_PROP_TS_CLK_STRENGTH_DEFAULT    3
   /* DD_TS_SETUP_PAR property, TS_DATA_SHAPE field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SETUP_PAR_PROP_TS_DATA_SHAPE_LSB         4
   #define  Si2164B_DD_TS_SETUP_PAR_PROP_TS_DATA_SHAPE_MASK        0x03
   #define  Si2164B_DD_TS_SETUP_PAR_PROP_TS_DATA_SHAPE_DEFAULT    1
   /* DD_TS_SETUP_PAR property, TS_DATA_STRENGTH field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SETUP_PAR_PROP_TS_DATA_STRENGTH_LSB         0
   #define  Si2164B_DD_TS_SETUP_PAR_PROP_TS_DATA_STRENGTH_MASK        0x0f
   #define  Si2164B_DD_TS_SETUP_PAR_PROP_TS_DATA_STRENGTH_DEFAULT    3
#endif /* Si2164B_DD_TS_SETUP_PAR_PROP */

/* Si2164B DD_TS_SETUP_SER property definition */
#define   Si2164B_DD_TS_SETUP_SER_PROP 0x1008

#ifdef    Si2164B_DD_TS_SETUP_SER_PROP
  #define Si2164B_DD_TS_SETUP_SER_PROP_CODE 0x001008


    typedef struct { /* Si2164B_DD_TS_SETUP_SER_PROP_struct */
      unsigned char   ts_byte_order;
      unsigned char   ts_clk_invert;
      unsigned char   ts_clk_shape;
      unsigned char   ts_clk_strength;
      unsigned char   ts_data_shape;
      unsigned char   ts_data_strength;
      unsigned char   ts_sync_duration;
   } Si2164B_DD_TS_SETUP_SER_PROP_struct;

   /* DD_TS_SETUP_SER property, TS_BYTE_ORDER field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_BYTE_ORDER_LSB         14
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_BYTE_ORDER_MASK        0x01
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_BYTE_ORDER_DEFAULT    0
    #define Si2164B_DD_TS_SETUP_SER_PROP_TS_BYTE_ORDER_MSB_FIRST  0
    #define Si2164B_DD_TS_SETUP_SER_PROP_TS_BYTE_ORDER_LSB_FIRST  1

   /* DD_TS_SETUP_SER property, TS_CLK_INVERT field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_INVERT_LSB         12
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_INVERT_MASK        0x01
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_INVERT_DEFAULT    1
    #define Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_INVERT_NOT_INVERTED  0
    #define Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_INVERT_INVERTED      1

   /* DD_TS_SETUP_SER property, TS_CLK_SHAPE field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_SHAPE_LSB         10
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_SHAPE_MASK        0x03
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_SHAPE_DEFAULT    3
   /* DD_TS_SETUP_SER property, TS_CLK_STRENGTH field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_STRENGTH_LSB         6
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_STRENGTH_MASK        0x0f
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_CLK_STRENGTH_DEFAULT    15
   /* DD_TS_SETUP_SER property, TS_DATA_SHAPE field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_DATA_SHAPE_LSB         4
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_DATA_SHAPE_MASK        0x03
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_DATA_SHAPE_DEFAULT    3
   /* DD_TS_SETUP_SER property, TS_DATA_STRENGTH field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_DATA_STRENGTH_LSB         0
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_DATA_STRENGTH_MASK        0x0f
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_DATA_STRENGTH_DEFAULT    15
   /* DD_TS_SETUP_SER property, TS_SYNC_DURATION field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_SYNC_DURATION_LSB         13
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_SYNC_DURATION_MASK        0x01
   #define  Si2164B_DD_TS_SETUP_SER_PROP_TS_SYNC_DURATION_DEFAULT    0
    #define Si2164B_DD_TS_SETUP_SER_PROP_TS_SYNC_DURATION_FIRST_BYTE  0
    #define Si2164B_DD_TS_SETUP_SER_PROP_TS_SYNC_DURATION_FIRST_BIT   1

#endif /* Si2164B_DD_TS_SETUP_SER_PROP */

/* Si2164B DD_TS_SLR_SERIAL property definition */
#define   Si2164B_DD_TS_SLR_SERIAL_PROP 0x1014

#ifdef    Si2164B_DD_TS_SLR_SERIAL_PROP
  #define Si2164B_DD_TS_SLR_SERIAL_PROP_CODE 0x001014


    typedef struct { /* Si2164B_DD_TS_SLR_SERIAL_PROP_struct */
      unsigned char   ts_clk_slr;
      unsigned char   ts_clk_slr_on;
      unsigned char   ts_data1_slr;
      unsigned char   ts_data1_slr_on;
      unsigned char   ts_data2_slr;
      unsigned char   ts_data2_slr_on;
      unsigned char   ts_data_slr;
      unsigned char   ts_data_slr_on;
   } Si2164B_DD_TS_SLR_SERIAL_PROP_struct;

   /* DD_TS_SLR_SERIAL property, TS_CLK_SLR field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_CLK_SLR_LSB         12
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_CLK_SLR_MASK        0x07
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_CLK_SLR_DEFAULT    0
   /* DD_TS_SLR_SERIAL property, TS_CLK_SLR_ON field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_LSB         15
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_MASK        0x01
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_DEFAULT    0
    #define Si2164B_DD_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_DISABLE  0
    #define Si2164B_DD_TS_SLR_SERIAL_PROP_TS_CLK_SLR_ON_ENABLE   1

   /* DD_TS_SLR_SERIAL property, TS_DATA1_SLR field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_LSB         4
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_MASK        0x07
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_DEFAULT    0
   /* DD_TS_SLR_SERIAL property, TS_DATA1_SLR_ON field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_LSB         7
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_MASK        0x01
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_DEFAULT    0
    #define Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_DISABLE  0
    #define Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA1_SLR_ON_ENABLE   1

   /* DD_TS_SLR_SERIAL property, TS_DATA2_SLR field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_LSB         8
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_MASK        0x07
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_DEFAULT    0
   /* DD_TS_SLR_SERIAL property, TS_DATA2_SLR_ON field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_LSB         11
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_MASK        0x01
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_DEFAULT    0
    #define Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_DISABLE  0
    #define Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA2_SLR_ON_ENABLE   1

   /* DD_TS_SLR_SERIAL property, TS_DATA_SLR field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA_SLR_LSB         0
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA_SLR_MASK        0x07
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA_SLR_DEFAULT    0
   /* DD_TS_SLR_SERIAL property, TS_DATA_SLR_ON field definition (NO TITLE)*/
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_LSB         3
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_MASK        0x01
   #define  Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_DEFAULT    0
    #define Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_DISABLE  0
    #define Si2164B_DD_TS_SLR_SERIAL_PROP_TS_DATA_SLR_ON_ENABLE   1

#endif /* Si2164B_DD_TS_SLR_SERIAL_PROP */

/* Si2164B DVBC2_ADC_CREST_FACTOR property definition */
#define   Si2164B_DVBC2_ADC_CREST_FACTOR_PROP 0x1702

#ifdef    Si2164B_DVBC2_ADC_CREST_FACTOR_PROP
  #define Si2164B_DVBC2_ADC_CREST_FACTOR_PROP_CODE 0x001702


    typedef struct { /* Si2164B_DVBC2_ADC_CREST_FACTOR_PROP_struct */
      unsigned char   crest_factor;
   } Si2164B_DVBC2_ADC_CREST_FACTOR_PROP_struct;

   /* DVBC2_ADC_CREST_FACTOR property, CREST_FACTOR field definition (NO TITLE)*/
   #define  Si2164B_DVBC2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB         0
   #define  Si2164B_DVBC2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK        0xff
   #define  Si2164B_DVBC2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_DEFAULT    130
#endif /* Si2164B_DVBC2_ADC_CREST_FACTOR_PROP */

/* Si2164B DVBC2_AFC_RANGE property definition */
#define   Si2164B_DVBC2_AFC_RANGE_PROP 0x1701

#ifdef    Si2164B_DVBC2_AFC_RANGE_PROP
  #define Si2164B_DVBC2_AFC_RANGE_PROP_CODE 0x001701


    typedef struct { /* Si2164B_DVBC2_AFC_RANGE_PROP_struct */
      unsigned int    range_khz;
   } Si2164B_DVBC2_AFC_RANGE_PROP_struct;

   /* DVBC2_AFC_RANGE property, RANGE_KHZ field definition (NO TITLE)*/
   #define  Si2164B_DVBC2_AFC_RANGE_PROP_RANGE_KHZ_LSB         0
   #define  Si2164B_DVBC2_AFC_RANGE_PROP_RANGE_KHZ_MASK        0xffff
   #define  Si2164B_DVBC2_AFC_RANGE_PROP_RANGE_KHZ_DEFAULT    550
#endif /* Si2164B_DVBC2_AFC_RANGE_PROP */


/* Si2164B DVBC_ADC_CREST_FACTOR property definition */
#define   Si2164B_DVBC_ADC_CREST_FACTOR_PROP 0x1104

#ifdef    Si2164B_DVBC_ADC_CREST_FACTOR_PROP
  #define Si2164B_DVBC_ADC_CREST_FACTOR_PROP_CODE 0x001104


    typedef struct { /* Si2164B_DVBC_ADC_CREST_FACTOR_PROP_struct */
      unsigned char   crest_factor;
   } Si2164B_DVBC_ADC_CREST_FACTOR_PROP_struct;

   /* DVBC_ADC_CREST_FACTOR property, CREST_FACTOR field definition (NO TITLE)*/
   #define  Si2164B_DVBC_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB         0
   #define  Si2164B_DVBC_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK        0xff
   #define  Si2164B_DVBC_ADC_CREST_FACTOR_PROP_CREST_FACTOR_DEFAULT    112
#endif /* Si2164B_DVBC_ADC_CREST_FACTOR_PROP */

/* Si2164B DVBC_AFC_RANGE property definition */
#define   Si2164B_DVBC_AFC_RANGE_PROP 0x1103

#ifdef    Si2164B_DVBC_AFC_RANGE_PROP
  #define Si2164B_DVBC_AFC_RANGE_PROP_CODE 0x001103


    typedef struct { /* Si2164B_DVBC_AFC_RANGE_PROP_struct */
      unsigned int    range_khz;
   } Si2164B_DVBC_AFC_RANGE_PROP_struct;

   /* DVBC_AFC_RANGE property, RANGE_KHZ field definition (NO TITLE)*/
   #define  Si2164B_DVBC_AFC_RANGE_PROP_RANGE_KHZ_LSB         0
   #define  Si2164B_DVBC_AFC_RANGE_PROP_RANGE_KHZ_MASK        0xffff
   #define  Si2164B_DVBC_AFC_RANGE_PROP_RANGE_KHZ_DEFAULT    100
#endif /* Si2164B_DVBC_AFC_RANGE_PROP */

/* Si2164B DVBC_CONSTELLATION property definition */
#define   Si2164B_DVBC_CONSTELLATION_PROP 0x1101

#ifdef    Si2164B_DVBC_CONSTELLATION_PROP
  #define Si2164B_DVBC_CONSTELLATION_PROP_CODE 0x001101


    typedef struct { /* Si2164B_DVBC_CONSTELLATION_PROP_struct */
      unsigned char   constellation;
   } Si2164B_DVBC_CONSTELLATION_PROP_struct;

   /* DVBC_CONSTELLATION property, CONSTELLATION field definition (NO TITLE)*/
   #define  Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_LSB         0
   #define  Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_MASK        0x3f
   #define  Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_DEFAULT    0
    #define Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_AUTO    0
    #define Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM16   7
    #define Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM32   8
    #define Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM64   9
    #define Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM128  10
    #define Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM256  11

#endif /* Si2164B_DVBC_CONSTELLATION_PROP */

/* Si2164B DVBC_SYMBOL_RATE property definition */
#define   Si2164B_DVBC_SYMBOL_RATE_PROP 0x1102

#ifdef    Si2164B_DVBC_SYMBOL_RATE_PROP
  #define Si2164B_DVBC_SYMBOL_RATE_PROP_CODE 0x001102


    typedef struct { /* Si2164B_DVBC_SYMBOL_RATE_PROP_struct */
      unsigned int    rate;
   } Si2164B_DVBC_SYMBOL_RATE_PROP_struct;

   /* DVBC_SYMBOL_RATE property, RATE field definition (NO TITLE)*/
   #define  Si2164B_DVBC_SYMBOL_RATE_PROP_RATE_LSB         0
   #define  Si2164B_DVBC_SYMBOL_RATE_PROP_RATE_MASK        0xffff
   #define  Si2164B_DVBC_SYMBOL_RATE_PROP_RATE_DEFAULT    6900
#endif /* Si2164B_DVBC_SYMBOL_RATE_PROP */


/* Si2164B DVBS2_ADC_CREST_FACTOR property definition */
#define   Si2164B_DVBS2_ADC_CREST_FACTOR_PROP 0x1403

#ifdef    Si2164B_DVBS2_ADC_CREST_FACTOR_PROP
  #define Si2164B_DVBS2_ADC_CREST_FACTOR_PROP_CODE 0x001403


    typedef struct { /* Si2164B_DVBS2_ADC_CREST_FACTOR_PROP_struct */
      unsigned char   crest_factor;
   } Si2164B_DVBS2_ADC_CREST_FACTOR_PROP_struct;

   /* DVBS2_ADC_CREST_FACTOR property, CREST_FACTOR field definition (NO TITLE)*/
   #define  Si2164B_DVBS2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB         0
   #define  Si2164B_DVBS2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK        0xff
   #define  Si2164B_DVBS2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_DEFAULT    104
#endif /* Si2164B_DVBS2_ADC_CREST_FACTOR_PROP */

/* Si2164B DVBS2_AFC_RANGE property definition */
#define   Si2164B_DVBS2_AFC_RANGE_PROP 0x1402

#ifdef    Si2164B_DVBS2_AFC_RANGE_PROP
  #define Si2164B_DVBS2_AFC_RANGE_PROP_CODE 0x001402


    typedef struct { /* Si2164B_DVBS2_AFC_RANGE_PROP_struct */
      unsigned int    range_khz;
   } Si2164B_DVBS2_AFC_RANGE_PROP_struct;

   /* DVBS2_AFC_RANGE property, RANGE_KHZ field definition (NO TITLE)*/
   #define  Si2164B_DVBS2_AFC_RANGE_PROP_RANGE_KHZ_LSB         0
   #define  Si2164B_DVBS2_AFC_RANGE_PROP_RANGE_KHZ_MASK        0xffff
   #define  Si2164B_DVBS2_AFC_RANGE_PROP_RANGE_KHZ_DEFAULT    4000
#endif /* Si2164B_DVBS2_AFC_RANGE_PROP */

/* Si2164B DVBS2_SYMBOL_RATE property definition */
#define   Si2164B_DVBS2_SYMBOL_RATE_PROP 0x1401

#ifdef    Si2164B_DVBS2_SYMBOL_RATE_PROP
  #define Si2164B_DVBS2_SYMBOL_RATE_PROP_CODE 0x001401


    typedef struct { /* Si2164B_DVBS2_SYMBOL_RATE_PROP_struct */
      unsigned int    rate;
   } Si2164B_DVBS2_SYMBOL_RATE_PROP_struct;

   /* DVBS2_SYMBOL_RATE property, RATE field definition (NO TITLE)*/
   #define  Si2164B_DVBS2_SYMBOL_RATE_PROP_RATE_LSB         0
   #define  Si2164B_DVBS2_SYMBOL_RATE_PROP_RATE_MASK        0xffff
   #define  Si2164B_DVBS2_SYMBOL_RATE_PROP_RATE_DEFAULT    27500
#endif /* Si2164B_DVBS2_SYMBOL_RATE_PROP */

/* Si2164B DVBS_ADC_CREST_FACTOR property definition */
#define   Si2164B_DVBS_ADC_CREST_FACTOR_PROP 0x1503

#ifdef    Si2164B_DVBS_ADC_CREST_FACTOR_PROP
  #define Si2164B_DVBS_ADC_CREST_FACTOR_PROP_CODE 0x001503


    typedef struct { /* Si2164B_DVBS_ADC_CREST_FACTOR_PROP_struct */
      unsigned char   crest_factor;
   } Si2164B_DVBS_ADC_CREST_FACTOR_PROP_struct;

   /* DVBS_ADC_CREST_FACTOR property, CREST_FACTOR field definition (NO TITLE)*/
   #define  Si2164B_DVBS_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB         0
   #define  Si2164B_DVBS_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK        0xff
   #define  Si2164B_DVBS_ADC_CREST_FACTOR_PROP_CREST_FACTOR_DEFAULT    104
#endif /* Si2164B_DVBS_ADC_CREST_FACTOR_PROP */

/* Si2164B DVBS_AFC_RANGE property definition */
#define   Si2164B_DVBS_AFC_RANGE_PROP 0x1502

#ifdef    Si2164B_DVBS_AFC_RANGE_PROP
  #define Si2164B_DVBS_AFC_RANGE_PROP_CODE 0x001502


    typedef struct { /* Si2164B_DVBS_AFC_RANGE_PROP_struct */
      unsigned int    range_khz;
   } Si2164B_DVBS_AFC_RANGE_PROP_struct;

   /* DVBS_AFC_RANGE property, RANGE_KHZ field definition (NO TITLE)*/
   #define  Si2164B_DVBS_AFC_RANGE_PROP_RANGE_KHZ_LSB         0
   #define  Si2164B_DVBS_AFC_RANGE_PROP_RANGE_KHZ_MASK        0xffff
   #define  Si2164B_DVBS_AFC_RANGE_PROP_RANGE_KHZ_DEFAULT    4000
#endif /* Si2164B_DVBS_AFC_RANGE_PROP */

/* Si2164B DVBS_SYMBOL_RATE property definition */
#define   Si2164B_DVBS_SYMBOL_RATE_PROP 0x1501

#ifdef    Si2164B_DVBS_SYMBOL_RATE_PROP
  #define Si2164B_DVBS_SYMBOL_RATE_PROP_CODE 0x001501


    typedef struct { /* Si2164B_DVBS_SYMBOL_RATE_PROP_struct */
      unsigned int    rate;
   } Si2164B_DVBS_SYMBOL_RATE_PROP_struct;

   /* DVBS_SYMBOL_RATE property, RATE field definition (NO TITLE)*/
   #define  Si2164B_DVBS_SYMBOL_RATE_PROP_RATE_LSB         0
   #define  Si2164B_DVBS_SYMBOL_RATE_PROP_RATE_MASK        0xffff
   #define  Si2164B_DVBS_SYMBOL_RATE_PROP_RATE_DEFAULT    27500
#endif /* Si2164B_DVBS_SYMBOL_RATE_PROP */


/* Si2164B DVBT2_ADC_CREST_FACTOR property definition */
#define   Si2164B_DVBT2_ADC_CREST_FACTOR_PROP 0x1303

#ifdef    Si2164B_DVBT2_ADC_CREST_FACTOR_PROP
  #define Si2164B_DVBT2_ADC_CREST_FACTOR_PROP_CODE 0x001303


    typedef struct { /* Si2164B_DVBT2_ADC_CREST_FACTOR_PROP_struct */
      unsigned char   crest_factor;
   } Si2164B_DVBT2_ADC_CREST_FACTOR_PROP_struct;

   /* DVBT2_ADC_CREST_FACTOR property, CREST_FACTOR field definition (NO TITLE)*/
   #define  Si2164B_DVBT2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB         0
   #define  Si2164B_DVBT2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK        0xff
   #define  Si2164B_DVBT2_ADC_CREST_FACTOR_PROP_CREST_FACTOR_DEFAULT    130
#endif /* Si2164B_DVBT2_ADC_CREST_FACTOR_PROP */

/* Si2164B DVBT2_AFC_RANGE property definition */
#define   Si2164B_DVBT2_AFC_RANGE_PROP 0x1301

#ifdef    Si2164B_DVBT2_AFC_RANGE_PROP
  #define Si2164B_DVBT2_AFC_RANGE_PROP_CODE 0x001301


    typedef struct { /* Si2164B_DVBT2_AFC_RANGE_PROP_struct */
      unsigned int    range_khz;
   } Si2164B_DVBT2_AFC_RANGE_PROP_struct;

   /* DVBT2_AFC_RANGE property, RANGE_KHZ field definition (NO TITLE)*/
   #define  Si2164B_DVBT2_AFC_RANGE_PROP_RANGE_KHZ_LSB         0
   #define  Si2164B_DVBT2_AFC_RANGE_PROP_RANGE_KHZ_MASK        0xffff
   #define  Si2164B_DVBT2_AFC_RANGE_PROP_RANGE_KHZ_DEFAULT    550
#endif /* Si2164B_DVBT2_AFC_RANGE_PROP */

/* Si2164B DVBT2_FEF_TUNER property definition */
#define   Si2164B_DVBT2_FEF_TUNER_PROP 0x1302

#ifdef    Si2164B_DVBT2_FEF_TUNER_PROP
  #define Si2164B_DVBT2_FEF_TUNER_PROP_CODE 0x001302


    typedef struct { /* Si2164B_DVBT2_FEF_TUNER_PROP_struct */
      unsigned char   tuner_delay;
      unsigned char   tuner_freeze_time;
      unsigned char   tuner_unfreeze_time;
   } Si2164B_DVBT2_FEF_TUNER_PROP_struct;

   /* DVBT2_FEF_TUNER property, TUNER_DELAY field definition (NO TITLE)*/
   #define  Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_DELAY_LSB         0
   #define  Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_DELAY_MASK        0xff
   #define  Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_DELAY_DEFAULT    1
   /* DVBT2_FEF_TUNER property, TUNER_FREEZE_TIME field definition (NO TITLE)*/
   #define  Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_FREEZE_TIME_LSB         8
   #define  Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_FREEZE_TIME_MASK        0x0f
   #define  Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_FREEZE_TIME_DEFAULT    1
   /* DVBT2_FEF_TUNER property, TUNER_UNFREEZE_TIME field definition (NO TITLE)*/
   #define  Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_UNFREEZE_TIME_LSB         12
   #define  Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_UNFREEZE_TIME_MASK        0x0f
   #define  Si2164B_DVBT2_FEF_TUNER_PROP_TUNER_UNFREEZE_TIME_DEFAULT    1
#endif /* Si2164B_DVBT2_FEF_TUNER_PROP */

/* Si2164B DVBT2_MODE property definition */
#define   Si2164B_DVBT2_MODE_PROP 0x1304

#ifdef    Si2164B_DVBT2_MODE_PROP
  #define Si2164B_DVBT2_MODE_PROP_CODE 0x001304


    typedef struct { /* Si2164B_DVBT2_MODE_PROP_struct */
      unsigned char   lock_mode;
   } Si2164B_DVBT2_MODE_PROP_struct;

   /* DVBT2_MODE property, LOCK_MODE field definition (NO TITLE)*/
   #define  Si2164B_DVBT2_MODE_PROP_LOCK_MODE_LSB         0
   #define  Si2164B_DVBT2_MODE_PROP_LOCK_MODE_MASK        0x03
   #define  Si2164B_DVBT2_MODE_PROP_LOCK_MODE_DEFAULT    0
    #define Si2164B_DVBT2_MODE_PROP_LOCK_MODE_ANY        0
    #define Si2164B_DVBT2_MODE_PROP_LOCK_MODE_BASE_ONLY  1
    #define Si2164B_DVBT2_MODE_PROP_LOCK_MODE_LITE_ONLY  2
    #define Si2164B_DVBT2_MODE_PROP_LOCK_MODE_RESERVED   3

#endif /* Si2164B_DVBT2_MODE_PROP */


/* Si2164B DVBT_ADC_CREST_FACTOR property definition */
#define   Si2164B_DVBT_ADC_CREST_FACTOR_PROP 0x1203

#ifdef    Si2164B_DVBT_ADC_CREST_FACTOR_PROP
  #define Si2164B_DVBT_ADC_CREST_FACTOR_PROP_CODE 0x001203


    typedef struct { /* Si2164B_DVBT_ADC_CREST_FACTOR_PROP_struct */
      unsigned char   crest_factor;
   } Si2164B_DVBT_ADC_CREST_FACTOR_PROP_struct;

   /* DVBT_ADC_CREST_FACTOR property, CREST_FACTOR field definition (NO TITLE)*/
   #define  Si2164B_DVBT_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB         0
   #define  Si2164B_DVBT_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK        0xff
   #define  Si2164B_DVBT_ADC_CREST_FACTOR_PROP_CREST_FACTOR_DEFAULT    130
#endif /* Si2164B_DVBT_ADC_CREST_FACTOR_PROP */

/* Si2164B DVBT_AFC_RANGE property definition */
#define   Si2164B_DVBT_AFC_RANGE_PROP 0x1202

#ifdef    Si2164B_DVBT_AFC_RANGE_PROP
  #define Si2164B_DVBT_AFC_RANGE_PROP_CODE 0x001202


    typedef struct { /* Si2164B_DVBT_AFC_RANGE_PROP_struct */
      unsigned int    range_khz;
   } Si2164B_DVBT_AFC_RANGE_PROP_struct;

   /* DVBT_AFC_RANGE property, RANGE_KHZ field definition (NO TITLE)*/
   #define  Si2164B_DVBT_AFC_RANGE_PROP_RANGE_KHZ_LSB         0
   #define  Si2164B_DVBT_AFC_RANGE_PROP_RANGE_KHZ_MASK        0xffff
   #define  Si2164B_DVBT_AFC_RANGE_PROP_RANGE_KHZ_DEFAULT    550
#endif /* Si2164B_DVBT_AFC_RANGE_PROP */

/* Si2164B DVBT_HIERARCHY property definition */
#define   Si2164B_DVBT_HIERARCHY_PROP 0x1201

#ifdef    Si2164B_DVBT_HIERARCHY_PROP
  #define Si2164B_DVBT_HIERARCHY_PROP_CODE 0x001201


    typedef struct { /* Si2164B_DVBT_HIERARCHY_PROP_struct */
      unsigned char   stream;
   } Si2164B_DVBT_HIERARCHY_PROP_struct;

   /* DVBT_HIERARCHY property, STREAM field definition (NO TITLE)*/
   #define  Si2164B_DVBT_HIERARCHY_PROP_STREAM_LSB         0
   #define  Si2164B_DVBT_HIERARCHY_PROP_STREAM_MASK        0x01
   #define  Si2164B_DVBT_HIERARCHY_PROP_STREAM_DEFAULT    0
    #define Si2164B_DVBT_HIERARCHY_PROP_STREAM_HP  0
    #define Si2164B_DVBT_HIERARCHY_PROP_STREAM_LP  1

#endif /* Si2164B_DVBT_HIERARCHY_PROP */



/* Si2164B MASTER_IEN property definition */
#define   Si2164B_MASTER_IEN_PROP 0x0401

#ifdef    Si2164B_MASTER_IEN_PROP
  #define Si2164B_MASTER_IEN_PROP_CODE 0x000401


    typedef struct { /* Si2164B_MASTER_IEN_PROP_struct */
      unsigned char   ctsien;
      unsigned char   ddien;
      unsigned char   errien;
      unsigned char   scanien;
   } Si2164B_MASTER_IEN_PROP_struct;

   /* MASTER_IEN property, CTSIEN field definition (NO TITLE)*/
   #define  Si2164B_MASTER_IEN_PROP_CTSIEN_LSB         7
   #define  Si2164B_MASTER_IEN_PROP_CTSIEN_MASK        0x01
   #define  Si2164B_MASTER_IEN_PROP_CTSIEN_DEFAULT    0
    #define Si2164B_MASTER_IEN_PROP_CTSIEN_OFF  0
    #define Si2164B_MASTER_IEN_PROP_CTSIEN_ON   1

   /* MASTER_IEN property, DDIEN field definition (NO TITLE)*/
   #define  Si2164B_MASTER_IEN_PROP_DDIEN_LSB         0
   #define  Si2164B_MASTER_IEN_PROP_DDIEN_MASK        0x01
   #define  Si2164B_MASTER_IEN_PROP_DDIEN_DEFAULT    0
    #define Si2164B_MASTER_IEN_PROP_DDIEN_OFF  0
    #define Si2164B_MASTER_IEN_PROP_DDIEN_ON   1

   /* MASTER_IEN property, ERRIEN field definition (NO TITLE)*/
   #define  Si2164B_MASTER_IEN_PROP_ERRIEN_LSB         6
   #define  Si2164B_MASTER_IEN_PROP_ERRIEN_MASK        0x01
   #define  Si2164B_MASTER_IEN_PROP_ERRIEN_DEFAULT    0
    #define Si2164B_MASTER_IEN_PROP_ERRIEN_OFF  0
    #define Si2164B_MASTER_IEN_PROP_ERRIEN_ON   1

   /* MASTER_IEN property, SCANIEN field definition (NO TITLE)*/
   #define  Si2164B_MASTER_IEN_PROP_SCANIEN_LSB         1
   #define  Si2164B_MASTER_IEN_PROP_SCANIEN_MASK        0x01
   #define  Si2164B_MASTER_IEN_PROP_SCANIEN_DEFAULT    0
    #define Si2164B_MASTER_IEN_PROP_SCANIEN_OFF  0
    #define Si2164B_MASTER_IEN_PROP_SCANIEN_ON   1

#endif /* Si2164B_MASTER_IEN_PROP */

/* Si2164B MCNS_ADC_CREST_FACTOR property definition */
#define   Si2164B_MCNS_ADC_CREST_FACTOR_PROP 0x1604

#ifdef    Si2164B_MCNS_ADC_CREST_FACTOR_PROP
  #define Si2164B_MCNS_ADC_CREST_FACTOR_PROP_CODE 0x001604


    typedef struct { /* Si2164B_MCNS_ADC_CREST_FACTOR_PROP_struct */
      unsigned char   crest_factor;
   } Si2164B_MCNS_ADC_CREST_FACTOR_PROP_struct;

   /* MCNS_ADC_CREST_FACTOR property, CREST_FACTOR field definition (NO TITLE)*/
   #define  Si2164B_MCNS_ADC_CREST_FACTOR_PROP_CREST_FACTOR_LSB         0
   #define  Si2164B_MCNS_ADC_CREST_FACTOR_PROP_CREST_FACTOR_MASK        0xff
   #define  Si2164B_MCNS_ADC_CREST_FACTOR_PROP_CREST_FACTOR_DEFAULT    112
#endif /* Si2164B_MCNS_ADC_CREST_FACTOR_PROP */

/* Si2164B MCNS_AFC_RANGE property definition */
#define   Si2164B_MCNS_AFC_RANGE_PROP 0x1603

#ifdef    Si2164B_MCNS_AFC_RANGE_PROP
  #define Si2164B_MCNS_AFC_RANGE_PROP_CODE 0x001603


    typedef struct { /* Si2164B_MCNS_AFC_RANGE_PROP_struct */
      unsigned int    range_khz;
   } Si2164B_MCNS_AFC_RANGE_PROP_struct;

   /* MCNS_AFC_RANGE property, RANGE_KHZ field definition (NO TITLE)*/
   #define  Si2164B_MCNS_AFC_RANGE_PROP_RANGE_KHZ_LSB         0
   #define  Si2164B_MCNS_AFC_RANGE_PROP_RANGE_KHZ_MASK        0xffff
   #define  Si2164B_MCNS_AFC_RANGE_PROP_RANGE_KHZ_DEFAULT    100
#endif /* Si2164B_MCNS_AFC_RANGE_PROP */

/* Si2164B MCNS_CONSTELLATION property definition */
#define   Si2164B_MCNS_CONSTELLATION_PROP 0x1601

#ifdef    Si2164B_MCNS_CONSTELLATION_PROP
  #define Si2164B_MCNS_CONSTELLATION_PROP_CODE 0x001601


    typedef struct { /* Si2164B_MCNS_CONSTELLATION_PROP_struct */
      unsigned char   constellation;
   } Si2164B_MCNS_CONSTELLATION_PROP_struct;

   /* MCNS_CONSTELLATION property, CONSTELLATION field definition (NO TITLE)*/
   #define  Si2164B_MCNS_CONSTELLATION_PROP_CONSTELLATION_LSB         0
   #define  Si2164B_MCNS_CONSTELLATION_PROP_CONSTELLATION_MASK        0x3f
   #define  Si2164B_MCNS_CONSTELLATION_PROP_CONSTELLATION_DEFAULT    0
    #define Si2164B_MCNS_CONSTELLATION_PROP_CONSTELLATION_AUTO    0
    #define Si2164B_MCNS_CONSTELLATION_PROP_CONSTELLATION_QAM64   9
    #define Si2164B_MCNS_CONSTELLATION_PROP_CONSTELLATION_QAM256  11

#endif /* Si2164B_MCNS_CONSTELLATION_PROP */

/* Si2164B MCNS_SYMBOL_RATE property definition */
#define   Si2164B_MCNS_SYMBOL_RATE_PROP 0x1602

#ifdef    Si2164B_MCNS_SYMBOL_RATE_PROP
  #define Si2164B_MCNS_SYMBOL_RATE_PROP_CODE 0x001602


    typedef struct { /* Si2164B_MCNS_SYMBOL_RATE_PROP_struct */
      unsigned int    rate;
   } Si2164B_MCNS_SYMBOL_RATE_PROP_struct;

   /* MCNS_SYMBOL_RATE property, RATE field definition (NO TITLE)*/
   #define  Si2164B_MCNS_SYMBOL_RATE_PROP_RATE_LSB         0
   #define  Si2164B_MCNS_SYMBOL_RATE_PROP_RATE_MASK        0xffff
   #define  Si2164B_MCNS_SYMBOL_RATE_PROP_RATE_DEFAULT    5361
#endif /* Si2164B_MCNS_SYMBOL_RATE_PROP */


/* Si2164B SCAN_FMAX property definition */
#define   Si2164B_SCAN_FMAX_PROP 0x0304

#ifdef    Si2164B_SCAN_FMAX_PROP
  #define Si2164B_SCAN_FMAX_PROP_CODE 0x000304


    typedef struct { /* Si2164B_SCAN_FMAX_PROP_struct */
      unsigned int    scan_fmax;
   } Si2164B_SCAN_FMAX_PROP_struct;

   /* SCAN_FMAX property, SCAN_FMAX field definition (NO TITLE)*/
   #define  Si2164B_SCAN_FMAX_PROP_SCAN_FMAX_LSB         0
   #define  Si2164B_SCAN_FMAX_PROP_SCAN_FMAX_MASK        0xffff
   #define  Si2164B_SCAN_FMAX_PROP_SCAN_FMAX_DEFAULT    0
#endif /* Si2164B_SCAN_FMAX_PROP */

/* Si2164B SCAN_FMIN property definition */
#define   Si2164B_SCAN_FMIN_PROP 0x0303

#ifdef    Si2164B_SCAN_FMIN_PROP
  #define Si2164B_SCAN_FMIN_PROP_CODE 0x000303


    typedef struct { /* Si2164B_SCAN_FMIN_PROP_struct */
      unsigned int    scan_fmin;
   } Si2164B_SCAN_FMIN_PROP_struct;

   /* SCAN_FMIN property, SCAN_FMIN field definition (NO TITLE)*/
   #define  Si2164B_SCAN_FMIN_PROP_SCAN_FMIN_LSB         0
   #define  Si2164B_SCAN_FMIN_PROP_SCAN_FMIN_MASK        0xffff
   #define  Si2164B_SCAN_FMIN_PROP_SCAN_FMIN_DEFAULT    0
#endif /* Si2164B_SCAN_FMIN_PROP */

/* Si2164B SCAN_IEN property definition */
#define   Si2164B_SCAN_IEN_PROP 0x0308

#ifdef    Si2164B_SCAN_IEN_PROP
  #define Si2164B_SCAN_IEN_PROP_CODE 0x000308


    typedef struct { /* Si2164B_SCAN_IEN_PROP_struct */
      unsigned char   buzien;
      unsigned char   reqien;
   } Si2164B_SCAN_IEN_PROP_struct;

   /* SCAN_IEN property, BUZIEN field definition (NO TITLE)*/
   #define  Si2164B_SCAN_IEN_PROP_BUZIEN_LSB         0
   #define  Si2164B_SCAN_IEN_PROP_BUZIEN_MASK        0x01
   #define  Si2164B_SCAN_IEN_PROP_BUZIEN_DEFAULT    0
    #define Si2164B_SCAN_IEN_PROP_BUZIEN_DISABLE  0
    #define Si2164B_SCAN_IEN_PROP_BUZIEN_ENABLE   1

   /* SCAN_IEN property, REQIEN field definition (NO TITLE)*/
   #define  Si2164B_SCAN_IEN_PROP_REQIEN_LSB         1
   #define  Si2164B_SCAN_IEN_PROP_REQIEN_MASK        0x01
   #define  Si2164B_SCAN_IEN_PROP_REQIEN_DEFAULT    0
    #define Si2164B_SCAN_IEN_PROP_REQIEN_DISABLE  0
    #define Si2164B_SCAN_IEN_PROP_REQIEN_ENABLE   1

#endif /* Si2164B_SCAN_IEN_PROP */

/* Si2164B SCAN_INT_SENSE property definition */
#define   Si2164B_SCAN_INT_SENSE_PROP 0x0307

#ifdef    Si2164B_SCAN_INT_SENSE_PROP
  #define Si2164B_SCAN_INT_SENSE_PROP_CODE 0x000307


    typedef struct { /* Si2164B_SCAN_INT_SENSE_PROP_struct */
      unsigned char   buznegen;
      unsigned char   buzposen;
      unsigned char   reqnegen;
      unsigned char   reqposen;
   } Si2164B_SCAN_INT_SENSE_PROP_struct;

   /* SCAN_INT_SENSE property, BUZNEGEN field definition (NO TITLE)*/
   #define  Si2164B_SCAN_INT_SENSE_PROP_BUZNEGEN_LSB         0
   #define  Si2164B_SCAN_INT_SENSE_PROP_BUZNEGEN_MASK        0x01
   #define  Si2164B_SCAN_INT_SENSE_PROP_BUZNEGEN_DEFAULT    1
    #define Si2164B_SCAN_INT_SENSE_PROP_BUZNEGEN_DISABLE  0
    #define Si2164B_SCAN_INT_SENSE_PROP_BUZNEGEN_ENABLE   1

   /* SCAN_INT_SENSE property, BUZPOSEN field definition (NO TITLE)*/
   #define  Si2164B_SCAN_INT_SENSE_PROP_BUZPOSEN_LSB         8
   #define  Si2164B_SCAN_INT_SENSE_PROP_BUZPOSEN_MASK        0x01
   #define  Si2164B_SCAN_INT_SENSE_PROP_BUZPOSEN_DEFAULT    0
    #define Si2164B_SCAN_INT_SENSE_PROP_BUZPOSEN_DISABLE  0
    #define Si2164B_SCAN_INT_SENSE_PROP_BUZPOSEN_ENABLE   1

   /* SCAN_INT_SENSE property, REQNEGEN field definition (NO TITLE)*/
   #define  Si2164B_SCAN_INT_SENSE_PROP_REQNEGEN_LSB         1
   #define  Si2164B_SCAN_INT_SENSE_PROP_REQNEGEN_MASK        0x01
   #define  Si2164B_SCAN_INT_SENSE_PROP_REQNEGEN_DEFAULT    0
    #define Si2164B_SCAN_INT_SENSE_PROP_REQNEGEN_DISABLE  0
    #define Si2164B_SCAN_INT_SENSE_PROP_REQNEGEN_ENABLE   1

   /* SCAN_INT_SENSE property, REQPOSEN field definition (NO TITLE)*/
   #define  Si2164B_SCAN_INT_SENSE_PROP_REQPOSEN_LSB         9
   #define  Si2164B_SCAN_INT_SENSE_PROP_REQPOSEN_MASK        0x01
   #define  Si2164B_SCAN_INT_SENSE_PROP_REQPOSEN_DEFAULT    1
    #define Si2164B_SCAN_INT_SENSE_PROP_REQPOSEN_DISABLE  0
    #define Si2164B_SCAN_INT_SENSE_PROP_REQPOSEN_ENABLE   1

#endif /* Si2164B_SCAN_INT_SENSE_PROP */

/* Si2164B SCAN_SAT_CONFIG property definition */
#define   Si2164B_SCAN_SAT_CONFIG_PROP 0x0302

#ifdef    Si2164B_SCAN_SAT_CONFIG_PROP
  #define Si2164B_SCAN_SAT_CONFIG_PROP_CODE 0x000302


    typedef struct { /* Si2164B_SCAN_SAT_CONFIG_PROP_struct */
      unsigned char   analog_detect;
      unsigned char   reserved1;
      unsigned char   reserved2;
      unsigned char   scan_debug;
   } Si2164B_SCAN_SAT_CONFIG_PROP_struct;

   /* SCAN_SAT_CONFIG property, ANALOG_DETECT field definition (NO TITLE)*/
   #define  Si2164B_SCAN_SAT_CONFIG_PROP_ANALOG_DETECT_LSB         0
   #define  Si2164B_SCAN_SAT_CONFIG_PROP_ANALOG_DETECT_MASK        0x01
   #define  Si2164B_SCAN_SAT_CONFIG_PROP_ANALOG_DETECT_DEFAULT    0
    #define Si2164B_SCAN_SAT_CONFIG_PROP_ANALOG_DETECT_DISABLED  0
    #define Si2164B_SCAN_SAT_CONFIG_PROP_ANALOG_DETECT_ENABLED   1

   /* SCAN_SAT_CONFIG property, RESERVED1 field definition (NO TITLE)*/
   #define  Si2164B_SCAN_SAT_CONFIG_PROP_RESERVED1_LSB         1
   #define  Si2164B_SCAN_SAT_CONFIG_PROP_RESERVED1_MASK        0x1f
   #define  Si2164B_SCAN_SAT_CONFIG_PROP_RESERVED1_DEFAULT    0
   /* SCAN_SAT_CONFIG property, RESERVED2 field definition (NO TITLE)*/
   #define  Si2164B_SCAN_SAT_CONFIG_PROP_RESERVED2_LSB         6
   #define  Si2164B_SCAN_SAT_CONFIG_PROP_RESERVED2_MASK        0x7f
   #define  Si2164B_SCAN_SAT_CONFIG_PROP_RESERVED2_DEFAULT    12
   /* SCAN_SAT_CONFIG property, SCAN_DEBUG field definition (NO TITLE)*/
   #define  Si2164B_SCAN_SAT_CONFIG_PROP_SCAN_DEBUG_LSB         13
   #define  Si2164B_SCAN_SAT_CONFIG_PROP_SCAN_DEBUG_MASK        0x07
   #define  Si2164B_SCAN_SAT_CONFIG_PROP_SCAN_DEBUG_DEFAULT    0
#endif /* Si2164B_SCAN_SAT_CONFIG_PROP */

/* Si2164B SCAN_SAT_UNICABLE_BW property definition */
#define   Si2164B_SCAN_SAT_UNICABLE_BW_PROP 0x0309

#ifdef    Si2164B_SCAN_SAT_UNICABLE_BW_PROP
  #define Si2164B_SCAN_SAT_UNICABLE_BW_PROP_CODE 0x000309


    typedef struct { /* Si2164B_SCAN_SAT_UNICABLE_BW_PROP_struct */
      unsigned int    scan_sat_unicable_bw;
   } Si2164B_SCAN_SAT_UNICABLE_BW_PROP_struct;

   /* SCAN_SAT_UNICABLE_BW property, SCAN_SAT_UNICABLE_BW field definition (NO TITLE)*/
   #define  Si2164B_SCAN_SAT_UNICABLE_BW_PROP_SCAN_SAT_UNICABLE_BW_LSB         0
   #define  Si2164B_SCAN_SAT_UNICABLE_BW_PROP_SCAN_SAT_UNICABLE_BW_MASK        0x3ff
   #define  Si2164B_SCAN_SAT_UNICABLE_BW_PROP_SCAN_SAT_UNICABLE_BW_DEFAULT    0
#endif /* Si2164B_SCAN_SAT_UNICABLE_BW_PROP */

/* Si2164B SCAN_SAT_UNICABLE_MIN_TUNE_STEP property definition */
#define   Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP 0x030a

#ifdef    Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP
  #define Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP_CODE 0x00030a


    typedef struct { /* Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP_struct */
      unsigned char   scan_sat_unicable_min_tune_step;
   } Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP_struct;

   /* SCAN_SAT_UNICABLE_MIN_TUNE_STEP property, SCAN_SAT_UNICABLE_MIN_TUNE_STEP field definition (NO TITLE)*/
   #define  Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_LSB         0
   #define  Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_MASK        0xff
   #define  Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_DEFAULT    50
#endif /* Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP */


/* Si2164B SCAN_SYMB_RATE_MAX property definition */
#define   Si2164B_SCAN_SYMB_RATE_MAX_PROP 0x0306

#ifdef    Si2164B_SCAN_SYMB_RATE_MAX_PROP
  #define Si2164B_SCAN_SYMB_RATE_MAX_PROP_CODE 0x000306


    typedef struct { /* Si2164B_SCAN_SYMB_RATE_MAX_PROP_struct */
      unsigned int    scan_symb_rate_max;
   } Si2164B_SCAN_SYMB_RATE_MAX_PROP_struct;

   /* SCAN_SYMB_RATE_MAX property, SCAN_SYMB_RATE_MAX field definition (NO TITLE)*/
   #define  Si2164B_SCAN_SYMB_RATE_MAX_PROP_SCAN_SYMB_RATE_MAX_LSB         0
   #define  Si2164B_SCAN_SYMB_RATE_MAX_PROP_SCAN_SYMB_RATE_MAX_MASK        0xffff
   #define  Si2164B_SCAN_SYMB_RATE_MAX_PROP_SCAN_SYMB_RATE_MAX_DEFAULT    0
#endif /* Si2164B_SCAN_SYMB_RATE_MAX_PROP */

/* Si2164B SCAN_SYMB_RATE_MIN property definition */
#define   Si2164B_SCAN_SYMB_RATE_MIN_PROP 0x0305

#ifdef    Si2164B_SCAN_SYMB_RATE_MIN_PROP
  #define Si2164B_SCAN_SYMB_RATE_MIN_PROP_CODE 0x000305


    typedef struct { /* Si2164B_SCAN_SYMB_RATE_MIN_PROP_struct */
      unsigned int    scan_symb_rate_min;
   } Si2164B_SCAN_SYMB_RATE_MIN_PROP_struct;

   /* SCAN_SYMB_RATE_MIN property, SCAN_SYMB_RATE_MIN field definition (NO TITLE)*/
   #define  Si2164B_SCAN_SYMB_RATE_MIN_PROP_SCAN_SYMB_RATE_MIN_LSB         0
   #define  Si2164B_SCAN_SYMB_RATE_MIN_PROP_SCAN_SYMB_RATE_MIN_MASK        0xffff
   #define  Si2164B_SCAN_SYMB_RATE_MIN_PROP_SCAN_SYMB_RATE_MIN_DEFAULT    0
#endif /* Si2164B_SCAN_SYMB_RATE_MIN_PROP */

/* Si2164B SCAN_TER_CONFIG property definition */
#define   Si2164B_SCAN_TER_CONFIG_PROP 0x0301

#ifdef    Si2164B_SCAN_TER_CONFIG_PROP
  #define Si2164B_SCAN_TER_CONFIG_PROP_CODE 0x000301


    typedef struct { /* Si2164B_SCAN_TER_CONFIG_PROP_struct */
      unsigned char   analog_bw;
      unsigned char   mode;
      unsigned char   scan_debug;
      unsigned char   search_analog;
   } Si2164B_SCAN_TER_CONFIG_PROP_struct;

   /* SCAN_TER_CONFIG property, ANALOG_BW field definition (NO TITLE)*/
   #define  Si2164B_SCAN_TER_CONFIG_PROP_ANALOG_BW_LSB         2
   #define  Si2164B_SCAN_TER_CONFIG_PROP_ANALOG_BW_MASK        0x03
   #define  Si2164B_SCAN_TER_CONFIG_PROP_ANALOG_BW_DEFAULT    3
    #define Si2164B_SCAN_TER_CONFIG_PROP_ANALOG_BW_6MHZ  1
    #define Si2164B_SCAN_TER_CONFIG_PROP_ANALOG_BW_7MHZ  2
    #define Si2164B_SCAN_TER_CONFIG_PROP_ANALOG_BW_8MHZ  3

   /* SCAN_TER_CONFIG property, MODE field definition (NO TITLE)*/
   #define  Si2164B_SCAN_TER_CONFIG_PROP_MODE_LSB         0
   #define  Si2164B_SCAN_TER_CONFIG_PROP_MODE_MASK        0x03
   #define  Si2164B_SCAN_TER_CONFIG_PROP_MODE_DEFAULT    0
    #define Si2164B_SCAN_TER_CONFIG_PROP_MODE_BLIND_SCAN    0
    #define Si2164B_SCAN_TER_CONFIG_PROP_MODE_MAPPING_SCAN  1
    #define Si2164B_SCAN_TER_CONFIG_PROP_MODE_BLIND_LOCK    2

   /* SCAN_TER_CONFIG property, SCAN_DEBUG field definition (NO TITLE)*/
   #define  Si2164B_SCAN_TER_CONFIG_PROP_SCAN_DEBUG_LSB         12
   #define  Si2164B_SCAN_TER_CONFIG_PROP_SCAN_DEBUG_MASK        0x0f
   #define  Si2164B_SCAN_TER_CONFIG_PROP_SCAN_DEBUG_DEFAULT    0
   /* SCAN_TER_CONFIG property, SEARCH_ANALOG field definition (NO TITLE)*/
   #define  Si2164B_SCAN_TER_CONFIG_PROP_SEARCH_ANALOG_LSB         4
   #define  Si2164B_SCAN_TER_CONFIG_PROP_SEARCH_ANALOG_MASK        0x01
   #define  Si2164B_SCAN_TER_CONFIG_PROP_SEARCH_ANALOG_DEFAULT    0
    #define Si2164B_SCAN_TER_CONFIG_PROP_SEARCH_ANALOG_DISABLE  0
    #define Si2164B_SCAN_TER_CONFIG_PROP_SEARCH_ANALOG_ENABLE   1

#endif /* Si2164B_SCAN_TER_CONFIG_PROP */




  /* --------------------------------------------*/
  /* PROPERTIES STRUCT                           */
  /* This stores all property fields             */
  /* --------------------------------------------*/
  typedef struct {
    #ifdef    Si2164B_DD_BER_RESOL_PROP
              Si2164B_DD_BER_RESOL_PROP_struct                     dd_ber_resol;
    #endif /* Si2164B_DD_BER_RESOL_PROP */
    #ifdef    Si2164B_DD_CBER_RESOL_PROP
              Si2164B_DD_CBER_RESOL_PROP_struct                    dd_cber_resol;
    #endif /* Si2164B_DD_CBER_RESOL_PROP */
    #ifdef    Si2164B_DD_DISEQC_FREQ_PROP
              Si2164B_DD_DISEQC_FREQ_PROP_struct                   dd_diseqc_freq;
    #endif /* Si2164B_DD_DISEQC_FREQ_PROP */
    #ifdef    Si2164B_DD_DISEQC_PARAM_PROP
              Si2164B_DD_DISEQC_PARAM_PROP_struct                  dd_diseqc_param;
    #endif /* Si2164B_DD_DISEQC_PARAM_PROP */

    #ifdef    Si2164B_DD_FER_RESOL_PROP
              Si2164B_DD_FER_RESOL_PROP_struct                     dd_fer_resol;
    #endif /* Si2164B_DD_FER_RESOL_PROP */
    #ifdef    Si2164B_DD_IEN_PROP
              Si2164B_DD_IEN_PROP_struct                           dd_ien;
    #endif /* Si2164B_DD_IEN_PROP */
    #ifdef    Si2164B_DD_IF_INPUT_FREQ_PROP
              Si2164B_DD_IF_INPUT_FREQ_PROP_struct                 dd_if_input_freq;
    #endif /* Si2164B_DD_IF_INPUT_FREQ_PROP */
    #ifdef    Si2164B_DD_INT_SENSE_PROP
              Si2164B_DD_INT_SENSE_PROP_struct                     dd_int_sense;
    #endif /* Si2164B_DD_INT_SENSE_PROP */
    #ifdef    Si2164B_DD_MODE_PROP
              Si2164B_DD_MODE_PROP_struct                          dd_mode;
    #endif /* Si2164B_DD_MODE_PROP */
    #ifdef    Si2164B_DD_PER_RESOL_PROP
              Si2164B_DD_PER_RESOL_PROP_struct                     dd_per_resol;
    #endif /* Si2164B_DD_PER_RESOL_PROP */
    #ifdef    Si2164B_DD_RSQ_BER_THRESHOLD_PROP
              Si2164B_DD_RSQ_BER_THRESHOLD_PROP_struct             dd_rsq_ber_threshold;
    #endif /* Si2164B_DD_RSQ_BER_THRESHOLD_PROP */
    #ifdef    Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP
              Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP_struct            dd_sec_ts_serial_diff;
    #endif /* Si2164B_DD_SEC_TS_SERIAL_DIFF_PROP */
    #ifdef    Si2164B_DD_SEC_TS_SETUP_PAR_PROP
              Si2164B_DD_SEC_TS_SETUP_PAR_PROP_struct              dd_sec_ts_setup_par;
    #endif /* Si2164B_DD_SEC_TS_SETUP_PAR_PROP */
    #ifdef    Si2164B_DD_SEC_TS_SETUP_SER_PROP
              Si2164B_DD_SEC_TS_SETUP_SER_PROP_struct              dd_sec_ts_setup_ser;
    #endif /* Si2164B_DD_SEC_TS_SETUP_SER_PROP */
    #ifdef    Si2164B_DD_SEC_TS_SLR_SERIAL_PROP
              Si2164B_DD_SEC_TS_SLR_SERIAL_PROP_struct             dd_sec_ts_slr_serial;
    #endif /* Si2164B_DD_SEC_TS_SLR_SERIAL_PROP */
    #ifdef    Si2164B_DD_SSI_SQI_PARAM_PROP
              Si2164B_DD_SSI_SQI_PARAM_PROP_struct                 dd_ssi_sqi_param;
    #endif /* Si2164B_DD_SSI_SQI_PARAM_PROP */
    #ifdef    Si2164B_DD_TS_FREQ_PROP
              Si2164B_DD_TS_FREQ_PROP_struct                       dd_ts_freq;
    #endif /* Si2164B_DD_TS_FREQ_PROP */
    #ifdef    Si2164B_DD_TS_FREQ_MAX_PROP
              Si2164B_DD_TS_FREQ_MAX_PROP_struct                   dd_ts_freq_max;
    #endif /* Si2164B_DD_TS_FREQ_MAX_PROP */
    #ifdef    Si2164B_DD_TS_MODE_PROP
              Si2164B_DD_TS_MODE_PROP_struct                       dd_ts_mode;
    #endif /* Si2164B_DD_TS_MODE_PROP */
    #ifdef    Si2164B_DD_TS_SERIAL_DIFF_PROP
              Si2164B_DD_TS_SERIAL_DIFF_PROP_struct                dd_ts_serial_diff;
    #endif /* Si2164B_DD_TS_SERIAL_DIFF_PROP */
    #ifdef    Si2164B_DD_TS_SETUP_PAR_PROP
              Si2164B_DD_TS_SETUP_PAR_PROP_struct                  dd_ts_setup_par;
    #endif /* Si2164B_DD_TS_SETUP_PAR_PROP */
    #ifdef    Si2164B_DD_TS_SETUP_SER_PROP
              Si2164B_DD_TS_SETUP_SER_PROP_struct                  dd_ts_setup_ser;
    #endif /* Si2164B_DD_TS_SETUP_SER_PROP */
    #ifdef    Si2164B_DD_TS_SLR_SERIAL_PROP
              Si2164B_DD_TS_SLR_SERIAL_PROP_struct                 dd_ts_slr_serial;
    #endif /* Si2164B_DD_TS_SLR_SERIAL_PROP */
    #ifdef    Si2164B_DVBC2_ADC_CREST_FACTOR_PROP
              Si2164B_DVBC2_ADC_CREST_FACTOR_PROP_struct           dvbc2_adc_crest_factor;
    #endif /* Si2164B_DVBC2_ADC_CREST_FACTOR_PROP */
    #ifdef    Si2164B_DVBC2_AFC_RANGE_PROP
              Si2164B_DVBC2_AFC_RANGE_PROP_struct                  dvbc2_afc_range;
    #endif /* Si2164B_DVBC2_AFC_RANGE_PROP */

    #ifdef    Si2164B_DVBC_ADC_CREST_FACTOR_PROP
              Si2164B_DVBC_ADC_CREST_FACTOR_PROP_struct            dvbc_adc_crest_factor;
    #endif /* Si2164B_DVBC_ADC_CREST_FACTOR_PROP */
    #ifdef    Si2164B_DVBC_AFC_RANGE_PROP
              Si2164B_DVBC_AFC_RANGE_PROP_struct                   dvbc_afc_range;
    #endif /* Si2164B_DVBC_AFC_RANGE_PROP */
    #ifdef    Si2164B_DVBC_CONSTELLATION_PROP
              Si2164B_DVBC_CONSTELLATION_PROP_struct               dvbc_constellation;
    #endif /* Si2164B_DVBC_CONSTELLATION_PROP */
    #ifdef    Si2164B_DVBC_SYMBOL_RATE_PROP
              Si2164B_DVBC_SYMBOL_RATE_PROP_struct                 dvbc_symbol_rate;
    #endif /* Si2164B_DVBC_SYMBOL_RATE_PROP */

    #ifdef    Si2164B_DVBS2_ADC_CREST_FACTOR_PROP
              Si2164B_DVBS2_ADC_CREST_FACTOR_PROP_struct           dvbs2_adc_crest_factor;
    #endif /* Si2164B_DVBS2_ADC_CREST_FACTOR_PROP */
    #ifdef    Si2164B_DVBS2_AFC_RANGE_PROP
              Si2164B_DVBS2_AFC_RANGE_PROP_struct                  dvbs2_afc_range;
    #endif /* Si2164B_DVBS2_AFC_RANGE_PROP */
    #ifdef    Si2164B_DVBS2_SYMBOL_RATE_PROP
              Si2164B_DVBS2_SYMBOL_RATE_PROP_struct                dvbs2_symbol_rate;
    #endif /* Si2164B_DVBS2_SYMBOL_RATE_PROP */
    #ifdef    Si2164B_DVBS_ADC_CREST_FACTOR_PROP
              Si2164B_DVBS_ADC_CREST_FACTOR_PROP_struct            dvbs_adc_crest_factor;
    #endif /* Si2164B_DVBS_ADC_CREST_FACTOR_PROP */
    #ifdef    Si2164B_DVBS_AFC_RANGE_PROP
              Si2164B_DVBS_AFC_RANGE_PROP_struct                   dvbs_afc_range;
    #endif /* Si2164B_DVBS_AFC_RANGE_PROP */
    #ifdef    Si2164B_DVBS_SYMBOL_RATE_PROP
              Si2164B_DVBS_SYMBOL_RATE_PROP_struct                 dvbs_symbol_rate;
    #endif /* Si2164B_DVBS_SYMBOL_RATE_PROP */

    #ifdef    Si2164B_DVBT2_ADC_CREST_FACTOR_PROP
              Si2164B_DVBT2_ADC_CREST_FACTOR_PROP_struct           dvbt2_adc_crest_factor;
    #endif /* Si2164B_DVBT2_ADC_CREST_FACTOR_PROP */
    #ifdef    Si2164B_DVBT2_AFC_RANGE_PROP
              Si2164B_DVBT2_AFC_RANGE_PROP_struct                  dvbt2_afc_range;
    #endif /* Si2164B_DVBT2_AFC_RANGE_PROP */
    #ifdef    Si2164B_DVBT2_FEF_TUNER_PROP
              Si2164B_DVBT2_FEF_TUNER_PROP_struct                  dvbt2_fef_tuner;
    #endif /* Si2164B_DVBT2_FEF_TUNER_PROP */
    #ifdef    Si2164B_DVBT2_MODE_PROP
              Si2164B_DVBT2_MODE_PROP_struct                       dvbt2_mode;
    #endif /* Si2164B_DVBT2_MODE_PROP */

    #ifdef    Si2164B_DVBT_ADC_CREST_FACTOR_PROP
              Si2164B_DVBT_ADC_CREST_FACTOR_PROP_struct            dvbt_adc_crest_factor;
    #endif /* Si2164B_DVBT_ADC_CREST_FACTOR_PROP */
    #ifdef    Si2164B_DVBT_AFC_RANGE_PROP
              Si2164B_DVBT_AFC_RANGE_PROP_struct                   dvbt_afc_range;
    #endif /* Si2164B_DVBT_AFC_RANGE_PROP */
    #ifdef    Si2164B_DVBT_HIERARCHY_PROP
              Si2164B_DVBT_HIERARCHY_PROP_struct                   dvbt_hierarchy;
    #endif /* Si2164B_DVBT_HIERARCHY_PROP */


    #ifdef    Si2164B_MASTER_IEN_PROP
              Si2164B_MASTER_IEN_PROP_struct                       master_ien;
    #endif /* Si2164B_MASTER_IEN_PROP */
    #ifdef    Si2164B_MCNS_ADC_CREST_FACTOR_PROP
              Si2164B_MCNS_ADC_CREST_FACTOR_PROP_struct            mcns_adc_crest_factor;
    #endif /* Si2164B_MCNS_ADC_CREST_FACTOR_PROP */
    #ifdef    Si2164B_MCNS_AFC_RANGE_PROP
              Si2164B_MCNS_AFC_RANGE_PROP_struct                   mcns_afc_range;
    #endif /* Si2164B_MCNS_AFC_RANGE_PROP */
    #ifdef    Si2164B_MCNS_CONSTELLATION_PROP
              Si2164B_MCNS_CONSTELLATION_PROP_struct               mcns_constellation;
    #endif /* Si2164B_MCNS_CONSTELLATION_PROP */
    #ifdef    Si2164B_MCNS_SYMBOL_RATE_PROP
              Si2164B_MCNS_SYMBOL_RATE_PROP_struct                 mcns_symbol_rate;
    #endif /* Si2164B_MCNS_SYMBOL_RATE_PROP */

    #ifdef    Si2164B_SCAN_FMAX_PROP
              Si2164B_SCAN_FMAX_PROP_struct                        scan_fmax;
    #endif /* Si2164B_SCAN_FMAX_PROP */
    #ifdef    Si2164B_SCAN_FMIN_PROP
              Si2164B_SCAN_FMIN_PROP_struct                        scan_fmin;
    #endif /* Si2164B_SCAN_FMIN_PROP */
    #ifdef    Si2164B_SCAN_IEN_PROP
              Si2164B_SCAN_IEN_PROP_struct                         scan_ien;
    #endif /* Si2164B_SCAN_IEN_PROP */
    #ifdef    Si2164B_SCAN_INT_SENSE_PROP
              Si2164B_SCAN_INT_SENSE_PROP_struct                   scan_int_sense;
    #endif /* Si2164B_SCAN_INT_SENSE_PROP */
    #ifdef    Si2164B_SCAN_SAT_CONFIG_PROP
              Si2164B_SCAN_SAT_CONFIG_PROP_struct                  scan_sat_config;
    #endif /* Si2164B_SCAN_SAT_CONFIG_PROP */
    #ifdef    Si2164B_SCAN_SAT_UNICABLE_BW_PROP
              Si2164B_SCAN_SAT_UNICABLE_BW_PROP_struct             scan_sat_unicable_bw;
    #endif /* Si2164B_SCAN_SAT_UNICABLE_BW_PROP */
    #ifdef    Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP
              Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP_struct  scan_sat_unicable_min_tune_step;
    #endif /* Si2164B_SCAN_SAT_UNICABLE_MIN_TUNE_STEP_PROP */

    #ifdef    Si2164B_SCAN_SYMB_RATE_MAX_PROP
              Si2164B_SCAN_SYMB_RATE_MAX_PROP_struct               scan_symb_rate_max;
    #endif /* Si2164B_SCAN_SYMB_RATE_MAX_PROP */
    #ifdef    Si2164B_SCAN_SYMB_RATE_MIN_PROP
              Si2164B_SCAN_SYMB_RATE_MIN_PROP_struct               scan_symb_rate_min;
    #endif /* Si2164B_SCAN_SYMB_RATE_MIN_PROP */
    #ifdef    Si2164B_SCAN_TER_CONFIG_PROP
              Si2164B_SCAN_TER_CONFIG_PROP_struct                  scan_ter_config;
    #endif /* Si2164B_SCAN_TER_CONFIG_PROP */

  } Si2164B_PropObj;

#define   Si2164B_GET_PROPERTY_STRING

#endif /* _Si2164B_PROPERTIES_H_ */







