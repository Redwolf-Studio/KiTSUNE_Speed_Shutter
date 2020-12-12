/*
KiTSUNE Speed shutter Tester Beta V0.1
By REDWOLF
*/

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

unsigned long StartTime, StopTime, DeltaTime;
float ShutterTime1,ShutterTime2;
volatile boolean shutterFlag = false, signalReading = false;

void setup() {
  Serial.begin(9600);

  //Pin setup
  //interrupts INT0
  DDRD &= ~(1 << DDD2);     //Set PD2 pin  to INPUT
  PORTD |= (1 << PORTD2);   //Enable pull-up
  //Pin 3 (Reset)
  DDRD &= ~(1 << DDD3);     //Set PD3 pin  to INPUT
  PORTD |= (1 << PORTD3);   //Enable pull-up

  //Interrupts setup 
  EICRA |= (1 << ISC00);    //Set INT0 to trigger on any logic change
  EIMSK |= (1 << INT0);     //Enable INT0
  sei();                    //Enable Interrupts

}//Void setup

void loop() {

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

    //Serial Debug
    Serial.println("---------------------------");
    
    Serial.print("Shutter : ");
    if(ShutterTime1 >= 1) {
      Serial.println(ShutterTime1, 5);
    }else if(ShutterTime1 < 1) {
      Serial.print("1/");
      Serial.println(ShutterTime2, 5);
    }



    Serial.println("\n");
    Serial.print("Start Time : ");
    Serial.println(StartTime);
    Serial.print("Stop Time  : ");
    Serial.println(StopTime);
    Serial.print("Microsec. : ");
    Serial.println(DeltaTime);
    Serial.print("Shutter Time : ");
    Serial.println(ShutterTime1, 5);
    Serial.print("Shutter 1/ : ");
    Serial.println(ShutterTime2, 5);
    Serial.println("---------------------------");    

    
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
