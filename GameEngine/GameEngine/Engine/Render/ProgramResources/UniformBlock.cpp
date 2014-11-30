#include <Render/ProgramResources/UniformBlock.hh>
#include <Render/ProgramResources/BlockResourcesFactory.hh>
#include <Render/Program.hh>

UniformBlock::UniformBlock(Program const &parent, std::string const &name) :
AProgramResources(parent, name, GL_UNIFORM_BLOCK),
_size_block(0)
{
	introspection(parent);
}

UniformBlock::UniformBlock(Program const &parent, std::string &&name) :
AProgramResources(parent, std::move(name), GL_UNIFORM_BLOCK),
_size_block(0)
{
	introspection(parent);
}

UniformBlock::UniformBlock(UniformBlock &&move) :
AProgramResources(std::move(move)),
_size_block(move._size_block)
{

}

/**
* Method:    operator()
* FullName:  UniformBlock::operator()
* Access:    public 
* Returns:   IProgramResources &
* Qualifier:
* Goal:		 synchronized the uniform with GPU
*/
IProgramResources & UniformBlock::operator()()
{
	return (*this);
}

/**
* Method:    introspection
* FullName:  UniformBlock::introspection
* Access:    private 
* Returns:   UniformBlock &
* Qualifier:
* Parameter: Program const & program
* Goal:		 introspect the uniform for allocate the good data
*/
UniformBlock &UniformBlock::introspection(Program const &program)
{
	glGetActiveUniformBlockiv(program.getId(), _id, GL_UNIFORM_BLOCK_DATA_SIZE, &_size_block);
	_buffer.resize(_size_block);
	GLint nbrUniforms;
	glGetActiveUniformBlockiv(program.getId(), _id, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &nbrUniforms);
	_blockResources.resize(nbrUniforms);
	std::vector<GLint> indices(nbrUniforms);
	glGetActiveUniformBlockiv(program.getId(), _id, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices.data());
	for (auto &index : indices) {
		assert(index != -1);
	}
	std::vector<GLint> offsets(nbrUniforms);
	std::vector<GLint> sizes(nbrUniforms);
	std::vector<GLint> types(nbrUniforms);
	std::vector<GLint> strides(nbrUniforms);
	std::vector<GLint> nameLenght(nbrUniforms);
	glGetActiveUniformsiv(program.getId(), nbrUniforms, (const GLuint *)indices.data(), GL_UNIFORM_OFFSET, offsets.data());
	glGetActiveUniformsiv(program.getId(), nbrUniforms, (const GLuint *)indices.data(), GL_UNIFORM_SIZE, sizes.data());
	glGetActiveUniformsiv(program.getId(), nbrUniforms, (const GLuint *)indices.data(), GL_UNIFORM_TYPE, types.data());
	glGetActiveUniformsiv(program.getId(), nbrUniforms, (const GLuint *)indices.data(), GL_UNIFORM_ARRAY_STRIDE, strides.data());
	glGetActiveUniformsiv(program.getId(), nbrUniforms, (const GLuint *)indices.data(), GL_UNIFORM_NAME_LENGTH, nameLenght.data());
	BlockResourcesFactory factory;
	for (GLint index = 0; index < nbrUniforms; ++index) {
		std::string name(nameLenght[index], 0);
		GLsizei nbrCharacters;
		glGetActiveUniformName(program.getId(), indices[index], nameLenght[index],	&nbrCharacters, (GLchar *)name.data());
		_blockResources[index] = factory(types[index], indices[index], std::move(name));
		_blockResources[index]->offset(offsets[index]);
		_blockResources[index]->size_array(sizes[index]);
		_blockResources[index]->stride(strides[index]);
	}
	return (*this);
}
