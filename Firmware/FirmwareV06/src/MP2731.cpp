#include "MP2731.h"

void mp2731::begin(){
    byte device;
    Wire2.begin();
    Wire2.setClock(F_clock1);
    Wire2.beginTransmission(MP2731_ADDRESS);
    device = Wire2.endTransmission();
    if(device == 0){
        Serial.print("MP2731 Found\n\r");
    }
    else{
        Serial.print("ERROR MP2731 not found.....\n\r");
    }
}

void mp2731::readADCBattvol(){
    readReg(ADC_of_battery_voltage);
}

void mp2731::readInputCurrLimit(){
    readReg(Input_current_limit);
}

void mp2731::writeInputCurrLimit(uint8_t val)
{
    writeReg(Input_current_limit,val);
}

void mp2731::writeADCcontrol(uint8_t val)
{
    writeReg(ADC_control_and_OTG_config,val);
}

void mp2731::readADCControl()
{
    readReg(ADC_control_and_OTG_config);
}

void mp2731::readchargeCurrent(){
    readReg(ADC_of_charge_current);
}

void mp2731::readinputCurrent(){
    readReg(ADC_of_input_current);
}


void mp2731::readReg(uint8_t reg){
    
    Wire2.beginTransmission(MP2731_ADDRESS);
    Wire2.write(reg);
    Wire2.endTransmission();
    Wire2.requestFrom(MP2731_ADDRESS,1);
    if(Wire2.available())
    {
        uint8_t voltage = Wire2.read();
        Serial.print("Value of Register 0x");
        Serial.print(reg,HEX);
        Serial.print(" :    ");
        Serial.print(voltage,HEX);
        Serial.println();
    }

}

void mp2731::writeReg(uint8_t reg, uint8_t val)
{
    Wire2.beginTransmission(MP2731_ADDRESS);
    Wire2.write(reg);
    Wire2.write(val);
    Wire2.endTransmission();
}
