#pragma once

# include <Utils/OpenGL.hh>
# include <Render/Key.hh>
# include <stdint.h>
# include <vector>
# include <memory>
# include <Render/Properties/IProperty.hh>

class BlockMemory;

class Vertices
{
public:
	Vertices(std::vector<GLenum> const &types, size_t nbrVertex, size_t nbrIndices, size_t offset);
	Vertices(Vertices const &copy);
	Vertices(Vertices &&move);

public:
	size_t nbr_indices() const;
	size_t nbr_vertex() const;
	size_t nbr_buffer() const;
	GLenum get_type(size_t index) const;
	std::vector<uint8_t> &&transfer_data(size_t index);
	std::vector<uint8_t> &&transfer_indices_data();
	template <typename type_t> type_t const *get_data(size_t index, size_t &size) const;
	template <typename type_t> Vertices &set_data(std::vector<type_t> const &data, size_t index);
	unsigned int const *get_indices(size_t &size) const;
	Vertices &set_indices(std::vector<unsigned int> const &data);
	Vertices &set_block_memory(std::shared_ptr<BlockMemory> const &blockMemory, size_t index);
	Vertices &set_indices_block_memory(std::shared_ptr<BlockMemory> const &blockMemory);
	Vertices &remove();
	Vertices &reset(size_t o);
	Vertices &draw(GLenum mode);
	Key<Property> add_property(std::shared_ptr<IProperty> const &prop);
	Vertices &remove_property(Key<Property> &key);
	std::shared_ptr<IProperty> get_property(Key<Property> const &p) const;
	Vertices &update(std::shared_ptr<Program> const &program);

private:
	size_t _index;
	size_t _offset;
	size_t _nbr_indices;
	size_t _nbr_vertex;
	std::vector<GLenum> _types;
	std::vector<std::vector<uint8_t>> _data;
	std::vector<uint8_t> _indices_data;
	std::vector<std::weak_ptr<BlockMemory>> _block_memories;
	std::weak_ptr<BlockMemory> _indices_block_memory;
	std::vector<std::shared_ptr<IProperty>> _properties;
};

template <typename type_t> 
type_t const *Vertices::get_data(size_t index, size_t &size) const
{
	if (_block_memories[index]->lock()) {
		auto &to_show = _block_memories[index]->lock()->get();
		size = to_show.size() / sizeof(type_t);
		return ((type_t const *)to_show.data());
	}
	else {
		size = _data[index].size() / sizeof(type_t);
		return ((type_t const *)_data[index]);
	}
}

template <typename type_t>
Vertices &Vertices::set_data(std::vector<type_t> const &data, size_t index)
{
	std::vector<uint8_t> tmp(data.size() * sizeof(type_t));
	std::memcpy(tmp.data(), data.data(), tmp.size());
	if (_block_memories[index].lock()) {
		*_block_memories[index].lock() = tmp;
		return (*this);
	}
	if (tmp.size() != _data[index].size()) {
		return (*this);
	}
	_data[index] = tmp;
	return (*this);
}