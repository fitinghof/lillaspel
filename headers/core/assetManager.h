#pragma once
#include <vector>
#include <unordered_map>

#include "../headers/core/audio/soundEngine.h"

class AssetManager
{
public:
	AssetManager();
	~AssetManager();



private:
	MusicTrack currentMusicTrack;
	SoundBank soundBank;
};

