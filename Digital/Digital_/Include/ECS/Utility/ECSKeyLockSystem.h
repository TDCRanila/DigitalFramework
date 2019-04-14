#pragma once

#include <ECS/Utility/ECSCompBitList.h>

#include <unordered_map>
#include <typeindex>
#include <stdint.h>

namespace DECS 
{

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
		int8_t GetComponentBitPlacement() const;

	private:
		std::unordered_map<std::type_index, int8_t> component_bit_placement_;

	};

#pragma region Template Function Implementation

	template <class T>
	bool ECSKeyLockSystem::IsComponentBitTrue(ComponentBitList& a_bit_var) const 
	{
		auto it = component_bit_placement_.find(typeid(T));
		if (it != component_bit_placement_.end()) 
		{
			int64_t temp = 1;
			return a_bit_var & (temp << it->second);
		} 
		else 
		{
			ERRORLOG("Unable to find ComponentBitPlacement of T: " << #T);
			return false;
		}
	}

	template <class T>
	void ECSKeyLockSystem::SetComponentBits(ComponentBitList& a_bit_var)
	{
		int64_t temp = 1;
		(a_bit_var |= (temp << GetComponentBitPlacement<T>()));
	}

	template <class T>
	void ECSKeyLockSystem::ResetComponentBits(ComponentBitList& a_bit_var) 
	{
		int64_t temp = 1;
		(a_bit_var &= ~(temp << GetComponentBitPlacement<T>()));
	}

	template <class T>
	int8_t ECSKeyLockSystem::GetComponentBitPlacement() const 
	{
		auto it = component_bit_placement_.find(typeid(T));
		if (it != component_bit_placement_.end())
		{
			return it->second;
		} 
		else 
		{
			ERRORLOG("Unable to find ComponentBitPlacement of T: " << #T);
			return -1;
		}
	}

#pragma endregion

} // End of namespace ~ DECS
