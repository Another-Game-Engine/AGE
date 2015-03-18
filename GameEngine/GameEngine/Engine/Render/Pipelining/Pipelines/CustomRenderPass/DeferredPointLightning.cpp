#include <Render/Pipelining/Pipelines/CustomRenderPass/DeferredPointLightning.hh>

#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/Tasks.hh>
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

#define DEFERRED_SHADING_STENCIL_VERTEX "../../Shaders/deferred_shading/basic_3d.vp"
#define DEFERRED_SHADING_STENCIL_FRAG "../../Shaders/deferred_shading/basic_3d.fp"

#define DEFERRED_SHADING_POINT_LIGHT_VERTEX "../../Shaders/deferred_shading/deferred_shading_point_light.vp"
#define DEFERRED_SHADING_POINT_LIGHT_FRAG "../../Shaders/deferred_shading/deferred_shading_point_light.fp"

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

		_programs[PROGRAM_STENCIL] = std::make_shared<Program>(Program(std::string("basic_3d_render"),
		{
			std::make_shared<UnitProg>(DEFERRED_SHADING_STENCIL_VERTEX, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(DEFERRED_SHADING_STENCIL_FRAG, GL_FRAGMENT_SHADER)
		}));

		_programs[PROGRAM_LIGHTNING] = std::make_shared<Program>(Program(std::string("program_point_light"),
		{
			std::make_shared<UnitProg>(DEFERRED_SHADING_POINT_LIGHT_VERTEX, GL_VERTEX_SHADER),
			std::make_shared<UnitProg>(DEFERRED_SHADING_POINT_LIGHT_FRAG, GL_FRAGMENT_SHADER)
		}));
	}

	void DeferredPointLightning::renderPass(RenderPipeline const &, RenderLightList const &lights, CameraInfos const &infos)
	{
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
		glDisable(GL_BLEND);
		// clear the light accumulation to zero
		OpenGLTasks::set_clear_color(glm::vec4(0));
		OpenGLTasks::clear_buffer(true, false, false);
		// activate depth test and func to check if sphere_depth > current_depth (normal zfail)
		OpenGLTasks::set_depth_test(true);
		glDepthFunc(GL_GEQUAL);
		// We activate the stencil test
		OpenGLTasks::set_stencil_test(true);
		// We do not write on the depth buffer
		glDepthMask(GL_FALSE);
		// And we set the blend mode to additive
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		// We get the sphere geometry
		Key<Vertices> sphereVertices;
		Key<Painter> spherePainter;
		GetRenderThread()->getIcoSphereGeometry(sphereVertices, spherePainter, 2);
		auto spherePainterPtr = _painterManager->get_painter(spherePainter);
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
			*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("ambiant_color") = glm::vec3(0);

			// We clear the stencil buffer
			OpenGLTasks::set_clear_stencil(0);
			OpenGLTasks::clear_buffer(false, false, true);

			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

			glStencilFunc(GL_ALWAYS, 0, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			glCullFace(GL_BACK);

			spherePainterPtr->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_STENCIL], Properties(), sphereVertices);

			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glStencilFunc(GL_EQUAL, 0, 0xFF);
			glCullFace(GL_FRONT);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

			spherePainterPtr->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_LIGHTNING], Properties(), sphereVertices);
		}
	}
}
