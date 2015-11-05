#include <Render/Pipelining/Pipelines/CustomRenderPass/DebugLightBillboards.hh>

#include <memory>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Configuration.hpp>

#include "Graphic\DRBCameraDrawableList.hpp"

#include <Core/Engine.hh>
#include <AssetManagement/AssetManager.hh>

#define DEFERRED_SHADING_DEBUG_LIGHT_VERTEX "../../Datas/Shaders/deferred_shading/deferred_shading_debug_light.vp"
#define DEFERRED_SHADING_DEBUG_LIGHT_FRAG "../../Datas/Shaders/deferred_shading/deferred_shading_debug_light.fp"

#include "Render/Textures/TextureBuffer.hh"
#include "Render/ProgramResources/Types/Uniform/Sampler/SamplerBuffer.hh"
#include "Render/Pipelining/Pipelines/PipelineTools.hh"

namespace AGE
{
	enum Programs
	{
		PROGRAM_BUFFERING_LIGHT = 0,
		PROGRAM_NBR
	};

	DebugLightBillboards::DebugLightBillboards(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
		std::shared_ptr<Texture2D> diffuse,
		std::shared_ptr<Texture2D> depth) :
		FrameBufferRender(screenSize.x, screenSize.y, painterManager)
	{
		push_storage_output(GL_COLOR_ATTACHMENT0, diffuse);
		push_storage_output(GL_DEPTH_STENCIL_ATTACHMENT, depth);

		_programs.resize(PROGRAM_NBR);

		_programs[PROGRAM_BUFFERING_LIGHT] = std::make_shared<Program>(Program(std::string("debug_lights"),
		{
			std::make_shared<UnitProg>(DEFERRED_SHADING_DEBUG_LIGHT_VERTEX, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(DEFERRED_SHADING_DEBUG_LIGHT_FRAG, GL_FRAGMENT_SHADER)
		}));
	}

	void DebugLightBillboards::init()
	{
		Key<Painter> quadPainterKey;
		GetRenderThread()->getQuadGeometry(_quadVertices, quadPainterKey);
		_quadPainter = _painterManager->get_painter(quadPainterKey);

		// We dont want to take the skinned or transparent meshes
		_forbidden[AGE_SKINNED] = true;
		_forbidden[AGE_SEMI_TRANSPARENT] = true;

		_positionBuffer = createRenderPassOutput<TextureBuffer>(_maxInstanciedBillboard, GL_RGBA32F, sizeof(glm::mat4), GL_DYNAMIC_DRAW);

		//@TOTO
		//_programs[PROGRAM_BUFFERING_LIGHT]->get_resource<SamplerBuffer>("model_matrix_tbo")->addInstanciedAlias("model_matrix");
	}

	void DebugLightBillboards::renderPass(const DRBCameraDrawableList &infos)
	{
		{
			SCOPE_profile_gpu_i("DeferredDebugBuffering render pass");
			SCOPE_profile_cpu_i("RenderTimer", "DeferredDebugBuffering render pass");

			OpenGLState::glDisable(GL_CULL_FACE);
			OpenGLState::glDepthMask(GL_TRUE);
			OpenGLState::glDepthFunc(GL_LESS);
			OpenGLState::glDisable(GL_BLEND);
			OpenGLState::glDisable(GL_STENCIL_TEST);
			OpenGLState::glEnable(GL_DEPTH_TEST);
			OpenGLState::glDepthMask(GL_TRUE);
			{
				SCOPE_profile_gpu_i("Overhead Pipeline");
				SCOPE_profile_cpu_i("RenderTimer", "Overhead Pipeline");
				_programs[PROGRAM_BUFFERING_LIGHT]->use();
				_programs[PROGRAM_BUFFERING_LIGHT]->get_resource<Mat4>("projection_matrix").set(infos.cameraInfos.data.projection);
				_programs[PROGRAM_BUFFERING_LIGHT]->get_resource<Mat4>("view_matrix").set(infos.cameraInfos.view);
			}
			//auto &pointLightList = infos.pointLights;
			//if (pointLightList.size() > 0)
			//{
			//	SCOPE_profile_gpu_i("Render debug sprite lights");
			//	SCOPE_profile_cpu_i("RenderTimer", "Render debug sprite pointlights");

			//	auto map = GetEngine()->getInstance<AssetsManager>()->getPointLightTexture();

			//	_programs[PROGRAM_BUFFERING_LIGHT]->get_resource<SamplerBuffer>("model_matrix_tbo").set(_positionBuffer);
			//	_programs[PROGRAM_BUFFERING_LIGHT]->get_resource<Sampler2D>("sprite_light_map").set(map);

			//	_positionBuffer->resetOffset();
			//	_quadPainter->instanciedDrawBegin(_programs[PROGRAM_BUFFERING_LIGHT]);
			//	for (auto &pl : pointLightList)
			//	{
			//		if (_positionBuffer->isFull())
			//		{
			//			_positionBuffer->sendBuffer();
			//			_quadPainter->instanciedDraw(GL_TRIANGLES, _programs[PROGRAM_BUFFERING_LIGHT], _quadVertices, _positionBuffer->getOffset());
			//			_positionBuffer->resetOffset();
			//		}
			//	}
			//	if (_positionBuffer->isEmpty() == false)
			//	{
			//		_positionBuffer->sendBuffer();
			//		_quadPainter->instanciedDraw(GL_TRIANGLES, _programs[PROGRAM_BUFFERING_LIGHT], _quadVertices, _positionBuffer->getOffset());
			//		_positionBuffer->resetOffset();
			//	}
			//	_quadPainter->instanciedDrawEnd();
			//}

			//@CESAR TODO
			//auto &spotLightList = infos.spotLights;
			//if (spotLightList.size() > 0)
			//{
			//	SCOPE_profile_gpu_i("Render debug sprite lights");
			//	SCOPE_profile_cpu_i("RenderTimer", "Render debug sprite spotlights");

			//	auto map = GetEngine()->getInstance<AssetsManager>()->getSpotLightTexture();

			//	_programs[PROGRAM_BUFFERING_LIGHT]->get_resource<SamplerBuffer>("model_matrix_tbo").set(_positionBuffer);
			//	_programs[PROGRAM_BUFFERING_LIGHT]->get_resource<Sampler2D>("sprite_light_map").set(map);

			//	_positionBuffer->resetOffset();
			//	_quadPainter->instanciedDrawBegin(_programs[PROGRAM_BUFFERING_LIGHT]);
			//	for (auto &pl : spotLightList)
			//	{
			//		auto &spotlight = (std::shared_ptr<DRBSpotLightData>&)(pl->spotLight);
			//		_programs[PROGRAM_BUFFERING_LIGHT]->registerProperties(spotlight->globalProperties);
			//		_programs[PROGRAM_BUFFERING_LIGHT]->updateProperties(spotlight->globalProperties);
			//		if (_positionBuffer->isFull())
			//		{
			//			_positionBuffer->sendBuffer();
			//			_quadPainter->instanciedDraw(GL_TRIANGLES, _programs[PROGRAM_BUFFERING_LIGHT], _quadVertices, _positionBuffer->getOffset());
			//			_positionBuffer->resetOffset();
			//		}
			//	}
			//	if (_positionBuffer->isEmpty() == false)
			//	{
			//		_positionBuffer->sendBuffer();
			//		_quadPainter->instanciedDraw(GL_TRIANGLES, _programs[PROGRAM_BUFFERING_LIGHT], _quadVertices, _positionBuffer->getOffset());
			//		_positionBuffer->resetOffset();
			//	}
			//	_quadPainter->instanciedDrawEnd();
			//}
		}
	}
}
