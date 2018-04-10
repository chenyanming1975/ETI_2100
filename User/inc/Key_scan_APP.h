//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Key_scan_APP.h
//	First EDIT		: Sam
//	Data			: 2012.06.13
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================

//--------------------------------------------------------------------------------------------

#define		KEY_CODE_UP			0x0000007D
#define		KEY_CODE_DOWN			0x0000007E
#define		KEY_CODE_LEFT			0x0000007B
#define		KEY_CODE_RIGHT			0x00000077
#define		KEY_CODE_ESC			0x0000006F
#define		KEY_CODE_ENTER			0x0000005F
#define		KEY_CODE_POWER			0x0000003F

#define		KEY_CODE_EXPORT			0x00000073			//2012-9-7 13:40


//--------------------------------------------------------------
#define 	ZHUSEBUN_RESET			0X00A5
#define 	BATTERY_VAL_OPEN		0X0001
#define 	BATTERY_VAL_CLOSE		0X0000
//#define 	BATTERY_VAL_OPEN		0X00A5
#ifdef	__KEY_APP__
volatile uint8_t Lever2_menu_num;

volatile uint8_t fix_axbc_falg;

volatile uint8_t fix_start_time_falg;

uint8_t Pwm_menu_lever;
uint8_t Pwm_menu_select;
uint8_t Pwm_menu_select1;
uint8_t Pwm_menu_bit_temp;
uint8_t Pwm_menu_PoSeg;
uint8_t Pwm_menu_PoSeg_pub;
uint8_t Pwm_select_cnt;
uint8_t Pwm_adj_cnt;

void Get_uart_note(void);

volatile uint8_t wd_use_flag;			//2015-7-3 10:10
volatile uint8_t wd_use_flag_chg;		//2015-7-3 10:10
volatile uint8_t wd_20_change_flag;		//2015-7-3 13:52
volatile uint8_t wd_data_cnt;			//2015-7-3 13:52
volatile float wd_data_buf0;
volatile float wd_data_buf1;
volatile float wd_data_buf2;
volatile float wd_data_buf3;
volatile float wd_data_used;
volatile uint16_t wd_use_ramdom;			//2015-7-3 10:10

#else
extern volatile uint8_t wd_use_flag;		//2015-7-3 10:10
extern volatile uint8_t wd_use_flag_chg;	//2015-7-3 10:10
extern volatile uint8_t wd_20_change_flag;	//2015-7-3 13:52
extern volatile uint8_t wd_data_cnt;		//2015-7-3 13:52
extern volatile float wd_data_buf0;
extern volatile float wd_data_buf1;
extern volatile float wd_data_buf2;
extern volatile float wd_data_buf3;
extern volatile float wd_data_used;
extern volatile uint16_t wd_use_ramdom;			//2015-7-3 10:10

extern volatile uint8_t fix_start_time_falg;

extern volatile uint8_t Lever2_menu_num;

extern volatile uint8_t fix_axbc_falg;

extern uint8_t Machine_note_use;
extern uint16_t Machine_note_temp;

extern uint8_t Key_code_bak;
extern uint8_t Key_scan_time_cnt;
extern uint8_t Pwm_menu_lever;
extern uint8_t Pwm_menu_select;
extern uint8_t Pwm_menu_select1;
extern uint8_t Pwm_menu_bit_temp;
extern uint8_t Pwm_menu_PoSeg;
extern uint8_t Pwm_menu_PoSeg_pub;
extern uint8_t Pwm_select_cnt;
extern uint8_t Pwm_adj_cnt;

extern volatile uint32_t E_wait_time_temp;			//2012-9-14 13:48
extern volatile uint32_t Prj_correlat;				//2012-9-14 13:48

extern uint32_t Allow_error;				//2012-11-7 15:18
extern uint32_t Alarm_hight_nd;				//2012-11-8 11:03
extern uint32_t Alarm_low_nd;				//2012-11-8 11:03
extern uint8_t	Alarm_active_flag;			//2012-11-8 11:03

extern uint8_t	Export_data_flag;			//2012-9-7 13:47
extern uint8_t  DCF_switch_select_flag;			//2012-7-18 15:04
extern volatile uint8_t  BCWD_NG_flag;				//2012-8-2 9:31

extern volatile uint8_t Menu_lever4_bit_temp;
extern uint8_t Menu_lever3_PoPa;
extern uint8_t Menu_lever3_PoSeg;
extern uint8_t Menu_lever4_PoPa;
extern uint8_t Menu_lever4_PoSeg;
extern uint8_t Menu_lever3_Pub_select_cnt;
extern uint8_t Menu_lever3_Pub_adj_cnt;

extern uint8_t Menu_lever4_Pub_select_cnt;		//for position 2012-7-6 9:14
extern uint8_t Menu_lever4_Pub_adj_cnt;			//2012-7-6 14:31


extern uint8_t Menu_lever_select;
extern uint8_t Menu_index_select;
extern uint8_t Menu_index_finc_select;
extern uint8_t Menu_lever1_point;
extern uint8_t Menu_lever2_point;
extern uint8_t Menu_lever3_point;
extern uint8_t Menu_lever3_select;

extern volatile uint8_t Menu_lever3_start_flag;

extern volatile uint16_t M_menur1_led_data_temp;			//波长
extern volatile uint8_t  M_menur1_lcd_Contrast_temp;
extern volatile uint8_t  M_menur1_language_select_temp;
extern volatile uint8_t  M_menur1_analy_obj_select_temp;
extern volatile uint32_t  M_menur1_lange_data_temp;
extern volatile uint8_t  M_menur1_unit_select_temp;
extern volatile uint32_t  M_menur1_led_dianliu_temp;
extern volatile uint8_t  M_menur1_dot_num_select_temp;
extern volatile uint32_t M_menur1_sample_v_temp;
extern volatile uint8_t  M_dongtai_flag;
extern volatile uint8_t  M_menur1_ZDXS_select_temp;
extern volatile uint8_t  M_menur1_LZDJ_select_temp;		//离子电子 2012-7-10 16:35
extern volatile uint8_t  M_menur1_DL_select_temp;		//定量方法 2012-7-10 16:36
extern volatile uint8_t  M_menur1_direction_select_temp;	//搅拌方向 2012-7-10 16:38
extern volatile uint32_t M_menur1_JB_speed_temp;		//2012-7-10 16:39
extern volatile uint32_t M_menur1_ZSB_speed_temp;		//2012-7-10 16:39

         
extern volatile uint32_t M_menur1_jz1nd_temp;
extern volatile uint32_t M_menur1_jz1_v_temp;
extern volatile uint32_t M_menur1_jz2nd_temp;
extern volatile uint32_t M_menur1_jz2_v_temp;
           
extern volatile uint32_t M_menur1_sj1_v_temp;
extern volatile uint32_t M_menur1_sj2_v_temp;
extern volatile uint32_t M_menur1_sj3_v_temp;
extern volatile uint32_t M_menur1_sj4_v_temp;
          
extern volatile uint32_t M_menur1_slope_temp;			//斜率
extern volatile uint8_t	M_menur1_slope_flag;
extern volatile uint8_t	Updata_slope_flag;

extern volatile uint8_t	Disple_flag;				//是否消解标志
       
extern volatile uint32_t M_menur1_FY_temp;
extern volatile uint32_t M_menur1_Fx_temp;
extern volatile uint32_t M_menur1_Fa_temp;
extern volatile uint32_t M_menur1_Fb_temp;
extern volatile uint32_t M_menur1_Fc_temp;
extern volatile uint32_t M_menur1_Fd_temp;

extern volatile uint8_t	 M_menur1_RX_time_temp;		//润洗次数
extern volatile uint32_t M_menur1_QYSJ_temp;
extern volatile uint32_t M_menur1_CSSJ_temp;
extern volatile uint32_t M_menur1_XHPY_temp;
extern volatile uint32_t M_menur1_DDSJ_temp;

extern volatile uint32_t M_lever4_FXPL_temp;
extern volatile uint32_t M_lever4_JZPL_temp;
extern volatile uint32_t M_lever4_QXPL_temp;
extern volatile uint32_t M_lever4_HCPL_temp;

extern volatile uint8_t		M_lever4_extern_trrig_flag;
extern volatile uint8_t		M_lever4_start_year_temp;		//2014-1-15 10:09 add
extern volatile uint8_t		M_lever4_start_hour_temp;
extern volatile uint8_t		M_lever4_start_mini_temp;
extern volatile uint8_t		M_lever4_start_munth_temp;
extern volatile uint8_t		M_lever4_start_day_temp;
extern volatile uint8_t		M_lever4_FXCS_temp;
extern volatile uint32_t	M_lever4_BYND_temp;
extern volatile uint32_t	M_lever4_move_to_temp;
extern volatile uint32_t	M_lever4_WD_temp;
extern volatile uint16_t	M_lever4_BCWD_temp;
extern volatile uint32_t	M_lever4_GDGDY_temp;
extern volatile uint32_t	M_lever4_PZDY_temp;
extern volatile uint32_t	M_lever4_PZDYZ_temp;
extern volatile uint32_t	M_lever4_CFZ_temp;

extern uint32_t	M_lever4_Out1_4ma_temp;
extern uint32_t	M_lever4_Out1_20ma_temp;
extern uint32_t	M_lever4_Out1_test_ma_temp;
extern uint32_t	M_lever4_Out2_4ma_temp;
extern uint32_t	M_lever4_Out2_20ma_temp;
extern uint32_t	M_lever4_Out2_test_ma_temp;

extern uint32_t	M_lever4_tp_Out1_4ma_temp;
extern uint32_t	M_lever4_tp_Out1_20ma_temp;
extern uint32_t	M_lever4_tp_Out1_test_ma_temp;
extern uint32_t	M_lever4_tp_Out2_4ma_temp;
extern uint32_t	M_lever4_tp_Out2_20ma_temp;
extern uint32_t	M_lever4_tp_Out2_test_ma_temp;

extern uint32_t	M_lever4_jz_Out1_4ma_temp;
extern uint32_t	M_lever4_jz_Out1_20ma_temp;
extern uint32_t	M_lever4_jz_Out1_test_ma_temp;
extern uint32_t	M_lever4_jz_Out2_4ma_temp;
extern uint32_t	M_lever4_jz_Out2_20ma_temp;
extern uint32_t	M_lever4_jz_Out2_test_ma_temp;

extern volatile uint8_t	M_lever3_BYHC_temp;		//2012-7-25 11:52 add
extern volatile uint32_t	M_lever3_HCPD_temp;		//2012-7-25 11:53 add
 
//extern uint16_t M_menur3_battery_all_set_temp;
//extern uint16_t M_menur3_direction_temp;
extern volatile uint16_t M_menur3_switch_temp;
extern volatile uint16_t M_menur3_ISE_temp;

extern volatile uint8_t  Hand_finction_select;
extern volatile uint16_t Hand_finction_wait_cnt;		//bit16 for flag
extern volatile uint8_t  Hand_finction_step_select;

extern volatile uint32_t	set_cal_data;			//2012-12-28 14:58 add

extern int32_t Read_IC_165_Data(void);
extern void Key_scan_prosecce(void);
extern void Key_scan_bounce_time(void);

extern void Menu_lever3_4_enter_finction(void);
extern void Hand_opration_finction(void);
extern void System_fill(void);
extern void Standar1_fill(void);
extern void Standar2_fill(void);
extern void Reagent1_fill(void);
extern void Reagent2_fill(void);
extern void Reagent3_fill(void);
extern void Sample_fill(void);

extern void Get_uart_note(void);

#endif

