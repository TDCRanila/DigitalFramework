#include <Editor/Console/MainConsole.h>

#include <imgui/imgui.h>

namespace DFW
{
    namespace DEditor
    {
        MainConsole::MainConsole(std::string a_stage_name, bool a_start_disabled)
            : DFW::StageBase(a_stage_name, a_start_disabled)
            , _have_filters_changed(false)
            , _default_history_limit(500)
            , _current_history_limit(_default_history_limit)
            , _log_entry_counter(0)
            , _auto_scroll(true)
        {
            // Subscribe to the logger.
            DFW::Logger::AddSubscriber(this->GetID(), DFW_BIND_FUNC(LogMessageCallback));

            _log_history.reserve(_current_history_limit);

        }

        MainConsole::~MainConsole()
        {
            // Formally unsubscribe from the logger.
            DFW::Logger::RemoveSubscriber(this->GetID());
        }

        void MainConsole::RenderImGui()
        {
            ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_FirstUseEver);
            ImGui::Begin(GetName().c_str());

            ImGuiStyle& style = ImGui::GetStyle();
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, (float)(int)(style.FramePadding.y * 0.45f)));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, (float)(int)(style.ItemSpacing.y * 0.45f)));

            const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
            ImGui::BeginChild("ConsoleScroll", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

            for (ParsedLogMessageStruct& log : _log_history)
            {
                if (log._show_message)
                {
                    ImGui::TextUnformatted(log._original_log_string.c_str());
                    std::string pop_up_identifier = "LogEntryContextMenu" + std::to_string(log._num_entry);
                    if (ImGui::BeginPopupContextItem(pop_up_identifier.c_str()))
                    {
                        if (ImGui::Selectable("Copy Text")) { ImGui::SetClipboardText(log._original_log_string.c_str()); }
                        ImGui::EndPopup();
                    }
                }
            }

            // Scroll to newest message.
            if (_auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();

            ImGui::PopStyleVar(2);

            ImGui::End();
        }

        void MainConsole::Update()
        {
            if (_have_filters_changed)
            {
                // apply show or not
            }
        }

        void MainConsole::OnAttached()
        {
        }

        void MainConsole::OnRemoved()
        {
        }

        void MainConsole::ParseLogMessage(ParsedLogMessageStruct& a_log_struct, std::string a_log_string)
        {
            a_log_struct._original_log_string = a_log_string;
            a_log_struct._num_entry = _log_entry_counter++;
        }

        bool MainConsole::DoesLogApplyToFilter()
        {
            return true;
        }

        void MainConsole::LogMessageCallback(std::string a_log_message)
        {
            ParsedLogMessageStruct& log = _log_history.emplace_back();

            ParseLogMessage(log, a_log_message);
        }

    } // End of namespace ~ DEditor.

} // End of namespace ~ DFW.
