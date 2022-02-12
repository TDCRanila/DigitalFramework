#include <Modules/Rendering/ViewTargetDirector.h>

#include <CoreSystems/Logging/Logger.h>

#include <algorithm>

namespace DFW
{
    namespace DRender
    {
        namespace Detail
        {
            void ClearViewTarget(SharedPtr<ViewTarget> a_view_target)
            {
                bgfx::resetView(a_view_target->view_target_id);
                a_view_target->view_target_id = ViewTarget::INVALID_VIEWTARGET_ID;
            }

            void SetViewTargetName(bgfx::ViewId a_view_id, char const* a_view_name)
            {
                bgfx::setViewName(a_view_id, a_view_name);
            }

            bool IsViewTargetIDValid(bgfx::ViewId a_view_id)
            {
                return a_view_id == ViewTarget::INVALID_VIEWTARGET_ID;
            }

        } // End of namespace ~ Detail.

        ViewTargetDirector::ViewTargetDirector()
            : _view_target_id_counter(static_cast<bgfx::ViewId>(-1))
        {
        }

        ViewTargetDirector::~ViewTargetDirector()
        {
            for (SharedPtr<ViewTarget> const& view_target : _view_target_registration)
                view_target->view_target_id = ViewTarget::INVALID_VIEWTARGET_ID;
        }

        SharedPtr<ViewTarget const> ViewTargetDirector::AllocateViewTarget()
        {
            uint16 new_view_target_name_id(_free_view_target_ids.empty() ? (_view_target_id_counter + 1) : _free_view_target_ids.back());
            std::string new_view_target_name("viewport---#" + std::to_string(new_view_target_name_id));

            return AllocateViewTarget(new_view_target_name);
        }

        SharedPtr<ViewTarget const> ViewTargetDirector::AllocateViewTarget(std::string const& a_view_target_name)
        {
            if (a_view_target_name.empty())
                return AllocateViewTarget();

            // Check if a new ViewTarget can be allocated.
            auto const name_compare = [&a_view_target_name](SharedPtr<ViewTarget> const& a_other) 
            { 
                return a_view_target_name == a_other->view_target_name; 
            };

            if (auto const it = std::find_if(_view_target_registration.begin(), _view_target_registration.end(), name_compare)
                ; it != _view_target_registration.end())
            {
                DFW_WARNLOG("Cannot allocate ViewTarget with name: {} - as it already exists.", a_view_target_name);
                return nullptr;
            }
            else
            {
                bgfx::ViewId new_id;
                if (_free_view_target_ids.empty())
                {
                    new_id = ++_view_target_id_counter;
                }
                else
                {
                    new_id = _free_view_target_ids.back();
                    _free_view_target_ids.pop_back();
                }

                Detail::SetViewTargetName(new_id, a_view_target_name.c_str());
                return _view_target_registration.emplace_back(MakeShared<ViewTarget>(a_view_target_name, new_id));
            }
        }

        void ViewTargetDirector::FreeViewTarget(SharedPtr<ViewTarget> const& a_view_target)
        {
            if (!a_view_target)
            {
                DFW_WARNLOG("Cannot free ViewTarget that is invalid.");
                return;
            }

            if (Detail::IsViewTargetIDValid(a_view_target->view_target_id))
            {
                DFW_WARNLOG("Cannot free ViewTarget with an invalid id.");
                return;
            }

            if (a_view_target->view_target_name == DEFAULT_MAIN_VIEWTARGET_NAME)
            {
                DFW_WARNLOG("Prevented an attempt to free the main ViewTarget.");
                return;
            }

            if (auto const it = std::find(_view_target_registration.begin(), _view_target_registration.end(), a_view_target)
                ; it != _view_target_registration.end())
            {
                Detail::ClearViewTarget(a_view_target);
                _view_target_registration.erase(it);
                _free_view_target_ids.emplace_back(a_view_target->view_target_id);
            }
            else
            {
                DFW_WARNLOG("Cannot free ViewTarget with - name:{}, id: {}", a_view_target->view_target_name, a_view_target->view_target_id);
            }
        }

        void ViewTargetDirector::FreeViewTarget(bgfx::ViewId a_view_id)
        {
            if (Detail::IsViewTargetIDValid(a_view_id))
            {
                DFW_WARNLOG("Cannot free ViewTarget with an invalid id.");
                return;
            }

            if (a_view_id == DEFAULT_MAIN_VIEWTARGET_ID)
            {
                DFW_WARNLOG("Prevented an attempt to free the main ViewTarget.");
                return;
            }

            auto const id_compare = [&a_view_id](SharedPtr<ViewTarget> const& a_other)
            { 
                return a_view_id == a_other->view_target_id; 
            };

            if (auto const it = std::find_if(_view_target_registration.begin(), _view_target_registration.end(), id_compare); 
                it != _view_target_registration.end())
            {
                Detail::ClearViewTarget(*it);
                _view_target_registration.erase(it);
                _free_view_target_ids.emplace_back(a_view_id);
            }
            else
            {
                DFW_WARNLOG("Cannot free ViewTarget with id: {}", a_view_id);
            }
        }

        SharedPtr<ViewTarget const> ViewTargetDirector::GetViewTarget(std::string const& a_view_target_name) const
        {
            auto const name_compare = [&a_view_target_name](SharedPtr<ViewTarget> const& a_other)
            { 
                return a_view_target_name == a_other->view_target_name; 
            };

            if (auto const it = std::find_if(_view_target_registration.begin(), _view_target_registration.end(), name_compare)
                ; it != _view_target_registration.end())
                return (*it);
            else
                return nullptr;
        }

        SharedPtr<ViewTarget const> ViewTargetDirector::GetMainViewTarget() const
        {
            if (_view_target_registration.empty())
            {
                DFW_ASSERT(false, "Cannot retrieve main ViewTarget.");
                return nullptr;
            }
            else
            {
                return _view_target_registration.front();
            }
        }

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
