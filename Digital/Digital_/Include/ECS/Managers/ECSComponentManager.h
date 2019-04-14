#pragma once

#include <ECS/Objects/ECSComponent.h>

#include <unordered_map>
#include <vector>
#include <typeindex>

namespace DECS 
{

	class ECSComponentManager final 
	{
	public:
		ECSComponentManager();
		~ECSComponentManager();

		template <class T>
		T& GetComponent(EntityID a_entity_id) const;

		template <class T>
		T& AddComponent(EntityID a_entity_id);

		template <class T>
		T& DeleteComponent(EntityID a_entity_id);

	private:
		std::unordered_map<std::type_index, std::vector<ECSComponent>> component_vectors_;

	};

} // End of namespace ~ DECS
