#include <iostream>
#include <string>

// Just the basic stuff, someone rewrite for more functionality

class Logger {
public:
	static void log(std::string info) {
		std::cout << info << std::endl;
	}

	static void warn(std::string info) {
		std::cout << info << std::endl;
	}

	static void error(std::string info) {
		std::cout << info << std::endl;
	}
};