#include <Render/ProgramResources/Factory/ProgramResourcesFactory.hh>
#include <Render/ProgramResources/IProgramResources.hh>
#include <Render/ProgramResources/Types/UniformBlock.hh>
#include <Render/ProgramResources/Types/Attribute.hh>
#include <Render/ProgramResources/Types/ProgramResourcesType.hh>
#include <Render/Program.hh>
#include <array>

namespace AGE
{
	ProgramResourcesFactory::ProgramResourcesFactory(Program const &program) :
		_program(program),
		_blue_prints({ _create_uniform(), _create_uniform_block(), _create_input()})
	{

	}

	std::shared_ptr<IProgramResources> ProgramResourcesFactory::build(GLenum mode, GLint id, std::string &&name)
	{
		for (auto &blue_print : _blue_prints) {
			if (mode == blue_print.first) {
				return (blue_print.second(id, std::move(name)));
			}
		}
		return (std::unique_ptr<IProgramResources>(nullptr));
	}

	ProgramResourcesFactory::create_type_t ProgramResourcesFactory::_create_uniform()
	{
		return (std::make_pair(GL_UNIFORM, LAMBDA_PROTO {
			auto const nbr_prop = 5;
			std::array<GLenum, nbr_prop> const prop = { GL_TYPE, GL_BLOCK_INDEX, GL_OFFSET, GL_ARRAY_SIZE, GL_ARRAY_STRIDE };
			std::array<GLint, nbr_prop> params;
			glGetProgramResourceiv(_program.id(), GL_UNIFORM, id, nbr_prop, prop.data(), nbr_prop, nullptr, params.data());
			if (params[1] == -1) {
				auto location = glGetUniformLocation(_program.id(), name.c_str());
				if (name.find("[0]") != std::string::npos) {
					std::cout << name << std::endl;
					return (_uniformsFactory.build_array(params[0], params[3], params[4], location, std::move(name)));
				}
				else {
					return (_uniformsFactory.build(params[0], location, std::move(name)));
				}
			}
			_block_resources.emplace_back(std::make_unique<BlockResources>(id, std::move(name), params[0], glm::vec3(params[2], params[3], params[4])));
			return (std::shared_ptr<IProgramResources>(nullptr));
		}));
	}

	ProgramResourcesFactory::create_type_t ProgramResourcesFactory::_create_uniform_block()
	{
		return std::make_pair(GL_UNIFORM_BLOCK, LAMBDA_PROTO{
			auto const nbr_prop = 2;
			std::array<GLenum, nbr_prop> const prop = { GL_BUFFER_DATA_SIZE, GL_NUM_ACTIVE_VARIABLES };
			std::array<GLint, nbr_prop> params;
			glGetProgramResourceiv(_program.id(), GL_UNIFORM_BLOCK, id, nbr_prop, prop.data(), nbr_prop, nullptr, params.data());
			std::vector<std::unique_ptr<BlockResources>> block_resources;
			block_resources.reserve(params[1]);
			GLenum const active_variable_prop = GL_ACTIVE_VARIABLES;
			std::vector<GLint> active_variables(params[1]);
			glGetProgramResourceiv(_program.id(), GL_UNIFORM_BLOCK, id, 1, &active_variable_prop, params[1], nullptr, active_variables.data());
			for (auto index = 0ull; index < params[1]; ++index) {
				for (auto &resource : _block_resources) {
					if (resource != nullptr && resource->id() == active_variables[index]) {
						block_resources.push_back(std::move(resource));
					}
				}
			}
			auto tmp = std::make_shared<UniformBlock>(id, std::move(name), std::move(block_resources), params[0]);
			return (std::static_pointer_cast<IProgramResources>(tmp));
		});
	}

	ProgramResourcesFactory::create_type_t ProgramResourcesFactory::_create_input()
	{
		return std::make_pair(GL_PROGRAM_INPUT, LAMBDA_PROTO{ \
			auto const nbr_prop = 1;
		std::array<GLenum, nbr_prop> const prop = { GL_TYPE };
		std::array<GLint, nbr_prop> params;
		glGetProgramResourceiv(_program.id(), GL_PROGRAM_INPUT, id, nbr_prop, prop.data(), nbr_prop, nullptr, params.data());
		auto &available_type = available_types.find(params[0]);
		if (available_type == available_types.end()) {
			return (std::shared_ptr<IProgramResources>(nullptr));
		}
		auto tmp = std::make_shared<Attribute>(id, glGetAttribLocation(_program.id(), name.c_str()), std::move(name), available_type->second);																									  \
			return (std::static_pointer_cast<IProgramResources>(tmp));
		});
	}

}