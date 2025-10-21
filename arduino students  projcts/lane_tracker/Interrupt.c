/*
 * Interrupt.c
 *
 *  Created on: Jul 6, 2023
 *      Author: usedn
 */

#include "Interrupt.h"
#include "avr/io.h"



void Interrupt_Initialize (interrupt_config_t *interrupt_config)
{



	switch (interrupt_config->interrupts_num)
	{
#ifdef EXTERNAL_INT_0
	case INT_0 :

		DDRD  &= (~(1<<PD2));               // Configure INT0/PD2 as input pin
		EXTERNAL_INTERRUPT_CONTROL_REG |= interrupt_config->_0_1_interrupt_sense_control ; // Trigger INT0 with the raising edge
		EXTERNAL_INTERRUPT_GENERAL_CONTROL  |= interrupt_config->interrupts_num ;                // Enable external interrupt pin INT0
		STATUS_REG  |= (1<<I_BIT);
		break;
#endif
#ifdef EXTERNAL_INT_1
	case INT_1 :

		DDRD  &= (~(1<<PD3));               // Configure INT1/PD3 as input pin
		EXTERNAL_INTERRUPT_CONTROL_REG |= interrupt_config->_0_1_interrupt_sense_control ; // Trigger INT0 with the raising edge
		EXTERNAL_INTERRUPT_GENERAL_CONTROL  |= interrupt_config->interrupts_num ;                // Enable external interrupt pin INT0
		STATUS_REG  |= (1<<I_BIT);
		break;
#endif
#ifdef EXTERNAL_INT_2
	case INT_2 :   //PROBLEM WITH 2

		DDRB   &= (~(1<<PB2));   // Configure INT2/PB2 as input pin
		EXTERNAL_INTERRUPT_CONTROLL_STATUS_REG |= interrupt_config->_0_1_interrupt_sense_control;     // Trigger INT2 with the raising edge
		EXTERNAL_INTERRUPT_GENERAL_CONTROL   |= interrupt_config->interrupts_num;	 // Enable external interrupt pin INT2
		SREG   |= (1<<7);
		break;
#endif
		// Enable interrupts by setting I-bit
	}     // Trigger INT2 with the raising edge



}





