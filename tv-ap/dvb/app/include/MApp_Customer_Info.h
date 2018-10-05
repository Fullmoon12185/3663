////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (!��MStar Confidential Information!�L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////


/* INPUT_SW_PROJECT
Chakra      -   01
POLLUX      -   02
ARCHIMEDES  -   03
Chakra2     !V   04
OBAMA       !V   05
*/

/* INPUT_PRODUCT_TYPE
TV-01
STB-02
*/

/* INPUT_TV_SYSTEM
ATSC-01
DVBT-02
DVBC-03
DVBS-04
DMBT-05
ATV Only-06
*/

//#define OLD_DEMO

#ifdef OLD_DEMO
//**************************************************************************
//********************** Customer Info Input Area **************************
//**************************************************************************
//Customer ID (Example:0009 = 0x0009) (Example: 20= 0x0014)
#define INPUT_CUSTOMER_ID_LOW_BYTE  0x66
#define INPUT_CUSTOMER_ID_HIGH_BYTE 0x66

//Model ID (Example:0001 = 0x0001)
#define INPUT_MODEL_ID_LOW_BYTE     0x01
#define INPUT_MODEL_ID_HIGH_BYTE    0x00

//Chip ID (Example:000B = 0x000B)
#define INPUT_CHIP_ID_LOW_BYTE      0x66
#define INPUT_CHIP_ID_HIGH_BYTE     0x66

#define INPUT_SW_PROJECT            0x04
#define INPUT_SW_GENERATION         0x01
#define INPUT_PRODUCT_TYPE          0x01
#define INPUT_TV_SYSTEM             0x02

//Label (Example:00000456 = 0x0001C8)
#define INPUT_LABEL_LOW_BYTE        0x08
#define INPUT_LABEL_MIDDLE_BYTE     0x00
#define INPUT_LABEL_HIGH_BYTE       0x00


#else
//31 00 61 98 C8 00 9B 01 01 02 0B 35 01 10 4C 02 01 01 30 30 30 30 30 30 30 30
//**************************************************************************
//********************** Customer Info Input Area **************************
//**************************************************************************
//Customer ID (Example:0009 = 0x0009) (Example: 20= 0x0014)
#define INPUT_CUSTOMER_ID_LOW_BYTE  0x31//0x86//0x66
#define INPUT_CUSTOMER_ID_HIGH_BYTE 0x00//0x66

//Model ID (Example:0001 = 0x0001)
#define INPUT_MODEL_ID_LOW_BYTE     0x61//0x68//0x01
#define INPUT_MODEL_ID_HIGH_BYTE    0x98//0x64//0x00

//Chip ID (Example:000B = 0x000B)
#define INPUT_CHIP_ID_LOW_BYTE      0xC8//0x5D//0x66
#define INPUT_CHIP_ID_HIGH_BYTE     0x00//0x66

#define INPUT_SW_PROJECT            0x9B//0x04
#define INPUT_SW_GENERATION         0x01
#define INPUT_PRODUCT_TYPE          0x01
#define INPUT_TV_SYSTEM             0x02

//Label (Example:00000456 = 0x0001C8)
#define INPUT_LABEL_LOW_BYTE        0x0B//0x08
#define INPUT_LABEL_MIDDLE_BYTE     0x35//0x00
#define INPUT_LABEL_HIGH_BYTE       0x01//0x00	

#endif

//CL(Change-List) (Example:00101234 = 0x18B72)
#define INPUT_CL_LOW_BYTE           0x10
#define INPUT_CL_MIDDLE_BYTE        0x4C
#define INPUT_CL_HIGH_BYTE          0x02

#define DRM_MODEL_ID  0x3130


//  SW Quality
//  01-BOOTLEG
//  02-Demo
//  03-Pre-Alpha
//  04-Alpha
//  05-Beta
//  06-RC
//  07-RTM
//  Quality flag can be modified by release engineer only

#define INPUT_RELEASE_QUALITY       0x01

//CPU TYPE
//01-MIPS
//02-AEON
#if (defined(MIPS_CHAKRA) || defined(ARM_CHAKRA))
#define  INPUT_CPU_TYPE 0x01
#else
#define  INPUT_CPU_TYPE 0x02
#endif
#ifdef OLD_DEMO
//Customer IP
unsigned char code IP_Cntrol_Mapping_1[8] = "00000000"; //Customer IP Control-1
unsigned char code IP_Cntrol_Mapping_2[8] = "00000000"; //Customer IP Control-2
unsigned char code IP_Cntrol_Mapping_3[8] = "00000000"; //Customer IP Control-3
unsigned char code IP_Cntrol_Mapping_4[8] = "08F80000"; //Customer IP Control-4

unsigned char code Customer_hash[] = {0x1a,0x30,0x4f,0x0a,0x3a,0x42,0xd9,0x4b,0x08,0xd5,0x74,0xa2,0xfd,0xf1,0x69,0x3f};
#else
//Customer IP
unsigned char code IP_Cntrol_Mapping_1[8] = "00000000"; //Customer IP Control-1
unsigned char code IP_Cntrol_Mapping_2[8] = "08000840";//"00000840"; //Customer IP Control-2
unsigned char code IP_Cntrol_Mapping_3[8] = "20000104"; //Customer IP Control-3
unsigned char code IP_Cntrol_Mapping_4[8] = "28F80000";//"08F80000"; //Customer IP Control-4
//94 16 82 2F 39 BF EB 84 4A 3D 52 7E 2B D0 88 19
//unsigned char code Customer_hash[] = {0x94,0x16,0x82,0x2f,0x39,0xbf,0xeb,0x84,0x4a,0x3d,0x52,0x7e,0x2b,0xd0,0x88,0x19};	
//D0 68 1B 79 FC 54 29 20 D1 10 91 DE 9F 5C E7 D4
unsigned char code Customer_hash[] = {0xd0,0x68,0x1b,0x79,0xfc,0x54,0x29,0x20,0xd1,0x10,0x91,0xde,0x9f,0x5c,0xe7,0xd4};	
#endif
U16 u16DRMModelID=DRM_MODEL_ID;
//**************************************************************************
//**************************************************************************
//**************************************************************************





unsigned char code CID_Buf[32] =  {
//Fix Value: Do not Modify
        'M', 'S', 'I', 'F',         // (Do not modify)Mstar Information:MSIF
        '0', '1',                   // (Do not modifyCustomer Info Class: 01

//Customer Info area
        INPUT_CUSTOMER_ID_LOW_BYTE,
        INPUT_CUSTOMER_ID_HIGH_BYTE,

        INPUT_MODEL_ID_LOW_BYTE,
        INPUT_MODEL_ID_HIGH_BYTE,

        INPUT_CHIP_ID_LOW_BYTE,
        INPUT_CHIP_ID_HIGH_BYTE,

        INPUT_SW_PROJECT,
        INPUT_SW_GENERATION,
        INPUT_PRODUCT_TYPE,
        INPUT_TV_SYSTEM,

        INPUT_LABEL_LOW_BYTE,
        INPUT_LABEL_MIDDLE_BYTE,
        INPUT_LABEL_HIGH_BYTE,

        INPUT_CL_LOW_BYTE,
        INPUT_CL_MIDDLE_BYTE,
        INPUT_CL_HIGH_BYTE,

        INPUT_RELEASE_QUALITY,

        INPUT_CPU_TYPE,
//Reserve
        '0', '0', '0', '0', '0', '0', '0', '0'   // Reserve
        };



