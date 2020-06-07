#ifndef INTR_EXCLUSIIVE
#include <functional>
#include <FunctionalInterrupt.h>
#endif
#include "buttons.h"
#include "Log.h"

uint8_t Buttons::add(uint8_t pin, bool level) {
  uint8_t result;
  button_t b;

  if (pin > 15)
    return 0xFF;
  b.pin = pin;
  b.level = level;
  b.paused = false;
  b.pressed = false;
  //b.dblclickable = false;
  b.duration = 0;
  b.xdbl=0;
  result = _btns.length();
  _btns.push_back(b);
  
  if (result < _btns.length()) {
    pinMode(pin, level ? INPUT : INPUT_PULLUP);
#ifdef INTR_EXCLUSIIVE
    ETS_GPIO_INTR_DISABLE();
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(pin));
    gpio_pin_intr_state_set(GPIO_ID_PIN(pin), GPIO_PIN_INTR_ANYEDGE);
    ETS_GPIO_INTR_ENABLE();
#else
    attachInterrupt(pin, [this]() { this->_isr(this); }, CHANGE);
#endif
  result = _btns.length()-1;
  }
  else{
  result = 0xFF;
  }

  return result;
}

void Buttons::pause(uint8_t index) {
  if (_btns.length() > index) {
    _btns[index].paused = true;
#ifdef INTR_EXCLUSIIVE
    ETS_GPIO_INTR_DISABLE();
    gpio_pin_intr_state_set(GPIO_ID_PIN(_btns[index].pin), GPIO_PIN_INTR_DISABLE);
    ETS_GPIO_INTR_ENABLE();
#else
    detachInterrupt(_btns[index].pin);
#endif
  }
}

void Buttons::resume(uint8_t index) {
  if (_btns.length() > index) {
    _btns[index].paused = false;
    _btns[index].pressed = false;
   //_btns[index].dblclickable = false;
    _btns[index].duration = 0;
#ifdef INTR_EXCLUSIIVE
    ETS_GPIO_INTR_DISABLE();
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(_btns[index].pin));
    gpio_pin_intr_state_set(GPIO_ID_PIN(_btns[index].pin), GPIO_PIN_INTR_ANYEDGE);
    ETS_GPIO_INTR_ENABLE();
#else
    attachInterrupt(_btns[index].pin, [this]() { this->_isr(this); }, CHANGE);
#endif
  }
}

void ICACHE_RAM_ATTR Buttons::_isr(Buttons *_this) {
#ifdef INTR_EXCLUSIIVE
  uint32_t status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
#endif

  if (_this->_btns.length()>0) {
    uint32_t time = millis() - _this->_isrtime;
    uint32_t inputs = GPI;
    for (uint8_t i = 0; i < _this->_btns.length(); ++i) {
      if (_this->_btns[i].paused)
        continue;
      //if (_this->_btns[i].duration) {
        

        if (time + _this->_btns[i].duration < 0xFFFF)
          _this->_btns[i].duration = time;
        else
          _this->_btns[i].duration = 0xFFFF;
        //}
      logg.logging("State="+String((inputs >> _this->_btns[i].pin) & 0x01));

      if (((inputs >> _this->_btns[i].pin) & 0x01) == _this->_btns[i].level) { // Button pressed
        if (! _this->_btns[i].pressed) {

          if (time + _this->_btns[i].duration < 0xFFFF)
          _this->_btns[i].duration += time;
        else
          _this->_btns[i].duration = 0xFFFF;

          //_this->_btns[i].dblclickable = (_this->_btns[i].duration > 0) && (_this->_btns[i].duration <= DBLCLICK_TIME);
          //_this->_btns[i].dblclickable = (_this->_btns[i].duration > 0) && (_this->_btns[i].duration <= DBLCLICK_TIME);
          if (_this->_btns[i].duration > DBLCLICK_TIME) 
          {
           _this->_btns[i].xdbl=0;
           //_this->_btns[i].duration=0;
          }
          _this->_btns[i].pressed = true;
          //_this->_btns[i].duration = 1;
          //_this->_btns[i].xdbl += 1;
          _this->onChange(BTN_PRESSED, i);
        }
      } else { // Button released
        if (_this->_btns[i].pressed) { // Was pressed
        logg.logging("dur="+String(_this->_btns[i].duration));
          if (_this->_btns[i].duration >= LONGCLICK_TIME) {
            _this->_btns[i].xdbl;
            _this->onChange(BTN_LONGCLICK, i,_this->_btns[i].xdbl);
            _this->_btns[i].xdbl = 0;
          } else if (_this->_btns[i].duration >= CLICK_TIME) {
            //if (_this->_btns[i].dblclickable)
            //  _this->onChange(BTN_DBLCLICK, i);
            //else
             _this->_btns[i].xdbl += 1;
             _this->onChange(BTN_CLICK, i,_this->_btns[i].xdbl);
          } else {
            //logg.logging("RELEASED STATE");
            _this->onChange(BTN_RELEASED, i);
             
          }
          _this->_btns[i].pressed = false;
          _this->_btns[i].duration = 0;
          //if ((_this->_btns[i].duration >= CLICK_TIME) && (_this->_btns[i].duration < LONGCLICK_TIME))
            //_this->_btns[i].duration = 1;
            //_this->_btns[i].xclc = 1;
          //else
            //_this->_btns[i].duration = 0;
        }
      }
    }
  }
#ifdef INTR_EXCLUSIIVE
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, status);
#endif
  _this->_isrtime = millis();
}

void Buttons::onChange(buttonstate_t state, uint8_t button, uint8_t cnt){
event_t evt;
evt.button=button;
evt.state=state;
evt.count=cnt;
_events.push_back(evt);
}

bool Buttons::getEvent(event_t * e){
  
  if (have_event())
  {
    *e  = _events.front();
    _events.pop_front();
    return   true;
  }
  
  return   false;
}

void Buttons::cleanup(void *ptr) {
#ifdef INTR_EXCLUSIIVE
  ETS_GPIO_INTR_DISABLE();
  gpio_pin_intr_state_set(GPIO_ID_PIN(((button_t*)ptr)->pin), GPIO_PIN_INTR_DISABLE);
  ETS_GPIO_INTR_ENABLE();
#else
  detachInterrupt(((button_t*)ptr)->pin);
#endif
}

bool Buttons::match(uint8_t index, const void *t) {
  if (index < _btns.length()) {
    return (_btns[index].pin == ((button_t*)t)->pin);
  }

  return false;
}
