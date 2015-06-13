
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredBasicBuffering.hh>

#include <memory>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Culling/Ouptut/RenderLight.hh>
#include <Culling/Ouptut/RenderPipeline.hh>
#include <Culling/Ouptut/RenderPainter.hh>
#include <Culling/Ouptut/RenderCamera.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>
#include <Configuration.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>
#include <Render/OcclusionTools/DepthMapHandle.hpp>
#include <Render/OcclusionTools/DepthMap.hpp>

#include "Graphic/DRBMesh.hpp"
#include "Graphic/DRBMeshData.hpp"

#include "AssetManagement\Instance\MeshInstance.hh"

#include <Graphic/DRBCameraDrawableList.hpp>

#define DEFERRED_SHADING_BUFFERING_VERTEX "deferred_shading/deferred_shading_get_buffer.vp"
#define DEFERRED_SHADING_BUFFERING_FRAG "deferred_shading/deferred_shading_get_buffer.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_BUFFERING = 0,
		PROGRAM_NBR
	};

	DeferredBasicBuffering::DeferredBasicBuffering(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
												std::shared_ptr<Texture2D> diffuse,
												std::shared_ptr<Texture2D> normal,
												std::shared_ptr<Texture2D> specular,
												std::shared_ptr<Texture2D> depth) :
		FrameBufferRender(screenSize.x, screenSize.y, painterManager)
		, _depth(depth)
	{
		AGE_ASSERT(depth != nullptr);


		// We dont want to take the skinned or transparent meshes
		_forbidden[AGE_SKINNED] = true;
		_forbidden[AGE_SEMI_TRANSPARENT] = true;

		push_storage_output(GL_COLOR_ATTACHMENT0, diffuse);
		push_storage_output(GL_COLOR_ATTACHMENT1, normal);
		push_storage_output(GL_COLOR_ATTACHMENT2, specular);
		push_storage_output(GL_DEPTH_STENCIL_ATTACHMENT, depth);

		_programs.resize(PROGRAM_NBR);

		auto confManager = GetEngine()->getInstance<ConfigurationManager>();

		auto shaderPath = confManager->getConfiguration<std::string>("ShadersPath");

		// you have to set shader directory in configuration path
		AGE_ASSERT(shaderPath != nullptr);

		auto vertexShaderPath = shaderPath->getValue() + DEFERRED_SHADING_BUFFERING_VERTEX;
		auto fragmentShaderPath = shaderPath->getValue() + DEFERRED_SHADING_BUFFERING_FRAG;

		_programs[PROGRAM_BUFFERING] = std::make_shared<Program>(Program(std::string("program_buffering"),
		{ 
			std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
		}));
	}

	void DeferredBasicBuffering::renderPass(const DRBCameraDrawableList &infos)
	{
		SCOPE_profile_gpu_i("DeferredBasicBuffering render pass");
		SCOPE_profile_cpu_i("RenderTimer", "DeferredBasicBuffering render pass");
		{
			SCOPE_profile_gpu_i("Clear buffer");
			SCOPE_profile_cpu_i("RenderTimer", "Clear buffer");

			OpenGLState::glEnable(GL_CULL_FACE);
			OpenGLState::glCullFace(GL_BACK);
			OpenGLState::glDepthMask(GL_TRUE);
			OpenGLState::glDepthFunc(GL_LEQUAL);
			OpenGLState::glDisable(GL_BLEND);
			OpenGLState::glEnable(GL_DEPTH_TEST);
			OpenGLState::glClearColor(glm::vec4(0.f, 0.0f, 0.0f, 0.0f));
			OpenGLState::glEnable(GL_STENCIL_TEST);
			OpenGLState::glClearStencil(0);
			OpenGLState::glStencilFunc(GL_ALWAYS, 0, 0xFFFFFFFF);
			OpenGLState::glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		}
//#ifdef OCCLUSION_CULLING
//
//		{
//			SCOPE_profile_gpu_i("Occluders pass");
//			SCOPE_profile_cpu_i("RenderTimer", "Occluders pass");
//
//			_programs[PROGRAM_BUFFERING]->use();
//			_programs[PROGRAM_BUFFERING]->get_resource<Mat4>("projection_matrix").set(infos.data.projection);
//			_programs[PROGRAM_BUFFERING]->get_resource<Mat4>("view_matrix").set(infos.view);
//
//			for (auto &meshPaint : pipeline.keys)
//			{
//				auto painter = _painterManager->get_painter(Key<Painter>::createKey(meshPaint.first));
//				for (auto &mode : meshPaint.second.drawables)
//				{
//					if (mode.renderMode.at(AGE_OCCLUDER) == true)
//					{
//						painter->draw(GL_TRIANGLES, _programs[PROGRAM_BUFFERING], mode.properties, mode.vertices);
//					}
//				}
//			}
//		}
//		{
//			SCOPE_profile_gpu_i("Copy occlusion depth to CPU");
//			SCOPE_profile_cpu_i("RenderTimer", "Copy occlusion depth to CPU");
//
//			auto writableBuffer = GetRenderThread()->getDepthMapManager().getWritableMap();
//			auto mipmapLevel = GetRenderThread()->getDepthMapManager().getMipmapLevel();
//
//			if (writableBuffer.isValid())
//			{
//				writableBuffer.setMV(infos.data.projection * infos.view);
//				glActiveTextureARB(GL_TEXTURE0_ARB);
//				_depth->bind();
//				glGenerateMipmap(GL_TEXTURE_2D);
//				_depth->get(static_cast<GLint>(mipmapLevel), GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, writableBuffer.getWritableBuffer());
//				_depth->unbind();
//			}
//		}
//
//		{
//			SCOPE_profile_gpu_i("Draw occluded objects");
//			SCOPE_profile_cpu_i("RenderTimer", "Draw occluded objects");
//
//			for (auto &meshPaint : pipeline.keys)
//			{
//				auto painter = _painterManager->get_painter(Key<Painter>::createKey(meshPaint.first));
//				for (auto &mode : meshPaint.second.drawables)
//				{
//					if (mode.renderMode.at(AGE_OCCLUDER) == false)
//					{
//						painter->draw(GL_TRIANGLES, _programs[PROGRAM_BUFFERING], mode.properties, mode.vertices);
//					}
//				}
//			}
//		}
//#else
		{
			SCOPE_profile_gpu_i("Draw all objects");
			SCOPE_profile_cpu_i("RenderTimer", "Draw all objects");

			_programs[PROGRAM_BUFFERING]->use();
			_programs[PROGRAM_BUFFERING]->get_resource<Mat4>("projection_matrix").set(infos.cameraInfos. data.projection);
			_programs[PROGRAM_BUFFERING]->get_resource<Mat4>("view_matrix").set(infos.cameraInfos.view);

			auto &meshList = (std::list<std::shared_ptr<DRBMeshData>>&)(infos.meshs);
			for (auto &meshPaint : meshList)
			{
				auto painter = _painterManager->get_painter(meshPaint->getPainterKey());
				painter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_BUFFERING], meshPaint->globalProperties, meshPaint->getVerticesKey());

			}
		}
//#endif
	}

}
