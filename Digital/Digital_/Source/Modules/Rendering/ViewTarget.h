#pragma once

#include <bgfx/bgfx.h>

#include <string>

namespace DFW
{
    namespace DRender
    {
        struct ViewTarget
        {
            static constexpr bgfx::ViewId INVALID_VIEWTARGET_ID = bgfx::kInvalidHandle;
            
            ViewTarget();
            ViewTarget(std::string a_name, uint16 a_id);
            
            operator bgfx::ViewId() const;

            std::string view_target_name;
            bgfx::ViewId view_target_id;
        };

    } // End of namespace ~ DFW.

} // End of namespace ~ DRender.
