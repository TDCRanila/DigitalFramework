#pragma once

#include <Modules/ECS/EntityHandle.h>

#include <CoreSystems/DUID.h>

namespace DFW
{
	namespace DECS
	{
		// FW Declare.
		class EntityManager;
		class EntityRegistry;

		class InternalEntity
		{
		private:
			friend EntityManager;

		public:
			InternalEntity();
			InternalEntity(EntityHandle const a_handle, EntityRegistry* a_registry);
			~InternalEntity() = default;

		public:
			EntityHandle GetHandle() const { return _handle; }
			EntityRegistry& GetRegistry() const { DFW_ASSERT(_registry); return *_registry; }

			bool IsEntityValid() const;
			bool IsMarkedForDestruction() const;

		protected:
			EntityHandle	_handle;
			EntityRegistry* _registry;
		};

	} // End of namespace ~ DECS.

} // End of namespace ~ DFW.
