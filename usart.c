#include "head.h"
//-----usart configuration-----
void usart_init(void)
{
	RCSTA=0x90;
	RCIF=0x00;
	TXIE=0x00;
	RCIE=0x00;
	BRGH=0x00;
	SPBRG=31;
	TXEN=0x01;
	return;
}
//-----Tx 1byte-----
void asitrn( unsigned char trn_dt)
{
	while(TRMT == 0);
	TXREG = trn_dt;
	return;	
}
//-----Tx Data-----	
void usart(void)
{
	check = 0xEB ^ usa_res[0] ^ usa_res[1] ^ temp_state.byte;
	if (set_ds_pin)	check = check ^ 0x95;
	else	check = check ^ 0x90;
	asitrn(0xEB);
	if (set_ds_pin)	asitrn(0x95);
	else	asitrn(0x90);
	asitrn(usa_res[0]);   
	asitrn(usa_res[1]);
	asitrn(temp_state.byte);
	asitrn(check);
	return;
}
