#include "../../../headers/core/audio/soundEngine.h"

void SetListenerPosition(float x, float y, float z)
{
	alListener3f(AL_POSITION, x, y, z);

	ALfloat listenerOrient[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
	alListenerfv(AL_ORIENTATION, listenerOrient);
}