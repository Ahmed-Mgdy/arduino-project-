#include <Wire.h>
#include <DS1302.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

// DS1302 pins
const int CE_PIN = 5;
const int IO_PIN = 6;
const int SCLK_PIN = 7;

DS1302 rtc(CE_PIN, IO_PIN, SCLK_PIN);

// LCD address: 0x20, size: 16x2
LiquidCrystal_I2C lcd(0x20, 16, 2);

// Button and Buzzer pins
const int OK_PIN = 8;
const int SWAP_PIN = 9;
const int BACK_PIN = 10;
const int BUZZER_PIN = 11;

// Alarm structure
struct Alarm {
  int hour;       // 1-12 for 12-hour format
  int minute;     // 0-59
  bool isPM;      // true = PM, false = AM
  bool active;    // true = alarm is on
};

// Max number of alarms
const int MAX_ALARMS = 5;

// EEPROM Address Mapping
const int EEPROM_ALARM_START = 0; // Starting EEPROM address for alarms
const int ALARM_SIZE = 4;         // Each alarm takes 4 bytes (hour, minute, isPM, active)

// Alarms array
Alarm alarms[MAX_ALARMS];
bool settingMode = false;
int currentAlarm = 0; // Index of the current alarm being edited
int settingStep = 0;  // 0: hour, 1: minute, 2: AM/PM, 3: ON/OFF
unsigned long blinkTimer = 0;
bool blinkState = true; // For blinking effect

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();
  
  rtc.halt(false);
  rtc.writeProtect(false);
  
  // Initialize pins
  pinMode(OK_PIN, INPUT_PULLUP);
  pinMode(SWAP_PIN, INPUT_PULLUP);
  pinMode(BACK_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  // Load alarms from EEPROM
  loadAlarmsFromEEPROM();

  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  if (settingMode) {
    handleSettingMode();
  } else {
    displayClock();
    checkAlarms();
  }
}

void displayClock() {
  Time t = rtc.time();
  
  // Convert to 12-hour format
  String period = (t.hr >= 12) ? "PM" : "AM";
  int hour = t.hr % 12;
  if (hour == 0) hour = 12;
  
  lcd.setCursor(0, 0);
  lcd.print("Time ");
  lcd.print(hour < 10 ? "0" : "");
  lcd.print(hour);
  lcd.print(":");
  lcd.print(t.min < 10 ? "0" : "");
  lcd.print(t.min);
  lcd.print(":");
  lcd.print(t.sec < 10 ? "0" : "");
  lcd.print(t.sec);
  lcd.print(" ");
  lcd.print(period);
  
  lcd.setCursor(0, 1);
  lcd.print("Date ");
  lcd.print(t.date < 10 ? "0" : "");
  lcd.print(t.date);
  lcd.print("/");
  lcd.print(t.mon < 10 ? "0" : "");
  lcd.print(t.mon);
  lcd.print("/");
  lcd.print(t.yr);

  // Enter setting mode
  if (digitalRead(OK_PIN) == LOW) {
    delay(200); // Debounce
    settingMode = true;
    lcd.clear();
    currentAlarm = 0;
    settingStep = 0;
  }
}

void checkAlarms() {
  Time t = rtc.time();
  // Convert RTC time to 12-hour format
  int hour = t.hr % 12;
  if (hour == 0) hour = 12;
  bool isPM = (t.hr >= 12);

  for (int i = 0; i < MAX_ALARMS; i++) {
    // Check if alarm is active and matches current time
    if (alarms[i].active && alarms[i].hour == hour && alarms[i].minute == t.min && alarms[i].isPM == isPM) {
      // Display alarm message
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Time for medicine!");
      
      // Activate the buzzer
      digitalWrite(BUZZER_PIN, HIGH);

      // Wait for the user to press the OFF button
      while (true) {
        if (digitalRead(BACK_PIN) == LOW) {
          // Debounce
          delay(200);
          // Turn off the buzzer
          digitalWrite(BUZZER_PIN, LOW);
          lcd.clear();
          break; // Exit the alarm loop
        }
      }
    }
  }
}

void handleSettingMode() {
  if (millis() - blinkTimer > 500) {
    blinkState = !blinkState;
    blinkTimer = millis();
  }
  
  lcd.setCursor(0, 0);
  lcd.print("Alarm ");
  lcd.print(currentAlarm + 1);
  lcd.setCursor(0, 1);
  
  // Display and blink the current setting step
  if (settingStep == 0 && blinkState) {
    lcd.print("  :");
  } else {
    lcd.print(alarms[currentAlarm].hour < 10 ? "0" : "");
    lcd.print(alarms[currentAlarm].hour);
    lcd.print(":");
  }

  if (settingStep == 1 && blinkState) {
    lcd.print("  ");
  } else {
    lcd.print(alarms[currentAlarm].minute < 10 ? "0" : "");
    lcd.print(alarms[currentAlarm].minute);
  }

  if (settingStep == 2 && blinkState) {
    lcd.print("    ");
  } else {
    lcd.print(alarms[currentAlarm].isPM ? " PM " : " AM ");
  }

  if (settingStep == 3 && blinkState) {
    lcd.print("    ");
  } else {
    lcd.print(alarms[currentAlarm].active ? "ON " : "OFF");
  }

  if (digitalRead(OK_PIN) == LOW) {
    delay(200);
    settingStep++;
    if (settingStep > 3) {
      settingStep = 0; // Completed all settings, move to next alarm
      currentAlarm++;
      if (currentAlarm >= MAX_ALARMS) {
        currentAlarm = 0; // Cycle back to first alarm
        settingMode = false;
        lcd.clear();
        return;
      }
    }
  }

  if (digitalRead(SWAP_PIN) == LOW) {
    delay(200);
    switch (settingStep) {
      case 0: alarms[currentAlarm].hour = (alarms[currentAlarm].hour % 12) + 1; break;
      case 1: alarms[currentAlarm].minute = (alarms[currentAlarm].minute + 1) % 60; break;
      case 2: alarms[currentAlarm].isPM = !alarms[currentAlarm].isPM; break;
      case 3: alarms[currentAlarm].active = !alarms[currentAlarm].active; break;
    }
  }

  if (digitalRead(BACK_PIN) == LOW) {
    delay(200);
    saveAlarmToEEPROM(currentAlarm);
    settingMode = false;
    lcd.clear();
  }
}

void saveAlarmToEEPROM(int alarmIndex) {
  int addr = EEPROM_ALARM_START + alarmIndex * ALARM_SIZE;
  EEPROM.update(addr, alarms[alarmIndex].hour);
  EEPROM.update(addr + 1, alarms[alarmIndex].minute);
  EEPROM.update(addr + 2, alarms[alarmIndex].isPM);
  EEPROM.update(addr + 3, alarms[alarmIndex].active);
}

void loadAlarmsFromEEPROM() {
  for (int i = 0; i < MAX_ALARMS; i++) {
    int addr = EEPROM_ALARM_START + i * ALARM_SIZE;
    alarms[i].hour = EEPROM.read(addr);
    alarms[i].minute = EEPROM.read(addr + 1);
    alarms[i].isPM = EEPROM.read(addr + 2);
    alarms[i].active = EEPROM.read(addr + 3);
    // Clear invalid data
    if (alarms[i].hour > 12 || alarms[i].minute > 59) {
      alarms[i] = {12, 0, false, false};
    }
  }
}
