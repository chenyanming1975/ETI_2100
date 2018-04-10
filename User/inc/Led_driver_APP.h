//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Led_driver_APP.c
//	First EDIT		: Sam
//	Data			: 2012.06.12
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================

#ifdef		__LED_APP__

#else
extern uint8_t  Led_port_status;
extern uint8_t	Led_data_flag;

extern void Led_disp_data(uint8_t Data);
extern void Led_status_show(void);

#endif
