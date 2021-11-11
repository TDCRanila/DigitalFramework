#pragma once

#include <CoreSystems/Stage/Stage.h>

namespace DGame
{
	class GameStageBase : public DFW::StageBase
	{
	public:
		GameStageBase(std::string a_stage_name, bool a_start_disabled = false) :
			DFW::StageBase(a_stage_name, a_start_disabled)
		{
		}

		~GameStageBase() = default;

	private:

	};

} // End of namespace ~ DGame.
