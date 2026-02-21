#include <Zynthurism/GameApplication.h>

#include <Zynthurism/Test/TestEditorStage.h>
#include <Zynthurism/Game/MainGameStage.h>

#include <DFW/Modules/Editor/Console/MainConsole.h>
#include <DFW/Modules/Editor/MainMenuBar.h>

#include <DFW/CoreSystems/CoreServices.h>

namespace DGame
{
	void GameApplication::PreApplicationInit()
	{
	}

	void GameApplication::PostApplicationInit()
	{
		// Stages
		DFW::CoreService::GetAppStageController()->AttachStage<DGame::MainGameStage>("MainGame", false);
		//DFW::CoreService::GetAppStageController()->AttachStage<DGame::TestEditorStage>("EditorDocker", false);
		DFW::CoreService::GetAppStageController()->AttachStage<DFW::DEditor::MainMenuBar>("MainMenuBar", false);
		DFW::CoreService::GetAppStageController()->AttachStage<DFW::DEditor::MainConsole>("Console", false);

	}

} // End of namespace ~ DGame.
