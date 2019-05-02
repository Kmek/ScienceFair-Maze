//lib_joystick.c

//Used for 5106C 2017-2018

/*************************
Includes:

-Button Placement
-Joystick Functions

**************************/

#ifndef FILE_JOYSTICK
#define FILE_JOYSTICK 1
#endif

//Joystick DeadZones
int joyStickDeadZone = 5;

/******************************/
// Joystick Settings
// Driving Joystick
/*****************************/

// Left Drive
short joystick_getLeftDriveSpeed() {
	short power = (vexRT[Ch2] + vexRT[Ch1] );  	// get the left side speed from joystick

	if (POS(power) <= joyStickDeadZone)       // check to see if left value is within dead zone.
		power = 0;                              // if in the dead zone then zero it out

	return power;
}

/*****************************/

// Right Drive
short joystick_getRightDriveSpeed() {
	short power =  (vexRT[Ch2] - vexRT[Ch1]); 	// get the right side speed from the joystick


	if (POS(power) <= joyStickDeadZone)       // check to see if right value is within dead zone
		power = 0;                              // if in the dead zone then zero it out
	return power;
}
