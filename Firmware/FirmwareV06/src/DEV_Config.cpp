
#include "DEV_Config.h"

SoftwareSerial mySerial(34, 35); // RX, TX

/******************************************************************************
function: 
  Uart receiving and sending
******************************************************************************/
UBYTE DEV_Uart_ReceiveByte()
{
  while(1){
    if(mySerial.available()){
      return  mySerial.read();
    }
  }
}

void DEV_Uart_SendByte(char data)
{
  mySerial.write(data);
}

void DEV_Uart_SendString(char *data)
{
  UWORD i;
  for(i=0; data[i] != '\0'; i++){
    mySerial.write(data[i]);
  }
}

void DEV_Uart_ReceiveString(char *data, UWORD Num)
{  
  UWORD i;
  while(1){
    if(mySerial.available()){
        data[i] =  mySerial.read();
        i++;
        //Serial.print(data[i]);
        if(i >= Num){
            break;
          }
      }
   }

  data[Num-1] = '\0';
}

void DEV_Set_GPIOMode(UWORD Pin, UWORD mode)
{

  if(mode == 1){
    pinMode(Pin, INPUT);
  }
  else if(mode == 0){
    pinMode(Pin, OUTPUT);
  }
}


void DEV_Set_Baudrate(UDOUBLE Baudrate)
{
  mySerial.begin(Baudrate);
}
