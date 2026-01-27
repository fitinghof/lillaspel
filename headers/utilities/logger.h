#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

// Just the basic stuff, someone rewrite for more functionality

#define RESET_COLOR	"\033[0m"
#define YELLOW	"\033[33m"
#define RED		"\033[31m" 

enum LogType
{
	consoleAndLogFile,
	consoleOnly,
	logFileOnly
};

class Logger {
public:
	static void Log(std::string info, LogType logType = LogType::consoleAndLogFile) {

		switch (logType)
		{
		case LogType::consoleAndLogFile:
			std::cout << info << std::endl;
			PrintToFile(info);
			break;
		case LogType::consoleOnly:
			std::cout << info << std::endl;
			break;
		case LogType::logFileOnly:
			PrintToFile(info);
			break;
		default:
			break;
		}

		
	}

	static void Warn(std::string info, LogType logType = LogType::consoleAndLogFile) {

		switch (logType)
		{
		case LogType::consoleAndLogFile:
			std::cout << YELLOW << "[WARNING] " << info << RESET_COLOR << std::endl;
			PrintToFile("[WARNING] " + info);
			break;
		case LogType::consoleOnly:
			std::cout << YELLOW << "[WARNING] " << info << RESET_COLOR << std::endl;
			break;
		case LogType::logFileOnly:
			PrintToFile("[WARNING] " + info);
			break;
		default:
			break;
		}
		
		
	}

	static void Error(std::string info, LogType logType = LogType::consoleAndLogFile) {

		switch (logType)
		{
		case LogType::consoleAndLogFile:
			std::cout << RED << "[ERROR] " << info << RESET_COLOR << std::endl;
			PrintToFile("[ERROR] " + info);
			break;
		case LogType::consoleOnly:
			std::cout << RED << "[ERROR] " << info << RESET_COLOR << std::endl;
			break;
		case LogType::logFileOnly:
			PrintToFile("[ERROR] " + info);
			break;
		default:
			break;
		}
		

		
	}

	static void PrintToFile(std::string info) {
		std::ofstream file("log.txt", std::ios::app);
		file << info << "\n";
		file.close();
	}
};