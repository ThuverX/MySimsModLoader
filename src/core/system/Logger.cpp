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

namespace Msml::Core::System {
    std::filesystem::path Logger::sModule = "";

    // TODO: Logging should probably be on its own thread
    void Logger::Log(const LogLevel kLevel, const bool kbDoCout, const char *pFile, const int kLine,
                     const char *pFormat, ...) {
        const std::time_t kNow = std::time(nullptr);
        std::tm timeInfo{};
        if (localtime_s(&timeInfo, &kNow) != 0) {
            std::cerr << "Failed to get local time\n";
            return;
        }

        char timeBuffer[20];
        std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &timeInfo);

#ifndef UNIT_TESTING
        char dateBuffer[11];
        std::strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &timeInfo);
        const auto kLogFileName = sModule / (std::string("log_") + dateBuffer + ".log");

        std::ofstream logFile(kLogFileName, std::ios::app);
        if (!logFile) {
            std::cerr << "Failed to open log file: " << kLogFileName << "\n";
        }
#endif // UNIT_TESTING

        char logMessage[1024];
        va_list args = nullptr;
        va_start(args, pFormat);
        std::vsnprintf(logMessage, sizeof(logMessage), pFormat, args);
        va_end(args);

        const std::string kRelativeFile = std::string(pFile).substr(strlen(PROJECT_ROOT) + 1);

        const std::string kLogEntry = "[" + std::string(timeBuffer) + "] [" + LogLevelToString(kLevel) + "] (" +
                                      kRelativeFile + ":" + std::to_string(kLine) + ") " + logMessage + "\n";

#ifndef UNIT_TESTING
        if (kbDoCout) {
            std::cout << kLogEntry;
        }
        if (logFile) {
            logFile << kLogEntry;
        }
#else
        std::cout << logEntry;
#endif // UNIT_TESTING
    }

    const char *Logger::LogLevelToString(const LogLevel kLevel) {
        switch (kLevel) {
            case LogLevel::kWarning: return "WARNING";
            case LogLevel::kError: return "ERROR";
            case LogLevel::kDebug: return "DEBUG";
            case LogLevel::kLua: return "LUA";
            case LogLevel::kInfo:
            default: return "INFO";
        }
    }
}
