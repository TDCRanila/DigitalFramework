#include <ECS/ECSKeyLockSystem.h>

#include <Defines/Defines.h>

#include <vector>

namespace DECS {

	ECSKeyLockSystem::ECSKeyLockSystem() { /*EMPTY*/ }
	ECSKeyLockSystem::~ECSKeyLockSystem() { /*EMPTY*/ }

	void ECSKeyLockSystem::GenerateComponentKeys() {
		uint8_t key_index = 0;
		for (auto const&[type, fac] : *DFactory::GetFactoryList(ComponentListName)) {
			component_bit_placement_[type] = key_index++;
		}
	}

} // End of namespace ~ DECS
