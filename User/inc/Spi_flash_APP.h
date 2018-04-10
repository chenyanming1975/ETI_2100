//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Spi_flash_APP.h	:AT45DB321D
//	First EDIT		: Sam
//	Data			: 2012.06.14
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================
//	Status bit
#define 	PSIZE 					0
#define 	PROTEC 					1
#define 	FLAG 					2
#define 	COMP 					6
#define 	BUSY 					7 
//--------------------------------------------------------------------------------------------
#define		FX_CHIP_START_ADDS			 0x00040000	//0004
#define		FX_CHIP_END_ADDS			 0x08030000	//2051		//2051-4=2047
#define		JZ_CHIP_START_ADDS			 0x08040000	//2052
#define		JZ_CHIP_END_ADDS			 0x10030000	//4099
#define		CW_CHIP_START_ADDS			 0x10040000	//4100
#define		CW_CHIP_END_ADDS			 0x18030000	//6147
#define		BJ_CHIP_START_ADDS			 0x18040000	//6148
#define		BJ_CHIP_END_ADDS			 0x1F9A0000	//8090		//0x1FFE0000	//8190 2012-10-24 9:35  2013-3-14 17:04


#define		DATA_HZ_START_ADDS			 0x1F9B0000	//8091		//2013-3-14 17:06
#define		DATA_HZ_END_ADDS			 0x1FB90000	//8121	30PAGE	//2013-3-14 17:06 write flag 5555AAAA
#define		DATA_6X8_ADDS			 	 0x1FBA0000	//8122	2PAGE	//2013-3-14 17:06
#define		DATA_8X16_ADDS			 	 0x1FBC0000	//8124	4PAGE	//2013-3-14 17:06
#define		CHIP_END_ADDS			 	 0x1FFE0000	//8190		//2013-3-14 17:06

#define		BACK_SYSTEM_REG_ADDS			 2
#define		FIRST_TIME_RW_FLAG			 0XCCCCCCCC
#define		FIRST_REG_PAGE			 	 0
#define		SYSTEM_REG_PAGE			 	 1
//--------------------------------------------------------------------------------------------
		 
#define		M_dongtai_flag_ADDS               	 0	
#define		M_menur1_lcd_Contrast_temp_ADDS          1
#define		M_menur1_language_select_temp_ADDS       2
#define		M_menur1_analy_obj_select_temp_ADDS	 3
#define		M_menur1_unit_select_temp_ADDS           4
#define		M_menur1_dot_num_select_temp_ADDS        5
#define		M_menur1_ZDXS_select_temp_ADDS           6
#define		M_menur1_LZDJ_select_temp_ADDS	         7
#define		M_menur1_DL_select_temp_ADDS	         8
#define		M_menur1_direction_select_temp_ADDS	 9

#define		M_menur1_led_data_temp_ADDS              10

#define		M_menur1_lange_data_temp_ADDS            12
#define		M_menur1_led_dianliu_temp_ADDS           16
#define		M_menur1_JB_speed_temp_ADDS		 20
#define		M_menur1_ZSB_speed_temp_ADDS             24	                                                 
#define		M_menur1_sample_v_temp_ADDS              28	                                                 
#define		M_menur1_slope_temp_ADDS 		 32
#define		M_menur1_jz1nd_temp_ADDS                 36 
#define		M_menur1_jz1_v_temp_ADDS                 40 
#define		M_menur1_jz2nd_temp_ADDS                 44 
#define		M_menur1_jz2_v_temp_ADDS             	 48 	                                
#define		M_menur1_sj1_v_temp_ADDS                 52 
#define		M_menur1_sj2_v_temp_ADDS                 56 
#define		M_menur1_sj3_v_temp_ADDS             	 60                                 
#define		M_menur1_FY_temp_ADDS                    64 
#define		M_menur1_Fx_temp_ADDS                    68 
#define		M_menur1_Fa_temp_ADDS                    72 
#define		M_menur1_Fb_temp_ADDS                    76 
#define		M_menur1_Fc_temp_ADDS                    80 

#define		M_menur1_RX_time_temp_ADDS		 84
#define		M_menur1_QYSJ_temp_ADDS   		 85
#define		M_menur1_CSSJ_temp_ADDS   		 89
#define		M_menur1_XHPY_temp_ADDS   		 93
#define		M_menur1_DDSJ_temp_ADDS   		 97

#define		M_lever4_FXPL_temp_ADDS  		 101   		                                	 
#define		M_lever4_Out1_4ma_temp_ADDS          	 105
#define		M_lever4_Out1_20ma_temp_ADDS         	 109
#define		M_lever4_Out1_test_ma_temp_ADDS      	 113
#define		M_lever4_Out2_4ma_temp_ADDS          	 117
#define		M_lever4_Out2_20ma_temp_ADDS         	 121
#define		M_lever4_Out2_test_ma_temp_ADDS      	 125
#define		M_lever4_extern_trrig_flag_ADDS      	 129      
	                                        	 
#define		M_lever3_BYHC_temp_ADDS			 130	//1=yes 2=no
#define		M_lever3_HCPD_temp_ADDS	        	 134 
#define		a_signle_flag_ADDS                   	 138
#define		b_signle_flag_ADDS                   	 142
#define		c_signle_flag_ADDS	                 146
#define		M_lever4_JZPL_temp_ADDS			 150
#define		M_lever4_QXPL_temp_ADDS			 154
#define		M_menur1_sj4_v_temp_ADDS             	 158                                 

#define		M_lever4_tp_Out1_4ma_temp_ADDS           162
#define		M_lever4_tp_Out1_20ma_temp_ADDS          166
#define		M_lever4_tp_Out1_test_ma_temp_ADDS       170
#define		M_lever4_tp_Out2_4ma_temp_ADDS           174
#define		M_lever4_tp_Out2_20ma_temp_ADDS          178
#define		M_lever4_tp_Out2_test_ma_temp_ADDS       182

#define		M_lever4_jz_Out1_4ma_temp_ADDS           186
#define		M_lever4_jz_Out1_20ma_temp_ADDS          190
#define		M_lever4_jz_Out1_test_ma_temp_ADDS       194
#define		M_lever4_jz_Out2_4ma_temp_ADDS           198
#define		M_lever4_jz_Out2_20ma_temp_ADDS          202
#define		M_lever4_jz_Out2_test_ma_temp_ADDS       206

#define		M_menur1_Fd_temp_ADDS                    210 
#define		d_signle_flag_ADDS	                 214
#define		M_lever4_HCPL_temp_ADDS	                 218
#define		M_menur1_slope_flag_ADDS	         222
#define		Updata_slope_flag_ADDS	        	 223
#define		Allow_error_ADDS	        	 224
#define		Alarm_low_nd_ADDS	        	 228
#define		Alarm_hight_nd_ADDS	        	 232


//------------------------------------------LENTH
#define		M_dongtai_flag_LENT               	 1
#define		M_menur1_lcd_Contrast_temp_LENT          1
#define		M_menur1_language_select_temp_LENT       1
#define		M_menur1_analy_obj_select_temp_LENT	 1
#define		M_menur1_unit_select_temp_LENT           1
#define		M_menur1_dot_num_select_temp_LENT        1
#define		M_menur1_ZDXS_select_temp_LENT           1
#define		M_menur1_LZDJ_select_temp_LENT	         1
#define		M_menur1_DL_select_temp_LENT	         1
#define		M_menur1_direction_select_temp_LENT 	 1
                                                         
#define		M_menur1_led_data_temp_LENT              2
                                                       
#define		Allow_error_LENT            	 	 4
#define		Alarm_low_nd_LENT	        	 4
#define		Alarm_hight_nd_LENT	        	 4

#define		M_menur1_lange_data_temp_LENT            4
#define		M_menur1_led_dianliu_temp_LENT           4
#define		M_menur1_JB_speed_temp_LENT		 4
#define		M_menur1_ZSB_speed_temp_LENT             4	                                                 
#define		M_menur1_sample_v_temp_LENT              4	
                                                 
#define		M_menur1_slope_temp_LENT 		 4
#define		M_menur1_slope_flag_LENT	         1
#define		Updata_slope_flag_LENT	         	 1

#define		M_menur1_jz1nd_temp_LENT                 4 
#define		M_menur1_jz1_v_temp_LENT                 4 
#define		M_menur1_jz2nd_temp_LENT                 4 
#define		M_menur1_jz2_v_temp_LENT             	 4 	                                
#define		M_menur1_sj1_v_temp_LENT                 4 
#define		M_menur1_sj2_v_temp_LENT                 4 
#define		M_menur1_sj3_v_temp_LENT             	 4                                 
#define		M_menur1_sj4_v_temp_LENT             	 4                                 
#define		M_menur1_FY_temp_LENT                    4 
#define		M_menur1_Fx_temp_LENT                    4 
#define		M_menur1_Fa_temp_LENT                    4 
#define		M_menur1_Fb_temp_LENT                    4 
#define		M_menur1_Fc_temp_LENT                    4 
#define		M_menur1_Fd_temp_LENT                    4 

#define		M_menur1_RX_time_temp_LENT		 1
#define		M_menur1_QYSJ_temp_LENT   		 4
#define		M_menur1_CSSJ_temp_LENT   		 4
#define		M_menur1_XHPY_temp_LENT   		 4
#define		M_menur1_DDSJ_temp_LENT   		 4

#define		M_lever4_FXPL_temp_LENT			 4   		                                	 
#define		M_lever4_Out1_4ma_temp_LENT          	 4
#define		M_lever4_Out1_20ma_temp_LENT         	 4
#define		M_lever4_Out1_test_ma_temp_LENT      	 4
#define		M_lever4_Out2_4ma_temp_LENT          	 4
#define		M_lever4_Out2_20ma_temp_LENT         	 4
#define		M_lever4_Out2_test_ma_temp_LENT      	 4
#define		M_lever4_extern_trrig_flag_LENT      	 1      
	                                        	 
#define		M_lever3_BYHC_temp_LENT			 1	//1=yes 2=no
#define		M_lever3_HCPD_temp_LENT	        	 4 
#define		a_signle_flag_LENT                   	 4
#define		b_signle_flag_LENT                   	 4
#define		c_signle_flag_LENT                	 4
#define		d_signle_flag_LENT                	 4
#define		M_lever4_JZPL_temp_LENT			 4
#define		M_lever4_QXPL_temp_LENT			 4
#define		M_lever4_HCPL_temp_LENT	                 4

#define		M_lever4_tp_Out1_4ma_temp_LENT           4
#define		M_lever4_tp_Out1_20ma_temp_LENT          4
#define		M_lever4_tp_Out1_test_ma_temp_LENT       4
#define		M_lever4_tp_Out2_4ma_temp_LENT           4
#define		M_lever4_tp_Out2_20ma_temp_LENT          4
#define		M_lever4_tp_Out2_test_ma_temp_LENT       4

#define		M_lever4_jz_Out1_4ma_temp_LENT           4
#define		M_lever4_jz_Out1_20ma_temp_LENT          4
#define		M_lever4_jz_Out1_test_ma_temp_LENT       4
#define		M_lever4_jz_Out2_4ma_temp_LENT           4
#define		M_lever4_jz_Out2_20ma_temp_LENT          4
#define		M_lever4_jz_Out2_test_ma_temp_LENT       4
                                                          
//--------------------------------------------------------------------------------------------
#ifdef __SPI_FLASH_APP__                                  
                                                          
#else                    
extern uint16_t  export_tx_lenth;
extern uint16_t  export_tx_lenth_cnt;
extern uint8_t  export_tx_flag;
extern uint8_t  export_data_select;      
extern uint32_t FX_export_index_adds_point;
extern uint32_t JZ_export_index_adds_point;
extern uint32_t BJ_export_index_adds_point;
extern uint32_t CW_export_index_adds_point;
                           
extern uint8_t	pub_Adds_h;                               
extern uint8_t	pub_Adds_m;                               
extern uint8_t	pub_Adds_l;                               
extern uint8_t	pub_buff_lenth;

extern uint8_t	buff_page[512];
extern uint8_t	buff_page_w[512];
extern uint8_t  buff_page_exp_tx[512];

extern uint8_t	Buff_68[512];			//2013-3-15 17:32
extern uint8_t	Buff_816[1024];

extern uint8_t disp_sec_screen_flag;

extern uint8_t Men_del_flag;

extern uint32_t FX_write_index_adds_point;
extern uint32_t FX_read_index_adds_point;
extern uint32_t JZ_write_index_adds_point;
extern uint32_t JZ_read_index_adds_point;
extern uint32_t BJ_write_index_adds_point;
extern uint32_t BJ_read_index_adds_point;
extern uint32_t CW_write_index_adds_point;
extern uint32_t CW_read_index_adds_point;

extern struct Save_data{
                 uint8_t  xuhao_h;
                 uint8_t  xuhao_l;
                 uint8_t  status;
                 uint8_t  year_h;
                 uint8_t  year_l;
                 uint8_t  munth;
                 uint8_t  day;
                 uint8_t  hour;
                 uint8_t  minu;
                 uint8_t  result_ph;
                 uint8_t  result_pl;
                 uint8_t  result_nh;
                 uint8_t  result_nl;
                 uint8_t  unit_alarm_wrong;
                 uint8_t  yes_or_no;
                 uint8_t  signe_result;   		//½á¹û·ûºÅ    
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
		}Fenxi,Jiaozheng,Cuowu,Baojing;

extern uint8_t	Wait_busy_time(void);
extern void 	read_device_id(void);
extern void 	Adj_adds_block_erase(uint32_t buff_lenth,uint8_t mode);
extern void 	Adj_adds_sector_erase(uint32_t buff_lenth,uint8_t mode);

extern uint32_t Adj_adds_C00000_800000_page(uint32_t page,uint8_t mode);
extern uint32_t Adj_adds_FFFC00_FFFE00(uint32_t buff_lenth,uint8_t mode);
extern uint32_t Memory_read_adds(uint32_t buff_adds,uint32_t page,uint8_t mode);
extern uint8_t  Spi_buff_read_one_byte512(uint8_t CMD_set,uint32_t Adds);
extern void 	Page_erase_free(uint32_t Adds);
extern void 	Spi_buff_write_one_byte512(uint8_t CMD_set,uint32_t Adds,uint8_t data);
extern void 	Spi_buff_write_data(uint8_t CMD_set,uint32_t Adds,uint8_t buff_lenth,uint8_t *data_adds);
extern void 	Spi_buff_write_to_memory(uint8_t CMD_set,uint32_t Adds);
extern void	Spi_memory_page_read(uint8_t CMD_set,uint32_t Adds,uint16_t lenth);
extern void	Exp_spi_memory_page_read(uint8_t CMD_set,uint32_t Adds,uint16_t lenth);
extern void 	Spi_write_memory_through_buff(uint8_t CMD_set,uint32_t Adds,uint16_t buff_lenth,uint8_t *data_adds);

//extern void 	Write_result_to_flash(uint32_t adds_temp,struct Save_data pub_save);
extern void 	Write_index_adds_point(void);
extern void 	Del_mem_data(uint8_t select);
extern uint32_t	Write_result_to_flash(uint32_t adds_temp,struct Save_data *point);
extern void 	Get_write_struct_data(uint8_t selset,uint8_t status,uint32_t Sresult,uint8_t unit,uint8_t YN,uint8_t signe);
extern void 	Read_result_from_flash(uint8_t select);
extern void 	Read_result_from_flash_index_up(uint8_t select);
extern void     Disp_history_data(uint8_t select,struct Save_data *point);
extern void 	Disp_sec_screen_data(struct Save_data *point);

//extern void 	Eare_allmem_data(uint32_t adds_st);
extern void 	Export__flash_data_finc(void);

extern void	Back_system_reg(void);
extern void	Load_bak_system_reg(void);

extern void	SPI_flash_init(void);
extern void	Spi_read(uint32_t page_address,uint8_t *buf);
extern void	Spi_write(uint32_t page_address,uint8_t *buf);

extern uint8_t  Rd_check_lcd_data(void);
extern void 	Check_updta_lcd_data(void);
extern void 	Rd_init_lcd_data(void);

extern void 	Write_error_status(void);		//2013-7-12 10:10


#endif

