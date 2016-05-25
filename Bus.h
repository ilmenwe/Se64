#pragma once

struct bit
{
	bool bit : 1;
};

typedef bit connector;




struct Buss8Bit
{
	connector connector[8];
};

struct Buss16Bit
{
	connector connector[8];
};


union DataBuss8Bit
{
	void setData(unsigned char data) { data = data_; }
	unsigned char data_;
	connector connector_[8];
};

union AddressBuss16Bit
{
	unsigned short data_;
	connector connector_[16];
};
