#pragma once

#include <memory>
#include <unordered_map>
#include <string>

// FW Declares.
namespace DCore
{
	class ApplicationInstance;
} // End of namespace ~ DCore.

namespace DECS 
{
	// FW Declares.
	class ECSystemManager;
	class ECSEntityManager;
	class ECSEventHandler;
	class ECSUniverse;

	class ECSModule final 
	{
	public:
		~ECSModule() = default;
			   
		ECSystemManager* const SystemManager() const;
		ECSEntityManager* const EntityManager() const;
		ECSEventHandler* const EventHandler() const;

		ECSUniverse* RegisterUniverse(std::string const& a_universe_name);
		ECSUniverse* const GetUniverse(std::string const& a_universe_name);
		ECSUniverse* const CurrentUniverse();

	protected:
		friend DCore::ApplicationInstance;

		ECSModule();

		void InitECS();
		void TerminateECS();
		void UpdateECS();

	private:
		
		ECSystemManager*	_system_manager;
		ECSEntityManager*	_entity_manager;
		ECSEventHandler*	_event_handler;
		ECSUniverse*		_current_universe;
		
		std::unordered_map<std::string, ECSUniverse*> _universes;

		bool				_initialized;

	};

} // End of namespace ~ DECS

