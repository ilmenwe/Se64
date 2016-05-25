#pragma once

union BitsAndByte
{
	struct {
	unsigned char zero : 1;
	unsigned char one : 1;
	unsigned char two : 1;
	unsigned char three : 1;
	unsigned char four : 1;
	unsigned char five : 1;
	unsigned char six : 1;
	unsigned char seven : 1;
	};
	unsigned char byte;
};
