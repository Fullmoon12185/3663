/********************************************************************
* UserDefinedIIC.c
* User Defined IIC Function
* By yooheeyong 2010/02/20
*********************************************************************/

#include "UserDefinedIIC.h"
#include "CyAPIUser.h"

#include "lpt.h"


Ciic m_iic;

extern int EXTERN_LPT_USB;
extern BOOL bCheckUSB;


/* IIC initialization */
void I2c_Init(void)
{
  /* Semco IIC initialization */
    if(EXTERN_LPT_USB == SEL_LPT)
  {
    if(bCheckUSB == TRUE)
    {
      Cypress_USB_Close();
      bCheckUSB = FALSE;
    }

    #define LPTDELAY    100  /* 20100714 mod jhkwon 4000->100 */
    m_iic.I2cInit(LPTDELAY);
  }
  else
  {
    if(bCheckUSB == FALSE)
    {
      bCheckUSB = TRUE;
    }
    double   retdval;
    char     rettxtBuffer[256];
    char    *rettxt;
    rettxt = rettxtBuffer;

    Cypress_Configure("-i2c", "400khz", 0, &retdval, &rettxt);  /* at 400kHz SCL rate     */
  }
}

/* IIC Deinitialization */
void I2c_DeInit(void)
{
  if(bCheckUSB)
  {
    bCheckUSB = FALSE;
    Cypress_USB_Close();
  }
}

/* SEMCO LPT IIC Write */
int    SEMCO_LPT_WriteI2C (unsigned char ChipAddress, unsigned char IndexSize, unsigned int Index, unsigned char ucDataBufferSize, unsigned char* Data )
{
  long           NbData;
  NbData        = ucDataBufferSize;
  m_iic.Semco_I2c_WriteBytes(ChipAddress, IndexSize, Index, Data, NbData);
  return NbData;
}

/* SEMCO LPT IIC Read */
int    SEMCO_LPT_ReadI2C  (unsigned char ChipAddress, unsigned char IndexSize, unsigned int Index, unsigned char ucDataBufferSize, unsigned char* Data )
{
  long           NbData;
  NbData        = ucDataBufferSize;
  m_iic.Semco_I2c_ReadBytes (ChipAddress, IndexSize, Index , Data, NbData);
  return NbData;
}

/* TCS2 IIC Write */
int TCS2_I2cWrite(unsigned char Chip, unsigned char nRegAddrSize, unsigned int *lpRegAddr, unsigned short nDataSize, unsigned char *lpData)
{
	/* Semco IIC function */
	if(EXTERN_LPT_USB == SEL_LPT)
	{
		m_iic.Semco_I2c_WriteBytes(Chip,nRegAddrSize,(unsigned int)*lpRegAddr,lpData,(long)nDataSize);
	}
	else
	{
		Cypress_USB_WriteI2C (Chip>>1, (unsigned char)nRegAddrSize, (unsigned char *)lpRegAddr, nDataSize, lpData);
	}

	return 0;
}

/* TCS2 IIC Read */
int TCS2_I2cRead(unsigned char Chip, unsigned char nRegAddrSize, unsigned char *lpRegAddr, unsigned short nDataSize, unsigned char *lpData)
{
	unsigned char ReadData;

    /* Semco IIC function */
	if(EXTERN_LPT_USB == SEL_LPT)
	{
		m_iic.Semco_I2c_ReadBytes(Chip,nRegAddrSize,(unsigned int)*lpRegAddr,lpData,(long)nDataSize);
	}
	else
	{
		Cypress_USB_ReadI2C(Chip>>1, (unsigned char)nRegAddrSize, lpRegAddr, nDataSize, lpData);
	}

	return 0;
}

/* HummerH3 IIC Write */
unsigned short HummerH3_I2cWriteOneRegister(unsigned char ChipAddr, unsigned char RegAddr, unsigned short Data)
{
  unsigned char data[2];

  data[0] = (unsigned char)(Data>>8);
  data[1] = (unsigned char)(Data);

  /* Semco IIC function */
  if(EXTERN_LPT_USB == SEL_LPT)
  {
    m_iic.I2cWriteOneRegister_HummerH3(ChipAddr, RegAddr, Data);
  }
  else
  {
    Cypress_USB_WriteI2C (ChipAddr>>1, 1, &RegAddr, 2,data);
  }

  return 1;
}

/* HummerH3 IIC Read */
unsigned short HummerH3_I2cReadOneRegister(unsigned char ChipAddr, unsigned char RegAddr)
{
  unsigned short ReadData=0x00;

  unsigned char Data[2];

  /* Semco IIC function */
  if(EXTERN_LPT_USB == SEL_LPT)
  {
    ReadData = m_iic.I2cReadOneRegister_HummerH3(ChipAddr, RegAddr, 2);
  }
  else
  {
    Cypress_USB_ReadI2C(ChipAddr>>1, 1, &RegAddr, 2,Data);

    ReadData = (Data[0]<<8 | Data[1]);
  }

  return ReadData;
}


/* User Defined Delay */
void SemcoSleep(unsigned int nSleepTims_ms)
{
  Sleep(nSleepTims_ms);
}

// ECHO, FOX
unsigned short I2cWriteOneRegister(unsigned char ChipAddr, unsigned char RegAddr, unsigned short Data)
{
	I2C_RESULT status = I2C_OK;//usb purpose

#ifdef SEMCO_IIC
	unsigned char data[2]={0,};

	data[0] = (unsigned char)(Data>>8);
	data[1] = (unsigned char)Data;

	/* Semco IIC function */
	 if(EXTERN_LPT_USB == SEL_LPT)
	 {
		m_iic.Semco_I2c_WriteBytes(ChipAddr,1,(unsigned int)RegAddr,data,2);
	 }else{
	    //hUSB.USB_WriteSingleMultipleAddr(REG_ADDR_SINGLE,TDA18273_ADDRESS,1,pAddr,WriteLen,pData); //recommendation
//	    m_comm.ComIICWrite(ChipAddr<<1,1,&RegAddr,2,data);

		 //ChipAddr,RegAddr,DATA[0],DATA[1]
		status = (I2C_RESULT)Cypress_USB_WriteI2C(ChipAddr>>1, 1, &RegAddr, 2, data);
	 }
#endif

	return 0;
}

unsigned short I2cReadOneRegister(unsigned char ChipAddr, unsigned char RegAddr)
{
	I2C_RESULT status = I2C_OK;//usb purpose
	unsigned short data;
	unsigned char ReadData[2]={0,};

	data = 0x0000;

#ifdef SEMCO_IIC
    /* Semco IIC function */
	 if(EXTERN_LPT_USB == SEL_LPT)
	 {
		m_iic.Semco_I2c_ReadBytes((ChipAddr&0xFE),1,(unsigned int)RegAddr,ReadData,2);
	 }else{
//	    m_comm.ComIICRead(ChipAddr<<1,1,&RegAddr,2,ReadData);
		 status = (I2C_RESULT)Cypress_USB_ReadI2C((ChipAddr&0xFE)>>1, 1, &RegAddr, 2, ReadData);
	 }

	 data = ReadData[0];
	 data <<= 8;
	 data |= ReadData[1];

#endif

	 return data;
}

/* S5H1411 IIC Write */
unsigned short SH1411_I2cWriteOneRegister(unsigned char ChipAddr, unsigned char RegAddr, unsigned short Data)
{
	I2C_RESULT status = I2C_OK;//usb purpose
	
#ifdef SEMCO_IIC
	unsigned char data[2]={0,};
    if(EXTERN_LPT_USB == SEL_LPT)
	{
        /* Semco IIC function */
		m_iic.I2cWriteOneRegister_s5h1411(ChipAddr,RegAddr,Data);
		status = I2C_OK;
	}else{
		data[0] = (unsigned char)(Data>>8);
		data[1] = (unsigned char)Data;
		status = (I2C_RESULT)Cypress_USB_WriteI2C(ChipAddr>>1, 1, &RegAddr, 2, data);
	}
#endif

	return 1;
}

/* S5H1411 IIC Read */
unsigned short SH1411_I2cReadOneRegister(unsigned char ChipAddr, unsigned char RegAddr)
{
	I2C_RESULT status = I2C_OK;//usb purpose
	unsigned short ReadData=0x00;
	unsigned char Data[2]={0,};

#ifdef SEMCO_IIC
    if(EXTERN_LPT_USB == SEL_LPT)
	{
		/* Semco IIC function */
		ReadData = m_iic.I2cReadOneRegister_s5h1411(ChipAddr,RegAddr);
		status = I2C_OK;
	}else{
		status = (I2C_RESULT)Cypress_USB_ReadI2C((ChipAddr&0xFE)>>1, 1, &RegAddr, 2, Data);
		ReadData = Data[0];
		ReadData <<= 8;
		ReadData |= Data[1];
	}

#endif

	return ReadData;
}

/* SH1432 IIC Write One Register */
unsigned char S5H1432A_WRITE_ONE_REGISTER(unsigned char ChipAddr, unsigned char RegAddr, unsigned char Data)
{
	I2C_RESULT status = I2C_OK;//usb purpose
	unsigned char result=0;

#ifdef SEMCO_IIC
	if(EXTERN_LPT_USB == SEL_LPT)
	{
		m_iic.S5H1432A_WRITE_ONE_REGISTER(ChipAddr, RegAddr, Data);
		result = 0;
	}
	else
	{
		status = (I2C_RESULT)Cypress_USB_WriteI2C(ChipAddr>>1, 1, &RegAddr, 1, &Data);
		result = (unsigned char)status;
	}
    //m_iic.Semco_I2c_WriteBytes(ChipAddr,1,(unsigned int)RegAddr,&Data,1);
    //Cypress_USB_WriteI2C(ChipAddr>>1, 1, &RegAddr, 1, &Data);
#endif

	return result;
}

/* SH1432 IIC Read One Register */
unsigned char S5H1432A_READ_ONE_REGISTER(unsigned char ChipAddr, unsigned char RegAddr)
{ 
	I2C_RESULT status = I2C_OK;//usb purpose
	unsigned char ReadData;

#ifdef SEMCO_IIC
	if(EXTERN_LPT_USB == SEL_LPT)
	{
		ReadData = m_iic.S5H1432A_READ_ONE_REGISTER(ChipAddr, RegAddr);
	}
	else
	{
		status = (I2C_RESULT)Cypress_USB_ReadI2C(ChipAddr>>1, 1, &RegAddr,1,&ReadData);
	}
	//Cypress_USB_ReadI2C(ChipAddr>>1, 1, &RegAddr,1,&ReadData); 
    //m_iic.Semco_I2c_ReadBytes(ChipAddr,1,(unsigned int)RegAddr,&ReadData,1);

    //ReadData = m_iic.S5H1432A_READ_ONE_REGISTER(ChipAddr, RegAddr);

#endif

	return ReadData;
}

/* SH1432 IIC Read Two Register */
unsigned short S5H1432A_READ_TWO_REGISTER(unsigned char ChipAddr, unsigned char RegAddr, unsigned char *lpReadData)
{
	I2C_RESULT status = I2C_OK;//usb purpose
	unsigned short ReadData = 0x00;
	//unsigned char Data[2]={0,};

#ifdef SEMCO_IIC
	if(EXTERN_LPT_USB == SEL_LPT)
	{
		ReadData = m_iic.S5H1432A_READ_NBYTE_REGISTER(ChipAddr, RegAddr);
		//20130730,csm,add
		lpReadData[0] = ReadData>>8;
		lpReadData[1] = ReadData;
		//end
	}
	else
	{
		status = (I2C_RESULT)Cypress_USB_ReadI2C((ChipAddr&0xFE)>>1, 1, &RegAddr, 2, lpReadData);
		ReadData = lpReadData[0];
		ReadData <<= 8;
		ReadData |= lpReadData[1];
	}
	//Cypress_USB_ReadI2C(ChipAddr>>1, 1, &RegAddr,2,lpReadData); 
    //m_iic.Semco_I2c_ReadBytes(ChipAddr,1,(unsigned int)RegAddr,lpReadData,2);
	//ReadData = lpReadData[0];
	//ReadData <<= 8;
	//ReadData |= lpReadData[1];

	//ReadData = m_iic.S5H1432A_READ_NBYTE_REGISTER(ChipAddr, RegAddr);
	////20130730,csm,add
	//lpReadData[0] = ReadData>>8;
	//lpReadData[1] = ReadData;
	////end
#endif

	return ReadData;
}