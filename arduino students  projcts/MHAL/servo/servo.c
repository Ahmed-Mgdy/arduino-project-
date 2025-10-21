#include "servo.h"
#include <util/delay.h>

#define SERVO_PIN 1 

void generate_pwm(uint16_t duration) {
    PORTB |= (1 << SERVO_PIN);
    _delay_us(duration);
    PORTB &= ~(1 << SERVO_PIN);
    _delay_us(20000 - duration);
}

void servo_init() {
    DDRB |= (1 << SERVO_PIN);
}

void move_servo(uint16_t angle) {
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    uint16_t pwm_duration = 500 + angle * 10;
    generate_pwm(pwm_duration);
}
