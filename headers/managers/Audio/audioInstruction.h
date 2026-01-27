#pragma once
#include "al.h"
#include "alc.h"
#include "sndfile.h"

struct AudioInstruction
{
	bool loopSound;
	float loopNrOfTimes; //0 = loops until something else tells it not to

	bool fadeIn;
	float fadeInTime;

	bool fadeOut;
	float fadeOutTime;
};