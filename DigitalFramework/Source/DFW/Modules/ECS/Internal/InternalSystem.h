#pragma once

#include <DFW/Modules/ECS/Utility/SystemTypeID.h>

#include <DFW/CoreSystems/DUID.h>

#include <string>

namespace DFW
{
	// FW Declare
	class EventDispatcher;

	namespace DECS
	{
		class EntityRegistry;
		class SystemManager;

		class InternalSystem
		{
		private:
			friend SystemManager;

		public:
			InternalSystem();
			virtual ~InternalSystem() = default;

			DFW::DUID GetID() const { return _id; }
			SystemTypeID GetTypeID() const { return _type_id; }
			std::string const& GetName() const { return _name; }
			bool IsSystemPaused() const { return _paused; }

		protected:
			// Can be overwritten by derived class.
			virtual void Init(EntityRegistry& a_registry) = 0;
			virtual void Terminate(EntityRegistry& a_registry) = 0;

			virtual void PreUpdate(EntityRegistry& a_registry) = 0;
			virtual void Update(EntityRegistry& a_registry) = 0;
			virtual void PostUpdate(EntityRegistry& a_registry) = 0;

			virtual void UpdateSystemImGui(EntityRegistry& a_registry) = 0;

			inline DFW::DECS::SystemManager& SystemManager() const { return *_system_manager; }
			inline DFW::EventDispatcher& ECSEventHandler() const { return *_event_handler; }

		private:
			// Called by ECSystemManager.
			void InternalInit(EntityRegistry& a_registry);
			void InternalTerminate(EntityRegistry& a_registry);

			void InternalPreUpdate(EntityRegistry& a_registry);
			void InternalUpdate(EntityRegistry& a_registry);
			void InternalPostUpdate(EntityRegistry& a_registry);

			void InternalPauseSystem(bool a_pause_on_true);

		private:
			DFW::DECS::SystemManager* _system_manager;
			DFW::EventDispatcher* _event_handler;

			DFW::DUID	_id;
			SystemTypeID _type_id;
			std::string _name;
			bool		_paused;

		};

	} // End of namespace ~ DECS

} // End of namespace ~ DFW.
