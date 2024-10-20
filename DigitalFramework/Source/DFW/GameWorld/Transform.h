#pragma once

#include <DFW/Modules/Math/EulerAngles.h>

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
        Transform(glm::vec3 const& a_translation, glm::quat const& a_orientation);
        Transform(glm::vec3 const& a_translation, DMath::EulerAngles const& a_rotation);
        Transform(glm::vec3 const& a_translation, glm::quat const& a_orientation, glm::vec3 const& a_scale);
        Transform(glm::vec3 const& a_translation, DMath::EulerAngles const& a_rotation, glm::vec3 const& a_scale);

    public:
        void Translate(glm::vec3 const& a_translation_vector) { translation += a_translation_vector; DirtifyFlag(); }
        void Rotate(glm::quat const& a_orientation) { orientation *= a_orientation; DirtifyFlag(); }
        void RotateEuler(DMath::EulerAngles const& a_rotation_euler) { orientation *= glm::quat(a_rotation_euler); DirtifyFlag(); }
        void Scale(glm::vec3 const& a_scale_vector) { scale += a_scale_vector; DirtifyFlag(); }

        void SetTranslation(glm::vec3 const& a_translation) { translation = a_translation; DirtifyFlag(); }
        void SetOrientation(glm::quat const& a_orientation) { orientation = a_orientation; DirtifyFlag(); }
        void SetRotation(DMath::EulerAngles const& a_rotation) { orientation = glm::quat(a_rotation); DirtifyFlag(); }
        void SetScale(glm::vec3 const& a_scale) { scale = a_scale; DirtifyFlag(); }

        glm::mat4 const& GetWorldTransformMatrix() const { return cached_world_transform; }
        glm::mat4 GetTransformMatrix() const;

        glm::vec3 const& GetWorldTranslation() const { return cached_world_translation; }
        glm::quat const& GetWorldOrientation() const { return cached_world_orientation; }
        DMath::EulerAngles const GetWorldRotation() const { return glm::eulerAngles(cached_world_orientation); }

        glm::vec3 const& GetTranslation() const { return translation; }
        glm::quat const& GetOrientation() const { return orientation; }
        DMath::EulerAngles const GetRotation() const { return glm::eulerAngles(orientation); };
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
        glm::quat cached_world_orientation;

        glm::vec3 translation;
        glm::quat orientation;
        glm::vec3 scale;

        bool is_dirty;

    };

} // End of namespace ~ DFW. 
