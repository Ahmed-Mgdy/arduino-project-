#include "ultrasonic.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#define F_CPU 16000000ul


#define TRIGGER_PIN 2
#define ECHO_PIN 3

volatile uint16_t pulse_start_time = 0;
volatile uint16_t pulse_end_time = 0;
volatile uint8_t echo_received = 0;

void ultrasonic_init() {
	// Set TRIGGER_PIN as an output and ECHO_PIN as an input
	DDRD |= (1 << TRIGGER_PIN);
	DDRD &= ~(1 << ECHO_PIN);
	// Enable external interrupt on ECHO_PIN (INT1)
	GICR |= (1 << INT1);
	MCUCR |= (1 << ISC11); // Falling edge triggers interrupt
}

void ultrasonic_trigger() {
	// Send a trigger pulse
	PORTD |= (1 << TRIGGER_PIN);
	_delay_us(10);
	PORTD &= ~(1 << TRIGGER_PIN);
}

ISR(INT1_vect) {
	if (echo_received == 0) {
		pulse_start_time = TCNT1;
		echo_received = 1;
		} else {
		pulse_end_time = TCNT1;
		echo_received = 0;
	}
}

void timer1_init() {
	// Set Timer1 to CTC mode
	TCCR1A = 0;
	TCCR1B = (1 << WGM12);
	
	// Set the prescaler to 64 for a 16MHz clock (adjust as needed)
	TCCR1B |= (1 << CS11) | (1 << CS10);
	
	// Enable Timer1 compare match interrupt
	TIMSK |= (1 << OCIE1A);
	
	// Set the compare value to your desired timeout (adjust as needed)
	OCR1A = 10000; // Example timeout value
}

uint16_t measure_distance() {
	ultrasonic_trigger();
	_delay_ms(1000); // Delay before next measurement

	if (pulse_start_time > 0 && pulse_end_time > 0) {
		uint16_t pulse_duration = pulse_end_time - pulse_start_time;
		uint16_t distance = (pulse_duration * 0.017); // Speed of sound at 20°C is 343 m/s

		return distance;
	}

	return 0; // If no valid distance measurement is available
}
/*
 * ultrasonic.c
 *
 * Created: 11/8/2023 11:10:45 AM
 *  Author: user
 */ 
