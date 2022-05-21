#include <Modules/Rendering/ViewTargetDirector.h>

#include <CoreSystems/Logging/Logger.h>

#include <Defines/MathDefines.h>

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

            uint32 max_num_views = DMath::GetMinValueOfType<uint32>();

        } // End of namespace ~ Detail.

        ViewTargetDirector::ViewTargetDirector()
            : _view_target_id_front_counter(static_cast<bgfx::ViewId>(-1))
            , _view_target_id_back_counter(static_cast<bgfx::ViewId>(-1))
        {
        }

        ViewTargetDirector::~ViewTargetDirector()
        {
            for (SharedPtr<ViewTarget> const& view_target : _view_target_registration)
                view_target->view_target_id = ViewTarget::INVALID_VIEWTARGET_ID;
        }

        void ViewTargetDirector::Init()
        {
            Detail::max_num_views = bgfx::getCaps()->limits.maxViews;
            _view_target_id_back_counter = Detail::max_num_views;
        }

        SharedPtr<ViewTarget const> ViewTargetDirector::AllocateViewTarget(ViewTargetInsertion const a_view_target_insertion)
        {
            // Find a suitable id number for the nameless ViewTarget.
            uint32 new_view_target_id(0);
            if (a_view_target_insertion == ViewTargetInsertion::Front)
                new_view_target_id = _free_front_view_target_ids.empty() ? _view_target_id_front_counter + 1 : _free_front_view_target_ids.back();
            else if (a_view_target_insertion == ViewTargetInsertion::Back)
                new_view_target_id = _free_back_view_target_ids.empty() ? _view_target_id_back_counter - 1 : _free_back_view_target_ids.back();

            std::string new_view_target_name("viewtarget---#" + std::to_string(new_view_target_id));

            return AllocateViewTarget(new_view_target_name, a_view_target_insertion);
        }

        SharedPtr<ViewTarget const> ViewTargetDirector::AllocateViewTarget(std::string const& a_view_target_name, ViewTargetInsertion const a_view_target_insertion)
        {
            if (a_view_target_name.empty())
                return AllocateViewTarget(a_view_target_insertion);

            if (HasReachedMaxViewTargets())
            {
                DFW_ERRORLOG("Cannot allocate ViewTarget with name: {} - as the the maximum number of ViewTargets has been reached. (MAX:{})"
                    , a_view_target_name, Detail::max_num_views);
                DFW_ASSERT(false && "Cannot allocate ViewTarget as the the maximum number of ViewTargets has been reached.");
                return nullptr;
            }

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
                // Front and Back insertions go inward from the minimum and maximum ViewTarget IDs.
                // Front: 0 -> 1 -> 2 for each Front-Insertion. Back: 255 -> 254 -> 253 for each Back-Insertion.
                // For both insertions, if there is a ViewTarget free ID, that one is used instead.
                bgfx::ViewId new_id;
                if (a_view_target_insertion == ViewTargetInsertion::Front)
                {
                    if (_free_front_view_target_ids.empty())
                    {
                        new_id = ++_view_target_id_front_counter;
                    }
                    else
                    {
                        new_id = _free_front_view_target_ids.back();
                        _free_front_view_target_ids.pop_back();
                    }
                }
                else if (a_view_target_insertion == ViewTargetInsertion::Back)
                {
                    if (_free_back_view_target_ids.empty())
                    {
                        new_id = --_view_target_id_back_counter;
                    }
                    else
                    {
                        new_id = _free_back_view_target_ids.back();
                        _free_back_view_target_ids.pop_back();
                    }
                }

                DFW_INFOLOG("Allocating new ViewTarget with name: {} and ID: {}.", a_view_target_name, new_id);

                Detail::SetViewTargetName(new_id, a_view_target_name.c_str());
                return _view_target_registration.emplace_back(MakeShared<ViewTarget>(a_view_target_name, new_id, a_view_target_insertion));
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

            if (auto const& it = std::find(_view_target_registration.begin(), _view_target_registration.end(), a_view_target)
                ; it != _view_target_registration.end())
            {
                // Return the ViewTargets id back to the director.
                if (a_view_target->view_insertion == ViewTargetInsertion::Front)
                    _free_front_view_target_ids.emplace_back(a_view_target->view_target_id);
                else if (a_view_target->view_insertion == ViewTargetInsertion::Back)
                    _free_back_view_target_ids.emplace_back(a_view_target->view_target_id);

                DFW_INFOLOG("Freeing ViewTarget with name: {} and ID: {}.", a_view_target->view_target_name, a_view_target->view_target_id);

                Detail::ClearViewTarget(a_view_target);
                _view_target_registration.erase(it);
            }
            else
            {
                DFW_WARNLOG("Cannot free ViewTarget with - name:{}, id: {}", a_view_target->view_target_name, a_view_target->view_target_id);
            }
        }

        void ViewTargetDirector::FreeViewTarget(bgfx::ViewId a_view_id)
        {
            auto const id_compare = [&a_view_id](SharedPtr<ViewTarget> const& a_other)
            { 
                return a_view_id == a_other->view_target_id; 
            };

            if (auto const& it = std::find_if(_view_target_registration.begin(), _view_target_registration.end(), id_compare); 
                it != _view_target_registration.end())
            {
                FreeViewTarget(*it);
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

        bool ViewTargetDirector::HasReachedMaxViewTargets() const
        {
            return _view_target_registration.size() >= Detail::max_num_views;
        }

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
