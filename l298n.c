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
#define max 24

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
long adcAux1;
long adcAux2;
long pwm_cont;

/*long pwm_duty[max] = { 512, 644, 768, 874, 955, 1006, 1023, 1006, 955, 874, 768,
 644, 512, 379, 256, 150, 68, 17, 1, 17, 68, 150, 256, 379 };

 #INT_TIMER2
 void isr_timer2() {
 clear_interrupt(INT_TIMER2);
 if (pwm_cont >= max)
 pwm_cont = 0;
 set_pwm1_duty(pwm_duty[pwm_cont++]);
 }*/

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

long le_canal(int channel) {
	set_adc_channel(channel);
	delay_us(40);
	return read_adc();
}

int main(void) {

	setup_adc(ADC_CLOCK_INTERNAL);
	setup_adc_ports(AN0_AN1_AN3);

	setup_timer_0(T0_INTERNAL | T0_DIV_4);
	set_timer0(tmr0reg);

	setup_timer_2(T2_DIV_BY_1, 0xFF, 16);

	setup_ccp1(CCP_OFF);
	set_pwm1_duty(512);

//	clear_interrupt(INT_TIMER2);
//	enable_interrupts(INT_TIMER2);
	enable_interrupts(GLOBAL);

	saida_onda(clr_output);

	while (TRUE) {

		adc = le_canal(0);

		if (adc != adcAux1) {
			adcAux1 = adc;
			tmr0reg = (long) 65535 / 1024 * adc;
		}

		adc = le_canal(1);

		if (adc != adcAux2) {
			adcAux2 = adc;
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
