#include <GameWorld/Graphics/DebugRenderSystem.h>

#include <GameWorld/TransformComponent.h>

#include <Modules/Rendering/RenderModule.h>
#include <Modules/Rendering/DebugDraw/debugdraw.h> 
#include <Modules/Rendering/ViewTargetDirector.h>

#include <CoreSystems/CoreServices.h>
#include <CoreSystems/Events/EventDispatcher.h>

#include <glm/gtc/type_ptr.hpp>

#include <bx/math.h>

namespace DFW
{
    DebugDrawEncoder* DebugRenderSystem::_dde = nullptr;

    DebugDrawSettings::DebugDrawSettings()
        : colour({255, 255, 255 ,255})
        , shape_complexity_lod(2)
        , enable_wireframe(true)
        , enable_depthtest(false)
    {
    }

    DebugDrawSettings::DebugDrawSettings(ColourRGBA a_colour, bool a_enable_wireframe)
        : colour(a_colour)
        , shape_complexity_lod(2)
        , enable_wireframe(a_enable_wireframe)
        , enable_depthtest(false)
    {
    }

    DebugDrawSettings::DebugDrawSettings(ColourRGBA a_colour, bool a_enable_wireframe, bool a_enable_depthtest, uint8 a_shape_complexity)
        : colour(a_colour)
        , enable_wireframe(a_enable_wireframe)
        , enable_depthtest(a_enable_depthtest)
        , shape_complexity_lod(a_shape_complexity)
    {
    }

    void DebugRenderSystem::Init()
    {
        SharedPtr<DRender::RenderModule> render_module = CoreService::GetRenderModule();

        // View Target
        _view_target = render_module->GetViewDirector().AllocateViewTarget("debugrendersystem", DRender::ViewTargetInsertion::Front);

        // Register Callbacks.
        CoreService::GetAppEventHandler()->RegisterCallback<WindowResizeEvent, &BaseRenderSystem::OnWindowResizeEvent>(this);
        ECSEventHandler().RegisterCallback<CameraNewActiveEvent, &BaseRenderSystem::OnCameraNewActiveEvent>(this);

        // Initialize Debug Drawer.
        ddInit();
        _dde = new DebugDrawEncoder();
        _dde->setupEncoder(_view_target->view_target_id);
    }

    void DebugRenderSystem::Terminate()
    {

        // Unregister Callbacks.
        CoreService::GetAppEventHandler()->UnregisterCallback<WindowResizeEvent, &BaseRenderSystem::OnWindowResizeEvent>(this);
        ECSEventHandler().UnregisterCallback<CameraNewActiveEvent, &BaseRenderSystem::OnCameraNewActiveEvent>(this);

        // Terminate Debug Drawer.
        delete _dde;
        ddShutdown();
    }

    void DebugRenderSystem::Update(DECS::EntityRegistry& /*a_registry*/)
    {
        PrepareRenderTarget();
        PrepareViewTarget();
        
        _dde->finalizeFrame();
    }

    void DebugRenderSystem::DrawLine(glm::vec3 const& a_start_pos, glm::vec3 const& a_end_pos, DebugDrawSettings const& a_settings)
    {
        a_settings.colour;

        _dde->setColor(a_settings.colour.GetABGRHex());
        _dde->moveTo(a_start_pos.x, a_start_pos.y, a_start_pos.z);
        _dde->lineTo(a_end_pos.x, a_end_pos.y, a_end_pos.z);
    }

    void DebugRenderSystem::DrawLine(glm::vec3 const& a_start_pos, glm::vec3 const& a_end_pos, float32 a_line_thickness, DebugDrawSettings const& a_settings)
    {
        if (a_line_thickness <= 0.0f)
            a_line_thickness = 0.5f;

        _dde->setColor(a_settings.colour.GetABGRHex());
        _dde->drawCylinder(bx::Vec3(a_start_pos.x, a_start_pos.y, a_start_pos.z), bx::Vec3(a_end_pos.x, a_end_pos.y, a_end_pos.z), a_line_thickness * 0.01f);
    }

    void DebugRenderSystem::DrawCircle(glm::vec3 const& a_center_pos, glm::vec3 const& a_normal, float32 a_circle_radius, DebugDrawSettings const& a_settings)
    {
        _dde->setColor(a_settings.colour.GetABGRHex());
        _dde->setWireframe(a_settings.enable_wireframe);
        _dde->setDepthTestLess(!a_settings.enable_depthtest);
        _dde->drawCircle(bx::normalize({ a_normal.x, a_normal.y, a_normal.z }), { a_center_pos.x, a_center_pos.y, a_center_pos.z }, a_circle_radius);
    }

    void DebugRenderSystem::DrawRectangle(glm::vec3 const& a_center_pos, glm::vec3 const& a_normal, float32 a_size, DebugDrawSettings const& a_settings)
    {
        _dde->setColor(a_settings.colour.GetABGRHex());
        _dde->setWireframe(a_settings.enable_wireframe);
        _dde->setDepthTestLess(!a_settings.enable_depthtest);
        _dde->drawQuad({ a_normal.x, a_normal.y, a_normal.z }, { a_center_pos.x, a_center_pos.y, a_center_pos.z }, a_size);
    }

    void DebugRenderSystem::DrawCube(Transform const& a_transform, float32 a_box_extend, DebugDrawSettings const& a_settings)
    {
        DrawBox(a_transform, glm::vec3(a_box_extend), a_settings);
    }

    void DebugRenderSystem::DrawBox(Transform a_transform, glm::vec3 const& a_extend, DebugDrawSettings const& a_settings)
    {
        bx::Obb obb;
        a_transform.SetScale(a_transform.GetScale() * a_extend);
        bx::memCopy(obb.mtx, glm::value_ptr(a_transform.GetWorldTransformMatrix()), sizeof(glm::mat4));
        
        _dde->setColor(a_settings.colour.GetABGRHex());
        _dde->setWireframe(a_settings.enable_wireframe);
        _dde->setDepthTestLess(!a_settings.enable_depthtest);
        _dde->draw(obb);

    }

    void DebugRenderSystem::DrawSphere(Transform const& a_transform, float32 a_sphere_radius, DebugDrawSettings const& a_settings)
    {
        bx::Sphere sphere;
        sphere.center = { 0.0f, 0.0f, 0.0f };
        sphere.radius = a_sphere_radius;
        
        _dde->setTransform(glm::value_ptr(a_transform.GetWorldTransformMatrix()));
        _dde->setLod(a_settings.shape_complexity_lod);
        _dde->setColor(a_settings.colour.GetABGRHex());
        _dde->setWireframe(a_settings.enable_wireframe);
        _dde->setDepthTestLess(!a_settings.enable_depthtest);
        _dde->draw(sphere);
        _dde->setTransform(nullptr);
    }

    void DebugRenderSystem::DrawCylinder(Transform const& a_transform, float32 a_cylinder_height, float32 a_cylinder_radius, DebugDrawSettings const& a_settings)
    {
        bx::Cylinder cylinder;
        cylinder.pos = bx::Vec3(0.0f, -(a_cylinder_height * 0.5f), 0.0f);
        cylinder.end = bx::Vec3(0.0f,  (a_cylinder_height * 0.5f), 0.0f);
        cylinder.radius = a_cylinder_radius;

        _dde->setTransform(glm::value_ptr(a_transform.GetWorldTransformMatrix()));
        _dde->setLod(a_settings.shape_complexity_lod);
        _dde->setColor(a_settings.colour.GetABGRHex());
        _dde->setWireframe(a_settings.enable_wireframe);
        _dde->setDepthTestLess(!a_settings.enable_depthtest);
        _dde->draw(cylinder);
        _dde->setTransform(nullptr);
    }

    void DebugRenderSystem::DrawCapsule(Transform const& a_transform, float32 a_capsule_height, float32 a_capsule_radius, DebugDrawSettings const& a_settings)
    {
        bx::Capsule capsule;
        capsule.pos = bx::Vec3(0.0f, -(a_capsule_height * 0.5f), 0.0f);
        capsule.end = bx::Vec3(0.0f,  (a_capsule_height * 0.5f), 0.0f);
        capsule.radius = a_capsule_radius;

        _dde->setTransform(glm::value_ptr(a_transform.GetWorldTransformMatrix()));
        _dde->setLod(a_settings.shape_complexity_lod);
        _dde->setColor(a_settings.colour.GetABGRHex());
        _dde->setWireframe(a_settings.enable_wireframe);
        _dde->setDepthTestLess(!a_settings.enable_depthtest);
        _dde->draw(capsule);
        _dde->setTransform(nullptr);
    }

    void DebugRenderSystem::DrawCone(Transform const& a_transform, float32 a_cone_height, float32 a_cone_radius, DebugDrawSettings const& a_settings)
    {
        bx::Cone cone;
        cone.pos = bx::Vec3(0.0f, 0.0f , 0.0f);
        cone.end = bx::Vec3(0.0f, a_cone_height , 0.0f);
        cone.radius = a_cone_radius;

        _dde->setTransform(glm::value_ptr(a_transform.GetWorldTransformMatrix()));
        _dde->setLod(a_settings.shape_complexity_lod);
        _dde->setColor(a_settings.colour.GetABGRHex());
        _dde->setWireframe(a_settings.enable_wireframe);
        _dde->setDepthTestLess(!a_settings.enable_depthtest);
        _dde->draw(cone);
        _dde->setTransform(nullptr);
    }

    void DebugRenderSystem::DrawOrb(Transform const& a_transform, float32 a_orb_radius, DebugDrawSettings const& a_settings)
    {
        _dde->setColor(a_settings.colour.GetABGRHex());
        _dde->setWireframe(a_settings.enable_wireframe);
        _dde->setDepthTestLess(!a_settings.enable_depthtest);
        _dde->drawOrb(a_transform.GetWorldTranslation().x, a_transform.GetWorldTranslation().y, a_transform.GetWorldTranslation().z, a_orb_radius);
    }

    void DebugRenderSystem::DrawAxis(Transform const& a_transform, float32 a_axis_length, DebugDrawSettings const& a_settings)
    {
        _dde->setColor(a_settings.colour.GetABGRHex());
        _dde->setWireframe(a_settings.enable_wireframe);
        _dde->setDepthTestLess(!a_settings.enable_depthtest);
        _dde->drawAxis(a_transform.GetWorldTranslation().x, a_transform.GetWorldTranslation().y, a_transform.GetWorldTranslation().z, a_axis_length);
    }

    void DebugRenderSystem::DrawGridX(glm::vec3 const& a_center_pos, uint32 a_grid_size, float32 a_grid_step, DebugDrawSettings const& a_settings)
    {
        _dde->drawGrid(Axis::X, { a_center_pos.x, a_center_pos.y, a_center_pos.z }, a_grid_size, a_grid_step);
    }

    void DebugRenderSystem::DrawGridY(glm::vec3 const& a_center_pos, uint32 a_grid_size, float32 a_grid_step, DebugDrawSettings const& a_settings)
    {
        _dde->drawGrid(Axis::Y, { a_center_pos.x, a_center_pos.y, a_center_pos.z }, a_grid_size, a_grid_step);
    }

    void DebugRenderSystem::DrawGridZ(glm::vec3 const& a_center_pos, uint32 a_grid_size, float32 a_grid_step, DebugDrawSettings const& a_settings)
    {
        _dde->drawGrid(Axis::Z, { a_center_pos.x, a_center_pos.y, a_center_pos.z }, a_grid_size, a_grid_step);
    }

    void DebugRenderSystem::DrawGrid(glm::vec3 const& a_center_pos, glm::vec3 const& a_normal, uint32 a_grid_size, float32 a_grid_step, DebugDrawSettings const& a_settings)
    {
        _dde->drawGrid(bx::normalize({a_normal.x, a_normal.y, a_normal.z }), { a_center_pos.x, a_center_pos.y, a_center_pos.z }, a_grid_size, a_grid_step);
    }

    void DebugRenderSystem::DrawFrustum(glm::mat4 const& a_view_projection_matrix, DebugDrawSettings const& a_settings)
    {
        _dde->setColor(a_settings.colour.GetABGRHex());
        _dde->setWireframe(a_settings.enable_wireframe);
        _dde->setDepthTestLess(!a_settings.enable_depthtest);
        _dde->drawFrustum(glm::value_ptr(a_view_projection_matrix));
    }

} // End of namespace ~ DFW.
