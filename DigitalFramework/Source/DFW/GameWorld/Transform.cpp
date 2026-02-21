#include <DFW/GameWorld/Transform.h>

namespace DFW
{
    Transform::Transform()
        : cached_world_transform(1.0f)
        , translation(0.0f)
        , orientation(glm::vec3(0.0f))
        , scale(1.0f)
        , is_dirty(true)
    {
    }

    Transform::Transform(glm::vec3 const& a_translation)
        : cached_world_transform(1.0f)
        , translation(a_translation)
        , orientation(glm::vec3(0.0f))
        , scale(1.0f)
        , is_dirty(true)
    {
        cached_world_transform = GetTransformMatrix();
    }

    Transform::Transform(glm::vec3 const& a_translation, glm::quat const& a_orientation)
        : cached_world_transform(1.0f)
        , translation(a_translation)
        , orientation(a_orientation)
        , scale(1.0f)
        , is_dirty(true)
    {
        cached_world_transform = GetTransformMatrix();
    }

    Transform::Transform(glm::vec3 const& a_translation, DMath::EulerAngles const& a_rotation)
        : cached_world_transform(1.0f)
        , translation(a_translation)
        , orientation(a_rotation)
        , scale(1.0f)
        , is_dirty(true)
    {
        cached_world_transform = GetTransformMatrix();
    }

    Transform::Transform(glm::vec3 const& a_translation, glm::quat const& a_orientation, glm::vec3 const& a_scale)
        : cached_world_transform(1.0f)
        , translation(a_translation)
        , orientation(a_orientation)
        , scale(a_scale)
        , is_dirty(true)
    {
        cached_world_transform = GetTransformMatrix();
    }

    Transform::Transform(glm::vec3 const& a_translation, DMath::EulerAngles const& a_rotation, glm::vec3 const& a_scale)
        : cached_world_transform(1.0f)
        , translation(a_translation)
        , orientation(a_rotation)
        , scale(a_scale)
        , is_dirty(true)
    {
        cached_world_transform = GetTransformMatrix();
    }

    glm::mat4 Transform::GetTransformMatrix() const
    {
        glm::mat4 t(1.0f);
        t = glm::translate(t, translation);
        t = t * glm::toMat4(orientation);
        t = glm::scale(t, scale);
        return t;
    }

    void Transform::CalculateWorldTransform(glm::mat4 const& a_parent_transform)
    {
        cached_world_transform = a_parent_transform * GetTransformMatrix();
        CleanFlag();
    }

} // End of namespace ~ DFW.
