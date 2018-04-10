//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Inlude_all.h
//	First EDIT		: Sam
//	Data			: 2012.06.06
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================
#define	Uint8 unsigned int
#define	uchar unsigned char


//#define __DEBUG__
#define __NEWPCB__
//#define	__UART_TEST__
//------------------------------------------------------
#include <stdio.h>	
#include <string.h>																										 
#include "NUC1xx.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"
#include "DrvUART.h"
#include "DrvSPI.h"
#include "DrvI2C.h"
#include "DrvPWM.h"
#include "DrvFMC.h"
#include <MATH.H>

//#include "stdlib.h"		//malloc
//#include "INTRINS.h"
//#include "memory.h"
//#include "message.h"

#include "Timer_isr.h"
#include "Uart_APP.h"
#include "Lcd_disp_APP.h"
#include "Led_driver_APP.h"
#include "Key_scan_APP.h"
#include "Spi_flash_APP.h"
#include "RTC_APP.h"
#include "Buzz_BL_APP.h"
#include "Pwm_APP.h"
#include "Opration_finc.h"
#include "SD_APP.h"
#include "Program_updata_App.h"

#ifndef	__SYS_INIT__
#include "Sys_Initial.h"
#endif






