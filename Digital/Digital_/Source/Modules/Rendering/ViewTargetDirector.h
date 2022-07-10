#pragma once

#include <bgfx/bgfx.h>

#include <Modules/Rendering/ViewTarget.h>

#include <CoreSystems/Memory.h>

#include <vector>

namespace DFW
{
    namespace DRender
    {
        class ViewTargetDirector
        {
        public:
            static constexpr char const* DEFAULT_MAIN_VIEWTARGET_NAME   = "main";
            static constexpr bgfx::ViewId DEFAULT_MAIN_VIEWTARGET_ID    = 0;

            ViewTargetDirector();
            ~ViewTargetDirector();

            void Init();

            SharedPtr<ViewTarget const> AllocateViewTarget(ViewTargetInsertion const a_view_target_insertion);
            SharedPtr<ViewTarget const> AllocateViewTarget(std::string const& a_view_target_name, ViewTargetInsertion const a_view_target_insertion);
            void FreeViewTarget(SharedPtr<ViewTarget> const& a_view_target);
            void FreeViewTarget(bgfx::ViewId a_view_id);

            SharedPtr<ViewTarget const> GetViewTarget(std::string const& a_view_target_name) const;
            SharedPtr<ViewTarget const> GetMainViewTarget() const;

        private:
            bool HasReachedMaxViewTargets() const;

            std::vector<SharedPtr<ViewTarget>>  _view_target_registration;
            std::vector<bgfx::ViewId>           _free_front_view_target_ids;
            std::vector<bgfx::ViewId>           _free_back_view_target_ids;
            bgfx::ViewId                        _view_target_id_front_counter;
            bgfx::ViewId                        _view_target_id_back_counter;
        };

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
