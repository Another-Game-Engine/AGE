#include <Render/Pipelining/Pipelines/CustomRenderPass/GaussianBlur.hh>

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

#define PROGRAM_BLUR_H_VERTEX "deferred_shading/blurh.vp"
#define PROGRAM_BLUR_V_VERTEX "deferred_shading/blurv.vp"
#define PROGRAM_BLUR_FRAG "deferred_shading/blur.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_BLUR = 0,
		PROGRAM_NBR
	};

	GaussianBlur::GaussianBlur(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
		std::shared_ptr<Texture2D> src, std::shared_ptr<Texture2D> dst, bool horizontal) :
										FrameBufferRender(screenSize.x, screenSize.y, painterManager)
	{
		push_storage_output(GL_COLOR_ATTACHMENT0, dst);

		_inverseSourceSize = 1.0f / glm::vec2(screenSize);
		_source = src;

		_programs.resize(PROGRAM_NBR);

		auto confManager = GetEngine()->getInstance<ConfigurationManager>();

		auto shaderPath = confManager->getConfiguration<std::string>("ShadersPath");

		// you have to set shader directory in configuration path
		AGE_ASSERT(shaderPath != nullptr);

		std::string vertexShaderPath;

		if (horizontal)
			vertexShaderPath = shaderPath->getValue() + PROGRAM_BLUR_H_VERTEX;
		else
			vertexShaderPath = shaderPath->getValue() + PROGRAM_BLUR_V_VERTEX;
		auto fragmentShaderPath = shaderPath->getValue() + PROGRAM_BLUR_FRAG;

		_programs[PROGRAM_BLUR] = std::make_shared<Program>(Program(std::string("gaussian_blur"),
		{
			std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
		}));
		Key<Painter> quadPainterKey;

		GetRenderThread()->getQuadGeometry(_quadVertices, quadPainterKey);
		_quadPainter = _painterManager->get_painter(quadPainterKey);
	}

	void GaussianBlur::renderPass(const DRBCameraDrawableList &infos)
	{
		SCOPE_profile_gpu_i("GaussianBlur");
		SCOPE_profile_cpu_i("RenderTimer", "GaussianBlur");

		OpenGLState::glDepthMask(GL_FALSE);
		OpenGLState::glDisable(GL_DEPTH_TEST);
		OpenGLState::glDisable(GL_STENCIL_TEST);
		OpenGLState::glDisable(GL_CULL_FACE);

		_programs[PROGRAM_BLUR]->use();
		_programs[PROGRAM_BLUR]->get_resource<Sampler2D>("sourceTexture").set(_source);
		_programs[PROGRAM_BLUR]->get_resource<Vec2>("inverseSourceSize").set(_inverseSourceSize);

		_quadPainter->uniqueDrawBegin(_programs[PROGRAM_BLUR]);
		_quadPainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_BLUR], _quadVertices);
		_quadPainter->uniqueDrawEnd();
	}
}
