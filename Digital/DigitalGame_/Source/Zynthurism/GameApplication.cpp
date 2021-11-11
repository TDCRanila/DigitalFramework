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
		DFW::CoreService::GetECS()->SystemManager()->AddSystem<TestSystem>();

		// Stages
		ProvideStageStackController().AttachStage<DFW::DEditor::DockerStage>("MainDockSpace", false);
		ProvideStageStackController().AttachStage<DFW::DEditor::MainConsole>("Console", false);
		//ProvideStageStackController().AttachStage<DFW::StageExample>("StageOne", false);
		//ProvideStageStackController().AttachStage<DFW::StageExample>("StageTwo", false);
		ProvideStageStackController().AttachStage<DGame::TestStage>("Test Stage", false);

	}

} // End of namespace ~ DGame.
