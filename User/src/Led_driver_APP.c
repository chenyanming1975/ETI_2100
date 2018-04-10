//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Led_driver_APP.c
//	First EDIT		: Sam
//	Data			: 2012.06.12
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================
#define		__LED_APP__
//-------------------------------------------------------------------------------------------
#include "Incude_all.h"

uint8_t  Led_port_status;

//=============================================================================== 
//	finction	:Led_disp_data	:driver 595	595驱动函数
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.12
//	modefy		:null
//===============================================================================
void Led_disp_data(uint8_t Data)
{
    uint8_t i,j,temp;
    for(i=0;i<8;i++)
    {
        j=0x80>>i;
    	temp=Data&j;    	
    	if(temp)
    	DrvGPIO_SetBit (E_GPC,3);	//MOSI=HIGHT
    	else
    	DrvGPIO_ClrBit (E_GPC,3);	//MOSI=LOW
    	
    	DrvGPIO_ClrBit (E_GPC,1);	//SCLK=LOW	
    	__nop();
    	__nop();
    	DrvGPIO_SetBit (E_GPC,1);	//SCLK=HIGHT	
    	__nop();
    }	
    DrvGPIO_ClrBit (E_GPC,14);	//LCLK=LOW	
    __nop();
    __nop();
     DrvGPIO_SetBit (E_GPC,14);	//LCLK=HIGHT	
    __nop();
    __nop();
    DrvGPIO_ClrBit (E_GPC,14);	//LCLK=LOW	
    DrvGPIO_ClrBit (E_GPC,1);	//SCLK=LOW 
}

//=============================================================================== 
//	finction	:Led_status_show	LED状态显示控制函数
//      Detials		:null
//	input		:Data_flag
//	output		:null
//	return		:null
//	edit		:sam 2012-12-3 9:56
//	modefy		:null
//===============================================================================
uint8_t	Led_sec_cnt;
uint8_t	Led_data_flag;

void Led_status_show(void)
{
     uint8_t i;
     ++Led_sec_cnt;
     if(Led_sec_cnt>8) Led_sec_cnt=0;     
#ifdef __NEWPCB__
     i=Led_data_flag;
     i&=BIT0;
     if(i==0) DrvGPIO_SetBit (E_GPC,15);		//no err

     i=Led_data_flag;
     i&=BIT1;
     if(i==0) DrvGPIO_SetBit (E_GPC,14);		//no alarm

     i=Led_data_flag;
     i&=BIT2;
     if(i==0)  DrvGPIO_ClrBit (E_GPB,15);		//normal run

     i=Led_data_flag;
     i&=BIT3;
     if(i==0)  DrvGPIO_SetBit (E_GPB,8);		//no CAL

     if(Led_sec_cnt==4)
     {
        i=Led_data_flag;
        i&=BIT0;
        if(i!=0) DrvGPIO_ClrBit (E_GPC,15);

        i=Led_data_flag;
        i&=BIT1;
        if(i!=0) DrvGPIO_ClrBit (E_GPC,14);	

        i=Led_data_flag;
        i&=BIT2;
        if(i!=0) DrvGPIO_ClrBit (E_GPB,15);

        i=Led_data_flag;
        i&=BIT3;
        if(i!=0) DrvGPIO_ClrBit (E_GPB,8);     
     }
     else if(Led_sec_cnt==8)
     {
        i=Led_data_flag;
        i&=BIT0;
        if(i!=0) DrvGPIO_SetBit (E_GPC,15);

        i=Led_data_flag;
        i&=BIT1;
        if(i!=0) DrvGPIO_SetBit (E_GPC,14);

        i=Led_data_flag;
        i&=BIT2;
        if(i!=0) DrvGPIO_SetBit (E_GPB,15);

        i=Led_data_flag;
        i&=BIT3;
        if(i!=0) DrvGPIO_SetBit (E_GPB,8);
     	     	     	
     	Led_sec_cnt=0;
     }
#else
     i=Led_data_flag;
     i&=BIT0;
     if(i==0) Led_port_status&=~BIT1;						//no err

     i=Led_data_flag;
     i&=BIT1;
     if(i==0) Led_port_status&=~BIT2;	 					//no alarm

     i=Led_data_flag;
     i&=BIT2;
     if(i==0) Led_port_status|=BIT3;						//normal run

     i=Led_data_flag;
     i&=BIT3;
     if(i==0) Led_port_status&=~BIT4;						//no CAL

     if(Led_sec_cnt==4)
     {
        i=Led_data_flag;
        i&=BIT0;
        if(i!=0) Led_port_status|=BIT1;

        i=Led_data_flag;
        i&=BIT1;
        if(i!=0) Led_port_status|=BIT2;

        i=Led_data_flag;
        i&=BIT2;
        if(i!=0) Led_port_status|=BIT3;

        i=Led_data_flag;
        i&=BIT3;
        if(i!=0) Led_port_status|=BIT4;        
     }
     else if(Led_sec_cnt==8)
     {
        i=Led_data_flag;
        i&=BIT0;
        if(i!=0) Led_port_status&=~BIT1;

        i=Led_data_flag;
        i&=BIT1;
        if(i!=0) Led_port_status&=~BIT2;

        i=Led_data_flag;
        i&=BIT2;
        if(i!=0) Led_port_status&=~BIT3;

        i=Led_data_flag;
        i&=BIT3;
        if(i!=0) Led_port_status&=~BIT4;    
     	     	     	
     	Led_sec_cnt=0;
     }
     Led_disp_data(Led_port_status);
#endif          	
}



