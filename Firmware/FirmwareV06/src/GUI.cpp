#include "GUI.h"

GUI::GUI(){}

void GUI::getConfig(){
    String s = Serial.readStringUntil('\n');
    RC1.DeviceID = Serial.readStringUntil('\n');
    RC1.Gain = Serial.readStringUntil('\n');
    RC1.micMode = Serial.readStringUntil('\n');
    RC1.RecordMode = Serial.readStringUntil('\n');
    RC1.RecordDuration = Serial.readStringUntil('\n');
  
    if(RC1.RecordMode =="Interval Mode"){
        RC1.SleepDuration = Serial.readStringUntil('\n');
        RC1.Count1 = Serial.readStringUntil('\n');
        if(RC1.Count1.toInt() > '8'){
        RC1.Count1 = '8';
        }
        if(RC1.Count1.toInt()>0){
            for(int i=0;i<RC1.Count1.toInt();i++){
            RC1.StartDuration[i] = Serial.readStringUntil('\n');
            RC1.EndDuration[i] = Serial.readStringUntil('\n');
            }
        }
    }
   RC1.uploadcheck = Serial.readStringUntil('\n');
    if (RC1.uploadcheck == "True")
    {
      RC1.server = Serial.readStringUntil('\n');
      RC1.user   = Serial.readStringUntil('\n');
      RC1.pass   = Serial.readStringUntil('\n');
    }
    
    
    saveDataEEPROM();

}

void GUI::saveDataEEPROM(){
    int eeAddress = 0;
    EEPROM.put(eeAddress,RC1.DeviceID);
    eeAddress += sizeof(RC1.DeviceID);
   
    EEPROM.put(eeAddress,RC1.Gain);
    eeAddress += sizeof(RC1.Gain);
   
    EEPROM.put(eeAddress,RC1.micMode);
    eeAddress += sizeof(RC1.micMode);
   
    EEPROM.put(eeAddress,RC1.RecordDuration);
    eeAddress += sizeof(RC1.RecordDuration);
   
    EEPROM.put(eeAddress,RC1.RecordMode);
    eeAddress += sizeof(RC1.RecordMode);

    if(RC1.RecordMode =="Interval Mode")
    {
        EEPROM.put(eeAddress,RC1.SleepDuration);
        eeAddress += sizeof(RC1.SleepDuration);

        EEPROM.put(eeAddress,RC1.Count1);
        eeAddress += eeAddress;

      if(RC1.Count1.toInt()>0){
         for(int i=0;i<RC1.Count1.toInt();i++){
            EEPROM.put(eeAddress,RC1.StartDuration[i]);
            eeAddress += eeAddress;

            EEPROM.put(eeAddress,RC1.EndDuration[i]);
            eeAddress += eeAddress;
         }
      }

   }
   EEPROM.put(eeAddress,RC1.uploadcheck);
   eeAddress += eeAddress;
   if (RC1.uploadcheck == "True")
   {
      EEPROM.put(eeAddress,RC1.server);
      eeAddress += eeAddress;
      EEPROM.put(eeAddress,RC1.user);
      eeAddress += eeAddress;
      EEPROM.put(eeAddress,RC1.pass);
      eeAddress += eeAddress;
   }
}

void GUI::getEEPROMData(){

   int eeAddress = 0;

   EEPROM.get(eeAddress,RC1.DeviceID);
   eeAddress += sizeof(RC1.DeviceID);

   EEPROM.get(eeAddress,RC1.Gain);
   eeAddress += sizeof(RC1.Gain);

   EEPROM.get(eeAddress,RC1.micMode);
   eeAddress += sizeof(RC1.micMode);

   EEPROM.get(eeAddress,RC1.RecordDuration);
   eeAddress += sizeof(RC1.RecordDuration);
   
   EEPROM.get(eeAddress,RC1.RecordMode);
   eeAddress += sizeof(RC1.RecordMode);

   if(RC1.RecordMode =="Interval Mode")
   {
      EEPROM.get(eeAddress,RC1.SleepDuration);
      eeAddress += sizeof(RC1.SleepDuration);

      EEPROM.get(eeAddress,RC1.Count1);
      eeAddress += eeAddress;

      if(RC1.Count1.toInt()>0){
         for(int i=0;i<RC1.Count1.toInt();i++){
            EEPROM.get(eeAddress,RC1.StartDuration[i]);
            eeAddress += eeAddress;

            EEPROM.get(eeAddress,RC1.EndDuration[i]);
            eeAddress += eeAddress;
         }
      }
   }
   EEPROM.get(eeAddress,RC1.uploadcheck);
   eeAddress += eeAddress;
   if (RC1.uploadcheck == "True")
   {
      EEPROM.get(eeAddress,RC1.server);
      eeAddress += eeAddress;
      EEPROM.get(eeAddress,RC1.user);
      eeAddress += eeAddress;
      EEPROM.get(eeAddress,RC1.pass);
      eeAddress += eeAddress;
   }


}

RecordConfig GUI::loadConfig(){
    
    getEEPROMData();
    return RC1;

}