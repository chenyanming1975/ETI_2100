//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: message.h
//	First EDIT		: Sam
//	Data			: 2012.06.08
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================
#define Msg_Buff_Size 10
//=======================================================================================
//message define£¬end at eMSG_End.
//enum MSG_HEAD
//{
// eMsg_PreProcess,       //MenuNewsProcess
// eMsg_LagProcess,       //
//
////#include "User_Message.txt"
// eMSG_End,
//};
//----------------------------------------------------------
struct Message
{
uchar mWord;       
uchar *aMsgBufStart;    
uchar *aMsgBufEnd;      
uchar *pMsgIn;
uchar *pMsgOut;
};
//-------------------------------------------------------------------------------
#ifdef _MessageH
struct Message sInMsg;
#else
extern void  Msg_Init(Uint8 mMsgBuffSize);
//extern void  Msg_Destory(void);

extern uchar Msg_Pop(void);  
//extern void  Msg_Cancel(uchar i);  
     
extern void  Msg_Push(uchar mMsg);  
extern void  Msg_ISR_Push(uchar mMsg);      
//extern void  Msg_Push_nByte(uchar i,j,k);

extern void  Msg_Loop(void);   

extern struct Message sInMsg;        
#endif


