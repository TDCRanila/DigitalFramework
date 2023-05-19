#pragma once

#include <Modules/Editor/EditorElementContainer.h>

#include <CoreSystems/Stage/Stage.h>

namespace DFW
{
	// FW Declare.
	class GameWorld;

	namespace DEditor
	{
		class EditorStage : public DFW::StageBase
		{
		public:
			EditorStage(std::string const& a_stage_name, bool a_start_disabled);
			~EditorStage() = default;

		protected:
			virtual void OnUpdate() override;
			virtual void OnRenderImGui() override;

			virtual void OnAttached() override;
			virtual void OnRemoved() override;
		
			inline SharedPtr<DFW::GameWorld> GameWorld() const { return _game_world; }

		private:
			void SetupEditorMenubar();
			void SetupDockingSpace();

		private:
			EditorElementContainer _element_container;
			SharedPtr<DFW::GameWorld> _game_world;

			bool _has_dockspace_been_created;

		};

	} // End of namespace ~ DFW

} // End of namespace ~ DFW.
