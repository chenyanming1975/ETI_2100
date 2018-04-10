//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Sys_Initial.c
//	First EDIT		: Sam
//	Data			: 2012.06.06
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================
#define		__SYS_INIT__
//-------------------------------------------------------------------------------------------
#include "Incude_all.h"

uint8_t	system_init_flag;

uint32_t Pro_auto_time_dcnt_bak;

//=============================================================================== 
//	finction	:System clk initial
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.06
//	modefy		:null
//===============================================================================
void System_clk_initial(void)		//系统时钟设置初始化
{
	UNLOCKREG();
//	SYSCLK->PWRCON.XTL32K_EN = 1;//Enable 32Khz for RTC clock source
	SYSCLK->PWRCON.XTL12M_EN = 1;
//	SYSCLK->PWRCON.OSC22M_EN = 1;
	SYSCLK->PLLCON.PLL_SRC = 0;
	SYSCLK->PLLCON.OE = 0;
	SYSCLK->PLLCON.PD = 0;	
        SYSCLK->PLLCON.OUT_DV=1;
        SYSCLK->PLLCON.IN_DV=1;
        SYSCLK->PLLCON.FB_DV=17;
        DrvSYS_Delay(100000);
	/* HCLK clock source. 0: external 12MHz; 2:PLL 4:internal 22MHz RC oscillator */
//	SYSCLK->CLKSEL0.HCLK_S = 4;	
	SYSCLK->CLKSEL0.HCLK_S =2;		
	LOCKREG();
	/* HCLK clock frequency = HCLK clock source / (HCLK_N + 1) */
//	DrvSYS_SetClockDivider(E_SYS_HCLK_DIV, 0); 
	/* Run 48Mhz */
//        DrvSYS_Open(48000000);
        DrvSYS_Delay(100000);	
}


//=============================================================================== 
//	finction	:Get_buff_32bit_data initial
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-13 10:20
//	modefy		:null
//===============================================================================
uint32_t Get_buff_32bit_data(uint8_t adds)
{
   uint8_t j;
   uint32_t i;
   uint8_t *index;
   
   i=0;        
   index=buff_page+adds;
   for(j=0;j<3;j++)
   {
      i+=*index;
      i<<=8;
      ++index;
   }
   i+=*index;
   return(i);
}   

//=============================================================================== 
//	finction	:Buff_get_reg_32bit_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-13 10:20
//	modefy		:null
//===============================================================================
void Buff_get_reg_32bit_data(uint8_t adds,uint32_t data)
{
   buff_page_w[adds]=data>>24;
   buff_page_w[adds+1]=data>>16;
   buff_page_w[adds+2]=data>>8;
   buff_page_w[adds+3]=data;
}   

//=============================================================================== 
//	finction	:Set_system_reg_to_buff		按位置排好参数位置
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-13 10:20
//	modefy		:null
//===============================================================================
void Set_system_reg_to_buff(void)
{
     buff_page_w[0]=M_dongtai_flag;
     buff_page_w[1]=M_menur1_lcd_Contrast_temp;
     buff_page_w[2]=M_menur1_language_select_temp;
     buff_page_w[3]=M_menur1_analy_obj_select_temp;
     buff_page_w[4]=M_menur1_unit_select_temp;
     buff_page_w[5]=M_menur1_dot_num_select_temp;
     buff_page_w[6]=M_menur1_ZDXS_select_temp;
     buff_page_w[7]=M_menur1_LZDJ_select_temp;
     buff_page_w[8]=M_menur1_DL_select_temp;
     buff_page_w[9]=M_menur1_direction_select_temp;
     buff_page_w[10]=M_menur1_led_data_temp>>8;
     buff_page_w[11]=M_menur1_led_data_temp;

     Buff_get_reg_32bit_data(M_menur1_lange_data_temp_ADDS,M_menur1_lange_data_temp);
     Buff_get_reg_32bit_data(M_menur1_led_dianliu_temp_ADDS,M_menur1_led_dianliu_temp);
     Buff_get_reg_32bit_data(M_menur1_JB_speed_temp_ADDS,M_menur1_JB_speed_temp);
     Buff_get_reg_32bit_data(M_menur1_ZSB_speed_temp_ADDS,M_menur1_ZSB_speed_temp);
     Buff_get_reg_32bit_data(M_menur1_sample_v_temp_ADDS,M_menur1_sample_v_temp);
     
     Buff_get_reg_32bit_data(M_menur1_slope_temp_ADDS,M_menur1_slope_temp);
     buff_page_w[M_menur1_slope_flag_ADDS]=M_menur1_slope_flag;   
     buff_page_w[Updata_slope_flag_ADDS]=Updata_slope_flag;     
     
     Buff_get_reg_32bit_data(M_menur1_jz1nd_temp_ADDS,M_menur1_jz1nd_temp);
     Buff_get_reg_32bit_data(M_menur1_jz1_v_temp_ADDS,M_menur1_jz1_v_temp);
     Buff_get_reg_32bit_data(M_menur1_jz2nd_temp_ADDS,M_menur1_jz2nd_temp);
     Buff_get_reg_32bit_data(M_menur1_jz2_v_temp_ADDS,M_menur1_jz2_v_temp);
     Buff_get_reg_32bit_data(M_menur1_sj1_v_temp_ADDS,M_menur1_sj1_v_temp);
     Buff_get_reg_32bit_data(M_menur1_sj2_v_temp_ADDS,M_menur1_sj2_v_temp);
     Buff_get_reg_32bit_data(M_menur1_sj3_v_temp_ADDS,M_menur1_sj3_v_temp);
     Buff_get_reg_32bit_data(M_menur1_FY_temp_ADDS,M_menur1_FY_temp);          
     Buff_get_reg_32bit_data(M_menur1_Fx_temp_ADDS,M_menur1_Fx_temp);
     Buff_get_reg_32bit_data(M_menur1_Fa_temp_ADDS,M_menur1_Fa_temp);
     Buff_get_reg_32bit_data(M_menur1_Fb_temp_ADDS,M_menur1_Fb_temp);
     Buff_get_reg_32bit_data(M_menur1_Fc_temp_ADDS,M_menur1_Fc_temp);                    
     Buff_get_reg_32bit_data(M_menur1_Fd_temp_ADDS,M_menur1_Fd_temp);                    

     buff_page_w[M_menur1_RX_time_temp_ADDS]=M_menur1_RX_time_temp;        
     Buff_get_reg_32bit_data(M_menur1_QYSJ_temp_ADDS,M_menur1_QYSJ_temp);                    
     Buff_get_reg_32bit_data(M_menur1_CSSJ_temp_ADDS,M_menur1_CSSJ_temp);                    
     Buff_get_reg_32bit_data(M_menur1_XHPY_temp_ADDS,M_menur1_XHPY_temp);                    
     Buff_get_reg_32bit_data(M_menur1_DDSJ_temp_ADDS,M_menur1_DDSJ_temp);                    

     Buff_get_reg_32bit_data(M_lever4_FXPL_temp_ADDS,M_lever4_FXPL_temp);      
     
     Buff_get_reg_32bit_data(M_lever4_Out1_4ma_temp_ADDS,M_lever4_Out1_4ma_temp);     
     Buff_get_reg_32bit_data(M_lever4_Out1_20ma_temp_ADDS,M_lever4_Out1_20ma_temp);    
     Buff_get_reg_32bit_data(M_lever4_Out1_test_ma_temp_ADDS,M_lever4_Out1_test_ma_temp); 
     Buff_get_reg_32bit_data(M_lever4_Out2_4ma_temp_ADDS,M_lever4_Out2_4ma_temp);     
     Buff_get_reg_32bit_data(M_lever4_Out2_20ma_temp_ADDS,M_lever4_Out2_20ma_temp);    
     Buff_get_reg_32bit_data(M_lever4_Out2_test_ma_temp_ADDS,M_lever4_Out2_test_ma_temp);    
      
     buff_page_w[M_lever4_extern_trrig_flag_ADDS]=M_lever4_extern_trrig_flag;                  
     buff_page_w[M_lever3_BYHC_temp_ADDS]=M_lever3_BYHC_temp; 					//1=yes 2=no
     
     Buff_get_reg_32bit_data(M_lever3_HCPD_temp_ADDS,M_lever3_HCPD_temp); 	         
     Buff_get_reg_32bit_data(a_signle_flag_ADDS,a_signle_flag); 
     Buff_get_reg_32bit_data(b_signle_flag_ADDS,b_signle_flag); 
     Buff_get_reg_32bit_data(c_signle_flag_ADDS,c_signle_flag); 	
     Buff_get_reg_32bit_data(d_signle_flag_ADDS,d_signle_flag); 				//2012-11-6 10:07
     Buff_get_reg_32bit_data(M_lever4_JZPL_temp_ADDS,M_lever4_JZPL_temp); 
     Buff_get_reg_32bit_data(M_lever4_QXPL_temp_ADDS,M_lever4_QXPL_temp); 
     Buff_get_reg_32bit_data(M_lever4_HCPL_temp_ADDS,M_lever4_HCPL_temp); 

     Buff_get_reg_32bit_data(M_menur1_sj4_v_temp_ADDS,M_menur1_sj4_v_temp);

     Buff_get_reg_32bit_data(M_lever4_tp_Out1_4ma_temp_ADDS,M_lever4_tp_Out1_4ma_temp);     
     Buff_get_reg_32bit_data(M_lever4_tp_Out1_20ma_temp_ADDS,M_lever4_tp_Out1_20ma_temp);    
     Buff_get_reg_32bit_data(M_lever4_tp_Out1_test_ma_temp_ADDS,M_lever4_tp_Out1_test_ma_temp); 
     Buff_get_reg_32bit_data(M_lever4_tp_Out2_4ma_temp_ADDS,M_lever4_tp_Out2_4ma_temp);     
     Buff_get_reg_32bit_data(M_lever4_tp_Out2_20ma_temp_ADDS,M_lever4_tp_Out2_20ma_temp);    
     Buff_get_reg_32bit_data(M_lever4_tp_Out2_test_ma_temp_ADDS,M_lever4_tp_Out2_test_ma_temp);   
                                        
     Buff_get_reg_32bit_data(M_lever4_jz_Out1_4ma_temp_ADDS,M_lever4_jz_Out1_4ma_temp);     
     Buff_get_reg_32bit_data(M_lever4_jz_Out1_20ma_temp_ADDS,M_lever4_jz_Out1_20ma_temp);    
     Buff_get_reg_32bit_data(M_lever4_jz_Out1_test_ma_temp_ADDS,M_lever4_jz_Out1_test_ma_temp); 
     Buff_get_reg_32bit_data(M_lever4_jz_Out2_4ma_temp_ADDS,M_lever4_jz_Out2_4ma_temp);     
     Buff_get_reg_32bit_data(M_lever4_jz_Out2_20ma_temp_ADDS,M_lever4_jz_Out2_20ma_temp);    
     Buff_get_reg_32bit_data(M_lever4_jz_Out2_test_ma_temp_ADDS,M_lever4_jz_Out2_test_ma_temp);  
     Buff_get_reg_32bit_data(Allow_error_ADDS,Allow_error);  
     Buff_get_reg_32bit_data(Alarm_low_nd_ADDS,Alarm_low_nd);  
     Buff_get_reg_32bit_data(Alarm_hight_nd_ADDS,Alarm_hight_nd);  
       
}   

//=============================================================================== 
//	finction	:Write_adds_reg_to_chip
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-13 10:20
//	modefy		:null
//===============================================================================
void Write_adds_reg_to_chip(void)
{
}

//=============================================================================== 
//	finction	:Write_system_reg_to_chip	保存系统参数到FLASH
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-13 10:20
//	modefy		:null
//===============================================================================
void Write_system_reg_to_chip(void)
{
	uint32_t k,i;
	
	Set_system_reg_to_buff();	
	Spi_write(SYSTEM_REG_PAGE,buff_page_w);					//write system reg		
        DrvSYS_Delay(200000);
        
	Buff_get_reg_32bit_data(0,FIRST_TIME_RW_FLAG);
	Buff_get_reg_32bit_data(4,FX_write_index_adds_point);
	Buff_get_reg_32bit_data(8,FX_read_index_adds_point);
	Buff_get_reg_32bit_data(12,JZ_write_index_adds_point);
	Buff_get_reg_32bit_data(16,JZ_read_index_adds_point);
	Buff_get_reg_32bit_data(20,CW_write_index_adds_point);
	Buff_get_reg_32bit_data(24,CW_read_index_adds_point);
	Buff_get_reg_32bit_data(28,BJ_write_index_adds_point);
	Buff_get_reg_32bit_data(32,BJ_read_index_adds_point);
	
        k=Float_to_int_reg(k2_prog_use);
        Buff_get_reg_32bit_data(36,k);
        k=Float_to_int_reg(j_prog_use);
        Buff_get_reg_32bit_data(40,k);
        k=Float_to_int_reg(k1_prog_use);
        Buff_get_reg_32bit_data(44,k);
        buff_page_w[49]=k2_signe_flag;
        
        Buff_get_reg_32bit_data(50,P_Tcall_temp);
        Buff_get_reg_32bit_data(54,E_wait_time_temp);
        Buff_get_reg_32bit_data(58,Prj_correlat);
                               
        k=adj_4ma_pwm_data;
        k<<=16;
        k+=adj_4ma_pwm1_data;
        Buff_get_reg_32bit_data(62,k);
        
        k=adj_20ma_pwm_data;
        k<<=16;
        k+=adj_20ma_pwm1_data;
        Buff_get_reg_32bit_data(66,k);

        Buff_get_reg_32bit_data(70,E_ADC_temperature1);
        Buff_get_reg_32bit_data(74,E_ADC_temperature2);
        
        buff_page_w[78]=Menu_lever3_start_flag;
        Buff_get_reg_32bit_data(79,Pro_auto_time_dcnt);        
        
        buff_page_w[83]=Machine_note_temp>>8;					//2012-10-29 11:17
        buff_page_w[84]=Machine_note_temp;        

        Buff_get_reg_32bit_data(85,Set_xishu);
        Buff_get_reg_32bit_data(89,set_cal_data);
        buff_page_w[93]=ZB_flag;
        buff_page_w[94]=Uart2_BAUD_select_temp;
        buff_page_w[95]=Uart2_JOJY_select_temp;
        buff_page_w[96]=Uart2_STOP_select_temp;

        for(i=0;i<13;i++)
           buff_page_w[97+i]=Wait_data_buff[i];
           
        buff_page_w[111]=Disple_flag;
        
        buff_page_w[112]=M_lever4_start_mini_temp;
        
        buff_page_w[113]=wd_use_flag;
                
	Spi_write(FIRST_REG_PAGE,buff_page_w);					//write first reg
}

//=============================================================================== 
//	finction	:Read_first_reg_data	从FLASH读第一扇区参数
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-13 10:20
//	modefy		:null
//===============================================================================
void Read_first_reg_data(void)
{	
        uint32_t adds_temp,i;
        
        Spi_read(FIRST_REG_PAGE,buff_page);					//read first reg	
           
        FX_write_index_adds_point=Get_buff_32bit_data(4);            	
        FX_read_index_adds_point=Get_buff_32bit_data(8);  
        JZ_write_index_adds_point=Get_buff_32bit_data(12);            	
        JZ_read_index_adds_point=Get_buff_32bit_data(16);            	                                        	
        CW_write_index_adds_point=Get_buff_32bit_data(20);            	
        CW_read_index_adds_point=Get_buff_32bit_data(24);            	
        BJ_write_index_adds_point=Get_buff_32bit_data(28);            	                                        	
	BJ_read_index_adds_point=Get_buff_32bit_data(32);  
	 
        adds_temp=Get_buff_32bit_data(36);
        k2_prog_use=Int_to_float_reg(adds_temp);
        adds_temp=Get_buff_32bit_data(40);
        j_prog_use=Int_to_float_reg(adds_temp);
        adds_temp=Get_buff_32bit_data(44);	
        k1_prog_use=Int_to_float_reg(adds_temp);					   
	k2_signe_flag=buff_page[49];
	if(k2_signe_flag==0) 
	   k2_prog_use=-k2_prog_use;
	   
	P_Tcall_temp=Get_buff_32bit_data(50);
	E_wait_time_temp=Get_buff_32bit_data(54);
	Prj_correlat=Get_buff_32bit_data(58); 
	
	adds_temp=Get_buff_32bit_data(62);  
	adj_4ma_pwm1_data=adds_temp;
	adj_4ma_pwm_data=adds_temp>>16;
	adds_temp=Get_buff_32bit_data(66);  
	adj_20ma_pwm1_data=adds_temp;
	adj_20ma_pwm_data=adds_temp>>16;

	E_ADC_temperature1=Get_buff_32bit_data(70);
	E_ADC_temperature2=Get_buff_32bit_data(74);

        Menu_lever3_start_flag=buff_page[78];		//2012-10-17 14:42
        if(Menu_lever3_start_flag==BIT5)
            Menu_lever3_start_flag=BIT1;
        if(Menu_lever3_start_flag==BIT6)
            Menu_lever3_start_flag=BIT2;
        if(Menu_lever3_start_flag==BIT7)
            Menu_lever3_start_flag=BIT3; 
            
        Pro_auto_time_dcnt_bak=Get_buff_32bit_data(79);	//

        adds_temp=buff_page[83];
        adds_temp<<=8;
        adds_temp&=0x0000ff00;
        adds_temp+=buff_page[84];
        Machine_note_temp=adds_temp;    
        Get_uart_note();            

        Set_xishu=Get_buff_32bit_data(85);  
        set_cal_data=Get_buff_32bit_data(89);    
        ZB_flag=buff_page[93];
        Uart2_BAUD_select_temp=buff_page[94];
        Uart2_JOJY_select_temp=buff_page[95];
        Uart2_STOP_select_temp=buff_page[96];
        
        for(i=0;i<13;i++)
           Wait_data_buff[i]=buff_page[97+i];
        
        Disple_flag=buff_page[111];
        
        M_lever4_start_mini_temp=buff_page[112];
        
        wd_use_flag=buff_page[113];        
}

//=============================================================================== 
//	finction	:System_reg_initial initial	系统参数初始化
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.06
//	modefy		:null
//===============================================================================
void System_reg_initial(void)
{
        uint32_t adds_temp;
        
        Led_port_status=0x00;            
	Hand_finction_step_select=0;
	Hand_finction_select=0;			//select null finc
	Back_light_time=1000;
//	Back_light_time_cnt=0;
//	Back_light_time_flag=1;
//----------------------------------------\SYSTEM REG  
        Read_first_reg_data(); 			//读第一扇区 2012-10-12 14:03     	
        DrvSYS_Delay(400000);
        DrvSYS_Delay(400000);
        DrvSYS_Delay(400000);
        DrvSYS_Delay(400000);
        DrvSYS_Delay(400000);
        DrvSYS_Delay(400000);
        
        adds_temp=0;      
        adds_temp+=buff_page[0];
        adds_temp<<=8;
        adds_temp+=buff_page[1];
        adds_temp<<=8;
        adds_temp+=buff_page[2];       			
        adds_temp<<=8;
        adds_temp+=buff_page[3];       			
/////	if(adds_temp!=FIRST_TIME_RW_FLAG)							//read flash data check and initial system reg
	if(adds_temp==FIRST_TIME_RW_FLAG)							//read flash data check and initial system reg
	{                                                                               		
           Spi_read(SYSTEM_REG_PAGE,buff_page);							//read system reg 读系统参数扇区									//delay 30ms	     
                                                                                        	
           M_dongtai_flag=buff_page[M_dongtai_flag_ADDS];					//标准体积2 动态标志 0:not use 1:use                 	
           M_menur1_lcd_Contrast_temp=buff_page[M_menur1_lcd_Contrast_temp_ADDS];     	
           M_menur1_language_select_temp=buff_page[M_menur1_language_select_temp_ADDS];  	
           M_menur1_analy_obj_select_temp=buff_page[M_menur1_analy_obj_select_temp_ADDS];		
           M_menur1_unit_select_temp=buff_page[M_menur1_unit_select_temp_ADDS];			//mg/L      	
           M_menur1_dot_num_select_temp=buff_page[M_menur1_dot_num_select_temp_ADDS];		//小数点位数 1,2,3   	
           M_menur1_ZDXS_select_temp=buff_page[M_menur1_ZDXS_select_temp_ADDS];			//自动稀释标志 1：是 2:否      	
           M_menur1_LZDJ_select_temp=buff_page[M_menur1_LZDJ_select_temp_ADDS];			//1:电极法 2：比色法		
           M_menur1_DL_select_temp=buff_page[M_menur1_DL_select_temp_ADDS];			//1:加入法 2：曲线法		
           M_menur1_direction_select_temp=buff_page[M_menur1_direction_select_temp_ADDS];	//搅拌方向 1:+ 2:-     
           if(M_menur1_direction_select_temp==0) M_menur1_direction_select_temp=1;       

           M_menur1_led_data_temp=buff_page[M_menur1_led_data_temp_ADDS];   
           M_menur1_led_data_temp<<=8;
           M_menur1_led_data_temp+=buff_page[M_menur1_led_data_temp_ADDS+1];      						
                                                					
           M_menur1_lange_data_temp=Get_buff_32bit_data(M_menur1_lange_data_temp_ADDS);		//0000.0       	
           M_menur1_led_dianliu_temp=Get_buff_32bit_data(M_menur1_led_dianliu_temp_ADDS);	//00.000 mA      	
           M_menur1_JB_speed_temp=Get_buff_32bit_data(M_menur1_JB_speed_temp_ADDS);		//搅拌速度默认 200		
           M_menur1_ZSB_speed_temp=Get_buff_32bit_data(M_menur1_ZSB_speed_temp_ADDS);		//注射泵速度默认 20		
           M_menur1_sample_v_temp=Get_buff_32bit_data(M_menur1_sample_v_temp_ADDS);  
                  	                                        	

           M_menur1_slope_temp=Get_buff_32bit_data(M_menur1_slope_temp_ADDS);			//斜率	
           M_menur1_slope_flag=buff_page[M_menur1_slope_flag_ADDS]; 
           Updata_slope_flag=buff_page[Updata_slope_flag_ADDS]; 
           if(JZ_write_index_adds_point==JZ_CHIP_START_ADDS)
           {
           	if(M_menur1_LZDJ_select_temp==2)
           	{
           	   M_menur1_slope_temp=0x00005960;
           	   M_menur1_slope_flag=2;
           	}
           	else
           	{
           	   M_menur1_slope_temp=0x00000100;
           	   M_menur1_slope_flag=1;
           	}
           }           
                      
           M_menur1_jz1nd_temp=Get_buff_32bit_data(M_menur1_jz1nd_temp_ADDS);            	
           M_menur1_jz1_v_temp=Get_buff_32bit_data(M_menur1_jz1_v_temp_ADDS);            	
           M_menur1_jz2nd_temp=Get_buff_32bit_data(M_menur1_jz2nd_temp_ADDS);            	
           M_menur1_jz2_v_temp=Get_buff_32bit_data(M_menur1_jz2_v_temp_ADDS);            	                                        	
           M_menur1_sj1_v_temp=Get_buff_32bit_data(M_menur1_sj1_v_temp_ADDS);            	
           M_menur1_sj2_v_temp=Get_buff_32bit_data(M_menur1_sj2_v_temp_ADDS);            	
           M_menur1_sj3_v_temp=Get_buff_32bit_data(M_menur1_sj3_v_temp_ADDS);            	                                        	
           M_menur1_FY_temp=Get_buff_32bit_data(M_menur1_FY_temp_ADDS);               	
           M_menur1_Fx_temp=Get_buff_32bit_data(M_menur1_Fx_temp_ADDS);               	
           M_menur1_Fa_temp=Get_buff_32bit_data(M_menur1_Fa_temp_ADDS);               	
           M_menur1_Fb_temp=Get_buff_32bit_data(M_menur1_Fb_temp_ADDS);               	
           M_menur1_Fc_temp=Get_buff_32bit_data(M_menur1_Fc_temp_ADDS);  	    
           M_menur1_Fd_temp=Get_buff_32bit_data(M_menur1_Fd_temp_ADDS);  	    

           M_menur1_RX_time_temp=buff_page[M_menur1_RX_time_temp_ADDS]; 
           M_menur1_QYSJ_temp=Get_buff_32bit_data(M_menur1_QYSJ_temp_ADDS);
           M_menur1_CSSJ_temp=Get_buff_32bit_data(M_menur1_CSSJ_temp_ADDS);   
           M_menur1_XHPY_temp=Get_buff_32bit_data(M_menur1_XHPY_temp_ADDS);   
           M_menur1_DDSJ_temp=Get_buff_32bit_data(M_menur1_DDSJ_temp_ADDS);                                

           M_lever4_FXPL_temp=Get_buff_32bit_data(M_lever4_FXPL_temp_ADDS);      
           
           M_lever4_Out1_4ma_temp=Get_buff_32bit_data(M_lever4_Out1_4ma_temp_ADDS);     
           M_lever4_Out1_20ma_temp=Get_buff_32bit_data(M_lever4_Out1_20ma_temp_ADDS);    
           M_lever4_Out1_test_ma_temp=Get_buff_32bit_data(M_lever4_Out1_test_ma_temp_ADDS); 
           M_lever4_Out2_4ma_temp=Get_buff_32bit_data(M_lever4_Out2_4ma_temp_ADDS);     
           M_lever4_Out2_20ma_temp=Get_buff_32bit_data(M_lever4_Out2_20ma_temp_ADDS);    
           M_lever4_Out2_test_ma_temp=Get_buff_32bit_data(M_lever4_Out2_test_ma_temp_ADDS);   
             
           M_lever4_extern_trrig_flag=buff_page[M_lever4_extern_trrig_flag_ADDS];                        
           M_lever3_BYHC_temp=buff_page[M_lever3_BYHC_temp_ADDS]; 		//1=yes 2=no
           
           M_lever3_HCPD_temp=Get_buff_32bit_data(M_lever3_HCPD_temp_ADDS); 	         
	   a_signle_flag=Get_buff_32bit_data(a_signle_flag_ADDS); 
	   b_signle_flag=Get_buff_32bit_data(b_signle_flag_ADDS); 
	   c_signle_flag=Get_buff_32bit_data(c_signle_flag_ADDS); 	
	   d_signle_flag=Get_buff_32bit_data(d_signle_flag_ADDS); 	
	   M_lever4_JZPL_temp=Get_buff_32bit_data(M_lever4_JZPL_temp_ADDS); 
	   M_lever4_QXPL_temp=Get_buff_32bit_data(M_lever4_QXPL_temp_ADDS); 
	   M_lever4_HCPL_temp=Get_buff_32bit_data(M_lever4_HCPL_temp_ADDS); 
	   
	   M_menur1_sj4_v_temp=Get_buff_32bit_data(M_menur1_sj4_v_temp_ADDS); 

           M_lever4_tp_Out1_4ma_temp=Get_buff_32bit_data(M_lever4_tp_Out1_4ma_temp_ADDS);     
           M_lever4_tp_Out1_20ma_temp=Get_buff_32bit_data(M_lever4_tp_Out1_20ma_temp_ADDS);    
           M_lever4_tp_Out1_test_ma_temp=Get_buff_32bit_data(M_lever4_tp_Out1_test_ma_temp_ADDS); 
           M_lever4_tp_Out2_4ma_temp=Get_buff_32bit_data(M_lever4_tp_Out2_4ma_temp_ADDS);     
           M_lever4_tp_Out2_20ma_temp=Get_buff_32bit_data(M_lever4_tp_Out2_20ma_temp_ADDS);    
           M_lever4_tp_Out2_test_ma_temp=Get_buff_32bit_data(M_lever4_tp_Out2_test_ma_temp_ADDS);   
                      
           M_lever4_jz_Out1_4ma_temp=Get_buff_32bit_data(M_lever4_jz_Out1_4ma_temp_ADDS);     
           M_lever4_jz_Out1_20ma_temp=Get_buff_32bit_data(M_lever4_jz_Out1_20ma_temp_ADDS);    
           M_lever4_jz_Out1_test_ma_temp=Get_buff_32bit_data(M_lever4_jz_Out1_test_ma_temp_ADDS); 
           M_lever4_jz_Out2_4ma_temp=Get_buff_32bit_data(M_lever4_jz_Out2_4ma_temp_ADDS);     
           M_lever4_jz_Out2_20ma_temp=Get_buff_32bit_data(M_lever4_jz_Out2_20ma_temp_ADDS);    
           M_lever4_jz_Out2_test_ma_temp=Get_buff_32bit_data(M_lever4_jz_Out2_test_ma_temp_ADDS);   
           Allow_error=Get_buff_32bit_data(Allow_error_ADDS); 
           Alarm_low_nd=Get_buff_32bit_data(Alarm_low_nd_ADDS); 
           Alarm_hight_nd=Get_buff_32bit_data(Alarm_hight_nd_ADDS); 
	   
           Set_en_buzz_finc(2,4,200,100);

	   system_init_flag=1;
	   Read_result_from_flash(1);
/////	   Read_result_from_flash(2);											 
/////	   Read_result_from_flash(3);
/////	   Read_result_from_flash(4);   	   
	   system_init_flag=0;
	   
	   Write_system_reg_to_chip();		//2012-11-7 10:43
	}
	else		//第一次使用试初始化为默认参数
	{
           M_dongtai_flag=0;								//标准体积2 动态标志 0:not use 1:use                 	
           M_menur1_lcd_Contrast_temp=0;     						
           M_menur1_language_select_temp=2;  						
           M_menur1_analy_obj_select_temp=1;							
           M_menur1_unit_select_temp=2;							//mg/L      	
           M_menur1_dot_num_select_temp=1;							//小数点位数 1,2,3   	
           M_menur1_ZDXS_select_temp=2;							//自动稀释标志 1：是 2:否      	
           M_menur1_LZDJ_select_temp=1;							//1:电极法 2：比色法		
           M_menur1_DL_select_temp=1;							//1:加入法 2：曲线法		
           M_menur1_direction_select_temp=1;						//搅拌方向 1:+ 2:-
           	                                					
           M_menur1_led_data_temp=0;         						
                                                					
           M_menur1_lange_data_temp=0;							//0000.0       	
           M_menur1_led_dianliu_temp=0;							//00.000 mA      	
           M_menur1_JB_speed_temp=0x200;							//搅拌速度默认 200		
           M_menur1_ZSB_speed_temp=0x020;							//注射泵速度默认 20		
           M_menur1_sample_v_temp=0;         	
           					                                        	
           M_menur1_slope_temp=0x00000100;						//斜率	
           M_menur1_slope_flag=1;
           Updata_slope_flag=0;
           
           M_menur1_jz1nd_temp=0;            	
           M_menur1_jz1_v_temp=0;            	
           M_menur1_jz2nd_temp=0;            	
           M_menur1_jz2_v_temp=0;            	                                        	
           M_menur1_sj1_v_temp=0x10;            	
           M_menur1_sj2_v_temp=0;            	
           M_menur1_sj3_v_temp=0;   
           M_menur1_sj4_v_temp=0; 	//2012-9-4 16:21      
             	                                        	
           M_menur1_FY_temp=0;               	
           M_menur1_Fx_temp=0;               	
           M_menur1_Fa_temp=0x00010000;               	
           M_menur1_Fb_temp=0x00010000;               	
           M_menur1_Fc_temp=0x00010000;     
           M_menur1_Fd_temp=0x00010000;     
           
           M_menur1_RX_time_temp=2;
           M_menur1_QYSJ_temp=0;   
           M_menur1_CSSJ_temp=0;   
           M_menur1_XHPY_temp=0x00000100;   
           M_menur1_DDSJ_temp=0x00000010;   
           
           M_lever4_FXPL_temp=0x00000120;    
           M_lever4_HCPL_temp=0; 
           
           M_lever4_Out1_4ma_temp=0;    
           M_lever4_Out1_20ma_temp=0;   
           M_lever4_Out1_test_ma_temp=0;
           M_lever4_Out2_4ma_temp=0;    
           M_lever4_Out2_20ma_temp=0;   
           M_lever4_Out2_test_ma_temp=0;    

           M_lever4_Out1_4ma_temp=0;    
           M_lever4_Out1_20ma_temp=0;   
           M_lever4_Out1_test_ma_temp=0;
           M_lever4_Out2_4ma_temp=0;    
           M_lever4_Out2_20ma_temp=0;   
           M_lever4_Out2_test_ma_temp=0;    

           M_lever4_Out1_4ma_temp=0;    
           M_lever4_Out1_20ma_temp=0;   
           M_lever4_Out1_test_ma_temp=0;
           M_lever4_Out2_4ma_temp=0;    
           M_lever4_Out2_20ma_temp=0;   
           M_lever4_Out2_test_ma_temp=0;    

           M_lever4_extern_trrig_flag=0;            
           
           M_lever3_BYHC_temp=2;		//1=yes 2=no
           M_lever3_HCPD_temp=0x00000000;	         
	   a_signle_flag=1;
	   b_signle_flag=1;
	   c_signle_flag=1;	
	   d_signle_flag=1;	
	   M_lever4_JZPL_temp=0;
	   M_lever4_QXPL_temp=0;

           FX_write_index_adds_point=FX_CHIP_START_ADDS;            	
           FX_read_index_adds_point=FX_CHIP_START_ADDS;            	
           JZ_write_index_adds_point=JZ_CHIP_START_ADDS;            	
           JZ_read_index_adds_point=JZ_CHIP_START_ADDS;            	                                        	
           CW_write_index_adds_point=CW_CHIP_START_ADDS;            	
           CW_read_index_adds_point=CW_CHIP_START_ADDS;            	
           BJ_write_index_adds_point=BJ_CHIP_START_ADDS;            	                                        	
	   BJ_read_index_adds_point=BJ_CHIP_START_ADDS; 
	   
	   k2_prog_use=1.0;  
	   k2_signe_flag=1;
	   j_prog_use=1.0;
	   
	   P_Tcall_temp=0;		//2012-9-13 11:42
	   E_wait_time_temp=0;		//2012-9-14 14:32
	   Prj_correlat=0x10001000;     //2012-9-14 14:32

	   Set_xishu=0x00001000;	//2012-12-17 10:25
	   set_cal_data=0x00000000;	//2013-1-7 11:00
	   
	   Allow_error=0x00000010;
	   Alarm_hight_nd=0x00099999;
	   Alarm_low_nd=0x00000000;
	   
	   Menu_lever3_start_flag=0;
	   Pro_auto_time_dcnt_bak=0;
	   
	   adj_4ma_pwm_data=adj_4ma_pwm1_data=952;
	   adj_20ma_pwm_data=adj_20ma_pwm1_data=4761;
           
           Machine_note_temp=0x0016;		//2012-10-29 11:27
           Get_uart_note();   
           Uart2_BAUD_select_temp=1;     
           Uart2_JOJY_select_temp=3;
           Uart2_STOP_select_temp=1;
           
           Disple_flag=0;			//2014-1-26 10:34
           
           DrvSYS_Delay(200000);
           DrvSYS_Delay(200000);	   	   
	   Write_system_reg_to_chip();
           DrvSYS_Delay(200000);
           DrvSYS_Delay(200000);	   	   
           
    	   Fenxi.xuhao_h=0;
    	   Fenxi.xuhao_l=1;
    	   Jiaozheng.xuhao_h=0;
    	   Jiaozheng.xuhao_l=1;
    	   Cuowu.xuhao_h=0;
    	   Cuowu.xuhao_l=1;
    	   Baojing.xuhao_h=0;
    	   Baojing.xuhao_l=1;

           Fenxi.status=0x00;            
           Fenxi.year_h=0x00;            
           Fenxi.year_l=0x00;            
           Fenxi.munth=0x00;             
           Fenxi.day=0x00;               
           Fenxi.hour=0x00;              
           Fenxi.minu=0x00;              
           Fenxi.result_ph=0x00;         
           Fenxi.result_pl=0x00;        
           Fenxi.result_nh=0x00;        
           Fenxi.result_nl=0x00;        
           Fenxi.unit_alarm_wrong=0x00; 
           Fenxi.yes_or_no=0x00;        
           Fenxi.signe_result=0x01;     
                                 
           Fenxi.e1_result_ph=0x00;     
           Fenxi.e1_result_pl=0x00;     
           Fenxi.e1_result_nh=0x00;     
           Fenxi.e1_result_nl=0x00;     
           Fenxi.e2_result_ph=0x00;     
           Fenxi.e2_result_pl=0x00;     
           Fenxi.e2_result_nh=0x00;     
           Fenxi.e2_result_nl=0x00;     
           Fenxi.fenxi_fangfa=0x00;     
           Fenxi.fenxi_obj=0x00;
  
    	   Write_result_to_flash(FX_write_index_adds_point,&Fenxi);  
           DrvSYS_Delay(200000);
           DrvSYS_Delay(200000);
    	   Write_result_to_flash(JZ_write_index_adds_point,&Fenxi);  
           DrvSYS_Delay(200000);
           DrvSYS_Delay(200000);
    	   Write_result_to_flash(CW_write_index_adds_point,&Fenxi);  	 
           DrvSYS_Delay(200000);
           DrvSYS_Delay(200000);
    	   Write_result_to_flash(BJ_write_index_adds_point,&Fenxi); 
           DrvSYS_Delay(200000);
           DrvSYS_Delay(200000);
						//delay 30ms	     	   
	   RTC_Set.sec_temp=00;       
	   RTC_Set.minu_temp=0x00;      
	   RTC_Set.hour_temp=0x16;      
	   RTC_Set.day_temp=0x18;       
	   RTC_Set.mouth_temp=0x08;     
	   RTC_Set.year_temp=0x12;      
	   RTC_Set.week_temp=0x04;                     
	   RTC_Set.alarm_minu_temp=0x00;
	   RTC_Set.alarm_hour_temp=0x00;
	   RTC_Set.alarm_day_temp=0x00; 
	   RTC_Set.alarm_week_temp=0x00;

           Set_en_buzz_finc(2,4,200,100);
////           if(M_menur1_language_select_temp==2)
////              Display_logo(cm_logo);
////           else
////	      Display_logo(cm_logo_en);
////	   
////	   RTC_read_12byte_data();
////	   RTC_Set=RTC_DATA;
////	   RTC_initial(); 
        }       
        Menu_lever_select=0;      		
	Pro_step_status=0;
	Pro_power_on_flag=1;
	M_lever4_FXCS_temp=0x01;  			//2012-8-23 17:13 sams
	
	Hand_finction_step_select=0;
	df_temp=1;				//2012-7-24 10:55
	E_step_cnt=0;
	Prog_disp_flag=0;
	if(Pro_auto_time_dcnt_bak==0)
	{
           if(Menu_lever3_start_flag==BIT4)
           {
	      Pro_auto_time_dcnt=Pro_auto_time_dcnt_bak;
	      Pro_auto_time_up_check_flag=0;
	      Memu1_auto_step_status=30;
	   }
	   else
	   Pro_auto_time_dcnt=0;
	}
	else
	{
	   Pro_auto_time_dcnt=Pro_auto_time_dcnt_bak;
	   Pro_auto_time_up_check_flag=0;
	   Memu1_auto_step_status=30;
	   Menu_lever3_start_flag=BIT4;
	}
	   
	Pro_auto_time_up_check_flag=0;
	Pro_dis_menu_flag=0;
	BCWD_NG_flag=1;				//2012-8-2 9:33
	Rx_TX_flag=0;

        Fenxi.disp_index=0;
        Jiaozheng.disp_index=0;
        Baojing.disp_index=0;
        Cuowu.disp_index=0;
	//read diancifa status initial	
	U2_rx_cnt=0;
	U2_rx_Flag=0;
	
	Key_scan_time_cnt=0;
	Clead_pass_word_reg_data();		//2012-10-23 10:08
	User_prio_temp=1;
	
	Reget_slope=0;				//2012-11-2 9:52	
	Alarm_active_flag=0;		
        
        if(M_menur1_JB_speed_temp<0x0175) M_menur1_JB_speed_temp=0x0200;
        
	M_menur1_language_select_temp=2;		//2016/4/20 13:23
	LCD_disp_flag=0;
	Prog_disp_flag=0;
	Led_data_flag=0;								//2012-12-3 11:57 LED状态显示标志	
	fix_start_time_falg=1;								//自动分析开始时间为系统时间
	DrvSYS_Delay(200000);  
}          
//=============================================================================== 
//	finction	:GPIO initial
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.06
//	modefy		:null
//===============================================================================
void GPIO_initial(void)
{
	uint8_t	i;
	DrvGPIO_InitFunction(E_FUNC_GPIO);	 //SET ALL PIN AS GPIO
//-------------------------PORTA	
	DrvGPIO_InitFunction(E_FUNC_I2C0);	// Set I2C0 A8,A9 Pin  FOR8563
	DrvGPIO_InitFunction(E_FUNC_PWM01);	/* Set PWM0 AND PWM1 A12,A13 Pin */
	for(i=0;i<8;i++)
	{
	    DrvGPIO_Open (E_GPA,i, E_IO_OUTPUT);	//D0~D7  A0~A7	
	}
	outpw(&SYS->GPAMFP, inpw(&SYS->GPAMFP) & ~0xff);	//2012-6-11 16:45
	
	DrvGPIO_Open (E_GPA,10, E_IO_OUTPUT);	//IDLE
	DrvGPIO_Open (E_GPA,11, E_IO_OUTPUT);	//RD1
	outpw(&SYS->GPAMFP, inpw(&SYS->GPAMFP) & ~(0x3<<10));	//2012-6-11 16:45
	DrvGPIO_Open (E_GPA,14, E_IO_OUTPUT);	//NC	2012-10-19 16:55
	DrvGPIO_Open (E_GPA,15, E_IO_OUTPUT);	//RD FOR LCD
	outpw(&SYS->GPAMFP, inpw(&SYS->GPAMFP) & ~(0x3<<14));	//2012-6-11 16:45
//-------------------------PORTB	
	DrvGPIO_InitFunction(E_FUNC_UART0);	// Set UART0 B0,B1 Pin 		UART-USB	
	DrvGPIO_InitFunction(E_FUNC_UART1);	// Set UART1 B4,B5 Pin 		RS485 RD1
	DrvGPIO_Open (E_GPB,2, E_IO_OUTPUT);	//CSM  B2
	DrvGPIO_Open (E_GPB,3, E_IO_OUTPUT);	//DRST B3
	outpw(&SYS->GPBMFP, inpw(&SYS->GPBMFP) & ~(0x3<<2));	//2012-6-8 16:45
	
	DrvGPIO_Open (E_GPB,6, E_IO_OUTPUT);	//COM1 B6
	DrvGPIO_Open (E_GPB,7, E_IO_OUTPUT);	//RD2  B7
	outpw(&SYS->GPBMFP, inpw(&SYS->GPBMFP) & ~(0x3<<6));	//2012-6-8 16:45
	
	DrvGPIO_Open (E_GPB,8, E_IO_OUTPUT);	//CAL LED B8
	DrvGPIO_Open (E_GPB,9, E_IO_OUTPUT);	//RS   B9
	DrvGPIO_Open (E_GPB,10, E_IO_OUTPUT);	//CS   B10
	
	DrvGPIO_Open (E_GPB,11, E_IO_OUTPUT);	//LIGHT  B11
	DrvGPIO_SetBit (E_GPB,11);
	
	DrvGPIO_Open (E_GPB,12, E_IO_OUTPUT);	//COM2  B12
	DrvGPIO_Open (E_GPB,13, E_IO_OUTPUT);	//COM0  B13
	DrvGPIO_Open (E_GPB,14, E_IO_INPUT);	//CLKINT  B14 FOR 8563		
	
	DrvGPIO_Open (E_GPB,15, E_IO_OUTPUT);	//RUN LED  B15	
//-------------------------PORTC	
//	DrvGPIO_InitFunction(E_FUNC_SPI0);	// Set SPI0 Pin FOR LCD KEY 595 C2,C3									 
//	DrvGPIO_InitFunction(E_FUNC_SPI1);	// Set SPI1 Pin FOR TFLASH  C10,C11	
									 
	outpw(&SYS->GPCMFP, inpw(&SYS->GPCMFP) & ~0x0F);			//2012-6-8 17:18
	DrvGPIO_Open (E_GPC,0, E_IO_INPUT);	//SW C0 FOR TFLASH 
	DrvGPIO_Open (E_GPC,1, E_IO_OUTPUT);	//SPI0 SCK
	DrvGPIO_Open (E_GPC,2, E_IO_INPUT);	//SPI0 MISO
	DrvGPIO_Open (E_GPC,3, E_IO_OUTPUT);	//SPI0 MOSI
	
	DrvGPIO_Open (E_GPC,6, E_IO_OUTPUT);	//LOAD  C6 
	
	outpw(&SYS->GPCMFP, inpw(&SYS->GPCMFP) & ~(0x3<<7));			//2012-6-8 17:18
	DrvGPIO_Open (E_GPC,7, E_IO_OUTPUT);	//CSIN  C7 FOR LCD 165 KEY	
	DrvGPIO_Open (E_GPC,8, E_IO_OUTPUT);	//WR  C8 FOR LCD 

	outpw(&SYS->GPCMFP, inpw(&SYS->GPCMFP) & ~(0x7<<9));			//2012-6-19 15:00
	DrvGPIO_Open (E_GPC,9, E_IO_OUTPUT);	//TCLK	
	DrvGPIO_Open (E_GPC,10, E_IO_INPUT);	//TMISO
	DrvGPIO_Open (E_GPC,11, E_IO_OUTPUT);	//TMOSI
	
	outpw(&SYS->GPCMFP, inpw(&SYS->GPCMFP) & ~(0x3<<14));			//2012-6-8 17:18	
	DrvGPIO_Open (E_GPC,14, E_IO_OUTPUT);	//ALR LED C14 
	DrvGPIO_Open (E_GPC,15, E_IO_OUTPUT);	//ERROE LED  C15 
//-------------------------PORTD	
	DrvGPIO_Open (E_GPD,6, E_IO_OUTPUT);	//SD  D6 FOR  QQQQQQQQQQQQQQQ
	DrvGPIO_Open (E_GPD,7, E_IO_OUTPUT);	//CSD  D7 FOR TFLASH

	DrvGPIO_InitFunction(E_FUNC_UART2);	//Set UART2 Pin D14,D15		RS485 RD2
//-------------------------PORTE	
	DrvGPIO_Open (E_GPE,5, E_IO_OUTPUT);	//BEEP  
        DrvGPIO_ClrBit (E_GPE,5);  		//close 	 

#ifndef __NEWPCB__   
        Led_port_status=0;       
	Led_port_status|=BIT3;
        Led_disp_data(Led_port_status);	
#else
        DrvGPIO_ClrBit (E_GPB,15);		//SET RUN LED	
#endif        
}

//=============================================================================== 
//	finction	:WDT_initial
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.14
//	modefy		:null
//===============================================================================
void WDT_initial(void)
{	
	uint32_t i;

	i=i;
	UNLOCKREG();		
	/* Step 1. Enable and Select WDT clock source */         
	SYSCLK->CLKSEL1.WDT_S =3;//Select 10Khz for WDT clock source  	
	SYSCLK->APBCLK.WDT_EN =1;//Enable WDT clock source	

////	WDT->WTCR.DBGACK_WDT=1;
	/* Step 2. Select Timeout Interval */
	WDT->WTCR.WTIS=6;//Select level 7			
	/* Step 3. Disable Watchdog Timer Reset function */
	i=WDT->WTCR.WTRF;
	WDT->WTCR.WTRE = 1;
	/* Step 4. Enable WDT interrupt */	
	WDT->WTCR.WTIF =1;//Write 1 to clear for safety		
	WDT->WTCR.WTIE = 0;
/////	NVIC_EnableIRQ(WDT_IRQn);
	/* Step 5. Enable WDT module */
	//Enable WDT
	WDT->WTCR.WTE = 1;
	 //Clear WDT counter
	WDT->WTCR.WTR = 1;		
	LOCKREG();	 
}

//=============================================================================== 
//	finction	:WDT_IRQHandler
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.14
//	modefy		:null
//===============================================================================
void WDT_IRQHandler(void) 
{ 
	UNLOCKREG(); 
	WDT->WTCR.WTIF =1;
	WDT->WTCR.WTR = 1;
	LOCKREG();
}

//=============================================================================== 
//	finction	:WDT_CLR
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.14
//	modefy		:null
//===============================================================================
void WDT_CLR(void) 
{ 
	UNLOCKREG(); 
////	WDT->WTCR.WTIF =1;
	WDT->WTCR.WTR = 1;
	LOCKREG();
}



