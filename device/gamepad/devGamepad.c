//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
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
#include "Board.h"

#if( ENABLE_GAMEPAD )

#include <string.h>

#include "debug.h"


#include "drvUSB.h"

#include "msAPI_Timer.h"

#include "devGamepad.h"

//=================================================================================
// Extern function...
extern U8 flib_Host20_Get_BulkIn_Data(U16 hwSize, U32 pwBuffer, U32 *actual_len, U16 wTimeOutMs, BOOLEAN bCancelAfterTimeOut);
extern U8 flib_Host20_Get_BulkIn_Data_Port2(U16 hwSize, U32 pwBuffer, U32 *actual_len, U16 wTimeOutMs, BOOLEAN bCancelAfterTimeOut);

//==================================================================================

#define DEBUG_GAMEPAD(x)                x
#define DEBUG_GAMEPAD_CONNECT_STATUS(x) //x

#define DEBUG_GAMEPAD_KEY               0

//---------------------------------------------------------------------------------------
#define DEBUG_GET_RAW_TIME      0

#if(DEBUG_GET_RAW_TIME )
    #define DEBUG_GAMEPAD_TIME(x)   x
#else
    #define DEBUG_GAMEPAD_TIME(x)
#endif

#if(DEBUG_GET_RAW_TIME )
#define GAMEPAD_RAWDATA_CAP_TIME    50
static U32 s_Gamepad_au32RawDataTime[GAMEPAD_RAWDATA_CAP_TIME];
static U8 s_Gamepad_u8RawDataTimeIdx;
#endif

//---------------------------------------------------------------------------------------

#define USB_PORT_NUM    2


/*
typedef enum
{
    GAMEPAD_USBPORT_0 = BIT0,
    GAMEPAD_USBPORT_1 = BIT1,
    GAMEPAD_USBPORT_NONE,
} EnuGamepadUSBPort;
*/

typedef enum
{
    E_GAMEPAD_USB_PORT_NO_CONNECT,
    E_GAMEPAD_USB_PORT_CONNECTED,
    E_GAMEPAD_USB_PORT_GAMEPAD_DETECTED,
    E_GAMEPAD_USB_PORT_NOT_GAMEPAD,
} EnuGamepadUsbPortStatus;

typedef enum
{
    E_GAMEPAD_STATE_INIT,
    E_GAMEPAD_STATE_CHECK_CONNECT,
    E_GAMEPAD_STATE_CHECK_GAMEPAD,
    E_GAMEPAD_STATE_DEVICE_READY,
} EnuGamepadState;

typedef struct
{
    U16 u16VendorID;
    U16 u16ProductID;
    EnuGamepadType eGamepadType;
}StruGamepadVendorInfo;

static StruGamepadVendorInfo g_astGamepadVendorInfo[] =
{
    {0x0079 , 0x0006, GAMEPAD_TYPE_DragonRise_PC_TWIN_SHOCK_Gamepad },  //DragonRise, PC TWIN SHOCK Gamepad
    {0x0079 , 0x0011, GAMEPAD_TYPE_DragonRise_Gamepad               }   //DragonRise, Gamepad
};

//static EnuGamepadState g_Gamepad_eState = E_GAMEPAD_STATE_INIT;
static EnuGamepadState g_Gamepad_aeState[USB_PORT_NUM];// = E_GAMEPAD_STATE_INIT;

//static U8 g_u8GamePadUSBPort = GAMEPAD_USBPORT_NONE;
static U8 g_GamePad_au8USBPortStatus[USB_PORT_NUM];// = GAMEPAD_USBPORT_NONE;

//static EnuGamepadType g_Gamepad_eType = GAMEPAD_TYPE_NUMS;
static EnuGamepadType g_Gamepad_aeType[USB_PORT_NUM];// = GAMEPAD_TYPE_NUMS;

static U8 g_Gamepad_au8LastConnectStatus[USB_PORT_NUM];// = 0;

static U8 g_Gamepad_au8KeyStatusBuf[USB_PORT_NUM][GAMEPAD_KEY_MAX_NUM];//= {0};

#define GAMEPAD_RAWDATA_SIZE    8

//static U8 g_USB_au8RawDataBuf[GAMEPAD_RAWDATA_SIZE];//= {0};
static U8 g_USB_au8RawDataBuf[USB_PORT_NUM][GAMEPAD_RAWDATA_SIZE];//= {0};

//===========================================================================
#ifndef ENABLE_AEON_GAMEPAD
    #define ENABLE_AEON_GAMEPAD 0
#endif
#if(ENABLE_AEON_GAMEPAD)
#include "devGamepadAeon.c"
#endif
//===========================================================================

U8 devGamepad_USB_Get_ConnectionStatus_Port(U8 u8UsbPortIdx)
{
    U8 u8ConnectStatusResult = 0;

    if( u8UsbPortIdx == 1 )
    {
        if(MDrv_UsbDeviceConnect_Port2())  //usb port 1
        {
            u8ConnectStatusResult = 1;
        }
    }
    else
    {
        if( MDrv_UsbDeviceConnect() ) //usb port 0
        {
            u8ConnectStatusResult = 1;
        }
    }

    return u8ConnectStatusResult;
}

#if 0
U8 devGamepad_USB_Get_GamepadConnectionStatus(void)
{
    U8 u8ConnectStatusResult = 0;

    {
        if( MDrv_UsbDeviceConnect() ) //usb port 0
        {
            DEBUG_GAMEPAD_CONNECT_STATUS(printf("[gamepad] p0 Device exist\n"););
            u8ConnectStatusResult |= BIT0;
        }

        if(MDrv_UsbDeviceConnect_Port2())  //usb port 1
        {
            DEBUG_GAMEPAD_CONNECT_STATUS(printf("[gamepad] p1 Device exist\n"););
            u8ConnectStatusResult |= BIT1;
        }
    }

    if( u8ConnectStatusResult == 0 )
    {
        DEBUG_GAMEPAD_CONNECT_STATUS(printf("[gamepad] Device not exist\n"););
    }

    return u8ConnectStatusResult;
}
#endif

BOOLEAN devGamepad_USB_DeviceEnumeration_Port(U8 u8UsbPortIdx)
{
    BOOLEAN bHIDDeviceEnumerationResult = FALSE;
    BOOLEAN bIsGetDeviceFromEnumeration = FALSE;


    if( u8UsbPortIdx == 0 )
    {
        bIsGetDeviceFromEnumeration = MDrv_UsbHost_Init_Enum();
    }
    else
    {
        bIsGetDeviceFromEnumeration = MDrv_UsbHost_Init_Enum_Port2();
    }

    if( bIsGetDeviceFromEnumeration )
    {
        if( u8UsbPortIdx == 0 )
        {
            if( MDrv_GetUsbDeviceType() == USB_INTERFACE_CLASS_HID)
            {
                DEBUG_GAMEPAD_CONNECT_STATUS(printf("[gamepad] Port%d Device is HID class\n", u8UsbPortIdx););
                bHIDDeviceEnumerationResult = TRUE;
            }
        }
        else
        {
            if( MDrv_GetUsbDeviceType_Port2() == USB_INTERFACE_CLASS_HID)
            {
                DEBUG_GAMEPAD_CONNECT_STATUS(printf("[gamepad] Port%d Device is HID class\n", u8UsbPortIdx););
                bHIDDeviceEnumerationResult = TRUE;
            }
        }
    }

    if( bHIDDeviceEnumerationResult == FALSE )
    {
        DEBUG_GAMEPAD_CONNECT_STATUS(printf("[gamepad] Port%d Device is not HID class\n", u8UsbPortIdx););
    }

    return bHIDDeviceEnumerationResult;
}

#define GAMEPAD_USB_GET_RAWDATA_TIMEOUT 10
BOOLEAN devGamepad_USB_Get_RawData_Port(U8 u8UsbPortIdx)
{
#if (ENABLE_AEON_GAMEPAD) // Aeon get rawdata

    if( g_GamepadAeon_bRawDataUpdated )
    {
        g_GamepadAeon_bRawDataUpdated = 0;

        if( g_GamepadAeon_u8LastRawDataIdx )
            memcpy(g_USB_au8RawDataBuf[u8UsbPortIdx], g_GamepadAeon_au8RawData[1], GAMEPAD_RAWDATA_SIZE);
        else
            memcpy(g_USB_au8RawDataBuf[u8UsbPortIdx], g_GamepadAeon_au8RawData[0], GAMEPAD_RAWDATA_SIZE);
    }

    return TRUE;

#else // HK get rawdata

    BOOLEAN bRtn = TRUE;
    U32 u32UsbActual_len = 0;
    U8 au8RawDataTmpBuf[GAMEPAD_RAWDATA_SIZE]= {0};
    U8 u8UsbDriverRtn;


  #if(DEBUG_GET_RAW_TIME )
    U32 u32T1 = MsOS_GetSystemTime();
  #endif

#if 1
    if( u8UsbPortIdx == 1 )
    {
        u8UsbDriverRtn = flib_Host20_Get_BulkIn_Data_Port2(GAMEPAD_RAWDATA_SIZE, (U32)au8RawDataTmpBuf, &u32UsbActual_len, GAMEPAD_USB_GET_RAWDATA_TIMEOUT, TRUE);
    }
    else
    {
        u8UsbDriverRtn = flib_Host20_Get_BulkIn_Data(GAMEPAD_RAWDATA_SIZE, (U32)au8RawDataTmpBuf, &u32UsbActual_len, GAMEPAD_USB_GET_RAWDATA_TIMEOUT, TRUE);
    }

#else
    switch(g_u8GamePadUSBPort)
    {
        case GAMEPAD_USBPORT_0:
            u8UsbDriverRtn = flib_Host20_Get_BulkIn_Data(GAMEPAD_RAWDATA_SIZE, (U32)au8RawDataTmpBuf, &u32UsbActual_len, GAMEPAD_USB_GET_RAWDATA_TIMEOUT, TRUE);
            //memcpy(g_USB_au8RawDataBuf, MDrv_Get_HID_Report_Raw_Data(), 8);
            break;
        case GAMEPAD_USBPORT_1:
            u8UsbDriverRtn = flib_Host20_Get_BulkIn_Data_Port2(GAMEPAD_RAWDATA_SIZE, (U32)au8RawDataTmpBuf, &u32UsbActual_len, GAMEPAD_USB_GET_RAWDATA_TIMEOUT, TRUE);
            //memcpy(g_USB_au8RawDataBuf, MDrv_Get_HID_Report_Raw_Data_Port2(), 8);
            break;
        default:
            printf("Invalid USB port\n");
            //bRtn = FALSE;
            break;
    }
#endif

    //printf("u32actual_len=%u, u8UsbDriverRtn=%u\n", u32actual_len, u8UsbDriverRtn);

  #if(DEBUG_GET_RAW_TIME )
    if( s_Gamepad_u8RawDataTimeIdx < GAMEPAD_RAWDATA_CAP_TIME )
    {
        s_Gamepad_au32RawDataTime[s_Gamepad_u8RawDataTimeIdx] = msAPI_Timer_DiffTime_2(u32T1, MsOS_GetSystemTime());

        s_Gamepad_u8RawDataTimeIdx += 1;

        if( s_Gamepad_u8RawDataTimeIdx == GAMEPAD_RAWDATA_CAP_TIME )
        {
            U8 i;
            for( i = 0; i < GAMEPAD_RAWDATA_CAP_TIME; i += 1)
            {
                printf("%d) %u\n", i, s_Gamepad_au32RawDataTime[i]);
            }
            s_Gamepad_u8RawDataTimeIdx += 1;
        }
    }
  #endif

    if( u32UsbActual_len != GAMEPAD_RAWDATA_SIZE )
    {
        printf("\nError: Usb rtn data len=%u\n", u32UsbActual_len);
        memset(g_USB_au8RawDataBuf[u8UsbPortIdx], 0, sizeof(g_USB_au8RawDataBuf) );
        //bRtn = FALSE;
    }
    else
    {
        memcpy(g_USB_au8RawDataBuf[u8UsbPortIdx], au8RawDataTmpBuf, GAMEPAD_RAWDATA_SIZE);
    }

    return bRtn;

#endif
}

//==============================================================
BOOLEAN devGamepad_CheckGamepadType(U8 u8UsbPortIdx)
{
    BOOLEAN bResult = FALSE;
    U16 u16USBPID = 0;  //product ID
    U16 u16USBVID = 0;  //vender ID
    U8 u8DeviceTypeIdx = 0;

    DEBUG_GAMEPAD( printf("devGamepad_CheckGamepadType(%u)\n", u8UsbPortIdx); );

    if(u8UsbPortIdx == 0)
    {
        MDrv_USBGetVIDPID(&u16USBVID,&u16USBPID); //waiting for UTPA API
    }
    else if(u8UsbPortIdx == 1)
    {
        MDrv_USBGetVIDPID_Port2(&u16USBVID,&u16USBPID);
    }

    DEBUG_GAMEPAD( printf(" u16USBPID %x , u16USBVID %x\n",u16USBPID, u16USBVID); );

    g_Gamepad_aeType[u8UsbPortIdx] = GAMEPAD_TYPE_Unknown;

    for(u8DeviceTypeIdx = 0 ; u8DeviceTypeIdx < (sizeof(g_astGamepadVendorInfo)/sizeof(StruGamepadVendorInfo) ); u8DeviceTypeIdx++)
    {
        if( (g_astGamepadVendorInfo[u8DeviceTypeIdx].u16VendorID == u16USBVID)
          &&(g_astGamepadVendorInfo[u8DeviceTypeIdx].u16ProductID == u16USBPID)
          )
        {
            g_Gamepad_aeType[u8UsbPortIdx] = g_astGamepadVendorInfo[u8DeviceTypeIdx].eGamepadType;
            break;
        }
    }

    DEBUG_GAMEPAD( printf(" g_Gamepad_eType=%d\n", g_Gamepad_aeType[u8UsbPortIdx]); );

    // Test ...
    if( g_Gamepad_aeType[u8UsbPortIdx] >= GAMEPAD_TYPE_NUMS )
    {
        bResult = FALSE;
    }
    else if( g_Gamepad_aeType[u8UsbPortIdx] == GAMEPAD_TYPE_Unknown )
    {
        printf("Can not find gamepad, use default gamepad setting!\n");
        bResult = TRUE;
    }
    else
    {
        bResult = TRUE;
    }

    return bResult;
}

#if 1
BOOLEAN devGamepad_CheckGamepadReady_Port(U8 u8UsbPortIdx)
{
    DEBUG_GAMEPAD( printf("devGamepad_CheckGamepadReady_Port(%d)\n", u8UsbPortIdx); );
    BOOLEAN bResult = FALSE;
    U8 u8PortMask;


    u8PortMask = 1 << u8UsbPortIdx;

    // Enum device
    if( devGamepad_USB_DeviceEnumeration_Port(u8UsbPortIdx) ) // Enum OK
    {
        if( devGamepad_CheckGamepadType(u8UsbPortIdx) )
        {
            g_GamePad_au8USBPortStatus[u8UsbPortIdx] = E_GAMEPAD_USB_PORT_GAMEPAD_DETECTED;
            printf("Found gamepad at port-%d\n", u8UsbPortIdx);
            bResult = TRUE;
        }
        else
        {
            g_GamePad_au8USBPortStatus[u8UsbPortIdx] = E_GAMEPAD_USB_PORT_NOT_GAMEPAD;
        }
    }

    return bResult;
}
#else
BOOLEAN devGamepad_CheckGamepadReady(U8 u8UsbPortStatus)
{
    DEBUG_GAMEPAD( printf("devGamepad_CheckGamepadReady(0x%X)\n", u8UsbPortStatus); );

    BOOLEAN bResult = FALSE;
    U8 u8PortIdx;
    U8 u8PortMask;

    g_u8GamePadUSBPort = 0;

    for( u8PortIdx = 0; u8PortIdx < 2; u8PortIdx += 1)
    {
        u8PortMask = 1 << u8PortIdx;

        // Enum device
        if( u8UsbPortStatus&u8PortMask )
        {
            if( devGamepad_USB_DeviceEnumeration_Port(u8PortIdx) ) // Enum OK
            {
                if( devGamepad_CheckGamepadType(u8PortIdx) )
                {
                    g_u8GamePadUSBPort = u8PortMask;
                    printf("Found gamepad at port%d\n", u8PortIdx);
                    bResult = TRUE;
                    break;
                }
            }
        }
    }

    return bResult;
}
#endif

void devGamepad_ResetAllVar(void)
{
    //g_Gamepad_eState = E_GAMEPAD_STATE_INIT;
    //g_u8GamePadUSBPort = GAMEPAD_USBPORT_NONE;
    //g_Gamepad_eType = GAMEPAD_TYPE_NUMS;


    U8 u8UsbPortIdx = 0;
    for( u8UsbPortIdx = 0; u8UsbPortIdx < USB_PORT_NUM; u8UsbPortIdx += 1 )
    {
        g_Gamepad_aeState[u8UsbPortIdx] = E_GAMEPAD_STATE_INIT;
        g_GamePad_au8USBPortStatus[u8UsbPortIdx] = E_GAMEPAD_USB_PORT_NO_CONNECT;
        g_Gamepad_aeType[u8UsbPortIdx] = GAMEPAD_TYPE_NUMS;
    }
}

void devGamepad_Init(void)
{
    DEBUG_GAMEPAD( PRINT_CURRENT_LINE(); );

    devGamepad_ResetAllVar();

    // Check connection...
    //U8 u8UsbConnectionStatus = devGamepad_USB_Get_ConnectionStatus();

    U8 u8UsbPortIdx = 0;
    for( u8UsbPortIdx = 0; u8UsbPortIdx < USB_PORT_NUM; u8UsbPortIdx += 1 )
    {
        g_Gamepad_aeState[u8UsbPortIdx] = E_GAMEPAD_STATE_CHECK_CONNECT;

        // Check connection...
        printf("Check USB port %u...\n", u8UsbPortIdx);
        U8 u8UsbConnectionStatus = devGamepad_USB_Get_ConnectionStatus_Port(u8UsbPortIdx);

        if( u8UsbConnectionStatus )
        {
            printf("USB device connected\n");
            g_GamePad_au8USBPortStatus[u8UsbPortIdx] = E_GAMEPAD_USB_PORT_CONNECTED;

            // enum device...
            //g_Gamepad_eState = E_GAMEPAD_STATE_CHECK_GAMEPAD;
            g_Gamepad_aeState[u8UsbPortIdx] = E_GAMEPAD_STATE_CHECK_GAMEPAD;

            printf("Check gamepad %u...\n", u8UsbPortIdx);
            if( FALSE == devGamepad_CheckGamepadReady_Port(u8UsbPortIdx) )
            {
                printf("\nError: check gamepad fail!\n");
                // todo
            }
            else
            {
                //g_Gamepad_eState = E_GAMEPAD_STATE_DEVICE_READY;
                g_Gamepad_aeState[u8UsbPortIdx] = E_GAMEPAD_STATE_DEVICE_READY;
                printf("Gamepad %d ready~\n", u8UsbPortIdx);
            }
        }
        else
        {
            // Do nothing...
            printf("No USB device\n");
            g_GamePad_au8USBPortStatus[u8UsbPortIdx] = E_GAMEPAD_USB_PORT_NO_CONNECT;
        }

        g_Gamepad_au8LastConnectStatus[u8UsbPortIdx] = u8UsbConnectionStatus;
    }

#if(ENABLE_AEON_GAMEPAD)
    devGamepad_Aeon_Init();
    //devGamepad_Aeon_Enable_GetRawData(ENABLE);
#endif

}

BOOLEAN devGamepad_ParseRawData_Default(U8* pu8RawDataBuf, U8* pu8KeyStatusBuf)
{
    //dump raw data,total 8 bytes
    //printf("[gamepad] 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",g_USB_au8RawDataBuf[0],g_USB_au8RawDataBuf[1],g_USB_au8RawDataBuf[2],g_USB_au8RawDataBuf[3],g_USB_au8RawDataBuf[4],g_USB_au8RawDataBuf[5],g_USB_au8RawDataBuf[6],g_USB_au8RawDataBuf[7]);
    memset(pu8KeyStatusBuf, 0, GAMEPAD_KEY_MAX_NUM);

    if(pu8RawDataBuf[3] == 0x00)  //g_USB_au8RawDataBuf[3] = left,right
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_LEFT] = GAMEPAD_ISKEYPRESSED;
        //printf("left\n");
    }
    if(pu8RawDataBuf[3] == 0xFF)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_RIGHT] = GAMEPAD_ISKEYPRESSED;
        //printf("right\n");
    }
    if(pu8RawDataBuf[4] == 0x00)  //g_USB_au8RawDataBuf[3] = up,down
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_UP] = GAMEPAD_ISKEYPRESSED;
        //printf("up\n");
    }
    if(pu8RawDataBuf[4] == 0xFF)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_DOWN] = GAMEPAD_ISKEYPRESSED;
        //printf("down\n");
    }
    if(pu8RawDataBuf[5] & BIT4)  //g_USB_au8RawDataBuf[5] = button1,button2,button3,button4
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_0] = GAMEPAD_ISKEYPRESSED;
        //printf("button1\n");
    }
    if(pu8RawDataBuf[5] & BIT5)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_1] = GAMEPAD_ISKEYPRESSED;
        //printf("button2\n");
    }
    if(pu8RawDataBuf[5] & BIT6)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_2] = GAMEPAD_ISKEYPRESSED;
        //printf("button3\n");
    }
    if(pu8RawDataBuf[5] & BIT7)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_3] = GAMEPAD_ISKEYPRESSED;
        //printf("button4\n");
    }
    if(pu8RawDataBuf[6] & BIT4) //g_USB_au8RawDataBuf[6] = select,start,L1,L2,R1,R2
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_SEL] = GAMEPAD_ISKEYPRESSED;
        //printf("select \n");
    }
    if(pu8RawDataBuf[6] & BIT5)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_START] = GAMEPAD_ISKEYPRESSED;
        //printf("start \n");
    }
    if(pu8RawDataBuf[6] & BIT0)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_L1] = GAMEPAD_ISKEYPRESSED;
        //printf("L1\n");
    }
    if(pu8RawDataBuf[6] & BIT1)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_R1] = GAMEPAD_ISKEYPRESSED;
        //printf("R1\n");
    }
    if(pu8RawDataBuf[6] & BIT2)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_L2] = GAMEPAD_ISKEYPRESSED;
        //printf("L2\n");
    }
    if(pu8RawDataBuf[6] & BIT3)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_R2] = GAMEPAD_ISKEYPRESSED;
        //printf("R2\n");
    }
    return TRUE;
}
BOOLEAN devGamepad_ParseRawData_DragonRise_SHOCK_Gamepad(U8* pu8RawDataBuf, U8* pu8KeyStatusBuf)
{
    memset(pu8KeyStatusBuf, 0, GAMEPAD_KEY_MAX_NUM);

    //dump raw data,total 8 bytes
    //printf("[gamepad] %02x %02x %02x %02x %02x %02x %02x %02x\n",g_USB_au8RawDataBuf[0],g_USB_au8RawDataBuf[1],g_USB_au8RawDataBuf[2],g_USB_au8RawDataBuf[3],g_USB_au8RawDataBuf[4],g_USB_au8RawDataBuf[5],g_USB_au8RawDataBuf[6],g_USB_au8RawDataBuf[7]);

    // default:   7F 7F 7D 7F 7F 0F 00 C0
    // Up:          0x7F 0x00 0x7E 0x7F 0x7F 0xF 0x0 0xC0
    // Down:     0x7F 0xFF 0x7E 0x7F 0x7F 0xF 0x0 0xC0

#if 1
    if(pu8RawDataBuf[0] == 0x00)  //g_USB_au8RawDataBuf[3] = left,right
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_LEFT] = GAMEPAD_ISKEYPRESSED;
        //printf("left\n");
    }
    if(pu8RawDataBuf[0] == 0xFF)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_RIGHT] = GAMEPAD_ISKEYPRESSED;
        //printf("right\n");
    }
    if(pu8RawDataBuf[1] == 0x00)  //g_USB_au8RawDataBuf[3] = up,down
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_UP] = GAMEPAD_ISKEYPRESSED;
        //printf("up\n");
    }
    if(pu8RawDataBuf[1] == 0xFF)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_DOWN] = GAMEPAD_ISKEYPRESSED;
        //printf("down\n");
    }

    if(pu8RawDataBuf[5] & BIT4)  //g_USB_au8RawDataBuf[5] = button1,button2,button3,button4
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_0] = GAMEPAD_ISKEYPRESSED;
        //printf("button1\n");
    }
    if(pu8RawDataBuf[5] & BIT5)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_1] = GAMEPAD_ISKEYPRESSED;
        //printf("button2\n");
    }
    if(pu8RawDataBuf[5] & BIT6)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_2] = GAMEPAD_ISKEYPRESSED;
        //printf("button3\n");
    }
    if(pu8RawDataBuf[5] & BIT7)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_3] = GAMEPAD_ISKEYPRESSED;
        //printf("button4\n");
    }

    if(pu8RawDataBuf[6] & BIT4) //g_USB_au8RawDataBuf[6] = select,start,L1,L2,R1,R2
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_SEL] = GAMEPAD_ISKEYPRESSED;
        //printf("select \n");
    }
    if(pu8RawDataBuf[6] & BIT5)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_START] = GAMEPAD_ISKEYPRESSED;
        //printf("start \n");
    }
    if(pu8RawDataBuf[6] & BIT0)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_L1] = GAMEPAD_ISKEYPRESSED;
        //printf("L1\n");
    }
    if(pu8RawDataBuf[6] & BIT1)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_R1] = GAMEPAD_ISKEYPRESSED;
        //printf("R1\n");
    }
    if(pu8RawDataBuf[6] & BIT2)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_L2] = GAMEPAD_ISKEYPRESSED;
        //printf("L2\n");
    }
    if(pu8RawDataBuf[6] & BIT3)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_R2] = GAMEPAD_ISKEYPRESSED;
        //printf("R2\n");
    }
#endif

    return TRUE;
}

BOOLEAN devGamepad_ParseRawData_DragonRise_Gamepad(U8* pu8RawDataBuf, U8* pu8KeyStatusBuf)
{
    //dump raw data,total 8 bytes
    //printf("[gamepad] 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",g_USB_au8RawDataBuf[0],g_USB_au8RawDataBuf[1],g_USB_au8RawDataBuf[2],g_USB_au8RawDataBuf[3],g_USB_au8RawDataBuf[4],g_USB_au8RawDataBuf[5],g_USB_au8RawDataBuf[6],g_USB_au8RawDataBuf[7]);
    memset(pu8KeyStatusBuf, 0, GAMEPAD_KEY_MAX_NUM);

    if(pu8RawDataBuf[3] == 0x00)  //g_USB_au8RawDataBuf[3] = left,right
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_LEFT] = GAMEPAD_ISKEYPRESSED;
        //printf("left\n");
    }
    if(pu8RawDataBuf[3] == 0xFF)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_RIGHT] = GAMEPAD_ISKEYPRESSED;
        //printf("right\n");
    }
    if(pu8RawDataBuf[4] == 0x00)  //g_USB_au8RawDataBuf[3] = up,down
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_UP] = GAMEPAD_ISKEYPRESSED;
        //printf("up\n");
    }
    if(pu8RawDataBuf[4] == 0xFF)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_DOWN] = GAMEPAD_ISKEYPRESSED;
        //printf("down\n");
    }
    if(pu8RawDataBuf[5] & BIT4)  //g_USB_au8RawDataBuf[5] = button1,button2,button3,button4
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_0] = GAMEPAD_ISKEYPRESSED;
        //printf("button1\n");
    }
    if(pu8RawDataBuf[5] & BIT5)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_1] = GAMEPAD_ISKEYPRESSED;
        //printf("button2\n");
    }
    if(pu8RawDataBuf[5] & BIT6)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_2] = GAMEPAD_ISKEYPRESSED;
        //printf("button3\n");
    }
    if(pu8RawDataBuf[5] & BIT7)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_3] = GAMEPAD_ISKEYPRESSED;
        //printf("button4\n");
    }
    if(pu8RawDataBuf[6] & BIT4) //g_USB_au8RawDataBuf[6] = select,start,L1,L2,R1,R2
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_SEL] = GAMEPAD_ISKEYPRESSED;
        //printf("select \n");
    }
    if(pu8RawDataBuf[6] & BIT5)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_START] = GAMEPAD_ISKEYPRESSED;
        //printf("start \n");
    }
    if(pu8RawDataBuf[6] & BIT0)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_L1] = GAMEPAD_ISKEYPRESSED;
        //printf("L1\n");
    }
    if(pu8RawDataBuf[6] & BIT1)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_R1] = GAMEPAD_ISKEYPRESSED;
        //printf("R1\n");
    }
    if(pu8RawDataBuf[6] & BIT2)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_L2] = GAMEPAD_ISKEYPRESSED;
        //printf("L2\n");
    }
    if(pu8RawDataBuf[6] & BIT3)
    {
        pu8KeyStatusBuf[GAMEPAD_KEY_R2] = GAMEPAD_ISKEYPRESSED;
        //printf("R2\n");
    }
    return TRUE;
}

BOOLEAN devGamepad_ParseRawData_Port(U8 u8UsbPortIdx)
{
    BOOLEAN bRtn = TRUE;

    memset(g_Gamepad_au8KeyStatusBuf[u8UsbPortIdx], 0, GAMEPAD_KEY_MAX_NUM);

    switch( g_Gamepad_aeType[u8UsbPortIdx] )
    {
        case GAMEPAD_TYPE_DragonRise_PC_TWIN_SHOCK_Gamepad:
            bRtn = devGamepad_ParseRawData_DragonRise_SHOCK_Gamepad(g_USB_au8RawDataBuf[u8UsbPortIdx], g_Gamepad_au8KeyStatusBuf[u8UsbPortIdx]);
            break;

        case GAMEPAD_TYPE_DragonRise_Gamepad:
            bRtn = devGamepad_ParseRawData_DragonRise_Gamepad(g_USB_au8RawDataBuf[u8UsbPortIdx], g_Gamepad_au8KeyStatusBuf[u8UsbPortIdx]);
            break;

        case GAMEPAD_TYPE_Unknown:
        default:
            bRtn = devGamepad_ParseRawData_Default(g_USB_au8RawDataBuf[u8UsbPortIdx], g_Gamepad_au8KeyStatusBuf[u8UsbPortIdx]);
            break;
    }

#if(DEBUG_GET_RAW_TIME)
    if( g_Gamepad_au8KeyStatusBuf[GAMEPAD_KEY_START] == GAMEPAD_ISKEYPRESSED )
    {
        s_Gamepad_u8RawDataTimeIdx = 0;
    }
#endif

#if(DEBUG_GAMEPAD_KEY)
    static U8 s_Gamepad_au8KeyStatusBuf_Last[USB_PORT_NUM][GAMEPAD_KEY_MAX_NUM];//= {0};

    U8* pu8KeyStatusBuf_Cur = g_Gamepad_au8KeyStatusBuf[u8UsbPortIdx];
    U8* pu8KeyStatusBuf_Last = s_Gamepad_au8KeyStatusBuf_Last[u8UsbPortIdx];

    // If key status changed
    //if( memcmp(g_Gamepad_au8KeyStatusBuf, s_Gamepad_au8KeyStatusBuf_Last, GAMEPAD_KEY_MAX_NUM) )
    {
        if( pu8KeyStatusBuf_Cur[GAMEPAD_KEY_START] != pu8KeyStatusBuf_Last[GAMEPAD_KEY_START] )
        {
            printf("Key_START=%d\n", pu8KeyStatusBuf_Cur[GAMEPAD_KEY_START]);
        }
        if( pu8KeyStatusBuf_Cur[GAMEPAD_KEY_SEL] != pu8KeyStatusBuf_Last[GAMEPAD_KEY_SEL] )
        {
            printf("Key_SELECT=%d\n", pu8KeyStatusBuf_Cur[GAMEPAD_KEY_SEL]);
        }

        if( pu8KeyStatusBuf_Cur[GAMEPAD_KEY_UP] != pu8KeyStatusBuf_Last[GAMEPAD_KEY_UP] )
        {
            printf("Key_UP=%d\n", pu8KeyStatusBuf_Cur[GAMEPAD_KEY_UP]);
        }
        if( pu8KeyStatusBuf_Cur[GAMEPAD_KEY_DOWN] != pu8KeyStatusBuf_Last[GAMEPAD_KEY_DOWN] )
        {
            printf("Key_DOWN=%d\n", pu8KeyStatusBuf_Cur[GAMEPAD_KEY_DOWN]);
        }
        if( pu8KeyStatusBuf_Cur[GAMEPAD_KEY_LEFT] != pu8KeyStatusBuf_Last[GAMEPAD_KEY_LEFT] )
        {
            printf("Key_LEFT=%d\n", pu8KeyStatusBuf_Cur[GAMEPAD_KEY_LEFT]);
        }
        if( pu8KeyStatusBuf_Cur[GAMEPAD_KEY_RIGHT] != pu8KeyStatusBuf_Last[GAMEPAD_KEY_RIGHT] )
        {
            printf("Key_RIGHT=%d\n", pu8KeyStatusBuf_Cur[GAMEPAD_KEY_RIGHT]);
        }

        if( pu8KeyStatusBuf_Cur[GAMEPAD_KEY_0] != pu8KeyStatusBuf_Last[GAMEPAD_KEY_0] )
        {
            printf("Key_0=%d\n", pu8KeyStatusBuf_Cur[GAMEPAD_KEY_0]);
        }
        if( pu8KeyStatusBuf_Cur[GAMEPAD_KEY_1] != pu8KeyStatusBuf_Last[GAMEPAD_KEY_1] )
        {
            printf("Key_1=%d\n", pu8KeyStatusBuf_Cur[GAMEPAD_KEY_1]);
        }
    }

    memcpy(pu8KeyStatusBuf_Last, pu8KeyStatusBuf_Cur, GAMEPAD_KEY_MAX_NUM);
#endif

    return bRtn;
}

void devGamepad_Task(void)
{
    static U32 s_devGamepad_Task_u32LastTime;

    if( msAPI_Timer_DiffTime_2(s_devGamepad_Task_u32LastTime, MsOS_GetSystemTime()) < 20 )
    {
        //printf("[gamepad]time < 20ms\n");
        return;
    }

    s_devGamepad_Task_u32LastTime = MsOS_GetSystemTime();


    U8 u8UsbPortIdx = 0;

    for( u8UsbPortIdx = 0; u8UsbPortIdx < USB_PORT_NUM; u8UsbPortIdx += 1 )
    {
        // Check connection...
        // if not, change state to CHECK_CONNECT
        U8 u8CurConnectionStatus = devGamepad_USB_Get_ConnectionStatus_Port(u8UsbPortIdx);
        //printf("u8CurConnectionStatus=%X\n", u8CurConnectionStatus);

        if( u8CurConnectionStatus )
        {
            // NoConnect  ==> Connect
            if( g_GamePad_au8USBPortStatus[u8UsbPortIdx] == E_GAMEPAD_USB_PORT_NO_CONNECT )
            {
                printf("Port %d Device Connect~\n", u8UsbPortIdx);
            }
            g_GamePad_au8USBPortStatus[u8UsbPortIdx] = E_GAMEPAD_USB_PORT_CONNECTED;
        }
        else // No connect
        {
            // Connect ==> NoConnect
            if( g_GamePad_au8USBPortStatus[u8UsbPortIdx] != E_GAMEPAD_USB_PORT_NO_CONNECT )
            {
                printf("Port %d Device lost!\n", u8UsbPortIdx);
                g_Gamepad_aeState[u8UsbPortIdx] = E_GAMEPAD_STATE_CHECK_CONNECT;
            }
            g_GamePad_au8USBPortStatus[u8UsbPortIdx] = E_GAMEPAD_USB_PORT_NO_CONNECT;
        }


        switch( g_Gamepad_aeState[u8UsbPortIdx] )
        {
            default:
            case E_GAMEPAD_STATE_INIT:
            case E_GAMEPAD_STATE_CHECK_CONNECT:
                if( u8CurConnectionStatus )
                {
                    //g_GamePad_au8USBPortStatus[u8UsbPortIdx] = E_GAMEPAD_USB_PORT_CONNECTED;
                    g_Gamepad_aeState[u8UsbPortIdx] = E_GAMEPAD_STATE_CHECK_GAMEPAD;
                }
                else
                {
                    //g_GamePad_au8USBPortStatus[u8UsbPortIdx] = E_GAMEPAD_USB_PORT_NO_CONNECT;
                }
                break;

            case E_GAMEPAD_STATE_CHECK_GAMEPAD:
                printf("%u [E_GAMEPAD_STATE_CHECK_GAMEPAD]\n", u8UsbPortIdx);
                if( FALSE == devGamepad_CheckGamepadReady_Port(u8UsbPortIdx) )
                {
                    printf("\nError: Check gamepad fail!\n");
                    // todo
                    g_Gamepad_aeState[u8UsbPortIdx] = E_GAMEPAD_STATE_CHECK_CONNECT;
                }
                else
                {
                    printf("Gamepad ready~\n");
                    g_Gamepad_aeState[u8UsbPortIdx] = E_GAMEPAD_STATE_DEVICE_READY;
                }
                break;

            case E_GAMEPAD_STATE_DEVICE_READY:
                {
                    // Get rawdata from USB device
                    if( FALSE == devGamepad_USB_Get_RawData_Port(u8UsbPortIdx) )
                        break;

                    // Convert raw data to key status
                    if( FALSE == devGamepad_ParseRawData_Port(u8UsbPortIdx) )
                        break;

                }
                break;

        }

    }

}

U8 devGamepad_Get_FirstGamepadPort(void)
{
    U8 u8UsbPortIdx;

    for( u8UsbPortIdx = 0; u8UsbPortIdx < USB_PORT_NUM; u8UsbPortIdx += 1 )
    {
        if( g_Gamepad_aeState[u8UsbPortIdx] == E_GAMEPAD_STATE_DEVICE_READY )
        {
            return u8UsbPortIdx;
        }
    }

    return 0;
}

BOOLEAN devGamepad_Is_AnyKeyPressed(U8 u8UsbPortIdx)
{
    U8 i;

    if( u8UsbPortIdx >= USB_PORT_NUM )
        return FALSE;

    if( g_Gamepad_aeState[u8UsbPortIdx] != E_GAMEPAD_STATE_DEVICE_READY )
        return FALSE;

    for( i = 0; i < GAMEPAD_KEY_MAX_NUM; i += 1 )
    {
        if( g_Gamepad_au8KeyStatusBuf[u8UsbPortIdx][i] == GAMEPAD_ISKEYPRESSED )
            return TRUE;
    }

    return FALSE;
}

BOOLEAN devGamepad_Get_KeyStatus(U8 u8UsbPortIdx, U8* pu8KeyStatusBuf, U8 u8KeyBufSize)
{
    if( u8UsbPortIdx >= USB_PORT_NUM )
        return FALSE;

    if( g_Gamepad_aeState[u8UsbPortIdx] != E_GAMEPAD_STATE_DEVICE_READY )
        return FALSE;

    if( u8KeyBufSize > GAMEPAD_KEY_MAX_NUM )
        u8KeyBufSize = GAMEPAD_KEY_MAX_NUM;

    memcpy( pu8KeyStatusBuf, g_Gamepad_au8KeyStatusBuf[u8UsbPortIdx], u8KeyBufSize );

    U8 i;

    for( i = 0; i < GAMEPAD_KEY_MAX_NUM; i += 1 )
    {
        if( g_Gamepad_au8KeyStatusBuf[u8UsbPortIdx][i] == GAMEPAD_ISKEYPRESSED )
            return TRUE;
    }

    return FALSE;
}

void devGamepad_DeInit(void)
{
    PRINT_CURRENT_LINE();

#if( ENABLE_AEON_GAMEPAD )
    devGamepad_Aeon_DeInit();
#endif
}

#endif // ENABLE_GAMEPAD

