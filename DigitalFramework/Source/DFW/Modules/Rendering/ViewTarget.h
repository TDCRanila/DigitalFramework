#pragma once

#include <bgfx/bgfx.h>

#include <string>

namespace DFW
{
    namespace DRender
    {
        enum class ViewTargetInsertion
        {
              Front = 0
            , Back = 1
            , Default

        };

        struct ViewTarget
        {
            static constexpr bgfx::ViewId INVALID_VIEWTARGET_ID = bgfx::kInvalidHandle;
            
            ViewTarget();
            ViewTarget(std::string a_name, uint16 a_id, ViewTargetInsertion view_insertion);
            
            operator bgfx::ViewId() const;

            std::string view_target_name;
            bgfx::ViewId view_target_id;
            ViewTargetInsertion view_insertion;
        };

    } // End of namespace ~ DFW.

} // End of namespace ~ DRender.
