

#include <Arduino.h>

#include "Log.h"
#include "HttpHelper.h"
#include "Rtc.h"
#include "Buttons.h"

HttpHelper httph;
Rtc1302 rtc;
Buttons btns;


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

}

void processButtons(){
event_t ev;
while (btns.getEvent(&ev)){
  switch (ev.state)
  {
  case BTN_CLICK:
    logg.logging("CLICK "+ String(ev.button)+" count="+String(ev.count));
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
int k=0;
event_t ev;
void loop() {
  // put your main code here, to run repeatedly:
  httph.clientHandle();
  processButtons(); 
  delay(5);
  i+=5;
  digitalWrite(LED_BUILTIN,i>1500);
  if (i>12000) 
  {
  i=0;
  k++;
  logg.logging(String(k));
  }
}