/**
*FileName   : crc16.c
*Description:
*Refence doc:
**/

/**
HISTORY:
 V0.01    11.11.09     creation

**/
#include "platform.h"
#include "crc16.h"

int16u CRC16(int16u crc_gen,int8u *_data,int8u length)
{
	int16u result;
	int8u i,j;
	
	result=0xFFFF;
	if(length!=0)
	{
		for(i=0;i<length;i++)
		{
			result^=(int16u)(_data[i]);
			for(j=0;j<8;j++)
			{
				if((result&0x0001)==0x0001)
				{
					result>>=1;
					result^=crc_gen;//a001
				}
				else result>>=1;
			}
		}
	}
	return result;
}






