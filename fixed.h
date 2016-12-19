//ee445l lab1
/*
* by: Jonathan Friesen (jtf698)
*			Christopher Friesen (cmf2536)
*			Lab Session: T/TH 2-3:30
*/	

#include <stdio.h>
#include <stdint.h>

//ST7735_sDecOut3
//Takes a signed integer between -9999 and 9999
// and converts it to a decimal numer by dividing
// the input by 1000
void ST7735_sDecOut3(signed long x);

//ST7735_uBinOut8
//Takes a signed integer between 0 and 255997
// and returns a fixed point decimal number 
// after dividing the input by 256
void ST7735_uBinOut8(signed long y);

//ST7735_XYplotinit
//clears the screen and draws the plot title, also stores the range
//and max values for screen fitting
void ST7735_XYplotInit(char* Title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY);

//ST7735_XYplot
//will make a scatter plot based on the range determined by ST7735_XYplotinit 
//given the number of points and two arrays of x and y values
void ST7735_XYplot(int32_t num, int32_t* buffX, int32_t* buffY, int32_t type);

