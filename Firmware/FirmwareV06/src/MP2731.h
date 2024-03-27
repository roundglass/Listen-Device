#ifndef MP2731_H_
#define MP2731_H_

#include <WProgram.h>
#include <Wire.h>

#define MP2731_ADDRESS                      (0x4B)
#define Input_current_limit                 (0x00)
#define Input_voltage_regulation            (0x01)
#define NTC_config_and_thermal_regulation   (0x02)
#define ADC_control_and_OTG_config          (0x03)
#define Charge_control_and_VSYS_config      (0x04)
#define Charge_current_config               (0x05)
#define Precharge_and_termination_curren    (0x06)
#define Charge_voltage_regulation           (0x07)
#define Timer_configuration                 (0x08)
#define Bandgap                             (0x09)
#define BATFET_configuration                (0x0A)
#define INT_MASK_and_USB_detection          (0x0B)
#define status                              (0x0C)
#define Fault                               (0x0D)
#define ADC_of_battery_voltage              (0x0E)
#define ADC_of_system_voltage               (0x0F)
#define ADC_of_NTC_voltage                  (0x10)
#define ADC_of_input_voltage                (0x11)
#define ADC_of_charge_current               (0x12)
#define ADC_of_input_current                (0x13)
#define Power_management_status             (0x14)
#define DPM_mask                            (0x15)
#define JEITA_configuration                 (0x16)
#define Safety_timer_status_and_part_number (0x17)


#define F_clock1                            100000
#define F_clock2                            400000    
class mp2731{
    public:

    void begin();
    void readADCBattvol();
    void readInputCurrLimit();
    void writeInputCurrLimit(uint8_t val);
    void writeADCcontrol(uint8_t);
    void readADCControl();
    void readchargeCurrent();
    void readinputCurrent();

    protected:
    void writeReg(uint8_t reg,uint8_t val);
    void readReg(uint8_t val);

};

#endif