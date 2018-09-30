#include <ECS/ECSKeyLockSystem.h>

#include <Defines/Defines.h>

#include <AutoFactory/AutoFactory.h>

#include <vector>

namespace DECS {

	ECSKeyLockSystem::ECSKeyLockSystem() { /*EMPTY*/ }
	ECSKeyLockSystem::~ECSKeyLockSystem() { /*EMPTY*/ }

	void ECSKeyLockSystem::GenerateComponentKeys() {
		int8_t key_index = 0;

		for (auto const&[type, fac] : *DFactory::GetFactoryList(ComponentListName)) {
			component_bit_placement_[type] = key_index++;
			// TODO: if (key_index >= 64) { WARNING }
		}
	}

} // End of namespace ~ DECS