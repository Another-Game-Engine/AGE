#include <Render/GeometryManagement/GraphicalMemory.hh>
#include <Render/Key.hh>
#include <Render/GeometryManagement/Vertices.hh>
#include <Render/GeometryManagement/Data.hh>
#include <tuple>

GraphicalMemory::GraphicalMemory()
{
}

GraphicalMemory::GraphicalMemory(GraphicalMemory &&move) :
_array_buffer(std::move(move._array_buffer)),
_elements(std::move(move._elements)),
_blocksMemory(std::move(move._blocksMemory))
{

}

GraphicalMemory &GraphicalMemory::init(std::vector<Attribute> const &attributes)
{
	_blocksMemory.resize(attributes.size());
	for (auto index = 0; index < attributes.size(); ++index) {
		_blocksMemory[index] = AttributeBlockMemory(attributes[index]);
	}
	return (*this);
}

Key<Vertices> GraphicalMemory::add(Vertices const &vertices)
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
		_blocksMemory[index].add(ptrData);
	}
	return (Key<Vertices>::createKey(_elements.size() - 1));
}

Key<Vertices> GraphicalMemory::add(Vertices &&vertices)
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
		_blocksMemory[index].add(ptrData);
	}
	return (Key<Vertices>::createKey(_elements.size() - 1));
}

GraphicalMemory & GraphicalMemory::remove(Key<Vertices> &memory)
{
	_elements.erase(_elements.begin() + memory.getId());
	memory.destroy();
	return (*this);
}

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

bool GraphicalMemory::_is_has_indices() const
{
	for (auto &block : _blocksMemory) {
		if (block == Attribute::Indices) {
			return (true);
		}
	}
	return (false);
}

GraphicalMemory & GraphicalMemory::bind()
{
	size_t index = 0;
	for (auto &block : _blocksMemory) {
		_array_buffer.attribute(index++, block, block.buffer());
	}
	return (*this);
}

GraphicalMemory & GraphicalMemory::update()
{
	for (auto &block : _blocksMemory) {
		block.update();
	}
	return (*this);
}

