//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Opration_finc.h
//	First EDIT		: Sam
//	Data			: 2012-7-18 10:27
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================

#define  ZSB_MOTO_STEP_V	8333		//8333 uL

#ifdef	__OPRATION_F__
volatile uint8_t Reget_slope;
volatile uint8_t U2_disple_flag;
volatile uint8_t  Program_reset_initial_flag;
volatile uint32_t Program_reset_initial_cnt; 
uint8_t Wait_data_buff[13];

void Hand_opration_finction(void);
void System_fill(void);
void Standar1_fill(void);
void Standar2_fill(void);
void Reagent1_fill(void);
void Reagent2_fill(void);
void Reagent3_fill(void);
void Reagent4_fill(void);
void Sample_fill(void);
void All_program_menu_disp(uint8_t selest);
uint8_t Get_step_num_each_finc(uint8_t selset);

//===============================================================================
#define OPV01		0x00
#define CLSV01		0x01
#define OPV02		0x02
#define CLSV02		0x03
#define OPV03		0x04
#define CLSV03		0x05
#define OPV04		0x06
#define CLSV04		0x07
#define OPV05		0x08
#define CLSV05		0x09
#define OPV06		0x0a
#define CLSV06		0x0b
#define OPV07		0x0c
#define CLSV07		0x0d
#define OPV08		0x0e
#define CLSV08		0x0f
#define OPV08_5S	0x10
#define OPV08_10S	0x11
#define OPV09		0x18
#define CLSV09		0x19
#define OPV10		0x1a
#define CLSV10		0x1b
#define OPV11		0x1c
#define CLSV11		0x1d
#define OPV12		0x1e
#define CLSV12		0x1f
#define OPV13		0x20
#define CLSV13		0x21
#define ZSB_T0_00S	0x12
#define ZSB_T0_01S	0x13
#define ZSB_T0_05S	0x14
#define ZSB_T0_20S	0x23
#define ZSB_T_5ML	0x15
#define ZSB_T_7ML	0x22
#define ZSB_T_9ML	0x16
#define STOP_MOTO	0x17
//===============================================================================
uint8_t const TB_Reagent1_fill_index[]={
	                              OPV08_10S,CLSV01,CLSV03,CLSV04,CLSV05,CLSV06,CLSV07,ZSB_T0_00S,OPV04,ZSB_T_5ML,CLSV04,
	                              ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV08,CLSV01,ZSB_T_5ML,
	                              0x88,OPV08_5S,CLSV08,STOP_MOTO,ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,0xff,
                                     };	                          
//===============================================================================
uint8_t const TB_Reagent2_fill_index[]={
	                              OPV08_10S,CLSV01,CLSV03,CLSV04,CLSV05,CLSV06,CLSV07,ZSB_T0_00S,OPV05,ZSB_T_5ML,CLSV05,
	                              ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV08,CLSV01,ZSB_T_5ML,
	                              0x88,OPV08_5S,CLSV08,STOP_MOTO,ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,0xff,
                                     };	                          
//===============================================================================
uint8_t const TB_Reagent3_fill_index[]={
	                              OPV08_10S,CLSV01,CLSV03,CLSV04,CLSV05,CLSV06,CLSV07,ZSB_T0_00S,OPV06,ZSB_T_5ML,CLSV06,
	                              ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV08,CLSV01,ZSB_T_5ML,
	                              0x88,OPV08_5S,CLSV08,STOP_MOTO,ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,0xff,
                                     };	                          
//===============================================================================
uint8_t const TB_Reagent4_fill_index[]={
	                              OPV08_10S,CLSV01,CLSV03,CLSV04,CLSV05,CLSV06,CLSV07,ZSB_T0_00S,OPV07,ZSB_T_5ML,CLSV07,
	                              ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV08,CLSV01,ZSB_T_5ML,
	                              0x88,OPV08_5S,CLSV08,STOP_MOTO,ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,0xff,
                                     };	                          
//===============================================================================
uint8_t const TB_Sample_fill_index[]={
	                              OPV08_10S,CLSV01,CLSV03,CLSV04,CLSV05,CLSV06,CLSV07,ZSB_T0_00S,OPV02,OPV03,
	                              ZSB_T_5ML,CLSV03,ZSB_T0_00S,OPV03,ZSB_T_5ML,CLSV02,CLSV03,ZSB_T0_00S,OPV01,ZSB_T_9ML,
	                              CLSV01,ZSB_T0_00S,OPV01,ZSB_T_9ML,0x88,OPV08_5S,CLSV08,STOP_MOTO,CLSV01,ZSB_T0_00S,
	                              OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,0xff,
                                     };	                          
//===============================================================================
uint8_t const TB_Clear_finction_index[]={
	                              OPV08_10S,CLSV01,CLSV03,CLSV04,CLSV05,CLSV06,CLSV07,0x88,CLSV08,OPV12,
	                              0x88,ZSB_T_9ML,0x88,CLSV12,OPV02,OPV03,ZSB_T0_00S,CLSV02,CLSV03,0x88,
                                      ZSB_T_9ML,0x88,OPV02,OPV03,ZSB_T0_05S,OPV12,CLSV02,0x88,0x88,ZSB_T_9ML,
                                      0x88,ZSB_T0_00S,0x88,ZSB_T_9ML,0x88,ZSB_T0_00S,0x88,STOP_MOTO,OPV08_10S,0x88,
                                      OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,OPV08_10S,CLSV08,
                                      OPV01,ZSB_T_7ML,CLSV01,ZSB_T0_00S,OPV01,ZSB_T_7ML,CLSV01,0xff,                                      
                                     };	                          
//===============================================================================
uint8_t const TB_System_fill_index[]={
	                              OPV08_5S,CLSV01,CLSV03,CLSV04,CLSV05,CLSV06,CLSV07,ZSB_T0_00S,OPV03,ZSB_T_5ML,
	                              CLSV03,ZSB_T0_00S,OPV03,ZSB_T_5ML,CLSV03,ZSB_T0_00S,OPV04,ZSB_T_5ML,CLSV04,ZSB_T0_00S,
	                              0x88,ZSB_T_5ML,CLSV05,ZSB_T0_00S,0x88,ZSB_T_5ML,CLSV06,ZSB_T0_00S,OPV01,ZSB_T_9ML,
	                              CLSV01,ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV01,CLSV08,ZSB_T_5ML,0x88,OPV08_5S,CLSV08,
                                      STOP_MOTO,ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,0xff,
                                     };	                          
//===============================================================================
uint8_t const TB_Standar1_fill_index[]={
	                              OPV08_5S,CLSV01,CLSV03,CLSV04,CLSV05,CLSV06,CLSV07,ZSB_T0_00S,OPV03,ZSB_T_5ML,
	                              CLSV03,ZSB_T0_00S,OPV03,ZSB_T_5ML,CLSV03,ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,
	                              OPV01,ZSB_T_9ML,CLSV01,CLSV08,ZSB_T_5ML,0x88,OPV08_5S,CLSV08,STOP_MOTO,ZSB_T0_00S,
	                              OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,0xff	                              
                                     };	                          
//===============================================================================
uint8_t const TB_Standar2_fill_index[]={
	                              OPV08_10S,CLSV01,CLSV03,CLSV04,CLSV05,CLSV06,CLSV07,ZSB_T0_00S,OPV05,ZSB_T_5ML,
	                              CLSV05,ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV01,CLSV08,
	                              ZSB_T_5ML,0x88,OPV08_5S,CLSV08,STOP_MOTO,ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,0xff,	                              
                                     };	                          
//===============================================================================
uint8_t const TB_Sample_adding_index[]={
	                              OPV02,OPV03,0x88,CLSV02,CLSV03,ZSB_T0_00S,0x88,
	                              OPV02,OPV03,0x88,CLSV02,CLSV03,ZSB_T0_00S,0xff,                            
                                     };	                          
//===============================================================================
uint8_t const TB_Sample_washing_index[]={
	                              OPV02,OPV03,0x88,CLSV02,CLSV03,ZSB_T0_00S,0x88,OPV02,OPV03,0x88,
	                              CLSV02,CLSV03,ZSB_T0_00S,OPV08_10S,CLSV08,STOP_MOTO,OPV02,OPV03,0x88,CLSV02, 
	                              CLSV03,ZSB_T0_00S,0x88,OPV02,OPV03,0x88,CLSV02,CLSV03,ZSB_T0_00S,OPV08_10S,
	                              CLSV08,STOP_MOTO,0xff,CLSV02,CLSV03,ZSB_T0_00S,OPV02,OPV03,0x88,                            
                                     };	                          
//===============================================================================
//uint8_t const TB_paikong_index[]={
//	                              OPV08_10S,CLSV01,CLSV03,CLSV04,ZSB_T0_05S,CLSV08,OPV01,ZSB_T_7ML,CLSV01,ZSB_T0_00S,
//	                              OPV01,ZSB_T_7ML,CLSV01,ZSB_T0_05S,OPV08_10S,CLSV08,OPV01,ZSB_T_7ML,CLSV01,
//	                              ZSB_T0_00S,OPV01,ZSB_T_7ML,CLSV01,ZSB_T0_05S,OPV08_10S,CLSV08,OPV01,ZSB_T_7ML,CLSV01,
//                                      ZSB_T0_00S,OPV01,ZSB_T_7ML,CLSV01,ZSB_T0_00S,STOP_MOTO,0xff,
//                                     };	 

//----2015.05.19
uint8_t const TB_paikong_index[]={
	                              OPV08_10S,CLSV01,CLSV03,CLSV04,ZSB_T0_05S,CLSV08,OPV01,0x88,CLSV01,ZSB_T0_00S,
	                              OPV01,0x88,CLSV01,ZSB_T0_05S,OPV08_10S,CLSV08,OPV01,0x88,CLSV01,
	                              ZSB_T0_00S,OPV01,0x88,CLSV01,ZSB_T0_05S,OPV08_10S,CLSV08,OPV01,0x88,CLSV01,
                                      ZSB_T0_00S,OPV01,0x88,CLSV01,ZSB_T0_00S,STOP_MOTO,0xff,
                                     };	                          
                                                              
//===============================================================================
uint8_t const TB_BY1_addtion_index[]={
	                              OPV03,0x88,CLSV03,ZSB_T0_00S,STOP_MOTO,0x88,0x88,OPV03,0x88,CLSV03,ZSB_T0_00S,0xff,
                                     };	                          
//===============================================================================
uint8_t const TB_BY1_wash_index[]={
	                              OPV03,0x88,CLSV03,ZSB_T0_00S,0x88,OPV03,0x88,CLSV03,ZSB_T0_00S,OPV08_10S,CLSV08,
	                              OPV03,0x88,CLSV03,ZSB_T0_00S,OPV03,0x88,CLSV03,ZSB_T0_00S,
	                              OPV08_10S,CLSV08,STOP_MOTO,0xff,
                                     };	                          

///uint8_t const TB_BY1_wash_index[]={
///	                              OPV03,ZSB_T_7ML,CLSV03,ZSB_T0_00S,STOP_MOTO,0x88,0x88,OPV03,ZSB_T_7ML,CLSV03,ZSB_T0_00S,
///	                              OPV08_10S,CLSV08,STOP_MOTO,0xff,
///                                     };	                          
//===============================================================================
////uint8_t const TB_ZSB_initial_index[]={
////	                              OPV08_5S,CLSV01,CLSV03,CLSV04,0x88,ZSB_T0_05S,CLSV08,0xff,
////                                     };	                          
//===============================================================================
uint8_t const TB_xishi_index[]={
	                              OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,0x88,OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_05S,OPV08_10S,
	                              CLSV08,OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_05S,OPV08_10S,
	                              CLSV08,OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_00S,OPV01,ZSB_T_9ML,CLSV01,ZSB_T0_05S,OPV08_10S,
	                              CLSV08,OPV02,OPV03,0x88,CLSV02,CLSV03,ZSB_T0_01S,OPV01,0x88,CLSV01,
	                              ZSB_T0_00S,OPV01,ZSB_T_5ML,CLSV01,ZSB_T0_00S,0xff,CLSV02,CLSV03,ZSB_T0_00S,OPV02,OPV03,0x88,	                              
                                     };	 	                         

uint8_t const TB_pub_finc[]={//div,addsh,addsl,datah,datal,wait_h,wait_l,
	                    0xd1,0x11,0x00,0x00,0x01,0x00,0x00,			//0    open V1
	                    0xd1,0x11,0x00,0x00,0x00,0x00,0x00,	//		//1    close V1
	                    0xd1,0x11,0x01,0x00,0x01,0x00,0x00,	//		//2    open V2
	                    0xd1,0x11,0x01,0x00,0x00,0x00,0x00,	//		//3    close V2
	                    0xd1,0x11,0x02,0x00,0x01,0x00,0x00,	//		//4    open V3
	                    0xd1,0x11,0x02,0x00,0x00,0x00,0x00,			//5    close V3
	                    0xd1,0x11,0x03,0x00,0x01,0x00,0x00,			//6    open V4
	                    0xd1,0x11,0x03,0x00,0x00,0x00,0x00,			//7    close V4
	                    0xd1,0x11,0x04,0x00,0x01,0x00,0x00,			//8    open V5
	                    0xd1,0x11,0x04,0x00,0x00,0x00,0x00,			//9    close V5
	                    0xd1,0x11,0x05,0x00,0x01,0x00,0x00,			//10   open V6
	                    0xd1,0x11,0x05,0x00,0x00,0x00,0x00,			//11   close V6
	                    0xd1,0x11,0x06,0x00,0x01,0x00,0x00,			//12   open V7
	                    0xd1,0x11,0x06,0x00,0x00,0x00,0x00,			//13   close V7
	                    0xd1,0x11,0x07,0x00,0x01,0x00,0x00,			//14   open V8
	                    0xd1,0x11,0x07,0x00,0x00,0x00,0x00,			//15   close V8
	                    0xd1,0x11,0x07,0x00,0x01,0x01,0xf4,			//16   open V8 5sec	
	                    0xd1,0x11,0x07,0x00,0x01,0x03,0xe8,			//17   open V8 10sec	
	                    0xd1,0x13,0x01,0x00,0x00,0x00,0x00,			//18   Set_ZSB_move_to 0                 
	                    0xd1,0x13,0x01,0x00,0x00,0x00,0x64,			//19   Set_ZSB_move_to 0 1sec                 
	                    0xd1,0x13,0x01,0x00,0x00,0x01,0xf4,			//20   Set_ZSB_move_to 0 5sec                
	                    0xd1,0x13,0x01,0x17,0x70,0x00,0x00,			//21   Set_ZSB_move_to 6000 5ml                               
	                    0xd1,0x13,0x01,0x2a,0x31,0x00,0x00,			//22   Set_ZSB_move_to 9ml
	                    U_SENCER_MODU_ADDS,0x13,0x00,0x00,0x00,0x00,0x00,			//23   stop moto

	                    0xd1,0x11,0x08,0x00,0x01,0x00,0x00,			//24    open V9
	                    0xd1,0x11,0x08,0x00,0x00,0x00,0x00,			//25    close V9
	                    0xd1,0x11,0x09,0x00,0x01,0x00,0x00,			//26    open V10
	                    0xd1,0x11,0x09,0x00,0x00,0x00,0x00,			//27    close V10
	                    0xd1,0x11,0x0a,0x00,0x01,0x00,0x00,			//28    open V11
	                    0xd1,0x11,0x0a,0x00,0x00,0x00,0x00,			//29    close V11	                                                  
	                    0xd1,0x11,0x0b,0x00,0x01,0x00,0x00,			//30    open V12
	                    0xd1,0x11,0x0b,0x00,0x00,0x00,0x00,			//31    close V12
	                    0xd1,0x11,0x0c,0x00,0x01,0x00,0x00,			//32    open V13
	                    0xd1,0x11,0x0c,0x00,0x00,0x00,0x00,			//33    close V13
	                    0xd1,0x13,0x01,0x20,U_SENCER_MODU_ADDS,0x00,0x00,			//34   Set_ZSB_move_to 7ml
	                    0xd1,0x13,0x01,0x00,0x00,0x00,0x00,			//35   Set_ZSB_move_to 0 20sec   
                          };
          
#else
extern volatile uint8_t  Program_reset_initial_flag;
extern volatile uint32_t Program_reset_initial_cnt; 
extern volatile uint8_t ZB_flag;					//zb

extern volatile uint8_t Wait_clock_flag;

extern volatile uint8_t Reget_slope;
extern volatile uint8_t U2_disple_flag;

extern volatile uint8_t 	Prog_total_step;
extern volatile uint8_t 	Prog_step_disp_cnt;

extern volatile uint8_t	program_menu_select;

extern uint8_t	buff_app[4];
extern uint8_t	buff_bool[4];
extern uint8_t  Wait_data_buff[13];

extern volatile uint8_t	Auto_check_clock_flag;

extern volatile float	df_temp;

extern float	P_Tcall_temp;		//2012-9-13 11:11
extern float	c1_prog_use;
extern float	c2_prog_use;
extern float	c3_prog_use;
extern float	c5_temp;
extern float	k1_prog_use;
extern float	j_prog_use;
//extern float	c5_prog_use;
extern float	c2_temp;
extern float	k2_prog_use;
extern uint8_t	k2_signe_flag;

extern volatile uint8_t		yes_not;
extern volatile uint8_t		result_signe;
extern volatile uint32_t	Pub_result_temp;

extern uint8_t		M_lever4_FXCS_cnt;
extern uint8_t		Pro_power_on_flag;
extern uint8_t 	Pro_step_status;
extern volatile uint8_t 	Pro_wait_time_flag;
extern volatile uint32_t	Pro_wait_time_set;
extern uint8_t 	Pro_wait_time0_flag;
extern uint32_t	Pro_wait_time0_set;

extern uint8_t		Pro_auto_time_up_check_flag;
extern uint32_t	Pro_auto_time_dcnt;
extern uint16_t	Pro_auto_qingxi_freq_cnt;
extern uint16_t	Pro_auto_jiaozheng_freq_cnt;
extern uint16_t	Pro_auto_hecha_freq_cnt;

extern uint32_t	Measurement_signal_11;
extern uint32_t	Measurement_signal_12;
extern uint32_t	Measurement_signal_21;
extern uint32_t	Measurement_signal_22;

extern uint32_t	e1_men_save;		//2012-8-20 14:47
extern uint32_t	e2_men_save;

extern volatile uint32_t	E_ADC_temperature1;
extern volatile uint32_t	E_ADC_temperature2;

extern uint32_t	K1_JZYZ;
extern uint8_t	E_wait_flag;
extern volatile uint32_t	E_wait_time_set;
extern volatile uint32_t	E_wait_time_cnt;

extern volatile uint8_t  	E_step_cnt;
extern volatile uint32_t	E_ADC_emperature;
extern volatile uint8_t		Prog_disp_flag;
        
extern volatile uint8_t		Memu1_auto_step_status;
extern volatile uint8_t  	Memu1_hand_step_status;
extern volatile uint8_t  	Memu1_jiaozheng_step_status;
extern volatile uint8_t  	Memu1_biaoye_step_status;
extern volatile uint8_t  	Memu1_xishi_step_status;


extern void Set_JB_direction(void);
extern void Menu1_auto_fenxi_finc(void);
extern void Menu1_hand_fenxi_finc(void);
extern void Menu1_hand_jiaozheng_finc(void);
extern void Menu1_biaoye_hecha_finc(void);
extern void Menu1_xishi_fenxi_finc(float c1_c2_tp);
extern void Menu3_program_all_finc(void);
extern void Set_ZSB_move_to(uint32_t data);
extern void Set_switch_open_close(uint8_t sw,uint8_t status);

extern void Program_wait_time(void);
extern void Triggle_get_sample(void);
extern void Program_power_on_initial(void);
extern void BY1_wash(void);

extern void Hand_opration_finction(void);
extern void System_fill(void);
extern void Standar1_fill(void);
extern void Standar2_fill(void);
extern void Reagent1_fill(void);
extern void Reagent2_fill(void);
extern void Reagent3_fill(void);
extern void Reagent4_fill(void);
extern void Sample_fill(void);

extern uint32_t Float_to_int_data(float data);
extern float Int_to_float_data(uint32_t data);
extern float Correcting_fintion(float x_temp);
extern uint32_t Float_to_int_reg(float data);
extern uint32_t Float_to_int_reg_ee(float data);
extern float DJFFXND_calculat(void);
extern void Test_signal_wait_time(void);
extern void Get_test_signal_11(void);
extern void Get_test_signal_12(void);
extern void Get_test_signal_21(void);
extern void Get_test_signal_22(void);
extern void Program_reset_initial(void);
extern void Menu1_auto_fenxi_time_down_cnt(void);
extern void Set_JB_speed(void);
extern void BY2_addtion(void);
extern float Get_df_data(float c1_c2);
extern void Add_xishi_sample(float c1_c2_tp);

extern float Int_to_float_reg(uint32_t data);
extern void Power_on_check_device(void);

extern void Disp_real_time_vol_data(uint8_t select,uint8_t finc_sl,uint32_t disp_d);
extern float Get_Prj_correlat(void);
extern uint32_t Get_e_wait_time(void);
extern void Test_signal_data_nc(uint8_t select,uint8_t finc_sl);
extern void Program_wait_time_nc(void);

extern void All_program_menu_disp(uint8_t selest);

extern void Read_last_result(uint32_t adds,uint8_t select,uint8_t data_select);

extern uint8_t Get_step_num_each_finc(uint8_t selset);
extern uint32_t Float_to_int_reg_ee2(float data);

extern void Extern_port_triggle_ctrl(void);

#endif


