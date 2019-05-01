//lib_drive.c

//Used for 5106C 2017-2018

/*************************
Includes:

-Drive Enums
-Drive Functions
-Drive Task
-Unused Code

**************************/

/***********************************************************

DRIVE ENUMS

************************************************************/
enum BrakeType     { APPLY_BRAKE, NO_BRAKE};
enum TurnDirection { TURN_RIGHT, TURN_LEFT, NO_TURN };
enum DriveDirection { FORWARD, BACKUP };

/***********************************************************

DRIVE FUNCTIONS

************************************************************/

void resetDriveEncoders()
{
	resetMotorEncoder(leftDrive);
	resetMotorEncoder(rightDrive);
	wait1Msec(10);
}

short getDriveMotor() {
	return leftDrive;
}

bool isRobotBlocked(short motor, int expectedMinimumRPM) {
	int actualRPM = getMotorVelocity(motor);
	if (actualRPM < expectedMinimumRPM)
		return true; // robot is blocked
	else
		return false;
}

/**********************/
//Encapsulated Drive functions
/**********************/

void driveMotors(short LPower, short RPower) {
	//if (RPower == 0) {
			motor[rightDrive] = RPower;
	//} else {
	//motor[rightDrive] = RPower - 7;
	//}
	motor[leftDrive] = LPower;
}

void driveForwardPower(int power) {
	driveMotors(POS(power),POS(power));
}

void driveBackupPower(int power) {
	driveMotors(NEG(power), NEG(power));
}

void drivePowerRightTurn(int power) {
	driveMotors(POS(power),NEG(power));// + 7));
}

void drivePowerLeftTurn(int power) {
	driveMotors(NEG(power), POS(power)); // + 7));
}

/**********************/
//Separate Motor Drive Functions
/**********************/

//void separateMotorDrive(short LPower, short RPower, short timeOut) {
//	driveMotors(LPower, RPower);
//	wait1Msec(timeOut);
//}
void separateMotorDriveNoTimeOut(short LPower, short RPower) {
	driveMotors(LPower, RPower);
}

/**********************
Drive (NO_TURN, TURN_LEFT, TURN_RIGHT, FORWARD, BACKUP, power)
**********************/
void drive(DriveDirection dir, TurnDirection turn, int power) {
	switch(turn) {
	case NO_TURN:
		if (dir == FORWARD)
			driveMotors(POS(power),POS(power)); // + 7));
		else
			driveMotors(NEG(power),NEG(power)); // + 7));
		break;

	case TURN_LEFT:
		if (dir == FORWARD)
			drivePowerLeftTurn(power);
		else
			drivePowerRightTurn(power);
		break;

	case TURN_RIGHT:
		if (dir == FORWARD)
			drivePowerRightTurn(power);
		else
			drivePowerLeftTurn(power);
		break;
	}
}

/**********************/
//Brake Functions
/**********************/
void driveEngageBrake(){
	short brakePower = 5; //10
	int   brakeMS = 200; //200
	short l = motor[rightDrive];
	short r = motor[leftDrive];

	if (l != 0) {
		if (l > 0)
			motor[leftDrive] = -brakePower;
		else
			motor[leftDrive] = brakePower;
	}
	if (r != 0){
		if (r > 0)
			motor[rightDrive] = -brakePower;
		else
			motor[leftDrive] = brakePower;
	}

	if (l != 0 || r != 0 )
		wait1Msec(brakeMS);

	driveMotors(0,0);
}

/***/

void driveStop() {
	driveMotors(0,0);
}

/**********************/
//Different Drive Functions
/**********************/

void driveForward(int power, int driveTime, BrakeType bType) {
	driveForwardPower(power);
	wait1Msec(driveTime);
	if (bType == APPLY_BRAKE) {
		driveEngageBrake();
	}
	driveStop();
}

void driveBackup(int power, int driveTime, BrakeType bType) {
	driveBackupPower(power);
	wait1Msec(driveTime);
	if (bType == APPLY_BRAKE) {
		driveEngageBrake();
	}
	driveStop();
}

void driveBackupWithEncoderVCheck(int power, int driveTime, BrakeType bType, short motor, int expectedMinimumRPM, int startTimeForRobotBlocked)
{
	driveBackupPower(power);
	CTimer T = getFreeTimer();
	while (time(T) < driveTime)
	{
		if (time(T) > startTimeForRobotBlocked && isRobotBlocked(motor, expectedMinimumRPM))
		{
			break;
		}
		wait1Msec(10);
	}
	if (bType == APPLY_BRAKE) {
		driveEngageBrake();
	}
	driveStop();
}

//Turn Left for driveTime MS

void driveTurnLeft(short power, int driveTime, BrakeType bType) {
	drivePowerLeftTurn(power);
	wait1Msec(driveTime);
	if (bType == APPLY_BRAKE) {
		driveEngageBrake();
	}
	driveStop();
}

//Turn Right for driveTime MS

void driveTurnRight(short power, int driveTime, BrakeType bType) {
	drivePowerRightTurn(power);
	wait1Msec(driveTime);
	if (bType == APPLY_BRAKE) {
		driveEngageBrake();
	}
	driveStop();
}

/*******************************************
4.0 inch wheel (Diameter)
160 RPM Encoder 392 ticks = 1 full rotation = 12.568 inches
1 inch = 392 / 12.568 = 31.2 ticks per inch.
*******************************************/
long encoderTicksFromInches(float inches) {
	float encoderTicks = inches * 61.43;//31.2;
	return (long) encoderTicks;
}
/*******************************************
4.0 inch wheel (Diameter)
160 RPM Encoder 392 ticks = 1 full rotation = 12.568 inches
1 inch = 392 / 12.568 = 31.2 ticks per inch.
*******************************************/
long encoderTicksFromDegrees(float degrees) {
	float encoderTicks = degrees * 3.8;
	return (long) encoderTicks;
}

//////////////////////////////////////////////////////////////////////
//               Drive the Robot forward N inches                   //
//                                                                  //
// 4.0 inch wheel (Diameter)                                        //
// 3.142 is PII  (remember PII we need it here)                     //
// Circumference of a circle = PII * D = 4.0 * 3.142  =  12.568     //
// 160 RPM Encoder 392 ticks = 1 full rotation = 12.568 inches      //
// 1 inch = 392 / 12.568 = 31.2 ticks per inch.                     //
// Note: motors coast, so we had to fixup the result to get close   //
// OK we need to measure a constant to use this, that constant turned //
// out to be 3.8 = 1 degree.                                        //
//////////////////////////////////////////////////////////////////////

void driveForwardTurn(TurnDirection bTurnDirection, float degrees, int power, long timeOut, BrakeType bType)
{
	resetMotorEncoder(rightDrive);
	resetMotorEncoder(leftDrive);
	resetMotorEncoder(port3);  //Always set to zero

	float encoderTicks = degrees * 3.2; //3.8;  //((31.2 * 7.55) / 90.0);  // ok a guess that it takes 4 wheel rotations to make a 90 degree turn.
	//encoderTicks = 360.0;
	CTimer T = getFreeTimer();

	if (bTurnDirection == TURN_RIGHT)
		drivePowerRightTurn(power);
	else
		drivePowerLeftTurn(power);

	if (bTurnDirection == TURN_LEFT)
		while ((time(T) <  timeOut) && encoderLessThanTarget(port3, encoderTicks))	{
		wait1Msec(10);
	}
	else
		while ((time(T) <  timeOut) && encoderGreaterThanTarget(port3, -encoderTicks))	{
		wait1Msec(10);
	}

	if (bType == APPLY_BRAKE) {
		if (bTurnDirection == TURN_LEFT)
			drivePowerRightTurn(10);
		else
			drivePowerLeftTurn(10);
		wait1Msec(200);
	}
	driveStop();
	freeTimer(T);
}




//////////////////////////////////////////////////////////////////////
//               Drive the Robot forward N inches                   //
//                                                                  //
// 4.0 inch wheel (Diameter)                                       //
// 3.142 is PII  (remember PII we need it here)                     //
// Circumference of a circle = PII * D = 4.0 * 3.142  =  12.568     //
// 160 RPM Encoder 392 ticks = 1 full rotation = 12.568 inches         //
// 1 inch = 392 / 12.568 = 31.2 ticks per inch.                        //
// Note: motors coast, so we had to fixup the result to get close   //
//                                                                  //
//////////////////////////////////////////////////////////////////////

bool isOnRightLine() {
	return (SensorValue(rightLineTracker) > 1500);
}

bool isOnLeftLine() {
	return (SensorValue(leftLineTracker) > 1500);
}



void driveForwardInches(float inches, int power, long timeOut, BrakeType bType)
{
	resetMotorEncoder(getDriveMotor());  //Always set to zero

	float encoderTicks = inches * 61.43; //31.2;
	CTimer T = getFreeTimer();

	short lPower = power;
	short rPower = power;

	while ((time(T) <  timeOut) && encoderLessThanTarget(getDriveMotor(), encoderTicks)) {
		driveMotors(lPower, rPower);

		if (isOnRightLine()) {
			rPower = power + 5;
		} else {
			rPower = power;
		}

		if (isOnLeftLine()) {
			lPower = power + 5;
		} else {
			lPower = power;
		}
	}

	driveStop();
	freeTimer(T);
}

void driveForwardEncoder(int target, int power, long timeOut, BrakeType bType)
{
	resetMotorEncoder(getDriveMotor());  //Always set to zero

	CTimer T = getFreeTimer();

	driveForwardPower(power);
	while ((time(T) <  timeOut) && encoderLessThanTarget(getDriveMotor(), target))
		wait1Msec(10);

	if (bType == APPLY_BRAKE) {
		driveEngageBrake();
	}
	driveStop();
	freeTimer(T);
}

/************************************
Drive Backward Inches
************************************/
void driveBackwardInches(int inches, int power, long timeOut, BrakeType bType)
{
	resetMotorEncoder(getDriveMotor());  //Always set to zero

	float encoderTicks = inches * 31.2;
	CTimer T = getFreeTimer();

	encoderTicks = encoderTicks * -1; // make it negative as encoder is returning negative results

	driveBackupPower(power);
	while ((time(T) <  timeOut) && encoderGreaterThanTarget(getDriveMotor(), encoderTicks))
		wait1Msec(10);

	if (bType == APPLY_BRAKE) {
		driveEngageBrake();
	}
	driveStop();

	freeTimer(T);

}




/***********************************************************

DRIVE TASK

************************************************************/



/***Task***/

task driveTask() {

	while(true) {

		if (driveTaskIsEnabled())
			driveMotors(joystick_getLeftDriveSpeed(),joystick_getRightDriveSpeed());

		wait1Msec(10);
	}
}


/***********************************************************

UNUSED CODE AREA

************************************************************/
#ifdef UNUSED_CODE


void curveTurnThenSpeedBack(short LPower, short RPower, short timeOut, short secondTimeOut)
{
	CTimer T = getFreeTimer();

	driveMotors(LPower, RPower);

	while ((time(T) < timeOut))
	{
		wait1Msec(10);																									// don't hog the CPU
	}

	driveBackup(127, secondTimeOut, NO_BRAKE);

	freeTimer(T);
}

/***/

void driveAlignWithWall(short power, int timeOut){
	for (int i=0; i < timeOut ; i++) {
		driveBackupPower(POWER(power));
		wait1Msec(1);
	}
}

void driveForwardLR(int powerL, int powerR) {
	driveMotors((powerL),(powerR));
}

void driveForward(int power, int driveTime) {
	driveForwardPower(power);
	wait1Msec(driveTime);
	driveStop();
}

void driveApplyBackwardBrake(){
	driveStop();
	driveForwardPower(10);
	wait1Msec(250);
	driveStop();
}

bool driveGreaterThan(long target) {
	return encoderGreaterThanTarget( getDriveMotor(), target );
}



//////////////////////////////////////////////////////////////////////
//               Drive the Robot forward N inches                   //
//                                                                  //
// 3.25 inch wheel (Diameter)                                       //
// 3.142 is PII  (remember PII we need it here)                     //
// Circumference of a circle = PII * D = 3.25 * 3.142  =  10.2      //
// 160 RPM Encoder 392 ticks = 1 full rotation = 10.2 inches         //
// 1 inch = 392 / 10.2 = 38.4 ticks per inch.                        //
// Note: motors coast, so we had to fixup the result to get close   //
//                                                                  //
//////////////////////////////////////////////////////////////////////


void driveForwardInches(int inches, int power, long timeOut)
{
	resetMotorEncoder(getDriveMotor());  //Always set to zero

	float encoderTicks = inches * 38.4;
	CTimer T = getFreeTimer();

	driveForwardPower(power);
	while ((time(T) <  timeOut) && encoderLessThanTarget(getDriveMotor(), encoderTicks))
		wait1Msec(10);

	driveApplyForwardBrake();

	freeTimer(T);
}

/************************************
Drive Backward Inches
************************************/
void driveBackwardInches(int inches, int power, long timeOut)
{
	resetMotorEncoder(getDriveMotor());  //Always set to zero

	float encoderTicks = inches * 38.4;
	CTimer T = getFreeTimer();

	encoderTicks = encoderTicks * -1; // make it negative as encoder is returning negative results

	driveBackupPower(power);
	while ((time(T) <  timeOut) && encoderGreaterThanTarget(getDriveMotor(), encoderTicks))
		wait1Msec(10);

	driveApplyBackwardBrake();

	freeTimer(T);

}

bool driveLessThan(long target) {
	return encoderLessThanTarget( getDriveMotor(), target );
}


#define CONVERSION_INCH_2_TICKS 38.4

long inches2Ticks(int inches) {
	float fInches = (float) inches;
	float encoderTicks = fInches * CONVERSION_INCH_2_TICKS;
	long retval = (long) encoderTicks;

	return retval;
}


void driveBackupPowerRaw(short leftPower, short rightPower){

	if (leftBackBumperPushed())
		leftPower = 0;

	if (rightBackBumperPushed())
		rightPower = 0;
	drive(NEG(leftPower), NEG(rightPower));
}


#endif
