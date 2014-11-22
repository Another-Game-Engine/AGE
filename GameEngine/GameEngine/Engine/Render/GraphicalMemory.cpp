#include <Render/GraphicalMemory.hh>
#include <Render/Key.hh>
#include <Render/Vertices.hh>
#include <Render/Data.hh>

GraphicalMemory::GraphicalMemory()
{
}

GraphicalMemory &GraphicalMemory::load(std::vector<Attribute> const &attributes)
{
	_blocksMemory.resize(attributes.size());
	for (auto index = 0; index < attributes.size(); ++index) {
		_blocksMemory[index] = AttributeBlockMemory(attributes[index]);
	}
	return (*this);
}

Key<Vertices> GraphicalMemory::handle(Vertices const &vertices)
{
	assert(isMatch(vertices));
	_elements.emplace_back(vertices);
	size_t index = 0;
	for (auto &ptrData : vertices) {
		_blocksMemory[index].handle(ptrData);
	}
	return ()
}

Key<Vertices> GraphicalMemory::handle(Vertices &&vertices)
{
	assert(isMatch(vertices));
	for (auto &ptrData : vertices) {
		for (auto &block : _blocksMemory) {
			auto &data = *ptrData;
			if (block == data) {
				block.handle(ptrData);
				break;
			}
		}
	}
	_elements.emplace_back(std::move(vertices));
	return (Key<Vertices>::createKeyWithId(_elements.size() - 1));
}

GraphicalMemory & GraphicalMemory::quit(Key<Vertices> const &memory)
{
	_elements.erase(_elements.begin() + memory.getId());
	return (*this);
}

bool GraphicalMemory::isMatch(Vertices const &vertices) const
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