#include <Zynthurism/GameApplication.h>

#include <Zynthurism/Test/TestEditorStage.h>

#include <Modules/Editor/Console/MainConsole.h>
#include <Modules/Editor/MainMenuBar.h>

#include <CoreSystems/CoreServices.h>

namespace DGame
{
	void GameApplication::PreApplicationInit()
	{
	}

	void GameApplication::PostApplicationInit()
	{
		// Stages
		DFW::CoreService::GetAppStageController()->AttachStage<DGame::TestEditorStage>("EditorDocker", false);
		DFW::CoreService::GetAppStageController()->AttachStage<DFW::DEditor::MainMenuBar>("MainMenuBar", false);
		DFW::CoreService::GetAppStageController()->AttachStage<DFW::DEditor::MainConsole>("Console", false);

	}

} // End of namespace ~ DGame.
