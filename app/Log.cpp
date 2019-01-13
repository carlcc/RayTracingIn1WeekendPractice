/**
 * Copyright (c) 2018 Duobei Brothers Information Technology Co.,Ltd. All rights reserved.
 * This file is part of Janus-gateway. 
 * Specifically, it is part of Duobei Streaming Plugin.
 *
 * Author: Chen Chen (chen.chen@duobei.com)
 * 
 * Date: 2018-8-27
 *
 * About this file: This file holds Logger methods/macros
 */

#include "Log.h"

//#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
//#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> __db_logger__ = nullptr;

extern bool loggerInit(const char* logFilePath, LogLevel logLevel)
{
    if (nullptr != logFilePath) {
        __db_logger__ = spdlog::daily_logger_mt("file", logFilePath);
    } else {
        __db_logger__ = spdlog::stdout_color_mt("con");;
    }
    if (__db_logger__ == nullptr) {
        return false;
    }

    spdlog::level::level_enum level;
    switch (logLevel) {
        case LogLevel::Trace:
            level = spdlog::level::trace;
            break;
        case LogLevel::Debug:
            level = spdlog::level::debug;
            break;
        case LogLevel::Info:
            level = spdlog::level::info;
            break;
        case LogLevel::Warn:
            level = spdlog::level::warn;
            break;
        case LogLevel::Fatal:
            level = spdlog::level::err;
            break;
        default:
            ASSERT(false, "impossible log level");
    }
    __db_logger__->set_level(level);

//    #ifdef NDEBUG
//    __db_logger__->flush_on(spdlog::level::info);
//    spdlog::set_pattern("%^[%H:%M:%S.%f %z][%l]%$%v");
//    #else
    __db_logger__->flush_on(level);
    spdlog::set_pattern("%^[%H:%M:%S.%f %z][%l][thread %t]%$%v");
//    #endif
    return true;
}

extern void loggerClean()
{
    __db_logger__ = nullptr;
}