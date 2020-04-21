#include <Wire.h>
#include "RTClib.h"
//#include <Adafruit_RGBLCDShield.h>

// include the SD library:
#include <SPI.h>
#include <SD.h>

RTC_DS1307 rtc;

File dataFile;

int led = 9;
int PT = 8;
int sensorPin = A0; //analog pin 0
File myFile;
Sd2Card card;
const int chipSelect = 4;

void setup () {
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }

    Serial.prinln("Testing");


    int val = analogRead(sensorPin);

    //rtc.adjust(DateTime(__DATE__, __TIME__)); //uncomment when need to reset time
   
   if (! rtc.begin()) {
      Serial.println("Couldn't find RTC");
      while (1);
    }
  
    if (! rtc.isrunning()) {
      Serial.println("RTC is NOT running!");
    }

    DateTime now = rtc.now();
    
    String date_time =  String(now.month()) + "/" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
    String dataString = String(now.year()) + "/" + date_time + " " + String(val);

    Serial.println(dataString);
    
    Serial.print("\nInitializing SD card...");
    if (!card.init(SPI_HALF_SPEED, chipSelect)) {
      Serial.println("initialization failed. Things to check:");
      Serial.println("* is a card inserted?");
      Serial.println("* is your wiring correct?");
      Serial.println("* did you change the chipSelect pin to match your shield or module?");
      while (1);
    } else {
      Serial.println("Wiring is correct and a card is present.");
    }

    if (!SD.begin(4)) {
      Serial.println("initialization failed!");
      while (1);
    }
    Serial.println("initialization done.");
    myFile = SD.open("OBS_DATA.txt", FILE_WRITE);
    if (myFile) {
      Serial.println("File opened correctly");
      myFile.close();
    }


}


void loop () {  
    DateTime now = rtc.now();
    int val = analogRead(sensorPin); // values currently range from around 0-1000

    String date_time =  String(now.month()) + "/" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
    String dataString = String(now.year()) + "/" + date_time + " " + String(val);

    myFile = SD.open("OBS_DATA.txt", FILE_WRITE);
    if (myFile) {
      Serial.println(dataString);
      myFile.println(dataString);
      myFile.close();
    }

    delay(1000);
   
}
