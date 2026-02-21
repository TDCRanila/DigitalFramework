#include <DFW/GameWorld/Controller/CharacterController.h>

namespace DFW
{
    CharacterController::CharacterController()
        : _controlled_character(nullptr)
    {
    }

    void CharacterController::PossesCharacter(Entity& a_entity)
    { 
        _controlled_character = &a_entity.GetComponent<CharacterComponent>(); 
    }
    
    void CharacterController::ReleaseCharacter()
    { 
        _controlled_character = nullptr; 
    }

} // End of namespace ~ DFW.
