#include <GameWorld/GameObject.h>

#include <Utility/AutoFactory/AutoFactory.h>

#include <CoreSystems/Logging/Logger.h>

namespace DFW
{
    GameObject::GameObject(Entity const& a_entity)
    {
        this->_id = a_entity.GetID();
        this->_universe = &a_entity.GetUniverse();
        this->_handle = a_entity.GetHandle();
    }

    void GameObject::OnCreate()
    {
    }
    
    void GameObject::OnDestroy()
    {
    }

} // End of namespace DFW.
