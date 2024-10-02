#include <DFW/Modules/Rendering/ViewTarget.h>


namespace DFW
{
    namespace DRender
    {
        ViewTarget::ViewTarget()
            : view_target_name("")
            , view_target_id(INVALID_VIEWTARGET_ID)
            , view_insertion(ViewTargetInsertion::Default)
        {
        }

        ViewTarget::ViewTarget(std::string a_name, uint16 a_id, ViewTargetInsertion view_insertion)
            : view_target_name(a_name)
            , view_target_id(a_id)
            , view_insertion(view_insertion)
        {
        }

        ViewTarget::operator bgfx::ViewId() const
        {
            return view_target_id;
        }

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
