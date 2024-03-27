#ifndef RECAUDIO_H_
#define RECAUDIO_H_

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=186,125
AudioAmplifier           amp2; //xy=326,148
AudioAmplifier           amp1;           //xy=334,64
AudioFilterBiquad        biquad2;        //xy=458,150
AudioFilterBiquad        biquad1;        //xy=459,64
AudioRecordQueue         queue2; //xy=595,149
AudioRecordQueue         queue1;         //xy=604,70
AudioConnection          patchCord1(i2s1, 0, amp1, 0);
AudioConnection          patchCord2(i2s1, 1, amp2, 0);
AudioConnection          patchCord3(amp2, biquad2);
AudioConnection          patchCord4(amp1, biquad1);
AudioConnection          patchCord5(biquad2, queue2);
AudioConnection          patchCord6(biquad1, queue1);
// GUItool: end automatically generated code



AudioSynthWaveformSine   sine1;          //xy=190,75
AudioConnection          patchCord9(sine1, queue1);

#endif