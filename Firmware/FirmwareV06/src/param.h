#ifndef PARAM_H_
#define PARAM_H_

#include <WProgram.h>

typedef struct RecordConfig{
  String DeviceID;
  String Gain ;
  String micMode;
  String RecordMode;
  String RecordDuration ;
  String SleepDuration;
  String Count1;
  String StartDuration[8];
  String EndDuration[8];
  String uploadcheck;
  String server;
  String user;
  String pass;

}RecordConfig;

extern RecordConfig RC1;
#endif