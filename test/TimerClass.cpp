#include "TimerClass.h"



LTimer::LTimer()
{
	mStartTicks = 0;
	mPauseTicks = 0;
	
	mStarted = false;
	mPaused = false;

}

LTimer::~LTimer()
{
}

void LTimer::start()
{
	//Start the timer
	mStarted = true;

	//Unpause the timer
	mPaused = false;

	//Get the current clock time
	mStartTicks = SDL_GetTicks();
	mPauseTicks = 0;
}

void LTimer::pause()
{
	//If the timer is runing an isn't already paused
	if (mStarted && !mPaused)
	{
		//Pause the timer
		mPaused = true;

		Uint32 current = SDL_GetTicks();
		//Claculate the paused ticks
		mPauseTicks = current - mStartTicks;
		mStartTicks = 0;
	}
}

void LTimer::unpause()
{
	//If the timer is running and paused
	if (mStarted && mPaused)
	{
		//Unpause the timer
		mPaused = false;

		//Current time
		Uint32 current = SDL_GetTicks();

		//Reset the starting ticks
		mStartTicks = current - mPauseTicks;

		//Reset the paused ticks
		mPauseTicks = 0;
	}

}

void LTimer::stop()
{
	//Stop the timer
	mStarted = false;

	//Unpause the timer
	mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPauseTicks = 0;
}

Uint32 LTimer::getTicks()
{
	Uint32 time = 0;
	//If the timer is running
	if (mStarted)
	{
		//If the timer is paused
		if (mPaused)
		{
			//Return the number of ticks when the timer was paused
			time = mPauseTicks;
		}
		else
		{
			//Return the current time minus the start time
			time = SDL_GetTicks() - mStartTicks;
		}
	}

	return time;
}

bool LTimer::isStarted()
{
	//Timer is running and paused or unpaused
	return mStarted;
}

bool LTimer::isPause()
{
	//Timer is running and paused
	return mPaused && mStarted;
}