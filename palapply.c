/*
 * Copyright (c) 2018-2019 Bryan Cain
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

// Converts an RGBA png to an indexed image using a given palette, and creates an alpha
// mask if needed.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <png.h>
#include <zlib.h>
#include "SDL_image.h"
#include "palapply.h"

typedef struct {
    uint32_t w;
    uint32_t h;
    bool hasAlphaChannel;
    uint32_t *pixels;
} Image32;

png_color pal[256];
int pal_ncolors; // number of colors in palette (1-256)

bool readPalette(const char *path)
{
    SDL_Surface *image = IMG_Load(path);
    if (!image)
    {
        printf("Error: %s\n", SDL_GetError());
        return false;
    }
    else if (!image->format->palette)
    {
        printf("Error: palette image %s is not indexed\n", path);
        SDL_FreeSurface(image);
        return false;
    }
    else
    {
        pal_ncolors = image->format->palette->ncolors;
        for (int i = 0; i < pal_ncolors; i++)
        {
            pal[i].red = image->format->palette->colors[i].r;
            pal[i].green = image->format->palette->colors[i].g;
            pal[i].blue = image->format->palette->colors[i].b;
        }
        printf("read palette with %i colors from %s\n", pal_ncolors, path);
        SDL_FreeSurface(image);
        return true;
    }
}

SDL_Surface *readSourceImage(const char *path)
{
    SDL_Surface *image = IMG_Load(path);
    if (!image)
    {
        printf("Error: %s\n", SDL_GetError());
        return NULL;
    }

    if (image->format->Amask)
    {
        printf("has alpha channel\n");
    }
    else
    {
        printf("no alpha channel\n");
    }

    SDL_Surface *image32 = SDL_CreateRGBSurface(0, image->w, image->h, 32,
            0xFF, 0xFF00, 0xFF0000, image->format->Amask ? 0xFF000000 : 0);
    SDL_BlitSurface(image, NULL, image32, NULL);
    SDL_FreeSurface(image);

    // FIXME: this will break true color RGBA PNGs with alpha channels that are all 0 or 255
    if (image32->format->Amask && !needsMask(image32))
    {
        image32->format->Amask = 0;
    }

    return image32;
}

// saves image as indexed PNG using nearest-color algorithm
bool saveIndexedPNG(const char *path, SDL_Surface *screen)
{
    uint32_t *source;
    int i, x, y;
    png_structp png_ptr;
    png_infop info_ptr;
    FILE *fp;
    uint8_t *line;

    fp = fopen(path, "wb");
    if (!fp) return false;
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return false;
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        fclose(fp);
        return false;
    }

    png_init_io(png_ptr, fp);
    png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
    png_set_IHDR(png_ptr, info_ptr, screen->w, screen->h,
                 8, PNG_COLOR_TYPE_PALETTE, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_set_PLTE(png_ptr, info_ptr, pal, pal_ncolors);
    png_write_info(png_ptr, info_ptr);
    line = (uint8_t*) malloc(screen->w);

    source = screen->pixels;

    for (y = 0; y < screen->h; y++)
    {
        source = (uint32_t *)(screen->pixels + (y * screen->pitch));
        for (i = 0, x = 0; x < screen->w; x++)
        {
            uint32_t color = 0;
            uint8_t r = 0, g = 0, b = 0, a = 0, nearest = 1;

            color = source[x];
            r = color & 0xff;
            g = (color >> 8) & 0xff;
            b = (color >> 16) & 0xff;
            a = (color >> 24) & 0xff;

            if (screen->format->Amask && a == 0) nearest = 0;
            else
            {
                int j;
                int nearest_dist_sq = 9999999;
                /* If the source has an alpha mask, don't use the transparent color (0) for any
                 * pixels that aren't completely transparent. */
                for (j = screen->format->Amask ? 1 : 0; j < pal_ncolors; j++)
                {
                    int rdist = r - pal[j].red;
                    int gdist = g - pal[j].green;
                    int bdist = b - pal[j].blue;
                    int dist_sq = rdist*rdist + gdist*gdist + bdist*bdist;
                    if (dist_sq < nearest_dist_sq)
                    {
                        nearest_dist_sq = dist_sq;
                        nearest = j;
                    }
                }
            }

            line[i++] = nearest;
        }
        png_write_row(png_ptr, line);
    }
    free(line);
    line = NULL;
    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    return true;
}

// saves alpha mask of image
bool saveMask(const char* filename, SDL_Surface *screen)
{
    uint32_t *source;
    int i, x, y;
    png_structp png_ptr;
    png_infop info_ptr;
    FILE *fp;
    uint8_t *line;

    fp = fopen(filename, "wb");
    if (!fp) return false;
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return false;
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        fclose(fp);
        return false;
    }
    png_init_io(png_ptr, fp);
    png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
    png_set_IHDR(png_ptr, info_ptr, screen->w, screen->h,
                 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);
    line = (uint8_t*) malloc(screen->w);


    for (y = 0; y < screen->h; y++)
    {
        source = (uint32_t *)(screen->pixels + (y * screen->pitch));
        for (i = 0, x = 0; x < screen->w; x++)
        {
            uint32_t color = source[x];
            uint8_t a = (color >> 24) & 0xff;
            line[i++] = a;
        }
        png_write_row(png_ptr, line);
    }
    free(line);
    line = NULL;
    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    return true;
}

// returns true if and only if alpha channel of img has at least one alpha value that isn't 0 or 255
bool needsMask(SDL_Surface *img)
{
    uint32_t x, y, *color;
    for (y = 0; y < img->h; y++)
    {
        color = img->pixels + (y * img->pitch);
        for (x = 0; x < img->w; x++)
        {
            uint32_t alpha = ((*color) >> 24) & 0xff;
            assert(alpha >= 0 && alpha <= 255);
            if (alpha != 0 && alpha != 255) return true;
            color++;
        }
    }

    return false;
}

int commandLineMain(int argc, char **argv)
{
    if (argc != 4 && argc != 5) // alpha masking is optional
    {
        fprintf(stderr, "Usage: %s palette source result [result_mask]\n", argv[0]);
        fprintf(stderr, "\n");
        fprintf(stderr, "palette: an indexed PNG with the target palette\n");
        fprintf(stderr, "source: the PNG to apply the palette to and generate the mask from;\n"
                        "        can be RGB, indexed, or grayscale, with or without alpha\n");
        fprintf(stderr, "result: path to which to save the resulting image as an indexed PNG\n");
        fprintf(stderr, "result_mask: path to which to save the resulting alpha mask as a grayscale PNG\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "The result_mask parameter can be omitted to skip producing an alpha mask.\n");
        fprintf(stderr, "Note that result and result_mask will be overwritten if the paths already exist.\n");
        return 1;
    }

    if (!readPalette(argv[1]))
    {
        fprintf(stderr, "error: failed to load palette image '%s'\n", argv[1]);
        goto error;
    }

    SDL_Surface *img = readSourceImage(argv[2]);
    if (img)
    {
        printf("read image %s\n", argv[2]);
    }
    else
    {
        fprintf(stderr, "error: failed to load image %s\n", argv[2]);
        goto error;
    }

    if (!saveIndexedPNG(argv[3], img))
    {
        fprintf(stderr, "error: failed to save result '%s'\n", argv[3]);
        goto error;
    } else printf("saved result to '%s'\n", argv[3]);

    if (img->format->Amask)
    {
        if (needsMask(img))
        {
            if (argc < 5)
            {
                fprintf(stderr, "warning: source has non-trivial alpha, but no mask filename given");
            }
            else if (!saveMask(argv[4], img))
            {
                fprintf(stderr, "error: failed to save alpha mask '%s'\n", argv[4]);
                goto error;
            }
            else printf("saved alpha mask to '%s'\n", argv[4]);
        }
        else printf("no alpha mask needed (simple alpha channel)\n");
    } else printf("no alpha mask needed (source has no alpha channel)\n");

    SDL_FreeSurface(img);

    return 0;

error:
    return 1;
}

