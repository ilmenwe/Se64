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


void render(SDL_Surface* screen, const std::string& characterRom)
{

	
	union
	{
		struct
		{
			char R;
			char G;
			char B;
			char A;
		};
		int color;
	} pixel;

	
	UINT32 foreground = SDL_MapRGB(screen->format, 0xff, 0xff, 0xFF); //RGB(0xFF, 0xFF, 0xFF);
	UINT32 backGround = SDL_MapRGB(screen->format, 0x35, 0x28, 0x79); //RGB(0x35, 0x28, 0x79);
	UINT32 colorOut = foreground;

	int sqrtVal = sqrt(1024 * 64);

	/*
	for (int y = 0; y < 200; y++)
	{
		for (int x = 0; x < 320; x++)
		{
			framebuf[x + y * 320] = rand();
		}
	}
	*/

	/*
	for (int y = 0; y < 200; y++)
	{
		for (int x = 0; x < 256; x++)
		{
			//unsigned char clrv = (platform_->getRam()[(x) + (y) *sqrtVal]);
			//UINT32 clr = RGB(0xff- clrv, 0xff, 0xff);

			//putpixel(screen, x, y, clr);
			//framebuf[x + y * 320] = 256*256;
			//COLORREF clr = RGB(clrv >> 16, clrv >> 8, 0xFF);
			//SetPixel(hdc, (x) , 200 + y , clr);
		}
	}
	*/

	
	int i = 0;
	for (int y = 0; y < 25; y++)
	{
		for (int x = 0;  x < 40; x++)
		{
			for (int cy = 0; cy < 8; cy++)
			{ 
				unsigned short ramAddress = 0x0400;
				unsigned char fontRow = characterRom[cy + (platform_->getRam()[(x + (y * 40))] * 8)];
				for (int cx = 0; cx < 8; cx++)
				{
					bool result = (fontRow >> cx)&0x01;
					colorOut = backGround;
					if (result)
					{
						colorOut = foreground;
					}
					putpixel(screen, (x * 8) + (8 - cx)+64, (y * 8) + cy + 64, colorOut);
					//framebuf[x*8, (y*8) ] = colorOut;
					//pixel.color = framebuf[x*cx, y*cy];
					//BitBlt(hdc, 200 + (x * 8) + (8 - cx), 200 + (y * 8) + cy, colorOut);
					//SetPixel(hdc,, , colorOut);
				}
				i++;
			}
		}
	}
	
	

}
private:
	Platform* platform_;

};