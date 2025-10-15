#include "Util.h"

/*------- Hardware configuration -------*/
extern const int colPins[8];
extern const int rowPins[8];

/*-------- Char, Digit Bitmasks --------*/
extern const uint8_t digitMasks[10][8];
extern const uint8_t OMasks[8];
extern const uint8_t KMasks[8];

/*--------        Buttons       --------*/
static Button but2 = Button(19, BUTTON_TOLERANCE);
static Button but3 = Button(9,  BUTTON_TOLERANCE);
static Button but1 = Button(13, BUTTON_TOLERANCE);
static Button but4 = Button(8,  BUTTON_TOLERANCE);

/*--------    Game Variables    --------*/
static int8_t state = 0;
static unsigned long curTime = 0;

void setup() {
  //set all column pins to OUTPUT, LOW
  DDRD = 0b11111111;
  PORTD = 0b00000000;

  //set all row pins to OUTPUT, HIGH
  for(const int row : rowPins) {
    pinMode(row, OUTPUT);
    digitalWrite(row, HIGH);
  }

  //set button pins to input
  pinMode(13, INPUT); //but1
  pinMode(19, INPUT); //but2
  pinMode(9, INPUT);  //but3
  pinMode(8, INPUT);  //but4
}


void loop() {
  // save time once per loop for consistent game logic + save processing time
  curTime = millis();

  //Process Button Presses
  if(BUTTON_LEFT.detectPress() == BUTTON_RELEASE){
    state = (state - 1) % 10;
    if(state < 0) state = 9;
  }
  if(BUTTON_RIGHT.detectPress() == BUTTON_PRESS){
    state = (state + 1) % 10;
  }
  if(BUTTON_LEFT_R.detectPress() == BUTTON_PRESS){
    state = 0;
  }
  if(BUTTON_RIGHT_R.detectPress() == BUTTON_PRESS){
    state = 9;
  }

  //drive selected digit onto matrix
  driveMatrix(digitMasks[state], MS_PER_FRAME / 8);

}