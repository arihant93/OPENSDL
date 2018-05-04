
#include <SoftwareSerial.h>
SoftwareSerial espSerial =  SoftwareSerial(10,11);      // arduino RX pin=2  arduino TX pin=3    connect the arduino RX pin to esp8266 module TX pin   -  connect the arduino TX pin to esp8266 module RX pin
#include <Wire.h>
#include "cactus_io_BME280_I2C.h"
BME280_I2C bme1(0x77);  // I2C using default 0x77 
BME280_I2C bme2(0x76);  // I2C using address 0x76

String apiKey = "JN5VV5XYAMU77M";// replace with your channel's thingspeak WRITE API key

String ssid="mywifi";    // Wifi network SSID
String password ="mywifipassword";  // Wifi network password

boolean DEBUG=true;
void showResponse(int waitTime){
    long t=millis();
    char c;
    while (t+waitTime>millis()){
      if (espSerial.available()){
        c=espSerial.read();
        if (DEBUG) Serial.print(c);
      }
    }
                   
}

boolean thingSpeakWrite(float value1, float value2){
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
  cmd += "184.106.153.149";                               // api.thingspeak.com
  cmd += "\",80";
  espSerial.println(cmd);
  if (DEBUG) Serial.println(cmd);
  if(espSerial.find("Error")){  

    if (DEBUG) Serial.println("AT+CIPSTART error");
    return false;
  }
  
  
  String getStr = "GET /update?api_key=";   // prepare GET string
  getStr += apiKey;
  
  getStr +="&field1=";
  getStr += String(value1);
  getStr +="&field2=";
  getStr += String(value2);
  // getStr +="&field3=";
  // getStr += String(value3);
  // ...
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  espSerial.println(cmd);
  if (DEBUG)  Serial.println(cmd);
  
  delay(100);
  if(espSerial.find(">")){
    espSerial.print(getStr);
    if (DEBUG)  Serial.print(getStr);
  }
  else{
    espSerial.println("AT+CIPCLOSE");
    // alert user
    if (DEBUG)   Serial.println("AT+CIPCLOSE");
    return false;
  }
  return true;
}
void setup() {                
  DEBUG=true;           // enable debug serial
  Serial.begin(9600); 
  
//  dht.begin(); // Start DHT sensor
 bme1.begin();
//bme2.begin();  
  espSerial.begin(9600);  // enable software serial
                // Your esp8266 module's speed is probably at 115200. 
               // For this reason the first time set the speed to 115200 or to your esp8266 configured speed 
              // and upload. Then change to 9600 and upload again
  
  String rst = "AT+RST"; 
  Serial.println("RESET");
  sendData(rst, 1000, 1);// Enable this line to reset the module;
  showResponse(1000);

  //espSerial.println("AT+UART_CUR=9600,8,1,0,0");    // Enable this line to set esp8266 serial speed to 9600 bps
  //showResponse(1000);
  
  

  espSerial.println("AT+CWMODE=3");   // set esp8266 as client
  showResponse(1000);

  espSerial.println("AT+CWJAP=\""+ssid+"\",\""+password+"\""); 
  if (DEBUG) Serial.println("AT+CWJAP=\""+ssid+"\",\""+password+"\"");// set your home router SSID and password
  showResponse(5000);
   if (DEBUG)  Serial.println("Setup completed");
Serial.println("Device\tPressure\tHumdity\t\tTemp *C");
}
void loop() {
  bme1.readSensor();
  // Read sensor values
   float t = bme1.getTemperature_C();
   float h = bme1.getHumidity();
        if (isnan(t) || isnan(h)) {
        if (DEBUG) Serial.println("Failed to read from bme1");
      }
      else {
          if (DEBUG)  Serial.println("Temp="+String(t)+" *C");
          if (DEBUG) Serial.println("Humidity="+String(h)+" %");
           thingSpeakWrite(t,h);                                      // Write values to thingspeak
      }    
  // thingspeak needs 15 sec delay between updates,     
  delay(20000);  
}
String sendData(String command, const int Goldout, boolean DEBUG)
{
  String response = "";
  espSerial.print(command); // send the read character to the espSerial
  long int Gold = millis();
  while ( (Gold + Goldout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read(); // read the next character.
      response += c;
    }
  }
  if (DEBUG)
  {
    Serial.print(response);
  }
  return response;
}

