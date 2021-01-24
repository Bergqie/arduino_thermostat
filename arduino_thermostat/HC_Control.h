#include "ClockBasics.h"
// States for the heating and cooling process of the thermostat
enum HCStates {HC_Nothing, HC_Heating, HC_Cooling }
HCState = HC_Nothing;

//States for room, set, and off on the thermostat
enum TemperatureStatus {Room, Set}
TempStatus = Room;

//Global Variables
float SetTemperature = 76.0;
float TemperatureTolerance = 5.0;
float CurrentTemperature;

//Move set temperature to go up or down
void ChangeSetTemperature(int ChangeTemperature) {

  SetTemperature += 0.25 * ChangeTemperature; // Change the temperature

} // end of ChangeSetTemperature

//Reads temperature and adjusts it according to the HCState
void UpdateTemperature() {

  CurrentTemperature = 100.0 * (1.1 * analogRead(A0) / 1024.0); // 100 degrees per volt

  switch (HCState) {
    case HC_Nothing:
      if (CurrentTemperature >= SetTemperature + TemperatureTolerance) { // if current temp is higher than the set temp and temp tolerence
        HCState = HC_Cooling; // start cooling
        LcdDriver.setCursor(0, 0); // set cursor to the top left
        LcdDriver.print("CL "); // show that it is cooling
      }
      else if (CurrentTemperature <= SetTemperature - TemperatureTolerance) { //else if less than that
        HCState = HC_Heating; // start heating
        LcdDriver.setCursor(0, 0); // set cursor to the top left
        LcdDriver.print("HTG"); // show that it is heating
      }
      break;
    case HC_Heating:
      if (CurrentTemperature >= SetTemperature) { // if current temp higher than set temp
        HCState = HC_Nothing; // do nothing
        LcdDriver.setCursor(0, 0); // set the cursor to the top left
        LcdDriver.print("OFF"); // show that the temperature reading is off
      }
      break;
    case HC_Cooling:
      if (CurrentTemperature <= SetTemperature) { // if current temp is less than set temp
        HCState = HC_Nothing; // do nothing
        LcdDriver.setCursor(0, 0); // set the cursor to the top left
        LcdDriver.print("OFF"); // show that the temperature reading is off
      }
      break;
  } // end of switch

} // end of UpdateTemperature

//method to send the current status of the temperature of the room
void SendTemperatureStatus() {

  switch (TempStatus) { // switch the for the temp status

    case Room:
      LcdDriver.setCursor(0, 1); //set cursor to the bottom left
      LcdDriver.print("ROOM"); // show that the room is in room temperature mode

      LcdDriver.setCursor(9, 8); // set the cursor to the bottom right
      LcdDriver.print(CurrentTemperature); // show the current temperature
      break;
    case Set:
      LcdDriver.setCursor(0, 1); // set the cursor to the bottom left
      LcdDriver.print("SET "); // show the room is in set temperature mode

      LcdDriver.setCursor(9, 8); // set the cursor to the bottom right
      LcdDriver.print(SetTemperature); //show the set temperature of the room
      break;
  } // end of switch
} // end of method

//method that changes the state of the temperature status
void MoveTemperatureState() {

  switch (TempStatus) { // switch the temperature status

    case Room:
      TempStatus = Set; // change temperature status to set
      LcdDriver.setCursor(9, 8); // set the cursor to the bottom right
      LcdDriver.print(SetTemperature); // change to set temperature
      break;
    case Set:
      TempStatus = Room; // change the temperature status to room
      LcdDriver.setCursor(9, 8); //set the cursor to the bottom right
      LcdDriver.print(CurrentTemperature); // change to current temperature
      break;

  } // end of switch
} // end of method
