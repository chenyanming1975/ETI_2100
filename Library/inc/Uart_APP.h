//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Uart_APP.h
//	First EDIT		: Sam
//	Data			: 2012.06.08
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================
#define CLEAR_ADDR_1	0xD3
#define CLEAR_ADDR_2	0x06



#define		TEST_OBJ_TYPE				0x0060

#define		U_SENCER_MODU_ADDS			0xD0
#define		U_DRIVER_MODU_ADDS			0xD1
//other adds from 0xd2~0xdf

//-----------------------------------------------------------------
#define		U_REC_TIME_OUT				10		//set 10sec for wait slaver return
#define		U_CRC_GEN				0XA001		//for CRC
                                                	
#define		U_CMD_READ_REG_DATA			0X03
#define		U_CMD_WRITE_REG_DATA			0X06
#define		U_CMD_WRITE_MORE_REG_DATA		0X10
                                                	
#define		U_ERR_CMD_READ_REG_DATA			0X83
#define		U_ERR_CMD_WRITE_REG_DATA		0X86
#define		U_ERR_CMD_WRITE_MORE_REG_DATA		0X90
//-----------d1                                                	
#define		U_DRIVER_READ_BOOT_ADDS			0X1000	//0X0000~0XFFFF 0X1204= DATE
#define		U_DRIVER_READ_APP_ADDS			0X1001	//0X0000~0XFFFF 0X1204= DATE
#define		U_DRIVER_READ_MOTO_STATUS_ADDS		0X1002	//15 MOTO WORK STATUS	0X00:WRI  0XAA:ERR
#define		U_DRIVER_READ_INPUT_VOL_ADDS		0X1003	//0:NOR 1:LOW 2:HIGHT
#define		U_DRIVER_FA1_WO_ADDS			0X1100	//0:CONNET  1:OPEN
#define		U_DRIVER_FA2_WO_ADDS			0X1101	//0:CONNET  1:OPEN
#define		U_DRIVER_FA3_WO_ADDS			0X1102	//0:CONNET  1:OPEN
#define		U_DRIVER_FA4_WO_ADDS			0X1103	//0:CONNET  1:OPEN
#define		U_DRIVER_FA5_WO_ADDS			0X1104	//0:CONNET  1:OPEN
#define		U_DRIVER_FA6_WO_ADDS			0X1105	//0:CONNET  1:OPEN
#define		U_DRIVER_FA7_WO_ADDS			0X1106	//0:CONNET  1:OPEN
#define		U_DRIVER_FA8_WO_ADDS			0X1107	//0:CONNET  1:OPEN
#define		U_DRIVER_FA9_WO_ADDS			0X1108	//0:CONNET  1:OPEN
#define		U_DRIVER_FA10_WO_ADDS			0X1109	//0:CONNET  1:OPEN
#define		U_DRIVER_FA11_WO_ADDS			0X110A	//0:CONNET  1:OPEN
#define		U_DRIVER_FA12_WO_ADDS			0X110B	//0:CONNET  1:OPEN
#define		U_DRIVER_FA13_WO_ADDS			0X110C	//0:CONNET  1:OPEN
#define		U_DRIVER_FA14_WO_ADDS			0X110D	//0:CONNET  1:OPEN	
#define		U_DRIVER_FA_ALL_WO_ADDS			0X11A0	//0:CONNET  1:OPEN
#define		U_DRIVER_PORT1_IN_RO_ADDS		0X1200	//0:CONNET  1:OPEN	
#define		U_DRIVER_PORT2_IN_RO_ADDS		0X1201	//0:CONNET  1:OPEN
#define		U_DRIVER_PORT3_IN_RO_ADDS		0X1202	//0:CONNET  1:OPEN
#define		U_DRIVER_PORT4_IN_RO_ADDS		0X1203	//0:CONNET  1:OPEN
#define		U_DRIVER_PORT5_IN_RO_ADDS		0X1204	//0:CONNET  1:OPEN
#define		U_DRIVER_PORT6_IN_RO_ADDS		0X1205	//0:CONNET  1:OPEN
#define		U_DRIVER_PORT7_IN_RO_ADDS		0X1206	//0:CONNET  1:OPEN
#define		U_DRIVER_PORT8_IN_RO_ADDS		0X1207	//0:CONNET  1:OPEN
#define		U_DRIVER_PORT_ALL_IN_ARO_DDS		0X12A0  //0:CONNET  1:OPEN
#define		U_DRIVER_MOTO_SPEED_WO_ADDS		0X1300	
#define		U_DRIVER_MOTO_POSITION_WO_ADDS		0X1301	
#define		U_DRIVER_MOTO_RST_REG_CTRL_WO_ADDS		0X1302	
#define		U_DRIVER_SELF_PROG_RO_ADDS		0X2000	
#define		U_DRIVER_SELF_PROG_DATA_CNT_RO_ADDS	0X2001	
#define		U_DRIVER_SELF_REG_CTRL_WO_ADDS		0X2002	
#define		U_DRIVER_SELF_PROG_DATA_WO_ADDS		0X2100	//0X2100~0X21C0 TOTLE 192 ADDS,64BYTE/1ADDS	
//--------d0
#define		U_SENSER_READ_BOOT_ADDS			0X1000	//0X0000~0XFFFF 0X1204= DATE
#define		U_SENSER_READ_APP_ADDS			0X1001	//0X0000~0XFFFF 0X1204= DATE
#define		U_SENSER_ADC_OFFSET_ADDS		0X1100	//
#define		U_SENSER_ADC_PHOTO_ADDS			0X1101	//
#define		U_SENSER_ADC_OFFSET_PHOTO_ADDS		0X1102	//
#define		U_SENSER_ADC_ISE_ADDS			0X1103	//
#define		U_SENSER_ADC_TEMP_ADDS			0X1104	//比色法用
#define		U_SENSER_ADC_TEMP1_ADDS			0X1105	//电极法用

#define		U_SENSER_OFFSET_PWM_ADDS		0X1200	//

#ifdef 	__NEWPCB__
#define		U_SENSER_MOTOR_SPEED_ADDS		0X1201	//0X1202	2012-10-24 16:23
#define		U_SENSER_LED_CURRIT_CTRL_ADDS		0X1202	//0X1203	2012-10-24 16:23
#else
#define		U_SENSER_TEMP_PWM_ADDS			0X1201	//
#define		U_SENSER_MOTOR_SPEED_ADDS		0X1202	//0X1202	2012-10-24 16:23
#define		U_SENSER_LED_CURRIT_CTRL_ADDS		0X1203	//0X1203	2012-10-24 16:23
#endif

#define		U_SENSER_MOTOR_RUN_ADDS			0X1300	
#define		U_SENSER_LED_LINGHT_CTRL_ADDS		0X1301	
#define		U_SENSER_ISE_CTRL_ADDS			0X1302	
#define		U_SENSER_NTC_CTRL_ADDS			0X1303
#define		U_SENSER_NTC_CTRL_WATER_ADDS		0X1304
#define		U_SENSER_SELF_PROG_RO_ADDS		0X2000	
#define		U_SENSER_SELF_PROG_DATA_CNT_RO_ADDS	0X2001	
#define		U_SENSER_SELF_REG_CTRL_WO_ADDS		0X2002	
#define		U_SENSER_SELF_PROG_DATA_WO_ADDS		0X2100	//0X2100~0X21FF 
	
#define		U_SENSER_MOTOR_N			0X00A5	
#define		U_SENSER_MOTOR_P			0X005A		
#define		U_SENSER_MOTOR_STOP			0X0000		

#ifdef	__UART_AP__

volatile int8_t Rtc_int_flag;
volatile int8_t Rtc_int_cnt;

#ifdef __UART_TEST__
volatile uint8_t		U2_P_mouth;
volatile uint8_t		U2_P_data;
volatile uint8_t		U2_P_hour;
volatile uint8_t		U2_P_minu;
#endif 

#define PUTCHAR_PROTOTYPE unsigned short fputc_new(unsigned short ch, FILE *f)		//2012.10.19    add sam for printf

#else
extern volatile uint8_t		Rx_TX_flag;		//2012-7-19 9:14

extern volatile uint8_t 	U_slave_add_temp;
extern volatile uint8_t 	U_cmd_temp;
extern volatile uint8_t 	U_save_data_lenth;
extern volatile uint16_t 	U_read_write_data_lenth;
extern volatile uint16_t	U_start_adds_temp;

extern uint8_t		U_data_buff[256];
extern volatile uint8_t		U_RX_TX_byte_lenth;		//for caculat the data lenth
extern volatile uint8_t		U_RX_TX_byte_lenth_cnt;		//for caculat the data lenth
extern volatile uint16_t	U_Rx_wait_time_flag_temp;
extern volatile uint8_t		Uart_tx_rx_fifo_int_FLAG;

extern volatile uint8_t		Uart2_BAUD_select_temp;
extern volatile uint8_t         Uart2_JOJY_select_temp;
extern volatile uint8_t         Uart2_STOP_select_temp;

extern void Init_UART012(void);
extern void Com1_rd_ctrl(uint8_t RD1_01);
extern void Com2_rd_ctrl(uint8_t RD2_01);
extern void UART0_IRQHandler_APP(uint32_t u32IntStatus);
extern void UART1_IRQHandler_APP(uint32_t u32IntStatus);
extern void UART2_IRQHandler_APP(uint32_t u32IntStatus);
extern void En_Uart012_THRE_int(uint32_t uPort);
extern void Uart012_FIFO_INT_CTRL(void);

extern uint16_t CRC16(uint8_t *_data,uint8_t length);
extern void Set_tx_data_and_start_tx(void);
extern void Wait_slave_return_time(void);
extern void Set_tx_cmd_data_finc(int8_t slave_adds,int8_t cmd,int16_t start_adds,int16_t data_lenth,int8_t save_data_lenth);

extern void Uart2_start_tx(void);
extern void Uart2_rx_time_cnt(void);

extern volatile int8_t Rtc_int_flag;
extern volatile int8_t Rtc_int_cnt;

#ifdef __UART_TEST__
extern volatile uint8_t		U2_P_mouth;
extern volatile uint8_t		U2_P_data;
extern volatile uint8_t		U2_P_hour;
extern volatile uint8_t		U2_P_minu;

extern void Get_Test_potocal_data(uint8_t select);
extern void Get_Test_potocal_loop_time(void);
extern void Run_potocal_test(void);
#endif  	

//--------------------------------------------------
extern volatile uint8_t		U2_rx_cnt;
extern volatile uint8_t		U2_rx_Flag;
extern volatile uint8_t		U2_TX_byte_lenth;
extern uint8_t		U2_re_table[256];

extern volatile uint8_t		Test_potocal_time_flag;
extern volatile uint8_t		Test_potocal_time_cnt;
extern volatile uint16_t	Test_potocal_time_mini_cnt;

extern float    jisuan_k_temp;
extern void tx_jisuan_data(uint8_t fram);


extern Reset_init_UART1(int8_t select);
#endif










