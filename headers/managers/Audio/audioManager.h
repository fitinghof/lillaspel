#pragma once
#include "al.h"
#include "alc.h"
#include "sndfile.h"
#include <iostream>

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	void Initialize();

private:
	ALCdevice* ALCDevice;
	ALCcontext* ALCContext;
};