#include <Arduino.h>
#include <TeensyThreads.h>
#include <utility/imxrt_hw.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <Audio.h>
#include <SD.h>
#include <Metro.h>

#include <bme680.h>
#include <sdcard.h>
#include <MP2731.h>
#include <ftpQueue.h>
#include "recAudio.h"
#include <param.h>

#include <SoftwareSerial.h>
#include "DEV_Config.h"
#include "L76X.h"
GNRMC GPS1;



//====================================//Flags are here//==========================================//
bool _recording = false;
bool locTime = false;
bool ftpConn = false;

//---------------------------------//Variable are here//-----------------------------------------//
volatile int id_bme;
volatile int id_interrupt;
volatile int id_record; 
volatile int id_ftptransfer;

int count = 0;
int micmode = 1;
int interrupts = 9;
int envCount = 0;
RecordConfig RC1;

//----------------------------------//Record Variable//--------------//
unsigned long ChunkSize = 0L;
unsigned long Subchunk1Size = 16;
unsigned int AudioFormat = 1;
unsigned int numChannels = 2;
unsigned long sampleRate = 48000;
unsigned int bitsPerSample = 16;
unsigned long byteRate = sampleRate * numChannels * (bitsPerSample / 8); // samplerate x channels x (bitspersample / 8)
unsigned int blockAlign = numChannels * bitsPerSample / 8;
unsigned long Subchunk2Size = 0L;
unsigned long recByteSaved = 0L;
unsigned long NumSamples = 0L;
byte byte1, byte2, byte3, byte4;

const int myInput = AUDIO_INPUT_MIC;

//----------------------filename-------------------------//
File recordFile;
char deviceID[15] = {'\0'};
char fileName[150] = {'\0'};


//----------------------------------------//Mutex defined here//-----------------------------------------//
Threads::Mutex configlock;
Threads::Mutex bmeLock;
Threads::Mutex recordlock;

//--------------------------------//Class objects//---------------------------------------------//
bme680 BME;
sdcard sd;
ftpQueue fileQueue;
Metro recordduration = Metro(10000);

//-------------------------------//function declaration//---------------------------------------//
time_t getTeensy3Time()
{ // Function for Teensy sync provider
  return Teensy3Clock.get();
}
void setGain(){   //-----------------------------Set Gain----------------------------------//
  if (RC1.Gain == "Low")
  {
    amp1.gain(10);
    amp2.gain(10);
  }
  else if (RC1.Gain == "Medium")
  {
    amp1.gain(15);
    amp2.gain(15);
  }
  else if (RC1.Gain == "High")
  {
    amp1.gain(20);
    amp2.gain(20);
  }
  else
  {
    amp1.gain(15);
    amp2.gain(15);
  }
  Serial.print("Gain Configured!\n\r");
}
void record();
void stop();
void writeOutheader();
void env();          // Env Data function
void Locationtime(); // Location and time function for Quectel
void getData();      // interrupt thread
void setI2SFreq(int);
void setTime(String);
void ftpConnect();
void appendRecord();
void appendEnv(String FileName);
void ftptransfer();
void stackCheck();
void softReset(){
  SCB_AIRCR = 0x05FA0004;
}
void recstart()
{ // function to add threads
  threads.addThread(record, 0, 4096);
}
void printtime(){
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
  Serial.print(second());
  Serial.print("\n\r");
}

//-------------------------------//Setup function//---------------------------------------------//
void setup(){ 
  
  pinMode(34, INPUT);
  pinMode(35, OUTPUT);
  
  mySerial.begin(9600); 

  mySerial.println("Hello World - SoftwareSerial");

  DEV_Set_Baudrate(9600);
  DEV_Delay_ms(500);

  /*
  Serial.begin(115200);
  Serial.print("//---------------------------Listen V1.4---------------------------------//\n\r");

  // GPIO SETUP
  setSyncProvider(getTeensy3Time);
  pinMode(interrupts, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
  digitalWrite(LED_BUILTIN, LOW);

  AudioMemory(400);

  biquad1.setHighpass(0, 200, 0.707);
  biquad2.setHighpass(0, 200, 0.707);
  
  sd.init();
  sd.createConfigFile();
  RC1 = sd.loadConfig();

  while (RC1.DeviceID == "")
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    Serial.print("NO DATA FOUND!!!!!\n");
  }

  setGain();
  setI2SFreq(48000);

  threads.addThread(stackCheck); //Function for Checking Stack Usage
  threads.addThread(getData);
  threads.addThread(env, 0, 2048);
  //threads.addThread(Locationtime);




  if(RC1.uploadcheck == "True"){
    Serial8.begin(921600);
    ftpConnect();
    threads.addThread(ftptransfer,0,2048);
  }


  if (RC1.micMode == "Mono L")
  {
    micmode = 2;
    numChannels = 1;
  }
  else if (RC1.micMode == "Mono R")
  {
    micmode = 3;
    numChannels = 1;
  }
  else
  {
    micmode = 1;
    numChannels = 2;
  }
  Serial.print("Microphone mode Configured!\n\r");

  //while(locTime==false){ } //This comment is to be removed

  if (RC1.RecordMode == "Interval Mode")
  {

    if (RC1.Count1.toInt() > 0)
    {
      for (int i = 0; i < RC1.Count1.toInt(); i++)
      {
        Alarm.alarmRepeat(RC1.StartDuration[i].toInt() / 100, RC1.StartDuration[i].toInt() % 100, 0, recstart);
        Alarm.alarmRepeat(RC1.EndDuration[i].toInt() / 100, RC1.EndDuration[i].toInt() % 100, 0, stop);
      }
    }
    else
    {
      while (!(second() % 10 == 0))
      {
      }
      recstart();
    }
  }
  else
  {
    while (!(second() % 10 == 0))
    {
    }
    recstart();
  }
  Serial.print("Mode Configured!\n\r");
  Serial.print("Setup Complete\n\r");
*/

}

void loop()
{
  Alarm.delay(0);
  GPS1 = L76X_Gat_GNRMC();
  Serial.print("\r\n");
  Serial.print("Time:");
  Serial.print(GPS1.Time_H);
  Serial.print(":");
  Serial.print(GPS1.Time_M); 
  Serial.print(":");
  Serial.print(GPS1.Time_S);
  Serial.print("\r\n");
}

//-----------------------------------//Envdata function//--------------------------------------//
void env()
{
  id_bme = threads.id();

  while (1)
  {
    threads.delay(60 * 1000);
    Threads::Scope bme(bmeLock);
    BME.saveEnv(envCount);
    BME.printData();
    count++;
    if (count == 60)
    {
      appendEnv(BME.getfileName());
      count = 0;
      envCount++;
    }
  }
}

//-----------------------------------//Loaction and time//------------------------------------//
void Locationtime(){
  String r;
  Serial8.begin(115200);

  Serial.print("AT\n\r");
  Serial8.print("AT\r\n");
  while (!Serial8.available())
  {
  }
  r = Serial8.readString();
  Serial.print(r);
  threads.delay(1000);

  Serial.print("ATE0\n\r");
  Serial8.print("ATE0\r\n");
  while (!Serial8.available())
  {
  }
  r = Serial8.readString();
  Serial.print(r);
  threads.delay(1000);

  Serial.print("AT+IPR=921600\n\r");
  Serial8.print("AT+IPR=921600\r\n");
  while (!Serial8.available())
  {
  }
  r = Serial8.readString();
  Serial.print(r);
  Serial8.begin(921600);
  threads.delay(1000);

  Serial.print("AT+IPR?\n\r");
  Serial8.print("AT+IPR?\r\n");
  while (!Serial8.available())
  {
  }
  r = Serial8.readString();
  Serial.print(r);
  threads.delay(1000);

  Serial.print("AT+QGPS?\n\r");
  Serial8.print("AT+QGPS?\r\n");
  while (!Serial8.available())
  {
  }
  r = Serial8.readString();
  Serial.print(r);
  threads.delay(1000);

  if (r == "\r\n+QGPS: 1\r\n\r\nOK\r\n")
  {
    Serial.print("AT+QGPSLOC?\n\r");
    Serial8.print("AT+QGPSLOC?\r\n");
    while (!Serial8.available())
    {
    }
    r = Serial8.readString();
    Serial.print(r);
  }
  else
  {
    Serial.print("AT+QGPS=1\n\r");
    Serial8.print("AT+QGPS=1\r\n");
    while (!Serial8.available())
    {
    }
    r = Serial8.readString();
    Serial.print(r);
    threads.delay(1000);

    Serial.print("AT+QGPSLOC?\n\r");
    Serial8.print("AT+QGPSLOC?\r\n");
    while (!Serial8.available())
    {
    }
    r = Serial8.readString();
    Serial.print(r);
    threads.delay(1000);
  }

  while (r == "\r\n+CME ERROR: 516\r\n")
  {
    threads.delay(35000);
    Serial.print("AT+QGPSLOC?\n\r");
    Serial8.print("AT+QGPSLOC?\r\n");
    while (!Serial8.available())
    {
    }
    r = Serial8.readString();
    Serial.print(r);
  }
  setTime(r);
  locTime = true;
}

//----------------------------------//Interuupt thread function//-----------------------------------//
void getData(){

  id_interrupt = threads.id();

  while (1)
  {
    if (digitalRead(interrupts) == LOW)
    {
      Serial.print("In configuration mode\n\r");

      while (!Serial.available())
      {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
      }

      sd.getConfig();
      Serial.print("exiting Confuguration Mode");
      softReset();
    }
    threads.delay(10000);
  }
}

//----------------------------------//Set sample Rate//--------------------------------------------------//
void setI2SFreq(int freq){
  // PLL between 27*24 = 648MHz und 54*24=1296MHz
  int n1 = 4; // SAI prescaler 4 => (n1*n2) = multiple of 4
  int n2 = 1 + (24000000 * 27) / (freq * 256 * n1);
  double C = ((double)freq * 256 * n1 * n2) / 24000000;
  int c0 = C;
  int c2 = 10000;
  int c1 = C * c2 - (c0 * c2);
  set_audioClock(c0, c1, c2, true);
  CCM_CS1CDR = (CCM_CS1CDR & ~(CCM_CS1CDR_SAI1_CLK_PRED_MASK | CCM_CS1CDR_SAI1_CLK_PODF_MASK)) | CCM_CS1CDR_SAI1_CLK_PRED(n1 - 1) // &0x07
               | CCM_CS1CDR_SAI1_CLK_PODF(n2 - 1);                                                                                // &0x3f
  Serial.printf("SetI2SFreq(%d)\n\r", freq);
}

//---------------------------------------------//Set Time//-----------------------------------------//
void setTime(String r){

  time_t time;
  int tim;
  int date;
  String t;
  String d;

  t = r.substring(12, 20);
  d = r.substring(69, 75);
  tim = t.toInt();
  date = d.toInt();
  int hour = tim / 10000;
  tim = tim % 10000;
  int minute = tim / 100;
  int second = tim % 100;

  int day = date / 10000;
  date = date % 10000;
  int month = date / 100;
  int year = date % 100;
  if (year > 99)
    year = year - 1970;
  else
    year += 30;

  time = makeTime(day, month, year, hour, minute, second);

  Teensy3Clock.set(time);
}

//------------------------------------------------------//Recordfunctions//-------------------------------------------//
void record()
{ // Record
  id_record = threads.id();
  while (1)
  {
    String r;
    snprintf(fileName, sizeof(fileName), "%s/record/%d%02d%02d_%02d%02d%02d_#%s.wav", RC1.DeviceID.c_str(),year(), month(), day(), hour(), minute(), second(), RC1.DeviceID.c_str());
    recordFile = SD.open(fileName, FILE_WRITE);
    if (recordFile)
    {
      queue1.begin();
      queue2.begin();
      recByteSaved = 0L;
    }
    recordduration.interval(RC1.RecordDuration.toInt() * 1000); // Record Duration;
    digitalWrite(LED_BUILTIN, HIGH);
    recordduration.reset();

    while (1)
    {
      if (recordduration.check() == 1)
      {
        recordlock.lock();
        Serial.print("Record Stop\n\r");
        appendRecord();
        queue1.end();
        queue2.end();
        if (recordFile)
        {
          while (queue1.available() > 0)
          {
            recordFile.write((byte *)queue1.readBuffer(), 256);
            recordFile.write((byte *)queue2.readBuffer(), 256);
            queue1.freeBuffer();
            queue2.freeBuffer();
            recByteSaved += 512;
          }
          writeOutheader();
          recordFile.close();
          digitalWrite(LED_BUILTIN, LOW);
          recordlock.unlock();
        }
        break;
        break;
      }
      else
      { 
        switch (micmode)
        {
        case '3':  
          if (queue1.available() >= 2) {
            recordlock.lock();
            byte *buffer = (byte *) malloc(512);
            memcpy(buffer, queue1.readBuffer(), 256);
            queue1.freeBuffer();
            memcpy(buffer + 256, queue1.readBuffer(), 256);
            queue1.freeBuffer();
            // write all 512 bytes to the SD card
            recordFile.write(buffer, 512);
            recByteSaved += 512;
            free(buffer);
            recordlock.unlock();
            }
          break;
        
        case '2':
          if (queue1.available() >= 2) {
            recordlock.lock();
            byte *buffer = (byte *) malloc(512);
            memcpy(buffer, queue2.readBuffer(), 256);
            queue1.freeBuffer();
            memcpy(buffer + 256, queue2.readBuffer(), 256);
            queue1.freeBuffer();
            // write all 512 bytes to the SD card
            Serial8.write(buffer,512);
            recordFile.write(buffer, 512);
            recByteSaved += 512;
            free(buffer);
            recordlock.unlock();
            }
          break;
        
        default:
        if (queue1.available() >= 2)
        {
          recordlock.lock();
          byte *buffer = (byte *) malloc(512);
          byte *bufferL = (byte *) malloc(256);
          byte *bufferR = (byte *) malloc(256);
          memcpy(bufferL, queue1.readBuffer(), 256);
          queue1.freeBuffer();
          memcpy(bufferR, queue2.readBuffer(), 256);
          queue2.freeBuffer();

          int b = 0;
          for (int i = 0; i < 512; i += 4)
          {
            buffer[i] = bufferL[b];
            buffer[i + 1] = bufferL[b + 1];
            buffer[i + 2] = bufferR[b];
            buffer[i + 3] = bufferR[b + 1];
            b = b + 2;
          }
          recordFile.write(buffer,512);
          Serial8.write(buffer,512);
          recByteSaved += 512;
          free(buffer);
          free(bufferL);
          free(bufferR);
          recordlock.unlock();
        }
          break;
        }
      }
    }
  
    threads.delay(RC1.SleepDuration.toInt()* 1000); // Sleep Duration;
  }
}

void stop()
{
  Serial.print("Record Stop\n\r");
  queue1.end();
  queue2.end();
  if (recordFile)
  {
    while (queue1.available() > 0)
    {
      recordFile.write((byte *)queue1.readBuffer(), 256);
      recordFile.write((byte *)queue2.readBuffer(), 256);
      queue1.freeBuffer();
      queue2.freeBuffer();
      recByteSaved += 512;
    }
    writeOutheader();
    recordFile.close();

    digitalWrite(LED_BUILTIN, LOW);
  }
}

void writeOutheader()
{ // Wav Header
  //  NumSamples = (recByteSaved*8)/bitsPerSample/numChannels;
  //  Subchunk2Size = NumSamples*numChannels*bitsPerSample/8; // number of samples x number of channels x number of bytes per sample
  Subchunk2Size = recByteSaved;
  ChunkSize = Subchunk2Size + 36;
  recordFile.seek(0);
  recordFile.write("RIFF");
  byte1 = ChunkSize & 0xff;
  byte2 = (ChunkSize >> 8) & 0xff;
  byte3 = (ChunkSize >> 16) & 0xff;
  byte4 = (ChunkSize >> 24) & 0xff;
  recordFile.write(byte1);
  recordFile.write(byte2);
  recordFile.write(byte3);
  recordFile.write(byte4);
  recordFile.write("WAVE");
  recordFile.write("fmt ");
  byte1 = Subchunk1Size & 0xff;
  byte2 = (Subchunk1Size >> 8) & 0xff;
  byte3 = (Subchunk1Size >> 16) & 0xff;
  byte4 = (Subchunk1Size >> 24) & 0xff;
  recordFile.write(byte1);
  recordFile.write(byte2);
  recordFile.write(byte3);
  recordFile.write(byte4);
  byte1 = AudioFormat & 0xff;
  byte2 = (AudioFormat >> 8) & 0xff;
  recordFile.write(byte1);
  recordFile.write(byte2);
  byte1 = numChannels & 0xff;
  byte2 = (numChannels >> 8) & 0xff;
  recordFile.write(byte1);
  recordFile.write(byte2);
  byte1 = sampleRate & 0xff;
  byte2 = (sampleRate >> 8) & 0xff;
  byte3 = (sampleRate >> 16) & 0xff;
  byte4 = (sampleRate >> 24) & 0xff;
  recordFile.write(byte1);
  recordFile.write(byte2);
  recordFile.write(byte3);
  recordFile.write(byte4);
  byte1 = byteRate & 0xff;
  byte2 = (byteRate >> 8) & 0xff;
  byte3 = (byteRate >> 16) & 0xff;
  byte4 = (byteRate >> 24) & 0xff;
  recordFile.write(byte1);
  recordFile.write(byte2);
  recordFile.write(byte3);
  recordFile.write(byte4);
  byte1 = blockAlign & 0xff;
  byte2 = (blockAlign >> 8) & 0xff;
  recordFile.write(byte1);
  recordFile.write(byte2);
  byte1 = bitsPerSample & 0xff;
  byte2 = (bitsPerSample >> 8) & 0xff;
  recordFile.write(byte1);
  recordFile.write(byte2);
  recordFile.write("data");
  byte1 = Subchunk2Size & 0xff;
  byte2 = (Subchunk2Size >> 8) & 0xff;
  byte3 = (Subchunk2Size >> 16) & 0xff;
  byte4 = (Subchunk2Size >> 24) & 0xff;
  recordFile.write(byte1);
  recordFile.write(byte2);
  recordFile.write(byte3);
  recordFile.write(byte4);
  // Serial.println("header written");
  // Serial.print("Subchunk2: ");
  // Serial.println(Subchunk2Size);
}

//-----------------------------------------------------//File transfer //---------------------------------------------------------//
void appendRecord(){
  
  fileQueue.enqueue(String(fileName));
  Serial.print("record appended\n\r");
}

void appendEnv(String FileName){
  fileQueue.enqueue(FileName);
}

void ftptransfer(){ 
  id_ftptransfer = threads.id();

  threads.delay((RC1.RecordDuration.toInt()+10)*1000);
  while (1)
  {  
  if(!fileQueue.isempty()){
    Serial.print("\n\r");
    String r;
    String uploadFile = fileQueue.dequeue();
    int str_len = uploadFile.length()+1;
    char upFile[str_len];
    uploadFile.toCharArray(upFile,str_len);

    File file;
    printtime();
    Serial.print(uploadFile);
    Serial.print("\n\r");
    Serial.print("AT+QFTPSTAT\n\r");
    Serial8.print("AT+QFTPSTAT\r\n");
    while (!Serial8.available())
    { }
    r = Serial8.readString();
    Serial.print(r);

    threads.delay(0);

    Serial.printf("AT+QFTPPUT=\"/%s\", \"COM:\",0\n\r",uploadFile.c_str());
    Serial8.printf("AT+QFTPPUT=\"%s/record/%s\", \"COM:\",0\r\n",RC1.DeviceID.c_str(),uploadFile.c_str());
    while (!Serial8.available())
    { }
    r = Serial8.readString();
    Serial.print(r);

    file = SD.open(uploadFile.c_str(),FILE_READ);
    Serial.print("File Open\n\r");

    Serial.print("Writing File\n\r");

    while (file.available())
    {
      //Serial.print("Writing File\n\r");
      Serial8.write(file.read());
      threads.delay(0);
    }
    Serial.print("Writen File\n\r");
    threads.delay(1000);
    Serial.print("+++");
    Serial8.print("+++");
    while (!Serial8.available())
    { }
    r = Serial8.readString();
    Serial.print(r);
    Serial.print("Writen File\n\r");
    threads.delay(1000);
  }
  else{
    Serial.print("No File\n\r");
    threads.delay(1000);
  }
  }
}

//-----------------------------------------------------//FTP connect//----------------------------------------------------------------//
void ftpConnect(){

  String r;
  Serial.print("AT\n\r");
  Serial8.print("AT\r\n");
  while (!Serial8.available())
  { }
  r = Serial8.readString();
  Serial.print(r);
  threads.delay(1000);

  Serial.print("AT+IPR?\n\r");
  Serial8.print("AT+IPR?\r\n");
  while (!Serial8.available())
  { }
  r = Serial8.readString();
  Serial.print(r);
  threads.delay(1000);

  Serial.print("AT+QIDEACT=1\n\r");
  Serial8.print("AT+QIDEACT=1\r\n");
  while (!Serial8.available())
  { }
  r = Serial8.readString();
  Serial.print(r);
  threads.delay(1000);

  Serial.print("AT+QIACT=1\n\r");
  Serial8.print("AT+QIACT=1\r\n");
  while (!Serial8.available())
  { }
  r = Serial8.readString();
  Serial.print(r);
  threads.delay(1000);

  Serial.print("AT+QIACT?\n\r");
  Serial8.print("AT+QIACT?\r\n");
  while (!Serial8.available())
  { }
  r = Serial8.readString();
  Serial.print(r);
  threads.delay(1000);

  Serial.print("AT+QFTPCFG=\"contextid\",1\n\r");
  Serial8.print("AT+QFTPCFG=\"contextid\",1\r\n");
  while (!Serial8.available())
  { }
  r = Serial8.readString();
  Serial.print(r);
  threads.delay(1000);

 Serial.printf("AT+QFTPCFG=\"account\",\"%s\",\"%s\"\n\r",RC1.user.c_str(),RC1.pass.c_str());
 //Serial.printf("RC1.User = %s\n\r",RC1.user.c_str());
 Serial8.printf("AT+QFTPCFG=\"account\",\"%s\",\"%s\"\n\r",RC1.user.c_str(),RC1.pass.c_str());  
 while (!Serial8.available())
  { }
  r = Serial8.readString();
  Serial.print(r);
  threads.delay(1000);

  Serial.print("AT+QFTPCFG=\"filetype\",1\n\r");
  Serial8.print("AT+QFTPCFG=\"filetype\",1\r\n");
  while (!Serial8.available())
  { }
  r = Serial8.readString();
  Serial.print(r);
  threads.delay(1000);

  Serial.print("AT+QFTPCFG=\"transmode\",1\n\r");
  Serial8.print("AT+QFTPCFG=\"transmode\",1\r\n");
  while (!Serial8.available())
  { }
  r = Serial8.readString();
  Serial.print(r);
  threads.delay(1000);

  Serial.print("AT+QFTPCFG=\"rsptimeout\",180\n\r");
  Serial8.print("AT+QFTPCFG=\"rsptimeout\",180\r\n");
  while (!Serial8.available())
  { }
  r = Serial8.readString();
  Serial.print(r);
  threads.delay(1000);

  Serial.print("AT+QFTPCFG=\"ssltype\",0\n\r");
  Serial8.print("AT+QFTPCFG=\"ssltype\",0\r\n");
  while (!Serial8.available())
  { }
  r = Serial8.readString();
  Serial.print(r);
  threads.delay(1000);


  Serial.printf("AT+QFTPOPEN=\"%s\",21\n\r",RC1.server.c_str());
  Serial8.printf("AT+QFTPOPEN=\"%s\",21\r\n",RC1.server.c_str());
  while (!Serial8.available())
  { }
  r = Serial8.readString();
  Serial.print(r);


  while (!Serial8.available())
  {  }
  r = Serial8.readString();
  Serial.print(r);
  if(r == "\r\n+QFTPOPEN: 0,0\r\n"){
  

  Serial.print("AT+QFTPCWD=\"/\" \n\r");
  Serial8.print("AT+QFTPCWD=\"/\"\r\n");
  while (!Serial8.available())
  { }
  r = Serial8.readString();
  Serial.print(r);
 
  Serial.printf("AT+QFTPMKDIR= \"%s\"\n\r",RC1.DeviceID.c_str());
  Serial8.printf("AT+QFTPMKDIR= \"%s\"\r\n",RC1.DeviceID.c_str());
  while (!Serial8.available())
  { }
  r = Serial8.readString();
  Serial.print(r);

  Serial.printf("AT+QFTPMKDIR= \"%s/record\"\n\r",RC1.DeviceID.c_str());
  Serial8.printf("AT+QFTPMKDIR= \"%s/record\"\r\n",RC1.DeviceID.c_str());
  while (!Serial8.available())
  { }
  r = Serial8.readString();
  Serial.print(r);

  Serial.printf("AT+QFTPMKDIR= \"%s/EnvData\" \n\r",RC1.DeviceID.c_str());
  Serial8.printf("AT+QFTPMKDIR= \"%s/EnvData\"\r\n",RC1.DeviceID.c_str());
  while (!Serial8.available())
  { }
  r = Serial8.readString();
  Serial.print(r);

   Serial.printf("AT+QFTPMKDIR= \"%s/battLog\" \n\r",RC1.DeviceID.c_str());
  Serial8.printf("AT+QFTPMKDIR= \"%s/battLog\"\r\n",RC1.DeviceID.c_str());
  while (!Serial8.available())
  { }
  r = Serial8.readString();
  Serial.print(r);
  
  Serial.print("FTP Configured");
  }
  else{
    Serial.print("Error Connecting to FTP\n\r");
  }

}

void stackCheck(){

  while(1){
  Serial.printf("Record Stack     : %d \n\rftptransfer Stack: %d \n\rStack Left       : %d",threads.getStackUsed(id_record),threads.getStackUsed(id_ftptransfer),threads.getStackRemaining(id_record));
  threads.delay(500);
  }

}

