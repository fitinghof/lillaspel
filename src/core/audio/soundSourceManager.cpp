#include "core/audio/soundSourceManager.h"

SoundSourceObject* SoundSourceManager::GetSoundSource(int id)
{
	for (int i = 0; i < this->soundSources.size(); i++)
	{
		if (id == this->soundSources[i]->GetId())
		{
			return this->soundSources[i];
		}
	}

	Logger::Error("SoundSource with id: does not exist!");
	return nullptr;
}

void SoundSourceManager::AddSoundSource(SoundSourceObject* speaker)
{
	speaker->SetId(this->idCounter);
	this->idCounter++;

	this->soundSources.push_back(speaker);
}

void SoundSourceManager::SetListenerPosition(float x, float y, float z)
{
	alListener3f(AL_POSITION, x, y, z);
}