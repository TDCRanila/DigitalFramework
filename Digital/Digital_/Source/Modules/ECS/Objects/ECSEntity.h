#pragma once

#include <AutoFactory/AutoFactory.h>

#include <CoreSystems/DUID.h>

#include <entt/entity/handle.hpp>

namespace ECS 
{
	// FW Declare.
	class Universe;
	class EntityManager;
	class ComponentManager;
	
	using EntityHandle								= entt::entity;
	constexpr entt::null_t DFW_NULL_ENTITY_HANDLE	= entt::null;

	class Entity : public DFactory::AutoFactory<Entity>
	{
	public:
		Entity();
		Entity(EntityHandle const& a_entity_handle, Universe* const a_universe);
		virtual ~Entity() = default;
		
		std::strong_ordering operator<=>(Entity const& a_other) const;

		operator EntityHandle();
		operator EntityHandle() const;
		
		DCore::DUID GetID() const;
		EntityHandle GetHandle() const;
		Universe* GetUniverse() const;

		bool IsEntityValid() const;
		bool IsPendingDeletion() const;

	private:
		friend EntityManager;
		friend ComponentManager;

		EntityHandle	_handle;
		Universe*	_universe;

	};

} // End of namespace ~ ECS
