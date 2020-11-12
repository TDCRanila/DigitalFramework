#include <Zynthurism/GameApplication.h>

#include <Zynthurism/GameMaster.h>

namespace DGame
{

	void GameApplication::PreApplicationLoad()
	{
	}

	void GameApplication::PostApplicationLoad()
	{
		RegisterStackCommunicator(std::make_shared<DGame::GameMaster>());

		ProvideStackController().AttachStage<DCore::StageExample>("StageOne", false);
		ProvideStackController().AttachStage<DCore::StageExample>("StageTwo", false);
		ProvideStackController().AttachStage<DCore::StageExample>("StageThree", false);
	}

} // End of namespace ~ DGame.
