#pragma once

#include <CoreSystems/Logging/Logger.h>
#include <CoreSystems/Stage/Stage.h>
#include <CoreSystems/Stage/StageEvents.h>
#include <CoreSystems/Stage/StageStackCommunicator.h>

#include <Utility/TemplateUtility.h>

namespace DFW
{
	const int8 MAX_SUPPORTED_STAGES = 32;

	class StageStackController final
	{
	public:
		StageStackController();
		~StageStackController();

		// TODO Provide argument documentation for user. (Basically the constructor arguments of the BaseStage class)

		template <class TStage, class... Args>
		requires (not IsDerivedFrom<TStage, StageBase>)
		StageID AttachStage(Args&&... a_args);

		template <class TStage, class... Args>
		requires (IsDerivedFrom<TStage, StageBase>)
		StageID AttachStage(Args&&... a_args);

		template <class TStage, class... Args>
		requires (not IsDerivedFrom<TStage, StageBase>)
		StageID AttachStageBack(Args&&... a_args);

		template <class TStage, class... Args>
		requires (IsDerivedFrom<TStage, StageBase>)
		StageID AttachStageBack(Args&&... a_args);
		
		bool RemoveStage(StageID a_id);
		bool RemoveStageBack(StageID a_id);
		void RemoveAllAttachedStages();

		std::vector<StageBase*>::iterator begin() { return _stages.begin(); }
		std::vector<StageBase*>::iterator end() { return _stages.end(); }
		std::vector<StageBase*>::reverse_iterator rbegin() { return _stages.rbegin(); }
		std::vector<StageBase*>::reverse_iterator rend() { return _stages.rend(); }
		
		std::vector<StageBase*>::const_iterator begin() const { return _stages.begin(); }
		std::vector<StageBase*>::const_iterator end()	const { return _stages.end(); }
		std::vector<StageBase*>::const_reverse_iterator rbegin() const { return _stages.rbegin(); }
		std::vector<StageBase*>::const_reverse_iterator rend() const { return _stages.rend(); }

	private:
		int64 _stage_insert_index;
		
		std::vector<StageBase*> _stages;

	};

} // End of namespace DFW.

#pragma region Template Implementation

namespace DFW
{

	template <class TStage, class... Args>
	requires (not IsDerivedFrom<TStage, StageBase>)
	StageID StageStackController::AttachStage(Args&&... a_args)
	{
		static_assert(IsAlwaysFalse<TStage>, __FUNCTION__ " - Trying to attach a stage of type T that isn't derived from DECS::StageBase.");
		return StageID();
	}

	template <class TStage, class... Args>
	requires (IsDerivedFrom<TStage, StageBase>)
	StageID StageStackController::AttachStage(Args&&... a_args)
	{
		const auto it = _stages.emplace(_stages.begin() + _stage_insert_index);
		StageBase*& stage_ptr = (*it);
		stage_ptr = new TStage(std::forward<Args>(a_args)...);
		++_stage_insert_index;

		DFW_INFOLOG("Attaching New Stage with ID: {}", stage_ptr->GetID());

		stage_ptr->OnAttached();

		return stage_ptr->GetID();
	}

	template <class TStage, class... Args>
	requires (not IsDerivedFrom<TStage, StageBase>)
	StageID	StageStackController::AttachStageBack(Args&&... a_args)
	{
		static_assert(always_false<TStage>::value, __FUNCTION__ " - Trying to a attach a stage of type T that isn't derived from DECS::StageBase.");
		return StageID();
	}

	template <class TStage, class... Args>
	requires (IsDerivedFrom<TStage, StageBase>)
	StageID StageStackController::AttachStageBack(Args&&... a_args)
	{
		StageBase*& stage_ptr	= _stages.emplace_back();
		stage_ptr				= new TStage(std::forward<Args>(a_args)...);

		DFW_INFOLOG("Attaching New Stage at the back of the stack with ID: {}", stage_ptr->GetID());

		stage_ptr->OnAttached();

		return stage_ptr->GetID();
	}

} // End of namespace ~ DFW.

#pragma endregion
