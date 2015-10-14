#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredSSAO.hh>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>
#include <Render/ProgramResources/Types/Uniform/Vec2.hh>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>

// TODO TO REMOVE
#include <glm/gtc/matrix_transform.hpp>

#include "Graphic/DRBCameraDrawableList.hpp"
#include "Graphic/DRBPointLightData.hpp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_NBR = 0
	};

	DeferredSSAO::DeferredSSAO(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager) :
		FrameBufferRender(screenSize.x, screenSize.y, painterManager),
		_is_initialized(false),
		_kernels(nullptr)
	{
		_programs.resize(PROGRAM_NBR);
		auto confManager = GetEngine()->getInstance<ConfigurationManager>();
		auto shaderPath = confManager->getConfiguration<std::string>("ShadersPath");
		// you have to set shader directory in configuration path
		AGE_ASSERT(shaderPath != nullptr);
		//auto vertexShaderPath = shaderPath->getValue() + PROGRAM_BLOOM_MERGE_VERTEX;
		//auto fragmentShaderPath = shaderPath->getValue() + PROGRAM_BLOOM_MERGE_FRAG;

		//_programs[PROGRAM_BLOOM_MERGE] = std::make_shared<Program>(Program(std::string("bloom_merge"),
		//{
		//	std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
		//	std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
		//}));
		// set the key painter
		Key<Painter> quadPainterKey;
		GetRenderThread()->getQuadGeometry(_quadVertices, quadPainterKey);
		_quadPainter = _painterManager->get_painter(quadPainterKey);
	}

	DeferredSSAO::~DeferredSSAO()
	{
		//delete[] _kernels;
	}

	void DeferredSSAO::renderPass(const DRBCameraDrawableList &infos)
	{
		std::cout << "SSAO presents" << std::endl;
		//if (!_is_initialized) 
		//{
		//	kernel = new glm::vec3[KERNEL_SIZE];
		//	for (int i = 0; i < KERNEL_SIZE; ++i)
		//	{
		//		kernel[]
		//	}
		//}
	}
}
