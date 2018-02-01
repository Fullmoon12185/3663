////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef IR_FORMAT_H
#define IR_FORMAT_H

//*************************************************************************
// Customer IR Specification parameter define (Please modify them by IR SPEC)
//*************************************************************************
#define IR_MODE_SEL           IR_TYPE_SWDECODE_MODE//IR_TYPE_SWDECODE_SMC_MODE //IR_TYPE_FULLDECODE_MODE
#define IR_CHANNEL_USE_AS_UPDOWN    0
#define IR_VOLUME_USE_AS_LEFTRIGHT      0
// IR Header code define
#define IR_HEADER_CODE0        0xfc//0x00   // Custom 0     //0x04
#define IR_HEADER_CODE1        0x3e //0xBF    // Custom 1     //0xFB

#define IR_IS_USER_HEADER_CODE(x)     ((x) == 0xCB04)  // (((x) >> 8) == 0xFB)   // user remoter
#define IR_IS_FACT_HEADER_CODE(x)     ((x) == 0xFC00)  // (((x) >> 8) == 0xFC)  // factory remoter


#define IR_SW_COUNT_BIT_NUM         8
#define IR_SW_REPEAT_TIMEOUT        220//110ms
#define IR_REPEAT_SKIP_COUNT        2
#define IR_MAX_IR_DATA_NUM            2


// IR Timing define
#define IR_HEADER_CODE_TIME     0x9000//9000    // us
#define IR_OFF_CODE_TIME        3360//4500    // us
#define IR_OFF_CODE_RP_TIME     2500    // us
#define IR_LOGI_01H_TIME        840//560     // us
#define IR_LOGI_0_TIME          0x3c0//1120    // us
#define IR_LOGI_1_TIME          0x7b0  // 2200  // us
#define IR_TIMEOUT_CYC          140000  // us

#define IR_HEADER_CODE_TIME_UB  100
#define IR_HEADER_CODE_TIME_LB  -100
#define IR_OFF_CODE_TIME_UB  100
#define IR_OFF_CODE_TIME_LB  -100
#define IR_OFF_CODE_RP_TIME_UB  100
#define IR_OFF_CODE_RP_TIME_LB  -100
#define IR_LOGI_01H_TIME_UB  145
#define IR_LOGI_01H_TIME_LB  -145
#define IR_LOGI_0_TIME_UB  100
#define IR_LOGI_0_TIME_LB  -100
#define IR_LOGI_1_TIME_UB  100
#define IR_LOGI_1_TIME_LB  -100
// IR Format define
#define IRKEY_DUMY              0xFF
#define IRDA_KEY_MAPPING_POWER  IRKEY_POWER

#define IR_LEADER_CODE_CHECKING_OPTION 0xBF
#define ENABLE_IR_MSTAR_SOURCE_HOTKEY   0
typedef enum _IrCommandType
{
    IRKEY_TV_ANTENNA        = IRKEY_DUMY,
    IRKEY_TV_RADIO          = IRKEY_DUMY-58,//IRKEY_DUMY-1,//0x16,//same as IRKEY_PAGE_UP //YRC-60p
    IRKEY_AVC			=  0x44,// IRKEY_DUMY-1,  // IRKEY_DUMY-6, // 0x0B,	//cyrus_080527 AVC hotkey disable

    IRKEY_CHANNEL_LIST      = IRKEY_DUMY-2,
    IRKEY_CHANNEL_FAV_LIST  = 0x30,	//IRKEY_DUMY-62,	// lin 120725 m
    IRKEY_CHANNEL_RETURN    = 0x2f, //IRKEY_DUMY-59,	// lin 120725 m
    IRKEY_CHANNEL_PLUS      = 0x11,
    IRKEY_CHANNEL_MINUS     = 0x12,

    IRKEY_AUDIO             = 0x6e,	//0x19,//0x44,	// lin 120725 m
    IRKEY_VOLUME_PLUS       = 0x14,
    IRKEY_VOLUME_MINUS      = 0x15,

    IRKEY_UP                = 0x57,
    IRKEY_POWER             = 0x16,
    IRKEY_EXIT              = 0xf3,
    IRKEY_MENU              = 0x20,
    IRKEY_DOWN              = 0x58,
    IRKEY_LEFT              = 0xf5,
    IRKEY_SELECT            = 0xf7,
    IRKEY_RIGHT             = 0xf6,
//IRKEY_DTV_MENU = 0x36,
//IRKEY_POWER_OFF = 0x46,
//IRKEY_POWER_ON = 0x47,
//IRKEY_3D_COMB = 0x49,

    IRKEY_NUM_0             = 0x0a,
    IRKEY_NUM_1             = 0x01,
    IRKEY_NUM_2             = 0x02,
    IRKEY_NUM_3             = 0x03,
    IRKEY_NUM_4             = 0x04,
    IRKEY_NUM_5             = 0x05,
    IRKEY_NUM_6             = 0x06,
    IRKEY_NUM_7             = 0x07,
    IRKEY_NUM_8             = 0x08,
    IRKEY_NUM_9             = 0x09,
    IRKEY_NUM_100             = 0x12,
    IRKEY_MUTE              = 0x17,
    IRKEY_FREEZE            = 0x53,//IRKEY_DUMY-3,
#ifdef ENABLE_DMP
    IRKEY_PAGE_UP           = IRKEY_DUMY-4,
#endif
    IRKEY_PAGE_DOWN         = IRKEY_DUMY-5,
    IRKEY_CLOCK             = IRKEY_DUMY, //IRKEY_DUMY-52,	// lin 120725 m

    IRKEY_INFO              = 0x1b,	//0xa8,	// lin 120725 m
    IRKEY_RED               = 0xb1,	//0x23,//0x35,//IRKEY_DUMY-6,	// lin 120725 m
    IRKEY_GREEN             = 0xb2,	//0x2e,//0x3c,//IRKEY_DUMY-7,	// lin 120725 m
    IRKEY_YELLOW            = 0xb4,	//0x38,//0x37,//IRKEY_DUMY-8,	// lin 120725 m
    IRKEY_BLUE              = 0xb3,	//0x39,//0x27,//IRKEY_DUMY-9,	// lin 120725 m
    IRKEY_MTS               = 0x18,
    IRKEY_NINE_LATTICE      = IRKEY_DUMY-10,
    IRKEY_TTX               = IRKEY_DUMY,	//IRKEY_DUMY-11,	// lin 120725 m
    IRKEY_CC                = IRKEY_DUMY,	//IRKEY_DUMY-61,	// lin 120725 m
    IRKEY_INPUT_SOURCE      = 0x13,
    IRKEY_CRADRD            = IRKEY_DUMY-13,
    IRKEY_PICTURE           = 0xf8,//IRKEY_DUMY-14,
    IRKEY_ZOOM              = IRKEY_DUMY,	//0x30,	// lin 120725 m
#if ENABLE_IR_MSTAR_SOURCE_HOTKEY
    IRKEY_AV                = IRKEY_DUMY,
    IRKEY_HDMI              = IRKEY_DUMY,
    IRKEY_SCART             = IRKEY_DUMY,
    IRKEY_PC                = IRKEY_DUMY,
    IRKEY_DTV               = IRKEY_DUMY,
    IRKEY_TV                = IRKEY_DUMY,
    IRKEY_COMPONENT         = IRKEY_DUMY,
    IRKEY_SV                = IRKEY_DUMY,

    IRKEY_DASH              = IRKEY_DUMY,  // no use
    IRKEY_BACKWARD          = IRKEY_DUMY,  // no use
    IRKEY_FORWARD           = IRKEY_DUMY, // no use
    IRKEY_PLAY              = IRKEY_DUMY, // no use
    IRKEY_RECORD            = IRKEY_DUMY, // no use
    IRKEY_STOP              = IRKEY_DUMY, // no use
    IRKEY_PAUSE             = IRKEY_DUMY, // no use
    IRKEY_MEMORY_RECALL 	= IRKEY_DUMY, // no use
#else
    IRKEY_DASH              = IRKEY_DUMY-16,
    IRKEY_BACKWARD          =0x22, //IRKEY_DUMY-17,
    IRKEY_FORWARD           = 0x21,//IRKEY_DUMY-18,
    IRKEY_PLAY              = 0xc2,	//IRKEY_DUMY-63,//IRKEY_DUMY-19,	// lin 120725 m

// lin 110428 m >
    IRKEY_RECORD            = 0xf4,	//0x28,	// lin 120725 m
//    IRKEY_RECORD            = IRKEY_DUMY-20,
// lin 110428 m <

    IRKEY_STOP              = 0xc3,	//0x31,//IRKEY_DUMY-21,	// lin 120725 m
    IRKEY_PAUSE             =  IRKEY_DUMY,	//0x2f,//IRKEY_DUMY-22,//YRC-60p		// lin 120725 m
    IRKEY_MEMORY_RECALL 	  = IRKEY_DUMY-23,

    IRKEY_AV                = 0x4E, // no use



    IRKEY_HDMI              = 0x42, // no use



    IRKEY_SCART             = IRKEY_DUMY-24, // no use
    IRKEY_PC                =  0x50, // no use
    IRKEY_DTV               = 0x4B, // no use
    IRKEY_TV                = 0x48, // no use
    IRKEY_COMPONENT  = 0x4D, // no use

    IRKEY_USB  = 			0x26,	//0x25, // no use	// lin 120725 m
//     IRKEY_COMPONENT1  = 0x4C, // no use

    IRKEY_SV                = IRKEY_DUMY-25, // no use
#endif
    IRKEY_SLEEP             = 0x1a,	//0x1E,	// lin 120725 m
    IRKEY_EPG               = 0x0b,	//0x3A,	// lin 120725 m

    IRKEY_P_CHECK           = IRKEY_DUMY-26,
    IRKEY_S_CHECK           = IRKEY_DUMY-27,

  	IRKEY_MIX               = IRKEY_DUMY-28,
    IRKEY_INDEX             = IRKEY_DUMY-29,
    IRKEY_HOLD              = IRKEY_DUMY-30,

// lin 110428 m >
    IRKEY_PREVIOUS          = 0xcc,	//0x52,	// lin 120725 m
    IRKEY_NEXT              = 0xcb,	//IRKEY_DUMY-60,	// lin 120725 m
//    IRKEY_PREVIOUS          = 0x4c,//IRKEY_DUMY-31,
//    IRKEY_NEXT              = 0x27,//IRKEY_DUMY-32,
// lin 110428 m <

    IRKEY_FF                = 0x4a,	//0x1D,	// lin 120725 m
    IRKEY_REW               = 0xa0,	//0x1A,	// lin 120725 m
  //IRKEY_BRIGHTNESS_PLUS   = 0x0E,
  //IRKEY_BRIGHTNESS_MINUS  = 0x12,

    IRKEY_IN_STOP           = IRKEY_DUMY-33,
    IRKEY_IN_START          = IRKEY_DUMY-34,
    IRKEY_POWERONLY         = IRKEY_DUMY-35,

    IRKEY_TTX_MODE          = IRKEY_DUMY-36,
    IRKEY_RED2              = IRKEY_DUMY-37,
    IRKEY_GREEN2            = IRKEY_DUMY-38,
    IRKEY_UPDATE            = IRKEY_DUMY-39,

    IRKEY_SUBTITLE          =  IRKEY_DUMY-40,   //YRC-60p
//  IRKEY_SUBCODE           = 0x53,
    IRKEY_TIME           	  = IRKEY_DUMY-41,
//IRKEY_RECALL =0x34,
    IRKEY_SIZE              =  IRKEY_DUMY-15,
    IRKEY_REVEAL            = IRKEY_DUMY-42,
    IRKEY_RETURN            = IRKEY_DUMY-43,
    IRKEY_ADJUST            = IRKEY_DUMY-44,
    IRKEY_TV_INPUT          = IRKEY_DUMY-45,
    IRKEY_KEY_DISABLE_KEYPAD = IRKEY_DUMY-46,
    IRKEY_BACK              = IRKEY_DUMY-47,
    IRKEY_SUBPAGE   =   IRKEY_DUMY-48,
    IRKEY_VGATIMING =   IRKEY_DUMY-49,
    IRKEY_FACTORY           = 0x00 ,
    IRKEY_BURNING           = IRKEY_DUMY-50 ,    
    IRKEY_BACKLIGHT       = IRKEY_DUMY-51 ,
    
// lin 120906 m >    
    IRKEY_RECALL       = 0x35 ,
//    IRKEY_RECALL       = 0x34 ,//IRKEY_DUMY-51 ,
// lin 120906 m <

	IRKEY_HOME       = IRKEY_DUMY-53 ,
IRKEY_SHIFT_TIME   =IRKEY_DUMY-54 ,

#if 0
#if SAMPO_IPTV
	IRKEY_IPTV					= 0x5c,
	IRKEY_IPTV_NETWORK_SET= 0x59,
	IRKEY_IPTV_MAIN_INDEX= 0x54,
	IRKEY_IPTV_CLOUD_MAIN_INDEX= 0x58,
	
#if SAMPO_IPTV_KALAOK
    IRKEY_KALAOK     = 0x5F, //keli
#endif

#endif
#endif
#if ENABLE_3D_PROCESS
	IRKEY_3D = 0x5D,
#endif
// lin 110428 a >
    IRKEY_RECMENU			= 0x81,	//0x3B	// lin 120725 m,
    IRKEY_SCHEDULEREC		= 0x3a, //0x3F,	// lin 120725 m
// lin 110428 a <

}IrCommandType;
//*************************************************************************

//*************************************************************************
// IR system parameter define for H/W setting (Please don't modify them)
//*************************************************************************
#define IR_CKDIV_NUM             ((BIU_CLOCK + 500000UL) / 1000000UL)
#define IR_CKDIV_NUM_BOOT        13

#define IR_CLK_BOOT             (BIU_CLOCK_BOOT / 1000000.0)
#define IR_CLK                  (BIU_CLOCK / 1000000.0)
#define irGetMinCnt_BOOT(time, tolerance) (((double)time*((double)IR_CLK_BOOT)/(IR_CKDIV_NUM_BOOT+1))*((double)1-tolerance))
#define irGetMaxCnt_BOOT(time, tolerance) (((double)time*((double)IR_CLK_BOOT)/(IR_CKDIV_NUM_BOOT+1))*((double)1+tolerance))
#define irGetMinCnt(time, tolerance) (((double)time*((double)IR_CLK)/(IR_CKDIV_NUM+1))*((double)1-tolerance))
#define irGetMaxCnt(time, tolerance) (((double)time*((double)IR_CLK)/(IR_CKDIV_NUM+1))*((double)1+tolerance))

#define irGetCnt_BOOT(time) (((double)time*((double)IR_CLK_BOOT)/(IR_CKDIV_NUM_BOOT+1))+0x300000UL)
#define irGetCnt(time) (((double)time*((double)IR_CLK)/(IR_CKDIV_NUM+1))+0x300000UL)

// 12Mhz
#define IR_RP_TIMEOUT_BOOT      irGetCnt_BOOT(IR_TIMEOUT_CYC)
#define IR_HDC_UPB_BOOT         irGetMaxCnt_BOOT(IR_HEADER_CODE_TIME, 0.2)
#define IR_HDC_LOB_BOOT         irGetMinCnt_BOOT(IR_HEADER_CODE_TIME, 0.2)
#define IR_OFC_UPB_BOOT         irGetMaxCnt_BOOT(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_LOB_BOOT         irGetMinCnt_BOOT(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_RP_UPB_BOOT      irGetMaxCnt_BOOT(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_OFC_RP_LOB_BOOT      irGetMinCnt_BOOT(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_LG01H_UPB_BOOT       irGetMaxCnt_BOOT(IR_LOGI_01H_TIME, 0.35)
#define IR_LG01H_LOB_BOOT       irGetMinCnt_BOOT(IR_LOGI_01H_TIME, 0.3)
#define IR_LG0_UPB_BOOT         irGetMaxCnt_BOOT(IR_LOGI_0_TIME, 0.2)
#define IR_LG0_LOB_BOOT         irGetMinCnt_BOOT(IR_LOGI_0_TIME, 0.2)
#define IR_LG1_UPB_BOOT         irGetMaxCnt_BOOT(IR_LOGI_1_TIME, 0.2)
#define IR_LG1_LOB_BOOT         irGetMinCnt_BOOT(IR_LOGI_1_TIME, 0.2)

// 90Mhz
#define IR_RP_TIMEOUT           irGetCnt(IR_TIMEOUT_CYC)
#define IR_HDC_UPB              irGetMaxCnt(IR_HEADER_CODE_TIME, 0.2)
#define IR_HDC_LOB              irGetMinCnt(IR_HEADER_CODE_TIME, 0.2)
#define IR_OFC_UPB              irGetMaxCnt(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_LOB              irGetMinCnt(IR_OFF_CODE_TIME, 0.2)
#define IR_OFC_RP_UPB           irGetMaxCnt(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_OFC_RP_LOB           irGetMinCnt(IR_OFF_CODE_RP_TIME, 0.2)
#define IR_LG01H_UPB            irGetMaxCnt(IR_LOGI_01H_TIME, 0.35)
#define IR_LG01H_LOB            irGetMinCnt(IR_LOGI_01H_TIME, 0.3)
#define IR_LG0_UPB              irGetMaxCnt(IR_LOGI_0_TIME, 0.2)
#define IR_LG0_LOB              irGetMinCnt(IR_LOGI_0_TIME, 0.2)
#define IR_LG1_UPB              irGetMaxCnt(IR_LOGI_1_TIME, 0.2)
#define IR_LG1_LOB              irGetMinCnt(IR_LOGI_1_TIME, 0.2)

// Power off mode
#define PM_IR_TH_VAL            (PM_TH0_VAL & 0x0F)
// PM_IR_TH_GRID specify the time (in us) for each threshold bit.
// So PM_IR_TH_GRID = (1/12M) * (2^14) * 1000000 = (2^14) / 12
#define PM_IR_TH_GRID           (16384/IR_CLK_BOOT)
#define PM_IR_HEADER_CODE_TIME  (IR_HEADER_CODE_TIME-(0x0F-PM_IR_TH_VAL)*PM_IR_TH_GRID)
#define PM_IR_HDC_UPB_BOOT      irGetMaxCnt_BOOT(PM_IR_HEADER_CODE_TIME, 0.6)
#define PM_IR_HDC_LOB_BOOT      irGetMinCnt_BOOT(PM_IR_HEADER_CODE_TIME, 0.6)
//*************************************************************************

#define IR_SMC_HEADER_CNT_LB       12000
#define IR_SMC_HEADER_CNT_UB       16000
#define IR_SMC_REPEATE_CNT_LB       9000
#define IR_SMC_REPEATE_CNT_UB       12000
#define IR_SMC_LOGIC0_CNT_LB       800
#define IR_SMC_LOGIC0_CNT_UB       1400
#define IR_SMC_LOGIC1_CNT_LB       1600
#define IR_SMC_LOGIC1_CNT_UB       2500
#define IR_SMC_REPEATE_TIMEOUT_CNT     150


//anviIR
#define IR_WAKEUP_KEY0              IRKEY_POWER //configured by application
#define IR_WAKEUP_KEY1              IRKEY_POWER //configured by application
#define IR_WAKEUP_KEY2              IRKEY_POWER //configured by application
#define IR_WAKEUP_KEY3              IRKEY_POWER //configured by application


#define IR_2ND_NEC_DECODE_EN        DISABLE
#define IR_HEADER_CODE_2ND0         0xFF    // 2nd Custom 0  // anviIR
#define IR_HEADER_CODE_2ND1         0xFF    // 2nd Custom 1

#endif
