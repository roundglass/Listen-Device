#include "sdcard.h"

sdcard::sdcard(){}

void sdcard::init(){

    while(!SD.begin(BUILTIN_SDCARD))
    {
      Serial.print("SD Card not found\n\r");
      delay(500);
    }

   // RC1 = GUI::loadConfig();
    RC1 = GUI::loadConfig();
}

void sdcard::getConfig(){

  

    GUI::getConfig();

}

RecordConfig sdcard::loadConfig(){
    return (RC1);
}

void sdcard::createConfigFile(){

    RC1.DeviceID = "FTP_TEST";
    RC1.RecordDuration = "300";
    RC1.uploadcheck = "True";
    RC1.server = "ftp.dlptest.com";
    RC1.user = "dlpuser";
    RC1.pass = "rNrKYTX9g7z3RgJRmxWuGHbeu";

    char deviceID[15] = {'\0'};
    RC1.DeviceID.toCharArray(deviceID, 15);
    
    strcpy(recordDir,deviceID);
    strcat(recordDir,"/record");
    SD.mkdir(recordDir);
    Serial.print(recordDir);
    Serial.print("  ");
    Serial.print("RecordDir Created \n\r");
   
    strcpy(envDir,deviceID);
    strcat(envDir,"/envData");
    SD.mkdir(envDir);
    Serial.print(envDir);
    Serial.print("  ");
    Serial.print("envDir Created \n\r");
   
    strcpy(battDir,deviceID);
    strcat(battDir,"/batteryLog");
    SD.mkdir(battDir);
    Serial.print(battDir);
    Serial.print("  ");
    Serial.print("battDir Created \n\r");

    if(SD.exists("config.txt")){
        SD.remove("config.txt");
    }
    file = SD.open("config.txt",FILE_WRITE);

    file.print("DeviceID: ");
    file.print(RC1.DeviceID);
    file.print("\n\r");

    file.print("Gain: ");
    file.print(RC1.Gain);
    file.print("\n\r");

    file.print("Recording Mode: ");
    file.print(RC1.RecordMode);
    file.print("\n\r");

    file.print("Record Duration: ");
    file.print(RC1.RecordDuration);
    file.print("\n\r");

    if(RC1.RecordMode =="Interval Mode"){
      file.print("Sleep Duration: ");
      file.print(RC1.SleepDuration);
      file.print("\n\r");

      file.print("Start and End Recording: \n\r");
      if(RC1.Count1.toInt() > 0){
        for(int i=0;i<RC1.Count1.toInt();i++){
          file.print(RC1.StartDuration[i]);
          file.print(" - ");
          file.print(RC1.EndDuration[i]);
          file.print("\n");
        }
        file.print("\r");
      }
      else{
        file.print("NA\n\r");
      }
    }
    
    file.close();



}



