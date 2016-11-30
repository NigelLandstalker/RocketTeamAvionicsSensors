#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(20);

File datalog;
char filename[] = "LOGGER00.csv";

void setup() {
  Serial.begin(115200);
  // Initalize SD Card  
 
  Serial.print("Intializing SD Card...");
  pinMode(10, OUTPUT); 
  
  if(!SD.begin(chipSelect)) 
  {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("Card initialized.");  
  
  Serial.print("Creating File...");
  

// Make a new file each time the arduino is powered

  for (uint8_t i = 0; i < 100; i++) 
  {  
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    
    if (!SD.exists(filename)) 
    {
      // only open a new file if it doesn't exist
      datalog = SD.open(filename, FILE_WRITE); 
      break;
    }
  }
  
  Serial.print("Logging to: ");
  Serial.println(filename);
  
  if(!datalog)
  {
  Serial.println("Couldn't Create File"); 

  return;
  }
  
   /* Initialise the sensors */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  if(!gyro.begin())
  {
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.print("Ooops, no L3GD20 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
}

void loop() {
  sensors_event_t event;
  

}
