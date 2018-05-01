#pragma once
#include <conio.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <sstream>
using namespace std;

class LTimer
{
public:
	//initialize 
	LTimer();
	~LTimer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	Uint32 getTicks();

	//Checks the status of the timer
	bool isStarted();
	bool isPause();

private:

	Uint32 mStartTicks;
	Uint32 mPauseTicks;

	//The timer status
	bool mPaused;
	bool mStarted;

};



