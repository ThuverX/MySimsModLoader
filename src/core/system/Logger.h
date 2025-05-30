//
// Created by exozg on 24/01/2025.
//

#ifndef LOGGER_H
#define LOGGER_H
#include <filesystem>

namespace msml::core::system {
    class Logger {
    public:
        enum class LogLevel {
            INFO,
            WARNING,
            ERR,
            LUA,
            DEBUG
        };
        static void Log(LogLevel level, bool do_cout, const char *file, int line, const char* format, ...);
        static std::filesystem::path module;

    private:
        static const char* LogLevelToString(LogLevel level);
    };
}

#define MSML_LOGLEVEL(level, format, ...) \
    ::msml::core::system::Logger::Log(::msml::core::system::Logger::LogLevel::level, true, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define MSML_LOG_INFO(format, ...) \
    MSML_LOGLEVEL(INFO, format, ##__VA_ARGS__)

#define MSML_LOG_WARNING(format, ...) \
    MSML_LOGLEVEL(WARNING, format, ##__VA_ARGS__)

#define MSML_LOG_ERROR(format, ...) \
    MSML_LOGLEVEL(ERR, format, ##__VA_ARGS__)

#ifndef NDEBUG
#define MSML_LOG_DEBUG(format, ...) \
    MSML_LOGLEVEL(DEBUG, format, ##__VA_ARGS__)
#else
#define MSML_LOG_DEBUG(format, ...) \
    ::msml::core::system::Logger::Log(::msml::core::system::Logger::LogLevel::DEBUG, false, __FILE__, __LINE__, format, ##__VA_ARGS__)
#endif

#define MSML_LOG_DEBUG_HIDDEN(format, ...) \
    ::msml::core::system::Logger::Log(::msml::core::system::Logger::LogLevel::DEBUG, false, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define MSML_LOG_LUA(format, ...) \
    MSML_LOGLEVEL(LUA, format, ##__VA_ARGS__)

#endif //LOGGER_H
