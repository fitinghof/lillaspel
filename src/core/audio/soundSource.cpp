#include "../../../headers/core/audio/soundSource.h"

SoundSource::SoundSource()
{
	this->currentInstructionSet.fadeIn = false; //will look at this later
	this->currentInstructionSet.fadeInTime = 0;
	this->currentInstructionSet.fadeOut = false;
	this->currentInstructionSet.fadeOutTime = 0;
	this->currentInstructionSet.loopSound = false;
	this->currentInstructionSet.loopNrOfTimes = 0;

	this->sources = new ALuint[this->nrOfSources];
	this->currentBuffers = new ALuint[this->nrOfSources];
	alGenSources(this->nrOfSources, this->sources);

	for (int i = 0; i < this->nrOfSources; i++)
	{
		alSourcef(this->sources[i], AL_PITCH, this->pitch);
		alSourcef(this->sources[i], AL_GAIN, this->gain);
		alSource3f(this->sources[i], AL_POSITION, this->position[0], this->position[1], this->position[2]);
		alSource3f(this->sources[i], AL_VELOCITY, this->velocity[0], this->velocity[1], this->velocity[2]);
		alSourcei(this->sources[i], AL_LOOPING, this->currentInstructionSet.loopSound);
	}

	//alSourcei(source, AL_BUFFER, *this->currentBuffer);
}

SoundSource::~SoundSource()
{
	alDeleteSources(this->nrOfSources, this->sources);
	delete[] this->sources;
	delete[] this->currentBuffers;
}

void SoundSource::Play(SoundClip* soundClip)
{
	for (int i = 0; i < this->nrOfSources; i++)
	{
		int index = (this->sourceIndex + i) % this->nrOfSources;

		ALint state;
		alGetSourcei(this->sources[index], AL_SOURCE_STATE, &state);

		if (state != AL_PLAYING)
		{
			alSourcei(this->sources[index], AL_BUFFER, (ALint)soundClip->bufferID);
			this->currentBuffers[index] = soundClip->bufferID;
			alSourcePlay(this->sources[index]);

			// Next search will start after this one
			this->sourceIndex = (index + 1) % this->nrOfSources;
			return;
		}
	}

	// No free source, overwrite the next
	alSourcei(this->sources[this->sourceIndex], AL_BUFFER, (ALint)soundClip->bufferID);
	this->currentBuffers[this->sourceIndex] = soundClip->bufferID;
	alSourcePlay(this->sources[this->sourceIndex]);

	this->sourceIndex = (this->sourceIndex + 1) % this->nrOfSources;
}

void SoundSource::SetId(int newId)
{
	this->id = newId;
}

int SoundSource::GetId()
{
	return this->id;
}

void SoundSource::GetSourceState(int index, ALint& sourceState)
{
	return alGetSourcei(this->sources[index], AL_SOURCE_STATE, &sourceState);
}

void SoundSource::GetCurrentSourcePosition(ALfloat* position)
{
	alGetSource3f(sources[this->sourceIndex], AL_POSITION, &position[0], &position[1], &position[2]);
}

void SoundSource::SetPosition(float x, float y, float z)
{
	this->position[0] = x;
	this->position[1] = y;
	this->position[2] = z;

	for (int i = 0; i < this->nrOfSources; i++)
	{
		alSource3f(this->sources[i], AL_POSITION, (ALfloat)this->position[0], (ALfloat)this->position[1], (ALfloat)this->position[2]);
	}
}

void SoundSource::SetGain(float gain)
{
	this->gain = gain;

	for (int i = 0; i < this->nrOfSources; i++)
	{
		alSourcef(this->sources[i], AL_GAIN, this->gain);
	}
}

void SoundSource::SetPitch(float pitch)
{
	this->pitch = pitch;

	for (int i = 0; i < this->nrOfSources; i++)
	{
		alSourcef(this->sources[i], AL_PITCH, this->pitch);
	}
}

void SoundSource::ChangePitch(float pitchChange)
{
	this->pitch += pitchChange;

	for (int i = 0; i < this->nrOfSources; i++)
	{
		alSourcef(this->sources[i], AL_PITCH, this->pitch);
	}
}

void SoundSource::ChangeGain(float gainChange)
{
	this->gain += gainChange;

	for (int i = 0; i < this->nrOfSources; i++)
	{
		alSourcef(this->sources[i], AL_GAIN, this->gain);
	}
}

void SoundSource::SetAudioInstruction(AudioInstruction instructionSet)
{
	this->currentInstructionSet = instructionSet;
	for (int i = 0; i < this->nrOfSources; i++)
	{
		alSourcei(this->sources[i], AL_LOOPING, this->currentInstructionSet.loopSound);
		//add more stuff later
	}
}
