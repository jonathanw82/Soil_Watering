<h1 align="center">
   <img src="static/images/.jpg" alt="logo image"/>
 </h1>
 
# Soil_Watering

These Files make up a autowatering system 
The code allows the ATmega328P to sleep in power down mode to extend battery life, 
After an alloted amount of time checked by a while statment and a sleep count an led will blink, when the wake up time is reached a 
watchdog interupt will wake the chip and enable a transistor connected to the soil probe (SensorPowerPin) to power the sensor,
then wait 400th of a second for the reading to stablise then determine what to do next.
If the soil is dry (PowerDownDisable) will stop the sleep routine enable the pump transistor (PumpPin)
to run the pump to water the soil until the soil is wet again then shut down the pump enable sleep and run the sequence all over again.

Reference:-
Ref# https://github.com/RalphBacon/Arduino-Deep-Sleep
Ref# https://wiki.dfrobot.com/Capacitive_Soil_Moisture_Sensor_SKU_SEN0193
