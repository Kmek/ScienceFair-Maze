#pragma config(Motor,  port2,           leftDrive,     tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port3,           rightDrive,    tmotorVex393HighSpeed_MC29, openLoop, reversed, encoderPort, I2C_2)

// Science Fair Maze Code

#include "includes.h"

#define X_COORD 0
#define Y_COORD 1

// False == open
// True == closed
int tiles[][] = {
	{false, false, false, false, false, false},
	{false, false, false, false, false, false},
	{false, false, false, false, false, false},
	{false, false, false, false, false, false},
	{false, false, false, false, false, false},
	{false, false, false, false, false, false}};

float tilesFCost[6][6] = {
	{-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1}};

int tilesGCost[6][6] = {
	{-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1}};

float tilesHCost[6][6] = {
	{-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1}};

// Checks the paths by 2s for x, y
int tilesPathCoords[36][6] = {
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1},

	{-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1},
};

int startCoord[] = {0, 0};
int endCoord[]   = {5, 5};

/**********************************************/

int indexFor(int blockX, int blockY) {
	return (blockY * 6) + blockX;
};

// 0=Right, 1=Down, 2=Left, 3=Up
int nearCo[][] = {
	{1, 0, -1, 0},
	{0, 1, 0, -1}
};

bool isInGrid(int xCoord, int yCoord) {
	if ((yCoord < 0) || (yCoord >= 6) || (xCoord < 0) || (xCoord >= 6)) {
		return false;
	}
	return true;
}

//
bool isMoveable(int direction, int xCoord, int yCoord) {
	if (isInGrid(xCoord + nearCo[X_COORD][direction], yCoord + nearCo[Y_COORD][direction])) {
		if (tiles[xCoord + nearCo[X_COORD][direction]][yCoord + nearCo[Y_COORD][direction]] == false
			&& tilesFCost[xCoord + nearCo[X_COORD][direction]][yCoord + nearCo[Y_COORD][direction]] == -1
			&& (xCoord + nearCo[X_COORD][direction] != startCoord[X_COORD] && yCoord + nearCo[Y_COORD][direction] != startCoord[Y_COORD])) {
			return true;
		}
	}
return false;
}

int getGCost(int xCoord, int yCoord) {
	int gCost;

	if (tilesGCost[xCoord][yCoord] == -1) {
		gCost = floor((sizeof(tilesPathCoords[indexFor(xCoord, yCoord)]) - 1) / 2);
		tilesGCost[xCoord][yCoord] = gCost;
	} else {
		gCost = tilesGCost[xCoord][yCoord];
	}

	return gCost;
}

float getHCost(int xCoord, int yCoord) {
	float hCost;

	if (tilesHCost[xCoord][yCoord] == -1) {
		long a = pow(endCoord[X_COORD] - xCoord, 2);
		long b = pow(endCoord[Y_COORD] - yCoord, 2);

		hCost = round(sqrt(a+b) * 100) / 100;
		tilesHCost[xCoord][yCoord] = hCost;
	} else {
		hCost = tilesHCost[xCoord][yCoord];
	}

	return hCost;
}

float getFCost(int xCoord, int yCoord) {
	float fCost;

	if (tilesFCost[xCoord][yCoord] == -1) {
		float cost = getGCost(xCoord, yCoord) + getHCost(xCoord, yCoord);
		fCost = round(cost * 100) / 100;

		tilesFCost[xCoord][yCoord] = fCost;
	}

	return tilesFCost[xCoord][yCoord];
}

task main()
{
	startTask(driveTask);
	enableDriveTask();

	while (true) {
		wait1Msec(10);
	}

}
