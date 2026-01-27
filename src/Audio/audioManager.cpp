#include "AudioManager.h"

AudioManager::AudioManager()
{
	this->ALCDevice = alcOpenDevice(nullptr);
	if (!this->ALCDevice) std::cout << "failed to get sound device!" << std::endl;

	this->ALCContext = alcCreateContext(this->ALCDevice, nullptr);
	if (!this->ALCContext) std::cout << "failed to create context!" << std::endl;

	if(!alcMakeContextCurrent(this->ALCContext)) std::cout << "failed to set this as current context!" << std::endl;

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(this->ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
	{
		name = alcGetString(this->ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	}
	if (!name || alcGetError(this->ALCDevice) != AL_NO_ERROR)
	{
		name = alcGetString(this->ALCDevice, ALC_DEVICE_SPECIFIER);
	}

	std::cout << "opened " << name << std::endl;
}

AudioManager::~AudioManager()
{
	if (!alcMakeContextCurrent(nullptr)) std::cout << "failed to unset context during termination!" << std::endl;

	alcDestroyContext(this->ALCContext);
	if (this->ALCContext) std::cout << "failed to destroy context!" << std::endl;

	if (!alcCloseDevice(this->ALCDevice)) std::cout << "failed to close sound device!" << std::endl;
}

void AudioManager::Initialize()
{
	//glahöe
}
