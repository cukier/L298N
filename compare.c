/*
 * compare.c
 *
 *  Created on: 22/09/2014
 *      Author: cuki
 */

/*
 * main.c
 *
 *  Created on: 07/02/2013
 *      Author: cuki
 */

#include<18F452.h>

#fuses HS
#use delay(clock=15MHz)
#use rs232(xmit=PIN_C6, baud=9600)

#define ccp_min 1000
#define ccp_max 60000
#define tempo_rampa 5 // em segundos#define freq_rede 60static short ctrl;//static short print;static long nr_divisoes;static long ccp_step;
static long ccp_cont = ccp_max;

#int_ext
void ext_isr() {
	clear_interrupt(INT_EXT);
	setup_ccp1(CCP_COMPARE_SET_ON_MATCH);
	set_timer1(0);
	CCP_1 = ccp_cont;
	ccp_cont -= ccp_step;
	clear_interrupt(INT_CCP1);
	enable_interrupts(INT_CCP1);
//	print = 1;
}

#int_ccp1
void ccp1_isr() {
	clear_interrupt(INT_CCP1);
	setup_ccp1(CCP_COMPARE_CLR_ON_MATCH);
	set_timer1(0);
	CCP_1 = 2500;
	disable_interrupts(INT_CCP1);
	if (!ctrl) {
		ctrl = 1;
		clear_interrupt(INT_EXT);
		enable_interrupts(INT_EXT_L2H);
	} else {
		ctrl = 0;
		clear_interrupt(INT_EXT);
		enable_interrupts(INT_EXT_H2L);
	}
	if (ccp_cont < ccp_min) {
		setup_ccp1(CCP_OFF);
		disable_interrupts(GLOBAL);
		output_high(PIN_C2);
	}
}

int main(void) {

	lcd_init();
	delay_ms(150);
	printf("\fStand by...");

	nr_divisoes = (long) ((float) freq_rede * tempo_rampa);
	ccp_step = (long) ((float) (ccp_max - ccp_min) / nr_divisoes);

	delay_ms(1);
	setup_ccp1(CCP_COMPARE_CLR_ON_MATCH);
	setup_timer_1(T1_INTERNAL);
	clear_interrupt(INT_EXT);
	enable_interrupts(INT_EXT_H2L);
	clear_interrupt(INT_CCP1);
	enable_interrupts(INT_CCP1);
	enable_interrupts(GLOBAL);
	while (TRUE) {
//		if (print) {
//			print = 0;
//			printf("\fCCP: %Lu", ccp_cont);
//		}
//		delay_ms(500);
	}
	return 0;
}
