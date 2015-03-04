#pragma once

# include <Utils/OpenGL.hh>
# include <vector>
# include <utility>
# include <functional>
# include <memory>
# include <Render/ProgramResources/IProgramResources.hh>
# include <Render/ProgramResources/Factory/UniformsFactory.hh>
#include <Render/ProgramResources/Types/BlockResources.hh>

#define LAMBDA_PROTO [this](GLint id, std::string &&name)

namespace AGE
{
	class Program;

	class ProgramResourcesFactory
	{
	public:
		typedef std::pair<GLenum, std::function<std::shared_ptr<IProgramResources>(GLint, std::string &&)>> create_type_t;

	public:
		ProgramResourcesFactory(Program const &program);
		ProgramResourcesFactory(ProgramResourcesFactory const &copy) = delete;
		ProgramResourcesFactory &operator=(ProgramResourcesFactory const &p) = delete;

	public:
		std::shared_ptr<IProgramResources> build(GLenum mode, GLint id, std::string &&name);

	private:
		create_type_t _create_uniform();
		create_type_t _create_uniform_block();
		create_type_t _create_input();

	private:
		Program const &_program;
		std::vector<std::pair<GLenum, std::function<std::shared_ptr<IProgramResources>(GLint, std::string &&)>>> _blue_prints;
		UniformsFactory _uniformsFactory;
		std::vector<std::unique_ptr<BlockResources>> _block_resources;
	};

}