#pragma once

#include <DFW/Modules/ECS/Utility/SystemTypeID.h>

#include <DFW/Utility/AutoFactory/AutoFactory.h>

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

		class System : public DFactory::AutoFactory<System>
		{
		private:
			friend SystemManager;

		public:
			System();
			virtual ~System() = default;

			DFW::DUID GetID() const { return _id; }
			SystemTypeID GetTypeID() const { return _type_id; }
			std::string const& GetName() const { return _name; }
			bool IsSystemPaused() const { return _paused; }

		public:
			void ExecuteBefore(System& a_system);
			void ExecuteAfter(System& a_system);
			void RemoveDependencies(System& a_system);

		protected:
			// Can be overwritten by derived class.
			virtual void Init(EntityRegistry& a_registry);
			virtual void Terminate(EntityRegistry& a_registry);

			virtual void PreUpdate(EntityRegistry& a_registry);
			virtual void Update(EntityRegistry& a_registry);
			virtual void PostUpdate(EntityRegistry& a_registry);

			virtual void UpdateSystemImGui(EntityRegistry& a_registry);

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
