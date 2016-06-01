#include <Windows.h>
#include <SDL.h>
#include <SDL_thread.h>
#include "platform.h"
#include <string>
#include <sstream>
#include <iostream>
#include <SDL.h>
#include "VicII.h"
#undef main


std::string fileToString(const std::string& filename)
{
	FILE *f;
	fopen_s(&f, filename.c_str(), "rb");
	fseek(f, 0, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  //same as rewind(f);

	std::string data;
	data.resize(fsize);
	//char *string = malloc(fsize + 1);
	fread((void*)data.c_str(), fsize, 1, f);
	fclose(f);

	return data;
}




Platform platform;
std::string charRom;

void init()
{

	std::string data = fileToString("1942");
	charRom = fileToString("char.rom");
	std::string kernelRom = fileToString("kernal.rom");
	std::string basicRom = fileToString("basic.rom");

	//platform.putIntoRam(data[0], data[1], data);
	platform.putIntoRoms(kernelRom, charRom, basicRom);
	platform.Reset();
	/*SetTimer(hWnd,             // handle to main window
	IDT_TIMER1,            // timer identifier
	1,                 // 10-second interval
	(TIMERPROC)NULL);     // no timer callback
	*/


}


int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Surface* screen;

	screen  =  SDL_SetVideoMode(640, 400, 24, SDL_SWSURFACE | SDL_ANYFORMAT);
	init();
	SDL_Event event;
	bool loop = true;
	int tickCount = 0;
	while(loop)
	{ 

		/* Update just the part of the display that we've changed */

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				/* handle keyboard stuff here */
				break;

			case SDL_QUIT:
				/* Set whatever flags are necessary to */
				/* end the main game loop here */
				loop = false;
				break;
			}	
		}

		unsigned char val = 0xe4 - 0xa4;

		Uint8* keys;
		keys = SDL_GetKeyState(NULL);
		
		if(keys[SDLK_F7] == 1)
		{
			platform.dumpRam();
		}
		if (keys[SDLK_SPACE] == 1)
		{
			platform.dumpVicRam();
		}
		platform.update(1);
		
		if(tickCount % 19656 == 0)
		{ 
			if (SDL_MUSTLOCK(screen)) {
				if (SDL_LockSurface(screen) < 0) {
					fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
					return 0;
				}
			}
			platform.getVicII()->render(screen, charRom);

			if (SDL_MUSTLOCK(screen)) {
				SDL_UnlockSurface(screen);
			}
			SDL_UpdateRect(screen, 0, 0, 0, 0);

		}
		tickCount++;

		//std::stringstream ss;
		//ss << tickCount;

		//std::cout << ss.str() << std::endl;
		//SDL_WM_SetCaption(, 0);

		//tickCount++;
		//std::stringstream ss;
		//ss << tickCount;

	}
	return 0;
}


/*void main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
}
*/

/*
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow)
{
	SDL_Init(SDL_INIT_EVERYTHING);


	return 0;
}

*/

/*
int SDL_main(int argc, char *argv[])
{

	return 0;
}

*/
