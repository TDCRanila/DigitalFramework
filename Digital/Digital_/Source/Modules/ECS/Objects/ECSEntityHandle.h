#pragma once

#include <entt/entity/handle.hpp>

namespace DFW
{
	namespace DECS
	{
		using EntityHandle = entt::entity;
		constexpr entt::null_t DFW_NULL_ENTITY_HANDLE = entt::null;

	} // End of namespace ~ DECS.

	using EntityHandle = DECS::EntityHandle;

} // End of namespace ~ DFW.
