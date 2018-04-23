//============================================================================================
//	Project name	: andan test
//	MCU TYPE		: Nuv100RE3AN
//	Finction		: main
//	First EDIT		: Sam
//	Data			: 2012.06.06
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================
//--------------------include files------------------------
#include "Incude_all.h"
//-------------------Pub Counst-- --------------------------

void Program_power_run_ctrl(void);
void Export_data_ctrl_finc(void);


//-------------------Pub Variable --------------------------
/////static uint16_t  System_status_cnt;
//uint8_t	Tx_buff[4]={0,1,2,3};

//===============================================================================
//	finction	:main
//	edit		:sam 2012.06.06
//	modefy		:null
//===============================================================================
int32_t	main(void)
{
    uint8_t i,data_buff[6];
//   int32_t Sub_adds;

    System_clk_initial();	  //初始化系统时钟
    DrvSYS_Delay(6000000);
    GPIO_initial();		      //初始化I/O
    DrvTIMER_Init();          //初始化系统定时器
    Timer0_initial();         //初始化定时器0
    Timer1_initial();		  //初始化定时器1
    Init_UART012();           //初始化串口0(232-usb).1(SP485R JP4).2(MAX3485)
    SPI_flash_init();		  //初始化SPI 2012-10-25 14:51
    Lcd_initial(); 			  //初始化LCD

#ifdef __THERMO__
    Display_logo_pic(cm1_logo_pic);			//显示tnermo公司LOGO
#else
    Clear_n_page(0,7);
    data_buff[0]=33;//“系统初始化”在字库中的索引
    data_buff[1]=34;
    data_buff[2]=203;
    data_buff[3]=15;
    data_buff[4]=204;
    data_buff[5]=0;
    for(i=0; i<5; i++)
    {
        Get_16x16_char_single(3,i*18+20,data_buff[i]);
    }
    Display_all_buff();

#endif
    System_reg_initial();         			//系统参数初始化
#ifdef __DEBUG__

    Pro_power_on_flag=0;
#endif
    Sd_io_initial();					//检查是否有SD卡并初始化
    if(Rd_check_lcd_data())				///2013-3-15 11:06 add 检查并更新字库
    {
        Check_updta_lcd_data();
    }
    else
    {
        if((DrvGPIO_GetBit(E_GPC,0))==0)//检测是否有插入SD卡
            Check_updta_lcd_data();
        Rd_init_lcd_data();
    }

    Power_on_check_device();//检查驱动板和传感器板是否存在
    InitPWM();   					  //初始化PWM输出
    Back_light_time_flag=1;
    Back_light_time_cnt=0;
/////   Display_menu_index();

//    if((buff_bool[0]==0)&&(buff_bool[1]==0)&&(buff_bool[2]==0)&&(buff_bool[3]==0))
//    {
//        Pro_power_on_flag=0;
//    }
//    else
    {
        Display_menu_index();   	//显示最后一次分析结果
    }

    RTC_read_12byte_data();
    RTC_Set = RTC_DATA;             //初始化RTC芯片，启动运行
    RTC_initial();                  
    Set_en_buzz_finc(1,4,200,100);//蜂鸣器参数设置
    DrvSYS_Delay(200000);				  //delay 200ms
    Pro_wait_time_flag=2;
#ifdef __UART_TEST__					    //北京认证上传数据协议的标志，已经不用。
    Test_potocal_time_cnt=0;
    Test_potocal_time_mini_cnt=0;
    Test_potocal_time_flag=2;		 //flag
#endif

    WDT_initial();					     //初始化看门狗
    while(1)
    {
        WDT_CLR();//clear WDT
        Key_scan_prosecce();			//按键处理
        Uart012_FIFO_INT_CTRL();	//串口接收处理

#ifndef __DEBUG__
        Program_power_on_initial();	//after initial this finction will not run .
#endif
        Program_power_run_ctrl();		//自动分析，手动分析，校正等流程控制。
        Hand_opration_finction();		//手动操作流程控制
        Disp_GDGDY_real_time();			//显示温度，光电管电压等 2012-9-6 17:01
        Export_data_ctrl_finc();		//导分析数据函数 2012-9-7 13:49
        Pwm_output_ctrl();			    //4~20ma输出控制
#ifdef __UART_TEST__
        Run_potocal_test(); 			  //北京认证通信协议控制
#endif
        Sd_io_initial();			      //检查是否有SD卡并初始化
        //Aoto_program_ctrl();			//软件自动更新控制函数		//2014-3-11 9:01 del
        Program_reset_initial();		//停止各分析流程后重新设置部分参数

        Extern_port_triggle_ctrl();			//2016/6/15 14:32
    }
}
//==================================================================================================================
//==================================================================================================================
//==================================================================================================================
//===============================================================================
//	finction	:Export_data_ctrl_finc	导数据控制函数
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-7 13:51
//	modefy		:null
//===============================================================================
void Export_data_ctrl_finc(void)
{
    if((Export_data_flag==1)&&(Buzz_en_flag==0))
    {
        Export__flash_data_finc();
    }
    else if(Export_data_flag==0)
    {
        Export_data_flag=2;
    }
}

//===============================================================================
//	finction	:Program_power_run_ctrl
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-25 10:42
//	modefy		:null
//===============================================================================
void Program_power_run_ctrl(void)
{
    uint8_t i;

    if((Pro_power_on_flag==0) && (Program_reset_initial_flag!=1))
    {
        if(Menu_lever3_start_flag == BIT4)		//自动分析
        {
            if(Auto_check_clock_flag==0)
                Menu1_auto_fenxi_finc();
            else
            {
                Disp_rtc_clock_wait();

                i = RTC_DATA.year_temp;
//                i &= 0x7f;
                if(M_lever4_start_year_temp <i )//自动分析开始时间的年单位 小于当前时间的年单位（开始时间不在今年）
                {
                    Auto_check_clock_flag=0;
                    Pro_auto_time_dcnt=0;
                    Pro_dis_menu_flag=0;
                    Menu1_auto_fenxi_finc();
                }
                else if(M_lever4_start_year_temp==i)//开始时间为今年
                {
                    i=RTC_DATA.mouth_temp & 0x1f;
                    if(M_lever4_start_munth_temp<i)//开始月数不在当月
                    {
                        Auto_check_clock_flag=0;
                        Pro_auto_time_dcnt=0;
                        Pro_dis_menu_flag=0;
                        Menu1_auto_fenxi_finc();
                    }
                    else if(M_lever4_start_munth_temp==i)//开始月为当月
                    {
                        i=RTC_DATA.day_temp & 0x3f;
                        if(M_lever4_start_day_temp<i)//开始日不是当日
                        {
                            Auto_check_clock_flag=0;
                            Pro_auto_time_dcnt=0;
                            Pro_dis_menu_flag=0;
                            Menu1_auto_fenxi_finc();
                        }
                        else if(M_lever4_start_day_temp==i)//开始日为当日
                        {
                            i=RTC_DATA.hour_temp & 0x3f;
                            if(M_lever4_start_hour_temp<i)//开始时不为当时
                            {
                                Auto_check_clock_flag=0;
                                Pro_auto_time_dcnt=0;
                                Pro_dis_menu_flag=0;
                                Menu1_auto_fenxi_finc();
                            }
                            else if(M_lever4_start_hour_temp==i)//开始小时为当时
                            {
                                i=RTC_DATA.minu_temp & 0x7f;
                                if(M_lever4_start_mini_temp<=i)//开始分
                                {
                                    Auto_check_clock_flag=0;
                                    Pro_auto_time_dcnt=0;
                                    Pro_dis_menu_flag=0;
                                    Menu1_auto_fenxi_finc();
                                }
                            }
                        }
                    }
                }
            }
        }
        else if(Menu_lever3_start_flag==BIT5)		//手动分析
        {
            if(M_lever4_FXCS_cnt==M_lever4_FXCS_temp)
            {
                M_lever4_FXCS_cnt=0;
                Menu_lever3_start_flag&=~BIT5;
                Write_system_reg_to_chip();
                Prog_disp_flag=0;
            }
            else 
				Menu1_hand_fenxi_finc();
        }
        else if(Menu_lever3_start_flag==BIT6)		//手动校正
            Menu1_hand_jiaozheng_finc();
        else if(Menu_lever3_start_flag==BIT7)		//手动核查
            Menu1_biaoye_hecha_finc();
        else
        {
            __nop();
        }
    }
}
