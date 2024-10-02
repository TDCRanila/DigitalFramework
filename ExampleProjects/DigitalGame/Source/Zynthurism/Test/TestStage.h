#pragma once

#include <DFW/CoreSystems/Stage/Stage.h>

namespace DGame
{
    class TestStage : public DFW::StageBase
    {
    public:
        TestStage(std::string const& a_stage_name, bool a_start_disabled = false);
        virtual ~TestStage() = default;

        virtual void OnUpdate() override;

        virtual void OnAttached() override;
        virtual void OnRemoved() override;

    };

} // End of namespace ~ DGame.
