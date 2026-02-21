#include <DFW/Modules/Rendering/RenderTargetDirector.h>

#include <DFW/Modules/Rendering/RenderTarget.h>

#include <DFW/CoreSystems/Logging/Logger.h>

namespace DFW
{
    namespace DRender
    {
        SharedPtr<RenderTarget const> RenderTargetDirector::RegisterRenderTarget(bgfx::FrameBufferHandle const& a_fbh, std::string const& a_render_target_name)
        {
            if (!bgfx::isValid(a_fbh))
            {
                DFW_ERRORLOG("Registering a RenderTarget, but the FBH is invalid - RenderTargetName: {}", a_render_target_name);
                return nullptr;
            }

            if (a_render_target_name.empty())
            {
                DFW_ERRORLOG("Registering a RenderTarget, but with no valid name.");
                return nullptr;
            }

            DFW_INFOLOG("Registering RenderTarget with name: {} and FBH-ID: {}", a_render_target_name, a_fbh.idx);
            return _render_target_registration.emplace_back(MakeShared<RenderTarget const>(a_fbh, a_render_target_name));
        }

        void RenderTargetDirector::FreeRenderTarget(RenderTarget const& a_render_target)
        {
            if (!IsRenderTargetValid(a_render_target))
            {
                DFW_WARNLOG("Trying to free RenderTarget - with name: {} - but is invalid.", a_render_target.name);
                return;
            }

            auto const render_target_id_compare = [&a_render_target](SharedPtr<RenderTarget const> const& a_other)
            {
                return a_render_target.fbh.idx == a_other->fbh.idx;
            };

            auto const& it = std::find_if(_render_target_registration.begin(), _render_target_registration.end(), render_target_id_compare);
            if (it == _render_target_registration.end())
                return;

            DFW_INFOLOG("Freeing RenderTarget with name: {} and FBH-ID: {}", a_render_target.name, a_render_target.fbh.idx);

            // Free.
            bgfx::destroy(a_render_target.fbh);
                
            // Erase Registration.
            _render_target_registration.erase(it);
        }

        void RenderTargetDirector::FreeAllRenderTargets()
        {
            for (SharedPtr<RenderTarget const> const& render_target : _render_target_registration)
                FreeRenderTarget(*render_target);
        }

        SharedPtr<RenderTarget const> RenderTargetDirector::GetRenderTarget(std::string const& a_render_target_name) const
        {
            auto const render_target_name_compare = [&a_render_target_name](SharedPtr<RenderTarget const> const& a_other)
            {
                return a_render_target_name == a_other->name;
            };

            auto const& it = std::find_if(_render_target_registration.begin(), _render_target_registration.end(), render_target_name_compare);
            if (it != _render_target_registration.end())
                return *it;
            else
                return nullptr;
        }

        bool RenderTargetDirector::IsRenderTargetValid(RenderTarget const& a_render_target) const
        {
            if (!bgfx::isValid(a_render_target.fbh))
                return false;

            return true;
        }

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.

