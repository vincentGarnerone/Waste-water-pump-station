/**
 * Waste water pumping station
 * 
 * Author Vincent Garnerone
 * 
 *
 * 
 * Components: Arduino UNO, DYPA02YYUM  v1.0
 * 
 * Arduino UNO +5V    --> DYPA02YYUM Pin 1 Red
 * Arduino UNO GND    --> DYPA02YYUM Pin 2 Black
 * Arduino UNO Pin 11 --> DYPA02YYUM Pin 3 or Floating
 * Arduino UNO Pin 10 --> DYPA02YYUM Pin 4
 * 
 * Since Arduino UNO does not have 2 hardware serial ports.
 * We are using 1 software serial port connected to the sensor.
 * All data from software serial port is copied onto hardware serial port to view in "Arduino IDE Serial Monitor".
 */

#include <SoftwareSerial.h>

SoftwareSerial ss (10, 11);   // RX, TX

byte hdr, data_h, data_l, chksum;
unsigned int distance;

const int PUMP = 6;
float duration;
float h1 = 100; // High level, close to the sensor: 100mm from the sensor
float h2 = 330; // Low level, far from the sensor: 330mm from the sensor
int dist;

void setup() {
  pinMode(PUMP, OUTPUT); // define the pump as an output
  digitalWrite(PUMP, LOW); // the pump is off at the beginning  
  Serial.begin(57600);  
  while (!Serial);
  ss.begin(9600);
}

void loop() {

  if (getDistance()<h1){ 
    while (getDistance()<h2){
      digitalWrite(PUMP, HIGH); // switch on the PUMP once we are close enough, and keep on until we are far enough.
      Serial.println(getDistance());
      Serial.println("Pump on");
      delay(100);
    }
  }
  digitalWrite(PUMP, LOW); // switch off the pump
  Serial.println(getDistance());
  Serial.println("Pump off");
  delay(100); 
}

float getDistance(){  // return the distance between the water level and the top of the tank
                      // code from Ritesh Talreja, Made in China, Warehouse: Shenzhen, Guangdong.
                      // This code was given with the sensor
 
if (ss.available())
  {
    hdr = (byte)ss.read();
    if (hdr == 255)
    {
      data_h = (byte)ss.read();
      data_l = (byte)ss.read();
      chksum = (byte)ss.read();

      if (chksum == ((hdr + data_h + data_l)&0x00FF))
      {
        Serial.print(hdr);
        Serial.print(",");
        Serial.print(data_h);
        Serial.print(",");
        Serial.print(data_l);
        Serial.print(",");
        Serial.print(chksum);
        
        Serial.print("=");
      
        Serial.print(hdr, HEX);
        Serial.print(",");
        Serial.print(data_h, HEX);
        Serial.print(",");
        Serial.print(data_l, HEX);
        Serial.print(",");
        Serial.print(chksum, HEX);
        Serial.print(" => ");
  
        distance = data_h * 256 + data_l;
        Serial.print(distance, HEX);
        Serial.print("=");
        Serial.print(distance, DEC);
        Serial.println(" mm");
        return distance;
      }
    }
  }
  delay(100);
  }
