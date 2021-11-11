#pragma once

#include <Defines/Defines.h>

#include <Utility/TemplateUtility.h>

#include <CoreSystems/Logging/Logger.h>
#include <Modules/ECS/Objects/ECSComponent.h>
#include <Modules/ECS/Utility/ECSConcepts.h>
#include <Modules/ECS/Utility/ECSCompBitList.h>

#include <unordered_map>
#include <typeindex>

namespace DFW
{
	namespace DECS
	{
		const int64 DFW_UNASSIGNED_COMPONENT_BIT = -1;
		const int8	DFW_MAX_REGISTERED_COMPONENTS = 64;

		class KeyLockSystem final
		{
		public:
			KeyLockSystem() = default;
			~KeyLockSystem() = default;

			static void GenerateComponentKeys();

			template <typename... TArgs>
			ComponentBitList ConstructComponentBitList() const;

			template <typename TComponent>
			requires IsValidComponentType<TComponent>
				bool IsComponentBitTrue(ComponentBitList const& a_bit_var) const;

			template <typename TComponent>
			requires IsValidComponentType<TComponent>
				void SetComponentBits(ComponentBitList& a_bit_var) const;

			template <typename TComponent>
			requires IsValidComponentType<TComponent>
				void ResetComponentBits(ComponentBitList& a_bit_var) const;

			template <typename TComponent>
			requires IsValidComponentType<TComponent>
				int8 GetComponentBitPlacement() const;

			template <typename TComponent>
			requires IsValidComponentType<TComponent>
				void GetComponentBitPlacement(int8& a_component_bit_list) const;

		private:
			static std::unordered_map<std::type_index, int8> _component_bit_placement;
			static bool _generated_component_keys;

		};

#pragma region Template Function Implementation

		template <typename... TArgs>
		ComponentBitList KeyLockSystem::ConstructComponentBitList() const
		{
			if constexpr ((not IsValidComponentType2<TArgs> || ...))
			{
				static_assert(always_false<TArgs...>::value, __FUNCTION__ " - Trying to construct bitlist with a Component of type T that isn't derived from DECS::Component.");
				return ComponentBitList();
			}
			else
			{
				ComponentBitList comp_bit_list(0);
				(SetComponentBits<TArgs>(comp_bit_list), ...);
				return comp_bit_list;
			}
		}

		template <typename TComponent>
		requires IsValidComponentType<TComponent>
			bool KeyLockSystem::IsComponentBitTrue(ComponentBitList const& a_bit_var) const
		{
			int8 component_bit_placement(0);
			GetComponentBitPlacement<TComponent>(component_bit_placement);
			if (component_bit_placement != DFW_UNASSIGNED_COMPONENT_BIT)
			{
				int64 temp(1);
				temp = (temp << component_bit_placement);
				return (a_bit_var & temp);
			}
			else
			{
				return false;
			}
		}

		template <typename TComponent>
		requires IsValidComponentType<TComponent>
			void KeyLockSystem::SetComponentBits(ComponentBitList& a_bit_var) const
		{
			int64 temp(1);
			int8 component_bit_placement(0);
			GetComponentBitPlacement<TComponent>(component_bit_placement);
			temp = (temp << component_bit_placement);
			a_bit_var = (a_bit_var | temp);
		}

		template <typename TComponent>
		requires IsValidComponentType<TComponent>
			void KeyLockSystem::ResetComponentBits(ComponentBitList& a_bit_var) const
		{
			int64 temp(1);
			int8 component_bit_placement(0);
			GetComponentBitPlacement<TComponent>(component_bit_placement);
			temp = ~(temp << component_bit_placement);
			a_bit_var = (a_bit_var & temp);
		}

		template <typename TComponent>
		requires IsValidComponentType<TComponent>
			int8 KeyLockSystem::GetComponentBitPlacement() const
		{
			int8 temp_bit_list(0);
			GetComponentBitPlacement<TComponent>(temp_bit_list);
			return temp_bit_list;
		}

		template <typename TComponent>
		requires IsValidComponentType<TComponent>
			void KeyLockSystem::GetComponentBitPlacement(int8& a_component_bit_list) const
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

#pragma endregion

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
