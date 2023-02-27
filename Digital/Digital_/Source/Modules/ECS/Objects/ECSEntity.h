#pragma once

#include <Modules/ECS/Objects/ECSEntityHandle.h>

#include <CoreSystems/DUID.h>

#include <Utility/AutoFactory/AutoFactory.h>

namespace DFW
{
	namespace DECS
	{
		// FW Declare.
		class EntityRegistry;
		class EntityManager;
		class ComponentManager;

		class Entity : public DFactory::AutoFactory<Entity>
		{
		private:
			friend EntityManager;
			friend ComponentManager;

		public:
			Entity();
			Entity(EntityHandle a_entity_handle, EntityRegistry& a_registry);
			virtual ~Entity() = default;

			std::strong_ordering operator<=>(Entity const& a_other) const;

			operator EntityHandle();
			operator EntityHandle() const;

			DFW::DUID GetID() const;
			EntityHandle GetHandle() const;
			EntityRegistry& GetRegistry() const;

			bool IsEntityValid() const;
			bool IsPendingDeletion() const;

		protected:
			DUID			_id;
			EntityHandle	_handle;
			EntityRegistry*	_registry;

		};

	} // End of namespace ~ DECS.

	using Entity = DECS::Entity;

} // End of namespace ~ DFW.
