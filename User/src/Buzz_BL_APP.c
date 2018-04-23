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

uint8_t	   LCD_bl_ctrl_data;//���ݸñ������ò�ͬ�ı���ǿ��:ȡֵ0-4
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

//���ñ���ʱ����ر���
uint32_t Back_light_time_cnt;//��ǰ����
uint32_t Back_light_time;    //���õ�ʱ��
uint8_t Back_light_time_flag;//1=�򿪼�ʱ

//===============================================================================
//	finction	:Back_light_time_cnter
//	edit		:sam 2012.07.08
//	description	:LCD����ʱ���ʱ
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
//	description	:���ӵ�ǰ����ǿ��
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
//	description	:���ٵ�ǰ����ǿ��
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
//	description	:���ñ���ǿ����10s����ʱ��,ĳЩ�����²�������ʱ:����
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
        Back_light_time_flag=0;//�رձ����ʱ
    else if((Memu1_hand_step_status>0)&&(Memu1_hand_step_status!=0xff))//�ֶ�����
        Back_light_time_flag=0;
    else if((Memu1_jiaozheng_step_status>0)&&(Memu1_jiaozheng_step_status!=0xff))//У������
        Back_light_time_flag=0;
    else if((Memu1_biaoye_step_status>0)&&(Memu1_biaoye_step_status!=0xff))//��Һ����
        Back_light_time_flag=0;
    else if((Memu1_xishi_step_status>0)&&(Memu1_xishi_step_status!=0xff))//ϡ�Ͳ���
        Back_light_time_flag=0;
    else if((Memu1_auto_step_status>0)&&(Memu1_auto_step_status!=0xff))//�Զ�����
        Back_light_time_flag=0;
    else
    {
        Back_light_time_flag=1;//���������ʱ
        Back_light_time_cnt=0;
    }
}


//===============================================================================
//	finction	:Set_en_buzz_finc
//	input		:B_repeat_set		    //�ظ�����
//                       B_freq_set		//Ƶ������
//                       B_time_set_op  //��ʱ��
//                       B_time_set_cls //����ʱ��
//	edit		:sam 2012.06.18
//	description	:���÷��������ѷ�ʽ
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
//	description	:���������ƺ���,��TMR0_IRQHandler�ж�ʱ����
//===============================================================================
void Buzz_ctrl_finc(void)
{
    if(Buzz_en_flag)//�ж��Ƿ�ʹ���˷�����
    {
		//�жϵ�ǰ�ظ����� �Ƿ���� ���õ��ظ�����
        if(Buzz_repeat_cnt == Buzz_repeat_set)	//�������
        {
            Buzz_en_flag=0;   //ʧ�ܷ�����
            Buzz_repeat_cnt=0;//��ռ���
            DrvGPIO_ClrBit (E_GPE,5); //�رշ�����
        }
        else
        {
            ++Buzz_time_cnt;
            if(Buzz_signle_change_Flag)
            {
                if(Buzz_time_cnt == Buzz_time_set_op)	//�������ѵ�ʱ��
                {
                    Buzz_signle_change_Flag=0;
                    Buzz_time_cnt=0;
                    DrvGPIO_ClrBit (E_GPE,5);  			//�رշ�����
                }
                else
                {
                    if(Buzz_clk_change_Flag)			//Ƶ��
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
                if(Buzz_time_cnt==Buzz_time_set_cls)		//�����ʱ��
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




