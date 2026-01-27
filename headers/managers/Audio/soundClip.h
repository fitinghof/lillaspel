#pragma once
#include "al.h"
#include "alc.h"
#include <string>

struct SoundClip
{
	std::string filepath;
	ALuint bufferID;
};