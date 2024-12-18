// include libraries
#include <Arduino.h> //used
#include <Wire.h> //used
#include <SPI.h> 
#include <LiquidCrystal_I2C.h> //used
#include <RTClib.h> //used

// define pins
#define LCD_SDA 1
#define LCD_SCL 0
#define RTC_SDA 1
#define RTC_SCL 0
#define button_1 4
#define button_2 5
#define button_3 6
#define button_4 7
#define ledPin 2

//variablesvariables 
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;
DateTime now;
char hour[16], minute[16], day[16], month[16];
int set_hour = 0, set_minute = 0;

void displayTime();
void getTime();
void blinkLed();
void wakeUp();
int char2int(char c[]);

void setup() {
    //Setup mcu
    Serial.begin(115200);

    //Setup buttons and led (gpio)
    // attachInterrupt(button_1, blinkLed, CHANGE);
    // attachInterrupt(button_2, blinkLed, CHANGE);
    // attachInterrupt(button_3, blinkLed, CHANGE);
    // attachInterrupt(button_4, blinkLed, CHANGE);
    pinMode(ledPin, OUTPUT);

    //Setup RTC
    Wire.begin(RTC_SDA,RTC_SCL);
    rtc.begin();
    if(!rtc.isrunning()){
        rtc.adjust(DateTime(__DATE__, __TIME__));
        Serial.print("RTC is good!");
    }
    
    now = rtc.now();

    //Setup LCD
    Wire.begin(1,0);
    lcd.init();
    lcd.backlight();
     
    //Whether the setup process is good
    Serial.print("The setup process was done!");
}

void loop() {
    //get the real time clock
    getTime();

    //display the time and date
    displayTime();

    //check whether it's the time to ring
    wakeUp();

    delay(3000);
}

void displayTime(){
    lcd.setCursor(0,0);
    lcd.print("    ");
    lcd.print(now.day(), DEC);
    lcd.print("/");
    lcd.print(now.month(), DEC);
    lcd.print("/");
    lcd.print(now.year(), DEC);

    lcd.setCursor(0,1);
    lcd.print("      ");
    sprintf(hour, "%02d", now.hour());
    lcd.print(hour);
    lcd.print(":");
    sprintf(minute, "%02d", now.minute());
    lcd.print(minute);
}


void getTime(){
    now = rtc.now();
}

void blinkLed(){
    digitalWrite(ledPin, HIGH);
}

void wakeUp(){
    if(char2int(minute) == set_minute && char2int(hour) == set_hour){
        blinkLed();
        Serial.println("Wake up time");
    }
}

int char2int(char c[]){
    String s = String(c);
    int i = s.toInt();
    return i;
}