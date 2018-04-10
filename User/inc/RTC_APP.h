//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: I2C_APP.h
//	First EDIT		: Sam
//	Data			: 2012.06.14
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================

#define		RTC_CTR_STATUS1_ADDS		0X00
#define		RTC_CTR_STATUS2_ADDS		0X01
#define		RTC_SEC_ADDS			0X02
#define		RTC_MIMUTE_ADDS			0X03
#define		RTC_HOUR_ADDS			0X04
#define		RTC_DAY_ADDS			0X05
#define		RTC_WEEK_DAY_ADDS		0X06
#define		RTC_MONTH_cFLAG_ADDS		0X07
#define		RTC_YEAR_ADDS			0X08
#define		RTC_MIMUTE_ALARM_ADDS		0X09
#define		RTC_HOUR_ALARM_ADDS		0X0A
#define		RTC_DAY_ALARM_ADDS		0X0B
#define		RTC_WEEK_DAY_ALARM_ADDS		0X0C
#define		RTC_CLK_OUT_FREQ_ADDS		0X0D
#define		RTC_TIMER_CTRL_ADDS		0X0E
#define		RTC_TIMER_COUNT_VALU_ADDS	0X0F

#define		RTC_AE_FLAG			BIT7
#define		RTC_AF_FLAG			BIT3
#define		RTC_AIE_FLAG			BIT1
#define		RTC_TE_FLAG			BIT7
#define		RTC_TF_FLAG			BIT2
#define		RTC_TIE_FLAG			BIT0
#define		RTC_FE_FLAG			BIT7
#define		RTC_TI_TP_FLAG			BIT4
#define		RTC_TEST1_FLAG			BIT7		//0=NOR MODE; 1=EXT CLK TEST MODE
#define		RTC_STOP_FLAG			BIT5		//0=RUN; 1=STOP
#define		RTC_TESTC_FLAG			BIT3		//0=DIS POWER ON RESET; 1=EN POWER ON RESET
#define		RTC_VL_FLAG			BIT7
#define		RTC_C_FALG			BIT7

#define		RTC_CMD_READ			0XA3
#define		RTC_CMD_WRITE			0XA2		
#define		RTC_CMD_WORD_ADDS		0XFF

//----------------------------------------------------------------------------------------
#ifdef __RTC_APP__

volatile uint8_t RTC_write_flag;

#else
extern volatile uint8_t RTC_write_flag;

extern volatile struct Rtc_str{
                 volatile uint8_t  sec_temp;
                 volatile uint8_t  minu_temp;
                 volatile uint8_t  hour_temp;
                 volatile uint8_t  day_temp;
                 volatile uint8_t  mouth_temp;
                 volatile uint8_t  year_temp;
                 volatile uint8_t  week_temp;

                 volatile uint8_t  alarm_minu_temp;
                 volatile uint8_t  alarm_hour_temp;
                 volatile uint8_t  alarm_day_temp;
                 volatile uint8_t  alarm_week_temp;
                 
                 volatile uint8_t  status1_temp;
                 volatile uint8_t  status2_temp;                 
              }RTC_DATA,RTC_Set;

extern uint8_t RTC_read_1byte(void);
extern void RTC_read_12byte_data(void);
extern void RTC_initial(void);
extern void RTC_write_date(void);
extern void EINT0Callback(void);

#endif


