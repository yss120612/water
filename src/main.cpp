#include <Arduino.h>
#include "Log.h"
#include "HttpHelper.h"

HttpHelper httph;

void setup() {
  // put your setup code here, to run once:
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
}

int i=0;

void loop() {
  // put your main code here, to run repeatedly:
  httph.clientHandle();
  delay(5);
  i+=5;
  digitalWrite(LED_BUILTIN,i>1500);
  if (i>3000) i=0;
  //logg.logging("AAA");
}