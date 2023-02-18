#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const int TRANSISTOR_PIN = 2; // Digital output pin for transistor
int THRESHOLD_TEMP = 25; // Temperature threshold in Celsius

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD with the I2C address and the number of columns and rows

Adafruit_AM2320 am2320 = Adafruit_AM2320(); // Initialize the AM2320 digital temperature and humidity sensor

void setup() {
  pinMode(TRANSISTOR_PIN, OUTPUT); // Set transistor pin as output
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight
  Serial.begin(9600); // Start serial communication
  am2320.begin(); // Start the AM2320 sensor
}

void loop() {
  float temp = am2320.readTemperature(); // Read temperature from AM2320 sensor
  float humidity = am2320.readHumidity(); // Read humidity from AM2320 sensor

  lcd.setCursor(0, 0); // Set cursor to the first column of the first row
  lcd.print("Temp: "); // Print "Temp: " on the LCD
  lcd.print(temp); // Print the measured temperature on the LCD
  lcd.print("C"); // Print the units on the LCD

  lcd.setCursor(0, 1); // Set cursor to the first column of the second row
  lcd.print("Humidity: "); // Print "Humidity: " on the LCD
  lcd.print(humidity); // Print the measured humidity on the LCD
  lcd.print("%"); // Print the units on the LCD

  char key = keypad.getKey(); // Get the key pressed on the keypad

  if (key != NO_KEY) { // If a key is pressed
    if (key >= '0' && key <= '9') { // If the key is a number between 0 and 9
      THRESHOLD_TEMP = key - '0'; // Update the threshold temperature
    }
  }

  if (temp > THRESHOLD_TEMP) { // If the measured temperature is greater than the threshold temperature
    digitalWrite(TRANSISTOR_PIN, LOW); // turn off transistor
  } else { // Otherwise
    digitalWrite(TRANSISTOR_PIN, HIGH); // turn on transistor
  }
  
  delay(5000); // wait 5 seconds before taking the next temperature and humidity reading
}
