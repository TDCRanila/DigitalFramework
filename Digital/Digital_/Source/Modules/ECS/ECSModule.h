#pragma once

#include <memory>
#include <unordered_map>
#include <string>

// FW Declares.
namespace DCore
{
	class ApplicationInstance;
} // End of namespace ~ DCore.

namespace ECS 
{
	// FW Declares.
	class SystemManager;
	class EntityManager;
	class EventHandler;
	class Universe;

	class ECSModule final 
	{
	public:
		~ECSModule() = default;
			   
		SystemManager* const SystemManager() const;
		EntityManager* const EntityManager() const;
		EventHandler* const EventHandler() const;

		Universe* RegisterUniverse(std::string const& a_universe_name);
		Universe* const GetUniverse(std::string const& a_universe_name);
		Universe* const CurrentUniverse();

	protected:
		friend DCore::ApplicationInstance;

		ECSModule();

		void InitECS();
		void TerminateECS();
		void UpdateECS();

	private:
		
		ECS::SystemManager*	_system_manager;
		ECS::EntityManager*	_entity_manager;
		ECS::EventHandler*	_event_handler;
		Universe*			_current_universe;
		
		std::unordered_map<std::string, Universe*> _universes;

		bool				_initialized;

	};

} // End of namespace ~ ECS

