#include <CoreSystems/Stage/Stage.h>

#include <CoreSystems/Logging/Logger.h>

namespace DFW
{
	StageBase::StageBase(std::string const& a_stage_name, bool a_start_disabled)
		: _name(a_stage_name)
		, _id(DFW::GenerateDUID())
		, _is_disabled(a_start_disabled)
	{
	}

	bool StageBase::operator==(StageBase const& a_other)
	{
		return this->_id == a_other._id;
	}

	void StageBase::Enable()
	{
		if (_is_disabled)
		{
			DFW_INFOLOG("StageID: {}, StageName: {} - being enabled.", _id, _name);
			_is_disabled = false;
			OnEnable();
		}
		else
		{
			DFW_INFOLOG("StageID: {}, StageName: {} - already enabled.", _id, _name);
		}
	}

	void StageBase::Disable()
	{
		if (!_is_disabled)
		{
			DFW_INFOLOG("StageID: {}, StageName: {} - being disabled.", _id, _name);
			_is_disabled = true;
			OnDisable();
		}
		else
		{
			DFW_INFOLOG("StageID: {}, StageName: {} - already disabled.", _id, _name);
		}
	}

	void StageBase::OnRender()
	{
	}

	void StageBase::OnRenderImGui()
	{
	}

	void StageBase::OnAttached()
	{
	}

	void StageBase::OnRemoved()
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
		
	StageID StageBase::GetID() const
	{
		return _id;
	}

	std::string StageBase::GetName() const
	{
		return _name;
	}

} // End of namespace ~ DFW.
