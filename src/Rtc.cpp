
#include "Rtc.h"
#include  "Config.h"
#include  "Log.h"

Rtc1302::Rtc1302(){

}

Rtc1302::~Rtc1302(){
if (_rtc!=NULL) delete(_rtc);
}


bool Rtc1302::settime(uint8_t offset){
    
return true;
}

void Rtc1302::setup(){
ThreeWire myWire(DS1302_DAT,DS1302_CLK,DS1302_RST); // IO, SCLK, CE
_rtc= new RtcDS1302<ThreeWire>(myWire);
//Serial.print(__DATE__);
//Serial.println(__TIME__);
RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

_rtc->Begin();

logg.logging(toString(compiled));

}

String Rtc1302::toString(const RtcDateTime& dt)
{
    char datestring[20];
    String s;
    
    snprintf_P(datestring, 
            20,
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Day(),
            dt.Month(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );

    return String(datestring);
}

