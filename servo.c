#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "SysTickInts.h"
#include <math.h>

volatile uint32_t count = 0;
volatile uint32_t duty = 0;
typedef enum {BACKWARDS,FORWARDS} direction_factor;
typedef enum {GRABBED,UNGRABBED} state;
direction_factor direction = FORWARDS;
state Hand_state = UNGRABBED;
short stopped = 15;
short speed = 0;
uint32_t grabCount = 0;
short on = 0;
//PE4
void servoInit(void){
  SYSCTL_RCGCGPIO_R |= 0x10;  // activate port F
  GPIO_PORTE_DIR_R |= 0x10;   // make PF2 output (PF2 built-in LED)
  GPIO_PORTE_AFSEL_R &= ~0x10;// disable alt funct on PF2
  GPIO_PORTE_DEN_R |= 0x10;   // enable digital I/O on PF2
                              // configure PF2 as GPIO
  GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R&0xFFF0FFFF)+0x00000000;
  GPIO_PORTE_AMSEL_R &= ~0x10;     // disable analog functionality on PF
 // SYSCTL_RCGCGPIO_R |= 0x08;  // activate port F
 // GPIO_PORTE_DIR_R &= ~0x07;   // make PF2 output (PF2 built-in LED)
 // GPIO_PORTE_AFSEL_R &= ~0x07;// disable alt funct on PF2
 // GPIO_PORTE_DEN_R |= 0x07;   // enable digital I/O on PF2
                              // configure PF2 as GPIO
//  GPIO_PORTD_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF000)+0x00000000;
 // GPIO_PORTD_AMSEL_R = 0;     // disable analog functionality on PF	
  SysTick_Init(8000);        // initialize SysTick timer 8000 --> .1ms
	duty = 15;									//set to not move
	grabCount = 0;						// don't grab
	
}

void servoChangeDuty(void){
	if(direction == FORWARDS){
		duty = stopped+speed;
	}else {
		duty = stopped-speed;
	}
}

void servoGo(void){
	servoChangeDuty();
}

void servoSetSpeed(short new_speed){
	speed = fabs(new_speed);
	if(speed>5){
		speed=5;
	}
}

void servoIncrementSpeed(void){
	servoSetSpeed(speed+1);
	//servoChangeDuty();
}

void servoDecrementSpeed(void){
	if(speed == 0){
		servoSetSpeed(speed);
	} else{
		servoSetSpeed(speed-1);
	}
	//servoChangeDuty();
}

void servoSetForwards(void){
	direction = FORWARDS;
	servoSetSpeed(speed);
	//servoChangeDuty();
}

void servoSetBackwards(void){
	direction = BACKWARDS;
	servoSetSpeed(speed);
	//servoChangeDuty();
}

void servoChangeDirection(void){
	if(direction == FORWARDS){
		direction = BACKWARDS;
	}else{
		direction = FORWARDS;
	}
	servoSetSpeed(speed);
}

void servoStop(void){
	uint32_t x = speed;
	servoSetSpeed(0);
	servoChangeDuty();
	servoSetSpeed(x);
}

void servoGrab(void){
	if(grabCount > 0){
		return;//currently grabbing
	}
	if(Hand_state == GRABBED){
		return;
	}
	Hand_state = GRABBED;
	servoSetForwards();
	//servoChangeDirection();
	//servoChangeDuty();
	///edit / calibrate these numbers
	switch (speed){
		case 0 :
			grabCount = 0;
			break;
		case 1 :
			grabCount = 30000;
		break;
		case 2 :
			grabCount = 21000;
		break;
		case 3 :
			grabCount = 15000;
		break;
		case 4 :
			grabCount = 10000;
		break;
		case 5 :
			grabCount = 12000;
		break;
	}
}

void servoUngrab(void){
	if(grabCount > 0){
		return;//currently grabbing
	}
	if(Hand_state == UNGRABBED){
		return;
	}
	Hand_state = UNGRABBED;
	servoSetBackwards();
	//servoChangeDuty();
	switch (speed){
		case 0 :
			grabCount = 0;
			break;
		case 1 :
			grabCount = 30000;
		break;
		case 2 :
			grabCount = 15000;
		break;
		case 3 :
			grabCount = 15000;
		break;
		case 4 :
			grabCount = 10000;
		break;
		case 5 :
			grabCount = 10000;
		break;
	}
}

void servoForward(void){
	servoSetForwards();
	grabCount = 100;
}

void servoBackward(void){
	servoSetBackwards();
	grabCount = 100;
}

void SysTick_Handler(void){
	count++;
	if(count<duty){
		GPIO_PORTE_DATA_R = (GPIO_PORTE_DATA_R | 0x10);
	}else if (count<200){
		GPIO_PORTE_DATA_R = (GPIO_PORTF_DATA_R & ~0x10);
	}else if (count>=200){
		count=0;
	}
	
	if(grabCount > 0){
		if(grabCount == 1){
			servoStop();
		}else{
			servoSetSpeed(speed);
			servoChangeDuty();
		}
		grabCount--;
	} 
}