#pragma once

#include <CoreSystems/ApplicationInstance.h>

namespace DGame
{

	class GameApplication final : public DCore::ApplicationInstance
	{
	public:
		GameApplication() = default;
		~GameApplication() = default;
		
	private:
		virtual void PreApplicationInit() override;
		virtual void PostApplicationInit() override;

	};

} // End of namespace DGame.
