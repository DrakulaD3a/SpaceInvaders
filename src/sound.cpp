#include "sound.h"
#include "engine.h"
#include "SDL_mixer.h"
#include "SDL.h"
#include <stdio.h>


Sound LoadSound(const char* path)
{
	Sound retSound;

	Mix_Chunk* chunk = Mix_LoadWAV_RW(SDL_RWFromFile(path, "rb"), 1);

	if (chunk == NULL)
	{
		fprintf(stderr, "SDL_mixer error: %s\n", SDL_GetError());
		return retSound;
	}

	retSound.chunk = chunk;


	return retSound;
}