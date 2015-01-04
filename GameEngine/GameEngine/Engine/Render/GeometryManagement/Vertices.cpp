#include <Render/GeometryManagement/Vertices.hh>
#include <Render/ProgramResources/Types/ProgramResourcesType.hh>
#include <Render/GeometryManagement/BlockMemory.hh>

Vertices::Vertices(std::vector<GLenum> &&types, size_t nbrVertex) :
_nbr_vertex(nbrVertex),
_types(std::move(types)),
_data(_types.size()),
_block_memories(_types.size())
{
	for (auto index = 0ull; index < _types.size(); ++index) {
		auto &iterator = available_types.find(_types[index]);
		if (iterator != available_types.end()) {
			auto &t = iterator->second;
			_data[index].resize(t.size * nbrVertex, 0);
		}
	}
}

Vertices::Vertices(Vertices const &copy) :
_nbr_vertex(copy._nbr_vertex),
_types(copy._types),
_data(copy._data),
_block_memories(copy._block_memories)
{
}

Vertices::Vertices(Vertices &&move) :
_nbr_vertex(move._nbr_vertex),
_types(std::move(move._types)),
_data(std::move(move._data)),
_block_memories(std::move(move._block_memories))
{

}

size_t Vertices::nbr_vertex() const
{
	return (_nbr_vertex);
}

size_t Vertices::nbr_buffer() const
{
	return (_data.size());
}

Vertices &Vertices::set_data(std::vector<uint8_t> const &data, size_t index)
{
	if (_block_memories[index].lock()) {
		*_block_memories[index].lock() = data;
		return (*this);
	}
	if (data.size() == _data[index].size()) {
		return (*this);
	}
	std::memcpy(_data[index].data(), data.data(), _data[index].size());
	return (*this);
}

std::vector<uint8_t> &&Vertices::transfer_data(size_t index)
{
	return (std::move(_data[index]));
}

GLenum Vertices::get_type(size_t index) const
{
	return (_types[index]);
}

Vertices & Vertices::set_block_memory(std::shared_ptr<BlockMemory> const &blockMemory, size_t index)
{
	_block_memories[index] = blockMemory;
	return (*this);
}
