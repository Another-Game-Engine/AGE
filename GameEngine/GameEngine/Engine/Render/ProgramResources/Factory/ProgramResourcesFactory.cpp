#include <Render/ProgramResources/Factory/ProgramResourcesFactory.hh>
#include <Render/Program.hh>
#include <array>

#define LAMBDA_PROTO [this](GLint id, std::string &&name)

# define DECLAR_BUILDERS \
std::make_pair(GL_UNIFORM, LAMBDA_PROTO \
{ \
	auto const nbr_prop = 2; \
	std::array<GLenum, nbr_prop> const prop = {GL_TYPE, GL_BLOCK_INDEX }; \
	std::array<GLint, nbr_prop> params; \
	glGetProgramResourceiv(_program.id(), GL_UNIFORM, id, nbr_prop, prop.data(), nbr_prop, NULL, params.data()); \
	return (params[1] == -1 ? _uniformsFactory.build(params[0], id, std::move(name)) : nullptr); \
})


ProgramResourcesFactory::ProgramResourcesFactory(Program const &program) :
_program(program),
_blue_prints({ DECLAR_BUILDERS })
{

}

std::unique_ptr<IProgramResources> ProgramResourcesFactory::build(GLenum mode, GLint id, std::string &&name)
{
	for (auto &blue_print : _blue_prints) {
		if (mode == blue_print.first) {
			return (blue_print.second(id, std::move(name)));
		}
	}
	return (std::unique_ptr<IProgramResources>(nullptr));
}



