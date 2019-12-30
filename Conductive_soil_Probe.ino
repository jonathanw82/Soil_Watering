/*
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Conductive Soil probe connected to a naked ATmega328P chip ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  This code allows the ATmega328P to sleep in power down mode to extend battery life, After an alloted amount of time checked by a while statment
  and a sleep count an led will blink, when the wake up time is reached a watchdog interupt will wake the chip and enable a transistor connected to the
  soil probe (SensorPowerPin) to power the sensor,then wait 400th of a second for the reading to stablise then determine what to do next.
  If the soil is dry (PowerDownDisable) will stop the sleep routine enable the pump transistor (PumpPin) to run the pump to water the soil
  until the soil is wet again then shut down the pump enable sleep and run the sequence all over again.
  Ref# https://github.com/RalphBacon/Arduino-Deep-Sleep
  Ref# https://wiki.dfrobot.com/Capacitive_Soil_Moisture_Sensor_SKU_SEN0193

  Please See attached wiring Schymatic!
*/
#include "Arduino.h"
#include <avr/sleep.h>
#include <avr/wdt.h>

const int ledPin = 2;        // output pin for the LED (to show it is awake)
const int PumpPin = 5;       // output to realy to control water pump
const int SensorPowerPin = 6;
int PowerDownDisable = 0;
volatile char sleepCnt = 8;  // makes the arduino sleep for ex amount of seconds 8 max
const int AirValue = 592;    //you need to replace this value with Value_1
const int WaterValue = 306;  //you need to replace this value with Value_2
int intervals = (AirValue - WaterValue) / 3;
int soilMoistureValue = 0;
int ReadSensorDelay = 0;
int delayTime = 0;
int previousedelayTime = 0;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SETUP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void setup() {
  Serial.begin(9600);        // open serial port, set the baud rate to 9600 bps
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  pinMode(PumpPin, OUTPUT);
  digitalWrite(PumpPin, LOW);
  pinMode(SensorPowerPin, OUTPUT);
  digitalWrite(SensorPowerPin, LOW);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ LOOP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void loop() {
  Debug();
  delayTime = millis();

  if (delayTime - previousedelayTime >= 400) {
    previousedelayTime = delayTime;
    ReadSensorDelay = 0;
  }
  else {
    ReadSensorDelay = 1;
    digitalWrite (SensorPowerPin, HIGH);
    Serial.println("READ SENSORS");
    digitalWrite (ledPin, LOW);
  }

  if (PowerDownDisable == 0 && ReadSensorDelay == 0) {
    digitalWrite (SensorPowerPin, LOW);
    Sleeping();
  }

  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil

  if (soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals))
  {
    digitalWrite (PumpPin, LOW);
    Serial.println("Very Wet");
    PowerDownDisable = 0;
  }
  else if (soilMoistureValue < AirValue && soilMoistureValue > (AirValue - intervals))
  {
    PowerDownDisable = 1;
    digitalWrite (PumpPin, HIGH);
    Serial.println("Dry");
  }
}
/*
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Examples ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  The intervals equal air value minus water value times by 3 in this case == 93
  Dry: (585 492]
  Wet: (492 399]
  Very Wet: (399 306]
*/
