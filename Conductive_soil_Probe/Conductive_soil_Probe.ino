/*
  ~~~~~~~~~~~~~~~~~~~~~~~~~~ Conductive Soil probe connected to a naked ATmega328P chip ~~~~~~~~~~~~~~~~~~~~~~

  This code allows the ATmega328P to sleep in power down mode to extend battery life, After an alloted amount of time checked by a while statment
  and a sleep count an led will blink, when the wake up time is reached a watchdog interupt will wake the chip and enable a transistor connected to the
  soil probe (SensorPowerPin) to power the sensor,then wait 400th of a second for the reading to stablise then determine what to do next.
  If the soil is dry (PowerDownDisable) will stop the sleep routine enable the pump transistor (PumpPin) to run the pump to water the soil
  until the soil is wet again then shut down the pump enable sleep and run the sequence all over again.
  Ref# https://github.com/RalphBacon/Arduino-Deep-Sleep
  Ref# https://wiki.dfrobot.com/Capacitive_Soil_Moisture_Sensor_SKU_SEN0193

  Please See attached wiring Schymatic!
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  SOIL PROBE VR1 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "Arduino.h"
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <OneWire.h>              // enable single sensor Lib

const int ledRed = 3;        // output pin for the LED
const int ledGreen = 5;        // output pin for the LED
const int ledBlue = 6;        // output pin for the LED
const int PumpPin = 8;       // output to realy to control water pump
const int SensorPowerPin = 9;
int PowerDownDisable = 0;
volatile char sleepCnt = 8;  // makes the arduino sleep for ex amount of seconds 8 max
const int AirValue = 625;    //you need to replace this value with Value_1
const int WaterValue = 300;  //you need to replace this value with Value_2
int intervals = (AirValue - WaterValue) / 3;
int soilMoistureValue = 0;
int ReadSensorDelay = 0;
int delayTime;
int previousedelayTime = 0;
int ledFadePeriod = 2000;
long LEDtime;
int ledVal = 0;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SETUP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void setup() {
 // Serial.begin(9600);        // open serial port, set the baud rate to 9600 bps
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(PumpPin, OUTPUT);
  digitalWrite(PumpPin, LOW);
  pinMode(SensorPowerPin, OUTPUT);
  digitalWrite(SensorPowerPin, LOW);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ LOOP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void loop() {
  Debug();
  LEDtime = millis();
  delayTime = millis();

  if (delayTime - previousedelayTime >= 400) {
    previousedelayTime = delayTime;
    ReadSensorDelay = 0;
  }
  else {
    ReadSensorDelay = 1;
    digitalWrite (SensorPowerPin, HIGH);
    Serial.println("READ SENSORS");
  }

  if (PowerDownDisable == 0 && ReadSensorDelay == 0) {
    digitalWrite (SensorPowerPin, LOW);
    digitalWrite(ledGreen, LOW);    
    Sleeping();
  }

  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil

  if (soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals))                   // dry
  {
    digitalWrite (PumpPin, LOW);
    digitalWrite (ledBlue, LOW);
    digitalWrite (ledRed, LOW);
    PowerDownDisable = 0;
  }
  else if (soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (AirValue - intervals)) // moist
  {
    ledBlueFade();
    digitalWrite (ledRed, LOW);
    PowerDownDisable = 1;
  }
  else if (soilMoistureValue < AirValue && soilMoistureValue > (AirValue - intervals))                // Wet
  {
    PowerDownDisable = 1;
    digitalWrite (PumpPin, HIGH);
    digitalWrite (ledBlue, LOW);
    ledRedFade();
  }
}
/*
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Examples ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  The intervals equal air value minus water value times by 3 in this case == 93
  Dry: (585 492]
  Wet: (492 399]
  Very Wet: (399 306]
*/
