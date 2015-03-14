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
#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>
#include <SpacePartitioning/Ouptut/RenderPipeline.hh>
#include <SpacePartitioning/Ouptut/RenderPainter.hh>
#include <SpacePartitioning/Ouptut/RenderCamera.hh>
#include <Utils/Debug.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/ThreadManager.hpp>

#define DEFERRED_SHADING_MERGING_VERTEX "../../Shaders/deferred_shading/deferred_shading_merge.vp"
#define DEFERRED_SHADING_MERGING_FRAG "../../Shaders/deferred_shading/deferred_shading_merge.fp"
#define DEFERRED_SHADING_LIGHTNING_VERTEX "../../Shaders/deferred_shading/deferred_shading_lighting.vp"
#define DEFERRED_SHADING_LIGHTNING_FRAG "../../Shaders/deferred_shading/deferred_shading_lighting.fp"
#define DEFERRED_SHADING_BUFFERING_VERTEX "../../Shaders/deferred_shading/deferred_shading_get_buffer.vp"
#define DEFERRED_SHADING_BUFFERING_FRAG "../../Shaders/deferred_shading/deferred_shading_get_buffer.fp"

namespace AGE
{

	DeferredShading::DeferredShading(glm::uvec2 const &screen_size, std::shared_ptr<PaintingManager> const &painter_manager) :
		ARenderingPipeline(std::string("deferred shading"), painter_manager)
	{
		_programs.resize(TOTAL);
		_rendering_list.resize(TOTAL);
		_programs[BUFFERING] = std::make_shared<Program>(Program(std::string("program_buffering"), { std::make_shared<UnitProg>(DEFERRED_SHADING_BUFFERING_VERTEX, GL_VERTEX_SHADER), std::make_shared<UnitProg>(DEFERRED_SHADING_BUFFERING_FRAG, GL_FRAGMENT_SHADER) }));
		_programs[LIGHTNING] = std::make_shared<Program>(Program(std::string("program lightning"), { std::make_shared<UnitProg>(DEFERRED_SHADING_LIGHTNING_VERTEX, GL_VERTEX_SHADER), std::make_shared<UnitProg>(DEFERRED_SHADING_LIGHTNING_FRAG, GL_FRAGMENT_SHADER) }));
		_programs[MERGING] = std::make_shared<Program>(Program(std::string("program_merging"), { std::make_shared<UnitProg>(DEFERRED_SHADING_MERGING_VERTEX, GL_VERTEX_SHADER), std::make_shared<UnitProg>(DEFERRED_SHADING_MERGING_FRAG, GL_FRAGMENT_SHADER) }));

		_rendering_list[BUFFERING] = std::make_shared<RenderingPass>([&](std::vector<Properties> const &properties, std::vector<Key<Vertices>> const &vertices, std::shared_ptr<Painter> const &painter) {
			OpenGLTasks::set_depth_test(true);
			OpenGLTasks::set_clear_color(glm::vec4(1.f, 0.0f, 0.0f, 1.0f));
			OpenGLTasks::clear_buffer();
			OpenGLTasks::set_blend_test(false, 0);
			OpenGLTasks::set_blend_test(false, 1);
			OpenGLTasks::set_blend_test(false, 2);
			painter->draw(GL_TRIANGLES, _programs[BUFFERING], properties, vertices);
		});

		_rendering_list[LIGHTNING] = std::make_shared<RenderingPass>([&](std::vector<Properties> const &properties, std::vector<Key<Vertices>> const &vertices, std::shared_ptr<Painter> const &painter){
			for (auto &light : _lights->pointLight) {
				Key<Painter> quadPainterKey;
				Key<Vertices> quadVerticesKey;
				GetRenderThread()->getQuadGeometry(quadVerticesKey, quadPainterKey);
				auto myPainter = painter_manager->get_painter(quadPainterKey);
				myPainter->uniqueDraw(GL_TRIANGLES, _programs[LIGHTNING], Properties(), quadVerticesKey);
			}
		});
		_rendering_list[MERGING] = std::make_shared<Rendering>([&](std::vector<Properties> const &properties, std::vector<Key<Vertices>> const &vertices, std::shared_ptr<Painter> const &painter){
			Key<Painter> quadPainterKey;
			Key<Vertices> quadVerticesKey;
			GetRenderThread()->getQuadGeometry(quadVerticesKey, quadPainterKey);
			auto myPainter = painter_manager->get_painter(quadPainterKey);
			myPainter->uniqueDraw(GL_TRIANGLES, _programs[MERGING], Properties(), quadVerticesKey);
		});

		_diffuseTexture = addRenderPassOutput<Texture2D, RenderingPass>(_rendering_list[BUFFERING], GL_COLOR_ATTACHMENT0, screen_size.x, screen_size.y, GL_RGBA8, true);
		_normalTexture = addRenderPassOutput<Texture2D, RenderingPass>(_rendering_list[BUFFERING], GL_COLOR_ATTACHMENT1, screen_size.x, screen_size.y, GL_RGBA8, true);
		_specularTexture = addRenderPassOutput<Texture2D, RenderingPass>(_rendering_list[BUFFERING], GL_COLOR_ATTACHMENT2, screen_size.x, screen_size.y, GL_RGBA8, true);
		_depthTexture = addRenderPassOutput<Texture2D, RenderingPass>(_rendering_list[BUFFERING], GL_DEPTH_ATTACHMENT, screen_size.x, screen_size.y, GL_DEPTH24_STENCIL8, true);

		_lightAccumulationTexture = addRenderPassOutput<Texture2D, RenderingPass>(_rendering_list[LIGHTNING], GL_COLOR_ATTACHMENT0, screen_size.x, screen_size.y, GL_RGB8, true);
	}

	DeferredShading::DeferredShading(DeferredShading &&move) :
		ARenderingPipeline(std::move(move))
	{

	}

	IRenderingPipeline & DeferredShading::render(RenderPipeline const &pipeline, RenderLightList const &lights, CameraInfos const &infos)
	{
		_lights = &lights;
		OpenGLTasks::set_depth_test(true);
		OpenGLTasks::set_clear_color(glm::vec4(1.f, 0.0f, 0.0f, 1.0f));
		OpenGLTasks::clear_buffer();
		if (!_programs[BUFFERING]->isCompiled()) {
			return (*this);
		}

		_programs[BUFFERING]->use();
		*_programs[BUFFERING]->get_resource<Mat4>("projection_matrix") = infos.projection;
		*_programs[BUFFERING]->get_resource<Mat4>("view_matrix") = infos.view;
		for (auto key : pipeline.keys)
		{
			_rendering_list[BUFFERING]->render(key.properties, key.vertices, _painter_manager->get_painter(key.painter));
		}

		_programs[LIGHTNING]->use();
		*_programs[LIGHTNING]->get_resource<Mat4>("projection_matrix") = infos.projection;
		*_programs[LIGHTNING]->get_resource<Mat4>("view_matrix") = infos.view;
		auto &normalMap = _programs[LIGHTNING]->get_resource<Sampler2D>("normal_buffer");
		auto &depthMap = _programs[LIGHTNING]->get_resource<Sampler2D>("depth_buffer");
		if (normalMap)
		{
			*normalMap = _normalTexture;
		}
		if (depthMap)
		{
			*depthMap = _depthTexture;
		}
		for (auto key : pipeline.keys)
		{
			_rendering_list[LIGHTNING]->render(key.properties, key.vertices, _painter_manager->get_painter(key.painter));
		}

		_programs[MERGING]->use();
		auto &mapColor = _programs[MERGING]->get_resource<Sampler2D>("diffuse_map");
		if (mapColor)
		{
			*mapColor = _diffuseTexture;
		}
		for (auto key : pipeline.keys)
		{
			_rendering_list[MERGING]->render(key.properties, key.vertices, _painter_manager->get_painter(key.painter));
		}
		return (*this);
	}

}