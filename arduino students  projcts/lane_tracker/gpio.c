/******************************************************************************
 *
 * Module: GPIO
 *
 * File Name: gpio.c
 *
 * Description: Source file for the AVR GPIO driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#include "gpio.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "avr/io.h" /* To use the IO Ports Registers */




#ifndef PORT_A

volatile uint8 *Direction_registers[] = { &PORT_A_DIRECTION_REG,&PORT_B_DIRECTION_REG, &PORT_C_DIRECTION_REG, &PORT_D_DIRECTION_REG};
/* Reference to the Data Latch Register (Read and Write to Data Latch) */
volatile uint8 *Value_registers[]  = { &PORT_A_DIRECTION_REG,&PORT_B_VALUE_REG , &PORT_C_VALUE_REG , &PORT_D_VALUE_REG};
/* Reference to the Port Status Register  */
volatile uint8 *Read_registers[] = { &PORT_A_DIRECTION_REG,&PORT_B_READ_REG, &PORT_C_READ_REG, &PORT_D_READ_REG};

#endif



void GPIO_Direction_Init_pro (pin_config_t * pin_config)
{
	switch (pin_config->direction)
	{
	case GPIO_DIRECTION_OUTPUT :
		SET_BIT(*Direction_registers[pin_config->port] ,pin_config->pin);

		break;

	case GPIO_DIRECTION_INPUT :
		CLEAR_BIT(*Direction_registers[pin_config->port],pin_config->pin);

		break;
	}
	switch (pin_config->logic)
	{
	case LOGIC_HIGH :
		SET_BIT(*Value_registers[pin_config->port],pin_config->pin);
		break;
	case LOGIC_LOW  :
		CLEAR_BIT(*Value_registers[pin_config->port],pin_config->pin);
		break;
	}
}


void GPIO_setupPinDirection (uint8 port_num, uint8 pin_num, GPIO_PinDirectionType direction)
{
	if((pin_num >= NUM_OF_PINS_PER_PORT) || (port_num >= NUM_OF_PORTS))
	{
		/* Do Nothing */
	}
	else
	{
		switch (port_num)
		{
#ifdef  PORT_A
		case PORTA_INDEX :

			if (direction == PIN_OUTPUT )
			{
				SET_BIT(DDRA,pin_num);
			}
			else
			{
				CLEAR_BIT(DDRA,pin_num);
			}


			break;
#endif
#ifdef  PORT_B
		case PORTB_INDEX :

			if (direction == PIN_OUTPUT )
			{
				SET_BIT(DDRB,pin_num);
			}
			else
			{
				CLEAR_BIT(DDRB,pin_num);
			}


			break;
#endif
#ifdef  PORT_C
		case PORTC_INDEX :

			if (direction == PIN_OUTPUT )
			{
				SET_BIT(DDRD,pin_num);
			}
			else
			{
				CLEAR_BIT(DDRD,pin_num);
			}


			break;
#endif
#ifdef  PORT_D
		case PORTD_INDEX :

			if (direction == PIN_OUTPUT )
			{
				SET_BIT(DDRC,pin_num);
			}
			else
			{
				CLEAR_BIT(DDRC,pin_num);
			}


			break;
#endif
		}

	}



}


void GPIO_writePin(uint8 port_num, uint8 pin_num, uint8 value)
{
	/*
	 * Check if the input port number is greater than NUM_OF_PINS_PER_PORT value.
	 * Or if the input pin number is greater than NUM_OF_PINS_PER_PORT value.
	 * In this case the input is not valid port/pin number
	 */
	if((pin_num >= NUM_OF_PINS_PER_PORT) || (port_num >= NUM_OF_PORTS))
	{
		/* Do Nothing */
	}
	else
	{
		/* Write the pin value as required */
		switch(port_num)
		{
#ifdef PORT_A
		case PORTA_INDEX:
			if(value == LOGIC_HIGH)
			{
				SET_BIT(PORTA,pin_num);
			}
			else
			{
				CLEAR_BIT(PORTA,pin_num);
			}
			break;
#endif
#ifdef PORT_B
		case PORTB_INDEX:
			if(value == LOGIC_HIGH)
			{
				SET_BIT(PORTB,pin_num);
			}
			else
			{
				CLEAR_BIT(PORTB,pin_num);
			}
			break;
#endif
#ifdef PORT_C
		case PORTC_INDEX:
			if(value == LOGIC_HIGH)
			{
				SET_BIT(PORTC,pin_num);
			}
			else
			{
				CLEAR_BIT(PORTC,pin_num);
			}
			break;
#endif
#ifdef PORT_D
		case PORTD_INDEX:
			if(value == LOGIC_HIGH)
			{
				SET_BIT(PORTD,pin_num);
			}
			else
			{
				CLEAR_BIT(PORTD,pin_num);
			}
			break;
#endif
		}
	}
}

uint8 GPIO_readPin(uint8 port_num, uint8 pin_num)
{
	uint8 pin_value = LOGIC_LOW;

	/*
	 * Check if the input port number is greater than NUM_OF_PINS_PER_PORT value.
	 * Or if the input pin number is greater than NUM_OF_PINS_PER_PORT value.
	 * In this case the input is not valid port/pin number
	 */
	if((pin_num >= NUM_OF_PINS_PER_PORT) || (port_num >= NUM_OF_PORTS))
	{
		/* Do Nothing */
	}
	else
	{
		/* Read the pin value as required */
		switch(port_num)
		{
#ifdef PORT_A
		case PORTA_INDEX:
			if(BIT_IS_SET(PINA,pin_num))
			{
				pin_value = LOGIC_HIGH;
			}
			else
			{
				pin_value = LOGIC_LOW;
			}
			break;
#endif
#ifdef PORT_B
		case PORTB_INDEX:
			if(BIT_IS_SET(PINB,pin_num))
			{
				pin_value = LOGIC_HIGH;
			}
			else
			{
				pin_value = LOGIC_LOW;
			}
			break;
#endif
#ifdef PORT_C
		case PORTC_INDEX:
			if(BIT_IS_SET(PINC,pin_num))
			{
				pin_value = LOGIC_HIGH;
			}
			else
			{
				pin_value = LOGIC_LOW;
			}
			break;
#endif
#ifdef PORT_D


		case PORTD_INDEX:
			if(BIT_IS_SET(PIND,pin_num))
			{
				pin_value = LOGIC_HIGH;
			}
			else
			{
				pin_value = LOGIC_LOW;
			}
			break;
#endif
		}
	}

	return pin_value;
}
