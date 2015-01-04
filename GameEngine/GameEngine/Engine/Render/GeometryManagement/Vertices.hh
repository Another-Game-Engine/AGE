#pragma once

# include <Utils/OpenGL.hh>
# include <stdint.h>
# include <vector>
# include <memory>

class BlockMemory;

class Vertices
{
public:
	Vertices(std::vector<GLenum> &&types, size_t nbrVertex);
	Vertices(Vertices const &copy);
	Vertices(Vertices &&move);

public:
	size_t nbr_vertex() const;
	size_t nbr_buffer() const;
	GLenum get_type(size_t index) const;
	std::vector<uint8_t> &&transfer_data(size_t index);
	Vertices &set_data(std::vector<uint8_t> const &data, size_t index);
	Vertices &set_block_memory(std::shared_ptr<BlockMemory> const &blockMemory, size_t index);

private:
	size_t _nbr_vertex;
	std::vector<GLenum> _types;
	std::vector<std::vector<uint8_t>> _data;
	std::vector<std::weak_ptr<BlockMemory>> _block_memories;
};