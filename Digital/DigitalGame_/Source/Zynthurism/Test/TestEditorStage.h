#pragma once

#include <GameWorld/Editor/EditorStage.h>

#include <GameWorld/GameWorld.h>

namespace DGame
{
    class TestEditorStage : public DFW::DEditor::EditorStage
    {
    public:
        TestEditorStage(std::string const& a_stage_name, bool a_start_disabled = false);
        virtual ~TestEditorStage() = default;

        virtual void OnUpdate() override;
        virtual void OnRenderImGui() override;

        virtual void OnAttached() override;
        virtual void OnRemoved() override;

    private:
        DFW::Entity main_entity;
        DFW::Entity camera_entity;

    };

} // End of namespace ~ DGame.
