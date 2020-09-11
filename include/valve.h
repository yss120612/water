#ifndef __VALVE_H
#define __VALVE_H

#include <Arduino.h>

class valve
{
private:
    uint8_t VOPEN;
    uint8_t VCLOSE;
public:
void setup();

    valve();
    ~valve();
};


#endif