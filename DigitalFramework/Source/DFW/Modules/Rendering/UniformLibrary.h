#pragma once

#include <Modules/Rendering/Uniform.h>

#include <CoreSystems/Logging/Logger.h>

 #include <Utility/TemplateUtility.h>

#include <bgfx/bgfx.h>

#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unordered_map>

namespace DFW
{
    namespace DRender
    {       
        template <typename TypeValue>
        concept IsValidUniformType = requires(TypeValue)
        {
            requires   AreSameTypes<TypeValue, glm::vec4> 
                    || AreSameTypes<TypeValue, glm::mat3> 
                    || AreSameTypes<TypeValue, glm::mat4>;
        };

        class UniformLibrary final
        {
        public:
            UniformLibrary() = default;
            ~UniformLibrary() = default;

            SharedPtr<Uniform> CreateUniform(std::string const& a_uniform_name, UniformTypes const a_uniform_type);
            void DestroyUniform(Uniform const& a_uniform);

            bool IsUniformPresent(Uniform const& a_uniform) const;

            template <typename TypeValue>
            void SetUniformValue(Uniform const& a_uniform, TypeValue const& a_value);

            template <typename TypeValue>
            void TrySetUniformValue(Uniform const& a_uniform, TypeValue const& a_value);

        private:
            std::unordered_map<std::string, SharedPtr<Uniform>> _uniform_map;

        };

#pragma region Template Function Implementation

        template <typename TypeValue>
        void UniformLibrary::SetUniformValue(Uniform const& a_uniform, TypeValue const& a_value)
        {
            if constexpr (not IsValidUniformType<TypeValue>)
            {
                static_assert(always_false<TypeValue>::value, __FUNCTION__ " - Attempting to set a uniform value of type that is not supported.");
                return;
            }
            else
            {
                DFW_ASSERT(IsUniformPresent(a_uniform) && "Trying to set value for uniform, but is not registered.");
                bgfx::setUniform(a_uniform.handle, glm::value_ptr(a_value));
            }
        }

        template <typename TypeValue>
        void UniformLibrary::TrySetUniformValue(Uniform const& a_uniform, TypeValue const& a_value)
        {
            if constexpr (not IsValidUniformType<TypeValue>)
            {
                static_assert(always_false<TypeValue>::value, __FUNCTION__ " - Attempting to set a uniform value of type that is not supported.");
                return;
            }
            else
            {
                if (!IsUniformPresent(a_uniform))
                    DFW_WARNLOG("Trying to set a value for uniform with name: {}, but is not registered.", a_uniform.info.name);
                else
                    bgfx::setUniform(a_uniform.handle, glm::value_ptr(a_value));
            }
        }

#pragma endregion

    } // End of namespace ~ DRender.

} // End of namespace ~ DFW.
