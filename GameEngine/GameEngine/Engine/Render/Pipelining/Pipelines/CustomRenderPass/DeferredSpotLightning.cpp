#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredSpotLightning.hh>

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

#define DEFERRED_SHADING_SPOT_LIGHT_VERTEX "deferred_shading/deferred_shading_spot_light.vp"
#define DEFERRED_SHADING_SPOT_LIGHT_FRAG "deferred_shading/deferred_shading_spot_light.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_LIGHTNING,
		PROGRAM_NBR
	};

	DeferredSpotLightning::DeferredSpotLightning(std::shared_ptr<PaintingManager> painterManager,
		std::shared_ptr<Texture2D> normal,
		std::shared_ptr<Texture2D> depth,
		std::shared_ptr<Texture2D> lightAccumulation) :
		FrameBufferRender(painterManager)
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

		auto vertexShaderPath = shaderPath->getValue() + DEFERRED_SHADING_SPOT_LIGHT_VERTEX;
		auto fragmentShaderPath = shaderPath->getValue() + DEFERRED_SHADING_SPOT_LIGHT_FRAG;

		_programs[PROGRAM_LIGHTNING] = std::make_shared<Program>(Program(std::string("program_point_light"),
		{
			std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
		}));
		GetRenderThread()->getQuadGeometry(_quad, _quadPainter);

	}

	void DeferredSpotLightning::renderPass(RenderPipeline const &, RenderLightList const &lights, CameraInfos const &infos)
	{
		glm::vec3 cameraPosition = -glm::transpose(glm::mat3(infos.view)) * glm::vec3(infos.view[3]);

		_programs[PROGRAM_LIGHTNING]->use();
		*_programs[PROGRAM_LIGHTNING]->get_resource<Mat4>("projection_matrix") = infos.projection;
		*_programs[PROGRAM_LIGHTNING]->get_resource<Mat4>("view_matrix") = infos.view;
		*_programs[PROGRAM_LIGHTNING]->get_resource<Sampler2D>("normal_buffer") = _normalInput;
		*_programs[PROGRAM_LIGHTNING]->get_resource<Sampler2D>("depth_buffer") = _depthInput;
//		*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("eye_pos") = cameraPosition;

		OpenGLState::glDisable(GL_CULL_FACE);
		OpenGLState::glDisable(GL_DEPTH_TEST);
		OpenGLState::glDisable(GL_STENCIL_TEST);
		// And we set the blend mode to additive
		OpenGLState::glEnable(GL_BLEND);
		OpenGLState::glBlendFunc(GL_ONE, GL_ONE);
		// Iterate throught each light
		for (auto &pl : lights.spotLights)
		{
			_programs[PROGRAM_LIGHTNING]->use();
			*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("position_light") = glm::vec3(pl.light.transformation[3]);
			*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("attenuation_light") = pl.light.attenuation;
			*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("direction_light") = pl.light.direction;
			*_programs[PROGRAM_LIGHTNING]->get_resource<Vec1>("spot_cut_off") = pl.light.spotCutOff;
			*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("color_light") = pl.light.color;
			*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("ambient_color") = glm::vec3(0);
			_painterManager->get_painter(_quadPainter)->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_LIGHTNING], Properties(), _quad);
		}
	}
}
