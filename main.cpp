/*
KiTSUNE Speed shutter Tester Beta V0.1 [Build test]
By REDWOLF

used ATMEGA328P ONLY!!
Libraries requirement
  - Adafruit BusIO
  - Adafruit GFX Library
  - Adafruit SSD1306
*/

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include <Wire.h>
#include <SPI.h>

uint8_t resetButt = 3, upButt = 4, downButt = 5;

unsigned long StartTime, StopTime, DeltaTime;
float ShutterTime1,ShutterTime2;

boolean SettingPage;
volatile boolean shutterFlag = false, signalReading = false;

void setup() {
  Serial.begin(9600);

  //Power_control -> on
  DDRD |= (1 << DDD6); //Set PD6 to output
  PORTD |= (1 << PD6); //PD6 HIGH


  //Pin setup
  //interrupts INT0
  DDRD &= ~(1 << DDD2);     //Set PD2 pin  to INPUT
  PORTD |= (1 << PORTD2);   //Enable pull-up
  

  //Interrupts setup 
  EICRA |= (1 << ISC00);    //Set INT0 to trigger on any logic change
  EIMSK |= (1 << INT0);     //Enable INT0
  sei();                    //Enable Interrupts


  pinMode(resetButt, INPUT_PULLUP);   // Reset | Enter
  pinMode(upButt, INPUT_PULLUP);      // Up ⬆
  pinMode(downButt, INPUT_PULLUP);    // Down ⬇

}//Void setup

void loop() {

  //Setting
  while(digitalRead(resetButt) == LOW) {
    uint16_t counter = millis();
    if(counter >= 2000) {
      SettingPage = !SettingPage;
    }
  }

  if(SettingPage == true) {
    //Display setting page
    uint8_t Page = 0;
    if(digitalRead(downButt) == LOW) {
      Page++;
      delay(5);
    }
    if(digitalRead(upButt) == LOW) {
      Page--;
      delay(5);
    }

    if(Page > 1 || Page == 255) {
        Page = 0;
    }
    
    //scene
    switch (Page) {
      //Auto shuttdown time
      case 0:
      break;
      //Dim display
      case 1:
      break;
    }
  }

  //Reset Value
  if(digitalRead(3) == LOW) {
    //RESET logic and value
    shutterFlag = false;
    signalReading = false;
    DeltaTime = 0;
    StartTime = 0;
    StopTime = 0;
    ShutterTime1 = 0;
    ShutterTime2 = 0;
  }

  if(signalReading == true) {

    DeltaTime = (StopTime - StartTime);
    ShutterTime1 = (float)DeltaTime/1000000;
    ShutterTime2 = 1/ShutterTime1;
    

    //RESET logic and value
    shutterFlag = false;
    signalReading = false;
    DeltaTime = 0;
    StartTime = 0;
    StopTime = 0;
    ShutterTime1 = 0;
    ShutterTime2 = 0;
  }

}//Void Loop


ISR (INT0_vect) {
  // interrupts code
  if(shutterFlag == false) {
    StartTime = micros();
    shutterFlag = true;
  } else{
    StopTime = micros();
    signalReading = true;
    
  }
  
}//ISR
