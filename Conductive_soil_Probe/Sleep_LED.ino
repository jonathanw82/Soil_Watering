/*
    128+127 = 255 
    PI = 3.14
    2 * 3.14 /ledFadePeriod (2000) = 0.00314
    ledFadePeriod = 0.00314
    LEDtime = 1000
    cos(0.00314 * 1000) = -1
    (0.00314 * 1000) = 3.14
    128+127 * cos(0.00314 * 1000) = 255
    
    The cosine will be 1 at multiples of ledFadePeriod, and -1 at multiple-and-a-half of ledFadePeriod. 
    It will start at 1 (counting from when the arduino was last reset) and go to -1 at millis()=1000, and back to 1 at millis()=2000.
    2pi radians in a circle the circumference of a circle is 2pi times the radius.  
 */
 
void led() {
  digitalWrite(ledGreen, HIGH);        // I would not usually use delay() however i need the led to blink before the sleep cycle is re-enabled
  delay(200);
  digitalWrite(ledGreen, LOW);
  delay(10);
  digitalWrite(ledGreen, HIGH);
  delay(200);
  digitalWrite(ledGreen, LOW);
}
void ledRedFade() {
  ledVal = 128 + 127 * cos(2 * PI / ledFadePeriod * LEDtime);                          
  analogWrite(ledRed, ledVal);           // sets the value (range from 0 to 255)      
}                                                                                     
void ledBlueFade() {                                                                   
  ledVal = 128 + 127 * cos(2 * PI / ledFadePeriod * LEDtime);
  analogWrite(ledBlue, ledVal);           // sets the value (range from 0 to 255)
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Sleeping() {

  // Disable the ADC (Analog to digital converter, pins A0 [14] to A5 [19])
  static byte prevADCSRA = ADCSRA;
  ADCSRA = 0;

  /* Set the type of sleep mode we want. Can be one of (in order of power saving):

     SLEEP_MODE_IDLE (Timer 0 will wake up every millisecond to keep millis running)
     SLEEP_MODE_ADC
     SLEEP_MODE_PWR_SAVE (TIMER 2 keeps running)
     SLEEP_MODE_EXT_STANDBY
     SLEEP_MODE_STANDBY (Oscillator keeps running, makes for faster wake-up)
     SLEEP_MODE_PWR_DOWN (Deep sleep)
  */
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();

  while (sleepCnt < 8) {
    // Turn of Brown Out Detection (low voltage). This is automatically re-enabled upon timer interrupt
    sleep_bod_disable();

    // Ensure we can wake up again by first disabling interrupts (temporarily) so
    // the wakeISR does not run before we are asleep and then prevent interrupts,
    // and then defining the ISR (Interrupt Service Routine) to run when poked awake by the timer
    noInterrupts();

    // clear various "reset" flags
    MCUSR = 0;  // allow changes, disable reset
    WDTCSR = bit (WDCE) | bit(WDE); // set interrupt mode and an interval
    WDTCSR = bit (WDIE) | bit(WDP3) | bit(WDP1) | bit(WDP0);    // set WDIE, and 1 second delay
    wdt_reset();

    // Send a message just to show we are about to sleep
    led();
    // Serial.println("Good night!");
    // Serial.flush();

    // Allow interrupts now
    interrupts();

    // And enter sleep mode as set above
    sleep_cpu();
  }

  // --------------------------------------------------------
  // �Controller is now asleep until woken up by an interrupt
  // --------------------------------------------------------

  // Prevent sleep mode, so we don't enter it again, except deliberately, by code
  sleep_disable();

  // Wakes up at this point when timer wakes up �C
  //  Serial.println("I'm awake!");

  // Reset sleep counter
  sleepCnt = 0;

  // Re-enable ADC if it was previously running
  ADCSRA = prevADCSRA;
  }

  // When WatchDog timer causes �C to wake it comes here
  ISR (WDT_vect) {

  // Turn off watchdog, we don't want it to do anything (like resetting this sketch)
  wdt_disable();

  // Increment the WDT interrupt count
  sleepCnt++;

  // Now we continue running the main Loop() just after we went to sleep
}
