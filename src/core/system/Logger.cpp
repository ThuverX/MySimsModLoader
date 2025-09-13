//
// Created by exozg on 24/01/2025.
//

#include "Logger.h"

#include <cstdarg>

#include "spdlog/logger.h"
#include "spdlog/sinks/daily_file_sink.h"


namespace Msml::Core::System {
    std::shared_ptr<spdlog::logger> Logger::sLogger = nullptr;
    std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> Logger::sStdoutSink = nullptr;
    std::shared_ptr<spdlog::sinks::daily_file_sink_mt> Logger::sBasicSink = nullptr;

    void Logger::Log(const LogLevel kLevel, const bool kbDoCout, const char *pFile, const int kLine,
                     const char *pFormat, ...) {
        if (!sLogger) {
            return;
        }

        if (kbDoCout) {
            sStdoutSink->set_level(spdlog::level::trace);
        } else {
            sStdoutSink->set_level(spdlog::level::off);
        }

        std::array<char, 2048> buffer{};
        va_list args = nullptr;
        va_start(args, pFormat);
        std::vsnprintf(buffer.data(), buffer.size(), pFormat, args);
        va_end(args);

        const auto kMsg = fmt::format("[{}:{}] {}", pFile, kLine, buffer.data());

        sLogger->log(LogLevelToSpdLog(kLevel), kMsg);
    }

    void Logger::Flush() {
        sBasicSink->flush();
        sStdoutSink->flush();
        sLogger->flush();
    }

    spdlog::level::level_enum Logger::LogLevelToSpdLog(const LogLevel kLevel) {
        switch (kLevel) {
            case LogLevel::kInfo:
                return spdlog::level::info;
            case LogLevel::kWarning:
                return spdlog::level::warn;
            case LogLevel::kError:
                return spdlog::level::err;
            case LogLevel::kLua:
                return spdlog::level::info;
            case LogLevel::kDebug:
                return spdlog::level::debug;
        }
        return spdlog::level::info;
    }

    void Logger::Enable() {
        sStdoutSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        sBasicSink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/msml.log", 0, 0);
        sBasicSink->set_level(spdlog::level::trace);
        sStdoutSink->set_level(spdlog::level::trace);
        std::vector<spdlog::sink_ptr> sinks{sStdoutSink, sBasicSink};
        sLogger = std::make_shared<spdlog::logger>("", sinks.begin(), sinks.end());
    }
}
