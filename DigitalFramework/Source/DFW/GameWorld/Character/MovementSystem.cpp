#include <DFW/GameWorld/Character/MovementSystem.h>

#include <DFW/Modules/ECS/Managers/EntityRegistry.h>

#include <DFW/GameWorld/Character/CharacterComponent.h>

#include <DFW/CoreSystems/CoreServices.h>
#include <DFW/CoreSystems/GameClock.h>

#include <DFW/Utility/JoltUtility.h>

namespace DFW
{
    void MovementSystem::Update(DECS::EntityRegistry& a_registry)
    {
        DFW::SharedPtr<DFW::GameClock> const& game_clock = DFW::CoreService::GetGameClock();

        for (auto&& [entity, character_comp] : a_registry.ENTT().view<CharacterComponent>().each())
        {
            JPH::Ref<JPH::Character>& character = character_comp.character;

            JPH::Vec3 move_direction = DUtility::GLMToJPH(character_comp.input_move_direction);
            
            // Cancel movement in opposite direction of normal when touching something we can't walk up
            JPH::Character::EGroundState const ground_state = character->GetGroundState();
            if (ground_state == JPH::Character::EGroundState::OnSteepGround || ground_state == JPH::Character::EGroundState::NotSupported)
            {
                JPH::Vec3 normal = character->GetGroundNormal();
                normal.SetY(0.0f);
                float32 const dot_result = normal.Dot(move_direction);
                
                if (dot_result < 0.0f)
                    move_direction -= (dot_result * normal) / normal.LengthSq();
            }

            // Movement
            if (character_comp.can_air_strafe || character->IsSupported())
            {
                // Update velocity
                JPH::Vec3 const current_velocity = character->GetLinearVelocity();
                JPH::Vec3 desired_velocity = move_direction * DUtility::GLMToJPH(character_comp.movement_speed);
                desired_velocity.SetY(current_velocity.GetY());
                JPH::Vec3 new_velocity = 0.75f * current_velocity + 0.25f * desired_velocity;

                // Check Jump
                if (character_comp.should_jump && ground_state == JPH::Character::EGroundState::OnGround)
                {
                    new_velocity += JPH::Vec3(0, character_comp.jump_velocity, 0);
                    character_comp.should_jump = false;
                }

                // Update the velocity
                character->SetLinearVelocity(new_velocity);
            }

            character_comp.input_move_direction = glm::vec3(0.0f);
        }
    }

    void MovementSystem::PostUpdate(DECS::EntityRegistry& a_registry)
    {
        for (auto&& [entity, character_comp] : a_registry.ENTT().view<CharacterComponent>().each())
        {
            JPH::Ref<JPH::Character>& character = character_comp.character;
            character->PostSimulation(character_comp.ground_clearance_tolerance);
        }
    }

} // End of namespace ~ DFW.
