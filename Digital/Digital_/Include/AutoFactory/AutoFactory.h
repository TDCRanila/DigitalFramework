#pragma once

#include <unordered_map>
#include <typeindex>
#include <string>

namespace DFactory {
		
	class AutoFactoryInterface;

	typedef std::unordered_map<std::type_index, AutoFactoryInterface*> ClassFactoryMap;
	typedef std::unordered_map<std::type_index, AutoFactoryInterface*>::iterator ClassFactoryMapIt;
	
	class AutoFactoryInterface {
	public:
		AutoFactoryInterface() {/*EMPTY*/ }
		virtual ~AutoFactoryInterface() {/*EMPTY*/ }
	};

	template <class T>
	class AutoFactory : public AutoFactoryInterface {
	public:
		AutoFactory() { /*EMPTY*/ }
		virtual ~AutoFactory() { /*EMPTY*/ }

		virtual T* Create() = 0;
	};

	// TODO: // Function to get map with class factories.
	std::unordered_map<std::string, ClassFactoryMap>* GetFactories();
	
	// TODO: // Function to get a map of class factories under a given list name identifier.
	ClassFactoryMap* GetFactoryList(std::string a_list_name);

	// TODO: // Function to construct a class from a factory under a given list name identifier.
	template <class T>
	T* ConstructFromFactory(std::string a_list_name);

	// TODO: // Function to get get a pointer to a class factory of specified type under a given list name identifier.
	template <class T>
	AutoFactory<T>* GetFactoryOfType(std::string a_list_name);

#define REGISTER_TYPE(fclass, list)									\
class fclass##Factory : public DFactory::AutoFactory<fclass> {		\
public:																\
	fclass##Factory() {												\
		(*DFactory::GetFactories())[list][typeid(fclass)] = this;	\
	}																\
	virtual fclass* Create() override {								\
		return new fclass();										\
	}																\
};																	\
static fclass##Factory global##fclass##FactoryObject;

#pragma region Template Function Implementation

	template <class T>
	T* ConstructFromFactory(std::string a_list_name) {
		// Find List
		auto it_list = DFactory::GetFactories().find(a_list_name);
		if (it_list != DFactory::GetFactories().end()) {
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
	AutoFactory<T>* GetFactoryOfType(std::string a_list_name) {
		// Find List
		auto it_list = DFactory::GetFactories().find(a_list_name);
		if (it_list != DFactory::GetFactories().end()) {
			// Find Factory
			auto it_fac = it_list->second.find(typeid(T));
			if (it_fac != it_list->second.end()) {
				// TODO: DEBUG->DynamicCast | RELEASE->StaticCast
				return dynamic_cast<AutoFactory<T>*>(it_fac->second);
			}
		}
		return nullptr;
	}

#pragma endregion

} // End of namespace ~ DFactory
