#pragma once

#include <CoreSystems/Stage/Stage.h>

namespace DGame
{
	class GameStageBase : public DCore::StageBase
	{
	public:
		GameStageBase(std::string a_stage_name, bool a_start_disabled = false) :
			DCore::StageBase(a_stage_name, a_start_disabled)
		{
		}

		~GameStageBase() = default;

	private:

	};

} // End of namespace ~ DGame.
