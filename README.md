# Solar-Fan-Thermostat
Personal Project to build a thermostat using the ATMega (programmed using the Arduino Uno) to regulate temperature in a greenhouse using a Solar Powered Fan, using parts found in my universities ECE parts shop. ATMega will just read a temperature and humidity from a AM2320 sensor once a minute, display them to an LCD using I2C, and turn the fan on if temp goes to high using a transistor connecting the solar panel to the fan.

Challenges included:
   -Making the end product as user-friendly as possible (done by limiting possible user actions), because it is intended to be used by anyone with access to the     thermostat
  -Learning about the Arduino and it's libraries/code, this is both my very first Arduino project and first personal project outside of class
  -Learning more about microcontrollers at a higher level, speicifically EEPROM and it's limitations, as well as I2C.

Programs are all written using the Arduino IDE. ̶ ̶M̶i̶g̶h̶t̶ ̶r̶e̶w̶r̶i̶t̶e̶ ̶t̶h̶e̶m̶ ̶i̶n̶ ̶C̶ ̶g̶i̶v̶e̶n̶ ̶e̶n̶o̶u̶g̶h̶ ̶t̶i̶m̶e̶ ̶t̶o̶ ̶l̶e̶a̶r̶n̶ ̶t̶o̶ ̶t̶r̶a̶n̶s̶l̶a̶t̶e̶ ̶b̶e̶t̶w̶e̶e̶n̶ ̶t̶h̶e̶ ̶t̶w̶o̶  coding an STM32 in C made me realize this project would take 100x longer, so its staying in the Arduino langauge. Code and Schmatics are free to use.
