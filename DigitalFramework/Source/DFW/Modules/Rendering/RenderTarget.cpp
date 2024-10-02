#include <Modules/Rendering/RenderTarget.h>

namespace DFW
{
    namespace DRender
    {
        RenderTarget::RenderTarget(bgfx::FrameBufferHandle const a_fbh, std::string const& a_render_target_name)
            : fbh(a_fbh)
            , name(a_render_target_name)
        {
        }

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
