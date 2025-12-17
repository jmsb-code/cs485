#define F_CPU 16000000L // tell code we are 16MHz
//#include <avr/delay.h>
//void _delay_ms(double msec)
//void _delay_us(double usec)

/*------- Hardware configuration -------*/
const int colPins[] = {23, 22, 21, 19, 18, 5, 17, 15};
const int rowPins[] = {14, 27, 26, 25, 33, 32, 4, 16};

const uint8_t OMasks[] = {0x7E, 0xE7, 0xC3, 0xC3, 0xC3, 0xC3, 0xE7, 0x7E};
const uint8_t KMasks[] = {0x63, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0x63, 0x61};

bool RANDOM_DIGITS = false;


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

int curDigit = 0;
int lastDigit = 0;

unsigned long startTime = 0;
uint8_t state = 0;
uint8_t offset = 0;

class Ticker {
  public:

  int ticks;
  unsigned long lastTick;
  unsigned long curTime;
  unsigned long ms;
  bool ticked;

  Ticker(){
    ticks = 0;
    ms = 1000;
    ticked = false;
    lastTick = millis();
    curTime = lastTick;
  }


    bool tick(){
      curTime = millis();

      if(curTime - lastTick >= ms) {
        ticks++;
        ticked = true;
        lastTick = curTime;
        return true;
        }
      ticked = false;
      return false;
    }
};

static Ticker secTicker = Ticker();

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

inline void driveRowOffset(const uint8_t row, const uint8_t firstCols, const uint8_t secondCols, double pTime, uint8_t offset){
  uint8_t combinedRow = secondCols >> (8-offset);
  combinedRow |= firstCols << offset;
  driveRow(row, combinedRow, pTime);
}

inline void driveMatrix(const uint8_t cols[8], double pTime){
  for(int i = 0; i < 8; i++) driveRow(i, cols[i], pTime);
}

inline void driveMatrixOffset(const uint8_t firstCols[8], const uint8_t secondCols[8], double pTime, uint8_t offset){
  for(int i = 0; i < 8; i++) driveRowOffset(i, firstCols[i], secondCols[i], pTime, offset);
}

void setup() {
  delay(500);
  // put your setup code here, to run once:
  for(const int col : colPins) {
    pinMode(col, OUTPUT);
    digitalWrite(col, LOW);
  }
  for(const int row : rowPins) {
    pinMode(row, OUTPUT);
    digitalWrite(row, HIGH);
  }

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  pinMode(12, INPUT);

}


void loop() {
  // put your main code here, to run repeatedly:

  if(digitalRead(12) == HIGH){
    if (secTicker.tick()){
      state = (state + 1) % 2;
      offset = 0;
      if(state == 0){
        lastDigit = curDigit;
        curDigit = (int) rand() % 10;
      }
    }else offset = (secTicker.curTime - secTicker.lastTick)*8/secTicker.ms;



    switch(state){
      case 0: 
        driveMatrix(digitMasks[lastDigit], 0.25);
        break;
      case 1: 
        driveMatrixOffset(digitMasks[lastDigit], digitMasks[curDigit], 0.25, offset);
        break;
    }
  }
  else{
    if (secTicker.tick()){
      state = (state + 1) % 8;
      offset = 0;
    }else offset = (secTicker.curTime - secTicker.lastTick)*8/secTicker.ms;

    

    switch(state){
      case 0: 
        for(int i = 0; i < 8; i++) driveRow(i, OMasks[i], 0.25);
        break;
      case 1: 
        for(int i = 0; i < 8; i++) driveRowOffset(i, OMasks[i], KMasks[i], 0.25, offset);
        break;
      case 2: 
        for(int i = 0; i < 8; i++) driveRow(i, KMasks[i], 0.25);
        break;
      case 3: 
        for(int i = 0; i < 8; i++) driveRowOffset(i, KMasks[i], ~OMasks[i], 0.25, offset);
        break;
      case 4: 
        for(int i = 0; i < 8; i++) driveRow(i, ~OMasks[i], 0.25);
        break;
      case 5: 
        for(int i = 0; i < 8; i++) driveRowOffset(i, ~OMasks[i], ~KMasks[i], 0.25, offset);
        break;
      case 6: 
        for(int i = 0; i < 8; i++) driveRow(i, ~KMasks[i], 0.25);
        break;
      case 7: 
        for(int i = 0; i < 8; i++) driveRowOffset(i, ~KMasks[i], OMasks[i], 0.25, offset);
        break;
    }
  }

  

}
