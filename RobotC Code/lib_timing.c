// Timing.c

//Used for 5106C 2017-2018

/*************************
Includes:

-functions used for managing time

**************************/

#define TIMER_FUNCTIONS

typedef short CTimer;

int _TimerState[] = {0,0,0,0};

void initTimers(){
	for (CTimer i=0; i < 4; i++)
		_TimerState[i] = 0;
}

bool timerIsAvailable(int x)
{
	if (_TimerState[x] == 0)
		return true;
	else
		return false;
}
void useTimer(int i)
{
	_TimerState[i] = 1;
}

long time(short i)
{
	long t = 0;
	switch(i)
	{
	case 0: t = time1[T1];  break;
	case 1: t = time1[T2];	break;
	case 2: t = time1[T3]; 	break;
	case 3: t = time1[T4];	break;
	}
	return t;
}

CTimer getFreeTimer()
{
	for (CTimer i=0; i < 4; i++)
		if (timerIsAvailable(i))
	{
		switch(i)
		{
		case 0:  useTimer(0); clearTimer(T1);  break;
		case 1:  useTimer(1); clearTimer(T2);  break;
		case 2:  useTimer(2); clearTimer(T3);  break;
		case 3:  useTimer(3); clearTimer(T4);  break;
		}

		writeDebugStreamLine("Assigned Timer[%d] (%d, %d, %d, %d)",i, _TimerState[0],_TimerState[1],_TimerState[2],_TimerState[3]);
		return i;
	}

	writeDebugStreamLine("ERROR NO Timer Avaliable for Use");
	return -1;
}



void freeTimer(CTimer t)
{
	_TimerState[t] = 0;
	//writeDebugStreamLine("Released Timer[%d] (%d, %d, %d, %d)",t,  _TimerState[0],_TimerState[1],_TimerState[2],_TimerState[3]);

}

bool timeIsBetween(CTimer T, int startTime, int endTime){
	return ((time(T) >= startTime) && (time(T) <= endTime));
}


/***********************************************************
                      UNUSED CODE
************************************************************/

#ifdef UNUSED_CODE

void startTimer(short i) {
	switch(i)
	{
	case 0:  clearTimer(T1);  break;
	case 1:  clearTimer(T2);  break;
	case 2:  clearTimer(T3);  break;
	case 3:  clearTimer(T4);  break;
	}
}
#endif
