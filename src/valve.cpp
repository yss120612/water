#include "valve.h"

valve::valve(uint8_t OP, uint8_t CL)
{
   VOPEN=OP;
   VCLOSE=CL;
}

void valve::setup()
{
    pinMode(VOPEN,OUTPUT);
    pinMode(VCLOSE,OUTPUT);
}

valve::~valve()
{
}