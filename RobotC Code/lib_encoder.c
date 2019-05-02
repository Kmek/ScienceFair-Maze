// encoder.c

//Used for 5106C 2017-2018

/*************************
Includes:

-Encoder Functions
-Unused Code

**************************/

#ifndef FILE_ENCODER_CODE
#define FILE_ENCODER_CODE 1
#endif

/***********************************************************

                ENCODER FUNCTIONS + VARIABLES

************************************************************/

// makes up for a motor that postive goes up but encoder values are negative.
long getEncoderValue(tMotor m) {
	long retval = nMotorEncoder[m];
	if (m == port8) // awful
		retval = retval * -1;

	return retval;
}

void resetAllEncoders() {
	resetMotorEncoder(port1);
	resetMotorEncoder(port2);
	resetMotorEncoder(port3);
	resetMotorEncoder(port4);
	resetMotorEncoder(port5);
	resetMotorEncoder(port6);
	resetMotorEncoder(port7);
	resetMotorEncoder(port8);
	resetMotorEncoder(port9);
	resetMotorEncoder(port10);
	//for (int i=0; i < 10; i++)
	//	resetMotorEncoder(getMotorEncoder(i));
	//wait1Msec(20);
}

// Resets all encoders
// Used in preauton
void resetEncoders()
{
	initTimers();
	resetAllEncoders();
	bLCDBacklight = true;
}

bool isEncoderWorking(tMotor m) {

	unsigned long encoderTimeStamp = 0;
	long encoderValue = 0;
	//Updates the encoder and timestamp values of the motor
	getEncoderAndTimeStamp(m, encoderValue, encoderTimeStamp);
	if (encoderTimeStamp == 0 ) {// || !encoderInRange(m)) <--- Need to think about this more and do more testing...
		return false;
	}
	else {
		return true;
	}
}

///*******************************************************
// check to see if we have reached the target
// returns true if encoder has failed, or we are less than target
// returns false encoder greater = the target and encoder is working
//*******************************************************/
bool encoderLessThanTarget(tMotor theMotor, long theTarget) {
	return (getEncoderValue(theMotor) <= theTarget); /*
	if (isEncoderWorking(theMotor))
	{
		return (getEncoderValue(theMotor) <= theTarget);
	}
	else
		return false;*/
}

bool encoderGreaterThanTarget(tMotor theMotor, long theTarget) {

	if (isEncoderWorking(theMotor))
	{
		return (getEncoderValue(theMotor) >= theTarget);
	}
	else
		return false;
}

/*********************************************/
//ABSOLUTE VALUE ENCODER FUNCTIONS
/*********************************************/

bool encoderLessThanTargetABS(tMotor theMotor, long theTarget) {

	if (isEncoderWorking(theMotor))
	{
		return (POS(getEncoderValue(theMotor)) <= POS(theTarget));
	}
	else
		return false;
}

bool encoderGreaterThanTargetABS(tMotor theMotor, long theTarget) {

	if (isEncoderWorking(theMotor))
	{
		return (POS(getEncoderValue(theMotor)) >= POS(theTarget));
	}
	else
		return false;
}

/*******************************************************
Encoder between
*******************************************************/
bool encoderBetweenABS(tMotor m, long a, long b){
	if ((POS(getEncoderValue(m)) >= POS(a)) && (POS(getEncoderValue(m)) <= POS(b)))
		return true;
	else
		return false;
}

/***********************************************************

                      UNUSED CODE

************************************************************/

#ifdef UNUSED_CODE


/*******************************************************
Encoder in Range
port8 is the armEncoder
port4 is the clawEncoder
retuns true unless test for specific encoders fails.
*******************************************************/
bool encoderInRange(tMotor m) {
	if (m == port8 && !encoderBetween(m, 0, 2000)) return false;
	if (m == port4 && !encoderBetween(m, 0,  900)) return false;
	return true;
}


#endif


/***********************************************************
                      UNUSED CODE
************************************************************/

#ifdef UNUSED_CODE
/*******************************************************
GENERIC moveMotorToEncoderTarget encapsulates functionality
*******************************************************/
bool moveMotorToEncoderTarget(tMotor theMotor, int theTarget, int thePower, int theTimeOut) {

	CTimer T = getFreeTimer();

	motor[theMotor] = thePower;																				// start the motor moving

	while ((time(T) < theTimeOut) && isEncoderWorking(theMotor) && getEncoderValue(theMotor) < theTarget)
	{
		wait1Msec(10);																									// don't hog the CPU
	}
	motor[theMotor] = 0;																							// we are all done now

	freeTimer(T);

	return isEncoderWorking(theMotor);

}


#endif
