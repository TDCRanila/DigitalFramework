#pragma once

#include <Autofactory/AutoFactory.h>

#include <ECS/Utility/ECSIDManager.h>

namespace DECS {

	class ECSystem : public DFactory::AutoFactory<ECSystem> {
	public:
		ECSystem(Key);
		virtual ~ECSystem();

		SystemID GetID() const;

		virtual void Init();
		virtual void Terminate();

		virtual void PreUpdate();
		virtual void Update();
		virtual void PostUpdate();

	private:
		SystemID system_id_;
	
	};

} // End of namespace ~ DECS