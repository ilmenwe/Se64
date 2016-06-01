#pragma once
#include <string>
//#include "CPU.h"
class VicII;
class CPU;
class Platform
{
public:
	Platform();
	void update( float dt);
	void tick();
	void PLA();
	void videoOut();
	unsigned char Read(unsigned short address);
	void Write(unsigned short address, unsigned char data);
	void Reset();
	bool is_writing() { return writing_; }
	bool set_writing(bool writing) { writing_ = writing; }
	void dumpRam();
	void dumpVicRam();
	std::string AVJack();

	VicII* getVicII() { return vic_II_; }

	void putIntoRoms(std::string kernelRom, std::string charRom, std::string basicRom)
	{
		for (unsigned int i = 0; i < charRom.length(); i++)
		{
			this->charRom[i] = charRom[i];
		}
		for (unsigned int i = 0; i < kernelRom.length(); i++)
		{
			this->basicRom[i] = basicRom[i];
			this->kernelRom[i] = kernelRom[i];
		}
	}
	unsigned char* getRam() { return &ram[0]; };


	void putIntoRam(unsigned short lo, unsigned short hi, std::string data)
	{
		unsigned short address = lo + (hi << 8);
		for (unsigned int i = 2; i < data.length(); i++)
		{
			ram[(i - 2) + address] = data[i];
		}
	}



	void set_CharEn(unsigned char value) { CharEn_ = value; }
	void set_HiRam(unsigned char value) { HiRam_ = value; }
	void set_LoRam(unsigned char value) { LoRam_ = value; }

	unsigned char CharEn() { return CharEn_;}
	unsigned char HiRam() { return HiRam_; }
	unsigned char LoRam() { return LoRam_; }


private:


	unsigned char CharEn_;
	unsigned char HiRam_;
	unsigned char LoRam_;

	unsigned char charRom[1024 * 8];
	unsigned char kernelRom[1024 * 8];
	unsigned char basicRom[1024 * 8];


	unsigned char ram[1024 * 64];
	CPU* cpu_;
	VicII* vic_II_;
	unsigned char data_;
	unsigned short address_;
	bool writing_;
	float hz_;
	float tick_time_left_;

};