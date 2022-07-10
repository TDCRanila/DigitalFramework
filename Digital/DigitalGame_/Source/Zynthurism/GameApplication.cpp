#include <Zynthurism/GameApplication.h>

#include <Zynthurism/GameMaster.h>
#include <Zynthurism/Game/TestStage.h>

#include <GameWorld/Camera/CameraSystem.h>
#include <GameWorld/Editor/EditorDockerStage.h>
#include <GameWorld/Graphics/RenderSystem.h>
#include <GameWorld/Graphics/SpriteSystem.h>

#include <Modules/Editor/Console/MainConsole.h>
#include <Modules/Editor/MainMenuBar.h>

#include <CoreSystems/CoreServices.h>

#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/Managers/ECSystemManager.h>

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
		ProvideStageStackController().AttachStage<DFW::DEditor::EditorDocker>("EditorDocker", false);
		ProvideStageStackController().AttachStage<DFW::DEditor::MainMenuBar>("MainMenuBar", false);
		ProvideStageStackController().AttachStage<DFW::DEditor::MainConsole>("Console", false);
		ProvideStageStackController().AttachStage<DGame::TestStage>("Test Stage", false);

	}

} // End of namespace ~ DGame.
