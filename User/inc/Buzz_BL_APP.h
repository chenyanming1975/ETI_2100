///============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Buzz_BL_APP.h
//	First EDIT		: Sam
//	Data			: 2012.06.18
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================


//-------------------------------------------------------------------------------------------
#ifdef		__BUZZ_BL_APP__

#else
extern uint8_t   Buzz_en_flag;

extern uint32_t Back_light_time_cnt;
extern uint32_t Back_light_time;
extern uint8_t Back_light_time_flag;

extern uint8_t LCD_bl_ctrl_data;
extern uint8_t LCD_bl_duty_cnt;

extern void Set_back_light_up(void);
extern void Set_back_light_down(void);
//extern void Back_light_ctrl_finc(void);

extern void Set_en_buzz_finc(uint8_t B_repeat_set,uint8_t B_freq_set,uint32_t B_time_set_op,uint32_t B_time_set_cls);
extern void Buzz_ctrl_finc(void);
extern void Back_light_time_cnter(void);
extern void Set_back_light_10sec(void);

#endif
