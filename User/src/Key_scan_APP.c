//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Key_scan_APP.c
//	First EDIT		: Sam
//	Data			: 2012.06.13
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================
#define		__KEY_APP__

/////#define		__OLD_NTC__
/////#define		__OLD_JBSPEED__

//-------------------------------------------------------------------------------------------
#include "Incude_all.h"
//-------------------------------------------------------------------------------------------
uint8_t Machine_note_use;					//�ӻ��ڵ�
uint16_t Machine_note_temp;					//�ӻ��ڵ�����

uint8_t	Key_code_bak;
uint8_t	Key_scan_time_cnt;

volatile uint32_t E_wait_time_temp;				//2012-9-14 13:48
volatile uint32_t Prj_correlat;					//2012-9-14 13:48

uint32_t Allow_error;						//�������Χ2012-11-7 15:18
uint32_t Alarm_hight_nd;					//��Ũ�ȱ���ֵ2012-11-8 11:03
uint32_t Alarm_low_nd;						//��Ũ�ȱ���ֵ2012-11-8 11:03
uint8_t	 Alarm_active_flag;					//������־2012-11-8 11:03

uint8_t	 Export_data_flag;					//2012-9-7 13:47

uint8_t  DCF_switch_select_flag;				//���Ŀ��ر�־2012-7-18 15:04
volatile uint8_t  BCWD_NG_flag;					//�����¶ȱ�־2012-8-2 9:31

volatile uint8_t Menu_lever4_bit_temp;
uint8_t Menu_lever_select;
uint8_t Menu_index_select;
uint8_t Menu_index_finc_select;
uint8_t Menu_lever1_point;
uint8_t Menu_lever2_point;
uint8_t Menu_lever3_point;
uint8_t Menu_lever3_select;

volatile uint8_t Menu_lever3_start_flag;			//2012-7-18 9:31

uint8_t Menu_lever3_PoPa;
uint8_t Menu_lever3_PoSeg;

uint8_t Menu_lever4_PoPa;
uint8_t Menu_lever4_PoSeg;

uint8_t Menu_lever3_Pub_select_cnt;				//for position 2012-7-6 9:14
uint8_t Menu_lever3_Pub_adj_cnt;				//2012-7-6 14:31

uint8_t Menu_lever4_Pub_select_cnt;				//for position 2012-7-6 9:14
uint8_t Menu_lever4_Pub_adj_cnt;				//2012-7-6 14:31

//struct Menu_reg{
//              };
volatile uint8_t  	M_dongtai_flag;				//��̬��־
volatile uint8_t  	M_menur1_lcd_Contrast_temp;		//LCD��Ӱ΢��
volatile uint8_t  	M_menur1_language_select_temp;		//����ѡ��
volatile uint8_t  	M_menur1_analy_obj_select_temp;		//1:NH4-N 2:PO4 3:Cu 4:CN
volatile uint8_t  	M_menur1_unit_select_temp;		//��λѡ��
volatile uint8_t  	M_menur1_dot_num_select_temp;		//С����λ��ѡ��
volatile uint8_t  	M_menur1_ZDXS_select_temp;		//�Զ�ϡ��ѡ��
volatile uint8_t  	M_menur1_LZDJ_select_temp;		//���ӵ��� 2012-7-10 16:35
volatile uint8_t  	M_menur1_DL_select_temp;		//�������� 2012-7-10 16:36
volatile uint8_t  	M_menur1_direction_select_temp;		//���跽�� 2012-7-10 16:38
volatile uint16_t 	M_menur1_led_data_temp;			//����
volatile uint32_t 	M_menur1_lange_data_temp;		//����ѡ��
volatile uint32_t 	M_menur1_led_dianliu_temp;		//LED��������
volatile uint32_t 	M_menur1_JB_speed_temp;			//�����ٶ�
volatile uint32_t 	M_menur1_ZSB_speed_temp;		//ע����ٶ�
volatile uint32_t 	M_menur1_sample_v_temp;			//��Ʒ���

volatile uint32_t 	M_menur1_slope_temp;			//��ǰʹ�õ�б��
volatile uint8_t	M_menur1_slope_flag;			//��ǰʹ�õ�б�ʷ���
volatile uint8_t	Updata_slope_flag;			//��ǰʹ��б��������±�־

volatile uint8_t	Disple_flag;				//�Ƿ������־

volatile uint32_t 	M_menur1_jz1nd_temp;			//��Һ1Ũ��
volatile uint32_t 	M_menur1_jz1_v_temp;                    //��Һ1���
volatile uint32_t 	M_menur1_jz2nd_temp;                    //��Һ2Ũ��
volatile uint32_t 	M_menur1_jz2_v_temp;                    //��Һ2���

volatile uint32_t 	M_menur1_sj1_v_temp;                    //�Լ�1���
volatile uint32_t 	M_menur1_sj2_v_temp;                    //�Լ�2���
volatile uint32_t 	M_menur1_sj3_v_temp;                    //�Լ�3���
volatile uint32_t 	M_menur1_sj4_v_temp;                    //�Լ�4���

volatile uint32_t  	M_menur1_FY_temp;			//У�����̵ĳ��죬����ϵ�����������Ա���
volatile uint32_t  	M_menur1_Fx_temp;               	//
volatile uint32_t  	M_menur1_Fa_temp;               	//
volatile uint32_t  	M_menur1_Fb_temp;               	//
volatile uint32_t  	M_menur1_Fc_temp;               	//
volatile uint32_t  	M_menur1_Fd_temp;               	//

volatile uint8_t	 M_menur1_RX_time_temp;			//��ϴ����
volatile uint32_t	 M_menur1_QYSJ_temp;            	//ȡ��ʱ��
volatile uint32_t        M_menur1_CSSJ_temp;            	//����ʱ��
volatile uint32_t        M_menur1_XHPY_temp;            	//�ź�Ư��
volatile uint32_t        M_menur1_DDSJ_temp;            	//�ȴ�ʱ��

volatile uint32_t        M_lever4_FXPL_temp;            	//����Ƶ��
volatile uint32_t        M_lever4_JZPL_temp;            	//У��Ƶ��
volatile uint32_t        M_lever4_QXPL_temp;            	//��ϴƵ��
volatile uint32_t        M_lever4_HCPL_temp;            	//�˲�Ƶ��

volatile uint8_t	M_lever4_extern_trrig_flag;		//�ⲿ������־
volatile uint8_t	M_lever4_start_year_temp;		//2014-1-15 10:09 add
volatile uint8_t	M_lever4_start_hour_temp;		//
volatile uint8_t	M_lever4_start_mini_temp;       	//
volatile uint8_t	M_lever4_start_munth_temp;      	//
volatile uint8_t	M_lever4_start_day_temp;        	//
volatile uint8_t	M_lever4_FXCS_temp;             	//��������
volatile uint32_t	M_lever4_BYND_temp;             	//��ҺŨ��
volatile uint32_t	M_lever4_move_to_temp;          	//ע����ƶ���XXλ��
volatile uint32_t	M_lever4_WD_temp;               	//�¶�ֵ
volatile uint16_t	M_lever4_BCWD_temp;             	//�����¶�ֵ
volatile uint32_t	M_lever4_GDGDY_temp;            	//���ܵ�ѹ
volatile uint32_t	M_lever4_PZDY_temp;             	//ƫ�õ�ѹ
volatile uint32_t	M_lever4_PZDYZ_temp;            	//ƫ�õ�ѹֵ
volatile uint32_t	M_lever4_CFZ_temp;              	//���ֵ

uint32_t	M_lever4_Out1_4ma_temp;				//�����ǽ����4~20ma����ļĴ���
uint32_t	M_lever4_Out1_20ma_temp;                        //
uint32_t	M_lever4_Out1_test_ma_temp;                     //
uint32_t	M_lever4_Out2_4ma_temp;                         //
uint32_t	M_lever4_Out2_20ma_temp;                        //
uint32_t	M_lever4_Out2_test_ma_temp;                     //

uint32_t	M_lever4_tp_Out1_4ma_temp;                      //�������¶ȵ�4~20ma����ļĴ���
uint32_t	M_lever4_tp_Out1_20ma_temp;                     //
uint32_t	M_lever4_tp_Out1_test_ma_temp;                  //
uint32_t	M_lever4_tp_Out2_4ma_temp;                      //
uint32_t	M_lever4_tp_Out2_20ma_temp;                     //
uint32_t	M_lever4_tp_Out2_test_ma_temp;                  //

uint32_t	M_lever4_jz_Out1_4ma_temp;                      //������У�������4~20ma����ļĴ���
uint32_t	M_lever4_jz_Out1_20ma_temp;                     //
uint32_t	M_lever4_jz_Out1_test_ma_temp;                  //
uint32_t	M_lever4_jz_Out2_4ma_temp;                      //
uint32_t	M_lever4_jz_Out2_20ma_temp;                     //
uint32_t	M_lever4_jz_Out2_test_ma_temp;                  //

volatile uint8_t	M_lever3_BYHC_temp;			//��Һ�˲� 2012-7-25 11:52 add
volatile uint32_t	M_lever3_HCPD_temp;			//�˲��ж� 2012-7-25 11:53 add

//uint16_t 	M_menur3_battery_all_set_temp;
//uint16_t	M_menur3_direction_temp;
volatile uint16_t 	M_menur3_switch_temp;
volatile uint16_t 	M_menur3_ISE_temp;

volatile uint8_t  	Hand_finction_select;
volatile uint16_t 	Hand_finction_wait_cnt;		//bit16 for flag
volatile uint8_t  	Hand_finction_step_select;

volatile uint32_t	set_cal_data;			//2012-12-28 14:58 add

void Menu_lever3_4_enter_finction(void);

void Key_Menu_lever3_up_finc(void);
void Key_Menu_lever3_down_finc(void);
void Lever4_Select_finc_adds(void);
void Key_Menu_lever4_down_finc(void);		//2012-7-11 16:05
void Key_Menu_lever4_up_finc(void);		//2012-7-11 16:05
uint8_t Lever4_Select_en_flag(void);		//2012-7-17 14:02

void Uart2_reinitial(void);			//2012-8-13 17:03

//===============================================================================
//	finction	:Shift_165_Data	:driver 165
//	input		:null
//	output		:null
//	return		:IO_data_temp	:IO bit data
//	edit		:sam 2012.06.13
//	modefy		:null
//===============================================================================
int32_t Shift_165_Data(void)			//����оƬ����
{
    uint32_t IO_data_temp=0;

    DrvGPIO_ClrBit (E_GPC,1);	//SCLK=LOW
    DrvSYS_Delay(500);
    DrvGPIO_SetBit (E_GPC,1);	//SCLK=hight
    DrvSYS_Delay(1000);
    IO_data_temp = DrvGPIO_GetBit (E_GPC, 2);
    DrvSYS_Delay(500);
    return (IO_data_temp);
}
//===============================================================================
//	finction	:Read_IC_165_Data	:driver 165
//	input		:null
//	output		:null
//	return		:data_165   key code
//	edit		:sam 2012.06.13
//	modefy		:null
//===============================================================================
int32_t Read_IC_165_Data(void)			//������оƬ�ļ�ֵ
{
    uint32_t i,j=0,data_165=0;

    DrvGPIO_Open (E_GPC,1, E_IO_OUTPUT);	//SPI0 SCK
    DrvGPIO_Open (E_GPC,2, E_IO_INPUT);	//SPI0 MISO
    DrvGPIO_Open (E_GPC,3, E_IO_OUTPUT);	//SPI0 MOSI

    DrvGPIO_ClrBit (E_GPC,7);	//Set 165 Pin CSIN=low

#ifndef __NEWPCB__
    DrvGPIO_ClrBit (E_GPC,15);	//Set 165 Pin LOAD=low
#else
    DrvGPIO_ClrBit (E_GPC,6);	//Set 165 Pin LOAD=low
#endif
    DrvSYS_Delay(500);
#ifndef __NEWPCB__
    DrvGPIO_SetBit (E_GPC,15);	//Set 165 Pin LOAD=hight
#else
    DrvGPIO_SetBit (E_GPC,6);	//Set 165 Pin LOAD=hight
#endif
    DrvSYS_Delay(500);
    for(i=0; i<7; i++)
    {
        j= Shift_165_Data();
        data_165+=j;
        data_165=data_165<<1;
    }
    data_165=data_165>>1;
    DrvGPIO_SetBit (E_GPC,7);	//Set 165 Pin CSIN=hight
    return(data_165);
}

//===============================================================================
//	finction	:Enter_return_disp
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-19 14:00
//	modefy		:null
//===============================================================================
void Enter_return_disp(void)			//��ȷ�ϼ�����ʾ����
{
    if(Check_pass_word_flag==1)			//2012-10-23 15:56
        Clead_pass_word_reg_data();
    else
    {
        if((Menu_lever_select>=3)&&(Key_code_bak!=KEY_CODE_ESC))
        {
            if(Menu_lever_select==3)
            {
                if((Menu_index_select==2)&&(Menu_index_finc_select==1))
                    M_menur1_analy_obj_select_temp=Menu_lever3_Pub_select_cnt;
                if((Menu_index_select==2)&&(Menu_index_finc_select==3))
                    M_menur1_unit_select_temp=Menu_lever3_Pub_select_cnt;
                if((Menu_index_select==2)&&(Menu_index_finc_select==11))
                    M_menur1_ZDXS_select_temp=Menu_lever3_Pub_select_cnt;
                if((Menu_index_select==2)&&(Menu_index_finc_select==17))
                    Updata_slope_flag=Menu_lever3_Pub_select_cnt;

                if((Menu_index_select==2)&&(Menu_index_finc_select==18))		//2014-1-26 10:28
                    Disple_flag=Menu_lever3_Pub_select_cnt;

                if((Menu_index_select==3)&&(Menu_index_finc_select==1))
                {
                    M_menur1_LZDJ_select_temp=Menu_lever3_Pub_select_cnt;
                    M_menur1_DL_select_temp=Menu_lever3_Pub_select_cnt;

                    if(M_menur1_LZDJ_select_temp==1)
                    {
                        M_menur1_slope_temp=0x00005960;
                        M_menur1_slope_flag=2;
                    }
                    else
                    {
                        M_menur1_slope_temp=0x00000100;
                        M_menur1_slope_flag=1;
                    }
                }
                if((Menu_index_select==3)&&(Menu_index_finc_select==2))
                    M_menur1_DL_select_temp=Menu_lever3_Pub_select_cnt;
                if((Menu_index_select==3)&&(Menu_index_finc_select==6))				//���跽��
                    M_menur1_direction_select_temp=Menu_lever3_Pub_select_cnt;
                if((Menu_index_select==3)&&(Menu_index_finc_select==8))
                    M_menur1_dot_num_select_temp=Menu_lever3_Pub_select_cnt;
                if((Menu_index_select==3)&&(Menu_index_finc_select==13))
                {
                    if(Menu_lever3_Pub_select_cnt==4) M_dongtai_flag=1;
                    else                              M_dongtai_flag=0;
                }
                if((Menu_index_select==7)&&(Menu_index_finc_select==3))
                    //M_menur1_language_select_temp=Menu_lever3_Pub_select_cnt;	//2016/4/20 14:00
                    M_menur1_language_select_temp=2;
            }
            else if(Menu_lever_select==4)
            {
                if((Menu_index_select==1)&&(Menu_index_finc_select==1)&&(Menu_lever3_select==1))	//�ⲿ����
                {
                    if(Menu_lever4_Pub_select_cnt==5) M_lever4_extern_trrig_flag=1;
                    else                              M_lever4_extern_trrig_flag=0;
                }
                if((Menu_index_select==4)&&(Menu_index_finc_select==5)&&(Menu_lever3_select==2))	//���跽��
                    M_menur1_direction_select_temp=Menu_lever4_Pub_select_cnt;
                if((Menu_index_select==6)&&(Menu_index_finc_select==1)&&(Menu_lever3_select==2))
                {
                    Uart2_BAUD_select_temp=Menu_lever4_Pub_select_cnt;
                    Uart2_reinitial();
                }
                if((Menu_index_select==6)&&(Menu_index_finc_select==1)&&(Menu_lever3_select==3))
                    Uart2_JOJY_select_temp=Menu_lever4_Pub_select_cnt;
                if((Menu_index_select==6)&&(Menu_index_finc_select==1)&&(Menu_lever3_select==4))
                    Uart2_STOP_select_temp=Menu_lever4_Pub_select_cnt;

            }
        }
        if((Menu_lever_select==3)&&(Menu_index_select==7)&&(Menu_index_finc_select==3))
        {
            if(Key_code_bak==KEY_CODE_ESC) --Menu_lever_select;
            else
            {
                Menu_lever_select=0;
                Menu_index_finc_select=1;
                Menu_lever2_point=1;
            }
        }
        else
            --Menu_lever_select;
    }


    Write_system_reg_to_chip();

    Pwm_menu_lever=0;
    Pwm_menu_select=0;
    Pwm_menu_select1=0;
    Pwm_select_cnt=0;
    Pwm_adj_cnt=0;
    Pwm_menu_bit_temp=0;
    Pwm_menu_PoSeg=0;
    Pwm_menu_PoSeg_pub=0;

    switch(Menu_lever_select)
    {
    case 3:
        //clear lever4 ram
        Menu_lever4_Pub_select_cnt=1;
        Menu_lever4_bit_temp=0;
        Menu_lever4_Pub_adj_cnt=0;
        Disp_gdgdy_flag=0;
//    	       Pro_wait_time_flag=0;
        DCF_switch_select_flag=0;
        break;
    case 2:
        Menu_lever3_point=1;
        Menu_lever3_select=1;
        Menu_lever3_Pub_select_cnt=1;
        Fenxi.disp_index=0;
        Jiaozheng.disp_index=0;
        Baojing.disp_index=0;
        Cuowu.disp_index=0;
////    	       prg_wait_clr_flag=0;
        break;
    case 1:
        Menu_lever2_point=1;
        Menu_index_finc_select=1;
        break;
    case 0:
        Menu_index_select=1;
        Menu_lever1_point=1;
        LCD_disp_flag=0;
        break;
    }
    DrvSYS_Delay(200000);				//delay 200ms
    Display_menu_index();
}

//===============================================================================
//	finction	:Key_scan_bounce_time
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.14
//	modefy		:null
//===============================================================================
void Key_scan_bounce_time(void)
{
    if(Key_scan_time_cnt&&0x80)
    {
        --Key_scan_time_cnt;
        if(Key_scan_time_cnt==0x80)
            Key_scan_time_cnt=0;
    }
}

//===============================================================================
//	finction	:Get_lv2menu_nun
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.14
//	modefy		:null
//===============================================================================
uint8_t Get_lv2menu_nun(void)
{
    uint8_t i=0;
    switch(Menu_index_select)
    {
    case 1:
        i=MENU_LV21_NUM;
        break;
    case 2:
        i=MENU_LV22_NUM;
        break;
    case 3:
        if(fix_axbc_falg==0)
            i=MENU_LV23_NUM;
        else
            i=MENU_LV23_NUM-2;
        break;
    case 4:
        i=MENU_LV24_NUM;
        break;
    case 5:
        i=MENU_LV25_NUM;
        break;
    case 6:
        i=MENU_LV26_NUM;
        break;
    case 7:
        i=MENU_LV27_NUM;
        break;
    }
    return(i);
}

//===============================================================================
//	finction	:Key_scan_prosecce	:key code from 165
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.14
//	modefy		:null
//===============================================================================
void Key_scan_prosecce(void)
{
    uint32_t Key_code_temp,Pub_temp,disp_d,rds;
    uint8_t i,m_buff[16];
    float a,b,c,d,rs;

    if(Key_scan_time_cnt==0)
    {
        Key_scan_time_cnt=0x86;			// key bounce time

        Key_code_temp=Read_IC_165_Data(); 	//get key code
        Key_code_temp&=0x7f;
        if(Key_code_bak!=Key_code_temp)
        {
            Key_code_bak=Key_code_temp;
            switch(Key_code_temp)
            {
            case KEY_CODE_UP:
                Set_back_light_10sec();
                Set_en_buzz_finc(1,4,200,100);
                if(Prog_disp_flag!=1)			//�Զ������д˰�����Ч
                {
                    if(Check_pass_word_flag==1)		//�Ƿ���������
                    {
                        --Pass_adj_cnt;
                        if(Pass_adj_cnt>=10)
                            Pass_adj_cnt=9;

                        Pass_adj_disp(Pass_adj_cnt,Pass_bit_select);
                    }
                    else
                    {
                        if(Menu_lever_select==0x01)		//��һ���˵�
                        {
                            if(M_menur1_language_select_temp==2)  i=4;
                            else i=6;

                            if(Menu_lever1_point<i)
                            {
                                ++Menu_lever1_point;
                                ++Menu_index_select;
                                Display_menu_index();
                            }
                            else if((Menu_index_select<MENU_LV1_NUM)&&(Menu_lever1_point==i))
                            {
                                ++Menu_index_select;
                                Display_menu_index();
                            }
                            else
                            {
                                Menu_lever1_point=Menu_index_select=1;
                                Display_menu_index();
                            }
                        }
                        if(Menu_lever_select==0x02)		//�ڶ����˵�
                        {
                            if(M_menur1_language_select_temp==2)  i=4;	//����4��
                            else i=6;      					//Ӣ��6��
                            switch(Menu_index_select)
                            {
                            case 0x01:
                                if((Menu_lever2_point<i)&&(Menu_lever2_point<Lever2_menu_num))
                                {
                                    ++Menu_lever2_point;
                                    ++Menu_index_finc_select;
                                    Display_menu_index();
                                }
                                else if((Menu_index_finc_select<MENU_LV21_NUM)&&(Menu_lever2_point==i))
                                {
                                    ++Menu_index_finc_select;
                                    Display_menu_index();
                                }
                                else
                                {
                                    Menu_lever2_point=Menu_index_finc_select=1;
                                    Display_menu_index();
                                }
                                break;
                            case 0x02:
                                if((Menu_lever2_point<i)&&(Menu_lever2_point<Lever2_menu_num))
                                {
                                    ++Menu_lever2_point;
                                    ++Menu_index_finc_select;
                                    Display_menu_index();
                                }
                                else if((Menu_index_finc_select<MENU_LV22_NUM)&&(Menu_lever2_point==i))
                                {
                                    ++Menu_index_finc_select;
                                    Display_menu_index();
                                }
                                else
                                {
                                    Menu_lever2_point=Menu_index_finc_select=1;
                                    Display_menu_index();
                                }
                                break;
                            case 0x03:
                                if((Menu_lever2_point<i)&&(Menu_lever2_point<Lever2_menu_num))
                                {
                                    ++Menu_lever2_point;
                                    ++Menu_index_finc_select;
                                    Display_menu_index();
                                }
                                else if((Menu_index_finc_select<MENU_LV23_NUM)&&(Menu_lever2_point==i)&&(fix_axbc_falg==0))
                                {
                                    ++Menu_index_finc_select;
                                    Display_menu_index();
                                }
                                else if((Menu_index_finc_select<(MENU_LV23_NUM-2))&&(Menu_lever2_point==i)&&(fix_axbc_falg==1))
                                {
                                    ++Menu_index_finc_select;
                                    Display_menu_index();
                                }
                                else
                                {
                                    Menu_lever2_point=Menu_index_finc_select=1;
                                    Display_menu_index();
                                }
                                break;
                            case 0x04:
                                if((Menu_lever2_point<i)&&(Menu_lever2_point<Lever2_menu_num))
                                {
                                    ++Menu_lever2_point;
                                    ++Menu_index_finc_select;
                                    Display_menu_index();
                                }
                                else if((Menu_index_finc_select<MENU_LV24_NUM)&&(Menu_lever2_point==i))
                                {
                                    ++Menu_index_finc_select;
                                    Display_menu_index();
                                }
                                else
                                {
                                    Menu_lever2_point=Menu_index_finc_select=1;
                                    Display_menu_index();
                                }
                                break;
                            case 0x05:
                                if((Menu_lever2_point<i)&&(Menu_lever2_point<Lever2_menu_num))
                                {
                                    ++Menu_lever2_point;
                                    ++Menu_index_finc_select;
                                    Display_menu_index();
                                }
                                else if((Menu_index_finc_select<MENU_LV25_NUM)&&(Menu_lever2_point==i))
                                {
                                    ++Menu_index_finc_select;
                                    Display_menu_index();
                                }
                                else
                                {
                                    Menu_lever2_point=Menu_index_finc_select=1;
                                    Display_menu_index();
                                }
                                break;
                            case 0x06:
                                if((Menu_lever2_point<i)&&(Menu_lever2_point<Lever2_menu_num))
                                {
                                    ++Menu_lever2_point;
                                    ++Menu_index_finc_select;
                                    Display_menu_index();
                                }
                                else if((Menu_index_finc_select<MENU_LV26_NUM)&&(Menu_lever2_point==i))
                                {
                                    ++Menu_index_finc_select;
                                    Display_menu_index();
                                }
                                else
                                {
                                    Menu_lever2_point=Menu_index_finc_select=1;
                                    Display_menu_index();
                                }
                                break;
                            case 0x07:
                                if((Menu_lever2_point<i)&&(Menu_lever2_point<Lever2_menu_num))
                                {
                                    ++Menu_lever2_point;
                                    ++Menu_index_finc_select;
                                    Display_menu_index();
                                }
                                else if((Menu_index_finc_select<MENU_LV27_NUM)&&(Menu_lever2_point==i))
                                {
                                    ++Menu_index_finc_select;
                                    Display_menu_index();
                                }
                                else
                                {
                                    Menu_lever2_point=Menu_index_finc_select=1;
                                    Display_menu_index();
                                }
                                break;
                            }
                        }
                        if(Menu_lever_select==0x03)		//�������˵�
                        {
                            Key_Menu_lever3_up_finc();		//2012-7-10 10:46
                        }
                        else if(Menu_lever_select==4)
                        {
                            if(Lever4_Select_en_flag())
                                Key_Menu_lever4_up_finc();		//2012-7-11 16:05
                        }
                    }
                }
                break;
//=============================================================================
            case KEY_CODE_DOWN:
                Set_back_light_10sec();
                Set_en_buzz_finc(1,4,200,100);
                if(Prog_disp_flag!=1)			//�Զ������д˰�����Ч
                {
                    if(Check_pass_word_flag==1)             //�Ƿ���������
                    {
                        ++Pass_adj_cnt;
                        if(Pass_adj_cnt>=10)
                            Pass_adj_cnt=0;

                        Pass_adj_disp(Pass_adj_cnt,Pass_bit_select);
                    }
                    else
                    {
                        if(Menu_lever_select==0x01)		//��һ���˵�
                        {
                            if(Menu_lever1_point>1)
                            {
                                --Menu_lever1_point;
                                --Menu_index_select;
                                Display_menu_index();
                            }
                            else if((Menu_index_select>1)&&(Menu_lever1_point==1))
                            {
                                --Menu_index_select;
                                Display_menu_index();
                            }
                            else if((Menu_index_select==1)&&(Menu_lever1_point==1))
                            {
                                if(M_menur1_language_select_temp==2)
                                    Menu_lever1_point=4;
                                else
                                    Menu_lever1_point=6;
                                Menu_index_select=MENU_LV1_NUM;
                                Display_menu_index();
                            }
                        }
                        if(Menu_lever_select==0x02)		//�ڶ����˵�
                        {
                            if(Menu_lever2_point>1)
                            {
                                --Menu_lever2_point;
                                --Menu_index_finc_select;
                                Display_menu_index();
                            }
                            else if((Menu_index_finc_select>1)&&(Menu_lever2_point==1))
                            {
                                --Menu_index_finc_select;
                                Display_menu_index();
                            }
                            else if((Menu_index_finc_select==1)&&(Menu_lever2_point==1))
                            {
                                if(Lever2_menu_num<6)
                                {
                                    if(M_menur1_language_select_temp==2)
                                        Menu_lever2_point=4;
                                    else
                                        Menu_lever2_point=Lever2_menu_num;
                                }
                                else
                                {
                                    if(M_menur1_language_select_temp==2)
                                        Menu_lever2_point=4;
                                    else
                                        Menu_lever2_point=6;
                                }
                                Menu_index_finc_select=Get_lv2menu_nun();
                                Display_menu_index();
                            }
                        }
                        if(Menu_lever_select==0x03)		//�������˵�
                        {
                            Key_Menu_lever3_down_finc();		//2012-7-10 10:46
                        }
                        else if(Menu_lever_select==4)
                        {
                            if(Lever4_Select_en_flag())
                                Key_Menu_lever4_down_finc();		//2012-7-11 16:05
                        }
                    }
                }
                break;
//=============================================================================
            case KEY_CODE_LEFT:
                Set_back_light_10sec();
                Set_en_buzz_finc(1,4,200,100);

                if(Prog_disp_flag==1)				//�Զ������д˰�����Ч
                    __nop();
                else if(Check_pass_word_flag==1)                   //�Ƿ���������
                {
                    if(Pass_bit_select>1)
                        --Pass_bit_select;
                    Pass_select_disp(Pass_bit_select);
                }
                else if(Menu_lever_select==0x03)		//�������˵�
                {
                    if((Menu_index_select==2)||(Menu_index_select==3)||(Menu_index_select==5)||(Menu_index_select==7))
                    {
                        if(Menu_lever3_Pub_select_cnt>1)
                            --Menu_lever3_Pub_select_cnt;
                        else if(Menu_lever3_Pub_select_cnt==1)
                        {
                            if(Menu_index_select==3)
                                Menu_lever3_Pub_select_cnt=Position_bit[Menu_index_finc_select+24];
                            else if(Menu_index_select==2)
                                Menu_lever3_Pub_select_cnt=Position_bit[Menu_index_finc_select+8];
                            else
                                Menu_lever3_Pub_select_cnt=Position_bit[Menu_index_finc_select];
                        }
                        Lever3_select_finc_adds();
                    }
                }
                else if(Menu_lever_select==4)		//���ļ��˵�
                {
                    if((Menu_index_select!=5)&&(Menu_index_select!=7))
                    {
                        if(Menu_lever4_Pub_select_cnt>1)
                            --Menu_lever4_Pub_select_cnt;
                        else if(Menu_lever4_Pub_select_cnt==1)
                            Menu_lever4_Pub_select_cnt=Menu_lever4_bit_temp;

                        Lever4_Select_finc_adds();

                        if(Pwm_menu_lever==3)
                        {
                            if(Pwm_select_cnt>1)
                                --Pwm_select_cnt;
                            else
                                Pwm_select_cnt=Pwm_menu_bit_temp;

                            Pwm_select_finc();
                        }
                    }
                }
                break;
//=============================================================================
            case KEY_CODE_RIGHT:
                Set_back_light_10sec();
                Set_en_buzz_finc(1,4,200,100);

                if(Prog_disp_flag==1)				//�Զ������д˰�����Ч
                    __nop();
                else if(Check_pass_word_flag==1)                   //�Ƿ���������
                {
                    if(Pass_bit_select<6)
                        ++Pass_bit_select;
                    Pass_select_disp(Pass_bit_select);
                }
                else if(Menu_lever_select==0x03)		//�������˵�
                {
                    if((Menu_index_select==2)||(Menu_index_select==3)||(Menu_index_select==5)||(Menu_index_select==7))
                    {
                        if(Menu_index_select==3)
                            Pub_temp=Position_bit[Menu_index_finc_select+24];
                        else if(Menu_index_select==2)
                            Pub_temp=Position_bit[Menu_index_finc_select+8];
                        else
                            Pub_temp=Position_bit[Menu_index_finc_select];

                        if(Menu_lever3_Pub_select_cnt<Pub_temp)
                            ++Menu_lever3_Pub_select_cnt;
                        else if(Menu_lever3_Pub_select_cnt==Pub_temp)
                            Menu_lever3_Pub_select_cnt=1;

                        Lever3_select_finc_adds();
                    }
                }
                else if(Menu_lever_select==4)		//���ļ��˵�
                {
                    if((Menu_index_select!=5)&&(Menu_index_select!=7))
                    {
                        if(Menu_lever4_Pub_select_cnt<Menu_lever4_bit_temp)
                            ++Menu_lever4_Pub_select_cnt;
                        else if(Menu_lever4_Pub_select_cnt==Menu_lever4_bit_temp)
                            Menu_lever4_Pub_select_cnt=1;

                        Lever4_Select_finc_adds();

                        if(Pwm_menu_lever==3)
                        {
                            if(Pwm_select_cnt<Pwm_menu_bit_temp)
                                ++Pwm_select_cnt;
                            else
                                Pwm_select_cnt=1;

                            Pwm_select_finc();
                        }
                    }
                }
                break;
//=============================================================================
            case KEY_CODE_ESC:
                Set_back_light_10sec();
                Set_en_buzz_finc(1,4,200,100);
                Led_port_status&=~BIT1;
                Led_port_status&=~BIT2;
                Led_data_flag&=~0x03;

#ifndef __NEWPCB__
                Led_disp_data(Led_port_status);
#else
                DrvGPIO_SetBit (E_GPC,15);
                DrvGPIO_SetBit (E_GPC,14);
                if(Alarm_active_flag==1)
                {
//               	         DrvGPIO_SetBit (E_GPC,14);		//close alarm led�������/��Ũ�ȱ�����
                    Set_switch_open_close(13,0);
                    DrvSYS_Delay(200000);
                    Set_switch_open_close(14,0);
                    Alarm_active_flag=0;
                }
#endif
                i=Led_port_status;
                i&=BIT1;
                if((Prog_disp_flag==1)||(i==BIT1))			//�����Զ��������л����˵���ʾ
                {
                    if(Prog_disp_flag==1)
                        Prog_disp_flag=2;

                    if(Menu_lever_select==4)
                        Display_lever4_index();
                    else
                    {
                        Display_menu_index();
                        if(Menu_lever_select<1)
                        {
                            i=ZB_flag;						//2013-1-7 11:43
                            i&=0x02;
                            if(i==0) Display_seg_single(1,126,0x00);
                            else     Display_seg_single(1,126,0x38);
                        }
                    }
                }
                else if(Pwm_menu_lever>1)			//���ж�PWM������������˳��ò���
                {
                    --Pwm_menu_lever;
                    Disp_pwm_menu();
                    if(Pwm_menu_lever==1)			//2012-9-21 16:00 reset pwm0 output result data.
                    {
                        Pwm_get_resul_from_flash(1);
                        a=Pwm_int2float(1,M_lever4_Out1_4ma_temp);
                        b=Pwm_int2float(2,M_lever4_Out1_20ma_temp);
                        c=b-a;
                        d=Pwm_last_result-a;
                        rs=4+16*d/c;

                        rds=Pwm_float2int(rs);
                        if(rds<0x00000400)
                            rds=0x00000400;
                        else if(rds>0x00002000)
                            rds=0x00002000;
                        Pwm_output_test(1,rds);
                    }
                }
                else if(Menu_lever_select>0)				//����˵�����1��������˳��˵�
                {
                    Enter_return_disp();
                    i=ZB_flag;						//2013-1-7 11:43
                    i&=0x02;
                    if(i==0) Display_seg_single(1,126,0x00);
                    else if(Menu_lever_select<1)
                        Display_seg_single(1,126,0x38);
                }
                else if((Prog_disp_flag==0)&&(Menu_lever_select==0))	//�����Զ������˵�ѡ����ʾ���һ�η������
                {
                    if(prg_wait_clr_flag==1)
                        prg_wait_clr_flag=0;
                    else
                        Display_menu_index();

                    i=ZB_flag;						//2013-1-7 11:43
                    i&=0x02;
                    if(i==0) Display_seg_single(1,126,0x00);
                    else     Display_seg_single(1,126,0x38);
                }
                else if((Prog_disp_flag==2)&&(Menu_lever_select==0))	//2012-9-1 22:42  �����Զ������л����Զ������Ĳ˵���ʾ
                {
                    Prog_disp_flag=1;
                    LCD_disp_flag=1;
                    Wait_clock_flag=0;		//2012-11-14 14:00
                    Clear_n_page(0,7);
                    if(Menu_lever3_start_flag==BIT4)
                    {
                        if(M_menur1_language_select_temp==2)
                        {
                            m_buff[0]=35;
                            m_buff[1]=36;
                            m_buff[2]=16;
                            m_buff[3]=17;
                        }
                        else
                        {
                            m_buff[0]='A';
                            m_buff[1]='U';
                            m_buff[2]='T';
                            m_buff[3]='O';
                            m_buff[4]=0;
                            m_buff[5]='A';
                            m_buff[6]='N';
                            m_buff[7]='A';
                        }
                    }
                    else if(Menu_lever3_start_flag==BIT5)
                    {
                        if(M_menur1_language_select_temp==2)
                        {
                            m_buff[0]=22;
                            m_buff[1]=36;
                            m_buff[2]=16;
                            m_buff[3]=17;
                        }
                        else
                        {
                            m_buff[0]='M';
                            m_buff[1]='A';
                            m_buff[2]='N';
                            m_buff[3]=19;
                            m_buff[4]=0;
                            m_buff[5]='A';
                            m_buff[6]='N';
                            m_buff[7]='A';
                        }
                    }
                    else if(Menu_lever3_start_flag==BIT6)
                    {
                        if(M_menur1_language_select_temp==2)
                        {
                            m_buff[0]=22;
                            m_buff[1]=36;
                            m_buff[2]=37;
                            m_buff[3]=38;
                        }
                        else
                        {
                            m_buff[0]='M';
                            m_buff[1]='A';
                            m_buff[2]='N';
                            m_buff[3]=19;
                            m_buff[4]=0;
                            m_buff[5]='C';
                            m_buff[6]='A';
                            m_buff[7]='L';
                        }
                    }
                    else if(Menu_lever3_start_flag==BIT7)
                    {
                        if(M_menur1_language_select_temp==2)
                        {
                            m_buff[0]=41;
                            m_buff[1]=42;
                            m_buff[2]=39;
                            m_buff[3]=40;
                        }
                        else
                        {
                            m_buff[0]='M';
                            m_buff[1]='A';
                            m_buff[2]='N';
                            m_buff[3]=19;
                            m_buff[4]=0;
                            m_buff[5]='C';
                            m_buff[6]='H';
                            m_buff[7]='K';
                        }
                    }
                    else					//2013-7-17 11:49
                    {
                        Prog_disp_flag=0;
                        if(Menu_lever_select>3)
                            Display_lever4_index();
                        else
                            Display_menu_index();
                        break;
                    }

                    if(M_menur1_language_select_temp==2)
                        Display_program_5opration_re(4,m_buff);
                    else
                    {
                        for(i=0; i<8; i++)
                            Get_6x8_char_single(0,i*6,m_buff[i]);		//line1
                    }

                    All_program_menu_disp(program_menu_select);			//2012-9-24 14:03

                    if(M_menur1_LZDJ_select_temp==1)
                    {
                        if(Measurement_signal_21!=0)
                        {
                            disp_d=Measurement_signal_21;
                            Disp_real_time_vol_data(2,1,disp_d);
                        }
                        if((M_menur1_DL_select_temp==1)&&(Measurement_signal_22!=0))	//���߷�����ʾ
                        {
                            disp_d=Measurement_signal_22;
                            Disp_real_time_vol_data(2,2,disp_d);
                        }
                    }
                    else
                    {
                        if(Measurement_signal_11!=0)
                        {
                            disp_d=Measurement_signal_11;
                            Disp_real_time_vol_data(1,1,disp_d);
                        }
                        if(Measurement_signal_12!=0)
                        {
                            disp_d=Measurement_signal_12;
                            Disp_real_time_vol_data(1,2,disp_d);
                        }
                    }
                }
                else
                {
                    Prog_disp_flag=1;
                    LCD_disp_flag=1;
                    Wait_clock_flag=0;		//2012-11-14 14:00
                    if(Menu_lever_select==4)
                        Display_lever4_index();
                    else
                        Display_menu_index();
                }
                break;
//=============================================================================
            case KEY_CODE_ENTER:
                Set_back_light_10sec();
                Set_en_buzz_finc(1,4,200,100);
                if(Prog_disp_flag==1)
                    break;

                if(Check_prio_finc())
                    break;

                else if(Menu_lever_select<3)	//�˵�С��3��ȷ�Ͻ�����һ���˵�
                {
                    ++Menu_lever_select;
                    if(Menu_lever_select==2)
                        Lever2_menu_num=Get_lv2menu_nun();
                    if(Menu_lever_select==3)
                    {
                        Menu_lever3_point=1;
                        Menu_lever3_select=1;
                        Menu_lever3_Pub_select_cnt=1;
                        Menu_lever3_Pub_adj_cnt=0;
                    }
                    Display_menu_index();
                }
                else if(Menu_lever_select==3)	//�������˵�ȷ�����
                {
                    if(Menu_lever3_start_flag>=0x10)
                    {
                        if(Wait_clock_flag==1) __nop();			//2013-6-8 9:58 �ȴ���ʱ�򿪷��޸�Ȩ��
                        else if((Menu_index_select==1)||(Menu_index_select==4)||(Menu_index_select==6))
                        {
                            Clear_n_page_ram(0,7);
                            m_buff[0]='B';
                            m_buff[1]='U';
                            m_buff[2]='S';
                            m_buff[3]='Y';
                            m_buff[4]=0;
                            m_buff[5]='A';
                            m_buff[6]='N';
                            m_buff[7]='A';
                            m_buff[8]=0;
                            m_buff[9]='R';
                            m_buff[10]='U';
                            m_buff[11]='N';
                            m_buff[12]='N';
                            m_buff[13]='I';
                            m_buff[14]='N';
                            m_buff[15]='G';

                            for(i=0; i<5; i++)
                                Get_6x8_char_single(3,i*6,m_buff[i]);		//line1

                            Display_all_buff();
                            break;
                        }
                    }

                    if(Display_lever4_en_flag())
                    {
                        Menu_lever4_Pub_select_cnt=1;
                        Menu_lever4_Pub_adj_cnt=0;
                        Menu_lever_select=4;
                        Display_lever4_index();
                    }
                    if(Menu_index_select==5)
                        Menu_lever_select=4;

                    Write_system_reg_to_chip();		//save reg data.

                    if((Menu_index_select!=3)&&(Menu_lever_select==3))
                    {
                        if((Menu_index_select==4)&&(Menu_index_finc_select==2)&&(Menu_lever3_select!=1))
                            __nop();
                        else if((Menu_index_select==4)&&(Menu_index_finc_select==6))
                            __nop();
                        else if((Menu_index_select==4)&&(Menu_index_finc_select==8))
                            __nop();
                        else if((Menu_index_select==4)&&(Menu_index_finc_select==7))
                            __nop();
                        else if((Menu_index_select==6)&&(Menu_index_finc_select==2))
                            __nop();
                        else if((Menu_index_select==6)&&(Menu_index_finc_select==4))
                            __nop();
                        else
                            Menu_lever3_4_enter_finction();
                    }
                    else if(Menu_index_select==3)			//updata program ��������¿��ƣ�
                    {
                        if(Menu_index_finc_select==19)
                        {
                            p_step_cnt=1;
                            switch(Menu_lever3_select)
                            {
                            case 2:					//drive
                                Aoto_program_flag=2;
                                break;
                            case 3:					//sencer
                                Aoto_program_flag=1;
                                break;
                            case 1:					//main
                                Aoto_program_flag=0;
                                DrvGPIO_SetBit (E_GPB,15);
                                Disp_updata_status(3);

                                UNLOCKREG();
                                DrvFMC_EnableISP();
                                FMC->ISPCON.CFGUEN = 1;
                                FMC->ISPCON.BS = 1;
                                rds=DrvFMC_WriteConfig(0x00200002,0x0001FE00);		//config0=0x00200002 ;config1=0x0001FE00

                                FMC->ISPCON.ISPEN = 0;
                                SYS->IPRSTC1.CHIP_RST = 1;
                                LOCKREG();
                                break;
                            }
                        }
                        else Enter_return_disp();
                    }
                }
                else if(Menu_lever_select==4)		//���ļ��˵�ȷ�����
                {
                    if(Pwm_menu_lever>=1)			//2012-9-18 14:03 pwm menu��PWM�����˵���
                    {
                        ++Pwm_menu_lever;
                        if(Pwm_menu_lever==2)
                            Pwm_menu_select1=1;

                        if(Pwm_menu_lever<=3)
                            Disp_pwm_menu();
                        else
                        {
                            //redo pwm out
                            Pwm_menu_lever=2;
                            Disp_pwm_menu();
                            Write_system_reg_to_chip();
                        }
                    }
                    else
                        Menu_lever3_4_enter_finction();      //���ļ��˵�ȷ�Ϻ���

                    Write_system_reg_to_chip();		//save reg data.
                }
                break;
//========================================================================================
            case KEY_CODE_POWER:
                Set_en_buzz_finc(1,4,200,100);
                if((Auto_check_clock_flag!=0)||(Menu_lever3_start_flag>=0x10))	//���ڵȴ��´η���������õȴ�
                {
                    Menu_lever3_start_flag=0;
                    prg_wait_clr_flag=0;
                    Auto_check_clock_flag=0;
                    fix_start_time_falg=1;

                    if(program_menu_select==19) Rx_TX_flag=0;

//////		    	 M_lever4_start_munth_temp=0;
//////		    	 M_lever4_start_day_temp=0;
//////		    	 M_lever4_start_hour_temp=0;
//////		    	 M_lever4_start_mini_temp=0;

////                     	 Program_reset_initial();	//reset all prog data.
////                     	 Read_first_reg_data();		//reset system reg. 72012-10-12 14:02
////                     	 Hand_finction_select=0;
                    Write_system_reg_to_chip();
                }
                if((Prog_disp_flag!=0)||(Hand_finction_select!=0))			//�����Զ�����ֹͣ�Զ�����
                {
                    if(Prog_disp_flag==1)
                    {
                        if(Menu_lever_select==4)
                            Display_lever4_index();
                        else
                            Display_menu_index();
                    }
                    else  Display_menu_index();		//2013-6-19 15:37

                    /////Program_reset_initial();		//reset all prog data.
                    Program_reset_initial_flag=1;
                    Program_reset_initial_cnt=0;
                    Read_first_reg_data();			//reset system reg. 72012-10-12 14:02
                    Hand_finction_select=0;
                }
                else if((Prog_disp_flag==0)&&(Hand_finction_select==0))		//ֱ�ӻص���ʾ���һ����ʷ��¼�˵�
                {
                    if(Check_pass_word_flag==1)
                    {
                        Clead_pass_word_reg_data();		//2012-10-23 10:08
                    }
/////                        if((Far_status_flag!=0)&&(Menu_lever3_start_flag<0x10)&&(Hand_finction_select==0))
/////                            Set_tx_cmd_data_finc(0xd1,0x06,U_DRIVER_FA_ALL_WO_ADDS,0x0000,0x00);		//close all v

                    Far_status_flag=0;

                    User_prio_temp=1;

                    Pwm_menu_lever=0;
                    Pwm_menu_select=0;
                    Pwm_menu_select1=0;
                    Pwm_select_cnt=0;
                    Pwm_adj_cnt=0;
                    Pwm_menu_bit_temp=0;
                    Pwm_menu_PoSeg=0;
                    Pwm_menu_PoSeg_pub=0;

                    Menu_index_select=1;
                    Menu_lever1_point=1;
                    LCD_disp_flag=0;
                    Menu_lever_select=0;

                    Menu_lever2_point=1;
                    Menu_index_finc_select=1;
                    Menu_lever3_point=1;
                    Menu_lever3_select=1;
                    Menu_lever3_Pub_select_cnt=1;
                    Fenxi.disp_index=0;
                    Jiaozheng.disp_index=0;
                    Baojing.disp_index=0;
                    Cuowu.disp_index=0;
                    Menu_lever4_Pub_select_cnt=1;
                    Menu_lever4_bit_temp=0;
                    Menu_lever4_Pub_adj_cnt=0;
                    Disp_gdgdy_flag=0;
                    Pro_wait_time_set=0;
                    Pro_wait_time_flag=2;
                    DCF_switch_select_flag=0;
                    Pro_power_on_flag=1;
                    Display_menu_index();
                }
                Set_back_light_10sec();
                break;
//========================================================================================
            case KEY_CODE_EXPORT:			//2012-9-7 13:51
                Set_back_light_10sec();
                Set_en_buzz_finc(1,4,200,100);

                if(Menu_lever_select==0)			//���ݵ�������
                {
                    if(Export_data_flag!=1)
                    {
                        Export_data_flag=1;
                        export_data_select=1;
                        export_tx_flag=0;
                        FX_export_index_adds_point=FX_write_index_adds_point;
                        JZ_export_index_adds_point=JZ_write_index_adds_point;
                        BJ_export_index_adds_point=BJ_write_index_adds_point;
                        CW_export_index_adds_point=CW_write_index_adds_point;
                    }
                    else
                    {
                        Export_data_flag=0;
                        export_data_select=0;
                    }
                }
                break;
//========================================================================================
            case 0x0000001F:		//enter+stop
                Set_en_buzz_finc(1,4,200,100);
                UNLOCKREG();
                SYS->IPRSTC1.CHIP_RST = 1;		//RESET CHIP
                LOCKREG();
                break;
            case 0x0000006D:		//esc+up
                Set_en_buzz_finc(1,4,200,100);
                ZB_flag&=~0x02;
                Display_seg_single(1,126,0x00);
                Write_system_reg_to_chip();		//save reg data.
                break;
            case 0x0000006E:		//esc+down
                Set_en_buzz_finc(1,4,200,100);
                ZB_flag|=0x02;
                Write_system_reg_to_chip();		//save reg data.
                break;
            case 0x0000006B:		//esc+left
                Set_en_buzz_finc(1,4,200,100);
                fix_axbc_falg=1;				//dis disp ax+b

                if((Menu_index_finc_select>(MENU_LV23_NUM-6))&&(Menu_lever_select==0x02)&&(Menu_index_select==3))
                {
/////		         Disp_16x16_area(2*(Menu_lever2_point-1),0,0,TB_clr_dot);
                    Menu_lever2_point=Menu_index_finc_select=1;
/////		         Disp_16x16_area(2*(Menu_lever2_point-1),0,0,TB_dot);
                    Display_menu_index();
                }
                break;
            case 0x00000067:		//esc+right
                Set_en_buzz_finc(1,4,200,100);
                fix_axbc_falg=0;				//en disp ax+b
                break;
                //case 0x0000007C:		//up+dowm	2015-7-3 10:08
                //     if(wd_use_flag_chg==1)
                //     {
                //     	wd_use_flag=1;
                //     	wd_use_flag_chg=0;
                //     	wd_20_change_flag=0;
                //     	wd_data_cnt=0;
                //     	Display_seg_single(7,126,0xc0);
                //     }
                //     else
                //     {
                //     	wd_use_flag=0;
                //     	wd_use_flag_chg=1;
                //     	Display_seg_single(7,126,0x00);
                //     }
                //break;
            }
        }
    }
}

//===============================================================================
//	finction	:Check_JB_speed_edag
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2013-8-1 14:58
//	modefy		:null
//===============================================================================
void Check_JB_speed_edag(void)
{
    uint32_t i;

    if(M_menur1_JB_speed_temp>0x00000235)
    {
        M_menur1_JB_speed_temp=0x00000235;

        i=M_menur1_JB_speed_temp>>4;
        i&=0x0f;
        i=Get_ascii_data(i);
        Get_6x8_char_single(3,36,i);
        Display_6x8_char_single(3,36,i);

        i=M_menur1_JB_speed_temp;
        i&=0x0f;
        i=Get_ascii_data(i);
        Get_6x8_char_single(3,42,i);
        Display_6x8_char_single(3,42,i);
    }
    else if(M_menur1_JB_speed_temp<0x00000175)
    {
        M_menur1_JB_speed_temp=0x00000175;

        i=M_menur1_JB_speed_temp>>4;
        i&=0x0f;
        i=Get_ascii_data(i);
        Get_6x8_char_single(3,36,i);
        Display_6x8_char_single(3,36,i);

        i=M_menur1_JB_speed_temp;
        i&=0x0f;
        i=Get_ascii_data(i);
        Get_6x8_char_single(3,42,i);
        Display_6x8_char_single(3,42,i);
    }
}

//===============================================================================
//	finction	:Key_Menu_lever3_up_finc
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.07.10
//	modefy		:null
//===============================================================================
void Key_Menu_lever3_up_finc(void)
{
    uint8_t  j;
    uint32_t i;
////  uint8_t  data_buff_t[3];

    switch(Menu_index_select)
    {
    case 0x07: 					//ϵͳ���ò�������
        --Menu_lever3_Pub_adj_cnt;
        if(Menu_index_finc_select==1)
        {
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt==0xff)
                    Menu_lever3_Pub_adj_cnt=2;

                j=Menu_lever3_Pub_adj_cnt;
                j<<=4;
                RTC_Set.hour_temp&=0x0f;
                RTC_Set.hour_temp|=j;
                break;
            case 2:
                i=RTC_Set.hour_temp;
                i&=0x20;
                if(i==0x20)
                {
                    if((Menu_lever3_Pub_adj_cnt>4)&&(Menu_lever3_Pub_adj_cnt!=0xff))
                        Menu_lever3_Pub_adj_cnt=0;
                    else if(Menu_lever3_Pub_adj_cnt==0xff)
                        Menu_lever3_Pub_adj_cnt=3;
                }
                else if(Menu_lever3_Pub_adj_cnt==0xff)
                    Menu_lever3_Pub_adj_cnt=9;

                RTC_Set.hour_temp&=0xf0;
                RTC_Set.hour_temp+=Menu_lever3_Pub_adj_cnt;
                break;
            case 3:
                if(Menu_lever3_Pub_adj_cnt==0xff)
                    Menu_lever3_Pub_adj_cnt=5;

                j=Menu_lever3_Pub_adj_cnt;
                j<<=4;
                RTC_Set.minu_temp&=0x0f;
                RTC_Set.minu_temp|=j;
                break;
            case 4:
                if(Menu_lever3_Pub_adj_cnt==0xff)
                    Menu_lever3_Pub_adj_cnt=9;

                RTC_Set.minu_temp&=0xf0;
                RTC_Set.minu_temp+=Menu_lever3_Pub_adj_cnt;
                break;
            }
        }
        if(Menu_index_finc_select==2)
        {
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt==0xff)
                    Menu_lever3_Pub_adj_cnt=9;

                j=Menu_lever3_Pub_adj_cnt;
                j<<=4;
                RTC_Set.year_temp&=0x0f;
                RTC_Set.year_temp|=j;
                break;
            case 2:
                if(Menu_lever3_Pub_adj_cnt==0xff)
                    Menu_lever3_Pub_adj_cnt=9;

                RTC_Set.year_temp&=0xf0;
                RTC_Set.year_temp+=Menu_lever3_Pub_adj_cnt;
                break;
            case 3:
                if(Menu_lever3_Pub_adj_cnt==0xff)
                    Menu_lever3_Pub_adj_cnt=1;

                j=Menu_lever3_Pub_adj_cnt;
                j<<=4;
                RTC_Set.mouth_temp&=0x0f;
                RTC_Set.mouth_temp|=j;
                break;
            case 4:
                i=RTC_Set.mouth_temp;
                i&=0x10;
                if(i==0x10)
                {
                    if((Menu_lever3_Pub_adj_cnt>2)&&(Menu_lever3_Pub_adj_cnt!=0xff))
                        Menu_lever3_Pub_adj_cnt=0;

                    else if(Menu_lever3_Pub_adj_cnt==0xff)
                        Menu_lever3_Pub_adj_cnt=2;
                }
                else if((Menu_lever3_Pub_adj_cnt==0)||(Menu_lever3_Pub_adj_cnt==0xff))
                    Menu_lever3_Pub_adj_cnt=9;

                RTC_Set.mouth_temp&=0x10;
                RTC_Set.mouth_temp+=Menu_lever3_Pub_adj_cnt;
                break;
            case 5:
                if(Menu_lever3_Pub_adj_cnt==0xff)
                    Menu_lever3_Pub_adj_cnt=3;

                j=Menu_lever3_Pub_adj_cnt;
                j<<=4;
                RTC_Set.day_temp&=0x0f;
                RTC_Set.day_temp|=j;
                break;
            case 6:
                if(Menu_lever3_Pub_adj_cnt==0)
                {
                    i=RTC_Set.day_temp;
                    i&=0x30;
                    if(i==00)
                    {
                        Menu_lever3_Pub_adj_cnt=9;
                    }
                }
                else
                {
                    i=RTC_Set.day_temp;
                    i&=0x30;
                    if(i==0x30)
                    {
                        if((Menu_lever3_Pub_adj_cnt>1)&&(Menu_lever3_Pub_adj_cnt!=0xff))
                            Menu_lever3_Pub_adj_cnt=0;
                        else if(Menu_lever3_Pub_adj_cnt==0xff)
                            Menu_lever3_Pub_adj_cnt=1;
                    }
                    else if(i==0x20)	//2��ƽ������
                    {
                        if(Menu_lever3_Pub_adj_cnt==0xff)
                            Menu_lever3_Pub_adj_cnt=9;
                    }
                    else if(Menu_lever3_Pub_adj_cnt==0xff)
                        Menu_lever3_Pub_adj_cnt=9;
                }
                RTC_Set.day_temp&=0x30;
                RTC_Set.day_temp+=Menu_lever3_Pub_adj_cnt;
                break;
            }
        }
        if(Menu_index_finc_select==5)
        {
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt>6)
                    Menu_lever3_Pub_adj_cnt=6;

                i=Menu_lever3_Pub_adj_cnt<<4;
                M_menur1_lcd_Contrast_temp&=0x0f;
                M_menur1_lcd_Contrast_temp+=i;
                break;
            case 2:
                if((M_menur1_lcd_Contrast_temp>=0x60)&&(Menu_lever3_Pub_adj_cnt==0))
                    Menu_lever3_Pub_adj_cnt=4;
                else if(Menu_lever3_Pub_adj_cnt>9)
                    Menu_lever3_Pub_adj_cnt=9;

                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_lcd_Contrast_temp&=0xf0;
                M_menur1_lcd_Contrast_temp+=i;
                break;
            }
/////               Adj_lcd_Contrast_data(); 			//2013-4-10 17:38
        }
        if(Menu_index_finc_select==6)
        {
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt==0xff)
                    Menu_lever3_Pub_adj_cnt=2;

                i=Menu_lever3_Pub_adj_cnt<<8;
                back_l_temp&=0x000000ff;
                back_l_temp+=i;

                Back_light_time=0;
                i=back_l_temp>>8;
                i&=0x0000000f;
                i*=10000;
                Back_light_time+=i;
                i=back_l_temp>>4;
                i&=0x0000000f;
                i*=1000;
                Back_light_time+=i;
                i=back_l_temp;
                i&=0x0000000f;
                i*=100;
                Back_light_time+=i;
                break;
            case 2:
                if((Back_light_time/10000)==2)
                {
                    if(Menu_lever3_Pub_adj_cnt==0xff)
                        Menu_lever3_Pub_adj_cnt=5;
                }
                else if(Menu_lever3_Pub_adj_cnt==0xff)
                    Menu_lever3_Pub_adj_cnt=9;

                i=Menu_lever3_Pub_adj_cnt<<4;
                back_l_temp&=0x00000f0f;
                back_l_temp+=i;

                Back_light_time=0;
                i=back_l_temp>>8;
                i&=0x0000000f;
                i*=10000;
                Back_light_time+=i;
                i=back_l_temp>>4;
                i&=0x0000000f;
                i*=1000;
                Back_light_time+=i;
                i=back_l_temp;
                i&=0x0000000f;
                i*=100;
                Back_light_time+=i;
                break;
            case 3:
                if((Back_light_time/1000)==25)
                {
                    if(Menu_lever3_Pub_adj_cnt==0xff)
                        Menu_lever3_Pub_adj_cnt=5;
                }
                else if(Menu_lever3_Pub_adj_cnt==0xff)
                    Menu_lever3_Pub_adj_cnt=9;

                i=Menu_lever3_Pub_adj_cnt;
                back_l_temp&=0x00000ff0;
                back_l_temp+=i;

                Back_light_time=0;
                i=back_l_temp>>8;
                i&=0x0000000f;
                i*=10000;
                Back_light_time+=i;
                i=back_l_temp>>4;
                i&=0x0000000f;
                i*=1000;
                Back_light_time+=i;
                i=back_l_temp;
                i&=0x0000000f;
                i*=100;
                Back_light_time+=i;
                break;
            }
        }
        if(Menu_index_finc_select==7)
        {
            if(Menu_lever3_Pub_adj_cnt==0)
                Menu_lever3_Pub_adj_cnt=1;
            Set_back_light_down();
        }
        Menu1_adj_valu();
        break;
    case 0x02: 					//��������-��������
        --Menu_lever3_Pub_adj_cnt;
        switch(Menu_index_finc_select)
        {
        case 2:			//����
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                M_menur1_lange_data_temp&=0x0000ffff;
                M_menur1_lange_data_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_menur1_lange_data_temp&=0x000f0fff;
                M_menur1_lange_data_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_lange_data_temp&=0x000ff0ff;
                M_menur1_lange_data_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_lange_data_temp&=0x000fff0f;
                M_menur1_lange_data_temp+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_lange_data_temp&=0x000ffff0;
                M_menur1_lange_data_temp+=i;
                break;
            }
            break;
        case 5:			//
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_QYSJ_temp&=0x000000ff;
                M_menur1_QYSJ_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_QYSJ_temp&=0x00000f0f;
                M_menur1_QYSJ_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_QYSJ_temp&=0x00000ff0;
                M_menur1_QYSJ_temp+=i;
                break;
            }
            break;
        case 4:			//
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_CSSJ_temp&=0x000000ff;
                M_menur1_CSSJ_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_CSSJ_temp&=0x00000f0f;
                M_menur1_CSSJ_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_CSSJ_temp&=0x00000ff0;
                M_menur1_CSSJ_temp+=i;
                break;
            }
            break;
        case 6:			//
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            M_menur1_RX_time_temp=Menu_lever3_Pub_adj_cnt;
            break;
        case 7:			//�ź�Ư��
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt>5)
                    Menu_lever3_Pub_adj_cnt=5;

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_menur1_XHPY_temp&=0x00000fff;
                M_menur1_XHPY_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_XHPY_temp&=0x0000f0ff;
                M_menur1_XHPY_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_XHPY_temp&=0x0000ff0f;
                M_menur1_XHPY_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_XHPY_temp&=0x0000fff0;
                M_menur1_XHPY_temp+=i;
                break;
            }
            break;
        case 8:			//�ȴ�ʱ��
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                E_wait_time_temp&=0x000000ff;
                E_wait_time_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                E_wait_time_temp&=0x000000f0f;
                E_wait_time_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                E_wait_time_temp&=0x00000ff0;
                E_wait_time_temp+=i;
                break;
            }
            break;
        case 9:			//����ʱ��
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_menur1_DDSJ_temp&=0x00000fff;
                M_menur1_DDSJ_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_DDSJ_temp&=0x0000f0ff;
                M_menur1_DDSJ_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_DDSJ_temp&=0x0000ff0f;
                M_menur1_DDSJ_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_DDSJ_temp&=0x0000fff0;
                M_menur1_DDSJ_temp+=i;
                break;
            }
            break;
//              case 10:			//б��
//              break;
//              case 12:			//adj y=ax^2+bx+c
//              break;
        case 13:
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                Allow_error&=0x00000fff;
                Allow_error+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                Allow_error&=0x0000f0ff;
                Allow_error+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                Allow_error&=0x0000ff0f;
                Allow_error+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                Allow_error&=0x0000fff0;
                Allow_error+=i;
                break;
            }
            break;
        case 14:
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
//                             if(Menu_lever3_Pub_adj_cnt>6)
//                                Menu_lever3_Pub_adj_cnt=6;
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                M_lever3_HCPD_temp&=0x0000ffff;
                M_lever3_HCPD_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_lever3_HCPD_temp&=0x000f0fff;
                M_lever3_HCPD_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_lever3_HCPD_temp&=0x000ff0ff;
                M_lever3_HCPD_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_lever3_HCPD_temp&=0x000fff0f;
                M_lever3_HCPD_temp+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_lever3_HCPD_temp&=0x000ffff0;
                M_lever3_HCPD_temp+=i;
                break;
            }
            break;
        case 15:
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                Alarm_low_nd&=0x0000ffff;
                Alarm_low_nd+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                Alarm_low_nd&=0x000f0fff;
                Alarm_low_nd+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                Alarm_low_nd&=0x000ff0ff;
                Alarm_low_nd+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                Alarm_low_nd&=0x000fff0f;
                Alarm_low_nd+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                Alarm_low_nd&=0x0000fff0;
                Alarm_low_nd+=i;
                break;
            }
            break;
        case 16:
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                Alarm_hight_nd&=0x0000ffff;
                Alarm_hight_nd+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                Alarm_hight_nd&=0x000f0fff;
                Alarm_hight_nd+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                Alarm_hight_nd&=0x000ff0ff;
                Alarm_hight_nd+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                Alarm_hight_nd&=0x000fff0f;
                Alarm_hight_nd+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                Alarm_hight_nd&=0x0000fff0;
                Alarm_hight_nd+=i;
                break;
            }
            break;
        }
        if(Menu_index_finc_select!=12)
            Menu1_adj_valu();
        break;
//-----------------------------------------------------------
    case 0x03: 					//��������-��������
        --Menu_lever3_Pub_adj_cnt;
        switch(Menu_index_finc_select)
        {
        case 3:			//LED ����
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt>5)
                    Menu_lever3_Pub_adj_cnt=5;

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                M_menur1_led_dianliu_temp&=0x0000ffff;
                M_menur1_led_dianliu_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_menur1_led_dianliu_temp&=0x000f0fff;
                M_menur1_led_dianliu_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_led_dianliu_temp&=0x000ff0ff;
                M_menur1_led_dianliu_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_led_dianliu_temp&=0x000fff0f;
                M_menur1_led_dianliu_temp+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_led_dianliu_temp&=0x000ffff0;
                M_menur1_led_dianliu_temp+=i;
                break;
            }
            break;
        case 4:
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_menur1_led_data_temp&=0x0fff;
                M_menur1_led_data_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_led_data_temp&=0xf0ff;
                M_menur1_led_data_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_led_data_temp&=0xff0f;
                M_menur1_led_data_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_led_data_temp&=0xfff0;
                M_menur1_led_data_temp+=i;
                break;
            }
            break;
        case 5:
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
#ifdef __OLD_JBSPEED__
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt>2)
                    Menu_lever3_Pub_adj_cnt=2;

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_JB_speed_temp&=0x000000ff;
                M_menur1_JB_speed_temp+=i;
                break;
            case 2:
                i=M_menur1_JB_speed_temp>>8;
                i&=0x0f;
                if(i==2)
                {
                    if(Menu_lever3_Pub_adj_cnt>4)
                        Menu_lever3_Pub_adj_cnt=4;
                }

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_JB_speed_temp&=0x00000f0f;
                M_menur1_JB_speed_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_JB_speed_temp&=0x00000ff0;
                M_menur1_JB_speed_temp+=i;
                break;
            }
#else
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt<1)
                    Menu_lever3_Pub_adj_cnt=2;

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_JB_speed_temp&=0x000000ff;
                M_menur1_JB_speed_temp+=i;

                Check_JB_speed_edag();
                break;
            case 2:
                i=M_menur1_JB_speed_temp>>8;
                i&=0x0f;
                if(i==2)
                {
                    if(Menu_lever3_Pub_adj_cnt>3)
                        Menu_lever3_Pub_adj_cnt=3;
                }
                if(i==1)
                {
                    if(Menu_lever3_Pub_adj_cnt<7)
                        Menu_lever3_Pub_adj_cnt=9;
                }

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_JB_speed_temp&=0x00000f0f;
                M_menur1_JB_speed_temp+=i;

                Check_JB_speed_edag();
                break;
            case 3:
                i=M_menur1_JB_speed_temp>>4;
                if(i==0x23)
                {
                    if(Menu_lever3_Pub_adj_cnt>5)
                        Menu_lever3_Pub_adj_cnt=5;
                }
                else if(i==0x17)
                {
                    if(Menu_lever3_Pub_adj_cnt<5)
                        Menu_lever3_Pub_adj_cnt=9;
                }
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_JB_speed_temp&=0x00000ff0;
                M_menur1_JB_speed_temp+=i;
                break;
            }
#endif
            break;
        case 7:			//ע����ٶ�
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt<1)
                    Menu_lever3_Pub_adj_cnt=2;

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_ZSB_speed_temp&=0x0000000f;
                M_menur1_ZSB_speed_temp+=i;

                if(M_menur1_ZSB_speed_temp>0x00000027)
                {
                    M_menur1_ZSB_speed_temp=0x00000027;

                    i=M_menur1_ZSB_speed_temp;
                    i&=0x0f;
                    i=Get_ascii_data(i);

                    Get_6x8_char_single(3,36,i);
                    Display_6x8_char_single(3,36,i);
                }
                break;
            case 2:
                i=M_menur1_ZSB_speed_temp>>4;
                i&=0x0f;
                if(i==2)
                {
                    if(Menu_lever3_Pub_adj_cnt>7)
                        Menu_lever3_Pub_adj_cnt=7;
                }

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_ZSB_speed_temp&=0x000000f0;
                M_menur1_ZSB_speed_temp+=i;
                break;
            }
            break;
        case 9:			//��Ʒ���
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt>2)
                    Menu_lever3_Pub_adj_cnt=2;

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_menur1_sample_v_temp&=0x00000fff;
                M_menur1_sample_v_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_sample_v_temp&=0x0000f0ff;
                M_menur1_sample_v_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_sample_v_temp&=0x0000ff0f;
                M_menur1_sample_v_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_sample_v_temp&=0x0000fff0;
                M_menur1_sample_v_temp+=i;
                break;
            }
            break;
        case 10:			//
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                M_menur1_jz1nd_temp&=0x0000ffff;
                M_menur1_jz1nd_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_menur1_jz1nd_temp&=0x000f0fff;
                M_menur1_jz1nd_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_jz1nd_temp&=0x000ff0ff;
                M_menur1_jz1nd_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_jz1nd_temp&=0x000fff0f;
                M_menur1_jz1nd_temp+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_jz1nd_temp&=0x000ffff0;
                M_menur1_jz1nd_temp+=i;
                break;
            }
            break;
        case 11:			//
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt>2)
                    Menu_lever3_Pub_adj_cnt=2;

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_menur1_jz1_v_temp&=0x00000fff;
                M_menur1_jz1_v_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_jz1_v_temp&=0x0000f0ff;
                M_menur1_jz1_v_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_jz1_v_temp&=0x0000ff0f;
                M_menur1_jz1_v_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_jz1_v_temp&=0x0000fff0;
                M_menur1_jz1_v_temp+=i;
                break;
            }
            break;
        case 12:			//
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                M_menur1_jz2nd_temp&=0x0000ffff;
                M_menur1_jz2nd_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_menur1_jz2nd_temp&=0x000f0fff;
                M_menur1_jz2nd_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_jz2nd_temp&=0x000ff0ff;
                M_menur1_jz2nd_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_jz2nd_temp&=0x000fff0f;
                M_menur1_jz2nd_temp+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_jz2nd_temp&=0x000ffff0;
                M_menur1_jz2nd_temp+=i;
                break;
            }
            break;
        case 13:
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_jz2_v_temp&=0x000000ff;
                M_menur1_jz2_v_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_jz2_v_temp&=0x00000f0f;
                M_menur1_jz2_v_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_jz2_v_temp&=0x00000ff0;
                M_menur1_jz2_v_temp+=i;
                break;
            }
            break;
        case 14:			//�Լ�1
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_sj1_v_temp&=0x0000000f;
                M_menur1_sj1_v_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_sj1_v_temp&=0x000000f0;
                M_menur1_sj1_v_temp+=i;
                break;
            }
            break;
        case 15:			//
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_sj2_v_temp&=0x0000000f;
                M_menur1_sj2_v_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_sj2_v_temp&=0x000000f0;
                M_menur1_sj2_v_temp+=i;
                break;
            }
            break;
        case 16:			//
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_sj3_v_temp&=0x0000000f;
                M_menur1_sj3_v_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_sj3_v_temp&=0x000000f0;
                M_menur1_sj3_v_temp+=i;
                break;
            }
            break;
        case 17:			//
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_sj4_v_temp&=0x0000000f;
                M_menur1_sj4_v_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_sj4_v_temp&=0x000000f0;
                M_menur1_sj4_v_temp+=i;
                break;
            }
            break;
        case 18:			//
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=1;
                i<<=28;
                Prj_correlat&=0x0fffffff;
                Prj_correlat+=i;
                break;
            case 2:
                if((Menu_lever3_Pub_adj_cnt==0)&&(Prj_correlat<10))
                    Menu_lever3_Pub_adj_cnt=1;

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=20;
                Prj_correlat&=0x000fffff;
                Prj_correlat+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                Prj_correlat&=0x00f0ffff;
                Prj_correlat+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                Prj_correlat&=0x00ff0fff;
                Prj_correlat+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                Prj_correlat&=0x00fff0ff;
                Prj_correlat+=i;
                break;
            case 6:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                Prj_correlat&=0x00ffff0f;
                Prj_correlat+=i;
                break;
            case 7:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                Prj_correlat&=0x00fffff0;
                Prj_correlat+=i;
                break;
            }
            break;
        case 20:			//
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=24;
                Set_xishu&=0x00ffffff;
                Set_xishu+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=20;
                Set_xishu&=0x0f0fffff;
                Set_xishu+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                Set_xishu&=0x0ff0ffff;
                Set_xishu+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                Set_xishu&=0x0fff0fff;
                Set_xishu+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                Set_xishu&=0x0ffff0ff;
                Set_xishu+=i;
                break;
            case 6:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                Set_xishu&=0x0fffff0f;
                Set_xishu+=i;
                break;
            case 7:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                Set_xishu&=0x0ffffff0;
                Set_xishu+=i;
                break;
            }
            break;
        case 21:
            if(Menu_lever3_Pub_adj_cnt==0xff)
                Menu_lever3_Pub_adj_cnt=9;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=24;
                set_cal_data&=0x00ffffff;
                set_cal_data+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=20;
                set_cal_data&=0x0f0fffff;
                set_cal_data+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                set_cal_data&=0x0ff0ffff;
                set_cal_data+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                set_cal_data&=0x0fff0fff;
                set_cal_data+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                set_cal_data&=0x0ffff0ff;
                set_cal_data+=i;
                break;
            case 6:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                set_cal_data&=0x0fffff0f;
                set_cal_data+=i;
                break;
            case 7:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                set_cal_data&=0x0ffffff0;
                set_cal_data+=i;
                break;
            }
            break;
        case 19:
            if(Menu_lever3_point<3)
            {
                ++Menu_lever3_point;
                ++Menu_lever3_select;
            }
            else
            {
                Menu_lever3_point=Menu_lever3_select=1;
            }
            Disp_lever3_menu();
            break;
        }
        if(Menu_index_finc_select==13)
        {
            if(Menu_lever3_Pub_select_cnt<4)
                Menu1_adj_valu();
        }
        else
            Menu1_adj_valu();
        break;
//---------------------------------------------------
    case 0x04: 					//�ֹ�����-��������
        if(M_menur1_language_select_temp==2)  i=4;
        else i=6;
        switch(Menu_index_finc_select)
        {
        case 0x01:
            if((Menu_lever3_point<i)&&(Menu_lever3_point<MENU_LV331_NUM))
            {
                ++Menu_lever3_point;
                ++Menu_lever3_select;
            }
            else if((Menu_lever3_select<MENU_LV331_NUM)&&(Menu_lever3_point==i))
            {
                ++Menu_lever3_select;
            }
            else
            {
                Menu_lever3_point=Menu_lever3_select=1;
            }
            Display_menu_index();
            break;
        case 0x02:
            if((Menu_lever3_point<i)&&(Menu_lever3_point<MENU_LV332_NUM))
            {
                ++Menu_lever3_point;
                ++Menu_lever3_select;
            }
            else if((Menu_lever3_select<MENU_LV332_NUM)&&(Menu_lever3_point==i))
            {
                ++Menu_lever3_select;
            }
            else
            {
                Menu_lever3_point=Menu_lever3_select=1;
            }
            Display_menu_index();
            break;
        case 0x03:
            if((Menu_lever3_point<i)&&(Menu_lever3_point<MENU_LV333_NUM))
            {
                ++Menu_lever3_point;
                ++Menu_lever3_select;
            }
            else if((Menu_lever3_select<MENU_LV333_NUM)&&(Menu_lever3_point==i))
            {
                ++Menu_lever3_select;
            }
            else
            {
                Menu_lever3_point=Menu_lever3_select=1;
            }
            Display_menu_index();
            break;
        case 0x04:
/////                   data_buff_t[0]=0;
/////                   data_buff_t[1]=0;
/////                   data_buff_t[2]=0;
/////                   for(i=0;i<4;i++)
/////                   {
/////                   	Display_8x16_char(2*i,72,3,data_buff_t);
/////                   }
            if((Menu_lever3_point<i)&&(Menu_lever3_point<MENU_LV334_NUM))
            {
                ++Menu_lever3_point;
                ++Menu_lever3_select;
            }
            else if((Menu_lever3_select<MENU_LV334_NUM)&&(Menu_lever3_point==i))
            {
                ++Menu_lever3_select;
            }
            else
            {
                Menu_lever3_point=Menu_lever3_select=1;
            }
            Display_menu_index();
            break;
        case 0x05:
            if((Menu_lever3_point<i)&&(Menu_lever3_point<MENU_LV335_NUM))
            {
                ++Menu_lever3_point;
                ++Menu_lever3_select;
                Display_menu_index();
            }
            else if((Menu_lever3_select<MENU_LV335_NUM)&&(Menu_lever3_point==i))
            {
                ++Menu_lever3_select;
                Display_menu_index();
            }
            else
            {
                Menu_lever3_point=Menu_lever3_select=1;
                Display_menu_index();
            }
            Display_menu_index();
            break;
        case 0x06:
            if((Menu_lever3_point<i)&&(Menu_lever3_point<MENU_LV336_NUM))
            {
                ++Menu_lever3_point;
                ++Menu_lever3_select;
            }
            else if((Menu_lever3_select<MENU_LV336_NUM)&&(Menu_lever3_point==i))
            {
                ++Menu_lever3_select;
            }
            else
            {
                Menu_lever3_point=Menu_lever3_select=1;
            }
            Display_menu_index();
            break;
        case 0x07:
            if((Menu_lever3_point<i)&&(Menu_lever3_point<MENU_LV337_NUM))
            {
                ++Menu_lever3_point;
                ++Menu_lever3_select;
            }
            else if((Menu_lever3_select<MENU_LV337_NUM)&&(Menu_lever3_point==i))
            {
                ++Menu_lever3_select;
            }
            else
            {
                Menu_lever3_point=Menu_lever3_select=1;
            }
            Display_menu_index();
            break;
        case 0x08:
            if((Menu_lever3_point<i)&&(Menu_lever3_point<MENU_LV338_NUM))
            {
                ++Menu_lever3_point;
                ++Menu_lever3_select;
            }
            else if((Menu_lever3_select<MENU_LV338_NUM)&&(Menu_lever3_point==i))
            {
                ++Menu_lever3_select;
            }
            else
            {
                Menu_lever3_point=Menu_lever3_select=1;
            }
            Display_menu_index();
            break;
        case 0x09:
            if((Menu_lever3_point<i)&&(Menu_lever3_point<MENU_LV339_NUM))
            {
                ++Menu_lever3_point;
                ++Menu_lever3_select;
            }
            else if((Menu_lever3_select<MENU_LV339_NUM)&&(Menu_lever3_point==i))
            {
                ++Menu_lever3_select;
            }
            else
            {
                Menu_lever3_point=Menu_lever3_select=1;
            }
            Display_menu_index();
            break;
        }
        break;
    case 0x05: 					//�鿴��ʷ��¼
        switch(Menu_index_finc_select)
        {
        case 1:
            if(disp_sec_screen_flag==2)
            {
                --disp_sec_screen_flag;
                Disp_history_data(1,&Fenxi);
            }
            else
            {
                disp_sec_screen_flag=1;
                Fenxi.disp_index+=1;
                Read_result_from_flash(1);
            }
            break;
        case 2:
            if(disp_sec_screen_flag==2)
            {
                --disp_sec_screen_flag;
                Disp_history_data(2,&Jiaozheng);
            }
            else
            {
                disp_sec_screen_flag=1;
                Jiaozheng.disp_index+=1;
                Read_result_from_flash(2);
            }
            break;
        case 3:
            if(disp_sec_screen_flag==2)
            {
                --disp_sec_screen_flag;
                Disp_history_data(4,&Baojing);
            }
            else
            {
                disp_sec_screen_flag=1;
                Baojing.disp_index+=1;
                Read_result_from_flash(4);
            }
            break;
        case 4:
            if(disp_sec_screen_flag==2)
            {
                --disp_sec_screen_flag;
                Disp_history_data(3,&Cuowu);
            }
            else
            {
                disp_sec_screen_flag=1;
                Cuowu.disp_index+=1;
                Read_result_from_flash(3);
            }
            break;
        }
        break;
//------------------------------------------------
    case 0x01: 					//��ʼ���ַ�������-��������
        if(M_menur1_language_select_temp==2)  i=4;
        else i=6;
        switch(Menu_index_finc_select)
        {
        case 0x01:
            if((Menu_lever3_point<i)&&(Menu_lever3_point<MENU_LV351_NUM))
            {
                ++Menu_lever3_point;
                ++Menu_lever3_select;
                Display_menu_index();
//                      Menu_lever3_start_flag&=~BIT0;
            }
            else if((Menu_lever3_select<MENU_LV351_NUM)&&(Menu_lever3_point==i))
            {
                ++Menu_lever3_select;
                Display_menu_index();
                if((Menu_lever3_select==6)&&(Prog_disp_flag==0))
                {
                    __nop();
////                      	if(Menu_lever3_start_flag<0x10)
////                      	Menu_lever3_start_flag=BIT0;
                }
            }
            else
            {
                Menu_lever3_point=Menu_lever3_select=1;
//                      Menu_lever3_start_flag&=~BIT0;
                Display_menu_index();
            }
            break;
        case 0x02:
            if((Menu_lever3_point<i)&&(Menu_lever3_point<MENU_LV352_NUM))
            {
                ++Menu_lever3_point;
                ++Menu_lever3_select;
////                      Menu_lever3_start_flag&=~BIT1;
                if((Menu_lever3_select==2)&&(Prog_disp_flag==0))
                {
                    __nop();
////                      	if((Menu_lever3_start_flag<0x10)&&(Auto_check_clock_flag!=1))
////                      	Menu_lever3_start_flag=BIT1;
                }
            }
            else if((Menu_lever3_select<MENU_LV352_NUM)&&(Menu_lever3_point==i))
                ++Menu_lever3_select;
            else
            {
                Menu_lever3_point=Menu_lever3_select=1;
////                      Menu_lever3_start_flag&=~BIT1;
            }
            Display_menu_index();
            break;
        case 0x03:
            if((Menu_lever3_point<i)&&(Menu_lever3_point<MENU_LV353_NUM))
            {
                ++Menu_lever3_point;
                ++Menu_lever3_select;
////                      Menu_lever3_start_flag&=~BIT2;
                if((Menu_lever3_select==3)&&(Prog_disp_flag==0))
                {
                    __nop();
////                      	if((Menu_lever3_start_flag<0x10)&&(Auto_check_clock_flag!=1))
////                      	Menu_lever3_start_flag=BIT2;
                }
            }
            else if((Menu_lever3_select<MENU_LV353_NUM)&&(Menu_lever3_point==i))
                ++Menu_lever3_select;
            else
            {
                Menu_lever3_point=Menu_lever3_select=1;
////                      Menu_lever3_start_flag&=~BIT2;
            }
            Display_menu_index();
            break;
        case 0x04:
            if((Menu_lever3_point<i)&&(Menu_lever3_point<MENU_LV354_NUM))
            {
                ++Menu_lever3_point;
                ++Menu_lever3_select;
////                      Menu_lever3_start_flag&=~BIT3;
                if((Menu_lever3_select==2)&&(Prog_disp_flag==0))
                {
                    __nop();
////                      	if((Menu_lever3_start_flag<0x10)&&(Auto_check_clock_flag!=1))
////                      	Menu_lever3_start_flag=BIT3;
                }
            }
            else if((Menu_lever3_select<MENU_LV354_NUM)&&(Menu_lever3_point==i))
                ++Menu_lever3_select;
            else
            {
                Menu_lever3_point=Menu_lever3_select=1;
////                      Menu_lever3_start_flag&=~BIT3;
            }
            Display_menu_index();
            break;
        }
        break;
    case 0x06: 					//�����������-��������
        if(M_menur1_language_select_temp==2)  i=4;
        else i=6;
        switch(Menu_index_finc_select)
        {
        case 0x01:
            if((Menu_lever3_point<i)&&(Menu_lever3_point<MENU_LV361_NUM))
            {
                ++Menu_lever3_point;
                ++Menu_lever3_select;
            }
            else if((Menu_lever3_select<MENU_LV361_NUM)&&(Menu_lever3_point==i))
                ++Menu_lever3_select;
            else
                Menu_lever3_point=Menu_lever3_select=1;

            Display_menu_index();
            break;
        case 0x02:
            if((Menu_lever3_point<i)&&(Menu_lever3_point<MENU_LV362_NUM))
            {
                ++Menu_lever3_point;
                ++Menu_lever3_select;
            }
            else if((Menu_lever3_select<MENU_LV362_NUM)&&(Menu_lever3_point==i))
                ++Menu_lever3_select;
            else
                Menu_lever3_point=Menu_lever3_select=1;

            Display_menu_index();
            break;
        case 0x03:
            if((Menu_lever3_point<i)&&(Menu_lever3_point<MENU_LV363_NUM))
            {
                ++Menu_lever3_point;
                ++Menu_lever3_select;
            }
            else if((Menu_lever3_select<MENU_LV363_NUM)&&(Menu_lever3_point==i))
                ++Menu_lever3_select;
            else
                Menu_lever3_point=Menu_lever3_select=1;

            Display_menu_index();
            break;
        case 0x04:
            if((Menu_lever3_point<i)&&(Menu_lever3_point<MENU_LV364_NUM))
            {
                ++Menu_lever3_point;
                ++Menu_lever3_select;
            }
            else if((Menu_lever3_select<MENU_LV364_NUM)&&(Menu_lever3_point==i))
                ++Menu_lever3_select;
            else
                Menu_lever3_point=Menu_lever3_select=1;

            Display_menu_index();
            break;
////               case 0x05:
////               case 0x06:
////                   Disp_16x16_area(2*(Menu_lever3_point-1),0,0,TB_clr_dot);
////               break;
        }
        break;
    }
}

//===============================================================================
//	finction	:Key_Menu_lever3_down_finc
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.07.10
//	modefy		:null
//===============================================================================
void Key_Menu_lever3_down_finc(void)
{
    uint8_t  j;
    uint32_t i;
////    uint8_t  data_buff_t[3];

    switch(Menu_index_select)
    {
    case 7: 					//ϵͳ����-��������
        if(Menu_index_finc_select==1)
        {
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt<3)
                {
                    ++Menu_lever3_Pub_adj_cnt;
                    if(Menu_lever3_Pub_adj_cnt==3)
                        Menu_lever3_Pub_adj_cnt=0;
                }

                j=Menu_lever3_Pub_adj_cnt;
                j<<=4;
                RTC_Set.hour_temp&=0x0f;
                RTC_Set.hour_temp|=j;
                break;
            case 2:
                i=RTC_Set.hour_temp;
                i&=0x20;
                if(i==0x20)
                {
                    if(Menu_lever3_Pub_adj_cnt>4)
                        Menu_lever3_Pub_adj_cnt=0;

                    else if(Menu_lever3_Pub_adj_cnt<4)
                    {
                        ++Menu_lever3_Pub_adj_cnt;
                        if(Menu_lever3_Pub_adj_cnt==4)
                            Menu_lever3_Pub_adj_cnt=0;
                    }
                }
                else if(Menu_lever3_Pub_adj_cnt<10)
                {
                    ++Menu_lever3_Pub_adj_cnt;
                    if(Menu_lever3_Pub_adj_cnt==10)
                        Menu_lever3_Pub_adj_cnt=0;
                }
                RTC_Set.hour_temp&=0xf0;
                RTC_Set.hour_temp+=Menu_lever3_Pub_adj_cnt;
                break;
            case 3:
                if(Menu_lever3_Pub_adj_cnt<6)
                {
                    ++Menu_lever3_Pub_adj_cnt;
                    if(Menu_lever3_Pub_adj_cnt==6)
                        Menu_lever3_Pub_adj_cnt=0;
                }
                j=Menu_lever3_Pub_adj_cnt;
                j<<=4;
                RTC_Set.minu_temp&=0x0f;
                RTC_Set.minu_temp|=j;
                break;
            case 4:
                if(Menu_lever3_Pub_adj_cnt<10)
                {
                    ++Menu_lever3_Pub_adj_cnt;
                    if(Menu_lever3_Pub_adj_cnt==10)
                        Menu_lever3_Pub_adj_cnt=0;
                }
                RTC_Set.minu_temp&=0xf0;
                RTC_Set.minu_temp+=Menu_lever3_Pub_adj_cnt;
                break;
            }
        }
        else if(Menu_index_finc_select==2)
        {
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt<10)
                {
                    ++Menu_lever3_Pub_adj_cnt;
                    if(Menu_lever3_Pub_adj_cnt==10)
                        Menu_lever3_Pub_adj_cnt=0;
                }

                j=Menu_lever3_Pub_adj_cnt;
                j<<=4;
                RTC_Set.year_temp&=0x0f;
                RTC_Set.year_temp|=j;
                break;
            case 2:
                if(Menu_lever3_Pub_adj_cnt<10)
                {
                    ++Menu_lever3_Pub_adj_cnt;
                    if(Menu_lever3_Pub_adj_cnt==10)
                        Menu_lever3_Pub_adj_cnt=0;
                }
                RTC_Set.year_temp&=0xf0;
                RTC_Set.year_temp+=Menu_lever3_Pub_adj_cnt;
                break;
            case 3:
                if(Menu_lever3_Pub_adj_cnt<2)
                {
                    ++Menu_lever3_Pub_adj_cnt;
                    if(Menu_lever3_Pub_adj_cnt==2)
                        Menu_lever3_Pub_adj_cnt=0;
                }
                j=Menu_lever3_Pub_adj_cnt;
                j<<=4;
                RTC_Set.mouth_temp&=0x0f;
                RTC_Set.mouth_temp|=j;
                break;
            case 4:
                i=RTC_Set.mouth_temp;
                i&=0x10;
                if(i==0x10)
                {
                    if(Menu_lever3_Pub_adj_cnt>3)
                        Menu_lever3_Pub_adj_cnt=0;

                    else if(Menu_lever3_Pub_adj_cnt<3)
                    {
                        ++Menu_lever3_Pub_adj_cnt;
                        if(Menu_lever3_Pub_adj_cnt==3)
                            Menu_lever3_Pub_adj_cnt=0;
                    }
                }
                else if(Menu_lever3_Pub_adj_cnt<10)
                {
                    ++Menu_lever3_Pub_adj_cnt;
                    if(Menu_lever3_Pub_adj_cnt==10)
                        Menu_lever3_Pub_adj_cnt=1;
                }
                RTC_Set.mouth_temp&=0x10;
                RTC_Set.mouth_temp+=Menu_lever3_Pub_adj_cnt;
                break;
            case 5:
                if(Menu_lever3_Pub_adj_cnt<4)
                {
                    ++Menu_lever3_Pub_adj_cnt;
                    if(Menu_lever3_Pub_adj_cnt==4)
                        Menu_lever3_Pub_adj_cnt=0;
                }
                j=Menu_lever3_Pub_adj_cnt;
                j<<=4;
                RTC_Set.day_temp&=0x0f;
                RTC_Set.day_temp|=j;
                break;
            case 6:
                i=RTC_Set.day_temp;
                i&=0x30;
                if(i==0x30)
                {
                    if(Menu_lever3_Pub_adj_cnt>2)
                        Menu_lever3_Pub_adj_cnt=0;
                    else if(Menu_lever3_Pub_adj_cnt<2)
                    {
                        ++Menu_lever3_Pub_adj_cnt;
                        if(Menu_lever3_Pub_adj_cnt==2)
                            Menu_lever3_Pub_adj_cnt=0;
                    }
                }
                else if(i==0)
                {
                    ++Menu_lever3_Pub_adj_cnt;
                    if(Menu_lever3_Pub_adj_cnt==10)
                        Menu_lever3_Pub_adj_cnt=1;
                }
                else if(Menu_lever3_Pub_adj_cnt<10)
                {
                    ++Menu_lever3_Pub_adj_cnt;
                    if(Menu_lever3_Pub_adj_cnt==10)
                        Menu_lever3_Pub_adj_cnt=0;
                }
                RTC_Set.day_temp&=0x30;
                RTC_Set.day_temp+=Menu_lever3_Pub_adj_cnt;
                break;
            }
        }
        else if(Menu_index_finc_select==5)
        {
            ++Menu_lever3_Pub_adj_cnt;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt>6)
                    Menu_lever3_Pub_adj_cnt=0;

                i=Menu_lever3_Pub_adj_cnt<<4;
                M_menur1_lcd_Contrast_temp&=0x0f;
                M_menur1_lcd_Contrast_temp+=i;
                break;
            case 2:
                if((M_menur1_lcd_Contrast_temp>=0x60)&&(Menu_lever3_Pub_adj_cnt>4))
                    Menu_lever3_Pub_adj_cnt=0;
                else if(Menu_lever3_Pub_adj_cnt>9)
                    Menu_lever3_Pub_adj_cnt=0;

                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_lcd_Contrast_temp&=0xf0;
                M_menur1_lcd_Contrast_temp+=i;
                break;
            }
//////              Adj_lcd_Contrast_data();
        }
        else if(Menu_index_finc_select==6)
        {
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt<2)
                {
                    ++Menu_lever3_Pub_adj_cnt;
                    if(Menu_lever3_Pub_adj_cnt==3)
                        Menu_lever3_Pub_adj_cnt=0;
                }

                i=Menu_lever3_Pub_adj_cnt<<8;
                back_l_temp&=0x000000ff;
                back_l_temp+=i;

                Back_light_time=0;
                i=back_l_temp>>8;
                i&=0x0000000f;
                i*=10000;
                Back_light_time+=i;
                i=back_l_temp>>4;
                i&=0x0000000f;
                i*=1000;
                Back_light_time+=i;
                i=back_l_temp;
                i&=0x0000000f;
                i*=100;
                Back_light_time+=i;
                break;
            case 2:
                ++Menu_lever3_Pub_adj_cnt;
                if((Back_light_time/10000)==2)
                {
                    if(Menu_lever3_Pub_adj_cnt>5)
                        Menu_lever3_Pub_adj_cnt=0;
                }
                else if(Menu_lever3_Pub_adj_cnt>9)
                    Menu_lever3_Pub_adj_cnt=0;

                i=Menu_lever3_Pub_adj_cnt<<4;
                back_l_temp&=0x00000f0f;
                back_l_temp+=i;

                Back_light_time=0;
                i=back_l_temp>>8;
                i&=0x0000000f;
                i*=10000;
                Back_light_time+=i;
                i=back_l_temp>>4;
                i&=0x0000000f;
                i*=1000;
                Back_light_time+=i;
                i=back_l_temp;
                i&=0x0000000f;
                i*=100;
                Back_light_time+=i;
                break;
            case 3:
                ++Menu_lever3_Pub_adj_cnt;
                if((Back_light_time/1000)==25)
                {
                    if(Menu_lever3_Pub_adj_cnt>5)
                        Menu_lever3_Pub_adj_cnt=0;
                }
                else if(Menu_lever3_Pub_adj_cnt>9)
                    Menu_lever3_Pub_adj_cnt=0;

                i=Menu_lever3_Pub_adj_cnt;
                back_l_temp&=0x00000ff0;
                back_l_temp+=i;

                Back_light_time=0;
                i=back_l_temp>>8;
                i&=0x0000000f;
                i*=10000;
                Back_light_time+=i;
                i=back_l_temp>>4;
                i&=0x0000000f;
                i*=1000;
                Back_light_time+=i;
                i=back_l_temp;
                i&=0x0000000f;
                i*=100;
                Back_light_time+=i;
                break;
            }
        }
        else if(Menu_index_finc_select==7)
        {
            if(Menu_lever3_Pub_adj_cnt<5)
                ++Menu_lever3_Pub_adj_cnt;
            Set_back_light_up();
        }
        Menu1_adj_valu();
        break;
//----------------------------------------------
    case 2: 					//��������-�������� 							//
        ++Menu_lever3_Pub_adj_cnt;
        if(Menu_lever3_Pub_adj_cnt>=10)
            Menu_lever3_Pub_adj_cnt=0;

        switch(Menu_index_finc_select)
        {
        case 2:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                M_menur1_lange_data_temp&=0x0000ffff;
                M_menur1_lange_data_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_menur1_lange_data_temp&=0x000f0fff;
                M_menur1_lange_data_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_lange_data_temp&=0x000ff0ff;
                M_menur1_lange_data_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_lange_data_temp&=0x000fff0f;
                M_menur1_lange_data_temp+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_lange_data_temp&=0x000ffff0;
                M_menur1_lange_data_temp+=i;
                break;
            }
            break;
        case 5:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_QYSJ_temp&=0x000000ff;
                M_menur1_QYSJ_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_QYSJ_temp&=0x00000f0f;
                M_menur1_QYSJ_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_QYSJ_temp&=0x00000ff0;
                M_menur1_QYSJ_temp+=i;
                break;
            }
            break;
        case 4:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_CSSJ_temp&=0x000000ff;
                M_menur1_CSSJ_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_CSSJ_temp&=0x00000f0f;
                M_menur1_CSSJ_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_CSSJ_temp&=0x00000ff0;
                M_menur1_CSSJ_temp+=i;
                break;
            }
            break;
        case 6:
            M_menur1_RX_time_temp=Menu_lever3_Pub_adj_cnt;
            break;
        case 7:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt>5)
                    Menu_lever3_Pub_adj_cnt=0;

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_menur1_XHPY_temp&=0x00000fff;
                M_menur1_XHPY_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_XHPY_temp&=0x0000f0ff;
                M_menur1_XHPY_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_XHPY_temp&=0x0000ff0f;
                M_menur1_XHPY_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_XHPY_temp&=0x0000fff0;
                M_menur1_XHPY_temp+=i;
                break;
            }
            break;
        case 8:			//
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                E_wait_time_temp&=0x000000ff;
                E_wait_time_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                E_wait_time_temp&=0x000000f0f;
                E_wait_time_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                E_wait_time_temp&=0x00000ff0;
                E_wait_time_temp+=i;
                break;
            }
            break;
        case 9:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_menur1_DDSJ_temp&=0x00000fff;
                M_menur1_DDSJ_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_DDSJ_temp&=0x0000f0ff;
                M_menur1_DDSJ_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_DDSJ_temp&=0x0000ff0f;
                M_menur1_DDSJ_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_DDSJ_temp&=0x0000fff0;
                M_menur1_DDSJ_temp+=i;
                break;
            }
            break;
        case 13:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                Allow_error&=0x00000fff;
                Allow_error+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                Allow_error&=0x0000f0ff;
                Allow_error+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                Allow_error&=0x0000ff0f;
                Allow_error+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                Allow_error&=0x0000fff0;
                Allow_error+=i;
                break;
            }
            break;
        case 14:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
//                             if(Menu_lever3_Pub_adj_cnt>6)
//                                Menu_lever3_Pub_adj_cnt=0;
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                M_lever3_HCPD_temp&=0x0000ffff;
                M_lever3_HCPD_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_lever3_HCPD_temp&=0x000f0fff;
                M_lever3_HCPD_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_lever3_HCPD_temp&=0x000ff0ff;
                M_lever3_HCPD_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_lever3_HCPD_temp&=0x000fff0f;
                M_lever3_HCPD_temp+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_lever3_HCPD_temp&=0x000ffff0;
                M_lever3_HCPD_temp+=i;
                break;
            }
            break;
        case 15:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                Alarm_low_nd&=0x0000ffff;
                Alarm_low_nd+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                Alarm_low_nd&=0x000f0fff;
                Alarm_low_nd+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                Alarm_low_nd&=0x000ff0ff;
                Alarm_low_nd+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                Alarm_low_nd&=0x000fff0f;
                Alarm_low_nd+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                Alarm_low_nd&=0x0000fff0;
                Alarm_low_nd+=i;
                break;
            }
            break;
        case 16:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                Alarm_hight_nd&=0x0000ffff;
                Alarm_hight_nd+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                Alarm_hight_nd&=0x000f0fff;
                Alarm_hight_nd+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                Alarm_hight_nd&=0x000ff0ff;
                Alarm_hight_nd+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                Alarm_hight_nd&=0x000fff0f;
                Alarm_hight_nd+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                Alarm_hight_nd&=0x0000fff0;
                Alarm_hight_nd+=i;
                break;
            }
            break;
        }
        Menu1_adj_valu();
        break;
//---------------------------------------------
    case 3: 					//��������-��������
        ++Menu_lever3_Pub_adj_cnt;
        if(Menu_lever3_Pub_adj_cnt>=10)
        {
            Menu_lever3_Pub_adj_cnt=0;
        }
        switch(Menu_index_finc_select)
        {
        case 3:			//LED ����
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt==6)		//50.000mA
                    Menu_lever3_Pub_adj_cnt=0;

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                M_menur1_led_dianliu_temp&=0x0000ffff;
                M_menur1_led_dianliu_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_menur1_led_dianliu_temp&=0x000f0fff;
                M_menur1_led_dianliu_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_led_dianliu_temp&=0x000ff0ff;
                M_menur1_led_dianliu_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_led_dianliu_temp&=0x000fff0f;
                M_menur1_led_dianliu_temp+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_led_dianliu_temp&=0x000ffff0;
                M_menur1_led_dianliu_temp+=i;
                break;
            }
            break;
        case 4:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_menur1_led_data_temp&=0x0fff;
                M_menur1_led_data_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_led_data_temp&=0xf0ff;
                M_menur1_led_data_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_led_data_temp&=0xff0f;
                M_menur1_led_data_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_led_data_temp&=0xfff0;
                M_menur1_led_data_temp+=i;
                break;
            }
            break;
        case 5:
#ifdef __OLD_JBSPEED__
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt==3)		//240
                    Menu_lever3_Pub_adj_cnt=0;

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_JB_speed_temp&=0x000000ff;
                M_menur1_JB_speed_temp+=i;
                break;
            case 2:
                i=M_menur1_JB_speed_temp>>8;
                i&=0x0f;
                if(i==2)
                {
                    if(Menu_lever3_Pub_adj_cnt>=5)
                        Menu_lever3_Pub_adj_cnt=0;
                }
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_JB_speed_temp&=0x00000f0f;
                M_menur1_JB_speed_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_JB_speed_temp&=0x00000ff0;
                M_menur1_JB_speed_temp+=i;
                break;
            }
#else
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt>2)		//235
                    Menu_lever3_Pub_adj_cnt=1;

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_JB_speed_temp&=0x000000ff;
                M_menur1_JB_speed_temp+=i;

                Check_JB_speed_edag();
                break;
            case 2:
                i=M_menur1_JB_speed_temp>>8;
                i&=0x0f;
                if(i==2)
                {
                    if(Menu_lever3_Pub_adj_cnt>3)
                        Menu_lever3_Pub_adj_cnt=0;
                }
                if(i==1)
                {
                    if(Menu_lever3_Pub_adj_cnt<7)
                        Menu_lever3_Pub_adj_cnt=7;
                }

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_JB_speed_temp&=0x00000f0f;
                M_menur1_JB_speed_temp+=i;

                Check_JB_speed_edag();
                break;
            case 3:
                i=M_menur1_JB_speed_temp>>4;
                if(i==0x23)
                {
                    if(Menu_lever3_Pub_adj_cnt>5)
                        Menu_lever3_Pub_adj_cnt=0;
                }
                else if(i==0x17)
                {
                    if(Menu_lever3_Pub_adj_cnt<5)
                        Menu_lever3_Pub_adj_cnt=9;
                }

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_JB_speed_temp&=0x00000ff0;
                M_menur1_JB_speed_temp+=i;
                break;
            }
#endif
            break;
        case 7:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt==3)
                    Menu_lever3_Pub_adj_cnt=1;
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_ZSB_speed_temp&=0x0000000f;
                M_menur1_ZSB_speed_temp+=i;

                if(M_menur1_ZSB_speed_temp>0x00000027)
                {
                    M_menur1_ZSB_speed_temp=0x00000027;

                    i=M_menur1_ZSB_speed_temp;
                    i&=0x0f;
                    i=Get_ascii_data(i);

                    Get_6x8_char_single(3,36,i);
                    Display_6x8_char_single(3,36,i);
                }
                break;
            case 2:
                i=M_menur1_ZSB_speed_temp>>4;
                i&=0x0f;
                if(i==2)
                {
                    if(Menu_lever3_Pub_adj_cnt>=8)
                        Menu_lever3_Pub_adj_cnt=0;
                }

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_ZSB_speed_temp&=0x000000f0;
                M_menur1_ZSB_speed_temp+=i;
                break;
            }
            break;
        case 9:		//��Ʒ��� 20.00
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt==3)
                    Menu_lever3_Pub_adj_cnt=0;

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_menur1_sample_v_temp&=0x00000fff;
                M_menur1_sample_v_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_sample_v_temp&=0x0000f0ff;
                M_menur1_sample_v_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_sample_v_temp&=0x0000ff0f;
                M_menur1_sample_v_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_sample_v_temp&=0x0000fff0;
                M_menur1_sample_v_temp+=i;
                break;
            }
            break;
        case 10:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                M_menur1_jz1nd_temp&=0x0000ffff;
                M_menur1_jz1nd_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_menur1_jz1nd_temp&=0x000f0fff;
                M_menur1_jz1nd_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_jz1nd_temp&=0x000ff0ff;
                M_menur1_jz1nd_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_jz1nd_temp&=0x000fff0f;
                M_menur1_jz1nd_temp+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_jz1nd_temp&=0x000ffff0;
                M_menur1_jz1nd_temp+=i;
                break;
            }
            break;
        case 11:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_Pub_adj_cnt==3)
                    Menu_lever3_Pub_adj_cnt=0;

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_menur1_jz1_v_temp&=0x00000fff;
                M_menur1_jz1_v_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_jz1_v_temp&=0x0000f0ff;
                M_menur1_jz1_v_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_jz1_v_temp&=0x0000ff0f;
                M_menur1_jz1_v_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_jz1_v_temp&=0x0000fff0;
                M_menur1_jz1_v_temp+=i;
                break;
            }
            break;
        case 12:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                M_menur1_jz2nd_temp&=0x0000ffff;
                M_menur1_jz2nd_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                M_menur1_jz2nd_temp&=0x000f0fff;
                M_menur1_jz2nd_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_jz2nd_temp&=0x000ff0ff;
                M_menur1_jz2nd_temp+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_jz2nd_temp&=0x000fff0f;
                M_menur1_jz2nd_temp+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_jz2nd_temp&=0x000ffff0;
                M_menur1_jz2nd_temp+=i;
                break;
            }
            break;
        case 13:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                M_menur1_jz2_v_temp&=0x000000ff;
                M_menur1_jz2_v_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_jz2_v_temp&=0x00000f0f;
                M_menur1_jz2_v_temp+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_jz2_v_temp&=0x00000ff0;
                M_menur1_jz2_v_temp+=i;
                break;
            }
            break;
        case 14:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_sj1_v_temp&=0x0000000f;
                M_menur1_sj1_v_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_sj1_v_temp&=0x000000f0;
                M_menur1_sj1_v_temp+=i;
                break;
            }
            break;
        case 15:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_sj2_v_temp&=0x0000000f;
                M_menur1_sj2_v_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_sj2_v_temp&=0x000000f0;
                M_menur1_sj2_v_temp+=i;
                break;
            }
            break;
        case 16:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_sj3_v_temp&=0x0000000f;
                M_menur1_sj3_v_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_sj3_v_temp&=0x000000f0;
                M_menur1_sj3_v_temp+=i;
                break;
            }
            break;
        case 17:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                M_menur1_sj4_v_temp&=0x0000000f;
                M_menur1_sj4_v_temp+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                M_menur1_sj4_v_temp&=0x000000f0;
                M_menur1_sj4_v_temp+=i;
                break;
            }
            break;
        case 18:			//
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=2;
                i<<=28;
                Prj_correlat&=0x0fffffff;
                Prj_correlat+=i;
                break;
            case 2:
                if((Menu_lever3_Pub_adj_cnt==0)&&(Prj_correlat<10))
                    Menu_lever3_Pub_adj_cnt=1;

                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=20;
                Prj_correlat&=0x000fffff;
                Prj_correlat+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                Prj_correlat&=0x00f0ffff;
                Prj_correlat+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                Prj_correlat&=0x00ff0fff;
                Prj_correlat+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                Prj_correlat&=0x00fff0ff;
                Prj_correlat+=i;
                break;
            case 6:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                Prj_correlat&=0x00ffff0f;
                Prj_correlat+=i;
                break;
            case 7:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                Prj_correlat&=0x00fffff0;
                Prj_correlat+=i;
                break;
            }
            break;
        case 20:			//
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=24;
                Set_xishu&=0x00ffffff;
                Set_xishu+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=20;
                Set_xishu&=0x0f0fffff;
                Set_xishu+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                Set_xishu&=0x0ff0ffff;
                Set_xishu+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                Set_xishu&=0x0fff0fff;
                Set_xishu+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                Set_xishu&=0x0ffff0ff;
                Set_xishu+=i;
                break;
            case 6:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                Set_xishu&=0x0fffff0f;
                Set_xishu+=i;
                break;
            case 7:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                Set_xishu&=0x0ffffff0;
                Set_xishu+=i;
                break;
            }
            break;
        case 21:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=24;
                set_cal_data&=0x00ffffff;
                set_cal_data+=i;
                break;
            case 2:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=20;
                set_cal_data&=0x0f0fffff;
                set_cal_data+=i;
                break;
            case 3:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=16;
                set_cal_data&=0x0ff0ffff;
                set_cal_data+=i;
                break;
            case 4:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=12;
                set_cal_data&=0x0fff0fff;
                set_cal_data+=i;
                break;
            case 5:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=8;
                set_cal_data&=0x0ffff0ff;
                set_cal_data+=i;
                break;
            case 6:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                i<<=4;
                set_cal_data&=0x0fffff0f;
                set_cal_data+=i;
                break;
            case 7:
                i=0;
                i=Menu_lever3_Pub_adj_cnt;
                set_cal_data&=0x0ffffff0;
                set_cal_data+=i;
                break;
            }
            break;
        case 19:
            if(Menu_lever3_point>1)
            {
                --Menu_lever3_point;
                --Menu_lever3_select;
            }
            else
            {
                Menu_lever3_point=Menu_lever3_select=3;
            }
            Disp_lever3_menu();
            break;
        }
        if(Menu_index_finc_select==13)
        {
            if(Menu_lever3_Pub_select_cnt<4)
                Menu1_adj_valu();
        }
        else
            Menu1_adj_valu();
        break;
//---------------------------------------------
    case 4:					//�ֹ�����-��������
    case 1: 					//��ʼ���ַ�������-��������
    case 6: 					//�����������-��������
        if(Menu_lever3_point>1)
        {
            --Menu_lever3_point;
            --Menu_lever3_select;
            Display_menu_index();
        }
        else if((Menu_lever3_select>1)&&(Menu_lever3_point==1))
        {
            --Menu_lever3_select;
            Display_menu_index();
        }
        else
        {
            switch(Menu_index_select)
            {
            case 1:	//4
                switch(Menu_index_finc_select)
                {
                case 1:
                    if(M_menur1_language_select_temp==2)
                        Menu_lever3_point=4;
                    else
                        Menu_lever3_point=6;
                    Menu_lever3_select=MENU_LV351_NUM;
////                     	    if(Menu_lever3_start_flag<0x10)
////                            Menu_lever3_start_flag=BIT0;
                    break;
                case 2:
                    Menu_lever3_point=Menu_lever3_select=MENU_LV352_NUM;
////                     	    if((Menu_lever3_start_flag<0x10)&&(Auto_check_clock_flag!=1))
////                            Menu_lever3_start_flag=BIT1;
                    break;
                case 3:
                    Menu_lever3_point=Menu_lever3_select=MENU_LV353_NUM;
////                     	    if((Menu_lever3_start_flag<0x10)&&(Auto_check_clock_flag!=1))
////                            Menu_lever3_start_flag=BIT2;
                    break;
                case 4:
                    Menu_lever3_point=Menu_lever3_select=MENU_LV354_NUM;
////                     	    if((Menu_lever3_start_flag<0x10)&&(Auto_check_clock_flag!=1))
////                            Menu_lever3_start_flag=BIT3;
                    break;
                }
                break;
            case 4:	//8
                switch(Menu_index_finc_select)
                {
                case 1:
                    if(M_menur1_language_select_temp==2)
                        Menu_lever3_point=4;
                    else
                        Menu_lever3_point=6;
                    Menu_lever3_select=MENU_LV331_NUM;
                    break;
                case 2:
                    Menu_lever3_point=Menu_lever3_select=MENU_LV332_NUM;
                    break;
                case 5:
                    Menu_lever3_point=Menu_lever3_select=MENU_LV335_NUM;
                    break;
                case 3:
                    if(M_menur1_language_select_temp==2)
                        Menu_lever3_point=4;
                    else
                        Menu_lever3_point=6;
                    Menu_lever3_select=MENU_LV333_NUM;
                    break;
                case 4:
                    if(M_menur1_language_select_temp==2)
                        Menu_lever3_point=4;
                    else
                        Menu_lever3_point=6;
                    Menu_lever3_select=MENU_LV334_NUM;
                    break;
                case 6:
                case 7:
                case 9:
                    Menu_lever3_point=Menu_lever3_point=Menu_lever3_select=2;
                    break;
                case 8:
                    if(M_menur1_language_select_temp==2)
                        Menu_lever3_point=4;
                    else
                        Menu_lever3_point=6;
                    Menu_lever3_select=MENU_LV338_NUM;
                    break;
//                     	case 9:
//                     	    Menu_lever3_select=MENU_LV339_NUM;
//                     	break;
                }
                break;
            case 6:	//6
                switch(Menu_index_finc_select)
                {
                case 1:
                    if(M_menur1_language_select_temp==2)
                        Menu_lever3_point=4;
                    else
                        Menu_lever3_point=5;
                    Menu_lever3_select=MENU_LV361_NUM;
                    break;
                case 2:
                    Menu_lever3_point=Menu_lever3_select=MENU_LV362_NUM;
                    break;
                case 3:
                    if(M_menur1_language_select_temp==2)
                        Menu_lever3_point=4;
                    else
                        Menu_lever3_point=6;
                    Menu_lever3_select=MENU_LV363_NUM;
                    break;
                case 4:
                    Menu_lever3_point=Menu_lever3_select=MENU_LV364_NUM;
                    break;
                }
                break;

            }
            Display_menu_index();
        }
        break;
    case 5: 					//��ʷ��¼
        switch(Menu_index_finc_select)
        {
        case 1:
            if(disp_sec_screen_flag==2)
            {
                --disp_sec_screen_flag;
                Disp_history_data(1,&Fenxi);
            }
            else if(Fenxi.disp_index>1)				//2013-7-2 11:32
            {
                Fenxi.disp_index-=1;
                Read_result_from_flash_index_up(1);
            }
            break;
        case 2:
            if(disp_sec_screen_flag==2)
            {
                --disp_sec_screen_flag;
                Disp_history_data(2,&Jiaozheng);
            }
            else if(Jiaozheng.disp_index>1)			//2013-7-2 11:32
            {
                Jiaozheng.disp_index-=1;
                Read_result_from_flash_index_up(2);
            }
            break;
        case 3:
            if(disp_sec_screen_flag==2)
            {
                --disp_sec_screen_flag;
                Disp_history_data(4,&Baojing);
            }
            else if(Baojing.disp_index>1)			//2013-7-2 11:32
            {
                Baojing.disp_index-=1;
                Read_result_from_flash_index_up(4);
            }
            break;
        case 4:
            if(disp_sec_screen_flag==2)
            {
                --disp_sec_screen_flag;
                Disp_history_data(3,&Cuowu);
            }
            else if(Cuowu.disp_index>1)			//2013-7-2 11:32
            {
                Cuowu.disp_index-=1;
                Read_result_from_flash_index_up(3);
            }
            break;
        }
        break;
    }
}

//===============================================================================
//	finction	:Menu_lever3_4_enter_finction
//	input		:cmd
//	output		:null
//	return		:null
//	edit		:sam 2012.07.04
//	modefy		:null
//===============================================================================
/////uint8_t return_disp_flag;

void Menu_lever3_4_enter_finction(void)
{
    uint8_t i;
    uint16_t j,k;
    float a;

/////   if(Menu_lever_select==3)
/////      return_disp_flag=0;
    if(Menu_lever3_start_flag>=0x10)		//�Զ�������ʱ�����β˵�1,4,6
    {
        if((Menu_index_select==1)||(Menu_index_select==4)||(Menu_index_select==6))
            goto exit_34_enter_finction;
    }

    switch(Menu_index_select)
    {
    case 0x01:		//Ҫ�����Ŀ���
        if(Prog_disp_flag!=0)
            break;

        if(Menu_lever_select==3)
        {
            i=Menu_lever3_start_flag;
            switch(Menu_index_finc_select)
            {
            case 1:
                if((Menu_lever3_start_flag<0x10)&&(Menu_lever3_select==6))
                {
                    Menu_lever3_start_flag&=~BIT0;

                    Menu_lever3_start_flag=BIT4;
                    Memu1_auto_step_status=0;
                    Pro_step_status=0;
                    Auto_check_clock_flag=0;
                    Pro_auto_qingxi_freq_cnt=0;
                    Pro_auto_jiaozheng_freq_cnt=0;
                    Pro_auto_hecha_freq_cnt=0;
                    Pro_auto_time_dcnt=0;
                    Pro_wait_time_flag=2;

                    i=RTC_DATA.year_temp;
                    i&=0x7f;
                    if(M_lever4_start_year_temp>i)
                    {
                        Auto_check_clock_flag=1;
                        prg_wait_clr_flag=0;
                        Disp_rtc_clock_wait();
                    }
                    else if(M_lever4_start_year_temp==i)
                    {
                        i=RTC_DATA.mouth_temp;
                        i&=0x1f;
                        if(M_lever4_start_munth_temp>i)
                        {
                            Auto_check_clock_flag=1;
                            prg_wait_clr_flag=0;
                            Disp_rtc_clock_wait();
                        }
                        else if(M_lever4_start_munth_temp==i)
                        {
                            i=RTC_DATA.day_temp;
                            i&=0x3f;
                            if(M_lever4_start_day_temp>i)
                            {
                                Auto_check_clock_flag=1;
                                prg_wait_clr_flag=0;
                                Disp_rtc_clock_wait();
                            }
                            else if(M_lever4_start_day_temp==i)
                            {
                                i=RTC_DATA.hour_temp;
                                i&=0x3f;
                                if(M_lever4_start_hour_temp>i)
                                {
                                    Auto_check_clock_flag=1;
                                    prg_wait_clr_flag=0;
                                    Disp_rtc_clock_wait();
                                }
                                else if(M_lever4_start_hour_temp==i)
                                {
                                    i=RTC_DATA.minu_temp;
                                    i&=0x7f;
                                    if(M_lever4_start_mini_temp>i)
                                    {
                                        Auto_check_clock_flag=1;
                                        prg_wait_clr_flag=0;
                                        Disp_rtc_clock_wait();
                                    }
                                }
                            }

                        }
                    }
                }
                break;
            case 2:
                if((Menu_lever3_select==2)&&(Menu_lever3_start_flag<0x10))
                {
                    Menu_lever3_start_flag&=~BIT1;
                    Menu_lever3_start_flag=BIT5;
                    Memu1_auto_step_status=0;
                    Memu1_hand_step_status=0;
                    Pro_step_status=0;
                    M_lever4_FXCS_cnt=0;
                    Pro_wait_time_flag=2;
                    Clear_n_page_ram(0,7);
                }
                break;
            case 3:
                if((Menu_lever3_select==3)&&(Menu_lever3_start_flag<0x10))
                {
                    Menu_lever3_start_flag&=~BIT2;
                    Menu_lever3_start_flag=BIT6;
                    Memu1_auto_step_status=0;
                    Memu1_jiaozheng_step_status=0;
                    Pro_step_status=0;
                    Pro_wait_time_flag=2;
                    Clear_n_page_ram(0,7);
                }
                break;
            case 4:
                if((Menu_lever3_select==2)&&(Menu_lever3_start_flag<0x10))
                {
                    Menu_lever3_start_flag&=~BIT3;
                    Menu_lever3_start_flag=BIT7;
                    Memu1_auto_step_status=0;
                    Memu1_biaoye_step_status=0;
                    Pro_step_status=0;
                    Pro_wait_time_flag=2;
                    Clear_n_page_ram(0,7);
                }
                break;
            }
        }
        break;
    case 0x02:
        break;
    case 0x03:
        if((Menu_index_finc_select==6)&&(Prog_disp_flag==0))
        {
            j=k=0;
            i=M_menur1_led_dianliu_temp>>16;
            i&=0x0f;
            j=i*10000;
            k+=j;
            i=M_menur1_led_dianliu_temp>>12;
            i&=0x0f;
            j=i*1000;
            k+=j;
            i=M_menur1_led_dianliu_temp>>8;
            i&=0x0f;
            j=i*100;
            k+=j;
            i=M_menur1_led_dianliu_temp>>4;
            i&=0x0f;
            j=i*10;
            k+=j;
            i=M_menur1_led_dianliu_temp;
            i&=0x0f;
            k+=i;

            a=k;
            j=(a/50000.0)*2047;

            Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_LED_CURRIT_CTRL_ADDS,j,0x00);
        }
        break;
    case 0x04:		//�ֶ������Ĳ������ÿ���
        if(Prog_disp_flag!=0)
            break;

        switch(Menu_index_finc_select)
        {
        case 1:
            Hand_finction_step_select=0;
            switch(Menu_lever3_select)
            {
            case 1:
//            	     	     System_fill();
                Hand_finction_select=1;
                break;
            case 2:
//            	     	     Standar1_fill();
                Hand_finction_select=2;
                break;
            case 3:
//            	     	     Standar2_fill();
                Hand_finction_select=3;
                break;
            case 4:
//            	     	     Sample_fill();
                Hand_finction_select=7;
                break;
            case 5:
//            	     	     Reagent1_fill();
                Hand_finction_select=4;
                break;
            case 6:
//            	     	     Reagent2_fill();
                Hand_finction_select=5;
                break;
            case 7:
//            	     	     Reagent1_fill();
                Hand_finction_select=6;
                break;
            case 8:
//            	     	     Reagent2_fill();
                Hand_finction_select=8;
                break;
            }
            break;
        case 2:				//ע���
            switch(Menu_lever3_select)
            {
            case 1:			//RESET ZHU SE BUN
                Set_tx_cmd_data_finc(0xd1,0x06,0x1302,ZHUSEBUN_RESET,0x00);
                break;
            case 2:			//set speed
                if(Menu_lever_select==4)
                {
                    i=M_menur1_ZSB_speed_temp>>4;
                    i&=0x0f;
                    j=i*10;
                    i=M_menur1_ZSB_speed_temp;
                    i&=0x0f;
                    j+=i;
                    Set_tx_cmd_data_finc(0xd1,0x06,0x1300,j,0x00);
                }
                break;
            case 3:			//set move to
                if(Menu_lever_select==4)
                    Set_ZSB_move_to(M_lever4_move_to_temp);
                break;
            }
            break;
        case 3:
            if(DCF_switch_select_flag)
                Set_switch_open_close(Menu_lever3_select,Menu_lever4_Pub_select_cnt);
            else if(Menu_lever_select==4)
                DCF_switch_select_flag=1;
            break;
        case 4:
/////            	     if(Menu_lever_select==3)
/////            	     {
/////            	     	Disp_input_switch_status(Menu_lever3_select);
/////		     }
            break;
        case 5:			//����
            switch(Menu_lever3_select)
            {
            case 1:
                Set_JB_speed();
                break;
            case 2:
//            	     	     Set_JB_direction(); 		//moto on
                break;
            case 3:
                Set_JB_direction(); 		//moto on
                break;
            case 4:
                Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_MOTOR_RUN_ADDS,U_SENSER_MOTOR_STOP,0x00);	//moto off stop
                break;
            }
            break;
        case 6:								//���ӵ缫ѡ��
            if((Menu_lever3_select==2)&&(Menu_lever_select==4))		//�˿�У׼
            {
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:		//+160mV
                    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_ISE_CTRL_ADDS,0x00AA,0x00);
                    break;
                case 2:		//0mV
                    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_ISE_CTRL_ADDS,0x005A,0x00);
                    break;
                case 3:		//160mV
                    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_ISE_CTRL_ADDS,0x00A5,0x00);
                    break;
                }
            }
            break;
        case 7:			//�¶Ȳ���  NTC0
            if((Menu_lever3_select==2)&&(Menu_lever_select==4))
            {
                k=0;
                i=M_lever4_BCWD_temp>>4;
                i&=0x0f;
                j=i*10;
                k+=j;
                i=M_lever4_BCWD_temp;
                i&=0x0f;
                j=i;
                k+=j;

                if(BCWD_NG_flag==1)
                    k+=1024;
                else if(BCWD_NG_flag==2)
                    k=1024-k;

#ifdef  __OLD_NTC__
                Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_NTC_CTRL_ADDS,k,0x00);
#else
                if(M_menur1_LZDJ_select_temp==1)
                    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_NTC_CTRL_WATER_ADDS,k,0x00);		//�缫��  2013-6-7 10:58
                else
                    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_NTC_CTRL_ADDS,k,0x00); 		//��ɫ��
#endif
            }
            break;
        case 8:
            if((Menu_lever3_select==1)&&(Menu_lever_select==4))				//LED����
            {
                j=k=0;
                i=M_menur1_led_dianliu_temp>>16;
                i&=0x0f;
                j=i*10000;
                k+=j;
                i=M_menur1_led_dianliu_temp>>12;
                i&=0x0f;
                j=i*1000;
                k+=j;
                i=M_menur1_led_dianliu_temp>>8;
                i&=0x0f;
                j=i*100;
                k+=j;
                i=M_menur1_led_dianliu_temp>>4;
                i&=0x0f;
                j=i*10;
                k+=j;
                i=M_menur1_led_dianliu_temp;
                i&=0x0f;
                k+=i;

                a=k;
                j=(a/50000.0)*2047;

                Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_LED_CURRIT_CTRL_ADDS,j,0x00);
            }
            else if((Menu_lever3_select==3)&&(Menu_lever_select==4))		//ƫ�õ�ѹ
            {
                j=k=0;
                i=M_lever4_PZDY_temp>>8;
                i&=0x0f;
                j=i*100;
                k+=j;
                i=M_lever4_PZDY_temp>>4;
                i&=0x0f;
                j=i*10;
                k+=j;
                i=M_lever4_PZDY_temp;
                i&=0x0f;
                k+=i;

                a=k;
                j=(a/500.0)*2047;
                Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_OFFSET_PWM_ADDS,j,0x00);
            }
            break;
        case 9:
            if((Auto_check_clock_flag==0)&&(Auto_check_clock_flag<0x10)&&(Hand_finction_select==0))
            {
                if(Menu_lever3_select==1)  Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_LED_LINGHT_CTRL_ADDS,0x0001,0x00);
                else if(Menu_lever3_select==2)  Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_LED_LINGHT_CTRL_ADDS,0x0000,0x00);
            }
            break;
        }
        break;
    case 0x05:			//��ʷ��¼
        switch(Menu_index_finc_select)
        {
        case 1:			//����
            if(Men_del_flag==1)
            {
                Del_mem_data(1);
            }
            Read_result_from_flash(1);
            Men_del_flag=0;
            break;
        case 2:			//У��
            if(Men_del_flag==1)
            {
                Del_mem_data(2);
            }
            Read_result_from_flash(2);
            Men_del_flag=0;
            break;
        case 3:			//����
            if(Men_del_flag==1)
            {
                Del_mem_data(4);
            }
            Read_result_from_flash(4);
            Men_del_flag=0;
            break;
        case 4:			//����
            if(Men_del_flag==1)
            {
                Del_mem_data(3);
            }
            Read_result_from_flash(3);
            Men_del_flag=0;
            break;
        }
        break;
    case 0x06: 			//�������
        if(Prog_disp_flag!=0)
            break;

/////    	    if(return_disp_flag>=2)
/////               return_disp_flag=0;
        switch(Menu_index_finc_select)
        {
        case 1:			//485 ����
            if(Menu_lever3_select<=4)
////		        Uart2_reinitial();
                __nop();		//test potocal
            else
                __nop();		//test potocal
            break;
        case 2:			//4-20mA
            if((Menu_lever_select==4)&&(Menu_index_select==6)&&(Menu_index_finc_select==2))
            {
//                    	if(Menu_lever3_point==1)

//                     	else

            }
            break;
        case 3:			//ģ������

            break;
        case 4:			//ģ�����
//            	     if(Menu_lever_select==4)
//            	     {
//            	     	if(Menu_lever4_Pub_select_cnt==1)
//            	           Set_switch_open_close(Menu_lever3_select+9,1);
//            	        else
//            	           Set_switch_open_close(Menu_lever3_select+9,0);
//            	     }
            break;
        case 5:			//��������

            break;
        case 6:			//�������

            break;
        }
        break;
    case 7:
        switch(Menu_index_finc_select)
        {
        case 1:			//����ʱ��
        case 2:
            RTC_write_date();
            //Test_potocal_time_flag=0x04;        //2014-2-24 17:40
            break;
//            	case 3:			// ����ѡ���л�
//            	break;
//            	case 4:			//
//            	case 5:			//
//            	case 6:			//
//            	case 7:			//
//            	case 8:			//
//            	case 9:			//
//            	case 10:		//
//            	break;
        }
        break;
    }
//-----------------------return disp-------------------------------------
exit_34_enter_finction:
    if(Menu_lever_select>=3)
    {
        if((Pwm_menu_lever>=3)&&(Menu_lever_select==4))
        {
            --Pwm_menu_lever;
            Disp_pwm_menu();
        }
        else if((Menu_lever_select==3)&&(Menu_index_select==2))
        {
            if(Menu_index_finc_select!=12)
                Enter_return_disp();
        }
        else if((Menu_lever_select==3)&&(Menu_index_select==4)&&(Menu_index_finc_select==9))
        {
            Enter_return_disp();
        }
        else if((Menu_lever_select==3)&&(Menu_index_select==7))
        {
            Enter_return_disp();
        }
        else if(Menu_lever_select==4)
        {
            if((Menu_index_finc_select==12)&&(Menu_index_select==2))
                Enter_return_disp();
            else
                Enter_return_disp();
        }
    }
}

//===============================================================================
//	finction	:Lever4_Select_en_flag
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-17 13:35
//	modefy		:null
//===============================================================================
uint8_t Lever4_Select_en_flag(void)
{
    uint8_t flag,a,b,c,d;

    a=Menu_index_select;
    b=Menu_index_finc_select;
    c=Menu_lever3_select;
    d=Menu_lever4_Pub_select_cnt;

    flag=1;

    if((a==1)&&(b==1)&&(c==1)&&(d==5))
        flag=0;

//    if((a==1)&&(b==1)&&(c==4))
//       flag=0;

    if((a==1)&&(b==1)&&(c==6))
        flag=0;

    if((a==1)&&(b==2)&&(c==2))
        flag=0;

    if((a==1)&&(b==3)&&(c==3))
        flag=0;

    if((a==1)&&(b==4)&&(c==2))
        flag=0;

    if((a==2)&&(b!=12))
        flag=0;

//    if((a==3)||(a==5))
//       flag=0;

    if((a==4)&&(b==1))
        flag=0;

    if((a==4)&&(b==3))
        flag=0;
    if((a==4)&&(b==4))
        flag=0;

    if((a==4)&&(b==2)&&(c==1))
        flag=0;

    if((a==4)&&(b==5)&&(c!=1))
        flag=0;

    if((a==4)&&(b==6)&&(c==1))
        flag=0;
    if((a==4)&&(b==7)&&(c==1))
        flag=0;

    if((a==4)&&(b==8)&&(c==2))
        flag=0;
    if((a==4)&&(b==8)&&(c==4))
        flag=0;
    if((a==4)&&(b==8)&&(c==5))
        flag=0;

    if((a==6)&&(b==1)&&(c!=1))
        flag=0;

    if(a==6)
    {
        if((b==3)||(b==4)||(b==5)||(b==6))
            flag=0;
    }

//    if((a==6)&&(b==2))
//    {
//    	if((c!=6)&&(c!=12))
//    	flag=0;
//    }

    return(flag);
}

//===============================================================================
//	finction	:Lever4_Select_finc_adds
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-11 16:08
//	modefy		:null
//===============================================================================
void Lever4_Select_finc_adds(void)			//����ѡ��Ҫ���õ�ѡ���ʾ
{
    uint8_t i;

//////     Menu_lever4_PoPa=3;

//////     if((Menu_index_select==6)&&(Menu_index_finc_select==2))
//////     __nop();
//////     else
//////     Clear_apage(Menu_lever4_PoPa);

//////     L4_seg_temp=Menu_lever4_PoSeg;
    switch(Menu_index_select)
    {
    case 1:
        switch(Menu_index_finc_select)
        {
        case 1:
            switch(Menu_lever3_select)
            {
            case 1:
/////     	    	    	     M_lever4_extern_trrig_flag=0;
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=M_lever4_FXPL_temp>>12;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    if(M_menur1_language_select_temp==2)
                    {
                        Seg_adds_bak=rev_clear_set_arear16(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,3,19,10);
                        Menu_lever4_PoPa=3;
                    }
                    else
                        Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_lever4_FXPL_temp>>8;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    if(M_menur1_language_select_temp==2)
                        Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,29,10);
                    else
                        Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,Menu_lever4_PoSeg+6,6);
                    break;
                case 3:
                    i=M_lever4_FXPL_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    if(M_menur1_language_select_temp==2)
                    {
                        Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,39,10);
                        Menu_lever4_PoPa=3;
                    }
                    else
                        Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,Menu_lever4_PoSeg+12,6);
                    break;
                case 4:
                    i=M_lever4_FXPL_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    L4_seg_temp=Menu_lever4_PoSeg+24;
                    if(M_menur1_language_select_temp==2)
                    {
                        Seg_adds_bak=rev_clear_set_arear16(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,3,49,10);
                        Menu_lever4_PoPa=3;
                    }
                    else
                        Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,Menu_lever4_PoSeg+18,6);
                    break;
                case 5:
                    if(M_menur1_language_select_temp==2)
                    {
                        Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,5,39,49);
                        Menu_lever4_PoPa=5;
                    }
                    else
                        Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,Menu_lever4_PoSeg+48,48);
/////     	    	    	              M_lever4_extern_trrig_flag=1;
                    break;
                }
                break;
            case 2:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=M_lever4_JZPL_temp>>8;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_lever4_JZPL_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+6,6);
                    break;
                case 3:
                    i=M_lever4_JZPL_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+12,6);
                    break;
                }
                break;
            case 3:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=M_lever4_QXPL_temp>>8;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_lever4_QXPL_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+6,6);
                    break;
                case 3:
                    i=M_lever4_QXPL_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+12,6);
                    break;
                }
                break;
            case 4:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=M_lever4_HCPL_temp>>8;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_lever4_HCPL_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+6,6);
                    break;
                case 3:
                    i=M_lever4_HCPL_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+12,6);
                    break;
                }
                break;
            case 5:
//     	    	    	     Menu_lever3_start_flag=BIT0;
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=M_lever4_start_year_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_lever4_start_year_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+6,6);
                    break;
                case 3:
                    i=M_lever4_start_munth_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+18,6);
                    break;
                case 4:
                    i=M_lever4_start_munth_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+24,6);
                    break;
                case 5:
                    i=M_lever4_start_day_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+36,6);
                    break;
                case 6:
                    i=M_lever4_start_day_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+42,6);
                    break;
                case 7:
                    i=M_lever4_start_hour_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+60,6);
                    break;
                case 8:
                    i=M_lever4_start_hour_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+66,6);
                    break;
                case 9:
                    i=M_lever4_start_mini_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+78,6);
                    break;
                case 10:
                    i=M_lever4_start_mini_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+84,6);
                    break;
                }

                break;
            }
            break;
        case 2:
            switch(Menu_lever3_select)
            {
            case 1:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=M_lever4_FXCS_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_lever4_FXCS_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+6,6);
                    break;
                }
                break;
            }
            break;
        case 3:
            switch(Menu_lever3_select)
            {
            case 1:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=M_menur1_jz1nd_temp>>16;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_menur1_jz1nd_temp>>12;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+6,6);
                    break;
                case 3:
                    i=M_menur1_jz1nd_temp>>8;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+12,6);
                    break;
                case 4:
                    i=M_menur1_jz1nd_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+24,6);
                    break;
                case 5:
                    i=M_menur1_jz1nd_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+30,6);
                    break;
                }
                break;
            case 2:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=M_menur1_jz2nd_temp>>16;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_menur1_jz2nd_temp>>12;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+6,6);
                    break;
                case 3:
                    i=M_menur1_jz2nd_temp>>8;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+12,6);
                    break;
                case 4:
                    i=M_menur1_jz2nd_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+18,6);
                    break;
                case 5:
                    i=M_menur1_jz2nd_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+30,6);
                    break;
                }
                break;
            }
            break;
        case 4:
            switch(Menu_lever3_select)
            {
            case 1:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=M_lever4_BYND_temp>>16;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_lever4_BYND_temp>>12;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+6,6);
                    break;
                case 3:
                    i=M_lever4_BYND_temp>>8;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+12,6);
                    break;
                case 4:
                    i=M_lever4_BYND_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+24,6);
                    break;
                case 5:
                    i=M_lever4_BYND_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+30,6);
                    break;
                }
                break;
            }
            break;
        }
        break;
    case 2:
        switch(Menu_index_finc_select)
        {
        case 12:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    Menu_lever4_Pub_adj_cnt=a_signle_flag;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_menur1_Fa_temp>>24;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+6,6);
                    break;
                case 3:
                    i=M_menur1_Fa_temp>>20;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+12,6);
                    break;
                case 4:
                    i=M_menur1_Fa_temp>>16;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+18,6);
                    break;
                case 5:
                    i=M_menur1_Fa_temp>>12;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+30,6);
                    break;
                case 6:
                    i=M_menur1_Fa_temp>>8;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+36,6);
                    break;
                case 7:
                    i=M_menur1_Fa_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+42,6);
                    break;
                case 8:
                    i=M_menur1_Fa_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+48,6);
                    break;
                }
                break;
            case 2:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    Menu_lever4_Pub_adj_cnt=b_signle_flag;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_menur1_Fb_temp>>24;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+6,6);
                    break;
                case 3:
                    i=M_menur1_Fb_temp>>20;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+12,6);
                    break;
                case 4:
                    i=M_menur1_Fb_temp>>16;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+18,6);
                    break;
                case 5:
                    i=M_menur1_Fb_temp>>12;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+30,6);
                    break;
                case 6:
                    i=M_menur1_Fb_temp>>8;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+36,6);
                    break;
                case 7:
                    i=M_menur1_Fb_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+42,6);
                    break;
                case 8:
                    i=M_menur1_Fb_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+48,6);
                    break;
                }
                break;
            case 3:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    Menu_lever4_Pub_adj_cnt=c_signle_flag;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_menur1_Fc_temp>>24;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+6,6);
                    break;
                case 3:
                    i=M_menur1_Fc_temp>>20;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+12,6);
                    break;
                case 4:
                    i=M_menur1_Fc_temp>>16;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+18,6);
                    break;
                case 5:
                    i=M_menur1_Fc_temp>>12;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+30,6);
                    break;
                case 6:
                    i=M_menur1_Fc_temp>>8;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+36,6);
                    break;
                case 7:
                    i=M_menur1_Fc_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+42,6);
                    break;
                case 8:
                    i=M_menur1_Fc_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+48,6);
                    break;
                }
                break;
            case 4:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    Menu_lever4_Pub_adj_cnt=d_signle_flag;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_menur1_Fd_temp>>24;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+6,6);
                    break;
                case 3:
                    i=M_menur1_Fd_temp>>20;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+12,6);
                    break;
                case 4:
                    i=M_menur1_Fd_temp>>16;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+18,6);
                    break;
                case 5:
                    i=M_menur1_Fd_temp>>12;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+30,6);
                    break;
                case 6:
                    i=M_menur1_Fd_temp>>8;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+36,6);
                    break;
                case 7:
                    i=M_menur1_Fd_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+42,6);
                    break;
                case 8:
                    i=M_menur1_Fd_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+48,6);
                    break;
                }
                break;
            }
            break;
        }
        break;
    case 4:
        switch(Menu_index_finc_select)
        {
        case 2:
            switch(Menu_lever3_select)
            {
            case 1:
                break;
            case 2:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=M_menur1_ZSB_speed_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_menur1_ZSB_speed_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+6,6);
                    break;
                }
                break;
            case 3:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=M_lever4_move_to_temp>>8;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_lever4_move_to_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+12,6);
                    break;
                case 3:
                    i=M_lever4_move_to_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+18,6);
                    break;
                }
                break;
            }
            break;
        case 3:
            DCF_switch_select_flag=1;
            switch(Menu_lever4_Pub_select_cnt)
            {
            case 1:
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,29,12);
                else
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,18,Menu_lever4_PoPa,41,12);
                break;
            case 2:
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,53,12);
                else
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,12,Menu_lever4_PoPa,41+18,18);
                break;
            }
            break;
/////     	    	case 4:
/////     	    	    switch(Menu_lever3_select)
/////     	    	    {
/////     	    	    	case 1:
/////     	    	    	break;
/////     	    	    	case 2:
/////     	    	    	break;
/////     	    	    }
/////     	    	break;
        case 5:
            switch(Menu_lever3_select)
            {
            case 1:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=M_menur1_JB_speed_temp>>8;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_menur1_JB_speed_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+6,6);
                    break;
                case 3:
                    i=M_menur1_JB_speed_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+12,6);
                    break;
                }
                break;
            case 2:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
/////     	    	    	              M_menur1_direction_select_temp=1;
                    if(M_menur1_language_select_temp==2)
                        Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,11,37);
                    else
                        Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,29,6);
                    break;
                case 2:
/////     	    	    	              M_menur1_direction_select_temp=2;
                    if(M_menur1_language_select_temp==2)
                        Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,59,37);
                    else
                        Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,41,6);
                    break;
                }
                break;
            }
            break;
        case 6:
            switch(Menu_lever3_select)
            {
            case 2:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,Menu_lever4_PoSeg,36);
                    break;
                case 2:
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,Menu_lever4_PoSeg+42,6);
                    break;
                case 3:
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,Menu_lever4_PoSeg+54,36);
                    break;
                }
                break;
            }
            break;
        case 7:
            switch(Menu_lever3_select)
            {
//     	    	    	case 1:
//     	    	    	     switch(Menu_lever4_Pub_select_cnt)
//     	    	    	     {
//     	    	    	         case 1:
//     	    	    	              i=M_lever4_WD_temp>>8;
//     	    	    	              i&=0x0f;
//     	    	    	              Menu_lever4_Pub_adj_cnt=i;
//     	    	    	         break;
//     	    	    	         case 2:
//     	    	    	              i=M_lever4_WD_temp>>4;
//     	    	    	              i&=0x0f;
//     	    	    	              Menu_lever4_Pub_adj_cnt=i;
//     	    	    	              L4_seg_temp=Menu_lever4_PoSeg+8;
//     	    	    	         break;
//     	    	    	         case 3:
//     	    	    	              i=M_lever4_WD_temp;
//     	    	    	              i&=0x0f;
//     	    	    	              Menu_lever4_Pub_adj_cnt=i;
//     	    	    	              L4_seg_temp=Menu_lever4_PoSeg+24;
//     	    	    	         break;
//     	    	    	     }
//     	    	    	break;
            case 2:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_lever4_BCWD_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,Menu_lever4_PoSeg+6,6);
                    break;
                case 3:
                    i=M_lever4_BCWD_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,Menu_lever4_PoSeg+18,6);
                    break;
                }
                break;
            }
            break;
        case 8:
            switch(Menu_lever3_select)
            {
            case 1:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=M_menur1_led_dianliu_temp>>16;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_menur1_led_dianliu_temp>>12;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+6,6);
                    break;
                case 3:
                    i=M_menur1_led_dianliu_temp>>8;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+18,6);
                    break;
                case 4:
                    i=M_menur1_led_dianliu_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+24,6);
                    break;
                case 5:
                    i=M_menur1_led_dianliu_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+30,6);
                    break;
                }
                break;
            case 3:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=M_lever4_PZDY_temp>>8;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    i=M_lever4_PZDY_temp>>4;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+12,6);
                    break;
                case 3:
                    i=M_lever4_PZDY_temp;
                    i&=0x0f;
                    Menu_lever4_Pub_adj_cnt=i;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+18,6);
                    break;
                }
                break;
            }
            break;
        }
        break;
/////     	case 5:
/////     	break;
    case 6:
        switch(Menu_index_finc_select)
        {
        case 1:
            switch(Menu_lever3_select)
            {
            case 1:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    L4_seg_temp=Menu_lever4_PoSeg;
                    Menu_lever4_Pub_adj_cnt=Machine_note_temp>>8;
                    Menu_lever4_Pub_adj_cnt&=0x0f;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+6,6);
                    Menu_lever4_Pub_adj_cnt=Machine_note_temp>>4;
                    Menu_lever4_Pub_adj_cnt&=0x0f;
                    break;
                case 3:
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,6,Menu_lever4_PoPa,Menu_lever4_PoSeg+12,6);
                    Menu_lever4_Pub_adj_cnt=Machine_note_temp;
                    Menu_lever4_Pub_adj_cnt&=0x0f;
                    break;
                }
                Get_uart_note();
                break;
            case 2:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,Menu_lever4_PoSeg,25);
                    break;
                case 2:
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,Menu_lever4_PoSeg+30,31);
                    break;
                case 3:
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,Menu_lever4_PoSeg+66,31);
                    break;
                }
                break;
            case 3:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,Menu_lever4_PoSeg,6);
                    break;
                case 2:
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,Menu_lever4_PoSeg+12,6);
////     	    	    	              Uart2_JOJY_select_temp=2;
                    break;
                case 3:
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,Menu_lever4_PoSeg+24,6);
////     	    	    	              Uart2_JOJY_select_temp=3;
                    break;
                }
                break;
            case 4:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
////     	    	    	              Uart2_STOP_select_temp=1;
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,29,6);
                    break;
                case 2:
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever4_PoPa,41,6);
////     	    	    	              Uart2_STOP_select_temp=2;
                    break;
                }
                break;
/////     	    	    	case 5:
/////     	    	    	break;
            }
            break;
/////     	    	case 2:
/////     	    	break;
/////     	    	case 3:
/////     	    	    switch(Menu_lever3_select)
/////     	    	    {
/////     	    	    	case 1:
/////     	    	    	break;
/////     	    	    	case 2:
/////     	    	    	break;
/////     	    	    }
/////     	    	break;
        case 4:
            switch(Menu_lever4_Pub_select_cnt)
            {
            case 1:
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,29,13);
                else
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,18,Menu_lever4_PoPa,Menu_lever4_PoSeg,12);
                break;
            case 2:
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,53,13);
                else
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever4_PoPa,Seg_adds_bak,12,Menu_lever4_PoPa,Menu_lever4_PoSeg+18,18);
                break;
            }
            break;
/////     	    	case 5:
/////     	    	break;
/////     	    	case 6:
/////     	    	break;
        }
        break;
    }
/////     if((Menu_index_select==4)&&(Menu_index_finc_select==6)&&(Menu_lever3_select==1))
/////        __nop();
/////     else if((Menu_index_select==4)&&(Menu_index_finc_select==7)&&(Menu_lever3_select==1))
/////        __nop();
/////     else if((Menu_index_select==4)&&(Menu_index_finc_select==8)&&(Menu_lever3_select==2))
/////        __nop();
/////     else if((Menu_index_select==4)&&(Menu_index_finc_select==8)&&(Menu_lever3_select==4))
/////        __nop();
/////     else if((Menu_index_select==4)&&(Menu_index_finc_select==8)&&(Menu_lever3_select==5))
/////        __nop();
/////     else if((Menu_index_select==6)&&(Menu_index_finc_select==2))
/////        __nop();
/////     else
/////     {
/////         Display_all_buff();
/////     }
    Display_all_buff();
}

//===============================================================================
//	finction	:Key_Menu_lever4_down_finc
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-11 16:08
//	modefy		:null
//===============================================================================
void Key_Menu_lever4_down_finc(void)
{
    uint32_t i;

    switch(Menu_index_select)
    {
    case 1:
        ++Menu_lever4_Pub_adj_cnt;
        if(Menu_lever4_Pub_adj_cnt==10)
            Menu_lever4_Pub_adj_cnt=0;

        switch(Menu_index_finc_select)
        {
        case 1:
            switch(Menu_lever3_select)
            {
            case 1:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_lever4_FXPL_temp&=0x00000fff;
                    M_lever4_FXPL_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_FXPL_temp&=0x0000f0ff;
                    M_lever4_FXPL_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_FXPL_temp&=0x0000ff0f;
                    M_lever4_FXPL_temp+=i;
                    break;
                case 4:
                    M_lever4_FXPL_temp&=0x0000fff0;
                    M_lever4_FXPL_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                }
//		             Lever4_adj_valu();
                break;
            case 2:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_JZPL_temp&=0x000000ff;
                    M_lever4_JZPL_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_JZPL_temp&=0x00000f0f;
                    M_lever4_JZPL_temp+=i;
                    break;
                case 3:
                    M_lever4_JZPL_temp&=0x00000ff0;
                    M_lever4_JZPL_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                }
                break;
            case 3:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_QXPL_temp&=0x000000ff;
                    M_lever4_QXPL_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_QXPL_temp&=0x00000f0f;
                    M_lever4_QXPL_temp+=i;
                    break;
                case 3:
                    M_lever4_QXPL_temp&=0x00000ff0;
                    M_lever4_QXPL_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                }
                break;
            case 4:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_HCPL_temp&=0x000000ff;
                    M_lever4_HCPL_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_HCPL_temp&=0x00000f0f;
                    M_lever4_HCPL_temp+=i;
                    break;
                case 3:
                    M_lever4_HCPL_temp&=0x00000ff0;
                    M_lever4_HCPL_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                }
                break;
            case 5:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt>9)
                        Menu_lever4_Pub_adj_cnt=0;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_start_year_temp&=0x0f;
                    M_lever4_start_year_temp+=i;
                    break;
                case 2:
                    if(Menu_lever4_Pub_adj_cnt>9)
                        Menu_lever4_Pub_adj_cnt=0;

                    M_lever4_start_year_temp&=0xf0;
                    M_lever4_start_year_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                case 3:
                    if(Menu_lever4_Pub_adj_cnt>2)
                        Menu_lever4_Pub_adj_cnt=0;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_start_munth_temp&=0x0f;
                    M_lever4_start_munth_temp+=i;

                    if(Menu_lever4_Pub_adj_cnt!=0)
                    {
                        i=M_lever4_start_munth_temp;
                        i&=0x0f;
                        if(i>2)
                        {
                            M_lever4_start_munth_temp&=0xf0;
                            Get_6x8_char_single(3,36,'0');
                        }
                    }
                    break;
                case 4:
                    if(((M_lever4_start_munth_temp>>4)==0)&&(Menu_lever4_Pub_adj_cnt==0))
                        Menu_lever4_Pub_adj_cnt=1;
                    else if((M_lever4_start_munth_temp>>4)==1)
                    {
                        if(Menu_lever4_Pub_adj_cnt>2)
                            Menu_lever4_Pub_adj_cnt=0;
                    }

                    M_lever4_start_munth_temp&=0xf0;
                    M_lever4_start_munth_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                case 5:
                    if(Menu_lever4_Pub_adj_cnt>3)
                        Menu_lever4_Pub_adj_cnt=0;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_start_day_temp&=0x0f;
                    M_lever4_start_day_temp+=i;
                    break;
                case 6:
                    if(((M_lever4_start_day_temp>>4)==0)&&(Menu_lever4_Pub_adj_cnt==0))
                        Menu_lever4_Pub_adj_cnt=1;
                    else if((M_lever4_start_day_temp>>4)==3)
                    {
                        if(Menu_lever4_Pub_adj_cnt>1)
                            Menu_lever4_Pub_adj_cnt=0;
                    } 					//���꼰�����¾�����д

                    M_lever4_start_day_temp&=0xf0;
                    M_lever4_start_day_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                case 7:
                    if(Menu_lever4_Pub_adj_cnt>2)
                        Menu_lever4_Pub_adj_cnt=0;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_start_hour_temp&=0x0f;
                    M_lever4_start_hour_temp+=i;
                    break;
                case 8:
                    if(((M_lever4_start_hour_temp>>4)==2)&&(Menu_lever4_Pub_adj_cnt>=4))
                        Menu_lever4_Pub_adj_cnt=0;

                    M_lever4_start_hour_temp&=0xf0;
                    M_lever4_start_hour_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                case 9:
                    if(Menu_lever4_Pub_adj_cnt>5)
                        Menu_lever4_Pub_adj_cnt=0;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_start_mini_temp&=0x0f;
                    M_lever4_start_mini_temp+=i;
                    break;
                case 10:
                    M_lever4_start_mini_temp&=0xf0;
                    M_lever4_start_mini_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                }
                break;
            }
            break;
        case 2:
            switch(Menu_lever3_select)
            {
            case 1:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_FXCS_temp&=0x0f;
                    M_lever4_FXCS_temp+=i;
                    break;
                case 2:
                    if(((M_lever4_FXCS_temp>>4)==0)&&(Menu_lever4_Pub_adj_cnt==0))
                        Menu_lever4_Pub_adj_cnt=9;

                    M_lever4_FXCS_temp&=0xf0;
                    M_lever4_FXCS_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                }
                break;
            }
            break;
        case 3:
            switch(Menu_lever3_select)
            {
            case 1:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_menur1_jz1nd_temp&=0x0000ffff;
                    M_menur1_jz1nd_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_menur1_jz1nd_temp&=0x000f0fff;
                    M_menur1_jz1nd_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_menur1_jz1nd_temp&=0x000ff0ff;
                    M_menur1_jz1nd_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_jz1nd_temp&=0x000fff0f;
                    M_menur1_jz1nd_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_jz1nd_temp&=0x000ffff0;
                    M_menur1_jz1nd_temp+=i;
                    break;
                }
                break;
            case 2:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_menur1_jz2nd_temp&=0x0000ffff;
                    M_menur1_jz2nd_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_menur1_jz2nd_temp&=0x000f0fff;
                    M_menur1_jz2nd_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_menur1_jz2nd_temp&=0x000ff0ff;
                    M_menur1_jz2nd_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_jz2nd_temp&=0x000fff0f;
                    M_menur1_jz2nd_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_jz2nd_temp&=0x000ffff0;
                    M_menur1_jz2nd_temp+=i;
                    break;
                }
                break;
            }
            break;
        case 4:
            switch(Menu_lever3_select)
            {
            case 1:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_lever4_BYND_temp&=0x0000ffff;
                    M_lever4_BYND_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_lever4_BYND_temp&=0x000f0fff;
                    M_lever4_BYND_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_BYND_temp&=0x000ff0ff;
                    M_lever4_BYND_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_BYND_temp&=0x000fff0f;
                    M_lever4_BYND_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_lever4_BYND_temp&=0x000ffff0;
                    M_lever4_BYND_temp+=i;
                    break;
                }
                break;
            }
            break;
        }
        break;
    case 2:
        ++Menu_lever4_Pub_adj_cnt;
        if(Menu_lever4_Pub_adj_cnt==10)
            Menu_lever4_Pub_adj_cnt=0;

        switch(Menu_index_finc_select)
        {
        case 12:
//     	    	    switch(Menu_lever3_select)
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:		//a
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt==3)
                        Menu_lever4_Pub_adj_cnt=1;
                    a_signle_flag=Menu_lever4_Pub_adj_cnt;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=24;
                    M_menur1_Fa_temp&=0x00ffffff;
                    M_menur1_Fa_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=20;
                    M_menur1_Fa_temp&=0x0f0fffff;
                    M_menur1_Fa_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_menur1_Fa_temp&=0x0ff0ffff;
                    M_menur1_Fa_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_menur1_Fa_temp&=0x0fff0fff;
                    M_menur1_Fa_temp+=i;
                    break;
                case 6:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_menur1_Fa_temp&=0x0ffff0ff;
                    M_menur1_Fa_temp+=i;
                    break;
                case 7:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_Fa_temp&=0x0fffff0f;
                    M_menur1_Fa_temp+=i;
                    break;
                case 8:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_Fa_temp&=0x0ffffff0;
                    M_menur1_Fa_temp+=i;
                    break;
                }
                break;
            case 2:		//b
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt==3)
                        Menu_lever4_Pub_adj_cnt=1;
                    b_signle_flag=Menu_lever4_Pub_adj_cnt;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=24;
                    M_menur1_Fb_temp&=0x00ffffff;
                    M_menur1_Fb_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=20;
                    M_menur1_Fb_temp&=0x0f0fffff;
                    M_menur1_Fb_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_menur1_Fb_temp&=0x0ff0ffff;
                    M_menur1_Fb_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_menur1_Fb_temp&=0x0fff0fff;
                    M_menur1_Fb_temp+=i;
                    break;
                case 6:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_menur1_Fb_temp&=0x0ffff0ff;
                    M_menur1_Fb_temp+=i;
                    break;
                case 7:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_Fb_temp&=0x0fffff0f;
                    M_menur1_Fb_temp+=i;
                    break;
                case 8:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_Fb_temp&=0x0ffffff0;
                    M_menur1_Fb_temp+=i;
                    break;
                }
                break;
            case 3:		//c
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt==3)
                        Menu_lever4_Pub_adj_cnt=1;
                    c_signle_flag=Menu_lever4_Pub_adj_cnt;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=24;
                    M_menur1_Fc_temp&=0x00ffffff;
                    M_menur1_Fc_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=20;
                    M_menur1_Fc_temp&=0x0f0fffff;
                    M_menur1_Fc_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_menur1_Fc_temp&=0x0ff0ffff;
                    M_menur1_Fc_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_menur1_Fc_temp&=0x0fff0fff;
                    M_menur1_Fc_temp+=i;
                    break;
                case 6:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_menur1_Fc_temp&=0x0ffff0ff;
                    M_menur1_Fc_temp+=i;
                    break;
                case 7:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_Fc_temp&=0x0fffff0f;
                    M_menur1_Fc_temp+=i;
                    break;
                case 8:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_Fc_temp&=0x0ffffff0;
                    M_menur1_Fc_temp+=i;
                    break;
                }
                break;
            case 4:		//d
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt==3)
                        Menu_lever4_Pub_adj_cnt=1;
                    d_signle_flag=Menu_lever4_Pub_adj_cnt;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=24;
                    M_menur1_Fd_temp&=0x00ffffff;
                    M_menur1_Fd_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=20;
                    M_menur1_Fd_temp&=0x0f0fffff;
                    M_menur1_Fd_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_menur1_Fd_temp&=0x0ff0ffff;
                    M_menur1_Fd_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_menur1_Fd_temp&=0x0fff0fff;
                    M_menur1_Fd_temp+=i;
                    break;
                case 6:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_menur1_Fd_temp&=0x0ffff0ff;
                    M_menur1_Fd_temp+=i;
                    break;
                case 7:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_Fd_temp&=0x0fffff0f;
                    M_menur1_Fd_temp+=i;
                    break;
                case 8:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_Fd_temp&=0x0ffffff0;
                    M_menur1_Fd_temp+=i;
                    break;
                }
                break;
            }
            break;
        }
        break;
    case 4:
        ++Menu_lever4_Pub_adj_cnt;
        if(Menu_lever4_Pub_adj_cnt==10)
            Menu_lever4_Pub_adj_cnt=0;

        switch(Menu_index_finc_select)
        {
//     	    	case 1:
//     	    	    switch(Menu_lever3_select)
//     	    	    {
//     	    	    	case 1:
//     	    	    	break;
//     	    	    	case 2:
//     	    	    	break;
//     	    	    	case 3:
//     	    	    	break;
//     	    	    	case 4:
//     	    	    	break;
//     	    	    	case 5:
//     	    	    	break;
//     	    	    	case 6:
//     	    	    	break;
//     	    	    }
//     	    	break;
        case 2:
            switch(Menu_lever3_select)
            {
            case 2:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt>2)
                        Menu_lever4_Pub_adj_cnt=1;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_ZSB_speed_temp&=0x0f;
                    M_menur1_ZSB_speed_temp+=i;

                    if(M_menur1_ZSB_speed_temp>0x00000027)
                    {
                        M_menur1_ZSB_speed_temp=0x00000027;

                        i=M_menur1_ZSB_speed_temp;
                        i&=0x0f;
                        i=Get_ascii_data(i);

                        Get_6x8_char_single(3,36,i);
                        Display_6x8_char_single(3,36,i);
                    }
                    break;
                case 2:
////     	    	    	              if(((M_menur1_ZSB_speed_temp>>4)==0)&&(Menu_lever4_Pub_adj_cnt<5))
////     	    	    	                 Menu_lever4_Pub_adj_cnt=5;
                    if(((M_menur1_ZSB_speed_temp>>4)==2)&&(Menu_lever4_Pub_adj_cnt>7))
                        Menu_lever4_Pub_adj_cnt=0;

                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_ZSB_speed_temp&=0xf0;
                    M_menur1_ZSB_speed_temp+=i;
                    break;
                }
                break;
            case 3:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_move_to_temp&=0x0ff;
                    M_lever4_move_to_temp+=i;
                    break;
                case 2:
                    i=M_lever4_move_to_temp>>8;
                    i&=0x0f;
                    if((i==9)&&(Menu_lever4_Pub_adj_cnt>5))
                        Menu_lever4_Pub_adj_cnt=0;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_move_to_temp&=0xf0f;
                    M_lever4_move_to_temp+=i;
                    break;
                case 3:
                    i=M_lever4_move_to_temp>>4;
                    if((i==0x95)&&(Menu_lever4_Pub_adj_cnt>8))
                        Menu_lever4_Pub_adj_cnt=0;

                    i=Menu_lever4_Pub_adj_cnt;
                    M_lever4_move_to_temp&=0xff0;
                    M_lever4_move_to_temp+=i;
                    break;
                }
                break;
            }
            break;
        case 3:
            switch(Menu_lever3_select)
            {
            case 1:
                break;
            case 2:
                break;
            }
            break;
        case 4:
            switch(Menu_lever3_select)
            {
            case 1:
                break;
            case 2:
                break;
            }
            break;
        case 5:
#ifdef __OLD_JBSPEED__
            switch(Menu_lever3_select)
            {
            case 1:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt>2)
                        Menu_lever4_Pub_adj_cnt=0;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_menur1_JB_speed_temp&=0x0ff;
                    M_menur1_JB_speed_temp+=i;
                    break;
                case 2:
                    i=M_menur1_JB_speed_temp>>8;
                    i&=0x0f;
                    if((i==2)&&(Menu_lever4_Pub_adj_cnt>4))
                        Menu_lever4_Pub_adj_cnt=0;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_JB_speed_temp&=0xf0f;
                    M_menur1_JB_speed_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_JB_speed_temp&=0xff0;
                    M_menur1_JB_speed_temp+=i;
                    break;
                }
                break;
            }
#else
            switch(Menu_lever3_select)
            {
            case 1:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt>2)
                        Menu_lever4_Pub_adj_cnt=1;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_menur1_JB_speed_temp&=0x0ff;
                    M_menur1_JB_speed_temp+=i;

                    Check_JB_speed_edag();
                    break;
                case 2:
                    i=M_menur1_JB_speed_temp>>8;
                    i&=0x0f;
                    if((i==2)&&(Menu_lever4_Pub_adj_cnt>3))
                        Menu_lever4_Pub_adj_cnt=0;
                    if((i==1)&&(Menu_lever4_Pub_adj_cnt<7))
                        Menu_lever4_Pub_adj_cnt=7;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_JB_speed_temp&=0xf0f;
                    M_menur1_JB_speed_temp+=i;

                    Check_JB_speed_edag();
                    break;
                case 3:
                    i=M_menur1_JB_speed_temp>>4;
                    if((i==0x23)&&(Menu_lever4_Pub_adj_cnt>5))
                        Menu_lever4_Pub_adj_cnt=0;
                    else if(i==0x17)
                    {
                        if(Menu_lever3_Pub_adj_cnt<5)
                            Menu_lever3_Pub_adj_cnt=9;
                    }

                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_JB_speed_temp&=0xff0;
                    M_menur1_JB_speed_temp+=i;
                    break;
                }
                break;
            }
#endif
            break;
        case 6:
            switch(Menu_lever3_select)
            {
            case 1:
//     	    	    	     switch(Menu_lever4_Pub_select_cnt)
//     	    	    	     {
//     	    	    	     	case 1:
//     	    	    	              i=Menu_lever4_Pub_adj_cnt;
//     	    	    	              i<<=20;
//     	    	    	              M_menur3_ISE_temp&=0x000fffff;
//     	    	    	              M_menur3_ISE_temp+=i;
//     	    	    	     	break;
//     	    	    	     	case 2:
//     	    	    	              i=Menu_lever4_Pub_adj_cnt;
//     	    	    	              i<<=16;
//     	    	    	              M_menur3_ISE_temp&=0x00f0ffff;
//     	    	    	              M_menur3_ISE_temp+=i;
//     	    	    	     	break;
//     	    	    	     	case 3:
//     	    	    	              i=Menu_lever4_Pub_adj_cnt;
//     	    	    	              i<<=12;
//     	    	    	              M_menur3_ISE_temp&=0x00ff0fff;
//     	    	    	              M_menur3_ISE_temp+=i;
//     	    	    	     	break;
//     	    	    	     	case 4:
//     	    	    	              i=Menu_lever4_Pub_adj_cnt;
//     	    	    	              i<<=8;
//     	    	    	              M_menur3_ISE_temp&=0x00fff0ff;
//     	    	    	              M_menur3_ISE_temp+=i;
//     	    	    	     	break;
//     	    	    	     	case 5:
//     	    	    	              i=Menu_lever4_Pub_adj_cnt;
//     	    	    	              i<<=4;
//     	    	    	              M_menur3_ISE_temp&=0x00ffff0f;
//     	    	    	              M_menur3_ISE_temp+=i;
//     	    	    	     	break;
//     	    	    	     	case 6:
//     	    	    	              i=Menu_lever4_Pub_adj_cnt;
//     	    	    	              M_menur3_ISE_temp&=0x00fffff0;
//     	    	    	              M_menur3_ISE_temp+=i;
//     	    	    	     	break;
//     	    	    	     }
                break;
            }
            break;
        case 7:
            switch(Menu_lever3_select)
            {
            case 1:
//     	    	    	     switch(Menu_lever4_Pub_select_cnt)
//     	    	    	     {
//     	    	    	     	case 1:
//     	    	    	              i=Menu_lever4_Pub_adj_cnt;
//     	    	    	              i<<=8;
//     	    	    	              M_lever4_WD_temp&=0x0ff;
//     	    	    	              M_lever4_WD_temp+=i;
//     	    	    	     	break;
//     	    	    	     	case 2:
//     	    	    	              i=Menu_lever4_Pub_adj_cnt;
//     	    	    	              i<<=4;
//     	    	    	              M_lever4_WD_temp&=0xf0f;
//     	    	    	              M_lever4_WD_temp+=i;
//     	    	    	     	break;
//     	    	    	     	case 3:
//     	    	    	              i=Menu_lever4_Pub_adj_cnt;
//     	    	    	              M_lever4_WD_temp&=0xff0;
//     	    	    	              M_lever4_WD_temp+=i;
//     	    	    	     	break;
//     	    	    	     }
                break;
            case 2:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    BCWD_NG_flag=1;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_BCWD_temp&=0xf0f;
                    M_lever4_BCWD_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_lever4_BCWD_temp&=0xff0;
                    M_lever4_BCWD_temp+=i;
                    break;
                }
                break;
            }
            break;
        case 8:
            switch(Menu_lever3_select)
            {
            case 1:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt>5)
                        Menu_lever4_Pub_adj_cnt=0;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_menur1_led_dianliu_temp&=0x0000ffff;
                    M_menur1_led_dianliu_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_menur1_led_dianliu_temp&=0x000f0fff;
                    M_menur1_led_dianliu_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_menur1_led_dianliu_temp&=0x000ff0ff;
                    M_menur1_led_dianliu_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_led_dianliu_temp&=0x000fff0f;
                    M_menur1_led_dianliu_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_led_dianliu_temp&=0x000ffff0;
                    M_menur1_led_dianliu_temp+=i;
                    break;
                }
                break;
            case 2:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=20;
                    M_lever4_GDGDY_temp&=0x000fffff;
                    M_lever4_GDGDY_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_lever4_GDGDY_temp&=0x00f0ffff;
                    M_lever4_GDGDY_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_lever4_GDGDY_temp&=0x00ff0fff;
                    M_lever4_GDGDY_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_GDGDY_temp&=0x00fff0ff;
                    M_lever4_GDGDY_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_GDGDY_temp&=0x00ffff0f;
                    M_lever4_GDGDY_temp+=i;
                    break;
                case 6:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_lever4_GDGDY_temp&=0x00fffff0;
                    M_lever4_GDGDY_temp+=i;
                    break;
                }
                break;
            case 3:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt>5)
                        Menu_lever4_Pub_adj_cnt=5;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_PZDY_temp&=0x0ff;
                    M_lever4_PZDY_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_PZDY_temp&=0xf0f;
                    M_lever4_PZDY_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_lever4_PZDY_temp&=0xff0;
                    M_lever4_PZDY_temp+=i;
                    break;
                }
                break;
            case 4:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=20;
                    M_lever4_PZDYZ_temp&=0x000fffff;
                    M_lever4_PZDYZ_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_lever4_PZDYZ_temp&=0x00f0ffff;
                    M_lever4_PZDYZ_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_lever4_PZDYZ_temp&=0x00ff0fff;
                    M_lever4_PZDYZ_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_PZDYZ_temp&=0x00fff0ff;
                    M_lever4_PZDYZ_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_PZDYZ_temp&=0x00ffff0f;
                    M_lever4_PZDYZ_temp+=i;
                    break;
                case 6:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_lever4_PZDYZ_temp&=0x00fffff0;
                    M_lever4_PZDYZ_temp+=i;
                    break;
                }
                break;
            case 5:
                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=20;
                    M_lever4_CFZ_temp&=0x000fffff;
                    M_lever4_CFZ_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_lever4_CFZ_temp&=0x00f0ffff;
                    M_lever4_CFZ_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_lever4_CFZ_temp&=0x00ff0fff;
                    M_lever4_CFZ_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_CFZ_temp&=0x00fff0ff;
                    M_lever4_CFZ_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_CFZ_temp&=0x00ffff0f;
                    M_lever4_CFZ_temp+=i;
                    break;
                case 6:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_lever4_CFZ_temp&=0x00fffff0;
                    M_lever4_CFZ_temp+=i;
                    break;
                }
                break;
            }
            break;
        }
        break;
    case 6:
        ++Menu_lever4_Pub_adj_cnt;
        if(Menu_lever4_Pub_adj_cnt==10)
            Menu_lever4_Pub_adj_cnt=0;
        if((Menu_index_finc_select==1)&&(Menu_lever3_select==1))
        {
            switch(Menu_lever4_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever4_Pub_adj_cnt>=3)
                    Menu_lever4_Pub_adj_cnt=0;

                i=Menu_lever4_Pub_adj_cnt;
                i&=0x0000000f;
                Machine_note_temp&=0x00ff;
                Machine_note_temp|=(i<<8);
                break;
            case 2:
                i=Machine_note_temp;
                i&=0x00000f00;
                if(i==0x00000200)
                {
                    if(Menu_lever4_Pub_adj_cnt>=6)
                        Menu_lever4_Pub_adj_cnt=0;
                }
                i=Menu_lever4_Pub_adj_cnt;
                i&=0x0000000f;
                Machine_note_temp&=0x0f0f;
                Machine_note_temp|=(i<<4);
                break;
            case 3:
                i=Machine_note_temp;
                i&=0x00000ff0;
                if(i==0x00000250)
                {
                    if(Menu_lever4_Pub_adj_cnt>=6)
                        Menu_lever4_Pub_adj_cnt=0;
                }
                i=Menu_lever4_Pub_adj_cnt;
                i&=0x0000000f;
                Machine_note_temp&=0x0ff0;
                Machine_note_temp|=i;
                break;
            }
            Get_uart_note();
        }
        else
        {
            switch(Pwm_menu_lever)		//+
            {
            case 1:
                if(Pwm_menu_select>1)
                    --Pwm_menu_select;
                else
                    Pwm_menu_select=3;
                Disp_pwm_menu();
                break;
            case 2:
                if(Pwm_menu_select1>1)
                    --Pwm_menu_select1;
                else
                    Pwm_menu_select1=5;
                Disp_pwm_menu();
                break;
            case 3:
                ++Pwm_adj_cnt;
                if(Pwm_adj_cnt==10)
                    Pwm_adj_cnt=0;
                Pwm_adj_data();
                break;
            }
            goto exit_l4_d;
        }
        break;
    }
    Lever4_adj_valu();
exit_l4_d:
    __nop();
}

//===============================================================================
//	finction	:Key_Menu_lever4_up_finc
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-11 16:08
//	modefy		:null
//===============================================================================
void Key_Menu_lever4_up_finc(void)
{
    uint32_t i;

    switch(Menu_index_select)
    {
    case 1:
        --Menu_lever4_Pub_adj_cnt;

        switch(Menu_index_finc_select)
        {
        case 1:
            switch(Menu_lever3_select)
            {
            case 1:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_lever4_FXPL_temp&=0x00000fff;
                    M_lever4_FXPL_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_FXPL_temp&=0x0000f0ff;
                    M_lever4_FXPL_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_FXPL_temp&=0x0000ff0f;
                    M_lever4_FXPL_temp+=i;
                    break;
                case 4:
                    M_lever4_FXPL_temp&=0x0000fff0;
                    M_lever4_FXPL_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                }
//		             Lever4_adj_valu();
                break;
            case 2:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_JZPL_temp&=0x000000ff;
                    M_lever4_JZPL_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_JZPL_temp&=0x00000f0f;
                    M_lever4_JZPL_temp+=i;
                    break;
                case 3:
                    M_lever4_JZPL_temp&=0x00000ff0;
                    M_lever4_JZPL_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                }
                break;
            case 3:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_QXPL_temp&=0x000000ff;
                    M_lever4_QXPL_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_QXPL_temp&=0x00000f0f;
                    M_lever4_QXPL_temp+=i;
                    break;
                case 3:
                    M_lever4_QXPL_temp&=0x00000ff0;
                    M_lever4_QXPL_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                }
                break;
            case 4:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_HCPL_temp&=0x000000ff;
                    M_lever4_HCPL_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_HCPL_temp&=0x00000f0f;
                    M_lever4_HCPL_temp+=i;
                    break;
                case 3:
                    M_lever4_HCPL_temp&=0x00000ff0;
                    M_lever4_HCPL_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                }
                break;
            case 5:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt>9)
                        Menu_lever4_Pub_adj_cnt=9;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_start_year_temp&=0x0f;
                    M_lever4_start_year_temp+=i;
                    break;
                case 2:
                    if(Menu_lever4_Pub_adj_cnt>9)
                        Menu_lever4_Pub_adj_cnt=9;

                    M_lever4_start_year_temp&=0xf0;
                    M_lever4_start_year_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                case 3:
                    if(Menu_lever4_Pub_adj_cnt>2)
                        Menu_lever4_Pub_adj_cnt=1;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_start_munth_temp&=0x0f;
                    M_lever4_start_munth_temp+=i;

                    if(Menu_lever4_Pub_adj_cnt!=0)
                    {
                        i=M_lever4_start_munth_temp;
                        i&=0x0f;
                        if(i>2)
                        {
                            M_lever4_start_munth_temp&=0xf0;
                            Get_6x8_char_single(3,36,'0');
                        }
                    }
                    break;
                case 4:
                    if(((M_lever4_start_munth_temp>>4)==0)&&(Menu_lever4_Pub_adj_cnt==0))
                        Menu_lever4_Pub_adj_cnt=1;
                    else if((M_lever4_start_munth_temp>>4)==1)
                    {
                        if(Menu_lever4_Pub_adj_cnt>2)
                            Menu_lever4_Pub_adj_cnt=2;
                    }

                    M_lever4_start_munth_temp&=0xf0;
                    M_lever4_start_munth_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                case 5:
                    if(Menu_lever4_Pub_adj_cnt>3)
                        Menu_lever4_Pub_adj_cnt=3;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_start_day_temp&=0x0f;
                    M_lever4_start_day_temp+=i;
                    break;
                case 6:
                    if(((M_lever4_start_day_temp>>4)==0)&&(Menu_lever4_Pub_adj_cnt==0))
                        Menu_lever4_Pub_adj_cnt=1;
                    else if((M_lever4_start_day_temp>>4)==3)
                    {
                        if(Menu_lever4_Pub_adj_cnt>1)
                            Menu_lever4_Pub_adj_cnt=1;
                    } 					//���꼰�����¾�����д

                    M_lever4_start_day_temp&=0xf0;
                    M_lever4_start_day_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                case 7:
                    if(Menu_lever4_Pub_adj_cnt>2)
                        Menu_lever4_Pub_adj_cnt=2;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_start_hour_temp&=0x0f;
                    M_lever4_start_hour_temp+=i;
                    break;
                case 8:
                    if(((M_lever4_start_hour_temp>>4)==2)&&(Menu_lever4_Pub_adj_cnt>=4))
                        Menu_lever4_Pub_adj_cnt=3;

                    M_lever4_start_hour_temp&=0xf0;
                    M_lever4_start_hour_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                case 9:
                    if(Menu_lever4_Pub_adj_cnt>5)
                        Menu_lever4_Pub_adj_cnt=5;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_start_mini_temp&=0x0f;
                    M_lever4_start_mini_temp+=i;
                    break;
                case 10:
                    M_lever4_start_mini_temp&=0xf0;
                    M_lever4_start_mini_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                }
                break;
            }
            break;
        case 2:
            switch(Menu_lever3_select)
            {
            case 1:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_FXCS_temp&=0x0f;
                    M_lever4_FXCS_temp+=i;
                    break;
                case 2:
                    if(((M_lever4_FXCS_temp>>4)==0)&&(Menu_lever4_Pub_adj_cnt==0))
                        Menu_lever4_Pub_adj_cnt=9;

                    M_lever4_FXCS_temp&=0xf0;
                    M_lever4_FXCS_temp+=Menu_lever4_Pub_adj_cnt;
                    break;
                }
                break;
            }
            break;
        case 3:
            switch(Menu_lever3_select)
            {
            case 1:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_menur1_jz1nd_temp&=0x0000ffff;
                    M_menur1_jz1nd_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_menur1_jz1nd_temp&=0x000f0fff;
                    M_menur1_jz1nd_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_menur1_jz1nd_temp&=0x000ff0ff;
                    M_menur1_jz1nd_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_jz1nd_temp&=0x000fff0f;
                    M_menur1_jz1nd_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_jz1nd_temp&=0x000ffff0;
                    M_menur1_jz1nd_temp+=i;
                    break;
                }
                break;
            case 2:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_menur1_jz2nd_temp&=0x0000ffff;
                    M_menur1_jz2nd_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_menur1_jz2nd_temp&=0x000f0fff;
                    M_menur1_jz2nd_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_menur1_jz2nd_temp&=0x000ff0ff;
                    M_menur1_jz2nd_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_jz2nd_temp&=0x000fff0f;
                    M_menur1_jz2nd_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_jz2nd_temp&=0x000ffff0;
                    M_menur1_jz2nd_temp+=i;
                    break;
                }
                break;
            }
            break;
        case 4:
            switch(Menu_lever3_select)
            {
            case 1:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_lever4_BYND_temp&=0x0000ffff;
                    M_lever4_BYND_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_lever4_BYND_temp&=0x000f0fff;
                    M_lever4_BYND_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_BYND_temp&=0x000ff0ff;
                    M_lever4_BYND_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_BYND_temp&=0x000fff0f;
                    M_lever4_BYND_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_lever4_BYND_temp&=0x000ffff0;
                    M_lever4_BYND_temp+=i;
                    break;
                }
                break;
            }
            break;
        }
        break;
    case 2:
        --Menu_lever4_Pub_adj_cnt;

        switch(Menu_index_finc_select)
        {
        case 12:
//     	    	    switch(Menu_lever3_select)
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:		//a
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt==0)
                        Menu_lever4_Pub_adj_cnt=2;
                    a_signle_flag=Menu_lever4_Pub_adj_cnt;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=24;
                    M_menur1_Fa_temp&=0x00ffffff;
                    M_menur1_Fa_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=20;
                    M_menur1_Fa_temp&=0x0f0fffff;
                    M_menur1_Fa_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_menur1_Fa_temp&=0x0ff0ffff;
                    M_menur1_Fa_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_menur1_Fa_temp&=0x0fff0fff;
                    M_menur1_Fa_temp+=i;
                    break;
                case 6:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_menur1_Fa_temp&=0x0ffff0ff;
                    M_menur1_Fa_temp+=i;
                    break;
                case 7:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_Fa_temp&=0x0fffff0f;
                    M_menur1_Fa_temp+=i;
                    break;
                case 8:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_Fa_temp&=0x0ffffff0;
                    M_menur1_Fa_temp+=i;
                    break;
                }
                break;
            case 2:		//b
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt==0)
                        Menu_lever4_Pub_adj_cnt=2;
                    b_signle_flag=Menu_lever4_Pub_adj_cnt;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=24;
                    M_menur1_Fb_temp&=0x00ffffff;
                    M_menur1_Fb_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=20;
                    M_menur1_Fb_temp&=0x0f0fffff;
                    M_menur1_Fb_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_menur1_Fb_temp&=0x0ff0ffff;
                    M_menur1_Fb_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_menur1_Fb_temp&=0x0fff0fff;
                    M_menur1_Fb_temp+=i;
                    break;
                case 6:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_menur1_Fb_temp&=0x0ffff0ff;
                    M_menur1_Fb_temp+=i;
                    break;
                case 7:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_Fb_temp&=0x0fffff0f;
                    M_menur1_Fb_temp+=i;
                    break;
                case 8:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_Fb_temp&=0x0ffffff0;
                    M_menur1_Fb_temp+=i;
                    break;
                }
                break;
            case 3:		//c
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt==3)
                        Menu_lever4_Pub_adj_cnt=1;
                    c_signle_flag=Menu_lever4_Pub_adj_cnt;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=24;
                    M_menur1_Fc_temp&=0x00ffffff;
                    M_menur1_Fc_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=20;
                    M_menur1_Fc_temp&=0x0f0fffff;
                    M_menur1_Fc_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_menur1_Fc_temp&=0x0ff0ffff;
                    M_menur1_Fc_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_menur1_Fc_temp&=0x0fff0fff;
                    M_menur1_Fc_temp+=i;
                    break;
                case 6:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_menur1_Fc_temp&=0x0ffff0ff;
                    M_menur1_Fc_temp+=i;
                    break;
                case 7:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_Fc_temp&=0x0fffff0f;
                    M_menur1_Fc_temp+=i;
                    break;
                case 8:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_Fc_temp&=0x0ffffff0;
                    M_menur1_Fc_temp+=i;
                    break;
                }
                break;
            case 4:		//d
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt==3)
                        Menu_lever4_Pub_adj_cnt=1;
                    d_signle_flag=Menu_lever4_Pub_adj_cnt;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=24;
                    M_menur1_Fd_temp&=0x00ffffff;
                    M_menur1_Fd_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=20;
                    M_menur1_Fd_temp&=0x0f0fffff;
                    M_menur1_Fd_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_menur1_Fd_temp&=0x0ff0ffff;
                    M_menur1_Fd_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_menur1_Fd_temp&=0x0fff0fff;
                    M_menur1_Fd_temp+=i;
                    break;
                case 6:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_menur1_Fd_temp&=0x0ffff0ff;
                    M_menur1_Fd_temp+=i;
                    break;
                case 7:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_Fd_temp&=0x0fffff0f;
                    M_menur1_Fd_temp+=i;
                    break;
                case 8:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_Fd_temp&=0x0ffffff0;
                    M_menur1_Fd_temp+=i;
                    break;
                }
                break;
            }
            break;
        }
        break;
    case 4:
        --Menu_lever4_Pub_adj_cnt;

        switch(Menu_index_finc_select)
        {
//     	    	case 1:
//     	    	    switch(Menu_lever3_select)
//     	    	    {
//     	    	    	case 1:
//     	    	    	break;
//     	    	    	case 2:
//     	    	    	break;
//     	    	    	case 3:
//     	    	    	break;
//     	    	    	case 4:
//     	    	    	break;
//     	    	    	case 5:
//     	    	    	break;
//     	    	    	case 6:
//     	    	    	break;
//     	    	    }
//     	    	break;
        case 2:
            switch(Menu_lever3_select)
            {
            case 2:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt<1)
                        Menu_lever4_Pub_adj_cnt=2;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_ZSB_speed_temp&=0x0f;
                    M_menur1_ZSB_speed_temp+=i;

                    if(M_menur1_ZSB_speed_temp>0x00000027)
                    {
                        M_menur1_ZSB_speed_temp=0x00000027;

                        i=M_menur1_ZSB_speed_temp;
                        i&=0x0f;
                        i=Get_ascii_data(i);

                        Get_6x8_char_single(3,36,i);
                        Display_6x8_char_single(3,36,i);
                    }
                    break;
                case 2:
////     	    	    	              if(((M_menur1_ZSB_speed_temp>>4)==0)&&(Menu_lever4_Pub_adj_cnt<5))
////     	    	    	                 Menu_lever4_Pub_adj_cnt=9;
                    if(((M_menur1_ZSB_speed_temp>>4)==2)&&(Menu_lever4_Pub_adj_cnt>7))
                        Menu_lever4_Pub_adj_cnt=7;

                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_ZSB_speed_temp&=0xf0;
                    M_menur1_ZSB_speed_temp+=i;
                    break;
                }
                break;
            case 3:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_move_to_temp&=0x000000ff;
                    M_lever4_move_to_temp+=i;
                    break;
                case 2:
                    i=M_lever4_move_to_temp>>8;
                    i&=0x0f;
                    if((i==9)&&(Menu_lever4_Pub_adj_cnt>5))
                        Menu_lever4_Pub_adj_cnt=5;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_move_to_temp&=0x00000f0f;
                    M_lever4_move_to_temp+=i;
                    break;
                case 3:
                    i=M_lever4_move_to_temp>>4;
                    if((i==0x95)&&(Menu_lever4_Pub_adj_cnt>8))
                        Menu_lever4_Pub_adj_cnt=8;

                    i=Menu_lever4_Pub_adj_cnt;
                    M_lever4_move_to_temp&=0x00000ff0;
                    M_lever4_move_to_temp+=i;
                    break;
                }
                break;
            }
            break;
        case 3:
            switch(Menu_lever3_select)
            {
            case 1:
                break;
            case 2:
                break;
            }
            break;
        case 4:
            switch(Menu_lever3_select)
            {
            case 1:
                break;
            case 2:
                break;
            }
            break;
        case 5:
#ifdef __OLD_JBSPEED__
            switch(Menu_lever3_select)
            {
            case 1:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt>2)
                        Menu_lever4_Pub_adj_cnt=2;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_menur1_JB_speed_temp&=0x0ff;
                    M_menur1_JB_speed_temp+=i;
                    break;
                case 2:
                    i=M_menur1_JB_speed_temp>>8;
                    i&=0x0f;
                    if((i==2)&&(Menu_lever4_Pub_adj_cnt>4))
                        Menu_lever4_Pub_adj_cnt=4;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_JB_speed_temp&=0xf0f;
                    M_menur1_JB_speed_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_JB_speed_temp&=0xff0;
                    M_menur1_JB_speed_temp+=i;
                    break;
                }
                break;
            }
#else
            switch(Menu_lever3_select)
            {
            case 1:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt<1)
                        Menu_lever4_Pub_adj_cnt=2;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_menur1_JB_speed_temp&=0x0ff;
                    M_menur1_JB_speed_temp+=i;

                    Check_JB_speed_edag();
                    break;
                case 2:
                    i=M_menur1_JB_speed_temp>>8;
                    i&=0x0f;
                    if((i==2)&&(Menu_lever4_Pub_adj_cnt>3))
                        Menu_lever4_Pub_adj_cnt=3;

                    if((i==1)&&(Menu_lever4_Pub_adj_cnt<7))
                        Menu_lever4_Pub_adj_cnt=9;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_JB_speed_temp&=0xf0f;
                    M_menur1_JB_speed_temp+=i;

                    Check_JB_speed_edag();
                    break;
                case 3:
                    i=M_menur1_JB_speed_temp>>4;
                    if((i==0x23)&&(Menu_lever4_Pub_adj_cnt>5))
                        Menu_lever4_Pub_adj_cnt=5;
                    else if(i==0x17)
                    {
                        if(Menu_lever3_Pub_adj_cnt<5)
                            Menu_lever3_Pub_adj_cnt=9;
                    }


                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_JB_speed_temp&=0xff0;
                    M_menur1_JB_speed_temp+=i;
                    break;
                }
                break;
            }
#endif
            break;
        case 6:
//     	    	    switch(Menu_lever3_select)
//     	    	    {
//     	    	    	case 1:
//     	    	    	break;
//     	    	    }
            break;
        case 7:
            switch(Menu_lever3_select)
            {
            case 1:
                break;
            case 2:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    BCWD_NG_flag=2;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_BCWD_temp&=0xf0f;
                    M_lever4_BCWD_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_lever4_BCWD_temp&=0xff0;
                    M_lever4_BCWD_temp+=i;
                    break;
                }
                break;
            }
            break;
        case 8:
            switch(Menu_lever3_select)
            {
            case 1:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt>5)
                        Menu_lever4_Pub_adj_cnt=5;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_menur1_led_dianliu_temp&=0x0000ffff;
                    M_menur1_led_dianliu_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_menur1_led_dianliu_temp&=0x000f0fff;
                    M_menur1_led_dianliu_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_menur1_led_dianliu_temp&=0x000ff0ff;
                    M_menur1_led_dianliu_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_menur1_led_dianliu_temp&=0x000fff0f;
                    M_menur1_led_dianliu_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_menur1_led_dianliu_temp&=0x000ffff0;
                    M_menur1_led_dianliu_temp+=i;
                    break;
                }
                break;
            case 2:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=20;
                    M_lever4_GDGDY_temp&=0x000fffff;
                    M_lever4_GDGDY_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_lever4_GDGDY_temp&=0x00f0ffff;
                    M_lever4_GDGDY_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_lever4_GDGDY_temp&=0x00ff0fff;
                    M_lever4_GDGDY_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_GDGDY_temp&=0x00fff0ff;
                    M_lever4_GDGDY_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_GDGDY_temp&=0x00ffff0f;
                    M_lever4_GDGDY_temp+=i;
                    break;
                case 6:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_lever4_GDGDY_temp&=0x00fffff0;
                    M_lever4_GDGDY_temp+=i;
                    break;
                }
                break;
            case 3:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    if(Menu_lever4_Pub_adj_cnt>5)
                        Menu_lever4_Pub_adj_cnt=5;

                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_PZDY_temp&=0x0ff;
                    M_lever4_PZDY_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_PZDY_temp&=0xf0f;
                    M_lever4_PZDY_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_lever4_PZDY_temp&=0xff0;
                    M_lever4_PZDY_temp+=i;
                    break;
                }
                break;
            case 4:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=20;
                    M_lever4_PZDYZ_temp&=0x000fffff;
                    M_lever4_PZDYZ_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_lever4_PZDYZ_temp&=0x00f0ffff;
                    M_lever4_PZDYZ_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_lever4_PZDYZ_temp&=0x00ff0fff;
                    M_lever4_PZDYZ_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_PZDYZ_temp&=0x00fff0ff;
                    M_lever4_PZDYZ_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_PZDYZ_temp&=0x00ffff0f;
                    M_lever4_PZDYZ_temp+=i;
                    break;
                case 6:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_lever4_PZDYZ_temp&=0x00fffff0;
                    M_lever4_PZDYZ_temp+=i;
                    break;
                }
                break;
            case 5:
                if(Menu_lever4_Pub_adj_cnt==0xff)
                    Menu_lever4_Pub_adj_cnt=9;

                switch(Menu_lever4_Pub_select_cnt)
                {
                case 1:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=20;
                    M_lever4_CFZ_temp&=0x000fffff;
                    M_lever4_CFZ_temp+=i;
                    break;
                case 2:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=16;
                    M_lever4_CFZ_temp&=0x00f0ffff;
                    M_lever4_CFZ_temp+=i;
                    break;
                case 3:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=12;
                    M_lever4_CFZ_temp&=0x00ff0fff;
                    M_lever4_CFZ_temp+=i;
                    break;
                case 4:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=8;
                    M_lever4_CFZ_temp&=0x00fff0ff;
                    M_lever4_CFZ_temp+=i;
                    break;
                case 5:
                    i=Menu_lever4_Pub_adj_cnt;
                    i<<=4;
                    M_lever4_CFZ_temp&=0x00ffff0f;
                    M_lever4_CFZ_temp+=i;
                    break;
                case 6:
                    i=Menu_lever4_Pub_adj_cnt;
                    M_lever4_CFZ_temp&=0x00fffff0;
                    M_lever4_CFZ_temp+=i;
                    break;
                }
                break;
            }
            break;
        }
        break;
    case 6:
        --Menu_lever4_Pub_adj_cnt;
        if(Menu_lever4_Pub_adj_cnt>9)
            Menu_lever4_Pub_adj_cnt=9;

        if((Menu_index_finc_select==1)&&(Menu_lever3_select==1))
        {
            switch(Menu_lever4_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever4_Pub_adj_cnt>=3)
                    Menu_lever4_Pub_adj_cnt=2;

                i=Menu_lever4_Pub_adj_cnt;
                i&=0x0000000f;
                Machine_note_temp&=0x00ff;
                Machine_note_temp|=(i<<8);
                break;
            case 2:
                i=Machine_note_temp;
                i&=0x00000f00;
                if(i==0x00000200)
                {
                    if(Menu_lever4_Pub_adj_cnt>=6)
                        Menu_lever4_Pub_adj_cnt=5;
                }
                i=Menu_lever4_Pub_adj_cnt;
                i&=0x0000000f;
                Machine_note_temp&=0x0f0f;
                Machine_note_temp|=(i<<4);
                break;
            case 3:
                i=Machine_note_temp;
                i&=0x00000ff0;
                if(i==0x00000250)
                {
                    if(Menu_lever4_Pub_adj_cnt>=6)
                        Menu_lever4_Pub_adj_cnt=5;
                }
                i=Menu_lever4_Pub_adj_cnt;
                i&=0x0000000f;
                Machine_note_temp&=0x0ff0;
                Machine_note_temp|=i;
                break;
            }
            Get_uart_note();
        }
        else
        {
            switch(Pwm_menu_lever)		//-
            {
            case 1:
                if(Pwm_menu_select<3)
                    ++Pwm_menu_select;
                else
                    Pwm_menu_select=1;
                Disp_pwm_menu();
                break;
            case 2:
                if(Pwm_menu_select1<5)
                    ++Pwm_menu_select1;
                else
                    Pwm_menu_select1=1;
                Disp_pwm_menu();
                break;
            case 3:
                --Pwm_adj_cnt;
                if(Pwm_adj_cnt==0xff)
                    Pwm_adj_cnt=9;
                Pwm_adj_data();
                break;
            }
            goto exit_l4_u;
        }
        break;
    }
    Lever4_adj_valu();
exit_l4_u:
    __nop();
}
//===============================================================================
//	finction	:Get_uart_note
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-8-13 17:03
//	modefy		:null
//===============================================================================
void Get_uart_note(void)		//ȡ�����ڵ�ֵ
{
    uint8_t i;
    uint16_t j;

    Machine_note_use=0;

    i=Machine_note_temp>>8;
    i&=0x0f;
    j=i*100;
    i=Machine_note_temp>>4;
    i&=0x0f;
    i*=10;
    j+=i;
    i=Machine_note_temp;
    i&=0x0f;
    j+=i;
    Machine_note_use=j;

    Com2_rd_ctrl(0);
}

//===============================================================================
//	finction	:Uart2_reinitial
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-8-13 17:03
//	modefy		:null
//===============================================================================
void Uart2_reinitial(void)		//����2��ʼ��
{
    STR_UART_T  sParam;

    Get_uart_note();

    DrvUART_Close(UART_PORT2);

    /* UART Setting */
    switch(Uart2_BAUD_select_temp)
    {
    case 1:
        sParam.u32BaudRate 	= 9600;
        break;
    case 2:
        sParam.u32BaudRate 	= 19200;
        break;
    case 3:
        sParam.u32BaudRate 	= 38400;
        break;
    }
    switch(Uart2_JOJY_select_temp)
    {
    case 1:
        sParam.u8cParity = DRVUART_PARITY_EVEN;
        break;
    case 2:
        sParam.u8cParity = DRVUART_PARITY_ODD;
        break;
    case 3:
        sParam.u8cParity = DRVUART_PARITY_NONE;
        break;
    }
    if(Uart2_STOP_select_temp==1)
        sParam.u8cStopBits = DRVUART_STOPBITS_1;
    else
        sParam.u8cStopBits = DRVUART_STOPBITS_2;

    sParam.u8cDataBits 		= DRVUART_DATABITS_8;
    sParam.u8cRxTriggerLevel    = DRVUART_FIFO_1BYTES;		//DRVUART_FIFO_1BYTES
//    sParam.u8TimeOut		= 0X010000;
    /* Set UART2 Configuration */
    DrvUART_Open(UART_PORT2,&sParam);

    /* Set CALL BACK FINCTION */
    DrvUART_EnableInt(UART_PORT2,(DRVUART_RDAINT |DRVUART_RLSINT),UART2_IRQHandler_APP);
    Com2_rd_ctrl(0);
}











