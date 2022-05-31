#include "text.h"
#include "engine.h"
#include "SDL_ttf.h"
#include <stdio.h>


Text LoadText(const char* path, int size, const char* message, const SDL_Color color) {
	Text retText = { NULL };

	TTF_Font* font = TTF_OpenFont(path, size);
	if (!font) {
		fprintf(stderr, "TTF_OpenFont error: %s\n", SDL_GetError());
		return retText;
	}

	auto surface = TTF_RenderText_Solid(font, message, color);
	retText.text_texture = SDL_CreateTextureFromSurface(gRenderer, surface);
	SDL_FreeSurface(surface);

	return retText;
}

void FreeText(Text retText) {
	if (retText.text_texture)
		SDL_DestroyTexture(retText.text_texture);
}