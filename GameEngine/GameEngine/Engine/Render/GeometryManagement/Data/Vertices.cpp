#include <Render/GeometryManagement/Data/Vertices.hh>
#include <Render/ProgramResources/Types/ProgramResourcesType.hh>
#include <Render/GeometryManagement/Data/BlockMemory.hh>
#include <Render/Program.hh>

namespace AGE
{
	Vertices::Vertices(std::vector<std::pair<GLenum, std::string>> const &types, size_t nbrVertex, size_t nbrIndices, size_t offset) :
		_offset(offset),
		_nbr_indices(nbrIndices),
		_nbr_vertex(nbrVertex),
		_types(types),
		_data(_types.size()),
		_indices_data(nbrIndices * sizeof(unsigned int), 0),
		_block_memories(_types.size())
	{
		auto index = 0ull;
		for (auto &type : _types) {
			auto const &iterator = available_types.find(type.first);
			if (iterator != available_types.end()) {
				auto const &available_type = iterator->second;
				_data[index].first = type.second;
				_block_memories[index].first = type.second;
				_data[index].second.resize(available_type.size * nbrVertex, 0);
			}
			++index;
		}
	}

	Vertices::Vertices(Vertices const &copy) :
		_offset(copy._offset),
		_nbr_indices(copy._nbr_indices),
		_nbr_vertex(copy._nbr_vertex),
		_types(copy._types),
		_data(copy._data),
		_indices_data(copy._indices_data),
		_block_memories(copy._block_memories),
		_indices_block_memory(copy._indices_block_memory),
		_properties(copy._properties)
	{
	}

	Vertices::Vertices(Vertices &&move) :
		_offset(move._offset),
		_nbr_indices(move._nbr_indices),
		_nbr_vertex(move._nbr_vertex),
		_types(std::move(move._types)),
		_data(std::move(move._data)),
		_indices_data(std::move(move._indices_data)),
		_block_memories(std::move(move._block_memories)),
		_indices_block_memory(std::move(move._indices_block_memory)),
		_properties(std::move(move._properties))
	{

	}

	size_t Vertices::nbr_indices() const
	{
		return (_nbr_indices);
	}

	size_t Vertices::nbr_vertex() const
	{
		return (_nbr_vertex);
	}

	size_t Vertices::nbr_buffer() const
	{
		return (_data.size());
	}

	std::vector<uint8_t> &&Vertices::transfer_data(std::string const &attribute)
	{
		for (auto &data : _data) {
			if (data.first == attribute) {
				return std::move(data.second);
			}
		}
		return std::move(_data.begin()->second);
	}

	std::vector<uint8_t> &&Vertices::transfer_indices_data()
	{
		return (std::move(_indices_data));
	}

	std::pair<GLenum, std::string> Vertices::get_type(size_t index) const
	{
		return (_types[index]);
	}

	Vertices & Vertices::set_block_memory(std::shared_ptr<BlockMemory> const &blockMemory, std::string const &attribute)
	{
		for (auto &block_memory : _block_memories) {
			if (block_memory.first == attribute) {
				block_memory.second = blockMemory;
				return *this;
			}
		}
		return (*this);
	}

	Vertices &Vertices::set_indices_block_memory(std::shared_ptr<BlockMemory> const &blockMemory)
	{
		_indices_block_memory = blockMemory;
		return (*this);
	}

	Vertices & Vertices::remove()
	{
		auto &data = _data.begin();
		for (auto &block : _block_memories) {
			if (block.second.lock()) {
				block.second.lock()->remove();
			}
			else {
				data->second.clear();
			}
			++data;
		}
		if (_indices_block_memory.lock()) {
			_indices_block_memory.lock()->remove();
		}
		else {
			_indices_data.clear();
		}
		_nbr_vertex = 0;
		_nbr_indices = 0;
		return (*this);
	}

	Vertices & Vertices::set_indices(std::vector<unsigned int> const &data)
	{
		std::vector<uint8_t> tmp(data.size() * sizeof(unsigned int));
		std::memcpy(tmp.data(), data.data(), tmp.size());
		if (_indices_block_memory.lock()) {
			*_indices_block_memory.lock() = tmp;
			return (*this);
		}
		if (tmp.size() != _indices_data.size()) {
			return (*this);
		}
		_indices_data = tmp;
		return (*this);
	}

	Vertices & Vertices::draw(GLenum mode)
	{
		if (_indices_block_memory.lock()) {
			auto offset = _indices_block_memory.lock()->offset();
			glDrawElementsBaseVertex(mode, GLsizei(_nbr_indices), GL_UNSIGNED_INT, (GLvoid *)offset, GLint(_offset));
		}
		else {
			glDrawArrays(mode, _offset, _nbr_vertex);
		}
		return (*this);
	}

	unsigned int const * Vertices::get_indices(size_t &size) const
	{
		if (_indices_block_memory.lock()) {
			auto &to_show = _indices_block_memory.lock()->get();
			size = to_show.size() / sizeof(unsigned int);
			return ((unsigned int const *)to_show.data());
		}
		else {
			size = _indices_data.size() / sizeof(unsigned int);
			return ((unsigned int const *)_indices_data.data());
		}
	}

	Vertices & Vertices::reset(size_t o)
	{
		_offset = o;
		return (*this);
	}

}
