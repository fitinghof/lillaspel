#include <iostream>
#include <string>

// Just the basic stuff, someone rewrite for more functionality

#define RESET_COLOR	"\033[0m"
#define YELLOW	"\033[33m"
#define RED		"\033[31m" 

class Logger {
public:
	static void log(std::string info) {
		std::cout << info << std::endl;
	}

	static void warn(std::string info) {
		std::cout << YELLOW << "[WARNING] " << info << RESET_COLOR << std::endl;
	}

	static void error(std::string info) {
		std::cout << RED << "[ERROR] " << info << RESET_COLOR << std::endl;
	}
};