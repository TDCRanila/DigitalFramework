#pragma once

#include <CoreSystems/Memory.h>

#include <unordered_map>
#include <string>

namespace DFW
{
	// FW Declare.
	class EventDispatcher;

	namespace DECS
	{
		// FW Declares.
		class SystemManager;
		class EntityRegistry;

		class ECSModule final
		{
		public:
			ECSModule();
			~ECSModule();

			void Init();
			void Terminate();
			void UpdateECS();
			void UpdateECSImGui();

		public:
			inline SystemManager& SystemManager() const { return *_system_manager; }
			inline EventDispatcher& EventHandler() const { return *_event_handler; }
			inline EntityRegistry& GetRegistry() const { return *_registry; }

		private:
			UniquePtr<DFW::DECS::SystemManager>	_system_manager;
			UniquePtr<EventDispatcher>			_event_handler;
			UniquePtr<EntityRegistry>			_registry;
			bool _initialized;

		};

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
