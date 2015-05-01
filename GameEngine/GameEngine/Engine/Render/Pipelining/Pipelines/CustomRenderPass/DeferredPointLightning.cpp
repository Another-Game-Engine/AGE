#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredPointLightning.hh>

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
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>

#define DEFERRED_SHADING_STENCIL_VERTEX "deferred_shading/basic_3d.vp"
#define DEFERRED_SHADING_STENCIL_FRAG "deferred_shading/basic_3d.fp"

#define DEFERRED_SHADING_POINT_LIGHT_VERTEX "deferred_shading/deferred_shading_point_light.vp"
#define DEFERRED_SHADING_POINT_LIGHT_FRAG "deferred_shading/deferred_shading_point_light.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_STENCIL = 0,
		PROGRAM_LIGHTNING,
		PROGRAM_NBR
	};

	DeferredPointLightning::DeferredPointLightning(std::shared_ptr<PaintingManager> painterManager,
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

		auto vertexShaderPath = shaderPath->getValue() + DEFERRED_SHADING_STENCIL_VERTEX;
		auto fragmentShaderPath = shaderPath->getValue() + DEFERRED_SHADING_STENCIL_FRAG;

		_programs[PROGRAM_STENCIL] = std::make_shared<Program>(Program(std::string("basic_3d_render"),
		{
			std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
		}));

		vertexShaderPath = shaderPath->getValue() + DEFERRED_SHADING_POINT_LIGHT_VERTEX;
		fragmentShaderPath = shaderPath->getValue() + DEFERRED_SHADING_POINT_LIGHT_FRAG;

		_programs[PROGRAM_LIGHTNING] = std::make_shared<Program>(Program(std::string("program_point_light"),
		{
			std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
		}));

		// We get the sphere geometry
		Key<Painter> spherePainterkey;
		GetRenderThread()->getIcoSphereGeometry(_sphereVertices, spherePainterkey, 2);
		_spherePainter = _painterManager->get_painter(spherePainterkey);
	}

	void DeferredPointLightning::renderPass(RenderPipeline const &, RenderLightList const &lights, CameraInfos const &infos)
	{
		SCOPE_profile_gpu_i("DeferredPointLightning render pass");
		SCOPE_profile_cpu_i("RenderTimer", "DeferredPointLightning render pass");

		glm::vec3 cameraPosition = -glm::transpose(glm::mat3(infos.view)) * glm::vec3(infos.view[3]);

		_programs[PROGRAM_LIGHTNING]->use();
		*_programs[PROGRAM_LIGHTNING]->get_resource<Mat4>("projection_matrix") = infos.projection;
		*_programs[PROGRAM_LIGHTNING]->get_resource<Mat4>("view_matrix") = infos.view;
		*_programs[PROGRAM_LIGHTNING]->get_resource<Sampler2D>("normal_buffer") = _normalInput;
		*_programs[PROGRAM_LIGHTNING]->get_resource<Sampler2D>("depth_buffer") = _depthInput;
		*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("eye_pos") = cameraPosition;

		_programs[PROGRAM_STENCIL]->use();
		*_programs[PROGRAM_STENCIL]->get_resource<Mat4>("projection_matrix") = infos.projection;
		*_programs[PROGRAM_STENCIL]->get_resource<Mat4>("view_matrix") = infos.view;
		// Disable blending to clear the color buffer
		OpenGLState::glDisable(GL_BLEND);
		OpenGLState::glEnable(GL_CULL_FACE);
		// clear the light accumulation to zero
		OpenGLState::glClearColor(glm::vec4(0));
		glClear(GL_COLOR_BUFFER_BIT);
		// activate depth test and func to check if sphere_depth > current_depth (normal zfail)
		OpenGLState::glEnable(GL_DEPTH_TEST);
		OpenGLState::glDepthFunc(GL_GEQUAL);
		// We activate the stencil test
		OpenGLState::glEnable(GL_STENCIL_TEST);
		// We do not write on the depth buffer
		OpenGLState::glDepthMask(GL_FALSE);
		// And we set the blend mode to additive
		OpenGLState::glEnable(GL_BLEND);
		OpenGLState::glBlendFunc(GL_ONE, GL_ONE);
		// Set stencil clear value to 0
		OpenGLState::glClearStencil(0);
		// Iterate throught each light
		for (auto &pl : lights.pointLight)
		{
			_programs[PROGRAM_STENCIL]->use();
			*_programs[PROGRAM_STENCIL]->get_resource<Mat4>("model_matrix") = pl.light.sphereTransform;

			_programs[PROGRAM_LIGHTNING]->use();
			*_programs[PROGRAM_LIGHTNING]->get_resource<Mat4>("model_matrix") = pl.light.sphereTransform;
			*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("position_light") = glm::vec3(pl.light.sphereTransform[3]);
			*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("attenuation_light") = pl.light.attenuation;
			*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("color_light") = pl.light.color;
			*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("ambient_color") = glm::vec3(0);

			// We clear the stencil buffer
			glClear(GL_STENCIL_BUFFER_BIT);

			OpenGLState::glColorMask(glm::bvec4(false));

			OpenGLState::glStencilFunc(GL_ALWAYS, 0, 0xFFFFFFFF);
			OpenGLState::glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			OpenGLState::glCullFace(GL_BACK);

			_spherePainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_STENCIL], Properties(), _sphereVertices);

			OpenGLState::glColorMask(glm::bvec4(true));

			OpenGLState::glStencilFunc(GL_EQUAL, 0, 0xFFFFFFFF);
			OpenGLState::glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			OpenGLState::glCullFace(GL_FRONT);

			_spherePainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_LIGHTNING], Properties(), _sphereVertices);
		}
	}
}
