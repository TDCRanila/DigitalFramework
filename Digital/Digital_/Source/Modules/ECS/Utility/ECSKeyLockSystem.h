#pragma once

#include <Modules/ECS/Utility/ECSCompBitList.h>
#include <Modules/ECS/Objects/ECSComponent.h>

#include <Defines/Defines.h>

#include <Utility/TemplateUtility.h>

#include <unordered_map>
#include <typeindex>

namespace DECS 
{
	const int64 UNASSIGNED_COMPONENT_BIT = -1;

	class ECSKeyLockSystem final 
	{
	public:
		ECSKeyLockSystem();
		~ECSKeyLockSystem();

		void GenerateComponentKeys();

		template <typename... TArgs>
		ComponentBitList ConstructorBitList();
		
		template <class T>
		bool IsComponentBitTrue(ComponentBitList& a_bit_var) const;

		template <class T>
		void SetComponentBits(ComponentBitList& a_bit_var);

		template <class T>
		void ResetComponentBits(ComponentBitList& a_bit_var);

		template <class T>
		int8 GetComponentBitPlacement() const;

	private:
		template <class T>
		void InternalConstructorBitList(ComponentBitList& a_component_bit_list);

		template <class TVoid, class TComponent, class... TArgs>
		typename std::enable_if<std::is_base_of<ECSComponent, TComponent>::value && !is_same<ECSComponent, TComponent>::value, void>::type
		InternalConstructorBitList(ComponentBitList& a_component_bit_list);

		template <class TVoid, class TComponent, class... TArgs>
		typename std::enable_if<!std::is_base_of<ECSComponent, TComponent>::value || is_same<ECSComponent, TComponent>::value, void>::type
		InternalConstructorBitList(ComponentBitList& a_component_bit_list);

		bool generated_component_keys_;

		std::unordered_map<std::type_index, int8> component_bit_placement_;

	};

#pragma region Template Function Implementation

	template <typename... TArgs>
	ComponentBitList ECSKeyLockSystem::ConstructorBitList()
	{
		ComponentBitList comp_bit_list(0);
		InternalConstructorBitList<void, TArgs...>(comp_bit_list);
		return comp_bit_list;
	}

	template <class T>
	void ECSKeyLockSystem::InternalConstructorBitList(ComponentBitList& /*a_component_bit_list*/)
	{
		return;
	}

	template <class TVoid, class TComponent, class... TArgs>
	typename std::enable_if<std::is_base_of<ECSComponent, TComponent>::value && !is_same<ECSComponent, TComponent>::value, void>::type
	ECSKeyLockSystem::InternalConstructorBitList(ComponentBitList& a_component_bit_list)
	{
		SetComponentBits<TComponent>(a_component_bit_list);
		InternalConstructorBitList<TVoid, TArgs...>(a_component_bit_list);
	}

	template <class TVoid, class TComponent, class... TArgs>
	typename std::enable_if<!std::is_base_of<ECSComponent, TComponent>::value || is_same<ECSComponent, TComponent>::value, void>::type
	ECSKeyLockSystem::InternalConstructorBitList(ComponentBitList& a_component_bit_list)
	{
		static_assert(always_false<TComponent>::value, __FUNCTION__ " - Trying to construct bitilsit with a Component of type T that isn't derived from DECS::ECSComponent.");
		return;
	}

	template <class T>
	bool ECSKeyLockSystem::IsComponentBitTrue(ComponentBitList& a_bit_var) const 
	{
		auto& type	= typeid(T);
		auto it		= component_bit_placement_.find(type);
		if (it != component_bit_placement_.end()) 
		{
			int64 temp (1);
			temp = (temp << it->second);
			// TODO - Compare bit, not final value.
			return (a_bit_var & temp);
		} 
		else 
		{
			DFW_ERRORLOG("Unable to find ComponentBitPlacement of T: " << type.name());
			return false;
		}
	}

	template <class T>
	void ECSKeyLockSystem::SetComponentBits(ComponentBitList& a_bit_var)
	{
		int64	temp (1);
		int8	component_bit_placement = GetComponentBitPlacement<T>();
		temp		= (temp << component_bit_placement);
		a_bit_var	= (a_bit_var | temp);
	}

	template <class T>
	void ECSKeyLockSystem::ResetComponentBits(ComponentBitList& a_bit_var) 
	{
		int64	temp(1);
		int8	component_bit_placement(GetComponentBitPlacement<T>());
		temp		= ~(temp << component_bit_placement);
		a_bit_var	=  (a_bit_var & temp);
	}

	template <class T>
	int8 ECSKeyLockSystem::GetComponentBitPlacement() const 
	{
		auto& type	= typeid(T);
		auto it		= component_bit_placement_.find(type);
		if (it != component_bit_placement_.end())
		{
			return it->second;
		} 
		else 
		{
			DFW_ERRORLOG("Unable to find ComponentBitPlacement of T: " << type.name());
			return UNASSIGNED_COMPONENT_BIT;
		}
	}

#pragma endregion

} // End of namespace ~ DECS
