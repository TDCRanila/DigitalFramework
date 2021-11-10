#pragma once

#include <Autofactory/AutoFactory.h>

#include <CoreSystems/DUID.h>

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
		public:
			System(Key);
			virtual ~System() = default;

			DFW::DUID GetID() const;
			std::string GetName() const;

			virtual void Init();
			virtual void Terminate();

			virtual void PreUpdate(Universe* const a_universe);
			virtual void Update(Universe* const a_universe);
			virtual void PostUpdate(Universe* const a_universe);

			bool IsSystemPaused() const;

		protected:
			friend class SystemManager;

			void InternalInit();
			void InternalTerminate();

			void InternalPreUpdate(Universe* const a_universe);
			void InternalUpdate(Universe* const a_universe);
			void InternalPostUpdate(Universe* const a_universe);

			void InternalPauseSystem(bool a_pause_on_true);

			DFW::DUID		_id;
			EntityManager*	_entity_manager;
			std::string		_name;

		protected:
			EntityManager* const EntityManager() const;

		private:
			bool				_paused;

		};

	} // End of namespace ~ DECS

} // End of namespace ~ DFW.
