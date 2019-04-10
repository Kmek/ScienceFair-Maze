#pragma config(Motor,  port2,           leftDrive,     tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port3,           rightDrive,    tmotorVex393HighSpeed_MC29, openLoop, reversed, encoderPort, I2C_2)

// Science Fair Maze Code

#include "includes.h"

task main()
{
	startTask(driveTask);
	enableDriveTask();

	while (true) {
		wait1Msec(10);
	}

}
