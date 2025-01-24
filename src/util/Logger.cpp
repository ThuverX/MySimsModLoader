//
// Created by exozg on 24/01/2025.
//

#include "Logger.h"

#include <cstdarg>
#include <cstdio>
#include <ctime>



void Logger::Log(const LogLevel level, const char *file, int line, const char *format, ...) {
    const std::time_t now = std::time(nullptr);

    char timeBuffer[20];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    printf("[%s] [%s] (%s:%d) ", timeBuffer, LogLevelToString(level), file, line);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
}

const char * Logger::LogLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::WARNING:     return "WARNING";
        case LogLevel::ERR:         return "ERROR";
        case LogLevel::DEBUG:       return "DEBUG";
        case LogLevel::INFO:
        default:                    return "INFO";
    }
}
