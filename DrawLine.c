
#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"


void ST7735_Line(uint16_t x_1, uint16_t y_1, uint16_t x_2, uint16_t y_2, uint16_t color){
	double x1 = x_1;
	double x2 = x_2;
	double y1 = y_1;
	double y2 = y_2;
	double my = (y2-y1);
	double mx = (x2-x1);
	double x_t = x1;
	double y_t = y1;
	int i = 1;
	while(i){
		ST7735_DrawPixel(x_t,y_t,ST7735_YELLOW);
		x_t = x_t+(mx/100);
		y_t = y_t+(my/100);
		if(((uint16_t)x_t == x2)&&((uint16_t)y_t == y2)){
			i = 0;
		}

}
}
