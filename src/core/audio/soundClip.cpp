#include "core/audio/soundClip.h"

SoundClip::SoundClip()
{
}

SoundClip::~SoundClip()
{
    if (buffer != 0)
    {
        alDeleteBuffers(1, &this->buffer);
    }
}