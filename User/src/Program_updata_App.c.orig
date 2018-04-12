//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Program_up_data.c
//	First EDIT		: Sam
//	Data			: 2012-9-10 11:27
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//	NOTE			;驱动板文件名格式为：BIN_XN.txt/BIN_XXN.txt/BIN_XXXN.txt;     X表示从0~9任意一个数字，不可以是数字以外的字符
//                              ;传感器板文件名格式为：BIN_X.txt/BIN_XX.txt/BIN_XXX.txt;      X表示从0~9任意一个数字，不可以是数字以外的字符
//============================================================================================
#define	__PRO_UP_DATA__
//-------------------------------------------------------------------------------
#include "Incude_all.h"

uint8_t	Aoto_program_flag;		//选择更新驱动板还是传感器板
uint8_t P_status_temp;			//save status BIT7 for flag 
uint8_t p_step_cnt;			//auto program step cnt
uint8_t P_cmd_adds;			//192 个包，每个包73字节
uint8_t P_cmd_adds_bak;

uint8_t p_packge_tx_flag;
uint8_t P_read_status_cnt;

uint32_t P_sch_adds_temp;
uint8_t Read_file_flag;

uint8_t Check_file_flag;
//===============================================================================
//	finction	:Get_bin_data  查找BIN文件读对应文件内容
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-12-25 9:42
//	modefy		:null
//===============================================================================
void Get_bin_data(void)
{
    uint8_t i;
    uint32_t fn_p2_tp,j;
    
    SD_read_flag=0;
    Read_file_flag=0;
    fn_p2_tp=0;
    if(P_cmd_adds>=10)
    {    	
    	if(P_cmd_adds>=100)
    	{
    	   i=j=P_cmd_adds;
    	   i/=100;
    	   i=Get_ascii_data(i);
    	   fn_p2_tp=(i<<24);
    	   
    	   j/=10;
    	   j%=10;
    	   j=Get_ascii_data(j);
    	   j<<=16;
    	   fn_p2_tp|=j;
    	   
    	   j=P_cmd_adds;
    	   j%=10;
    	   j=Get_ascii_data(j);
    	   j<<=8;  
    	   fn_p2_tp|=j;  	   
    	   fn_p2_tp|=0x00000020;  
    	     	
           if(Aoto_program_flag!=1)				//Aoto_program_flag=1:d0(传感器板)    =2:d1（驱动板）
           {
              fn_p2_tp&=0xffffff00;
              fn_p2_tp|=0x0000004E;				//
           }    	  	
    	}
    	else
    	{
    	   i=j=P_cmd_adds;
    	   i/=10;
    	   i=Get_ascii_data(i);
    	   fn_p2_tp=(i<<24);
    	   
    	   j%=10;
    	   j=Get_ascii_data(j);
    	   j<<=16;
    	   fn_p2_tp|=j;    	   
    	   fn_p2_tp|=0x00002020;    	

           if(Aoto_program_flag!=1)				//Aoto_program_flag=1:d0(传感器板)    =0:d1（驱动板）
           {
              fn_p2_tp&=0xffff00ff;
              fn_p2_tp|=0x00004E00;				//
           }    	  	
    	}
    }
    else
    {
    	i=P_cmd_adds;
    	i=Get_ascii_data(i);
    	fn_p2_tp=(i<<24);
    	fn_p2_tp|=0x00202020;  

       if(Aoto_program_flag!=1)				//Aoto_program_flag=1:d0(传感器板)    =2:d1（驱动板）
       {
       	  fn_p2_tp&=0xff00ffff;
       	  fn_p2_tp|=0x004E0000;				//
       }    	  	
    }   
    
    for(j=0;j<10;j++)
    {       
       i=Sd_read_file(0x00545854,0x42494E5F,fn_p2_tp);	//txt file BIN_XXX
       if(i!=0) j=11;
       else
       {
       	  SD_read_flag=0;
       	  Read_file_flag=1;
       }
    }    
    if(Check_file_flag==0)	
    {
       for(i=0;i<73;i++)
          U_data_buff[i]=SD_data_buff[i];
    }

}

//=============================================================================== 
//	finction	:Disp_updata_status	显示程序更新的状态
//	input		:finc 1=no car; 2=clear eeprom; 3=program; 4=faile;5=succese
//	output		:null
//	return		:null
//	edit		:sam 2012-9-10 11:56
//	modefy		:null
//===============================================================================
void Disp_updata_status(uint8_t finc)
{
    uint8_t data_buff[16],i=0;
    
////    Clear_screen();
	Clear_n_page(0,7); 
    switch(finc)
    {
       case 1:
             data_buff[0]='N';
             data_buff[1]='O';
             data_buff[2]=0;
             data_buff[3]='F';  
             data_buff[4]='I';  
             data_buff[5]='L'; 
             data_buff[6]='E'; 
             i=7;
       break;
       case 2:
             data_buff[0]='C';
             data_buff[1]='L';
             data_buff[2]='E';
             data_buff[3]='A';  
             data_buff[4]='R';  
             data_buff[5]=0; 
             data_buff[6]='E';
             data_buff[7]='E';
             data_buff[8]='P';
             data_buff[9]='R';  
             data_buff[10]='O';  
             data_buff[11]='M';  
             i=12;
       break;
       case 3:
///             data_buff[0]='P';
///             data_buff[1]='R';
///             data_buff[2]='O';
///             data_buff[3]='G';  
///             data_buff[4]='R';  
///             data_buff[5]='A'; 
///             data_buff[6]='M';
///             data_buff[7]=19;  
///             data_buff[8]=19;  
///             data_buff[9]=19;  
///             i=10;

             data_buff[0]='L';
             data_buff[1]='O';
             data_buff[2]='A';
             data_buff[3]='D';  
             data_buff[4]='I';  
             data_buff[5]='N'; 
             data_buff[6]='G';
             data_buff[7]=0;  
             data_buff[8]='P';  
             data_buff[9]='R';  
             data_buff[10]='O';  
             data_buff[11]='G'; 
             data_buff[12]='R';
             data_buff[13]='A';
             data_buff[14]='M';
             i=15;             
       break;
       case 4:
             data_buff[0]='P';
             data_buff[1]='R';
             data_buff[2]='O';
             data_buff[3]='G';  
             data_buff[4]='R';  
             data_buff[5]='A'; 
             data_buff[6]='M';
             data_buff[7]=0;  
             data_buff[8]='F';
             data_buff[9]='A';  
             data_buff[10]='I';  
             data_buff[11]='L'; 
             data_buff[12]='E';  
             i=13;
       break;
       case 5:
             data_buff[0]='P';
             data_buff[1]='R';
             data_buff[2]='O';
             data_buff[3]='G';  
             data_buff[4]='R';  
             data_buff[5]='A'; 
             data_buff[6]='M';
             data_buff[7]=0;  
             data_buff[8]='S';
             data_buff[9]='U';  
             data_buff[10]='C';  
             data_buff[11]='C'; 
             data_buff[12]='E';
             data_buff[13]='S';
             data_buff[14]='E';  
             i=15;
       break;
       case 6:
             data_buff[0]='I';
             data_buff[1]='N';
             data_buff[2]='I';
             data_buff[3]='T';  
             data_buff[4]='I';  
             data_buff[5]='A'; 
             data_buff[6]='L';
             data_buff[7]=0;  
             data_buff[8]='E';
             data_buff[9]='R';  
             data_buff[10]='R';  
             i=11;
       break;
    }
    Display_6x8_char(3,0,i,data_buff);				//line3 
}

//=============================================================================== 
//	finction	:Aoto_program_ctrl	软件更新控制函数
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-10 11:56
//	modefy		:null
//===============================================================================
//p_step_cnt=1; Aoto_program_flag=1;
void Aoto_program_ctrl(void)
{
    uint8_t i;
    uint32_t k;
    
    if((Auto_check_clock_flag==0)&&(Pro_power_on_flag==0)&(Hand_finction_select==0)&&(Menu_lever3_start_flag<BIT4))
    {
       if(Aoto_program_flag>0)			//1=d0;2=d1;0=null
       {          	  
       	  if((DrvGPIO_GetBit(E_GPC,0))==1)	//check SD sw pin ,if hight then exit program. 2012-9-13 17:27
       	  {
       	     Aoto_program_flag=p_step_cnt=0;  
       	     Set_en_buzz_finc(6,4,200,100);
       	     
       	     Disp_updata_status(1);
       	  }     
/////       	  else
/////       	  {
/////       	     P_cmd_adds_bak=P_cmd_adds;
/////       	     P_cmd_adds=0;
/////       	     Check_file_flag=1;
/////       	     Get_bin_data();
/////       	     if(Read_file_flag==1)
/////       	     {
/////       	         Aoto_program_flag=p_step_cnt=0;  
/////       	         Set_en_buzz_finc(6,4,200,100);
/////       	         
/////       	         Disp_updata_status(1);
/////       	     }
/////       	     P_cmd_adds=P_cmd_adds_bak;
/////       	  }  	     
       	  switch(p_step_cnt)  
       	  {
       	     case 1:
       	         if(Rx_TX_flag==0)
       	         {
                    if(Aoto_program_flag==1)
                    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_SELF_REG_CTRL_WO_ADDS,0x00A5,0x00);  //clear flash   
       	            else
                    Set_tx_cmd_data_finc(0xd1,0x06,U_DRIVER_SELF_REG_CTRL_WO_ADDS,0x00A5,0x00);  //clear flash          	         
       	            
       	            ++p_step_cnt;       	     
       	            P_cmd_adds=0;     	     
       	            p_packge_tx_flag=0;
       	            P_read_status_cnt=0;
                    Pro_wait_time_set=300;		//wait 5sec
                    Pro_wait_time_flag=1;
                    P_sch_adds_temp=0;
                    
                    Disp_updata_status(2);  		//disp clear eeprom
       	         }  
       	     break;
       	     case 2:
       	         if((Rx_TX_flag==0)&&(Pro_wait_time_flag==2))
       	         {
                    if(Aoto_program_flag==1)
                    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_SELF_PROG_RO_ADDS,0x0001,0x00);   //read status  
       	            else
                    Set_tx_cmd_data_finc(0xd1,0x03,U_DRIVER_SELF_PROG_RO_ADDS,0x0001,0x00);   //read status         	            
       	            ++p_step_cnt;         	            
       	         }
       	     break;
       	     case 3:
       	         if(Rx_TX_flag==0)
       	         {
                    i=U_data_buff[4];			//get status bit data  
                    i&=0x0f; 
                    P_status_temp|=i; 
                    i&=0x02; 
                    if(i==0x02)	
                    {
                       ++p_step_cnt;		//check erase finish   
                       
                       Set_en_buzz_finc(1,4,400,200);
                       Disp_updata_status(3); 
                    }                    
                    else
                    {
                       p_step_cnt=2;   
                       Pro_wait_time_set=50;		//wait 1sec
                       Pro_wait_time_flag=1;
                       ++P_read_status_cnt;
                       if(P_read_status_cnt==20)
                       {
                       	  P_read_status_cnt=0;
                       	  Aoto_program_flag=p_step_cnt=0;	//stop program
                       	  Set_en_buzz_finc(6,4,200,100);

       	                  Disp_updata_status(4);
                       }  
                    }
       	         }
       	     break;
       	     case 4:					//send file data
       	         if(Rx_TX_flag==0)
       	         {
             	     
             	     if(p_packge_tx_flag==0)
             	     {
             	        p_packge_tx_flag=1;
             	        
             	        Check_file_flag=0;
             	        Get_bin_data();
       	                if(Read_file_flag==1)
       	                {
       	                    Aoto_program_flag=p_step_cnt=0;  
       	                    Set_en_buzz_finc(6,4,200,100);
       	                    
       	                    Disp_updata_status(1);
       	                }
             	        else
             	        {             	                     	                   	     
/////             	            if(Aoto_program_flag==1)
/////             	            Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x10,U_SENSER_SELF_PROG_DATA_WO_ADDS+P_cmd_adds,0x0020,0x40); 
/////             	            else
/////             	            Set_tx_cmd_data_finc(0xd1,0x10,U_DRIVER_SELF_PROG_DATA_WO_ADDS+P_cmd_adds,0x0020,0x40);              	     
  
                            Uart_tx_rx_fifo_int_FLAG&=0xf9;	//clear bit2,3
                            U_RX_TX_byte_lenth=73;
                            DrvGPIO_ClrBit (E_GPB,6); 
                            Com1_rd_ctrl(1);
                            U_Rx_wait_time_flag_temp=0; 
                            U_RX_TX_byte_lenth_cnt=1;
                            DrvUART_Write(UART_PORT1,U_data_buff,1);
                            En_Uart012_THRE_int(UART_PORT1);
                            
                            k=GPIOB->DOUT;
                            k&=0x00000800;
                            if(k==0x00000800)
                            {
                               k=GPIOB->DOUT;
                               k|=0x00000800;
                               GPIOB->DOUT=k;
                            }   
                            Rx_TX_flag=1;                            
             	            ++P_cmd_adds;  
             	        }           	        
             	     }
             	     ++p_step_cnt;
                     Pro_wait_time_set=10;		//wait 1sec
                     Pro_wait_time_flag=1;    	        
		 }       	         
       	     break;
       	     case 5:
       	         if((Rx_TX_flag==0)&&(Pro_wait_time_flag==2))
       	         {
             	     if(Aoto_program_flag==1)
                     Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_SELF_PROG_RO_ADDS,0x0001,0x00);   //read status  
       	             else
                     Set_tx_cmd_data_finc(0xd1,0x03,U_DRIVER_SELF_PROG_RO_ADDS,0x0001,0x00);   //read status  
                     
                     ++p_step_cnt;       	               
       	         }
       	     break;
       	     case 6:
       	         if(Rx_TX_flag==0)
       	         {                    
                    i=U_data_buff[4];			//get status bit data  
                    i&=0x0f; 
                    P_status_temp|=i; 
                    i&=0x03; 
                    if(i==0x03)				//check send data finish
                    {
                       ++p_step_cnt;                        
                       P_cmd_adds=0;
                    }
                    else if(P_cmd_adds==192)
                    {
                       Pro_wait_time_set=50;		//wait 1sec
                       Pro_wait_time_flag=1;
                       p_step_cnt=5;

                       ++P_read_status_cnt;
                       if(P_read_status_cnt==20)
                       {
                       	  P_read_status_cnt=0;
                       	  Aoto_program_flag=p_step_cnt=0;	//stop program
                       	  Set_en_buzz_finc(6,4,200,100);
                       	  
                          Disp_updata_status(4);                       	  
                       }  
                    }                                                             
                    else
                    {
                    	 p_step_cnt=4;			//goon next packge      
                    	 p_packge_tx_flag=0;
                    }
       	         }
       	     break;
       	     case 7:					//send program cmd
       	         if(Rx_TX_flag==0)
       	         {                                     
                    P_cmd_adds=0;
                    if(Aoto_program_flag==1)
                    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x06,U_SENSER_SELF_REG_CTRL_WO_ADDS,0x005A,0x00);  //program flash   
       	            else
                    Set_tx_cmd_data_finc(0xd1,0x06,U_DRIVER_SELF_REG_CTRL_WO_ADDS,0x005A,0x00);  //program flash          	         
       	            
       	            ++p_step_cnt; 
                    Pro_wait_time_set=1000;		//wait 5sec
       	            Pro_wait_time_flag=1;  
       	         }    	     
       	     break;
       	     case 8:					//read and check program finish
       	         if((Rx_TX_flag==0)&&(Pro_wait_time_flag==2))
       	         {
                    if(Aoto_program_flag==1)
                    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_SELF_PROG_RO_ADDS,0x0001,0x00);   //read status  
       	            else
       	            Set_tx_cmd_data_finc(0xd1,0x03,U_DRIVER_SELF_PROG_RO_ADDS,0x0001,0x00);   //read status 

       	            ++p_step_cnt;  
       	         }
       	     break;
       	     case 9:
       	         if(Rx_TX_flag==0)
       	         {                    
                    i=U_data_buff[4];			//get status bit data  
                    i&=0x0f; 
                    P_status_temp|=i; 
                    i&=0x0C; 
                    if(i==0x0C)				//check program finish
                    {
                       ++p_step_cnt; 
                       P_cmd_adds=0;
                       Aoto_program_flag=0;		//surccess
                       Set_en_buzz_finc(2,6,200,100);
                       
                       Disp_updata_status(5);  
                    }
                    else if(P_read_status_cnt<11)
                    {
                       p_step_cnt=8;
                       Pro_wait_time_set=50;		//wait 1sec
       	               Pro_wait_time_flag=1;  

                       ++P_read_status_cnt;
                       if(P_read_status_cnt==10)
                       {
                       	  P_read_status_cnt=0;
                       	  Aoto_program_flag=p_step_cnt=0;	//stop program
                       	  Set_en_buzz_finc(6,4,200,100);
                       	  
                       	  Disp_updata_status(4);  
                       }  
                    }
                    else p_step_cnt=1;			//redo	                       
                 }
       	     break;       	     
       	  }
       }      
    }
}




