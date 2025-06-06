//
// Created by exozg on 24/01/2025.
//

#include "Logger.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdarg>
#include <string>
#include <iomanip>
#include <sstream>
#include <windows.h>

namespace msml::core::system {
    std::filesystem::path Logger::module = "";

    // TODO: Logging should probably be on its own thread
    void Logger::Log(const LogLevel level, const bool do_cout, const char *file, int line, const char *format, ...) {
        const std::time_t now = std::time(nullptr);
        std::tm timeInfo{};
        if (localtime_s(&timeInfo, &now) != 0) {
            std::cerr << "Failed to get local time" << std::endl;
            return;
        }

        char timeBuffer[20];
        std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &timeInfo);

#ifndef UNIT_TESTING
        char dateBuffer[11];
        std::strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &timeInfo);
        const auto logFileName = module / (std::string("log_") + dateBuffer + ".log");

        std::ofstream logFile(logFileName, std::ios::app);
        if (!logFile) {
            std::cerr << "Failed to open log file: " << logFileName << std::endl;
        }
#endif // UNIT_TESTING

        char logMessage[1024];
        va_list args;
        va_start(args, format);
        std::vsnprintf(logMessage, sizeof(logMessage), format, args);
        va_end(args);

        const std::string relativeFile = std::string(file).substr(strlen(PROJECT_ROOT) + 1);

        const std::string logEntry = "[" + std::string(timeBuffer) + "] [" + LogLevelToString(level) + "] (" + relativeFile + ":" + std::to_string(line) + ") " + logMessage + "\n";

#ifndef UNIT_TESTING
        if (do_cout)
            std::cout << logEntry;
        if (logFile)
            logFile << logEntry;
#else
        std::cout << logEntry;
#endif // UNIT_TESTING
    }

    const char * Logger::LogLevelToString(LogLevel level) {
        switch (level) {
            case LogLevel::WARNING:     return "WARNING";
            case LogLevel::ERR:         return "ERROR";
            case LogLevel::DEBUG:       return "DEBUG";
            case LogLevel::LUA:         return "LUA";
            case LogLevel::INFO:
                default:                    return "INFO";
        }
    }
}