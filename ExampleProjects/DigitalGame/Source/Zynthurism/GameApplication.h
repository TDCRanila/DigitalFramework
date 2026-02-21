#pragma once

#include <DFW/CoreSystems/ApplicationInstance.h>

namespace DGame
{

	class GameApplication final : public DFW::ApplicationInstance
	{
	public:
		GameApplication() = default;
		~GameApplication() = default;
		
	private:
		virtual void PreApplicationInit() override;
		virtual void PostApplicationInit() override;

	};

} // End of namespace DGame.
