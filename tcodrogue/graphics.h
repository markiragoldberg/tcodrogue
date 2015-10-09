#pragma once

#include "libtcod.hpp"
#include <SDL.h>
#include <SDL_image.h>

class MyRenderer : public ITCODSDLRenderer {
public:
	void render(void *sdlSurface) {
		SDL_Surface *surface = (SDL_Surface *)sdlSurface;
		//... draw something on s
		SDL_Surface *person32 = SDL_LoadBMP("32x32 person.png");

		//mark updated regions as needing update
		//void TCODConsole::setDirty(int x, int y, int w, int h)
	}
};
//TCODSystem::registerSDLRenderer(new MyRenderer());
