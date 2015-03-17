#pragma once

# include <Render/Pipelining/Pipelines/IRenderingPipeline.hh>
# include <utility>

namespace AGE
{
	class ARender;
	class Painter;
	class Program;
	class PaintingManager;
	class PropertyManager;

	class ARenderingPipeline : public IRenderingPipeline
	{
	public:
		virtual ~ARenderingPipeline() {}
		virtual bool recompileShaders();

		std::shared_ptr<PaintingManager> getPainterManager() const;

	protected:
		ARenderingPipeline(std::string &&name, std::shared_ptr<PaintingManager> const &painter_manager);
		ARenderingPipeline(ARenderingPipeline &&move);

	protected:
		virtual std::string const &name() const override final;

	protected:
		std::string _name;
		std::shared_ptr<PaintingManager> _painter_manager;
		std::vector<std::shared_ptr<ARender>> _rendering_list;
	};

}