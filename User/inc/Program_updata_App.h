//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Program_up_data.h
//	First EDIT		: Sam
//	Data			: 2012-9-10 11:30
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================

//--------------------------------------------------------------------------------------------
#define 	P_ERASE				0X00A5
#define 	P_WRITE				0X005A
//-------program status read adds 0x2000	when aoto program finish it return 0x0000
#define 	P_TX_FINISH			BIT0
#define 	P_FLASH_ERASE_FINISH		BIT1
#define 	P_FLASH_READ_FINISH		BIT2		//when finish bit0,1=0; when bit0,1=1 write.
#define 	P_FLASH_CHECK_OK		BIT3

#define 	SD_SW_PIN_H			0X00000001	//2012-9-14 9:54

//--------------------------------------------------------------------------------------------
#ifdef	__PRO_UP_DATA__

#else
extern uint8_t	Aoto_program_flag;		//选择更新驱动板还是传感器板
extern uint8_t  P_status_temp;			//save status BIT7 for flag 
extern uint8_t  p_step_cnt;			//auto program step cnt
extern uint8_t  P_cmd_adds;			//192 个包，每个包73字节
extern uint8_t  p_packge_tx_flag;

extern uint32_t P_sch_adds_temp;

extern void Aoto_program_ctrl(void);
extern void Get_bin_data(void);
extern void Disp_updata_status(uint8_t finc);
#endif


