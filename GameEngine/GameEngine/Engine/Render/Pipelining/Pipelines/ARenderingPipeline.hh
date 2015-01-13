#pragma once

# include <Render/Pipelining/Pipelines/IRenderingPipeline.hh>

class ARenderingPipeline : public IRenderingPipeline
{
public:
	virtual ~ARenderingPipeline() {};

protected:
	ARenderingPipeline();
	ARenderingPipeline(ARenderingPipeline &&move);

protected:
	virtual IRenderingPipeline &set_rendering(std::vector<std::shared_ptr<IRendering>> const &rendering_list) override final;
	virtual std::vector<std::shared_ptr<IRendering>> const &get_rendering() const override final;
	virtual std::string const &name() const override final;

protected:
	std::vector<std::shared_ptr<IRendering>> _rendering_list;
};