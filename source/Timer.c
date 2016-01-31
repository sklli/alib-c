#include "Timer_private.h"

/*******Private Functions*******/
/* Gets the current time based on the timers magnitude and sets
 * 'out_time'.
 *
 * Does not check for bad arguments. */
static void get_time(uint64_t* out_time, struct TimerBase* t)
{
	clock_gettime(CLOCK_MONOTONIC, &t->time);

	/* Get the start time. */
	switch(t->magnitude)
	{
	case SECONDS:
		*out_time = t->time.tv_sec;
		break;
	case MILLIS:
		*out_time = SECONDS_TO_MILLIS(t->time.tv_sec);
		*out_time += NANOS_TO_MILLIS(t->time.tv_nsec);
		break;
	case MICROS:
		*out_time = SECONDS_TO_MICROS(t->time.tv_sec);
		*out_time += NANOS_TO_MICROS(t->time.tv_nsec);
		break;
	case NANOS:
		*out_time = SECONDS_TO_NANOS(t->time.tv_sec);
		*out_time += t->time.tv_nsec;
		break;
	case PICOS:
	default:
		*out_time = SECONDS_TO_PICOS(t->time.tv_sec);
		*out_time += t->time.tv_nsec;
	}
}
/*******************************/

/*******Public Functions*******/
/* Resets the timer and sets the start time of the timer. */
void Timer_begin(Timer* t)
{
	if(!t)return;

	get_time(&t->start_time, &t->base);
	t->base.rang = 0;
}
/* Checks to see if the timer has rung.  Once a timer has rung, it will
 * stay in the rung state until it is reset by 'Timer_reset()'.
 *
 * Returns:
 * 		!0: Timer has rung.
 * 		 0: Timer has not rung yet. */
char Timer_check(Timer* t)
{
	if(!t)return(ALIB_BAD_ARG);
	if(t->base.rang)return(t->base.rang);
	
	uint64_t now, ellapsed;
	get_time(&now, &t->base);
	
	if(now < t->start_time)
	{
		ellapsed = UINT64_MAX - t->start_time;
		ellapsed += now;
	}
	else
		ellapsed = now - t->start_time;
	
	if(ellapsed >= t->run_time)
		t->base.rang = 1;
	
	return(t->base.rang);
}

	/* Getters */
/* Returns the time that the timer was started.
 *
 * Assumes 't' is not null. */
uint64_t Timer_get_start_time(Timer* t){return(t->start_time);}
/* Returns the time to wait before ringing the timer.
 *
 * Assumes 't' is not null. */
uint64_t Timer_get_run_time(Timer* t){return(t->run_time);}
/* Returns what magnitude the timer stores time in.
 *
 * Assumes 't' is not null. */
MagnitudeTime Timer_get_magnitude(Timer* t){return(t->base.magnitude);}
	/***********/

	/* Setters */
/* Sets the run time of the timer.
 *
 * Assumes 't' is not null. */
void Timer_set_run_time(Timer* t, uint64_t run_time){t->run_time = run_time;}
/* Sets the magnitude of the timer.
 *
 * The timer's start and run time will be updated to the new magnitude.
 * WARNING: Overflow may occur, depending on the set internal values. */
void Timer_set_magnitude(Timer* t, MagnitudeTime mag)
{
	if(!t)return;

	char mag_diff = mag - t->base.magnitude;

	/* If we decrease the magnitude (representing a smaller value),
	 * then we must multiply. */
	for(; mag_diff < 0; ++mag_diff)
	{
		t->run_time *= 1000;
		t->start_time *= 1000;
	}
	/* Increasing the magnitude (representing a larger value),
	 * must divide. */
	for(; mag_diff > 0; --mag_diff)
	{
		t->run_time /= 1000;
		t->start_time /= 1000;
	}

	t->base.magnitude = mag;
}
	/***********/
/******************************/

/*******Constructors*******/
/* Constructs a new Timer object. */
Timer* newTimer(uint64_t run_time, MagnitudeTime mag)
{
	Timer* t = malloc(sizeof(Timer));
	if(!t)return(NULL);
	
	t->start_time = 0;
	t->run_time = run_time;
	t->base.rang = 1;
	t->base.magnitude = mag;
	
	return(t);
}
/* Deletes a timer object. */
void delTimer(Timer** t)
{
	delTimerBase((TimerBase**)t);
}
/**************************/
