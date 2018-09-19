#pragma once

#include <unordered_map>
#include <typeindex>
#include <string>

namespace DFactory {
	
	class AutoFactoryInterface {
	public:
		AutoFactoryInterface() {/*EMPTY*/}
		virtual ~AutoFactoryInterface() {/*EMPTY*/}
	};
	
	typedef std::unordered_map<std::type_index, AutoFactoryInterface*> ClassFactoryMap;
	typedef std::unordered_map<std::type_index, AutoFactoryInterface*>::iterator ClassFactoryMapIt;
	
	// TODO: Static Init Order Fiasco Fix Problems Occur.
	static std::unordered_map<std::string, ClassFactoryMap> factories;

	template <class T>
	class AutoFactory : public AutoFactoryInterface {
	public:
		AutoFactory() { /*EMPTY*/ }
		virtual ~AutoFactory() { /*EMPTY*/ }

		virtual T* Create() = 0;
	};

	static ClassFactoryMap* GetFactoryList(std::string a_list_name) {
		auto it_list = DFactory::factories.find(a_list_name);
		if (it_list != DFactory::factories.end()) {
			return &it_list->second;
		}
		return nullptr;
	}

	template <class T>
	static T* ConstructFromFactory(std::string a_list_name) {
		// Find List
		auto it_list = DFactory::factories.find(a_list_name);
		if (it_list != DFactory::factories.end()) {
			// Find Factory
			auto it_fac = it_list->second.find(typeid(T));
			if (it_fac != it_list->second.end()) {
				// TODO: DEBUG->DynamicCast | RELEASE->StaticCast
				auto factory = dynamic_cast<AutoFactory<T>*>(it_fac->second);
				return factory->Create();
			}
		}
		return nullptr;
	}

	template <class T>
	static AutoFactory<T>* GetFactoryOfType(std::string a_list_name) {
		// Find List
		auto it_list = DFactory::factories.find(a_list_name);
		if (it_list != DFactory::factories.end()) {
			// Find Factory
			auto it_fac = it_list->second.find(typeid(T));
			if (it_fac != it_list->second.end()) {
			// TODO: DEBUG->DynamicCast | RELEASE->StaticCast
			return dynamic_cast<AutoFactory<T>*>(it_fac->second);
			}
		}
		return nullptr;
	}

#define REGISTER_TYPE(fclass, list)								\
class fclass##Factory : public DFactory::AutoFactory<fclass> {	\
public:															\
	fclass##Factory() {											\
		DFactory::factories[#list][typeid(fclass)] = this;		\
	}															\
	virtual fclass* Create() override {							\
		return new fclass();									\
	}															\
};																\
static fclass##Factory global##fclass##FactoryObject;

} // End of namespace ~ DFactory
