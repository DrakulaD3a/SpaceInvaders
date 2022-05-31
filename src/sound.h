#pragma once
#include "SDL_mixer.h"
#include "SDL.h"
#define PlaySound(struct) Mix_PlayChannelTimed(struct.channel, struct.chunk, struct.loops, struct.ticks)

typedef struct Sound {
	Mix_Chunk* chunk = NULL;
	int channel = -1;
	int loops = 0;
	int ticks = -1;
} Sound;


Sound LoadSound(const char* path);