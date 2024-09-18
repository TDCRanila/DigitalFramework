#pragma once

#include <GameWorld/Transform.h>

#include <Modules/ECS/System.h>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Physics/Body/MotionType.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Character/Character.h>

#include <Utility/JoltUtility.h>

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

    namespace DECS
    {
        class EntityDestroyedEvent;
    } // End of namespace ~ DECS.

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
        void AddRigidBodyToSystem(JPH::BodyID const a_body_id) { _rigid_bodies_pending_spawn.emplace_back(a_body_id); }

        JPH::BodyID CreateMeshRigidBody(Transform const& a_transform, JPH::ShapeSettings const& a_mesh_shape_settings, JPH::EMotionType const a_rigid_body_type);

        JPH::BodyID CreateBoxRigidBody(Transform const& a_transform, glm::vec3 const& a_extend, JPH::EMotionType const a_rigid_body_type);
        JPH::BodyID CreateBoxRigidBody(glm::vec3 const& a_extend, JPH::EMotionType const a_rigid_body_type);

        JPH::BodyID CreateSphereRigidBody(Transform const& a_transform, float32 const a_sphere_radius, JPH::EMotionType const a_rigid_body_type);
        JPH::BodyID CreateSphereRigidBody(float32 const a_sphere_radius, JPH::EMotionType const a_rigid_body_type);

        JPH::Ref<JPH::Character> CreateCharacter(Transform const& a_transform, JPH::CharacterSettings const& a_charachter_settings);

        void DestroyRigidBody(JPH::BodyID const a_rigid_body_id);

    private:
        virtual void Init(DECS::EntityRegistry& a_registry) override;
        virtual void Terminate(DECS::EntityRegistry& a_registry) override;

        virtual void PreUpdate(DECS::EntityRegistry& a_registry) override;
        virtual void Update(DECS::EntityRegistry& a_registry) override;
        virtual void PostUpdate(DECS::EntityRegistry& a_registry) override;

        void OnEntityDestroyedEvent(DECS::EntityDestroyedEvent const& a_event);

        void SyncStaticRigidBodyTransforms(DECS::EntityRegistry& a_registry);
        void SyncDynamicAndKinematicRigidBodyTransforms(DECS::EntityRegistry& a_registry);

        void AddAwaitingRigidBodies();
        void RemoveMarkedRigidBodies();

    private:
        UniquePtr<DPhysics::PhysicsSystemContext> _context;
        UniquePtr<JoltDebugRenderer> _jolt_debug_renderer;
        
        std::vector<JPH::BodyID> _rigid_bodies_pending_spawn;
        std::unordered_set<JPH::BodyID> _rigid_bodies_pending_despawn;
        bool _should_optimize_broadphase_layer;

        bool _should_debug_draw_shapes;
    };


} // End of namespace ~ DFW.
