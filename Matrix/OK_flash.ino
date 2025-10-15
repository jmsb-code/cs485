#define F_CPU 16000000L // tell code we are 16MHz
#include <avr/delay.h>
//void _delay_ms(double msec)
//void _delay_us(double usec)

/*------- Hardware configuration -------*/
const int colPins[] = {0, 1, 2, 3, 4, 5, 6, 7};
const int rowPins[] = {18, 17, 16, 15, 14, 12, 11, 10};

const uint8_t OMasks[] = {0x7E, 0xE7, 0xC3, 0xC3, 0xC3, 0xC3, 0xE7, 0x7E};
const uint8_t KMasks[] = {0x63, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0x63, 0x61};

unsigned long startTime = 0;
uint8_t state = 0;
inline void clearRow(uint8_t row){
  digitalWrite(rowPins[row], HIGH);
  PORTD = 0;
}

inline void driveRow(uint8_t row, uint8_t cols, double delay){
  PORTD = cols;
  digitalWrite(rowPins[row], LOW);
  _delay_ms(delay);
  clearRow(row);
}

inline void driveRowOffset(uint8_t row, uint8_t firstCols, uint8_t secondCols, double delay, uint8_t offset){
  uint8_t combinedRow = secondCols >> (8-offset);
  combinedRow |= firstCols << offset;
  driveRow(row, combinedRow, delay);
}

inline void driveMatrix(uint8_t cols[8], double delay){
  for(int i = 0; i < 8; i++) driveRow(i, cols[i], delay);
}

void setup() {
  // put your setup code here, to run once:
  for(const int col : colPins) {
    pinMode(col, OUTPUT);
    digitalWrite(col, LOW);
  }
  for(const int row : rowPins) {
    pinMode(row, OUTPUT);
    digitalWrite(row, HIGH);
  }

  startTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(millis() - startTime >= 1000){
    state = (state + 1) % 4;
    startTime = millis();
  }
  switch(state){
    case 0: 
      for(int i = 0; i < 8; i++) driveRow(i, OMasks[i], 0.25);
      break;
    case 1: 
      for(int i = 0; i < 8; i++) driveRow(i, KMasks[i], 0.25);
      break;
    case 2: 
      for(int i = 0; i < 8; i++) driveRow(i, ~OMasks[i], 0.25);
      break;
    case 3: 
      for(int i = 0; i < 8; i++) driveRow(i, ~KMasks[i], 0.25);
      break;
  }

}