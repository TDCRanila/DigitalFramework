#pragma once

#include <Utility/AutoFactory/AutoFactory.h>

#include <CoreSystems/DUID.h>

#include <entt/entity/handle.hpp>

namespace DFW
{
	namespace DECS
	{
		// FW Declare.
		class Universe;
		class EntityManager;
		class ComponentManager;

		using EntityHandle = entt::entity;
		constexpr entt::null_t DFW_NULL_ENTITY_HANDLE = entt::null;

		class Entity : public DFactory::AutoFactory<Entity>
		{
		private:
			friend EntityManager;
			friend ComponentManager;

		public:
			Entity();
			Entity(EntityHandle a_entity_handle, Universe& a_universe);
			virtual ~Entity() = default;

			std::strong_ordering operator<=>(Entity const& a_other) const;

			operator EntityHandle();
			operator EntityHandle() const;

			DFW::DUID GetID() const;
			EntityHandle GetHandle() const;
			Universe& GetUniverse() const;

			bool IsEntityValid() const;
			bool IsPendingDeletion() const;

		private:
			DUID			_id;
			EntityHandle	_handle;
			Universe*		_universe;

		};

	} // End of namespace ~ DECS.

	using Entity = DECS::Entity;
	using EntityHandle = DECS::EntityHandle;

} // End of namespace ~ DFW.
