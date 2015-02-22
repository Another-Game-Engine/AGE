#include <Render/Pipelining/Pipelines/CustomPipeline/DeferredShading.hh>
#include <Render/UnitProg.hh>
#include <Render/Program.hh>
#include <Render/Pipelining/Render/RenderingPass.hh>
#include <Render/Pipelining/Render/Rendering.hh>
#include <Render/ProgramResources/Types/UniformBlock.hh>
#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/Tasks.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <SpacePartitioning/Ouptut/RenderPipeline.hh>
#include <SpacePartitioning/Ouptut/RenderPainter.hh>
#include <SpacePartitioning/Ouptut/RenderCamera.hh>

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
		_programs[BUFFERING] = std::make_shared<Program>(Program(std::string("program_buffering"), { std::make_shared<UnitProg>(DEFERRED_SHADING_BUFFERING_VERTEX, GL_VERTEX_SHADER), std::make_shared<UnitProg>(DEFERRED_SHADING_MERGING_FRAG, GL_FRAGMENT_SHADER) }));
		_programs[LIGHTNING] = std::make_shared<Program>(Program(std::string("program lightning"), { std::make_shared<UnitProg>(DEFERRED_SHADING_LIGHTNING_VERTEX, GL_VERTEX_SHADER), std::make_shared<UnitProg>(DEFERRED_SHADING_LIGHTNING_FRAG, GL_FRAGMENT_SHADER) }));
		_programs[MERGING] = std::make_shared<Program>(Program(std::string("program_merging"), { std::make_shared<UnitProg>(DEFERRED_SHADING_MERGING_VERTEX, GL_VERTEX_SHADER), std::make_shared<UnitProg>(DEFERRED_SHADING_MERGING_FRAG, GL_FRAGMENT_SHADER) }));
		_rendering_list[BUFFERING] = std::make_shared<RenderingPass>([&](FUNCTION_ARGS) {
			OpenGLTasks::set_depth_test(true);
			OpenGLTasks::set_clear_color(glm::vec4(0.25f, 0.25f, 0.25f, 1.0f));
			OpenGLTasks::set_blend_test(false, 0);
			OpenGLTasks::set_blend_test(false, 1);
			OpenGLTasks::set_blend_test(false, 2);
			OpenGLTasks::clear_buffer();
			painter->draw(GL_TRIANGLES, _programs[BUFFERING], properties, vertices);
			glFlush();
		});
		_rendering_list[LIGHTNING] = std::make_shared<RenderingPass>([&](FUNCTION_ARGS){
		});
		_rendering_list[MERGING] = std::make_shared<RenderingPass>([&](FUNCTION_ARGS){
		});
		std::static_pointer_cast<RenderingPass>(_rendering_list[BUFFERING])->push_storage_output(GL_COLOR_ATTACHMENT0, std::make_shared<Texture2D>(Texture2D(screen_size.x, screen_size.y, GL_RGBA8, true)));
		std::static_pointer_cast<RenderingPass>(_rendering_list[BUFFERING])->push_storage_output(GL_COLOR_ATTACHMENT1, std::make_shared<Texture2D>(Texture2D(screen_size.x, screen_size.y, GL_RGBA8, true)));
		std::static_pointer_cast<RenderingPass>(_rendering_list[BUFFERING])->push_storage_output(GL_COLOR_ATTACHMENT2, std::make_shared<Texture2D>(Texture2D(screen_size.x, screen_size.y, GL_RGBA8, true)));
		std::static_pointer_cast<RenderingPass>(_rendering_list[BUFFERING])->push_storage_output(GL_DEPTH_ATTACHMENT, std::make_shared<Texture2D>(Texture2D(screen_size.x, screen_size.y, GL_RGBA8, true)));
	}

	DeferredShading::DeferredShading(DeferredShading &&move) :
		ARenderingPipeline(std::move(move))
	{

	}

	IRenderingPipeline & DeferredShading::render(ARGS_FUNCTION_RENDER)
	{
		_programs[BUFFERING]->use();
		*_programs[BUFFERING]->get_resource<Mat4>("projection_matrix") = infos.projection;
		*_programs[BUFFERING]->get_resource<Mat4>("view_matrix") = infos.view;
		for (auto key : pipeline.keys) {
			_rendering_list[BUFFERING]->render(key.properties, key.vertices, _painter_manager->get_painter(key.painter));
		}
		return (*this);
	}

}