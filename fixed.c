//ee445l lab1
/*
* by: Jonathan Friesen (jtf698)
*			Christopher Friesen (cmf2536)
*			Lab Session: T/TH 2-3:30
*/	

#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"
#include "DrawLine.h"
static int32_t rangeY;
static int32_t rangeX;
static int32_t GmaxY;
static int32_t GmaxX;
static int32_t GminY;
static int32_t GminX;


/*ST7735_sDecOut3
*Takes a signed integer between -9999 and 9999
* and converts it to a decimal numer by dividing
* the input by 1000
*/

void ST7735_sDecOut3(signed long input){
	if (input>9999 | input<-9999){
		ST7735_OutString(" *.***");
	}else{
		unsigned long number = input;
		if(input<0){
				ST7735_OutChar('-');
				number = input*(-1);
		}else{
			ST7735_OutChar(' ');
		}
		ST7735_OutUDec((number/1000)%10);
		ST7735_OutChar('.');
		ST7735_OutUDec((number/100)%10);
		ST7735_OutUDec((number/10)%10);
		ST7735_OutUDec(number%10);
	}
}


/* ST7735_uBinOut8
*	 Takes a signed integer between 0 and 255997
*  and returns a fixed point decimal number 
*  after dividing the input by 256
*/
void ST7735_uBinOut8(int32_t input){
	if((input >= 256000)|(input < 0)){
		ST7735_OutString("***.**");
	}else{
		int32_t number = input/256;
		if(number>99){
			ST7735_OutUDec(number);
		}else if (number>9){
			ST7735_OutChar(' ');
			ST7735_OutUDec(number);
		}else{
			ST7735_OutChar(' ');
			ST7735_OutChar(' ');
			ST7735_OutUDec(number);
		}
		ST7735_OutChar('.');
		ST7735_OutUDec(((input*10)/256)%10);
		ST7735_OutUDec(((input*100)/256)%10);
//		int32_t check = (input*10)/256;
//		if((check%100)==0){
//			ST7735_OutUDec(0);
//		}
//		if((((1000*(input))/256)%10) >= 5){//round up
//			check=check+1;
//		}
//		ST7735_OutUDec(check%100);
	}
}

void ST7735_XYplotInit(char* Title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY){ 
  Output_Clear();         
	// initialize output device
	//store the range values for plotting
	rangeY = maxY - minY;
	rangeX = maxX - minX;
	//set max values
	GmaxY = maxY;
	GmaxX = maxX;
	GminY = minY;
	GminX = minX;
	//check for error
	if(rangeY < 0){
		rangeY = rangeY*-1;
		GmaxY = minY;
		GminY = maxY;
	}
	if(rangeX < 0){
		rangeX = rangeX*-1;
		GmaxX = minX;
		GmaxX = maxX;
	}
	//draw title
	ST7735_DrawString(8, 1, Title, ST7735_BLUE);
}

void ST7735_XYplot(int32_t num, int32_t* buffX, int32_t* buffY, int32_t type){
	int32_t x;
	int32_t y;
	int32_t saved_y = y;
	for(int32_t i = 0; i < num; i++){
	//x goes from 0 to 127
	//y goes from 0 to 159
	//use ratio of the range to fit to the screen
		y = 15+ (130*(GmaxY - buffY[i]))/rangeY;
		if(y<0){
			y = 0;
		}
		if(y>159){
			y = 130;
		}
		x = (127*(buffX[i] - GminX))/rangeX;
		if(x<0){
			x = 0;
		}
		if(x>127){
			x = 127;
		}
		//draw pixel after determing its position relative to the range and screen
		if(type == 0){
		ST7735_DrawPixel(x, y, ST7735_YELLOW);
		ST7735_DrawPixel(x+1, y, ST7735_YELLOW);
		ST7735_DrawPixel(x, y+1, ST7735_YELLOW);
		ST7735_DrawPixel(x-1, y, ST7735_YELLOW);
		ST7735_DrawPixel(x, y-1, ST7735_YELLOW);		
		}
		if(type == 1){
			ST7735_DrawFastVLine(x,y,159, ST7735_YELLOW);
		//	ST7735_Line(x,y,x,120,ST7735_YELLOW);
			
		}
		
		
	}
	if(type == 1){
		ST7735_FillRect(0, 145, 130, 15, ST7735_BLACK);
		ST7735_DrawFastHLine(0,145,200,ST7735_BLUE);
		// Move the cursor to the desired X- and Y-position.  The
// next character will be printed here.  X=0 is the leftmost
// column.  Y=0 is the top row.
// inputs: newX  new X-position of the cursor (0<=newX<=20)
//         newY  new Y-position of the cursor (0<=newY<=15)
		ST7735_SetCursor(0,15);
		ST7735_OutUDec(GminX);
		ST7735_SetCursor(16,15);
		ST7735_OutUDec(GmaxX);
		ST7735_SetCursor(9,15);
		ST7735_OutUDec((GmaxX+GminX)/2);
		
		
		
		
	}
}	
