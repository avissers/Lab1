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
void activateLEDs();

// Constants
const int ci_HeartbeatLED = 2;                  // GPIO pin of built-in LED for heartbeat
const int ci_HeartbeatInterval = 500;           // heartbeat blink interval, in milliseconds
const int ci_LEDPins[] = {
  32, 33, 25, 26, 27, 14};                      // select the GPIO pins for the LEDs (added pins 27 and 14 for exercise 2)
const int ci_NumberLEDs =  
  sizeof(ci_LEDPins) / sizeof(ci_LEDPins[0]);   // number of LEDs in display

// Variables
boolean b_Heartbeat = true;                     // state of heartbeat LED
boolean b_Dir = 0;                              // sweep direction: 0, ascending; 1, descending
boolean b_TurnOn = 1;                           // 1 = turn LEDs on; 0 = turn LEDs off
boolean b_Pattern = 0;        // used to generate light pattern: 0 = turn 1 LED on, 1 = turn 2 LEDs on
unsigned long ul_LastHeartbeat = 0;             // time of last heartbeat state change
unsigned long ul_CurMillis = 0;                 // current time, in milliseconds
unsigned long ul_PrevMillis = 0;                // start time for delay cycle, in milliseconds
unsigned long ul_Rate = 1000;                   // time delay for switching effect
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
   long potRead = analogRead(36);                // read potentiometer
   ul_Rate = map(potRead, 0, 4096, 50, 1000);    // update the rate within the bounds of 50 - 1000ms
   if (ul_CurMillis - ul_PrevMillis > ul_Rate){  // check to see if elapsed time matched the desired delay
     ul_PrevMillis = ul_CurMillis;               // update the start time for the next delay cycle
     for(int i = 0; i < ci_NumberLEDs; i++){     // turn off all LEDs
      setLED(i, LOW);                            //
     }
     activateLEDS();                            // run function to determine which LEDs need to be turned on
     if (b_Dir == 0) {                          // "ascending"
       if (i_Pos == ci_NumberLEDs - 1){         // if at the last LED in the row
            b_Dir = 1;                          // change direction (to descending)
            i_Pos--;                            // decrement position
         }else{
          i_Pos++;                              // if not at the end, increment counter
         }
     }else{
      if(i_Pos == 0){                           //"descending"
        b_Dir = 0;                              // change direction
        i_Pos++;                                // increment position
      }else{
        i_Pos--;                                // decrement position
      }
     }

     }
   doHeartbeat();                                // update heartbeat LED
 }


// determine which lights to turn on/off
void activateLEDS(){
  if(b_Pattern == 0){                               // check which LED pattern we are on
    setLED(i_Pos, HIGH);                            // set LED at i position to ON
    b_Pattern = 1;                                  // switch the pattern for next iteration                       
  }else{                                            // if we are on pattern "1"
    setLED(i_Pos, HIGH);                            // turn on LED at i_Pos
    if(b_Dir == 0){                                 // if ascending
      if(i_Pos != 0){                               // if we are not at the first LED
        setLED(i_Pos - 1, HIGH);                    // set previous LED to ON
        i_Pos--;                                    // decrement position
      }
    }else{                                          // descending
      if(i_Pos != ci_NumberLEDs - 1){               // make sure we are not at the last LED
        setLED(i_Pos + 1, HIGH);                    // set LED at higher position to ON
        i_Pos++;                                    // increment position
    }
    }
    b_Pattern = 0;                                  // change pattern for next iteration
  }
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