

#include <Arduino.h>


#include "Log.h"
#include "HttpHelper.h"
#include "Rtc.h"
#include "Buttons.h"

HttpHelper httph;
Rtc1302 rtc;
Buttons btns;

//NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);//0ne per day
//NTPClient timeClient(ntpUDP);//0ne per day
int8_t timeZone = 8;

const PROGMEM char *ntpServer = "pool.ntp.org";
#define NTP_TIMEOUT 1500



void setup() {
  // put your setup code here, to run once:
#ifdef _SERIAL
	Serial.begin(74800);
	logg.logging("_SERIAL is defined");
#else
	logg.logging("_SERIAL is NOT defined");
#endif
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  logg.logging("ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000="+String((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000));
	logg.logging("ESP.getFreeSketchSpace()=" + String(ESP.getFreeSketchSpace()));
	logg.logging("ESP.getFlashChipRealSize()=" + String(ESP.getFlashChipRealSize()));
  logg.logging("ESP.getFlashChipSize()=" + String(ESP.getFlashChipSize()));
	logg.logging("ESP.getChipId()=" + String(ESP.getChipId()));
  logg.logging("ESP.getSketchSize()=" + String(ESP.getSketchSize()));
  logg.logging("ESP.getVcc()=" + String(ESP.getVcc()));
  logg.logging("ESP.getCoreVersion()=" + ESP.getCoreVersion());
  	
  httph.setup();
  rtc.setup();
  btns.add(D3,LOW);

    

  //timeClient.begin();
  //NTP.setInterval (63);
  //NTP.setNTPTimeout (NTP_TIMEOUT);
  //NTP.begin (ntpServer, timeZone, false);

  
}

void processButtons(){
event_t ev;
while (btns.getEvent(&ev)){
  switch (ev.state)
  {
  case BTN_CLICK:
    logg.logging("CLICK "+ String(ev.button)+" count="+String(ev.count));
    if (ev.count==3)  if (true)
    {
      
    }else{
      
    }
    if (ev.count==2) {
      logg.logging("Success= "+ String(rtc.isSuccess())+" time="+rtc.timestring());
    }
    break;
  case BTN_LONGCLICK:
    logg.logging("LONGCLICK "+ String(ev.button)+" count="+String(ev.count));
    break;
    case BTN_RELEASED:
    //logg.logging("RELEASED "+ String(ev.button));
    break;
    case BTN_DBLCLICK:
    //logg.logging("XCLICK "+ String(ev.button));
    break;
    case BTN_PRESSED:
      logg.logging("PRESSED "+ String(ev.button));
    break;
  }
}
}

int i=0;


void loop() {
  // put your main code here, to run repeatedly:
  long ms = millis();
  httph.clientHandle();
  processButtons(); 
  rtc.loop(ms);
  delay(5);
  i+=5;
    

  if (i>500) {
    //logg.logging(timeClient.getFormattedTime());
    //logg.logging(String(timeClient.getMinutes()));
    i=0;
    
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  }
}

