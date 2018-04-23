//============================================================================================
//	Project name		: andan test
//	MCU TYPE		: Nuv100RE3AN
//	File name		: Pwm_APP.h	
//	First EDIT		: Sam
//	Data			: 2012.06.19
//	Issue			: V1
//	contact us		: sufangyi@126.com
//	Modefy by		:
//============================================================================================


//--------------------------------------------------------------------------------------------
#ifdef __PWM_APP__
uint8_t  Pwm_out_adj_flag;//用于标志修改了4ma/20ma对应值 1=修改了,2=初始化值
uint16_t adj_4ma_pwm_data;//用于赋值PWMA->CMR0
uint16_t adj_20ma_pwm_data;//用于赋值PWMA->CMR0

uint16_t adj_4ma_pwm1_data;//用于赋值PWMA->CMR1
uint16_t adj_20ma_pwm1_data;//用于赋值PWMA->CMR1

float Pwm_last_result;

#else
extern float Pwm_last_result;

extern uint8_t Pwm_out_adj_flag;
extern uint16_t adj_4ma_pwm_data;
extern uint16_t adj_20ma_pwm_data;

extern uint16_t adj_4ma_pwm1_data;
extern uint16_t adj_20ma_pwm1_data;

extern void InitPWM(void);
extern void Start_pwm0(void);
extern void Stop_pwm0(void);
extern void Start_pwm1(void);
extern void Stop_pwm1(void);
extern void Adj_pwm0_cnr_cmr_data(uint32_t d_cnr,uint32_t d_cmr);
extern void Adj_pwm1_cnr_cmr_data(uint32_t d_cnr,uint32_t d_cmr);
extern void Pwm_output_test(uint8_t ch,uint32_t data);
extern void Pwm_output_ctrl(void);
extern void Pwm_get_resul_from_flash(uint8_t select);
extern float Pwm_int2float(uint8_t sele,uint32_t data);
extern uint32_t Pwm_float2int(float data);
extern void InitPWM_4(void);

#endif
