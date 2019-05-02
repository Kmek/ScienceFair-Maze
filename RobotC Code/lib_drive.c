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
	return rightDrive;
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
	motor[rightDrive] = RPower;
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

bool isOnRightLine() {
	return (SensorValue(rightLineTracker) > 1500);
}

bool isOnLeftLine() {
	return (SensorValue(leftLineTracker) > 1500);
}

void driveForwardInches(float inches, int power, long timeOut, BrakeType bType)
{
	short lPower = power;
	short rPower = power;

	resetMotorEncoder(rightDrive);
	resetMotorEncoder(leftDrive);

	// reset motor encoder to properly drive
	resetMotorEncoder(getDriveMotor());  //Always set to zero

	// converts inches to encoder ticks
	float encoderTicks = inches * 61.43;

	// gets a timer
	CTimer T = getFreeTimer();

	// while timeout has not run out and we have not reached target, drive
	while ((time(T) <  timeOut) && encoderLessThanTarget(getDriveMotor(), encoderTicks)) {
		driveMotors(lPower, rPower);

		// if we are over the right line, increase right power
		if (isOnRightLine()) {
			rPower = power + 10;
		} else {
			rPower = power;
		}

		// if we are over the left line, increase left power
		if (isOnLeftLine()) {
			lPower = power + 10;
		} else {
			lPower = power;
		}
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
