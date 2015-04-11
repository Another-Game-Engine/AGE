
#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredBasicBuffering.hh>

#include <memory>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/OpenGLState.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <SpacePartitioning/Ouptut/RenderLight.hh>
#include <SpacePartitioning/Ouptut/RenderPipeline.hh>
#include <SpacePartitioning/Ouptut/RenderPainter.hh>
#include <SpacePartitioning/Ouptut/RenderCamera.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Core/ConfigurationManager.hpp>
#include <Core/Engine.hh>
#include <Configuration.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>

#define DEFERRED_SHADING_BUFFERING_VERTEX "deferred_shading/deferred_shading_get_buffer.vp"
#define DEFERRED_SHADING_BUFFERING_FRAG "deferred_shading/deferred_shading_get_buffer.fp"
#ifdef OCCLUSION_CULLING
#define DEFERRED_SHADING_BUFFERING_OCCLUSION_GEO "deferred_shading/deferred_shading_get_buffer_occlusion.gp"
#define DEFERRED_SHADING_BUFFERING_OCCLUSION_VERTEX "deferred_shading/deferred_shading_get_buffer_occlusion.vp"
#endif

namespace AGE
{
#ifdef OCCLUSION_CULLING
	enum Programs
	{
		PROGRAM_OCCLUDER = 0,
		PROGRAM_BASIC,
		PROGRAM_NBR
	};
#else
	enum Programs
	{
		PROGRAM_BUFFERING = 0,
		PROGRAM_NBR
	};
#endif

	DeferredBasicBuffering::DeferredBasicBuffering(std::shared_ptr<PaintingManager> painterManager,
												std::shared_ptr<Texture2D> diffuse,
												std::shared_ptr<Texture2D> normal,
												std::shared_ptr<Texture2D> specular,
												std::shared_ptr<Texture2D> depth) :
		FrameBufferRender(painterManager)
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

#ifdef OCCLUSION_CULLING
		{
			auto vertexShaderPath = shaderPath->getValue() + DEFERRED_SHADING_BUFFERING_VERTEX;
			auto fragmentShaderPath = shaderPath->getValue() + DEFERRED_SHADING_BUFFERING_FRAG;

			_programs[PROGRAM_BASIC] = std::make_shared<Program>(Program(std::string("program_buffering_occlusion"),
			{
				std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
				std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
			}));
		}
		{
			auto vertexShaderPath = shaderPath->getValue() + DEFERRED_SHADING_BUFFERING_OCCLUSION_VERTEX;
			auto geoShaderPath = shaderPath->getValue() + DEFERRED_SHADING_BUFFERING_OCCLUSION_GEO;
			auto fragmentShaderPath = shaderPath->getValue() + DEFERRED_SHADING_BUFFERING_FRAG;

			_programs[PROGRAM_OCCLUDER] = std::make_shared<Program>(Program(std::string("program_buffering"),
			{
				std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
				std::make_shared<UnitProg>(geoShaderPath, GL_GEOMETRY_SHADER),
				std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
			}));
		}
#else
		auto vertexShaderPath = shaderPath->getValue() + DEFERRED_SHADING_BUFFERING_VERTEX;
		auto fragmentShaderPath = shaderPath->getValue() + DEFERRED_SHADING_BUFFERING_FRAG;

		_programs[PROGRAM_BUFFERING] = std::make_shared<Program>(Program(std::string("program_buffering"),
		{ 
			std::make_shared<UnitProg>(vertexShaderPath, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(fragmentShaderPath, GL_FRAGMENT_SHADER)
		}));
#endif
	}

	void DeferredBasicBuffering::renderPass(RenderPipeline const &pipeline, RenderLightList const &, CameraInfos const &infos)
	{
		OpenGLState::glEnable(GL_CULL_FACE);
		OpenGLState::glCullFace(GL_BACK);
		OpenGLState::glDepthMask(GL_TRUE);
		OpenGLState::glDepthFunc(GL_LEQUAL);
		OpenGLState::glDisable(GL_BLEND);
		OpenGLState::glDisable(GL_STENCIL_TEST);
		OpenGLState::glEnable(GL_DEPTH_TEST);
		OpenGLState::glClearColor(glm::vec4(0.f, 0.0f, 0.0f, 0.0f));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef OCCLUSION_CULLING

		if (_programs[PROGRAM_OCCLUDER]->isCompiled() == false)
		{
			return;
		}

		_programs[PROGRAM_OCCLUDER]->use();
		*_programs[PROGRAM_OCCLUDER]->get_resource<Mat4>("projection_matrix") = infos.projection;
		*_programs[PROGRAM_OCCLUDER]->get_resource<Mat4>("view_matrix") = infos.view;

		for (auto &meshPaint : pipeline.keys)
		{
			auto painter = _painterManager->get_painter(Key<Painter>::createKey(meshPaint.first));
			for (auto &mode : meshPaint.second.drawables)
			{
				if (mode.renderMode.at(AGE_OCCLUDER) == true)
				{
					painter->draw(GL_TRIANGLES, _programs[PROGRAM_OCCLUDER], mode.properties, mode.vertices);
				}
			}
		}

		// mipmap depth
		glActiveTextureARB(GL_TEXTURE0_ARB);
		_depth->bind();
		glGenerateMipmap(GL_TEXTURE_2D);
		_depth->unbind();

		if (_programs[PROGRAM_BASIC]->isCompiled() == false)
		{
			return;
		}

		_programs[PROGRAM_BASIC]->use();
		*_programs[PROGRAM_BASIC]->get_resource<Mat4>("projection_matrix") = infos.projection;
		*_programs[PROGRAM_BASIC]->get_resource<Mat4>("view_matrix") = infos.view;

		for (auto &meshPaint : pipeline.keys)
		{
			auto painter = _painterManager->get_painter(Key<Painter>::createKey(meshPaint.first));
			for (auto &mode : meshPaint.second.drawables)
			{
				if (mode.renderMode.at(AGE_OCCLUDER) == false)
				{
					painter->draw(GL_TRIANGLES, _programs[PROGRAM_BASIC], mode.properties, mode.vertices);
				}
			}
		}
#else
		_programs[PROGRAM_BUFFERING]->use();
		*_programs[PROGRAM_BUFFERING]->get_resource<Mat4>("projection_matrix") = infos.projection;
		*_programs[PROGRAM_BUFFERING]->get_resource<Mat4>("view_matrix") = infos.view;

		for (auto &meshPaint : pipeline.keys)
		{
			auto painter = _painterManager->get_painter(Key<Painter>::createKey(meshPaint.first));
			for (auto &mode : meshPaint.second.drawables)
			{
				if (renderModeCompatible(mode.renderMode))
				{
					painter->draw(GL_TRIANGLES, _programs[PROGRAM_BUFFERING], mode.properties, mode.vertices);
				}
			}
		}
#endif

	}

}
