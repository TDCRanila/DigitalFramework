#include <GameWorld/Physics/PhysicsSystem.h>

#include <GameWorld/TransformComponent.h>
#include <GameWorld/Physics/PhysicsLayers.h>
#include <GameWorld/Physics/PhysicsSystemContext.h>
#include <GameWorld/Physics/PhysicsComponent.h>
#include <GameWorld/Physics/JoltDebugRenderImpl.h>

#include <Modules/ECS/EntityEvents.h>

#include <CoreSystems/Events/EventDispatcher.h>

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
        JPH::BodyCreationSettings mesh_settings(
            &a_mesh_shape_settings
            , DUtility::GLMToJPH(a_transform.GetWorldTranslation())
            , DUtility::GLMToJPH(glm::quat(a_transform.GetWorldRotation()))
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
        JPH::BodyCreationSettings box_settings(
            // BoxShape gets deleted by JPH itself.
            new JPH::BoxShape(DUtility::GLMToJPH(a_extend))
            , DUtility::GLMToJPH(a_transform.GetWorldTranslation())
            , DUtility::GLMToJPH(glm::quat(a_transform.GetWorldRotation()))
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
        JPH::BodyCreationSettings sphere_settings(
            // SphereShape gets deleted by JPH itself.
            new JPH::SphereShape(a_sphere_radius)
            , DUtility::GLMToJPH(a_transform.GetWorldTranslation())
            , DUtility::GLMToJPH(glm::quat(a_transform.GetWorldRotation()))
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

    void PhysicsSystem::Init(DECS::EntityRegistry& /*a_registry*/)
    {
        _context->Init();
        JoltPhysics().SetGravity(DFW_PHYSICS_DEFAULT_GRAVITY_VECTOR);

        // Event
        ECSEventHandler().RegisterCallback<DECS::EntityDestroyedEvent, &PhysicsSystem::OnEntityDestroyedEvent>(this);
        
        // Debug Draw;
        _jolt_debug_renderer->Init();
    }

    void PhysicsSystem::Terminate(DECS::EntityRegistry& /*a_registry*/)
    {
        // Event
        ECSEventHandler().UnregisterCallback<DECS::EntityDestroyedEvent, &PhysicsSystem::OnEntityDestroyedEvent>(this);

        // Safely remove all bodies registered in the physcis system.
        JPH::BodyIDVector bodies;
        JoltPhysics().GetBodies(bodies);
        JoltBodyInterface().RemoveBodies(bodies.data(), bodies.size());

        _context->Terminate();
    }

    void PhysicsSystem::PreUpdate(DECS::EntityRegistry& /*a_registry*/)
    {
        AddAwaitingRigidBodies();

        if (_should_optimize_broadphase_layer)
        {
            JoltPhysics().OptimizeBroadPhase();

            _should_optimize_broadphase_layer = false;
        }
    }

    void PhysicsSystem::Update(DECS::EntityRegistry& a_registry)
    {
        SyncStaticRigidBodyTransforms(a_registry);

        _context->UpdatePhysicsWorld(DFW_PHYSICS_DELTATIME, DFW_PHYSICS_COLLISION_STEPS);
        
        SyncDynamicAndKinematicRigidBodyTransforms(a_registry);

        if (_should_debug_draw_shapes)
        {
            static JPH::BodyManager::DrawSettings draw_settings;
            draw_settings.mDrawShapeWireframe = true;
            JoltPhysics().DrawBodies(draw_settings, _jolt_debug_renderer.get());
        }
    }

    void PhysicsSystem::PostUpdate(DECS::EntityRegistry& a_registry)
    {
        RemoveMarkedRigidBodies();
    }

    void PhysicsSystem::OnEntityDestroyedEvent(DECS::EntityDestroyedEvent const& a_event)
    {
        if (RigidBodyComponent const* rigid_body_component = a_event.entity.TryGetComponent<RigidBodyComponent>())
            DestroyRigidBody(rigid_body_component->body_id);
    }

    void PhysicsSystem::SyncStaticRigidBodyTransforms(DECS::EntityRegistry& a_registry)
    {
        // Update the Static Rigidbody's transform with the Entity's transform.
        JPH::BodyInterface& body_interface = JoltBodyInterface();
        JPH::Quat quat_rotation;
        JPH::Vec3 translation;
        for (auto&& [entity, transform, rigid_body] : a_registry.ENTT().view<TransformComponent, RigidBodyComponent>().each())
        {
            DFW_ASSERT(!rigid_body.body_id.IsInvalid());

            if (!body_interface.IsAdded(rigid_body.body_id))
                continue;

            if (body_interface.GetMotionType(rigid_body.body_id) != JPH::EMotionType::Static)
                continue;

            quat_rotation = DUtility::GLMToJPH(glm::quat(transform.GetWorldRotation()));
            translation = DUtility::GLMToJPH(transform.GetWorldTranslation());

            body_interface.SetPositionAndRotationWhenChanged(rigid_body.body_id, translation, quat_rotation, JPH::EActivation::DontActivate);
        }
    }

    void PhysicsSystem::SyncDynamicAndKinematicRigidBodyTransforms(DECS::EntityRegistry& a_registry)
    {
        // Update the Entity's transform with the Dynamic/Kinematic Rigidbody's transform.
        JPH::BodyInterface& body_interface = JoltBodyInterface();
        JPH::Quat quat_rotation;
        JPH::Vec3 translation;
        for (auto&& [entity, transform, rigid_body] : a_registry.ENTT().view<TransformComponent, RigidBodyComponent>().each())
        {
            DFW_ASSERT(!rigid_body.body_id.IsInvalid());

            if (!body_interface.IsAdded(rigid_body.body_id))
                continue;

            if (body_interface.GetMotionType(rigid_body.body_id) == JPH::EMotionType::Static)
                continue;

            body_interface.GetPositionAndRotation(rigid_body.body_id, translation, quat_rotation);
            transform.SetTranslation(DUtility::JPHToGLM(translation));
            transform.SetRotation(DUtility::JPHToGLM(quat_rotation));
        }
    }

    void PhysicsSystem::AddAwaitingRigidBodies()
    {
        if (!_rigid_bodies_pending_spawn.empty())
        {
            for (JPH::BodyID const& body_id : _rigid_bodies_pending_spawn)
                JoltBodyInterface().AddBody(body_id, JPH::EActivation::Activate);

            _rigid_bodies_pending_spawn.clear();

            _should_optimize_broadphase_layer = true;
        }
    }

    void PhysicsSystem::RemoveMarkedRigidBodies()
    {
        class BodyCollector : public JPH::CollideShapeBodyCollector
        {
        public:
            BodyCollector(JPH::PhysicsSystem& a_jolt_physics) : _jolt_physics(a_jolt_physics) {}

            virtual void AddHit(const JPH::BodyID& inBodyID) override
            {
                JPH::BodyLockWrite lock(_jolt_physics.GetBodyLockInterface(), inBodyID);
                JPH::Body& body = lock.GetBody();
                if (!body.IsActive())
                    _body_ids.emplace_back(body.GetID());
            }

            virtual void Reset() override
            {
                JPH::CollideShapeBodyCollector::Reset();
                _body_ids.clear();
            }

            JPH::BodyIDVector const& GetHitBodyIDs() const { return _body_ids; }

        private:
            JPH::PhysicsSystem& _jolt_physics;
            JPH::BodyIDVector _body_ids;
        };

        // Remove rigid bodies awaiting destruction.
        if (!_rigid_bodies_pending_despawn.empty())
        {
            std::vector<JPH::BodyID> body_ids;
            body_ids.reserve(_rigid_bodies_pending_despawn.size());
            body_ids.assign(_rigid_bodies_pending_despawn.begin(), _rigid_bodies_pending_despawn.end());

            // Wake up an intersecting bodies with the remove body using broad phase query.
            BodyCollector collector(JoltPhysics());
            JPH::RMat44 body_transform;
            JPH::TransformedShape body_shape;
            for (JPH::BodyID const body_id : body_ids)
            {
                body_transform = JoltBodyInterface().GetWorldTransform(body_id);
                body_shape = JoltBodyInterface().GetTransformedShape(body_id);
                JoltPhysics().GetBroadPhaseQuery().CollideAABox(body_shape.GetWorldSpaceBounds(), collector);
                JoltBodyInterface().ActivateBodies(collector.GetHitBodyIDs().data(), collector.GetHitBodyIDs().size());
                collector.Reset();
            }

            // Finally attempt to remove physics bodies.
            JoltBodyInterface().RemoveBodies(body_ids.data(), body_ids.size());

            _rigid_bodies_pending_despawn.clear();

            _should_optimize_broadphase_layer = true;
        }
    }

} // End of namespace ~ DFW.
