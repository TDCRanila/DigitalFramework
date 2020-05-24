#pragma once

#include <Utility/StringUtility.h>

#include <boost/functional/hash.hpp>

#include <memory>
#include <algorithm>
#include <functional>
#include <utility>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace DFactory 
{
		
	template <class Base, class... Args>
	class AutoFactory 
	{
	public:
		typedef std::function<std::unique_ptr<Base>(Args...)>													FuncType;
		typedef std::pair<std::string, std::type_index>															StringTypePair;
		typedef std::unordered_map< StringTypePair, FuncType, boost::hash<StringTypePair>>						FactoryMap;
		typedef typename std::unordered_map< StringTypePair, FuncType, boost::hash<StringTypePair> >::iterator	FactoryMapIt;

		friend Base;

		template <class... Args>
		static std::unique_ptr<Base> Construct(const std::string& a_type_name, Args&&...a_args) 
		{

			// Create Compare Lambda used to find the type_name in the factories map.
			auto compare_lambda = [&a_type_name](const std::pair<StringTypePair, FuncType>& a_it_element) 
			{
				return a_it_element.first.first == a_type_name;
			};

			FactoryMap& fac	= GetFactories();
			FactoryMapIt it = std::find_if(fac.begin(), fac.end(), compare_lambda);

			// Check if type was registered in the map or not.
			if (it == fac.end()) 
			{
				_ASSERT(false); // -> Element has not been found in factory_map.
				return std::unique_ptr<Base>();
			} 
			else 
			{
				return it->second(std::forward<Args>(a_args)...);
			}
		}

		static FactoryMap& GetFactories() 
		{
			static FactoryMap factory_map;
			return factory_map;
		}

		template <class T>
		class Registrar : public Base 
		{
		public:
			friend T;

			static bool RegisterType() 
			{
				// Create Factory Construction Lambda for Type T.
				FuncType constructor_lambda = [](Args... a_args) -> std::unique_ptr<Base> 
				{
					return std::make_unique<T>(std::forward<Args>(a_args)...);
				};

				const std::type_index type_id	= typeid(T);
				std::string type_name			= type_id.name();

				// TODO: MSVC Further Specific Demangling. ~ Implement Other compilers?
				DUtility::SubtractString(type_name, std::string("class "));
				DUtility::SubtractString(type_name, std::string("struct "));

				AutoFactory::GetFactories()[std::make_pair(type_name, type_id)] = constructor_lambda;

				return true;
			}

			static bool registered;

		private:
			Registrar() : Base(Key{}) { (void)registered; }

		}; // Registrar.

	private:
		class Key 
		{
		private:
			Key() = default;
			
			template <class T> friend class Registrar;

		}; // Key.

		AutoFactory() = default;

	};

#pragma region Template Magic

	// Static Variable initialization so we can execute code before main().
	template <class Base, class... Args>
	template <class T>
	bool AutoFactory<Base, Args...>::template Registrar<T>::registered = AutoFactory<Base, Args...>::template Registrar<T>::RegisterType();

#pragma endregion

} // End of namespace ~ DFactory
