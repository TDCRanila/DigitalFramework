#pragma once

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace DFW
{
    struct Transform
    {
        Transform();
        Transform(glm::vec3 const& a_translation);
        Transform(glm::vec3 const& a_translation, glm::vec3 const& a_rotation);
        Transform(glm::vec3 const& a_translation, glm::vec3 const& a_rotation, glm::vec3 const& a_scale);

        glm::mat4 GetTransformMatrix() const;

        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scale;

    };

} // End of namespace ~ DFW. 
