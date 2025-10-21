// #include <MAX30100.h>
// #include <MAX30100_BeatDetector.h>
// #include <MAX30100_Filters.h>
// #include <MAX30100_PulseOximeter.h>
// #include <MAX30100_Registers.h>
// #include <MAX30100_SpO2Calculator.h>
// #include <Wire.h>

// #define REPORTING_PERIOD_MS     500

// PulseOximeter pox;

// const int numReadings=10;
// float filterweight=0.5;
// uint32_t tsLastReport = 0;
// uint32_t last_beat=0;
// int readIndex=0;
// int average_beat=0;
// int average_SpO2=0;
// bool calculation_complete=false;
// bool calculating=false;
// bool initialized=false;
// byte beat=0;


// void onBeatDetected() //Calls back when pulse is detected
// {
//   viewBeat();
//   last_beat=millis();
// }

// void viewBeat() 
// {

//   if (beat==0) {
//    Serial.print("_");
//     beat=1;
//   } 
//   else
//   {
//    Serial.print("^");
//     beat=0;
//   }
// }

// void initial_display() 
// {
//   if (not initialized) 
//   {
//     viewBeat();
//   Serial.print(" MAX30100 Pulse Oximeter Test");
//   Serial.println("******************************************");
//     Serial.println("Place place your finger on the sensor");
//   Serial.println("********************************************");  
//     initialized=true;
//   }
// }

// void display_calculating(int j){

//   viewBeat();
//   Serial.println("Measuring"); 
//   for (int i=0;i<=j;i++) {
//     Serial.print(". ");
//   }
// }

// void display_values()
// {
//   Serial.print(average_beat);
//   Serial.print("| Bpm ");
//   Serial.print("| SpO2 ");
//   Serial.print(average_SpO2);
//   Serial.print("%"); 
// }

// void calculate_average(int beat, int SpO2) 
// {
//   if (readIndex==numReadings) {
//     calculation_complete=true;
//     calculating=false;
//     initialized=false;
//     readIndex=0;
//     display_values();
//   }
  
//   if (not calculation_complete and beat>30 and beat<220 and SpO2>50) {
//     average_beat = filterweight * (beat) + (1 - filterweight ) * average_beat;
//     average_SpO2 = filterweight * (SpO2) + (1 - filterweight ) * average_SpO2;
//     readIndex++;
//     display_calculating(readIndex);
//   }
// }

// void setup()
// {
//     Serial.begin(115200);
//     pox.begin();
//     pox.setOnBeatDetectedCallback(onBeatDetected);
// }


// void loop()
// {
    
//     pox.update(); 
//     if ((millis() - tsLastReport > REPORTING_PERIOD_MS) and (not calculation_complete)) {
//         calculate_average(pox.getHeartRate(),pox.getSpO2());
//         tsLastReport = millis();
//     }
//     if ((millis()-last_beat>10000)) {
//       calculation_complete=false;
//       average_beat=0;
//       average_SpO2=0;
//       initial_display();
//     }
// }
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx;
PulseOximeter pox;

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C bus first
  Wire.begin();
  Wire.setClock(100000); // Standard 100kHz I2C speed

  // Initialize MLX90614 with error handling
  if(!mlx.begin()) {
    Serial.println("MLX90614 init failed. Retrying...");
    delay(1000);
    if(!mlx.begin()) {
      Serial.println("MLX90614 not responding. Check wiring.");
      while(1);
    }
  }

  // Initialize MAX30100 with error handling
  if(!pox.begin()) {
    Serial.println("MAX30100 init failed. Retrying...");
    delay(1000);
    if(!pox.begin()) {
      Serial.println("MAX30100 not responding. Check wiring.");
      while(1);
    }
  }

  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  pox.update();
  
  static uint32_t lastReport = 0;
  if(millis() - lastReport >= 1000) {
    float ambient = mlx.readAmbientTempC();
    float object = mlx.readObjectTempC();
    
    // Check for NaN values
    if(isnan(ambient) || isnan(object)) {
      Serial.println("MLX90614 read error!");
      mlx.begin(); // Reinitialize sensor
    } else {
      Serial.print("Ambient: "); Serial.print(ambient); Serial.print("°C");
      Serial.print(" | Object: "); Serial.print(object); Serial.print("°C");
      Serial.print(" | HR: "); Serial.print(pox.getHeartRate()); Serial.print("bpm");
      Serial.print(" | SpO2: "); Serial.print(pox.getSpO2()); Serial.println("%");
    }
    
    lastReport = millis();
  }
}

void onBeatDetected() {
  Serial.println("♥ Beat!");
}