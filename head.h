#include "htc.h"
#ifndef _XTAL_FREQ
	#define _XTAL_FREQ 20000000
#endif
//-----Define of Type-----
typedef union _byte_form
 {  unsigned char byte;
    struct 
     {
		unsigned bit0:1;
		unsigned bit1:1;
		unsigned bit2:1;
		unsigned bit3:1;
		unsigned bit4:1;
		unsigned bit5:1;
		unsigned bit6:1;
		unsigned bit7:1;
	 };
 }byte_form;
typedef union _temp_form
 {  unsigned char byte[2];
    unsigned short tempData;
 }temp_form;
//-----------Define of Function---------------//
extern void usart_init(void);
extern void usart(void);
extern void asitrn( unsigned char trn_dt);
extern void tm1_duty(void);
extern void tm1_reset(void);
extern void rst_temp(void);
extern void write_bit0(void);
extern void write_bit1(void);
extern unsigned char read_bit(void);
extern unsigned char read_byte(void);
extern void write_byte(unsigned char st_dt);
extern void mea_temp(void);
extern void scratchpad_input(void);
extern void pwm_on(void);
extern void pwm_out(temp_form pwm_dt);
extern void pwm(void);
extern void crc(void);
extern void crc_conv(void);
extern void crc_shift(void);

//-----------Define of Variable---------------//
extern unsigned char result[9], usa_res[2], check, delta_cnt;
extern unsigned char rw_sword, set_ds_pin, r_flg, rst_dt, rst_cnt;
extern byte_form tem_dt, temp_state, crc_data, pwm_dt0, pwm_dt1;
extern temp_form temp_val, temp_val0, temp_val1, temp_buf;
extern temp_form delta, err_cnt;
extern temp_form pwm_reftemp, pwm_dt;
extern float in0, in0_k, in0_kk, out0, out0_k, out0_kk;
extern float in1, in1_k, in1_kk, out1, out1_k, out1_kk;

//------------const values---------------------//
/* target_value 75 1200 triac_frequency 400Hz
temp_value		pwm_reftemp			temp/0.0625		pwm_min_time(repeat)ms		duty_triac
	69				1104				96					120						2
	70				1120				80					100						3
	71				1136				64					80						4
	72				1152				48					60						5(min)
	73				1168				32					40						8
`	74				1184				16					20						16
115ms
*/

