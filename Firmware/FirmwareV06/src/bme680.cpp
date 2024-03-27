#include <bme680.h>
Adafruit_BME680 atmos(&Wire2);
 
bme680::bme680(){
    atmos.begin();
    
}

void bme680::saveEnv(int count){

    Serial.print("Writing Env data to SD\n\r");
    /*
    //Error Code
    char FileName[] = "EnvData.csv";
    char Count[] = "\0";
    String r = String(count);
    r.toCharArray(Count,16);
    strcat(FileName, Count);
    //
    */
    if(first == true){
        file = SD.open("EnvData.csv",FILE_WRITE);
        file.print("Date&time,Temperature,Humidity,Pressure");
        file.println();
        first = false;
    }
    else{
    file = SD.open("EnvData.csv",FILE_WRITE);
    }

    if(file){
    file.print(year());
    file.print("-");
    file.print(month());
    file.print("-");
    file.print(day());
    file.print(" ");
    file.print(hour());
    file.print(":");
    if (minute() < 10)
        file.print('0');
    file.print(minute());
    file.print(":");
    if (second() < 10)
        file.print('0');
    file.print(second());
    file.print(",");
    file.print(atmos.temperature);
    file.print(",");
    file.print(atmos.humidity);
    file.print(",");
    file.print(atmos.pressure);
    file.print(",");

    file.println();
    }
    file.close();
}

void bme680::sendEnvToQuectel(){
    
    Serial.print("Sending data to Quectel EC20\n\r");
    file = SD.open("EnvData.csv",FILE_WRITE);
    String r;
    Serial.printf("AT+QFUPL=\"SD:EnvData.csv,%ld\"\n\r",file.size());
    Serial8.printf("AT+QFUPL=\"SD:EnvData.csv,%ld\"\r\n",file.size());
    
    while (!Serial8.available()){}
    r = Serial8.readString();
    Serial.print(r);
    while(file.available()){
      Serial8.write(file.read());}
    while(!Serial8.available()){}                                 
    r = Serial8.readString();
    Serial.print(r);

    file.close();

    SD.remove("EnvData.csv");

}

void bme680::printData(){

    Serial.print(year());
    Serial.print("-");
    Serial.print(month());
    Serial.print("-");
    Serial.print(day());
    Serial.print(" ");
    Serial.print(hour());
    Serial.print(":");
    if (minute() < 10)
        Serial.print('0');
    Serial.print(minute());
    Serial.print(":");
    if (second() < 10)
        Serial.print('0');

    Serial.printf("\n\r Temperature - %d \n\r Humidity - %d \n\r Pressure - %d\n\r",atmos.readTemperature(),atmos.readHumidity(),atmos.readPressure());
}


String bme680::getfileName(){
    return(String(FileName));
}