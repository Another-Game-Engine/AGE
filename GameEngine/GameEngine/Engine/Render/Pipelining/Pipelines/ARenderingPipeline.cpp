#include <Render/Pipelining/Pipelines/ARenderingPipeline.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/Program.hh>

ARenderingPipeline::ARenderingPipeline(std::string &&name) :
_name(std::move(name))
{

}

ARenderingPipeline::ARenderingPipeline(ARenderingPipeline &&move) : 
_name(std::move(move._name)),
_rendering_list(std::move(move._rendering_list)),
_programs(std::move(move._programs))
{

}

std::vector<std::shared_ptr<IRendering>> const & ARenderingPipeline::get_rendering() const
{
	return (_rendering_list);
}

std::string const & ARenderingPipeline::name() const
{
	return (_name);
}

std::vector<std::shared_ptr<Program>> const & ARenderingPipeline::get_programs() const
{
	return (_programs);
}
