#pragma once

#include <GameWorld/Graphics/BaseRenderSystem.h>
#include <Utility/ColourUtility.h>

#include <glm/glm.hpp>

// FW Declare.
struct DebugDrawEncoder;

namespace DFW
{
    class JoltDebugRenderer;

    namespace DRender
    {
        struct TextureData;
    }

    struct DebugDrawSettings
    {
        DebugDrawSettings();
        DebugDrawSettings(ColourRGBA a_colour, bool a_enable_wireframe);
        DebugDrawSettings(ColourRGBA a_colour, bool a_enable_wireframe, bool a_enable_depthtest, uint8 a_shape_complexity);

        ColourRGBA colour;
        uint8 shape_complexity_lod;
        bool enable_wireframe;
        bool enable_depthtest;

    };

    class DebugRenderSystem final : public DECS::System::Registrar<DebugRenderSystem, BaseRenderSystem>
    {
    public:
        friend JoltDebugRenderer;

        DebugRenderSystem() = default;
        virtual ~DebugRenderSystem() = default;

    public:
        void DrawLine(glm::vec3 const& a_start_pos, glm::vec3 const& a_end_pos, DebugDrawSettings const& a_settings);
        void DrawLine(glm::vec3 const& a_start_pos, glm::vec3 const& a_end_pos, float32 a_line_thickness, DebugDrawSettings const& a_settings);

        void DrawArrow(glm::vec3 const& a_start_pos, glm::vec3 const& a_end_pos, float32 a_arrow_thickness, DebugDrawSettings const& a_settings);
        void DrawArrow(glm::vec3 const& a_start_pos, glm::vec3 const& a_direction, float32 a_arrow_length, float32 a_arrow_thickness, DebugDrawSettings const& a_settings);

        void DrawCircle(glm::vec3 const& a_center_pos, glm::vec3 const& a_normal, float32 a_circle_radius, DebugDrawSettings const& a_settings);
        void DrawTriangle(glm::vec3 const& a_center_pos, glm::vec3 const& a_normal, glm::vec3 const& a_extend_from_center, DebugDrawSettings const& a_settings);
        void DrawRectangle(glm::vec3 const& a_center_pos, glm::vec3 const& a_normal, float32 a_size, DebugDrawSettings const& a_settings);
        void DrawSprite(glm::vec3 const& a_center_pos, glm::vec3 const& a_normal, DRender::TextureData const& a_texture, DebugDrawSettings const& a_settings);

        void DrawCube(Transform const& a_transform, float32 a_box_extend, DebugDrawSettings const& a_settings);
        void DrawBox(Transform a_transform, glm::vec3 const& a_extend, DebugDrawSettings const& a_settings);

        void DrawSphere(Transform const& a_transform, float32 a_sphere_radius, DebugDrawSettings const& a_settings);
        void DrawCylinder(Transform const& a_transform, float32 a_cylinder_height, float32 a_cylinder_radius, DebugDrawSettings const& a_settings);
        void DrawCapsule(Transform const& a_transform, float32 a_capsule_height, float32 a_capsule_radius, DebugDrawSettings const& a_settings);
        void DrawCone(Transform const& a_transform, float32 a_cone_height, float32 a_cone_radius, DebugDrawSettings const& a_settings);

        void DrawOrb(Transform const& a_transform, float32 a_orb_radius, DebugDrawSettings const& a_settings);
        void DrawAxis(Transform const& a_transform, float32 a_axis_length, DebugDrawSettings const& a_settings);

        void DrawGridX(glm::vec3 const& a_center_pos, uint32 a_grid_size, float32 a_grid_step, DebugDrawSettings const& a_settings);
        void DrawGridY(glm::vec3 const& a_center_pos, uint32 a_grid_size, float32 a_grid_step, DebugDrawSettings const& a_settings);
        void DrawGridZ(glm::vec3 const& a_center_pos, uint32 a_grid_size, float32 a_grid_step, DebugDrawSettings const& a_settings);
        void DrawGrid(glm::vec3 const& a_center_pos, glm::vec3 const& a_normal, uint32 a_grid_size, float32 a_grid_step, DebugDrawSettings const& a_settings);

        void DrawFrustum(glm::mat4 const& a_view_projection_matrix, DebugDrawSettings const& a_settings);
        
    private:
        virtual void Init() override;
        virtual void Terminate() override;

        virtual void Update(DECS::EntityRegistry& a_registry) override;

    private:
        static DebugDrawEncoder* _dde;

    };

} // End of namespace ~ DRender.
