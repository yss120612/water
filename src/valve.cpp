#include "valve.h"

Valve::Valve(uint8_t OP, uint8_t CL)
{
   VOPEN=OP;
   VCLOSE=CL;
   
}

void Valve::setup(Rtc1302 * r)
{
    pinMode(VOPEN,OUTPUT);
    pinMode(VCLOSE,OUTPUT);
    in_progress=false;
    action=RELAXED;
    status=CLOSED;
    rtc=r;
    if (rtc->getMemory(MEM_VALVE)>0)
    {
     open();
    }
    else
    {
     close();
    }
}



Valve::~Valve()
{
}

void Valve::open()
{
    action=INOPEN;
    pinMode(VOPEN,HIGH);
    rtc->setMemory(1,MEM_VALVE);
    run();
}

void Valve::close()
{
    action=INCLOSE;
    pinMode(VCLOSE,HIGH);
    rtc->setMemory(0,MEM_VALVE);
    run();
}

void Valve::swc()
{
    if (status!=OPENED) return;
    pinMode(VCLOSE,HIGH);
    action=INSWITCH;
    run();
}

void Valve::run()
{
    in_progress=true;
    curr_time=millis();
}

void Valve::stop()
{
    in_progress=false;
    pinMode(VOPEN,LOW);
    pinMode(VCLOSE,LOW);
    action=RELAXED;
    switch (action)
    {
    case INOPEN:
        status=OPENED;
        break;
    case INCLOSE:
        status=CLOSED;
        break;
    case INSWITCH:
        status=CLOSED;
        open();
        break;
    default:
        break;
    }
}

void Valve::processValves(long m){
    
    if (!in_progress) return;
    if (m-curr_time>ACTION_TIME){
        stop();
    }
}