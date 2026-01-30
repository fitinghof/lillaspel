#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <sstream>

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

	inline static std::string logString = "";

	static constexpr const char* colors[3] = { "\033[0m", "\033[33m", "\033[31m"};
	static constexpr const char* logTexts[3] = { "", "[WARNING]", "[ERROR] " };

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

	static std::string readLogFile() {

	}

	template <typename T, typename... Types>
	static void createLog(std::string logOutput, T info, Types... infos) {
		std::ostringstream stringToReturn;
		stringToReturn << info;
		logOutput.append(stringToReturn.str());
		createLog(logOutput, infos...);
	}

	static void createLog(std::string logOutput) {
		logString.append(logOutput + "\n");
		PrintToFile(logOutput);
	}

public:
	template <typename T, typename... Types>
	static void Log(T info, Types... infos) {
		createLog(logTexts[LogType::log], info, infos...);
	}
	template <typename T, typename... Types>
	static void Warn(T info, Types... infos) {
		createLog(logTexts[LogType::warning], info, infos...);
	}
	template <typename T, typename... Types>
	static void Error(T info, Types... infos) {
		createLog(logTexts[LogType::error], info, infos...);
	}
	
	static std::string_view getLogStringRef() {
		return logString;
	}
};