#ifndef SERVO_H
#define SERVO_H

#include <avr/io.h>

void servo_init();
void move_servo(uint16_t angle);

#endif
