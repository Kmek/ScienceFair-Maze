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
int armJoystickDZ = 40;
int liftJoystickDZ = 30;

/******************************
Buttons
*****************************/

// Open/Close Claw
bool pincerCloseButton() { return (vexRT[Btn5D] == 1); }
bool pincerOpenButton()  { return (vexRT[Btn5U] == 1); }

// Plunger Button
bool mobileGoalEjectorButton()    { return (vexRT[Btn8D] == 1); }
bool emergencyPlungerInButton()		{ return (vexRT[Btn8L] == 1); }
bool emergencyPlungerOutButton()	{ return (vexRT[Btn8R] == 1); }

// 20pt Mobile Goal Button
bool mobileGoal20Button()			{ return (vexRT[Btn8U] == 1); }

// Button that brings arm down + locks it
bool armAndLiftDownButton() { return (vexRT[Btn7D] == 1); }
//bool activatePreloadScorerButtonX() { return (vexRT[Btn7R] == 1); }

// Emergency BuBtn6D
bool emergencyDownMGLButton()			{ return (vexRT[Btn6D] == 1); }
bool emergencyUpMGLButton()			{ return (vexRT[Btn6U] == 1); }

//bool testArmPreloadButton()			{ return (vexRT[Btn7U] == 1); }

//bool preloadLoaderButtonX()			{ return (vexRT[Btn7L] == 1);	}

bool coneScoreButton()					{ return (vexRT[Btn7U] == 1); }

bool addConeForAutomaticButton()	{ return (vexRT[Btn7L] == 1); }
bool subtractConeForAutomaticButton() { return (vexRT[Btn7R] == 1); }

// Logger Buttons to Start and Stop the task
//bool logEnableButton() { return (vexRT[Btn8L] == 1); }
//bool logDisableButton() { return (vexRT[Btn8R] == 1); }



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

/******************************/
// Joystick Settings
// Lift and Arm Joystick
/*****************************/

// Lift
short joystick_getLiftPower() {
	int power = vexRT[Ch3];											//Channel 4 is used for the claw
	if (POS(power)<= liftJoystickDZ)					//Uses the deadzone to make sure the claw does not move
		power = 0;
	return power;
}

/*****************************/

// Arm
short joystick_getArmPower() {
	int power = vexRT[Ch4];										  //Channel 3 is used for the arm
	if (POS(power)<= armJoystickDZ)						  //Uses the deadzone to make sure the arm does not move up/down when stopped
		power = 0;
	return power;
}

/*****************************/
