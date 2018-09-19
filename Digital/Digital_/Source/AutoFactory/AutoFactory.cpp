#include <AutoFactory/AutoFactory.h>

namespace DFactory {

	std::unordered_map<std::string, ClassFactoryMap>& GetFactories() {
		static std::unordered_map<std::string, ClassFactoryMap>* factories =
			new std::unordered_map<std::string, ClassFactoryMap>();
		return *factories;
	}

} // End of namespace ~ DFactory