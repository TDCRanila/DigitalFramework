#pragma once

namespace DFW
{
    namespace DRender
    {
        // FW Declare.
        class RenderModuleContext;

        class ViewTargetDirector;
        class RenderTargetDirector;
        class ShaderLibrary;
        class UniformLibrary;
        class TextureLibrary;
        struct ViewTarget;

        class RenderModule
        {
        public:
            RenderModule();
            ~RenderModule();

            void Init();
            void Terminate();

            void BeginFrame();
            void EndFrame();
            void RenderFrame();

        public:
            ViewTargetDirector& GetViewDirector() const;
            RenderTargetDirector& GetRenderTargetDirector() const;
            ShaderLibrary& GetShaderLibrary() const;
            UniformLibrary& GetUniformLibrary() const;
            TextureLibrary& GetTextureLibrary() const;

            void SubmitMesh();
            void SubmitSprite();

            void ChangeRendererSettings();

            void Debug_RendererInfo(/*RenderModuleDebugOptions& const a_debug_option*/);

        private:
            UniquePtr<RenderModuleContext> _render_module_context;

            UniquePtr<ViewTargetDirector> _view_director;
            UniquePtr<RenderTargetDirector> _render_target_director;
            UniquePtr<ShaderLibrary> _shader_library;
            UniquePtr<UniformLibrary> _uniform_library;
            UniquePtr<TextureLibrary> _texture_library;

            SharedPtr<ViewTarget const> _main_view_target;

        };

    } // End of namespace ~ Render.

} // End of namespace ~ DFW.
