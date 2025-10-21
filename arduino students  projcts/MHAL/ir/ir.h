#ifndef IR_H
#define IR_H

#include <avr/io.h>

// Function to initialize the IR sensor
void IR_Init(void);

// Function to check if an object is detected
uint8_t IR_ObjectDetected(void);

#endif // IR_H
