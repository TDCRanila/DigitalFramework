#pragma once

#include <Utility/StringUtility.h>
#include <Utility/TemplateUtility.h>

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
		
	template <typename BaseType, typename... Args>
	class AutoFactory 
	{
	public:
		typedef std::function<std::unique_ptr<BaseType>(Args...)>												FuncType;
		typedef std::pair<std::string, std::type_index>															StringTypePair;
		typedef std::unordered_map<StringTypePair, FuncType, boost::hash<StringTypePair>>						FactoryMap;
		typedef typename std::unordered_map<StringTypePair, FuncType, boost::hash<StringTypePair>>::iterator	FactoryMapIt;

		friend BaseType;

		template <typename... Args>
		static std::unique_ptr<BaseType> Construct(const std::string& a_type_name, Args&&...a_args) 
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
				return std::unique_ptr<BaseType>();
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

		template <typename RegistrarType, typename DerivedClassOverride = BaseType>
		requires (IsDerivedFrom<DerivedClassOverride, BaseType> || AreSameTypes<DerivedClassOverride, BaseType>)
		class Registrar : public DerivedClassOverride
		{
		public:
			friend RegistrarType;

			static bool RegisterType() 
			{
				// Create Factory Construction Lambda for Type RegistrarType.
				FuncType constructor_lambda = [](Args... a_args) -> std::unique_ptr<BaseType> 
				{
					return std::make_unique<RegistrarType>(std::forward<Args>(a_args)...);
				};

				const std::type_index type_id	= typeid(RegistrarType);
				std::string type_name			= type_id.name();

				// TODO: MSVC Further Specific Demangling. ~ Implement Other compilers?
				DUtility::SubtractString(type_name, std::string("class "));
				DUtility::SubtractString(type_name, std::string("struct "));

				AutoFactory::GetFactories()[std::make_pair(type_name, type_id)] = constructor_lambda;

				return true;
			}

			static bool registered;

		private:
			Registrar() { (void)registered; }

		}; // Registrar.

		template <typename RegistrarType>
		class StrictRegistrar : public BaseType
		{
		public:
			friend RegistrarType;

			static bool RegisterType()
			{
				// Create Factory Construction Lambda for Type RegistrarType.
				FuncType constructor_lambda = [](Args... a_args) -> std::unique_ptr<BaseType>
				{
					return std::make_unique<RegistrarType>(std::forward<Args>(a_args)...);
				};

				const std::type_index type_id = typeid(RegistrarType);
				std::string type_name = type_id.name();

				// TODO: MSVC Further Specific Demangling. ~ Implement Other compilers?
				DUtility::SubtractString(type_name, std::string("class "));
				DUtility::SubtractString(type_name, std::string("struct "));

				AutoFactory::GetFactories()[std::make_pair(type_name, type_id)] = constructor_lambda;

				return true;
			}

			static bool registered;

		private:
			StrictRegistrar() : BaseType(Key{}) { (void)registered; }

		}; // StrictRegistrar.

	private:
		class Key 
		{
		private:
			Key() = default;

			template <typename RegistrarType>
			friend class StrictRegistrar;

		}; // Key.

		AutoFactory() = default;

	};

#pragma region Template Magic

	// Static Variable initialization so we can execute code before main().
	template <typename BaseType, typename... Args>
	template <typename RegistrarType, typename DerivedClassOverride>
	requires (IsDerivedFrom<DerivedClassOverride, BaseType> || AreSameTypes<DerivedClassOverride, BaseType>)
	bool AutoFactory<BaseType, Args...>::template Registrar<RegistrarType, DerivedClassOverride>::registered = AutoFactory<BaseType, Args...>::template Registrar<RegistrarType, DerivedClassOverride>::RegisterType();

	template <typename BaseType, typename... Args>
	template <typename RegistrarType>
	bool AutoFactory<BaseType, Args...>::template StrictRegistrar<RegistrarType>::registered = AutoFactory<BaseType, Args...>::template StrictRegistrar<RegistrarType>::RegisterType();

#pragma endregion

} // End of namespace ~ DFactory
