#pragma once
#include <string>

#define MACROSTR(k) #k

class LogStrings
{
public:
	LogStrings()
	{


	}

static std::ofstream logStream;
static std::string log_address;
static std::string log_op_address;
static std::string log_op;
static std::string log_op_name;
static std::string log_address_style;
static std::string log_A;
static std::string log_X;
static std::string log_Y;
static std::string log_NVBDI_ZC;
static std::string log_SP;
};

void log();

void LogDate();
void init_log();
void persist_log();
void close_log();
void LogRam(unsigned char* ram);
void Log(std::string data);
void LogIO(std::string data);