//
// Created by exozg on 24/01/2025.
//

#include "Logger.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdarg>
#include <string>

#include "../modloader/ModLoader.h"


void Logger::Log(const LogLevel level, const char *file, int line, const char *format, ...) {
    const std::time_t now = std::time(nullptr);
    std::tm timeInfo{};
    if (localtime_s(&timeInfo, &now) != 0) {
        std::cerr << "Failed to get local time" << std::endl;
        return;
    }

    char timeBuffer[20];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &timeInfo);

    char dateBuffer[11];
    std::strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &timeInfo);
    const std::string logFileName = ModLoader::GetInstance().modulePath + std::string("log_") + dateBuffer + ".txt";

    std::ofstream logFile(logFileName, std::ios::app);
    if (!logFile) {
        std::cerr << "Failed to open log file: " << logFileName << std::endl;
        return;
    }

    char logMessage[1024];
    va_list args;
    va_start(args, format);
    std::vsnprintf(logMessage, sizeof(logMessage), format, args);
    va_end(args);

    const std::string logEntry = "[" + std::string(timeBuffer) + "] [" + LogLevelToString(level) + "] (" + file + ":" + std::to_string(line) + ") " + logMessage + "\n";

    std::cout << logEntry;
    logFile << logEntry;
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
