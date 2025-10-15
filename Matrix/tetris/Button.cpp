#include "Button.h"

Button::Button(int p, unsigned int t){
  this->pin = p;
  this->buttonPressedTime = 0;
  this->buttonReleasedTime = 0;
  this->tolerance = t;
  this->pressed = false;
}

char Button::detectPress(){
  int buttonState = digitalRead(pin);
  if(buttonState == HIGH && !pressed && curTime - buttonReleasedTime > tolerance){
    buttonPressedTime = curTime;
    pressed = true;
    return 1;
  }else if(buttonState == LOW && pressed && curTime - buttonPressedTime > tolerance){
    buttonReleasedTime = curTime;
    pressed = false;
    return -1;
  }

  return 0;
}