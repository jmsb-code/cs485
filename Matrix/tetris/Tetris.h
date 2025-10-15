//Contains the Shape class and various functions that interface with a provided gameState
#ifndef TETRIS_H
#define TETRIS_H

#include "Arduino.h"
#include <avr/io.h>

extern unsigned long curTime;

enum Shape_Type : uint8_t {SHAPE_O, SHAPE_I, SHAPE_S, SHAPE_Z, SHAPE_L, SHAPE_J, SHAPE_T};

class Shape{
  public:
    int8_t x;          //x coordinate of top left corner of shape frame
    int8_t y;          //y coordinate of top left corner of shape frame
  private:
    uint8_t ht;        //height of unrotated shape
    uint8_t wd;        //width of unrotated shape

    uint8_t rotState;  //rotation state: value [0, 3] = number of clockwise rotations from default
    uint8_t shapeM[4]; //encoding of the shape (undefined outside bounds ht and wd)

  public:
    //Create an empty, uninitialized Shape
    Shape(){}
    //Create a new Shape just above the gameState in the center
    Shape(Shape_Type type);

    uint8_t getHeight();
    uint8_t getWidth();

    //Attempt to move shape left, fail if it results in overlap of filled area or escapes gamespace
    void moveLeft(uint8_t gameState[8]);

    //Attempt to move shape right, fail if it results in overlap of filled area or escapes gamespace
    void moveRight(uint8_t gameState[8]);

    //Returns true if (xPos, yPos) within the frame of this shape is a filled cell
    inline bool isSet(int8_t xPos, int8_t yPos);

    //Returns true if (xPos, yPos) within the frame of this shape with a hypothetical rotation state is a filled cell
    inline bool isSet(int8_t xPos, int8_t yPos, uint8_t rot);

    //perform a clockwise rotation on this shape
    void rotateCW(uint8_t gameState[8]);

    //perform a counter-clockwise rotation on this shape
    void rotateCCW(uint8_t gameState[8]);

    //Drop shape down one row. Returns 1 on successful drop, 0 on successful absorption, -1 on game over
    int8_t drop(uint8_t gameState[8]);

};

//Writes a matrix that combines gameState with Shape to screen for output to LED matrix
void printScreen(uint8_t gameState[8], uint8_t screen[8], Shape as);

//Absorb shape into gameState, returns true on success, false on gameover
int8_t absorbBlock(uint8_t gameState[8], Shape as);

//Removes each completed row from gameState and shifts rows above it down, returns number of levels cleared in this way
uint8_t clearFullRows(uint8_t gameState[8]);

#endif
