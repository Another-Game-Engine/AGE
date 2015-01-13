#include <Render/Pipelining/Pipelines/ARenderingPipeline.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/Program.hh>

ARenderingPipeline::ARenderingPipeline(std::string &&name, std::vector<std::shared_ptr<Painter>> const &painters, std::vector<std::shared_ptr<Program>> const &programs) :
_name(std::move(name)),
_painters(painters),
_programs(programs)
{

}

ARenderingPipeline::ARenderingPipeline(ARenderingPipeline &&move) : 
_name(std::move(move._name)),
_rendering_list(std::move(move._rendering_list)),
_painters(std::move(move._painters)),
_programs(std::move(move._programs))
{

}

IRenderingPipeline & ARenderingPipeline::set_rendering(std::vector<std::shared_ptr<IRendering>> const &rendering_list)
{
	_rendering_list = rendering_list;
	return (*this);
}

std::vector<std::shared_ptr<IRendering>> const & ARenderingPipeline::get_rendering() const
{
	return (_rendering_list);
}

std::string const & ARenderingPipeline::name() const
{
	return (_name);
}

std::vector<std::shared_ptr<Painter>> const & ARenderingPipeline::get_painters() const
{
	return (_painters);
}

std::vector<std::shared_ptr<Program>> const & ARenderingPipeline::get_programs() const
{
	return (_programs);
}
