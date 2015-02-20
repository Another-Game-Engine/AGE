#include <Render/Pipelining/Pipelines/ARenderingPipeline.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/Program.hh>

namespace AGE
{

	ARenderingPipeline::ARenderingPipeline(std::string &&name, std::shared_ptr<PaintingManager> const &painter_manager,
		std::shared_ptr<PropertyManager> const &property_manager) :
		_name(std::move(name)),
		_painter_manager(painter_manager),
		_property_manager(property_manager)
	{

	}

	ARenderingPipeline::ARenderingPipeline(ARenderingPipeline &&move) :
		_name(std::move(move._name)),
		_painter_manager(std::move(move._painter_manager)),
		_property_manager(std::move(move._property_manager)),
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

}