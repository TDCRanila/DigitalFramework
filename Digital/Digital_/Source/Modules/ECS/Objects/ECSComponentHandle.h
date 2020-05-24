#pragma once

#include <Modules/ECS/Objects/ECSComponent.h>

namespace DECS
{
	template <class T>
	class ECSComponentHandle final
	{
	public:
		ECSComponentHandle();
		ECSComponentHandle(const T* a_component);
		~ECSComponentHandle();

		bool IsComponentValid() const;

		const T* operator->() const;

	private:
		const T* component_ptr_;

	};

#pragma region Template Function Implementation

	template <class T>
	ECSComponentHandle<T>::ECSComponentHandle() :
		component_ptr_(nullptr)
	{
		/*EMPTY*/
	}

	template <class T>
	ECSComponentHandle<T>::ECSComponentHandle(const T* a_component) :
		component_ptr_(a_component)
	{
		/*EMPTY*/
	}

	template <class T>
	ECSComponentHandle<T>::~ECSComponentHandle() { /*EMPTY*/ }

	template <class T>
	bool ECSComponentHandle<T>::IsComponentValid() const
	{
		return component_ptr_;
	}

	template <class T>
	const T* ECSComponentHandle<T>::operator->() const
	{
		return component_ptr_;
	}

#pragma endregion

} // End of namespace ~ DECS
