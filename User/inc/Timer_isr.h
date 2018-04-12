//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Timer_isr.h
//	First EDIT		: Sam
//	Data			: 2012.06.06
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================

#ifdef	__TIMER_ISR_F__
int32_t volatile bIsTimer0Initial     = FALSE,
                 bIsTimer1Initial     = FALSE,
                 bIsTimer2Initial     = FALSE,
                 bIsTimer3Initial     = FALSE;

#else
extern void DrvTIMER_Init(void);
extern void Timer0_initial(void);
extern void Timer1_initial(void);

#endif

