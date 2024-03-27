#ifndef GUI_H_
#define GUI_H_

#include <WProgram.h>
#include <TimeLib.h>
#include <EEPROM.h>
#include "param.h"

class GUI{
private:
    void saveDataEEPROM();
    void getEEPROMData();
    RecordConfig RC1;
public:
    GUI();
    void getConfig();
    RecordConfig loadConfig();
};





#endif