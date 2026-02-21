#pragma once

#include <DFW/GameWorld/Controller/CharacterController.h>
#include <DFW/GameWorld/Character/CharacterComponent.h>

#include <DFW/CoreSystems/Command.h>
#include <DFW/CoreSystems/Memory.h>

namespace DFW
{
    class FPSCharacterController : public CharacterController
    {
    public:
        virtual ~FPSCharacterController() = default;

        void HandleDirectionalMoveInput(glm::vec3 const& a_move_direction)
        {
            // TODO: Not great allocating and freeing a command, might look into a better solution.
            QueueAction(DFW::MakeUnique<MoveCommand>(GetCharacter(), a_move_direction));
        }

        void Jump()
        {
            // TODO: Not great allocating and freeing a command, might look into a better solution.
            QueueAction(DFW::MakeUnique<JumpCommand>(GetCharacter()));
        }

    private:
        class MoveCommand : public Command
        {
        public:
            MoveCommand(CharacterComponent& a_character, glm::vec3 const& a_move_direction) : character(a_character), move_direction(a_move_direction) {}

            virtual void Execute() override
            {
                character.get().input_move_direction += move_direction;
            }

            DFW::RefWrap<CharacterComponent> character;
            glm::vec3 move_direction;
        };

        class JumpCommand : public Command
        {
        public:
            JumpCommand(CharacterComponent& a_character) : character(a_character) {}

            virtual void Execute() override { character.get().should_jump = true; }

            DFW::RefWrap<CharacterComponent> character;
        };
    };

} // End of namespace ~ DFW.
