
#include "Rtc.h"
#include  "Config.h"
#include  "Log.h"

Rtc1302::Rtc1302(){
_short_interval=60000;
last_update=0;
upd_success=false;
}

Rtc1302::~Rtc1302(){
if (_rtc!=NULL) delete(_rtc);
if (timeClient!=NULL) delete(timeClient);
if (ntpUDP!=NULL) delete(ntpUDP);
}


bool Rtc1302::settime(uint8_t offset){
    
return true;
}

void Rtc1302::setup(int interval){
ThreeWire myWire(DS1302_DAT,DS1302_CLK,DS1302_RST); // IO, SCLK, CE
_rtc= new RtcDS1302<ThreeWire>(myWire);
_interval=interval;
ntpUDP=new WiFiUDP();
timeClient = new NTPClient(*ntpUDP ,ntp_server , 3600*TIME_OFFSET, _interval);
timeClient->begin();
//RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

_rtc->Begin();

//logg.logging(toString(compiled));

}

void Rtc1302::loop(long ms)
{
    if (!timeClient)
        return;
    

    
    if (upd_success)
    {
        if (ms - last_update > _interval)
        {
            upd_success = timeClient->forceUpdate();
            if (upd_success)
            {
              
                setfrominet();
            }else 
            {
                logg.logging("Error update time on long period");
            }
            last_update=ms;
        }
    }
    else
    {
        if (ms - last_update > _short_interval)
        {
            upd_success = timeClient->forceUpdate();
            if (upd_success)
            {
             
                setfrominet();
            }else 
            {
                
                logg.logging("Error update time on short period");
            }
            last_update=ms;
        }
    }
}

void Rtc1302::setfrominet(){
    RtcDateTime d;
    
    d.InitWithEpoch64Time(timeClient->getEpochTime());
    logg.logging(String(d.Hour())+":"+String(d.Minute())+":"+String(d.Second()));
    _rtc->SetDateTime(d);

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


String Rtc1302::timestring()
{
    return toString(_rtc->GetDateTime());
}


