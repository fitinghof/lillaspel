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

	void Tick();

	//MusicTrackManager Functions
	void InitializeMusicTrackManager(std::string pathToMusicFolder); // always end path with /
	void AddMusicTrackStandardFolder(std::string filename, std::string id);
	void AddMusicTrack(std::string path, std::string id);
	void SetMusicGain(float gain); //sets master volume for all music tracks
	void Play(std::string id);
	void Stop(std::string id);
	void FadeInPlay(std::string id, float startGain, float seconds);
	void FadeOutStop(std::string id, float seconds);
	void GetMusicTrackSourceState(std::string id, ALint& sourceState);
	void SetGain(std::string id, float gain); //sets gain for individual music track
	void SetPitch(std::string id, float pitch);
	MusicTrack* GetMusicTrack(std::string id);

private:
	ALCdevice* ALCDevice;
	ALCcontext* ALCContext;

	MusicTrackManager musicTrackManager;
};