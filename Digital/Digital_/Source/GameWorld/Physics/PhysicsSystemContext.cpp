#include <GameWorld/Physics/PhysicsSystemContext.h>

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>

#include <CoreSystems/Logging/Logger.h>

#include <thread>

namespace DFW
{
    namespace DPhysics
    {
    namespace Detail
    {
        static void TraceLogImplementation(const char* a_formatted_trace, ...)
        {
            // Format the message
            va_list list;
            va_start(list, a_formatted_trace);
            char buffer[1024];
            vsnprintf(buffer, sizeof(buffer), a_formatted_trace, list);
            va_end(list);

            DFW_LOG(buffer);
        }

        static bool AssertFailedImplementation(const char* a_expression, const char* a_message, const char* a_file, uint32 a_line)
        {
            const char* message = a_message != nullptr ? a_message : "";
            DFW_ASSERT(false, message);

            return true;
        }
    }

    bool ObjectLayerPairFilter::ShouldCollide(JPH::ObjectLayer a_object_a, JPH::ObjectLayer a_object_b) const
    {
        switch (a_object_a)
        {
        case PhysicsLayers::NON_MOVING:
            return a_object_b == PhysicsLayers::MOVING; // Non moving only collides with moving
        case PhysicsLayers::MOVING:
            return true; // Moving collides with everything
        default:
            DFW_ASSERT(false);
            return false;
        }
    }

    BPLayerInterface::BPLayerInterface()
    {
        // Create a mapping table from object to broad phase layer
        _object_to_broadphase[PhysicsLayers::NON_MOVING] = BroadPhasePhysicsLayers::NON_MOVING;
        _object_to_broadphase[PhysicsLayers::MOVING] = BroadPhasePhysicsLayers::MOVING;
    }

    uint32 BPLayerInterface::GetNumBroadPhaseLayers() const
    {
        return BroadPhasePhysicsLayers::NUM_LAYERS;
    }

    JPH::BroadPhaseLayer BPLayerInterface::GetBroadPhaseLayer(JPH::ObjectLayer a_layer) const
    {
        DFW_ASSERT(a_layer < PhysicsLayers::NUM_LAYERS);
        return _object_to_broadphase[a_layer];
    }

    const char* BPLayerInterface::GetBroadPhaseLayerName(JPH::BroadPhaseLayer a_layer) const
    {
        switch (static_cast<JPH::BroadPhaseLayer::Type>(a_layer))
        {
        case (static_cast<JPH::BroadPhaseLayer::Type>(BroadPhasePhysicsLayers::NON_MOVING)):
            return "NON_MOVING";
        case (static_cast<JPH::BroadPhaseLayer::Type>(BroadPhasePhysicsLayers::MOVING)):
            return "MOVING";
        default:
            DFW_ASSERT(false);
            return "INVALID";
        }
    }

    bool ObjectVsBroadPhaseLayerFilter::ShouldCollide(JPH::ObjectLayer a_layer_a, JPH::BroadPhaseLayer a_layer_b) const
    {
        switch (a_layer_a)
        {
        case PhysicsLayers::NON_MOVING:
            return a_layer_b == BroadPhasePhysicsLayers::MOVING;
        case PhysicsLayers::MOVING:
            return true;
        default:
            DFW_ASSERT(false);
            return false;
        }
    }

    JPH::ValidateResult	ContactListener::OnContactValidate(
        JPH::Body const& a_body_a, JPH::Body const& a_body_b, JPH::RVec3Arg a_base_offset, JPH::CollideShapeResult const& a_collision_result)
    {
        // TODO: Entity Messaging?
        /*std::cout << "Contact validate callback" << std::endl;*/

        // Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
        return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
    }

    void ContactListener::OnContactAdded(
        JPH::Body const& a_body_a, JPH::Body const& a_body_b, JPH::ContactManifold const& a_manifold, JPH::ContactSettings& a_contact_settings)
    {
        // TODO: Entity Messaging?
        /*std::cout << "A contact was added" << std::endl;*/
    }

    void ContactListener::OnContactPersisted(
        JPH::Body const& a_body_a, JPH::Body const& a_body_b, JPH::ContactManifold const& a_manifold, JPH::ContactSettings& a_contact_settings)
    {
        // TODO: Entity Messaging?
        /*std::cout << "A contact was persisted" << std::endl;*/
    }

    void ContactListener::OnContactRemoved(JPH::SubShapeIDPair const& a_subshape_pair)
    {
        // TODO: Entity Messaging?
        /*std::cout << "A contact was removed" << std::endl;*/
    }

    PhysicsSystemContext::PhysicsSystemContext()
        : _temp_allocator(10 * 1024 * 1024)
    {
        JPH::Factory::sInstance = new JPH::Factory();
    }

    PhysicsSystemContext::~PhysicsSystemContext()
    {
        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }

    void PhysicsSystemContext::Init()
    {
        JPH::RegisterDefaultAllocator();

        // Setup Callbacks.
        JPH::Trace = Detail::TraceLogImplementation;
        JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = Detail::AssertFailedImplementation;);

        JPH::RegisterTypes();

        // TODO: JOB SYSTEM.
        // Might want to look into a job system, for now using Jolt's provided one.
        jph_job_system = MakeUnique<JPH::JobSystemThreadPool>(MAX_NUM_PHYSICS_JOBS, MAX_NUM_PHYSICS_BARRIERS, std::thread::hardware_concurrency() - 1);

        // Now we can create the actual physics system.
        jph_physics_system = MakeUnique<JPH::PhysicsSystem>();
        jph_physics_system->Init(
              MAX_NUM_BODIES
            , NUM_BODY_MUTEXES
            , MAX_NUM_CONTACT_CONSTRAINTS
            , MAX_NUM_CONTACT_CONSTRAINTS
            , _broad_phase_layer_interface
            , _object_vs_broadphase_layer_filter
            , _object_vs_object_layer_filter
        );

        // Add Additional Listeners.
        jph_physics_system->SetContactListener(&_contact_listener);

    }

    void PhysicsSystemContext::Terminate()
    {
        JPH::UnregisterTypes();
    }

    void PhysicsSystemContext::UpdatePhysicsWorld(float32 const a_delta_time, uint8 const a_physics_steps)
    {
        jph_physics_system->Update(a_delta_time, a_physics_steps, &_temp_allocator, jph_job_system.get());
    }

    } // End of namespace ~ DPhysics.

} // End of namespace ~ DFW.
