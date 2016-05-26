#include "VicII.h"


void VicII::Init(Platform* platform)
{
	platform_ = platform;
}

void VicII::render(SDL_Surface * screen, const std::string & characterRom)
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


	for (int y = 0; y < 25; y++)
	{
		for(int x = 0; x < 40; x++)
		{
			for (int cy = 0; cy < 8; cy++)
			{
				int ramIndex = ((x + (y * 40)) + 0x0400)+cy*8;
				unsigned char ramVal = platform_->getRam()[ramIndex];
				unsigned char charRow = characterRom[ ramVal ];
				for(int cx = 0; cx < 8; cx++ )
				{
					bool result = (charRow >> cx) & 0x01;
					colorOut = backGround;
					if (result)
					{
						colorOut = foreground;
					}
					
					int destX = (x * 8)+cx;
					int destY = (y * 8)+cy;

					putpixel(screen, destX,destY, colorOut);
				}
				
			}
		}
	}

/*	for (unsigned short i = 0x0400; i < 0x0800; i++)
	{
		
		unsigned char glyph = platform_->getRam()[i];
		int y = (1.0 / i)*(40 * 8);
		int x = (i - 0x400) % 25;
		for (int cx = 0; cx < 8; cx++)
		{
			bool result = (glyph >> cx) & 0x01;
			colorOut = backGround;
			if (result)
			{
				colorOut = foreground;
			}
			putpixel(screen, (x + cx), y, colorOut);
		}
	}
	*/
	/*
	for (int y = 0; y < 25; y++)
	{
		for (int x = 0; x < 40; x++)
		{
			for (int cy = 0; cy < 8; cy++)
			{

				unsigned short ramAddress = 0x0400;
				unsigned char fontRow = characterRom[(cy + (platform_->getRam()[(x + (y * 40)) + 0x0400] * 8))];
				for (int cx = 0; cx < 8; cx++)
				{
					bool result = (fontRow >> cx) & 0x01;
					colorOut = backGround;
					if (result)
					{
						colorOut = foreground;
					}
					putpixel(screen, (x * 8) + (8 - cx) + 64, (y * 8) + cy + 64, colorOut);
					//framebuf[x*8, (y*8) ] = colorOut;
					//pixel.color = framebuf[x*cx, y*cy];
					//BitBlt(hdc, 200 + (x * 8) + (8 - cx), 200 + (y * 8) + cy, colorOut);
					//SetPixel(hdc,, , colorOut);
				}
				i++;
			}
		}
	}*/



}
