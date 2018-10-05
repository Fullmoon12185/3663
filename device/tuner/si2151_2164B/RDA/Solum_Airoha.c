/*************************************************************************************************************/
/*                     SiLabs Layer 1                  RF Tuner AV2018                                       */
/*-----------------------------------------------------------------------------------------------------------*/
/*   This source code contains all API Functions for the AV2018 Tuner on the RF board                        */
/*     All functions are declared in Silabs_L1_RF_AV2018_API.h                                              */
/*************************************************************************************************************/
/* Change log:

 As from SVN5948:
  <improvement>[single RXIQ out/RSSI] In L1_RF_AV2018_RSSI:
    Updated tables to match reg[3] settings to 0x54

 As from SVN5946:
  <improvement>[single RXIQ out] In L1_RF_AV2018_Tune:
    Using reg[3]=(char) (((Frac<<7)&0x80) | 0x54); since AV2018 is single ended

 As from SVN5712:
  <improvementt>[Airoha/VDAPPS-367] Comment received from Airoha:
   'Airoha would like to inform you that below register change may help you to have better improved CN performance.
   The variation of improvement will depend on your customized PCB and circuit design, test environment and different TPs.'
   reg[ 8]=(char) (0x3f); ( Updated from 0x0e to 0x3f )
   reg[29]=(char) (0xee); ( Updated from 0x02 to 0xee )
   changes in L1_RF_AV2018_RSSI tables to return correct RSSI due to the above changes

 As from SVN5294:
  <cleanup/blindscan> Removing auto_scan field from context, since this is not used with SiLabs demodulators.
    It has negative effects on SAT blindscan, which requires the BW to be maximum.
  <correction/RSSI> In L1_RF_AV2018_RSSI: returning rssi level based on AGC value, instead of returning internal gain.

 As from SVN5285: <correction/RSSI> In L1_RF_AV2018_RSSI: returning rssi (now int) based on measurements of AGC level. Unit is dBm.

 As from SVN5258: <correction/RSSI> In L1_RF_AV2018_RSSI: inverting test when finding segment, returning value divided by 1000.

 As from SVN5221: <correction>[SAT_blindscan] Keeping context->auto_scan always 0. Otherwise, the SAT spectrum is limited due to LPF limitation to 27MHz
 coming from the original Airoha driver.

 As from SVN4960:
  <improvement> [SW_Startup] In L1_RF_AV2018_Init: Checking i2c read result.
   If it fails, display an error trace, and try all possible addresses to see if one works.
   NB: This is for SW startup purpose. It is not intended to be used as an automatic i2c address detection, since
       it can have negative effects on the application.
      For instance, SiLabs TER tuners use the same i2c address range, and it could generate conflicts.
      If an incorrect i2c address is detected, it MUST be corrected in the SW configuration, and the code needs
       to be rebuilt with the correction.

 *************************************************************************************************************/
//#define   SiLEVEL          1
//#define   SiTAG            context->i2c->tag

#include "Semco_RDA.h"

#define Time_DELAY_MS      system_wait

#define Tuner_I2C_write( index, buffer, count)   L0_WriteBytes (context->i2c, index, count, buffer)
#define Tuner_I2C_read(  index, buffer, count)   L0_ReadBytes  (context->i2c, index, count, buffer)


int   L1_RF_AV2018_InitAfterReset (RDA_Context *context) {
  unsigned char *reg;
  unsigned char add;

  context->tuner_crystal = 24;
  reg = context->tuner_log;

  SiTRACE("L1_RF_AV2018_InitAfterReset starting...\n");
  /* Register initial flag. Static constant for first entry */
  /* At Power ON, tuner_initial = 0, will run sequence 1~3 at first call of "Tuner_control(). */
  /* Initial registers R0~R41 */
  /* [Important] Please notice that the default RF registers must be sent in this sequence:
     R0~R11, R13~R41, and then R12. R12 must be sent at last.  */
  if (1) {
  reg[ 0]=(char) (0x5A);
  reg[ 1]=(char) (0x95);
  reg[ 2]=(char) (0x55);
  reg[ 3]=(char) (0x50);
  reg[ 4]=(char) (0x1f);
  reg[ 5]=(char) (0xa3);
  reg[ 6]=(char) (0xfd);
  reg[ 7]=(char) (0x58);
  //reg[ 8]=(char) (0x3f); /* Updated from 0x0e to 0x3f (VDAPPS-367) */
  reg[ 8]=(char) (0x16); /* 20151111 mod PGA_3dB */
  reg[ 9]=(char) (0x80);
  reg[10]=(char) (0x88);
  reg[11]=(char) (0xb4);

  reg[12]=(char) (0xd6); /* RFLP=ON at Power on initial */

  reg[13]=(char) (0x40);
  reg[14]=(char) (0x94);
  reg[15]=(char) (0x4a);
  reg[16]=(char) (0x66);
  reg[17]=(char) (0x40);
  reg[18]=(char) (0x80);
  reg[19]=(char) (0x2b);
  reg[20]=(char) (0x6a);
  reg[21]=(char) (0x50);
  reg[22]=(char) (0x91);
  reg[23]=(char) (0x27);
  reg[24]=(char) (0x8f);
  reg[25]=(char) (0xcc);
  reg[26]=(char) (0x21);
  reg[27]=(char) (0x10);
  reg[28]=(char) (0x80);
  reg[29]=(char) (0xee); /* Updated from 0x02 to 0xee (VDAPPS-367) */
  reg[30]=(char) (0xf5);
  reg[31]=(char) (0x7f);
  reg[32]=(char) (0x4a);
  reg[33]=(char) (0x9b);
  reg[34]=(char) (0xe0);
  reg[35]=(char) (0xe0);
  reg[36]=(char) (0x36);
  reg[37]=(char) (0x00); /* Disable FT function at Power on initial */
  reg[38]=(char) (0xab);
  reg[39]=(char) (0x97);
  reg[40]=(char) (0xc5);
  reg[41]=(char) (0xa8);
  }

  /* Sequence 1 */
  /* Send Reg0 ->Reg11 */
  Tuner_I2C_write(0,reg,12);
  /* Sequence 2 */
  /* Send Reg13 ->Reg24 */
  Tuner_I2C_write(13,reg+13,12);
  /* Send Reg25 ->Reg35 */
  Tuner_I2C_write(25,reg+25,11);
  /* Send Reg36 ->Reg41 */
  Tuner_I2C_write(36,reg+36,6);

  /* Sequence 3 */
  /* send reg12 */
  Tuner_I2C_write(12,reg+12,1);


  	/* Time delay ms*/			
	Time_DELAY_MS(100);		
	/*Reinitial again*/
	{
		/* Sequence 1 */
		/* Send Reg0 ->Reg11 */
		Tuner_I2C_write(0,reg,12);
		/* Sequence 2 */
		/* Send Reg13 ->Reg24 */
		Tuner_I2C_write(13,reg+13,12);
		/* Send Reg25 ->Reg35 */
		Tuner_I2C_write(25,reg+25,11);
		/* Send Reg36 ->Reg41 */
		Tuner_I2C_write(36,reg+36,6);

		/* Sequence 3 */
		/* send reg12 */
		Tuner_I2C_write(12,reg+12,1);
	}


  /* Making sure the i2c address is correct */
  if (L0_ReadRegister(context->i2c, 12, 0, 8, 0) != reg[12]) {
    SiTRACE("AV2018 read  error! Check your i2c implementation and/or i2c address for the AV2018!! (currently 0x%02x)\n", context->i2c->address);
    SiERROR("AV2018 read  error! Check your i2c implementation and/or i2c address for the AV2018!!");
    for (add = 0xc0; add <= 0xc6; add = add +2) { /* try all possible i2c addresses for AV2018 */
      context->i2c->address = add;
      Tuner_I2C_write(12,reg+12,1);
      if (L0_ReadRegister(context->i2c, 12, 0, 8, 0) != reg[12]) {
        SiTRACE("AV2018 READ not working for i2c address 0x%02x...\n", add);
      } else {
        SiTRACE("AV2018 READ working for i2c address 0x%02x. The AV2018 i2c address can be 0x%02x.\n", add, add);
      }
    }
  }

  /* Time delay 5ms */
  Time_DELAY_MS(5);

  //L1_RF_AV2018_Tune (context, context->RF);

  SiTRACE("L1_RF_AV2018_InitAfterReset complete...\n");
  return 0;
}

int   L1_RF_AV2018_Wakeup         (RDA_Context *context) {
  L1_RF_AV2018_InitAfterReset (context);
  return 0;
}

int   L1_RF_AV2018_Standby        (RDA_Context *context) {
  AV2018_WRITE (context, pd_soft, pd_soft_power_down);
  return 0;
}

int   L1_RF_AV2018_ClockOn        (RDA_Context *context) {
  AV2018_WRITE (context, xocore_ena, 1);
  return 0;
}

int   L1_RF_AV2018_ClockOff       (RDA_Context *context) {
  AV2018_WRITE (context, xocore_ena, 0);
  return 0;
}

int   L1_RF_AV2018_LockStatus     (RDA_Context *context) {
  int ret = 0;

  ret = AV2018_READ(context, bwlf); // Filter bandwidth lock
  if(ret != 1)
	  return 0;
  ret = AV2018_READ(context, chlf); // Channel lock

  return ret;
}

int   L1_RF_AV2018_Get_IF         (RDA_Context *context) {
   return context->IF;}

int   L1_RF_AV2018_Get_RF         (RDA_Context *context) {
   return context->RF;}

int   L1_RF_AV2018_Get_minRF      (RDA_Context *context) {
   return context->minRF;}

int   L1_RF_AV2018_Get_maxRF      (RDA_Context *context) {
   return context->maxRF;}

int   L1_RF_AV2018_Tune           (RDA_Context *context, int channel_freq_kHz) {
  int Int;
  int Frac;
  int BW_kHz;
  unsigned long BF;
  unsigned char *reg;
  unsigned int ctrl_clk_khz;
  unsigned int BW_CLK_khz;	
  unsigned int XO_khz;
  unsigned int filter_khz;

  BW_kHz  = context->LPF;
  reg = context->tuner_log;

  SiTRACE("L1_RF_AV2018_Tune channel_freq_kHz %8d\n", channel_freq_kHz);

  Int  =  (channel_freq_kHz +      context->tuner_crystal*1000/2)     /(context->tuner_crystal*1000);
  Frac = ((channel_freq_kHz -  Int*context->tuner_crystal*1000  )<<17)/(context->tuner_crystal*1000);
  SiTRACE("L1_RF_AV2018_Tune Channel_freq_kHz %8d, Int %3d, Frac %d\n", channel_freq_kHz, Int, Frac);
  SiTRACE("L1_RF_AV2018_Tune Channel_freq_kHz %8d, Int %3d, Frac %d\n", channel_freq_kHz, Int, Frac);
  
  XO_khz = context->tuner_crystal*1000;
  if( XO_khz != 27000 )
  {		
		if(channel_freq_kHz > 1167750) 
           reg[16] = 0x56; // VCO Divider=/2
        else
           reg[16] = 0x46; // VCO Divider=/4               	       
        
        Tuner_I2C_write(16,reg+16,1);// Send Reg16
			
  } 
    
  Time_DELAY_MS(4); 

  reg[0]=(char) (Int & 0xff);
  reg[1]=(char) ((Frac>>9)&0xff);
  reg[2]=(char) ((Frac>>1)&0xff);

  /******************************************************
  reg[3]_D7 is Frac<0>, D6~D0 is 0x50, For differential RXIQ out
  reg[3]_D7 is Frac<0>, D6~D0 is 0x54, For single RXIQ out
  ******************************************************/
  //reg[3]=(char) (((Frac<<7)&0x80) | 0x54);
  reg[3]=(char) (((Frac<<7)&0x80) | 0x50);  // 20151112 mod ifferential RXIQ out

  /* BF = BW(MHz) * 1.27 / 211KHz */
  //BF = (BW_kHz*127 + 21100/2) / (21100);

	BF = BW_kHz*135/200+5000;
	//BF = (unsigned long)(BW_kHz*135/200+2000)*1.08;
	//BF = (unsigned long)(BW_kHz*135/200+2000)*108/100;
	SiTRACE("L1_RF_AV2018_Tune BF1 %8d\n", BF);

	//XO_khz = context->tuner_crystal*1000;
	filter_khz = BF;
	if( XO_khz < 25000 )       
        ctrl_clk_khz = XO_khz;
	else 
	    ctrl_clk_khz = XO_khz/2;	
	/* Filter Bandwidth reference CLK calculation */
	BW_CLK_khz = (ctrl_clk_khz +64/2)/64;
	/* BF = filter_BW (KHz) * 1.27 / BW_CLK (KHz)*/	
    BF = (filter_khz*127 + 100*BW_CLK_khz/2)/(100*BW_CLK_khz); 
	SiTRACE("L1_RF_AV2018_Tune BF2 %8d\n", BF);

  SiTRACE("L1_RF_AV2018_Tune Channel_freq_kHz %d LPF %d BW_kHz %d BF %8d\n", channel_freq_kHz, context->LPF, BW_kHz, BF);
  reg[5] = (unsigned char)BF;
  SiTRACE("L1_RF_AV2018_Tune reg[5] 0x%x\n", reg[5]);

  reg[4]=0x1f; 
  Time_DELAY_MS(4);

    /* Sequence 4 */
    /* Send Reg0 ->Reg4 */
    Tuner_I2C_write(0,reg,5);

    // --------------------------retune channel after 100ms on Nov.04.2013--Start-------------------------------
    /* Time delay 100ms */
    /* Send Reg0 ->Reg4 */
    Tuner_I2C_write(0,reg,5);
	//---------------------------retune channel after 100ms on Nov.04.2013--End---------------------------------

    /* Time delay 4ms */
    Time_DELAY_MS(4);

    /* Sequence 5 */
    /* Send Reg5 */
    Tuner_I2C_write(5, reg+5, 1);

	/* Time delay 4ms */
    Time_DELAY_MS(4);

    /* Fine-tune Function Control */
	
	/* Time delay 1ms */
    Time_DELAY_MS(1);
    /* Non-auto-scan mode. FT_block=1, FT_EN=1, FT_hold=0 */
    reg[37] = 0x06;
    Tuner_I2C_write(37, reg+37, 1);
	/* Time delay 15ms */
    Time_DELAY_MS(15);
    /* Fine-tune function is starting tracking after sending reg[37]. */

	reg[8] = (char) (0x16);
    Tuner_I2C_write(8, reg+8, 1);
	SiTRACE("L1_RF_AV2018_Tune reg[8] BB gain 0x%x\n", reg[8]);

    /* Make sure the RFAGC do not have a sharp jump. */
    /* Disable RFLP at Lock Channel sequence after reg[37] if not used */
    reg[12] = 0x96;
    Tuner_I2C_write(12, reg+12, 1);

    /* Time delay 5ms */
    Time_DELAY_MS(5);

  context->RF = (Int*context->tuner_crystal*1000) + ((Frac*context->tuner_crystal*1000)>>17);
  SiTRACE("L1_RF_AV2018_Tune context->RF %8d\n", context->RF);
  SiTRACE("L1_RF_AV2018_Tune context->RF - channel_freq_kHz %8d\n", context->RF - channel_freq_kHz);

  return context->RF;
}



