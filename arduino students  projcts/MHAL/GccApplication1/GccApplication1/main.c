/*
 * inteput.c
 *
 * Created: 5/5/2023 2:35:16 PM
 * Author : Ahmed2000
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "STDMACROS.h"
#include "intrrupt.h"


int main(void)
{
	INT0_INIT();
	INT1_INIT();
	
	setbit(DDRC,7);
	setbit(DDRC,2);
    /* Replace with your application code */
    while (1) 
    {
		toggelbit(DDRD,6);
    }
}

