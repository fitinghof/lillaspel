#include "core/audio/soundClip.h"

SoundClip::SoundClip()
{
}

SoundClip::~SoundClip()
{
    if (buffer != 0)
    {
        alDeleteBuffers(1, &this->buffer); //may move to soundBank later since soundBank already initializes the memory creation for a clip
    }
}