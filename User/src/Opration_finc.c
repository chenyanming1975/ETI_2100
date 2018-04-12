//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Opration_finc.c
//	First EDIT		: Sam
//	Data			: 2012-7-18 10:27
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================
#define		__OPRATION_F__
/////#define		__WD_BUCHANG__
#define		FIX_BJ_TEMP		1.06

//--------------------------------------------------------------------------------------------
#include "Incude_all.h"
volatile uint8_t 	ZB_flag;					//zb
volatile uint8_t 	Prog_total_step;				//分析步数
volatile uint8_t 	Prog_step_disp_cnt;				//显示分析步数
volatile uint8_t	program_menu_select;				//程序更新状态显示
uint8_t		buff_app[4];						//程序版本
uint8_t		buff_bool[4];						//程序引导版本

volatile uint8_t	Auto_check_clock_flag;				//自动分析判断时间标志
volatile float 		pk,pr_data;
volatile uint32_t 	pi;

volatile float		P_Tcall_temp;					//校正时的温度 2012-9-13 11:11

float		pub_math_temp;					//公用结果，计算寄存器2012-7-24 10:16
float		c1_temp;                                        //
float		c2_temp;                                        //
float		c3_temp;                                        //
float		c5_temp;                                        //
float		k1_temp;                                        //
float		k2_temp;                                        //
float		df_temp;                                        //
float		j_temp;
//
float		c1_prog_use;                                    //
float		c2_prog_use;                                    //
float		c3_prog_use;                                    //
//float		c5_prog_use;
float		k1_prog_use;                                    //
float		k2_prog_use;                                    //
float		j_prog_use;                                     //
uint8_t		k2_signe_flag;

uint8_t		yes_not;
uint32_t	Pub_result_temp;
uint32_t 	p_s2v_temp;
uint8_t		result_signe;					//分析结果的符号

uint8_t		M_lever4_FXCS_cnt;				//分析次数
volatile uint8_t	Pro_auto_time_up_check_flag;                    //自动分析时间检查标志
volatile uint32_t	Pro_auto_time_dcnt;                             //自动分析时间计时
uint16_t	Pro_auto_qingxi_freq_cnt;                       //清洗频率计数
uint16_t	Pro_auto_jiaozheng_freq_cnt;                    //校正频率计数
uint16_t	Pro_auto_hecha_freq_cnt;                        //核查频率计数

uint8_t		Pro_power_on_flag;                              //上电开机标志
uint8_t 	Pro_step_status;                                //分析步骤状态
volatile uint8_t 	Pro_wait_time_flag;                             //分析定时计时
volatile uint32_t	Pro_wait_time_set;                              //分析定时计时
uint8_t 	Pro_wait_time0_flag;                            //分析定时计时
uint32_t	Pro_wait_time0_set;                             //分析定时计时

uint32_t	Measurement_signal_11;                          //比色法光电管电信号值
uint32_t	Measurement_signal_12;                          //比色法光电管电信号值
uint32_t	Measurement_signal_21;                          //电极法电极电信号值
uint32_t	Measurement_signal_22;                          //电极法电极电信号值

uint32_t	e1_men_save;					//保存电压值E1
uint32_t	e2_men_save;					//保存电压值E2

uint32_t	K1_JZYZ;					//校正因子

volatile uint32_t	E_buff[7];					//
volatile uint32_t	Delta_buff[3];                                  //
volatile uint32_t	E_temp;                                         //
volatile uint32_t	Delta_temp;                                     //
volatile uint8_t	Delta_cnt;                                      //
//
volatile uint8_t	E_step_cnt;                                     //这些变量是用来读光电管电压或读电极电压值的
volatile uint8_t	E_conter;                                       //
volatile uint8_t	E_wait_flag;                                    //
volatile uint32_t	E_wait_time_set;                                //
volatile uint32_t	E_wait_time_cnt;                                //
volatile uint32_t	E_ADC_temperature1;                             //
volatile uint32_t	E_ADC_temperature2;                             //
volatile uint8_t	Prog_disp_flag;                                 //分析状态显示控制标志

uint8_t		Memu1_auto_step_status;					//分析步骤状态计数
uint8_t         Memu1_hand_step_status;                                 //
uint8_t         Memu1_jiaozheng_step_status;                            //
uint8_t         Memu1_biaoye_step_status;                               //
uint8_t         Memu1_xishi_step_status;                                //

volatile uint8_t	jiaozheng_sec_time_cnt;                         //校正次数计数

//===============================================================================
//	finction	:Read_pub_tx_tab
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2013-2-5 11:37
//	modefy		:null
//===============================================================================
void Read_pub_tx_tab(uint8_t select)		//读流程控制函数
{
    volatile uint16_t cnt,adds,data,wait_t;
    volatile uint8_t div;
    volatile uint8_t const *index;

    cnt=select*7;
    index=TB_pub_finc+cnt;

    div=(*index);

    ++index;
    adds=(*index);
    adds<<=8;
    ++index;
    adds|=(*index);

    ++index;
    data=(*index);
    data<<=8;
    ++index;
    data|=(*index);

    ++index;
    wait_t=(*index);
    wait_t<<=8;
    ++index;
    wait_t|=(*index);

    Set_tx_cmd_data_finc(div,0x06,adds,data,0x00);
    if(wait_t>0)
    {
        Pro_wait_time_set=wait_t;	//set wait time
    }
}

//===============================================================================
//	finction	:Pub_finc_index
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2013-2-5 11:37
//	modefy		:null
//===============================================================================
void Pub_finc_index(uint8_t const *index,uint8_t select)	//读流程表并执行对应的控制
{
    volatile uint8_t sl_data;

    sl_data=*(index+select);
    Read_pub_tx_tab(sl_data);
}

//===============================================================================
//	finction	:Get_Prj_correlat
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-14 14:37
//	modefy		:null
//===============================================================================
float Get_Prj_correlat(void)		//转换工程系数参数
{
    volatile float r_data,k;
    volatile uint32_t i,j;

    j=Prj_correlat;
    r_data=0;

    i=j>>20;
    i&=0x0000000f;
    k=i;
    k*=100;
    r_data+=k;

    i=j>>16;
    i&=0x0000000f;
    k=i;
    k*=10;
    r_data+=k;

    i=j>>12;
    i&=0x0000000f;
    k=i;
    r_data+=k;

    i=j>>8;
    i&=0x0000000f;
    k=i;
    k/=10.0;
    r_data+=k;

    i=j>>4;
    i&=0x0000000f;
    k=i;
    k/=100.0;
    r_data+=k;

    i=j;
    i&=0x0000000f;
    k=i;
    k/=1000.0;
    r_data+=k;

    i=j>>28;
    i&=0x0f;

    if(i==2)
        r_data=-r_data;

    return(r_data);
}

//===============================================================================
//	finction	:Get_e_wait_time
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-14 14:37
//	modefy		:null
//===============================================================================
uint32_t Get_e_wait_time(void)			//转换取得分析的测试时间
{
    volatile uint32_t r_data,i,j;

    j=E_wait_time_temp;
    r_data=0;

    i=j>>8;
    i&=0x0000000f;
    i*=100;
    r_data+=i;
    i=j>>4;
    i&=0x0000000f;
    i*=10;
    r_data+=i;
    i=j;
    i&=0x0000000f;
    r_data+=i;

    r_data*=100;

    return(r_data);
}

//===============================================================================
//	finction	:DDSJ_change
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-23 9:50
//	modefy		:null
//===============================================================================
uint32_t DDSJ_change(uint32_t tmr)			//转换取得分析的等待时间
{
    volatile uint8_t	i;
    volatile uint32_t	j;

    j=0;
    i=tmr>>8;
    i&=0x0f;
    i*=100;
    j+=i;

    i=tmr>>4;
    i&=0x0f;
    i*=10;
    j+=i;

    i=tmr;
    i&=0x0f;
    j+=i;

    j*=100;

    return(j);
}

//===============================================================================
//	finction	:Int_to_float_reg
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-23 9:50
//	modefy		:null
//===============================================================================
float Int_to_float_reg(uint32_t data)		//整形转为浮点
{
    volatile float i,m;
    volatile uint32_t a,b,k;

    a=data>>16;
    b=data;
    b&=0x0000ffff;

    i=0;
    k=a>>12;
    k&=0x0000000f;
    k*=1000;
    i+=k;
    k=a>>8;
    k&=0x0000000f;
    k*=100;
    i+=k;
    k=a>>4;
    k&=0x0000000f;
    k*=10;
    i+=k;
    k=a;
    k&=0x0000000f;
    k*=1;
    i+=k;
//--------------------------------
    k=b>>12;
    k&=0x0000000f;
    m=k;
    m/=10.0;
    i+=m;
    k=b>>8;
    k&=0x0000000f;
    m=k;
    m/=100.0;
    i+=m;
    k=b>>4;
    k&=0x0000000f;
    m=k;
    m/=1000.0;
    i+=m;
    k=b;
    k&=0x0000000f;
    m=k;
    m/=10000.0;
    i+=m;

    return(i);
}

//===============================================================================
//	finction	:Float_to_int_reg
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-23 9:50
//	modefy		:null
//===============================================================================
uint32_t fti_jtp;

uint32_t Float_to_int_reg(float data)		//浮点转为整形
{
    volatile uint32_t i,zs,ss,r_data;
    volatile float a;

    fti_jtp=r_data=0;

    a=data*10000;
    i=a;
    zs=data;
    ss=i-zs*10000;

    fti_jtp=zs/1000;
    fti_jtp%=10;
    fti_jtp<<=28;
    r_data+=fti_jtp;

    fti_jtp=zs/100;
    fti_jtp%=10;
    fti_jtp<<=24;
    r_data+=fti_jtp;

    fti_jtp=zs/10;
    fti_jtp%=10;
    fti_jtp<<=20;
    r_data+=fti_jtp;

    fti_jtp=zs;
    fti_jtp%=10;
    fti_jtp<<=16;
    r_data+=fti_jtp;
//------------------------
    fti_jtp=ss/1000;
    fti_jtp<<=12;
    r_data+=fti_jtp;

    fti_jtp=ss/100;
    fti_jtp%=10;
    fti_jtp<<=8;
    r_data+=fti_jtp;

    fti_jtp=ss/10;
    fti_jtp%=10;
    fti_jtp<<=4;
    r_data+=fti_jtp;

    fti_jtp=ss/1;
    fti_jtp%=10;
    r_data+=fti_jtp;

    return(r_data);
}

//===============================================================================
//	finction	:Float_to_int_reg_ee
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-23 9:50
//	modefy		:null
//===============================================================================
uint32_t Float_to_int_reg_ee(float data)		//浮点转为整形
{
    volatile uint32_t i,zs,ss,r_data;
    volatile float a;

    r_data=0;
    data=data/10.0;

    a=data*10000;
    i=a;
    zs=data;
    ss=i-zs*10000;

    fti_jtp=zs/1000;
    fti_jtp%=10;
    fti_jtp<<=28;
    r_data+=fti_jtp;

    fti_jtp=zs/100;
    fti_jtp%=10;
    fti_jtp<<=24;
    r_data+=fti_jtp;

    fti_jtp=zs/10;
    fti_jtp%=10;
    fti_jtp<<=20;
    r_data+=fti_jtp;

    fti_jtp=zs;
    fti_jtp%=10;
    fti_jtp<<=16;
    r_data+=fti_jtp;
//------------------------
    fti_jtp=ss/1000;
    fti_jtp%=10;
    fti_jtp<<=12;
    r_data+=fti_jtp;

    fti_jtp=ss/100;
    fti_jtp%=10;
    fti_jtp<<=8;
    r_data+=fti_jtp;

    fti_jtp=ss/10;
    fti_jtp%=10;
    fti_jtp<<=4;
    r_data+=fti_jtp;

    fti_jtp=ss/1;
    fti_jtp%=10;
    r_data+=fti_jtp;

    return(r_data);
}

//===============================================================================
//	finction	:Float_to_int_reg_ee2
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-23 9:50
//	modefy		:null
//===============================================================================
uint32_t Float_to_int_reg_ee2(float data)		//浮点转为整形
{
    volatile uint32_t i,zs,ss,r_data;
    volatile float a;

    r_data=0;
    data=data/100.0;

    a=data*10000;
    i=a;
    zs=data;
    ss=i-zs*10000;

    fti_jtp=zs/1000;
    fti_jtp%=10;
    fti_jtp<<=28;
    r_data+=fti_jtp;

    fti_jtp=zs/100;
    fti_jtp%=10;
    fti_jtp<<=24;
    r_data+=fti_jtp;

    fti_jtp=zs/10;
    fti_jtp%=10;
    fti_jtp<<=20;
    r_data+=fti_jtp;

    fti_jtp=zs;
    fti_jtp%=10;
    fti_jtp<<=16;
    r_data+=fti_jtp;
//------------------------
    fti_jtp=ss/1000;
    fti_jtp%=10;
    fti_jtp<<=12;
    r_data+=fti_jtp;

    fti_jtp=ss/100;
    fti_jtp%=10;
    fti_jtp<<=8;
    r_data+=fti_jtp;

    fti_jtp=ss/10;
    fti_jtp%=10;
    fti_jtp<<=4;
    r_data+=fti_jtp;

    fti_jtp=ss/1;
    fti_jtp%=10;
    r_data+=fti_jtp;

    return(r_data);
}

//===============================================================================
//	finction	:Float_to_int_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-23 9:50
//	modefy		:null
//===============================================================================
uint32_t Float_to_int_data(float data)		//浮点转为整形
{
    volatile uint32_t k,i,r_data;

    i=data*10000;
    r_data=0;

    k=i/100000;
    k=k%10;
    k=k<<20;
    r_data+=k;

    k=i/10000;
    k=k%10;
    k=k<<16;
    r_data+=k;

    k=i/1000;
    k=k%10;
    k=k<<12;
    r_data+=k;

    k=i/100;
    k=k%10;
    k=k<<8;
    r_data+=k;

    k=i/10;
    k=k%10;
    k=k<<4;
    r_data+=k;

    k=i;
    k=k%10;
    r_data+=k;

    return(r_data);
}

//===============================================================================
//	finction	:Int_to_float_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-23 9:50
//	modefy		:null
//===============================================================================
float Int_to_float_data(uint32_t data)		//浮点转为整形
{
    volatile float k,r_data;
    volatile uint32_t i;

    r_data=0;

    i=data>>24;
    i&=0x0000000f;
    k=i;
    k=k*100;
    r_data+=k;

    i=data>>20;
    i&=0x0000000f;
    k=i;
    k=k*10;
    r_data+=k;

    i=data>>16;
    i&=0x0000000f;
    k=i;
    r_data+=k;

    i=data>>12;
    i&=0x0000000f;
    k=i;
    k=k/10;
    r_data+=k;

    i=data>>8;
    i&=0x0000000f;
    k=i;
    k=k/100;
    r_data+=k;

    i=data>>4;
    i&=0x0000000f;
    k=i;
    k=k/1000;
    r_data+=k;

    i=data;
    i&=0x0000000f;
    k=i;
    k=k/10000;
    r_data+=k;

    return(r_data);
}

//===============================================================================
//	finction	:nd_to_float_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-23 9:50
//	modefy		:null
//===============================================================================
float nd_to_float_data(uint32_t data)		//浮点转为整形
{
    volatile float k,r_data;
    volatile uint32_t i;

    r_data=0;

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
    k=k/10.0;
    r_data+=k;

    i=data;
    i&=0x0000000f;
    k=i;
    k=k/100.0;
    r_data+=k;

    switch(M_menur1_unit_select_temp)
    {
    case 1:
        r_data*=1000.0;
        break;
    case 3:
    case 5:
        r_data/=1000.0;
        break;
    }

    return(r_data);
}

//===============================================================================
//	finction	:nd2_to_float_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-23 9:50
//	modefy		:null
//===============================================================================
float nd2_to_float_data(uint32_t data)		//浮点转为整形
{
    volatile float k,r_data;
    volatile uint32_t i;

    r_data=0;

    i=data>>16;
    i&=0x0000000f;
    k=i;
    k=k*1000;
    r_data+=k;

    i=data>>12;
    i&=0x0000000f;
    k=i;
    k=k*100;
    r_data+=k;

    i=data>>8;
    i&=0x0000000f;
    k=i;
    k=k*10;
    r_data+=k;

    i=data>>4;
    i&=0x0000000f;
    k=i;
    k=k*1.0;
    r_data+=k;

    i=data;
    i&=0x0000000f;
    k=i;
    k=k/10.0;
    r_data+=k;

    switch(M_menur1_unit_select_temp)
    {
    case 1:
        r_data*=1000.0;
        break;
    case 3:
    case 5:
        r_data/=1000.0;
        break;
    }

    return(r_data);
}

//===============================================================================
//	finction	:TJ_to_float_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-23 9:50
//	modefy		:null
//===============================================================================
float TJ_to_float_data(uint32_t data)		//浮点转为整形 试剂参数用
{
    volatile float k,r_data;
    volatile uint32_t i;

    r_data=0;

    i=data>>12;
    i&=0x0000000f;
    k=i;
    k=k*10;
    r_data+=k;

    i=data>>8;
    i&=0x0000000f;
    k=i;
    k=k*1.0;
    r_data+=k;

    i=data>>4;
    i&=0x0000000f;
    k=i;
    k=k/10.0;
    r_data+=k;

    i=data;
    i&=0x0000000f;
    k=i;
    k=k/100.0;
    r_data+=k;

    return(r_data);
}

//===============================================================================
//	finction	:SJTJ_to_float_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-23 9:50
//	modefy		:null
//===============================================================================
float SJTJ_to_float_data(uint32_t data)		//浮点转为整形 试剂参数用
{
    volatile float k,r_data;
    uint32_t i;

    r_data=0;

    i=data>>4;
    i&=0x0000000f;
    k=i;
    k=k*1.0;
    r_data+=k;

    i=data;
    i&=0x0000000f;
    k=i;
    k=k/10.0;
    r_data+=k;

    return(r_data);
}

//===============================================================================
//	finction	:Correcting_fintion
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-23 9:50
//	modefy		:null
//===============================================================================
float Correcting_fintion(float x_temp)		//校正方程
{
    volatile float a,b,c,d,x,y;

    a=Int_to_float_data(M_menur1_Fa_temp);
    b=Int_to_float_data(M_menur1_Fb_temp);
    c=Int_to_float_data(M_menur1_Fc_temp);
    d=Int_to_float_data(M_menur1_Fd_temp);
    if(a_signle_flag!=1)
        a=-a;
    if(b_signle_flag!=1)
        b=-b;
    if(c_signle_flag!=1)
        c=-c;
    if(d_signle_flag!=1)
        d=-d;

    x=x_temp;

    y=a*x*x*x+b*x*x+c*x+d;
//    y=a*x*x+b*x+c;
    M_menur1_FY_temp=Float_to_int_reg(y);		//Float_to_int_data(y);

    return(y);
}

//===============================================================================
//	finction	:JZYZ_calculat
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-23 14:45
//	modefy		:null
//===============================================================================
float JZYZ_calculat(void)		//计算-校正因子
{
    volatile float ts1_2,x1,y1,s1c,e1,e2,t1,t2;

#ifdef 	__WD_BUCHANG__
    t1=E_ADC_temperature1;
    t1=(t1-1024)/10.0;

    t2=E_ADC_temperature2;
    t2=(t2-1024)/10.0;

    x1=1+(t1-25)*0.0029;
    y1=1+(t2-25)*0.0029;

    e1=Measurement_signal_11;
    e2=Measurement_signal_12;

    e1*=x1;
    e2*=y1;
#else
    e1=Measurement_signal_11;
    e2=Measurement_signal_12;
#endif
    ts1_2=e1/10.0-e2/10.0;
    x1=ts1_2/1000.0;

    y1=Correcting_fintion(x1);
    s1c=nd_to_float_data(M_menur1_jz1nd_temp);
    k1_temp=s1c/y1;

    K1_JZYZ=Float_to_int_reg(k1_temp);
    return(k1_temp);
}

//===============================================================================
//	finction	:Fix_temp_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2015-7-3 13:40
//	modefy		:null
//===============================================================================
float Fix_temp_data(float data)		//温度测试时，结果的校正。
{
    volatile float t1;
    volatile float tparam;
    volatile float wd_data_used_temp;
    //volatile float t1,t2,tparam;
    volatile uint16_t isr_d_tp;

    tparam=data;
    t1=E_ADC_temperature1;			//2015-8-12 9:56
    tparam=tparam*(1+(t1-1274)*0.002);		//2015-8-12 9:56

    if(wd_use_flag==1)
    {
        t1=E_ADC_temperature1;
        if((t1<1204)||(t1>=1324))		//小于18度，大于30度。
        {
            wd_20_change_flag=0;
            wd_data_cnt=0;

            isr_d_tp=wd_use_ramdom;
            isr_d_tp%=8;

            wd_data_used_temp=wd_data_used;
            tparam=wd_data_used;
            tparam*=0.02;
            tparam/=10.0;
            switch(isr_d_tp)
            {
            case 0:
                tparam=wd_data_used_temp-tparam;
                break;
            case 1:
                tparam*=2;
                tparam=wd_data_used_temp-tparam;
                break;
            case 2:
                tparam*=3;
                tparam=wd_data_used_temp-tparam;
                break;
            case 3:
                tparam*=4;
                tparam=wd_data_used_temp-tparam;
                break;
            case 4:
                tparam=wd_data_used_temp+tparam;
                break;
            case 5:
                tparam*=2;
                tparam=wd_data_used_temp+tparam;
                break;
            case 6:
                tparam*=3;
                tparam=wd_data_used_temp+tparam;
                break;
            case 7:
            case 8:
            default:
                tparam*=4;
                tparam=wd_data_used_temp+tparam;
                break;
            }
        }
        else //17~23 度
        {
            wd_data_buf0=wd_data_buf1;
            wd_data_buf1=wd_data_buf2;
            wd_data_buf2=tparam;
            wd_data_used=(wd_data_buf0+wd_data_buf1)/2.0;
        }
    }
    else
    {
    }

    return tparam;
}

//===============================================================================
//	finction	:BSFND_calculat
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-23 14:45
//	modefy		:null
//===============================================================================
float BSFND_calculat(void)		//计算-比色法浓度
{
    volatile float ts1_2,x3,k1,y1,t1,t2,e1,e2,kk;
    volatile uint8_t i;

#ifdef 	__WD_BUCHANG__
    t1=E_ADC_temperature1;
    t1=(t1-1024)/10.0;

    t2=E_ADC_temperature2;
    t2=(t2-1024)/10.0;

    k1=1+(t1-25)*0.0029;
    y1=1+(t2-25)*0.0029;

    e1=Measurement_signal_11;
    e2=Measurement_signal_12;

    e1*=k1;
    e2*=y1;
#else
    e1=Measurement_signal_11;
    e2=Measurement_signal_12;
#endif
    ts1_2=e1/10.0-e2/10.0;
    x3=ts1_2/1000.0;

    y1=Correcting_fintion(x3);

    k1=0;
    i=M_menur1_slope_temp>>16;
    i&=0x0f;
    kk=i*100;
    k1=kk;
    i=M_menur1_slope_temp>>12;
    i&=0x0f;
    kk=i*10;
    k1+=kk;
    i=M_menur1_slope_temp>>8;
    i&=0x0f;
    kk=i;
    k1+=kk;
    i=M_menur1_slope_temp>>4;
    i&=0x0f;
    kk=i;
    kk/=10.0;
    k1+=kk;
    i=M_menur1_slope_temp;
    i&=0x0f;
    kk=i;
    kk/=100.0;
    k1+=kk;
    if(M_menur1_slope_flag!=1)  k1=-k1;

//    if(k2_prog_use!=0)
//       k1=k2_prog_use;			//复用 k2_prog_use 做斜率
//    else
//    {
//    	Reget_slope=1;
//    	Read_result_from_flash(2);
//    	k1=k2_prog_use;
//    	Reget_slope=0;
//    }

    c1_temp=y1*k1*1.0;
    ////c1_temp=Fix_temp_data(c1_temp);		//2015-7-3 15:42

    x3=Get_Prj_correlat();		//2012-9-14 15:43 工程系数
    c1_temp=c1_temp*x3;

    if(c1_temp<0)		//2012-9-7 11:25
        c1_temp=0;

    df_temp=1;
    return(c1_temp);
}

//===============================================================================
//	finction	:DJXL_calculat
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-23 16:23
//	modefy		:null
//===============================================================================
float DJXL_calculat(void)		//计算-电极斜率
{
    volatile float x2,t1,t2,param,s1v,s2v,r1v,r2v,s1c,s2c;
    volatile float e1,e2,d1,d2,d3,d4;
////    uint8_t data_buff[4];
////
////    data_buff[0]=40;
////    data_buff[1]=41;
////    data_buff[2]=42;
////    data_buff[3]=43;
////    Display_program_menu(4,data_buff);

    t1=E_ADC_temperature1;
    t1=(t1-1024)/10.0;

    t2=E_ADC_temperature2;
    t2=(t2-1024)/10.0;

    e1=Measurement_signal_21;
    e1=(e1-31250)/100.0;

    e2=Measurement_signal_22;
    e2=(e2-31250)/100.0;

//    a=e1*298;
//    b=e2*298;
//    c=(273+t1);
//    d=(273+t2);
//    x2=a/c-b/d;
    x2=e1-e2;				//2012-9-13 10:55
    P_Tcall_temp=(t1+t2)/2.0;
    P_Tcall_temp+=273;		//2013-1-31 15:16

    Write_system_reg_to_chip();

    s1v=TJ_to_float_data(M_menur1_jz1_v_temp);
    s2v=TJ_to_float_data(M_menur1_jz2_v_temp);
    r1v=SJTJ_to_float_data(M_menur1_sj1_v_temp);
    r2v=SJTJ_to_float_data(M_menur1_sj2_v_temp);
    s1c=nd_to_float_data(M_menur1_jz1nd_temp);
    s2c=nd2_to_float_data(M_menur1_jz2nd_temp);

    d1=s1v*s1c;
    d2=(s1v+s2v+r1v+r2v);
    d3=(s1v+r1v+r2v)*1.0;
    d4=s2v*s2c;
    param=((d1*d2)/d3)/(d1+d4);

    pub_math_temp=log10(param);
    k2_temp=x2/(pub_math_temp*1.0);

    t2=E_ADC_temperature2;
    t2=(t2-1024)/10.0;
    e2=Measurement_signal_22;
    e2=(e2-31250)/100.0;

    s1v=TJ_to_float_data(M_menur1_jz1_v_temp);
    s2v=TJ_to_float_data(M_menur1_jz2_v_temp);
    r1v=SJTJ_to_float_data(M_menur1_sj1_v_temp);
    r2v=SJTJ_to_float_data(M_menur1_sj2_v_temp);
    s1c=nd_to_float_data(M_menur1_jz1nd_temp);
    s2c=nd2_to_float_data(M_menur1_jz2nd_temp);

    d1=s1v*s1c;
    d2=(s1v+s2v+r1v+r2v);
    d3=(s1v+r1v+r2v)*1.0;
    d4=s2v*s2c;

    param=(d1+d4)/(d2*1.0);
    pub_math_temp=log10(param);
//    j_temp=(e2*298)/((273+t2)*1.0)-(k2_temp*pub_math_temp);
    j_temp=e2-(k2_temp*pub_math_temp);

    //Float_to_int_reg(j_temp);
    return(x2);
}

//===============================================================================
//	finction	:DJFFXND_calculat
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-23 16:54
//	modefy		:null
//===============================================================================
//    float cc4,dd1,dd2,dd3;

float DJFFXND_calculat(void)		//计算-电极法分析浓度
{
    volatile float x2,c4,s2v,r1v,r2v,s2c,sv,d1,d2,d3,t1,t2,Tmea,k1,kk,e1,e2;
    volatile uint8_t i;
////    uint8_t data_buff[7];
////
////    data_buff[0]=40;
////    data_buff[1]=41;
////    data_buff[2]=56;
////    data_buff[3]=49;
////    data_buff[4]=50;
////    data_buff[5]=47;
////    data_buff[6]=48;
////    Display_program_menu(7,data_buff);

    t1=E_ADC_temperature1;
    t1=(t1-1024)/10.0;
    t2=E_ADC_temperature2;
    t2=(t2-1024)/10.0;
    Tmea=(t1+t2)/2.0;
    Tmea+=273;					//2013-1-31 15:22

////    x2=DJXL_calculat();
    e1=Measurement_signal_21;
    e1=(e1-31250)/100.0;

    e2=Measurement_signal_22;
    e2=(e2-31250)/100.0;
    x2=e1-e2;

    sv=TJ_to_float_data(M_menur1_sample_v_temp);
    if(M_dongtai_flag>=1)
        s2v=TJ_to_float_data(p_s2v_temp);
    else
        s2v=TJ_to_float_data(M_menur1_jz2_v_temp);
    r1v=SJTJ_to_float_data(M_menur1_sj1_v_temp);
    r2v=SJTJ_to_float_data(M_menur1_sj2_v_temp);
    s2c=nd2_to_float_data(M_menur1_jz2nd_temp);

    k1=0;
    i=M_menur1_slope_temp>>16;
    i&=0x0f;
    kk=i*100;
    k1=kk;
    i=M_menur1_slope_temp>>12;
    i&=0x0f;
    kk=i*10;
    k1+=kk;
    i=M_menur1_slope_temp>>8;
    i&=0x0f;
    kk=i;
    k1+=kk;
    i=M_menur1_slope_temp>>4;
    i&=0x0f;
    kk=i;
    kk/=10.0;
    k1+=kk;
    i=M_menur1_slope_temp;
    i&=0x0f;
    kk=i;
    kk/=100.0;
    k1+=kk;
    if(M_menur1_slope_flag!=1)  k1=-k1;

    jisuan_k_temp=k1;							//2013-4-25 17:01
    pub_math_temp=pow(10,(-x2)/(k1*Tmea/P_Tcall_temp));
//    pub_math_temp=pow(10,(-x2)/(k2_prog_use*Tmea/P_Tcall_temp));	//old

    d1=(s2v*s2c)/(sv*1.0);
    d2=(sv+s2v+r1v+r2v)*pub_math_temp;
    d3=(sv+r1v+r2v);
    c4=d1/(((d2/d3)-1)*1.0);

    pub_math_temp=(Tmea-223)/(P_Tcall_temp-223);			//2013-1-31 15:22
    c4*=pub_math_temp;							//2013-1-31 15:22

    x2=Get_Prj_correlat();						//2012-9-14 15:43

    if((Memu1_xishi_step_status!=0x00)&&(Memu1_xishi_step_status!=0xff))
        c2_temp=c4*df_temp*x2;
    else
        c2_temp=c4*x2*1.0;

    if(c2_temp<0) c2_temp=0;
    df_temp=1;

/////    tx_jisuan_data(2);							//for test
    return(c2_temp);
}

//===============================================================================
//	finction	:DJFCLJG_calculat
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-24 10:45
//	modefy		:null
//===============================================================================
float DJFCLJG_calculat(void)		//计算-电极法测量结果
{
    volatile float t1,param,sv,r1v,r2v,e1,e2,d1,k1,kk;
    volatile uint8_t i;
////    uint8_t data_buff[7];
////
////    data_buff[0]=40;
////    data_buff[1]=41;
////    data_buff[2]=56;
////    data_buff[3]=20;
////    data_buff[4]=21;
////    data_buff[5]=51;
////    data_buff[6]=52;
////    Display_program_menu(7,data_buff);

    k1=0;
    i=M_menur1_slope_temp>>16;
    i&=0x0f;
    kk=i*100;
    k1=kk;
    i=M_menur1_slope_temp>>12;
    i&=0x0f;
    kk=i*10;
    k1+=kk;
    i=M_menur1_slope_temp>>8;
    i&=0x0f;
    kk=i;
    k1+=kk;
    i=M_menur1_slope_temp>>4;
    i&=0x0f;
    kk=i;
    kk/=10.0;
    k1+=kk;
    i=M_menur1_slope_temp;
    i&=0x0f;
    kk=i;
    kk/=100.0;
    k1+=kk;
    if(M_menur1_slope_flag!=1)  k1=-k1;

    t1=E_ADC_temperature1;
    t1=(t1-1024)/10.0;

    e1=Measurement_signal_21;
    e1=(e1-31250)/100.0;

    e2=Measurement_signal_22;
    e2=(e2-31250)/100.0;

    sv=TJ_to_float_data(M_menur1_sample_v_temp);
    r1v=SJTJ_to_float_data(M_menur1_sj1_v_temp);
    r2v=SJTJ_to_float_data(M_menur1_sj2_v_temp);

    d1=(sv+r1v+r2v);
//    param=((e1*298)/(273+t2)-j_prog_use)/(k2_prog_use*1.0);		//old
//    param=(e1-j_prog_use)/(k2_prog_use*t1/P_Tcall_temp);		//old
    param=(e1-j_prog_use)/(k1*t1/P_Tcall_temp);
    pub_math_temp=pow(10,param);
    pub_math_temp=pub_math_temp*d1/sv;

    c5_temp=pub_math_temp;

    param=Get_Prj_correlat();						//2012-9-14 15:43

    if((Memu1_xishi_step_status!=0x00)&&(Memu1_xishi_step_status!=0xff))
        c3_temp=c5_temp*df_temp*param;
    else
        c3_temp=c5_temp*param*1.0;

    if(c1_temp<0) c1_temp=0;
    df_temp=1;
    return(c3_temp);
}

//===============================================================================
//	finction	:Calculat_all
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-24 10:45
//	modefy		:null
//===============================================================================
void Calculat_all(void)
{
    df_temp=1;
}

//===============================================================================
//	finction	:Set_ZSB_move_to
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Set_ZSB_move_to(uint32_t data)		//注射泵移动到对应位置函数
{
    uint32_t i,j,k;

    DrvSYS_Delay(100000);

    j=0;
    i=data>>8;
    i&=0x0000000f;
    i*=1000;
    j+=i;
    i=data>>4;
    i&=0x0000000f;
    i*=100;
    j+=i;
    i=data;
    i&=0x0000000f;
    i*=10;
    j+=i;

    k=(j*10000)/ZSB_MOTO_STEP_V;

    Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,k,0x00);
}

//===============================================================================
//	finction	:Get_Volume_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
uint32_t Get_Volume_data(uint32_t data)		//转换体积
{
    uint32_t i,j;

    j=0;
    i=data>>12;
    i&=0x0000000f;
    i*=10000;
    j+=i;
    i=data>>8;
    i&=0x0000000f;
    i*=1000;
    j+=i;
    i=data>>4;
    i&=0x0000000f;
    i*=100;
    j+=i;
    i=data;
    i&=0x0000000f;
    i*=10;
    j+=i;

    j=(j*10000);
    return(j);
}

//===============================================================================
//	finction	:Get_SJ_Volume_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
uint32_t Get_SJ_Volume_data(uint32_t data)	//转换试剂体积
{
    uint32_t i,j;

    j=0;
    i=data>>12;
    i&=0x0000000f;
    i*=100000;
    j+=i;
    i=data>>8;
    i&=0x0000000f;
    i*=10000;
    j+=i;
    i=data>>4;
    i&=0x0000000f;
    i*=1000;
    j+=i;
    i=data;
    i&=0x0000000f;
    i*=100;
    j+=i;

    j=(j*10000);
    return(j);
}
//===============================================================================
//	finction	:MgL_to_ugL
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
uint32_t MgL_to_ugL(uint32_t data)
{
    uint32_t i,j;

    j=0;
    i=data>>16;
    i&=0x0000000f;
    i*=1000000;
    j+=i;
    i=data>>12;
    i&=0x0000000f;
    i*=100000;
    j+=i;
    i=data>>8;
    i&=0x0000000f;
    i*=10000;
    j+=i;
    i=data>>4;
    i&=0x0000000f;
    i*=1000;
    j+=i;
    i=data;
    i&=0x0000000f;
    i*=100;
    j+=i;					//unit=ug/L

    return(j);
}

//===============================================================================
//	finction	:Set_ZSB_speed
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Set_ZSB_speed(void)
{
    uint32_t i,j;

    j=0;
    i=M_menur1_ZSB_speed_temp>>4;
    i&=0x0000000f;
    i=i*10;
    j+=i;

    i=M_menur1_ZSB_speed_temp;
    i&=0x0000000f;
    j+=i;

    Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_SPEED_WO_ADDS,j,0x00);	//zhu she bon speed
}

//===============================================================================
//	finction	:Set_JB_speed
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Set_JB_speed(void)
{
    uint32_t i,j;

    j=0;
    i=M_menur1_JB_speed_temp>>8;
    i&=0x0000000f;
    i=i*100;
    j+=i;

    i=M_menur1_JB_speed_temp>>4;
    i&=0x0000000f;
    i=i*10;
    j+=i;

    i=M_menur1_JB_speed_temp;
    i&=0x0000000f;
    j+=i;

    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_MOTOR_SPEED_ADDS,j,0x00);	//zhu she bon speed
}

//===============================================================================
//	finction	:Set_JB_direction
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Set_JB_direction(void)
{
    if(M_menur1_direction_select_temp==1)
        Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_MOTOR_RUN_ADDS,U_SENSER_MOTOR_P,0x00);	//Positive

    else if(M_menur1_direction_select_temp==2)
        Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_MOTOR_RUN_ADDS,U_SENSER_MOTOR_N,0x00);	//Negative

}


//===============================================================================
//	finction	:Set_switch_open_close
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Set_switch_open_close(uint8_t sw,uint8_t status)		//14个阀的开关控制函数
{
    uint16_t temp,jj;

    temp=0;
    jj=1<<sw;
    if(status==1)  temp=1<<sw;
    else           temp=0<<sw;
    Far_status_flag&=~jj;

    Far_status_flag|=temp;

    switch(sw)
    {
    case 1:
        if(status==1)
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_OPEN,0x00);
        else
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
        break;
    case 2:
        if(status==1)
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA2_WO_ADDS,BATTERY_VAL_OPEN,0x00);
        else
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA2_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
        break;
    case 3:
        if(status==1)
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA3_WO_ADDS,BATTERY_VAL_OPEN,0x00);
        else
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA3_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
        break;
    case 4:
        if(status==1)
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA4_WO_ADDS,BATTERY_VAL_OPEN,0x00);
        else
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA4_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
        break;
    case 5:
        if(status==1)
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA5_WO_ADDS,BATTERY_VAL_OPEN,0x00);
        else
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA5_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
        break;
    case 6:
        if(status==1)
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA6_WO_ADDS,BATTERY_VAL_OPEN,0x00);
        else
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA6_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
        break;
    case 7:
        if(status==1)
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA7_WO_ADDS,BATTERY_VAL_OPEN,0x00);
        else
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA7_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
        break;
    case 8:
        if(status==1)
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA8_WO_ADDS,BATTERY_VAL_OPEN,0x00);
        else
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA8_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
        break;
    case 9:
        if(status==1)
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA9_WO_ADDS,BATTERY_VAL_OPEN,0x00);
        else
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA9_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
        break;
    case 10:
        if(status==1)
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA10_WO_ADDS,BATTERY_VAL_OPEN,0x00);
        else
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA10_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
        break;
    case 11:
        if(status==1)
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA11_WO_ADDS,BATTERY_VAL_OPEN,0x00);
        else
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA11_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
        break;
    case 12:
        if(status==1)
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA12_WO_ADDS,BATTERY_VAL_OPEN,0x00);
        else
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA12_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
        break;
    case 13:
        if(status==1)
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA13_WO_ADDS,BATTERY_VAL_OPEN,0x00);
        else
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA13_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
        break;
    case 14:
        if(status==1)
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA14_WO_ADDS,BATTERY_VAL_OPEN,0x00);
        else
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA14_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
        break;
    }
}

//===============================================================================
//	finction	:Program_wait_time
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Program_wait_time(void)			//中断计时调用函数--分析读电压值用
{
    if(Pro_wait_time_flag==1)
    {
        if(Pro_wait_time_set==0)
            Pro_wait_time_flag=2;
        else
            --Pro_wait_time_set;
    }
}

//===============================================================================
//	finction	:Program_wait_time_nc
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-15 23:05
//	modefy		:null
//===============================================================================
void Program_wait_time_nc(void)			//中断计时调用函数--分析读电压值用
{
    uint8_t i;

    i=Pro_wait_time_flag;
    i&=0x01;
    if(i==1)
    {
        if(Pro_wait_time0_set==0)
            Pro_wait_time0_flag=0;
        else
            --Pro_wait_time0_set;
    }
}

//===============================================================================
//	finction	:Power_on_check_device
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-8-30 16:00
//	modefy		:null
//===============================================================================
void Power_on_check_device(void)			//开机检查驱动板和传感器板函数
{
    uint8_t i,data_buff[14];

    Pro_wait_time_set=400;	//wait sec
    Pro_wait_time_flag=1;

    while(Pro_wait_time_flag)
    {
        if(Pro_wait_time_flag==2)
            Pro_wait_time_flag=0;
        __nop();
    }

    Rx_TX_flag_local=1;
    Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x03,U_DRIVER_READ_APP_ADDS,0x0001,0x00);    //检查驱动板

    U_data_buff[4]=U_data_buff[3]=0;

    for(i=0; i<250; i++)
    {
        DrvSYS_Delay(6000);
        if(Rx_TX_flag_local==0)
            i=252;
    }
    Rx_TX_flag_local=2;
    Uart_tx_rx_fifo_int_FLAG&=~BIT3;

    Com1_rd_ctrl(0);
    DrvGPIO_SetBit_new(E_GPB,6);

    U_Rx_wait_time_flag_temp=0x8000;		//set wait 10sec fot time out
    DrvSYS_Delay(2000000);
    for(i=0; i<250; i++)
    {
        DrvSYS_Delay(6000);
        if(Rx_TX_flag_local==0)
            i=252;
    }

    Uart_tx_rx_fifo_int_FLAG&=~BIT2;

    buff_app[0]=U_data_buff[3];
    buff_app[1]=U_data_buff[4];
    if((i==250)&&(buff_app[0]==0))			//error
    {
        Get_write_struct_data(3,3,0,0,0,0);		//记录错误信息
        Write_error_status();

        data_buff[7]=Get_ascii_data('D');
        data_buff[8]=Get_ascii_data('E');
        data_buff[9]=Get_ascii_data('V');
        data_buff[10]=Get_ascii_data('I');
        data_buff[11]=Get_ascii_data('C');
        data_buff[12]=Get_ascii_data('E');

        goto check_error;
    }
//-------------------------------------------
    U_Rx_wait_time_flag_temp=0x0000;

    Pro_wait_time_set=50;	//wait sec
    Pro_wait_time_flag=1;

    while(Pro_wait_time_flag)
    {
        if(Pro_wait_time_flag==2)
            Pro_wait_time_flag=0;
        __nop();
    }

    Rx_TX_flag_local=1;
    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_READ_APP_ADDS,0x0001,0x00);    //检查传感器板

    U_data_buff[4]=U_data_buff[3]=0;

    for(i=0; i<250; i++)
    {
        DrvSYS_Delay(6000);
        if(Rx_TX_flag_local==0)
            i=252;
    }
    Rx_TX_flag_local=2;
    Uart_tx_rx_fifo_int_FLAG&=~BIT3;

    Com1_rd_ctrl(0);
    DrvGPIO_SetBit_new(E_GPB,6);

    U_Rx_wait_time_flag_temp=0x8000;		//set wait 10sec fot time out
    DrvSYS_Delay(2000000);
    for(i=0; i<250; i++)
    {
        DrvSYS_Delay(6000);
        if(Rx_TX_flag_local==0)
            i=252;
    }

    Uart_tx_rx_fifo_int_FLAG&=~BIT2;

    buff_app[2]=U_data_buff[3];
    buff_app[3]=U_data_buff[4];
    if((i==250)&&(buff_app[3]==0))			//error
    {
        Get_write_struct_data(3,4,0,0,0,0);		//记录错误信息
        Write_error_status();

        data_buff[7]=Get_ascii_data('S');
        data_buff[8]=Get_ascii_data('E');
        data_buff[9]=Get_ascii_data('N');
        data_buff[10]=Get_ascii_data('S');
        data_buff[11]=Get_ascii_data('O');
        data_buff[12]=Get_ascii_data('R');
check_error:
        Set_en_buzz_finc(4,4,200,100);
//	Led_port_status|=BIT1;
        Led_data_flag|=BIT0;
//#ifndef __NEWPCB__
//        Led_disp_data(Led_port_status);
//#else
//        DrvGPIO_ClrBit (E_GPC,15);
//#endif

        data_buff[0]=Get_ascii_data('E');
        data_buff[1]=Get_ascii_data('R');
        data_buff[2]=Get_ascii_data('R');
        data_buff[3]=12;
        data_buff[4]=Get_ascii_data('N');
        data_buff[5]=Get_ascii_data('O');
        data_buff[6]=0;
        LCD_disp_flag=1;
////	Clear_screen();
        Clear_n_page(0,7);
        for(i=0; i<13; i++)
            Display_8x16_char_single(3,i*10,data_buff[i]);

        buff_bool[0]=buff_bool[1]=buff_bool[2]=buff_bool[3]=0;
        U_Rx_wait_time_flag_temp=0x0000;
        goto check_exit;
    }
//---------------------------------------------------
    Pro_wait_time_set=50;	//wait sec
    Pro_wait_time_flag=1;

    while(Pro_wait_time_flag)
    {
        if(Pro_wait_time_flag==2)
            Pro_wait_time_flag=0;
        __nop();
    }

    Rx_TX_flag_local=1;
    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_DRIVER_READ_BOOT_ADDS,0x0001,0x00);   //检查传感器板引导
    for(i=0; i<250; i++)
    {
        DrvSYS_Delay(6000);
        if(Rx_TX_flag_local==0)
            i=252;
    }
    Rx_TX_flag_local=2;
    Uart_tx_rx_fifo_int_FLAG&=~BIT3;

    Com1_rd_ctrl(0);
    DrvGPIO_SetBit_new(E_GPB,6);

    U_Rx_wait_time_flag_temp=0x8000;		//set wait 10sec fot time out
    DrvSYS_Delay(2000000);
    for(i=0; i<250; i++)
    {
        DrvSYS_Delay(6000);
        if(Rx_TX_flag_local==0)
            i=252;
    }

    Uart_tx_rx_fifo_int_FLAG&=~BIT2;

    buff_bool[0]=U_data_buff[3];
    buff_bool[1]=U_data_buff[4];
//---------------------------------------------------
    Pro_wait_time_set=50;	//wait sec
    Pro_wait_time_flag=1;

    while(Pro_wait_time_flag)
    {
        if(Pro_wait_time_flag==2)
            Pro_wait_time_flag=0;
        __nop();
    }

    Rx_TX_flag_local=1;
    Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x03,U_SENSER_READ_BOOT_ADDS,0x0001,0x00);   //检查驱动板引导
    for(i=0; i<250; i++)
    {
        DrvSYS_Delay(6000);
        if(Rx_TX_flag_local==0)
            i=252;
    }
    Rx_TX_flag_local=2;
    Uart_tx_rx_fifo_int_FLAG&=~BIT3;

    Com1_rd_ctrl(0);
    DrvGPIO_SetBit_new(E_GPB,6);

    U_Rx_wait_time_flag_temp=0x8000;		//set wait 10sec fot time out
    DrvSYS_Delay(2000000);
    for(i=0; i<250; i++)
    {
        DrvSYS_Delay(6000);
        if(Rx_TX_flag_local==0)
            i=252;
    }

    Uart_tx_rx_fifo_int_FLAG&=~BIT2;

    buff_bool[2]=U_data_buff[3];
    buff_bool[3]=U_data_buff[4];
check_exit:
    DrvSYS_Delay(2000000);
#ifdef  __OLD_NTC__
    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_NTC_CTRL_ADDS,0x0001,0x00); 		//读温度补偿值
#else
    if(M_menur1_LZDJ_select_temp==1)
        Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_NTC_CTRL_WATER_ADDS,0x0001,0x00);		//电极法  读温度补偿值
    else
        Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_NTC_CTRL_ADDS,0x0001,0x00); 		//比色法  读温度补偿值
#endif
    Pro_wait_time_set=50;	//wait sec
    Pro_wait_time_flag=1;

    while(Pro_wait_time_flag)
    {
        if(Pro_wait_time_flag==2)
            Pro_wait_time_flag=0;
        __nop();
    }
    M_lever4_BCWD_temp=U_data_buff[3];
    M_lever4_BCWD_temp<<=8;
    M_lever4_BCWD_temp|=U_data_buff[4];
    if(M_lever4_BCWD_temp>1024)
    {
        M_lever4_BCWD_temp-=1024;
        if(M_lever4_BCWD_temp==100) M_lever4_BCWD_temp=0x0099;
        else
        {
            data_buff[0]=data_buff[1]=M_lever4_BCWD_temp;
            data_buff[0]/=10;
            data_buff[1]%=10;
            M_lever4_BCWD_temp=0;
            M_lever4_BCWD_temp=data_buff[0];
            M_lever4_BCWD_temp<<=4;
            M_lever4_BCWD_temp|=data_buff[0];
        }
        BCWD_NG_flag=1;
    }
    else
    {
        M_lever4_BCWD_temp=1024-M_lever4_BCWD_temp;
        if(M_lever4_BCWD_temp==100) M_lever4_BCWD_temp=0x0099;
        else
        {
            data_buff[0]=data_buff[1]=M_lever4_BCWD_temp;
            data_buff[0]/=10;
            data_buff[1]%=10;
            M_lever4_BCWD_temp=0;
            M_lever4_BCWD_temp=data_buff[0];
            M_lever4_BCWD_temp<<=4;
            M_lever4_BCWD_temp|=data_buff[0];
        }
        BCWD_NG_flag=2;
    }

    Rx_TX_flag=0;
}

//===============================================================================
//	finction	:Program_power_on_initial
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Program_power_on_initial(void)			//开机初始化硬件函数
{
//    uint16_t i;

    if((Pro_power_on_flag==1)&&(Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
    {
        Pro_wait_time_set=60;	//wait 500ms
        switch(Pro_step_status)
        {
        case 0:
            Far_status_flag=0;
//                    Reset_init_UART1(0);
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA_ALL_WO_ADDS,0x0000,0x00);		//close all v
            ++Pro_step_status;
            break;
        case 1:
            Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_MOTOR_RUN_ADDS,U_SENSER_MOTOR_STOP,0x00);		//jiaoban moto off stop
            ++Pro_step_status;
            break;
        case 2:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x03,U_DRIVER_PORT_ALL_IN_ARO_DDS,0x0001,0x00);		//read all in
            ++Pro_step_status;
            break;
        case 3:
            Set_ZSB_speed();
            ++Pro_step_status;
            break;
        case 4:
            Set_JB_speed();
            ++Pro_step_status;
            break;
        case 5:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_RST_REG_CTRL_WO_ADDS,ZHUSEBUN_RESET,0x00);	//reset zhu she bon moto
            Pro_wait_time_set=500;	//wait 5sec
            ++Pro_step_status;
            break;
        case 6:
//    	            U2_disple_flag=0;
//
//	            U2_re_table[0]=Machine_note_use;
//	            U2_re_table[1]=0x65;			//结束消解
//	            U2_re_table[2]=0x00;
//	            U2_re_table[3]=0x00;
//	            U2_re_table[4]=0x00;
//	            U2_re_table[5]=0x00;
//
//	            i=CRC16(U2_re_table,6);
//	            U2_re_table[6]=i;
//	            U2_re_table[7]=i>>8;
//
//	            U2_TX_byte_lenth=0x08;
//	            Uart2_start_tx();
            ++Pro_step_status;
            break;
        case 7:
            Pro_power_on_flag=0;
            Pro_step_status=0;
            Hand_finction_step_select=0;
            break;
        }
        Pro_wait_time_flag=1;
    }
}

//===============================================================================
//	finction	:Program_reset_initial
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-25 9:18
//	modefy		:null
//===============================================================================
void Program_reset_initial(void)			//开机初始化硬件参数函数
{
    if(Program_reset_initial_flag==1)
    {
        ++Program_reset_initial_cnt;
        if(Program_reset_initial_cnt>=0x00400000) Rx_TX_flag=0;

        if(Rx_TX_flag==0)	/* Wait Tx empty */
        {
            Program_reset_initial_flag=0;
////        Hand_finction_step_select=0xff;
            Hand_finction_step_select=0;
            E_step_cnt=0;
            df_temp=1;				//2012-7-24 10:55

            Led_port_status&=~BIT3;
            Led_port_status&=~BIT4;
            Led_data_flag&=~0x0c;
#ifndef   __NEWPCB__
            Led_disp_data(Led_port_status);
#else
            DrvGPIO_ClrBit (E_GPB,15);
            DrvGPIO_SetBit (E_GPB,8);
#endif

            Pro_step_status=0;
            Prog_disp_flag=0;
            U2_disple_flag=0;			//2014-1-26 17:08

            if(Pro_power_on_flag==0)
                Pro_power_on_flag=1;

//////          M_lever4_start_munth_temp=0;	//2013-2-7 17:18
//////          M_lever4_start_day_temp=0;
//////          M_lever4_start_hour_temp=0;
//////          M_lever4_start_mini_temp=0;
            Auto_check_clock_flag=0;

            Pro_wait_time_set=0;
            Pro_wait_time_flag=2;
            Pro_auto_time_dcnt=0;
            Pro_auto_time_dcnt_bak=0;
            Pro_auto_time_up_check_flag=0;
            Pro_dis_menu_flag=0;
//          M_lever4_FXCS_temp=0;
//          Rx_TX_flag=0;
//          k1_prog_use=0;
//          k2_prog_use=0;
            c1_prog_use=0;
            c2_prog_use=0;
            c3_prog_use=0;

            Memu1_auto_step_status=0xff;
            Memu1_hand_step_status=0xff;
            Memu1_jiaozheng_step_status=0xff;
            Memu1_biaoye_step_status=0xff;
            Memu1_xishi_step_status=0xff;

            if(Menu_lever3_start_flag==BIT4)
                Menu_lever3_start_flag=BIT0;
            if(Menu_lever3_start_flag==BIT5)
                Menu_lever3_start_flag=BIT1;
            if(Menu_lever3_start_flag==BIT6)
                Menu_lever3_start_flag=BIT2;
            if(Menu_lever3_start_flag==BIT7)
                Menu_lever3_start_flag=BIT3;

            Write_system_reg_to_chip();			//2012-10-17 14:41

            Read_first_reg_data();		//reset system reg. 72012-10-12 14:02
            Hand_finction_select=0;
        }
    }
}


//===============================================================================
//	finction	:Triggle_get_sample
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Triggle_get_sample(void)		//触发取样函数
{
    uint32_t i,j;
//    uint8_t data_buff[4];
    if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
    {
        Pro_wait_time_set=60;	//wait 500ms
        switch(Pro_step_status)
        {
        case 0:
            All_program_menu_disp(18);

            if(M_menur1_QYSJ_temp!=0)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA11_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            }

            ++Pro_step_status;
            break;
        case 1:
            if(M_menur1_CSSJ_temp!=0)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA12_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            }

            ++Pro_step_status;

            j=0;
            i=M_menur1_CSSJ_temp>>8;
            i&=0x0000000f;
            i=i*100;
            j+=i;

            i=M_menur1_CSSJ_temp>>4;
            i&=0x0000000f;
            i=i*10;
            j+=i;

            i=M_menur1_CSSJ_temp;
            i&=0x0000000f;
            j+=i;

            j=j*100;		//set to ms.

            if(j==0)
                j=10;

            Pro_wait_time_set=j;
            break;
        case 2:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA11_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;

            j=0;
            i=M_menur1_QYSJ_temp>>8;
            i&=0x0000000f;
            i=i*100;
            j+=i;

            i=M_menur1_QYSJ_temp>>4;
            i&=0x0000000f;
            i=i*10;
            j+=i;

            i=M_menur1_QYSJ_temp;
            i&=0x0000000f;
            j+=i;

            j=j*100;		//set to ms.

            if(j==0)
                j=10;

            Pro_wait_time_set=j;
            break;
        case 3:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA12_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 4:
            Pro_step_status=0xff;
            break;
        }
        Pro_wait_time_flag=1;
    }
}

//===============================================================================
//	finction	:Ye_wei_check
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Ye_wei_check(void)			//wait later for get finction then add it.
{
//    uint32_t i,j;
    uint8_t data_buff[4];

    switch(Pro_step_status)
    {
    case 0:
        data_buff[0]=42;
        data_buff[1]=49;
        data_buff[2]=63;
        data_buff[3]=64;
        Display_program_menu(4,data_buff);
        ++Pro_step_status;
        break;
    }
}

//===============================================================================
//	finction	:ZSB_initial
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void ZSB_initial(void)			//注射泵初始化函数
{
//    uint32_t j;
    if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
    {
        Pro_wait_time_set=60;	//wait 500ms
        switch(Pro_step_status)
        {
        case 0:
            All_program_menu_disp(1);

            //Set_JB_direction(); 		//moto on 		2015-7-20 10:28
            Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_MOTOR_RUN_ADDS,U_SENSER_MOTOR_P,0x00);
            ++Pro_step_status;
            break;
        case 1:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA8_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            ++Pro_step_status;
            Pro_wait_time_set=500;	//wait 5sec
            break;
        case 2:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 3:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA2_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 4:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA3_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 5:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA4_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 6:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA5_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 7:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA6_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 8:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA7_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 9:
            Set_ZSB_speed();
            ++Pro_step_status;
            break;
        case 10:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_RST_REG_CTRL_WO_ADDS,ZHUSEBUN_RESET,0x00);	//reset zhu she bon
            ++Pro_step_status;
            Pro_wait_time_set=500;	//wait 5sec
            break;
        case 11:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA8_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;

        case 12:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA8_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            ++Pro_step_status;
            Pro_wait_time_set=500; //wait 5sec
            break;

        case 13:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA8_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;

        case 14:
            Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_MOTOR_RUN_ADDS,U_SENSER_MOTOR_STOP,0x00);
            ++Pro_step_status;
            break;

        case 15:
            Pro_step_status=0xff;
            break;
        }
        Pro_wait_time_flag=1;
    }
}

//===============================================================================
//	finction	:BY1_wash
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void BY1_wash(void)			//标液1清洗函数
{
    uint32_t  s1v,r1v,r2v,result_temp;

    if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
    {
        Pro_wait_time_set=60;	//wait 500ms
        switch(Pro_step_status)
        {
        case 0:
            All_program_menu_disp(2);
            break;
        case 1:
        case 6:
        case 12:
        case 16:
            s1v=Get_Volume_data(M_menur1_jz1_v_temp);
            r1v=Get_Volume_data(M_menur1_sj1_v_temp);
            r2v=Get_Volume_data(M_menur1_sj2_v_temp);
            result_temp=((r1v+r2v+s1v)/2)/ZSB_MOTO_STEP_V;

            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,result_temp,0x00);
            ++Pro_step_status;
            goto ex_BY1_wash;
        case 4:
            Set_JB_direction(); 		//moto on
            ++Pro_step_status;
            goto ex_BY1_wash;
        case 22:
            Pro_step_status=0xff;
            goto ex_BY1_wash;
        }
        if(Pro_step_status!=0xff)
        {
            DrvSYS_Delay(10000);
            Pub_finc_index(TB_BY1_wash_index,Pro_step_status);
            ++Pro_step_status;
        }
ex_BY1_wash:
        Pro_wait_time_flag=1;
    }
}


//===============================================================================
//	finction	:BY1_addtion
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void BY1_addtion(void)			//加标液1函数
{
    uint32_t i,s1v;

    if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
    {
        Pro_wait_time_set=60;	//wait 500ms
        switch(Pro_step_status)
        {
        case 0:
            All_program_menu_disp(3);
            break;
        case 1:
            Pro_wait_time_set=100;	        //wait 1sec
            Pro_wait_time_flag=1;
        case 8:
            DrvSYS_Delay(100000);
            s1v=Get_Volume_data(M_menur1_jz1_v_temp);
            i=s1v/2/ZSB_MOTO_STEP_V;

            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,i,0x00);
            ++Pro_step_status;

            Pro_wait_time_set=100;	//wait 1sec
            Pro_wait_time_flag=1;
            goto ex_BY1_addtion;
        case 5:
            Set_JB_speed();
            ++Pro_step_status;
            goto ex_BY1_addtion;
        case 6:
            Set_JB_direction(); 		//moto on
            ++Pro_step_status;
            goto ex_BY1_addtion;
        case 11:
            Pro_step_status=0xff;
            goto ex_BY1_addtion;
        }
        if(Pro_step_status!=0xff)
        {
            DrvSYS_Delay(10000);
            Pub_finc_index(TB_BY1_addtion_index,Pro_step_status);
            ++Pro_step_status;
        }
ex_BY1_addtion:
        Pro_wait_time_flag=1;
    }
}

//===============================================================================
//	finction	:SJ1_addtion
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void SJ1_addtion(void)			//加试剂1函数
{
    uint32_t i,j,r1v;

    if((Rx_TX_flag==0)&&(Pro_wait_time_flag==2))
    {
        Pro_wait_time_set=200;	//wait 500ms
        switch(Pro_step_status)
        {
        case 0:				//open V1
            All_program_menu_disp(5);

            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            ++Pro_step_status;
            break;
        case 1:
            ++Pro_step_status;
            break;
        case 2:				//move to 0.2mL
            Set_ZSB_move_to(0x00000020);
            ++Pro_step_status;
            break;
        case 3:				//close V1
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 4:				//move to 2.2mL
            Set_ZSB_move_to(0x00000220);
            ++Pro_step_status;
            break;
        case 5:				//open V4
            i=M_menur1_sj2_v_temp;
            i&=0x000000ff;
            if(i==0)
                //if(M_menur1_sj2_v_temp==0)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA4_WO_ADDS,BATTERY_VAL_OPEN,0x00);
                ++Pro_step_status;
            }
            else if(M_menur1_sj3_v_temp==0)
            {
                Set_ZSB_move_to(0x00000230);
                Pro_step_status=10;
            }
            else if(M_menur1_sj4_v_temp==0)
            {
                Set_ZSB_move_to(0x00000260);
                Pro_step_status=12;
            }
            else
            {
                Set_ZSB_move_to(0x00000290);
                Pro_step_status=14;
            }
            break;
        case 6:				//move to 2.2mL+R1V
            j=Get_Volume_data(0x00000220);
            r1v=Get_SJ_Volume_data(M_menur1_sj1_v_temp);
            r1v+=j;
            i=r1v/ZSB_MOTO_STEP_V;

            if(i>11500)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,i-6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
                Pro_step_status=16;  									//分2次加
            }
            else
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,i,0x00);   			//ZSB_move_to    2012-7-24 13:32
                ++Pro_step_status;
                Pro_wait_time_set=200;	//wait 2sec
            }
            break;
        case 7:				//close V4
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA4_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 8:				//move to 0
            Set_ZSB_move_to(0x00000000);
            ++Pro_step_status;
            break;
        case 9:
            Pro_step_status=0xff;
            break;
        case 10:
        case 12:
        case 14:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA4_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            ++Pro_step_status;
            break;
        case 11:
        case 13:
        case 15:
            if(Pro_step_status==11)
                j=Get_Volume_data(0x00000230);
            else if(Pro_step_status==13)
                j=Get_Volume_data(0x00000260);
            else
                j=Get_Volume_data(0x00000290);

            r1v=Get_SJ_Volume_data(M_menur1_sj1_v_temp);
            r1v+=j;
            i=r1v/ZSB_MOTO_STEP_V;

            if(i>11500)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,i-6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
                Pro_step_status=16;  									//分2次加
            }
            else
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,i,0x00);   			//ZSB_move_to    2012-7-24 13:32
                Pro_step_status=7;
            }
            break;
        case 16:				//close V4
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA4_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 17:				//move to 0
            Set_ZSB_move_to(0x00000000);
            ++Pro_step_status;
            break;
        case 18:				//open V4
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA4_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            ++Pro_step_status;
            break;
        case 19:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
            Pro_step_status=7;

            Pro_wait_time_set=200;	//wait 2sec
            break;
        }
        Pro_wait_time_flag=1;
    }
}

//===============================================================================
//	finction	:SJ2_addtion
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void SJ2_addtion(void)			//加试剂2函数
{
    uint32_t i,j,r2v;

    if((Rx_TX_flag==0)&&(Pro_wait_time_flag==2))
    {
        Pro_wait_time_set=200;	//wait 500ms
        switch(Pro_step_status)
        {
        case 0:				//open V1
            All_program_menu_disp(6);

            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            ++Pro_step_status;
            break;
        case 1:
            ++Pro_step_status;
            break;
        case 2:				//move to 0.2mL
            Set_ZSB_move_to(0x00000020);
            ++Pro_step_status;
            break;
        case 3:				//close V1
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 4:				//move to 2.0mL
            Set_ZSB_move_to(0x00000200);
            ++Pro_step_status;
            break;
        case 5:				//open V5
            if(M_menur1_sj3_v_temp==0)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA5_WO_ADDS,BATTERY_VAL_OPEN,0x00);
                ++Pro_step_status;
            }
            else if(M_menur1_sj4_v_temp==0)
            {
                Set_ZSB_move_to(0x00000230);		//move to 2.3mL
                Pro_step_status=10;
            }
            else
            {
                Set_ZSB_move_to(0x00000260);		//move to 2.6mL
                Pro_step_status=12;
            }
            break;
        case 6:				//move to 2.0mL+R2V
            j=Get_Volume_data(0x00000200);
            r2v=Get_SJ_Volume_data(M_menur1_sj2_v_temp);
            r2v+=j;
            i=r2v/ZSB_MOTO_STEP_V;

            if(i>11500)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,i-6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
                Pro_step_status=14;  									//分2次加
            }
            else
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,i,0x00);   			//ZSB_move_to    2012-7-24 13:32
                ++Pro_step_status;
            }
            break;
        case 7:				//close V5
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA5_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 8:				//move to 0
            Set_ZSB_move_to(0x00000000);
            ++Pro_step_status;
            break;
        case 9:
            Pro_step_status=0xff;
            break;
        case 10:				//open V5
        case 12:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA5_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            ++Pro_step_status;
            break;
        case 11:
        case 13:
            if(Pro_step_status==11)
                j=Get_Volume_data(0x00000230);
            else
                j=Get_Volume_data(0x00000260);
            r2v=Get_SJ_Volume_data(M_menur1_sj2_v_temp);
            r2v+=j;
            i=r2v/ZSB_MOTO_STEP_V;

            if(i>11500)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,i-6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
                Pro_step_status=14;  									//分2次加
            }
            else
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,i,0x00);   			//ZSB_move_to    2012-7-24 13:32
                Pro_step_status=7;
            }
            break;
        case 14:				//close V5
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA5_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 15:				//move to 0
            Set_ZSB_move_to(0x00000000);
            ++Pro_step_status;
            break;
        case 16:				//open V5
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA5_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            ++Pro_step_status;
            break;
        case 17:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
            Pro_step_status=7;

            Pro_wait_time_set=200;	//wait 2sec
            break;
        }
        Pro_wait_time_flag=1;
    }
}

//===============================================================================
//	finction	:SJ3_addtion
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void SJ3_addtion(void)			//加试剂3函数
{
    uint32_t i,j,r3v;

    if((Rx_TX_flag==0)&&(Pro_wait_time_flag==2))
    {
        Pro_wait_time_set=200;	//wait 500ms
        switch(Pro_step_status)
        {
        case 0:				//open V1
            All_program_menu_disp(7);

            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            ++Pro_step_status;
            break;
        case 1:
            ++Pro_step_status;
            break;
        case 2:				//move to 0.2mL
            Set_ZSB_move_to(0x00000020);
            ++Pro_step_status;
            break;
        case 3:				//close V1
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 4:				//move to 2.0mL
            Set_ZSB_move_to(0x00000200);
            ++Pro_step_status;
            break;
        case 5:				//open V6
            if(M_menur1_sj4_v_temp==0)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA6_WO_ADDS,BATTERY_VAL_OPEN,0x00);
                ++Pro_step_status;
            }
            else
            {
                Set_ZSB_move_to(0x00000230);	//move to 2.2mL+R3V
                Pro_step_status=10;
            }
            break;
        case 6:				//move to 2.0mL+R3V
            j=Get_Volume_data(0x00000200);
            r3v=Get_SJ_Volume_data(M_menur1_sj3_v_temp);
            r3v+=j;
            i=r3v/ZSB_MOTO_STEP_V;

            if(i>11500)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,i-6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
                Pro_step_status=12;  									//分2次加
            }
            else
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,i,0x00);   			//ZSB_move_to    2012-7-24 13:32
                ++Pro_step_status;
            }
            break;
        case 7:				//close V6
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA6_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 8:				//move to 0
            Set_ZSB_move_to(0x00000000);
            ++Pro_step_status;
            break;
        case 9:				//move to 0
            Pro_step_status=0xff;
            break;
        case 10:			//open V6
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA6_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            ++Pro_step_status;
            break;
        case 11:				//move to 2.3mL+R3V
            j=Get_Volume_data(0x00000230);
            r3v=Get_SJ_Volume_data(M_menur1_sj3_v_temp);
            r3v+=j;
            i=r3v/ZSB_MOTO_STEP_V;

            if(i>11500)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,i-6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
                ++Pro_step_status;  									//分2次加
            }
            else
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,i,0x00);   			//ZSB_move_to    2012-7-24 13:32
                Pro_step_status=7;
            }
            break;
        case 12:				//close V6
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA6_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 13:				//move to 0
            Set_ZSB_move_to(0x00000000);
            ++Pro_step_status;
            break;
        case 14:				//open V6
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA6_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            ++Pro_step_status;
            break;
        case 15:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
            Pro_step_status=7;
            break;
        }
        Pro_wait_time_flag=1;
    }
}

//===============================================================================
//	finction	:SJ4_addtion
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-5 9:50
//	modefy		:null
//===============================================================================
void SJ4_addtion(void)			//加试剂4函数
{
    uint32_t i,j,r4v;

    if((Rx_TX_flag==0)&&(Pro_wait_time_flag==2))
    {
        Pro_wait_time_set=200;	//wait 500ms
        switch(Pro_step_status)
        {
        case 0:				//open V1
            All_program_menu_disp(8);

            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            ++Pro_step_status;
            break;
        case 1:
            ++Pro_step_status;
            break;
        case 2:				//move to 0.2mL
            Set_ZSB_move_to(0x00000020);
            ++Pro_step_status;
            break;
        case 3:				//close V1
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 4:				//move to 2.0mL
            Set_ZSB_move_to(0x00000200);
            ++Pro_step_status;
            break;
        case 5:				//open V7
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA7_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            ++Pro_step_status;
            break;
        case 6:				//move to 2.0mL+R4V
            j=Get_Volume_data(0x00000200);
            r4v=Get_SJ_Volume_data(M_menur1_sj4_v_temp);
            r4v+=j;
            i=r4v/ZSB_MOTO_STEP_V;

            if(i>11500)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,i-6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
                Pro_step_status=10;
                //分2次加
            }
            else
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,i,0x00);   			//ZSB_move_to    2012-7-24 13:32
                ++Pro_step_status;
            }
            break;
        case 7:				//close V7
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA7_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 8:				//move to 0
            Set_ZSB_move_to(0x00000000);
            ++Pro_step_status;
            break;
        case 9:				//move to 0
            Pro_step_status=0xff;
            break;
        case 10:				//close V7
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA7_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 11:				//move to 0
            Set_ZSB_move_to(0x00000000);
            ++Pro_step_status;
            break;
        case 12:				//open V7
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA7_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            ++Pro_step_status;
            break;
        case 13:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
            Pro_step_status=7;
            break;
        }
        Pro_wait_time_flag=1;
    }
}


//===============================================================================
//	finction	:BY2_addtion
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void BY2_addtion(void)			//加标液2函数
{
    float  s1v,r1v,r2v,s2c,result_temp;
    uint32_t  i,j,s2v_temp;

    s2v_temp=0;
    if(M_dongtai_flag==1)		//active status  S2V data
    {
        DJFCLJG_calculat();		//get c5_temp data 2012-8-2 14:44
        s1v=TJ_to_float_data(M_menur1_jz1_v_temp);
        r1v=SJTJ_to_float_data(M_menur1_sj1_v_temp);
        r2v=SJTJ_to_float_data(M_menur1_sj2_v_temp);
        s2c=nd2_to_float_data(M_menur1_jz2nd_temp);
        result_temp=10*c5_temp*(r1v+r2v+s1v)/(s2c*1.0);

        if(result_temp<1)
            s2v_temp=0x00000030;
        else if(result_temp<=10)
            s2v_temp=0x00000050;
        else if(result_temp>10)
            s2v_temp=0x00000100;

        M_dongtai_flag=2;
        p_s2v_temp=s2v_temp;
    }
    else if(M_dongtai_flag==0)
    {
        p_s2v_temp=s2v_temp=M_menur1_jz2_v_temp;
    }

    if((Rx_TX_flag==0)&&(Pro_wait_time_flag==2))
    {
        Pro_wait_time_set=60;	//wait 500ms
        switch(Pro_step_status)
        {
        case 0:				//open V1
            All_program_menu_disp(4);

            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_OPEN,0x00);
//                  Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA5_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            ++Pro_step_status;
            break;
        case 1:				//move to 0.2mL
            Set_ZSB_move_to(0x00000020);
            ++Pro_step_status;
            break;
        case 2:				//close V1
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 3:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_SPEED_WO_ADDS,10,0x00);	//zhu she bon speed 10
            ++Pro_step_status;
            break;
        case 4:				//move to 2.2mL
            Set_ZSB_move_to(0x00000220);
            ++Pro_step_status;
            break;
        case 5:				//open V5
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA5_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            ++Pro_step_status;
            break;
        case 6:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_SPEED_WO_ADDS,20,0x00);	//zhu she bon speed 20
            ++Pro_step_status;
            break;
        case 7:				//move to 2.2mL+S2V
            j=Get_Volume_data(0x00000220);

            s2v_temp=Get_Volume_data(p_s2v_temp);

            i=(s2v_temp+j)/ZSB_MOTO_STEP_V;

            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,i,0x00);
            ++Pro_step_status;

            Pro_wait_time_set=100;	//wait 1sec
            break;
        case 8:				//close V5
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA5_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;
        case 9:				//move to 0
            Set_ZSB_move_to(0x00000000);
            ++Pro_step_status;
            break;
        case 10:			//move to 0
            Pro_step_status=0xff;
            if(M_dongtai_flag==2)
                M_dongtai_flag=1;
            break;
        }
        Pro_wait_time_flag=1;
    }
}

////===============================================================================
////	finction	:PaiKong_washing
////	input		:null
////	output		:null
////	return		:null
////	edit		:sam 2012-7-20 11:46
////	modefy		:null
////===============================================================================
//void PaiKong_washing(void)			//排空清洗
//{
//   if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
//   {
//       Pro_wait_time_set=200;	//wait 500ms 2015-7-2 11:13
//       switch(Pro_step_status)
//       {
//       	  case 0:
//       	         All_program_menu_disp(13);
//                 if(Disple_flag==1)
//                 {
////                     Reset_init_UART1(1);
////	             DrvSYS_Delay(200000);
//	             U2_disple_flag=0;
//	             Set_tx_cmd_data_finc(0x55,0xAA,0x6500,0x66BB,0x00);
//	             U_Rx_wait_time_flag_temp=0;
//	             Rx_TX_flag=0;
//                     Pro_wait_time_set=150;	//wait 1.5sec
//
//                     Pro_step_status=37;
//                     goto ex_paikong;
//                 }
//       	  break;
//       	  case 35:
//                 if(Disple_flag==1)
//                 {
//    	             Pro_wait_time_set=24000;	//wait 240sec
//    	             ++Pro_step_status;
//    	             goto ex_paikong;
//    	         }
//    	         else Pro_step_status=0xff;
//       	  break;
//       	  case 36:
//       	        Pro_step_status=0xff;
//       	  break;
//       	  case 37:
//    	         if(U2_disple_flag==1)
//    	         {
//    	             U2_disple_flag=0;
//    	             Pro_step_status=0;
////    	             Reset_init_UART1(0);
//    	             break;
//    	         }
//    	         else
//    	         {
//    	             Set_tx_cmd_data_finc(0x55,0xAA,0x6500,0x66BB,0x00);
//                     U_Rx_wait_time_flag_temp=0;
//	             Rx_TX_flag=0;
//	             Pro_wait_time_set=150;	//wait 1.5sec
//    	         }
//       	  goto ex_paikong;
//       }
//       if(Pro_step_status!=0xff)
//       {
//          DrvSYS_Delay(10000);
//          Pub_finc_index(TB_paikong_index,Pro_step_status);
//          ++Pro_step_status;
//       }
//ex_paikong:
//       Pro_wait_time_flag=1;
//   }
//}

//===============================================================================
//	finction	:PaiKong_washing
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-20 11:46
//	modefy		:null
//===============================================================================
/*
void PaiKong_washing(void)			//排空清洗
{
    uint32_t tv,sv,r1v,r2v,r3v,r4v;

    r1v=Get_SJ_Volume_data(M_menur1_sj1_v_temp);
    r2v=Get_SJ_Volume_data(M_menur1_sj2_v_temp);
    r3v=Get_SJ_Volume_data(M_menur1_sj3_v_temp);
    r4v=Get_SJ_Volume_data(M_menur1_sj4_v_temp);
    //sv=Get_Volume_data(M_menur1_sample_v_temp);
    sv=Get_Volume_data(0x00001000);				//2017/4/25 15:42
    tv=((sv+r1v+r2v+r3v+r4v)/2+0.5)/ZSB_MOTO_STEP_V;			//2015.05.19 add

   if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
   {
       Pro_wait_time_set=60;	//wait 500ms
       switch(Pro_step_status)
       {
       	  case 0:
       	         All_program_menu_disp(13);
       	  break;
       	  case 7:
       	  case 11:
       	  case 17:
       	  case 21:
       	  case 27:
       	  case 31:
       	         DrvSYS_Delay(100000);
       	         Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv,0x00);   		//ZSB_move_to
       	  break;
       	  //------------------------------------2016/6/16 10:33
       	  case 35:
       	         Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA10_WO_ADDS,BATTERY_VAL_OPEN,0x00);  	//2016/5/17 15:25
       	         Pro_wait_time_set=200;

                 ++Pro_step_status;
       	  goto ex_paikong;
       	  case 36:
       	         Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA10_WO_ADDS,BATTERY_VAL_CLOSE,0x00);  //2016/5/17 15:25

                 ++Pro_step_status;
       	  goto ex_paikong;
       	  case 37:
                 Pro_step_status=0xff;
       	  goto ex_paikong;
       }
       if(Pro_step_status!=0xff)
       {
          DrvSYS_Delay(10000);
          Pub_finc_index(TB_paikong_index,Pro_step_status);
          ++Pro_step_status;
       }
ex_paikong:
       Pro_wait_time_flag=1;
   }
}
*/
void PaiKong_washing(void)			//排空清洗
{
    uint32_t tv,sv,r1v,r2v,r3v,r4v;

    r1v=Get_SJ_Volume_data(M_menur1_sj1_v_temp);
    r2v=Get_SJ_Volume_data(M_menur1_sj2_v_temp);
    r3v=Get_SJ_Volume_data(M_menur1_sj3_v_temp);
    r4v=Get_SJ_Volume_data(M_menur1_sj4_v_temp);
    sv=Get_Volume_data(0x00001000);
    tv=((sv+r1v+r2v+r3v+r4v)/2+0.5)/ZSB_MOTO_STEP_V;

    if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
    {
        Pro_wait_time_set=60;	//wait 500ms
        switch(Pro_step_status)
        {
        case 0:
            All_program_menu_disp(13);
            Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_MOTOR_RUN_ADDS,U_SENSER_MOTOR_P,0x00);
            Pro_step_status = 60;
            break;

        case 60:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0001,0x00);
            Pro_step_status = 1;
            Pro_wait_time_set = 0x0104;
            Pro_wait_time_flag=1;
            break;

        case 1:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1100,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 2:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1102,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 3:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1103,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 4:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,0x0000,0x00);
            ++Pro_step_status;
            Pro_wait_time_set = 0x0104;
            Pro_wait_time_flag=1;
            break;

        case 5:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 6:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0001,0x00);
            ++Pro_step_status;
            Pro_wait_time_set = 0x03f8;
            Pro_wait_time_flag=1;
            break;

        case 7:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 8:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1100,0x0001,0x00);
            ++Pro_step_status;
            break;

        case 9:
            DrvSYS_Delay(100000);
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv,0x00);   		//ZSB_move_to
            ++Pro_step_status;
            Pro_wait_time_set = 0x0104;
            Pro_wait_time_flag=1;
            break;

        case 10:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1100,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 11:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 12:	//打开阀1---10
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1100,0x0001,0x00);
            ++Pro_step_status;
            break;

        case 13:
            DrvSYS_Delay(100000);
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv,0x00);   		//ZSB_move_to
            ++Pro_step_status;
            Pro_wait_time_set = 0x0104;
            Pro_wait_time_flag=1;
            break;

        case 14:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1100,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 15:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,0x0000,0x00);
            ++Pro_step_status;
            Pro_wait_time_set = 0x0104;
            Pro_wait_time_flag=1;
            break;

        case 16:	//打开阀8，对应排空阀
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0001,0x00);
            ++Pro_step_status;
            Pro_wait_time_set = 0x0104;
            Pro_wait_time_flag=1;
            break;
        case 17:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 18:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0001,0x00);
            ++Pro_step_status;
            Pro_wait_time_set = 0x03f8;
            Pro_wait_time_flag=1;
            break;

        case 19:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 20:	//打开阀1---10
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1100,0x0001,0x00);
            ++Pro_step_status;
            break;

        case 21:
            DrvSYS_Delay(100000);
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv,0x00);   		//ZSB_move_to
            ++Pro_step_status;
            Pro_wait_time_set = 0x0104;
            Pro_wait_time_flag=1;
            break;

        case 22:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1100,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 23:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 24:	//打开阀1---10
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1100,0x0001,0x00);
            ++Pro_step_status;
            break;

        case 25:
            DrvSYS_Delay(100000);
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv,0x00);   		//ZSB_move_to
            ++Pro_step_status;
            Pro_wait_time_set = 0x0104;
            Pro_wait_time_flag=1;
            break;

        case 26:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1100,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 27:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,0x0000,0x00);
            ++Pro_step_status;
            Pro_wait_time_set = 0x0104;
            Pro_wait_time_flag=1;
            break;

        case 28:	//打开阀8，对应排空阀
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0001,0x00);
            ++Pro_step_status;
            Pro_wait_time_set = 0x0104;
            Pro_wait_time_flag=1;
            break;
        case 29:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 30:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0001,0x00);
            ++Pro_step_status;
            Pro_wait_time_set = 0x03f8;
            Pro_wait_time_flag=1;
            break;

        case 31:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 32:	//打开阀1---10
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1100,0x0001,0x00);
            ++Pro_step_status;
            break;

        case 33:
            DrvSYS_Delay(100000);
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv,0x00);   		//ZSB_move_to
            ++Pro_step_status;
            Pro_wait_time_set = 0x0104;
            Pro_wait_time_flag=1;
            break;

        case 34:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1100,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 35:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 36:	//打开阀1---10
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1100,0x0001,0x00);
            ++Pro_step_status;
            break;

        case 37:
            DrvSYS_Delay(100000);
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv,0x00);   		//ZSB_move_to
            ++Pro_step_status;
            Pro_wait_time_set = 0x0104;
            Pro_wait_time_flag=1;
            break;

        case 38:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1100,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 39:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 40:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA10_WO_ADDS,BATTERY_VAL_OPEN,0x00);  	//2016/5/17 15:25
            Pro_wait_time_set=200;
            ++Pro_step_status;
            Pro_wait_time_flag=1;
            break;

        case 41:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA10_WO_ADDS,BATTERY_VAL_CLOSE,0x00);  //2016/5/17 15:25
            ++Pro_step_status;
            break;

        case 42:
            Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_MOTOR_RUN_ADDS,U_SENSER_MOTOR_STOP,0x00);
            Pro_step_status=0xff;
            break;

        default:
            break;
        }
    }
}

//===============================================================================
//	finction	:Sample_washing
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-20 13:46
//	modefy		:null
//===============================================================================
uint8_t SW_atemp;
/*
void Sample_washing(void)		//样品润洗
{
    uint32_t tv,sv,r1v,r2v,r3v,r4v;

    //r1v=Get_SJ_Volume_data(M_menur1_sj1_v_temp);
    //r2v=Get_SJ_Volume_data(M_menur1_sj2_v_temp);
    //r3v=Get_SJ_Volume_data(M_menur1_sj3_v_temp);
    //r4v=Get_SJ_Volume_data(M_menur1_sj4_v_temp);
    //sv=Get_Volume_data(M_menur1_sample_v_temp);
    //tv=(sv+r1v+r2v+r3v+r4v)/2/ZSB_MOTO_STEP_V;	//2015-7-20 13:36

//--------------------------------------\	2017/4/21 10:01
    sv=M_menur1_sample_v_temp;
    sv&=0x0000ffff;
    if(sv < 0x00001000)
    {
    	sv=Get_Volume_data(0x00000400);
    }
    else
    {
    	sv=Get_Volume_data(M_menur1_sample_v_temp);
    }
//-------------------------------------/
    tv=sv/2/ZSB_MOTO_STEP_V;				//2015-7-20 13:36

   if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
   {
       Pro_wait_time_set=60;	//wait 500ms
       switch(Pro_step_status)
       {
       	  case 0:
       	         All_program_menu_disp(14);
       	         SW_atemp=0;
       	  break;
       	  case 6:
       	  case 22:
    	         Set_JB_direction(); 		//moto on
                 ++Pro_step_status;
       	  goto ex_Sample_washing;
          case 2:		//move to SV/2mL
	         DrvSYS_Delay(100000);
             	 if(tv>11500)
              	 {
             	    Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv-6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
             	    Pro_step_status=33;  						//分2次加
             	 }
             	 else
             	 {
             	    Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv,0x00);   		//ZSB_move_to    2012-7-24 13:32
                    ++Pro_step_status;
                 }
          goto ex_Sample_washing;
          case 9:
	         DrvSYS_Delay(100000);
             	 if(tv>11500)
              	 {
             	    Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv-6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
             	    Pro_step_status=33;  						//分2次加
             	    SW_atemp=1;
             	 }
             	 else
             	 {
             	    Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv,0x00);   		//ZSB_move_to    2012-7-24 13:32
                    ++Pro_step_status;
                 }
          goto ex_Sample_washing;
          case 18:
	         DrvSYS_Delay(100000);
             	 if(tv>11500)
              	 {
             	    Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv-6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
             	    Pro_step_status=33;  						//分2次加
             	    SW_atemp=2;
             	 }
             	 else
             	 {
             	    Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv,0x00);   		//ZSB_move_to    2012-7-24 13:32
                    ++Pro_step_status;
                 }
          goto ex_Sample_washing;
          case 25:
	         DrvSYS_Delay(100000);
             	 if(tv>11500)
              	 {
             	    Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv-6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
             	    Pro_step_status=33;  						//分2次加
             	    SW_atemp=3;
             	 }
             	 else
             	 {
             	    Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv,0x00);   		//ZSB_move_to    2012-7-24 13:32
                    ++Pro_step_status;
                 }
          goto ex_Sample_washing;
	  case 38:
             	 Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
                 if(SW_atemp==1)
                    Pro_step_status=10;
                 else if(SW_atemp==2)
                    Pro_step_status=19;
                 else if(SW_atemp==3)
                    Pro_step_status=26;
                 else
                    Pro_step_status=3;
	  goto ex_Sample_washing;
       	  case 32:
                 sv=M_menur1_sample_v_temp;
                 sv&=0x0000ffff;
                 if(sv < 0x00001000)
                 {
                 	Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_OPEN,0x00);
                 	Pro_step_status=40;
                 }
                 else   Pro_step_status=0xff;
       	  goto ex_Sample_washing;
       	  case 40:
                 Set_ZSB_move_to(0x00000800);
                 ++Pro_step_status;
       	  goto ex_Sample_washing;
       	  case 41:
                 Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
                 ++Pro_step_status;
       	  goto ex_Sample_washing;
       	  case 42:
                 Set_ZSB_move_to(0x00000000);
                 ++Pro_step_status;
       	  goto ex_Sample_washing;
       	  case 43:
                 Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_OPEN,0x00);
                 ++Pro_step_status;
       	  goto ex_Sample_washing;
       	  case 44:
                 Set_ZSB_move_to(0x00000800);
                 ++Pro_step_status;
       	  goto ex_Sample_washing;
       	  case 45:
                 Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
                 ++Pro_step_status;
       	  goto ex_Sample_washing;
       	  case 46:
                 Set_ZSB_move_to(0x00000000);
                 ++Pro_step_status;
       	  goto ex_Sample_washing;
       	  case 47:
                 Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA8_WO_ADDS,BATTERY_VAL_OPEN,0x00);
                 ++Pro_step_status;
                 Pro_wait_time_set=800;
       	  goto ex_Sample_washing;
       	  //case 16:		//2017/8/23 9:41
       	  case 48:
                 Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA8_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
                 Pro_step_status=0xff;
       	  goto ex_Sample_washing;
       }
       if(Pro_step_status!=0xff)
       {
          DrvSYS_Delay(10000);
          Pub_finc_index(TB_Sample_washing_index,Pro_step_status);
          ++Pro_step_status;
       }
ex_Sample_washing:
       Pro_wait_time_flag=1;
   }
}
*/
void Sample_washing(void)		//样品润洗
{
    uint32_t tv,sv;//,r1v,r2v,r3v,r4v;

    sv = M_menur1_sample_v_temp;
    sv&=0x0000ffff;
    if(sv < 0x00001000)
    {
        sv=Get_Volume_data(0x00000400);
    }
    else
    {
        sv=Get_Volume_data(M_menur1_sample_v_temp);
    }

    tv=sv/2/ZSB_MOTO_STEP_V;

    if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
    {
        Pro_wait_time_set=60;
        switch(Pro_step_status)
        {
        case 0:
            All_program_menu_disp(14);
            SW_atemp = 0;
            ++Pro_step_status;
            break;

        case 1:	//打开阀2
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1101,0x0001,0x00);
            ++Pro_step_status;
            break;

        case 2: //打开阀3
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1102,0x0001,0x00);
            ++Pro_step_status;
            break;

        case 3: //
            DrvSYS_Delay(100000);
            if(tv > 11500)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv-6000,0x00); 	   	//ZSB_move_to	2012-8-27 13:27
                Pro_step_status = 55;						   				//分2次加
            }
            else
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv,0x00);		   		//ZSB_move_to	2012-7-24 13:32
                ++Pro_step_status;
            }
            break;

        case 4:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1101,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 5:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1102,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 6:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 7:
            Set_JB_direction(); 		//moto on
            ++Pro_step_status;
            break;

        case 8:	//打开阀2
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1101,0x0001,0x00);
            ++Pro_step_status;
            break;

        case 9: //打开阀3
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1102,0x0001,0x00);
            ++Pro_step_status;
            break;

        case 10:
            DrvSYS_Delay(100000);
            if(tv>11500)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv-6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
                Pro_step_status=55;  										//分2次加
                SW_atemp=1;
            }
            else
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv,0x00);   			//ZSB_move_to    2012-7-24 13:32
                ++Pro_step_status;
            }
            break;

        case 11:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1101,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 12:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1102,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 13:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 14:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0001,0x00);
            ++Pro_step_status;
            Pro_wait_time_set = 0x0104;
            Pro_wait_time_flag=1;
            break;

        case 15:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 16:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0001,0x00);
            ++Pro_step_status;
            Pro_wait_time_set = 0x03f8;
            Pro_wait_time_flag=1;
            break;

        case 17:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 18:
            Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,0x1300,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 19:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1101,0x0001,0x00);
            ++Pro_step_status;
            break;

        case 20:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1102,0x0001,0x00);
            ++Pro_step_status;
            break;

        case 21:
            DrvSYS_Delay(100000);
            if(tv>11500)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv-6000,0x00);		  //ZSB_move_to    2012-8-27 13:27
                Pro_step_status=55;						  //分2次加
                SW_atemp=2;
            }
            else
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv,0x00);		  //ZSB_move_to    2012-7-24 13:32
                ++Pro_step_status;
            }
            break;

        case 22:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1101,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 23:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1102,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 24:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 25:
            Set_JB_direction(); 		//moto on
            ++Pro_step_status;
            break;

        case 26:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1101,0x0001,0x00);
            ++Pro_step_status;
            break;

        case 27:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1102,0x0001,0x00);
            ++Pro_step_status;
            break;

        case 28:
            DrvSYS_Delay(100000);
            if(tv>11500)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv-6000,0x00); 	   //ZSB_move_to	2012-8-27 13:27
                Pro_step_status=55;						   //分2次加
                SW_atemp=3;
            }
            else
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,tv,0x00);		   //ZSB_move_to	2012-7-24 13:32
                ++Pro_step_status;
            }

        case 29:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1101,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 30:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1102,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 31:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 32:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0001,0x00);
            ++Pro_step_status;
            Pro_wait_time_set = 0x0104;
            Pro_wait_time_flag=1;
            break;

        case 33:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 34:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0001,0x00);
            ++Pro_step_status;
            Pro_wait_time_set = 0x03f8;
            Pro_wait_time_flag=1;
            break;

        case 35:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1107,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 36:
            Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,0x1300,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 37:
            sv=M_menur1_sample_v_temp;
            sv&=0x0000ffff;
            if(sv < 0x00001000)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_OPEN,0x00);
                Pro_step_status=40;
            }
            else   Pro_step_status=0xff;

        case 40:
            Set_ZSB_move_to(0x00000800);
            ++Pro_step_status;
            break;

        case 41:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;

        case 42:
            Set_ZSB_move_to(0x00000000);
            ++Pro_step_status;
            break;

        case 43:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            ++Pro_step_status;
            break;

        case 44:
            Set_ZSB_move_to(0x00000800);
            ++Pro_step_status;
            break;

        case 45:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;

        case 46:
            Set_ZSB_move_to(0x00000000);
            ++Pro_step_status;
            break;

        case 47:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA8_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            ++Pro_step_status;
            Pro_wait_time_set = 0x0104;
            Pro_wait_time_flag=1;
            break;

        case 48:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA8_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            break;

        case 49:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA8_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            Pro_step_status = 60;
            Pro_wait_time_set = 0x03f8;
            Pro_wait_time_flag=1;
            break;

        case 60:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA8_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            Pro_step_status = 0xff;
            break;

        case 50:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1101,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 51:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1102,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 52:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,0x0000,0x00);
            ++Pro_step_status;
            break;

        case 53: //打开阀2
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1101,0x0001,0x00);
            ++Pro_step_status;
            break;

        case 54: //打开阀3
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1102,0x0001,0x00);
            ++Pro_step_status;
            break;

        case 55:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,6000,0x00);		   //ZSB_move_to	2012-8-27 13:27
            if(SW_atemp==1)
                Pro_step_status=11;
            else if(SW_atemp==2)
                Pro_step_status=22;
            else if(SW_atemp==3)
                Pro_step_status=29;
            else
                Pro_step_status=4;
            break;

        default:
            break;

        }
    }
}

//===============================================================================
//	finction	:Sample_adding
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-20 13:46
//	modefy		:null
//===============================================================================
void Sample_adding(void)		//加样品
{
    uint32_t i,sv;

    if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
    {
        Pro_wait_time_set=200;	//wait 500ms
        switch(Pro_step_status)
        {
        case 0:
            All_program_menu_disp(15);
            break;
        case 2:		//move to SV/2mL
        case 9:
            DrvSYS_Delay(100000);
            sv=Get_Volume_data(M_menur1_sample_v_temp);
            i=sv/2/ZSB_MOTO_STEP_V;

            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_POSITION_WO_ADDS,i,0x00);
            ++Pro_step_status;
            goto ex_Sample_adding;
        case 6:
            Set_JB_direction(); 		//moto on
            ++Pro_step_status;
            goto ex_Sample_adding;
        case 13:				//2017/4/21 10:31
            sv=M_menur1_sample_v_temp;
            sv&=0x0000ffff;
            if(sv < 0x00001000)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_OPEN,0x00);
                ++Pro_step_status;
            }
            else
            {
                Pro_step_status=0xff;
            }
            goto ex_Sample_adding;
        case 14:				//2017/4/21 10:31
            sv=Get_Volume_data(M_menur1_sample_v_temp);
            i=Get_Volume_data(0x00001000);
            i-=sv;

            sv=i/ZSB_MOTO_STEP_V;

            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,sv,0x00);

            Pro_wait_time_set=500;
            ++Pro_step_status;
            goto ex_Sample_adding;
        case 15:				//2017/4/21 10:31
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA1_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            ++Pro_step_status;
            goto ex_Sample_adding;
        case 16:
            Set_ZSB_move_to(0x00000000);
            Pro_step_status=0xff;
            goto ex_Sample_adding;
        }
        if(Pro_step_status!=0xff)
        {
            DrvSYS_Delay(10000);
            Pub_finc_index(TB_Sample_adding_index,Pro_step_status);
            ++Pro_step_status;
        }
ex_Sample_adding:
        Pro_wait_time_flag=1;
    }
}


//===============================================================================
//	finction	:Get_df_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-20 13:46
//	modefy		:null
//===============================================================================
float Get_df_data(float c1_c2)			//转换取得 DF 参数
{
    float df_data;
//    uint32_t i,j;

    df_data=0;
    pr_data=0;

    pi=M_menur1_lange_data_temp;
    pi>>=16;
    pi&=0x0000000f;
    pk=pi;
    pk=pk*1000;
    pr_data+=pk;

    pi=M_menur1_lange_data_temp;
    pi>>=12;
    pi&=0x0000000f;
    pk=pi;
    pk=pk*100;
    pr_data+=pk;

    pi=M_menur1_lange_data_temp;
    pi>>=8;
    pi&=0x0000000f;
    pk=pi;
    pk=pk*10;
    pr_data+=pk;

    pi=M_menur1_lange_data_temp;
    pi>>=4;
    pi&=0x0000000f;
    pk=pi;
    pr_data+=pk;

    pi=M_menur1_lange_data_temp;
    pi&=0x0000000f;
    pk=pi;
    pk=pk/10.0;
    pr_data+=pk;

    if(M_menur1_LZDJ_select_temp==1)		//电极法
    {
        pk=c1_c2/(pr_data*1.0);
        if(pk<=1.2)
            df_data=1;
        else if((pk>1.2)&&(pk<=4))
            df_data=5;
        else if(pk>4)
            df_data=10;

//        else if((pk>4)&&(pk<=6))
//           df_data=10;
//        else if(pk>6)
//           df_data=50;
    }
    else					//比色法
    {
        pk=c1_c2/(pr_data*1.0);
        if(pk<=1.2)
            df_data=1;
        else if((pk>1.2)&&(pk<=1.5))
            df_data=2;
        else if((pk>1.5)&&(pk<=2))
            df_data=5;
        else if((pk>2)&&(pk<=3))
            df_data=10;
        else if(pk>3)
            df_data=20;
    }

    df_temp=df_data;
    return(df_data);
}

//===============================================================================
//	finction	:Add_xishi_sample
//	input		:c1_c2_tp
//	output		:null
//	return		:null
//	edit		:sam 2012-7-20 13:46
//	modefy		:null
//===============================================================================
float s1v_xi,c1_c2_xi;
uint32_t k_xi;

void Add_xishi_sample(float c1_c2_tp)		//加稀释样品函数
{
    float df_tp,i;

    if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
    {
        Pro_wait_time_set=60;	//wait 500ms
        switch(Pro_step_status)
        {
        case 0:
            All_program_menu_disp(16);
            break;
        case 4:
            Set_JB_direction(); 		//moto on
            ++Pro_step_status;
            goto ex_xishi;
        case 33:
            DrvSYS_Delay(100000);
            s1v_xi=TJ_to_float_data(M_menur1_jz1_v_temp);
            df_tp=Get_df_data(c1_c2_xi);
            s1v_xi=s1v_xi/(df_tp*1.0);
            k_xi=(s1v_xi*10000000)/ZSB_MOTO_STEP_V;

            if(k_xi>11500)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,k_xi-6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
                Pro_step_status=46;  						//分2次加
            }
            else
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,k_xi,0x00);   		//ZSB_move_to    2012-7-24 13:32
                ++Pro_step_status;
            }
            goto ex_xishi;
        case 38:
            DrvSYS_Delay(100000);
            s1v_xi=TJ_to_float_data(M_menur1_jz1_v_temp);
            i=s1v_xi;
            df_tp=Get_df_data(c1_c2_xi);
            s1v_xi=i-s1v_xi/(df_tp*1.0);
            k_xi=(s1v_xi*10000000)/ZSB_MOTO_STEP_V;
            if(k_xi>11500)
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,k_xi-6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
                ++Pro_step_status;  						//分2次加
            }
            else
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,k_xi,0x00);   		//ZSB_move_to    2012-7-24 13:32
                Pro_step_status=43;
            }
            goto ex_xishi;
        case 51:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,0x1301,6000,0x00);   		//ZSB_move_to    2012-8-27 13:27
            Pro_step_status=34;
            break;
        case 45:
            Pro_step_status=0xff;
            goto ex_xishi;
        }
        if(Pro_step_status!=0xff)
        {
            DrvSYS_Delay(10000);
            Pub_finc_index(TB_xishi_index,Pro_step_status);
            ++Pro_step_status;
        }
ex_xishi:
        Pro_wait_time_flag=1;
    }
}

//===============================================================================
//	finction	:Disp_real_time_vol_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Disp_real_time_vol_data(uint8_t select,uint8_t finc_sl,uint32_t disp_d)	//显示实时电压值
{
    uint8_t data_buff[16],j;

    data_buff[1]=20;
    data_buff[2]=0;
    data_buff[3]=0;
    data_buff[4]=0;
    data_buff[12]=0;
    data_buff[13]='M';
    data_buff[14]='V';

    if(select==2)
    {
        if(disp_d<31250)
        {
            data_buff[4]=14;		//-
            disp_d=31250-disp_d;
        }
        else
        {
            data_buff[4]=0;		//+
            disp_d=disp_d-31250;
        }
    }
//	else
//	{
//           if(disp_d<25000)
//           {
//               data_buff[2]=4;		//-
//               disp_d=25000-disp_d;
//           }
//           else
//           {
//               data_buff[2]=0;		//+
//               disp_d=disp_d-25000;
//           }
//	}

    if(M_menur1_LZDJ_select_temp==2)
        disp_d=Float_to_int_reg_ee(disp_d);
    else
        disp_d=Float_to_int_reg_ee2(disp_d);

    j=disp_d>>28;
    j&=0x0f;
    if(j>=0x0a)
        j=0;

    if(j==0)
        data_buff[5]=0;
    else
        data_buff[5]=Get_ascii_data(j);
    j=disp_d>>24;
    j&=0x0f;
    if((j==0)&&(data_buff[5]==0))
        data_buff[6]=0;
    else
        data_buff[6]=Get_ascii_data(j);
    j=disp_d>>20;
    j&=0x0f;
    if((j==0)&&(data_buff[5]==0)&&(data_buff[6]==0))
        data_buff[7]=0;
    else
        data_buff[7]=Get_ascii_data(j);
    j=disp_d>>16;
    j&=0x0f;
    data_buff[8]=Get_ascii_data(j);
    data_buff[9]=19;
    j=disp_d>>12;
    j&=0x0f;
    data_buff[10]=Get_ascii_data(j);
    j=disp_d>>8;
    j&=0x0f;
    data_buff[11]=Get_ascii_data(j);
//     	j=disp_d>>4;
//     	j&=0x0f;
//     	data_buff[10]=Get_ascii_data(j);
//     	j=disp_d;
//      	j&=0x0f;
//    	data_buff[11]=Get_ascii_data(j);
    if(Prog_disp_flag==1)
    {
        if(finc_sl==1)
        {
///     	      data_buff[0]='Q';
///              if(M_menur1_language_select_temp==1)
///              {
            data_buff[0]='E';
            data_buff[1]='1';
            data_buff[2]=20;
///              }
            Display_6x8_char(4,6,15,data_buff);	//disp E1
            for(j=0; j<15; j++)
                Get_6x8_char_single(4,6+6*j,data_buff[j]);
        }
        else
        {
///     	      data_buff[0]='H';
///              if(M_menur1_language_select_temp==1)
///              {
            data_buff[0]='E';
            data_buff[1]='2';
            data_buff[2]=20;
///              }
            Display_6x8_char(5,6,15,data_buff);	//disp E2
            for(j=0; j<15; j++)
                Get_6x8_char_single(5,6+6*j,data_buff[j]);
        }
    }

}

//===============================================================================
//	finction	:Test_signal_wait_time
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Test_signal_wait_time(void)		//分析读电压值计时
{
    if(E_wait_flag==1)
    {
        ++E_wait_time_cnt;
        if((E_wait_time_cnt==E_wait_time_set)||(E_wait_time_cnt>0xf41dc))
        {
            E_wait_time_cnt=E_wait_flag=0;
        }
        if(E_wait_time_set>0xf41dc)
        {
            E_wait_time_cnt=0;
            E_wait_time_set=1500;
        }
    }
}

//===============================================================================
//	finction	:Read_cal_temp
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2013-3-29 9:29
//	modefy		:null
//===============================================================================
uint16_t Read_cal_temp(uint16_t cmd)		//取校正温度
{
    uint32_t i;

    Rx_TX_flag_local=1;
    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,cmd,0x0001,0x00);
    for(i=0; i<250; i++)
    {
        DrvSYS_Delay(6000);
        if(Rx_TX_flag_local==0)
            i=250;
    }
    Rx_TX_flag_local=2;
    Uart_tx_rx_fifo_int_FLAG&=~BIT3;
////       DrvSYS_Delay(2000);				//delay 2ms
    Com1_rd_ctrl(0);
    DrvGPIO_SetBit(E_GPB,6);

    U_Rx_wait_time_flag_temp=0x8000;		//set wait 10sec fot time out
    for(i=0; i<250; i++)
    {
        DrvSYS_Delay(6000);
        if(Rx_TX_flag_local==0)
            i=253;
    }
    U_Rx_wait_time_flag_temp=0x0000;
    Uart_tx_rx_fifo_int_FLAG&=~BIT2;
    DrvSYS_Delay(200000);
    DrvSYS_Delay(200000);
    DrvSYS_Delay(200000);
    DrvSYS_Delay(200000);
    DrvSYS_Delay(200000);
    DrvSYS_Delay(200000);
    DrvSYS_Delay(200000);
//       if(i==253)				//2013-6-9 9:13
//       {
    i=U_data_buff[3];
    i=i<<8;
    i+=U_data_buff[4];
//       }
//       else i=1274;				//25度
////       E_ADC_temperature2=i;
    return(i);
}

//===============================================================================
//	finction	:Test_signal_data_nc
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-15 22:43
//	modefy		:null
//===============================================================================
/////uint8_t   waitting_dsp_flag;

void Test_signal_data_nc(uint8_t select,uint8_t finc_sl)
{
/////    uint8_t m_buff[8];
    uint32_t i;

    if(Prog_disp_flag!=2)
    {
/////       if(waitting_dsp_flag==0)
/////       {
/////          waitting_dsp_flag=1;
/////
/////          Clear_apage(2);
/////          Clear_apage(3);
/////          m_buff[0]='W';
/////          m_buff[1]='A';
/////          m_buff[2]='I';
/////          m_buff[3]='T';
/////          m_buff[4]='T';
/////          m_buff[5]='I';
/////          m_buff[6]='N';
/////          m_buff[7]='G';
/////          Display_6x8_char(2,6,8,m_buff);		//line2
/////          for(i=0;i<8;i++)
/////             Get_6x8_char_single(2,6+i*6,m_buff[i]);
/////       }

        i=Pro_wait_time0_flag;
        i&=0x00000001;
        if((Rx_TX_flag==0)&&(i==0))
        {
            if(select==1)
                Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_ADC_PHOTO_ADDS,0x0001,0x00);
            else
                Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_ADC_ISE_ADDS,0x0001,0x00);

            Pro_wait_time0_set=150;	//wait 1.5sec
            Pro_wait_time0_flag&=~0x02;
            Pro_wait_time0_flag|=0x01;
        }
        else if(Rx_TX_flag==0)
        {
            i=Pro_wait_time0_flag;
            i&=0x00000002;
            if(i==0)
            {
                i=U_data_buff[3];
                i=i<<8;
                i+=U_data_buff[4];

                Disp_real_time_vol_data(select,finc_sl,i);
                Pro_wait_time0_flag|=0x02;
            }
        }
    }
}

//===============================================================================
//	finction	:Get_ebf6_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Get_ebf6_data(uint8_t select,uint8_t finc_sl)
{
    uint8_t j,k;
    uint32_t i;//,disp_d;
//    float m,n;

/////    waitting_dsp_flag=0;
    k=0;
next_get:
    j=0;
    ++k;
    if(k==10) goto exit_g;
    if(select==1)
        Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_ADC_PHOTO_ADDS,0x0001,0x00);
    else
        Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_ADC_ISE_ADDS,0x0001,0x00);

    for(i=0; i<250; i++)
    {
        DrvSYS_Delay(20000);
        if(Rx_TX_flag==0) i=253;
        if(i==249) j=1;

    }
    Rx_TX_flag=0;
    if(j==1) goto next_get;
exit_g:
    Rx_TX_flag=0;
    i=U_data_buff[3];
    i=i<<8;
    i+=U_data_buff[4];
    E_buff[6]=i;
    Disp_real_time_vol_data(select,finc_sl,i);

////        Measurement_signal_11=i;
////     	e1_men_save=Measurement_signal_11;
////        E_ADC_temperature1=Read_cal_temp(U_SENSER_ADC_TEMP_ADDS);
////#ifdef 	__WD_BUCHANG__
////        m=E_ADC_temperature1;
////        m=(m-1024)/10.0;
////        n=1+(m-25)*0.0029;
////        m=e1_men_save;
////        disp_d=n*m;
////        e1_men_save=Measurement_signal_11=disp_d;
////     	Disp_real_time_vol_data(select,finc_sl,disp_d);
////#else
////    Disp_real_time_vol_data(select,finc_sl,i);
////#endif

}

//===============================================================================
//	finction	:Test_signal_1_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Test_signal_1_data(uint8_t select,uint8_t finc_sl)		//公共函数--分析读电压值半段稳定条件和读取时间判断
{
//    float k,r_data,m1,m2,m3,m4,m5;
//    uint32_t i,a,b;
    volatile float k,r_data;
    volatile uint32_t i;

    if(E_step_cnt==1)
    {
/////    	waitting_dsp_flag=0;
        E_step_cnt=2;
        E_wait_time_cnt=0;
        E_conter=0;
        E_temp=0;
        Delta_temp=0;
        Delta_cnt=0;

        r_data=0;

        i=M_menur1_DDSJ_temp>>12;
        i&=0x0000000f;
        k=i;
        k=k*1000;
        r_data+=k;

        i=M_menur1_DDSJ_temp>>8;
        i&=0x0000000f;
        k=i;
        k=k*100;
        r_data+=k;

        i=M_menur1_DDSJ_temp>>4;
        i&=0x0000000f;
        k=i;
        k=k*10;
        r_data+=k;

        i=M_menur1_DDSJ_temp;
        i&=0x0000000f;
        k=i;
        r_data+=k;

        E_wait_time_set=r_data;
        E_wait_time_set*=100;
        E_wait_time_cnt=0;
        E_wait_flag=1;

        if(select==1)
            Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_ADC_PHOTO_ADDS,0x0001,0x00);
        else
            Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_ADC_ISE_ADDS,0x0001,0x00);
    }
    else if((E_step_cnt==2)&&(Rx_TX_flag==0))
    {
        if(E_wait_flag==1)
        {
            if(E_conter<5)
            {
                i=U_data_buff[3];
                i=i<<8;
                i+=U_data_buff[4];
                E_buff[E_conter]=i;

                Disp_real_time_vol_data(select,finc_sl,i);

                ++E_conter;
            }
            else
            {
                i=U_data_buff[3];
                i=i<<8;
                i+=U_data_buff[4];
                E_temp=i;

                Disp_real_time_vol_data(select,finc_sl,i);

                r_data=0;
                i=M_menur1_XHPY_temp>>12;
                i&=0x0000000f;
                k=i;
                k=k*10;
                r_data+=k;

                i=M_menur1_XHPY_temp>>8;
                i&=0x0000000f;
                k=i;
                r_data+=k;

                i=M_menur1_XHPY_temp>>4;
                i&=0x0000000f;
                k=i;
                k=k/10.0;
                r_data+=k;

                i=M_menur1_XHPY_temp;
                i&=0x0000000f;
                k=i;
                k=k/100.0;
                r_data+=k;

//               if(select==1)
//               {
                if(E_conter<7)
                {
                    E_buff[E_conter]=E_temp;
                    ++E_conter;
                }
                else
                {
                    E_buff[0]=E_buff[1];
                    E_buff[1]=E_buff[2];
                    E_buff[2]=E_buff[3];
                    E_buff[3]=E_buff[4];
                    E_buff[4]=E_buff[5];
                    E_buff[5]=E_buff[6];
                    E_buff[6]=E_temp;
                }
                if(E_conter>=7)
                {
                    Delta_buff[0]=0;
                    for(i=0; i<7; i++)
                    {
                        if(Delta_buff[0]<E_buff[i])
                            Delta_buff[0]= E_buff[i];
                    }
                    Delta_buff[1]=E_buff[0];
                    for(i=0; i<7; i++)
                    {
                        if(Delta_buff[1]>E_buff[i])
                            Delta_buff[1]= E_buff[i];
                    }
                    if(Delta_buff[0]>Delta_buff[1])
                        Delta_buff[0]=Delta_buff[0]-Delta_buff[1];
                    else
                        Delta_buff[0]=Delta_buff[1]-Delta_buff[0];

                    k=Delta_buff[0];

                    if(M_menur1_LZDJ_select_temp==1) k/=100.0;
                    else                             k/=10.0;

                    if(k<=r_data)
                        E_step_cnt=6;
                }
            }
            if(E_step_cnt!=6)
                E_step_cnt=3;
            Pro_wait_time_set=150;	//wait 1.5sec
            Pro_wait_time_flag=1;
        }
        else
        {
            if(E_conter<7)
            {
//               E_buff[6]=E_buff[E_conter];
                Get_ebf6_data(select,finc_sl);
            }

            E_step_cnt=6;
        }
    }
//-------------------------------------------------
    else if(E_step_cnt==3)
    {
        if(E_wait_flag==1)
        {
            if(Pro_wait_time_flag==2)
            {
                E_step_cnt=2;
                if(select==1)
                    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_ADC_PHOTO_ADDS,0x0001,0x00);
                else
                    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_ADC_ISE_ADDS,0x0001,0x00);
            }
        }
        else
        {
            if(E_conter<7)
            {
//               E_buff[6]=E_buff[E_conter];
                Get_ebf6_data(select,finc_sl);
            }

            E_step_cnt=6;
        }
    }
    else if(E_wait_flag==0)
    {
        Get_ebf6_data(select,finc_sl);
        E_step_cnt=6;
    }
}

//===============================================================================
//	finction	:Get_test_signal_11
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Get_test_signal_11(void)			//比色法读E1电压值
{
    uint32_t disp_d;
//    float i,j;

    if(E_step_cnt!=6)
    {
        if((E_step_cnt!=1)&&(E_step_cnt!=2)&&(E_step_cnt!=3))
        {
            E_step_cnt=1;
/////    	   waitting_dsp_flag=0;

            All_program_menu_disp(9);
        }
        Test_signal_1_data(1,1);
    }
    else if(E_step_cnt==6)
    {
//       Measurement_signal_11=E_buff[2];
        E_step_cnt=7;
        Measurement_signal_11=E_buff[6];

        disp_d=Measurement_signal_11;
        e1_men_save=disp_d;
        E_ADC_temperature1=Read_cal_temp(U_SENSER_ADC_TEMP_ADDS);
#ifdef 	__WD_BUCHANG__
        i=E_ADC_temperature1;
        i=(i-1024)/10.0;
        j=1+(i-25)*0.0029;
        i=e1_men_save;
        disp_d=j*i;
        e1_men_save=Measurement_signal_11=disp_d;
        Disp_real_time_vol_data(1,1,disp_d);
#else
        Disp_real_time_vol_data(1,1,disp_d);
#endif
    }
}

//===============================================================================
//	finction	:Get_test_signal_12
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Get_test_signal_12(void)			//比色法读E2电压值
{
    uint32_t disp_d;
//    float i,j;

    if(E_step_cnt!=6)
    {
        if((E_step_cnt!=1)&&(E_step_cnt!=2)&&(E_step_cnt!=3))
        {
            E_step_cnt=1;
/////    	   waitting_dsp_flag=0;

            All_program_menu_disp(10);
        }
        Test_signal_1_data(1,2);
    }
    else if(E_step_cnt==6)
    {
//       Measurement_signal_12=E_buff[2];
        E_step_cnt=7;
        Measurement_signal_12=E_buff[6];

        disp_d=Measurement_signal_12;
        e2_men_save=disp_d;

        E_ADC_temperature1=Read_cal_temp(U_SENSER_ADC_TEMP1_ADDS);		//读悬空温度=电极法传感器 2014-6-27 13:39
        __nop();
        __nop();
        __nop();
        __nop();
        E_ADC_temperature2=Read_cal_temp(U_SENSER_ADC_TEMP_ADDS);		//读光敏管温度
#ifdef 	__WD_BUCHANG__
        i=E_ADC_temperature2;
        i=(i-1024)/10.0;
        j=1+(i-25)*0.0029;
        i=e2_men_save;
        disp_d=j*i;
        e2_men_save=Measurement_signal_12=disp_d;
        Disp_real_time_vol_data(1,2,disp_d);
#else
        Disp_real_time_vol_data(1,2,disp_d);
#endif

    }
}

//===============================================================================
//	finction	:Get_test_signal_21
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Get_test_signal_21(void)			//电极法读E1电压值
{
    uint32_t disp_d;

    if(E_step_cnt!=6)
    {
        if((E_step_cnt!=1)&&(E_step_cnt!=2)&&(E_step_cnt!=3))
        {
            E_step_cnt=1;
/////    	   waitting_dsp_flag=0;

            All_program_menu_disp(11);
        }
        Test_signal_1_data(2,1);
    }
    else if(E_step_cnt==6)
    {
        E_step_cnt=7;
        Measurement_signal_21=E_buff[6];

        disp_d=Measurement_signal_21;
        e1_men_save=disp_d;

        Disp_real_time_vol_data(2,1,disp_d);

#ifdef __OLD_NTC__
        E_ADC_temperature1=Read_cal_temp(U_SENSER_ADC_TEMP_ADDS);
#else
        E_ADC_temperature1=Read_cal_temp(U_SENSER_ADC_TEMP1_ADDS);
#endif
    }
}

//===============================================================================
//	finction	:Get_test_signal_22
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Get_test_signal_22(void)			//电极法读E2电压值
{
    uint32_t disp_d;

    if(E_step_cnt!=6)
    {
        if((E_step_cnt!=1)&&(E_step_cnt!=2)&&(E_step_cnt!=3))
        {
            E_step_cnt=1;
/////    	   waitting_dsp_flag=0;

            All_program_menu_disp(12);
        }
        Test_signal_1_data(2,2);
    }
    else if(E_step_cnt==6)
    {
        E_step_cnt=7;
        Measurement_signal_22=E_buff[6];

        disp_d=Measurement_signal_22;
        e2_men_save=disp_d;

        Disp_real_time_vol_data(2,2,disp_d);

#ifdef __OLD_NTC__
        E_ADC_temperature2=Read_cal_temp(U_SENSER_ADC_TEMP_ADDS);
#else
        E_ADC_temperature2=Read_cal_temp(U_SENSER_ADC_TEMP1_ADDS);
#endif
    }
}

//===============================================================================
//	finction	:Hand_opration_finction
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.07.09
//	modefy		:null
//===============================================================================
void Hand_opration_finction(void)			//手动操作各种填充控制函数
{
    if((Auto_check_clock_flag!=1)&&(Menu_lever3_start_flag<0x10)&&(Pro_power_on_flag==0))
    {
        switch(Hand_finction_select)
        {
        case 1:
            System_fill();
            break;
        case 2:
            Standar1_fill();
            break;
        case 3:
            Standar2_fill();
            break;
        case 4:
            Reagent1_fill();
            break;
        case 5:
            Reagent2_fill();
            break;
        case 6:
            Reagent3_fill();
            break;
        case 7:
            Sample_fill();
            break;
        case 8:
            Reagent4_fill();
            break;
        }
    }
}

//===============================================================================
//	finction	:System_fill
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.07.09
//	modefy		:null
//===============================================================================
void System_fill(void)			//样品填充
{
    uint8_t i;

    if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
    {
        Pro_wait_time_set=60;	//wait 500ms
        switch(Hand_finction_step_select)
        {
        case 0:
            Set_back_light_10sec();
            break;
        case 20:
            i=M_menur1_sj2_v_temp;
            if((i==0)&&(M_menur1_jz2_v_temp==0))
                //if((M_menur1_sj2_v_temp==0)&&(M_menur1_jz2_v_temp==0))
            {
                Hand_finction_step_select=28;  	//v1 on
            }
            else
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA5_WO_ADDS,BATTERY_VAL_OPEN,0x00);
                ++Hand_finction_step_select;
            }
            goto ex_System_fill;
        case 24:
            if(M_menur1_sj3_v_temp==0)
            {
                Hand_finction_step_select=28;  	//v1 on
            }
            else
            {
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA6_WO_ADDS,BATTERY_VAL_OPEN,0x00);
                ++Hand_finction_step_select;
            }
            goto ex_System_fill;
        case 37:
            Set_JB_direction(); 		//moto on
            Pro_wait_time_set=500;	//wait 5sec
            Pro_wait_time_flag=1;
            ++Hand_finction_step_select;
            goto ex_System_fill;
        case 46:
            Hand_finction_step_select=0xff;
            Hand_finction_select=0;
            Back_light_time_cnt=0;
            Back_light_time_flag=1;
            goto ex_System_fill;
        }
        if(Hand_finction_step_select!=0xff)
        {
            DrvSYS_Delay(10000);
            Pub_finc_index(TB_System_fill_index,Hand_finction_step_select);
            ++Hand_finction_step_select;
        }
ex_System_fill:
        Pro_wait_time_flag=1;
    }
}

//===============================================================================
//	finction	:Standar1_fill
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.07.09
//	modefy		:null
//===============================================================================
void Standar1_fill(void)			//标液1填充
{
    if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
    {
        Pro_wait_time_set=60;	//wait 500ms
        switch(Hand_finction_step_select)
        {
        case 0:
            Set_back_light_10sec();
            break;
        case 25:
            Set_JB_direction(); 		//moto on
            Pro_wait_time_set=500;	//wait 5sec
            Pro_wait_time_flag=1;
            ++Hand_finction_step_select;
            goto ex_Standar1_fill;
        case 34:
            Hand_finction_step_select=0xff;
            Hand_finction_select=0;
            Back_light_time_cnt=0;
            Back_light_time_flag=1;
            goto ex_Standar1_fill;
        }
        if(Hand_finction_step_select!=0xff)
        {
            DrvSYS_Delay(10000);
            Pub_finc_index(TB_Standar1_fill_index,Hand_finction_step_select);
            ++Hand_finction_step_select;
        }
ex_Standar1_fill:
        Pro_wait_time_flag=1;
    }
}

//===============================================================================
//	finction	:Standar2_fill
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.07.09
//	modefy		:null
//===============================================================================
void Standar2_fill(void)			//标液2填充
{
    if(M_menur1_jz2_v_temp!=0)
    {
        if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
        {
            Pro_wait_time_set=60;	//wait 500ms
            switch(Hand_finction_step_select)
            {
            case 0:
                Set_back_light_10sec();
                break;
            case 21:
                Set_JB_direction(); 		//moto on
                Pro_wait_time_set=500;	//wait 5sec
                Pro_wait_time_flag=1;
                ++Hand_finction_step_select;
                goto ex_Standar2_fill;
            case 30:
                Hand_finction_step_select=0xff;
                Hand_finction_select=0;
                Back_light_time_cnt=0;
                Back_light_time_flag=1;
                goto ex_Standar2_fill;
            }
            if(Hand_finction_step_select!=0xff)
            {
                DrvSYS_Delay(10000);
                Pub_finc_index(TB_Standar2_fill_index,Hand_finction_step_select);
                ++Hand_finction_step_select;
            }
ex_Standar2_fill:
            Pro_wait_time_flag=1;
        }
    }
}

//===============================================================================
//	finction	:Reagent1_fill
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.07.09
//	modefy		:null
//===============================================================================
void Reagent1_fill(void)			//试剂1填充
{
    if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
    {
        Pro_wait_time_set=60;	//wait 500ms
        switch(Hand_finction_step_select)
        {
        case 0:
            Set_back_light_10sec();
            break;
        case 21:
            Set_JB_direction(); 		//moto on
            Pro_wait_time_set=500;	//wait 5sec
            Pro_wait_time_flag=1;
            ++Hand_finction_step_select;
            goto ex_Reagent1_fill;
        case 30:
            Hand_finction_step_select=0xff;
            Hand_finction_select=0;
            Back_light_time_cnt=0;
            Back_light_time_flag=1;
            goto ex_Reagent1_fill;
        }
        if(Hand_finction_step_select!=0xff)
        {
            DrvSYS_Delay(10000);
            Pub_finc_index(TB_Reagent1_fill_index,Hand_finction_step_select);
            ++Hand_finction_step_select;
        }
ex_Reagent1_fill:
        Pro_wait_time_flag=1;
    }
}

//===============================================================================
//	finction	:Reagent2_fill
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.07.09
//	modefy		:null
//===============================================================================
void Reagent2_fill(void)			//试剂2填充
{
    uint8_t i;

    i=M_menur1_sj2_v_temp;
    if(i!=0)
        //if(M_menur1_sj2_v_temp!=0)
    {
        if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
        {
            Pro_wait_time_set=60;	//wait 500ms
            switch(Hand_finction_step_select)
            {
            case 0:
                Set_back_light_10sec();
                break;
            case 21:
                Set_JB_direction(); 		//moto on
                Pro_wait_time_set=500;	//wait 5sec
                Pro_wait_time_flag=1;
                ++Hand_finction_step_select;
                goto ex_Reagent2_fill;
            case 30:
                Hand_finction_step_select=0xff;
                Hand_finction_select=0;
                Back_light_time_cnt=0;
                Back_light_time_flag=1;
                goto ex_Reagent2_fill;
            }
            if(Hand_finction_step_select!=0xff)
            {
                DrvSYS_Delay(10000);
                Pub_finc_index(TB_Reagent2_fill_index,Hand_finction_step_select);
                ++Hand_finction_step_select;
            }
ex_Reagent2_fill:
            Pro_wait_time_flag=1;
        }
    }
}

//===============================================================================
//	finction	:Reagent3_fill
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.07.09
//	modefy		:null
//===============================================================================
void Reagent3_fill(void)			//试剂3填充
{
    if(M_menur1_sj3_v_temp!=0)
    {
        if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
        {
            Pro_wait_time_set=60;	//wait 500ms
            switch(Hand_finction_step_select)
            {
            case 0:
                Set_back_light_10sec();
                break;
            case 21:
                Set_JB_direction(); 		//moto on
                Pro_wait_time_set=500;	//wait 5sec
                Pro_wait_time_flag=1;
                ++Hand_finction_step_select;
                goto ex_Reagent3_fill;
            case 30:
                Hand_finction_step_select=0xff;
                Hand_finction_select=0;
                Back_light_time_cnt=0;
                Back_light_time_flag=1;
                goto ex_Reagent3_fill;
            }
            if(Hand_finction_step_select!=0xff)
            {
                DrvSYS_Delay(10000);
                Pub_finc_index(TB_Reagent3_fill_index,Hand_finction_step_select);
                ++Hand_finction_step_select;
            }
ex_Reagent3_fill:
            Pro_wait_time_flag=1;
        }
    }
}

//===============================================================================
//	finction	:Reagent4_fill
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-4 17:15
//	modefy		:null
//===============================================================================
void Reagent4_fill(void)			//试剂4填充
{
    if(M_menur1_sj4_v_temp!=0)
    {
        if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
        {
            Pro_wait_time_set=60;	//wait 500ms
            switch(Hand_finction_step_select)
            {
            case 0:
                Set_back_light_10sec();
                break;
            case 21:
                Set_JB_direction(); 		//moto on
                Pro_wait_time_set=500;	//wait 5sec
                Pro_wait_time_flag=1;
                ++Hand_finction_step_select;
                goto ex_Reagent4_fill;
            case 30:
                Hand_finction_step_select=0xff;
                Hand_finction_select=0;
                Back_light_time_cnt=0;
                Back_light_time_flag=1;
                goto ex_Reagent4_fill;
            }
            if(Hand_finction_step_select!=0xff)
            {
                DrvSYS_Delay(10000);
                Pub_finc_index(TB_Reagent4_fill_index,Hand_finction_step_select);
                ++Hand_finction_step_select;
            }
ex_Reagent4_fill:
            Pro_wait_time_flag=1;
        }
    }
}

//===============================================================================
//	finction	:Sample_fill
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.07.09
//	modefy		:null
//===============================================================================
void Sample_fill(void)			//样品填充
{
    if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
    {
        Pro_wait_time_set=60;	//wait 500ms
        switch(Hand_finction_step_select)
        {
        case 0:
            Set_back_light_10sec();
            break;
        case 24:
            Set_JB_direction(); 		//moto on
            Pro_wait_time_set=500;	//wait 5sec
            Pro_wait_time_flag=1;
            ++Hand_finction_step_select;
            goto ex_Sample_fill;
        case 34:
            Hand_finction_step_select=0xff;
            Hand_finction_select=0;
            Back_light_time_cnt=0;
            Back_light_time_flag=1;
            goto ex_Sample_fill;
        }
        if(Hand_finction_step_select!=0xff)
        {
            DrvSYS_Delay(10000);
            Pub_finc_index(TB_Sample_fill_index,Hand_finction_step_select);
            ++Hand_finction_step_select;
        }
ex_Sample_fill:
        Pro_wait_time_flag=1;
    }
}

//===============================================================================
//	finction	:Clear_finction
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-11-9 11:54
//	modefy		:null
//===============================================================================
uint8_t Fa_clean_select_temp,clean_loop_cnt;

void Clear_finction(void)			//清洗
{
    uint8_t i;

    if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
    {
        Pro_wait_time_set=60;	//wait 500ms
        switch(Pro_step_status)
        {
        case 0:
            Fa_clean_select_temp=0;
            clean_loop_cnt=0;
            All_program_menu_disp(17);

            i=M_menur1_sj2_v_temp;
            if((M_menur1_sj1_v_temp!=0)&&(i==0))
                //if((M_menur1_sj1_v_temp!=0)&&(M_menur1_sj2_v_temp==0))
            {
                if(M_menur1_jz2_v_temp == 0)		//2016/9/18 19:48
                {
                    Fa_clean_select_temp=5;
                }
                else
                {
                    Fa_clean_select_temp=6;
                }
                M_menur1_sj3_v_temp=M_menur1_sj4_v_temp=0;
            }
            else if((i!=0)&&(M_menur1_sj3_v_temp==0))
                //else if((M_menur1_sj2_v_temp!=0)&&(M_menur1_sj3_v_temp==0))
            {
                if(M_menur1_jz2_v_temp == 0)		//2016/9/18 19:48
                {
                    Fa_clean_select_temp=6;
                }
                else
                {
                    Fa_clean_select_temp=7;
                }
                M_menur1_sj4_v_temp=0;
            }
            else if((M_menur1_sj3_v_temp!=0)&&(M_menur1_sj4_v_temp==0))
            {
                if(M_menur1_jz2_v_temp == 0)		//2016/9/18 19:48
                {
                    Fa_clean_select_temp=7;
                }
                else
                {
                    Fa_clean_select_temp=9;
                }
            }
            else
                Fa_clean_select_temp=9;
            break;
        case 7:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_MOTO_RST_REG_CTRL_WO_ADDS,ZHUSEBUN_RESET,0x00);	//reset zhu she bon moto
            ++Pro_step_status;
            goto ex_Clear;
        case 10:
        case 19:
        case 28:
        case 32:
            Set_switch_open_close(Fa_clean_select_temp,1);					 //open vx
            ++Pro_step_status;
            goto ex_Clear;
        case 12:
        case 21:
        case 30:
        case 34:
            Set_switch_open_close(Fa_clean_select_temp,0);					 //close vx
            ++Pro_step_status;
            goto ex_Clear;
        case 27:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA3_WO_ADDS,BATTERY_VAL_CLOSE,0x00); 	//close v3

            ++clean_loop_cnt;
            if(clean_loop_cnt==2) 			//do tow times
            {
                ++Pro_step_status;
                clean_loop_cnt=0;
            }
            else
                Pro_step_status=7;
            goto ex_Clear;
        case 36:
            Set_JB_direction(); 		//moto on
            Pro_wait_time_set=6000;	//wait 60sec
            Pro_wait_time_flag=1;
            ++Pro_step_status;
            goto ex_Clear;
        case 39:
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA8_WO_ADDS,BATTERY_VAL_CLOSE,0x00); 	//close v8
            ++clean_loop_cnt;
            if(clean_loop_cnt==2) 			//do tow times
            {
                ++Pro_step_status;
                clean_loop_cnt=0;
            }
            else
                Pro_step_status=26;
            goto ex_Clear;
        case 53:
            Set_ZSB_move_to(0x00000000);
            Pro_step_status=0xff;
            goto ex_Clear;
        }
        if(Pro_step_status!=0xff)
        {
            DrvSYS_Delay(10000);
            Pub_finc_index(TB_Clear_finction_index,Pro_step_status);
            ++Pro_step_status;
        }
ex_Clear:
        Pro_wait_time_flag=1;
    }
}

//===============================================================================
//	finction	:Menu1_hand_fenxi_finc
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Menu1_hand_fenxi_finc(void)			//手动分析控制流程函数
{
    uint8_t m_buff[9];
    volatile uint8_t i,j;
    volatile uint32_t Sub_adds;
    volatile float kk,temp,uctemp,cal_temp;
    volatile float a,b,c,d,rs,rds;

    switch(Memu1_hand_step_status)
    {
    case 0:
        DrvSYS_Delay(200000);
/////    	    tx_jisuan_data(1);					//for test

        Pro_wait_time0_flag=0;
        if((Pro_step_status==0xff)||(Pro_step_status==0x00))
        {
            ++Memu1_hand_step_status;
            Pro_step_status=0;
            c1_prog_use=0;
            c2_prog_use=0;
            c3_prog_use=0;
//    	       k1_prog_use=0;
            yes_not=0;
            if(Prog_disp_flag==0)
                Prog_disp_flag=1;
            if(Menu_lever3_start_flag!=BIT4)
            {
                if(M_menur1_language_select_temp==2)
                {
                    m_buff[0]=22;
                    m_buff[1]=36;
                    m_buff[2]=16;
                    m_buff[3]=17;
                    Display_program_5opration(4,m_buff);
                }
                else if(Prog_disp_flag==1)
                {
                    Clear_n_page(0,0);
                    Clear_n_page(2,1);
                    m_buff[0]='M';
                    m_buff[1]='A';
                    m_buff[2]='N';
                    m_buff[3]=19;
                    m_buff[4]=0;
                    m_buff[5]='A';
                    m_buff[6]='N';
                    m_buff[7]='A';
                    Display_6x8_char(0,0,8,m_buff);		//line1
                    for(i=0; i<8; i++)
                        Get_6x8_char_single(0,i*6,m_buff[i]);
                }
                if(Prog_disp_flag==1)
                {
                    Clear_n_page(4,4);
                }
            }
        }
//    	    else  Triggle_get_sample();

        Set_back_light_10sec();

        Measurement_signal_21=Measurement_signal_22=0;
        Prog_total_step=Get_step_num_each_finc(1);
        break;
    case 1:
        if(Pro_step_status==0xff)
        {
            ++Memu1_hand_step_status;
            Pro_step_status=0;
        }
        else  ZSB_initial();
        break;
    case 2:
        if(Pro_step_status==0xff)
        {

            Pro_step_status=0;

            if(Disple_flag!=1)
            {
                Memu1_hand_step_status=9;		//不做消解
            }
            else
            {
                U2_disple_flag=0;

//		   Reset_init_UART1(1);
//		   DrvSYS_Delay(100000);

                All_program_menu_disp(19);

                Set_tx_cmd_data_finc(CLEAR_ADDR_1,CLEAR_ADDR_2,0x6000,0x66BB,0x00);   //开始消解
                DrvSYS_Delay(200000);
                Set_tx_cmd_data_finc(CLEAR_ADDR_1,CLEAR_ADDR_2,0x6000,0x66BB,0x00);
                DrvSYS_Delay(200000);
                ++Memu1_hand_step_status;

                U_Rx_wait_time_flag_temp=0;
            }
        }
        else Triggle_get_sample();
        break;
    case 3:		//等待消解完成,然后取消解液
        //if(U2_disple_flag==1)
        if((U2_disple_flag==1) ||((U_data_buff[1] == 0x06)&&(U_data_buff[2]==0x1F)))
        {
            U2_disple_flag=0;
//    	    	Reset_init_UART1(0);
            Memu1_hand_step_status=9;
        }
        else if(Menu_lever3_start_flag==BIT4)
        {
            if((Pro_auto_time_dcnt==0)&&(Pro_auto_time_up_check_flag==1))
            {
                //U2_disple_flag=1;
                Rx_TX_flag=0;
                Memu1_hand_step_status=9;
            }
        }
        break;
    case 9:
        if(Pro_step_status==0xff)
        {
            ++Memu1_hand_step_status;
            Pro_step_status=0;
        }
        else  Sample_washing();
        break;
    case 10:
        if(Pro_step_status==0xff)
        {
            ++Memu1_hand_step_status;
            Pro_step_status=0;
        }
        else  Sample_adding();
        break;
    case 11:
        if(Pro_step_status==0xff)
        {
            Pro_step_status=0;
            if(M_menur1_LZDJ_select_temp==2)			//比色法
            {
                i=M_menur1_sj3_v_temp;
                if(i==0)
                    //if(M_menur1_sj3_v_temp==0)
                {
                    Memu1_hand_step_status=16;
                    E_step_cnt=0;

//                      Pro_wait_time_set=200;	//wait 2sec
                    Pro_wait_time_set=Get_e_wait_time();
                    Pro_wait_time_flag=1;
                }
                else
                {
                    i=M_menur1_sj4_v_temp;
                    if(i==0)
                        //if(M_menur1_sj4_v_temp==0)
                        ++Memu1_hand_step_status;
                    else
                        Memu1_hand_step_status=30;	//2012-9-5 10:05
                }
            }
            else if(M_menur1_LZDJ_select_temp==1)		//电极法
            {
                i=M_menur1_sj2_v_temp;
                if(i==0)
                    //if(M_menur1_sj2_v_temp==0)
                {
                    Memu1_hand_step_status=21;
                    E_step_cnt=0;

//                      Pro_wait_time_set=200;	//wait 2sec
                    Pro_wait_time_set=Get_e_wait_time();
                    Pro_wait_time_flag=1;
                }
                else
                {
                    Memu1_hand_step_status=20;
                }
            }
        }
        else  SJ1_addtion();
        break;
    case 12:
        if(Pro_step_status==0xff)
        {
            ++Memu1_hand_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ2_addtion();
        break;
    case 13:
        if(E_step_cnt==7)
        {
            ++Memu1_hand_step_status;
            Pro_step_status=0;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_11();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,1);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,1);
            }
        }
        break;
    case 14:
        if(Pro_step_status==0xff)
        {
            ++Memu1_hand_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ3_addtion();
        break;
    case 15:
        if(E_step_cnt==7)
        {
            Memu1_hand_step_status=19;
            Pro_step_status=0;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_12();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,2);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,2);
            }
        }
        break;
    case 16:
        if(E_step_cnt==7)
        {
            ++Memu1_hand_step_status;
            Pro_step_status=0;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_11();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,1);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,1);
            }
        }
        break;
    case 17:
        if(Pro_step_status==0xff)
        {
            ++Memu1_hand_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ2_addtion();
        break;
    case 18:
        if(E_step_cnt==7)
        {
            Memu1_hand_step_status=19;
            Pro_step_status=0;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_12();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,2);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,2);
            }
        }
        break;
    case 19:
        c1_prog_use=BSFND_calculat();		//计算比色法分析浓度
        Pro_step_status=0;
        Memu1_hand_step_status=26;
        break;
    case 20:
        if(Pro_step_status==0xff)
        {
            ++Memu1_hand_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ2_addtion();
        break;
    case 21:
        if(E_step_cnt==7)
        {
            Pro_step_status=0;
            if(M_menur1_DL_select_temp==1)
                ++Memu1_hand_step_status;
            else
                Memu1_hand_step_status=25;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_21();
        else if(E_step_cnt==0)
            Test_signal_data_nc(2,1);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(2,1);
            }
        }
        break;
    case 22:
        if(Pro_step_status==0xff)
        {
            ++Memu1_hand_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  BY2_addtion();
        break;
    case 23:
        if(E_step_cnt==7)
        {
            ++Memu1_hand_step_status;
            Pro_step_status=0;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_22();
        else if(E_step_cnt==0)
            Test_signal_data_nc(2,2);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(2,2);
            }
        }
        break;
    case 24:
        Memu1_hand_step_status=26;
        c2_prog_use=DJFFXND_calculat();			//计算电极法分析浓度

/////    	    c3_prog_use=DJFCLJG_calculat();		//2012-10-12 14:19

        Pro_step_status=0;
        break;
    case 25:
        Memu1_hand_step_status=26;
        c3_prog_use=DJFCLJG_calculat();			//计算电极法测量结果
        Pro_step_status=0;
        break;
    case 26:
        ++Memu1_hand_step_status;
        break;
    case 27:
        if(Pro_step_status==0xff)
        {
            Memu1_hand_step_status=0xff;
            Pro_step_status=0;
            E_step_cnt=0;
            Pro_dis_menu_flag=0;
            program_menu_select=0;
            ++M_lever4_FXCS_cnt;
            Prog_step_disp_cnt=Prog_total_step=0;		//2012-10-18 11:23

//=====================================================
            uctemp=temp=cal_temp=0;
            i=ZB_flag;
            i&=0x02;
            if(i!=0)				//是否启用AX+B
            {
                i=Set_xishu>>20;
                i&=0x0f;
                temp=i;
                uctemp+=temp*100;
                i=Set_xishu>>16;
                i&=0x0f;
                temp=i;
                uctemp+=temp*10;
                i=Set_xishu>>12;
                i&=0x0f;
                temp=i;
                uctemp+=temp;

                i=Set_xishu>>8;
                i&=0x0f;
                temp=i;
                uctemp+=temp/10.0;
                i=Set_xishu>>4;
                i&=0x0f;
                temp=i;
                uctemp+=temp/100.0;
                i=Set_xishu;
                i&=0x0f;
                temp=i;
                uctemp+=temp/1000.0;
                //------------------------------------get a
                i=set_cal_data>>20;
                i&=0x0f;
                temp=i;
                cal_temp+=temp*100;
                i=set_cal_data>>16;
                i&=0x0f;
                temp=i;
                cal_temp+=temp*10;
                i=set_cal_data>>12;
                i&=0x0f;
                temp=i;
                cal_temp+=temp;

                i=set_cal_data>>8;
                i&=0x0f;
                temp=i;
                cal_temp+=temp/10.0;
                i=set_cal_data>>4;
                i&=0x0f;
                temp=i;
                cal_temp+=temp/100.0;
                i=set_cal_data;
                i&=0x0f;
                temp=i;
                cal_temp+=temp/1000.0;
                //------------------------------------get b
                if(c1_prog_use!=0)
                    kk=c1_prog_use;
                else if(c2_prog_use!=0)
                    kk=c2_prog_use;
                else if(c3_prog_use!=0)
                    kk=c3_prog_use;
                else
                    kk=0;
                //------------------------------------get x
                temp=uctemp*kk+cal_temp;		//r=ax+b

                //------------------------------------
                i=Pro_auto_time_dcnt>>4;
                i%=10;
                switch(i)
                {
                case 0:				//-2%
                case 5:
                    uctemp=temp;
                    kk=uctemp*0.02;		//get c
                    temp-=kk;
                    break;
                case 1:				//-1%
                case 6:
                    uctemp=temp;
                    kk=uctemp*0.01;		//get c
                    temp-=kk;
                    break;
                case 2:				//0
                case 7:
                    break;
                case 3:				//1%
                case 8:
                    uctemp=temp;
                    kk=uctemp*0.01;		//get c
                    temp+=kk;
                    break;
                case 4:				//2%
                case 9:
                    uctemp=temp;
                    kk=uctemp*0.02;		//get c
                    temp+=kk;
                    break;
                }
                //------------------------------------temp=ax+b+c
                kk=temp;
                if(c1_prog_use!=0)
                    c1_prog_use=kk;
                else if(c2_prog_use!=0)
                    c2_prog_use=kk;
                else if(c3_prog_use!=0)
                    c3_prog_use=kk;
            }
//====================================================
            Display_program_result(1);
            if(result_signe==2)
                yes_not=1;

            if((Memu1_auto_step_status==0)||(Memu1_auto_step_status==0xff))
            {
                Back_light_time_cnt=0;
                Back_light_time_flag=1;
                Get_write_struct_data(1,1,Pub_result_temp,M_menur1_unit_select_temp,yes_not,result_signe);	 //save data

                i=M_lever4_FXCS_temp>>4;
                i&=0x0f;
                i*=10;
                j=M_lever4_FXCS_temp;
                j&=0x0f;
                i+=j;
////                  if(M_lever4_FXCS_cnt<M_lever4_FXCS_temp)
                if(M_lever4_FXCS_cnt<i)
                    Memu1_hand_step_status=0;
                else
                {
                    M_lever4_FXCS_cnt=0;
                    Menu_lever3_start_flag&=~BIT5;
                    Write_system_reg_to_chip();
                    Prog_disp_flag=0;
                    Memu1_hand_step_status=0;
                }
            }
            else
                Get_write_struct_data(1,3,Pub_result_temp,M_menur1_unit_select_temp,yes_not,result_signe);	 //save data

            Sub_adds=Write_result_to_flash(FX_write_index_adds_point,&Fenxi);
            FX_write_index_adds_point=Sub_adds;
            Write_index_adds_point();

            if((Memu1_auto_step_status==0)||(Memu1_auto_step_status==0xff))		//2013-6-6 9:59
                Display_last_result(FX_write_index_adds_point);

            Pwm_get_resul_from_flash(1);
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
        else if(Pro_wait_time_flag==2)
            PaiKong_washing();
        break;
//-------------------------------
    case 30:
        if(Pro_step_status==0xff)
        {
            ++Memu1_hand_step_status;
            Pro_step_status=0;
            E_step_cnt=0;
        }
        else  SJ2_addtion();
        break;
    case 31:
        if(Pro_step_status==0xff)
        {
            ++Memu1_hand_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ3_addtion();
        break;
    case 32:
        if(E_step_cnt==7)
        {
            ++Memu1_hand_step_status;
            Pro_step_status=0;
        }
        else  if(Pro_wait_time_flag==2)
            Get_test_signal_11();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,1);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,1);
            }
        }
        break;
    case 33:
        if(Pro_step_status==0xff)
        {
            ++Memu1_hand_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ4_addtion();
        break;
    case 34:
        if(E_step_cnt==7)
        {
            Memu1_hand_step_status=19;
            Pro_step_status=0;
        }
        else  if(Pro_wait_time_flag==2)
            Get_test_signal_12();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,2);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,2);
            }
        }
        break;
    }
}
//===============================================================================
//	finction	:Menu1_hand_jiaozheng_finc
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Menu1_hand_jiaozheng_finc(void)		//手动校正流程控制函数
{
    uint8_t m_buff[8];
    volatile uint8_t i;
    volatile uint32_t Sub_adds;

    switch(Memu1_jiaozheng_step_status)
    {
    case 0:
        if((Memu1_auto_step_status==0)||(Memu1_auto_step_status==0xff))
            Led_data_flag|=BIT3;

        ++Memu1_jiaozheng_step_status;
        Pro_wait_time0_flag=0;
        Pro_step_status=0;
        k1_prog_use=0;
        k2_prog_use=0;
        j_prog_use=0;

        if(Prog_disp_flag==0)
            Prog_disp_flag=1;

        if(M_menur1_language_select_temp==2)	//中文
        {
            m_buff[0]=22;
            m_buff[1]=36;
            m_buff[2]=37;
            m_buff[3]=38;
            Display_program_5opration(4,m_buff);
        }
        else					//英文
        {
            m_buff[0]='M';
            m_buff[1]='A';
            m_buff[2]='N';
            m_buff[3]=19;
            m_buff[4]=0;
            m_buff[5]='C';
            m_buff[6]='A';
            m_buff[7]='L';
            Display_6x8_char(0,0,8,m_buff);		//line1
            for(i=0; i<8; i++)
                Get_6x8_char_single(0,i*6,m_buff[i]);
        }

        if(Prog_disp_flag==1)
        {
            Clear_n_page(4,4);
        }
        Set_back_light_10sec();
        Prog_total_step=Get_step_num_each_finc(2);
        break;
    case 1:
        if(Pro_step_status==0xff)
        {
            ++Memu1_jiaozheng_step_status;
            Pro_step_status=0;
            if(Disple_flag==1)
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA10_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            DrvSYS_Delay(200000);
        }
        else  ZSB_initial();
        break;
    case 2:
        if(Disple_flag!=1) Memu1_jiaozheng_step_status=8;	//不做消解
        else
        {
            U2_disple_flag=0;

//              Reset_init_UART1(1);
//              DrvSYS_Delay(100000);

            All_program_menu_disp(19);

            Set_tx_cmd_data_finc(CLEAR_ADDR_1,CLEAR_ADDR_2,0x6000,0x66BB,0x00);   //开始消解
            DrvSYS_Delay(200000);
            Set_tx_cmd_data_finc(CLEAR_ADDR_1,CLEAR_ADDR_2,0x6000,0x66BB,0x00);
            DrvSYS_Delay(200000);
            ++Memu1_jiaozheng_step_status;
            U_Rx_wait_time_flag_temp=0;
        }
        break;
    case 3:		//等待消解完成,然后取消解液
        //if(U2_disple_flag==1)
        if((U2_disple_flag==1) ||((U_data_buff[1] == 0x06)&&(U_data_buff[2]==0x1F)))
        {
            U2_disple_flag=0;
//    	    	Reset_init_UART1(0);
            Memu1_jiaozheng_step_status=8;
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA10_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            DrvSYS_Delay(200000);
        }
        else if(Menu_lever3_start_flag==BIT4)
        {
            if((Pro_auto_time_dcnt==0)&&(Pro_auto_time_up_check_flag==1))
            {
                //U2_disple_flag=1;
                Rx_TX_flag=0;
                Memu1_jiaozheng_step_status=8;
            }
        }
        break;
    case 8:
        if(Pro_step_status==0xff)
        {
            ++Memu1_jiaozheng_step_status;
            Pro_step_status=0;
        }
        //else  BY1_wash();
        else Sample_washing();
        break;
    case 9:
        if(Pro_step_status==0xff)
        {
            ++Memu1_jiaozheng_step_status;
            Pro_step_status=0;
        }
        //else  BY1_addtion();
        else Sample_adding();
        break;
    case 10:
        if(Pro_step_status==0xff)
        {
            Pro_step_status=0;
            if(M_menur1_LZDJ_select_temp==2)			//比色法
            {
                i=M_menur1_sj3_v_temp;
                if(i==0)
                    //if(M_menur1_sj3_v_temp==0)
                {
                    Memu1_jiaozheng_step_status=15;
                    E_step_cnt=0;

//                      Pro_wait_time_set=200;	//wait 2sec
                    Pro_wait_time_set=Get_e_wait_time();
                    Pro_wait_time_flag=1;
                }
                else
                {
                    i=M_menur1_sj4_v_temp;
                    if(i==0)
                        //if(M_menur1_sj4_v_temp==0)
                        ++Memu1_jiaozheng_step_status;
                    else
                        Memu1_jiaozheng_step_status=30;	//2012-9-5 10:05
                }
            }
            else if(M_menur1_LZDJ_select_temp==1)		//电极法
            {
                i=M_menur1_sj2_v_temp;
                if(i==0)
                    //if(M_menur1_sj2_v_temp==0)
                {
                    Memu1_jiaozheng_step_status=20;
                    E_step_cnt=0;

//                      Pro_wait_time_set=200;	//wait 2sec
                    Pro_wait_time_set=Get_e_wait_time();
                    Pro_wait_time_flag=1;
                }
                else
                {
                    Memu1_jiaozheng_step_status=19;
                }
            }
        }
        else  SJ1_addtion();
        break;
    case 11:
        if(Pro_step_status==0xff)
        {
            ++Memu1_jiaozheng_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ2_addtion();
        break;
    case 12:
        if(E_step_cnt==7)
        {
            ++Memu1_jiaozheng_step_status;
            Pro_step_status=0;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_11();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,1);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,1);
            }
        }
        break;
    case 13:
        if(Pro_step_status==0xff)
        {
            ++Memu1_jiaozheng_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ3_addtion();
        break;
    case 14:
        if(E_step_cnt==7)
        {
            Memu1_jiaozheng_step_status=18;
            Pro_step_status=0;
        }
        else  if(Pro_wait_time_flag==2)
            Get_test_signal_12();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,2);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,2);
            }
        }
        break;
    case 15:
        if(E_step_cnt==7)
        {
            ++Memu1_jiaozheng_step_status;
            Pro_step_status=0;
        }
        else  if(Pro_wait_time_flag==2)
            Get_test_signal_11();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,1);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,1);
            }
        }
        break;
    case 16:
        if(Pro_step_status==0xff)
        {
            ++Memu1_jiaozheng_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ2_addtion();
        break;
    case 17:
        if(E_step_cnt==7)
        {
            ++Memu1_jiaozheng_step_status;
            Pro_step_status=0;
        }
        else  if(Pro_wait_time_flag==2)
            Get_test_signal_12();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,2);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,2);
            }
        }
        break;
    case 18:
        k1_prog_use=JZYZ_calculat();		//计算校正因子--比色法

        k2_prog_use=k1_prog_use;			//2012-9-5 14:22 复用
        if(k2_prog_use>=0)
            k2_signe_flag=1;
        else
            k2_signe_flag=0;

        //if(Updata_slope_flag==1)
        if(Updata_slope_flag!=2)
        {
            M_menur1_slope_flag=k2_signe_flag;

            M_menur1_slope_temp=Float_to_int_reg(k2_prog_use);
            M_menur1_slope_temp>>=8;
        }
        Write_system_reg_to_chip();

        Memu1_jiaozheng_step_status=24;
        break;
    case 19:
        if(Pro_step_status==0xff)
        {
            ++Memu1_jiaozheng_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ2_addtion();
        break;
    case 20:
        if(E_step_cnt==7)
        {
            ++Memu1_jiaozheng_step_status;
            Pro_step_status=0;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_21();
        else if(E_step_cnt==0)
            Test_signal_data_nc(2,1);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(2,1);
            }
        }
        break;
    case 21:
        if(Pro_step_status==0xff)
        {
            ++Memu1_jiaozheng_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  BY2_addtion();
        break;
    case 22:
        if(E_step_cnt==7)
        {
            ++Memu1_jiaozheng_step_status;
            Pro_step_status=0;
        }
        else  if(Pro_wait_time_flag==2)
            Get_test_signal_22();
        else if(E_step_cnt==0)
            Test_signal_data_nc(2,2);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(2,2);
            }
        }
        break;
    case 23:
        DJXL_calculat();		//计算电极斜率--电极法

        k2_prog_use=k2_temp;
        if(k2_prog_use>=0)
            k2_signe_flag=1;
        else
            k2_signe_flag=0;

        //if(Updata_slope_flag==1)
        if(Updata_slope_flag!=2)
        {
            M_menur1_slope_flag=k2_signe_flag;

            M_menur1_slope_temp=Float_to_int_reg(k2_prog_use);
            M_menur1_slope_temp>>=8;
        }
        Write_system_reg_to_chip();

        j_prog_use=j_temp;
        Pro_step_status=0;
        ++Memu1_jiaozheng_step_status;
        break;
    case 24:
        yes_not=0;
        if(k1_prog_use!=0)					// bisefa
        {
            if((k1_prog_use>=0.8)&&(k1_prog_use<=1.2))
                __nop();
            else
                yes_not=1;
        }
        else if(k2_prog_use!=0)
        {
            if(M_menur1_analy_obj_select_temp==1)		//CH4-N
            {
                if((k2_prog_use>=-65)&&(k2_prog_use<=-50))
                    __nop();
                else
                    yes_not=1;
            }
            if(M_menur1_analy_obj_select_temp==4)		//F
            {
                if((k2_prog_use>=50)&&(k2_prog_use<=65))
                    __nop();
                else
                    yes_not=1;
            }
        }
        else 		//all is 0
            yes_not=0;

        ++Memu1_jiaozheng_step_status;
        break;
    case 25:
        if(Pro_step_status==0xff)
        {
            Memu1_jiaozheng_step_status=0xff;
            Pro_step_status=0;
            E_step_cnt=0;
            Pro_dis_menu_flag=0;
            program_menu_select=0;

            Led_data_flag&=~BIT3;
            Display_program_result(2);

            if((Memu1_auto_step_status==0)||(Memu1_auto_step_status==0xff))
                Get_write_struct_data(2,6,Pub_result_temp,4,yes_not,result_signe);	 //手动校正

            else
                Get_write_struct_data(2,7,Pub_result_temp,4,yes_not,result_signe);	 //自动校正

            Sub_adds=Write_result_to_flash(JZ_write_index_adds_point,&Jiaozheng);
            JZ_write_index_adds_point=Sub_adds;

            Write_index_adds_point();
            if((Memu1_auto_step_status==0)||(Memu1_auto_step_status==0xff))
            {
//    	          PWMB->PCR.CH0EN=1;		//en backlight
                Back_light_time_cnt=0;
                Back_light_time_flag=1;
                Prog_disp_flag=0;
                Memu1_jiaozheng_step_status=0;

                Menu_lever3_start_flag&=~BIT6;
                Write_system_reg_to_chip();
            }
        }
        else  PaiKong_washing();
        break;
//-------------------------------
    case 30:
        if(Pro_step_status==0xff)
        {
            ++Memu1_jiaozheng_step_status;
            Pro_step_status=0;
            E_step_cnt=0;
        }
        else  SJ2_addtion();
        break;
    case 31:
        if(Pro_step_status==0xff)
        {
            ++Memu1_jiaozheng_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ3_addtion();
        break;
    case 32:
        if(E_step_cnt==7)
        {
            ++Memu1_jiaozheng_step_status;
            Pro_step_status=0;
        }
        else  if(Pro_wait_time_flag==2)
            Get_test_signal_11();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,1);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,1);
            }
        }
        break;
    case 33:
        if(Pro_step_status==0xff)
        {
            ++Memu1_jiaozheng_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ4_addtion();
        break;
    case 34:
        if(E_step_cnt==7)
        {
            Memu1_jiaozheng_step_status=18;
            Pro_step_status=0;
        }
        else  if(Pro_wait_time_flag==2)
            Get_test_signal_12();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,2);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,2);
            }
        }
        break;
    }
}


//===============================================================================
//	finction	:Menu1_biaoye_hecha_finc
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Menu1_biaoye_hecha_finc(void)		//标液核查流程控制函数
{
    uint8_t m_buff[8];
    volatile uint8_t i;
    volatile uint32_t Sub_adds;

    switch(Memu1_biaoye_step_status)
    {
    case 0:
        ++Memu1_biaoye_step_status;
        Pro_wait_time0_flag=0;
        Pro_step_status=0;
        c1_prog_use=0;
        c2_prog_use=0;
        c3_prog_use=0;
//    	    k1_prog_use=0;
        yes_not=0;
        if(Prog_disp_flag==0)
            Prog_disp_flag=1;

        if((Hand_finction_step_select==0)||(Hand_finction_step_select==0xff))
        {
            if(M_menur1_language_select_temp==2)
            {
                m_buff[0]=41;
                m_buff[1]=42;
                m_buff[2]=39;
                m_buff[3]=40;
                Display_program_5opration(4,m_buff);
            }
            else
            {
                m_buff[0]='A';
                m_buff[1]='U';
                m_buff[2]='T';
                m_buff[3]='O';
                m_buff[4]=0;
                m_buff[5]='C';
                m_buff[6]='H';
                m_buff[7]='K';
                Display_6x8_char(0,0,8,m_buff);		//line1
                for(i=0; i<8; i++)
                    Get_6x8_char_single(0,i*6,m_buff[i]);
            }
            if(Prog_disp_flag==1)
            {
                Clear_n_page(4,4);
            }
        }
        Set_back_light_10sec();
        Prog_total_step=Get_step_num_each_finc(4);		//取步数
        break;
    case 1:
        if(Pro_step_status==0xff)
        {
            ++Memu1_biaoye_step_status;
            Pro_step_status=0;
            if(Disple_flag==1)
                Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA10_WO_ADDS,BATTERY_VAL_OPEN,0x00);
            DrvSYS_Delay(200000);
        }
        else  ZSB_initial();
        break;
    case 2:
        if(Disple_flag!=1) Memu1_biaoye_step_status=8;	//不做消解
        else
        {
            U2_disple_flag=0;

//              Reset_init_UART1(1);
//              DrvSYS_Delay(100000);

            All_program_menu_disp(19);

            Set_tx_cmd_data_finc(CLEAR_ADDR_1,CLEAR_ADDR_2,0x6000,0x66BB,0x00);   //开始消解
            DrvSYS_Delay(200000);
            Set_tx_cmd_data_finc(CLEAR_ADDR_1,CLEAR_ADDR_2,0x6000,0x66BB,0x00);
            DrvSYS_Delay(200000);
            ++Memu1_biaoye_step_status;
            U_Rx_wait_time_flag_temp=0;
        }
        break;
    case 3:		//等待消解完成,然后取消解液
        //if(U2_disple_flag==1)
        if((U2_disple_flag==1)||((U_data_buff[1] == 0x06)&&(U_data_buff[2]==0x1F)))
        {
            U2_disple_flag=0;
//    	    	Reset_init_UART1(0);
            Memu1_biaoye_step_status=8;
            Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x06,U_DRIVER_FA10_WO_ADDS,BATTERY_VAL_CLOSE,0x00);
            DrvSYS_Delay(200000);
        }
        else if(Menu_lever3_start_flag==BIT4)
        {
            if((Pro_auto_time_dcnt==0)&&(Pro_auto_time_up_check_flag==1))
            {
                //U2_disple_flag=1;
                Rx_TX_flag=0;
                Memu1_biaoye_step_status=8;
            }
        }
        break;
    case 8:
        if(Pro_step_status==0xff)
        {
            ++Memu1_biaoye_step_status;
            Pro_step_status=0;
        }
        else  BY1_wash();
        break;
    case 9:
        if(Pro_step_status==0xff)
        {
            ++Memu1_biaoye_step_status;
            Pro_step_status=0;
        }
        else  BY1_addtion();
        break;
    case 10:
        if(Pro_step_status==0xff)
        {
            Pro_step_status=0;
            if(M_menur1_LZDJ_select_temp==2)			//比色法
            {
                i=M_menur1_sj3_v_temp;
                if(i==0)
                    //if(M_menur1_sj3_v_temp==0)
                {
                    Memu1_biaoye_step_status=15;
                    E_step_cnt=0;

//                      Pro_wait_time_set=200;	//wait 2sec
                    Pro_wait_time_set=Get_e_wait_time();
                    Pro_wait_time_flag=1;
                }
                else
                {
                    i=M_menur1_sj4_v_temp;
                    if(i==0)
                        //if(M_menur1_sj4_v_temp==0)
                        ++Memu1_biaoye_step_status;
                    else
                        Memu1_biaoye_step_status=30;	//2012-9-5 10:05
                }
            }
            else if(M_menur1_LZDJ_select_temp==1)		//电极法
            {
                i=M_menur1_sj2_v_temp;
                if(i==0)
                    //if(M_menur1_sj2_v_temp==0)
                {
                    Memu1_biaoye_step_status=20;
                    E_step_cnt=0;

//                      Pro_wait_time_set=200;	//wait 2sec
                    Pro_wait_time_set=Get_e_wait_time();
                    Pro_wait_time_flag=1;
                }
                else
                {
                    Memu1_biaoye_step_status=19;
                }
            }
        }
        else  SJ1_addtion();
        break;
    case 11:
        if(Pro_step_status==0xff)
        {
            ++Memu1_biaoye_step_status;
            Pro_step_status=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ2_addtion();
        break;
    case 12:
        if(E_step_cnt==7)
        {
            ++Memu1_biaoye_step_status;
            Pro_step_status=0;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_11();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,1);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,1);
            }
        }
        break;
    case 13:
        if(Pro_step_status==0xff)
        {
            ++Memu1_biaoye_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ3_addtion();
        break;
    case 14:
        if(E_step_cnt==7)
        {
            Memu1_biaoye_step_status=18;
            Pro_step_status=0;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_12();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,2);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,2);
            }
        }
        break;
    case 15:
        if(E_step_cnt==7)
        {
            ++Memu1_biaoye_step_status;
            Pro_step_status=0;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_11();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,1);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,1);
            }
        }
        break;
    case 16:
        if(Pro_step_status==0xff)
        {
            ++Memu1_biaoye_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ2_addtion();
        break;
    case 17:
        if(E_step_cnt==7)
        {
            Memu1_biaoye_step_status=18;
            Pro_step_status=0;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_12();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,2);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,2);
            }
        }
        break;
    case 18:
        c1_prog_use=BSFND_calculat();		//计算比色法浓度
        Memu1_biaoye_step_status=25;
        break;
    case 19:
        if(Pro_step_status==0xff)
        {
            ++Memu1_biaoye_step_status;
            Pro_step_status=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ2_addtion();
        break;
    case 20:
        if(E_step_cnt==7)
        {
            Pro_step_status=0;
            if(M_menur1_DL_select_temp==1)
                ++Memu1_biaoye_step_status;
            else
                Memu1_biaoye_step_status=24;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_21();
        else if(E_step_cnt==0)
            Test_signal_data_nc(2,1);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(2,1);
            }
        }
        break;
    case 21:
        if(Pro_step_status==0xff)
        {
            ++Memu1_biaoye_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  BY2_addtion();
        break;
    case 22:
        if(E_step_cnt==7)
        {
            ++Memu1_biaoye_step_status;
            Pro_step_status=0;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_22();
        else if(E_step_cnt==0)
            Test_signal_data_nc(2,2);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(2,2);
            }
        }
        break;
    case 23:
        Memu1_biaoye_step_status=25;
        c2_prog_use=DJFFXND_calculat();		//计算电极法分析浓度
        Pro_step_status=0;
        break;
    case 24:
        Memu1_biaoye_step_status=25;
        c3_prog_use=DJFCLJG_calculat();		//计算电极法测量结果
        Pro_step_status=0;
        break;
    case 25:
        if(Pro_step_status==0xff)
        {
            Memu1_biaoye_step_status=0xff;
            Pro_step_status=0;
            E_step_cnt=0;
            Pro_dis_menu_flag=0;
            program_menu_select=0;

            Display_program_result(3);

            if(result_signe==2)
                yes_not=1;

            if((Memu1_auto_step_status==0)||(Memu1_auto_step_status==0xff))
            {
//    	          PWMB->PCR.CH0EN=1;		//en backlight
                Back_light_time_cnt=0;
                Back_light_time_flag=1;
                Prog_disp_flag=0;
                Memu1_biaoye_step_status=0;

                Get_write_struct_data(1,2,Pub_result_temp,M_menur1_unit_select_temp,yes_not,result_signe);	 //save data

                Menu_lever3_start_flag&=~BIT7;
                Write_system_reg_to_chip();
            }
            else
                Get_write_struct_data(1,4,Pub_result_temp,M_menur1_unit_select_temp,yes_not,result_signe);	 //save data

            Sub_adds=Write_result_to_flash(FX_write_index_adds_point,&Fenxi);
            FX_write_index_adds_point=Sub_adds;
            Write_index_adds_point();
            Display_last_result(FX_write_index_adds_point);
        }
        else  PaiKong_washing();			//排空清洗
        break;
//-------------------------------
    case 30:
        if(Pro_step_status==0xff)
        {
            ++Memu1_biaoye_step_status;
            Pro_step_status=0;
            E_step_cnt=0;
        }
        else  SJ2_addtion();
        break;
    case 31:
        if(Pro_step_status==0xff)
        {
            ++Memu1_biaoye_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ3_addtion();
        break;
    case 32:
        if(E_step_cnt==7)
        {
            ++Memu1_biaoye_step_status;
            Pro_step_status=0;
        }
        else  if(Pro_wait_time_flag==2)
            Get_test_signal_11();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,1);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,1);
            }
        }
        break;
    case 33:
        if(Pro_step_status==0xff)
        {
            ++Memu1_biaoye_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ4_addtion();
        break;
    case 34:
        if(E_step_cnt==7)
        {
            Memu1_biaoye_step_status=18;
            Pro_step_status=0;
        }
        else  if(Pro_wait_time_flag==2)
            Get_test_signal_12();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,2);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,2);
            }
        }
        break;
    }
}

//===============================================================================
//	finction	:Menu1_xishi_fenxi_finc
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
void Menu1_xishi_fenxi_finc(float c1_c2_tp)		//稀释分析流程控制函数
{
    uint8_t m_buff[8];
    volatile uint8_t i;
    volatile uint32_t Sub_adds;

    switch(Memu1_xishi_step_status)
    {
    case 0:
        Memu1_xishi_step_status=7;
        Pro_wait_time0_flag=0;
        Pro_step_status=0;
        yes_not=0;
        c1_prog_use=0;
        c2_prog_use=0;
        c3_prog_use=0;
        if(Prog_disp_flag==0)
            Prog_disp_flag=1;

        if(M_menur1_language_select_temp==2)
        {
            m_buff[0]=67;
            m_buff[1]=68;
            m_buff[2]=16;
            m_buff[3]=17;
            Display_program_5opration(4,m_buff);
        }
        else
        {
            m_buff[0]='A';
            m_buff[1]='U';
            m_buff[2]='T';
            m_buff[3]='O';
            m_buff[4]=0;
            m_buff[5]='D';
            m_buff[6]='I';
            m_buff[7]='L';
            Display_6x8_char(0,0,8,m_buff);		//line1
            for(i=0; i<8; i++)
                Get_6x8_char_single(0,i*6,m_buff[i]);
        }

        if(Prog_disp_flag==1)
        {
            Clear_n_page(4,4);
        }
        Set_back_light_10sec();
        Prog_total_step=Get_step_num_each_finc(3);
        break;
    case 7:
        if(Pro_step_status==0xff)
        {
            ++Memu1_xishi_step_status;
            Pro_step_status=0;
        }
        else  ZSB_initial();
        break;
    case 8:
        ++Memu1_xishi_step_status;
//    	    df_tp=Get_df_data(c1_c2_tp);
        break;
    case 9:
        ++Memu1_xishi_step_status;
        //纯水清洗
        break;
    case 10:
        if(Pro_step_status==0xff)
        {
            ++Memu1_xishi_step_status;
            Pro_step_status=0;
        }
        else  Add_xishi_sample(c1_c2_tp);
        break;
    case 11:
        if(Pro_step_status==0xff)
        {
            Pro_step_status=0;
            if(M_menur1_LZDJ_select_temp==2)			//比色法
            {
                i=M_menur1_sj3_v_temp;
                if(i==0)
                    //if(M_menur1_sj3_v_temp==0)
                {
                    Memu1_xishi_step_status=16;
                    E_step_cnt=0;

//                      Pro_wait_time_set=200;	//wait 2sec
                    Pro_wait_time_set=Get_e_wait_time();
                    Pro_wait_time_flag=1;
                }
                else
                {
                    i=M_menur1_sj4_v_temp;
                    if(i==0)
                        //if(M_menur1_sj4_v_temp==0)
                        ++Memu1_xishi_step_status;
                    else
                        Memu1_xishi_step_status=30;	//2012-9-5 10:05
                }
            }
            else if(M_menur1_LZDJ_select_temp==1)		//电极法
            {
                i=M_menur1_sj2_v_temp;
                if(i==0)
                    //if(M_menur1_sj2_v_temp==0)
                {
                    Memu1_xishi_step_status=21;
                    E_step_cnt=0;

//                      Pro_wait_time_set=200;	//wait 2sec
                    Pro_wait_time_set=Get_e_wait_time();
                    Pro_wait_time_flag=1;
                }
                else
                {
                    i=M_menur1_sj4_v_temp;
                    if(i==0)
                        //if(M_menur1_sj4_v_temp==0)
                        ++Memu1_xishi_step_status;
                    else
                        Memu1_xishi_step_status=30;	//2012-9-5 10:05
                }
            }
        }
        else  SJ1_addtion();
        break;
    case 12:
        if(Pro_step_status==0xff)
        {
            ++Memu1_xishi_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ2_addtion();
        break;
    case 13:
        if(E_step_cnt==7)
        {
            ++Memu1_xishi_step_status;
            Pro_step_status=0;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_11();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,1);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,1);
            }
        }
        break;
    case 14:
        if(Pro_step_status==0xff)
        {
            ++Memu1_xishi_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ3_addtion();
        break;
    case 15:
        if(E_step_cnt==7)
        {
            Memu1_xishi_step_status=19;
            Pro_step_status=0;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_12();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,2);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,2);
            }
        }
        break;
    case 16:
        if(E_step_cnt==7)
        {
            ++Memu1_xishi_step_status;
            Pro_step_status=0;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_11();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,1);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,1);
            }
        }
        break;
    case 17:
        if(Pro_step_status==0xff)
        {
            ++Memu1_xishi_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ2_addtion();
        break;
    case 18:
        if(E_step_cnt==7)
        {
            Memu1_xishi_step_status=19;
            Pro_step_status=0;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_12();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,2);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,2);
            }
        }
        break;
    case 19:
        c1_prog_use=BSFND_calculat();
        Pro_step_status=0;
        Memu1_xishi_step_status=26;
        break;
    case 20:
        if(Pro_step_status==0xff)
        {
            ++Memu1_xishi_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ2_addtion();
        break;
    case 21:
        if(E_step_cnt==7)
        {
            Pro_step_status=0;
            if(M_menur1_DL_select_temp==1)
                ++Memu1_xishi_step_status;
            else
                Memu1_xishi_step_status=25;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_21();
        else if(E_step_cnt==0)
            Test_signal_data_nc(2,1);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(2,1);
            }
        }
        break;
    case 22:
        if(Pro_step_status==0xff)
        {
            ++Memu1_xishi_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  BY2_addtion();
        break;
    case 23:
        if(E_step_cnt==7)
        {
            ++Memu1_xishi_step_status;
            Pro_step_status=0;
        }
        else if(Pro_wait_time_flag==2)
            Get_test_signal_22();
        else if(E_step_cnt==0)
            Test_signal_data_nc(2,2);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(2,2);
            }
        }
        break;
    case 24:
        Memu1_xishi_step_status=26;
        c2_prog_use=DJFFXND_calculat();		//计算电极法分析浓度
        Pro_step_status=0;
        break;
    case 25:
        Memu1_xishi_step_status=26;
        c3_prog_use=DJFCLJG_calculat();		//计算电极法测量结果
        Pro_step_status=0;
        break;
    case 26:
        if(Pro_step_status==0xff)
        {
            Memu1_xishi_step_status=0xff;
            Pro_step_status=0;
            E_step_cnt=0;
            Pro_dis_menu_flag=0;
            program_menu_select=0;

            Display_program_result(4);

            if(result_signe==2)
                yes_not=1;

            Get_write_struct_data(1,5,Pub_result_temp,M_menur1_unit_select_temp,yes_not,result_signe);	 //save data
            Sub_adds=Write_result_to_flash(FX_write_index_adds_point,&Fenxi);
            FX_write_index_adds_point=Sub_adds;
            Write_index_adds_point();
            Display_last_result(FX_write_index_adds_point);
            if((Memu1_auto_step_status==0)||(Memu1_auto_step_status==0xff))
            {
//    	          PWMB->PCR.CH0EN=1;		//en backlight
                Back_light_time_cnt=0;
                Back_light_time_flag=1;
                Prog_disp_flag=0;
                Memu1_xishi_step_status=0;
            }
        }
        else  PaiKong_washing();
        break;
//-------------------------------
    case 30:
        if(Pro_step_status==0xff)
        {
            ++Memu1_xishi_step_status;
            Pro_step_status=0;
            E_step_cnt=0;
        }
        else  SJ2_addtion();
        break;
    case 31:
        if(Pro_step_status==0xff)
        {
            ++Memu1_xishi_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ3_addtion();
        break;
    case 32:
        if(E_step_cnt==7)
        {
            ++Memu1_xishi_step_status;
            Pro_step_status=0;
        }
        else  if(Pro_wait_time_flag==2)
            Get_test_signal_11();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,1);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,1);
            }
        }
        break;
    case 33:
        if(Pro_step_status==0xff)
        {
            ++Memu1_xishi_step_status;
            Pro_step_status=0;
            E_step_cnt=0;

//               Pro_wait_time_set=200;	//wait 2sec
            Pro_wait_time_set=Get_e_wait_time();
            Pro_wait_time_flag=1;
        }
        else  SJ4_addtion();
        break;
    case 34:
        if(E_step_cnt==7)
        {
            Memu1_xishi_step_status=19;
            Pro_step_status=0;
        }
        else  if(Pro_wait_time_flag==2)
            Get_test_signal_12();
        else if(E_step_cnt==0)
            Test_signal_data_nc(1,2);

        if((E_step_cnt!=0)&&(E_step_cnt<6))
        {
            if(E_wait_flag==0)
            {
                Pro_wait_time_flag=2;
                Rx_TX_flag=0;
                Get_ebf6_data(1,2);
            }
        }
        break;
    }
}

//===============================================================================
//	finction	:Menu1_auto_fenxi_time_down_cnt
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
volatile uint8_t  Save_time_cnt_flag;
volatile uint16_t Save_time_cnt;

void Menu1_auto_fenxi_time_down_cnt(void)	//自动分析时间计时
{
    if(Pro_auto_time_dcnt>0)
    {
        --Pro_auto_time_dcnt;
        if(Pro_auto_time_dcnt==0)
            Pro_auto_time_up_check_flag=1;

        ++Save_time_cnt;
        if(Save_time_cnt==6000)
        {
            Save_time_cnt=0;
            Save_time_cnt_flag=1;
        }
    }
}

//===============================================================================
//	finction	:Get_auto_man_time
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-10-18 9:42
//	modefy		:null
//===============================================================================
void Get_auto_man_time(void)		//取自动分析的周期时间
{
    volatile  uint8_t i;

    Spi_read(FIRST_REG_PAGE,buff_page);

    buff_page[78]=Menu_lever3_start_flag;

    buff_page[79]=Pro_auto_time_dcnt>>24;
    buff_page[80]=Pro_auto_time_dcnt>>16;
    buff_page[81]=Pro_auto_time_dcnt>>8;
    buff_page[82]=Pro_auto_time_dcnt;

    for(i=0; i<13; i++)
        buff_page[97+i]=Wait_data_buff[i];

    Spi_write(FIRST_REG_PAGE,buff_page);
}

//===============================================================================
//	finction	:Get_next_man_clock
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-11-12 14:35
//	modefy		:null
//===============================================================================
void Get_next_man_clock(void)		//取下次分析的时间
{
    volatile  uint8_t i,j,mouth,dat,hour,minu,omouth,odat,ohour,ominu;
    volatile  uint32_t a,b,c;

    j=RTC_DATA.mouth_temp;
    j=j>>4;
    j&=0x01;
    Wait_data_buff[0]=j;
    j=RTC_DATA.mouth_temp;
    j&=0x0f;
    Wait_data_buff[1]=j;
    Wait_data_buff[2]=14;
    j=RTC_DATA.day_temp;
    j=j>>4;
    j&=0x03;
    Wait_data_buff[3]=j;
    j=RTC_DATA.day_temp;
    j&=0x0f;
    Wait_data_buff[4]=j;
    Wait_data_buff[5]=0;
    Wait_data_buff[6]=0;

    j=RTC_DATA.hour_temp;
    j=j>>4;
    j&=0x03;
    Wait_data_buff[7]=j;
    j=RTC_DATA.hour_temp;
    j&=0x0f;
    Wait_data_buff[8]=j;
    Wait_data_buff[9]=20;
    j=RTC_DATA.minu_temp;
    j=j>>4;
    j&=0x07;
    Wait_data_buff[10]=j;
    j=RTC_DATA.minu_temp;
    j&=0x0f;
    Wait_data_buff[11]=j;
//-------------------------------------------------------
    b=0;
    a=M_lever4_FXPL_temp>>12;
    a&=0x0000000f;
    a*=1000;
    b+=a;
    a=M_lever4_FXPL_temp>>8;
    a&=0x0000000f;
    a*=100;
    b+=a;
    a=M_lever4_FXPL_temp>>4;
    a&=0x0000000f;
    a*=10;
    b+=a;
    a=M_lever4_FXPL_temp;
    a&=0x0000000f;
    b+=a;

    a=b;
    dat=a/1440;
    c=b;
    a=dat*1440;
    a=c-a;
    hour=a/60;
    a=b;
    minu=a%60;

    mouth=0;
//-------------------------------------------------------
    ominu=Wait_data_buff[10]*10+Wait_data_buff[11];
    ohour=Wait_data_buff[7]*10+Wait_data_buff[8];
    odat=Wait_data_buff[3]*10+Wait_data_buff[4];
    omouth=Wait_data_buff[0]*10+Wait_data_buff[1];
//-------------------------------------------------------
    j=0;
    i=minu+ominu;
    if(i>=60)
    {
        minu=i-60;
        j=1;
    }
    else
        minu=i;

    i=hour+ohour+j;
    j=0;
    if(i>=24)
    {
        hour=i-24;
        j=1;
    }
    else
        hour=i;

    i=dat+odat+j;
    j=0;
    switch(omouth)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if(i>31)
        {
            dat=i-31;
            j=1;
        }
        else
            dat=i;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if(i>30)
        {
            dat=i-30;
            j=1;
        }
        else
            dat=i;
        break;
    case 2:
        if(i>28)
        {
            dat=i-28;
            j=1;
        }
        else
            dat=i;
        break;
    }
    i=mouth+omouth+j;
    if(i>12)
        mouth=i-12;
    else
        mouth=i;
//-----------------------------------
    j=mouth/10;
    Wait_data_buff[0]=Get_ascii_data(j);
    j=mouth%10;
    Wait_data_buff[1]=Get_ascii_data(j);
    j=dat/10;
    Wait_data_buff[3]=Get_ascii_data(j);
    j=dat%10;
    Wait_data_buff[4]=Get_ascii_data(j);

    j=hour/10;
    Wait_data_buff[7]=Get_ascii_data(j);
    j=hour%10;
    Wait_data_buff[8]=Get_ascii_data(j);
    j=minu/10;
    Wait_data_buff[10]=Get_ascii_data(j);
    j=minu%10;
    Wait_data_buff[11]=Get_ascii_data(j);
}

//===============================================================================
//	finction	:Menu1_auto_fenxi_finc
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-18 10:27
//	modefy		:null
//===============================================================================
volatile uint8_t aoto_paikong_flag;
volatile uint8_t Wait_clock_flag;
volatile uint8_t fix_repower_flag;

void Menu1_auto_fenxi_finc(void)		//自动分析流程控制函数
{
    volatile float k=0;
    volatile uint32_t i,j,Sub_adds,mq,mt;
    volatile uint16_t ai,aj;
    volatile uint8_t  m,n,kk;
    uint8_t  m_buff[16];

    j=0;

//save time
    if(Save_time_cnt_flag)
    {
        Save_time_cnt_flag=0;
        Get_auto_man_time();			//取自动分析时间
    }
    switch(Memu1_auto_step_status)
    {
    case 0:
        Led_data_flag|=BIT2;
        if(Pro_auto_time_dcnt==0)
        {
            if(Prog_disp_flag==0)
            {
                Prog_disp_flag=1;
                Clear_n_page_ram(0,7);
            }
            if(M_menur1_language_select_temp==2)
            {
                m_buff[0]=35;
                m_buff[1]=36;
                m_buff[2]=16;
                m_buff[3]=17;
                Display_program_5opration(4,m_buff);
            }
            else if(Prog_disp_flag==1)
            {
                Clear_n_page_ram(0,1);

                m_buff[0]='A';
                m_buff[1]='U';
                m_buff[2]='T';
                m_buff[3]='O';
                m_buff[4]=0;
                m_buff[5]='A';
                m_buff[6]='N';
                m_buff[7]='A';
/////            	   Display_6x8_char(0,0,8,m_buff);		//line1
                for(i=0; i<8; i++)
                    Get_6x8_char_single(0,i*6,m_buff[i]);

                for(i=0; i<128; i++)
                    Disp_buff128x64[128+i]|=0x10;

                Display_all_buff();
            }

            Pro_dis_menu_flag=0;
            aoto_paikong_flag=0;
            jiaozheng_sec_time_cnt=0;

            j=0;
            i=M_lever4_FXPL_temp>>12;		//取分析频率
            i&=0x0000000f;
            i*=1000;
            j+=i;
            i=M_lever4_FXPL_temp>>8;
            i&=0x0000000f;
            i*=100;
            j+=i;
            i=M_lever4_FXPL_temp>>4;
            i&=0x0000000f;
            i*=10;
            j+=i;
            i=M_lever4_FXPL_temp;
            i&=0x0000000f;
            j+=i;

            //i=j;
            j*=6000;

            j-=2470;

            Pro_auto_time_dcnt=j;    	    //start down cnt
            Pro_auto_time_up_check_flag=0;
        }
        Wait_clock_flag=0;
        Memu1_auto_step_status=7;
        Pro_step_status=0;
        Save_time_cnt_flag=Save_time_cnt=0;
        Measurement_signal_21=Measurement_signal_22=0;
        Set_back_light_10sec();
        Get_next_man_clock();				//取下次分析时间
        Get_auto_man_time();				//2012-10-18 9:43
//            Write_system_reg_to_chip();			//2012-10-17 14:41
        fix_repower_flag=0;
        break;
    case 7:				   //是否清洗
        aj=0;
        ai=M_lever4_QXPL_temp>>8;	   //取清洗频率
        ai&=0x000f;
        ai*=100;
        aj=ai;
        ai=M_lever4_QXPL_temp>>4;
        ai&=0x000f;
        ai*=10;
        aj+=ai;
        ai=M_lever4_QXPL_temp;
        ai&=0x000f;
        aj+=ai;

        if(M_lever4_QXPL_temp==0x00000000)
            Memu1_auto_step_status=9;
        else if(Pro_auto_qingxi_freq_cnt==aj)
        {
            Memu1_auto_step_status=8;
            Pro_step_status=0;
            Fa_clean_select_temp=clean_loop_cnt=0;
        }
        else
            Memu1_auto_step_status=9;
        break;
    case 8:		//清洗
        if(Pro_step_status==0xff)
        {
            ++Memu1_auto_step_status;
            Pro_step_status=0;
        }
        else  Clear_finction();	   //调清洗函数
        break;
    case 9:				   //是否校准
        yes_not=0;

        aj=0;
        ai=M_lever4_JZPL_temp>>8;
        ai&=0x000f;
        ai*=100;
        aj=ai;
        ai=M_lever4_JZPL_temp>>4;
        ai&=0x000f;
        ai*=10;
        aj+=ai;
        ai=M_lever4_JZPL_temp;
        ai&=0x000f;
        aj+=ai;

        if(M_lever4_JZPL_temp==0x00000000)		//不做校正
        {
            Memu1_auto_step_status=14;
            Memu1_hand_step_status=0;
        }
        else if(Pro_auto_jiaozheng_freq_cnt==aj)	//校准频率相等，下次调用校正分析函数
        {
            ++Memu1_auto_step_status;
            Memu1_jiaozheng_step_status=0;
        }
        else					//不做校正
        {
            Memu1_auto_step_status=14;
            Memu1_hand_step_status=0;
        }
        break;
    case 10:
        if(Memu1_jiaozheng_step_status==0xff)
        {
            ++jiaozheng_sec_time_cnt;
            ++Memu1_auto_step_status;
            Pro_step_status=0;
            Memu1_jiaozheng_step_status=0;
            Pro_auto_jiaozheng_freq_cnt=0;		//clear conter
            aoto_paikong_flag=1;
        }
        else  Menu1_hand_jiaozheng_finc();			//调用校正分析函数
        break;
    case 11:
        if(j_prog_use!=0)
        {
            if(M_menur1_analy_obj_select_temp==1)		//分析物是NH4-N ，判断校正值是否在范围内，做对应的控制
            {
                if((k2_prog_use>=-65)&&(k2_prog_use<=-50))
                    Memu1_auto_step_status=13;
                else
                {
                    Memu1_auto_step_status=12;
                    yes_not=1;
                }
            }
            if(M_menur1_analy_obj_select_temp==4)		//分析物是F ，判断校正值是否在范围内，做对应的控制
            {
                if((k2_prog_use>=50)&&(k2_prog_use<=65))
                    Memu1_auto_step_status=13;
                else
                {
                    Memu1_auto_step_status=12;
                    yes_not=1;
                }
            }
        }
        else if(k1_prog_use!=0)				//其他分析物 ，判断校正值是否在范围内，做对应的控制
        {
            if((k1_prog_use>=0.8)&&(k1_prog_use<=1.2))
                Memu1_auto_step_status=13;
            else
            {
                Memu1_auto_step_status=12;
                yes_not=1;
            }
        }
        else 		//all is 0
        {
            Memu1_auto_step_status=12;
            yes_not=1;
        }
        break;
    case 12:
        if(jiaozheng_sec_time_cnt!=2)		//每次校正都做2次
        {
            Memu1_auto_step_status=10;
            Memu1_hand_step_status=0;
            Pro_step_status=0;
        }
        else
        {
            Memu1_auto_step_status=13;
            jiaozheng_sec_time_cnt=0;
        }
        break;
    case 13:
        ++Memu1_auto_step_status;
        Memu1_hand_step_status=0;
        Pro_step_status=0;

//    	    Get_write_struct_data(2,3,Pub_result_temp,4,yes_not,result_signe);	 //save data
//    	    Sub_adds=Write_result_to_flash(JZ_write_index_adds_point,&Jiaozheng);
//    	    JZ_write_index_adds_point=Sub_adds;
//    	    Write_index_adds_point();

        yes_not=0;
        break;
    case 14:
        if(Memu1_hand_step_status==0xff)
        {
            ++Memu1_auto_step_status;
            Pro_step_status=0;
            Memu1_hand_step_status=0;
            ++Pro_auto_jiaozheng_freq_cnt;		//inc conter
            ++Pro_auto_qingxi_freq_cnt;
            ++Pro_auto_hecha_freq_cnt;
            aoto_paikong_flag=1;
        }
        else  Menu1_hand_fenxi_finc();		//调用手动分析流程
        break;
    case 15:
        pr_data=0;

        pi=M_menur1_lange_data_temp;        	//取量程值
        pi>>=16;
        pi&=0x0000000f;
        pk=pi;
        pk=pk*1000;
        pr_data+=pk;

        pi=M_menur1_lange_data_temp;
        pi>>=12;
        pi&=0x0000000f;
        pk=pi;
        pk=pk*100;
        pr_data+=pk;

        pi=M_menur1_lange_data_temp;
        pi>>=8;
        pi&=0x0000000f;
        pk=pi;
        pk=pk*10;
        pr_data+=pk;

        pi=M_menur1_lange_data_temp;
        pi>>=4;
        pi&=0x0000000f;
        pk=pi;
        pr_data+=pk;

        pi=M_menur1_lange_data_temp;
        pi&=0x0000000f;
        pk=pi;
        pk=pk/10.0;
        pr_data+=pk;

        pr_data*=1.2;			//2012-9-13 10:52 sam

        if(c1_prog_use!=0)			//分析结果与量程比较，然后做对应的控制，如果超标，判断是否要做稀释分析
        {
            if(c1_prog_use>pr_data)
                ++Memu1_auto_step_status;
            else
                Memu1_auto_step_status=18;
        }
        else if(c2_prog_use!=0)
        {
            if(c2_prog_use>pr_data)
                ++Memu1_auto_step_status;
            else
                Memu1_auto_step_status=18;
        }
        else if(c3_prog_use!=0)
        {
            if(c3_prog_use>pr_data)
                ++Memu1_auto_step_status;
            else
                Memu1_auto_step_status=18;
        }
        else
            Memu1_auto_step_status=18;
        break;
    case 16:
        if(M_menur1_ZDXS_select_temp==1)
        {
            ++Memu1_auto_step_status;
            Memu1_xishi_step_status=0;

            if(c1_prog_use!=0)
                c1_c2_xi=c1_prog_use;
            else if(c2_prog_use!=0)
//    	           c1_c2_xi=c2_prog_use;  		//2012-10-12 14:22
                c1_c2_xi=c3_prog_use;  		//2012-10-12 14:22
            else if(c3_prog_use!=0)
                c1_c2_xi=c3_prog_use;  		//2012-10-12 14:22
        }
        else
            Memu1_auto_step_status=18;
        break;
    case 17:
        if(Memu1_xishi_step_status==0xff)
        {
            ++Memu1_auto_step_status;
            Pro_step_status=0;
            Memu1_xishi_step_status=0;
            aoto_paikong_flag=1;
        }
        else
            Menu1_xishi_fenxi_finc(c1_c2_xi);	//调用稀释分析流程函数
        break;
    case 18:
        ++Memu1_auto_step_status;

//    	    Get_write_struct_data(1,1,Pub_result_temp,M_menur1_unit_select_temp,yes_not,result_signe);	 //save data
//    	    Sub_adds=Write_result_to_flash(FX_write_index_adds_point,&Fenxi);
//    	    FX_write_index_adds_point=Sub_adds;
//    	    Write_index_adds_point();
        break;
    case 19:			//标液核查
        aj=0;
        ai=M_lever4_HCPL_temp>>8;	//取核查频率值
        ai&=0x000f;
        ai*=100;
        aj=ai;
        ai=M_lever4_HCPL_temp>>4;
        ai&=0x000f;
        ai*=10;
        aj+=ai;
        ai=M_lever4_HCPL_temp;
        ai&=0x000f;
        aj+=ai;
        if(M_lever4_HCPL_temp==0x00000000)
            Memu1_auto_step_status=24;

        else if(Pro_auto_hecha_freq_cnt==aj)	//核查频率相等，则判断是否达到核查的条件
        {
            jiaozheng_sec_time_cnt=0;
            pr_data=0;

            pi=M_lever3_HCPD_temp;
            pi>>=16;
            pi&=0x0000000f;
            pk=pi;
            pk=pk*100.0;
            pr_data+=pk;

            pi=M_lever3_HCPD_temp;
            pi>>=12;
            pi&=0x0000000f;
            pk=pi;
            pk=pk*10.0;
            pr_data+=pk;

            pi=M_lever3_HCPD_temp;
            pi>>=8;
            pi&=0x0000000f;
            pk=pi;
            pr_data+=pk;

            pi=M_lever3_HCPD_temp;
            pi>>=4;
            pi&=0x0000000f;
            pk=pi;
            pk=pk/10.0;
            pr_data+=pk;

            pi=M_lever3_HCPD_temp;
            pi&=0x0000000f;
            pk=pi;
            pk=pk/100.0;
            pr_data+=pk;


            if(c1_prog_use!=0)
            {
                if(c1_prog_use>pr_data)
                {
                    ++Memu1_auto_step_status;
                    Memu1_biaoye_step_status=0;
                }
                else
                    Memu1_auto_step_status=24;
            }
            else if(c2_prog_use!=0)
            {
                if(c2_prog_use>pr_data)
                {
                    ++Memu1_auto_step_status;
                    Memu1_biaoye_step_status=0;
                }
                else
                    Memu1_auto_step_status=24;
            }
            else if(c3_prog_use!=0)
            {
                if(c3_prog_use>pr_data)
                {
                    ++Memu1_auto_step_status;
                    Memu1_biaoye_step_status=0;
                }
                else
                    Memu1_auto_step_status=24;
            }
            else
            {
                ++Memu1_auto_step_status;
                Memu1_biaoye_step_status=0;
            }
        }
        else Memu1_auto_step_status=24;
        break;
    case 20:
        if(Memu1_biaoye_step_status==0xff)
        {
            ++Memu1_auto_step_status;
            ++jiaozheng_sec_time_cnt;
            Pro_step_status=0;
            Memu1_biaoye_step_status=0;
            aoto_paikong_flag=1;
        }
        else  Menu1_biaoye_hecha_finc();		//调用核查流程控制函数
        break;
    case 21:
        k=pr_data=0;

        pi=M_lever3_HCPD_temp;
        pi>>=8;
        pi&=0x0000000f;
        pk=pi;
        pk=pk*100.0;
        pr_data+=pk;

        pi=M_lever3_HCPD_temp;
        pi>>=4;
        pi&=0x0000000f;
        pk=pi;
        pk=pk*10.0;
        pr_data+=pk;

        pi=M_lever3_HCPD_temp;
        pi&=0x0000000f;
        pk=pi;
        pr_data+=pk;
//------------------------------------------
        if(Allow_error!=0)
        {
            pi=Allow_error;
            pi>>=12;
            pi&=0x0000000f;
            pk=pi;
            pk=pk*100.0;
            k+=pk;

            pi=Allow_error;
            pi>>=8;
            pi&=0x0000000f;
            pk=pi;
            pk=pk*10.0;
            k+=pk;

            pi=Allow_error;
            pi>>=4;
            pi&=0x0000000f;
            pk=pi;
            k+=pk;

            pi=Allow_error;
            pi&=0x0000000f;
            pk=pi;
            pk/=10.0;
            k+=pk;
            k/=100.0;
            k=k*pr_data;
        }
        else
            k=0;
//-----------------------------------------

        if(c1_prog_use!=0)
            pk=c1_prog_use;
        else if(c2_prog_use!=0)
            pk=c2_prog_use;
        else if(c3_prog_use!=0)
            pk=c3_prog_use;

//            pr_data=pk/pr_data;

//            if((pr_data>=0.95)&&(pr_data<=1.05))
        if((pk>=(pr_data-k))&&(pk<=(pr_data+k)))
        {
            Memu1_auto_step_status=23;
            Pro_auto_hecha_freq_cnt=0;
        }
        else Memu1_auto_step_status=22;
        break;
    case 22:
        if(jiaozheng_sec_time_cnt==2)
        {
            ++Memu1_auto_step_status;
            yes_not=1;
            Pro_auto_hecha_freq_cnt=0;
        }
        else
        {
            Memu1_auto_step_status=20;
            Memu1_biaoye_step_status=0;
        }
        break;
    case 23:
        ++Memu1_auto_step_status;
        Pro_step_status=0;
        E_step_cnt=0;

        yes_not=0;
        break;
    case 24:
        if(Pro_step_status==0xff)
        {
            U2_disple_flag=0;
            ++Memu1_auto_step_status;
            Pro_step_status=0;
            E_step_cnt=0;
            aoto_paikong_flag=0;
//               //-----------------------------------------    //结束消解
//               if(Disple_flag==1)
//               {
//	           Reset_init_UART1(1);
//	           DrvSYS_Delay(200000);
//	           U2_disple_flag=0;
//	           Set_tx_cmd_data_finc(Machine_note_use,0x65,0x0000,0x0000,0x00);
//	           //DrvSYS_Delay(200000);
//	           //Set_tx_cmd_data_finc(Machine_note_use,0x65,0x0100,0x0000,0x00);
//	           U_Rx_wait_time_flag_temp=0;
//
//	           Memu1_auto_step_status=35;
//	           Rx_TX_flag=0;
//                   Pro_wait_time_set=42000;	//wait 420sec
//                   Pro_wait_time_flag=1;
//	       }
/////    	       Display_program_result(0);

            if(Alarm_low_nd!=0)		//分析结果判断是否低于报警低浓度值
            {
                pi=Alarm_low_nd;
                pi>>=16;
                pi&=0x0000000f;
                pk=pi;
                pk=pk*1000.0;
                k+=pk;

                pi=Alarm_low_nd;
                pi>>=12;
                pi&=0x0000000f;
                pk=pi;
                pk=pk*100.0;
                k+=pk;

                pi=Alarm_low_nd;
                pi>>=8;
                pi&=0x0000000f;
                pk=pi;
                pk=pk*10.0;
                k+=pk;

                pi=Alarm_low_nd;
                pi>>=4;
                pi&=0x0000000f;
                pk=pi;
                k+=pk;

                pi=Alarm_low_nd;
                pi&=0x0000000f;
                pk=pi;
                pk/=10.0;
                k+=pk;
                pr_data=k;
            }
            else
                pr_data=0;
//----------------------------------------------------
            if(Alarm_hight_nd!=0)		//分析结果判断是否高于报警高浓度值
            {
                pi=Alarm_hight_nd;
                pi>>=16;
                pi&=0x0000000f;
                pk=pi;
                pk=pk*1000.0;
                k+=pk;

                pi=Alarm_hight_nd;
                pi>>=12;
                pi&=0x0000000f;
                pk=pi;
                pk=pk*100.0;
                k+=pk;

                pi=Alarm_hight_nd;
                pi>>=8;
                pi&=0x0000000f;
                pk=pi;
                pk=pk*10.0;
                k+=pk;

                pi=Alarm_hight_nd;
                pi>>=4;
                pi&=0x0000000f;
                pk=pi;
                k+=pk;

                pi=Alarm_hight_nd;
                pi&=0x0000000f;
                pk=pi;
                pk/=10.0;
                k+=pk;
            }
            else
                k=0;
//----------------------------------------------------
            if(c1_prog_use!=0)
                pk=c1_prog_use;
            else if(c2_prog_use!=0)
                pk=c2_prog_use;
            else if(c3_prog_use!=0)
                pk=c3_prog_use;
            else
                pk=0;

            if(pk>=0)
                result_signe=0;
            else
                result_signe=2;

            if((pk>pr_data)&&(pk<k))		//结果在范围内
            {
//               	   DrvGPIO_SetBit (E_GPC,14);		//close alarm led
                Led_data_flag&=~BIT1;
                Set_switch_open_close(13,0);
                DrvSYS_Delay(600000);
                Set_switch_open_close(14,0);
                Alarm_active_flag=0;

//                   Pro_wait_time_set=1000;	//wait 10sec
//                   Pro_wait_time_flag=1;
            }
            else if(pk<pr_data)		//结果小于范围内
            {
//               	   DrvGPIO_ClrBit (E_GPC,14);		//open alarm led
                Led_data_flag|=BIT1;
                Set_switch_open_close(13,1);
                Alarm_active_flag=1;
                Set_en_buzz_finc(4,4,200,100);

                Clear_n_page(0,7);
                m_buff[0]='E';
                m_buff[1]='R';
                m_buff[2]='R';
                m_buff[3]=0;
                m_buff[4]='A';
                m_buff[5]='L';
                m_buff[6]='A';
                m_buff[7]='R';
                m_buff[8]='M';
                m_buff[9]=0;
                m_buff[10]='L';
                Display_8x16_char(3,8,11,m_buff);

                Get_write_struct_data(3,4,Pub_result_temp,M_menur1_unit_select_temp,1,result_signe);	 //save data
                Sub_adds=Write_result_to_flash(BJ_write_index_adds_point,&Baojing);
                BJ_write_index_adds_point=Sub_adds;
                Write_index_adds_point();

//                   Pro_wait_time_set=1000;	//wait 10sec
//                   Pro_wait_time_flag=1;
            }
            else if(pk>k)			//结果大于范围内
            {
//               	   DrvGPIO_ClrBit (E_GPC,14);		//open alarm led
                Led_data_flag|=BIT1;
                Set_switch_open_close(14,1);
                Alarm_active_flag=1;
                Set_en_buzz_finc(4,4,200,100);

                Clear_n_page(0,7);
                m_buff[0]='E';
                m_buff[1]='R';
                m_buff[2]='R';
                m_buff[3]=0;
                m_buff[4]='A';
                m_buff[5]='L';
                m_buff[6]='A';
                m_buff[7]='R';
                m_buff[8]='M';
                m_buff[9]=0;
                m_buff[10]='H';
                Display_6x8_char(3,8,11,m_buff);

                Get_write_struct_data(3,4,Pub_result_temp,M_menur1_unit_select_temp,1,result_signe);	 //save data
                Sub_adds=Write_result_to_flash(BJ_write_index_adds_point,&Baojing);
                BJ_write_index_adds_point=Sub_adds;
                Write_index_adds_point();

//                   Pro_wait_time_set=1000;	//wait 10sec
//                   Pro_wait_time_flag=1;
            }
            Pro_wait_time_set=1000;	//wait 10sec
            Pro_wait_time_flag=1;

            PWMB->PCR.CH0EN=1;		//en backlight
            Back_light_time_cnt=0;
            Back_light_time_flag=1;

            if(Prog_disp_flag==2)
                Display_last_result(FX_write_index_adds_point);

            program_menu_select=0;
            Measurement_signal_11=Measurement_signal_12=Measurement_signal_21=Measurement_signal_22=0;
//-------------------------------------------output pwm result
            if(M_lever3_BYHC_temp!=1) 		//no check
                Pwm_out_adj_flag=2;
            else
            {
                Pwm_get_resul_from_flash(1);		//status !=2,4
                Pwm_out_adj_flag=2;
            }
        }
        else if(aoto_paikong_flag==1) 	//已经做过排空清洗
            Pro_step_status=0xff;
        else
            PaiKong_washing();		//调用排空清洗
        break;
    case 25:
        if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
        {
            if((Pro_auto_time_dcnt==0)&&(Pro_auto_time_up_check_flag==1))		//检查分析时间，如没到则显示下次分析时间等待
            {
                Memu1_auto_step_status=0;
            }
            else
            {
                if(Prog_disp_flag==1)
                {
                    if(Wait_clock_flag==0)
                    {
                        Wait_clock_flag=1;
                        Clear_n_page(0,7);
                        if(M_menur1_language_select_temp==2)
                        {
                            Get_16x16_char_single(2,6,206);
                            Get_16x16_char_single(2,6+12,118);
                            Get_16x16_char_single(2,6+24,16);
                            Get_16x16_char_single(2,6+36,17);
                            Get_16x16_char_single(2,6+48,52);
                            Get_16x16_char_single(2,6+60,53);
                            Display_all_buff();
                        }
                        else
                        {
                            m_buff[0]='N';
                            m_buff[1]='E';
                            m_buff[2]='X';
                            m_buff[3]='T';
                            m_buff[4]=0;
                            m_buff[5]='A';
                            m_buff[6]='N';
                            m_buff[7]='A';
                            m_buff[8]=19;
                            m_buff[9]=0;
                            m_buff[10]='T';
                            m_buff[11]='I';
                            m_buff[12]='M';
                            m_buff[13]='E';
                            m_buff[14]=20;
                            Display_6x8_char(2,6,15,m_buff);
                        }
                        Display_6x8_char(4,24,12,Wait_data_buff);
                    }
                }

                if(fix_repower_flag==1)
                {
                    i=RTC_DATA.mouth_temp;
                    i&=0x1f;
                    m=Wait_data_buff[0]-0x30;
                    m<<=4;
                    m|=Wait_data_buff[1]-0x30;
                    if(m<i)
                    {
                        Auto_check_clock_flag=0;
                        Pro_auto_time_dcnt=0;
                        Pro_dis_menu_flag=0;
                        Memu1_auto_step_status=0;
                        Menu1_auto_fenxi_finc();
                    }
                    else if(m==i)
                    {
                        i=RTC_DATA.day_temp;
                        i&=0x3f;
                        m=Wait_data_buff[3]-0x30;
                        m<<=4;
                        m|=Wait_data_buff[4]-0x30;
                        if(m<i)
                        {
                            Auto_check_clock_flag=0;
                            Pro_auto_time_dcnt=0;
                            Pro_dis_menu_flag=0;
                            Memu1_auto_step_status=0;
                            Menu1_auto_fenxi_finc();
                        }
                        else if(m==i)
                        {
                            i=RTC_DATA.hour_temp;
                            i&=0x3f;
                            m=Wait_data_buff[7]-0x30;
                            m<<=4;
                            m|=Wait_data_buff[8]-0x30;
                            if(m<=i)
                            {
                                Auto_check_clock_flag=0;
                                Pro_auto_time_dcnt=0;
                                Pro_dis_menu_flag=0;
                                Memu1_auto_step_status=0;
                                Menu1_auto_fenxi_finc();
                            }
                        }
                    }
                }
            }
        }
        break;
    case 30:		//断电重新启动后执行排空清洗然后再判断时间是否开始分析。
        Prog_disp_flag=1;
        Clear_n_page_ram(0,7);
        Memu1_auto_step_status=0;
        if(M_menur1_language_select_temp==2)
        {
            m_buff[0]=35;
            m_buff[1]=36;
            m_buff[2]=16;
            m_buff[3]=17;
            Display_program_5opration(4,m_buff);
        }
        else
        {
            m_buff[0]='A';
            m_buff[1]='U';
            m_buff[2]='T';
            m_buff[3]='O';
            m_buff[4]=0;
            m_buff[5]='A';
            m_buff[6]='N';
            m_buff[7]='A';
            for(i=0; i<8; i++)
                Get_6x8_char_single(0,i*6,m_buff[i]);		//line1
            for(i=0; i<128; i++)
            {
                if(M_menur1_language_select_temp==2)
                    Disp_buff128x64[128+i]|=0x80;
                else
                    Disp_buff128x64[128+i]=0x10;
            }
            Display_all_buff();
        }
        Memu1_auto_step_status=31;

        Pro_step_status=0;
        Prog_total_step=8;
        Prog_step_disp_cnt=7;
        Set_back_light_10sec();
        break;
    case 31:
        if(Pro_step_status==0xff)
        {
            kk=0;
            n=RTC_DATA.mouth_temp;
            n&=0x1f;
            m=Wait_data_buff[0]-0x30;
            m<<=4;
            m|=Wait_data_buff[1]-0x30;
            if(m==n)
            {
                n=RTC_DATA.day_temp;
                n&=0x3f;
                m=Wait_data_buff[3]-0x30;
                m<<=4;
                m|=Wait_data_buff[4]-0x30;
                if(m==n)
                {
                    n=RTC_DATA.hour_temp;
                    n&=0x3f;
                    m=Wait_data_buff[7]-0x30;
                    m<<=4;
                    m|=Wait_data_buff[8]-0x30;
                    if(m==n)
                    {
                        n=RTC_DATA.minu_temp;
                        n&=0x7f;
                        m=Wait_data_buff[10]-0x30;
                        m<<=4;
                        m|=Wait_data_buff[11]-0x30;
                        if(m>=n)
                        {
fix_auto_power:
                            kk=1;
                            Auto_check_clock_flag=0;
                            Pro_auto_time_dcnt=0;
                            Pro_dis_menu_flag=0;
                            Memu1_auto_step_status=0;
                            Menu1_auto_fenxi_finc();
                        }
                        j=mq=mt=0;
                        i=RTC_DATA.minu_temp>>4;
                        i&=0x00000007;
                        i*=10;
                        j+=i;
                        i=RTC_DATA.minu_temp;
                        i&=0x0000000f;
                        j+=i;
                        mt=j;

                        j=0;
                        i=M_lever4_start_mini_temp>>4;
                        i&=0x00000007;
                        i*=10;
                        j+=i;
                        i=M_lever4_start_mini_temp;
                        i&=0x0000000f;
                        j+=i;
                        mq=j;

                        j=0;
                        i=M_lever4_FXPL_temp>>12;		//取分析频率
                        i&=0x0000000f;
                        i*=1000;
                        j+=i;
                        i=M_lever4_FXPL_temp>>8;
                        i&=0x0000000f;
                        i*=100;
                        j+=i;
                        i=M_lever4_FXPL_temp>>4;
                        i&=0x0000000f;
                        i*=10;
                        j+=i;
                        i=M_lever4_FXPL_temp;
                        i&=0x0000000f;
                        j+=i;

                        if((RTC_DATA.minu_temp)>M_lever4_start_mini_temp)
                        {
                            mq=mt-mq;
                            j-=mq;
                        }
                        else
                        {
                            mq=mq-mt;
                            j+=mq;
                        }
                        // j=60-j;
                        j*=6000;
                        j-=2470;

                        Pro_auto_time_dcnt=0;
                        Pro_auto_time_dcnt+=j;    	    //start down cnt
                        Pro_auto_time_up_check_flag=0;
//----------------------------------------------------------------------------------------------
                        j=0;
                        i=M_lever4_start_mini_temp>>4;
                        i&=0x00000007;
                        i*=10;
                        j+=i;
                        i=M_lever4_start_mini_temp;
                        i&=0x0000000f;
                        j+=i;
                        mq=j;

                        mt=j=0;
                        i=M_lever4_FXPL_temp>>12;		//取分析频率
                        i&=0x0000000f;
                        i*=1000;
                        j+=i;
                        i=M_lever4_FXPL_temp>>8;
                        i&=0x0000000f;
                        i*=100;
                        j+=i;
                        i=M_lever4_FXPL_temp>>4;
                        i&=0x0000000f;
                        i*=10;
                        j+=i;
                        i=M_lever4_FXPL_temp;
                        i&=0x0000000f;
                        j+=i;

                        j+=mq;

                        mt=j;
                        mt%=60;

                        i=mt;
                        i/=10;
                        Wait_data_buff[10]=Get_ascii_data(i);
                        i=mt;
                        i%=10;
                        Wait_data_buff[11]=Get_ascii_data(i);	//mini

                        mt=j;
                        mt/=60;

                        i=RTC_DATA.hour_temp>>4;
                        i&=0x03;
                        i*=10;
                        j=RTC_DATA.hour_temp;
                        j&=0x0f;
                        i+=j;

                        i+=mt;

                        mt=i;
                        mt%=24;
                        j=mt;
                        j/=10;
                        Wait_data_buff[7]=Get_ascii_data(j);
                        j=mt;
                        j%=10;
                        Wait_data_buff[8]=Get_ascii_data(j);	//hout

                        mt=i;
                        mt/=24;

                        i=RTC_DATA.day_temp>>4;
                        i&=0x03;
                        i*=10;
                        j=RTC_DATA.day_temp;
                        j&=0x0f;
                        i+=j;
                        mt+=i;

                        i=RTC_DATA.mouth_temp>>4;
                        i&=0x03;
                        i*=10;
                        j=RTC_DATA.mouth_temp;
                        j&=0x0f;
                        i+=j;

                        switch(i)
                        {
                        case 2:
                            if(mt>28)
                            {
                                mt-=28;
                                ++i;
                                if(i>12) i=1;
                            }
                            break;
                        case 1:
                        case 3:
                        case 5:
                        case 7:
                        case 8:
                        case 10:
                        case 12:
                            if(mt>31)
                            {
                                mt-=31;
                                ++i;
                                if(i>12) i=1;
                            }
                            break;
                        case 4:
                        case 6:
                        case 9:
                        case 11:
                            if(mt>30)
                            {
                                mt-=30;
                                ++i;
                                if(i>12) i=1;
                            }
                            break;
                        }

                        j=i;
                        j/=10;
                        Wait_data_buff[0]=Get_ascii_data(j);
                        j=i;
                        j%=10;
                        Wait_data_buff[1]=Get_ascii_data(j);

                        j=mt;
                        j/=10;
                        Wait_data_buff[3]=Get_ascii_data(j);
                        j=mt;
                        j%=10;
                        Wait_data_buff[4]=Get_ascii_data(j);
                    }
                    else goto fix_auto_power;
                }
                else goto fix_auto_power;
            }
            else goto fix_auto_power;

            if(kk==0)
            {
                Memu1_auto_step_status=25;
                if(Pro_auto_time_dcnt==0) Pro_auto_time_up_check_flag=1;

                Auto_check_clock_flag=0;
                Pro_dis_menu_flag=0;

            }

            PWMB->PCR.CH0EN=1;		//en backlight
            Back_light_time_cnt=0;
            Back_light_time_flag=1;
        }
        else
            PaiKong_washing();
        break;
//    	case 35:
//    	    if(U2_disple_flag==1)
//    	    {
//    	    	U2_disple_flag=0;
//    	    	++Memu1_auto_step_status;
//    	    }
//    	    else if(Pro_wait_time_set<=41850)
//    	    {
//    	    	Set_tx_cmd_data_finc(Machine_note_use,0x65,0x0000,0x0000,0x00);
//                U_Rx_wait_time_flag_temp=0;
//	        Rx_TX_flag=0;
//	        DrvSYS_Delay(200000);
//    	    }
//    	break;
//    	case 36:
//    	    if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
//    	    {
//    	        Reset_init_UART1(0);
//    	        Memu1_auto_step_status=25;
//    	    }
//    	break;
    }
}

//===============================================================================
//	finction	:All_program_menu_disp
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.06
//	modefy		:null
//===============================================================================
void All_program_menu_disp(uint8_t selest)		//显示所有的分析步骤函数
{
    uint16_t j;
    uint8_t i,k,data_buff[8];
    uint8_t const *Adds_index;
    uint8_t const *index;

    for(i=0; i<8; i++)
        data_buff[i]=0;

    if(Key_code_bak!=KEY_CODE_ESC)
        ++Prog_step_disp_cnt;
    if((selest==19)&&(Key_code_bak!=KEY_CODE_ESC))
        --Prog_step_disp_cnt;

    program_menu_select=selest;
    if((M_menur1_language_select_temp==2)&&(Prog_disp_flag!=2))
    {
/////       Clear_n_page_ram(2,2);
        switch(program_menu_select)
        {
        case 1:	//ZSB_initial
            data_buff[0]=73;
            data_buff[1]=74;
            data_buff[2]=75;
            data_buff[3]=203;
            data_buff[4]=15;
            data_buff[5]=204;
            i=6;
            break;
        case 2:	//BY1_wash
            data_buff[0]=41;
            data_buff[1]=42;
            data_buff[2]=221;
            data_buff[3]=56;
            data_buff[4]=51;
            i=5;
            break;
        case 3:	//BY1_addtion
            data_buff[0]=154;
            data_buff[1]=41;
            data_buff[2]=42;
            data_buff[3]=221;
            i=4;
            break;
        case 4:	//BY2_addtion
            data_buff[0]=154;
            data_buff[1]=41;
            data_buff[2]=42;
            data_buff[3]=222;
            i=4;
            break;
        case 5:	//SJ1_addtion
            data_buff[0]=154;
            data_buff[1]=109;
            data_buff[2]=110;
            data_buff[3]=221;
            i=4;
            break;
        case 6:	//SJ2_addtion
            data_buff[0]=154;
            data_buff[1]=109;
            data_buff[2]=110;
            data_buff[3]=222;
            i=4;
            break;
        case 7:	//SJ3_addtion
            data_buff[0]=154;
            data_buff[1]=109;
            data_buff[2]=110;
            data_buff[3]=223;
            i=4;
            break;
        case 8:	//SJ4_addtion
            data_buff[0]=154;
            data_buff[1]=109;
            data_buff[2]=110;
            data_buff[3]=224;
            i=4;
            break;
        case 9:	    //Get_test_signal_11
            data_buff[0]=63;
            data_buff[1]=46;
            data_buff[2]=57;
            data_buff[3]=58;
            data_buff[4]=221;
            data_buff[5]=221;
            i=6;
            break;
        case 10:  //Get_test_signal_12
            data_buff[0]=63;
            data_buff[1]=46;
            data_buff[2]=57;
            data_buff[3]=58;
            data_buff[4]=221;
            data_buff[5]=222;
            i=6;
            break;
        case 11:  //Get_test_signal_21
            data_buff[0]=63;
            data_buff[1]=46;
            data_buff[2]=57;
            data_buff[3]=58;
            data_buff[4]=222;
            data_buff[5]=221;
            i=6;
            break;
        case 12:  //Get_test_signal_22
            data_buff[0]=63;
            data_buff[1]=46;
            data_buff[2]=57;
            data_buff[3]=58;
            data_buff[4]=222;
            data_buff[5]=222;
            i=6;
            break;
        case 13:  //PaiKong_washing
            data_buff[0]=201;
            data_buff[1]=202;
            data_buff[2]=97;
            data_buff[3]=51;
            i=4;
            break;
        case 14:  //Sample_washing
            data_buff[0]=55;
            data_buff[1]=108;
            data_buff[2]=56;
            data_buff[3]=51;
            i=4;
            break;
        case 15:  //Sample_adding
            data_buff[0]=154;
            data_buff[1]=55;
            data_buff[2]=108;
            i=3;
            break;
        case 16:  //Add_xishi_sample
            data_buff[0]=154;
            data_buff[1]=67;
            data_buff[2]=68;
            data_buff[3]=55;
            data_buff[4]=108;
            i=5;
            break;
        case 17:  //Clear
            data_buff[0]=97;
            data_buff[1]=51;
            i=2;
            break;
        case 18:  //triggle get sample
            data_buff[0]=177;
            data_buff[1]=178;
            data_buff[2]=54;
            data_buff[3]=55;
            i=4;
            break;

        case 19:  //disple
            data_buff[0]=207;
            data_buff[1]=208;
            data_buff[2]=142;
            data_buff[3]=0;
            i=3;
            break;
        }
        if(program_menu_select!=0)
            Display_program_menu(i,data_buff);
//       else if((program_menu_select==0)&&(Menu_lever3_start_flag==BIT4))
//          Display_program_result(1);
    }
    else	if(Prog_disp_flag!=2)	//eng
    {
        Clear_n_page(2,0);

        j=(program_menu_select-1)*16;
        index= Prog_en_menu_index+(program_menu_select-1);			//char num.
        for(k=0; k<*index; k++)
        {
            Adds_index=Prog_en_menu_char+j;				//char data
/////           Display_8x16_char_single(2,k*8,*Adds_index);
            Get_6x8_char_single(2,6+k*6,*Adds_index);
            ++j;
        }
    }
    for(i=0; i<8; i++)
        data_buff[i]=0;

    if(Prog_step_disp_cnt<10)
    {
        data_buff[0]=0;
        data_buff[1]=Get_ascii_data(Prog_step_disp_cnt);
    }
    else
    {
        data_buff[0]=Prog_step_disp_cnt/10;
        data_buff[0]=Get_ascii_data(data_buff[0]);
        data_buff[1]=Prog_step_disp_cnt%10;
        data_buff[1]=Get_ascii_data(data_buff[1]);
    }
    data_buff[2]=18;
    if(Prog_total_step<10)
    {
        data_buff[4]=0;
        data_buff[3]=Get_ascii_data(Prog_total_step);
    }
    else
    {
        data_buff[3]=Prog_total_step/10;
        data_buff[3]=Get_ascii_data(data_buff[3]);
        data_buff[4]=Prog_total_step%10;
        data_buff[4]=Get_ascii_data(data_buff[4]);
    }
    if((Menu_lever3_start_flag==BIT4)&&(Prog_disp_flag!=2))
    {
        for(i=5; i>0; i--)
        {
            data_buff[i]=data_buff[i-1];
        }
        data_buff[0]=0;
        switch(Memu1_auto_step_status)
        {
        case 10:
            data_buff[0]='C';

            break;
        case 14:
            data_buff[0]='A';
            break;
        case 17:
            data_buff[0]='D';
            break;
        case 20:
            data_buff[0]='S';
            break;
        }
        for(i=0; i<6; i++)
            Get_6x8_char_single(0,80+i*6,data_buff[i]);				//line1
    }
    else if(Prog_disp_flag!=2)
    {
        for(i=0; i<5; i++)
            Get_6x8_char_single(0,88+i*6,data_buff[i]);				//line1
    }
    if(M_menur1_language_select_temp==2)
    {
        for(i=0; i<40; i++)
        {
            Disp_buff128x64[128+80+i]<<=3;
            Disp_buff128x64[128+80+i]&=0xf8;
            k=Disp_buff128x64[80+i];
            Disp_buff128x64[80+i]<<=3;
            k>>=5;
            k&=0x07;
            Disp_buff128x64[128+80+i]|=k;
            Disp_buff128x64[128+80+i]&=0x07;
        }
    }

    for(i=0; i<128; i++)
    {
        if(M_menur1_language_select_temp==2)
            Disp_buff128x64[128+i]|=0x80;
        else
            Disp_buff128x64[128+i]=0x10;
    }
    if(Prog_disp_flag!=2)
        Display_all_buff();
}

//===============================================================================
//	finction	:Read_last_result
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-10-10 17:03
//	modefy		:null
//===============================================================================
void Read_last_result(uint32_t adds,uint8_t select,uint8_t data_select)
{
    uint8_t  buff[6],i;
    uint32_t *intp,adds_a,adds_b,sub;
    float *fp,k,R_rs;

    adds_a=adds_b=adds;

    adds_b&=0x000001ff;						//byte adds
    adds_a>>=16;
    adds_a&=0x0000ffff;						//page adds
    if(select==1)
    {
        if((adds_b==0x00000000)&&(adds_a==0x00000004))		//get fenxi first adds
        {
            __nop();
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
    {
        if((adds_b==0x00000000)&&(adds_a==0x00000804))		//get jaozheng first adds
        {
            __nop();
        }
        else if(adds_b==0x00000000)
        {
            adds_b=0x000001e0;
            adds_a-=1;
        }
        else
            adds_b-=0x20;
    }
    Spi_read(adds_a,buff_page);

    if(data_select==1)
    {
        U2_re_table[3]=buff_page[adds_b+3];		  //yh
        U2_re_table[4]=buff_page[adds_b+4];                //yl
        U2_re_table[5]=buff_page[adds_b+5];                //m
        U2_re_table[6]=buff_page[adds_b+6];                //d
        U2_re_table[7]=buff_page[adds_b+7];                //h
        U2_re_table[8]=buff_page[adds_b+8];                //m time
    }
    else
    {
        buff[0]=buff_page[adds_b+9];
        buff[1]=buff_page[adds_b+10];
        buff[2]=buff_page[adds_b+11];
        buff[3]=buff_page[adds_b+12];
        buff[4]=buff_page[adds_b+15];			//signe_result

        sub=0;
        i=buff[0]>>4;
        i&=0x0f;
        sub=i*1000;

        i=buff[0];
        i&=0x0f;
        sub+=i*100;

        i=buff[1]>>4;
        i&=0x0f;
        sub+=i*10;

        i=buff[1];
        i&=0x0f;
        sub+=i;
//----------------------------------
        R_rs=sub;

        sub=buff[2]>>4;
        sub&=0x0000000f;
        k=sub;
        k/=10.0;
        R_rs+=k;

        sub=buff[2];
        sub&=0x0000000f;
        k=sub;
        k/=100.0;
        R_rs+=k;

        sub=buff[3]>>4;
        sub&=0x0000000f;
        k=sub;
        k/=1000.0;
        R_rs+=k;

        sub=buff[3];
        sub&=0x0000000f;
        k=sub;
        k/=10000.0;
        R_rs+=k;

        if(buff[4]==2)
            R_rs=-R_rs;
//----------------------------------
        fp=&R_rs;
        intp=(uint32_t *)fp;
        sub=*intp;
        U2_re_table[3]=sub>>24;
        U2_re_table[4]=sub>>16;
        U2_re_table[5]=sub>>8;
        U2_re_table[6]=sub;
    }
}

//===============================================================================
//	finction	:Get_step_num_each_finc
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-10-18 10:25
//	modefy		:null
//===============================================================================
uint8_t Get_step_num_each_finc(uint8_t selset)		//1=hand man,2=cal,3=xishi,4=check
{
    uint8_t total=0;
    uint8_t i=0;

    switch(selset)
    {
    case 1:
        if(M_menur1_LZDJ_select_temp==1)		//dj
        {
            i=M_menur1_sj2_v_temp;
            if(i==0)
                //if(M_menur1_sj2_v_temp==0)
            {
                if(M_menur1_DL_select_temp==1)	//jr
                {
                    total=9;
                }
                else					//qx
                {
                    total=7;
                }
            }
            else
            {
                if(M_menur1_DL_select_temp==1)	//jr
                {
                    total=10;
                }
                else					//qx
                {
                    total=8;
                }
            }
        }
        else					//bs
        {
            i=M_menur1_sj3_v_temp;
            if(i==0)
                //if(M_menur1_sj3_v_temp==0)
            {
                total=9;
            }
            else if(M_menur1_sj4_v_temp==0)
            {
                total=10;
            }
            else
            {
                total=11;
            }
        }
        break;
    case 2:
        if(M_menur1_LZDJ_select_temp==1)		//dj
        {
            i=M_menur1_sj2_v_temp;
            if(i==0)
                //if(M_menur1_sj2_v_temp==0)
            {
                total=8;
            }
            else
            {
                total=9;
            }
        }
        else					//bs
        {
            i=M_menur1_sj3_v_temp;
            if(i==0)
                //if(M_menur1_sj3_v_temp==0)
            {
                total=8;
            }
            else if(M_menur1_sj4_v_temp==0)
            {
                total=9;
            }
            else
            {
                total=10;
            }
        }
        break;
    case 3:
        if(M_menur1_LZDJ_select_temp==1)		//dj
        {
            i=M_menur1_sj2_v_temp;
            if(i==0)
                //if(M_menur1_sj2_v_temp==0)
            {
                if(M_menur1_DL_select_temp==1)	//jr
                {
                    total=7;
                }
                else					//qx
                {
                    total=5;
                }
            }
            else
            {
                if(M_menur1_DL_select_temp==1)	//jr
                {
                    total=8;
                }
                else					//qx
                {
                    total=6;
                }
            }
        }
        else					//bs
        {
            i=M_menur1_sj3_v_temp;
            if(i==0)
                //if(M_menur1_sj3_v_temp==0)
            {
                total=7;
            }
            else if(M_menur1_sj4_v_temp==0)
            {
                total=8;
            }
            else
            {
                total=9;
            }
        }
        break;
    case 4:
        if(M_menur1_LZDJ_select_temp==1)		//dj
        {
            i=M_menur1_sj2_v_temp;
            if(i==0)
                //if(M_menur1_sj2_v_temp==0)
            {
                if(M_menur1_DL_select_temp==1)	//jr
                {
                    total=8;
                }
                else					//qx
                {
                    total=6;
                }
            }
            else
            {
                if(M_menur1_DL_select_temp==1)	//jr
                {
                    total=9;
                }
                else					//qx
                {
                    total=7;
                }
            }
        }
        else					//bs
        {
            i=M_menur1_sj3_v_temp;
            if(i==0)
                //if(M_menur1_sj3_v_temp==0)
            {
                total=8;
            }
            else
            {
                total=9;
            }
        }
        break;
    }
    Prog_step_disp_cnt=0;
    return(total);
}

//===============================================================================
//	finction	:Menu3_program_all_finc
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.06
//	modefy		:null
//===============================================================================
void Menu3_program_all_finc(void)
{
}


//===============================================================================
//	finction	:Extern_port_triggle_ctrl
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2016/5/18 15:27
//	modefy		:null
//===============================================================================
void Extern_port_triggle_ctrl(void)
{
    if((Pro_power_on_flag==0) && (Program_reset_initial_flag!=1))
    {
        if((Menu_lever3_start_flag < BIT4)&&(M_lever4_extern_trrig_flag == 1))
        {
            if((Pro_wait_time_flag==2)&&(Rx_TX_flag==0))
            {
                if(Pro_step_status==0)
                {
                    Pro_step_status=1;
                    Set_tx_cmd_data_finc(U_DRIVER_MODU_ADDS,0x03,U_DRIVER_PORT1_IN_RO_ADDS,0x0001,0x00);	//read input1.
                }
                else
                {
                    if(U_data_buff[4] == 0x00)
                    {
                        Menu_lever3_start_flag&=~BIT1;
                        Menu_lever3_start_flag=BIT5;
                        Memu1_auto_step_status=0;
                        Memu1_hand_step_status=0;
                        M_lever4_FXCS_cnt=0;
                        Pro_wait_time_flag=2;
                    }
                    else
                    {
                        Pro_wait_time_set=180;	//wait 1.8s
                        Pro_wait_time_flag=1;
                    }
                    Pro_step_status=0;
                }
            }
        }
    }
}



