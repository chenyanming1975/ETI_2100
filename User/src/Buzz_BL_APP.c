//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Buzz_BL_APP.c	
//	First EDIT		: Sam
//	Data			: 2012.06.18
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================
#define		__BUZZ_BL_APP__
//-------------------------------------------------------------------------------------------
#include "Incude_all.h"

uint8_t	   LCD_bl_ctrl_data;
uint8_t	   LCD_bl_duty_cnt;
uint8_t    Buzz_en_flag;

static uint8_t    Buzz_signle_change_Flag;
static uint8_t    Buzz_clk_change_Flag;
static uint8_t    Buzz_repeat_cnt;
static uint32_t   Buzz_time_cnt;
static uint8_t    Buzz_freq_cnt;
static uint8_t    Buzz_repeat_set;
static uint32_t   Buzz_time_set_op;
static uint32_t   Buzz_time_set_cls;
static uint8_t    Buzz_freq_set;

uint32_t Back_light_time_cnt;
uint32_t Back_light_time;
uint8_t Back_light_time_flag;

//=============================================================================== 
//	finction	:Back_light_time_cnter
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.07.08
//	modefy		:null
//===============================================================================
void Back_light_time_cnter(void)		//LCD背光时间计时
{    
    if(Back_light_time_flag)
    {
       ++Back_light_time_cnt;
       if(Back_light_time_cnt==Back_light_time)
       {
       	  Back_light_time_flag=0;
       	  Back_light_time_cnt=0;

//          PWMB->PCR.CH0EN=0;			//close backlight.  
          PWMB->CMR0=0x0000;			//backlight.          			       	  
       }
    }
}

//=============================================================================== 
//	finction	:Set_back_light_up
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.18
//	modefy		:null
//===============================================================================
void Set_back_light_up(void)		//LCD背光亮度选择
{
    if(LCD_bl_ctrl_data<4)
    {
       ++LCD_bl_ctrl_data;
       switch(LCD_bl_ctrl_data)
       {
       	  case 0:
              PWMB->CMR0=0x0000;			//backlight.            
       	  break;
       	  case 1:
              PWMB->CMR0=0x0080;			//backlight.            
       	  break;
       	  case 2:
              PWMB->CMR0=0x00FF;			//backlight.            
       	  break;
       	  case 3:
              PWMB->CMR0=0x0180;			//backlight.            
       	  break;
       	  case 4:
              PWMB->CMR0=0x01FF;			//backlight.            
       	  break;       	  
       }
    }
}

//=============================================================================== 
//	finction	:Set_back_light_down
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.18
//	modefy		:null
//===============================================================================
void Set_back_light_down(void)		//LCD背光亮度选择
{
    if(LCD_bl_ctrl_data!=0)
    {
       --LCD_bl_ctrl_data;
       switch(LCD_bl_ctrl_data)
       {
       	  case 0:
              PWMB->CMR0=0x0000;			//backlight.            
       	  break;
       	  case 1:
              PWMB->CMR0=0x0080;			//backlight.            
       	  break;
       	  case 2:
              PWMB->CMR0=0x00FF;			//backlight.            
       	  break;
       	  case 3:
              PWMB->CMR0=0x0180;			//backlight.            
       	  break;
       	  case 4:
              PWMB->CMR0=0x01FF;			//backlight.            
       	  break;       	  
       }
    }
}

//=============================================================================== 
//	finction	:Set_back_light_10sec
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-10-15 16:04
//	modefy		:null
//===============================================================================
void Set_back_light_10sec(void)		//设置LCD背光10秒
{
    switch(LCD_bl_ctrl_data)
    {
    	case 0:
         PWMB->CMR0=0x0001;			//backlight.            
    	break;
    	case 1:
         PWMB->CMR0=0x0080;			//backlight.            
    	break;
    	case 2:
         PWMB->CMR0=0x00FF;			//backlight.            
    	break;
    	case 3:
         PWMB->CMR0=0x0180;			//backlight.            
    	break;
    	case 4:
         PWMB->CMR0=0x01FE;			//backlight.            
    	break;       	  
    }

    PWMB->PCR.CH0EN=1;
    if((Hand_finction_select>0)&&(Hand_finction_select<=8))
        Back_light_time_flag=0;
    else if((Memu1_hand_step_status>0)&&(Memu1_hand_step_status!=0xff))
        Back_light_time_flag=0;
    else if((Memu1_jiaozheng_step_status>0)&&(Memu1_jiaozheng_step_status!=0xff))
        Back_light_time_flag=0;
    else if((Memu1_biaoye_step_status>0)&&(Memu1_biaoye_step_status!=0xff))
        Back_light_time_flag=0;
    else if((Memu1_xishi_step_status>0)&&(Memu1_xishi_step_status!=0xff))
        Back_light_time_flag=0;
    else if((Memu1_auto_step_status>0)&&(Memu1_auto_step_status!=0xff))
        Back_light_time_flag=0;                
    else
    {
        Back_light_time_flag=1;
        Back_light_time_cnt=0;	
    }
}

//=============================================================================== 
//	finction	:Back_light_ctrl_finc
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.18
//	modefy		:null
//===============================================================================
//void Back_light_ctrl_finc(void)
//{
//    uint32_t	i;
//    
//    if(Back_light_time_flag)
//    {
//       if(LCD_bl_duty_cnt==80)
//          LCD_bl_duty_cnt=0;
//
//       i=GPIOB->DMASK;
//       i&=0x0000f7ff;
//       GPIOB->DMASK=i;
//
//       switch(LCD_bl_ctrl_data)
//       {
//          case 0:
//              PWMB->CMR0=0x0001;			//close backlight.            
//          break;
//          case 4:
//              PWMB->CMR0=0x01FE;			//open backlight.            
//          break;
//          case 1:
//              ++LCD_bl_duty_cnt;
//              if(LCD_bl_duty_cnt<20)
//                 DrvGPIO_SetBit (E_GPB,11);			//open bl           
//              else
//                 DrvGPIO_ClrBit (E_GPB,11);			//close bl           
//          break;
//          case 2:
//              ++LCD_bl_duty_cnt;
//              if(LCD_bl_duty_cnt<40)
//                 DrvGPIO_SetBit (E_GPB,11);			//open bl           
//              else
//                 DrvGPIO_ClrBit (E_GPB,11);			//close bl           
//          break;
//          case 3:
//              ++LCD_bl_duty_cnt;
//              if(LCD_bl_duty_cnt<60)
//                 DrvGPIO_SetBit (E_GPB,11);			//open bl           
//              else
//                 DrvGPIO_ClrBit (E_GPB,11);			//close bl           
//          break;
//       }
//    }
////    else DrvGPIO_ClrBit (E_GPB,11);			//close bl     
//}

//=============================================================================== 
//	finction	:Set_en_buzz_finc
//	input		:B_repeat_set		//重复次数
//                       B_freq_set		//频率设置
//                       B_time_set_op          //响时间
//                       B_time_set_cls		//不响时间
//	output		:null
//	return		:null
//	edit		:sam 2012.06.18
//	modefy		:null
//===============================================================================
void Set_en_buzz_finc(uint8_t B_repeat_set,uint8_t B_freq_set,uint32_t B_time_set_op,uint32_t B_time_set_cls)
{	
	Buzz_repeat_cnt=0;         
	Buzz_time_cnt=0;           
	Buzz_freq_cnt=0;
	           
	Buzz_repeat_set=B_repeat_set;         
	Buzz_freq_set=B_freq_set;           	
	Buzz_time_set_op=B_time_set_op*8;        
	Buzz_time_set_cls=B_time_set_cls*8;  

	Buzz_signle_change_Flag=1; 
	Buzz_clk_change_Flag=1;  	  	     
	Buzz_en_flag=1;            
}

//=============================================================================== 
//	finction	:Buzz_ctrl_finc
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.18
//	modefy		:null
//===============================================================================
void Buzz_ctrl_finc(void)
{
    if(Buzz_en_flag)
    {
    	if(Buzz_repeat_cnt==Buzz_repeat_set)			//重复次数
    	{
    	    Buzz_en_flag=0;
    	    Buzz_repeat_cnt=0;
    	    DrvGPIO_ClrBit (E_GPE,5);  				//close buzz	
    	}
    	else
    	{
   	    ++Buzz_time_cnt;
    	    if(Buzz_signle_change_Flag)
    	    {
     	    	if(Buzz_time_cnt==Buzz_time_set_op)		//响的时间
    	    	{
    	    	   Buzz_signle_change_Flag=0;
    	    	   Buzz_time_cnt=0;
    	    	   DrvGPIO_ClrBit (E_GPE,5);  			//close buzz
    	    	}
    	    	else
    	    	{
    	    	   if(Buzz_clk_change_Flag)			//频率
    	    	   {
    	    	       DrvGPIO_SetBit (E_GPE,5);  		//open buzz 
    	    	       ++Buzz_freq_cnt;
    	    	       if(Buzz_freq_cnt==Buzz_freq_set)
    	    	       {
    	    	       	   Buzz_clk_change_Flag=0;
    	    	       	   Buzz_freq_cnt=0;
    	    	       }	
    	    	   }
    	    	   else
    	    	   {
    	    	       DrvGPIO_ClrBit (E_GPE,5);  		//close buzz
    	    	       ++Buzz_freq_cnt;
    	    	       if(Buzz_freq_cnt==Buzz_freq_set)
    	    	       {
    	    	       	   Buzz_clk_change_Flag=1;
    	    	       	   Buzz_freq_cnt=0;
    	    	       }	
    	    	   }	
    	    	}    	    	
    	    }
    	    else
    	    {    	    	    	    	
    	    	if(Buzz_time_cnt==Buzz_time_set_cls)		//间隔的时间
    	    	{
    	    	   Buzz_time_cnt=0;
    	    	   Buzz_freq_cnt=0;
    	    	   Buzz_clk_change_Flag=1;
    	    	   Buzz_signle_change_Flag=1;
    	    	   ++Buzz_repeat_cnt;
    	    	}
    	    }	
    	}
    }
}




