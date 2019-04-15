#pragma once

#include <ECS/Utility/ECSCompBitList.h>

#include <Defines/Defines.h>

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
		
		template <class T>
		bool IsComponentBitTrue(ComponentBitList& a_bit_var) const;

		template <class T>
		void SetComponentBits(ComponentBitList& a_bit_var);

		template <class T>
		void ResetComponentBits(ComponentBitList& a_bit_var);

		template <class T>
		int8 GetComponentBitPlacement() const;

	private:
		std::unordered_map<std::type_index, int8> component_bit_placement_;

	};

#pragma region Template Function Implementation

	template <class T>
	bool ECSKeyLockSystem::IsComponentBitTrue(ComponentBitList& a_bit_var) const 
	{
		auto type	= typeid(T);
		auto it		= component_bit_placement_.find(type);
		if (it != component_bit_placement_.end()) 
		{
			int64 temp = 1;
			return a_bit_var & (temp << it->second);
		} 
		else 
		{
			ERRORLOG("Unable to find ComponentBitPlacement of T: " << type.name());
			return false;
		}
	}

	template <class T>
	void ECSKeyLockSystem::SetComponentBits(ComponentBitList& a_bit_var)
	{
		int64 temp = 1;
		(a_bit_var |= (temp << GetComponentBitPlacement<T>()));
	}

	template <class T>
	void ECSKeyLockSystem::ResetComponentBits(ComponentBitList& a_bit_var) 
	{
		int64 temp = 1;
		(a_bit_var &= ~(temp << GetComponentBitPlacement<T>()));
	}

	template <class T>
	int8 ECSKeyLockSystem::GetComponentBitPlacement() const 
	{
		auto type	= typeid(T);
		auto it		= component_bit_placement_.find(type);
		if (it != component_bit_placement_.end())
		{
			return it->second;
		} 
		else 
		{
			ERRORLOG("Unable to find ComponentBitPlacement of T: " << type.name());
			return UNASSIGNED_COMPONENT_BIT;
		}
	}

#pragma endregion

} // End of namespace ~ DECS
