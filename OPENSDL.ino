#include <Wire.h>
#include <SparkFunTSL2561.h>
#include <cactus_io_BME280_I2C.h>

#include <stdint.h>
#include <SD.h> 
#include "Wire.h"
#include "SPI.h"
#include "RTClib.h"

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

BME280_I2C bme1(0x77);  // I2C using default 0x77 
BME280_I2C bme2(0x76);  // I2C using address 0x76
float t1;
float t2;
float l;

SFE_TSL2561 light;  
int chipSelect = 10; 
File mySensorData; 

boolean gain;     // Gain setting, 0 = X1, 1 = X16;
unsigned int ms;  // Integration ("shutter") time in milliseconds

unsigned int sampleNumber = 0; 
void setup()
{
while (!Serial); // for Leonardo/Micro/Zero
  Serial.begin(9600);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
  // following line sets the RTC to the date & time this sketch was compiled
 rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      }

  pinMode(10, OUTPUT); //Must declare 10 an output and reserve it
  SD.begin(4); //Initialize the SD card reader

  light.begin();
  gain = 0;
  unsigned char time = 2;
  Serial.println("Set timing...");
  light.setTiming(gain,time,ms);

  // To start taking measurements, power up the sensor:
    Serial.println("Powerup...");
  light.setPowerUp();
    
  Serial.println("Multi Bosch BME280 Barometric Pressure - Humidity - Temp Sensor | cactus.io"); 
  if (!bme1.begin()) {
    Serial.println("Could not find a First BME280 sensor, check wiring!");
    while (1);
  }
   if (!bme2.begin()) {
    Serial.println("Could not find a Second BME280 sensor, check wiring!");
    while (1);
  }
//Build a first-row of column headers
  Serial.print("\n\n");
  Serial.print("Date,");
  Serial.print("Time,");
  Serial.print("Sample,");
  Serial.print("%RH1, ");
  Serial.print("T1(deg C),");
  Serial.print("%RH2,");
  Serial.print("");
  Serial.print("T2(deg C)");
  Serial.println("Lux");
  mySensorData = SD.open("Logged01.csv", FILE_WRITE);
  mySensorData.print("Date, ");
  mySensorData.print("Time, ");
  mySensorData.print("Sample, ");  
  mySensorData.print("RH (1), ");
  mySensorData.print("Temperature (1), ");
  mySensorData.print("RH (2), ");
  mySensorData.print("Temperature (2), ");
  mySensorData.println("Lux,");
  mySensorData.close();
}
void loop()
{
  delay(ms);
  unsigned int data0, data1;
  if (light.getData(data0,data1))
  {
    Serial.print("data0: ");
    Serial.print(data0);
    Serial.print(" data1: ");
    Serial.print(data1);

    double lux;    // Resulting lux value
    boolean good;  // True if neither sensor is saturated
    good = light.getLux(gain,ms,data0,data1,lux);
    Serial.print(" lux: ");
    l=lux;
    Serial.print((0.8816*l)-2.8856);
    Serial.print(lux);
    if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
  
   bme1.readSensor();
   bme2.readSensor();
   mySensorData = SD.open("Logged01.csv", FILE_WRITE);
   Serial.print(sampleNumber);
   Serial.print(",");
   Serial.print(bme1.getHumidity());
   Serial.print(",");
   Serial.print(bme1.getTemperature_C());
   Serial.print(",");
   t1=bme1.getTemperature_C();
   Serial.print((1.0555*t1)-2.5825);
   Serial.print(bme2.getHumidity());
   Serial.print(",");
   Serial.print(bme2.getTemperature_C());
   t2=bme2.getTemperature_C();
   Serial.print((1.0501*t2)-2.6965);
   Serial.println(" ");

   DateTime now = rtc.now();
   
   Serial.print(now.year(), DEC);
   Serial.print('/');
   Serial.print(now.month(), DEC);
   Serial.print('/');
   Serial.print(now.day(), DEC);
   Serial.print(" (");
   Serial.print(now.hour(), DEC);
   Serial.print(':');
   Serial.print(now.minute(), DEC);
   Serial.print(':');
   Serial.print(now.second(), DEC);
   Serial.print(") ");

   mySensorData.print(sampleNumber);
   mySensorData.print(",");   
   mySensorData.print(bme1.getHumidity());                             
   mySensorData.print(",");                               
   mySensorData.print((1.0555*t1)-2.5825);
   t1=bme1.getTemperature_C();                        
   mySensorData.print(",");

    mySensorData.print(bme2.getHumidity());                             
    mySensorData.print(",");                               
    mySensorData.print((1.0501*t2)-2.6965);
    mySensorData.print(",");
    t2=bme2.getTemperature_C();

    mySensorData.print((0.8816*l)-2.8856);
    mySensorData.println(",");

    mySensorData.print(now.day ());
    mySensorData.print("/");
    mySensorData.print(now.month ());
    mySensorData.print("/");
    mySensorData.print(now.year ());
    mySensorData.print(" ");
    mySensorData.print(",");
    mySensorData.print(now.hour ());
    mySensorData.print(":");
    mySensorData.print(now.minute ());
    mySensorData.print(":");
    mySensorData.print(now.second ());                    
    
    mySensorData.print(",");
    mySensorData.close();                                  //close the file

   sampleNumber++;

  delay(300000);

}
}  

