/*
initializes port f and systick, as well as other servo variables
*/
void servoInit();

/*
set speed 0-5
if you set something greater than 5 it goes to 5
negatives get counted as positives
DOES NOT CHANGE DUTY
*/
void servoSetSpeed(short speed);

/*
increase speed by one
will not go higher than 5
DOES NOT CHANGE DUTY
*/
void servoIncrementSpeed(void);
	
/*
decrement speed by one
will not go less than 0
DOES NOT CHANGE DUTY
*/
void servoDecrementSpeed(void);
	
/*
set servos to go forward
DOES NOT CAHNGE DUTY
*/
void servoSetForwards(void);
	
/*
set servos to go backwards
DOES NOT CHANGE DUTY
*/
void servoSetBackwards(void);
	
/*
flip direction
DOES NOT CHANGE DUTY
*/
void servoChangeDirection(void);
	
/*
stop the servos
*/
void servoStop(void);

/*
make the servo go
*/
void servoGo(void);

//TODO
/*
these two use systick timer, which is the same timer used to output duty.
should be able to be timed down to .1 ms, which is the period of the systick
will use the saved speed and direction to determine the time to spin
*/
void servoGrab(void);
void servoUngrab(void);

