#include <CoreSystems/Stage/StageStackController.h>

namespace DCore
{

	StageStackController::StageStackController()
		: _stack_communicator(nullptr)
		, _stage_insert_index(0)
	{
		_stages.reserve(MAX_SUPPORTED_STAGES);
	}

	StageStackController::~StageStackController()
	{
		RemoveAttachedStages();
		DeleteAttachedStages();
	}

	void StageStackController::RemoveAttachedStages()
	{
		for (StageBase*& it : _stages)
		{	
			RemoveStage(it->GetID());
		}
	}

	void StageStackController::DeleteAttachedStages()
	{
		for (StageBase*& it : _stages)
		{
			if (it != nullptr)
			{
				DFW_LOG("Deleting Stage with ID: {}", it->GetID());
				delete it;
				it = nullptr;
			}
		}

		_stages.clear();
	}

	void StageStackController::RemoveStage(StageID a_id)
	{
		const auto it = std::find_if(_stages.begin(), _stages.end(), [a_id](const StageBase* a_stage) { return a_stage->GetID() == a_id; });
		if (it != _stages.end())
		{
			StageBase* stage_ptr = (*it);
			delete stage_ptr;
			DFW_INFOLOG("Removing attached Stage - ID: {}", a_id);

			_stages.erase(it);
			--_stage_insert_index;
		}
		else
		{
			DFW_WARNLOG("Trying to remove unknown stage with ID: {}.", a_id);
		}
	}

	void StageStackController::RemoveStageBack(StageID a_id)
	{
		const auto it = std::find_if(_stages.begin(), _stages.end(), [a_id](const StageBase* a_stage) { return a_stage->GetID() == a_id; });
		if (it != _stages.end())
		{
			StageBase* stage_ptr = (*it);
			delete stage_ptr;
			DFW_INFOLOG("Removing attached Stage - ID: {}", a_id);

			_stages.erase(it);
		}
		else
		{
			DFW_WARNLOG("Trying to remove unknown back-stage with ID: {}.", a_id);
		}
	}

	const std::vector<StageBase*>& StageStackController::GetStages() const
	{
		return _stages;
	}

} // End of namespace ~ DCore.
