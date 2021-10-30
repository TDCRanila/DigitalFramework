#include <Zynthurism/GameApplication.h>

#include <Zynthurism/GameMaster.h>

#include <Zynthurism/Game/TestStage.h>
#include <Zynthurism/Game/GameplaySystems/TestSystem.h>

#include <CoreSystems/CoreServices.h>

#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/Managers/ECSystemManager.h>

#include <Editor/MainDockStage.h>
#include <Editor/Console/MainConsole.h>

namespace DGame
{
	void GameApplication::PreApplicationInit()
	{
		RegisterStageStackCommunicator(std::make_shared<DGame::GameMaster>());
		
	}

	void GameApplication::PostApplicationInit()
	{
		// Systems
		CoreService::GetECS()->SystemManager()->AddSystem<TestSystem>();

		// Stages
		ProvideStageStackController().AttachStage<DEditor::DockerStage>("MainDockSpace", false);
		ProvideStageStackController().AttachStage<DEditor::MainConsole>("Console", false);
		//ProvideStageStackController().AttachStage<DCore::StageExample>("StageOne", false);
		//ProvideStageStackController().AttachStage<DCore::StageExample>("StageTwo", false);
		ProvideStageStackController().AttachStage<DGame::TestStage>("Test Stage", false);

	}

} // End of namespace ~ DGame.
