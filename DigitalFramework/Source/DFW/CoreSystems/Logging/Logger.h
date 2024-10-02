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

#include <CoreSystems/DUID.h>
#include <CoreSystems/Memory.h>

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
		void sink_it_(spdlog::details::log_msg const& a_msg) override;
		void flush_() override;

	};

	using DFWSink_mt = DFWSinkImpl<std::mutex>;
	using DFWSink_st = DFWSinkImpl<spdlog::details::null_mutex>;

	using LogSubscriberMessageFunc = std::function<void(std::string const&)>;

	class Logger
	{
	private:
		friend DFWSink_mt;
		friend DFWSink_st;
	public:
		Logger() = default;
		~Logger() = default;

		static void Init(bool const a_enable_automatic_flush, int32 const a_flush_interval_in_ms);
		static void ManuelFlush();

		static spdlog::logger& ProvideMainLogger();
		static void AdjustLoggingLevel(LogType const a_logger, LogLevel const a_log_level);

		// TODO Find a way to clean this function up some more, to avoid having the user to call std::bind(func, object, placeholder)
		// Could then also force the user to use a shared_ptr to an object?
		static void AddSubscriber(DUID const a_subscriber_id, LogSubscriberMessageFunc const& a_func);
		static void RemoveSubscriber(DUID const a_subscriber_id);

	private:
		static spdlog::logger _main_logger;

		static std::unordered_map<DUID, LogSubscriberMessageFunc> _dfw_sink_subscribers;

		// TODO ST -> MT
		static SharedPtr<spdlog::sinks::wincolor_stdout_sink_st> _console_sink;
		static SharedPtr<DFWSink_st> _framework_sink;
		static SharedPtr<spdlog::sinks::basic_file_sink_st> _file_sink;

	};

} // End of namespace ~ DFW

#if defined(DFW_PDEBUG64) || defined(DFW_DEBUG64) || defined(DFW_PRERELEASE64)

#define DFW_LOG(...)		SPDLOG_LOGGER_TRACE(&DFW::Logger::ProvideMainLogger(), __VA_ARGS__)
#define DFW_DEBUGLOG(...)	SPDLOG_LOGGER_DEBUG(&DFW::Logger::ProvideMainLogger(), __VA_ARGS__)
#define DFW_INFOLOG(...)	SPDLOG_LOGGER_INFO(&DFW::Logger::ProvideMainLogger(), __VA_ARGS__)
#define DFW_WARNLOG(...)	SPDLOG_LOGGER_WARN(&DFW::Logger::ProvideMainLogger(), __VA_ARGS__)
#define DFW_ERRORLOG(...)	SPDLOG_LOGGER_ERROR(&DFW::Logger::ProvideMainLogger(), __VA_ARGS__)

#elif defined(DFW_RELEASE64)

#define DFW_LOG(...)
#define DFW_DEBUGLOG(...)
#define DFW_INFOLOG(...)
#define DFW_WARNLOG(...)
#define DFW_ERRORLOG(...)

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
	void DFWSinkImpl<Mutex>::sink_it_(spdlog::details::log_msg const& a_msg)
	{
		spdlog::memory_buf_t formatted;
		//std::string formatted_string;

		spdlog::sinks::base_sink<Mutex>::formatter_->format(a_msg, formatted);
		std::string const formatted_string(fmt::to_string(formatted));

		for (auto const& [sink_id, sink_func] : Logger::_dfw_sink_subscribers)
		{
			sink_func(formatted_string);
		}
	}

	template<class Mutex> 
	void DFWSinkImpl<Mutex>::flush_()
	{
		std::cout << std::flush;
	}

#pragma endregion

} // End of namespace ~ DFW.
