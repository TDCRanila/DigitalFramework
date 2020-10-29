#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/details/null_mutex.h>

#include <mutex>

namespace DCore
{

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

		// TODO Find a way to clean this function up some more, to avoid having the user to call std::bind(func, object, placeholder)
		// Could then also force the user to use a shared_ptr to an object?
		static void AddSubscriber(const LogSubscriberMessageFunc& a_func);

	protected:
		friend DFWSink_mt;
		friend DFWSink_st;
		static std::vector<LogSubscriberMessageFunc> _dfw_sink_subscribers;

	private:
		static spdlog::logger _main_logger;

		// TODO ST -> MT
		static std::shared_ptr<spdlog::sinks::wincolor_stdout_sink_st> _console_sink;
		static std::shared_ptr<DFWSink_st> _dfw_sink;
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

#define DFW_LOG(...)		{ /*EMPTY*/ }
#define DFW_DEBUGLOG(...)	{ /*EMPTY*/ }
#define DFW_INFOLOG(...)	{ /*EMPTY*/ }
#define DFW_WARNLOG(...)	{ /*EMPTY*/ }
#define DFW_ERRORLOG(...)	{ /*EMPTY*/ }

#endif

#pragma region Template Implementation

namespace DCore
{
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

		base_sink<Mutex>::formatter_->format(msg, formatted);
		formatted_string = fmt::to_string(formatted);
		
		for (const LogSubscriberMessageFunc& func: Logger::_dfw_sink_subscribers)
		{
			func(formatted_string);
		}

	}

	template<class Mutex> 
	void DFWSinkImpl<Mutex>::flush_()
	{
		std::cout << std::flush;
	}

} // End of namespace ~ DCore
#pragma endregion
