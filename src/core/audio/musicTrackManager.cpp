#include "core/audio/musicTrackManager.h"

MusicTrackManager::MusicTrackManager()
{
}

MusicTrackManager::~MusicTrackManager()
{
	//ADD UNIQUE POINTERS LATER, MEMORY WILL BE LOST!
	this->musicTrakcs.clear();
}

void MusicTrackManager::Initialize(std::string standardFolder)
{
	this->pathToMusicFolder = standardFolder;
}

void MusicTrackManager::AddMusicTrackStandardFolder(std::string filename, std::string id)
{
	MusicTrack* newTrack = new MusicTrack();
	newTrack->Initialize(this->pathToMusicFolder);
	newTrack->LoadTrackStandardFolder(filename, id);

	this->musicTracks.insert(std::make_pair(id, newTrack));
}

void MusicTrackManager::PlayMusicTrack(std::string id)
{
	this->musicTracks[id]->Play();
}

void MusicTrackManager::AddMusicTrack(std::string path, std::string id)
{
}
