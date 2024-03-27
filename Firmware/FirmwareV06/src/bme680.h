#ifndef BME680_H_
#define BME680_H_

#include <WProgram.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <SD.h>
#include <TimeLib.h>


class bme680{
private:
    bool first = true;
    float temperature, pressure, humidity;
    File file;
    char FileName[];
    
public:

    bme680();
    void saveEnv(int);
    void sendEnvToQuectel();
    void printData();
    String getfileName();
    
};




#endif