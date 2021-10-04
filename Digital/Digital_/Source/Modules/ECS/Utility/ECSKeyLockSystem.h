#pragma once

#include <Modules/ECS/Utility/ECSCompBitList.h>
#include <Modules/ECS/Objects/ECSComponent.h>

#include <Defines/Defines.h>

#include <Utility/TemplateUtility.h>

#include <CoreSystems/Logging/Logger.h>

#include <unordered_map>
#include <typeindex>

namespace DECS 
{
	const int64 DFW_UNASSIGNED_COMPONENT_BIT	= -1;
	const int8	DFW_MAX_REGISTERED_COMPONENTS	= 64;

	// Concepts
	// TODO TEMPORARY SECOND CONCEPT FOR COMPONENT TYPE CHECK - NEEDS TO BE REPLACEMENT
	template <typename ComponentType>
	concept IsValidComponentType2 = IsDerivedFrom<ComponentType, ECSComponent>;

	class ECSKeyLockSystem final 
	{
	public:
		ECSKeyLockSystem() = default;
		~ECSKeyLockSystem() = default;

		void GenerateComponentKeys();

		template <typename... TArgs>
		ComponentBitList ConstructComponentBitList() const;
		
		template <typename TComponent>
		requires IsValidComponentType2<TComponent>
		bool IsComponentBitTrue(ComponentBitList const& a_bit_var) const;

		template <typename TComponent>
		requires IsValidComponentType2<TComponent>
		void SetComponentBits(ComponentBitList& a_bit_var) const;

		template <typename TComponent>
		requires IsValidComponentType2<TComponent>
		void ResetComponentBits(ComponentBitList& a_bit_var) const;

		template <typename TComponent>
		requires IsValidComponentType2<TComponent>
		int8 GetComponentBitPlacement() const;

		template <typename TComponent>
		requires IsValidComponentType2<TComponent>
		void GetComponentBitPlacement(int8& a_component_bit_list) const;

	private:
		template <typename TComponent, typename... TArgs>
		void InternalConstructComponentBitList(ComponentBitList& a_component_bit_list) const;

		static bool _generated_component_keys;
		static std::unordered_map<std::type_index, int8> _component_bit_placement;

	};

#pragma region Template Function Implementation

	template <typename... TArgs>
	ComponentBitList ECSKeyLockSystem::ConstructComponentBitList() const
	{
		ComponentBitList comp_bit_list(0);
		InternalConstructComponentBitList<TArgs...>(comp_bit_list);
		return comp_bit_list;
	}

	template <typename TComponent>
	requires IsValidComponentType2<TComponent>
	bool ECSKeyLockSystem::IsComponentBitTrue(ComponentBitList const& a_bit_var) const 
	{
		int8 component_bit_placement(0);
		GetComponentBitPlacement<TComponent>(component_bit_placement);
		if (component_bit_placement != DFW_UNASSIGNED_COMPONENT_BIT) 
		{
			int64 temp (1);
			temp = (temp << component_bit_placement);
			return (a_bit_var & temp);
		} 
		else
		{
			return false;
		}
	}

	template <typename TComponent>
	requires IsValidComponentType2<TComponent>
	void ECSKeyLockSystem::SetComponentBits(ComponentBitList& a_bit_var) const
	{
		int64 temp (1);
		int8 component_bit_placement(0);
		GetComponentBitPlacement<TComponent>(component_bit_placement);
		temp		= (temp << component_bit_placement);
		a_bit_var	= (a_bit_var | temp);
	}

	template <typename TComponent>
	requires IsValidComponentType2<TComponent>
	void ECSKeyLockSystem::ResetComponentBits(ComponentBitList& a_bit_var) const
	{
		int64 temp(1);
		int8 component_bit_placement(0);
		GetComponentBitPlacement<TComponent>(component_bit_placement);
		temp		= ~(temp << component_bit_placement);
		a_bit_var	=  (a_bit_var & temp);
	}

	template <typename TComponent>
	requires IsValidComponentType2<TComponent>
	int8 ECSKeyLockSystem::GetComponentBitPlacement() const 
	{
		int8 temp_bit_list(0);
		GetComponentBitPlacement<TComponent>(temp_bit_list);
		return temp_bit_list;
	}

	template <typename TComponent>
	requires IsValidComponentType2<TComponent>
	void ECSKeyLockSystem::GetComponentBitPlacement(int8& a_component_bit_list) const
	{
		const type_info& type = typeid(TComponent);
		auto it = _component_bit_placement.find(type);
		if (it != _component_bit_placement.end())
		{
			a_component_bit_list = it->second;
		}
		else
		{
			DFW_ERRORLOG("Unable to find ComponentBitPlacement of TComponent: ", type.name());
			a_component_bit_list = DFW_UNASSIGNED_COMPONENT_BIT;
		}
	}

	template <typename TComponent, typename... TArgs>
	void ECSKeyLockSystem::InternalConstructComponentBitList(ComponentBitList& a_component_bit_list) const
	{
		if constexpr (not IsValidComponentType2<TComponent>)
		{
			static_assert(always_false<TComponent>::value, __FUNCTION__ " - Trying to construct bitlist with a Component of type T that isn't derived from DECS::ECSComponent.");
			return;
		}
		else if constexpr (sizeof...(TArgs) == 0)
		{
			SetComponentBits<TComponent>(a_component_bit_list);
		}
		else
		{
			SetComponentBits<TComponent>(a_component_bit_list);
			InternalConstructorBitList<TArgs...>(a_component_bit_list);
		}
	}

#pragma endregion

} // End of namespace ~ DECS
