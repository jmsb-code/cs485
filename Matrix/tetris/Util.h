//Define Macros, Includes, and most Global Variables
#ifndef UTIL_H
#define UTIL_H
#define F_CPU 16000000L // tell code we are 16MHz
#define EMPTY_M {0, 0, 0, 0, 0, 0, 0, 0}
#define BUTTON_PRESS 1   
#define BUTTON_RELEASE -1
#define BUTTON_LEFT    but2
#define BUTTON_RIGHT   but4
#define BUTTON_LEFT_R  but1
#define BUTTON_RIGHT_R but3
#define BUTTON_TOLERANCE 55
#define MS_PER_FRAME 5 //used to calculate frame delay rate
#define TICKRATE 500   //defines game tickrate i.e. how often blocks drop a level
#define SCROLLSPEED 750
#define MAX_SCORE_DIGITS 6
#include <avr/delay.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <string.h>
#include "Button.h"
#include "Matrix.h"
#include "Tetris.h"

/*------- Hardware configuration -------*/
const int colPins[8] = {0, 1, 2, 3, 4, 5, 6, 7};
const int rowPins[8] = {18, 17, 16, 15, 14, 12, 11, 10};

/*-------- Char, Digit Bitmasks --------*/
const uint8_t digitMasks[10][8] = {
 {0b00111100, //0
  0b01100110,
  0b01100110,
  0b01100110,
  0b01100110,
  0b01100110,
  0b00111100,
  0b00000000},
 {0b00011000, //1
  0b00011000,
  0b00111000,
  0b00011000,
  0b00011000,
  0b00011000,
  0b01111110,
  0b00000000},
 {0b00111100, //2
  0b01100110,
  0b00000110,
  0b00001100,
  0b00110000,
  0b01100000,
  0b01111110,
  0b00000000},
 {0b00111100, //3
  0b01100110,
  0b00000110,
  0b00011100,
  0b00000110,
  0b01100110,
  0b00111100,
  0b00000000},
 {0b01100110, //4
  0b01100110,
  0b01100110,
  0b01100110,
  0b01111110,
  0b00000110,
  0b00000110,
  0b00000000},
 {0b01111110, //5
  0b01100000,
  0b01111100,
  0b00000110,
  0b00000110,
  0b01100110,
  0b00111100,
  0b00000000},
 {0b00111100, //6
  0b01100110,
  0b01100000,
  0b01111100,
  0b01100110,
  0b01100110,
  0b00111100,
  0b00000000},
 {0b01111110, //7
  0b01100110,
  0b00001100,
  0b00011000,
  0b00011000,
  0b00011000,
  0b00011000,
  0b00000000},
 {0b00111100, //8
  0b01100110,
  0b01100110,
  0b00111100,
  0b01100110,
  0b01100110,
  0b00111100,
  0b00000000},
 {0b00111100, //9
  0b01100110,
  0b01100110,
  0b00111110,
  0b00000110,
  0b01100110,
  0b00111100,
  0b00000000} 
};
const uint8_t GMasks[8] = {0B00111100, 0B01100110, 0B11000000,0B11000000,0B11001110,0B01100110,0B00111110,0B00000000};

uint8_t gameState[8] = EMPTY_M;
uint8_t screenOut[8] = EMPTY_M;

inline void reboot() {
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while (1) {}
}

void (* resetFunc) (void) = 0;


#endif
