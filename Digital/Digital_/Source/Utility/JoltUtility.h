#pragma once

#include <Jolt/Jolt.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>

namespace DFW
{
    namespace DUtility
    {
        inline JPH::Vec3 GLMToJPH(glm::vec3 const& a_vector) { return { a_vector.x, a_vector.y, a_vector.z }; }
        inline JPH::Vec4 GLMToJPH(glm::vec4 const& a_vector) { return { a_vector.x, a_vector.y, a_vector.z, a_vector.w }; }
        inline JPH::Quat GLMToJPH(glm::quat const& a_quat) { return {a_quat.x, a_quat.y, a_quat.z, a_quat.w }; }

        inline glm::vec3 JPHToGLM(JPH::Vec3 const& a_vector) { return { a_vector.GetX(), a_vector.GetY(), a_vector.GetZ() }; }
        inline glm::vec4 JPHToGLM(JPH::Vec4 const& a_vector) { return { a_vector.GetX(), a_vector.GetY(), a_vector.GetZ(), a_vector.GetW() }; }
        inline glm::quat JPHToGLM(JPH::Quat const& a_quat) { return { a_quat.GetX(), a_quat.GetY(), a_quat.GetZ(), a_quat.GetW() }; }

    } // End of namespace ~ DUtility.

} // End of namespace ~ DFW.
