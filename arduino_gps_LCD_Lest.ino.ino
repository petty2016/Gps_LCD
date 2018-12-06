#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <TinyGPS.h>

const int TxPin = 4; //LCD Display Setup
const int ledPin = 9;
const int btnPin = 1; // the number of the pushbutton pin

//GPS stuff
TinyGPS gps;
SoftwareSerial serialgps(4, 3);
int mode = HIGH; //Mode for GPS: LOW for location, HIGH for time

//LCD Stuff
// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Button Stuff
int prevState=HIGH;

int year;
byte month, day, hour, minute, second, hundredths;
unsigned long chars;
unsigned short sentences, failed_checksum;
int initial = 0;

void setup() {
    Serial.begin(115200);
    serialgps.begin(9600);
    //LED
    pinMode(ledPin, OUTPUT);
    //Button
    pinMode(btnPin, INPUT);

    // set up the LCD's number of columns and rows: 
    lcd.begin(16, 2);
    // Print a message to the LCD.
    lcd.print("Arduino GPS");
    lcd.setCursor(0, 1);
    lcd.print("Waiting for lock");      //Wait for satellite to lock onto GPS module
    delay(1500);

}

void loop() {
  //Mode toggling
  int currState = digitalRead(btnPin);
   if(currState != prevState)
   {
      // A transition occurred (from pressed to released or from released to pressed)
      if(mode == HIGH){
        mode = LOW;
        lcd.clear();
        //delay(1);
      }else{
        mode = HIGH;
       // delay(1);
        lcd.clear();
      }
   }
   prevState = currState;
   
    while (serialgps.available()) {
        digitalWrite(ledPin, HIGH);
        int c = serialgps.read();
        if (gps.encode(c)) {
            if (initial == 0) {
                lcd.clear();
                initial += 1;
            }
            float latitude, longitude;

            gps.f_get_position( & latitude, & longitude);
            Serial.print("Lat/Long: ");
            Serial.print(latitude, 5);
            Serial.print(", ");
            Serial.println(longitude, 5);

            gps.crack_datetime( & year, & month, & day, & hour, & minute, & second, & hundredths);
            
            if (mode == LOW) {
                lcd.setCursor(0, 0);
                lcd.print("Lat: ");
                lcd.print(latitude, 5);
                lcd.setCursor(0, 1);
                lcd.print("Long: ");
                lcd.print(longitude, 5);
            } else if (mode == HIGH) {
                lcd.setCursor(0, 0);
                lcd.print("Time: ");
                lcd.print(hour, DEC);
                lcd.print(":");
                lcd.print(minute, DEC);
                lcd.print(":");
                lcd.print(second, DEC);
                lcd.print(".");
                lcd.print(hundredths, DEC);
                lcd.setCursor(0, 1);
                lcd.print("Date: ");
                lcd.print(month, DEC);
                lcd.print("/");
                lcd.print(day, DEC);
                lcd.print("/");
                lcd.print(year);

            } else {
                lcd.setCursor(0, 0);
                lcd.print("Error...");
            }
            Serial.print("Date: ");
            Serial.print(month, DEC);
            Serial.print("/");
            Serial.print(day, DEC);
            Serial.print("/");
            Serial.print(year);
            Serial.print(" Time: ");
            Serial.print(hour, DEC);
            Serial.print(":");
            Serial.print(minute, DEC);
            Serial.print(":");
            Serial.print(second, DEC);
            Serial.print(".");
            Serial.println(hundredths, DEC);
            Serial.print("Altitude (meters): ");
            Serial.println(gps.f_altitude());
            Serial.print("Course (degrees): ");
            Serial.println(gps.f_course());
            Serial.print("Speed(kmph): ");
            Serial.println(gps.f_speed_kmph());
            Serial.print("Satellites: ");
            Serial.println(gps.satellites());
            Serial.println();


            gps.stats( & chars, & sentences, & failed_checksum);

        }
    }
    digitalWrite(ledPin, LOW);

}
