#pragma once

#include <Utils/OpenGL.hh>
#include <Render/Key.hh>
#include <utility>
#include <stdint.h>
#include <vector>
#include <memory>
#include <Render/Properties/IProperty.hh>

namespace AGE
{
	class BlockMemory;

	class Vertices
	{
	public:
		Vertices(std::vector<std::pair<GLenum, std::string>> const &types, size_t nbrVertex, size_t nbrIndices, size_t offset);
		Vertices(Vertices const &copy);
		Vertices(Vertices &&move);

	public:
		size_t nbr_indices() const;
		size_t nbr_vertex() const;
		size_t nbr_buffer() const;
		std::pair<GLenum, std::string> get_type(size_t index) const;
		std::vector<uint8_t> &&transfer_data(std::string const &attribute);
		std::vector<uint8_t> &&transfer_indices_data();
		template <typename type_t> type_t const *get_data(size_t index, size_t &size) const;
		template <typename type_t> bool set_data(std::vector<type_t> const &data, std::string const &attribute);
		unsigned int const *get_indices(size_t &size) const;
		Vertices &set_indices(std::vector<unsigned int> const &data);
		Vertices &set_block_memory(std::shared_ptr<BlockMemory> const &blockMemory, std::string const &attribute);
		Vertices &set_indices_block_memory(std::shared_ptr<BlockMemory> const &blockMemory);
		Vertices &remove();
		Vertices &reset(size_t o);
		Vertices &draw(GLenum mode);

	private:
		size_t _offset;
		size_t _nbr_indices;
		size_t _nbr_vertex;
		std::vector<std::pair<GLenum, std::string>> _types;
		std::vector<std::pair<std::string, std::vector<uint8_t>>> _data;
		std::vector<uint8_t> _indices_data;
		std::vector<std::pair<std::string, std::weak_ptr<BlockMemory>>> _block_memories;
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
	bool Vertices::set_data(std::vector<type_t> const &data, std::string const &attribute)
	{
		std::vector<uint8_t> tmp(data.size() * sizeof(type_t));
		std::memcpy(tmp.data(), data.data(), tmp.size());
		for (auto &block_memory : _block_memories) {
			if (block_memory.first == attribute) {
				if (block_memory.second.lock()) {
					*block_memory.second.lock() = tmp;
					return true;
				}
			}
		}
		for (auto &data : _data) {
			if (data.first == attribute) {
				data.second = tmp;
				return true;
			}
		}
		return false;
	}

}