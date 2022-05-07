#include <GameWorld/Graphics/SpriteSystem.h>

#include <GameWorld/TransformComponent.h>
#include <GameWorld/Camera/CameraComponent.h>
#include <GameWorld/Camera/CameraSystem.h>
#include <GameWorld/Graphics/SpriteComponent.h>

#include <Modules/ECS/Managers/ECSystemManager.h>
#include <Modules/ECS/Objects/ECSUniverse.h>
#include <Modules/Rendering/RenderModule.h>

#include <CoreSystems/Events/EventDispatcher.h>
#include <CoreSystems/Window/WindowManagement.h>

#include <bgfx/bgfx.h>
#include <bx/math.h>

#include <glm/gtc/type_ptr.hpp>

#include <unordered_set>

namespace Detail
{
	struct SpriteQuadLayout
	{
		float32 x, y, z;
		float32 u, v;
		uint32 vertex_colour;

		static void init()
		{
			vertex_layout
				.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
				.end();
		};

		static bgfx::VertexLayout vertex_layout;
	};

	bgfx::VertexLayout SpriteQuadLayout::vertex_layout;

	constexpr SpriteQuadLayout quad_vertices[] =
	{
		{-0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 0xfffffff },
		{ 0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0xfffffff },
		{ 0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0xfffffff },
		{-0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0xfffffff },
	};

	constexpr uint16 quad_indices[] =
	{
		0, 1, 2, // 0
		0, 3, 1,
	};

	bgfx::VertexBufferHandle quad_vbh;
	bgfx::IndexBufferHandle quad_ibh;
	bgfx::TextureHandle basic_texture_handle;

	struct SpriteInstanceDataLayout
	{
		static size_t GetLayoutStride()
		{
			static constexpr size_t layout_stride(
				sizeof(transform)
				+ sizeof(colour)
				+ sizeof(texture_index)
				+ sizeof(padding)
			);

			return  layout_stride;
		}

		// Layout must be packed in multiples of 16Bytes.
		glm::mat4 transform; // 64
		glm::vec4 colour; // 16
		float32 texture_index; // 4
		uint8 padding[12]; // 12
	};

	uint32 max_texture_samplers_per_batch;

} // End of namespace ~ Detail.

namespace DFW
{
	struct SpriteBatch
	{
		SpriteBatch()
			: num_sprites_to_draw(0)
		{
			unique_texture_handles.reserve(Detail::max_texture_samplers_per_batch);
		}

		~SpriteBatch() = default;

		void Reset()
		{
			unique_texture_handles.clear();
			data_buffer = bgfx::InstanceDataBuffer();
			num_sprites_to_draw = 0;
		}

		std::unordered_set<uint16> unique_texture_handles;
		bgfx::InstanceDataBuffer data_buffer;
		uint32 num_sprites_to_draw;
	};

	SpriteSystem::SpriteSystem()
		: _rendering_camera(nullptr)
		, window_width(DWindow::DFW_DEFAULT_WINDOW_WIDTH)
		, window_height(DWindow::DFW_DEFAULT_WINDOW_HEIGHT)
	{
	}

	void SpriteSystem::Init()
	{
		Detail::max_texture_samplers_per_batch = bgfx::getCaps()->limits.maxTextureSamplers;
		
		// Setup Sprite ~ Quad
		Detail::SpriteQuadLayout::init();
		
		Detail::quad_vbh = bgfx::createVertexBuffer(
			bgfx::makeRef(Detail::quad_vertices, sizeof(Detail::quad_vertices))
			, Detail::SpriteQuadLayout::vertex_layout
		);

		Detail::quad_ibh = bgfx::createIndexBuffer(
			bgfx::makeRef(Detail::quad_indices, sizeof(Detail::quad_indices))
		);
		
		// Basic Sprite ~ Texture.
		std::array<uint8, 3> pixel_texture = { 255, 255, 255 };
		bgfx::TextureFormat::Enum texture_format = bgfx::TextureFormat::RGB8;
		bgfx::Memory const* data = bgfx::copy(pixel_texture.data(), static_cast<uint32>(pixel_texture.size() * sizeof(uint8)));
		Detail::basic_texture_handle = bgfx::createTexture2D(1, 1, false, 1, texture_format, 0, data);

		DRender::RenderModule* render_module(CoreService::GetRenderModule());
		// View Target
		_view_target = render_module->view_director.AllocateViewTarget("spritesystem");

		// Shaders
		_program_ptr = render_module->shader_library.ConstructProgram("vs_sprites", "fs_sprites");

		// Uniforms
		_texture_sampler_uniform = render_module->uniform_library.CreateUniform("s_texture", DRender::UniformTypes::Sampler);

		// Register Callbacks.
		CoreService::GetMainEventHandler()->RegisterCallback<WindowResizeEvent, &SpriteSystem::OnWindowResizeEvent>(this);
		ECSEventHandler().RegisterCallback<CameraNewActiveEvent, &SpriteSystem::OnCameraNewActiveEvent>(this);
	}

	void SpriteSystem::Terminate()
	{
		// Unload Shaders.
		CoreService::GetRenderModule()->shader_library.DestroyProgram(_program_ptr);

		// Unregister Callbacks.
		CoreService::GetMainEventHandler()->UnregisterCallback<WindowResizeEvent, &SpriteSystem::OnWindowResizeEvent>(this);
		ECSEventHandler().UnregisterCallback<CameraNewActiveEvent, &SpriteSystem::OnCameraNewActiveEvent>(this);
	}

	void SpriteSystem::Update(DECS::Universe& a_universe)
	{
		// Clear Target.
		DRender::ViewTarget const& view_target = *_view_target;
		bgfx::touch(view_target);

		uint32 const num_sprites(static_cast<uint32>(a_universe.registry.view<SpriteComponent>().size()));
		if (num_sprites <= 0)
			return;

		// Camera Setup.
		if (_rendering_camera)
		{
			bgfx::setViewTransform(view_target, glm::value_ptr(_rendering_camera->view), glm::value_ptr(_rendering_camera->projection));
			bgfx::setViewRect(view_target, 0, 0, static_cast<uint16>(window_width), static_cast<uint16>(window_height));
		}
		else
		{
			// No camera present, create a "camera" at world origin.
			float32 view[16];
			bx::Vec3 const at = { 0.0f, 0.0f, 1.0f };
			bx::Vec3 const eye = { 0.0f, 0.0f, 0.0f };
			bx::mtxLookAt(view, eye, at);

			float32 proj[16];
			bx::mtxProj(proj, DFW_DEFAULT_CAMERA_FOV, float32(window_width) / float32(window_height), 0.1f, 1000.0f, bgfx::getCaps()->homogeneousDepth);

			bgfx::setViewTransform(view_target, view, proj);
			bgfx::setViewRect(view_target, 0, 0, static_cast<uint16>(window_width), static_cast<uint16>(window_height));
		}

		// TODO Possibly sort SpriteComponents based on texture handle. 
		// low to high to pack more sprites together in a sprite batch.
		// Should reduce amount of batches which outherwise would be limited by texture handles.
		// a_universe.registry.sort<SpriteComponent>();
		
		// Prepare Instance Buffers.
		size_t const instance_stride(Detail::SpriteInstanceDataLayout::GetLayoutStride());
		uint32 const max_num_drawable_sprites(bgfx::getAvailInstanceDataBuffer(num_sprites, static_cast<uint16>(instance_stride)));

		bgfx::InstanceDataBuffer idb;
		bgfx::allocInstanceDataBuffer(&idb, max_num_drawable_sprites, static_cast<uint16>(instance_stride));

		// Start Sprite Batching Process.
		SpriteBatch batch;
		batch.data_buffer = idb;
		uint8_t* data_ptr_shifter = idb.data;

		auto NewSpriteBatch = [&batch, &idb, &data_ptr_shifter]() -> void
		{
			batch.Reset();
			batch.data_buffer = idb;

			// Return to start of instance data buffer.
			// Next batch will reuse and overwrite data.
			data_ptr_shifter = idb.data;
		};

		for (auto&& [entity, sprite, transform] : a_universe.registry.group<SpriteComponent>(entt::get<TransformComponent>).each())
		{
			if (!sprite.is_visible)
				continue;

			// Debug - Should be removed eventually.
			if (!sprite.texture)
			{
				sprite.texture = MakeShared<DRender::TextureData>();
				sprite.texture->handle = Detail::basic_texture_handle;
			}

			// Check for unique Texture.
			if (auto is_unique_it = batch.unique_texture_handles.find(sprite.texture->handle.idx); 
				is_unique_it == batch.unique_texture_handles.end())
			{
				// Prevent Texture Overflow.
				if (batch.unique_texture_handles.size() > Detail::max_texture_samplers_per_batch)
				{
					FlushSpriteBatch(batch, view_target);
					NewSpriteBatch();
				}
				
				// Add Unique Texture.
				batch.unique_texture_handles.emplace(sprite.texture->handle.idx);
			}

			// Prevent Buffer Overflow.
			if (batch.num_sprites_to_draw >= max_num_drawable_sprites)
			{
				FlushSpriteBatch(batch, view_target);
				NewSpriteBatch();
			}

			// Fill Buffer Data.
			{
				// 64 Bytes - Matrix
				glm::mat4* mtx = (glm::mat4*)data_ptr_shifter;
				mtx[0] = transform.Transform();

				// 16 Bytes - Sprite Colour.
				glm::vec4* color = (glm::vec4*)&data_ptr_shifter[64];
				color[0] = sprite.colour;

				// 4 Bytes - Texture Index.
				float32* texture_index = (float32*)&data_ptr_shifter[80];
				uint8 const current_texture_index(static_cast<uint8>(batch.unique_texture_handles.size() - 1));
				texture_index[0] = static_cast<float32>(current_texture_index);

				++batch.num_sprites_to_draw;
			}

			// Move to next instance data in the buffer..
			data_ptr_shifter += instance_stride;
		}

		// Flush Last Batch.
		FlushSpriteBatch(batch, view_target);

	}

	void SpriteSystem::FlushSpriteBatch(SpriteBatch const& a_sprite_batch, DRender::ViewTarget const& a_view_target)
	{
		DFW_ASSERT(a_sprite_batch.num_sprites_to_draw > 0 && "Should not be flushing the batch when no sprites are there to be drawn.");

		bgfx::setVertexBuffer(0, Detail::quad_vbh);
		bgfx::setIndexBuffer(Detail::quad_ibh);
		bgfx::setInstanceDataBuffer(&a_sprite_batch.data_buffer, 0, a_sprite_batch.num_sprites_to_draw);

		for (uint8 texture_index_counter(0); uint16 texture_handle : a_sprite_batch.unique_texture_handles)
		{
			bgfx::setTexture(texture_index_counter, _texture_sampler_uniform->handle, bgfx::TextureHandle(texture_handle));
			++texture_index_counter;
		}

		uint64_t constexpr state = 0
			| BGFX_STATE_WRITE_R
			| BGFX_STATE_WRITE_G
			| BGFX_STATE_WRITE_B
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_MSAA
			;

		bgfx::setState(state);

		bgfx::submit(a_view_target, _program_ptr->shader_program_handle);
	}

	void SpriteSystem::OnWindowResizeEvent(WindowResizeEvent const& a_window_event)
	{
		window_width = a_window_event.new_width;
		window_height = a_window_event.new_height;
	}

	void SpriteSystem::OnCameraNewActiveEvent(CameraNewActiveEvent const& a_camera_event)
	{
		_rendering_camera = SystemManager().GetSystem<CameraSystem>()->GetCamera(a_camera_event.camera_identifier);
	}

} // End of namespace ~ DFW.
