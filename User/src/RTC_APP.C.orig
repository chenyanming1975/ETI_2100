//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: I2C_APP.h	
//	First EDIT		: Sam
//	Data			: 2012.06.14
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================
#define		__RTC_APP__
//-------------------------------------------------------------------------------------------
#include "Incude_all.h"

//-------------------------------------------------------------------------------------------
uint8_t  System_heart;

volatile struct Rtc_str{
                 volatile uint8_t  sec_temp;
                 volatile uint8_t  minu_temp;
                 volatile uint8_t  hour_temp;
                 volatile uint8_t  day_temp;
                 volatile uint8_t  mouth_temp;
                 volatile uint8_t  year_temp;
                 volatile uint8_t  week_temp;

                 volatile uint8_t  alarm_minu_temp;
                 volatile uint8_t  alarm_hour_temp;
                 volatile uint8_t  alarm_day_temp;
                 volatile uint8_t  alarm_week_temp;
                 
                 volatile uint8_t  status1_temp;
                 volatile uint8_t  status2_temp;                 
              }RTC_DATA,RTC_Set;

//=============================================================================== 
//	finction	:RTC_read_1byte	
//	input		:null
//	output		:null
//	return		:data_temp
//	edit		:sam 2012.06.14
//	modefy		:null
//===============================================================================
uint8_t RTC_read_1byte(void)
{  
    uint8_t  data_temp=0xff;      

    DrvI2C_Open (I2C_PORT0, 100000);		//set freq 100khz
    
    DrvI2C_Ctrl(I2C_PORT0, 1, 0, 1, 0);		//clr si and set start
    while (I2C0->I2CON.SI == 0);		//poll si flag
    
    I2C0->I2CDAT = RTC_CMD_READ;		//write CMD read
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0);		//clr si
    while( I2C0->I2CON.SI == 0 );		//poll si flag      
         
    I2C0->I2CDAT = 0xff;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag
    
    data_temp = I2C0->I2CDAT;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag
    
    DrvI2C_Ctrl(I2C_PORT0, 0, 1, 1, 0);		//clr si and set stop
    
    DrvI2C_Close(I2C_PORT0);
    return(data_temp);
}

//=============================================================================== 
//	finction	:RTC_write_date		写时间参数给时钟芯片
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.19
//	modefy		:null
//===============================================================================
void RTC_write_date(void)
{
//    uint8_t j;
    uint32_t i;
    
    RTC_write_flag=1;
    
    DrvI2C_Open (I2C_PORT0, 100000);		//set freq 100khz
    
    DrvI2C_Ctrl(I2C_PORT0, 1, 0, 1, 0);		//clr si and set start
    while (I2C0->I2CON.SI == 0);		//poll si flag
    
    I2C0->I2CDAT = RTC_CMD_WRITE;		//write CMD read
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0);		//clr si flag
    while( I2C0->I2CON.SI == 0 );		//poll si flag
    
    //send word address 
    I2C0->I2CDAT = 0x00;		
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 	//clr si and set ack	
    while( I2C0->I2CON.SI == 0 );		//poll si flag
	
    //send data 
    I2C0->I2CDAT = 0x08;			//status1
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = 0x11;			//status2	//定时器中断脉冲
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = 0x00;			//sec
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = RTC_Set.minu_temp;		//minu
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
                                        	
    I2C0->I2CDAT = RTC_Set.hour_temp;		//hour
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = RTC_Set.day_temp;		//day	
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = RTC_Set.week_temp;		//week
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = RTC_Set.mouth_temp;		//munth
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
                                        	
    I2C0->I2CDAT = RTC_Set.year_temp;		//year
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );					
                                        	
    I2C0->I2CDAT = 0x00;			//alarm minu
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = 0x00;			//alarm hour
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 			
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = 0x00;			//alarm day
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
                                        	
    I2C0->I2CDAT = 0x00;			//alarm week
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 			
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = 0x01;			//clock out put ctrl
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = 0x81;			//timer ctrl
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
//    I2C0->I2CDAT = 0x40;			//timer data  64  1秒
    I2C0->I2CDAT = 0x20;			//timer data  32  0.5秒
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
                                        	
    DrvI2C_Ctrl(I2C_PORT0, 0, 1, 1, 0); 	//send stop	
    while( I2C0->I2CON.STO);
    
    for(i=0;i<60;i++);
    DrvI2C_Close(I2C_PORT0);
     	
    for(i=0;i<6000;i++); 
    
    RTC_write_flag=0;	

#ifdef __UART_TEST__
//    j=RTC_Set.mouth_temp;
//    j>>=4;
//    j&=0x01;
//    j*=10;
//    U2_P_mouth=j;
//    j=RTC_Set.mouth_temp;
//    j&=0x0f;
//    U2_P_mouth+=j;
//
//    j=RTC_Set.day_temp;
//    j>>=4;
//    j&=0x03;
//    j*=10;
//    U2_P_data=j;
//    j=RTC_Set.day_temp;
//    j&=0x0f;
//    U2_P_data+=j;
//
//    j=RTC_Set.hour_temp;
//    j>>=4;
//    j&=0x03;
//    j*=10;
//    U2_P_hour=j;
//    j=RTC_Set.hour_temp;
//    j&=0x0f;
//    U2_P_hour+=j;
//
//    j=RTC_Set.minu_temp;
//    j>>=4;
//    j&=0x07;
//    j*=10;
//    U2_P_minu=j;
    j=RTC_Set.minu_temp;
    j&=0x0f;
    U2_P_minu+=j;  
#endif		
}

//=============================================================================== 
//	finction	:RTC_read_12byte_data	读时钟芯片的时间
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.19
//	modefy		:null
//===============================================================================
void RTC_read_12byte_data(void)
{  
//    uint8_t j;
    uint32_t i;
    
    DrvI2C_Open (I2C_PORT0, 100000);		//set freq 100khz
    
    DrvI2C_Ctrl(I2C_PORT0, 1, 0, 1, 0);		//clr si and set start
    while (I2C0->I2CON.SI == 0);		//poll si flag
    
    I2C0->I2CDAT = RTC_CMD_WRITE;		//write CMD read
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0);		//clr si
    while( I2C0->I2CON.SI == 0 );		//poll si flag      
         
    I2C0->I2CDAT = 0x00;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag

    DrvI2C_Ctrl(I2C_PORT0, 1, 0, 1, 0);		//clr si and set start
    while (I2C0->I2CON.SI == 0);		//poll si flag
    
    I2C0->I2CDAT = RTC_CMD_READ;		//write CMD read
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0);		//clr si
    while( I2C0->I2CON.SI == 0 );		//poll si flag      
    
    RTC_DATA.status1_temp = I2C0->I2CDAT;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag

    RTC_DATA.status1_temp = I2C0->I2CDAT;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag

    RTC_DATA.status2_temp = I2C0->I2CDAT;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag

    RTC_DATA.sec_temp = I2C0->I2CDAT;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag

    RTC_DATA.minu_temp = I2C0->I2CDAT;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag

    RTC_DATA.hour_temp = I2C0->I2CDAT;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag

    RTC_DATA.day_temp = I2C0->I2CDAT;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag

    RTC_DATA.week_temp = I2C0->I2CDAT;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag

    RTC_DATA.mouth_temp = I2C0->I2CDAT;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag

    RTC_DATA.year_temp = I2C0->I2CDAT;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag

    RTC_DATA.alarm_minu_temp = I2C0->I2CDAT;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag

    RTC_DATA.alarm_hour_temp = I2C0->I2CDAT;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag
    
    RTC_DATA.alarm_day_temp = I2C0->I2CDAT;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag
    
    RTC_DATA.alarm_week_temp = I2C0->I2CDAT;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag

    i = I2C0->I2CDAT;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag

    i = I2C0->I2CDAT;
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 1);		//clr si and set ack
    while( I2C0->I2CON.SI == 0 );		//poll si flag
        
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
                                        	
    DrvI2C_Ctrl(I2C_PORT0, 0, 1, 1, 0); 	//send stop	
    while( I2C0->I2CON.STO);
    

    for(i=0;i<60;i++);
    DrvI2C_Close(I2C_PORT0);

//    j=RTC_DATA.mouth_temp;
//    j>>=4;
//    j&=0x01;
//    j*=10;
//    U2_P_mouth=j;
//    j=RTC_DATA.mouth_temp;
//    j&=0x0f;
//    U2_P_mouth+=j;
//
//    j=RTC_DATA.day_temp;
//    j>>=4;
//    j&=0x03;
//    j*=10;
//    U2_P_data=j;
//    j=RTC_DATA.day_temp;
//    j&=0x0f;
//    U2_P_data+=j;
//
//    j=RTC_DATA.hour_temp;
//    j>>=4;
//    j&=0x03;
//    j*=10;
//    U2_P_hour=j;
//    j=RTC_DATA.hour_temp;
//    j&=0x0f;
//    U2_P_hour+=j;       
}

//=============================================================================== 
//	finction	:EINT0Callback		外部中断1函数--心跳LED和其他LED的控制还有时间的显示刷新
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.19
//	modefy		:null
//===============================================================================
void EINT0Callback(void)
{
//    uint8_t j;

    GPIOB->ISRC  &=~(1UL << 14);			//clear int flag

    //clear TF flag or not???
//    if(Back_light_time_flag==0)
//       DrvGPIO_ClrBit (E_GPB,11);
//    else
//       DrvGPIO_SetBit (E_GPB,11);
       
//    Rtc_int_flag=0x01;
    if((RTC_write_flag!=1)||(Auto_check_clock_flag==1))
       RTC_read_12byte_data();
       
    Led_status_show();

    if(System_heart!=0)
    {
    	System_heart=0;   
        GPIOA->DMASK=0X0000FBFF;
        GPIOA->DOUT=0X0000FBFF;
        GPIOA->DMASK=0X00000000;        
        if((LCD_disp_flag==0)&&(Prog_disp_flag==0))
          Disp_rtc_clock();
        else if((LCD_disp_flag==0)&&(Prog_disp_flag==2)&&(Menu_lever_select==0))
          Disp_rtc_clock();
    }
    else
    {
        System_heart=1;    
        GPIOA->DMASK=0X0000FBFF;
        GPIOA->DOUT=~0X0000FBFF;
        GPIOA->DMASK=0X00000000;        
        if((LCD_disp_flag==0)&&(Menu_lever_select==0)&&(Prog_disp_flag==0))
           Display_6x8_char_single(7,12,0);
        else if((LCD_disp_flag==0)&&(Prog_disp_flag==2)&&(Menu_lever_select==0))
           Display_6x8_char_single(7,12,0);
/////        else if(Auto_check_clock_flag==1)
/////           Display_6x8_char_single(7,12,0);
    }    
}

//=============================================================================== 
//	finction	:RTC_initial		初始化时钟芯片
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.14
//	modefy		:null
//===============================================================================
void RTC_initial(void)		
{    
//    uint8_t j;
    uint32_t i;

    DrvI2C_Open (I2C_PORT0, 100000);		//set freq 100khz
    
    DrvI2C_Ctrl(I2C_PORT0, 1, 0, 1, 0);		//clr si and set start
    while (I2C0->I2CON.SI == 0);		//poll si flag
    
    I2C0->I2CDAT = RTC_CMD_WRITE;		//write CMD write
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0);		//clr si flag
    while( I2C0->I2CON.SI == 0 );		//poll si flag
    
    //send word address 
    I2C0->I2CDAT = 0x00;		
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 	//clr si and set ack	
    while( I2C0->I2CON.SI == 0 );		//poll si flag
	
    //send 16buye data 
    I2C0->I2CDAT = 0x08;			//status1
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = 0x11;			//status2	//定时器中断脉冲
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = RTC_Set.sec_temp;			//sec
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = RTC_Set.minu_temp;		//minu
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
                                        	
    I2C0->I2CDAT = RTC_Set.hour_temp;		//hour
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = RTC_Set.day_temp;		//day	
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = RTC_Set.week_temp;		//week
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = RTC_Set.mouth_temp;		//munth
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
                                        	
    I2C0->I2CDAT = RTC_Set.year_temp;		//year
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = RTC_Set.alarm_minu_temp;			//alarm minu
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = RTC_Set.alarm_hour_temp;			//alarm hour
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 			
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = RTC_Set.alarm_day_temp;			//alarm day
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
                                        	
    I2C0->I2CDAT = RTC_Set.alarm_week_temp;			//alarm week
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 			
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = 0x01;			//clock out put ctrl
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
    I2C0->I2CDAT = 0x81;			//timer ctrl
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
//    I2C0->I2CDAT = 0x40;			//timer data  64  1秒
    I2C0->I2CDAT = 0x20;			//timer data  32  0.5秒
    DrvI2C_Ctrl(I2C_PORT0, 0, 0, 1, 0); 		
    while( I2C0->I2CON.SI == 0 );			
                                        	
    DrvI2C_Ctrl(I2C_PORT0, 0, 1, 1, 0); 	//send stop	
    while( I2C0->I2CON.STO);
    
    for(i=0;i<60;i++);
    DrvI2C_Close(I2C_PORT0);
     	
    for(i=0;i<6000;i++);
    for(i=0;i<6000;i++);   
    
    RTC_write_flag=0;
    	
    DrvGPIO_EnableEINT0 (E_IO_FALLING,E_MODE_EDGE,EINT0Callback); 	

#ifdef __UART_TEST__
//    j=RTC_Set.mouth_temp;
//    j>>=4;
//    j&=0x01;
//    j*=10;
//    U2_P_mouth=j;
//    j=RTC_Set.mouth_temp;
//    j&=0x0f;
//    U2_P_mouth+=j;
//
//    j=RTC_Set.day_temp;
//    j>>=4;
//    j&=0x03;
//    j*=10;
//    U2_P_data=j;
//    j=RTC_Set.day_temp;
//    j&=0x0f;
//    U2_P_data+=j;
//
//    j=RTC_Set.hour_temp;
//    j>>=4;
//    j&=0x03;
//    j*=10;
//    U2_P_hour=j;
//    j=RTC_Set.hour_temp;
//    j&=0x0f;
//    U2_P_hour+=j;
//
//    j=RTC_Set.minu_temp;
//    j>>=4;
//    j&=0x07;
//    j*=10;
//    U2_P_minu=j;
    j=RTC_Set.minu_temp;
    j&=0x0f;
    U2_P_minu+=j;   
#endif    
//    Rtc_int_flag=0x01; 
}



