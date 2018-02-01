////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
#define IR_MODE_SEL             IR_TYPE_FULLDECODE_MODE
#define IR_CHANNEL_USE_AS_UPDOWN   0
#define IR_VOLUME_USE_AS_LEFTRIGHT      0
// IR Header code define
#define IR_HEADER_CODE0         0x00    // Custom 0     //0x04
#define IR_HEADER_CODE1         0xDF    // Custom 1     //0xFB

// IR Timing define
#define IR_HEADER_CODE_TIME     9000    // us
#define IR_OFF_CODE_TIME        4500    // us
#define IR_OFF_CODE_RP_TIME     2500    // us
#define IR_LOGI_01H_TIME        560     // us
#define IR_LOGI_0_TIME          1125    // us
#define IR_LOGI_1_TIME          2250    // us
#define IR_TIMEOUT_CYC          140000  // us

#define IR_HEADER_CODE_TIME_UB  20
#define IR_HEADER_CODE_TIME_LB  -20
#define IR_OFF_CODE_TIME_UB  20
#define IR_OFF_CODE_TIME_LB  -20
#define IR_OFF_CODE_RP_TIME_UB  20
#define IR_OFF_CODE_RP_TIME_LB  -20
#define IR_LOGI_01H_TIME_UB  35
#define IR_LOGI_01H_TIME_LB  -30
#define IR_LOGI_0_TIME_UB  20
#define IR_LOGI_0_TIME_LB  -20
#define IR_LOGI_1_TIME_UB  20
#define IR_LOGI_1_TIME_LB  -20
// IR Format define
#define IRKEY_DUMY              0xFF
#define IRDA_KEY_MAPPING_POWER  IRKEY_POWER

#define IR_LEADER_CODE_CHECKING_OPTION 0xBF
typedef enum _IrCommandType
{
    IRKEY_ANTENNA           = IRKEY_DUMY-12,
    IRKEY_TV_RADIO          = IRKEY_DUMY-10,
    IRKEY_CHANNEL_LIST      = 0x52,
    IRKEY_CHANNEL_FAV_LIST  = 0x58,
    IRKEY_CHANNEL_RETURN    = IRKEY_DUMY-2,
    IRKEY_CHANNEL_PLUS      = 0x09,
    IRKEY_CHANNEL_MINUS     = 0x05,

    IRKEY_AUDIO             = 0x4A,
    IRKEY_VOLUME_PLUS       = 0x4b,
    IRKEY_VOLUME_MINUS      = 0x4f,

    IRKEY_UP                = 0x1a,
    IRKEY_POWER             = 0x1c,
    IRKEY_EXIT              = 0x49,
    IRKEY_MENU              = 0x18,
    IRKEY_DOWN              = 0x48,
    IRKEY_LEFT              = 0x47,
    IRKEY_SELECT            = 0x06,
    IRKEY_RIGHT             = 0x07,

    IRKEY_NUM_0             = 0x0c,
    IRKEY_NUM_1             = 0x54,
    IRKEY_NUM_2             = 0x16,
    IRKEY_NUM_3             = 0x15,
    IRKEY_NUM_4             = 0x50,
    IRKEY_NUM_5             = 0x12,
    IRKEY_NUM_6             = 0x11,
    IRKEY_NUM_7             = 0x4c,
    IRKEY_NUM_8             = 0x0e,
    IRKEY_NUM_9             = 0x0d,

    IRKEY_MUTE              = 0x08,
    IRKEY_FREEZE            = IRKEY_DUMY-40,
    IRKEY_PAGE_UP           = IRKEY_DUMY-3,
    IRKEY_PAGE_DOWN         = IRKEY_DUMY-4,
    IRKEY_CLOCK             = IRKEY_DUMY-5,

    IRKEY_INFO              = 0x0a,
    IRKEY_RED               = 0x5d,
    IRKEY_GREEN             = 0x59,
    IRKEY_YELLOW            = 0x51,
    IRKEY_BLUE              = 0x55,
    IRKEY_MTS               = 0x14,
    IRKEY_NINE_LATTICE      = IRKEY_DUMY-11,
    IRKEY_TTX               = 0x03,
    IRKEY_CC                = IRKEY_DUMY-17,
    IRKEY_INPUT_SOURCE      = 0x17,
    IRKEY_CRADRD            = IRKEY_DUMY-13,
    IRKEY_PICTURE           = 0x4e,
    IRKEY_ZOOM              = 0x02,
    IRKEY_DASH              = 0x10,
    IRKEY_SLEEP             = 0x1e,
    IRKEY_EPG               = 0x40,

    IRKEY_P_CHECK           = IRKEY_DUMY-7,
    IRKEY_S_CHECK           = IRKEY_DUMY-8,

    IRKEY_MIX               = IRKEY_DUMY-14,
    IRKEY_INDEX             = IRKEY_DUMY-15,
    IRKEY_HOLD              = IRKEY_DUMY-16,
    //IRKEY_PREVIOUS          = IRKEY_DUMY-17,
    //IRKEY_NEXT              = IRKEY_DUMY-18,
  //IRKEY_BRIGHTNESS_PLUS   = 0x0E,
  //IRKEY_BRIGHTNESS_MINUS  = 0x12,
    IRKEY_BACKWARD          = IRKEY_DUMY-19,
    IRKEY_FORWARD           = IRKEY_DUMY-20,
    IRKEY_PLAY              = IRKEY_DUMY-21,
    IRKEY_RECORD            = IRKEY_DUMY-22,
    IRKEY_STOP              = IRKEY_DUMY-23,
    IRKEY_PAUSE             = IRKEY_DUMY-24,

    IRKEY_POWERONLY         = IRKEY_DUMY-53,

    IRKEY_TTX_MODE          = IRKEY_DUMY-54,
    IRKEY_RED2              = IRKEY_DUMY-55,
    IRKEY_GREEN2            = IRKEY_DUMY-56,
    IRKEY_UPDATE            = IRKEY_DUMY-57,
    IRKEY_SUBTITLE          = 0x57,
//  IRKEY_SUBCODE           = IRKEY_DUMY-59,
    IRKEY_TIME               = IRKEY_DUMY-60,


    IRKEY_SIZE              = IRKEY_DUMY-25,
    IRKEY_REVEAL            = IRKEY_DUMY-26,
    IRKEY_RETURN            = 0x00,
    IRKEY_ADJUST            = IRKEY_DUMY-27,
    IRKEY_TV_INPUT          = IRKEY_DUMY-28,
    IRKEY_KEY_DISABLE_KEYPAD = IRKEY_DUMY-29,
    IRKEY_BACK              = IRKEY_DUMY-30,


    // source switching hotkeys
    IRKEY_AV                 = IRKEY_DUMY-31,
    IRKEY_HDMI            = IRKEY_DUMY-32,
    IRKEY_SCART         = IRKEY_DUMY-33,
    IRKEY_PC                = IRKEY_DUMY-34,
    IRKEY_TV                = IRKEY_DUMY-35,
    IRKEY_DTV              = IRKEY_DUMY-36,
    IRKEY_COMPONENT  = IRKEY_DUMY-37,
    IRKEY_SV                = IRKEY_DUMY-38,

    IRKEY_SUBPAGE   =   IRKEY_DUMY-39,
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
//********************************
//
//  IR Wakeup keys for standby mode
//
//********************************
#define IR_WAKEUP_KEY0              IRKEY_POWER //configured by application
#define IR_WAKEUP_KEY1              IRKEY_POWER //configured by application
#define IR_WAKEUP_KEY2              IRKEY_POWER //configured by application
#define IR_WAKEUP_KEY3              IRKEY_POWER //configured by application

//********************************
//
//  Support 2nd NEC Full Decode
//
//********************************
// 2nd IR Header code define
#define IR_2ND_NEC_DECODE_EN        DISABLE
#define IR_HEADER_CODE_2ND0         0xFF    // 2nd Custom 0
#define IR_HEADER_CODE_2ND1         0xFF    // 2nd Custom 1

#endif
