#include <Zynthurism/GameApplication.h>

#include <Editor/MainDockStage.h>
#include <Editor/Console/MainConsole.h>

#include <Zynthurism/GameMaster.h>

#include <Zynthurism/Game/GameStage.h>

namespace DGame
{

	void GameApplication::PreApplicationLoad()
	{
		RegisterStackCommunicator(std::make_shared<DGame::GameMaster>());
	}

	void GameApplication::PostApplicationLoad()
	{
		ProvideStageStackController().AttachStage<DEditor::DockerStage>("MainDockSpace", false);
		ProvideStageStackController().AttachStage<DEditor::MainConsole>("Console", false);
		//ProvideStageStackController().AttachStage<DCore::StageExample>("StageOne", false);
		//ProvideStageStackController().AttachStage<DCore::StageExample>("StageTwo", false);
		ProvideStageStackController().AttachStage<DGame::GameTestStage>("Game Test", false);

	}

} // End of namespace ~ DGame.
