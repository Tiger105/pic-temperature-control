#include "head.h"

//-----reset pulse Configuration-----
void rst_temp(void)
{
	rst_dt = 1;
	if(!set_ds_pin)
	{
		TRISD = 0x20;
		RD4 = 0;
	}
	else
	{
		TRISD = 0x10;
		RD5 = 0;
	}
	tm1_reset();
	rst_dt = 0;
}
//-----write bit0 Configuration-----
void write_bit0(void)
{  
	if(!set_ds_pin)
	{
		TRISD = 0x20;
		RD4 = 0;
	}
	else
	{
		TRISD = 0x10;
		RD5 = 0;
	}
	tm1_duty();
	TRISD = 0x30;
	_delay(30);
	CLRWDT();
 }
//-----write bit1 Configuration-----
void write_bit1(void)
{
	if(!set_ds_pin)
	{
		TRISD = 0x20;
		RD4 = 0;
	}
	else
	{
		TRISD = 0x10;
		RD5 = 0;
	}
	_delay(6);
	CLRWDT();
	TRISD = 0x30;
	tm1_duty();
}
//-----read bit Configuration----- 
unsigned char read_bit(void)
{
	rw_sword = 0;
	if(!set_ds_pin)
	{
		TRISD = 0x20;
		RD4 = 0;
	}
	else
	{
		TRISD = 0x10;
		RD5 = 0;
	}
	_delay(6);
	CLRWDT();
	TRISD = 0x30;
	r_flg = 0;
	TMR1H = 0xFF;
	TMR1L = 0xE0;	
	TMR1ON = 1;
	while (!r_flg);
	if(!set_ds_pin)
	{
		if (RD4)
		rw_sword = 1;
	}
	else
	{
		if (RD5)
		rw_sword = 1;
	}
	tm1_duty();
	return (rw_sword);
}
//-----write byte Configuration-----
void write_byte(unsigned char st_dt)
{
	tem_dt.byte = st_dt;
	if(!tem_dt.bit0)	write_bit0();
	else	write_bit1();
	if(!tem_dt.bit1)	write_bit0();
	else	write_bit1();
	if(!tem_dt.bit2)	write_bit0();
	else	write_bit1();
	if(!tem_dt.bit3)	write_bit0();
	else	write_bit1();
	if(!tem_dt.bit4)	write_bit0();
	else	write_bit1();
	if(!tem_dt.bit5)	write_bit0();
	else	write_bit1();
	if(!tem_dt.bit6)	write_bit0();
	else	write_bit1();
	if(!tem_dt.bit7)	write_bit0();
	else	write_bit1();
} 
//-----read byte Configuration-----
unsigned char read_byte(void)
{
	unsigned char tt_dt;
	tem_dt.byte = 0x00;
	tt_dt = read_bit();
	if(tt_dt)	tem_dt.bit0 = 1;
	tt_dt = read_bit();
	if(tt_dt)	tem_dt.bit1 = 1;
	tt_dt = read_bit();
	if(tt_dt)	tem_dt.bit2 = 1;
	tt_dt = read_bit();
	if(tt_dt)	tem_dt.bit3 = 1;
	tt_dt = read_bit();
	if(tt_dt)	tem_dt.bit4 = 1;
	tt_dt = read_bit();
	if(tt_dt)	tem_dt.bit5 = 1;
	tt_dt = read_bit();
	if(tt_dt)	tem_dt.bit6 = 1;
	tt_dt = read_bit();
	if(tt_dt)	tem_dt.bit7 = 1; 
	return (tem_dt.byte);
}
//-----scratchpad Configuration-----
void scratchpad_input(void)
{
	set_ds_pin = 0;
	rst_temp(); 
	write_byte(0xCC); 
	write_byte(0x4E);
	write_byte(0x7D);
	write_byte(0xB7);
	write_byte(0x7F);
	rst_temp(); 
	write_byte(0xCC);
	write_byte(0x48); 
	TRISB = 0x20;
	while (!RD4);
	set_ds_pin = 1;
	rst_temp(); 
	write_byte(0xCC); 
	write_byte(0x4E);
	write_byte(0x7D);
	write_byte(0xB7);
	write_byte(0x7F);
	rst_temp(); 
	write_byte(0xCC);
	write_byte(0x48); 
	TRISB = 0x10;
	while (!RD5);
	TRISD = 0x30;
}
//-----measure temperature-----
void mea_temp(void)
{
	rst_temp(); 
	write_byte(0xCC);
	write_byte(0x44);
	if(set_ds_pin)	while (!RD5);
	else	while (!RD4);
	rst_temp(); 
	write_byte(0xCC);
	write_byte(0xBE);
	result[0] = read_byte();
	result[1] = read_byte();
	read_byte();
	read_byte();
	read_byte();
	read_byte();
	result[6] = read_byte();
	read_byte();
	result[8] = read_byte();
	temp_buf.byte[0] = result[0];
	temp_buf.byte[1] = result[1];
	if(set_ds_pin)	temp_val.tempData = temp_val1.tempData;
	else	temp_val.tempData = temp_val0.tempData;
	if(!(temp_buf.tempData == 0xFFFF || temp_buf.tempData == 0x0550))
	{
		crc();
		if(!crc_data.byte)
		{
			if(temp_buf.tempData < 0x07D0 || temp_buf.tempData > 0xFC90)
			{
				if (delta_cnt < 100)	delta_cnt ++;
				if(temp_buf.tempData >= temp_val.tempData)
					delta.tempData = temp_buf.tempData - temp_val.tempData;
				else	delta.tempData = temp_val.tempData - temp_buf.tempData;
				if((RB2 == 1 &&delta_cnt > 29 && delta.tempData < 30) || (RB2 == 0 && delta_cnt > 29 && delta.tempData < 20) || temp_val.tempData == 0xFC90)
				{
					temp_val.tempData = temp_buf.tempData;
					err_cnt.tempData = 0;
				}
				else	err_cnt.tempData ++;
			}
			//else	err_cnt.tempData ++;
		}
		//else	err_cnt.tempData ++;
	}
	else	err_cnt.tempData ++;
	if(err_cnt.tempData == 0x02FF)
	{
		RB0 = 0;
		RD2 = 0;
		RB1 = 1;
		RD1 = 1;
		temp_state.bit1 = 1;
		CCPR1L = 0;
		__delay_ms(2);
		CLRWDT();
		RD1 = 0;
		err_cnt.tempData = 0;
		delta_cnt = 0;
	}
	if(set_ds_pin)	temp_val1.tempData = temp_val.tempData;
	else	temp_val0.tempData = temp_val.tempData;
	usa_res[0] = temp_val.byte[0];
	usa_res[1] = temp_val.byte[1];
}
void crc_conv(void)
{
	crc_data.byte = crc_data.byte ^ 0x19;
}
void crc_shift(void)
{
	if(crc_data.bit0)
	{
		crc_data.byte = crc_data.byte>>1;
		crc_data.bit7 = 1;
	}
	else
	{
		crc_data.byte = crc_data.byte>>1;
		crc_data.bit7 = 0;
	}
}
void crc(void)
{
	crc_data.byte = 0;
	for(int j = 0; j < 9; j++)
	{
		if(result[j] & 0x01 == 0x01)	crc_conv();
		crc_shift();
		if(result[j] & 0x02 == 0x02)	crc_conv();
		crc_shift();
		if(result[j] & 0x04 == 0x04)	crc_conv();
		crc_shift();
		if(result[j] & 0x08 == 0x08)	crc_conv();
		crc_shift();
		if(result[j] & 0x10 == 0x10)	crc_conv();
		crc_shift();
		if(result[j] & 0x20 == 0x20)	crc_conv();
		crc_shift();
		if(result[j] & 0x40 == 0x40)	crc_conv();
		crc_shift();
		if(result[j] & 0x80 == 0x80)	crc_conv();
		crc_shift();
	}
}
