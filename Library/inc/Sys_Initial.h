//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Sys_Initial.h
//	First EDIT		: Sam
//	Data			: 2012.06.06
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================

extern uint8_t	system_init_flag;
extern uint32_t	Pro_auto_time_dcnt_bak;

extern void System_clk_initial(void);
extern void GPIO_initial(void);
extern void WDT_initial(void);
extern void WDT_IRQHandler(void);
extern void System_reg_initial(void);
extern uint32_t Get_buff_32bit_data(uint8_t adds);
extern void Buff_get_reg_32bit_data(uint8_t adds,uint32_t data);
extern void Set_system_reg_to_buff(void);
extern void Write_system_reg_to_chip(void);
extern void Reset_First_write_reg(void);
extern void Write_adds_reg_to_chip(void);
extern void Read_first_reg_data(void);
extern void WDT_CLR(void);
