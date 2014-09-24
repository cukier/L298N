/*
 * l298n.c
 *
 *  Created on: 22/09/2014
 *      Author: cuki
 */

#include<18F252.h>
#DEVICE ADC=10

#FUSES HS

#use delay(clock=15MHz)
//#use delay(clock=9MHz)
//#use rs232(xmit=PIN_C6, baud=9600)

#define debouce 100
//#define tmr0reg 0
//#define tmr0reg 59285
#define clr_output	255

#define inc 200

#define R PIN_C0
#define S PIN_C1
#define T PIN_C3

#define pin_sobe PIN_B0
#define pin_desce PIN_B1

short ctrl_bto = TRUE;
short bto_sobe;
short bto_desce;
short dir;
short write;
short ctrl_inc;

int estado;

long timer0cont;
long tmr0reg;
long adc;
long adcAux;

void saida_onda(int i_estado) {
	switch (i_estado) {
	case 0:
		output_high(R);
		output_low(S);
		output_high(T);
		break;
	case 1:
		output_high(R);
		output_low(S);
		output_low(T);
		break;
	case 2:
		output_high(R);
		output_high(S);
		output_low(T);
		break;
	case 3:
		output_low(R);
		output_high(S);
		output_low(T);
		break;
	case 4:
		output_low(R);
		output_high(S);
		output_high(T);
		break;
	case 5:
		output_low(R);
		output_low(S);
		output_high(T);
		break;
	default:
		output_low(R);
		output_low(S);
		output_low(T);
	}
}

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

	saida_onda(estado);
}

int main(void) {

	setup_adc(ADC_CLOCK_INTERNAL);
	setup_adc_ports(AN0);
	set_adc_channel(0);

	setup_timer_0(T0_INTERNAL | T0_DIV_1);
	set_timer0(tmr0reg);

	setup_timer_2(T2_DIV_BY_1, 0xFF, 1);

//	setup_ccp1(CCP_PWM);
	setup_ccp1(CCP_OFF);
	set_pwm1_duty(512);

	enable_interrupts(GLOBAL);

	saida_onda(clr_output);

	while (TRUE) {

		delay_us(10);
		adc = read_adc();

		if (adc != adcAux) {
			adcAux = adc;
//			tmr0reg = (long) 65535 / 1024 * adc;
			set_pwm1_duty(adc);
		}

		bto_sobe = !input(pin_sobe);
		bto_desce = !input(pin_desce);

		if (bto_sobe ^ bto_desce) {
			if (ctrl_bto) {
				ctrl_bto = FALSE;
				delay_ms(debouce);
				clear_interrupt(INT_TIMER0);
				enable_interrupts(INT_TIMER0);
				dir = bto_sobe;
				setup_ccp1(CCP_PWM);
			}
		} else if (!ctrl_bto) {
			ctrl_bto = TRUE;
			disable_interrupts(INT_TIMER0);
			estado = 0;
			saida_onda(clr_output);
			setup_ccp1(CCP_OFF);
		}
	}

	return 0;
}
