#pragma once
#include "al.h"
#include "alc.h"
#include <string>
#include <iostream>
#include "sndfile.h"
#include "AudioInstruction.h"

class MusicTrack
{
public:
	MusicTrack(std::string pathToSoundFolder);
	~MusicTrack();

	const std::string pathToSoundFolder;
	
	void Play();
	void Pause();
	void Restart();

	void UpdateBufferStream();
	void LoadTrackStandardFolder(std::string filename);
	void LoadTrack(std::string filepath);
	void SetPitch(float pitch);
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

	MusicTrack() = delete;
};