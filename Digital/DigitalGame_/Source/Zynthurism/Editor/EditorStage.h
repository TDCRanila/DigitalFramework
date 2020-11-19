#pragma once

#include <Zynthurism/GameStageBase.h>

namespace DEditor
{
	class EditorStage : public DGame::GameStage
	{
	public:
		EditorStage(std::string a_stage_name, bool a_start_disabled = false)
			: DGame::GameStage(a_stage_name, a_start_disabled)
			, _main_editor_id("MainEditorWindow")
			, _hide_main_editor(true)
		{
		}
			
		~EditorStage() = default;

		// dockspace, toolbar
		void InitEditor();

		void BeginEditorRender();
		void EndEditorRender();

		void AddEditorElement();
		void RemoveEditorElement();

		virtual void RenderImGui() override;

		virtual void Update() override
		{

		}

		virtual void OnAttached() override
		{
			InitEditor();
		}

		virtual void OnRemoved() override
		{

		}

		const char* const _main_editor_id;
		bool _hide_main_editor;

	private:

	};

} // End of namespace ~ DEditor.
