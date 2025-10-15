#ifndef BUTTON_H
#define BUTTON_H
#include "Arduino.h"
extern unsigned long curTime;

class Button{
  public:

  int pin;
  unsigned long buttonPressedTime;
  unsigned long buttonReleasedTime;
  unsigned int tolerance;
  bool pressed;

  //Establish a button on pin p with tolerance t
  Button(int p, unsigned int t);

  //returns 1 on button press, -1 on button release, 0 otherwise
  char detectPress();
  
};

#endif
