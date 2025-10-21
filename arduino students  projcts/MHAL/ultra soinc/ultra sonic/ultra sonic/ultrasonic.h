#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <avr/io.h>

void ultrasonic_init();
void ultrasonic_trigger();
uint16_t measure_distance();

#endif // ULTRASONIC_H
