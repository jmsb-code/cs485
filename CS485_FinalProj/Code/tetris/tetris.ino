#include "Util.h"

/*------- Hardware configuration -------*/
extern int colPins[8];
extern int rowPins[8];

/*-------- Char, Digit Bitmasks --------*/
extern uint8_t digitMasks[10][8];
extern uint8_t GMasks[8];

/*--------        Buttons       --------*/
static Button but1 = Button(34, BUTTON_TOLERANCE);
static Button but2 = Button(35, BUTTON_TOLERANCE);
static Button but3 = Button(36,  BUTTON_TOLERANCE);
static Button but4 = Button(37,  BUTTON_TOLERANCE);

/*--------    Game Variables    --------*/
extern uint8_t gameState[8];
extern uint8_t screenOut[8];
Shape cShape;
int8_t state = 0;
uint8_t offset = 0;
unsigned long curTime = 0;
int score = 0;
unsigned long lastGameTick;
bool Game_Over = false;
uint8_t num_score_digits = 0;
int scoreDigits[MAX_SCORE_DIGITS]; //Supports scores up to 999,999 by default setting

//Interpret score and display with 'good game' message
void gameOver(){
  if(num_score_digits == 0){ //determine number of digits in score on first loop through gameover
    int tempScore = score;
    if(tempScore <= 0) {
      num_score_digits = 1;
      scoreDigits[MAX_SCORE_DIGITS-1] = 0;
    }
    while(tempScore > 0 && num_score_digits < MAX_SCORE_DIGITS) {
      num_score_digits++;
      scoreDigits[MAX_SCORE_DIGITS-num_score_digits] = tempScore % 10;
      tempScore /= 10;
    }
    lastGameTick = curTime;
  }

  BUTTON_LEFT.detectPress();
  BUTTON_RIGHT.detectPress();
  if(BUTTON_LEFT.pressed && BUTTON_RIGHT.pressed) {
    for(int i=0;i<8;i++) gameState[i] = 0;
    cShape = Shape((Shape_Type) ((uint8_t) rand() % 7));
    score = 0;
    num_score_digits = 0;
    Game_Over = false;
    state = 0;
    return;
  }

  if(curTime - lastGameTick >= SCROLLSPEED){ 
    lastGameTick = curTime;
    state = (state + 1) % (num_score_digits+3);
  }

  offset = (curTime - lastGameTick)*8/SCROLLSPEED;

  if(state == 0 || state == 2) driveMatrix(GMasks, MS_PER_FRAME / 8);
  else if(state == 1) driveMatrixOffset(GMasks, GMasks, MS_PER_FRAME / 8, offset);
  else if(state == num_score_digits+2 || num_score_digits == 1) driveMatrix(digitMasks[scoreDigits[(MAX_SCORE_DIGITS-num_score_digits)+(state-3)]], MS_PER_FRAME / 8);
  else driveMatrixOffset(digitMasks[scoreDigits[(MAX_SCORE_DIGITS-num_score_digits)+(state-3)]], digitMasks[scoreDigits[(MAX_SCORE_DIGITS-num_score_digits)+(state-2)]], MS_PER_FRAME / 8, offset);
    
}

void setup() {
  //Serial.begin(9600);
  //set all column pins to OUTPUT, LOW
  for(int col : colPins) {
    pinMode(col, OUTPUT);
    digitalWrite(col, LOW);
  }
  for(int row : rowPins) {
    pinMode(row, OUTPUT);
    digitalWrite(row, HIGH);
  }

  //set all row pins to OUTPUT, HIGH
  for(int row : rowPins) {
    pinMode(row, OUTPUT);
    digitalWrite(row, HIGH);
  }

  //set button pins to input
  pinMode(34, INPUT); //but1
  pinMode(35, INPUT); //but2
  pinMode(36, INPUT);  //but3
  pinMode(37, INPUT);  //but4

  lastGameTick = millis();
  srand(lastGameTick);

  cShape = Shape((Shape_Type) ((uint8_t) rand() % 7));

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  pinMode(12, INPUT);
}


void loop() {
  // save time once per loop for consistent game logic + save processing time
  curTime = millis();
  int speedUp = 1;

  if(Game_Over) {
    gameOver();
    return;
  }

  //Process Button Presses
  if(BUTTON_LEFT.detectPress() == BUTTON_PRESS){
    cShape.moveLeft(gameState);
  }
  if(BUTTON_RIGHT.detectPress() == BUTTON_PRESS){
    cShape.moveRight(gameState);
  }
  if(BUTTON_LEFT_R.detectPress() == BUTTON_PRESS){
    cShape.rotateCCW(gameState);
  }
  if(BUTTON_RIGHT_R.detectPress() == BUTTON_PRESS){
    cShape.rotateCW(gameState);
  }
  else if(BUTTON_RIGHT.pressed && BUTTON_LEFT.pressed){
    speedUp = 2;
  }

  if(curTime - lastGameTick >= TICKRATE/speedUp){
    lastGameTick = curTime;
    int8_t drop = cShape.drop(gameState);
    switch(drop){
      case -1:
        Game_Over = true;
        return;
      case 0:
        score += (int) (clearFullRows(gameState));
        cShape = Shape((Shape_Type) ((uint8_t) rand() % 7));
        if(cShape.drop(gameState) == -1){
          Game_Over = true;
          return;
        }
        break;
    }
  }

  //drive selected digit onto matrix
  printScreen(gameState, screenOut, cShape);
  driveMatrix(screenOut, MS_PER_FRAME / 8);

}