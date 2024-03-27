#ifndef sdcard_H_
#define sdcard_H_

#include <WProgram.h>
#include <GUI.h>
#include <SD.h>
#include "param.h"

class sdcard:private GUI{
public:
    sdcard();
    void init();
    void getConfig();
    RecordConfig loadConfig();
    void createConfigFile();

private:
    File file;
    RecordConfig RC1;
    char recordDir[];
    char envDir[];
    char battDir[];
};

#endif