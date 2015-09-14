#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredBloomMerge.hh>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Culling/Output/RenderPipeline.hh>
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

#define PROGRAM_BLOOM_MERGE_VERTEX "deferred_shading/bloom_merge.vp"
#define PROGRAM_BLOOM_MERGE_FRAG "deferred_shading/bloom_merge.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_BLOOM_MERGE = 0,
		PROGRAM_NBR
	};

	DeferredBloomMerge::DeferredBloomMerge(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
		std::shared_ptr<Texture2D> blurred1,
		std::shared_ptr<Texture2D> blurred2,
		std::shared_ptr<Texture2D> clean,
		std::shared_ptr<Texture2D> dst) :
		FrameBufferRender(screenSize.x, screenSize.y, painterManager)
	{
		push_storage_output(GL_COLOR_ATTACHMENT0, dst);

		_clean = clean;
		_blurred1 = blurred1;
		_blurred2 = blurred2;

		_programs.resize(PROGRAM_NBR);

		auto confManager = GetEngine()->getInstance<ConfigurationManager>();

		auto shaderPath = confManager->getConfiguration<std::string>("ShadersPath");

		// you have to set shader directory in configuration path
		AGE_ASSERT(shaderPath != nullptr);

		auto vertexShaderPath = shaderPath->getValue() + PROGRAM_BLOOM_MERGE_VERTEX;
		auto fragmentShaderPath = shaderPath->getValue() + PROGRAM_BLOOM_MERGE_FRAG;

		_programs[PROGRAM_BLOOM_MERGE] = std::make_shared<Program>(Program(std::string("bloom_merge"),
		{
			std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
		}));
		Key<Painter> quadPainterKey;

		GetRenderThread()->getQuadGeometry(_quadVertices, quadPainterKey);
		_quadPainter = _painterManager->get_painter(quadPainterKey);
	}

	void DeferredBloomMerge::renderPass(const DRBCameraDrawableList &infos)
	{
		if (infos.cameraInfos.data.bloom)
		{
			SCOPE_profile_gpu_i("Depth of field");
			SCOPE_profile_cpu_i("RenderTimer", "Depth of field");

			OpenGLState::glDepthMask(GL_FALSE);
			OpenGLState::glDisable(GL_DEPTH_TEST);
			OpenGLState::glDisable(GL_STENCIL_TEST);
			OpenGLState::glDisable(GL_CULL_FACE);

			_programs[PROGRAM_BLOOM_MERGE]->use();
			_programs[PROGRAM_BLOOM_MERGE]->get_resource<Sampler2D>("cleanMap").set(_clean);
			_programs[PROGRAM_BLOOM_MERGE]->get_resource<Sampler2D>("blurredMap1").set(_blurred1);
			_programs[PROGRAM_BLOOM_MERGE]->get_resource<Sampler2D>("blurredMap2").set(_blurred2);

			_quadPainter->uniqueDrawBegin(_programs[PROGRAM_BLOOM_MERGE]);
			_quadPainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_BLOOM_MERGE], Properties(), _quadVertices);
			_quadPainter->uniqueDrawEnd();
		}
	}
}
