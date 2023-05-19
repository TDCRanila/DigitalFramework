#pragma once

#include <GameWorld/Physics/PhysicsSystemContext.h>
#include <GameWorld/Transform.h>

#include <Modules/ECS/System.h>

#include <Jolt/Jolt.h>

#include <glm/glm.hpp>

#include <unordered_set>
#include <vector>

namespace JPH
{
    class PhysicsSystem;
    class BodyInterface;

} // End of namespace ~ JPH.

namespace DFW
{
    class GameClock;
    class JoltDebugRenderer;

    namespace DPhysics
    {
        class PhysicsSystemContext;

    } // End of namespace ~ DPhysics.

    // Default gravity.
    const JPH::Vec3 DFW_PHYSICS_DEFAULT_GRAVITY_VECTOR = JPH::Vec3(0.f, -10.f, 0.f);

    // We simulate the physics world in discrete time steps. 60 Hz is a good rate to update the physics system.
    constexpr float DFW_PHYSICS_DELTATIME = 1.0f / 60.0f;

    // Do 1 collision step per 1 / 60th of a second (round up).
    constexpr uint8 DFW_PHYSICS_COLLISION_STEPS = 1;

    // If you want more accurate step results you can do multiple sub steps within a collision step. Usually you would set this to 1.
    constexpr uint8 DFW_PHYSICS_INTEGRATION_SUBSTEPS = 1;

    class PhysicsSystem : public DECS::System
    {
    public:
        PhysicsSystem();
        virtual ~PhysicsSystem();

    public:
        JPH::PhysicsSystem& JoltPhysics();
        JPH::BodyInterface& JoltBodyInterface();

        void Debug_EnableDebugDraw();
        void Debug_DisableDebugDraw();

    public:
        JPH::BodyID CreateMeshRigidBody(Transform const& a_transform, JPH::ShapeSettings const& a_mesh_shape_settings, JPH::EMotionType const a_rigid_body_type);

        JPH::BodyID CreateBoxRigidBody(Transform const& a_transform, glm::vec3 const& a_extend, JPH::EMotionType const a_rigid_body_type);
        JPH::BodyID CreateBoxRigidBody(glm::vec3 const& a_extend, JPH::EMotionType const a_rigid_body_type);

        JPH::BodyID CreateSphereRigidBody(Transform const& a_transform, float32 const a_sphere_radius, JPH::EMotionType const a_rigid_body_type);
        JPH::BodyID CreateSphereRigidBody(float32 const a_sphere_radius, JPH::EMotionType const a_rigid_body_type);

        void DestroyRigidBody(JPH::BodyID const a_rigid_body_id);

    private:
        virtual void Init() override;
        virtual void Terminate() override;

        virtual void PreUpdate(DECS::EntityRegistry& a_registry) override;
        virtual void Update(DECS::EntityRegistry& a_registry) override;

    private:
        UniquePtr<DPhysics::PhysicsSystemContext> _context;
        UniquePtr<JoltDebugRenderer> _jolt_debug_renderer;
        
        std::vector<JPH::BodyID> _rigid_bodies_pending_spawn;
        std::unordered_set<JPH::BodyID> _rigid_bodies_pending_despawn;
        bool _should_optimize_broadphase_layer;

        bool _should_debug_draw_shapes;
    };


} // End of namespace ~ DFW.
