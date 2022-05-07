#pragma once

#include <glm/vec4.hpp>

namespace DFW
{
    struct Renderable
    {
        Renderable();

        bool is_visible;
        glm::vec4 colour;
    };

} // End of namespace ~ DFW.
