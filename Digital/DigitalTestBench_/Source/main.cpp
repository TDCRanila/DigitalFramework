#include <stdio.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>

#include <string>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <CoreSystems/ApplicationInstance.h>
#include <CoreSystems/ImGuiLayer.h>

#define BOOST_UUID_NO_TYPE_TRAITS 
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/container_hash/hash.hpp>

//
////
////#include <Autofactory/AutoFactory.h>
////
////#include <CoreSystems/ApplicationInstance.h>
////#include <CoreSystems/TimeTracker.h>
////
////#include <Modules/ECS/ECSModule.h>
////#include <Modules/ECS/Managers/ECSComponentManager.h>
////#include <Modules/ECS/Managers/ECSEntityManager.h>
////#include <Modules/ECS/Utility/ECSKeyLockSystem.h>
////
////#include <Modules/ECS/Objects/ECSystem.h>
////#include <Modules/ECS/Objects/ECSComponent.h>
////#include <Modules/ECS/Objects/ECSComponentHandle.h>
//
//#include <Defines/Defines.h>
//
////struct X : public DFactory::AutoFactory<DECS::ECSComponent>::Registrar<X>
////{
////	int32 variable;
////};
////
////struct Y : public DFactory::AutoFactory<DECS::ECSComponent>::Registrar<Y>
////{
////	int32 variable;
////};
////
////struct Z : public DFactory::AutoFactory<DECS::ECSComponent>::Registrar<Z>
////{
////	int32 variable;
////};
////
////class SystemOne : public DFactory::AutoFactory<DECS::ECSystem>::Registrar<SystemOne>
////{
////public:
////	SystemOne()
////	{
////		this->system_name_ = "SystemOne";
////	}
////	~SystemOne() {}
////
////	void Update(DECS::ECSEntityManager* /*a_entity_manager*/, float32 /*a_delta_time*/) override
////	{
////		DECS::ECSComponentHandle<X> x;
////		DECS::ECSComponentHandle<Y> y;
////		DECS::ECSComponentHandle<Z> z;
////		//for (auto e : GetECSModule->EntitiesWith(x, y))
////		//{
////		//	// x & y are references.
////		//	// check if valid for safety from ComponentHandle;
////		//}
////	}
////
////};
//

struct PosColorVertex
{
	float x;
	float y;
	float z;
	uint32_t abgr;
};

static PosColorVertex cubeVertices[] =
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

static const uint16_t cubeTriList[] =
{
	0, 1, 2,
	1, 3, 2,
	4, 6, 5,
	5, 6, 7,
	0, 2, 4,
	4, 2, 6,
	1, 5, 3,
	5, 7, 3,
	0, 4, 1,
	4, 5, 1,
	2, 3, 6,
	6, 3, 7,
};
static void glfw_errorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

bgfx::ShaderHandle loadShader(const char* FILENAME)
{
	const char* shaderPath = "";

	switch (bgfx::getRendererType()) {
	case bgfx::RendererType::Noop:
	case bgfx::RendererType::Direct3D9:  shaderPath = "bgfx-shaders/dx9/";   break;
	case bgfx::RendererType::Direct3D11:
	case bgfx::RendererType::Direct3D12: shaderPath = "bgfx-shaders/dx11/";  break;
	case bgfx::RendererType::Gnm:        shaderPath = "bgfx-shaders/pssl/";  break;
	case bgfx::RendererType::Metal:      shaderPath = "bgfx-shaders/metal/"; break;
	case bgfx::RendererType::OpenGL:     shaderPath = "bgfx-shaders/glsl/";  break;
	case bgfx::RendererType::OpenGLES:   shaderPath = "bgfx-shaders/essl/";  break;
	case bgfx::RendererType::Vulkan:     shaderPath = "bgfx-shaders/spirv/"; break;
	}

	size_t shaderLen = strlen(shaderPath);
	size_t fileLen = strlen(FILENAME);
	char* filePath = (char*)calloc(1, shaderLen + fileLen + 1);
	memcpy(filePath, shaderPath, shaderLen);
	memcpy(&filePath[shaderLen], FILENAME, fileLen);

	FILE* file = fopen(filePath, "rb");
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	const bgfx::Memory* mem = bgfx::alloc(fileSize + 1);
	fread(mem->data, 1, fileSize, file);
	mem->data[mem->size - 1] = '\0';
	fclose(file);

	return bgfx::createShader(mem);
}

//DIGIAPP_NAMED_ENTRY(Zynthurism)

class DUID : public boost::uuids::uuid
{
public:
	DUID()
		: boost::uuids::uuid(boost::uuids::nil_generator()())
	{
	}
	
	explicit DUID(boost::uuids::uuid const& u)
		: boost::uuids::uuid(u)
	{}

	operator boost::uuids::uuid() {
		return static_cast<boost::uuids::uuid&>(*this);
	}

	operator boost::uuids::uuid() const {
		return static_cast<boost::uuids::uuid const&>(*this);
	}

	operator std::string() const {
		return boost::uuids::to_string(*this);
	}
};

class DUIDGenerator
{
public:
	static DUID GenerateID()
	{
		DUID id;
		id.swap(boost::uuids::random_generator()());
		return id;
	}
};

namespace std
{
	template<>
	struct hash<DUID>
	{
		std::size_t operator()(const DUID& a_duid) const
		{
			boost::hash<DUID> duid_hasher;
			std::size_t hash_val = duid_hasher(a_duid);
			return hash_val;
		}
	};
}

DUID u1;
DUID u2;

class OhNo
{
	DUID id;
};

int main(int /*argc*/, char** /*argv*/) 
{
	int kappa = 0;
	DUID u2;
	INFOLOG(u2.size());

	OhNo a;
	OhNo b;

	for (int i = 0; i <= 100; ++i)
	{
		DUID u1;
		std::string strA(u1);
		INFOLOG("DUID1: " << u1);
	}


	std::unordered_map<DUID, bool> map;

	DUID wow;
	wow = DUIDGenerator::GenerateID();

	map.emplace(DUID(), true);
	map.emplace(DUID(), true);
	map.emplace(wow, false);
	map.emplace(DUID(), true);

	auto it = map.find(wow);
	auto wow_really = it->first;

	std::string wow_string_really = wow_really;
	
	DUID u3;
	DUID u4;

	bool result1 = u3 == u4;
	bool result2 = u3 == u3;

	kappa++;

	{
		//if (true)
		//{
		//	//int32 width(1280);
		//	//int32 height(720);
		//	//GLFWwindow* window(nullptr);
		//	//
		//	//glfwSetErrorCallback(glfw_errorCallback);
		//	//glfwInit();
		//	//glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		//	//window = glfwCreateWindow(width, height, "Zynthurism", NULL, NULL);

		//	//// Call bgfx::renderFrame before bgfx::init to signal to bgfx not to create a render thread.
		//	////bgfx::renderFrame();

		//	//bgfx::PlatformData platform_data;
		//	//platform_data.nwh = glfwGetWin32Window(window);
		//	//bgfx::setPlatformData(platform_data);

		//	//bgfx::Init bgfx_init;
		//	//bgfx_init.type = bgfx::RendererType::Count; // Auto Choose Renderer
		//	//bgfx_init.resolution.width	= width;
		//	//bgfx_init.resolution.height	= height;
		//	//bgfx_init.resolution.reset	= BGFX_RESET_VSYNC;
		//	//bgfx::init(bgfx_init);

		//	//imguiCreate();

		//	//// Set view 0 to the same dimensions as the window and to clear the color buffer.
		//	////const bgfx::ViewId kClearView = 0;
		//	//bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x22222222, 1.0f, 0);
		//	//bgfx::setViewRect(0, 0, 0, width, height);
		//	////bgfx::setViewRect(kClearView, 0, 0, bgfx::BackbufferRatio::Equal);

		//	//bgfx::VertexLayout pcvDecl;
		//	//pcvDecl.begin()
		//	//	.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		//	//	.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
		//	//	.end();
		//	//bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), pcvDecl);
		//	//bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));

		//	//bgfx::ShaderHandle vsh = loadShader("vs_cubes.bin");
		//	//bgfx::ShaderHandle fsh = loadShader("fs_cubes.bin");
		//	//bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);

		//	//unsigned int counter = 0;
		//	//while (!glfwWindowShouldClose(window)) 
		//	//{
		//	//	glfwPollEvents();

		//	//	imguiBeginFrame(0, 0, 0, 0, width, height);

		//	//	bool* show_demo = new bool(true);
		//	//	ImGui::ShowDemoWindow(show_demo);

		//	//	imguiEndFrame();

		//	//	// Handle window resize.
		//	//	int32 oldWidth = width, oldHeight = height;
		//	//	glfwGetWindowSize(window, &width, &height);
		//	//	if (width != oldWidth || height != oldHeight) 
		//	//	{
		//	//		bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC);
		//	//		bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
		//	//		//bgfx::setViewRect(0, 0, 0, width, height);
		//	//	}

		//	//	// Use debug font to print information about this example.
		//	//	bgfx::dbgTextClear();
		//	//	bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
		//	//	bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
		//	//	bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
		//	//	bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
		//	//	const bgfx::Stats* stats = bgfx::getStats();
		//	//	bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);
		//	//	
		//	//	// Enable stats or debug text.
		//	//	bgfx::setDebug(BGFX_DEBUG_STATS);


		//	//	// Set view and projection matrix for view 0.
		//	//	const bx::Vec3 at = { 0.0f, 0.0f,   0.0f };
		//	//	const bx::Vec3 eye = { 0.0f, 0.0f, -20.0f };
		//	//	
		//	//	float view[16];
		//	//	bx::mtxLookAt(view, eye, at);

		//	//	float proj[16];
		//	//	bx::mtxProj(proj, 60.0f, float(width) / float(height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
		//	//	bgfx::setViewTransform(0, view, proj);

		//	//		// Set view 0 default viewport.
		//	//		//bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));
		//	//	

		//	//	// This dummy draw call is here to make sure that view 0 is cleared if no other draw calls are submitted to view 0.
		//	//	//bgfx::touch(0);

		//	//	// Cube
		//	//	{
		//	//		float mtxA[16]{0};
		//	//		float mtxB[16]{0};
		//	//		float mtxC[16]{0};

		//	//		bx::mtxRotateXY(mtxA, counter * 0.005f, counter * 0.01f);

		//	//		bx::mtxTranslate(mtxB, bx::sin(counter * 0.02f)*2, bx::cos(counter * 0.02f)*2, 0);

		//	//		// Set model matrix for rendering.
		//	//		bx::mtxMul(mtxC,mtxA, mtxB);
		//	//		bgfx::setTransform(mtxC);

		//	//		// Set vertex and index buffer.
		//	//		bgfx::setVertexBuffer(0, vbh);
		//	//		bgfx::setIndexBuffer(ibh);

		//	//		// Set render states.
		//	//		//bgfx::setState(BGFX_STATE_PT_LINES);

		//	//		bgfx::submit(0, program);
		//	//	}

		//	//	// Cube
		//	//	{
		//	//		float mtxA[16]{ 0 };
		//	//		float mtxB[16]{ 0 };
		//	//		float mtxC[16]{ 0 };

		//	//		bx::mtxRotateXY(mtxA, counter * 0.005f, counter * 0.01f);

		//	//		bx::mtxTranslate(mtxB, bx::sin(bx::kPi + counter * 0.02f) * 2, bx::cos(bx::kPi + counter * 0.02f) *2, 0);

		//	//		// Set model matrix for rendering.
		//	//		bx::mtxMul(mtxC, mtxA, mtxB);
		//	//		bgfx::setTransform(mtxC);

		//	//		// Set vertex and index buffer.
		//	//		bgfx::setVertexBuffer(0, vbh);
		//	//		bgfx::setIndexBuffer(ibh);

		//	//		// Set render states.
		//	//		//bgfx::setState(BGFX_STATE_PT_LINES);

		//	//		bgfx::submit(0, program);
		//	//	}
		//	//	
		//	//	// Advance to next frame. Process submitted rendering primitives.
		//	//	bgfx::frame();
		//	//	counter++;
		//	//}
		//}
		//else
		//{
		//	/*INFOLOG("error" << std::to_string(0.5f));

		//	X x;
		//	DECS::ECSIDManager::GetComponentTypeID<X>();
		//	Y y;
		//	Z z;
		//	DECS::ECSModule ecs_module;

		//	ecs_module.InitECS();
		//	ecs_module.UpdateECS();
		//	ecs_module.TerminateECS();

		//	auto e1 = ecs_module.entity_manager_->CreateEntity();
		//	auto e = ecs_module.entity_manager_->CreateEntity();

		//	ecs_module.component_manager_->AddComponent<Z>(e.GetID());
		//	ecs_module.component_manager_->AddComponent<X>(e.GetID());*/

		//	//ecs_module.entity_manager_->GetEntity(0);
		//	//ecs_module.entity_manager_->GetEntity(1);

		//	////ecs_module.entity_manager_->DeleteEntity(0);
		//	////ecs_module.entity_manager_->DeleteEntity(1);

		//	//DECS::ComponentBitList a(0);
		//	//DECS::ComponentBitList b(0);
		//	//a = ecs_module.component_manager_->key_lock_system_->ConstructorBitList<X>();
		//	//b = ecs_module.component_manager_->key_lock_system_->ConstructorBitList<Y, X, Y>();

		//	//ecs_module.component_manager_->AddComponent<Z>(e.GetID());
		//	//ecs_module.component_manager_->AddComponent<Kappa>(e.GetID());
		//	//ecs_module.component_manager_->AddComponent<X>(e.GetID());
		//	////auto& cbl2 = ecs_module.entity_manager_->GetComponentBitList(e.GetID());
		//	////bool atest = ecs_module.component_manager_->HasComponents<X>(e.GetID());
		//	////bool btest = ecs_module.component_manager_->HasComponents<X, Y>(e.GetID());
		//	////bool ctest = ecs_module.component_manager_->HasComponents<X, Z, Y>(e.GetID());
		//	//ecs_module.component_manager_->DeleteComponent<Y>(e.GetID());
		//	//ecs_module.component_manager_->DeleteComponent<X>(e.GetID());
		//	//ecs_module.component_manager_->GetComponent<X>(e.GetID());
		//}
	}
}

