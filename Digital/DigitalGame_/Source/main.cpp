#include <iostream>

#include <Autofactory/AutoFactory.h>

#include <CoreSystems/ApplicationInstance.h>
#include <CoreSystems/TimeTracker.h>

#include <Modules/ECS/ECSModule.h>
#include <Modules/ECS/Managers/ECSComponentManager.h>
#include <Modules/ECS/Managers/ECSEntityManager.h>
#include <Modules/ECS/Utility/ECSKeyLockSystem.h>

#include <Modules/ECS/Objects/ECSystem.h>
#include <Modules/ECS/Objects/ECSComponent.h>
#include <Modules/ECS/Objects/ECSComponentHandle.h>

#include <Defines/Defines.h>
#include <Defines/Defines.h>

#include <windows.h>

struct X : public DFactory::AutoFactory<DECS::ECSComponent>::Registrar<X>
{
	int32 variable;
};

struct Y : public DFactory::AutoFactory<DECS::ECSComponent>::Registrar<Y>
{
	int32 variable;
};

struct Z : public DFactory::AutoFactory<DECS::ECSComponent>::Registrar<Z>
{
	int32 variable;
};

class SystemOne : public DFactory::AutoFactory<DECS::ECSystem>::Registrar<SystemOne>
{
public:
	SystemOne()
	{
		this->system_name_ = "SystemOne";
	}
	~SystemOne() {}

	void Update(DECS::ECSEntityManager* /*a_entity_manager*/, float32 /*a_delta_time*/) override
	{
		DECS::ECSComponentHandle<X> x;
		DECS::ECSComponentHandle<Y> y;
		DECS::ECSComponentHandle<Z> z;
		//for (auto e : GetECSModule->EntitiesWith(x, y))
		//{
		//	// x & y are references.
		//	// check if valid for safety from ComponentHandle;
		//}
	}

};

class DigitalGameApplication final : public DCore::ApplicationInstance
{
public:
	DigitalGameApplication() {}
	~DigitalGameApplication() {}
};

int main(int /*argc*/, char** /*argv*/) 
{
	if (true)
	{
		DigitalGameApplication app;
		app.RunApplication();
	}
	else
	{
		INFOLOG("error" << std::to_string(0.5f));

		X x;
		DECS::ECSIDManager::GetComponentTypeID<X>();
		Y y;
		Z z;
		DECS::ECSModule ecs_module;

		ecs_module.InitECS();
		ecs_module.UpdateECS();
		ecs_module.TerminateECS();

		auto e1 = ecs_module.entity_manager_->CreateEntity();
		auto e = ecs_module.entity_manager_->CreateEntity();

		ecs_module.component_manager_->AddComponent<Z>(e.GetID());
		ecs_module.component_manager_->AddComponent<X>(e.GetID());

		//ecs_module.entity_manager_->GetEntity(0);
		//ecs_module.entity_manager_->GetEntity(1);

		////ecs_module.entity_manager_->DeleteEntity(0);
		////ecs_module.entity_manager_->DeleteEntity(1);

		//DECS::ComponentBitList a(0);
		//DECS::ComponentBitList b(0);
		//a = ecs_module.component_manager_->key_lock_system_->ConstructorBitList<X>();
		//b = ecs_module.component_manager_->key_lock_system_->ConstructorBitList<Y, X, Y>();

		//ecs_module.component_manager_->AddComponent<Z>(e.GetID());
		//ecs_module.component_manager_->AddComponent<Kappa>(e.GetID());
		//ecs_module.component_manager_->AddComponent<X>(e.GetID());
		////auto& cbl2 = ecs_module.entity_manager_->GetComponentBitList(e.GetID());
		////bool atest = ecs_module.component_manager_->HasComponents<X>(e.GetID());
		////bool btest = ecs_module.component_manager_->HasComponents<X, Y>(e.GetID());
		////bool ctest = ecs_module.component_manager_->HasComponents<X, Z, Y>(e.GetID());
		//ecs_module.component_manager_->DeleteComponent<Y>(e.GetID());
		//ecs_module.component_manager_->DeleteComponent<X>(e.GetID());
		//ecs_module.component_manager_->GetComponent<X>(e.GetID());
	}

}
