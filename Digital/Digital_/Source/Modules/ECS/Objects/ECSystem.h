#pragma once

#include <Utility/AutoFactory/AutoFactory.h>

#include <CoreSystems/DUID.h>
#include <CoreSystems/Memory.h>

#include <string>

namespace DFW
{
	// FW Declare
	class EventDispatcher;
	
	namespace DECS
	{
		class Universe;
		class SystemManager;
		class EntityManager;

		class System : public DFactory::AutoFactory<System>
		{
		private:
			friend SystemManager;

		public:
			System();
			virtual ~System() = default;

			DFW::DUID GetID() const;
			std::string GetName() const;
			bool IsSystemPaused() const;

		protected:
			// Can be overwritten by derived class.
			virtual void Init();
			virtual void Terminate();

			virtual void PreUpdate(Universe& a_universe);
			virtual void Update(Universe& a_universe);
			virtual void PostUpdate(Universe& a_universe);

			virtual void UpdateSystemImGui(Universe& a_universe);

			inline DFW::DECS::SystemManager& SystemManager() const { return *_system_manager; }
			inline DFW::DECS::EntityManager& EntityManager() const { return *_entity_manager; }
			inline DFW::EventDispatcher& ECSEventHandler() const { return *_event_handler; }

		private:
			// Called by ECSystemManager.
			void InternalInit();
			void InternalTerminate();

			void InternalPreUpdate(Universe& a_universe);
			void InternalUpdate(Universe& a_universe);
			void InternalPostUpdate(Universe& a_universe);

			void InternalPauseSystem(bool a_pause_on_true);

		private:
			DFW::DECS::SystemManager* _system_manager;
			DFW::DECS::EntityManager* _entity_manager;
			DFW::EventDispatcher* _event_handler;

			DFW::DUID	_id;
			std::string _name;
			bool		_paused;

		};

	} // End of namespace ~ DECS

} // End of namespace ~ DFW.
