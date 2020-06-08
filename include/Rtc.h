// Rtc.h
#ifndef _RTC_h
#define _RTC_h
#include "arduino.h"

//#include  "Config.h"

#include <ThreeWire.h>  
#include <RtcDS1302.h>

class Rtc1302
{
public:
Rtc1302();
~Rtc1302();
void setup();
bool settime(uint8_t offset);
String toString(const RtcDateTime& dt);
private:
RtcDS1302<ThreeWire> * _rtc;
RtcDateTime compiled;
};
#endif