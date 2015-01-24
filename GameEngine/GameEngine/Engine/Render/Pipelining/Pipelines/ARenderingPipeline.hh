#pragma once

# include <Render/Pipelining/Pipelines/IRenderingPipeline.hh>
# include <utility>

class Painter;
class Program;

class ARenderingPipeline : public IRenderingPipeline
{
public:
	virtual ~ARenderingPipeline() {}

protected:
	ARenderingPipeline(std::string &&name);
	ARenderingPipeline(ARenderingPipeline &&move);

protected:
	virtual std::vector<std::shared_ptr<IRendering>> const &get_rendering() const override final;
	virtual std::vector<std::shared_ptr<Program>> const &get_programs() const override final;
	virtual std::string const &name() const override final;

protected:
	std::string _name;
	std::vector<std::shared_ptr<IRendering>> _rendering_list;
	std::vector<std::shared_ptr<Program>> _programs;
};