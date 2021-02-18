#ifndef F4MP_CLIENT_LOGGER_H
#define F4MP_CLIENT_LOGGER_H

#include <Windows.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/async.h>

/*
 * TODO: Find a better alternative to macros here
 *
 */

#define LOGGER_INIT() { \
       AllocConsole();  \
       freopen_s((FILE**)stdout, "CONOUT$", "w", stdout); \
       auto console = spdlog::stdout_color_mt("console");      \
       auto fileConsole = spdlog::basic_logger_mt<spdlog::async_factory>("f4mp_logger", "logs/f4mp.txt"); \
       spdlog::set_default_logger(fileConsole);\
}

#define LOGGER_INFO(fmt, ...){ \
	spdlog::get("console")->info(fmt, __VA_ARGS__); \
	spdlog::get("f4mp_logger")->info(fmt, __VA_ARGS__);\
}

#define LOGGER_WARN(fmt, ...){ \
	spdlog::get("console")->warn(fmt, __VA_ARGS__); \
	spdlog::get("f4mp_logger")->warn(fmt, __VA_ARGS__);\
}

#define LOGGER_ERR(fmt, ...){ \
	spdlog::get("console")->warn(fmt, __VA_ARGS__); \
	spdlog::get("f4mp_logger")->warn(fmt, __VA_ARGS__);\
}

#endif //F4MP_CLIENT_LOGGER_H
