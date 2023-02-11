#pragma once

#include <memory>
#include <spdlog/spdlog.h>

class Log
{
public:
    static void Init();
    static inline std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

private:
    static std::shared_ptr<spdlog::logger> s_Logger;
};

#if defined(GL_DEBUG) || defined(GL_RELEASE)
#   define LOG_TRACE(...)    ::Log::GetLogger()->trace(__VA_ARGS__)
#   define LOG_INFO(...)     ::Log::GetLogger()->info(__VA_ARGS__)
#   define LOG_WARN(...)     ::Log::GetLogger()->warn(__VA_ARGS__)
#   define LOG_ERROR(...)    ::Log::GetLogger()->error(__VA_ARGS__)
#   define LOG_FATAL(...)    ::Log::GetLogger()->critical(__VA_ARGS__)
#else
#   define LOG_TRACE(...)    ::Log::GetLogger()->trace(__VA_ARGS__)
#   define LOG_INFO(...)     ::Log::GetLogger()->info(__VA_ARGS__)
#   define LOG_WARN(...)     ::Log::GetLogger()->warn(__VA_ARGS__)
#   define LOG_ERROR(...)    ::Log::GetLogger()->error(__VA_ARGS__)
#   define LOG_FATAL(...)    ::Log::GetLogger()->critical(__VA_ARGS__)
#endif
