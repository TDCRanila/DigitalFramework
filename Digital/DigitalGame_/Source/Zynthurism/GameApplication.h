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
		virtual void PreApplicationLoad() override;
		virtual void PostApplicationLoad() override;

	};

} // End of namespace DGame.
