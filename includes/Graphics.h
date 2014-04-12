/*====================================================================
File:    Graphics.h
Author:  Chuck Bolin, May 2006
Purpose: Defines windows based function for use with console
         programming.
====================================================================*/
#include <windows.h>

//constants for color values
//darker colors - low intensity
const int BLACK = 0;
const int BLUE = 1;
const int GREEN = 2;
const int CYAN = 3;
const int RED = 4;
const int MAGENTA = 5;
const int YELLOW = 6;
const int WHITE = 7;
const int GRAY = 8;

//brighter colors - high intensity
const int HI_BLUE = 9;
const int HI_GREEN = 10;
const int HI_CYAN = 11;
const int HI_RED = 12;
const int HI_MAGENTA = 13;
const int HI_YELLOW = 14;
const int HI_WHITE = 15;

//console graphics functions
void setColor(int foreColor, int backColor);
void posCursor( int x, int y);
void fullScreen(void);