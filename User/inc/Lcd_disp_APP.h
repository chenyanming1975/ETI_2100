//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Lcd_disp_APP.h
//	First EDIT		: Sam
//	Data			: 2012.06.11
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================

#define		CMD_LCM_ON			0X000000AF
#define		CMD_LCM_OFF			0X000000AE
#define		CMD_DIS_START_LINE		0X00000060	//01XXXXXXB
#define		CMD_PAGE_ADDS_SET		0X000000B0	//1011XXXXB
#define		CMD_LADDS_HI			0X00000010	//0001XXXXB
#define		CMD_LADDS_LOW			0X00000001	//0000XXXXB
#define		CMD_DIS_ADDS_L2R		0X000000A0
#define		CMD_DIS_ADDS_R2L		0X000000A1
#define		CMD_MORMAL_DISP			0X000000A6
#define		CMD_REVERSE_DISP		0X000000A7
#define		CMD_NDISP_ALL_DOT		0X000000A4
#define		CMD_DISP_ALL_DOT		0X000000A5
#define		CMD_SET_BIAS_VOL9		0X000000A2
#define		CMD_SET_BIAS_VOL7		0X000000A3
#define		CMD_FW_RESET			0X000000E2
#define		CMD_NOR_LINE_SCAN		0X000000C0
#define		CMD_REV_LINE_SCAN		0X000000C8	//C1
#define		CMD_POWER_CONTRL		0X00000028	//00101XXXB
#define		CMD_INTER_RESE_SET		0X00000020

#define		CMD_INTER_LCD_VOL_SET		0X00000081
#define		CMD_INTER_LCD_VOL_DATA		0X00000000	//0~64 LEVEL FOR SET

#define		CMD_STACT_DISP_CLOSE		0X000000AC	
#define		CMD_STACT_DISP_OPEN		0X000000AD
	
#define		CMD_NOP_OPRATION		0X000000E3	

#define		MENU_LV1_NUM			7	
#define		MENU_LV21_NUM			4	
#define		MENU_LV22_NUM			18	//14	//
#define		MENU_LV23_NUM			21	//18	
#define		MENU_LV24_NUM			9	//16	
#define		MENU_LV25_NUM			4	//5	
#define		MENU_LV26_NUM			6
#define		MENU_LV27_NUM			10

#define		MENU_LV2319_NUM			3
//------------ lever3 menu4	
#define		MENU_LV331_NUM			8	//6	
#define		MENU_LV332_NUM			3	
#define		MENU_LV333_NUM			14	
#define		MENU_LV334_NUM			8				
#define		MENU_LV335_NUM			4
#define		MENU_LV336_NUM			2	
#define		MENU_LV337_NUM			2	
#define		MENU_LV338_NUM			5
#define		MENU_LV339_NUM			2
	
#define		MENU_LV341_NUM			4	//5	
#define		MENU_LV342_NUM			4	//5	
#define		MENU_LV343_NUM			3	//4	
#define		MENU_LV344_NUM			3	//4	
#define		MENU_LV345_NUM			6
		
#define		MENU_LV351_NUM			6	
#define		MENU_LV352_NUM			2	
#define		MENU_LV353_NUM			3	
#define		MENU_LV354_NUM			2
		
#define		MENU_LV361_NUM			5	
#define		MENU_LV362_NUM			2	//12	
#define		MENU_LV363_NUM			8	
#define		MENU_LV364_NUM			4	
#define		MENU_LV365_NUM			0	
#define		MENU_LV366_NUM			0	

#define		MENU_LV371_NUM			0
#define		MENU_LV372_NUM			0	
#define		MENU_LV373_NUM			0
#define		MENU_LV374_NUM			0
#define		MENU_LV375_NUM			0
#define		MENU_LV376_NUM			0
#define		MENU_LV377_NUM			0
#define		MENU_LV378_NUM			0

#define		SUPER_PASS_WORD			0X00273169
#define		ENG_PASS_WORD			0X00182037
#define		CTRL_PASS_WORD			0X00171717
//#define		CTRL_PASS_WORD			0X00100000
#define		USER_PASS_WORD			0X00123456

#define		SYSTEM_SERIES_BIT		11

#define		MAIN_MENU			10

//--------------------------------------------------------------------------------------------
#ifdef	__LCD8080_AP__
void Disp_pwm_menu(void);
void Display_menu_index(void);

volatile uint8_t prg_wait_clr_flag;
volatile uint32_t Set_xishu;
volatile uint8_t  Lever3_menu_line;
volatile uint8_t  Seg_adds_bak;
volatile uint8_t  Seg_rev_lenth;
volatile uint16_t  Far_status_flag;

#else
extern uint8_t  Disp_buff128x64[1024];		//2013-3-18 12:01

extern volatile uint8_t  prg_wait_clr_flag;  
extern volatile uint32_t Set_xishu;
extern volatile uint8_t  Lever3_menu_line;
extern volatile uint8_t  Seg_adds_bak;
extern volatile uint8_t  Seg_rev_lenth;
extern volatile uint16_t  Far_status_flag;


extern uint8_t  User_prio_temp;
extern uint8_t	 Check_pass_word_flag;
extern uint8_t	 Pass_bit_select;
extern uint8_t	 Pass_adj_cnt;
extern uint32_t Key_input_pass_temp;

extern uint8_t  Disp_gdgdy_flag;
extern uint8_t  LCD_disp_flag;
extern uint8_t  LCD_Contrast;
extern uint8_t  L4_seg_temp;
extern uint8_t  Pro_dis_menu_flag; 	//2012-7-26 20:03
extern uint8_t  a_signle_flag;
extern uint8_t  b_signle_flag;
extern uint8_t  c_signle_flag;
extern uint8_t  d_signle_flag;
extern uint32_t back_l_temp;

extern volatile uint8_t Rx_TX_flag_local;

extern void Lcd_initial(void);
extern void Clear_screen(void);
extern void Clear_apage(uint8_t Page_addrs);
extern void Clear_8x8_area(uint8_t Page_addrs,uint8_t Seg_addrs_h,uint8_t Seg_addrs_l);
extern void Clear_a_byte_seg(uint8_t Page_addrs,uint8_t Seg_addrs_h,uint8_t Seg_addrs_l);
extern void Set_a_byte_seg(uint8_t Page_addrs,uint8_t Seg_addrs_h,uint8_t Seg_addrs_l);
extern void Disp_a_byte_seg(uint8_t Page_addrs,uint8_t Seg_addrs_h,uint8_t Seg_addrs_l,uint8_t Seg_data);
extern void Disp_16x16_area(uint8_t Page_addrs,uint8_t Seg_addrs_h,uint8_t Seg_addrs_l,uint8_t const *address);
extern void Disp_8x8_area(uint8_t Page_addrs,uint8_t Seg_addrs_h,uint8_t Seg_addrs_l,uint8_t const *address);
extern void Disp_apage(uint8_t Page_addrs,uint8_t const *address);
extern void Disp_ascreen(uint8_t const *address);
extern void Display_logo(uint8_t const *address);
extern void Disp_16x64_area(uint8_t Seg_addrs_h,uint8_t Seg_addrs_l,uint8_t const *address);
extern void Disp_8x64_area(uint8_t Seg_addrs_h,uint8_t Seg_addrs_l,uint8_t const *address);

//extern void Adj_lcd_Contrast_up(void);
//extern void Adj_lcd_Contrast_down(void);

extern void Adj_lcd_Contrast_data(void);

extern void Display_menu_index(void);
extern void Lever3_select_finc_adds(void);
extern void Disp_lever1_menu(void);
extern void Display_lever4_index(void);
extern void Tx_data_to_lcm_p(uint8_t const *data);
extern void Display_8x16_char(uint8_t page_adds,uint8_t seg_adds,uint8_t total_num,uint8_t data_buff[]);
extern void Display_8x16_char_single(uint8_t page_adds,uint8_t seg_adds,uint8_t data);
extern void Disp_rtc_clock(void);
extern void Menu1_adj_valu(void);
extern void Lever4_adj_valu(void);
extern uint8_t Display_lever4_en_flag(void);
extern void Display_program_menu(uint8_t num,uint8_t disp_buff[]);
extern void Display_program_result(uint8_t num);
extern void Display_program_5opration(uint8_t num,uint8_t disp_buff[]);
extern uint8_t Get_ascii_data(uint8_t data);
extern void Disp_rtc_clock_wait(void);
extern void Display_program_5opration_re(uint8_t num,uint8_t disp_buff[]);

extern void Disp_GDGDY_real_time(void);
extern void Display_last_result(uint32_t adds);
extern void Disp_pwm_menu(void);
extern void Pwm_select_finc(void);
extern void Pwm_adj_data(void);

extern void Input_pass_word_disp(void);
extern void Pass_select_disp(uint8_t bit_select);
extern void Pass_adj_disp(uint8_t Adj_data,uint8_t bit_select);
extern void Clead_pass_word_reg_data(void);

extern uint8_t const TB_number[];
extern uint8_t const Position_bit[];

extern uint8_t const Lever1_menu_index[];
extern uint8_t const *Lever1_menu[];
extern uint8_t const TB_lever1_menu1[];
extern uint8_t const TB_lever1_menu2[];
extern uint8_t const TB_lever1_menu3[];
extern uint8_t const TB_lever1_menu4[];
extern uint8_t const TB_lever1_menu5[];
extern uint8_t const TB_lever1_menu6[];

extern uint8_t const Lever2_menu1_index[];
extern uint8_t const *Lever2_menu1[];

extern uint8_t const TB_num_char_8x12[];

extern uint8_t const Prog_en_menu_index[];
extern uint8_t const Prog_en_menu_char[];

extern uint8_t const Lever1_en_menu_index[];
extern uint8_t const Lever1_en_menu_char[];
extern uint8_t const Lever1_menu_index[];	                          
extern uint8_t const TB_Lever1_menu[];

extern void Display_seg_single(uint8_t page_adds,uint8_t seg_adds,uint8_t data);

extern void Display_logo_pic(uint8_t const *address);		//2013-3-12 16:20
extern uint8_t const cm_logo_pic[];				//2013-3-12 16:21
extern uint8_t const cm1_logo_pic[];				//2013-6-19 16:11
extern void Display_6x8_char(uint8_t page_adds,uint8_t seg_adds,uint8_t total_num,uint8_t data_buff[]);
extern void Display_6x8_char_single(uint8_t page_adds,uint8_t seg_adds,uint8_t data);
extern void Clear_n_page(uint8_t start,uint8_t Page_addrs);
extern void Clear_n_page(uint8_t start,uint8_t Page_addrs);
extern void Get_6x8_char_single(uint8_t page_adds,uint8_t seg_adds,uint8_t data);
extern void Get_8x16_char_single(uint8_t page_adds,uint8_t seg_adds,uint8_t data);
/////extern void Display_16x16_char_single(uint8_t page_adds,uint8_t seg_adds,uint8_t data);
extern void Get_16x16_char_single(uint8_t page_adds,uint8_t seg_adds,uint8_t data);
extern void Display_all_buff(void);
extern uint8_t rev_clear_set_arear(uint8_t c_page,uint8_t c_seg,uint8_t c_lenth,uint8_t s_page,uint8_t s_seg,uint8_t s_lenth);
extern uint8_t rev_clear_set_arear16(uint8_t c_page,uint8_t c_seg,uint8_t c_lenth,uint8_t s_page,uint8_t s_seg,uint8_t s_lenth);
extern void Disp_lever3_menu(void);
extern void Clear_n_page_ram(uint8_t start,uint8_t Page_addrs);
extern uint8_t Check_prio_finc(void);

#endif

