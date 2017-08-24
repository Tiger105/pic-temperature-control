#include "head.h"
void pwm_on(void)
{
	PR2=0xFF;
	CCPR1L=0x00;
	CCP1CON=0x0F;
	CCPR2L=0x00;
	CCP2CON=0x0F;
	TRISC=0xF9;
	T2CON=0x07;
}
void pwm_out(temp_form pwm_dt)
{
	pwm_dt0.byte = pwm_dt.byte[0];
	pwm_dt1.byte = pwm_dt.byte[1];
	if (set_ds_pin)
	{
		if (pwm_dt0.bit0)	CCP2Y = 1;
		else	CCP2Y = 0;
		if (pwm_dt0.bit1)	CCP2X = 1;
		else	CCP2X = 0;
		pwm_dt0.byte = pwm_dt0.byte >> 2;
		if (pwm_dt1.bit0)	pwm_dt0.bit6 = 1;
		else	pwm_dt0.bit6 = 0;
		if (pwm_dt1.bit1)	pwm_dt0.bit7 = 1;
		else	pwm_dt0.bit7 = 0;
		CCPR2L = pwm_dt0.byte;
	}
	else
	{
		if (pwm_dt0.bit0)	CCP1Y = 1;
		else	CCP1Y = 0;
		if (pwm_dt0.bit1)	CCP1X = 1;
		else	CCP1X = 0;
		pwm_dt0.byte = pwm_dt0.byte >> 2;
		if (pwm_dt1.bit0)	pwm_dt0.bit6 = 1;
		else	pwm_dt0.bit6 = 0;
		if (pwm_dt1.bit1)	pwm_dt0.bit7 = 1;
		else	pwm_dt0.bit7 = 0;
		CCPR1L = pwm_dt0.byte;
	}
} 
void pwm(void)
{
	if (set_ds_pin)	temp_val.tempData = temp_val1.tempData;
	else	temp_val.tempData = temp_val0.tempData;
	if(temp_val.tempData > 0xFC90 || temp_val.tempData < (pwm_reftemp.tempData - 100))
	{
		pwm_dt.tempData = 1023;
		pwm_out(pwm_dt);
		RB0=1;
		RD2=1;
		temp_state.bit2=1;
		if (!set_ds_pin)
		{
			RB2=0;
			RD0=0;
			temp_state.bit0 = 0;
		}
	}
	if (temp_val.tempData > (pwm_reftemp.tempData ) && temp_val.tempData < 1201)//1350)
	{
		if (!set_ds_pin)
		{
			in0 = 1200 - temp_val.tempData;
			out0 = in0 * 22;//2.01 * in0 - 3.956 * in0_k + 1.946 * in0_kk + 1.968 * out0 - 0.968 * out0_kk;
			/*in0_kk = in0_k;
			in0_k = in0;
			out0_kk = out0_k;
			out0_k = out0;
			if (out0 > 102)
				pwm_dt.tempData = 1023;
			else if(out0 < 0)
				pwm_dt.tempData = 0;
			else*/	pwm_dt.tempData = (unsigned short)(out0);
			pwm_out(pwm_dt);
			if (temp_val.tempData > 1195 && temp_val.tempData < 1210)
			{
				RB2=1;
				RD0=1;
				temp_state.bit0=1;
			}
		}
		else// if (set_ds_pin == 1 && temp_val.tempData > pwm_reftemp.tempData )
		{
			in1 = 1200 - temp_val.tempData;
			out1 = in1 * 22;//2.01 * in1 - 3.956 * in1_k + 1.946 * in1_kk + 1.968 * out1 - 0.968 * out1_kk;
			/*in1_kk = in1_k;
			in1_k = in1;
			out1_kk = out1_k;
			out1_k = out1;
			if (out1 > 102)
				pwm_dt.tempData = 1023;
			else if(out1 < 0)
				pwm_dt.tempData = 0;
			else*/	pwm_dt.tempData = (unsigned short)(out1);
			pwm_out(pwm_dt);
		}
	}
	if(temp_val.tempData > 1350 && temp_val.tempData < 2000)
	{
		RB2 = 0;
		RD0 = 0;
		temp_state.bit0 = 0;
		RB1 = 1;
		RD1 = 1;
		temp_state.bit1 = 1;
		pwm_dt.tempData = 0;
		pwm_out(pwm_dt);
		__delay_ms(2);
		CLRWDT();
		RD1 = 0;
		delta_cnt = 0;
	}
}


