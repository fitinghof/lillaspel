#pragma once

class MasterVolume
{
public:
	MasterVolume(MasterVolume& other) = delete;
	MasterVolume& operator=(const MasterVolume&) = delete;

	static MasterVolume& GetInstance();

	void SetMusicGain(float musicMaster);
	float GetMusicGain() const;
	void SetSoundEffectsGain(float musicMaster);
	float GetSoundEffectsGain() const;

private:
	MasterVolume() = default;

	float music = 1.0f;
    float soundEffects = 1.0f;
};

inline MasterVolume& MasterVolume::GetInstance()
{
	static MasterVolume instance;
	return instance;
}

inline void MasterVolume::SetMusicGain(float musicMaster) 
{
	this->music = musicMaster;
}

inline float MasterVolume::GetMusicGain() const
{
	return this->music;
}

inline void MasterVolume::SetSoundEffectsGain(float soundEffectsMaster)
{
	this->soundEffects = soundEffectsMaster;
}

inline float MasterVolume::GetSoundEffectsGain() const
{
	return this->soundEffects;
}
