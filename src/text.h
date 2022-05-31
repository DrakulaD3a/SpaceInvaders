#pragma once
#include <SDL.h>

typedef struct Text {
	SDL_Texture* text_texture;
} Text;

Text LoadText(const char* path, int size, const char* message, const SDL_Color color);

void FreeText(Text retText);