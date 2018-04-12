//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Uart_APP.c
//	First EDIT		: Sam
//	Data			: 2012.06.08
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================
#define	__UART_AP__

//-------------------------------------------------------------------------------
#include "Incude_all.h"

//-------------------Pub Variable --------------------------
uint8_t		Uart_tx_rx_fifo_int_FLAG;

volatile uint8_t		Rx_TX_flag;

uint8_t 	re_table0[4];
uint8_t 	re_table2[2];

volatile uint8_t 	U_slave_add_temp;
volatile uint8_t 	U_cmd_temp;
volatile uint8_t 	U_save_data_lenth;
volatile uint16_t 	U_read_write_data_lenth;
volatile uint16_t	U_start_adds_temp;

uint8_t		U_data_buff[256];
volatile uint8_t	U_RX_TX_byte_lenth;		//for caculat the data lenth
volatile uint8_t	U_RX_TX_byte_lenth_cnt;		//for caculat the data lenth

volatile uint16_t	U_Rx_wait_time_flag_temp;	//wait for 10sec

volatile uint8_t	Uart2_BAUD_select_temp;
volatile uint8_t        Uart2_JOJY_select_temp;
volatile uint8_t        Uart2_STOP_select_temp;
//-------------------Uart2 use Variable --------------------------
volatile uint8_t	U2_TX_byte_lenth;
volatile uint8_t	U2_rx_cnt;
volatile uint8_t	U2_rx_Flag;
volatile uint8_t	U2_tx_Flag;
/////uint8_t 	U2_re_table[16];
uint8_t 	U2_re_table[256];

//-------------------Test potocal Variable --------------------------
//##0093ST=32;CN=8013; PW=;MN=5256994000210 0;CP=&&DataTime=YYYY MMDDHHMMSS;060-Rtd=X XXX.XX,060-Flag=X&&X XXX\r\n
uint8_t 	Test_potocal_buff[256];

volatile uint8_t	Test_potocal_data_lenth;
volatile uint8_t	Test_potocal_data_lenth_cnt;
volatile uint8_t	Test_potocal_time_flag;
volatile uint8_t	Test_potocal_time_cnt;
volatile uint16_t	Test_potocal_time_mini_cnt;

//===============================================================================
//	finction	:CRC16
//	input		:data
//			 length
//	output		:null
//	return		:null
//	edit		:sam 2012.06.25
//	modefy		:null
//===============================================================================
uint16_t CRC16(uint8_t *data,uint8_t length)
{
    uint16_t result;
    uint8_t i,j;

    result=0xFFFF;
    if(length!=0)
    {
        for(i=0; i<length; i++)
        {
            result^=(uint16_t)(data[i]);
            for(j=0; j<8; j++)
            {
                if((result&0x0001)==0x0001)
                {
                    result>>=1;
                    result^=U_CRC_GEN;	//a001
                }
                else result>>=1;
            }
        }
    }
    return result;
}

//===============================================================================
//	finction	:Com1_rd_ctrl
//	input		:RD1_01
//	output		:null
//	return		:null
//	edit		:sam 2012.06.08
//	modefy		:null
//===============================================================================
void Com1_rd_ctrl(uint8_t RD1_01)			//串口1收发硬件控制转换
{
    if(RD1_01)
        DrvGPIO_SetBit (E_GPA,11);	//RD1=HIGHT
    else
        DrvGPIO_ClrBit (E_GPA,11);	//RD1=LOW
}

//===============================================================================
//	finction	:Com2_rd_ctrl
//	input		:RD2_01
//	output		:null
//	return		:null
//	edit		:sam 2012.06.08
//	modefy		:null
//===============================================================================
void Com2_rd_ctrl(uint8_t RD2_01)			//串口2收发硬件控制转换
{
    if(RD2_01)
        DrvGPIO_SetBit (E_GPB,7);	//RD1=HIGHT
    else
        DrvGPIO_ClrBit (E_GPB,7);	//RD1=LOW
}

//===============================================================================
//	finction	:En_Uart012_THRE_int
//	input		:uPort	:UART_PORT0/UART_PORT1/UART_PORT2
//	output		:null
//	return		:null
//	edit		:sam 2012.06.13
//	modefy		:null
//===============================================================================
void En_Uart012_THRE_int(uint32_t uPort)			//串口0.1.2允许发送中断控制
{
    UART_T  *tUART=0;

    if(uPort==UART_PORT0)
    {
        tUART = (UART_T *)((uint32_t)UART0 + UART_PORT0);
    }
    if(uPort==UART_PORT1)
    {
        tUART = (UART_T *)((uint32_t)UART0 + UART_PORT1);
    }
    if(uPort==UART_PORT2)
    {
        tUART = (UART_T *)((uint32_t)UART0 + UART_PORT2);
    }
    tUART->IER.THRE_IEN=1;				//en thre int
}

#ifdef __UART_TEST__
//===============================================================================
//	finction	:Get_Test_potocal_loop_time		//北京认证上传数据间隔时间计时
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-10-31 11:18
//	modefy		:null
//===============================================================================
void Get_Test_potocal_loop_time(void)
{
    if(Test_potocal_time_flag>0)
    {
        ++Test_potocal_time_mini_cnt;
        if(Test_potocal_time_mini_cnt==5959)
        {
            Test_potocal_time_mini_cnt=0;
            ++U2_P_minu;
            if(U2_P_minu>=10)
            {
                U2_P_minu=0;
                //   ++U2_P_hour;
                //   if(U2_P_hour==24)
                //   {
                //   	U2_P_hour=0;
                //   	++U2_P_data;
                //   	if(U2_P_data==29)
                //   	{
                //   	   U2_P_data=1;
                //   	   ++U2_P_mouth;
                //   	}
                //   }
                Test_potocal_time_flag=0x04;
            }
            //Test_potocal_time_flag=0x04;
        }
    }
}

//===============================================================================
//	finction	:Get_Test_potocal_data		//北京认证数据上传协议
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-10-31 11:18
//	modefy		:null
//===============================================================================
//##0093ST=31;CN=2051;PW=123456;MN=GZYC0000000001;CP=&&DataTime=YYYYMMDDHHMMSS;Z01-Avg=XXXX.XXXX,Z01-Flag=X&&XXXX\r\n
//##0093ST=31;CN=2051;PW=123456;MN=GZYC0000000001;CP=&&DataTime=YYYYMMDDHHMMSS;Z01-Avg=XXXX.XXXX,Z01-Flag=NXXXX\r\n

//##0093ST=31;CN=2051;PW=123456;MN=GZYC0000000001;CP=&&DataTime=YYYYMMDDHHMMSS;Z01-Avg=XXXX.XXXX&&XXXX\r\n
void Get_Test_potocal_data(uint8_t select)
{
    uint8_t i,j,k,buff_temp[512],check,check_rtc;
    uint16_t CRC_buff_temp;
    uint32_t adds_a,adds_b;
    uint8_t const *index;

    uint32_t   u32delayno;

    UART_T * tUART;
    tUART = (UART_T *)((uint32_t)UART0 + UART_PORT2);

    Test_potocal_buff[0]='#';
    Test_potocal_buff[1]='#';

    Test_potocal_buff[2]='0';		//lenth
    Test_potocal_buff[3]='0';
    Test_potocal_buff[4]='9';
    Test_potocal_buff[5]='0';
    Test_potocal_buff[6]='S';
    Test_potocal_buff[7]='T';
    Test_potocal_buff[8]='=';
    Test_potocal_buff[9]='3';
    Test_potocal_buff[10]='1';
    Test_potocal_buff[11]=';';
    Test_potocal_buff[12]='C';
    Test_potocal_buff[13]='N';
    Test_potocal_buff[14]='=';

//    if(select==1)
//    {
//       Test_potocal_buff[15]='2';
//       Test_potocal_buff[16]='0';
//       Test_potocal_buff[17]='1';
//       Test_potocal_buff[18]='1';
//    }
//    else
//    {
    Test_potocal_buff[15]='2';
    Test_potocal_buff[16]='0';
    Test_potocal_buff[17]='5';
    Test_potocal_buff[18]='1';
//    }
    Test_potocal_buff[19]=';';
    Test_potocal_buff[20]='P';
    Test_potocal_buff[21]='W';
    Test_potocal_buff[22]='=';
    Test_potocal_buff[23]='1';
    Test_potocal_buff[24]='2';
    Test_potocal_buff[25]='3';
    Test_potocal_buff[26]='4';
    Test_potocal_buff[27]='5';
    Test_potocal_buff[28]='6';
    Test_potocal_buff[29]=';';
    Test_potocal_buff[30]='M';
    Test_potocal_buff[31]='N';
    Test_potocal_buff[32]='=';
    Test_potocal_buff[33]='G';
    Test_potocal_buff[34]='Z';
    Test_potocal_buff[35]='Y';
    Test_potocal_buff[36]='C';
    Test_potocal_buff[37]='0';
    Test_potocal_buff[38]='0';
    Test_potocal_buff[39]='0';
    Test_potocal_buff[40]='0';
    Test_potocal_buff[41]='0';
    Test_potocal_buff[42]='0';
    Test_potocal_buff[43]='0';
    Test_potocal_buff[44]='0';
    Test_potocal_buff[45]='0';
    Test_potocal_buff[46]=Get_ascii_data(Machine_note_use); //仪器编号=1,2,3
    Test_potocal_buff[47]=';';
    Test_potocal_buff[48]='C';
    Test_potocal_buff[49]='P';
    Test_potocal_buff[50]='=';
    Test_potocal_buff[51]='&';
    Test_potocal_buff[52]='&';
    Test_potocal_buff[53]='D';
    Test_potocal_buff[54]='a';
    Test_potocal_buff[55]='t';
    Test_potocal_buff[56]='a';
    Test_potocal_buff[57]='T';
    Test_potocal_buff[58]='i';
    Test_potocal_buff[59]='m';
    Test_potocal_buff[60]='e';
    Test_potocal_buff[61]='=';
//------------------------------------------------------ processe data
    adds_a=adds_b=FX_write_index_adds_point;
    adds_b&=0x000001ff;				//byte adds
    adds_a>>=16;
    adds_a&=0x0000ffff;				//page adds
    if((adds_b==0x00000000)&&(adds_a==0x0000004))		//get first adds
        __nop();
    else
    {
        if(adds_b==0x00000000)
        {
            adds_b=0x000001e0;
            adds_a-=1;
        }
        else
            adds_b-=0x20;
    }
    Spi_read(adds_a,buff_temp);
    Test_potocal_buff[62]='2';
    Test_potocal_buff[63]='0';
    Test_potocal_buff[64]='1';
    Test_potocal_buff[65]='5';

//       i=buff_temp[adds_b+3]>>4;
//       i&=0x0f;
//       Test_potocal_buff[62]=Get_ascii_data(i);		//year
//       i=buff_temp[adds_b+3];
//       i&=0x0f;
//       Test_potocal_buff[63]=Get_ascii_data(i);		//year
//       i=buff_temp[adds_b+4]>>4;
//       i&=0x0f;
//       Test_potocal_buff[64]=Get_ascii_data(i);		//year
//       i=buff_temp[adds_b+4];
//       i&=0x0f;
//       Test_potocal_buff[65]=Get_ascii_data(i);		//year
//
    i=buff_temp[adds_b+5]>>4;
    i&=0x0f;
    Test_potocal_buff[66]=Get_ascii_data(i);		//mouth
    i=buff_temp[adds_b+5];
    i&=0x0f;
    Test_potocal_buff[67]=Get_ascii_data(i);		//mouth
    i=buff_temp[adds_b+6]>>4;
    i&=0x0f;
    Test_potocal_buff[68]=Get_ascii_data(i);		//day
    i=buff_temp[adds_b+6];
    i&=0x0f;
    Test_potocal_buff[69]=Get_ascii_data(i);		//day

    i=buff_temp[adds_b+7]>>4;
    i&=0x0f;
    check=i;
    check<<=4;
    Test_potocal_buff[70]=Get_ascii_data(i);		//hour
    i=buff_temp[adds_b+7];
    i&=0x0f;
    check|=i;
    Test_potocal_buff[71]=Get_ascii_data(i);		//hour

    check_rtc=RTC_DATA.hour_temp;
    check_rtc&=0x3f;

    if((check==0x00)&&(check_rtc==0x00))	__nop();
    else if(check_rtc==0x00) check_rtc=0x24;

    if(check>=check_rtc)
    {
        adds_a=adds_b=FX_write_index_adds_point;
        adds_b&=0x000001ff;				//byte adds
        adds_a>>=16;
        adds_a&=0x0000ffff;				//page adds
        if((adds_b==0x00000000)&&(adds_a==0x0000004))		//get first adds
            __nop();
        else
        {
            if(adds_b==0x00000000)
            {
                adds_b=0x000001c0;
                adds_a-=1;
            }
            else if(adds_b==0x00000020)
            {
                adds_b=0x000001e0;
                adds_a-=1;
            }
            else
                adds_b-=0x40;
        }
        Spi_read(adds_a,buff_temp);

        i=buff_temp[adds_b+5]>>4;
        i&=0x0f;
        Test_potocal_buff[66]=Get_ascii_data(i);		//mouth
        i=buff_temp[adds_b+5];
        i&=0x0f;
        Test_potocal_buff[67]=Get_ascii_data(i);		//mouth
        i=buff_temp[adds_b+6]>>4;
        i&=0x0f;
        Test_potocal_buff[68]=Get_ascii_data(i);		//day
        i=buff_temp[adds_b+6];
        i&=0x0f;
        Test_potocal_buff[69]=Get_ascii_data(i);		//day

        i=buff_temp[adds_b+7]>>4;
        i&=0x0f;
        Test_potocal_buff[70]=Get_ascii_data(i);		//hour
        i=buff_temp[adds_b+7];
        i&=0x0f;
        Test_potocal_buff[71]=Get_ascii_data(i);		//hour
    }

//       j=RTC_DATA.minu_temp;
//       j=j>>4;
//       j&=0x03;
//       Test_potocal_buff[72]=Get_ascii_data(j);
//       j=RTC_DATA.minu_temp;
//       j&=0x0f;
//       Test_potocal_buff[73]=Get_ascii_data(j);

    //////RTC_read_12byte_data();
    ////Test_potocal_buff[62]='2';
    ////Test_potocal_buff[63]='0';
    ////Test_potocal_buff[64]='1';
    ////Test_potocal_buff[65]='4';
    ////j=U2_P_mouth;
    ////j/=10;
    ////Test_potocal_buff[66]=Get_ascii_data(j);
    ////j=U2_P_mouth;
    ////j%=10;
    ////Test_potocal_buff[67]=Get_ascii_data(j);
    ////j=U2_P_data;
    ////j/=10;
    ////Test_potocal_buff[68]=Get_ascii_data(j);
    ////j=U2_P_data;
    ////j%=10;
    ////Test_potocal_buff[69]=Get_ascii_data(j);
    ////j=U2_P_hour;
    ////j/=10;
    ////Test_potocal_buff[70]=Get_ascii_data(j);
    ////j=U2_P_hour;
    ////j%=10;
    ////Test_potocal_buff[71]=Get_ascii_data(j);

    Test_potocal_buff[72]='0';		        //minute
    Test_potocal_buff[73]='0';		        //minute

    Test_potocal_buff[74]='0';		        //sec
    Test_potocal_buff[75]='0';		        //sec
    Test_potocal_buff[76]=';';
//--------------------------------------
    Test_potocal_buff[77]='Z';
    Test_potocal_buff[78]='0';
    Test_potocal_buff[79]='2';
    Test_potocal_buff[80]='-';
    Test_potocal_buff[81]='A';
    Test_potocal_buff[82]='v';
    Test_potocal_buff[83]='g';
    Test_potocal_buff[84]='=';

    i=buff_temp[adds_b+9]>>4;
    i&=0x0f;
    Test_potocal_buff[85]=Get_ascii_data(i);
    i=buff_temp[adds_b+9];
    i&=0x0f;
    Test_potocal_buff[86]=Get_ascii_data(i);
    i=buff_temp[adds_b+10]>>4;
    i&=0x0f;
    Test_potocal_buff[87]=Get_ascii_data(i);
    i=buff_temp[adds_b+10];
    i&=0x0f;
    Test_potocal_buff[88]=Get_ascii_data(i);
    Test_potocal_buff[89]='.';
    i=buff_temp[adds_b+11]>>4;
    i&=0x0f;
    Test_potocal_buff[90]=Get_ascii_data(i);
    i=buff_temp[adds_b+11];
    i&=0x0f;
    Test_potocal_buff[91]=Get_ascii_data(i);
    i=buff_temp[adds_b+12]>>4;
    i&=0x0f;
    Test_potocal_buff[92]=Get_ascii_data(i);
    i=buff_temp[adds_b+12];
    i&=0x0f;
    Test_potocal_buff[93]=Get_ascii_data(i);
//-----------------------------------------------------
    Test_potocal_buff[94]='&';
    Test_potocal_buff[95]='&';
    Test_potocal_buff[96]='0';
    Test_potocal_buff[97]='0';
    Test_potocal_buff[98]='0';
    Test_potocal_buff[99]='0';
    Test_potocal_buff[100]=13;		//CR   回车 13
    Test_potocal_buff[101]=10;
    k=90;				//lenth
//----------------------------------------------------set data
    if((Test_potocal_buff[87]=='0')&&(Test_potocal_buff[85]=='0')&&(Test_potocal_buff[86]=='0'))
    {
        k-=3;
        for(i=85; i<99; i++)
            Test_potocal_buff[i]=Test_potocal_buff[i+3];

        j=3;
    }
    else if((Test_potocal_buff[86]=='0')&&(Test_potocal_buff[85]=='0'))
    {
        k-=2;
        for(i=85; i<100; i++)
            Test_potocal_buff[i]=Test_potocal_buff[i+2];

        j=2;
    }
    else if(Test_potocal_buff[85]=='0')
    {
        k-=1;
        for(i=85; i<101; i++)
            Test_potocal_buff[i]=Test_potocal_buff[i+1];

        j=1;
    }
    i=k;
    i/=100;
    Test_potocal_buff[3]=Get_ascii_data(i);		//重设长度
    i=k;
    i/=10;
    i%=10;
    Test_potocal_buff[4]=Get_ascii_data(i);
    i=k;
    i%=10;
    Test_potocal_buff[5]=Get_ascii_data(i);
//----------------------------------------------------
    for(i=0; i<k; i++)
    {
        buff_temp[i]=Test_potocal_buff[i+6];
    }
    CRC_buff_temp=CRC16(buff_temp,k);

    i=CRC_buff_temp>>12;
    i&=0x0f;
    buff_temp[0]=i;
    i=CRC_buff_temp>>8;
    i&=0x0f;
    buff_temp[1]=i;
    i=CRC_buff_temp>>4;
    i&=0x0f;
    buff_temp[2]=i;
    i=CRC_buff_temp;
    i&=0x0f;
    buff_temp[3]=i;

    Test_potocal_buff[96-j]=Get_ascii_data(buff_temp[2]);
    Test_potocal_buff[97-j]=Get_ascii_data(buff_temp[3]);
    Test_potocal_buff[98-j]=Get_ascii_data(buff_temp[0]);
    Test_potocal_buff[99-j]=Get_ascii_data(buff_temp[1]);

    Test_potocal_data_lenth=102-j;

    Com2_rd_ctrl(1);
    DrvGPIO_ClrBit(E_GPB,12);
    DrvSYS_Delay(2000);				//delay 1ms
//    Test_potocal_data_lenth_cnt=1;
//    DrvUART_Write(UART_PORT2,Test_potocal_buff,1);
//    En_Uart012_THRE_int(UART_PORT2);
    Test_potocal_data_lenth_cnt=0;
//------------------------------------------------start yx
    for(Test_potocal_data_lenth_cnt=0; Test_potocal_data_lenth_cnt<Test_potocal_data_lenth; Test_potocal_data_lenth_cnt++)
    {
        re_table2[0]=Test_potocal_buff[Test_potocal_data_lenth_cnt];
        DrvUART_Write(UART_PORT2,re_table2,1);

        u32delayno = 0;
        while (tUART->FSR.TE_FLAG !=1)						    	/* Wait Tx empty */
        {
            u32delayno++;
            if ( u32delayno >= 0x00400000 )
            {
                Set_en_buzz_finc(4,4,200,100);
                break;
            }
        }
    }
    DrvSYS_Delay(4000);
    Com2_rd_ctrl(0);
    DrvGPIO_SetBit(E_GPB,12);
}

//===============================================================================
//	finction	:Run_potocal_test			//北京认证上传数据与心跳协议控制
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-11-1 11:56
//	modefy		:null
//===============================================================================
uint8_t ret_p_flag;

void Run_potocal_test(void)
{
    uint8_t i;

    if(Test_potocal_time_flag==0x04)
    {
        if(Rx_TX_flag!=0)
        {
            for(i=0; i<250; i++)
                DrvSYS_Delay(2000);
        }
        Get_Test_potocal_data(1);
        Test_potocal_time_flag=2;
    }
    else
    {
        i=RTC_DATA.minu_temp;
        i&=0x7f;
        if((i==0)&&(ret_p_flag!=0x55))
        {
            Get_Test_potocal_data(1);
            ret_p_flag=0x55;
        }
        else if(i!=0) ret_p_flag=0;
    }
}

#endif

//===============================================================================
//	finction	:Com1_rd_ctrl
//	input		:RD1_01
//	output		:null
//	return		:null
//	edit		:sam 2012.06.08
//	modefy		:null
//===============================================================================
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
//USART_SendData(EVAL_COM1, (uint8_t) ch);
//  	USARTx_Tx(USART2 ,ch);
//	USARTx_TxEN(USART2);
//	while((USART2->SR&(1<<6))!=(1<<6));
//	USARTx_TxUN(USART2);
//	USARTx_Status_Reset(USART2);

    /* Loop until the end of transmission */
    uint8_t *i,j;

    j=(uint8_t)ch;
    i=&j;

    DrvUART_Write(UART_PORT2,i,1);

    return(ch);
}

//===============================================================================
//	finction	:Dis_Uart012_THRE_int
//	input		:uPort	:UART_PORT0/UART_PORT1/UART_PORT2
//	output		:null
//	return		:null
//	edit		:sam 2012.06.13
//	modefy		:null
//===============================================================================
void Dis_Uart012_THRE_int(uint32_t uPort)			//串口0.1.2禁止发送中断控制
{
    UART_T  *tUART=0;

    if(uPort==UART_PORT0)
    {
        tUART = (UART_T *)((uint32_t)UART0 + UART_PORT0);
    }
    if(uPort==UART_PORT1)
    {
        tUART = (UART_T *)((uint32_t)UART0 + UART_PORT1);
    }
    if(uPort==UART_PORT2)
    {
        tUART = (UART_T *)((uint32_t)UART0 + UART_PORT2);
    }
    tUART->IER.THRE_IEN=0;				//DIS thre int
}

//===============================================================================
//	finction	:Write_more_byte_set_data
//	input		:null
//			 null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.26
//	modefy		:null
//===============================================================================
void Write_more_byte_set_data(uint8_t data_lenth)
{
    uint8_t i_len;

    if(data_lenth<=0x78)
    {
//        Spi_buff_page_read512(CMD_BUFF1_READ,pub_Adds_h,pub_Adds_m,pub_Adds_l,data_lenth);
//        Spi_memory_page_read(CMD_MAIN_MMP_READ,adds_temp,SYSTEM_REG_LENTH);
        for(i_len=0; i_len<=data_lenth; i_len++)
        {
            U_data_buff[7+i_len]=buff_page[i_len];
        }
    }
}

//===============================================================================
//	finction	:Set_tx_data_and_start_tx
//	input		:null
//			 null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.26
//	modefy		:null
//===============================================================================
void Set_tx_data_and_start_tx(void)			//窜口1设置使用查询方式发送数据。
{
    uint32_t k;
    uint16_t i;

    uint8_t    RT_buff[2];
    uint32_t   u32delayno;

    UART_T * tUART;
    tUART = (UART_T *)((uint32_t)UART0 + UART_PORT1);


    U_data_buff[0]=U_slave_add_temp;
    U_data_buff[1]=U_cmd_temp;
    U_data_buff[2]=U_start_adds_temp>>8;
    U_data_buff[3]=U_start_adds_temp;

    U_data_buff[4]=U_read_write_data_lenth>>8;
    U_data_buff[5]=U_read_write_data_lenth;
    U_RX_TX_byte_lenth=6;

    if(U_data_buff[1]==0x10)
    {
        U_data_buff[6]=U_save_data_lenth;
        ++U_RX_TX_byte_lenth;
        U_RX_TX_byte_lenth=U_RX_TX_byte_lenth+U_save_data_lenth;		//set data form U_data_buff[7]

        Write_more_byte_set_data(U_save_data_lenth);			//set save data

        i=CRC16(U_data_buff,U_RX_TX_byte_lenth);
        U_data_buff[U_RX_TX_byte_lenth+1]=i>>8;
        U_data_buff[U_RX_TX_byte_lenth]=i;

        U_RX_TX_byte_lenth+=2;
    }
    else
    {
        i=CRC16(U_data_buff,U_RX_TX_byte_lenth);
//       DrvSYS_Delay(5000);			//delay 5ms
        U_data_buff[6]=i;
        U_data_buff[7]=i>>8;

        U_RX_TX_byte_lenth=8;
    }
//--------------------------------\start tx
    DrvGPIO_ClrBit (E_GPB,6);
    Com1_rd_ctrl(1);
    U_Rx_wait_time_flag_temp=0;

    Dis_Uart012_THRE_int(UART_PORT1);
    DrvGPIO_DisableEINT0();							//2013-7-10 10:59

    for(U_RX_TX_byte_lenth_cnt=0; U_RX_TX_byte_lenth_cnt<U_RX_TX_byte_lenth; U_RX_TX_byte_lenth_cnt++)
    {
        RT_buff[0]=U_data_buff[U_RX_TX_byte_lenth_cnt];
        DrvUART_Write(UART_PORT1,RT_buff,1);

        u32delayno = 0;
        while (tUART->FSR.TE_FLAG !=1)						    	/* Wait Tx empty */
        {
            u32delayno++;
            if ( u32delayno >= 0x00400000 )
            {
                Set_en_buzz_finc(4,4,200,100);
                break;
            }
        }
    }

    DrvGPIO_EnableEINT0 (E_IO_FALLING,E_MODE_EDGE,EINT0Callback); 		//2013-7-10 10:59

    Uart_tx_rx_fifo_int_FLAG|=BIT3;
////    Dis_Uart012_THRE_int(UART_PORT1);
    U_RX_TX_byte_lenth_cnt=0x00;

    if(Rx_TX_flag_local==0x01)
        Rx_TX_flag_local=0;

    DrvSYS_Delay(200);
    Com1_rd_ctrl(0);
    U_Rx_wait_time_flag_temp=0x8000;

    U_data_buff[0]=U_data_buff[1]=0;

    k=GPIOB->DOUT;
    k&=0x00000800;
    if(k==0x00000800)
    {
        k=GPIOB->DOUT;
        k|=0x00000800;
        GPIOB->DOUT=k;
    }
}

//===============================================================================
//	finction	:UART0_IRQHandler_APP	串口0收发中断函数
//	input		:u32IntStatus
//	output		:null
//	return		:null
//	edit		:sam 2012.06.08
//	modefy		:null
//===============================================================================
void UART0_IRQHandler_APP(uint32_t u32IntStatus)
{
    uint8_t RT_buff[2];

    if(u32IntStatus & DRVUART_RDAINT)
    {
        /* Get all the input characters */
        while(UART0->ISR.RDA_IF==1) 		//RXD INT READ DATA
        {
            DrvGPIO_ClrBit(E_GPB,13);
            DrvUART_Read(UART_PORT0,re_table0,1);
            u32IntStatus&=~BIT1;
            Uart_tx_rx_fifo_int_FLAG|=BIT0;
        }
    }
    if(u32IntStatus & DRVUART_THREINT)
    {
        ++export_tx_lenth_cnt;
        RT_buff[0]=buff_page_exp_tx[export_tx_lenth_cnt];
        if((export_tx_lenth_cnt<export_tx_lenth)&&(export_tx_flag==1))
        {
            DrvUART_Write(UART_PORT0,RT_buff,1);
        }
        else
        {
            DrvSYS_Delay(5000);				//delay 5ms
            Uart_tx_rx_fifo_int_FLAG|=BIT1;
            Dis_Uart012_THRE_int(UART_PORT0);
            export_tx_lenth_cnt=0x00;
            export_tx_flag=0;
        }
    }
}

//===============================================================================
//	finction	:UART1_IRQHandler_APP	串口1收发中断函数
//	input		:u32IntStatus
//	output		:null
//	return		:null
//	edit		:sam 2012.06.08
//	modefy		:null
//===============================================================================
void UART1_IRQHandler_APP(uint32_t u32IntStatus)
{
    uint8_t    RT_buff[2],i;
/////    uint32_t   u32delayno;

    UART_T * tUART;
    tUART = (UART_T *)((uint32_t)UART0 + UART_PORT1);

    if(u32IntStatus & DRVUART_RDAINT)
    {
        /* Get all the input characters */
        while(UART1->ISR.RDA_IF==1) 		//RXD INT READ DATA
        {
            DrvGPIO_ClrBit(E_GPB,6);
            U_Rx_wait_time_flag_temp=0;
            Uart_tx_rx_fifo_int_FLAG&=~BIT3;

            DrvUART_Read(UART_PORT1,RT_buff,1);
            U_data_buff[U_RX_TX_byte_lenth_cnt]=RT_buff[0];
            ++U_RX_TX_byte_lenth_cnt;

            u32IntStatus&=~BIT1;

            if((U_data_buff[0]==0xd0)||(U_data_buff[0]==0xd1)||(U_data_buff[0]==CLEAR_ADDR_1))
            {
                i=U_data_buff[1];
                i&=0x80;
                if((i==0x80)&&(U_data_buff[1]!=CLEAR_ADDR_2))
                {
                    if(U_RX_TX_byte_lenth_cnt==5)
                        Uart_tx_rx_fifo_int_FLAG|=BIT2;
                    DrvGPIO_SetBit(E_GPB,6);
                    Rx_TX_flag=0;
                }

                else if(U_RX_TX_byte_lenth_cnt == 3)
                {
                    if(U_data_buff[2]==0x1F)
                    {
                        U2_disple_flag=1;
                        DrvGPIO_SetBit(E_GPB,6);
                        Rx_TX_flag=0;
                        U_RX_TX_byte_lenth_cnt=0;
                    }
                    else if(U_data_buff[2]==0x60)
                    {
                        Rx_TX_flag=0;
                        U_RX_TX_byte_lenth_cnt=0;
                    }
                    else if((U_data_buff[2]==0x55)||(U_data_buff[2]==0x65)||(U_data_buff[2]==0x59))  //U_data_buff[2]==0x1F,由0x62变更为0x1F
                    {
                        if(U_RX_TX_byte_lenth_cnt>=2)
                        {
                            U2_disple_flag=1;
                            DrvGPIO_SetBit(E_GPB,6);
                            Rx_TX_flag=0;
                        }
                    }
                    else
                    {}

                }


                // else if((U_data_buff[2]==0x60)&&(U_RX_TX_byte_lenth_cnt==2))
                //{
                // U_RX_TX_byte_lenth_cnt=0;
                // Rx_TX_flag=0;
                // }

                else if((U_data_buff[1]==0x10)||(U_data_buff[1]==0x06))
                {
                    if(U_RX_TX_byte_lenth_cnt==8)
                    {
                        Uart_tx_rx_fifo_int_FLAG|=BIT2;
                        DrvGPIO_SetBit(E_GPB,6);
                        Rx_TX_flag=0;
                    }
                }
                else if((U_RX_TX_byte_lenth_cnt==(U_data_buff[2]+5))&&(U_RX_TX_byte_lenth_cnt>=3))
                {
                    Uart_tx_rx_fifo_int_FLAG|=BIT2;
                    DrvGPIO_SetBit(E_GPB,6);
                    Rx_TX_flag=0;
                }
                else if((Rx_TX_flag_local==2)&&(U_RX_TX_byte_lenth_cnt>=6))
                {
                    DrvGPIO_SetBit(E_GPB,6);
                    Rx_TX_flag_local=0;
                    Rx_TX_flag=0;
                }
            }
            else
            {
                U_RX_TX_byte_lenth_cnt=0;
                DrvGPIO_SetBit(E_GPB,6);
            }
        }
    }
    if(u32IntStatus & DRVUART_THREINT)
    {
/////        RT_buff[0]=U_data_buff[U_RX_TX_byte_lenth_cnt];
/////        ++U_RX_TX_byte_lenth_cnt;
/////        if(U_RX_TX_byte_lenth_cnt<=U_RX_TX_byte_lenth)
/////        {
/////            DrvUART_Write(UART_PORT1,RT_buff,1);
/////        }
/////        else
/////        {
/////	   Uart_tx_rx_fifo_int_FLAG|=BIT3;
        Dis_Uart012_THRE_int(UART_PORT1);
/////	   U_RX_TX_byte_lenth_cnt=0x00;
/////
/////	   if(Rx_TX_flag_local==0x01)
/////	      Rx_TX_flag_local=0;
/////
/////           u32delayno = 0;
/////           while (tUART->FSR.TE_FLAG !=1)						    	/* Wait Tx empty */
/////           {
/////              u32delayno++;
/////              if ( u32delayno >= 0x00400000 )
/////              {
/////                 Set_en_buzz_finc(4,4,200,100);
/////////                 Debug_write_BJ(0x60000000);
/////                 goto time_o;
/////              }
/////           }
/////time_o:
/////   	   Com1_rd_ctrl(0);
/////   	   U_Rx_wait_time_flag_temp=0x8000;
/////        }
    }
}

//===============================================================================
//	finction	:Uart2_rx_time_cnt
//	input		:u32IntStatus
//	output		:null
//	return		:null
//	edit		:sam 2012.06.08
//	modefy		:null
//===============================================================================
uint8_t U2_rx_time_cnt;

void Uart2_rx_time_cnt(void)
{
    if(U2_rx_Flag==1)
    {
        ++U2_rx_time_cnt;
        if(U2_rx_time_cnt==20)
        {
            U2_rx_Flag=U2_rx_time_cnt=0;
            re_table2[0]=0;
        }
    }
}

//===============================================================================
//	finction	:UART2_IRQHandler_APP	串口2收发中断函数
//	input		:u32IntStatus
//	output		:null
//	return		:null
//	edit		:sam 2012.06.08
//	modefy		:null
//===============================================================================
void UART2_IRQHandler_APP(uint32_t u32IntStatus)
{
    uint32_t u32IntStatus_flag,i,u32delayno;

    UART_T * tUART;
    tUART = (UART_T *)((uint32_t)UART0 + UART_PORT2);

    i=u32IntStatus_flag=u32IntStatus;
    u32IntStatus_flag&=DRVUART_RDAINT;
    if(u32IntStatus_flag==DRVUART_RDAINT)
    {
        DrvUART_ClearIntFlag(UART_PORT2,DRVUART_RDAINT_FLAG);
        /* Get all the input characters */
        while(UART2->ISR.RDA_IF==1) 		//RXD INT READ DATA
        {
            UART2->ISR.RDA_IF=0;
            GPIOB->DMASK=0X0000EFFF;		//liaght led
            GPIOB->DOUT=0X00000000;
            GPIOB->DMASK=0X00000000;

//    	    u32IntStatus&=~BIT1;

#ifdef  __UART_TEST__
            DrvUART_Read(UART_PORT2,re_table2,1);
            UART2->ISR.RDA_IF=0;
            GPIOB->DMASK=0X0000EFFF;		//dark led
            GPIOB->DOUT|=0X00001000;
            GPIOB->DMASK=0X00000000;
#else
            if(U2_rx_cnt==0)
            {
                DrvUART_Read(UART_PORT2,re_table2,1);

//    	       if(re_table2[0]==0x10)
                if(re_table2[0]==Machine_note_use)
                {
                    U2_re_table[U2_rx_cnt]=re_table2[0];
                    U2_rx_Flag=1;
                    ++U2_rx_cnt;
                    U2_rx_time_cnt=0;
                }
                else
                {
                    U2_rx_Flag=0;
                    U2_rx_time_cnt=0;
                    re_table2[0]=0;
                    UART2->ISR.RDA_IF=0;
                    GPIOB->DMASK=0X0000EFFF;		//dark led
                    GPIOB->DOUT|=0X00001000;
                    GPIOB->DMASK=0X00000000;
                }
            }

            else if(U2_rx_Flag==1)
            {
                DrvUART_Read(UART_PORT2,re_table2,1);
                U2_re_table[U2_rx_cnt]=re_table2[0];
                ++U2_rx_cnt;

                if(U2_rx_cnt==8)
                {
                    U2_rx_Flag=0;
                    U2_rx_cnt=0;
                    U2_rx_time_cnt=0;

//    	       	  if((U2_re_table[1]==0x06)||(U2_re_table[1]==0x55)||(U2_re_table[1]==0x65))
//    	       	  U2_disple_flag=1;
//    	       	  else
                    Uart_tx_rx_fifo_int_FLAG|=BIT4;

/////	          DrvSYS_Delay(2000);
/////	          Com2_rd_ctrl(1);

                    GPIOB->DMASK=0X0000EFFF;		//dark led
                    GPIOB->DOUT|=0X00001000;
                    GPIOB->DMASK=0X00000000;
                    Dis_Uart012_THRE_int(UART_PORT2);
                    UART2->ISR.RDA_IF=0;
                }
            }
            else if(U2_rx_cnt>0)
            {
                U2_rx_cnt=0;
                DrvUART_Read(UART_PORT2,re_table2,1);
            }
#endif
        }
    }
    else
    {
#ifdef  __UART_TEST__
        u32IntStatus_flag=i;
        u32IntStatus_flag&=DRVUART_THREINT;
//        if(u32IntStatus_flag==DRVUART_THREINT)
//        {
//           DrvUART_ClearIntFlag(UART_PORT2,DRVUART_THREINT_FLAG);
//
//           re_table2[0]=Test_potocal_buff[Test_potocal_data_lenth_cnt];
//           ++Test_potocal_data_lenth_cnt;
//           if(Test_potocal_data_lenth_cnt<=Test_potocal_data_lenth)
//           {
//               DrvUART_Write(UART_PORT2,re_table2,1);
//           }
//           else
//           {
//	       Dis_Uart012_THRE_int(UART_PORT2);
//
//	       Test_potocal_data_lenth_cnt=0;
//   	       u32delayno = 0;
//               while (tUART->FSR.TE_FLAG !=1)				/* Wait Tx empty */
//               {
//                  u32delayno++;
//                  if ( u32delayno >= 0x00400000 )
//                  {
//                     Set_en_buzz_finc(4,4,200,100);
//                     goto time_o1;
//                  }
//               }
//time_o1:
//   	       Com2_rd_ctrl(0);
//   	       DrvGPIO_SetBit(E_GPB,12);
//           }
//        }
#else
        u32IntStatus_flag=i;
        u32IntStatus_flag&=DRVUART_THREINT;
        if((u32IntStatus_flag==DRVUART_THREINT)&&(U2_tx_Flag==1))
        {
            DrvUART_ClearIntFlag(UART_PORT2,DRVUART_THREINT_FLAG);

            re_table2[0]=U2_re_table[U2_rx_cnt];
            ++U2_rx_cnt;
            if(U2_rx_cnt<=U2_TX_byte_lenth)
            {
                DrvUART_Write(UART_PORT2,re_table2,1);
            }
            else
            {
                Uart_tx_rx_fifo_int_FLAG|=BIT5;
                Dis_Uart012_THRE_int(UART_PORT2);

                U2_tx_Flag=U2_rx_cnt=0;
                u32delayno = 0;
                while (tUART->FSR.TE_FLAG !=1)				/* Wait Tx empty */
                {
                    u32delayno++;
                    if ( u32delayno >= 0x00400000 )
                    {
                        Set_en_buzz_finc(4,4,200,100);
                        ////                  Debug_write_BJ(0x60000000);
                        goto time_o;
                    }
                }
time_o:
                Com2_rd_ctrl(0);
            }
        }
#endif
    }
}

//===============================================================================
//	finction	:Uart2_start_tx
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-10-9 10:28
//	modefy		:null
//===============================================================================
void Uart2_start_tx(void)
{
    Com2_rd_ctrl(1);
    DrvGPIO_ClrBit(E_GPB,12);
    DrvSYS_Delay(2000);				//delay 1ms
    U2_tx_Flag=U2_rx_cnt=1;
    DrvUART_Write(UART_PORT2,U2_re_table,1);
    En_Uart012_THRE_int(UART_PORT2);
}

//===============================================================================
//	finction	:Uart 0,1,2 initial		初始化串口0.1.2
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.08
//	modefy		:null
//===============================================================================
void Init_UART012(void)
{
    STR_UART_T  sParam;

    /* Enable UART clock */
    //DrvSYS_SelectIPClockSource(E_SYS_UART_CLKSRC,0);
    SYSCLK->CLKSEL1.UART_S = 0;				//UART clock select 12MHz
    SYSCLK->CLKDIV.UART_N = 0;

    /* UART Setting */
    sParam.u32BaudRate 		= 57600;
    sParam.u8cDataBits 		= DRVUART_DATABITS_8;
    sParam.u8cStopBits 		= DRVUART_STOPBITS_1;
    sParam.u8cParity 		= DRVUART_PARITY_NONE;
    sParam.u8cRxTriggerLevel    = DRVUART_FIFO_1BYTES;		//DRVUART_FIFO_1BYTES
//    sParam.u8TimeOut		= 0X010000;
    /* Set UART0 Configuration */
    DrvUART_Open(UART_PORT0,&sParam);
    /* Set UART2 Configuration */
//    sParam.u32BaudRate 		= 19200;
    Uart2_BAUD_select_temp=1;
    sParam.u32BaudRate 		= 9600;			//2012-10-24 9:18
    DrvUART_Open(UART_PORT2,&sParam);

    /* Set UART1 Configuration */
    sParam.u8cStopBits 		= DRVUART_STOPBITS_2;
    sParam.u32BaudRate 		= 38400;
    DrvUART_Open(UART_PORT1,&sParam);


    /* Set CALL BACK FINCTION */
    DrvUART_EnableInt(UART_PORT0,(DRVUART_RDAINT ),UART0_IRQHandler_APP);
    DrvUART_EnableInt(UART_PORT1,(DRVUART_RDAINT ),UART1_IRQHandler_APP);
    DrvUART_EnableInt(UART_PORT2,(DRVUART_RDAINT ),UART2_IRQHandler_APP);
    Com1_rd_ctrl(0);
    Com2_rd_ctrl(0);

    Dis_Uart012_THRE_int(UART_PORT2);
    DrvUART_ClearIntFlag(UART_PORT2,DRVUART_RDAINT_FLAG);
    DrvUART_ClearIntFlag(UART_PORT2,DRVUART_THREINT_FLAG);
    U2_tx_Flag=U2_rx_Flag=U2_rx_cnt=0;
}

//===============================================================================
//	finction	:Uart 1 initial		初始化串口1
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2014-2-10 10:27
//	modefy		:null
//===============================================================================
void Reset_init_UART1(int8_t select)
{
    STR_UART_T  sParam;

    /* Enable UART clock */
    //DrvSYS_SelectIPClockSource(E_SYS_UART_CLKSRC,0);
    DrvUART_Close(UART_PORT1);
    DrvSYS_Delay(100000);

    SYSCLK->CLKSEL1.UART_S = 0;				//UART clock select 12MHz
    SYSCLK->CLKDIV.UART_N = 0;

    if(select==1)
    {
        sParam.u32BaudRate 		= 9600;
    }
    else
    {
        sParam.u32BaudRate 		= 38400;
    }
    /* UART Setting */
    sParam.u8cDataBits 		= DRVUART_DATABITS_8;
    sParam.u8cParity 		= DRVUART_PARITY_NONE;
    sParam.u8cRxTriggerLevel    = DRVUART_FIFO_1BYTES;		//DRVUART_FIFO_1BYTES
    sParam.u8cStopBits 		= DRVUART_STOPBITS_2;

    DrvUART_Open(UART_PORT1,&sParam);
    /* Set CALL BACK FINCTION */
    DrvUART_EnableInt(UART_PORT1,(DRVUART_RDAINT ),UART1_IRQHandler_APP);
    Com1_rd_ctrl(0);
}

//===============================================================================
//	finction	:Wait_slave_return_time
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.13
//	modefy		:null
//===============================================================================
void Wait_slave_return_time(void)
{
    if((U_Rx_wait_time_flag_temp>=0x8000)&&(U_Rx_wait_time_flag_temp<0x8bb9))
    {
        ++U_Rx_wait_time_flag_temp;
///    	if(U_Rx_wait_time_flag_temp==0x8020)		//20 sec    10ms
        if(U_Rx_wait_time_flag_temp==0x8bb8)		//30 sec    10ms
        {
            U_Rx_wait_time_flag_temp=0x7fff;
        }
    }
}

//===============================================================================
//	finction	:Set_tx_cmd_data_finc		输入组织要发送的数据并启动发送
//	input		:slave_adds
//			 cmd
//			 start_adds
//			 data_lenth
//			 save_data_lenth
//	output		:null
//	return		:null
//	edit		:sam 2012.06.13
//	modefy		:null
//===============================================================================
void Set_tx_cmd_data_finc(int8_t slave_adds,int8_t cmd,int16_t start_adds,int16_t data_lenth,int8_t save_data_lenth)
{
/////   uint16_t i;
/////   for(i=0;i<40;i++)
    DrvSYS_Delay(6000000);		//delay 0.5s
    Uart_tx_rx_fifo_int_FLAG&=0xf9;	//clear bit2,3

    U_slave_add_temp=slave_adds;
    U_cmd_temp=cmd;
    U_start_adds_temp=start_adds;
    U_read_write_data_lenth=data_lenth;
    U_save_data_lenth=save_data_lenth;

    Set_tx_data_and_start_tx();
    Rx_TX_flag=1;
}

//===============================================================================
//	finction	:Uart012_FIFO_INT_CTRL		串口0.1.2处理函数
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.13
//	modefy		:null
//===============================================================================
void Uart012_FIFO_INT_CTRL(void)
{
    uint8_t mm,data_buff[14],m_buff[8];
    uint16_t i,j;
    uint32_t result;
    //------------------------------------UART0
    if(Uart_tx_rx_fifo_int_FLAG&BIT0)
    {
        Uart_tx_rx_fifo_int_FLAG&=~BIT0;
        DrvUART_Write(UART_PORT0,re_table0,1);
        En_Uart012_THRE_int(UART_PORT0);
        DrvGPIO_SetBit(E_GPB,13);
    }
    if(Uart_tx_rx_fifo_int_FLAG&BIT1)
    {
        Uart_tx_rx_fifo_int_FLAG&=~BIT1;
        DrvGPIO_SetBit(E_GPB,13);
    }
    //------------------------------------UART1
    if(Uart_tx_rx_fifo_int_FLAG&BIT2)
    {
        Uart_tx_rx_fifo_int_FLAG&=~BIT2;

        i=CRC16(U_data_buff,U_RX_TX_byte_lenth_cnt-2);
        j=U_data_buff[U_RX_TX_byte_lenth_cnt-1];
        j=j<<8;
        j+=U_data_buff[U_RX_TX_byte_lenth_cnt-2];

        if(j==i)
        {
            if((U_cmd_temp==U_data_buff[1])&&(U_slave_add_temp==U_data_buff[0]))
                __nop();
            else
            {
                mm=U_data_buff[1];
                mm&=0x80;
                if(mm==0x80)					//slave module return erro message
                {
                    Get_write_struct_data(3,2,0,0,0,0);		//记录错误信息
/////	         Program_reset_initial();			//reset all prog data.
                    Set_en_buzz_finc(4,4,200,100);
                    Led_data_flag|=BIT0;
                    Led_port_status|=BIT1;
                    switch(U_data_buff[1])
                    {
                    case 0x83:
                        data_buff[7]=Get_ascii_data('8');
                        data_buff[8]=Get_ascii_data('3');
                        break;
                    case 0x86:					//显示错误的状态
                        data_buff[7]=Get_ascii_data('8');
                        data_buff[8]=Get_ascii_data('6');
                        break;
                    case 0x90:
                        data_buff[7]=Get_ascii_data('9');
                        data_buff[8]=Get_ascii_data('0');
                        break;
                    }
                    data_buff[0]=Get_ascii_data('E');
                    data_buff[1]=Get_ascii_data('R');
                    data_buff[2]=Get_ascii_data('R');
                    data_buff[3]=20;
                    data_buff[4]=Get_ascii_data('D');
                    if(U_data_buff[0]==0xd0)
                        data_buff[5]=Get_ascii_data('0');
                    else
                        data_buff[5]=Get_ascii_data('1');
                    data_buff[6]=0;
                    data_buff[9]=0;
                    data_buff[10]=Get_ascii_data('0');
                    switch(U_data_buff[2])
                    {
                    case 0x01:
                        data_buff[11]=Get_ascii_data('1');
                        break;
                    case 0x02:
                        data_buff[11]=Get_ascii_data('2');
                        break;
                    case 0x03:
                        data_buff[11]=Get_ascii_data('3');
                        break;
                    }
                    Clear_apage(6);
                    Display_6x8_char(6,18,12,data_buff);
                }
            }
        }
        else						//crc erro
        {
            __nop();

        }
    }
    if(Uart_tx_rx_fifo_int_FLAG&BIT3)			//uart1 tx finish
    {
        Uart_tx_rx_fifo_int_FLAG&=~BIT3;
        DrvGPIO_SetBit(E_GPB,6);
    }
    //------------------------------------UART2
    if(Uart_tx_rx_fifo_int_FLAG&BIT4)
    {
        Uart_tx_rx_fifo_int_FLAG&=~BIT4;

        DrvGPIO_SetBit(E_GPB,12);

#ifdef  __UART_TEST__

#else
        i=CRC16(U2_re_table,6);
        j=U2_re_table[7];
        j=j<<8;
        j+=U2_re_table[6];

        if(j==i)			//crc ok
        {
            //check busy
/////	   if((Pro_power_on_flag==0)&&(Hand_finction_select==0)&&(Hand_finction_step_select==0)&&(Menu_lever3_start_flag<0x10)&&(M_lever4_extern_trrig_flag==1))
            if((Pro_power_on_flag==0)&&(M_lever4_extern_trrig_flag==1))
            {
                if(U2_re_table[1]==0x03)		//read
                {
                    //U2_re_table[0]=0x10;
                    U2_re_table[1]=0x03;
                    U2_re_table[0]=Machine_note_use;
                    switch(U2_re_table[3])
                    {
                    case 0x00:
                        U2_re_table[2]=0x02;
                        U2_re_table[3]=0x00;
                        U2_re_table[4]=M_menur1_analy_obj_select_temp;		//

                        i=CRC16(U2_re_table,5);
                        U2_re_table[5]=i;
                        U2_re_table[6]=i>>8;

                        U2_TX_byte_lenth=0x07;
                        break;
                    case 0x02:
                        U2_re_table[2]=0x02;
                        U2_re_table[3]=0x00;
                        U2_re_table[4]=M_menur1_unit_select_temp;		//

                        i=CRC16(U2_re_table,5);
                        U2_re_table[5]=i;
                        U2_re_table[6]=i>>8;

                        U2_TX_byte_lenth=0x07;
                        break;
                    case 0x04:
                        U2_re_table[2]=0x04;

                        Read_last_result(FX_write_index_adds_point,1,2);
                        i=CRC16(U2_re_table,7);

                        U2_re_table[7]=i;
                        U2_re_table[8]=i>>8;
                        U2_TX_byte_lenth=0x09;
                        break;
                    case 0x06:
                        U2_re_table[2]=0x06;

                        Read_last_result(FX_write_index_adds_point,1,1);

                        i=CRC16(U2_re_table,9);
                        U2_re_table[9]=i;
                        U2_re_table[10]=i>>8;
                        U2_TX_byte_lenth=0x0b;
                        break;
                    case 0x0A:
                        U2_re_table[2]=0x04;

                        Read_last_result(JZ_write_index_adds_point,2,2);
                        i=CRC16(U2_re_table,7);

                        U2_re_table[7]=i;
                        U2_re_table[8]=i>>8;
                        U2_TX_byte_lenth=0x09;
                        break;
                    case 0x0C:
                        U2_re_table[2]=0x06;

                        Read_last_result(JZ_write_index_adds_point,2,1);

                        i=CRC16(U2_re_table,9);
                        U2_re_table[9]=i;
                        U2_re_table[10]=i>>8;
                        U2_TX_byte_lenth=0x0b;
                        break;
                    case 0x14:
                        U2_re_table[2]=0x02;
                        U2_re_table[3]=0x00;
                        if((Menu_lever3_start_flag<0x10)&&(Hand_finction_select==0))
                            U2_re_table[4]=0x00;
                        else
                            U2_re_table[4]=0x01;

                        i=CRC16(U2_re_table,5);
                        U2_re_table[5]=i;
                        U2_re_table[6]=i>>8;

                        U2_TX_byte_lenth=0x07;
                        break;
                    case 0x18:
                        i=j=0;

                        mm=M_lever4_FXPL_temp>>12;
                        mm&=0x0f;
                        i=mm*1000;
                        j+=i;

                        mm=M_lever4_FXPL_temp>>8;
                        mm&=0x0f;
                        i=mm*100;
                        j+=i;

                        mm=M_lever4_FXPL_temp>>4;
                        mm&=0x0f;
                        i=mm*10;
                        j+=i;

                        mm=M_lever4_FXPL_temp;
                        mm&=0x0f;
                        j+=mm;

                        U2_re_table[2]=0x02;
                        U2_re_table[3]=j>>8;
                        U2_re_table[4]=j;

                        i=CRC16(U2_re_table,5);
                        U2_re_table[5]=i;
                        U2_re_table[6]=i>>8;

                        U2_TX_byte_lenth=0x07;
                        break;
                    case 0x19:
                        i=j=0;

                        mm=M_lever4_JZPL_temp>>8;
                        mm&=0x0f;
                        i=mm*100;
                        j+=i;

                        mm=M_lever4_JZPL_temp>>4;
                        mm&=0x0f;
                        i=mm*10;
                        j+=i;

                        mm=M_lever4_JZPL_temp;
                        mm&=0x0f;
                        j+=mm;

                        U2_re_table[2]=0x02;
                        U2_re_table[3]=j>>8;
                        U2_re_table[4]=j;

                        i=CRC16(U2_re_table,5);
                        U2_re_table[5]=i;
                        U2_re_table[6]=i>>8;

                        U2_TX_byte_lenth=0x07;
                        break;
                    case 0x1A:
                        i=j=0;

                        mm=M_lever4_QXPL_temp>>8;
                        mm&=0x0f;
                        i=mm*100;
                        j+=i;

                        mm=M_lever4_QXPL_temp>>4;
                        mm&=0x0f;
                        i=mm*10;
                        j+=i;

                        mm=M_lever4_QXPL_temp;
                        mm&=0x0f;
                        j+=mm;

                        U2_re_table[2]=0x02;
                        U2_re_table[3]=j>>8;
                        U2_re_table[4]=j;

                        i=CRC16(U2_re_table,5);
                        U2_re_table[5]=i;
                        U2_re_table[6]=i>>8;

                        U2_TX_byte_lenth=0x07;
                        break;
                    case 0x1B:
                        U2_TX_byte_lenth=0x08;
                        break;
                    }
                    Uart2_start_tx();
                }
                else if(U2_re_table[1]==0x06)	//write
                {
                    result=i=0;

                    U2_re_table[0]=Machine_note_use;		//2012-10-29 11:56
                    i=U2_re_table[4];
                    i<<=8;
                    i+=U2_re_table[5];
                    switch(U2_re_table[3])
                    {
                    case 0x18:
                        if(i<10000)
                        {
                            M_lever4_FXPL_temp=0;

                            mm=i/1000;
                            result=mm;
                            result<<=12;
                            M_lever4_FXPL_temp=result;


                            mm=i/100;
                            mm%=10;
                            result=mm;
                            result<<=8;
                            M_lever4_FXPL_temp+=result;

                            mm=i/10;
                            mm%=10;
                            result=mm;
                            result<<=4;
                            M_lever4_FXPL_temp+=result;

                            mm=i%10;
                            result=mm;
                            M_lever4_FXPL_temp+=result;
                        }
                        break;
                    case 0x19:
                        if(i<1000)
                        {
                            M_lever4_JZPL_temp=0;

                            mm=i/100;
                            result=mm;
                            result<<=8;
                            M_lever4_JZPL_temp+=result;

                            mm=i/10;
                            mm%=10;
                            result=mm;
                            result<<=4;
                            M_lever4_JZPL_temp+=result;

                            mm=i%10;
                            result=mm;
                            M_lever4_JZPL_temp+=result;
                        }
                        break;
                    case 0x1A:
                        if(i<1000)
                        {
                            M_lever4_QXPL_temp=0;

                            mm=i/100;
                            result=mm;
                            result<<=8;
                            M_lever4_QXPL_temp+=result;

                            mm=i/10;
                            mm%=10;
                            result=mm;
                            result<<=4;
                            M_lever4_QXPL_temp+=result;

                            mm=i%10;
                            result=mm;
                            M_lever4_QXPL_temp+=result;
                        }
                        break;
                    case 0x1B:
                        break;
                    case 0x1D:
                        if(Prog_disp_flag==1)
                        {
                            if(Menu_lever_select==4)
                                Display_lever4_index();
                            else
                                Display_menu_index();
                        }
/////                        Program_reset_initial();			//reset all prog data.
/////                        Hand_finction_select=0;
                        Program_reset_initial_flag=1;
                        Program_reset_initial_cnt=0;
                        break;
                    case 0x1E:
                        Memu1_auto_step_status=0;
                        Pro_step_status=0;
                        Auto_check_clock_flag=0;
                        Pro_auto_qingxi_freq_cnt=0;
                        Pro_auto_jiaozheng_freq_cnt=0;
                        Pro_auto_time_dcnt=0;
                        prg_wait_clr_flag=0;
                        Auto_check_clock_flag=1;
                        Clear_n_page(0,7);
                        m_buff[0]=58;
                        m_buff[1]=59;
                        m_buff[2]=49;
                        m_buff[3]=50;
                        Display_program_5opration(4,m_buff);

                        Menu_lever3_start_flag=BIT4;
                        break;
                    case 0x1F:
                        //Memu1_hand_step_status=0;
                        //Pro_step_status=0;
                        //M_lever4_FXCS_cnt=0;
                        Clear_n_page(0,7);

                        //Menu_lever3_start_flag=BIT5;

                        Menu_lever3_start_flag&=~BIT1;
                        Menu_lever3_start_flag=BIT5;
                        Memu1_auto_step_status=0;
                        Memu1_hand_step_status=0;
                        Pro_step_status=0;
                        M_lever4_FXCS_cnt=0;
                        Pro_wait_time_flag=2;
                        break;
                    case 0x20:
                        Memu1_jiaozheng_step_status=0;
                        Pro_step_status=0;
                        Clear_n_page(0,7);

                        Menu_lever3_start_flag=BIT6;
                        break;
                    case 0x21:
                        Memu1_biaoye_step_status=0;
                        Pro_step_status=0;
                        Clear_n_page(0,7);

                        Menu_lever3_start_flag=BIT7;
                        break;
                    }
                    U2_TX_byte_lenth=0x08;
                    Uart2_start_tx();
                }
            }
            else		//busy
            {
                U2_re_table[0]=Machine_note_use;		//2012-10-29 11:56
                if(U2_re_table[3]==0x14)			//check busy
                {
                    U2_re_table[2]=0x02;
                    U2_re_table[3]=0x00;
                    U2_re_table[4]=0x01;

                    i=CRC16(U2_re_table,5);
                    U2_re_table[5]=i;
                    U2_re_table[6]=i>>8;

                    U2_TX_byte_lenth=0x07;
                }
                else if((U2_re_table[3]==0x1d))			//stop all run finction
                {
                    if(Prog_disp_flag==1)
                    {
                        if(Menu_lever_select==4)
                            Display_lever4_index();
                        else
                            Display_menu_index();
                    }
/////                    Program_reset_initial();			//reset all prog data.
/////                    Hand_finction_select=0;
                    Program_reset_initial_flag=1;
                    Program_reset_initial_cnt=0;

                    U2_TX_byte_lenth=0x08;
                }
                else if(U2_re_table[1]==0x03)
                {
                    U2_re_table[0]=Machine_note_use;
                    U2_re_table[1]=0x03;
                    switch(U2_re_table[3])
                    {
                    case 0x00:
                        U2_re_table[2]=0x02;
                        U2_re_table[3]=0x00;
                        U2_re_table[4]=M_menur1_analy_obj_select_temp;		//

                        i=CRC16(U2_re_table,5);
                        U2_re_table[5]=i;
                        U2_re_table[6]=i>>8;

                        U2_TX_byte_lenth=0x07;
                        break;
                    case 0x02:
                        U2_re_table[2]=0x02;
                        U2_re_table[3]=0x00;
                        U2_re_table[4]=M_menur1_unit_select_temp;		//

                        i=CRC16(U2_re_table,5);
                        U2_re_table[5]=i;
                        U2_re_table[6]=i>>8;

                        U2_TX_byte_lenth=0x07;
                        break;
                    case 0x04:
                        U2_re_table[2]=0x04;
                        Read_last_result(FX_write_index_adds_point,1,2);
                        i=CRC16(U2_re_table,7);

                        U2_re_table[7]=i;
                        U2_re_table[8]=i>>8;
                        U2_TX_byte_lenth=0x09;
                        break;
                    case 0x06:
                        U2_re_table[2]=0x06;

                        Read_last_result(FX_write_index_adds_point,1,1);

                        i=CRC16(U2_re_table,9);
                        U2_re_table[9]=i;
                        U2_re_table[10]=i>>8;
                        U2_TX_byte_lenth=0x0b;
                        break;
                    }
                }
                else U2_TX_byte_lenth=0x08;
                Uart2_start_tx();
            }
        }
        else
        {
            U2_rx_cnt=0;
            re_table2[0]=re_table2[1]=0;
            Com2_rd_ctrl(0);
            DrvGPIO_SetBit(E_GPB,12);
        }
#endif
    }
    if(Uart_tx_rx_fifo_int_FLAG&BIT5)
    {
        Uart_tx_rx_fifo_int_FLAG&=~BIT5;

        Com2_rd_ctrl(0);
        DrvGPIO_SetBit(E_GPB,12);
    }
//-------------------------------------------------------------------------------
    if(U_Rx_wait_time_flag_temp==0x7fff)	//wait return time out
    {
        Get_write_struct_data(3,1,0,0,0,0);
        Rx_TX_flag=0;
        i=Led_port_status;
        i&=BIT1;
        if(i==BIT1)
            goto  erro_exit;

////	Led_data_flag|=BIT0;
////	Led_port_status|=BIT1;

/////        data_buff[0]=Get_ascii_data('E');
/////        data_buff[1]=Get_ascii_data('R');
/////        data_buff[2]=Get_ascii_data('R');
/////        data_buff[3]=20;
/////        data_buff[4]=Get_ascii_data('T');
/////        data_buff[5]=Get_ascii_data('I');
/////        data_buff[6]=Get_ascii_data('M');
/////        data_buff[7]=Get_ascii_data('E');
/////        data_buff[8]=0;
/////        data_buff[9]=Get_ascii_data('O');
/////        data_buff[10]=Get_ascii_data('U');
/////        data_buff[11]=Get_ascii_data('T');


        data_buff[0]=Get_ascii_data('E');
        data_buff[1]=Get_ascii_data('R');
        data_buff[2]=Get_ascii_data('R');
        data_buff[3]=20;
        data_buff[4]=Get_ascii_data('T');
        data_buff[5]=Get_ascii_data('O');

        LCD_disp_flag=1;
        Clear_apage(6);
        Display_6x8_char(6,18,6,data_buff);
////erro_clr:
        for(i=0; i<256; i++)
        {
            U_data_buff[i]=0;
        }
        U_slave_add_temp=0;
        U_cmd_temp=0;
        U_start_adds_temp=0;
        U_read_write_data_lenth=0;
        U_RX_TX_byte_lenth=0;
        U_RX_TX_byte_lenth_cnt=0;
        U_Rx_wait_time_flag_temp=0;

////	    i=Led_port_status;
////		i&=BIT1;
////		if(i==BIT1)
////		   goto  erro_exit;

        Set_en_buzz_finc(4,4,200,100);
////        Program_reset_initial();	//reset all prog data.
    }
erro_exit:
    __nop();
}

//===============================================================================
//	finction	:tx_jisuan_data		用于测试返回斜率不对时相关参数的值，用于DEBUG
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2013-4-25 16:01
//	modefy		:null
//===============================================================================
float    jisuan_k_temp;
void tx_jisuan_data(uint8_t fram)
{
    uint8_t i;
    uint16_t crc;
    uint32_t temp,temp1,temp2;

    U2_re_table[0]=0x01;
    U2_re_table[1]=0x10;
    U2_re_table[2]=0x00;
    U2_re_table[3]=0x00;
    U2_re_table[4]=0x00;
    U2_re_table[5]=0x1c;
    U2_re_table[6]=0x38;
    U2_re_table[7]=0x00;
    U2_re_table[8]=fram;
//------------------------------------------------
    temp=M_menur1_sj1_v_temp<<12;
    U2_re_table[9]=temp>>24;
    U2_re_table[10]=temp>>16;
    U2_re_table[11]=temp>>8;
    U2_re_table[12]=temp;

////    temp=M_menur1_sj2_v_temp<<12;		//

    temp1=P_Tcall_temp;
    temp2=temp1/100;
    temp2<<=24;
    temp|=temp2;
    temp2=temp1/10;
    temp2%=10;
    temp2<<=20;
    temp|=temp2;
    temp2=temp1%10;
    temp2<<=16;
    temp|=temp2;
    U2_re_table[13]=temp>>24;
    U2_re_table[14]=temp>>16;
    U2_re_table[15]=temp>>8;
    U2_re_table[16]=temp;
////    temp=M_menur1_sj3_v_temp<<12;		//
    temp=Prj_correlat;
    temp1=Prj_correlat;
    temp<<=4;
    temp1&=0xf0000000;
    temp1|=temp;
    U2_re_table[17]=temp1>>24;
    U2_re_table[18]=temp1>>16;
    U2_re_table[19]=temp1>>8;
    U2_re_table[20]=temp1;

////    temp=M_menur1_sj4_v_temp<<12;		//
    temp1=E_ADC_temperature1;
    i=temp1/1000;
    temp=i<<28;
    i=temp1/100;
    i=i%10;
    temp2=i<<24;
    temp|=temp2;
    i=temp1/10;
    i=i%10;
    temp2=i<<20;
    temp|=temp2;
    i=temp1%10;
    temp2=i<<16;
    temp|=temp2;
    U2_re_table[21]=temp>>24;
    U2_re_table[22]=temp>>16;
    U2_re_table[23]=temp>>8;
    U2_re_table[24]=temp;
/////    U2_re_table[21]=Pro_power_on_flag;
/////    U2_re_table[22]=Menu_lever3_start_flag;
/////    U2_re_table[23]=Auto_check_clock_flag;
/////    U2_re_table[24]=Pro_auto_time_dcnt;

    temp=M_menur1_jz1_v_temp<<8;
    U2_re_table[25]=temp>>24;
    U2_re_table[26]=temp>>16;
    U2_re_table[27]=temp>>8;
    U2_re_table[28]=temp;
    temp=M_menur1_jz2_v_temp<<8;
    U2_re_table[29]=temp>>24;
    U2_re_table[30]=temp>>16;
    U2_re_table[31]=temp>>8;
    U2_re_table[32]=temp;
    temp=M_menur1_jz1nd_temp<<8;
    U2_re_table[33]=temp>>24;
    U2_re_table[34]=temp>>16;
    U2_re_table[35]=temp>>8;
    U2_re_table[36]=temp;
    temp=M_menur1_jz2nd_temp<<12;
    U2_re_table[37]=temp>>24;
    U2_re_table[38]=temp>>16;
    U2_re_table[39]=temp>>8;
    U2_re_table[40]=temp;
//------------------------------
    U2_re_table[41]=M_menur1_slope_flag;
    U2_re_table[42]=Rx_TX_flag;

    temp=M_menur1_slope_temp<<8;
    U2_re_table[43]=temp>>24;
    U2_re_table[44]=temp>>16;
    U2_re_table[45]=temp>>8;
    U2_re_table[46]=temp;

    temp=Float_to_int_reg(c2_temp);;
    U2_re_table[47]=temp>>24;
    U2_re_table[48]=temp>>16;
    U2_re_table[49]=temp>>8;
    U2_re_table[50]=temp;

    if(Measurement_signal_21>31250) temp=Measurement_signal_21-31250;
    else                            temp=31250-Measurement_signal_21;
    temp=Float_to_int_reg_ee2(temp);
    U2_re_table[51]=temp>>24;
    U2_re_table[52]=temp>>16;
    U2_re_table[53]=temp>>8;
    U2_re_table[54]=temp;

    if(Measurement_signal_22>31250) temp=Measurement_signal_22-31250;
    else                            temp=31250-Measurement_signal_22;
    temp=Float_to_int_reg_ee2(temp);
    U2_re_table[55]=temp>>24;
    U2_re_table[56]=temp>>16;
    U2_re_table[57]=temp>>8;
    U2_re_table[58]=temp;

    if(jisuan_k_temp>0) i=1;
    else i=0;
    temp=Float_to_int_reg_ee(jisuan_k_temp);
    temp<<=4;
    if(i==1) temp|=0x10000000;

    U2_re_table[59]=temp>>24;
    U2_re_table[60]=temp>>16;
    U2_re_table[61]=temp>>8;
    U2_re_table[62]=temp;
//------------------------------------------------   CRC
    crc=CRC16(U2_re_table,63);
    U2_re_table[64]=crc>>8;
    U2_re_table[63]=crc;
    U2_TX_byte_lenth=65;
    Uart2_start_tx();
}



