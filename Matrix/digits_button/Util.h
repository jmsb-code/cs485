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
#include <avr/delay.h>
#include <avr/io.h>
#include <string.h>
#include "Button.h"
#include "Matrix.h"

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
const uint8_t OMasks[8] = {0x7E, 0xE7, 0xC3, 0xC3, 0xC3, 0xC3, 0xE7, 0x7E};
const uint8_t KMasks[8] = {0x63, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0x63, 0x61};


#endif
