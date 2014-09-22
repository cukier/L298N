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

#define ccp_nr 60000

#define debouce 100

#define pin_sobe PIN_B0
#define pin_desce PIN_B1

short ctrl1;
short ctrl2;
short ctrl_bto = TRUE;
short a;
short b;

#int_ccp1
void ccp1_isr() {
	clear_interrupt(INT_CCP1);
	ctrl1 = !ctrl1;
	if (ctrl1) {
		setup_ccp1(CCP_COMPARE_CLR_ON_MATCH);
		set_timer1(0);
		CCP_1 = ccp_nr;
	} else {
		setup_ccp1(CCP_COMPARE_SET_ON_MATCH);
		set_timer1(0);
		CCP_1 = ccp_nr;
	}
}

#int_ccp2
void ccp2_isr() {
	clear_interrupt(INT_CCP2);
	ctrl2 = !ctrl2;
	if (ctrl2) {
		setup_ccp2(CCP_COMPARE_CLR_ON_MATCH);
		set_timer3(0);
		CCP_2 = ccp_nr;
	} else {
		setup_ccp2(CCP_COMPARE_SET_ON_MATCH);
		set_timer3(0);
		CCP_2 = ccp_nr;
	}
}

int main(void) {

	set_timer1(0);
	setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
	set_timer1(0);
	setup_timer_3(T3_INTERNAL | T3_CCP2 | T3_DIV_BY_1);
	set_timer3(0);

	setup_ccp1(CCP_COMPARE_CLR_ON_MATCH);
	CCP_1 = ccp_nr;
	setup_ccp2(CCP_COMPARE_CLR_ON_MATCH);
	CCP_2 = ccp_nr;

	clear_interrupt(INT_CCP1);
	clear_interrupt(INT_CCP2);
//	enable_interrupts(INT_CCP1);
//	enable_interrupts(INT_CCP2);
	enable_interrupts(GLOBAL);

	while (TRUE) {
		a = !input(pin_sobe);
		b = !input(pin_desce);

		if (a ^ b) {
			delay_ms(debouce);
			if (ctrl_bto) {
				ctrl_bto = FALSE;
				if (a) {
					clear_interrupt(INT_CCP1);
					enable_interrupts(INT_CCP1);
				} else {
					clear_interrupt(INT_CCP2);
					enable_interrupts(INT_CCP2);
				}
			}
		} else if (!ctrl_bto) {
			ctrl_bto = TRUE;
			setup_ccp1(CCP_COMPARE_CLR_ON_MATCH);
			setup_ccp2(CCP_COMPARE_CLR_ON_MATCH);
			disable_interrupts(INT_CCP1);
			disable_interrupts(INT_CCP2);
			ctrl1 = ctrl2 = FALSE;
		}
	}

	return 0;
}
