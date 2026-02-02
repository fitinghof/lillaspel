#pragma once
#include "al.h"
#include "alc.h"
#include "sndfile.h"
#include <iostream>
#include "utilities/logger.h"

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