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

volatile uint8_t 	U_slave_add_temp;//从机地址
volatile uint8_t 	U_cmd_temp;      //串口发送中的功能码
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
//	description :SP485R收发控制
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
    Spi_Page_Read(adds_a,buff_temp);
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
        Spi_Page_Read(adds_a,buff_temp);

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
//	input		:data_lenth
//	edit		:sam 2012.06.26
//	description	:将buff_page中的第i个数据 放入U_data_buff中的7+i的位置，依次复制data_lenth个
//===============================================================================
void Write_more_byte_set_data(uint8_t data_lenth)
{
    uint8_t i_len;

    if(data_lenth<=0x78)
    {
        for(i_len=0; i_len<=data_lenth; i_len++)
        {
            U_data_buff[7+i_len]=buff_page[i_len];
        }
    }
}

//===============================================================================
//	finction	:Set_tx_data_and_start_tx
//	input		:null
//	edit		:sam 2012.06.26
//	description	:Set_tx_cmd_data_finc函数中进行调用
//===============================================================================
void Set_tx_data_and_start_tx(void)			//窜口1设置使用查询方式发送数据。
{
    uint32_t k;
    uint16_t i;

    uint8_t    RT_buff[2];
    uint32_t   u32delayno;

    UART_T * tUART;
    tUART = (UART_T *)((uint32_t)UART0 + UART_PORT1);

    U_data_buff[0]=U_slave_add_temp;//从机地址
    U_data_buff[1]=U_cmd_temp;      //功能码
    U_data_buff[2]=U_start_adds_temp>>8;//起始地址高字节
    U_data_buff[3]=U_start_adds_temp;   //起始地址低字节
    U_data_buff[4]=U_read_write_data_lenth>>8;//数据长度高字节
    U_data_buff[5]=U_read_write_data_lenth;   //数据长度低字节
    U_RX_TX_byte_lenth=6;

    if(U_data_buff[1]==0x10)//写多个保持寄存器
    {
        U_data_buff[6]=U_save_data_lenth;
        ++U_RX_TX_byte_lenth;
        U_RX_TX_byte_lenth += U_save_data_lenth;    //set data form U_data_buff[7]
        Write_more_byte_set_data(U_save_data_lenth);//设置U_data_buff[7]之后的U_save_data_lenth个数据
		
        i=CRC16(U_data_buff,U_RX_TX_byte_lenth);//计算校验码
        U_data_buff[U_RX_TX_byte_lenth+1]=i>>8;//校验码高8位
        U_data_buff[U_RX_TX_byte_lenth]=i;     //校验码低8位

        U_RX_TX_byte_lenth+=2;
    }
    else
    {
        i=CRC16(U_data_buff,U_RX_TX_byte_lenth);
        U_data_buff[6]=i;     //CRC校验码低8位   
        U_data_buff[7]=i>>8;  //CRC校验码高8位

        U_RX_TX_byte_lenth=8;
    }
//--------------------------------\start tx
    DrvGPIO_ClrBit (E_GPB,6);//点亮LED-COM1
    Com1_rd_ctrl(1);         //发送使能
    U_Rx_wait_time_flag_temp=0;

    Dis_Uart012_THRE_int(UART_PORT1);//禁止发送中断
    DrvGPIO_DisableEINT0();			 //2013-7-10 10:59

    for(U_RX_TX_byte_lenth_cnt=0; U_RX_TX_byte_lenth_cnt<U_RX_TX_byte_lenth; U_RX_TX_byte_lenth_cnt++)
    {
        RT_buff[0]=U_data_buff[U_RX_TX_byte_lenth_cnt];
        DrvUART_Write(UART_PORT1,RT_buff,1);

        u32delayno = 0;
        while (tUART->FSR.TE_FLAG !=1)						    	/* Wait Tx empty */
        {
            u32delayno++;
            if ( u32delayno >= 0x00400000 )//发送超时
            {
                Set_en_buzz_finc(4,4,200,100);//蜂鸣器报警
                break;
            }
        }
    }

    
	DrvGPIO_EnableEINT0 (E_IO_FALLING,E_MODE_EDGE,EINT0Callback);//2013-7-10 10:59 使能EINT0中断

    Uart_tx_rx_fifo_int_FLAG |= BIT3;//置位bit3

    U_RX_TX_byte_lenth_cnt=0x00;//清零计数

    if(Rx_TX_flag_local==0x01)
        Rx_TX_flag_local=0;

    DrvSYS_Delay(200);
    Com1_rd_ctrl(0);  //接收使能
    U_Rx_wait_time_flag_temp=0x8000;//设置接收超时时间

    U_data_buff[0]=U_data_buff[1]=0;//清空从机地址与功能码
	
	//PB12(LED-COM2)输出高电平,灭灯
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
//	descripton  :串口0发送用于导出功能
//===============================================================================
void UART0_IRQHandler_APP(uint32_t u32IntStatus)
{
    uint8_t RT_buff[2];

	/* Receive Data Available Interrupt */
    if(u32IntStatus & DRVUART_RDAINT)
    {
        /* Get all the input characters */
        while(UART0->ISR.RDA_IF==1) 		//RXD INT READ DATA
        {
            DrvGPIO_ClrBit(E_GPB,13);//点亮LED-COM0
            DrvUART_Read(UART_PORT0,re_table0,1);//从串口0读取一个byte的数据
            u32IntStatus &=~ BIT1;   //清零bit1位
            Uart_tx_rx_fifo_int_FLAG |= BIT0;//置位bit0位
        }
    }
	
	/* Transmit Holding Register Empty Interrupt */
    if(u32IntStatus & DRVUART_THREINT)// 
    {
        ++export_tx_lenth_cnt;
        RT_buff[0]=buff_page_exp_tx[export_tx_lenth_cnt];
		//导出功能开启，并且数据还没有发送完
        if( (export_tx_lenth_cnt<export_tx_lenth) && (export_tx_flag==1))
        {
            DrvUART_Write(UART_PORT0,RT_buff,1);//发送1byte数据
        }
        else
        {
            DrvSYS_Delay(5000);				 //delay 5ms
            Uart_tx_rx_fifo_int_FLAG |= BIT1;//置位bit1位
            Dis_Uart012_THRE_int(UART_PORT0);//关中断
            export_tx_lenth_cnt=0x00;        //清空计数
            export_tx_flag=0;                //清标志
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

    UART_T * tUART;
    tUART = (UART_T *)((uint32_t)UART0 + UART_PORT1);
	
	/* Receive Data Available Interrupt */
    if(u32IntStatus & DRVUART_RDAINT)
    {
        /* Get all the input characters */
        while(UART1->ISR.RDA_IF==1) 		//RXD INT READ DATA
        {
            DrvGPIO_ClrBit(E_GPB,6);//点亮LED-COM1
            U_Rx_wait_time_flag_temp=0;
            Uart_tx_rx_fifo_int_FLAG &= ~BIT3;//清bit3

            DrvUART_Read(UART_PORT1,RT_buff,1);//读取1byte数据，放入U_data_buff中
            U_data_buff[U_RX_TX_byte_lenth_cnt]=RT_buff[0];
            ++U_RX_TX_byte_lenth_cnt;

            u32IntStatus &= ~DRVUART_THREINT;

            if((U_data_buff[0]==0xd0)||(U_data_buff[0]==0xd1)||(U_data_buff[0]==CLEAR_ADDR_1)) //判断从机地址是否为合法地址
            {
				
                i=U_data_buff[1];
                i&=0x80;
                if( (i==0x80) && (U_data_buff[1]!=CLEAR_ADDR_2) )
                {
                    if(U_RX_TX_byte_lenth_cnt==5)
                        Uart_tx_rx_fifo_int_FLAG |= BIT2;
                    DrvGPIO_SetBit(E_GPB,6);//灭灯LED-COM1
                    Rx_TX_flag=0;
                }
                else if(U_RX_TX_byte_lenth_cnt == 3)//当接收到3个数据时
                {
                    if(U_data_buff[2]==0x1F)
                    {
                        U2_disple_flag=1;
                        DrvGPIO_SetBit(E_GPB,6);//灭灯LED-COM1
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
                            DrvGPIO_SetBit(E_GPB,6);//灭灯LED-COM1
                            Rx_TX_flag=0;
                        }
                    }
                    else
                    {
					}
                }
				//判断数据接收完成，并设置Uart_tx_rx_fifo_int_FLAG 与 Rx_TX_flag标志位
                else if((U_data_buff[1]==0x10)||(U_data_buff[1]==0x06))//判断功能码
                {
                    if(U_RX_TX_byte_lenth_cnt==8)//数据接收完成
                    {
                        Uart_tx_rx_fifo_int_FLAG|=BIT2;
                        DrvGPIO_SetBit(E_GPB,6);
                        Rx_TX_flag=0;
                    }
                }
                else if( (U_RX_TX_byte_lenth_cnt == (U_data_buff[2]+5)) && (U_RX_TX_byte_lenth_cnt>=3) )
                {
                    Uart_tx_rx_fifo_int_FLAG|=BIT2;
                    DrvGPIO_SetBit(E_GPB,6);//灭灯LED-COM1
                    Rx_TX_flag=0;
                }
                else if((Rx_TX_flag_local==2)&&(U_RX_TX_byte_lenth_cnt>=6))
                {
                    DrvGPIO_SetBit(E_GPB,6);//灭灯LED-COM1
                    Rx_TX_flag_local=0;
                    Rx_TX_flag=0;
                }
            }
            else
            {
                U_RX_TX_byte_lenth_cnt=0;
                DrvGPIO_SetBit(E_GPB,6);//灭灯LED-COM1
            }
        }
    }
    if(u32IntStatus & DRVUART_THREINT)
    {
        Dis_Uart012_THRE_int(UART_PORT1);//禁止串口1发送中断
    }
}

//===============================================================================
//	finction	:Uart2_rx_time_cnt
//	edit		:sam 2012.06.08
//	description	:在TMR1_IRQHandler函数中进行调用，用于判断串口2接收数据是否超时
//===============================================================================
uint8_t U2_rx_time_cnt;
void Uart2_rx_time_cnt(void)
{
    if(U2_rx_Flag==1)
    {
        ++U2_rx_time_cnt;
        if(U2_rx_time_cnt==20)
        {
            U2_rx_Flag = U2_rx_time_cnt = 0;//标志与计数清零
            re_table2[0]=0;//清空串口2接收缓存的0位
        }
    }
}

//===============================================================================
//	finction	:UART2_IRQHandler_APP	串口2收发中断函数
//	input		:u32IntStatus
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
            GPIOB->DMASK=0X0000EFFF;		//liaght led-COM2（PB12）
            GPIOB->DOUT=0X00000000;
            GPIOB->DMASK=0X00000000;

#ifdef  __UART_TEST__
            DrvUART_Read(UART_PORT2,re_table2,1);
            UART2->ISR.RDA_IF=0;
            GPIOB->DMASK=0X0000EFFF;		//dark led-COM2(PB12)
            GPIOB->DOUT|=0X00001000;
            GPIOB->DMASK=0X00000000;
#else
            if(U2_rx_cnt==0)
            {
                DrvUART_Read(UART_PORT2,re_table2,1);//读取1byte数据

                if(re_table2[0]==Machine_note_use)//判断机器节点
                {
                    U2_re_table[U2_rx_cnt]=re_table2[0];
                    U2_rx_Flag=1;//开启超时
                    ++U2_rx_cnt;
                    U2_rx_time_cnt=0;
                }
                else//非法数据，不继续接收
                {
                    U2_rx_Flag=0;
                    U2_rx_time_cnt=0;
                    re_table2[0]=0;
                    UART2->ISR.RDA_IF=0;
                    GPIOB->DMASK=0X0000EFFF;		//dark led-COM2(PB12)
                    GPIOB->DOUT|=0X00001000;
                    GPIOB->DMASK=0X00000000;
                }
            }
            else if(U2_rx_Flag==1)//该标志位一定时间后清0 ，用于保证接收的串口数据位连续的
            {
                DrvUART_Read(UART_PORT2,re_table2,1);
                U2_re_table[U2_rx_cnt]=re_table2[0];
                ++U2_rx_cnt;

                if(U2_rx_cnt==8)//数据接收完成
                {
                    U2_rx_Flag=0;
                    U2_rx_cnt=0;
                    U2_rx_time_cnt=0;
                    Uart_tx_rx_fifo_int_FLAG|=BIT4;
					
                    GPIOB->DMASK=0X0000EFFF;		//dark led
                    GPIOB->DOUT|=0X00001000;
                    GPIOB->DMASK=0X00000000;
					
                    Dis_Uart012_THRE_int(UART_PORT2);//禁止串口中断
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
        u32IntStatus_flag=i;
        u32IntStatus_flag &= DRVUART_THREINT;
		/* Transmit Holding Register Empty Interrupt */
        if((u32IntStatus_flag==DRVUART_THREINT) && (U2_tx_Flag==1))
        {
            DrvUART_ClearIntFlag(UART_PORT2, DRVUART_THREINT_FLAG);

            re_table2[0]=U2_re_table[U2_rx_cnt];
            ++U2_rx_cnt;
            if(U2_rx_cnt<=U2_TX_byte_lenth)
            {
                DrvUART_Write(UART_PORT2,re_table2,1);//发送1byte数据
            }
            else//数据发送完成
            {
                Uart_tx_rx_fifo_int_FLAG|=BIT5;
                Dis_Uart012_THRE_int(UART_PORT2);//禁用中断

                U2_tx_Flag=U2_rx_cnt=0;
                u32delayno = 0;
                while (tUART->FSR.TE_FLAG !=1)				/* Wait Tx empty */
                {
                    u32delayno++;
                    if ( u32delayno >= 0x00400000 )
                    {
                        Set_en_buzz_finc(4,4,200,100);
                        goto time_o;
                    }
                }
time_o:
                Com2_rd_ctrl(0);//设置为接收模式
            }
        }
    }
}

//===============================================================================
//	finction	:Uart2_start_tx
//	edit		:sam 2012-10-9 10:28
//	modefy		:null
//===============================================================================
void Uart2_start_tx(void)
{
    Com2_rd_ctrl(1);//设置为发送模式
    DrvGPIO_ClrBit(E_GPB,12);//点灯LED-COM2
    DrvSYS_Delay(2000);				//delay 1ms
    U2_tx_Flag = U2_rx_cnt = 1;
    DrvUART_Write(UART_PORT2,U2_re_table,1);//发送1byte数据
    En_Uart012_THRE_int(UART_PORT2);//使能发送中断
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
//	finction	:Wait_slave_return_time
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.13
//	modefy		:null
//===============================================================================
void Wait_slave_return_time(void)
{
    if( (U_Rx_wait_time_flag_temp>=0x8000) && (U_Rx_wait_time_flag_temp<0x8bb9) )
    {
        ++U_Rx_wait_time_flag_temp;
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
//===============================================================================
void Set_tx_cmd_data_finc(int8_t slave_adds,int8_t cmd,int16_t start_adds,int16_t data_lenth,int8_t save_data_lenth)
{
    DrvSYS_Delay(6000000);		//delay 0.5s
    Uart_tx_rx_fifo_int_FLAG &= 0xf9;	//clear bit2,3

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
    if(Uart_tx_rx_fifo_int_FLAG&BIT0)//UART0 Rx Finished
    {
        Uart_tx_rx_fifo_int_FLAG&=~BIT0;
        DrvUART_Write(UART_PORT0,re_table0,1);//发送1byte数据
        En_Uart012_THRE_int(UART_PORT0);      //使能中断
        DrvGPIO_SetBit(E_GPB,13);//灭灯LED-COM0
    }
    if(Uart_tx_rx_fifo_int_FLAG&BIT1)//UART0 Tx Finished
    {
        Uart_tx_rx_fifo_int_FLAG&=~BIT1;
        DrvGPIO_SetBit(E_GPB,13);//灭灯LED-COM0
    }
    //------------------------------------UART1
    if(Uart_tx_rx_fifo_int_FLAG&BIT2)//UART2 Rx Finished
    {
        Uart_tx_rx_fifo_int_FLAG&=~BIT2;

        i=CRC16(U_data_buff,U_RX_TX_byte_lenth_cnt-2);
        j=U_data_buff[U_RX_TX_byte_lenth_cnt-1];
        j=j<<8;
        j+=U_data_buff[U_RX_TX_byte_lenth_cnt-2];

        if(j==i)//校验通过
        {
            if((U_cmd_temp==U_data_buff[1]) && (U_slave_add_temp==U_data_buff[0]))//判断从机地址与功能码
                __nop();
            else //数据错误，在显示屏显示错误信息
            {
                mm=U_data_buff[1];
                mm&=0x80;
                if(mm==0x80)					//slave module return erro message
                {
                    Get_write_struct_data(3,2,0,0,0,0);		//记录错误信息
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

        i=CRC16(U2_re_table,6);
        j=U2_re_table[7];
        j=j<<8;
        j+=U2_re_table[6];

        if(j==i)			//crc ok
        {
			if( (Pro_power_on_flag==0) && (M_lever4_extern_trrig_flag==1) ) 
            {
                if(U2_re_table[1]==0x03)		//read
                {
                    U2_re_table[1]=0x03;
                    U2_re_table[0]=Machine_note_use;
                    switch(U2_re_table[3])
                    {
                    case 0x00:
                        U2_re_table[2]=0x02;
                        U2_re_table[3]=0x00;
                        U2_re_table[4]=M_menur1_analy_obj_select_temp;//1:NH4-N 2:PO4 3:Cu 4:CN

                        i=CRC16(U2_re_table,5);//CRC
                        U2_re_table[5]=i;
                        U2_re_table[6]=i>>8;

                        U2_TX_byte_lenth=0x07;
                        break;
                    case 0x02:
                        U2_re_table[2]=0x02;
                        U2_re_table[3]=0x00;
                        U2_re_table[4]=M_menur1_unit_select_temp;//单位选择

                        i=CRC16(U2_re_table,5);
                        U2_re_table[5]=i;
                        U2_re_table[6]=i>>8;

                        U2_TX_byte_lenth=0x07;
                        break;
                    case 0x04:
                        U2_re_table[2]=0x04;

                        Read_last_result(FX_write_index_adds_point,1,2);//初始化U2_re_table中的第3位-6位
                        i=CRC16(U2_re_table,7);

                        U2_re_table[7]=i;
                        U2_re_table[8]=i>>8;
                        U2_TX_byte_lenth=0x09;
                        break;
                    case 0x06:
                        U2_re_table[2]=0x06;

                        Read_last_result(FX_write_index_adds_point,1,1);//初始化U2_re_table中的第3位-8位

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
                        Clear_n_page(0,7);
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
                else if(U2_re_table[3]==0x1d)			//stop all run finction
                {
                    if(Prog_disp_flag==1)
                    {
                        if(Menu_lever_select==4)
                            Display_lever4_index();
                        else
                            Display_menu_index();
                    }
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
                else 
					U2_TX_byte_lenth=0x08;
                Uart2_start_tx();
            }
        }
        else//CRC ERROR
        {
            U2_rx_cnt=0;
            re_table2[0]=re_table2[1]=0;
            Com2_rd_ctrl(0);
            DrvGPIO_SetBit(E_GPB,12);
        }
    }
    if(Uart_tx_rx_fifo_int_FLAG&BIT5)//UART2 Tx Finished
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
        data_buff[0]=Get_ascii_data('E');
        data_buff[1]=Get_ascii_data('R');
        data_buff[2]=Get_ascii_data('R');
        data_buff[3]=20;
        data_buff[4]=Get_ascii_data('T');
        data_buff[5]=Get_ascii_data('O');

        LCD_disp_flag=1;
        Clear_apage(6);
        Display_6x8_char(6,18,6,data_buff);

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

        Set_en_buzz_finc(4,4,200,100);
    }
erro_exit:
    __nop();
}


