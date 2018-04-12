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
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.19
//	modefy		:null
//===============================================================================
void Stop_pwm0(void)
{
    PWMA->PCR.CH0EN=0;			//PWM function->0:Disable, 1:Enable
}

//===============================================================================
//	finction	:Start_pwm0
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.19
//	modefy		:null
//===============================================================================
void Start_pwm0(void)
{
    PWMA->PCR.CH0EN=1;			//PWM function->0:Disable, 1:Enable
}

//===============================================================================
//	finction	:Stop_pwm1
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.19
//	modefy		:null
//===============================================================================
void Stop_pwm1(void)
{
    PWMA->PCR.CH1EN=0;			//PWM function->0:Disable, 1:Enable
}

//===============================================================================
//	finction	:Start_pwm1
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.19
//	modefy		:null
//===============================================================================
void Start_pwm1(void)
{
    PWMA->PCR.CH1EN=1;			//PWM function->0:Disable, 1:Enable
}

//===============================================================================
//	finction	:Adj_pwm0_cnr_cmr_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.19
//	modefy		:null
//===============================================================================
void Adj_pwm0_cnr_cmr_data(uint32_t d_cnr,uint32_t d_cmr)
{
    PWMA->CNR0=d_cnr;			//
    PWMA->CMR0=d_cmr;			//chang cnr cmr data to set diffrent duty
}

//===============================================================================
//	finction	:Adj_pwm1_cnr_cmr_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.19
//	modefy		:null
//===============================================================================
void Adj_pwm1_cnr_cmr_data(uint32_t d_cnr,uint32_t d_cmr)
{
    PWMA->CNR1=d_cnr;			//
    PWMA->CMR1=d_cmr;			//chang cnr cmr data to set diffrent duty
}

//===============================================================================
//	finction	:InitPWM
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.19
//	modefy		:null
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
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.19
//	modefy		:null
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
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-20 9:53
//	modefy		:null
//===============================================================================
void Pwm_output_test(uint8_t ch,uint32_t data)
{
    float k,r_data;
    uint32_t i;

    r_data=0;

    i=data>>12;
    i&=0x0000000f;
    k=i;
    k*=10;
    r_data+=k;

    i=data>>8;
    i&=0x0000000f;
    k=i;
    r_data+=k;

    i=data>>4;
    i&=0x0000000f;
    k=i;
    k/=10.0;
    r_data+=k;

    i=data;
    i&=0x0000000f;
    k=i;
    k/=100.0;
    r_data+=k;

    if(ch==1)
    {
        k=adj_20ma_pwm_data-adj_4ma_pwm_data;
        k/=16.0;
        r_data*=k;
        i=r_data;

        PWMA->PCR.CH0EN=0;
        PWMA->CMR0=i;
        PWMA->PCR.CH0EN=1;
    }
    else if(ch==2)
    {
        k=adj_20ma_pwm1_data-adj_4ma_pwm1_data;
        k/=16.0;
        r_data*=k;
        i=r_data;

        PWMA->PCR.CH1EN=0;
        PWMA->CMR1=i;
        PWMA->PCR.CH1EN=1;
    }
}

//===============================================================================
//	finction	:Pwm_int2float
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-20 9:53
//	modefy		:null
//===============================================================================
float Pwm_int2float(uint8_t sele,uint32_t data)
{
    float k,r_data;
    uint32_t i;

    r_data=0;
    if(sele==2)
    {
        i=data>>20;
        i&=0x0000000f;
        k=i;
        k=k*1000;
        r_data+=k;

        i=data>>16;
        i&=0x0000000f;
        k=i;
        k=k*100;
        r_data+=k;

        i=data>>12;
        i&=0x0000000f;
        k=i;
        k=k*10;
        r_data+=k;

        i=data>>8;
        i&=0x0000000f;
        k=i;
        r_data+=k;

        i=data>>4;
        i&=0x0000000f;
        k=i;
        k=k/10;
        r_data+=k;

        i=data;
        i&=0x0000000f;
        k=i;
        k=k/100;
        r_data+=k;
    }
    else
    {
        i=data>>8;
        i&=0x0000000f;
        k=i;
        k=k*10;
        r_data+=k;

        i=data>>4;
        i&=0x0000000f;
        k=i;
        r_data+=k;

        i=data;
        i&=0x0000000f;
        k=i;
        k=k/10;
        r_data+=k;
    }

    i=data;
    i&=0x80000000;
    if(i==0x80000000)
        r_data=-r_data;

    return(r_data);
}

//===============================================================================
//	finction	:Pwm_float2int
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-20 9:53
//	modefy		:null
//===============================================================================
uint32_t Pwm_float2int(float data)
{
    uint32_t k,i,r_data;

    data*=100.0;

    i=data;
    k=i%10;
    r_data=k;		//0.01

    k=i/10;
    k%=10;
    k<<=4;
    r_data|=k;		//0.1

    k=i/100;
    k%=10;
    k<<=8;
    r_data|=k;		//1

    k=i/1000;
    k<<=12;
    r_data|=k;		//10

    return(r_data);
}

//===============================================================================
//	finction	:Pwm_get_resul_from_flash
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-8-6 15:01
//	modefy		:null
//===============================================================================
void Pwm_get_resul_from_flash(uint8_t select)
{
    uint32_t adds_a,adds_b,adds_bk=0;
    uint8_t i;
    float k,r_data=0,t1,t2;

    if(select==3)
    {
        t1=E_ADC_temperature1;
        t1=(t1-1024)/10.0;
        t2=E_ADC_temperature2;
        t2=(t1-1024)/10.0;

        Pwm_last_result=(t1+t2)/2.0;
    }
    else
    {
        if(select==1)
            adds_bk=FX_write_index_adds_point;
        else if(select==2)
            adds_bk=JZ_write_index_adds_point;

sub_case11:
        adds_a=adds_b=adds_bk;
        adds_b&=0x000001ff;				//byte adds
        adds_a>>=16;
        adds_a&=0x0000ffff;				//page adds

        if((adds_b==0x00000000)&&(adds_a==0x00000004))		//get first adds
            goto sub_case12;

        else if(adds_b==0x00000000)
        {
            adds_b=0x000001e0;
            adds_a-=1;
        }
        else
            adds_b-=0x20;

        Spi_read(adds_a,buff_page);    			//read opration
        adds_a<<=16;
        adds_bk=adds_a+adds_b;

        if(select==2)
            goto p_sub;
        if((buff_page[adds_b+2]==3)||(buff_page[adds_b+2]==5))	//check status
        {
p_sub:
            i=buff_page[adds_b+9]>>4;
            i&=0x0f;
            k=i;
            k*=1000;
            r_data+=k;
            i=buff_page[adds_b+9];
            i&=0x0f;
            k=i;
            k*=100;
            r_data+=k;

            i=buff_page[adds_b+10]>>4;
            i&=0x0f;
            k=i;
            k*=10;
            r_data+=k;
            i=buff_page[adds_b+10];
            i&=0x0f;
            k=i;
            r_data+=k;

            i=buff_page[adds_b+11]>>4;
            i&=0x0f;
            k=i;
            k/=10.0;
            r_data+=k;
            i=buff_page[adds_b+11];
            i&=0x0f;
            k=i;
            k/=100.0;
            r_data+=k;

            i=buff_page[adds_b+12]>>4;
            i&=0x0f;
            k=i;
            k/=1000.0;
            r_data+=k;

            Pwm_last_result=r_data;

            if(buff_page[adds_b+15]==2)				//signe_result
                Pwm_last_result=-Pwm_last_result;
        }
        else goto sub_case11;
sub_case12:
        __nop();
    }
}

//===============================================================================
//	finction	:Pwm_output_ctrl
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-20 9:53
//	modefy		:null
//===============================================================================
void Pwm_output_ctrl(void)
{
    float rs,a=0,b=0,c,d;
    uint32_t rds;

    if(Pwm_out_adj_flag==2)					//power on
    {
        if((M_lever4_Out1_4ma_temp==0)&&(M_lever4_Out1_20ma_temp==0))
            Pwm_output_test(1,0x00000400);
        else
        {
            a=Pwm_int2float(1,M_lever4_Out1_4ma_temp);
            b=Pwm_int2float(2,M_lever4_Out1_20ma_temp);
            c=b-a;
            d=Pwm_last_result-a;
            rs=4+16*d/c;

            rds=Pwm_float2int(rs);
            if(rds<0x00000400)
                rds=0x00000400;
            else if(rds>0x00002000)
                rds=0x00002000;

            Pwm_output_test(1,rds);
        }
        if((M_lever4_Out2_4ma_temp==0)&&(M_lever4_Out2_20ma_temp==0))
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
    else if((Pwm_menu_lever==3)&&(Pwm_out_adj_flag==1))		//change
    {
        if(Menu_lever3_select==1)
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
        else if(Menu_lever3_select==2)
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

