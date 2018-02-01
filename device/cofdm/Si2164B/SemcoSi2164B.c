#ifdef __NO_LINUX__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <math.h>
#endif

#include "SemcoSi2164B.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Check Never Lock */
/* Return : Never lock (1) */
unsigned char SiDemod_Check_NeverLock(L1_Si2164B_Context *demod)
{
  unsigned char return_code=0;

  return_code = return_code;     /* to avoid compiler warning when not used */

  /* DVB-T/T2 auto detect seek loop, using Si2164B_L1_DD_STATUS                                          */
  /* if DL LOCKED                            : demod is locked on a dd_status->modulation signal        */
  /* if DL NO_LOCK and rsqstat_bit5 NO_CHANGE : demod is searching for a DVB-T/T2 signal                 */
  /* if DL NO_LOCK and rsqstat_bit5 CHANGE    : demod says this is not a DVB-T/T2 signal (= 'neverlock') */

  return_code = Si2164B_L1_DD_STATUS(demod, Si2164B_DD_STATUS_CMD_INTACK_CLEAR);

  if ( demod->rsp->dd_status.dl  == Si2164B_DD_STATUS_RESPONSE_DL_LOCKED   ) {
    /* Return 0 to signal that the Si2164B is locked on a valid DVB-T/T2 channel */
    return 0;
  } 
  else {
	//if ( demod->rsp->dd_status.rsqint_bit5 == Si2164B_DD_STATUS_RESPONSE_RSQINT_BIT5_CHANGED ) {
   if ( demod->rsp->dd_status.rsqstat_bit5 == Si2164B_DD_STATUS_RESPONSE_RSQSTAT_BIT5_CHANGE ) {
      /* Return 1 if firmware signals 'no DVB-T/T2 channel' */
      SiTRACE ("'no DVB-T/T2 channel': will never lock\n");
      return 1;
    }
  }
  return 0;
}

/* Get DVB-C Demod lock in range */
signed char SiDemod_L1_DVB_C_get_dl_lock_in_range(L1_Si2164B_Context *demod, signed long range_kBps)
{
  signed int return_code;
  unsigned long correctedSR;
  unsigned int ui_range;
  signed long long_temp1,long_temp2;
  
  return_code = Si2164B_L1_DVBC_STATUS(demod, Si2164B_DVBC_STATUS_CMD_INTACK_CLEAR);

  if (return_code != NO_Si2164B_ERROR) {
    SiTRACE("Si2164B_L1_DVB_C_get_Demod_lock_in_range: Si2164B_L1_DVBC_STATUS error, aborting\n");
    return 0;
  }

  ui_range = (unsigned int)range_kBps; 
  if ( (demod->rsp->dvbc_status.dl  == Si2164B_DVBC_STATUS_RESPONSE_DL_LOCKED   ) & ( demod->rsp->dvbc_status.pcl == Si2164B_DVBC_STATUS_RESPONSE_PCL_LOCKED ) ) {

       long_temp1=1000*demod->prop->dvbc_symbol_rate.rate;
       long_temp2=demod->prop->dvbc_symbol_rate.rate*demod->rsp->dvbc_status.timing_offset;	  
       long_temp2/=1000;
       correctedSR = long_temp1-long_temp2;
	   
    return(signed char) (((unsigned long)(1000*demod->prop->dvbc_symbol_rate.rate - ui_range) <= correctedSR) && (correctedSR <= (unsigned long)(1000*demod->prop->dvbc_symbol_rate.rate + ui_range)));
  } else {
    return 0;
  }
}

/* Get DVB-S/S2 lock in AFC range */
signed char SiDemod_L1_DVB_S_S2_get_lock_in_AFC_range(L1_Si2164B_Context *demod)
{
  signed int return_code=0;
  signed int afc_range_khz=0;
  signed int afc_freq_khz=0;

  return_code = Si2164B_L1_DD_STATUS(demod, Si2164B_DD_STATUS_CMD_INTACK_CLEAR);
  if (return_code != NO_Si2164B_ERROR) {
    SiTRACE("Si2164B_L1_DVB_S_S2_get_lock_in_AFC_range: Si2164B_L1_DD_STATUS error\n");
    SiERROR("Si2164B_L1_DVB_S_S2_get_lock_in_AFC_range: Si2164B_L1_DD_STATUS error\n");
    return 0;
  }
  if (demod->rsp->dd_status.dl != Si2164B_DD_STATUS_RESPONSE_DL_LOCKED) {
    /* Return 0 to signal that the SiDemod is not locked */
    return 0;
  }
  /* Retrieving status and setting afc_range_khz for current modulation */
  if (demod->rsp->dd_status.modulation == Si2164B_DD_MODE_PROP_MODULATION_DVBS2) {
    if (Si2164B_L1_DVBS2_STATUS    (demod, Si2164B_DVBS2_STATUS_CMD_INTACK_OK) != NO_Si2164B_ERROR) {
      SiTRACE("Si2164B_L1_DVB_S_S2_get_lock_in_AFC_range: Si2164B_L1_DVBS2_STATUS error\n");
      SiERROR("Si2164B_L1_DVB_S_S2_get_lock_in_AFC_range: Si2164B_L1_DVBS2_STATUS error\n");
      return 0;
    }
    afc_freq_khz  = demod->rsp->dvbs2_status.afc_freq;

    /* In DVBS2       mode, the AFC range limit is always in dvbs2_afc_range.range_khz */
    afc_range_khz = demod->prop->dvbs2_afc_range.range_khz;
  }
  if (demod->rsp->dd_status.modulation == Si2164B_DD_MODE_PROP_MODULATION_DVBS ) {
    if (Si2164B_L1_DVBS_STATUS     (demod, Si2164B_DVBS_STATUS_CMD_INTACK_OK) != NO_Si2164B_ERROR) {
      SiTRACE("Si2164B_L1_DVB_S_S2_get_lock_in_AFC_range: Si2164B_L1_DVBS_STATUS error\n");
      SiERROR("Si2164B_L1_DVB_S_S2_get_lock_in_AFC_range: Si2164B_L1_DVBS_STATUS error\n");
      return 0;
    }
    afc_freq_khz = demod->rsp->dvbs_status.afc_freq;

    if ( demod->prop->dd_mode.modulation == Si2164B_DD_MODE_PROP_MODULATION_AUTO_DETECT) {
      /* In AUTO_DETECT mode, the AFC range limit is set in dvbs2_afc_range.range_khz */
      afc_range_khz  = demod->prop->dvbs2_afc_range.range_khz;
    } else {
      /* In DVBS        mode, the AFC range limit is set in dvbs_afc_range.range_khz */
      afc_range_khz  = demod->prop->dvbs_afc_range.range_khz;
    }
  }
  SiTRACE("Si2164B_L1_DVB_S_S2_get_lock_in_AFC_range: AFC freq of %d\n", afc_freq_khz);

  if ( afc_freq_khz < 0) afc_freq_khz = -afc_freq_khz;

    if ( afc_freq_khz > (afc_range_khz*105/100) ) {
      SiTRACE("Si2164B_L1_DVB_S_S2_get_lock_in_AFC_range: above AFC range of %d\n", afc_range_khz);
      return 0;
    }

    SiTRACE("Si2164B_L1_DVB_S_S2_get_lock_in_AFC_range: Locked within AFC range of %d (+5%%)\n", afc_range_khz);
    return 1;
}


/* DVB-C SSI */
signed int SiDemod_DVB_C_SSI_for_Received_Power(L1_Si2164B_Context *demod, signed int  Prec)
{
  signed int Prel;
   signed int   ssi;
   unsigned char constel;
   signed int return_code;

    signed int  Pref[5] = {-84, -81, -78, -75, -72};
    signed int signed_temp;

	return_code = Si2164B_L1_DVBC_STATUS(demod, Si2164B_DVBC_STATUS_CMD_INTACK_CLEAR);

    if (return_code != NO_Si2164B_ERROR) {
      SiTRACE("SiDemod_DVB_C_SSI_for_Received_Power: Si2164B_L1_DVBC_STATUS error, aborting\n");
      return 0;
    }

    switch (demod->rsp->dvbc_status.constellation) {
      case Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM16 : constel = 0; break;
      case Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM32 : constel = 1; break;
      case Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM64 : constel = 2; break;
      case Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM128: constel = 3; break;
      case Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM256: constel = 4; break;
      default:                     constel = 0; break;
    }

    Prel = Prec - Pref[constel];

     if (Prel < -10) {
      ssi = 0;
    } else if (Prel < 0)  {
      signed_temp=((Prel*10+100)*5/6);//mul 10
      signed_temp=signed_temp/10;// divide 10
      ssi = signed_temp;
    } else if (Prel < 15) {
      signed_temp=Prel*53+100; // mul 10
      signed_temp=signed_temp/10; // divide 10
      ssi = signed_temp;
    } else if (Prel < 27) {
     signed_temp=((Prel*10-150)*5/6+900);// mul 10
      signed_temp=signed_temp/10;// divide 10
      ssi = signed_temp;
    } else                {
      ssi = 100;
    }

    return ssi;
}

/* DVB-C SQI */
signed int SiDemod_DVB_C_SQI(L1_Si2164B_Context *demod)
{
    signed int   sqi = 0;

 unsigned int CNrel;
    unsigned int CNrec;
//unsigned int unsigned_temp;
//signed int int_temp;

    unsigned char constel;
    signed int return_code;

   unsigned int Pref[5] = {165, 195, 225, 250, 280}; // mul 10

    return_code = Si2164B_L1_DVBC_STATUS(demod, Si2164B_DVBC_STATUS_CMD_INTACK_CLEAR);

    if (return_code != NO_Si2164B_ERROR) {
      SiTRACE("Si2164B_DVB_C_SQI: Si2164B_L1_DVBC_STATUS error, aborting\n");
      return 0;
    }

    switch (demod->rsp->dvbc_status.constellation) {
      case Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM16 : constel = 0; break;
      case Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM32 : constel = 1; break;
      case Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM64 : constel = 2; break;
      case Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM128: constel = 3; break;
      case Si2164B_DVBC_CONSTELLATION_PROP_CONSTELLATION_QAM256: constel = 4; break;
      default:                     constel = 0; break;
    }

 CNrec = (demod->rsp->dvbc_status.cnr*10)/4; //mul 10
    CNrel = Pref[constel]; //mul 10

    if (CNrec >= (CNrel + 60)) {
      sqi = 100;
    } else if (CNrec < CNrel)  {
      sqi = 0;
    } else if ((CNrel <= CNrec) && (CNrec < (CNrel + 60))) {
      sqi = (signed int)((CNrec-CNrel)/6*10);//div 10 
    }


    return sqi;
}


/* T2CS2_DUAL DVB-S/S2 SSI */
signed int SiDemod_DVB_SAT_SSI_T2S2_DUAL(L1_Si2164B_Context *demod, signed int rssi)
{
	signed int s_ssi = 0;

    demod=demod;/*avoid compile warning*/

	s_ssi = 235 * rssi + 13817;

	if(s_ssi > 10000)
	{
		s_ssi = 10000;
	}
	else if(s_ssi < 0)
	{
		s_ssi = 0;
	}

	if(s_ssi != 0)
	{
		s_ssi /= 100;
	}

	return s_ssi;
}

/* T2CS2_Single NIM DVB-S/S2 SSI */
signed int SiDemod_DVB_SAT_SSI_T2CS2_SINGLE(L1_Si2164B_Context *demod, signed int rssi)
{
	signed int s_ssi = 0;

    demod=demod;/*avoid compile warning*/
    
	s_ssi = (rssi * 260 + 16408);

	if(s_ssi > 10000)
	{
		s_ssi = 10000;
	}
	else if(s_ssi < 0)
	{
		s_ssi = 0;
	}

	if(s_ssi != 0)
	{
		s_ssi /= 100;
	}

	return s_ssi;
}

/* DVB-S SQI */
signed int SiDemod_DVB_S_SQI(L1_Si2164B_Context *demod)
{
	signed int   sqi = 0;
	unsigned int  CNrel;
	unsigned char c_rate=0;
	unsigned int  cn;

	unsigned int  Pref[5] = {425, 550, 625, 650, 675};// mul 100 

	switch(demod->rsp->dvbs_status.code_rate)
	{
		case Si2164B_DVBS_STATUS_RESPONSE_CODE_RATE_1_2:  c_rate = 0; break;
		case Si2164B_DVBS_STATUS_RESPONSE_CODE_RATE_2_3:  c_rate = 1; break;
		case Si2164B_DVBS_STATUS_RESPONSE_CODE_RATE_3_4:  c_rate = 2; break;
		case Si2164B_DVBS_STATUS_RESPONSE_CODE_RATE_5_6:  c_rate = 3; break;
		case Si2164B_DVBS_STATUS_RESPONSE_CODE_RATE_7_8:  c_rate = 4; break;
		default:                                         c_rate = 2; break;
	}

	cn = demod->rsp->dvbs_status.cnr*100/4;// mul 100 s

	if(cn > 1325)
	{
		cn += 25;
	}

	CNrel = Pref[c_rate]; // mul 100

	if (cn >= (CNrel + 1000)) {
		sqi = 100;
	} else if (cn < CNrel)  {
		sqi = 0;
	} else if ((CNrel <= cn) && (cn < (CNrel + 1000))) {
		sqi = (signed int)((cn-CNrel)/10);// div 100 
	}

	return sqi;
}
/* DVB-S2 SQI */
signed int SiDemod_DVB_S2_SQI(L1_Si2164B_Context *demod)
{
    signed int   sqi = 0;
    unsigned int  CNrel;
    unsigned char s2_const, s2_rate=0;
    unsigned int  cn;

	switch (demod->rsp->dvbs2_status.constellation)
	{
		case Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_QPSK:  s2_const = 0; break;
		case Si2164B_DVBS2_STATUS_RESPONSE_CONSTELLATION_8PSK:  s2_const = 1; break;
		default:                     s2_const = 0; break;
	}

	if(s2_const == 0)
	{
		unsigned int  Pref_qpsk[8] = {0, 12, 24, 37, 42, 48, 59, 60};  // mul 10

		switch(demod->rsp->dvbs_status.code_rate)
		{
			case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_1_2:  s2_rate = 0; break;
			case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_3_5:  s2_rate = 1; break;
			case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_2_3:  s2_rate = 2; break;
			case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_3_4:  s2_rate = 3; break;
			case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_4_5:  s2_rate = 4; break;
			case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_5_6:  s2_rate = 5; break;
			case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_8_9:  s2_rate = 6; break;
			case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_9_10: s2_rate = 7; break;
			default:                                          s2_rate = 3; break;
		}

		cn = demod->rsp->dvbs2_status.cnr*10/4;// mul 10 

		CNrel = Pref_qpsk[s2_rate];// mul 10

		if (cn >= (CNrel + 60)) {
			sqi = 100;
		} else if (cn < CNrel)  {
			sqi = 0;
		} else if ((CNrel <= cn) && (cn < (CNrel + 60))) {
			sqi = (signed int)((cn-CNrel)/6*10); // div 10
		}
	}
	else if(s2_const == 1)
	{
		unsigned int   Pref_8psk[6] = {53, 63, 76, 91, 106, 108};// mul 10

		switch (demod->rsp->dvbs2_status.code_rate)
		{
			case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_3_5:  s2_rate = 0; break;
			case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_2_3:  s2_rate = 1; break;
			case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_3_4:  s2_rate = 2; break;
			case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_5_6:  s2_rate = 3; break;
			case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_8_9:  s2_rate = 4; break;
			case Si2164B_DVBS2_STATUS_RESPONSE_CODE_RATE_9_10: s2_rate = 5; break;
			default:                                          s2_rate = 3; break;
		}

		cn = demod->rsp->dvbs2_status.cnr*10/4;// mul 10 

		CNrel = Pref_8psk[s2_rate];

		if (cn >= (CNrel + 60)) {
			sqi = 100;
		} else if (cn < CNrel)  {
			sqi = 0;
		} else if ((CNrel <= cn) && (cn < (CNrel + 60))) {
			sqi = (signed int)((cn-CNrel)/6*10); //div 10 
		}
	}

    return sqi;
}
/* DVB-S2 PER */
signed int SiDemod_DVB_S2_PER(L1_Si2164B_Context *demod)
{
  signed int s2_sqi = 0;
  signed int per = 0;

  s2_sqi = SiDemod_DVB_S2_SQI(demod);

  per = (signed int)((100-s2_sqi)*21);

  if(per >= 2000)
  {
    per = 2000;
  }

  return per;
}

#ifdef __cplusplus
}
#endif

