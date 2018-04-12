//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Timer_isr.c
//	First EDIT		: Sam
//	Data			: 2012.06.07
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================
#define		__TIMER_ISR_F__
//--------------------------------------------------------------------------------------------
#include "Incude_all.h"
//-------------------Pub Variable ------------------------------------------------------------
//static uint8_t	timer_test;

//===============================================================================
//	finction	:DrvTIMER_Init
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.06
//	modefy		:null
//      User must to call this function before any timer operations after system boot up.
//===============================================================================
void DrvTIMER_Init(void)
{
    bIsTimer0Initial    = TRUE;
    bIsTimer1Initial    = TRUE;
    bIsTimer2Initial    = TRUE;
    bIsTimer3Initial    = TRUE;
}

//===============================================================================
//	finction	:Timer0 initial
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.06
//	modefy		:null
//===============================================================================
void Timer0_initial(void)
{
    /* Step 1. Enable and Select Timer clock source */
    SYSCLK->CLKSEL1.TMR0_S = 0;	//0:Select 12Mhz for Timer0 clock source 4:Select internal 22Mhz
    SYSCLK->APBCLK.TMR0_EN =1;	//Enable Timer0 clock source

    /* Step 2. Select Operation mode */
    TIMER0->TCSR.MODE=1;		//Select periodic mode for operation mode

    /* Step 3. Select Time out period = (Period of timer clock input) * (8-bit Prescale + 1) * (24-bit TCMP)*/
    TIMER0->TCSR.PRESCALE=128;	// Set Prescale [0~255]			//use 22M set to 128 and 6 is 35.2us
    TIMER0->TCMPR = 7;		// 7:14.375M Set 8kHz			//use 22M set to 6 is 70us


    /* Step 4. Enable interrupt */
    TIMER0->TCSR.IE = 1;
    TIMER0->TISR.TIF = 1;		//Write 1 to clear for safty
    NVIC_EnableIRQ(TMR0_IRQn);	//Enable Timer0 Interrupt

    /* Step 5. Enable Timer module */
    TIMER0->TCSR.CRST = 1;		//Reset up counter
    TIMER0->TCSR.CEN = 1;		//Enable Timer0

//  	TIMER0->TCSR.TDR_EN=1;		// Enable TDR function
}

//===============================================================================
//	finction	:Timer1 initial
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.06
//	modefy		:null
//===============================================================================
void Timer1_initial(void)
{
    /* Step 1. Enable and Select Timer clock source */
    SYSCLK->CLKSEL1.TMR1_S = 0;	//Select 12Mhz for Timer1 clock source
    SYSCLK->APBCLK.TMR1_EN =1;	//Enable Timer1 clock source

    /* Step 2. Select Operation mode */
    TIMER1->TCSR.MODE=1;		//Select periodic mode for operation mode

    /* Step 3. Select Time out period = (Period of timer clock input) * (8-bit Prescale + 1) * (24-bit TCMP)*/
    TIMER1->TCSR.PRESCALE=255;	// Set Prescale [0~255]
//	TIMER1->TCMPR = 58;		// Set 1kHz 58:use 14.375M  47:use 12M 88:use 22M
    TIMER1->TCMPR = 580;		// Set 100Hz 58:use 14.375M  47:use 12M 88:use 22M


    /* Step 4. Enable interrupt */
    TIMER1->TCSR.IE = 1;
    TIMER1->TISR.TIF = 1;		//Write 1 to clear for safty
    NVIC_EnableIRQ(TMR1_IRQn);	//Enable Timer1 Interrupt

    /* Step 5. Enable Timer module */
    TIMER1->TCSR.CRST = 1;		//Reset up counter
    TIMER1->TCSR.CEN = 1;		//Enable Timer1

//  	TIMER1->TCSR.TDR_EN=1;		// Enable TDR function
}

//============================================================================================
// Function:        TMR0_IRQHandler
//
// Parameters:
//                  None
// Returns:
//                  None
// Description:
//                  The TIMER0 default IRQ, declared in startup_NUC1xx.s
//============================================================================================
void TMR0_IRQHandler(void)
{
    if ((TIMER0->TCSR.IE == 1) && (TIMER0->TISR.TIF == 1))
        TIMER0->TISR.TIF = 1;

    if ((TIMER0->TEXCON.TEXEN == 1) && (TIMER0->TEXCON.TEXIEN == 1) && (TIMER0->TEXISR.TEXIF == 1))
        TIMER0->TEXISR.TEXIF = 1;

//    Back_light_ctrl_finc();
    Buzz_ctrl_finc();  			//蜂鸣器控制函数


//-----------------------------\for test clock
//    if(timer_test==0)
//    {
//    	DrvGPIO_SetBit(E_GPD,6);
//    	timer_test=1;
//    }
//    else
//    {
//    	DrvGPIO_ClrBit(E_GPD,6);
//    	timer_test=0;
//    }
}

//============================================================================================
// Function:        TMR1_IRQHandler
//
// Parameters:
//                  None
// Returns:
//                  None
// Description:
//                  The TIMER1 default IRQ, declared in startup_NUC1xx.s
//============================================================================================
void TMR1_IRQHandler(void) // Timer1 interrupt subroutine
{
    TIMER1->TISR.TIF =1;
//    TimerCounter1+=1;

//    if(Rtc_int_flag==0x01)
//    {
//    	Rtc_int_cnt=0;
//    	Rtc_int_flag=0x02;
//    }
//    else if(Rtc_int_flag==0x02)
//    {
//    	++Rtc_int_cnt;
//    	if(Rtc_int_cnt>=0xc8)
//    	{
//    	   Rtc_int_flag=0x03;
//    	}
//    }
    ++wd_use_ramdom;

    Key_scan_bounce_time(); 		//按键去抖动计时
    Wait_slave_return_time();		//通讯超时计时
    Back_light_time_cnter();		//LCD背光计时
    Program_wait_time();		//分析流程公用计时
    Program_wait_time_nc();		//分析流程公用计时
    Test_signal_wait_time();		//分析流程读电压值公用计时
    Menu1_auto_fenxi_time_down_cnt();	//自动分析间隔时间计时
    Uart2_rx_time_cnt();		//串口2计时
#ifdef __UART_TEST__
    Get_Test_potocal_loop_time();	//北京测试通信协议计时
#endif

}

