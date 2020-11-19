#include <Zynthurism/GameApplication.h>

#include <Zynthurism/GameMaster.h>

#include <Zynthurism/Editor/EditorStage.h>

namespace DGame
{

	void GameApplication::PreApplicationLoad()
	{
	}

	void GameApplication::PostApplicationLoad()
	{
		RegisterStackCommunicator(std::make_shared<DGame::GameMaster>());

		ProvideStackController().AttachStage<DEditor::EditorStage>("Editor", false);
		ProvideStackController().AttachStage<DCore::StageExample>("StageOne", false);
		ProvideStackController().AttachStage<DCore::StageExample>("StageTwo", false);

	}

} // End of namespace ~ DGame.
