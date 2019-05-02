
#define X_COORD 0
#define Y_COORD 1

// open == unchecked!
enum tileType {OPEN, CLOSED, CHECKED, VISITED, START, END};
enum Direction {NORTH, SOUTH, EAST, WEST};

Direction robotDir = SOUTH;

tileType tiles[6][6] = {
	{OPEN,  OPEN, OPEN,   OPEN,   OPEN,   OPEN},
	{OPEN, OPEN, OPEN,   OPEN,   OPEN,   OPEN},
	{OPEN, OPEN, OPEN, OPEN, OPEN, OPEN},
	{OPEN, OPEN,   OPEN,   OPEN,   OPEN, OPEN},
	{OPEN, OPEN,   OPEN,   OPEN,   OPEN, OPEN},
	{OPEN, OPEN,   OPEN,   OPEN,   OPEN,   OPEN}};

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
int tilesPathCoords[36][73] = {
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1},
	{-1}, {-1}, {-1}, {-1}, {-1}, {-1}
};

int startCoord[] = {0,0};
int currCoord[]  = {0,0};
int endCoord[] = {3,3};

void init() {
	tilesGCost[startCoord[X_COORD]][startCoord[Y_COORD]] = 0;
	tiles[startCoord[X_COORD]][startCoord[Y_COORD]] = VISITED;
}

int checkedCount = 0;
bool breakBool = false;

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

// co is X_COORD or Y_COORD
int getPathIndex(int co, int tile) {
	return (((tile - 1) * 2) + co);
}

int getGCost(int xCoord, int yCoord) {
	int gCost;

	if (tilesGCost[xCoord][yCoord] == -1) {
		int tile = indexFor(xCoord, yCoord);
		gCost = tilesGCost[currCoord[X_COORD]][currCoord[Y_COORD]] + 1;

		// memcpy won't work and robotc is bad, so here's another for loop

		// copying the last path to the current tile's path one coordinate half at a time
		// there ought to be (last tile's gCost * 2) values in the array
		for (int i = 0; i < tilesGCost[currCoord[X_COORD]][currCoord[Y_COORD]] * 2; i++) {
			tilesPathCoords[tile][i] = tilesPathCoords[indexFor(currCoord[X_COORD], currCoord[Y_COORD])][i];
		}

		if (tilesPathCoords[indexFor(xCoord, yCoord)][0] == -1) {
			//replace the coord
			tilesPathCoords[tile][0] = xCoord;
			tilesPathCoords[tile][1] = yCoord;
		} else {
			tilesPathCoords[tile][getPathIndex(X_COORD, gCost)] = xCoord;
			tilesPathCoords[tile][getPathIndex(Y_COORD, gCost)] = yCoord;
		}
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

//0=Right, 1=Down, 2=Left, 3=Up
// checks that a tile in a direction is moveable by robot (not closed)
bool isMoveable(int direction, int xCoord, int yCoord) {
	// checks that its in the grid
	if (isInGrid(xCoord + nearCo[X_COORD][direction], yCoord + nearCo[Y_COORD][direction])) {
		// checks that the tile is OPEN
		if (tiles[xCoord + nearCo[X_COORD][direction]][yCoord + nearCo[Y_COORD][direction]] == OPEN) {
			// AND that the coords are not the start coords
			if (xCoord + nearCo[X_COORD][direction] == startCoord[X_COORD] && yCoord + nearCo[Y_COORD][direction] == startCoord[Y_COORD]) {
				return false;
		} else {
			return true;
		}}
	}
return false;
}

void lightCheck(int xCoord, int yCoord) {
	for (int i = 0; i < 4; i++) {
		if (isMoveable(i, xCoord, yCoord)) {
			getFCost(xCoord + nearCo[X_COORD][i], yCoord + nearCo[Y_COORD][i]);

			tiles[xCoord + nearCo[X_COORD][i]][yCoord + nearCo[Y_COORD][i]] = CHECKED;
			checkedCount++;
		}
	}
}

void lowestFCost() {
	int index[] = {-1, -1};
	float lowestFCost = -1;

	for (int i = 0; i < 6; i++) {
		for (int k = 0; k < 6; k++) {
			if (tiles[i][k] == CHECKED) {
				if (lowestFCost == -1 || tilesFCost[i][k] < lowestFCost) {
					index[X_COORD] = i;
					index[Y_COORD] = k;
					lowestFCost = tilesFCost[i][k];
				}
			}
		}
	}
	// sets the currCoord to the lowest f cost
	currCoord[X_COORD] = index[X_COORD];
	currCoord[Y_COORD] = index[Y_COORD];

	tiles[currCoord[X_COORD]][currCoord[Y_COORD]] = VISITED;
}

void turnAround() {
	driveForwardTurn(TURN_LEFT, 10, 50, 800, APPLY_BRAKE);
}

void turnLeft() {
	drivePowerLeftTurn(50);
	wait1Msec(250);
	drivePowerRightTurn(5);
	wait1Msec(100);
	driveStop();
	//driveForwardTurn(TURN_LEFT, 10, 50, 400, APPLY_BRAKE);
}

void turnRight() {
	drivePowerRightTurn(50);
	wait1Msec(250);
 	drivePowerLeftTurn(5);
 	wait1Msec(100);
	driveStop();
	//driveForwardTurn(TURN_RIGHT, 10, 50, 400, APPLY_BRAKE);
}

void driveOneTile() {
	driveForwardInches(8.9, 25, 1350, APPLY_BRAKE);
}

void moveRobot() {
	int lastGCost = tilesGCost[currCoord[X_COORD]][currCoord[Y_COORD]] + 1; // + 1 for the last tile
	int tile = indexFor(currCoord[X_COORD], currCoord[Y_COORD]);

	int robotX = startCoord[X_COORD];
	int robotY = startCoord[Y_COORD];

	for (int i = 1; i < lastGCost; i++) {
		int nextXCoord = tilesPathCoords[tile][getPathIndex(X_COORD, i)];
		int nextYCoord = tilesPathCoords[tile][getPathIndex(Y_COORD, i)];

		// Robot must go North
		if (nextYCoord < robotY) {

			// Must move to face North
			if (robotDir == SOUTH) {
				turnAround();
			} else if (robotDir == EAST) {
				turnLeft();
			} else if (robotDir == WEST) {
				turnRight();
			}

			robotDir = NORTH;

		} // ROBOT HAS TO DRIVE SOUTH
		else if (nextYCoord > robotY) {
			// face south!
			if (robotDir == NORTH) {
				turnAround();
			} else if (robotDir == EAST) {
				turnRight();
			} else if (robotDir == WEST) {
				turnLeft();
			}

			robotDir = SOUTH;
		} // ROBOT HAS TO DRIVE EAST
		else if (nextXCoord > robotX) {
			// face east!
			if (robotDir == NORTH) {
				turnRight();
			} else if (robotDir == SOUTH) {
				turnLeft();
			} else if (robotDir == WEST) {
				turnAround();
			}

			robotDir = EAST;
		} // ROBOT HAS TO DRIVE WEST
		else if (nextXCoord < robotX) {
			// face east!
			if (robotDir == NORTH) {
				turnLeft();
			} else if (robotDir == SOUTH) {
				turnRight();
			} else if (robotDir == EAST) {
				turnAround();
			}
			robotDir = WEST;
		}

		wait1Msec(500);
		driveOneTile();
		wait1Msec(500);

		robotX = nextXCoord;
		robotY = nextYCoord;
	}
}

void checkAndMoveSimplified() {

	writeDebugStreamLine("CheckAndMoveSimplified");
	if (checkedCount == 0 && currCoord != startCoord) {
		// one last check
		lightCheck(currCoord[X_COORD], currCoord[Y_COORD]);

		if (checkedCount == 0) {
			breakBool = true;
		}
	} else if (currCoord[X_COORD] == endCoord[X_COORD] && currCoord[Y_COORD] == endCoord[Y_COORD]) {
		int tile = indexFor(currCoord[X_COORD], currCoord[Y_COORD]);

		tilesPathCoords[tile][getPathIndex(X_COORD, tilesGCost[currCoord[X_COORD]][currCoord[Y_COORD]])] = currCoord[X_COORD];
		tilesPathCoords[tile][getPathIndex(Y_COORD, tilesGCost[currCoord[X_COORD]][currCoord[Y_COORD]])] = currCoord[Y_COORD];

		//yay done!
		moveRobot();
		breakBool = true;
	} else {
		lightCheck(currCoord[X_COORD], currCoord[Y_COORD]);

		lowestFCost();

		if (currCoord != endCoord) {
			tiles[currCoord[X_COORD]][currCoord[Y_COORD]] = VISITED;
		}
	}
}
