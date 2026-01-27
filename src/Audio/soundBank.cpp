#include "SoundBank.h"

SoundBank::SoundBank(std::string pathToSoundFolder) : pathToSoundFolder(pathToSoundFolder)
{
}

SoundBank::~SoundBank()
{
}

void SoundBank::AddSoundClipStandardFolder(const std::string filename)
{
	CreateSoundBuffer(this->pathToSoundFolder + filename);
}

void SoundBank::AddSoundClip(const std::string relativePath)
{
	CreateSoundBuffer(relativePath);
}

SoundClip SoundBank::GetSoundClipStandardFolder(const std::string filename)
{
	std::string fullpath = this->pathToSoundFolder + filename;

	auto it = this->soundClips.find(fullpath);
	if (it != this->soundClips.end())
	{
		return this->soundClips[fullpath];
	}

	std::cout << "couldn't find " << fullpath << std::endl;
	SoundClip nullClip;
	nullClip.filepath = "";
	nullClip.bufferID = -1;

	return nullClip;
}

SoundClip SoundBank::GetSoundClip(const std::string relativePath)
{
	auto it = this->soundClips.find(relativePath);
	if (it != this->soundClips.end())
	{
		return this->soundClips[relativePath];
	}

	std::cout << "couldn't find " << relativePath << std::endl;
	SoundClip nullClip;
	nullClip.filepath = "";
	nullClip.bufferID = -1;

	return nullClip;
}

bool SoundBank::RemoveSoundClipStandardFolder(const std::string filename)
{
	std::string fullpath = this->pathToSoundFolder + filename;

	auto it = this->soundClips.find(fullpath);
	if (it != this->soundClips.end())
	{
		this->soundClips.erase(it);
		return true;
	}

	return false;
}

bool SoundBank::RemoveSoundClip(const std::string relativePath)
{
	auto it = this->soundClips.find(relativePath);
	if (it != this->soundClips.end())
	{
		this->soundClips.erase(it);
		return true;
	}

	return false;
}

void SoundBank::CreateSoundBuffer(std::string fullpath)
{
	ALenum error, format;
	ALuint buffer = 0;
	SNDFILE* sndfile;
	SF_INFO sfInfo;
	short* membuf;
	sf_count_t numFrames;
	ALsizei numBytes;

	const char* filepath = fullpath.c_str();
	sndfile = sf_open(filepath, SFM_READ, &sfInfo);

	if (!sndfile) std::cout << "could not open " << fullpath << std::endl;
	if (sfInfo.frames < 1 || sfInfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfInfo.channels)
	{
		std::cout << "bad sample count in " << fullpath << ", " << sfInfo.frames << std::endl;
		return;
	}

	format = AL_NONE;

	switch (sfInfo.channels)
	{
	case 1:
		format = AL_FORMAT_MONO16;
		break;

	case 2:
		format = AL_FORMAT_STEREO16;
		break;

		//there are 2 more cases, see tutorial part 1 for details

	default:
		std::cout << "unsupported channel count: " << sfInfo.channels << std::endl;
		sf_close(sndfile);
		return;
	}

	membuf = static_cast<short*>(malloc((size_t)(sfInfo.frames * sfInfo.channels) * sizeof(short)));

	numFrames = sf_readf_short(sndfile, membuf, sfInfo.frames);
	if (numFrames < 1)
	{
		free(membuf);
		sf_close(sndfile);

		std::cout << "failed to read samples in " << fullpath << ", " << numFrames << std::endl;
		return;
	}

	numBytes = (ALsizei)(numFrames * sfInfo.channels) * (ALsizei)(sizeof(short));
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, membuf, numBytes, sfInfo.samplerate);
	free(membuf);
	sf_close(sndfile);

	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		std::cout << "OpenAL error: " << alGetString(error) << std::endl;

		if (buffer && alIsBuffer(buffer)) alDeleteBuffers(1, &buffer);
		return;
	}

	SoundClip newClip;
	newClip.bufferID = buffer;
	newClip.filepath = filepath;

	this->soundClips.insert(std::make_pair(fullpath, newClip));
}

void SoundBank::DeleteSoundBuffer(std::string filepath)
{
}
