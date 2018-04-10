//============================================================================================
//	Project name	: andan test
//	MCU TYPE		: Nuv100RE3AN
//	Finction		: main
//	First EDIT		: Sam
//	Data			: 2012.06.06
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================
//--------------------include files------------------------																										 
#include "Incude_all.h"
//-------------------Pub Counst-- --------------------------

void Program_power_run_ctrl(void);
void Export_data_ctrl_finc(void);

extern void Updata_xuhao_dec(uint8_t select,uint8_t dec_data);

//-------------------Pub Variable --------------------------
/////static uint16_t  System_status_cnt;
//uint8_t	Tx_buff[4]={0,1,2,3};

//=============================================================================== 
//	finction	:main		
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.06
//	modefy		:null
//===============================================================================
int32_t	main(void)
{            	
   int8_t i,data_buff[6];									        
//   int32_t Sub_adds;

   System_clk_initial();				//��ʼ��ϵͳʱ��
   DrvSYS_Delay(6000000);                               
   GPIO_initial();					//��ʼ��I/O				  
   DrvTIMER_Init();                                     //��ʼ��ϵͳ��ʱ��
   Timer0_initial();                                    //��ʼ����ʱ��0
   Timer1_initial();					//��ʼ����ʱ��1			  
   Init_UART012();                                      //��ʼ������0.1.2
   SPI_flash_init();					//��ʼ��SPI 2012-10-25 14:51
   Lcd_initial(); 					//��ʼ��LCD   	
   
#ifdef __THERMO__
   Display_logo_pic(cm1_logo_pic);			//��ʾtnermo��˾LOGO
#else   
   //Display_logo_pic(cm_logo_pic);			//��ʾETI��˾LOGO
   Clear_n_page(0,7); 
   data_buff[0]=33;
   data_buff[1]=34;
   data_buff[2]=203;
   data_buff[3]=15;
   data_buff[4]=204;
   data_buff[5]=0; 
   for(i=0;i<5;i++)
   {
       Get_16x16_char_single(3,i*18+20,data_buff[i]); 
   }
   Display_all_buff();

#endif   
   System_reg_initial();         			//ϵͳ������ʼ��
#ifdef __DEBUG__   

   Pro_power_on_flag=0;
#endif   
   Sd_io_initial();					//����Ƿ���SD������ʼ��
   if(Rd_check_lcd_data())				///2013-3-15 11:06 add ��鲢�����ֿ�
   {
   	Check_updta_lcd_data();
   }
   else
   {   	
   	if((DrvGPIO_GetBit(E_GPC,0))==0)
   	    Check_updta_lcd_data();
   	Rd_init_lcd_data();
   }      

   Power_on_check_device();				//���������ʹ��������Ƿ����
   InitPWM();   					//��ʼ��PWM���	
   Back_light_time_flag=1;
   Back_light_time_cnt=0;     
/////   Display_menu_index();   

   if((buff_bool[0]==0)&&(buff_bool[1]==0)&&(buff_bool[2]==0)&&(buff_bool[3]==0)) 	
   {
      Pro_power_on_flag=0;
   }
   else   
   {
      Display_menu_index();   				//��ʾ���һ�η������
   }

   RTC_read_12byte_data();				//
   RTC_Set=RTC_DATA;                                    //��ʼ��RTCоƬ����������
   RTC_initial();                                       //
   Set_en_buzz_finc(1,4,200,100);
   DrvSYS_Delay(200000);				//delay 200ms
   Pro_wait_time_flag=2;
#ifdef __UART_TEST__					//������֤�ϴ�����Э��ı�־���Ѿ����á�
   Test_potocal_time_cnt=0;
   Test_potocal_time_mini_cnt=0;
   Test_potocal_time_flag=2;				//flag
#endif  
/////   tx_jisuan_data(0);					//for test

//               Get_write_struct_data(2,6,0x00010000,4,1,1);	 //�ֶ�У��               
//    	       Sub_adds=Write_result_to_flash(JZ_write_index_adds_point,&Jiaozheng);
//    	       JZ_write_index_adds_point=Sub_adds;   
//    	       Write_index_adds_point();	
//    	                            
//               M_menur1_slope_flag=1;    	        
//    	       M_menur1_slope_temp=0x00000100;
//    	       Write_system_reg_to_chip();

   WDT_initial();					//��ʼ�����Ź�   
   while(1)
   {
	WDT_CLR();
	//----------------------------------/clear WDT
  	Key_scan_prosecce();			//��������
  	Uart012_FIFO_INT_CTRL();		//���ڽ��մ���
  	
#ifndef __DEBUG__   
  	Program_power_on_initial();		//after initial this finction will not run .
#endif     	
  	Program_power_run_ctrl();		//�Զ��������ֶ�������У�������̿��ơ�
  	Hand_opration_finction();		//�ֶ��������̿���
  	Disp_GDGDY_real_time();			//��ʾ�¶ȣ����ܵ�ѹ�� 2012-9-6 17:01
  	Export_data_ctrl_finc();		//���������ݺ��� 2012-9-7 13:49
  	Pwm_output_ctrl();			//4~20ma�������
#ifdef __UART_TEST__
        Run_potocal_test(); 			//������֤ͨ��Э�����
#endif  	
        Sd_io_initial();			//����Ƿ���SD������ʼ��
        //Aoto_program_ctrl();			//����Զ����¿��ƺ���		//2014-3-11 9:01 del
        Program_reset_initial();		//ֹͣ���������̺��������ò��ֲ���
        
        Extern_port_triggle_ctrl();			//2016/6/15 14:32

//        if(Rtc_int_flag==0x03)
//        {        
//            RTC_read_12byte_data();				//
//            RTC_Set=RTC_DATA;                                    //��ʼ��RTCоƬ����������
//            RTC_initial();     
//        }  

////        if(wd_use_flag==1)
////        {
////            Display_seg_single(7,126,0xc0);   
////        }
   }
}
//================================================================================================================== 
//================================================================================================================== 
//================================================================================================================== 
//=============================================================================== 
//	finction	:Export_data_ctrl_finc	�����ݿ��ƺ���
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-7 13:51
//	modefy		:null
//===============================================================================
void Export_data_ctrl_finc(void)
{   
   if((Export_data_flag==1)&&(Buzz_en_flag==0))
   {
      Export__flash_data_finc();
   }
   else if(Export_data_flag==0)
   {
      Export_data_flag=2;
   }
}
   
//=============================================================================== 
//	finction	:Program_power_run_ctrl	
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-25 10:42
//	modefy		:null
//===============================================================================
void Program_power_run_ctrl(void)
{    
    uint8_t i;
    
    if((Pro_power_on_flag==0)&&(Program_reset_initial_flag!=1))
    {
        if(Menu_lever3_start_flag==BIT4)		//�Զ�����
        {
           if(Auto_check_clock_flag==0)
              Menu1_auto_fenxi_finc();
           else
           {
               Disp_rtc_clock_wait();
               

               i=RTC_DATA.year_temp;
               i&=0x7f;
               if(M_lever4_start_year_temp<i)
               {            	    
		   Auto_check_clock_flag=0;
		   Pro_auto_time_dcnt=0;
		   Pro_dis_menu_flag=0;
		   Menu1_auto_fenxi_finc();
               } 
               else if(M_lever4_start_year_temp==i)
               {               
                   i=RTC_DATA.mouth_temp;
                   i&=0x1f;
                   if(M_lever4_start_munth_temp<i)
                   {            	    
		       Auto_check_clock_flag=0;
		       Pro_auto_time_dcnt=0;
		       Pro_dis_menu_flag=0;
		       Menu1_auto_fenxi_finc();
                   } 
                   else if(M_lever4_start_munth_temp==i)
                   {
                       i=RTC_DATA.day_temp;
                       i&=0x3f;
                       if(M_lever4_start_day_temp<i)
                       {
		           Auto_check_clock_flag=0;
		           Pro_auto_time_dcnt=0;
		           Pro_dis_menu_flag=0;
		           Menu1_auto_fenxi_finc();
                       } 
                       else if(M_lever4_start_day_temp==i)
                       {
                           i=RTC_DATA.hour_temp;
                           i&=0x3f;
                           if(M_lever4_start_hour_temp<i)
                           {
		               Auto_check_clock_flag=0;
		               Pro_auto_time_dcnt=0;
		               Pro_dis_menu_flag=0;
		               Menu1_auto_fenxi_finc();
                           }
                           else if(M_lever4_start_hour_temp==i)
                           {
                               i=RTC_DATA.minu_temp;
                               i&=0x7f;
                               if(M_lever4_start_mini_temp<=i)
                               {
		    	           Auto_check_clock_flag=0;
		    	           Pro_auto_time_dcnt=0;
		    	           Pro_dis_menu_flag=0;
		    	           Menu1_auto_fenxi_finc();
                       	       }
                           }
                       } 
                   }  
                }         
           }
        }
        else if(Menu_lever3_start_flag==BIT5)		//�ֶ�����
        {
          if(M_lever4_FXCS_cnt==M_lever4_FXCS_temp)
          {
              M_lever4_FXCS_cnt=0;
              Menu_lever3_start_flag&=~BIT5; 
              Write_system_reg_to_chip();
              Prog_disp_flag=0;
          }
          else Menu1_hand_fenxi_finc();
        }
        else if(Menu_lever3_start_flag==BIT6)		//�ֶ�У��
           Menu1_hand_jiaozheng_finc();
        else if(Menu_lever3_start_flag==BIT7)		//�ֶ��˲�
           Menu1_biaoye_hecha_finc();
        else
        {
           __nop();
        }
    }
}
