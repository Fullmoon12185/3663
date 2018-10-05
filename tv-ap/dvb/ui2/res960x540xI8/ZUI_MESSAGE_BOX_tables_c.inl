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


/// @Keymap_Table_Begin
static KEYEVENT _ZUI_TBLSEG _Msgbox_Btn_Ok_Item_Keymap[] =
{
#if (ENABLE_UPDATE_MULTIPLEX_VIA_NIT)
{VK_SELECT, EN_EXE_MESSAGE_BOX_BTN_SELECT_OK},
#ifdef ENABLE_DELETE_PROGRAM_CONTROL
{VK_LEFT, EN_EXE_MESSAGE_BOX_BTN_SELECT_YES},
{VK_RIGHT, EN_EXE_MESSAGE_BOX_BTN_SELECT_NO},
#endif
#endif
{VK_NULL, 0},
};

static KEYEVENT _ZUI_TBLSEG _Msgbox_USB_List_Item_Keymap[] =
{
{VK_SELECT, EN_EXE_USB_LIST_SEL},
{VK_NULL, 0},
};
/// @Keymap_Table_End

/////////////////////////////////////////////////////

// Navigation Table

/////////////////////////////////////////////////////

static HWND _MP_TBLSEG _MSGBOX_USB_LIST_ITEM0_Navigation[] = {HWND_MSGBOX_USB_LIST_ITEM3, HWND_MSGBOX_USB_LIST_ITEM1, HWND_MSGBOX_USB_LIST_ITEM0, HWND_MSGBOX_USB_LIST_ITEM0};

static HWND _MP_TBLSEG _MSGBOX_USB_LIST_ITEM1_Navigation[] = {HWND_MSGBOX_USB_LIST_ITEM0, HWND_MSGBOX_USB_LIST_ITEM2, HWND_MSGBOX_USB_LIST_ITEM1, HWND_MSGBOX_USB_LIST_ITEM1};

static HWND _MP_TBLSEG _MSGBOX_USB_LIST_ITEM2_Navigation[] = {HWND_MSGBOX_USB_LIST_ITEM1, HWND_MSGBOX_USB_LIST_ITEM3, HWND_MSGBOX_USB_LIST_ITEM2, HWND_MSGBOX_USB_LIST_ITEM2};

static HWND _MP_TBLSEG _MSGBOX_USB_LIST_ITEM3_Navigation[] = {HWND_MSGBOX_USB_LIST_ITEM2, HWND_MSGBOX_USB_LIST_ITEM1, HWND_MSGBOX_USB_LIST_ITEM3, HWND_MSGBOX_USB_LIST_ITEM3};

//////////////////////////////////////////////////////
// Window List

WINDOWDATA _MP_TBLSEG _GUI_WindowList_Zui_Message_Box[] =
{
    // HWND_MAINFRAME
    {
        EN_ZUI_MAINFRAMEWINPROC, NULL, WS_VISIBLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_BG
    {
        EN_ZUI_AUTOCLOSE_WINPROC, (void*)MESSAGE_BOX_TIME_OUT_MS, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_BG_TOP
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_NEW_BG_L
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_NEW_BG_C
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_NEW_BG_R
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_BG_L
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_BG_C
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_BG_R
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_TITLE
    {
        EN_ZUI_DYNAMICTEXT_WINPROC, NULL, WS_VISIBLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_TEXT_PANE
    {
        EN_ZUI_MSGBOXTEXTPANE_WINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_TEXT1
    {
        EN_ZUI_DYNAMICTEXT_WINPROC, NULL, WS_VISIBLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_TEXT2
    {
        EN_ZUI_DYNAMICTEXT_WINPROC, NULL, WS_VISIBLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_PASSWORD_PANE
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_PASSWORD_INPUT_1
    {
        EN_ZUI_MSGBOX_PWDINPUT_WINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_PASSWORD_INPUT_2
    {
        EN_ZUI_MSGBOX_PWDINPUT_WINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_PASSWORD_INPUT_3
    {
        EN_ZUI_MSGBOX_PWDINPUT_WINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_PASSWORD_INPUT_4
    {
        EN_ZUI_MSGBOX_PWDINPUT_WINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_PASSWORD_PRESSED_PANE
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_PASSWORD_PRESSED_1
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_PASSWORD_PRESSED_2
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_PASSWORD_PRESSED_3
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_PASSWORD_PRESSED_4
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_BTN_PANE
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_BTN_CLEAR
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_BTN_CLEAR_LEFT_ARROW
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_BTN_CLEAR_TEXT
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_BTN_OK
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_BTN_CANCEL_RIGHT_ARROW
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_BTN_CANCEL_TEXT
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_COMMON_MSG_OK
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        _Msgbox_Btn_Ok_Item_Keymap,
    },

    // HWND_MSGBOX_USB_LIST_PANE
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_USB_LIST_ITEM0
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        _MSGBOX_USB_LIST_ITEM0_Navigation,
        _Msgbox_USB_List_Item_Keymap,
    },

    // HWND_MSGBOX_USB_LIST_ITEM0_TEXT
    {
        EN_ZUI_DYNAMICTEXT_WINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_USB_LIST_ITEM1
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        _MSGBOX_USB_LIST_ITEM1_Navigation,
        _Msgbox_USB_List_Item_Keymap,
    },

    // HWND_MSGBOX_USB_LIST_ITEM1_TEXT
    {
        EN_ZUI_DYNAMICTEXT_WINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_USB_LIST_ITEM2
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        _MSGBOX_USB_LIST_ITEM2_Navigation,
        _Msgbox_USB_List_Item_Keymap,
    },

    // HWND_MSGBOX_USB_LIST_ITEM2_TEXT
    {
        EN_ZUI_DYNAMICTEXT_WINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

    // HWND_MSGBOX_USB_LIST_ITEM3
    {
        EN_ZUI_DEFAULTWINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        _MSGBOX_USB_LIST_ITEM3_Navigation,
        _Msgbox_USB_List_Item_Keymap,
    },

    // HWND_MSGBOX_USB_LIST_ITEM3_TEXT
    {
        EN_ZUI_DYNAMICTEXT_WINPROC, NULL, WS_VISIBLE|WS_FOCUSABLE,
        NULL,
        NULL,
    },

};
