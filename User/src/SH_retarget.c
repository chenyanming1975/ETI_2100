/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2009 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include "NUC1xx.h"

#if defined ( __CC_ARM   )
#if (__ARMCC_VERSION < 400000)
#else
/* Insist on keeping widthprec, to avoid X propagation by benign code in C-lib */
#pragma import _printf_widthprec
#endif
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Macro Definition                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

//#define DEBUG_ENABLE_UART
#define DEBUG_ENABLE_SEMIHOST

/* Using UART0 or UART1 */  
#define DEBUG_PORT  0     	/* 0:UART0   1:UART1 */

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;


extern int SH_DoCommand(int n32In_R0, int n32In_R1, int *pn32Out_R0);
#ifndef DEBUG_ENABLE_SEMIHOST
int SH_DoCommand(int n32In_R0, int n32In_R1, int *pn32Out_R0)
{
	return 0;
}
#endif

/*---------------------------------------------------------------------------------------------------------*/
/* Routine to write a char                                                                                 */
/*---------------------------------------------------------------------------------------------------------*/

#if defined DEBUG_ENABLE_UART || defined DEBUG_ENABLE_SEMIHOST
static char g_buf[16];
static char g_buf_len = 0;
#endif

#ifdef DEBUG_ENABLE_UART
void SendChar_ToUART(int ch)
{
	//if(SH_DoCommand(0x03, ch, NULL) != 0)
	//	return;

	if(DEBUG_PORT == 1)
	{
	    while(UART1->FSR.TX_FULL == 1);
		UART1->DATA = ch;
	    if(ch == '\n')
	    {
	        while(UART1->FSR.TX_FULL == 1);
	        UART1->DATA = '\r';
	    }
	}
	else if(DEBUG_PORT == 0)
	{
		while(UART0->FSR.TX_FULL == 1);
		UART0->DATA = ch;
	    if(ch == '\n')
	    {
	        while(UART0->FSR.TX_FULL == 1);
	        UART0->DATA = '\r';
	    }
	}
}
#endif

void SendChar(int ch)
{
#if defined DEBUG_ENABLE_UART || defined DEBUG_ENABLE_SEMIHOST

	g_buf[g_buf_len++] = ch;
	g_buf[g_buf_len] = '\0';
	if(g_buf_len + 1 >= sizeof(g_buf) || ch == '\n' || ch == '\0')
	{


#ifdef DEBUG_ENABLE_SEMIHOST
		/* Send the char */
		g_buf_len = 0;
		if(SH_DoCommand(0x04, (int)g_buf, NULL) != 0)
			return;
#endif

#ifdef DEBUG_ENABLE_UART
		int i = 0;
		g_buf_len = 0;
		while(g_buf[i] != '\0')
			SendChar_ToUART(g_buf[i++]);
#endif
	}

#endif
}


/*---------------------------------------------------------------------------------------------------------*/
/* Routine to get a char                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
char GetChar()
{
#ifdef DEBUG_ENABLE_SEMIHOST
	int nRet;
	while(SH_DoCommand(0x101, 0, &nRet) != 0)
	{
		if(nRet != 0)
		{
			SH_DoCommand(0x07, 0, &nRet);
			return (char)nRet;
		}
	}
#endif

#ifdef DEBUG_ENABLE_UART
	if(DEBUG_PORT == 1)
	{
		while (1){
			if(UART1->FSR.RX_EMPTY == 0 )
			{
				return (UART1->DATA);
				
		    }
		}
	}
	else
	{
		while (1){
			if(UART0->FSR.RX_EMPTY == 0 )
			{
				return (UART0->DATA);
				
		    }
		}
	}
#else
	return 0;
#endif
}


/*---------------------------------------------------------------------------------------------------------*/
/* C library retargetting                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/

void _ttywrch(int ch)
{
  SendChar(ch);
  return;
}

int fputc(int ch, FILE *f)
{
  SendChar(ch);
  return 0;
}

int fgetc(FILE *f) {
  return (GetChar());
}


int ferror(FILE *f) {
  return EOF;
}
