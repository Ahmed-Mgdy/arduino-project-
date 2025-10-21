/*
 * Interrupt.h
 *
 *  Created on: Jul 6, 2023
 *      Author: usedn
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "microcontroller_config.h"

#define NUMBER_OF_INTERRUPTS 3

#define I_BIT 7


#define STATUS_REG EXTERNAL_INTERRUPT_STATUS_REG







typedef enum
{

	INT_0=64,
	INT_1=128,
	INT_2=32

}interrupts_num_t;

typedef enum
{
	_0_LOW_LEVEL=0,//0
	_0_ANY_LOGIC,//1
	_0_FALLING_EDGE,//2
	_0_RISING_EDGE,//3
	_1_LOW_LEVEL=0,//0
	_1_ANY_LOGIC=4,//
	_1_FALLING_EDGE=8,//8
	_1_RISING_EDGE=12//12



}_0_1_interrupt_sense_control_t;

typedef enum
{
	_2_FALLING_EDGE=0,
	_2_RISING_EDGE=64

}_2_interrupt_sense_control_t;

typedef struct
{
	interrupts_num_t interrupts_num;
	_0_1_interrupt_sense_control_t _0_1_interrupt_sense_control;
	_2_interrupt_sense_control_t   _2_interrupt_sense_control;

}interrupt_config_t;


void Interrupt_Initialize (interrupt_config_t *interrupt_config);
void Interrupt_Initialize_X (void);



#endif /* INTERRUPT_H_ */
