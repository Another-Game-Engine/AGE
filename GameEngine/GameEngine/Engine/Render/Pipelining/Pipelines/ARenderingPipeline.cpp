#include <Render/Pipelining/Pipelines/ARenderingPipeline.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/Program.hh>
#include <Render/Pipelining/Render/ARender.hh>

namespace AGE
{

	ARenderingPipeline::ARenderingPipeline(std::string &&name, std::shared_ptr<PaintingManager> const &painter_manager) :
		_name(std::move(name)),
		_painter_manager(painter_manager)
	{

	}

	ARenderingPipeline::ARenderingPipeline(ARenderingPipeline &&move) :
		_name(std::move(move._name)),
		_painter_manager(std::move(move._painter_manager))
	{

	}

	std::shared_ptr<PaintingManager> ARenderingPipeline::getPainterManager() const
	{
		return (_painter_manager);
	}

	std::string const & ARenderingPipeline::name() const
	{
		return (_name);
	}

	bool ARenderingPipeline::recompileShaders()
	{
		for (auto &e : _rendering_list)
		{
			auto success = e->recompilePrograms();
			if (!success)
			{
				return false;
			}
		}
		return true;
	}

	IRenderingPipeline & ARenderingPipeline::render(RenderPipeline const &pipeline, RenderLightList const &lights, CameraInfos const &camInfos)
	{
		// We iterate over the entry points
		for (auto &renderPass : _rendering_list)
		{
			renderPass->render(pipeline, lights, camInfos);
		}
		return (*this);
	}

}