#include <Render/Pipelining/Pipelines/CustomPipeline/DeferredShading.hh>
#include <Render/UnitProg.hh>
#include <Render/Program.hh>
#include <Render/Pipelining/Render/RenderingPass.hh>
#include <Render/Pipelining/Render/Rendering.hh>
#include <Render/Pipelining/Buffer/Renderbuffer.hh>
#include <Render/ProgramResources/Types/UniformBlock.hh>
#include <Render/OpenGLTask/Tasks.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Vec3.hh>
#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>
#include <SpacePartitioning/Ouptut/RenderPipeline.hh>
#include <SpacePartitioning/Ouptut/RenderPainter.hh>
#include <SpacePartitioning/Ouptut/RenderCamera.hh>
#include <Utils/Debug.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>

#define DEFERRED_SHADING_MERGING_VERTEX "../../Shaders/deferred_shading/deferred_shading_merge.vp"
#define DEFERRED_SHADING_MERGING_FRAG "../../Shaders/deferred_shading/deferred_shading_merge.fp"

#define DEFERRED_SHADING_STENCIL_VERTEX "../../Shaders/deferred_shading/basic_3d.vp"
#define DEFERRED_SHADING_STENCIL_FRAG "../../Shaders/deferred_shading/basic_3d.fp"

#define DEFERRED_SHADING_LIGHTNING_VERTEX "../../Shaders/deferred_shading/deferred_shading_lighting.vp"
#define DEFERRED_SHADING_LIGHTNING_FRAG "../../Shaders/deferred_shading/deferred_shading_lighting.fp"

#define DEFERRED_SHADING_BUFFERING_VERTEX "../../Shaders/deferred_shading/deferred_shading_get_buffer.vp"
#define DEFERRED_SHADING_BUFFERING_FRAG "../../Shaders/deferred_shading/deferred_shading_get_buffer.fp"


namespace AGE
{

	DeferredShading::DeferredShading(glm::uvec2 const &screen_size, std::shared_ptr<PaintingManager> const &painter_manager) :
		ARenderingPipeline(std::string("deferred shading"), painter_manager)
	{
		_programs.resize(TOTAL_PROGRAMS);
		_rendering_list.resize(TOTAL_RENDER);
		_programs[PROGRAM_BUFFERING] = std::make_shared<Program>(Program(std::string("program_buffering"), { std::make_shared<UnitProg>(DEFERRED_SHADING_BUFFERING_VERTEX, GL_VERTEX_SHADER), std::make_shared<UnitProg>(DEFERRED_SHADING_BUFFERING_FRAG, GL_FRAGMENT_SHADER) }));
		_programs[PROGRAM_STENCIL_BASIC] = std::make_shared<Program>(Program(std::string("program_stencil"), { std::make_shared<UnitProg>(DEFERRED_SHADING_STENCIL_VERTEX, GL_VERTEX_SHADER), std::make_shared<UnitProg>(DEFERRED_SHADING_STENCIL_FRAG, GL_FRAGMENT_SHADER) }));
		_programs[PROGRAM_LIGHTNING] = std::make_shared<Program>(Program(std::string("program lightning"), { std::make_shared<UnitProg>(DEFERRED_SHADING_LIGHTNING_VERTEX, GL_VERTEX_SHADER), std::make_shared<UnitProg>(DEFERRED_SHADING_LIGHTNING_FRAG, GL_FRAGMENT_SHADER) }));
		_programs[PROGRAM_MERGING] = std::make_shared<Program>(Program(std::string("program_merging"), { std::make_shared<UnitProg>(DEFERRED_SHADING_MERGING_VERTEX, GL_VERTEX_SHADER), std::make_shared<UnitProg>(DEFERRED_SHADING_MERGING_FRAG, GL_FRAGMENT_SHADER) }));

		_rendering_list[RENDER_BUFFERING] = std::make_shared<RenderingPass>([&](std::vector<Properties> const &properties, std::vector<Key<Vertices>> const &vertices, std::shared_ptr<Painter> const &painter) {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glDepthMask(GL_TRUE);
			glDepthFunc(GL_LEQUAL);
			OpenGLTasks::set_stencil_test(false);
			OpenGLTasks::set_depth_test(true);
			OpenGLTasks::set_clear_color(glm::vec4(0.f, 0.0f, 0.0f, 1.0f));
			OpenGLTasks::clear_buffer();
			OpenGLTasks::set_blend_test(false, 0);
			OpenGLTasks::set_blend_test(false, 1);
			OpenGLTasks::set_blend_test(false, 2);
			painter->draw(GL_TRIANGLES, _programs[PROGRAM_BUFFERING], properties, vertices);
		});

		_rendering_list[RENDER_CLEAR_STEP] = std::make_shared<RenderingPass>([&](std::vector<Properties> const &properties, std::vector<Key<Vertices>> const &vertices, std::shared_ptr<Painter> const &painter){
			glDisable(GL_BLEND);
			OpenGLTasks::set_clear_color(glm::vec4(0, 0, 0, 0));
			OpenGLTasks::clear_buffer(true, false, false);
		});

		_rendering_list[RENDER_LIGHTNING] = std::make_shared<RenderingPass>([&](std::vector<Properties> const &properties, std::vector<Key<Vertices>> const &vertices, std::shared_ptr<Painter> const &painter){

			OpenGLTasks::set_depth_test(true);
			glDepthFunc(GL_GEQUAL);
			glDepthMask(GL_FALSE);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			OpenGLTasks::set_clear_stencil(0);
			OpenGLTasks::clear_buffer(false, false, true);
			OpenGLTasks::set_stencil_test(true);

			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

			glStencilFunc(GL_ALWAYS, 0, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
			glCullFace(GL_BACK);

			painter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_STENCIL_BASIC], Properties(), vertices.back());

			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glStencilFunc(GL_EQUAL, 0, 0xFF);
			glCullFace(GL_FRONT);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

			painter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_LIGHTNING], Properties(), vertices.back());
		});
		_rendering_list[RENDER_MERGING] = std::make_shared<Rendering>([&](std::vector<Properties> const &properties, std::vector<Key<Vertices>> const &vertices, std::shared_ptr<Painter> const &painter){
			Key<Painter> quadPainterKey;
			Key<Vertices> quadVerticesKey;
	
			glDisable(GL_BLEND);
			glDisable(GL_CULL_FACE);
			OpenGLTasks::set_depth_test(false);
			OpenGLTasks::set_stencil_test(false);
			GetRenderThread()->getQuadGeometry(quadVerticesKey, quadPainterKey);
			auto myPainter = painter_manager->get_painter(quadPainterKey);
			myPainter->uniqueDraw(GL_TRIANGLES, _programs[PROGRAM_MERGING], Properties(), quadVerticesKey);
		});

		_diffuseTexture = addRenderPassOutput<Texture2D, RenderingPass>(_rendering_list[RENDER_BUFFERING], GL_COLOR_ATTACHMENT0, screen_size.x, screen_size.y, GL_RGBA8, true);
		_normalTexture = addRenderPassOutput<Texture2D, RenderingPass>(_rendering_list[RENDER_BUFFERING], GL_COLOR_ATTACHMENT1, screen_size.x, screen_size.y, GL_RGBA8, true);
		_specularTexture = addRenderPassOutput<Texture2D, RenderingPass>(_rendering_list[RENDER_BUFFERING], GL_COLOR_ATTACHMENT2, screen_size.x, screen_size.y, GL_RGBA8, true);
		_depthTexture = addRenderPassOutput<Texture2D, RenderingPass>(_rendering_list[RENDER_BUFFERING], GL_DEPTH_STENCIL_ATTACHMENT, screen_size.x, screen_size.y, GL_DEPTH24_STENCIL8, true);

		std::static_pointer_cast<RenderingPass>(_rendering_list[RENDER_LIGHTNING])->push_storage_output(GL_DEPTH_STENCIL_ATTACHMENT, _depthTexture);

		// RGB = light color, A = specular power
		_lightAccumulationTexture = addRenderPassOutput<Texture2D, RenderingPass>(_rendering_list[RENDER_LIGHTNING], GL_COLOR_ATTACHMENT0, screen_size.x, screen_size.y, GL_RGBA8, true);

		std::static_pointer_cast<RenderingPass>(_rendering_list[RENDER_CLEAR_STEP])->push_storage_output(GL_COLOR_ATTACHMENT0, _lightAccumulationTexture);
	}

	DeferredShading::DeferredShading(DeferredShading &&move) :
		ARenderingPipeline(std::move(move))
	{

	}

	IRenderingPipeline & DeferredShading::render(RenderPipeline const &pipeline, RenderLightList const &lights, CameraInfos const &infos)
	{
		OpenGLTasks::set_depth_test(true);

		if (!_programs[PROGRAM_BUFFERING]->isCompiled()) {
			return (*this);
		}

		glm::vec3 cameraPosition = -glm::transpose(glm::mat3(infos.view)) * glm::vec3(infos.view[3]);

		_programs[PROGRAM_BUFFERING]->use();
		*_programs[PROGRAM_BUFFERING]->get_resource<Mat4>("projection_matrix") = infos.projection;
		*_programs[PROGRAM_BUFFERING]->get_resource<Mat4>("view_matrix") = infos.view;
		for (auto key : pipeline.keys)
		{
			_rendering_list[RENDER_BUFFERING]->render(key.properties, key.vertices, _painter_manager->get_painter(key.painter));
		}

		_rendering_list[RENDER_CLEAR_STEP]->render(std::vector<Properties>(), std::vector<Key<Vertices>>(), nullptr);

		_programs[PROGRAM_LIGHTNING]->use();
		*_programs[PROGRAM_LIGHTNING]->get_resource<Mat4>("projection_matrix") = infos.projection;
		*_programs[PROGRAM_LIGHTNING]->get_resource<Mat4>("view_matrix") = infos.view;
		*_programs[PROGRAM_LIGHTNING]->get_resource<Sampler2D>("normal_buffer") = _normalTexture;
		*_programs[PROGRAM_LIGHTNING]->get_resource<Sampler2D>("depth_buffer") = _depthTexture;
		*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("eye_pos") = cameraPosition;

		_programs[PROGRAM_STENCIL_BASIC]->use();
		*_programs[PROGRAM_STENCIL_BASIC]->get_resource<Mat4>("projection_matrix") = infos.projection;
		*_programs[PROGRAM_STENCIL_BASIC]->get_resource<Mat4>("view_matrix") = infos.view;

		for (auto &pl : lights.pointLight)
		{
			_programs[PROGRAM_STENCIL_BASIC]->use();
			*_programs[PROGRAM_STENCIL_BASIC]->get_resource<Mat4>("model_matrix") = pl.light.sphereTransform;

			_programs[PROGRAM_LIGHTNING]->use();
			*_programs[PROGRAM_LIGHTNING]->get_resource<Mat4>("model_matrix") = pl.light.sphereTransform;
			*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("position_light") = glm::vec3(pl.light.sphereTransform[3]);
			*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("attenuation_light") = pl.light.attenuation;
			*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("color_light") = pl.light.color;
			*_programs[PROGRAM_LIGHTNING]->get_resource<Vec3>("ambiant_color") = glm::vec3(0);

			std::vector<Key<Vertices>> sphereVertices;
			Key<Painter> spherePainter;
			
			sphereVertices.emplace_back();
			GetRenderThread()->getIcoSphereGeometry(sphereVertices.back(), spherePainter, 3);

			_rendering_list[RENDER_LIGHTNING]->render(std::vector<Properties>(), sphereVertices, _painter_manager->get_painter(spherePainter));
		}

		_programs[PROGRAM_MERGING]->use();
		*_programs[PROGRAM_MERGING]->get_resource<Sampler2D>("diffuse_map") = _diffuseTexture;
		*_programs[PROGRAM_MERGING]->get_resource<Sampler2D>("light_buffer") = _lightAccumulationTexture;
//		*_programs[PROGRAM_MERGING]->get_resource<Sampler2D>("specular_map") = _specularTexture;
		for (auto key : pipeline.keys)
		{
			_rendering_list[RENDER_MERGING]->render(key.properties, key.vertices, _painter_manager->get_painter(key.painter));
		}
		return (*this);
	}

}