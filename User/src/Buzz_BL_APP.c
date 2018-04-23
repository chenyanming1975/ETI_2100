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

uint8_t	   LCD_bl_ctrl_data;//根据该变量设置不同的背光强度:取值0-4
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

//设置背光时间相关变量
uint32_t Back_light_time_cnt;//当前计数
uint32_t Back_light_time;    //设置的时间
uint8_t Back_light_time_flag;//1=打开计时

//===============================================================================
//	finction	:Back_light_time_cnter
//	edit		:sam 2012.07.08
//	description	:LCD背光时间计时
//===============================================================================
void Back_light_time_cnter(void)
{
    if(Back_light_time_flag)
    {
        ++Back_light_time_cnt;
        if(Back_light_time_cnt == Back_light_time)
        {
            Back_light_time_flag=0;
            Back_light_time_cnt=0;

            PWMB->CMR0=0x0000;			//backlight.
        }
    }
}

//===============================================================================
//	finction	:Set_back_light_up
//	edit		:sam 2012.06.18
//	description	:增加当前背光强度
//===============================================================================
void Set_back_light_up(void)		
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
//	edit		:sam 2012.06.18
//	description	:减少当前背光强度
//===============================================================================
void Set_back_light_down(void)
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
//	edit		:sam 2012-10-15 16:04
//	description	:设置背光强度与10s背光时间,某些步骤下不开启计时:常亮
//===============================================================================
void Set_back_light_10sec(void)	
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
    if((Hand_finction_select>0) && (Hand_finction_select<=8))
        Back_light_time_flag=0;//关闭背光计时
    else if((Memu1_hand_step_status>0)&&(Memu1_hand_step_status!=0xff))//手动步骤
        Back_light_time_flag=0;
    else if((Memu1_jiaozheng_step_status>0)&&(Memu1_jiaozheng_step_status!=0xff))//校正步骤
        Back_light_time_flag=0;
    else if((Memu1_biaoye_step_status>0)&&(Memu1_biaoye_step_status!=0xff))//标液步骤
        Back_light_time_flag=0;
    else if((Memu1_xishi_step_status>0)&&(Memu1_xishi_step_status!=0xff))//稀释步骤
        Back_light_time_flag=0;
    else if((Memu1_auto_step_status>0)&&(Memu1_auto_step_status!=0xff))//自动步骤
        Back_light_time_flag=0;
    else
    {
        Back_light_time_flag=1;//开启背光计时
        Back_light_time_cnt=0;
    }
}


//===============================================================================
//	finction	:Set_en_buzz_finc
//	input		:B_repeat_set		    //重复次数
//                       B_freq_set		//频率设置
//                       B_time_set_op  //响时间
//                       B_time_set_cls //不响时间
//	edit		:sam 2012.06.18
//	description	:设置蜂鸣器鸣笛方式
//===============================================================================
void Set_en_buzz_finc(uint8_t B_repeat_set,uint8_t B_freq_set,uint32_t B_time_set_op,uint32_t B_time_set_cls)
{
    Buzz_repeat_cnt=0;
    Buzz_time_cnt=0;
    Buzz_freq_cnt=0;

    Buzz_repeat_set   = B_repeat_set;
    Buzz_freq_set     = B_freq_set;
    Buzz_time_set_op  = B_time_set_op*8;
    Buzz_time_set_cls = B_time_set_cls*8;

    Buzz_signle_change_Flag = 1;
    Buzz_clk_change_Flag    = 1;
    Buzz_en_flag            = 1;
}

//===============================================================================
//	finction	:Buzz_ctrl_finc
//	edit		:sam 2012.06.18
//	description	:蜂鸣器控制函数,在TMR0_IRQHandler中定时调用
//===============================================================================
void Buzz_ctrl_finc(void)
{
    if(Buzz_en_flag)//判断是否使能了蜂鸣器
    {
		//判断当前重复次数 是否等于 设置的重复次数
        if(Buzz_repeat_cnt == Buzz_repeat_set)	//鸣笛完成
        {
            Buzz_en_flag=0;   //失能蜂鸣器
            Buzz_repeat_cnt=0;//清空计数
            DrvGPIO_ClrBit (E_GPE,5); //关闭蜂鸣器
        }
        else
        {
            ++Buzz_time_cnt;
            if(Buzz_signle_change_Flag)
            {
                if(Buzz_time_cnt == Buzz_time_set_op)	//连续鸣笛的时间
                {
                    Buzz_signle_change_Flag=0;
                    Buzz_time_cnt=0;
                    DrvGPIO_ClrBit (E_GPE,5);  			//关闭蜂鸣器
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




