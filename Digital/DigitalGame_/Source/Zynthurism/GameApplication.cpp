#include <Zynthurism/GameApplication.h>

#include <Editor/MainDockStage.h>
#include <Editor/Console/MainConsole.h>

#include <Zynthurism/GameMaster.h>

namespace DGame
{

	void GameApplication::PreApplicationLoad()
	{
	}

	void GameApplication::PostApplicationLoad()
	{
		RegisterStackCommunicator(std::make_shared<DGame::GameMaster>());

		ProvideStackController().AttachStage<DEditor::DockerStage>("MainDockSpace", false);
		ProvideStackController().AttachStage<DEditor::MainConsole>("Console", false);
		//ProvideStackController().AttachStage<DCore::StageExample>("StageOne", false);
		//ProvideStackController().AttachStage<DCore::StageExample>("StageTwo", false);

	}

} // End of namespace ~ DGame.
