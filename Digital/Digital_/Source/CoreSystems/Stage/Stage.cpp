#include <CoreSystems/Stage/Stage.h>

#include <CoreSystems/Logging/Logger.h>

namespace DCore
{

	StageBase::StageBase(std::string a_stage_name, bool a_start_disabled)
		: _name(a_stage_name)
		, _id(DUIDGenerator::GenerateID())
		, _is_disabled(a_start_disabled)
	{
	}

	void StageBase::RenderImGui()
	{

	}

	bool StageBase::IsDisabled() const
	{
		return _is_disabled;
	}

	void StageBase::OnEnable()
	{

	}

	void StageBase::OnDisable()
	{

	}
	
	void StageBase::OnApplicationEvent(ApplicationEvent& a_event)
	{

	}
	
	StageID StageBase::GetID() const
	{
		return _id;
	}

	std::string StageBase::GetName() const
	{
		return _name;
	}

	void StageBase::Enable()
	{
		if (_is_disabled)
		{
			_is_disabled = false;
			OnEnable();
			DFW_INFOLOG("StageID: {}, StageName: {} - being enabled.", _id, _name);
		}
		else
		{
			DFW_INFOLOG("StageID: {}, StageName: {} - already enabled.", _id, _name);
		}
	}

	void StageBase::Disable()
	{
		if (_is_disabled)
		{
			_is_disabled = true;
			OnDisable();
			DFW_INFOLOG("StageID: {}, StageName: {} - being disabled.", _id, _name);
		}
		else
		{
			DFW_INFOLOG("StageID: {}, StageName: {} - already disabled.", _id, _name);
		}
	}

} // End of namespace ~ DCore.
