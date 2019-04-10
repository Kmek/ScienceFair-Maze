// Defines.h includes

//Used for 5106C 2017-2018

/*************************
Includes:

-common defines

defined in includes.h used to make the code more readable
these defines simply disappear when precompiled, but in the
code it makes it easy to track the meaning of parameters passed
to a function.

for example, driveForward(POWER(127), TIMEOUT(250));
after precompile looks like driveForward(127,250); less readable.
**************************/
#define POWER(x) x
#define RIGHT_POWER(x) x
#define LEFT_POWER(x) x
#define TIMEOUT(x) x
#define TIME_IN_MS(x) x
#define DISTANCE(x) x
#define TARGET_DISTANCE_CM(x) x
#define WAIT_TIME(x) x
#define HOLD_POWER(x) x
#define STOP_WHEN_TARGET_IS_CLOSER_THAN(x) x
#define TIMING(x) x

#define INCHES(x) x
#define HUNT_CLOSE_TO_MM(x) x
#define DEGREES(x) x
#define BACKUP_INCHES(x) x

#define START_TIME(x) x
#define END_TIME(x) x
#define START_POWER(x) x
#define END_POWER(x) x
#define COMPLETE_BY_TIME(x) x

#define TIME(x) x
#define FORWARD_MS(x) x
#define BACKUP_MS(x) x
#define DRIVE_TIME(x) x
#define DRIVE_FORWARD_MS(x) x
#define DRIVE_BACKUP_MS(x) x
#define DRIVE_START_FOWWARD_MS(x) x
#define DRIVE_START_BACKUP(x) x
#define DRIVE_END_BACKUP(x) x
#define DRIVE_POWER(x) x
#define DRIVE_MS(x) x
#define DRIVE_START_MS(x) x
#define DRIVE_DELAY_MS(x) x

#define LOWER_ARM_POWER(x) x
#define LOWER_ARM_MS(x) x

#define armForward_POWER(x) x
#define armForward_MS(x) x
#define armForward_START_MS(x) x
#define armForward_END_MS(x) x

#define DELAY_SHOT_MS(x) x
#define START_BACKUP_MS(x) x

#define CONE_RELEASE_START(x) x
#define ARM_FORWARD_START(x) x

#define MAX_TIME 120000

#define TARGET(x) x
#define FLICK_START(x) x
#define FLICK_LAUNCH(x) x

#define ARM_HEIGHT(x) x
#define LIFT_TIME(x) x

#define DEFAULT(x) x

#define MIN_MM(x) x
#define MAX_MM(x) x
#define ENCODER_TARGET(x) x
#define RIGHT_ENCODER_TARGET(x) x
#define LEFT_ENCODER_TARGET(x) x
#define MS(x) x
