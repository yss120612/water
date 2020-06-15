
#include "Rtc.h"
#include  "Config.h"
#include  "Log.h"

Rtc1302::Rtc1302(){
_short_interval=120000;
last_update=0;
upd_success=false;
}

Rtc1302::~Rtc1302(){

if (_rtc!=NULL) delete(_rtc);
if (_tw!=NULL) delete(_tw);
if (timeClient!=NULL) delete(timeClient);
if (ntpUDP!=NULL) delete(ntpUDP);
}


bool Rtc1302::settime(uint8_t offset){
    
return true;
}

void Rtc1302::setup(int interval){
_tw=new ThreeWire(DS1302_DAT,DS1302_CLK,DS1302_RST);// IO, SCLK, CE   
//ThreeWire myWire(DS1302_DAT,DS1302_CLK,DS1302_RST); 

_rtc= new RtcDS1302<ThreeWire>(*_tw);
_interval=interval;
ntpUDP=new WiFiUDP();

timeClient = new NTPClient(*ntpUDP ,ntp_server , 3600*TIME_OFFSET, _interval);

//RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
timeClient->begin();
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

bool Rtc1302::setMemory(uint8_t d,uint8_t addr){
    _rtc->SetMemory(&d,1);
}

String Rtc1302::test(){
String res;
res="Memory one="+String(_rtc->GetMemory(0));
res+=" WP one="+String(_rtc->GetIsWriteProtected());
_rtc->SetIsRunning(true);
_rtc->SetIsWriteProtected(false);
//RtcDateTime dt(2020,6,15,17,5,0);
//_rtc->SetDateTime(dt);
_rtc->SetMemory((uint8_t)0,(uint8_t)7);

//_rtc->SetDateTime()
res+=" Memory two="+String(_rtc->GetMemory(0));
res+=" WP two="+String(_rtc->GetIsWriteProtected());
return res;    
}

String Rtc1302::timestring()
{
    if (_rtc->IsDateTimeValid())
    return toString(_rtc->GetDateTime());
    else
    return "Incorrect DateTime";
    
}


