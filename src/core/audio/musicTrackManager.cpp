#include "core/audio/musicTrackManager.h"

MusicTrackManager::MusicTrackManager()
{
}

MusicTrackManager::~MusicTrackManager()
{
	for (const auto& it : this->musicTracks)
	{
		if (it.second != nullptr)
		{
			delete it.second;
		}
	}

	this->musicTracks.clear();
}

void MusicTrackManager::Initialize(std::string standardFolder)
{
	this->pathToMusicFolder = standardFolder;
}

void MusicTrackManager::AddMusicTrackStandardFolder(std::string filename, std::string id)
{
	MusicTrack* newTrack = new MusicTrack();
	newTrack->Initialize(this->pathToMusicFolder + filename, id);

	this->musicTracks.insert(std::make_pair(id, newTrack));
}

void MusicTrackManager::Play(std::string id)
{
	this->musicTracks[id]->Play();
	this->activeTracks.push_back(this->musicTracks[id]);
}

void MusicTrackManager::Stop(std::string id)
{
	this->musicTracks[id]->Stop();
	
	for (int i = 0; i < this->activeTracks.size(); i++)
	{
		if (this->activeTracks[i]->id == id)
		{
			this->activeTracks.erase(this->activeTracks.begin() + i);
			return;
		}
	}

	Logger::Log("failed to stop music track : " + id + ": it isn't active");
}

void MusicTrackManager::FadeInPlay(std::string id, float startGain, float seconds)
{
	this->musicTracks[id]->FadeIn(startGain, seconds);
	this->Play(id);
}

void MusicTrackManager::GetMusicTrackSourceState(std::string id, ALint& sourceState)
{
	this->musicTracks[id]->GetSourceState(sourceState);
}

void MusicTrackManager::SetGain(std::string id, float gain)
{
	this->musicTracks[id]->SetGain(gain);
}

MusicTrack* MusicTrackManager::GetMusicTrack(std::string id)
{
	return this->musicTracks[id];
}

void MusicTrackManager::Tick()
{
	for (int i = 0; i < this->activeTracks.size(); i++)
	{
		this->activeTracks[i]->UpdateBufferStream();
	}
}

void MusicTrackManager::AddMusicTrack(std::string path, std::string id)
{
	MusicTrack* newTrack = new MusicTrack();
	newTrack->Initialize(path, id);

	this->musicTracks.insert(std::make_pair(id, newTrack));
}
