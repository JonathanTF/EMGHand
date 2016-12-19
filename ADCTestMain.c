// ADCTestMain.c
// Runs on TM4C123
// This program periodically samples ADC channel 0 and stores the
// result to a global variable that can be accessed with the JTAG
// debugger and viewed with the variable watch feature.
// Daniel Valvano
// September 5, 2015

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
 
 
 //EMG HAND PROJECT
 //Chris and Jon Friesen
 //
 //This main file is adapted from Dr. Valvano's code (above)
 //from Lab 2 of 445L, Embedded Systems Design, Fall 2016
 //
 //
 
// center of X-ohm potentiometer connected to PE3/AIN0
// bottom of X-ohm potentiometer connected to ground
// top of X-ohm potentiometer connected to +3.3V 
#include <stdint.h>
#include "ADCSWTrigger.h"
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "Timer2.h"
#include "fixed.h"
#include "SysTickInts.h"
#include "servo.h"

#define PF2             (*((volatile uint32_t *)0x40025010))
#define PF1             (*((volatile uint32_t *)0x40025008))
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

volatile uint32_t ADCvalue;
volatile uint32_t Counts = 0;
//volatile uint32_t count = 0;
//volatile uint32_t duty = 15;
// This debug function initializes Timer0A to request interrupts
// at a 100 Hz frequency.  It is similar to FreqMeasure.c.
void Timer0A_Init100HzInt(void){
  volatile uint32_t delay;
  DisableInterrupts();
  // **** general initialization ****
  SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0
  delay = SYSCTL_RCGCTIMER_R;      // allow time to finish activating
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER0_CFG_R = 0;                // configure for 32-bit timer mode
  // **** timer0A initialization ****
                                   // configure for periodic mode
  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
 // TIMER0_TAILR_R = 799999;         // start value for 100 Hz interrupts
	  TIMER0_TAILR_R = 9999;  
  TIMER0_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// clear timer0A timeout flag
  TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 32-b, periodic, interrupts
  // **** interrupt initialization ****
                                   // Timer0A=priority 2
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; // top 3 bits
  NVIC_EN0_R = 1<<19;              // enable interrupt 19 in NVIC
}
	

void (*PeriodicTask)(void);   // user function
	static int32_t InitialTime = 0;
// ***************** TIMER1_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer1_Init(void){
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
  //PeriodicTask = task;          // user function
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER1_TAILR_R = 0xFFFFFFFF;    // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
 // TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00008000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 37, interrupt number 21
  //NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC
  TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
	InitialTime = TIMER1_TAR_R;
	
}


static int32_t Time[1000];
	static int32_t ADCval[1000];
	static int32_t i = 0;
	static int32_t tempi = 0;


void UserTask(void){

}
int32_t truSig = 0;
	int32_t time_dif[999];
	int32_t min_time = 0x7FFFFFFF;
	int32_t max_time = 0;
	int32_t ADCvalI[1000];
	int32_t length = 0;
	int32_t numADC[1000];
	int32_t	yesNew = 1;
	int32_t test = 0;
	int32_t x = 0;
	int negX = 0;
	int32_t d1 = 0;
	int32_t d23 = 0;
	int32_t d45 = 0;
		int32_t prevd1 = 0;
		int32_t avgd1 = 2000;
		int32_t avgI = 0;
		int32_t avgd1save = 0;
		int32_t plotd1 = 0;
		int32_t plot2d1 = 0;
		int32_t prevd23 = 0;
		int32_t avgd23 = 0;
		int32_t avgd23save = 0;		
		int32_t plotd23 = 0;
		int32_t plot2d23 = 0;
		int32_t prevd45 = 0;
		int32_t avgd45 = 0;
		int32_t avgd45save = 0;		
		int32_t plotd45 = 0;
		int32_t plot2d45 = 0;
		
		int32_t trud1 = 0;
		int32_t trud23 = 0;
		int32_t trud45 = 0;
 volatile uint32_t data[3];
 int32_t threshold1 = 500;
 int32_t threshold2 = 500;
//getADCvals
//Processes input from the global 'data' array
//(see ADCSWTrigger.c for ADC input configuration)
//Only changes globals associated with each ADC
// d1 = thumb
// d23 = index and middle
// d45 = ring and pinky
//
void getADCvals(void){
	
	//		 x = ADC0_InSeq3();
	d1 = data[0];
	trud1 = d1;
	d23 = data[1];
	trud23 = d23;
//	d45 = data[2];	
	avgI = avgI + 1;
	avgd1 = avgd1 + d1;
	avgd23 = avgd23 + d23;
	avgd45 = avgd45 + d45;	
////		
	d1 = d1 - avgd1save;
	d23 = d23 - avgd23save;
	d45 = d45 - avgd45save;	
	if(d1<0){
		d1 = d1*-1;
	}
	if(d23<0){
		d23 = d23*-1;
	}
	if(d45<0){
		d45 = d45*-1;
	}		
	double pre = prevd1;
	double now = d1;//(100*2.0*x)/4096;
	now = (now*.1) + (pre*(1-.1));
	d1 = now;
	prevd1 = d1;
//		
	pre = prevd23;
	now = d23;//(100*2.0*x)/4096;
	now = (now*.1) + (pre*(1-.1));
	d23 = now;
	prevd23 = d23;
	pre = prevd45;
	now = d45;//(100*2.0*x)/4096;
	now = (now*.1) + (pre*(1-.1));
	d45 = now;
	prevd45 = d45;		
//	
	if(avgI == 10){
		avgd1save = avgd1/10;
		avgI = 0;
		avgd1 = 0;
		avgd23save = avgd23/10;
		avgd23 = 0;
		avgd45save = avgd45/10;
		avgd45 = 0;				
	}

}
int32_t s1 =0;
int32_t s2 =0;
int32_t s3 =0;
int32_t s4 =0;
int32_t clenched1 = 0;
int32_t clenched2 = 0;
void getSwitch(void){
			 if( (GPIO_PORTB_DATA_R &= 0x01) == 0x01){
				 s1 = 0;
			 }else{
				 s1 = 1;
			 }
			 if( (GPIO_PORTB_DATA_R &= 0x02) == 0x02){
				 s2 = 0;
			 }else{
				 s2 = 1;
			 }			 if( (GPIO_PORTB_DATA_R &= 0x04) == 0x04){
				 s3 = 0;
			 }else{
				 s3 = 1;
			 }			 if( (GPIO_PORTB_DATA_R &= 0x08) == 0x08){
				 s4 = 0;
			 }else{
				 s4 = 1;
			 }			 
}

int main(void){

  PLL_Init(Bus80MHz);                   // 80 MHz
  SYSCTL_RCGCGPIO_R |= 0x20;  // activate port F
  //Counts = 0;
  GPIO_PORTF_DIR_R |= 0x04;   // make PF2 output (PF2 built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x04;// disable alt funct on PF2
  GPIO_PORTF_DEN_R |= 0x04;   // enable digital I/O on PF2
                              // configure PF2 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF0FF)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;     // disable analog functionality on PF
  SYSCTL_RCGCGPIO_R |= 0x02;  // activate port F
  //Counts = 0;
  GPIO_PORTB_DIR_R &= ~0x0F;   // make PB
  GPIO_PORTB_AFSEL_R &= ~0x0F;// disable alt funct on PB
  GPIO_PORTB_DEN_R |= 0x0F;   // enable digital I/O on PB
                              // configure PB as GPIO
  GPIO_PORTB_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFF0000)+0x00000000;
  GPIO_PORTB_AMSEL_R = 0;     // disable analog functionality on PB	
  SYSCTL_RCGCGPIO_R |= 0x08;  // activate port B
  //Counts = 0;
  GPIO_PORTD_DIR_R &= ~0x07;   // make PD output (PD built-in LED)
  GPIO_PORTD_AFSEL_R &= ~0x07;// disable alt funct on PD
  GPIO_PORTD_DEN_R |= 0x07;   // enable digital I/O on PD
                              // configure PD as GPIO
  GPIO_PORTD_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF000)+0x00000000;
  GPIO_PORTD_AMSEL_R = 0;     // disable analog functionality on PD	
	  SYSCTL_RCGCGPIO_R |= 0x02;  // activate port D
  Counts = 0;
  GPIO_PORTF_DIR_R &= 0x01;   // make PF2 output (PF2 built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x04;// disable alt funct on PF2
  GPIO_PORTF_DEN_R |= 0x04;   // enable digital I/O on PF2
                              // configure PF2 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF0FF)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;     // disable analog functionality on PF
  SYSCTL_RCGCGPIO_R |= 0x08;  // activate port F
	ADC_Init189();
	servoInit();
	EnableInterrupts();
	servoSetSpeed(5);
	ST7735_InitR(INITR_REDTAB);
	int32_t handClenched = 0;
	while(1){
		Timer0A_Init100HzInt(); //reset the timer   // set up Timer0A for 100 Hz interrupts
		length = 0;
		i = 0;
		int32_t prevx = 0;
		int32_t avg = 0;
		int32_t avgI = 0;
		int32_t plotx = 0;
		int32_t plotx2 = 0;		
		int Saveavg = 0;
		EnableInterrupts();
		while(1){ //main running
			PF1 ^= 0x02;  // toggles when running in main
			getADCvals();
			getSwitch();
			ST7735_SetCursor(0,0);	
	
							ST7735_DrawPixel(plotx, ((-4096/50))+140,ST7735_BLUE);	
							ST7735_DrawPixel(plotx, ((-3500/50))+140,ST7735_BLUE);	
							ST7735_DrawPixel(plotx, ((-3000/50))+140,ST7735_BLUE);	
							ST7735_DrawPixel(plotx, ((-2500/50))+140,ST7735_BLUE);				
							ST7735_DrawPixel(plotx, ((-2000/50))+140,ST7735_BLUE);
							ST7735_DrawPixel(plotx, ((-1500/50))+140,ST7735_BLUE);				
							ST7735_DrawPixel(plotx, ((-1000/50))+140,ST7735_BLUE);
							ST7735_DrawPixel(plotx, ((-500/50))+140,ST7735_BLUE);	
							if(truSig == 1){
							ST7735_DrawPixel(plotx, ((-trud1/50))+140,ST7735_YELLOW);			
							ST7735_DrawPixel(plotx, ((-trud23/50))+140,ST7735_CYAN);
							}else{
							ST7735_DrawPixel(plotx, ((-d1*2/50))+140,ST7735_YELLOW);			
							ST7735_DrawPixel(plotx, ((-d23*2/50))+140,ST7735_CYAN);
							}
							ST7735_DrawPixel(plotx, ((-threshold1*2/50))+140,ST7735_MAGENTA);
							ST7735_DrawPixel(plotx, ((-threshold2*2/50))+140,ST7735_GREEN);	
							ST7735_DrawPixel(plotx, ((0/50))+140,ST7735_GREEN);					
						//	ST7735_DrawPixel(plotx, ((-d45*2/50))+140,ST7735_MAGENTA);		
							Saveavg = avgd1save;//(avgd1save + avgd23save + avgd45save)/3;
							ST7735_DrawPixel(plotx, ((-Saveavg/50))+140,ST7735_RED);				
						
	

		ST7735_DrawFastVLine(plotx+1, 40, 160, ST7735_BLACK);
			plotx = plotx + 1;
			if(plotx > 120){
				//Output_Clear();
				plotx = 0;
			}
		if(s1 == 0){
			threshold1 = threshold1 - 2;
		}
		if(s2 == 0){
			threshold1 = threshold1 + 2;
		}		
		if(s3 == 0){
			threshold2 = threshold2 - 2;
		}
		if(s4 == 0){
			threshold2 = threshold2 + 2;
		}
		if((s4 == 0) && (s3 == 0)){
			ST7735_SetCursor(3,8);
			ST7735_OutString("Servo Control");
			getSwitch();
			while((s4 == 0) || (s3 == 0)){
					getSwitch();	
			}
			while((s4 != 0) || (s3 != 0)){
				if(s4 == 0){
					servoForward();
				}
				if(s3 == 0){
					servoBackward();
				}	
			getSwitch();				
			}
			while((s4 == 0) || (s3 == 0)){
					getSwitch();	
			}
		}
		if((s1 == 0) && (s2 == 0)){
			ST7735_SetCursor(3,8);
			if(truSig == 0){
				truSig = 1;
				ST7735_OutString("True Signal");
			}else{
				truSig = 0;
				ST7735_OutString("Processed Signal");				
			}
			getSwitch();
			while((s1 == 0) || (s2 == 0)){
						getSwitch();	
			}			
		}		
		if(threshold1 < 0){
			threshold1 = 0;
		}
		if(threshold2 < 0){
			threshold2 = 0;
		}		
		if(d1 >= threshold1){
			clenched1 = clenched1 + 1;
		}else{
			clenched1 = clenched1 - 1;
		}
		if(d23 >= threshold2){
			clenched2 = clenched2 + 1;
		}else{
			clenched2 = clenched2 - 1;
		}		
		if((clenched1 > 20) || (clenched2 > 20)){
			servoGrab();
		}else{
			servoUngrab();
		}
		if(clenched1 < 0){
			clenched1 = 0;
		}
		if(clenched1 > 21){
			clenched1 = 21;
		}
		if(clenched2 < 0){
			clenched2 = 0;
		}
		if(clenched2 > 21){
			clenched2 = 21;
		}
		 
						ST7735_SetCursor(0,2);
						ST7735_OutUDec(d1);
						ST7735_SetCursor(0,3);
						ST7735_OutUDec(d23);
						ST7735_SetCursor(0,4);
						ST7735_OutUDec(d45);	
						ST7735_SetCursor(5,1);
						ST7735_OutUDec(s1);
						ST7735_SetCursor(5,2);
						ST7735_OutUDec(s2);
						ST7735_SetCursor(5,3);
						ST7735_OutUDec(s3);
						ST7735_SetCursor(5,4);
						ST7735_OutUDec(s4);	
						ST7735_SetCursor(8,1);
						if(clenched1 > 10){
						ST7735_OutString("D1 YES");
						}	else{
						ST7735_OutString("D1  NO");
						}	
						ST7735_SetCursor(8,3);						
						if(clenched2 > 10){
						ST7735_OutString("D2 YES");
						}	else{
						ST7735_OutString("D2  NO");
						}							

		}
		DisableInterrupts();


	} //while(1)
} // main()


void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer0A timeout
	ADC_In189(data); 
  //PF2 ^= 0x04;                  // profile
}
void Timer1A_Handler(void){
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
 // (*PeriodicTask)();                // execute user task
}



