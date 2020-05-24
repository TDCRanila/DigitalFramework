#pragma once

#include <Autofactory/AutoFactory.h>

#include <Modules/ECS/Utility/ECSIDManager.h>

#include <string>

namespace DECS 
{
	class ECSEntityManager;

	class ECSystem : public DFactory::AutoFactory<ECSystem> 
	{
	public:
		ECSystem(Key);
		virtual ~ECSystem();

		SystemID GetID() const;

		virtual void Init(ECSEntityManager* a_entity_manager);
		virtual void Terminate(ECSEntityManager* a_entity_manager);

		virtual void PreUpdate(ECSEntityManager* a_entity_manager);
		virtual void Update(ECSEntityManager* a_entity_manager, float32 a_delta_time);
		virtual void PostUpdate(ECSEntityManager* a_entity_manager);

		bool IsSystemPaused();

	protected:
		std::string system_name_;

	private:
		friend class ECSystemManager;

		void InternalInit(ECSEntityManager* a_entity_manager);
		void InternalTerminate();

		void InternalPreUpdate();
		void InternalUpdate();
		void InternalPostUpdate();

		void InternalPauseSystem(bool a_pause_on_true);

	private:
		SystemID system_id_;
		ECSEntityManager* entity_manager_;
		bool paused_;
	
	};

} // End of namespace ~ DECS
