//lib_utility.c

//Used for 5106C 2017-2018

/*************************
Includes:

-Pos/Neg Thing

**************************/

#ifndef FILE_UTILITY_CODE
#define FILE_UTILITY_CODE 1
#endif
#ifdef FILE_UTILITY_CODE

/* POS Forces value to positive */
int POS(int v) {
	if (v < 0)
		return -v;
	else
		return v;
}

/* NEG Forces value to negative */
int NEG(int v) {
	if (v > 0)
		return -v;
	else
		return v;
}


#endif
