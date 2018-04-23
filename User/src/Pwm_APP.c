//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Pwm_APP.c
//	First EDIT		: Sam
//	Data			: 2012.06.19
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================
#define		__PWM_APP__
//--------------------------------------------------------------------------------------------
#include "Incude_all.h"
//--------------------------------------------------------------------------------------------

//===============================================================================
//	finction	:Stop_pwm0
//	edit		:sam 2012.06.19
//	description	:未调用
//===============================================================================
void Stop_pwm0(void)
{
    PWMA->PCR.CH0EN=0;			//PWM function->0:Disable, 1:Enable
}

//===============================================================================
//	finction	:Start_pwm0
//	edit		:sam 2012.06.19
//	description	:未调用
//===============================================================================
void Start_pwm0(void)
{
    PWMA->PCR.CH0EN=1;			//PWM function->0:Disable, 1:Enable
}

//===============================================================================
//	finction	:Stop_pwm1
//	edit		:sam 2012.06.19
//	description	:未调用
//===============================================================================
void Stop_pwm1(void)
{
    PWMA->PCR.CH1EN=0;			//PWM function->0:Disable, 1:Enable
}

//===============================================================================
//	finction	:Start_pwm1
//	edit		:sam 2012.06.19
//	description	:未调用
//===============================================================================
void Start_pwm1(void)
{
    PWMA->PCR.CH1EN=1;			//PWM function->0:Disable, 1:Enable
}

//===============================================================================
//	finction	:Adj_pwm0_cnr_cmr_data
//	input		:null
//	edit		:sam 2012.06.19
//	description	:未调用
//===============================================================================
void Adj_pwm0_cnr_cmr_data(uint32_t d_cnr,uint32_t d_cmr)
{
    PWMA->CNR0=d_cnr;			//
    PWMA->CMR0=d_cmr;			//chang cnr cmr data to set diffrent duty
}

//===============================================================================
//	finction	:Adj_pwm1_cnr_cmr_data
//	edit		:sam 2012.06.19
//	description	:未调用
//===============================================================================
void Adj_pwm1_cnr_cmr_data(uint32_t d_cnr,uint32_t d_cmr)
{
    PWMA->CNR1=d_cnr;			//
    PWMA->CMR1=d_cmr;			//chang cnr cmr data to set diffrent duty
}

//===============================================================================
//	finction	:InitPWM
//	edit		:sam 2012.06.19
//	modefy		:初始化PWM输出
//===============================================================================
void InitPWM(void)
{
// 	/* Step 1. GPIO initial */
//	SYS->GPAMFP.PWM0_AD13=1;

    /* Step 2. Enable and Select PWM clock source*/
    SYSCLK->APBCLK.PWM01_EN = 3;		//Enable PWM clock
    SYSCLK->CLKSEL1.PWM01_S = 2;		//Select PLL for PWM clock source

    PWMA->PPR.CP01=1;			//Prescaler 0~255, Setting 0 to stop output clock
    PWMA->CSR.CSR0=4;			// PWM0 clock = clock source/(Prescaler + 1)/divider
    PWMA->CSR.CSR1=4;			// PWM1 clock = clock source/(Prescaler + 1)/divider

    /* Step 3. Select PWM Operation mode */
    //PWM0
    PWMA->PCR.CH0MOD=1;			//0:One-shot mode, 1:Auto-load mode
    //CNR and CMR will be auto-cleared after setting CH0MOD form 0 to 1.
/////	PWMA->CNR0=0x01FF;			//set 50%duty
/////	PWMA->CMR0=0x01FE;			//chang cnr cmr data to set diffrent duty
    PWMA->CNR0=0x1388;			//set 50%duty
    PWMA->CMR0=0x03b8;			//chang cnr cmr data to set diffrent duty
    //PWM1
    PWMA->PCR.CH1MOD=1;			//0:One-shot mode, 1:Auto-load mode
/////	PWMA->CNR1=0x01FF;			//set 50%duty circle
/////	PWMA->CMR1=0x00FF;			//change cmr cmr data to set diffrent duty
    PWMA->CNR1=0x1388;			//set 50%duty circle
    PWMA->CMR1=0x03b8;			//change cmr cmr data to set diffrent duty

    PWMA->PCR.DZEN01=0;			//Dead-Zone->0:off, 1:on
    PWMA->PCR.CH0INV=0;			//Inverter->0:off, 1:on
    PWMA->PCR.CH0EN=0;			//PWM function->0:Disable, 1:Enable
    PWMA->POE.PWM0=1;			//Output to pin->0:Diasble, 1:Enable
    PWMA->PCR.CH1INV=0;			//Inverter->0:off, 1:on
    PWMA->PCR.CH1EN=0;			//PWM function->0:Disable, 1:Enable
    PWMA->POE.PWM1=1;			//Output to pin->0:Diasble, 1:Enable
    Pwm_out_adj_flag=2;
}

//===============================================================================
//	finction	:InitPWM_4
//	edit		:sam 2012.06.19
//	description	:配置PB11为PWM输出,用于控制LCD背光
//===============================================================================
void InitPWM_4(void)
{
    DrvPWM_Open();
    //PWM4
    DrvGPIO_InitFunction(E_FUNC_PWM4);	//PB11:PWM4

    SYSCLK->APBCLK.PWM45_EN = 3;		//Enable PWM clock
    SYSCLK->CLKSEL2.PWM45_S = 2;		//Select PLL for PWM clock source
    PWMB->PPR.CP01=1;
    PWMB->CSR.CSR0=1;			// PWM4 clock = clock source/(Prescaler + 1)/divider

    PWMB->PCR.CH0MOD=1;			//0:One-shot mode, 1:Auto-load mode
    //CNR and CMR will be auto-cleared after setting CH0MOD form 0 to 1.
    PWMB->CNR0=0x01FF;			//set 50%duty
    PWMB->CMR0=0x00FF;			//chang cnr cmr data to set diffrent duty

    PWMB->PCR.DZEN01=0;			//Dead-Zone->0:off, 1:on
    PWMB->PCR.CH0INV=0;			//Inverter->0:off, 1:on
    PWMB->PCR.CH0EN=1;			//PWM function->0:Disable, 1:Enable
    PWMB->POE.PWM0=1;			//Output to pin->0:Diasble, 1:Enable

    LCD_bl_ctrl_data=2;
}
//===============================================================================
//	finction	:Pwm_output_test
//  input       :ch: 1=通道0; 2=通道1
//	edit		:sam 2012-9-20 9:53
//	description	:配置PWM输出
//===============================================================================
void Pwm_output_test(uint8_t ch,uint32_t CMR_Value)
{
    float r_data = 0;

    r_data =  ((CMR_Value>>12)& 0xf) * 10;
    r_data +=  (CMR_Value>>8) & 0xf;
    r_data += ((CMR_Value>>4) & 0xf) / 10.0;
    r_data +=  (CMR_Value     & 0xf) / 100.0;

    if(ch==1)
    {
        r_data *= (adj_20ma_pwm_data-adj_4ma_pwm_data)/16.0;

        PWMA->PCR.CH0EN=0;
        PWMA->CMR0 = r_data;
        PWMA->PCR.CH0EN=1;
    }
    else if(ch==2)
    {
        r_data *=(adj_20ma_pwm1_data-adj_4ma_pwm1_data)/16.0;
        PWMA->PCR.CH1EN=0;
        PWMA->CMR1=r_data;
        PWMA->PCR.CH1EN=1;
    }
}

//===============================================================================
//	finction	:Pwm_int2float
//  input       :
//	edit		:sam 2012-9-20 9:53
//	modefy		:整形转换为浮点型
//===============================================================================
float Pwm_int2float(uint8_t sele,uint32_t data)
{
    float r_data = 0;
    uint32_t i;

    if(sele==2)
    {
        r_data = ((data>>20) & 0xf) * 1000;
        r_data+= ((data>>16) & 0xf) * 100;
		r_data+= ((data>>12) & 0xf) * 10;
		r_data+= ((data>>8 ) & 0xf);
		r_data+= ((data>>4 ) & 0xf) / 10;
		r_data+= ( data      & 0xf) / 100;
    }
    else
    {
        r_data = ((data>>8) & 0xf) * 10;
		r_data+= ((data>>4) & 0xf);
		r_data+= ((data)    & 0xf) / 10;
    }

    i = data & 0x80000000;
    if(i==0x80000000)//判断符号位
        r_data = -r_data;

    return(r_data);
}

//===============================================================================
//	finction	:Pwm_float2int
//	input		:null
//	return		:null
//	edit		:sam 2012-9-20 9:53
//	description	:null
//===============================================================================
uint32_t Pwm_float2int(float data)
{
    uint32_t i,r_data;
    i = data * 100.0;
    r_data = i % 10;		         //0.01
    r_data |= (( (i/10)  % 10) << 4);//0.1
    r_data |= (( (i/100) % 10) << 8);//1
    r_data |= (  (i/1000)      << 12);//10
    return(r_data);
}

//===============================================================================
//	finction	:Pwm_get_resul_from_flash
//	input		:select: 1=结果; 2=校正 ; 3=温度 
//	edit		:sam 2012-8-6 15:01
//	description	:null
//===============================================================================
void Pwm_get_resul_from_flash(uint8_t select)
{
    uint32_t adds_a,adds_b,adds_bk=0;
    float r_data=0,t1,t2;

    if(select==3)
    {
        t1=(E_ADC_temperature1-1024)/10.0;
        t2=(t1-1024)/10.0;
        Pwm_last_result = (t1+t2)/2.0;
    }
    else
    {
        if(select==1)
            adds_bk = FX_write_index_adds_point;//分析选择
        else if(select==2)
            adds_bk = JZ_write_index_adds_point;//校正选择

sub_case11:
        adds_b = adds_bk & 0x000001ff;		//byte adds
        adds_a = adds_bk >> 16;				//page adds

        if( (adds_b==0x00000000) && (adds_a==0x00000004) )		//get first adds
            goto sub_case12;

        else if(adds_b==0x00000000)//向下溢出
        {
            adds_b=0x1e0;
            adds_a-=1;
        }
        else
            adds_b-=0x20;//上一条数据

        Spi_Page_Read(adds_a,buff_page);    			//read a page data
        adds_bk= (adds_a<<16) + adds_b;

        if(select==2)
            goto p_sub;
        if( (buff_page[adds_b+2] == 3) || (buff_page[adds_b+2] == 5) )	//check status
        {
p_sub:
            r_data += (buff_page[adds_b+9] >> 4 ) * 1000;
            r_data += (buff_page[adds_b+9] & 0xf) * 100;
            r_data += (buff_page[adds_b+10] >> 4) * 10;
            r_data += (buff_page[adds_b+10] & 0xf);
            r_data += (buff_page[adds_b+11]>>4)/10.0;
            r_data += (buff_page[adds_b+11] & 0xf)/100.0;
            r_data += (buff_page[adds_b+12]>>4)/1000.0;

            Pwm_last_result = r_data;

            if(buff_page[adds_b+15]==2)				//signe_result
                Pwm_last_result=-Pwm_last_result;
        }
        else 
			goto sub_case11;
sub_case12:
        __nop();
    }
}

//===============================================================================
//	finction	:Pwm_output_ctrl
//	edit		:sam 2012-9-20 9:53
//	modefy		:null
//===============================================================================
void Pwm_output_ctrl(void)
{
    float rs,a=0,b=0,c,d;
    uint32_t rds;

    if(Pwm_out_adj_flag==2)					//power on
    {
        if((M_lever4_Out1_4ma_temp==0) && (M_lever4_Out1_20ma_temp==0))//4ma对应值 与 20ma对应值 为0
            Pwm_output_test(1,0x400);
        else
        {
            a=Pwm_int2float(1,M_lever4_Out1_4ma_temp);//4ma对应值保留小数点前两位,小数点后一位
            b=Pwm_int2float(2,M_lever4_Out1_20ma_temp);//20ma对应值保留小数点前4位,小数量后两位
            c=b-a;
            d=Pwm_last_result-a;
            rs=4 + 16*d/c;

            rds=Pwm_float2int(rs);
            if(rds<0x00000400)
                rds=0x00000400;
            else if(rds>0x00002000)
                rds=0x00002000;

            Pwm_output_test(1,rds);
        }
		
        if((M_lever4_Out2_4ma_temp==0) && (M_lever4_Out2_20ma_temp==0))
            Pwm_output_test(2,0x00000400);
        else
        {
            a=Pwm_int2float(1,M_lever4_Out2_4ma_temp);
            b=Pwm_int2float(2,M_lever4_Out2_20ma_temp);
            c=b-a;
            d=Pwm_last_result-a;
            rs=4+16*d/c;

            rds=Pwm_float2int(rs);
            if(rds<0x00000400)
                rds=0x00000400;
            else if(rds>0x00002000)
                rds=0x00002000;

            Pwm_output_test(2,rds);
        }
        Pwm_out_adj_flag=0;
    }
    else if( (Pwm_menu_lever==3) && (Pwm_out_adj_flag==1) )		//change
    {
        if(Menu_lever3_select==1)//输出1结果
        {
            switch(Pwm_menu_select)
            {
            case 1:						//result
                Pwm_get_resul_from_flash(1);
                a=Pwm_int2float(1,M_lever4_Out1_4ma_temp);
                b=Pwm_int2float(2,M_lever4_Out1_20ma_temp);
                break;
            case 2:						//temp
                Pwm_get_resul_from_flash(3);
                a=Pwm_int2float(1,M_lever4_tp_Out1_4ma_temp);
                b=Pwm_int2float(2,M_lever4_tp_Out1_20ma_temp);
                break;
            case 3:						//jiaozheng
                Pwm_get_resul_from_flash(2);
                a=Pwm_int2float(1,M_lever4_jz_Out1_4ma_temp);
                b=Pwm_int2float(2,M_lever4_jz_Out1_20ma_temp);
                break;
            }
        }
        else if(Menu_lever3_select==2)//输出2结果
        {
            switch(Pwm_menu_select)
            {
            case 1:						//result
                Pwm_get_resul_from_flash(1);
                a=Pwm_int2float(1,M_lever4_Out2_4ma_temp);
                b=Pwm_int2float(2,M_lever4_Out2_20ma_temp);
                break;
            case 2:						//temp
                Pwm_get_resul_from_flash(3);
                a=Pwm_int2float(1,M_lever4_tp_Out2_4ma_temp);
                b=Pwm_int2float(2,M_lever4_tp_Out2_20ma_temp);
                break;
            case 3:						//jiaozheng
                Pwm_get_resul_from_flash(2);
                a=Pwm_int2float(1,M_lever4_jz_Out2_4ma_temp);
                b=Pwm_int2float(2,M_lever4_jz_Out2_20ma_temp);
                break;
            }
        }
        c=b-a;
        d=Pwm_last_result-a;
        rs=4+16*d/c;

        rds=Pwm_float2int(rs);
        if(rds<0x00000400)
            rds=0x00000400;
        else if(rds>0x00002000)
            rds=0x00002000;

        Pwm_output_test(Menu_lever3_select,rds);

        Pwm_out_adj_flag=0;
    }
}

