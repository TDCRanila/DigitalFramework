#include <CoreSystems/Logging/Logger.h>

#include <Utility/FileSystemUtility.h>
#include <Utility/StringUtility.h>
#include <Utility/TimeUtility.h>

namespace DCore
{
	spdlog::logger Logger::_main_logger("Main Log");

	std::shared_ptr<spdlog::sinks::wincolor_stdout_sink_st> Logger::_console_sink;
	std::shared_ptr<DFWSink_st> Logger::_dfw_sink;
	std::shared_ptr<spdlog::sinks::basic_file_sink_st> Logger::_file_sink;
	std::vector<LogSubscriberMessageFunc> Logger::_dfw_sink_subscribers;

	Logger::Logger()
	{
	}

	Logger::~Logger()
	{
	}

	void Logger::Init(bool a_enable_automatic_flush, int32 a_flush_interval_in_ms)
	{
		//trace		= SPDLOG_LEVEL_TRACE,
		//debug		= SPDLOG_LEVEL_DEBUG,
		//info		= SPDLOG_LEVEL_INFO,
		//warn		= SPDLOG_LEVEL_WARN,
		//err		= SPDLOG_LEVEL_ERROR,
		//critical	= SPDLOG_LEVEL_CRITICAL,
		//off		= SPDLOG_LEVEL_OFF,

		_console_sink	= std::make_shared<spdlog::sinks::wincolor_stdout_sink_st>();
		_dfw_sink		= std::make_shared<DFWSink_st>();
		// TODO Proper File Path Set
		// TODO filesystem Check if file exist for debuglog#000.txt, find suitable number

		std::string log_folder_name = std::string("logs");
		if (!DUtility::CreateNewDirectory("logs"))
		{
			DFW_ERRORLOG("Cannot create new directory named 'logs'.");
		}

		std::string log_file_type	= std::string(".txt");
		std::string log_file_date	= DUtility::GetTimeAndDateStamp();
		DUtility::FindAndRemoveChar(log_file_date, ':');
		std::string log_file_name	= std::string("DFW-debuglog-" + log_file_date + log_file_type);

		_file_sink		= std::make_shared<spdlog::sinks::basic_file_sink_st>(std::string(log_folder_name + DIR_SLASH + log_file_name));

		_main_logger.set_level(spdlog::level::trace);

		_main_logger.sinks().emplace_back(_console_sink);
		_console_sink->set_level(spdlog::level::debug);
		_console_sink->set_pattern("[%H:%M:%S] [%n] [%^%l%$] %v (%s@%#)"); // "[23:46:59.678] [mylogger] [info] Some message (main.cpp@21)"

		_main_logger.sinks().emplace_back(_dfw_sink);
		_dfw_sink->set_level(spdlog::level::debug);
		_dfw_sink->set_pattern("[%H:%M:%S] [%n] [%^%l%$] %v (%s@%#)"); // "[23:46:59.678] [mylogger] [info] Some message (main.cpp@21)"

		_main_logger.sinks().emplace_back(_file_sink);
		_file_sink->set_level(spdlog::level::trace);
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

	void Logger::AddSubscriber(const LogSubscriberMessageFunc& a_func)
	{
		_dfw_sink_subscribers.emplace_back(a_func);
	}

} // End of namespace ~ DCore.
