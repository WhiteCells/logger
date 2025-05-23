#ifndef _LOGGER_H_
#define _LOGGER_H_

#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>
#include <spdlog/async_logger.h>
#include <memory>
#include <vector>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
// #include <mutex>

class Logger
{
public:
    static void init(
        std::string title = "logger",
        unsigned que_size = 8192,
        unsigned thread_cnt = 1,
        const std::string &log_dir = "logs",
        unsigned max_byte = 1048576 * 5,
        unsigned max_save = 3)
    {
        spdlog::init_thread_pool(que_size, thread_cnt);

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern("[%H:%M:%S %z] [%^%L%$] %v");

        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm tm_now;

#if defined(_WIN32)
        localtime_s(&tm_now, &now_c);
#else
        localtime_r(&now_c, &tm_now);
#endif
        std::ostringstream oss;
        oss << log_dir << "/"
            << title << "_"
            << std::put_time(&tm_now, "%Y-%m-%d_%H-%M-%S") << ".log";
        std::string log_path = oss.str();

        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_path, max_byte, max_save);

        std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};

        logger = std::make_shared<spdlog::async_logger>(
            title,
            sinks.begin(), sinks.end(),
            spdlog::thread_pool(),
            spdlog::async_overflow_policy::block);

        spdlog::register_logger(logger);
        logger->set_level(spdlog::level::debug);
        logger->flush_on(spdlog::level::info);
    }

    template <typename... Args>
    static void debug(Args &&...args)
    {
        get()->debug(std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void info(Args &&...args)
    {
        get()->info(std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void warn(Args &&...args)
    {
        get()->warn(std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void error(Args &&...args)
    {
        get()->error(std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void critical(Args &&...args)
    {
        get()->critical(std::forward<Args>(args)...);
    }

private:
    static std::shared_ptr<spdlog::logger> get()
    {
        // std::call_once(init_flag, []() {
        //     init();
        // });
        return logger;
    }

private:
    static std::shared_ptr<spdlog::logger> logger;
    // static std::once_flag init_flag;
};

// std::once_flag Logger::init_flag;

#endif // _LOGGER_H_