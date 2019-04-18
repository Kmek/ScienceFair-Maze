
#define X_COORD 0
#define Y_COORD 1

// open == unchecked!
enum tileType {OPEN, CLOSED, CHECKED, VISITED, START, END};
enum Direction {NORTH, SOUTH, EAST, WEST};

Direction robotDir = SOUTH;

tileType tiles[6][6] = {
	{OPEN, OPEN, OPEN, OPEN, OPEN, OPEN},
	{OPEN, OPEN, OPEN, OPEN, OPEN, OPEN},
	{OPEN, OPEN, OPEN, OPEN, OPEN, OPEN},
	{OPEN, OPEN, OPEN, OPEN, OPEN, OPEN},
	{OPEN, OPEN, OPEN, OPEN, OPEN, OPEN},
	{OPEN, OPEN, OPEN, OPEN, OPEN, OPEN}};

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
int endCoord[] = {5,5};

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
			//getpathindex(take the gcost of the current tile + 1)
			tilesPathCoords[tile][getPathIndex(X_COORD, gCost)] = xCoord;
			tilesPathCoords[tile][getPathIndex(Y_COORD, gCost)] = yCoord;
			//tempArray[getPathIndex(X_COORD, tilesGCost[currCoord[X_COORD]][currCoord[Y_COORD]])] = xCoord;
			//tempArray[getPathIndex(Y_COORD, tilesGCost[currCoord[X_COORD]][currCoord[Y_COORD]])] = yCoord;
		}

		// the previous tile's gcost + 1
		 //gcost of the prev + 1; //floor((sizeof(tilesPathCoords[indexFor(xCoord, yCoord)]) - 1) / 2);
		tilesGCost[xCoord][yCoord] = gCost;

		//tilesPathCoords[indexFor(xCoord, yCoord)] = tempArray;

		/*
		tilesPathCoords[indexFor(xCoord, yCoord)][0]++;
		index = tilesPathCoords[indexFor(xCoord, yCoord)][0];

		tilesPathCoords[indexFor(xCoord, yCoord)][index] = xCoord;
		tilesPathCoords[indexFor(xCoord, yCoord)][index + 1] = yCoord; */
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
		//writeDebugStreamLine("IsInGrid!");
		// checks that the tile is OPEN
		if (tiles[xCoord + nearCo[X_COORD][direction]][yCoord + nearCo[Y_COORD][direction]] == OPEN) {
			//writeDebugStreamLine("IS OPEN!");

			//writeDebugStreamLine("XCoord %d, YCoord %d, near xCoord %d, near yCoord %d", xCoord, yCoord, xCoord + nearCo[X_COORD][direction], yCoord + nearCo[Y_COORD][direction]);

			// AND that the coords are not the start coords
			if (xCoord + nearCo[X_COORD][direction] == startCoord[X_COORD] && yCoord + nearCo[Y_COORD][direction] == startCoord[Y_COORD]) {
				//writeDebugStreamLine("is start coord!");
				return false;
		} else {
			return true;
		}}
	}
return false;
}

void lightCheck(int xCoord, int yCoord) {
	//writeDebugStreamLine(" LightCheck");
	for (int i = 0; i < 4; i++) {
		//writeDebugStreamLine("In for loop");
		if (isMoveable(i, xCoord, yCoord)) {
			//writeDebugStreamLine("isMoveable!");
			float fcost = getFCost(xCoord + nearCo[X_COORD][i], yCoord + nearCo[Y_COORD][i]);
			//writeDebugStreamLine("f cost: %f", fcost);

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
				//writeDebugStreamLine("got into this if statement");
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

	writeDebugStreamLine("Visited Index of x: %d    Index of y: %d", index[X_COORD], index[Y_COORD]);
}

void turnAround() {
	driveForwardTurn(TURN_LEFT, 180, 50, 5000, APPLY_BRAKE);
}

void turnLeft() {
	driveForwardTurn(TURN_LEFT, 85, 50, 5000, APPLY_BRAKE);
}

void turnRight() {
	driveForwardTurn(TURN_RIGHT, 85, 50, 5000, APPLY_BRAKE);
}

void driveOneTile() {
	driveForwardInches(9, 25, 5000, NO_BRAKE);
}

void moveRobot() {
	int lastGCost = tilesGCost[currCoord[X_COORD]][currCoord[Y_COORD]];
	int tile = indexFor(currCoord[X_COORD], currCoord[Y_COORD]);

	int robotX = startCoord[X_COORD];
	int robotY = startCoord[Y_COORD];

	for (int m = 1; m < lastGCost; m++) {
		writeDebugStreamLine("coord set: %d %d", tilesPathCoords[tile][getPathIndex(X_COORD, m)], tilesPathCoords[tile][getPathIndex(Y_COORD, m)]);
		m++;
		}

	for (int i = 1; i < lastGCost; i++) {
		int nextXCoord = tilesPathCoords[tile][getPathIndex(X_COORD, i)];
		int nextYCoord = tilesPathCoords[tile][getPathIndex(Y_COORD, i)];

		// ROBOT HAS TO DRIVE NORTH ^
		if (nextYCoord < robotY) {

			// face north!
			if (robotDir == SOUTH) {
				turnAround();
			} else if (robotDir == EAST) {
				turnLeft();
			} else if (robotDir == WEST) {
				turnRight();
			}

			robotDir = NORTH;
			driveOneTile();
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
			driveOneTile();
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
			driveOneTile();
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

			wait1Msec(250);
			robotDir = WEST;
			driveOneTile();
			wait1Msec(250);
		}

		robotX = nextXCoord;
		robotY = nextYCoord;
	}
}

// hmm
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
			writeDebugStreamLine("Visited Index of x: %d    Index of y: %d", currCoord[X_COORD], currCoord[Y_COORD]);
			tiles[currCoord[X_COORD]][currCoord[Y_COORD]] = VISITED;
		}
	}
}

void test() {
	if (isMoveable(3, 0, 0)) {
		driveForwardInches(100, 30, 5000, APPLY_BRAKE);
	}
}
