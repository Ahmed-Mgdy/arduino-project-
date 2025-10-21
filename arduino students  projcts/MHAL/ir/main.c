#include <avr/io.h>
#include <util/delay.h>
#include "ir.h" // Include the IR sensor interface

int main(void) {
    // Initialize the IR sensor
    IR_Init();

    // Set PC0 as output to control an LED (for example)
    DDRC |= (1 << PC0);

    while (1) {
        // Check if an object is detected
        if (IR_ObjectDetected()) {
            // Object detected (sensor is active), turn on the LED
            PORTC |= (1 << PC0);
        } else {
            // No object detected (sensor is not active), turn off the LED
            PORTC &= ~(1 << PC0);
        }

        _delay_ms(100); // Delay to avoid rapid LED flickering
    }

    return 0;
}
