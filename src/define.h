//libraries
#include <Arduino.h> //used
#include <Wire.h> //used
#include <LiquidCrystal_I2C.h> //used
#include <RTClib.h> //used
#include <TaskScheduler.h> //used
#include <LunarConverter.h> //used

using namespace Espace; //used

//define variables
#define SDA 1 //used
#define SCL 0 //used
#define button_1 18 //used
#define button_2 5 //used
#define ledPin 2 //used
#define debounce_delay 250 //used

LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;
DateTime now;
Scheduler runner;

char hour[16], minute[16], day[16], month[16];
unsigned int set_hour = 0, set_minute = 0;

volatile unsigned long last_button1_interrupt_time = 0;
volatile unsigned long last_button2_interrupt_time = 0;
volatile unsigned long last_button3_interrupt_time = 0;
volatile unsigned long last_button4_interrupt_time = 0;

//functions
void IRAM_ATTR turnOnLed(){
    unsigned long current_time = millis();
    if(current_time - last_button1_interrupt_time > debounce_delay){
        last_button1_interrupt_time = current_time;
        digitalWrite(ledPin, HIGH);
    }
    last_button1_interrupt_time = current_time;
}

void IRAM_ATTR turnOffLed(){
    unsigned long current_time = millis();
    if(current_time - last_button2_interrupt_time > debounce_delay){
        last_button2_interrupt_time = current_time;
        digitalWrite(ledPin, LOW);
    }
    last_button2_interrupt_time = current_time;
}

int char2int(char c[]){
        String s = String(c);
    int i = s.toInt();
    return i;
}

void getLunarDate(){
    now = rtc.now();
    Date solarDate(now.day(), now.month(), now.year());

    LunarConverter converter;
    Date lunarDate = converter.Convert(solarDate);
    int dd = lunarDate.DayInMonth;
    int mm = lunarDate.Month;
}

void displayTime(){
    now = rtc.now();

    Date solarDate(now.day(), now.month(), now.year()); //get solar date
    LunarConverter converter;   //create a converter object
    Date lunarDate = converter.Convert(solarDate); //convert solar date to lunar date

    lcd.setCursor(0,0);
    lcd.print(now.day(), DEC);
    lcd.print("/");
    lcd.print(now.month(), DEC);
    lcd.print("/");
    lcd.print(now.year(), DEC);


    lcd.print(" ");
    lcd.print(lunarDate.DayInMonth);
    lcd.print("t");
    lcd.print(lunarDate.Month);

    lcd.setCursor(0,1);
    lcd.print("    ");
    sprintf(hour, "%02d", now.hour());
    lcd.print(hour);
    lcd.print(":");
    sprintf(minute, "%02d", now.minute());
    lcd.print(minute);
    if(now.hour() >= 0 && now.hour() <= 12){
        lcd.print(" AM");
    } else {
        lcd.print(" PM");
    }
}

void wakeUp(){
    if(char2int(minute) == set_minute && char2int(hour) == set_hour){
        digitalWrite(ledPin, HIGH);
    }
}
