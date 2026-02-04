#pragma once
#include "al.h"
#include "alc.h"
#include "sndfile.h"
#include <iostream>
#include "utilities/logger.h"
#include "core/audio/musicTrackManager.h"

//ALuint's can be safely copied according to AI

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	void Initialize();

private:
	ALCdevice* ALCDevice;
	ALCcontext* ALCContext;

	MusicTrackManager musicTrackManager;
};