#pragma once

#include <DFW/CoreSystems/Logging/Logger.h>
#include <DFW/CoreSystems/Stage/Stage.h>

#include <string>
#include <vector>

namespace DFW
{
	namespace DEditor
	{
		class MainConsole : public DFW::StageBase
		{
		public:
			MainConsole(std::string const& a_stage_name, bool a_start_disabled);
			~MainConsole();

			void LogMessageCallback(std::string a_log_message);

		private:
			virtual void OnUpdate() override;
			virtual void OnRenderImGui() override;

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

	} // End of namespace ~ DEditor.

} // End of namespace ~ DFW.
