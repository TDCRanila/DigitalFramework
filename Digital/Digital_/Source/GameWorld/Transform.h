#pragma once

#include <Modules/Math/EulerAngles.h>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace DFW
{
    struct Transform
    {
    public:
        Transform();
        Transform(glm::vec3 const& a_translation);
        Transform(glm::vec3 const& a_translation, glm::vec3 const& a_rotation);
        Transform(glm::vec3 const& a_translation, glm::vec3 const& a_rotation, glm::vec3 const& a_scale);

    public:
        void Translate(glm::vec3 const& a_translation_vector) { translation += a_translation_vector; DirtifyFlag(); }
        void Rotate(DMath::EulerAngles const& a_rotation_vector) { rotation += a_rotation_vector; DirtifyFlag(); }
        void Scale(glm::vec3 const& a_scale_vector) { scale += a_scale_vector; DirtifyFlag(); }

        void SetTranslation(glm::vec3 const& a_translation) { translation = a_translation; DirtifyFlag(); }
        void SetRotation(DMath::EulerAngles const& a_rotation) { rotation = a_rotation; DirtifyFlag(); }
        void SetRotation(glm::quat const& a_rotation_quat) { rotation = glm::eulerAngles(a_rotation_quat); DirtifyFlag(); }
        void SetScale(glm::vec3 const& a_scale) { scale = a_scale; DirtifyFlag(); }

        glm::mat4 const& GetWorldTransformMatrix() const { return cached_world_transform; }
        glm::mat4 GetTransformMatrix() const;

        glm::vec3 const& GetWorldTranslation() const { return cached_world_translation; }
        DMath::EulerAngles const& GetWorldRotation() const { return cached_world_rotation; }

        glm::vec3 const& GetTranslation() const { return translation; }
        DMath::EulerAngles const& GetRotation() const { return rotation; };
        glm::vec3 const& GetScale() const { return scale; }

    public:
        void CalculateWorldTransform();
        void CalculateWorldTransform(glm::mat4 const& a_parent_transform);
        bool IsDirty() const { return is_dirty; }

    private:
        void DirtifyFlag() { is_dirty = true; }
        void CleanFlag() { is_dirty = false; }

        glm::mat4 cached_world_transform;
        glm::vec3 cached_world_translation;
        DMath::EulerAngles cached_world_rotation; // In radians.

        glm::vec3 translation;
        DMath::EulerAngles rotation; // In radians.
        glm::vec3 scale;

        bool is_dirty;

    };

} // End of namespace ~ DFW. 
