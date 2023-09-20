#pragma once

#include <GameWorld/Character/CharacterComponent.h>

#include <CoreSystems/Command.h>
#include <CoreSystems/Memory.h>

namespace DFW
{
    class CharacterController
    {
    public:
        void ExecuteActions();
        void EraseActionList();
        void QueueAction(Command* a_command);

    public:
        void PossesCharacter(Entity& a_entity) { _controlled_character = &a_entity.GetComponent<CharacterComponent>(); }
        void ReleaseCharacter() { _controlled_character = nullptr; }

        CharacterComponent& GetCharacter() const { return *_controlled_character; }

    private:
        std::vector<Command*> command_list;
        CharacterComponent* _controlled_character;

    };

    class FPSCharacterController : public CharacterController
    {
    public:
        void HandleDirectionalMoveInput(glm::vec3 const& a_move_direction)
        {
            // TODO: Not great allocating and freeing a command, might look into a better solution.
            QueueAction(new MoveCommand(GetCharacter(), a_move_direction));
        }

        void Jump()
        {
            // TODO: Not great allocating and freeing a command, might look into a better solution.
            QueueAction(new JumpCommand(GetCharacter()));
        }

    private:
        class MoveCommand : public Command
        {
        public:
            MoveCommand(CharacterComponent& a_character, glm::vec3 a_move_direction) : character(a_character), move_direction(a_move_direction) {}

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
