//integer position that can change at any time
volatile int EncoderPosition;

// set up two separate integers for PinA (Pin2) and PinB (Pin 3) and set them to the respective pins
int PinA = 2;
int PinB = 3;

//interrupt service routines
void EncoderMonitorA() {
  if (digitalRead(PinA) == digitalRead(PinB)) { // if input A and B are set equal
    EncoderPosition++; //increment encoderPosition
  } // end of if
  else { //if not
    EncoderPosition--; //decrement encoderPosition
  } //end of else
} // end of EncoderMonitorA

//interrupt service routines; similar to the first function but it does the opposite
void EncoderMonitorB() {
  if (digitalRead(PinA) == digitalRead(PinB)) { // if input A and B are set equal
    EncoderPosition--; // decrement encoderPosition
  } // end of if
  else { // if not
    EncoderPosition++; // increment encoderPosition
  } // end of else
} // end of EncoderMonitorB

//function to start encoder monitoring
void EncoderInitialize() {
  pinMode(PinA, INPUT); // set PinA (Pin 2) as an input
  pinMode(PinB, INPUT); // set PinB (Pin 3) as an input
  attachInterrupt(digitalPinToInterrupt(PinA), EncoderMonitorA, CHANGE); //attatch the interrupt to PinA (Pin 2)
  attachInterrupt(digitalPinToInterrupt(PinB), EncoderMonitorB, CHANGE); // attach the interrupt to PinB (Pin 3)
}
