//Inline functions related to driving Matrix
#ifndef MATRIX_H
#define MATRIX_H
//#include <avr/io.h>
//#include "Arduino.h"

extern int colPins[];
extern int rowPins[];

//turn specified row off and clear column pins
inline void clearRow(uint8_t row){
  digitalWrite(rowPins[row], HIGH);
  for(int i = 0; i < 8; i++) digitalWrite(colPins[i], LOW);
}

inline void driveRow(uint8_t row, uint8_t cols, double pTime){
  for(int i = 0; i < 8; i++){
    if(cols & (1 << i)) digitalWrite(colPins[i], HIGH);
  }
  digitalWrite(rowPins[row], LOW);
  delay(pTime);
  clearRow(row);
}

//display firstCols << offset and secondCols >> 8-offset on pin row for delay ms
inline void driveRowOffset(uint8_t row, uint8_t firstCols, uint8_t secondCols, double delay, uint8_t offset){
  uint8_t combinedRow = secondCols >> (8-offset);
  combinedRow |= firstCols << offset;
  driveRow(row, combinedRow, delay);
}

//drive full 8x8 frame encoded by cols rowise for delay_ms each
inline void driveMatrix(uint8_t cols[8], double delay) { for(int i = 0; i < 8; i++) driveRow(i, cols[i], delay); }

//drive firstCols << offset and secondCols >> 8-offset to matrix 
inline void driveMatrixOffset(uint8_t firstCols[8], uint8_t secondCols[8], double delay, uint8_t offset){
  for(int i = 0; i < 8; i++) driveRowOffset(i, firstCols[i], secondCols[i], delay, offset);
}

#endif
