#include <LiquidCrystal.h>

#define MOTOR_EN 10        // Motor enable pin for speed (PWM)
#define MOTOR_IN1 11       // Motor direction pin 1
#define MOTOR_IN2 12       // Motor direction pin 2

// Button pins
#define BTN_MENU 8
#define BTN_UP 9
#define BTN_DOWN 13
#define BTN_OK A0
#define BTN_BACK A1

// Limit switches for each floor
#define LIMIT_SWITCH_GND A2 // Ground floor limit switch
#define LIMIT_SWITCH_1 A3   // 1st floor limit switch

#define DOOR_SEFTY_PIN 18
// UART for communication with Interface Card
#define BAUD_RATE 115200

// Initialize the regular LCD (not I2C) with pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // (rs, enable, d4, d5, d6, d7)

// Define buzzer pin
#define BUZZER_PIN A5
unsigned long lastAReceivedTime = 0; // Track last time 'A' was received
const unsigned long BUZZER_TIMEOUT = 1000; // Timeout period in milliseconds

volatile bool stopRequested = false; // Flag for stop request
volatile bool floorMessageSent = false; // Flag to track if floor message has been sent

void setup() {
    Serial.begin(BAUD_RATE);
    pinMode(MOTOR_EN, OUTPUT);
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    
    pinMode(BTN_MENU, INPUT_PULLUP);
    pinMode(BTN_UP, INPUT_PULLUP);
    pinMode(BTN_DOWN, INPUT_PULLUP);
    pinMode(BTN_OK, INPUT_PULLUP);
    pinMode(BTN_BACK, INPUT_PULLUP);
    
    pinMode(LIMIT_SWITCH_GND, INPUT_PULLUP);
    pinMode(LIMIT_SWITCH_1, INPUT_PULLUP);
    pinMode(DOOR_SEFTY_PIN, INPUT_PULLUP);
    
    lcd.begin(16, 2);
    lcd.print("loading.....");
    delay(500);
    lcd.clear();
    
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);  
}

void moveMotorUp() {
    if (digitalRead(DOOR_SEFTY_PIN) == LOW) {
        digitalWrite(MOTOR_IN1, HIGH);
        digitalWrite(MOTOR_IN2, LOW);
        analogWrite(MOTOR_EN, 255);
        Serial.println("Motor moving up");
    }
}

void moveMotorDown() {
    if (digitalRead(DOOR_SEFTY_PIN) == LOW) {
        digitalWrite(MOTOR_IN1, LOW);
        digitalWrite(MOTOR_IN2, HIGH);
        analogWrite(MOTOR_EN, 255);
        Serial.println("Motor moving down");
    }
}

void stopMotor() {
    analogWrite(MOTOR_EN, 0);
    Serial.println("Motor stopped");
}

void moveToFloor(int targetFloor) {
    lcd.clear();
    //lcd.print("Moving to Floor ");
   // lcd.print(targetFloor);

    stopRequested = false;
    floorMessageSent = false;

    if (targetFloor == 0) {
        moveMotorDown();
        while (digitalRead(LIMIT_SWITCH_GND) == HIGH && !stopRequested) {
            // Check if the door safety pin goes LOW
            if (digitalRead(DOOR_SEFTY_PIN) == HIGH) {
                stopRequested = true;
                stopMotor();
                lcd.clear();
                lcd.print("Door Opened!");
                Serial.println("Motor stopped due to door safety trigger.");
                return;  // Exit the function to avoid further movement
            }

            // Check for stop command
            if (Serial.available()) {  
                char command = Serial.read();
                if (command == 'S') {
                    stopRequested = true;
                    stopMotor();
                    lcd.clear();
                    //lcd.print("Elevator Stopped");
                    return;
                }
            }
            delay(50);
        }
    } else if (targetFloor == 1) {
        moveMotorUp();  
        while (digitalRead(LIMIT_SWITCH_1) == HIGH && !stopRequested) {
            // Check if the door safety pin goes LOW
            if (digitalRead(DOOR_SEFTY_PIN) == HIGH) {
                stopRequested = true;
                stopMotor();
                lcd.clear();
                //lcd.print("Door Opened!");
                Serial.println("Motor stopped due to door safety trigger.");
                return;  // Exit the function to avoid further movement
            }

            // Check for stop command
            if (Serial.available()) {
                char command = Serial.read();
                if (command == 'S') {
                    stopRequested = true;
                    stopMotor();
                    lcd.clear();
                    //lcd.print("Elevator Stopped");
                    return;
                }
            }
            delay(50);
        }
    }

    stopMotor();
    lcd.clear();
   // lcd.print("Reached Floor ");
   // lcd.print(targetFloor);

    if (!floorMessageSent) {
        Serial.print("Elevator arrived at Floor ");
        Serial.println(targetFloor);
        floorMessageSent = true;
    }

    delay(1000);
}

void openMenu() {
    lcd.clear();
    lcd.print("Menu Opened");
    delay(500);
    bool inMenu = true;

    while (inMenu) {
        lcd.setCursor(0, 1);  // Start display at the beginning of the second row
        lcd.print("Select Option");

        if (digitalRead(BTN_UP) == LOW) {
            lcd.clear();
            lcd.print("Going Up");
            moveMotorUp();
            while (digitalRead(BTN_UP) == LOW) { delay(10); }
            stopMotor();
            lcd.clear();
            lcd.print("Menu Opened");
            delay(500);
        }

        if (digitalRead(BTN_DOWN) == LOW) {
            lcd.clear();
            lcd.print("Going Down");
            moveMotorDown();
            while (digitalRead(BTN_DOWN) == LOW) { delay(10); }
            stopMotor();
            lcd.clear();
            lcd.print("Menu Opened");
            delay(500);
        }

        if (digitalRead(BTN_OK) == LOW) {
            lcd.clear();
            lcd.print("Additional Mode");
            delay(500);
            lcd.clear();
            lcd.print("Menu Opened");
        }

        if (digitalRead(BTN_BACK) == LOW) {
            lcd.clear();
            lcd.print("Exiting Menu");
            delay(500);
            inMenu = false;
            lcd.clear();
        }

        delay(100);  // Small delay to stabilize button reads and avoid rapid looping
    }
}

void loop() {
    if (Serial.available()) {
        char command = Serial.read();
        if (command == '0') {
            moveToFloor(0);
        } else if (command == '1') {
            moveToFloor(1);
        } else if (command == 'W') {
            lcd.clear();
            //lcd.print("Alarm Activated");
            digitalWrite(BUZZER_PIN, HIGH);
            lastAReceivedTime = millis();
            delay(500);
            lcd.clear();
        } else if (command == 'S') {
            stopRequested = true;
            stopMotor();
            lcd.clear();
           // lcd.print("Elevator Stopped");
            delay(500);
            lcd.clear();
        } else {
            digitalWrite(BUZZER_PIN, LOW);
        }
    }

    if (millis() - lastAReceivedTime > BUZZER_TIMEOUT) {
        digitalWrite(BUZZER_PIN, LOW);
    }

    if (digitalRead(BTN_MENU) == LOW) {
        openMenu();
    }

    delay(70);
}
