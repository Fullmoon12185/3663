#ifndef _L0_API_H_
#define _L0_API_H_
/*
****************************************************************************************
 Change log:

 As from V3.4.5
 Adding STRING_APPEND_SAFE to replace snprintf, since this function behaves differently
   between platforms when called with snprintf(msg, 1000, "%s%d", msg, value);
   Under Windows: the text for value is APPENDED to msg
   Under Linux:   the text for value is REPLACING the original msg.

 As from V3.3.9:
  Changing comment related to using #define SiTRACES_FEATURES     SiTRACES_MINIMAL
    WARNING : the minimal features mode disables tracing functions in the extern file

****************************************************************************************/

#define SEC_T2_TO_CHECK 1
#define SEC_T2_TO_CHECK_01 1
#define SEC_T2_TO_CHECK_02 1    //for SAT tuner compile error


/* Un-comment the line below to compile for non-windows platforms (such as Linux) */
#ifdef NO_LINUX
#else
#define NO_WIN32
#endif

#ifdef    NO_WIN32
  #include <stdlib.h>
  #include <stdio.h>
  #include <time.h>
  #include <string.h>
  #include <stdarg.h>
#ifndef   NO_MATH_H_APPLICATION
  #include <math.h>
#endif /* NO_MATH_H_APPLICATION */
#else  /* NO_WIN32 not defined: the platform is Windows-based */
  #include <windows.h>
  #include <time.h>
  #include <stdio.h>
/* <porting> Comment the line below to avoid using the Cypress chip for USB communication */
  #define USB_Capability
#endif /* NO_WIN32 */





//20131120,prevent tool
#ifdef   NO_LINUX
  /*#define  SiTRACES*/
  #define CUSTOM_PRINTF safe_printf
  #define safe_printf   printf
  #define safe_sprintf(s,...)  _snprintf((char*)s,1000,__VA_ARGS__)
  #define safe_strcat(msg,txt)   strncat((char*)msg,txt,1000)
  #define safe_strtok   strtok
  #define SiERROR               L0_StoreError
#else  /* NO_LINUX */
  #define CHECK_FOR_ERRORS  if (L0_ErrorMessage()) safe_printf("\n\n**************\n%s**************\n\n\n", L0_error_message);
  /* Uncomment the following line to activate all traces in the code */
  /*#define  SiTRACES*/
  /* <porting> Replace  CUSTOM_PRINTF with your printf  function. (CCDebug::Printf ? )*/
  #define CUSTOM_PRINTF safe_printf
  /* <porting> Replace  safe_printf   with your printf  function. (CCDebug::Printf ? )*/
  #define safe_printf   printf
  /* <porting> Replace  safe_sprintf  with your sprintf function. (snprintf ?)        */
  #define safe_sprintf(s,...)              snprintf((char*)s,1000,__VA_ARGS__)
  /* <porting> Replace  safe_strcat   with your strcat  function. (strncat ?)         */
  #define safe_strcat(msg,txt)             strncat((char*)msg,(char*)txt,1000)
  /* <porting> Replace  safe_strtok   with your strtok  function. (strtok_r ?)        */
  #define safe_strtok(str,delim,saveptr)   strtok_r((char*)str,(char*)delim,saveptr)
  /* <porting> Replace  SiERROR with your error management function.            */
  #define SiERROR               safe_printf
  //#define CHECK_FOR_ERRORS      /* empty (not thread-safe) */
#endif /* NO_LINUX */

#define ERROR_MESSAGE_MAX_LENGH 1000
#define SILABS_TAG_SIZE           20

#ifdef    SiTRACES
  /* Porting: Select which of the following lines will allow you to display the function names */
  /* Porting: __FUNCTION__ is defined by GCC                                                   */
  /* Porting: __func__     is defined by C99 (it is not defined for VisualStudio 6)            */
  /* Porting: ""           will not display the function names, but will work on all platforms */
  #define SiTRACE(...)       printf(__VA_ARGS__) //SiTraceFunction(__FILE__, __LINE__, __FUNCTION__ ,__VA_ARGS__)
  /*  #define SiTRACE(...)        SiTraceFunction(__FILE__, __LINE__, __func__     ,__VA_ARGS__) */
  /*#define SiTRACE(...)        SiTraceFunction(__FILE__, __LINE__, ""           ,__VA_ARGS__)*/
  /*#define SiTRACE(...) */       /* nothing */

  /* Replace 'SiTRACES_FULL' by 'SiTRACES_MINIMAL' in the following line to de-activate full features mode */
  /* WARNING : the minimal features mode disables tracing functions in stdout and the extern file          */
  #define SiTRACES_FULL                  1
  #define SiTRACES_MINIMAL               0
  #define SiTRACES_FEATURES     SiTRACES_FULL
#else
  #define SiTRACE(...)               printf(__VA_ARGS__) /* empty */
  #define SiTraceConfiguration(...)  /* empty */
#endif /* SiTRACES */

#define TRACES_PRINTF                SiTRACE
#define ALLOCATION_ERROR_MESSAGE     SiTRACE
#define TREAT_ERROR_MESSAGE          SiTRACE
#define TRACES_ERR                   SiTRACE
#define TRACES_TRACE                 SiTRACE
#define TRACES_SHOW                  SiTRACE
#define TRACES_USE                   SiTRACE

#ifdef RWTRACES
/*    #warning "register-level traces activated (RWTRACES defined)" */
  #define L1_READ(ptr,  register)     L0_ReadRegisterTrace  (ptr->i2c, #register,     register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_SIGNED)
  #define L1_WRITE(ptr, register, v ) L0_WriteRegisterTrace (ptr->i2c, #register, #v, register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_ALONE, v)
#else
  #define L1_READ(ptr,  register)     L0_ReadRegister       (ptr->i2c,                register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_SIGNED)
  #define L1_WRITE(ptr, register, v ) L0_WriteRegister      (ptr->i2c,                register##_ADDRESS, register##_OFFSET, register##_NBBIT, register##_ALONE, v)
#endif

typedef enum _CONNECTION_TYPE
{
    SIMU = 0,
    USB,
    CUSTOMER,
    none
}  CONNECTION_TYPE;

typedef struct _L0_Context
{
    unsigned char   address;
    signed int             indexSize;
    CONNECTION_TYPE connectionType;
    signed int             trackWrite;
    signed int             trackRead;
    signed int             mustReadWithoutStop;
    unsigned char   writeBuffer[100];// KERNEL
    unsigned char            tag[SILABS_TAG_SIZE];
} L0_Context;

/* Layer 1 core types definitions */
typedef enum _ErrorCode
{
    Error_CODE_BASE                 = 2000,
    Error_INCORRECT_NB_READ,       /* 2001 */
    Error_INCORRECT_NB_WRITTEN,    /* 2002 */
    Error_DLL_LOAD,                /* 2003 */
    Error_ENUM_VAL_UNKNOWN,        /* 2004 */
    Error_COULD_NOT_WRITE_ITEM,    /* 2005 */
    Error_REGISTER_TYPE_UNKNOWN,   /* 2006 */
    Error_ENUM_OUT_OF_RANGE,       /* 2007 */
    Error_TYPE_IS_NOT_ENUMERATE,   /* 2008 */
    Error_VALUE_NOT_IN_ENUM_LIST,  /* 2009 */
    Error_COULD_NOT_UPDATE_ITEM,   /* 2010 */
    Error_VALUE_OUT_OF_RANGE,      /* 2011 */
    Error_UNKNOW_REGISTER,         /* 2012 */
    Error_READ_TRACES_ERROR,       /* 2013 */
    Error_WRITE_TRACES_ERROR,      /* 2014 */
    Error_UNKNOWN_COMMAND,         /* 2015 */
    Error_BUFFER_DOES_NOT_CONTAIN_REQUIRED_DATA         /* 2016 */
} ErrorCode;

typedef struct L1_Context {
  L0_Context *i2c;
  void       *specific;
} L1_Context;

#define MAX_PROPS   60
#define PROPS_LENTH  9
#define MAX_CMDS     7
#define CMDS_LENTH  17
#define DEMOD_FW_LINE_LENGTH 17

#ifdef __cplusplus
extern "C" {
#endif

    #ifdef    SiTRACES
    void   SiTraceDefaultConfiguration(void);
    void   SiTraceFunction     (const char *name, signed int trace_linenumber, const char *func, const char *fmt, ...);
    unsigned char * SiTraceConfiguration(const unsigned char *config);
    #else
    #define SiTraceDefaultConfiguration(...) /* empty */
    #endif /* SiTRACES */

extern    unsigned char   *L0_error_message;

    signed int     system_wait    (signed int time_ms);
    signed int     system_time    (void);

    signed int     strcmp_nocase  (const unsigned char* str1, const unsigned char* str2);
    signed int     strcmp_nocase_n(const unsigned char* str1, const unsigned char* str2, unsigned int n);
    signed int     L0_StoreError  (const unsigned char *message);
    signed int     L0_ErrorMessage(void);

    void    L0_Init              (L0_Context* i2c); /* initialize the Layer 0 DLL. It sets the layer 0 context parameters to startup values. It must be called first. It is automatically called by the Layer 1 init function.*/
    signed int     L0_Connect           (L0_Context* i2c, CONNECTION_TYPE connType); /* Select the connection mode */
	const unsigned char*   L0_InterfaceType     (L0_Context* i2c);
    signed int     L0_SetAddress        (L0_Context* i2c, unsigned int add, signed int addSize); /* set the IP's i2c address. It must be called only once at startup, as the IP's address is not expected to change over time.*/
    signed int     L0_ReadBytes         (L0_Context* i2c, unsigned int iI2CIndex, signed int iNbBytes, unsigned char *pbtDataBuffer) ;/* read  iNbBytes bytes from the IP. A buffer large enough to hold the values needs to be reserved by the upper layer.*/
    signed int     L0_WriteBytes        (L0_Context* i2c, unsigned int iI2CIndex, signed int iNbBytes, unsigned char *pbtDataBuffer); /* write iNbBytes bytes in the IP. The referenced buffer contains the bytes to be written.*/
    void    L0_TrackRead         (L0_Context* i2c, unsigned int track); /* toggle the read traces. Useful for debug purposes, mostly to control that data read in the IP is properly translated to the above layers.*/
    void    L0_TrackWrite        (L0_Context* i2c, unsigned int track); /* toggle the write traces at the lower possible layer. Useful for debug purposes, to check the i2C bus data sent to the IP.*/

    signed int     L0_ReadRawBytes      (L0_Context* i2c, unsigned int iI2CIndex, signed int iNbBytes, unsigned char *pbtDataBuffer);
    signed int     L0_WriteRawBytes     (L0_Context* i2c, unsigned int iI2CIndex, signed int iNbBytes, unsigned char *pbtDataBuffer);

    signed int     L0_ReadCommandBytes  (L0_Context* i2c, signed int iNbBytes, unsigned char *pucDataBuffer);
    signed int     L0_WriteCommandBytes (L0_Context* i2c, signed int iNbBytes, unsigned char *pucDataBuffer);

   signed long    L0_ReadRegister      (L0_Context *i2c,                           unsigned int iI2CIndex, unsigned char offset, unsigned char nbbit, unsigned int isSigned);
   signed long    L0_ReadRegisterTrace (L0_Context *i2c, unsigned char* name,               unsigned int iI2CIndex, unsigned char offset, unsigned char nbbit, unsigned int isSigned);
  signed  long    L0_WriteRegister     (L0_Context *i2c,                           unsigned int iI2CIndex, unsigned char offset, unsigned char nbbit, unsigned char alone, signed long Value);
   signed long    L0_WriteRegisterTrace(L0_Context *i2c, unsigned char* name, unsigned char* valtxt, unsigned int iI2CIndex, unsigned char offset, unsigned char nbbit, unsigned char alone, signed long Value);

    signed int     L0_ReadString        (L0_Context* i2c, unsigned char *readString, unsigned char *pbtDataBuffer);
    signed int     L0_WriteString       (L0_Context* i2c, unsigned char *writeString);
    const unsigned char*   Si_I2C_TAG_TEXT      (void);

    signed int     L0_FastI2C           (void);
    signed int     L0_SlowI2C           (void);

#ifdef    USB_Capability
    signed int     L0_Cypress_Process   (const char *cmd, const char *text, double dval, double *retdval, char **rettxt);
    signed int     L0_Cypress_Configure (const char *cmd, const char *text, double dval, double *retdval, char **rettxt);
    signed int     L0_Cypress_Cget      (const char *cmd, const char *text, double dval, double *retdval, char **rettxt);
#endif /* USB_Capability*/

#ifdef __cplusplus
}
#endif

#endif /* _L0_API_H_*/

