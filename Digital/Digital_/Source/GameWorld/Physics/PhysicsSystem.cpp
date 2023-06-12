#include <GameWorld/Physics/PhysicsSystem.h>

#include <GameWorld/TransformComponent.h>
#include <GameWorld/Physics/PhysicsLayers.h>
#include <GameWorld/Physics/PhysicsSystemContext.h>
#include <GameWorld/Physics/PhysicsComponent.h>
#include <GameWorld/Physics/JoltDebugRenderImpl.h>

#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace DFW
{
    using namespace JPH::literals;

    namespace Detail
    {
        constexpr uint8 DFW_PHYSICS_PENDING_BUFFER_RESERVATION = 64;

        uint8 GetPhysicsLayerIDFromMotionType(JPH::EMotionType a_motion_type)
        {
            switch (a_motion_type) {
            case (JPH::EMotionType::Dynamic):
                return PhysicsLayers::MOVING;
            case (JPH::EMotionType::Static):
                return PhysicsLayers::NON_MOVING;
            default:
                DFW_ASSERT(false);
                break;
            }

            return PhysicsLayers::NUM_LAYERS;
        }
    }

    PhysicsSystem::PhysicsSystem()
        : _should_optimize_broadphase_layer(false)
        , _should_debug_draw_shapes(false)
    {
        _context = MakeUnique<DPhysics::PhysicsSystemContext>();
        _jolt_debug_renderer = MakeUnique<JoltDebugRenderer>();
        _rigid_bodies_pending_spawn.reserve(Detail::DFW_PHYSICS_PENDING_BUFFER_RESERVATION);
        _rigid_bodies_pending_despawn.reserve(Detail::DFW_PHYSICS_PENDING_BUFFER_RESERVATION);
    }

    PhysicsSystem::~PhysicsSystem() = default;

    JPH::PhysicsSystem& PhysicsSystem::JoltPhysics()
    { 
        return *_context->jph_physics_system;
    }
    
    JPH::BodyInterface& PhysicsSystem::JoltBodyInterface() 
    { 
        return _context->jph_physics_system->GetBodyInterface(); 
    }
       
    void PhysicsSystem::Debug_EnableDebugDraw()
    {
        _should_debug_draw_shapes = true;
    }

    void PhysicsSystem::Debug_DisableDebugDraw()
    {
        _should_debug_draw_shapes = false;
    }

    JPH::BodyID PhysicsSystem::CreateMeshRigidBody(Transform const& a_transform, JPH::ShapeSettings const& a_mesh_shape_settings, JPH::EMotionType const a_rigid_body_type)
    {
        JPH::RVec3 const initial_rigidbody_translation(a_transform.GetWorldTranslation().x, a_transform.GetWorldTranslation().y, a_transform.GetWorldTranslation().z);
        JPH::Quat const intial_rigidbody_quat(JPH::Quat::sEulerAngles({ a_transform.GetWorldRotation().x, a_transform.GetWorldRotation().y, a_transform.GetWorldRotation().z}));
        JPH::BodyCreationSettings mesh_settings(
            &a_mesh_shape_settings
            , initial_rigidbody_translation
            , intial_rigidbody_quat
            , a_rigid_body_type
            , Detail::GetPhysicsLayerIDFromMotionType(a_rigid_body_type)
        );

        JPH::Body* const body = JoltBodyInterface().CreateBody(mesh_settings);
        DFW_ASSERT(body, "Failed to create rigid body.");
        JPH::BodyID const& body_id = body->GetID();

        _rigid_bodies_pending_spawn.emplace_back(body_id);

        return body_id;
    }

    JPH::BodyID PhysicsSystem::CreateBoxRigidBody(Transform const& a_transform, glm::vec3 const& a_extend, JPH::EMotionType const a_rigid_body_type)
    {
        // Create the RigidBody.
        JPH::Vec3 const box_extend(a_extend.x, a_extend.y, a_extend.z);
        JPH::RVec3 const initial_rigidbody_translation(a_transform.GetWorldTranslation().x, a_transform.GetWorldTranslation().y, a_transform.GetWorldTranslation().z);
        JPH::Quat const intial_rigidbody_quat(JPH::Quat::sEulerAngles({ a_transform.GetWorldRotation().x, a_transform.GetWorldRotation().y, a_transform.GetWorldRotation().z }));
        JPH::BodyCreationSettings box_settings(
            // BoxShape gets deleted by JPH itself.
            new JPH::BoxShape(box_extend)
            , initial_rigidbody_translation
            , intial_rigidbody_quat
            , a_rigid_body_type
            , Detail::GetPhysicsLayerIDFromMotionType(a_rigid_body_type)
        );

        JPH::Body* const body = JoltBodyInterface().CreateBody(box_settings);
        DFW_ASSERT(body, "Failed to create rigid body.");
        JPH::BodyID const& body_id = body->GetID();

        _rigid_bodies_pending_spawn.emplace_back(body_id);

        return body_id;
    }

    JPH::BodyID PhysicsSystem::CreateBoxRigidBody(glm::vec3 const& a_extend, JPH::EMotionType const a_rigid_body_type)
    {
        return CreateBoxRigidBody(Transform(), a_extend, a_rigid_body_type);
    }

    JPH::BodyID PhysicsSystem::CreateSphereRigidBody(Transform const& a_transform, float32 const a_sphere_radius, JPH::EMotionType const a_rigid_body_type)
    {
        // Create RigidBody.        
        JPH::RVec3 const initial_rigidbody_translation(a_transform.GetWorldTranslation().x, a_transform.GetWorldTranslation().y, a_transform.GetWorldTranslation().z);
        JPH::Quat const intial_rigidbody_quat(JPH::Quat::sEulerAngles({ a_transform.GetWorldRotation().x, a_transform.GetWorldRotation().y, a_transform.GetWorldRotation().z }));
        JPH::BodyCreationSettings sphere_settings(
            // SphereShape gets deleted by JPH itself.
            new JPH::SphereShape(a_sphere_radius)
            , initial_rigidbody_translation
            , intial_rigidbody_quat
            , a_rigid_body_type
            , Detail::GetPhysicsLayerIDFromMotionType(a_rigid_body_type)
        );

        JPH::Body* const body = JoltBodyInterface().CreateBody(sphere_settings);
        DFW_ASSERT(body, "Failed to create rigid body.");

        JPH::BodyID const& body_id = body->GetID();
        _rigid_bodies_pending_spawn.emplace_back(body_id);

        return body_id;
    }

    JPH::BodyID PhysicsSystem::CreateSphereRigidBody(float32 const a_sphere_radius, JPH::EMotionType const a_rigid_body_type)
    {
        return CreateSphereRigidBody(Transform(), a_sphere_radius, a_rigid_body_type);
    }

    void PhysicsSystem::DestroyRigidBody(JPH::BodyID const a_rigid_body_id)
    {
        if (JoltBodyInterface().IsAdded(a_rigid_body_id))
            _rigid_bodies_pending_despawn.emplace(a_rigid_body_id);
    }

    void PhysicsSystem::Init()
    {
        _context->Init();
        JoltPhysics().SetGravity(DFW_PHYSICS_DEFAULT_GRAVITY_VECTOR);

        // Debug Draw;
        _jolt_debug_renderer->Init();
    }

    void PhysicsSystem::Terminate()
    {
        _context->Terminate();
    }

    void PhysicsSystem::PreUpdate(DECS::EntityRegistry& /*a_registry*/)
    {
        // Add rigid bodies awaiting spawn.
        if (!_rigid_bodies_pending_spawn.empty())
        {
            for (JPH::BodyID const& body_id : _rigid_bodies_pending_spawn)
                JoltBodyInterface().AddBody(body_id, JPH::EActivation::Activate);

            _rigid_bodies_pending_spawn.clear();

            _should_optimize_broadphase_layer = true;
        }

        // Remove rigid bodies awaiting destruction.
        if (!_rigid_bodies_pending_despawn.empty())
        {
            std::vector<JPH::BodyID> body_ids;
            body_ids.reserve(_rigid_bodies_pending_despawn.size());
            body_ids.assign(_rigid_bodies_pending_despawn.begin(), _rigid_bodies_pending_despawn.end());
            JoltBodyInterface().RemoveBodies(body_ids.data(), body_ids.size());

            _rigid_bodies_pending_despawn.clear();

            _should_optimize_broadphase_layer = true;
        }

        if (_should_optimize_broadphase_layer)
        {
            JoltPhysics().OptimizeBroadPhase();

            _should_optimize_broadphase_layer = false;
        }
    }

    void PhysicsSystem::Update(DECS::EntityRegistry& a_registry)
    {
        _context->UpdatePhysicsWorld(DFW_PHYSICS_DELTATIME, DFW_PHYSICS_COLLISION_STEPS, DFW_PHYSICS_INTEGRATION_SUBSTEPS);

        // Sync Transform
        JPH::BodyInterface& body_interface = JoltBodyInterface();
        JPH::Quat quat_rotation;
        JPH::Vec3 translation;
        for (auto&& [entity, transform, rigid_body] : a_registry.ENTT().view<TransformComponent, RigidBodyComponent>().each())
        {
            if (rigid_body.body_id.IsInvalid())
                continue;

            if (!body_interface.IsAdded(rigid_body.body_id))
                continue;

            glm::quat quat(transform.GetWorldRotation());
            quat_rotation.mValue.SetX(quat.x);
            quat_rotation.mValue.SetY(quat.y);
            quat_rotation.mValue.SetZ(quat.z);
            quat_rotation.mValue.SetW(quat.w);

            translation.SetX(transform.GetWorldTranslation().x);
            translation.SetY(transform.GetWorldTranslation().y);
            translation.SetZ(transform.GetWorldTranslation().z);

            body_interface.SetPositionAndRotationWhenChanged(rigid_body.body_id, translation, quat_rotation, JPH::EActivation::DontActivate);
        }

        if (_should_debug_draw_shapes)
        {
            static JPH::BodyManager::DrawSettings draw_settings;
            draw_settings.mDrawShapeWireframe = true;
            JoltPhysics().DrawBodies(draw_settings, _jolt_debug_renderer.get());

        }
    }

} // End of namespace ~ DFW.
