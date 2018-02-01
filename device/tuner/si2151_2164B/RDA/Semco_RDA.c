/*************************************************************************************************************/
/*                                       RF Tuner RDA_functions                                        */
/*-----------------------------------------------------------------------------------------------------------*/
/*   This source code contains all API RDA5816sw_functions for the RDA5816sw Tuner on the RF board           */
/*     All included RDA5816sw_functions can be accessed by the higher software level as imports              */
/*************************************************************************************************************/
/*#define RWTRACES*/
#define Layer1_RDA
#include "Semco_RDA.h"
#define SemcoSleep system_wait


#define Tuner_I2C_write( index, buffer, count)   L0_WriteBytes (context->i2c, index, count, buffer)
#define Tuner_I2C_read(  index, buffer, count)   L0_ReadBytes  (context->i2c, index, count, buffer)

//#define Xtal_27M
//#define Xtal_30M
#define Xtal_24M

  /* gain band/gain_stage:    0     1     2     3     4     5     6     7     8     9    10    11    12    13   14&15 */
  signed int RDA5815gain[6][15] = {
  /* 1  950= Freq <1150 */ { -38, -38,  23,  83, 142, 198, 293, 376, 439, 516, 573, 633, 674, 732, 732 },
  /* 2 1150= Freq <1350 */ { -23, -23,  37,  97, 156, 212, 305, 387, 443, 512, 570, 630, 670, 731, 731 },
  /* 3 1350= Freq <1550 */ { -5,  -5,   56, 117, 176, 232, 321, 403, 453, 517, 576, 636, 674, 736, 736 },
  /* 4 1550= Freq <1900 */ {  14,  14,  74, 134, 194, 251, 338, 419, 465, 526, 587, 647, 683, 747, 747 },
  /* 5 1900= Freq <2150 */ { -4,  -4,   57, 117, 176, 235, 319, 401, 442, 500, 563, 623, 656, 723, 723 },
  };
  /* gain band/gain_stage:    0        1       2     3     4      5      6     7     8     9    10    11    12    13    14    15 */
  signed int RDA5816swgain[13][16] = {
							{ -23,   -24,    36,  96, 151 , 243, 302, 355, 410, 476, 535, 585, 679, 688, 688,  688},  /* 1  950MHz<=Freq<1000MHz */
							{ -29,   -29,    30,  90, 145 , 237, 296, 353, 406, 473, 532, 582, 682, 682, 682,  682},  /* 2  1000MHz<=Freq<1100MHz */
							{ -30,   -30,    30,  89, 144 , 236, 296, 354, 409, 476, 534, 586, 680, 680, 680,  680},  /* 3  1100MHz<=Freq<1200MHz */
							{ -27,   -28,    32,  92, 147 , 239, 298, 360, 414, 482, 541, 592, 680, 680, 680,  680},  /* 4  1200MHz<=Freq<1300MHz */
							{ -27,   -28,    32,  92, 146 , 238, 298, 363, 417, 486, 545, 597, 678, 678, 678,  678},  /* 5  1300MHz<=Freq<1400MHz */
							{ -31,   -32,    28,  88, 143 , 235, 294, 363, 417, 487, 546, 597, 672, 672, 672,  672},  /* 6  1400MHz<=Freq<1500MHz */
							{ -37,   -38,    22,  82, 136 , 228, 288, 360, 415, 486, 544, 596, 663, 663, 663,  663},  /* 7  1500MHz<=Freq<1600MHz */
							{ -43,   -44,    16,  76, 130 , 223, 282, 358, 413, 485, 544, 595, 656, 656, 656,  656},  /* 8  1600MHz<=Freq<1700MHz */
							{ -51,   -52,    8,   68, 122 , 214, 274, 353, 408, 481, 540, 590, 648, 648, 648,  648},  /* 9  1700MHz<=Freq<1800MHz */
							{ -51,   -52,    8,   68, 122 , 215, 274, 356, 411, 486, 545, 596, 649, 649, 649,  649},  /* 10  1800MHz<=Freq<1900MHz */
							{ -65,   -65,   -5,   55, 109 , 201, 261, 345, 400, 476, 535, 585, 634, 634, 634,  634},  /* 11  1900MHz<=Freq<2000MHz */
							{ -72,   -73,   -13,  47, 102 , 194, 254, 337, 393, 470, 529, 579, 624, 624, 624,  624},  /* 12  2000MHz<=Freq<2100MHz */
							{ -77,   -78,   -18,  42,  97 , 189, 248, 331, 387, 465, 524, 573, 617, 617, 617,  617}	  /* 13  2100MHz<=Freq<=2150MHz */
						};

signed int RDA_Communication_Check (RDA_Context *context) {
	unsigned char byteBuffer[5];
    if (L0_ReadCommandBytes(context->i2c, 5, byteBuffer) != 5) {
      SiTRACE("RDA_Communication_Check ERROR reading 5 bytes 0!\n");
      safe_printf ("RDA_Communication_Check ERROR reading 5 bytes 0!\n");
	  /*system("pause");*/
      return 0;
    }
    if (byteBuffer[0] == 0xff) {
      SiTRACE("RDA_Communication_Check ERROR: first byte 0xff!\n");
      return 0;
    }
    if (byteBuffer[0] == 0x00) {
      SiTRACE("RDA_Communication_Check ERROR: first byte 0x00!\n");
      return 0;
    }
	return 1;
}

void SemcoRDA_IIC_Write (RDA_Context *context, unsigned char index1, unsigned char value) {
	context->bytes[0] = value;
	L0_WriteBytes (context->i2c, index1, 1, context->bytes);
}

void SemcoRDA_IIC_Read (RDA_Context *context, unsigned char index1, unsigned char *value) {
	L0_ReadBytes (context->i2c, index1, 1, value);
}

/* Initialize RDA */
void SemcoRDA_Initialize(RDA_Context *context)
{

	unsigned char temp;			// Vco Pulling Solution 2, 2013-06-19
	SemcoSleep(1);//Wait 1ms.

	switch(context->i2c->address)
	{
		case SAT2_ADDRESS: // SINGLE T2CS2 5815M
			context->version = 0x00;
			 // Chip register soft reset 	
			SemcoRDA_IIC_Write(context, 0x04,0x04);
			SemcoRDA_IIC_Write(context, 0x04,0x05); 

			// Initial configuration start

			//pll setting 
			SemcoRDA_IIC_Write(context, 0x1a,0x13);
			SemcoRDA_IIC_Write(context, 0x41,0x53);
			SemcoRDA_IIC_Write(context, 0x38,0x9B);
			SemcoRDA_IIC_Write(context, 0x39,0x15);
			SemcoRDA_IIC_Write(context, 0x3A,0x00);
			SemcoRDA_IIC_Write(context, 0x3B,0x00);
			SemcoRDA_IIC_Write(context, 0x3C,0x0c);
			SemcoRDA_IIC_Write(context, 0x0c,0xE2);
			SemcoRDA_IIC_Write(context, 0x2e,0x6F);
		#ifdef Xtal_27M
			SemcoRDA_IIC_Write(context, 0x72,0x07);	// v1.1, 1538~1539
			SemcoRDA_IIC_Write(context, 0x73,0x10);
			SemcoRDA_IIC_Write(context, 0x74,0x71);
			SemcoRDA_IIC_Write(context, 0x75,0x06); // v1.1, 1363~1364, 1862~1863
			SemcoRDA_IIC_Write(context, 0x76,0x40);
			SemcoRDA_IIC_Write(context, 0x77,0x89);
			SemcoRDA_IIC_Write(context, 0x79,0x04);	// v1.1, 900
			SemcoRDA_IIC_Write(context, 0x7A,0x2A);
			SemcoRDA_IIC_Write(context, 0x7B,0xAA);
			SemcoRDA_IIC_Write(context, 0x7C,0xAB);
		#endif
		#ifdef Xtal_30M
			SemcoRDA_IIC_Write(context, 0x72,0x06);	// v1.2, 1544~1545
			SemcoRDA_IIC_Write(context, 0x73,0x60);
			SemcoRDA_IIC_Write(context, 0x74,0x66);
			SemcoRDA_IIC_Write(context, 0x75,0x05); // v1.2, 1364~1365, 1859~1860
			SemcoRDA_IIC_Write(context, 0x76,0xA0);
			SemcoRDA_IIC_Write(context, 0x77,0x7B);
			SemcoRDA_IIC_Write(context, 0x79,0x03);	// v1.2, 901
			SemcoRDA_IIC_Write(context, 0x7A,0xC0);
			SemcoRDA_IIC_Write(context, 0x7B,0x00);
			SemcoRDA_IIC_Write(context, 0x7C,0x00);
		#endif
		#ifdef Xtal_24M
			SemcoRDA_IIC_Write(context, 0x72,0x08);	// v1.3, 1547~1548
			SemcoRDA_IIC_Write(context, 0x73,0x00);
			SemcoRDA_IIC_Write(context, 0x74,0x80);
			SemcoRDA_IIC_Write(context, 0x75,0x07); // v1.3, 1367~1368, 1859~1860
			SemcoRDA_IIC_Write(context, 0x76,0x10);
			SemcoRDA_IIC_Write(context, 0x77,0x9A);
			SemcoRDA_IIC_Write(context, 0x79,0x04);	// v1.3, 901
			SemcoRDA_IIC_Write(context, 0x7A,0xB0);
			SemcoRDA_IIC_Write(context, 0x7B,0x00);
			SemcoRDA_IIC_Write(context, 0x7C,0x00);
		#endif

			SemcoRDA_IIC_Write(context, 0x2f,0x57);
			SemcoRDA_IIC_Write(context, 0x0d,0x70);
			SemcoRDA_IIC_Write(context, 0x18,0x4B);
			SemcoRDA_IIC_Write(context, 0x30,0xFF);
			SemcoRDA_IIC_Write(context, 0x5c,0xFF);
			SemcoRDA_IIC_Write(context, 0x65,0x00);
			SemcoRDA_IIC_Write(context, 0x70,0x3F);
			SemcoRDA_IIC_Write(context, 0x71,0x3F);
			SemcoRDA_IIC_Write(context, 0x53,0xA8);
			SemcoRDA_IIC_Write(context, 0x46,0x21);
			SemcoRDA_IIC_Write(context, 0x47,0x84);
			SemcoRDA_IIC_Write(context, 0x48,0x10);
			SemcoRDA_IIC_Write(context, 0x49,0x08);
			SemcoRDA_IIC_Write(context, 0x60,0x80);
			SemcoRDA_IIC_Write(context, 0x61,0x80);
			SemcoRDA_IIC_Write(context, 0x6A,0x08);
			SemcoRDA_IIC_Write(context, 0x6B,0x63);
			SemcoRDA_IIC_Write(context, 0x69,0xF8);
			SemcoRDA_IIC_Write(context, 0x57,0x64);
			SemcoRDA_IIC_Write(context, 0x05,0xaa);
			SemcoRDA_IIC_Write(context, 0x06,0xaa);
			SemcoRDA_IIC_Write(context, 0x15,0xAE);
			SemcoRDA_IIC_Write(context, 0x4a,0x67);
			SemcoRDA_IIC_Write(context, 0x4b,0x77);
 	   
			//agc setting
			SemcoRDA_IIC_Write(context, 0x4f,0x40);
			SemcoRDA_IIC_Write(context, 0x5b,0x20);
   
			SemcoRDA_IIC_Write(context, 0x16,0x0C);
			SemcoRDA_IIC_Write(context, 0x18,0x0C);            
			SemcoRDA_IIC_Write(context, 0x30,0x1C);            
			SemcoRDA_IIC_Write(context, 0x5c,0x2C);            
			SemcoRDA_IIC_Write(context, 0x6c,0x3C);            
			SemcoRDA_IIC_Write(context, 0x6e,0x3C);            
			SemcoRDA_IIC_Write(context, 0x1b,0x7C);            
			SemcoRDA_IIC_Write(context, 0x1d,0xBD);            
			SemcoRDA_IIC_Write(context, 0x1f,0xBD);            
			SemcoRDA_IIC_Write(context, 0x21,0xBE);            
			SemcoRDA_IIC_Write(context, 0x23,0xBE);            
			SemcoRDA_IIC_Write(context, 0x25,0xFE);            
			SemcoRDA_IIC_Write(context, 0x27,0xFF);            
			SemcoRDA_IIC_Write(context, 0x29,0xFF);            
			SemcoRDA_IIC_Write(context, 0xb3,0xFF);            
			SemcoRDA_IIC_Write(context, 0xb5,0xFF);            
                                         
			SemcoRDA_IIC_Write(context, 0x17,0xF0);            
			SemcoRDA_IIC_Write(context, 0x19,0xF0);            
			SemcoRDA_IIC_Write(context, 0x31,0xF0);            
			SemcoRDA_IIC_Write(context, 0x5d,0xF0);            
			SemcoRDA_IIC_Write(context, 0x6d,0xF0);            
			SemcoRDA_IIC_Write(context, 0x6f,0xF1);            
			SemcoRDA_IIC_Write(context, 0x1c,0xF5);            
			SemcoRDA_IIC_Write(context, 0x1e,0x35);            
			SemcoRDA_IIC_Write(context, 0x20,0x79);            
			SemcoRDA_IIC_Write(context, 0x22,0x9D);            
			SemcoRDA_IIC_Write(context, 0x24,0xBE);            
			SemcoRDA_IIC_Write(context, 0x26,0xBE);            
			SemcoRDA_IIC_Write(context, 0x28,0xBE);            
			SemcoRDA_IIC_Write(context, 0x2a,0xCF);            
			SemcoRDA_IIC_Write(context, 0xb4,0xDF);            
			SemcoRDA_IIC_Write(context, 0xb6,0x0F);            
                                         
			SemcoRDA_IIC_Write(context, 0xb7,0x15);	//start    
			SemcoRDA_IIC_Write(context, 0xb9,0x6c);	           
			SemcoRDA_IIC_Write(context, 0xbb,0x63);	           
			SemcoRDA_IIC_Write(context, 0xbd,0x5a);	           
			SemcoRDA_IIC_Write(context, 0xbf,0x5a);	           
			SemcoRDA_IIC_Write(context, 0xc1,0x55);	           
			SemcoRDA_IIC_Write(context, 0xc3,0x55);	           
			SemcoRDA_IIC_Write(context, 0xc5,0x47);	           
			SemcoRDA_IIC_Write(context, 0xa3,0x53);	           
			SemcoRDA_IIC_Write(context, 0xa5,0x4f);	           
			SemcoRDA_IIC_Write(context, 0xa7,0x4e);	           
			SemcoRDA_IIC_Write(context, 0xa9,0x4e);	           
			SemcoRDA_IIC_Write(context, 0xab,0x54);            
			SemcoRDA_IIC_Write(context, 0xad,0x31);            
			SemcoRDA_IIC_Write(context, 0xaf,0x43);            
			SemcoRDA_IIC_Write(context, 0xb1,0x9f);               
                                         
                                         
			SemcoRDA_IIC_Write(context, 0xb8,0x6c); //end      
			SemcoRDA_IIC_Write(context, 0xba,0x92);            
			SemcoRDA_IIC_Write(context, 0xbc,0x8a);            
			SemcoRDA_IIC_Write(context, 0xbe,0x8a);            
			SemcoRDA_IIC_Write(context, 0xc0,0x82);            
			SemcoRDA_IIC_Write(context, 0xc2,0x93);            
			SemcoRDA_IIC_Write(context, 0xc4,0x85);            
			SemcoRDA_IIC_Write(context, 0xc6,0x77);            
			SemcoRDA_IIC_Write(context, 0xa4,0x82);            
			SemcoRDA_IIC_Write(context, 0xa6,0x7e);            
			SemcoRDA_IIC_Write(context, 0xa8,0x7d);            
			SemcoRDA_IIC_Write(context, 0xaa,0x6f);            
			SemcoRDA_IIC_Write(context, 0xac,0x65);            
			SemcoRDA_IIC_Write(context, 0xae,0x43);            
			SemcoRDA_IIC_Write(context, 0xb0,0x9f);             
			SemcoRDA_IIC_Write(context, 0xb2,0xf0);             
                                         
                                         
			SemcoRDA_IIC_Write(context, 0x81,0x92); //rise     
			SemcoRDA_IIC_Write(context, 0x82,0xb4);            
			SemcoRDA_IIC_Write(context, 0x83,0xb3);            
			SemcoRDA_IIC_Write(context, 0x84,0xac);            
			SemcoRDA_IIC_Write(context, 0x85,0xba);            
			SemcoRDA_IIC_Write(context, 0x86,0xbc);            
			SemcoRDA_IIC_Write(context, 0x87,0xaf);            
			SemcoRDA_IIC_Write(context, 0x88,0xa2);            
			SemcoRDA_IIC_Write(context, 0x89,0xac);            
			SemcoRDA_IIC_Write(context, 0x8a,0xa9);            
			SemcoRDA_IIC_Write(context, 0x8b,0x9b);            
			SemcoRDA_IIC_Write(context, 0x8c,0x7d);            
			SemcoRDA_IIC_Write(context, 0x8d,0x74);            
			SemcoRDA_IIC_Write(context, 0x8e,0x9f);           
			SemcoRDA_IIC_Write(context, 0x8f,0xf0);               
                                         
			SemcoRDA_IIC_Write(context, 0x90,0x15); //fall     
			SemcoRDA_IIC_Write(context, 0x91,0x39);            
			SemcoRDA_IIC_Write(context, 0x92,0x30);            
			SemcoRDA_IIC_Write(context, 0x93,0x27);            
			SemcoRDA_IIC_Write(context, 0x94,0x29);            
			SemcoRDA_IIC_Write(context, 0x95,0x0d);            
			SemcoRDA_IIC_Write(context, 0x96,0x10);            
			SemcoRDA_IIC_Write(context, 0x97,0x1e);            
			SemcoRDA_IIC_Write(context, 0x98,0x1a);            
			SemcoRDA_IIC_Write(context, 0x99,0x19);            
			SemcoRDA_IIC_Write(context, 0x9a,0x19);            
			SemcoRDA_IIC_Write(context, 0x9b,0x32);            
			SemcoRDA_IIC_Write(context, 0x9c,0x1f);            
			SemcoRDA_IIC_Write(context, 0x9d,0x31);            
			SemcoRDA_IIC_Write(context, 0x9e,0x43);     
			break;

		case SAT1_MAIN_ADDRESS:  // DUAL 5816SW 1
		case SAT1_SUB_ADDRESS:   // DUAL 5816SW 2

			// Chip register soft reset
			SemcoRDA_IIC_Write(context, 0x04,0x04);
			SemcoRDA_IIC_Write(context, 0x04,0x05);

			// Initial configuration start

			//pll setting
			SemcoRDA_IIC_Write(context, 0x2b,0x97); //clk_interface_27m=1
			SemcoRDA_IIC_Write(context, 0x1a,0x13);
			SemcoRDA_IIC_Write(context, 0x41,0x53);
			SemcoRDA_IIC_Write(context, 0x38,0x9B); /* 20120413 mod 0x90-> 0x93 */
			SemcoRDA_IIC_Write(context, 0x39,0x15);
			SemcoRDA_IIC_Write(context, 0x3A,0x00);
			SemcoRDA_IIC_Write(context, 0x3B,0x00);
			SemcoRDA_IIC_Write(context, 0x3C,0x0C);  // 20110701 add [강전계 특성 수정]
			SemcoRDA_IIC_Write(context, 0x0c,0xE2);
			SemcoRDA_IIC_Write(context, 0x2e,0x6F);
			SemcoRDA_IIC_Write(context, 0x5b,0x20);
			SemcoRDA_IIC_Write(context, 0x2f,0x57);
			SemcoRDA_IIC_Write(context, 0x0d,0x70);
			SemcoRDA_IIC_Write(context, 0x16,0x03);
			SemcoRDA_IIC_Write(context, 0x18,0x4B);
			SemcoRDA_IIC_Write(context, 0x30,0xFF);
			SemcoRDA_IIC_Write(context, 0x5c,0xFF);
			SemcoRDA_IIC_Write(context, 0x6c,0xFF);
			SemcoRDA_IIC_Write(context, 0x6e,0xFF);
			SemcoRDA_IIC_Write(context, 0x65,0xD4);
			SemcoRDA_IIC_Write(context, 0x70,0x3F);
			SemcoRDA_IIC_Write(context, 0x71,0x3F);
			SemcoRDA_IIC_Write(context, 0x75,0x07);//for 24M crystal
			SemcoRDA_IIC_Write(context, 0x76,0x10);//for 24M crystal
			SemcoRDA_IIC_Write(context, 0x77,0x9A);//for 24M crystal
			SemcoRDA_IIC_Write(context, 0x53,0xA8);
			SemcoRDA_IIC_Write(context, 0x46,0x21);
			SemcoRDA_IIC_Write(context, 0x47,0x84);
			SemcoRDA_IIC_Write(context, 0x48,0x10);
			SemcoRDA_IIC_Write(context, 0x49,0x08);
			SemcoRDA_IIC_Write(context, 0x60,0x80);
			SemcoRDA_IIC_Write(context, 0x61,0x80);
			SemcoRDA_IIC_Write(context, 0x6A,0x08);
			SemcoRDA_IIC_Write(context, 0x6B,0x63);
			SemcoRDA_IIC_Write(context, 0x69,0xF8);
			SemcoRDA_IIC_Write(context, 0x57,0x64);
			SemcoRDA_IIC_Write(context, 0x05,0x55);
			SemcoRDA_IIC_Write(context, 0x06,0x65);
			SemcoRDA_IIC_Write(context, 0x15,0xFF);
			SemcoRDA_IIC_Write(context, 0x4a,0xBB);
			SemcoRDA_IIC_Write(context, 0x4b,0xDB);

			//th1=901M
			SemcoRDA_IIC_Write(context, 0x79,0x04);//for 24M crystal
			SemcoRDA_IIC_Write(context, 0x7a,0xB0);//for 24M crystal
			SemcoRDA_IIC_Write(context, 0x7b,0x00);//for 24M crystal
			SemcoRDA_IIC_Write(context, 0x7c,0x00);//for 24M crystal

			//th2=1580M
			SemcoRDA_IIC_Write(context, 0x72,0x08);//for 24M crystal
			SemcoRDA_IIC_Write(context, 0x73,0x00);//for 24M crystal
			SemcoRDA_IIC_Write(context, 0x74,0x80);//for 24M crystal

			//agc setting
			SemcoRDA_IIC_Write(context, 0x4f,0x40);
			SemcoRDA_IIC_Write(context, 0x5b,0x20);

			SemcoRDA_IIC_Read(context,0x01,&temp);		// v1.1
			context->version = temp;


			SemcoRDA_IIC_Write(context,0x16,0x0C);		// stage setting
			SemcoRDA_IIC_Write(context,0x18,0x0C);
			SemcoRDA_IIC_Write(context,0x30,0x1C);
			SemcoRDA_IIC_Write(context,0x5c,0x2C);
			SemcoRDA_IIC_Write(context,0x6c,0x2C);
			SemcoRDA_IIC_Write(context,0x6e,0xAC);
			SemcoRDA_IIC_Write(context,0x1b,0xBC);
			SemcoRDA_IIC_Write(context,0x1d,0xBD);
			SemcoRDA_IIC_Write(context,0x1f,0xBD);
			SemcoRDA_IIC_Write(context,0x21,0xBE);
			SemcoRDA_IIC_Write(context,0x23,0xFE);
			SemcoRDA_IIC_Write(context,0x25,0xFE);
			SemcoRDA_IIC_Write(context,0x27,0xFF);
			SemcoRDA_IIC_Write(context,0x29,0xFF);
			SemcoRDA_IIC_Write(context,0xb3,0xFF);
			SemcoRDA_IIC_Write(context,0xb5,0xFF);

			SemcoRDA_IIC_Write(context,0x17,0xF0);
			SemcoRDA_IIC_Write(context,0x19,0xF0);
			SemcoRDA_IIC_Write(context,0x31,0xF0);
			SemcoRDA_IIC_Write(context,0x5d,0xF0);
			SemcoRDA_IIC_Write(context,0x6d,0xF1);
			SemcoRDA_IIC_Write(context,0x6f,0xF1);
			SemcoRDA_IIC_Write(context,0x1c,0xF1);
			SemcoRDA_IIC_Write(context,0x1e,0xF5);
			SemcoRDA_IIC_Write(context,0x20,0xF6);
			SemcoRDA_IIC_Write(context,0x22,0xFA);
			SemcoRDA_IIC_Write(context,0x24,0xFA);
			SemcoRDA_IIC_Write(context,0x26,0xFB);
			SemcoRDA_IIC_Write(context,0x28,0xFF);
			SemcoRDA_IIC_Write(context,0x2a,0xFF);
			SemcoRDA_IIC_Write(context,0xb4,0xFF);
			SemcoRDA_IIC_Write(context,0xb6,0xFF);  

			SemcoRDA_IIC_Write(context,0xb7,0x00);//start0
			SemcoRDA_IIC_Write(context,0xb9,0x42);//start1
			SemcoRDA_IIC_Write(context,0xbb,0x4B);//start2
			SemcoRDA_IIC_Write(context,0xbd,0x4A);//start3
			SemcoRDA_IIC_Write(context,0xbf,0x5E);//start4
			SemcoRDA_IIC_Write(context,0xc1,0x44);//start5
			SemcoRDA_IIC_Write(context,0xc3,0x45);//start6
			SemcoRDA_IIC_Write(context,0xc5,0x47);//start7
			SemcoRDA_IIC_Write(context,0xa3,0x48);//start8
			SemcoRDA_IIC_Write(context,0xa5,0x46);//start9
			SemcoRDA_IIC_Write(context,0xa7,0x46);//start10
			SemcoRDA_IIC_Write(context,0xa9,0x43);//start11
			SemcoRDA_IIC_Write(context,0xab,0x24);//start12
			SemcoRDA_IIC_Write(context,0xad,0x34);//start13
			SemcoRDA_IIC_Write(context,0xaf,0x63);//start14
			SemcoRDA_IIC_Write(context,0xb1,0x95);//start15

			SemcoRDA_IIC_Write(context,0xb8,0x43);//end0
			SemcoRDA_IIC_Write(context,0xba,0x78);//end1
			SemcoRDA_IIC_Write(context,0xbc,0x78);//end2
			SemcoRDA_IIC_Write(context,0xbe,0x87);//end3
			SemcoRDA_IIC_Write(context,0xc0,0x8A);//end4
			SemcoRDA_IIC_Write(context,0xc2,0x72);//end5
			SemcoRDA_IIC_Write(context,0xc4,0x73);//end6
			SemcoRDA_IIC_Write(context,0xc6,0x75);//end7
			SemcoRDA_IIC_Write(context,0xa4,0x76);//end8
			SemcoRDA_IIC_Write(context,0xa6,0x74);//end9
			SemcoRDA_IIC_Write(context,0xa8,0x6D);//end10
			SemcoRDA_IIC_Write(context,0xaa,0x6A);//end11
			SemcoRDA_IIC_Write(context,0xac,0x35);//end12
			SemcoRDA_IIC_Write(context,0xae,0x63);//end13
			SemcoRDA_IIC_Write(context,0xb0,0x95);//end14
			SemcoRDA_IIC_Write(context,0xb2,0xCA);//end15 

			SemcoRDA_IIC_Write(context,0x81,0x7F);//rise0
			SemcoRDA_IIC_Write(context,0x82,0xA7);//rise1
			SemcoRDA_IIC_Write(context,0x83,0xB6);//rise2
			SemcoRDA_IIC_Write(context,0x84,0xB5);//rise3
			SemcoRDA_IIC_Write(context,0x85,0xB8);//rise4
			SemcoRDA_IIC_Write(context,0x86,0xA3);//rise5
			SemcoRDA_IIC_Write(context,0x87,0xA3);//rise6
			SemcoRDA_IIC_Write(context,0x88,0xA5);//rise7
			SemcoRDA_IIC_Write(context,0x89,0xA6);//rise8
			SemcoRDA_IIC_Write(context,0x8a,0x9E);//rise9
			SemcoRDA_IIC_Write(context,0x8b,0x8A);//rise10
			SemcoRDA_IIC_Write(context,0x8c,0xc1);//rise11
			SemcoRDA_IIC_Write(context,0x8d,0x6A);//rise12
			SemcoRDA_IIC_Write(context,0x8e,0x9C);//rise13
			SemcoRDA_IIC_Write(context,0x8f,0xD1);//rise14

			SemcoRDA_IIC_Write(context,0x90,0x00);//fall1
			SemcoRDA_IIC_Write(context,0x91,0x0A);//fall2
			SemcoRDA_IIC_Write(context,0x92,0x13);//fall3
			SemcoRDA_IIC_Write(context,0x93,0x17);//fall4
			SemcoRDA_IIC_Write(context,0x94,0x0B);//fall5
			SemcoRDA_IIC_Write(context,0x95,0x0C);//fall6
			SemcoRDA_IIC_Write(context,0x96,0x0F);//fall7
			SemcoRDA_IIC_Write(context,0x97,0x10);//fall8
			SemcoRDA_IIC_Write(context,0x98,0x0E);//fall9
			SemcoRDA_IIC_Write(context,0x99,0x0E);//fall10
			SemcoRDA_IIC_Write(context,0x9a,0x14);//fall11
			SemcoRDA_IIC_Write(context,0x9b,0x00);//fall12
			SemcoRDA_IIC_Write(context,0x9c,0x1C);//fall13
			SemcoRDA_IIC_Write(context,0x9d,0x2C);//fall14
			SemcoRDA_IIC_Write(context,0x9e,0x5B);//fall15

			SemcoSleep(10); //Wait 10ms;

			SemcoRDA_IIC_Write(context, 0x04,0x85);  // enable current channel of dual-channel tuner

			context->i2c->address = context->i2c->address ^ 0x02;
			SemcoRDA_IIC_Read(context, 0x04, &temp);
			temp |= 0x80;
			SemcoRDA_IIC_Write(context, 0x04, temp);  // enable adjacent channel of dual-channel tuner
			context->i2c->address = context->i2c->address ^ 0x02;
			break;

		default :
			break;
	}

	SemcoSleep(10); //Wait 10ms;

}
/*************************************************************************/
//	Function to Set the RDA
//	fPLL:   Frequency        			unit: MHz  from 250 to 2300
//	fSym:   SymbolRate       			unit: KS/s from 1000 to 45000
/************************************************************************/
unsigned int SemcoRDA_Set       (RDA_Context *context, unsigned long fPLL, unsigned long fSym)
{
 	unsigned char buffer=0;
 	unsigned long temp_value = 0;
	unsigned long bw;/*,temp_value1 = 0,temp_value2=0 ;*/
	unsigned char Filter_bw_control_bit;

    RDA_Communication_Check(context);


	switch(context->i2c->address)
	{
		case SAT2_ADDRESS: // SINGLE T2CS2 5815M

			SemcoRDA_IIC_Write(context, 0x04,0xc1); //add by rda 2011.8.9,RXON = 0 , change normal working state to idle state
			SemcoRDA_IIC_Write(context, 0x2b,0x95);//clk_interface_27m=0  add by rda 2012.1.12   

			//set frequency start
			#ifdef Xtal_27M		// v1.1
			temp_value = (unsigned long int)fPLL* 77672;//((2^21) / RDA5815_XTALFREQ);
			#endif
			#ifdef Xtal_30M		// v1.2
			temp_value = (unsigned long int)fPLL* 69905;//((2^21) / RDA5815_XTALFREQ);
			#endif
			#ifdef Xtal_24M		// v1.3
			temp_value = (unsigned long int)fPLL* 87381;//((2^21) / RDA5815_XTALFREQ);
			#endif

			buffer = ((unsigned char)((temp_value>>24)&0xff));
			SemcoRDA_IIC_Write(context, 0x07,buffer);
			buffer = ((unsigned char)((temp_value>>16)&0xff));	
			SemcoRDA_IIC_Write(context, 0x08,buffer);	
			buffer = ((unsigned char)((temp_value>>8)&0xff));
			SemcoRDA_IIC_Write(context, 0x09,buffer);	
			buffer = ((unsigned char)( temp_value&0xff));
			SemcoRDA_IIC_Write(context, 0x0a,buffer);
			//set frequency end
	
			// set Filter bandwidth start
			bw=fSym;
			SiTRACE("bw %d\n", (signed int)bw);
			if(bw<4000)
			bw= 4000;    // KHz
			else if(bw>45000)
			bw = 40000;   // KHz    //modify by rda 2012.1.12   
	
			SiTRACE("bw %d\n", (signed int)bw);
			Filter_bw_control_bit = (unsigned char)((bw*135/200+4000)/1000);
	
			Filter_bw_control_bit&=0x3f;

			#ifdef Xtal_30M		// v1.2
			Filter_bw_control_bit|=0x40;
			#endif
			#ifdef Xtal_24M		// v1.2
			Filter_bw_control_bit|=0x40;
			#endif

			SemcoRDA_IIC_Write(context, 0x0b,Filter_bw_control_bit);
			// set Filter bandwidth end
	
			SemcoRDA_IIC_Write(context, 0x04,0xc3); //add by rda 2011.8.9,RXON = 0 ,rxon=1,normal working
			SemcoRDA_IIC_Write(context, 0x2b,0x97);//clk_interface_27m=1  add by rda 2012.1.12  
			break;

		case SAT1_MAIN_ADDRESS:  // DUAL 5816SW 1
		case SAT1_SUB_ADDRESS:   // DUAL 5816SW 2

			SemcoRDA_IIC_Write(context, 0x04,0xC1);

			//set frequency start

			if(fPLL>1075)
			{
				SemcoRDA_IIC_Read(context, 0x65,&buffer);
				buffer &= 0xF7;
				buffer |= 0x10;
				SemcoRDA_IIC_Write(context, 0x65,buffer);
				//fPLL = fPLL;
			}
			else
			{
				SemcoRDA_IIC_Read(context, 0x65,&buffer);
				buffer &= 0xEF;
				buffer |= 0x08;
				SemcoRDA_IIC_Write(context, 0x65,buffer);
				fPLL = fPLL * 2;
			}

			temp_value = (unsigned long)fPLL* 87381;	//for 24M crystal

			buffer = ((unsigned char)((temp_value>>24)&0xff));
			SemcoRDA_IIC_Write(context, 0x07,buffer);
			buffer = ((unsigned char)((temp_value>>16)&0xff));	
			SemcoRDA_IIC_Write(context, 0x08,buffer);	
			buffer = ((unsigned char)((temp_value>>8)&0xff));
			SemcoRDA_IIC_Write(context, 0x09,buffer);	
			buffer = ((unsigned char)( temp_value&0xff));
			SemcoRDA_IIC_Write(context, 0x0a,buffer);
			//set frequency end

			// set Filter bandwidth start
			bw=fSym;
			SiTRACE("bw %d\n", (signed int)bw);

			Filter_bw_control_bit = (unsigned char)((bw*135/200+4000)/1000);

			if(Filter_bw_control_bit<4)
			Filter_bw_control_bit = 4;		// MHz
			else if(Filter_bw_control_bit>40)
			Filter_bw_control_bit = 40;	// MHz

			Filter_bw_control_bit &= 0x3f;
			Filter_bw_control_bit |= 0x40;
			SemcoRDA_IIC_Write(context, 0x0b,Filter_bw_control_bit);
			// set Filter bandwidth end

			SemcoRDA_IIC_Write(context, 0x04,0xc3);
			break;

		default :
			break;
	}

	SemcoSleep(5);//Wait 5ms;

	return 1;
}
/* Get RDA Status */
void  L1_RF_RDA_Init          (RDA_Context *context, unsigned int add) {
    SiTRACE("RDA_Init starting...\n");
    context->i2c = &(context->i2cObj);
    L0_Init(context->i2c);
    L0_SetAddress(context->i2c, add, 1);
    context->i2c->mustReadWithoutStop =1;
    context->IF                   =                     0;
    context->RF                   =                950000;
    context->minRF                =                925000;
    context->maxRF                =               2175000;
    context->LPF                  =              38100000;
    SiTRACE("L1_RF_RDA_Init complete...\n");
}
signed int   L1_RF_RDA_InitAfterReset(RDA_Context *context) {
	SiTRACE("L1_RF_RDA_InitAfterReset starting...\n");

	switch(context->i2c->address)
	{
		case SAT1_MAIN_ADDRESS:
		case SAT1_SUB_ADDRESS:
		case SAT2_ADDRESS:
			SemcoRDA_Initialize(context);
			break;
		case SAT4_ADDRESS:
			L1_RF_AV2018_InitAfterReset(context);
			break;
		default :
			break;
	}

	SiTRACE("L1_RF_RDA_InitAfterReset complete...\n");
	return 0;
}
signed int   L1_RF_RDA_Get_RF        (RDA_Context *context) {
   return context->RF;
}

signed int   L1_RF_RDA_Tune          (RDA_Context *context, signed int rf) {
   /* Checking limits */
   if (rf < (context->minRF)) {SiTRACE("rf %d < minRF %d\n",rf,context->minRF); return L1_RF_RDA_Tune(context, context->minRF);}
   if (rf > (context->maxRF)) {SiTRACE("rf %d > maxRF %d\n",rf,context->maxRF); return L1_RF_RDA_Tune(context, context->maxRF);}
   context->i2c->trackRead = context->i2c->trackWrite;

	switch(context->i2c->address)
	{
		case SAT1_MAIN_ADDRESS:
		case SAT1_SUB_ADDRESS:
		case SAT2_ADDRESS:
			SemcoRDA_Set(context, rf/1000, context->LPF/1000);
			break;
		case SAT4_ADDRESS:
			L1_RF_AV2018_Tune(context, rf);
			break;
		default :
			break;
	}
    
    context->RF = rf;
   return context->RF;
}

signed int   L1_RF_RDA_LPF           (RDA_Context *context, signed int lpf) {

	switch(context->i2c->address)
	{
		case SAT1_MAIN_ADDRESS:
		case SAT1_SUB_ADDRESS:
		case SAT2_ADDRESS:
			context->LPF = lpf;
			break;
		case SAT4_ADDRESS:
			context->LPF = lpf/1000;
			break;
		default :
			break;
	}

  return context->LPF;
}

unsigned char L1_RF_RDA_Wakeup        (RDA_Context *context)
{
	unsigned char temp;

	switch(context->i2c->address)
	{
		case SAT1_MAIN_ADDRESS:
		case SAT1_SUB_ADDRESS:
		case SAT2_ADDRESS:
			SemcoRDA_IIC_Read(context, 0x04, &temp);
			temp = temp | 0x80;
			SemcoRDA_IIC_Write(context, 0x04,temp);
			break;
		case SAT4_ADDRESS:
			L1_RF_AV2018_Wakeup(context);
			break;
		default :
			break;
	}

    return 0;
}

unsigned char L1_RF_RDA_Standby       (RDA_Context *context)
{
	unsigned char temp;

	switch(context->i2c->address)
	{
		case SAT1_MAIN_ADDRESS:
		case SAT1_SUB_ADDRESS:
		case SAT2_ADDRESS:
			SemcoRDA_IIC_Read(context, 0x04, &temp);
			temp = temp & 0x7F;
			SemcoRDA_IIC_Write(context, 0x04,temp);
			break;
		case SAT4_ADDRESS:
			L1_RF_AV2018_Standby(context);
			break;
		default :
			break;
	}

    return 0;
}

unsigned char L1_RF_RDA_LockStatus       (RDA_Context *context)
{
	unsigned char buffer=0, i=0 ;
	int ret = 0;

	switch(context->i2c->address)
	{
		case SAT1_MAIN_ADDRESS:
		case SAT1_SUB_ADDRESS:
		case SAT2_ADDRESS:
			for(i=0; i<100; i++)
			{
				SemcoRDA_IIC_Read(context, 0x03,&buffer);
				if((buffer & 0x03) != 0x03)
				{
					return 0;
				}
			}
			break;
		case SAT4_ADDRESS:
			ret = L1_RF_AV2018_LockStatus(context);
			if(ret != 1)
			{
				return 0;
			}
			break;
		default :
			break;
	}

	return 1;
}

// rf_chn=1 : switch to the same-side RF_pin;
// rf_chn=0 : switch to the different-side RF_pin.
//unsigned int RDA_RfSel(RDA_Context *context, unsigned char rf_chn)
unsigned int L1_RF_RDA_RfSel(RDA_Context *context, unsigned char rf_chn)
{
	unsigned char buffer = 0;

	if (rf_chn)		// Connect the same-side RF_pin, disconnect the different-side RF_pin
	{
		SemcoRDA_IIC_Read(context,0x65,&buffer);
		SemcoRDA_IIC_Write(context,0x65,(buffer | 0x04));

		context->i2c->address = context->i2c->address ^ 0x02;
		SemcoRDA_IIC_Read(context,0x65,&buffer);
		SemcoRDA_IIC_Write(context,0x65,(buffer & 0xFD));
		context->i2c->address = context->i2c->address ^ 0x02;
	}
	else				// Connect the different-side RF_pin, disconnect the same-side RF_pin
	{
		context->i2c->address = context->i2c->address ^ 0x02;
		SemcoRDA_IIC_Read(context,0x65,&buffer);
		SemcoRDA_IIC_Write(context,0x65,(buffer | 0x02));
		context->i2c->address = context->i2c->address ^ 0x02;

		SemcoRDA_IIC_Read(context,0x65,&buffer);
		SemcoRDA_IIC_Write(context,0x65,(buffer & 0xFB));
	}
	return 1;
}


unsigned int L1_RF_RDA_RfSel_Read   (RDA_Context *context, unsigned char *same_side, unsigned char *diff_side)
{
	unsigned char buffer = 0;

	// Read the same-side RF_pin setting value
	SemcoRDA_IIC_Read(context,0x65,&buffer);
	*same_side = (buffer >> 2) & 0x01;

	// Read the different-side RF_pin setting value
	context->i2c->address = context->i2c->address ^ 0x02;
	SemcoRDA_IIC_Read(context,0x65,&buffer);
	*diff_side = (buffer >> 1) & 0x01;
	context->i2c->address = context->i2c->address ^ 0x02;

	return 1;
}

signed int  L1_RF_RDA_RSSI (RDA_Context *context)
{
/* Gain stage limits
                                              filter..
                                               i2v--..
                                           post.. --..
                                          st2--.. --..
                                       pre.. --.. --..
                                     st1--.. --.. --..    */
#define RDA5815_Gain_Stage__0 0xc00 /* '1100 0000 0000' */
#define RDA5815_Gain_Stage__1 0xc00 /* '1100 0000 0000' */
#define RDA5815_Gain_Stage__2 0xc01 /* '1100 0000 0001' */
#define RDA5815_Gain_Stage__3 0xc02 /* '1100 0000 0010' */
#define RDA5815_Gain_Stage__4 0xc03 /* '1100 0000 0011' */
#define RDA5815_Gain_Stage__5 0xc13 /* '1100 0001 0011' */
#define RDA5815_Gain_Stage__6 0xd17 /* '1101 0001 0111' */
#define RDA5815_Gain_Stage__7 0xd5b /* '1101 0101 1011' */
#define RDA5815_Gain_Stage__8 0xe5b /* '1110 0101 1011' */
#define RDA5815_Gain_Stage__9 0xf9b /* '1111 1001 1011' */
#define RDA5815_Gain_Stage_10 0xfab /* '1111 1010 1011' */
#define RDA5815_Gain_Stage_11 0xfaf /* '1111 1010 1111' */
#define RDA5815_Gain_Stage_12 0xfeb /* '1111 1110 1111' */
#define RDA5815_Gain_Stage_13 0xfff /* '1111 1111 1111' */
#define RDA5815_Gain_Stage_14 0xfff /* '1111 1111 1111' */
#define RDA5815_Gain_Stage_15 0xfff /* '1111 1111 1111' */


#define RDA5816sw_Gain_Stage__0 0xC00 /* '1100 0000 0000' */
#define RDA5816sw_Gain_Stage__1 0xC00 /* '1100 0000 0000' */
#define RDA5816sw_Gain_Stage__2 0xC01 /* '1100 0000 0001' */
#define RDA5816sw_Gain_Stage__3 0xC02 /* '1100 0000 0010' */
#define RDA5816sw_Gain_Stage__4 0xC12 /* '1100 0001 0010' */
#define RDA5816sw_Gain_Stage__5 0xC1A /* '1100 0001 1010' */
#define RDA5816sw_Gain_Stage__6 0xC1B /* '1100 0001 1011' */
#define RDA5816sw_Gain_Stage__7 0xD5B /* '1101 0101 1011' */
#define RDA5816sw_Gain_Stage__8 0xD6B /* '1101 0110 1011' */
#define RDA5816sw_Gain_Stage__9 0xEAB /* '1110 1010 1011' */
#define RDA5816sw_Gain_Stage_10 0xEAF /* '1110 1010 1111' */
#define RDA5816sw_Gain_Stage_11 0xEBF /* '1110 1011 1111' */
#define RDA5816sw_Gain_Stage_12 0xFFF /* '1111 1111 1111' */
#define RDA5816sw_Gain_Stage_13 0xFFF /* '1111 1111 1111' */
#define RDA5816sw_Gain_Stage_14 0xFFF /* '1111 1111 1111' */
#define RDA5816sw_Gain_Stage_15 0xFFF /* '1111 1111 1111' */


  unsigned char data16,data17,st1,pre,st2,post,i2v,filter,vga;
  unsigned int  stage_code = 0;
  unsigned char gain_stage = 0;
  unsigned char band = 0;
  signed   int  vga_gain = 0;
  signed   int  total_gain = 0;


  //data16 = RDA5815ReadReg(context, 0x16);
  SemcoRDA_IIC_Read(context, 0x16,&data16);
  i2v    = (data16&0xc0)>>6;
  filter = (data16&0x30)>>4;
  st1    = (data16&0x0c)>>2;
  st2    = (data16&0x03)>>0;

  //data17 = RDA5815ReadReg(context, 0x17);
  SemcoRDA_IIC_Read(context, 0x17,&data17);
  pre    = (data17&0x0c)>>2;
  post   = (data17&0x03)>>0;

  stage_code = (st1<<10) + (pre<<8) + (st2<<6) + (post<<4) + (i2v<<2) + (filter<<0);

	switch(context->i2c->address)
	{
		case SAT2_ADDRESS: // SINGLE T2CS2 5815M
					 if (stage_code >= RDA5815_Gain_Stage_14 ) { gain_stage = 14; }
/* Prevent fix CID 96986 */
#if 0
				else if (stage_code >= RDA5815_Gain_Stage_13 ) { gain_stage = 13; }
#endif
				else if (stage_code >= RDA5815_Gain_Stage_12 ) { gain_stage = 12; }
				else if (stage_code >= RDA5815_Gain_Stage_11 ) { gain_stage = 11; }
				else if (stage_code >= RDA5815_Gain_Stage_10 ) { gain_stage = 10; }
				else if (stage_code >= RDA5815_Gain_Stage__9 ) { gain_stage =  9; }
				else if (stage_code >= RDA5815_Gain_Stage__8 ) { gain_stage =  8; }
				else if (stage_code >= RDA5815_Gain_Stage__7 ) { gain_stage =  7; }
				else if (stage_code >= RDA5815_Gain_Stage__6 ) { gain_stage =  6; }
				else if (stage_code >= RDA5815_Gain_Stage__5 ) { gain_stage =  5; }
				else if (stage_code >= RDA5815_Gain_Stage__4 ) { gain_stage =  4; }
				else if (stage_code >= RDA5815_Gain_Stage__3 ) { gain_stage =  3; }
				else if (stage_code >= RDA5815_Gain_Stage__2 ) { gain_stage =  2; }
				else if (stage_code >= RDA5815_Gain_Stage__1 ) { gain_stage =  1; }
				else  { gain_stage =  0; }

					 if (context->RF >= 1900000) { band = 5; }
				else if (context->RF >= 1550000) { band = 4; }
				else if (context->RF >= 1350000) { band = 3; }
				else if (context->RF >= 1150000) { band = 2; }
				else                             { band = 1; }

				//vga = RDA5815ReadReg(context, 0xb7);
				SemcoRDA_IIC_Read(context, 0xb7,&vga);
				vga_gain = vga*30/255;

				total_gain = RDA5815gain[band-1][gain_stage]/10 + vga_gain;

			break;

		case SAT1_MAIN_ADDRESS:  // DUAL 5816SW 1
		case SAT1_SUB_ADDRESS:   // DUAL 5816SW 2
					 if (stage_code >= RDA5816sw_Gain_Stage_14 ) { gain_stage = 14; }
/* Prevent fix CID 96986 */
#if 0
				else if (stage_code >= RDA5816sw_Gain_Stage_13 ) { gain_stage = 13; }
				else if (stage_code >= RDA5816sw_Gain_Stage_12 ) { gain_stage = 12; }
#endif
				else if (stage_code >= RDA5816sw_Gain_Stage_11 ) { gain_stage = 11; }
				else if (stage_code >= RDA5816sw_Gain_Stage_10 ) { gain_stage = 10; }
				else if (stage_code >= RDA5816sw_Gain_Stage__9 ) { gain_stage =  9; }
				else if (stage_code >= RDA5816sw_Gain_Stage__8 ) { gain_stage =  8; }
				else if (stage_code >= RDA5816sw_Gain_Stage__7 ) { gain_stage =  7; }
				else if (stage_code >= RDA5816sw_Gain_Stage__6 ) { gain_stage =  6; }
				else if (stage_code >= RDA5816sw_Gain_Stage__5 ) { gain_stage =  5; }
				else if (stage_code >= RDA5816sw_Gain_Stage__4 ) { gain_stage =  4; }
				else if (stage_code >= RDA5816sw_Gain_Stage__3 ) { gain_stage =  3; }
				else if (stage_code >= RDA5816sw_Gain_Stage__2 ) { gain_stage =  2; }
				else if (stage_code >= RDA5816sw_Gain_Stage__1 ) { gain_stage =  1; }
				else  { gain_stage =  0; }

	if ( context->RF < 950000 )           return 1;
	else if ( context->RF < 1000000 )			 {band =0;}
	else if ( context->RF < 1100000 )			 {band =1;}
	else if ( context->RF < 1200000 )			 {band =2;}
	else if ( context->RF < 1300000 )			 {band =3;}
	else if ( context->RF < 1400000 )			 {band =4;}
	else if ( context->RF < 1500000 )			 {band =5;}
	else if ( context->RF < 1600000 )			 {band =6;}
	else if ( context->RF < 1700000 )			 {band =7;}
	else if ( context->RF < 1800000 )			 {band =8;}
	else if ( context->RF < 1900000 )			 {band =9;}
	else if ( context->RF < 2000000 )			 {band =10;}
	else if ( context->RF < 2100000 )			 {band =11;}
	else if ( context->RF <= 2150000 )		     {band =12;}
	else                                        return 1;

				//vga = RDA5815ReadReg(context, 0xb7);
				SemcoRDA_IIC_Read(context, 0xb7,&vga);
				vga_gain = vga*30/255;

				total_gain = RDA5816swgain[band][gain_stage]/10 + vga_gain;
			break;

		default :
			break;
	}


  if (0) {
    SiTRACE("RDA stage_code %02x%02x %02x%02x %02x%02x -> stage_code 0x%06x\n", st1, pre, st2, post, i2v, filter, stage_code);
    SiTRACE("RDA st1 %d, pre %d, st2 %d, post %d, i2v %d, filter %d -> stage_code 0x%06x -> gain_stage %d\n", st1, pre, st2, post, i2v, filter, stage_code, gain_stage);
    SiTRACE("0x16: 0x%02X	0x17: 0x%02X	0xb7: 0x%02X\n", data16, data17, vga);
	SiTRACE("RDA vga %d -> vga_gain %d\n", vga, vga_gain);
    SiTRACE("RDA5815 RSSI = gain[%d][%d] + vga_gain = %f + %f = %f\n", band-1, gain_stage, RDA5815gain[band-1][gain_stage]/10, vga_gain, total_gain);
    SiTRACE("RDA5816sw RSSI = gain[%d][%d] + vga_gain = %f + %f = %f\n", band-1, gain_stage, RDA5816swgain[band-1][gain_stage]/10, vga_gain, total_gain);
	SiTRACE("RDA band: %d\n", band);
	SiTRACE("RDA Context: RF %d, IF %d, minRF %d, maxRF %d, LPF %d, Status %d\n",context->RF,context->IF,context->minRF,context->maxRF,context->LPF,context->Status);
  }

  return  -total_gain;
}

//csm
const unsigned char*         L1_RF_RDA_TAG_TEXT        (void) { return "TAGNAME"; }
//end