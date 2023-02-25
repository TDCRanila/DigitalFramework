#pragma once

#include <Modules/ECS/Objects/ECSEntity.h>

#include <Utility/AutoFactory/AutoFactory.h>

namespace DFW
{
    // FW Declare.
    class GameWorld;

    class GameObject : public DECS::Entity::Registrar<GameObject>
    {
    private:
        friend GameWorld;
    public:
        GameObject() = default;
        virtual ~GameObject() = default;

        virtual void OnCreate();
        virtual void OnDestroy();
    
    private:
        GameObject(Entity const& a_entity);

    };

} // End of namespace DFW.
