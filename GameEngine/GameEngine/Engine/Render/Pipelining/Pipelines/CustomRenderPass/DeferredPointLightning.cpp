#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredPointLightning.hh>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Culling/Ouptut/RenderLight.hh>
#include <Culling/Ouptut/RenderPipeline.hh>
#include <Culling/Ouptut/RenderPainter.hh>
#include <Culling/Ouptut/RenderCamera.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>
#include <Render/ProgramResources/Types/Uniform/Vec3.hh>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>

// TODO TO REMOVE
#include <glm/gtc/matrix_transform.hpp>

#include "Graphic/DRBCameraDrawableList.hpp"
#include "Graphic/DRBPointLightData.hpp"

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

	DeferredPointLightning::DeferredPointLightning(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
												std::shared_ptr<Texture2D> normal,
												std::shared_ptr<Texture2D> depth,
												std::shared_ptr<Texture2D> specular,
												std::shared_ptr<Texture2D> lightAccumulation,
												std::shared_ptr<Texture2D> shinyAccumulation) :
										FrameBufferRender(screenSize.x, screenSize.y, painterManager)
	{
		push_storage_output(GL_COLOR_ATTACHMENT0, lightAccumulation);
		push_storage_output(GL_COLOR_ATTACHMENT1, shinyAccumulation);
		push_storage_output(GL_DEPTH_STENCIL_ATTACHMENT, depth);

		_normalInput = normal;
		_depthInput = depth;
		_specularInput = specular;

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

	void DeferredPointLightning::renderPass(const DRBCameraDrawableList &infos)
	{
		SCOPE_profile_gpu_i("DeferredPointLightning");
		SCOPE_profile_cpu_i("RenderTimer", "DeferredPointLightning");

		glm::vec3 cameraPosition = -glm::transpose(glm::mat3(infos.cameraInfos.view)) * glm::vec3(infos.cameraInfos.view[3]);

		{
			SCOPE_profile_gpu_i("Overhead pipeline");
			SCOPE_profile_cpu_i("RenderTimer", "Overhead pipeline");
			_programs[PROGRAM_LIGHTNING]->use();
			_programs[PROGRAM_LIGHTNING]->get_resource<Mat4>("projection_matrix").set(infos.cameraInfos.data.projection);
			_programs[PROGRAM_LIGHTNING]->get_resource<Mat4>("view_matrix").set(infos.cameraInfos.view);
			_programs[PROGRAM_LIGHTNING]->get_resource<Sampler2D>("normal_buffer").set(_normalInput);
			_programs[PROGRAM_LIGHTNING]->get_resource<Sampler2D>("depth_buffer").set(_depthInput);
			_programs[PROGRAM_LIGHTNING]->get_resource<Sampler2D>("specular_buffer").set(_specularInput);
			_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("eye_pos").set(cameraPosition);

			_programs[PROGRAM_STENCIL]->use();
			_programs[PROGRAM_STENCIL]->get_resource<Mat4>("projection_matrix").set(infos.cameraInfos.data.projection);
			_programs[PROGRAM_STENCIL]->get_resource<Mat4>("view_matrix").set(infos.cameraInfos.view);
		}


		// Disable blending to clear the color buffer
		OpenGLState::glDisable(GL_BLEND);
		OpenGLState::glEnable(GL_CULL_FACE);
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

		auto &pointList = (std::list<std::shared_ptr<DRBPointLightData>>&)(infos.pointLights);

		for (auto &pl : pointList)
		{
			SCOPE_profile_gpu_i("Lightpoints");
			SCOPE_profile_cpu_i("RenderTimer", "Lightpoints");


			//@PROUT TODO -> add correct properties to PointLightData

			_programs[PROGRAM_STENCIL]->use();
			pl->globalProperties.update_properties(_programs[PROGRAM_STENCIL]);

			_programs[PROGRAM_LIGHTNING]->use();
			pl->globalProperties.update_properties(_programs[PROGRAM_LIGHTNING]);

			// We clear the stencil buffer
			glClear(GL_STENCIL_BUFFER_BIT);

			OpenGLState::glColorMask(glm::bvec4(false));

			OpenGLState::glStencilFunc(GL_ALWAYS, 0, 0xFFFFFFFF);
			OpenGLState::glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			OpenGLState::glCullFace(GL_BACK);

			_spherePainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_STENCIL], pl->globalProperties, _sphereVertices);

			OpenGLState::glColorMask(glm::bvec4(true));

			OpenGLState::glStencilFunc(GL_EQUAL, 0, 0xFFFFFFFF);
			OpenGLState::glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			OpenGLState::glCullFace(GL_FRONT);

			_spherePainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_LIGHTNING], pl->globalProperties, _sphereVertices);

		}
	}
}
