//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: SD_APP.c
//	First EDIT		: Sam
//	Data			: 2012-11-23 15:51
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================
#define	__SD_H__
//--------------------include files------------------------
#include "Incude_all.h"

//typedef struct
//{
//    uint8_t	bios_drive;
//    uint8_t	unused;
//    uint8_t	ext_boot_signature;
//    uint32_t	volume_serial_number;
//    uint8_t 	volume_label[10];
//    uint8_t 	fs[5];
//}SD_SYS_INFO;

//-----------------------------------------
//typedef struct
//{
//    uint8_t	bsjump[3];		//3
//    uint8_t	bsOemName[8];		//8
//    SD_BPB 	bpb;			//25
//    SD_SYS_INFO	fs;			//22
//    uint8_t	code[428];
//    uint8_t	signature[2];
//}SD_DBR;

//-----------------------------------------
typedef struct
{
    uint16_t	bytes_per_sector;
    uint8_t 	sectors_per_cluster;
    uint16_t	reserved_sectors;
    uint8_t 	number_of_fats;
    uint16_t	sectors_per_fats;
    uint32_t	root_entries;
    uint16_t	fat_type;
    uint32_t	total_sector_num;
//    uint16_t	sectors_on_small;
//    uint8_t 	media_descriptor;
//    uint16_t	sectors_per_track;
//    uint16_t	heads;
//    uint32_t	hidden_sectors;
//    uint32_t	sectors_on_large;
} SD_BPB;

SD_BPB		Sys_BPB;

volatile uint32_t boot_sector_adds;

uint32_t	fat1_start_adds;
uint32_t	fat2_start_adds;
uint32_t	root_start_adds;
uint32_t    	read_file_lenth_cnt;

typedef struct
{
    uint32_t	file_current_cluster_num;
    uint32_t	file_next_cluster_num;
    uint32_t	file_current_cluster_adds;
    uint32_t	file_next_cluster_adds;
    uint32_t	file_type_name;
    uint32_t	file_root_name;
    uint32_t    file_name_part1;
    uint32_t    file_name_part2;
    uint8_t     file_attrib;
    uint16_t    file_creat_time;
    uint16_t    file_modefy_time;
    uint16_t    file_visit_time;
    uint32_t    file_lenth;

    uint32_t    root_index_adds;
    uint32_t    fat_index_adds;
    uint32_t    root_free_write_adds;
    uint32_t    fat1_free_write_adds;
    uint32_t    file_free_write_cluster_adds;
    uint16_t    file_num;
    uint16_t    catalog_num;
    uint32_t	next_root_cluster_adds;
} FILE_INFO;

FILE_INFO	Index;

typedef struct
{
    uint32_t	file_type_name;
    uint32_t    file_name_part1;
    uint32_t    file_name_part2;
    uint8_t     file_attrib;
    uint8_t     file_creat_time_ms;
    uint16_t    file_creat_time;
    uint16_t    file_modefy_time;
    uint16_t	file_creat_date;
    uint16_t    file_modefy_date;
    uint16_t    file_visit_date;
    uint32_t    file_lenth;
} FILE_CREAK;

FILE_CREAK	C_file_info;


//-------------------Pub Counst-- --------------------------
//============================================================================================
//	CRC HIGHT LOW TAB
//============================================================================================
///static uint8_t  auchCRCHi[] =
///{
///0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
///0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
///0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
///0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
///0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
///0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
///0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
///0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
///0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
///0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
///0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
///0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
///0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
///0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
///0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
///0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
///0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
///0x40
///} ;
///
///static uint8_t  auchCRCLo[] =
///{
///0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
///0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
///0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
///0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
///0x11, 0xD1, U_SENCER_MODU_ADDS, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
///0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
///0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
///0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
///0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
///0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
///0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
///0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
///0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
///0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
///0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
///0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
///0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
///0x40
///};

//===============================================================================
//	finction	:CRC7
//      detials         :Get SD CRC7 value
//	input		:chr - point to the Arrarybuff; cnt - number needed to workout
//	output		:null
//	return		:CRC value
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
uint8_t SD_CRC7(uint8_t * chr, uint16_t cnt)
{
    uint16_t i,a;
    uint8_t crc,Data;
    crc=0;
    for (a=0; a<cnt; a++)
    {
        Data=chr[a];
        for (i=0; i<8; i++)
        {
            crc <<= 1;
            if ((Data & 0x80)^(crc & 0x80))
                crc ^=0x09;
            Data <<= 1;
        }
    }
    crc=(crc<<1)|1;
    return(crc);
}

//===============================================================================
//	finction	:SD_CRC16
//      detials         :Get SD CRC16 value
//	input		:puchMsg - message to calculate CRC upon; usDataLen - quantity of bytes in message
//	output		:null
//	return		:CRC value
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
///uint16_t SD_CRC16(uint8_t * puchMsg,uint16_t usDataLen )
///{
///   uint8_t uchCRCHi = 0xFF ; 				/* high byte of CRC initialized */
///   uint8_t uchCRCLo = 0xFF ; 				/* low byte of CRC initialized */
///   uint16_t uIndex ; 					/* will index into CRC lookup table */
///   while (usDataLen--) 					/* pass through message buffer */
///   {
///       uIndex = uchCRCLo ^ *puchMsg++ ; 		/* calculate the CRC */
///       uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex] ;
///       uchCRCHi = auchCRCLo[uIndex] ;
///   }
///   return (uchCRCHi << 8 | uchCRCLo) ;
///}
///
//===============================================================================
//	finction	:sd_send_byte
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
void sd_send_byte(uint8_t data)
{
    SPI1->TX[0]=data;
    SPI1->CNTRL.GO_BUSY=1;				//start send
    while(SPI1->CNTRL.GO_BUSY==1);			//wait complete
    return;
}

//===============================================================================
//	finction	:sd_receive_byte
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
uint8_t sd_receive_byte(void)
{
    sd_send_byte(0xFF);
    return SPI1->RX[0];
}

//===============================================================================
//	finction	:csd_high
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
void csd_high(void)
{
    DrvGPIO_SetBit(E_GPD,7);		//CSD=hight
    DrvGPIO_SetBit(E_GPB,2);		//FLASH CSM=hight
}

//===============================================================================
//	finction	:csd_low
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
void csd_low(void)
{
    DrvGPIO_ClrBit(E_GPD,7);		//CSD=low
}

//===============================================================================
//	finction	:sd_send_cmd
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
uint8_t sd_send_cmd(uint8_t cmd,uint32_t arg)
{
    uint8_t tmp,result;

    //complete pre cmd
    csd_high();
    sd_receive_byte();
    DrvSYS_Delay(DELAY_LTIME);

    csd_low();
    sd_receive_byte();

    sd_send_byte(cmd);
    sd_send_byte((arg>>24)&0xFF);
    sd_send_byte((arg>>16)&0xFF);
    sd_send_byte((arg>>8)&0xFF);
    sd_send_byte(arg&0xFF);

    //send CRC7
    tmp=0x01;
    if(cmd==CMD0)tmp=0x95;
    if(cmd==CMD8)tmp=0x87;
    sd_send_byte(tmp);

    //wait Ncr (2~64 clock cycle)
    tmp=0;
    do {
        result=sd_receive_byte();
        if((result&0x80)==0)break;
        tmp++;
    } while(tmp<10);

    return result;
}

//===============================================================================
//	finction	:sd_init		初始化SD卡
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
uint8_t sd_init(void)
{
    uint8_t result;
    uint8_t tmp,rst;
    uint8_t ocr[4];
    uint32_t time;

    DrvGPIO_ClrBit(E_GPD,6);				//ON SD LED

//	SPI1->CNTRL.TX_NEG=1;
//	SPI1->CNTRL.RX_NEG=1;
//	SPI1->CNTRL.TX_BIT_LEN=8;
//	SPI1->CNTRL.TX_NUM=0;
//	SPI1->CNTRL.LSB=0;
//	SPI1->CNTRL.CLKP=0;
//	SPI1->DIVIDER.DIVIDER=2;			//7.5MHz clock

    SPI1->DIVIDER.DIVIDER=100;							//200KHz clock use for init
    //80 dummy clock with CS high and low freqency
    DrvSYS_Delay(2000000);
    csd_high();
    DrvSYS_Delay(2000);
    for (tmp=0; tmp<10; tmp++)sd_receive_byte();
    DrvSYS_Delay(2000);
    result=FALSE;
    card_type=CT_NULL;

    for(tmp=0; tmp<100; tmp++)				//reset SD 100 times
    {
        rst=sd_send_cmd(CMD0,0);
        if(rst==0x01) break;
        if(tmp==99) rst=0;
    }
    if(rst==0x01)						//reset SD
    {
        for(tmp=0; tmp<4; tmp++)
            ocr[tmp]=sd_receive_byte();

        if(sd_send_cmd(CMD8,0x1AA)==0x01)				//SD_V2
        {
            for(tmp=0; tmp<4; tmp++)
                ocr[tmp]=sd_receive_byte();

            //The card can work at vdd range of 2.7-3.6V
            if((ocr[2]==0x01)&&(ocr[3]==0xAA))
            {
                time=10000;
                do {
                    if(sd_send_cmd(CMD55,0)<=0x01)
                    {
                        if(sd_send_cmd(ACMD41,1<<30)==0)break;
                    }
                    DrvSYS_Delay(DELAY_LTIME);
                    time--;
                } while(time>0);

                //check CCS bit in the OCR
                if(time!=0)
                {
                    if(sd_send_cmd(CMD58,0)==0)
                    {
                        for(tmp=0; tmp<4; tmp++)
                            ocr[tmp]=sd_receive_byte();

                        //check SD_V2 cap
                        if((ocr[0]&0x40)==0x40)
                            card_type=CT_SD2_HC;
                        else card_type=CT_SD2_SC;

                        result=TRUE;
                    }
                }
            }
        }
        else 											//SD_V1
        {
            time=10000;
            do {
                if(sd_send_cmd(CMD55,0)<=0x01)
                {
                    if(sd_send_cmd(ACMD41,0)==0)break;
                }
                DrvSYS_Delay(DELAY_LTIME);
                time--;
            } while(time>0);

            if(time!=0)
            {
                card_type=CT_SD1;
                result=TRUE;
            }
        }
    }

    //Set R/W block length to 512 byte
    if((result==TRUE)&&(sd_send_cmd(CMD16,512)!=0))
    {
        card_type=CT_NULL;
        result=FALSE;
    }

    csd_high();
    sd_receive_byte();

    SPI1->DIVIDER.DIVIDER=2;								//7.5MHz clock
//	dbg_send_char(card_type);

    Sd_io_init_flag=1;
    DrvGPIO_SetBit(E_GPD,6);				//OFF SD LED
    return result;
}

//===============================================================================
//	finction	:sd_write  page(512byte)	扇区写SD卡
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
uint8_t sd_write(uint32_t page_address,uint8_t *buf)
{
    uint8_t result;
    uint16_t i;
    uint32_t time;

    DrvGPIO_ClrBit(E_GPD,6);				//ON SD LED

    result=FALSE;
    if(card_type!=CT_SD2_HC)page_address<<=9;

    if(sd_send_cmd(CMD24,page_address)==0)
    {
        //data block header and Nwr
        sd_send_byte(0xFF);
        sd_send_byte(0xFE);

        //512 byte data
        for(i=0; i<512; i++)
        {
            sd_send_byte(*(buf+i));
        }

        //2 byte CRC(0)
        for(i=0; i<2; i++)
        {
            sd_send_byte(0);
        }

        if((sd_receive_byte()&0x1F)==0x05)
        {
            //wait busy(DO become high level from low level,time out 500ms)
            time=100;
            do {
                if(sd_receive_byte()==0)break;
                DrvSYS_Delay(DELAY_LTIME);
                time--;
            } while(time>0);

            if(time!=0)
            {
                time=500;
                do {
                    if(sd_receive_byte()==0xFF)break;
                    DrvSYS_Delay(DELAY_LTIME);
                    time--;
                } while(time>0);

                if(time!=0)result=TRUE;
            }
        }
    }

    csd_high();
    sd_receive_byte();

    DrvGPIO_SetBit(E_GPD,6);				//OFF SD LED
    return result;
}

//===============================================================================
//	finction	:sd_read  page(512byte)		读SD卡1扇区
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
uint8_t sd_read(uint32_t page_address,uint8_t *buf)
{
    uint8_t result;
    uint8_t tmp;
    uint16_t i;
    uint32_t time;

    DrvGPIO_ClrBit(E_GPD,6);				//ON SD LED

    result=FALSE;
    if(card_type!=CT_SD2_HC)page_address<<=9;

    if(sd_send_cmd(CMD17,page_address)==0)
    {
        //wait for data packet(500ms)
        time=500;
        do {
            tmp=sd_receive_byte();
            if(tmp!=0xFF)break;
            DrvSYS_Delay(DELAY_LTIME);
            DrvSYS_Delay(DELAY_LTIME);
            time--;
        } while(time>0);

        //data packet token valid
        if((time!=0)&&(tmp==0xFE))
        {
            //receive 512 byte
            for(i=0; i<512; i++)
            {
                *(buf+i)=sd_receive_byte();
            }

            //skip 2 byte CRC
            for(i=0; i<2; i++)
            {
                sd_receive_byte();
            }

            result=TRUE;
        }
    }

    csd_high();
    sd_receive_byte();

    DrvGPIO_SetBit(E_GPD,6);				//OFF SD LED
    return result;
}

//===============================================================================
//	finction	:Sd_io_initial  	检查是否插入或拔出SD卡并初始化或显示错误
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
void Sd_io_initial(void)
{
    uint8_t result;

    result=0;
    if((DrvGPIO_GetBit(E_GPC,0))==0)//检测是否有插入SD卡
    {
        if(Sd_io_init_flag==0)
        {
            Set_en_buzz_finc(2,4,200,100);
            DrvSYS_Delay(4000000);				//delay 200ms
            result=sd_init();
            if(result==TRUE) 
							Sd_get_BPB_start_adds_data();
            else 
							Disp_updata_status(6);				//initial err
        }
    }
    else
    {
        Sd_io_init_flag=0;
    }
}

//=====================================================================================================================
//
//		SD FILE SYSTEM		FAT32
//
//
//=====================================================================================================================

//===============================================================================
//	finction	:Sd_get_BPB_start_adds_data  读SD卡的引导等信息
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
void Sd_get_BPB_start_adds_data(void)
{
    volatile uint32_t adr_temp;

    sd_read(0x00000000,Sd_buff);
    adr_temp=((Sd_buff[0x0003]<<24)|(Sd_buff[0x0004]<<16)|(Sd_buff[0x0005]<<8)|Sd_buff[0x0006]);
    if(adr_temp==0x00000000)		//2G SD CAR
    {
        adr_temp=((Sd_buff[0x01c7]<<8)|Sd_buff[0x01c6]);
        sd_read(adr_temp,Sd_buff);
    }
    else
    {
        adr_temp=0x00000000;
    }

    Sys_BPB.bytes_per_sector=((Sd_buff[0x000c]<<8)|Sd_buff[0x000b]);
    Sys_BPB.sectors_per_cluster=Sd_buff[0x000d];
    Sys_BPB.reserved_sectors=((Sd_buff[0x000f]<<8)|Sd_buff[0x000e]);
    Sys_BPB.number_of_fats=Sd_buff[0x0010];
    Sys_BPB.sectors_per_fats=((Sd_buff[0x0025]<<8)|Sd_buff[0x0024]);
    Sys_BPB.root_entries=((Sd_buff[0x002F]<<24)|(Sd_buff[0x002E]<<16)|(Sd_buff[0x002D]<<8)|Sd_buff[0x002C]);
    Sys_BPB.fat_type=((Sd_buff[0x0055]<<8)|Sd_buff[0x0056]);
    Sys_BPB.total_sector_num=((Sd_buff[0x0023]<<24)|(Sd_buff[0x0022]<<16)|(Sd_buff[0x0021]<<8)|Sd_buff[0x0020]);

    boot_sector_adds=adr_temp*(Sys_BPB.bytes_per_sector);

    fat1_start_adds=(Sys_BPB.reserved_sectors)*(Sys_BPB.bytes_per_sector)+boot_sector_adds;
    fat2_start_adds=fat1_start_adds+(Sys_BPB.sectors_per_fats)*(Sys_BPB.bytes_per_sector);
    root_start_adds=fat2_start_adds+(Sys_BPB.sectors_per_fats)*Sys_BPB.bytes_per_sector;

    Get_free_root_adds();
    Get_free_fat1_tab_adds();
    Get_file_free_cluster_adds();
}

//===============================================================================
//	finction	:Sd_search_file  	按文件名查找文件
//      detials         :not super long file name file&catalog
//	input		:null
//	output		:null
//	return		:result 0=file not found. 1=file found.
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
uint8_t Sd_search_file(uint32_t adds,uint32_t file_type,uint32_t fn_p1,uint32_t fn_p2)
{
    uint8_t  k,result,root_pcb_cnt;
    uint16_t i,j;
    uint32_t R_adds,adds_temp,RF_adds;

    result=root_pcb_cnt=0;
    Index.file_num=0;
    Index.catalog_num=0;

    R_adds=fat1_start_adds/Sys_BPB.bytes_per_sector;
    sd_read(R_adds,Sd_buff);
    Index.next_root_cluster_adds=((Sd_buff[0x000b]<<24)|(Sd_buff[0x00a]<<16)|(Sd_buff[0x009]<<8)|Sd_buff[0x008]);

    R_adds=adds/Sys_BPB.bytes_per_sector;
next_sector:
    sd_read(R_adds,Sd_buff);

    ++root_pcb_cnt;
    if(root_pcb_cnt==9)
    {
        root_pcb_cnt=1;

        if(Index.next_root_cluster_adds==EOF_CLUT_FLAG)
        {
            result=0;
            goto end_search1;						//is end
        }
        adds_temp=Index.next_root_cluster_adds-2;
        adds_temp<<=12;
        R_adds=(adds+adds_temp)/Sys_BPB.bytes_per_sector;

        adds_temp=(Index.next_root_cluster_adds)*4+fat1_start_adds;
        RF_adds=adds_temp/Sys_BPB.bytes_per_sector;
        adds_temp%=Sys_BPB.bytes_per_sector;

        sd_read(RF_adds,Sd_buff);
        Index.next_root_cluster_adds=((Sd_buff[adds_temp+3]<<24)|(Sd_buff[adds_temp+2]<<16)|(Sd_buff[adds_temp+1]<<8)|Sd_buff[adds_temp]);
        sd_read(R_adds,Sd_buff);
    }

    for(k=0; k<16; k++)
    {
        i=k*CAT_FILE_SIZE;
        j=i+0x0b;

        Index.file_attrib=Sd_buff[j];
        if(Index.file_attrib==0) goto end_search1;						//is end
        if(Index.file_attrib==0x10)								//is catalog type
        {
            j=i+06;
            if(Sd_buff[j-0x06]==0xE5) continue;
            if(Sd_buff[j]==0x7E) continue;

            ++Index.catalog_num;
        }
//        if((Index.file_attrib==0x20)||(Index.file_attrib==0x0F))				//is file type
        if(Index.file_attrib==0x20)								//is file type
        {
//            if(Index.file_attrib==0x0F)
//            {
//            	if(Sd_buff[i]==0xE5) continue;							//it was used but had del
//            	if((Sd_buff[i+8]==0xFF)&&(Sd_buff[i+9]==0xFF)&&(Sd_buff[i+10]==0xFF))  continue;
//            	if(Sd_buff[i]>3)  continue;							//file name small then 39 char
//
//            	//get file name
//            	//loc_fp1=
//            	//loc_fp2=
//
//                ++k;
//                i=k*CAT_FILE_SIZE;
//                j=i+0x06;
//                Index.file_type_name=((Sd_buff[j+2]<<16)|(Sd_buff[j+3]<<8)|Sd_buff[j+4]);	//get file type name
//                Index.file_type_name&=0x00ffffff;
//                if(Index.file_type_name==file_type)  						//file type name is right
//                {
//                    Index.file_name_part1=loc_fp1;
//                    Index.file_name_part2=loc_fp2;
//                }
//                else Index.file_type_name=0;
//            }
//            else
//            {
            j=i+0x06;
            if(Sd_buff[j-0x06]==0xE5) continue;						//it was used but had del
            if(Sd_buff[j]==0x7E) continue;							//long file name

            ++Index.file_num;

            Index.file_type_name=((Sd_buff[j+2]<<16)|(Sd_buff[j+3]<<8)|Sd_buff[j+4]);	//get file type name
            Index.file_type_name&=0x00ffffff;
            if(Index.file_type_name==file_type)  						//file type name is right
            {
                j=i;
                Index.file_name_part1=((Sd_buff[j]<<24)|(Sd_buff[j+1]<<16)|(Sd_buff[j+2]<<8)|Sd_buff[j+3]);
                Index.file_name_part2=((Sd_buff[j+4]<<24)|(Sd_buff[j+5]<<16)|(Sd_buff[j+6]<<8)|Sd_buff[j+7]);
                if((Index.file_name_part1==fn_p1)&&(Index.file_name_part2==fn_p2))		//is the file witch was searching
                {
                    j=i+0x0e;
                    Index.file_creat_time=((Sd_buff[j]<<8)|Sd_buff[j+1]);
                    Index.file_modefy_time=((Sd_buff[j+2]<<8)|Sd_buff[j+3]);
                    Index.file_visit_time=((Sd_buff[j+4]<<8)|Sd_buff[j+5]);
                    j=i+0x1c;
                    Index.file_lenth=((Sd_buff[j+3]<<24)|(Sd_buff[j+2]<<16)|(Sd_buff[j+1]<<8)|Sd_buff[j]);
                    Index.file_current_cluster_num=((Sd_buff[j-7]<<24)|(Sd_buff[j-8]<<16)|(Sd_buff[j-1]<<8)|Sd_buff[j-2]);
                    result=1;
                    Index.file_current_cluster_adds=root_start_adds+(Index.file_current_cluster_num-2)*(Sys_BPB.sectors_per_cluster)*(Sys_BPB.bytes_per_sector);
                    Index.file_next_cluster_adds=fat1_start_adds+(Index.file_current_cluster_num)*4;
                    //get next cluster num
                    goto end_search;
                }
                else
                {
                    Index.file_name_part1=Index.file_name_part2=0;
                }
            }
            else Index.file_type_name=0;
//            }
        }
    }
    ++R_adds;
    goto  next_sector;
end_search1:
    if((result==0)&&(Index.catalog_num!=0))	//search from sec lever catalog
    {
    }
end_search:
    return(result);
}

//===============================================================================
//	finction	:Get_next_cluster_num  取下一簇地址
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
void Get_next_cluster_num(void)
{
    uint16_t i,clus_index;
    uint32_t R_adds;

//    R_adds=Index.file_next_cluster_adds/Sys_BPB.bytes_per_sector;
//    clus_index=Index.file_next_cluster_adds%Sys_BPB.bytes_per_sector;	//get next clus num
    R_adds=Index.file_next_cluster_adds/Sys_BPB.bytes_per_sector;
    i=R_adds;
    clus_index=Index.file_next_cluster_adds-i*Sys_BPB.bytes_per_sector;

    sd_read(R_adds,Sd_buff);
    Index.file_next_cluster_num=((Sd_buff[clus_index+3]<<24)|(Sd_buff[clus_index+2]<<16)|(Sd_buff[clus_index+1]<<8)|Sd_buff[clus_index]);
}

//===============================================================================
//	finction	:Updata_current_cluster_num_adds_next_cluster_adds  更新当前使用的簇号和下一簇的簇号
//      detials         :null
//	input		:null
//	output		:null
//	return		:result 0=nor 1=have bad cluster
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
uint8_t Updata_current_cluster_num_adds_next_cluster_adds(void)
{
    uint8_t result;
//    uint32_t R_adds;

    Index.file_current_cluster_num=Index.file_next_cluster_num;
    Index.file_current_cluster_adds=root_start_adds+(Index.file_current_cluster_num-2)*(Sys_BPB.sectors_per_cluster)*(Sys_BPB.bytes_per_sector);
    Index.file_next_cluster_adds=fat1_start_adds+(Index.file_current_cluster_num)*4;
    if(Index.file_next_cluster_num==BAD_CLUT_FLAG) result=1;
    else result=0;

    return(result);
}

//===============================================================================
//	finction	:Get_free_root_adds  	取得根目录的空闲地址
//      detials         :get the empty juan dizhi
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-12-13 11:57
//	modefy		:null
//===============================================================================
void Get_free_root_adds(void)
{
    uint8_t i;
    uint32_t root_st_adds,rw_adds,setor_cnt,look_loop;

    root_st_adds=root_start_adds;
    rw_adds=root_st_adds/(Sys_BPB.bytes_per_sector);
    look_loop=Sys_BPB.total_sector_num-rw_adds;

    for(setor_cnt=0; setor_cnt<look_loop-1; setor_cnt++)
    {
        sd_read(rw_adds,Sd_buff);
        for(i=0; i<16; i++)
        {
//           if((Sd_buff[i*0x10+0x0b]==0x00)||(Sd_buff[i*0x10]==0xe5))
            if(Sd_buff[i*0x20+0x0b]==0x00)
            {
                Index.root_free_write_adds=rw_adds*(Sys_BPB.bytes_per_sector)+i*0x20;
                goto out_root;
            }
        }
        ++rw_adds;
    }
out_root:
    __nop();
///     Index.root_free_write_adds-=0x20;
//     i-=2;
//     for(j=0;j<64;j++)
//     Sd_buff[i*0x20+j]=0;
//     sd_write(rw_adds,Sd_buff);
}

//===============================================================================
//	finction	:Get_free_fat1_tab_adds  	取分区表1地址
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-12-13 11:57
//	modefy		:null
//===============================================================================
void Get_free_fat1_tab_adds(void)
{
    uint8_t i;
    uint32_t fat1_st_adds,rw_adds,setor_cnt,look_loop;

    fat1_st_adds=fat1_start_adds;
    rw_adds=fat1_st_adds/(Sys_BPB.bytes_per_sector);
    look_loop=Sys_BPB.sectors_per_fats;

    for(setor_cnt=0; setor_cnt<look_loop; setor_cnt++)
    {
        sd_read(rw_adds,Sd_buff);
        for(i=0; i<128; i++)
        {
            if((Sd_buff[i*0x04+0x00]+Sd_buff[i*0x04+0x01]+Sd_buff[i*0x04+0x02]+Sd_buff[i*0x04+0x03])==0x00)
            {
                Index.fat1_free_write_adds=rw_adds*(Sys_BPB.bytes_per_sector)+i*0x04;
                goto out_fat1;
            }
        }
        ++rw_adds;
    }
out_fat1:
    __nop();
}

//===============================================================================
//	finction	:Get_file_free_cluster_adds  取文件目录的空闲簇地址
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-12-13 11:57
//	modefy		:null
//===============================================================================
void Get_file_free_cluster_adds(void)
{
    uint8_t i;
    uint32_t rw_adds,setor_cnt,max_id,min_id,next_rt_cluster_adds,adds_temp,RF_adds,Flenth=0;

    min_id=max_id=0;
////     look_loop=Sys_BPB.sectors_per_fats;
////     fat1_st_adds=fat1_start_adds;
////     rw_adds=fat1_st_adds/(Sys_BPB.bytes_per_sector);
////
////     for(setor_cnt=0;setor_cnt<look_loop;setor_cnt++)
//////     for(setor_cnt=0;setor_cnt<512;setor_cnt++)			//for test
////     {
////        sd_read(rw_adds,Sd_buff);
////        for(i=0;i<128;i++)
////        {
////           min_id=((Sd_buff[i*0x04+0x00])|(Sd_buff[i*0x04+0x01]<<8)|(Sd_buff[i*0x04+0x02]<<16)|(Sd_buff[i*0x04+0x03]<<24));
////           if((min_id==BIAO_XIANG_FLAG)||(min_id==EOF_CLUT_FLAG)||(min_id==FIRST_CLUT_FLAG)||(min_id==BIAO_XIANG_FLAG1)||(min_id==BAD_CLUT_FLAG)||(min_id==OTHER_FLAG1)) continue;
////           if(min_id>max_id) max_id=min_id;
////        }
////        ++rw_adds;
////
////        if((DrvGPIO_GetBit(E_GPC,0))==1) break;		//CAR TURN OFF
////     }
//--------look from root
    setor_cnt=0;
    rw_adds=fat1_start_adds/Sys_BPB.bytes_per_sector;
    sd_read(rw_adds,Sd_buff);
    next_rt_cluster_adds=((Sd_buff[0x000b]<<24)|(Sd_buff[0x00a]<<16)|(Sd_buff[0x009]<<8)|Sd_buff[0x008]);

    rw_adds=root_start_adds/(Sys_BPB.bytes_per_sector);
look_rl:
    sd_read(rw_adds,Sd_buff);
    ++setor_cnt;
    if(setor_cnt==9)
    {
        setor_cnt=1;

        if(next_rt_cluster_adds==EOF_CLUT_FLAG)  goto out_root_l;						//is end

        adds_temp=next_rt_cluster_adds-2;
        adds_temp<<=12;
        rw_adds=root_start_adds;
        rw_adds=(rw_adds+adds_temp)/Sys_BPB.bytes_per_sector;

        adds_temp=(next_rt_cluster_adds)*4+fat1_start_adds;
        RF_adds=adds_temp/Sys_BPB.bytes_per_sector;
        adds_temp%=Sys_BPB.bytes_per_sector;

        sd_read(RF_adds,Sd_buff);
        next_rt_cluster_adds=((Sd_buff[adds_temp+3]<<24)|(Sd_buff[adds_temp+2]<<16)|(Sd_buff[adds_temp+1]<<8)|Sd_buff[adds_temp]);

        sd_read(rw_adds,Sd_buff);
    }

    for(i=0; i<16; i++)
    {
        if((Sd_buff[i*0x20]==0xE5)&&(Sd_buff[i*0x20+0x0b]==0x20)) continue;

//           temp=((Sd_buff[i*0x20+0x0b])|(Sd_buff[i*0x20+0x0a]<<8)|(Sd_buff[i*0x20+0x09]<<16)|(Sd_buff[i*0x20+0x08]<<24));
//           if(temp!=0x54585420) continue;

        else if(Sd_buff[i*0x20+0x0b]!=0x20)  continue;
        else
        {
            min_id=((Sd_buff[i*0x20+0x1a])|(Sd_buff[i*0x20+0x1b]<<8)|(Sd_buff[i*0x20+0x14]<<16)|(Sd_buff[i*0x20+0x15])<<24);
            if(min_id>max_id) max_id=min_id;

            Flenth=((Sd_buff[i*0x20+0x1c])|(Sd_buff[i*0x20+0x1d]<<8)|(Sd_buff[i*0x20+0x1e]<<16)|(Sd_buff[i*0x20+0x1f])<<24);
        }
    }
    ++rw_adds;
    goto look_rl;
out_root_l:
    adds_temp=Flenth/(Sys_BPB.bytes_per_sector);
    ++max_id;
    Flenth=adds_temp/(Sys_BPB.sectors_per_cluster);
    Index.file_free_write_cluster_adds=Flenth+max_id;
    __nop();
}

//===============================================================================
//	finction	:Copy_fat_tab_each_other  备份分区表
//      detials         :null
//	input		:select=1:fat1 to fat2; select=2:fat2 to fat1
//	output		:null
//	return		:null
//	edit		:sam 2012-12-13 11:57
//	modefy		:null
//===============================================================================
void Copy_fat_tab_each_other(uint8_t select)
{
    uint16_t fat_sector,fat_sector_cnt;
    uint32_t fat1_tab_adds,fat2_tab_adds,rw_adds;

    fat_sector=Sys_BPB.sectors_per_fats;
    fat1_tab_adds=fat1_start_adds/(Sys_BPB.bytes_per_sector);
    fat2_tab_adds=fat2_start_adds/(Sys_BPB.bytes_per_sector);
    for(fat_sector_cnt=0; fat_sector_cnt<fat_sector; fat_sector_cnt++)
    {
        if(select==1)
            rw_adds=fat2_tab_adds+fat_sector_cnt;
        else
            rw_adds=fat1_tab_adds+fat_sector_cnt;
        sd_read(rw_adds,Sd_buff);

        if(select==1)
            rw_adds=fat1_tab_adds+fat_sector_cnt;
        else
            rw_adds=fat2_tab_adds+fat_sector_cnt;
        sd_write(rw_adds,Sd_buff);
    }
}

//===============================================================================
//	finction	:Creak_file_info  建立文件信息，暂时没用
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-12-13 11:57
//	modefy		:null
//===============================================================================
void Creak_file_info(void)
{
    uint8_t temp,j;
    uint16_t i,mm;

    C_file_info.file_type_name=0x00545854;			//file type is .txt
    C_file_info.file_name_part1=0x30313233;
    C_file_info.file_name_part2=0x20202020;
    C_file_info.file_attrib=0x20;
    C_file_info.file_creat_time_ms=0x02;


    i=0;
    C_file_info.file_creat_time=0x0008;
    temp=RTC_DATA.minu_temp;
    temp&=0x7f;
    j=(temp>>4)*10;
    i+=j;
    j=temp;
    j&=0x0f;
    i+=j;
    i<<=5;
    C_file_info.file_creat_time|=i;

    i=0;
    temp=RTC_DATA.hour_temp;
    temp&=0x3f;
    j=(temp>>4)*10;
    i+=j;
    j=temp;
    j&=0x0f;
    i+=j;
    i<<=11;
    C_file_info.file_creat_time|=i;

    C_file_info.file_modefy_time=C_file_info.file_creat_time;

    i=0;
    temp=RTC_DATA.day_temp;
    temp&=0x3f;
    j=(temp>>4)*10;
    i+=j;
    j=temp;
    j&=0x0f;
    i+=j;
    C_file_info.file_creat_date=i;

    i=0;
    temp=RTC_DATA.mouth_temp;
    temp&=0x1f;
    j=(temp>>4)*10;
    i+=j;
    j=temp;
    j&=0x0f;
    i+=j;
    i<<=5;
    C_file_info.file_creat_date|=i;

    i=0;
    temp=RTC_DATA.year_temp;
    temp&=0x3f;
    j=(temp>>4)*10;
    i+=j;
    j=temp;
    j&=0x0f;
    i+=j;

    temp=RTC_DATA.mouth_temp;
    temp&=0x80;
    if(temp==0x80)
        mm=1900;
    else
        mm=2000;

    i+=mm;
    i-=1980;
    i<<=9;
    C_file_info.file_creat_date|=i;

    C_file_info.file_modefy_date=C_file_info.file_creat_date;
    C_file_info.file_visit_date=C_file_info.file_creat_date;
    C_file_info.file_lenth=0x00;
}

//===============================================================================
//	finction	:Write_file_dir_info
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-12-13 11:57
//	modefy		:null
//===============================================================================
void Write_file_dir_info(void)
{
//    uint8_t  i;
    uint16_t start_byte_adds;
    uint32_t rw_adds;

    rw_adds=(Index.root_free_write_adds)/(Sys_BPB.bytes_per_sector);
    start_byte_adds=(Index.root_free_write_adds)%(Sys_BPB.bytes_per_sector);
    Creak_file_info();
    sd_read(rw_adds,Sd_buff);

    Sd_buff[start_byte_adds+0]=C_file_info.file_name_part1>>24;
    Sd_buff[start_byte_adds+1]=C_file_info.file_name_part1>>16;
    Sd_buff[start_byte_adds+2]=C_file_info.file_name_part1>>8;
    Sd_buff[start_byte_adds+3]=C_file_info.file_name_part1;
    Sd_buff[start_byte_adds+4]=C_file_info.file_name_part2>>24;
    Sd_buff[start_byte_adds+5]=C_file_info.file_name_part2>>16;
    Sd_buff[start_byte_adds+6]=C_file_info.file_name_part2>>8;
    Sd_buff[start_byte_adds+7]=C_file_info.file_name_part2;

    Sd_buff[start_byte_adds+8]=C_file_info.file_type_name>>16;
    Sd_buff[start_byte_adds+9]=C_file_info.file_type_name>>8;
    Sd_buff[start_byte_adds+10]=C_file_info.file_type_name;
    Sd_buff[start_byte_adds+11]=C_file_info.file_attrib;

    Sd_buff[start_byte_adds+12]=0x00;

    Sd_buff[start_byte_adds+13]=C_file_info.file_creat_time_ms;		//c ms
    Sd_buff[start_byte_adds+14]=C_file_info.file_creat_time;
    Sd_buff[start_byte_adds+15]=C_file_info.file_creat_time>>8;
    Sd_buff[start_byte_adds+16]=C_file_info.file_creat_date;
    Sd_buff[start_byte_adds+17]=C_file_info.file_creat_date>>8;
    Sd_buff[start_byte_adds+18]=C_file_info.file_visit_date;
    Sd_buff[start_byte_adds+19]=C_file_info.file_visit_date>>8;

    Sd_buff[start_byte_adds+20]=Index.file_free_write_cluster_adds>>16;
    Sd_buff[start_byte_adds+21]=Index.file_free_write_cluster_adds>>24;

    Sd_buff[start_byte_adds+22]=C_file_info.file_modefy_time;
    Sd_buff[start_byte_adds+23]=C_file_info.file_modefy_time>>8;
    Sd_buff[start_byte_adds+24]=C_file_info.file_modefy_date;
    Sd_buff[start_byte_adds+25]=C_file_info.file_modefy_date>>8;

    Sd_buff[start_byte_adds+26]=Index.file_free_write_cluster_adds;
    Sd_buff[start_byte_adds+27]=Index.file_free_write_cluster_adds>>8;

    Sd_buff[start_byte_adds+28]=C_file_info.file_lenth>>24;
    Sd_buff[start_byte_adds+29]=C_file_info.file_lenth>>16;
    Sd_buff[start_byte_adds+30]=C_file_info.file_lenth>>8;
    Sd_buff[start_byte_adds+31]=C_file_info.file_lenth;

    sd_write(rw_adds,Sd_buff);
}

//===============================================================================
//	finction	:Write_file_fat_tab_dir_info
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-12-13 11:57
//	modefy		:null
//===============================================================================
void Write_file_fat_tab_dir_info(uint32_t fat_data)
{
//    uint8_t  i;
    uint16_t start_byte_adds;
    uint32_t rw_adds;

    rw_adds=(Index.fat1_free_write_adds)/(Sys_BPB.bytes_per_sector);
    start_byte_adds=(Index.fat1_free_write_adds)%(Sys_BPB.bytes_per_sector);


    sd_read(rw_adds,Sd_buff);

    start_byte_adds-=4;

    Sd_buff[start_byte_adds+3]=fat_data>>24;
    Sd_buff[start_byte_adds+2]=fat_data>>16;
    Sd_buff[start_byte_adds+1]=fat_data>>8;
    Sd_buff[start_byte_adds+0]=fat_data;
    sd_write(rw_adds,Sd_buff);
}

//===============================================================================
//	finction	:Sd_read_file  	按文件名读取SD卡中的文件
//      detials         :null
//	input		:null
//	output		:null
//	return		:result 0=file not found. 1=file found and reading. 2=file read end.
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
uint8_t Sd_read_file(uint32_t file_type,uint32_t fn_p1,uint32_t fn_p2)
{
    uint8_t result,k;
    uint16_t l_cnt;

    if(SD_read_flag==0)
    {
        result=Sd_search_file(root_start_adds,file_type,fn_p1,fn_p2);
        if(result==1)								//file found.
        {
            GB_read_sec_cnt=0;
            SD_read_flag=1;
            Get_next_cluster_num();
            GB_read_adds_temp=Index.file_current_cluster_adds/Sys_BPB.bytes_per_sector;

            sd_read(GB_read_adds_temp,Sd_buff);
            for(l_cnt=0; l_cnt<512; l_cnt++)
            {
                SD_data_buff[l_cnt]=Sd_buff[l_cnt];
                ++read_file_lenth_cnt;
                if(read_file_lenth_cnt==(Index.file_lenth)) break;
            }
            if(read_file_lenth_cnt!=(Index.file_lenth))
            {
                ++GB_read_adds_temp;
                ++GB_read_sec_cnt;
            }
            else    								//end file
            {
                result=2;
                SD_read_flag=0;
            }
        }
        else									//file not found.
        {
            result=0;
        }
    }
    else
    {
        result=1;
        sd_read(GB_read_adds_temp,Sd_buff);
        for(l_cnt=0; l_cnt<512; l_cnt++)
        {
            SD_data_buff[l_cnt]=Sd_buff[l_cnt];
            ++read_file_lenth_cnt;
            if(read_file_lenth_cnt==(Index.file_lenth)) break;
        }
        if(read_file_lenth_cnt!=(Index.file_lenth))
        {
            ++GB_read_adds_temp;
            ++GB_read_sec_cnt;
            if(GB_read_sec_cnt==Sys_BPB.sectors_per_cluster)
            {
                GB_read_sec_cnt=0;
                k=Updata_current_cluster_num_adds_next_cluster_adds();	//updata num&adds
                Get_next_cluster_num();
                GB_read_adds_temp=Index.file_current_cluster_adds/Sys_BPB.bytes_per_sector;
                if(k==1) result=0;						//have bad  cluster
            }
        }
        else    								//end file
        {
            result=2;
            SD_read_flag=0;
        }
    }
#ifdef __DEBUG__
    if(result>0)
    {
        export_tx_lenth=512;
        export_tx_flag=1;
        export_tx_lenth_cnt=0;
        DrvUART_Write(UART_PORT0,Sd_buff,1);
        En_Uart012_THRE_int(UART_PORT0);
        while(export_tx_flag==1);						//wait complete
        return(result);
    }
#endif
    return(result);
}

//===============================================================================
//	finction	:Sd_write_file
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
void Sd_write_file(void)
{
    uint8_t result;

    result=result;
}

//===============================================================================
//	finction	:Sd_creat_file
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
void Sd_creat_file(void)
{
    uint8_t result;

    Get_free_root_adds();
    Get_free_fat1_tab_adds();
    Get_file_free_cluster_adds();

    Write_file_dir_info();
    Write_file_fat_tab_dir_info(EOF_CLUT_FLAG);
    result=result;
}


//===============================================================================
//	finction	:Sd_del_file
//      detials         :null
//	input		:null
//	output		:null
//	return		:null
//	edit		:sam 2012-11-23 15:43
//	modefy		:null
//===============================================================================
void Sd_del_file(void)
{
    uint8_t result;

    result=result;
}






