//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Spi_flash_APP.c	:AT45DB321D
//	First EDIT		: Sam
//	Data			: 2012.06.14
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================
#define		__SPI_FLASH_APP__
//-------------------------------------------------------------------------------------------
#include "Incude_all.h"
//-------------------------------------------------------------------------------------------
uint16_t export_tx_lenth;//需要导出的数据总长度
uint16_t export_tx_lenth_cnt;//当前导出的数据长度
uint8_t export_tx_flag;//导出标志, 1=正在导出数据过程中; 0 = 导出数据完成/串口空闲
uint8_t export_data_select;//需要导出的数据类型(分析/校正/报警/错误)
uint32_t FX_export_index_adds_point;//分析导出开始地址:高16位保存页地址, 低9位保存该页中的下标,从0x00040000开始
uint32_t JZ_export_index_adds_point;//校正导出开始地址
uint32_t BJ_export_index_adds_point;//报警导出开始地址
uint32_t CW_export_index_adds_point;//错误导出开始地址
//-----------------------------------------
uint8_t	buff_page[512];  //用于保存从flash中读取出的一页数据
uint8_t	buff_page_w[512];//用于保存写flash的数据
uint8_t buff_page_exp[512];
uint8_t buff_page_exp_tx[512];//用于保存导出数据的缓存
uint8_t	Buff_68[512];  //用于保存6×8的字库
uint8_t	Buff_816[1024];//用于保存8×16的字库
uint8_t disp_sec_screen_flag;

uint8_t Men_del_flag;//用于标志是否擦除memory中的数据
//-----------------------------------------
uint32_t FX_write_index_adds_point;//分析写地址
uint32_t FX_read_index_adds_point; //分析读地址
uint32_t JZ_write_index_adds_point;//校正写地址
uint32_t JZ_read_index_adds_point; //校正读地址
uint32_t BJ_write_index_adds_point;//报警写地址
uint32_t BJ_read_index_adds_point; //报警读地址
uint32_t CW_write_index_adds_point;//错误写地址
uint32_t CW_read_index_adds_point; //错误读地址

uint32_t FX_disp_read_index_adds_point;//需要读取的分析记录地址
uint32_t JZ_disp_read_index_adds_point;
uint32_t BJ_disp_read_index_adds_point;
uint32_t CW_disp_read_index_adds_point;

struct Save_data {
    uint8_t  xuhao_h;//序号
    uint8_t  xuhao_l;
    uint8_t  status; //状态
    uint8_t  year_h; //年
    uint8_t  year_l;
    uint8_t  munth;  //月
    uint8_t  day;    //日
    uint8_t  hour;   //时
    uint8_t  minu;   //分
    uint8_t  result_ph;
    uint8_t  result_pl;
    uint8_t  result_nh;
    uint8_t  result_nl;
    uint8_t  unit_alarm_wrong;		//单位选择/报警，错误的序号
    uint8_t  yes_or_no;        		//结果是正常与不正常
    uint8_t  signe_result;   		//结果符号
    uint16_t disp_index;

    uint8_t  e1_result_ph;
    uint8_t  e1_result_pl;
    uint8_t  e1_result_nh;
    uint8_t  e1_result_nl;
    uint8_t  e2_result_ph;
    uint8_t  e2_result_pl;
    uint8_t  e2_result_nh;
    uint8_t  e2_result_nl;
    uint8_t  fenxi_fangfa;
    uint8_t  fenxi_obj;
} Fenxi,Jiaozheng,Cuowu,Baojing;
//===================================================================================================================
//===================================================================================================================
//===============================================================================
//	finction	:Spi_receive_byte
//	edit		:sam 2012-10-25 11:18
//	description	:发送1byte数据
//===============================================================================
void Spi_send_byte(uint8_t data)
{
    SPI1->TX[0]=data;
    SPI1->CNTRL.GO_BUSY=1;				//start send
    while(SPI1->CNTRL.GO_BUSY==1);			//wait complete
    return;
}

//===============================================================================
//	finction	:Spi_receive_byte
//	edit		:sam 2012-10-25 11:18
//	description	:接收1byte数据
//===============================================================================
uint8_t Spi_receive_byte(void)
{
    Spi_send_byte(0xFF);
    return SPI1->RX[0];
}

//===============================================================================
//	finction	:Spi_read_status
//	edit		:sam 2012-10-25 11:18
//	description	:读取FLASH状态:是否忙
//===============================================================================
uint8_t Spi_read_status(void)
{
    uint8_t result;

    DrvGPIO_ClrBit (E_GPB,2);		//CSM=low
    DrvSYS_Delay(500);

    Spi_send_byte(0xD7);
    result=Spi_receive_byte();

    DrvGPIO_SetBit (E_GPB,2);		//CSM=hight
    DrvSYS_Delay(500);

    return result;
}

//===============================================================================
//	finction	:SPI_flash_init
//	edit		:sam 2012-10-25 11:18
//	description	:初始化spi flash
//===============================================================================
void SPI_flash_init(void)
{
    uint8_t tmp,i;

    //enable SPI1 clock and reset
    SYSCLK->APBCLK.SPI1_EN=1;
    SYS->IPRSTC2.SPI1_RST=1;
    SYS->IPRSTC2.SPI1_RST=0;

    //set IO
    SYS->GPCMFP.SPI1_CLK=1;
    SYS->GPCMFP.SPI1_MISO0=1;
    SYS->GPCMFP.SPI1_MOSI0=1;

    //NVIC.ICER/ICPR,disable interrupt and clear interrupt flag
    NVIC_DisableIRQ(SPI1_IRQn);
    SPI1->CNTRL.IF = 1;

    DrvGPIO_SetBit (E_GPB,2);		//CSM=hight

    SPI1->CNTRL.TX_NEG=1;
    SPI1->CNTRL.RX_NEG=1;
    SPI1->CNTRL.TX_BIT_LEN=8;
    SPI1->CNTRL.TX_NUM=0;
    SPI1->CNTRL.LSB=0;
    SPI1->CNTRL.CLKP=0;

    *(uint32_t*)(&SPI1->DMA)=0;			//disable PDMA
    SPI1->DIVIDER.DIVIDER=2;			//7.5MHz clock

    tmp=Spi_read_status();//读状态

    i=tmp;
    i&=0x01;
    if(i==0)
    {
        //set page size to 512 byte
        DrvGPIO_ClrBit (E_GPB,2);		//CSM=low
        DrvSYS_Delay(500);
        Spi_send_byte(0x3D);
        Spi_send_byte(0x2A);
        Spi_send_byte(0x80);
        Spi_send_byte(0xA6);
        DrvGPIO_SetBit (E_GPB,2);		//CSM=hight
        DrvSYS_Delay(500);

        //power_down
        DrvGPIO_ClrBit (E_GPB,2);		//CSM=low
        DrvSYS_Delay(50000);
        Spi_send_byte(0xB9);
        DrvGPIO_SetBit (E_GPB,2);		//CSM=hight
        DrvSYS_Delay(500);

        //power_up
        DrvGPIO_ClrBit (E_GPB,2);		//CSM=low
        DrvSYS_Delay(500);
        Spi_send_byte(0xAB);
        DrvGPIO_SetBit (E_GPB,2);		//CSM=hight
        DrvSYS_Delay(500);
    }
    i=tmp;
    i&=0x02;
    if(i==0x02)
    {
        //disable protec
        DrvGPIO_ClrBit (E_GPB,2);		//CSM=low
        DrvSYS_Delay(500);
        Spi_send_byte(0x3D);
        Spi_send_byte(0x2A);
        Spi_send_byte(0x7F);
        Spi_send_byte(0x9A);
        DrvGPIO_SetBit (E_GPB,2);		//CSM=hight
        DrvSYS_Delay(500);
    }
}

//===============================================================================
//	finction	:Spi_ready
//	edit		:sam 2012-10-25 11:18
//	description	:用于检测flash是否擦写完成
//===============================================================================
uint8_t Spi_ready(void)
{
    if((Spi_read_status()&(1<<BUSY))==0)
        return 0;
    else
        return 1;
}

//===============================================================================
//	finction	:Spi_erase
//	edit		:sam 2012-10-25 11:18
//	description	:未使用到的驱动函数, 擦除SPI FLASH
//===============================================================================
void Spi_erase(void)
{
    uint16_t i;

    i=500;
    do {
        if(Spi_ready()==1)
            break;
        DrvSYS_Delay(1000);
        i--;
    } while(i>0);

    DrvGPIO_ClrBit (E_GPB,2);		//CSM=low
    DrvSYS_Delay(500);
    Spi_send_byte(0xC7);
    Spi_send_byte(0x94);
    Spi_send_byte(0x80);
    Spi_send_byte(0x9A);
    DrvGPIO_SetBit (E_GPB,2);		//CSM=hight
    DrvSYS_Delay(500);

    //time out 500ms max
    i=500;
    do {
        if(Spi_ready()==1)
            break;
        DrvSYS_Delay(1000);
        i--;
    } while(i>0);
}

//===============================================================================
//	finction	:Spi_Page_Write
//	edit		:sam 2012-10-25 11:18
//	description	:按页写
//===============================================================================
void Spi_Page_Write(uint32_t page_address,uint8_t *buf)
{
    uint16_t i;

    //time out 50ms max
    i=500;
    do {
        if(Spi_ready()==1)
            break;
        DrvSYS_Delay(1000);
        i--;
    } while(i>0);
    //write to buffer
    DrvGPIO_ClrBit (E_GPB,2);		//CSM=low
    DrvSYS_Delay(500);

    Spi_send_byte(0x84);
    Spi_send_byte(0x00);
    Spi_send_byte(0x00);
    Spi_send_byte(0x00);

    for(i=0; i<512; i++)//写一页的数据到spi flash
    {
        Spi_send_byte(*(buf+i));
    }

    DrvGPIO_SetBit (E_GPB,2);		//CSM=hight
    DrvSYS_Delay(500);

    //buffer to memory
    DrvGPIO_ClrBit (E_GPB,2);		//CSM=low
    DrvSYS_Delay(500);

    Spi_send_byte(0x83);
    Spi_send_byte((page_address>>7)&0xFF);
    Spi_send_byte((page_address<<1)&0xFE);
    Spi_send_byte(0x00);

    DrvGPIO_SetBit (E_GPB,2);		//CSM=hight
}

//===============================================================================
//	finction	:Spi_Page_Read
//	edit		:sam 2012-10-25 11:18
//	modefy		:null
//===============================================================================
void Spi_Page_Read(uint32_t page_address,uint8_t *buf)
{
    uint16_t i;

    //time out 50ms max
    i=500;
    do {
        if(Spi_ready()==1)
            break;
        DrvSYS_Delay(1000);
        i--;
    } while(i>0);

    //write to buffer
    DrvGPIO_ClrBit (E_GPB,2);		//CSM=low
    DrvSYS_Delay(500);

    Spi_send_byte(0x03);
    Spi_send_byte((page_address>>7)&0xFF);
    Spi_send_byte((page_address<<1)&0xFE);
    Spi_send_byte(0x00);

    for(i=0; i<512; i++)
    {
        *(buf+i)=Spi_receive_byte();
    }

    DrvGPIO_SetBit (E_GPB,2);		//CSM=hight
}

//===================================================================================================================
//===================================================================================================================
//===============================================================================
//	finction	:Get_write_adds_struct_data
//	input		:select: 1-分析  2-校正  3-错误  4-报警
//				:status:状态
//              :Sresult:结果
//              :unit:单位
//              :YN:yes_or_no
//              :signe:符号
//	modefy		:将参数中的值写入Save_data结构体中
//===============================================================================
void Get_write_struct_data(uint8_t selset,uint8_t status,uint32_t Sresult,uint8_t unit,uint8_t YN,uint8_t signe)
{
    uint8_t i;

    switch(selset)
    {
    case 1://分析
		//序号自增
        if(Fenxi.xuhao_l==0xff)//序号低位达到最大值, 高位进位, 低位清零
        {
            Fenxi.xuhao_l=0;
            Fenxi.xuhao_h+=1;
        }
        else
            Fenxi.xuhao_l+=1;
		
        Fenxi.status=status;

        i=RTC_DATA.mouth_temp;
        i&=0x80;
        if(i==0x80)
            Fenxi.year_h=0x19;
        else
            Fenxi.year_h=0x20;

        Fenxi.year_l=RTC_DATA.year_temp;
        Fenxi.munth=RTC_DATA.mouth_temp;
        Fenxi.day=RTC_DATA.day_temp;
        Fenxi.hour=RTC_DATA.hour_temp;
        Fenxi.minu=RTC_DATA.minu_temp;

        Fenxi.year_l&=0x3f;
        Fenxi.munth&=0x1f;
        Fenxi.day&=0x3f;
        Fenxi.hour&=0x3f;
        Fenxi.minu&=0x7f;

        Fenxi.result_ph=Sresult>>24;
        Fenxi.result_pl=Sresult>>16;
        Fenxi.result_nh=Sresult>>8;
        Fenxi.result_nl=Sresult;

        Fenxi.unit_alarm_wrong = unit;
        Fenxi.yes_or_no=YN;
        Fenxi.signe_result=signe;

        Fenxi.e1_result_ph=e1_men_save>>24;
        Fenxi.e1_result_pl=e1_men_save>>16;
        Fenxi.e1_result_nh=e1_men_save>>8;
        Fenxi.e1_result_nl=e1_men_save;
        Fenxi.e2_result_ph=e2_men_save>>24;
        Fenxi.e2_result_pl=e2_men_save>>16;
        Fenxi.e2_result_nh=e2_men_save>>8;
        Fenxi.e2_result_nl=e2_men_save;
        Fenxi.fenxi_fangfa=M_menur1_LZDJ_select_temp;
        Fenxi.fenxi_obj=M_menur1_analy_obj_select_temp;
        break;
    case 2://校正
        if(Jiaozheng.xuhao_l==0xff)
        {
            Jiaozheng.xuhao_l=0;
            Jiaozheng.xuhao_h+=1;
        }
        else
            Jiaozheng.xuhao_l+=1;

        Jiaozheng.status=status;

        i=RTC_DATA.mouth_temp;
        i&=0x80;
        if(i==0x80)
            Jiaozheng.year_h=0x19;
        else
            Jiaozheng.year_h=0x20;

        Jiaozheng.year_l=RTC_DATA.year_temp;
        Jiaozheng.munth=RTC_DATA.mouth_temp;
        Jiaozheng.day=RTC_DATA.day_temp;
        Jiaozheng.hour=RTC_DATA.hour_temp;
        Jiaozheng.minu=RTC_DATA.minu_temp;

        Jiaozheng.year_l&=0x3f;
        Jiaozheng.munth&=0x1f;
        Jiaozheng.day&=0x3f;
        Jiaozheng.hour&=0x3f;
        Jiaozheng.minu&=0x7f;

        Jiaozheng.result_ph=Sresult>>24;
        Jiaozheng.result_pl=Sresult>>16;
        Jiaozheng.result_nh=Sresult>>8;
        Jiaozheng.result_nl=Sresult;

        Jiaozheng.unit_alarm_wrong=unit;
        Jiaozheng.yes_or_no=YN;
        Jiaozheng.signe_result=signe;

        Jiaozheng.e1_result_ph=e1_men_save>>24;
        Jiaozheng.e1_result_pl=e1_men_save>>16;
        Jiaozheng.e1_result_nh=e1_men_save>>8;
        Jiaozheng.e1_result_nl=e1_men_save;
        Jiaozheng.e2_result_ph=e2_men_save>>24;
        Jiaozheng.e2_result_pl=e2_men_save>>16;
        Jiaozheng.e2_result_nh=e2_men_save>>8;
        Jiaozheng.e2_result_nl=e2_men_save;
        Jiaozheng.fenxi_fangfa=M_menur1_LZDJ_select_temp;
        Jiaozheng.fenxi_obj=M_menur1_analy_obj_select_temp;
        break;
    case 3://错误
        if(Cuowu.xuhao_l==0xff)
        {
            Cuowu.xuhao_l=0;
            Cuowu.xuhao_h+=1;
        }
        else
            Cuowu.xuhao_l+=1;

        Cuowu.status=status;

        i=RTC_DATA.mouth_temp;
        i&=0x80;
        if(i==0x80)
            Cuowu.year_h=0x19;
        else
            Cuowu.year_h=0x20;

        Cuowu.year_l=RTC_DATA.year_temp;
        Cuowu.munth=RTC_DATA.mouth_temp;
        Cuowu.day=RTC_DATA.day_temp;
        Cuowu.hour=RTC_DATA.hour_temp;
        Cuowu.minu=RTC_DATA.minu_temp;

        Cuowu.year_l&=0x3f;
        Cuowu.munth&=0x1f;
        Cuowu.day&=0x3f;
        Cuowu.hour&=0x3f;
        Cuowu.minu&=0x7f;

//----------------------------------------------- 2013-5-14 11:57
        Cuowu.result_ph=U_data_buff[0];
        Cuowu.result_pl=U_data_buff[1];
        Cuowu.result_nh=U_start_adds_temp>>8;
        Cuowu.result_nl=U_start_adds_temp;

        i=status;
        i<<=4;
        i|=Rx_TX_flag;
        Cuowu.unit_alarm_wrong=i;

        Cuowu.yes_or_no=M_menur1_slope_flag;
        Cuowu.signe_result=signe;

        Cuowu.e1_result_ph=M_menur1_slope_temp>>24;
        Cuowu.e1_result_pl=M_menur1_slope_temp>>16;
        Cuowu.e1_result_nh=M_menur1_slope_temp>>8;
        Cuowu.e1_result_nl=M_menur1_slope_temp;
        Cuowu.e2_result_ph=0;
        Cuowu.e2_result_pl=0;
        Cuowu.e2_result_nh=0;
        Cuowu.e2_result_nl=0;
        Cuowu.fenxi_fangfa=M_menur1_LZDJ_select_temp;
        Cuowu.fenxi_obj=M_menur1_analy_obj_select_temp;
        break;
    case 4://报警
        if(Baojing.xuhao_l==0xff)
        {
            Baojing.xuhao_l=0;
            Baojing.xuhao_h+=1;
        }
        else
            Baojing.xuhao_l+=1;

        Baojing.status=status;

        i=RTC_DATA.mouth_temp;
        i&=0x80;
        if(i==0x80)
            Baojing.year_h=0x19;
        else
            Baojing.year_h=0x20;

        Baojing.year_l=RTC_DATA.year_temp;
        Baojing.munth=RTC_DATA.mouth_temp;
        Baojing.day=RTC_DATA.day_temp;
        Baojing.hour=RTC_DATA.hour_temp;
        Baojing.minu=RTC_DATA.minu_temp;

        Baojing.year_l&=0x3f;
        Baojing.munth&=0x1f;
        Baojing.day&=0x3f;
        Baojing.hour&=0x3f;
        Baojing.minu&=0x7f;

        Baojing.result_ph=Sresult>>24;
        Baojing.result_pl=Sresult>>16;
        Baojing.result_nh=Sresult>>8;
        Baojing.result_nl=Sresult;

        Baojing.unit_alarm_wrong=unit;
        Baojing.yes_or_no=YN;
        Baojing.signe_result=signe;

        Baojing.e1_result_ph=e1_men_save>>24;
        Baojing.e1_result_pl=e1_men_save>>16;
        Baojing.e1_result_nh=e1_men_save>>8;
        Baojing.e1_result_nl=e1_men_save;
        Baojing.e2_result_ph=e2_men_save>>24;
        Baojing.e2_result_pl=e2_men_save>>16;
        Baojing.e2_result_nh=e2_men_save>>8;
        Baojing.e2_result_nl=e2_men_save;
        Baojing.fenxi_fangfa=M_menur1_LZDJ_select_temp;
        Baojing.fenxi_obj=M_menur1_analy_obj_select_temp;
        break;
    }
}

//===============================================================================
//	finction	:Disp_sec_screen_data
//	input		:Save_data *point 数据结构
//	input		:null
//	edit		:sam 2012-8-20 16:23
//	description	:显示分析,校正,报警,错误历史记录
//===============================================================================
void Disp_sec_screen_data(struct Save_data *point)
{
    uint8_t	data_buff[16],j;
    uint32_t disp_d = 0;

    data_buff[0]='E';
    data_buff[1]='1';
    data_buff[2]=20;
    data_buff[3]=0;
    data_buff[11]=0;
    data_buff[12]='M';
    data_buff[13]='V';

    disp_d+=((point->e1_result_ph)<<24);
    disp_d+=((point->e1_result_pl)<<16);
    disp_d+=((point->e1_result_nh)<<8);
    disp_d+=point->e1_result_nl;

    if(point->fenxi_fangfa!=2)//根据不同的分析方法计算disp_d的值
    {
        if(disp_d<31250)
        {
            data_buff[3]=14;	//-
            disp_d=31250-disp_d;
        }
        else
        {
            data_buff[3]=0;		//+
            disp_d=disp_d-31250;
        }
        disp_d=Float_to_int_reg_ee2(disp_d);
    }
    else
        disp_d=Float_to_int_reg_ee(disp_d);
	/*--------------------------------*/
    j=(disp_d>>28) & 0xf;
    if(j>=0x0a)
        j=0;
	/*--------------------------------*/
    if(j==0)
        data_buff[4]=0;
    else
        data_buff[4]=Get_ascii_data(j);
	/*--------------------------------*/
    j=(disp_d>>24)&0xf;
    if((j==0)&&(data_buff[4]==0))
        data_buff[5]=0;
    else
        data_buff[5]=Get_ascii_data(j);
	/*--------------------------------*/
    j=(disp_d>>20)&0xf;
    if((j==0)&&(data_buff[4]==0)&&(data_buff[5]==0))
        data_buff[6]=0;
    else
        data_buff[6]=Get_ascii_data(j);
	/*--------------------------------*/
    data_buff[7]=Get_ascii_data((disp_d>>16) & 0xf);
    data_buff[8]=19;
    data_buff[9]=Get_ascii_data((disp_d>>12) & 0xf);
    data_buff[10]=Get_ascii_data((disp_d>>8)&0xf);

    for(j=0; j<14; j++)
        Get_6x8_char_single(5,j*6,data_buff[j]);		//line5

//-----------------------------------------------------------
    
    data_buff[0]='E';
    data_buff[1]='2';
    data_buff[2]=20;
    data_buff[3]=0;
    data_buff[11]=0;
    data_buff[12]='M';
    data_buff[13]='V';
	
    disp_d = ((point->e2_result_ph)<<24);
    disp_d+=((point->e2_result_pl)<<16);
    disp_d+=((point->e2_result_nh)<<8);
    disp_d+=point->e2_result_nl;

    if(point->fenxi_fangfa!=2)
    {
        if(disp_d<31250)
        {
            data_buff[3]=14;	//-
            disp_d=31250-disp_d;
        }
        else
        {
            data_buff[3]=0;		//+
            disp_d=disp_d-31250;
        }
        disp_d=Float_to_int_reg_ee2(disp_d);
    }
    else
        disp_d=Float_to_int_reg_ee(disp_d);
	/*--------------------------------*/
    j=(disp_d>>28)&0xf;
    if(j>=0x0a)
        j=0;

    if(j==0)
        data_buff[4]=0;
    else
        data_buff[4]=Get_ascii_data(j);
	/*--------------------------------*/
    j=(disp_d>>24)&0xf;
    if((j==0)&&(data_buff[4]==0))
        data_buff[5]=0;
    else
        data_buff[5]=Get_ascii_data(j);
	/*--------------------------------*/
    j=(disp_d>>20) & 0xf;
    if((j==0)&&(data_buff[4]==0)&&(data_buff[5]==0))
        data_buff[6]=0;
    else
        data_buff[6]=Get_ascii_data(j);
	/*--------------------------------*/
    j=(disp_d>>16)&0xf;
    data_buff[7]=Get_ascii_data(j);
    data_buff[8]=19;
    data_buff[9]=Get_ascii_data((disp_d>>12)&0xf);
    data_buff[10]=Get_ascii_data((disp_d>>8)&0xf);
    for(j=0; j<14; j++)
        Get_6x8_char_single(6,j*6,data_buff[j]);		//line6
}

//===============================================================================
//	finction	:Disp_history_data
//      Detials		:显示历史记录
//	input		:select  1=分析，2=校正，3=错误，4=报警
//                      :Save_data *point 数据结构
//	edit		:sam 2012-8-7 9:42
//	description	:
//===============================================================================
void Disp_history_data(uint8_t select,struct Save_data *point)
{
    uint32_t	res;
    uint8_t	data_buff[22],j,k;

    if(system_init_flag!=1)
    {
        LCD_disp_flag=1;
        Clear_n_page_ram(0,7);//清屏
		
        data_buff[0]=Get_ascii_data(point->disp_index/10000%10);
        data_buff[1]=Get_ascii_data(point->disp_index/1000%10);
        data_buff[2]=Get_ascii_data(point->disp_index/100%10);
        data_buff[3]=Get_ascii_data(point->disp_index/10%10);
        data_buff[4]=Get_ascii_data(point->disp_index%10);
        data_buff[5]=20;

        if((select==1)||(select==2))
        {
            if(select==2)	//校正
            {
                j=point->status;
                if(M_menur1_language_select_temp==2)
                {
                    if(j==6)
                    {
                        data_buff[6]=209;				//hand 	//维护
                        data_buff[7]=210;
                        Disp_buff128x64[84]=0x38;	//(
                        Disp_buff128x64[85]=0x44;
                        Disp_buff128x64[86]=0x82;
                        Get_6x8_char_single(0,87,'M');
                        Disp_buff128x64[93]=0x82; //)
                        Disp_buff128x64[94]=0x44;
                        Disp_buff128x64[95]=0x38;
                    }
                    else
                    {
                        data_buff[6]=37;				//auto	//校验
                        data_buff[7]=147;

                        Disp_buff128x64[84]=0x38;	//(
                        Disp_buff128x64[85]=0x44;
                        Disp_buff128x64[86]=0x82;
                        Get_6x8_char_single(0,87,'C');
                        Disp_buff128x64[93]=0x82; //)
                        Disp_buff128x64[94]=0x44;
                        Disp_buff128x64[95]=0x38;
                    }

                    for(j=0; j<6; j++)
                        Get_6x8_char_single(0,j*6,data_buff[j]);		//line1
                    for(j=0; j<2; j++)
                        Get_16x16_char_single(0,j*12+36+24,data_buff[j+6]); 		//line1

                    for(j=0; j<128; j++)				 	//line2
                        Disp_buff128x64[128+j]|=0x80;
                    for(j=0; j<36; j++)
                    {
                        k=Disp_buff128x64[j];
                        Disp_buff128x64[j]<<=6;
                        k>>=2;
                        Disp_buff128x64[128+j]|=k;
                    }
                }
            }
            else  //分析
            {
                j=point->status;
                switch(j)
                {
                case 1:	//hand fx
                    if(M_menur1_language_select_temp==2)
                    {
                        data_buff[6]=209;				//hand 	//维护
                        data_buff[7]=210;
                        data_buff[8]=16;
                        data_buff[9]=17;

                        Disp_buff128x64[84]=0x38;	//(
                        Disp_buff128x64[85]=0x44;
                        Disp_buff128x64[86]=0x82;
                        Get_6x8_char_single(0,87,'M');
                        Disp_buff128x64[93]=0x82; //)
                        Disp_buff128x64[94]=0x44;
                        Disp_buff128x64[95]=0x38;
                    }
                    break;
                case 2:	//hand hc
                    if(M_menur1_language_select_temp==2)
                    {
                        data_buff[6]=209;				//hand 	//维护
                        data_buff[7]=210;
                        data_buff[8]=39;
                        data_buff[9]=40;
                        Disp_buff128x64[84]=0x38;	//(
                        Disp_buff128x64[85]=0x44;
                        Disp_buff128x64[86]=0x82;
                        Get_6x8_char_single(0,87,'M');
                        Disp_buff128x64[93]=0x82; //)
                        Disp_buff128x64[94]=0x44;
                        Disp_buff128x64[95]=0x38;
                    }
                    break;
                case 3:	//auto fx
                    if(M_menur1_language_select_temp==2)
                    {
                        data_buff[6]=35;
                        data_buff[7]=36;
                        data_buff[8]=16;
                        data_buff[9]=17;
                    }
                    break;
                case 4:	//auto hc
                    if(M_menur1_language_select_temp==2)
                    {
                        data_buff[6]=41;		//标样核查
                        data_buff[7]=55;
                        data_buff[8]=39;
                        data_buff[9]=40;
                        for(j=0; j<2; j++)
                            Get_16x16_char_single(0,j*12+36+48,data_buff[j+8]); 	//line1
                        Disp_buff128x64[108]=0x38;	//(
                        Disp_buff128x64[109]=0x44;
                        Disp_buff128x64[110]=0x82;
                        Get_6x8_char_single(0,111,'S');
                        Get_6x8_char_single(0,117,'C');
                        Disp_buff128x64[123]=0x82; //)
                        Disp_buff128x64[124]=0x44;
                        Disp_buff128x64[125]=0x38;
                    }
                    break;
                case 5:	//auto xs
                    if(M_menur1_language_select_temp==2)
                    {
                        data_buff[6]=35;
                        data_buff[7]=36;
                        data_buff[8]=67;
                        data_buff[9]=68;
                    }
                    break;
                case 6:	//hand jz
                    if(M_menur1_language_select_temp==2)
                    {
                        data_buff[6]=37;	//校验
                        data_buff[7]=147;
                        data_buff[8]=37;
                        data_buff[9]=38;

                        Disp_buff128x64[84]=0x38;	//(
                        Disp_buff128x64[85]=0x44;
                        Disp_buff128x64[86]=0x82;
                        Get_6x8_char_single(0,87,'C');
                        Disp_buff128x64[93]=0x82; //)
                        Disp_buff128x64[94]=0x44;
                        Disp_buff128x64[95]=0x38;
                    }
                    break;
                case 0:
                    data_buff[6]=0;
                    data_buff[7]=0;
                    data_buff[8]=0;
                    data_buff[9]=0;
                    data_buff[10]=0;
                    data_buff[11]=0;
                    data_buff[12]=0;
                    data_buff[13]=0;
                    break;
                }

                if(M_menur1_language_select_temp==2)
                {
                    for(j=0; j<6; j++)
                        Get_6x8_char_single(0,j*6,data_buff[j]);		//line1

                    for(j=0; j<2; j++)
                        Get_16x16_char_single(0,j*12+36+24,data_buff[j+6]); 	//line1

                    for(j=0; j<128; j++)				 	//line2
                        Disp_buff128x64[128+j]|=0x80;
                    for(j=0; j<36; j++)
                    {
                        k=Disp_buff128x64[j];
                        Disp_buff128x64[j]<<=6;
                        k>>=2;
                        Disp_buff128x64[128+j]|=k;
                    }
                }
            }

            //---------------------------------------------------------
            data_buff[9]=Get_ascii_data((point->year_h>>4) & 0xf);
            data_buff[10]=Get_ascii_data((point->year_h) & 0xf);
            data_buff[11]=Get_ascii_data((point->year_l>>4)&0xf);
            data_buff[12]=Get_ascii_data(point->year_l&0xf);
            data_buff[13]=14;
            data_buff[14]=Get_ascii_data((point->munth>>4) & 0x1);
            data_buff[15]=Get_ascii_data(point->munth&0xf);
            data_buff[16]=14;
            data_buff[17]=Get_ascii_data((point->day>>4)&0x3);
            data_buff[18]=Get_ascii_data(point->day&0xf);
            data_buff[5]=data_buff[6]=data_buff[7]=data_buff[8]=0;
            data_buff[0]=Get_ascii_data((point->hour>>4)&0x3);
            data_buff[1]=Get_ascii_data(point->hour&0xf);
            data_buff[2]=20;
            data_buff[3]=Get_ascii_data((point->minu>>4)&0x7);
            data_buff[4]=Get_ascii_data(point->minu&0xf);
            for(j=0; j<19; j++)
                Get_6x8_char_single(2,j*6,data_buff[j]);		//line3

//---------------------------------------------------------

            data_buff[0]=0;
            data_buff[1]=0;
            data_buff[2]=0;
            data_buff[3]=0;
            res=(point->result_ph)<<24;
            res|=(point->result_pl)<<16;
            res|=(point->result_nh)<<8;
            res|=(point->result_nl);

            j=(point->result_ph>>4)&0xf;
            if(j==0)
                data_buff[4]=0;
            else
                data_buff[4]=Get_ascii_data(j);
			
            j=point->result_ph & 0xf;
            if((j==0)&&(data_buff[4]==0))
                data_buff[5]=0;
            else
                data_buff[5]=Get_ascii_data(j);
			
            j=(point->result_pl >> 4)& 0xf;
            if((j==0)&&(data_buff[4]==0)&&(data_buff[5]==0))
                data_buff[6]=0;
            else
                data_buff[6]=Get_ascii_data(j);
			
            data_buff[7]=Get_ascii_data(point->result_pl & 0xf);
            data_buff[8]=19;
            data_buff[9]=Get_ascii_data((point->result_nh>>4)&0xf);
            data_buff[10]=Get_ascii_data(point->result_nh&0xf);
            data_buff[11]=Get_ascii_data((point->result_nl>>4)&0xf);
            data_buff[12]=Get_ascii_data(point->result_nl&0xf);

            if(select==2)
            {
                data_buff[13]=0;
                data_buff[14]=0;
                data_buff[15]=0;
            }
            else
            {
                data_buff[13]='M';
                data_buff[14]='G';
                data_buff[15]=18;
                data_buff[16]='L';

                j=point->unit_alarm_wrong;
                switch(j)
                {
                case 1:
                    data_buff[13]='G';
                    data_buff[14]=18;
                    data_buff[15]='L';
                    data_buff[16]=0;
                    break;
                case 3:
                    data_buff[13]='U';
                    break;
                case 4:
                    data_buff[13]=0;
                    data_buff[14]='M';
                    data_buff[15]='V';
                    data_buff[16]=0;
                    break;
                }
            }
            for(j=0; j<4; j++)
                Get_6x8_char_single(4,j*6,data_buff[j]);

            k=0;
            if(data_buff[4]==0)
            {
                ++k;
                if(data_buff[5]==0)
                {
                    ++k;
                    if(data_buff[6]==0)
                        ++k;
                }
            }

            Get_6x8_char_single(4,64-k*10,data_buff[8]);

            for(j=0; j<4; j++)
                Get_6x8_char_single(4,106+j*6,data_buff[13+j]);

            for(j=0; j<4-k; j++)
                Get_8x16_char_single(3,24+j*10,data_buff[j+4+k]);
            for(j=0; j<4; j++)
                Get_8x16_char_single(3,(70+j*10)-k*10,data_buff[j+9]);

            j=point->signe_result;
            if((j!=1)&&(res!=0))
            {
                for(k=0; k<5; k++)
                    Disp_buff128x64[4*128+18+k]=0x03;
            }
            //---------------------------------------------------------
            j=point->yes_or_no;
//
            data_buff[0]=0;
            data_buff[1]=0;
            data_buff[2]=0;
            data_buff[3]=0;
            data_buff[4]=0;
            data_buff[5]=0;
            data_buff[6]=0;
            data_buff[7]=0;
            data_buff[8]=0;
            data_buff[9]=0;
            data_buff[10]=0;

            for(j=0; j<11; j++)
                Get_6x8_char_single(7,j*6,data_buff[j]);

            data_buff[0]='D';
            data_buff[1]='E';
            data_buff[2]='L';
            data_buff[3]=20;
            data_buff[4]='N';
            data_buff[5]=18;
            data_buff[6]='Y';

            for(j=0; j<7; j++)
                Get_6x8_char_single(7,j*6+86,data_buff[j]);

            for(j=1; j<8; j++)
                Disp_buff128x64[128*7+108+j]=~Disp_buff128x64[128*7+108+j];

            Men_del_flag=0;
//-------------------------------------------------------------------------
            switch(Menu_index_finc_select)
            {
            case 1:
                Disp_sec_screen_data(&Fenxi);//分析
                break;
            case 2:
                Disp_sec_screen_data(&Jiaozheng);//校正
                break;
            case 3:
                Disp_sec_screen_data(&Baojing);//报警
                break;
            case 4:
                Disp_sec_screen_data(&Cuowu);//错误
                break;
            }
        }
        else
        {
            for(j=0; j<6; j++)
                Get_6x8_char_single(0,j*6,data_buff[j]);		//line1

            //---------------------------------------------------------
			data_buff[0]=Get_ascii_data((point->hour>>4) & 0x03);
            data_buff[1]=Get_ascii_data(point->hour & 0xf);
            data_buff[2]=20;
            data_buff[3]=Get_ascii_data((point->minu>>4)&0x00);
            data_buff[4]=Get_ascii_data(point->minu&0x00);
            data_buff[5]=data_buff[6]=data_buff[7]=data_buff[8]=0;
			data_buff[9]=Get_ascii_data((point->year_h>>4) & 0xf);
            data_buff[10]=Get_ascii_data((point->year_h) & 0xf);
            data_buff[11]=Get_ascii_data((point->year_l>>4)&0xf);
            data_buff[12]=Get_ascii_data(point->year_l&0xf);
            data_buff[13]=14;
            data_buff[14]=Get_ascii_data((point->munth>>4) & 0x1);
            data_buff[15]=Get_ascii_data(point->munth&0xf);
            data_buff[16]=14;
            data_buff[17]=Get_ascii_data((point->day>>4)&0x3);
            data_buff[18]=Get_ascii_data(point->day&0xf);

            for(j=0; j<19; j++)
                Get_6x8_char_single(2,j*6,data_buff[j]);		//line3
//---------------------------------------------------------
            data_buff[0]='R';
            data_buff[1]='S';
            data_buff[2]=20;
            if(select==3)				//error
            {
                if(M_menur1_language_select_temp==2)
                {
                    data_buff[3]=211;		//故障
                    data_buff[4]=212;
                    Disp_buff128x64[84]=0x38;	//(
                    Disp_buff128x64[85]=0x44;
                    Disp_buff128x64[86]=0x82;
                    Get_6x8_char_single(0,87,'D');
                    Disp_buff128x64[93]=0x82; //)
                    Disp_buff128x64[94]=0x44;
                    Disp_buff128x64[95]=0x38;
                }
            }
            else					//alarm
            {
                if(M_menur1_language_select_temp==2)
                {
                    data_buff[3]=89;
                    data_buff[4]=90;
                }
            }
            data_buff[8]=0;
            data_buff[9]=Get_ascii_data((point->unit_alarm_wrong>>8)&0xf);
            data_buff[10]=Get_ascii_data((point->unit_alarm_wrong>>4)&0xf);
            data_buff[11]=Get_ascii_data(point->unit_alarm_wrong&0xf);

            if(M_menur1_language_select_temp==2)
            {
                for(j=0; j<2; j++)
                {
                    Get_16x16_char_single(0,j*12+30+24,data_buff[j+3]); 	//line1
                    Get_16x16_char_single(4,j*12+18,data_buff[j+3]); 	//line4
                }
                for(j=0; j<3; j++)
                    Get_8x16_char_single(4,60+j*10,data_buff[j+9]);

                for(j=0; j<128; j++)				 	//line2
                    Disp_buff128x64[128+j]|=0x80;

                for(j=0; j<36; j++)
                {
                    k=Disp_buff128x64[j];
                    Disp_buff128x64[j]<<=6;
                    k>>=2;
                    Disp_buff128x64[128+j]|=k;
                }
                for(j=0; j<3; j++)
                    Get_6x8_char_single(4,j*6,data_buff[j]);
            }
            else
            {
                for(j=0; j<6; j++)
                    Get_6x8_char_single(0,j*6+36,data_buff[j+3]);		//line1

                for(j=0; j<9; j++)
                    Get_8x16_char_single(4,18+j*10,data_buff[j+3]);

                for(j=0; j<3; j++)
                    Get_6x8_char_single(5,j*6,data_buff[j]);

                for(j=0; j<128; j++)				 	//line2
                    Disp_buff128x64[128+j]|=0x10;
            }
//---------------------------------------------------------

            data_buff[0]='D';
            data_buff[1]='E';
            data_buff[2]='L';
            data_buff[3]=20;
            data_buff[4]='N';
            data_buff[5]=18;
            data_buff[6]='Y';
            for(j=0; j<7; j++)
                Get_6x8_char_single(7,j*6+86,data_buff[j]);

            for(j=1; j<8; j++)
                Disp_buff128x64[128*7+108+j]=~Disp_buff128x64[128*7+108+j];

            Men_del_flag=0;
        }
        Display_all_buff();
    }
}

//===============================================================================
//	finction	:Write_index_adds_point
//	edit		:sam 2012-8-17 10:25
//	description	:初始化buff_page_w的值,并写入flash
//              :buff_page_w中保存了分析/校正/错误/报警 历史记录的读写地址
//===============================================================================
void Write_index_adds_point(void)
{
    uint32_t	k,adds_a,adds_b;;

    Buff_get_reg_32bit_data(0,0xCCCCCCCC);               //buff_page_w 0-3
    Buff_get_reg_32bit_data(4,FX_write_index_adds_point);//buff_page_w 4-7

    adds_b=FX_write_index_adds_point & 0x000001ff;
    adds_a=(FX_write_index_adds_point >> 16) & 0xffff;//page adds
	
    if(adds_b==0x00000000)
    {
        adds_b=0x000001e0;
        adds_a-=1;
    }
    else
        adds_b-=0x20;

    adds_a<<=16;
    FX_read_index_adds_point = adds_a + adds_b;
    Buff_get_reg_32bit_data(8,FX_read_index_adds_point);//buff_page_w 8-11
//----------------------
    Buff_get_reg_32bit_data(12,JZ_write_index_adds_point);
    adds_a=adds_b=JZ_write_index_adds_point;
    adds_b = JZ_write_index_adds_point & 0x000001ff;
    adds_a = (JZ_write_index_adds_point >> 16) & 0xffff;//page adds
    if(adds_b==0x00000000)
    {
        adds_b=0x000001e0;
        adds_a-=1;
    }
    else
        adds_b-=0x20;

    adds_a<<=16;
    JZ_read_index_adds_point=adds_a+adds_b;
    Buff_get_reg_32bit_data(16,JZ_read_index_adds_point);
//-----------------------
    Buff_get_reg_32bit_data(20,CW_write_index_adds_point);
    adds_a=adds_b=CW_write_index_adds_point;
    adds_b&=0x000001ff;
    adds_a>>=16;
    adds_a&=0x0000ffff;				//page adds
    if(adds_b==0x00000000)
    {
        adds_b=0x000001e0;
        adds_a-=1;
    }
    else
        adds_b-=0x20;

    adds_a<<=16;
    CW_read_index_adds_point=adds_a+adds_b;
    Buff_get_reg_32bit_data(24,CW_read_index_adds_point);
//---------------------
    Buff_get_reg_32bit_data(28,BJ_write_index_adds_point);
    adds_a=adds_b=BJ_write_index_adds_point;
    adds_b&=0x000001ff;
    adds_a>>=16;
    adds_a&=0x0000ffff;				//page adds
    if(adds_b==0x00000000)
    {
        adds_b=0x000001e0;
        adds_a-=1;
    }
    else
        adds_b-=0x20;

    adds_a<<=16;
    BJ_read_index_adds_point=adds_a+adds_b;
    Buff_get_reg_32bit_data(32,BJ_read_index_adds_point);

    k=Float_to_int_reg(k2_prog_use);
    Buff_get_reg_32bit_data(36,k);
    k=Float_to_int_reg(j_prog_use);
    Buff_get_reg_32bit_data(40,k);
    k=Float_to_int_reg(k1_prog_use);
    Buff_get_reg_32bit_data(44,k);
    buff_page_w[49]=k2_signe_flag;

    Spi_Page_Write(FIRST_REG_PAGE,buff_page_w);					//write system reg page0
    DrvSYS_Delay(50000);
}

//===============================================================================
//	finction	:Write_result_to_flah
//  return      :地址
//	edit		:sam 2012-10-25 16:44
//	description	:将Save_data中的数据,写入Flash当中
//===============================================================================
uint32_t Write_result_to_flash(uint32_t adds_temp,struct Save_data *point)
{
    uint32_t adds_a,adds_b,updata_adds;

    e1_men_save=e2_men_save=0;
re_write_adds:
    adds_a=adds_b=adds_temp;
    adds_b&=0x000001ff;		//byte adds
    adds_a>>=16;
    adds_a&=0x0000ffff;		//page adds
	
    Spi_Page_Read(adds_a,buff_page);

    buff_page[adds_b+0]=point->xuhao_h;
    buff_page[adds_b+1]=point->xuhao_l;
    buff_page[adds_b+2]=point->status;
    buff_page[adds_b+3]=point->year_h;
    buff_page[adds_b+4]=point->year_l;
    buff_page[adds_b+5]=point->munth;
    buff_page[adds_b+6]=point->day;
    buff_page[adds_b+7]=point->hour;
    buff_page[adds_b+8]=point->minu;
    buff_page[adds_b+9]=point->result_ph;
    buff_page[adds_b+10]=point->result_pl;
    buff_page[adds_b+11]=point->result_nh;
    buff_page[adds_b+12]=point->result_nl;
    buff_page[adds_b+13]=point->unit_alarm_wrong;
    buff_page[adds_b+14]=point->yes_or_no;
    buff_page[adds_b+15]=point->signe_result;

    buff_page[adds_b+16]=point->e1_result_ph;
    buff_page[adds_b+17]=point->e1_result_pl;
    buff_page[adds_b+18]=point->e1_result_nh;
    buff_page[adds_b+19]=point->e1_result_nl;
    buff_page[adds_b+20]=point->e2_result_ph;
    buff_page[adds_b+21]=point->e2_result_pl;
    buff_page[adds_b+22]=point->e2_result_nh;
    buff_page[adds_b+23]=point->e2_result_nl;
    buff_page[adds_b+24]=point->fenxi_fangfa;
    buff_page[adds_b+25]=point->fenxi_obj;

    Spi_Page_Write(adds_a,buff_page);

    if(adds_b==0x000001e0)
    {
        adds_b=0;
        adds_a+=1;
        adds_a<<=16;

        switch(adds_a)
        {
        case FX_CHIP_END_ADDS:
            adds_temp=adds_a=FX_CHIP_START_ADDS;
            Fenxi.xuhao_h=point->xuhao_h=0;
            Fenxi.xuhao_l=point->xuhao_l=1;
            Fenxi.disp_index=1;
            goto re_write_adds;
        case JZ_CHIP_END_ADDS:
            adds_a=JZ_CHIP_START_ADDS;
            Jiaozheng.xuhao_h=point->xuhao_h=0;
            Jiaozheng.xuhao_l=point->xuhao_l=1;
            Jiaozheng.disp_index=1;
            goto re_write_adds;
        case CW_CHIP_END_ADDS:
            adds_a=CW_CHIP_START_ADDS;
            Cuowu.xuhao_h=point->xuhao_h=0;
            Cuowu.xuhao_l=point->xuhao_l=1;
            Cuowu.disp_index=1;
            goto re_write_adds;
        case BJ_CHIP_END_ADDS:
            adds_a=BJ_CHIP_START_ADDS;
            Baojing.xuhao_h=point->xuhao_h=0;
            Baojing.xuhao_l=point->xuhao_l=1;
            Baojing.disp_index=1;
            goto re_write_adds;
        }
    }
    else
    {
        adds_b+=0x20;
        adds_a<<=16;
    }

    updata_adds=adds_a+adds_b;
    return(updata_adds);
}

//===============================================================================
//	finction	:Write_error_status
//	edit		:sam 2013-7-12 10:10
//	description	:CW_write_index_adds_point 地址变更
//===============================================================================
void Write_error_status(void)
{
    CW_write_index_adds_point = Write_result_to_flash(CW_write_index_adds_point,&Cuowu);
    Write_index_adds_point();
}

//===============================================================================
//	finction	:Read_result_from_flash
//	edit		:sam 2012-8-6 15:01
//  input       :select  1=分析，2=校正，3=错误，4=报警
//	description	:从flash中读取历史数据,并显示出来
//===============================================================================
void Read_result_from_flash(uint8_t select)
{
    uint32_t adds_a,adds_b;
    uint16_t rt_cnt = 0;
    uint8_t i,z_cnt = 0;
    float j;

    switch(select)
    {
    case 1:
sub_case11:
        if((Fenxi.disp_index)<=1)
        {
            adds_a=adds_b=FX_disp_read_index_adds_point=FX_write_index_adds_point;
            Fenxi.disp_index=1;
            Jiaozheng.disp_index=1;
            Cuowu.disp_index=1;
            Baojing.disp_index=1;

            adds_b&=0x000001ff;				//byte adds
            adds_a>>=16;
            adds_a&=0x0000ffff;				//page adds

            if((adds_b==0x00000000)&&(adds_a==0x0000004))		//get first adds
            {
                if(system_init_flag==1)
                    break;
            }
            else if(adds_b==0x00000000)
            {
                adds_b=0x000001e0;
                adds_a-=1;
            }
            else
                adds_b-=0x20;
        }
        else
sub_case1:
        {
            adds_a=adds_b=FX_disp_read_index_adds_point;
            adds_b&=0x000001ff;				//byte adds
            adds_a>>=16;
            adds_a&=0x0000ffff;				//page adds

            if((adds_b==0x00000000)&&(adds_a==0x00000004))
            {
                Fenxi.disp_index=1;
                FX_disp_read_index_adds_point=FX_write_index_adds_point;

                ++z_cnt;
                if(z_cnt<2) goto sub_case1;
            }
            else
            {
                if(adds_b==0x00000000)				//get adds
                {
                    adds_b=0x000001e0;
                    adds_a-=1;
                }
                else
                    adds_b-=0x20;
            }
        }

        Spi_Page_Read(adds_a,buff_page);

        if(system_init_flag==1)
        {
            Fenxi.xuhao_h=buff_page[adds_b+0];
            Fenxi.xuhao_l=buff_page[adds_b+1];
        }
        Fenxi.status=buff_page[adds_b+2];
        Fenxi.year_h=buff_page[adds_b+3];
        Fenxi.year_l=buff_page[adds_b+4];
        Fenxi.munth=buff_page[adds_b+5];
        Fenxi.day=buff_page[adds_b+6];
        Fenxi.hour=buff_page[adds_b+7];
        Fenxi.minu=buff_page[adds_b+8];
        Fenxi.result_ph=buff_page[adds_b+9];
        Fenxi.result_pl=buff_page[adds_b+10];
        Fenxi.result_nh=buff_page[adds_b+11];
        Fenxi.result_nl=buff_page[adds_b+12];
        Fenxi.unit_alarm_wrong=buff_page[adds_b+13];
        Fenxi.yes_or_no=buff_page[adds_b+14];
        Fenxi.signe_result=buff_page[adds_b+15];

        Fenxi.e1_result_ph=buff_page[adds_b+16];
        Fenxi.e1_result_pl=buff_page[adds_b+17];
        Fenxi.e1_result_nh=buff_page[adds_b+18];
        Fenxi.e1_result_nl=buff_page[adds_b+19];
        Fenxi.e2_result_ph=buff_page[adds_b+20];
        Fenxi.e2_result_pl=buff_page[adds_b+21];
        Fenxi.e2_result_nh=buff_page[adds_b+22];
        Fenxi.e2_result_nl=buff_page[adds_b+23];
        Fenxi.fenxi_fangfa=buff_page[adds_b+24];

        adds_a<<=16;
        FX_disp_read_index_adds_point=adds_a+adds_b;
        if(((Fenxi.munth==0)&&(Fenxi.day==0))||(Fenxi.year_h!=0x20)||(Fenxi.year_l<0x12))
        {
            WDT_CLR();
            if((adds_b==0x00000000)&&(adds_a==0x00040000))
            {
                if(Men_del_flag==1)
                    __nop();
                else if(FX_write_index_adds_point!=0x00040000)
                {
                    ++rt_cnt;
                    if(rt_cnt<10)
                    {
                        Fenxi.disp_index=1;
                        goto sub_case11;
                    }
                }
            }
            else
                goto sub_case1;
        }


        Disp_history_data(select,&Fenxi);
        break;
    case 2:
sub_case21:
        if((Jiaozheng.disp_index)<=1)
        {
            adds_a=adds_b=JZ_disp_read_index_adds_point=JZ_write_index_adds_point;
            Fenxi.disp_index=1;
            Jiaozheng.disp_index=1;
            Cuowu.disp_index=1;
            Baojing.disp_index=1;

            adds_b&=0x000001ff;				//byte adds
            adds_a>>=16;
            adds_a&=0x0000ffff;				//page adds

            if((adds_b==0x00000000)&&(adds_a==0x00000804))		//get first adds
            {
                if(system_init_flag==1)
                    break;
            }
            else if(adds_b==0x00000000)
            {
                adds_b=0x000001e0;
                adds_a-=1;
            }
            else
                adds_b-=0x20;
        }
        else
sub_case2:
        {
            adds_a=adds_b=JZ_disp_read_index_adds_point;
            adds_b&=0x000001ff;				//byte adds
            adds_a>>=16;
            adds_a&=0x0000ffff;				//page adds

            if((adds_b==0x00000000)&&(adds_a==0x00000804))
            {
                Jiaozheng.disp_index=1;
                goto sub_case21;
            }
            else if(adds_b==0x00000000)				//get adds
            {
                adds_b=0x000001e0;
                adds_a-=1;
            }
            else
                adds_b-=0x20;
        }

        Spi_Page_Read(adds_a,buff_page);

        if(system_init_flag==1)
        {
            Jiaozheng.xuhao_h=buff_page[adds_b+0];
            Jiaozheng.xuhao_l=buff_page[adds_b+1];
        }
        Jiaozheng.status=buff_page[adds_b+2];
        Jiaozheng.year_h=buff_page[adds_b+3];
        Jiaozheng.year_l=buff_page[adds_b+4];
        Jiaozheng.munth=buff_page[adds_b+5];
        Jiaozheng.day=buff_page[adds_b+6];
        Jiaozheng.hour=buff_page[adds_b+7];
        Jiaozheng.minu=buff_page[adds_b+8];
        Jiaozheng.result_ph=buff_page[adds_b+9];
        Jiaozheng.result_pl=buff_page[adds_b+10];
        Jiaozheng.result_nh=buff_page[adds_b+11];
        Jiaozheng.result_nl=buff_page[adds_b+12];
        Jiaozheng.unit_alarm_wrong=buff_page[adds_b+13];
        Jiaozheng.yes_or_no=buff_page[adds_b+14];
        Jiaozheng.signe_result=buff_page[adds_b+15];

        Jiaozheng.e1_result_ph=buff_page[adds_b+16];
        Jiaozheng.e1_result_pl=buff_page[adds_b+17];
        Jiaozheng.e1_result_nh=buff_page[adds_b+18];
        Jiaozheng.e1_result_nl=buff_page[adds_b+19];
        Jiaozheng.e2_result_ph=buff_page[adds_b+20];
        Jiaozheng.e2_result_pl=buff_page[adds_b+21];
        Jiaozheng.e2_result_nh=buff_page[adds_b+22];
        Jiaozheng.e2_result_nl=buff_page[adds_b+23];
        Jiaozheng.fenxi_fangfa=buff_page[adds_b+24];

        adds_a<<=16;
        JZ_disp_read_index_adds_point=adds_a+adds_b;
        if(((Jiaozheng.munth==0)&&(Jiaozheng.day==0))||(Jiaozheng.year_h!=0x20)||(Jiaozheng.year_l<0x12))
        {
            WDT_CLR();
            if((adds_b==0x00000000)&&(adds_a==0x08040000))
            {
                ++rt_cnt;
                if(rt_cnt<10)
                {
                    Jiaozheng.disp_index=1;
                    goto sub_case21;
                }
            }
            else
                goto sub_case2;
        }
        if(Reget_slope==1)
        {
            k2_prog_use=j=0;
            i=(Jiaozheng.result_ph)>>4;
            i&=0x0f;
            j=i;
            j*=1000.0;
            k2_prog_use=j;
            i=Jiaozheng.result_ph;
            i&=0x0f;
            j=i;
            j*=100.0;
            k2_prog_use+=j;
            i=(Jiaozheng.result_pl)>>4;
            i&=0x0f;
            j=i;
            j*=10.0;
            k2_prog_use+=j;
            i=Jiaozheng.result_pl;
            i&=0x0f;
            j=i;
            k2_prog_use+=j;
            i=(Jiaozheng.result_nh)>>4;
            i&=0x0f;
            j=i;
            j/=10.0;
            k2_prog_use+=j;
            i=Jiaozheng.result_nh;
            i&=0x0f;
            j=i;
            j/=100.0;
            k2_prog_use+=j;
            i=(Jiaozheng.result_nl)>>4;
            i&=0x0f;
            j=i;
            j/=1000.0;
            k2_prog_use+=j;
            i=Jiaozheng.result_nl;
            i&=0x0f;
            j=i;
            j/=10000.0;
            k2_prog_use+=j;

            if(Jiaozheng.signe_result!=1)
                k2_prog_use=-k2_prog_use;
        }
        else
            Disp_history_data(select,&Jiaozheng);
        break;
    case 3:
sub_case31:
        if((Cuowu.disp_index)<=1)
        {
//    	        adds_a=adds_b=CW_read_index_adds_point=CW_write_index_adds_point;
            adds_a=adds_b=CW_disp_read_index_adds_point=CW_write_index_adds_point;
            Fenxi.disp_index=1;
            Jiaozheng.disp_index=1;
            Cuowu.disp_index=1;
            Baojing.disp_index=1;

            adds_b&=0x000001ff;				//byte adds
            adds_a>>=16;
            adds_a&=0x0000ffff;				//page adds

            if((adds_b==0x00000000)&&(adds_a==0x00001004))		//get first adds
            {
                if(system_init_flag==1)
                    break;
            }
            else if(adds_b==0x00000000)
            {
                adds_b=0x000001e0;
                adds_a-=1;
            }
            else
                adds_b-=0x20;
        }
        else
sub_case3:
        {
            adds_a=adds_b=CW_disp_read_index_adds_point;
            adds_b&=0x000001ff;				//byte adds
            adds_a>>=16;
            adds_a&=0x0000ffff;				//page adds

            if((adds_b==0x00000000)&&(adds_a==0x00001004))
            {
                Cuowu.disp_index=1;
                goto sub_case31;
            }
            else if(adds_b==0x00000000)				//get adds
            {
                adds_b=0x000001e0;
                adds_a-=1;
            }
            else
                adds_b-=0x20;
        }

        Spi_Page_Read(adds_a,buff_page);

        if(system_init_flag==1)
        {
            Cuowu.xuhao_h=buff_page[adds_b+0];
            Cuowu.xuhao_l=buff_page[adds_b+1];
        }
        Cuowu.status=buff_page[adds_b+2];
        Cuowu.year_h=buff_page[adds_b+3];
        Cuowu.year_l=buff_page[adds_b+4];
        Cuowu.munth=buff_page[adds_b+5];
        Cuowu.day=buff_page[adds_b+6];
        Cuowu.hour=buff_page[adds_b+7];
        Cuowu.minu=buff_page[adds_b+8];
        Cuowu.result_ph=buff_page[adds_b+9];
        Cuowu.result_pl=buff_page[adds_b+10];
        Cuowu.result_nh=buff_page[adds_b+11];
        Cuowu.result_nl=buff_page[adds_b+12];
        Cuowu.unit_alarm_wrong=buff_page[adds_b+13];
        Cuowu.yes_or_no=buff_page[adds_b+14];
        Cuowu.signe_result=buff_page[adds_b+15];

        Cuowu.e1_result_ph=buff_page[adds_b+16];
        Cuowu.e1_result_pl=buff_page[adds_b+17];
        Cuowu.e1_result_nh=buff_page[adds_b+18];
        Cuowu.e1_result_nl=buff_page[adds_b+19];
        Cuowu.e2_result_ph=buff_page[adds_b+20];
        Cuowu.e2_result_pl=buff_page[adds_b+21];
        Cuowu.e2_result_nh=buff_page[adds_b+22];
        Cuowu.e2_result_nl=buff_page[adds_b+23];
        Cuowu.fenxi_fangfa=buff_page[adds_b+24];

        adds_a<<=16;
        CW_disp_read_index_adds_point=adds_a+adds_b;
        if((Cuowu.munth==0)&&(Cuowu.day==0))
        {
            WDT_CLR();
            if((adds_b==0x00000000)&&(adds_a==0x10040000))
            {
                ++rt_cnt;
                if(rt_cnt<10)
                {
                    Cuowu.disp_index=1;
                    goto sub_case31;
                }
            }
            else
                goto sub_case3;
        }
        Disp_history_data(select,&Cuowu);
        break;
    case 4:
sub_case41:
        if((Baojing.disp_index)<=1)
        {
//    	        adds_a=adds_b=BJ_read_index_adds_point=BJ_write_index_adds_point;
            adds_a=adds_b=BJ_disp_read_index_adds_point=BJ_write_index_adds_point;
            Fenxi.disp_index=1;
            Jiaozheng.disp_index=1;
            Cuowu.disp_index=1;
            Baojing.disp_index=1;

            adds_b&=0x000001ff;				//byte adds
            adds_a>>=16;
            adds_a&=0x0000ffff;				//page adds

            if((adds_b==0x00000000)&&(adds_a==0x00001804))		//get first adds
            {
                if(system_init_flag==1)
                    break;
            }
            else if(adds_b==0x00000000)
            {
                adds_b=0x000001e0;
                adds_a-=1;
            }
            else
                adds_b-=0x20;
        }
        else
sub_case4:
        {
            adds_a=adds_b=BJ_disp_read_index_adds_point;
            adds_b&=0x000001ff;				//byte adds
            adds_a>>=16;
            adds_a&=0x0000ffff;				//page adds

            if((adds_b==0x00000000)&&(adds_a==0x00001804))
            {
                Baojing.disp_index=1;
                goto sub_case41;
            }
            else if(adds_b==0x00000000)				//get adds
            {
                adds_b=0x000001e0;
                adds_a-=1;
            }
            else
                adds_b-=0x20;
        }

        Spi_Page_Read(adds_a,buff_page);

        if(system_init_flag==1)
        {
            Baojing.xuhao_h=buff_page[adds_b+0];
            Baojing.xuhao_l=buff_page[adds_b+1];
        }
        Baojing.status=buff_page[adds_b+2];
        Baojing.year_h=buff_page[adds_b+3];
        Baojing.year_l=buff_page[adds_b+4];
        Baojing.munth=buff_page[adds_b+5];
        Baojing.day=buff_page[adds_b+6];
        Baojing.hour=buff_page[adds_b+7];
        Baojing.minu=buff_page[adds_b+8];
        Baojing.result_ph=buff_page[adds_b+9];
        Baojing.result_pl=buff_page[adds_b+10];
        Baojing.result_nh=buff_page[adds_b+11];
        Baojing.result_nl=buff_page[adds_b+12];
        Baojing.unit_alarm_wrong=buff_page[adds_b+13];
        Baojing.yes_or_no=buff_page[adds_b+14];
        Baojing.signe_result=buff_page[adds_b+15];

        Baojing.e1_result_ph=buff_page[adds_b+16];
        Baojing.e1_result_pl=buff_page[adds_b+17];
        Baojing.e1_result_nh=buff_page[adds_b+18];
        Baojing.e1_result_nl=buff_page[adds_b+19];
        Baojing.e2_result_ph=buff_page[adds_b+20];
        Baojing.e2_result_pl=buff_page[adds_b+21];
        Baojing.e2_result_nh=buff_page[adds_b+22];
        Baojing.e2_result_nl=buff_page[adds_b+23];
        Baojing.fenxi_fangfa=buff_page[adds_b+24];

        adds_a<<=16;
        BJ_disp_read_index_adds_point=adds_a+adds_b;
        if((Baojing.munth==0)&&(Baojing.day==0))
        {
            WDT_CLR();
            if((adds_b==0x00000000)&&(adds_a==0x18040000))
            {
                ++rt_cnt;
                if(rt_cnt<10)
                {
                    Baojing.disp_index=1;
                    goto sub_case41;
                }
            }
            else
                goto sub_case4;
        }
        Disp_history_data(select,&Baojing);
        break;
    }
}

//===============================================================================
//	finction	:Read_result_from_flash_index_up
//	edit		:sam 2012-8-6 15:01
//	description	:当在历史记录界面检测到按上下键时,从flash中读取上一个历史记录并显示出来
//===============================================================================
void Read_result_from_flash_index_up(uint8_t select)
{
    uint32_t adds_a=0,adds_b=0;
    uint16_t j,rt_cnt;

    rt_cnt=0;
    switch(select)
    {
    case 1:
sub_case1:
        if((Fenxi.disp_index)<=1)
        {
            if(Fenxi.disp_index==1)
            {
                adds_a=adds_b=FX_disp_read_index_adds_point;
                adds_b&=0x000001ff;				//byte adds
                adds_a>>=16;
                adds_a&=0x0000ffff;				//page adds
                if(adds_b==0x000001e0)
                {
                    adds_b=0;
                    adds_a+=1;
                }
                else	adds_b+=0x20;
            }
            else if(Fenxi.disp_index==0)
            {
                Fenxi.disp_index=Fenxi.xuhao_h<<8;
                Fenxi.disp_index+=Fenxi.xuhao_l;
                if(Fenxi.disp_index==0)
                {
                    Fenxi.disp_index=1;
                    Fenxi.xuhao_l=1;
                }

                adds_a=adds_b=FX_disp_read_index_adds_point=FX_CHIP_START_ADDS;
                adds_b&=0x000001ff;				//byte adds
                adds_a>>=16;
                adds_a&=0x0000ffff;				//page adds
            }

            else if(FX_disp_read_index_adds_point==FX_write_index_adds_point)
            {
                Fenxi.disp_index=1;
                adds_b=0;
                adds_a=0x00000004;

                j=Fenxi.xuhao_h;
                j<<=8;
                j+=Fenxi.xuhao_l;
                Fenxi.disp_index=j;
            }
            else
            {
                Fenxi.disp_index=1;
                if(adds_b==0x000001e0)
                {
                    adds_b=0;
                    adds_a>>=16;
                    adds_a+=1;
                    if(adds_a==0x00000803)
                    {
                        adds_a=0x00000004;

                        j=Fenxi.xuhao_h;
                        j<<=8;
                        j+=Fenxi.xuhao_l;
/////		           j&=0x0fff;
                        Fenxi.disp_index=j;
                    }
                }
                else
                    adds_b+=0x20;
            }
        }
        else
sub_case1_1:
        {
            adds_a=adds_b=FX_disp_read_index_adds_point;
            adds_b&=0x000001ff;				//byte adds
            adds_a>>=16;
            adds_a&=0x0000ffff;				//page adds

            if(FX_disp_read_index_adds_point==FX_write_index_adds_point)
            {
                adds_b=0;
                adds_a=0x00000004;

                j=Fenxi.xuhao_h;
                j<<=8;
                j+=Fenxi.xuhao_l;
/////		    j&=0x0fff;
                Fenxi.disp_index=j;
            }
            else if(adds_b==0x000001e0)
            {
                adds_b=0;
                adds_a+=1;
                if(adds_a==0x00000803)
                {
                    adds_a=0x00000004;

                    j=Fenxi.xuhao_h;
                    j<<=8;
                    j+=Fenxi.xuhao_l;
/////		       j&=0x0fff;
                    Fenxi.disp_index=j;
                }
            }
            else
                adds_b+=0x20;
        }

        Spi_Page_Read(adds_a,buff_page);

        if(system_init_flag==1)
        {
            Fenxi.xuhao_h=buff_page[adds_b+0];
            Fenxi.xuhao_l=buff_page[adds_b+1];
        }
        Fenxi.status=buff_page[adds_b+2];
        Fenxi.year_h=buff_page[adds_b+3];
        Fenxi.year_l=buff_page[adds_b+4];
        Fenxi.munth=buff_page[adds_b+5];
        Fenxi.day=buff_page[adds_b+6];
        Fenxi.hour=buff_page[adds_b+7];
        Fenxi.minu=buff_page[adds_b+8];
        Fenxi.result_ph=buff_page[adds_b+9];
        Fenxi.result_pl=buff_page[adds_b+10];
        Fenxi.result_nh=buff_page[adds_b+11];
        Fenxi.result_nl=buff_page[adds_b+12];
        Fenxi.unit_alarm_wrong=buff_page[adds_b+13];
        Fenxi.yes_or_no=buff_page[adds_b+14];
        Fenxi.signe_result=buff_page[adds_b+15];

        Fenxi.e1_result_ph=buff_page[adds_b+16];
        Fenxi.e1_result_pl=buff_page[adds_b+17];
        Fenxi.e1_result_nh=buff_page[adds_b+18];
        Fenxi.e1_result_nl=buff_page[adds_b+19];
        Fenxi.e2_result_ph=buff_page[adds_b+20];
        Fenxi.e2_result_pl=buff_page[adds_b+21];
        Fenxi.e2_result_nh=buff_page[adds_b+22];
        Fenxi.e2_result_nl=buff_page[adds_b+23];
        Fenxi.fenxi_fangfa=buff_page[adds_b+24];

        adds_a<<=16;
        FX_disp_read_index_adds_point=adds_a+adds_b;
        if(((Fenxi.munth==0)&&(Fenxi.day==0))||(Fenxi.year_h!=0x20)||(Fenxi.year_l<0x12))
        {
            WDT_CLR();
            if((adds_b==0x00000000)&&(adds_a==0x00040000))
            {
                ++rt_cnt;
                if(rt_cnt<10)
                    goto sub_case1_1;
            }
            else
            {
                ++rt_cnt;
                if(rt_cnt<20)
                    goto sub_case1;
            }
        }
        Disp_history_data(select,&Fenxi);
        break;
    case 2:
sub_case2:
        if((Jiaozheng.disp_index)<=1)
        {
            if(Jiaozheng.disp_index==1)
            {
                adds_a=adds_b=JZ_disp_read_index_adds_point;
                adds_b&=0x000001ff;				//byte adds
                adds_a>>=16;
                adds_a&=0x0000ffff;				//page adds
                if(adds_b==0x000001e0)
                {
                    adds_b=0;
                    adds_a+=1;
                }
                else	adds_b+=0x20;
            }
            else if(Jiaozheng.disp_index==0)
            {
                Jiaozheng.disp_index=Jiaozheng.xuhao_h<<8;
                Jiaozheng.disp_index+=Jiaozheng.xuhao_l;
                if(Jiaozheng.disp_index==0)
                {
                    Jiaozheng.disp_index=1;
                    Jiaozheng.xuhao_l=1;
                }

                adds_a=adds_b=JZ_disp_read_index_adds_point=JZ_CHIP_START_ADDS;
                adds_b&=0x000001ff;				//byte adds
                adds_a>>=16;
                adds_a&=0x0000ffff;				//page adds
            }
            else if(JZ_disp_read_index_adds_point==JZ_write_index_adds_point)
            {
                Jiaozheng.disp_index=1;
                adds_b=0;
                adds_a=0x00000804;

                j=Jiaozheng.xuhao_h;
                j<<=8;
                j+=Jiaozheng.xuhao_l;
                Jiaozheng.disp_index=j;
            }
            else
            {
                Jiaozheng.disp_index=1;
                if(adds_b==0x000001e0)
                {
                    adds_b=0;
                    adds_a>>=16;
                    adds_a+=1;
                    if(adds_a==0x00001003)
                    {
                        adds_a=0x00000804;

                        j=Jiaozheng.xuhao_h;
                        j<<=8;
                        j+=Jiaozheng.xuhao_l;
                        Jiaozheng.disp_index=j;
                    }
                }
                else
                    adds_b+=0x20;
            }
        }
        else
sub_case2_1:
        {
            adds_a=adds_b=JZ_disp_read_index_adds_point;
            adds_b&=0x000001ff;				//byte adds
            adds_a>>=16;
            adds_a&=0x0000ffff;				//page adds

            if(JZ_disp_read_index_adds_point==JZ_write_index_adds_point)
            {
                adds_b=0;
                adds_a=0x00000804;

                j=Jiaozheng.xuhao_h;
                j<<=8;
                j+=Jiaozheng.xuhao_l;
                Jiaozheng.disp_index=j;
            }
            else if(adds_b==0x000001e0)
            {
                adds_b=0;
                adds_a+=1;
                if(adds_a==0x00001003)
                {
                    adds_a=0x00000804;

                    j=Jiaozheng.xuhao_h;
                    j<<=8;
                    j+=Jiaozheng.xuhao_l;
                    Jiaozheng.disp_index=j;
                }
            }
            else
                adds_b+=0x20;
        }

        Spi_Page_Read(adds_a,buff_page);

        if(system_init_flag==1)
        {
            Jiaozheng.xuhao_h=buff_page[adds_b+0];
            Jiaozheng.xuhao_l=buff_page[adds_b+1];
        }
        Jiaozheng.status=buff_page[adds_b+2];
        Jiaozheng.year_h=buff_page[adds_b+3];
        Jiaozheng.year_l=buff_page[adds_b+4];
        Jiaozheng.munth=buff_page[adds_b+5];
        Jiaozheng.day=buff_page[adds_b+6];
        Jiaozheng.hour=buff_page[adds_b+7];
        Jiaozheng.minu=buff_page[adds_b+8];
        Jiaozheng.result_ph=buff_page[adds_b+9];
        Jiaozheng.result_pl=buff_page[adds_b+10];
        Jiaozheng.result_nh=buff_page[adds_b+11];
        Jiaozheng.result_nl=buff_page[adds_b+12];
        Jiaozheng.unit_alarm_wrong=buff_page[adds_b+13];
        Jiaozheng.yes_or_no=buff_page[adds_b+14];
        Jiaozheng.signe_result=buff_page[adds_b+15];

        Jiaozheng.e1_result_ph=buff_page[adds_b+16];
        Jiaozheng.e1_result_pl=buff_page[adds_b+17];
        Jiaozheng.e1_result_nh=buff_page[adds_b+18];
        Jiaozheng.e1_result_nl=buff_page[adds_b+19];
        Jiaozheng.e2_result_ph=buff_page[adds_b+20];
        Jiaozheng.e2_result_pl=buff_page[adds_b+21];
        Jiaozheng.e2_result_nh=buff_page[adds_b+22];
        Jiaozheng.e2_result_nl=buff_page[adds_b+23];
        Jiaozheng.fenxi_fangfa=buff_page[adds_b+24];

        adds_a<<=16;
        JZ_disp_read_index_adds_point=adds_a+adds_b;
        if(((Jiaozheng.munth==0)&&(Jiaozheng.day==0))||(Jiaozheng.year_h!=0x20)||(Jiaozheng.year_l<0x12))
        {
            WDT_CLR();
            if((adds_b==0x00000000)&&(adds_a==0x08040000))
            {
                ++rt_cnt;
                if(rt_cnt<10)
                    goto sub_case2_1;
            }
            else
            {
                ++rt_cnt;
                if(rt_cnt<20)
                    goto sub_case2;
            }
        }

        Disp_history_data(select,&Jiaozheng);
        break;
    case 3:
sub_case3:
        if((Cuowu.disp_index)<=1)
        {
            if(Cuowu.disp_index==1)
            {
                adds_a=adds_b=CW_disp_read_index_adds_point;
                adds_b&=0x000001ff;				//byte adds
                adds_a>>=16;
                adds_a&=0x0000ffff;				//page adds
                if(adds_b==0x000001e0)
                {
                    adds_b=0;
                    adds_a+=1;
                }
                else	adds_b+=0x20;
            }
            else if(Cuowu.disp_index==0)
            {
                Cuowu.disp_index=Cuowu.xuhao_h<<8;
                Cuowu.disp_index+=Cuowu.xuhao_l;
                if(Cuowu.disp_index==0)
                {
                    Cuowu.disp_index=1;
                    Cuowu.xuhao_l=1;
                }

                adds_a=adds_b=CW_disp_read_index_adds_point=CW_CHIP_START_ADDS;
                adds_b&=0x000001ff;				//byte adds
                adds_a>>=16;
                adds_a&=0x0000ffff;				//page adds
            }
            else if(CW_disp_read_index_adds_point==CW_write_index_adds_point)
            {
                Cuowu.disp_index=1;
                adds_b=0;
                adds_a=0x00001004;

                j=Cuowu.xuhao_h;
                j<<=8;
                j+=Cuowu.xuhao_l;
/////		    j&=0x0fff;
                Cuowu.disp_index=j;
            }
            else
            {
                Cuowu.disp_index=1;
                if(adds_b==0x000001e0)
                {
                    adds_b=0;
                    adds_a>>=16;
                    adds_a+=1;
                    if(adds_a==0x00001803)
                    {
                        adds_a=0x00001004;

                        j=Cuowu.xuhao_h;
                        j<<=8;
                        j+=Cuowu.xuhao_l;
/////		           j&=0x0fff;
                        Cuowu.disp_index=j;
                    }
                }
                else
                    adds_b+=0x20;
            }
        }
        else
sub_case3_1:
        {
            adds_a=adds_b=CW_disp_read_index_adds_point;
            adds_b&=0x000001ff;				//byte adds
            adds_a>>=16;
            adds_a&=0x0000ffff;				//page adds

            if(CW_disp_read_index_adds_point==CW_write_index_adds_point)
            {
                adds_b=0;
                adds_a=0x00001004;

                j=Cuowu.xuhao_h;
                j<<=8;
                j+=Cuowu.xuhao_l;
/////		    j&=0x0fff;
                Cuowu.disp_index=j;
            }
            else if(adds_b==0x000001e0)
            {
                adds_b=0;
                adds_a+=1;
                if(adds_a==0x00001803)
                {
                    adds_a=0x00001004;

                    j=Cuowu.xuhao_h;
                    j<<=8;
                    j+=Cuowu.xuhao_l;
/////		       j&=0x0fff;
                    Cuowu.disp_index=j;
                }
            }
            else
                adds_b+=0x20;
        }

        Spi_Page_Read(adds_a,buff_page);

        if(system_init_flag==1)
        {
            Cuowu.xuhao_h=buff_page[adds_b+0];
            Cuowu.xuhao_l=buff_page[adds_b+1];
        }
        Cuowu.status=buff_page[adds_b+2];
        Cuowu.year_h=buff_page[adds_b+3];
        Cuowu.year_l=buff_page[adds_b+4];
        Cuowu.munth=buff_page[adds_b+5];
        Cuowu.day=buff_page[adds_b+6];
        Cuowu.hour=buff_page[adds_b+7];
        Cuowu.minu=buff_page[adds_b+8];
        Cuowu.result_ph=buff_page[adds_b+9];
        Cuowu.result_pl=buff_page[adds_b+10];
        Cuowu.result_nh=buff_page[adds_b+11];
        Cuowu.result_nl=buff_page[adds_b+12];
        Cuowu.unit_alarm_wrong=buff_page[adds_b+13];
        Cuowu.yes_or_no=buff_page[adds_b+14];
        Cuowu.signe_result=buff_page[adds_b+15];

        Cuowu.e1_result_ph=buff_page[adds_b+16];
        Cuowu.e1_result_pl=buff_page[adds_b+17];
        Cuowu.e1_result_nh=buff_page[adds_b+18];
        Cuowu.e1_result_nl=buff_page[adds_b+19];
        Cuowu.e2_result_ph=buff_page[adds_b+20];
        Cuowu.e2_result_pl=buff_page[adds_b+21];
        Cuowu.e2_result_nh=buff_page[adds_b+22];
        Cuowu.e2_result_nl=buff_page[adds_b+23];
        Cuowu.fenxi_fangfa=buff_page[adds_b+24];

        adds_a<<=16;
        CW_disp_read_index_adds_point=adds_a+adds_b;
        if((Cuowu.munth==0)&&(Cuowu.day==0))
        {
            WDT_CLR();
            if((adds_b==0x00000000)&&(adds_a==0x10040000))
            {
                ++rt_cnt;
                if(rt_cnt<10)
                    goto sub_case3_1;
            }
            else
            {
                ++rt_cnt;
                if(rt_cnt<20)
                    goto sub_case3;
            }
        }

        Disp_history_data(select,&Cuowu);
        break;
    case 4:
sub_case4:
        if((Baojing.disp_index)<=1)
        {
            if(Baojing.disp_index==1)
            {
                adds_a=adds_b=BJ_disp_read_index_adds_point;
                adds_b&=0x000001ff;				//byte adds
                adds_a>>=16;
                adds_a&=0x0000ffff;				//page adds
                if(adds_b==0x000001e0)
                {
                    adds_b=0;
                    adds_a+=1;
                }
                else	adds_b+=0x20;
            }
            else if(Baojing.disp_index==0)
            {
                Baojing.disp_index=Baojing.xuhao_h<<8;
                Baojing.disp_index+=Baojing.xuhao_l;
                if(Baojing.disp_index==0)
                {
                    Baojing.disp_index=1;
                    Baojing.xuhao_l=1;
                }

                adds_a=adds_b=BJ_disp_read_index_adds_point=BJ_CHIP_START_ADDS;
                adds_b&=0x000001ff;				//byte adds
                adds_a>>=16;
                adds_a&=0x0000ffff;				//page adds
            }
            else if(BJ_disp_read_index_adds_point==BJ_write_index_adds_point)
            {
                Baojing.disp_index=1;
                adds_b=0;
                adds_a=0x00001804;

                j=Baojing.xuhao_h;
                j<<=8;
                j+=Baojing.xuhao_l;
                Baojing.disp_index=j;
            }
            else
            {
                Baojing.disp_index=1;
                if(adds_b==0x000001e0)
                {
                    adds_b=0;
                    adds_a>>=16;
                    adds_a+=1;
                    if(adds_a==0x00001FFE)		//0x003E8000
                    {
                        adds_a=0x00001804;

                        j=Baojing.xuhao_h;
                        j<<=8;
                        j+=Baojing.xuhao_l;
/////		           j&=0x0fff;
                        Baojing.disp_index=j;
                    }
                }
                else
                    adds_b+=0x20;
            }
        }
        else
sub_case4_1:
        {
            adds_a=adds_b=BJ_disp_read_index_adds_point;
            adds_b&=0x000001ff;				//byte adds
            adds_a>>=16;
            adds_a&=0x0000ffff;				//page adds

            if(BJ_disp_read_index_adds_point==BJ_write_index_adds_point)
            {
                adds_b=0;
                adds_a=0x00001804;

                j=Baojing.xuhao_h;
                j<<=8;
                j+=Baojing.xuhao_l;
                Baojing.disp_index=j;
            }
            else if(adds_b==0x000001e0)
            {
                adds_b=0;
                adds_a+=1;
                if(adds_a==0x00001FFE)
                {
                    adds_a=0x00001804;

                    j=Baojing.xuhao_h;
                    j<<=8;
                    j+=Baojing.xuhao_l;
/////		       j&=0x0fff;
                    Baojing.disp_index=j;
                }
            }
            else
                adds_b+=0x20;
        }

        Spi_Page_Read(adds_a,buff_page);

        if(system_init_flag==1)
        {
            Baojing.xuhao_h=buff_page[adds_b+0];
            Baojing.xuhao_l=buff_page[adds_b+1];
        }
        Baojing.status=buff_page[adds_b+2];
        Baojing.year_h=buff_page[adds_b+3];
        Baojing.year_l=buff_page[adds_b+4];
        Baojing.munth=buff_page[adds_b+5];
        Baojing.day=buff_page[adds_b+6];
        Baojing.hour=buff_page[adds_b+7];
        Baojing.minu=buff_page[adds_b+8];
        Baojing.result_ph=buff_page[adds_b+9];
        Baojing.result_pl=buff_page[adds_b+10];
        Baojing.result_nh=buff_page[adds_b+11];
        Baojing.result_nl=buff_page[adds_b+12];
        Baojing.unit_alarm_wrong=buff_page[adds_b+13];
        Baojing.yes_or_no=buff_page[adds_b+14];
        Baojing.signe_result=buff_page[adds_b+15];

        Baojing.e1_result_ph=buff_page[adds_b+16];
        Baojing.e1_result_pl=buff_page[adds_b+17];
        Baojing.e1_result_nh=buff_page[adds_b+18];
        Baojing.e1_result_nl=buff_page[adds_b+19];
        Baojing.e2_result_ph=buff_page[adds_b+20];
        Baojing.e2_result_pl=buff_page[adds_b+21];
        Baojing.e2_result_nh=buff_page[adds_b+22];
        Baojing.e2_result_nl=buff_page[adds_b+23];
        Baojing.fenxi_fangfa=buff_page[adds_b+24];

        adds_a<<=16;
        BJ_disp_read_index_adds_point=adds_a+adds_b;
        if((Baojing.munth==0)&&(Baojing.day==0))
        {
            WDT_CLR();
            if((adds_b==0x00000000)&&(adds_a==0x18040000))
            {
                ++rt_cnt;
                if(rt_cnt<10)
                    goto sub_case4_1;
            }
            else
            {
                ++rt_cnt;
                if(rt_cnt<20)
                    goto sub_case4;
            }
        }

        Disp_history_data(select,&Baojing);
        break;
    }
}


//===============================================================================
//	finction	:Updata_xuhao_data
//	input		:select  1=分析，2=校正，3=错误，4=报警
//	edit		:sam 2012-9-1 23:25
//	description	:更新序号并将新序号写入flash
//===============================================================================
void Updata_xuhao_data(uint8_t select)
{
    uint8_t xuhao_h=0,xuhao_l=0;
    uint32_t adds_a=0,adds_b=0;

    switch(select)
    {
    case 1:
        xuhao_h=Fenxi.xuhao_h;
        xuhao_l=Fenxi.xuhao_l;
        adds_a=adds_b=FX_write_index_adds_point;
        break;
    case 2:
        xuhao_h=Jiaozheng.xuhao_h;
        xuhao_l=Jiaozheng.xuhao_l;
        adds_a=adds_b=JZ_write_index_adds_point;
        break;
    case 3:
        xuhao_h=Cuowu.xuhao_h;
        xuhao_l=Cuowu.xuhao_l;
        adds_a=adds_b=CW_write_index_adds_point;
        break;
    case 4:
        xuhao_h=Baojing.xuhao_h;
        xuhao_l=Baojing.xuhao_l;
        adds_a=adds_b=BJ_write_index_adds_point;
        break;
    }
    adds_b&=0x000001ff;				//byte adds
    adds_a>>=16;
    adds_a&=0x0000ffff;				//page adds

    if(adds_b==0x00000000)
    {
        adds_b=0x000001e0;
        adds_a-=1;
    }
    else
        adds_b-=0x20;

    Spi_Page_Read(adds_a,buff_page);

    buff_page[adds_b+0]=xuhao_h;
    buff_page[adds_b+1]=xuhao_l;

    Spi_Page_Write(adds_a,buff_page);
}

//===============================================================================
//	finction	:Del_mem_data
//	input		:select  1=分析，2=校正，3=错误，4=报警
//	edit		:sam 2012-8-20 9:59
//	description	:删除一条flash记录
//===============================================================================
void Del_mem_data(uint8_t select)
{
    uint32_t adds_a=0,adds_b=0;
    uint8_t i;

    switch(select)
    {
    case 1://分析
        adds_a = adds_b = FX_disp_read_index_adds_point;
        if(Fenxi.xuhao_l==0)
        {
            Fenxi.xuhao_l=0xff;
            Fenxi.xuhao_h-=1;
        }
        else
            Fenxi.xuhao_l-=1;
        break;
    case 2://校正
        adds_a=adds_b=JZ_disp_read_index_adds_point;
        if(Jiaozheng.xuhao_l==0)
        {
            Jiaozheng.xuhao_l=0xff;
            Jiaozheng.xuhao_h-=1;
        }
        else
            Jiaozheng.xuhao_l-=1;
        break;
    case 3://错误
        adds_a=adds_b=CW_disp_read_index_adds_point;
        if(Cuowu.xuhao_l==0)
        {
            Cuowu.xuhao_l=0xff;
            Cuowu.xuhao_h-=1;
        }
        else
            Cuowu.xuhao_l-=1;
        break;
    case 4://报警
        adds_a=adds_b=BJ_disp_read_index_adds_point;
        if(Baojing.xuhao_l==0)
        {
            Baojing.xuhao_l=0xff;
            Baojing.xuhao_h-=1;
        }
        else
            Baojing.xuhao_l-=1;
        break;
    }
    adds_b &= 0x1ff;				//byte adds : 一页大小为512byte
    adds_a = (adds_a>>16) & 0xffff;	//page adds

    Spi_Page_Read(adds_a,buff_page);//将改页的数据读入buff_page缓存

    for(i=0; i<32; i++) //一条记录占用32个byte字节
    {
        buff_page[adds_b+i]=0;//删除一条记录
    }

    Spi_Page_Write(adds_a,buff_page);//将修改好的buff_page存入flash

    Updata_xuhao_data(select);  	//更新序号
    Menu_lever_select=3;            //更新界面深度
    Menu_index_select=5;
}


//===============================================================================
//	finction	:Export__flash_data_finc
//	edit		:select  1=分析，2=校正，3=错误，4=报警
//	description	:依次从flash中导出分析-校正-错误-报警记录
//===============================================================================
void Export__flash_data_finc(void)
{
    uint32_t adds_a,adds_b,sum = sum;
    uint16_t i,k = 0;

    Set_back_light_10sec();
    switch(export_data_select)
    {
    case 1://分析
        if(export_tx_flag==0)
        {
            adds_a=adds_b=FX_export_index_adds_point;
            adds_b = (FX_export_index_adds_point) & 0x1ff;          //计算数据在该页中的从一个byte开始
            adds_a = (FX_export_index_adds_point >> 16) & 0xffff;	//取出页地址

            if((adds_a==0x00000004) && (adds_b==0x00000000))		//第一条分析记录的地址,表示flash中还未保存有数据
            {
                export_data_select = 2;//导出校正记录

                for(i=0; i<26; i++)
                    buff_page_exp_tx[i]=0x00;

                export_tx_lenth=25;
                export_tx_flag=1;
                export_tx_lenth_cnt=0;
                DrvUART_Write(UART_PORT0,buff_page_exp_tx,1);
                En_Uart012_THRE_int(UART_PORT0);
            }
            else              //flash中有分析记录
            {
                Spi_Page_Read(adds_a,buff_page_exp);//读取一页数据

                k=0;
                for(i=16; i>0; i--)//一条记录32byte ,一页512byte, 所以一页最多保存16条记录
                {
                    sum = buff_page_exp[(i-1)*0x20+9]+buff_page_exp[(i-1)*0x20+10]+buff_page_exp[(i-1)*0x20+11]+buff_page_exp[(i-1)*0x20+12];
					//判断为20XX年(每条记录的年高位都是固定的0x20), 判断记录的月份是小于等于12月, 判断记录的小时小于等于24
					//通过时间判断该条记录是否合法
                    if((buff_page_exp[(i-1)*0x20+3]==0x20) && (buff_page_exp[(i-1)*0x20+5]<=0x12) && (buff_page_exp[(i-1)*0x20+7]<0x24))
                    {
                        buff_page_exp_tx[k*25+0]=buff_page_exp[(i-1)*32+2];
                        buff_page_exp_tx[k*25+1]=buff_page_exp[(i-1)*32+3];
                        buff_page_exp_tx[k*25+2]=buff_page_exp[(i-1)*32+4];
                        buff_page_exp_tx[k*25+3]=buff_page_exp[(i-1)*32+5];
                        buff_page_exp_tx[k*25+4]=buff_page_exp[(i-1)*32+6];
                        buff_page_exp_tx[k*25+5]=buff_page_exp[(i-1)*32+7];
                        buff_page_exp_tx[k*25+6]=buff_page_exp[(i-1)*32+8];
                        buff_page_exp_tx[k*25+7]=buff_page_exp[(i-1)*32+9];
                        buff_page_exp_tx[k*25+8]=buff_page_exp[(i-1)*32+10];
                        buff_page_exp_tx[k*25+9]=buff_page_exp[(i-1)*32+11];
                        buff_page_exp_tx[k*25+10]=buff_page_exp[(i-1)*32+12];
                        buff_page_exp_tx[k*25+11]=buff_page_exp[(i-1)*32+13];
                        buff_page_exp_tx[k*25+12]=buff_page_exp[(i-1)*32+14];
                        buff_page_exp_tx[k*25+13]=buff_page_exp[(i-1)*32+15];
                        buff_page_exp_tx[k*25+14]=buff_page_exp[(i-1)*32+16];
                        buff_page_exp_tx[k*25+15]=buff_page_exp[(i-1)*32+17];
                        buff_page_exp_tx[k*25+16]=buff_page_exp[(i-1)*32+18];
                        buff_page_exp_tx[k*25+17]=buff_page_exp[(i-1)*32+19];
                        buff_page_exp_tx[k*25+18]=buff_page_exp[(i-1)*32+20];
                        buff_page_exp_tx[k*25+19]=buff_page_exp[(i-1)*32+21];
                        buff_page_exp_tx[k*25+20]=buff_page_exp[(i-1)*32+22];
                        buff_page_exp_tx[k*25+21]=buff_page_exp[(i-1)*32+23];
                        buff_page_exp_tx[k*25+22]=buff_page_exp[(i-1)*32+24];
                        buff_page_exp_tx[k*25+23]=buff_page_exp[(i-1)*32+25];
                        buff_page_exp_tx[k*25+24]=0xff;
                        ++k;//用于计算该页中有多少记录
                    }
                }
                if(adds_a==0x00000004)//所有数据都导出
                    FX_export_index_adds_point = 0x00040000;
                else
                {
                    adds_a = (adds_a - 1) << 16;//每导出一页的数据, 页地址减一
                    FX_export_index_adds_point = adds_a;

                    if(adds_a==0x00040000)
                        FX_export_index_adds_point=0x00040001;
                }
				
                if(k!=0)//该页有合法的分析记录
                {
                    export_tx_lenth=k*25;
                    export_tx_flag=1;
                    export_tx_lenth_cnt=0;
                    DrvUART_Write(UART_PORT0,buff_page_exp_tx,1);
                    En_Uart012_THRE_int(UART_PORT0);
                }
            }
        }
        break;
    case 2://校正
        if(export_tx_flag==0)
        {
            adds_a=adds_b=JZ_export_index_adds_point;
            adds_b&=0x000001ff;
            adds_a>>=16;
            adds_a&=0x0000ffff;				//page adds

            if((adds_a==0x00000804)&&(adds_b==0x00000000))		//第一条校正记录的地址
            {
                export_data_select=3;

                for(i=0; i<26; i++)
                    buff_page_exp_tx[i]=0x00;

                export_tx_lenth=25;
                export_tx_flag=1;
                export_tx_lenth_cnt=0;
                DrvUART_Write(UART_PORT0,buff_page_exp_tx,1);
                En_Uart012_THRE_int(UART_PORT0);
            }
            else
            {
                Spi_Page_Read(adds_a,buff_page_exp);

                k=0;
                for(i=16; i>0; i--)
                {
                    sum=buff_page_exp[(i-1)*0x20+9]+buff_page_exp[(i-1)*0x20+10]+buff_page_exp[(i-1)*0x20+11]+buff_page_exp[(i-1)*0x20+12];
                    if((buff_page_exp[(i-1)*0x20+3]==0x20)&&(buff_page_exp[(i-1)*0x20+5]<=0x12)&&(buff_page_exp[(i-1)*0x20+7]<0x24))
                    {
                        buff_page_exp_tx[k*25+0]=buff_page_exp[(i-1)*32+2];
                        buff_page_exp_tx[k*25+1]=buff_page_exp[(i-1)*32+3];
                        buff_page_exp_tx[k*25+2]=buff_page_exp[(i-1)*32+4];
                        buff_page_exp_tx[k*25+3]=buff_page_exp[(i-1)*32+5];
                        buff_page_exp_tx[k*25+4]=buff_page_exp[(i-1)*32+6];
                        buff_page_exp_tx[k*25+5]=buff_page_exp[(i-1)*32+7];
                        buff_page_exp_tx[k*25+6]=buff_page_exp[(i-1)*32+8];
                        buff_page_exp_tx[k*25+7]=buff_page_exp[(i-1)*32+9];
                        buff_page_exp_tx[k*25+8]=buff_page_exp[(i-1)*32+10];
                        buff_page_exp_tx[k*25+9]=buff_page_exp[(i-1)*32+11];
                        buff_page_exp_tx[k*25+10]=buff_page_exp[(i-1)*32+12];
                        buff_page_exp_tx[k*25+11]=buff_page_exp[(i-1)*32+13];
                        buff_page_exp_tx[k*25+12]=buff_page_exp[(i-1)*32+14];
                        buff_page_exp_tx[k*25+13]=buff_page_exp[(i-1)*32+15];
                        buff_page_exp_tx[k*25+14]=buff_page_exp[(i-1)*32+16];
                        buff_page_exp_tx[k*25+15]=buff_page_exp[(i-1)*32+17];
                        buff_page_exp_tx[k*25+16]=buff_page_exp[(i-1)*32+18];
                        buff_page_exp_tx[k*25+17]=buff_page_exp[(i-1)*32+19];
                        buff_page_exp_tx[k*25+18]=buff_page_exp[(i-1)*32+20];
                        buff_page_exp_tx[k*25+19]=buff_page_exp[(i-1)*32+21];
                        buff_page_exp_tx[k*25+20]=buff_page_exp[(i-1)*32+22];
                        buff_page_exp_tx[k*25+21]=buff_page_exp[(i-1)*32+23];
                        buff_page_exp_tx[k*25+22]=buff_page_exp[(i-1)*32+24];
                        buff_page_exp_tx[k*25+23]=buff_page_exp[(i-1)*32+25];
                        buff_page_exp_tx[k*25+24]=0xff;
//    	                    Switch_e1e2_data(buff_page_exp_tx[k*25+22],(k*25+14));
                        ++k;
/////    	                 }
                    }
                }
                if(adds_a==0x00000804)
                    JZ_export_index_adds_point=0x08040000;
                else
                {
                    adds_a-=1;
                    adds_a<<=16;
                    JZ_export_index_adds_point=adds_a;

                    if(adds_a==0x08040000)
                        JZ_export_index_adds_point=0x08040001;
                }
                if(k!=0)
                {
                    export_tx_lenth=k*25;
                    export_tx_flag=1;
                    export_tx_lenth_cnt=0;
                    DrvUART_Write(UART_PORT0,buff_page_exp_tx,1);
                    En_Uart012_THRE_int(UART_PORT0);
                }
            }
        }
        break;
    case 3://错误
        if(export_tx_flag==0)
        {
            adds_a=adds_b=CW_export_index_adds_point;
            adds_b&=0x000001ff;
            adds_a>>=16;
            adds_a&=0x0000ffff;				//page adds

            if((adds_a==0x00001004)&&(adds_b==0x00000000))		//get first adds
            {
                export_data_select=4;

                for(i=0; i<26; i++)
                    buff_page_exp_tx[i]=0x00;

                export_tx_lenth=25;
                export_tx_flag=1;
                export_tx_lenth_cnt=0;
                DrvUART_Write(UART_PORT0,buff_page_exp_tx,1);
                En_Uart012_THRE_int(UART_PORT0);
            }
            else
            {
                Spi_Page_Read(adds_a,buff_page_exp);

                k=0;
                for(i=16; i>0; i--)
                {
                    sum=0;
                    sum=buff_page_exp[(i-1)*0x20+9]+buff_page_exp[(i-1)*0x20+10]+buff_page_exp[(i-1)*0x20+11]+buff_page_exp[(i-1)*0x20+12];
                    if((buff_page_exp[(i-1)*0x20+3]==0x20)&&(buff_page_exp[(i-1)*0x20+5]<=0x12)&&(buff_page_exp[(i-1)*0x20+7]<0x24))
                    {
/////    	                 if(sum>=0x00)
/////    	                 {
                        buff_page_exp_tx[k*25+0]=buff_page_exp[(i-1)*32+2];
                        buff_page_exp_tx[k*25+1]=buff_page_exp[(i-1)*32+3];
                        buff_page_exp_tx[k*25+2]=buff_page_exp[(i-1)*32+4];
                        buff_page_exp_tx[k*25+3]=buff_page_exp[(i-1)*32+5];
                        buff_page_exp_tx[k*25+4]=buff_page_exp[(i-1)*32+6];
                        buff_page_exp_tx[k*25+5]=buff_page_exp[(i-1)*32+7];
                        buff_page_exp_tx[k*25+6]=buff_page_exp[(i-1)*32+8];
                        buff_page_exp_tx[k*25+7]=buff_page_exp[(i-1)*32+9];
                        buff_page_exp_tx[k*25+8]=buff_page_exp[(i-1)*32+10];
                        buff_page_exp_tx[k*25+9]=buff_page_exp[(i-1)*32+11];
                        buff_page_exp_tx[k*25+10]=buff_page_exp[(i-1)*32+12];
                        buff_page_exp_tx[k*25+11]=buff_page_exp[(i-1)*32+13];
                        buff_page_exp_tx[k*25+12]=buff_page_exp[(i-1)*32+14];
                        buff_page_exp_tx[k*25+13]=buff_page_exp[(i-1)*32+15];
                        buff_page_exp_tx[k*25+14]=buff_page_exp[(i-1)*32+16];
                        buff_page_exp_tx[k*25+15]=buff_page_exp[(i-1)*32+17];
                        buff_page_exp_tx[k*25+16]=buff_page_exp[(i-1)*32+18];
                        buff_page_exp_tx[k*25+17]=buff_page_exp[(i-1)*32+19];
                        buff_page_exp_tx[k*25+18]=buff_page_exp[(i-1)*32+20];
                        buff_page_exp_tx[k*25+19]=buff_page_exp[(i-1)*32+21];
                        buff_page_exp_tx[k*25+20]=buff_page_exp[(i-1)*32+22];
                        buff_page_exp_tx[k*25+21]=buff_page_exp[(i-1)*32+23];
                        buff_page_exp_tx[k*25+22]=buff_page_exp[(i-1)*32+24];
                        buff_page_exp_tx[k*25+23]=buff_page_exp[(i-1)*32+25];
                        buff_page_exp_tx[k*25+24]=0xff;
//    	                    Switch_e1e2_data(buff_page_exp_tx[k*25+22],(k*25+14));
                        ++k;
/////    	                 }
                    }
                }
                if(adds_a==0x00001004)
                    CW_export_index_adds_point=0x10040000;
                else
                {
                    adds_a-=1;
                    adds_a<<=16;
                    CW_export_index_adds_point=adds_a;

                    if(adds_a==0x10040000)
                        CW_export_index_adds_point=0x10040001;
                }
                if(k!=0)
                {
                    export_tx_lenth=k*25;
                    export_tx_flag=1;
                    export_tx_lenth_cnt=0;
                    DrvUART_Write(UART_PORT0,buff_page_exp_tx,1);
                    En_Uart012_THRE_int(UART_PORT0);
                }
            }
        }
        break;
    case 4://报警
        if(export_tx_flag==0)
        {
            adds_a=adds_b=BJ_export_index_adds_point;
            adds_b&=0x000001ff;
            adds_a>>=16;
            adds_a&=0x0000ffff;				//page adds

            if((adds_a==0x00001804)&&(adds_b==0x00000000))		//get first adds
            {
                for(i=0; i<26; i++)
                    buff_page_exp_tx[i]=0x00;

                export_data_select=0;
                Export_data_flag=0;
                Set_en_buzz_finc(2,4,200,100);
            }
            else
            {
                Spi_Page_Read(adds_a,buff_page_exp);

                k=0;
                for(i=16; i>0; i--)
                {
                    sum=0;
                    sum=buff_page_exp[(i-1)*0x20+9]+buff_page_exp[(i-1)*0x20+10]+buff_page_exp[(i-1)*0x20+11]+buff_page_exp[(i-1)*0x20+12];
                    if((buff_page_exp[(i-1)*0x20+3]==0x20)&&(buff_page_exp[(i-1)*0x20+5]<=0x12)&&(buff_page_exp[(i-1)*0x20+7]<0x24))
                    {
/////    	                 if(sum>=0x00)
/////    	                 {
                        buff_page_exp_tx[k*25+0]=buff_page_exp[(i-1)*32+2];
                        buff_page_exp_tx[k*25+1]=buff_page_exp[(i-1)*32+3];
                        buff_page_exp_tx[k*25+2]=buff_page_exp[(i-1)*32+4];
                        buff_page_exp_tx[k*25+3]=buff_page_exp[(i-1)*32+5];
                        buff_page_exp_tx[k*25+4]=buff_page_exp[(i-1)*32+6];
                        buff_page_exp_tx[k*25+5]=buff_page_exp[(i-1)*32+7];
                        buff_page_exp_tx[k*25+6]=buff_page_exp[(i-1)*32+8];
                        buff_page_exp_tx[k*25+7]=buff_page_exp[(i-1)*32+9];
                        buff_page_exp_tx[k*25+8]=buff_page_exp[(i-1)*32+10];
                        buff_page_exp_tx[k*25+9]=buff_page_exp[(i-1)*32+11];
                        buff_page_exp_tx[k*25+10]=buff_page_exp[(i-1)*32+12];
                        buff_page_exp_tx[k*25+11]=buff_page_exp[(i-1)*32+13];
                        buff_page_exp_tx[k*25+12]=buff_page_exp[(i-1)*32+14];
                        buff_page_exp_tx[k*25+13]=buff_page_exp[(i-1)*32+15];
                        buff_page_exp_tx[k*25+14]=buff_page_exp[(i-1)*32+16];
                        buff_page_exp_tx[k*25+15]=buff_page_exp[(i-1)*32+17];
                        buff_page_exp_tx[k*25+16]=buff_page_exp[(i-1)*32+18];
                        buff_page_exp_tx[k*25+17]=buff_page_exp[(i-1)*32+19];
                        buff_page_exp_tx[k*25+18]=buff_page_exp[(i-1)*32+20];
                        buff_page_exp_tx[k*25+19]=buff_page_exp[(i-1)*32+21];
                        buff_page_exp_tx[k*25+20]=buff_page_exp[(i-1)*32+22];
                        buff_page_exp_tx[k*25+21]=buff_page_exp[(i-1)*32+23];
                        buff_page_exp_tx[k*25+22]=buff_page_exp[(i-1)*32+24];
                        buff_page_exp_tx[k*25+23]=buff_page_exp[(i-1)*32+25];
                        buff_page_exp_tx[k*25+24]=0xff;
//    	                    Switch_e1e2_data(buff_page_exp_tx[k*25+22],(k*25+14));
                        ++k;
/////    	                 }
                    }
                }
                if(adds_a==0x00001804)
                    BJ_export_index_adds_point=0x18040000;
                else
                {
                    adds_a-=1;
                    adds_a<<=16;
                    BJ_export_index_adds_point=adds_a;

                    if(adds_a==0x18040000)
                        BJ_export_index_adds_point=0x18040001;
                }
                if(k!=0)
                {
                    export_tx_lenth=k*25;
                    export_tx_flag=1;
                    export_tx_lenth_cnt=0;
                    DrvUART_Write(UART_PORT0,buff_page_exp_tx,1);
                    En_Uart012_THRE_int(UART_PORT0);
                }
            }
        }
        break;
    }
}

//===============================================================================
//	finction	:Back_system_reg
//	edit		:sam 2012-10-24 10:21
//	description	:备份系统配置:从SYSTEM_REG_PAGE地址读取数据保存到BACK_SYSTEM_REG_ADDS中
//===============================================================================
void Back_system_reg(void)
{
    Spi_Page_Read(SYSTEM_REG_PAGE,buff_page);
    Spi_Page_Write(BACK_SYSTEM_REG_ADDS,buff_page);
}

//===============================================================================
//	finction	:Load_bak_system_reg
//	edit		:sam 2012-10-24 10:21
//	description	:将BACK_SYSTEM_REG_ADDS中的数据保存到SYSTEM_REG_PAGE地址中
//              :其中分析/校正/错误/报警的读写地址要是当前地址
//===============================================================================
void Load_bak_system_reg(void)
{
    uint8_t i;
	//将地址写入buff_page_w缓存中
    Buff_get_reg_32bit_data(4,FX_write_index_adds_point);//从4开始是为了方便下面的for循环
    Buff_get_reg_32bit_data(8,FX_read_index_adds_point);
    Buff_get_reg_32bit_data(12,JZ_write_index_adds_point);
    Buff_get_reg_32bit_data(16,JZ_read_index_adds_point);
    Buff_get_reg_32bit_data(20,CW_write_index_adds_point);
    Buff_get_reg_32bit_data(24,CW_read_index_adds_point);
    Buff_get_reg_32bit_data(28,BJ_write_index_adds_point);
    Buff_get_reg_32bit_data(32,BJ_read_index_adds_point);

    Spi_Page_Read(BACK_SYSTEM_REG_ADDS, buff_page);
    for(i=0; i<32; i++) 
		buff_page[i+4] = buff_page_w[i+4];
    Spi_Page_Write(SYSTEM_REG_PAGE, buff_page);
}


//===============================================================================
//	finction	:Rd_check_lcd_data
//	edit		:sam 2013-3-15 10:33
//  return      :1:有字库更新, 0:无字库更新
//	description	:判断字符是否有更新
//===============================================================================
uint8_t Rd_check_lcd_data(void)
{
    uint32_t adds_a,temp;

    adds_a = (DATA_HZ_END_ADDS >> 16) & 0xffff;//page adds

    Spi_Page_Read(adds_a,buff_page);
    temp=(buff_page[0]|(buff_page[1]<<8)|(buff_page[2]<<16)|(buff_page[3]<<24));
    if(temp!=0xAAAA5555) 
			return(1);
    else 
			return(0);
}

//===============================================================================
//	finction	:Check_updta_lcd_data
//	edit		:sam 2013-3-15 10:33
//	description	:更新字库,将sd卡中的字库读取,写入到flash中
//===============================================================================
void Check_updta_lcd_data(void)
{
    uint8_t   j,rs;
    uint16_t  i;
    uint32_t  adds_a;

    if((DrvGPIO_GetBit(E_GPC,0))==0)//检测是否有SD卡
    {
        SD_read_flag=read_file_lenth_cnt=0;
        Sd_read_file(0x00545854,0x44415441,0x36382020);		//DATA68,读取数据到SD_data_buff中
        for(i=0; i<512; i++)
        {
            if(SD_data_buff[i]<0x3a)
                SD_data_buff[i]-=0x30;
            else
                SD_data_buff[i]-=0x37;

            buff_page[i]=0;
        }
		//初始化Buff_68与buff_page中的第0-255位的数据,并将SD_data_buff中的数据赋为0x30
        for(i=0; i<256; i++)
        {
            Buff_68[i] = buff_page[i] = ((SD_data_buff[i*2]<<4)|SD_data_buff[i*2+1]);
            SD_data_buff[i*2] = SD_data_buff[i*2+1] = 0x30;
        }
		
        Sd_read_file(0x00545854,0x44415441,0x36382020);		//DATA68
        for(i=0; i<512; i++)
        {
            if(SD_data_buff[i]<0x3a)
                SD_data_buff[i]-=0x30;
            else
                SD_data_buff[i]-=0x37;
        }
		//初始化Buff_68与buff_page中的第256-511位的数据,并将SD_data_buff中的数据赋为0x30
        for(i=256; i<512; i++)
        {
            Buff_68[i] = buff_page[i] = ((SD_data_buff[(i-256)*2]<<4)|SD_data_buff[(i-256)*2+1]);
            SD_data_buff[(i-256)*2] = SD_data_buff[(i-256)*2+1] = 0x30;
        }
        adds_a=(DATA_6X8_ADDS>>16) & 0xffff;//page adds
        Spi_Page_Write(adds_a,buff_page);//
//--------------------------------------------------------------------------
        SD_read_flag=read_file_lenth_cnt=0;
        for(j=0; j<2; j++)
        {
            rs=Sd_read_file(0x00545854,0x44415441,0x38313620);		//DATA816
            for(i=0; i<512; i++)
            {
                if(SD_data_buff[i]<0x3a)
                    SD_data_buff[i]-=0x30;
                else
                    SD_data_buff[i]-=0x37;

                buff_page[i]=0;
            }
            for(i=0; i<256; i++)
            {
                Buff_816[j*512+i]=buff_page[i]=((SD_data_buff[i*2]<<4)|SD_data_buff[i*2+1]);
                SD_data_buff[i*2]=SD_data_buff[i*2+1]=0x30;
            }
            if(rs==2) goto w1_ex;
            rs=Sd_read_file(0x00545854,0x44415441,0x38313620);		//DATA816
            for(i=0; i<512; i++)
            {
                if(SD_data_buff[i]<0x3a)
                    SD_data_buff[i]-=0x30;
                else
                    SD_data_buff[i]-=0x37;
            }
            for(i=256; i<512; i++)
            {
                Buff_816[j*512+i]=buff_page[i]=((SD_data_buff[(i-256)*2]<<4)|SD_data_buff[(i-256)*2+1]);
                SD_data_buff[(i-256)*2]=SD_data_buff[(i-256)*2+1]=0x30;
            }
w1_ex:
            adds_a=DATA_8X16_ADDS;
            adds_a>>=16;
            adds_a&=0x0000ffff;				//page adds
            adds_a+=j;
            Spi_Page_Write(adds_a,buff_page);
            DrvSYS_Delay(6000000);
            if(rs==2) 
				break;
        }
//--------------------------------------------------------------------------
        SD_read_flag=read_file_lenth_cnt=0;
        for(j=0; j<12; j++)
        {
            rs=Sd_read_file(0x00545854,0x44415441,0x485a2020);		//DATAHZ
            for(i=0; i<512; i++)
            {
                if(SD_data_buff[i]<0x3a)
                    SD_data_buff[i]-=0x30;
                else
                    SD_data_buff[i]-=0x37;

                buff_page[i]=0;
            }
            for(i=0; i<256; i++)
            {
                buff_page[i]=((SD_data_buff[i*2]<<4)|SD_data_buff[i*2+1]);
                SD_data_buff[i*2]=SD_data_buff[i*2+1]=0x30;
            }
            if(rs==2) 
							goto w2_ex;
						
            rs=Sd_read_file(0x00545854,0x44415441,0x485a2020);		//DATAHZ
            for(i=0; i<512; i++)
            {
                if(SD_data_buff[i]<0x3a)
                    SD_data_buff[i]-=0x30;
                else
                    SD_data_buff[i]-=0x37;
            }
            for(i=256; i<512; i++)
            {
                buff_page[i]=((SD_data_buff[(i-256)*2]<<4)|SD_data_buff[(i-256)*2+1]);
                SD_data_buff[(i-256)*2]=SD_data_buff[(i-256)*2+1]=0x30;
            }
w2_ex:
            adds_a=DATA_HZ_START_ADDS;
            adds_a>>=16;
            adds_a&=0x0000ffff;				//page adds
            adds_a+=j;
            Spi_Page_Write(adds_a,buff_page);
            DrvSYS_Delay(6000000);
            if(rs==2) break;
        }
    }
    else
    {
        while(1) {}
    }
//-------------------------------write flag
    adds_a=DATA_HZ_END_ADDS>>16;//page adds

    for(i=0; i<512; i++)
        buff_page[i]=0;

    buff_page[0]=buff_page[1]=0x55;
    buff_page[2]=buff_page[3]=0xAA;
    Spi_Page_Write(adds_a,buff_page);
}

//===============================================================================
//	finction	:Rd_check_lcd_data
//	edit		:sam 2013-3-15 10:33
//	description	:从flash中读取字库文件到缓存数组中
//===============================================================================
void Rd_init_lcd_data(void)
{
    uint16_t  i;
    uint32_t  adds_a;

    adds_a=DATA_6X8_ADDS>>16;     //page adds
    Spi_Page_Read(adds_a,Buff_68);//6x8 data

    adds_a=DATA_8X16_ADDS>>16;     //page adds
    Spi_Page_Read(adds_a,Buff_816);//8x16 data

    adds_a=(DATA_8X16_ADDS>>16) + 1;//page adds
    Spi_Page_Read(adds_a,buff_page);//8x16 data

    for(i=0; i<512; i++)
        Buff_816[i+512] = buff_page[i];
}
