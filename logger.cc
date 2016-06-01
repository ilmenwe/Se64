#include "logger.h"
#include <fstream>
#include <sstream>
#include <time.h>
#include <iostream>

std::string LogStrings::log_address = "";
std::string LogStrings::log_op = "";
std::string LogStrings::log_op_name = "";
std::string LogStrings::log_address_style = "";
std::string LogStrings::log_A = "";
std::string LogStrings::log_X = "";
std::string LogStrings::log_Y = "";
std::string LogStrings::log_NVBDI_ZC = "";
std::string LogStrings::log_SP = "";
std::string LogStrings::log_op_address = "";

void LogDate()
{
	/*
	std::ofstream log("log.txt", std::ios::app);
	{
		time_t rawtime(0);
		struct tm timeinfo;

		time_t t = time(0);   // get time now
		localtime_s(&timeinfo,&t);
		log << (timeinfo.tm_year + 1900) << '-'
			<< (timeinfo.tm_mon + 1) << '-'
			<< timeinfo.tm_mday << " " 
			<< timeinfo.tm_hour << ":" << timeinfo.tm_min << ":" << timeinfo.tm_sec << std::endl;


		//localtime_s(&timeinfo, &rawtime);
		//log << timeinfo.tm_year << " " << timeinfo.tm_wday << " " << timeinfo.tm_mday << 
		log.close();
	}
	*/
}


std::ofstream LogStrings::logStream;
void init_log()
{
	LogStrings::logStream = std::ofstream("log.txt", std::ios::app);

}

void persist_log()
{
	LogStrings::logStream.flush();
}

void close_log()
{
	LogStrings::logStream.close();
}

void log()
{
	
	std::stringstream ss;
	

	char opdat[3];
	memset(opdat, '0', 2);
	opdat[2] = '\0';

	if (LogStrings::log_op.length() == 1)
	{
		LogStrings::log_op = "0" + LogStrings::log_op;
	}
	opdat[0] = LogStrings::log_op[0];
	opdat[1] = LogStrings::log_op[1];



	ss << LogStrings::log_address << "  ";
	ss << opdat << "        ";
	ss << LogStrings::log_op_name << "    ";
	ss << LogStrings::log_op_address << " ";
	ss << LogStrings::log_address_style << "                        ";
	ss << LogStrings::log_A << " ";
	ss << LogStrings::log_X << " ";
	ss << LogStrings::log_Y << " ";
	ss << LogStrings::log_NVBDI_ZC << " ";
	ss << LogStrings::log_SP << "";
	ss << std::endl;

	std::string logstr = ss.str();
	for (int i = 0; i < logstr.length(); i++)
	{
		
		logstr[i] = toupper(logstr[i]);
	}
	//std::cout << logstr << std::endl;
	LogStrings::logStream << logstr;
	LogStrings::logStream.flush();
	
}

void LogRam(unsigned char* ram)
{
	/*
	std::string out;
	out.resize(1024 * 64);

	memcpy((void*)out.c_str(), ram, 1024 * 64);
	Log(out);
	std::ofstream log("ramDump.txt", std::ios::app);
	log << out << std::endl;
	log.close();
	*/
}

void LogIO(std::string data)
{
	/*
	std::ofstream log("logIO.txt", std::ios::app);
	log << data << std::endl;
	log.flush();
	*/
}
void Log(std::string data)
{
	
	//std::ofstream log("log.txt", std::ios::app);
	LogStrings::logStream << data << std::endl;

}