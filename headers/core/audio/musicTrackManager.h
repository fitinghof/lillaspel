#pragma once
#include <unordered_map>
#include "core/audio/musicTrack.h"

class MusicTrackManager
{
public:
	MusicTrackManager();
	~MusicTrackManager();

	void Initialize(std::string standardFolder);

	/// <summary>
	/// filename or path is ONLY used for loading, id is used for Get()'ing a track
	/// </summary>
	/// <param name="filename"></param>
	/// <param name="id"></param>
	void AddMusicTrackStandardFolder(std::string filename, std::string id);
	void AddMusicTrack(std::string path, std::string id);
	void PlayMusicTrack(std::string id);

private:
	std::unordered_map<std::string, MusicTrack*> musicTracks;
	std::string pathToMusicFolder = "";
};