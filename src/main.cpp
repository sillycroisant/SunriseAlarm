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
char hour[16], minute[16], day[16], month[16];
int set_hour = 0, set_minute = 0;

void displayTime();
void getTime();
void turnOnLed();
void turnOffLed();
void wakeUp();
int char2int(char c[]);
void standoff();

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
     
    //Whether the setup process is good
    Serial.print("The setup process was done!");
}

void loop() {
    getTime();
    displayTime();
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

void turnOnLed(){
    unsigned long current_time = millis();
    if(current_time - last_button1_interrupt_time > debounce_delay){
        last_button1_interrupt_time = current_time;
        digitalWrite(ledPin, HIGH);
    }
    last_button1_interrupt_time = current_time;
}

void turnOffLed(){
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