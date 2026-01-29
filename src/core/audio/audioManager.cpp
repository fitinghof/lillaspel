#include "../../../headers/core/audio/audioManager.h"

AudioManager::AudioManager()
{
	this->ALCDevice = alcOpenDevice(nullptr);
	if (!this->ALCDevice) Logger::Error("failed to get sound device!");

	this->ALCContext = alcCreateContext(this->ALCDevice, nullptr);
	if (!this->ALCContext) Logger::Error("failed to create context!");

	if (!alcMakeContextCurrent(this->ALCContext)) Logger::Error("failed to set this as current context!");

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(this->ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
	{
		name = alcGetString(this->ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	}
	if (!name || alcGetError(this->ALCDevice) != AL_NO_ERROR)
	{
		name = alcGetString(this->ALCDevice, ALC_DEVICE_SPECIFIER);
	}

	std::string n = name;
	Logger::Log("opened " + n);
}

AudioManager::~AudioManager()
{
	if (!alcMakeContextCurrent(nullptr)) Logger::Error("failed to unset context during termination!");

	alcDestroyContext(this->ALCContext);
	if (this->ALCContext) Logger::Error("failed to destroy context!");

	if (!alcCloseDevice(this->ALCDevice)) Logger::Error("failed to close sound device!");
}

void AudioManager::Initialize()
{
	//glahöe
}
