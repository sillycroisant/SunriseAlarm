// include libraries
#include <Arduino.h> //used
#include <Wire.h> //used
#include <SPI.h> 
#include <LiquidCrystal_I2C.h> //used
#include <RTClib.h> //used
#include <TaskScheduler.h> //used
#include <Date.h>
#include <LunarConverter.h>

using namespace Espace;

// define pins
#define LCD_SDA 1
#define LCD_SCL 0
#define RTC_SDA 1
#define RTC_SCL 0
#define button_1 18
#define button_2 5
#define button_3 6
#define button_4 7
#define ledPin 2
#define debounce_delay 200

volatile unsigned long last_button1_interrupt_time = 0;
volatile unsigned long last_button2_interrupt_time = 0;
volatile unsigned long last_button3_interrupt_time = 0;
volatile unsigned long last_button4_interrupt_time = 0;

//variablesvariables 
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;
DateTime now;
Scheduler runner;

char hour[16], minute[16], day[16], month[16];
int set_hour = 0, set_minute = 0;


void displayTime();
void getLunarDate();
void IRAM_ATTR turnOnLed();
void IRAM_ATTR turnOffLed();
void wakeUp();
int char2int(char c[]);
void standoff();

Task t1(60000, TASK_FOREVER, &displayTime); //every 60 seconds
Task t2(86400000, TASK_FOREVER, &getLunarDate); //every 24 hours

void setup() {
    //Setup mcu
    Serial.begin(115200);

    //Setup buttons and led (gpio)
    pinMode(button_1, INPUT_PULLUP);
    pinMode(button_2, INPUT_PULLUP);
    pinMode(button_3, INPUT_PULLUP);
    pinMode(button_4, INPUT_PULLUP);

    attachInterrupt(button_1, turnOnLed, FALLING);
    attachInterrupt(button_2, turnOffLed, FALLING);
    attachInterrupt(button_3, standoff, FALLING);
    attachInterrupt(button_4, standoff, FALLING);

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

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
     
    //Setup task
    runner.addTask(t1);
    runner.addTask(t2);
    t1.enable();
    t2.enable();

    //Lunar date
    now = rtc.now();
    Date solarDate(now.day(), now.month(), now.year()); //get solar date
    LunarConverter converter;   //create a converter object
    Date lunarDate = converter.Convert(solarDate); //convert solar date to lunar date

    displayTime();
    //Whether the setup process is good
    Serial.print("The setup process was done!");
}

void loop() {
    runner.execute();
    wakeUp(); //check if the time is the same as the set time
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
    lcd.print(" âm)");

    lcd.setCursor(0,1);
    lcd.print("     ");
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

void wakeUp(){
    if(char2int(minute) == set_minute && char2int(hour) == set_hour){
        digitalWrite(ledPin, HIGH);
    }
}

int char2int(char c[]){
    String s = String(c);
    int i = s.toInt();
    return i;
}

void standoff(){

}