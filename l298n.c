/*
 * l298n.c
 *
 *  Created on: 22/09/2014
 *      Author: cuki
 */

#include<18F252.h>

#FUSES HS

#use delay(clock=15MHz)
//#use delay(clock=9MHz)
#use rs232(xmit=PIN_C6, baud=9600)

#define debouce 100
#define tmr0reg 55118
//#define tmr0reg 59285

#define pin_sobe PIN_B0
#define pin_desce PIN_B1

short ctrl_bto = TRUE;
short ba;
short bb;
short dir;

int estado;
int saida[6] = { 0b101, 0b100, 0b110, 0b010, 0b011, 0b001 };

#INT_TIMER0
void isr_timer0() {
	clear_interrupt(INT_TIMER0);
	set_timer0(tmr0reg);
	if (dir) {
		estado++;
		if (estado > 5)
			estado = 0;
	} else {
		estado--;
		if (estado > 5)
			estado = 5;
	}
	output_c(saida[estado]);
}

int main(void) {

	setup_timer_0(T0_INTERNAL | T0_DIV_1);
	set_timer0(tmr0reg);

//	enable_interrupts(INT_TIMER0);
	enable_interrupts(GLOBAL);

	while (TRUE) {
		ba = !input(pin_sobe);
		bb = !input(pin_desce);

		if (ba ^ bb) {
			if (ctrl_bto) {
				ctrl_bto = FALSE;
				delay_ms(debouce);
				clear_interrupt(INT_TIMER0);
				enable_interrupts(INT_TIMER0);
				dir = ba;
			}
		} else if (!ctrl_bto) {
			ctrl_bto = TRUE;
			disable_interrupts(INT_TIMER0);
			estado = 0;
		}
	}

	return 0;
}
