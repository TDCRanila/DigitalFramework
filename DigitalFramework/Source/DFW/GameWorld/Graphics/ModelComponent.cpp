#include <DFW/GameWorld/Graphics/ModelComponent.h>

#include <DFW/Modules/Rendering/MeshData.h>
#include <DFW/Modules/Rendering/TextureData.h>
#include <DFW/Modules/Rendering/ShaderProgram.h>

#include <DFW/CoreSystems/Memory.h>

namespace DFW
{
    namespace Detail
    {
		struct PosColorVertex
		{
			float32 m_x;
			float32 m_y;
			float32 m_z;
			uint32 m_abgr;

			static void init()
			{
				ms_layout
					.begin()
					.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
					.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
					.end();
			};

			static bgfx::VertexLayout ms_layout;
		};

		bgfx::VertexLayout PosColorVertex::ms_layout;

		static PosColorVertex s_cubeVertices[] =
		{
			{-1.0f,  1.0f,  1.0f, 0xff000000 },
			{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
			{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
			{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
			{-1.0f,  1.0f, -1.0f, 0xffff0000 },
			{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
			{-1.0f, -1.0f, -1.0f, 0xffffff00 },
			{ 1.0f, -1.0f, -1.0f, 0xffffffff },
		};

		static const uint16 s_cubeTriList[] =
		{
			0, 1, 2, // 0
			1, 3, 2,
			4, 6, 5, // 2
			5, 6, 7,
			0, 2, 4, // 4
			4, 2, 6,
			1, 5, 3, // 6
			5, 7, 3,
			0, 4, 1, // 8
			4, 5, 1,
			2, 3, 6, // 10
			6, 3, 7,
		};

    } // End of namespace ~ Detail.

	SharedPtr<DRender::MeshData> Debug_CreateBasicCube()
	{
		return Debug_CreateBasicCube(DFW::ColourRGBA(255, 255, 255));
	}
    
	SharedPtr<DRender::MeshData> Debug_CreateBasicCube(DFW::ColourRGBA const& a_cube_colour)
	{
		static SharedPtr<DRender::MeshData> debug_cube_mesh_data;
		static std::unordered_map<DFW::ColourRGBA, SharedPtr<DRender::MeshData>> debug_cubes;

		if (debug_cube_mesh_data == nullptr)
		{
			debug_cube_mesh_data = MakeShared<DRender::MeshData>();

			// Uniform Layouts
			Detail::PosColorVertex::init();

			// Buffers
			DRender::SubMeshData& submodel = debug_cube_mesh_data->submeshes.emplace_back();
			submodel.vertex_layout = Detail::PosColorVertex::ms_layout;
			submodel.vbh = bgfx::createVertexBuffer(
				bgfx::makeRef(Detail::s_cubeVertices, sizeof(Detail::s_cubeVertices))
				, Detail::PosColorVertex::ms_layout
			);

			submodel.ibh = bgfx::createIndexBuffer(
				bgfx::makeRef(Detail::s_cubeTriList, sizeof(Detail::s_cubeTriList))
			);
		}

		SharedPtr<DRender::MeshData>& debug_cube = debug_cubes[a_cube_colour];
		if (!debug_cube)
		{
			debug_cube = MakeShared<DRender::MeshData>(*debug_cube_mesh_data);
			DRender::SubMeshData& submodel = debug_cube->submeshes.at(0);

			// Texture
			{
				int32 width(1), height(1);
				std::array<uint8, 3> pixel_texture = { a_cube_colour.r, a_cube_colour.g, a_cube_colour.b };
				bgfx::TextureFormat::Enum texture_format = bgfx::TextureFormat::RGB8;
				bgfx::Memory const* data = bgfx::copy(pixel_texture.data(), static_cast<uint32>(pixel_texture.size() * sizeof(uint8)));
				bgfx::TextureHandle texture_handle = bgfx::createTexture2D(width, height, false, 1, texture_format, 0, data);
				submodel.textures.emplace_back(DFW::MakeShared<DRender::TextureData>(texture_handle, width, height, BGFX_TEXTURE_NONE, 0));
			}
		}

		return debug_cube;
    }

} // End of namespace ~ DFW.
