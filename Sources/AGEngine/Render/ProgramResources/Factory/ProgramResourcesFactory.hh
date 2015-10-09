#pragma once

# include <Utils/OpenGL.hh>
# include <vector>
# include <utility>
# include <functional>
# include <memory>
# include <Render/ProgramResources/IProgramResources.hh>
# include <Render/ProgramResources/Factory/UniformsFactory.hh>
#include <Render/ProgramResources/Types/BlockResources.hh>

namespace AGE
{
	class Program;

	class ProgramResourcesFactory
	{
	public:
		ProgramResourcesFactory(Program const &program);
		ProgramResourcesFactory(ProgramResourcesFactory const &copy) = delete;
		ProgramResourcesFactory &operator=(ProgramResourcesFactory const &p) = delete;

	public:
		std::shared_ptr<IProgramResources> build(GLenum mode, GLint id, std::string &&name, bool isArray);
		void reset();
	private:
		Program const &_program;
		std::vector<std::pair<GLenum, std::function<std::shared_ptr<IProgramResources>(GLint, std::string &&, bool)>>> _blue_prints;
		UniformsFactory _uniformsFactory;
		std::vector<std::unique_ptr<BlockResources>> _block_resources;
	};
}