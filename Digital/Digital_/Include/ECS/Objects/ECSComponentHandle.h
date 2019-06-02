#pragma once

#include <ECS/Objects/ECSComponent.h>

namespace DECS
{
	template <class T>
	class ECSComponentHandle final
	{
	public:
		ECSComponentHandle();
		ECSComponentHandle(T* a_component);
		~ECSComponentHandle();

		bool IsComponentValid() const;

		T* operator->() const;

	private:
		T* component_ptr;

	};

#pragma region Template Function Implementation

	template <class T>
	ECSComponentHandle<T>::ECSComponentHandle() :
		component_ptr(nullptr)
	{
		/*EMPTY*/
	}

	template <class T>
	ECSComponentHandle<T>::ECSComponentHandle(T* a_component) :
		component_ptr(a_component)
	{
		/*EMPTY*/
	}

	template <class T>
	ECSComponentHandle<T>::~ECSComponentHandle() { /*EMPTY*/ }

	template <class T>
	bool ECSComponentHandle<T>::IsComponentValid() const
	{
		return component_ptr;
	}

	template <class T>
	T* ECSComponentHandle<T>::operator->() const
	{
		return component_ptr;
	}

#pragma endregion

} // End of namespace ~ DECS
