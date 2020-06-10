#pragma once

#include <Modules/ECS/Utility/ECSIDManager.h>

#include <Modules/ECS/Objects/ECSComponentHandle.h>
#include <Modules/ECS/Utility/ECSCompBitList.h>

#include <tuple>

namespace DECS 
{
	class ECSEntityManager;
	class ECSComponentManager;

	class ECSEntity final 
	{
	public:
		ECSEntity();
		~ECSEntity();

		EntityID GetID() const;
		
		bool IsEntityValid() const;

		bool IsPendingDeletion() const;

		template <class T>
		ECSComponentHandle<T> AddComponent() const;

		template <class T>
		bool RemoveComponent() const;

		template <class T>
		ECSComponentHandle<T> GetComponent() const;

		template <class ...T>
		std::tuple<ECSComponentHandle<T>...> GetComponents() const;

	protected:
		friend class ECSEntityManager;
		ECSEntity(EntityID a_entity_id, ECSEntityManager* a_entity_manager);

	private:
		EntityID			entity_id_;
		ECSEntityManager*	entity_manager_;

	};

#pragma region Template Function Implementation

	template <class T>
	ECSComponentHandle<T> ECSEntity::AddComponent() const
	{
		// TODO
	}

	template <class T>
	bool ECSEntity::RemoveComponent() const
	{
		// TODO
	}

	template <class T>
	ECSComponentHandle<T> ECSEntity::GetComponent() const
	{
		// TODO
	}

	template <class ...T>
	std::tuple<ECSComponentHandle<T>...> ECSEntity::GetComponents() const
	{
		// TODO
	}

#pragma endregion

} // End of namespace ~ DECS
