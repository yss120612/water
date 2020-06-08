

#include <Arduino.h>
#include <NtpClientLib.h>

#include "Log.h"
#include "HttpHelper.h"
#include "Rtc.h"
#include "Buttons.h"

HttpHelper httph;
Rtc1302 rtc;
Buttons btns;
//WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000*60*24);//0ne per day
//NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);//0ne per day
//NTPClient timeClient(ntpUDP);//0ne per day
int8_t timeZone = 1;
int8_t minutesTimeZone = 0;
const PROGMEM char *ntpServer = "pool.ntp.org";
#define NTP_TIMEOUT 1500


boolean syncEventTriggered = false; // True if a time even has been triggered
NTPSyncEvent_t ntpEvent; // Last triggered event

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

    NTP.onNTPSyncEvent ([](NTPSyncEvent_t event) {
        ntpEvent = event;
        syncEventTriggered = true;
    });

  //timeClient.begin();
  NTP.setInterval (63);
  NTP.setNTPTimeout (NTP_TIMEOUT);
  NTP.begin (ntpServer, timeZone, true, minutesTimeZone);

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
      logg.logging("Success update "+NTP.getTimeDateString ());
    }else{
      logg.logging("Failed update");
    }
    if (ev.count==2) {
      ///logg.logging("Errot="+String(ntpUDP.getWriteError())+" port="+String(ntpUDP.remotePort()));
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
void processSyncEvent (NTPSyncEvent_t ntpEvent) {
    if (ntpEvent < 0) {
        Serial.printf ("Time Sync error: %d\n", ntpEvent);
        if (ntpEvent == noResponse)
            Serial.println ("NTP server not reachable");
        else if (ntpEvent == invalidAddress)
            Serial.println ("Invalid NTP server address");
        else if (ntpEvent == errorSending)
            Serial.println ("Error sending request");
        else if (ntpEvent == responseError)
            Serial.println ("NTP response error");
    } else {
        if (ntpEvent == timeSyncd) {
            Serial.print ("Got NTP time: ");
            Serial.println (NTP.getTimeDateString (NTP.getLastNTPSync ()));
        }
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  httph.clientHandle();
  processButtons(); 
 
  delay(5);
  i+=5;
    

  if (i>12000) {
    //logg.logging(timeClient.getFormattedTime());
    //logg.logging(String(timeClient.getMinutes()));
    i=0;
    
    NTP.getTime();
    if (syncEventTriggered) {
        processSyncEvent (ntpEvent);
        syncEventTriggered = false;
    }

    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  }
}

