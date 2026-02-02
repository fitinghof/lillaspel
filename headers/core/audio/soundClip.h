#pragma once
#include "al.h"
#include "alc.h"
#include <string>

struct SoundClip
{
	SoundClip();
	~SoundClip();

    std::string id = "";
	std::string filepath = "";
	ALuint buffer = 0;

    //delete copy constructor to avoid copying
	SoundClip(const SoundClip&) = delete;
	SoundClip& operator=(const SoundClip&) = delete;

    ////Add move constructor, so it can be moved
    //SoundClip(SoundClip&& other) noexcept : filepath(std::move(other.filepath)), buffer(other.buffer)
    //{
    //    other.buffer = 0; // Reset to avoid double-free
    //}

    ////Add move assignment operator
    //SoundClip& operator=(SoundClip&& other) noexcept
    //{
    //    if (this != &other)
    //    {
    //        filepath = std::move(other.filepath);
    //        buffer = other.buffer;
    //        other.buffer = 0;
    //    }
    //    return *this;
    //}
};