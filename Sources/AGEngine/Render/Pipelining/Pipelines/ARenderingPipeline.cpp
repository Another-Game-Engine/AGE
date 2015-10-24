#include <Render/Pipelining/Pipelines/ARenderingPipeline.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/Program.hh>
#include <Render/Pipelining/Render/ARender.hh>

namespace AGE
{

	ARenderingPipeline::~ARenderingPipeline()
	{
		if (_spotlightRenderInfos != nullptr)
		{
			delete _spotlightRenderInfos;
		}
	}


	ARenderingPipeline::ARenderingPipeline(std::string &&name, std::shared_ptr<PaintingManager> const &painter_manager) :
		_name(std::move(name)),
		_painter_manager(painter_manager)
	{
		_spotlightRenderInfos = nullptr;
	}

	ARenderingPipeline::ARenderingPipeline(ARenderingPipeline &&move) :
		_name(std::move(move._name)),
		_painter_manager(std::move(move._painter_manager))
	{
		_spotlightRenderInfos = nullptr;
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

	bool ARenderingPipeline::init()
	{
		for (auto &e : _rendering_list)
		{
			e->init();
		}
		return true;
	}

	IRenderingPipeline & ARenderingPipeline::render(const DRBCameraDrawableList &infos)
	{
		SCOPE_profile_cpu_i("RenderTimer", "RenderPipeline");
		renderBegin();
		// We iterate over the entry points5
		for (auto &renderPass : _rendering_list)
		{
			renderPass->render(infos);
		}
		renderEnd();
		return (*this);
	}
}