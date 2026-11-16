#include "SR04.h"
#define TRIG_PIN 12
#define ECHO_PIN 11
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 


LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;

void setup() {
    Serial.begin(9600);
    delay(1000);
    lcd.init(); 
    lcd.init();
    // Print a message to the LCD.
    lcd.backlight();
    lcd.setCursor(3,0);
    lcd.print("Hello, world!");
    lcd.setCursor(2,1);
    lcd.print("Ywrobot Arduino!");
    lcd.setCursor(0,2);
    lcd.print("Arduino LCM IIC 2004");
    lcd.setCursor(2,3);
    lcd.print("Power By Ec-yuan!");
}

void loop() {
   a=sr04.Distance();
   lcd.print(a);
   lcd.println("cm");
   delay(1000);
}
