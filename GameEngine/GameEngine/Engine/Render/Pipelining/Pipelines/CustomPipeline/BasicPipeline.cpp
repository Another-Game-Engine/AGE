#include <Render/Pipelining/Pipelines/CustomPipeline/BasicPipeline.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4Array255.hh>
#include <Render/ProgramResources/Types/Uniform/Vec1.hh>
#include <Render/Program.hh>
#include <Render/Pipelining/Render/Rendering.hh>
#include <Render/OpenGLTask/Tasks.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/Properties/Properties.hh>
#include <Render/GeometryManagement/Painting/PaintingManager.hh>
#include <SpacePartitioning/Ouptut/RenderPipeline.hh>
#include <SpacePartitioning/Ouptut/RenderPainter.hh>
#include <SpacePartitioning/Ouptut/RenderCamera.hh>
#include <vector>
#include <memory>

#define VERTEX_SHADER "../../Shaders/test_pipeline_1.vp", GL_VERTEX_SHADER
#define FRAGMENT_SHADER "../../Shaders/test_pipeline_1.fp", GL_FRAGMENT_SHADER

namespace AGE
{

	BasicPipeline::BasicPipeline(std::shared_ptr<PaintingManager> const &painter_manager) :
		ARenderingPipeline(std::string("BasicName"), painter_manager)
	{
		_programs.resize(TOTAL);
		std::vector<std::shared_ptr<UnitProg>> units = { std::make_shared<UnitProg>(VERTEX_SHADER), std::make_shared<UnitProg>(FRAGMENT_SHADER) };
		_programs[RENDER] = std::make_shared<Program>(Program(std::string("basic program"), units));
		_rendering_list.resize(TOTAL);
		_rendering_list[RENDER] = std::make_shared<Rendering>([&](FUNCTION_ARGS) {
			OpenGLTasks::set_depth_test(true);
			OpenGLTasks::set_clear_color(glm::vec4(0, 0, 0.2, 1));
			OpenGLTasks::clear_buffer();
			painter->draw(GL_TRIANGLES, _programs[RENDER], properties, vertices);
		});
		auto &rendering = std::static_pointer_cast<Rendering>(_rendering_list[RENDER]);
	}

	BasicPipeline::BasicPipeline(BasicPipeline &&move) :
		ARenderingPipeline(std::move(move))
	{
	}

	IRenderingPipeline &BasicPipeline::render(ARGS_FUNCTION_RENDER)
	{
		_programs[RENDER]->use();
		*_programs[RENDER]->get_resource<Mat4>("projection_matrix") = infos.projection;
		*_programs[RENDER]->get_resource<Mat4>("view_matrix") = infos.view;
		//*_programs[RENDER]->get_resource<Mat4Array255>("bones") = &(infos.view);
		*_programs[RENDER]->get_resource<Vec1>("skinned") = 0.0f;

		for (auto key : pipeline.keys) {
			_rendering_list[RENDER]->render(key.properties, key.vertices, _painter_manager->get_painter(key.painter));
		}
		return (*this);
	}

}