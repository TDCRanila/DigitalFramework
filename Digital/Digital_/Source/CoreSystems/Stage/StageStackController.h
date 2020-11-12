#pragma once

#include <CoreSystems/Logging/Logger.h>
#include <CoreSystems/Stage/Stage.h>

#include <Utility/TemplateUtility.h>

namespace DCore
{
	const int8 MAX_SUPPORTED_STAGES = 32;

	class StageStackController final
	{
	public:
		StageStackController();
		~StageStackController();

		void RemoveAttachedStages();
		void DeleteAttachedStages();

		// TODO Privide argument documentation for user. (Basically the constructor arguments of the BaseStage class)

		template <class TStage, class... Args>
		typename std::enable_if<!std::is_base_of<StageBase, TStage>::value || is_same<StageBase, TStage>::value, StageID>::type
		/*StageID*/ AttachStage(Args&&... a_args);

		template <class TStage, class... Args>
		typename std::enable_if<std::is_base_of<StageBase, TStage>::value && !is_same<StageBase, TStage>::value, StageID>::type
		/*StageID*/ AttachStage(Args&&... a_args);

		template <class TStage, class... Args>
		typename std::enable_if<!std::is_base_of<StageBase, TStage>::value || is_same<StageBase, TStage>::value, StageID>::type
		/*StageID*/ AttachStageBack(Args&&... a_args);

		template <class TStage, class... Args>
		typename std::enable_if<std::is_base_of<StageBase, TStage>::value && !is_same<StageBase, TStage>::value, StageID>::type
		/*StageID*/ AttachStageBack(Args&&... a_args);
		
		void RemoveStage(StageID a_id);
		void RemoveStageBack(StageID a_id);

		std::vector<StageBase*>::iterator begin() { return _stages.begin(); }
		std::vector<StageBase*>::iterator end() { return _stages.end(); }
		std::vector<StageBase*>::reverse_iterator rbegin() { return _stages.rbegin(); }
		std::vector<StageBase*>::reverse_iterator rend() { return _stages.rend(); }
		
		std::vector<StageBase*>::const_iterator begin() const { return _stages.begin(); }
		std::vector<StageBase*>::const_iterator end()	const { return _stages.end(); }
		std::vector<StageBase*>::const_reverse_iterator rbegin() const { return _stages.rbegin(); }
		std::vector<StageBase*>::const_reverse_iterator rend() const { return _stages.rend(); }

	protected:

		friend class ApplicationInstance;

		const std::vector<StageBase*>& GetStages() const;

	private:
		std::vector<StageBase*> _stages;

		int64 _stage_insert_index;

		std::shared_ptr<StageStackCommunicator> _stack_communicator;
		
	};

} // End of namespace DCore.

#pragma region Template Implementation

namespace DCore
{

	template <class TStage, class... Args>
	typename std::enable_if<!std::is_base_of<StageBase, TStage>::value || is_same<StageBase, TStage>::value, StageID>::type
	/*StageID*/ StageStackController::AttachStage(Args&&... a_args)
	{
		static_assert(always_false<TStage>::value, __FUNCTION__ " - Trying to attach a stage of type T that isn't derived from DECS::StageBase.");
		return StageID();
	}

	template <class TStage, class... Args>
	typename std::enable_if<std::is_base_of<StageBase, TStage>::value && !is_same<StageBase, TStage>::value, StageID>::type
	/*StageID*/ StageStackController::AttachStage(Args&&... a_args)
	{
		const auto it = _stages.emplace(_stages.begin() + _stage_insert_index);
		StageBase*& stage_ptr = (*it);
		stage_ptr = new TStage(std::forward<Args>(a_args)...);
		stage_ptr->_stage_stack_communicator = _stack_communicator;
		++_stage_insert_index;

		DFW_INFOLOG("Attaching New Stage - ID: {}", stage_ptr->GetID());
		return stage_ptr->GetID();
	}

	template <class TStage, class... Args>
	typename std::enable_if<!std::is_base_of<StageBase, TStage>::value || is_same<StageBase, TStage>::value, StageID>::type
	/*StageID*/	StageStackController::AttachStageBack(Args&&... a_args)
	{
		static_assert(always_false<TStage>::value, __FUNCTION__ " - Trying to a attach a stage of type T that isn't derived from DECS::StageBase.");
		return StageID();
	}

	template <class TStage, class... Args>
	typename std::enable_if<std::is_base_of<StageBase, TStage>::value && !is_same<StageBase, TStage>::value, StageID>::type
	/*StageID*/ StageStackController::AttachStageBack(Args&&... a_args)
	{
		StageBase*& stage_ptr	= _stages.emplace_back();
		stage_ptr				= new TStage(std::forward(a_args));
		stage_ptr->_stage_stack_communicator = _stack_communicator;

		DFW_INFOLOG("Attaching New Stage at back - ID: {}", stage_ptr->GetID());
		return stage_ptr->GetID();
	}

} // End of namespace ~ DCore.

#pragma endregion
