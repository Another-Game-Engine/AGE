#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredSkyBox.hh>

#include <memory>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Culling/Output/RenderPipeline.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Vec3.hh>
#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler3D.hh>
#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>
#include <Configuration.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Render/OcclusionTools/DepthMapHandle.hpp>
#include <Render/OcclusionTools/DepthMap.hpp>

#include "Graphic/DRBCameraDrawableList.hpp"

#define DEFERRED_SHADING_BUFFERING_VERTEX "deferred_shading/deferred_shading_skybox.vp"
#define DEFERRED_SHADING_BUFFERING_FRAG "deferred_shading/deferred_shading_skybox.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_SKYBOX = 0,
		PROGRAM_NBR
	};

	DeferredSkyBox::DeferredSkyBox(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
		std::shared_ptr<Texture2D> diffuse,
		std::shared_ptr<Texture2D> depth,
		std::shared_ptr<Texture2D> lightAccumulation) :
		FrameBufferRender(screenSize.x, screenSize.y, painterManager)
	{
		// We dont want to take the skinned or transparent meshes
		_forbidden[AGE_SKINNED] = true;
		_forbidden[AGE_SEMI_TRANSPARENT] = true;

		push_storage_output(GL_DEPTH_STENCIL_ATTACHMENT, depth);
		push_storage_output(GL_COLOR_ATTACHMENT0, diffuse);
		push_storage_output(GL_COLOR_ATTACHMENT1, lightAccumulation);

		_programs.resize(PROGRAM_NBR);

		auto confManager = GetEngine()->getInstance<ConfigurationManager>();
		auto shaderPath = confManager->getConfiguration<std::string>("ShadersPath");
		// you have to set shader directory in configuration path
		AGE_ASSERT(shaderPath != nullptr);
		auto vertexShaderPath = shaderPath->getValue() + DEFERRED_SHADING_BUFFERING_VERTEX;
		auto fragmentShaderPath = shaderPath->getValue() + DEFERRED_SHADING_BUFFERING_FRAG;
		_programs[PROGRAM_SKYBOX] = std::make_shared<Program>(Program(std::string("program_skybox"),
		{
			std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
		}));
		GetRenderThread()->getCube(_cube, _painterCube);
	}

	void DeferredSkyBox::setSkyboxLighting(glm::vec3 lighting)
	{
		_lighting = lighting;
	}

	void DeferredSkyBox::renderPass(const DRBCameraDrawableList &infos)
	{
//@PROUT TODO
		SCOPE_profile_gpu_i("DeferredSkybox render pass");
		SCOPE_profile_cpu_i("RenderTimer", "DeferredSkybox render pass");
		OpenGLState::glDisable(GL_BLEND);
		OpenGLState::glDisable(GL_DEPTH_TEST);
		OpenGLState::glDisable(GL_CULL_FACE);
		OpenGLState::glEnable(GL_STENCIL_TEST);
		OpenGLState::glStencilFunc(GL_LEQUAL, 1, 0xFF);
		OpenGLState::glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		OpenGLState::glDepthMask(GL_FALSE);
		{
			SCOPE_profile_gpu_i("Overhead Pipeline");
			SCOPE_profile_cpu_i("RenderTimer", "Skybox buffer");
			_programs[PROGRAM_SKYBOX]->use();

			// @PROUT
			// TODO : Pass this infos as properties !
			_programs[PROGRAM_SKYBOX]->get_resource<Mat4>("projection").set(infos.cameraInfos.data.projection);
			_programs[PROGRAM_SKYBOX]->get_resource<Mat4>("view").set(infos.cameraInfos.view);
			_programs[PROGRAM_SKYBOX]->get_resource<Sampler3D>("skybox").set(infos.cameraInfos.data.texture);
			_programs[PROGRAM_SKYBOX]->get_resource<Vec3>("lighting").set(_lighting);
		}
		
		auto painter = _painterManager->get_painter(_painterCube);
		painter->uniqueDrawBegin(_programs[PROGRAM_SKYBOX]);
		painter->uniqueDraw(GL_QUADS, _programs[PROGRAM_SKYBOX], Properties(), _cube);
		painter->uniqueDrawEnd();
	}

}
