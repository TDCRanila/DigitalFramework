#pragma once

#include <CoreSystems/Logging/Logger.h>
#include <CoreSystems/Stage/Stage.h>

#include <string>
#include <vector>

namespace DEditor
{

	class MainConsole : public DCore::StageBase
	{
	public:
		MainConsole(std::string a_stage_name, bool a_start_disabled = false);

		~MainConsole() = default;

		virtual void RenderImGui() override;

		virtual void Update() override;

		virtual void OnAttached() override;

		virtual void OnRemoved() override;

		void LogMessageCallback(std::string a_log_message);

	private:

		struct ConsoleLogFilter
		{
			std::string _filter_string;
			int32 _log_level;
		};

		struct ParsedLogMessageStruct
		{
			std::string _original_log_string;
			//int32 time_stamp;
			//int32 type;
			//int32 message;
			//int32 message_begin;
			//int32 message_end;
			bool _show_message = true;
			int64 _num_entry = 0;
		};

		void ParseLogMessage(ParsedLogMessageStruct& a_log_struct, std::string a_log_string);

		bool DoesLogApplyToFilter();

		void SnapToNewestMessage();

		std::vector<ParsedLogMessageStruct> _log_history;
		
		bool _have_filters_changed;
		bool _auto_scroll;

		int32 _default_history_limit;
		int32 _current_history_limit;

		int64 _log_entry_counter;

	};

} // End of namespace ~ DCore
