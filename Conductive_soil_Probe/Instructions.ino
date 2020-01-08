

/*
*************************************************   Calibration code   ********************************************************

void setup() {
  Serial.begin(9600); // open serial port, set the baud rate as 9600 bps
}
void loop() {
  int val;
  val = analogRead(0); //connect sensor to Analog 0
  Serial.println(val); //print the value to serial port
  delay(100);
}
*******************************************************************************************************************************
Open the serial port monitor and set the baud rate to 9600

Record the sensor value when the probe is exposed to the air as "Value 1". This is the boundary value of dry soil “Humidity: 0%RH”

Take a cup of water and insert the probe into it no further than the red line in the diagram

Record the sensor value when the probe is exposed to the water as "Value 2". This is the boundary value of moist soil “Humidity: 100%RH”

Section Settings
The final output value is affected by probe insertion depth and how tight the soil packed around it is. We regard "value_1" as dry soil and "value_2" as soaked soil.
This is the sensor detection range. For example: Value_1 = 520; Value_2 = 260. The range will be divided into three sections: dry, wet, water. Their related values are:

Example
Dry: (520 430]
Wet: (430 350]
Water: (350 260]
*/
