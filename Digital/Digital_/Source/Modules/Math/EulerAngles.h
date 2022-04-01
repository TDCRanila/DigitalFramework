#pragma once

#include <glm/vec3.hpp>

namespace DFW
{
    namespace DMath
    {
        struct EulerAngles : public glm::vec<3, float32, glm::qualifier::defaultp>
        {
            EulerAngles();
            EulerAngles(glm::vec3 const& a_vec);
            EulerAngles(float32 a_pitch, float32 a_yaw, float32 a_roll);

            inline float32& pitch() { return this->x; };
            inline float32& yaw() { return this->y; };
            inline float32& roll() { return this->z; };

            inline float32 const& pitch() const { return this->x; };
            inline float32 const& yaw() const { return this->y; };
            inline float32 const& roll() const { return this->z; };
        };

    } // End of namespace ~ DMath.

} // End of namespace ~ DFW.
