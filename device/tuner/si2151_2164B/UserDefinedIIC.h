/********************************************************************
* UserDefinedIIC.h
* User Defined IIC Header
* By yooheeyong 2010/02/20
*********************************************************************/


#ifndef _USERDEFINEDIIC_H
#define _USERDEFINEDIIC_H


#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

// 인터페이스 
typedef enum{
  SEL_LPT = 0,
  SEL_USB,
  SEL_CYPRESS_USB
}SEL_INTERFACE;


#define READ_MODE    0
#define WRITE_MODE   1


/* IIC initialization */
void I2c_Init(void);
/* IIC Deinitialization */
void I2c_DeInit(void);

//#define CUSTOMER_ReadI2C  SEMCO_LPT_ReadI2C
//#define CUSTOMER_WriteI2C SEMCO_LPT_WriteI2C

/* SEMCO LPT IIC Write */
int    SEMCO_LPT_WriteI2C (unsigned char ChipAddress, unsigned char IndexSize, unsigned int Index, unsigned char ucDataBufferSize, unsigned char* Data );
/* SEMCO LPT IIC Read */
int    SEMCO_LPT_ReadI2C  (unsigned char ChipAddress, unsigned char IndexSize, unsigned int Index, unsigned char ucDataBufferSize, unsigned char* Data );

/* TCS2 IIC Write */
int TCS2_I2cWrite(unsigned char Chip, unsigned char nRegAddrSize, unsigned int *lpRegAddr, unsigned short nDataSize, unsigned char *lpData);
/* TCS2 IIC Read */
int TCS2_I2cRead(unsigned char Chip, unsigned char nRegAddrSize, unsigned char *lpRegAddr, unsigned short nDataSize, unsigned char *lpData);


/* HummerH3 IIC Write */
unsigned short HummerH3_I2cWriteOneRegister(unsigned char ChipAddr, unsigned char RegAddr,unsigned short Data);
/* HummerH3 IIC Read */
unsigned short HummerH3_I2cReadOneRegister (unsigned char ChipAddr, unsigned char RegAddr);

/* User Defined Sleep */
void SemcoSleep(unsigned int nSleepTims_ms);

//ECHO, FOX
unsigned short I2cWriteOneRegister(unsigned char ChipAddr, unsigned char RegAddr, unsigned short Data);
unsigned short I2cReadOneRegister(unsigned char ChipAddr, unsigned char RegAddr);

/* SH1411 IIC Write */
unsigned short SH1411_I2cWriteOneRegister(unsigned char ChipAddr, unsigned char RegAddr,unsigned short Data);
/* SH1411 IIC Read */
unsigned short SH1411_I2cReadOneRegister(unsigned char ChipAddr, unsigned char RegAddr);

/* SH1432 IIC Write One Register */
unsigned char S5H1432A_WRITE_ONE_REGISTER(unsigned char ChipAddr, unsigned char RegAddr, unsigned char Data);
/* SH1432 IIC Read One Register */
unsigned char S5H1432A_READ_ONE_REGISTER(unsigned char ChipAddr, unsigned char RegAddr);
/* SH1432 IIC Read Two Register */
unsigned short S5H1432A_READ_TWO_REGISTER(unsigned char ChipAddr, unsigned char RegAddr, unsigned char *lpReadData);

#if defined( __cplusplus )
}
#endif

#endif
