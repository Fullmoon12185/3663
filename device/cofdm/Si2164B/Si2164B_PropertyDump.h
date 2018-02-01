/* Automatically generated from a template on 2013 August 28 at 12:04 */ /*( Processed on PC0008680 (mdorval) ) */

#ifndef   _Si2164B_PROPERTY_DUMP_H_
#define   _Si2164B_PROPERTY_DUMP_H_

#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif
signed int Si2164B_DumpProperties(L1_Si2164B_Context *api);
unsigned char Si2164B_DumpAllToMem(L1_Si2164B_Context *api,unsigned long *pDataAddr, unsigned long  *ulSize);
unsigned char Si2164B_DumpCmdToMem(L1_Si2164B_Context *api,unsigned long *pDataAddr, unsigned long  *ulSize);
unsigned char Si2164B_RestoreFromMem(L1_Si2164B_Context *api);
#if defined( __cplusplus )
}
#endif
#endif /*_Si2164B_PROPERTY_DUMP_H_                     */

#ifdef TEST_SIM

unsigned char Data_Compare(void);

#endif // TEST_SIM

/* End of template (2013 August 28 at 12:04) */
