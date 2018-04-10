//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: SD_APP.h
//	First EDIT		: Sam
//	Data			: 2012-11-23 15:51
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================

#ifdef	__SD_H__
#define	CRC7_BASE	137

//---------------------------------------------------------
//	Command
#define CMD0 	(0x40+0)	//GO_IDLE_STATE
#define CMD1 	(0x40+1)	//SEND_OP_COND (MMC)
#define	ACMD41	(0x40+41)	//SEND_OP_COND (SDC)
#define CMD8	(0x40+8)	//SEND_IF_COND
#define CMD9	(0x40+9)	//READ_CSD
#define CMD16	(0x40+16)	//SET_BLOCKLEN
#define CMD17	(0x40+17)	//READ_SINGLE_BLOCK
#define CMD24	(0x40+24)	//WRITE_SIGNAL_BLOCK
#define CMD29	(0x40+29)	//CLEAR WRITE PROTECT
#define CMD55	(0x40+55)	//APP_CMD
#define CMD58	(0x40+58)	//READ_OCR

//	Card type flags
#define CT_NULL 		0
#define CT_SD1 			0x01		//SD V1
#define CT_SD2_SC 		0x02		//SD V2 low cap
#define CT_SD2_HC 		0x04		//SD V2 high cap

//	Delay const
#define DELAY_TIME 		50
#define DELAY_LTIME 		5000		//620000
                        	        	
#define LONG_FILE_NAME_FLAG	0X7E31		//AT OFFSET 06,07
#define SECTOR_SIZE		512		//Sector size
#define CAT_FILE_SIZE		32		//FILE ROOT SIZE
#define EXT_NAME_OFFSET		0X08		//FILE EXT NAME OFFSET
#define FILE_ATTRIB_OFFSET	0X0B		//
#define FILE_CREAT_OFFSET	0X0E		//
#define FILE_MODEFY_OFFSET	0X10		//
#define FILE_VISIT_OFFSET	0X12		//
#define FILE_LENTH_OFFSET	0X1C		//
#define FILE_CLUT_H_OFFSET	0X14		//
#define FILE_CLUT_L_OFFSET	0X1A		//

#define FILE_ATTRIB_RW		0X00		//
#define FILE_ATTRIB_RR		0X01		//
#define FILE_ATTRIB_HH		0X02		//
#define FILE_ATTRIB_SS		0X04		//
#define FILE_ATTRIB_JB		0X08		//
#define FILE_ATTRIB_GD		0X20		//
#define FILE_ATTRIB_ZZ		0X10		//

#define EOF_CLUT_FLAG		0X0FFFFFFF	//
#define BAD_CLUT_FLAG		0XF7FFFF0F	//
#define BIAO_XIANG_FLAG		0XFFFFFF0F	//
#define BIAO_XIANG_FLAG1	0XFFFFFFFF	//
#define FIRST_CLUT_FLAG		0X0FFFFFF8	//
#define OTHER_FLAG1		0X7FFFFFFF	//

//---------------------------------------------------------
volatile uint8_t card_type;
uint8_t		Sd_buff[512];
uint8_t		SD_data_buff[512];
uint8_t		Sd_io_init_flag;
uint8_t		GB_read_sec_cnt;
uint8_t		SD_read_flag;
uint32_t	GB_read_adds_temp;
//---------------------------------------------------------
void sd_send_byte(uint8_t data);
void Sd_get_BPB_start_adds_data(void);
void Get_free_root_adds(void);
void Get_free_fat1_tab_adds(void);
void Get_file_free_cluster_adds(void);
void Write_file_fat_tab_dir_info(uint32_t fat_data);

uint8_t sd_receive_byte(void);
uint8_t sd_send_cmd(uint8_t cmd,uint32_t arg);
uint8_t Sd_search_file(uint32_t adds,uint32_t file_type,uint32_t fn_p1,uint32_t fn_p2);

#else
extern uint8_t	GB_read_sec_cnt;
extern uint8_t	SD_read_flag;
extern uint8_t	Sd_buff[512];
extern uint8_t	SD_data_buff[512];
extern uint8_t	Sd_io_init_flag;
extern uint32_t	fat1_start_adds;
extern uint32_t	fat2_start_adds;
extern uint32_t	root_start_adds;
extern uint32_t read_file_lenth_cnt;    
extern uint32_t	GB_read_adds_temp;
extern uint8_t  Sys_BPB;

extern uint8_t  SD_CRC7(uint8_t * chr, uint16_t cnt);
extern uint16_t SD_CRC16(uint8_t * puchMsg,uint16_t usDataLen );
extern uint8_t  sd_init(void);
extern uint8_t  sd_write(uint32_t page_address,uint8_t *buf);
extern uint8_t  sd_read(uint32_t page_address,uint8_t *buf);
extern void 	Sd_io_initial(void);

//---------------------------------------------------------
extern void Sd_get_BPB_start_adds_data(void);
//extern void Sd_get_root_sector_data(uint32_t adds);
extern uint8_t Sd_search_file(uint32_t adds,uint32_t file_type,uint32_t fn_p1,uint32_t fn_p2);
extern uint8_t Sd_read_file(uint32_t file_type,uint32_t fn_p1,uint32_t fn_p2);
extern uint8_t Updata_current_cluster_num_adds_next_cluster_adds(void);
extern void    Sd_creat_file(void);
extern void    Get_free_root_adds(void);

#endif

