#pragma once
#include "al.h"
#include "alc.h"
#include <string>
#include <iostream>
#include "sndfile.h"
#include "audioInstruction.h"
#include "utilities/logger.h"

class MusicTrack
{
public:
	MusicTrack();
	~MusicTrack();

	std::string pathToMusicFolder = "";
	std::string id = "";
	
	void Play();
	void Pause();
	void Restart();

	void Initialize(std::string pathToMusicFolder);
	void UpdateBufferStream();
	void LoadTrackStandardFolder(std::string filename, std::string id);
	void LoadTrack(std::string filepath, std::string id);
	void SetPitch(float pitch);
	void SetGain(float gain);
	void SetAudioInstruction(AudioInstruction audioInstruction);

private:
	ALuint source;
	AudioInstruction audioInstruction;
	float pitch = 1.0f;
	float gain = 1;
	float position[3] = { 0, 0, 0 };
	float velocity[3] = { 0, 0, 0 };

	static const int BUFFER_SAMPLES = 8192;
	static const int NUM_BUFFERS = 4;
	ALuint buffers[NUM_BUFFERS];
	SNDFILE* sndfile;
	SF_INFO sfInfo;
	short* membuf;
	ALenum format;
};