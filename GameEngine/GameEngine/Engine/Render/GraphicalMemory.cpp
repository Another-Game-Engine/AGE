#include <Render/GraphicalMemory.hh>
#include <Render/Key.hh>
#include <Render/Vertices.hh>
#include <Render/Data.hh>
#include <tuple>

GraphicalMemory::GraphicalMemory()
{
}

/**
* Method:    load
* FullName:  GraphicalMemory::load
* Access:    public 
* Returns:   GraphicalMemory &
* Qualifier:
* Parameter: std::vector<Attribute> const & attributes
* Goal:		 load element the good attribute adapted for the shader targeted
*/
GraphicalMemory &GraphicalMemory::load(std::vector<Attribute> const &attributes)
{
	_blocksMemory.resize(attributes.size());
	for (auto index = 0; index < attributes.size(); ++index) {
		_blocksMemory[index] = AttributeBlockMemory(attributes[index]);
	}
	return (*this);
}

/**
* Method:    handle
* FullName:  GraphicalMemory::handle
* Access:    public 
* Returns:   Key<Vertices>
* Qualifier:
* Parameter: Vertices const & vertices
* Goal:		 Handle a new vertices inside the graphical memory
*/
Key<Vertices> GraphicalMemory::handle(Vertices const &vertices)
{
	assert(_is_match(vertices));
	size_t offsetVertices = 0;
	if (!_elements.empty()) {
		offsetVertices = _elements.back().offset() + _elements.back().size();
	}
	_elements.emplace_back(vertices);
	_elements.back().offset(offsetVertices);
	size_t index = 0;
	for (auto &ptrData : vertices) {
		_blocksMemory[index].handle(ptrData);
	}
	return (Key<Vertices>::createKey(_elements.size() - 1));
}

/**
* Method:    handle
* FullName:  GraphicalMemory::handle
* Access:    public 
* Returns:   Key<Vertices>
* Qualifier:
* Parameter: Vertices & & vertices
* Goal:		 Handle a new vertices inside the graphical memory
*/
Key<Vertices> GraphicalMemory::handle(Vertices &&vertices)
{
	std::vector<int> toto;
	assert(_is_match(vertices));
	size_t offsetVertices = 0;
	if (!_elements.empty()) {
		offsetVertices = _elements.back().offset() + _elements.back().size();
	}
	_elements.emplace_back(std::move(vertices));
	_elements.back().offset(offsetVertices);
	size_t index = 0;
	for (auto &ptrData : vertices) {
		_blocksMemory[index].handle(ptrData);
	}
	return (Key<Vertices>::createKey(_elements.size() - 1));
}

/**
* Method:    unhandle
* FullName:  GraphicalMemory::unhandle
* Access:    public 
* Returns:   GraphicalMemory &
* Qualifier:
* Parameter: Key<Vertices> & memory
* Goal:		 unhandle an vertices from the graphic memory
*/
GraphicalMemory & GraphicalMemory::unhandle(Key<Vertices> &memory)
{
	_elements.erase(_elements.begin() + memory.getId());
	memory.destroy();
	return (*this);
}

/**
* Method:    _is_match
* FullName:  GraphicalMemory::_is_match
* Access:    private 
* Returns:   bool
* Qualifier: const
* Parameter: Vertices const & vertices
* Goal:		 check if the data inside the vertices match 
*			 with the attribute block inside the graphic memory
*/
bool GraphicalMemory::_is_match(Vertices const &vertices) const
{
	size_t nbrData = 0;
	for (auto &ptrData : vertices) {
		++nbrData;
		for (auto &block : _blocksMemory) {
			auto &data = *ptrData;
			if (block != data) {
				return (false);
			}
		}
	}
	if (nbrData != _blocksMemory.size()) {
		return (false);
	}
	return (true);
}

/**
* Method:    draw
* FullName:  GraphicalMemory::draw
* Access:    public 
* Returns:   GraphicalMemory &
* Qualifier:
* Parameter: GLenum mode
* Parameter: Key<Vertices> & memory
* Goal:		 draw the vertices targeted
*/
GraphicalMemory & GraphicalMemory::draw(GLenum mode, Key<Vertices> &memory)
{
	assert(memory);
	auto &vertices = _elements[memory.getId()];
	if (_is_has_indices()) {
		glDrawElementsBaseVertex(mode, vertices.size(), GL_UNSIGNED_INT, (GLvoid const *)(vertices.offset() * sizeof(int)), vertices.offset());
		return (*this);
	}
	glDrawArrays(mode, vertices.offset(), vertices.size());
	return (*this);
}

/**
* Method:    _is_has_indices
* FullName:  GraphicalMemory::_is_has_indices
* Access:    private 
* Returns:   bool
* Qualifier: const
* Goal:		 check if the graphical memory has a block for the indices
*/
bool GraphicalMemory::_is_has_indices() const
{
	for (auto &block : _blocksMemory) {
		if (block == Attribute::Indices) {
			return (true);
		}
	}
	return (false);
}

/**
* Method:    bind
* FullName:  GraphicalMemory::bind
* Access:    public 
* Returns:   GraphicalMemory &
* Qualifier:
* Goal:		 bind all attribute blocks
*/
GraphicalMemory & GraphicalMemory::bind()
{
	size_t index = 0;
	for (auto &block : _blocksMemory) {
		block.bind();
		glVertexAttribPointer(index++, std::get<Property::NbrComponent>(attProperty[block]), 
			std::get<Property::GLType>(attProperty[block]), GL_FALSE, 0, 0);
	}
	return (*this);
}

/**
* Method:    update
* FullName:  GraphicalMemory::update
* Access:    public 
* Returns:   GraphicalMemory &
* Qualifier:
* Goal:		 update all attribute blocks
*/
GraphicalMemory & GraphicalMemory::update()
{
	for (auto &block : _blocksMemory) {
		block.update();
	}
	return (*this);
}

