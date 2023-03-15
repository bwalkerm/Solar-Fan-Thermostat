//libraries
#include <EEPROM.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//NumPad vars
const byte ROWS = 4; //number of rows
const byte COLS = 3; //numbers of cols

char hexaKeys[ROWS][COLS] = { //num pad definition
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
/////////////////////////////////////////////////////
//                                                 //
//  NPPIN:  1    2    3    4    5    6    7    8   //
//  ARPIN:  8    7    6    5    4    3    2    X   //
//  NAME : ROW1 ROW2 ROW3 ROW4 COL1 COL2 COL3  X   //
//                                                 //
/////////////////////////////////////////////////////
byte rowPins[ROWS] = {8, 7, 6, 5}; //row pin definition
byte colPins[COLS] = {4, 3, 2};   //col pin definition


//temp vars
int thresh;
float sensTemp; //tempature - read from sensor
float sensHum; //humidity - read from sensor

//time-keeping vars
const long sensInterval = 60000; //interval at which sens measurements will happen (1 min)
const long lcdInterval = 10000; //interval at which the lcd screen will stay on for (10 seconds)
unsigned long lcdPrev, sensPrev = 0; //records last time associated system was updated
bool off = true; // varible to detirmine if lcd on or off (intialzied to off)

//class init
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); //initalize keypad
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
Adafruit_AM2320 am2320 = Adafruit_AM2320();

//Fan Control vars
const int tranGatePin = 9; //gate of the transistor used to control fans
int gateState = LOW; //always start with the transistor off

void setup(){ //ONLY RUNS DURING POWER ON

  thresh = EEPROM.read(4); //threshold temp, restored from EEPROM, initalized in setup so it restores every power cycle
  pinMode(tranGatePin, OUTPUT); //set the gate pin to output
  digitalWrite(tranGatePin, gateState); //intialize output to the gate (always starts low)
  lcd.init(); // Initialize the LCD
  am2320.begin(); // turn on sensor
  sensTemp = am2320.readTemperature(); //get temp
  sensHum = am2320.readHumidity(); //get humidity
  homescreen(); //display homescreen
  
}
  
void loop(){ //CONTINOUSLY RUNS
  unsigned long currentMillis = millis(); //time keeping var
  ////////LCD & NUMPAD COTNROL////////
  if ((off == false) & (currentMillis - lcdPrev >= lcdInterval)) { //if the LCD is already on and 10 seconds has passed since last key press
    lcd.noDisplay(); //turn off the LCD dispaly
    lcd.noBacklight();  //turn off the backlight
    off = true; //update the off var to OFF
  }
  char customKey = customKeypad.getKey(); //read numpad
  if(customKey){ //if button pressed
    lcdPrev = currentMillis; //record last time a key was pressed
    if(off = true) { // if the LCD is off
      lcd.display(); //turn on the LCD screen
      lcd.backlight(); //turn on the backlight
      off = false; //update off var to ON
    }
    if(customKey == '#') { // # is the set temp button
      thresh = setTemp();
      EEPROM.update(4, thresh); //update the value stored in EEPROM for power cycling
    }
    else{ //if a button was pressed but not one that should do anything
      lcd.clear(); //clear screen of text
      lcd.printstr("Press '#' to"); //print instructions
      lcd.setCursor(0, 1); //out of space, next row      
      lcd.print("change temp"); //print instructons
      delay(1000); // flash the message for one second
    }
    homescreen();
  }

  ////////TEMP CONTROL////////
  if (currentMillis - sensPrev >= sensInterval) { // only if enough time has passed since last measurement
    sensPrev = currentMillis; // save the last time a measurement was taken
    sensTemp = am2320.readTemperature(); //get temp
    sensHum = am2320.readHumidity(); //get humidity
  }

  ////////FAN CONTROL////////
  if((gateState == LOW) & (thresh < (sensTemp - 2)))  { //if the fans are OFF AND the measured temp is higher than the desired temp (sensTemp + 2 is to make sure fans will actaully make a difference)
    gateState = HIGH; //change state to HIGH
    digitalWrite(tranGatePin, gateState); //turn on the transistor
  }
  if((gateState == HIGH) & (thresh > (sensTemp + 2))) { //if the fans are ON AND the measured temp is lower than the desired temp
    gateState = LOW; //change state to LOW
    digitalWrite(tranGatePin, gateState); //turn off transistor
  }

}

void homescreen() { //function to display homescreen
  lcd.clear(); //clear the screen to prep for printing
  //lcd.setCursor(0, 0); // Set cursor to the first column of the first row
  lcd.printstr("T: "); // Print temp reading
  lcd.print(sensTemp);
  lcd.print(" H: ");  // print the humidity reading
  lcd.print(sensHum);
  lcd.setCursor(0, 1); //set to next row
  lcd.printstr("Set Temp: "); //print string of set threshhold
  lcd.print(thresh); //print threshold var

}


int setTemp() { //function to set the threshold temp to a different value
  lcd.clear(); //clear screen
  lcd.setCursor(0,0); //set to starting postion
  lcd.printstr("Set New Temp: "); //prompt new thresh temp

  char customKey = customKeypad.waitForKey(); //get char
  while((customKey == '*') | (customKey == '#')) { //check to make sure it wasn't a symbol
    customKey = customKeypad.waitForKey(); //get char
  }

  lcd.print(customKey); //print char
  int newTemp = 10 * (customKey - '0'); //set tens place of the new temp to typed char

  customKey = customKeypad.waitForKey(); //get char
  while((customKey == '#') | (customKey == '*')) { //check to make sure it wasn't a symbol
    customKey = customKeypad.waitForKey(); //get char
  }

  newTemp = newTemp + (customKey - '0'); //set ones place of new temp to typed char
  
  return newTemp;

}