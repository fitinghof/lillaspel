#pragma once

#include <iostream>
#include <string>
#include <fstream>

// Just the basic stuff, someone rewrite for more functionality

#define RESET_COLOR	"\033[0m"
#define YELLOW	"\033[33m"
#define RED		"\033[31m" 

class Logger {
public:
	static void Log(std::string info) {
		std::cout << info << std::endl;

		PrintToFile(info);
	}

	static void Warn(std::string info) {
		std::cout << YELLOW << "[WARNING] " << info << RESET_COLOR << std::endl;
		
		PrintToFile("[WARNING] " + info);
	}

	static void Error(std::string info) {
		std::cout << RED << "[ERROR] " << info << RESET_COLOR << std::endl;

		PrintToFile("[ERROR] " + info);
	}

	static void PrintToFile(std::string info) {
		std::ofstream file("log.txt", std::ios::app);
		file << info << "\n";
		file.close();
	}
};