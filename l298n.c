/*
 * l298n.c
 *
 *  Created on: 22/09/2014
 *      Author: cuki
 */

#include<18F252.h>

#FUSES HS

#use delay(clock=16MHz)
#use rs232(xmit=PIN_C6, baud=9600)

int main(void) {

	printf("hello");
	return 0;

}
