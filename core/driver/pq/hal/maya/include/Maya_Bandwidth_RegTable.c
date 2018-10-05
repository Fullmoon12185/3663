////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (; MStar; Confidential; Information; ) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//


//****************************************************
// DDR800MHz
//****************************************************
code U8 BWTABLE_COM[][REG_ADDR_SIZE+REG_BANK_SIZE+REG_MASK_SIZE+1] =
{//Reg          Bank  Mask  Value
 { REG(REG_TABLE_END), 0x00, 0x00 }
};

code U8 BWTABLE[][REG_ADDR_SIZE+REG_BANK_SIZE+REG_MASK_SIZE+BWTABLE_NUMS]=
{//Reg          Bank  Mask  Value
 { REG(0x2F40), 0x12, 0xFF, 0x18 /*SDi_mode*/, 0x18/*SDp_mode*/, 0x18/*HD_mode*/, 0x18/*DTV_HD*/, 0x18/*DTV_SD*/, 0x18/*HDMI_4K*/, 0x18/*MM_MOVIE_HD*/, 0x18/*MM_MOVIE_SD*/, 0x18/*MM_PHOTO*/, 0x10/*PC_HD*/, 0x18/*VD_COMB*/, },
 { REG(0x2F41), 0x12, 0xFF, 0x18 /*SDi_mode*/, 0x18/*SDp_mode*/, 0x18/*HD_mode*/, 0x18/*DTV_HD*/, 0x18/*DTV_SD*/, 0x18/*HDMI_4K*/, 0x18/*MM_MOVIE_HD*/, 0x18/*MM_MOVIE_SD*/, 0x18/*MM_PHOTO*/, 0x10/*PC_HD*/, 0x18/*VD_COMB*/, },
 { REG(0x2F42), 0x12, 0xFF, 0x18 /*SDi_mode*/, 0x18/*SDp_mode*/, 0x18/*HD_mode*/, 0x18/*DTV_HD*/, 0x18/*DTV_SD*/, 0x18/*HDMI_4K*/, 0x18/*MM_MOVIE_HD*/, 0x18/*MM_MOVIE_SD*/, 0x18/*MM_PHOTO*/, 0x10/*PC_HD*/, 0x18/*VD_COMB*/, },
 { REG(0x2F43), 0x12, 0xFF, 0x18 /*SDi_mode*/, 0x18/*SDp_mode*/, 0x18/*HD_mode*/, 0x18/*DTV_HD*/, 0x18/*DTV_SD*/, 0x18/*HDMI_4K*/, 0x18/*MM_MOVIE_HD*/, 0x18/*MM_MOVIE_SD*/, 0x18/*MM_PHOTO*/, 0x10/*PC_HD*/, 0x18/*VD_COMB*/, },
 { REG(0x2F44), 0x12, 0xFF, 0x28 /*SDi_mode*/, 0x28/*SDp_mode*/, 0x28/*HD_mode*/, 0x28/*DTV_HD*/, 0x28/*DTV_SD*/, 0x28/*HDMI_4K*/, 0x28/*MM_MOVIE_HD*/, 0x28/*MM_MOVIE_SD*/, 0x28/*MM_PHOTO*/, 0x38/*PC_HD*/, 0x28/*VD_COMB*/, },
 { REG(0x2F45), 0x12, 0xFF, 0x28 /*SDi_mode*/, 0x28/*SDp_mode*/, 0x28/*HD_mode*/, 0x28/*DTV_HD*/, 0x28/*DTV_SD*/, 0x28/*HDMI_4K*/, 0x28/*MM_MOVIE_HD*/, 0x28/*MM_MOVIE_SD*/, 0x28/*MM_PHOTO*/, 0x38/*PC_HD*/, 0x28/*VD_COMB*/, },
 { REG(0x2F46), 0x12, 0xFF, 0x28 /*SDi_mode*/, 0x28/*SDp_mode*/, 0x28/*HD_mode*/, 0x28/*DTV_HD*/, 0x28/*DTV_SD*/, 0x28/*HDMI_4K*/, 0x28/*MM_MOVIE_HD*/, 0x28/*MM_MOVIE_SD*/, 0x28/*MM_PHOTO*/, 0x20/*PC_HD*/, 0x28/*VD_COMB*/, },
 { REG(0x2F47), 0x12, 0xFF, 0x28 /*SDi_mode*/, 0x28/*SDp_mode*/, 0x28/*HD_mode*/, 0x28/*DTV_HD*/, 0x28/*DTV_SD*/, 0x28/*HDMI_4K*/, 0x28/*MM_MOVIE_HD*/, 0x28/*MM_MOVIE_SD*/, 0x28/*MM_PHOTO*/, 0x20/*PC_HD*/, 0x28/*VD_COMB*/, },
 { REG(0x2F48), 0x12, 0xFF, 0x38 /*SDi_mode*/, 0x38/*SDp_mode*/, 0x38/*HD_mode*/, 0x38/*DTV_HD*/, 0x38/*DTV_SD*/, 0x38/*HDMI_4K*/, 0x38/*MM_MOVIE_HD*/, 0x38/*MM_MOVIE_SD*/, 0x38/*MM_PHOTO*/, 0x38/*PC_HD*/, 0x38/*VD_COMB*/, },
 { REG(0x1F33), 0x00, 0x07, 0x04 /*SDi_mode*/, 0x04/*SDp_mode*/, 0x07/*HD_mode*/, 0x07/*DTV_HD*/, 0x07/*DTV_SD*/, 0x07/*HDMI_4K*/, 0x07/*MM_MOVIE_HD*/, 0x07/*MM_MOVIE_SD*/, 0x07/*MM_PHOTO*/, 0x07/*PC_HD*/, 0x04/*VD_COMB*/, },
 { REG(0x1F33), 0x00, 0xC0, 0xC0 /*SDi_mode*/, 0xC0/*SDp_mode*/, 0xC0/*HD_mode*/, 0xC0/*DTV_HD*/, 0xC0/*DTV_SD*/, 0xC0/*HDMI_4K*/, 0xC0/*MM_MOVIE_HD*/, 0xC0/*MM_MOVIE_SD*/, 0xC0/*MM_PHOTO*/, 0xC0/*PC_HD*/, 0xC0/*VD_COMB*/, },
 { REG(0x1F32), 0x00, 0xFF, 0x70 /*SDi_mode*/, 0x70/*SDp_mode*/, 0x70/*HD_mode*/, 0x70/*DTV_HD*/, 0x70/*DTV_SD*/, 0x70/*HDMI_4K*/, 0x70/*MM_MOVIE_HD*/, 0x70/*MM_MOVIE_SD*/, 0x70/*MM_PHOTO*/, 0x70/*PC_HD*/, 0x70/*VD_COMB*/, },
 { REG(0x1429), 0xFF, 0xFF, 0x24 /*$SDi_mode*/, 0x24/*$SDp_mode*/, 0x24/*$HD_mode*/, 0x24/*$DTV_HD*/, 0x24/*$DTV_SD*/, 0x24/*$HDMI_4K*/, 0x24/*$MM_MOVIE_HD*/, 0x24/*$MM_MOVIE_SD*/, 0x24/*$MM_PHOTO*/, 0x24/*$PC_HD*/, 0x24/*$VD_COMB*/, },
 { REG(0x1428), 0xFF, 0x1F, 0x08 /*SDi_mode*/, 0x08/*SDp_mode*/, 0x08/*HD_mode*/, 0x08/*DTV_HD*/, 0x08/*DTV_SD*/, 0x08/*HDMI_4K*/, 0x08/*MM_MOVIE_HD*/, 0x08/*MM_MOVIE_SD*/, 0x08/*MM_PHOTO*/, 0x08/*PC_HD*/, 0x08/*VD_COMB*/, },
 { REG(0x121F), 0xFF, 0xFF, 0x80 /*SDi_mode*/, 0x80/*SDp_mode*/, 0x80/*HD_mode*/, 0x80/*DTV_HD*/, 0x80/*DTV_SD*/, 0x80/*HDMI_4K*/, 0x80/*MM_MOVIE_HD*/, 0x80/*MM_MOVIE_SD*/, 0x80/*MM_PHOTO*/, 0x80/*PC_HD*/, 0x80/*VD_COMB*/, },
 { REG(0x1240), 0xFF, 0xFF, 0x15 /*SDi_mode*/, 0x15/*SDp_mode*/, 0x15/*HD_mode*/, 0x1D/*DTV_HD*/, 0x1D/*DTV_SD*/, 0x15/*HDMI_4K*/, 0x1D/*MM_MOVIE_HD*/, 0x1D/*MM_MOVIE_SD*/, 0x1D/*MM_PHOTO*/, 0x15/*PC_HD*/, 0x15/*VD_COMB*/, },
 { REG(0x1241), 0xFF, 0xFF, 0x80 /*SDi_mode*/, 0x80/*SDp_mode*/, 0x80/*HD_mode*/, 0x80/*DTV_HD*/, 0x80/*DTV_SD*/, 0x80/*HDMI_4K*/, 0x80/*MM_MOVIE_HD*/, 0x80/*MM_MOVIE_SD*/, 0x80/*MM_PHOTO*/, 0x80/*PC_HD*/, 0x80/*VD_COMB*/, },
 { REG(0x1242), 0xFF, 0xFF, 0x08 /*SDi_mode*/, 0x08/*SDp_mode*/, 0x08/*HD_mode*/, 0x08/*DTV_HD*/, 0x08/*DTV_SD*/, 0x08/*HDMI_4K*/, 0x08/*MM_MOVIE_HD*/, 0x08/*MM_MOVIE_SD*/, 0x08/*MM_PHOTO*/, 0x08/*PC_HD*/, 0x08/*VD_COMB*/, },
 { REG(0x1243), 0xFF, 0xFF, 0x20 /*SDi_mode*/, 0x20/*SDp_mode*/, 0x20/*HD_mode*/, 0x10/*DTV_HD*/, 0x10/*DTV_SD*/, 0x20/*HDMI_4K*/, 0x10/*MM_MOVIE_HD*/, 0x10/*MM_MOVIE_SD*/, 0x10/*MM_PHOTO*/, 0x20/*PC_HD*/, 0x20/*VD_COMB*/, },
 { REG(0x1244), 0xFF, 0xFF, 0x00 /*SDi_mode*/, 0x00/*SDp_mode*/, 0x00/*HD_mode*/, 0x00/*DTV_HD*/, 0x00/*DTV_SD*/, 0x00/*HDMI_4K*/, 0x00/*MM_MOVIE_HD*/, 0x00/*MM_MOVIE_SD*/, 0x00/*MM_PHOTO*/, 0x00/*PC_HD*/, 0x00/*VD_COMB*/, },
 { REG(0x1245), 0xFF, 0xFF, 0x04 /*SDi_mode*/, 0x04/*SDp_mode*/, 0x04/*HD_mode*/, 0x04/*DTV_HD*/, 0x04/*DTV_SD*/, 0x04/*HDMI_4K*/, 0x04/*MM_MOVIE_HD*/, 0x04/*MM_MOVIE_SD*/, 0x04/*MM_PHOTO*/, 0x04/*PC_HD*/, 0x04/*VD_COMB*/, },
 { REG(0x1248), 0xFF, 0xFF, 0xFF /*SDi_mode*/, 0xFF/*SDp_mode*/, 0xFF/*HD_mode*/, 0xFF/*DTV_HD*/, 0xFF/*DTV_SD*/, 0xFF/*HDMI_4K*/, 0xFF/*MM_MOVIE_HD*/, 0xFF/*MM_MOVIE_SD*/, 0xFF/*MM_PHOTO*/, 0xFF/*PC_HD*/, 0xFF/*VD_COMB*/, },
 { REG(0x1249), 0xFF, 0xFF, 0xFF /*SDi_mode*/, 0xFF/*SDp_mode*/, 0xFF/*HD_mode*/, 0xFF/*DTV_HD*/, 0xFF/*DTV_SD*/, 0xFF/*HDMI_4K*/, 0xFF/*MM_MOVIE_HD*/, 0xFF/*MM_MOVIE_SD*/, 0xFF/*MM_PHOTO*/, 0xFF/*PC_HD*/, 0xFF/*VD_COMB*/, },
 { REG(0x124A), 0xFF, 0xFF, 0x10 /*$SDi_mode*/, 0x10/*$SDp_mode*/, 0x10/*$HD_mode*/, 0x10/*$DTV_HD*/, 0x10/*$DTV_SD*/, 0x10/*$HDMI_4K*/, 0x10/*$MM_MOVIE_HD*/, 0x10/*$MM_MOVIE_SD*/, 0x10/*$MM_PHOTO*/, 0x10/*$PC_HD*/, 0x10/*$VD_COMB*/, },
 { REG(0x124B), 0xFF, 0xFF, 0x32 /*$SDi_mode*/, 0x32/*$SDp_mode*/, 0x32/*$HD_mode*/, 0x32/*$DTV_HD*/, 0x32/*$DTV_SD*/, 0x32/*$HDMI_4K*/, 0x32/*$MM_MOVIE_HD*/, 0x32/*$MM_MOVIE_SD*/, 0x32/*$MM_PHOTO*/, 0x32/*$PC_HD*/, 0x32/*$VD_COMB*/, },
 { REG(0x124C), 0xFF, 0xFF, 0x54 /*$SDi_mode*/, 0x54/*$SDp_mode*/, 0x54/*$HD_mode*/, 0x54/*$DTV_HD*/, 0x54/*$DTV_SD*/, 0x54/*$HDMI_4K*/, 0x54/*$MM_MOVIE_HD*/, 0x54/*$MM_MOVIE_SD*/, 0x54/*$MM_PHOTO*/, 0x54/*$PC_HD*/, 0x54/*$VD_COMB*/, },
 { REG(0x124D), 0xFF, 0xFF, 0x76 /*$SDi_mode*/, 0x76/*$SDp_mode*/, 0x76/*$HD_mode*/, 0x76/*$DTV_HD*/, 0x76/*$DTV_SD*/, 0x76/*$HDMI_4K*/, 0x76/*$MM_MOVIE_HD*/, 0x76/*$MM_MOVIE_SD*/, 0x76/*$MM_PHOTO*/, 0x76/*$PC_HD*/, 0x76/*$VD_COMB*/, },
 { REG(0x124E), 0xFF, 0xFF, 0x98 /*$SDi_mode*/, 0x98/*$SDp_mode*/, 0x98/*$HD_mode*/, 0x98/*$DTV_HD*/, 0x98/*$DTV_SD*/, 0x98/*$HDMI_4K*/, 0x98/*$MM_MOVIE_HD*/, 0x98/*$MM_MOVIE_SD*/, 0x98/*$MM_PHOTO*/, 0x98/*$PC_HD*/, 0x98/*$VD_COMB*/, },
 { REG(0x124F), 0xFF, 0xFF, 0xBA /*$SDi_mode*/, 0xBA/*$SDp_mode*/, 0xBA/*$HD_mode*/, 0xBA/*$DTV_HD*/, 0xBA/*$DTV_SD*/, 0xBA/*$HDMI_4K*/, 0xBA/*$MM_MOVIE_HD*/, 0xBA/*$MM_MOVIE_SD*/, 0xBA/*$MM_PHOTO*/, 0xBA/*$PC_HD*/, 0xBA/*$VD_COMB*/, },
 { REG(0x1250), 0xFF, 0xFF, 0xDC /*$SDi_mode*/, 0xDC/*$SDp_mode*/, 0xDC/*$HD_mode*/, 0xDC/*$DTV_HD*/, 0xDC/*$DTV_SD*/, 0xDC/*$HDMI_4K*/, 0xDC/*$MM_MOVIE_HD*/, 0xDC/*$MM_MOVIE_SD*/, 0xDC/*$MM_PHOTO*/, 0xDC/*$PC_HD*/, 0xDC/*$VD_COMB*/, },
 { REG(0x1251), 0xFF, 0xFF, 0xFE /*$SDi_mode*/, 0xFE/*$SDp_mode*/, 0xFE/*$HD_mode*/, 0xFE/*$DTV_HD*/, 0xFE/*$DTV_SD*/, 0xFE/*$HDMI_4K*/, 0xFE/*$MM_MOVIE_HD*/, 0xFE/*$MM_MOVIE_SD*/, 0xFE/*$MM_PHOTO*/, 0xFE/*$PC_HD*/, 0xFE/*$VD_COMB*/, },
 { REG(0x1260), 0xFF, 0xFF, 0x15 /*SDi_mode*/, 0x15/*SDp_mode*/, 0x15/*HD_mode*/, 0x15/*DTV_HD*/, 0x15/*DTV_SD*/, 0x15/*HDMI_4K*/, 0x15/*MM_MOVIE_HD*/, 0x15/*MM_MOVIE_SD*/, 0x15/*MM_PHOTO*/, 0x15/*PC_HD*/, 0x15/*VD_COMB*/, },
 { REG(0x1261), 0xFF, 0xFF, 0x80 /*SDi_mode*/, 0x80/*SDp_mode*/, 0x80/*HD_mode*/, 0x81/*DTV_HD*/, 0x81/*DTV_SD*/, 0x80/*HDMI_4K*/, 0x81/*MM_MOVIE_HD*/, 0x81/*MM_MOVIE_SD*/, 0x81/*MM_PHOTO*/, 0x80/*PC_HD*/, 0x80/*VD_COMB*/, },
 { REG(0x1262), 0xFF, 0xFF, 0x08 /*SDi_mode*/, 0x08/*SDp_mode*/, 0x08/*HD_mode*/, 0x08/*DTV_HD*/, 0x08/*DTV_SD*/, 0x08/*HDMI_4K*/, 0x08/*MM_MOVIE_HD*/, 0x08/*MM_MOVIE_SD*/, 0x08/*MM_PHOTO*/, 0x08/*PC_HD*/, 0x08/*VD_COMB*/, },
 { REG(0x1263), 0xFF, 0xFF, 0x20 /*SDi_mode*/, 0x20/*SDp_mode*/, 0x20/*HD_mode*/, 0x20/*DTV_HD*/, 0x20/*DTV_SD*/, 0x20/*HDMI_4K*/, 0x20/*MM_MOVIE_HD*/, 0x20/*MM_MOVIE_SD*/, 0x20/*MM_PHOTO*/, 0x20/*PC_HD*/, 0x20/*VD_COMB*/, },
 { REG(0x1264), 0xFF, 0xFF, 0x00 /*SDi_mode*/, 0x00/*SDp_mode*/, 0x00/*HD_mode*/, 0x00/*DTV_HD*/, 0x00/*DTV_SD*/, 0x00/*HDMI_4K*/, 0x00/*MM_MOVIE_HD*/, 0x00/*MM_MOVIE_SD*/, 0x00/*MM_PHOTO*/, 0x00/*PC_HD*/, 0x00/*VD_COMB*/, },
 { REG(0x1265), 0xFF, 0xFF, 0x04 /*SDi_mode*/, 0x04/*SDp_mode*/, 0x04/*HD_mode*/, 0x04/*DTV_HD*/, 0x04/*DTV_SD*/, 0x04/*HDMI_4K*/, 0x04/*MM_MOVIE_HD*/, 0x04/*MM_MOVIE_SD*/, 0x04/*MM_PHOTO*/, 0x04/*PC_HD*/, 0x04/*VD_COMB*/, },
 { REG(0x1268), 0xFF, 0xFF, 0xFF /*SDi_mode*/, 0xFF/*SDp_mode*/, 0xFF/*HD_mode*/, 0xF7/*DTV_HD*/, 0xF7/*DTV_SD*/, 0xFF/*HDMI_4K*/, 0xF7/*MM_MOVIE_HD*/, 0xF7/*MM_MOVIE_SD*/, 0xF7/*MM_PHOTO*/, 0xFF/*PC_HD*/, 0xFF/*VD_COMB*/, },
 { REG(0x1269), 0xFF, 0xFF, 0xFF /*SDi_mode*/, 0xFF/*SDp_mode*/, 0xFF/*HD_mode*/, 0xFF/*DTV_HD*/, 0xFF/*DTV_SD*/, 0xFF/*HDMI_4K*/, 0xFF/*MM_MOVIE_HD*/, 0xFF/*MM_MOVIE_SD*/, 0xFF/*MM_PHOTO*/, 0xFF/*PC_HD*/, 0xFF/*VD_COMB*/, },
 { REG(0x126A), 0xFF, 0xFF, 0x10 /*$SDi_mode*/, 0x10/*$SDp_mode*/, 0x10/*$HD_mode*/, 0x10/*$DTV_HD*/, 0x10/*$DTV_SD*/, 0x10/*$HDMI_4K*/, 0x10/*$MM_MOVIE_HD*/, 0x10/*$MM_MOVIE_SD*/, 0x10/*$MM_PHOTO*/, 0x10/*$PC_HD*/, 0x10/*$VD_COMB*/, },
 { REG(0x126B), 0xFF, 0xFF, 0x32 /*$SDi_mode*/, 0x32/*$SDp_mode*/, 0x32/*$HD_mode*/, 0x32/*$DTV_HD*/, 0x32/*$DTV_SD*/, 0x32/*$HDMI_4K*/, 0x32/*$MM_MOVIE_HD*/, 0x32/*$MM_MOVIE_SD*/, 0x32/*$MM_PHOTO*/, 0x32/*$PC_HD*/, 0x32/*$VD_COMB*/, },
 { REG(0x126C), 0xFF, 0xFF, 0x54 /*$SDi_mode*/, 0x54/*$SDp_mode*/, 0x54/*$HD_mode*/, 0x54/*$DTV_HD*/, 0x54/*$DTV_SD*/, 0x54/*$HDMI_4K*/, 0x54/*$MM_MOVIE_HD*/, 0x54/*$MM_MOVIE_SD*/, 0x54/*$MM_PHOTO*/, 0x54/*$PC_HD*/, 0x54/*$VD_COMB*/, },
 { REG(0x126D), 0xFF, 0xFF, 0x67 /*$SDi_mode*/, 0x67/*$SDp_mode*/, 0x67/*$HD_mode*/, 0x67/*$DTV_HD*/, 0x67/*$DTV_SD*/, 0x67/*$HDMI_4K*/, 0x67/*$MM_MOVIE_HD*/, 0x67/*$MM_MOVIE_SD*/, 0x67/*$MM_PHOTO*/, 0x67/*$PC_HD*/, 0x67/*$VD_COMB*/, },
 { REG(0x126E), 0xFF, 0xFF, 0x98 /*$SDi_mode*/, 0x98/*$SDp_mode*/, 0x98/*$HD_mode*/, 0x98/*$DTV_HD*/, 0x98/*$DTV_SD*/, 0x98/*$HDMI_4K*/, 0x98/*$MM_MOVIE_HD*/, 0x98/*$MM_MOVIE_SD*/, 0x98/*$MM_PHOTO*/, 0x98/*$PC_HD*/, 0x98/*$VD_COMB*/, },
 { REG(0x126F), 0xFF, 0xFF, 0xBA /*$SDi_mode*/, 0xBA/*$SDp_mode*/, 0xBA/*$HD_mode*/, 0xBA/*$DTV_HD*/, 0xBA/*$DTV_SD*/, 0xBA/*$HDMI_4K*/, 0xBA/*$MM_MOVIE_HD*/, 0xBA/*$MM_MOVIE_SD*/, 0xBA/*$MM_PHOTO*/, 0xBA/*$PC_HD*/, 0xBA/*$VD_COMB*/, },
 { REG(0x1270), 0xFF, 0xFF, 0xDC /*$SDi_mode*/, 0xDC/*$SDp_mode*/, 0xDC/*$HD_mode*/, 0xDC/*$DTV_HD*/, 0xDC/*$DTV_SD*/, 0xDC/*$HDMI_4K*/, 0xDC/*$MM_MOVIE_HD*/, 0xDC/*$MM_MOVIE_SD*/, 0xDC/*$MM_PHOTO*/, 0xDC/*$PC_HD*/, 0xDC/*$VD_COMB*/, },
 { REG(0x1271), 0xFF, 0xFF, 0xFE /*$SDi_mode*/, 0xFE/*$SDp_mode*/, 0xFE/*$HD_mode*/, 0xFE/*$DTV_HD*/, 0xFE/*$DTV_SD*/, 0xFE/*$HDMI_4K*/, 0xFE/*$MM_MOVIE_HD*/, 0xFE/*$MM_MOVIE_SD*/, 0xFE/*$MM_PHOTO*/, 0xFE/*$PC_HD*/, 0xFE/*$VD_COMB*/, },
 { REG(0x1274), 0xFF, 0xFF, 0x00 /*SDi_mode*/, 0x00/*SDp_mode*/, 0x00/*HD_mode*/, 0x11/*DTV_HD*/, 0x11/*DTV_SD*/, 0x00/*HDMI_4K*/, 0x11/*MM_MOVIE_HD*/, 0x11/*MM_MOVIE_SD*/, 0x11/*MM_PHOTO*/, 0x00/*PC_HD*/, 0x00/*VD_COMB*/, },
 { REG(0x1275), 0xFF, 0xFF, 0x00 /*SDi_mode*/, 0x00/*SDp_mode*/, 0x00/*HD_mode*/, 0x80/*DTV_HD*/, 0x80/*DTV_SD*/, 0x00/*HDMI_4K*/, 0x80/*MM_MOVIE_HD*/, 0x80/*MM_MOVIE_SD*/, 0x80/*MM_PHOTO*/, 0x00/*PC_HD*/, 0x00/*VD_COMB*/, },
 { REG(0x127C), 0xFF, 0xFF, 0x00 /*SDi_mode*/, 0x00/*SDp_mode*/, 0x00/*HD_mode*/, 0x08/*DTV_HD*/, 0x08/*DTV_SD*/, 0x00/*HDMI_4K*/, 0x08/*MM_MOVIE_HD*/, 0x08/*MM_MOVIE_SD*/, 0x08/*MM_PHOTO*/, 0x00/*PC_HD*/, 0x00/*VD_COMB*/, },
 { REG(0x1280), 0xFF, 0xFF, 0x15 /*SDi_mode*/, 0x15/*SDp_mode*/, 0x15/*HD_mode*/, 0x15/*DTV_HD*/, 0x15/*DTV_SD*/, 0x15/*HDMI_4K*/, 0x15/*MM_MOVIE_HD*/, 0x15/*MM_MOVIE_SD*/, 0x15/*MM_PHOTO*/, 0x15/*PC_HD*/, 0x15/*VD_COMB*/, },
 { REG(0x1281), 0xFF, 0xFF, 0x80 /*SDi_mode*/, 0x80/*SDp_mode*/, 0x80/*HD_mode*/, 0x80/*DTV_HD*/, 0x80/*DTV_SD*/, 0x80/*HDMI_4K*/, 0x80/*MM_MOVIE_HD*/, 0x80/*MM_MOVIE_SD*/, 0x80/*MM_PHOTO*/, 0x80/*PC_HD*/, 0x80/*VD_COMB*/, },
 { REG(0x1282), 0xFF, 0xFF, 0x08 /*SDi_mode*/, 0x08/*SDp_mode*/, 0x08/*HD_mode*/, 0x08/*DTV_HD*/, 0x08/*DTV_SD*/, 0x08/*HDMI_4K*/, 0x08/*MM_MOVIE_HD*/, 0x08/*MM_MOVIE_SD*/, 0x08/*MM_PHOTO*/, 0x08/*PC_HD*/, 0x08/*VD_COMB*/, },
 { REG(0x1283), 0xFF, 0xFF, 0x20 /*SDi_mode*/, 0x20/*SDp_mode*/, 0x20/*HD_mode*/, 0x20/*DTV_HD*/, 0x20/*DTV_SD*/, 0x20/*HDMI_4K*/, 0x20/*MM_MOVIE_HD*/, 0x20/*MM_MOVIE_SD*/, 0x20/*MM_PHOTO*/, 0x20/*PC_HD*/, 0x20/*VD_COMB*/, },
 { REG(0x1284), 0xFF, 0xFF, 0x00 /*SDi_mode*/, 0x00/*SDp_mode*/, 0x00/*HD_mode*/, 0x00/*DTV_HD*/, 0x00/*DTV_SD*/, 0x00/*HDMI_4K*/, 0x00/*MM_MOVIE_HD*/, 0x00/*MM_MOVIE_SD*/, 0x00/*MM_PHOTO*/, 0x00/*PC_HD*/, 0x00/*VD_COMB*/, },
 { REG(0x1285), 0xFF, 0xFF, 0x04 /*SDi_mode*/, 0x04/*SDp_mode*/, 0x04/*HD_mode*/, 0x04/*DTV_HD*/, 0x04/*DTV_SD*/, 0x04/*HDMI_4K*/, 0x04/*MM_MOVIE_HD*/, 0x04/*MM_MOVIE_SD*/, 0x04/*MM_PHOTO*/, 0x04/*PC_HD*/, 0x04/*VD_COMB*/, },
 { REG(0x1288), 0xFF, 0xFF, 0xFF /*SDi_mode*/, 0xFF/*SDp_mode*/, 0xFF/*HD_mode*/, 0xFF/*DTV_HD*/, 0xFF/*DTV_SD*/, 0xFF/*HDMI_4K*/, 0xFF/*MM_MOVIE_HD*/, 0xFF/*MM_MOVIE_SD*/, 0xFF/*MM_PHOTO*/, 0xFF/*PC_HD*/, 0xFF/*VD_COMB*/, },
 { REG(0x1289), 0xFF, 0xFF, 0xFF /*SDi_mode*/, 0xFF/*SDp_mode*/, 0xFF/*HD_mode*/, 0xFF/*DTV_HD*/, 0xFF/*DTV_SD*/, 0xFF/*HDMI_4K*/, 0xFF/*MM_MOVIE_HD*/, 0xFF/*MM_MOVIE_SD*/, 0xFF/*MM_PHOTO*/, 0xFF/*PC_HD*/, 0xFF/*VD_COMB*/, },
 { REG(0x128A), 0xFF, 0xFF, 0x10 /*$SDi_mode*/, 0x10/*$SDp_mode*/, 0x10/*$HD_mode*/, 0x10/*$DTV_HD*/, 0x10/*$DTV_SD*/, 0x10/*$HDMI_4K*/, 0x10/*$MM_MOVIE_HD*/, 0x10/*$MM_MOVIE_SD*/, 0x10/*$MM_PHOTO*/, 0x10/*$PC_HD*/, 0x10/*$VD_COMB*/, },
 { REG(0x128B), 0xFF, 0xFF, 0x32 /*$SDi_mode*/, 0x32/*$SDp_mode*/, 0x32/*$HD_mode*/, 0x32/*$DTV_HD*/, 0x32/*$DTV_SD*/, 0x32/*$HDMI_4K*/, 0x32/*$MM_MOVIE_HD*/, 0x32/*$MM_MOVIE_SD*/, 0x32/*$MM_PHOTO*/, 0x32/*$PC_HD*/, 0x32/*$VD_COMB*/, },
 { REG(0x128C), 0xFF, 0xFF, 0x54 /*$SDi_mode*/, 0x54/*$SDp_mode*/, 0x54/*$HD_mode*/, 0x54/*$DTV_HD*/, 0x54/*$DTV_SD*/, 0x54/*$HDMI_4K*/, 0x54/*$MM_MOVIE_HD*/, 0x54/*$MM_MOVIE_SD*/, 0x54/*$MM_PHOTO*/, 0x54/*$PC_HD*/, 0x54/*$VD_COMB*/, },
 { REG(0x128D), 0xFF, 0xFF, 0x76 /*$SDi_mode*/, 0x76/*$SDp_mode*/, 0x76/*$HD_mode*/, 0x76/*$DTV_HD*/, 0x76/*$DTV_SD*/, 0x76/*$HDMI_4K*/, 0x76/*$MM_MOVIE_HD*/, 0x76/*$MM_MOVIE_SD*/, 0x76/*$MM_PHOTO*/, 0x76/*$PC_HD*/, 0x76/*$VD_COMB*/, },
 { REG(0x128E), 0xFF, 0xFF, 0x98 /*$SDi_mode*/, 0x98/*$SDp_mode*/, 0x98/*$HD_mode*/, 0x98/*$DTV_HD*/, 0x98/*$DTV_SD*/, 0x98/*$HDMI_4K*/, 0x98/*$MM_MOVIE_HD*/, 0x98/*$MM_MOVIE_SD*/, 0x98/*$MM_PHOTO*/, 0x98/*$PC_HD*/, 0x98/*$VD_COMB*/, },
 { REG(0x128F), 0xFF, 0xFF, 0xBA /*$SDi_mode*/, 0xBA/*$SDp_mode*/, 0xBA/*$HD_mode*/, 0xBA/*$DTV_HD*/, 0xBA/*$DTV_SD*/, 0xBA/*$HDMI_4K*/, 0xBA/*$MM_MOVIE_HD*/, 0xBA/*$MM_MOVIE_SD*/, 0xBA/*$MM_PHOTO*/, 0xBA/*$PC_HD*/, 0xBA/*$VD_COMB*/, },
 { REG(0x1290), 0xFF, 0xFF, 0xDC /*$SDi_mode*/, 0xDC/*$SDp_mode*/, 0xDC/*$HD_mode*/, 0xDC/*$DTV_HD*/, 0xDC/*$DTV_SD*/, 0xDC/*$HDMI_4K*/, 0xDC/*$MM_MOVIE_HD*/, 0xDC/*$MM_MOVIE_SD*/, 0xDC/*$MM_PHOTO*/, 0xDC/*$PC_HD*/, 0xDC/*$VD_COMB*/, },
 { REG(0x1291), 0xFF, 0xFF, 0xFE /*$SDi_mode*/, 0xFE/*$SDp_mode*/, 0xFE/*$HD_mode*/, 0xFE/*$DTV_HD*/, 0xFE/*$DTV_SD*/, 0xFE/*$HDMI_4K*/, 0xFE/*$MM_MOVIE_HD*/, 0xFE/*$MM_MOVIE_SD*/, 0xFE/*$MM_PHOTO*/, 0xFE/*$PC_HD*/, 0xFE/*$VD_COMB*/, },
 { REG(0x12DC), 0xFF, 0xFF, 0xE1 /*$SDi_mode*/, 0xE1/*$SDp_mode*/, 0xE1/*$HD_mode*/, 0xE1/*$DTV_HD*/, 0xE1/*$DTV_SD*/, 0xE1/*$HDMI_4K*/, 0xE1/*$MM_MOVIE_HD*/, 0xE1/*$MM_MOVIE_SD*/, 0xE1/*$MM_PHOTO*/, 0xE1/*$PC_HD*/, 0xE1/*$VD_COMB*/, },
 { REG(0x12DD), 0xFF, 0x01, 0x01 /*SDi_mode*/, 0x01/*SDp_mode*/, 0x01/*HD_mode*/, 0x01/*DTV_HD*/, 0x01/*DTV_SD*/, 0x01/*HDMI_4K*/, 0x01/*MM_MOVIE_HD*/, 0x01/*MM_MOVIE_SD*/, 0x01/*MM_PHOTO*/, 0x01/*PC_HD*/, 0x01/*VD_COMB*/, },
 { REG(0x12DD), 0xFF, 0x01, 0x00 /*SDi_mode*/, 0x00/*SDp_mode*/, 0x00/*HD_mode*/, 0x00/*DTV_HD*/, 0x00/*DTV_SD*/, 0x00/*HDMI_4K*/, 0x00/*MM_MOVIE_HD*/, 0x00/*MM_MOVIE_SD*/, 0x00/*MM_PHOTO*/, 0x00/*PC_HD*/, 0x00/*VD_COMB*/, },
 { REG(0x12FF), 0xFF, 0xFF, 0x00 /*SDi_mode*/, 0x00/*SDp_mode*/, 0x00/*HD_mode*/, 0x01/*DTV_HD*/, 0x01/*DTV_SD*/, 0x00/*HDMI_4K*/, 0x01/*MM_MOVIE_HD*/, 0x01/*MM_MOVIE_SD*/, 0x01/*MM_PHOTO*/, 0x00/*PC_HD*/, 0x00/*VD_COMB*/, },
 { REG(0x1508), 0x16, 0xFF, 0x1C /*SDi_mode*/, 0x1C/*SDp_mode*/, 0x1C/*HD_mode*/, 0x1C/*DTV_HD*/, 0x1C/*DTV_SD*/, 0x1C/*HDMI_4K*/, 0x1C/*MM_MOVIE_HD*/, 0x1C/*MM_MOVIE_SD*/, 0x1C/*MM_PHOTO*/, 0x1C/*PC_HD*/, 0x1C/*VD_COMB*/, },
 { REG(0x1500), 0x16, 0xFF, 0x15 /*SDi_mode*/, 0x15/*SDp_mode*/, 0x15/*HD_mode*/, 0x15/*DTV_HD*/, 0x15/*DTV_SD*/, 0x15/*HDMI_4K*/, 0x15/*MM_MOVIE_HD*/, 0x15/*MM_MOVIE_SD*/, 0x15/*MM_PHOTO*/, 0x15/*PC_HD*/, 0x15/*VD_COMB*/, },
 { REG(0x1501), 0x16, 0xFF, 0x80 /*SDi_mode*/, 0x80/*SDp_mode*/, 0x80/*HD_mode*/, 0x80/*DTV_HD*/, 0x80/*DTV_SD*/, 0x80/*HDMI_4K*/, 0x80/*MM_MOVIE_HD*/, 0x80/*MM_MOVIE_SD*/, 0x80/*MM_PHOTO*/, 0x80/*PC_HD*/, 0x80/*VD_COMB*/, },
 { REG(0x151C), 0x16, 0xFF, 0xE3 /*SDi_mode*/, 0xE3/*SDp_mode*/, 0xE3/*HD_mode*/, 0xE3/*DTV_HD*/, 0xE3/*DTV_SD*/, 0xE3/*HDMI_4K*/, 0xE3/*MM_MOVIE_HD*/, 0xE3/*MM_MOVIE_SD*/, 0xE3/*MM_PHOTO*/, 0xE3/*PC_HD*/, 0xE3/*VD_COMB*/, },
 { REG(0x15E0), 0x16, 0xFF, 0x02 /*SDi_mode*/, 0x02/*SDp_mode*/, 0x02/*HD_mode*/, 0x02/*DTV_HD*/, 0x02/*DTV_SD*/, 0x02/*HDMI_4K*/, 0x02/*MM_MOVIE_HD*/, 0x02/*MM_MOVIE_SD*/, 0x02/*MM_PHOTO*/, 0x02/*PC_HD*/, 0x02/*VD_COMB*/, },
 { REG(0x15E2), 0x16, 0xFF, 0x04 /*SDi_mode*/, 0x04/*SDp_mode*/, 0x04/*HD_mode*/, 0x04/*DTV_HD*/, 0x04/*DTV_SD*/, 0x04/*HDMI_4K*/, 0x04/*MM_MOVIE_HD*/, 0x04/*MM_MOVIE_SD*/, 0x04/*MM_PHOTO*/, 0x04/*PC_HD*/, 0x04/*VD_COMB*/, },
 { REG(0x15EB), 0x16, 0xFF, 0x01 /*SDi_mode*/, 0x01/*SDp_mode*/, 0x01/*HD_mode*/, 0x01/*DTV_HD*/, 0x01/*DTV_SD*/, 0x01/*HDMI_4K*/, 0x01/*MM_MOVIE_HD*/, 0x01/*MM_MOVIE_SD*/, 0x01/*MM_PHOTO*/, 0x01/*PC_HD*/, 0x01/*VD_COMB*/, },
 { REG(REG_TABLE_END), 0x00, 0x00 }
};


