#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/details/null_mutex.h>

#include <CoreSystems/Logging/CustomTypes.h>

#include <mutex>
#include <unordered_map>

namespace DFW
{
	enum class LogLevel
	{
		None = 0,	//off		= SPDLOG_LEVEL_OFF,
		Trace,		//trace		= SPDLOG_LEVEL_TRACE,
		Debug,		//debug		= SPDLOG_LEVEL_DEBUG,
		Info,		//info		= SPDLOG_LEVEL_INFO,
		Warning,	//warn		= SPDLOG_LEVEL_WARN,
		Error,		//err		= SPDLOG_LEVEL_ERROR,
	};

	enum class LogType
	{
		None = 0,
		PlatformConsole,
		FrameworkLogger, 
		FileLogger,
	};

	template<class Mutex>
	class DFWSinkImpl : public spdlog::sinks::base_sink <Mutex>
	{
	public: 
		DFWSinkImpl();
		~DFWSinkImpl();
	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override;
		void flush_() override;

	};

	typedef DFWSinkImpl<std::mutex>						DFWSink_mt;
	typedef DFWSinkImpl<spdlog::details::null_mutex>	DFWSink_st;

	typedef std::function<void(std::string)> LogSubscriberMessageFunc;

	class Logger
	{
	public:
		Logger();
		~Logger();

		static void Init(bool a_enable_automatic_flush, int32 a_flush_interval_in_ms);
		static void ManuelFlush();

		static spdlog::logger& ProvideMainLogger();
		static void AdjustLoggingLevel(LogType a_logger, LogLevel a_log_level);

		// TODO Find a way to clean this function up some more, to avoid having the user to call std::bind(func, object, placeholder)
		// Could then also force the user to use a shared_ptr to an object?
		static void AddSubscriber(DUID a_subscriber_id, const LogSubscriberMessageFunc& a_func);
		static void RemoveSubscriber(DUID a_subscriber_id, const LogSubscriberMessageFunc& a_func);

	protected:
		friend DFWSink_mt;
		friend DFWSink_st;
		static std::unordered_map<DUID, LogSubscriberMessageFunc> _dfw_sink_subscribers;

	private:
		static spdlog::logger _main_logger;

		// TODO ST -> MT
		static std::shared_ptr<spdlog::sinks::wincolor_stdout_sink_st> _console_sink;
		static std::shared_ptr<DFWSink_st> _dfw_sink;
		static std::shared_ptr<spdlog::sinks::basic_file_sink_st> _file_sink;

	};

} // End of namespace ~ DFW

#if defined(DFW_PDEBUG64) || defined(DFW_DEBUG64) || defined(DFW_PRERELEASE64)

#define DFW_LOG(...)		SPDLOG_LOGGER_TRACE(&DFW::Logger::ProvideMainLogger(), __VA_ARGS__)
#define DFW_DEBUGLOG(...)	SPDLOG_LOGGER_DEBUG(&DFW::Logger::ProvideMainLogger(), __VA_ARGS__)
#define DFW_INFOLOG(...)	SPDLOG_LOGGER_INFO(&DFW::Logger::ProvideMainLogger(), __VA_ARGS__)
#define DFW_WARNLOG(...)	SPDLOG_LOGGER_WARN(&DFW::Logger::ProvideMainLogger(), __VA_ARGS__)
#define DFW_ERRORLOG(...)	SPDLOG_LOGGER_ERROR(&DFW::Logger::ProvideMainLogger(), __VA_ARGS__)

#elif defined(DFW_RELEASE64) || defined(DFW_FINAL64)

#define DFW_LOG(...)		{ /*EMPTY*/ }
#define DFW_DEBUGLOG(...)	{ /*EMPTY*/ }
#define DFW_INFOLOG(...)	{ /*EMPTY*/ }
#define DFW_WARNLOG(...)	{ /*EMPTY*/ }
#define DFW_ERRORLOG(...)	{ /*EMPTY*/ }

#endif

namespace DFW
{
#pragma region Template Implementation
	template<class Mutex> 
	DFWSinkImpl<Mutex>::DFWSinkImpl()
	{
	}

	template<class Mutex> 
	DFWSinkImpl<Mutex>::~DFWSinkImpl()
	{
	}

	template<class Mutex> 
	void DFWSinkImpl<Mutex>::sink_it_(const spdlog::details::log_msg& msg)
	{
		spdlog::memory_buf_t formatted;
		std::string formatted_string;

		spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
		formatted_string = fmt::to_string(formatted);
		
		for (const auto&[id, func] : Logger::_dfw_sink_subscribers)
		{
			func(formatted_string);
		}

	}

	template<class Mutex> 
	void DFWSinkImpl<Mutex>::flush_()
	{
		std::cout << std::flush;
	}

#pragma endregion

} // End of namespace ~ DFW.
