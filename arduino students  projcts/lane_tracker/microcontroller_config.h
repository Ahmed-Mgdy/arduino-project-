/*
 * gpio_config.h
 *
 *  Created on: Oct 31, 2023
 *      Author: usedn
 */

#ifndef MICROCONTROLLER_CONFIG_H_
#define MICROCONTROLLER_CONFIG_H_

#include <avr/io.h>
#include <stdio.h>
#include "std_types.h"

/* you can choose from ->atmega_16 =1
 *                     ->atmega_32 =2
 *                     ->pic_18f4620 =3
 *                     ->
 */



#define atmega_32_or_atmega_16 1
#define ATtiny48               2
#define ATmega_8A              3


/* ******************************************************** */

#define  microcontroller  ATmega_8A

/* ******************************************************** */



#if (microcontroller == atmega_32_or_atmega_16)

//       I/O  REG
#define NUM_OF_PORTS           4
#define NUM_OF_PINS_PER_PORT   8

#define PORT_A_DIRECTION_REG        *((volatile uint8*)0x3A)
#define PORT_A_VALUE_REG            *((volatile uint8*)0x3B)
#define PORT_A_READ_REG             *((volatile uint8*)0x39)

#define PORT_B_DIRECTION_REG        *((volatile uint8*)0x37)
#define PORT_B_VALUE_REG            *((volatile uint8*)0x38)
#define PORT_B_READ_REG             *((volatile uint8*)0x36)

#define PORT_C_DIRECTION_REG        *((volatile uint8*)0x34)
#define PORT_C_VALUE_REG            *((volatile uint8*)0x35)
#define PORT_C_READ_REG             *((volatile uint8*)0x33)

#define PORT_D_DIRECTION_REG        *((volatile uint8*)0x31)
#define PORT_D_VALUE_REG            *((volatile uint8*)0x32)
#define PORT_D_READ_REG             *((volatile uint8*)0x30)

//       INTERUPT  REG

#define EXTERNAL_INT_0
#define EXTERNAL_INT_1
#define EXTERNAL_INT_2

#define EXTERNAL_INTERRUPT_CONTROL_REG         *((volatile uint8*)0x55)
#define EXTERNAL_INTERRUPT_CONTROLL_STATUS_REG *((volatile uint8*)0x54)
#define EXTERNAL_INTERRUPT_GENERAL_CONTROL     *((volatile uint8*)0x5B)
#define EXTERNAL_INTERRUPT_STATUS_REG          *((volatile uint8*)0x5F)







#elif (microcontroller == ATtiny48)
//       I/O  REG
#define NUM_OF_PORTS           4
#define NUM_OF_PINS_PER_PORT   8

#define PORT_A_DIRECTION_REG        *((volatile uint8*)0x2D)
#define PORT_A_VALUE_REG            *((volatile uint8*)0x2E)
#define PORT_A_READ_REG             *((volatile uint8*)0x2C)

#define PORT_B_DIRECTION_REG        *((volatile uint8*)0x24)
#define PORT_B_VALUE_REG            *((volatile uint8*)0x25)
#define PORT_B_READ_REG             *((volatile uint8*)0x23)

#define PORT_C_DIRECTION_REG        *((volatile uint8*)0x27)
#define PORT_C_VALUE_REG            *((volatile uint8*)0x28)
#define PORT_C_READ_REG             *((volatile uint8*)0x26)

#define PORT_D_DIRECTION_REG        *((volatile uint8*)0x2A)
#define PORT_D_VALUE_REG            *((volatile uint8*)0x0A)
#define PORT_D_READ_REG             *((volatile uint8*)0x09)



#elif (microcontroller == ATmega_8A)

//    I/O  REGG

#define NUM_OF_PORTS           3
#define NUM_OF_PINS_PER_PORT   8


#define PORT_B
#define PORT_C
#define PORT_D



#define PORT_A_DIRECTION_REG        *((volatile uint8*)NULL)
#define PORT_A_VALUE_REG            *((volatile uint8*)NULL)
#define PORT_A_READ_REG             *((volatile uint8*)NULL)

#define PORT_B_DIRECTION_REG        *((volatile uint8*)0x37)
#define PORT_B_VALUE_REG            *((volatile uint8*)0x38)
#define PORT_B_READ_REG             *((volatile uint8*)0x36)

#define PORT_C_DIRECTION_REG        *((volatile uint8*)0x34)
#define PORT_C_VALUE_REG            *((volatile uint8*)0x35)
#define PORT_C_READ_REG             *((volatile uint8*)0x33)

#define PORT_D_DIRECTION_REG        *((volatile uint8*)0x31)
#define PORT_D_VALUE_REG            *((volatile uint8*)0x32)
#define PORT_D_READ_REG             *((volatile uint8*)0x30)


//    INTERRUPT REG

#define EXTERNAL_INT_0
#define EXTERNAL_INT_1



#define EXTERNAL_INTERRUPT_CONTROL_REG         *((volatile uint8*)0x55)
#define EXTERNAL_INTERRUPT_GENERAL_CONTROL     *((volatile uint8*)0x5B)
#define EXTERNAL_INTERRUPT_STATUS_REG          *((volatile uint8*)0x5F)


#endif













#endif /* MICROCONTROLLER_CONFIG_H_ */
