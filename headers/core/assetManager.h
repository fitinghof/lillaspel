#pragma once
#include <vector>
#include <unordered_map>
#include "gameObjects/meshObjData.h"

#include "../headers/core/audio/soundEngine.h"
#include "gameObjects/mesh.h"
#include "rendering/material.h"
#include "gameObjects/objectLoader.h"

class AssetManager
{
public:
	AssetManager() = default;
	~AssetManager() = default;

	void InitializeSoundBank(std::string pathToSoundFolder); //end the path with /
	void AddSoundClipStandardFolder(std::string filename);
	void AddSoundClip(std::string path);

	bool GetMaterial(std::string identifier);
	bool GetMesh(std::string identifier);
	MeshObjData GetTemplate(std::string identifier);


	SoundClip* GetSoundClipStandardFolder(std::string filename);
	SoundClip* GetSoundClip(std::string path);

private:
	MusicTrack currentMusicTrack;
	SoundBank soundBank;
	ObjectLoader objectLoader;

	std::unordered_map<std::string, >
};

