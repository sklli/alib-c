#ifndef TIMER_IS_DEFINED
#define TIMER_IS_DEFINED


#include "alib_error.h"
#include "time.h"
#include "TimerBase.h"

/* Basic timer object.  Stores a start time and a run time.
 * When a 'Timer_check()' call is made on the object, the time difference
 * is checked which will tell the caller if the timer has rung or not.
 *
 * The Timer object has memory so that once it has rung, it must be reset
 * before it can be reused.  This is done simply by calling 'Timer_begin()'.
 *
 * This uses all 64 bit integers for storing time.  If a timer is only needed
 * for a short period of time try using 'TimerLight', it uses 'size_t' instead
 * of 'uint64_t'.
 *
 * Inherits from 'TimerBase' (Private). */
typedef struct Timer Timer;

/*******Public Functions*******/
/* Resets the timer and sets the start time of the timer. */
void Timer_begin(Timer* t);
/* Checks to see if the timer has rung.  Once a timer has rung, it will
 * stay in the rung state until it is reset by 'Timer_reset()'.
 *
 * Returns:
 * 		!0: Timer has rung.
 * 		 0: Timer has not rung yet. */
char Timer_check(Timer* t);

	/* Getters */
/* Returns the time that the timer was started.
 *
 * Assumes 't' is not null. */
uint64_t Timer_get_start_time(Timer* t);
/* Returns the time to wait before ringing the timer.
 *
 * Assumes 't' is not null. */
uint64_t Timer_get_run_time(Timer* t);
/* Returns what magnitude the timer stores time in.
 *
 * Assumes 't' is not null. */
MagnitudeTime Timer_get_magnitude(Timer* t);
	/***********/

	/* Setters */
/* Sets the run time of the timer.
 *
 * Assumes 't' is not null. */
void Timer_set_run_time(Timer* t, uint64_t run_time);
/* Sets the magnitude of the timer.
 *
 * The timer's start and run time will be updated to the new magnitude.
 * WARNING: Overflow may occur, depending on the set internal values. */
void Timer_set_magnitude(Timer* t, MagnitudeTime mag);
	/***********/
/******************************/

/*******Constructors*******/
/* Constructs a new Timer object. */
Timer* newTimer(uint64_t run_time, MagnitudeTime mag);
/* Deletes a timer object. */
void delTimer(Timer** t);
/**************************/
#endif
