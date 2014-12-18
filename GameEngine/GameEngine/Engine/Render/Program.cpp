#include <Render/Program.hh>
#include <assert.h>
#include <Utils/OpenGL.hh>
#include <array>

Program::Program(std::vector<std::shared_ptr<UnitProg>> const &u) :
_unitsProg(u),
_graphicalMemory(nullptr),
_resources_factory(*this)
{
	_create();
}

Program::~Program()
{
	_destroy();
}

Program::Program(Program &&move) :
_programResources(std::move(move._programResources)),
_unitsProg(std::move(move._unitsProg)),
_graphicalMemory(std::move(move._graphicalMemory)),
_resources_factory(*this),
_id(std::move(move._id))
{
	move._id = 0;
}

GLuint Program::getId() const
{
	return (_id);
}

Program const & Program::use() const
{
	static GLint currentProgram = 0;
	if (currentProgram == _id) {
		return (*this);
	}
	currentProgram = _id;
	glUseProgram(_id);
	return (*this);
}

void Program::_create()
{
	_id = glCreateProgram();
	for (auto &element : _unitsProg) {
		glAttachShader(_id, element->getId());
	}
	glLinkProgram(_id);
	_getProgramResources();
}

void Program::_destroy()
{
	if (_id > 0) {
		glDeleteProgram(_id);
	}
}

Key<ProgramResource> & Program::addResource(std::string const &name)
{
	for (size_t index = 0; index < _programResources.size(); ++index) {
		if (name == _programResources[index]->name()) {
			return (Key<ProgramResource>::createKey(index));
		}
	}
	assert(0);
	return (Key<ProgramResource>::createKey(-1));
}

IProgramResources &Program::getResource(Key<ProgramResource> const &key)
{
	return (*_programResources[key.getId()].get());
}

bool Program::has(Key<ProgramResource> const &key)
{
	return (_programResources.size() > key.getId());
}

static size_t const nbr_resources = 22;

static std::array<GLenum, nbr_resources> const available_resources = 
{
	GL_UNIFORM,
	GL_UNIFORM_BLOCK,
	GL_ATOMIC_COUNTER_BUFFER,
	GL_PROGRAM_INPUT,
	GL_PROGRAM_OUTPUT,
	GL_TRANSFORM_FEEDBACK_VARYING,
	GL_TRANSFORM_FEEDBACK_BUFFER,
	GL_BUFFER_VARIABLE,
	GL_SHADER_STORAGE_BLOCK,
	GL_VERTEX_SUBROUTINE,
	GL_GEOMETRY_SUBROUTINE,
	GL_FRAGMENT_SUBROUTINE,
	GL_TESS_CONTROL_SUBROUTINE,
	GL_TESS_EVALUATION_SUBROUTINE,
	GL_COMPUTE_SUBROUTINE,
	GL_VERTEX_SUBROUTINE_UNIFORM,
	GL_GEOMETRY_SUBROUTINE_UNIFORM,
	GL_FRAGMENT_SUBROUTINE_UNIFORM,
	GL_TESS_CONTROL_SUBROUTINE_UNIFORM,
	GL_TESS_EVALUATION_SUBROUTINE_UNIFORM,
	GL_COMPUTE_SUBROUTINE_UNIFORM,
};

void Program::_getProgramResources()
{
	use();
	for (auto &resource : available_resources) {
		GLint nbr_active_resources = 0;
		GLint max_name_lenght = 0;
		glGetProgramInterfaceiv(_id, resource, GL_ACTIVE_RESOURCES, &nbr_active_resources);
		glGetProgramInterfaceiv(_id, resource, GL_MAX_NAME_LENGTH, &max_name_lenght);
		std::string buffer(max_name_lenght, 0);
		for (size_t index = 0; index < nbr_active_resources; ++index) {
			GLint size = 0;
			glGetProgramResourceName(_id, resource, index, max_name_lenght, &size, (GLchar *)buffer.data());
			std::string name(buffer, 0, size);
			GLenum type_prop = GL_NUM_ACTIVE_VARIABLES;
			GLint prop = 0;
			glGetProgramResourceiv(_id, resource, index, 1, &type_prop, 1, NULL, &prop);
			std::cout << "id = " << index << ", name = " << name << ", num active inside = " << prop << std::endl;
			
		}
	}
}

Program const & Program::print_resources() const
{
	for (auto &resource : _programResources) {
		std::cout << resource->name() << std::endl;
	}
	return (*this);
}




