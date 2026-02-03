#pragma once
#include <vector>
#include <unordered_map>
#include "gameObjects/meshObjData.h"

#include "../headers/core/audio/soundEngine.h"
#include "gameObjects/mesh.h"
#include "rendering/material.h"

class AssetManager
{
public:
	AssetManager() = default;
	~AssetManager() = default;

	void InitializeSoundBank(std::string pathToSoundFolder); //end the path with /
	void AddSoundClipStandardFolder(std::string filename);
	void AddSoundClip(std::string path);

	std::string AddMesh(Mesh* mesh);
	std::string AddMaterial(Mesh* mesh);
	std::string AddObjTemplate(meshObjData data);

	SoundClip* GetSoundClipStandardFolder(std::string filename);
	SoundClip* GetSoundClip(std::string path);

private:
	MusicTrack currentMusicTrack;
	SoundBank soundBank;
};

