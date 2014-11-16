#pragma once

# include <vector>
# include <utility>
# include <Utils/Singleton.hh>

template <typename type_t> class Key;
enum class Attribute;
class Vertices;
class AttributeBlockMemory;

class GraphicalMemory : public Singleton<GraphicalMemory>
{
public:
	Key<GraphicalMemory> addMemory(Attribute attribute);
	GraphicalMemory &deleteMemory(Key<GraphicalMemory> const &memory);
	GraphicalMemory &setMemory(Key<GraphicalMemory> const &memory);

private:
	std::vector<Vertices> _elements;
	std::vector<AttributeBlockMemory> _blocksMemory;
};