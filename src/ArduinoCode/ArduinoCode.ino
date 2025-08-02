#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <string.h>
#include "SevSeg.h"

#define LCD_ROWS 2
#define LCD_COLS 16

String day = "";
int displayHourMinute = 0;
String description = "";
String feelsLike = "";
String humidity = "";


const byte digitPins[] = {10,11, 12, 13}; // seven segment display digit pins
const byte shiftRegisterPins[] = {7, 8, 9}; // Shift registers pins

SevSeg sevseg;
LiquidCrystal_I2C lcd(0x27,LCD_COLS,LCD_ROWS);

void setup() {
  Serial.begin(115200); //for communication with ESP-01
  sevseg.begin(COMMON_ANODE,4, digitPins, shiftRegisterPins);
  sevseg.setBrightness(90);
  lcd.init();
  lcd.backlight();
}

void loop() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
  //message formating avaible in src/ESP-01_code/message_format
    if (line.startsWith("time:")) {
      String timeData = line.substring(5);
      int spaceIndex = timeData.indexOf(' ');
      if (spaceIndex != -1 && timeData.length() >= spaceIndex + 5) {
        day = timeData.substring(0, spaceIndex);        
        String hourMinuteStr = timeData.substring(spaceIndex + 1); 
        displayHourMinute = hourMinuteStr.toInt();       
      }
    } else if (line.startsWith("description:")) {
      description = line.substring(12);
    } else if (line.startsWith("feels_like:")) {
      feelsLike = line.substring(11);
    } else if (line.startsWith("humidity:")) {
      humidity = line.substring(9);
    }

    // LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(day);
    lcd.print(" ");
    lcd.print(description); //can create problems (sometimes descriptions are too long and it doesnt fit on the lcd)
    lcd.setCursor(0, 1);
    lcd.print("T:");
    lcd.print(feelsLike);
    lcd.print(" H:");
    lcd.print(humidity);
  }

  // 7-segment display
  sevseg.setNumber(displayHourMinute,2); //point on second space
  sevseg.refreshDisplay();
}