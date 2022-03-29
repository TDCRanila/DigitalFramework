#pragma once

#include <Utility/AutoFactory/AutoFactory.h>

#include <CoreSystems/DUID.h>
#include <CoreSystems/Memory.h>

#include <string>

namespace DFW
{
	namespace DECS
	{
		// FW Declare
		class ECSModule;
		class Universe;
		class EntityManager;
		class SystemManager;

		class System : public DFactory::AutoFactory<System>
		{
		private:
			friend SystemManager;

		public:
			System(Key);
			virtual ~System() = default;

			virtual void Init();
			virtual void Terminate();

			virtual void PreUpdate(Universe& a_universe);
			virtual void Update(Universe& a_universe);
			virtual void PostUpdate(Universe & a_universe);

			virtual void UpdateSystemImGui(Universe& a_universe);

			DFW::DUID GetID() const;
			std::string GetName() const;
			bool IsSystemPaused() const;

		protected:
			void InternalInit();
			void InternalTerminate();

			void InternalPreUpdate(Universe& a_universe);
			void InternalUpdate(Universe& a_universe);
			void InternalPostUpdate(Universe& a_universe);

			void InternalPauseSystem(bool a_pause_on_true);

		protected:
			EntityManager* EntityManager() const;

		private:
			DFW::DUID _id;
			DFW::DECS::EntityManager*	_entity_manager;
			std::string _name;
			bool _paused;

		};

	} // End of namespace ~ DECS

} // End of namespace ~ DFW.
