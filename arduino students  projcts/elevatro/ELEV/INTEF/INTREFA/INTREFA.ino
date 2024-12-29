// // Define BCD input pins for 74LS47 decoder
// #define BCD_A_PIN 2
// #define BCD_B_PIN 3
// #define BCD_C_PIN 4
// #define BCD_D_PIN 5

// // Define switch pins
// #define FLOOR0_SWITCH_PIN 6
// #define FLOOR1_SWITCH_PIN 7
// #define ALARM_SWITCH_PIN 8
// #define STOP_SWITCH_PIN 9
// #define CABINET1_SWITCH_PIN 10
// #define CABINET2_SWITCH_PIN 11

// // Define LED pins
// #define LED_PIN_0 12
// #define LED_PIN_1 13
// #define LED_PIN_ALARM 14
// #define LED_PIN_STOP 15
// #define LED_PIN_CABINET1 16
// #define LED_PIN_CABINET2 17

// // Variables to track previous states of switches
// bool prevFloor0State = LOW;
// bool prevFloor1State = LOW;
// bool floor1Sent = false; // Flag to track if FLOOR1 was sent
// bool prevAlarmState = LOW; // Initial state with INPUT_PULLUP
// bool prevStopState = LOW;
// bool prevCabinet1State = LOW;
// bool prevCabinet2State = LOW;

// // Variable to track the current floor
// int currentFloor = -1;  // -1 indicates no floor, 0 is floor 0, 1 is floor 1

// // Timer variables for LED actions
// unsigned long ledTimers[] = {0, 0, 0, 0, 0, 0}; // Array to track timers for each LED
// const unsigned long ledDuration = 2000; // Duration (2 seconds) in milliseconds

// void setup() {
//   // Initialize BCD input pins for 74LS47 decoder
//   pinMode(BCD_A_PIN, OUTPUT);
//   pinMode(BCD_B_PIN, OUTPUT);
//   pinMode(BCD_C_PIN, OUTPUT);
//   pinMode(BCD_D_PIN, OUTPUT);

//   // Initialize switch pins with pull-up resistor for the alarm pin
//   pinMode(FLOOR0_SWITCH_PIN, INPUT);
//   pinMode(FLOOR1_SWITCH_PIN, INPUT);
//   pinMode(ALARM_SWITCH_PIN, INPUT_PULLUP);  // Enable pull-up resistor for alarm
//   pinMode(STOP_SWITCH_PIN, INPUT);
//   pinMode(CABINET1_SWITCH_PIN, INPUT);
//   pinMode(CABINET2_SWITCH_PIN, INPUT);

//   // Initialize LED pins
//   pinMode(LED_PIN_0, OUTPUT);
//   pinMode(LED_PIN_1, OUTPUT);
//   pinMode(LED_PIN_ALARM, OUTPUT);
//   pinMode(LED_PIN_STOP, OUTPUT);
//   pinMode(LED_PIN_CABINET1, OUTPUT);
//   pinMode(LED_PIN_CABINET2, OUTPUT);

//   Serial.begin(115200);  // Start serial communication at a higher baud rate

//   // Set initial LED states: all LEDs off except for LED on pin 15 (STOP LED)
//   digitalWrite(LED_PIN_0, LOW);
//   digitalWrite(LED_PIN_1, LOW);
//   digitalWrite(LED_PIN_ALARM, LOW);
//   digitalWrite(LED_PIN_STOP, HIGH);  // Keep STOP LED on by default
//   digitalWrite(LED_PIN_CABINET1, LOW);
//   digitalWrite(LED_PIN_CABINET2, LOW);
// }

// // void loop() {
// //   // Read current switch states
// //   bool floor0Pressed = digitalRead(FLOOR0_SWITCH_PIN) == LOW;
// //   bool floor1Pressed = digitalRead(FLOOR1_SWITCH_PIN) == LOW;
// //   bool alarmPressed = digitalRead(ALARM_SWITCH_PIN) == LOW;
// //   bool stopPressed = digitalRead(STOP_SWITCH_PIN) == HIGH;
// //   bool cabinet1Pressed = digitalRead(CABINET1_SWITCH_PIN) == LOW;
// //   bool cabinet2Pressed = digitalRead(CABINET2_SWITCH_PIN) == LOW;

// //   // Send switch status to control card via UART once per button press
// //   if (floor0Pressed && !prevFloor0State) {
// //     Serial.println("FLOOR0");
// //     ledTimers[0] = millis(); // Activate LED timer for FLOOR0 LED (pin 12)
// //   }
// //   if (floor1Pressed && !prevFloor1State && !floor1Sent) {
// //     Serial.println("FLOOR1");
// //     floor1Sent = true; // Set flag to prevent repeated sending
// //     ledTimers[1] = millis(); // Activate LED timer for FLOOR1 LED (pin 13)
// //   }
// //   if (!floor1Pressed) {
// //     floor1Sent = false; // Reset flag when button is released
// //   }
// //   if (alarmPressed && !prevAlarmState) {
// //     Serial.println('A');
// //     ledTimers[2] = millis(); // Activate LED timer for ALARM LED (pin 14)
// //   }
// //   if (stopPressed && !prevStopState) {
// //     Serial.println('S');
// //     ledTimers[3] = millis(); // Activate LED timer for STOP LED (pin 15)
// //   }
// //   if (cabinet1Pressed && !prevCabinet1State) {
// //     Serial.println("CABINET1");
// //     ledTimers[4] = millis(); // Activate LED timer for CABINET1 LED (pin 16)
// //   }
// //   if (cabinet2Pressed && !prevCabinet2State) {
// //     Serial.println("CABINET2");
// //     ledTimers[5] = millis(); // Activate LED timer for CABINET2 LED (pin 17)
// //   }

// //   // Update previous states
// //   prevFloor0State = floor0Pressed;
// //   prevFloor1State = floor1Pressed;
// //   prevAlarmState = alarmPressed;
// //   prevStopState = stopPressed;
// //   prevCabinet1State = cabinet1Pressed;
// //   prevCabinet2State = cabinet2Pressed;

// //   // Update LED states based on timers (turn on LEDs for 2 seconds)
// //   digitalWrite(LED_PIN_0, (millis() - ledTimers[0] < ledDuration) ? HIGH : LOW);
// //   digitalWrite(LED_PIN_1, (millis() - ledTimers[1] < ledDuration) ? HIGH : LOW);
// //   digitalWrite(LED_PIN_ALARM, (millis() - ledTimers[2] < ledDuration) ? HIGH : LOW);
// //   digitalWrite(LED_PIN_CABINET1, (millis() - ledTimers[4] < ledDuration) ? HIGH : LOW);
// //   digitalWrite(LED_PIN_CABINET2, (millis() - ledTimers[5] < ledDuration) ? HIGH : LOW);
  
// //   // Ensure STOP LED (pin 15) remains on unless stop button is pressed
// //   digitalWrite(LED_PIN_STOP, HIGH);

// //   // Check for UART message indicating elevator arrival at a floor
// //   if (Serial.available()) {
// //     char received = Serial.read();  // Read the incoming byte
    
// //     if (received == '0') {
// //       currentFloor = 0;  // Elevator arrived at floor 0
// //     } else if (received == '1') {
// //       currentFloor = 1;  // Elevator arrived at floor 1
// //     }
// //     updateSevenSegment(currentFloor);  // Update display immediately after receiving UART message
// //   }
// // }
// void loop() {
//   // Read current switch states
//   bool floor0Pressed = digitalRead(FLOOR0_SWITCH_PIN) == LOW;
//   bool floor1Pressed = digitalRead(FLOOR1_SWITCH_PIN) == LOW;
//   bool alarmPressed = digitalRead(ALARM_SWITCH_PIN) == LOW;
//   bool stopPressed = digitalRead(STOP_SWITCH_PIN) == HIGH;  // Check pin 9 state
//   bool cabinet1Pressed = digitalRead(CABINET1_SWITCH_PIN) == LOW;
//   bool cabinet2Pressed = digitalRead(CABINET2_SWITCH_PIN) == LOW;

//   // Send switch status to control card via UART once per button press
//   if (floor0Pressed && !prevFloor0State) {
//     Serial.println("FLOOR0");
//     ledTimers[0] = millis(); // Activate LED timer for FLOOR0 LED (pin 12)
//   }
//   if (floor1Pressed && !prevFloor1State && !floor1Sent) {
//     Serial.println("FLOOR1");
//     floor1Sent = true; // Set flag to prevent repeated sending
//     ledTimers[1] = millis(); // Activate LED timer for FLOOR1 LED (pin 13)
//   }
//   if (!floor1Pressed) {
//     floor1Sent = false; // Reset flag when button is released
//   }
//   if (alarmPressed && !prevAlarmState) {
//     Serial.println('A');
//     ledTimers[2] = millis(); // Activate LED timer for ALARM LED (pin 14)
//   }
//   if (stopPressed && !prevStopState) {
//     Serial.println('S');
//     ledTimers[3] = millis(); // Activate LED timer for STOP LED (pin 15)
//   }
//   if (cabinet1Pressed && !prevCabinet1State) {
//     Serial.println("CABINET1");
//     ledTimers[4] = millis(); // Activate LED timer for CABINET1 LED (pin 16)
//   }
//   if (cabinet2Pressed && !prevCabinet2State) {
//     Serial.println("CABINET2");
//     ledTimers[5] = millis(); // Activate LED timer for CABINET2 LED (pin 17)
//   }

//   // Update previous states
//   prevFloor0State = floor0Pressed;
//   prevFloor1State = floor1Pressed;
//   prevAlarmState = alarmPressed;
//   prevStopState = stopPressed;
//   prevCabinet1State = cabinet1Pressed;
//   prevCabinet2State = cabinet2Pressed;

//   // Update LED states based on timers (turn on LEDs for 2 seconds)
//   digitalWrite(LED_PIN_0, (millis() - ledTimers[0] < ledDuration) ? HIGH : LOW);
//   digitalWrite(LED_PIN_1, (millis() - ledTimers[1] < ledDuration) ? HIGH : LOW);
//   digitalWrite(LED_PIN_ALARM, (millis() - ledTimers[2] < ledDuration) ? HIGH : LOW);
//   digitalWrite(LED_PIN_CABINET1, (millis() - ledTimers[4] < ledDuration) ? HIGH : LOW);
//   digitalWrite(LED_PIN_CABINET2, (millis() - ledTimers[5] < ledDuration) ? HIGH : LOW);

//   // Control LED on pin 15 based on pin 9 state
//   digitalWrite(LED_PIN_STOP, stopPressed ? HIGH : LOW);

//   // Check for UART message indicating elevator arrival at a floor
//   if (Serial.available()) {
//     char received = Serial.read();  // Read the incoming byte
    
//     if (received == '0') {
//       currentFloor = 0;  // Elevator arrived at floor 0
//     } else if (received == '1') {
//       currentFloor = 1;  // Elevator arrived at floor 1
//     }
//     updateSevenSegment(currentFloor);  // Update display immediately after receiving UART message
//   }
// }


// void updateSevenSegment(int floor) {
//   // Map the floor number to a BCD value (0 for floor 0, 1 for floor 1)
//   byte bcdValue = floor; // For floor 0, bcdValue = 0; for floor 1, bcdValue = 1

//   // Send the BCD value to the 74LS47 decoder (4-bit input)
//   digitalWrite(BCD_A_PIN, (bcdValue & 0x01) >> 0);  // LSB (A)
//   digitalWrite(BCD_B_PIN, (bcdValue & 0x02) >> 1);  // 2nd bit (B)
//   digitalWrite(BCD_C_PIN, (bcdValue & 0x04) >> 2);  // 3rd bit (C)
//   digitalWrite(BCD_D_PIN, (bcdValue & 0x08) >> 3);  // MSB (D)
// }

// Define BCD input pins for 74LS47 decoder
#define BCD_A_PIN 2
#define BCD_B_PIN 3
#define BCD_C_PIN 4
#define BCD_D_PIN 5

// Define switch pins
#define FLOOR0_SWITCH_PIN 6
#define FLOOR1_SWITCH_PIN 7
#define ALARM_SWITCH_PIN 8
#define STOP_SWITCH_PIN 9
#define CABINET1_SWITCH_PIN 10
#define CABINET2_SWITCH_PIN 11

// Define LED pins
#define LED_PIN_0 12
#define LED_PIN_1 13
#define LED_PIN_ALARM 14
#define LED_PIN_STOP 15
#define LED_PIN_CABINET1 16
#define LED_PIN_CABINET2 17

// Variables to track previous states of switches
bool prevFloor0State = LOW;
bool prevFloor1State = LOW;
bool floor1Sent = false; // Flag to track if FLOOR1 was sent
bool prevAlarmState = LOW; // Initial state with INPUT_PULLUP
bool prevStopState = LOW;
bool prevCabinet1State = LOW;
bool prevCabinet2State = LOW;

// Variable to track the current floor
int currentFloor = -1;  // -1 indicates no floor, 0 is floor 0, 1 is floor 1

// Timer variables for LED actions
unsigned long ledTimers[] = {0, 0, 0, 0, 0, 0}; // Array to track timers for each LED
const unsigned long ledDuration = 2000; // Duration (2 seconds) in milliseconds

void setup() {
  // Initialize BCD input pins for 74LS47 decoder
  pinMode(BCD_A_PIN, OUTPUT);
  pinMode(BCD_B_PIN, OUTPUT);
  pinMode(BCD_C_PIN, OUTPUT);
  pinMode(BCD_D_PIN, OUTPUT);

  // Initialize switch pins with pull-up resistor for the alarm pin
  pinMode(FLOOR0_SWITCH_PIN, INPUT);
  pinMode(FLOOR1_SWITCH_PIN, INPUT);
  pinMode(ALARM_SWITCH_PIN, INPUT_PULLUP);  // Enable pull-up resistor for alarm
  pinMode(STOP_SWITCH_PIN, INPUT);
  pinMode(CABINET1_SWITCH_PIN, INPUT);
  pinMode(CABINET2_SWITCH_PIN, INPUT);

  // Initialize LED pins
  pinMode(LED_PIN_0, OUTPUT);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_ALARM, OUTPUT);
  pinMode(LED_PIN_STOP, OUTPUT);
  pinMode(LED_PIN_CABINET1, OUTPUT);
  pinMode(LED_PIN_CABINET2, OUTPUT);

  Serial.begin(115200);  // Start serial communication at a higher baud rate

  // Set initial LED states: all LEDs off except for LED on pin 15 (STOP LED)
  digitalWrite(LED_PIN_0, LOW);
  digitalWrite(LED_PIN_1, LOW);
  digitalWrite(LED_PIN_ALARM, LOW);
  digitalWrite(LED_PIN_STOP, HIGH);  // Keep STOP LED on by default
  digitalWrite(LED_PIN_CABINET1, LOW);
  digitalWrite(LED_PIN_CABINET2, LOW);
}

void loop() {
  // Read current switch states
  bool floor0Pressed = digitalRead(FLOOR0_SWITCH_PIN) == LOW;
  bool floor1Pressed = digitalRead(FLOOR1_SWITCH_PIN) == LOW;
  bool alarmPressed = digitalRead(ALARM_SWITCH_PIN) == LOW;
  bool stopPressed = digitalRead(STOP_SWITCH_PIN) == HIGH;  // Check pin 9 state
  bool cabinet1Pressed = digitalRead(CABINET1_SWITCH_PIN) == LOW;
  bool cabinet2Pressed = digitalRead(CABINET2_SWITCH_PIN) == LOW;

  // Debugging: Print out which switch is pressed
  if (alarmPressed && !prevAlarmState) {
    Serial.println("ALARM Button Pressed");
    ledTimers[2] = millis(); // Activate LED timer for ALARM LED (pin 14)
  }
  if (cabinet1Pressed && !prevCabinet1State) {
    Serial.println("CABINET1 Button Pressed");
    ledTimers[4] = millis(); // Activate LED timer for CABINET1 LED (pin 16)
  }

  // Send switch status to control card via UART once per button press
  if (floor0Pressed && !prevFloor0State) {
    Serial.println("FLOOR0");
    ledTimers[0] = millis(); // Activate LED timer for FLOOR0 LED (pin 12)
  }
  if (floor1Pressed && !prevFloor1State && !floor1Sent) {
    Serial.println("FLOOR1");
    floor1Sent = true; // Set flag to prevent repeated sending
    ledTimers[1] = millis(); // Activate LED timer for FLOOR1 LED (pin 13)
  }
  if (!floor1Pressed) {
    floor1Sent = false; // Reset flag when button is released
  }
  if (alarmPressed && !prevAlarmState) {
    Serial.println('W');
    ledTimers[2] = millis(); // Activate LED timer for ALARM LED (pin 14)
  }
  if (stopPressed && !prevStopState) {
    Serial.println('S');
    ledTimers[3] = millis(); // Activate LED timer for STOP LED (pin 15)
  }
  if (cabinet1Pressed && !prevCabinet1State) {
    Serial.println("CABINET1");
    ledTimers[4] = millis(); // Activate LED timer for CABINET1 LED (pin 16)
  }
  if (cabinet2Pressed && !prevCabinet2State) {
    Serial.println("CABINET2");
    ledTimers[5] = millis(); // Activate LED timer for CABINET2 LED (pin 17)
  }

  // Update previous states
  prevFloor0State = floor0Pressed;
  prevFloor1State = floor1Pressed;
  prevAlarmState = alarmPressed;
  prevStopState = stopPressed;
  prevCabinet1State = cabinet1Pressed;
  prevCabinet2State = cabinet2Pressed;

  // Update LED states based on timers (turn on LEDs for 2 seconds)
  digitalWrite(LED_PIN_0, (millis() - ledTimers[0] < ledDuration) ? HIGH : LOW);
  digitalWrite(LED_PIN_1, (millis() - ledTimers[1] < ledDuration) ? HIGH : LOW);
  digitalWrite(LED_PIN_ALARM, (millis() - ledTimers[2] < ledDuration) ? HIGH : LOW);
  digitalWrite(LED_PIN_CABINET1, (millis() - ledTimers[4] < ledDuration) ? HIGH : LOW);
  digitalWrite(LED_PIN_CABINET2, (millis() - ledTimers[5] < ledDuration) ? HIGH : LOW);

  // Control LED on pin 15 based on pin 9 state
  digitalWrite(LED_PIN_STOP, stopPressed ? HIGH : LOW);

  // Check for UART message indicating elevator arrival at a floor
  if (Serial.available()) {
    char received = Serial.read();  // Read the incoming byte
    
    if (received == '0') {
      currentFloor = 0;  // Elevator arrived at floor 0
    } else if (received == '1') {
      currentFloor = 1;  // Elevator arrived at floor 1
    }
    updateSevenSegment(currentFloor);  // Update display immediately after receiving UART message
  }
}

void updateSevenSegment(int floor) {
  // Map the floor number to a BCD value (0 for floor 0, 1 for floor 1)
  byte bcdValue = floor; // For floor 0, bcdValue = 0; for floor 1, bcdValue = 1

  // Send the BCD value to the 74LS47 decoder (4-bit input)
  digitalWrite(BCD_A_PIN, (bcdValue & 0x01) >> 0);  // LSB (A)
  digitalWrite(BCD_B_PIN, (bcdValue & 0x02) >> 1);  // 2nd bit (B)
  digitalWrite(BCD_C_PIN, (bcdValue & 0x04) >> 2);  // 3rd bit (C)
  digitalWrite(BCD_D_PIN, (bcdValue & 0x08) >> 3);  // MSB (D)
}
