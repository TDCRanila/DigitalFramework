#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include <memory>

namespace DCore
{
	class Logger
	{
	public:
		Logger();
		~Logger();

		static void Init(bool a_enable_automatic_flush, int32 a_flush_interval_in_ms);
		static void ManuelFlush();

		static spdlog::logger& ProvideMainLogger();

	private:
		static spdlog::logger _main_logger;

		// TODO ST -> MT
		static std::shared_ptr<spdlog::sinks::wincolor_stdout_sink_st> _console_sink;
		static std::shared_ptr<spdlog::sinks::basic_file_sink_st> _file_sink;

	};

} // End of namespace ~ DCore

#if defined(DFW_PDEBUG64) || defined(DFW_DEBUG64) || defined(DFW_PRERELEASE64)

#define DFW_LOG(...)		SPDLOG_LOGGER_TRACE(&DCore::Logger::ProvideMainLogger(), __VA_ARGS__)
#define DFW_DEBUGLOG(...)	SPDLOG_LOGGER_DEBUG(&DCore::Logger::ProvideMainLogger(), __VA_ARGS__)
#define DFW_INFOLOG(...)	SPDLOG_LOGGER_INFO(&DCore::Logger::ProvideMainLogger(), __VA_ARGS__)
#define DFW_WARNLOG(...)	SPDLOG_LOGGER_WARN(&DCore::Logger::ProvideMainLogger(), __VA_ARGS__)
#define DFW_ERRORLOG(...)	SPDLOG_LOGGER_ERROR(&DCore::Logger::ProvideMainLogger(), __VA_ARGS__)

#elif defined(DFW_RELEASE64) || defined(DFW_FINAL64)

#define DFW_DEBUGLOG(...)	{ /*EMPTY*/ }
#define DFW_INFOLOG(...)	{ /*EMPTY*/ }
#define DFW_WARNLOG(...)	{ /*EMPTY*/ }
#define DFW_ERRORLOG(...)	{ /*EMPTY*/ }

#endif
