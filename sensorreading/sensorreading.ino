#include <Wire.h>
#include <SD.h>
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

  for (uint8_t i = 0; i < 100; i++){  
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    
    if (!SD.exists(filename)){
      // only open a new file if it doesn't exist
      datalog = SD.open(filename, FILE_WRITE); 
      break;
    }
  }
  Serial.print("Logging to: ");
  Serial.println(filename);
  
  if(!datalog){
    datalog.println("Couldn't Create File");
    return;
  }
  
   /* Initialise the sensors */
  if(!accel.begin()){
    /* There was a problem detecting the ADXL345 ... check your connections */
    datalog.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }
  if(!mag.begin()){
    /* There was a problem detecting the LSM303 ... check your connections */
    datalog.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  if(!bmp.begin()){
    /* There was a problem detecting the BMP085 ... check your connections */
    datalog.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  if(!gyro.begin()){
    /* There was a problem detecting the L3GD20 ... check your connections */
    datalog.print("Ooops, no L3GD20 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
}

void loop() {
  sensors_event_t event;

  if(datalog){
    //PRINT ACCELEROMETER DATA
    accel.getEvent(&event);
    datalog.print(F("ACCEL "));
    datalog.print("X: "); datalog.print(event.acceleration.x); datalog.print("  ");
    datalog.print("Y: "); datalog.print(event.acceleration.y); datalog.print("  ");
    datalog.print("Z: "); datalog.print(event.acceleration.z); datalog.print("  ");datalog.println("m/s^2 ");
  
    /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
    //PRINT MAGNETIC SENSOR DATA
    mag.getEvent(&event);
    datalog.print(F("MAG   "));
    datalog.print("X: "); datalog.print(event.magnetic.x); datalog.print("  ");
    datalog.print("Y: "); datalog.print(event.magnetic.y); datalog.print("  ");
    datalog.print("Z: "); datalog.print(event.magnetic.z); datalog.print("  ");datalog.println("uT");
  
    /* Display the results (gyrocope values in rad/s) */
    //PRINT GYRO DATA
    gyro.getEvent(&event);
    datalog.print(F("GYRO  "));
    datalog.print("X: "); datalog.print(event.gyro.x); datalog.print("  ");
    datalog.print("Y: "); datalog.print(event.gyro.y); datalog.print("  ");
    datalog.print("Z: "); datalog.print(event.gyro.z); datalog.print("  ");datalog.println("rad/s ");  
  
    /* Display the pressure sensor results (barometric pressure is measure in hPa) */
    //PRINT PRESSURE SENSOR DATA
    bmp.getEvent(&event);
    if (event.pressure){
      /* Display atmospheric pressure in hPa */
      datalog.print(F("PRESS "));
      datalog.print(event.pressure);
      datalog.print(F(" hPa, "));
      /* Display ambient temperature in C */
      float temperature;
      bmp.getTemperature(&temperature);
      datalog.print(temperature);
      datalog.print(F(" C, "));
      /* Then convert the atmospheric pressure, SLP and temp to altitude    */
      /* Update this next line with the current SLP for better results      */
      float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
      datalog.print(bmp.pressureToAltitude(seaLevelPressure,
                                          event.pressure,
                                          temperature)); 
      datalog.println(F(" m"));
    }
    
    datalog.println(F(""));
      
  }
  delay(1);
}
