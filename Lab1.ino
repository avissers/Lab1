// 
// MME 4487 Lab 1
// 
//  Language: Arduino (C++)
//  Target:   ESP32
//  Author:   Michael Naish
//  Date:     2023 08 30 
//

#include <Arduino.h>                            // Necessary for PlatformIO

// Function declarations
void doHeartbeat();
void setLED(int pos, boolean state);

// Constants
const int ci_HeartbeatLED = 2;                  // GPIO pin of built-in LED for heartbeat
const int ci_HeartbeatInterval = 500;           // heartbeat blink interval, in milliseconds
const int ci_LEDPins[] = {
  32, 33, 25, 26, 27, 14};                              // select the GPIO pins for the LEDs
const int ci_NumberLEDs =  
  sizeof(ci_LEDPins) / sizeof(ci_LEDPins[0]);   // number of LEDs in display

// Variables
boolean b_Heartbeat = true;                     // state of heartbeat LED
boolean b_Dir = 0;                              // sweep direction: 0, ascending; 1, descending
boolean b_TurnOn = 1;                           // 1 = turn LEDs on; 0 = turn LEDs off
unsigned long ul_LastHeartbeat = 0;             // time of last heartbeat state change
unsigned long ul_CurMillis = 0;                 // current time, in milliseconds
unsigned long ul_PrevMillis = 0;                // start time for delay cycle, in milliseconds
unsigned long ul_Rate = 100;                    // time delay for switching effect
int i_Pos = 0;                                  // position index of LED to be illuminated

void setup() {
  // configure built-in LED for heartbeat
  pinMode(ci_HeartbeatLED, OUTPUT);

  // set all LED pins as outputs
  for (int i = 0; i < ci_NumberLEDs; i++) {
    pinMode(ci_LEDPins[i], OUTPUT);
  }
}

void loop() {
  ul_CurMillis = millis();                      // get the current time in milliseconds
  if (ul_CurMillis - ul_PrevMillis > ul_Rate) { // check to see if elapsed time matched the desired delay
    ul_PrevMillis = ul_CurMillis;               // update the start time for the next delay cycle
    setLED(i_Pos, b_TurnOn);                    // set state of current LED
    if (b_Dir == 0) {                           // "ascending"
      if (i_Pos == ci_NumberLEDs - 1) {         // if at the last LED in the row
        if (!b_TurnOn) {                        // and all LEDs now off
           b_Dir = 1;                           // change direction (to descending)
        }
        else {                              
          i_Pos = 0;                            // otherwise go to first LED
        }
        b_TurnOn = !b_TurnOn;                   // toggle desired LED on/off state 
      }
      else {
        i_Pos++;                                // otherwise go to next LED
      }
    }
    else {                                      // "descending"
      if (i_Pos == 0) {                         // if at the first LED in the row
        if (!b_TurnOn) {                        // and all LEDs now off
          b_Dir = 0;                            // change direction (to ascending)
        }
        else {                             
          i_Pos = ci_NumberLEDs - 1;            // otherwise go to last LED in row
        }
        b_TurnOn = !b_TurnOn;                   // toggle desired LED on/off state
      }
      else {
         i_Pos--;                               // otherwise go to next LED
      }
    }  
  }
  doHeartbeat();                                // update heartbeat LED
}

// blink heartbeat LED
void doHeartbeat() {
  ul_CurMillis = millis();                      // get the current time in milliseconds
  // check to see if elapsed time matches the heartbeat interval
  if ((ul_CurMillis - ul_LastHeartbeat) > ci_HeartbeatInterval) {
    ul_LastHeartbeat = ul_CurMillis;            // update the heartbeat toggle time for the next cycle
    b_Heartbeat = !b_Heartbeat;                 // toggle state of LED
    digitalWrite(ci_HeartbeatLED, b_Heartbeat); // update LED
  }
}

// set the state of a specified LED
void setLED(int pos, boolean state) {
  if (pos >= 0 && pos < ci_NumberLEDs) {          // ignore invalid LED indicies
    if (state) {
      digitalWrite(ci_LEDPins[pos], HIGH);        // turn specified LED on
    }
    else {
      digitalWrite(ci_LEDPins[pos], LOW);         // turn specified LED off
    }
  }
}