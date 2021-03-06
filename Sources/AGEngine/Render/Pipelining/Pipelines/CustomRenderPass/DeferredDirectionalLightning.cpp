#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredDirectionalLightning.hh>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>
#include <Render/ProgramResources/Types/Uniform/Vec3.hh>
#include <Render/ProgramResources/Types/Uniform/Vec1.hh>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>
#include "Utils/Profiler.hpp"
#include <Utils/StringID.hpp>


#include "Graphic/DRBCameraDrawableList.hpp"

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

		std::string vertexShaderPath = shaderPath->getValue() + DEFERRED_SHADING_DIRECTIONAL_LIGHT_VERTEX;
		std::string fragmentShaderPath = shaderPath->getValue() + DEFERRED_SHADING_DIRECTIONAL_LIGHT_FRAG;

		_programs[PROGRAM_LIGHTNING] = std::make_shared<Program>(Program(StringID("program_directional_light", 0x7e61ad386d9fad6f),
		{
			std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
		}));
		GetRenderThread()->getQuadGeometry(_quad, _quadPainter);

	}

	void DeferredDirectionalLightning::renderPass(const DRBCameraDrawableList &infos)
	{
		SCOPE_profile_gpu_i("DeferredDirectionalLightning render pass");
		SCOPE_profile_cpu_i("RenderTimer", "DeferredDirectionalLightning render pass");

		//auto &meshList = (std::list<std::shared_ptr<DRBMeshData>>&)(infos.meshs);

		glm::vec3 cameraPosition = -glm::transpose(glm::mat3(infos.cameraInfos.view)) * glm::vec3(infos.cameraInfos.view[3]);

		_programs[PROGRAM_LIGHTNING]->use();
		_programs[PROGRAM_LIGHTNING]->get_resource<Mat4>     (StringID("projection_matrix", 0x92b1e336c34a1224)).set(infos.cameraInfos.data.projection);
		_programs[PROGRAM_LIGHTNING]->get_resource<Mat4>     (StringID("view_matrix", 0xd15d560e7965726c)).set(infos.cameraInfos.view);
		_programs[PROGRAM_LIGHTNING]->get_resource<Sampler2D>(StringID("normal_buffer", 0x313e2189c71f910d)).set(_normalInput);
		_programs[PROGRAM_LIGHTNING]->get_resource<Sampler2D>(StringID("depth_buffer", 0x2a88a65798cfc925)).set(_depthInput);
		_programs[PROGRAM_LIGHTNING]->get_resource<Sampler2D>(StringID("specular_buffer", 0x0824313afd644f03)).set(_specularInput);
		_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>     (StringID("eye_pos", 0xe58566afddb7bc1f)).set(cameraPosition);

		{
			SCOPE_profile_gpu_i("clear buffer");
			SCOPE_profile_cpu_i("RenderTimer", "clear buffer");
			// clear the light accumulation to zero
			OpenGLState::glClearColor(glm::vec4(0));
			glClear(GL_COLOR_BUFFER_BIT);

			OpenGLState::glDisable(GL_CULL_FACE);
			OpenGLState::glDisable(GL_DEPTH_TEST);
			OpenGLState::glEnable(GL_STENCIL_TEST);
			OpenGLState::glStencilFunc(GL_EQUAL, 0, 0xFF);
			OpenGLState::glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			// And we set the blend mode to additive

			OpenGLState::glEnable(GL_BLEND);
			OpenGLState::glBlendFunc(GL_ONE, GL_ONE);
		}
		{
			SCOPE_profile_gpu_i("Directional light");
			SCOPE_profile_cpu_i("RenderTimer", "Directional light");

			//@PROUT TODO -> PASS DIRECTIONNAL LIGHT INFOS
			//for (auto &pl : lights.directionalLights)
			//{
			//	SCOPE_profile_gpu_i("One Directional light");
			//	SCOPE_profile_cpu_i("RenderTimer", "One Directional light");
			//	{
			//		SCOPE_profile_gpu_i("Overhead pipeline");
			//		SCOPE_profile_cpu_i("RenderTimer", "One Directional light");
			//		_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("direction_light").set(glm::normalize(glm::transpose(glm::inverse(glm::mat3(pl.light.transformation))) * glm::vec3(0.f, 1.0f, 0.f)));
			//		_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("color_light").set(pl.light.data.color);
			//	}
			//	_painterManager->get_painter(_quadPainter)->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_LIGHTNING], _quad);
			//}
		}
	}
}
