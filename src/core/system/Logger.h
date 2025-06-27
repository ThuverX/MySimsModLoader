//
// Created by exozg on 24/01/2025.
//

#ifndef LOGGER_H
#define LOGGER_H
#include <filesystem>

namespace Msml::Core::System {
    class Logger {
    public:
        enum class LogLevel: uint8_t {
            kInfo,
            kWarning,
            kError,
            kLua,
            kDebug
        };
        static void Log(LogLevel kLevel, bool kbDoCout, const char *pFile, int kLine, const char* pFormat, ...);
        static std::filesystem::path sModule;

    private:
        static const char* LogLevelToString(LogLevel kLevel);
    };
}

#define MSML_LOGLEVEL(level, format, ...) \
    ::Msml::Core::System::Logger::Log(::Msml::Core::System::Logger::LogLevel::level, true, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define MSML_LOG_INFO(format, ...) \
    MSML_LOGLEVEL(kInfo, format, ##__VA_ARGS__)

#define MSML_LOG_WARNING(format, ...) \
    MSML_LOGLEVEL(kWarning, format, ##__VA_ARGS__)

#define MSML_LOG_ERROR(format, ...) \
    MSML_LOGLEVEL(kError, format, ##__VA_ARGS__)

#ifndef NDEBUG
#define MSML_LOG_DEBUG(format, ...) \
    MSML_LOGLEVEL(kDebug, format, ##__VA_ARGS__)
#else
#define MSML_LOG_DEBUG(format, ...) \
    ::Msml::Core::System::Logger::Log(::Msml::Core::System::Logger::LogLevel::kDebug, false, __FILE__, __LINE__, format, ##__VA_ARGS__)
#endif

#define MSML_LOG_DEBUG_HIDDEN(format, ...) \
    ::Msml::Core::System::Logger::Log(::Msml::Core::System::Logger::LogLevel::kDebug, false, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define MSML_LOG_LUA(format, ...) \
    MSML_LOGLEVEL(kLua, format, ##__VA_ARGS__)

#endif //LOGGER_H
