#include <AutoFactory/AutoFactory.h>

namespace DFactory {

	std::unordered_map<std::string, ClassFactoryMap>* GetFactories() {
		static std::unordered_map<std::string, ClassFactoryMap>* factories =
			new std::unordered_map<std::string, ClassFactoryMap>();
		return factories;
	}

	ClassFactoryMap* GetFactoryList(std::string a_list_name) {
		auto it_list = DFactory::GetFactories()->find(a_list_name);
		if (it_list != DFactory::GetFactories()->end()) {
			return &it_list->second;
		}
		return nullptr;
	}

} // End of namespace ~ DFactory