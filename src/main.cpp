// include libraries
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

// define pins
#define LCD_SDA 1
#define LCD_SCL 0

LiquidCrystal_I2C lcd(0x27, 16, 2);

void display();

void setup() {
    //Setup mcu
    Serial.begin(115200);

    //Setup buttons and led (gpio)


    //Setup RTC


    //Setup LCD
    Wire.begin(1,0);
    lcd.init();
    lcd.backlight();
     
    //Whether the setup process is good
    Serial.print("The setup process was done!");
}

void loop() {
    display();
}

void display(){
    lcd.setCursor(0,0);
    lcd.print("Hello, world!");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Hello, world!");
    delay(1000);
    lcd.clear();
}