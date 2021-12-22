#include <Joystick.h>

#define DEBUG 1

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x) 
#define debugln(x) 
#endif

#include <Wire.h>
#include "Adafruit_MCP23017.h"
//#include "ADS1X15.h"

//ADS1115 ADS(0x48);


const uint8_t addr1 = 0; // Adresse 0x20 / 0
const uint8_t addr2 = 1; // Adresse 0x21 / 1
const uint8_t addr3 = 2; // Adresse 0x22 / 2

#define DEBOUNCE_TIME 5 

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;
Adafruit_MCP23017 mcp3;

Joystick_ Joystick(0x03,
  //  joystickType          buttonCount   hatSwitchCount  
  JOYSTICK_TYPE_JOYSTICK,     48,            0,
  //includeXAxis includeYAxis includeZAxis includeRxAxis includeRyAxis includeRzAxis
       false,         false,       false,       false,        false,        false,
  //includeRudder  includeThrottle includeAccelerator includeBrake includeSteering
       false,           false,            false,            false,       false);
//Joystick_ Joystick(0x03);


byte switch_state[48];
byte switch_state_old[48];
byte reading, clk, clk_old, realpin, realbutton;
unsigned long debounce_time[48];




///---setup
void setup() {
  Serial.begin(9600);
//  while (!Serial); // Leonardo: wait for serial monitor

  mcp1.begin(addr1);      // use default address 0
  mcp2.begin(addr2);
  mcp3.begin(addr3);

    for (int i=0; i < 16; i++) {
          mcp1.pinMode(i, INPUT);
          mcp1.pullUp(i, HIGH);  // turn on a 100K pullup internally
          mcp2.pinMode(i, INPUT);
          mcp2.pullUp(i, HIGH);  // turn on a 100K pullup internally
          mcp3.pinMode(i, INPUT);
          mcp3.pullUp(i, HIGH);  // turn on a 100K pullup internally
    }

  
}

///----setup



void GetButtons(){
        int ButtonA = 0;
        int mcppins  [] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
        for (int i=0; i<sizeof mcppins/sizeof mcppins[0]; i++) {           
            int s = mcppins[i];
            debounceVal(ButtonA,(int) !mcp1.digitalRead(s));
            ButtonA++;          
        }
        for (int i=0; i<sizeof mcppins/sizeof mcppins[0]; i++) {           
            int s = mcppins[i];
            debounceVal(ButtonA,(int) !mcp2.digitalRead(s));
            ButtonA++;
        }

        for (int i=0; i<sizeof mcppins/sizeof mcppins[0]; i++) {           
            int s = mcppins[i];
            debounceVal(ButtonA,(int) !mcp3.digitalRead(s));
            ButtonA++;
        }
       
}



void debounceVal(int BtnNum, int CurrentVal){
            reading = (byte)CurrentVal;
            if (reading == switch_state[BtnNum]) debounce_time[BtnNum] = millis() + (unsigned long)DEBOUNCE_TIME;
            else if (millis() > debounce_time[BtnNum]) switch_state[BtnNum] = reading;
            if (switch_state[BtnNum] != switch_state_old[BtnNum]) { // debounced button has changed state

              Joystick.setButton(BtnNum,CurrentVal);
//              Joystick.pressButton(BtnNum);
              debug("press button");
              debugln(BtnNum);
              switch_state_old[BtnNum] = switch_state[BtnNum]; // store new state such that the above gets done only once
            }
}



void loop() {
    GetButtons();
    Joystick.sendState();
  //Serial.println("Loop");
 // MCP_LoopA(); 
 //   ADS_Loop();
 // delay(500);
  // put your main code here, to run repeatedly:

}
