//
// Created by exozg on 24/01/2025.
//

#include "Logger.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdarg>
#include <string>

// Currently, the logger is depending on the ModLoader class. This is causing trouble
// with unit tests, since it would require the ModLoader to be included along with
// dependencies such as sigmatch, which isn't ideal for unit tests...
// There should be a better solution for this in the future.
#ifndef UNIT_TESTING
#   include "../modloader/ModLoader.h"
#endif // UNIT_TESTING

void Logger::Log(const LogLevel level, bool cout, const char *file, int line, const char *format, ...) {
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
    const std::string logFileName = ModLoader::GetInstance().modulePath + std::string("log_") + dateBuffer + ".txt";

    std::ofstream logFile(logFileName, std::ios::app);
    if (!logFile) {
        std::cerr << "Failed to open log file: " << logFileName << std::endl;
        return;
    }
#endif // UNIT_TESTING

    char logMessage[1024];
    va_list args;
    va_start(args, format);
    std::vsnprintf(logMessage, sizeof(logMessage), format, args);
    va_end(args);

    const std::string logEntry = "[" + std::string(timeBuffer) + "] [" + LogLevelToString(level) + "] (" + file + ":" + std::to_string(line) + ") " + logMessage + "\n";

#ifndef UNIT_TESTING
    if (cout)
        std::cout << logEntry;
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
