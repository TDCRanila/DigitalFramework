#pragma once

#include <Autofactory/AutoFactory.h>

#include <CoreSystems/DUID.h>

#include <string>

namespace DECS 
{
	// FW Declare
	class ECSModule;
	class ECSUniverse;
	class ECSEntityManager;
	class ECSystemManager;

	class ECSystem : public DFactory::AutoFactory<ECSystem> 
	{
	public:
		ECSystem(Key);
		virtual ~ECSystem() = default;

		DCore::DUID GetID() const;
		std::string GetName() const;

		virtual void Init();
		virtual void Terminate();

		virtual void PreUpdate(ECSUniverse* const a_universe);
		virtual void Update(ECSUniverse* const a_universe);
		virtual void PostUpdate(ECSUniverse* const a_universe);

		bool IsSystemPaused() const;

	protected:
		friend class ECSystemManager;
		
		void InternalInit();
		void InternalTerminate();

		void InternalPreUpdate(ECSUniverse* const a_universe);
		void InternalUpdate(ECSUniverse* const a_universe);
		void InternalPostUpdate(ECSUniverse* const a_universe);

		void InternalPauseSystem(bool a_pause_on_true);

		DCore::DUID			_id;
		ECSEntityManager*	_entity_manager;
		std::string			_name;

	protected:
		ECSEntityManager* const EntityManager() const;

	private:
		bool				_paused;
	
	};

} // End of namespace ~ DECS
