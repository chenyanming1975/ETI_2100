//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: memory.h
//	First EDIT		: Sam
//	Data			: 2012.06.08
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================

//============================================================================================
extern void Memory_Init(uchar *pRamEnd);
//extern void Memory_Destory(void);
extern uchar *Memory_Malloc(uchar mSize);
extern void Memory_Memset(uchar *pRam,uchar mChar,uchar mLen);

//extern void Memory_Init(uchar *pRamEnd);



