#pragma once

#include <GameWorld/Physics/PhysicsLayers.h>

#include <Jolt/Jolt.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/PhysicsSystem.h>

namespace DFW
{
    namespace DPhysics
    {
        constexpr int32 MAX_NUM_PHYSICS_JOBS = 2048;
        constexpr int32 MAX_NUM_PHYSICS_BARRIERS = 8;

        // This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
        uint32 const MAX_NUM_BODIES = 65536;

        // This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
        uint32 const NUM_BODY_MUTEXES = 0;

        // This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
        // body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
        // too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
        uint32 const MAX_NUM_BODY_PAIRS = 65536;

        // This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
        // number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
        uint32 const MAX_NUM_CONTACT_CONSTRAINTS = 10240;

        /// Class that determines if two object layers can collide
        class ObjectLayerPairFilter : public JPH::ObjectLayerPairFilter
        {
        public:
            virtual bool ShouldCollide(JPH::ObjectLayer a_object_a, JPH::ObjectLayer a_object_b) const override;
        };

        // BroadPhaseLayerInterface implementation
        // This defines a mapping between object and broadphase layers.
        class BPLayerInterface final : public JPH::BroadPhaseLayerInterface
        {
        public:
            BPLayerInterface();
            ~BPLayerInterface() = default;

            virtual uint32 GetNumBroadPhaseLayers() const override;

            virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer a_layer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
            virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer a_layer) const override;
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

        private:
            JPH::BroadPhaseLayer _object_to_broadphase[PhysicsLayers::NUM_LAYERS];
        };

        /// Class that determines if an object layer can collide with a broadphase layer
        class ObjectVsBroadPhaseLayerFilter : public JPH::ObjectVsBroadPhaseLayerFilter
        {
        public:
            virtual bool ShouldCollide(JPH::ObjectLayer a_layer_a, JPH::BroadPhaseLayer a_layer_b) const override;
        };

        class ContactListener : public JPH::ContactListener
        {
        public:
            virtual JPH::ValidateResult	OnContactValidate(
                JPH::Body const& a_body_a, JPH::Body const& a_body_b, JPH::RVec3Arg a_base_offset,JPH::CollideShapeResult const& a_collision_result) override;

            virtual void OnContactAdded(
                JPH::Body const& a_body_a, JPH::Body const& a_body_b, JPH::ContactManifold const& a_manifold, JPH::ContactSettings& a_contact_settings) override;

            virtual void OnContactPersisted(
                JPH::Body const& a_body_a, JPH::Body const& a_body_b, JPH::ContactManifold const& a_manifold, JPH::ContactSettings& a_contact_settings) override;

            virtual void OnContactRemoved(JPH::SubShapeIDPair const& a_subshape_pair) override;
        };

        class PhysicsSystemContext
        {
        public:
            PhysicsSystemContext();
            ~PhysicsSystemContext();

            void Init();
            void Terminate();

            void UpdatePhysicsWorld(float32 const a_delta_time, uint8 const a_physics_steps);

            UniquePtr<JPH::JobSystemThreadPool> jph_job_system;
            UniquePtr<JPH::PhysicsSystem> jph_physics_system;

        private:
            JPH::TempAllocatorImpl _temp_allocator;

            ContactListener _contact_listener;
            BPLayerInterface _broad_phase_layer_interface;
            ObjectVsBroadPhaseLayerFilter _object_vs_broadphase_layer_filter;
            ObjectLayerPairFilter _object_vs_object_layer_filter;

        };

    } // End of namespace ~ DPhysics.

} // End of namespace ~ DFW.
