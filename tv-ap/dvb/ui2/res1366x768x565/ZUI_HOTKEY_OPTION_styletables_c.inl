////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2010 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (''MStar Confidential Information'') by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// This file is automatically generated by SkinTool [Version:0.2.3][Build:Jun 14 2017 09:17:51]
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////
// MAINFRAME styles..


/////////////////////////////////////////////////////
// HOTKEY_BG styles..


/////////////////////////////////////////////////////
// HOTKEY_BG_C styles..

static DRAWSTYLE _MP_TBLSEG _Zui_Hotkey_Bg_C_Normal_DrawStyle[] =
{
    { CP_BITMAP, CP_ZUI_BITMAP_INDEX_143 },
    { CP_NOON, 0 },
};

/////////////////////////////////////////////////////
// HOTKEY_PICTURE_MODE styles..

static DRAWSTYLE _MP_TBLSEG _Zui_Hotkey_Picture_Mode_Normal_DrawStyle[] =
{
    { CP_BITMAP, CP_ZUI_BITMAP_INDEX_211 },
    { CP_NOON, 0 },
};

/////////////////////////////////////////////////////
// HOTKEY_PICTURE_MODE_TEXT styles..

static DRAWSTYLE _MP_TBLSEG _Zui_Hotkey_Picture_Mode_Text_Normal_DrawStyle[] =
{
    { CP_TEXT_OUT, CP_ZUI_TEXT_OUT_INDEX_921 },
    { CP_NOON, 0 },
};
#define _Zui_Hotkey_Picture_Mode_Text_Focus_DrawStyle _Zui_Hotkey_Picture_Mode_Text_Normal_DrawStyle

/////////////////////////////////////////////////////
// HOTKEY_SOUND_MODE styles..

static DRAWSTYLE _MP_TBLSEG _Zui_Hotkey_Sound_Mode_Normal_DrawStyle[] =
{
    { CP_BITMAP, CP_ZUI_BITMAP_INDEX_212 },
    { CP_NOON, 0 },
};

/////////////////////////////////////////////////////
// HOTKEY_SOUND_MODE_TEXT styles..

#define _Zui_Hotkey_Sound_Mode_Text_Normal_DrawStyle _Zui_Hotkey_Picture_Mode_Text_Normal_DrawStyle
#define _Zui_Hotkey_Sound_Mode_Text_Focus_DrawStyle _Zui_Hotkey_Picture_Mode_Text_Normal_DrawStyle

/////////////////////////////////////////////////////
// HOTKEY_ZOOM styles..

static DRAWSTYLE _MP_TBLSEG _Zui_Hotkey_Zoom_Normal_DrawStyle[] =
{
    { CP_BITMAP, CP_ZUI_BITMAP_INDEX_213 },
    { CP_NOON, 0 },
};

/////////////////////////////////////////////////////
// HOTKEY_ZOOM_TEXT styles..

static DRAWSTYLE _MP_TBLSEG _Zui_Hotkey_Zoom_Text_Normal_DrawStyle[] =
{
    { CP_TEXT_OUT, CP_ZUI_TEXT_OUT_INDEX_922 },
    { CP_NOON, 0 },
};
#define _Zui_Hotkey_Zoom_Text_Focus_DrawStyle _Zui_Hotkey_Zoom_Text_Normal_DrawStyle

/////////////////////////////////////////////////////
// HOTKEY_SLEEP_TIMER styles..

static DRAWSTYLE _MP_TBLSEG _Zui_Hotkey_Sleep_Timer_Normal_DrawStyle[] =
{
    { CP_BITMAP, CP_ZUI_BITMAP_INDEX_214 },
    { CP_NOON, 0 },
};

/////////////////////////////////////////////////////
// HOTKEY_SLEEP_TIMER_TEXT styles..

static DRAWSTYLE _MP_TBLSEG _Zui_Hotkey_Sleep_Timer_Text_Normal_DrawStyle[] =
{
    { CP_TEXT_OUT, CP_ZUI_TEXT_OUT_INDEX_923 },
    { CP_NOON, 0 },
};
#define _Zui_Hotkey_Sleep_Timer_Text_Focus_DrawStyle _Zui_Hotkey_Sleep_Timer_Text_Normal_DrawStyle

/////////////////////////////////////////////////////
// HOTKEY_MTS styles..

static DRAWSTYLE _MP_TBLSEG _Zui_Hotkey_Mts_Normal_DrawStyle[] =
{
    { CP_FILL_RECT, CP_ZUI_FILL_RECT_INDEX_14 },
    { CP_NOON, 0 },
};

/////////////////////////////////////////////////////
// HOTKEY_MTS_TEXT styles..

static DRAWSTYLE _MP_TBLSEG _Zui_Hotkey_Mts_Text_Normal_DrawStyle[] =
{
    { CP_TEXT_OUT, CP_ZUI_TEXT_OUT_INDEX_924 },
    { CP_NOON, 0 },
};
#define _Zui_Hotkey_Mts_Text_Focus_DrawStyle _Zui_Hotkey_Mts_Text_Normal_DrawStyle

/////////////////////////////////////////////////////
// HOTKEY_FREEZE styles..

#define _Zui_Hotkey_Freeze_Normal_DrawStyle _Zui_Hotkey_Mts_Normal_DrawStyle

/////////////////////////////////////////////////////
// HOTKEY_FREEZE_TEXT styles..

#define _Zui_Hotkey_Freeze_Text_Normal_DrawStyle _Zui_Hotkey_Mts_Text_Normal_DrawStyle
#define _Zui_Hotkey_Freeze_Text_Focus_DrawStyle _Zui_Hotkey_Mts_Text_Normal_DrawStyle

/////////////////////////////////////////////////////
// HOTKEY_PIP_MODE styles..

static DRAWSTYLE _MP_TBLSEG _Zui_Hotkey_Pip_Mode_Normal_DrawStyle[] =
{
    { CP_BITMAP, CP_ZUI_BITMAP_INDEX_215 },
    { CP_NOON, 0 },
};

/////////////////////////////////////////////////////
// HOTKEY_PIP_MODE_TEXT styles..

#define _Zui_Hotkey_Pip_Mode_Text_Normal_DrawStyle _Zui_Hotkey_Picture_Mode_Text_Normal_DrawStyle
#define _Zui_Hotkey_Pip_Mode_Text_Focus_DrawStyle _Zui_Hotkey_Picture_Mode_Text_Normal_DrawStyle

/////////////////////////////////////////////////////
// HOTKEY_CC styles..

#define _Zui_Hotkey_Cc_Normal_DrawStyle _Zui_Hotkey_Mts_Normal_DrawStyle

/////////////////////////////////////////////////////
// HOTKEY_CC_TEXT styles..

#define _Zui_Hotkey_Cc_Text_Normal_DrawStyle _Zui_Hotkey_Mts_Text_Normal_DrawStyle
#define _Zui_Hotkey_Cc_Text_Focus_DrawStyle _Zui_Hotkey_Mts_Text_Normal_DrawStyle

//////////////////////////////////////////////////////
// Window Draw Style List  (normal, focused, disable)

WINDOWDRAWSTYLEDATA _MP_TBLSEG _GUI_WindowsDrawStyleList_Zui_Hotkey_Option[] =
{
    // HWND_MAINFRAME
    { NULL, NULL, NULL     },

    // HWND_HOTKEY_BG
    { NULL, NULL, NULL     },

    // HWND_HOTKEY_BG_C
    { _Zui_Hotkey_Bg_C_Normal_DrawStyle, NULL, NULL     },

    // HWND_HOTKEY_PICTURE_MODE
    { _Zui_Hotkey_Picture_Mode_Normal_DrawStyle, NULL, NULL     },

    // HWND_HOTKEY_PICTURE_MODE_TEXT
    { _Zui_Hotkey_Picture_Mode_Text_Normal_DrawStyle, _Zui_Hotkey_Picture_Mode_Text_Focus_DrawStyle, NULL     },

    // HWND_HOTKEY_SOUND_MODE
    { _Zui_Hotkey_Sound_Mode_Normal_DrawStyle, NULL, NULL     },

    // HWND_HOTKEY_SOUND_MODE_TEXT
    { _Zui_Hotkey_Sound_Mode_Text_Normal_DrawStyle, _Zui_Hotkey_Sound_Mode_Text_Focus_DrawStyle, NULL     },

    // HWND_HOTKEY_ZOOM
    { _Zui_Hotkey_Zoom_Normal_DrawStyle, NULL, NULL     },

    // HWND_HOTKEY_ZOOM_TEXT
    { _Zui_Hotkey_Zoom_Text_Normal_DrawStyle, _Zui_Hotkey_Zoom_Text_Focus_DrawStyle, NULL     },

    // HWND_HOTKEY_SLEEP_TIMER
    { _Zui_Hotkey_Sleep_Timer_Normal_DrawStyle, NULL, NULL     },

    // HWND_HOTKEY_SLEEP_TIMER_TEXT
    { _Zui_Hotkey_Sleep_Timer_Text_Normal_DrawStyle, _Zui_Hotkey_Sleep_Timer_Text_Focus_DrawStyle, NULL     },

    // HWND_HOTKEY_MTS
    { _Zui_Hotkey_Mts_Normal_DrawStyle, NULL, NULL     },

    // HWND_HOTKEY_MTS_TEXT
    { _Zui_Hotkey_Mts_Text_Normal_DrawStyle, _Zui_Hotkey_Mts_Text_Focus_DrawStyle, NULL     },

    // HWND_HOTKEY_FREEZE
    { _Zui_Hotkey_Freeze_Normal_DrawStyle, NULL, NULL     },

    // HWND_HOTKEY_FREEZE_TEXT
    { _Zui_Hotkey_Freeze_Text_Normal_DrawStyle, _Zui_Hotkey_Freeze_Text_Focus_DrawStyle, NULL     },

    // HWND_HOTKEY_PIP_MODE
    { _Zui_Hotkey_Pip_Mode_Normal_DrawStyle, NULL, NULL     },

    // HWND_HOTKEY_PIP_MODE_TEXT
    { _Zui_Hotkey_Pip_Mode_Text_Normal_DrawStyle, _Zui_Hotkey_Pip_Mode_Text_Focus_DrawStyle, NULL     },

    // HWND_HOTKEY_CC
    { _Zui_Hotkey_Cc_Normal_DrawStyle, NULL, NULL     },

    // HWND_HOTKEY_CC_TEXT
    { _Zui_Hotkey_Cc_Text_Normal_DrawStyle, _Zui_Hotkey_Cc_Text_Focus_DrawStyle, NULL     },

};
