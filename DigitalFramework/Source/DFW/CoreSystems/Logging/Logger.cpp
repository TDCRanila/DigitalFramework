#include <CoreSystems/Logging/Logger.h>

#include <Utility/FileSystemUtility.h>
#include <Utility/StringUtility.h>
#include <Utility/TimeUtility.h>

namespace DFW
{
	spdlog::logger Logger::_main_logger("Main Log");

	std::unordered_map<DUID, LogSubscriberMessageFunc> Logger::_dfw_sink_subscribers;
	
	SharedPtr<spdlog::sinks::wincolor_stdout_sink_st> Logger::_console_sink;
	SharedPtr<DFWSink_st> Logger::_framework_sink;
	SharedPtr<spdlog::sinks::basic_file_sink_st> Logger::_file_sink;

	void Logger::Init(bool const a_enable_automatic_flush, int32 const a_flush_interval_in_ms)
	{
		//trace		= SPDLOG_LEVEL_TRACE,
		//debug		= SPDLOG_LEVEL_DEBUG,
		//info		= SPDLOG_LEVEL_INFO,
		//warn		= SPDLOG_LEVEL_WARN,
		//err		= SPDLOG_LEVEL_ERROR,
		//critical	= SPDLOG_LEVEL_CRITICAL,
		//off		= SPDLOG_LEVEL_OFF,

		// Main logger should track all log events.
		_main_logger.set_level(spdlog::level::trace);

		// Console Logger.
		_console_sink = MakeShared<spdlog::sinks::wincolor_stdout_sink_st>();
		_main_logger.sinks().emplace_back(_console_sink);
		_console_sink->set_level(spdlog::level::debug);
		_console_sink->set_pattern("[%H:%M:%S] [%n] [%^%l%$] %v (%s@%#)"); // "[23:46:59.678] [mylogger] [info] Some message (main.cpp@21)"

		// Framework Logger.
		_framework_sink = MakeShared<DFWSink_st>();
		_main_logger.sinks().emplace_back(_framework_sink);
		_framework_sink->set_level(spdlog::level::debug);
		_framework_sink->set_pattern("[%H:%M:%S] [%n] [%^%l%$] %v (%s@%#)"); // "[23:46:59.678] [mylogger] [info] Some message (main.cpp@21)"

		// File Logger.
		// TODO Proper File Path Set.
		std::string const log_folder_name("logs");
		DFW_ASSERT(DUtility::CreateNewDirectory(log_folder_name) && "Cannot create new directory named with provided name.");

		std::string log_file_date(DUtility::GetTimeAndDateStamp());
		DUtility::FindAndRemoveChar(log_file_date, ':');

		std::string const log_file_extension(".txt");
		std::string const log_file_name("DFW-debuglog-" + log_file_date + log_file_extension);
		_file_sink = MakeShared<spdlog::sinks::basic_file_sink_st>(std::string(log_folder_name + DIR_SLASH + log_file_name));
		_main_logger.sinks().emplace_back(_file_sink);
		_file_sink->set_level(spdlog::level::debug);
		_file_sink->set_pattern("[%D %H:%M:%S] [%n] [%l] %v (%s@%#)"); //"[2014-10-31 23:46:59.678] [mylogger] [info] Some message (main.cpp@21)"

		if (a_enable_automatic_flush)
		{
			_main_logger.flush_on(spdlog::level::trace);
			spdlog::flush_every(std::chrono::seconds(static_cast<int32>(a_flush_interval_in_ms * 0.001f)));
		}
	}

	void Logger::ManuelFlush()
	{
		_main_logger.flush();
	}

	spdlog::logger& Logger::ProvideMainLogger()
	{
		return _main_logger;
	}

	void Logger::AdjustLoggingLevel(LogType const a_logger, LogLevel const a_log_level)
	{
		spdlog::level::level_enum new_log_level(spdlog::level::off);
		switch (a_log_level)
		{
			case LogLevel::None:	new_log_level = spdlog::level::off;		break;
			case LogLevel::Trace:	new_log_level = spdlog::level::trace;	break;
			case LogLevel::Debug:	new_log_level = spdlog::level::debug;	break;
			case LogLevel::Info:	new_log_level = spdlog::level::info;	break;
			case LogLevel::Warning:	new_log_level = spdlog::level::warn;	break;
			case LogLevel::Error:	new_log_level = spdlog::level::err;		break;
			default: DFW_WARNLOG("Trying to adjust the log level of a log type to something that is invalid."); break;
		}

		switch (a_logger)
		{
			case LogType::PlatformConsole: 
			{
				if (_console_sink->level() != new_log_level)
					_console_sink->set_level(new_log_level);
				break;
			}
			case LogType::FrameworkLogger:
			{
				if (_framework_sink->level() != new_log_level)
					_framework_sink->set_level(new_log_level);
				break;
			}
			case LogType::FileLogger:
			{
				if (_file_sink->level() != new_log_level)
					_file_sink->set_level(new_log_level);
				break;
			}
			case LogType::None:
			default: 
			{
				DFW_WARNLOG("Trying to adjust the log level of a log type that is invalid.");
				break;
			}
		}
	}

	void Logger::AddSubscriber(DUID const a_subscriber_id, LogSubscriberMessageFunc const& a_func)
	{
		DFW_INFOLOG("Adding logging subscriber with ID: {}", a_subscriber_id);
		_dfw_sink_subscribers.emplace(a_subscriber_id, a_func);
	}

	void Logger::RemoveSubscriber(DUID const a_subscriber_id)
	{
		if (auto const& it_result = _dfw_sink_subscribers.find(a_subscriber_id); it_result == _dfw_sink_subscribers.end())
		{
			DFW_LOG("Couldn't find logging subscriber with ID: {} when trying to remove it.", a_subscriber_id);
		}
		else
		{
			DFW_INFOLOG("Removing logging subscriber with ID: {}", a_subscriber_id);
			_dfw_sink_subscribers.erase(it_result);
		}
	}

} // End of namespace ~ DFW.
