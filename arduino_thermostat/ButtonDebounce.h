enum ButtonStates {ButtonIdle, ButtonWait, ButtonLow, ButtonLowLong}; // global variable that holds the three button states
int ButtonPin = 4; // integer used for the pin the button is on
unsigned long ButtonTimer; //unsigned long to hold the time to debounce the Switch
ButtonStates buttonStates = ButtonIdle; // set the ButtonState to idle

//function to setup the system to read button presses
void ButtonInitialize() {

  pinMode(ButtonPin, INPUT); // get the output of pin 4 used for the button
  pinMode(13, OUTPUT); //use the led on pin 13 (to test)
  digitalWrite(13, LOW); // set the led to be off
} // end of ButtonInitialize method

//Function that is to be called in loop to service the switch
// Returns 1 when the button is being pressed, 2 when it isn't, and 0 when nothing is happening.
int ButtonNextState(int Press) {

  //Read the buttons current value
  switch (buttonStates) {

    case ButtonIdle: //State where nothing has been happening
      if (Press == LOW) { // if button has gone low
        ButtonTimer = millis(); // Record time of high to low transition
        buttonStates = ButtonWait; // Move to Wait State
        digitalWrite(13, HIGH); //set the led to be on (for testing)

      } //end of if low statement
      break;

    case ButtonWait: //State has gone low and we are waiting for it to remain low for 5 milliseconds
      if (Press == HIGH) { //if button has gone high
        buttonStates = ButtonIdle; // set State back to idle
        digitalWrite(13, LOW); // set the led off (for testing)
      } //end of if statement
      else { //start of else

        if (millis() - ButtonTimer >= 5) { // if 5 milliseconds has passed
          buttonStates = ButtonLow; // Move to low state
          digitalWrite(13, LOW); // set the led off (for testing)
          return 1; // indicate that the button has been pressed
        } //end of if
      } // end of else
      break;

    case ButtonLow: //Button is low and has been for 5 milliseconds
      if (Press == HIGH) { //Once button is released
        buttonStates = ButtonLowLong; //go back to idle state
        if (millis() - ButtonTimer < 500)
          return 3;// indicate released before 0.5 sec.
        return 2;// indicate released after 0.5 sec.
      } // end of if statement
      break;

    case ButtonLowLong: // Button is low and has been 1.5 seconds
      if (Press == HIGH) { //Once button is released
        buttonStates = ButtonIdle; // go back to idle state
        if (millis() - ButtonTimer > 1500)
          return 4; //indicate released before 1.5 sec.
      } // end of if
      break;

  } //end of switch on buttonStates


  return 0; // indicates nothing happening

} // end of method for ButtonStates
