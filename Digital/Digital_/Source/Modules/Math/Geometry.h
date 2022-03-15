#pragma once

#include <glm/glm.hpp>

namespace DFW
{
    namespace DMath
    {
        struct Ray
        {
            glm::vec3 pos;
            glm::vec3 dir;
        };

        struct Line
        {
            glm::vec3 start;
            glm::vec3 end;
        };

        struct Sphere
        {
            glm::vec3 pos;
            float32 radius;
        };

        struct Cube
        {
            glm::vec3 pos;
            float32 extend;
        };
        
        struct Box
        {
            glm::vec3 min;
            glm::vec3 max;
        };

        typedef Box AABB;

        struct OBB
        {
            glm::mat4 matrix;
        };


    } // End of namespace ~ DMath.

} // End of namespace ~ DFW.
