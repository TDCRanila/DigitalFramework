#pragma once

#include <DFW/GameWorld/Controller/BaseController.h>
#include <DFW/GameWorld/Character/CharacterComponent.h>


namespace DFW
{
    class CharacterController : public BaseController
    {
    public:
        CharacterController();
        virtual ~CharacterController() = default;

        void PossesCharacter(Entity& a_entity);
        void ReleaseCharacter();

        CharacterComponent& GetCharacter() const { return *_controlled_character; }

    private:
        CharacterComponent* _controlled_character;

    };

} // End of namespace ~ DFW.
