#include <Render/Program.hh>
#include <assert.h>
#include <Utils/OpenGL.hh>
#include <Render/ProgramResources/Types/ProgramResourcesType.hh>
#include <Render/GeometryManagement/Buffer/BufferPrograms.hh>
#include <Render/ProgramResources/Types/Attribute.hh>
#include <Utils/Profiler.hpp>
#include <Render/Properties/Properties.hh>

namespace AGE
{
	std::size_t Program::_ageIdCounter = 0;

	Program::Program(std::string &&name, std::vector<std::shared_ptr<UnitProg>> const &u) :
		_unitsProg(u),
		_resources_factory(*this),
		_name(std::move(name)),
		_compiled(false),
		_id(0)
	{
#ifdef AGE_DEBUG
		_version = 0;
#endif
		_ageId = -1;
	}

	Program::~Program()
	{

	}

	void Program::destroy()
	{
		if (_id > 0)
		{
			glDeleteProgram(_id);
			_id = 0;
		}
		_program_resources.clear();
		_resources_factory.reset();
	}

	Program::Program(Program &&move) :
		_program_resources(std::move(move._program_resources)),
		_unitsProg(std::move(move._unitsProg)),
		_resources_factory(*this),
		_id(move._id),
		_name(std::move(move._name))
	{
		move._id = 0;
		_ageId = move._ageId;
	}

	GLuint Program::id() const
	{
		return (_id);
	}

	Program const & Program::use() const
	{
		glUseProgram(_id);
		return (*this);
	}

	Key<ProgramResource> & Program::get_key(std::string const &name)
	{
		for (auto index = 0; index < _program_resources.size(); ++index) {
			if (name == _program_resources[index]->name()) {
				return (Key<ProgramResource>::createKey(index));
			}
		}
		return (Key<ProgramResource>::createKey(-1));
	}

	bool Program::has_resource(Key<ProgramResource> const &key)
	{
		return (_program_resources.size() > key.getId());
	}

	void Program::_get_resource(size_t index, GLenum resource, std::string const & buffer)
	{
		auto size = 0;
		glGetProgramResourceName(_id, resource, GLuint(index), GLsizei(buffer.size()), (GLsizei *)&size, (GLchar *)buffer.data());
		std::string name(buffer, 0, size);
		// disgusting way to detect arrays
		bool isArray = size > 0 && (name[size - 1] == ']');
		if (isArray)
		{
			std::string nameWithoutBacketDisgusting;
			for (auto &c : name)
			{
				if (c == '[')
					break;
				nameWithoutBacketDisgusting += c;
			}
			name = nameWithoutBacketDisgusting;
		}
		auto element = _resources_factory.build(resource, (GLint)index, std::move(name), isArray);
		if (element) {
			_program_resources.emplace_back(std::move(element));
		}
		else
		{
			int i = 0;
		}
	}

	void Program::_get_resources()
	{
		use();
		for (auto &resource : available_resources)
		{
			auto nbr_active_resources = 0;
			auto max_name_lenght = 0;
			glGetProgramInterfaceiv(_id, resource, GL_ACTIVE_RESOURCES, &nbr_active_resources);
			if (nbr_active_resources == 0) {
				continue;
			}
			glGetProgramInterfaceiv(_id, resource, GL_MAX_NAME_LENGTH, &max_name_lenght);
			auto buffer = std::string(max_name_lenght, 0);
			for (size_t index = 0; index < nbr_active_resources; ++index)
			{
				_get_resource(index, resource, buffer);
			}
		}
	}

	Program const & Program::print_resources() const
	{
		for (auto &resource : _program_resources) {
			resource->print();
		}
		return (*this);
	}


	Program & Program::update()
	{
		//@PROFILER_COMMENTED
		//SCOPE_profile_cpu_function("RenderTimer");

		use();
		for (auto &resource : _program_resources)
		{
			resource->update();
		}
		return (*this);
	}

	size_t Program::nbr_resources() const
	{
		return (_program_resources.size());
	}

	Program &Program::set_attributes(BufferPrograms const &buffers)
	{

		SCOPE_profile_cpu_function("RenderTimer");

		for (auto &buffer : buffers.get_buffers())
		{
			auto resource = Program::get_resource<Attribute>(buffer->name());
			if (resource.isValid())
			{
				resource.set(buffer);
			}
		}
		return *this;
	}

	bool Program::coherent_attributes(std::vector<std::pair<GLenum, std::string>> const &coherent)
	{
		for (auto &resource : _program_resources) {
			if (resource->type() == GL_PROGRAM_INPUT) {
				bool attribFound = false;

				for (auto &attrib : coherent) {
					if (*static_cast<Attribute *>(resource.get()) == attrib)
					{
						attribFound = true;
						break;
					}
				}

				if (attribFound == false)
					return (false);
			}
		}
		return (true);
	}

	bool Program::compile()
	{
		_compiled = false;
		destroy();
		_id = glCreateProgram();

		if (_id == 0)
		{
			std::cerr << "Error glCreateProgram()\n";
			return false;
		}

		for (auto &element : _unitsProg)
		{
			element->destroy();
			auto success = element->compile();
			if (!success)
			{
				return false;
			}
			glAttachShader(_id, element->getId());
		}
		glLinkProgram(_id);

		GLint isLinked = 0;
		glGetProgramiv(_id, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(_id, maxLength, &maxLength, &infoLog[0]);
			std::cerr << infoLog.data() << std::endl;
			return false;
		}

		_get_resources();
		_compiled = true;
#ifdef AGE_DEBUG
		++_version;
#endif
		if (_ageId == -1)
		{
			_ageId = _ageIdCounter++;
		}
		return true;
	}

	std::string const & Program::name() const
	{
		return (_name);
	}

	bool Program::operator==(Program const &p) const
	{
		if (_id == p._id) {
			return (true);
		}
		return (false);
	}

	bool Program::operator!=(Program const &p) const
	{
		return (!(*this == p));
	}

	std::shared_ptr<IProgramResources> Program::get_resource_interface(std::string const &name)
	{
		for (size_t index = 0; index < _program_resources.size(); ++index) {
			if (name == _program_resources[index]->name()) {
				return (_program_resources[index]);
			}
		}
		return (nullptr);
	}

	void Program::registerProperties(Properties &properties)
	{
		SCOPE_profile_cpu_function("RenderTimer");

		// if properties is empty we return
		if (properties.empty())
			return;

		std::size_t id = properties.getProgramId(_ageId);
		std::size_t hash = properties.getHash();

		// if the properties is already registered
		// and his hash didn't change
		// we return
		if (id != -1 && _propertiesRegister.size() != 0 && _propertiesRegister[id].propertiesHash == hash)
			return;

		PropertiesRegister *propRegister = nullptr;

		id = 0;
		// we search for the hash
		for (auto &reg : _propertiesRegister)
		{
			if (reg.propertiesHash == hash)
			{
				propRegister = &reg;
				break;
			}
			++id;
		}

		// if the hash never have been registered
		if (propRegister == nullptr)
		{
			// we register the hash
			_propertiesRegister.resize(_propertiesRegister.size() + 1);
			id = _propertiesRegister.size() - 1;
			propRegister = &_propertiesRegister[id];
			propRegister->propertiesHash = hash;
			// we register properties index
			for (auto &r : _program_resources)
			{
				if (r->isInstancied())
				{
					auto &alias = r->getInstanciedAlias();
					for (auto &a : alias)
					{
						std::shared_ptr<IProperty> p = properties.getProperty(a);
						if (p != nullptr)
						{
							auto index = properties.getPropertyIndex(a);
							propRegister->propertyIndex.push_back(PropertyRegister(index, r, p->getInstanciedUpdateFunction(), true));
						}
					}
				}
				else
				{
					std::shared_ptr<IProperty> p = properties.getProperty(r->name());
					if (p)
					{
						auto index = properties.getPropertyIndex(r->name());
						propRegister->propertyIndex.push_back(PropertyRegister(index, r, p->getUpdateFunction(), false));
					}
				}
			}
		}
		// we give the id the the properties
		properties.setProgramId(_ageId, id);
	}

	void Program::updateProperties(Properties &properties)
	{
		//@PROFILER_COMMENTED
		//SCOPE_profile_cpu_function("RenderTimer");

		std::size_t id = properties.getProgramId(_ageId);
		if (id == -1)
		{
			return;
		}
		PropertiesRegister *propRegister = &_propertiesRegister[id];

		for (auto &i : propRegister->propertyIndex)
		{
			i.updateFunction(i.resource.get(), properties.getProperty(i.index).get());
		}
	}

	void Program::updateInstanciedProperties(Properties &properties)
	{
		SCOPE_profile_cpu_function("RenderTimer");

		std::size_t id = properties.getProgramId(_ageId);
		if (id == -1)
		{
			return;
		}
		PropertiesRegister *propRegister = &_propertiesRegister[id];

		for (auto &i : propRegister->propertyIndex)
		{
			if (i.instancied)
			{
				i.updateFunction(i.resource.get(), properties.getProperty(i.index).get());
			}
		}
	}

	void Program::updateNonInstanciedProperties(Properties &properties)
	{
		SCOPE_profile_cpu_function("RenderTimer");

		std::size_t id = properties.getProgramId(_ageId);
		if (id == -1)
		{
			return;
		}
		PropertiesRegister *propRegister = &_propertiesRegister[id];

		for (auto &i : propRegister->propertyIndex)
		{
			if (i.instancied == false)
			{
				i.updateFunction(i.resource.get(), properties.getProperty(i.index).get());
			}
		}
	}
}