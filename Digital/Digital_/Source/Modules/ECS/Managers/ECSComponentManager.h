#pragma once

#include <Modules/ECS/Managers/ECSEntityManager.h>
#include <Modules/ECS/Utility/ECSKeyLockSystem.h>

#include <Modules/ECS/Objects/ECSEntity.h>
#include <Modules/ECS/Objects/ECSComponent.h>
#include <Modules/ECS/Objects/ECSComponentHandle.h>

#include <Utility/TemplateUtility.h>

#include <memory>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <type_traits>

namespace DECS 
{
	class ECSComponentManager final 
	{
	public:
		ECSComponentManager();
		~ECSComponentManager();

		void Init(ECSEntityManager* a_entity_manager);

		void Terminate();

	// TODO Make them protected again - Was testing out things: - protected:
		template <class T> 
		typename std::enable_if<std::is_base_of<ECSComponent, T>::value && !is_same<ECSComponent, T>::value, ECSComponentHandle<T>>::type
		GetComponent(EntityID a_entity_id) const;

		template <class T>
		typename std::enable_if<!std::is_base_of<ECSComponent, T>::value || is_same<ECSComponent, T>::value, ECSComponentHandle<T>>::type
		GetComponent(EntityID a_entity_id) const;

		template <class ...TArgs>
		bool HasComponents(EntityID a_entity_id) const;

		template <class T>
		typename std::enable_if<std::is_base_of<ECSComponent, T>::value && !is_same<ECSComponent, T>::value, ECSComponentHandle<T>>::type
		AddComponent(EntityID a_entity_id);

		template <class T>
		typename std::enable_if<!std::is_base_of<ECSComponent, T>::value || is_same<ECSComponent, T>::value, ECSComponentHandle<T>>::type
		AddComponent(EntityID a_entity_id);

		template <class T>
		typename std::enable_if<std::is_base_of<ECSComponent, T>::value, bool>::type
		DeleteComponent(EntityID a_entity_id);

		template <class T>
		typename std::enable_if<!std::is_base_of<ECSComponent, T>::value, bool>::type
		DeleteComponent(EntityID a_entity_id);

	// TODO Make them private again - Was testing out things: - private:
		template <class T>
		typename std::enable_if<std::is_base_of<ECSComponent, T>::value, bool>::type
		InternalHasComponents(EntityID a_entity_id) const;

		template <class T>
		typename std::enable_if<!std::is_base_of<ECSComponent, T>::value, bool>::type
		InternalHasComponents(EntityID a_entity_id) const;

		template <class Ta, class Tb, class... TArgs>
		bool InternalHasComponents(EntityID a_entity_id) const;

		ECSEntityManager* entity_manager_;
		std::unique_ptr<ECSKeyLockSystem>	key_lock_system_;

		std::unordered_map<std::type_index, std::unordered_map<EntityID, ECSComponent*>> component_vectors_;

	};

#pragma region Template Function Implementation

	template <class T>
	typename std::enable_if<std::is_base_of<ECSComponent, T>::value && !is_same<ECSComponent, T>::value, ECSComponentHandle<T>>::type
	ECSComponentManager::GetComponent(EntityID a_entity_id) const
	{
		const auto& handle = entity_manager_->GetEntity(a_entity_id);
		if (!handle.IsEntityValid())
		{
			return ECSComponentHandle<T>(nullptr);
		}
		else
		{
			if (!HasComponents<T>(a_entity_id))
			{
				return ECSComponentHandle<T>(nullptr);
			}
			else
			{
				const ECSComponent* component_ptr(component_vectors_.at(typeid(T)).at(handle.GetID()));
				return ECSComponentHandle<T>(static_cast<const T*>(component_ptr));
			}
		}
	}

	template <class T>
	typename std::enable_if<!std::is_base_of<ECSComponent, T>::value || is_same<ECSComponent, T>::value, ECSComponentHandle<T>>::type
	ECSComponentManager::GetComponent(EntityID /*a_entity_id*/) const
	{
		static_assert(always_false<T>::value, __FUNCTION__ " - Trying to get a Component of type T that isn't derived from DECS::ECSComponent.");
		return ECSComponentHandle<T>(nullptr);
	}

	template <class ...TArgs>
	bool ECSComponentManager::HasComponents(EntityID a_entity_id) const
	{
		// TODO. ECSComponent when checkinf BitComponent returns false.
		return InternalHasComponents<ECSComponent, TArgs...>(a_entity_id);
	}
	
	template <class T>
	typename std::enable_if<std::is_base_of<ECSComponent, T>::value, bool>::type
	ECSComponentManager::InternalHasComponents(EntityID a_entity_id) const
	{
		if (is_same<ECSComponent, T>())
		{
			return true;
		}

		const auto& entity = entity_manager_->GetEntity(a_entity_id);
		if (!entity.IsEntityValid())
		{
			return false;
		}
		else
		{
			ComponentBitList& entity_bit_list(entity_manager_->GetComponentBitList(a_entity_id));
			return key_lock_system_->IsComponentBitTrue<T>(entity_bit_list);
		}
	}

	template <class T>
	typename std::enable_if<!std::is_base_of<ECSComponent, T>::value, bool>::type
	ECSComponentManager::InternalHasComponents(EntityID /*a_entity_id*/) const
	{
		static_assert(always_false<T>::value, __FUNCTION__ " - Trying to check for a Component of type T that isn't derived from DECS::ECSComponent.");
		return false;
	}

	template <class Ta, class Tb, class... TArgs>
	bool ECSComponentManager::InternalHasComponents(EntityID a_entity_id) const
	{
		return InternalHasComponents<Tb>(a_entity_id) && InternalHasComponents<Ta, TArgs...>(a_entity_id);
	}

	template <class T>
	typename std::enable_if<std::is_base_of<ECSComponent, T>::value && !is_same<ECSComponent, T>::value, ECSComponentHandle<T>>::type
	ECSComponentManager::AddComponent(EntityID a_entity_id)
	{
		// TODO - Find Component in storage.
		key_lock_system_->SetComponentBits<T>(entity_manager_->GetComponentBitList(a_entity_id));
		return ECSComponentHandle<T>(nullptr);
	}

	template <class T>
	typename std::enable_if<!std::is_base_of<ECSComponent, T>::value || is_same<ECSComponent, T>::value, ECSComponentHandle<T>>::type
	ECSComponentManager::AddComponent(EntityID /*a_entity_id*/)
	{
		static_assert(always_false<T>::value, __FUNCTION__ " - Trying to add a Component of type T that isn't derived from DECS::ECSComponent.");
		return ECSComponentHandle<T>(nullptr);
	}

	template <class T>
	typename std::enable_if<std::is_base_of<ECSComponent, T>::value, bool>::type
	ECSComponentManager::DeleteComponent(EntityID /*a_entity_id*/)
	{
		// TODO
		return false;
	}

	template <class T>
	typename std::enable_if<!std::is_base_of<ECSComponent, T>::value, bool>::type
	ECSComponentManager::DeleteComponent(EntityID /*a_entity_id*/)
	{
		static_assert(always_false<T>::value, __FUNCTION__ " - Trying to delete a Component of type T that isn't derived from DECS::ECSComponent.");
		return false;
	}

#pragma endregion

} // End of namespace ~ DECS
