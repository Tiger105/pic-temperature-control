#include "head.h"
void tm1_reset(void)
{
	r_flg = 0;
	TMR1H = 0xF6;
	TMR1L = 0x9F;	
	TMR1ON = 1;
	while (!r_flg);
}
void tm1_duty(void)
{
	r_flg = 0;
	TMR1H = 0xFF;
	TMR1L = 0x00;	
	TMR1ON = 1;
	while (!r_flg);
}
