# OPENSDL
An open source data logger for monitoring temperature, RH, and light levels (illuminance) with the help of an Arduino Uno
This is a DIY for developing the OPENSDL data logger.

Do It Yourself (DIY)
Time required: 2-3 hours for soldering, 5 hours for packaging (4 hours - 3D printing, and 1 hour for laser cutting) 
Skills required: Soldering, little or no knowledge in programming and electronics
Step #1: Parts required:
Table 16 Parts list for OPENSDL
Arduino Uno with cable	Data logger shield 
CR1220 coin cell battery 	BME280 temperature humidity pressure sensor breakout board
TSL2561 light sensor breakout board	ESP01-8266 Wi-Fi module
RJ-9 male and female connector	Shield stacking headers for Arduino 
SD memory card (any capacity)	Vector board (26 x 18 holes)
8 AA batteries	Battery holder

Required Accessories
•	Soldering iron (35W)
•	Solder
•	Wire cutter
•	Crimper tool
•	Multimeter
Software required
•	Arduino IDE (1.0.5 or higher)
Arduino libraries required
•	Wire library
•	SparkFun TSL2561 library
•	Cactus BME280 multisensor library
•	SD card library
•	SPI library
•	RTC library

Note: The BME280 sensor is a very accurate, temperature, relative humidity and pressure sensor from Bosch. Similarly, the DS1307 is an accurate real time clock from Maxim and the TSL2561 is an accurate light sensor. There are less expensive and less accurate alternatives for these products, but this tutorial was aimed towards people who were interested in gathering data for building performance evaluation and building monitoring applications that require high precision and accuracy. This means that any specific hardware setup and software setup (libraries, program code) was strictly meant for the specified products only.

Step #2: Assembly
The data logger shield can be easily stacked on top of the Arduino Uno board. This shield provides the data logging capabilities (time keeping and data storage). The shield had to be stacked. A CR1220 coin cell battery had to be inserted in the round slot provided to keep the clock running even when the Arduino is powered off. The SD memory card must be inserted in the on-board card slot provided. 
A unique customized shield was developed by using RJ-9 connector female pins and Arduino shield stacking headers. The appropriate headers were soldered in the appropriate locations so as the shield fits perfectly on the Arduino board. The Arduino has 18 pins on one side and 14 pins on the other side. The headers with same number of pins were used at same spacing (18 pins apart) as on Arduino. The remaining extra space adjacent to the headers was used for placing the RJ-9 connector.
 

The headers were the best way to use the required pins, while making them still available for use to other components. The sensors used follow the I2C communication protocol, which requires 4 pins from Arduino, namely: SDA (also available as A4), SCL (also available as A5), 3.3V & GND. The four wires coming out from the RJ-9 connector were soldered into these four header pins. The number of RJ-9 connectors required depends on the number of sensors. In this project, 3 RJ-9 connectors were used (two for BME280 & one for TSL2561). 








The four wires coming out of the RJ-9 connector were color coded, and each color wire was designated a specific pin for all the RJ-9 connectors. It must be noted that the color code may vary on different RJ-9 pieces. In such a case, the location of the wire on the connector must be noted. The RJ-9 connector, after soldering, was made to stick on the vector board using a Feviqwik, so that it gets fixed on the surface. 
                                      

These connections can be verified by using the continuity mode on the multimeter. When in continuity mode, the multimeter should show zero resistance. Connect one of the probes of the multimeter to the soldered pin, and another probe to the pin inside the RJ-9 connector. The multimeter should emit a tone, which means that the solder joints are proper, and the connections have been made properly. If the tone is not emitted, check the solder joints.
Similarly, solder the RJ-9 connector with same wires connecting to the same pinholes on the sensor breakout boards, i.e. A4, A5, 3.3V & GND.
 
The BME280 sensor supports two I2C addresses, meaning that two BME280 sensors can be connected to the same controller at once. While doing so, the address of one of the sensors has to be changed by bridging the solder pads on the sensor. An ESP-01 wireless connection chip required the following connections with the Arduino.
Table 17 ESP01 and Arduino interfacing
ESP-01	Arduino Uno
10	TX
11	RX
Vcc	CH_PD
Vcc	Vcc
GND	GND

Note:- The multiple LEDs; on Arduino Uno were removed for improving the battery life. The power indicator LED, RX, and TX LEDs were removed by heating up the solder joints and pushing the LED with forceps.
Step #3: Setup IDEs and libraries
Before doing any programming, Arduino IDE (Integrated Development Environment) must be downloaded. The programming was done on this platform. Different libraries were required to interact with different components of OPENSDL. The following libraries were used for the given components (refer Table 18).
Table 18 Software libraries for different components
Component	Library
BME280 temperature & RH sensor	Cactus_io_BME280_I2C.h
Light sensor	SparkFun TSL2561.h
Real time clock	RTClib.h
SD card socket	SD.h
I2C connection	Wire.h
	
A separate library for communicating with the ESP01 is not required as the code uploaded in the Arduino has AT commands, which are sent to the serial monitor, from where the ESP-01 takes the instructions. So, basically, the AT commands by which ESP01 runs, are printed into the Serial Monitor, which are taken as an input command by ESP-01. For installing these libraries, after downloading them, open the Arduino IDE, go to Sketch -> Include Library -> Add .Zip library, and select the downloaded libraries. 

Step #4: Programming the system
Before programming the OPENSDL, connect the Arduino with a laptop. After connecting, go to Tools -> Port, and select the COM port in which the OPENSDL is connected. Also, make sure that under Tools -> Boards, Arduino Uno is selected. Refer Figure 18 for selecting board and port.
 
Figure 18 Arduino IDE board and port selection
The OPENSDL was developed to work in 2 modes. In the first mode, it stores the data on the SD card on the data logger shield. In second mode, it sends the data over the internet to a website by using an ESP-01 Wi-Fi chip. The program for both the modes is different. The program for both the modes is provided in the Appendix A.
 of this document. These lines of code can be directly copied and pasted in the Arduino IDE editor, and used directly. Once in the code, we need to make a few customizations as per our needs:
1.	Manually change the value of delay (1000) at the end of the code to change the logging interval. The value 1000 represents interval in milliseconds.
2.	Edit the line of code which says mySensorData = SD.open("Logged01.csv", FILE_WRITE); and replace the Logged01 with the file name of the desired file name. The extension of the file can also be changed by modifying the .csv extension just after the file name. 
3.	The calibration equation achieved by finding the correlation between the Master/reference sensor and the BME280 will vary with each sensor. Replace this line of code with the equation for calibrating the sensors:   Serial.print((1.0533*t2)-2.2374) – for sensor with default address (0x77), where t2 is the value read from the temperature sensor. 

A separate program has been provided for programming the second available mode of OPENSDL, which is the wireless system. The ESP-01 must be connected to the OPENSDL according to the connections as explained in the Step #2. After completing the connections, connect the Arduino to the laptop, and upload a blank sketch in the Arduino. Put the ESP-01 in the update mode and update the firmware to the latest available update. After updating, make sure to connect the reset pin of Arduino with the 3.3V pin, which bypasses the Arduino bootloader. 


Step #5: Fabrication
An enclosure for OPENSDL was created for protection and to improve the aesthetics. The casings were developed by 3D printing using PLA material, and the casing for the microcontroller was developed by laser cutting the MDF sheet and gluing the pieces together. The 3D printed models were developed by using SketchUp software, and the 2D dxf drawings for laser cutting were created by using AutoCAD. 

These drawings are not to scale and are for representation purposes only.
 






For 3D printing, the STL files produced by using SketchUp were opened and checked in Ultimaker Cura 3.2.1 software. Make sure that PLA material is used, and the nozzle of the printer used is for 0.4mm printing. The build plate of the 3D printer may require glue to stick the 3D printed object. But when the printing is complete, the glue creates a strong adhesion between the printed object and the build plate. 

 

