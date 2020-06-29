

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
  btns.add(D3,LOW);
  btns.add(D8,HIGH);
  rtc.setup();  

    

  //timeClient.begin();
  //NTP.setInterval (63);
  //NTP.setNTPTimeout (NTP_TIMEOUT);
  //NTP.begin (ntpServer, timeZone, false);

  
}

void processButtons(long ms){
event_t ev;
if (btns.getEvent(&ev,ms)){
  switch (ev.state)
  {
  case BTN_CLICK:
    logg.logging("CLICK "+ String(ev.button)+" count="+String(ev.count)+" wait="+String(ms-ev.wait_time)+ " millis="+String(ms));
    if (ev.count==1)  if (true)
    {
      logg.logging(rtc.timestring());
    }else{
      
    }
    if (ev.count==2) {
      logg.logging("Success= "+ String(rtc.isSuccess()));
      //+" time="+rtc.timestring());
      logg.logging(rtc.test());
      // pinMode(D0,OUTPUT);
      // pinMode(D1,OUTPUT);
      // pinMode(D2,OUTPUT);
      // digitalWrite(D0,HIGH);
      // digitalWrite(D1,HIGH);
      // digitalWrite(D2,HIGH);
    }
    break;
  case BTN_LONGCLICK:
    logg.logging("LONGCLICK "+ String(ev.button)+" count="+String(ev.count));
    
    break;
    case BTN_RELEASED:
    //logg.logging("RELEASED "+ String(ev.button));
    break;
    case BTN_DBLCLICK:
    logg.logging("XCLICK "+ String(ev.button));
    break;
    case BTN_PRESSED:
      //logg.logging("PRESSED "+ String(ev.button));
    break;
  }
}
}

void processMQQT(char* topic, byte* payload, unsigned int length) {//callback function
  String res="Message arrived ["+String(topic)+"] :";
  logg.logging("Message arrived ["+String(topic)+"] ");
  for (int i = 0; i < length; i++) {
    res+=(char)payload[i];
  }
  
logg.logging(res);
}



int i=0;
int k=1;

void loop() {
  // put your main code here, to run repeatedly:
  long ms = millis();
  httph.clientHandle();
  processButtons(ms); 
  rtc.loop(ms);
  delay(5);
  i+=5*k;
  if (i>=1020) k=-1;
  else if (i<=5) k=1;
  analogWrite(LED_BUILTIN,i);
  

  if (i>500) {
  //if (k==0)    rtc.setup();
  //k=1;
    //logg.logging(timeClient.getFormattedTime());
    //logg.logging(String(timeClient.getMinutes()));
    //i=0;
    
    //digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  }
}

