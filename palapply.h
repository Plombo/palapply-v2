#pragma once

#include <stdbool.h>
#include "SDL.h"

bool readPalette(const char *path);
SDL_Surface *readSourceImage(const char *path);
bool saveIndexedPNG(const char *path, SDL_Surface *screen);
bool saveMask(const char* filename, SDL_Surface *screen);
bool needsMask(SDL_Surface *img);


