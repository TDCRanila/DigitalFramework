#include <CoreSystems/Components/ModelComponent.h>

#include <Modules/Rendering/ModelData.h>
#include <Modules/Rendering/TextureData.h>
#include <Modules/Rendering/ShaderProgram.h>

#include <CoreSystems/Memory.h>

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

    SharedPtr<DRender::ModelData> Debug_CreateBasicCube()
    {
		static SharedPtr<DRender::ModelData> data;

		if (data != nullptr)
			return data;

		data = MakeShared<DRender::ModelData>();

		// Uniform Layouts
		Detail::PosColorVertex::init();
		data->vertex_layout = Detail::PosColorVertex::ms_layout;

		// Buffers
		DRender::SubModelData& submodel = data->submodels.emplace_back();
		submodel.vbh = bgfx::createVertexBuffer(
			  bgfx::makeRef(Detail::s_cubeVertices, sizeof(Detail::s_cubeVertices))
			, Detail::PosColorVertex::ms_layout
		);

		submodel.ibh = bgfx::createIndexBuffer(
			bgfx::makeRef(Detail::s_cubeTriList, sizeof(Detail::s_cubeTriList))
		);

		return data;
    }

	ModelComponent::ModelComponent()
		: is_visible(true)
	{
	}

} // End of namespace ~ DFW.
