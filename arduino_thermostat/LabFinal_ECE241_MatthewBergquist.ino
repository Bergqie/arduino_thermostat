//System header files
#include <EEPROM.h>
#include <MsTimer2.h>


//Local header files
#include "ButtonDebounce.h"
#include "ClockBasics.h"
#include "EncoderMonitor.h"
#include "HC_Control.h"

unsigned long ClockTimer; //declare an unsigned long for a timer for the clock
unsigned long EEPROMTimer; //declare an unsigned long time for sending info to EEPROM

int TrackEncoderPosition = EncoderPosition; // int to keep track of EncoderPoistion

//start of setup
void setup() {

  Serial.begin(9600); // start serial port
  EncoderInitialize(); //Start the encoder
  ButtonInitialize(); // Code to set up pin and state machine;
  LcdDriver.begin(16, 2); //set up the LCD display
  LcdDriver.clear(); // clear the LCD display
  ClockTimer = 0; //set up the clock timer
  EEPROMTimer = 0; //set up the timer for sending to EEPROM
  analogReference (INTERNAL); //use the 3.3 volt reference
  CLK_Hours = 12;
  CLK_Minutes = 34;


  EEPROM.get (0, SetTemperature); // get the temperature from EEPROM
  if (isnan (SetTemperature)) { // load it in

    SetTemperature = 76.0; // set temp
    EEPROM.put (0, SetTemperature); // put it in the EEPROM

  } // end of if

  // Timer to maintain display
  MsTimer2::set(100, DisplayClock); // 100 ms period
  MsTimer2::start(); // start the timer to update display

} //end of setup

//start of loop
void loop() {

  if (millis() - ClockTimer >= 1000) { // if clock timer is greater than 1 second

    UpdateClock(); //increase time
    UpdateTemperature(); // check temp
    DisplayClock(); // display the clock
    DisplayTemperatureStatus(); // display the temperature status of the room

    ClockTimer += 1000; //increment clock timer by 1 second
  }

  if (millis() - EEPROMTimer >= 30000) { // if timer is greater than 30 seconds
    EEPROM.put (0, SetTemperature); // Write set temperture to EEPROM
    EEPROMTimer += 30000; // increment timer by 30 seconds
  } // end of timer if
  ButtonPress(); // call the button press method
  EncoderDetentTemperatureChange(); //change the set temperature



} //end of loop

//method that handles the button press
void ButtonPress() {

  switch (ButtonNextState(digitalRead(4))) { //Check status of button and switch is connected to pin 4
    case 1:
      break;
    case 2: // increase clock on short press
      IncreaseClock();
      DisplayClock();
      break;
    case 3: // move to next state on long press
      MoveClockState();
      DisplayClock();
      break;
    case 4: // move to next state on longer press
      MoveTemperatureState();
      DisplayTemperatureStatus();
      break;
    case 0: // Nothing happening
      break;

  } // end of switch
} // end of ButtonPress

// method that handles the temperature change with the encoder
void EncoderDetentTemperatureChange() {
  if ((EncoderPosition - TrackEncoderPosition) >= 4)
  {
    ChangeSetTemperature(-1); // go down by negative 1
    TrackEncoderPosition += 4; // move tracking variable
  } // end of if
  else if ((EncoderPosition - TrackEncoderPosition) <= -4) {
    // the encoder moved in the opposite direction
    ChangeSetTemperature(+1); // go up by positive 1
    TrackEncoderPosition -= 4; // update tracking variable.
  } // end of else if
} // end of encoder temperature change

//method to send the clock to display
void DisplayClock() {
  LcdDriver.setCursor(4, 0); // set the cursor to the top left of the LCD
  SendClock(); //send clock information to display

} // end of method

//method to send the current status of the temperature
void DisplayTemperatureStatus() {
  SendTemperatureStatus();
} //end of display temperature status
