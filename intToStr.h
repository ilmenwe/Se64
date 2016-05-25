#pragma once

#include <string>
#include <sstream>

static std::string intToStr(int data)
{
	std::stringstream ss;
	ss << std::hex << data;
	return  ss.str();
}