#include <CoreSystems/Stage/StageStackController.h>

namespace DFW
{
	StageStackController::StageStackController()
		:  _stage_insert_index(0)
	{
		_stages.reserve(MAX_SUPPORTED_STAGES);
	}

	StageStackController::~StageStackController()
	{
		RemoveAllAttachedStages();
		DeleteAllAttachedStages();
	}

	void StageStackController::SetStageStackCommunicator(std::shared_ptr<StageStackCommunicator> a_communicator)
	{
		_stage_stack_communicator = a_communicator;

		for (auto stage_it = this->rbegin(); stage_it != this->rend(); ++stage_it)
		{
			(*stage_it)->SetStageStackCommunicator(a_communicator);
			(*stage_it)->BindStageEventFunc(DFW_BIND_FUNC(_stage_stack_communicator->OnStageEventReceived));
		}
	}

	void StageStackController::RemoveAllAttachedStages()
	{
		bool has_deleted_stage;
		for (int32 it = 0; it < _stages.size();)
		{
			has_deleted_stage = RemoveStage(_stages[it]->GetID());
			if (!has_deleted_stage)
				++it;
		}
	}

	void StageStackController::DeleteAllAttachedStages()
	{
		// Cleanup anything that is left-over, however, this shouldn't be the case.
		for (StageBase*& stage_ptr : _stages)
		{
			if (stage_ptr != nullptr)
			{
				DFW_LOG("Deleting Stage with ID: {}", stage_ptr->GetID());
				delete stage_ptr;
				stage_ptr = nullptr;
			}
		}

		_stages.clear();
	}

	bool StageStackController::RemoveStage(StageID a_id)
	{
		const auto it = std::find_if(_stages.begin(), _stages.end(), [a_id](const StageBase* a_stage) { return a_stage->GetID() == a_id; });
		if (it != _stages.end())
		{
			DFW_INFOLOG("Removing attached Stage with ID: {}", a_id);
			StageBase* stage_ptr = (*it);
			stage_ptr->OnRemoved();

			StageRemovedEvent event(stage_ptr->GetID(), stage_ptr->GetName(), stage_ptr->IsDisabled());
			_stage_stack_communicator->OnStageEventReceived(event);
			
			delete stage_ptr;

			_stages.erase(it);
			--_stage_insert_index;
			return true;
		}
		else
		{
			DFW_WARNLOG("Trying to remove unknown stage with ID: {}.", a_id);
			return false;
		}
	}

	bool StageStackController::RemoveStageBack(StageID a_id)
	{
		const auto it = std::find_if(_stages.begin(), _stages.end(), [a_id](const StageBase* a_stage) { return a_stage->GetID() == a_id; });
		if (it != _stages.end())
		{
			DFW_INFOLOG("Removing attached Stage with ID: {}", a_id);
			StageBase* stage_ptr = (*it);
			stage_ptr->OnRemoved();

			StageRemovedEvent event(stage_ptr->GetID(), stage_ptr->GetName(), stage_ptr->IsDisabled());
			_stage_stack_communicator->OnStageEventReceived(event);

			delete stage_ptr;

			_stages.erase(it);
			return true;
		}
		else
		{
			DFW_WARNLOG("Trying to remove unknown stage at back with ID: {}.", a_id);
			return false;
		}
	}

	const std::vector<StageBase*>& StageStackController::GetStages() const
	{
		return _stages;
	}

} // End of namespace ~ DFW.
