#include <Render/Pipelining/Pipelines/CustomPipeline/BasicPipeline.hh>
#include <Render/Program.hh>
#include <Render/Pipelining/Render/Rendering.hh>
#include <vector>
#include <memory>

#define VERTEX_SHADER "../../Shaders/.vp", GL_VERTEX_SHADER
#define FRAGMENT_SHADER "../../Shaders/.fp", GL_FRAGMENT_SHADER

namespace AGE
{

	BasicPipeline::BasicPipeline(std::shared_ptr<PaintingManager> const &painter_manager) :
		ARenderingPipeline(std::string("BasicName"), painter_manager),
	{
		_programs.resize(TOTAL);
		std::vector<std::shared_ptr<UnitProg>> units = { std::make_shared<UnitProg>(VERTEX_SHADER), std::make_shared<UnitProg>(FRAGMENT_SHADER) };
		_programs[RENDER] = std::make_shared<Program>(Program(std::string("basic program"), units));
		_rendering_list.resize(TOTAL);
		_rendering_list[RENDER] = std::make_shared<Rendering>([&](FUNCTION_ARGS) {
			painter.draw(GL_TRIANGLES, _programs[RENDER], vertices);
		});
		auto &rendering = std::static_pointer_cast<Rendering>(_rendering_list[RENDER]);
	}

	BasicPipeline::BasicPipeline(BasicPipeline &&move) :
		ARenderingPipeline(std::move(move))
	{
	}

	IRenderingPipeline &BasicPipeline::render(ARGS_FUNCTION_RENDER)
	{
		for (auto &painter : pipeline.painters) {
			auto p = _painter_manager->get_painter(painter.painter);
			_rendering_list[RENDER]->render(painter.vertices, *p);
		}
		return (*this);
	}

}