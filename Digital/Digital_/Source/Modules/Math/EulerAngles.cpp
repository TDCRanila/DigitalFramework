#include <Modules/Math/EulerAngles.h>

namespace DFW
{
    namespace DMath
    {
        EulerAngles::EulerAngles()
            : vec(0.0f)
        {
        }

        EulerAngles::EulerAngles(glm::vec3 const& a_vec)
            : vec(a_vec)
        {
        }

        EulerAngles::EulerAngles(float32 a_pitch, float32 a_yaw, float32 a_roll)
            : vec(a_pitch, a_yaw, a_roll)
        {
        }

    } // End of namespace ~ DMath.

} // End of namespace ~ DFW.
