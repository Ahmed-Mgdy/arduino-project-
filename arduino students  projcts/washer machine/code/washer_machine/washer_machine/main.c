#define F_CPU 8000000UL // Define CPU clock as 8 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

// Global variables
volatile uint8_t timer_done_flag = 0;
volatile uint8_t stop_flag = 0; // Flag to indicate system stop

// UART Initialization
void UART_init(unsigned int baud) {
	unsigned int ubrr = F_CPU / 16 / baud - 1;
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)(ubrr);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Enable RX and TX
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

// UART Send Data
void UART_send(char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

// UART Send String
void UART_send_string(const char *str) {
	while (*str) {
		UART_send(*str++);
	}
	UART_send('\r');
	UART_send('\n');
}

// PWM Initialization for Motor Control
void PWM_init() {
	TCCR0A = (1 << WGM01) | (1 << WGM00) | (1 << COM0A1); // Fast PWM, Non-Inverting
	TCCR0B = (1 << CS01); // Prescaler 8
	DDRD |= (1 << PD6); // PWM Output on PD6
}

// ADC Initialization
void ADC_init() {
	ADMUX = (1 << REFS0); // AVcc as reference voltage
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // Enable ADC, Prescaler 64
}

// ADC Read
uint16_t ADC_read(uint8_t channel) {
	ADMUX = (ADMUX & 0xF8) | channel; // Select ADC channel
	ADCSRA |= (1 << ADSC); // Start Conversion
	while (ADCSRA & (1 << ADSC)); // Wait for Conversion to Complete
	return ADC;
}

// Timer1 Initialization
void Timer1_init() {
	TCCR1B = (1 << WGM12); // CTC mode
	TIMSK1 = (1 << OCIE1A); // Enable Timer1 Compare Match A Interrupt
	sei(); // Enable Global Interrupts
}

// Start Timer1 with a given delay in milliseconds
void start_timer(uint16_t ms) {
	OCR1A = (F_CPU / 1024 / 1000) * ms - 1; // Calculate compare match value
	TCNT1 = 0; // Reset Timer Counter
	timer_done_flag = 0; // Reset flag
	TCCR1B |= (1 << CS12) | (1 << CS10); // Start Timer with Prescaler 1024
}

// Check if Timer is Done
uint8_t timer_done() {
	return timer_done_flag;
}

// Timer1 Compare Match A Interrupt Service Routine
ISR(TIMER1_COMPA_vect) {
	timer_done_flag = 1;
	TCCR1B &= ~((1 << CS12) | (1 << CS10)); // Stop Timer
}

// External Interrupt 2 ISR (Stop Button)
ISR(INT0_vect) {
	UART_send_string("Stop Switch Pressed");
	UART_send_string("Emergency Stop Activated");
	PORTB = 0b00100000; // Turn off all outputs on PORTB
	PORTC &= 0b00000000; // Turn off outputs on PORTC
	PORTD &= 0b00000000;
	PORTD &= ~(1 << PD4); // Turn off Buzzer
	stop_flag = 1; // Set stop flag
}

int main() {
	UART_init(9600);
	PWM_init();
	ADC_init();
	Timer1_init();

	DDRB = 0b11111011; // Set all PORTB pins as output except PB2
	DDRD = 0b11111111; // Set PORTD as output
	DDRC = 0b01111110; // Set PC4-PC7 as output
	DDRD &= ~(1 << PD2); // Set PD2 as input (new external interrupt)
	DDRD &= ~(1 << PD5); // Set PD5 as input (start switch)
	PORTD |= (1 << PD5); // Enable pull-up resistor for PD5
	PORTD |= (1 << PD2); // Enable pull-up resistor for PD2

	// Configure External Interrupt on INT0 (PD2)
	EICRA |= (1 << ISC01); // Trigger interrupt on falling edge (button press)
	EIMSK |= (1 << INT0);  // Enable external interrupt INT0

	sei(); // Enable global interrupts

	uint8_t prev_pwm = 0;

	while (1) {
		if (stop_flag) {
			UART_send_string("System Halted Due to Stop Command");
			stop_flag = 0; // Optionally reset the stop flag to allow restarting
			continue; // Skip the rest of the loop
		}

		// Start Switch Logic
		if (!(PIND & (1 << PD5))) {
			PORTB &= ~(1 << PC5);
			UART_send_string("system start");

			// Valve 1 ON
			PORTB |= (1 << PB0);
			PORTB |= (1 << PB4);
			PORTC |= (1 << PC4);
			UART_send_string("Valve 1 ON");
			start_timer(2000);
			while (!timer_done()) {
				if (stop_flag) break;
			}
			if (stop_flag) continue;
			PORTB &= ~(1 << PB0);
			PORTC &= ~(1 << PC4);
			UART_send_string("Valve 1 OFF");

			// Valve 2 ON
			PORTB |= (1 << PB1);
			PORTC |= (1 << PC5);
			UART_send_string("Valve 2 ON");
			start_timer(2000);
			while (!timer_done()) {
				if (stop_flag) break;
			}
			if (stop_flag) continue;
			PORTB &= ~(1 << PB1);
			PORTC &= ~(1 << PC5);
			UART_send_string("Valve 2 OFF");

			// Valve 3 ON
			PORTB |= (1 << PB7);
			PORTC |= (1 << PC3);
			UART_send_string("Valve 3 ON");
			start_timer(2000);
			while (!timer_done()) {
				if (stop_flag) break;
			}
			if (stop_flag) continue;
			PORTB &= ~(1 << PB7);
			PORTC &= ~(1 << PC3);
			UART_send_string("Valve 3 OFF");

			// Simulate Process
			UART_send_string("motor start forward");
			start_timer(5000);
			do {
				if (stop_flag) break;
				uint16_t adc_value = ADC_read(0); // Read ADC value from channel 0
				uint8_t new_pwm = adc_value / 4; // Scale 10-bit ADC to 8-bit PWM (0-255)
				if (new_pwm != prev_pwm) { // Check if PWM value has changed
					OCR0A = new_pwm;       // Update PWM duty cycle
					prev_pwm = new_pwm;    // Store the new value
					// Send updated ADC and PWM values via UART
					char buffer[50];
					sprintf(buffer, "ADC: %d, PWM: %d", adc_value, new_pwm);
					UART_send_string(buffer);
				}
				PORTC |= (1 << PC1);
				PORTC &= ~(1 << PC2);
			} while (!timer_done());
			if (stop_flag) continue;
			UART_send_string("motor start reverse");
			start_timer(5000);
			do {
				if (stop_flag) break;
				uint16_t adc_value = ADC_read(0); // Read ADC value from channel 0
				uint8_t new_pwm = adc_value / 4; // Scale 10-bit ADC to 8-bit PWM (0-255)
				if (new_pwm != prev_pwm) { // Check if PWM value has changed
					OCR0A = new_pwm;       // Update PWM duty cycle
					prev_pwm = new_pwm;    // Store the new value
					// Send updated ADC and PWM values via UART
					char buffer[50];
					sprintf(buffer, "ADC: %d, PWM: %d", adc_value, new_pwm);
					UART_send_string(buffer);
				}
				PORTC |= (1 << PC2);
				PORTC &= ~(1 << PC1);
			} while (!timer_done());
			if (stop_flag) continue;
			PORTC &= ~(1 << PC1);
			PORTC &= ~(1 << PC2);

			// Outlet Valve ON
			PORTB |= (1 << PB3);
			PORTB |= (1 << PB6);
			UART_send_string("Outlet Valve ON");
			start_timer(5000);
			while (!timer_done()) {
				if (stop_flag) break;
			}
			if (stop_flag) continue;
			PORTB &= ~(1 << PB3);
			PORTB &= ~(1 << PB6);
			UART_send_string("Outlet Valve OFF");

			// Buzzer ON
			PORTD |= (1 << PD4);
			UART_send_string("Buzzer ON");
			start_timer(1000);
			while (!timer_done()) {
				if (stop_flag) break;
			}
			if (stop_flag) continue;
			PORTD &= ~(1 << PD4);
			UART_send_string("Buzzer OFF");

			UART_send_string("PROCESS END");
		}
	}

	return 0;
}
