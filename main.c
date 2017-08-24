#include "head.h"
__CONFIG(HS & UNPROTECT & WDTDIS & PWRTDIS & BORDIS);

//-----declaration of variable-----
unsigned char rw_sword, check, set_ds_pin, r_flg, rst_dt, rst_cnt;
unsigned char result[9], usa_res[2], delta_cnt;
byte_form tem_dt, temp_state, crc_data, pwm_dt0, pwm_dt1;
temp_form temp_val, temp_val0, temp_val1, temp_buf;
temp_form pwm_reftemp, delta, err_cnt, pwm_dt;
float in0, in0_k, in0_kk, out0, out0_k, out0_kk;
float in1, in1_k, in1_kk, out1, out1_k, out1_kk;

//-----interrupt process----- 
static void interrupt temp(void)
{
	if (TMR1IF)
 	{
  		TMR1IF = 0;
  		if (rst_dt)
  		{
			rst_cnt ++;
//-----Tx reset pulse-----
		    if (rst_cnt)
		    {
				TRISD = 0x30;
				TMR1H = 0xF6;
				TMR1L = 0x9F;	
		    };
//-----Rx presence pulse-----
			if (rst_cnt == 2)
			{
				rst_cnt = 0; 
				TMR1ON = 0;
				r_flg = 1;
			};
		} 
		else
		{
//-----read & write state-----
			TMR1ON = 0; 
			r_flg = 1;
		};
	};
};

void main(void)
{
	TRISD = 0x30;  
	TRISB = 0;
	PORTB = 0;
	PORTD = 0;
	pwm_on();
//-----Timer & interrupt_Configuration-----
	GIE = 1;
	PEIE = 1;
	T1CON = 0;
	TMR1IE = 1;
	rst_cnt = 0;
//-----TempData_Configuration-----
	pwm_reftemp.tempData = 1155;
	in0_kk = 0;	in1_kk = 0;
	in0_k = 0;	in1_k = 0;
	out0_kk = 0;	out1_kk = 0;
	out0_k = 0;	out1_k = 0;
	err_cnt.tempData = 0;
	delta_cnt = 0;
	temp_val0.tempData = 0xFC90;	temp_val1.tempData = 0xFC90;
	usa_res[0] = 0;
	usa_res[1] = 0;
	temp_state.byte = 0;
	result[2] = 0x7D;
	result[3] = 0xB7;
	result[4] = 0x7F;
	result[5] = 0xFF;
	result[7] = 0x10;
//-----DS18B20 & Usart_Configuration-----
	__delay_ms(30);
	CLRWDT();
	__delay_ms(30);
	CLRWDT();
	__delay_ms(30);
	CLRWDT();
	usart_init();
	scratchpad_input();

	while(1)
	{
		set_ds_pin = 0; 
		mea_temp();
		pwm();
		usart();

		set_ds_pin = 1; 
		mea_temp();
		pwm();
		usart();

	};

}

