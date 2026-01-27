#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>


enum LogOption
{
	consoleAndLogFile,
	consoleOnly,
	logFileOnly
};

class Logger {

private:

	enum LogType
	{
		log = 0,
		warning = 1,
		error = 2
	};

	static constexpr const char* colors[3] = { "\033[0m", "\033[33m", "\033[31m"};
	static constexpr const char* logTexts[3] = { "", "[WARNING]", "[ERROR] " };

	static void createLog(std::string info, LogOption logOption, LogType logType) {
		switch (logOption)
		{
		case LogOption::consoleAndLogFile:
			std::cout << colors[logType] << logTexts[logType] << info << colors[0] << std::endl;
			PrintToFile(logTexts[logType] + info);
			break;
		case LogOption::consoleOnly:
			std::cout << colors[logType] << logTexts[logType] << info << colors[0] << std::endl;
			break;
		case LogOption::logFileOnly:
			PrintToFile(logTexts[logType] + info);
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
	
	static void TimeStamp() {
		time_t timestamp;
		time(&timestamp);
		//cout << ctime(&timestamp);
	}

public:
	static void Log(std::string info, LogOption logOption = LogOption::consoleAndLogFile) {
		createLog(info, logOption, LogType::log);
	}

	static void Warn(std::string info, LogOption logOption = LogOption::consoleAndLogFile) {
		createLog(info, logOption, LogType::warning);
	}

	static void Error(std::string info, LogOption logOption = LogOption::consoleAndLogFile) {
		createLog(info, logOption, LogType::error);
	}

	
};