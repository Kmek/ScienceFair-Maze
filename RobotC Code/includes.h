//includes.h

//Used for 5106C 2017-2018

/*************************
Links up all of our Files together
**************************/

#ifndef INCLUDES
#define INCLUDES 1

// Necessary Files with Globals, Enums, etc
#include "lib_globals.c"
#include "lib_defines.h"
#include "lib_timing.c"
#include "lib_utility.c"

// Joystick File with controls
#include "lib_joystick.c"

// Encoders and Sonar
#include "lib_encoder.c"

// Libraries for movement (drive, mg ejector, mg litfer)
#include "lib_drive.c"

// For science fair
#include "lib_pathfinding.c"

#endif
