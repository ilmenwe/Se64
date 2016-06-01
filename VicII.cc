#include "VicII.h"


void VicII::Init(Platform* platform)
{
	platform_ = platform;
}

void VicII::readGlyph(Glyph& glyphOut, const std::string &characterRom, int id )
{
	unsigned char* startIndex = platform_->getRam(); 
	for (int r = 0; r < 8; r++)
	{
		unsigned short characterIndex = 0x0400 + id;
		char row = characterRom[startIndex[characterIndex]];
		glyphOut.row[r] = row;
	}

}


void VicII::renderGlyph(SDL_Surface * screen, Glyph&g, int x, int y)
{
	UINT32 foreground = SDL_MapRGB(screen->format, 0xff, 0xff, 0xFF); //RGB(0xFF, 0xFF, 0xFF);
	UINT32 backGround = SDL_MapRGB(screen->format, 0x35, 0x28, 0x79); //RGB(0x35, 0x28, 0x79);
	UINT32 colorOut = foreground;
	for (int gy = 0; gy < 8; gy++)
	{
		unsigned char rowData = g.row[gy];
		for (int gx = 0; gx < 8; gx++)
		{
			bool result = ((rowData >> gx) & 0x1);
			colorOut = backGround;
			if (result)
			{
				colorOut = foreground;
			}


			putpixel(screen, (x*8)+gx, (y * 8) +gy, colorOut);

		}

	}
}

void VicII::render(SDL_Surface * screen, const std::string & characterRom)
{
	Glyph g;
	for (int y = 0; y < 25; y++)
	{
		for(int x = 0; x < 40; x++)
		{
			readGlyph(g, characterRom, x + (y * 40));
			renderGlyph(screen, g, x, y);
		}
	}
}
