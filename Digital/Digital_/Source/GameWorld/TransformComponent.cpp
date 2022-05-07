#include <GameWorld/TransformComponent.h>

namespace DFW
{
    TransformComponent::TransformComponent()
        : translation(glm::vec3(0.0f))
        , rotation(glm::vec3(0.0f))
        , scale(glm::vec3(1.0f))
    {
    }

    TransformComponent::TransformComponent(glm::vec3 const& a_translation, glm::vec3 const& a_rotation, glm::vec3 const& a_scale)
        : translation(a_translation)
        , rotation(a_rotation)
        , scale(a_scale)
    {
    }

    glm::mat4 TransformComponent::Transform() const
    {
        glm::mat4 t(1.0f);
        t = glm::translate(t, translation);
        t = t * glm::toMat4(glm::quat(rotation));
        t = glm::scale(t, scale);
        return t;
    }

} // End of namespace ~ DFW.
