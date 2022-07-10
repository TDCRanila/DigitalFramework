#include <GameWorld/Transform.h>

namespace DFW
{
    Transform::Transform()
        : translation(0.0f)
        , rotation(0.0f)
        , scale(1.0f)
    {
    }

    Transform::Transform(glm::vec3 const& a_translation)
        : translation(a_translation)
        , rotation(0.0f)
        , scale(1.0f)
    {
    }

    Transform::Transform(glm::vec3 const& a_translation, glm::vec3 const& a_rotation, glm::vec3 const& a_scale)
        : translation(a_translation)
        , rotation(a_rotation)
        , scale(a_scale)
    {
    }

    glm::mat4 Transform::GetTransformMatrix() const
    {
        glm::mat4 t(1.0f);
        t = glm::translate(t, translation);
        t = t * glm::toMat4(glm::quat(rotation));
        t = glm::scale(t, scale);
        return t;
    }

} // End of namespace ~ DFW.
