#include <Modules/Rendering/ViewTarget.h>


namespace DFW
{
    namespace DRender
    {
        ViewTarget::ViewTarget()
            : view_target_name("")
            , view_target_id(INVALID_VIEWTARGET_ID)
        {
        }

        ViewTarget::ViewTarget(std::string a_name, uint16 a_id)
            : view_target_name(a_name)
            , view_target_id(a_id)
        {
        }

        ViewTarget::operator bgfx::ViewId() const
        {
            return view_target_id;
        }

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
