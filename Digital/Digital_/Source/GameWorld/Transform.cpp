#include <GameWorld/Transform.h>

namespace DFW
{
    Transform::Transform()
        : cached_world_transform(1.0f)
        , cached_world_translation(0.0f)
        , cached_world_rotation(0.0f, 0.0f, 0.0f)
        , translation(0.0f)
        , rotation(0.0f, 0.0f, 0.0f)
        , scale(1.0f)
        , is_dirty(true)
    {
    }

    Transform::Transform(glm::vec3 const& a_translation)
        : cached_world_transform(1.0f)
        , cached_world_translation(a_translation)
        , cached_world_rotation(0.0f, 0.0f, 0.0f)
        , translation(a_translation)
        , rotation(0.0f, 0.0f, 0.0f)
        , scale(1.0f)
        , is_dirty(true)
    {
    }

    Transform::Transform(glm::vec3 const& a_translation, glm::vec3 const& a_rotation)
        : cached_world_transform(1.0f)
        , cached_world_translation(a_translation)
        , cached_world_rotation(a_rotation)
        , translation(a_translation)
        , rotation(a_rotation)
        , scale(1.0f)
        , is_dirty(true)
    {
    }

    Transform::Transform(glm::vec3 const& a_translation, glm::vec3 const& a_rotation, glm::vec3 const& a_scale)
        : cached_world_transform(1.0f)
        , cached_world_translation(a_translation)
        , cached_world_rotation(a_rotation)
        , translation(a_translation)
        , rotation(a_rotation)
        , scale(a_scale)
        , is_dirty(true)
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

    void Transform::CalculateWorldTransform()
    {
        cached_world_transform = GetTransformMatrix();
        cached_world_translation = translation;
        cached_world_rotation = rotation;
        CleanFlag();
    }

    void Transform::CalculateWorldTransform(glm::mat4 const& a_parent_transform)
    {
        cached_world_transform = a_parent_transform * GetTransformMatrix();
        cached_world_translation = { cached_world_transform[3][0], cached_world_transform[3][1], cached_world_transform[3][2] };
        cached_world_rotation = glm::eulerAngles(glm::quat(cached_world_transform));
        CleanFlag();
    }

} // End of namespace ~ DFW.
