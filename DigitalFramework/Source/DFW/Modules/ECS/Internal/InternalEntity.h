#pragma once

#include <DFW/Modules/ECS/EntityHandle.h>

#include <DFW/CoreSystems/DUID.h>

namespace DFW
{
	namespace DECS
	{
		// FW Declare.
		class EntityRegistry;

		class InternalEntity
		{
		public:
			InternalEntity();
			InternalEntity(EntityHandle const a_handle, EntityRegistry* a_registry);
			~InternalEntity() = default;

			operator bool() const { return IsEntityValid(); }

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
