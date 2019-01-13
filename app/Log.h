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

#pragma once

#include "spdlog/spdlog.h"

/*
 * The following 2 function are just used to get the file name(not path) of a file.
 * Write in recursive style so that this code can be compiled under C++ 11
 */
constexpr const char* __LOG_FILE_NAME2__(const char* f)
{
    return (f[0] == '/' || f[0] == '\\') ? (f + 1) : __LOG_FILE_NAME2__(f - 1);
}

constexpr const char* __LOG_FILE_NAME1__(const char* filename)
{
    return (filename[0] == 0) ? (
            __LOG_FILE_NAME2__(filename)
    ) : (
            __LOG_FILE_NAME1__(filename + 1)
           );
}

enum LogLevel {
    Trace,
    Debug,
    Info,
    Warn,
    Fatal
};

extern std::shared_ptr<spdlog::logger> __db_logger__;

extern bool loggerInit(const char* logFilePath, LogLevel logLevel);

extern void loggerClean();

#ifdef DBSTREAMING_TRACE_ON
#define TRACE(fmt, ...) do { __db_logger__->trace("({}:{}):" fmt, __FILE__, __LINE__, ##__VA_ARGS__); } while (false)
#else
#define TRACE(...)
#endif
#define TRACE_MEMBER(fmt, ...) TRACE("{}(): " fmt, __FUNCTION__, ##__VA_ARGS__)

#ifdef DBSTREAMING_DEBUG_ON
#define DEBUG(fmt, ...) do { __db_logger__->debug("({}:{}):" fmt, __FILE__, __LINE__, ##__VA_ARGS__); } while (false)
#else
#define DEBUG(fmt, ...)
#endif

#define PRINT(fmt, ...) do { __db_logger__->info(fmt, ##__VA_ARGS__); } while (false)

#define INFO(fmt, ...) do { __db_logger__->info("({}:{}):" fmt, __LOG_FILE_NAME1__(__FILE__), __LINE__, ##__VA_ARGS__); } while (false)
#define WARN(fmt, ...) do { __db_logger__->warn("({}:{}):" fmt, __LOG_FILE_NAME1__(__FILE__), __LINE__, ##__VA_ARGS__); } while (false)
#define FATAL(fmt, ...) do { __db_logger__->error("({}:{}):" fmt, __LOG_FILE_NAME1__(__FILE__), __LINE__, ##__VA_ARGS__); } while (false)

// 暂不使用 FATAL
//#define FATAL(fmt, ...) do { __db_logger__->critical("({}:{}):" fmt, __LOG_FILE_NAME1__(__FILE__), __LINE__, ##__VA_ARGS__); } while (false)

#define TEST_TRACE() TRACE("{}()", __FUNCTION__)
#define TEST_TRACE_MEMBER() TRACE("{}(), thisptr = {}", __FUNCTION__, (void*)this)

#define ASSERT(cond, msg, ...) do { if (cond) break; FATAL(msg, ##__VA_ARGS__); abort(); } while (false)