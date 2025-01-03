#include <SPI.h> 
#include "define.h" //used

void displayTime();
void IRAM_ATTR setHour();
void IRAM_ATTR setMinute();
void getLunarDate();
// void IRAM_ATTR setTimeButton(); //button 3 for setting time
void IRAM_ATTR turnOffLed();
void setTimeScreen();
void wakeUp();
int char2int(char c[]);

Task t1(60000, TASK_FOREVER, &displayTime); //every 60 seconds
Task t2(86400000, TASK_FOREVER, &getLunarDate); //every 24 hours

///@brief ------setup------
void setup() {
    //Setup mcu
    Serial.begin(115200);

    //Setup buttons and led (gpio)
    pinMode(button_1, INPUT_PULLUP);
    pinMode(button_2, INPUT_PULLUP);
    pinMode(button_3, INPUT_PULLUP);

    attachInterrupt(button_1, setHour, FALLING);
    attachInterrupt(button_2, setMinute, FALLING);
    attachInterrupt(button_3, turnOffLed, FALLING);

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    //Setup RTC
    Wire.begin(SDA,SCL);
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

/// @brief ---LOOP---
void loop() {
    runner.execute();
    wakeUp(); //check if the time is the same as the set time
}







