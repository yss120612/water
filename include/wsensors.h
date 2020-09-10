#ifndef __WSENSORS_H
#define __WSENSORS_H

#include <Arduino.h>
#include <QList.h>
#include "Rtc.h"

struct alarm_t {
    uint8_t sensorN;
    uint8_t count;
    volatile long wait_time;
  };

struct sensor_t{
    String name;
    uint8_t pin;
    uint8_t alarmscount;
};




class Wsensors
{
private:
    
    const uint8_t max_alarms=3;
    const int check_time=3000; //every 3 sec

    QList<alarm_t> _alrms;
    QList<sensor_t> _snsrs;
    unsigned long last_check;
    void alarm_event(uint8_t n);
    void disalarm_event(uint8_t n);
    uint8_t ALARM;
    Rtc1302 * rtc;
public:
    void processSensors(long ms);
    void setup(Rtc1302 * r);
    void addSensor(uint8_t pin,String name); 
    bool isALARM(){return ALARM>0;}
    Wsensors(/* args */);
    void alarm();
    void disalarm();
    ~Wsensors();
};



#endif