#include "ir.h"
#include "ir_privet.h" 

void IR_Init(void) {
    // Initialize the sensor (if required)
    
    DDRA &= ~(1 << IR_SENSOR_PIN);
}

uint8_t IR_ObjectDetected(void) {

    // Read the IR sensor's output pin
    uint8_t sensorValue = PINA & (1 << IR_SENSOR_PIN);

    // Check if an object is detected (you may need to adjust the logic)
    if (sensorValue == 0) {
        return 1; // Object detected
    } else {
        return 0; // No object detected
    }
}


