#include "../../../headers/core/audio/musicTrack.h"

MusicTrack::MusicTrack()
{
	this->audioInstruction.fadeIn = false; //will look at this later
	this->audioInstruction.fadeInTime = 0;
	this->audioInstruction.fadeOut = false;
	this->audioInstruction.fadeOutTime = 0;
	this->audioInstruction.loopSound = false;
	this->audioInstruction.loopNrOfTimes = 0;
}

MusicTrack::~MusicTrack()
{
	alDeleteSources(1, &this->source);

	if (this->sndfile)
	{
		sf_close(this->sndfile);
		this->sndfile = nullptr;
	}

	free(this->membuf);
	alDeleteBuffers(NUM_BUFFERS, this->buffers);
}

void MusicTrack::Initialize(std::string pathToMusicFolder)
{
	this->pathToMusicFolder = pathToMusicFolder;
}

void MusicTrack::Play()
{
	alGetError();
	alSourceRewind(this->source);
	alSourcei(this->source, AL_BUFFER, 0);

	for (int i = 0; i < NUM_BUFFERS; i++)
	{
		sf_count_t slen = sf_readf_short(this->sndfile, this->membuf, BUFFER_SAMPLES);
		if (slen < 1) break;

		slen *= this->sfInfo.channels * (sf_count_t)sizeof(short);
		alBufferData(this->buffers[i], this->format, this->membuf, (ALsizei)slen, this->sfInfo.samplerate);
	}

	if (alGetError() != AL_NO_ERROR)
	{
		Logger::Error("error buffering for playback");
		return;
	}

	alSourceQueueBuffers(this->source, NUM_BUFFERS - 1, this->buffers);
	alSourcePlay(this->source);

	if (alGetError() != AL_NO_ERROR)
	{
		Logger::Error("error buffering for playback");
	}
}

void MusicTrack::LoadTrackStandardFolder(std::string filename)
{
	alGenSources(1, &this->source);
	alGenBuffers(NUM_BUFFERS, this->buffers);

	alSourcef(this->source, AL_PITCH, this->pitch);
	alSourcef(this->source, AL_GAIN, this->gain);
	alSource3f(this->source, AL_POSITION, this->position[0], this->position[1], this->position[2]);
	alSource3f(this->source, AL_VELOCITY, this->velocity[0], this->velocity[1], this->velocity[2]);
	alSourcei(this->source, AL_LOOPING, this->audioInstruction.loopSound);

	std::string fullpath = this->pathToMusicFolder + filename;
	const char* file = fullpath.c_str();
	sndfile = sf_open(file, SFM_READ, &this->sfInfo);

	if (!sndfile)
	{
		Logger::Error("could not open " + fullpath);
		return;
	}

	this->format = AL_NONE;

	switch (sfInfo.channels)
	{
	case 1:
		this->format = AL_FORMAT_MONO16;
		break;

	case 2:
		this->format = AL_FORMAT_STEREO16;
		break;

		//there are 2 more cases, see tutorial part 1 for details

	default:
		//std::cout << "unsupported channel count: " << sfInfo.channels << std::endl;
		Logger::Error("unsupported channel count: ");
		sf_close(sndfile);
		return;
	}

	std::size_t frameSize = ((size_t)BUFFER_SAMPLES * (size_t)this->sfInfo.channels) * sizeof(short);
	this->membuf = static_cast<short*>(malloc(frameSize));

	//numFrames = sf_readf_short(sndfile, membuf, sfInfo.frames);
	//if (numFrames < 1)
	//{
	//	free(membuf);
	//	sf_close(sndfile);

	//	std::cout << "failed to read samples in " << fullpath << ", " << numFrames << std::endl;
	//	return;
	//}

	//numBytes = (ALsizei)(numFrames * sfInfo.channels) * (ALsizei)(sizeof(short));
	//alGenBuffers(1, &buffer);
	//alBufferData(buffer, format, membuf, numBytes, sfInfo.samplerate);
	//free(membuf);
	//sf_close(sndfile);

	//error = alGetError();
	//if (error != AL_NO_ERROR)
	//{
	//	std::cout << "OpenAL error: " << alGetString(error) << std::endl;

	//	if (buffer && alIsBuffer(buffer)) alDeleteBuffers(1, &buffer);
	//	return;
	//}

	//this->soundBuffers.insert(std::make_pair(fullpath, buffer));
}

void MusicTrack::SetPitch(float pitch)
{
	this->pitch = pitch;
	alSourcef(this->source, AL_PITCH, this->pitch);
}

void MusicTrack::SetAudioInstruction(AudioInstruction audioInstruction)
{
	this->audioInstruction = audioInstruction;
	alSourcei(this->source, AL_LOOPING, this->audioInstruction.loopSound);
	//add more stuff later
}

void MusicTrack::UpdateBufferStream()
{
	ALint processed, state;

	alGetError();
	alGetSourcei(this->source, AL_SOURCE_STATE, &state);
	alGetSourcei(this->source, AL_BUFFERS_PROCESSED, &processed);

	if (alGetError() != AL_NO_ERROR)
	{
		Logger::Error("error  checking music source state");
	}

	while (processed > 0)
	{
		ALuint bufferID;
		sf_count_t slen;

		alSourceUnqueueBuffers(this->source, 1, &bufferID);
		processed--;

		//Read next chunk of data and refill the buffer, and then queue it
		slen = sf_readf_short(this->sndfile, this->membuf, BUFFER_SAMPLES);
		if (slen > 0)
		{
			slen *= this->sfInfo.channels * (sf_count_t)sizeof(short);
			alBufferData(bufferID, this->format, this->membuf, (ALsizei)slen, this->sfInfo.samplerate);
			alSourceQueueBuffers(this->source, 1, &bufferID);
		}

		if (alGetError() != AL_NO_ERROR)
		{
			Logger::Error("error buffering music data while streaming");
			return;
		}
	}

	//make sure buffers hasn't underrun
	if (state != AL_PLAYING && state != AL_PAUSED)
	{
		ALint queued;

		//if no buffers are queued, playback is finished
		alGetSourcei(this->source, AL_BUFFERS_QUEUED, &queued);

		if (queued == 0) return;

		alSourcePlay(this->source);
		if (alGetError() != AL_NO_ERROR)
		{
			Logger::Error("error restarting music");
		}
	}
}
