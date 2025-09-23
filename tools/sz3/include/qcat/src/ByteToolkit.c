/**
 *  @file ByteToolkit.c
 *  @author Sheng Di
 *  @date April, 2016
 *  @brief Byte Toolkit
 *  (C) 2016 by Mathematics and Computer Science (MCS), Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include <ByteToolkit.h>
#include <rw.h>
#include <stdlib.h>
#include "qcat.h"
#include <math.h>

inline unsigned short bytesToUInt16_bigEndian(unsigned char* bytes)
{
	int temp = 0;
	unsigned short res = 0;
	
	temp = bytes[0] & 0xff;
	res |= temp;	

	res <<= 8;
	temp = bytes[1] & 0xff;
	res |= temp;
	
	return res;
}	
inline unsigned int bytesToUInt32_bigEndian(unsigned char* bytes)
{
	unsigned int temp = 0;
	unsigned int res = 0;
	
	res <<= 8;
	temp = bytes[0] & 0xff;
	res |= temp;	

	res <<= 8;
	temp = bytes[1] & 0xff;
	res |= temp;

	res <<= 8;
	temp = bytes[2] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = bytes[3] & 0xff;
	res |= temp;
	
	return res;
}

inline unsigned long bytesToUInt64_bigEndian(unsigned char* b) {
	unsigned long temp = 0;
	unsigned long res = 0;

	res <<= 8;
	temp = b[0] & 0xff;
	res |= temp;

	res <<= 8;
	temp = b[1] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = b[2] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = b[3] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = b[4] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = b[5] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = b[6] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = b[7] & 0xff;
	res |= temp;						
	
	return res;
}
	
inline short bytesToInt16_bigEndian(unsigned char* bytes)
{
	int temp = 0;
	short res = 0;
	
	temp = bytes[0] & 0xff;
	res |= temp;	

	res <<= 8;
	temp = bytes[1] & 0xff;
	res |= temp;
	
	return res;
}	
	
inline int bytesToInt32_bigEndian(unsigned char* bytes)
{
	int temp = 0;
	int res = 0;
	
	res <<= 8;
	temp = bytes[0] & 0xff;
	res |= temp;	

	res <<= 8;
	temp = bytes[1] & 0xff;
	res |= temp;

	res <<= 8;
	temp = bytes[2] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = bytes[3] & 0xff;
	res |= temp;
	
	return res;
}

inline long bytesToInt64_bigEndian(unsigned char* b) {
	long temp = 0;
	long res = 0;

	res <<= 8;
	temp = b[0] & 0xff;
	res |= temp;

	res <<= 8;
	temp = b[1] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = b[2] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = b[3] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = b[4] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = b[5] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = b[6] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = b[7] & 0xff;
	res |= temp;						
	
	return res;
}

inline int bytesToInt_bigEndian(unsigned char* bytes)
{
	int temp = 0;
	int res = 0;
	
	res <<= 8;
	temp = bytes[0] & 0xff;
	res |= temp;	

	res <<= 8;
	temp = bytes[1] & 0xff;
	res |= temp;

	res <<= 8;
	temp = bytes[2] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = bytes[3] & 0xff;
	res |= temp;
	
	return res;
}

/**
 * @unsigned char *b the variable to store the converted bytes (length=4)
 * @unsigned int num
 * */
inline void intToBytes_bigEndian(unsigned char *b, unsigned int num)
{
	b[0] = (unsigned char)(num >> 24);	
	b[1] = (unsigned char)(num >> 16);	
	b[2] = (unsigned char)(num >> 8);	
	b[3] = (unsigned char)(num);	
	
	//note: num >> xxx already considered endian_type...
//if(dataEndianType==LITTLE_ENDIAN_DATA)
//		symTransform_4bytes(*b); //change to BIG_ENDIAN_DATA
}

inline void int64ToBytes_bigEndian(unsigned char *b, uint64_t num)
{
	b[0] = (unsigned char)(num>>56);
	b[1] = (unsigned char)(num>>48);
	b[2] = (unsigned char)(num>>40);
	b[3] = (unsigned char)(num>>32);
	b[4] = (unsigned char)(num>>24);
	b[5] = (unsigned char)(num>>16);
	b[6] = (unsigned char)(num>>8);
	b[7] = (unsigned char)(num);
}

inline void int32ToBytes_bigEndian(unsigned char *b, uint32_t num)
{
	b[0] = (unsigned char)(num >> 24);	
	b[1] = (unsigned char)(num >> 16);	
	b[2] = (unsigned char)(num >> 8);	
	b[3] = (unsigned char)(num);		
}

inline void int16ToBytes_bigEndian(unsigned char *b, uint16_t num)
{
	b[0] = (unsigned char)(num >> 8);	
	b[1] = (unsigned char)(num);
}

/**
 * @endianType: refers to the endian_type of unsigned char* b.
 * */
inline long bytesToLong_bigEndian(unsigned char* b) {
	long temp = 0;
	long res = 0;

	res <<= 8;
	temp = b[0] & 0xff;
	res |= temp;

	res <<= 8;
	temp = b[1] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = b[2] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = b[3] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = b[4] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = b[5] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = b[6] & 0xff;
	res |= temp;
	
	res <<= 8;
	temp = b[7] & 0xff;
	res |= temp;						
	
	return res;
}

inline void longToBytes_bigEndian(unsigned char *b, unsigned long num) 
{
	b[0] = (unsigned char)(num>>56);
	b[1] = (unsigned char)(num>>48);
	b[2] = (unsigned char)(num>>40);
	b[3] = (unsigned char)(num>>32);
	b[4] = (unsigned char)(num>>24);
	b[5] = (unsigned char)(num>>16);
	b[6] = (unsigned char)(num>>8);
	b[7] = (unsigned char)(num);
//	if(dataEndianType==LITTLE_ENDIAN_DATA)
//		symTransform_8bytes(*b);
}


inline long doubleToOSEndianLong(double value)
{
	ldouble buf;
	buf.value = value;
	return buf.lvalue;
}

inline int floatToOSEndianInt(float value)
{
	lfloat buf;
	buf.value = value;
	return buf.ivalue;
}

//TODO: debug: lfBuf.lvalue could be actually little_endian....
inline short getExponent_float(float value)
{
	//int ivalue = floatToBigEndianInt(value);

	lfloat lbuf;
	lbuf.value = value;
	int ivalue = lbuf.ivalue;
	
	int expValue = (ivalue & 0x7F800000) >> 23;
	expValue -= 127;
	return (short)expValue;
}

inline short getPrecisionReqLength_float(float precision)
{
	lfloat lbuf;
	lbuf.value = precision;
	int ivalue = lbuf.ivalue;
	
	int expValue = (ivalue & 0x7F800000) >> 23;
	expValue -= 127;
//	unsigned char the1stManBit = (unsigned char)((ivalue & 0x00400000) >> 22);
//	if(the1stManBit==1)
//		expValue--;	
	return (short)expValue;
}

inline short getExponent_double(double value)
{
	//long lvalue = doubleToBigEndianLong(value);
	
	ldouble lbuf;
	lbuf.value = value;
	long lvalue = lbuf.lvalue;
	
	int expValue = (int)((lvalue & 0x7FF0000000000000) >> 52);
	expValue -= 1023;
	return (short)expValue;
}

short getPrecisionReqLength_double(double precision)
{
	ldouble lbuf;
	lbuf.value = precision;
	long lvalue = lbuf.lvalue;
	
	int expValue = (int)((lvalue & 0x7FF0000000000000) >> 52);
	expValue -= 1023;
//	unsigned char the1stManBit = (unsigned char)((lvalue & 0x0008000000000000) >> 51);
//	if(the1stManBit==1)
//		expValue--;
	return (short)expValue;
}

unsigned char numberOfLeadingZeros_Int(int i) {
	if (i == 0)
		return 32;
	unsigned char n = 1;
	if (((unsigned int)i) >> 16 == 0) { n += 16; i <<= 16; }
	if (((unsigned int)i) >> 24 == 0) { n +=  8; i <<=  8; }
	if (((unsigned int)i) >> 28 == 0) { n +=  4; i <<=  4; }
	if (((unsigned int)i) >> 30 == 0) { n +=  2; i <<=  2; }
	n -= ((unsigned int)i) >> 31;
	return n;
}

unsigned char numberOfLeadingZeros_Long(long i) {
	 if (i == 0)
		return 64;
	unsigned char n = 1;
	int x = (int)(((unsigned long)i) >> 32);
	if (x == 0) { n += 32; x = (int)i; }
	if (((unsigned int)x) >> 16 == 0) { n += 16; x <<= 16; }
	if (((unsigned int)x) >> 24 == 0) { n +=  8; x <<=  8; }
	if (((unsigned int)x) >> 28 == 0) { n +=  4; x <<=  4; }
	if (((unsigned int)x) >> 30 == 0) { n +=  2; x <<=  2; }
	n -= ((unsigned int)x) >> 31;
	return n;
}

unsigned char getLeadingNumbers_Int(int v1, int v2)
{
	int v = v1 ^ v2;
	return (unsigned char)numberOfLeadingZeros_Int(v);
}

unsigned char getLeadingNumbers_Long(long v1, long v2)
{
	long v = v1 ^ v2;
	return (unsigned char)numberOfLeadingZeros_Long(v);
}

/**
 * By default, the endian type is OS endian type.
 * */
short bytesToShort(unsigned char* bytes)
{
	lint16 buf;
	memcpy(buf.byte, bytes, 2);
	
	return buf.svalue;
}

void shortToBytes(unsigned char* b, short value)
{
	lint16 buf;
	buf.svalue = value;
	memcpy(b, buf.byte, 2);
}

int bytesToInt(unsigned char* bytes)
{
	lfloat buf;
	memcpy(buf.byte, bytes, 4);
	return buf.ivalue;
}

long bytesToLong(unsigned char* bytes)
{
	ldouble buf;
	memcpy(buf.byte, bytes, 8);
	return buf.lvalue;
}

//the byte to input is in the big-endian format
inline float bytesToFloat(unsigned char* bytes)
{
	lfloat buf;
	memcpy(buf.byte, bytes, 4);
	if(sysEndianType==LITTLE_ENDIAN_SYSTEM)
		symTransform_4bytes(buf.byte);	
	return buf.value;
}

inline void floatToBytes(unsigned char *b, float num)
{
	lfloat buf;
	buf.value = num;
	memcpy(b, buf.byte, 4);
	if(sysEndianType==LITTLE_ENDIAN_SYSTEM)
		symTransform_4bytes(b);		
}

//the byte to input is in the big-endian format
inline double bytesToDouble(unsigned char* bytes)
{
	ldouble buf;
	memcpy(buf.byte, bytes, 8);
	if(sysEndianType==LITTLE_ENDIAN_SYSTEM)
		symTransform_8bytes(buf.byte);
	return buf.value;
}

inline void doubleToBytes(unsigned char *b, double num)
{
	ldouble buf;
	buf.value = num;
	memcpy(b, buf.byte, 8);
	if(sysEndianType==LITTLE_ENDIAN_SYSTEM)
		symTransform_8bytes(b);
}

inline int getMaskRightCode(int m) {
	switch (m) {
	case 1:
		return 0x01;
	case 2:
		return 0x03;
	case 3:
		return 0x07;
	case 4:
		return 0x0F;
	case 5:
		return 0x1F;
	case 6:
		return 0x3F;
	case 7:
		return 0X7F;
	case 8:
		return 0XFF;
	default:
		return 0;
	}
}

inline int getLeftMovingCode(int kMod8)
{
	return getMaskRightCode(8 - kMod8);
}

inline int getRightMovingSteps(int kMod8, int resiBitLength) {
	return 8 - kMod8 - resiBitLength;
}

inline int getRightMovingCode(int kMod8, int resiBitLength)
{
	int rightMovingSteps = 8 - kMod8 - resiBitLength;
	if(rightMovingSteps < 0)
	{
		switch(-rightMovingSteps)
		{
		case 1:
			return 0x80;
		case 2:
			return 0xC0;
		case 3:
			return 0xE0;
		case 4:
			return 0xF0;
		case 5:
			return 0xF8;
		case 6:
			return 0xFC;
		case 7:
			return 0XFE;
		default:
			return 0;
		}    		
	}
	else //if(rightMovingSteps >= 0)
	{
		int a = getMaskRightCode(8 - kMod8);
		int b = getMaskRightCode(8 - kMod8 - resiBitLength);
		int c = a - b;
		return c;
	}
}

short* convertByteDataToShortArray(unsigned char* bytes, size_t byteLength)
{
	lint16 ls;
	size_t i, stateLength = byteLength/2;
	short* states = (short*)malloc(stateLength*sizeof(short));
	if(sysEndianType==dataEndianType)
	{	
		for(i=0;i<stateLength;i++)
		{
			ls.byte[0] = bytes[i*2];
			ls.byte[1] = bytes[i*2+1];
			states[i] = ls.svalue;
		}
	}
	else
	{
		for(i=0;i<stateLength;i++)
		{
			ls.byte[0] = bytes[i*2+1];
			ls.byte[1] = bytes[i*2];
			states[i] = ls.svalue;
		}		
	}
	return states;
} 

unsigned short* convertByteDataToUShortArray(unsigned char* bytes, size_t byteLength)
{
	lint16 ls;
	size_t i, stateLength = byteLength/2;
	unsigned short* states = (unsigned short*)malloc(stateLength*sizeof(unsigned short));
	if(sysEndianType==dataEndianType)
	{	
		for(i=0;i<stateLength;i++)
		{
			ls.byte[0] = bytes[i*2];
			ls.byte[1] = bytes[i*2+1];
			states[i] = ls.usvalue;
		}
	}
	else
	{
		for(i=0;i<stateLength;i++)
		{
			ls.byte[0] = bytes[i*2+1];
			ls.byte[1] = bytes[i*2];
			states[i] = ls.usvalue;
		}		
	}
	return states;
} 

void convertShortArrayToBytes(short* states, size_t stateLength, unsigned char* bytes)
{
	lint16 ls;
	size_t i;
	if(sysEndianType==dataEndianType)
	{
		for(i=0;i<stateLength;i++)
		{
			ls.svalue = states[i];
			bytes[i*2] = ls.byte[0];
			bytes[i*2+1] = ls.byte[1];
		}		
	}
	else
	{
		for(i=0;i<stateLength;i++)
		{
			ls.svalue = states[i];
			bytes[i*2] = ls.byte[1];
			bytes[i*2+1] = ls.byte[0];
		}			
	}
}

void convertUShortArrayToBytes(unsigned short* states, size_t stateLength, unsigned char* bytes)
{
	lint16 ls;
	size_t i;
	if(sysEndianType==dataEndianType)
	{
		for(i=0;i<stateLength;i++)
		{
			ls.usvalue = states[i];
			bytes[i*2] = ls.byte[0];
			bytes[i*2+1] = ls.byte[1];
		}		
	}
	else
	{
		for(i=0;i<stateLength;i++)
		{
			ls.usvalue = states[i];
			bytes[i*2] = ls.byte[1];
			bytes[i*2+1] = ls.byte[0];
		}			
	}
}

void convertIntArrayToBytes(int* states, size_t stateLength, unsigned char* bytes)
{
	lint32 ls;
	size_t index = 0;
	size_t i;
	if(sysEndianType==dataEndianType)
	{
		for(i=0;i<stateLength;i++)
		{
			index = i << 2; //==i*4
			ls.ivalue = states[i];
			bytes[index] = ls.byte[0];
			bytes[index+1] = ls.byte[1];
			bytes[index+2] = ls.byte[2];
			bytes[index+3] = ls.byte[3];
		}		
	}
	else
	{
		for(i=0;i<stateLength;i++)
		{
			index = i << 2; //==i*4
			ls.ivalue = states[i];
			bytes[index] = ls.byte[3];
			bytes[index+1] = ls.byte[2];
			bytes[index+2] = ls.byte[1];
			bytes[index+3] = ls.byte[0];
		}			
	}
}

void convertUIntArrayToBytes(unsigned int* states, size_t stateLength, unsigned char* bytes)
{
	lint32 ls;
	size_t index = 0;
	size_t i;
	if(sysEndianType==dataEndianType)
	{
		for(i=0;i<stateLength;i++)
		{
			index = i << 2; //==i*4
			ls.uivalue = states[i];
			bytes[index] = ls.byte[0];
			bytes[index+1] = ls.byte[1];
			bytes[index+2] = ls.byte[2];
			bytes[index+3] = ls.byte[3];
		}		
	}
	else
	{
		for(i=0;i<stateLength;i++)
		{
			index = i << 2; //==i*4
			ls.uivalue = states[i];
			bytes[index] = ls.byte[3];
			bytes[index+1] = ls.byte[2];
			bytes[index+2] = ls.byte[1];
			bytes[index+3] = ls.byte[0];
		}			
	}
}

void convertLongArrayToBytes(int64_t* states, size_t stateLength, unsigned char* bytes)
{
	lint64 ls;
	size_t index = 0;
	size_t i;
	if(sysEndianType==dataEndianType)
	{
		for(i=0;i<stateLength;i++)
		{
			index = i << 3; //==i*8
			ls.lvalue = states[i];
			bytes[index] = ls.byte[0];
			bytes[index+1] = ls.byte[1];
			bytes[index+2] = ls.byte[2];
			bytes[index+3] = ls.byte[3];
			bytes[index+4] = ls.byte[4];
			bytes[index+5] = ls.byte[5];
			bytes[index+6] = ls.byte[6];
			bytes[index+7] = ls.byte[7];	
		}		
	}
	else
	{
		for(i=0;i<stateLength;i++)
		{
			index = i << 3; //==i*8
			ls.lvalue = states[i];
			bytes[index] = ls.byte[7];
			bytes[index+1] = ls.byte[6];
			bytes[index+2] = ls.byte[5];
			bytes[index+3] = ls.byte[4];
			bytes[index+4] = ls.byte[3];
			bytes[index+5] = ls.byte[2];
			bytes[index+6] = ls.byte[1];
			bytes[index+7] = ls.byte[0];	
		}			
	}
}

void convertULongArrayToBytes(uint64_t* states, size_t stateLength, unsigned char* bytes)
{
	lint64 ls;
	size_t index = 0;
	size_t i;
	if(sysEndianType==dataEndianType)
	{
		for(i=0;i<stateLength;i++)
		{
			index = i << 3; //==i*8
			ls.ulvalue = states[i];
			bytes[index] = ls.byte[0];
			bytes[index+1] = ls.byte[1];
			bytes[index+2] = ls.byte[2];
			bytes[index+3] = ls.byte[3];
			bytes[index+4] = ls.byte[4];
			bytes[index+5] = ls.byte[5];
			bytes[index+6] = ls.byte[6];
			bytes[index+7] = ls.byte[7];			
		}		
	}
	else
	{
		for(i=0;i<stateLength;i++)
		{
			index = i << 3; //==i*8
			ls.ulvalue = states[i];
			bytes[index] = ls.byte[7];
			bytes[index+1] = ls.byte[6];
			bytes[index+2] = ls.byte[5];
			bytes[index+3] = ls.byte[4];
			bytes[index+4] = ls.byte[3];
			bytes[index+5] = ls.byte[2];
			bytes[index+6] = ls.byte[1];
			bytes[index+7] = ls.byte[0];	
		}			
	}
}


/**
 * put 'buf_nbBits' bits represented by buf into a long byte stream (the current output byte pointer is p, where offset is the number of bits already filled out for this byte so far)
 * */
void put_codes_to_output(unsigned int buf, int bitSize, unsigned char** p, int* lackBits, size_t *outSize)
{
	int byteSize, byteSizep;
	if(*lackBits == 0)
	{
		byteSize = bitSize%8==0 ? bitSize/8 : bitSize/8+1; //it's equal to the number of bytes involved (for *outSize)
		byteSizep = bitSize >> 3; //it's used to move the pointer p for next data
		intToBytes_bigEndian(*p, buf);
		(*p) += byteSizep;
		*outSize += byteSize;
		(*lackBits) = bitSize%8==0 ? 0 : 8 - bitSize%8;
	}
	else
	{
		**p = (**p) | (unsigned char)(buf >> (32 - *lackBits));
		if((*lackBits) < bitSize)
		{
			(*p)++;
			int newCode = buf << (*lackBits);
			intToBytes_bigEndian(*p, newCode);
			bitSize -= *lackBits;
			byteSizep = bitSize >> 3; // =bitSize/8
			byteSize = bitSize%8==0 ? byteSizep : byteSizep+1;
			*p += byteSizep;
			(*outSize)+=byteSize;
			(*lackBits) = bitSize%8==0 ? 0 : 8 - bitSize%8;
		}
		else
		{
			(*lackBits) -= bitSize;
			if(*lackBits==0)
				(*p)++;
		}
	}
}



void valuateData(const char* oriFilePath, const char* decFilePath, long* shape,struct compResult* result){
	/*
	 * compare the content of two float files
	 * with the metric of:
	   PSNR SSIM RMSE MAX MIN RANGE
	*/
	int x = 1;
	int status = 0;
	size_t r4 = 0, r3 = 0, r2 = 0, r1 = 0;
    r4 = shape[3];
    r3 = shape[2];
    r2 = shape[1];
    r1 = shape[0];
    char *y = (char*)&x;
        if(*y==1)
    {     
	 sysEndianType = 0; //LITTLE_ENDIAN_SYSTEM;
    }
    else //=0
    {
      sysEndianType = 1; //BIG_ENDIAN_SYSTEM;
    }
    float* errors = NULL;
	float *ori_data = NULL, *dec_data = NULL;
	size_t nbEle = 0, nbEle2 = 0;
    ori_data = readFloatData(oriFilePath, &nbEle, &status);
		
    dec_data = readFloatData(decFilePath, &nbEle2, &status);
	if(nbEle != nbEle2)
	{
		printf("Error: number of elements is not consistent\n");
		return;
	}
	size_t i = 0;
	double Max = 0, Min = 0, diffMax = 0;
	Max = ori_data[0];
	Min = ori_data[0];
	diffMax = dec_data[0]>ori_data[0]?dec_data[0]-ori_data[0]:ori_data[0]-dec_data[0];
    double sum1 = 0, sum2 = 0, sum22 = 0;
    for (i = 0; i < nbEle; i++)
	{
		sum1 += ori_data[i];
		sum2 += dec_data[i];
		sum22 += dec_data[i]*dec_data[i];
	}	
	double mean1 = sum1/nbEle;
	double mean2 = sum2/nbEle;
    double maxpw_relerr = 0; 
	double sum3 = 0, sum4 = 0;
	double sum = 0, prodSum = 0, relerr = 0;
    for (i = 0; i < nbEle; i++)
	{
		if (Max < ori_data[i]) Max = ori_data[i];
		if (Min > ori_data[i]) Min = ori_data[i];
		float err_ = dec_data[i] - ori_data[i];
		float err = fabs(err_);
		if(ori_data[i]!=0)
		{
			relerr = err/fabs(ori_data[i]);
			if(maxpw_relerr<relerr)
				maxpw_relerr = relerr;
		}

		if (diffMax < err)
			diffMax = err;
		prodSum += (ori_data[i]-mean1)*(dec_data[i]-mean2);
		sum3 += (ori_data[i] - mean1)*(ori_data[i]-mean1);
		sum4 += (dec_data[i] - mean2)*(dec_data[i]-mean2);
		sum += err*err;	
	}
	double std1 = sqrt(sum3/nbEle);
	double std2 = sqrt(sum4/nbEle);
	double ee = prodSum/nbEle;
	double acEff = ee/std1/std2;

	double mse = sum/nbEle;
	double range = Max - Min;
	double psnr = 20*log10(range)-10*log10(mse);
	double normErr = sqrt(sum);
	double normErr_norm = normErr/sqrt(sum22);
	double nrmse = sqrt(mse)/range;
    double ssim = calculateSSIM(ori_data, dec_data, QCAT_FLOAT, r4, r3, r2, r1);
    
	printf ("Min = %.20G, Max = %.20G, range = %.20G\n", Min, Max, range);
	printf ("Max absolute error = %.10f\n", diffMax);
	printf ("Max relative error = %f\n", diffMax/(Max-Min));
	//printf ("Max pw relative error = %f\n", maxpw_relerr);
	printf ("PSNR = %f, NRMSE = %.20G\n", psnr,nrmse);
	//printf ("normErr = %f, normErr_norm = %f\n", normErr, normErr_norm);
	//printf ("pearson coeff = %f\n", acEff);
	printf ("SSIM = %f\n", ssim);
    /*
	FILE *file = fopen(csvPath, "a");
    fprintf(file, "%.20G,%.20G,%.20G,%.10f,%f,%f,%f,%.20G,%f,%f,%f,%f\n",
             Min, Max, range, diffMax, diffMax / (Max - Min), maxpw_relerr, psnr, nrmse, normErr, normErr_norm, acEff, ssim);
    fclose(file);
	*/
	result->Min=Min;
	result->Max=Max;
	result->Range=range;
	result->MaxAbsoluteError=diffMax;
	result->MaxRelativeError=diffMax/(Max-Min);
	result->MaxPWRelativeError=maxpw_relerr;
	result->PSNR=psnr;
	result->NRMSE=nrmse;
	result->NormErr=normErr;
	result->NormErrNorm=normErr_norm;
	result->PearsonCoeff=acEff;
	result->SSIM=ssim;
	if (ori_data != NULL) free(ori_data);
    if (dec_data != NULL) free(dec_data);
}
