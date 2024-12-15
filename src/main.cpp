// include libraries
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

// define pins
#define LCD_SDA 1
#define LCD_SCL 0
#define RTC_SDA 1
#define RTC_SCL 0

LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;

void display();
void getTime();

DateTime now;

void setup() {
    //Setup mcu
    Serial.begin(115200);

    //Setup buttons and led (gpio)


    //Setup RTC
    Wire.begin(RTC_SDA,RTC_SCL);
    rtc.begin();
    // if(rtc.isrunning()){
    //     rtc.adjust(DateTime(__DATE__, __TIME__));
    //     Serial.print("RTC is good!");
    // }

    //Setup LCD
    Wire.begin(1,0);
    lcd.init();
    lcd.backlight();
     
    //Whether the setup process is good
    Serial.print("The setup process was done!");
}

void loop() {
    getTime();
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


void getTime(){
    now = rtc.now();
    Serial.print(now.year(),DEC);
    Serial.print("/");
    Serial.print(now.month(),DEC);
    Serial.print("/");
    Serial.println(now.day(),DEC);
    Serial.print(now.hour(),DEC);
    Serial.print(":");
    Serial.print(now.minute(),DEC);
    Serial.print(":");
    Serial.println(now.second(),DEC);
    
    delay(1000);
}