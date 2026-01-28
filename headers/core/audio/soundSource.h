#pragma once
#include "al.h"
#include "alc.h"
#include <string>
#include <iostream>
#include "sndfile.h"
#include "audioInstruction.h"
#include "soundClip.h"
#include "utilities/logger.h"

class SoundSource
{
public:
	SoundSource();
	~SoundSource();

	void Play(SoundClip soundClip);
	void SetPosition(float* vector3);
	void SetGain(float gain);
	void SetPitch(float pitch);
	void ChangePitch(float pitchChange);
	void ChangeGain(float gainChange);
	void SetAudioInstruction(AudioInstruction instructionSet);

private:
	int nrOfSources = 6;
	int sourceIndex = 0;
	ALuint* sources;
	ALuint* currentBuffers;
	AudioInstruction currentInstructionSet;

	float pitch = 1;
	float gain = 1;
	float position[3] = { 0, 0, 0 };
	float velocity[3] = { 0, 0, 0 };
};