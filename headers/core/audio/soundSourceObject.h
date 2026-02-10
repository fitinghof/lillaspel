#pragma once
#include "al.h"
#include "alc.h"
#include <string>
#include <iostream>
#include "sndfile.h"
#include "audioInstruction.h"
#include "soundClip.h"
#include "utilities/logger.h"
#include "utilities/masterVolume.h"
#include "gameObjects/gameObject3D.h"
#include <random>

class SoundSourceObject : public GameObject3D
{
public:
	SoundSourceObject();
	~SoundSourceObject();
	SoundSourceObject(const SoundSourceObject&) = delete;
	SoundSourceObject& operator=(const SoundSourceObject&) = delete;

	virtual void Tick() override;

	void Play(SoundClip* soundClip);
	void SetGain(float gain);
	void SetPitch(float pitch);
	void SetRandomPitch(float minPitch, float maxPitch);
	void ChangePitch(float pitchChange);
	void ChangeGain(float gainChange);
	void SetAudioInstruction(AudioInstruction instructionSet);
	void SetId(int newId);
	int GetId();

	void GetSourceState(int index, ALint& sourceState); //for debug
	void GetCurrentSourcePosition(ALfloat* position); //for debug

	void LinkMasterSoundEffectsGain(float* masterGainLink);

private:
	int id = -1;
	int nrOfSources = 6;
	int sourceIndex = 0;
	ALuint* sources;
	AudioInstruction currentInstructionSet;

	float pitch = 1;
	float gain = 1;
	float* masterGain = nullptr;
	float velocity[3] = { 0, 0, 0 };
};