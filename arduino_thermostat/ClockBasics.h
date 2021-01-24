#ifndef ClockBasics_H
#define ClockBasics_H

#include <LiquidCrystal.h>

LiquidCrystal LcdDriver(A5, A4, 5, 6, 7, 8); //Defines the pins used for LCD
// Variable used as clock settings.
int CLK_Hours, CLK_Minutes, CLK_Seconds;
//this function sets up the SPI for the LED
// This function is to be called every second
// to update the clock represented by the
// global variables Hours, Minutes, Seconds
void UpdateClock()
{
  // Check if Seconds not at wrap point.
  if (CLK_Seconds < 59)
  {
    CLK_Seconds++; // Move seconds ahead.
  }
  else
  {
    CLK_Seconds = 0; // Reset Seconds
    // and check Minutes for wrap.
    if (CLK_Minutes < 59)
    {
      CLK_Minutes++; // Move seconds ahead.
    }
    else
    {
      CLK_Minutes = 0; // Reset Minutes
      // check Hours for wrap
      if (CLK_Hours < 23)
      {
        CLK_Hours++;// Move Hours ahead.
      }
      else
      {
        CLK_Hours = 0;// Reset Hours
      }// End of Hours test.
    } // End of Minutes test
  } // End of Seconds test
} // end of UpdateClock()

// States for setting clock.
enum ClockStates { CLOCK_RUNNING, CLOCK_SET_HOURS,
                   CLOCK_SET_MINUTES, CLOCK_SET_SECONDS
                 };
ClockStates clockState = CLOCK_RUNNING;

// Function that processes incoming characters to set the clock.
void SettingClock(char Input)
{
  // interpret input based on state
  switch (clockState)
  {
    case CLOCK_RUNNING:
      if (Input == 'S')
      {
        clockState = CLOCK_SET_HOURS;
        CLK_Hours = 0;   // Reset clock values.
        CLK_Minutes = 0;
        CLK_Seconds = 0;
      }
      break;
    case CLOCK_SET_HOURS: //
      if (Input >= '0' && Input <= '9')
        CLK_Hours = 10 * (CLK_Hours % 10) + Input - '0';
      else if (Input == ':')
        clockState = CLOCK_SET_MINUTES;
      else if (Input == 'R')
        clockState = CLOCK_RUNNING;
      break;
    case CLOCK_SET_MINUTES: //
      if (Input >= '0' && Input <= '9')
        CLK_Minutes = 10 * (CLK_Minutes % 10) + Input - '0';
      else if (Input == ':')
        clockState = CLOCK_SET_SECONDS;
      else if (Input == 'R')
        clockState = CLOCK_RUNNING;
      break;
    case CLOCK_SET_SECONDS: //
      if (Input >= '0' && Input <= '9')
        CLK_Seconds = 10 * (CLK_Seconds % 10) + Input - '0';
      else if (Input == 'R')
        clockState = CLOCK_RUNNING;
      break;

  }// End of clock mode switch.

} // End of SettingClock

void SendClock()
{

  // Check if leading zero needs to be sent
  if (CLK_Hours < 10)
  {
    LcdDriver.print("0");
  }
  LcdDriver.print(CLK_Hours); // Then send hours
  LcdDriver.print(":"); // And separator
  // Check for leading zero on Minutes.
  if (CLK_Minutes < 10)
  {
    LcdDriver.print("0");
  }
  LcdDriver.print(CLK_Minutes); // Then send Minutes
  LcdDriver.print(":"); // And separator
  // Check for leading zero needed for Seconds.
  if (CLK_Seconds < 10)
  {
    LcdDriver.print("0");
  }
  LcdDriver.print(CLK_Seconds); // Then send Seconds
  // with new line

  switch (clockState) { // start switch statement for moving cursor when setting time

    case CLOCK_RUNNING:
      break;
    case CLOCK_SET_HOURS: // if setting hours
      LcdDriver.setCursor(5, 0); // set cursor next to hours
      break;
    case CLOCK_SET_MINUTES: // if setting minutes
      LcdDriver.setCursor(8, 0); // set cursor next to minutes
      break;
    case CLOCK_SET_SECONDS: // if setting seconds
      LcdDriver.setCursor(11, 0); // set cursor next to seconds
      break;
  } // end of switch

  LcdDriver.cursor(); //turn on cursor on LCD
  LcdDriver.blink(); // and make it blink
} // End of SendClock()


//Function used to move through
//the states of setting the clock
void MoveClockState() {

  switch (clockState) {
    case CLOCK_RUNNING: //From Running
      clockState = CLOCK_SET_HOURS; // go to set the hours
      break;
    case CLOCK_SET_HOURS: // From set hours
      clockState = CLOCK_SET_MINUTES; //go to set the minutes
      break;
    case CLOCK_SET_MINUTES:
      clockState = CLOCK_SET_SECONDS; //go to set the seconds
      break;
    case CLOCK_SET_SECONDS:
      clockState = CLOCK_RUNNING; //set back to running
      break;
  } // end of switch
} // end of move clock state

//Function that increases the clock time
//based on the state of the clock
void IncreaseClock() {

  //interpret input based on state
  switch (clockState) {

    case CLOCK_RUNNING: // nothing to change if running
      break;
    case CLOCK_SET_HOURS: // in clock set hours state
      CLK_Hours++; // the hours to be incremented
      if (CLK_Hours > 23) // watch for wrap around
        CLK_Hours = 0; // set back to zero
      break;
    case CLOCK_SET_MINUTES: // in clock set minutes state
      CLK_Minutes++; // increment minutes on clock
      if (CLK_Minutes > 59) // wrap around for minutes
        CLK_Minutes = 0; // set minutes back to zero
      break;
    case CLOCK_SET_SECONDS: // in the clock set seconds state
      CLK_Seconds++; // increment seconds on clock
      if (CLK_Seconds > 59) // wrap around for seconds
        CLK_Seconds = 0; // set seconds back to zero
      break;

  } // end of switch
} // end of increase clock

#endif
