#include "platform.h"
#include "CPU.h"
#include "VicII.h"
#include "BitsAndBytes.h"
#include <sstream>
#include "logger.h"
#include "intToStr.h"
#include <fstream>

Platform::Platform()
{
	tick_time_left_ = 0;
	hz_ = 100;
	cpu_ = new CPU(this);
	vic_II_ = new VicII();
	vic_II_->Init(this);

	init_log();

}

void Platform::update(float dt)
{



	/*tick_time_left_ -= dt;
	if (tick_time_left_ < 0.0f)
	{
		tick_time_left_ += hz_;*/
		tick();
	/*}*/

}


unsigned char Platform::Read(unsigned short address)
{
	BitsAndByte bbyte;
	bbyte.byte = ram[0x01];
	/*
	Log("READ");
	if (address >= 0x0000 && address <= 0x00FF)
	{
		Log("Page 0 Zeropage addressing");
	}
	if (address >= 0x0100 && address <= 0x01FF)
	{
		Log("Page 2 Enhanced Zeropage contains the stack");
	}
	if (address >= 0x0200 && address <= 0x02FF)
	{
		Log("Page 2 Operating System and BASIC pointers");
	}
	if (address >= 0x0300 && address <= 0x03FF)
	{
		Log("Page 3 Operating System and BASIC pointers");
	}
	if (address >= 0x0400 && address <= 0x07FF)
	{
		Log("Page 4-7 Screen Memory");
	}
	if (address >= 0x0800 && address <= 0x9FFF)
	{
		Log("Page 8-159 Free BASIC program storage area (38911 bytes)");
	}
	if (address >= 0xA000 && address <= 0xBFFF)
	{
		Log("Page 160-191 Free machine language program storage area (when switched-out with ROM)");
	}
	if (address >= 0xC000 && address <= 0xCFFF)
	{
		Log("Page 192-207 Free machine language program storage area");
	}
	if (address >= 0xD000 && address <= 0xD3FF)
	{
		Log("Page 208-211");
	}
	if (address >= 0xd400 && address <= 0xd4FF)
	{
		Log("Page 212-215");
	}
	if (address >= 0xD800 && address <= 0xD8FF)
	{
		Log("Page 216-219");
	}
	if (address >= 0xDC00 && address <= 0xDCFF)
	{
		Log("Page 220");
	}
	if (address >= 0xDD00 && address <= 0xDDFF)
	{
		Log("Page 221");
	}
	if (address >= 0xDE00 && address <= 0xDFFF)
	{
		Log("Page 222-223 Reserved for interface extensions");
	}
	if (address >= 0xE000 && address <= 0xFFFF)
	{
		Log("Page 224-255 Free machine language program storage area(when switched - out with ROM)");
	}
	*/
	
	unsigned char data = 0;
	if (address >= 0xA000 && address <= 0xBFFF)
	{
		bool breakhere = true;
	}

	if (address >= 0xA000 && address <= 0xBFFF && bbyte.zero)
	{
		data = basicRom[address - 0xA000];
	}
	else if (address >= 0xE000 && address <= 0xFFFF && bbyte.one)
	{
		unsigned shortRel = address - 0xE000;
		data = kernelRom[shortRel];
	}
	else if (address >= 0xDC00 && address <= 0xDC0F)
	{
		//Log("NON IMPLEMTEND CIA 1 STUFF!!!!");
		data = 0xFF;

	}
	else if (address >= 0xDD00 && address <= 0xDD0F)
	{
		//Log("NON IMPLEMTEND CIA 2 STUFF!!!!");
		data = 0xFF;
	}
	else
	{
		data = ram[address];
	}
	if (address == 0xd012)
	{
		data = 0;
	}


	LogIO("Read from address " + intToStr(address) + " got data: " + intToStr(data));
	if (address > 0x03ff && address < 0x0400 + (40 * 25))
	{
		bool breakhere = true;
	}

	return data;
}


void Platform::Write(unsigned short address, unsigned char data)
{
	LogIO("Write to address " + intToStr(address) + " with data: " + intToStr(data));
	if (address == 0x0400)
	{
		bool breakhere = true;
	}
	/*
	Log("WRITE");
	if (address >= 0x0000 && address <= 0x00FF)
	{
		Log("Page 0 Zeropage addressing");
	}
	if (address >= 0x0100 && address <= 0x01FF)
	{
		Log("Page 2 Enhanced Zeropage contains the stack");
	}
	if (address >= 0x0200 && address <= 0x02FF)
	{
		Log("Page 2 Operating System and BASIC pointers");
	}
	if (address >= 0x0300 && address <= 0x03FF)
	{
		Log("Page 3 Operating System and BASIC pointers");
	}
	if (address >= 0x0400 && address <= 0x07FF)
	{
		Log("Page 4-7 Screen Memory");
	}
	if (address >= 0x0800 && address <= 0x9FFF)
	{
		Log("Page 8-159 Free BASIC program storage area (38911 bytes)");
	}
	if (address >= 0xA000 && address <= 0xBFFF)
	{
		Log("Page 160-191 Free machine language program storage area (when switched-out with ROM)");
	}
	if (address >= 0xC000 && address <= 0xCFFF)
	{
		Log("Page 192-207 Free machine language program storage area");
	}
	if (address >= 0xD000 && address <= 0xD3FF)
	{
		Log("Page 208-211");
	}
	if (address >= 0xd400 && address <= 0xd4FF)
	{
		Log("Page 212-215");
	}
	if (address >= 0xD800 && address <= 0xD8FF)
	{
		Log("Page 216-219");
	}
	if (address >= 0xDC00 && address <= 0xDCFF)
	{
		Log("Page 220");
	}
	if (address >= 0xDD00 && address <= 0xDDFF)
	{
		Log("Page 221");
	}
	if (address >= 0xDE00 && address <= 0xDFFF)
	{
		Log("Page 222-223 Reserved for interface extensions");
	}
	if (address >= 0xE000 && address <= 0xFFFF)
	{
		Log("Page 224-255 Free machine language program storage area(when switched - out with ROM)");
	}
	
	Log("Write to address " + intToStr(address) + " with data: " + intToStr(data));
	*/
	ram[address] = data;
}


void Platform::Reset()
{
	memset(ram, 0xffffffff, 64 * 1024);
	ram[0xd012] = 0x00;
	
	cpu_->Reset();
	LogRam(ram);
}

void Platform::dumpRam()
{
	std::ofstream ramDump("ram.bin");
	ramDump.write((char*)ram, 65536);

}
void Platform::dumpVicRam()
{
	std::ofstream ramDump("vicram.bin");
	char vicRam[0x07FF - 0x0400];
	memcpy(vicRam, &ram[0x0400], 0x07FF - 0x0400);
	ramDump.write((char*)vicRam, 0x07FF - 0x0400);

}

void Platform::tick()
{
	cpu_->Tick();
	vic_II_->Tick();

}
void Platform::PLA()
{

}
