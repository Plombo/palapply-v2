#pragma once

#include <stdbool.h>
#include "SDL.h"

typedef enum {
    ALPHA_NONE,        // alpha channel is all 255
    ALPHA_SIMPLE,      // alpha channel is all 0 or 255
    ALPHA_MASK_NEEDED, // alpha channel has values that are not 0 or 255
} AlphaType;

bool readPalette(const char *path);
SDL_Surface *readSourceImage(const char *path);
bool saveIndexedPNG(const char *path, SDL_Surface *screen);
bool saveMask(const char* filename, SDL_Surface *screen);
AlphaType alphaType(SDL_Surface *img);


