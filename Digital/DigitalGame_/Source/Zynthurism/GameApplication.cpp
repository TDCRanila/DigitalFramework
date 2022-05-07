#include <Zynthurism/GameApplication.h>

#include <Zynthurism/GameMaster.h>
#include <Zynthurism/Game/TestStage.h>

#include <CoreSystems/CoreServices.h>

#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/Managers/ECSystemManager.h>

#include <GameWorld/Camera/CameraSystem.h>
#include <GameWorld/Graphics/RenderSystem.h>
#include <GameWorld/Graphics/SpriteSystem.h>

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
		DFW::CoreService::GetECS()->SystemManager().AddSystem<DFW::RenderSystem>();
		DFW::CoreService::GetECS()->SystemManager().AddSystem<DFW::SpriteSystem>();
		DFW::CoreService::GetECS()->SystemManager().AddSystem<DFW::CameraSystem>();

		// Stages
		ProvideStageStackController().AttachStage<DFW::DEditor::DockerStage>("MainDockSpace", false);
		ProvideStageStackController().AttachStage<DFW::DEditor::MainConsole>("Console", false);
		ProvideStageStackController().AttachStage<DGame::TestStage>("Test Stage", false);

	}

} // End of namespace ~ DGame.
