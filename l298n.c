/*
 * l298n.c
 *
 *  Created on: 22/09/2014
 *      Author: cuki
 */

#include<18F252.h>

#FUSES HS

#use delay(clock=15MHz)
#use rs232(xmit=PIN_C6, baud=9600)

#define ccp_nr 15000

short ctrl;

#int_ccp1
void ccp1_isr() {
	clear_interrupt(INT_CCP1);
	ctrl = !ctrl;
	if (ctrl) {
		setup_ccp1(CCP_COMPARE_CLR_ON_MATCH);
		set_timer1(0);
		CCP_1 = ccp_nr;
	} else {
		setup_ccp1(CCP_COMPARE_SET_ON_MATCH);
		set_timer1(0);
		CCP_1 = ccp_nr;
	}
}

int main(void) {

	setup_ccp1(CCP_COMPARE_CLR_ON_MATCH);
	set_timer1(0);
	CCP_1 = ccp_nr;
	setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);

	clear_interrupt(INT_CCP1);
	enable_interrupts(INT_CCP1);
	enable_interrupts(GLOBAL);

	while (TRUE) {

	}

	return 0;
}
