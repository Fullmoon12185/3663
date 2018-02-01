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

#ifndef _DRV_IIC_COMMON_H_
#define _DRV_IIC_COMMON_H_


#include "Board.h"


#ifndef I2C_USE_UTOPIA
 #if( CHIP_FAMILY_EULER == CHIP_FAMILY_TYPE )
    #define I2C_USE_UTOPIA  1
 #elif( defined(__aeon__) )
    #define I2C_USE_UTOPIA  0
 #else
    #define I2C_USE_UTOPIA  1
 #endif
#endif

//====================================================================================

#if( I2C_USE_UTOPIA == 0 )
#if (USE_SW_I2C_HIGHSPEED==1)
#define REG_OEN(pad)                CONCAT(GPIO_NUM(pad),_OEN)
#define REG_OUT(pad)                CONCAT(GPIO_NUM(pad),_OUT)
#define REG_IN(pad)                 CONCAT(GPIO_NUM(pad),_IN)
#define _I2C_PAD(pad)               REG_OEN(pad), REG_OUT(pad), REG_IN(pad)
#define _I2C_BUS(scl, sda, dly)     _I2C_PAD(scl), _I2C_PAD(sda), dly
#define I2C_BUS(bus)                _I2C_BUS(bus)
typedef struct {
    U32 SclOenReg; U8  SclOenBit;   U32 SclOutReg; U8  SclOutBit;   U32 SclInReg;U8  SclInBit;
    U32 SdaOenReg; U8  SdaOenBit;   U32 SdaOutReg; U8  SdaOutBit;   U32 SdaInReg;U8  SdaInBit;
    U8  defDelay;
} I2C_Bus;
#else
#define __I2C_BUS(scl, sda, dly)    scl, sda, dly
#define I2C_BUS( bus )      __I2C_BUS( bus )
typedef struct
{
    U16 padSCL;
    U16 padSDA;
    U16 defDelay;
} I2C_Bus;
#endif
#endif

//====================================================================================

void MDrv_IIC_Init(void);

#if( I2C_USE_UTOPIA )

#include "apiSWI2C.h"

#define MDrv_IIC_WriteBytes         MApi_SWI2C_WriteBytes
#define MDrv_IIC_ReadBytes          MApi_SWI2C_ReadBytes
#define MDrv_IIC_ReadByte           MApi_SWI2C_ReadByte
#define MDrv_IIC_WriteByte          MApi_SWI2C_WriteByte
#define MDrv_IIC_Write2Bytes        MApi_SWI2C_Write2Bytes
#define MDrv_IIC_Read2Bytes         MApi_SWI2C_Read2Bytes
#define MDrv_IIC_Write4Bytes        MApi_SWI2C_Write4Bytes
#define MDrv_IIC_ReadByteDirectly   MApi_SWI2C_ReadByteDirectly
#define MDrv_IIC_WriteGroupBytes    MApi_SWI2C_WriteGroupBytes
#define MDrv_IIC_ReadGroupBytes     MApi_SWI2C_ReadGroupBytes
#define MDrv_IIC_Speed_Setting      MApi_SWI2C_Speed_Setting
#define IIC_GetByte                 MApi_SWI2C_GetByte
#define IIC_SendByte                MApi_SWI2C_SendByte
#define IIC_AccessStart             MApi_SWI2C_AccessStart
#define IIC_Stop                    MApi_SWI2C_Stop

#else // I2C_USE_UTOPIA == 0

BOOLEAN MDrv_IIC_WriteBytes(U16 u16BusNumSlaveID, U8 u8addrcount, U8* pu8addr, U16 u16size, U8* pu8data);
BOOLEAN MDrv_IIC_ReadBytes(U16 u16BusNumSlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data);
BOOLEAN MDrv_IIC_ReadByte(U16 u16BusNumSlaveID, U8 u8RegAddr, U8 *pu8Data);
BOOLEAN MDrv_IIC_WriteByte(U16 u16BusNumSlaveID, U8 u8RegAddr, U8 u8Data);

BOOLEAN MDrv_IIC_Write2Bytes(U16 u16BusNumSlaveID, U8 u8addr, U16 u16data);
U16 MDrv_IIC_Read2Bytes(U16 u16BusNumSlaveID, U8 u8addr);
BOOLEAN MDrv_IIC_Write4Bytes(U16 u16BusNumSlaveID, U32 u32Data, U8 u8EndData);

BOOLEAN MDrv_IIC_ReadByteDirectly(U16 u16BusNumSlaveID, U8 * pu8Data);

BOOLEAN MDrv_IIC_WriteGroupBytes(U16 u16BusNumSlaveID, U8 u8SubGroup, U16 u16Addr, U16 u16Data);
U16 MDrv_IIC_ReadGroupBytes(U16 u16BusNumSlaveID, U8 u8SubGroup, U16 u16Addr);

U32 MDrv_IIC_Speed_Setting(U8 u8BusNum, U32 u32Speed_K);

U8 IIC_GetByte (U16  bAck);
BOOLEAN IIC_SendByte(U8 u8dat);
BOOLEAN IIC_AccessStart(U8 ucSlaveAdr, U8 trans_t);
void IIC_Stop(void);
void IIC_UseBus( U8 u8BusChn );

#endif // I2C_USE_UTOPIA

//====================================================================================

#endif // _DRV_IIC_COMMON_H_

