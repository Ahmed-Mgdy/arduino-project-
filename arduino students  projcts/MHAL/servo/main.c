#include <avr/io.h>
#include <util/delay.h>
#include "servo.h"
#include "config.h"

int main() {
   
    servo_init();

    while (1) {
      
        move_servo(0);
        _delay_ms(1000);

        move_servo(90);
        _delay_ms(1000);

        move_servo(180);
        _delay_ms(1000);
    }

    return 0;
}
