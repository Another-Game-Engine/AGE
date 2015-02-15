#include <Render/Pipelining/Pipelines/CustomPipeline/DeferredShading.hh>
#include <Render/UnitProg.hh>
#include <Render/Program.hh>
#include <Render/Pipelining/Render/RenderingPass.hh>
#include <Render/ProgramResources/Types/UniformBlock.hh>
#include <Render/Textures/Texture2D.hh>
#include <Render/OpenGLTask/Tasks.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Vec3.hh>
#include <SpacePartitioning/Ouptut/RenderPipeline.hh>
#include <SpacePartitioning/Ouptut/RenderPainter.hh>
#include <SpacePartitioning/Ouptut/RenderCamera.hh>

#define DEFERRED_SHADING_MERGING_VERTEX "../../Shaders/Deffered_shading/deferred_shading_merge.vp"
#define DEFERRED_SHADING_MERGING_FRAG "../../Shaders/Deffered_shading/deferred_shading_merge.fp"
#define DEFERRED_SHADING_LIGHTNING_VERTEX "../../Shaders/Deffered_shading/deffered_shading_lighting.vp"
#define DEFERRED_SHADING_LIGHTNING_FRAG "../../Shaders/Deffered_shading/deffered_shading_lighting.fp"
#define DEFERRED_SHADING_BUFFERING_VERTEX "../../Shaders/Deffered_shading/deffered_shading_get_buffer.vp"
#define DEFERRED_SHADING_BUFFERING_FRAG "../../Shaders/Deffered_shading/deffered_shading_get_buffer.fp"

namespace AGE
{
	std::vector<glm::vec2> DeferredShading::_quadPos =
	{
		glm::vec2(-1, 1),
		glm::vec2(1, 1),
		glm::vec2(1, -1),
		glm::vec2(-1, -1)
	};

	DeferredShading::DeferredShading(glm::vec2 const &screen_size, std::shared_ptr<PaintingManager> const &painter_manager) :
		ARenderingPipeline(std::string("deferred shading"), painter_manager)
	{
		std::vector<GLenum> types;

		types.push_back(GL_FLOAT_VEC2);

		if (_painter_manager->has_painter(types))
			_quadPainter = _painter_manager->get_painter(types);
		else
			_quadPainter = _painter_manager->add_painter(std::move(types));

		auto quadPainterPtr = _painter_manager->get_painter(_quadPainter);

		_quadVertices = quadPainterPtr->add_vertices(4, 4);
		auto quadVerticesPtr = quadPainterPtr->get_vertices(_quadVertices);

		quadVerticesPtr->set_data(_quadPos, 0);

		_programs.resize(TOTAL);
		_programs[BUFFERING] = std::make_shared<Program>(Program(std::string("program_buffering"), { std::make_shared<UnitProg>(DEFERRED_SHADING_BUFFERING_VERTEX, GL_VERTEX_SHADER), std::make_shared<UnitProg>(DEFERRED_SHADING_MERGING_FRAG, GL_FRAGMENT_SHADER) }));
		_programs[LIGHTNING] = std::make_shared<Program>(Program(std::string("program lightning"), { std::make_shared<UnitProg>(DEFERRED_SHADING_LIGHTNING_VERTEX, GL_VERTEX_SHADER), std::make_shared<UnitProg>(DEFERRED_SHADING_LIGHTNING_FRAG, GL_FRAGMENT_SHADER) }));
		_programs[MERGING] = std::make_shared<Program>(Program(std::string("program_merging"), { std::make_shared<UnitProg>(DEFERRED_SHADING_MERGING_VERTEX, GL_VERTEX_SHADER), std::make_shared<UnitProg>(DEFERRED_SHADING_MERGING_FRAG, GL_FRAGMENT_SHADER) }));
		*_programs[BUFFERING]->get_resource<UniformBlock>("global_state")->get_resource("projection_matrix") = glm::mat4(1.0f);
		*_programs[LIGHTNING]->get_resource<UniformBlock>("global_state")->get_resource("projection_matrix") = glm::mat4(1.0f);

		_rendering_list.resize(TOTAL);
		_rendering_list[BUFFERING] = std::make_shared<RenderingPass>([&](FUNCTION_ARGS){
			OpenGLTasks::set_depth_test(true);
			OpenGLTasks::set_clear_color(glm::vec4(0));
			OpenGLTasks::set_blend(false, 0);
			OpenGLTasks::set_blend(false, 1);
			OpenGLTasks::set_blend(false, 2);
			OpenGLTasks::clear_buffer();
			painter.draw(GL_TRIANGLES, _programs[BUFFERING], vertices);
		});
		_rendering_list[LIGHTNING] = std::make_shared<RenderingPass>([&](FUNCTION_ARGS){
			OpenGLTasks::clear_buffer();
			OpenGLTasks::set_depth_test(false);
			OpenGLTasks::set_blend(true, 0);
			OpenGLTasks::set_blend_func(GL_SRC_ALPHA, GL_ONE);
			painter.draw(GL_QUADS, _programs[LIGHTNING], vertices);
		});

		_rendering_list[MERGING] = std::make_shared<RenderingPass>([&](FUNCTION_ARGS){
			painter.draw(GL_QUADS, _programs[MERGING], vertices);
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
		auto quadPainter = _painter_manager->get_painter(_quadPainter);

		// Generate the geometry buffer
		_programs[BUFFERING]->use();
		*_programs[BUFFERING]->get_resource<Mat4>("projection_matrix") = infos.projection;
		*_programs[BUFFERING]->get_resource<Mat4>("view_matrix") = infos.view;
		for (auto &painterKey : pipeline.keys) {
			auto painter = _painter_manager->get_painter(painterKey.painter);
			_rendering_list[BUFFERING]->render(painterKey.vertices, *painter);
		}
		// Compute the lights
		_programs[LIGHTNING]->use();
		*_programs[LIGHTNING]->get_resource<Mat4>("view_matrix") = infos.view;
		for (auto &pointLight : lights.pointLight)
		{
			*_programs[LIGHTNING]->get_resource<Vec3>("position_light") = pointLight.light.position;
			*_programs[LIGHTNING]->get_resource<Vec3>("attenuation_light") = pointLight.light.attenuation;
			*_programs[LIGHTNING]->get_resource<Vec3>("color_light") = pointLight.light.color;
			_rendering_list[LIGHTNING]->render({ _quadVertices }, *quadPainter);
		}
		// Merge the buffers
		_programs[MERGING]->use();
		_rendering_list[MERGING]->render({ _quadVertices }, *quadPainter);
		return (*this);
	}

}