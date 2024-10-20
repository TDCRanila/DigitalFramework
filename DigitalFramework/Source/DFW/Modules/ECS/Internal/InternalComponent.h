#pragma once

#include <DFW/Modules/ECS/Internal/InternalEntity.h>

#include <DFW/CoreSystems/DUID.h>

namespace DFW
{
    namespace DECS
    {
        // FW Declare.
        class Entity;

        class InternalComponent
        {
        private:
            friend Entity;

        public:
            InternalComponent() = default;
            ~InternalComponent() = default;

            DFW::DUID GetID() const { return _id; }

        protected:
            DFW::DUID _id;
            InternalEntity _owner;
        };
    }
}
