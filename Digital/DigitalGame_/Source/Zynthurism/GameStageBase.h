#pragma once

#include <CoreSystems/Stage/Stage.h>

namespace DGame
{
	class GameStage : public DCore::StageBase
	{
	public:
		GameStage(std::string a_stage_name, bool a_start_disabled = false) :
			DCore::StageBase(a_stage_name, a_start_disabled)
		{
		}

		~GameStage() = default;

	private:


	};

} // End of namespace ~ DGame.
