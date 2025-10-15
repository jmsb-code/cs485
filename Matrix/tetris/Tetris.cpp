#include "Tetris.h"

//Create a new Shape just above the gameState in the center
Shape::Shape(Shape_Type type){
  switch(type){
    case SHAPE_O:
      ht = 2;
      wd = 2;
      shapeM[0] = 0b11;
      shapeM[1] = 0b11;
      break;
    case SHAPE_I:
      ht = 4;
      wd = 1;
      shapeM[0] = 0b1;
      shapeM[1] = 0b1;
      shapeM[2] = 0b1;
      shapeM[3] = 0b1;
      break;
    case SHAPE_S:
      ht = 2;
      wd = 3;
      shapeM[0] = 0b011;
      shapeM[1] = 0b110;
      break;
    case SHAPE_Z:
      ht = 2;
      wd = 3;
      shapeM[0] = 0b110;
      shapeM[1] = 0b011;
      break;
    case SHAPE_L:
      ht = 3;
      wd = 2;
      shapeM[0] = 0b10;
      shapeM[1] = 0b10;
      shapeM[2] = 0b11;
      break;
    case SHAPE_J:
      ht = 3;
      wd = 2;
      shapeM[0] = 0b01;
      shapeM[1] = 0b01;
      shapeM[2] = 0b11;
      break;
    case SHAPE_T:
      ht = 2;
      wd = 3;
      shapeM[0] = 0b111;
      shapeM[1] = 0b010;
      break;
  }

  rotState = 0;
  x = 5-(wd/2);
  y = 0-ht;
}

uint8_t Shape::getHeight(){
  if(rotState % 2 == 0) return ht;
  else return wd;
}

uint8_t Shape::getWidth(){
  if(rotState % 2 == 0) return wd;
  else return ht;
}

//Returns true if (xPos, yPos) within the frame of the shape is a filled cell
inline bool Shape::isSet(int8_t xPos, int8_t yPos){
  if(rotState == 0)     return shapeM[yPos] & (1 << xPos);                            
  if(rotState == 1)     return shapeM[(ht-1)-xPos] & (1 << yPos);              
  if(rotState == 2)     return shapeM[(ht-1)-yPos] & (1 << ((wd-1) - xPos));            
  if(rotState == 3)     return shapeM[xPos] & (1 << ((wd-1) - yPos));             
}

//Returns true if (xPos, yPos) within the frame of this shape with a hypothetical rotation state is a filled cell
inline bool Shape::isSet(int8_t xPos, int8_t yPos, uint8_t rot){
  if(rot == 0)     return shapeM[yPos] & (1 << xPos);                             
  if(rot == 1)     return shapeM[(ht-1)-xPos] & (1 << yPos);                   
  if(rot == 2)     return shapeM[(ht-1)-yPos] & (1 << ((wd-1) - xPos));  
  if(rot == 3)     return shapeM[xPos] & (1 << ((wd-1) - yPos));        
}


void Shape::rotateCW(uint8_t gameState[8]){
  int8_t newX = x;
  int8_t newY = y;
  uint8_t newRot = (rotState + 1) % 4;

  int8_t offsets[] = {0, -1, 1, -2, 2};

  for(int8_t offset : offsets) {
    bool goodOffset = true;
    if(getHeight() < getWidth()) {
      newX -= (getHeight() - getWidth())/2 + offset;
      newY += (getHeight() - getWidth())/2;
    }
    else {
      newX += (getWidth() - getHeight())/2 + offset;
      newY -= (getHeight() - getWidth())/2;
    }

    for(int i = 0; i < getHeight(); i++){
      if(newX + i < 0 || newX + i > 7) return;
      for(int j = 0; j < getWidth(); j++){
        if(y + j > 7) return;
        if(!isSet(i, j, newRot)) continue;

        //check if gamespace is full where this block will be brought
        if(gameState[y+j] & ( 1 << (7-(newX+i))) ) {
          goodOffset = false;
          break;
        }
      }
    }
    if(goodOffset) break;
  }
  x = newX;
  y = newY;
  rotState = newRot;
}

void Shape::rotateCCW(uint8_t gameState[8]){
  int8_t newX = x;
  int8_t newRot = (rotState - 1) % 4;
  if (newRot < 0) newRot = 3;
  if(getHeight() < getWidth()) newX -= (getHeight() - getWidth())/2;
  else newX += (getWidth() - getHeight())/2;

  for(int i = 0; i < getHeight(); i++){
    if(newX + i < 0 || newX + i > 7) return;
    for(int j = 0; j < getWidth(); j++){
      if(y + j > 7) return;
      if(!isSet(i, j, newRot)) continue;
      //check if gamespace is full to the left of all set left-facing blocks of this Shape
      if(gameState[y+j] & ( 1 << (7-(newX+i)))) return;
    }
  }
  x = newX;
  rotState = newRot;
}

//Attempt to move shape left, fail if it results in overlap of filled area or escapes gamespace
void Shape::moveLeft(uint8_t gameState[8]){
  if(x - 1 < 0) return;
  
  for(int i = 0; i < getWidth(); i++){
    for(int j = 0; j < getHeight(); j++){
      if(!isSet(i, j)) continue;
      int8_t newX = x + i - 1;
      //fail if gamespace is full to the left of any set left-facing blocks of this Shape
      if(newX > 7 || (gameState[y+j] & ( 1 << (7-(newX)) )) ) return;
    }
  }

  x--;
}

//Attempt to move shape right, fail if it results in overlap of filled area or escapes gamespace
void Shape::moveRight(uint8_t gameState[8]){
  if(x + getWidth() > 7) return;
  
  for(int i = 0; i < getWidth(); i++){
    for(int j = 0; j < getHeight(); j++){
      if(!isSet(i, j)) continue;
      int8_t newX = x + i + 1;
      //fail if gamespace is full to the right of any set right-facing blocks of this Shape
      if(newX < 0 || (gameState[y+j] & ( 1 << (7-(newX)) )) ) return;
    }
  }

  x++;
}

//Drop shape down one row. Returns 1 on successful drop, 0 on successful absorption, -1 on game over
int8_t Shape::drop(uint8_t gameState[8]){
  for(int j = 0; j < getHeight(); j++){
    for(int i = 0; i < getWidth(); i++){
      if(!isSet(i, j)) continue; 
      int8_t newY = y + j + 1;
      if(newY < 0) continue;
      //check if gamespace is full beneath all bottom-facing blocks of this Shape
      if(newY > 7 || gameState[newY] & (1 << (7-(x+i))) ){
        return absorbBlock(gameState, *this);
      } 
    }
  }
  //no collisions found
  y++;
  return 1;
}

//Writes a matrix that combines gameState with Shape to screen for output to LED matrix
void printScreen(uint8_t gameState[8], uint8_t screen[8], Shape as){
  for(int i = 0; i < 8; i++) screen[i] = gameState[i];

  for(int i = 0; i < as.getWidth(); i++){
    for(int j = 0; j < as.getHeight(); j++){
      if(as.y + j < 0) continue; //ignore levels too high up to matter
      if(as.isSet(i, j)) screen[j+as.y] |= (1 << (7 - (i + as.x))); 
    }
  }
}

//Absorb shape into gameState, returns 0 on success, -1 on gameover
int8_t absorbBlock(uint8_t gameState[8], Shape as){
  for(int i = 0; i < as.getWidth(); i++){
    for(int j = 0; j < as.getHeight(); j++){
      if(!as.isSet(i, j)) continue; 
      if(as.y + j < 0) return -1; //attempted to build over top row, game over
      gameState[j+as.y] |= (1 << (7-(i + as.x)) ); 
      
    }
  }

  return 0;
}

//Removes each completed row from gameState and shifts rows above it down, returns number of levels cleared in this way
int clearFullRows(uint8_t gameState[8]){
  int cRows = 0;
  for(int i = 0; i < 8; i++){
    if(gameState[i] == 0b11111111) { //full row detected
      for(int j = i; j > 0; j--) gameState[j] = gameState[j-1];
      gameState[0] = 0;
      cRows++;
    }
  }
  return cRows;
}