#include "Platform.h"
#include "CPU.h"
#include <windows.h>
#include <SDL.h>
#pragma once

/*
00 00 00
FF FF FF
68 37 2B
70 A4 B2
6F 3D 86
58 8D 43
35 28 79
B8 C7 6F
6F 4F 25
43 39 00
9A 67 59
44 44 44
6C 6C 6C
9A D2 84
6C 5E B5
95 95 95
*/

class Platform;

class VicII
{

public:

struct screenData
{


};

void Init(Platform* platform);

void Tick()
{

}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		*p = pixel;
		break;

	case 2:
		*(Uint16 *)p = pixel;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32 *)p = pixel;
		break;
	}
}


void render(SDL_Surface* screen, const std::string& characterRom);
private:
	Platform* platform_;

};