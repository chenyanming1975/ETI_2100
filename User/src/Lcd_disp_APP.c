//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Lcd_disp_APP.c
//	First EDIT		: Sam
//	Data			: 2012.06.08
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================
#define	__LCD8080_AP__

//#define	__THERMO__

//-------------------------------------------------------------------------------
#include "Incude_all.h"
#include "Lcd_data_bace.h"

//-------------------Pub Variable --------------------------
////static int8_t   Seg_temp;
uint8_t Disp_buff128x64[1024];								//显示缓存区 2013-3-18 12:02

uint8_t  User_prio_temp;                               			//用户密码权限,密码输入用的
uint8_t  Check_pass_word_flag;                         			//
uint8_t  Pass_bit_select;                              			//
uint8_t  Pass_adj_cnt;                                 			//
uint32_t Key_input_pass_temp;                          			//

uint8_t Disp_gdgdy_flag;                               			//菜单显示的符号
uint8_t L4_seg_temp;                                   			//
uint8_t LCD_disp_flag;                                 			//
uint8_t Pro_dis_menu_flag;                             			//
uint8_t a_signle_flag;                                 			//校正方程的系数的符号
uint8_t b_signle_flag;                                 			//
uint8_t c_signle_flag;                                 			//
uint8_t d_signle_flag;                                 			//
uint32_t back_l_temp;                                  			//
uint8_t Rx_TX_flag_local;                              			//

uint8_t const Program_ver[]= {'V','2','1','0','0','E','T','P','0','0','5','T','B',};	//程序版本号
//uint8_t const Program_series[]={'2','0','1','6','0','9','1','8',0,0,0,0,0,0};	        //序列号

uint8_t const Position_bit[]= {1,4,6,2,8,2,3,1,4,                                       //菜单7的位数
                               // 13,5,5,3,3,1,4,3,4,6,2,4,4,5,5,5,                         //菜单2的位数
                               16,5,5,3,3,1,4,3,4,6,2,4,4,5,5,5,                         //菜单2的位数
                               2,2,5,4,3,2,2,3,4,5,4,5,4,2,2,2,2,7,7,7                   //菜单3的位数
                              };

//uint8_t const Lever41_position_bit[]={1,4,6,2,8,1,1,1,4,4,5,3,2,2,5,4,3,2,2,3,4,5,4,5,4,2,2,2,3,5,2,
//                                      3,3,1,3,3,
//                                     };

/*===============================================================================
//	finction	:Tx_cmd_to_lcm	:LCM use 8080 system
//	input		:cmd
//	edit		:sam 2012.06.11
//	description:发送命令到LCD
//===============================================================================*/
void Tx_cmd_to_lcm(int32_t data)
{
    uint32_t k;

    DrvGPIO_Open (E_GPA,15, E_IO_OUTPUT);			//RD FOR LCD
    outpw(&SYS->GPAMFP, inpw(&SYS->GPAMFP) & ~(0x3<<14));	//2013-8-8 13:01
    GPIOA->DMASK=0X00000000;

//    DrvGPIO_ClrBit_new (E_GPB,10);		//set cs low
//    DrvGPIO_SetBit_new (E_GPB,9);		//set rs hight
    GPIOB->DMASK=0X0000F1FF;
    k=GPIOB->DOUT;
    k&=0x0000F9FF;
    GPIOB->DOUT=k;
    DrvGPIO_SetBit_new (E_GPA,15);		//set rd hight
    GPIOC->DMASK=0X0000FEFF;
    k=GPIOC->DOUT;
    k&=0X0000FEFF;
    GPIOC->DOUT=k;				//set wr low PORTC8

    GPIOA->DMASK=0X0000FF00;
    GPIOA->DOUT=data;
    GPIOA->DMASK=0X00000000;

    GPIOC->DOUT|=0x00000100;		//set wr hight PORTC8
    GPIOC->DMASK=0X00000000;
//    DrvGPIO_SetBit_new (E_GPA,15);		//set rd hight
    k=GPIOB->DOUT;
    k&=0x0000F9FF;
    k|=0x00000600;
    GPIOB->DOUT=k;				//set cs hight
    GPIOB->DMASK=0X00000000;
}

/*===============================================================================
//	finction:Tx_data_to_lcm	:LCM use 8080 system
//	input		:data
//	edit		:sam 2012.06.11
//	description:发送数据到LCD
//===============================================================================*/
void Tx_data_to_lcm(uint8_t data)
{
    uint32_t k;
	
    GPIOB->DMASK=0X0000F1FF;
    k=GPIOB->DOUT;
    k&=0x0000F9FF;
    k|=0x00000200;
    GPIOB->DOUT=k;

    DrvGPIO_SetBit_new (E_GPA,15);		//set rd hight
    GPIOC->DMASK=0X0000FEFF;
    k=GPIOC->DOUT;
    k&=0X0000FEFF;
    GPIOC->DOUT=k;				//set wr low PORTC8

    GPIOA->DMASK=0X0000FF00;
    GPIOA->DOUT=data;
    GPIOA->DMASK=0X00000000;

    GPIOC->DOUT|=0x00000100;		//set wr hight PORTC8
    GPIOC->DMASK=0X00000000;
//	DrvGPIO_SetBit_new (E_GPA,15);		//set rd hight
    k=GPIOB->DOUT;
    k&=0x0000F9FF;
    k|=0x00000600;
    GPIOB->DOUT=k;				//set cs hight
    GPIOB->DMASK=0X00000000;
}

/*===============================================================================
//	finction	:Tx_data_to_lcm_p	:LCM use 8080 system
//	input		:data
//	edit		:sam 2012.06.11
//	description:指针方式发送数据到LCD
//===============================================================================*/
void Tx_data_to_lcm_p(uint8_t const *data)
{
    uint32_t k;
//    DrvGPIO_ClrBit_new (E_GPB,10);		//set cs low
//    DrvGPIO_SetBit_new (E_GPB,9);		//set rs hight
    GPIOB->DMASK=0X0000F1FF;
    k=GPIOB->DOUT;
    k&=0x0000F9FF;
    k|=0x00000200;
    GPIOB->DOUT=k;

    DrvGPIO_SetBit_new (E_GPA,15);		//set rd hight
    GPIOC->DMASK=0X0000FEFF;
    k=GPIOC->DOUT;
    k&=0X0000FEFF;
    GPIOC->DOUT=k;				//set wr low PORTC8

    GPIOA->DMASK=0X0000FF00;
    GPIOA->DOUT=*data;
    GPIOA->DMASK=0X00000000;

    GPIOC->DOUT|=0x00000100;		//set wr hight PORTC8
    GPIOC->DMASK=0X00000000;
//	DrvGPIO_SetBit_new (E_GPA,15);		//set rd hight
    k=GPIOB->DOUT;
    k&=0x0000F9FF;
    k|=0x00000600;
    GPIOB->DOUT=k;				//set cs hight
    GPIOB->DMASK=0X00000000;
}

/*===============================================================================
//	finction	:Tx_data_to_lcm_del	:LCM use 8080 system
//	input		:data
//	edit		:sam 2012.06.11
//	description:将数据取反发送到lCD
//===============================================================================*/
void Tx_data_to_lcm_del(uint8_t const *data)
{
    uint32_t k;
//    DrvGPIO_ClrBit_new (E_GPB,10);		//set cs low
//    DrvGPIO_SetBit_new (E_GPB,9);		//set rs hight
    GPIOB->DMASK=0X0000F1FF;
    k=GPIOB->DOUT;
    k&=0x0000F9FF;
    k|=0x00000200;
    GPIOB->DOUT=k;

    DrvGPIO_SetBit_new (E_GPA,15);		//set rd hight
    GPIOC->DMASK=0X0000FEFF;
    k=GPIOC->DOUT;
    k&=0X0000FEFF;
    GPIOC->DOUT=k;				//set wr low PORTC8

    GPIOA->DMASK=0X0000FF00;
    GPIOA->DOUT=~(*data);
    GPIOA->DMASK=0X00000000;

    GPIOC->DOUT|=0x00000100;		//set wr hight PORTC8
    GPIOC->DMASK=0X00000000;
//	DrvGPIO_SetBit_new (E_GPA,15);		//set rd hight
    k=GPIOB->DOUT;
    k&=0x0000F9FF;
    k|=0x00000600;
    GPIOB->DOUT=k;				//set cs hight
    GPIOB->DMASK=0X00000000;
}

/*===============================================================================
//	finction	:Clear a page	:LCM use 8080 system
//	input		:Page_addrs	0~8
//	edit		:sam 2012.06.11
//	description:清除1页
//===============================================================================*/
void Clear_apage(uint8_t Page_addrs)
{
    uint8_t i;

    Tx_cmd_to_lcm(CMD_PAGE_ADDS_SET+Page_addrs);//设置地址，一个画面64行分成8页。
    Tx_cmd_to_lcm(CMD_LADDS_HI);//设置列地址的高4位
    Tx_cmd_to_lcm(CMD_LADDS_LOW);//设置列地址的低4位
    for(i=0; i<=128; i++)
    {
        Tx_data_to_lcm(0X00);
    }
}

/*===============================================================================
//	finction	:Clear_n_page	:LCM use 8080 system
//	input		:Page_addrs	0~8
//	edit		:sam 2012.06.11
//	description:清除1页和对应页的显示区
//===============================================================================*/
void Clear_n_page(uint8_t start,uint8_t Page_addrs)
{
    uint8_t i,j;

    for(j=start; j<=(Page_addrs+start); j++)
    {
        Tx_cmd_to_lcm(CMD_PAGE_ADDS_SET+j);
        Tx_cmd_to_lcm(CMD_LADDS_HI);
        Tx_cmd_to_lcm(CMD_LADDS_LOW);
        for(i=0; i<=128; i++)
        {
            Tx_data_to_lcm(0X00);
            Disp_buff128x64[j*128+i]=0;
        }
    }
}

/*===============================================================================
//	finction	:Clear_n_page_ram	:LCM use 8080 system
//	input		:Page_addrs	0~8
//	edit		:sam 2012.06.11
//	description:清除多页显示区
//===============================================================================*/
void Clear_n_page_ram(uint8_t start,uint8_t Page_addrs)
{
    uint8_t i,j;
    for(i=start; i<=(Page_addrs+start); i++)
    {
        for(j=0; j<128; j++)
            Disp_buff128x64[i*128+j]=0;
    }
}

/*===============================================================================
//	finction	:Clear 8*8 area 	:LCM use 8080 system
//	input		:Page_addrs	0~8
//			     Seg_addrs_h	0~8
//			     Seg_addrs_l	0~15
//	edit		:sam 2012.06.12
//	description:清除一个8×8的区域，未使用
//===============================================================================*/
void Clear_8x8_area(uint8_t Page_addrs,uint8_t Seg_addrs_h,uint8_t Seg_addrs_l)
{
    uint8_t i;

    Tx_cmd_to_lcm(CMD_PAGE_ADDS_SET+Page_addrs);
    Tx_cmd_to_lcm(CMD_LADDS_HI+Seg_addrs_h);
    Tx_cmd_to_lcm(CMD_LADDS_LOW+Seg_addrs_l);
    for(i=0; i<8; i++)
    {
        Tx_data_to_lcm(0X00);
    }
}


/*===============================================================================
//	finction	:Disp 8*8 area 	:LCM use 8080 system
//	input		:Page_addrs	0~8
//			 Seg_addrs_h	0~7
//			 Seg_addrs_l	0~15
//			 *address       data addrs
//	output		:null
//	return		:null
//	edit		:sam 2012.06.11
//	description:未使用函数
//===============================================================================*/
void Disp_8x8_area(uint8_t Page_addrs,uint8_t Seg_addrs_h,uint8_t Seg_addrs_l,uint8_t const *address)
{
    uint8_t i;

    Tx_cmd_to_lcm(CMD_PAGE_ADDS_SET+Page_addrs);
    Tx_cmd_to_lcm(CMD_LADDS_HI+Seg_addrs_h);
    Tx_cmd_to_lcm(CMD_LADDS_LOW+Seg_addrs_l);
    for(i=0; i<8; i++)
    {
        Tx_data_to_lcm(*address);
        address++;
    }
}

/*===============================================================================
//	finction	:Adj_lcd_Contrast_data	:LCM use 8080 system
//	input		:null     //Contrast--->0~63
//	edit		:sam 2012.06.11
//description:调整对比度，未使用函数
//===============================================================================*/
void Adj_lcd_Contrast_data(void)
{
    uint8_t i,j,LCD_Contrast;

    i=M_menur1_lcd_Contrast_temp>>4;
    i&=0x0f;
    i*=10;
    j=M_menur1_lcd_Contrast_temp;
    j&=0x0f;
    LCD_Contrast=i+j;

    Tx_cmd_to_lcm(0x23);			//粗调对比度

    Tx_cmd_to_lcm(CMD_INTER_LCD_VOL_SET);
    Tx_cmd_to_lcm(LCD_Contrast);			//微调对比度
}

/*===============================================================================
//	finction	:Lcd initial	:LCM use 8080 system
//	edit		:sam 2012.06.11
//	description:LCD初始化
//===============================================================================*/
void Lcd_initial(void)
{
    InitPWM_4();
//    DrvGPIO_SetBit_new (E_GPA,14);			//CONT set HIGHT

    DrvGPIO_ClrBit_new (E_GPB,3);			//HW reset set low
    DrvSYS_Delay(300000);			//delay 300ms
    DrvGPIO_SetBit_new (E_GPB,3);			//reset lcm
    DrvSYS_Delay(10000);			//delay 10ms

    Tx_cmd_to_lcm(CMD_FW_RESET);//软复位
    DrvSYS_Delay(10000);			//delay 10ms

    Tx_cmd_to_lcm(0x2c);      //升压步骤1
    DrvSYS_Delay(10000);			
    Tx_cmd_to_lcm(0x2e);      //升压步骤2
    DrvSYS_Delay(10000);			
    Tx_cmd_to_lcm(0x2f);      //升压步骤3
    DrvSYS_Delay(10000);			

    Tx_cmd_to_lcm(0x23);			//粗调对比度，范围0x20 - 0x27
    Tx_cmd_to_lcm(CMD_INTER_LCD_VOL_SET);//微调对比度
    Tx_cmd_to_lcm(0x36);			//微调对比度的值， 范围0x00-0x3f
    M_menur1_lcd_Contrast_temp=0x36;

    Tx_cmd_to_lcm(CMD_SET_BIAS_VOL9);		//1/9 偏压比
    Tx_cmd_to_lcm(CMD_REV_LINE_SCAN);		//行扫描顺序：从上到下
    Tx_cmd_to_lcm(CMD_DIS_ADDS_L2R);		//列扫描顺序：从左到右
    Tx_cmd_to_lcm(CMD_MORMAL_DISP);     //常规显示
    Tx_cmd_to_lcm(CMD_DIS_START_LINE);	//起始行：第一行
    Tx_cmd_to_lcm(CMD_LCM_ON);          //开显示

    Clear_n_page(0,7);
    Menu_lever_select=0;
    Menu_index_select=1;
    Menu_index_finc_select=1;
    Menu_lever1_point=1;
    Menu_lever2_point=1;
    Menu_lever3_point=1;
    Menu_lever3_select=1;

}

/*===============================================================================
//	finction	:Display_seg_single	:LCM use 8080 system
//	input		:null
//	edit		:sam 2012.07.04
// descriptiong:在弟page_adds页，第seg_adds列显示某一点数据
//===============================================================================*/
void Display_seg_single(uint8_t page_adds,uint8_t seg_adds,uint8_t data)
{
    uint8_t n,m;

    m=seg_adds%16;//取低4位
    n=seg_adds/16;//取高4位

    Tx_cmd_to_lcm(CMD_PAGE_ADDS_SET+page_adds); 	//set page address total 8page from 0~7
    Tx_cmd_to_lcm(CMD_LADDS_HI+n);//设置列地址高4位
    Tx_cmd_to_lcm(CMD_LADDS_LOW+m);//设置列地址低4位
    Tx_data_to_lcm(data);
}

/*===============================================================================
//	finction	:Disp_apage	:LCM use 8080 system
//	input		:Page_addrs	0~8
//          *address       data addrs
//	edit		:sam 2012.06.11
//	modefy		:null
//  description: display a page of 128*8 dots, 未使用函数
//===============================================================================*/
void Disp_apage(uint8_t Page_addrs,uint8_t const *address)
{
    uint8_t j;

    Tx_cmd_to_lcm(CMD_PAGE_ADDS_SET+Page_addrs); 	//set page address total 8page from 0~7
    Tx_cmd_to_lcm(CMD_LADDS_HI);
    Tx_cmd_to_lcm(CMD_LADDS_LOW);
    for(j=0; j<=128; j++)			//lie addrs total 128 lie from 0~127
    {
        Tx_data_to_lcm(*address);
        address++;
    }
}

/*===============================================================================
//	finction	:Display_logo_pic	:LCM use 8080 system
//	input		:null
//	edit		:sam 2013-3-12 14:01
//  description:显示公司LOGO,未使用
//===============================================================================*/
void Display_logo_pic(uint8_t const *address)
{
    uint8_t i,j;
    for(i=0; i<8; i++)
    {
        Tx_cmd_to_lcm(CMD_PAGE_ADDS_SET+i); 	//set page address total 8page from 0~7
        Tx_cmd_to_lcm(CMD_LADDS_HI);
        Tx_cmd_to_lcm(CMD_LADDS_LOW);
        for(j=0; j<128; j++)			//lie addrs total 128 lie from 0~127
        {
            Tx_data_to_lcm(*address);
            address++;
        }
    }
}

/*===============================================================================
//	finction	:Display_all_buff	:LCM use 8080 system
//	edit		:sam 2013-3-19 16:05
// description:显示Disp_buff128x64数组中的值
//===============================================================================*/
void Display_all_buff(void)
{
    uint8_t i,j;
    for(i=0; i<8; i++)
    {
        Tx_cmd_to_lcm(CMD_PAGE_ADDS_SET+i); 	//set page address total 8page from 0~7
        Tx_cmd_to_lcm(CMD_LADDS_HI);
        Tx_cmd_to_lcm(CMD_LADDS_LOW);
        for(j=0; j<128; j++)			//lie addrs total 128 lie from 0~127
            Tx_data_to_lcm(Disp_buff128x64[i*128+j]);
    }
}

/*===============================================================================
//	finction	:rev_clear_set_arear	:LCM use 8080 system
//	input		  :null
//	edit		  :sam 2013-4-8 17:20
//	modefy		:null
//  description:
//===============================================================================*/
uint8_t rev_clear_set_arear(uint8_t c_page, uint8_t c_seg, uint8_t c_lenth, uint8_t s_page, uint8_t s_seg, uint8_t s_lenth)
{
    uint8_t i;

    for(i=0; i<c_lenth+1; i++)
    {
        Disp_buff128x64[(c_page-0)*128 + c_seg + i] = ~Disp_buff128x64[(c_page-0)*128+c_seg+i];
        Disp_buff128x64[(c_page+1)*128 + c_seg + i] &= ~0x01;
    }

    for(i=0; i<s_lenth+1; i++)
    {
        Disp_buff128x64[(s_page-0)*128+s_seg+i] = ~Disp_buff128x64[(s_page-0)*128+s_seg+i];
        Disp_buff128x64[(s_page+1)*128+s_seg+i] |= 0x01;
    }

    Display_all_buff();
    Seg_rev_lenth = s_lenth;

    return(s_seg);
}

/*===============================================================================
//	finction	:rev_clear_set_arear16	:LCM use 8080 system
//	input		  :null
//	edit		  :sam 2013-4-8 17:20
//	description:反显区域
//===============================================================================*/
uint8_t rev_clear_set_arear16(uint8_t c_page,uint8_t c_seg,uint8_t c_lenth,uint8_t s_page,uint8_t s_seg,uint8_t s_lenth)
{
    uint8_t i;

    for(i=0; i<c_lenth+1; i++)
    {
        Disp_buff128x64[(c_page-0)*128+c_seg+i]=~Disp_buff128x64[(c_page-0)*128+c_seg+i];
        Disp_buff128x64[(c_page+1)*128+c_seg+i]=~Disp_buff128x64[(c_page+1)*128+c_seg+i];
    }

    for(i=0; i<s_lenth+1; i++)
    {
        Disp_buff128x64[(s_page-0)*128+s_seg+i]=~Disp_buff128x64[(s_page-0)*128+s_seg+i];
        Disp_buff128x64[(s_page+1)*128+s_seg+i]=~Disp_buff128x64[(s_page+1)*128+s_seg+i];
    }

    Display_all_buff();
    Seg_rev_lenth=s_lenth;

    return(s_seg);
}

/*===============================================================================
//	finction	:Get_ascii_data	:LCM use 8080 system
//	input		:null
//	output		:null
//	edit		:sam 2012.07.04
//	description:ASCII码转换
//===============================================================================*/
uint8_t Get_ascii_data(uint8_t data)
{
    uint8_t i;

    i=data;
    switch(data)
    {
    case 0:
        i='0';
        break;
    case 1:
        i='1';
        break;
    case 2:
        i='2';
        break;
    case 3:
        i='3';
        break;
    case 4:
        i='4';
        break;
    case 5:
        i='5';
        break;
    case 6:
        i='6';
        break;
    case 7:
        i='7';
        break;
    case 8:
        i='8';
        break;
    case 9:
        i='9';
        break;
    case 0x0A:
        i='A';
        break;
    case 0x0B:
        i='B';
        break;
    case 0x0C:
        i='C';
        break;
    case 0x0D:
        i='D';
        break;
    case 0x0E:
        i='E';
        break;
    case 0x0F:
        i='F';
        break;
    }
    return(i);
}

/*===============================================================================
//	finction	:Display_8x16_char	:LCM use 8080 system
//	input		  :null
//	edit		  :sam 2012.07.04
//	description:从第page_adds页第seg_adds列开始显示total_num个data_buff数组中的数据
//===============================================================================*/
void Display_6x8_char(uint8_t page_adds,uint8_t seg_adds,uint8_t total_num,uint8_t data_buff[])
{
    uint16_t cnt=0;
    uint8_t i,j,n,m,k;
    uint8_t *index;

    for(i=0; i<total_num; i++)
    {
        if(data_buff[i]<48)
        {
            cnt=data_buff[i]*6;
        }
        if(data_buff[i]>=65)
        {
            cnt=(data_buff[i]-44)*6;
        }
        else if(data_buff[i]>=48)
        {
            cnt=(data_buff[i]-47)*6;
        }
        index=Buff_68+cnt;

        k=i*6+seg_adds;
        m=k%16;
        n=k/16;
        Tx_cmd_to_lcm(CMD_PAGE_ADDS_SET+page_adds); 	//set page address total 8page from 0~7
        Tx_cmd_to_lcm(CMD_LADDS_HI+n);
        Tx_cmd_to_lcm(CMD_LADDS_LOW+m);
        for(j=0; j<6; j++)				//lie addrs total 128 lie from 0~127
        {
            Tx_data_to_lcm_p(index);
            index++;
        }
    }
}

/*===============================================================================
//	finction	:Get_6x8_char_single	:LCM use 8080 system
//	input		  :null
//	edit		  :sam 2012.07.04
//	description:将生产的6×8的数据写入到Disp_buff128x64数组当中
//===============================================================================*/
void Get_6x8_char_single(uint8_t page_adds,uint8_t seg_adds,uint8_t data)
{
    uint16_t cnt=0;
    uint8_t j;
    uint8_t *index;

    if(data<48)
    {
        cnt=data*6;
    }
    if(data>=65)
    {
        cnt=(data-44)*6;
    }
    else if(data>=48)
    {
        cnt=(data-47)*6;
    }
    index=Buff_68+cnt;
    for(j=0; j<6; j++)				//lie addrs total 128 lie from 0~127
    {
        Disp_buff128x64[page_adds*128+seg_adds+j]=*index;
        index++;
    }
}

/*===============================================================================
//	finction	:Display_6x8_char_single	:LCM use 8080 system
//	input		:null
//	edit		:sam 2012.07.04
//	description:将6×8格式的数据显示在液晶屏上
//===============================================================================*/
void Display_6x8_char_single(uint8_t page_adds,uint8_t seg_adds,uint8_t data)
{
    uint16_t cnt=0;
    uint8_t j,n,m;
    uint8_t *index;


    if(data<48)
    {
        cnt=data*6;
    }
    if(data>=65)
    {
        cnt=(data-44)*6;
    }
    else if(data>=48)
    {
        cnt=(data-47)*6;
    }
    index=Buff_68+cnt;

    m=seg_adds%16;
    n=seg_adds/16;

    Tx_cmd_to_lcm(CMD_PAGE_ADDS_SET+page_adds); 	//set page address total 8page from 0~7
    Tx_cmd_to_lcm(CMD_LADDS_HI+n);
    Tx_cmd_to_lcm(CMD_LADDS_LOW+m);
    for(j=0; j<6; j++)				//lie addrs total 128 lie from 0~127
    {
        Tx_data_to_lcm_p(index);
        index++;
    }
}

/*===============================================================================
//	finction	:Display_8x16_char	:LCM use 8080 system
//	input		:null
//	edit		:sam 2012.07.04
//	description:以8×16的格式显示字符
//===============================================================================*/
void Display_8x16_char(uint8_t page_adds,uint8_t seg_adds,uint8_t total_num,uint8_t data_buff[])
{
    uint16_t cnt=0;
    uint8_t i,j,n,m,k;
    uint8_t *index;

    for(i=0; i<total_num; i++)
    {

        if(data_buff[i]<48)
        {
            cnt=data_buff[i]*20;
        }
        if(data_buff[i]>=65)
        {
            cnt=(data_buff[i]-51)*20;
        }
        else if(data_buff[i]>=48)
        {
            cnt=(data_buff[i]-47)*20;
        }

        index=Buff_816+cnt;

        k=i*10+seg_adds;
        m=k%16;
        n=k/16;
        Tx_cmd_to_lcm(CMD_PAGE_ADDS_SET+page_adds); 	//set page address total 8page from 0~7
        Tx_cmd_to_lcm(CMD_LADDS_HI+n);
        Tx_cmd_to_lcm(CMD_LADDS_LOW+m);
        for(j=0; j<10; j++)				//lie addrs total 128 lie from 0~127
        {
            Tx_data_to_lcm_p(index);
            index++;
        }
        Tx_cmd_to_lcm(CMD_PAGE_ADDS_SET+page_adds+1); 	//set page address total 8page from 0~7
        Tx_cmd_to_lcm(CMD_LADDS_HI+n);
        Tx_cmd_to_lcm(CMD_LADDS_LOW+m);
        for(j=0; j<10; j++)				//lie addrs total 128 lie from 0~127
        {
            Tx_data_to_lcm_p(index);
            index++;
        }
    }
}

//===============================================================================
//	finction	:Display_8x16_char_single	:LCM use 8080 system
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.07.04
//	modefy		:8×16的方式显示一个byte的数据
//===============================================================================
void Display_8x16_char_single(uint8_t page_adds,uint8_t seg_adds,uint8_t data)
{
    uint16_t cnt=0;
    uint8_t j,n,m;
    uint8_t *index;


    if(data<48)
    {
        cnt=data*20;
    }
    if(data>=65)
    {
        cnt=(data-51)*20;
    }
    else if(data>=48)
    {
        cnt=(data-47)*20;
    }

    index=Buff_816+cnt;

    m=seg_adds%16;
    n=seg_adds/16;

    Tx_cmd_to_lcm(CMD_PAGE_ADDS_SET+page_adds); 	//set page address total 8page from 0~7
    Tx_cmd_to_lcm(CMD_LADDS_HI+n);
    Tx_cmd_to_lcm(CMD_LADDS_LOW+m);
    for(j=0; j<10; j++)				//lie addrs total 128 lie from 0~127
    {
        Tx_data_to_lcm_p(index);
        index++;
    }
    Tx_cmd_to_lcm(CMD_PAGE_ADDS_SET+page_adds+1); 	//set page address total 8page from 0~7
    Tx_cmd_to_lcm(CMD_LADDS_HI+n);
    Tx_cmd_to_lcm(CMD_LADDS_LOW+m);
    for(j=0; j<10; j++)				//lie addrs total 128 lie from 0~127
    {
        Tx_data_to_lcm_p(index);
        index++;
    }
}

//===============================================================================
//	finction	:Get_8x16_char_single	:LCM use 8080 system
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.07.04
//	modefy		:null
//===============================================================================
void Get_8x16_char_single(uint8_t page_adds,uint8_t seg_adds,uint8_t data)
{
    uint16_t cnt=0;
    uint8_t j;
    uint8_t *index;


    if(data<48)
    {
        cnt=data*20;
    }
    if(data>=65)
    {
        cnt=(data-51)*20;
    }
    else if(data>=48)
    {
        cnt=(data-47)*20;
    }
//	if(data>=97)
//	{
//	   cnt=(data-54)*16;
//	}

    index=Buff_816+cnt;
    for(j=0; j<10; j++)				//lie addrs total 128 lie from 0~127
    {
        Disp_buff128x64[page_adds*128+seg_adds+j]=*index;
        index++;
    }
    for(j=0; j<10; j++)				//lie addrs total 128 lie from 0~127
    {
        Disp_buff128x64[(page_adds+1)*128+seg_adds+j]=*index;
        index++;
    }
}

//===============================================================================
//	finction	:Get_16x16_char_single	:LCM use 8080 system
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.07.04
//	modefy		:null
//===============================================================================
void Get_16x16_char_single(uint8_t page_adds,uint8_t seg_adds,uint8_t data)
{
    volatile uint32_t adds_a,temp;
    volatile uint16_t byte;
    volatile uint8_t i,j,k,mm,page;
    uint8_t *index;

    if(data>=11) i=12;
    else         i=16;

    if(data==0)
    {
        for(j=0; j<16; j++)				//lie addrs total 128 lie from 0~127
            Disp_buff128x64[page_adds*128+seg_adds+j]=0x00;

        for(j=0; j<16; j++)				//lie addrs total 128 lie from 0~127
            Disp_buff128x64[(page_adds+1)*128+seg_adds+j]=0x00;
    }
    else if(data>=220)
    {
        j=0;
        if(data<230)
        {
            data-=219;
            j=1;
        }
        else data-=216;
        Get_8x16_char_single(page_adds,seg_adds+j,data);
    }
    else
    {
        if(data<=10)
        {
            page=0;
            byte=data*0x20;
        }
        else
        {
            temp=320+(data-11)*24;
            byte=temp;
            page=temp/512;
            byte%=512;
        }

        adds_a=DATA_HZ_START_ADDS;
        adds_a>>=16;
        adds_a&=0x0000ffff;				//page adds
        adds_a+=page;
        Spi_read(adds_a,buff_page);			//16x16 data

        if((byte+24)>512)
        {
            mm=512-byte;
            index=buff_page+byte;
            if(mm==12)
            {
                for(j=0; j<mm; j++)				//lie addrs total 128 lie from 0~127
                {
                    Disp_buff128x64[page_adds*128+seg_adds+j]=*index;
                    index++;
                }
                k=seg_adds+mm;
                mm=24-mm;
                ++adds_a;
                Spi_read(adds_a,buff_page);			//16x16 data
                index=buff_page+0;
                for(j=0; j<mm; j++)				//lie addrs total 128 lie from 0~127
                {
                    Disp_buff128x64[(page_adds+1)*128+k+j]=*index;
                    index++;
                }
            }
            else if(mm>12)
            {
                for(j=0; j<12; j++)				//lie addrs total 128 lie from 0~127
                {
                    Disp_buff128x64[page_adds*128+seg_adds+j]=*index;
                    index++;
                }
                for(j=0; j<mm-12; j++)				//lie addrs total 128 lie from 0~127
                {
                    Disp_buff128x64[(page_adds+1)*128+seg_adds+j]=*index;
                    index++;
                }
                k=seg_adds+mm-12;
                mm=24-mm;
                ++adds_a;
                Spi_read(adds_a,buff_page);			//16x16 data
                index=buff_page+0;
                for(j=0; j<mm; j++)				//lie addrs total 128 lie from 0~127
                {
                    Disp_buff128x64[(page_adds+1)*128+k+j]=*index;
                    index++;
                }
            }
            else
            {
                for(j=0; j<mm; j++)				//lie addrs total 128 lie from 0~127
                {
                    Disp_buff128x64[page_adds*128+seg_adds+j]=*index;
                    index++;
                }
                ++adds_a;
                Spi_read(adds_a,buff_page);			//16x16 data
                index=buff_page+0;
                for(j=0; j<12-mm; j++)				//lie addrs total 128 lie from 0~127
                {
                    Disp_buff128x64[page_adds*128+seg_adds+mm+j]=*index;
                    index++;
                }
                for(j=0; j<12; j++)				//lie addrs total 128 lie from 0~127
                {
                    Disp_buff128x64[(page_adds+1)*128+seg_adds+j]=*index;
                    index++;
                }
            }
        }
        else
        {
            index=buff_page+byte;
            for(j=0; j<i; j++)				//lie addrs total 128 lie from 0~127
            {
                Disp_buff128x64[page_adds*128+seg_adds+j]=*index;
                index++;
            }
            for(j=0; j<i; j++)				//lie addrs total 128 lie from 0~127
            {
                Disp_buff128x64[(page_adds+1)*128+seg_adds+j]=*index;
                index++;
            }
        }
        if(data>=11)
        {
            for(j=0; j<i; j++)
            {
                Disp_buff128x64[(page_adds+1)*128+seg_adds+j]<<=3;
                Disp_buff128x64[(page_adds+1)*128+seg_adds+j]&=0xf8;
                k=Disp_buff128x64[page_adds*128+seg_adds+j];
                Disp_buff128x64[page_adds*128+seg_adds+j]<<=3;
                k>>=5;
                k&=0x07;
                Disp_buff128x64[(page_adds+1)*128+seg_adds+j]|=k;
            }
        }
    }
}

//===============================================================================
//	finction	:Disp_rtc_clock	:LCM use 8080 system
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.07.04
//	modefy		:null
//===============================================================================
void Disp_rtc_clock(void)			//显示时间
{
    uint8_t j,data_buff[22];

    j=RTC_DATA.hour_temp;
    j=j>>4;
    j&=0x03;
    data_buff[0]=Get_ascii_data(j);
    j=RTC_DATA.hour_temp;
    j&=0x0f;
    data_buff[1]=Get_ascii_data(j);
    data_buff[2]=20;
    j=RTC_DATA.minu_temp;
    j=j>>4;
    j&=0x07;
    data_buff[3]=Get_ascii_data(j);
    j=RTC_DATA.minu_temp;
    j&=0x0f;
    data_buff[4]=Get_ascii_data(j);

    data_buff[5]=0;
    data_buff[6]=0;
    data_buff[7]=0;
    data_buff[8]=0;
    data_buff[9]=0;
    data_buff[10]=0;
    data_buff[11]='2';
    data_buff[12]='0';
    data_buff[21]=0;
    j=RTC_DATA.year_temp;
    j=j>>4;
    j&=0x03;
    data_buff[13]=Get_ascii_data(j);
    j=RTC_DATA.year_temp;
    j&=0x0f;
    data_buff[14]=Get_ascii_data(j);
    data_buff[15]=14;
    j=RTC_DATA.mouth_temp;
    j=j>>4;
    j&=0x01;
    data_buff[16]=Get_ascii_data(j);
    j=RTC_DATA.mouth_temp;
    j&=0x0f;
    data_buff[17]=Get_ascii_data(j);
    data_buff[18]=14;
    j=RTC_DATA.day_temp;
    j=j>>4;
    j&=0x03;
    data_buff[19]=Get_ascii_data(j);
    j=RTC_DATA.day_temp;
    j&=0x0f;
    data_buff[20]=Get_ascii_data(j);

/////    if((Menu_lever_select==0)||(Auto_check_clock_flag==1))
    if(Menu_lever_select==0)
    {
        Display_6x8_char(7,0,22,data_buff);
    }
}

//===============================================================================
//	finction	:Disp_rtc_clock_wait	:LCM use 8080 system
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-8-24 9:33
//	modefy		:null
//===============================================================================
void Disp_rtc_clock_wait(void)			//自动分析有开始时间未到，显示开始分析时间
{
    uint8_t j,data_buff[16];

    if(prg_wait_clr_flag==0)
    {
        if(prg_wait_clr_flag==0)
            Clear_n_page(0,7);

        prg_wait_clr_flag=1;

        data_buff[0]='S';
        data_buff[1]='T';
        data_buff[2]='A';
        data_buff[3]='R';
        data_buff[4]='T';
        data_buff[5]=0;
        data_buff[6]='A';
        data_buff[7]='N';
        data_buff[8]='A';
        data_buff[9]=0;
        data_buff[10]='T';
        data_buff[11]='I';
        data_buff[12]='M';
        data_buff[13]='E';
        data_buff[14]=20;
        Display_6x8_char(2,0,15,data_buff);

        j=M_lever4_start_year_temp;
        j=j>>4;
        j&=0x0f;
        data_buff[8]=Get_ascii_data(j);
        j=M_lever4_start_year_temp;
        j&=0x0f;
        data_buff[9]=Get_ascii_data(j);
        data_buff[10]=14;

///              j=RTC_DATA.mouth_temp;
        j=M_lever4_start_munth_temp;
        j=j>>4;
        j&=0x01;
        data_buff[11]=Get_ascii_data(j);
///              j=RTC_DATA.mouth_temp;
        j=M_lever4_start_munth_temp;
        j&=0x0f;
        data_buff[12]=Get_ascii_data(j);
        data_buff[13]=14;
///              j=RTC_DATA.day_temp;
        j=M_lever4_start_day_temp;
        j=j>>4;
        j&=0x03;
        data_buff[14]=Get_ascii_data(j);
///              j=RTC_DATA.day_temp;
        j=M_lever4_start_day_temp;
        j&=0x0f;
        data_buff[15]=Get_ascii_data(j);
        data_buff[5]=0;
        data_buff[6]=0;
        data_buff[7]=0;

///              j=RTC_DATA.hour_temp;
        j=M_lever4_start_hour_temp;
        j=j>>4;
        j&=0x03;
        data_buff[0]=Get_ascii_data(j);
///              j=RTC_DATA.hour_temp;
        j=M_lever4_start_hour_temp;
        j&=0x0f;
        data_buff[1]=Get_ascii_data(j);
        data_buff[2]=20;
///              j=RTC_DATA.minu_temp;
        j=M_lever4_start_mini_temp;
        j=j>>4;
        j&=0x07;
        data_buff[3]=Get_ascii_data(j);
///              j=RTC_DATA.minu_temp;
        j=M_lever4_start_mini_temp;
        j&=0x0f;
        data_buff[4]=Get_ascii_data(j);

////              Display_8x16_char(3,6,13,data_buff);
        Display_6x8_char(4,6,16,data_buff);
    }
}

//===============================================================================
//	finction	:Disp_lever1_menu	:LCM use 8080 system
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.27
//	modefy		:null
//===============================================================================
void Disp_lever1_menu(void)				//第一级菜单显示控制函数
{
    uint8_t i,Line;
    uint16_t k;
    uint8_t const *Adds_index;
    uint8_t const *index;

    Clear_n_page_ram(0,7);
    i=Menu_index_select-Menu_lever1_point;
    if(M_menur1_language_select_temp==2)		//中文
    {
        for(Line=0; Line<4; Line++)
        {
            Adds_index=TB_Lever1_menu+i*7;			        //char data
            index= Lever1_menu_index+i;				//char num.

            for(k=0; k<*index; k++)
            {
                Get_16x16_char_single(Line*2,k*12+6,*Adds_index);
                ++Adds_index;
            }
            Get_8x16_char_single(Line*2,118,'Z'+1);
            ++i;
        }

        for(k=0; k<256; k++)
            Disp_buff128x64[(Menu_lever1_point-1)*256+k]=~Disp_buff128x64[(Menu_lever1_point-1)*256+k];
    }
    //else						// 英文
    //{
    //   for(k=1;k<MAIN_MENU;k++)
    //   {
    //       Adds_index=Main_menu_char+k-1;				//disp main menu
    //       Get_6x8_char_single(0,k*6,*Adds_index);
    //       if(Menu_index_select==1)
    //          Get_6x8_char_single(0,122,0x5c);
    //       else if(Menu_index_select==7)
    //          Get_6x8_char_single(0,122,0x5b);
    //       else
    //          Get_6x8_char_single(0,122,0x5d);
    //   }
    //   for(j=0;j<128;j++)
    //      Disp_buff128x64[128+j]=0x10;
    //
    //	for(Line=2;Line<8;Line++)					//diap menu char num.
    //	{
    //       j=i*16;
    //       index= Lever1_en_menu_index+i;
    //	    for(k=1;k<*index+1;k++)
    //	    {
    //           Adds_index=Lever1_en_menu_char+j;
    //           Get_6x8_char_single(Line,k*6,*Adds_index);
    //           ++j;
    //       }
    //       Get_6x8_char_single(Line,122,0x5e);
    //       ++i;
    //	}
    //   for(k=0;k<128;k++)
    //       Disp_buff128x64[(Menu_lever1_point+1)*128+k]=~Disp_buff128x64[(Menu_lever1_point+1)*128+k];
    //}
    Display_all_buff();
}

//===============================================================================
//	finction	:Menu1_adj_valu	:LCM use 8080 system
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.28
//	modefy		:null
//===============================================================================
void Menu1_adj_valu(void)			//第三级菜单设置参数值的调整显示
{
    uint8_t i,j;

    i=Menu_index_finc_select;

    if(Menu_index_select==7)
    {
        if((i!=3)&&(i!=4)&&(i!=8))
        {
            if(i==5)
            {
                j=Menu_lever3_Pub_adj_cnt;
                j&=0x0f;
                i=Get_ascii_data(j);
                Get_6x8_char_single(Menu_lever3_PoPa,Seg_adds_bak+1,i);
            }
            else
            {
                i=Get_ascii_data(Menu_lever3_Pub_adj_cnt);
                Get_6x8_char_single(Menu_lever3_PoPa,Seg_adds_bak+1,i);
            }
            for(i=0; i<6; i++)
            {
                Disp_buff128x64[128*Menu_lever3_PoPa+Seg_adds_bak+i+1]=~Disp_buff128x64[128*Menu_lever3_PoPa+Seg_adds_bak+i+1];
                Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Seg_adds_bak+i+1]|=0x01;
            }
        }
    }
    else if((Menu_index_select==2)&&(i!=1)&&(i!=3)&&(i!=10)&&(i!=11)&&(i!=12)&&(i!=17)&&(i!=18))
    {
        i=Get_ascii_data(Menu_lever3_Pub_adj_cnt);
        Get_6x8_char_single(Menu_lever3_PoPa,Seg_adds_bak+1,i);

        for(i=0; i<6; i++)
        {
            Disp_buff128x64[128*Menu_lever3_PoPa+Seg_adds_bak+i+1]=~Disp_buff128x64[128*Menu_lever3_PoPa+Seg_adds_bak+i+1];
            Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Seg_adds_bak+i+1]|=0x01;
        }
    }
    else if((Menu_index_select==3)&&(i!=1)&&(i!=2)&&(i!=6)&&(i!=8)&&(i!=19))
    {
        if((Menu_index_finc_select==18)&&(Menu_lever3_Pub_select_cnt==1))
        {
            i=Prj_correlat>>28;
            i&=0x0f;

            if(i==2)
                Get_6x8_char_single(Menu_lever3_PoPa,Seg_adds_bak+1,14);
            else
                Get_6x8_char_single(Menu_lever3_PoPa,Seg_adds_bak+1,13);
        }
        else if((Menu_index_finc_select==13)&&(M_menur1_language_select_temp==2))
        {
            i=Get_ascii_data(Menu_lever3_Pub_adj_cnt);
            Get_8x16_char_single(Menu_lever3_PoPa,Seg_adds_bak+1,i);

            for(i=0; i<10; i++)
            {
                Disp_buff128x64[128*Menu_lever3_PoPa+Seg_adds_bak+i+1]=~Disp_buff128x64[128*Menu_lever3_PoPa+Seg_adds_bak+i+1];
                Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Seg_adds_bak+i+1]=~Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Seg_adds_bak+i+1];
            }
            goto ex_adj1;
        }
        else
        {
            i=Get_ascii_data(Menu_lever3_Pub_adj_cnt);
            Get_6x8_char_single(Menu_lever3_PoPa,Seg_adds_bak+1,i);
        }
        for(i=0; i<6; i++)
        {
            Disp_buff128x64[128*Menu_lever3_PoPa+Seg_adds_bak+i+1]=~Disp_buff128x64[128*Menu_lever3_PoPa+Seg_adds_bak+i+1];
            Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Seg_adds_bak+i+1]|=0x01;
        }
    }
ex_adj1:
    Display_all_buff();
}

//===============================================================================
//	finction	:Disp_lever2_menu	:LCM use 8080 system
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.28
//	modefy		:null
//===============================================================================
void Disp_lever2_menu(void)				//第二级菜单显示控制函数
{
    uint16_t k;
    uint8_t i,j,Line,n;
    uint8_t const *Adds_index=0;
    uint8_t const *index=0;

////     Clear_screen();
    Clear_n_page_ram(0,7);

    i=Menu_index_finc_select-Menu_lever2_point;
    n=Menu_index_finc_select-Menu_lever2_point;
    if(M_menur1_language_select_temp==2)
    {
        for(Line=0; Line<4; Line++)
        {
            switch(Menu_index_select)
            {
            case 1:
                Adds_index=Lever2_menu1+i*7;
                index= Lever2_menu1_index+i;
                break;
            case 2:
                Adds_index=Lever2_menu2+i*7;
                index= Lever2_menu2_index+i;
                break;
            case 3:
                Adds_index=Lever2_menu3+i*7;
                index= Lever2_menu3_index+i;
                break;
            case 4:
                Adds_index=Lever2_menu4+i*7;
                index= Lever2_menu4_index+i;
                break;
            case 5:
                Adds_index=Lever2_menu5+i*7;
                index= Lever2_menu5_index+i;
                break;
            case 6:
                Adds_index=Lever2_menu6+i*7;
                index= Lever2_menu6_index+i;
                break;
            case 7:
                Adds_index=Lever2_menu7+i*7;
                index= Lever2_menu7_index+i;
                break;
            }
            for(k=0; k<*index; k++)
            {
                Get_16x16_char_single(Line*2,k*12+6,*Adds_index);
                ++Adds_index;
            }
            if((Menu_index_select==6)&&(n<=1))
            {
                if((n==0)&&(Line==1))		j=0;
                else if((n==1)&&(Line==0))	j=1;
                else				j=3;
                if(j<=1)
                {
                    for(k=0; k<73; k++)
                    {
                        Disp_buff128x64[128*(2-j*2)+102-k]=Disp_buff128x64[128*(2-j*2)+90-k];
                        Disp_buff128x64[128*(3-j*2)+102-k]=Disp_buff128x64[128*(3-j*2)+90-k];
                    }
                    Get_8x16_char_single(2-j*2,19,11);
                }
            }
            Get_8x16_char_single(Line*2,118,'Z'+1);
            ++i;
        }

        for(k=0; k<256; k++)
            Disp_buff128x64[(Menu_lever2_point-1)*256+k]=~Disp_buff128x64[(Menu_lever2_point-1)*256+k];
    }
    //else
    //{
    //
    //   j=(Menu_index_select-1)*16;
    //   index= Lever1_en_menu_index+Menu_index_select-1;       		//在第一页显示上一级菜单的名称
    //	for(k=1;k<*index+1;k++)
    //	{
    //       Adds_index=Lever1_en_menu_char+j;
    //       Get_6x8_char_single(0,k*6,*Adds_index);
    //       ++j;
    //   }
    //
    //   if(Menu_index_finc_select==1)
    //      Get_6x8_char_single(0,122,0x5c);
    //   else if(Menu_index_finc_select==Lever2_menu_num)
    //      Get_6x8_char_single(0,122,0x5b);
    //   else
    //      Get_6x8_char_single(0,122,0x5d);
    //
    //   for(j=0;j<128;j++)
    //      Disp_buff128x64[128+j]=0x10;
    //
    //	//for(Line=2;Line<=Lever2_menu_num+1;Line++)
    //	for(Line=2;Line<8;Line++)
    //	{
    //	    jj=i*16;
    //       switch(Menu_index_select)
    //       {
    //           case 1:
    //                   index= Lever21_en_menu_index+i;				//char num.
    //           break;
    //           case 2:
    //                   index= Lever22_en_menu_index+i;				//char num.
    //           break;
    //           case 3:
    //                   index= Lever23_en_menu_index+i;				//char num.
    //           break;
    //           case 4:
    //                   index= Lever24_en_menu_index+i;				//char num.
    //           break;
    //           case 5:
    //                   index= Lever25_en_menu_index+i;				//char num.
    //           break;
    //           case 6:
    //                   index= Lever26_en_menu_index+i;				//char num.
    //           break;
    //           case 7:
    //                   index= Lever27_en_menu_index+i;				//char num.
    //           break;
    //       }
    //	    for(k=1;k<*index+1;k++)
    //	    {
    //           switch(Menu_index_select)
    //           {
    //	    case 1:
    //                    Adds_index=Lever21_en_menu_char+jj;				//char data
    //	    break;
    //	    case 2:
    //                    Adds_index=Lever22_en_menu_char+jj;				//char data
    //	    break;
    //	    case 3:
    //                    Adds_index=Lever23_en_menu_char+jj;				//char data
    //	    break;
    //	    case 4:
    //                    Adds_index=Lever24_en_menu_char+jj;				//char data
    //	    break;
    //	    case 5:
    //                    Adds_index=Lever25_en_menu_char+jj;				//char data
    //	    break;
    //	    case 6:
    //                    Adds_index=Lever26_en_menu_char+jj;				//char data
    //	    break;
    //	    case 7:
    //                    Adds_index=Lever27_en_menu_char+jj;				//char data
    //	    break;
    //           }
    //           Get_6x8_char_single(Line,k*6,*Adds_index);
    //           ++jj;
    //       }
    //       Get_6x8_char_single(Line,122,0x5e);
    //       ++i;
    //	}
    //   for(k=0;k<128;k++)
    //       Disp_buff128x64[(Menu_lever2_point+1)*128+k]=~Disp_buff128x64[(Menu_lever2_point+1)*128+k];
    //}
    Display_all_buff();
}

//===============================================================================
//	finction	:Lever3_select_finc_adds	:LCM use 8080 system
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.27
//	modefy		:null
//===============================================================================
void Lever3_select_finc_adds(void)		//for select and modefy the setting day  第三级菜单的显示控制函数。
{
    uint8_t i;

    if(Menu_index_select==2)
    {
        switch(Menu_index_finc_select)
        {
        case 1:		//分析物
/////    	         M_menur1_analy_obj_select_temp=Menu_lever3_Pub_select_cnt;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_PoPa==7)
                    Seg_adds_bak=rev_clear_set_arear(7,Seg_adds_bak,Seg_rev_lenth,3,11,30);
                else
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,11,30);
                M_menur1_LZDJ_select_temp=1;
                Menu_lever3_PoPa=3;
                break;
            case 2:
                Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,47,18);
                M_menur1_LZDJ_select_temp=2;
                break;
            case 3:
                Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,71,12);
                M_menur1_LZDJ_select_temp=2;
                break;
            case 4:
                if(Menu_lever3_PoPa==5)
                    Seg_adds_bak=rev_clear_set_arear(5,Seg_adds_bak,Seg_rev_lenth,3,89,18);
                else
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,89,18);
                M_menur1_LZDJ_select_temp=2;
                Menu_lever3_PoPa=3;
                break;
            //--------------------------------------------
            case 5:
                if(Menu_lever3_PoPa==3)
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,5,11,12);
                else
                    Seg_adds_bak=rev_clear_set_arear(5,Seg_adds_bak,Seg_rev_lenth,5,11,12);
                M_menur1_LZDJ_select_temp=2;
                Menu_lever3_PoPa=5;
                break;
            case 6:
                Seg_adds_bak=rev_clear_set_arear(5,Seg_adds_bak,Seg_rev_lenth,5,29,12);
                M_menur1_LZDJ_select_temp=2;
                break;
            case 7:
                Seg_adds_bak=rev_clear_set_arear(5,Seg_adds_bak,Seg_rev_lenth,5,47,12);
                M_menur1_LZDJ_select_temp=2;
                break;
            case 8:
                Seg_adds_bak=rev_clear_set_arear(5,Seg_adds_bak,Seg_rev_lenth,5,65,12);
                M_menur1_LZDJ_select_temp=2;
                break;
            case 9:
                Seg_adds_bak=rev_clear_set_arear(5,Seg_adds_bak,Seg_rev_lenth,5,83,12);
                M_menur1_LZDJ_select_temp=2;
                break;
            case 10:
                if(Menu_lever3_PoPa==7)
                    Seg_adds_bak=rev_clear_set_arear(7,Seg_adds_bak,Seg_rev_lenth,5,101,6);
                else
                    Seg_adds_bak=rev_clear_set_arear(5,Seg_adds_bak,Seg_rev_lenth,5,101,6);
                M_menur1_LZDJ_select_temp=1;
                Menu_lever3_PoPa=5;
                break;
            //--------------------------------------------
            case 11:
                if(Menu_lever3_PoPa==5)
                    Seg_adds_bak=rev_clear_set_arear(5,Seg_adds_bak,Seg_rev_lenth,7,11,24);
                else
                    Seg_adds_bak=rev_clear_set_arear(7,Seg_adds_bak,Seg_rev_lenth,7,11,24);
                M_menur1_LZDJ_select_temp=2;
                Menu_lever3_PoPa=7;
                break;
            case 12:
                Seg_adds_bak=rev_clear_set_arear(7,Seg_adds_bak,Seg_rev_lenth,7,41,12);
                M_menur1_LZDJ_select_temp=2;
                break;
            case 13:
                //if(Menu_lever3_PoPa==3)
                //Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,7,59,12);
                //else
                Seg_adds_bak=rev_clear_set_arear(7,Seg_adds_bak,Seg_rev_lenth,7,59,12);
                //Menu_lever3_PoPa=7;
                break;

            case 14:
                Seg_adds_bak=rev_clear_set_arear(7,Seg_adds_bak,Seg_rev_lenth,7,77,12);;
                break;
            case 15:
                Seg_adds_bak=rev_clear_set_arear(7,Seg_adds_bak,Seg_rev_lenth,7,95,12);
                break;
            case 16:
                if(Menu_lever3_PoPa==3)
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,7,113,12);
                else
                    Seg_adds_bak=rev_clear_set_arear(7,Seg_adds_bak,Seg_rev_lenth,7,113,12);
                Menu_lever3_PoPa=7;
                break;
            }
            break;
        case 2:	//量程
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_menur1_lange_data_temp>>16;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_menur1_lange_data_temp>>12;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=M_menur1_lange_data_temp>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            case 4:
                i=M_menur1_lange_data_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+18,6);
                break;
            case 5:
                i=M_menur1_lange_data_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+30,6);
                break;
            }
            break;
        case 3:	//单位
/////    	         M_menur1_unit_select_temp=Menu_lever3_Pub_select_cnt;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(Menu_lever3_PoPa==5)
                    Seg_adds_bak=rev_clear_set_arear(5,Seg_adds_bak,Seg_rev_lenth,3,11,18);
                else
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,11,18);
                Menu_lever3_PoPa=3;
                break;
            case 2:
                Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,35,24);
                break;
            case 3:
                if(Menu_lever3_PoPa==5)
                    Seg_adds_bak=rev_clear_set_arear(5,Seg_adds_bak,Seg_rev_lenth,3,65,24);
                else
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,65,24);
                Menu_lever3_PoPa=3;
                break;
            case 4:
                if(Menu_lever3_PoPa==3)
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,5,11,18);
                else
                    Seg_adds_bak=rev_clear_set_arear(5,Seg_adds_bak,Seg_rev_lenth,5,11,18);
                Menu_lever3_PoPa=5;
                break;
            case 5:
                if(Menu_lever3_PoPa==3)
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,5,35,18);
                else
                    Seg_adds_bak=rev_clear_set_arear(5,Seg_adds_bak,Seg_rev_lenth,5,35,18);
                Menu_lever3_PoPa=5;
                break;
            }
            break;
        case 4:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_menur1_CSSJ_temp>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_menur1_CSSJ_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=M_menur1_CSSJ_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            }
            break;
        case 5:	//取样时间
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_menur1_QYSJ_temp>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_menur1_QYSJ_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=M_menur1_QYSJ_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            }
            break;
        case 8:	//分析等待时间
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=E_wait_time_temp>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=E_wait_time_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=E_wait_time_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            }
            break;
        case 7:	//信号漂移
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_menur1_XHPY_temp>>12;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_menur1_XHPY_temp>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=M_menur1_XHPY_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+18,6);
                break;
            case 4:
                i=M_menur1_XHPY_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+24,6);
                break;
            }
            break;
        case 9:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_menur1_DDSJ_temp>>12;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_menur1_DDSJ_temp>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=M_menur1_DDSJ_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            case 4:
                i=M_menur1_DDSJ_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+18,6);
                break;
            }
            break;
        case 11:	//自动稀释
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
/////    	        	    M_menur1_ZDXS_select_temp=1;
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,29,12);
                else
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,29,18);
                break;
            case 2:
/////    	        	    M_menur1_ZDXS_select_temp=2;
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,53,12);
                else
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,53,12);
                break;
            }
            break;
        case 12:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+24,6);
                break;
            case 3:
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+48,6);
                break;
            case 4:
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+66,6);
                break;
            }
            break;
        case 13:	//允许误差
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=Allow_error>>12;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=Allow_error>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=Allow_error>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            case 4:
                i=Allow_error;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+24,6);
                break;
            }
            break;
        case 14:	//核查判断
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_lever3_HCPD_temp>>16;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_lever3_HCPD_temp>>12;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=M_lever3_HCPD_temp>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            case 4:
                i=M_lever3_HCPD_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+24,6);
                break;
            case 5:
                i=M_lever3_HCPD_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+30,6);
                break;
            }
            break;
        case 15:	//低浓度报警
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=Alarm_low_nd>>16;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=Alarm_low_nd>>12;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=Alarm_low_nd>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            case 4:
                i=Alarm_low_nd>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+18,6);
                break;
            case 5:
                i=Alarm_low_nd;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+30,6);
                break;
            }
            break;
        case 16:	//高浓度报警
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=Alarm_hight_nd>>16;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=Alarm_hight_nd>>12;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=Alarm_hight_nd>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            case 4:
                i=Alarm_hight_nd>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+18,6);
                break;
            case 5:
                i=Alarm_hight_nd;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+30,6);
                break;
            }
            break;
        case 17:	//斜率更新
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,29,12);
                else
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,29,18);
/////    	        	    Updata_slope_flag=1;
                break;
            case 2:
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,53,12);
                else
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,53,12);
/////    	        	    Updata_slope_flag=2;
                break;
            }
            break;
        case 18:	//是否消解
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,29,12);
                else
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,29,18);
                break;
            case 2:
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,53,12);
                else
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,53,12);
                break;
            }
            break;
        }
    }
    else if(Menu_index_select==3)
    {
        switch(Menu_index_finc_select)
        {
        case 1:	//电极法，比色法选择
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:		//电极法
/////    	         	     M_menur1_LZDJ_select_temp=1;
/////    	         	     M_menur1_DL_select_temp=1;
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,11,36);
                else
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,29,30);
                break;
            case 2:		//比色法
/////    	         	     M_menur1_LZDJ_select_temp=2;
/////    	         	     M_menur1_DL_select_temp=2;
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,59,36);
                else
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,65,36);
                break;
            }
            break;
        case 2:	//标准法，曲线法选择
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
/////    	         	     M_menur1_DL_select_temp=1;
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,11,36);
                else
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,29,48);
                break;
            case 2:
/////    	         	     M_menur1_DL_select_temp=2;
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,59,36);
                else
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,83,30);
                break;
            }
            break;
        case 3:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_menur1_led_dianliu_temp>>16;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_menur1_led_dianliu_temp>>12;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=M_menur1_led_dianliu_temp>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+18,6);
                break;
            case 4:
                i=M_menur1_led_dianliu_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+24,6);
                break;
            case 5:
                i=M_menur1_led_dianliu_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+30,6);
                break;
            }
            break;
        case 4:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_menur1_led_data_temp>>12;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_menur1_led_data_temp>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=M_menur1_led_data_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            case 4:
                i=M_menur1_led_data_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+24,6);
                break;
            }
            break;
        case 5:	//搅拌速度
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_menur1_JB_speed_temp>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_menur1_JB_speed_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=M_menur1_JB_speed_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            }
            break;
        case 6:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
/////    	         	     M_menur1_direction_select_temp=1;
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,11,36);
                else
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,29,6);
                break;
            case 2:
/////    	         	     M_menur1_direction_select_temp=2;
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,59,36);
                else
                    Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,41,6);
                break;
            }
            break;
        case 7:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_menur1_ZSB_speed_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_menur1_ZSB_speed_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            }
            break;
        case 8:		//dot num 小数点位数
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
/////    	         	    M_menur1_dot_num_select_temp=1;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
/////    	         	    M_menur1_dot_num_select_temp=2;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            case 3:
/////    	         	    M_menur1_dot_num_select_temp=3;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+24,6);
                break;
            }
            break;
        case 9:		//**.**mL
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_menur1_sample_v_temp>>12;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_menur1_sample_v_temp>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=M_menur1_sample_v_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+18,6);
                break;
            case 4:
                i=M_menur1_sample_v_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+24,6);
                break;
            }
            break;
        case 11:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_menur1_jz1_v_temp>>12;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_menur1_jz1_v_temp>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=M_menur1_jz1_v_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+18,6);
                break;
            case 4:
                i=M_menur1_jz1_v_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+24,6);
                break;
            }
            break;
        case 10:		//***.**mg/L
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_menur1_jz1nd_temp>>16;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_menur1_jz1nd_temp>>12;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=M_menur1_jz1nd_temp>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            case 4:
                i=M_menur1_jz1nd_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+24,6);
                break;
            case 5:
                i=M_menur1_jz1nd_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+30,6);
                break;
            }
            break;
        case 12:		//****.*mg/L
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_menur1_jz2nd_temp>>16;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_menur1_jz2nd_temp>>12;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=M_menur1_jz2nd_temp>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            case 4:
                i=M_menur1_jz2nd_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+18,6);
                break;
            case 5:
                i=M_menur1_jz2nd_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+30,6);
                break;
            }
            break;
        case 13:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
/////    	         	M_dongtai_flag=0;
                i=M_menur1_jz2_v_temp>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,11,10);
                else
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,29,6);
                break;
            case 2:
                i=M_menur1_jz2_v_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,31,10);
                else
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,41,6);
//////                        M_dongtai_flag=0;
                break;
            case 3:
                i=M_menur1_jz2_v_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,41,10);
                else
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,47,6);
/////                        M_dongtai_flag=0;
                break;
            case 4:
                if(M_menur1_language_select_temp==2)
                    Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,81,24);
                else
                    Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,Seg_rev_lenth,3,71,42);
/////                        M_dongtai_flag=1;
                break;
            }
            break;
        case 14:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_menur1_sj1_v_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_menur1_sj1_v_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            }
            break;
        case 15:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_menur1_sj2_v_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_menur1_sj2_v_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            }
            break;
        case 16:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_menur1_sj3_v_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_menur1_sj3_v_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            }
            break;
        case 17:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_menur1_sj4_v_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_menur1_sj4_v_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            }
            break;
        case 18:		//工程系数
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=Prj_correlat>>28;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=Prj_correlat>>20;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=Prj_correlat>>16;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            case 4:
                i=Prj_correlat>>12;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+18,6);
                break;
            case 5:
                i=Prj_correlat>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+30,6);
                break;
            case 6:
                i=Prj_correlat>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+36,6);
                break;
            case 7:
                i=Prj_correlat;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+42,6);
                break;
            }
            break;
        case 20:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=Set_xishu>>24;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=Set_xishu>>20;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=Set_xishu>>16;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            case 4:
                i=Set_xishu>>12;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+18,6);
                break;
            case 5:
                i=Set_xishu>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+30,6);
                break;
            case 6:
                i=Set_xishu>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+36,6);
                break;
            case 7:
                i=Set_xishu;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+42,6);
                break;
            }
            break;
        case 21:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=set_cal_data>>24;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=set_cal_data>>20;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=set_cal_data>>16;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            case 4:
                i=set_cal_data>>12;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+18,6);
                break;
            case 5:
                i=set_cal_data>>8;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+30,6);
                break;
            case 6:
                i=set_cal_data>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+36,6);
                break;
            case 7:
                i=set_cal_data;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+42,6);
                break;
            }
            break;
        }
    }
    else if(Menu_index_select==7)
    {
        switch(Menu_index_finc_select)
        {
        case 1:	//设置时间
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=RTC_DATA.hour_temp;
                i=i>>4;
                i&=0x03;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=RTC_DATA.hour_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=RTC_DATA.minu_temp;
                i=i>>4;
                i&=0x07;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+18,6);
                break;
            case 4:
                i=RTC_DATA.minu_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+24,6);
                break;
            }
            break;
        case 2:	//设置日期
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=RTC_DATA.year_temp;
                i=i>>4;
                i&=0x03;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=RTC_DATA.year_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                i=RTC_DATA.mouth_temp;
                i=i>>4;
                i&=0x01;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+18,6);
                break;
            case 4:
                i=RTC_DATA.mouth_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+24,6);
                break;
            case 5:
                i=RTC_DATA.day_temp;
                i=i>>4;
                i&=0x03;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+36,6);
                break;
            case 6:
                i=RTC_DATA.day_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+42,6);
                break;
            }
            break;
        case 3:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
/////                	    M_menur1_language_select_temp=1;
                Seg_adds_bak=rev_clear_set_arear16(Menu_lever3_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever3_PoPa,19,30);
                break;
            case 2:
/////                	    M_menur1_language_select_temp=2;
                Seg_adds_bak=rev_clear_set_arear16(Menu_lever3_PoPa,Seg_adds_bak,Seg_rev_lenth,Menu_lever3_PoPa,59,24);
                break;
            }
            break;
        case 5:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                i=M_menur1_lcd_Contrast_temp>>4;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                i=M_menur1_lcd_Contrast_temp;
                i&=0x0f;
                Menu_lever3_Pub_adj_cnt=i;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            }
            break;
        case 6:
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                Menu_lever3_Pub_adj_cnt=Back_light_time/10000;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg,6);
                break;
            case 2:
                Menu_lever3_Pub_adj_cnt=Back_light_time/1000;
                Menu_lever3_Pub_adj_cnt=Menu_lever3_Pub_adj_cnt%10;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+6,6);
                break;
            case 3:
                Menu_lever3_Pub_adj_cnt=Back_light_time/100;
                Menu_lever3_Pub_adj_cnt=Menu_lever3_Pub_adj_cnt%10;
                Seg_adds_bak=rev_clear_set_arear(Menu_lever3_PoPa,Seg_adds_bak,6,Menu_lever3_PoPa,Menu_lever3_PoSeg+12,6);
                break;
            }
            break;
        }
    }
    else if(Menu_index_select==5)
    {
/////        if((Key_code_bak==KEY_CODE_RIGHT)&&(disp_sec_screen_flag!=2))
        if(Key_code_bak==KEY_CODE_RIGHT)
        {
            for(i=1; i<8; i++)
                Disp_buff128x64[128*7+108+i]=~Disp_buff128x64[128*7+108+i];
            for(i=1; i<8; i++)
                Disp_buff128x64[128*7+120+i]=~Disp_buff128x64[128*7+120+i];

            if(Men_del_flag==1)
            {
                Men_del_flag=0;
            }
            else
            {
                Menu_lever3_Pub_select_cnt=2;
                Men_del_flag=1;
            }
            Display_all_buff();
        }
////        else if(Key_code_bak==KEY_CODE_LEFT)
////        {
////      	  disp_sec_screen_flag=2;
////      	  switch(Menu_index_finc_select)
////      	  {
////      	     case 1:
////                    	Disp_sec_screen_data(&Fenxi);
////      	     break;
////      	     case 2:
////                    	Disp_sec_screen_data(&Jiaozheng);
////     	     break;
////      	     case 3:
////                    	Disp_sec_screen_data(&Baojing);
////     	     break;
////      	     case 4:
////                    	Disp_sec_screen_data(&Cuowu);
////      	     break;
////      	  }
////        }
    }
}
//===============================================================================
//	finction	:Disp_lever3_menu	:LCM use 8080 system
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.28
//	modefy		:null
//===============================================================================
void Disp_lever3_menu(void)		//第三级菜单显示控制函数
{
    uint16_t jj;
    uint8_t i,j,k,Line,m,buff_d0[6],buff_d1[6];
    uint8_t const *Adds_index=0;
    uint8_t const *index=0;

    uint8_t data_buff[20];		//for setting disp

    Clear_n_page_ram(0,7);

    i=Menu_index_finc_select-1;
    if(M_menur1_language_select_temp!=2)
    {
        //  jj=i*16;
        //  switch(Menu_index_select)
        //  {
        //      case 1:
        //              index= Lever21_en_menu_index+i;				//char num.
        //      break;
        //      case 2:
        //              index= Lever22_en_menu_index+i;				//char num.
        //      break;
        //      case 3:
        //              index= Lever23_en_menu_index+i;				//char num.
        //      break;
        //      case 4:
        //              index= Lever24_en_menu_index+i;				//char num.
        //      break;
        //      case 5:
        //              index= Lever25_en_menu_index+i;				//char num.
        //      break;
        //      case 6:
        //              index= Lever26_en_menu_index+i;				//char num.
        //      break;
        //      case 7:
        //              index= Lever27_en_menu_index+i;				//char num.
        //      break;
        //  }
        //  for(k=1;k<*index+1;k++)
        //  {
        //      switch(Menu_index_select)
        //      {
        //      case 1:
        //               Adds_index=Lever21_en_menu_char+jj;				//char data
        //      break;
        //      case 2:
        //               Adds_index=Lever22_en_menu_char+jj;				//char data
        //      break;
        //      case 3:
        //               Adds_index=Lever23_en_menu_char+jj;				//char data
        //      break;
        //      case 4:
        //               Adds_index=Lever24_en_menu_char+jj;				//char data
        //      break;
        //      case 5:
        //               Adds_index=Lever25_en_menu_char+jj;				//char data
        //      break;
        //      case 6:
        //               Adds_index=Lever26_en_menu_char+jj;				//char data
        //      break;
        //      case 7:
        //               Adds_index=Lever27_en_menu_char+jj;				//char data
        //      break;
        //      }
        //      Get_6x8_char_single(0,k*6,*Adds_index);
        //      ++jj;
        //  }
        //  for(j=0;j<128;j++)
        //     Disp_buff128x64[128+j]=0x10;
    }
    else		//中文
    {
        switch(Menu_index_select)
        {
        case 1:
            Adds_index=Lever2_menu1+i*7;
            index= Lever2_menu1_index+i;
            break;
        case 2:
            Adds_index=Lever2_menu2+i*7;
            index= Lever2_menu2_index+i;
            break;
        case 3:
            Adds_index=Lever2_menu3+i*7;
            index= Lever2_menu3_index+i;
            break;
        case 4:
            Adds_index=Lever2_menu4+i*7;
            index= Lever2_menu4_index+i;
            break;
        case 5:
            Adds_index=Lever2_menu5+i*7;
            index= Lever2_menu5_index+i;
            break;
        case 6:
            Adds_index=Lever2_menu6+i*7;
            index= Lever2_menu6_index+i;
            break;
        case 7:
            Adds_index=Lever2_menu7+i*7;
            index= Lever2_menu7_index+i;
            break;
        }
        for(k=0; k<*index; k++)
        {
            Get_16x16_char_single(0,k*12+6,*Adds_index);
            ++Adds_index;
        }
        for(k=0; k<128; k++)
            Disp_buff128x64[128+k]|=0x80;
    }
    switch(Menu_index_select)
    {
    case 7:
        Menu_lever3_Pub_select_cnt=1;
        switch(Menu_index_finc_select)
        {
        case 1:
            RTC_Set.hour_temp=RTC_DATA.hour_temp;
            RTC_Set.minu_temp=RTC_DATA.minu_temp;

            j=RTC_DATA.hour_temp;
            j=j>>4;
            j&=0x03;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=RTC_DATA.hour_temp;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            data_buff[2]=20;
            j=RTC_DATA.minu_temp;
            j=j>>4;
            j&=0x07;
            data_buff[3]=Get_ascii_data(j);
            j=RTC_DATA.minu_temp;
            j&=0x0f;
            data_buff[4]=Get_ascii_data(j);

            for(i=0; i<5; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);
            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 2:
            data_buff[0]='2';
            data_buff[1]='0';

            RTC_Set.year_temp=RTC_DATA.year_temp;
            RTC_Set.mouth_temp=RTC_DATA.mouth_temp;
            RTC_Set.day_temp=RTC_DATA.day_temp;

            j=RTC_DATA.year_temp;
            j=j>>4;
            j&=0x03;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[2]=Get_ascii_data(j);
            j=RTC_DATA.year_temp;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            data_buff[4]=14;
            j=RTC_DATA.mouth_temp;
            j=j>>4;
            j&=0x01;
            data_buff[5]=Get_ascii_data(j);
            j=RTC_DATA.mouth_temp;
            j&=0x0f;
            data_buff[6]=Get_ascii_data(j);
            data_buff[7]=14;
            j=RTC_DATA.day_temp;
            j=j>>4;
            j&=0x03;
            data_buff[8]=Get_ascii_data(j);
            j=RTC_DATA.day_temp;
            j&=0x0f;
            data_buff[9]=Get_ascii_data(j);

            for(i=0; i<10; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);
            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+41+i]=~Disp_buff128x64[128*3+41+i];
                Disp_buff128x64[128*4+41+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=41;
            break;
        case 3:
            data_buff[0]=234;	// E
            data_buff[1]=243;        // N
            data_buff[2]=236;        // G
            for(i=0; i<3; i++)
                Get_16x16_char_single(3,20+10*i,data_buff[i]);

            Get_8x16_char_single(3,31+20,11);
            Get_16x16_char_single(3,60,142);
            Get_16x16_char_single(3,72,143);


            Menu_lever3_PoPa=3;
            if(M_menur1_language_select_temp==2)
            {
                Seg_rev_lenth=25;
                Seg_adds_bak=Menu_lever3_PoSeg=59;
                Menu_lever3_Pub_select_cnt=2;
            }
            else
            {
                Seg_rev_lenth=31;
                Seg_adds_bak=Menu_lever3_PoSeg=19;
                Menu_lever3_Pub_select_cnt=1;
            }

            for(i=0; i<Seg_rev_lenth; i++)
            {
                Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i];
                Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i];
            }
            --Seg_rev_lenth;
            break;
        case 4:
            buff_d0[0]='D';
            buff_d0[1]=20;
            buff_d0[2]=buff_bool[0];
            buff_d0[2]>>=4;
            buff_d0[3]=buff_bool[0];
            buff_d0[3]&=0x0f;
            buff_d0[4]=buff_bool[1];
            buff_d0[4]>>=4;
            buff_d0[5]=buff_bool[1];
            buff_d0[5]&=0x0f;
            buff_d0[2]=Get_ascii_data(buff_d0[2]);
            buff_d0[3]=Get_ascii_data(buff_d0[3]);
            buff_d0[4]=Get_ascii_data(buff_d0[4]);
            buff_d0[5]=Get_ascii_data(buff_d0[5]);
//------------------------------------------------------
            buff_d1[0]='S';
            buff_d1[1]=20;
            buff_d1[2]=buff_bool[2];
            buff_d1[2]>>=4;
            buff_d1[3]=buff_bool[2];
            buff_d1[3]&=0x0f;
            buff_d1[4]=buff_bool[3];
            buff_d1[4]>>=4;
            buff_d1[5]=buff_bool[3];
            buff_d1[5]&=0x0f;
            buff_d1[2]=Get_ascii_data(buff_d1[2]);
            buff_d1[3]=Get_ascii_data(buff_d1[3]);
            buff_d1[4]=Get_ascii_data(buff_d1[4]);
            buff_d1[5]=Get_ascii_data(buff_d1[5]);
//-------------------------------------------------
            data_buff[0]='M';
            data_buff[1]=20;
//#ifdef __DEBUG__
//                       for(i=0;i<SYSTEM_SERIES_BIT;i++)
//                          data_buff[i+2]=Get_ascii_data(Program_series[i]);
//#else
            index=(uint8_t const *)0x0001FE00;
            for(i=0; i<SYSTEM_SERIES_BIT; i++,index++)
                data_buff[i+2]=*index;
//#endif
            for(i=0; i<6; i++)
                Get_6x8_char_single(5,i*6+6,buff_d1[i]);
            for(i=0; i<6; i++)
                Get_6x8_char_single(4,i*6+6,buff_d0[i]);
            for(i=0; i<SYSTEM_SERIES_BIT+2; i++)
                Get_6x8_char_single(3,i*6+6,data_buff[i]);
            break;
        case 5:
            j=M_menur1_lcd_Contrast_temp>>4;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=M_menur1_lcd_Contrast_temp;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            data_buff[2]=0;
            data_buff[3]=0;
            data_buff[4]=0;

            for(i=0; i<5; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);
            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 6:
            j=Back_light_time/10000;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            back_l_temp&=0x000000ff;
            j=j<<8;
            back_l_temp+=j;

            j=Back_light_time/1000;
            j=j%10;
            data_buff[1]=Get_ascii_data(j);
            back_l_temp&=0x00000f0f;
            j=j<<4;
            back_l_temp+=j;

            j=Back_light_time/100;
            j=j%10;
            back_l_temp&=0x00000ff0;
            back_l_temp+=j;

            data_buff[2]=Get_ascii_data(j);
            data_buff[3]='S';
            data_buff[4]=0;

            for(i=0; i<5; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);
            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 7:
            Menu_lever3_Pub_adj_cnt=LCD_bl_ctrl_data;
            data_buff[0]=Get_ascii_data(Menu_lever3_Pub_adj_cnt);
            data_buff[1]=20;
            data_buff[2]=15;
            data_buff[3]=15;
            data_buff[4]=15;

            for(i=0; i<5; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);
            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 8:
            buff_d0[0]='D';
            buff_d0[1]=20;
            buff_d0[2]=buff_app[0];
            buff_d0[2]>>=4;
            buff_d0[3]=buff_app[0];
            buff_d0[3]&=0x0f;
            buff_d0[4]=buff_app[1];
            buff_d0[4]>>=4;
            buff_d0[5]=buff_app[1];
            buff_d0[5]&=0x0f;
            buff_d0[2]=Get_ascii_data(buff_d0[2]);
            buff_d0[3]=Get_ascii_data(buff_d0[3]);
            buff_d0[4]=Get_ascii_data(buff_d0[4]);
            buff_d0[5]=Get_ascii_data(buff_d0[5]);
//------------------------------------------------------
            buff_d1[0]='S';
            buff_d1[1]=20;
            buff_d1[2]=buff_app[2];
            buff_d1[2]>>=4;
            buff_d1[3]=buff_app[2];
            buff_d1[3]&=0x0f;
            buff_d1[4]=buff_app[3];
            buff_d1[4]>>=4;
            buff_d1[5]=buff_app[3];
            buff_d1[5]&=0x0f;
            buff_d1[2]=Get_ascii_data(buff_d1[2]);
            buff_d1[3]=Get_ascii_data(buff_d1[3]);
            buff_d1[4]=Get_ascii_data(buff_d1[4]);
            buff_d1[5]=Get_ascii_data(buff_d1[5]);
//-------------------------------------------------
            data_buff[0]='M';
            data_buff[1]=20;
            data_buff[2]=Get_ascii_data(Program_ver[0]);
            data_buff[3]=Get_ascii_data(Program_ver[1]);
            data_buff[4]=Get_ascii_data(Program_ver[2]);
            data_buff[5]=Get_ascii_data(Program_ver[3]);
            data_buff[6]=Get_ascii_data(Program_ver[4]);
            data_buff[7]=Get_ascii_data(Program_ver[5]);
            data_buff[8]=Get_ascii_data(Program_ver[6]);
            data_buff[9]=Get_ascii_data(Program_ver[7]);
            data_buff[10]=Get_ascii_data(Program_ver[8]);
            data_buff[11]=Get_ascii_data(Program_ver[9]);
            data_buff[12]=Get_ascii_data(Program_ver[10]);
            data_buff[13]=Get_ascii_data(Program_ver[11]);
            data_buff[14]=Get_ascii_data(Program_ver[12]);

            for(i=0; i<6; i++)
                Get_6x8_char_single(5,i*6+6,buff_d1[i]);
            for(i=0; i<6; i++)
                Get_6x8_char_single(4,i*6+6,buff_d0[i]);
            for(i=0; i<15; i++)
                Get_6x8_char_single(3,i*6+6,data_buff[i]);
            break;
        case 9:		//bak data
            Menu_lever_select-=2;
            Display_menu_index();
            Back_system_reg();
            Menu_lever2_point=1;
            Menu_index_finc_select=1;
            break;
        case 10:	//reset data
            Menu_lever_select-=2;
            Display_menu_index();
            Load_bak_system_reg();
            Menu_lever2_point=1;
            Menu_index_finc_select=1;
            break;
        }
        break;
//----------------------------------------------
    case 2:
        switch(Menu_index_finc_select)
        {
        case 1:
            data_buff[0]='N';
            data_buff[1]='H';
            data_buff[2]='4';
            data_buff[3]=14;
            data_buff[4]='N';
            data_buff[5]=0;
            data_buff[6]='P';
            data_buff[7]='O';
            data_buff[8]='4';
            data_buff[9]=0;
            data_buff[10]='C';
            data_buff[11]='U';
            data_buff[12]=0;
            data_buff[13]='C';
            data_buff[14]='O';
            data_buff[15]='D';
            for(i=0; i<16; i++)
                Get_6x8_char_single(3,i*6+12,data_buff[i]);

            data_buff[0]='N';
            data_buff[1]='I';
            data_buff[2]=0;
            data_buff[3]='C';
            data_buff[4]='N';
            data_buff[5]=0;
            data_buff[6]='C';
            data_buff[7]='R';
            data_buff[8]=0;
            data_buff[9]='F';
            data_buff[10]='E';
            data_buff[11]=0;
            data_buff[12]='M';
            data_buff[13]='N';
            data_buff[14]=0;
            data_buff[15]='F';
            for(i=0; i<16; i++)
                Get_6x8_char_single(5,i*6+12,data_buff[i]);

            data_buff[0]='P';
            data_buff[1]='H';
            data_buff[2]='E';
            data_buff[3]='N';
            data_buff[4]=0;
            data_buff[5]='P';
            data_buff[6]='B';
            data_buff[7]=0;
            data_buff[8]='C';
            data_buff[9]='D';

            data_buff[10]=0;
            data_buff[11]='Z';
            data_buff[12]='N';
            data_buff[13]=0;
            data_buff[14]='T';
            data_buff[15]='N';
            data_buff[16]=0;
            data_buff[17]='T';
            data_buff[18]='P';

            for(i=0; i<19; i++)
                Get_6x8_char_single(7,i*6+12,data_buff[i]);

            Menu_lever3_PoPa=3;
            Seg_rev_lenth=13;
            switch(M_menur1_analy_obj_select_temp)
            {
            case 1:
                Menu_lever3_Pub_select_cnt=1;
                Seg_adds_bak=Menu_lever3_PoSeg=11;
                Seg_rev_lenth=31;
                break;
            case 2:
                Menu_lever3_Pub_select_cnt=2;
                Seg_adds_bak=Menu_lever3_PoSeg=47;
                Seg_rev_lenth=19;
                break;
            case 3:
                Menu_lever3_Pub_select_cnt=3;
                Seg_adds_bak=Menu_lever3_PoSeg=71;
                break;
            case 4:
                Menu_lever3_Pub_select_cnt=4;
                Seg_adds_bak=Menu_lever3_PoSeg=89;
                Seg_rev_lenth=19;
                break;
            case 5:
                Menu_lever3_Pub_select_cnt=5;
                Menu_lever3_PoPa=5;
                Seg_adds_bak=Menu_lever3_PoSeg=11;
                break;
            case 6:
                Menu_lever3_Pub_select_cnt=6;
                Menu_lever3_PoPa=5;
                Seg_adds_bak=Menu_lever3_PoSeg=29;
                break;
            case 7:
                Menu_lever3_Pub_select_cnt=7;
                Menu_lever3_PoPa=5;
                Seg_adds_bak=Menu_lever3_PoSeg=47;
                break;
            case 8:
                Menu_lever3_Pub_select_cnt=8;
                Menu_lever3_PoPa=5;
                Seg_adds_bak=Menu_lever3_PoSeg=65;
                break;
            case 9:
                Menu_lever3_Pub_select_cnt=9;
                Menu_lever3_PoPa=5;
                Seg_adds_bak=Menu_lever3_PoSeg=83;
                break;
            case 10:
                Menu_lever3_Pub_select_cnt=10;
                Menu_lever3_PoPa=5;
                Seg_adds_bak=Menu_lever3_PoSeg=101;
                Seg_rev_lenth=7;
                break;
            case 11:
                Menu_lever3_Pub_select_cnt=11;
                Menu_lever3_PoPa=7;
                Seg_adds_bak=Menu_lever3_PoSeg=11;
                Seg_rev_lenth=25;
                break;
            case 12:
                Menu_lever3_Pub_select_cnt=12;
                Menu_lever3_PoPa=7;
                Seg_adds_bak=Menu_lever3_PoSeg=41;
                break;
            case 13:
                Menu_lever3_Pub_select_cnt=13;
                Menu_lever3_PoPa=7;
                Seg_adds_bak=Menu_lever3_PoSeg=59;
                break;

            case 14:
                Menu_lever3_Pub_select_cnt=14;
                Menu_lever3_PoPa=7;
                Seg_adds_bak=Menu_lever3_PoSeg=77;
                break;
            case 15:
                Menu_lever3_Pub_select_cnt=15;
                Menu_lever3_PoPa=7;
                Seg_adds_bak=Menu_lever3_PoSeg=95;
                break;
            case 16:
                Menu_lever3_Pub_select_cnt=16;
                Menu_lever3_PoPa=7;
                Seg_adds_bak=Menu_lever3_PoSeg=113;
                break;
            }
            for(i=0; i<Seg_rev_lenth; i++)
            {
                Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i];
                Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i]|=0x01;
            }
            --Seg_rev_lenth;
            break;
        case 2:
            j=M_menur1_lange_data_temp>>16;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=M_menur1_lange_data_temp>>12;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            j=M_menur1_lange_data_temp>>8;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            j=M_menur1_lange_data_temp>>4;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            data_buff[4]=19;
            j=M_menur1_lange_data_temp;
            j&=0x0f;
            data_buff[5]=Get_ascii_data(j);

            for(i=0; i<6; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 3:
            data_buff[0]='G';
            data_buff[1]=18;
            data_buff[2]='L';
            data_buff[3]=0;
            data_buff[4]='M';
            data_buff[5]='G';
            data_buff[6]=18;
            data_buff[7]='L';
            data_buff[8]=0;
            data_buff[9]='U';
            data_buff[10]='G';
            data_buff[11]=18;
            data_buff[12]='L';
            for(i=0; i<13; i++)
                Get_6x8_char_single(3,i*6+12,data_buff[i]);

            data_buff[0]='P';
            data_buff[1]='P';
            data_buff[2]='M';
            data_buff[3]=0;
            data_buff[4]='P';
            data_buff[5]='P';
            data_buff[6]='B';

            for(i=0; i<7; i++)
                Get_6x8_char_single(5,i*6+12,data_buff[i]);

            Menu_lever3_PoPa=3;
            Seg_rev_lenth=19;
            switch(M_menur1_unit_select_temp)
            {
            case 1:
                Menu_lever3_Pub_select_cnt=1;
                Seg_adds_bak=Menu_lever3_PoSeg=11;
                break;
            case 2:
                Menu_lever3_Pub_select_cnt=2;
                Seg_adds_bak=Menu_lever3_PoSeg=35;
                Seg_rev_lenth=25;
                break;
            case 3:
                Menu_lever3_Pub_select_cnt=3;
                Seg_adds_bak=Menu_lever3_PoSeg=65;
                Seg_rev_lenth=25;
                break;
            case 4:
                Menu_lever3_Pub_select_cnt=4;
                Seg_adds_bak=Menu_lever3_PoSeg=11;
                Menu_lever3_PoPa=5;
                break;
            case 5:
                Menu_lever3_Pub_select_cnt=5;
                Seg_adds_bak=Menu_lever3_PoSeg=35;
                Menu_lever3_PoPa=5;
                break;
            }
            for(i=0; i<Seg_rev_lenth; i++)
            {
                Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i];
                Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i]|=0x01;
            }
            --Seg_rev_lenth;
            break;
        case 5:
            j=M_menur1_QYSJ_temp>>8;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=M_menur1_QYSJ_temp>>4;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            j=M_menur1_QYSJ_temp;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            data_buff[3]=0;
            data_buff[4]='S';
            for(i=0; i<5; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 4:
            j=M_menur1_CSSJ_temp>>8;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=M_menur1_CSSJ_temp>>4;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            j=M_menur1_CSSJ_temp;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            data_buff[3]=0;
            data_buff[4]='S';
            for(i=0; i<5; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 6:
            Menu_lever3_Pub_adj_cnt=M_menur1_RX_time_temp;
            data_buff[0]=Get_ascii_data(M_menur1_RX_time_temp);
            Get_6x8_char_single(3,30,data_buff[0]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 7:
            j=M_menur1_XHPY_temp>>12;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=M_menur1_XHPY_temp>>8;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            data_buff[2]=19;
            j=M_menur1_XHPY_temp>>4;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            j=M_menur1_XHPY_temp;
            j&=0x0f;
            data_buff[4]=Get_ascii_data(j);
            for(i=0; i<5; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 8:		//wait time
            j=E_wait_time_temp>>8;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=E_wait_time_temp>>4;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            j=E_wait_time_temp;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            data_buff[3]=0;
            data_buff[4]='S';
            for(i=0; i<5; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 9:		//test time
            j=M_menur1_DDSJ_temp>>12;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=M_menur1_DDSJ_temp>>8;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            j=M_menur1_DDSJ_temp>>4;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            j=M_menur1_DDSJ_temp;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            data_buff[4]=0;
            data_buff[5]='S';
            for(i=0; i<6; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 10:			//斜率
            if(M_menur1_slope_flag==1)
                data_buff[0]=0;
            else
                data_buff[0]=14;

            j=M_menur1_slope_temp>>16;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            j=M_menur1_slope_temp>>12;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            j=M_menur1_slope_temp>>8;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            data_buff[4]=19;
            j=M_menur1_slope_temp>>4;
            j&=0x0f;
            data_buff[5]=Get_ascii_data(j);
            j=M_menur1_slope_temp;
            j&=0x0f;
            data_buff[6]=Get_ascii_data(j);
            for(i=0; i<7; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            break;
        case 11:	//auto xishi
            if(M_menur1_language_select_temp==2)
            {
                Get_16x16_char_single(3,30,120);
                Get_8x16_char_single(3,13+30,11);
                Get_16x16_char_single(3,24+30,121);

                Menu_lever3_PoPa=3;
                if(M_menur1_ZDXS_select_temp==2)
                {
                    Seg_rev_lenth=13;
                    Seg_adds_bak=Menu_lever3_PoSeg=53;
                    Menu_lever3_Pub_select_cnt=2;
                }
                else
                {
                    Seg_rev_lenth=13;
                    Seg_adds_bak=Menu_lever3_PoSeg=29;
                    Menu_lever3_Pub_select_cnt=1;
                }

                for(i=0; i<Seg_rev_lenth; i++)
                {
                    Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i];
                    Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i];
                }
                --Seg_rev_lenth;
            }
            //else	//20140216
            //{
            //   data_buff[0]='Y';
            //   data_buff[1]='E';
            //   data_buff[2]='S';
            //   data_buff[3]=18;
            //   data_buff[4]='N';
            //   data_buff[5]='O';
            //   for(i=0;i<6;i++)
            //      Get_6x8_char_single(3,i*6+30,data_buff[i]);
            //
            //   Menu_lever3_PoPa=3;
            //   if(M_menur1_ZDXS_select_temp==2)
            //   {
            //      Seg_rev_lenth=13;
            //      Seg_adds_bak=Menu_lever3_PoSeg=53;
            //      Menu_lever3_Pub_select_cnt=2;
            //   }
            //   else
            //   {
            //      Seg_rev_lenth=19;
            //      Seg_adds_bak=Menu_lever3_PoSeg=29;
            //      Menu_lever3_Pub_select_cnt=1;
            //   }
            //
            //   for(i=0;i<Seg_rev_lenth;i++)
            //   {
            //       Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i];
            //       Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i]|=0x01;
            //   }
            //   --Seg_rev_lenth;
            //}
            break;
        case 17:	//gengxin xielv
            if(M_menur1_language_select_temp==2)
            {
                Get_16x16_char_single(3,30,120);
                Get_8x16_char_single(3,13+30,11);
                Get_16x16_char_single(3,24+30,121);

                Menu_lever3_PoPa=3;
                if(Updata_slope_flag==2)
                {
                    Seg_rev_lenth=13;
                    Seg_adds_bak=Menu_lever3_PoSeg=53;
                    Menu_lever3_Pub_select_cnt=2;
                }
                else
                {
                    Seg_rev_lenth=13;
                    Seg_adds_bak=Menu_lever3_PoSeg=29;
                    Menu_lever3_Pub_select_cnt=1;
                }

                for(i=0; i<Seg_rev_lenth; i++)
                {
                    Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i];
                    Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i];
                }
                --Seg_rev_lenth;
            }
            //else	//20140216
            //{
            //   data_buff[0]='Y';
            //   data_buff[1]='E';
            //   data_buff[2]='S';
            //   data_buff[3]=18;
            //   data_buff[4]='N';
            //   data_buff[5]='O';
            //   for(i=0;i<6;i++)
            //      Get_6x8_char_single(3,i*6+30,data_buff[i]);
            //
            //   Menu_lever3_PoPa=3;
            //   if(Updata_slope_flag==2)
            //   {
            //      Seg_rev_lenth=13;
            //      Seg_adds_bak=Menu_lever3_PoSeg=53;
            //      Menu_lever3_Pub_select_cnt=2;
            //   }
            //   else
            //   {
            //      Seg_rev_lenth=19;
            //      Seg_adds_bak=Menu_lever3_PoSeg=29;
            //      Menu_lever3_Pub_select_cnt=1;
            //   }
            //
            //   for(i=0;i<Seg_rev_lenth;i++)
            //   {
            //       Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i];
            //       Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i]|=0x01;
            //   }
            //   --Seg_rev_lenth;
            //}
            break;
        case 12:
            data_buff[0]='Y';
            data_buff[1]=17;
            data_buff[2]='A';
            data_buff[3]='X';
            data_buff[4]=12;
            data_buff[5]=13;
            data_buff[6]='B';
            data_buff[7]='X';
            data_buff[8]=11;
            data_buff[9]=13;
            data_buff[10]='C';
            data_buff[11]='X';
            data_buff[12]=13;
            data_buff[13]='D';
            for(i=0; i<14; i++)
                Get_6x8_char_single(3,i*6+18,data_buff[i]);

            Seg_rev_lenth=7;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 0:
            case 1:
                Seg_adds_bak=29;
                break;
            case 2:
                Seg_adds_bak=29+24;
                break;
            case 3:
                Seg_adds_bak=29+48;
                break;
            case 4:
                Seg_adds_bak=29+66;
                break;
            }

            for(i=0; i<Seg_rev_lenth; i++)
            {
                Disp_buff128x64[128*3+Seg_adds_bak+i]=~Disp_buff128x64[128*3+Seg_adds_bak+i];
                Disp_buff128x64[128*4+Seg_adds_bak+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Menu_lever3_PoSeg=29;
/////                    Menu_lever3_Pub_select_cnt=1;
            break;
        case 13:
            data_buff[0]=13;  			//±

            j=Allow_error>>12;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[1]=Get_ascii_data(j);
            j=Allow_error>>8;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            j=Allow_error>>4;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            data_buff[4]=19;
            j=Allow_error;
            j&=0x0f;
            data_buff[5]=Get_ascii_data(j);
            data_buff[6]=0;			//%
            for(i=0; i<7; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<5; i++)
                Disp_buff128x64[128*3+30+i]|=0x80;    		//add _

            Disp_buff128x64[128*3+66+0]|=0x40;
            Disp_buff128x64[128*3+66+1]|=0x26;
            Disp_buff128x64[128*3+66+2]|=0x16;
            Disp_buff128x64[128*3+66+3]|=0x68;
            Disp_buff128x64[128*3+66+4]|=0x66;

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+35+i]=~Disp_buff128x64[128*3+35+i];
                Disp_buff128x64[128*4+35+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=35;
            break;
        case 14:
            j=M_lever3_HCPD_temp>>16;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=M_lever3_HCPD_temp>>12;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[1]=Get_ascii_data(j);
            j=M_lever3_HCPD_temp>>8;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[2]=Get_ascii_data(j);
            data_buff[3]=19;
            j=M_lever3_HCPD_temp>>4;
            j&=0x0f;
            data_buff[4]=Get_ascii_data(j);
            j=M_lever3_HCPD_temp;
            j&=0x0f;
            data_buff[5]=Get_ascii_data(j);

            if((M_menur1_unit_select_temp==3)||(M_menur1_unit_select_temp==5))
                data_buff[6]='U';
            else if((M_menur1_unit_select_temp==2)||(M_menur1_unit_select_temp==4))
                data_buff[6]='M';
            else
                data_buff[6]=0;

            data_buff[7]='G';
            data_buff[8]=18;
            data_buff[9]='L';
            for(i=0; i<10; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 15:
            j=Alarm_low_nd>>16;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=Alarm_low_nd>>12;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            j=Alarm_low_nd>>8;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            j=Alarm_low_nd>>4;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            data_buff[4]=19;
            j=Alarm_low_nd;
            j&=0x0f;
            data_buff[5]=Get_ascii_data(j);
            for(i=0; i<6; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 16:
            j=Alarm_hight_nd>>16;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=Alarm_hight_nd>>12;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            j=Alarm_hight_nd>>8;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            j=Alarm_hight_nd>>4;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            data_buff[4]=19;
            j=Alarm_hight_nd;
            j&=0x0f;
            data_buff[5]=Get_ascii_data(j);
            for(i=0; i<6; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 18:	//是否消解
            if(M_menur1_language_select_temp==2)
            {
                Get_16x16_char_single(3,30,120);
                Get_8x16_char_single(3,13+30,11);
                Get_16x16_char_single(3,24+30,121);

                Menu_lever3_PoPa=3;
                if(Disple_flag==2)
                {
                    Seg_rev_lenth=13;
                    Seg_adds_bak=Menu_lever3_PoSeg=53;
                    Menu_lever3_Pub_select_cnt=2;
                }
                else
                {
                    Seg_rev_lenth=13;
                    Seg_adds_bak=Menu_lever3_PoSeg=29;
                    Menu_lever3_Pub_select_cnt=1;
                }

                for(i=0; i<Seg_rev_lenth; i++)
                {
                    Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i];
                    Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i];
                }
                --Seg_rev_lenth;
            }
            //else	//20140216
            //{
            //   data_buff[0]='Y';
            //   data_buff[1]='E';
            //   data_buff[2]='S';
            //   data_buff[3]=18;
            //   data_buff[4]='N';
            //   data_buff[5]='O';
            //   for(i=0;i<6;i++)
            //      Get_6x8_char_single(3,i*6+30,data_buff[i]);
            //
            //   Menu_lever3_PoPa=3;
            //   if(Disple_flag==2)
            //   {
            //      Seg_rev_lenth=13;
            //      Seg_adds_bak=Menu_lever3_PoSeg=53;
            //      Menu_lever3_Pub_select_cnt=2;
            //   }
            //   else
            //   {
            //      Seg_rev_lenth=19;
            //      Seg_adds_bak=Menu_lever3_PoSeg=29;
            //      Menu_lever3_Pub_select_cnt=1;
            //   }
            //
            //   for(i=0;i<Seg_rev_lenth;i++)
            //   {
            //       Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i];
            //       Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i]|=0x01;
            //   }
            //   --Seg_rev_lenth;
            //}
            break;
        }
        break;
    case 3:
        switch(Menu_index_finc_select)
        {
        case 1:
            if(M_menur1_language_select_temp==2)
            {
                data_buff[0]=82;
                data_buff[1]=83;
                data_buff[2]=19;
                data_buff[3]=129;
                data_buff[4]=130;
                data_buff[5]=19;

                for(i=0; i<3; i++)
                {
                    Get_16x16_char_single(3,12+12*i,data_buff[i]);
                    Get_16x16_char_single(3,48+12+12*i,data_buff[i+3]);
                }
                Get_8x16_char_single(3,37+12,11);

                Menu_lever3_PoPa=3;
                if(M_menur1_LZDJ_select_temp==2)
                {
                    Seg_rev_lenth=37;
                    Seg_adds_bak=Menu_lever3_PoSeg=59;
                    Menu_lever3_Pub_select_cnt=2;
                }
                else
                {
                    Seg_rev_lenth=37;
                    Seg_adds_bak=Menu_lever3_PoSeg=11;
                    Menu_lever3_Pub_select_cnt=1;
                }

                for(i=0; i<Seg_rev_lenth; i++)
                {
                    Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i];
                    Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i];
                }
                --Seg_rev_lenth;
            }
            //else	//20140216
            //{
            //   data_buff[0]='I';
            //   data_buff[1]=0;
            //   data_buff[2]='S';
            //   data_buff[3]=0;
            //   data_buff[4]='E';
            //   data_buff[5]=18;
            //   data_buff[6]='C';
            //   data_buff[7]='O';
            //   data_buff[8]='L';
            //   data_buff[9]='O';
            //   data_buff[10]='R';
            //   data_buff[11]='I';
            //   for(i=0;i<12;i++)
            //      Get_6x8_char_single(3,i*6+30,data_buff[i]);
            //
            //   Menu_lever3_PoPa=3;
            //   if(M_menur1_LZDJ_select_temp==1)			//电极法
            //   {
            //       Seg_rev_lenth=31;
            //       Seg_adds_bak=Menu_lever3_PoSeg=29;
            //       Menu_lever3_Pub_select_cnt=1;
            //   }
            //   else
            //   {
            //       Seg_rev_lenth=37;
            //       Seg_adds_bak=Menu_lever3_PoSeg=65;		//比色法
            //       Menu_lever3_Pub_select_cnt=2;
            //   }
            //
            //   for(i=0;i<Seg_rev_lenth;i++)
            //   {
            //       Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i];
            //       Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i]|=0x01;
            //   }
            //   --Seg_rev_lenth;
            //}
            break;
        case 2:
            if(M_menur1_language_select_temp==2)
            {
                data_buff[0]=154;
                data_buff[1]=155;
                data_buff[2]=19;
                data_buff[3]=156;
                data_buff[4]=157;
                data_buff[5]=19;
                for(i=0; i<3; i++)
                {
                    Get_16x16_char_single(3,12+12*i,data_buff[i]);
                    Get_16x16_char_single(3,48+12+12*i,data_buff[i+3]);
                }
                Get_8x16_char_single(3,37+12,11);

                Menu_lever3_PoPa=3;
                if(M_menur1_DL_select_temp==2)
                {
                    Seg_rev_lenth=37;
                    Seg_adds_bak=Menu_lever3_PoSeg=59;
                    Menu_lever3_Pub_select_cnt=2;
                }
                else
                {
                    Seg_rev_lenth=37;
                    Seg_adds_bak=Menu_lever3_PoSeg=11;
                    Menu_lever3_Pub_select_cnt=1;
                }

                for(i=0; i<Seg_rev_lenth; i++)
                {
                    Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i];
                    Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i];
                }
                --Seg_rev_lenth;
            }
            //else	//20140216
            //{
            //   data_buff[0]='A';
            //   data_buff[1]='D';
            //   data_buff[2]='D';
            //   data_buff[3]='I';
            //   data_buff[4]='T';
            //   data_buff[5]='I';
            //   data_buff[6]='O';
            //   data_buff[7]='N';
            //   data_buff[8]=18;
            //   data_buff[9]='C';
            //   data_buff[10]='U';
            //   data_buff[11]='R';
            //   data_buff[12]='V';
            //   data_buff[13]='E';
            //   data_buff[14]=0;
            //   for(i=0;i<15;i++)
            //      Get_6x8_char_single(3,i*6+30,data_buff[i]);
            //
            //   Menu_lever3_PoPa=3;
            //   if(M_menur1_DL_select_temp==1)			//加入法
            //   {
            //       Seg_rev_lenth=49;
            //       Seg_adds_bak=Menu_lever3_PoSeg=29;
            //       Menu_lever3_Pub_select_cnt=1;
            //   }
            //   else
            //   {
            //       Seg_rev_lenth=31;
            //       Seg_adds_bak=Menu_lever3_PoSeg=83;		//曲线法
            //       Menu_lever3_Pub_select_cnt=2;
            //   }
            //
            //   for(i=0;i<Seg_rev_lenth;i++)
            //   {
            //       Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i];
            //       Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i]|=0x01;
            //   }
            //   --Seg_rev_lenth;
            //}
            break;
        case 3:
            j=M_menur1_led_dianliu_temp>>16;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=M_menur1_led_dianliu_temp>>12;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            data_buff[2]=19;
            j=M_menur1_led_dianliu_temp>>8;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            j=M_menur1_led_dianliu_temp>>4;
            j&=0x0f;
            data_buff[4]=Get_ascii_data(j);
            j=M_menur1_led_dianliu_temp;
            j&=0x0f;
            data_buff[5]=Get_ascii_data(j);
            data_buff[6]='M';
            data_buff[7]='A';
            for(i=0; i<8; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 4:			//波长
            j=M_menur1_led_data_temp>>12;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=M_menur1_led_data_temp>>8;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            j=M_menur1_led_data_temp>>4;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            data_buff[3]=19;
            j=M_menur1_led_data_temp;
            j&=0x0f;
            data_buff[4]=Get_ascii_data(j);

            data_buff[5]='N';
            data_buff[6]='M';
            for(i=0; i<7; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 5:
            j=M_menur1_JB_speed_temp>>8;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=M_menur1_JB_speed_temp>>4;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            j=M_menur1_JB_speed_temp;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            for(i=0; i<3; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 6:
            if(M_menur1_language_select_temp==2)
            {
                data_buff[0]=158;
                data_buff[1]=160;
                data_buff[2]=161;
                data_buff[3]=159;
                data_buff[4]=160;
                data_buff[5]=161;
                for(i=0; i<3; i++)
                {
                    Get_16x16_char_single(3,12+12*i,data_buff[i]);
                    Get_16x16_char_single(3,48+12+12*i,data_buff[i+3]);
                }
                Get_8x16_char_single(3,37+12,11);

                Menu_lever3_PoPa=3;
                if(M_menur1_direction_select_temp==2)
                {
                    Seg_rev_lenth=37;
                    Seg_adds_bak=Menu_lever3_PoSeg=59;
                    Menu_lever3_Pub_select_cnt=2;
                }
                else
                {
                    Seg_rev_lenth=37;
                    Seg_adds_bak=Menu_lever3_PoSeg=11;
                    Menu_lever3_Pub_select_cnt=1;
                }

                for(i=0; i<Seg_rev_lenth; i++)
                {
                    Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i];
                    Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i];
                }
                --Seg_rev_lenth;
            }
            //else	//20140216
            //{
            //   data_buff[0]=13;
            //   data_buff[1]=18;
            //   data_buff[2]=14;
            //
            //   for(i=0;i<3;i++)
            //      Get_6x8_char_single(3,i*6+30,data_buff[i]);
            //
            //   Menu_lever3_PoPa=3;
            //   Seg_rev_lenth=7;
            //   if(M_menur1_direction_select_temp==1)		//顺时针
            //   {
            //       Seg_adds_bak=Menu_lever3_PoSeg=29;
            //       Menu_lever3_Pub_select_cnt=1;
            //   }
            //   else
            //   {;
            //       Seg_adds_bak=Menu_lever3_PoSeg=41;		//逆时针
            //       Menu_lever3_Pub_select_cnt=2;
            //   }
            //
            //   for(i=0;i<Seg_rev_lenth;i++)
            //   {
            //       Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i];
            //       Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i]|=0x01;
            //   }
            //   --Seg_rev_lenth;
            //}
            break;
        case 7:
            j=M_menur1_ZSB_speed_temp>>4;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=M_menur1_ZSB_speed_temp;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            for(i=0; i<2; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 8:
            data_buff[0]='1';
            data_buff[1]=14;
            data_buff[2]='2';
            data_buff[3]=14;
            data_buff[4]='3';
            for(i=0; i<5; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            Menu_lever3_PoPa=3;
            Menu_lever3_PoSeg=29;
            Menu_lever3_Pub_select_cnt=M_menur1_dot_num_select_temp;
            switch(M_menur1_dot_num_select_temp)
            {
            case 1:
                Seg_adds_bak=29;
                break;
            case 2:
                Seg_adds_bak=41;
                break;
            case 3:
                Seg_adds_bak=53;
                break;
            }

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+Seg_adds_bak+i]=~Disp_buff128x64[128*3+Seg_adds_bak+i];
                Disp_buff128x64[128*4+Seg_adds_bak+i]|=0x01;
            }
            break;
        case 9:	//sample Volume
            j=M_menur1_sample_v_temp>>12;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=M_menur1_sample_v_temp>>8;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            data_buff[2]=19;
            j=M_menur1_sample_v_temp>>4;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            j=M_menur1_sample_v_temp;
            j&=0x0f;
            data_buff[4]=Get_ascii_data(j);
            data_buff[5]='M';
            data_buff[6]='L';
            for(i=0; i<7; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 10:
            j=M_menur1_jz1nd_temp>>16;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=M_menur1_jz1nd_temp>>12;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            j=M_menur1_jz1nd_temp>>8;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            data_buff[3]=19;
            j=M_menur1_jz1nd_temp>>4;
            j&=0x0f;
            data_buff[4]=Get_ascii_data(j);
            j=M_menur1_jz1nd_temp;
            j&=0x0f;
            data_buff[5]=Get_ascii_data(j);
            data_buff[7]='G';
            data_buff[8]=18;
            data_buff[9]='L';
            switch(M_menur1_unit_select_temp)
            {
            case 1:
                data_buff[6]='G';
                data_buff[7]=18;
                data_buff[8]='L';
                data_buff[9]=0;
                break;
            case 2:
                data_buff[6]='M';
                break;
            case 3:
                data_buff[6]='U';
                break;
            case 4:
                data_buff[6]='P';
                data_buff[7]='P';
                data_buff[8]='M';
                data_buff[9]=0;
                break;
            case 5:
                data_buff[6]='P';
                data_buff[7]='P';
                data_buff[8]='B';
                data_buff[9]=0;
                break;
            }
            for(i=0; i<10; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 11:
            j=M_menur1_jz1_v_temp>>12;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=M_menur1_jz1_v_temp>>8;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            data_buff[2]=19;
            j=M_menur1_jz1_v_temp>>4;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            j=M_menur1_jz1_v_temp;
            j&=0x0f;
            data_buff[4]=Get_ascii_data(j);
            data_buff[5]='M';
            data_buff[6]='L';
            for(i=0; i<7; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 12:
            j=M_menur1_jz2nd_temp>>16;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=M_menur1_jz2nd_temp>>12;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            j=M_menur1_jz2nd_temp>>8;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            j=M_menur1_jz2nd_temp>>4;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            data_buff[4]=19;
            j=M_menur1_jz2nd_temp;
            j&=0x0f;
            data_buff[5]=Get_ascii_data(j);
            data_buff[7]='G';
            data_buff[8]=18;
            data_buff[9]='L';
            switch(M_menur1_unit_select_temp)
            {
            case 1:
                data_buff[6]='G';
                data_buff[7]=18;
                data_buff[8]='L';
                data_buff[9]=0;
                break;
            case 2:
                data_buff[6]='M';
                break;
            case 3:
                data_buff[6]='U';
                break;
            case 4:
                data_buff[6]='P';
                data_buff[7]='P';
                data_buff[8]='M';
                data_buff[9]=0;
                break;
            case 5:
                data_buff[6]='P';
                data_buff[7]='P';
                data_buff[8]='B';
                data_buff[9]=0;
                break;
            }
            for(i=0; i<10; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 13:
////                    data_buff[0]=0;			//check -
            j=M_menur1_jz2_v_temp>>8;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            data_buff[1]=19;
            j=M_menur1_jz2_v_temp>>4;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            j=M_menur1_jz2_v_temp;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            data_buff[4]='M';
            data_buff[5]='L';
            data_buff[6]=18;

            if(M_menur1_language_select_temp==2)
            {
                for(i=0; i<6; i++)
                    Get_8x16_char_single(3,i*10+12,data_buff[i]);

                Get_16x16_char_single(3,82,36);
                Get_16x16_char_single(3,94,172);

                Get_8x16_char_single(3,60+12,11);
                for(i=0; i<10; i++)
                {
                    Disp_buff128x64[128*3+22+i]=0x00;
                    Disp_buff128x64[128*4+22+i]=0x00;
                }
                Disp_buff128x64[128*4+22+4]=0x30;
                Disp_buff128x64[128*4+22+5]=0x30;
            }
            //else     	//20140216
            //{
            //   data_buff[7]='D';
            //   data_buff[8]='Y';
            //   data_buff[9]='N';
            //   data_buff[10]='A';
            //   data_buff[11]='M';
            //   data_buff[12]='I';
            //   data_buff[13]='C';
            //   for(i=0;i<14;i++)
            //      Get_6x8_char_single(3,i*6+30,data_buff[i]);
            //}

            Menu_lever3_PoPa=3;
            if(M_dongtai_flag==1)				//动态
            {
                Menu_lever3_Pub_select_cnt=4;
                Seg_adds_bak=Menu_lever3_PoSeg=71;
                if(M_menur1_language_select_temp==2)
                {
                    Seg_rev_lenth=25;
                    Seg_adds_bak=Menu_lever3_PoSeg=81;
                }
                else
                    Seg_rev_lenth=43;
            }
            else
            {
                Menu_lever3_Pub_select_cnt=1;
                if(M_menur1_language_select_temp==2)
                {
                    Seg_rev_lenth=11;
                    Seg_adds_bak=Menu_lever3_PoSeg=11;
                }
                else
                {
                    Seg_rev_lenth=7;
                    Seg_adds_bak=Menu_lever3_PoSeg=29;
                }
            }
            if(M_menur1_language_select_temp==2)
            {
                for(i=0; i<Seg_rev_lenth; i++)
                {
                    Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i];
                    Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i];
                }
                --Seg_rev_lenth;
            }
            //else	//20140216
            //{
            //    for(i=0;i<Seg_rev_lenth;i++)
            //    {
            //        Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i]=~Disp_buff128x64[128*Menu_lever3_PoPa+Menu_lever3_PoSeg+i];
            //        Disp_buff128x64[128*(Menu_lever3_PoPa+1)+Menu_lever3_PoSeg+i]|=0x01;
            //    }
            //    --Seg_rev_lenth;
            //}
            break;
        case 14:
            j=M_menur1_sj1_v_temp>>4;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            data_buff[1]=19;
            j=M_menur1_sj1_v_temp;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            data_buff[3]='M';
            data_buff[4]='L';
            for(i=0; i<5; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 15:
            j=M_menur1_sj2_v_temp>>4;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            data_buff[1]=19;
            j=M_menur1_sj2_v_temp;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            data_buff[3]='M';
            data_buff[4]='L';
            for(i=0; i<5; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 16:
            j=M_menur1_sj3_v_temp>>4;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            data_buff[1]=19;
            j=M_menur1_sj3_v_temp;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            data_buff[3]='M';
            data_buff[4]='L';
            for(i=0; i<5; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 17:
            j=M_menur1_sj4_v_temp>>4;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            data_buff[1]=19;
            j=M_menur1_sj4_v_temp;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            data_buff[3]='M';
            data_buff[4]='L';
            for(i=0; i<5; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 18:
            j=Prj_correlat>>28;
            j&=0x0f;
            if(j!=2)
                data_buff[0]=13;
            else if(j==2)
                data_buff[0]=14;
            Menu_lever3_Pub_adj_cnt=j;

            j=Prj_correlat>>20;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            j=Prj_correlat>>16;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            j=Prj_correlat>>12;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            data_buff[4]=19;
            j=Prj_correlat>>8;
            j&=0x0f;
            data_buff[5]=Get_ascii_data(j);
            j=Prj_correlat>>4;
            j&=0x0f;
            data_buff[6]=Get_ascii_data(j);
            j=Prj_correlat;
            j&=0x0f;
            data_buff[7]=Get_ascii_data(j);
            for(i=0; i<8; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 20:
            j=Set_xishu>>24;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=Set_xishu>>20;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            j=Set_xishu>>16;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            j=Set_xishu>>12;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            data_buff[4]=19;
            j=Set_xishu>>8;
            j&=0x0f;
            data_buff[5]=Get_ascii_data(j);
            j=Set_xishu>>4;
            j&=0x0f;
            data_buff[6]=Get_ascii_data(j);
            j=Set_xishu;
            j&=0x0f;
            data_buff[7]=Get_ascii_data(j);
            for(i=0; i<8; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        case 21:
            j=set_cal_data>>24;
            j&=0x0f;
            Menu_lever3_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=set_cal_data>>20;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            j=set_cal_data>>16;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            j=set_cal_data>>12;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            data_buff[4]=19;
            j=set_cal_data>>8;
            j&=0x0f;
            data_buff[5]=Get_ascii_data(j);
            j=set_cal_data>>4;
            j&=0x0f;
            data_buff[6]=Get_ascii_data(j);
            j=set_cal_data;
            j&=0x0f;
            data_buff[7]=Get_ascii_data(j);
            for(i=0; i<8; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Menu_lever3_PoPa=3;
            Seg_adds_bak=Menu_lever3_PoSeg=29;
            break;
        }
        break;
    }
    if((Menu_index_select==7)||(Menu_index_select==2)||(Menu_index_select==3))
    {
        if((Menu_index_select==3)&&(Menu_index_finc_select==19))
            goto sub_231;

    }
//---------------------------------------------------------------------------
    else
    {
sub_231:
        if(M_menur1_language_select_temp!=2)	//20140216
        {
            //if((Menu_lever3_select>=6)&&(Menu_lever3_point>1)&&(Menu_lever3_point<6))
            //   m=Menu_lever3_select+(6-Menu_lever3_point)-6;
            //else if((Menu_lever3_select>6)&&(Menu_lever3_point==1))
            //   m=Menu_lever3_select-Menu_lever3_point;
            //else if(Menu_lever3_select>6)
            //   m=Menu_lever3_select-6;
            //else
            //   m=Menu_lever3_select-Menu_lever3_point;
            //
            //switch(Menu_index_select)
            //{
            //    case 1:
            //        switch(Menu_index_finc_select)
            //        {
            //           case 1:
            //                Lever3_menu_line=6;
            //         	    index= Lever311_en_menu_index+m;	//char num.
            //           break;
            //           case 2:
            //                Lever3_menu_line=2;
            //         	    index= Lever312_en_menu_index+m;	//char num.
            //           break;
            //           case 3:
            //                Lever3_menu_line=3;
            //         	    index= Lever313_en_menu_index+m;	//char num.
            //           break;
            //           case 4:
            //                Lever3_menu_line=2;
            //         	    index= Lever314_en_menu_index+m;	//char num.
            //           break;
            //       }
            //    break;
            //    case 3:
            //        Lever3_menu_line=3;
            //        if(Menu_index_finc_select==19)
            //            index= Lever319_en_menu_index+m;	//char num.
            //    break;
            //    case 4:
            //        switch(Menu_index_finc_select)
            //        {
            //           case 1:
            //                Lever3_menu_line=MENU_LV331_NUM;
            //         	    index= Lever341_en_menu_index+m;	//char num.
            //           break;
            //           case 2:
            //                Lever3_menu_line=MENU_LV332_NUM;
            //         	    index= Lever342_en_menu_index+m;	//char num.
            //           break;
            //           case 3:
            //                Lever3_menu_line=MENU_LV333_NUM;
            //         	    index= Lever343_en_menu_index+m;	//char num.
            //           break;
            //           case 4:
            //                Lever3_menu_line=MENU_LV334_NUM;
            //         	    index= Lever344_en_menu_index+m;	//char num.
            //           break;
            //           case 5:
            //                Lever3_menu_line=MENU_LV335_NUM;
            //         	    index= Lever345_en_menu_index+m;	//char num.
            //           break;
            //           case 6:
            //                Lever3_menu_line=MENU_LV336_NUM;
            //         	    index= Lever346_en_menu_index+m;	//char num.
            //           break;
            //           case 7:
            //                Lever3_menu_line=MENU_LV337_NUM;
            //         	    index= Lever347_en_menu_index+m;	//char num.
            //           break;
            //           case 8:
            //                Lever3_menu_line=MENU_LV338_NUM;
            //         	    index= Lever348_en_menu_index+m;	//char num.
            //           break;
            //           case 9:				//照明灯
            //                Lever3_menu_line=MENU_LV339_NUM;
            //         	    index= Lever349_en_menu_index+m;	//char num.
            //           break;
            //        }
            //    break;
            //    case 5:
            //        switch(Menu_index_finc_select)
            //        {
            //           case 1:
            //                if(Fenxi.disp_index==0)
            //                {
            //	       Fenxi.disp_index=1;
            //	       disp_sec_screen_flag=1;
            //	    }
            //	    Read_result_from_flash(1);
            //           break;
            //           case 2:
            //                if(Jiaozheng.disp_index==0)
            //                {
            //	       Jiaozheng.disp_index=1;
            //	       disp_sec_screen_flag=1;
            //	    }
            //            Read_result_from_flash(2);
            //           break;
            //           case 3:
            //                if(Baojing.disp_index==0)
            //                {
            //	       Baojing.disp_index=1;
            //	       disp_sec_screen_flag=1;
            //	    }
            //	    Read_result_from_flash(4);
            //           break;
            //           case 4:
            //                if(Cuowu.disp_index==0)
            //                {
            //	       Cuowu.disp_index=1;
            //	       disp_sec_screen_flag=1;
            //	    }
            //	    Read_result_from_flash(3);
            //           break;
            //       }
            //    break;
            //    case 6:
            //        switch(Menu_index_finc_select)
            //        {
            //           case 1:
            //                Lever3_menu_line=5;
            //         	    index= Lever361_en_menu_index+m;	//char num.
            //           break;
            //           case 2:
            //                Lever3_menu_line=2; 			//Line=12; 2012-9-17 14:42
            //         	    index= Lever362_en_menu_index+m;	//char num.
            //           break;
            //           case 3:
            //                Lever3_menu_line=8;
            //         	    index= Lever363_en_menu_index+m;	//char num.
            //           break;
            //           case 4:
            //                Lever3_menu_line=4;
            //         	    index= Lever364_en_menu_index+m;	//char num.
            //           break;
            //           case 5:
            //                Lever3_menu_line=1;
            //         	    index= Lever365_en_menu_index+m;	//char num.
            //           break;
            //           case 6:
            //                Lever3_menu_line=1;
            //         	    index= Lever366_en_menu_index+m;	//char num.
            //           break;
            //       }
            //    break;
            //}
            //if(Menu_index_select!=5)
            //{
            //for(i=0;(i<6)&&(i<Lever3_menu_line);i++)
            //{
            //      if(Menu_lever3_select>=6)
            //      {
            //         if(Menu_lever3_point==6)
            //            jj=(i+Menu_lever3_select-6)*16;
            //         else if(Menu_lever3_point==1)
            //            jj=(i+Menu_lever3_select-1)*16;
            //         else
            //            jj=(i+(Menu_lever3_select+(6-Menu_lever3_point))-6)*16;
            //      }
            //      else
            //         jj=(i+Menu_lever3_select-Menu_lever3_point)*16;
            //
            //      for(k=0;k<*index;k++)
            //      {
            //          switch(Menu_index_select)
            //          {
            //          case 1:
            //               switch(Menu_index_finc_select)
            //               {
            //               	   case 1:
            //                            Adds_index=Lever311_en_menu_char+jj;	//char data
            //               	   break;
            //               	   case 2:
            //                            Adds_index=Lever312_en_menu_char+jj;	//char data
            //               	   break;
            //               	   case 3:
            //                            Adds_index=Lever313_en_menu_char+jj;	//char data
            //               	   break;
            //               	   case 4:
            //                            Adds_index=Lever314_en_menu_char+jj;	//char data
            //               	   break;
            //               }
            //                   if(i<(Lever3_menu_line-1))
            //                      Get_6x8_char_single(i+2,122,0x5e);
            //          break;
            //              case 3:
            //                   if(Menu_index_finc_select==19)
            //                       Adds_index=Lever319_en_menu_char+jj;	//char data
            //              break;
            //          case 4:
            //               switch(Menu_index_finc_select)
            //               {
            //               	   case 1:
            //                            Adds_index=Lever341_en_menu_char+jj;	//char data
            //               	   break;
            //               	   case 2:
            //                            Adds_index=Lever342_en_menu_char+jj;	//char data
            //                           if(i>0)
            //                               Get_6x8_char_single(i+2,122,0x5e);
            //               	   break;
            //               	   case 3:
            //                            Adds_index=Lever343_en_menu_char+jj;	//char data
            //                            Get_6x8_char_single(i+2,122,0x5e);
            //               	   break;
            //               	   case 4:
            //                            Adds_index=Lever344_en_menu_char+jj;	//char data
            //                            Get_6x8_char_single(i+2,122,0x5e);
            //               	   break;
            //               	   case 5:
            //                            Adds_index=Lever345_en_menu_char+jj;	//char data
            //                            if(i<2)
            //                            Get_6x8_char_single(i+2,122,0x5e);
            //               	   break;
            //               	   case 6:
            //                            Adds_index=Lever346_en_menu_char+jj;	//char data
            //                            Get_6x8_char_single(i+2,122,0x5e);
            //               	   break;
            //               	   case 7:
            //                            Adds_index=Lever347_en_menu_char+jj;	//char data
            //                            Get_6x8_char_single(i+2,122,0x5e);
            //               	   break;
            //               	   case 8:
            //                            Adds_index=Lever348_en_menu_char+jj;	//char data
            //                            Get_6x8_char_single(i+2,122,0x5e);
            //               	   break;
            //               	   case 9:
            //                            Adds_index=Lever349_en_menu_char+jj;	//char data
            //               	   break;
            //               }
            //          break;
            //          case 6:
            //               switch(Menu_index_finc_select)
            //               {
            //               	   case 1:
            //                            Adds_index=Lever361_en_menu_char+jj;	//char data
            //                            if(i<(Lever3_menu_line-1))
            //                               Get_6x8_char_single(i+2,122,0x5e);
            //               	   break;
            //               	   case 2:
            //                            Adds_index=Lever362_en_menu_char+jj;	//char data
            //                            Get_6x8_char_single(i+2,122,0x5e);
            //               	   break;
            //               	   case 3:
            //                            Adds_index=Lever363_en_menu_char+jj;	//char data
            //                            Get_6x8_char_single(i+2,122,0x5e);
            //               	   break;
            //               	   case 4:
            //                            Adds_index=Lever364_en_menu_char+jj;	//char data
            //                            Get_6x8_char_single(i+2,122,0x5e);
            //               	   break;
            //               	   case 5:
            //                            Adds_index=Lever365_en_menu_char+jj;	//char data
            //               	   break;
            //               	   case 6:
            //                            Adds_index=Lever366_en_menu_char+jj;	//char data
            //               	   break;
            //               }
            //          break;
            //          }
            //          Get_6x8_char_single(i+2,k*6+6,*Adds_index);
            //          ++jj;
            //      }
            //      ++index;
            //}
            //   for(k=0;k<128;k++)
            //       Disp_buff128x64[(Menu_lever3_point+1)*128+k]=~Disp_buff128x64[(Menu_lever3_point+1)*128+k];
            //
            //   if(Menu_lever3_select==1)
            //      Get_6x8_char_single(0,122,0x5c);
            //   else if(Menu_lever3_select==Lever3_menu_line)
            //      Get_6x8_char_single(0,122,0x5b);
            //   else
            //      Get_6x8_char_single(0,122,0x5d);
            //}
        }
//----------------------------------------------------------------------------------
        else if((Menu_index_select!=2)&&(Menu_index_select!=7))
        {
            Line=2;

            if((Menu_lever3_select>=4)&&(Menu_lever3_point>1)&&(Menu_lever3_point<4))
                m=Menu_lever3_select+(4-Menu_lever3_point)-4;
            else if((Menu_lever3_select>4)&&(Menu_lever3_point==1))
                m=Menu_lever3_select-Menu_lever3_point;
            else if(Menu_lever3_select>4)
                m=Menu_lever3_select-4;
            else
                m=Menu_lever3_select-Menu_lever3_point;

            if((Menu_index_select==3)&&(Menu_index_finc_select==19))
            {
                j=1;
                Clear_n_page_ram(2,5);
            }
            else
            {
                j=0;
                Clear_n_page_ram(0,7);
            }

            for(i=j; (i<4)&&(i<Line); i++)
            {
                switch(Menu_index_select)
                {
                case 3:
                    if(Menu_index_finc_select==19)
                    {
                        Line=MENU_LV2319_NUM+1;
                        Adds_index=Lever3_menu319+m*7;
                        index= Lever3_menu319_index+m;
                    }
                    break;
                case 4:
                    switch(Menu_index_finc_select)
                    {
                    case 1:
                        Line=MENU_LV331_NUM;
                        Adds_index=Lever3_menu31+m*7;
                        index= Lever3_menu31_index+m;
                        break;
                    case 2:
                        Line=MENU_LV332_NUM;
                        Adds_index=Lever3_menu32+m*7;
                        index= Lever3_menu32_index+m;
                        if(i>0)
                            Get_8x16_char_single(i*2,118,'Z'+1);
                        break;
                    case 3:
                        Line=MENU_LV333_NUM;
                        Adds_index=Lever3_menu33+m*7;
                        index= Lever3_menu33_index+m;
                        Get_8x16_char_single(i*2,118,'Z'+1);
                        break;
                    case 4:
                        Line=MENU_LV334_NUM;
                        Adds_index=Lever3_menu34+m*7;
                        index= Lever3_menu34_index+m;
                        Get_8x16_char_single(i*2,118,'Z'+1);
                        break;
                    case 5:
                        Line=MENU_LV335_NUM;
                        Adds_index=Lever3_menu37+m*7;
                        index= Lever3_menu37_index+m;
                        if(i<(Line-2))
                            Get_8x16_char_single(i*2,118,'Z'+1);
                        break;
                    case 6:
                        Line=MENU_LV336_NUM;
                        Adds_index=Lever3_menu38+m*7;
                        index= Lever3_menu38_index+m;
                        Get_8x16_char_single(i*2,118,'Z'+1);
                        break;
                    case 7:
                        Line=MENU_LV337_NUM;
                        Adds_index=Lever3_menu39+m*7;
                        index= Lever3_menu39_index+m;
                        Get_8x16_char_single(i*2,118,'Z'+1);
                        break;
                    case 8:
                        Line=MENU_LV338_NUM;
                        Adds_index=Lever3_menu310+m*7;
                        index= Lever3_menu310_index+m;
                        Get_8x16_char_single(i*2,118,'Z'+1);
                        break;
                    case 9:				//照明灯
                        Line=MENU_LV339_NUM;
                        Adds_index=Lever3_menu311+m*7;
                        index= Lever3_menu311_index+m;
                        break;
                    }
                    break;
                case 5:
                    switch(Menu_index_finc_select)
                    {
                    case 1:
                        if(Fenxi.disp_index==0)
                        {
                            Fenxi.disp_index=1;
                            disp_sec_screen_flag=1;
                        }
                        Read_result_from_flash(1);
                        break;
                    case 2:
                        if(Jiaozheng.disp_index==0)
                        {
                            Jiaozheng.disp_index=1;
                            disp_sec_screen_flag=1;
                        }
                        Read_result_from_flash(2);
                        break;
                    case 3:
                        if(Baojing.disp_index==0)
                        {
                            Baojing.disp_index=1;
                            disp_sec_screen_flag=1;
                        }
                        Read_result_from_flash(4);
                        break;
                    case 4:
                        if(Cuowu.disp_index==0)
                        {
                            Cuowu.disp_index=1;
                            disp_sec_screen_flag=1;
                        }
                        Read_result_from_flash(3);
                        break;
                    }
                    break;
                case 1:
                    switch(Menu_index_finc_select)
                    {
                    case 1:
                        Line=6;
                        Adds_index=Lever3_menu51+m*7;
                        index= Lever3_menu51_index+m;
                        if(Menu_lever3_point==4)
                        {
                            if((Menu_lever3_select==6)&&(i<3))
                                Get_8x16_char_single(i*2,118,'Z'+1);
                            else if(Menu_lever3_select!=6)
                                Get_8x16_char_single(i*2,118,'Z'+1);
                        }
                        else if((Menu_lever3_select>=4)&&(i<3))
                            Get_8x16_char_single(i*2,118,'Z'+1);
                        else if(Menu_lever3_select==3)
                        {
                            if((Menu_lever3_point==1)&&(i<3))
                                Get_8x16_char_single(i*2,118,'Z'+1);
                            else if(Menu_lever3_point!=1)
                                Get_8x16_char_single(i*2,118,'Z'+1);
                        }
                        else if(Menu_lever3_select<3)
                            Get_8x16_char_single(i*2,118,'Z'+1);
                        break;
                    case 2:
                        Line=2;
                        Adds_index=Lever3_menu52+m*7;
                        index= Lever3_menu52_index+m;
                        if(i<(Line-1))
                            Get_8x16_char_single(i*2,118,'Z'+1);
                        break;
                    case 3:
                        Line=3;
                        Adds_index=Lever3_menu53+m*7;
                        index= Lever3_menu53_index+m;
                        if(i<(Line-1))
                            Get_8x16_char_single(i*2,118,'Z'+1);
                        break;
                    case 4:
                        Line=2;
                        Adds_index=Lever3_menu54+m*7;
                        index= Lever3_menu54_index+m;
                        if(i<(Line-1))
                            Get_8x16_char_single(i*2,118,'Z'+1);
                        break;
                    }
                    break;
                case 6:
                    switch(Menu_index_finc_select)
                    {
                    case 1:
                        Line=5;
                        Adds_index=Lever3_menu61+m*7;
                        index= Lever3_menu61_index+m;
                        if(Menu_lever3_point==4)
                        {
                            if((Menu_lever3_select==5)&&(i<3))
                                Get_8x16_char_single(i*2,118,'Z'+1);
                            else if(Menu_lever3_select!=6)
                                Get_8x16_char_single(i*2,118,'Z'+1);
                        }
                        else if((Menu_lever3_select>=4)&&(i<3))
                            Get_8x16_char_single(i*2,118,'Z'+1);
                        else if(Menu_lever3_select==3)
                        {
                            if((Menu_lever3_point==1)&&(i<3))
                                Get_8x16_char_single(i*2,118,'Z'+1);
                            else if(Menu_lever3_point!=1)
                                Get_8x16_char_single(i*2,118,'Z'+1);
                        }
                        else if(Menu_lever3_select<3)
                            Get_8x16_char_single(i*2,118,'Z'+1);
                        break;
                    case 2:
                        Line=2; 			//Line=12; 2012-9-17 14:42
                        Adds_index=Lever3_menu62+m*7;
                        index= Lever3_menu62_index+m;
                        Get_8x16_char_single(i*2,118,'Z'+1);
                        break;
                    case 3:
                        Line=8;
                        Adds_index=Lever3_menu63+m*7;
                        index= Lever3_menu63_index+m;
                        Get_8x16_char_single(i*2,118,'Z'+1);
                        break;
                    case 4:
                        Line=4;
                        Adds_index=Lever3_menu64+m*7;
                        index= Lever3_menu64_index+m;
                        Get_8x16_char_single(i*2,118,'Z'+1);
                        break;
                    case 5:
                        Line=1;
                        Adds_index=Lever3_menu65+m*7;
                        index= Lever3_menu65_index+m;
                        break;
                    case 6:
                        Line=1;
                        Adds_index=Lever3_menu66+m*7;
                        index= Lever3_menu66_index+m;
                        break;
                    }
                    break;
                }
//---------------------------------------------------------------------------
                if(Menu_index_select!=5)
                {
                    for(k=0; k<*index; k++)
                    {
                        Get_16x16_char_single(i*2,k*12+6,*Adds_index);
                        ++Adds_index;
                    }
/////                  Get_8x16_char_single(i*2,118,'Z'+1);
                    ++m;
                }
            }
            if((Menu_index_select==4)&&(Menu_index_finc_select==9))
            {
                for(jj=0; jj<128; jj++)
                {
                    Disp_buff128x64[5*128+jj]=Disp_buff128x64[3*128+jj];
                    Disp_buff128x64[4*128+jj]=Disp_buff128x64[2*128+jj];
                    Disp_buff128x64[3*128+jj]=Disp_buff128x64[128+jj];
                    Disp_buff128x64[2*128+jj]=Disp_buff128x64[jj];
                }
                Adds_index=Lever2_menu4+(Menu_index_finc_select-1)*7;
                index= Lever2_menu4_index+(Menu_index_finc_select-1);
                for(k=0; k<*index; k++)
                {
                    Get_16x16_char_single(0,k*12+6,*Adds_index);
                    ++Adds_index;
                }
                for(jj=0; jj<128; jj++)
                {
                    Disp_buff128x64[128+jj]|=0x80;
                    Disp_buff128x64[Menu_lever3_point*256+jj]|=0x01;
                }
                for(jj=0; jj<256; jj++)
                    Disp_buff128x64[Menu_lever3_point*256+jj]=~Disp_buff128x64[Menu_lever3_point*256+jj];
            }
            else if(Menu_index_select!=5)
            {
                for(jj=0; jj<256; jj++)
                {
                    if((Menu_index_select==3)&&(Menu_index_finc_select==19))
                    {
                        if(jj<128)
                            Disp_buff128x64[(Menu_lever3_point)*256+jj]|=0x01;

                        Disp_buff128x64[(Menu_lever3_point)*256+jj]=~Disp_buff128x64[(Menu_lever3_point)*256+jj];
                    }
                    else
                        Disp_buff128x64[(Menu_lever3_point-1)*256+jj]=~Disp_buff128x64[(Menu_lever3_point-1)*256+jj];
                }
            }
        }
    }
    Display_all_buff();
}

//===============================================================================
//	finction	:Lever4_adj_valu	:LCM use 8080 system
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-13 15:37
//	modefy		:null
//===============================================================================
void Lever4_adj_valu(void)		//第四级菜单参数调整显示控制函数
{
    uint8_t i;

    if((Menu_index_select==2)&&(Menu_index_finc_select==12)&&(Menu_lever4_Pub_select_cnt==1))
    {
        if(Menu_lever4_Pub_adj_cnt==1)
            Get_6x8_char_single(Menu_lever4_PoPa,Seg_adds_bak+1,13);
        else
            Get_6x8_char_single(Menu_lever4_PoPa,Seg_adds_bak+1,14);
    }
    else if((Menu_index_select==4)&&(Menu_index_finc_select==7)&&(Menu_lever3_select==1))
        __nop();
    else if((Menu_index_select==4)&&(Menu_index_finc_select==7)&&(Menu_lever3_select==2)&&(Menu_lever4_Pub_select_cnt==1))
    {
        if(BCWD_NG_flag==1)
            Get_6x8_char_single(Menu_lever4_PoPa,Seg_adds_bak+1,13);
        else
            Get_6x8_char_single(Menu_lever4_PoPa,Seg_adds_bak+1,14);
    }
    else if((Menu_index_select==1)&&(Menu_index_finc_select==1)&&(Menu_lever3_select==1)&&(M_menur1_language_select_temp==2))
    {
        i=Get_ascii_data(Menu_lever4_Pub_adj_cnt);
        Get_8x16_char_single(3,Seg_adds_bak+1,i);

        for(i=0; i<10; i++)
        {
            Disp_buff128x64[128*Menu_lever4_PoPa+Seg_adds_bak+i+1]=~Disp_buff128x64[128*Menu_lever4_PoPa+Seg_adds_bak+i+1];
            Disp_buff128x64[128*(Menu_lever4_PoPa+1)+Seg_adds_bak+i+1]=~Disp_buff128x64[128*(Menu_lever4_PoPa+1)+Seg_adds_bak+i+1];
        }
        goto ex_adj2;
    }
    else if((Menu_index_select!=3)&&(Menu_index_select!=5)&&(Menu_index_select!=7))
    {
        Menu_lever4_Pub_adj_cnt&=0x0f;
        i=Get_ascii_data(Menu_lever4_Pub_adj_cnt);
        Get_6x8_char_single(Menu_lever4_PoPa,Seg_adds_bak+1,i);
    }
    else if(Menu_index_select==5) goto ex_adj2;

    for(i=0; i<6; i++)
    {
        Disp_buff128x64[128*Menu_lever4_PoPa+Seg_adds_bak+i+1]=~Disp_buff128x64[128*Menu_lever4_PoPa+Seg_adds_bak+i+1];
        Disp_buff128x64[128*(Menu_lever4_PoPa+1)+Seg_adds_bak+i+1]|=0x01;
    }
ex_adj2:
    Display_all_buff();
}

//===============================================================================
//	finction	:Display_lever4_en_flag	:LCM use 8080 system
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-17 14:07
//	modefy		:null
//===============================================================================
uint8_t Display_lever4_en_flag(void)			//取允许显示的第四级菜单标志
{
    uint8_t flag;

    flag=1;

    if((Menu_index_select==4)&&(Menu_index_finc_select==9))
        flag=0;
    if((Menu_index_select==4)&&(Menu_index_finc_select==1))
        flag=0;
    if((Menu_index_select==4)&&(Menu_index_finc_select==4))
        flag=0;

    if((Menu_index_select==1)&&(Menu_index_finc_select==1)&&(Menu_lever3_select==6))
        flag=0;

    if((Menu_index_select==1)&&(Menu_index_finc_select==2)&&(Menu_lever3_select==2))
        flag=0;

    if((Menu_index_select==1)&&(Menu_index_finc_select==3)&&(Menu_lever3_select==3))
        flag=0;

    if((Menu_index_select==1)&&(Menu_index_finc_select==4)&&(Menu_lever3_select==2))
        flag=0;

    if((Menu_index_select==2)&&(Menu_index_finc_select!=12))
        flag=0;

    if((Menu_index_select==3)||(Menu_index_select==5)||(Menu_index_select==7))
//    if((Menu_index_select==3)||(Menu_index_select==7))
        flag=0;

    if((Menu_index_select==4)&&(Menu_index_finc_select==2)&&(Menu_lever3_select==1))
        flag=0;

    if((Menu_index_select==4)&&(Menu_index_finc_select==5)&&(Menu_lever3_select==3))
        flag=0;

    if((Menu_index_select==4)&&(Menu_index_finc_select==5)&&(Menu_lever3_select==4))
        flag=0;

//    if((Menu_index_select==6)&&(Menu_index_finc_select==1)&&(Menu_lever3_select==1))
//       flag=0;

    if((Menu_index_select==6)&&(Menu_index_finc_select==1)&&(Menu_lever3_select==5))
        flag=0;

    if(Menu_index_select==6)
    {
        if((Menu_index_finc_select==3)||(Menu_index_finc_select==5)||(Menu_index_finc_select==6))
            flag=0;
    }

    return(flag);
}

//===============================================================================
//	finction	:Display_lever4_index	:LCM use 8080 system
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-7-13 15:33
//	modefy		:null
//===============================================================================
void Display_lever4_index(void)			//第四级菜单显示控制函数
{
    uint8_t i,j,k;
    uint8_t const *index=0;
    uint8_t const *index_bit=0;
    uint8_t const *Adds_index=0;
    uint8_t data_buff[22];		//for setting disp
    uint16_t pb_temp,j16;
    uint32_t calcula_tp,kkk;

    Clear_n_page_ram(0,7);
    Menu_lever4_PoPa=3;
    switch(Menu_index_select)
    {
    case 1:
        switch(Menu_index_finc_select)
        {
        case 1:
            i=Menu_lever3_select-1;
            index= Lever4_m11_char_num+i;
            index_bit= Lever4_m11_bit_num+i;
            switch(Menu_lever3_select)
            {
            case 1:		//分析频率
                j=M_lever4_FXPL_temp>>12;
//                             j=j>>12;
                j&=0x0f;
                Menu_lever4_Pub_adj_cnt=j;
                data_buff[0]=Get_ascii_data(j);
                j=M_lever4_FXPL_temp>>8;
//                             j=j>>8;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);
                j=M_lever4_FXPL_temp>>4;
//                             j=j>>4;
                j&=0x0f;
                data_buff[2]=Get_ascii_data(j);
                j=M_lever4_FXPL_temp;
                j&=0x0f;
                data_buff[3]=Get_ascii_data(j);
                data_buff[4]='M';
                data_buff[5]='I';
                data_buff[6]='N';
                data_buff[7]=18;

                if(M_menur1_language_select_temp==2)
                {
/////                                data_buff[8]=175;
/////                                data_buff[9]=176;
/////                                data_buff[10]=177;
/////                                data_buff[11]=178;

                    for(i=0; i<7; i++)
                        Get_8x16_char_single(3,i*10+20,data_buff[i]);
                    Get_8x16_char_single(3,70+20,11);

                    Get_16x16_char_single(5,40,175);
                    Get_16x16_char_single(5,52,176);
                    Get_16x16_char_single(5,64,177);
                    Get_16x16_char_single(5,76,178);

                    Menu_lever4_PoSeg=19;
                    if(M_lever4_extern_trrig_flag==1)
                    {
                        Seg_adds_bak=39;
                        Menu_lever4_PoPa=5;
                        Menu_lever4_Pub_select_cnt=5;
                        Seg_rev_lenth=49;
                        for(i=0; i<Seg_rev_lenth; i++)
                        {
                            Disp_buff128x64[128*Menu_lever4_PoPa+39+i]=~Disp_buff128x64[128*Menu_lever4_PoPa+39+i];
                            Disp_buff128x64[128*(Menu_lever4_PoPa+1)+39+i]=~Disp_buff128x64[128*(Menu_lever4_PoPa+1)+39+i];
                        }
                    }
                    else
                    {
                        Seg_adds_bak=19;
                        Menu_lever4_PoPa=3;
                        Menu_lever4_Pub_select_cnt=1;
                        Seg_rev_lenth=11;
                        for(i=0; i<Seg_rev_lenth; i++)
                        {
                            Disp_buff128x64[128*Menu_lever4_PoPa+Menu_lever4_PoSeg+i]=~Disp_buff128x64[128*Menu_lever4_PoPa+Menu_lever4_PoSeg+i];
                            Disp_buff128x64[128*(Menu_lever4_PoPa+1)+Menu_lever4_PoSeg+i]=~Disp_buff128x64[128*(Menu_lever4_PoPa+1)+Menu_lever4_PoSeg+i];
                        }
                    }
                    --Seg_rev_lenth;
                }
                //else	//20140216
                //{
                //   data_buff[8]='E';
                //   data_buff[9]='X';
                //   data_buff[10]='T';
                //   data_buff[11]=0;
                //   data_buff[12]='T';
                //   data_buff[13]='R';
                //   data_buff[14]='I';
                //   data_buff[15]='G';
                //   for(j=0;j<16;j++)
                //      Get_6x8_char_single(3,12+j*6,data_buff[j]);
                //
                //   Menu_lever4_PoSeg=11;
                //   if(M_lever4_extern_trrig_flag==1)
                //   {
                //       Seg_adds_bak=59;
                //       Menu_lever4_Pub_select_cnt=5;
                //       Seg_rev_lenth=49;
                //       for(i=0;i<Seg_rev_lenth;i++)
                //       {
                //           Disp_buff128x64[128*Menu_lever4_PoPa+59+i]=~Disp_buff128x64[128*Menu_lever4_PoPa+59+i];
                //           Disp_buff128x64[128*(Menu_lever4_PoPa+1)+59+i]|=0x01;
                //       }
                //   }
                //   else
                //   {
                //       Seg_adds_bak=11;
                //       Menu_lever4_Pub_select_cnt=1;
                //       Seg_rev_lenth=7;
                //       for(i=0;i<Seg_rev_lenth;i++)
                //       {
                //           Disp_buff128x64[128*Menu_lever4_PoPa+Menu_lever4_PoSeg+i]=~Disp_buff128x64[128*Menu_lever4_PoPa+Menu_lever4_PoSeg+i];
                //           Disp_buff128x64[128*(Menu_lever4_PoPa+1)+Menu_lever4_PoSeg+i]|=0x01;
                //       }
                //   }
                //   --Seg_rev_lenth;
                //}
                break;
            case 2:
                j=M_lever4_JZPL_temp>>8;
//                             j=j>>8;
                j&=0x0f;
                Menu_lever4_Pub_adj_cnt=j;
                data_buff[0]=Get_ascii_data(j);
                j=M_lever4_JZPL_temp>>4;
//                             j=j>>4;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);
                j=M_lever4_JZPL_temp;
                j&=0x0f;
                data_buff[2]=Get_ascii_data(j);
/////                             if(M_menur1_language_select_temp==2)
/////                             {
/////                                data_buff[3]=182;
/////                                data_buff[4]=183;
/////                             }
/////                             else
/////                             {
                data_buff[3]=0;
                data_buff[4]='T';
                data_buff[5]='I';
                data_buff[6]='M';
                data_buff[7]='E';
                data_buff[8]='S';
/////                             }

                for(j=0; j<9; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);

                for(i=0; i<7; i++)
                {
                    Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                    Disp_buff128x64[128*4+29+i]|=0x01;
                }
                Seg_adds_bak=Menu_lever4_PoSeg=29;
                break;
            case 3:
                j=M_lever4_QXPL_temp>>8;
//                             j=j>>8;
                j&=0x0f;
                Menu_lever4_Pub_adj_cnt=j;
                data_buff[0]=Get_ascii_data(j);
                j=M_lever4_QXPL_temp>>4;
//                             j=j>>4;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);
                j=M_lever4_QXPL_temp;
                j&=0x0f;
                data_buff[2]=Get_ascii_data(j);
/////                             if(M_menur1_language_select_temp==2)
/////                             {
/////                                data_buff[3]=182;
/////                                data_buff[4]=183;
/////                             }
/////                             else
/////                             {
                data_buff[3]=0;
                data_buff[4]='T';
                data_buff[5]='I';
                data_buff[6]='M';
                data_buff[7]='E';
                data_buff[8]='S';
/////                             }

                for(j=0; j<9; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);

                for(i=0; i<7; i++)
                {
                    Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                    Disp_buff128x64[128*4+29+i]|=0x01;
                }
                Seg_adds_bak=Menu_lever4_PoSeg=29;
                break;
            case 4:
                j=M_lever4_HCPL_temp>>8;
//                             j=j>>8;
                j&=0x0f;
                Menu_lever4_Pub_adj_cnt=j;
                data_buff[0]=Get_ascii_data(j);
                j=M_lever4_HCPL_temp>>4;
//                             j=j>>4;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);
                j=M_lever4_HCPL_temp;
                j&=0x0f;
                data_buff[2]=Get_ascii_data(j);
/////                             if(M_menur1_language_select_temp==2)
/////                             {
/////                                data_buff[3]=182;
/////                                data_buff[4]=183;
/////                             }
/////                             else
/////                             {
                data_buff[3]=0;
                data_buff[4]='T';
                data_buff[5]='I';
                data_buff[6]='M';
                data_buff[7]='E';
                data_buff[8]='S';
/////                             }

                for(j=0; j<9; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);

                for(i=0; i<7; i++)
                {
                    Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                    Disp_buff128x64[128*4+29+i]|=0x01;
                }
                Seg_adds_bak=Menu_lever4_PoSeg=29;
                break;
            case 5:
                if(fix_start_time_falg==1)
                {
                    fix_start_time_falg=0;
                    M_lever4_start_year_temp=RTC_DATA.year_temp;
                    M_lever4_start_hour_temp=RTC_DATA.hour_temp&0x3f;
                    M_lever4_start_mini_temp=RTC_DATA.minu_temp&0x7f;
                    M_lever4_start_munth_temp=RTC_DATA.mouth_temp&0x1f;
                    M_lever4_start_day_temp=RTC_DATA.day_temp&0x3f;
                }

                j=M_lever4_start_year_temp;
                j=j>>4;
                j&=0x0f;
                Menu_lever4_Pub_adj_cnt=j;
                data_buff[0]=Get_ascii_data(j);
                j=M_lever4_start_year_temp;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);
                data_buff[2]=14; 				//-------/2014-1-15 10:18

                j=M_lever4_start_munth_temp;
                j=j>>4;
                j&=0x0f;
                Menu_lever4_Pub_adj_cnt=j;
                data_buff[3]=Get_ascii_data(j);
                j=M_lever4_start_munth_temp;
                j&=0x0f;
                data_buff[4]=Get_ascii_data(j);
                data_buff[5]=14;
                j=M_lever4_start_day_temp;
                j=j>>4;
                j&=0x0f;
                data_buff[6]=Get_ascii_data(j);
                j=M_lever4_start_day_temp;
                j&=0x0f;
                data_buff[7]=Get_ascii_data(j);
                data_buff[8]=0;
                data_buff[9]=0;
                j=M_lever4_start_hour_temp;
                j=j>>4;
                j&=0x0f;
                data_buff[10]=Get_ascii_data(j);
                j=M_lever4_start_hour_temp;
                j&=0x0f;
                data_buff[11]=Get_ascii_data(j);
                data_buff[12]=20;
                j=M_lever4_start_mini_temp;
                j=j>>4;
                j&=0x0f;
                data_buff[13]=Get_ascii_data(j);
                j=M_lever4_start_mini_temp;
                j&=0x0f;
                data_buff[14]=Get_ascii_data(j);

                for(j=0; j<15; j++)
                    Get_6x8_char_single(3,12+j*6,data_buff[j]);

                for(i=0; i<7; i++)
                {
                    Disp_buff128x64[128*3+11+i]=~Disp_buff128x64[128*3+11+i];
                    Disp_buff128x64[128*4+11+i]|=0x01;
                }
                Seg_adds_bak=Menu_lever4_PoSeg=11;
                break;
            }
            break;
        case 2:
            i=Menu_lever3_select-1;
            index= Lever4_m12_char_num+i;
            index_bit= Lever4_m12_bit_num+i;

/////                    M_lever4_FXCS_temp=0x01;  			//2012-8-23 17:13 sams
/////                    Menu_lever4_Pub_adj_cnt=0x01;
            Menu_lever4_Pub_adj_cnt=M_lever4_FXCS_temp;
            j=M_lever4_FXCS_temp;
            j=j>>4;
            j&=0x0f;
            Menu_lever4_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=M_lever4_FXCS_temp;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            data_buff[2]=0;
            data_buff[3]='T';
            data_buff[4]='I';
            data_buff[5]='M';
            data_buff[6]='E';
            data_buff[7]='S';

            for(j=0; j<8; j++)
                Get_6x8_char_single(3,30+j*6,data_buff[j]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Seg_adds_bak=Menu_lever4_PoSeg=29;
            break;
        case 3:
            i=Menu_lever3_select-1;
            index= Lever4_m13_char_num+i;
            index_bit= Lever4_m13_bit_num+i;
            switch(Menu_lever3_select)
            {
            case 1:
                j=M_menur1_jz1nd_temp>>16;
                j&=0x0f;
                Menu_lever4_Pub_adj_cnt=j;
                data_buff[0]=Get_ascii_data(j);
                j=M_menur1_jz1nd_temp>>12;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);
                j=M_menur1_jz1nd_temp>>8;
                j&=0x0f;
                data_buff[2]=Get_ascii_data(j);
                data_buff[3]=19;
                j=M_menur1_jz1nd_temp>>4;
                j&=0x0f;
                data_buff[4]=Get_ascii_data(j);
                j=M_menur1_jz1nd_temp;
                j&=0x0f;
                data_buff[5]=Get_ascii_data(j);

                switch(M_menur1_unit_select_temp)
                {
                case 1:
                    data_buff[6]=0;
                    break;
                case 2:
                    data_buff[6]='M';
                    break;
                case 3:
                    data_buff[6]='U';
                    break;
                }
                data_buff[7]='G';
                data_buff[8]=18;
                data_buff[9]='L';
                if(M_menur1_unit_select_temp>3)
                {
                    data_buff[6]='P';
                    data_buff[7]='P';
                    data_buff[9]=0;
                    if(M_menur1_unit_select_temp==4)
                        data_buff[8]='M';
                    else
                        data_buff[8]='B';
                }
                for(j=0; j<10; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);

                for(i=0; i<7; i++)
                {
                    Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                    Disp_buff128x64[128*4+29+i]|=0x01;
                }
                Seg_adds_bak=Menu_lever4_PoSeg=29;
                break;
            case 2:
                j=M_menur1_jz2nd_temp>>16;
                j&=0x0f;
                Menu_lever4_Pub_adj_cnt=j;
                data_buff[0]=Get_ascii_data(j);
                j=M_menur1_jz2nd_temp>>12;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);
                j=M_menur1_jz2nd_temp>>8;
                j&=0x0f;
                data_buff[2]=Get_ascii_data(j);
                data_buff[4]=19;
                j=M_menur1_jz2nd_temp>>4;
                j&=0x0f;
                data_buff[3]=Get_ascii_data(j);
                j=M_menur1_jz2nd_temp;
                j&=0x0f;
                data_buff[5]=Get_ascii_data(j);

                switch(M_menur1_unit_select_temp)
                {
                case 1:
                    data_buff[6]=0;
                    break;
                case 2:
                    data_buff[6]='M';
                    break;
                case 3:
                    data_buff[6]='U';
                    break;
                }
                data_buff[7]='G';
                data_buff[8]=18;
                data_buff[9]='L';
                if(M_menur1_unit_select_temp>3)
                {
                    data_buff[6]='P';
                    data_buff[7]='P';
                    data_buff[9]=0;
                    if(M_menur1_unit_select_temp==4)
                        data_buff[8]='M';
                    else
                        data_buff[8]='B';
                }

                for(j=0; j<10; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);

                for(i=0; i<7; i++)
                {
                    Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                    Disp_buff128x64[128*4+29+i]|=0x01;
                }
                Seg_adds_bak=Menu_lever4_PoSeg=29;
                break;
            }
            break;
        case 4:
            i=Menu_lever3_select-1;
            index= Lever4_m14_char_num+i;
            index_bit= Lever4_m14_bit_num+i;

            j=M_lever4_BYND_temp>>16;
//                    j=j>>16;
            j&=0x0f;
            Menu_lever4_Pub_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=M_lever4_BYND_temp>>12;
//                    j=j>>12;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            j=M_lever4_BYND_temp>>8;
//                    j=j>>8;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            data_buff[3]=19;
            j=M_lever4_BYND_temp>>4;
//                    j=j>>4;
            j&=0x0f;
            data_buff[4]=Get_ascii_data(j);
            j=M_lever4_BYND_temp;
            j&=0x0f;
            data_buff[5]=Get_ascii_data(j);

            switch(M_menur1_unit_select_temp)
            {
            case 1:
                data_buff[6]=0;
                break;
            case 2:
                data_buff[6]='M';
                break;
            case 3:
                data_buff[6]='U';
                break;
            }
            data_buff[7]='G';
            data_buff[8]=18;
            data_buff[9]='L';
            if(M_menur1_unit_select_temp>3)
            {
                data_buff[6]='P';
                data_buff[7]='P';
                data_buff[9]=0;
                if(M_menur1_unit_select_temp==4)
                    data_buff[8]='M';
                else
                    data_buff[8]='B';
            }

            for(j=0; j<10; j++)
                Get_6x8_char_single(3,30+j*6,data_buff[j]);

            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Seg_adds_bak=Menu_lever4_PoSeg=29;
            break;
        }
        break;
    case 4:
/////    	     if(Prog_disp_flag!=0)		//modefy 2013-7-9 15:51
        if((Prog_disp_flag!=0)&&(Wait_clock_flag==0))
            goto L4_sub_1;

        switch(Menu_index_finc_select)
        {
        case 2:
            i=Menu_lever3_select-1;
            index= Lever4_m42_char_num+i;
            index_bit= Lever4_m42_bit_num+i;
            switch(Menu_lever3_select)
            {
            case 2:
                j=M_menur1_ZSB_speed_temp>>4;
//                             j=j>>4;
                j&=0x0f;
                Menu_lever4_Pub_adj_cnt=j;
                data_buff[0]=Get_ascii_data(j);
                j=M_menur1_ZSB_speed_temp;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);

                for(j=0; j<2; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);

                for(i=0; i<7; i++)
                {
                    Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                    Disp_buff128x64[128*4+29+i]|=0x01;
                }
                Seg_adds_bak=Menu_lever4_PoSeg=29;
                break;
            case 3:
                j=M_lever4_move_to_temp>>8;
//                             j=j>>8;
                j&=0x0f;
                Menu_lever4_Pub_adj_cnt=j;
                data_buff[0]=Get_ascii_data(j);
                data_buff[1]=19;
                j=M_lever4_move_to_temp>>4;
//                             j=j>>4;
                j&=0x0f;
                data_buff[2]=Get_ascii_data(j);
                j=M_lever4_move_to_temp;
                j&=0x0f;
                data_buff[3]=Get_ascii_data(j);
                data_buff[4]='M';
                data_buff[5]='L';

                for(j=0; j<6; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);

                for(i=0; i<7; i++)
                {
                    Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                    Disp_buff128x64[128*4+29+i]|=0x01;
                }
                Seg_adds_bak=Menu_lever4_PoSeg=29;
                break;
            }
            break;
        case 3:
            i=Menu_lever3_select-1;
            index= Lever4_m43_char_num+i;
            index_bit= Lever4_m43_bit_num+i;

            pb_temp=Far_status_flag;
            pb_temp>>=Menu_lever3_select;
            pb_temp&=0x01;
            DCF_switch_select_flag=1;

            if(M_menur1_language_select_temp==2)
            {
                Get_16x16_char_single(3,30,14);
                Get_8x16_char_single(3,13+30,11);
                Get_16x16_char_single(3,24+30,119);

                Menu_lever4_PoPa=3;
                Seg_rev_lenth=13;
                if(pb_temp!=1)
                {
                    Seg_adds_bak=Menu_lever4_PoSeg=53;
                    Menu_lever4_Pub_select_cnt=2;
                }
                else
                {
                    Seg_adds_bak=Menu_lever4_PoSeg=29;
                    Menu_lever4_Pub_select_cnt=1;
                }

                for(i=0; i<Seg_rev_lenth; i++)
                {
                    Disp_buff128x64[128*Menu_lever4_PoPa+Menu_lever4_PoSeg+i]=~Disp_buff128x64[128*Menu_lever4_PoPa+Menu_lever4_PoSeg+i];
                    Disp_buff128x64[128*(Menu_lever4_PoPa+1)+Menu_lever4_PoSeg+i]=~Disp_buff128x64[128*(Menu_lever4_PoPa+1)+Menu_lever4_PoSeg+i];
                }
                --Seg_rev_lenth;
            }
            //else	//20140216
            //{
            //   data_buff[0]=0;
            //   data_buff[1]=0;
            //   data_buff[2]='O';
            //   data_buff[3]='N';
            //   data_buff[4]=18;
            //   data_buff[5]='O';
            //   data_buff[6]='F';
            //   data_buff[7]='F';
            //   data_buff[8]=0;
            //
            //   for(j=0;j<9;j++)
            //      Get_6x8_char_single(3,30+j*6,data_buff[j]);
            //
            //   if(pb_temp!=1)
            //   {
            //      Seg_rev_lenth=19;
            //      Seg_adds_bak=Menu_lever4_PoSeg=59;
            //      Menu_lever4_Pub_select_cnt=2;
            //   }
            //   else
            //   {
            //      Seg_rev_lenth=13;
            //      Seg_adds_bak=Menu_lever4_PoSeg=41;
            //      Menu_lever4_Pub_select_cnt=1;
            //   }
            //   for(i=0;i<Seg_rev_lenth;i++)
            //   {
            //       Disp_buff128x64[128*3+Menu_lever4_PoSeg+i]=~Disp_buff128x64[128*3+Menu_lever4_PoSeg+i];
            //       Disp_buff128x64[128*4+Menu_lever4_PoSeg+i]|=0x01;
            //   }
            //}
            break;
        case 5:
            i=Menu_lever3_select-1;
            index= Lever4_m45_char_num+i;
            index_bit= Lever4_m45_bit_num+i;
            switch(Menu_lever3_select)
            {
            case 1:
                j=M_menur1_JB_speed_temp>>8;
//                             j=j>>8;
                j&=0x0f;
                Menu_lever4_Pub_adj_cnt=j;
                data_buff[0]=Get_ascii_data(j);
                j=M_menur1_JB_speed_temp>>4;
//                             j=j>>4;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);
                j=M_menur1_JB_speed_temp;
                j&=0x0f;
                data_buff[2]=Get_ascii_data(j);

                for(j=0; j<3; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);

                for(i=0; i<7; i++)
                {
                    Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                    Disp_buff128x64[128*4+29+i]|=0x01;
                }
                Seg_adds_bak=Menu_lever4_PoSeg=29;
                break;
            case 2:
                if(M_menur1_language_select_temp==2)
                {
                    data_buff[0]=158;
                    data_buff[1]=160;
                    data_buff[2]=161;
                    data_buff[3]=159;
                    data_buff[4]=160;
                    data_buff[5]=161;

                    for(i=0; i<3; i++)
                    {
                        Get_16x16_char_single(3,12+12*i,data_buff[i]);
                        Get_16x16_char_single(3,48+12+12*i,data_buff[i+3]);
                    }
                    Get_8x16_char_single(3,37+12,11);

                    Menu_lever4_PoPa=3;
                    if(M_menur1_direction_select_temp==2)
                    {
                        Seg_rev_lenth=37;
                        Seg_adds_bak=Menu_lever4_PoSeg=59;
                        Menu_lever4_Pub_select_cnt=2;
                    }
                    else
                    {
                        Seg_rev_lenth=37;
                        Seg_adds_bak=Menu_lever4_PoSeg=11;
                        Menu_lever4_Pub_select_cnt=1;
                    }

                    for(i=0; i<Seg_rev_lenth; i++)
                    {
                        Disp_buff128x64[128*Menu_lever4_PoPa+Menu_lever4_PoSeg+i]=~Disp_buff128x64[128*Menu_lever4_PoPa+Menu_lever4_PoSeg+i];
                        Disp_buff128x64[128*(Menu_lever4_PoPa+1)+Menu_lever4_PoSeg+i]=~Disp_buff128x64[128*(Menu_lever4_PoPa+1)+Menu_lever4_PoSeg+i];
                    }
                    --Seg_rev_lenth;
                }
                //else	//20140216
                //{
                //   data_buff[0]=13;
                //   data_buff[1]=18;
                //   data_buff[2]=14;
                //
                //   for(i=0;i<3;i++)
                //      Get_6x8_char_single(3,i*6+30,data_buff[i]);
                //
                //   Menu_lever4_PoPa=3;
                //   Seg_rev_lenth=7;
                //   if(M_menur1_direction_select_temp==1)		//顺时针
                //   {
                //       Seg_adds_bak=Menu_lever4_PoSeg=29;
                //       Menu_lever4_Pub_select_cnt=1;
                //   }
                //   else
                //   {;
                //       Seg_adds_bak=Menu_lever4_PoSeg=41;		//逆时针
                //       Menu_lever4_Pub_select_cnt=2;
                //   }
                //
                //   for(i=0;i<Seg_rev_lenth;i++)
                //   {
                //       Disp_buff128x64[128*Menu_lever4_PoPa+Menu_lever4_PoSeg+i]=~Disp_buff128x64[128*Menu_lever4_PoPa+Menu_lever4_PoSeg+i];
                //       Disp_buff128x64[128*(Menu_lever4_PoPa+1)+Menu_lever4_PoSeg+i]|=0x01;
                //   }
                //   --Seg_rev_lenth;
                //}
                break;
            }
            break;
        case 6:
            i=Menu_lever3_select-1;
            index= Lever4_m46_char_num+i;
            index_bit= Lever4_m46_bit_num+i;
            switch(Menu_lever3_select)
            {
            case 1:
                if(M_menur1_LZDJ_select_temp==2)
                {
                    data_buff[0]='0';
                    data_buff[1]='0';
                    data_buff[2]='0';
                    data_buff[3]='0';
                    data_buff[4]=19;
                    data_buff[5]='0';
                    data_buff[6]='0';
                }
                else	//read ise data
                {
                    Rx_TX_flag_local=1;
                    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_ADC_ISE_ADDS,0x0001,0x00);
                    for(i=0; i<250; i++)
                    {
                        DrvSYS_Delay(6000);
                        if(Rx_TX_flag_local==0)
                            i=250;
                    }
                    Rx_TX_flag_local=2;
                    Uart_tx_rx_fifo_int_FLAG&=~BIT3;
/////    	                        DrvSYS_Delay(2000);				//delay 2ms
                    Com1_rd_ctrl(0);
                    DrvGPIO_SetBit_new(E_GPB,6);
///	                        for(i=0;i<255;i++)				//clear tx buff data
///	                        {
///	                           U_data_buff[i]=0;
///	                        }
                    U_Rx_wait_time_flag_temp=0x8000;		//set wait 10sec fot time out
                    for(i=0; i<250; i++)
                    {
                        DrvSYS_Delay(6000);
                        if(Rx_TX_flag_local==0)
                            i=250;
                    }

                    Uart_tx_rx_fifo_int_FLAG&=~BIT2;

                    M_menur3_ISE_temp=0x0000;
                    M_menur3_ISE_temp=U_data_buff[3];
                    M_menur3_ISE_temp=M_menur3_ISE_temp<<8;
                    M_menur3_ISE_temp+=U_data_buff[4];

                    pb_temp=M_menur3_ISE_temp;
                    if(M_menur3_ISE_temp<31250)
                    {
                        data_buff[0]=14;		//-
                        pb_temp=31250-pb_temp;
                    }
                    else
                    {
                        data_buff[0]=0;		//+
                        pb_temp=pb_temp-31250;
                    }

                    calcula_tp=kkk=0;
                    j16=pb_temp/10000;
                    j16&=0x000f;
                    kkk=j16<<16;
                    calcula_tp+=kkk;
                    j16=pb_temp/1000;
                    j16=j16%10;
                    j16&=0x000f;
                    kkk=j16<<12;
                    calcula_tp+=kkk;
                    j16=pb_temp/100;
                    j16=j16%10;
                    j16&=0x000f;
                    kkk=j16<<8;
                    calcula_tp+=kkk;
                    j16=pb_temp/10;
                    j16=j16%10;
                    j16&=0x000f;
                    kkk=j16<<4;

                    calcula_tp+=kkk;
                    j=pb_temp%10;
                    j&=0x0f;
                    calcula_tp+=j;
//                                M_menur3_ISE_temp=calcula_tp;

                    j=calcula_tp>>16;
                    j&=0x0f;
                    data_buff[1]=Get_ascii_data(j);
                    j=calcula_tp>>12;
                    j&=0x0f;
                    data_buff[2]=Get_ascii_data(j);
                    j=calcula_tp>>8;
                    j&=0x0f;
                    data_buff[3]=Get_ascii_data(j);
                    data_buff[4]=19;
                    j=calcula_tp>>4;
                    j&=0x0f;
                    data_buff[5]=Get_ascii_data(j);
                    j=calcula_tp;
                    j&=0x0f;
                    data_buff[6]=Get_ascii_data(j);
                }
                data_buff[7]=0;
                data_buff[8]='M';
                data_buff[9]='V';

                for(j=0; j<10; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);

                Disp_gdgdy_flag=3;		//2013-6-26 9:10
                Pro_wait_time_flag=2;
                break;
            case 2:
                data_buff[0]=14;
                data_buff[1]='1';
                data_buff[2]='6';
                data_buff[3]='0';
                data_buff[4]='M';
                data_buff[5]='V';
                data_buff[6]=0;
                data_buff[7]='0';
                data_buff[8]=0;
                data_buff[9]=13;
                data_buff[10]='1';
                data_buff[11]='6';
                data_buff[12]='0';
                data_buff[13]='M';
                data_buff[14]='V';

                for(j=0; j<15; j++)
                    Get_6x8_char_single(3,12+j*6,data_buff[j]);

                Seg_rev_lenth=37;
                Seg_adds_bak=Menu_lever4_PoSeg=11;
                for(i=0; i<Seg_rev_lenth; i++)
                {
                    Disp_buff128x64[128*3+11+i]=~Disp_buff128x64[128*3+11+i];
                    Disp_buff128x64[128*4+11+i]|=0x01;
                }
                --Seg_rev_lenth;
                break;
            }
            break;
        case 7:			//读温度
            i=Menu_lever3_select-1;
            index= Lever4_m47_char_num+i;
            index_bit= Lever4_m47_bit_num+i;
            switch(Menu_lever3_select)
            {
            case 1:
                Rx_TX_flag_local=1;
                if(M_menur1_LZDJ_select_temp==1)	//电极法
                    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_ADC_TEMP1_ADDS,0x0001,0x00);    	//2012-10-24 16:38
                else
                    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_ADC_TEMP_ADDS,0x0001,0x00);
                for(i=0; i<250; i++)
                {
                    DrvSYS_Delay(6000);
                    if(Rx_TX_flag_local==0)
                        i=250;
                }
                Rx_TX_flag_local=2;
                Uart_tx_rx_fifo_int_FLAG&=~BIT3;
/////    	                     DrvSYS_Delay(2000);				//delay 2ms
                Com1_rd_ctrl(0);
                DrvGPIO_SetBit_new(E_GPB,6);
///	                     for(i=0;i<255;i++)				//clear tx buff data
///	                     {
///	                        U_data_buff[i]=0;
///	                     }
                U_Rx_wait_time_flag_temp=0x8000;		//set wait 10sec fot time out
                for(i=0; i<250; i++)
                {
                    DrvSYS_Delay(6000);
                    if(Rx_TX_flag_local==0)
                        i=250;
                }

                Uart_tx_rx_fifo_int_FLAG&=~BIT2;

                M_lever4_WD_temp=0x00000000;
                M_lever4_WD_temp=U_data_buff[3];
                M_lever4_WD_temp=M_lever4_WD_temp<<8;
                M_lever4_WD_temp+=U_data_buff[4];

                if(M_lever4_WD_temp>=1024)
                {
                    data_buff[0]=0;		//+
                    M_lever4_WD_temp-=1024;
                }
                else
                {
                    data_buff[0]=14;		//-
                    M_lever4_WD_temp=1024-M_lever4_WD_temp;
                }

                calcula_tp=0x00000000;
                j=M_lever4_WD_temp/1000;
                kkk=j<<12;
                calcula_tp+=kkk;
                j=M_lever4_WD_temp/100;
                j=j%10;
                kkk=j<<8;
                calcula_tp+=kkk;
                j=M_lever4_WD_temp/10;
                j=j%10;
                kkk=j<<4;
                calcula_tp+=kkk;
                j=M_lever4_WD_temp%10;
                calcula_tp+=j;
//                             M_lever4_WD_temp=calcula_tp;

                j=calcula_tp>>8;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);
                j=calcula_tp>>4;
                j&=0x0f;
                data_buff[2]=Get_ascii_data(j);
                data_buff[3]=19;
                j=calcula_tp;
                j&=0x0f;
                data_buff[4]=Get_ascii_data(j);
                data_buff[5]=0;
                data_buff[6]='C';

                Disp_gdgdy_flag=2;		//2012-9-21 16:04
                Pro_wait_time_flag=2;
//                             Back_light_time_flag=0;
//                             Back_light_time_cnt=0;
                Set_back_light_10sec();

                for(j=0; j<7; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);

                Disp_buff128x64[3*128+63]=0x04;
                Disp_buff128x64[3*128+64]=0x0a;
                Disp_buff128x64[3*128+65]=0x04;
                break;
            case 2:
                if(BCWD_NG_flag==2)
                    data_buff[0]=14;
                else
                    data_buff[0]=13;

                j=M_lever4_BCWD_temp>>4;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);
                data_buff[2]=19;
                j=M_lever4_BCWD_temp;
                j&=0x0f;
                data_buff[3]=Get_ascii_data(j);

                for(j=0; j<4; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);

                for(i=0; i<7; i++)
                {
                    Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                    Disp_buff128x64[128*4+29+i]|=0x01;
                }
                Seg_adds_bak=Menu_lever4_PoSeg=29;
                Seg_rev_lenth=6;
                break;
            }
            break;
        case 8:		//读光电管 等 电位
            i=Menu_lever3_select-1;
            index= Lever4_m48_char_num+i;
            index_bit= Lever4_m48_bit_num+i;
            switch(Menu_lever3_select)
            {
            case 1:
                j=M_menur1_led_dianliu_temp>>16;
                j&=0x0f;
                data_buff[0]=Get_ascii_data(j);
                j=M_menur1_led_dianliu_temp>>12;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);
                data_buff[2]=19;
                j=M_menur1_led_dianliu_temp>>8;
                j&=0x0f;
                data_buff[3]=Get_ascii_data(j);
                j=M_menur1_led_dianliu_temp>>4;
                j&=0x0f;
                data_buff[4]=Get_ascii_data(j);
                j=M_menur1_led_dianliu_temp;
                j&=0x0f;
                data_buff[5]=Get_ascii_data(j);
                data_buff[6]='M';
                data_buff[7]='A';

                for(j=0; j<8; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);

                for(i=0; i<7; i++)
                {
                    Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                    Disp_buff128x64[128*4+29+i]|=0x01;
                }
                Seg_adds_bak=Menu_lever4_PoSeg=29;
                Seg_rev_lenth=7;
                break;
            case 2:
                if(M_menur1_LZDJ_select_temp==2)	//读光电管电压
                {
                    Rx_TX_flag_local=1;
                    Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_ADC_PHOTO_ADDS,0x0001,0x00);
                    for(i=0; i<250; i++)
                    {
                        DrvSYS_Delay(6000);
                        if(Rx_TX_flag_local==0)
                            i=250;
                    }
                    Rx_TX_flag_local=2;
                    Uart_tx_rx_fifo_int_FLAG&=~BIT3;
/////    	                        DrvSYS_Delay(2000);				//delay 2ms
                    Com1_rd_ctrl(0);
                    DrvGPIO_SetBit_new(E_GPB,6);
///	                        for(i=0;i<255;i++)				//clear tx buff data
///	                        {
///	                           U_data_buff[i]=0;
///	                        }
                    U_Rx_wait_time_flag_temp=0x8000;		//set wait 10sec fot time out
                    for(i=0; i<250; i++)
                    {
                        DrvSYS_Delay(6000);
                        if(Rx_TX_flag_local==0)
                            i=250;
                    }

                    Uart_tx_rx_fifo_int_FLAG&=~BIT2;

                    M_lever4_GDGDY_temp=0x00000000;
                    M_lever4_GDGDY_temp=U_data_buff[3];
                    M_lever4_GDGDY_temp=M_lever4_GDGDY_temp<<8;
                    M_lever4_GDGDY_temp+=U_data_buff[4];

                    calcula_tp=0x00000000;
                    j=M_lever4_GDGDY_temp/10000;
                    kkk=j<<16;
                    calcula_tp+=kkk;
                    j=M_lever4_GDGDY_temp/1000;
                    j=j%10;
                    kkk=j<<12;
                    calcula_tp+=kkk;
                    j=M_lever4_GDGDY_temp/100;
                    j=j%10;
                    kkk=j<<8;
                    calcula_tp+=kkk;
                    j=M_lever4_GDGDY_temp/10;
                    j=j%10;
                    kkk=j<<4;
                    calcula_tp+=kkk;
                    j=M_lever4_GDGDY_temp%10;
                    calcula_tp+=j;
//                                M_lever4_GDGDY_temp=calcula_tp;

                    j=calcula_tp>>16;
                    j&=0x0f;
                    data_buff[0]=Get_ascii_data(j);
                    j=calcula_tp>>12;
                    j&=0x0f;
                    data_buff[1]=Get_ascii_data(j);
                    j=calcula_tp>>8;
                    j&=0x0f;
                    data_buff[2]=Get_ascii_data(j);
                    j=calcula_tp>>4;
                    j&=0x0f;
                    data_buff[3]=Get_ascii_data(j);
                    data_buff[4]=19;
                    j=calcula_tp;
                    j&=0x0f;
                    data_buff[5]=Get_ascii_data(j);
                    data_buff[6]='0';

                    Disp_gdgdy_flag=1;		//2012-9-6 16:46
                    Pro_wait_time_flag=2;
//                                Back_light_time_flag=0;
//                                Back_light_time_cnt=0;
                    Set_back_light_10sec();
                }
                else
                {
                    data_buff[0]='0';
                    data_buff[1]='0';
                    data_buff[2]='0';
                    data_buff[3]='0';
                    data_buff[4]=19;
                    data_buff[5]='0';
                    data_buff[6]='0';
                }
                data_buff[7]='M';
                data_buff[8]='V';

                for(j=0; j<9; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);
                break;
            case 3:		//设置偏置电压
                j=M_lever4_PZDY_temp>>8;
                j&=0x0f;
                data_buff[0]=Get_ascii_data(j);
                data_buff[1]=19;
                j=M_lever4_PZDY_temp>>4;
                j&=0x0f;
                data_buff[2]=Get_ascii_data(j);
                j=M_lever4_PZDY_temp;
                j&=0x0f;
                data_buff[3]=Get_ascii_data(j);
                data_buff[4]='V';

                for(j=0; j<5; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);

                for(i=0; i<7; i++)
                {
                    Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                    Disp_buff128x64[128*4+29+i]|=0x01;
                }
                Seg_adds_bak=Menu_lever4_PoSeg=29;
                Seg_rev_lenth=7;
                break;
            case 4:		//读偏置电压值
                Rx_TX_flag_local=1;
                Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_ADC_OFFSET_ADDS,0x0001,0x00);
                for(i=0; i<250; i++)
                {
                    DrvSYS_Delay(6000);
                    if(Rx_TX_flag_local==0)
                        i=250;
                }
                Rx_TX_flag_local=2;
                Uart_tx_rx_fifo_int_FLAG&=~BIT3;
/////    	                     DrvSYS_Delay(2000);				//delay 2ms
                Com1_rd_ctrl(0);
                DrvGPIO_SetBit_new(E_GPB,6);
///	                     for(i=0;i<255;i++)				//clear tx buff data
///	                     {
///	                        U_data_buff[i]=0;
///	                     }
                U_Rx_wait_time_flag_temp=0x8000;		//set wait 10sec fot time out
                for(i=0; i<250; i++)
                {
                    DrvSYS_Delay(6000);
                    if(Rx_TX_flag_local==0)
                        i=250;
                }

                Uart_tx_rx_fifo_int_FLAG&=~BIT2;

                M_lever4_PZDYZ_temp=0x00000000;
                M_lever4_PZDYZ_temp=U_data_buff[3];
                M_lever4_PZDYZ_temp=M_lever4_PZDYZ_temp<<8;
                M_lever4_PZDYZ_temp+=U_data_buff[4];

                calcula_tp=0x00000000;
                j=M_lever4_PZDYZ_temp/10000;
                kkk=j<<16;
                calcula_tp+=kkk;
                j=M_lever4_PZDYZ_temp/1000;
                j=j%10;
                kkk=j<<12;
                calcula_tp+=kkk;
                j=M_lever4_PZDYZ_temp/100;
                j=j%10;
                kkk=j<<8;
                calcula_tp+=kkk;
                j=M_lever4_PZDYZ_temp/10;
                j=j%10;
                kkk=j<<4;
                calcula_tp+=kkk;
                j=M_lever4_PZDYZ_temp%10;
                calcula_tp+=j;
//                             M_lever4_PZDYZ_temp=calcula_tp;

                j=calcula_tp>>16;
                j&=0x0f;
                data_buff[0]=Get_ascii_data(j);
                j=calcula_tp>>12;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);
                j=calcula_tp>>8;
                j&=0x0f;
                data_buff[2]=Get_ascii_data(j);
                j=calcula_tp>>4;
                j&=0x0f;
                data_buff[3]=Get_ascii_data(j);
                data_buff[4]=19;
                j=calcula_tp;
                j&=0x0f;
                data_buff[5]=Get_ascii_data(j);
                data_buff[6]='0';
                data_buff[7]='M';
                data_buff[8]='V';

                for(j=0; j<9; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);
                break;
            case 5:			//读差分值
                Rx_TX_flag_local=1;
                Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_ADC_OFFSET_PHOTO_ADDS,0x0001,0x00);
                for(i=0; i<250; i++)
                {
                    DrvSYS_Delay(6000);
                    if(Rx_TX_flag_local==0)
                        i=250;
                }
                Rx_TX_flag_local=2;
                Uart_tx_rx_fifo_int_FLAG&=~BIT3;
/////    	                     DrvSYS_Delay(2000);				//delay 2ms
                Com1_rd_ctrl(0);
                DrvGPIO_SetBit_new(E_GPB,6);
///	                     for(i=0;i<255;i++)				//clear tx buff data
///	                     {
///	                        U_data_buff[i]=0;
///	                     }
                U_Rx_wait_time_flag_temp=0x8000;		//set wait 10sec fot time out
                for(i=0; i<250; i++)
                {
                    DrvSYS_Delay(6000);
                    if(Rx_TX_flag_local==0)
                        i=250;
                }

                Uart_tx_rx_fifo_int_FLAG&=~BIT2;

                M_lever4_CFZ_temp=0x00000000;
                M_lever4_CFZ_temp=U_data_buff[3];
                M_lever4_CFZ_temp=M_lever4_CFZ_temp<<8;
                M_lever4_CFZ_temp+=U_data_buff[4];

                pb_temp=M_lever4_CFZ_temp;
                if(M_lever4_CFZ_temp<2500)
                {
                    data_buff[0]=4;		//-
                    pb_temp=2500-pb_temp;
                }
                else
                {
                    data_buff[0]=0;		//+
                    pb_temp=pb_temp-2500;
                }

                calcula_tp=0x00000000;
                j16=pb_temp/10000;
                j16&=0x000f;
                kkk=j16<<16;
                calcula_tp+=kkk;
                j16=pb_temp/1000;
                j16=j16%10;
                j16&=0x000f;
                kkk=j16<<12;
                calcula_tp+=kkk;
                j16=pb_temp/100;
                j16=j16%10;
                j16&=0x000f;
                kkk=j16<<8;
                calcula_tp+=kkk;
                j16=pb_temp/10;
                j16=j16%10;
                j16&=0x000f;
                kkk=j16<<4;
                calcula_tp+=kkk;

                j=pb_temp%10;
                calcula_tp+=j;
//                             M_lever4_PZDYZ_temp=calcula_tp;

                j=calcula_tp>>20;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);
                j=calcula_tp>>16;
                j&=0x0f;
                data_buff[2]=Get_ascii_data(j);
                j=calcula_tp>>12;
                j&=0x0f;
                data_buff[3]=Get_ascii_data(j);
                j=calcula_tp>>8;
                j&=0x0f;
                data_buff[4]=Get_ascii_data(j);
                data_buff[5]=19;
                j=calcula_tp>>4;
                j&=0x0f;
                data_buff[6]=Get_ascii_data(j);
                j=calcula_tp;
                j&=0x0f;
                data_buff[7]='0';
                data_buff[8]='M';
                data_buff[9]='V';

                for(j=0; j<10; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);
                break;
            }
            break;
        }
        break;
/////    	case 5:

/////    	break;
    case 6:
/////    	     if(Prog_disp_flag!=0)		//modefy 2013-7-9 15:51
        if((Prog_disp_flag!=0)&&(Wait_clock_flag==0))
            goto L4_sub_ex;

        switch(Menu_index_finc_select)
        {
        case 1:
            i=Menu_lever3_select-1;
            index= Lever4_m61_char_num+i;
            index_bit= Lever4_m61_bit_num+i;
            switch(Menu_lever3_select)
            {
            case 1:
                data_buff[0]='N';
                data_buff[1]=20;

                j=Machine_note_temp>>8;
                j&=0x0f;
                Menu_lever4_Pub_adj_cnt=j;
                data_buff[2]=Get_ascii_data(j);
                j=Machine_note_temp>>4;
                j&=0x0f;
                data_buff[3]=Get_ascii_data(j);
                j=Machine_note_temp;
                j&=0x0f;
                data_buff[4]=Get_ascii_data(j);

                for(j=0; j<5; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);
                for(i=0; i<7; i++)
                {
                    Disp_buff128x64[128*3+41+i]=~Disp_buff128x64[128*3+41+i];
                    Disp_buff128x64[128*4+41+i]|=0x01;
                }
                Seg_adds_bak=Menu_lever4_PoSeg=41;
                break;
            case 2:
                Menu_lever4_Pub_select_cnt=Uart2_BAUD_select_temp;
                data_buff[0]='9';
                data_buff[1]='6';
                data_buff[2]='0';
                data_buff[3]='0';
                data_buff[4]=18;
                data_buff[5]='1';
                data_buff[6]='9';
                data_buff[7]='2';
                data_buff[8]='0';
                data_buff[9]='0';
                data_buff[10]=18;
                data_buff[11]='3';
                data_buff[12]='8';
                data_buff[13]='4';
                data_buff[14]='0';
                data_buff[15]='0';
                for(j=0; j<16; j++)
                    Get_6x8_char_single(3,12+j*6,data_buff[j]);

                Menu_lever4_PoSeg=11;
                if(Uart2_BAUD_select_temp<=1)			//
                {
                    Seg_rev_lenth=25;
                    Seg_adds_bak=11;
                    Menu_lever4_Pub_select_cnt=Uart2_BAUD_select_temp=1;
                }
                else if(Uart2_BAUD_select_temp==2)
                {
                    Seg_rev_lenth=31;
                    Seg_adds_bak=41;
                }
                else
                {
                    Seg_rev_lenth=31;
                    Seg_adds_bak=77;		//
                    Menu_lever4_Pub_select_cnt=Uart2_BAUD_select_temp=3;
                }

                for(i=0; i<Seg_rev_lenth; i++)
                {
                    Disp_buff128x64[128*Menu_lever4_PoPa+Seg_adds_bak+i]=~Disp_buff128x64[128*Menu_lever4_PoPa+Seg_adds_bak+i];
                    Disp_buff128x64[128*(Menu_lever4_PoPa+1)+Seg_adds_bak+i]|=0x01;
                }
                --Seg_rev_lenth;
                break;
            case 3:
                Menu_lever4_Pub_select_cnt=Uart2_JOJY_select_temp;
                data_buff[0]='1';
                data_buff[1]=18;
                data_buff[2]='2';
                data_buff[3]=18;
                data_buff[4]='N';
                for(j=0; j<5; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);

                Seg_rev_lenth=7;
                Menu_lever4_PoSeg=29;
                if(Uart2_JOJY_select_temp<=1)			//
                {
                    Seg_adds_bak=29;
                    Menu_lever4_Pub_select_cnt=1;
                }
                else if(Uart2_JOJY_select_temp==2)
                {
                    Seg_adds_bak=41;
                    Menu_lever4_Pub_select_cnt=2;
                }
                else
                {
                    Seg_adds_bak=53;				//
                    Menu_lever4_Pub_select_cnt=3;
                }
                for(i=0; i<Seg_rev_lenth; i++)
                {
                    Disp_buff128x64[128*Menu_lever4_PoPa+Seg_adds_bak+i]=~Disp_buff128x64[128*Menu_lever4_PoPa+Seg_adds_bak+i];
                    Disp_buff128x64[128*(Menu_lever4_PoPa+1)+Seg_adds_bak+i]|=0x01;
                }
                --Seg_rev_lenth;
                break;
            case 4:
                Menu_lever4_Pub_select_cnt=Uart2_STOP_select_temp;
                data_buff[0]='1';
                data_buff[1]=18;
                data_buff[2]='2';
                for(j=0; j<3; j++)
                    Get_6x8_char_single(3,30+j*6,data_buff[j]);

                Seg_rev_lenth=7;
                if(Uart2_STOP_select_temp<=1)			//
                {
                    Seg_adds_bak=Menu_lever4_PoSeg=29;
                    Menu_lever4_Pub_select_cnt=1;
                }
                else
                {
                    Seg_adds_bak=Menu_lever4_PoSeg=41;		//
                    Menu_lever4_Pub_select_cnt=2;
                }
                for(i=0; i<Seg_rev_lenth; i++)
                {
                    Disp_buff128x64[128*Menu_lever4_PoPa+Seg_adds_bak+i]=~Disp_buff128x64[128*Menu_lever4_PoPa+Seg_adds_bak+i];
                    Disp_buff128x64[128*(Menu_lever4_PoPa+1)+Seg_adds_bak+i]|=0x01;
                }
                --Seg_rev_lenth;
                break;
            }
            break;
        case 2:					//2012-9-18 10:46
            Pwm_menu_lever=1;
            Pwm_menu_select=1;
            Disp_pwm_menu();
////    	     	goto L4_sub_ex;
            break;
        case 4:					//2012-8-28 10:31
            i=Menu_lever3_select-1;
            index= Lever4_m64_char_num+i;
            index_bit= Lever4_m64_bit_num+i;
            if(M_menur1_language_select_temp==2)
            {
                Get_16x16_char_single(3,30,14);
                Get_8x16_char_single(3,13+30,11);
                Get_16x16_char_single(3,24+30,119);

                Menu_lever4_PoPa=3;
////            	       if(M_menur1_ZDXS_select_temp==2)
////            	       {
                Seg_rev_lenth=13;
                Seg_adds_bak=Menu_lever4_PoSeg=53;
                Menu_lever4_Pub_select_cnt=2;
////            	       }
////            	       else
////            	       {
////                          Seg_rev_lenth=13;
////                          Seg_adds_bak=Menu_lever4_PoSeg=29;
////                          Menu_lever4_Pub_select_cnt=1;
////            	       }
////
                for(i=0; i<Seg_rev_lenth; i++)
                {
                    Disp_buff128x64[128*Menu_lever4_PoPa+Menu_lever4_PoSeg+i]=~Disp_buff128x64[128*Menu_lever4_PoPa+Menu_lever4_PoSeg+i];
                    Disp_buff128x64[128*(Menu_lever4_PoPa+1)+Menu_lever4_PoSeg+i]=~Disp_buff128x64[128*(Menu_lever4_PoPa+1)+Menu_lever4_PoSeg+i];
                }
                --Seg_rev_lenth;
            }
            //else	//20140216
            //{
            //   data_buff[0]=0;
            //   data_buff[1]=0;
            //   data_buff[2]='O';
            //   data_buff[3]='N';
            //   data_buff[4]=18;
            //   data_buff[5]='O';
            //   data_buff[6]='F';
            //   data_buff[7]='F';
            //   data_buff[8]=0;
            //   for(j=0;j<9;j++)
            //      Get_6x8_char_single(3,30+j*6,data_buff[j]);
            //   for(i=0;i<13;i++)
            //   {
            //       Disp_buff128x64[128*3+41+i]=~Disp_buff128x64[128*3+41+i];
            //       Disp_buff128x64[128*4+41+i]|=0x01;
            //   }
            //   Seg_adds_bak=Menu_lever4_PoSeg=41;
            //   Menu_lever4_Pub_select_cnt=1;
            //}
            break;
        }
        break;
    case 2:
        i=Menu_lever3_Pub_select_cnt-1;
        index= Lever4_m220_char_num+i;
        index_bit= Lever4_m220_bit_num+i;
        data_buff[0]='Y';
        data_buff[1]=17;
        data_buff[2]='A';
        data_buff[3]='X';
        data_buff[4]=12;
        data_buff[5]=13;
        data_buff[6]='B';
        data_buff[7]='X';
        data_buff[8]=11;
        data_buff[9]=13;
        data_buff[10]='C';
        data_buff[11]='X';
        data_buff[12]=13;
        data_buff[13]='D';
        data_buff[14]=0;
        data_buff[15]=0;
        data_buff[16]=0;
        data_buff[17]='A';
        data_buff[17]+=Menu_lever3_Pub_select_cnt;
        --data_buff[17];
        for(i=0; i<18; i++)
            Get_6x8_char_single(0,i*6+6,data_buff[i]);

        if(Menu_index_finc_select==12)
        {
            Menu_lever4_Pub_select_cnt=1;
            switch(Menu_lever3_Pub_select_cnt)
            {
            case 1:
                if(a_signle_flag==1)
                    data_buff[0]=13;
                else
                    data_buff[0]=14;
                Menu_lever4_Pub_adj_cnt=a_signle_flag;

                j=M_menur1_Fa_temp>>24;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);
                j=M_menur1_Fa_temp>>20;
                j&=0x0f;
                data_buff[2]=Get_ascii_data(j);
                j=M_menur1_Fa_temp>>16;
                j&=0x0f;
                data_buff[3]=Get_ascii_data(j);
                data_buff[4]=19;
                j=M_menur1_Fa_temp>>12;
                j&=0x0f;
                data_buff[5]=Get_ascii_data(j);
                j=M_menur1_Fa_temp>>8;
                j&=0x0f;
                data_buff[6]=Get_ascii_data(j);
                j=M_menur1_Fa_temp>>4;
                j&=0x0f;
                data_buff[7]=Get_ascii_data(j);
                j=M_menur1_Fa_temp;
                j&=0x0f;
                data_buff[8]=Get_ascii_data(j);
                break;
            case 2:
                if(b_signle_flag==1)
                    data_buff[0]=13;
                else
                    data_buff[0]=14;
                Menu_lever4_Pub_adj_cnt=b_signle_flag;

                j=M_menur1_Fb_temp>>24;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);
                j=M_menur1_Fb_temp>>20;
                j&=0x0f;
                data_buff[2]=Get_ascii_data(j);
                j=M_menur1_Fb_temp>>16;
                j&=0x0f;
                data_buff[3]=Get_ascii_data(j);
                data_buff[4]=19;
                j=M_menur1_Fb_temp>>12;
                j&=0x0f;
                data_buff[5]=Get_ascii_data(j);
                j=M_menur1_Fb_temp>>8;
                j&=0x0f;
                data_buff[6]=Get_ascii_data(j);
                j=M_menur1_Fb_temp>>4;
                j&=0x0f;
                data_buff[7]=Get_ascii_data(j);
                j=M_menur1_Fb_temp;
                j&=0x0f;
                data_buff[8]=Get_ascii_data(j);
                break;
            case 3:
                if(c_signle_flag==1)
                    data_buff[0]=13;
                else
                    data_buff[0]=14;
                Menu_lever4_Pub_adj_cnt=c_signle_flag;

                j=M_menur1_Fc_temp>>24;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);
                j=M_menur1_Fc_temp>>20;
                j&=0x0f;
                data_buff[2]=Get_ascii_data(j);
                j=M_menur1_Fc_temp>>16;
                j&=0x0f;
                data_buff[3]=Get_ascii_data(j);
                data_buff[4]=19;
                j=M_menur1_Fc_temp>>12;
                j&=0x0f;
                data_buff[5]=Get_ascii_data(j);
                j=M_menur1_Fc_temp>>8;
                j&=0x0f;
                data_buff[6]=Get_ascii_data(j);
                j=M_menur1_Fc_temp>>4;
                j&=0x0f;
                data_buff[7]=Get_ascii_data(j);
                j=M_menur1_Fc_temp;
                j&=0x0f;
                data_buff[8]=Get_ascii_data(j);
                break;
            case 4:
                if(d_signle_flag==1)
                    data_buff[0]=13;
                else
                    data_buff[0]=14;
                Menu_lever4_Pub_adj_cnt=d_signle_flag;

                j=M_menur1_Fd_temp>>24;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);
                j=M_menur1_Fd_temp>>20;
                j&=0x0f;
                data_buff[2]=Get_ascii_data(j);
                j=M_menur1_Fd_temp>>16;
                j&=0x0f;
                data_buff[3]=Get_ascii_data(j);
                data_buff[4]=19;
                j=M_menur1_Fd_temp>>12;
                j&=0x0f;
                data_buff[5]=Get_ascii_data(j);
                j=M_menur1_Fd_temp>>8;
                j&=0x0f;
                data_buff[6]=Get_ascii_data(j);
                j=M_menur1_Fd_temp>>4;
                j&=0x0f;
                data_buff[7]=Get_ascii_data(j);
                j=M_menur1_Fd_temp;
                j&=0x0f;
                data_buff[8]=Get_ascii_data(j);
                break;
            }
            for(j=0; j<9; j++)
                Get_6x8_char_single(3,30+j*6,data_buff[j]);
            for(i=0; i<7; i++)
            {
                Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
                Disp_buff128x64[128*4+29+i]|=0x01;
            }
            Seg_adds_bak=Menu_lever4_PoSeg=29;
        }
        break;
    }
//------------------------
    if((Menu_index_select==6)&&(Menu_index_finc_select==2))
        __nop();
    else if((Menu_index_select!=3)&&(Menu_index_select!=5))
    {
        if((Menu_index_select==2)&&(Menu_index_finc_select==12)) goto L4_sub_1;
        if(M_menur1_language_select_temp==1) 	//20140216
        {
            //switch(Menu_index_select)
            //{
            //    case 1:
            //        switch(Menu_index_finc_select)
            //        {
            //           case 1:
            //          	    index= Lever311_en_menu_index+Menu_lever3_select-1;	//char num.
            //           break;
            //           case 2:
            //          	    index= Lever312_en_menu_index+Menu_lever3_select-1;	//char num.
            //           break;
            //           case 3:
            //          	    index= Lever313_en_menu_index+Menu_lever3_select-1;	//char num.
            //           break;
            //           case 4:
            //          	    index= Lever314_en_menu_index+Menu_lever3_select-1;	//char num.
            //           break;
            //       }
            //    break;
            //    case 4:
            //        switch(Menu_index_finc_select)
            //        {
            //           case 1:
            //          	    index= Lever341_en_menu_index+Menu_lever3_select-1;	//char num.
            //           break;
            //           case 2:
            //          	    index= Lever342_en_menu_index+Menu_lever3_select-1;	//char num.
            //           break;
            //           case 3:
            //          	    index= Lever343_en_menu_index+Menu_lever3_select-1;	//char num.
            //            break;
            //           case 4:
            //          	    index= Lever344_en_menu_index+Menu_lever3_select-1;	//char num.
            //           break;
            //           case 5:
            //          	    index= Lever345_en_menu_index+Menu_lever3_select-1;	//char num.
            //           break;
            //           case 6:
            //          	    index= Lever346_en_menu_index+Menu_lever3_select-1;	//char num.
            //           break;
            //           case 7:
            //          	    index= Lever347_en_menu_index+Menu_lever3_select-1;	//char num.
            //           break;
            //           case 8:
            //          	    index= Lever348_en_menu_index+Menu_lever3_select-1;	//char num.
            //           break;
            //           case 9:				//照明灯
            //          	    index= Lever349_en_menu_index+Menu_lever3_select-1;	//char num.
            //           break;
            //        }
            //    break;
            //    case 6:
            //        switch(Menu_index_finc_select)
            //        {
            //           case 1:
            //          	    index= Lever361_en_menu_index+Menu_lever3_select-1;	//char num.
            //           break;
            //           case 2:
            //          	    index= Lever362_en_menu_index+Menu_lever3_select-1;	//char num.
            //           break;
            //           case 3:
            //          	    index= Lever363_en_menu_index+Menu_lever3_select-1;	//char num.
            //           break;
            //           case 4:
            //          	    index= Lever364_en_menu_index+Menu_lever3_select-1;	//char num.
            //           break;
            //           case 5:
            //          	    index= Lever365_en_menu_index+Menu_lever3_select-1;	//char num.
            //           break;
            //           case 6:
            //          	    index= Lever366_en_menu_index+Menu_lever3_select-1;	//char num.
            //           break;
            //       }
            //    break;
            //}
            //j16=(Menu_lever3_select-1)*16;
            //for(k=0;k<*index;k++)
            //{
            //    switch(Menu_index_select)
            //    {
            //        case 1:
            //             switch(Menu_index_finc_select)
            //             {
            //                 case 1:
            //                   Adds_index=Lever311_en_menu_char+j16;	//char data
            //                 break;
            //                 case 2:
            //                   Adds_index=Lever312_en_menu_char+j16;	//char data
            //                 break;
            //                 case 3:
            //                   Adds_index=Lever313_en_menu_char+j16;	//char data
            //                 break;
            //                 case 4:
            //                   Adds_index=Lever314_en_menu_char+j16;	//char data
            //                 break;
            //             }
            //        break;
            //        case 4:
            //             switch(Menu_index_finc_select)
            //             {
            //                 case 1:
            //                    Adds_index=Lever341_en_menu_char+j16;	//char data
            //                 break;
            //                 case 2:
            //                    Adds_index=Lever342_en_menu_char+j16;	//char data
            //                 break;
            //                 case 3:
            //                    Adds_index=Lever343_en_menu_char+j16;	//char data
            //                 break;
            //                 case 4:
            //                    Adds_index=Lever344_en_menu_char+j16;	//char data
            //                 break;
            //                 case 5:
            //                    Adds_index=Lever345_en_menu_char+j16;	//char data
            //                 break;
            //                 case 6:
            //                    Adds_index=Lever346_en_menu_char+j16;	//char data
            //                 break;
            //                 case 7:
            //                    Adds_index=Lever347_en_menu_char+j16;	//char data
            //                 break;
            //                 case 8:
            //                    Adds_index=Lever348_en_menu_char+j16;	//char data
            //                 break;
            //                 case 9:
            //                    Adds_index=Lever349_en_menu_char+j16;	//char data
            //                 break;
            //             }
            //        break;
            //        case 6:
            //             switch(Menu_index_finc_select)
            //             {
            //                 case 1:
            //                    Adds_index=Lever361_en_menu_char+j16;	//char data
            //                 break;
            //                 case 2:
            //                    Adds_index=Lever362_en_menu_char+j16;	//char data
            //                 break;
            //                 case 3:
            //                    Adds_index=Lever363_en_menu_char+j16;	//char data
            //                 break;
            //                 case 4:
            //                    Adds_index=Lever364_en_menu_char+j16;	//char data
            //                 break;
            //                 case 5:
            //                    Adds_index=Lever365_en_menu_char+j16;	//char data
            //                 break;
            //                 case 6:
            //                    Adds_index=Lever366_en_menu_char+j16;	//char data
            //                 break;
            //             }
            //        break;
            //    }
            //    Get_6x8_char_single(0,k*6+6,*Adds_index);
            //    ++j16;
            //}
        }
        else
        {
            j16=Menu_lever3_select-1;
            switch(Menu_index_select)
            {
            case 4:
                switch(Menu_index_finc_select)
                {
                case 1:
                    Adds_index=Lever3_menu31+j16*7;
                    index= Lever3_menu31_index+j16;
                    break;
                case 2:
                    Adds_index=Lever3_menu32+j16*7;
                    index= Lever3_menu32_index+j16;
                    break;
                case 3:
                    Adds_index=Lever3_menu33+j16*7;
                    index= Lever3_menu33_index+j16;
                    break;
                case 4:
                    Adds_index=Lever3_menu34+j16*7;
                    index= Lever3_menu34_index+j16;
                    break;
                case 5:
                    Adds_index=Lever3_menu37+j16*7;
                    index= Lever3_menu37_index+j16;
                    break;
                case 6:
                    Adds_index=Lever3_menu38+j16*7;
                    index= Lever3_menu38_index+j16;
                    break;
                case 7:
                    Adds_index=Lever3_menu39+j16*7;
                    index= Lever3_menu39_index+j16;
                    break;
                case 8:
                    Adds_index=Lever3_menu310+j16*7;
                    index= Lever3_menu310_index+j16;
                    break;
                case 9:
                    Adds_index=Lever3_menu311+j16*7;
                    index= Lever3_menu311_index+j16;
                    break;
                }
                break;
            case 1:
                switch(Menu_index_finc_select)
                {
                case 1:
                    Adds_index=Lever3_menu51+j16*7;
                    index= Lever3_menu51_index+j16;
                    break;
                case 2:
                    Adds_index=Lever3_menu52+j16*7;
                    index= Lever3_menu52_index+j16;
                    break;
                case 3:
                    Adds_index=Lever3_menu53+j16*7;
                    index= Lever3_menu53_index+j16;
                    break;
                case 4:
                    Adds_index=Lever3_menu54+j16*7;
                    index= Lever3_menu54_index+j16;
                    break;
                }
                break;
            case 6:
                switch(Menu_index_finc_select)
                {
                case 1:
                    Adds_index=Lever3_menu61+j16*7;
                    index= Lever3_menu61_index+j16;
                    break;
                case 2:
                    Adds_index=Lever3_menu62+j16*7;
                    index= Lever3_menu62_index+j16;
                    break;
                case 3:
                    Adds_index=Lever3_menu63+j16*7;
                    index= Lever3_menu63_index+j16;
                    break;
                case 4:
                    Adds_index=Lever3_menu64+j16*7;
                    index= Lever3_menu64_index+j16;
                    break;
                case 5:
                    Adds_index=Lever3_menu65+j16*7;
                    index= Lever3_menu65_index+j16;
                    break;
                case 6:
                    Adds_index=Lever3_menu66+j16*7;
                    index= Lever3_menu66_index+j16;
                    break;
                }
                break;
            }
//------------------------------------------------------------------------
            if((Menu_index_select!=2)&&(Menu_index_select!=5))
            {
                for(k=0; k<*index; k++)
                {
                    Get_16x16_char_single(0,k*12+6,*Adds_index);
                    ++Adds_index;
                }
            }
        }
L4_sub_1:
        for(j=0; j<128; j++)
        {
            if(M_menur1_language_select_temp==2)
                Disp_buff128x64[128+j]|=0x80;
            else
                Disp_buff128x64[128+j]=0x10;
        }
L4_sub_ex:
        Menu_lever4_bit_temp=*index_bit;
        Display_all_buff();
    }
}

//===============================================================================
//	finction	:Display_last_result	:LCM use 8080 system
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.27
//	modefy		:null
//===============================================================================
void Display_last_result(uint32_t adds)			//显示最后一次的分析结果
{
    uint8_t  buff[16],data_buff[16],j;
    uint32_t adds_a,adds_b,sub,res;
    float k;

    if(Menu_lever_select>0)		//2012-9-15 22:02
        goto ex_dsrs;

    adds_a=adds_b=adds;

    adds_b&=0x000001ff;						//byte adds
    adds_a>>=16;
    adds_a&=0x0000ffff;						//page adds
next_d:
    WDT_CLR();
    if((adds_b==0x00000000)&&(adds_a==0x00000004))		//get first adds
    {
        Spi_read(adds_a,buff_page);
        if((buff_page[0]==0xff)&&(buff_page[1]==0xff))
        {
            for(j=0; j<0x20; j++)
                buff_page[j]=0x00;

            Spi_write(adds_a,buff_page);
        }
        goto out_d;
    }
    else if(adds_b==0x00000000)
    {
        adds_b=0x000001e0;
        adds_a-=1;
    }
    else
        adds_b-=0x20;

    Spi_read(adds_a,buff_page);

    if((buff_page[adds_b+3]==0)&&(buff_page[adds_b+4]==0)&&(buff_page[adds_b+5]==0)&&(buff_page[adds_b+6]==0))
        goto next_d;
out_d:
    buff[0]=M_menur1_analy_obj_select_temp;
    buff[1]=buff_page[adds_b+2];		//status
    buff[2]=buff_page[adds_b+3];		//yh
    buff[3]=buff_page[adds_b+4];                //yl
    buff[4]=buff_page[adds_b+5];                //m
    buff[5]=buff_page[adds_b+6];                //d
    buff[6]=buff_page[adds_b+7];                //h
    buff[7]=buff_page[adds_b+8];                //m time
    buff[8]=buff_page[adds_b+9];
    buff[9]=buff_page[adds_b+10];
    buff[10]=buff_page[adds_b+11];
    buff[11]=buff_page[adds_b+12];
    buff[12]=buff_page[adds_b+15];		//signe_result
    buff[13]=buff_page[adds_b+13];		//unit
//-------------------------------------------------------------------
    Clear_n_page_ram(0,6);
    switch(buff[1])
    {
    case 1:	//hand fx
        if(M_menur1_language_select_temp==2)
        {
            data_buff[0]=22;
            data_buff[1]=36;
            data_buff[2]=16;
            data_buff[3]=17;
            data_buff[4]=0;
            data_buff[5]=0;
            data_buff[6]=0;
            data_buff[7]=0;
        }
        else
        {
            data_buff[0]='M';
            data_buff[1]='A';
            data_buff[2]='N';
            data_buff[3]=19;
            data_buff[4]=0;
            data_buff[5]='A';
            data_buff[6]='N';
            data_buff[7]='A';
        }
        break;
    case 2:	//hand hc
        if(M_menur1_language_select_temp==2)
        {
            data_buff[0]=22;
            data_buff[1]=36;
            data_buff[2]=39;
            data_buff[3]=40;
            data_buff[4]=0;
            data_buff[5]=0;
            data_buff[6]=0;
            data_buff[7]=0;
        }
        else
        {
            data_buff[0]='M';
            data_buff[1]='A';
            data_buff[2]='N';
            data_buff[3]=19;
            data_buff[4]=0;
            data_buff[5]='C';
            data_buff[6]='H';
            data_buff[7]='K';
        }
        break;
    case 0:
    case 3:	//auto fx
        if(M_menur1_language_select_temp==2)
        {
            data_buff[0]=35;
            data_buff[1]=36;
            data_buff[2]=16;
            data_buff[3]=17;
            data_buff[4]=0;
            data_buff[5]=0;
            data_buff[6]=0;
            data_buff[7]=0;
        }
        else
        {
            data_buff[0]='A';
            data_buff[1]='U';
            data_buff[2]='T';
            data_buff[3]='O';
            data_buff[4]=0;
            data_buff[5]='A';
            data_buff[6]='N';
            data_buff[7]='A';
        }
        break;
    case 4:	//auto hc
        if(M_menur1_language_select_temp==2)
        {
            data_buff[0]=35;
            data_buff[1]=36;
            data_buff[2]=39;
            data_buff[3]=40;
            data_buff[4]=0;
            data_buff[5]=0;
            data_buff[6]=0;
            data_buff[7]=0;
        }
        else
        {
            data_buff[0]='A';
            data_buff[1]='U';
            data_buff[2]='T';
            data_buff[3]='O';
            data_buff[4]=0;
            data_buff[5]='C';
            data_buff[6]='H';
            data_buff[7]='K';
        }
        break;
    case 5:	//auto xs
        if(M_menur1_language_select_temp==2)
        {
            data_buff[0]=35;
            data_buff[1]=36;
            data_buff[2]=67;
            data_buff[3]=68;
            data_buff[4]=0;
            data_buff[5]=0;
            data_buff[6]=0;
            data_buff[7]=0;
        }
        else
        {
            data_buff[0]='A';
            data_buff[1]='U';
            data_buff[2]='T';
            data_buff[3]='O';
            data_buff[4]=0;
            data_buff[5]='D';
            data_buff[6]='I';
            data_buff[7]='L';
        }
        break;
    case 6:	//hand jz
        if(M_menur1_language_select_temp==2)
        {
            data_buff[0]=21;
            data_buff[1]=35;
            data_buff[2]=36;
            data_buff[3]=37;
            data_buff[4]=0;
            data_buff[5]=0;
            data_buff[6]=0;
            data_buff[7]=0;
        }
        else
        {
            data_buff[0]='M';
            data_buff[1]='A';
            data_buff[2]='N';
            data_buff[3]=1;
            data_buff[4]=0;
            data_buff[5]='C';
            data_buff[6]='A';
            data_buff[7]='L';
        }
        break;
//       case 0:
//           data_buff[0]=0;
//           data_buff[1]=0;
//           data_buff[2]=0;
//           data_buff[3]=0;
//           data_buff[4]=0;
//           data_buff[5]=0;
//           data_buff[6]=0;
//           data_buff[7]=0;
//       break;
    }
    data_buff[8]=0;
    data_buff[9]=0;
    data_buff[13]=0;
    data_buff[14]=0;
    switch(buff[0])
    {
    case 0:
    case 1:
        data_buff[10]='N';
        data_buff[11]='H';
/////           data_buff[12]='4';
        data_buff[12]=0;
        data_buff[13]=11;
        data_buff[14]='N';
        break;
    case 2:
        data_buff[10]='P';
        data_buff[11]='O';
/////           data_buff[12]='4';
        data_buff[12]=0;
        break;
    case 3:
        data_buff[10]='C';
        data_buff[11]='U';
        data_buff[12]=0;
        break;
    case 4:
        data_buff[10]='C';
        data_buff[11]='O';
        data_buff[12]='D';
        break;
    case 5:
        data_buff[10]='N';
        data_buff[11]='I';
        data_buff[12]=0;
        break;
    case 6:
        data_buff[10]='C';
        data_buff[11]='N';
        data_buff[12]=0;
        break;
    case 7:
        data_buff[10]='C';
        data_buff[11]='R';
        data_buff[12]=0;
        break;
    case 8:
        data_buff[10]='F';
        data_buff[11]='E';
        data_buff[12]=0;
        break;
    case 9:
        data_buff[10]='M';
        data_buff[11]='N';
        data_buff[12]=0;
        break;
    case 10:
        data_buff[10]='F';
        data_buff[11]=0;
        data_buff[12]=0;
        break;
    case 11:
        data_buff[10]='P';
        data_buff[11]='H';
        data_buff[12]='E';
        data_buff[13]='N';
        break;
    case 12:
        data_buff[10]='P';
        data_buff[11]='B';
        data_buff[12]=0;
        break;
    case 13:
        data_buff[10]='C';
        data_buff[11]='D';
        data_buff[12]=0;
        break;
    case 14:
        data_buff[10]='Z';
        data_buff[11]='N';
        data_buff[12]=0;
        break;
    case 15:
        data_buff[10]='T';
        data_buff[11]='N';
        data_buff[12]=0;
        break;
    case 16:
        data_buff[10]='T';
        data_buff[11]='P';
        data_buff[12]=0;
        break;
    }
    for(j=0; j<5; j++)
        Get_8x16_char_single(0,76+j*10,data_buff[j+10]);

    if((buff[0]==1)||(buff[0]==2))
    {
        Get_6x8_char_single(1,96,'4');
        for(j=0; j<20; j++)
        {
            Disp_buff128x64[102+j]=Disp_buff128x64[106+j];
            Disp_buff128x64[230+j]=Disp_buff128x64[234+j];
        }
        for(j=0; j<4; j++)
        {
            Disp_buff128x64[122+j]=0;
            Disp_buff128x64[250+j]=0;
        }
    }
    if(M_menur1_language_select_temp==2)
    {
        for(j=0; j<4; j++)
            Get_16x16_char_single(0,j*12,data_buff[j]);
    }
    else
    {
        for(j=0; j<10; j++)
            Get_6x8_char_single(1,j*6,data_buff[j]);

        data_buff[0]='L';
        data_buff[1]='A';
        data_buff[2]='S';
        data_buff[3]='T';
        data_buff[4]=0;
        data_buff[5]='R';
        data_buff[6]='E';
        data_buff[7]='S';
        data_buff[8]='U';
        data_buff[9]='L';
        data_buff[10]='T';
        for(j=0; j<11; j++)
            Get_6x8_char_single(0,j*6,data_buff[j]);
    }
//-------------------------------------------------------------
    j=buff[2];
    j=j>>4;
    j&=0x0f;
    data_buff[6]=Get_ascii_data(j);
    j=buff[2];
    j&=0x0f;
    data_buff[7]=Get_ascii_data(j);
    j=buff[3];
    j=j>>4;
    j&=0x03;
    data_buff[8]=Get_ascii_data(j);
    j=buff[3];
    j&=0x0f;
    data_buff[9]=Get_ascii_data(j);
    data_buff[10]=14;
    j=buff[4];
    j=j>>4;
    j&=0x01;
    data_buff[11]=Get_ascii_data(j);
    j=buff[4];
    j&=0x0f;
    data_buff[12]=Get_ascii_data(j);
    data_buff[13]=14;
    j=buff[5];
    j=j>>4;
    j&=0x03;
    data_buff[14]=Get_ascii_data(j);
    j=buff[5];
    j&=0x0f;
    data_buff[15]=Get_ascii_data(j);
    data_buff[5]=0;
    j=buff[6];
    j=j>>4;
    j&=0x03;
    data_buff[0]=Get_ascii_data(j);
    j=buff[6];
    j&=0x0f;
    data_buff[1]=Get_ascii_data(j);
    data_buff[2]=20;
    j=buff[7];
    j=j>>4;
    j&=0x07;
    data_buff[3]=Get_ascii_data(j);
    j=buff[7];
    j&=0x0f;
    data_buff[4]=Get_ascii_data(j);

    for(j=0; j<6; j++)
        Get_6x8_char_single(3,j*6,data_buff[j]);		//line2
    for(j=0; j<10; j++)
        Get_6x8_char_single(3,j*6+66,data_buff[j+6]);		//line2
    //---------------------------------------------------------
/////   j=buff[12];
/////   if(j!=2)
/////      data_buff[0]=0;
/////   else
    data_buff[0]=0;

    res=0;
    res=buff[8]<<24;
    sub=0;
    sub=buff[9]<<16;
    res|=sub;
    sub=0;
    sub=buff[10]<<8;
    res|=sub;
    sub=0;
    sub=buff[11];
    res|=sub;

    j=buff[8];
    j=j>>4;
    j&=0x0f;

    if(j==0)
        data_buff[1]=0;
    else
        data_buff[1]=Get_ascii_data(j);

    sub=j*1000;
    k=sub;
    Pwm_last_result+=k;

    j=buff[8];
    j&=0x0f;
    if((j==0)&&(data_buff[1]==0))
        data_buff[2]=0;
    else
        data_buff[2]=Get_ascii_data(j);

    sub=j*100;
    k=sub;
    Pwm_last_result+=k;

    j=buff[9];
    j=j>>4;
    j&=0x0f;
    if((j==0)&&(data_buff[1]==0)&&(data_buff[2]==0))
        data_buff[3]=0;
    else
        data_buff[3]=Get_ascii_data(j);

    sub=j*10;
    k=sub;
    Pwm_last_result+=k;

    j=buff[9];
    j&=0x0f;
    data_buff[4]=Get_ascii_data(j);

    sub=j;
    k=sub;
    Pwm_last_result+=k;

    data_buff[5]=19;
    j=buff[10];
    j=j>>4;
    j&=0x0f;
    data_buff[6]=Get_ascii_data(j);

    k=j;
    k/=10.0;
    Pwm_last_result+=k;

    j=buff[10];
    j&=0x0f;
    data_buff[7]=Get_ascii_data(j);

    k=j;
    k/=100.0;
    Pwm_last_result+=k;

    j=buff[11];
    j=j>>4;
    j&=0x0f;
    data_buff[8]=Get_ascii_data(j);

    k=j;
    k/=1000.0;
    Pwm_last_result+=k;

    j=buff[11];
    j&=0x0f;
    data_buff[9]=Get_ascii_data(j);

//   switch(M_menur1_dot_num_select_temp)
//   {
//      case 1:
//          data_buff[7]=data_buff[8]=data_buff[9]=0;
//      break;
//      case 2:
//          data_buff[8]=data_buff[9]=0;
//      break;
//      case 3:
//          data_buff[9]=0;
//      break;
//   }

    data_buff[10]='M';
    data_buff[11]='G';
    data_buff[12]=18;
    data_buff[13]='L';
    switch(buff[13])
    {
    case 1:
        data_buff[10]='G';
        data_buff[11]=18;
        data_buff[12]='L';
        data_buff[13]=0;
        break;
    case 3:
        data_buff[10]='U';
        break;
    }
////   Display_8x16_char(4,0,14,data_buff);		//line3
    Get_6x8_char_single(5,0,data_buff[0]);
    Get_6x8_char_single(5,46,data_buff[5]);
    for(j=0; j<4; j++)
        Get_6x8_char_single(5,98+j*6,data_buff[10+j]);
    for(j=0; j<4; j++)
        Get_8x16_char_single(4,6+j*10,data_buff[j+1]);
    for(j=0; j<4; j++)
        Get_8x16_char_single(4,52+j*10,data_buff[j+6]);

    j=buff[12];
    if((j!=1)&&(res!=0))
    {
        for(j=0; j<5; j++)
            Disp_buff128x64[5*128+18+j]=0x03;
    }
//--------------------------------------------------
    for(j=0; j<128; j++)
        Disp_buff128x64[128*2+j]=0x10;

    Display_all_buff();
ex_dsrs:
    __nop();
    Pwm_get_resul_from_flash(1);		//2016/4/20 8:45
////   Disp_rtc_clock();
}

//===============================================================================
//	finction	:Display_menu_index	:LCM use 8080 system
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.27
//	modefy		:null
//===============================================================================
void Display_menu_index(void)		//菜单显示
{
    LCD_disp_flag=1;
    switch(Menu_lever_select)
    {
    case 0:				//main menu
//    	     Display_logo(cm_logo);
        Display_last_result(FX_write_index_adds_point);	//2012-9-14 10:43
        Disp_rtc_clock();
        break;
//------------------------------------------------------------------------------
    case 1:				//lever1 menu
        Disp_lever1_menu();	//显示一级菜单

        if((Far_status_flag!=0)&&(Menu_lever3_start_flag<0x10)&&(Hand_finction_select==0))
            Set_tx_cmd_data_finc(0xd1,0x06,U_DRIVER_FA_ALL_WO_ADDS,0x0000,0x00);		//close all v
        Far_status_flag=0;
        break;
//------------------------------------------------------------------------------
    case 2:				//lever2 menu
        Disp_lever2_menu();	//显示二级菜单
        break;
//------------------------------------------------------------------------------
    case 3:
        Disp_lever3_menu();	//显示三级菜单
        break;
    }
    LCD_disp_flag=0;
}

//===============================================================================
//	finction	:Display_program_menu	:LCM use 8080 system
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.27
//	modefy		:null
//===============================================================================
void Display_program_menu(uint8_t num,uint8_t disp_buff[])	//显示分析流程中每一步的名称
{
    uint8_t i;
/////     uint16_t cnt;
/////     uint8_t const *index;

    if(Prog_disp_flag==1)
    {
        Clear_n_page_ram(2,1);
        for(i=0; i<num; i++)
        {
            Get_16x16_char_single(2,i*12+6,disp_buff[i]);
        }
        Display_all_buff();
    }
}

//===============================================================================
//	finction	:Display_program_5opration_re	:LCM use 8080 system
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.27
//	modefy		:null
//===============================================================================
void Display_program_5opration_re(uint8_t num,uint8_t disp_buff[])	//显示5个分析流程的名称
{
    uint8_t i;
/////   uint16_t cnt;
/////   uint8_t const *index;

    Clear_n_page_ram(0,1);
    for(i=0; i<num; i++)
    {
        Get_16x16_char_single(0,i*12+6,disp_buff[i]);
    }
    Display_all_buff();
}

//===============================================================================
//	finction	:Display_program_5opration	:LCM use 8080 system
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012.06.27
//	modefy		:null
//===============================================================================
void Display_program_5opration(uint8_t num,uint8_t disp_buff[])		//显示5个分析流程的名称
{
    uint8_t i;
/////     uint16_t cnt;
/////     uint8_t const *index;

    if(Prog_disp_flag==1)
    {
        if((Memu1_auto_step_status==0)||(Memu1_auto_step_status==0xff))
        {
            Hand_finction_select=0;
            if(Pro_dis_menu_flag==0)
            {
                Pro_dis_menu_flag=1;
                Clear_n_page_ram(0,1);

                Set_back_light_10sec();
                for(i=0; i<num; i++)
                {
                    Get_16x16_char_single(0,i*12+6,disp_buff[i]);
                }
                Display_all_buff();
            }
        }
    }
}

//===============================================================================
//	finction	:Display_program_result	:LCM use 8080 system
//	input		:num  1:HF 2:HJ 3:BH 4:XF
//	output		:null
//	return		:null
//	edit		:sam 2012.06.27
//	modefy		:null
//===============================================================================
void Display_program_result(uint8_t num)	//page 5,6	分析完成，排空清洗后显示分析结果
{
    uint8_t   data_buff[18],i;
    uint32_t  disp_d;
    float k=0;

    for(i=0; i<18; i++)
        data_buff[i]=0;

    if(Prog_disp_flag>2)
    {
        if(Menu_lever_select==0) Prog_disp_flag=1;
        else  Prog_disp_flag=2;
    }

    if(Prog_disp_flag==1)
    {
        Clear_n_page(2,6);
    }

    Set_en_buzz_finc(1,4,200,100);
    if(num!=0)
    {

        data_buff[0]='R';
        data_buff[1]=20;
        data_buff[12]=0;
        data_buff[13]='M';
        data_buff[14]='G';
        data_buff[15]=18;
        data_buff[16]='L';
        switch(M_menur1_unit_select_temp)
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
            data_buff[13]='P';
            data_buff[14]='P';
            data_buff[15]='M';
            data_buff[16]=0;
            break;
        case 5:
            data_buff[13]='P';
            data_buff[14]='P';
            data_buff[15]='B';
            data_buff[16]=0;
            break;
        }
        switch(num)
        {
        case 1:
            if(c1_prog_use!=0)
                k=c1_prog_use;
            else if(c2_prog_use!=0)
                k=c2_prog_use;
            else if(c3_prog_use!=0)
                k=c3_prog_use;
            else
                k=0;
            break;
        case 2:
            data_buff[11]=0;
            if(k1_prog_use!=0)
            {
                k=k1_prog_use;
                data_buff[13]='C';
                data_buff[14]='A';
                data_buff[15]='L';
                data_buff[16]=0;
            }
            else if(k2_prog_use!=0)
            {
                k=k2_prog_use;
                data_buff[13]=0;
                data_buff[14]='K';
                data_buff[15]=0;
                data_buff[16]=0;
            }
            else
            {
                data_buff[13]=0;
                data_buff[14]=0;
                data_buff[15]=0;
                data_buff[16]=0;
            }
            break;
        case 3:
            if(c1_prog_use!=0)
                k=c1_prog_use;
            else if(c2_prog_use!=0)
                k=c2_prog_use;
            else if(c3_prog_use!=0)
                k=c3_prog_use;
            else
                k=0;
            break;
        case 4:
            if(c1_prog_use!=0)
                k=c1_prog_use;
            else if(c2_prog_use!=0)
                k=c2_prog_use;
            else if(c3_prog_use!=0)
                k=c3_prog_use;
            else
                k=0;
            break;
        }

        if(k>=0)
        {
            data_buff[2]=0;
            result_signe=1;
        }
        else
        {
            data_buff[2]=14;
            result_signe=2;
        }

        disp_d=Float_to_int_reg(k);
        Pub_result_temp=disp_d;

        i=disp_d>>28;
        i&=0x0f;
        if(i>=0x0a)
            i=0;

        if(i==0)
            data_buff[3]=0;
        else
            data_buff[3]=Get_ascii_data(i);
        i=disp_d>>24;
        i&=0x0f;
        if((i==0)&&(data_buff[3]==0))
            data_buff[4]=0;
        else
            data_buff[4]=Get_ascii_data(i);
        i=disp_d>>20;
        i&=0x0f;
        if((i==0)&&(data_buff[3]==0)&&(data_buff[4]==0))
            data_buff[5]=0;
        else
            data_buff[5]=Get_ascii_data(i);
        i=disp_d>>16;
        i&=0x0f;
        data_buff[6]=Get_ascii_data(i);
        data_buff[7]=19;
        i=disp_d>>12;
        i&=0x0f;
        data_buff[8]=Get_ascii_data(i);
        i=disp_d>>8;
        i&=0x0f;
        data_buff[9]=Get_ascii_data(i);
        i=disp_d>>4;
        i&=0x0f;
        data_buff[10]=Get_ascii_data(i);
        i=disp_d;
        i&=0x0f;
        data_buff[11]=Get_ascii_data(i);

        switch(M_menur1_dot_num_select_temp)
        {
        case 1:
            data_buff[9]=data_buff[10]=data_buff[11]=0;
            break;
        case 2:
            data_buff[10]=data_buff[11]=0;
            break;
        case 3:
            data_buff[11]=0;
            break;
        }

        if(Prog_disp_flag==1)
            Display_6x8_char(3,6,17,data_buff);	//disp result

        data_buff[0]='E';
        data_buff[1]='1';
//     	data_buff[0]='Q';
//     	data_buff[1]=20;
        data_buff[2]=20;
        data_buff[12]=0;
        data_buff[13]='M';
        data_buff[14]='V';

        //Measurement_signal_11 Measurement_signal_21
        if(M_menur1_LZDJ_select_temp==2)
        {
            k=Measurement_signal_11;

//           if(Measurement_signal_11<25000)
//           {
//               data_buff[2]=4;		//-
//               k=25000-k;
//           }
//           else
//           {
//               data_buff[2]=0;		//+
//               k=k-25000;
//           }
        }
        else
        {
            k=Measurement_signal_21;

            if(Measurement_signal_21<31250)
            {
                data_buff[3]=14;		//-
                k=31250-k;
            }
            else
            {
                data_buff[3]=0;		//+
                k=k-31250;
            }
        }

        if(M_menur1_LZDJ_select_temp==2)
            disp_d=Float_to_int_reg_ee(k);
        else
            disp_d=Float_to_int_reg_ee2(k);
        i=disp_d>>28;
        i&=0x0f;
        if(i>=0x0a)
            i=0;
        data_buff[4]=Get_ascii_data(i);
        i=disp_d>>24;
        i&=0x0f;
        data_buff[5]=Get_ascii_data(i);
        i=disp_d>>20;
        i&=0x0f;
        data_buff[6]=Get_ascii_data(i);
        i=disp_d>>16;
        i&=0x0f;
        data_buff[7]=Get_ascii_data(i);
        data_buff[8]=19;
        i=disp_d>>12;
        i&=0x0f;
        data_buff[9]=Get_ascii_data(i);
        i=disp_d>>8;
        i&=0x0f;
        data_buff[10]=Get_ascii_data(i);
        data_buff[11]=0;

        if(Prog_disp_flag==1)
            Display_6x8_char(4,6,15,data_buff);	//disp E1

        //Measurement_signal_12 Measurement_signal_22
        if(M_menur1_LZDJ_select_temp==2)
        {
            k=Measurement_signal_12;

//           if(Measurement_signal_12<25000)
//           {
//               data_buff[2]=4;		//-
//               k=25000-k;
//           }
//           else
//           {
//               data_buff[2]=0;		//+
//               k=k-25000;
//           }
        }
        else
        {
            k=Measurement_signal_22;

            if(Measurement_signal_22<31250)
            {
                data_buff[3]=14;		//-
                k=31250-k;
            }
            else
            {
                data_buff[3]=0;		//+
                k=k-31250;
            }
        }

        if(M_menur1_LZDJ_select_temp==2)
            disp_d=Float_to_int_reg_ee(k);
        else
            disp_d=Float_to_int_reg_ee2(k);
////     	data_buff[0]='E';
        data_buff[1]='2';

        i=disp_d>>28;
        i&=0x0f;
        if(i>=0x0a)
            i=0;
        data_buff[4]=Get_ascii_data(i);
        i=disp_d>>24;
        i&=0x0f;
        data_buff[5]=Get_ascii_data(i);
        i=disp_d>>20;
        i&=0x0f;
        data_buff[6]=Get_ascii_data(i);
        i=disp_d>>16;
        i&=0x0f;
        data_buff[7]=Get_ascii_data(i);
        data_buff[8]=19;
        i=disp_d>>12;
        i&=0x0f;
        data_buff[9]=Get_ascii_data(i);
        i=disp_d>>8;
        i&=0x0f;
        data_buff[10]=Get_ascii_data(i);
        data_buff[11]=0;

        if(Prog_disp_flag==1)
        {
            if((M_menur1_LZDJ_select_temp==1)&&(M_menur1_DL_select_temp==2))
                __nop();
            else
                Display_6x8_char(5,6,15,data_buff);	//disp E2
        }
    }
    else if(Memu1_auto_step_status>23)
    {
        data_buff[0]='R';
        data_buff[1]=20;
        data_buff[12]=0;
        data_buff[13]='M';
        data_buff[14]='G';
        data_buff[15]=18;
        data_buff[16]='L';
        switch(M_menur1_unit_select_temp)
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
            data_buff[13]='P';
            data_buff[14]='P';
            data_buff[15]='M';
            data_buff[16]=0;
            break;
        case 5:
            data_buff[13]='P';
            data_buff[14]='P';
            data_buff[15]='B';
            data_buff[16]=0;
            break;
        }
        if(c1_prog_use!=0)
            k=c1_prog_use;
        else if(c2_prog_use!=0)
            k=c2_prog_use;
        else if(c3_prog_use!=0)
            k=c3_prog_use;
        else
            k=0;

        if(k>=0)
        {
            data_buff[2]=0;
            result_signe=1;
        }
        else
        {
            data_buff[2]=14;
            result_signe=2;
        }

        Pwm_last_result=k;				//2012-9-20 16:53

        disp_d=Float_to_int_reg(k);
        Pub_result_temp=disp_d;

        i=disp_d>>28;
        i&=0x0f;
        if(i>=0x0a)
            i=0;

        if(i==0)
            data_buff[3]=0;
        else
            data_buff[3]=Get_ascii_data(i);
        i=disp_d>>24;
        i&=0x0f;
        if((i==0)&&(data_buff[3]==0))
            data_buff[4]=0;
        else
            data_buff[4]=Get_ascii_data(i);
        i=disp_d>>20;
        i&=0x0f;
        if((i==0)&&(data_buff[3]==0)&&(data_buff[4]==0))
            data_buff[5]=0;
        else
            data_buff[5]=Get_ascii_data(i);
        i=disp_d>>16;
        i&=0x0f;
        data_buff[6]=Get_ascii_data(i);
        data_buff[7]=19;
        i=disp_d>>12;
        i&=0x0f;
        data_buff[8]=Get_ascii_data(i);
        i=disp_d>>8;
        i&=0x0f;
        data_buff[9]=Get_ascii_data(i);
        i=disp_d>>4;
        i&=0x0f;
        data_buff[10]=Get_ascii_data(i);
        i=disp_d;
        i&=0x0f;
        data_buff[11]=Get_ascii_data(i);

        switch(M_menur1_dot_num_select_temp)
        {
        case 1:
            data_buff[9]=data_buff[10]=data_buff[11]=0;
            break;
        case 2:
            data_buff[10]=data_buff[11]=0;
            break;
        case 3:
            data_buff[11]=0;
            break;
        }

        if(Prog_disp_flag==1)
            Display_6x8_char(3,6,17,data_buff);	//disp result

        data_buff[0]='E';
        data_buff[1]='1';
//     	data_buff[0]='Q';
//     	data_buff[1]=2;
        data_buff[2]=20;
        data_buff[12]=0;
        data_buff[13]='M';
        data_buff[14]='V';

        //Measurement_signal_11 Measurement_signal_21
        if(M_menur1_LZDJ_select_temp==2)
            k=Measurement_signal_11;
        else
        {
            k=Measurement_signal_21;

            if(Measurement_signal_21<31250)
            {
                data_buff[3]=14;		//-
                k=31250-k;
            }
            else
            {
                data_buff[3]=0;		//+
                k=k-31250;
            }
        }

        if(M_menur1_LZDJ_select_temp==2)
            disp_d=Float_to_int_reg_ee(k);
        else
            disp_d=Float_to_int_reg_ee2(k);
        i=disp_d>>28;
        i&=0x0f;
        if(i>=0x0a)
            i=0;
        data_buff[4]=Get_ascii_data(i);
        i=disp_d>>24;
        i&=0x0f;
        data_buff[5]=Get_ascii_data(i);
        i=disp_d>>20;
        i&=0x0f;
        data_buff[6]=Get_ascii_data(i);
        i=disp_d>>16;
        i&=0x0f;
        data_buff[7]=Get_ascii_data(i);
        data_buff[8]=19;
        i=disp_d>>12;
        i&=0x0f;
        data_buff[9]=Get_ascii_data(i);
        i=disp_d>>8;
        i&=0x0f;
        data_buff[10]=Get_ascii_data(i);
        data_buff[11]=0;

        if(Prog_disp_flag==1)
            Display_6x8_char(4,6,15,data_buff);	//disp E1

        //Measurement_signal_12 Measurement_signal_22
        if(M_menur1_LZDJ_select_temp==2)
            k=Measurement_signal_12;
        else
        {
            k=Measurement_signal_22;

            if(Measurement_signal_22<31250)
            {
                data_buff[3]=14;		//-
                k=31250-k;
            }
            else
            {
                data_buff[3]=0;		//+
                k=k-31250;
            }
        }

        if(M_menur1_LZDJ_select_temp==2)
            disp_d=Float_to_int_reg_ee(k);
        else
            disp_d=Float_to_int_reg_ee2(k);
        data_buff[1]='2';

        i=disp_d>>28;
        i&=0x0f;
        data_buff[4]=Get_ascii_data(i);
        i=disp_d>>24;
        i&=0x0f;
        data_buff[5]=Get_ascii_data(i);
        i=disp_d>>20;
        i&=0x0f;
        data_buff[6]=Get_ascii_data(i);
        i=disp_d>>16;
        i&=0x0f;
        data_buff[7]=Get_ascii_data(i);
        data_buff[8]=19;
        i=disp_d>>12;
        i&=0x0f;
        data_buff[9]=Get_ascii_data(i);
        i=disp_d>>8;
        i&=0x0f;
        data_buff[10]=Get_ascii_data(i);
        data_buff[11]=0;

        if(Prog_disp_flag==1)
        {
            if((M_menur1_LZDJ_select_temp==1)&&(M_menur1_DL_select_temp==2))
                __nop();
            else
                Display_6x8_char(5,6,15,data_buff);	//disp E2
        }
    }
}

//===============================================================================
//	finction	:Disp_input_switch_status	:LCM use 8080 system
//	input		:num  1:HF 2:HJ 3:BH 4:XF
//	output		:null
//	return		:null
//	edit		:sam 2012.06.27
//	modefy		:null
//===============================================================================
void Disp_input_switch_status(uint8_t select)
{
    uint16_t	op_adds=0,status_temp;
    uint8_t	data_buff_o[2],data_buff_c[3],i;

    switch(select)
    {
    case 1:
        op_adds=U_DRIVER_PORT1_IN_RO_ADDS;
        break;
    case 2:
        op_adds=U_DRIVER_PORT1_IN_RO_ADDS;
        break;
    case 3:
        op_adds=U_DRIVER_PORT1_IN_RO_ADDS;
        break;
    case 4:
        op_adds=U_DRIVER_PORT1_IN_RO_ADDS;
        break;
    case 5:
        op_adds=U_DRIVER_PORT1_IN_RO_ADDS;
        break;
    case 6:
        op_adds=U_DRIVER_PORT1_IN_RO_ADDS;
        break;
    case 7:
        op_adds=U_DRIVER_PORT1_IN_RO_ADDS;
        break;
    case 8:
        op_adds=U_DRIVER_PORT1_IN_RO_ADDS;
        break;
    }
    Rx_TX_flag_local=1;
    Set_tx_cmd_data_finc(0xd1,0x03,op_adds,0x0001,0x00);
    for(i=0; i<250; i++)
    {
        DrvSYS_Delay(6000);
        if(Rx_TX_flag_local==0)
            i=250;
    }
    Rx_TX_flag_local=2;
    Uart_tx_rx_fifo_int_FLAG&=~BIT3;
/////     DrvSYS_Delay(2000);				//delay 2ms
    Com1_rd_ctrl(0);
    DrvGPIO_SetBit_new(E_GPB,6);
///     for(i=0;i<255;i++)				//clear tx buff data
///     {
///        U_data_buff[i]=0;
///     }
    U_Rx_wait_time_flag_temp=0x8000;		//set wait 10sec fot time out
    for(i=0; i<250; i++)
    {
        DrvSYS_Delay(6000);
        if(Rx_TX_flag_local==0)
            i=250;
    }

    Uart_tx_rx_fifo_int_FLAG&=~BIT2;

    status_temp=0x0000;
    status_temp=U_data_buff[3];
    status_temp=status_temp<<8;
    status_temp+=U_data_buff[4];

    data_buff_c[0]=0;
    data_buff_c[1]=0;
    data_buff_c[2]=0;
    for(i=0; i<4; i++)
    {
        Display_8x16_char(2*i,72,3,data_buff_c);
    }

    if(M_menur1_language_select_temp==2)
    {
        data_buff_o[0]=184;
        data_buff_o[1]=185;
        data_buff_c[0]=186;
        data_buff_c[1]=187;
        data_buff_c[2]=0;
    }
    else
    {
        data_buff_o[0]='O';
        data_buff_o[1]='N';
        data_buff_c[0]='O';
        data_buff_c[1]='F';
        data_buff_c[2]='F';
    }
    if(status_temp==0)
        Display_8x16_char(2*(Menu_lever3_point-1),72,2,data_buff_o);
    else
        Display_8x16_char(2*(Menu_lever3_point-1),72,3,data_buff_c);
}


//===============================================================================
//	finction	:Disp_GDGDY_real_time
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-6 16:42
//	modefy		:null
//===============================================================================
void Disp_GDGDY_real_time(void)		//实时显示光电管，电极电压和温度值
{
    uint8_t i,data_buff[16];		//for setting disp
    uint16_t j,pb_temp,j16;
    uint32_t calcula_tp,kkk;

    if(((Disp_gdgdy_flag==1)||(Disp_gdgdy_flag==2)||(Disp_gdgdy_flag==3))&&(Pro_wait_time_flag==2))
    {
        Back_light_time_flag=0;

        Rx_TX_flag_local=1;
        if(Disp_gdgdy_flag==1)
            Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_ADC_PHOTO_ADDS,0x0001,0x00);
        else if(Disp_gdgdy_flag==2)
        {
            if(M_menur1_LZDJ_select_temp==1)	//电极法
                Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_ADC_TEMP1_ADDS,0x0001,0x00);    	//2012-10-24 16:38
            else
                Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_ADC_TEMP_ADDS,0x0001,0x00);
        }
        else  Set_tx_cmd_data_finc(U_SENCER_MODU_ADDS,0x03,U_SENSER_ADC_ISE_ADDS,0x0001,0x00);

        for(i=0; i<250; i++)
        {
            DrvSYS_Delay(2000);
            if(Rx_TX_flag_local==0)
                i=250;
        }
        Rx_TX_flag_local=2;
        Uart_tx_rx_fifo_int_FLAG&=~BIT3;
        DrvSYS_Delay(2000);				//delay 2ms
        Com1_rd_ctrl(0);
        DrvGPIO_SetBit_new(E_GPB,6);

        U_Rx_wait_time_flag_temp=0x8000;		//set wait 10sec fot time out
        for(i=0; i<250; i++)
        {
            DrvSYS_Delay(2000);
            if(Rx_TX_flag_local==0)
                i=250;
        }

        Uart_tx_rx_fifo_int_FLAG&=~BIT2;
        U_Rx_wait_time_flag_temp=0x0000;
        if(Disp_gdgdy_flag==1)
        {
            M_lever4_GDGDY_temp=0x00000000;
            M_lever4_GDGDY_temp=U_data_buff[3];
            M_lever4_GDGDY_temp=M_lever4_GDGDY_temp<<8;
            M_lever4_GDGDY_temp+=U_data_buff[4];

            calcula_tp=0x00000000;
            j=M_lever4_GDGDY_temp/10000;
            kkk=j<<16;
            calcula_tp+=kkk;
            j=M_lever4_GDGDY_temp/1000;
            j=j%10;
            kkk=j<<12;
            calcula_tp+=kkk;
            j=M_lever4_GDGDY_temp/100;
            j=j%10;
            kkk=j<<8;
            calcula_tp+=kkk;
            j=M_lever4_GDGDY_temp/10;
            j=j%10;
            kkk=j<<4;
            calcula_tp+=kkk;
            j=M_lever4_GDGDY_temp%10;
            calcula_tp+=j;

            j=calcula_tp>>16;
            j&=0x0f;
            data_buff[0]=Get_ascii_data(j);
            j=calcula_tp>>12;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            j=calcula_tp>>8;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            j=calcula_tp>>4;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            data_buff[4]=19;
            j=calcula_tp;
            j&=0x0f;
            data_buff[5]=Get_ascii_data(j);
            data_buff[6]='0';
            data_buff[7]='M';
            data_buff[8]='V';

            for(i=0; i<9; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);
        }
        else if(Disp_gdgdy_flag==2)
        {
            Back_light_time_flag=0;

            M_lever4_WD_temp=0x00000000;
            M_lever4_WD_temp=U_data_buff[3];
            M_lever4_WD_temp=M_lever4_WD_temp<<8;
            M_lever4_WD_temp+=U_data_buff[4];

            if(M_lever4_WD_temp>=1024)
            {
                data_buff[0]=13;		//+
                M_lever4_WD_temp-=1024;
            }
            else
            {
                data_buff[0]=14;		//-
                M_lever4_WD_temp=1024-M_lever4_WD_temp;
            }

            calcula_tp=0x00000000;
            j=M_lever4_WD_temp/1000;
            kkk=j<<12;
            calcula_tp+=kkk;
            j=M_lever4_WD_temp/100;
            j=j%10;
            kkk=j<<8;
            calcula_tp+=kkk;
            j=M_lever4_WD_temp/10;
            j=j%10;
            kkk=j<<4;
            calcula_tp+=kkk;
            j=M_lever4_WD_temp%10;
            calcula_tp+=j;

            j=calcula_tp>>8;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            j=calcula_tp>>4;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            data_buff[3]=19;
            j=calcula_tp;
            j&=0x0f;
            data_buff[4]=Get_ascii_data(j);
            data_buff[5]=0;
            data_buff[6]='C';

            for(i=0; i<7; i++)
                Get_6x8_char_single(3,i*6+30,data_buff[i]);

            Disp_buff128x64[3*128+63]=0x04;
            Disp_buff128x64[3*128+64]=0x0a;
            Disp_buff128x64[3*128+65]=0x04;
        }
        else
        {
            if(M_menur1_LZDJ_select_temp==2)
            {
                data_buff[0]='0';
                data_buff[1]='0';
                data_buff[2]='0';
                data_buff[3]='0';
                data_buff[4]=19;
                data_buff[5]='0';
                data_buff[6]='0';
            }
            else	//read ise data
            {
                M_menur3_ISE_temp=0x0000;
                M_menur3_ISE_temp=U_data_buff[3];
                M_menur3_ISE_temp=M_menur3_ISE_temp<<8;
                M_menur3_ISE_temp+=U_data_buff[4];

                pb_temp=M_menur3_ISE_temp;
                if(M_menur3_ISE_temp<31250)
                {
                    data_buff[0]=14;		//-
                    pb_temp=31250-pb_temp;
                }
                else
                {
                    data_buff[0]=0;		//+
                    pb_temp=pb_temp-31250;
                }

                calcula_tp=kkk=0;
                j16=pb_temp/10000;
                j16&=0x000f;
                kkk=j16<<16;
                calcula_tp+=kkk;
                j16=pb_temp/1000;
                j16=j16%10;
                j16&=0x000f;
                kkk=j16<<12;
                calcula_tp+=kkk;
                j16=pb_temp/100;
                j16=j16%10;
                j16&=0x000f;
                kkk=j16<<8;
                calcula_tp+=kkk;
                j16=pb_temp/10;
                j16=j16%10;
                j16&=0x000f;
                kkk=j16<<4;

                calcula_tp+=kkk;
                j=pb_temp%10;
                j&=0x0f;
                calcula_tp+=j;
//            M_menur3_ISE_temp=calcula_tp;

                j=calcula_tp>>16;
                j&=0x0f;
                data_buff[1]=Get_ascii_data(j);
                j=calcula_tp>>12;
                j&=0x0f;
                data_buff[2]=Get_ascii_data(j);
                j=calcula_tp>>8;
                j&=0x0f;
                data_buff[3]=Get_ascii_data(j);
                data_buff[4]=19;
                j=calcula_tp>>4;
                j&=0x0f;
                data_buff[5]=Get_ascii_data(j);
                j=calcula_tp;
                j&=0x0f;
                data_buff[6]=Get_ascii_data(j);
            }
            data_buff[7]=0;
            data_buff[8]='M';
            data_buff[9]='V';

            for(j=0; j<10; j++)
                Get_6x8_char_single(3,30+j*6,data_buff[j]);
        }
        Pro_wait_time_set=150;	//wait 2sec
        Pro_wait_time_flag=1;

        Display_all_buff();
    }
}

//===============================================================================
//	finction	:Disp_pwm_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-18 10:38
//	modefy		:null
//===============================================================================
void Disp_pwm_data(uint8_t data_buff[])			//显示PWM菜单
{
    uint8_t i,j,k;
    uint8_t const *Adds_index;
    uint8_t const *index;

    Clear_n_page_ram(0,7);
    Pwm_select_cnt=1;
    switch(Pwm_menu_select1)
    {
    case 1:
    case 2:
/////          i=7;
        Pwm_menu_bit_temp=2;
        Pwm_select_cnt=1;
        if(M_menur1_language_select_temp==2)
        {
            data_buff[0]=205;
            data_buff[1]=133;
            data_buff[2]=206;
            data_buff[3]=133;
            for(i=0; i<2; i++)
            {
                Get_16x16_char_single(3,20+12*i,data_buff[i]);
                Get_16x16_char_single(3,36+20+12*i,data_buff[i+2]);
            }
            Get_8x16_char_single(3,25+20,11);

            Seg_rev_lenth=25;
            Seg_adds_bak=Pwm_menu_PoSeg=19;
            for(i=0; i<Seg_rev_lenth; i++)
            {
                Disp_buff128x64[128*3+Pwm_menu_PoSeg+i]=~Disp_buff128x64[128*3+Pwm_menu_PoSeg+i];
                Disp_buff128x64[128*4+Pwm_menu_PoSeg+i]=~Disp_buff128x64[128*4+Pwm_menu_PoSeg+i];
            }
            --Seg_rev_lenth;
        }
        //else	//20140216
        //{
        //    for(j=0;j<3;j++)
        //        Get_6x8_char_single(3,j*6+30,data_buff[j]);
        //
        //    for(i=0;i<7;i++)
        //    {
        //        Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
        //        Disp_buff128x64[128*4+29+i]|=0x01;
        //    }
        //    Seg_adds_bak=Pwm_menu_PoSeg=29;
        //}
        break;
    case 3:
/////          i=5;
        Pwm_menu_bit_temp=4;
        for(j=0; j<5; j++)
            Get_6x8_char_single(3,j*6+30,data_buff[j]);

        for(i=0; i<7; i++)
        {
            Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
            Disp_buff128x64[128*4+29+i]|=0x01;
        }
        Seg_adds_bak=Pwm_menu_PoSeg=29;
        break;
    case 4:
/////          i=8;
        Pwm_menu_bit_temp=7;
        for(j=0; j<8; j++)
            Get_6x8_char_single(3,j*6+30,data_buff[j]);

        for(i=0; i<7; i++)
        {
            Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
            Disp_buff128x64[128*4+29+i]|=0x01;
        }
        Seg_adds_bak=Pwm_menu_PoSeg=29;
        break;
    case 5:
/////          i=6;
        Pwm_menu_bit_temp=3;
        for(j=0; j<6; j++)
            Get_6x8_char_single(3,j*6+30,data_buff[j]);

        for(i=0; i<7; i++)
        {
            Disp_buff128x64[128*3+29+i]=~Disp_buff128x64[128*3+29+i];
            Disp_buff128x64[128*4+29+i]|=0x01;
        }
        Seg_adds_bak=Pwm_menu_PoSeg=29;
        break;
    }
/////   for(j=0;j<i;j++)
/////       Get_6x8_char_single(3,j*6+30,data_buff[j]);
    if(M_menur1_language_select_temp==2)
    {
        i=Pwm_menu_select1-1;

        Adds_index=Lever4_menu62+i*7;
        index= Lever4_menu62_index+i;
        for(k=0; k<*index; k++)
        {
            Get_16x16_char_single(0,k*12+6,*Adds_index);
            ++Adds_index;
        }
    }
    else
    {
        j=(Pwm_menu_select1-1)*16;
        index= Lever4_menu62_en_index+Pwm_menu_select1-1;		//char num.
        for(k=0; k<*index; k++)
        {
            Adds_index=Lever4_menu62_en_char+j;				//char data
            Get_6x8_char_single(0,k*6+6,*Adds_index);
            ++j;
        }
    }
    for(j=0; j<128; j++)
    {
        if(M_menur1_language_select_temp==2)
            Disp_buff128x64[128+j]|=0x80;
        else
            Disp_buff128x64[128+j]=0x10;
    }
    Display_all_buff();
}
//===============================================================================
//	finction	:Get_pwm_reg_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-18 10:38
//	modefy		:null
//===============================================================================
uint32_t Get_pwm_reg_data(void)		//转换设置的PWM值
{
    uint32_t i=0;

    switch(Menu_lever3_select)
    {
    case 1:
        switch(Pwm_menu_select)
        {
        case 1:
            switch(Pwm_menu_select1)
            {
            case 3:
                i=M_lever4_Out1_4ma_temp;
                break;
            case 4:
                i=M_lever4_Out1_20ma_temp;
                break;
            case 5:
                i=M_lever4_Out1_test_ma_temp;
                break;
            }
            break;
        case 2:
            switch(Pwm_menu_select1)
            {
            case 3:
                i=M_lever4_tp_Out1_4ma_temp;
                break;
            case 4:
                i=M_lever4_tp_Out1_20ma_temp;
                break;
            case 5:
                i=M_lever4_tp_Out1_test_ma_temp;
                break;
            }
            break;
        case 3:
            switch(Pwm_menu_select1)
            {
            case 3:
                i=M_lever4_jz_Out1_4ma_temp;
                break;
            case 4:
                i=M_lever4_jz_Out1_20ma_temp;
                break;
            case 5:
                i=M_lever4_jz_Out1_test_ma_temp;
                break;
            }
            break;
        }
        break;
    case 2:
        switch(Pwm_menu_select)
        {
        case 1:
            switch(Pwm_menu_select1)
            {
            case 3:
                i=M_lever4_Out2_4ma_temp;
                break;
            case 4:
                i=M_lever4_Out2_20ma_temp;
                break;
            case 5:
                i=M_lever4_Out2_test_ma_temp;
                break;
            }
            break;
        case 2:
            switch(Pwm_menu_select1)
            {
            case 3:
                i=M_lever4_tp_Out2_4ma_temp;
                break;
            case 4:
                i=M_lever4_tp_Out2_20ma_temp;
                break;
            case 5:
                i=M_lever4_tp_Out2_test_ma_temp;
                break;
            }
            break;
        case 3:
            switch(Pwm_menu_select1)
            {
            case 3:
                i=M_lever4_jz_Out2_4ma_temp;
                break;
            case 4:
                i=M_lever4_jz_Out2_20ma_temp;
                break;
            case 5:
                i=M_lever4_jz_Out2_test_ma_temp;
                break;
            }
            break;
        }
        break;
    }
    return(i);
}

//===============================================================================
//	finction	:Disp_pwm_menu
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-18 10:38
//	modefy		:null
//===============================================================================
void Disp_pwm_menu(void)		//PWM3级显示菜单控制
{
    uint8_t i,j,k,Line,data_buff[16];
    uint8_t const *Adds_index;
    uint8_t const *index;
    uint16_t m;
    uint32_t pub_32_data_temp,o_temp;

    switch(Pwm_menu_lever)
    {
    case 1:
        Clear_n_page_ram(0,7);
        i=0;
        if(M_menur1_language_select_temp==2)
        {
            for(Line=0; Line<3; Line++)
            {
                Adds_index=Lever4_menu621+i*7;
                index= Lever4_menu621_index+i;
                for(k=0; k<*index; k++)
                {
                    Get_16x16_char_single(Line*2,k*12+6,*Adds_index);
                    ++Adds_index;
                }
                Get_8x16_char_single(Line*2,118,'Z'+1);
                ++i;
            }
            for(m=0; m<256; m++)
                Disp_buff128x64[(Pwm_menu_select-1)*256+m]=~Disp_buff128x64[(Pwm_menu_select-1)*256+m];
        }
        //else	//20140216
        //{
        //   for(Line=2;Line<5;Line++)
        //   {
        //      j=(Line-2)*16;
        //      index= Lever4_menu621_en_index+Line;					//char num.
        //      for(k=0;k<*index;k++)
        //      {
        //          Adds_index=Lever4_menu621_en_char+j;				//char data
        //          Get_6x8_char_single(Line,6+k*6,*Adds_index);
        //          ++j;
        //      }
        //      Get_6x8_char_single(Line,122,0x5e);
        //   }
        //   for(k=0;k<8;k++)
        //   {
        //       Adds_index=Lever362_en_menu_char+(Menu_lever3_select-1)*16+k;	//disp menu
        //       Get_6x8_char_single(0,k*6+6,*Adds_index);
        //   }
        //   if(Pwm_menu_select==1)
        //      Get_6x8_char_single(0,122,0x5c);
        //   else if(Pwm_menu_select==3)
        //      Get_6x8_char_single(0,122,0x5b);
        //   else
        //      Get_6x8_char_single(0,122,0x5d);
        //   for(j=0;j<128;j++)
        //   {
        //       Disp_buff128x64[(Pwm_menu_select+1)*128+j]=~Disp_buff128x64[(Pwm_menu_select+1)*128+j];
        //       Disp_buff128x64[128+j]=0x10;
        //   }
        //}
        Display_all_buff();
        break;
    case 2:
        i=0;
        if(Pwm_menu_select1<=4)
        {
            k=Pwm_menu_select1;

            if(Pwm_menu_select1==1)
            {
                if(Menu_lever3_select==1)
                    PWMA->CMR0=adj_4ma_pwm_data;
                else if(Menu_lever3_select==2)
                    PWMA->CMR1=adj_4ma_pwm1_data;
            }
            else if(Pwm_menu_select1==2)
            {
                if(Menu_lever3_select==1)
                    PWMA->CMR0=adj_20ma_pwm_data;
                else if(Menu_lever3_select==2)
                    PWMA->CMR1=adj_20ma_pwm1_data;
            }
        }
        else
        {
////             i=1;
            k=4;
        }
        Clear_n_page_ram(0,7);
        if(M_menur1_language_select_temp==2)
        {
            if(Pwm_menu_select1<=4) i=0;
            else                    i=1;
            for(Line=0; Line<4; Line++)
            {
                Adds_index=Lever4_menu62+i*7;
                index= Lever4_menu62_index+i;
                for(k=0; k<*index; k++)
                {
                    Get_16x16_char_single(Line*2,k*12+6,*Adds_index);
                    ++Adds_index;
                }
                Get_8x16_char_single(Line*2,118,'Z'+1);
                ++i;
            }
            if(Pwm_menu_select1<=4) i=Pwm_menu_select1;
            else                    i=4;
            for(m=0; m<256; m++)
                Disp_buff128x64[(i-1)*256+m]=~Disp_buff128x64[(i-1)*256+m];
        }
        //else	//20140216
        //{
        //   for(Line=0;Line<5;Line++)
        //   {
        //      j=i*16;
        //      index= Lever4_menu62_en_index+i;				//char num.
        //      for(k=0;k<*index;k++)
        //      {
        //          Adds_index=Lever4_menu62_en_char+j;				//char data
        //          Get_6x8_char_single(Line+2,6+k*6,*Adds_index);
        //          ++j;
        //      }
        //      Get_6x8_char_single(Line+2,122,0x5e);
        //      ++i;
        //   }
        //   for(k=0;k<6;k++)
        //   {
        //       Adds_index=Lever4_menu621_en_char+(Pwm_menu_select-1)*16+k;	//disp menu
        //       Get_6x8_char_single(0,k*6+6,*Adds_index);
        //   }
        //   if(Pwm_menu_select1==1)
        //      Get_6x8_char_single(0,122,0x5c);
        //   else if(Pwm_menu_select1==5)
        //      Get_6x8_char_single(0,122,0x5b);
        //   else
        //      Get_6x8_char_single(0,122,0x5d);
        //   for(j=0;j<128;j++)
        //   {
        //       Disp_buff128x64[(Pwm_menu_select1+1)*128+j]=~Disp_buff128x64[(Pwm_menu_select1+1)*128+j];
        //       Disp_buff128x64[128+j]=0x10;
        //   }
        //}
        Display_all_buff();
        break;
    case 3:
        pub_32_data_temp=Get_pwm_reg_data();
        Pwm_adj_cnt=0;

        data_buff[0]=13;			//+
        j=pub_32_data_temp>>28;
        j&=0x80;
        if(j==0x80)
            data_buff[0]=14;			//-

/////          if(Menu_lever3_select==1)
/////          {
        switch(Pwm_menu_select1)
        {
        case 1:
        case 2:
///    	     	     data_buff[0]='U';
///    	     	     data_buff[1]='P';
///    	     	     data_buff[2]=18;
///    	     	     data_buff[3]='D';
///    	     	     data_buff[4]='O';
///    	     	     data_buff[5]='W';
///    	     	     data_buff[6]='N';

            data_buff[0]=13;
            data_buff[1]=18;
            data_buff[2]=14;
            break;
        case 3:
            j=pub_32_data_temp>>8;
            j&=0x0f;
            Pwm_adj_cnt=j;
            data_buff[1]=Get_ascii_data(j);
            j=pub_32_data_temp>>4;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            data_buff[3]=19;
            j=pub_32_data_temp;
            j&=0x0f;
            data_buff[4]=Get_ascii_data(j);
            Pwm_out_adj_flag=1;
            break;
        case 4:
            j=pub_32_data_temp>>20;
            j&=0x0f;
            Pwm_adj_cnt=j;
            data_buff[1]=Get_ascii_data(j);
            j=pub_32_data_temp>>16;
            j&=0x0f;
            data_buff[2]=Get_ascii_data(j);
            j=pub_32_data_temp>>12;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            j=pub_32_data_temp>>8;
            j&=0x0f;
            data_buff[4]=Get_ascii_data(j);
            data_buff[5]=19;
            j=pub_32_data_temp>>4;
            j&=0x0f;
            data_buff[6]=Get_ascii_data(j);
            j=pub_32_data_temp;
            j&=0x0f;
            data_buff[7]=Get_ascii_data(j);
            Pwm_out_adj_flag=1;
            break;
        case 5:
            j=pub_32_data_temp>>8;
            j&=0x0f;
            Pwm_adj_cnt=j;
            data_buff[0]=Get_ascii_data(j);
            j=pub_32_data_temp>>4;
            j&=0x0f;
            data_buff[1]=Get_ascii_data(j);
            data_buff[2]=19;
            j=pub_32_data_temp;
            j&=0x0f;
            data_buff[3]=Get_ascii_data(j);
            data_buff[4]='M';
            data_buff[5]='A';
            o_temp=pub_32_data_temp<<4;
            Pwm_output_test(Menu_lever3_select,o_temp);
            break;
        }
        Disp_pwm_data(data_buff);
        break;
    }
}

//===============================================================================
//	finction	:Pwm_select_finc
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-18 10:38
//	modefy		:null
//===============================================================================
void Pwm_select_finc(void)
{
////   uint8_t m,n;
    uint32_t pub_32_data_temp;

    pub_32_data_temp=Get_pwm_reg_data();
    switch(Pwm_menu_select1)
    {
    case 1:
    case 2:
        switch(Pwm_select_cnt)
        {
        case 1:
/////                 Pwm_menu_PoSeg_pub=Pwm_menu_PoSeg;
            if(M_menur1_language_select_temp==2)
                Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,19,24);
            else
                Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,6,3,Pwm_menu_PoSeg,6);
            break;
        case 2:
/////                 Pwm_menu_PoSeg_pub=Pwm_menu_PoSeg+32;
            if(M_menur1_language_select_temp==2)
                Seg_adds_bak=rev_clear_set_arear16(3,Seg_adds_bak,Seg_rev_lenth,3,55,24);
            else
                Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,6,3,Pwm_menu_PoSeg+12,6);
            break;
        }
        break;
    case 3:
        switch(Pwm_select_cnt)
        {
        case 1:
/////                 Pwm_menu_PoSeg_pub=Pwm_menu_PoSeg;
            Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,6,3,Pwm_menu_PoSeg,6);
            break;
        case 2:
/////                 Pwm_menu_PoSeg_pub=Pwm_menu_PoSeg+8;
            Pwm_adj_cnt=pub_32_data_temp>>8;
            Pwm_adj_cnt&=0x0f;
            Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,6,3,Pwm_menu_PoSeg+6,6);
            break;
        case 3:
/////                 Pwm_menu_PoSeg_pub=Pwm_menu_PoSeg+16;
            Pwm_adj_cnt=pub_32_data_temp>>4;
            Pwm_adj_cnt&=0x0f;
            Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,6,3,Pwm_menu_PoSeg+12,6);
            break;
        case 4:
/////                 Pwm_menu_PoSeg_pub=Pwm_menu_PoSeg+30;
            Pwm_adj_cnt=pub_32_data_temp;
            Pwm_adj_cnt&=0x0f;
            Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,6,3,Pwm_menu_PoSeg+24,6);
            break;
        }
        break;
    case 4:
        switch(Pwm_select_cnt)
        {
        case 1:
/////                 Pwm_menu_PoSeg_pub=Pwm_menu_PoSeg;
            Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,6,3,Pwm_menu_PoSeg,6);
            break;
        case 2:
/////                 Pwm_menu_PoSeg_pub=Pwm_menu_PoSeg+8;
            Pwm_adj_cnt=pub_32_data_temp>>20;
            Pwm_adj_cnt&=0x0f;
            Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,6,3,Pwm_menu_PoSeg+6,6);
            break;
        case 3:
/////                 Pwm_menu_PoSeg_pub=Pwm_menu_PoSeg+16;
            Pwm_adj_cnt=pub_32_data_temp>>16;
            Pwm_adj_cnt&=0x0f;
            Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,6,3,Pwm_menu_PoSeg+12,6);
            break;
        case 4:
/////                 Pwm_menu_PoSeg_pub=Pwm_menu_PoSeg+24;
            Pwm_adj_cnt=pub_32_data_temp>>12;
            Pwm_adj_cnt&=0x0f;
            Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,6,3,Pwm_menu_PoSeg+18,6);
            break;
        case 5:
/////                 Pwm_menu_PoSeg_pub=Pwm_menu_PoSeg+32;
            Pwm_adj_cnt=pub_32_data_temp>>8;
            Pwm_adj_cnt&=0x0f;
            Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,6,3,Pwm_menu_PoSeg+24,6);
            break;
        case 6:
/////                 Pwm_menu_PoSeg_pub=Pwm_menu_PoSeg+48;
            Pwm_adj_cnt=pub_32_data_temp>>4;
            Pwm_adj_cnt&=0x0f;
            Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,6,3,Pwm_menu_PoSeg+36,6);
            break;
        case 7:
/////                 Pwm_menu_PoSeg_pub=Pwm_menu_PoSeg+56;
            Pwm_adj_cnt=pub_32_data_temp;
            Pwm_adj_cnt&=0x0f;
            Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,6,3,Pwm_menu_PoSeg+42,6);
            break;
        }
        break;
    case 5:
        switch(Pwm_select_cnt)
        {
        case 1:
/////                 Pwm_menu_PoSeg_pub=Pwm_menu_PoSeg;
            Pwm_adj_cnt=pub_32_data_temp>>8;
            Pwm_adj_cnt&=0x0f;
            Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,6,3,Pwm_menu_PoSeg,6);
            break;
        case 2:
/////                 Pwm_menu_PoSeg_pub=Pwm_menu_PoSeg+8;
            Pwm_adj_cnt=pub_32_data_temp>>4;
            Pwm_adj_cnt&=0x0f;
            Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,6,3,Pwm_menu_PoSeg+6,6);
            break;
        case 3:
/////                 Pwm_menu_PoSeg_pub=Pwm_menu_PoSeg+24;
            Pwm_adj_cnt=pub_32_data_temp;
            Pwm_adj_cnt&=0x0f;
            Seg_adds_bak=rev_clear_set_arear(3,Seg_adds_bak,6,3,Pwm_menu_PoSeg+18,6);
            break;
        }
        break;
    }
/////   Clear_apage(4);
/////   m=Pwm_menu_PoSeg_pub%16;
/////   n=Pwm_menu_PoSeg_pub/16;
/////   Disp_8x8_area(4,n,m,TB_select_dot);
}

//===============================================================================
//	finction	:switch_pwm_reg_data3
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-19 10:14
//	modefy		:null
//===============================================================================
uint32_t switch_pwm_reg_data3(uint32_t data_tp)
{
    uint32_t i,data;
    data=data_tp;

    if(((Pwm_menu_select1==3)||(Pwm_menu_select1==4))&&((Key_code_bak==KEY_CODE_DOWN)&&(Pwm_select_cnt==1)))
    {
        data&=~0x80000000;
/////        Display_6x8_char_single(3,Pwm_menu_PoSeg,13);
        Get_6x8_char_single(3,Pwm_menu_PoSeg+1,13);
        for(i=0; i<6; i++)
            Disp_buff128x64[128*3+Pwm_menu_PoSeg+i+1]=~Disp_buff128x64[128*3+Pwm_menu_PoSeg+i+1];

        Display_all_buff();
    }
    else if(((Pwm_menu_select1==3)||(Pwm_menu_select1==4))&&((Key_code_bak==KEY_CODE_UP)&&(Pwm_select_cnt==1)))
    {
        data|=0x80000000;
/////        Display_6x8_char_single(3,Pwm_menu_PoSeg,14);
        Get_6x8_char_single(3,Pwm_menu_PoSeg+1,14);
        for(i=0; i<6; i++)
            Disp_buff128x64[128*3+Pwm_menu_PoSeg+i+1]=~Disp_buff128x64[128*3+Pwm_menu_PoSeg+i+1];

        Display_all_buff();
    }
    else if(Pwm_menu_select1==5)
    {
        switch(Pwm_select_cnt)
        {
        case 1:
            i=0;
            i=Pwm_adj_cnt;
            i<<=8;
            data&=0xf00000ff;
            data+=i;
            break;
        case 2:
            i=0;
            i=Pwm_adj_cnt;
            i<<=4;
            data&=0xf0000f0f;
            data+=i;
            break;
        case 3:
            i=0;
            i=Pwm_adj_cnt;
            data&=0xf0000ff0;
            data+=i;
            break;
        }
    }
    else
    {
        switch(Pwm_select_cnt)
        {
        case 2:
            i=0;
            i=Pwm_adj_cnt;
            i<<=8;
            data&=0xf00000ff;
            data+=i;
            break;
        case 3:
            i=0;
            i=Pwm_adj_cnt;
            i<<=4;
            data&=0xf0000f0f;
            data+=i;
            break;
        case 4:
            i=0;
            i=Pwm_adj_cnt;
            data&=0xf0000ff0;
            data+=i;
            break;
        }
    }
    return(data);
}

//===============================================================================
//	finction	:switch_pwm_reg_data6
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-19 10:14
//	modefy		:null
//===============================================================================
uint32_t switch_pwm_reg_data6(uint32_t data_tp)
{
    uint32_t i,data;
    data=data_tp;

    if(((Pwm_menu_select1==3)||(Pwm_menu_select1==4))&&((Key_code_bak==KEY_CODE_DOWN)&&(Pwm_select_cnt==1)))
    {
        data&=~0x80000000;
/////        Display_6x8_char_single(3,Pwm_menu_PoSeg,13);
        Get_6x8_char_single(3,Pwm_menu_PoSeg+1,13);
        for(i=0; i<6; i++)
            Disp_buff128x64[128*3+Pwm_menu_PoSeg+i+1]=~Disp_buff128x64[128*3+Pwm_menu_PoSeg+i+1];

        Display_all_buff();
    }
    else if(((Pwm_menu_select1==3)||(Pwm_menu_select1==4))&&((Key_code_bak==KEY_CODE_UP)&&(Pwm_select_cnt==1)))
    {
        data|=0x80000000;
/////        Display_6x8_char_single(3,Pwm_menu_PoSeg,14);
        Get_6x8_char_single(3,Pwm_menu_PoSeg+1,14);
        for(i=0; i<6; i++)
            Disp_buff128x64[128*3+Pwm_menu_PoSeg+i+1]=~Disp_buff128x64[128*3+Pwm_menu_PoSeg+i+1];

        Display_all_buff();
    }
    else
    {
        switch(Pwm_select_cnt)
        {
        case 2:
            i=0;
            i=Pwm_adj_cnt;
            i<<=20;
            data&=0xf00fffff;
            data+=i;
            break;
        case 3:
            i=0;
            i=Pwm_adj_cnt;
            i<<=16;
            data&=0xf0f0ffff;
            data+=i;
            break;
        case 4:
            i=0;
            i=Pwm_adj_cnt;
            i<<=12;
            data&=0xf0ff0fff;
            data+=i;
            break;
        case 5:
            i=0;
            i=Pwm_adj_cnt;
            i<<=8;
            data&=0xf0fff0ff;
            data+=i;
            break;
        case 6:
            i=0;
            i=Pwm_adj_cnt;
            i<<=4;
            data&=0xf0ffff0f;
            data+=i;
            break;
        case 7:
            i=0;
            i=Pwm_adj_cnt;
            data&=0xf0fffff0;
            data+=i;
            break;
        }
    }
    return(data);
}

//===============================================================================
//	finction	:Adj_pwm_reg_save_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-19 10:14
//	modefy		:null
//===============================================================================
void Adj_pwm_reg_save_data(void)
{
    uint32_t o_temp;

    switch(Menu_lever3_select)
    {
    case 1:
        switch(Pwm_menu_select)
        {
        case 1:
            switch(Pwm_menu_select1)
            {
            case 3:
                M_lever4_Out1_4ma_temp=switch_pwm_reg_data3(M_lever4_Out1_4ma_temp);
                Pwm_out_adj_flag=1;
                break;
            case 4:
                M_lever4_Out1_20ma_temp=switch_pwm_reg_data6(M_lever4_Out1_20ma_temp);
                Pwm_out_adj_flag=1;
                break;
            case 5:
                M_lever4_Out1_test_ma_temp=switch_pwm_reg_data3(M_lever4_Out1_test_ma_temp);

                if(M_lever4_Out1_test_ma_temp<0x00000040)
                    M_lever4_Out1_test_ma_temp=0x00000040;
                else if(M_lever4_Out1_test_ma_temp>0x00000200)
                    M_lever4_Out1_test_ma_temp=0x00000200;

                o_temp=M_lever4_Out1_test_ma_temp<<4;
                Pwm_output_test(Menu_lever3_select,o_temp);
                break;
            }
            break;
        case 2:
            switch(Pwm_menu_select1)
            {
            case 3:
                M_lever4_tp_Out1_4ma_temp=switch_pwm_reg_data3(M_lever4_tp_Out1_4ma_temp);
                Pwm_out_adj_flag=1;
                break;
            case 4:
                M_lever4_tp_Out1_20ma_temp=switch_pwm_reg_data6(M_lever4_tp_Out1_20ma_temp);
                Pwm_out_adj_flag=1;
                break;
            case 5:
                M_lever4_tp_Out1_test_ma_temp=switch_pwm_reg_data3(M_lever4_tp_Out1_test_ma_temp);

                if(M_lever4_tp_Out1_test_ma_temp<0x00000040)
                    M_lever4_tp_Out1_test_ma_temp=0x00000040;
                else if(M_lever4_tp_Out1_test_ma_temp>0x00000200)
                    M_lever4_tp_Out1_test_ma_temp=0x00000200;

                o_temp=M_lever4_Out1_test_ma_temp<<4;
                Pwm_output_test(Menu_lever3_select,o_temp);
                break;
            }
            break;
        case 3:
            switch(Pwm_menu_select1)
            {
            case 3:
                M_lever4_jz_Out1_4ma_temp=switch_pwm_reg_data3(M_lever4_jz_Out1_4ma_temp);
                Pwm_out_adj_flag=1;
                break;
            case 4:
                M_lever4_jz_Out1_20ma_temp=switch_pwm_reg_data6(M_lever4_jz_Out1_20ma_temp);
                Pwm_out_adj_flag=1;
                break;
            case 5:
                M_lever4_jz_Out1_test_ma_temp=switch_pwm_reg_data3(M_lever4_jz_Out1_test_ma_temp);

                if(M_lever4_jz_Out1_test_ma_temp<0x00000040)
                    M_lever4_jz_Out1_test_ma_temp=0x00000040;
                else if(M_lever4_jz_Out1_test_ma_temp>0x00000200)
                    M_lever4_jz_Out1_test_ma_temp=0x00000200;

                o_temp=M_lever4_Out1_test_ma_temp<<4;
                Pwm_output_test(Menu_lever3_select,o_temp);
                break;
            }
            break;
        }
        break;
    case 2:
        switch(Pwm_menu_select)
        {
        case 1:
            switch(Pwm_menu_select1)
            {
            case 3:
                M_lever4_Out2_4ma_temp=switch_pwm_reg_data3(M_lever4_Out2_4ma_temp);
                Pwm_out_adj_flag=1;
                break;
            case 4:
                M_lever4_Out2_20ma_temp=switch_pwm_reg_data6(M_lever4_Out2_20ma_temp);
                Pwm_out_adj_flag=1;
                break;
            case 5:
                M_lever4_Out2_test_ma_temp=switch_pwm_reg_data3(M_lever4_Out2_test_ma_temp);

                if(M_lever4_Out2_test_ma_temp<0x00000040)
                    M_lever4_Out2_test_ma_temp=0x00000040;
                else if(M_lever4_Out2_test_ma_temp>0x00000200)
                    M_lever4_Out2_test_ma_temp=0x00000200;

                o_temp=M_lever4_Out1_test_ma_temp<<4;
                Pwm_output_test(Menu_lever3_select,o_temp);
                break;
            }
            break;
        case 2:
            switch(Pwm_menu_select1)
            {
            case 3:
                M_lever4_tp_Out2_4ma_temp=switch_pwm_reg_data3(M_lever4_tp_Out2_4ma_temp);
                Pwm_out_adj_flag=1;
                break;
            case 4:
                M_lever4_tp_Out2_20ma_temp=switch_pwm_reg_data6(M_lever4_tp_Out2_20ma_temp);
                Pwm_out_adj_flag=1;
                break;
            case 5:
                M_lever4_tp_Out2_test_ma_temp=switch_pwm_reg_data3(M_lever4_tp_Out2_test_ma_temp);

                if(M_lever4_tp_Out2_test_ma_temp<0x00000040)
                    M_lever4_tp_Out2_test_ma_temp=0x00000040;
                else if(M_lever4_tp_Out2_test_ma_temp>0x00000200)
                    M_lever4_tp_Out2_test_ma_temp=0x00000200;

                o_temp=M_lever4_Out1_test_ma_temp<<4;
                Pwm_output_test(Menu_lever3_select,o_temp);
                break;
            }
            break;
        case 3:
            switch(Pwm_menu_select1)
            {
            case 3:
                M_lever4_jz_Out2_4ma_temp=switch_pwm_reg_data3(M_lever4_jz_Out2_4ma_temp);
                Pwm_out_adj_flag=1;
                break;
            case 4:
                M_lever4_jz_Out2_20ma_temp=switch_pwm_reg_data6(M_lever4_jz_Out2_20ma_temp);
                Pwm_out_adj_flag=1;
                break;
            case 5:
                M_lever4_jz_Out2_test_ma_temp=switch_pwm_reg_data3(M_lever4_jz_Out2_test_ma_temp);

                if(M_lever4_jz_Out2_test_ma_temp<0x00000040)
                    M_lever4_jz_Out2_test_ma_temp=0x00000040;
                else if(M_lever4_jz_Out2_test_ma_temp>0x00000200)
                    M_lever4_jz_Out2_test_ma_temp=0x00000200;

                o_temp=M_lever4_Out1_test_ma_temp<<4;
                Pwm_output_test(Menu_lever3_select,o_temp);
                break;
            }
            break;
        }
        break;
    }
}

//===============================================================================
//	finction	:Pwm_adj_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-19 9:33
//	modefy		:null
//===============================================================================
void Pwm_adj_data(void)
{
    uint8_t i;

    if(Pwm_menu_select1>=3)
    {
//       if((Pwm_menu_select1==5)&&(Pwm_adj_cnt<4))
//          Pwm_adj_cnt=9;

        i=Get_ascii_data(Pwm_adj_cnt);
        Get_6x8_char_single(3,Seg_adds_bak+1,i);
        for(i=0; i<6; i++)
/////       {
            Disp_buff128x64[128*3+Seg_adds_bak+i+1]=~Disp_buff128x64[128*3+Seg_adds_bak+i+1];
/////           Disp_buff128x64[128*(3+1)+Pwm_menu_PoSeg+i+1]|=0x01;
/////       }
        Display_all_buff();

        Adj_pwm_reg_save_data();
        Write_system_reg_to_chip();
    }
    else if(Pwm_menu_select1==1)  		//4ma adj
    {
        if(Menu_lever3_select==1)		//cn0
        {
            PWMA->PCR.CH0EN=0;
            if(Pwm_select_cnt==1)		//up
            {
                adj_4ma_pwm_data+=1;
                if(adj_4ma_pwm_data>=5000)
                    adj_4ma_pwm_data=4999;
                PWMA->CMR0=adj_4ma_pwm_data;
            }
            else					//down
            {
                adj_4ma_pwm_data-=1;
                if(adj_4ma_pwm_data<=1)
                    adj_4ma_pwm_data=1;
                PWMA->CMR0=adj_4ma_pwm_data;
            }
            PWMA->PCR.CH0EN=1;
        }
        else if(Menu_lever3_select==2)		//cn1
        {
            PWMA->PCR.CH1EN=0;
            if(Pwm_select_cnt==1)		//up
            {
                adj_4ma_pwm1_data+=1;
                if(adj_4ma_pwm1_data>=5000)
                    adj_4ma_pwm1_data=4999;
                PWMA->CMR1=adj_4ma_pwm1_data;
            }
            else					//down
            {
                adj_4ma_pwm1_data-=1;
                if(adj_4ma_pwm1_data<=1)
                    adj_4ma_pwm1_data=1;
                PWMA->CMR1=adj_4ma_pwm1_data;
            }
            PWMA->PCR.CH1EN=1;
        }
        Write_system_reg_to_chip();
    }
    else if(Pwm_menu_select1==2)   		//20ma adj
    {
        if(Menu_lever3_select==1)		//cn0
        {
            PWMA->PCR.CH0EN=0;
            if(Pwm_select_cnt==1)		//up
            {
                adj_20ma_pwm_data+=1;
                if(adj_20ma_pwm_data>=5000)
                    adj_20ma_pwm_data=4999;
                PWMA->CMR0=adj_20ma_pwm_data;
            }
            else					//down
            {
                adj_20ma_pwm_data-=1;
                if(adj_20ma_pwm_data<=1)
                    adj_20ma_pwm_data=1;
                PWMA->CMR0=adj_20ma_pwm_data;
            }
            PWMA->PCR.CH0EN=1;
        }
        else if(Menu_lever3_select==2)		//cn1
        {
            PWMA->PCR.CH1EN=0;
            if(Pwm_select_cnt==1)		//up
            {
                adj_20ma_pwm1_data+=1;
                if(adj_20ma_pwm1_data>=5000)
                    adj_20ma_pwm1_data=4999;
                PWMA->CMR1=adj_20ma_pwm1_data;
            }
            else					//down
            {
                adj_20ma_pwm1_data-=1;
                if(adj_20ma_pwm1_data<=1)
                    adj_20ma_pwm1_data=1;
                PWMA->CMR1=adj_20ma_pwm1_data;
            }
            PWMA->PCR.CH1EN=1;
        }
        Write_system_reg_to_chip();
    }
}

//===============================================================================
//	finction	:INPUT_PASS_WORD_DISP
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-19 9:33
//	modefy		:null
//===============================================================================
void Input_pass_word_disp(void)		//显示输入密码界面
{
    uint8_t i,data_buff[12];

    Clear_n_page_ram(0,7);
    if(M_menur1_language_select_temp==2)
    {
        data_buff[0]=30;
        data_buff[1]=31;
        data_buff[2]=98;
        data_buff[3]=99;
        for(i=0; i<4; i++)
            Get_16x16_char_single(1,i*12+6,data_buff[i]);

        Get_8x16_char_single(1,54,12);
        Display_all_buff();
    }
    else
    {
        data_buff[0]='P';
        data_buff[1]='A';
        data_buff[2]='S';
        data_buff[3]='S';
        data_buff[4]='W';
        data_buff[5]='O';
        data_buff[6]='R';
        data_buff[7]='D';
        data_buff[8]=12;
        data_buff[9]=0;

        for(i=0; i<10; i++)
            Get_8x16_char_single(1,i*10,data_buff[i]);

        Display_all_buff();
/////    	Display_8x16_char(1,0,10,data_buff);
    }
//----------------------------------------------------------------- line 1
    data_buff[0]='0';
    data_buff[1]=13;
    data_buff[2]=13;
    data_buff[3]=13;
    data_buff[4]=13;
    data_buff[5]=13;
    Display_8x16_char(3,40,6,data_buff);
//----------------------------------------------------------------- line 2
    Check_pass_word_flag=1;		//clear by POWER key or 5minu time up
    Key_input_pass_temp=0;
    Pass_bit_select=1;
    Pass_adj_cnt=0;
}

//===============================================================================
//	finction	:Pass_select_disp
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-19 9:33
//	modefy		:null
//===============================================================================
void Pass_select_disp(uint8_t bit_select)
{
    uint8_t data_buff[12];

    data_buff[0]=13;
    data_buff[1]=13;
    data_buff[2]=13;
    data_buff[3]=13;
    data_buff[4]=13;
    data_buff[5]=13;

    data_buff[bit_select-1]='0';
    Display_8x16_char(3,40,6,data_buff);
    Pass_adj_cnt=0;
}

//===============================================================================
//	finction	:Pass_adj_disp
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-19 9:33
//	modefy		:null
//===============================================================================
void Pass_adj_disp(uint8_t Adj_data,uint8_t bit_select)
{
    uint8_t seg_adds;
    uint32_t data_temp;

    data_temp=0;
    data_temp+=Adj_data;

    Adj_data=Get_ascii_data(Adj_data);
    seg_adds=bit_select*10+30;
    Display_8x16_char_single(3,seg_adds,Adj_data);

    switch(bit_select)
    {
    case 1:
        data_temp<<=20;
        Key_input_pass_temp&=0x000fffff;
        break;
    case 2:
        data_temp<<=16;
        Key_input_pass_temp&=0x00f0ffff;
        break;
    case 3:
        data_temp<<=12;
        Key_input_pass_temp&=0x00ff0fff;
        break;
    case 4:
        data_temp<<=8;
        Key_input_pass_temp&=0x00fff0ff;
        break;
    case 5:
        data_temp<<=4;
        Key_input_pass_temp&=0x00ffff0f;
        break;
    case 6:
        Key_input_pass_temp&=0x00fffff0;
        break;
    }
    Key_input_pass_temp+=data_temp;
}

//===============================================================================
//	finction	:Clead_pass_word_reg_data
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-19 9:33
//	modefy		:null
//===============================================================================
void Clead_pass_word_reg_data(void)
{
    Key_input_pass_temp=0;
    Check_pass_word_flag=0;		//clear by ESC key or 5minu time up
    Key_input_pass_temp=0;
    Pass_bit_select=0;
    Pass_adj_cnt=0;

    if(Key_code_bak==KEY_CODE_POWER)
        User_prio_temp=1;			//reset user prio
}

//===============================================================================
//	finction	:Check_prio_finc
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-9-19 9:33
//	modefy		:null
//===============================================================================
uint8_t Check_prio_finc(void)			//检查密码用户的优先级
{
    uint8_t return_data=0;

    if(((Menu_lever_select==1)&&(Menu_index_select==3))||((Menu_lever_select==1)&&(Menu_index_select==2)))
    {
        if(Check_pass_word_flag!=1)
        {
            if(User_prio_temp<=1)
            {
                Input_pass_word_disp();
                return_data=1;
            }
            else
                return_data=0;
        }
        else
        {
            Check_pass_word_flag=0;

            Key_input_pass_temp&=0x00FFFFFF;		     //2013-1-9 17:40

            if(Key_input_pass_temp==USER_PASS_WORD)           //check pass word
            {
                return_data=1;
                User_prio_temp=1;
                Display_menu_index();
            }
            else if(Key_input_pass_temp==CTRL_PASS_WORD)
            {
                return_data=0;
                User_prio_temp=2;
            }
            else if(Key_input_pass_temp==ENG_PASS_WORD)
            {
                return_data=0;
                User_prio_temp=3;
            }
            else if(Key_input_pass_temp==SUPER_PASS_WORD)
                ////else if(Key_input_pass_temp==0x00100000)
            {
                return_data=0;
                User_prio_temp=4;
            }
            else
            {
                return_data=1;
                User_prio_temp=1;
                Display_menu_index();
                Clead_pass_word_reg_data();
            }
        }
    }
//    if((Menu_lever_select==2)&&(Menu_index_select==7)&&(Menu_index_finc_select==4))
//    {
//    	if(Check_pass_word_flag!=1)
//    	{
//    	   if(User_prio_temp<=3)
//    	   {
//    	      Input_pass_word_disp();
//    	      return_data=1;
//    	   }
//    	   else
//    	      return_data=0;
//        }
//        else
//        {
//           Check_pass_word_flag=0;
//           if(Key_input_pass_temp==USER_PASS_WORD)           //check pass word
//           {
//               return_data=1;
//               User_prio_temp=1;
//               Display_menu_index();
//           }
//           else if(Key_input_pass_temp==CTRL_PASS_WORD)
//           {
//               return_data=1;
//               User_prio_temp=2;
//               Display_menu_index();
//           }
//           else if(Key_input_pass_temp==ENG_PASS_WORD)
//           {
//               return_data=1;
//               User_prio_temp=3;
//               Display_menu_index();
//           }
//           else if(Key_input_pass_temp==SUPER_PASS_WORD)
//           {
//               return_data=0;
//               User_prio_temp=4;
//           }
//           else
//           {
//               return_data=1;
//               User_prio_temp=1;
//               Display_menu_index();
//               Clead_pass_word_reg_data();
//           }
//        }
//    }
    if((Menu_lever_select==2)&&(Menu_index_select==7)&&(Menu_index_finc_select==9))		//备份
    {
        if(Check_pass_word_flag!=1)
        {
            if(User_prio_temp<=2)
            {
                Input_pass_word_disp();
                return_data=1;
            }
            else
                return_data=0;
        }
        else
        {
            Check_pass_word_flag=0;
            if(Key_input_pass_temp==USER_PASS_WORD)           //check pass word
            {
                return_data=1;
                User_prio_temp=1;
                Display_menu_index();
            }
            else if(Key_input_pass_temp==CTRL_PASS_WORD)
            {
                return_data=1;
                User_prio_temp=2;
                Display_menu_index();
            }
            else if(Key_input_pass_temp==ENG_PASS_WORD)
            {
                return_data=0;
                User_prio_temp=3;
            }
            else if(Key_input_pass_temp==SUPER_PASS_WORD)
            {
                return_data=0;
                User_prio_temp=4;
            }
            else
            {
                return_data=1;
                User_prio_temp=1;
                Display_menu_index();
                Clead_pass_word_reg_data();
            }
        }
    }
    if((Menu_lever_select==2)&&(Menu_index_select==7)&&(Menu_index_finc_select==10))	//还原
    {
        if(Check_pass_word_flag!=1)
        {
            if(User_prio_temp<=2)
            {
                Input_pass_word_disp();
                return_data=1;
            }
            else
                return_data=0;
        }
        else
        {
            Check_pass_word_flag=0;
            if(Key_input_pass_temp==USER_PASS_WORD)           //check pass word
            {
                return_data=1;
                User_prio_temp=1;
                Display_menu_index();
            }
            else if(Key_input_pass_temp==CTRL_PASS_WORD)
            {
                return_data=1;
                User_prio_temp=2;
                Display_menu_index();
            }
            else if(Key_input_pass_temp==ENG_PASS_WORD)
            {
                return_data=0;
                User_prio_temp=3;
            }
            else if(Key_input_pass_temp==SUPER_PASS_WORD)
            {
                return_data=0;
                User_prio_temp=4;
            }
            else
            {
                return_data=1;
                User_prio_temp=1;
                Display_menu_index();
                Clead_pass_word_reg_data();
            }
        }
    }
    if((Menu_lever_select==3)&&(Menu_index_select==5))
    {
        if(Men_del_flag==0) return_data=1;
        else if(Men_del_flag==1)
        {
            if(Check_pass_word_flag!=1)
            {
                if(User_prio_temp<=1)
                {
                    Input_pass_word_disp();
                    return_data=1;
                }
                else
                {
                    if(Men_del_flag==1)
                        Menu_lever3_4_enter_finction();
                    else
                        Display_menu_index();

                    return_data=1;
                }
            }
            else
            {
                Check_pass_word_flag=0;
                if(Key_input_pass_temp==USER_PASS_WORD)           //check pass word
                {
                    return_data=1;
                    User_prio_temp=1;

                    Display_menu_index();

                    Clead_pass_word_reg_data();
                }
                else if(Key_input_pass_temp==CTRL_PASS_WORD)
                {
////                  return_data=0;
                    User_prio_temp=2;
                }
                else if(Key_input_pass_temp==ENG_PASS_WORD)
                {
////                  return_data=0;
                    User_prio_temp=3;
                }
                else if(Key_input_pass_temp==SUPER_PASS_WORD)
                {
////                  return_data=0;
                    User_prio_temp=4;
                }
                else
                {
                    return_data=1;
                    User_prio_temp=1;

                    Display_menu_index();

                    Clead_pass_word_reg_data();
                }
                if(User_prio_temp>1)
                {
                    return_data=1;

                    if(Men_del_flag==1)
                        Menu_lever3_4_enter_finction();
                    else
                        Display_menu_index();
                }
            }
        }
    }
    return(return_data);
}


