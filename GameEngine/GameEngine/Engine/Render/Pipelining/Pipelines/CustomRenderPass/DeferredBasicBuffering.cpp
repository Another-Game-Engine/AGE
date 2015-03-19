
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

#define DEFERRED_SHADING_BUFFERING_VERTEX "../../Shaders/deferred_shading/deferred_shading_get_buffer.vp"
#define DEFERRED_SHADING_BUFFERING_FRAG "../../Shaders/deferred_shading/deferred_shading_get_buffer.fp"

namespace AGE
{
	enum Programs
	{
		PROGRAM_BUFFERING = 0,
		PROGRAM_NBR
	};

	DeferredBasicBuffering::DeferredBasicBuffering(std::shared_ptr<PaintingManager> painterManager,
												std::shared_ptr<Texture2D> diffuse,
												std::shared_ptr<Texture2D> normal,
												std::shared_ptr<Texture2D> specular,
												std::shared_ptr<Texture2D> depth) :
		FrameBufferRender(painterManager)
	{
		push_storage_output(GL_COLOR_ATTACHMENT0, diffuse);
		push_storage_output(GL_COLOR_ATTACHMENT1, normal);
		push_storage_output(GL_COLOR_ATTACHMENT2, specular);
		push_storage_output(GL_DEPTH_STENCIL_ATTACHMENT, depth);

		_programs.resize(PROGRAM_NBR);

		_programs[PROGRAM_BUFFERING] = std::make_shared<Program>(Program(std::string("program_buffering"),
		{ 
			std::make_shared<UnitProg>(DEFERRED_SHADING_BUFFERING_VERTEX, GL_VERTEX_SHADER), 
			std::make_shared<UnitProg>(DEFERRED_SHADING_BUFFERING_FRAG, GL_FRAGMENT_SHADER) 
		}));
		_forbidden[AGE_OPAQUE] = true;
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

		_programs[PROGRAM_BUFFERING]->use();
		*_programs[PROGRAM_BUFFERING]->get_resource<Mat4>("projection_matrix") = infos.projection;
		*_programs[PROGRAM_BUFFERING]->get_resource<Mat4>("view_matrix") = infos.view;

		for (auto meshPaint : pipeline.keys)
		{
			auto painter = _painterManager->get_painter(Key<Painter>::createKey(meshPaint.first));
			for (auto mode : meshPaint.second.drawables)
			{
				if (renderModeCompatible(mode.renderMode))
				{
					painter->draw(GL_TRIANGLES, _programs[PROGRAM_BUFFERING], mode.properties, mode.vertices);
				}
			}
		}
	}

}
