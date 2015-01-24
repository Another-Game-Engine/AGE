#pragma once

# include <Render/Pipelining/Pipelines/IRenderingPipeline.hh>

class Painter;
class Program;

class ARenderingPipeline : public IRenderingPipeline
{
public:
	virtual ~ARenderingPipeline() {};

protected:
	ARenderingPipeline(std::string &&name, std::vector<std::shared_ptr<Painter>> const &painters, std::vector<std::shared_ptr<Program>> const &programs);
	ARenderingPipeline(ARenderingPipeline &&move);

protected:
	virtual IRenderingPipeline &set_rendering(std::vector<std::shared_ptr<IRendering>> const &rendering_list) override final;
	virtual std::vector<std::shared_ptr<IRendering>> const &get_rendering() const override final;
	virtual std::string const &name() const override final;
	virtual std::vector<std::shared_ptr<Program>> const &get_programs() const override final;

protected:
	std::string _name;
	std::vector<std::shared_ptr<IRendering>> _rendering_list;
	std::vector<std::shared_ptr<Program>> _programs;
};