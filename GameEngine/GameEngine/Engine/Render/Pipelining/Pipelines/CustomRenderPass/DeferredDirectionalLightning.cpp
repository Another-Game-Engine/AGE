#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredDirectionalLightning.hh>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <SpacePartitioning/Ouptut/RenderLight.hh>
#include <SpacePartitioning/Ouptut/RenderPipeline.hh>
#include <SpacePartitioning/Ouptut/RenderPainter.hh>
#include <SpacePartitioning/Ouptut/RenderCamera.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>
#include <Render/ProgramResources/Types/Uniform/Vec3.hh>
#include <Render/ProgramResources/Types/Uniform/Vec1.hh>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>

#define DEFERRED_SHADING_DIRECTIONAL_LIGHT_VERTEX "deferred_shading/deferred_shading_directional_light.vp"
#define DEFERRED_SHADING_DIRECTIONAL_LIGHT_FRAG "deferred_shading/deferred_shading_directional_light.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_LIGHTNING,
		PROGRAM_NBR
	};

	DeferredDirectionalLightning::DeferredDirectionalLightning(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
		std::shared_ptr<Texture2D> normal,
		std::shared_ptr<Texture2D> depth,
		std::shared_ptr<Texture2D> lightAccumulation) :
		FrameBufferRender(screenSize.x, screenSize.y, painterManager)
	{
		push_storage_output(GL_COLOR_ATTACHMENT0, lightAccumulation);
		push_storage_output(GL_DEPTH_STENCIL_ATTACHMENT, depth);

		_normalInput = normal;
		_depthInput = depth;

		_programs.resize(PROGRAM_NBR);

		auto confManager = GetEngine()->getInstance<ConfigurationManager>();

		auto shaderPath = confManager->getConfiguration<std::string>("ShadersPath");

		// you have to set shader directory in configuration path
		AGE_ASSERT(shaderPath != nullptr);

		auto vertexShaderPath = shaderPath->getValue() + DEFERRED_SHADING_DIRECTIONAL_LIGHT_VERTEX;
		auto fragmentShaderPath = shaderPath->getValue() + DEFERRED_SHADING_DIRECTIONAL_LIGHT_FRAG;

		_programs[PROGRAM_LIGHTNING] = std::make_shared<Program>(Program(std::string("program_directional_light"),
		{
			std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
		}));
		GetRenderThread()->getQuadGeometry(_quad, _quadPainter);

	}

	void DeferredDirectionalLightning::renderPass(RenderPipeline const &, RenderLightList const &lights, CameraInfos const &infos)
	{
		glm::vec3 cameraPosition = -glm::transpose(glm::mat3(infos.view)) * glm::vec3(infos.view[3]);

		_programs[PROGRAM_LIGHTNING]->use();
		*_programs[PROGRAM_LIGHTNING]->get_resource<Mat4>("projection_matrix") = infos.projection;
		*_programs[PROGRAM_LIGHTNING]->get_resource<Mat4>("view_matrix") = infos.view;
		*_programs[PROGRAM_LIGHTNING]->get_resource<Sampler2D>("normal_buffer") = _normalInput;
		*_programs[PROGRAM_LIGHTNING]->get_resource<Sampler2D>("depth_buffer") = _depthInput;
		*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("eye_pos") = cameraPosition;

		// clear the light accumulation to zero
		OpenGLState::glClearColor(glm::vec4(0));
		glClear(GL_COLOR_BUFFER_BIT);

		OpenGLState::glDisable(GL_CULL_FACE);
		OpenGLState::glDisable(GL_DEPTH_TEST);
		OpenGLState::glEnable(GL_STENCIL_TEST);
		OpenGLState::glStencilFunc(GL_LESS, 0, 0xFFFFFFFF);
		OpenGLState::glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		// And we set the blend mode to additive

		OpenGLState::glEnable(GL_BLEND);
		OpenGLState::glBlendFunc(GL_ONE, GL_ONE);

		for (auto &pl : lights.directionalLights)
		{
			*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("direction_light") = glm::normalize(glm::transpose(glm::inverse(glm::mat3(pl.light.transformation))) * glm::vec3(0.f, 1.0f, 0.f));
			*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("color_light") = pl.light.data.color;
			_painterManager->get_painter(_quadPainter)->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_LIGHTNING], Properties(), _quad);
		}
	}
}
