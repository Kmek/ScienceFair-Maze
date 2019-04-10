//Globals.c

//Used for 5106C 2017-2018

/*************************
Includes:

-Various Globals

**************************/

/**************************************************
Drive Task Control
**************************************************/
bool GLOBAL_DriveTaskTaskEnabledFlag = true;

bool driveTaskIsEnabled(){
	bool retval = false;
	retval =  GLOBAL_DriveTaskTaskEnabledFlag;
	return retval;
}

void enableDriveTask(){
	if (!driveTaskIsEnabled())
		GLOBAL_DriveTaskTaskEnabledFlag = true;
}

void disableDriveTask() {
	if (driveTaskIsEnabled())
		GLOBAL_DriveTaskTaskEnabledFlag = false;
}
