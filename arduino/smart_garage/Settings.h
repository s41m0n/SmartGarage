#ifndef __SETTINGS__
#define __SETTINGS__
//Different init time
#define TASK_GARAGE_MANAGER_TIME 50
#define TASK_PARK_ASSISTANT_TIME 50
#define TASK_SERIAL_MANAGER_TIME 50
#define SCHEDULER_TIME 50 //MCD

//PIN used
#define LR 3
#define LDIST1 6
#define LDIST2 5
#define TOUCH 12
#define CLOSE 11
#define PIR 7
#define PROX_ECHO 8
#define PROX_TRIG 9

//Time during pir detect
#define PIR_RELEVATION_TIME 5000
//How often park assistant print data
#define PRINT_TIME 1000

//Default Parameters for distance
#define DIST_MAX 1
#define DIST_MIN 0.1
#define DIST_MED (DIST_MAX+DIST_MIN)/2
#define DIST_CLOSE 0.5
//Intensity step to simulate led incremental fading in 2 seconds
#define FADE_STEP 255/(2000/TASK_GARAGE_MANAGER_TIME)

//Input
#define STR_OPEN "open"
#define STR_STOP "stop"

#endif
