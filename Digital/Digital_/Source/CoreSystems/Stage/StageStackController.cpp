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

} // End of namespace ~ DFW.
